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
    Double_t rawchisq;
    Double_t ndf;

    TH1F* sigPdf;
    TH1F* bckPdf;
    TH1D* data;
};

class histFunction2 {
  public:
    histFunction2(TH1D* h, TH1D* h2);
    ~histFunction2() {
        delete histBck;
        delete histSig;
    };

    Double_t evaluate(Double_t* x, Double_t* par);
    TH1D* histSig;
    TH1D* histBck;
    Double_t lowEdge;
    Double_t highEdge;
    Double_t nbin;
};

histFunction2::histFunction2(TH1D* h, TH1D* h2) {
    histSig = (TH1D*)h->Clone();
    histBck = (TH1D*)h2->Clone();

    nbin = h->GetNbinsX();
    lowEdge = h->GetBinLowEdge(1);
    highEdge = h->GetBinLowEdge(nbin + 1);

    histSig->SetName("hSig");
    histSig->Scale(1. / histSig->Integral(1, histSig->GetNbinsX() + 1));
    histBck->SetName("hBck");
    histBck->Scale(1. / histBck->Integral(1, histBck->GetNbinsX() + 1));
}

Double_t histFunction2::evaluate(Double_t* x, Double_t* par) {
    Double_t xx = x[0];
    Int_t binNum = histBck->FindBin(xx);
    Int_t shiftedSigBinNum = histSig->FindBin(xx + par[2]);
    return par[0] * (histSig->GetBinContent(shiftedSigBinNum) * par[1] + histBck->GetBinContent(binNum) * (1 - par[1]));
}


PhotonPurity doFit(CutConfiguration config, TH1D* hSig = 0, TH1D* hBkg = 0, TH1D* hData1 = 0) {
    Float_t varLow = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_purityFitLow];
    Float_t varHigh = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_purityFitHigh];
    Float_t purityBinVal = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_puritySieieThreshold];

    TH1D* hDatatmp = (TH1D*)hData1->Clone(Form("%s_datatmp", hData1->GetName()));
    Int_t nBins = hDatatmp->GetNbinsX();
    histFunction2* myFits = new histFunction2(hSig, hBkg);
    TF1* f = new TF1("f", myFits, &histFunction2::evaluate, varLow, varHigh, 3);
    f->SetParameters(hDatatmp->Integral(1, nBins + 1), 0.72, 0.0);
    f->SetParLimits(1, 0, 1);
    f->FixParameter(2, 0.0);
    hDatatmp->Fit("f", "WL 0 Q", "", varLow, varHigh);
    hDatatmp->Fit("f", "WL 0 Q", "", varLow, varHigh);
    hDatatmp->Fit("f", "WL M 0 Q", "", varLow, varHigh);

    PhotonPurity res;
    res.nSig = 0;
    Double_t nev = f->GetParameter(0);
    Double_t ratio = f->GetParameter(1);
    Double_t shift = f->GetParameter(2);
    Double_t nevError = f->GetParError(0);
    Double_t ratioError = f->GetParError(1);
    Double_t shiftError = f->GetParError(2);

    std::cout << "nev: " << nev << " nevError: " << nevError << std::endl;
    std::cout << "ratio: " << ratio << " ratioError: " << ratioError << std::endl;
    std::cout << "shift: " << shift << " shiftError: " << shiftError << std::endl;

    res.nSig    = nev * ratio;
    res.nSigErr = nev * ratioError;
    res.chisq = (Double_t)f->GetChisquare() / f->GetNDF();
    res.rawchisq = f->GetChisquare();
    res.ndf = f->GetNDF();

    TH1F* hSigPdf = (TH1F*)hSig->Clone(Form("%s_tmp", hSig->GetName()));
    hSigPdf->Scale(res.nSig / hSigPdf->Integral(1, nBins + 1));

    TH1F* hBckPdf = (TH1F*)hBkg->Clone(Form("%s_tmp", hBkg->GetName()));
    hBckPdf->Scale((nev - res.nSig) / hBckPdf->Integral(1, nBins + 1));

    Double_t ss1 = hSigPdf->Integral(1, hSigPdf->FindBin(purityBinVal), "width");
    Double_t bb1 = hBckPdf->Integral(1, hBckPdf->FindBin(purityBinVal), "width");
    res.purity = ss1 / (ss1 + bb1);

    res.sigPdf = (TH1F*)hSigPdf->Clone(Form("%s_sig", hSig->GetName()));
    res.bckPdf = (TH1F*)hBckPdf->Clone(Form("%s_bck", hBkg->GetName()));
    res.data = (TH1D*)hData1->Clone(Form("%s_cand", hData1->GetName()));

    return res;
}

PhotonPurity getPurity(CutConfiguration config, TTree* dataTree, TTree* mcTree,
                       TCut dataCandidateCut, TCut sidebandCut,
                       TCut mcSignalCut, TTree* bkgmcTree = 0, int index = 0) {
    const Int_t nSIGMABINS = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].i[CUTS::PHO::k_puritySieieBins];
    const Float_t maxSIGMA = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_puritySieieHistMax];
    const std::string mcWeightLabel_s = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_monteCarloWeightLabel];
    TCut mcWeightLabel = mcWeightLabel_s.c_str();

    TH1D* hCand = new TH1D("cand", "", nSIGMABINS, 0, maxSIGMA);
    TH1D* hBkg = (TH1D*)hCand->Clone("bkg");
    TH1D* hSig = (TH1D*)hCand->Clone("sig");

    float signalShift[4] = {-0.00006, -0.00003, -0.00003, 0};
    float backgroundShift[4] = {0.00003, 0, 0.00001, 0};

    TString sigshift = "+";
    sigshift += signalShift[index];
    TString bkgshift = "+";
    bkgshift += backgroundShift[index];

    dataTree->Project(hCand->GetName(), "phoSigmaIEtaIEta_2012[phoIdx]", dataCandidateCut, "");
    if (bkgmcTree == 0) {
        dataTree->Project(hBkg->GetName(), "phoSigmaIEtaIEta_2012[phoIdx]" + bkgshift, sidebandCut, "");
    } else {
        bkgmcTree->Project(hBkg->GetName(), "phoSigmaIEtaIEta_2012[phoIdx]" + bkgshift, mcWeightLabel * sidebandCut, "");
    }
    mcTree->Project(hSig->GetName(), "phoSigmaIEtaIEta_2012[phoIdx]" + sigshift, mcWeightLabel * mcSignalCut, "");

    std::cout << "dataCount: " << hCand->GetEntries() << std::endl;
    std::cout << "bkgCount: " << hBkg->GetEntries() << std::endl;
    std::cout << "sigCount: " << hSig->GetEntries() << std::endl;

    PhotonPurity fitr = doFit(config, hSig, hBkg, hCand);

    std::cout << "Purity: " << fitr.purity << std::endl;
    std::cout << "nSig: " << fitr.nSig << std::endl;
    std::cout << "chisq: " << fitr.chisq << std::endl;
    std::cout << "rawchisq: " << fitr.rawchisq << std::endl;
    std::cout << "ndf: " << fitr.ndf << std::endl;

    delete hSig;
    delete hBkg;
    delete hCand;

    fitr.sigMeanShift = signalShift[index];

    return fitr;
}

#endif
