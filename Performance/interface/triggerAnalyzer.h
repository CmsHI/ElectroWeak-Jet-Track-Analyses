/*
 * general purpose class for trigger performance analysis
 */

#ifndef PERFORMANCE_INTERFACE_TRIGGERANALYZER_H_
#define PERFORMANCE_INTERFACE_TRIGGERANALYZER_H_

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
#include "../../Utilities/mathUtil.h"
#include "../../Utilities/styleUtil.h"
#include "../../Utilities/systemUtil.h"

namespace TRIGGERANA {

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

enum DEPS {   // trigger performance dependencies
    kETA,
    kRECOPT,
    kCENT,
    kSUMISO,
    kECALISO,
    kHCALISO,
    kTRKISO,
    kSIEIE,
    kN_DEPS
};

const std::string DEP_LABELS[kN_DEPS] = {
        "ETA",
        "RECOPT",
        "CENT",
        "SUMISO",
        "ECALISO",
        "HCALISO",
        "TRKISO",
        "SIEIE"
};

enum RANGES {   // trigger performance ranges/cuts
    rETA,
    rRECOPT,
    rCENT,
    rSUMISO,
    rSIEIE,
    rR9,
    kN_RANGES
};

const std::string RANGE_LABELS[kN_RANGES] = {
        "ETA",
        "RECOPT",
        "CENT",
        "SUMISO",
        "SIEIE",
        "R9"
};

// observables
enum OBS {
    kEFF,        // trigger efficiency
    kINEFF,      // trigger inefficiency
    kFAKE,       // trigger fake is a trigger fire without a matching offline object
    kN_OBS
};

const std::string OBS_LABELS[kN_OBS] = {"eff", "ineff", "fake"};

};

class triggerAnalyzer {
public :
    triggerAnalyzer(){
        nBinsX = 0;

        hNum = 0;
        hDenom = 0;
        hEff = 0;
        gEff = 0;
        isValid_hNum = false;
        isValid_hDenom = false;
        isValid_hEff = false;
        isValid_gEff = false;

        h2Num = 0;
        h2Denom = 0;
        h2Eff = 0;
        isValid_h2Num = false;
        isValid_h2Denom = false;
        isValid_h2Eff = false;

        hNumInEff = 0;
        hInEff = 0;
        gInEff = 0;
        isValid_hNumInEff = false;
        isValid_hInEff = false;
        isValid_gInEff = false;

        hFakeNum = 0;
        hFakeDenom = 0;
        hFakeRatio = 0;
        gFakeRatio = 0;
        isValid_hFakeNum = false;
        isValid_hFakeDenom = false;
        isValid_hFakeRatio = false;
        isValid_gFakeRatio = false;

        pathNum = "";
        pathDenom = "";
        pathNumText = "";
        pathDenomText = "";
        ptTreshold = 0;

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

        for (int i=0; i<TRIGGERANA::kN_RANGES; ++i) {
            ranges[i][0] = 0;
            ranges[i][1] = -1;  // no upper bound
        }
    };
    ~triggerAnalyzer(){};

    void FillHNum(double x, double w, std::vector<double> vars);
    void FillHDenom(double x, double w, std::vector<double> vars);
    void FillH2Num(double x, double y, double w, std::vector<double> vars);
    void FillH2Denom(double x, double y, double w, std::vector<double> vars);
    void FillHNumInEff(double x, double w, std::vector<double> vars);
    void FillHNumFake(double x, double w, std::vector<double> vars);
    void FillHDenomFake(double x, double w, std::vector<double> vars);

    std::string getPathNum() { return pathNum; };
    std::string getPathDenom() { return pathDenom; };
    std::string getPathNumText() { return pathNumText; };
    std::string getPathDenomText() { return pathDenomText; };

    bool insideRange(std::vector<double> vars);

    std::string getRangeText(int iRange);

    bool isValid();
    void update();
    void updateTH1();

    static std::string getObjectStr(int iObj);
    static std::string getTObjectStr(int iTObj);
    std::string getObjectName(int iObj, int iTObj = 0);

    void prepareTitleRanges();
    void prepareTextLinesRanges();
    std::string getTitleAll();
    std::vector<std::string> getTextLinesAll();
    static std::vector<std::string> splitTextLines(std::vector<std::string> textLines, int nColumns);

    void postLoop();
    void calcEff();
    void calcEff2D();
    void calcInEff();
    void calcFakeRatio();
    void writeObjects(TCanvas* c);

    static void setPad4Observable(TPad* p, int iObs, int iDep);
    void setPad4Observable(TPad* p, int iObs);
    void setLatex(TLatex* latex, std::string position);
    void drawLine4PtThreshold(TPad* p, int lineColor = kBlack);

    std::string getTriggerPathText(std::string triggerPath);
    static std::string getHLTObjectName(std::string triggerName);
    double extractPtThreshold(std::string triggerName);
    double extractEtaThreshold(std::string triggerName);
    double getOffset4PtThreshold(double ptThreshold);

    static int setBranchAdressTrigger(TTree* tree, std::string branch, int & branchVal);
    static void setBranchesTrigger(TTree* tree, std::vector<std::string> branchNames, int val[], int nVal);

    int nBinsX;

    // objects for efficiency
    TH1D* hNum;
    TH1D* hDenom;
    TH1D* hEff;
    TGraphAsymmErrors* gEff;
    bool isValid_hNum;
    bool isValid_hDenom;
    bool isValid_hEff;
    bool isValid_gEff;

    TH2D* h2Num;
    TH2D* h2Denom;
    TH2D* h2Eff;
    bool isValid_h2Num;
    bool isValid_h2Denom;
    bool isValid_h2Eff;

    // objects for inefficiency
    TH1D* hNumInEff;
    TH1D* hInEff;
    TGraphAsymmErrors* gInEff;
    bool isValid_hNumInEff;
    bool isValid_hInEff;
    bool isValid_gInEff;

    // objects for fake rate
    TH1D* hFakeNum;
    TH1D* hFakeDenom;
    TH1D* hFakeRatio;
    TGraphAsymmErrors* gFakeRatio;
    bool isValid_hFakeNum;
    bool isValid_hFakeDenom;
    bool isValid_hFakeRatio;
    bool isValid_gFakeRatio;

    std::string pathNum;
    std::string pathDenom;
    std::string pathNumText;
    std::string pathDenomText;
    double ptTreshold;

    enum OBJ{
        kNum,
        kDenom,
        kEff,
        kNumInEff,
        kInEff,
        kFakeNum,
        kFakeDenom,
        kFakeRatio,
        kN_OBJ
    };

    enum TOBJ{
        kTH1D,
        kTH2D,
        kTGraph,
        kN_TOBJ
    };

    int recoObj;
    int dep;            // If the x-axis is eta, then dep = TRIGGERANA::kETA

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
    double ranges[TRIGGERANA::kN_RANGES][2];
};

void triggerAnalyzer::FillHNum(double x, double w, std::vector<double> vars)
{
    if (isValid_hNum && insideRange(vars))
        hNum->Fill(x, w);
}

void triggerAnalyzer::FillHDenom(double x, double w, std::vector<double> vars)
{
    if (isValid_hDenom && insideRange(vars))
        hDenom->Fill(x, w);
}

void triggerAnalyzer::FillH2Num(double x, double y, double w, std::vector<double> vars)
{
    if (isValid_h2Num && insideRange(vars))
        h2Num->Fill(x, y, w);
}

void triggerAnalyzer::FillH2Denom(double x, double y, double w, std::vector<double> vars)
{
    if (isValid_h2Denom && insideRange(vars))
        h2Denom->Fill(x, y, w);
}

void triggerAnalyzer::FillHNumInEff(double x, double w, std::vector<double> vars)
{
    if (isValid_hNumInEff && insideRange(vars))
        hNumInEff->Fill(x, w);
}

void triggerAnalyzer::FillHNumFake(double x, double w, std::vector<double> vars)
{
    vars[TRIGGERANA::rRECOPT] = -1;

    if (isValid_hFakeNum && insideRange(vars)) {
        hFakeNum->Fill(x, w);
    }
}

void triggerAnalyzer::FillHDenomFake(double x, double w, std::vector<double> vars)
{
    vars[TRIGGERANA::rRECOPT] = -1;

    if (isValid_hFakeDenom && insideRange(vars))
        hFakeDenom->Fill(x, w);
}

/*
 * check if the given variables lie inside the kinematic range defined for this object.
 */
bool triggerAnalyzer::insideRange(std::vector<double> vars)
{
    double eta = vars[TRIGGERANA::rETA];
    double recoPt = vars[TRIGGERANA::rRECOPT];
    double cent = vars[TRIGGERANA::rCENT];

    // make sure to pass the selection if no explicit kinematic range is specified.
    if (eta == -999)  eta = ranges[TRIGGERANA::rETA][0];
    if (recoPt == -1)  recoPt = ranges[TRIGGERANA::rRECOPT][0];
    if (cent == -1)  cent = ranges[TRIGGERANA::rCENT][0];

    if (recoObj == TRIGGERANA::OBJS::kPHOTON) {
        double sumIso = vars[TRIGGERANA::rSUMISO];
        double sieie = vars[TRIGGERANA::rSIEIE];
        double r9 = vars[TRIGGERANA::rR9];

        if (sumIso == -999)  sumIso = ranges[TRIGGERANA::rSUMISO][0];
        if (sieie == -1)  sieie = ranges[TRIGGERANA::rSIEIE][0];
        if (r9 == -1)  r9 = ranges[TRIGGERANA::rR9][0];

        if(ranges[TRIGGERANA::rETA][0] <= TMath::Abs(eta) &&
           (ranges[TRIGGERANA::rETA][1] <= -1 || TMath::Abs(eta) < ranges[TRIGGERANA::rETA][1])) {
        if(ranges[TRIGGERANA::rRECOPT][0] <= recoPt &&
           (ranges[TRIGGERANA::rRECOPT][1] <= -1 || recoPt < ranges[TRIGGERANA::rRECOPT][1])) {
        if(ranges[TRIGGERANA::rCENT][0] <= cent &&
           (ranges[TRIGGERANA::rCENT][1] <= -1 || cent < ranges[TRIGGERANA::rCENT][1])) {
        if((ranges[TRIGGERANA::rSUMISO][0] <= sumIso && sumIso < ranges[TRIGGERANA::rSUMISO][1]) ||
           (ranges[TRIGGERANA::rSUMISO][0] > ranges[TRIGGERANA::rSUMISO][1])) {
        if(ranges[TRIGGERANA::rSIEIE][0] <= sieie &&
           (ranges[TRIGGERANA::rSIEIE][1] <= -1 || sieie < ranges[TRIGGERANA::rSIEIE][1])) {
        if(ranges[TRIGGERANA::rR9][0] <= r9 &&
           (ranges[TRIGGERANA::rR9][1] <= -1 || r9 < ranges[TRIGGERANA::rR9][1])) {
                return true;
        }}}}}}
        return false;
    }
    else
        return false;
}

std::string triggerAnalyzer::getRangeText(int iRange) {

    std::string res = "";

    switch (iRange) {
    case TRIGGERANA::rETA : {

        if (ranges[TRIGGERANA::rETA][0] > ranges[TRIGGERANA::rETA][1])
            break;
        else if (ranges[TRIGGERANA::rETA][0] <= 0 && ranges[TRIGGERANA::rETA][1] > 0)
            res  = Form("|#eta|<%.2f", ranges[TRIGGERANA::rETA][1]);
        else if (ranges[TRIGGERANA::rETA][0] > 0 && ranges[TRIGGERANA::rETA][1] > 0)
            res  = Form("%.2f<|#eta|<%.2f", ranges[TRIGGERANA::rETA][0], ranges[TRIGGERANA::rETA][1]);
        break;
    }
    case TRIGGERANA::rRECOPT : {

        if (ranges[TRIGGERANA::rRECOPT][0] > 0 && ranges[TRIGGERANA::rRECOPT][1] <= -1)
            res  = Form("p_{T}^{reco}>%.0f", ranges[TRIGGERANA::rRECOPT][0]);
        else if (ranges[TRIGGERANA::rRECOPT][0] <= 0 && ranges[TRIGGERANA::rRECOPT][1] > 0)
            res = Form("p_{T}^{reco}<%.0f", ranges[TRIGGERANA::rRECOPT][1]);
        else if (ranges[TRIGGERANA::rRECOPT][0] > 0 && ranges[TRIGGERANA::rRECOPT][1] > 0)
            res = Form("%.0f<p_{T}^{reco}<%.0f", ranges[TRIGGERANA::rRECOPT][0], ranges[TRIGGERANA::rRECOPT][1]);
        break;
    }
    case TRIGGERANA::rCENT : {

        if (ranges[TRIGGERANA::rCENT][0] >= 0 && ranges[TRIGGERANA::rCENT][1] > 0)
            res = Form("Cent:%.0f-%.0f%%", ranges[TRIGGERANA::rCENT][0], ranges[TRIGGERANA::rCENT][1]);
        break;
    }
    case TRIGGERANA::rSUMISO : {

        if (recoObj == TRIGGERANA::kPHOTON) {
            if (ranges[TRIGGERANA::rSUMISO][0] > ranges[TRIGGERANA::rSUMISO][1])
                break;
            else if (ranges[TRIGGERANA::rSUMISO][0] > -999 && ranges[TRIGGERANA::rSUMISO][1] <= -999)
                res  = Form("sumIso>%.1f", ranges[TRIGGERANA::rSUMISO][0]);
            else if (ranges[TRIGGERANA::rSUMISO][0] <= -999 && ranges[TRIGGERANA::rSUMISO][1] > -999)
                res = Form("sumIso<%.1f", ranges[TRIGGERANA::rSUMISO][1]);
            else if (ranges[TRIGGERANA::rSUMISO][0] > -999 && ranges[TRIGGERANA::rSUMISO][1] > -999)
                res = Form("%.1f<sumIso<%.1f", ranges[TRIGGERANA::rSUMISO][0], ranges[TRIGGERANA::rSUMISO][1]);
        }
        break;
    }
    case TRIGGERANA::rSIEIE : {

        if (recoObj == TRIGGERANA::kPHOTON) {
            if (ranges[TRIGGERANA::rSIEIE][0] > 0 && ranges[TRIGGERANA::rSIEIE][1] <= -1)
                res  = Form("#sigma_{#eta#eta}>%.2f", ranges[TRIGGERANA::rSIEIE][0]);
            else if (ranges[TRIGGERANA::rSIEIE][0] <= 0 && ranges[TRIGGERANA::rSIEIE][1] > 0)
                res = Form("#sigma_{#eta#eta}<%.2f", ranges[TRIGGERANA::rSIEIE][1]);
            else if (ranges[TRIGGERANA::rSIEIE][0] > 0 && ranges[TRIGGERANA::rSIEIE][1] > 0)
                res = Form("%.2f<#sigma_{#eta#eta}<%.2f", ranges[TRIGGERANA::rSIEIE][0], ranges[TRIGGERANA::rSIEIE][1]);
        }
        break;
    }
    case TRIGGERANA::rR9 : {

        if (recoObj == TRIGGERANA::kPHOTON) {
            if (ranges[TRIGGERANA::rR9][0] > 0 && ranges[TRIGGERANA::rR9][1] <= -1)
                res  = Form("R9>%.2f", ranges[TRIGGERANA::rR9][0]);
            else if (ranges[TRIGGERANA::rR9][0] <= 0 && ranges[TRIGGERANA::rR9][1] > 0)
                res = Form("R9<%.2f", ranges[TRIGGERANA::rR9][1]);
            else if (ranges[TRIGGERANA::rR9][0] > 0 && ranges[TRIGGERANA::rR9][1] > 0)
                res = Form("%.2f<R9<%.2f", ranges[TRIGGERANA::rR9][0], ranges[TRIGGERANA::rR9][1]);
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

bool triggerAnalyzer::isValid()
{
    if (isValid_hNum)  return true;
    if (isValid_hDenom)  return true;
    if (isValid_hEff)  return true;
    if (isValid_gEff)  return true;

    if (isValid_h2Num)  return true;
    if (isValid_h2Denom)  return true;
    if (isValid_h2Eff)  return true;

    if (isValid_hNumInEff)  return true;
    if (isValid_hInEff)  return true;
    if (isValid_gInEff)  return true;

    if (isValid_hFakeNum)  return true;
    if (isValid_hFakeDenom)  return true;
    if (isValid_hFakeRatio)  return true;
    if (isValid_gFakeRatio)  return true;

    return false;
}

void triggerAnalyzer::update()
{
    pathNumText = getTriggerPathText(pathNum);
    pathDenomText = getTriggerPathText(pathDenom);

    ptTreshold = extractPtThreshold(pathNum.c_str());
    if (dep != TRIGGERANA::kRECOPT) {
        ptTreshold += getOffset4PtThreshold(ptTreshold);
        ranges[TRIGGERANA::rRECOPT][0] = ptTreshold;
    }

    title = pathNumText;

    updateTH1();
}

void triggerAnalyzer::updateTH1()
{
    isValid_hNum = (hNum != 0 && !hNum->IsZombie());
    isValid_hDenom = (hDenom != 0 && !hDenom->IsZombie());
    isValid_hEff = (hEff != 0 && !hEff->IsZombie());
    isValid_gEff = (gEff != 0 && !gEff->IsZombie());

    isValid_h2Num = (h2Num != 0 && !h2Num->IsZombie());
    isValid_h2Denom = (h2Denom != 0 && !h2Denom->IsZombie());
    isValid_h2Eff = (h2Eff != 0 && !h2Eff->IsZombie());

    isValid_hNumInEff = (hNumInEff != 0 && !hNumInEff->IsZombie());
    isValid_hInEff = (hInEff != 0 && !hInEff->IsZombie());
    isValid_gInEff = (gInEff != 0 && !gInEff->IsZombie());

    isValid_hFakeNum = (hFakeNum != 0 && !hFakeNum->IsZombie());
    isValid_hFakeDenom = (hFakeDenom != 0 && !hFakeDenom->IsZombie());
    isValid_hFakeRatio = (hFakeRatio != 0 && !hFakeRatio->IsZombie());
    isValid_gFakeRatio = (gFakeRatio != 0 && !gFakeRatio->IsZombie());

    if (isValid_hNum) {
        nBinsX = hNum->GetXaxis()->GetNbins();
    }

    if(isValid_hNum) {
        hNum->SetTitle(title.c_str());
    }
    if(isValid_hDenom) {
        hDenom->SetTitle(title.c_str());
    }
    if(isValid_hEff) {
        hEff->SetTitle(title.c_str());
    }
    if(isValid_gEff) {
        gEff->SetTitle(title.c_str());
    }

    if(isValid_h2Num) {
        h2Num->SetTitle(title.c_str());
    }
    if(isValid_h2Denom) {
        h2Denom->SetTitle(title.c_str());
    }
    if(isValid_h2Eff) {
        h2Eff->SetTitle(title.c_str());
    }

    if(isValid_hNumInEff) {
        hNumInEff->SetTitle(title.c_str());
    }
    if(isValid_hInEff) {
        hInEff->SetTitle(title.c_str());
    }
    if(isValid_gInEff) {
        gInEff->SetTitle(title.c_str());
    }
}

std::string triggerAnalyzer::getObjectStr(int iObj)
{
    switch (iObj) {
    case triggerAnalyzer::OBJ::kNum :
        return "Num";
    case triggerAnalyzer::OBJ::kDenom :
        return "Denom";
    case triggerAnalyzer::OBJ::kEff :
        return "Eff";
    case triggerAnalyzer::OBJ::kNumInEff :
        return "NumInEff";
    case triggerAnalyzer::OBJ::kInEff :
        return "InEff";
    case triggerAnalyzer::OBJ::kFakeNum :
        return "FakeNum";
    case triggerAnalyzer::OBJ::kFakeDenom :
        return "FakeDenom";
    case triggerAnalyzer::OBJ::kFakeRatio :
        return "FakeRatio";
    default :
        return "";
    }

    return "";
}

std::string triggerAnalyzer::getTObjectStr(int iTObj)
{
    switch (iTObj) {
    case triggerAnalyzer::TOBJ::kTH1D :
        return "h";
    case triggerAnalyzer::TOBJ::kTH2D :
        return "h2";
    case triggerAnalyzer::TOBJ::kTGraph :
        return "g";
    default :
        return "";
    }

    return "";
}

std::string triggerAnalyzer::getObjectName(int iObj, int iTObj)
{
    std::string objStr =  triggerAnalyzer::getObjectStr(iObj);
    std::string tObjStr = triggerAnalyzer::getTObjectStr(iTObj);

    return Form("%s%s_%s", tObjStr.c_str(), objStr.c_str(), name.c_str());
}

/*
 * prepare the object title using the ranges
 */
void triggerAnalyzer::prepareTitleRanges()
{
    // prepare histogram title
    // if etaStr is empty, then there is no eta range/selection.
    std::string etaStr = "";
    std::string recoPtStr = "";
    std::string centStr = "";
    std::string sumIsoStr = "";
    std::string sieieStr = "";
    std::string r9Str = "";

    etaStr  = getRangeText(TRIGGERANA::rETA);
    recoPtStr  = getRangeText(TRIGGERANA::rRECOPT);
    centStr  = getRangeText(TRIGGERANA::rCENT);
    sumIsoStr = getRangeText(TRIGGERANA::rSUMISO);
    sieieStr = getRangeText(TRIGGERANA::rSIEIE);
    r9Str = getRangeText(TRIGGERANA::rR9);

    titleRanges = "";
    if (etaStr.size() > 0)  titleRanges.append(Form("%s", etaStr.c_str()));
    if (recoPtStr.size() > 0) titleRanges.append(Form(", %s", recoPtStr.c_str()));
    if (centStr.size() > 0)  titleRanges.append(Form(", %s", centStr.c_str()));
    if (sumIsoStr.size() > 0)  titleRanges.append(Form(", %s", sumIsoStr.c_str()));
    if (sieieStr.size() > 0)  titleRanges.append(Form(", %s", sieieStr.c_str()));
    if (r9Str.size() > 0)  titleRanges.append(Form(", %s", r9Str.c_str()));
}

/*
 * prepare the text lines using the ranges
 */
void triggerAnalyzer::prepareTextLinesRanges()
{
    // if etaStr is empty, then there is no eta range/selection.
    std::string etaStr = "";
    std::string recoPtStr = "";
    std::string centStr = "";
    std::string sumIsoStr = "";
    std::string sieieStr = "";
    std::string r9Str = "";

    etaStr  = getRangeText(TRIGGERANA::rETA);
    recoPtStr  = getRangeText(TRIGGERANA::rRECOPT);
    centStr  = getRangeText(TRIGGERANA::rCENT);
    sumIsoStr = getRangeText(TRIGGERANA::rSUMISO);
    sieieStr = getRangeText(TRIGGERANA::rSIEIE);
    r9Str = getRangeText(TRIGGERANA::rR9);

    textLinesRanges.clear();
    if (etaStr.size() > 0)  textLinesRanges.push_back(etaStr);
    if (recoPtStr.size() > 0) textLinesRanges.push_back(recoPtStr);
    if (centStr.size() > 0)  textLinesRanges.push_back(centStr);
    if (sumIsoStr.size() > 0)  textLinesRanges.push_back(sumIsoStr);
    if (sieieStr.size() > 0)  textLinesRanges.push_back(sieieStr);
    if (r9Str.size() > 0)  textLinesRanges.push_back(r9Str);
}

std::string triggerAnalyzer::getTitleAll()
{
    std::string res = title;

    if (res.size() > 0)  res.append(Form(" %s", titleRanges.c_str()));
    else                 res.append(titleRanges.c_str());

    return res;
}

std::vector<std::string> triggerAnalyzer::getTextLinesAll()
{
    std::vector<std::string> res = textLines;

    res.insert(res.end(), textLinesRanges.begin(), textLinesRanges.end());

    return res;
}

/*
 * split the textLines so that the information looks uniform over multiple columns
 */
std::vector<std::string> triggerAnalyzer::splitTextLines(std::vector<std::string> textLines, int nColumns)
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

void triggerAnalyzer::postLoop()
{
    calcEff();
    calcEff2D();
    calcInEff();
    calcFakeRatio();
}

void triggerAnalyzer::calcEff()
{
    if (!isValid_hNum || !isValid_hDenom) return;

    hNum->SetTitle(title.c_str());
    hNum->SetXTitle(titleX.c_str());
    setTH1_efficiency(hNum, titleOffsetX, titleOffsetY);

    hDenom->SetTitle(title.c_str());
    hDenom->SetXTitle(titleX.c_str());
    setTH1_efficiency(hDenom, titleOffsetX, titleOffsetY);

    if (isValid_gEff) {
        gEff->Delete();
        isValid_gEff = false;
    }

    gEff = new TGraphAsymmErrors();
    std::string tmpName;
    tmpName = getObjectName(triggerAnalyzer::OBJ::kEff, triggerAnalyzer::TOBJ::kTGraph);
    gEff->SetName(tmpName.c_str());
    gEff->BayesDivide(hNum, hDenom);
    gEff->SetTitle(title.c_str());
    gEff->GetXaxis()->SetTitle(titleX.c_str());
    gEff->GetYaxis()->SetTitle("Efficiency");
    gEff->SetMarkerStyle(kFullCircle);

    isValid_gEff = true;

    if (isValid_hEff) {
        hEff->Delete();
        isValid_hEff = false;
    }

    tmpName = getObjectName(triggerAnalyzer::OBJ::kEff, triggerAnalyzer::TOBJ::kTH1D);
    hEff = (TH1D*)hNum->Clone(tmpName.c_str());
    fillTH1fromTGraph(hEff, gEff);
    setTH1_efficiency(hEff, titleOffsetX, titleOffsetY);
    hEff->SetTitle(title.c_str());
    hEff->SetXTitle(titleX.c_str());
    hEff->SetYTitle("Efficiency");
    hEff->SetMinimum(0);
    hEff->SetMaximum(1.2);

    isValid_hEff = true;
}

void triggerAnalyzer::calcEff2D()
{
    if (!isValid_h2Num || !isValid_h2Denom) return;

    h2Num->SetTitle(title.c_str());
    h2Num->SetXTitle(titleX.c_str());
    setTH1_efficiency(h2Num, titleOffsetX, titleOffsetY);

    h2Denom->SetTitle(title.c_str());
    h2Denom->SetXTitle(titleX.c_str());
    setTH1_efficiency(h2Denom, titleOffsetX, titleOffsetY);

    std::string tmpName;
    tmpName = getObjectName(triggerAnalyzer::OBJ::kEff, triggerAnalyzer::TOBJ::kTH2D);
    h2Eff = (TH2D*)h2Num->Clone(tmpName.c_str());
    h2Eff->Divide(h2Denom);
    setTH1_efficiency(h2Eff, titleOffsetX, titleOffsetY);
    h2Eff->SetTitle(title.c_str());
    h2Eff->SetXTitle(titleX.c_str());
    h2Eff->GetZaxis()->SetRangeUser(0, 1.0);

    isValid_h2Eff = true;
}

void triggerAnalyzer::calcInEff()
{
    if (!isValid_hNumInEff || !isValid_hDenom) return;

    hNumInEff->SetTitle(title.c_str());
    hNumInEff->SetXTitle(titleX.c_str());
    setTH1_efficiency(hNumInEff, titleOffsetX, titleOffsetY);

    hDenom->SetTitle(title.c_str());
    hDenom->SetXTitle(titleX.c_str());
    setTH1_efficiency(hDenom, titleOffsetX, titleOffsetY);

    if (isValid_gInEff) {
        gInEff->Delete();
        isValid_gInEff = false;
    }

    gInEff = new TGraphAsymmErrors();
    std::string tmpName;
    tmpName = getObjectName(triggerAnalyzer::OBJ::kInEff, triggerAnalyzer::TOBJ::kTGraph);
    gInEff->SetName(tmpName.c_str());
    gInEff->BayesDivide(hNumInEff, hDenom);
    gInEff->SetTitle(title.c_str());
    gInEff->GetXaxis()->SetTitle(titleX.c_str());
    gInEff->GetYaxis()->SetTitle("Inefficiency");
    gInEff->SetMarkerStyle(kFullCircle);

    isValid_gInEff = true;

    if (isValid_hInEff) {
        hInEff->Delete();
        isValid_hInEff = false;
    }

    tmpName = getObjectName(triggerAnalyzer::OBJ::kInEff, triggerAnalyzer::TOBJ::kTH1D);
    hInEff = (TH1D*)hNumInEff->Clone(tmpName.c_str());
    fillTH1fromTGraph(hInEff, gInEff);
    setTH1_efficiency(hInEff, titleOffsetX, titleOffsetY);
    hInEff->SetTitle(title.c_str());
    hInEff->SetXTitle(titleX.c_str());
    hInEff->SetYTitle("Inefficiency");
    hInEff->SetMinimum(0);
    hInEff->SetMaximum(1.2);

    isValid_hInEff = true;
}

/*
 * write histograms with a particular dependence
 * use "c" as a template
 */
void triggerAnalyzer::calcFakeRatio()
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
    nameTmp = getObjectName(triggerAnalyzer::OBJ::kFakeRatio, triggerAnalyzer::TOBJ::kTGraph);
    gFakeRatio->SetName(nameTmp.c_str());
    gFakeRatio->BayesDivide(hFakeNum, hFakeDenom);
    gFakeRatio->SetTitle(title.c_str());
    gFakeRatio->GetXaxis()->SetTitle(titleX.c_str());
    gFakeRatio->GetYaxis()->SetTitle("Trigger Fake Rate");
    gFakeRatio->SetMarkerStyle(kFullCircle);

    isValid_gFakeRatio = true;

    if (isValid_hFakeRatio) {
        hFakeRatio->Delete();
        isValid_hFakeRatio = false;
    }

    nameTmp = getObjectName(triggerAnalyzer::OBJ::kFakeRatio, triggerAnalyzer::TOBJ::kTH1D);
    hFakeRatio = (TH1D*)hFakeNum->Clone(nameTmp.c_str());
    fillTH1fromTGraph(hFakeRatio, gFakeRatio);
    setTH1_efficiency(hFakeDenom, titleOffsetX, titleOffsetY);
    hFakeRatio->SetTitle(title.c_str());
    hFakeRatio->SetXTitle(titleX.c_str());
    hFakeRatio->SetYTitle("Trigger Fake Rate");
    hFakeRatio->SetMinimum(0);
    hFakeRatio->SetMaximum(1.2);

    isValid_hFakeRatio = true;
}

void triggerAnalyzer::writeObjects(TCanvas* c)
{
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
    TLatex* latex = new TLatex();
    std::vector<std::string> textLinesAll = getTextLinesAll();

    // efficiency objects
    if (isValid_hNum) {
        canvasName = Form("cnv_%s_%s", getObjectStr(triggerAnalyzer::OBJ::kNum).c_str(), name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        hNum->SetMarkerSize(markerSize);
        hNum->Draw("e");
        hNum->Write("",TObject::kOverwrite);

        latex = new TLatex();
        setLatex(latex, "NE");
        drawTextLines(latex, c, textLinesAll, "NE", textOffsetX, textOffsetY);

        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.
    }
    if (isValid_hDenom) {
        canvasName = Form("cnv_%s_%s", getObjectStr(triggerAnalyzer::OBJ::kDenom).c_str(), name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        hDenom->SetMarkerSize(markerSize);
        hDenom->Draw("e");
        hDenom->Write("",TObject::kOverwrite);

        latex = new TLatex();
        setLatex(latex, "NE");
        drawTextLines(latex, c, textLinesAll, "NE", textOffsetX, textOffsetY);

        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.
    }
    if (isValid_hEff) {
        int iObs = TRIGGERANA::kEFF;
        canvasName = Form("cnv_%sH1D_%s", TRIGGERANA::OBS_LABELS[iObs].c_str() , name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        hEff->SetMarkerSize(markerSize);
        hEff->Draw("e");
        hEff->Write("",TObject::kOverwrite);

        latex = new TLatex();
        setLatex(latex, "NE");
        drawTextLines(latex, c, textLinesAll, "NE", textOffsetX, textOffsetY);

        setPad4Observable((TPad*) c, iObs);
        drawLine4PtThreshold((TPad*) c);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.
    }
    if (isValid_hEff && isValid_gEff) {
        int iObs = TRIGGERANA::kEFF;
        canvasName = Form("cnv_%s_%s", TRIGGERANA::OBS_LABELS[iObs].c_str() , name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        // dummy histogram to be used as template for the graph
        TH1D* hTmp = (TH1D*)hEff->Clone("hTmp");
        hTmp->Reset();
        hTmp->Draw();
        gEff->SetMarkerSize(markerSize);
        gEff->Draw("p e");
        gEff->Write("",TObject::kOverwrite);

        latex = new TLatex();
        setLatex(latex, "NE");
        drawTextLines(latex, c, textLinesAll, "NE", textOffsetX, textOffsetY);

        setPad4Observable((TPad*) c, iObs);
        drawLine4PtThreshold((TPad*) c);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.
        hTmp->Delete();
    }
    if (isValid_h2Num) {
        canvasName = Form("cnv2D_%s_%s", getObjectStr(triggerAnalyzer::OBJ::kNum).c_str(), name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin+0.08, bottomMargin, topMargin);
        h2Num->SetMarkerSize(markerSize);
        h2Num->Draw("colz");
        h2Num->Write("",TObject::kOverwrite);

        latex = new TLatex();
        setLatex(latex, "NE");
        drawTextLines(latex, c, textLinesAll, "NE", textOffsetX, textOffsetY);

        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.
    }
    if (isValid_h2Denom) {
        canvasName = Form("cnv2D_%s_%s", getObjectStr(triggerAnalyzer::OBJ::kDenom).c_str(), name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin+0.08, bottomMargin, topMargin);
        h2Denom->SetMarkerSize(markerSize);
        h2Denom->Draw("colz");
        h2Denom->Write("",TObject::kOverwrite);

        latex = new TLatex();
        setLatex(latex, "NE");
        drawTextLines(latex, c, textLinesAll, "NE", textOffsetX, textOffsetY);

        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.
    }
    if (isValid_h2Eff) {
        int iObs = TRIGGERANA::kEFF;
        canvasName = Form("cnv2D_%s_%s", TRIGGERANA::OBS_LABELS[iObs].c_str() , name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin+0.08, bottomMargin, topMargin);
        h2Eff->SetMarkerSize(markerSize);
        h2Eff->Draw("colz");
        h2Eff->Write("",TObject::kOverwrite);

        latex = new TLatex();
        setLatex(latex, "NE");
        drawTextLines(latex, c, textLinesAll, "NE", textOffsetX, textOffsetY);

        setPad4Observable((TPad*) c, iObs);
        drawLine4PtThreshold((TPad*) c);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.
    }

    // inefficiency objects
    if (isValid_hNumInEff) {
        canvasName = Form("cnv_%s_%s", getObjectStr(triggerAnalyzer::OBJ::kNumInEff).c_str(), name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        hNumInEff->SetMarkerSize(markerSize);
        hNumInEff->Draw("e");
        hNumInEff->Write("",TObject::kOverwrite);

        latex = new TLatex();
        setLatex(latex, "NE");
        drawTextLines(latex, c, textLinesAll, "NE", textOffsetX, textOffsetY);

        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.
    }
    if (isValid_hInEff) {
        int iObs = TRIGGERANA::kINEFF;
        canvasName = Form("cnv_%sH1D_%s", TRIGGERANA::OBS_LABELS[iObs].c_str() , name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        hInEff->SetMarkerSize(markerSize);
        hInEff->Draw("e");
        hInEff->Write("",TObject::kOverwrite);

        latex = new TLatex();
        setLatex(latex, "NE");
        drawTextLines(latex, c, textLinesAll, "NE", textOffsetX, textOffsetY);

        setPad4Observable((TPad*) c, iObs);
        drawLine4PtThreshold((TPad*) c);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.
    }
    if (isValid_hInEff && isValid_gInEff) {
        int iObs = TRIGGERANA::kINEFF;
        canvasName = Form("cnv_%s_%s", TRIGGERANA::OBS_LABELS[iObs].c_str() , name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        // dummy histogram to be used as template for the graph
        TH1D* hTmp = (TH1D*)hInEff->Clone("hTmp");
        hTmp->Reset();
        hTmp->Draw();
        gInEff->SetMarkerSize(markerSize);
        gInEff->Draw("p e");
        gInEff->Write("",TObject::kOverwrite);

        latex = new TLatex();
        setLatex(latex, "NE");
        drawTextLines(latex, c, textLinesAll, "NE", textOffsetX, textOffsetY);

        setPad4Observable((TPad*) c, iObs);
        drawLine4PtThreshold((TPad*) c);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.
        hTmp->Delete();
    }

    // fake rate objects
    if (isValid_hFakeNum) {
        canvasName = Form("cnv_%s_%s", getObjectStr(triggerAnalyzer::OBJ::kFakeNum).c_str(), name.c_str());
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
        canvasName = Form("cnv_%s_%s", getObjectStr(triggerAnalyzer::OBJ::kFakeDenom).c_str(), name.c_str());
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
        int iObs = TRIGGERANA::kFAKE;
        canvasName = Form("cnv_%s_%s", TRIGGERANA::OBS_LABELS[iObs].c_str() , name.c_str());
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
        drawLine4PtThreshold((TPad*) c);
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
        int iObs = TRIGGERANA::kFAKE;
        canvasName = Form("cnv_%sgraph_%s", TRIGGERANA::OBS_LABELS[iObs].c_str() , name.c_str());
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
        drawLine4PtThreshold((TPad*) c);
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

     if (line != 0)  line->Delete();
     latex->Delete();
}

void triggerAnalyzer::setPad4Observable(TPad* p, int iObs, int iDep)
{
    TLine* line = 0;

    p->Update();
    bool hasH2D = containsClassInstance(p, "TH2");
    if (!hasH2D && (iObs == TRIGGERANA::kEFF || iObs == TRIGGERANA::kINEFF || iObs == TRIGGERANA::kFAKE)) {

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

    if (iDep == TRIGGERANA::kETA) {
        // draw line for EE-EB transition
        double ECAL_boundary_1 = 1.4442;
        double ECAL_boundary_2 = 1.566;

        double yMin = p->GetUymin();
        double yMax = p->GetUymax();
        if (!hasH2D && (iObs == TRIGGERANA::kEFF || iObs == TRIGGERANA::kINEFF || iObs == TRIGGERANA::kFAKE))  yMax = 1;

        // draw lines for ECAL transition region
        std::vector<double> lineXvalues {-1*ECAL_boundary_1, ECAL_boundary_1, -1*ECAL_boundary_2, ECAL_boundary_2};
        for (std::vector<double>::const_iterator itLine = lineXvalues.begin(); itLine !=lineXvalues.end(); ++itLine) {

            line = new TLine((*itLine), yMin, (*itLine), yMax);
            line->SetLineStyle(kDashed);
            line->Draw();
        }
    }
}

void triggerAnalyzer::setPad4Observable(TPad* p, int iObs)
{
    setPad4Observable(p, iObs, dep);
}

void triggerAnalyzer::setLatex(TLatex* latex, std::string position)
{
    setTextAlignment(latex, position.c_str());
    latex->SetTextFont(textFont);
    latex->SetTextSize(textSize);
}

/*
 * If the x-axis is reco pt, then draws vertical line at the pt threshold.
 */
void triggerAnalyzer::drawLine4PtThreshold(TPad* p, int lineColor)
{
    TLine* line = 0;

    p->Update();
    double x1 = p->GetUxmin();
    double x2 = p->GetUxmax();
    double y1 = p->GetUymin();
    double y2 = p->GetUymax();
    if (y2 > 1)  y2 = 1;        // vertical line rises to at most y = 1

    if (dep == TRIGGERANA::kRECOPT) {

        // vertical line at the pt threshold
        if (ptTreshold > x1 && ptTreshold < x2) {

            line = new TLine(ptTreshold, y1, ptTreshold, y2);
            line->SetLineStyle(kDotted);
            line->SetLineColor(lineColor);
            line->SetLineWidth(line->GetLineWidth()*3);
            line->Draw();
        }
    }
}

/*
 * make the trigger path simpler by removing unnecessary substrings.
 */
std::string triggerAnalyzer::getTriggerPathText(std::string triggerPath)
{
    std::string res = triggerPath;

    // remove the "HLT_" prefix
    res = replaceAll(res, "HLT_", "");

    // trigger name ends with _v1, _v2, _v3, etc.
    // remove that suffix
    // Ex. HLT_HISinglePhoton20_Eta1p5_v1 becomes HLT_HISinglePhoton20_Eta1p5
    std::string suffix = "_v";
    int len = 0;
    res = res.substr(0, res.rfind(suffix)+len);

    return res;
}

/*
 * return the TTree* name of HLT object corresponding to the trigger bit
 * return empty string if nothing is found
 */
std::string triggerAnalyzer::getHLTObjectName(std::string triggerName)
{
    // should be an HLT object
    if (triggerName.find("HLT") != 0) return "";

    // trigger name ends with _v1, _v2, _v3, etc.
    // HLT object tree ends with _v. trim the number at the end
    // Ex. HLT bit is HLT_HISinglePhoton20_Eta1p5_v1
    // HLT object is HLT_HISinglePhoton20_Eta1p5_v
    std::string suffix = "_v";
    int suffixLen = suffix.size();

    if (triggerName.find(suffix.c_str()) != std::string::npos) {
        return triggerName.substr(0, triggerName.rfind(suffix)+suffixLen);
    }
    else
        return triggerName;
}

/*
 * extract pt threshold from the name of trigger
 * Ex. "HLT_HISinglePhoton15_Eta3p1_v5" should return 15
 */
double triggerAnalyzer::extractPtThreshold(std::string triggerName)
{
    std::string strSub = "Photon";
    if (triggerName.find("L1") == 0) {
        // this is L1 trigger, look for EG
        strSub = "EG";
    }
    int len = strSub.size();
    size_t indexStart = triggerName.find(strSub.c_str()) + len;

    std::string strXX = triggerName.substr(indexStart, 2);

    std::istringstream sin(strXX);

    float res;
    sin >> res;

    return res;
}

/*
 * extract eta threshold from the name of trigger
 * Ex. "HLT_HISinglePhoton15_Eta3p1_v5" should return 3.1
 */
double triggerAnalyzer::extractEtaThreshold(std::string triggerName)
{
    std::string strSub = "Eta";
    int len = strSub.size();
    size_t indexStart = triggerName.find(strSub.c_str()) + len;

    std::string strXpY_X = triggerName.substr(indexStart, 1);
    std::string strXpY_Y = triggerName.substr(indexStart+2, 1);

    std::istringstream sinX(strXpY_X);
    std::istringstream sinY(strXpY_Y);

    float resX, resY;
    sinX >> resX;
    sinY >> resY;

    return resX+(0.1*resY);
}

/*
 * the offset will be used for trigger efficiency as func. of eta, eta-phi
 */
double triggerAnalyzer::getOffset4PtThreshold(double ptThreshold)
{
    if (ptThreshold >= 40)  return 5;
    else if (ptThreshold >= 20)  return 4;
    else if (ptThreshold >= 10)  return 3;
    else if (ptThreshold >= 5)  return 2;
    return 0;
}

int triggerAnalyzer::setBranchAdressTrigger(TTree* tree, std::string branch, int & branchVal)
{
    if (branch.size() == 0 || branch == CONFIGPARSER::nullInput) {
        branchVal = 1;
        return -2;
    }
    else if (tree->GetBranch(branch.c_str())) {
        tree->SetBranchStatus(branch.c_str(),1);
        tree->SetBranchAddress(branch.c_str(),&(branchVal));
        return 0;
    }
    else {
        std::cout<<"could not GetBranch() : "<<branch.c_str()<<std::endl;
        std::cout<<"default value set to  : "<<branch.c_str()<<" = 1"<<std::endl;
        branchVal = 1;
        return -1;
    }
}

void triggerAnalyzer::setBranchesTrigger(TTree* tree, std::vector<std::string> branchNames, int val[], int nVal)
{
    for (int i = 0; i < nVal; ++i) {
        int branchSetFlag = triggerAnalyzer::setBranchAdressTrigger(tree, branchNames[i], val[i]);
        if (branchSetFlag == -1) {
            std::cout << "set branch addresses for triggers" << std::endl;
            std::cout << "Following branch is not found : "  << branchNames[i].c_str() << std::endl;
        }
    }
}

#endif
