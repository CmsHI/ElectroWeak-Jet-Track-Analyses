/*
 * general purpose class for spectra analysis
 */

#ifndef PERFORMANCE_INTERFACE_SPECTRAANALYZER_H_
#define PERFORMANCE_INTERFACE_SPECTRAANALYZER_H_

#include <TH1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TF1.h>
#include <TFormula.h>
#include <TMath.h>
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

namespace SPECTRAANA {

enum OBJS {   // objects
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

enum DEPS {   // dependencies (x-axis)
    kETA,
    kRECOPT,
    kCENT,
    kSUMISO,
    kECALISO,
    kHCALISO,
    kTRKISO,
    kSIEIE,
    kR9,
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
        "SIEIE",
        "R9"
};

enum RANGES {   // ranges/cuts
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
    kRAW,        // distribution with raw number of entries, no normalization, no scaling
    kNORM,       // distribution normalized to 1 (by the number of entries), no scaling --> h->Integral() gives 1
    kNORMW,      // distribution normalized to 1 (by the number of entries) using bin width --> h->Integral("width") gives 1
    kNORMEVT,    // distribution normalized by the number of events, no scaling
    kNORMEVTW,   // distribution normalized by the number of events using bin width
    kN_OBS
};

const std::string OBS_LABELS[kN_OBS] = {"", "_norm1", "_norm1w", "_normEvt", "_normEvtw"};

};

class spectraAnalyzer {
public :
    spectraAnalyzer(){
        nBinsX = 0;

        h = 0;
        isValid_h = false;

        for (int i = 0; i < SPECTRAANA::kN_OBS; ++i) {
            h1D[i] = 0;
            isValid_h1D[i] = false;
        }

        nEvents = 0;

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

        for (int i=0; i<SPECTRAANA::kN_RANGES; ++i) {
            ranges[i][0] = 0;
            ranges[i][1] = -1;  // no upper bound
        }
    };
    ~spectraAnalyzer(){};

    void FillH(double x, double w, std::vector<double> vars);

    bool insideRange(std::vector<double> vars);
    void incrementEventCount(double eventWeight);

    std::string getRangeText(int iRange);

    bool isValid();
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
    void writeObjects(TCanvas* c);

    static void setPad4Observable(TPad* p, int iDep);
    void setPad4Observable(TPad* p);
    void setLatex(TLatex* latex, std::string position);

    int nBinsX;
    TH1D* h;
    bool isValid_h;

    /*
     * by definition, h1D[SPECTRAANA::kRAW] = h
     */
    TH1D* h1D[SPECTRAANA::kN_OBS];
    bool isValid_h1D[SPECTRAANA::kN_OBS];

    double nEvents;

    enum OBJ{
        kRaw,
        kNorm,
        kNormw,
        kNormEvt,
        kNormEvtw,
        kN_OBJ
    };

    enum TOBJ{
        kTH1D,
        kTH2D,
        kTGraph,
        kN_TOBJ
    };

    int recoObj;
    int dep;            // If the x-axis is eta, then dep = SPECTRAANA::kETA

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
    double ranges[SPECTRAANA::kN_RANGES][2];
};

void spectraAnalyzer::FillH(double x, double w, std::vector<double> vars)
{
    if (isValid_h && insideRange(vars)) {
        h->Fill(x, w);
    }
}

/*
 * check if the given variables lie inside the kinematic range defined for this object.
 */
bool spectraAnalyzer::insideRange(std::vector<double> vars)
{
    double eta = vars[SPECTRAANA::rETA];
    double recoPt = vars[SPECTRAANA::rRECOPT];
    double cent = vars[SPECTRAANA::rCENT];

    // make sure to pass the selection if no explicit kinematic range is specified.
    if (eta == -999)  eta = ranges[SPECTRAANA::rETA][0];
    if (recoPt == -1)  recoPt = ranges[SPECTRAANA::rRECOPT][0];
    if (cent == -1)  cent = ranges[SPECTRAANA::rCENT][0];

    if (recoObj == SPECTRAANA::OBJS::kPHOTON) {
        double sumIso = vars[SPECTRAANA::rSUMISO];
        double sieie = vars[SPECTRAANA::rSIEIE];
        double r9 = vars[SPECTRAANA::rR9];

        if (sumIso == -999)  sumIso = ranges[SPECTRAANA::rSUMISO][0];
        if (sieie == -1)  sieie = ranges[SPECTRAANA::rSIEIE][0];
        if (r9 == -1)  r9 = ranges[SPECTRAANA::rR9][0];

        if(ranges[SPECTRAANA::rETA][0] <= TMath::Abs(eta) &&
           (ranges[SPECTRAANA::rETA][1] == -1 || ranges[SPECTRAANA::rETA][1] > TMath::Abs(eta))){
        if(ranges[SPECTRAANA::rRECOPT][0] <= recoPt       &&
           (ranges[SPECTRAANA::rRECOPT][1] == -1 || ranges[SPECTRAANA::rRECOPT][1] > recoPt)) {
        if(ranges[SPECTRAANA::rCENT][0] <= cent           &&
           (ranges[SPECTRAANA::rCENT][1] == -1 || ranges[SPECTRAANA::rCENT][1] > cent)) {
        if(ranges[SPECTRAANA::rSUMISO][0] <= sumIso       &&
           (ranges[SPECTRAANA::rSUMISO][1] == -999 || ranges[SPECTRAANA::rSUMISO][1] > sumIso)) {
        if(ranges[SPECTRAANA::rSIEIE][0] <= sieie         &&
           (ranges[SPECTRAANA::rSIEIE][1] == -1 || ranges[SPECTRAANA::rSIEIE][1] > sieie)) {
        if(ranges[SPECTRAANA::rR9][0] <= r9         &&
           (ranges[SPECTRAANA::rR9][1] == -1 || ranges[SPECTRAANA::rR9][1] > r9)) {
                return true;
        }}}}}}
        return false;
    }
    else
        return false;
}

void spectraAnalyzer::incrementEventCount(double eventWeight) {
    nEvents += eventWeight;
}

std::string spectraAnalyzer::getRangeText(int iRange) {

    std::string res = "";

    switch (iRange) {
    case SPECTRAANA::rETA : {

        if (ranges[SPECTRAANA::rETA][0] <= 0 && ranges[SPECTRAANA::rETA][1] > 0)
            res  = Form("|#eta|<%.2f", ranges[SPECTRAANA::rETA][1]);
        else if (ranges[SPECTRAANA::rETA][0] > 0 && ranges[SPECTRAANA::rETA][1] > 0)
            res  = Form("%.2f<|#eta|<%.2f", ranges[SPECTRAANA::rETA][0], ranges[SPECTRAANA::rETA][1]);
        break;
    }
    case SPECTRAANA::rRECOPT : {

        if (ranges[SPECTRAANA::rRECOPT][0] > 0 && ranges[SPECTRAANA::rRECOPT][1] <= -1)
            res  = Form("p_{T}^{reco}>%.0f", ranges[SPECTRAANA::rRECOPT][0]);
        else if (ranges[SPECTRAANA::rRECOPT][0] <= 0 && ranges[SPECTRAANA::rRECOPT][1] > 0)
            res = Form("p_{T}^{reco}<%.0f", ranges[SPECTRAANA::rRECOPT][1]);
        else if (ranges[SPECTRAANA::rRECOPT][0] > 0 && ranges[SPECTRAANA::rRECOPT][1] > 0)
            res = Form("%.0f<p_{T}^{reco}<%.0f", ranges[SPECTRAANA::rRECOPT][0], ranges[SPECTRAANA::rRECOPT][1]);
        break;
    }
    case SPECTRAANA::rCENT : {

        if (ranges[SPECTRAANA::rCENT][0] >= 0 && ranges[SPECTRAANA::rCENT][1] > 0)
            res = Form("Cent:%.0f-%.0f%%", ranges[SPECTRAANA::rCENT][0], ranges[SPECTRAANA::rCENT][1]);
        break;
    }
    case SPECTRAANA::rSUMISO : {

        if (recoObj == SPECTRAANA::kPHOTON) {
            if (ranges[SPECTRAANA::rSUMISO][0] > -999 && ranges[SPECTRAANA::rSUMISO][1] <= -999)
                res  = Form("sumIso>%.1f", ranges[SPECTRAANA::rSUMISO][0]);
            else if (ranges[SPECTRAANA::rSUMISO][0] <= -999 && ranges[SPECTRAANA::rSUMISO][1] > -999)
                res = Form("sumIso<%.1f", ranges[SPECTRAANA::rSUMISO][1]);
            else if (ranges[SPECTRAANA::rSUMISO][0] > -999 && ranges[SPECTRAANA::rSUMISO][1] > -999)
                res = Form("%.1f<sumIso<%.1f", ranges[SPECTRAANA::rSUMISO][0], ranges[SPECTRAANA::rSUMISO][1]);
        }
        break;
    }
    case SPECTRAANA::rSIEIE : {

        if (recoObj == SPECTRAANA::kPHOTON) {
            if (ranges[SPECTRAANA::rSIEIE][0] > 0 && ranges[SPECTRAANA::rSIEIE][1] <= -1)
                res  = Form("#sigma_{#eta#eta}>%.2f", ranges[SPECTRAANA::rSIEIE][0]);
            else if (ranges[SPECTRAANA::rSIEIE][0] <= 0 && ranges[SPECTRAANA::rSIEIE][1] > 0)
                res = Form("#sigma_{#eta#eta}<%.2f", ranges[SPECTRAANA::rSIEIE][1]);
            else if (ranges[SPECTRAANA::rSIEIE][0] > 0 && ranges[SPECTRAANA::rSIEIE][1] > 0)
                res = Form("%.2f<#sigma_{#eta#eta}<%.2f", ranges[SPECTRAANA::rSIEIE][0], ranges[SPECTRAANA::rSIEIE][1]);
        }
        break;
    }
    case SPECTRAANA::rR9 : {

        if (recoObj == SPECTRAANA::kPHOTON) {
            if (ranges[SPECTRAANA::rR9][0] > 0 && ranges[SPECTRAANA::rR9][1] <= -1)
                res  = Form("R9>%.2f", ranges[SPECTRAANA::rR9][0]);
            else if (ranges[SPECTRAANA::rR9][0] <= 0 && ranges[SPECTRAANA::rR9][1] > 0)
                res = Form("R9<%.2f", ranges[SPECTRAANA::rR9][1]);
            else if (ranges[SPECTRAANA::rR9][0] > 0 && ranges[SPECTRAANA::rR9][1] > 0)
                res = Form("%.2f<R9<%.2f", ranges[SPECTRAANA::rR9][0], ranges[SPECTRAANA::rR9][1]);
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

bool spectraAnalyzer::isValid()
{
    if (isValid_h)  return true;

    for (int i = 0; i < SPECTRAANA::kN_OBS; ++i) {
        if (isValid_h1D[i])  return true;
    }

    return false;
}

void spectraAnalyzer::updateTH1()
{
    isValid_h = (h != 0 && !h->IsZombie());
    if (isValid_h) {
        nBinsX = h->GetXaxis()->GetNbins();
    }

    if(isValid_h) {
        h->SetTitle(title.c_str());
    }

    for (int i = 0; i < SPECTRAANA::kN_OBS; ++i) {
        isValid_h1D[i] = isValid_h;
    }
}

std::string spectraAnalyzer::getObjectStr(int iObj)
{
    switch (iObj) {
    case spectraAnalyzer::OBJ::kRaw :
        return "";
    case spectraAnalyzer::OBJ::kNorm :
        return "Norm1";
    case spectraAnalyzer::OBJ::kNormw :
        return "Norm1w";
    case spectraAnalyzer::OBJ::kNormEvt :
        return "NormEvt";
    case spectraAnalyzer::OBJ::kNormEvtw :
        return "NormEvtw";
    default :
        return "";
    }

    return "";
}

std::string spectraAnalyzer::getTObjectStr(int iTObj)
{
    switch (iTObj) {
    case spectraAnalyzer::TOBJ::kTH1D :
        return "h";
    case spectraAnalyzer::TOBJ::kTH2D :
        return "h2";
    case spectraAnalyzer::TOBJ::kTGraph :
        return "g";
    default :
        return "";
    }

    return "";
}

std::string spectraAnalyzer::getObjectName(int iObj, int iTObj)
{
    std::string objStr =  spectraAnalyzer::getObjectStr(iObj);
    std::string tObjStr = spectraAnalyzer::getTObjectStr(iTObj);

    return Form("%s%s_%s", tObjStr.c_str(), objStr.c_str(), name.c_str());
}

/*
 * prepare the object title using the ranges
 */
void spectraAnalyzer::prepareTitleRanges()
{
    // prepare histogram title
    // if etaStr is empty, then there is no eta range/selection.
    std::string etaStr = "";
    std::string recoPtStr = "";
    std::string centStr = "";
    std::string sumIsoStr = "";
    std::string sieieStr = "";
    std::string r9Str = "";

    etaStr  = getRangeText(SPECTRAANA::rETA);
    recoPtStr  = getRangeText(SPECTRAANA::rRECOPT);
    centStr  = getRangeText(SPECTRAANA::rCENT);
    sumIsoStr = getRangeText(SPECTRAANA::rSUMISO);
    sieieStr = getRangeText(SPECTRAANA::rSIEIE);
    r9Str = getRangeText(SPECTRAANA::rR9);

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
void spectraAnalyzer::prepareTextLinesRanges()
{
    // if etaStr is empty, then there is no eta range/selection.
    std::string etaStr = "";
    std::string recoPtStr = "";
    std::string centStr = "";
    std::string sumIsoStr = "";
    std::string sieieStr = "";
    std::string r9Str = "";

    etaStr  = getRangeText(SPECTRAANA::rETA);
    recoPtStr  = getRangeText(SPECTRAANA::rRECOPT);
    centStr  = getRangeText(SPECTRAANA::rCENT);
    sumIsoStr = getRangeText(SPECTRAANA::rSUMISO);
    sieieStr = getRangeText(SPECTRAANA::rSIEIE);
    r9Str = getRangeText(SPECTRAANA::rR9);

    textLinesRanges.clear();
    if (etaStr.size() > 0)  textLinesRanges.push_back(etaStr);
    if (recoPtStr.size() > 0) textLinesRanges.push_back(recoPtStr);
    if (centStr.size() > 0)  textLinesRanges.push_back(centStr);
    if (sumIsoStr.size() > 0)  textLinesRanges.push_back(sumIsoStr);
    if (sieieStr.size() > 0)  textLinesRanges.push_back(sieieStr);
    if (r9Str.size() > 0)  textLinesRanges.push_back(r9Str);
}

std::string spectraAnalyzer::getTitleAll()
{
    std::string res = title;

    if (res.size() > 0)  res.append(Form(" %s", titleRanges.c_str()));
    else                 res.append(titleRanges.c_str());

    return res;
}

std::vector<std::string> spectraAnalyzer::getTextLinesAll()
{
    std::vector<std::string> res = textLines;

    res.insert(res.end(), textLinesRanges.begin(), textLinesRanges.end());

    return res;
}

/*
 * split the textLines so that the information looks uniform over multiple columns
 */
std::vector<std::string> spectraAnalyzer::splitTextLines(std::vector<std::string> textLines, int nColumns)
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

void spectraAnalyzer::postLoop()
{
    if (!isValid_h)  return;

    h->SetTitleOffset(titleOffsetX, "X");
    h->SetTitleOffset(titleOffsetY, "Y");
    h->SetStats(false);
    h->GetXaxis()->CenterTitle();
    h->GetYaxis()->CenterTitle();
    h->SetMarkerStyle(kFullCircle);
    h->SetMinimum(0);

    h1D[SPECTRAANA::kRAW] = h;

    for (int iObs = 0; iObs < SPECTRAANA::kN_OBS; ++iObs) {

        if (iObs == SPECTRAANA::kRAW) continue;
        if (!isValid_h1D[iObs]) continue;

        std::string label = SPECTRAANA::OBS_LABELS[iObs];

        h1D[iObs] = (TH1D*)h->Clone(Form("h%s_%s", label.c_str(), name.c_str()));
        h1D[iObs]->SetTitle(title.c_str());
        h1D[iObs]->SetXTitle(titleX.c_str());
    }

    if (isValid_h1D[SPECTRAANA::kNORM]) {
        int iObs = SPECTRAANA::kNORM;

        h1D[iObs]->Scale(1./h1D[iObs]->Integral());
        h1D[iObs]->SetYTitle("Norm. to 1");
    }
    if (isValid_h1D[SPECTRAANA::kNORMW]) {
        int iObs = SPECTRAANA::kNORMW;

        h1D[iObs]->Scale(1./h1D[iObs]->Integral(), "width");
        h1D[iObs]->SetYTitle(Form("#frac{1}{N} #frac{dN}{d%s}", titleX.c_str()));
    }
    if (isValid_h1D[SPECTRAANA::kNORMEVT]) {
        int iObs = SPECTRAANA::kNORMEVT;

        h1D[iObs]->Scale(1./(double)nEvents);
        h1D[iObs]->SetYTitle("Norm. by # events");
    }
    if (isValid_h1D[SPECTRAANA::kNORMEVTW]) {
        int iObs = SPECTRAANA::kNORMEVTW;

        h1D[iObs]->Scale(1./(double)nEvents, "width");
        h1D[iObs]->SetYTitle(Form("#frac{1}{N_{Event}} #frac{dN}{d%s}", titleX.c_str()));
    }

    for (int iObs = 0; iObs < SPECTRAANA::kN_OBS; ++iObs) {
        if (!isValid_h1D[iObs]) continue;

        h1D[iObs]->SetMinimum(0);
    }
}

/*
 * write histograms with a particular dependence
 * use "c" as a template
 */
void spectraAnalyzer::writeObjects(TCanvas* c)
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
    //TLegend* leg = 0;
    TLatex* latex = new TLatex();
    std::vector<std::string> textLinesAll = getTextLinesAll();

    //double legHeight = -1;
    //double legWidth = -1;

    for (int iObs = 0; iObs < SPECTRAANA::kN_OBS; ++iObs) {

        if (!isValid_h1D[iObs]) continue;

        std::string label = SPECTRAANA::OBS_LABELS[iObs];

        canvasName = Form("cnv%s_%s", label.c_str(), name.c_str());
        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        h1D[iObs]->SetMarkerSize(markerSize);
        h1D[iObs]->Draw("e");
        h1D[iObs]->Write("",TObject::kOverwrite);

        latex = new TLatex();
        setLatex(latex, "NE");
        drawTextLines(latex, c, textLinesAll, "NE", textOffsetX, textOffsetY);

        setPad4Observable((TPad*) c, iObs);
        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.
    }

     if (line != 0)  line->Delete();
     latex->Delete();
}

void spectraAnalyzer::setPad4Observable(TPad* p, int iDep)
{
    TLine* line = 0;

    p->Update();

    if (iDep == SPECTRAANA::kETA) {
        // draw line for EE-EB transition
        double ECAL_boundary_1 = 1.4442;
        double ECAL_boundary_2 = 1.566;

        double yMin = p->GetUymin();
        double yMax = p->GetUymax();

        // draw lines for ECAL transition region
        std::vector<double> lineXvalues {-1*ECAL_boundary_1, ECAL_boundary_1, -1*ECAL_boundary_2, ECAL_boundary_2};
        for (std::vector<double>::const_iterator itLine = lineXvalues.begin(); itLine !=lineXvalues.end(); ++itLine) {

            line = new TLine((*itLine), yMin, (*itLine), yMax);
            line->SetLineStyle(kDashed);
            line->Draw();
        }
    }
}

void spectraAnalyzer::setPad4Observable(TPad* p)
{
    setPad4Observable(p, dep);
}

void spectraAnalyzer::setLatex(TLatex* latex, std::string position)
{
    setTextAlignment(latex, position.c_str());
    latex->SetTextFont(textFont);
    latex->SetTextSize(textSize);
}

#endif
