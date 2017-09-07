// photonPurity.C
// Author: Alex Barbieri
// Cobbled together from code written by Yongsun Kim

// Calculates and plots the photon purity using a sideband estimation
// of the background and a MC calculation of the signal. This info is
// used to do a template fit in the sigmaIetaIeta variable.

#include "TStyle.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TCut.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "TString.h"
#include "TLatex.h"

#include <vector>
#include <string>
#include <iostream>

#include "PhotonPurity.h"

#include "../Utilities/interface/CutConfigurationParser.h"
#include "../Plotting/commonUtility.h"

const TCut noiseCut = "!((phoE3x3[phoIdx]/phoE5x5[phoIdx] > 2./3.-0.03 && phoE3x3[phoIdx]/phoE5x5[phoIdx] < 2./3.+0.03) && (phoE1x5[phoIdx]/phoE5x5[phoIdx] > 1./3.-0.03 && phoE1x5[phoIdx]/phoE5x5[phoIdx] < 1./3.+0.03) && (phoE2x5[phoIdx]/phoE5x5[phoIdx] > 2./3.-0.03 && phoE2x5[phoIdx]/phoE5x5[phoIdx] < 2./3.+0.03))";

const vector<int> min_cent = {100, 60, 20, 0};
const vector<int> max_cent = {200, 100, 60, 20};
const std::size_t ncent = min_cent.size();

const vector<float> min_pt = {60};
const vector<float> max_pt = {9999};
const std::size_t npt = min_pt.size();

int quickPhotonPurity(const TString configFile, const TString skim_data, const TString skim_mc, const std::string label) {
  gStyle->SetOptStat(0);

  TH1::SetDefaultSumw2();

  CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());
  if (!configCuts.isValid) {
    std::cout << "Invalid configfile, terminating." << std::endl;
    return 1;
  }

  std::string jetCollection = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kJET].s[CUTS::JET::k_jetCollection];

  std::string trigger = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_trigger_gammaJet].c_str();
  // trigger is named differently in MC, hardcode for now :(
  std::string trigger_mc = "(HLT_HISinglePhoton40_Eta1p5_v2)";

  TFile* fskim_data = TFile::Open(skim_data);
  TTree* hlt_tree = (TTree*)fskim_data->Get("hltTree");
  TTree* photon_tree = (TTree*)fskim_data->Get("EventTree");    // photons
  TTree* photonjet_tree  = (TTree*)fskim_data->Get(Form("gamma_%s", jetCollection.c_str()));
  TTree* event_tree = (TTree*)fskim_data->Get("HiEvt");       // HiEvt tree will be placed in PP forest as well.

  TFile* fskim_mc = TFile::Open(skim_mc);
  TTree* hlt_tree_mc = (TTree*)fskim_mc->Get("hltTree");
  TTree* photon_tree_mc = (TTree*)fskim_mc->Get("EventTree");    // photons
  TTree* photonjet_tree_mc  = (TTree*)fskim_mc->Get(Form("gamma_%s", jetCollection.c_str()));
  TTree* event_tree_mc = (TTree*)fskim_mc->Get("HiEvt");       // HiEvt tree will be placed in PP forest as well

  photonjet_tree->AddFriend(hlt_tree, "Hlt");
  photonjet_tree->AddFriend(photon_tree, "Pho");
  photonjet_tree->AddFriend(event_tree, "HiEvt");

  photonjet_tree_mc->AddFriend(hlt_tree_mc, "Hlt");
  photonjet_tree_mc->AddFriend(photon_tree_mc, "Pho");
  photonjet_tree_mc->AddFriend(event_tree_mc, "HiEvt");

  TFile* output = new TFile(Form("%s.root", label.c_str()), "RECREATE");

  const TCut candidate_isolation = "(pho_sumIsoCorrected[phoIdx]) < 1.0 && phoHoverE[phoIdx]<0.1";
  const TCut sideband_isolation = "((pho_sumIsoCorrected[phoIdx])>10) && ((pho_sumIsoCorrected[phoIdx])<20) && phoHoverE[phoIdx]<0.1";
  const TCut signal_selection = "(pho_genMatchedIndex[phoIdx]!= -1) && mcCalIsoDR04[pho_genMatchedIndex[phoIdx]]<5 && mcPID[pho_genMatchedIndex[phoIdx]] == 22 && (abs(mcMomPID[pho_genMatchedIndex[phoIdx]])<=22 || mcMomPID[pho_genMatchedIndex[phoIdx]] == -999)";

  for (std::size_t i = 0; i < ncent; ++i) {
    for (std::size_t j = 0; j < npt; ++j) {
      TString pt_cut = Form("(phoEtCorrected[phoIdx] >= %f) && (phoEtCorrected[phoIdx] < %f)",
          min_pt[j], max_pt[j]);
      TString cent_cut = Form("(hiBin >= %i) && (hiBin < %i)",
          min_cent[i], max_cent[i]);
      TString eta_cut = "(abs(phoEta[phoIdx]) < 1.44)";

      TCut trigger_selection = Form("%s == 1", trigger.c_str());
      TCut trigger_selection_mc = Form("%s == 1", trigger_mc.c_str());

      TCut candidate_cut = candidate_isolation && eta_cut && pt_cut && cent_cut && noiseCut && trigger_selection;
      TCut sideband_cut = sideband_isolation && eta_cut && pt_cut && cent_cut && noiseCut && trigger_selection;
      TCut signal_cut = signal_selection && eta_cut && pt_cut && cent_cut && trigger_selection_mc;

      PhotonPurity purity = calc_purity(configCuts, photonjet_tree, photonjet_tree_mc, candidate_cut, sideband_cut, signal_cut);

      TH1D* hsig = purity.sig_fit;
      TH1D* hbkg = purity.bkg_fit;
      TH1D* hdata  = purity.data;
      hsig->Add(hbkg);

      hsig->Write(Form("signal_template_%zu_%zu", i, j), TObject::kOverwrite);
      hbkg->Write(Form("background_template_%zu_%zu", i ,j), TObject::kOverwrite);
      hdata->Write(Form("data_points_%zu_%zu", i, j), TObject::kOverwrite);
    }
  }

  output->Close();

  return 0;
}

int main(int argc, char* argv[]) {
  if (argc == 5)
    return quickPhotonPurity(argv[1], argv[2], argv[3], argv[4]);

  return 1;
}
