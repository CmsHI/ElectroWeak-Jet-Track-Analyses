/*
 * general purpose class for energy scale histograms
 */

#ifndef PERFORMANCE_INTERFACE_ENERGYSCALEHIST_H_
#define PERFORMANCE_INTERFACE_ENERGYSCALEHIST_H_

#include <TH1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TMath.h>
#include <TObjArray.h>
#include <TPad.h>
#include <TCanvas.h>

#include <string>

#include "../../Utilities/th1Util.h"

namespace ENERGYSCALE {

enum DEPS {   // energy scale dependencies
    kETA,
    kGENPT,
    kRECOPT,
    kHIBIN,
    kN_DEPS
};

const std::string ENERGYSCALE_DEP_LABELS[kN_DEPS] = {
        "ETA",
        "GENPT",
        "RECOPT",
        "HIBIN"
};

// observables
enum OBS {
    kESCALE,    // energy scale
    kERES,      // energy resolution
    kESCALEARITH,    // energy scale
    kERESARITH,      // energy resolution
    kN_OBS
};

const std::string OBS_LABELS[kN_OBS] = {"eScale", "eRes", "eScaleArith", "eResArith"};

};

/*
 * class to organize the analysis of the 1D energy scale distribution
 */
class eScaleAna {
public :
    eScaleAna(){
        hInitialized = false;
        f1Initialized = false;
        hPullInitialized = false;

        fitOption = "Q M R N";

        hMean = -1;
        hStdDev = -1;
    };
    ~eScaleAna(){};

    bool fit() {
        if (hInitialized && f1Initialized) {
            h->Fit(f1, fitOption.c_str());
            return true;
        }
        return false;
    };
    bool makePull() {
        if (hInitialized && f1Initialized) {
            hPull = (TH1D*)getPullHistogram(h, f1);
            hPullInitialized = true;

            setPullTH1D();
            return true;
        }
        return false;
    };
    void setPullTH1D() {
        if (hPullInitialized) {
            hPull->SetYTitle("Pull");
            hPull->SetMarkerStyle(kFullCircle);
            if (f1Initialized) {
                hPull->SetMarkerColor(f1->GetLineColor());
            }

            // set maximum and minimum of the pull distributions symmetric about y = 0
            double extremum = std::max(TMath::Abs(hPull->GetMaximum()), TMath::Abs(hPull->GetMaximum()));
            extremum *= 1.1;
            hPull->SetMaximum(extremum);
            hPull->SetMinimum(-1*extremum);
        }
    };
    void update() {
        if (hInitialized) {
            hMean = h->GetMean();
            hMeanErr = h->GetMeanError();

            hStdDev = h->GetStdDev();
            hStdDevErr = h->GetStdDevError();
        }
    };

    TH1D* h;       // energy scale distribution
    TF1*  f1;      // fit function
    TH1D* hPull;   // pull distribution for the fit

    std::string fitOption;

    bool hInitialized;
    bool f1Initialized;
    bool hPullInitialized;

    double hMean;    // histogram (arithmetic) mean of h, used as cross-check for the mean extracted from fit
    double hMeanErr;
    double hStdDev;   // histogram (arithmetic) std dev of h, used as cross-check for the width extracted from fit
    double hStdDevErr;
};

class energyScaleHist {
public :
    energyScaleHist(){
        nBinsX = 0;

        h2Dinitialized = false;
        hInitialized = false;
        h2DcorrInitialized = false;

        dep = -1;

        name = "";
        title = "";
        titleX = "";
        titleOffsetX = 1;
        titleOffsetY = 1;

        xMin = {0, 0};
        xMax = {-1, -1};
        yMin = {0, 0};
        yMax = {-1, -1};

        fitFncs = {};        // {"gaus"};
        fitOptions = {};     // {"Q M R N"};
        fitFncs_xMin = {};
        fitFncs_xMax = {};
        fitColors = {};

        for (int i=0; i<ENERGYSCALE::kN_DEPS; ++i) {
            ranges[i][0] = 0;
            ranges[i][1] = -1;  // no upper bound
        }
    };
    ~energyScaleHist(){};

    void FillH2D(double energyScale, double x, double w, float eta = -999, float genPt = -1, float recoPt = -1, int hiBin = -1);
    void FillH(double energyScale, double w, float eta = -999, float genPt = -1, float recoPt = -1, int hiBin = -1);
    void FillH2Dcorr(double genPt, double recoPt, double w, float eta = -999, int hiBin = -1);

    bool insideRange(float eta = -999, float genPt = -1, float recoPt = -1, int hiBin = -1);

    std::string getRangeTextEta();
    std::string getRangeTextGenPt();
    std::string getRangeTextRecoPt();
    std::string getRangeTextHiBin();

    std::string getBinEdgeText(int binLow, int binUp);

    void updateFncs();
    void updateH1DsliceY();

    void prepareTitle();

    void postLoop();
    void fitRecoGen();
    void calcArithmetic();
    void writeObjects(TCanvas* c);

    TH2D* h2D;
    int nBinsX;
    /*
     * h1D[0] = energy scale histogram, mean for a Gaussian fit
     * h1D[1] = energy resolution histogram, StdDev for a Gaussian fit
     * h1D[2] = energy scale histogram, mean from histogram mean of 1D distribution
     * h1D[3] = energy resolution histogram, StdDev from histogram mean of 1D distribution
     * h1D[4] = Constant for Gaussian fit
     * h1D[5] = chi2/ndf for Gaussian fit
     */
    TH1D* h1D[6];

    std::vector<TH1D*> h1DsliceY;           // energy scale distribution for each bin along x-axis
    /*
     * analyzers for 1D energy scale distribution histograms in h1DsliceY
     * esa is 2D vector with [nBinsX][nFitFncs]
     * esa[i][0] : initial fit from TH2::FitSlicesY
     * esa[i][1] : fit is seeded by FitSlicesY, uses bin range that covers 98% of the integral
     * esa[i][2] : fit is seeded by FitSlicesY, uses bin range that covers 95% of the integral
     */
    std::vector<std::vector<eScaleAna>> esa;
    std::vector<std::vector<TF1*>>  f1sv2;  // Fit functions for each histogram in h1DsliceY, these functions are input from user.
                                            // They are seed by f1sliceY if a function is Gaussian.
                                            // f1sv2 is 2D vector with [nBinsX][nFitFncs]

    TH1D* h;            // energy scale distribution
    TH2D* h2Dcorr;      // reco pt vs. gen pt correlation histogram.

    bool h2Dinitialized;
    bool hInitialized;
    bool h2DcorrInitialized;

    int dep;            // If the x-axis is eta, then dep = ENERGYSCALE::kETA

    std::string name;   // this is basically histogram name excluding the "h1D"/"h2D" prefix
    std::string title;
    std::string titleX;
    float titleOffsetX;
    float titleOffsetY;

    std::vector<float> xMin;
    std::vector<float> xMax;
    std::vector<float> yMin;
    std::vector<float> yMax;

    // fit functions for reco pt / gen pt distributiıon
    // These functions are in these functions are input from user.
    std::vector<std::string> fitFncs;
    std::vector<std::string> fitOptions;
    std::vector<double> fitFncs_xMin;
    std::vector<double> fitFncs_xMax;
    std::vector<int> fitColors;

    // range of oberservables for which the histograms are made.
    // histograms are filled if range[i][0] <= observable < range[i][1]
    float ranges[ENERGYSCALE::kN_DEPS][2];
};

void energyScaleHist::FillH2D(double energyScale, double x, double w, float eta, float genPt, float recoPt, int hiBin)
{
    if (h2Dinitialized && insideRange(eta, genPt, recoPt, hiBin))
        h2D->Fill(x, energyScale, w);
}

void energyScaleHist::FillH(double energyScale, double w, float eta, float genPt, float recoPt, int hiBin)
{
    // make sure to fill the histogram if no explicit kinematic range is specified.
    if (hInitialized && insideRange(eta, genPt, recoPt, hiBin))
        h->Fill(energyScale, w);
}

void energyScaleHist::FillH2Dcorr(double genPt, double recoPt, double w, float eta, int hiBin)
{
    if (h2DcorrInitialized && insideRange(eta, -1, -1, hiBin))
        h2Dcorr->Fill(genPt, recoPt, w);
}

/*
 * check if the given variables lie inside the kinematic range defined for this object.
 */
bool energyScaleHist::insideRange(float eta, float genPt, float recoPt, int hiBin)
{
    // make sure to pass the selection if no explicit kinematic range is specified.
    if (eta == -999)  eta = ranges[ENERGYSCALE::kETA][0];
    if (genPt == -1) genPt = ranges[ENERGYSCALE::kGENPT][0];
    if (recoPt == -1)  recoPt = ranges[ENERGYSCALE::kRECOPT][0];
    if (hiBin == -1)  hiBin = ranges[ENERGYSCALE::kHIBIN][0];

    if(ranges[ENERGYSCALE::kETA][0] <= TMath::Abs(eta) &&
       (ranges[ENERGYSCALE::kETA][1] == -1 || ranges[ENERGYSCALE::kETA][1] > TMath::Abs(eta))){
    if(ranges[ENERGYSCALE::kGENPT][0] <= genPt         &&
       (ranges[ENERGYSCALE::kGENPT][1] == -1  || ranges[ENERGYSCALE::kGENPT][1] > genPt)) {
    if(ranges[ENERGYSCALE::kRECOPT][0] <= recoPt       &&
       (ranges[ENERGYSCALE::kRECOPT][1] == -1 || ranges[ENERGYSCALE::kRECOPT][1] > recoPt)) {
    if(ranges[ENERGYSCALE::kHIBIN][0] <= hiBin         &&
       (ranges[ENERGYSCALE::kHIBIN][1] == -1  || ranges[ENERGYSCALE::kHIBIN][1] > hiBin)) {
            return true;
    }}}}
    return false;
}

std::string energyScaleHist::getRangeTextEta()
{
    std::string res = "";

    if (ranges[ENERGYSCALE::DEPS::kETA][0] <= 0 && ranges[ENERGYSCALE::DEPS::kETA][1] > 0)
        res  = Form("|#eta|<%.2f", ranges[ENERGYSCALE::DEPS::kETA][1]);
    else if (ranges[ENERGYSCALE::DEPS::kETA][0] > 0 && ranges[ENERGYSCALE::DEPS::kETA][1] > 0)
        res  = Form("%.2f<|#eta|<%.2f", ranges[ENERGYSCALE::DEPS::kETA][0], ranges[ENERGYSCALE::DEPS::kETA][1]);

    return res;
}

std::string energyScaleHist::getRangeTextGenPt()
{
    std::string res = "";

    if (ranges[ENERGYSCALE::DEPS::kGENPT][0] > 0 && ranges[ENERGYSCALE::DEPS::kGENPT][1] <= -1)
        res  = Form("p_{T}^{gen}>%.0f", ranges[ENERGYSCALE::DEPS::kGENPT][0]);
    else if (ranges[ENERGYSCALE::DEPS::kGENPT][0] > 0 && ranges[ENERGYSCALE::DEPS::kGENPT][1] > 0)
        res  = Form("%.0f<p_{T}^{gen}<%.0f", ranges[ENERGYSCALE::DEPS::kGENPT][0], ranges[ENERGYSCALE::DEPS::kGENPT][1]);
    else if (ranges[ENERGYSCALE::DEPS::kGENPT][0] <= 0 && ranges[ENERGYSCALE::DEPS::kGENPT][1] > 0)
        res  = Form("p_{T}^{gen}<%.0f", ranges[ENERGYSCALE::DEPS::kGENPT][1]);

    return res;
}

std::string energyScaleHist::getRangeTextRecoPt()
{
    std::string res = "";

    if (ranges[ENERGYSCALE::DEPS::kRECOPT][0] > 0 && ranges[ENERGYSCALE::DEPS::kRECOPT][1] <= -1)
        res  = Form("p_{T}^{reco}>%.0f", ranges[ENERGYSCALE::DEPS::kRECOPT][0]);
    else if (ranges[ENERGYSCALE::DEPS::kRECOPT][0] > 0 && ranges[ENERGYSCALE::DEPS::kRECOPT][1] > 0)
        res = Form("%.0f<p_{T}^{reco}<%.0f", ranges[ENERGYSCALE::DEPS::kRECOPT][0], ranges[ENERGYSCALE::DEPS::kRECOPT][1]);
    else if (ranges[ENERGYSCALE::DEPS::kRECOPT][0] <= 0 && ranges[ENERGYSCALE::DEPS::kRECOPT][1] > 0)
        res = Form("p_{T}^{reco}<%.0f", ranges[ENERGYSCALE::DEPS::kRECOPT][1]);

    return res;
}

std::string energyScaleHist::getRangeTextHiBin()
{
    std::string res = "";

    if (ranges[ENERGYSCALE::DEPS::kHIBIN][0] >= 0 && ranges[ENERGYSCALE::DEPS::kHIBIN][1] > 0)
        res = Form("Cent:%.0f-%.0f%%", ranges[ENERGYSCALE::DEPS::kHIBIN][0]/2, ranges[ENERGYSCALE::DEPS::kHIBIN][1]/2);

    return res;
}

/*
 * get range for histograms in "h1DsliceY"
 *
 * Ex. dep = ENERGYSCALE::kGENPT
 *     x-axis of h2D = {20,30,40,50,70,100}
 *     then
 *     getBinEdgeText(1, 1) returns "20<p_{T}^{gen}<30"
 *     getBinEdgeText(2, 2) returns "30<p_{T}^{gen}<40"
 *     getBinEdgeText(3, 4) returns "40<p_{T}^{gen}<70"
 */
std::string energyScaleHist::getBinEdgeText(int binLow, int binUp)
{
    std::string res = "";

    double xLow = h2D->GetXaxis()->GetBinLowEdge(binLow);
    double xUp = h2D->GetXaxis()->GetBinUpEdge(binUp);

    if (dep == ENERGYSCALE::kETA) {
        res  = Form("%.2f < #eta < %.2f", xLow, xUp);
    }
    else if (dep == ENERGYSCALE::kGENPT) {
        res  = Form("%.0f <p_{T}^{gen}< %.0f", xLow, xUp);
    }
    else if (dep == ENERGYSCALE::kRECOPT) {
        res = Form("%.0f <p_{T}^{reco}< %.0f", xLow, xUp);
    }
    else if (dep == ENERGYSCALE::kHIBIN) {
        res = Form("Cent:%.0f-%.0f%%", xLow/2, xUp/2);
    }

    return res;
}

void energyScaleHist::updateFncs()
{
    int nFitFncs = fitFncs.size();
    int nFitOptions = fitOptions.size();
    int nFitFncs_xMin = fitFncs_xMin.size();
    int nFitFncs_xMax = fitFncs_xMax.size();
    int nFitColors = fitColors.size();

    if (nFitFncs > nFitOptions) {
        for (int i = 0; i < nFitFncs-nFitOptions; ++i) {
            fitOptions.push_back(fitOptions[0].c_str());
        }
    }

    if (nFitFncs > nFitFncs_xMin) {
        for (int i = 0; i < nFitFncs-nFitFncs_xMin; ++i) {
            fitFncs_xMin.push_back(fitFncs_xMin[0]);
        }
    }

    if (nFitFncs > nFitFncs_xMax) {
        for (int i = 0; i < nFitFncs-nFitFncs_xMax; ++i) {
            fitFncs_xMax.push_back(fitFncs_xMax[0]);
        }
    }

    if (nFitFncs > nFitColors) {
        for (int i = 0; i < nFitFncs-nFitColors; ++i) {
            fitColors.push_back(fitColors[0]);
        }
    }
}

void energyScaleHist::updateH1DsliceY()
{
    h1DsliceY.clear();
    h1DsliceY.resize(nBinsX);

    // reco pt / gen pt distributions
    TH1D* hTmp = 0;

    for (int i=1; i<=nBinsX; ++i) {

        hTmp = h2D->ProjectionY(Form("h1D_projYbin%d_%s", i, name.c_str()), i, i, "");
        hTmp->SetTitleOffset(titleOffsetX, "X");
        hTmp->SetStats(false);
        hTmp->SetMarkerStyle(kFullCircle);
        h1DsliceY[i-1] = hTmp;
    }
}

/*
 * prepare the object title using the given ranges
 */
void energyScaleHist::prepareTitle()
{
    // prepare histogram title
    std::string etaStr  = "";       // whole detector
    std::string genPtStr  = "";     // whole pT range
    std::string recoPtStr = "";     // whole pT range
    std::string hiBinStr  = "";     // whole centrality range

    // special cases
    if (ranges[ENERGYSCALE::kETA][0] <= 0 && ranges[ENERGYSCALE::kETA][1] > 0)
        etaStr  = Form("|#eta|<%.2f", ranges[ENERGYSCALE::kETA][1]);
    else if (ranges[ENERGYSCALE::kETA][0] > 0 && ranges[ENERGYSCALE::kETA][1] > 0)
        etaStr  = Form("%.2f<|#eta|<%.2f", ranges[ENERGYSCALE::kETA][0], ranges[ENERGYSCALE::kETA][1]);

    if (ranges[ENERGYSCALE::kGENPT][0] > 0 && ranges[ENERGYSCALE::kGENPT][1] <= -1)
        genPtStr  = Form("p_{T}^{GEN}>%.0f", ranges[ENERGYSCALE::kGENPT][0]);
    else if (ranges[ENERGYSCALE::kGENPT][0] > 0 && ranges[ENERGYSCALE::kGENPT][1] > 0)
        genPtStr  = Form("%.0f<p_{T}^{GEN}<%.0f", ranges[ENERGYSCALE::kGENPT][0], ranges[ENERGYSCALE::kGENPT][1]);

    if (ranges[ENERGYSCALE::kRECOPT][0] > 0 && ranges[ENERGYSCALE::kRECOPT][1] <= -1)
        recoPtStr  = Form("p_{T}^{RECO}>%.0f", ranges[ENERGYSCALE::kRECOPT][0]);
    else if (ranges[ENERGYSCALE::kRECOPT][0] > 0 && ranges[ENERGYSCALE::kRECOPT][1] > 0)
        recoPtStr = Form("%.0f<p_{T}^{RECO}<%.0f", ranges[ENERGYSCALE::kRECOPT][0], ranges[ENERGYSCALE::kRECOPT][1]);

    if (ranges[ENERGYSCALE::kHIBIN][0] <= 0 && ranges[ENERGYSCALE::kHIBIN][1] <= -1)
        hiBinStr = "";
    else if (ranges[ENERGYSCALE::kHIBIN][0] >= 0 && ranges[ENERGYSCALE::kHIBIN][1] > 0)
        hiBinStr  = Form("hiBin:%.0f-%.0f", ranges[ENERGYSCALE::kHIBIN][0], ranges[ENERGYSCALE::kHIBIN][1]);

    std::string tmpHistTitle = "";
    if (etaStr.size() > 0)  tmpHistTitle.append(Form("%s", etaStr.c_str()));
    if (genPtStr.size() > 0)  tmpHistTitle.append(Form(" %s", genPtStr.c_str()));
    if (recoPtStr.size() > 0) tmpHistTitle.append(Form(" %s", recoPtStr.c_str()));
    if (hiBinStr.size() > 0)  tmpHistTitle.append(Form(" %s", hiBinStr.c_str()));

    title = tmpHistTitle.c_str();
    if(h2Dinitialized) {
        h2D->SetTitle(title.c_str());
        titleX = h2D->GetXaxis()->GetTitle();
    }
    if (hInitialized) {
        h->SetTitle(title.c_str());
    }
    if(h2DcorrInitialized) {
        h2Dcorr->SetTitle(title.c_str());
    }
}

void energyScaleHist::postLoop()
{
    if (hInitialized) {
        h->SetMarkerStyle(kFullCircle);
    }

    if (!h2Dinitialized) return;

    nBinsX = h2D->GetXaxis()->GetNbins();

    TObjArray aSlices;
    h2D->FitSlicesY(0,0,-1,0,"Q LL M N", &aSlices);

    // energy scale
    h1D[0] = (TH1D*)aSlices.At(1)->Clone(Form("h1D_%s_%s", ENERGYSCALE::OBS_LABELS[0].c_str(), name.c_str()));
    h1D[0]->SetTitle(title.c_str());
    h1D[0]->SetXTitle(titleX.c_str());
    setTH1_energyScale(h1D[0], titleOffsetX, titleOffsetY);
    if (yMax[ENERGYSCALE::kESCALE] > yMin[ENERGYSCALE::kESCALE])
        h1D[0]->SetAxisRange(yMin[ENERGYSCALE::kESCALE], yMax[ENERGYSCALE::kESCALE], "Y");

    // width of energy scale
    h1D[1] = (TH1D*)aSlices.At(2)->Clone(Form("h1D_%s_%s", ENERGYSCALE::OBS_LABELS[1].c_str(), name.c_str()));
    h1D[1]->SetTitle(title.c_str());
    h1D[1]->SetXTitle(titleX.c_str());
    setTH1_energyWidth(h1D[1], titleOffsetX, titleOffsetY);
    if (yMax[ENERGYSCALE::kERES] > yMin[ENERGYSCALE::kERES])
        h1D[1]->SetAxisRange(yMin[ENERGYSCALE::kERES], yMax[ENERGYSCALE::kERES], "Y");

    // energy scale with arithmetic mean
    h1D[2] = (TH1D*)h1D[0]->Clone(Form("h1D_%s_%s", ENERGYSCALE::OBS_LABELS[2].c_str(), name.c_str()));
    h1D[2]->SetTitle(title.c_str());
    h1D[2]->SetXTitle(titleX.c_str());
    setTH1_energyScale(h1D[2], titleOffsetX, titleOffsetY);
    h1D[2]->SetYTitle(Form("%s (Arith)", h1D[2]->GetYaxis()->GetTitle()));
    if (yMax[ENERGYSCALE::kESCALE] > yMin[ENERGYSCALE::kESCALE])
        h1D[2]->SetAxisRange(yMin[ENERGYSCALE::kESCALE], yMax[ENERGYSCALE::kESCALE], "Y");

    // width of energy scale with arithmetic std dev
    h1D[3] = (TH1D*)h1D[1]->Clone(Form("h1D_%s_%s", ENERGYSCALE::OBS_LABELS[3].c_str(), name.c_str()));
    h1D[3]->SetTitle(title.c_str());
    h1D[3]->SetXTitle(titleX.c_str());
    setTH1_energyWidth(h1D[3], titleOffsetX, titleOffsetY);
    h1D[3]->SetYTitle(Form("%s (Arith)", h1D[3]->GetYaxis()->GetTitle()));
    if (yMax[ENERGYSCALE::kERES] > yMin[ENERGYSCALE::kERES])
        h1D[3]->SetAxisRange(yMin[ENERGYSCALE::kERES], yMax[ENERGYSCALE::kERES]/20, "Y");

    updateH1DsliceY();
    calcArithmetic();

    // Constant for Gaussian fit
    h1D[4] = (TH1D*)aSlices.At(0)->Clone(Form("h1D_gausConst_%s", name.c_str()));
    h1D[4]->SetTitle(title.c_str());
    h1D[4]->SetXTitle(titleX.c_str());
    h1D[4]->SetYTitle("Constant for Gaussian fit");
    h1D[4]->SetStats(false);
    h1D[4]->SetMarkerStyle(kFullCircle);

    // chi2/ndf for Gaussian fit
    h1D[5] = (TH1D*)aSlices.At(3)->Clone(Form("h1D_gausChi2ndf_%s", name.c_str()));
    h1D[5]->SetTitle(title.c_str());
    h1D[5]->SetXTitle(titleX.c_str());
    h1D[5]->SetYTitle("chi2/ndf for Gaussian fit");
    h1D[5]->SetStats(false);
    h1D[5]->SetMarkerStyle(kFullCircle);

    fitRecoGen();
}

/*
 * fit distributions that compare reco-level and gen-level objects
 * Ex. fit reco pt / gen pt distribution
 */
void energyScaleHist::fitRecoGen()
{
    // reco pt / gen pt distributions and fits
    TH1D* hTmp = 0;
    TF1* f1Tmp = 0;
    updateFncs();

    esa.clear();
    esa.resize(nBinsX);

    f1sv2.clear();
    f1sv2.resize(nBinsX);

    for (int i=1; i<=nBinsX; ++i) {

        hTmp = h1DsliceY[i-1];

        // fit functions for that bin along x-axis of h2D
        std::vector<TF1*> f1sTmp;

        // initial fit from TH2::FitSlicesY
        f1Tmp = new TF1(Form("f1_bin%d_fnc0_%s", i, name.c_str()), "gaus", 0, 2);
        double p0 = h1D[4]->GetBinContent(i);   // constant
        double p1 = h1D[0]->GetBinContent(i);   // mean
        double p2 = h1D[1]->GetBinContent(i);   // StdDev
        f1Tmp->SetParameters(p0, p1, p2);
        f1Tmp->SetLineColor(kGreen);
        //        double chi2ndf = h1D[5]->GetBinContent(i);
        //        f1Tmp->SetChisquare(chi2ndf*100);
        //        f1Tmp->SetNDF(100);
        f1sTmp.push_back(f1Tmp);

        int binMax = hTmp->GetMaximumBin();
        int nBinsTmp = hTmp->GetNbinsX();
        std::string option = "Q M R N";

        // Gaus fit seeded by FitSlicesY, uses bin range that covers 98% of the integral
        // Gaus fit seeded by FitSlicesY, uses bin range that covers 95% of the integral
        std::vector<std::string> fncFormulas = {"gaus", "gaus"};
        std::vector<double> fractions = {0.98, 0.95};
        std::vector<int> fncColors = {kBlue, kRed};

        int nFnc = fncFormulas.size();
        for (int iFnc = 0; iFnc < nFnc; ++iFnc) {

            std::vector<int> fncRange = getLeftRightBins4IntegralFraction(h, binMax, fractions[iFnc]);
            int binLow = std::max(fncRange[0], 1);
            int binUp = std::min(fncRange[1], nBinsTmp);
            f1Tmp = new TF1(Form("f1_bin%d_fnc%d_%s", i, iFnc+1, name.c_str()), fncFormulas[iFnc].c_str(), hTmp->GetBinLowEdge(binLow), hTmp->GetBinLowEdge(binUp+1));
            f1Tmp->SetLineColor(fncColors[iFnc]);
            if (fncFormulas[iFnc] == "gaus") {
                // use the fit from TH2::FitSlicesY as seed
                f1Tmp->SetParameters(p0, p1, p2);
            }
            f1sTmp.push_back(f1Tmp);
        }

        int nF1s = f1sTmp.size();
        std::vector<eScaleAna> esaTmp(nF1s);
        for (int j = 0; j < nF1s; ++j) {
            esaTmp[j].h = hTmp;
            esaTmp[j].hInitialized = true;
            esaTmp[j].f1 = f1sTmp[j];
            esaTmp[j].f1Initialized = true;

            esaTmp[j].fitOption = option;
            // j = 0 corresponds to fit initial from TH2::FitSlicesY, do not refit.
            if (j > 0)  esaTmp[j].fit();

            esaTmp[j].makePull();
            esaTmp[j].hPullInitialized = true;
            std::string hpullName = replaceAll(esaTmp[j].f1->GetName(), "f1_", "hpull");
            esaTmp[j].hPull->SetName(hpullName.c_str());

            esaTmp[j].update();
        }
        esa[i-1] = esaTmp;

        int nFncsv2 = fitFncs.size();
        // fit functions for that bin along x-axis
        std::vector<TF1*> f1sv2Tmp;
        for (int iFnc = 0; iFnc < nFncsv2; ++iFnc) {
            std::string f1Name = Form("f1_bin%d_fncv2%d_%s", i, iFnc+1, name.c_str());

            std::string formulaTmp = fitFncs[iFnc];
            f1Tmp = new TF1(f1Name.c_str(), formulaTmp.c_str(), fitFncs_xMin[iFnc], fitFncs_xMax[iFnc]);

            if (formulaTmp == "gaus") {
                // use the fit from TH2::FitSlicesY as seed
                f1Tmp->SetParameters(p0, p1, p2);
            }
            f1Tmp->SetLineColor(fitColors[iFnc]);

            hTmp->Fit(f1Tmp, fitOptions[iFnc].c_str());
            f1sv2Tmp.push_back(f1Tmp);
        }
        f1sv2[i-1] = f1sv2Tmp;
    }
}

/*
 * arithmetic mean and std dev of energy scale distributions
 */
void energyScaleHist::calcArithmetic()
{
    for (int i = 1; i <= nBinsX; ++i) {

        double mean = h1DsliceY[i-1]->GetMean();
        double meanErr = h1DsliceY[i-1]->GetMeanError();
        h1D[2]->SetBinContent(i, mean);
        h1D[2]->SetBinError(i, meanErr);

        double stdDev = h1DsliceY[i-1]->GetStdDev();
        double stdDevErr = h1DsliceY[i-1]->GetStdDevError();;
        h1D[3]->SetBinContent(i, stdDev);
        h1D[3]->SetBinContent(i, stdDevErr);
    }
}

/*
 * write histograms with a particular dependence
 * use "c" as a template
 *
 * The objects in h1DsliceY, f1sliceY, f1sliceYv2 will not be written so as not to inflate output file size.
 * But they will be plotted on an multipanel canvas and that canvas will be written.
 */
void energyScaleHist::writeObjects(TCanvas* c)
{
    if (hInitialized) {
        h->SetMarkerStyle(kFullCircle);
        h->Write();
    }
    if (h2DcorrInitialized)
        h2Dcorr->Write();

    if (!h2Dinitialized) return;

    // extract information from "c"
    int windowWidth = c->GetWw();
    int windowHeight = c->GetWh();
    double leftMargin = c->GetLeftMargin();
    double rightMargin = c->GetRightMargin();
    double bottomMargin = c->GetBottomMargin();
    double topMargin = c->GetTopMargin();

    TLine* line = 0;

    std::string canvasName = "";
    canvasName = replaceAll(h2D->GetName(), "h2D", "cnv2D");
    c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
    c->cd();
    setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
    h2D->SetTitleOffset(titleOffsetX, "X");
    h2D->SetTitleOffset(titleOffsetY, "Y");
    h2D->SetStats(false);
    h2D->Draw("colz");
    h2D->Write("",TObject::kOverwrite);
    setCanvasFinal(c);
    c->Write("",TObject::kOverwrite);
    c->Close();         // do not use Delete() for TCanvas.

    // energy scale
    canvasName = Form("cnv_%s_%s", ENERGYSCALE::OBS_LABELS[0].c_str(), name.c_str());
    c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
    c->cd();
    setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
    float markerSize = (float)windowWidth/600;
    h1D[0]->SetMarkerSize(markerSize);
    h1D[0]->Draw("e");
    h1D[0]->Write("",TObject::kOverwrite);

    // draw line y = 1
    float x1 = h1D[0]->GetXaxis()->GetXmin();
    float x2 = h1D[0]->GetXaxis()->GetXmax();
    line = new TLine(x1, 1, x2, 1);
    line->SetLineStyle(kDashed);
    line->Draw();
    setCanvasFinal(c);
    c->Write("",TObject::kOverwrite);
    c->Close();         // do not use Delete() for TCanvas.

    // width of energy scale
    canvasName = Form("cnv_%s_%s", ENERGYSCALE::OBS_LABELS[1].c_str(), name.c_str());
    c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
    c->cd();
    setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
    h1D[1]->SetMarkerSize(markerSize);
    h1D[1]->Draw("e");
    h1D[1]->Write("",TObject::kOverwrite);
    setCanvasFinal(c);
    c->Write("",TObject::kOverwrite);
    c->Close();         // do not use Delete() for TCanvas.

    // energy scale from histogram mean
    canvasName = Form("cnv_%s_%s", ENERGYSCALE::OBS_LABELS[2].c_str(), name.c_str());
    c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
    c->cd();
    setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
    markerSize = (float)windowWidth/600;
    h1D[2]->SetMarkerSize(markerSize);
    h1D[2]->Draw("e");
    h1D[2]->Write("",TObject::kOverwrite);

    // draw line y = 1
    x1 = h1D[2]->GetXaxis()->GetXmin();
    x2 = h1D[2]->GetXaxis()->GetXmax();
    line = new TLine(x1, 1, x2, 1);
    line->SetLineStyle(kDashed);
    line->Draw();
    setCanvasFinal(c);
    c->Write("",TObject::kOverwrite);
    c->Close();         // do not use Delete() for TCanvas.

    // width of energy scale from histogram std dev
    canvasName = Form("cnv_%s_%s", ENERGYSCALE::OBS_LABELS[3].c_str(), name.c_str());
    c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
    c->cd();
    setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
    h1D[3]->SetMarkerSize(markerSize);
    h1D[3]->Draw("e");
    h1D[3]->Write("",TObject::kOverwrite);
    setCanvasFinal(c);
    c->Write("",TObject::kOverwrite);
    c->Close();         // do not use Delete() for TCanvas.

    // plot 1D reco pt / gen pt distribution for each bin along x-axis
    int nH1DsliceY = h1DsliceY.size();
    int columns = calcNcolumns(nH1DsliceY);
    int rows = calcNrows(nH1DsliceY);

    double normCanvasHeight = calcNormCanvasHeight(rows, bottomMargin, topMargin, 0);
    double normCanvasWidth = calcNormCanvasWidth(columns, leftMargin, rightMargin, 0);
    c = new TCanvas(Form("cnv_projY_%s", name.c_str()),"",windowWidth,windowHeight);
    setCanvasSizeMargin(c, normCanvasWidth, normCanvasHeight, leftMargin, rightMargin, bottomMargin, topMargin);
    setCanvasFinal(c);
    c->cd();

    int nPads = rows * columns;
    TPad* pads[nPads];
    divideCanvas(c, pads, rows, columns, leftMargin, rightMargin, bottomMargin, topMargin, 0, topMargin, 0.05);

    TLatex* latex = new TLatex();
    for (int i = 0; i < nH1DsliceY; ++i) {
        c->cd(i+1);

        h1DsliceY[i]->SetMarkerSize(markerSize);
        //h1DsliceY[i]->Draw("axis");     // for now draw only axis, points will be plotted after the fit functions
        h1DsliceY[i]->Draw("e");

        pads[i]->Update();
        float y1 = gPad->GetUymin();
        float y2 = gPad->GetUymax();

        line = new TLine(1, y1, 1, y2);
        line->SetLineStyle(kDashed);
        line->Draw();

        int nFitFncs = esa[i].size();
        for (int iFnc = 0; iFnc < nFitFncs; ++iFnc) {
            if (esa[i][iFnc].f1Initialized) {
                esa[i][iFnc].f1->Draw("same");
            }
        }

        int nFitFncsv2 = f1sv2[i].size();
        for (int iFnc = 0; iFnc < nFitFncsv2; ++iFnc) {
            f1sv2[i][iFnc]->Draw("same");
        }

        h1DsliceY[i]->Draw("e same");   // points should line above functions

        std::vector<std::string> textLinesTmp;
        std::string textLineTmp = getBinEdgeText(i+1, i+1);
        if (textLineTmp.size() > 0) textLinesTmp.push_back(textLineTmp.c_str());
        drawTextLines(latex, pads[i], textLinesTmp, "NW", 0.04, 0.1);
    }
    c->Write("",TObject::kOverwrite);
    // plot 1D reco pt / gen pt distribution for each bin along x-axis - END

    // plot pull distributions for energy scale fits
    c = new TCanvas(Form("cnv_projYPull_%s", name.c_str()),"",windowWidth,windowHeight);
    setCanvasSizeMargin(c, normCanvasWidth, normCanvasHeight, leftMargin, rightMargin, bottomMargin, topMargin);
    setCanvasFinal(c);
    c->cd();

    divideCanvas(c, pads, rows, columns, leftMargin, rightMargin, bottomMargin, topMargin, 0, topMargin, 0.05);

    for (int i = 0; i < nH1DsliceY; ++i) {
        c->cd(i+1);

        int nEsa = esa[i].size();
        int j1 = 1;

        for (int j = j1; j < nEsa; ++j) {
            if (esa[i][j].hPullInitialized)  {

                esa[i][j].hPull->SetMarkerSize(markerSize);

                std::string drawOption = "e same";
                if (j == j1)  drawOption = "e";
                esa[i][j].hPull->Draw(drawOption.c_str());
            }
        }

        if (nEsa > 0 && esa[i][j1].hInitialized) {
            float x1 = esa[i][j1].hPull->GetXaxis()->GetXmin();
            float x2 = esa[i][j1].hPull->GetXaxis()->GetXmax();

            line = new TLine(x1, 0, x2, 0);
            line->SetLineStyle(kSolid);
            line->Draw();

            pads[i]->Update();
            float yPadMin = gPad->GetUymin();
            float yPadMax = gPad->GetUymax();

            if (yPadMax > 2) {
                line = new TLine(x1, 2, x2, 2);
                line->SetLineStyle(kDashed);
                line->Draw();
            }

            if (yPadMin < -2) {
                line = new TLine(x1, -2, x2, -2);
                line->SetLineStyle(kDashed);
                line->Draw();
            }
        }

        std::vector<std::string> textLinesTmp;
        std::string textLineTmp = getBinEdgeText(i+1, i+1);
        if (textLineTmp.size() > 0) textLinesTmp.push_back(textLineTmp.c_str());
        drawTextLines(latex, pads[i], textLinesTmp, "NW", 0.04, 0.1);
    }
    c->Write("",TObject::kOverwrite);
    // plot pull distributions for energy scale fits - END

    line->Delete();
    latex->Delete();
    for (int i = 0; i < nPads; ++i) {
        if (pads[i] != 0)  pads[i]->Delete();
    }
    c->Close();         // do not use Delete() for TCanvas.
}

#endif
