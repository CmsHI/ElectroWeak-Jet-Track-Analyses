/*
 * general purpose class for photon analysis
 */

#ifndef PERFORMANCE_PHOTONS_PHOTONANALYZER_H_
#define PERFORMANCE_PHOTONS_PHOTONANALYZER_H_

#include <TH1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TMath.h>
#include <TPad.h>
#include <TCanvas.h>

#include <string>

#include "../../TreeHeaders/ggHiNtuplizerTree.h"
#include "../../Utilities/th1Util.h"

namespace PHOTONANA {

// observables of interest
enum DIST {
    kPt,
    kEta,
    kPhi,
    kHoverE,
    kSumIso,
    kEcalIso,
    kHcalIso,
    kTrkIso,
    kSieie,
    kR9,
    kN_DIST
};

const float DIST_AXIS[kN_DIST][3] = {
  {60, 0, 150},
  {40, -3.0, 3.0},
  {40, -3.1415, 3.1415},
  {40, 0, 1},
  {40, -20, 20},
  {40, -20, 20},
  {40, -20, 20},
  {40, -20, 20},
  {40,  0, 0.05},
  {40,  0, 1}
};

const std::string DIST_LABELS[kN_DIST] = {
        "pt",
        "eta",
        "phi",
        "hovere",
        "sumIso",
        "ecaliso",
        "hcaliso",
        "trkiso",
        "sieie",
        "r9"
};

const std::string DIST_TITLES[kN_DIST] = {
        "p^{#gamma}_{T}",
        "#eta^{#gamma}",
        "#phi^{#gamma}",
        "H/E",
        "sumIso",
        "ecalIso",
        "hcalIso",
        "trkIso",
        "#sigma_{#eta#eta}",
        "R9"
};

// photon selections
enum SEL {
    kSpikeRej,
    kIsoHoverE,
    kIso,
    kSig,
    kBkg,
    kNoSel,
    kN_SEL
};

const std::string SEL_LABELS[SEL::kN_SEL] = {
        "spikeRej",
        "hovere",
        "iso",
        "sig",
        "bkg",
        "noSel"
};

const std::string SEL_TITLES[SEL::kN_SEL] = {
        "spike rejection",
        "H/E",
        "sumIso",
        "signal",
        "bkg",
        "no selection"
};

// select multiplicity per event
enum MULT {
    kLead,
    kSubLead,
    kIncl,
    kN_MULT
};

const std::string SELMULT_LABELS[MULT::kN_MULT] = {
        "lead",
        "sublead",
        "incl"
};

const std::string SELMULT_TITLES[MULT::kN_MULT] = {
        "leading",
        "subleading",
        "inclusive"
};

enum BINS {
    kETA,
    kPT,
    kPTGEN,
    kHIBIN,
    kN_BINS
};

const std::string BIN_LABELS[kN_BINS] = {
        "ETA",
        "PT",
        "PTGEN",
        "HIBIN"
};

enum BINS_ETA {
    kEB,
    kEE,        //  1.48 < |eta| < 2.4 full ECAL Endcap
    kEE1,       //  1.48 < |eta| < 2.0
    kEE2,       //  2.0  < |eta| < 2.4
    kINCL,
    kN_BINS_ETA
};

const float BINS_ETA_MINMAX[kN_BINS_ETA][2] = {
  {-1, 1.44},
  {1.56, 2.4},
  {1.56, 2.0},
  {2.0, 2.4},
  {-1, 2.4}
};

const std::string BINS_ETA_LABELS[kN_BINS_ETA] = {
    "etaEB",
    "etaEE",
    "etaEE1",
    "etaEE2",
    "etaIncl"
};

const std::string BINS_ETA_TITLE[kN_DIST] = {
        "|#eta^{#gamma}| < 1.44",
        "1.56 < |#eta^{#gamma}| < 2.4",
        "1.56 < |#eta^{#gamma}| < 2.0",
        "2.0  < |#eta^{#gamma}| < 2.4",
        "|#eta^{#gamma}| < 2.4"
};

enum TH1NORM {
    kRaw,
    kNorm,
    kNormEvent,
    kN_NORM
};

const std::string TH1NORM_LABELS[kN_NORM] = {
    "",
    "norm",
    "normEvent"
};

};

class photonAnalyzer {
public :
    photonAnalyzer(){

        selectionIndex = -1;
        for (int i=0; i<PHOTONANA::kN_BINS; ++i) {
            ranges[i][0] = 0;
            ranges[i][1] = -1;  // no upper bound
        }

        name = "";
        title = "";
        titleX = "";
        titleOffsetX = 1;
        titleOffsetY = 1;

        xMin = 0;
        xMax = -1;
        yMin = 0;
        yMax = -1;

        h2Dinitialized = false;
        hInitialized = false;
        h2DcorrInitialized = false;
        hFilled = false;
        hNevents = 0;
    };
    ~photonAnalyzer(){};

    static std::vector<double> getPhoVars(ggHiNtuplizer ggHi, int i);

    bool insideRange(double val, int iBin);
    bool insidePtRange(double val);
    bool insideEtaRange(double val);
    bool insideHiBinRange(double val);

    void setRange(float min, float max, int iBin);
    void setRangeEta(float min = 0, float max = -1);
    void setRangePt(float min = 0, float max = -1);
    void setRangePtGen(float min = 0, float max = -1);
    void setRangeHiBin(float min = 0, float max = -1);

    std::string getSelectionText();
    std::string getRangeTextEta();
    std::string getRangeTextPt();
    std::string getRangeTextPtGen();
    std::string getRangeTextHiBin();

    static std::string findEtaLabel(float min, float max);
    TH1D* getTH1(int iNormIndex);

    void prepareTitle();

    void postLoop();
    void writeObjects(TCanvas* c);

    TH2D* h2D;

    TH1D* h;            // observable distribution
    TH1D* h_norm;       // observable distribution normalized to 1
    TH1D* h_normEvent;  // observable distribution normalized by number of events
    TH2D* h2Dcorr;        // reco pt vs. observable correlation histogram.

    bool h2Dinitialized;
    bool hInitialized;
    bool h2DcorrInitialized;
    bool hFilled;    // boolean whether the variable is filled in that event
    int  hNevents;   // number of events that h is filled.

    std::string name;   // this is basically histogram name excluding the "h1D"/"h2D" prefix
    std::string title;
    std::string titleX;
    float titleOffsetX;
    float titleOffsetY;

    double xMin;
    double xMax;
    double yMin;
    double yMax;

    int selectionIndex;
    float ranges[PHOTONANA::kN_BINS][2];
};

/*
 * read photon variables that will fill distributions
 * Assume phoVars.size() = PHOTONANA::DIST::kN_DIST
 */
std::vector<double> photonAnalyzer::getPhoVars(ggHiNtuplizer ggHi, int i)
{
    std::vector<double> phoVars = {
        (*ggHi.phoEt)[i],
        (*ggHi.phoEta)[i],
        (*ggHi.phoPhi)[i],
        (*ggHi.phoHoverE)[i],
        ((*ggHi.pho_ecalClusterIsoR4)[i] + (*ggHi.pho_hcalRechitIsoR4)[i] + (*ggHi.pho_trackIsoR4PtCut20)[i]),
        (*ggHi.pho_ecalClusterIsoR4)[i],
        (*ggHi.pho_hcalRechitIsoR4)[i],
        (*ggHi.pho_trackIsoR4PtCut20)[i],
        (*ggHi.phoSigmaIEtaIEta_2012)[i],
        (*ggHi.phoR9)[i]
    };

    return phoVars;
}

/*
 * check if the given variables lie inside the kinematic range defined for this object.
 */
bool photonAnalyzer::insideRange(double val, int iBin)
{
    if(ranges[iBin][0] <= 0 || ranges[iBin][0] <= val) {
        if(ranges[iBin][1] == -1 || ranges[iBin][1] > val) {
            return true;
        }
    }

    return false;
}

/*
 * check if the given eta lies inside the given eta bin
 */
bool photonAnalyzer::insidePtRange(double val)
{
    return insideRange(val, PHOTONANA::BINS::kPT);
}

/*
 * check if the given eta lies inside the given eta bin
 */
bool photonAnalyzer::insideEtaRange(double val)
{
    return insideRange(val, PHOTONANA::BINS::kETA);
}

/*
 * check if the given eta lies inside the given eta bin
 */
bool photonAnalyzer::insideHiBinRange(double val)
{
    return insideRange(val, PHOTONANA::BINS::kHIBIN);
}


void photonAnalyzer::setRange(float min, float max, int iBin)
{
    ranges[iBin][0] = min;
    ranges[iBin][1] = max;
}

void photonAnalyzer::setRangeEta(float min, float max)
{
    setRange(min, max, PHOTONANA::BINS::kETA);
}

void photonAnalyzer::setRangePt(float min, float max)
{
    setRange(min, max, PHOTONANA::BINS::kPT);
}

void photonAnalyzer::setRangePtGen(float min, float max)
{
    setRange(min, max, PHOTONANA::BINS::kPTGEN);
}

void photonAnalyzer::setRangeHiBin(float min, float max)
{
    setRange(min, max, PHOTONANA::BINS::kHIBIN);
}

std::string photonAnalyzer::getSelectionText()
{
    std::string res = "";

    if (selectionIndex >= 0 && selectionIndex < PHOTONANA::SEL::kN_SEL)
        res = PHOTONANA::SEL_TITLES[selectionIndex];

    return res;
}

std::string photonAnalyzer::getRangeTextEta()
{
    std::string res = "";

    if (ranges[PHOTONANA::BINS::kETA][0] <= 0 && ranges[PHOTONANA::BINS::kETA][1] > 0)
        res  = Form("|#eta|<%.2f", ranges[PHOTONANA::BINS::kETA][1]);
    else if (ranges[PHOTONANA::BINS::kETA][0] > 0 && ranges[PHOTONANA::BINS::kETA][1] > 0)
        res  = Form("%.2f<|#eta|<%.2f", ranges[PHOTONANA::BINS::kETA][0], ranges[PHOTONANA::BINS::kETA][1]);

    return res;
}

std::string photonAnalyzer::getRangeTextPt()
{
    std::string res = "";

    if (ranges[PHOTONANA::BINS::kPT][0] > 0 && ranges[PHOTONANA::BINS::kPT][1] <= -1)
        res  = Form("p_{T}>%.0f", ranges[PHOTONANA::BINS::kPT][0]);
    else if (ranges[PHOTONANA::BINS::kPT][0] > 0 && ranges[PHOTONANA::BINS::kPT][1] > 0)
        res = Form("%.0f<p_{T}<%.0f", ranges[PHOTONANA::BINS::kPT][0], ranges[PHOTONANA::BINS::kPT][1]);
    else if (ranges[PHOTONANA::BINS::kPT][0] <= 0 && ranges[PHOTONANA::BINS::kPT][1] > 0)
        res = Form("p_{T}<%.0f", ranges[PHOTONANA::BINS::kPT][1]);

    return res;
}

std::string photonAnalyzer::getRangeTextPtGen()
{
    std::string res = "";

    if (ranges[PHOTONANA::BINS::kPTGEN][0] > 0 && ranges[PHOTONANA::BINS::kPTGEN][1] <= -1)
        res  = Form("p_{T}^{gen}>%.0f", ranges[PHOTONANA::BINS::kPTGEN][0]);
    else if (ranges[PHOTONANA::BINS::kPTGEN][0] > 0 && ranges[PHOTONANA::BINS::kPTGEN][1] > 0)
        res  = Form("%.0f<p_{T}^{gen}<%.0f", ranges[PHOTONANA::BINS::kPTGEN][0], ranges[PHOTONANA::BINS::kPTGEN][1]);
    else if (ranges[PHOTONANA::BINS::kPTGEN][0] <= 0 && ranges[PHOTONANA::BINS::kPTGEN][1] > 0)
        res  = Form("p_{T}^{gen}<%.0f", ranges[PHOTONANA::BINS::kPTGEN][1]);

    return res;
}

std::string photonAnalyzer::getRangeTextHiBin()
{
    std::string res = "";

    if (ranges[PHOTONANA::BINS::kHIBIN][0] >= 0 && ranges[PHOTONANA::BINS::kHIBIN][1] > 0)
        res = Form("Cent:%.0f-%.0f%%", ranges[PHOTONANA::BINS::kHIBIN][0]/2, ranges[PHOTONANA::BINS::kHIBIN][1]/2);

    return res;
}

/*
 * find the eta label that matches the given eta range best.
 */
std::string photonAnalyzer::findEtaLabel(float minEta, float maxEta)
{
    std::string res = "";

    int eb = PHOTONANA::BINS_ETA::kEB;
    int ee = PHOTONANA::BINS_ETA::kEE;
    int ee1 = PHOTONANA::BINS_ETA::kEE1;
    int ee2 = PHOTONANA::BINS_ETA::kEE2;
    int incl = PHOTONANA::BINS_ETA::kINCL;

    if (maxEta <= 0 ||
            (minEta <= 0 && maxEta <= PHOTONANA::BINS_ETA_MINMAX[incl][1] + 0.15)) res = PHOTONANA::BINS_ETA_LABELS[incl];
    else if (maxEta < PHOTONANA::BINS_ETA_MINMAX[eb][1])  res = PHOTONANA::BINS_ETA_LABELS[eb];
    else if (minEta >= PHOTONANA::BINS_ETA_MINMAX[ee1][0] - 0.15 && maxEta <= PHOTONANA::BINS_ETA_MINMAX[ee1][1] + 0.15)
        res = PHOTONANA::BINS_ETA_LABELS[ee1];
    else if (minEta >= PHOTONANA::BINS_ETA_MINMAX[ee2][0] - 0.15 && maxEta <= PHOTONANA::BINS_ETA_MINMAX[ee2][1] + 0.15)
        res = PHOTONANA::BINS_ETA_LABELS[ee2];
    else if (minEta >= PHOTONANA::BINS_ETA_MINMAX[ee][0] - 0.15)
        res = PHOTONANA::BINS_ETA_LABELS[ee];

    return res;
}

/*
 * return the histogram that corresponds to given normalization index
 */
TH1D* photonAnalyzer::getTH1(int iNormIndex)
{
    if (iNormIndex == PHOTONANA::TH1NORM::kRaw)  return h;
    else if (iNormIndex == PHOTONANA::TH1NORM::kNorm)  return h_norm;
    else if (iNormIndex == PHOTONANA::TH1NORM::kNormEvent)  return h_normEvent;
    else return 0;
}


/*
 * prepare the object title using the given ranges
 */
void photonAnalyzer::prepareTitle()
{
    // prepare histogram title
    std::string selectionStr = getSelectionText();
    std::string etaStr = getRangeTextEta();
    std::string ptStr = getRangeTextPt();
    std::string ptGenStr = getRangeTextPtGen();
    std::string hiBinStr = getRangeTextHiBin();

    std::string tmpHistTitle = "";
    if (selectionStr.size() > 0)  tmpHistTitle.append(Form("%s", selectionStr.c_str()));
    if (etaStr.size() > 0)  tmpHistTitle.append(Form(", %s", etaStr.c_str()));
    if (ptGenStr.size() > 0)  tmpHistTitle.append(Form(", %s", ptGenStr.c_str()));
    if (ptStr.size() > 0) tmpHistTitle.append(Form(", %s", ptStr.c_str()));
    if (hiBinStr.size() > 0)  tmpHistTitle.append(Form(", %s", hiBinStr.c_str()));

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

void photonAnalyzer::postLoop()
{
    if (hInitialized) {
        h->SetMarkerStyle(kFullCircle);

        h_norm = (TH1D*)h->Clone(Form("h_%s_%s", PHOTONANA::TH1NORM_LABELS[PHOTONANA::TH1NORM::kNorm].c_str(),
                                                 name.c_str()));
        h_norm->SetYTitle(Form("#frac{1}{N} #frac{dN}{d%s}", titleX.c_str()));
        h_norm->Scale(1./h->Integral());

        h_normEvent = (TH1D*)h->Clone(Form("h_%s_%s", PHOTONANA::TH1NORM_LABELS[PHOTONANA::TH1NORM::kNormEvent].c_str(),
                                                      name.c_str()));
        h_normEvent->SetYTitle(Form("#frac{1}{N_{Event}} #frac{dN}{d%s}", titleX.c_str()));
        h_normEvent->Scale(1./(double)hNevents);
    }
}

/*
 * use "c" as a template
 */
void photonAnalyzer::writeObjects(TCanvas* c)
{
    // write histograms with a particular dependence
    if (hInitialized) {
        h->SetMarkerStyle(kFullCircle);

        // extract information from "c"
        int windowWidth = c->GetWw();
        int windowHeight = c->GetWh();
        double leftMargin = c->GetLeftMargin();
        double rightMargin = c->GetRightMargin();
        double bottomMargin = c->GetBottomMargin();
        double topMargin = c->GetTopMargin();
        int logX = c->GetLogx();
        int logY = c->GetLogy();
        int logZ = c->GetLogz();

        std::string canvasName = "";

        // unnormalized distribution
        canvasName = Form("cnv_%s", name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        h->SetStats(false);
        h->Draw("e");
        h->Write("",TObject::kOverwrite);
        setCanvasFinal(c, logX, logY, logZ);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.

        // unity normalized distribution
        canvasName = Form("cnv_%s_%s", PHOTONANA::TH1NORM_LABELS[PHOTONANA::TH1NORM::kNorm].c_str(),
                                       name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        h_norm->SetStats(false);
        h_norm->Draw("e");
        h_norm->Write("",TObject::kOverwrite);
        setCanvasFinal(c, logX, logY, logZ);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.

        // distribution normalized by number of events
        canvasName = Form("cnv_%s_%s", PHOTONANA::TH1NORM_LABELS[PHOTONANA::TH1NORM::kNormEvent].c_str(),
                                       name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        h_normEvent->SetStats(false);
        h_normEvent->Draw("e");
        h_normEvent->Write("",TObject::kOverwrite);
        setCanvasFinal(c, logX, logY, logZ);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.
    }
}

#endif
