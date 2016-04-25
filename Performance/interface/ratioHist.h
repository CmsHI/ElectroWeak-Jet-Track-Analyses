/*
 * simple class for ratio and efficiency histograms
 */

#ifndef PERFORMANCE_INTERFACE_RATIOHIST_H_
#define PERFORMANCE_INTERFACE_RATIOHIST_H_

#include <TH1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TGraphAsymmErrors.h>
#include <TMath.h>

#include <string>

#include "../../Utilities/th1Util.h"

class ratioHist {
public :
    ratioHist(){

        name = "";
        hNumInitialized = false;
        hDenomInitialized = false;
        hRatioInitialized = false;
        graphAsymmErrorsInitialized = false;
    };
    ~ratioHist(){};

    void calcRatio();

    TH1D* hNum;
    TH1D* hDenom;
    TH1D* hRatio;
    TGraphAsymmErrors* a;

    bool hNumInitialized;
    bool hDenomInitialized;
    bool hRatioInitialized;
    bool graphAsymmErrorsInitialized;

    std::string name;   // this is basically histogram name excluding the "hNum"/"hDenom" prefix
    std::string title;
    std::string titleX;
};

void ratioHist::calcRatio()
{
    if (!hNumInitialized || !hDenomInitialized) return;

    if (graphAsymmErrorsInitialized) {
        a->Delete();
        graphAsymmErrorsInitialized = false;
    }

    a = new TGraphAsymmErrors();
    a->SetName(Form("a_%s", name.c_str()));
    a->BayesDivide(hNum, hDenom);
    a->SetTitle(title.c_str());
    a->GetXaxis()->SetTitle(titleX.c_str());

    graphAsymmErrorsInitialized = true;

    if (hRatioInitialized) {
        hRatio->Delete();
        hRatioInitialized = false;
    }

    hRatio = (TH1D*)Graph2Histogram(a);
    hRatio->SetName(Form("h_Ratio_%s", name.c_str()));
    hRatio->SetTitle(title.c_str());
    hRatio->SetXTitle(titleX.c_str());

    hRatioInitialized = true;
}

#endif
