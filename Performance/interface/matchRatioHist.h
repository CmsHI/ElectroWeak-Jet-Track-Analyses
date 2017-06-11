/*
 * general purpose class for matching efficiency and fake ratio histograms
 */

#ifndef PERFORMANCE_INTERFACE_MATCHRATIOHIST_H_
#define PERFORMANCE_INTERFACE_MATCHRATIOHIST_H_

#include <TH1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TGraphAsymmErrors.h>
#include <TMath.h>

#include <string>

#include "../../Utilities/th1Util.h"

namespace MATCHRATIO {

enum MATCHRATIO_DEP {   // matching efficiency / fake ratio dependencies
    kETA,
    kGENPT,
    kRECOPT,
    kHIBIN,
    kN_MATCHRATIO_DEP
};

const std::string MATCHRATIO_DEP_LABELS[kN_MATCHRATIO_DEP] = {
        "ETA",
        "GENPT",
        "RECOPT",
        "HIBIN"
};

};

class matchRatioHist {
public :
    matchRatioHist(){

        for (int i=0; i<MATCHRATIO::kN_MATCHRATIO_DEP; ++i) {
            ranges[i][0] = 0;
            ranges[i][1] = -1;  // no upper bound
        }

        name = "";
        hNumInitialized = false;
        hDenomInitialized = false;
        hRatioInitialized = false;
        graphAsymmErrorsInitialized = false;
    };
    ~matchRatioHist(){};

    void FillHNum(double x, float eta = -999, float genPt = -1, float recoPt = -1, int hiBin = -1);
    void FillHDenom(double x, float eta = -999, float genPt = -1, float recoPt = -1, int hiBin = -1);
    void calcRatio();

    bool insideRange(float eta = -999, float genPt = -1, float recoPt = -1, int hiBin = -1);

    void prepareTitle();

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

    // range of oberservables for which the histograms are made.
    // histograms are filled if range[i][0] <= observable < range[i][1]
    float ranges[MATCHRATIO::kN_MATCHRATIO_DEP][2];
};

void matchRatioHist::FillHNum(double x, float eta, float genPt, float recoPt, int hiBin)
{
    if (insideRange(eta, genPt, recoPt, hiBin))
        hNum->Fill(x);
}

void matchRatioHist::FillHDenom(double x, float eta, float genPt, float recoPt, int hiBin)
{
    if (insideRange(eta, genPt, recoPt, hiBin))
        hDenom->Fill(x);
}

void matchRatioHist::calcRatio()
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

/*
 * check if the given variables lie inside the kinematic range defined for this object.
 */
bool matchRatioHist::insideRange(float eta, float genPt, float recoPt, int hiBin)
{
    // make sure to pass the selection if no explicit kinematic range is specified.
    if (eta == -999)  eta = ranges[MATCHRATIO::kETA][0];
    if (genPt == -1) genPt = ranges[MATCHRATIO::kGENPT][0];
    if (recoPt == -1)  recoPt = ranges[MATCHRATIO::kRECOPT][0];
    if (hiBin == -1)  hiBin = ranges[MATCHRATIO::kHIBIN][0];

    if(ranges[MATCHRATIO::kETA][0] <= TMath::Abs(eta) &&
       (ranges[MATCHRATIO::kETA][1] == -1 || ranges[MATCHRATIO::kETA][1] > TMath::Abs(eta))){
    if(ranges[MATCHRATIO::kGENPT][0] <= genPt         &&
       (ranges[MATCHRATIO::kGENPT][1] == -1  || ranges[MATCHRATIO::kGENPT][1] > genPt)) {
    if(ranges[MATCHRATIO::kRECOPT][0] <= recoPt       &&
       (ranges[MATCHRATIO::kRECOPT][1] == -1 || ranges[MATCHRATIO::kRECOPT][1] > recoPt)) {
    if(ranges[MATCHRATIO::kHIBIN][0] <= hiBin         &&
       (ranges[MATCHRATIO::kHIBIN][1] == -1  || ranges[MATCHRATIO::kHIBIN][1] > hiBin)) {
            return true;
    }}}}
    return false;
}

/*
 * prepare the object title using the given ranges
 */
void matchRatioHist::prepareTitle()
{
    // prepare histogram title
    std::string etaStr  = "";       // whole detector
    std::string genPtStr  = "";     // whole pT range
    std::string recoPtStr = "";     // whole pT range
    std::string hiBinStr  = "";     // whole centrality range

    // special cases
    if (ranges[MATCHRATIO::kETA][0] <= 0 && ranges[MATCHRATIO::kETA][1] > 0)
        etaStr  = Form("|#eta|<%.2f", ranges[MATCHRATIO::kETA][1]);
    else if (ranges[MATCHRATIO::kETA][0] > 0 && ranges[MATCHRATIO::kETA][1] > 0)
        etaStr  = Form("%.2f<|#eta|<%.2f", ranges[MATCHRATIO::kETA][0], ranges[MATCHRATIO::kETA][1]);

    if (ranges[MATCHRATIO::kGENPT][0] > 0 && ranges[MATCHRATIO::kGENPT][1] <= -1)
        genPtStr  = Form("p_{T}^{GEN}>%.0f", ranges[MATCHRATIO::kGENPT][0]);
    else if (ranges[MATCHRATIO::kGENPT][0] > 0 && ranges[MATCHRATIO::kGENPT][1] > 0)
        genPtStr  = Form("%.0f<p_{T}^{GEN}<%.0f", ranges[MATCHRATIO::kGENPT][0], ranges[MATCHRATIO::kGENPT][1]);

    if (ranges[MATCHRATIO::kRECOPT][0] > 0 && ranges[MATCHRATIO::kRECOPT][1] <= -1)
        recoPtStr  = Form("p_{T}^{RECO}>%.0f", ranges[MATCHRATIO::kRECOPT][0]);
    else if (ranges[MATCHRATIO::kRECOPT][0] > 0 && ranges[MATCHRATIO::kRECOPT][1] > 0)
        recoPtStr = Form("%.0f<p_{T}^{RECO}<%.0f", ranges[MATCHRATIO::kRECOPT][0], ranges[MATCHRATIO::kRECOPT][1]);

    if (ranges[MATCHRATIO::kHIBIN][0] <= 0 && ranges[MATCHRATIO::kHIBIN][1] <= -1)
        hiBinStr = "";
    else if (ranges[MATCHRATIO::kHIBIN][0] >= 0 && ranges[MATCHRATIO::kHIBIN][1] > 0)
        hiBinStr  = Form("hiBin:%.0f-%.0f", ranges[MATCHRATIO::kHIBIN][0], ranges[MATCHRATIO::kHIBIN][1]);

    std::string tmpHistTitle = "";
    if (etaStr.size() > 0)  tmpHistTitle.append(Form("%s", etaStr.c_str()));
    if (genPtStr.size() > 0)  tmpHistTitle.append(Form(" %s", genPtStr.c_str()));
    if (recoPtStr.size() > 0) tmpHistTitle.append(Form(" %s", recoPtStr.c_str()));
    if (hiBinStr.size() > 0)  tmpHistTitle.append(Form(" %s", hiBinStr.c_str()));

    title = tmpHistTitle.c_str();
    if(hNumInitialized) {
        hNum->SetTitle(title.c_str());
        titleX = hNum->GetXaxis()->GetTitle();
    }
    if (hDenomInitialized) {
        hDenom->SetTitle(title.c_str());
    }
    if(graphAsymmErrorsInitialized) {
        a->SetTitle(title.c_str());
    }
    if(hRatioInitialized) {
        hRatio->SetTitle(title.c_str());
    }
}

#endif
