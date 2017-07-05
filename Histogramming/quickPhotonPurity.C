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

const Int_t CENTBINS[] = {60, 100};
const Int_t nCENTBINS = sizeof(CENTBINS)/sizeof(Int_t) -1;

const Double_t PTBINS[] = {40, 50, 60, 80, 100, 9999};
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

  const TCut sidebandIsolation = "((pho_sumIsoCorrected[phoIdx])>10) && ((pho_sumIsoCorrected[phoIdx])<20) && phoHoverE[phoIdx]<0.1";
  const TCut mcIsolation = "(pho_genMatchedIndex[phoIdx]!= -1) && mcCalIsoDR04[pho_genMatchedIndex[phoIdx]]<5 && abs(mcPID[pho_genMatchedIndex[phoIdx]])<=22";

  TCanvas* cPurity = new TCanvas("c1", "c1", 400*nPTBINS, 400*nCENTBINS);
  makeMultiPanelCanvas(cPurity, nPTBINS, nCENTBINS, 0.0, 0.0 , 0.2, 0.15, 0.005);
  for (Int_t i = 0; i < nPTBINS; ++i) {
    for (Int_t j = 0; j < nCENTBINS; ++j) {
      for (Int_t k = 0; k < 1; ++k) {
        TString ptCut = Form("(phoEtCorrected[phoIdx] >= %f) && (phoEtCorrected[phoIdx] < %f)",
                             PTBINS[i], PTBINS[i+1]);
        TString centCut = Form("(hiBin >= %i) && (hiBin < %i)",
                               CENTBINS[j], CENTBINS[j+1]);
        TString etaCut = "(abs(phoEta[phoIdx]) < 1.44)";

        TCut triggerCut = Form("%s == 1", trigger.c_str());
        TCut triggerCutMC =  Form("%s == 1", triggerMC_forPurity.c_str());

        const TCut sampleIsolation = "(pho_sumIsoCorrected[phoIdx]) < 1.0 && phoHoverE[phoIdx]<0.1";

        TCut dataCandidateCut = sampleIsolation && etaCut && ptCut && centCut && noiseCut && triggerCut;
        TCut sidebandCut =  sidebandIsolation && etaCut && ptCut && centCut && noiseCut && triggerCut;
        TCut mcSignalCut =  mcIsolation && etaCut && ptCut && centCut && triggerCutMC;

        PhotonPurity fitr = getPurity(configCuts, tgj, tmcgj,
                                      dataCandidateCut, sidebandCut,
                                      mcSignalCut);

        cPurity->cd((k+j)*nPTBINS+i+1);

        TH1F* hSigPdf = fitr.sigPdf;
        TH1F* hBckPdf = fitr.bckPdf;
        TH1D* hData1  = fitr.data;
        hSigPdf->Add(hBckPdf);

        TString name = "mcfit_total_ptbin";
        name += i;

        // outFile->cd();
        // hSigPdf->SetName(name);
        // hSigPdf->Write();

        // TH1D* err = (TH1D*)hSigPdf->Clone("error");
        // TH1D* tempErr[4];
        // err->Reset();
        // for (int s = 0; s < 4; s++) {
        //   if (s == 0)
        //     tempErr[s] = (TH1D*)TFile::Open("photonPurity_sys_loose.root")->Get(name);
        //   else if (s == 1)
        //     tempErr[s] = (TH1D*)TFile::Open("photonPurity_sys_tight.root")->Get(name);
        //   else if (s == 2)
        //     tempErr[s] = (TH1D*)TFile::Open("photonPurity_sys_sigshift.root")->Get(name);
        //   else if (s == 3)
        //     tempErr[s] = (TH1D*)TFile::Open("photonPurity_sys_bkgshift.root")->Get(name);
        //   tempErr[s]->Divide(hSigPdf);
        //   for (Int_t l=1; l<=tempErr[s]->GetNbinsX(); l++) {
        //     tempErr[s]->SetBinContent(l, TMath::Abs(tempErr[s]->GetBinContent(l))-1);
        //   }
        // }
        // for (Int_t l=1; l<=err->GetNbinsX(); l++) {
        //   Double_t errVal = TMath::Sqrt(
        //     tempErr[0]->GetBinContent(l) * tempErr[0]->GetBinContent(l) +
        //     tempErr[1]->GetBinContent(l) * tempErr[1]->GetBinContent(l) +
        //     tempErr[2]->GetBinContent(l) * tempErr[2]->GetBinContent(l) +
        //     tempErr[3]->GetBinContent(l) * tempErr[3]->GetBinContent(l)
        //   );
        //   err->SetBinContent(l, errVal);
        // }

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
        // drawSys(hSigPdf, err, kRed, -1, 0.001);
        hBckPdf->DrawCopy("same hist");
        hData1->DrawCopy("same");

        hSigPdf->Write(Form("signal_template_%i_%i", i, j), TObject::kOverwrite);
        hBckPdf->Write(Form("background_template_%i_%i", i ,j), TObject::kOverwrite);
        hData1->Write(Form("data_points_%i_%i", i, j), TObject::kOverwrite);

        Float_t xpos = 0.60;
        if (2*(k+j)*nPTBINS+i+1 == 1)
          xpos = 0.6;
        if (i == 0) xpos = 0.66;

        if (i == 0) {
          // drawText("CMS Preliminary", 0.225, 0.90, 1, 27);

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

          //drawText(LABEL, xpos, 0.50, 1, 27);
          //drawText(Form("%.0f - %.0f%c", CENTBINS[j]/2., CENTBINS[j+1]/2., '%'), xpos, 0.40, 1, 27);
        }

        if (i == 0) {
            drawText(Form("p_{T}^{#gamma} #in (%.0f, %.0f) GeV/c", PTBINS[i], PTBINS[i+1]), 0.50, 0.90, 1, 27);
        }
        else if (i == 1 || i == 2) {
          drawText(Form("p_{T}^{#gamma} #in (%.0f, %.0f) GeV/c", PTBINS[i], PTBINS[i+1]), 0.40, 0.90, 1, 27);
        }
        else if (i == 3) {
          drawText(Form("p_{T}^{#gamma} #in (%.0f, %.0f) GeV/c", PTBINS[i], PTBINS[i+1]), 0.36, 0.90, 1, 27);
        } else {
          drawText(Form("p_{T}^{#gamma} > %.0f GeV/c", PTBINS[i]), 0.50, 0.90, 1, 27);
        }
        drawText(Form("Purity : %.2f", (Float_t)fitr.purity), xpos, 0.80, 1, 27);
        drawText(Form("#chi^{2}/ndf : %.2f", (Float_t)fitr.chisq), xpos, 0.70, 1, 27);
      }
    }
  }

  TH1D* permaCopy[3];
  permaCopy[0] = new TH1D("copy0", "", 1, 0, 1);
  permaCopy[1] = new TH1D("copy1", "", 1, 0, 1);
  permaCopy[2] = new TH1D("copy2", "", 1, 0, 1);
  handsomeTH1(permaCopy[0]);
  mcStyle(permaCopy[0]);
  sbStyle(permaCopy[1]);

  cPurity->cd(1);
  TLegend* t3 = new TLegend(0.54, 0.36, 0.84, 0.66);
  TLegendEntry* ent1 = t3->AddEntry(permaCopy[2], Form("%s Cent %d-%d%%", LABEL.Data(), CENTBINS[0]/2, CENTBINS[1]/2), "pl");
  TLegendEntry* ent2 = t3->AddEntry(permaCopy[0], "Signal", "lf");
  TLegendEntry* ent3 = t3->AddEntry(permaCopy[1], "Background", "lf");

  ent1->SetMarkerStyle(20);
  ent2->SetLineColor(kPink);
  ent2->SetFillColor(kOrange+7);
  ent2->SetFillStyle(3004);
  ent3->SetLineColor(kGreen+4);
  ent3->SetFillColor(kGreen+1);
  ent3->SetFillStyle(3001);

  t3->SetFillColor(0);
  t3->SetBorderSize(0);
  t3->SetFillStyle(0);
  t3->SetTextFont(43);
  t3->SetTextSize(22);
  t3->Draw();

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
