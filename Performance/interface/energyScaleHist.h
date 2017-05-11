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
    kN_OBS
};

const std::string OBS_LABELS[kN_OBS] = {"eScale", "eRes"};

};

class energyScaleHist {
public :
    energyScaleHist(){

        for (int i=0; i<ENERGYSCALE::kN_DEPS; ++i) {
            ranges[i][0] = 0;
            ranges[i][1] = -1;  // no upper bound
        }

        name = "";
        title = "";
        titleX = "";
        titleOffsetX = 1;
        titleOffsetY = 1;

        xMin = {0, 0};
        xMax = {-1, -1};
        yMin = {0, 0};
        yMax = {-1, -1};

        h2Dinitialized = false;
        hInitialized = false;
        h2DcorrInitialized = false;
    };
    ~energyScaleHist(){};

    void FillH2D(double energyScale, double x, float eta = -999, float genPt = -1, float recoPt = -1, int hiBin = -1);
    void FillH(double energyScale, float eta = -999, float genPt = -1, float recoPt = -1, int hiBin = -1);
    void FillH2Dcorr(float genPt, float recoPt, float eta = -999, int hiBin = -1);

    bool insideRange(float eta = -999, float genPt = -1, float recoPt = -1, int hiBin = -1);

    std::string getRangeTextEta();
    std::string getRangeTextGenPt();
    std::string getRangeTextRecoPt();
    std::string getRangeTextHiBin();

    void prepareTitle();

    void postLoop();
    void writeObjects(TCanvas* c);

    TH2D* h2D;
    TH1D* h1D[2];       // h1D[0] = energy scale histogram
                        // h1D[1] = energy resolution histogram

    TH1D* h;            // energy scale distribution
    TH2D* h2Dcorr;      // reco pt vs. gen pt correlation histogram.

    bool h2Dinitialized;
    bool hInitialized;
    bool h2DcorrInitialized;

    std::string name;   // this is basically histogram name excluding the "h1D"/"h2D" prefix
    std::string title;
    std::string titleX;
    float titleOffsetX;
    float titleOffsetY;

    std::vector<float> xMin;
    std::vector<float> xMax;
    std::vector<float> yMin;
    std::vector<float> yMax;

    // range of oberservables for which the histograms are made.
    // histograms are filled if range[i][0] <= observable < range[i][1]
    float ranges[ENERGYSCALE::kN_DEPS][2];
};

void energyScaleHist::FillH2D(double energyScale, double x, float eta, float genPt, float recoPt, int hiBin)
{
    if (h2Dinitialized && insideRange(eta, genPt, recoPt, hiBin))
        h2D->Fill(x, energyScale);
}

void energyScaleHist::FillH(double energyScale, float eta, float genPt, float recoPt, int hiBin)
{
    // make sure to fill the histogram if no explicit kinematic range is specified.
    if (hInitialized && insideRange(eta, genPt, recoPt, hiBin))
        h->Fill(energyScale);
}

void energyScaleHist::FillH2Dcorr(float genPt, float recoPt, float eta, int hiBin)
{
    if (h2DcorrInitialized && insideRange(eta, -1, -1, hiBin))
        h2Dcorr->Fill(genPt, recoPt);
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

    TObjArray aSlices;
    h2D->FitSlicesY(0,0,-1,0,"Q LL m", &aSlices);

    // energy scale
    h1D[0] = (TH1D*)aSlices.At(1)->Clone(Form("h1D_%s_%s", ENERGYSCALE::OBS_LABELS[0].c_str(), name.c_str()));
    h1D[0]->SetTitle(title.c_str());
    h1D[0]->SetXTitle(titleX.c_str());
    setTH1_energyScale(h1D[0], titleOffsetX, titleOffsetY);
    if (yMax.at(ENERGYSCALE::kESCALE) > yMin.at(ENERGYSCALE::kESCALE))
        h1D[0]->SetAxisRange(yMin.at(ENERGYSCALE::kESCALE), yMax.at(ENERGYSCALE::kESCALE), "Y");

    // width of energy scale
    h1D[1] = (TH1D*)aSlices.At(2)->Clone(Form("h1D_%s_%s", ENERGYSCALE::OBS_LABELS[1].c_str(), name.c_str()));
    h1D[1]->SetTitle(title.c_str());
    h1D[1]->SetXTitle(titleX.c_str());
    setTH1_energyWidth(h1D[1], titleOffsetX, titleOffsetY);
    if (yMax.at(ENERGYSCALE::kERES) > yMin.at(ENERGYSCALE::kERES))
        h1D[1]->SetAxisRange(yMin.at(ENERGYSCALE::kERES), yMax.at(ENERGYSCALE::kERES), "Y");
}

/*
 * use "c" as a template
 */
void energyScaleHist::writeObjects(TCanvas* c)
{
    // write histograms with a particular dependence
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
    TLine line(x1, 1, x2,1);
    line.SetLineStyle(kDashed);
    line.Draw();
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
}

#endif
