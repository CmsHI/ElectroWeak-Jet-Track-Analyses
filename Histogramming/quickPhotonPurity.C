// photonPurity.C
// Author: Alex Barbieri
// Cobbled together from code written by Yongsun Kim

// Calculates and plots the photon purity using a sideband estimation
// of the background and a MC calculation of the signal. This info is
// used to do a template fit in the sigmaIetaIeta variable.

#include "TFile.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TH1D.h"
#include "TCut.h"
#include "TProfile.h"
#include "TGraphAsymmErrors.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TString.h"
#include "TLatex.h"
#include "stdio.h"
#include <iostream>
#include "PhotonPurity.h"
#include "../Utilities/interface/CutConfigurationParser.h"
#include "../TreeHeaders/CutConfigurationTree.h"
#include "../Plotting/commonUtility.h"

const TString LABEL = "pp #sqrt{s}_{_{NN}}=5.02 TeV";
const TCut sampleIsolation_PbPb = "(pho_ecalClusterIsoR4[phoIdx] + pho_hcalRechitIsoR4[phoIdx] + pho_trackIsoR4PtCut20[phoIdx]) < 1.0 && phoHoverE[phoIdx]<0.1";
const TCut noiseCut = "!((phoE3x3[phoIdx]/phoE5x5[phoIdx] > 2/3-0.03 && phoE3x3[phoIdx]/phoE5x5[phoIdx] < 2/3+0.03) && (phoE1x5[phoIdx]/phoE5x5[phoIdx] > 1/3-0.03 && phoE1x5[phoIdx]/phoE5x5[phoIdx] < 1/3+0.03) && (phoE2x5[phoIdx]/phoE5x5[phoIdx] > 2/3-0.03 && phoE2x5[phoIdx]/phoE5x5[phoIdx] < 2/3+0.03))";

const Double_t sigShifts[] = {0, 0, 0, 0};

// last entry is upper bound on last bin
//const Int_t CENTBINS[] = {0, 20, 60, 100, 200};
const Int_t CENTBINS[] = {-1,200};
const Int_t nCENTBINS = sizeof(CENTBINS)/sizeof(Int_t) -1;

const Double_t PTBINS[] = {40, 50, 60, 80, 1000};
const Int_t nPTBINS = sizeof(PTBINS)/sizeof(Double_t) -1;

const Double_t ETABINS[] = {-1.44, 1.44};
const Int_t nETABINS = sizeof(ETABINS)/sizeof(Double_t) -1;

void quickPhotonPurity(const TString configFile, const TString inputData, const TString inputMC, const TString outputName)
{
  TH1::SetDefaultSumw2();

  CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());
  if(!configCuts.isValid){
    std::cout << "Invalid configfile, terminating." << std::endl;
    return;
  }
  TTree *configTree = setupConfigurationTreeForWriting(configCuts);

  std::string jetCollection = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kJET].s[CUTS::JET::k_jetCollection];
  
  // isolation for PP
  float cut_phoHOverE_EB=0;         // Barrel
  float cut_pfcIso4_EB=0;
  float cut_pfnIso4_c0_EB=0;
  float cut_pfnIso4_c1_EB=0;
  float cut_pfnIso4_c2_EB=0;
  float cut_pfpIso4_c0_EB=0;
  float cut_pfpIso4_c1_EB=0;
  // float cut_phoHOverE_EE;         // Endcap
  // float cut_pfcIso4_EE;
  // float cut_pfnIso4_c0_EE;
  // float cut_pfnIso4_c1_EE;
  // float cut_pfnIso4_c2_EE;
  // float cut_pfpIso4_c0_EE;
  // float cut_pfpIso4_c1_EE;

  bool usePPstyleIso = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].i[CUTS::PHO::k_usePPstyleIso];
  if(usePPstyleIso){
    std::cout << "Using pp-style isolation." << std::endl;
  } else { 
    std::cout << "Using PbPb-style isolation." << std::endl;
  }
  if(usePPstyleIso){
    // Barrel
    cut_phoHOverE_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_phoHOverE_EB];
    cut_pfcIso4_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pfcIso4_EB];
    cut_pfnIso4_c0_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pfnIso4_c0_EB];
    cut_pfnIso4_c1_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pfnIso4_c1_EB];
    cut_pfnIso4_c2_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pfnIso4_c2_EB];
    cut_pfpIso4_c0_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pfpIso4_c0_EB];
    cut_pfpIso4_c1_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pfpIso4_c1_EB];
    // // Endcap
    // cut_phoHOverE_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_phoHOverE_EE];
    // cut_pfcIso4_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pfcIso4_EE];
    // cut_pfnIso4_c0_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pfnIso4_c0_EE];
    // cut_pfnIso4_c1_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pfnIso4_c1_EE];
    // cut_pfnIso4_c2_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pfnIso4_c2_EE];
    // cut_pfpIso4_c0_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pfpIso4_c0_EE];
    // cut_pfpIso4_c1_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pfpIso4_c1_EE];
  }

  TFile *input = TFile::Open(inputData);
  TTree *tHlt = (TTree*)input->Get("hltTree");
  TTree *tPho = (TTree*)input->Get("EventTree");    // photons
  TTree *tgj  = (TTree*)input->Get(Form("gamma_%s", jetCollection.c_str()));
  TTree *tHiEvt = (TTree*)input->Get("HiEvt");       // HiEvt tree will be placed in PP forest as well.

  TFile *inputMCFile = TFile::Open(inputMC);
  TTree *tmcHlt = (TTree*)inputMCFile->Get("hltTree");
  TTree *tmcPho = (TTree*)inputMCFile->Get("EventTree");    // photons
  TTree *tmcgj  = (TTree*)inputMCFile->Get(Form("gamma_%s", jetCollection.c_str()));
  TTree *tmcHiEvt = (TTree*)inputMCFile->Get("HiEvt");       // HiEvt tree will be placed in PP forest as well

  tgj->AddFriend(tHlt, "Hlt");
  tgj->AddFriend(tPho, "Pho");
  tgj->AddFriend(tHiEvt, "HiEvt");

  tmcgj->AddFriend(tmcHlt, "Hlt");
  tmcgj->AddFriend(tmcPho, "Pho");
  tmcgj->AddFriend(tmcHiEvt, "HiEvt");

  TFile *outFile = new TFile(Form("%s.root",outputName.Data()),"RECREATE");

  const TCut sidebandIsolation = "((pho_ecalClusterIsoR4[phoIdx] + pho_hcalRechitIsoR4[phoIdx] + pho_trackIsoR4PtCut20[phoIdx])>10) && ((pho_ecalClusterIsoR4[phoIdx] + pho_hcalRechitIsoR4[phoIdx] + pho_trackIsoR4PtCut20[phoIdx])<20) && phoHoverE[phoIdx]<0.1";
  const TCut mcIsolation = "(pho_genMatchedIndex[phoIdx]!= -1) && mcCalIsoDR04[pho_genMatchedIndex[phoIdx]]<5 && abs(mcPID[pho_genMatchedIndex[phoIdx]])<=22";

  //TCanvas *cPurity[nPTBINS];
  //TCanvas *cPurity = new TCanvas("c1","c1",337*nPTBINS,300*nCENTBINS/**2*/);
  TCanvas *cPurity = new TCanvas("c1","c1",400*nPTBINS,400*nCENTBINS);
  //cPurity->Divide(nPTBINS,2*nCENTBINS,0,0);
  //cPurity->Divide(nPTBINS,nCENTBINS,0,0);
  makeMultiPanelCanvas(cPurity, nPTBINS, nCENTBINS, 0.0, 0.0 , 0.2, 0.15, 0.005);
  for(Int_t i = 0; i < nPTBINS; ++i) {
    //cPurity[i] = new TCanvas(Form("c1_%d",i),"",1920,1000);
    //cPurity[i]->Divide(nETABINS,2,0,0);
    for(Int_t j = 0; j < nCENTBINS; ++j) {
      for(Int_t k = 0; k< nETABINS; ++k) {
	TString ptCut = Form("(phoEt[phoIdx] >= %f) && (phoEt[phoIdx] < %f)",
			     PTBINS[i], PTBINS[i+1]);
	TString centCut = Form("((hiBin[phoIdx]) >= %i) && ((hiBin[phoIdx]) < %i)",
			     CENTBINS[j], CENTBINS[j+1]);
	TString etaCut = Form("(phoEta[phoIdx] >= %f) && (phoEta[phoIdx] < %f)",
			      ETABINS[k], ETABINS[k+1]);

	TCut sampleIsolation;
	if(usePPstyleIso){
	  TCut selectionIso_ppstyle_EB = "";
	  std::string cut_pfpIso4_EB_str = Form("(%f + %f * phoEt[phoIdx])", cut_pfpIso4_c0_EB, cut_pfpIso4_c1_EB);
	  std::string cut_pfnIso4_EB_str = Form("(%f + %f * phoEt[phoIdx] + %f * phoEt[phoIdx]*phoEt[phoIdx])",
						cut_pfnIso4_c0_EB, cut_pfnIso4_c1_EB, cut_pfnIso4_c2_EB);
	  selectionIso_ppstyle_EB = selectionIso_ppstyle_EB && Form("phoHoverE[phoIdx] < %f", cut_phoHOverE_EB);
	  selectionIso_ppstyle_EB = selectionIso_ppstyle_EB && Form("pfcIso4[phoIdx] < %f", cut_pfcIso4_EB);
	  selectionIso_ppstyle_EB = selectionIso_ppstyle_EB && Form("pfnIso4[phoIdx] < %s", cut_pfnIso4_EB_str.c_str());
	  selectionIso_ppstyle_EB = selectionIso_ppstyle_EB && Form("pfpIso4[phoIdx] < %s", cut_pfpIso4_EB_str.c_str());
	  sampleIsolation = selectionIso_ppstyle_EB;
	} else {
	  sampleIsolation = sampleIsolation_PbPb;
	}
	
	TCut dataCandidateCut = sampleIsolation && etaCut && ptCut && centCut && noiseCut;
	TCut sidebandCut =  sidebandIsolation && etaCut && ptCut && centCut && noiseCut;
	TCut mcSignalCut = dataCandidateCut && mcIsolation;

	// if(nETABINS != 1)
	// {
	//   dataCandidateCut = sampleIsolation && pPbflipetaCut && ptCut && centCut;
	//   sidebandCut =  sidebandIsolation && pPbflipetaCut && ptCut && centCut;
	//   mcSignalCut =  sampleIsolation && etaCut && ptCut && centCut && mcIsolation;
	// }

	PhotonPurity fitr = getPurity(configCuts, tgj, tmcgj,
				      dataCandidateCut, sidebandCut,
				      mcSignalCut);

	//cPurity[i*nCENTBINS+j] = new TCanvas(Form("cpurity%d",i*nCENTBINS+j),
	// 					 "",500,500);
	cPurity->cd((k+j)*nPTBINS+i+1);
	//cPurity->cd((k+j)*nPTBINS+i+1);
	//cPurity[i]->cd(k+1);

	TH1F *hSigPdf = fitr.sigPdf;
	TH1F *hBckPdf = fitr.bckPdf;
	TH1D *hData1  = fitr.data;
	hSigPdf->Add(hBckPdf);

	TString name = "mcfit_total_ptbin";
	name += i;

	// outFile->cd();
	// hSigPdf->SetName(name);
	// hSigPdf->Write();


	// TH1D *err = (TH1D*)hSigPdf->Clone("error");
	// TH1D *tempErr[4];
	// err->Reset();
	// for(int s = 0; s < 4; s++)
	// {
	//   if(s == 0)
	//     tempErr[s] = (TH1D*)TFile::Open("photonPurity_sys_loose.root")->Get(name);
	//   else if(s ==1)
	//     tempErr[s] = (TH1D*)TFile::Open("photonPurity_sys_tight.root")->Get(name);
	//   else if(s ==2)
	//     tempErr[s] = (TH1D*)TFile::Open("photonPurity_sys_sigshift.root")->Get(name);
	//   else if(s ==3)
	//     tempErr[s] = (TH1D*)TFile::Open("photonPurity_sys_bkgshift.root")->Get(name);
	//   tempErr[s]->Divide(hSigPdf);
	//   for (Int_t l=1; l<=tempErr[s]->GetNbinsX();l++)
	//   {
	//     tempErr[s]->SetBinContent(l, TMath::Abs(tempErr[s]->GetBinContent(l))-1);
	//   }
	// }
	// for (Int_t l=1; l<=err->GetNbinsX();l++)
	// {
	//   Double_t errVal = TMath::Sqrt(tempErr[0]->GetBinContent(l)*tempErr[0]->GetBinContent(l) +
	// 				tempErr[1]->GetBinContent(l)*tempErr[1]->GetBinContent(l) +
	// 				tempErr[2]->GetBinContent(l)*tempErr[2]->GetBinContent(l) +
	// 				tempErr[3]->GetBinContent(l)*tempErr[3]->GetBinContent(l)
	//     );
	//   err->SetBinContent(l, errVal);
	// }

	// plot stacked histos
	handsomeTH1(hSigPdf);
	mcStyle(hSigPdf);
	sbStyle(hBckPdf);
	cleverRange(hSigPdf,1.5);
	hSigPdf->SetAxisRange(0.001,0.024,"X");
	hSigPdf->SetNdivisions(505);
	hSigPdf->GetYaxis()->SetTitleOffset(1.75);
	hSigPdf->SetYTitle("Entries");
	hSigPdf->SetXTitle("#sigma_{#eta #eta}");

	hSigPdf->DrawCopy("hist");
	//drawSys(hSigPdf, err, kRed, -1, 0.001);
	hBckPdf->DrawCopy("same hist");
	hData1->DrawCopy("same");

	Float_t xpos = 0.44;
	if(2*(k+j)*nPTBINS+i+1 == 1)
	  xpos = 0.54;

	TLegend *t3=new TLegend(xpos, 0.45, 0.92, 0.71);
	t3->AddEntry(hData1,LABEL,"pl");
	t3->AddEntry(hSigPdf,"Signal","lf");
	t3->AddEntry(hBckPdf,"Background","lf");
	t3->SetFillColor(0);
	t3->SetBorderSize(0);
	t3->SetFillStyle(0);
	t3->SetTextFont(43);
	t3->SetTextSize(20);
	//if(i == 0)
	// TH1D *dummyHist = new TH1D("dummyHist","",10,0,10);
	// dummyHist->Fill(1);
	// dummyHist->SetFillColor(kRed);
	// dummyHist->SetLineColor(kRed);
	// dummyHist->SetFillStyle(1001);
	// t3->AddEntry(dummyHist,"MC Sys. Error","f");
	// if(i == 0)
	//   t3->Draw();

	if(i == 3)
	{
	  drawText("CMS Preliminary", xpos, 0.68,1,20);
	  drawText(LABEL, xpos, 0.60,1,20);
	  //drawText("#intL = 150 #mub^{-1}", xpos, 0.50,1,20);
	}



	//drawText("|#eta_{#gamma}| < 1.479",0.5680963,0.9);
	//drawText(Form("%f shift",fitr.sigMeanShift),0.57,0.82);
	//drawText("Background Correction",0.57,0.82);
	//drawText("bkg Tighter",0.57,0.82);
	//if(nPTBINS != 1)
	drawText(Form("%.0f GeV < p_{T}^{#gamma} < %.0f GeV",
		      PTBINS[i], PTBINS[i+1]),
		 xpos, 0.90,1,20);
	// if(/*nCENTBINS != 1 && */i ==0)
	// drawText(Form("%.0f - %.0f%c",
	// 	      CENTBINS[j]/2., CENTBINS[j+1]/2.,'%'),
	// 	 xpos, 0.68,1,20);
	// if(nETABINS != 1)
	// drawText(Form("%.3f < #eta_{#gamma} < %.3f",
	// 	      ETABINS[k], ETABINS[k+1]),
	// 	 xpos, 0.82,1,20);
	drawText("10<sideBand<20",
		 xpos, 0.82,1,20);
	drawText(Form("Purity (#sigma_{#eta#eta} < 0.01) : %.2f", (Float_t)fitr.purity),
		 xpos, 0.76,1,20);
	drawText(Form("#chi^{2}/ndf : %.2f", (Float_t)fitr.chisq),
		 xpos, 0.45,1,20);


	// //plot ratio
	// cPurity->cd((2*(j+k)+1)*nPTBINS+i+1);
	// //cPurity[i]->cd(nETABINS + k+ 1);
	// TH1D* ratio = (TH1D*)hData1->Clone("ratio");
	// ratio->Divide(hData1, hSigPdf, 1, 1);
	// ratio->SetMinimum(0);
	// ratio->SetMaximum(3);
	// ratio->SetXTitle("#sigma_{#eta #eta}");
	// ratio->GetXaxis()->CenterTitle();
	// ratio->SetYTitle("Data/Fit");
	// ratio->GetYaxis()->CenterTitle();
	// ratio->DrawCopy("E");
	// TLine *line = new TLine(0,1,maxSIGMA,1);
	// line->SetLineStyle(2);
	// line->Draw("same");

	// TString savename = Form("purity_pA_barrel_pt%.0f_hf%.0f_plot",
	// 			PTBINS[i], CENTBINS[j]);
	// cPurity[i*nCENTBINS+j]->SaveAs(savename+".C");
	// cPurity[i*nCENTBINS+j]->SaveAs(savename+".pdf");
	// cPurity[i*nCENTBINS+j]->SaveAs(savename+".png");

      }
    }
    //cPurity[i]->SaveAs(Form("pPb_purity_etadep_wshift_ptbin%.0f.png",PTBINS[i]));
    //cPurity[i]->SaveAs(Form("pPb_purity_etadep_noshift_inclusive.png"));
  }
  outFile->cd();
  configTree->Write();
  cPurity->Write();
  outFile->Close();
  //cPurity->SaveAs(SAVENAME+".C");
  cPurity->SaveAs(Form("%s.png", outputName.Data()));
  //cPurity->SaveAs(SAVENAME+".pdf");
}

int main(int argc, char **argv)
{
  if(argc == 5){
    quickPhotonPurity(argv[1], argv[2], argv[3], argv[4]);
  return 0;
  } else {
    return 1;
  }
}
