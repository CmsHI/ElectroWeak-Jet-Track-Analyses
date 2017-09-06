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
#include "stdio.h"
#include <iostream>

#include "PhotonPurity.h"

#include "../Utilities/interface/CutConfigurationParser.h"
#include "../Plotting/commonUtility.h"

const TString LABEL = "PbPb";
const TCut noiseCut = "!((phoE3x3[phoIdx]/phoE5x5[phoIdx] > 2./3.-0.03 && phoE3x3[phoIdx]/phoE5x5[phoIdx] < 2./3.+0.03) && (phoE1x5[phoIdx]/phoE5x5[phoIdx] > 1./3.-0.03 && phoE1x5[phoIdx]/phoE5x5[phoIdx] < 1./3.+0.03) && (phoE2x5[phoIdx]/phoE5x5[phoIdx] > 2./3.-0.03 && phoE2x5[phoIdx]/phoE5x5[phoIdx] < 2./3.+0.03))";

const Int_t CENTBINS[] = {0, 20, 60, 100, 200};
const Int_t nCENTBINS = sizeof(CENTBINS)/sizeof(Int_t) -1;

const Double_t PTBINS[] = {60, 9999};
const Int_t nPTBINS = sizeof(PTBINS)/sizeof(Double_t) -1;

int quickPhotonPurity(const TString configFile, const TString inputData, const TString inputMC, const TString outputName) {
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
  std::string triggerMC_forPurity = "(HLT_HISinglePhoton40_Eta1p5_v2)";

  TFile* input = TFile::Open(inputData);
  TTree* tHlt = (TTree*)input->Get("hltTree");
  TTree* tPho = (TTree*)input->Get("EventTree");    // photons
  TTree* tgj  = (TTree*)input->Get(Form("gamma_%s", jetCollection.c_str()));
  TTree* tHiEvt = (TTree*)input->Get("HiEvt");       // HiEvt tree will be placed in PP forest as well.

  TFile* inputMCFile = TFile::Open(inputMC);
  TTree* tmcHlt = (TTree*)inputMCFile->Get("hltTree");
  TTree* tmcPho = (TTree*)inputMCFile->Get("EventTree");    // photons
  TTree* tmcgj  = (TTree*)inputMCFile->Get(Form("gamma_%s", jetCollection.c_str()));
  TTree* tmcHiEvt = (TTree*)inputMCFile->Get("HiEvt");       // HiEvt tree will be placed in PP forest as well

  tgj->AddFriend(tHlt, "Hlt");
  tgj->AddFriend(tPho, "Pho");
  tgj->AddFriend(tHiEvt, "HiEvt");

  tmcgj->AddFriend(tmcHlt, "Hlt");
  tmcgj->AddFriend(tmcPho, "Pho");
  tmcgj->AddFriend(tmcHiEvt, "HiEvt");

  TFile* outFile = new TFile(Form("%s.root", outputName.Data()), "RECREATE");

  const TCut sampleIsolation = "(pho_sumIsoCorrected[phoIdx]) < 1.0 && phoHoverE[phoIdx]<0.1";
  const TCut sidebandIsolation = "((pho_sumIsoCorrected[phoIdx])>10) && ((pho_sumIsoCorrected[phoIdx])<20) && phoHoverE[phoIdx]<0.1";
  const TCut mcIsolation = "(pho_genMatchedIndex[phoIdx]!= -1) && mcCalIsoDR04[pho_genMatchedIndex[phoIdx]]<5 && mcPID[pho_genMatchedIndex[phoIdx]] == 22 && (abs(mcMomPID[pho_genMatchedIndex[phoIdx]])<=22 || mcMomPID[pho_genMatchedIndex[phoIdx]] == -999)";

  TCanvas* cPurity = new TCanvas("c1", "c1", 400*nCENTBINS, 400*nPTBINS);
  makeMultiPanelCanvas(cPurity, nCENTBINS, nPTBINS, 0.0, 0.0 , 0.2, 0.15, 0.005);
  for (Int_t i = 0; i < nCENTBINS; ++i) {
    for (Int_t j = 0; j < nPTBINS; ++j) {
      TString ptCut = Form("(phoEtCorrected[phoIdx] >= %f) && (phoEtCorrected[phoIdx] < %f)",
          PTBINS[j], PTBINS[j+1]);
      TString centCut = Form("(hiBin >= %i) && (hiBin < %i)",
          CENTBINS[i], CENTBINS[i+1]);
      TString etaCut = "(abs(phoEta[phoIdx]) < 1.44)";

      TCut triggerCut = Form("%s == 1", trigger.c_str());
      TCut triggerCutMC = Form("%s == 1", triggerMC_forPurity.c_str());

      TCut dataCandidateCut = sampleIsolation && etaCut && ptCut && centCut && noiseCut && triggerCut;
      TCut sidebandCut = sidebandIsolation && etaCut && ptCut && centCut && noiseCut && triggerCut;
      TCut mcSignalCut = mcIsolation && etaCut && ptCut && centCut && triggerCutMC;

      PhotonPurity fitr = calc_purity(configCuts, tgj, tmcgj, dataCandidateCut, sidebandCut, mcSignalCut);

      cPurity->cd(j*nCENTBINS+i+1);

      TH1D* hSigPdf = fitr.sig_fit;
      TH1D* hBckPdf = fitr.bkg_fit;
      TH1D* hData1  = fitr.data;
      hSigPdf->Add(hBckPdf);

      TString name = "mcfit_total_ptbin";
      name += i;

      // plot stacked histos
      handsomeTH1(hSigPdf);
      mcStyle(hSigPdf);
      sbStyle(hBckPdf);
      cleverRange(hSigPdf, 1.5);

      hSigPdf->SetAxisRange(0.001, 0.024, "X");
      hSigPdf->SetNdivisions(505);
      hSigPdf->GetYaxis()->SetTitleOffset(1.30);
      hSigPdf->SetYTitle("Entries");
      hSigPdf->SetXTitle("#sigma_{#eta#eta}");
      hSigPdf->SetLabelFont(43);
      hSigPdf->SetLabelSize(27);
      hSigPdf->SetTitleFont(43);
      hSigPdf->SetTitleSize(27);
      hSigPdf->GetYaxis()->SetLabelFont(43);
      hSigPdf->GetYaxis()->SetLabelSize(27);
      hSigPdf->GetYaxis()->SetTitleFont(43);
      hSigPdf->GetYaxis()->SetTitleSize(27);
      hSigPdf->GetYaxis()->SetLabelOffset(0.005);
      hSigPdf->GetXaxis()->SetTitleOffset(1);

      hSigPdf->DrawCopy("hist");
      hBckPdf->DrawCopy("same hist");
      hData1->DrawCopy("same");

      hSigPdf->Write(Form("signal_template_%i_%i", i, j), TObject::kOverwrite);
      hBckPdf->Write(Form("background_template_%i_%i", i ,j), TObject::kOverwrite);
      hData1->Write(Form("data_points_%i_%i", i, j), TObject::kOverwrite);

      Float_t xpos = 0.60;

      if (i == 0) {
        xpos = 0.66;

        TLatex* latexCMS = new TLatex();
        latexCMS->SetTextFont(63);
        latexCMS->SetTextSize(40);
        latexCMS->DrawLatexNDC(0.24, 0.9, "CMS");

        bool isPreliminary = false;
        if (isPreliminary) {
          TLatex* latexPrelim = new TLatex();
          latexPrelim->SetTextFont(53);
          latexPrelim->SetTextSize(28);
          latexPrelim->DrawLatexNDC(0.4, 0.9, "Preliminary");
        }
      }

      drawText(Form("%i - %i %% Cent.", CENTBINS[i]/2, CENTBINS[i+1]/2), 0.50, 0.90, 1, 27);
      drawText(Form("Purity: %.2f", (Float_t)fitr.purity), xpos, 0.80, 1, 27);
      drawText(Form("#chi^{2}/ndf: %.0f/%.0f", (float)fitr.chisq, (float)fitr.ndf), xpos, 0.70, 1, 27);
      drawText(Form("#chi^{2}/ndf: %.2f", (Float_t)fitr.chisq), xpos, 0.60, 1, 27);
    }
  }

  TH1D* permaCopy[3];
  permaCopy[0] = new TH1D("copy0", "", 1, 0, 1);
  permaCopy[1] = new TH1D("copy1", "", 1, 0, 1);
  permaCopy[2] = new TH1D("copy2", "", 1, 0, 1);
  handsomeTH1(permaCopy[0]);
  mcStyle(permaCopy[0]);
  sbStyle(permaCopy[1]);

  outFile->cd();
  cPurity->Write();
  outFile->Close();

  // cPurity->SaveAs(Form("%s.png", outputName.Data()));
  cPurity->SaveAs(Form("%s.pdf", outputName.Data()));

  return 0;
}

int main(int argc, char* argv[]) {
  if (argc == 5)
    return quickPhotonPurity(argv[1], argv[2], argv[3], argv[4]);

  return 1;
}
