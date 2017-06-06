/*
 * general purpose class for energy scale histograms
 */

#ifndef PERFORMANCE_INTERFACE_ENERGYSCALEHIST_H_
#define PERFORMANCE_INTERFACE_ENERGYSCALEHIST_H_

#include <TH1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TF1.h>
#include <TGraph.h>
#include <TGraphAsymmErrors.h>
#include <TMath.h>
#include <TObjArray.h>
#include <TPad.h>
#include <TCanvas.h>

#include <string>
#include <map>

#include "../../Utilities/th1Util.h"
#include "../../Utilities/tgraphUtil.h"

namespace ENERGYSCALE {

enum DEPS {   // energy scale dependencies
    kETA,
    kGENPT,
    kRECOPT,
    kCENT,
    kSUMISO,
    kSIEIE,
    kN_DEPS
};

const std::string ENERGYSCALE_DEP_LABELS[kN_DEPS] = {
        "ETA",
        "GENPT",
        "RECOPT",
        "CENT",
        "SUMISO",
        "SIEIE"
};

// observables
enum OBS {
    kESCALE,    // energy scale
    kERES,      // energy resolution
    kESCALEARITH,    // energy scale
    kERESARITH,      // energy resolution
    kEFF,            // matching efficiency
    kFAKE,            // fake rate
    kN_OBS
};

const std::string OBS_LABELS[kN_OBS] = {"eScale", "eRes", "eScaleArith", "eResArith", "eff", "fakeRate"};

enum FNCS {
    kGAUS_FitSlicesY,   // initial fit from TH2::FitSlicesY
    kGAUS_95,   // Gaus fit seeded by FitSlicesY, uses bin range that covers 95% of the integral
    kGAUS_98,   // Gaus fit seeded by FitSlicesY, uses bin range that covers 98% of the integral
    kN_FNCS
};

const std::string fncFormulas[kN_FNCS] = {"gaus", "gaus", "gaus"};
const double intFractions[kN_FNCS] = {1, 0.95, 0.98};
const int fncColors[kN_FNCS] = {kGreen+2, kRed, kBlue};
const std::string fitOption = "Q M R N";

// list of particles that can fake RECO-level objects
enum FAKECAND {
    k_unknown,
    k_electron,
    k_eneutrino,
    k_muon,
    k_muneutrino,
    k_tau,
    k_kaonL,
    k_pionch,
    k_kaonS,
    k_kaonch,
    k_neutron,
    k_proton,
    k_sigmaM,
    k_lambda,
    k_sigmaP,
    k_cascadeM,
    k_cascade0,
    kN_FakeCand
};

struct particle
{
    particle() : name(""), label(""), PDG(0), m(0), charge(0) {}
    particle(std::string name = "", std::string label = "", int PDG = 0, double m = 0, int charge = 0) {
        this->name = name;
        this->label = label;
        this->PDG = PDG;
        this->m = m;
        this->charge = charge;
    }

    std::string name;
    std::string label;
    int PDG;
    double m;
    int charge;
};

const particle particles[kN_FakeCand] =
{
   particle("unknown", "other", 0),
   particle("electron", "e^{#pm}", 11),
   particle("eneutrino", "#nu_{e}", 12),
   particle("muon", "#mu^{#pm}", 13),
   particle("muneutrino", "#nu_{#mu}", 14),
   particle("tau", "#tau^{#pm}", 16),
   particle("kaonL", "K^{0}_{L}", 130),
   particle("pionch", "#pi^{#pm}", 211),
   particle("kaonS", "K^{0}_{S}", 310),
   particle("kaonch", "K^{#pm}", 321),
   particle("neutron", "n", 2112),
   particle("proton", "p", 2212),
   particle("sigmaM", "#Sigma^{-}", 3112),
   particle("lambda", "#Lambda", 3122),
   particle("sigmaP", "#Sigma^{+}", 3222),
   particle("cascadeM", "#Xi^{-}", 3312),
   particle("cascade0", "#Xi^{0}", 3322)
};

};

/*
 * class to organize the analysis of the 1D energy scale distribution
 */
class eScaleAna {
public :
    eScaleAna(){
        h = 0;
        f1 = 0;
        hPull = 0;
        isValid_h = false;
        isValid_f1 = false;
        isValid_hPull = false;

        fitOption = "Q M R N";

        hMean = -1;
        hStdDev = -1;
    };
    ~eScaleAna(){};

    bool fit() {
        if (isValid_h && isValid_f1) {
            h->Fit(f1, fitOption.c_str());
            return true;
        }
        return false;
    };
    bool makePull() {
        if (isValid_h && isValid_f1) {
            hPull = (TH1D*)getPullHistogram(h, f1);
            isValid_hPull = true;

            setPullTH1D();
            return true;
        }
        return false;
    };
    void setPullTH1D() {
        if (isValid_hPull) {
            hPull->SetYTitle("Pull");
            hPull->SetMarkerStyle(kFullCircle);
            if (isValid_f1) {
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

        isValid_h = (h != 0 && !h->IsZombie());
        isValid_f1 = (f1 != 0 && !f1->IsZombie());
        isValid_hPull = (hPull != 0 && !hPull->IsZombie());

        if (isValid_h) {
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

    bool isValid_h;
    bool isValid_f1;
    bool isValid_hPull;

    double hMean;    // histogram (arithmetic) mean of h, used as cross-check for the mean extracted from fit
    double hMeanErr;
    double hStdDev;   // histogram (arithmetic) std dev of h, used as cross-check for the width extracted from fit
    double hStdDevErr;
};

class energyScaleHist {
public :
    energyScaleHist(){
        nBinsX = 0;
        h2D = 0;

        for (int i = 0; i < 6; ++i) {
            h1DeScale[i] = 0;
        }
        for (int i = 0; i < 6; ++i) {
            h1D[i] = 0;
        }
        hEscale = 0;
        h2Dcorr = 0;

        isValid_h2D = false;
        isValid_hEscale = false;
        isValid_h2Dcorr = false;

        indexFnc = ENERGYSCALE::kGAUS_95;

        hNum = 0;
        hDenom = 0;
        hRatio = 0;
        gRatio = 0;
        isValid_hNum = false;
        isValid_hDenom = false;
        isValid_hRatio = false;
        isValid_gRatio = false;

        hNumFake = 0;
        hDenomFake = 0;
        hRatioFake = 0;
        gRatioFake = 0;
        hRatioFakeOther = 0;
        isValid_hNumFake = false;
        isValid_hDenomFake = false;
        isValid_hRatioFake = false;
        isValid_gRatioFake = false;
        isValid_hRatioFakeOther = false;

        fakeIndices = {
                ENERGYSCALE::FAKECAND::k_kaonS,
                ENERGYSCALE::FAKECAND::k_pionch,
                ENERGYSCALE::FAKECAND::k_kaonL,
                ENERGYSCALE::FAKECAND::k_kaonch,
                ENERGYSCALE::FAKECAND::k_electron,
                ENERGYSCALE::FAKECAND::k_neutron,
                ENERGYSCALE::FAKECAND::k_proton,
                ENERGYSCALE::FAKECAND::k_lambda,
        };

        nFakePDGs = fakeIndices.size();
        fakePDGs.resize(nFakePDGs);

        hFakeParticle.resize(nFakePDGs);
        hRatioFakeParticle.resize(nFakePDGs);

        isValid_hFakeParticle.resize(nFakePDGs);
        isValid_hRatioFakeParticle.resize(nFakePDGs);

        hFakeParticleGenPt.resize(nFakePDGs);
        hRatioFakeParticleGenPt.resize(nFakePDGs);

        isValid_hFakeParticleGenPt.resize(nFakePDGs);
        isValid_hRatioFakeParticleGenPt.resize(nFakePDGs);

        passedMinFakeFraction.resize(nFakePDGs);
        passedMinFakeFractionGenPt.resize(nFakePDGs);

        for (int i = 0; i < nFakePDGs; ++i) {

            int index = fakeIndices.at(i);
            fakePDGs[i] = ENERGYSCALE::particles[index].PDG;

            hFakeParticle[i] = 0;
            hRatioFakeParticle[i] = 0;
            hRatioFakeOther = 0;

            isValid_hFakeParticle[i] = false;
            isValid_hRatioFakeParticle[i] = false;
            isValid_hRatioFakeOther = false;

            hFakeParticleGenPt[i] = 0;
            hAllFakeParticlesGenPt = 0;
            hRatioFakeParticleGenPt[i] = 0;
            hRatioFakeOtherGenPt = 0;

            isValid_hFakeParticleGenPt[i] = false;
            isValid_hAllFakeParticlesGenPt = false;
            isValid_hRatioFakeParticleGenPt[i] = false;
            isValid_hRatioFakeOtherGenPt = false;

            passedMinFakeFraction[i] = false;
            passedMinFakeFractionGenPt[i] = false;
        }
        minFakeFraction = 0.05;

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

    void FillH2D(double energyScale, double x, double w, float eta = -999, float genPt = -1, float recoPt = -1, int cent = -1);
    void FillH(double energyScale, double w, float eta = -999, float genPt = -1, float recoPt = -1, int cent = -1);
    void FillH2Dcorr(double genPt, double recoPt, double w, float eta = -999, int cent = -1);

    void FillHNum(double x, double w, float genEta = -999, float genPt = -1, float recoPt = -1, int cent = -1);
    void FillHDenom(double x, double w, float genEta = -999, float genPt = -1, int cent = -1);
    void FillHNumFake(double x, double w, float eta = -999, float recoPt = -1, int cent = -1);
    void FillHDenomFake(double x, double w, float eta = -999, float recoPt = -1, int cent = -1);
    void FillHFakeParticle(double x, int pdg, double w, float eta = -999, float genPt = -1, float recoPt = -1, int cent = -1);
    void FillHFakeParticleGenPt(double genPt, int pdg, double w, float eta = -999, float recoPt = -1, int cent = -1);

    bool insideRange(float eta = -999, float genPt = -1, float recoPt = -1, int cent = -1);

    std::string getRangeTextEta();
    std::string getRangeTextGenPt();
    std::string getRangeTextRecoPt();
    std::string getRangeTextCent();

    std::string getBinEdgeText(int binLow, int binUp);

    void updateTH1();
    void updateFncs();
    void updateH1DsliceY();
    void updateH1DeScale();
    int getFakePDG(int iPDG);

    void prepareTitle();

    void postLoop();
    void fitRecoGen();
    void calcRatio();
    void calcRatioFake();
    void calcRatioFakeParticle();
    void calcRatioFakeParticleGenPt();
    void writeObjects(TCanvas* c);

    static void setPad4Observable(TPad* p, int iObs, int iDep);
    void setPad4Observable(TPad* p, int iObs);
    void drawLine4PtRange(TPad* p, int lineColor = kBlack);

    int nBinsX;
    TH2D* h2D;
    /*
     * h1DeScale[0] = energy scale histogram, mean for a Gaussian fit
     * h1DeScale[1] = energy resolution histogram, StdDev for a Gaussian fit
     * h1DeScale[2] = energy scale histogram, mean from histogram mean of 1D distribution
     * h1DeScale[3] = energy resolution histogram, StdDev from histogram mean of 1D distribution
     * h1DeScale[4] = Constant for Gaussian fit
     * h1DeScale[5] = chi2/ndf for Gaussian fit
     */
    TH1D* h1DeScale[6];

    /*
     * h1D[0] = energy scale histogram, mean for a Gaussian fit
     * h1D[1] = energy resolution histogram, StdDev for a Gaussian fit
     * h1D[2] = energy scale histogram, mean from histogram mean of 1D distribution
     * h1D[3] = energy resolution histogram, StdDev from histogram mean of 1D distribution
     * h1D[4] = matching efficiency
     * h1D[5] = fake rate
     */
    TH1D* h1D[6];

    std::vector<TH1D*> h1DsliceY;           // energy scale distribution for each bin along x-axis
    /*
     * analyzers for 1D energy scale distribution histograms in h1DsliceY
     * esa is 2D vector with [nBinsX][nFitFncs]
     * esa[i][0] : initial fit from TH2::FitSlicesY
     * esa[i][1] : fit is seeded by FitSlicesY, uses bin range that covers 95% of the integral
     * esa[i][2] : fit is seeded by FitSlicesY, uses bin range that covers 98% of the integral
     */
    std::vector<std::vector<eScaleAna>> esa;
    int indexFnc;    // index of the fit function to set the bins of h1DeScale[0], h1DeScale[1]
                     // function whose results will be shown in the final plots

    std::vector<std::vector<TF1*>>  f1sv2;  // Fit functions for each histogram in h1DsliceY, these functions are input from user.
                                            // They are seed by f1sliceY if a function is Gaussian.
                                            // f1sv2 is 2D vector with [nBinsX][nFitFncs]

    TH1D* hEscale;      // energy scale distribution for all the bins along x-axis
    TH2D* h2Dcorr;      // reco pt vs. gen pt correlation histogram.

    bool isValid_h2D;
    bool isValid_hEscale;
    bool isValid_h2Dcorr;

    // objects for efficiency
    TH1D* hNum;
    TH1D* hDenom;
    TH1D* hRatio;
    TGraphAsymmErrors* gRatio;

    bool isValid_hNum;
    bool isValid_hDenom;
    bool isValid_hRatio;
    bool isValid_gRatio;

    // objects for fake rate
    TH1D* hNumFake;
    TH1D* hDenomFake;
    TH1D* hRatioFake;
    TGraphAsymmErrors* gRatioFake;
    // particles to be used for fake rate composition
    std::vector<int> fakeIndices;
    std::vector<int> fakePDGs;
    int nFakePDGs;
    // objects for each particle in fake rate composition
    /*
     * If the GEN-level particle matches the RECO-level fake, then fill hFakeParticle.
     */
    std::vector<TH1D*> hFakeParticle;
    /*
     * hRatioFakeParticle = hFakeParticle / hNumFake
     * hRatioFakeParticle is contribution of this particle to the total fake rate.
     */
    std::vector<TH1D*> hRatioFakeParticle;
    TH1D* hRatioFakeOther;

    // objects for particle composition of fake rate as function of GEN-level particle pt
    std::vector<TH1D*> hFakeParticleGenPt;
    /*
     * hRatioFakeParticleGenPt = hFakeParticleGenPt / hAllFakeParticlesGenPt
     * NOTE : x-axis of these histograms is the pt of the GEN-level particles that fake the RECO-level particle,
     * not necessarily the same as "dep".
     */
    TH1D* hAllFakeParticlesGenPt;
    std::vector<TH1D*> hRatioFakeParticleGenPt;
    TH1D* hRatioFakeOtherGenPt;

    bool isValid_hNumFake;
    bool isValid_hDenomFake;
    bool isValid_hRatioFake;
    bool isValid_gRatioFake;
    std::vector<bool> isValid_hFakeParticle;
    std::vector<bool> isValid_hRatioFakeParticle;
    bool isValid_hRatioFakeOther;
    std::vector<bool> isValid_hFakeParticleGenPt;
    std::vector<bool> isValid_hRatioFakeParticleGenPt;
    bool isValid_hAllFakeParticlesGenPt;
    bool isValid_hRatioFakeOtherGenPt;

    /*
     * Ex. minFakeFraction = 0.05 and there is particle a which makes up less 5% of the fakes for each bin along x-axis
     * That particle will not be plotted separately, but will be shown under "other" particle (hFakeRatioOther)
     */
    double minFakeFraction;
    std::vector<bool> passedMinFakeFraction;
    std::vector<bool> passedMinFakeFractionGenPt;

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

void energyScaleHist::FillH2D(double energyScale, double x, double w, float eta, float genPt, float recoPt, int cent)
{
    if (isValid_h2D && insideRange(eta, genPt, recoPt, cent))
        h2D->Fill(x, energyScale, w);
}

void energyScaleHist::FillH(double energyScale, double w, float eta, float genPt, float recoPt, int cent)
{
    if (isValid_hEscale && insideRange(eta, genPt, recoPt, cent))
        hEscale->Fill(energyScale, w);
}

void energyScaleHist::FillH2Dcorr(double genPt, double recoPt, double w, float eta, int cent)
{
    if (isValid_h2Dcorr && insideRange(eta, -1, -1, cent))
        h2Dcorr->Fill(genPt, recoPt, w);
}

void energyScaleHist::FillHNum(double x, double w, float genEta, float genPt, float recoPt, int cent)
{
    if (isValid_hNum && insideRange(genEta, genPt, recoPt, cent))
        hNum->Fill(x, w);
}

void energyScaleHist::FillHDenom(double x, double w, float genEta, float genPt, int cent)
{
    if (isValid_hDenom && insideRange(genEta, genPt, -1, cent))
        hDenom->Fill(x, w);
}
/*
 * fill numerator for fake rate
 * no GEN-level selection is applied
 */
void energyScaleHist::FillHNumFake(double x, double w, float eta, float recoPt, int cent)
{
    if (isValid_hNumFake && insideRange(eta, -1, recoPt, cent)) {
        hNumFake->Fill(x, w);
    }
}

/*
 * fill denominator for fake rate
 * no GEN-level selection is applied
 */
void energyScaleHist::FillHDenomFake(double x, double w, float eta, float recoPt, int cent)
{
    if (isValid_hDenomFake && insideRange(eta, -1, recoPt, cent))
        hDenomFake->Fill(x, w);
}

void energyScaleHist::FillHFakeParticle(double x, int pdg, double w, float eta, float genPt, float recoPt, int cent)
{
    if (insideRange(eta, genPt, recoPt, cent)) {

        std::vector<int>::iterator it = std::find(fakePDGs.begin(), fakePDGs.end(), pdg);
        int iPDG = (it == fakePDGs.end()) ? -1 : int(it - fakePDGs.begin());

        if (iPDG > -1 && isValid_hFakeParticle[iPDG]) {
            hFakeParticle[iPDG]->Fill(x, w);
        }
    }
}

void energyScaleHist::FillHFakeParticleGenPt(double genPt, int pdg, double w, float eta, float recoPt, int cent)
{
    if (insideRange(eta, -1, recoPt, cent)) {

        if (isValid_hAllFakeParticlesGenPt) {
            hAllFakeParticlesGenPt->Fill(genPt, w);
        }

        std::vector<int>::iterator it = std::find(fakePDGs.begin(), fakePDGs.end(), pdg);
        int iPDG = (it == fakePDGs.end()) ? -1 : int(it - fakePDGs.begin());

        if (iPDG > -1 && isValid_hFakeParticleGenPt[iPDG]) {
            hFakeParticleGenPt[iPDG]->Fill(genPt, w);
        }
    }
}

/*
 * check if the given variables lie inside the kinematic range defined for this object.
 */
bool energyScaleHist::insideRange(float eta, float genPt, float recoPt, int cent)
{
    // make sure to pass the selection if no explicit kinematic range is specified.
    if (eta == -999)  eta = ranges[ENERGYSCALE::kETA][0];
    if (genPt == -1) genPt = ranges[ENERGYSCALE::kGENPT][0];
    if (recoPt == -1)  recoPt = ranges[ENERGYSCALE::kRECOPT][0];
    if (cent == -1)  cent = ranges[ENERGYSCALE::kCENT][0];

    if(ranges[ENERGYSCALE::kETA][0] <= TMath::Abs(eta) &&
       (ranges[ENERGYSCALE::kETA][1] == -1 || ranges[ENERGYSCALE::kETA][1] > TMath::Abs(eta))){
    if(ranges[ENERGYSCALE::kGENPT][0] <= genPt         &&
       (ranges[ENERGYSCALE::kGENPT][1] == -1  || ranges[ENERGYSCALE::kGENPT][1] > genPt)) {
    if(ranges[ENERGYSCALE::kRECOPT][0] <= recoPt       &&
       (ranges[ENERGYSCALE::kRECOPT][1] == -1 || ranges[ENERGYSCALE::kRECOPT][1] > recoPt)) {
    if(ranges[ENERGYSCALE::kCENT][0] <= cent         &&
       (ranges[ENERGYSCALE::kCENT][1] == -1  || ranges[ENERGYSCALE::kCENT][1] > cent)) {
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

std::string energyScaleHist::getRangeTextCent()
{
    std::string res = "";

    if (ranges[ENERGYSCALE::DEPS::kCENT][0] >= 0 && ranges[ENERGYSCALE::DEPS::kCENT][1] > 0)
        res = Form("Cent:%.0f-%.0f%%", ranges[ENERGYSCALE::DEPS::kCENT][0], ranges[ENERGYSCALE::DEPS::kCENT][1]);

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
    else if (dep == ENERGYSCALE::kCENT) {
        res = Form("Cent:%.0f-%.0f%%", xLow, xUp);
    }
    else if (dep == ENERGYSCALE::kSUMISO) {
        res = Form("%.1f < sumIso < %.1f", xLow, xUp);
    }
    else if (dep == ENERGYSCALE::kSIEIE) {
        res = Form("%.3f < #sigma_{#eta#eta} < %.3f", xLow, xUp);
    }

    return res;
}

void energyScaleHist::updateTH1()
{
    isValid_h2D = (h2D != 0 && !h2D->IsZombie());
    if (isValid_h2D) {
        nBinsX = h2D->GetXaxis()->GetNbins();
    }
    isValid_hEscale = (hEscale != 0 && !hEscale->IsZombie());
    isValid_h2Dcorr = (h2Dcorr != 0 && !h2Dcorr->IsZombie());

    isValid_hNum = (hNum != 0 && !hNum->IsZombie());
    isValid_hDenom = (hDenom != 0 && !hDenom->IsZombie());
    isValid_hRatio = (hRatio != 0 && !hRatio->IsZombie());
    isValid_gRatio = (gRatio != 0 && !gRatio->IsZombie());

    isValid_hNumFake = (hNumFake != 0 && !hNumFake->IsZombie());
    isValid_hDenomFake = (hDenomFake != 0 && !hDenomFake->IsZombie());
    isValid_hRatioFake = (hRatioFake != 0 && !hRatioFake->IsZombie());
    isValid_gRatioFake = (gRatioFake != 0 && !gRatioFake->IsZombie());
    isValid_hRatioFakeOther = (hRatioFakeOther != 0 && !hRatioFakeOther->IsZombie());
    isValid_hRatioFakeOtherGenPt = (hRatioFakeOtherGenPt != 0 && !hRatioFakeOtherGenPt->IsZombie());

    for (int i = 0; i < nFakePDGs; ++i) {
        isValid_hFakeParticle[i] = (hFakeParticle[i] != 0 && !hFakeParticle[i]->IsZombie());
        isValid_hRatioFakeParticle[i] = (hRatioFakeParticle[i] != 0 && !hRatioFakeParticle[i]->IsZombie());

        isValid_hFakeParticleGenPt[i] = (hFakeParticleGenPt[i] != 0 && !hFakeParticleGenPt[i]->IsZombie());
        isValid_hRatioFakeParticleGenPt[i] = (hRatioFakeParticleGenPt[i] != 0 && !hRatioFakeParticleGenPt[i]->IsZombie());
    }

    isValid_hAllFakeParticlesGenPt = (hAllFakeParticlesGenPt != 0 && !hAllFakeParticlesGenPt->IsZombie());
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
        hTmp->GetXaxis()->CenterTitle();
        hTmp->GetYaxis()->CenterTitle();
        hTmp->SetMarkerStyle(kFullCircle);
        h1DsliceY[i-1] = hTmp;
    }
}

void energyScaleHist::updateH1DeScale()
{
    for (int i = 1; i <= nBinsX; ++i) {

        if (esa[i-1][indexFnc].isValid_h && esa[i-1][indexFnc].isValid_f1) {

            h1DeScale[0]->SetBinContent(i, esa[i-1][indexFnc].f1->GetParameter(1));
            h1DeScale[0]->SetBinError(i, esa[i-1][indexFnc].f1->GetParError(1));

            h1DeScale[1]->SetBinContent(i, esa[i-1][indexFnc].f1->GetParameter(2));
            h1DeScale[1]->SetBinError(i, esa[i-1][indexFnc].f1->GetParError(2));

            h1DeScale[4]->SetBinContent(i, esa[i-1][indexFnc].f1->GetParameter(0));
            h1DeScale[4]->SetBinError(i, esa[i-1][indexFnc].f1->GetParError(0));

            h1DeScale[5]->SetBinContent(i, esa[i-1][indexFnc].f1->GetChisquare()/esa[i-1][indexFnc].f1->GetNDF());
            h1DeScale[5]->SetBinError(i, 0);
        }

        // arithmetic mean and std dev of energy scale distributions
        double mean = h1DsliceY[i-1]->GetMean();
        double meanErr = h1DsliceY[i-1]->GetMeanError();
        h1DeScale[2]->SetBinContent(i, mean);
        h1DeScale[2]->SetBinError(i, meanErr);

        double stdDev = h1DsliceY[i-1]->GetStdDev();
        double stdDevErr = h1DsliceY[i-1]->GetStdDevError();;
        h1DeScale[3]->SetBinContent(i, stdDev);
        h1DeScale[3]->SetBinContent(i, stdDevErr);
    }
}

int energyScaleHist::getFakePDG(int iPDG)
{
    if (iPDG < nFakePDGs)  return fakePDGs[iPDG];
    else                   return 0;
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
    std::string centStr  = "";      // whole centrality range

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

    if (ranges[ENERGYSCALE::kCENT][0] <= 0 && ranges[ENERGYSCALE::kCENT][1] <= -1)
        centStr = "";
    else if (ranges[ENERGYSCALE::kCENT][0] >= 0 && ranges[ENERGYSCALE::kCENT][1] > 0)
        centStr  = Form("Cent:%.0f-%.0f%%", ranges[ENERGYSCALE::kCENT][0], ranges[ENERGYSCALE::kCENT][1]);

    std::string tmpHistTitle = "";
    if (etaStr.size() > 0)  tmpHistTitle.append(Form("%s", etaStr.c_str()));
    if (genPtStr.size() > 0)  tmpHistTitle.append(Form(" %s", genPtStr.c_str()));
    if (recoPtStr.size() > 0) tmpHistTitle.append(Form(" %s", recoPtStr.c_str()));
    if (centStr.size() > 0)  tmpHistTitle.append(Form(" %s", centStr.c_str()));

    title = tmpHistTitle.c_str();
    if(isValid_h2D) {
        h2D->SetTitle(title.c_str());
        titleX = h2D->GetXaxis()->GetTitle();
    }
    if (isValid_hEscale) {
        hEscale->SetTitle(title.c_str());
    }
    if(isValid_h2Dcorr) {
        h2Dcorr->SetTitle(title.c_str());
    }
    if(isValid_hNum) {
        hNum->SetTitle(title.c_str());
    }
    if(isValid_hDenom) {
        hDenom->SetTitle(title.c_str());
    }
    if(isValid_gRatio) {
        gRatio->SetTitle(title.c_str());
    }
    if(isValid_hRatio) {
        hRatio->SetTitle(title.c_str());
    }
}

void energyScaleHist::postLoop()
{
    if (isValid_hEscale) {
        hEscale->SetMarkerStyle(kFullCircle);
    }

    if (!isValid_h2D) return;

    TObjArray aSlices;
    h2D->FitSlicesY(0,0,-1,0,"Q LL M N", &aSlices);

    // energy scale
    h1DeScale[0] = (TH1D*)aSlices.At(1)->Clone(Form("h1D_%s_%s", ENERGYSCALE::OBS_LABELS[0].c_str(), name.c_str()));
    h1DeScale[0]->SetTitle(title.c_str());
    h1DeScale[0]->SetXTitle(titleX.c_str());
    setTH1_energyScale(h1DeScale[0], titleOffsetX, titleOffsetY);
    if (yMax[ENERGYSCALE::kESCALE] > yMin[ENERGYSCALE::kESCALE])
        h1DeScale[0]->SetAxisRange(yMin[ENERGYSCALE::kESCALE], yMax[ENERGYSCALE::kESCALE], "Y");

    // width of energy scale
    h1DeScale[1] = (TH1D*)aSlices.At(2)->Clone(Form("h1D_%s_%s", ENERGYSCALE::OBS_LABELS[1].c_str(), name.c_str()));
    h1DeScale[1]->SetTitle(title.c_str());
    h1DeScale[1]->SetXTitle(titleX.c_str());
    setTH1_energyWidth(h1DeScale[1], titleOffsetX, titleOffsetY);
    if (yMax[ENERGYSCALE::kERES] > yMin[ENERGYSCALE::kERES])
        h1DeScale[1]->SetAxisRange(yMin[ENERGYSCALE::kERES], yMax[ENERGYSCALE::kERES], "Y");

    // energy scale with arithmetic mean
    h1DeScale[2] = (TH1D*)h1DeScale[0]->Clone(Form("h1D_%s_%s", ENERGYSCALE::OBS_LABELS[2].c_str(), name.c_str()));
    h1DeScale[2]->SetTitle(title.c_str());
    h1DeScale[2]->SetXTitle(titleX.c_str());
    setTH1_energyScale(h1DeScale[2], titleOffsetX, titleOffsetY);
    h1DeScale[2]->SetYTitle(Form("%s (Arith)", h1DeScale[2]->GetYaxis()->GetTitle()));
    if (yMax[ENERGYSCALE::kESCALE] > yMin[ENERGYSCALE::kESCALE])
        h1DeScale[2]->SetAxisRange(yMin[ENERGYSCALE::kESCALE], yMax[ENERGYSCALE::kESCALE], "Y");

    // width of energy scale with arithmetic std dev
    h1DeScale[3] = (TH1D*)h1DeScale[1]->Clone(Form("h1D_%s_%s", ENERGYSCALE::OBS_LABELS[3].c_str(), name.c_str()));
    h1DeScale[3]->SetTitle(title.c_str());
    h1DeScale[3]->SetXTitle(titleX.c_str());
    setTH1_energyWidth(h1DeScale[3], titleOffsetX, titleOffsetY);
    h1DeScale[3]->SetYTitle(Form("%s (Arith)", h1DeScale[3]->GetYaxis()->GetTitle()));
    if (yMax[ENERGYSCALE::kERES] > yMin[ENERGYSCALE::kERES])
        h1DeScale[3]->SetAxisRange(yMin[ENERGYSCALE::kERES], yMax[ENERGYSCALE::kERES]/20, "Y");

    // Constant for Gaussian fit
    h1DeScale[4] = (TH1D*)aSlices.At(0)->Clone(Form("h1D_gausConst_%s", name.c_str()));
    h1DeScale[4]->SetTitle(title.c_str());
    h1DeScale[4]->SetXTitle(titleX.c_str());
    h1DeScale[4]->SetYTitle("Constant for Gaussian fit");
    h1DeScale[4]->SetStats(false);
    h1DeScale[4]->SetMarkerStyle(kFullCircle);

    // chi2/ndf for Gaussian fit
    h1DeScale[5] = (TH1D*)aSlices.At(3)->Clone(Form("h1D_gausChi2ndf_%s", name.c_str()));
    h1DeScale[5]->SetTitle(title.c_str());
    h1DeScale[5]->SetXTitle(titleX.c_str());
    h1DeScale[5]->SetYTitle("chi2/ndf for Gaussian fit");
    h1DeScale[5]->SetStats(false);
    h1DeScale[5]->SetMarkerStyle(kFullCircle);

    updateH1DsliceY();

    fitRecoGen();
    // up to this point bins of h1DeScale[0], h1DeScale[1], h1DeScale[4], h1DeScale[5] are set by the initial fit from TH2::FitSlicesY
    updateH1DeScale();

    calcRatio();
    calcRatioFake();
    calcRatioFakeParticle();
    calcRatioFakeParticleGenPt();

    // Final histograms point to observables
    h1D[ENERGYSCALE::kESCALE] = h1DeScale[ENERGYSCALE::kESCALE];
    h1D[ENERGYSCALE::kERES] = h1DeScale[ENERGYSCALE::kERES];
    h1D[ENERGYSCALE::kESCALEARITH] = h1DeScale[ENERGYSCALE::kESCALEARITH];
    h1D[ENERGYSCALE::kERESARITH] = h1DeScale[ENERGYSCALE::kERESARITH];

    if (isValid_hRatio)  h1D[ENERGYSCALE::kEFF] = hRatio;
    if (isValid_hRatioFake)  h1D[ENERGYSCALE::kFAKE] = hRatioFake;
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

        double p0 = h1DeScale[4]->GetBinContent(i);   // constant
        double p0Err = h1DeScale[4]->GetBinError(i);

        double p1 = h1DeScale[0]->GetBinContent(i);   // mean
        double p1Err = h1DeScale[0]->GetBinError(i);

        double p2 = h1DeScale[1]->GetBinContent(i);   // StdDev
        double p2Err = h1DeScale[1]->GetBinError(i);

        int binMax = hTmp->GetMaximumBin();
        int nBinsTmp = hTmp->GetNbinsX();

        // fit functions for that bin along x-axis of h2D
        std::vector<TF1*> f1sTmp;
        for (int iFnc = 0; iFnc < ENERGYSCALE::kN_FNCS; ++iFnc) {

            f1Tmp = new TF1(Form("f1_bin%d_fnc%d_%s", i, iFnc, name.c_str()), ENERGYSCALE::fncFormulas[iFnc].c_str());

            std::vector<int> fncRange = getLeftRightBins4IntegralFraction(hTmp, binMax, ENERGYSCALE::intFractions[iFnc]);
            int binLow = std::max(fncRange[0], 1);
            int binUp  = std::min(fncRange[1], nBinsTmp);
            f1Tmp->SetRange(hTmp->GetBinLowEdge(binLow), hTmp->GetBinLowEdge(binUp+1));

            f1Tmp->SetLineColor(ENERGYSCALE::fncColors[iFnc]);

            if (iFnc == ENERGYSCALE::kGAUS_FitSlicesY) {
                // initial fit from TH2::FitSlicesY
                f1Tmp->SetParameters(p0, p1, p2);
                double parErr[3] = {p0Err, p1Err, p2Err};
                f1Tmp->SetParErrors(parErr);
                //        double chi2ndf = h1DeScale[5]->GetBinContent(i);
                //        f1Tmp->SetChisquare(chi2ndf*100);
                //        f1Tmp->SetNDF(100);
            }
            else {
                if (ENERGYSCALE::fncFormulas[iFnc] == "gaus") {
                    // use the fit from TH2::FitSlicesY as seed
                    f1Tmp->SetParameters(p0, p1, p2);
                }
            }

            f1sTmp.push_back(f1Tmp);
        }

        int nF1s = f1sTmp.size();
        std::vector<eScaleAna> esaTmp(nF1s);
        for (int j = 0; j < nF1s; ++j) {
            esaTmp[j].h = hTmp;
            esaTmp[j].isValid_h = true;
            esaTmp[j].f1 = f1sTmp[j];
            esaTmp[j].isValid_f1 = true;

            esaTmp[j].fitOption = ENERGYSCALE::fitOption;
            // j = 0 corresponds to fit initial from TH2::FitSlicesY, do not refit.
            if (j > 0)  esaTmp[j].fit();

            esaTmp[j].makePull();
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

void energyScaleHist::calcRatio()
{
    if (!isValid_hNum || !isValid_hDenom) return;

    hNum->SetTitle(title.c_str());
    hNum->SetXTitle(titleX.c_str());
    setTH1_efficiency(hNum, titleOffsetX, titleOffsetY);

    hDenom->SetTitle(title.c_str());
    hDenom->SetXTitle(titleX.c_str());
    setTH1_efficiency(hDenom, titleOffsetX, titleOffsetY);

    if (isValid_gRatio) {
        gRatio->Delete();
        isValid_gRatio = false;
    }

    gRatio = new TGraphAsymmErrors();
    gRatio->SetName(Form("gRatio_%s", name.c_str()));
    gRatio->BayesDivide(hNum, hDenom);
    gRatio->SetTitle(title.c_str());
    gRatio->GetXaxis()->SetTitle(titleX.c_str());
    gRatio->GetYaxis()->SetTitle("Efficiency");
    gRatio->SetMarkerStyle(kFullCircle);

    isValid_gRatio = true;

    if (isValid_hRatio) {
        hRatio->Delete();
        isValid_hRatio = false;
    }

    hRatio = (TH1D*)hNum->Clone(Form("hRatio_%s", name.c_str()));
    fillTH1fromTGraph(hRatio, gRatio);
    setTH1_efficiency(hDenom, titleOffsetX, titleOffsetY);
    hRatio->SetTitle(title.c_str());
    hRatio->SetXTitle(titleX.c_str());
    hRatio->SetYTitle("Efficiency");
    hRatio->SetMinimum(0);
    hRatio->SetMaximum(1.2);

    isValid_hRatio = true;
}

void energyScaleHist::calcRatioFake()
{
    if (!isValid_hNumFake || !isValid_hDenomFake) return;

    hNumFake->SetTitle(title.c_str());
    hNumFake->SetXTitle(titleX.c_str());
    setTH1_efficiency(hNumFake, titleOffsetX, titleOffsetY);

    hDenomFake->SetTitle(title.c_str());
    hDenomFake->SetXTitle(titleX.c_str());
    setTH1_efficiency(hDenomFake, titleOffsetX, titleOffsetY);

    if (isValid_gRatioFake) {
        gRatioFake->Delete();
        isValid_gRatioFake = false;
    }

    gRatioFake = new TGraphAsymmErrors();
    gRatioFake->SetName(Form("gRatioFake_%s", name.c_str()));
    gRatioFake->BayesDivide(hNumFake, hDenomFake);
    gRatioFake->SetTitle(title.c_str());
    gRatioFake->GetXaxis()->SetTitle(titleX.c_str());
    gRatioFake->GetYaxis()->SetTitle("Efficiency");
    gRatioFake->SetMarkerStyle(kFullCircle);

    isValid_gRatioFake = true;

    if (isValid_hRatioFake) {
        hRatioFake->Delete();
        isValid_hRatioFake = false;
    }

    hRatioFake = (TH1D*)hNumFake->Clone(Form("hRatioFake_%s", name.c_str()));
    fillTH1fromTGraph(hRatioFake, gRatioFake);
    setTH1_efficiency(hDenomFake, titleOffsetX, titleOffsetY);
    hRatioFake->SetTitle(title.c_str());
    hRatioFake->SetXTitle(titleX.c_str());
    hRatioFake->SetYTitle("Fake Rate");
    hRatioFake->SetMinimum(0);
    hRatioFake->SetMaximum(1.2);

    isValid_hRatioFake = true;
}

void energyScaleHist::calcRatioFakeParticle()
{
    if (isValid_hNumFake) {
        hRatioFakeOther = (TH1D*)hNumFake->Clone(Form("hRatioFakeOther_%s", name.c_str()));
        hRatioFakeOther->Reset();

        setTH1_efficiency(hRatioFakeOther, titleOffsetX, titleOffsetY);
        hRatioFakeOther->SetTitle(title.c_str());
        hRatioFakeOther->SetXTitle(titleX.c_str());
        hRatioFakeOther->SetYTitle("Contribution to Fakes, Other");
        hRatioFakeOther->SetMinimum(0);
        hRatioFakeOther->SetMaximum(1.5);

        isValid_hRatioFakeOther = true;
    }

    for (int i = 0; i < nFakePDGs; ++i) {
        if (!isValid_hFakeParticle[i])  continue;

        hFakeParticle[i]->SetTitle(title.c_str());
        hFakeParticle[i]->SetXTitle(titleX.c_str());
        setTH1_efficiency(hFakeParticle[i], titleOffsetX, titleOffsetY);

        if (isValid_hRatioFakeParticle[i]) {
            hRatioFakeParticle[i]->Delete();
            isValid_hRatioFakeParticle[i] = false;
        }

        if (!isValid_hNumFake)  continue;

        std::string label = ENERGYSCALE::particles[i].label;
        std::string tmpHistName = replaceAll(hFakeParticle[i]->GetName(), "FakePDG", "RatioFakePDG");
        hRatioFakeParticle[i] = (TH1D*)hFakeParticle[i]->Clone(tmpHistName.c_str());
        hRatioFakeParticle[i]->Divide(hNumFake);
        setTH1_efficiency(hRatioFakeParticle[i], titleOffsetX, titleOffsetY);
        hRatioFakeParticle[i]->SetTitle(title.c_str());
        hRatioFakeParticle[i]->SetXTitle(titleX.c_str());
        hRatioFakeParticle[i]->SetYTitle(Form("Contribution to Fakes, %s", label.c_str()));
        hRatioFakeParticle[i]->SetMinimum(0);
        hRatioFakeParticle[i]->SetMaximum(1.5);

        isValid_hRatioFakeParticle[i] = true;

        double maxContent = hRatioFakeParticle[i]->GetBinContent(hRatioFakeParticle[i]->GetMaximumBin());
        passedMinFakeFraction[i] = (maxContent >= minFakeFraction);

        if (isValid_hRatioFakeOther && !passedMinFakeFraction[i]) {
            hRatioFakeOther->Add(hRatioFakeParticle[i]);
        }
    }

    // check if bin contents sum up to 1
    if (isValid_hRatioFakeOther) {
        for (int iBin = 1; iBin<=hRatioFakeOther->GetNbinsX(); ++iBin) {
            double content = 0;

            for (int i = 0; i < nFakePDGs; ++i) {
                if (isValid_hRatioFakeParticle[i] && passedMinFakeFraction[i]) {
                    content += hRatioFakeParticle[i]->GetBinContent(iBin);
                }
            }

            if (isValid_hRatioFakeOther) {
                content += hRatioFakeOther->GetBinContent(iBin);
            }

            if (content < 0.99 || content > 1.01) {
                std::cout << "WARNING : bin contents do not sum up to 1." << name.c_str() << std::endl;
                std::cout << "name = " << name.c_str() << std::endl;
                std::cout << "hRatioFakeParticle" << std::endl;
                std::cout << Form("bin, content = %d, %f", iBin, content) << std::endl;
            }
        }
    }
}

void energyScaleHist::calcRatioFakeParticleGenPt()
{
    if (isValid_hAllFakeParticlesGenPt) {
        hRatioFakeOtherGenPt = (TH1D*)hAllFakeParticlesGenPt->Clone(Form("hRatioFakeOtherGenPt_%s", name.c_str()));
        hRatioFakeOtherGenPt->Reset();

        setTH1_efficiency(hRatioFakeOtherGenPt, titleOffsetX, titleOffsetY);
        hRatioFakeOtherGenPt->SetTitle(title.c_str());
        hRatioFakeOtherGenPt->SetXTitle(titleX.c_str());
        hRatioFakeOtherGenPt->SetYTitle("Contribution to Fakes, Other");
        hRatioFakeOtherGenPt->SetMinimum(0);
        hRatioFakeOtherGenPt->SetMaximum(1.5);

        isValid_hRatioFakeOtherGenPt = true;
    }

    for (int i = 0; i < nFakePDGs; ++i) {
        if (!isValid_hFakeParticleGenPt[i])  continue;

        hFakeParticleGenPt[i]->SetTitle(title.c_str());
        hFakeParticleGenPt[i]->SetXTitle("Gen p_{T} (GeV/c)");
        setTH1_efficiency(hFakeParticleGenPt[i], titleOffsetX, titleOffsetY);

        if (isValid_hRatioFakeParticleGenPt[i]) {
            hRatioFakeParticleGenPt[i]->Delete();
            isValid_hRatioFakeParticleGenPt[i] = false;
        }

        if (!isValid_hAllFakeParticlesGenPt)  continue;

        std::string label = ENERGYSCALE::particles[i].label;
        std::string tmpHistName = replaceAll(hFakeParticleGenPt[i]->GetName(), "FakeGenPtPDG", "RatioFakeGenPtPDG");
        hRatioFakeParticleGenPt[i] = (TH1D*)hFakeParticleGenPt[i]->Clone(tmpHistName.c_str());
        hRatioFakeParticleGenPt[i]->Divide(hAllFakeParticlesGenPt);
        setTH1_efficiency(hRatioFakeParticleGenPt[i], titleOffsetX, titleOffsetY);
        hRatioFakeParticleGenPt[i]->SetTitle(title.c_str());
        hRatioFakeParticleGenPt[i]->SetXTitle("Gen p_{T} (GeV/c)");
        hRatioFakeParticleGenPt[i]->SetYTitle(Form("Contribution to Fakes, %s", label.c_str()));
        hRatioFakeParticleGenPt[i]->SetMinimum(0);
        hRatioFakeParticleGenPt[i]->SetMaximum(1.5);

        isValid_hRatioFakeParticleGenPt[i] = true;

        double maxContent = hRatioFakeParticleGenPt[i]->GetBinContent(hRatioFakeParticleGenPt[i]->GetMaximumBin());
        passedMinFakeFractionGenPt[i] = (maxContent >= minFakeFraction);

        if (isValid_hRatioFakeOtherGenPt && !passedMinFakeFractionGenPt[i]) {
            hRatioFakeOtherGenPt->Add(hRatioFakeParticleGenPt[i]);
        }
    }

    // check if bin contents sum up to 1
    if (isValid_hRatioFakeOtherGenPt) {
        for (int iBin = 1; iBin<=hRatioFakeOtherGenPt->GetNbinsX(); ++iBin) {
            double content = 0;

            for (int i = 0; i < nFakePDGs; ++i) {
                if (isValid_hRatioFakeParticleGenPt[i] && passedMinFakeFractionGenPt[i]) {
                    content += hRatioFakeParticleGenPt[i]->GetBinContent(iBin);
                }
            }

            if (isValid_hRatioFakeOtherGenPt) {
                content += hRatioFakeOtherGenPt->GetBinContent(iBin);
            }

            if (content < 0.99 || content > 1.01) {
                std::cout << "WARNING : bin contents do not sum up to 1." << name.c_str() << std::endl;
                std::cout << "name = " << name.c_str() << std::endl;
                std::cout << "hRatioFakeParticleGenPt" << std::endl;
                std::cout << Form("bin, content = %d, %f", iBin, content) << std::endl;
            }
        }
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
    if (isValid_hEscale) {
        hEscale->SetMarkerStyle(kFullCircle);
        hEscale->Write();
    }
    if (isValid_h2Dcorr)
        h2Dcorr->Write();

    if (!isValid_h2D) return;

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
    setPad4Observable((TPad*) c, 0);
    setCanvasFinal(c);
    c->Write("",TObject::kOverwrite);
    c->Close();         // do not use Delete() for TCanvas.

    if (isValid_h2Dcorr) {
        canvasName = replaceAll(h2Dcorr->GetName(), "h2Dcorr", "cnv2Dcorr");
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        h2Dcorr->SetTitleOffset(titleOffsetX, "X");
        h2Dcorr->SetTitleOffset(titleOffsetY, "Y");
        h2Dcorr->SetStats(false);
        h2Dcorr->Draw("colz");
        h2Dcorr->Write("",TObject::kOverwrite);

        // draw y = x correlation line
        c->Update();
        line = new TLine(c->GetUxmin(), c->GetUymin(), c->GetUxmax(), c->GetUymax());
        line->SetLineStyle(kDashed);
        line->SetLineWidth(line->GetLineWidth()*2);
        line->Draw();

        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.
    }

    // energy scale
    canvasName = Form("cnv_%s_%s", ENERGYSCALE::OBS_LABELS[0].c_str(), name.c_str());
    c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
    c->cd();
    setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
    float markerSize = (float)windowWidth/600;
    h1DeScale[0]->SetMarkerSize(markerSize);
    h1DeScale[0]->Draw("e");
    h1DeScale[0]->Write("",TObject::kOverwrite);
    setPad4Observable((TPad*) c, 0);
    setCanvasFinal(c);
    c->Write("",TObject::kOverwrite);
    c->Close();         // do not use Delete() for TCanvas.

    // width of energy scale
    canvasName = Form("cnv_%s_%s", ENERGYSCALE::OBS_LABELS[1].c_str(), name.c_str());
    c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
    c->cd();
    setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
    h1DeScale[1]->SetMarkerSize(markerSize);
    h1DeScale[1]->Draw("e");
    h1DeScale[1]->Write("",TObject::kOverwrite);
    setPad4Observable((TPad*) c, 1);
    setCanvasFinal(c);
    c->Write("",TObject::kOverwrite);
    c->Close();         // do not use Delete() for TCanvas.

    // energy scale from histogram mean
    canvasName = Form("cnv_%s_%s", ENERGYSCALE::OBS_LABELS[2].c_str(), name.c_str());
    c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
    c->cd();
    setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
    markerSize = (float)windowWidth/600;
    h1DeScale[2]->SetMarkerSize(markerSize);
    h1DeScale[2]->Draw("e");
    h1DeScale[2]->Write("",TObject::kOverwrite);
    setPad4Observable((TPad*) c, 2);
    setCanvasFinal(c);
    c->Write("",TObject::kOverwrite);
    c->Close();         // do not use Delete() for TCanvas.

    // width of energy scale from histogram std dev
    canvasName = Form("cnv_%s_%s", ENERGYSCALE::OBS_LABELS[3].c_str(), name.c_str());
    c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
    c->cd();
    setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
    h1DeScale[3]->SetMarkerSize(markerSize);
    h1DeScale[3]->Draw("e");
    h1DeScale[3]->Write("",TObject::kOverwrite);
    setPad4Observable((TPad*) c, 3);
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
            if (esa[i][iFnc].isValid_f1) {
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
    // plot the same canvas in log-scale as well
    for (int i = 0; i < nH1DsliceY; ++i) {
        pads[i]->SetLogy(1);
        pads[i]->Update();
    }
    canvasName = replaceAll(c->GetName(), "cnv_", "cnvLogy_");
    c->SetName(canvasName.c_str());
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
            if (esa[i][j].isValid_hPull)  {

                esa[i][j].hPull->SetMarkerSize(markerSize);

                if (j != indexFnc)  esa[i][j].hPull->SetMarkerStyle(kOpenSquare);

                std::string drawOption = "e same";
                if (j == j1)  drawOption = "e";
                esa[i][j].hPull->Draw(drawOption.c_str());
            }
        }

        if (nEsa > 0 && esa[i][j1].isValid_h) {
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

    // efficiency objects
    if (isValid_hNum) {
        canvasName = Form("cnv_Num_%s", name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        hNum->SetMarkerSize(markerSize);
        hNum->Draw("e");
        hNum->Write("",TObject::kOverwrite);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.
    }
    if (isValid_hDenom) {
        canvasName = Form("cnv_Denom_%s", name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        hDenom->SetMarkerSize(markerSize);
        hDenom->Draw("e");
        hDenom->Write("",TObject::kOverwrite);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.
    }
    if (isValid_hRatio) {
        int iObs = ENERGYSCALE::kEFF;
        canvasName = Form("cnv_%sH1D_%s", ENERGYSCALE::OBS_LABELS[iObs].c_str() , name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        hRatio->SetMarkerSize(markerSize);
        hRatio->Draw("e");
        hRatio->Write("",TObject::kOverwrite);
        setPad4Observable((TPad*) c, iObs);
        drawLine4PtRange((TPad*) c);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.
    }
    if (isValid_hRatio && isValid_gRatio) {
        int iObs = ENERGYSCALE::kEFF;
        canvasName = Form("cnv_%s_%s", ENERGYSCALE::OBS_LABELS[iObs].c_str() , name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        // dummy histogram to be used as template for the graph
        TH1D* hTmp = (TH1D*)hRatio->Clone("hTmp");
        hTmp->Reset();
        hTmp->Draw();
        gRatio->SetMarkerSize(markerSize);
        gRatio->Draw("p e");
        gRatio->Write("",TObject::kOverwrite);
        setPad4Observable((TPad*) c, iObs);
        drawLine4PtRange((TPad*) c);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.
        hTmp->Delete();
    }
    // fake rate objects
    if (isValid_hNumFake) {
        canvasName = Form("cnv_NumFake_%s", name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        hNumFake->SetMarkerSize(markerSize);
        hNumFake->Draw("e");
        hNumFake->Write("",TObject::kOverwrite);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.
    }
    if (isValid_hDenomFake) {
        canvasName = Form("cnv_DenomFake_%s", name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        hDenomFake->SetMarkerSize(markerSize);
        hDenomFake->Draw("e");
        hDenomFake->Write("",TObject::kOverwrite);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.
    }
    if (isValid_hRatioFake) {
        int iObs = ENERGYSCALE::kFAKE;
        canvasName = Form("cnv_%s_%s", ENERGYSCALE::OBS_LABELS[iObs].c_str() , name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        hRatioFake->SetMarkerSize(markerSize);
        hRatioFake->Draw("e");
        hRatioFake->Write("",TObject::kOverwrite);
        setPad4Observable((TPad*) c, iObs);
        drawLine4PtRange((TPad*) c);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);

        // plot fake rate in log-scale as well
        double minContent = hRatioFake->GetBinContent(getMinimumBin(hRatioFake, 0));
        double logYmin = TMath::Floor(TMath::Log10(minContent));
        hRatioFake->SetMinimum(TMath::Power(10, logYmin));
        c->SetLogy(1);
        c->Update();
        canvasName = replaceAll(c->GetName(), "cnv_", "cnvLogy_");
        c->SetName(canvasName.c_str());
        c->Write("",TObject::kOverwrite);
        hRatioFake->SetMinimum(0);  // restore the minimum after log-scale canvas.

        c->Close();         // do not use Delete() for TCanvas.
    }
    if (isValid_hRatioFake && isValid_gRatioFake) {
        int iObs = ENERGYSCALE::kFAKE;
        canvasName = Form("cnv_%sgraph_%s", ENERGYSCALE::OBS_LABELS[iObs].c_str() , name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        // dummy histogram to be used as template for the graph
        TH1D* hTmp = (TH1D*)hRatioFake->Clone("hTmp");
        hTmp->Reset();
        hTmp->Draw();
        gRatioFake->SetMarkerSize(markerSize);
        gRatioFake->Draw("p e");
        gRatioFake->Write("",TObject::kOverwrite);
        setPad4Observable((TPad*) c, iObs);
        drawLine4PtRange((TPad*) c);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);

        // plot fake rate in log-scale as well
        double minContent = getMinimum(gRatioFake, 0);
        double logYmin = TMath::Floor(TMath::Log10(minContent));
        hTmp->SetMinimum(TMath::Power(10, logYmin));
        c->SetLogy(1);
        c->Update();
        canvasName = replaceAll(c->GetName(), "cnv_", "cnvLogy_");
        c->SetName(canvasName.c_str());
        c->Write("",TObject::kOverwrite);

        c->Close();         // do not use Delete() for TCanvas.
        hTmp->Delete();
    }
    // particle composition of fake rate
    for (int i = 0; i < nFakePDGs; ++i) {

        if (!isValid_hFakeParticle[i])  continue;
        hFakeParticle[i]->Write("",TObject::kOverwrite);

        if (!isValid_hRatioFakeParticle[i])  continue;
        hRatioFakeParticle[i]->Write("",TObject::kOverwrite);
    }
    if (isValid_hNumFake) {

        int iObs = ENERGYSCALE::kFAKE;
        canvasName = Form("cnv_%sPDGs_%s", ENERGYSCALE::OBS_LABELS[iObs].c_str() , name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        TH1D* hTmp = 0;

        if (isValid_hRatioFakeOther) {
            // dummy histogram to be used as template
            hTmp = (TH1D*)hRatioFakeOther->Clone("hTmp");
            hTmp->Reset();
            hTmp->SetYTitle("Fake Composition");
            hTmp->SetMaximum(1.5);
            hTmp->Draw();
        }

        TLegend* leg = new TLegend();
        for (int i = 0; i < nFakePDGs; ++i) {

            if (!isValid_hRatioFakeParticle[i])  continue;
            if (!passedMinFakeFraction[i])  continue;

            hRatioFakeParticle[i]->SetMarkerSize(markerSize);
            hRatioFakeParticle[i]->SetMarkerColor(GRAPHICS::colors[(i+1)%13]);
            hRatioFakeParticle[i]->Draw("e same");

            int index = fakeIndices[i];
            std::string label = ENERGYSCALE::particles[index].label;
            leg->AddEntry(hRatioFakeParticle[i], label.c_str(), "lpf");
        }

        if (isValid_hRatioFakeOther) {
            hRatioFakeOther->Draw("e same");
            leg->AddEntry(hRatioFakeOther, "Other", "lpf");
        }
        double legHeight = calcTLegendHeight(leg);
        double legWidth = calcTLegendWidth(leg);
        int nLegRows = leg->GetNRows();
        if (nLegRows >= 6) {
            leg->SetNColumns(2);
            legHeight /= 2;
            legWidth *= 1.4;
        }
        setLegendPosition(leg, "NW", c, legHeight, legWidth, 0.04, 0.04, true);
        leg->SetFillColor(-1);
        leg->SetFillStyle(4000);
        leg->SetBorderSize(0);
        leg->Draw();

        setPad4Observable((TPad*) c, iObs);
        drawLine4PtRange((TPad*) c);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.
        leg->Delete();
        if (hTmp != 0)  hTmp->Delete();
    }
    // particle composition of fake rate as function of GEN-level particle pt
     for (int i = 0; i < nFakePDGs; ++i) {

         if (!isValid_hFakeParticleGenPt[i])  continue;
         hFakeParticleGenPt[i]->Write("",TObject::kOverwrite);

         if (!isValid_hRatioFakeParticleGenPt[i])  continue;
         hRatioFakeParticleGenPt[i]->Write("",TObject::kOverwrite);
     }
     if (isValid_hAllFakeParticlesGenPt) {

         hAllFakeParticlesGenPt->Write("",TObject::kOverwrite);

         int iObs = ENERGYSCALE::kFAKE;
         canvasName = Form("cnv_%sGenPtPDGs_%s", ENERGYSCALE::OBS_LABELS[iObs].c_str() , name.c_str());
         c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
         c->cd();
         setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
         TH1D* hTmp = 0;

         if (isValid_hRatioFakeOtherGenPt) {
             // dummy histogram to be used as template
             hTmp = (TH1D*)hRatioFakeOtherGenPt->Clone("hTmp");
             hTmp->Reset();
             hTmp->SetYTitle("Fake Composition");
             hTmp->SetMaximum(1.5);
             hTmp->Draw();
         }

         TLegend* leg = new TLegend();
         for (int i = 0; i < nFakePDGs; ++i) {

             if (!isValid_hRatioFakeParticleGenPt[i])  continue;
             if (!passedMinFakeFractionGenPt[i])  continue;

             hRatioFakeParticleGenPt[i]->SetMarkerSize(markerSize);
             hRatioFakeParticleGenPt[i]->SetMarkerColor(GRAPHICS::colors[(i+1)%13]);
             hRatioFakeParticleGenPt[i]->Draw("e same");

             int index = fakeIndices[i];
             std::string label = ENERGYSCALE::particles[index].label;
             leg->AddEntry(hRatioFakeParticleGenPt[i], label.c_str(), "lpf");
         }

         if (isValid_hRatioFakeOtherGenPt) {
             hRatioFakeOtherGenPt->SetMarkerSize(markerSize);
             hRatioFakeOtherGenPt->Draw("e same");
             leg->AddEntry(hRatioFakeOtherGenPt, "Other", "lpf");
         }
         double legHeight = calcTLegendHeight(leg);
         double legWidth = calcTLegendWidth(leg);
         int nLegRows = leg->GetNRows();
         if (nLegRows >= 6) {
             leg->SetNColumns(2);
             legHeight /= 2;
             legWidth *= 1.4;
         }
         setLegendPosition(leg, "NW", c, legHeight, legWidth, 0.04, 0.04, true);
         leg->SetFillColor(-1);
         leg->SetFillStyle(4000);
         leg->SetBorderSize(0);
         leg->Draw();

         setPad4Observable((TPad*) c, iObs);
         drawLine4PtRange((TPad*) c);
         setCanvasFinal(c);
         c->Write("",TObject::kOverwrite);
         c->Close();         // do not use Delete() for TCanvas.
         leg->Delete();
         if (hTmp != 0)  hTmp->Delete();
     }
}

void energyScaleHist::setPad4Observable(TPad* p, int iObs, int iDep)
{
    TLine* line = 0;

    p->Update();
    if (iObs == ENERGYSCALE::kESCALE || iObs == ENERGYSCALE::kESCALEARITH ||
        iObs == ENERGYSCALE::kEFF || iObs == ENERGYSCALE::kFAKE) {

        // draw line y = 1
        double x1 = p->GetUxmin();
        double x2 = p->GetUxmax();
        double y1 = p->GetUymin();
        double y2 = p->GetUymax();
        if (y1 < 1 && 1 < y2) {
            line = new TLine(x1, 1, x2, 1);
            line->SetLineStyle(kDashed);
            line->SetLineWidth(line->GetLineWidth()*2);
            line->Draw();
        }
    }

    if (iDep == ENERGYSCALE::kETA) {
        // draw line for EE-EB transition
        double ECAL_boundary_1 = 1.4442;
        double ECAL_boundary_2 = 1.566;

        double yMin = p->GetUymin();
        double yMax = p->GetUymax();
        if (iObs == ENERGYSCALE::kEFF)  yMax = 1;

        // draw lines for ECAL transition region
        std::vector<double> lineXvalues {-1*ECAL_boundary_1, ECAL_boundary_1, -1*ECAL_boundary_2, ECAL_boundary_2};
        for (std::vector<double>::const_iterator itLine = lineXvalues.begin(); itLine !=lineXvalues.end(); ++itLine) {

            line = new TLine((*itLine), yMin, (*itLine), yMax);
            line->SetLineStyle(kDashed);
            line->Draw();
        }
    }
}

void energyScaleHist::setPad4Observable(TPad* p, int iObs)
{
    setPad4Observable(p, iObs, dep);
}

/*
 * draw vertical lines for the pt range
 * Ex. x-axis axis is reco pt and the gen Pt range is 10<pt<30, then it draws vertical lines at x=10 and x=30.
 * Ex. x-axis axis is gen pt and the reco Pt range is pt>20, then it draws vertical line at x=20.
 */
void energyScaleHist::drawLine4PtRange(TPad* p, int lineColor)
{
    TLine* line = 0;

    p->Update();
    double x1 = p->GetUxmin();
    double x2 = p->GetUxmax();
    double y1 = p->GetUymin();
    double y2 = p->GetUymax();
    if (y2 > 1)  y2 = 1;        // vertical rises to at most y = 1

    if (dep == ENERGYSCALE::kGENPT) {

        // vertical lines for reco pt range
        if (ranges[ENERGYSCALE::kRECOPT][0] > 0 &&
            ranges[ENERGYSCALE::kRECOPT][0] > x1 && ranges[ENERGYSCALE::kRECOPT][0] < x2) {

            line = new TLine(ranges[ENERGYSCALE::kRECOPT][0], y1, ranges[ENERGYSCALE::kRECOPT][0], y2);
            line->SetLineStyle(kDotted);
            line->SetLineColor(lineColor);
            line->SetLineWidth(line->GetLineWidth()*3);
            line->Draw();
        }
        if (ranges[ENERGYSCALE::kRECOPT][1] > 0 &&
            ranges[ENERGYSCALE::kRECOPT][1] > x1 && ranges[ENERGYSCALE::kRECOPT][1] < x2) {

            line = new TLine(ranges[ENERGYSCALE::kRECOPT][1], y1, ranges[ENERGYSCALE::kRECOPT][1], y2);
            line->SetLineStyle(kDotted);
            line->SetLineColor(lineColor);
            line->SetLineWidth(line->GetLineWidth()*3);
            line->Draw();
        }
    }
    else if (dep == ENERGYSCALE::kRECOPT) {

        // vertical lines for gen pt range
        if (ranges[ENERGYSCALE::kGENPT][0] > 0 &&
            ranges[ENERGYSCALE::kGENPT][0] > x1 && ranges[ENERGYSCALE::kGENPT][0] < x2) {

            line = new TLine(ranges[ENERGYSCALE::kGENPT][0], y1, ranges[ENERGYSCALE::kGENPT][0], y2);
            line->SetLineStyle(kDotted);
            line->SetLineColor(lineColor);
            line->SetLineWidth(line->GetLineWidth()*3);
            line->Draw();
        }
        if (ranges[ENERGYSCALE::kGENPT][1] > 0 &&
            ranges[ENERGYSCALE::kGENPT][1] > x1 && ranges[ENERGYSCALE::kGENPT][1] < x2) {

            line = new TLine(ranges[ENERGYSCALE::kGENPT][1], y1, ranges[ENERGYSCALE::kGENPT][1], y2);
            line->SetLineStyle(kDotted);
            line->SetLineColor(lineColor);
            line->SetLineWidth(line->GetLineWidth()*3);
            line->Draw();
        }
    }
}

#endif
