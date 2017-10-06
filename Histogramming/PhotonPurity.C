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

#include "Purity.h"

#include "../Utilities/interface/CutConfigurationParser.h"

const TCut noiseCut = "!((phoE3x3[phoIdx]/phoE5x5[phoIdx] > 2./3.-0.03 && phoE3x3[phoIdx]/phoE5x5[phoIdx] < 2./3.+0.03) && (phoE1x5[phoIdx]/phoE5x5[phoIdx] > 1./3.-0.03 && phoE1x5[phoIdx]/phoE5x5[phoIdx] < 1./3.+0.03) && (phoE2x5[phoIdx]/phoE5x5[phoIdx] > 2./3.-0.03 && phoE2x5[phoIdx]/phoE5x5[phoIdx] < 2./3.+0.03))";

const std::vector<int> min_cent = {0, 0, 30, 0, 20, 60, 100};
const std::vector<int> max_cent = {200, 60, 2000, 20, 60, 100, 200};
const std::size_t ncent = min_cent.size();

const std::vector<float> min_pt = {40, 60, 40, 50, 60, 80, 80, 100};
const std::vector<float> max_pt = {9999, 9999, 50, 60, 80, 9999, 100, 9999};
const std::size_t npt = min_pt.size();

int PhotonPurity(const TString configFile, const TString skim_data, const TString skim_mc, const std::string label) {
  gStyle->SetOptStat(0);

  TH1::SetDefaultSumw2();

  CutConfiguration config = CutConfigurationParser::Parse(configFile.Data());
  if (!config.isValid) {
    std::cout << "Invalid configfile, terminating." << std::endl;
    return 1;
  }

  float purity_values[ncent][npt] = {0};

  std::string jetCollection = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kJET].s[CUTS::JET::k_jetCollection];

  std::string trigger = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_trigger_gammaJet].c_str();
  // trigger is named differently in MC, hardcode for now :(
  std::string trigger_mc = "(HLT_HISinglePhoton40_Eta1p5_v2)";

  TFile* fskim_data = TFile::Open(skim_data);
  TTree* data_tree  = (TTree*)fskim_data->Get(Form("gamma_%s", jetCollection.c_str()));
  TTree* hlt_tree = (TTree*)fskim_data->Get("hltTree");
  TTree* photon_tree = (TTree*)fskim_data->Get("EventTree");
  TTree* event_tree = (TTree*)fskim_data->Get("HiEvt");

  TFile* fskim_mc = TFile::Open(skim_mc);
  TTree* mc_tree  = (TTree*)fskim_mc->Get(Form("gamma_%s", jetCollection.c_str()));
  TTree* mc_hlt_tree = (TTree*)fskim_mc->Get("hltTree");
  TTree* mc_photon_tree = (TTree*)fskim_mc->Get("EventTree");
  TTree* mc_event_tree = (TTree*)fskim_mc->Get("HiEvt");

  data_tree->AddFriend(hlt_tree, "Hlt");
  data_tree->AddFriend(photon_tree, "Pho");
  data_tree->AddFriend(event_tree, "HiEvt");

  mc_tree->AddFriend(mc_hlt_tree, "Hlt");
  mc_tree->AddFriend(mc_photon_tree, "Pho");
  mc_tree->AddFriend(mc_event_tree, "HiEvt");

  TFile* output = new TFile(Form("%s.root", label.c_str()), "RECREATE");

  const TCut eta_cut = "(abs(phoEta[phoIdx]) < 1.44)";

  const TCut candidate_isolation = "(pho_sumIsoCorrected[phoIdx]) < 1.0 && phoHoverE[phoIdx]<0.1";
  const TCut sideband_isolation = "((pho_sumIsoCorrected[phoIdx])>10) && ((pho_sumIsoCorrected[phoIdx])<20) && phoHoverE[phoIdx]<0.1";
  const TCut signal_selection = "(pho_genMatchedIndex[phoIdx]!= -1) && mcCalIsoDR04[pho_genMatchedIndex[phoIdx]]<5 && mcPID[pho_genMatchedIndex[phoIdx]] == 22 && (abs(mcMomPID[pho_genMatchedIndex[phoIdx]])<=22 || mcMomPID[pho_genMatchedIndex[phoIdx]] == -999)";

  const std::string weight_string = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_monteCarloWeightLabel];
  TCut weight_label = weight_string.c_str();

  for (std::size_t i = 0; i < ncent; ++i) {
    for (std::size_t j = 0; j < npt; ++j) {
      TString pt_cut = Form("(phoEtCorrected[phoIdx] >= %f) && (phoEtCorrected[phoIdx] < %f)", min_pt[j], max_pt[j]);
      TString cent_cut = Form("(hiBin >= %i) && (hiBin < %i)", min_cent[i], max_cent[i]);

      TCut trigger_selection = Form("%s == 1", trigger.c_str());
      TCut trigger_selection_mc = Form("%s == 1", trigger_mc.c_str());

      TCut candidate_cut = candidate_isolation && eta_cut && pt_cut && cent_cut && noiseCut && trigger_selection;
      TCut sideband_cut = sideband_isolation && eta_cut && pt_cut && cent_cut && noiseCut && trigger_selection;
      TCut signal_cut = signal_selection && eta_cut && pt_cut && cent_cut && trigger_selection_mc;
      signal_cut = signal_cut * weight_label;

      const int nbins = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].i[CUTS::PHO::k_puritySieieBins];
      const int divisions = 1;
      float sig_shift = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_puritySignalShift];
      float bkg_shift = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_purityBackgroundShift];

      TH1D* hdata = new TH1D(Form("hdata_%zu_%zu", i, j), "", nbins, 0, 0.025);
      data_tree->Project(hdata->GetName(), "phoSigmaIEtaIEta_2012[phoIdx]", candidate_cut, "");
      std::cout << "data count: " << hdata->GetEntries() << std::endl;

      TH1D* hbkg = new TH1D(Form("hbkg_%zu_%zu", i, j), "", nbins * divisions, 0, 0.025);
      data_tree->Project(hbkg->GetName(), Form("phoSigmaIEtaIEta_2012[phoIdx] + %f", bkg_shift), sideband_cut, "");
      TH1D* hsig = new TH1D(Form("hsig_%zu_%zu", i, j), "", nbins * divisions, 0, 0.025);
      mc_tree->Project(hsig->GetName(), Form("phoSigmaIEtaIEta_2012[phoIdx] + %f", sig_shift), signal_cut, "");
      std::cout << "bkg count: " << hbkg->GetEntries() << std::endl;
      std::cout << "sig count: " << hsig->GetEntries() << std::endl;

      float range_low = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_purityFitLow];
      float range_high = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_purityFitHigh];
      float sieie_cut = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_puritySieieThreshold];

      /* fit using binned templates */
      Templates* templates = new Templates(hdata, hsig, hbkg, divisions);

      Purity* purity = new Purity();
      purity->fit(templates, hdata, range_low, range_high, sieie_cut);

      std::cout << "nsig: " << purity->nsig << std::endl;
      std::cout << "purity: " << purity->purity << std::endl;
      std::cout << "chisq: " << purity->chisq << std::endl;
      std::cout << "ndf: " << purity->ndf << std::endl;

      purity->write();

      purity_values[i][j] = purity->purity;
    }
  }

  output->Close();

  for (std::size_t j=0; j<npt; ++j)
    for (std::size_t i=0; i<ncent; ++i)
      printf("%f, ", purity_values[i][j]);
  printf("\n");

  return 0;
}

int main(int argc, char* argv[]) {
  if (argc == 5)
    return PhotonPurity(argv[1], argv[2], argv[3], argv[4]);

  return 1;
}
