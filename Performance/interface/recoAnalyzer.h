/*
 * general purpose class for reconstruction analysis
 */

#ifndef PERFORMANCE_INTERFACE_RECOANALYZER_H_
#define PERFORMANCE_INTERFACE_RECOANALYZER_H_

#include <TH1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TF1.h>
#include <TFormula.h>
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
#include "../../Utilities/mathUtil.h"
#include "../../Utilities/styleUtil.h"
#include "../../Utilities/systemUtil.h"
#include "eScaleAnalyzer.h"

namespace RECOANA {

enum OBJS {   // reco objects
    kMUON,
    kELECTRON,
    kPHOTON,
    kZBOSON,
    kJET,
    kPFCAND,
    kTRACK,
    kN_OBJS
};

const std::string OBJ_LABELS[kN_OBJS] = {
        "mu",
        "ele",
        "pho",
        "zBoson",
        "jet",
        "pfCand",
        "track"
};

enum DEPS {   // reco performance dependencies
    kETA,
    kGENPT,
    kRECOPT,
    kCENT,
    kSUMISO,
    kSIEIE,
    kN_DEPS
};

const std::string DEP_LABELS[kN_DEPS] = {
        "ETA",
        "GENPT",
        "RECOPT",
        "CENT",
        "SUMISO",
        "SIEIE"
};

enum RANGES {   // reco performance ranges/cuts
    rETA,
    rGENPT,
    rRECOPT,
    rCENT,
    rSUMISO,
    rSIEIE,
    rR9,
    kN_RANGES
};

const std::string RANGE_LABELS[kN_RANGES] = {
        "ETA",
        "GENPT",
        "RECOPT",
        "CENT",
        "SUMISO",
        "SIEIE",
        "R9"
};

// observables
enum OBS {
    kESCALE,    // energy scale
    kERES,      // energy resolution
    kESCALEARITH,    // energy scale from histogram mean
    kERESARITH,      // energy resolution from histogram std dev
    kERESEFF,      // energy resolution from sigmaEff
    kERESHM,      // energy resolution from sigmaHM
    kEFF,            // matching efficiency
    kFAKE,            // fake rate
    kN_OBS
};

const std::string OBS_LABELS[kN_OBS] = {"eScale", "eRes", "eScaleArith", "eResArith", "eResEff", "eResHM", "matchEff", "fakeRatio"};

enum CORRS {   // correction types
    k_corrE,        // very last correction giving the final energy or momentum
    k_corrEta,      // very last correction giving the final eta
    k_corrSCE,      // Super Cluster energy correction : SC E / SC Raw E
    kN_CORRS
};

const std::string CORR_LABELS[kN_CORRS] = {"corrE", "corrEta", "corrSCE"};

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

enum NEUTRALMESONS {
    k_pion0,
    k_etaMeson,
    k_etaPrime,
    k_omegaMeson,
    kN_NeutralMesons
};

const particle neutralMesons[kN_NeutralMesons] =
{
   particle("pion0", "#pi^{0}", {111}),
   particle("etaMeson", "#eta", {221}),
   particle("etaPrime", "#eta^{#prime}", {331}),
   particle("omegaMeson", "#omega", {223})
};

};

class recoAnalyzer {
public :
    recoAnalyzer(){
        nBinsX = 0;
        h2D = 0;
        isValid_h2D = false;

        for (int i = 0; i < RECOANA::kN_OBS; ++i) {
            h1D[i] = 0;
        }
        for (int i = 0; i < 8; ++i) {
            h1DeScale[i] = 0;
        }
        for (int i = 0; i < RECOANA::kN_CORRS; ++i) {
            h2Dcorr[i] = 0;
            h1Dcorr[i] = 0;
            isValid_h2Dcorr[i] = false;
        }

        h2Dcc = 0;
        isValid_h2Dcc = false;

        indexFncFinal = ESANA::kDSCB_92;
        indicesFnc = {ESANA::kGAUS_95, ESANA::kDSCB_92};

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
                RECOANA::FAKECAND::k_kaonS,
                RECOANA::FAKECAND::k_pionch,
                RECOANA::FAKECAND::k_kaonL,
                RECOANA::FAKECAND::k_kaonch,
                RECOANA::FAKECAND::k_electron,
                RECOANA::FAKECAND::k_nucleon,
                RECOANA::FAKECAND::k_lambda,
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
            fakeParticles[i] = RECOANA::particles[index];

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

        recoObj = -1;
        dep = -1;

        name = "";
        title = "";
        titleRanges = "";
        titleX = "";
        titleOffsetX = 1;
        titleOffsetY = 1;
        textFont = 43;
        textSize = 32;
        textOffsetX = 0.06;
        textOffsetY = 0.06;

        xMin = {0, 0};
        xMax = {-1, -1};
        yMin = {0, 0};
        yMax = {-1, -1};

        for (int i=0; i<RECOANA::kN_RANGES; ++i) {
            ranges[i][0] = 0;
            ranges[i][1] = -1;  // no upper bound
        }
    };
    ~recoAnalyzer(){};

    void FillH2D(double energyScale, double x, double w, std::vector<double> vars);
    void FillH2DCorr(double correction, double x, int iCorr, double w, std::vector<double> vars);
    void FillH2Dcc(double genPt, double recoPt, double w, std::vector<double> vars);

    void FillHNum(double x, double w, std::vector<double> vars);
    void FillHDenom(double x, double w, std::vector<double> vars);
    void FillHNumFake(double x, double w, std::vector<double> vars);
    void FillHDenomFake(double x, double w, std::vector<double> vars);
    void FillHFakeParticle(double x, int pdg, double w, std::vector<double> vars);
    void FillHFakeParticleGenPt(double genPt, int pdg, double w, std::vector<double> vars);

    bool insideRange(std::vector<double> vars);

    std::string getRangeText(int iRange);

    std::string getBinEdgeText(int binLow, int binUp);

    bool isValid();
    void updateTH1();
    void updateH1DsliceY();
    void updateH1DeScale();
    void updateH1Dcorr();

    int getParticleIndex(int pdg);
    int getFakePDG(int iParticle);
    std::string getFakePDGstr(int iParticle);
    static std::string getObjectStr(int iObj);
    static std::string getTObjectStr(int iTObj);
    std::string getObjectName(int iObj, int iTObj = 0);

    void prepareTitleRanges();
    void prepareTextLinesRanges();
    std::string getTitleAll();
    std::vector<std::string> getTextLinesAll();
    static std::vector<std::string> splitTextLines(std::vector<std::string> textLines, int nColumns);

    void postLoop();
    void fitRecoGen();
    void calcMatchEff();
    void calcFakeRatio();
    void calcFakeParticleRatio();
    void calcFakeParticleRatioGenPt();
    void writeObjects(TCanvas* c);

    static void setPad4Observable(TPad* p, int iObs, int iDep);
    void setPad4Observable(TPad* p, int iObs);
    void setLatex(TLatex* latex, std::string position);
    void drawLine4PtRange(TPad* p, int lineColor = kBlack);

    int nBinsX;
    TH2D* h2D;
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
    TH1D* h1D[RECOANA::kN_OBS];
    bool isValid_h2D;

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
     * histograms for corrections applied to the RECO object
     */
    TH2D* h2Dcorr[RECOANA::kN_CORRS];
    TH1D* h1Dcorr[RECOANA::kN_CORRS];
    bool isValid_h2Dcorr[RECOANA::kN_CORRS];

    std::vector<TH1D*> h1DsliceY;           // energy scale distribution for each bin along x-axis
    /*
     * analyzers for 1D energy scale distribution histograms in h1DsliceY
     * esa is 2D vector with [nBinsX][nFitFncs]
     * esa[i][0] : initial fit from TH2::FitSlicesY
     * esa[i][1] : fit is seeded by FitSlicesY, uses bin range that covers 95% of the integral
     * esa[i][2] : fit is seeded by FitSlicesY, uses bin range that covers 98% of the integral
     */
    std::vector<std::vector<eScaleAnalyzer>> esa;
    int indexFncFinal;    // index of the fit function to set the bins of h1DeScale[0], h1DeScale[1]
                          // function whose results will be the final fit results
    std::vector<int> indicesFnc;    // indices of the fit functions to be shown in the 1D energy scale plots

    TH2D* h2Dcc;        // reco pt vs. gen pt correlation histogram.
    bool isValid_h2Dcc;

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
    std::vector<RECOANA::particle> fakeParticles;
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
        k_corrE,
        k_corrEta,
        k_corrSCE,
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

    int recoObj;
    int dep;            // If the x-axis is eta, then dep = RECOANA::kETA

    std::string name;   // this is basically histogram name excluding the "h1D"/"h2D" prefix
    std::string title;
    std::string titleRanges;
    std::string titleX;
    float titleOffsetX;
    float titleOffsetY;
    std::vector<std::string> textLines;
    std::vector<std::string> textLinesRanges;
    float textFont;
    float textSize;
    float textOffsetX;
    float textOffsetY;

    std::vector<float> xMin;
    std::vector<float> xMax;
    std::vector<float> yMin;
    std::vector<float> yMax;

    // range of oberservables for which the histograms are made.
    // histograms are filled if range[i][0] <= observable < range[i][1]
    double ranges[RECOANA::kN_RANGES][2];
};

void recoAnalyzer::FillH2D(double energyScale, double x, double w, std::vector<double> vars)
{
    if (isValid_h2D && insideRange(vars))
        h2D->Fill(x, energyScale, w);
}

void recoAnalyzer::FillH2DCorr(double correction, double x, int iCorr, double w, std::vector<double> vars)
{
    if (isValid_h2Dcorr[iCorr] && insideRange(vars))
        h2Dcorr[iCorr]->Fill(x, correction, w);
}

void recoAnalyzer::FillH2Dcc(double genPt, double recoPt, double w, std::vector<double> vars)
{
    vars[RECOANA::rGENPT] = -1;
    vars[RECOANA::rRECOPT] = -1;

    if (isValid_h2Dcc && insideRange(vars))
        h2Dcc->Fill(genPt, recoPt, w);
}

void recoAnalyzer::FillHNum(double x, double w, std::vector<double> vars)
{
    if (isValid_hMatchNum && insideRange(vars))
        hMatchNum->Fill(x, w);
}

void recoAnalyzer::FillHDenom(double x, double w, std::vector<double> vars)
{
    vars[RECOANA::rRECOPT] = -1;
    vars[RECOANA::rSUMISO] = -999;
    vars[RECOANA::rSIEIE] = -1;
    vars[RECOANA::rR9] = -1;

    if (isValid_hMatchDenom && insideRange(vars))
        hMatchDenom->Fill(x, w);
}
/*
 * fill numerator for fake rate
 * no GEN-level selection is applied
 */
void recoAnalyzer::FillHNumFake(double x, double w, std::vector<double> vars)
{
    vars[RECOANA::rGENPT] = -1;

    if (isValid_hFakeNum && insideRange(vars)) {
        hFakeNum->Fill(x, w);
    }
}

/*
 * fill denominator for fake rate
 * no GEN-level selection is applied
 */
void recoAnalyzer::FillHDenomFake(double x, double w, std::vector<double> vars)
{
    vars[RECOANA::rGENPT] = -1;

    if (isValid_hFakeDenom && insideRange(vars))
        hFakeDenom->Fill(x, w);
}

void recoAnalyzer::FillHFakeParticle(double x, int pdg, double w, std::vector<double> vars)
{
    if (insideRange(vars)) {

        int iParticle = getParticleIndex(pdg);

        if (iParticle > -1 && isValid_hFakeParticle[iParticle]) {
            hFakeParticle[iParticle]->Fill(x, w);
        }
        else if (isValid_hFakeOther){
            hFakeOther->Fill(x, w);
        }
    }
}

void recoAnalyzer::FillHFakeParticleGenPt(double genPt, int pdg, double w, std::vector<double> vars)
{
    vars[RECOANA::rGENPT] = -1;

    if (insideRange(vars)) {

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
bool recoAnalyzer::insideRange(std::vector<double> vars)
{
    double eta = vars[RECOANA::rETA];
    double genPt = vars[RECOANA::rGENPT];
    double recoPt = vars[RECOANA::rRECOPT];
    double cent = vars[RECOANA::rCENT];

    // make sure to pass the selection if no explicit kinematic range is specified.
    if (eta == -999)  eta = ranges[RECOANA::rETA][0];
    if (genPt == -1) genPt = ranges[RECOANA::rGENPT][0];
    if (recoPt == -1)  recoPt = ranges[RECOANA::rRECOPT][0];
    if (cent == -1)  cent = ranges[RECOANA::rCENT][0];

    if (recoObj == RECOANA::OBJS::kPHOTON) {
        double sumIso = vars[RECOANA::rSUMISO];
        double sieie = vars[RECOANA::rSIEIE];
        double r9 = vars[RECOANA::rR9];

        if (sumIso == -999)  sumIso = ranges[RECOANA::rSUMISO][0];
        if (sieie == -1)  sieie = ranges[RECOANA::rSIEIE][0];
        if (r9 == -1)  r9 = ranges[RECOANA::rR9][0];

        if(ranges[RECOANA::rETA][0] <= TMath::Abs(eta) &&
           (ranges[RECOANA::rETA][1] <= -1 || TMath::Abs(eta) < ranges[RECOANA::rETA][1])) {
        if(ranges[RECOANA::rGENPT][0] <= genPt &&
           (ranges[RECOANA::rGENPT][1] <= -1 || genPt < ranges[RECOANA::rGENPT][1])) {
        if(ranges[RECOANA::rRECOPT][0] <= recoPt &&
           (ranges[RECOANA::rRECOPT][1] <= -1 || recoPt < ranges[RECOANA::rRECOPT][1])) {
        if(ranges[RECOANA::rCENT][0] <= cent &&
           (ranges[RECOANA::rCENT][1] <= -1 || cent < ranges[RECOANA::rCENT][1])) {
        if((ranges[RECOANA::rSUMISO][0] <= sumIso && sumIso < ranges[RECOANA::rSUMISO][1]) ||
           (ranges[RECOANA::rSUMISO][0] > ranges[RECOANA::rSUMISO][1])) {
        if(ranges[RECOANA::rSIEIE][0] <= sieie &&
           (ranges[RECOANA::rSIEIE][1] <= -1 || sieie < ranges[RECOANA::rSIEIE][1])) {
        if(ranges[RECOANA::rR9][0] <= r9 &&
           (ranges[RECOANA::rR9][1] <= -1 || r9 < ranges[RECOANA::rR9][1])) {
                return true;
        }}}}}}}
        return false;
    }
    else
        return false;
}

std::string recoAnalyzer::getRangeText(int iRange) {

    std::string res = "";

    switch (iRange) {
    case RECOANA::rETA : {

        if (ranges[RECOANA::rETA][0] > ranges[RECOANA::rETA][1])
            break;
        else if (ranges[RECOANA::rETA][0] <= 0 && ranges[RECOANA::rETA][1] > 0)
            res  = Form("|#eta|<%.2f", ranges[RECOANA::rETA][1]);
        else if (ranges[RECOANA::rETA][0] > 0 && ranges[RECOANA::rETA][1] > 0)
            res  = Form("%.2f<|#eta|<%.2f", ranges[RECOANA::rETA][0], ranges[RECOANA::rETA][1]);
        break;
    }
    case RECOANA::rGENPT : {

        if (ranges[RECOANA::rGENPT][0] > 0 && ranges[RECOANA::rGENPT][1] <= -1)
            res  = Form("p_{T}^{gen}>%.0f", ranges[RECOANA::rGENPT][0]);
        else if (ranges[RECOANA::rGENPT][0] <= 0 && ranges[RECOANA::rGENPT][1] > 0)
            res  = Form("p_{T}^{gen}<%.0f", ranges[RECOANA::rGENPT][1]);
        else if (ranges[RECOANA::rGENPT][0] > 0 && ranges[RECOANA::rGENPT][1] > 0)
            res  = Form("%.0f<p_{T}^{gen}<%.0f", ranges[RECOANA::rGENPT][0], ranges[RECOANA::rGENPT][1]);
        break;
    }
    case RECOANA::rRECOPT : {

        if (ranges[RECOANA::rRECOPT][0] > 0 && ranges[RECOANA::rRECOPT][1] <= -1)
            res  = Form("p_{T}^{reco}>%.0f", ranges[RECOANA::rRECOPT][0]);
        else if (ranges[RECOANA::rRECOPT][0] <= 0 && ranges[RECOANA::rRECOPT][1] > 0)
            res = Form("p_{T}^{reco}<%.0f", ranges[RECOANA::rRECOPT][1]);
        else if (ranges[RECOANA::rRECOPT][0] > 0 && ranges[RECOANA::rRECOPT][1] > 0)
            res = Form("%.0f<p_{T}^{reco}<%.0f", ranges[RECOANA::rRECOPT][0], ranges[RECOANA::rRECOPT][1]);
        break;
    }
    case RECOANA::rCENT : {

        if (ranges[RECOANA::rCENT][0] >= 0 && ranges[RECOANA::rCENT][1] > 0)
            res = Form("Cent:%.0f-%.0f%%", ranges[RECOANA::rCENT][0], ranges[RECOANA::rCENT][1]);
        break;
    }
    case RECOANA::rSUMISO : {

        if (recoObj == RECOANA::kPHOTON) {

            if (ranges[RECOANA::rSUMISO][0] > ranges[RECOANA::rSUMISO][1])
                break;
            else if (ranges[RECOANA::rSUMISO][0] > -999 && ranges[RECOANA::rSUMISO][1] <= -999)
                res  = Form("sumIso>%.1f", ranges[RECOANA::rSUMISO][0]);
            else if (ranges[RECOANA::rSUMISO][0] <= -999 && ranges[RECOANA::rSUMISO][1] > -999)
                res = Form("sumIso<%.1f", ranges[RECOANA::rSUMISO][1]);
            else if (ranges[RECOANA::rSUMISO][0] > -999 && ranges[RECOANA::rSUMISO][1] > -999)
                res = Form("%.1f<sumIso<%.1f", ranges[RECOANA::rSUMISO][0], ranges[RECOANA::rSUMISO][1]);
        }
        break;
    }
    case RECOANA::rSIEIE : {

        if (recoObj == RECOANA::kPHOTON) {

            if (ranges[RECOANA::rSIEIE][0] > 0 && ranges[RECOANA::rSIEIE][1] <= -1)
                res  = Form("#sigma_{#eta#eta}>%.2f", ranges[RECOANA::rSIEIE][0]);
            else if (ranges[RECOANA::rSIEIE][0] <= 0 && ranges[RECOANA::rSIEIE][1] > 0)
                res = Form("#sigma_{#eta#eta}<%.2f", ranges[RECOANA::rSIEIE][1]);
            else if (ranges[RECOANA::rSIEIE][0] > 0 && ranges[RECOANA::rSIEIE][1] > 0)
                res = Form("%.2f<#sigma_{#eta#eta}<%.2f", ranges[RECOANA::rSIEIE][0], ranges[RECOANA::rSIEIE][1]);
        }
        break;
    }
    case RECOANA::rR9 : {

        if (recoObj == RECOANA::kPHOTON) {

            if (ranges[RECOANA::rR9][0] > 0 && ranges[RECOANA::rR9][1] <= -1)
                res  = Form("R9>%.2f", ranges[RECOANA::rR9][0]);
            else if (ranges[RECOANA::rR9][0] <= 0 && ranges[RECOANA::rR9][1] > 0)
                res = Form("R9<%.2f", ranges[RECOANA::rR9][1]);
            else if (ranges[RECOANA::rR9][0] > 0 && ranges[RECOANA::rR9][1] > 0)
                res = Form("%.2f<R9<%.2f", ranges[RECOANA::rR9][0], ranges[RECOANA::rR9][1]);
        }
        break;
    }
    default : {
        res = "";
        break;
    }
    }

    return res;
}

/*
 * get range for histograms in "h1DsliceY"
 *
 * Ex. dep = RECOANA::kGENPT
 *     x-axis of h2D = {20,30,40,50,70,100}
 *     then
 *     getBinEdgeText(1, 1) returns "20<p_{T}^{gen}<30"
 *     getBinEdgeText(2, 2) returns "30<p_{T}^{gen}<40"
 *     getBinEdgeText(3, 4) returns "40<p_{T}^{gen}<70"
 */
std::string recoAnalyzer::getBinEdgeText(int binLow, int binUp)
{
    std::string res = "";

    double xLow = h2D->GetXaxis()->GetBinLowEdge(binLow);
    double xUp = h2D->GetXaxis()->GetBinUpEdge(binUp);

    if (dep == RECOANA::kETA) {
        res  = Form("%.2f < #eta < %.2f", xLow, xUp);
    }
    else if (dep == RECOANA::kGENPT) {
        res  = Form("%.0f <p_{T}^{gen}< %.0f", xLow, xUp);
    }
    else if (dep == RECOANA::kRECOPT) {
        res = Form("%.0f <p_{T}^{reco}< %.0f", xLow, xUp);
    }
    else if (dep == RECOANA::kCENT) {
        res = Form("Cent:%.0f-%.0f%%", xLow, xUp);
    }
    else if (dep == RECOANA::kSUMISO) {
        res = Form("%.1f < sumIso < %.1f", xLow, xUp);
    }
    else if (dep == RECOANA::kSIEIE) {
        res = Form("%.3f < #sigma_{#eta#eta} < %.3f", xLow, xUp);
    }

    return res;
}

bool recoAnalyzer::isValid()
{
    if (isValid_h2D)  return true;
    if (isValid_h2Dcc)  return true;

    if (isValid_hMatchNum)  return true;
    if (isValid_hMatchDenom)  return true;
    if (isValid_hMatchEff)  return true;
    if (isValid_gMatchEff)  return true;

    if (isValid_hFakeNum)  return true;
    if (isValid_hFakeDenom)  return true;
    if (isValid_hFakeRatio)  return true;
    if (isValid_gFakeRatio)  return true;

    for (int i = 0; i < nFakeParticles; ++i) {
        if (isValid_hFakeParticle[i])  return true;
        if (isValid_hFakeParticleRatio[i])  return true;

        if (isValid_hFakeParticleGenPt[i])  return true;
        if (isValid_hFakeParticleRatioGenPt[i])  return true;
    }

    if (isValid_hFakeOther)  return true;
    if (isValid_hFakeOtherRatio)  return true;

    if (isValid_hFakeOtherRatio)  return true;
    if (isValid_hFakeOtherRatioGenPt)  return true;
    if (isValid_hFakeAllGenPt)  return true;

    return false;
}

void recoAnalyzer::updateTH1()
{
    isValid_h2D = (h2D != 0 && !h2D->IsZombie());
    if (isValid_h2D) {
        nBinsX = h2D->GetXaxis()->GetNbins();
    }
    for (int i = 0; i < RECOANA::kN_CORRS; ++i) {
        isValid_h2Dcorr[i] = (h2Dcorr[i] != 0 && !h2Dcorr[i]->IsZombie());
    }
    isValid_h2Dcc = (h2Dcc != 0 && !h2Dcc->IsZombie());

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

    if(isValid_h2D) {
        h2D->SetTitle(title.c_str());
    }
    for (int i = 0; i < RECOANA::kN_CORRS; ++i) {
        if (isValid_h2Dcorr[i]) {
            h2Dcorr[i]->SetTitle(title.c_str());
        }
    }
    if(isValid_h2Dcc) {
        h2Dcc->SetTitle(title.c_str());
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

void recoAnalyzer::updateH1DsliceY()
{
    h1DsliceY.clear();
    h1DsliceY.resize(nBinsX);

    // reco pt / gen pt distributions
    TH1D* hTmp = 0;

    for (int i=1; i<=nBinsX; ++i) {

        hTmp = h2D->ProjectionY(Form("h1D_projYbin%d_%s", i, name.c_str()), i, i, "");
        hTmp->SetTitleOffset(titleOffsetX, "X");
        hTmp->SetTitleOffset(titleOffsetY, "Y");
        hTmp->SetStats(false);
        hTmp->GetXaxis()->CenterTitle();
        hTmp->GetYaxis()->CenterTitle();
        hTmp->SetMarkerStyle(kFullCircle);

        h1DsliceY[i-1] = hTmp;
    }
}

void recoAnalyzer::updateH1DeScale()
{
    for (int i = 1; i <= nBinsX; ++i) {

        if (!esa[i-1][indexFncFinal].isValid_h) continue;

        if (esa[i-1][indexFncFinal].isValid_f1) {
            h1DeScale[RECOANA::kESCALE]->SetBinContent(i, esa[i-1][indexFncFinal].f1Mean);
            h1DeScale[RECOANA::kESCALE]->SetBinError(i, esa[i-1][indexFncFinal].f1MeanErr);

            h1DeScale[RECOANA::kERES]->SetBinContent(i, esa[i-1][indexFncFinal].f1Sigma);
            h1DeScale[RECOANA::kERES]->SetBinError(i, esa[i-1][indexFncFinal].f1SigmaErr);

            h1DeScale[6]->SetBinContent(i, esa[i-1][indexFncFinal].f1->GetParameter(0));
            h1DeScale[6]->SetBinError(i, esa[i-1][indexFncFinal].f1->GetParError(0));

            h1DeScale[7]->SetBinContent(i, esa[i-1][indexFncFinal].f1Chi2/esa[i-1][indexFncFinal].f1Ndf);
            h1DeScale[7]->SetBinError(i, 0);
        }

        // arithmetic mean and std dev of energy scale distributions
        double y = esa[i-1][indexFncFinal].hMean;
        double yErr = esa[i-1][indexFncFinal].hMeanErr;
        h1DeScale[RECOANA::kESCALEARITH]->SetBinContent(i, y);
        h1DeScale[RECOANA::kESCALEARITH]->SetBinError(i, yErr);

        y= esa[i-1][indexFncFinal].hStdDev;
        yErr = esa[i-1][indexFncFinal].hStdDevErr;
        h1DeScale[RECOANA::kERESARITH]->SetBinContent(i, y);
        h1DeScale[RECOANA::kERESARITH]->SetBinError(i, yErr);

        y = esa[i-1][indexFncFinal].sigmaEff;
        yErr = esa[i-1][indexFncFinal].sigmaEffErr;
        h1DeScale[RECOANA::kERESEFF]->SetBinContent(i, y);
        h1DeScale[RECOANA::kERESEFF]->SetBinError(i, yErr);

        y = esa[i-1][indexFncFinal].sigmaHM;
        yErr = esa[i-1][indexFncFinal].sigmaHMErr;
        h1DeScale[RECOANA::kERESHM]->SetBinContent(i, y);
        h1DeScale[RECOANA::kERESHM]->SetBinError(i, yErr);
    }
}

void recoAnalyzer::updateH1Dcorr()
{
    for (int i = 0; i < RECOANA::kN_CORRS; ++i) {

        if (!isValid_h2Dcorr[i]) continue;

        h2Dcorr[i]->SetTitleOffset(titleOffsetX, "X");
        h2Dcorr[i]->SetTitleOffset(titleOffsetY, "Y");
        h2Dcorr[i]->SetStats(false);
        h2Dcorr[i]->GetXaxis()->CenterTitle();
        h2Dcorr[i]->GetYaxis()->CenterTitle();

        std::string tmpHistName = replaceAll(h2Dcorr[i]->GetName(), "h2D_", "h1D_");
        h1Dcorr[i] = (TH1D*)h2Dcorr[i]->ProjectionX(tmpHistName.c_str());

        std::string tmpTitleY = Form("< %s >", h2Dcorr[i]->GetYaxis()->GetTitle());
        h1Dcorr[i]->SetYTitle(tmpTitleY.c_str());

        h1Dcorr[i]->SetTitleOffset(titleOffsetX, "X");
        h1Dcorr[i]->SetTitleOffset(titleOffsetY, "Y");
        h1Dcorr[i]->SetStats(false);
        h1Dcorr[i]->GetXaxis()->CenterTitle();
        h1Dcorr[i]->GetYaxis()->CenterTitle();
        h1Dcorr[i]->SetMarkerStyle(kFullCircle);

        int nBinsTmp = h2Dcorr[i]->GetXaxis()->GetNbins();
        for (int iBin = 1; iBin <= nBinsTmp; ++iBin) {
            double mean = h2Dcorr[i]->ProjectionY("hTmp", iBin, iBin)->GetMean();
            double meanErr = h2Dcorr[i]->ProjectionY("hTmp", iBin, iBin)->GetMeanError();

            h1Dcorr[i]->SetBinContent(iBin, mean);
            h1Dcorr[i]->SetBinError(iBin, meanErr);
        }
    }
}

/*
 * return the first particle that contains the given PDG
 */
int recoAnalyzer::getParticleIndex(int pdg)
{
    for (int i = 0; i < nFakeParticles; ++i) {

        std::vector<int> PDGs = fakeParticles[i].PDG;
        std::vector<int>::iterator it = std::find(PDGs.begin(), PDGs.end(), pdg);

        if (it != PDGs.end())  return i;
    }

    return -1;
}

int recoAnalyzer::getFakePDG(int iParticle)
{
    if (iParticle < nFakeParticles)  return fakeParticles[iParticle].PDG[0];
    else                   return 0;
}

std::string recoAnalyzer::getFakePDGstr(int iParticle)
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

std::string recoAnalyzer::getObjectStr(int iObj)
{
    switch (iObj) {
    case recoAnalyzer::OBJ::k_corrE :
        return RECOANA::CORR_LABELS[RECOANA::k_corrE].c_str();
    case recoAnalyzer::OBJ::k_corrEta :
        return RECOANA::CORR_LABELS[RECOANA::k_corrEta].c_str();
    case recoAnalyzer::OBJ::k_corrSCE :
        return RECOANA::CORR_LABELS[RECOANA::k_corrSCE].c_str();
    case recoAnalyzer::OBJ::kMatchNum :
        return "MatchNum";
    case recoAnalyzer::OBJ::kMatchDenom :
        return "MatchDenom";
    case recoAnalyzer::OBJ::kMatchEff :
        return "MatchEff";
    case recoAnalyzer::OBJ::kFakeNum :
        return "FakeNum";
    case recoAnalyzer::OBJ::kFakeDenom :
        return "FakeDenom";
    case recoAnalyzer::OBJ::kFakeRatio :
        return "FakeRatio";
    case recoAnalyzer::OBJ::kFakeParticle :
        return "FakePDG";
    case recoAnalyzer::OBJ::kFakeParticleRatio :
        return "FakeRatioPDG";
    case recoAnalyzer::OBJ::kFakeOther :
        return "FakeOther";
    case recoAnalyzer::OBJ::kFakeOtherRatio :
        return "FakeOtherRatio";
    case recoAnalyzer::OBJ::kFakeParticleGenPt :
        return "FakeGenPtPDG";
    case recoAnalyzer::OBJ::kFakeParticleRatioGenPt :
        return "FakeRatioGenPtPDG";
    case recoAnalyzer::OBJ::kFakeOtherGenPt :
        return "FakeOtherGenPt";
    case recoAnalyzer::OBJ::kFakeOtherRatioGenPt :
        return "FakeOtherRatioGenPt";
    case recoAnalyzer::OBJ::kFakeAllGenPt :
        return "FakeAllGenPt";
    default :
        return "";
    }

    return "";
}

std::string recoAnalyzer::getTObjectStr(int iTObj)
{
    switch (iTObj) {
    case recoAnalyzer::TOBJ::kTH1D :
        return "h";
    case recoAnalyzer::TOBJ::kTH2D :
        return "h2D_";
    case recoAnalyzer::TOBJ::kTGraph :
        return "g";
    default :
        return "";
    }

    return "";
}

std::string recoAnalyzer::getObjectName(int iObj, int iTObj)
{
    std::string objStr =  recoAnalyzer::getObjectStr(iObj);
    std::string tObjStr = recoAnalyzer::getTObjectStr(iTObj);

    return Form("%s%s_%s", tObjStr.c_str(), objStr.c_str(), name.c_str());
}

/*
 * prepare the object title using the ranges
 */
void recoAnalyzer::prepareTitleRanges()
{
    // prepare histogram title
    // if etaStr is empty, then there is no eta range/selection.
    std::string etaStr = "";
    std::string genPtStr = "";
    std::string recoPtStr = "";
    std::string centStr = "";
    std::string sumIsoStr = "";
    std::string sieieStr = "";
    std::string r9Str = "";

    etaStr  = getRangeText(RECOANA::rETA);
    genPtStr = getRangeText(RECOANA::rGENPT);
    recoPtStr  = getRangeText(RECOANA::rRECOPT);
    centStr  = getRangeText(RECOANA::rCENT);
    sumIsoStr = getRangeText(RECOANA::rSUMISO);
    sieieStr = getRangeText(RECOANA::rSIEIE);
    r9Str = getRangeText(RECOANA::rR9);

    titleRanges = "";
    if (etaStr.size() > 0)  titleRanges.append(Form("%s", etaStr.c_str()));
    if (genPtStr.size() > 0)  titleRanges.append(Form(", %s", genPtStr.c_str()));
    if (recoPtStr.size() > 0) titleRanges.append(Form(", %s", recoPtStr.c_str()));
    if (centStr.size() > 0)  titleRanges.append(Form(", %s", centStr.c_str()));
    if (sumIsoStr.size() > 0)  titleRanges.append(Form(", %s", sumIsoStr.c_str()));
    if (sieieStr.size() > 0)  titleRanges.append(Form(", %s", sieieStr.c_str()));
    if (r9Str.size() > 0)  titleRanges.append(Form(", %s", r9Str.c_str()));
}

/*
 * prepare the text lines using the ranges
 */
void recoAnalyzer::prepareTextLinesRanges()
{
    // if etaStr is empty, then there is no eta range/selection.
    std::string etaStr = "";
    std::string genPtStr = "";
    std::string recoPtStr = "";
    std::string centStr = "";
    std::string sumIsoStr = "";
    std::string sieieStr = "";
    std::string r9Str = "";

    etaStr  = getRangeText(RECOANA::rETA);
    genPtStr = getRangeText(RECOANA::rGENPT);
    recoPtStr  = getRangeText(RECOANA::rRECOPT);
    centStr  = getRangeText(RECOANA::rCENT);
    sumIsoStr = getRangeText(RECOANA::rSUMISO);
    sieieStr = getRangeText(RECOANA::rSIEIE);
    r9Str = getRangeText(RECOANA::rR9);

    textLinesRanges.clear();
    if (etaStr.size() > 0)  textLinesRanges.push_back(etaStr);
    if (genPtStr.size() > 0)  textLinesRanges.push_back(genPtStr);
    if (recoPtStr.size() > 0) textLinesRanges.push_back(recoPtStr);
    if (centStr.size() > 0)  textLinesRanges.push_back(centStr);
    if (sumIsoStr.size() > 0)  textLinesRanges.push_back(sumIsoStr);
    if (sieieStr.size() > 0)  textLinesRanges.push_back(sieieStr);
    if (r9Str.size() > 0)  textLinesRanges.push_back(r9Str);
}

std::string recoAnalyzer::getTitleAll()
{
    std::string res = title;

    if (res.size() > 0)  res.append(Form(" %s", titleRanges.c_str()));
    else                 res.append(titleRanges.c_str());

    return res;
}

std::vector<std::string> recoAnalyzer::getTextLinesAll()
{
    std::vector<std::string> res = textLines;

    res.insert(res.end(), textLinesRanges.begin(), textLinesRanges.end());

    return res;
}

/*
 * split the textLines so that the information looks uniform over multiple columns
 */
std::vector<std::string> recoAnalyzer::splitTextLines(std::vector<std::string> textLines, int nColumns)
{
    std::vector<std::string> res;
    res.resize(nColumns);

    int nTextLines = textLines.size();
    int nLinesPerColumn = nTextLines / nColumns;
    int remainder = nTextLines % nColumns;

    int iLine = 0;
    for (int iCol = 0; iCol < nColumns; ++iCol) {

        std::string lineTmp = "";
        for (int j = 0; j < nLinesPerColumn; ++j) {

            if (lineTmp.size() > 0)  lineTmp.append(Form(", %s", textLines[iLine].c_str()));
            else                     lineTmp.append(textLines[iLine].c_str());

            ++iLine;
        }
        if (iCol < remainder) {

            if (lineTmp.size() > 0)  lineTmp.append(Form(", %s", textLines[iLine].c_str()));
            else                     lineTmp.append(textLines[iLine].c_str());

            ++iLine;
        }

        res[iCol] = lineTmp;
    }

    return res;
}

void recoAnalyzer::postLoop()
{
    if (isValid_h2D) {
        h2D->SetTitleOffset(titleOffsetX, "X");
        h2D->SetTitleOffset(titleOffsetY, "Y");
        h2D->SetStats(false);
        h2D->GetXaxis()->CenterTitle();
        h2D->GetYaxis()->CenterTitle();

        TObjArray aSlices;
        h2D->FitSlicesY(0,0,-1,0,"Q LL M N", &aSlices);
        std::string tmpTitleY = h2D->GetYaxis()->GetTitle();
        std::string titleY_eScale = Form("< %s >", tmpTitleY.c_str());
        std::string titleY_eRes = Form("#sigma( %s )", tmpTitleY.c_str());

        // energy scale
        int iObs = RECOANA::kESCALE;
        std::string label = RECOANA::OBS_LABELS[iObs];
        h1DeScale[iObs] = (TH1D*)aSlices.At(1)->Clone(Form("h1D_%s_%s", label.c_str(), name.c_str()));
        h1DeScale[iObs]->SetTitle(title.c_str());
        h1DeScale[iObs]->SetXTitle(titleX.c_str());
        setTH1_energyScale(h1DeScale[iObs], titleOffsetX, titleOffsetY);
        h1DeScale[iObs]->SetYTitle(titleY_eScale.c_str());
        if (yMax[iObs] > yMin[iObs])
            h1DeScale[iObs]->SetAxisRange(yMin[iObs], yMax[iObs], "Y");

        // width of energy scale
        iObs = RECOANA::kERES;
        label = RECOANA::OBS_LABELS[iObs];
        h1DeScale[iObs] = (TH1D*)aSlices.At(2)->Clone(Form("h1D_%s_%s", label.c_str(), name.c_str()));
        h1DeScale[iObs]->SetTitle(title.c_str());
        h1DeScale[iObs]->SetXTitle(titleX.c_str());
        setTH1_energyWidth(h1DeScale[iObs], titleOffsetX, titleOffsetY);
        h1DeScale[iObs]->SetYTitle(titleY_eRes.c_str());
        if (yMax[iObs] > yMin[iObs])
            h1DeScale[iObs]->SetAxisRange(yMin[iObs], yMax[iObs], "Y");

        // energy scale with arithmetic mean
        iObs = RECOANA::kESCALEARITH;
        label = RECOANA::OBS_LABELS[iObs];
        h1DeScale[iObs] = (TH1D*)h1DeScale[RECOANA::kESCALE]->Clone(Form("h1D_%s_%s", label.c_str(), name.c_str()));
        h1DeScale[iObs]->SetTitle(title.c_str());
        h1DeScale[iObs]->SetXTitle(titleX.c_str());
        setTH1_energyScale(h1DeScale[iObs], titleOffsetX, titleOffsetY);
        h1DeScale[iObs]->SetYTitle(titleY_eScale.c_str());
        h1DeScale[iObs]->SetYTitle(Form("%s (Arith)", h1DeScale[iObs]->GetYaxis()->GetTitle()));
        if (yMax[RECOANA::kESCALE] > yMin[RECOANA::kESCALE])
            h1DeScale[iObs]->SetAxisRange(yMin[RECOANA::kESCALE], yMax[RECOANA::kESCALE], "Y");

        // width of energy scale with arithmetic std dev
        iObs = RECOANA::kERESARITH;
        label = RECOANA::OBS_LABELS[iObs];
        h1DeScale[iObs] = (TH1D*)h1DeScale[RECOANA::kERES]->Clone(Form("h1D_%s_%s", label.c_str(), name.c_str()));
        h1DeScale[iObs]->SetTitle(title.c_str());
        h1DeScale[iObs]->SetXTitle(titleX.c_str());
        setTH1_energyWidth(h1DeScale[iObs], titleOffsetX, titleOffsetY);
        h1DeScale[iObs]->SetYTitle(titleY_eRes.c_str());
        h1DeScale[iObs]->SetYTitle(Form("%s (Arith)", h1DeScale[iObs]->GetYaxis()->GetTitle()));
        if (yMax[RECOANA::kERES] > yMin[RECOANA::kERES])
            h1DeScale[iObs]->SetAxisRange(yMin[RECOANA::kERES], yMax[RECOANA::kERES], "Y");

        // width of energy scale with sigmaEff
        iObs = RECOANA::kERESEFF;
        label = RECOANA::OBS_LABELS[iObs];
        h1DeScale[iObs] = (TH1D*)h1DeScale[RECOANA::kERES]->Clone(Form("h1D_%s_%s", label.c_str(), name.c_str()));
        h1DeScale[iObs]->SetTitle(title.c_str());
        h1DeScale[iObs]->SetXTitle(titleX.c_str());
        setTH1_energyWidth(h1DeScale[iObs], titleOffsetX, titleOffsetY);
        h1DeScale[iObs]->SetYTitle(titleY_eRes.c_str());
        h1DeScale[iObs]->SetYTitle(Form("%s (Effective)", h1DeScale[iObs]->GetYaxis()->GetTitle()));
        if (yMax[RECOANA::kERES] > yMin[RECOANA::kERES])
            h1DeScale[iObs]->SetAxisRange(yMin[RECOANA::kERES], yMax[RECOANA::kERES], "Y");

        // width of energy scale with sigmaHM
        iObs = RECOANA::kERESHM;
        label = RECOANA::OBS_LABELS[iObs];
        h1DeScale[iObs] = (TH1D*)h1DeScale[RECOANA::kERES]->Clone(Form("h1D_%s_%s", label.c_str(), name.c_str()));
        h1DeScale[iObs]->SetTitle(title.c_str());
        h1DeScale[iObs]->SetXTitle(titleX.c_str());
        setTH1_energyWidth(h1DeScale[iObs], titleOffsetX, titleOffsetY);
        h1DeScale[iObs]->SetYTitle(titleY_eRes.c_str());
        h1DeScale[iObs]->SetYTitle(Form("%s (FWHM / 2.35)", h1DeScale[iObs]->GetYaxis()->GetTitle()));
        if (yMax[RECOANA::kERES] > yMin[RECOANA::kERES])
            h1DeScale[iObs]->SetAxisRange(yMin[RECOANA::kERES], yMax[RECOANA::kERES], "Y");

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

    updateH1Dcorr();

    calcMatchEff();
    calcFakeRatio();
    calcFakeParticleRatio();
    calcFakeParticleRatioGenPt();

    // Final histograms point to observables
    if (isValid_h2D) {
        h1D[RECOANA::kESCALE] = h1DeScale[RECOANA::kESCALE];
        h1D[RECOANA::kERES] = h1DeScale[RECOANA::kERES];
        h1D[RECOANA::kESCALEARITH] = h1DeScale[RECOANA::kESCALEARITH];
        h1D[RECOANA::kERESARITH] = h1DeScale[RECOANA::kERESARITH];
        h1D[RECOANA::kERESEFF] = h1DeScale[RECOANA::kERESEFF];
        h1D[RECOANA::kERESHM] = h1DeScale[RECOANA::kERESHM];
    }

    if (isValid_hMatchEff)  h1D[RECOANA::kEFF] = hMatchEff;
    if (isValid_hFakeRatio)  h1D[RECOANA::kFAKE] = hFakeRatio;
}

/*
 * fit distributions that compare reco-level and gen-level objects
 * Ex. fit reco pt / gen pt distribution
 */
void recoAnalyzer::fitRecoGen()
{
    // reco pt / gen pt distributions and fits
    TH1D* hTmp = 0;
    TF1* f1Tmp = 0;

    esa.clear();
    esa.resize(nBinsX);

    for (int i=1; i<=nBinsX; ++i) {

        hTmp = h1DsliceY[i-1];

        double p0 = h1DeScale[6]->GetBinContent(i);   // constant
        double p0Err = h1DeScale[6]->GetBinError(i);

        double p1 = h1DeScale[RECOANA::kESCALE]->GetBinContent(i);   // mean
        double p1Err = h1DeScale[RECOANA::kESCALE]->GetBinError(i);

        double p2 = h1DeScale[RECOANA::kERES]->GetBinContent(i);   // StdDev
        double p2Err = h1DeScale[RECOANA::kERES]->GetBinError(i);

        int binMax = hTmp->GetMaximumBin();
        int nBinsTmp = hTmp->GetNbinsX();

        // fit functions for that bin along x-axis of h2D
        std::vector<TF1*> f1sTmp;
        for (int iFnc = 0; iFnc < ESANA::kN_FNCS; ++iFnc) {

            if (ESANA::FNC_LABELS[iFnc] == "DSCB") {
                f1Tmp = new TF1(Form("f1_bin%d_fnc%d_%s", i, iFnc, name.c_str()), fnc_DSCB, 0, 1, getFncNpar(fnc_DSCB));
            }
            else {
                std::string formulaTmp = ESANA::FNC_FORMULAS[iFnc];
                f1Tmp = new TF1(Form("f1_bin%d_fnc%d_%s", i, iFnc, name.c_str()), formulaTmp.c_str());
            }

            std::vector<int> fncRange = getLeftRightBins4IntegralFraction(hTmp, binMax, ESANA::intFractions[iFnc]);
            int binLow = std::max(fncRange[0], 1);
            int binUp  = std::min(fncRange[1], nBinsTmp);
            f1Tmp->SetRange(hTmp->GetBinLowEdge(binLow), hTmp->GetBinLowEdge(binUp+1));

            f1Tmp->SetLineColor(ESANA::fncColors[iFnc]);

            if (iFnc == ESANA::kGAUS_FitSlicesY) {
                // initial fit from TH2::FitSlicesY
                f1Tmp->SetParameters(p0, p1, p2);
                double parErr[3] = {p0Err, p1Err, p2Err};
                f1Tmp->SetParErrors(parErr);
                //        double chi2ndf = h1DeScale[7]->GetBinContent(i);
                //        f1Tmp->SetChisquare(chi2ndf*100);
                //        f1Tmp->SetNDF(100);
            }
            else {
                if (ESANA::FNC_LABELS[iFnc] == ESANA::labelGaus.c_str()) {
                    // use the fit from TH2::FitSlicesY as seed
                    f1Tmp->SetParameters(hTmp->GetBinContent(binMax), p1, p2);
                }
                else if (ESANA::FNC_LABELS[iFnc] == ESANA::labelCB.c_str()) {
                    // use the fit from TH2::FitSlicesY as seed
                    f1Tmp->SetParameters(hTmp->GetBinContent(binMax), p1, p2, 1, 1);
                }
                else if (ESANA::FNC_LABELS[iFnc] == ESANA::labelDSCB.c_str()) {
                    // use the fit from TH2::FitSlicesY as seed
                    f1Tmp->SetParameters(hTmp->GetBinContent(binMax), p1, p2, 1, 1, 1, 1);
                }
            }

            f1sTmp.push_back(f1Tmp);
        }

        int nF1s = f1sTmp.size();
        std::vector<eScaleAnalyzer> esaTmp(nF1s);
        for (int j = 0; j < nF1s; ++j) {
            esaTmp[j].h = hTmp;
            esaTmp[j].isValid_h = true;
            esaTmp[j].f1 = f1sTmp[j];
            esaTmp[j].isValid_f1 = true;

            esaTmp[j].fitOption = ESANA::fitOption;
            esaTmp[j].fncLabel = ESANA::FNC_LABELS[j];
            // j = 0 corresponds to fit initial from TH2::FitSlicesY, do not refit.
            if (j > 0)  esaTmp[j].fit();

            esaTmp[j].makePull();
            std::string hpullName = replaceAll(esaTmp[j].f1->GetName(), "f1_", "hpull_");
            esaTmp[j].hPull->SetName(hpullName.c_str());

            esaTmp[j].update();
        }
        esa[i-1] = esaTmp;
    }
}

void recoAnalyzer::calcMatchEff()
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
    tmpName = getObjectName(recoAnalyzer::OBJ::kMatchEff, recoAnalyzer::TOBJ::kTGraph);
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

    tmpName = getObjectName(recoAnalyzer::OBJ::kMatchEff, recoAnalyzer::TOBJ::kTH1D);
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

void recoAnalyzer::calcFakeRatio()
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
    nameTmp = getObjectName(recoAnalyzer::OBJ::kFakeRatio, recoAnalyzer::TOBJ::kTGraph);
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

    nameTmp = getObjectName(recoAnalyzer::OBJ::kFakeRatio, recoAnalyzer::TOBJ::kTH1D);
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

void recoAnalyzer::calcFakeParticleRatio()
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

        std::string tmpHistName = getObjectName(recoAnalyzer::OBJ::kFakeOtherRatio, recoAnalyzer::TOBJ::kTH1D);
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
        std::string tmpHistName = replaceAll(hFakeParticle[i]->GetName(), getObjectStr(recoAnalyzer::OBJ::kFakeParticle).c_str(),
                                                                          getObjectStr(recoAnalyzer::OBJ::kFakeParticleRatio).c_str());
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

void recoAnalyzer::calcFakeParticleRatioGenPt()
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

        std::string tmpHistName = getObjectName(recoAnalyzer::OBJ::kFakeOtherRatioGenPt, recoAnalyzer::TOBJ::kTH1D);
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
        std::string tmpHistName = replaceAll(hFakeParticleGenPt[i]->GetName(), getObjectStr(recoAnalyzer::OBJ::kFakeParticleGenPt).c_str(),
                                                                               getObjectStr(recoAnalyzer::OBJ::kFakeParticleRatioGenPt).c_str());
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
void recoAnalyzer::writeObjects(TCanvas* c)
{
    if (isValid_h2Dcc) {
        h2Dcc->Write();
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
    std::vector<std::string> textLinesAll = getTextLinesAll();

    double legHeight = -1;
    double legWidth = -1;

    if (isValid_h2D) {
        canvasName = replaceAll(h2D->GetName(), "h2D_", "cnv2D_");
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        h2D->Draw("colz");
        h2D->Write("",TObject::kOverwrite);
        latex = new TLatex();
        setLatex(latex, "NE");
        drawTextLines(latex, c, textLinesAll, "NE", textOffsetX, textOffsetY);
        setPad4Observable((TPad*) c, 0);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.

        // energy scale
        int iObs = RECOANA::kESCALE;
        std::string label = RECOANA::OBS_LABELS[iObs];
        canvasName = Form("cnv_%s_%s", label.c_str(), name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        h1DeScale[iObs]->SetMarkerSize(markerSize);
        h1DeScale[iObs]->Draw("e");
        h1DeScale[iObs]->Write("",TObject::kOverwrite);
        latex = new TLatex();
        setLatex(latex, "NE");
        drawTextLines(latex, c, textLinesAll, "NE", textOffsetX, textOffsetY);
        setPad4Observable((TPad*) c, iObs);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.

        // width of energy scale
        iObs = RECOANA::kERES;
        label = RECOANA::OBS_LABELS[iObs];
        canvasName = Form("cnv_%s_%s", label.c_str(), name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        h1DeScale[iObs]->SetMarkerSize(markerSize);
        h1DeScale[iObs]->Draw("e");
        h1DeScale[iObs]->Write("",TObject::kOverwrite);
        latex = new TLatex();
        setLatex(latex, "NE");
        drawTextLines(latex, c, textLinesAll, "NE", textOffsetX, textOffsetY);
        setPad4Observable((TPad*) c, iObs);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.

        // energy scale from histogram mean
        iObs = RECOANA::kESCALEARITH;
        label = RECOANA::OBS_LABELS[iObs];
        canvasName = Form("cnv_%s_%s", label.c_str(), name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        h1DeScale[iObs]->SetMarkerSize(markerSize);
        h1DeScale[iObs]->Draw("e");
        h1DeScale[iObs]->Write("",TObject::kOverwrite);
        latex = new TLatex();
        setLatex(latex, "NE");
        drawTextLines(latex, c, textLinesAll, "NE", textOffsetX, textOffsetY);
        setPad4Observable((TPad*) c, iObs);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.

        // width of energy scale from histogram std dev
        iObs = RECOANA::kERESARITH;
        label = RECOANA::OBS_LABELS[iObs];
        canvasName = Form("cnv_%s_%s", label.c_str(), name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        h1DeScale[iObs]->SetMarkerSize(markerSize);
        h1DeScale[iObs]->Draw("e");
        h1DeScale[iObs]->Write("",TObject::kOverwrite);
        latex = new TLatex();
        setLatex(latex, "NE");
        drawTextLines(latex, c, textLinesAll, "NE", textOffsetX, textOffsetY);
        setPad4Observable((TPad*) c, iObs);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.

        // width of energy scale from histogram sigmaEff
        iObs = RECOANA::kERESEFF;
        label = RECOANA::OBS_LABELS[iObs];
        canvasName = Form("cnv_%s_%s", label.c_str(), name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        h1DeScale[iObs]->SetMarkerSize(markerSize);
        h1DeScale[iObs]->Draw("e");
        h1DeScale[iObs]->Write("",TObject::kOverwrite);
        latex = new TLatex();
        setLatex(latex, "NE");
        drawTextLines(latex, c, textLinesAll, "NE", textOffsetX, textOffsetY);
        setPad4Observable((TPad*) c, iObs);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.

        // width of energy scale from histogram sigmaHM
        iObs = RECOANA::kERESHM;
        label = RECOANA::OBS_LABELS[iObs];
        canvasName = Form("cnv_%s_%s", label.c_str(), name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        h1DeScale[iObs]->SetMarkerSize(markerSize);
        h1DeScale[iObs]->Draw("e");
        h1DeScale[iObs]->Write("",TObject::kOverwrite);
        latex = new TLatex();
        setLatex(latex, "NE");
        drawTextLines(latex, c, textLinesAll, "NE", textOffsetX, textOffsetY);
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
        std::vector<int> tmpIndices = {RECOANA::kESCALE, RECOANA::kESCALEARITH};
        std::string fncTitle = Form("%s Fit", ESANA::FNC_TITLES[indexFncFinal].c_str());
        std::vector<std::string> tmpLabels = {fncTitle.c_str(), "Arithmetic"};
        std::vector<int> tmpColors = {kBlack, kGreen+2};
        int nTmp = tmpIndices.size();
        for (int j = 0; j < nTmp; ++j) {
            h1DeScale[tmpIndices[j]]->SetMarkerColor(tmpColors[j]);
            h1DeScale[tmpIndices[j]]->SetLineColor(tmpColors[j]);
            if (tmpIndices[j] == RECOANA::kESCALE) {
                h1DeScale[tmpIndices[j]]->SetMarkerStyle(kFullSquare);
                h1DeScale[tmpIndices[j]]->SetMarkerSize(h1DeScale[tmpIndices[j]]->GetMarkerSize()*1.4);
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
        latex = new TLatex();
        setLatex(latex, "NE");
        drawTextLines(latex, c, textLinesAll, "NE", textOffsetX, textOffsetY);
        setPad4Observable((TPad*) c, RECOANA::kESCALE);
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
        tmpIndices = {RECOANA::kERES, RECOANA::kERESARITH, RECOANA::kERESEFF, RECOANA::kERESHM};
        tmpLabels = {fncTitle.c_str(), "Arithmetic", "Effective (#pm#sigma #rightarrow 68.3%)", "FWHM / 2.35"};
        tmpColors = {kBlack, kGreen+2, kRed+1, kBlue};
        nTmp = tmpIndices.size();
        for (int j = 0; j < nTmp; ++j) {
            h1DeScale[tmpIndices[j]]->SetMarkerColor(tmpColors[j]);
            h1DeScale[tmpIndices[j]]->SetLineColor(tmpColors[j]);
            if (tmpIndices[j] == RECOANA::kERES) {
                h1DeScale[tmpIndices[j]]->SetMarkerStyle(kFullSquare);
                h1DeScale[tmpIndices[j]]->SetMarkerSize(h1DeScale[tmpIndices[j]]->GetMarkerSize()*1.4);
            }
            else if (tmpIndices[j] == RECOANA::kERESEFF) {
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
        latex = new TLatex();
        setLatex(latex, "NE");
        drawTextLines(latex, c, textLinesAll, "NE", textOffsetX, textOffsetY);
        setPad4Observable((TPad*) c, RECOANA::kERES);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.
        if (leg != 0)  leg->Delete();

        // plot 1D reco pt / gen pt distribution for each bin along x-axis
        int nH1DsliceY = h1DsliceY.size();
        int columns = calcNcolumns(nH1DsliceY);
        int rows = calcNrows(nH1DsliceY);

        double normCanvasHeight = calcNormCanvasHeight(rows, 0.8, bottomMargin, topMargin, 0);
        double normCanvasWidth = calcNormCanvasWidth(columns, 0.8, leftMargin, rightMargin, 0);
        c = new TCanvas(Form("cnv_projY_%s", name.c_str()),"",windowWidth,windowHeight);
        setCanvasSizeMargin(c, normCanvasWidth, normCanvasHeight, leftMargin, rightMargin, bottomMargin, topMargin);
        setCanvasFinal(c);
        c->cd();

        int nPads = rows * columns;
        TPad* pads[nPads];
        divideCanvas(c, pads, rows, columns, leftMargin, rightMargin, bottomMargin, topMargin, 0, topMargin, 0.8, 0.8, 0.05);

        std::vector<std::string> linesTitlesAll;
        if (title.size() > 0) linesTitlesAll.push_back(title);
        linesTitlesAll.insert(linesTitlesAll.end(), textLinesAll.begin(), textLinesAll.end());
        std::vector<std::string> columnTitlesTmp = recoAnalyzer::splitTextLines(linesTitlesAll, columns);

        for (int i = 0; i < nH1DsliceY; ++i) {
            c->cd(i+1);

            if (i < columns) {
                h1DsliceY[i]->SetTitle(columnTitlesTmp[i].c_str());
            }
            // show title only for histograms in the 1st row
            else if (i >= columns) {
                h1DsliceY[i]->SetTitle("");
            }

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

                if (std::find(indicesFnc.begin(), indicesFnc.end(), iFnc) == indicesFnc.end()) continue;

                if (esa[i][iFnc].isValid_f1) {
                    esa[i][iFnc].f1->Draw("same");
                }
            }

            h1DsliceY[i]->Draw("e same");   // points should line above functions

            std::vector<std::string> textLinesTmp;
            std::string textLineTmp = getBinEdgeText(i+1, i+1);
            if (textLineTmp.size() > 0) textLinesTmp.push_back(textLineTmp.c_str());
            latex = new TLatex();
            setTextAlignment(latex, "NW");
            drawTextLines(latex, pads[i], textLinesTmp, "NW", 0.04, 0.1);

            // mean values and resolutions from histogram mean and stdDev
            textLinesTmp = esa[i][indexFncFinal].getTextLines4HistResult();
            latex->SetLineColor(kBlack);
            float textSizeTmp = latex->GetTextSize();
            latex->SetTextSize(textSizeTmp*0.84);
            drawTextLines(latex, pads[i], textLinesTmp, "NW", 0.04, 0.18);

            // mean values and resolutions from fit
            if (esa[i][indexFncFinal].isValid_f1) {
                textLinesTmp = esa[i][indexFncFinal].getTextLines4FitResult();
                int lineColor = esa[i][indexFncFinal].f1->GetLineColor();
                latex->SetTextColor(lineColor);
                latex->SetTextSize(textSizeTmp*0.84);
                drawTextLines(latex, pads[i], textLinesTmp, "NW", 0.04, 0.30);
            }
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

        divideCanvas(c, pads, rows, columns, leftMargin, rightMargin, bottomMargin, topMargin, 0, topMargin, 0.8, 0.8, 0.05);

        for (int i = 0; i < nH1DsliceY; ++i) {
            c->cd(i+1);

            int nFitFncs = esa[i].size();

            // the pull from iFnc = 0 is not to be shown.
            for (int iFnc = 1; iFnc < nFitFncs; ++iFnc) {

                if (std::find(indicesFnc.begin(), indicesFnc.end(), iFnc) == indicesFnc.end()) continue;

                if (esa[i][iFnc].isValid_hPull)  {

                    esa[i][iFnc].hPull->SetMarkerSize(markerSize);

                    if (iFnc != indexFncFinal)  esa[i][iFnc].hPull->SetMarkerStyle(kOpenSquare);

                    if (i < columns) {
                        esa[i][iFnc].hPull->SetTitle(columnTitlesTmp[i].c_str());
                    }
                    // show title only for histograms in the 1st row
                    else if (i >= columns) {
                        esa[i][iFnc].hPull->SetTitle("");
                    }

                    std::string drawOption = "e same";
                    if (iFnc == 1)  drawOption = "e";
                    esa[i][iFnc].hPull->Draw(drawOption.c_str());
                }
            }

            if (nFitFncs > 0 && esa[i][indexFncFinal].isValid_h) {
                float x1 = esa[i][indexFncFinal].hPull->GetXaxis()->GetXmin();
                float x2 = esa[i][indexFncFinal].hPull->GetXaxis()->GetXmax();

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
            latex = new TLatex();
            setTextAlignment(latex, "NW");
            drawTextLines(latex, pads[i], textLinesTmp, "NW", 0.04, 0.1);

            // mean values and resolutions from histogram mean and stdDev
            textLinesTmp = esa[i][indexFncFinal].getTextLines4HistResult();
            latex->SetLineColor(kBlack);
            float textSizeTmp = latex->GetTextSize();
            latex->SetTextSize(textSizeTmp*0.84);
            drawTextLines(latex, pads[i], textLinesTmp, "NW", 0.04, 0.18);

            // mean values and resolutions from fit
            if (esa[i][indexFncFinal].isValid_f1) {
                textLinesTmp = esa[i][indexFncFinal].getTextLines4FitResult();
                int lineColor = esa[i][indexFncFinal].f1->GetLineColor();
                latex->SetTextColor(lineColor);
                latex->SetTextSize(textSizeTmp*0.84);
                int nTextLinesTmp = textLinesTmp.size();
                if (nTextLinesTmp < 4)  drawTextLines(latex, pads[i], textLinesTmp, "SW", 0.04, 0.16);
                //else                    drawTextLines(latex, pads[i], textLinesTmp, "SW", 0.04, 0.30);
            }
        }
        c->Write("",TObject::kOverwrite);
        // plot pull distributions for energy scale fits - END

        for (int i = 0; i < nPads; ++i) {
            if (pads[i] != 0)  pads[i]->Delete();
        }
        c->Close();         // do not use Delete() for TCanvas.
    }

    for (int i = 0; i < RECOANA::kN_CORRS; ++i) {

        if (!isValid_h2Dcorr[i])  continue;

        // 1D histogram for correction
        canvasName = replaceAll(h1Dcorr[i]->GetName(), "h1D_", "cnv_");
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        h1Dcorr[i]->SetMinimum(0.5);
        h1Dcorr[i]->SetMaximum(1.5);
        h1Dcorr[i]->Draw("e");
        h1Dcorr[i]->Write("",TObject::kOverwrite);
        latex = new TLatex();
        setLatex(latex, "NE");
        drawTextLines(latex, c, textLinesAll, "NE", textOffsetX, textOffsetY);
        // set the pad as if it is an energy scale observable
        setPad4Observable((TPad*) c, RECOANA::kESCALE);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.

        // 1D and 2D histograms together
        canvasName = replaceAll(h2Dcorr[i]->GetName(), "h2D_", "cnv2D_");
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        h2Dcorr[i]->Draw("colz");
        h2Dcorr[i]->Write("",TObject::kOverwrite);
        h1Dcorr[i]->SetMarkerColor(kRed);
        h1Dcorr[i]->Draw("e same");
        latex = new TLatex();
        setLatex(latex, "NE");
        drawTextLines(latex, c, textLinesAll, "NE", textOffsetX, textOffsetY);
        // set the pad as if it is an energy scale observable
        setPad4Observable((TPad*) c, RECOANA::kESCALE);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.
    }

    if (isValid_h2Dcc) {
        canvasName = replaceAll(h2Dcc->GetName(), "h2Dcc_", "cnv2Dcc_");
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        h2Dcc->SetTitleOffset(titleOffsetX, "X");
        h2Dcc->SetTitleOffset(titleOffsetY, "Y");
        h2Dcc->SetStats(false);
        h2Dcc->Draw("colz");
        h2Dcc->Write("",TObject::kOverwrite);

        latex = new TLatex();
        setLatex(latex, "NW");
        drawTextLines(latex, c, textLinesAll, "NW", textOffsetX, textOffsetY);

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
        canvasName = Form("cnv_%s_%s", getObjectStr(recoAnalyzer::OBJ::kMatchNum).c_str(), name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        hMatchNum->SetMarkerSize(markerSize);
        hMatchNum->Draw("e");
        hMatchNum->Write("",TObject::kOverwrite);

        latex = new TLatex();
        setLatex(latex, "NE");
        drawTextLines(latex, c, textLinesAll, "NE", textOffsetX, textOffsetY);

        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.
    }
    if (isValid_hMatchDenom) {
        canvasName = Form("cnv_%s_%s", getObjectStr(recoAnalyzer::OBJ::kMatchDenom).c_str(), name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        hMatchDenom->SetMarkerSize(markerSize);
        hMatchDenom->Draw("e");
        hMatchDenom->Write("",TObject::kOverwrite);

        latex = new TLatex();
        setLatex(latex, "NE");
        drawTextLines(latex, c, textLinesAll, "NE", textOffsetX, textOffsetY);

        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.
    }
    if (isValid_hMatchEff) {
        int iObs = RECOANA::kEFF;
        canvasName = Form("cnv_%sH1D_%s", RECOANA::OBS_LABELS[iObs].c_str() , name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        hMatchEff->SetMarkerSize(markerSize);
        hMatchEff->Draw("e");
        hMatchEff->Write("",TObject::kOverwrite);

        latex = new TLatex();
        setLatex(latex, "NE");
        drawTextLines(latex, c, textLinesAll, "NE", textOffsetX, textOffsetY);

        setPad4Observable((TPad*) c, iObs);
        drawLine4PtRange((TPad*) c);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.
    }
    if (isValid_hMatchEff && isValid_gMatchEff) {
        int iObs = RECOANA::kEFF;
        canvasName = Form("cnv_%s_%s", RECOANA::OBS_LABELS[iObs].c_str() , name.c_str());
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

        latex = new TLatex();
        setLatex(latex, "NE");
        drawTextLines(latex, c, textLinesAll, "NE", textOffsetX, textOffsetY);

        setPad4Observable((TPad*) c, iObs);
        drawLine4PtRange((TPad*) c);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.
        hTmp->Delete();
    }

    // fake rate objects
    if (isValid_hFakeNum) {
        canvasName = Form("cnv_%s_%s", getObjectStr(recoAnalyzer::OBJ::kFakeNum).c_str(), name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        hFakeNum->SetMarkerSize(markerSize);
        hFakeNum->Draw("e");
        hFakeNum->Write("",TObject::kOverwrite);

        latex = new TLatex();
        setLatex(latex, "NE");
        drawTextLines(latex, c, textLinesAll, "NE", textOffsetX, textOffsetY);

        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.
    }
    if (isValid_hFakeDenom) {
        canvasName = Form("cnv_%s_%s", getObjectStr(recoAnalyzer::OBJ::kFakeDenom).c_str(), name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        hFakeDenom->SetMarkerSize(markerSize);
        hFakeDenom->Draw("e");
        hFakeDenom->Write("",TObject::kOverwrite);

        latex = new TLatex();
        setLatex(latex, "NE");
        drawTextLines(latex, c, textLinesAll, "NE", textOffsetX, textOffsetY);

        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.
    }
    if (isValid_hFakeRatio) {
        int iObs = RECOANA::kFAKE;
        canvasName = Form("cnv_%s_%s", RECOANA::OBS_LABELS[iObs].c_str() , name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        hFakeRatio->SetMarkerSize(markerSize);
        hFakeRatio->Draw("e");
        hFakeRatio->Write("",TObject::kOverwrite);

        latex = new TLatex();
        setLatex(latex, "NE");
        drawTextLines(latex, c, textLinesAll, "NE", textOffsetX, textOffsetY);

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
        int iObs = RECOANA::kFAKE;
        canvasName = Form("cnv_%sgraph_%s", RECOANA::OBS_LABELS[iObs].c_str() , name.c_str());
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

        latex = new TLatex();
        setLatex(latex, "NE");
        drawTextLines(latex, c, textLinesAll, "NE", textOffsetX, textOffsetY);

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

        int iObs = RECOANA::kFAKE;
        canvasName = Form("cnv_%sPDGs_%s", RECOANA::OBS_LABELS[iObs].c_str() , name.c_str());
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

            int markerColor = RECOANA::particlesColor[fakeIndices[i]];
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

        latex = new TLatex();
        setLatex(latex, "NE");
        drawTextLines(latex, c, textLinesAll, "NE", textOffsetX, textOffsetY);

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

         int iObs = RECOANA::kFAKE;
         canvasName = Form("cnv_%sGenPtPDGs_%s", RECOANA::OBS_LABELS[iObs].c_str() , name.c_str());
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

             int markerColor = RECOANA::particlesColor[fakeIndices[i]];
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

         latex = new TLatex();
         setLatex(latex, "NE");
         drawTextLines(latex, c, textLinesAll, "NE", textOffsetX, textOffsetY);

         setPad4Observable((TPad*) c, iObs);
         drawLine4PtRange((TPad*) c);
         setCanvasFinal(c);
         c->Write("",TObject::kOverwrite);
         c->Close();         // do not use Delete() for TCanvas.
         if (leg != 0)  leg->Delete();
         if (hTmp != 0)  hTmp->Delete();
     }

     if (line != 0)  line->Delete();
     latex->Delete();
}

void recoAnalyzer::setPad4Observable(TPad* p, int iObs, int iDep)
{
    TLine* line = 0;

    p->Update();
    if (iObs == RECOANA::kESCALE || iObs == RECOANA::kESCALEARITH ||
        iObs == RECOANA::kEFF || iObs == RECOANA::kFAKE) {

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

    if (iObs == RECOANA::kESCALE || iObs == RECOANA::kESCALEARITH) {

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

    if (iObs == RECOANA::kERES || iObs == RECOANA::kERESARITH ||
        iObs == RECOANA::kERESEFF || iObs == RECOANA::kERESHM) {

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

    if (iDep == RECOANA::kETA) {
        // draw line for EE-EB transition
        double ECAL_boundary_1 = 1.4442;
        double ECAL_boundary_2 = 1.566;

        double yMin = p->GetUymin();
        double yMax = p->GetUymax();
        if (iObs == RECOANA::kEFF || iObs == RECOANA::kFAKE)  yMax = 1;

        // draw lines for ECAL transition region
        std::vector<double> lineXvalues {-1*ECAL_boundary_1, ECAL_boundary_1, -1*ECAL_boundary_2, ECAL_boundary_2};
        for (std::vector<double>::const_iterator itLine = lineXvalues.begin(); itLine !=lineXvalues.end(); ++itLine) {

            line = new TLine((*itLine), yMin, (*itLine), yMax);
            line->SetLineStyle(kDashed);
            line->Draw();
        }
    }
}

void recoAnalyzer::setPad4Observable(TPad* p, int iObs)
{
    setPad4Observable(p, iObs, dep);
}

void recoAnalyzer::setLatex(TLatex* latex, std::string position)
{
    setTextAlignment(latex, position.c_str());
    latex->SetTextFont(textFont);
    latex->SetTextSize(textSize);
}

/*
 * draw vertical lines for the pt range
 * Ex. x-axis is reco pt and the gen Pt range is 10<pt<30, then it draws vertical lines at x=10 and x=30.
 * Ex. x-axis is gen pt and the reco Pt range is pt>20, then it draws vertical line at x=20.
 */
void recoAnalyzer::drawLine4PtRange(TPad* p, int lineColor)
{
    TLine* line = 0;

    p->Update();
    double x1 = p->GetUxmin();
    double x2 = p->GetUxmax();
    double y1 = p->GetUymin();
    double y2 = p->GetUymax();
    if (y2 > 1)  y2 = 1;        // vertical line rises to at most y = 1

    if (dep == RECOANA::kGENPT) {

        // vertical lines for reco pt range
        if (ranges[RECOANA::rRECOPT][0] > 0 &&
            ranges[RECOANA::rRECOPT][0] > x1 && ranges[RECOANA::rRECOPT][0] < x2) {

            line = new TLine(ranges[RECOANA::rRECOPT][0], y1, ranges[RECOANA::rRECOPT][0], y2);
            line->SetLineStyle(kDotted);
            line->SetLineColor(lineColor);
            line->SetLineWidth(line->GetLineWidth()*3);
            line->Draw();
        }
        if (ranges[RECOANA::rRECOPT][1] > 0 &&
            ranges[RECOANA::rRECOPT][1] > x1 && ranges[RECOANA::rRECOPT][1] < x2) {

            line = new TLine(ranges[RECOANA::rRECOPT][1], y1, ranges[RECOANA::rRECOPT][1], y2);
            line->SetLineStyle(kDotted);
            line->SetLineColor(lineColor);
            line->SetLineWidth(line->GetLineWidth()*3);
            line->Draw();
        }
    }
    else if (dep == RECOANA::kRECOPT) {

        // vertical lines for gen pt range
        if (ranges[RECOANA::rGENPT][0] > 0 &&
            ranges[RECOANA::rGENPT][0] > x1 && ranges[RECOANA::rGENPT][0] < x2) {

            line = new TLine(ranges[RECOANA::rGENPT][0], y1, ranges[RECOANA::rGENPT][0], y2);
            line->SetLineStyle(kDotted);
            line->SetLineColor(lineColor);
            line->SetLineWidth(line->GetLineWidth()*3);
            line->Draw();
        }
        if (ranges[RECOANA::rGENPT][1] > 0 &&
            ranges[RECOANA::rGENPT][1] > x1 && ranges[RECOANA::rGENPT][1] < x2) {

            line = new TLine(ranges[RECOANA::rGENPT][1], y1, ranges[RECOANA::rGENPT][1], y2);
            line->SetLineStyle(kDotted);
            line->SetLineColor(lineColor);
            line->SetLineWidth(line->GetLineWidth()*3);
            line->Draw();
        }
    }
}

#endif
