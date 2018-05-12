/*
 * macro to produce the pp data vs MC photon+jet JS plots
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

#include "../../Utilities/styleUtil.h"
#include "../../Utilities/th1Util.h"

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

// TLatex CMS Preliminary
float textXCMSpreliminary;
float textYCMSpreliminary;
int textAlignCMSpreliminary;
int textFontCMSpreliminary;
float textSizeCMSpreliminary;

void plotPhotonJetJSdataMC(int figureIndex, std::string inputFile);
void plot_js_pp_data_mc(std::string inputFile);
void setTH1D(int iHist, TH1D* h);
void setTGraph(int iGraph, TGraph* gr);
void setTGraphSys(int iSys, TGraph* gr);
void setCanvas(TCanvas* c);
void setLegend(TLegend* leg);
void setLatex(int iText, TLatex* latex);
void setLatexOverPad(int iText, TLatex* latex);
void setLatexCMS(TLatex* latex);
void setLatexCMSextraLabel(TLatex* latex, std::string text = "CMS");

void plotPhotonJetJSdataMC(int figureIndex, std::string inputFile)
{
    std::cout<<"running plotPhotonJetJSdataMC()"<<std::endl;
    std::cout<<"figureIndex = "<< figureIndex <<std::endl;
    std::cout<<"inputFile = "<< inputFile.c_str() <<std::endl;

    switch (figureIndex) {
        case k_js_pp_data_mc:
            plot_js_pp_data_mc(inputFile);
            break;
        default:
            break;
    }

    std::cout<<"plotPhotonJetJSdataMC() - END"<<std::endl;
}

void plot_js_pp_data_mc(std::string inputFile)
{
    std::cout<<"running plot_js_pp_data_mc()"<<std::endl;

    TFile* input  = TFile::Open(inputFile.c_str());

    // no horizontal error bars
    gStyle->SetErrorX(0);
    gStyle->SetHatchesLineWidth(3);

    windowWidth = 800;
    windowHeight = 800;
    int rows = 1;
    int columns = 1;
    float yMargin = 0.14;
    float xMargin = 0;

    logX = 0;
    logY = 0;
    leftMargin   = 0.21;
    rightMargin  = 0.04;
    bottomMargin = 0.0;
    topMargin    = 0.06;
    TCanvas* c = 0 ;

    double normCanvasWidth = calcNormCanvasWidth(columns, 0.8, leftMargin, rightMargin, xMargin);
    double normCanvasHeight = calcNormCanvasHeight(rows, 0.8, bottomMargin, topMargin, yMargin);

    c = new TCanvas(figureNames[k_js_pp_data_mc].c_str(), "", windowWidth,windowHeight);

    std::cout<<"preparing canvas : "<< c->GetName() <<std::endl;
    setCanvasSizeMargin(c, normCanvasWidth, normCanvasHeight, leftMargin, rightMargin, bottomMargin, topMargin);
    setCanvasFinal(c);
    c->cd();

    double windowHeightFraction = 0.4;
    double windowHeightScale = 1 + windowHeightFraction;
    c->SetCanvasSize(c->GetWw(), c->GetWh()*windowHeightScale);

    double yMinOffset = windowHeightFraction;

    TPad* pads[2];
    divideCanvas(c, pads, rows, columns, leftMargin, rightMargin, bottomMargin, topMargin, xMargin, yMargin, 0.8, 0.8, yMinOffset);
    //divideCanvas(c, pads, rows, columns, leftMargin, rightMargin, bottomMargin, topMargin, xMargin, yMargin, 0);

    setPadFinal(pads[0], logX, logY);

    std::string padNameTmp = Form("%s_lower", pads[0]->GetName());
    double x1_lowerPad = pads[0]->GetXlowNDC();
    double y1_lowerPad = pads[0]->GetYlowNDC();
    y1_lowerPad = 0;
    double x2_lowerPad = pads[0]->GetXlowNDC()+pads[0]->GetWNDC();
    double y2_lowerPad = pads[0]->GetYlowNDC();

    c->cd();
    pads[1] = new TPad(padNameTmp.c_str(), "", x1_lowerPad, y1_lowerPad, x2_lowerPad, y2_lowerPad);
    pads[1]->SetLeftMargin(pads[0]->GetLeftMargin());
    pads[1]->SetRightMargin(pads[0]->GetRightMargin());
    pads[1]->SetBottomMargin(yMargin * (pads[0]->GetAbsHNDC() / pads[1]->GetAbsHNDC()));
    pads[1]->SetTopMargin(0);

    setPadFinal(pads[1], pads[0]->GetLogx(), 0);  // do not draw the y-axis in log scale for the ratio histogram.

    pads[1]->Draw();
    pads[1]->cd();
    pads[1]->SetNumber(2);


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

    yMin = 0.3;
    yMax = 40;

    enum HISTLABELS {
        k_pp,
        k_MC,
        k_ratio,
        kN_HISTLABELS
    };

    histPaths = {
            "hjs_final_ppdata_corrjsrecoreco_100_200",
            "hjs_final_ppmc_ref0gen0_100_200",
            "hjs_final_ratio_ppmc_ppdata_100_200"
    };
    markerColors = {kBlack, kBlack, kBlack};
    markerStyles = {kFullCircle, kOpenCircle, kFullCircle};
    markerSizes = {1.70, 1.70, 1.70};
    lineColors = {kBlack, kRed, kBlack};
    lineTransparencies = {1.0, 1.0, 1.0};
    lineWidths = {3, 4, 3};
    fillColors = {TColor::GetColor("#6699cc"), 0, TColor::GetColor("#6699cc")};
    fillTransparencies = {0.7, 0, 0.7};
    drawOptions = {"e same", "hist same", "e same"};
    sysPaths = {
            "hjs_final_ppdata_corrjsrecoreco_100_200_systematics",
            "NULL",
            "hjs_final_ratio_ppmc_ppdata_100_200_systematics",
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

        h1Ds[i] = (TH1D*)input->Get(histPaths[i].c_str());
        h1DsSys[i] = (TH1D*)input->Get(sysPaths[i].c_str());
        setTH1D(i, h1Ds[i]);

        // set x-axis range
        h1Ds[i]->SetAxisRange(0 + 0.001, 0.3 - 0.001, "X");

        h1Ds[i]->GetXaxis()->SetLabelSize(h1Ds[i]->GetXaxis()->GetLabelSize()*1.6);
        h1Ds[i]->GetYaxis()->SetLabelSize(h1Ds[i]->GetYaxis()->GetLabelSize()*1.6);

        h1Ds[i]->GetXaxis()->SetLabelOffset(h1Ds[i]->GetXaxis()->GetLabelOffset()*2.5);
        h1Ds[i]->GetYaxis()->SetLabelOffset(h1Ds[i]->GetYaxis()->GetLabelOffset()*2);
    }
    h1Ds[k_MC]->SetLineStyle(kDashed);

    h1Ds[k_ratio]->SetYTitle("MC / Data");
    h1Ds[k_ratio]->SetMinimum(0.85);
    h1Ds[k_ratio]->SetMaximum(1.15);

    h1Ds[k_ratio]->GetXaxis()->SetLabelSize(h1Ds[k_ratio]->GetXaxis()->GetLabelSize()*1.6 * windowHeightScale);
    h1Ds[k_ratio]->GetYaxis()->SetLabelSize(h1Ds[k_ratio]->GetYaxis()->GetLabelSize()*1.4 * windowHeightScale);

    h1Ds[k_ratio]->GetXaxis()->SetLabelOffset(h1Ds[k_ratio]->GetXaxis()->GetLabelOffset()*1.8);
    h1Ds[k_ratio]->GetYaxis()->SetLabelOffset(h1Ds[k_ratio]->GetYaxis()->GetLabelOffset()*2);

    h1Ds[k_ratio]->SetTitleOffset(0.9, "X");
    h1Ds[k_ratio]->SetTitleOffset(0.6, "Y");

    h1Ds[k_ratio]->SetTitleSize(0.16, "X");
    h1Ds[k_ratio]->SetTitleSize(0.15, "Y");

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
    textSize = 32;
    textLines = {
            "p_{T}^{#gamma} > 60 GeV/c, |#eta^{#gamma}| < 1.44",
            "p_{T}^{trk} > 1 GeV/c",
            "anti-k_{T} jet R = 0.3",
            "p_{T}^{jet} > 30 GeV/c",
            "|#eta^{jet}| < 1.6",
            "#Delta#phi_{j#gamma} > #frac{7#pi}{8}"
    };
    int nTextLines = textLines.size();
    textX = 0.92;
    textYs.resize(nTextLines, 0.64);
    TLatex* latex = 0;
    for (int i = 0; i < nTextLines; ++i) {
        latex = new TLatex();
        textYs[i] = textYs[0] - i*0.065;
        setLatex(i, latex);
        latex->Draw();
    }

    textAlign = 11;
    textFont = 43;
    textSize = 32;
    textLines = {
            "#sqrt{s} = 5.02 TeV, pp 27.4 pb^{-1}"
    };
    nTextLines = textLines.size();
    textX = 0.52;
    textYs.clear();
    textYs.resize(nTextLines, 0.95);
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

    bool isPreliminary = true;
    if (isPreliminary) {
        textXCMSpreliminary = 0.25;
        textYCMSpreliminary = 0.78;
        textAlignCMSpreliminary = 11;
        textFontCMSpreliminary = 52;
        textSizeCMSpreliminary = 0.06;
        latex = new TLatex();
        setLatexCMSextraLabel(latex, "Preliminary");
        latex->Draw();
    }

    bool isSupplementary = false;
    if (isSupplementary) {
        textXCMSpreliminary = 0.25;
        textYCMSpreliminary = 0.72;
        textAlignCMSpreliminary = 11;
        textFontCMSpreliminary = 52;
        textSizeCMSpreliminary = 0.06;
        latex = new TLatex();
        setLatexCMSextraLabel(latex, "Supplementary");
        latex->Draw();
    }

    c->Update();

    c->cd();
    TPad* emptyBox = 0;
    emptyBox = new TPad("box1", "", c->GetX1()+leftMargin-0.10, pads[0]->GetYlowNDC()-0.015,
                                    c->GetX1()+leftMargin-0.017, pads[0]->GetYlowNDC()+0.035);
    emptyBox->Draw();
    /*
    emptyBox = new TPad("box2", "", c->GetX1()+leftMargin-0.10, pads[1]->GetYlowNDC()+0.081,
                                    c->GetX1()+leftMargin-0.001, pads[1]->GetYlowNDC()+0.12);
    emptyBox->Draw();
    */

    c->cd(2);
    TLine* line = new TLine(gPad->GetUxmin(), 1, gPad->GetUxmax(), 1);
    line->SetLineWidth(3);
    line->SetLineStyle(kDashed);
    line->Draw();

    c->cd();
    c->Update();
    c->SaveAs(Form("%s.pdf", c->GetName()));
    c->SaveAs(Form("%s.png", c->GetName()));
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

void setLatexCMSextraLabel(TLatex* latex, std::string text)
{
    latex->SetTextAlign(textAlignCMSpreliminary);
    latex->SetTextFont(textFontCMSpreliminary);
    latex->SetTextSize(textSizeCMSpreliminary);

    latex->SetNDC();
    latex->SetText(textXCMSpreliminary, textYCMSpreliminary, text.c_str());
}

int main(int argc, char** argv)
{
    if (argc == 3) {
        plotPhotonJetJSdataMC(atoi(argv[1]), argv[2]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./plotPhotonJetJSdataMC.exe <figureIndex> <inputFile>"
                << std::endl;
        return 1;
    }
}
