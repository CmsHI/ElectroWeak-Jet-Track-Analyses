#ifndef PhotonPurity_h
#define PhotonPurity_h

#include <TH1.h>
#include <TNtuple.h>
#include <TCut.h>
#include <TF1.h>
#include <string>

#include "../Utilities/interface/CutConfigurationParser.h"

class PhotonPurity {
public:
  Double_t nSig;
  Double_t nSigErr;
  Double_t purity;
  Double_t chisq;
  Double_t sigMeanShift;

  TH1F *sigPdf;
  TH1F *bckPdf;
  TH1D *data;
};

class histFunction2
{
public:
  histFunction2(TH1D *h, TH1D *h2);
  ~histFunction2(){
    delete histBck;
    delete histSig;
  };

  Double_t evaluate(Double_t *x, Double_t *par);
  TH1D *histSig;
  TH1D *histBck;
  Double_t lowEdge;
  Double_t highEdge;
  Double_t nbin;
};

histFunction2::histFunction2(TH1D *h,TH1D *h2)
{
  histSig=(TH1D*)h->Clone();
  histBck=(TH1D*)h2->Clone();

  nbin=h->GetNbinsX();
  lowEdge=h->GetBinLowEdge(1);
  highEdge=h->GetBinLowEdge(nbin+1);

  histSig->SetName("hSig");
  histSig->Scale(1./histSig->Integral(1,histSig->GetNbinsX()+1));
  histBck->SetName("hBck");
  histBck->Scale(1./histBck->Integral(1,histBck->GetNbinsX()+1));
}

Double_t histFunction2::evaluate(Double_t *x, Double_t *par) {

  Double_t xx = x[0];
  Int_t binNum=histSig->FindBin(xx);

  return par[0]*(histSig->GetBinContent(binNum)*par[1]+histBck->GetBinContent(binNum)*(1-par[1]));
}


PhotonPurity doFit(CutConfiguration config, TH1D* hSig=0, TH1D* hBkg=0, TH1D* hData1=0)
{
  Float_t varLow = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_purityFitLow];
  Float_t varHigh = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_purityFitHigh];
  Float_t purityBinVal = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_puritySieieThreshold];

  TH1D* hDatatmp = (TH1D*)hData1->Clone(Form("%s_datatmp",hData1->GetName()));
  Int_t nBins = hDatatmp->GetNbinsX();
  histFunction2 *myFits = new histFunction2(hSig,hBkg);
  TF1 *f = new TF1("f",myFits,&histFunction2::evaluate,varLow,varHigh,2);
  f->SetParameters( hDatatmp->Integral(1,nBins+1), 0.3);
  f->SetParLimits(1,0,1);
  hDatatmp->Fit("f","WL M 0 Q","",varLow,varHigh);
  hDatatmp->Fit("f","WL M 0 Q","",varLow,varHigh);

  PhotonPurity res;
  res.nSig =0;
  Double_t nev = f->GetParameter(0);
  Double_t ratio = f->GetParameter(1);
  Double_t ratioErr = f->GetParError(1);
  res.nSig    = nev * ratio;
  res.nSigErr = nev * ratioErr;
  res.chisq = (Double_t)f->GetChisquare()/ f->GetNDF();

  TH1F *hSigPdf = (TH1F*)hSig->Clone(Form("%s_tmp",hSig->GetName()));
  hSigPdf->Scale(res.nSig/hSigPdf->Integral(1,nBins+1));

  TH1F *hBckPdf = (TH1F*)hBkg->Clone(Form("%s_tmp",hBkg->GetName()));
  hBckPdf->Scale((nev-res.nSig)/hBckPdf->Integral(1,nBins+1));

  Double_t ss1 = hSigPdf->Integral(1, hSigPdf->FindBin(purityBinVal),"width");
  Double_t bb1 = hBckPdf->Integral(1, hBckPdf->FindBin(purityBinVal),"width");
  res.purity = ss1/(ss1+bb1);

  res.sigPdf = (TH1F*)hSigPdf->Clone(Form("%s_sig",hSig->GetName()));
  res.bckPdf = (TH1F*)hBckPdf->Clone(Form("%s_bck",hBkg->GetName()));
  res.data = (TH1D*)hData1->Clone(Form("%s_cand",hData1->GetName()));

  return res;
}

PhotonPurity getPurity(CutConfiguration config ,TTree *dataTree, TTree *mcTree,
		       TCut dataCandidateCut, TCut sidebandCut,
		       TCut mcSignalCut)
{
  Float_t signalShift = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_puritySignalShift];
  Float_t backgroundShift = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_purityBackgroundShift];
  const Int_t nSIGMABINS = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].i[CUTS::PHO::k_puritySieieBins];
  const Float_t maxSIGMA = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_puritySieieHistMax];
  const std::string mcWeightLabel_s = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_monteCarloWeightLabel];
  TCut mcWeightLabel = mcWeightLabel_s.c_str();

  TH1D* hCand = new TH1D("cand","",nSIGMABINS,0,maxSIGMA);
  TH1D* hBkg = (TH1D*)hCand->Clone("bkg");
  TH1D* hSig = (TH1D*)hCand->Clone("sig");

  TString sigshift = "+";
  sigshift += signalShift;
  TString bkgshift = "+";
  bkgshift += backgroundShift;

  dataTree->Project(hCand->GetName(), "phoSigmaIEtaIEta", dataCandidateCut, "");
  dataTree->Project(hBkg->GetName(), "phoSigmaIEtaIEta"+bkgshift, sidebandCut, "");
  mcTree->Project(hSig->GetName(), "phoSigmaIEtaIEta"+sigshift, mcWeightLabel*mcSignalCut, "");

  PhotonPurity fitr = doFit(config, hSig, hBkg, hCand);

  delete hSig;
  delete hBkg;
  delete hCand;

  fitr.sigMeanShift = signalShift;

  return fitr;
}

#endif
