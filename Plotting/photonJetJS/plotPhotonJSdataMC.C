/*
 * Stand-alone macro to produce the pp data vs MC photon+jet JS plots
 */
#include <TFile.h>
#include <TDirectoryFile.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TAxis.h>
#include <TGaxis.h>
#include <TH1D.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TLine.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TMath.h>
#include <TStyle.h>
#include <TColor.h>

#include <vector>
#include <string>
#include <iostream>

//#include "../../Utilities/styleUtil.h"

enum FIGURE{
    k_js_pp_data_mc,
    kN_FIGURES
};

std::string figureNames[kN_FIGURES] = {
        "js_pp_data_mc"
};

// Canvas
int windowWidth;
int windowHeight;
int logX;
int logY;
float leftMargin;
float rightMargin;
float bottomMargin;
float topMargin;
// TH1D
std::string xTitle;
std::string yTitle;
float xTitleSize;
float yTitleSize;
float xTitleOffset;
float yTitleOffset;
int xTitleFont;
int yTitleFont;
double yMin;
double yMax;
std::vector<std::string> histPaths;
std::vector<int> markerColors;
std::vector<int> markerStyles;
std::vector<float> markerSizes;
std::vector<int> lineColors;
std::vector<float> lineTransparencies;
std::vector<int> lineWidths;
std::vector<int> fillColors;
std::vector<float> fillTransparencies;
std::vector<std::string> drawOptions;
std::vector<std::string> sysPaths;
std::vector<bool> sysUseRelUnc;
std::vector<int> sysColors;
std::vector<float> sysTransparencies;
std::vector<int> sysFillStyles;
// TGraph
std::vector<std::string> graphPaths;
std::vector<int> graphMarkerColors;
std::vector<float> graphMarkerTransparencies;
std::vector<int> graphMarkerStyles;
std::vector<float> graphMarkerSizes;
std::vector<int> graphLineColors;
std::vector<float> graphLineTransparencies;
std::vector<int> graphLineWidths;
std::vector<int> graphLineStyles;
std::vector<int> graphFillColors;
std::vector<float> graphFillTransparencies;
std::vector<std::string> graphDrawOptions;
// TLegend
float legendX1;
float legendY1;
float legendWidth;
float legendHeight;
float legendMargin;
std::vector<std::string> legendEntryTexts;
std::vector<std::string> legendEntryOptions;
// TLatex
float textX;
std::vector<float> textYs;
int textAlign;
int textFont;
float textSize;
std::vector<std::string> textLines;
// TLatex over pad
std::vector<float> textXsOverPad;
float textYOverPad;
std::vector<int> textAlignsOverPad;
int textFontOverPad;
float textSizeOverPad;
std::vector<std::string> textOverPadLines;
// TLatex CMS
float textXCMS;
float textYCMS;
int textAlignCMS;
int textFontCMS;
float textSizeCMS;

// TLatex CMS Projection
float textXCMSProj;
float textYCMSProj;
int textAlignCMSProj;
int textFontCMSProj;
float textSizeCMSProj;

void plotPhotonJSdataMC(int figureIndex, std::string inputFile);
void plot_js_pp_data_mc(std::string inputFile);
void setTH1D(int iHist, TH1D* h);
void setTGraph(int iGraph, TGraph* gr);
void setTGraphSys(int iSys, TGraph* gr);
void setCanvas(TCanvas* c);
void setLegend(TLegend* leg);
void setLatex(int iText, TLatex* latex);
void setLatexOverPad(int iText, TLatex* latex);
void setLatexCMS(TLatex* latex);
void setLatexCMSProj(TLatex* latex, std::string text = "CMS");
double calcNormCanvasWidth(int columns = 1, float leftMargin = 0.1, float rightMargin = 0.1, float xMargin = 0.01);
double calcNormCanvasHeight(int rows = 1, float bottomMargin = 0.1, float topMargin = 0.1, float yMargin = 0.01);
void divideCanvas(TCanvas* c, int rows = 1, int columns = 1, float leftMargin = 0.1, float rightMargin = 0.1, float bottomMargin = 0.1, float topMargin = 0.1, float xMargin = 0.01, float yMargin = 0.01, float yMinOffSet = 0);
void divideCanvas(TCanvas* c, TPad* pads[], int rows = 1, int columns = 1, float leftMargin = 0.1, float rightMargin = 0.1, float bottomMargin = 0.1, float topMargin = 0.1, float xMargin = 0.01, float yMargin = 0.01, float yMinOffSet = 0);
void setSysUncBox(TGraph* gr, TH1* h, TH1* hSys, int bin, bool doRelUnc = false, double binWidth = -1, double binWidthScale = 1);
void drawSysUncBoxes(TGraph* gr, TH1* h, TH1* hSys, bool doRelUnc = false, double binWidth = -1, double binWidthScale = 1);

void plotPhotonJSdataMC(int figureIndex, std::string inputFile)
{
    std::cout<<"running plotPhotonJSdataMC()"<<std::endl;
    std::cout<<"figureIndex = "<< figureIndex <<std::endl;
    std::cout<<"inputFile = "<< inputFile.c_str() <<std::endl;

    switch (figureIndex) {
        case k_js_pp_data_mc:
            plot_js_pp_data_mc(inputFile);
            break;
        default:
            break;
    }

    std::cout<<"plotPhotonJSdataMC() - END"<<std::endl;
}

void plot_js_pp_data_mc(std::string inputFile)
{
    std::cout<<"running plot_js_pp_data_mc()"<<std::endl;

    TFile* input  = TFile::Open(inputFile.c_str());

    // no horizontal error bars
    gStyle->SetErrorX(0);
    gStyle->SetHatchesLineWidth(3);

    int rows = 2;
    int columns = 1;
    float yMargin = 0;
    float xMargin = 0;
    double normCanvasHeight = calcNormCanvasHeight(rows, bottomMargin, topMargin, yMargin);
    double normCanvasWidth = calcNormCanvasWidth(columns, leftMargin, rightMargin, xMargin);

    windowWidth = 800;
    windowHeight = 700;
    logX = 0;
    logY = 0;
    leftMargin   = 0.21;
    rightMargin  = 0.04;
    bottomMargin = 0.18;
    topMargin    = 0.08;
    TCanvas* c = 0 ;

    c = new TCanvas(figureNames[k_js_pp_data_mc].c_str(), "", windowWidth, windowHeight);

    std::cout<<"preparing canvas : "<< c->GetName() <<std::endl;
    c->SetCanvasSize(windowWidth*normCanvasWidth, windowHeight*normCanvasHeight);
    c->SetLeftMargin(leftMargin);
    c->SetRightMargin(rightMargin);
    c->SetBottomMargin(bottomMargin);
    c->SetTopMargin(topMargin);

    TPad* pads[2];
    divideCanvas(c, pads, rows, columns, leftMargin, rightMargin, bottomMargin, topMargin, xMargin, yMargin, 0);

    for (int i = 0; i < 2; ++i) {
        pads[i]->SetBorderMode(0);
        pads[i]->SetBorderSize(0);
        pads[i]->SetFrameBorderMode(0);
        pads[i]->SetFrameLineColor(0);

        // put ticks to upper and right part of the axis.
        pads[i]->SetTickx(1);
        pads[i]->SetTicky(1);
    }

    xTitle = "r";
    yTitle = "#rho(r)";

    xTitleSize = 0.075;
    yTitleSize = 0.075;
    xTitleOffset = 1.05;
    yTitleOffset = 1.25;
    xTitleFont = 42;
    yTitleFont = 42;

    yMin = 0.2;
    yMax = 60;

    enum HISTLABELS {
        k_pp,
        k_MC,
        k_ratio,
        kN_HISTLABELS
    };

    histPaths = {
            "hjs_final_ppdata_corrjsrecoreco_100_200",
            "hjs_final_ppmc_ref0gen0_100_200",
            "hjs_final_ratio_ppdata_ppmc_100_200"
    };
    markerColors = {kBlack, kBlack, kBlack};
    markerStyles = {kFullCircle, kOpenCircle, kFullCircle};
    markerSizes = {1.70, 1.70, 1.70};
    lineColors = {kBlack, kRed, kBlack};
    lineTransparencies = {1.0, 1.0, 1.0};
    lineWidths = {3, 3, 3};
    fillColors = {TColor::GetColor("#6699cc"), 0, TColor::GetColor("#6699cc")};
    fillTransparencies = {0.7, 0, 0.7};
    drawOptions = {"e same", "hist same", "e same"};
    sysPaths = {
            "hjs_final_ppdata_corrjsrecoreco_100_200_systematics",
            "NULL",
            "hjs_final_ratio_ppdata_ppmc_100_200_systematics",
    };
    sysUseRelUnc = {false, false, false};
    sysColors = {TColor::GetColor("#6699cc"), 0, TColor::GetColor("#6699cc")};
    sysTransparencies = {0.7, 0.7, 0.7};
    sysFillStyles = {1001, 1001, 1001};

    int nHistPaths = histPaths.size();
    std::vector<TH1D*> h1Ds(nHistPaths, 0);
    std::vector<TH1D*> h1DsSys(nHistPaths, 0);
    TGraph* gr = 0;
    TH1D* hTmp = 0;
    for (int i = 0; i < nHistPaths; ++i) {

        if (i < 2) {
            h1Ds[i] = (TH1D*)input->Get(histPaths[i].c_str());
            h1DsSys[i] = (TH1D*)input->Get(sysPaths[i].c_str());
        }
        else if (i == k_ratio) {
            h1Ds[i] = (TH1D*)h1Ds[k_pp]->Clone(histPaths[i].c_str());
            h1Ds[i]->Divide(h1Ds[k_MC]);

            h1DsSys[i] = (TH1D*)h1DsSys[k_pp]->Clone(sysPaths[i].c_str());
            h1DsSys[i]->Divide(h1Ds[k_pp]);
            h1DsSys[i]->Multiply(h1Ds[i]);
        }
        setTH1D(i, h1Ds[i]);

        // set x-axis range
        h1Ds[i]->SetAxisRange(0 + 0.001, 0.3 - 0.001, "X");

        if (i < 2) {
            h1Ds[i]->GetXaxis()->SetLabelSize(h1Ds[i]->GetXaxis()->GetLabelSize()*1.6);
            h1Ds[i]->GetYaxis()->SetLabelSize(h1Ds[i]->GetYaxis()->GetLabelSize()*1.6);

            h1Ds[i]->GetXaxis()->SetLabelOffset(h1Ds[i]->GetXaxis()->GetLabelOffset()*2.5);
            h1Ds[i]->GetYaxis()->SetLabelOffset(h1Ds[i]->GetYaxis()->GetLabelOffset()*2);
        }
    }
    h1Ds[k_ratio]->SetYTitle("DATA / MC");
    h1Ds[k_ratio]->SetMinimum(0.8);
    h1Ds[k_ratio]->SetMaximum(1.2);

    // draw histograms
    for (int i = 0; i < nHistPaths; ++i) {

        c->cd(1);
        if (i == k_ratio) c->cd(2);

        if (i == 0 || i == k_ratio) {
            hTmp = (TH1D*)h1Ds[i]->Clone(Form("%s_tmpDraw", h1Ds[i]->GetName()));
            hTmp->Draw("e");
        }

        if (h1DsSys[i] != 0) {
            gr = new TGraph();
            setTGraphSys(i, gr);
            drawSysUncBoxes(gr, h1Ds[i], h1DsSys[i], sysUseRelUnc[i]);
        }

        h1Ds[i]->Draw(drawOptions[i].c_str());
        if (i == k_MC) {
            // redraw data points
            h1Ds[k_pp]->Draw(drawOptions[k_pp].c_str());
        }
    }

    c->cd(1);
    gPad->SetLogy(1);

    legendX1 = 0.62;
    legendY1 = 0.74;
    legendWidth = 0.54;
    legendHeight = 0.16;
    legendMargin = 0.15;
    legendEntryTexts = {
            "pp",
            "PYTHIA 8"
    };
    legendEntryOptions = {
            "pf",
            "l"
    };
    TLegend* leg = new TLegend();

    hTmp = (TH1D*)h1Ds[k_pp]->Clone(Form("%s_tmp", h1Ds[k_pp]->GetName()));
    hTmp->SetLineWidth(0);
    leg->AddEntry(hTmp, legendEntryTexts[k_pp].c_str(), legendEntryOptions[k_pp].c_str());
    hTmp = (TH1D*)h1Ds[k_MC]->Clone(Form("%s_tmp", h1Ds[k_MC]->GetName()));
    leg->AddEntry(hTmp, legendEntryTexts[k_MC].c_str(), legendEntryOptions[k_MC].c_str());

    setLegend(leg);
    leg->Draw();

    c->Update();
    c->cd(1);
    textAlign = 31;
    textFont = 43;
    textSize = 26;
    textLines = {
            "p_{T}^{#gamma} > 60 GeV/c, |#eta^{#gamma}| < 1.44",
            "anti-k_{T} jet R = 0.3",
            "p_{T}^{jet} > 30 GeV/c, |#eta^{jet}| < 1.6",
            "#Delta#phi_{j#gamma} > #frac{7#pi}{8}",
            "p_{T}^{trk} > 1 GeV/c"
    };
    int nTextLines = textLines.size();
    textX = 0.92;
    textYs.resize(nTextLines, 0.68);
    TLatex* latex = 0;
    for (int i = 0; i < nTextLines; ++i) {
        latex = new TLatex();
        textYs[i] = textYs[0] - i*0.065;
        setLatex(i, latex);
        latex->Draw();
    }

    textAlign = 11;
    textFont = 43;
    textSize = 30;
    textLines = {
            "#sqrt{s} = 5.02 TeV, pp 27.4 pb^{-1}"
    };
    nTextLines = textLines.size();
    textX = 0.38;
    textYs.clear();
    textYs.resize(nTextLines, 0.94);
    latex = 0;
    for (int i = 0; i < nTextLines; ++i) {
        latex = new TLatex();
        textYs[i] = textYs[0] - i*0.08;
        setLatex(i, latex);
        latex->Draw();
    }

    textXCMS = 0.25;
    textYCMS = 0.84;
    textAlignCMS = 11;
    textFontCMS = 61;
    textSizeCMS = 0.07;
    latex = new TLatex();
    setLatexCMS(latex);
    latex->Draw();

    textXCMSProj = 0.25;
    textYCMSProj = 0.78;
    textAlignCMSProj = 11;
    textFontCMSProj = 52;
    textSizeCMSProj = 0.06;
    latex = new TLatex();
    setLatexCMSProj(latex, "Preliminary");
    latex->Draw();

    c->Update();

    c->cd();
    TPad* emptyBox = 0;
    emptyBox = new TPad("box1", "", c->GetX1()+leftMargin-0.10, pads[0]->GetYlowNDC()-0.025,
                                    c->GetX1()+leftMargin-0.001, pads[0]->GetYlowNDC()+0.025);
    emptyBox->Draw();

    emptyBox = new TPad("box2", "", c->GetX1()+leftMargin-0.10, pads[1]->GetYlowNDC()+0.081,
                                    c->GetX1()+leftMargin-0.001, pads[1]->GetYlowNDC()+0.12);
    emptyBox->Draw();

    c->cd(2);
    TLine* line = new TLine(gPad->GetUxmin(), 1, gPad->GetUxmax(), 1);
    line->SetLineWidth(3);
    line->SetLineStyle(kDashed);
    line->Draw();

    c->cd();
    c->Update();
    c->SaveAs(Form("%s.pdf", c->GetName()));
    c->Close();         // do not use Delete() for TCanvas.

    std::cout<<"Closing the input file"<<std::endl;
    input->Close();

    std::cout<<"running plot_js_pp_data_mc() - END"<<std::endl;
}

void setTH1D(int iHist, TH1D* h)
{
    h->SetTitle("");
    h->SetStats(false);
    h->SetXTitle(xTitle.c_str());
    h->SetYTitle(yTitle.c_str());
    h->SetTitleSize(xTitleSize, "X");
    h->SetTitleSize(yTitleSize, "Y");
    h->SetTitleOffset(xTitleOffset, "X");
    h->SetTitleOffset(yTitleOffset, "Y");
    h->SetTitleFont(xTitleFont, "X");
    h->SetTitleFont(yTitleFont, "Y");
    h->GetXaxis()->CenterTitle();
    h->GetYaxis()->CenterTitle();
    h->SetMinimum(yMin);
    h->SetMaximum(yMax);
    h->SetMarkerColor(markerColors[iHist]);
    h->SetMarkerStyle(markerStyles[iHist]);
    h->SetMarkerSize(markerSizes[iHist]);
    h->SetLineColor(lineColors[iHist]);
    h->SetLineColorAlpha(lineColors[iHist], lineTransparencies[iHist]);
    h->SetLineStyle(kSolid);
    h->SetLineWidth(lineWidths[iHist]);
    h->SetFillColor(fillColors[iHist]);
    h->SetFillColorAlpha(fillColors[iHist], fillTransparencies[iHist]);
}

void setTGraph(int iGraph, TGraph* gr)
{
    gr->SetMarkerColor(graphMarkerColors[iGraph]);
    gr->SetMarkerColorAlpha(graphMarkerColors[iGraph], graphMarkerTransparencies[iGraph]);
    gr->SetMarkerStyle(graphMarkerStyles[iGraph]);
    gr->SetMarkerSize(graphMarkerSizes[iGraph]);
    gr->SetLineColor(graphLineColors[iGraph]);
    gr->SetLineColorAlpha(graphLineColors[iGraph], graphLineTransparencies[iGraph]);
    gr->SetLineWidth(graphLineWidths[iGraph]);
    gr->SetLineStyle(graphLineStyles[iGraph]);
    gr->SetFillColor(graphFillColors[iGraph]);
    gr->SetFillColorAlpha(graphFillColors[iGraph], graphFillTransparencies[iGraph]);
}

void setTGraphSys(int iSys, TGraph* gr)
{
    gr->SetFillColor(sysColors[iSys]);
    gr->SetFillColorAlpha(sysColors[iSys], sysTransparencies[iSys]);
    gr->SetFillStyle(sysFillStyles[iSys]);
}

void setCanvas(TCanvas* c)
{
    c->SetLogx(logX);
    c->SetLogy(logY);

    float defaultMargin = 0.1;
    double normWidth  = (1 - defaultMargin*2 + leftMargin + rightMargin);
    double normHeight = (1 - defaultMargin*2 + bottomMargin + topMargin);

    // https://root.cern.ch/doc/master/TCanvas_8h_source.html#l00058
    UInt_t width = c->GetWindowWidth();     // fWindowWidth  : Width of window (including borders, etc.)
    UInt_t height = c->GetWindowHeight();   // fWindowHeight : Height of window (including menubar, borders, etc.)

    // assume the function is called in batch mode, so use SetCanvasSize() instead of SetWindowSize()
    c->SetCanvasSize(width*normWidth, height*normHeight);
    c->SetLeftMargin(leftMargin);
    c->SetRightMargin(rightMargin);
    c->SetBottomMargin(bottomMargin);
    c->SetTopMargin(topMargin);

    c->SetBorderMode(0);
    c->SetBorderSize(0);
    c->SetFrameBorderMode(0);
    c->SetFrameLineColor(0);

    // put ticks to upper and right part of the axis.
    c->SetTickx(1);
    c->SetTicky(1);
}

void setLegend(TLegend* leg)
{
    // make legend transparent
    leg->SetFillColor(-1);
    leg->SetFillStyle(4000);
    leg->SetBorderSize(0);

    leg->SetX1(legendX1);
    leg->SetX2(legendX1+legendWidth);
    leg->SetY1(legendY1);
    leg->SetY2(legendY1+legendHeight);
    leg->SetMargin(legendMargin);
}

void setLatex(int iText, TLatex* latex)
{
    latex->SetTextAlign(textAlign);
    latex->SetTextFont(textFont);
    latex->SetTextSize(textSize);

    latex->SetNDC();
    latex->SetText(textX, textYs[iText], textLines[iText].c_str());
}

void setLatexOverPad(int iText, TLatex* latex)
{
    latex->SetTextAlign(textAlignsOverPad[iText]);
    latex->SetTextFont(textFontOverPad);
    latex->SetTextSize(textSizeOverPad);

    latex->SetNDC();
    latex->SetText(textXsOverPad[iText], textYOverPad, textOverPadLines[iText].c_str());
}

void setLatexCMS(TLatex* latex)
{
    latex->SetTextAlign(textAlignCMS);
    latex->SetTextFont(textFontCMS);
    latex->SetTextSize(textSizeCMS);

    latex->SetNDC();
    latex->SetText(textXCMS, textYCMS, "CMS");
}

void setLatexCMSProj(TLatex* latex, std::string text)
{
    latex->SetTextAlign(textAlignCMSProj);
    latex->SetTextFont(textFontCMSProj);
    latex->SetTextSize(textSizeCMSProj);

    latex->SetNDC();
    latex->SetText(textXCMSProj, textYCMSProj, text.c_str());
}

/*
 * calculate the width of a TCanvas in a normalization scheme where the width is 1 for
 * a canvas with a columns = 1, bottomMargin + topMargin = 2*defaultMargin and xMargin = 0
 */
double calcNormCanvasWidth(int columns, float leftMargin, float rightMargin, float xMargin)
{
    double defaultMargin = 0.1;
    double padWidth = (1 - defaultMargin*2 + xMargin);

    float x_max[columns];
    x_max[0] = padWidth + leftMargin - xMargin/2;   // left margin is inside the width of leftmost panel
    for (int i = 1; i < columns; ++i) {
        x_max[i] = x_max[i-1] + padWidth;
    }
    x_max[columns-1] += rightMargin - xMargin/2;

    return x_max[columns-1];
}

/*
 * calculate the height of a TCanvas in a normalization scheme where the height is 1 for
 * a canvas with a rows = 1, bottomMargin + topMargin = 2*defaultMargin and yMargin = 0
 */
double calcNormCanvasHeight(int rows, float bottomMargin, float topMargin, float yMargin)
{
    double defaultMargin = 0.1;
    double padHeight = (1 - defaultMargin*2 + yMargin);

    float y_min[rows], y_max[rows];
    y_min[rows-1] = 0;
    y_max[rows-1] = padHeight + bottomMargin - yMargin/2;  // bottom margin is inside the height of bottom panel
    for (int i = rows - 2; i >= 0; --i) {
        y_min[i] = y_max[i+1];
        y_max[i] = y_min[i] + padHeight;
    }
    y_max[0] += topMargin - yMargin/2;

    return y_max[0];
}

void divideCanvas(TCanvas* c, int rows, int columns, float leftMargin, float rightMargin, float bottomMargin, float topMargin, float xMargin, float yMargin, float yMinOffset)
{
    TPad* pads[rows*columns];
    divideCanvas(c, pads, rows, columns, leftMargin, rightMargin, bottomMargin, topMargin, xMargin, yMargin, yMinOffset);
}

void divideCanvas(TCanvas* c, TPad* pads[], int rows, int columns, float leftMargin, float rightMargin, float bottomMargin, float topMargin, float xMargin, float yMargin, float yMinOffset)
{
    c->Clear();

    double normPadWidth = calcNormCanvasWidth(1, leftMargin, rightMargin, xMargin);
    double normPadHeight = calcNormCanvasHeight(1, bottomMargin, topMargin, yMargin);

    float x_min[columns], x_max[columns];
    x_min[0] = 0;
    x_max[0] = normPadWidth + leftMargin - xMargin/2;   // left margin is inside the width of leftmost panel
    for (int i = 1; i < columns; ++i) {
        x_min[i] = x_max[i-1];
        x_max[i] = x_max[i-1] + normPadWidth;
    }
    x_max[columns-1] += rightMargin - xMargin/2;

    float y_min[rows], y_max[rows];
    y_min[rows-1] = yMinOffset;
    y_max[rows-1] = normPadHeight + bottomMargin - yMargin/2;  // bottom margin is inside the height of bottom panel
    for (int i = rows - 2; i >= 0; --i) {
        y_min[i] = y_max[i+1]+yMinOffset;
        y_max[i] = y_min[i] + normPadHeight;
    }
    y_max[0] += topMargin - yMargin/2;

    double normCanvasWidth = x_max[columns-1];
    double normCanvasHeight = y_max[0];
    // normalize the coordinates such that x_max[columns-1] = 1 and y_max[0] = 1
    for (int i = 0; i < columns; ++i) {
        x_min[i] /= normCanvasWidth;
        x_max[i] /= normCanvasWidth;
    }
    for (int i = 0; i < rows; ++i) {
        y_min[i] /= normCanvasHeight;
        y_max[i] /= normCanvasHeight;
    }

    for (int i=0; i<rows; i++) {
        for (int j=0; j<columns; j++) {
            c->cd();
            int ij = i*columns+j;
            pads[ij] = new TPad(Form("pad_%d_%d", i, j), Form("pad_%d_%d", i, j), x_min[j], y_min[i], x_max[j], y_max[i]);

            if (i == 0) pads[ij]->SetTopMargin(topMargin);
            else pads[ij]->SetTopMargin(yMargin/2);
            if (i == rows - 1) pads[ij]->SetBottomMargin(bottomMargin);
            else pads[ij]->SetBottomMargin(yMargin/2);
            if (j == 0) pads[ij]->SetLeftMargin(leftMargin);
            else pads[ij]->SetLeftMargin(xMargin/2);
            if (j == columns - 1) pads[ij]->SetRightMargin(rightMargin);
            else pads[ij]->SetRightMargin(xMargin/2);

            pads[ij]->Draw();
            pads[ij]->cd();
            pads[ij]->SetNumber(ij+1);
        }
    }
}

void setSysUncBox(TGraph* gr, TH1* h, TH1* hSys, int bin, bool doRelUnc, double binWidth, double binWidthScale)
{
   double val = h->GetBinContent(bin);
   double x   = h->GetBinCenter(bin);
   int binSys = hSys->FindBin(x);

   double error = TMath::Abs(hSys->GetBinContent(binSys));             // if the uncertainty is calculated using differences
   if (doRelUnc) error = TMath::Abs(val * hSys->GetBinContent(binSys));    // if the uncertainty is calculated using ratios

   std::string hSysName = hSys->GetName();

   if (binWidth < 0) {
     binWidth = h->GetBinLowEdge(bin+1) - h->GetBinLowEdge(bin);
   }

   double errorLow = val - error;
   double errorUp = val + error;
   if (errorLow < h->GetMinimum())  errorLow = h->GetMinimum();
   if (errorUp  > h->GetMaximum())  errorUp = h->GetMaximum();

   gr->SetPoint(0, x - (binWidth/2)*binWidthScale, errorLow);
   gr->SetPoint(1, x + (binWidth/2)*binWidthScale, errorLow);
   gr->SetPoint(2, x + (binWidth/2)*binWidthScale, errorUp);
   gr->SetPoint(3, x - (binWidth/2)*binWidthScale, errorUp);
}

/*
 * draws SysUnc boxes using TGraph objects instead of TBox. TBox objects with transparent fill do not
 * show up in ".png" files. Hence, use this version of the function to produce transparent boxes in ".png" files
 * if doRelUnc == true, then draw SysUnc. boxes using relative values, otherwise draw it using absolute values
 */
void drawSysUncBoxes(TGraph* gr, TH1* h, TH1* hSys, bool doRelUnc, double binWidth, double binWidthScale)
{
    int nBins = h->GetNbinsX();
    for (int i = 1; i <= nBins; ++i) {
        if (h->GetBinError(i) == 0) continue;
        if (h->GetBinContent(i) < h->GetMinimum()) continue;
        if (h->GetBinContent(i) > h->GetMaximum()) continue;

        setSysUncBox(gr, h, hSys, i, doRelUnc, binWidth, binWidthScale);
        gr->DrawClone("f");
    }
}

int main(int argc, char** argv)
{
    if (argc == 3) {
        plotPhotonJSdataMC(atoi(argv[1]), argv[2]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./plotPhotonJSdataMC.exe <figureIndex> <inputFile>"
                << std::endl;
        return 1;
    }
}
