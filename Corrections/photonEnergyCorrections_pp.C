#include "TFile.h"
#include "TTree.h"
#include "TStyle.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TLegend.h"

#include "TString.h"

#include <vector>

#include "../TreeHeaders/CutConfigurationTree.h"
#include "../Utilities/interface/CutConfigurationParser.h"

#define _SET_BRANCH_VEC(tree, type, branch)     \
    std::vector<type>* branch = 0;              \
    tree->SetBranchStatus(#branch, 1);          \
    tree->SetBranchAddress(#branch, &branch);   \

#define _SET_BRANCH_VAR(tree, type, branch)     \
    type branch;                                \
    tree->SetBranchStatus(#branch, 1);          \
    tree->SetBranchAddress(#branch, &branch);   \

float dR(float eta1, float eta2, float phi1, float phi2);

int photonEnergyCorrections_pp(const TString configFile = "CutConfigurations/gammaJet_pp.conf",
                            const char* fn_Zee = "/mnt/hadoop/cms/store/user/tatar/official/Pythia8_Z30eeJet/HINppWinter16DR-75X_mcRun2_asymptotic_ppAt5TeV_v3_ext1-v1-FOREST/0.root") {
    TFile* f_Zee = new TFile(fn_Zee, "read");
    TTree* t_photon = (TTree*)f_Zee->Get("ggHiNtuplizer/EventTree");
    TTree* t_event = (TTree*)f_Zee->Get("hiEvtAnalyzer/HiTree");

    t_photon->SetBranchStatus("*", 0);
    t_event->SetBranchStatus("*", 0);

    _SET_BRANCH_VEC(t_photon, float, mcEt);
    _SET_BRANCH_VEC(t_photon, float, mcEta);
    _SET_BRANCH_VEC(t_photon, float, mcPhi);
    _SET_BRANCH_VEC(t_photon, float, mcCalIsoDR04);
    _SET_BRANCH_VEC(t_photon, float, phoEt);
    _SET_BRANCH_VEC(t_photon, float, phoEta);
    _SET_BRANCH_VEC(t_photon, float, phoPhi);
    _SET_BRANCH_VEC(t_photon, float, phoHoverE);
    _SET_BRANCH_VEC(t_photon, float, pho_ecalClusterIsoR4);
    _SET_BRANCH_VEC(t_photon, float, pho_hcalRechitIsoR4);
    _SET_BRANCH_VEC(t_photon, float, pho_trackIsoR4PtCut20);
    _SET_BRANCH_VEC(t_photon, float, phoSigmaIEtaIEta_2012);
    _SET_BRANCH_VEC(t_photon, float, pho_swissCrx);
    _SET_BRANCH_VEC(t_photon, float, pho_seedTime);
    _SET_BRANCH_VEC(t_photon, int, pho_genMatchedIndex);

    _SET_BRANCH_VEC(t_photon, float, elePt);
    _SET_BRANCH_VEC(t_photon, float, eleEta);
    _SET_BRANCH_VEC(t_photon, float, elePhi);
    _SET_BRANCH_VEC(t_photon, float, eleEoverP);
    _SET_BRANCH_VEC(t_photon, float, eleSigmaIEtaIEta_2012);
    _SET_BRANCH_VEC(t_photon, float, eledEtaAtVtx);
    _SET_BRANCH_VEC(t_photon, float, eledPhiAtVtx);
    _SET_BRANCH_VEC(t_photon, float, eleHoverE);
    _SET_BRANCH_VEC(t_photon, float, eleEoverPInv);
    _SET_BRANCH_VEC(t_photon, float, eleD0);
    _SET_BRANCH_VEC(t_photon, float, eleDz);
    _SET_BRANCH_VEC(t_photon, float, eleMissHits);

    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

    std::vector<float> etaBins[2];
    etaBins[0] = ConfigurationParser::ParseListFloat(configCuts.proc[CUTS::kCORRECTION].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_eta_gt]);
    etaBins[1] = ConfigurationParser::ParseListFloat(configCuts.proc[CUTS::kCORRECTION].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_eta_lt]);
    int nEtaBins = etaBins[0].size();

    gStyle->SetOptStat(0);

    TH2F* h_pt[nEtaBins];
    TH2F* h_ptratio[nEtaBins];
    TProfile* h_ptprof[nEtaBins];

    int nPtBins = 40;

    for (int j=0; j<nEtaBins; ++j) {
        h_pt[j] = new TH2F(Form("h_pt_%i", j), Form("%.2f<|#eta|<%.2f;reco p_{T}^{#gamma};gen p_{T}^{#gamma}", etaBins[0][j], etaBins[1][j]), nPtBins, 0, 200, nPtBins, 0, 200);
        h_ptratio[j] = new TH2F(Form("h_ptratio_%i", j), Form("%.2f<|#eta|<%.2f;reco p_{T}^{#gamma};#frac{reco p_{T}^{#gamma}}{gen p_{T}^{#gamma}}", etaBins[0][j], etaBins[1][j]), nPtBins, 0, 200, 50, 0, 4);
    }

    uint64_t nentries = t_photon->GetEntries();
    for (uint64_t i=0; i<nentries; ++i) {
        if (i % 5000 == 0)
            printf("entry: %lu\n", i);

        t_photon->GetEntry(i);
        t_event->GetEntry(i);

        for (uint32_t j=0; j<phoEt->size(); ++j) {
            if (phoEt->at(j) < 20)
                continue;
            if (fabs(phoEta->at(j)) > 2.4)
                continue;
            if (pho_genMatchedIndex->at(j) < 0)
                continue;
            if (pho_swissCrx->at(j) > 0.9)
                continue;
            if (fabs(pho_seedTime->at(j)) > 3)
                continue;

            int matchedEleIndex = -1;
            float matchedEoverP = 100;
            for (uint32_t k=0; k<elePt->size(); ++k) {
                if (elePt->at(k) < 10)
                    continue;
                if (eleSigmaIEtaIEta_2012->at(k) > 0.01107)
                    continue;
                if (eleEoverPInv->at(k) > 0.28051)
                    continue;
                if (fabs(eledEtaAtVtx->at(k)) > 0.01576)
                    continue;
                if (fabs(eledPhiAtVtx->at(k)) > 0.15724)
                    continue;
                if (eleHoverE->at(k) > 0.08849)
                    continue;
                if (fabs(eleD0->at(k)) > 0.05216)
                    continue;
                if (fabs(eleDz->at(k)) > 0.12997)
                    continue;
                if (eleMissHits->at(k) > 1)
                    continue;
                if (dR(phoEta->at(j), eleEta->at(k), phoPhi->at(j), elePhi->at(k)) > 0.04242)
                    continue;
                if (eleEoverP->at(k) > matchedEoverP)
                    continue;
                matchedEleIndex = k;
            }
            if (matchedEleIndex == -1)
                continue;

            int32_t mcindex = pho_genMatchedIndex->at(j);

            if (mcEt->at(mcindex) < 10)
                continue;
            if (mcCalIsoDR04->at(mcindex) > 5)
                continue;

            int ieta = 0;
            for (; fabs(phoEta->at(j))>=etaBins[1][ieta] && ieta<nEtaBins; ++ieta);

            h_pt[ieta]->Fill(phoEt->at(j), mcEt->at(mcindex));
            h_ptratio[ieta]->Fill(phoEt->at(j), phoEt->at(j)/mcEt->at(mcindex));
        }
    }

    TCanvas* c1 = new TCanvas("c1", "", 1500, 300 * 2 * nEtaBins);
    c1->Divide(nEtaBins, 2);
    for (int j=0; j<nEtaBins; ++j) {
        c1->cd(j+1);
        h_ptprof[j] = h_ptratio[j]->ProfileX();
        h_ptprof[j]->SetTitle(Form("%.2f<|#eta|<%.2f;reco p_{T}^{#gamma};#frac{reco p_{T}^{#gamma}}{gen p_{T}^{#gamma}}", etaBins[0][j], etaBins[1][j]));
        h_ptprof[j]->SetAxisRange(0.8, 1.5, "Y");
        h_ptprof[j]->SetMarkerStyle(20);
        h_ptprof[j]->SetMarkerSize(0.4);
        h_ptprof[j]->SetLineColor(1);
        h_ptprof[j]->Draw();

        c1->cd(nEtaBins+j+1);
        h_ptratio[j]->SetTitle(Form("%.2f<|#eta|<%.2f;reco p_{T}^{#gamma};#frac{reco p_{T}^{#gamma}}{gen p_{T}^{#gamma}}}", etaBins[0][j], etaBins[1][j]));
        h_ptratio[j]->Draw("colz");
    }

    TH1D* h_ecorr[nEtaBins];
    for (int j=0; j<nEtaBins; ++j)
        h_ecorr[j] = h_ptprof[j]->ProjectionX();

    TCanvas* c2 = new TCanvas("c2", "", 1500, 300 * nEtaBins);
    c2->Divide(nEtaBins, 1);
    for (int j=0; j<nEtaBins; ++j) {
        c2->cd(j+1);
        h_ecorr[j]->SetTitle(Form("%.2f<|#eta|<%.2f;reco p_{T}^{#gamma};#frac{reco p_{T}^{#gamma}}{gen p_{T}^{#gamma}}", etaBins[0][j], etaBins[1][j]));
        h_ecorr[j]->SetAxisRange(0.8, 1.8, "Y");
        h_ecorr[j]->SetMarkerStyle(20);
        h_ecorr[j]->SetMarkerSize(0.4);
        h_ecorr[j]->SetLineColor(1);
        h_ecorr[j]->Draw();
    }

    TFile* energyCorrectionFile = TFile::Open(configCuts.proc[CUTS::kCORRECTION].obj[CUTS::kPHOTON].s[CUTS::PHO::k_energy_correction_file_pp].c_str(), "recreate");
    c1->Write("", TObject::kOverwrite);
    c2->Write("", TObject::kOverwrite);
    for (int j=0; j<nEtaBins; ++j) {
        h_ptprof[j]->Write(Form("h_ptprof_eta%i", j), TObject::kOverwrite);
        h_ptratio[j]->Write(Form("h_ptratio_eta%i", j), TObject::kOverwrite);
        h_ecorr[j]->Write(Form("photonEnergyCorr_eta%i", j), TObject::kOverwrite);
    }
    energyCorrectionFile->Close();

    printf("\nroot -l %s\n", configCuts.proc[CUTS::kCORRECTION].obj[CUTS::kPHOTON].s[CUTS::PHO::k_energy_correction_file_pp].c_str());

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 3)
        return photonEnergyCorrections_pp(argv[1], argv[2]);
    else if (argc == 1)
        return photonEnergyCorrections_pp();
    else
        printf("Usage: ./photonEnergyCorrections_pp.exe <configFile> <Zee MC Forest>\n");

    return 1;
}

#define _PI 3.1415926535897932384
float dR(float eta1, float eta2, float phi1, float phi2) {
    float deta = eta1 - eta2;
    float dphi = fabs(phi1 - phi2);
    dphi = (dphi > _PI) ? 2 * _PI - dphi : dphi;
    return sqrt(deta * deta + dphi * dphi);
}
