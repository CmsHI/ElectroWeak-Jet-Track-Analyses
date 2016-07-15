#include "TTree.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TString.h"
#include "TLorentzVector.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TLegend.h"

#include "RooFit.h"
#include "RooRealVar.h"
#include "RooBreitWigner.h"
#include "RooCBShape.h"
#include "RooVoigtian.h"
#include "RooFFTConvPdf.h"
#include "RooPlot.h"
#include "RooDataHist.h"

#include <vector>

#include "../TreeHeaders/CutConfigurationTree.h"
#include "../Utilities/interface/CutConfigurationParser.h"
#include "../Utilities/interface/InputConfigurationParser.h"

#define _SET_BRANCH_VEC(tree, type, branch)     \
    std::vector<type>* branch = 0;              \
    tree->SetBranchStatus(#branch, 1);          \
    tree->SetBranchAddress(#branch, &branch);   \

#define _SET_BRANCH_VAR(tree, type, branch)     \
    type branch;                                \
    tree->SetBranchStatus(#branch, 1);          \
    tree->SetBranchAddress(#branch, &branch);   \

void fill_hists(TTree* t_photon, TTree* t_event, TH1D* h_mass, TH2D* h_weights,
                std::vector<int> centBins[2], std::vector<float> etaBins[2], TH1D** h_mass_cent,
                int apply_corrections, TH1D** h_ecorr_ptr);
void fill_hists_gen(TTree* t_photon, TTree* t_event, TH1D* h_mass, TH2D* h_weights,
                    std::vector<int> centBins[2], TH1D** h_mass_cent);

int eleMatchedPhotonZPeak(const TString configFile = "CutConfigurations/gammaJet.conf", const int apply_corrections = 1,
                          const char* fn_data = "/mnt/hadoop/cms/store/user/rbi/azsigmon-HIRun2015E-PromptReco-AOD-DielectronSkim-ElePt8-v3_forest_csjet_v1_3/0.root",
                          const char* fn_mc = "/mnt/hadoop/cms/store/user/tatar/official/Pythia8_Z30eeJet_Hydjet_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13_ext1-FOREST/0.root",
                          const char* fn_weights = "Performance/Zee_weights.root") {
    TFile* f_data = new TFile(fn_data, "read");
    TTree* t_photon = (TTree*)f_data->Get("ggHiNtuplizer/EventTree");
    TTree* t_event = (TTree*)f_data->Get("hiEvtAnalyzer/HiTree");

    InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

    TFile* f_weights = new TFile(fn_weights, "read");
    TH2D* h_weights = 0;
    if (f_weights && f_weights->IsOpen())
        (TH2D*)f_weights->Get("h_weights");

    std::vector<int> centBins[2];
    centBins[0] = ConfigurationParser::ParseListInteger(configCuts.proc[CUTS::kCORRECTION].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_gt]);
    centBins[1] = ConfigurationParser::ParseListInteger(configCuts.proc[CUTS::kCORRECTION].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_lt]);
    int nCentBins = centBins[0].size();

    std::vector<float> etaBins[2];
    etaBins[0] = ConfigurationParser::ParseListFloat(configCuts.proc[CUTS::kCORRECTION].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_eta_gt]);
    etaBins[1] = ConfigurationParser::ParseListFloat(configCuts.proc[CUTS::kCORRECTION].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_eta_lt]);
    int nEtaBins = etaBins[0].size();

    TFile* energyCorrectionFile = TFile::Open(configCuts.proc[CUTS::kCORRECTION].obj[CUTS::kPHOTON].s[CUTS::PHO::k_energy_correction_file].c_str(), "read");
    TH1D* h_ecorr[nCentBins][nEtaBins];
    for (int i=0; i<nCentBins; ++i)
        for (int j=0; j<nEtaBins; ++j)
            h_ecorr[i][j] = (TH1D*)energyCorrectionFile->Get(Form("photonEnergyCorr_cent%i_eta%i", i, j));

    gStyle->SetOptStat(0);

    TH1::SetDefaultSumw2();

    TH1D* h_mass = new TH1D("h_mass", "", 30, 60, 120);

    TH1D* h_mass_cent[nCentBins];
    for (int i=0; i<nCentBins; ++i)
        h_mass_cent[i] = new TH1D(Form("h_mass_cent_%i", i), Form("%i-%i%% Centrality;M_{Inv};", centBins[0][i]/2, centBins[1][i]/2), 30, 60, 120);

    fill_hists(t_photon, t_event, h_mass, 0, centBins, etaBins, h_mass_cent, apply_corrections, (TH1D**)h_ecorr);

    TFile* f_mc = new TFile(fn_mc, "read");
    t_photon = (TTree*)f_mc->Get("ggHiNtuplizer/EventTree");
    t_event = (TTree*)f_mc->Get("hiEvtAnalyzer/HiTree");

    TH1D* h_mass_mc = new TH1D("h_mass_mc", "", 30, 60, 120);

    TH1D* h_mass_cent_mc[nCentBins];
    for (int i=0; i<nCentBins; ++i)
        h_mass_cent_mc[i] = new TH1D(Form("h_mass_cent_mc_%i", i), Form("%i-%i%% Centrality;M_{Inv};", centBins[0][i]/2, centBins[1][i]/2), 30, 60, 120);

    fill_hists(t_photon, t_event, h_mass_mc, h_weights, centBins, etaBins, h_mass_cent_mc, apply_corrections, (TH1D**)h_ecorr);

    TH1D* h_mass_gen = new TH1D("h_mass_gen", "", 30, 60, 120);

    TH1D* h_mass_cent_gen[nCentBins];
    for (int i=0; i<nCentBins; ++i)
        h_mass_cent_gen[i] = new TH1D(Form("h_mass_cent_gen_%i", i), Form("%i-%i%% Centrality;M_{Inv};", centBins[0][i]/2, centBins[1][i]/2), 30, 60, 120);

    fill_hists_gen(t_photon, t_event, h_mass_gen, h_weights, centBins, h_mass_cent_gen);

    RooRealVar x("x", "x", 60, 120);

    RooRealVar m0("m0", "m0", 90, 60, 120);
    RooRealVar width("width", "width", 10, 0, 40);
    RooBreitWigner bw("bw", "bw", x, m0, width);

    RooRealVar mean("mean", "mean", 0);
    RooRealVar sigma("sigma", "sigma", 10, 0, 40);
    RooRealVar alpha("alpha", "alpha", 10, 0, 40);
    RooRealVar n("n", "n", 10, 0, 40);
    RooCBShape cb("cb", "cb", x, mean, sigma, alpha, n);

    RooFFTConvPdf pdf("pdf", "pdf", x, bw, cb);

    RooRealVar vmean("vmean", "vmean", 90, 60, 120);
    RooRealVar vwidth("vwidth", "vwidth", 10, 0, 40);
    RooRealVar vsigma("vsigma", "vsigma", 10, 0, 40);
    RooVoigtian v("v", "v", x, vmean, vwidth, vsigma);

    double hist_centBins[nCentBins + 1];
    for (int i=0; i<nCentBins; ++i)
        hist_centBins[i] = centBins[0][i];
    hist_centBins[nCentBins] = centBins[1][nCentBins - 1];

    TH1D* h_z_mass = new TH1D("h_z_mass", "Z Mass;Centrality;M_{Inv}", nCentBins, hist_centBins);
    TH1D* h_z_mass_mc = new TH1D("h_z_mass_mc", "Z Mass;Centrality;M_{Inv}", nCentBins, hist_centBins);
    TH1D* h_z_mass_gen = new TH1D("h_z_mass_gen", "Z Mass;Centrality;M_{Inv}", nCentBins, hist_centBins);

    for (int i=0; i<nCentBins; ++i) {
        RooDataHist data_cent("data_cent", "dataset", x, h_mass_cent[i]);
        bw.fitTo(data_cent);
        h_z_mass->SetBinContent(i+1, m0.getVal());
        h_z_mass->SetBinError(i+1, m0.getError());

        RooDataHist data_cent_mc("data_cent_mc", "dataset", x, h_mass_cent_mc[i]);
        bw.fitTo(data_cent_mc);
        h_z_mass_mc->SetBinContent(i+1, m0.getVal());
        h_z_mass_mc->SetBinError(i+1, m0.getError());

        RooDataHist data_cent_gen("data_cent_gen", "dataset", x, h_mass_cent_gen[i]);
        bw.fitTo(data_cent_gen);
        h_z_mass_gen->SetBinContent(i+1, m0.getVal());
        h_z_mass_gen->SetBinError(i+1, m0.getError());
    }

    TCanvas* c1 = new TCanvas("c1", "", 400, 400);

    h_z_mass_gen->SetMarkerStyle(21);
    h_z_mass_gen->SetMarkerSize(0.8);
    h_z_mass_gen->SetMarkerColor(4);
    h_z_mass_gen->SetLineColor(4);
    h_z_mass_gen->SetAxisRange(80, 110, "Y");

    h_z_mass_mc->SetMarkerStyle(20);
    h_z_mass_mc->SetMarkerSize(0.8);
    h_z_mass_mc->SetMarkerColor(2);
    h_z_mass_mc->SetLineColor(2);
    h_z_mass_mc->SetAxisRange(80, 110, "Y");

    h_z_mass->SetMarkerStyle(22);
    h_z_mass->SetMarkerSize(0.8);
    h_z_mass->SetMarkerColor(1);
    h_z_mass->SetLineColor(1);
    h_z_mass->SetAxisRange(80, 110, "Y");

    h_z_mass_gen->Draw();
    h_z_mass_mc->Draw("same");
    h_z_mass->Draw("same");

    TLegend* l1 = new TLegend(0.6, 0.64, 0.8, 0.75);
    l1->SetBorderSize(0);
    l1->SetTextSize(0.04);
    l1->AddEntry(h_z_mass, "Data");
    l1->AddEntry(h_z_mass_mc, "MC");
    l1->AddEntry(h_z_mass_gen, "Gen");
    l1->Draw();

    int nrows = (nCentBins - 1) / 5 + 1;
    TCanvas* c2 = new TCanvas("c2", "", 1500, 300 * nrows);
    c2->Divide(5, nrows);
    for (int i=0; i<nCentBins; ++i) {
        c2->cd(i+1);
        h_mass_cent_gen[i]->Scale(1/h_mass_cent_gen[i]->Integral());
        h_mass_cent_gen[i]->SetMarkerSize(0);
        h_mass_cent_gen[i]->SetLineColor(4);
        h_mass_cent_gen[i]->Draw("hist e");
        h_mass_cent_mc[i]->Scale(1/h_mass_cent_mc[i]->Integral());
        h_mass_cent_mc[i]->SetMarkerSize(0);
        h_mass_cent_mc[i]->SetLineColor(2);
        h_mass_cent_mc[i]->Draw("same hist e");
        h_mass_cent[i]->Scale(1/h_mass_cent[i]->Integral());
        h_mass_cent[i]->SetMarkerStyle(20);
        h_mass_cent[i]->SetMarkerSize(0.7);
        h_mass_cent[i]->SetMarkerColor(1);
        h_mass_cent[i]->SetLineColor(1);
        h_mass_cent[i]->Draw("same p e");
    }

    TFile* f_out = new TFile("Performance/eleMatchedPhotonZPeak.root", "recreate");
    h_z_mass->Write("h_z_mass", TObject::kOverwrite);
    h_z_mass_mc->Write("h_z_mass_mc", TObject::kOverwrite);
    h_z_mass_gen->Write("h_z_mass_gen", TObject::kOverwrite);
    for (int i=0; i<nCentBins; ++i) {
        h_mass_cent_gen[i]->Write("", TObject::kOverwrite);
        h_mass_cent_mc[i]->Write("", TObject::kOverwrite);
        h_mass_cent[i]->Write("", TObject::kOverwrite);
    }
    c1->Write("", TObject::kOverwrite);
    c2->Write("", TObject::kOverwrite);
    f_out->Close();

    printf("\n%s\n", "root -l Performance/eleMatchedPhotonZPeak.root");

    return 0;
}

void fill_hists(TTree* t_photon, TTree* t_event, TH1D* h_mass, TH2D* h_weights,
                std::vector<int> centBins[2], std::vector<float> etaBins[2], TH1D** h_mass_cent,
                int apply_corrections, TH1D** h_ecorr_ptr) {
    int nCentBins = centBins[1].size();
    int nEtaBins = etaBins[1].size();

    TH1D* h_ecorr[nCentBins][nEtaBins];
    memcpy(h_ecorr, h_ecorr_ptr, nCentBins * nEtaBins * sizeof(TH1D*));

    t_photon->SetBranchStatus("*", 0);
    _SET_BRANCH_VAR(t_photon, int, nPho)
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

    t_event->SetBranchStatus("*", 0);
    _SET_BRANCH_VAR(t_event, int, hiBin);
    _SET_BRANCH_VAR(t_event, float, vz);

    int32_t nentries = t_photon->GetEntries();
    for (int32_t i=0; i<nentries; ++i) {
        if (i % 5000 == 0)
            printf("entry: %i\n", i);

        t_photon->GetEntry(i);
        t_event->GetEntry(i);

        for (int32_t j=0; j<nPho; ++j) {
            if (phoEt->at(j) < 20)
                break;
            if (fabs(phoEta->at(j)) > 1.44)
                continue;
            if (phoSigmaIEtaIEta_2012->at(j) > 0.01)
                continue;
            if (phoHoverE->at(j) > 0.1)
                continue;
            if (pho_swissCrx->at(j) > 0.9)
                continue;
            if (fabs(pho_seedTime->at(j)) > 3)
                continue;

            for (int32_t k=j+1; k<nPho; ++k) {
                if (phoEt->at(k) < 20)
                    break;
                if (fabs(phoEta->at(k)) > 1.44)
                    continue;
                if (phoSigmaIEtaIEta_2012->at(k) > 0.01)
                    continue;
                if (phoHoverE->at(k) > 0.1)
                    continue;
                if (pho_swissCrx->at(k) > 0.9)
                    continue;
                if (fabs(pho_seedTime->at(k)) > 3)
                    continue;

                int icent = 0;
                for (; hiBin>=centBins[1][icent] && icent<nCentBins; ++icent);

                int ieta1 = 0;
                for (; fabs(phoEta->at(j))>=etaBins[1][ieta1] && ieta1<nEtaBins; ++ieta1);
                int ieta2 = 0;
                for (; fabs(phoEta->at(k))>=etaBins[1][ieta2] && ieta2<nEtaBins; ++ieta2);

                float phoEt1 = phoEt->at(j);
                float phoEt2 = phoEt->at(k);
                if (apply_corrections) {
                    phoEt1 /= h_ecorr[icent][ieta1]->GetBinContent(h_ecorr[icent][ieta1]->FindBin(phoEt1));
                    phoEt2 /= h_ecorr[icent][ieta2]->GetBinContent(h_ecorr[icent][ieta2]->FindBin(phoEt2));
                }

                TLorentzVector pho1, pho2;
                pho1.SetPtEtaPhiM(phoEt1, phoEta->at(j), phoPhi->at(j), 0);
                pho2.SetPtEtaPhiM(phoEt2, phoEta->at(k), phoPhi->at(k), 0);
                TLorentzVector z = pho1 + pho2;

                double weight;
                if (h_weights)
                    weight = h_weights->GetBinContent(h_weights->FindBin(hiBin, vz));
                else
                    weight = 1;

                if (z.M() != 0) {
                    h_mass->Fill(z.M(), weight);
                    h_mass_cent[icent]->Fill(z.M(), weight);
                }
            }
        }
    }
}

void fill_hists_gen(TTree* t_photon, TTree* t_event, TH1D* h_mass, TH2D* h_weights,
                    std::vector<int> centBins[2], TH1D** h_mass_cent) {
    t_photon->SetBranchStatus("*", 0);
    _SET_BRANCH_VAR(t_photon, int, nMC);
    _SET_BRANCH_VEC(t_photon, int, mcPID);
    _SET_BRANCH_VEC(t_photon, int, mcStatus);
    _SET_BRANCH_VEC(t_photon, float, mcPt);
    _SET_BRANCH_VEC(t_photon, float, mcEta);
    _SET_BRANCH_VEC(t_photon, float, mcPhi);

    t_event->SetBranchStatus("*", 0);
    _SET_BRANCH_VAR(t_event, int, hiBin);
    _SET_BRANCH_VAR(t_event, float, vz);

    const double electronMass = 0.000510998;

    int32_t nentries = t_photon->GetEntries();
    for (int32_t i=0; i<nentries; ++i) {
        if (i % 5000 == 0)
            printf("entry: %i\n", i);

        t_photon->GetEntry(i);
        t_event->GetEntry(i);

        for (int32_t j=0; j<nMC; ++j) {
            if (abs(mcPID->at(j)) != 11)
                continue;
            if (mcStatus->at(j) != 1)
                continue;
            if (mcPt->at(j) < 20)
                continue;
            if (fabs(mcEta->at(j)) > 1.44)
                continue;

            for (int32_t k=j+1; k<nMC; ++k) {
                if (abs(mcPID->at(k)) != 11)
                    continue;
                if (mcStatus->at(k) != 1)
                    continue;
                if (mcPt->at(k) < 20)
                    continue;
                if (fabs(mcEta->at(k)) > 1.44)
                    continue;

                if (mcPID->at(j) == mcPID->at(k))
                    continue;

                int icent = 0;
                int nCentBins = centBins[1].size();
                for (; hiBin>=centBins[1][icent] && icent<nCentBins; ++icent);

                TLorentzVector ele1, ele2;
                ele1.SetPtEtaPhiM(mcPt->at(j), mcEta->at(j), mcPhi->at(j), electronMass);
                ele2.SetPtEtaPhiM(mcPt->at(k), mcEta->at(k), mcPhi->at(k), electronMass);
                TLorentzVector z = ele1 + ele2;

                double weight;
                if (h_weights)
                    weight = h_weights->GetBinContent(h_weights->FindBin(hiBin, vz));
                else
                    weight = 1;

                if (z.M() != 0) {
                    h_mass->Fill(z.M(), weight);
                    h_mass_cent[icent]->Fill(z.M(), weight);
                }
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc == 1)
        return eleMatchedPhotonZPeak();
    else if (argc == 3)
        return eleMatchedPhotonZPeak(argv[1], atoi(argv[2]));
    else if (argc == 6)
        return eleMatchedPhotonZPeak(argv[1], atoi(argv[2]), argv[3], argv[4], argv[5]);

    return 1;
}
