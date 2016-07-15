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

    TFile* f_input = new TFile(inputFile, "read");
    TTree* t_photon = (TTree*)f_input->Get("EventTree");
    TTree* t_gj = (TTree*)f_input->Get(Form("gamma_%s", jetCollections[0].c_str()));
    TTree* t_event = (TTree*)f_input->Get("HiEvt");

    t_photon->SetBranchStatus("*", 0);
    _SET_BRANCH_VEC(t_photon, float, phoPhi);
    _SET_BRANCH_VEC(t_photon, float, pho_ecalClusterIsoR4);
    _SET_BRANCH_VEC(t_photon, float, pho_hcalRechitIsoR4);
    _SET_BRANCH_VEC(t_photon, float, pho_trackIsoR4PtCut20);

    t_gj->SetBranchStatus("*", 0);
    _SET_BRANCH_VAR(t_gj, int, phoIdx);

    t_event->SetBranchStatus("*", 0);
    float hiEvtPlanes[29];
    t_event->SetBranchStatus("hiEvtPlanes", 1);
    t_event->SetBranchAddress("hiEvtPlanes", hiEvtPlanes);

    gStyle->SetOptStat(0);

    TFile* f_output = TFile::Open(configCuts.proc[CUTS::kCORRECTION].obj[CUTS::kPHOTON].s[CUTS::PHO::k_sumiso_correction_file].c_str(), "recreate");

    TH2D* h2D_sumIso_angle = new TH2D("h2D_sumIso_angle", "", 80, 0, 1.6, 800, -200, 200);

    uint64_t nentries = t_photon->GetEntries();
    for (uint64_t i=0; i<nentries; ++i) {
        t_photon->GetEntry(i);
        t_gj->GetEntry(i);
        t_event->GetEntry(i);

        if (phoIdx < 0)
            continue;

        double angle = getAngleToEP(fabs((*phoPhi)[phoIdx] - hiEvtPlanes[8]));
        double sumIso = (*pho_ecalClusterIsoR4)[phoIdx] + (*pho_hcalRechitIsoR4)[phoIdx] + (*pho_trackIsoR4PtCut20)[phoIdx];

        h2D_sumIso_angle->Fill(angle, sumIso);
    }

    double meanSumIso = h2D_sumIso_angle->GetMean(2);

    TProfile* h1D_meanSumIso_angle = (TProfile*)h2D_sumIso_angle->ProfileX("h1D_meanSumIso_angle");
    h1D_meanSumIso_angle->RebinX(4);

    TH1D* sumIsoCorrections = (TH1D*)h1D_meanSumIso_angle->Clone("sumIsoCorrections");
    sumIsoCorrections->Scale(1/meanSumIso);

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

