#include "TFile.h"
#include "TTree.h"
#include "TStyle.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TLine.h"
#include "TMath.h"
#include "TCanvas.h"

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

double getAngleToEP(double angle);

int isolationCorrections(const TString configFile, const char* inputFile) {
    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

    std::vector<std::string> jetCollections = ConfigurationParser::ParseList(configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].s[CUTS::JET::k_jetCollection]);
    if (!jetCollections.size())
        return 1;

    std::vector<int> centBins[2];
    centBins[0] = ConfigurationParser::ParseListInteger(configCuts.proc[CUTS::kCORRECTION].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_gt]);
    centBins[1] = ConfigurationParser::ParseListInteger(configCuts.proc[CUTS::kCORRECTION].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_lt]);
    int nCentBins = centBins[0].size();

    TFile* f_input = new TFile(inputFile, "read");
    TTree* t_photon = (TTree*)f_input->Get("EventTree");
    TTree* t_gj = (TTree*)f_input->Get(Form("gamma_%s", jetCollections[0].c_str()));
    TTree* t_event = (TTree*)f_input->Get("HiEvt");

    t_photon->SetBranchStatus("*", 0);
    _SET_BRANCH_VEC(t_photon, float, phoPhi);
    _SET_BRANCH_VEC(t_photon, float, pho_ecalClusterIsoR4);
    _SET_BRANCH_VEC(t_photon, float, pho_hcalRechitIsoR4);
    _SET_BRANCH_VEC(t_photon, float, pho_trackIsoR4PtCut20);
    _SET_BRANCH_VEC(t_photon, float, phoE3x3);
    _SET_BRANCH_VEC(t_photon, float, phoE1x5);
    _SET_BRANCH_VEC(t_photon, float, phoE2x5);
    _SET_BRANCH_VEC(t_photon, float, phoE5x5);
    _SET_BRANCH_VEC(t_photon, float, phoHoverE);

    t_gj->SetBranchStatus("*", 0);
    _SET_BRANCH_VAR(t_gj, int, phoIdx);

    t_event->SetBranchStatus("*", 0);
    float hiEvtPlanes[29];
    t_event->SetBranchStatus("hiEvtPlanes", 1);
    t_event->SetBranchAddress("hiEvtPlanes", hiEvtPlanes);
    _SET_BRANCH_VAR(t_event, int, hiBin);

    gStyle->SetOptStat(0);

    TFile* f_output = TFile::Open(configCuts.proc[CUTS::kCORRECTION].obj[CUTS::kPHOTON].s[CUTS::PHO::k_sumiso_correction_file].c_str(), "recreate");

    TH2D* h2D_sumIso_angle[nCentBins] = {0};
    for (int j=0; j<nCentBins; ++j)
        h2D_sumIso_angle[j] = new TH2D(Form("h2D_sumIso_angle_cent%i", j), "", 80, 0, 1.6, 800, -200, 200);

    uint64_t nentries = t_photon->GetEntries();
    for (uint64_t i=0; i<nentries; ++i) {
        t_photon->GetEntry(i);
        t_gj->GetEntry(i);
        t_event->GetEntry(i);

        if (phoIdx < 0)
            continue;
        if (((*phoE3x3)[phoIdx]/(*phoE5x5)[phoIdx] > 2./3.-0.03 && (*phoE3x3)[phoIdx]/(*phoE5x5)[phoIdx] < 2./3.+0.03) &&
            ((*phoE1x5)[phoIdx]/(*phoE5x5)[phoIdx] > 1./3.-0.03 && (*phoE1x5)[phoIdx]/(*phoE5x5)[phoIdx] < 1./3.+0.03) &&
            ((*phoE2x5)[phoIdx]/(*phoE5x5)[phoIdx] > 2./3.-0.03 && (*phoE2x5)[phoIdx]/(*phoE5x5)[phoIdx] < 2./3.+0.03))
            continue;
        if ((*phoHoverE)[phoIdx] > 0.1)
            continue;

        double sumIso = (*pho_ecalClusterIsoR4)[phoIdx] + (*pho_hcalRechitIsoR4)[phoIdx] + (*pho_trackIsoR4PtCut20)[phoIdx];
        double angle = getAngleToEP(fabs((*phoPhi)[phoIdx] - hiEvtPlanes[8]));

        int icent = 0;
        for (; hiBin>=centBins[1][icent] && icent<nCentBins; ++icent);

        h2D_sumIso_angle[icent]->Fill(angle, sumIso);
    }

    double meanSumIso[nCentBins];
    TProfile* h1D_meanSumIso_angle[nCentBins] = {0};
    TH1D* sumIsoCorrections[nCentBins] = {0};

    for (int j=0; j<nCentBins; ++j) {
        meanSumIso[j] = h2D_sumIso_angle[j]->GetMean(2);
        h1D_meanSumIso_angle[j] = (TProfile*)h2D_sumIso_angle[j]->ProfileX(Form("h1D_meanSumIso_angle_cent%i", j));
        h1D_meanSumIso_angle[j]->RebinX(8);

        sumIsoCorrections[j] = new TH1D(Form("sumIsoCorrections_cent%i", j), "", 10, 0, 1.6);
        for (int k=1; k<=sumIsoCorrections[j]->GetNbinsX(); ++k)
            sumIsoCorrections[j]->SetBinContent(k, h1D_meanSumIso_angle[j]->GetBinContent(k) - meanSumIso[j]);
    }

    f_output->Write("", TObject::kOverwrite);
    f_output->Close();

    return 0;
}

double getAngleToEP(double angle) {
    angle = (angle > TMath::Pi()) ? 2 * TMath::Pi() - angle : angle;
    return (angle > TMath::Pi()/2) ? TMath::Pi() - angle : angle;
}

int main(int argc, char* argv[]) {
    if (argc == 3)
        return isolationCorrections(argv[1], argv[2]);
    else
        printf("Usage: ./isolationCorrections.exe <configFile> <inputFile>\n");

    return 1;
}

