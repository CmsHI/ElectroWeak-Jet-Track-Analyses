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
#include <vector>
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
    kERESEFF,      // energy resolution from sigmaEff
    kERESHM,      // energy resolution from sigmaHM
    kEFF,            // matching efficiency
    kFAKE,            // fake rate
    kN_OBS
};

const std::string OBS_LABELS[kN_OBS] = {"eScale", "eRes", "eScaleArith", "eResArith", "eResEff", "eResHM", "matchEff", "fakeRatio"};

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
    k_nucleon,
    k_sigmaM,
    k_lambda,
    k_sigmaP,
    k_cascadeM,
    k_cascade0,
    kN_FakeCand
};

struct particle
{
    particle() {
        this->name = "";
        this->label = "";
        this->PDG.clear();
        this->PDG.push_back(0);
        this->m.clear();
        this->m.push_back(0);
        this->charge.clear();
        this->charge.push_back(0);
    }
    particle(std::string name, std::string label, std::vector<int> PDG = std::vector<int>({0}),
                                                  std::vector<double> m = std::vector<double>({0}),
                                                  std::vector<int> charge = std::vector<int>({0})) {
        this->name = name;
        this->label = label;
        this->PDG = PDG;
        this->m = m;
        this->charge = charge;
    }

    std::string name;
    std::string label;
    std::vector<int> PDG;
    std::vector<double> m;
    std::vector<int> charge;
};

const particle particles[kN_FakeCand] =
{
   particle("unknown", "other", {0}),
   particle("electron", "e^{#pm}", {11}),
   particle("eneutrino", "#nu_{e}", {12}),
   particle("muon", "#mu^{#pm}", {13}),
   particle("muneutrino", "#nu_{#mu}", {14}),
   particle("tau", "#tau^{#pm}", {16}),
   particle("kaonL", "K^{0}_{L}", {130}),
   particle("pionch", "#pi^{#pm}", {211}),
   particle("kaonS", "K^{0}_{S}", {310}),
   particle("kaonch", "K^{#pm}", {321}),
   particle("neutron", "n", {2112}),
   particle("proton", "p", {2212}),
   particle("nucleon", "p/n", {2112, 2212}),
   particle("sigmaM", "#Sigma^{-}", {3112}),
   particle("lambda", "#Lambda", {3122}),
   particle("sigmaP", "#Sigma^{+}", {3222}),
   particle("cascadeM", "#Xi^{-}", {3312}),
   particle("cascade0", "#Xi^{0}", {3322})
};

const int particlesColor[kN_FakeCand] =
{
   kBlack,      // unknown
   kCyan+1,     // electron
   kCyan,     // eneutrino
   kOrange+3,   // muon
   kOrange+1,   // muneutrino
   kOrange+2,    // tau
   kGreen+3,   // kaonL
   kRed+1,      // pionch
   kOrange-1,   // kaonS
   kBlue+1,     // kaonch
   kYellow+2,  // neutron
   38,   // proton
   38,   // nucleon
   kRed+1,       // sigmaM
   kViolet,      // lambda
   kRed+2,    // sigmaP
   kAzure+1,     // cascadeM
   kAzure+2      // cascade0
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
        hMeanErr = -1;
        hStdDev = -1;
        hStdDevErr = -1;
        sigmaEff = -1;
        sigmaEffErr = -1;
        sigmaHM = -1;
        sigmaHMErr = -1;
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
    void calcSigmaEff() {

        if (!isValid_h)  return;

        int binMax = h->GetMaximumBin();

        double fraction = 0.6827;

        std::vector<int> binRange = getLeftRightBins4IntegralFraction(h, binMax, fraction);
        double fracFromBinEdges = h->Integral(binRange[0], binRange[1]) / h->Integral();

        double x1 = h->GetBinLowEdge(binRange[0]);
        double x2 = h->GetBinLowEdge(binRange[1]+1);
        double width = x2-x1;

        // by definition fracFromBinEdges >= fraction
        // scale the width to remove any residual deviation from 0.6827 due to binned data.
        width *= fraction / fracFromBinEdges;

        sigmaEff = width / 2;
        sigmaEffErr = hStdDevErr;       // for the moment, same error as in histogram stdDev
    };
    void calcSigmaHM() {

        if (!isValid_h)  return;

        int binMax = h->GetMaximumBin();
        double maxContent = h->GetBinContent(binMax);

        double halfMax = maxContent/2;

        int binLeft = getMaximumBin(h, halfMax, 1, binMax);
        int binRight = getMaximumBin(h, halfMax, binMax);

        double x1 = h->GetBinLowEdge(binLeft);
        double x2 = h->GetBinLowEdge(binRight+1);
        double width = x2-x1;

        // For a Gaussian distribution
        // full-width-at-half-maximum = 2.355 * sigma
        sigmaHM = width / 2.355;
        sigmaHMErr = hStdDevErr;       // for the moment, same error as in histogram stdDev
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

        calcSigmaEff();
        calcSigmaHM();
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
    double sigmaEff;  // half-width of the narrowest interval containing 68.27% of the distribution
    double sigmaEffErr;
    double sigmaHM;   // full-width-at-half-maximum (FWHM) of the distribution divided by 2.355
    double sigmaHMErr;
};

class energyScaleHist {
public :
    energyScaleHist(){
        nBinsX = 0;
        h2D = 0;

        for (int i = 0; i < 8; ++i) {
            h1DeScale[i] = 0;
        }
        for (int i = 0; i < 8; ++i) {
            h1D[i] = 0;
        }
        hEscale = 0;
        h2Dcorr = 0;

        isValid_h2D = false;
        isValid_hEscale = false;
        isValid_h2Dcorr = false;

        indexFnc = ENERGYSCALE::kGAUS_95;

        hMatchNum = 0;
        hMatchDenom = 0;
        hMatchEff = 0;
        gMatchEff = 0;
        isValid_hMatchNum = false;
        isValid_hMatchDenom = false;
        isValid_hMatchEff = false;
        isValid_gMatchEff = false;

        hFakeNum = 0;
        hFakeDenom = 0;
        hFakeRatio = 0;
        gFakeRatio = 0;
        hFakeOtherRatio = 0;
        isValid_hFakeNum = false;
        isValid_hFakeDenom = false;
        isValid_hFakeRatio = false;
        isValid_gFakeRatio = false;

        fakeIndices = {
                ENERGYSCALE::FAKECAND::k_kaonS,
                ENERGYSCALE::FAKECAND::k_pionch,
                ENERGYSCALE::FAKECAND::k_kaonL,
                ENERGYSCALE::FAKECAND::k_kaonch,
                ENERGYSCALE::FAKECAND::k_electron,
                ENERGYSCALE::FAKECAND::k_nucleon,
                ENERGYSCALE::FAKECAND::k_lambda,
        };

        nFakeParticles = fakeIndices.size();
        fakeParticles.resize(nFakeParticles);

        hFakeParticle.resize(nFakeParticles);
        hFakeParticleRatio.resize(nFakeParticles);
        isValid_hFakeParticle.resize(nFakeParticles);
        isValid_hFakeParticleRatio.resize(nFakeParticles);

        hFakeParticleGenPt.resize(nFakeParticles);
        hFakeParticleRatioGenPt.resize(nFakeParticles);
        isValid_hFakeParticleGenPt.resize(nFakeParticles);
        isValid_hFakeParticleRatioGenPt.resize(nFakeParticles);

        passedMinFakeFraction.resize(nFakeParticles);
        passedMinFakeFractionGenPt.resize(nFakeParticles);

        for (int i = 0; i < nFakeParticles; ++i) {

            int index = fakeIndices.at(i);
            fakeParticles[i] = ENERGYSCALE::particles[index];

            hFakeParticle[i] = 0;
            hFakeParticleRatio[i] = 0;
            isValid_hFakeParticle[i] = false;
            isValid_hFakeParticleRatio[i] = false;

            hFakeParticleGenPt[i] = 0;
            hFakeParticleRatioGenPt[i] = 0;
            isValid_hFakeParticleGenPt[i] = false;
            isValid_hFakeParticleRatioGenPt[i] = false;

            passedMinFakeFraction[i] = false;
            passedMinFakeFractionGenPt[i] = false;
        }
        hFakeOther = 0;
        hFakeOtherRatio = 0;
        isValid_hFakeOther = false;
        isValid_hFakeOtherRatio = false;

        hFakeOtherGenPt = 0;
        hFakeOtherRatioGenPt = 0;
        isValid_hFakeOtherGenPt = false;
        isValid_hFakeOtherRatioGenPt = false;

        hFakeAllGenPt = 0;
        isValid_hFakeAllGenPt = false;

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

    int getParticleIndex(int pdg);
    int getFakePDG(int iParticle);
    std::string getFakePDGstr(int iParticle);
    static std::string getObjectStr(int iObj);
    static std::string getTObjectStr(int iTObj);
    std::string getObjectName(int iObj, int iTObj = 0);

    void prepareTitle();

    void postLoop();
    void fitRecoGen();
    void calcMatchEff();
    void calcFakeRatio();
    void calcFakeParticleRatio();
    void calcFakeParticleRatioGenPt();
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
     * h1DeScale[4] = energy resolution histogram, sigmaEff of 1D distribution
     * h1DeScale[5] = energy resolution histogram, sigmaHM of 1D distribution
     * h1DeScale[6] = Constant for Gaussian fit
     * h1DeScale[7] = chi2/ndf for Gaussian fit
     */
    TH1D* h1DeScale[8];

    /*
     * h1D[0] = energy scale histogram, mean for a Gaussian fit
     * h1D[1] = energy resolution histogram, StdDev for a Gaussian fit
     * h1D[2] = energy scale histogram, mean from histogram mean of 1D distribution
     * h1D[3] = energy resolution histogram, StdDev from histogram mean of 1D distribution
     * h1D[4] = energy resolution histogram, sigmaEff of 1D distribution
     * h1D[5] = energy resolution histogram, sigmaHM of 1D distribution
     * h1D[6] = matching efficiency
     * h1D[7] = fake rate
     */
    TH1D* h1D[8];

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

    TH1D* hEscale;      // energy scale distribution for all the bins along x-axis
    TH2D* h2Dcorr;      // reco pt vs. gen pt correlation histogram.

    bool isValid_h2D;
    bool isValid_hEscale;
    bool isValid_h2Dcorr;

    // objects for efficiency
    TH1D* hMatchNum;
    TH1D* hMatchDenom;
    TH1D* hMatchEff;
    TGraphAsymmErrors* gMatchEff;

    bool isValid_hMatchNum;
    bool isValid_hMatchDenom;
    bool isValid_hMatchEff;
    bool isValid_gMatchEff;

    // objects for fake rate
    TH1D* hFakeNum;
    TH1D* hFakeDenom;
    TH1D* hFakeRatio;
    TGraphAsymmErrors* gFakeRatio;

    bool isValid_hFakeNum;
    bool isValid_hFakeDenom;
    bool isValid_hFakeRatio;
    bool isValid_gFakeRatio;

    // particles to be used for fake composition
    std::vector<int> fakeIndices;
    std::vector<ENERGYSCALE::particle> fakeParticles;
    int nFakeParticles;
    // objects for each particle in fake composition
    /*
     * If the GEN-level particle matches the RECO-level fake, then fill hFakeParticle.
     */
    std::vector<TH1D*> hFakeParticle;
    TH1D* hFakeOther;
    /*
     * hRatioFakeParticle = hFakeParticle / hNumFake
     * hRatioFakeParticle is contribution of this particle to the total fake rate.
     */
    std::vector<TH1D*> hFakeParticleRatio;
    TH1D* hFakeOtherRatio;

    std::vector<bool> isValid_hFakeParticle;
    std::vector<bool> isValid_hFakeParticleRatio;
    bool isValid_hFakeOther;
    bool isValid_hFakeOtherRatio;

    // objects for particle composition of fake rate as function of GEN-level particle pt
    std::vector<TH1D*> hFakeParticleGenPt;
    TH1D* hFakeOtherGenPt;
    /*
     * hRatioFakeParticleGenPt = hFakeParticleGenPt / hAllFakeParticlesGenPt
     * NOTE : x-axis of these histograms is the pt of the GEN-level particles that fake the RECO-level particle,
     * not necessarily the same as "dep".
     */
    std::vector<TH1D*> hFakeParticleRatioGenPt;
    TH1D* hFakeOtherRatioGenPt;
    TH1D* hFakeAllGenPt;

    std::vector<bool> isValid_hFakeParticleGenPt;
    std::vector<bool> isValid_hFakeParticleRatioGenPt;
    bool isValid_hFakeOtherGenPt;
    bool isValid_hFakeOtherRatioGenPt;
    bool isValid_hFakeAllGenPt;

    /*
     * Ex. minFakeFraction = 0.05 and there is particle a which makes up less 5% of the fakes for each bin along x-axis
     * That particle will not be plotted separately, but will be shown under "other" particle (hFakeRatioOther)
     */
    double minFakeFraction;
    std::vector<bool> passedMinFakeFraction;
    std::vector<bool> passedMinFakeFractionGenPt;

    enum OBJ{
        kMatchNum,
        kMatchDenom,
        kMatchEff,
        kFakeNum,
        kFakeDenom,
        kFakeRatio,
        kFakeParticle,
        kFakeParticleRatio,
        kFakeOther,
        kFakeOtherRatio,
        kFakeParticleGenPt,
        kFakeParticleRatioGenPt,
        kFakeOtherGenPt,
        kFakeOtherRatioGenPt,
        kFakeAllGenPt,
        kN_OBJ
    };

    enum TOBJ{
        kTH1D,
        kTH2D,
        kTGraph,
        kN_TOBJ
    };

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
    if (isValid_hMatchNum && insideRange(genEta, genPt, recoPt, cent))
        hMatchNum->Fill(x, w);
}

void energyScaleHist::FillHDenom(double x, double w, float genEta, float genPt, int cent)
{
    if (isValid_hMatchDenom && insideRange(genEta, genPt, -1, cent))
        hMatchDenom->Fill(x, w);
}
/*
 * fill numerator for fake rate
 * no GEN-level selection is applied
 */
void energyScaleHist::FillHNumFake(double x, double w, float eta, float recoPt, int cent)
{
    if (isValid_hFakeNum && insideRange(eta, -1, recoPt, cent)) {
        hFakeNum->Fill(x, w);
    }
}

/*
 * fill denominator for fake rate
 * no GEN-level selection is applied
 */
void energyScaleHist::FillHDenomFake(double x, double w, float eta, float recoPt, int cent)
{
    if (isValid_hFakeDenom && insideRange(eta, -1, recoPt, cent))
        hFakeDenom->Fill(x, w);
}

void energyScaleHist::FillHFakeParticle(double x, int pdg, double w, float eta, float genPt, float recoPt, int cent)
{
    if (insideRange(eta, genPt, recoPt, cent)) {

        int iParticle = getParticleIndex(pdg);

        if (iParticle > -1 && isValid_hFakeParticle[iParticle]) {
            hFakeParticle[iParticle]->Fill(x, w);
        }
        else if (isValid_hFakeOther){
            hFakeOther->Fill(x, w);
        }
    }
}

void energyScaleHist::FillHFakeParticleGenPt(double genPt, int pdg, double w, float eta, float recoPt, int cent)
{
    if (insideRange(eta, -1, recoPt, cent)) {

        if (isValid_hFakeAllGenPt) {
            hFakeAllGenPt->Fill(genPt, w);
        }

        int iParticle = getParticleIndex(pdg);

        if (iParticle > -1 && isValid_hFakeParticleGenPt[iParticle]) {
            hFakeParticleGenPt[iParticle]->Fill(genPt, w);
        }
        else if (isValid_hFakeOtherGenPt){
            hFakeOtherGenPt->Fill(genPt, w);
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

    isValid_hMatchNum = (hMatchNum != 0 && !hMatchNum->IsZombie());
    isValid_hMatchDenom = (hMatchDenom != 0 && !hMatchDenom->IsZombie());
    isValid_hMatchEff = (hMatchEff != 0 && !hMatchEff->IsZombie());
    isValid_gMatchEff = (gMatchEff != 0 && !gMatchEff->IsZombie());

    isValid_hFakeNum = (hFakeNum != 0 && !hFakeNum->IsZombie());
    isValid_hFakeDenom = (hFakeDenom != 0 && !hFakeDenom->IsZombie());
    isValid_hFakeRatio = (hFakeRatio != 0 && !hFakeRatio->IsZombie());
    isValid_gFakeRatio = (gFakeRatio != 0 && !gFakeRatio->IsZombie());

    for (int i = 0; i < nFakeParticles; ++i) {
        isValid_hFakeParticle[i] = (hFakeParticle[i] != 0 && !hFakeParticle[i]->IsZombie());
        isValid_hFakeParticleRatio[i] = (hFakeParticleRatio[i] != 0 && !hFakeParticleRatio[i]->IsZombie());

        isValid_hFakeParticleGenPt[i] = (hFakeParticleGenPt[i] != 0 && !hFakeParticleGenPt[i]->IsZombie());
        isValid_hFakeParticleRatioGenPt[i] = (hFakeParticleRatioGenPt[i] != 0 && !hFakeParticleRatioGenPt[i]->IsZombie());
    }
    isValid_hFakeOther = (hFakeOther != 0 && !hFakeOther->IsZombie());
    isValid_hFakeOtherRatio = (hFakeOtherRatio != 0 && !hFakeOtherRatio->IsZombie());

    isValid_hFakeOtherGenPt = (hFakeOtherGenPt != 0 && !hFakeOtherGenPt->IsZombie());
    isValid_hFakeOtherRatioGenPt = (hFakeOtherRatioGenPt != 0 && !hFakeOtherRatioGenPt->IsZombie());
    isValid_hFakeAllGenPt = (hFakeAllGenPt != 0 && !hFakeAllGenPt->IsZombie());
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

        if (!esa[i-1][indexFnc].isValid_h) continue;

        if (esa[i-1][indexFnc].isValid_f1) {
            h1DeScale[ENERGYSCALE::kESCALE]->SetBinContent(i, esa[i-1][indexFnc].f1->GetParameter(1));
            h1DeScale[ENERGYSCALE::kESCALE]->SetBinError(i, esa[i-1][indexFnc].f1->GetParError(1));

            h1DeScale[ENERGYSCALE::kERES]->SetBinContent(i, esa[i-1][indexFnc].f1->GetParameter(2));
            h1DeScale[ENERGYSCALE::kERES]->SetBinError(i, esa[i-1][indexFnc].f1->GetParError(2));

            h1DeScale[6]->SetBinContent(i, esa[i-1][indexFnc].f1->GetParameter(0));
            h1DeScale[6]->SetBinError(i, esa[i-1][indexFnc].f1->GetParError(0));

            h1DeScale[7]->SetBinContent(i, esa[i-1][indexFnc].f1->GetChisquare()/esa[i-1][indexFnc].f1->GetNDF());
            h1DeScale[7]->SetBinError(i, 0);
        }

        // arithmetic mean and std dev of energy scale distributions
        double y = esa[i-1][indexFnc].hMean;
        double yErr = esa[i-1][indexFnc].hMeanErr;
        h1DeScale[ENERGYSCALE::kESCALEARITH]->SetBinContent(i, y);
        h1DeScale[ENERGYSCALE::kESCALEARITH]->SetBinError(i, yErr);

        y= esa[i-1][indexFnc].hStdDev;
        yErr = esa[i-1][indexFnc].hStdDevErr;
        h1DeScale[ENERGYSCALE::kERESARITH]->SetBinContent(i, y);
        h1DeScale[ENERGYSCALE::kERESARITH]->SetBinError(i, yErr);

        y = esa[i-1][indexFnc].sigmaEff;
        yErr = esa[i-1][indexFnc].sigmaEffErr;
        h1DeScale[ENERGYSCALE::kERESEFF]->SetBinContent(i, y);
        h1DeScale[ENERGYSCALE::kERESEFF]->SetBinError(i, yErr);

        y = esa[i-1][indexFnc].sigmaHM;
        yErr = esa[i-1][indexFnc].sigmaHMErr;
        h1DeScale[ENERGYSCALE::kERESHM]->SetBinContent(i, y);
        h1DeScale[ENERGYSCALE::kERESHM]->SetBinError(i, yErr);
    }
}

/*
 * return the first particle that contains the given PDG
 */
int energyScaleHist::getParticleIndex(int pdg)
{
    for (int i = 0; i < nFakeParticles; ++i) {

        std::vector<int> PDGs = fakeParticles[i].PDG;
        std::vector<int>::iterator it = std::find(PDGs.begin(), PDGs.end(), pdg);

        if (it != PDGs.end())  return i;
    }

    return -1;
}

int energyScaleHist::getFakePDG(int iParticle)
{
    if (iParticle < nFakeParticles)  return fakeParticles[iParticle].PDG[0];
    else                   return 0;
}

std::string energyScaleHist::getFakePDGstr(int iParticle)
{
    std::string res = "";
    if (iParticle < nFakeParticles)  {

        int nPDGs = fakeParticles[iParticle].PDG.size();
        if (nPDGs >= 1) {
            res = Form("%d", fakeParticles[iParticle].PDG[0]);
            for (int i = 1; i < nPDGs; ++i) {
                res.append(Form("_%d", fakeParticles[iParticle].PDG[i]));
            }
        }
    }

    return res;
}

std::string energyScaleHist::getObjectStr(int iObj)
{
    switch (iObj) {
    case energyScaleHist::OBJ::kMatchNum :
        return "MatchNum";
    case energyScaleHist::OBJ::kMatchDenom :
        return "MatchDenom";
    case energyScaleHist::OBJ::kMatchEff :
        return "MatchEff";
    case energyScaleHist::OBJ::kFakeNum :
        return "FakeNum";
    case energyScaleHist::OBJ::kFakeDenom :
        return "FakeDenom";
    case energyScaleHist::OBJ::kFakeRatio :
        return "FakeRatio";
    case energyScaleHist::OBJ::kFakeParticle :
        return "FakePDG";
    case energyScaleHist::OBJ::kFakeParticleRatio :
        return "FakeRatioPDG";
    case energyScaleHist::OBJ::kFakeOther :
        return "FakeOther";
    case energyScaleHist::OBJ::kFakeOtherRatio :
        return "FakeOtherRatio";
    case energyScaleHist::OBJ::kFakeParticleGenPt :
        return "FakeGenPtPDG";
    case energyScaleHist::OBJ::kFakeParticleRatioGenPt :
        return "FakeRatioGenPtPDG";
    case energyScaleHist::OBJ::kFakeOtherGenPt :
        return "FakeOtherGenPt";
    case energyScaleHist::OBJ::kFakeOtherRatioGenPt :
        return "FakeOtherRatioGenPt";
    case energyScaleHist::OBJ::kFakeAllGenPt :
        return "FakeAllGenPt";
    default :
        return "";
    }

    return "";
}

std::string energyScaleHist::getTObjectStr(int iTObj)
{
    switch (iTObj) {
    case energyScaleHist::TOBJ::kTH1D :
        return "h";
    case energyScaleHist::TOBJ::kTH2D :
        return "h2D_";
    case energyScaleHist::TOBJ::kTGraph :
        return "g";
    default :
        return "";
    }

    return "";
}

std::string energyScaleHist::getObjectName(int iObj, int iTObj)
{
    std::string objStr =  energyScaleHist::getObjectStr(iObj);
    std::string tObjStr = energyScaleHist::getTObjectStr(iTObj);

    return Form("%s%s_%s", tObjStr.c_str(), objStr.c_str(), name.c_str());
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
    if(isValid_hMatchNum) {
        hMatchNum->SetTitle(title.c_str());
    }
    if(isValid_hMatchDenom) {
        hMatchDenom->SetTitle(title.c_str());
    }
    if(isValid_gMatchEff) {
        gMatchEff->SetTitle(title.c_str());
    }
    if(isValid_hMatchEff) {
        hMatchEff->SetTitle(title.c_str());
    }
}

void energyScaleHist::postLoop()
{
    if (isValid_hEscale) {
        hEscale->SetMarkerStyle(kFullCircle);
    }

    if (isValid_h2D) {
        TObjArray aSlices;
        h2D->FitSlicesY(0,0,-1,0,"Q LL M N", &aSlices);

        // energy scale
        int iObs = ENERGYSCALE::kESCALE;
        std::string label = ENERGYSCALE::OBS_LABELS[iObs];
        h1DeScale[iObs] = (TH1D*)aSlices.At(1)->Clone(Form("h1D_%s_%s", label.c_str(), name.c_str()));
        h1DeScale[iObs]->SetTitle(title.c_str());
        h1DeScale[iObs]->SetXTitle(titleX.c_str());
        setTH1_energyScale(h1DeScale[iObs], titleOffsetX, titleOffsetY);
        if (yMax[iObs] > yMin[iObs])
            h1DeScale[iObs]->SetAxisRange(yMin[iObs], yMax[iObs], "Y");

        // width of energy scale
        iObs = ENERGYSCALE::kERES;
        label = ENERGYSCALE::OBS_LABELS[iObs];
        h1DeScale[iObs] = (TH1D*)aSlices.At(2)->Clone(Form("h1D_%s_%s", label.c_str(), name.c_str()));
        h1DeScale[iObs]->SetTitle(title.c_str());
        h1DeScale[iObs]->SetXTitle(titleX.c_str());
        setTH1_energyWidth(h1DeScale[iObs], titleOffsetX, titleOffsetY);
        if (yMax[iObs] > yMin[iObs])
            h1DeScale[iObs]->SetAxisRange(yMin[iObs], yMax[iObs], "Y");

        // energy scale with arithmetic mean
        iObs = ENERGYSCALE::kESCALEARITH;
        label = ENERGYSCALE::OBS_LABELS[iObs];
        h1DeScale[iObs] = (TH1D*)h1DeScale[ENERGYSCALE::kESCALE]->Clone(Form("h1D_%s_%s", label.c_str(), name.c_str()));
        h1DeScale[iObs]->SetTitle(title.c_str());
        h1DeScale[iObs]->SetXTitle(titleX.c_str());
        setTH1_energyScale(h1DeScale[iObs], titleOffsetX, titleOffsetY);
        h1DeScale[iObs]->SetYTitle(Form("%s (Arith)", h1DeScale[iObs]->GetYaxis()->GetTitle()));
        if (yMax[ENERGYSCALE::kESCALE] > yMin[ENERGYSCALE::kESCALE])
            h1DeScale[iObs]->SetAxisRange(yMin[ENERGYSCALE::kESCALE], yMax[ENERGYSCALE::kESCALE], "Y");

        // width of energy scale with arithmetic std dev
        iObs = ENERGYSCALE::kERESARITH;
        label = ENERGYSCALE::OBS_LABELS[iObs];
        h1DeScale[iObs] = (TH1D*)h1DeScale[ENERGYSCALE::kERES]->Clone(Form("h1D_%s_%s", label.c_str(), name.c_str()));
        h1DeScale[iObs]->SetTitle(title.c_str());
        h1DeScale[iObs]->SetXTitle(titleX.c_str());
        setTH1_energyWidth(h1DeScale[iObs], titleOffsetX, titleOffsetY);
        h1DeScale[iObs]->SetYTitle(Form("%s (Arith)", h1DeScale[iObs]->GetYaxis()->GetTitle()));
        if (yMax[ENERGYSCALE::kERES] > yMin[ENERGYSCALE::kERES])
            h1DeScale[iObs]->SetAxisRange(yMin[ENERGYSCALE::kERES], yMax[ENERGYSCALE::kERES], "Y");

        // width of energy scale with sigmaEff
        iObs = ENERGYSCALE::kERESEFF;
        label = ENERGYSCALE::OBS_LABELS[iObs];
        h1DeScale[iObs] = (TH1D*)h1DeScale[ENERGYSCALE::kERES]->Clone(Form("h1D_%s_%s", label.c_str(), name.c_str()));
        h1DeScale[iObs]->SetTitle(title.c_str());
        h1DeScale[iObs]->SetXTitle(titleX.c_str());
        setTH1_energyWidth(h1DeScale[iObs], titleOffsetX, titleOffsetY);
        h1DeScale[iObs]->SetYTitle(Form("%s (Effective)", h1DeScale[iObs]->GetYaxis()->GetTitle()));
        if (yMax[ENERGYSCALE::kERES] > yMin[ENERGYSCALE::kERES])
            h1DeScale[iObs]->SetAxisRange(yMin[ENERGYSCALE::kERES], yMax[ENERGYSCALE::kERES], "Y");

        // width of energy scale with sigmaHM
        iObs = ENERGYSCALE::kERESHM;
        label = ENERGYSCALE::OBS_LABELS[iObs];
        h1DeScale[iObs] = (TH1D*)h1DeScale[ENERGYSCALE::kERES]->Clone(Form("h1D_%s_%s", label.c_str(), name.c_str()));
        h1DeScale[iObs]->SetTitle(title.c_str());
        h1DeScale[iObs]->SetXTitle(titleX.c_str());
        setTH1_energyWidth(h1DeScale[iObs], titleOffsetX, titleOffsetY);
        h1DeScale[iObs]->SetYTitle(Form("%s (FWHM / 2.35)", h1DeScale[iObs]->GetYaxis()->GetTitle()));
        if (yMax[ENERGYSCALE::kERES] > yMin[ENERGYSCALE::kERES])
            h1DeScale[iObs]->SetAxisRange(yMin[ENERGYSCALE::kERES], yMax[ENERGYSCALE::kERES], "Y");

        // Constant for Gaussian fit
        h1DeScale[6] = (TH1D*)aSlices.At(0)->Clone(Form("h1D_gausConst_%s", name.c_str()));
        h1DeScale[6]->SetTitle(title.c_str());
        h1DeScale[6]->SetXTitle(titleX.c_str());
        h1DeScale[6]->SetYTitle("Constant for Gaussian fit");
        h1DeScale[6]->SetStats(false);
        h1DeScale[6]->SetMarkerStyle(kFullCircle);

        // chi2/ndf for Gaussian fit
        h1DeScale[7] = (TH1D*)aSlices.At(3)->Clone(Form("h1D_gausChi2ndf_%s", name.c_str()));
        h1DeScale[7]->SetTitle(title.c_str());
        h1DeScale[7]->SetXTitle(titleX.c_str());
        h1DeScale[7]->SetYTitle("chi2/ndf for Gaussian fit");
        h1DeScale[7]->SetStats(false);
        h1DeScale[7]->SetMarkerStyle(kFullCircle);

        updateH1DsliceY();

        fitRecoGen();
        // up to this point bins of h1DeScale[0], h1DeScale[1], h1DeScale[6], h1DeScale[7] are set by the initial fit from TH2::FitSlicesY
        updateH1DeScale();
    }

    calcMatchEff();
    calcFakeRatio();
    calcFakeParticleRatio();
    calcFakeParticleRatioGenPt();

    // Final histograms point to observables
    if (isValid_h2D) {
        h1D[ENERGYSCALE::kESCALE] = h1DeScale[ENERGYSCALE::kESCALE];
        h1D[ENERGYSCALE::kERES] = h1DeScale[ENERGYSCALE::kERES];
        h1D[ENERGYSCALE::kESCALEARITH] = h1DeScale[ENERGYSCALE::kESCALEARITH];
        h1D[ENERGYSCALE::kERESARITH] = h1DeScale[ENERGYSCALE::kERESARITH];
        h1D[ENERGYSCALE::kERESEFF] = h1DeScale[ENERGYSCALE::kERESEFF];
        h1D[ENERGYSCALE::kERESHM] = h1DeScale[ENERGYSCALE::kERESHM];
    }

    if (isValid_hMatchEff)  h1D[ENERGYSCALE::kEFF] = hMatchEff;
    if (isValid_hFakeRatio)  h1D[ENERGYSCALE::kFAKE] = hFakeRatio;
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

    for (int i=1; i<=nBinsX; ++i) {

        hTmp = h1DsliceY[i-1];

        double p0 = h1DeScale[6]->GetBinContent(i);   // constant
        double p0Err = h1DeScale[6]->GetBinError(i);

        double p1 = h1DeScale[ENERGYSCALE::kESCALE]->GetBinContent(i);   // mean
        double p1Err = h1DeScale[ENERGYSCALE::kESCALE]->GetBinError(i);

        double p2 = h1DeScale[ENERGYSCALE::kERES]->GetBinContent(i);   // StdDev
        double p2Err = h1DeScale[ENERGYSCALE::kERES]->GetBinError(i);

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
                //        double chi2ndf = h1DeScale[7]->GetBinContent(i);
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
    }
}

void energyScaleHist::calcMatchEff()
{
    if (!isValid_hMatchNum || !isValid_hMatchDenom) return;

    std::string tmpName;

    hMatchNum->SetTitle(title.c_str());
    hMatchNum->SetXTitle(titleX.c_str());
    setTH1_efficiency(hMatchNum, titleOffsetX, titleOffsetY);

    hMatchDenom->SetTitle(title.c_str());
    hMatchDenom->SetXTitle(titleX.c_str());
    setTH1_efficiency(hMatchDenom, titleOffsetX, titleOffsetY);

    if (isValid_gMatchEff) {
        gMatchEff->Delete();
        isValid_gMatchEff = false;
    }

    gMatchEff = new TGraphAsymmErrors();
    tmpName = getObjectName(energyScaleHist::OBJ::kMatchEff, energyScaleHist::TOBJ::kTGraph);
    gMatchEff->SetName(tmpName.c_str());
    gMatchEff->BayesDivide(hMatchNum, hMatchDenom);
    gMatchEff->SetTitle(title.c_str());
    gMatchEff->GetXaxis()->SetTitle(titleX.c_str());
    gMatchEff->GetYaxis()->SetTitle("Efficiency");
    gMatchEff->SetMarkerStyle(kFullCircle);

    isValid_gMatchEff = true;

    if (isValid_hMatchEff) {
        hMatchEff->Delete();
        isValid_hMatchEff = false;
    }

    tmpName = getObjectName(energyScaleHist::OBJ::kMatchEff, energyScaleHist::TOBJ::kTH1D);
    hMatchEff = (TH1D*)hMatchNum->Clone(tmpName.c_str());
    fillTH1fromTGraph(hMatchEff, gMatchEff);
    setTH1_efficiency(hMatchDenom, titleOffsetX, titleOffsetY);
    hMatchEff->SetTitle(title.c_str());
    hMatchEff->SetXTitle(titleX.c_str());
    hMatchEff->SetYTitle("Efficiency");
    hMatchEff->SetMinimum(0);
    hMatchEff->SetMaximum(1.2);

    isValid_hMatchEff = true;
}

void energyScaleHist::calcFakeRatio()
{
    if (!isValid_hFakeNum || !isValid_hFakeDenom) return;

    std::string nameTmp;

    hFakeNum->SetTitle(title.c_str());
    hFakeNum->SetXTitle(titleX.c_str());
    setTH1_efficiency(hFakeNum, titleOffsetX, titleOffsetY);

    hFakeDenom->SetTitle(title.c_str());
    hFakeDenom->SetXTitle(titleX.c_str());
    setTH1_efficiency(hFakeDenom, titleOffsetX, titleOffsetY);

    if (isValid_gFakeRatio) {
        gFakeRatio->Delete();
        isValid_gFakeRatio = false;
    }

    gFakeRatio = new TGraphAsymmErrors();
    nameTmp = getObjectName(energyScaleHist::OBJ::kFakeRatio, energyScaleHist::TOBJ::kTGraph);
    gFakeRatio->SetName(nameTmp.c_str());
    gFakeRatio->BayesDivide(hFakeNum, hFakeDenom);
    gFakeRatio->SetTitle(title.c_str());
    gFakeRatio->GetXaxis()->SetTitle(titleX.c_str());
    gFakeRatio->GetYaxis()->SetTitle("Fake Rate");
    gFakeRatio->SetMarkerStyle(kFullCircle);

    isValid_gFakeRatio = true;

    if (isValid_hFakeRatio) {
        hFakeRatio->Delete();
        isValid_hFakeRatio = false;
    }

    nameTmp = getObjectName(energyScaleHist::OBJ::kFakeRatio, energyScaleHist::TOBJ::kTH1D);
    hFakeRatio = (TH1D*)hFakeNum->Clone(nameTmp.c_str());
    fillTH1fromTGraph(hFakeRatio, gFakeRatio);
    setTH1_efficiency(hFakeDenom, titleOffsetX, titleOffsetY);
    hFakeRatio->SetTitle(title.c_str());
    hFakeRatio->SetXTitle(titleX.c_str());
    hFakeRatio->SetYTitle("Fake Rate");
    hFakeRatio->SetMinimum(0);
    hFakeRatio->SetMaximum(1.2);

    isValid_hFakeRatio = true;
}

void energyScaleHist::calcFakeParticleRatio()
{
    // write hFakeOther now, before its content is modified.
    if (isValid_hFakeOther) {
        hFakeOther->Write("",TObject::kOverwrite);
    }

    // Check if hFakeParticle passes the minimum fake fraction.
    // If not, then transfer its content into hFakeOther
    TH1D* hTmp = 0;
    for (int i = 0; i < nFakeParticles; ++i) {
        if (!isValid_hFakeParticle[i])  continue;

        if (!isValid_hFakeNum)  continue;

        std::string tmpHistName = Form("%s_Tmp", hFakeParticle[i]->GetName());
        hTmp = (TH1D*)hFakeParticle[i]->Clone(tmpHistName.c_str());
        hTmp->Divide(hFakeNum);

        double maxContent = hTmp->GetBinContent(hTmp->GetMaximumBin());
        passedMinFakeFraction[i] = (maxContent >= minFakeFraction);

        if (isValid_hFakeOther && !passedMinFakeFraction[i]) {
            hFakeOther->Add(hFakeParticle[i]);
        }

        if (hTmp != 0)  hTmp->Delete();
    }

    if (isValid_hFakeOther) {

        std::string tmpHistName = getObjectName(energyScaleHist::OBJ::kFakeOtherRatio, energyScaleHist::TOBJ::kTH1D);
        hFakeOtherRatio = (TH1D*)hFakeOther->Clone(tmpHistName.c_str());
        if (isValid_hFakeNum) {
            hFakeOtherRatio->Divide(hFakeNum);
        }
        setTH1_efficiency(hFakeOtherRatio, titleOffsetX, titleOffsetY);
        hFakeOtherRatio->SetTitle(title.c_str());
        hFakeOtherRatio->SetXTitle(titleX.c_str());
        hFakeOtherRatio->SetYTitle("Contribution to Fakes, Other");
        hFakeOtherRatio->SetMinimum(0);
        hFakeOtherRatio->SetMaximum(1.5);

        isValid_hFakeOtherRatio = true;
    }

    for (int i = 0; i < nFakeParticles; ++i) {
        if (!isValid_hFakeParticle[i])  continue;

        hFakeParticle[i]->SetTitle(title.c_str());
        hFakeParticle[i]->SetXTitle(titleX.c_str());
        setTH1_efficiency(hFakeParticle[i], titleOffsetX, titleOffsetY);

        if (isValid_hFakeParticleRatio[i]) {
            hFakeParticleRatio[i]->Delete();
            isValid_hFakeParticleRatio[i] = false;
        }

        std::string label = fakeParticles[i].label;
        std::string tmpHistName = replaceAll(hFakeParticle[i]->GetName(), getObjectStr(energyScaleHist::OBJ::kFakeParticle).c_str(),
                                                                          getObjectStr(energyScaleHist::OBJ::kFakeParticleRatio).c_str());
        hFakeParticleRatio[i] = (TH1D*)hFakeParticle[i]->Clone(tmpHistName.c_str());
        if (isValid_hFakeNum) {
            hFakeParticleRatio[i]->Divide(hFakeNum);
        }
        setTH1_efficiency(hFakeParticleRatio[i], titleOffsetX, titleOffsetY);
        hFakeParticleRatio[i]->SetTitle(title.c_str());
        hFakeParticleRatio[i]->SetXTitle(titleX.c_str());
        hFakeParticleRatio[i]->SetYTitle(Form("Contribution to Fakes, %s", label.c_str()));
        hFakeParticleRatio[i]->SetMinimum(0);
        hFakeParticleRatio[i]->SetMaximum(1.5);

        isValid_hFakeParticleRatio[i] = true;
    }

    // check if bin contents sum up to 1
    if (isValid_hFakeOtherRatio) {
        for (int iBin = 1; iBin<=hFakeOtherRatio->GetNbinsX(); ++iBin) {
            double content = 0;

            if (isValid_hFakeOtherRatio) {
                content += hFakeOtherRatio->GetBinContent(iBin);
            }

            for (int i = 0; i < nFakeParticles; ++i) {
                if (isValid_hFakeParticleRatio[i] && passedMinFakeFraction[i]) {
                    content += hFakeParticleRatio[i]->GetBinContent(iBin);
                }
            }

            if ((content < 0.99 || content > 1.01) && content > 0.00001) {
                std::cout << "WARNING : bin contents do not sum up to 1 : " << name.c_str() << std::endl;
                std::cout << "calcFakeParticleRatio()" << std::endl;
                std::cout << Form("bin, content = %d, %f", iBin, content) << std::endl;
            }
        }
    }
}

void energyScaleHist::calcFakeParticleRatioGenPt()
{
    // write hFakeOtherGenPt now, before its content is modified.
    if (isValid_hFakeOtherGenPt) {
        hFakeOtherGenPt->Write("",TObject::kOverwrite);
    }

    // Check if hFakeParticleGenPt passes the minimum fake fraction.
    // If not, then transfer its content into hFakeOtherGenPt
    TH1D* hTmp = 0;
    for (int i = 0; i < nFakeParticles; ++i) {
        if (!isValid_hFakeParticleGenPt[i])  continue;

        if (!isValid_hFakeAllGenPt)  continue;

        std::string tmpHistName = Form("%s_Tmp", hFakeParticleGenPt[i]->GetName());
        hTmp = (TH1D*)hFakeParticleGenPt[i]->Clone(tmpHistName.c_str());
        hTmp->Divide(hFakeAllGenPt);

        double maxContent = hTmp->GetBinContent(hTmp->GetMaximumBin());
        passedMinFakeFractionGenPt[i] = (maxContent >= minFakeFraction);

        if (isValid_hFakeOtherGenPt && !passedMinFakeFractionGenPt[i]) {
            hFakeOtherGenPt->Add(hFakeParticleGenPt[i]);
        }

        if (hTmp != 0)  hTmp->Delete();
    }

    if (isValid_hFakeOtherGenPt) {

        std::string tmpHistName = getObjectName(energyScaleHist::OBJ::kFakeOtherRatioGenPt, energyScaleHist::TOBJ::kTH1D);
        hFakeOtherRatioGenPt = (TH1D*)hFakeOtherGenPt->Clone(tmpHistName.c_str());
        if (isValid_hFakeAllGenPt) {
            hFakeOtherRatioGenPt->Divide(hFakeAllGenPt);
        }
        setTH1_efficiency(hFakeOtherRatioGenPt, titleOffsetX, titleOffsetY);
        hFakeOtherRatioGenPt->SetTitle(title.c_str());
        hFakeOtherRatioGenPt->SetXTitle(titleX.c_str());
        hFakeOtherRatioGenPt->SetYTitle("Contribution to Fakes, Other");
        hFakeOtherRatioGenPt->SetMinimum(0);
        hFakeOtherRatioGenPt->SetMaximum(1.5);

        isValid_hFakeOtherRatioGenPt = true;
    }

    for (int i = 0; i < nFakeParticles; ++i) {
        if (!isValid_hFakeParticleGenPt[i])  continue;

        hFakeParticleGenPt[i]->SetTitle(title.c_str());
        hFakeParticleGenPt[i]->SetXTitle("Gen p_{T} (GeV/c)");
        setTH1_efficiency(hFakeParticleGenPt[i], titleOffsetX, titleOffsetY);

        if (isValid_hFakeParticleRatioGenPt[i]) {
            hFakeParticleRatioGenPt[i]->Delete();
            isValid_hFakeParticleRatioGenPt[i] = false;
        }

        std::string label = fakeParticles[i].label;
        std::string tmpHistName = replaceAll(hFakeParticleGenPt[i]->GetName(), getObjectStr(energyScaleHist::OBJ::kFakeParticleGenPt).c_str(),
                                                                               getObjectStr(energyScaleHist::OBJ::kFakeParticleRatioGenPt).c_str());
        hFakeParticleRatioGenPt[i] = (TH1D*)hFakeParticleGenPt[i]->Clone(tmpHistName.c_str());
        if (isValid_hFakeAllGenPt) {
            hFakeParticleRatioGenPt[i]->Divide(hFakeAllGenPt);
        }
        setTH1_efficiency(hFakeParticleRatioGenPt[i], titleOffsetX, titleOffsetY);
        hFakeParticleRatioGenPt[i]->SetTitle(title.c_str());
        hFakeParticleRatioGenPt[i]->SetXTitle("Gen p_{T} (GeV/c)");
        hFakeParticleRatioGenPt[i]->SetYTitle(Form("Contribution to Fakes, %s", label.c_str()));
        hFakeParticleRatioGenPt[i]->SetMinimum(0);
        hFakeParticleRatioGenPt[i]->SetMaximum(1.5);

        isValid_hFakeParticleRatioGenPt[i] = true;
    }

    // check if bin contents sum up to 1
    if (isValid_hFakeOtherRatioGenPt) {
        for (int iBin = 1; iBin<=hFakeOtherRatioGenPt->GetNbinsX(); ++iBin) {
            double content = 0;

            if (isValid_hFakeOtherRatioGenPt) {
                content += hFakeOtherRatioGenPt->GetBinContent(iBin);
            }

            for (int i = 0; i < nFakeParticles; ++i) {
                if (isValid_hFakeParticleRatioGenPt[i] && passedMinFakeFractionGenPt[i]) {
                    content += hFakeParticleRatioGenPt[i]->GetBinContent(iBin);
                }
            }

            if ((content < 0.99 || content > 1.01) && content > 0.00001) {
                std::cout << "WARNING : bin contents do not sum up to 1 : " << name.c_str() << std::endl;
                std::cout << "calcFakeParticleRatioGenPt()" << std::endl;
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
        hEscale->Write();
    }
    if (isValid_h2Dcorr) {
        h2Dcorr->Write();
    }

    std::string canvasName = "";
    // extract information from "c"
    int windowWidth = c->GetWw();
    int windowHeight = c->GetWh();
    double leftMargin = c->GetLeftMargin();
    double rightMargin = c->GetRightMargin();
    double bottomMargin = c->GetBottomMargin();
    double topMargin = c->GetTopMargin();
    float markerSize = (float)windowWidth/600;

    TLine* line = 0;
    TLegend* leg = 0;
    TLatex* latex = new TLatex();

    double legHeight = -1;
    double legWidth = -1;

    if (isValid_h2D) {
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

        // energy scale
        int iObs = ENERGYSCALE::kESCALE;
        std::string label = ENERGYSCALE::OBS_LABELS[iObs];
        canvasName = Form("cnv_%s_%s", label.c_str(), name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        h1DeScale[iObs]->SetMarkerSize(markerSize);
        h1DeScale[iObs]->Draw("e");
        h1DeScale[iObs]->Write("",TObject::kOverwrite);
        setPad4Observable((TPad*) c, iObs);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.

        // width of energy scale
        iObs = ENERGYSCALE::kERES;
        label = ENERGYSCALE::OBS_LABELS[iObs];
        canvasName = Form("cnv_%s_%s", label.c_str(), name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        h1DeScale[iObs]->SetMarkerSize(markerSize);
        h1DeScale[iObs]->Draw("e");
        h1DeScale[iObs]->Write("",TObject::kOverwrite);
        setPad4Observable((TPad*) c, iObs);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.

        // energy scale from histogram mean
        iObs = ENERGYSCALE::kESCALEARITH;
        label = ENERGYSCALE::OBS_LABELS[iObs];
        canvasName = Form("cnv_%s_%s", label.c_str(), name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        h1DeScale[iObs]->SetMarkerSize(markerSize);
        h1DeScale[iObs]->Draw("e");
        h1DeScale[iObs]->Write("",TObject::kOverwrite);
        setPad4Observable((TPad*) c, iObs);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.

        // width of energy scale from histogram std dev
        iObs = ENERGYSCALE::kERESARITH;
        label = ENERGYSCALE::OBS_LABELS[iObs];
        canvasName = Form("cnv_%s_%s", label.c_str(), name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        h1DeScale[iObs]->SetMarkerSize(markerSize);
        h1DeScale[iObs]->Draw("e");
        h1DeScale[iObs]->Write("",TObject::kOverwrite);
        setPad4Observable((TPad*) c, iObs);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.

        // width of energy scale from histogram sigmaEff
        iObs = ENERGYSCALE::kERESEFF;
        label = ENERGYSCALE::OBS_LABELS[iObs];
        canvasName = Form("cnv_%s_%s", label.c_str(), name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        h1DeScale[iObs]->SetMarkerSize(markerSize);
        h1DeScale[iObs]->Draw("e");
        h1DeScale[iObs]->Write("",TObject::kOverwrite);
        setPad4Observable((TPad*) c, iObs);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.

        // width of energy scale from histogram sigmaHM
        iObs = ENERGYSCALE::kERESHM;
        label = ENERGYSCALE::OBS_LABELS[iObs];
        canvasName = Form("cnv_%s_%s", label.c_str(), name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        h1DeScale[iObs]->SetMarkerSize(markerSize);
        h1DeScale[iObs]->Draw("e");
        h1DeScale[iObs]->Write("",TObject::kOverwrite);
        setPad4Observable((TPad*) c, iObs);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.

        // plot mean from Gaussian fit and histogram mean together
        canvasName = Form("cnv_eScaleAll_%s", name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        leg = new TLegend();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        std::vector<int> tmpIndices = {ENERGYSCALE::kESCALE, ENERGYSCALE::kESCALEARITH};
        std::vector<std::string> tmpLabels = {"Gaussian Fit", "Arithmetic"};
        std::vector<int> tmpColors = {kBlack, kGreen+2};
        int nTmp = tmpIndices.size();
        for (int j = 0; j < nTmp; ++j) {
            h1DeScale[tmpIndices[j]]->SetMarkerColor(tmpColors[j]);
            h1DeScale[tmpIndices[j]]->SetLineColor(tmpColors[j]);
            if (j == 0) {
                h1DeScale[tmpIndices[j]]->SetMarkerStyle(kFullSquare);
                h1DeScale[tmpIndices[j]]->SetMarkerSize(h1DeScale[tmpIndices[j]]->GetMarkerSize()*1.3);
            }
            if (j == 0) h1DeScale[tmpIndices[j]]->Draw("e");
            else        h1DeScale[tmpIndices[j]]->Draw("e same");
            leg->AddEntry(h1DeScale[tmpIndices[j]], tmpLabels[j].c_str(), "lpf");
        }
        legHeight = calcTLegendHeight(leg);
        legWidth = 0.56;
        setLegendPosition(leg, "NW", c, legHeight, legWidth, 0.04, 0.04, true);
        leg->SetFillColor(-1);
        leg->SetFillStyle(4000);
        leg->SetBorderSize(0);
        leg->Draw();
        setPad4Observable((TPad*) c, ENERGYSCALE::kESCALE);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.
        if (leg != 0)  leg->Delete();

        // plot resolution from Gaussian fit, histogram stdDev, sigmaEff and sigmaHM together
        canvasName = Form("cnv_eResAll_%s", name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        leg = new TLegend();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        tmpIndices = {ENERGYSCALE::kERES, ENERGYSCALE::kERESARITH, ENERGYSCALE::kERESEFF, ENERGYSCALE::kERESHM};
        tmpLabels = {"Gaussian Fit", "Arithmetic", "Effective (#pm#sigma #rightarrow 68.3%)", "FWHM / 2.35"};
        tmpColors = {kBlack, kGreen+2, kRed+1, kBlue};
        nTmp = tmpIndices.size();
        for (int j = 0; j < nTmp; ++j) {
            h1DeScale[tmpIndices[j]]->SetMarkerColor(tmpColors[j]);
            h1DeScale[tmpIndices[j]]->SetLineColor(tmpColors[j]);
            if (j == 0) {
                h1DeScale[tmpIndices[j]]->SetMarkerStyle(kFullSquare);
                h1DeScale[tmpIndices[j]]->SetMarkerSize(h1DeScale[tmpIndices[j]]->GetMarkerSize()*1.3);
            }
            if (j == 0) h1DeScale[tmpIndices[j]]->Draw("e");
            else        h1DeScale[tmpIndices[j]]->Draw("e same");
            leg->AddEntry(h1DeScale[tmpIndices[j]], tmpLabels[j].c_str(), "lpf");
        }
        legHeight = calcTLegendHeight(leg);
        legWidth = 0.56;
        setLegendPosition(leg, "NW", c, legHeight, legWidth, 0.04, 0.04, true);
        leg->SetFillColor(-1);
        leg->SetFillStyle(4000);
        leg->SetBorderSize(0);
        leg->Draw();
        setPad4Observable((TPad*) c, ENERGYSCALE::kERES);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.
        if (leg != 0)  leg->Delete();

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

        for (int i = 0; i < nPads; ++i) {
            if (pads[i] != 0)  pads[i]->Delete();
        }
        c->Close();         // do not use Delete() for TCanvas.
    }

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

    // efficiency objects
    if (isValid_hMatchNum) {
        canvasName = Form("cnv_%s_%s", getObjectStr(energyScaleHist::OBJ::kMatchNum).c_str(), name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        hMatchNum->SetMarkerSize(markerSize);
        hMatchNum->Draw("e");
        hMatchNum->Write("",TObject::kOverwrite);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.
    }
    if (isValid_hMatchDenom) {
        canvasName = Form("cnv_%s_%s", getObjectStr(energyScaleHist::OBJ::kMatchDenom).c_str(), name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        hMatchDenom->SetMarkerSize(markerSize);
        hMatchDenom->Draw("e");
        hMatchDenom->Write("",TObject::kOverwrite);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.
    }
    if (isValid_hMatchEff) {
        int iObs = ENERGYSCALE::kEFF;
        canvasName = Form("cnv_%sH1D_%s", ENERGYSCALE::OBS_LABELS[iObs].c_str() , name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        hMatchEff->SetMarkerSize(markerSize);
        hMatchEff->Draw("e");
        hMatchEff->Write("",TObject::kOverwrite);
        setPad4Observable((TPad*) c, iObs);
        drawLine4PtRange((TPad*) c);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.
    }
    if (isValid_hMatchEff && isValid_gMatchEff) {
        int iObs = ENERGYSCALE::kEFF;
        canvasName = Form("cnv_%s_%s", ENERGYSCALE::OBS_LABELS[iObs].c_str() , name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        // dummy histogram to be used as template for the graph
        TH1D* hTmp = (TH1D*)hMatchEff->Clone("hTmp");
        hTmp->Reset();
        hTmp->Draw();
        gMatchEff->SetMarkerSize(markerSize);
        gMatchEff->Draw("p e");
        gMatchEff->Write("",TObject::kOverwrite);
        setPad4Observable((TPad*) c, iObs);
        drawLine4PtRange((TPad*) c);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.
        hTmp->Delete();
    }

    // fake rate objects
    if (isValid_hFakeNum) {
        canvasName = Form("cnv_%s_%s", getObjectStr(energyScaleHist::OBJ::kFakeNum).c_str(), name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        hFakeNum->SetMarkerSize(markerSize);
        hFakeNum->Draw("e");
        hFakeNum->Write("",TObject::kOverwrite);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.
    }
    if (isValid_hFakeDenom) {
        canvasName = Form("cnv_%s_%s", getObjectStr(energyScaleHist::OBJ::kFakeDenom).c_str(), name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        hFakeDenom->SetMarkerSize(markerSize);
        hFakeDenom->Draw("e");
        hFakeDenom->Write("",TObject::kOverwrite);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.
    }
    if (isValid_hFakeRatio) {
        int iObs = ENERGYSCALE::kFAKE;
        canvasName = Form("cnv_%s_%s", ENERGYSCALE::OBS_LABELS[iObs].c_str() , name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        hFakeRatio->SetMarkerSize(markerSize);
        hFakeRatio->Draw("e");
        hFakeRatio->Write("",TObject::kOverwrite);
        setPad4Observable((TPad*) c, iObs);
        drawLine4PtRange((TPad*) c);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);

        // plot fake rate in log-scale as well
        int minBin = getMinimumBin(hFakeRatio, 0);
        double minContent = 0.001;
        if (minBin > -1) minContent = hFakeRatio->GetBinContent(minBin);
        double logYmin = TMath::Floor(TMath::Log10(minContent));
        hFakeRatio->SetMinimum(TMath::Power(10, logYmin));
        c->SetLogy(1);
        c->Update();
        canvasName = replaceAll(c->GetName(), "cnv_", "cnvLogy_");
        c->SetName(canvasName.c_str());
        c->Write("",TObject::kOverwrite);
        hFakeRatio->SetMinimum(0);  // restore the minimum after log-scale canvas.

        c->Close();         // do not use Delete() for TCanvas.
    }
    if (isValid_hFakeRatio && isValid_gFakeRatio) {
        int iObs = ENERGYSCALE::kFAKE;
        canvasName = Form("cnv_%sgraph_%s", ENERGYSCALE::OBS_LABELS[iObs].c_str() , name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        // dummy histogram to be used as template for the graph
        TH1D* hTmp = (TH1D*)hFakeRatio->Clone("hTmp");
        hTmp->Reset();
        hTmp->Draw();
        gFakeRatio->SetMarkerSize(markerSize);
        gFakeRatio->Draw("p e");
        gFakeRatio->Write("",TObject::kOverwrite);
        setPad4Observable((TPad*) c, iObs);
        drawLine4PtRange((TPad*) c);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);

        // plot fake rate in log-scale as well
        int minBinIndex = getMinimumBinIndex(gFakeRatio, 0);
        double minContent = 0.001;
        if (minBinIndex > -1) minContent = getMinimum(gFakeRatio, 0);
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
    for (int i = 0; i < nFakeParticles; ++i) {

        if (!isValid_hFakeParticle[i])  continue;
        hFakeParticle[i]->Write("",TObject::kOverwrite);

        if (!isValid_hFakeParticleRatio[i])  continue;
        hFakeParticleRatio[i]->Write("",TObject::kOverwrite);
    }
    if (isValid_hFakeNum && isValid_hFakeOtherRatio) {

        int iObs = ENERGYSCALE::kFAKE;
        canvasName = Form("cnv_%sPDGs_%s", ENERGYSCALE::OBS_LABELS[iObs].c_str() , name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        TH1D* hTmp = 0;

        // dummy histogram to be used as template
        hTmp = (TH1D*)hFakeOtherRatio->Clone("hTmp");
        hTmp->Reset();
        hTmp->SetYTitle("Fake Composition");
        hTmp->SetMaximum(1.5);
        hTmp->Draw();

        leg = new TLegend();
        for (int i = 0; i < nFakeParticles; ++i) {

            if (!isValid_hFakeParticleRatio[i])  continue;
            if (!passedMinFakeFraction[i])  continue;

            hFakeParticleRatio[i]->SetMarkerSize(markerSize);

            int markerColor = ENERGYSCALE::particlesColor[fakeIndices[i]];
            hFakeParticleRatio[i]->SetLineColor(markerColor);
            hFakeParticleRatio[i]->SetMarkerColor(markerColor);
            hFakeParticleRatio[i]->Draw("e same");

            std::string label = fakeParticles[i].label;
            leg->AddEntry(hFakeParticleRatio[i], label.c_str(), "lpf");
        }

        if (isValid_hFakeOtherRatio) {
            hFakeOtherRatio->SetMarkerSize(markerSize);
            hFakeOtherRatio->Draw("e same");
            leg->AddEntry(hFakeOtherRatio, "Other", "lpf");
        }
        legHeight = calcTLegendHeight(leg);
        legWidth = calcTLegendWidth(leg);
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
        if (leg != 0)  leg->Delete();
        if (hTmp != 0)  hTmp->Delete();
    }

    // particle composition of fake rate as function of GEN-level particle pt
     for (int i = 0; i < nFakeParticles; ++i) {

         if (!isValid_hFakeParticleGenPt[i])  continue;
         hFakeParticleGenPt[i]->Write("",TObject::kOverwrite);

         if (!isValid_hFakeParticleRatioGenPt[i])  continue;
         hFakeParticleRatioGenPt[i]->Write("",TObject::kOverwrite);
     }
     if (isValid_hFakeAllGenPt && isValid_hFakeOtherRatioGenPt) {

         hFakeAllGenPt->Write("",TObject::kOverwrite);

         int iObs = ENERGYSCALE::kFAKE;
         canvasName = Form("cnv_%sGenPtPDGs_%s", ENERGYSCALE::OBS_LABELS[iObs].c_str() , name.c_str());
         c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
         c->cd();
         setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
         TH1D* hTmp = 0;

         // dummy histogram to be used as template
         hTmp = (TH1D*)hFakeOtherRatioGenPt->Clone("hTmp");
         hTmp->Reset();
         hTmp->SetYTitle("Fake Composition");
         hTmp->SetMaximum(1.5);
         hTmp->Draw();

         TLegend* leg = new TLegend();
         for (int i = 0; i < nFakeParticles; ++i) {

             if (!isValid_hFakeParticleRatioGenPt[i])  continue;
             if (!passedMinFakeFractionGenPt[i])  continue;

             hFakeParticleRatioGenPt[i]->SetMarkerSize(markerSize);

             int markerColor = ENERGYSCALE::particlesColor[fakeIndices[i]];
             hFakeParticleRatioGenPt[i]->SetLineColor(markerColor);
             hFakeParticleRatioGenPt[i]->SetMarkerColor(markerColor);
             hFakeParticleRatioGenPt[i]->Draw("e same");

             std::string label = fakeParticles[i].label;
             leg->AddEntry(hFakeParticleRatioGenPt[i], label.c_str(), "lpf");
         }

         if (isValid_hFakeOtherRatioGenPt) {
             hFakeOtherRatioGenPt->SetMarkerSize(markerSize);
             hFakeOtherRatioGenPt->Draw("e same");
             leg->AddEntry(hFakeOtherRatioGenPt, "Other", "lpf");
         }
         legHeight = calcTLegendHeight(leg);
         legWidth = calcTLegendWidth(leg);
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

     if (line != 0)  line->Delete();
     latex->Delete();
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

    if (iObs == ENERGYSCALE::kESCALE || iObs == ENERGYSCALE::kESCALEARITH) {

        // draw horizontal lines
        std::vector<double> linesH = {0.96, 1.04, 1.1};
        for (int i = 0; i < (int)linesH.size(); ++i) {

            double y = linesH.at(i);

            double x1 = p->GetUxmin();
            double x2 = p->GetUxmax();
            double y1 = p->GetUymin();
            double y2 = p->GetUymax();
            if (y1 < y && y < y2) {
                line = new TLine(x1, y, x2, y);
                line->SetLineStyle(kDotted);
                line->SetLineWidth(line->GetLineWidth()*2);
                line->Draw();
            }
        }
    }

    if (iObs == ENERGYSCALE::kERES || iObs == ENERGYSCALE::kERESARITH ||
        iObs == ENERGYSCALE::kERESEFF || iObs == ENERGYSCALE::kERESHM) {

        // draw horizontal lines
        std::vector<double> linesH = {0.05, 0.1, 0.2};
        for (int i = 0; i < (int)linesH.size(); ++i) {

            double y = linesH.at(i);

            double x1 = p->GetUxmin();
            double x2 = p->GetUxmax();
            double y1 = p->GetUymin();
            double y2 = p->GetUymax();
            if (y1 < y && y < y2) {
                line = new TLine(x1, y, x2, y);
                line->SetLineStyle(kDashed);
                line->SetLineWidth(line->GetLineWidth()*2);
                line->Draw();
            }
        }
    }

    if (iDep == ENERGYSCALE::kETA) {
        // draw line for EE-EB transition
        double ECAL_boundary_1 = 1.4442;
        double ECAL_boundary_2 = 1.566;

        double yMin = p->GetUymin();
        double yMax = p->GetUymax();
        if (iObs == ENERGYSCALE::kEFF || iObs == ENERGYSCALE::kFAKE)  yMax = 1;

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
