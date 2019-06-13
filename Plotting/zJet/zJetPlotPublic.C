/*
 * Stand-alone macro to reproduce the public plots for Z+jet paper, HIN-15-013
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

enum FIGURE{
    k_xjz,
    k_dphijz,
    k_rjz,
    k_xjzMean,
    k_xjz_pp_Theory,
    k_xjz_pbpb_Theory,
    k_dphijz_pp_Theory,
    k_dphijz_pbpb_Theory,
    k_rjz_pp_Theory,
    k_rjz_pbpb_Theory,
    k_xjzMean_pp_Theory,
    k_xjzMean_pbpb_Theory,
    k_M_Zee_pbpb,
    k_M_Zmm_pbpb,
    k_bkgSubtraction_dphijz_pbpb,
    k_xjz_xjg_pp,
    k_xjz_xjg_pbpb,
    k_xjzMean_xjgMean_pp,
    k_xjzMean_xjgMean_pbpb,
    kN_FIGURES
};

std::string figureNames[kN_FIGURES] = {"zJet_xjz", "zJet_dphijz", "zJet_rjz", "zJet_xjzMean",
        "zJet_xjz_pp_Theory", "zJet_xjz_pbpb_Theory",
        "zJet_dphijz_pp_Theory", "zJet_dphijz_pbpb_Theory",
        "zJet_rjz_pp_Theory", "zJet_rjz_pbpb_Theory",
        "zJet_xjzMean_pp_Theory", "zJet_xjzMean_pbpb_Theory",
        "zJet_M_Zee_pbpb", "zJet_M_Zmm_pbpb",
        "zJet_bkgSubtraction_dphijz_pbpb",
        "zJet_xjz_xjg_pp", "zJet_xjz_xjg_pbpb",
        "zJet_xjzMean_xjgMean_pp", "zJet_xjzMean_xjgMean_pbpb"};

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

void zJetPlotPublic(int figureIndex, std::string inputFile);
void zJetPlot_xjz(std::string inputFile);
void zJetPlot_dphijz(std::string inputFile);
void zJetPlot_rjz(std::string inputFile);
void zJetPlot_xjzMean(std::string inputFile);
void zJetPlot_xjz_pp_Theory(std::string inputFile);
void zJetPlot_xjz_pbpb_Theory(std::string inputFile);
void zJetPlot_dphijz_pp_Theory(std::string inputFile);
void zJetPlot_dphijz_pbpb_Theory(std::string inputFile);
void zJetPlot_rjz_pp_Theory(std::string inputFile);
void zJetPlot_rjz_pbpb_Theory(std::string inputFile);
void zJetPlot_xjzMean_pp_Theory(std::string inputFile);
void zJetPlot_xjzMean_pbpb_Theory(std::string inputFile);
void zJetPlot_M_Zee_pbpb(std::string inputFile);
void zJetPlot_M_Zmm_pbpb(std::string inputFile);
void zJetPlot_bkgSubtraction_dphijz_pbpb(std::string inputFile);
void zJetPlot_xjz_xjg(std::string inputFile, bool isPP);
void zJetPlot_xjz_xjg_pp(std::string inputFile);
void zJetPlot_xjz_xjg_pbpb(std::string inputFile);
void zJetPlot_xjzMean_xjgMean(std::string inputFile, bool isPP);
void zJetPlot_xjzMean_xjgMean_pp(std::string inputFile);
void zJetPlot_xjzMean_xjgMean_pbpb(std::string inputFile);
void setTH1D(int iHist, TH1D* h);
void setTGraph(int iGraph, TGraph* gr);
void setTGraphSys(int iSys, TGraph* gr);
void setCanvas(TCanvas* c);
void setLegend(TLegend* leg);
void setLatex(int iText, TLatex* latex);
void setLatexOverPad(int iText, TLatex* latex);
void setLatexCMS(TLatex* latex);
void setSysUncBox(TGraph* gr, TH1* h, TH1* hSys, int bin, bool doRelUnc = false, double binWidth = -1, double binWidthScale = 1);
void drawSysUncBoxes(TGraph* gr, TH1* h, TH1* hSys, bool doRelUnc = false, double binWidth = -1, double binWidthScale = 1);

void zJetPlotPublic(int figureIndex, std::string inputFile)
{
    std::cout<<"running zJetPlotPublic()"<<std::endl;
    std::cout<<"figureIndex = "<< figureIndex <<std::endl;
    std::cout<<"inputFile = "<< inputFile.c_str() <<std::endl;

    switch (figureIndex) {
        case k_xjz:
            zJetPlot_xjz(inputFile);
            break;
        case k_dphijz:
            zJetPlot_dphijz(inputFile);
            break;
        case k_rjz:
            zJetPlot_rjz(inputFile);
            break;
        case k_xjzMean:
            zJetPlot_xjzMean(inputFile);
            break;
        case k_xjz_pp_Theory:
            zJetPlot_xjz_pp_Theory(inputFile);
            break;
        case k_xjz_pbpb_Theory:
            zJetPlot_xjz_pbpb_Theory(inputFile);
            break;
        case k_dphijz_pp_Theory:
            zJetPlot_dphijz_pp_Theory(inputFile);
            break;
        case k_dphijz_pbpb_Theory:
            zJetPlot_dphijz_pbpb_Theory(inputFile);
            break;
        case k_rjz_pp_Theory:
            zJetPlot_rjz_pp_Theory(inputFile);
            break;
        case k_rjz_pbpb_Theory:
            zJetPlot_rjz_pbpb_Theory(inputFile);
            break;
        case k_xjzMean_pp_Theory:
            zJetPlot_xjzMean_pp_Theory(inputFile);
            break;
        case k_xjzMean_pbpb_Theory:
            zJetPlot_xjzMean_pbpb_Theory(inputFile);
            break;
        case k_M_Zee_pbpb:
            zJetPlot_M_Zee_pbpb(inputFile);
            break;
        case k_M_Zmm_pbpb:
            zJetPlot_M_Zmm_pbpb(inputFile);
            break;
        case k_bkgSubtraction_dphijz_pbpb:
            zJetPlot_bkgSubtraction_dphijz_pbpb(inputFile);
            break;
        case k_xjz_xjg_pp:
            zJetPlot_xjz_xjg_pp(inputFile);
            break;
        case k_xjz_xjg_pbpb:
            zJetPlot_xjz_xjg_pbpb(inputFile);
            break;
        case k_xjzMean_xjgMean_pp:
            zJetPlot_xjzMean_xjgMean_pp(inputFile);
            break;
        case k_xjzMean_xjgMean_pbpb:
            zJetPlot_xjzMean_xjgMean_pbpb(inputFile);
            break;
        default:
            break;
    }

    std::cout<<"zJetPlotPublic() - END"<<std::endl;
}

void zJetPlot_xjz(std::string inputFile)
{
    std::cout<<"running zJetPlot_xjz()"<<std::endl;

    TFile* input  = TFile::Open(inputFile.c_str());

    // no horizontal error bars
    gStyle->SetErrorX(0);
    gStyle->SetHatchesLineWidth(3);

    windowWidth = 800;
    windowHeight = 800;
    logX = 0;
    logY = 0;
    leftMargin   = 0.21;
    rightMargin  = 0.03;
    bottomMargin = 0.15;
    topMargin    = 0.06;
    TCanvas* c = new TCanvas(figureNames[k_xjz].c_str(), "", windowWidth, windowHeight);
    std::cout<<"preparing canvas : "<< c->GetName() <<std::endl;
    setCanvas(c);
    c->cd();

    xTitle = "x_{jZ} = p^{jet}_{T}/p^{Z}_{T}";
    yTitle = "#frac{1}{N_{Z}} #frac{dN_{jZ}}{dx_{jZ}}";
    xTitleSize = 0.0525;
    yTitleSize = 0.0525;
    xTitleOffset = 1.25;
    yTitleOffset = 1.5;
    xTitleFont = 42;
    yTitleFont = 42;

    yMin = -0.05;
    yMax = 1;

    enum HISTLABELS {
        k_pbpb,
        k_pp,
        kN_HISTLABELS
    };

    histPaths = {
            "h1D_xjz_pbpb_cent030",
            "h1D_xjz_pp_cent030"
    };
    markerColors = {kBlack, kBlack};
    markerStyles = {kFullCircle, kOpenSquare};
    markerSizes = {2, 2};
    lineColors = {46, kBlack};
    lineTransparencies = {0.7, 1.0};
    lineWidths = {3, 2};
    fillColors = {46, 30};
    fillTransparencies = {0.7, 0.7};
    drawOptions = {"e same", "e same"};
    sysPaths = {
            "h1D_sysVar_xjz_pbpb_cent030_rel",
            "h1D_sysVar_xjz_pp_cent030_rel"
    };
    sysUseRelUnc = {true, true};
    sysColors = {46, 30};
    sysTransparencies = {0.7, 0.7};
    sysFillStyles = {1001, 1001};

    int nHistPaths = histPaths.size();
    std::vector<TH1D*> h1Ds(nHistPaths, 0);
    std::vector<TH1D*> h1DsSys(nHistPaths, 0);
    TGraph* gr = 0;
    TH1D* hTmp = 0;
    for (int i = 0; i < nHistPaths; ++i) {

        h1Ds[i] = (TH1D*)input->Get(histPaths[i].c_str());
        setTH1D(i, h1Ds[i]);

        if (i == 0) {
            hTmp = (TH1D*)h1Ds[i]->Clone(Form("%s_tmpDraw", h1Ds[i]->GetName()));
            hTmp->Draw("e");
        }

        h1DsSys[i] = (TH1D*)input->Get(sysPaths[i].c_str());
        if (h1DsSys[i] != 0) {
            gr = new TGraph();
            setTGraphSys(i, gr);
            drawSysUncBoxes(gr, h1Ds[i], h1DsSys[i], sysUseRelUnc[i]);
        }

        h1Ds[i]->Draw(drawOptions[i].c_str());
    }
    h1Ds[k_pp]->Draw("e same");

    legendX1 = 0.6;
    legendY1 = 0.7875;
    legendWidth = 0.44;
    legendHeight = 0.1125;
    legendMargin = 0.15;
    legendEntryTexts = {
            "PbPb, 0-30 %",
            "Smeared pp"
    };
    legendEntryOptions = {
            "pf",
            "pf"
    };
    TLegend* leg = new TLegend();

    hTmp = (TH1D*)h1Ds[k_pbpb]->Clone(Form("%s_tmp", h1Ds[k_pbpb]->GetName()));
    hTmp->SetLineWidth(0);
    leg->AddEntry(hTmp, legendEntryTexts[k_pbpb].c_str(), legendEntryOptions[k_pbpb].c_str());
    hTmp = (TH1D*)h1Ds[k_pp]->Clone(Form("%s_tmp", h1Ds[k_pp]->GetName()));
    hTmp->SetLineWidth(0);
    leg->AddEntry(hTmp, legendEntryTexts[k_pp].c_str(), legendEntryOptions[k_pp].c_str());

    setLegend(leg);
    leg->Draw();

    textX = 0.93;
    textYs = {0.72, 0.6633, 0.6076, 0.5529, 0.4992};
    textAlign = 31;
    textFont = 43;
    textSize = 34;
    textLines = {
            "p_{T}^{Z} > 60 GeV/c",
            "anti-k_{T} jet R = 0.3",
            "p_{T}^{jet} > 30 GeV/c",
            "|#eta^{jet}| < 1.6",
            "#Delta#phi_{jZ} > #frac{7}{8}#pi"
    };
    int nTextLines = textLines.size();
    TLatex* latex = 0;
    for (int i = 0; i < nTextLines; ++i) {
        latex = new TLatex();
        setLatex(i, latex);
        latex->Draw();
    }

    textXsOverPad = {0.22, 0.96};
    textYOverPad = 0.96;
    textAlignsOverPad = {11, 31};
    textFontOverPad = 43;
    textSizeOverPad = 30;
    textOverPadLines = {
            "#sqrt{s_{NN}} = 5.02 TeV",
            "PbPb 404 #mub^{-1}, pp 27.4 pb^{-1}"
    };
    int nTextOverPadLines = textOverPadLines.size();
    for (int i = 0; i < nTextOverPadLines; ++i) {
        latex = new TLatex();
        setLatexOverPad(i, latex);
        latex->Draw();
    }

    textXCMS = 0.25;
    textYCMS = 0.86;
    textAlignCMS = 11;
    textFontCMS = 61;
    textSizeCMS = 0.06299999;
    latex = new TLatex();
    setLatexCMS(latex);
    latex->Draw();

    c->Update();

    TLine* line = new TLine(gPad->GetUxmin(), 0, gPad->GetUxmax(), 0);
    line->SetLineStyle(kDashed);
    line->Draw();

    c->SaveAs(Form("%s.pdf", c->GetName()));
    c->Close();         // do not use Delete() for TCanvas.

    std::cout<<"Closing the input file"<<std::endl;
    input->Close();

    std::cout<<"running zJetPlot_xjz() - END"<<std::endl;
}

void zJetPlot_dphijz(std::string inputFile)
{
    std::cout<<"running zJetPlot_dphijz()"<<std::endl;

    TFile* input  = TFile::Open(inputFile.c_str());

    // no horizontal error bars
    gStyle->SetErrorX(0);
    gStyle->SetHatchesLineWidth(3);

    windowWidth = 800;
    windowHeight = 800;
    logX = 0;
    logY = 0;
    leftMargin   = 0.21;
    rightMargin  = 0.03;
    bottomMargin = 0.15;
    topMargin    = 0.06;
    TCanvas* c = new TCanvas(figureNames[k_dphijz].c_str(), "", windowWidth, windowHeight);
    std::cout<<"preparing canvas : "<< c->GetName() <<std::endl;
    setCanvas(c);
    c->cd();

    xTitle = "#Delta#phi_{jZ}";
    yTitle = "#frac{1}{N_{Z}} #frac{dN_{jZ}}{d#Delta#phi_{jZ}}";
    xTitleSize = 0.0525;
    yTitleSize = 0.0525;
    xTitleOffset = 1.25;
    yTitleOffset = 1.5;
    xTitleFont = 42;
    yTitleFont = 42;

    yMin = -0.2;
    yMax = 3;

    enum HISTLABELS {
        k_pbpb,
        k_pp,
        kN_HISTLABELS
    };

    histPaths = {
            "h1D_dphijz_pbpb_cent030",
            "h1D_dphijz_pp_cent030"
    };
    markerColors = {kBlack, kBlack};
    markerStyles = {kFullCircle, kOpenSquare};
    markerSizes = {2, 2};
    lineColors = {46, kBlack};
    lineTransparencies = {0.7, 1.0};
    lineWidths = {3, 2};
    fillColors = {46, 30};
    fillTransparencies = {0.7, 0.7};
    drawOptions = {"e same", "e same"};
    sysPaths = {
            "h1D_sysVar_dphijz_pbpb_cent030_diff",
            "h1D_sysVar_dphijz_pp_cent030_diff"
    };
    sysUseRelUnc = {false, false};
    sysColors = {46, 30};
    sysTransparencies = {0.7, 0.7};
    sysFillStyles = {1001, 1001};

    int nHistPaths = histPaths.size();
    std::vector<TH1D*> h1Ds(nHistPaths, 0);
    std::vector<TH1D*> h1DsSys(nHistPaths, 0);
    TGraph* gr = 0;
    TH1D* hTmp = 0;
    for (int i = 0; i < nHistPaths; ++i) {

        h1Ds[i] = (TH1D*)input->Get(histPaths[i].c_str());
        setTH1D(i, h1Ds[i]);

        if (i == 0) {
            hTmp = (TH1D*)h1Ds[i]->Clone(Form("%s_tmpDraw", h1Ds[i]->GetName()));
            hTmp->Draw("e");
        }

        h1DsSys[i] = (TH1D*)input->Get(sysPaths[i].c_str());
        if (h1DsSys[i] != 0) {
            gr = new TGraph();
            setTGraphSys(i, gr);
            drawSysUncBoxes(gr, h1Ds[i], h1DsSys[i], sysUseRelUnc[i]);
        }

        h1Ds[i]->Draw(drawOptions[i].c_str());
    }
    h1Ds[k_pp]->Draw("e same");

    legendX1 = 0.25;
    legendY1 = 0.6775;
    legendWidth = 0.44;
    legendHeight = 0.1125;
    legendMargin = 0.15;
    legendEntryTexts = {
            "PbPb, 0-30 %",
            "Smeared pp"
    };
    legendEntryOptions = {
            "pf",
            "pf"
    };
    TLegend* leg = new TLegend();

    hTmp = (TH1D*)h1Ds[k_pbpb]->Clone(Form("%s_tmp", h1Ds[k_pbpb]->GetName()));
    hTmp->SetLineWidth(0);
    leg->AddEntry(hTmp, legendEntryTexts[k_pbpb].c_str(), legendEntryOptions[k_pbpb].c_str());
    hTmp = (TH1D*)h1Ds[k_pp]->Clone(Form("%s_tmp", h1Ds[k_pp]->GetName()));
    hTmp->SetLineWidth(0);
    leg->AddEntry(hTmp, legendEntryTexts[k_pp].c_str(), legendEntryOptions[k_pp].c_str());

    setLegend(leg);
    leg->Draw();

    textX = 0.25;
    textYs = {0.61, 0.5544, 0.4998, 0.43792};
    textAlign = 11;
    textFont = 43;
    textSize = 34;
    textLines = {
            "p_{T}^{Z} > 60 GeV/c",
            "anti-k_{T} jet R = 0.3",
            "p_{T}^{jet} > 30 GeV/c",
            "|#eta^{jet}| < 1.6"
    };
    int nTextLines = textLines.size();
    TLatex* latex = 0;
    for (int i = 0; i < nTextLines; ++i) {
        latex = new TLatex();
        setLatex(i, latex);
        latex->Draw();
    }

    textXsOverPad = {0.22, 0.96};
    textYOverPad = 0.96;
    textAlignsOverPad = {11, 31};
    textFontOverPad = 43;
    textSizeOverPad = 30;
    textOverPadLines = {
            "#sqrt{s_{NN}} = 5.02 TeV",
            "PbPb 404 #mub^{-1}, pp 27.4 pb^{-1}"
    };
    int nTextOverPadLines = textOverPadLines.size();
    for (int i = 0; i < nTextOverPadLines; ++i) {
        latex = new TLatex();
        setLatexOverPad(i, latex);
        latex->Draw();
    }

    textXCMS = 0.25;
    textYCMS = 0.86;
    textAlignCMS = 11;
    textFontCMS = 61;
    textSizeCMS = 0.06299999;
    latex = new TLatex();
    setLatexCMS(latex);
    latex->Draw();

    c->Update();

    TLine* line = new TLine(gPad->GetUxmin(), 0, gPad->GetUxmax(), 0);
    line->SetLineStyle(kDashed);
    line->Draw();

    c->SaveAs(Form("%s.pdf", c->GetName()));
    c->Close();         // do not use Delete() for TCanvas.

    std::cout<<"Closing the input file"<<std::endl;
    input->Close();

    std::cout<<"running zJetPlot_dphijz() - END"<<std::endl;
}

void zJetPlot_rjz(std::string inputFile)
{
    std::cout<<"running zJetPlot_rjz()"<<std::endl;

    TFile* input  = TFile::Open(inputFile.c_str());

    // no horizontal error bars
    gStyle->SetErrorX(0);
    gStyle->SetHatchesLineWidth(3);

    windowWidth = 800;
    windowHeight = 800;
    logX = 0;
    logY = 0;
    leftMargin   = 0.21;
    rightMargin  = 0.03;
    bottomMargin = 0.15;
    topMargin    = 0.06;
    TCanvas* c = new TCanvas(figureNames[k_rjz].c_str(), "", windowWidth, windowHeight);
    std::cout<<"preparing canvas : "<< c->GetName() <<std::endl;
    setCanvas(c);
    c->cd();

    xTitle = "p^{Z}_{T} (GeV/c)";
    yTitle = "R_{jZ}";
    xTitleSize = 0.0525;
    yTitleSize = 0.0525;
    xTitleOffset = 1.25;
    yTitleOffset = 1.5;
    xTitleFont = 42;
    yTitleFont = 42;

    yMin = 0;
    yMax = 1;

    enum HISTLABELS {
        k_pp,
        k_pbpb,
        kN_HISTLABELS
    };

    histPaths = {
            "h1D_rjz_pp_cent030",
            "h1D_rjz_pbpb_cent030"
    };
    markerColors = {kBlack, kBlack};
    markerStyles = {kOpenSquare, kFullCircle};
    markerSizes = {2, 2};
    lineColors = {kBlack, 46};
    lineTransparencies = {1.0, 0.7};
    lineWidths = {2, 3};
    fillColors = {30, 46};
    fillTransparencies = {0.7, 0.7};
    drawOptions = {"e same", "e same"};
    sysPaths = {
            "h1D_sysVar_rjz_pp_cent030_rel",
            "h1D_sysVar_rjz_pbpb_cent030_rel"
    };
    sysUseRelUnc = {true, true};
    sysColors = {30, 46};
    sysTransparencies = {0.7, 0.7};
    sysFillStyles = {1001, 1001};

    int nHistPaths = histPaths.size();
    std::vector<TH1D*> h1Ds(nHistPaths, 0);
    std::vector<TH1D*> h1DsSys(nHistPaths, 0);
    TGraph* gr = 0;
    TH1D* hTmp = 0;
    for (int i = 0; i < nHistPaths; ++i) {

        h1Ds[i] = (TH1D*)input->Get(histPaths[i].c_str());
        setTH1D(i, h1Ds[i]);

        if (i == 0) {
            hTmp = (TH1D*)h1Ds[i]->Clone(Form("%s_tmpDraw", h1Ds[i]->GetName()));
            hTmp->Draw("e");
        }

        h1DsSys[i] = (TH1D*)input->Get(sysPaths[i].c_str());
        if (h1DsSys[i] != 0) {
            gr = new TGraph();
            setTGraphSys(i, gr);
            drawSysUncBoxes(gr, h1Ds[i], h1DsSys[i], sysUseRelUnc[i]);
        }

        h1Ds[i]->Draw(drawOptions[i].c_str());
    }
    h1Ds[k_pbpb]->Draw("e same");

    legendX1 = 0.25;
    legendY1 = 0.6775;
    legendWidth = 0.44;
    legendHeight = 0.1125;
    legendMargin = 0.15;
    legendEntryTexts = {
            "Smeared pp",
            "PbPb, 0-30 %"
    };
    legendEntryOptions = {
            "pf",
            "pf"
    };
    TLegend* leg = new TLegend();

    hTmp = (TH1D*)h1Ds[k_pbpb]->Clone(Form("%s_tmp", h1Ds[k_pbpb]->GetName()));
    hTmp->SetLineWidth(0);
    leg->AddEntry(hTmp, legendEntryTexts[k_pbpb].c_str(), legendEntryOptions[k_pbpb].c_str());
    hTmp = (TH1D*)h1Ds[k_pp]->Clone(Form("%s_tmp", h1Ds[k_pp]->GetName()));
    hTmp->SetLineWidth(0);
    leg->AddEntry(hTmp, legendEntryTexts[k_pp].c_str(), legendEntryOptions[k_pp].c_str());

    setLegend(leg);
    leg->Draw();

    textX = 0.93;
    textYs = {0.37, 0.3168, 0.2646, 0.2134};
    textAlign = 31;
    textFont = 43;
    textSize = 34;
    textLines = {
            "anti-k_{T} jet R = 0.3",
            "p_{T}^{jet} > 30 GeV/c",
            "|#eta^{jet}| < 1.6",
            "#Delta#phi_{jZ} > #frac{7}{8}#pi"
    };
    int nTextLines = textLines.size();
    TLatex* latex = 0;
    for (int i = 0; i < nTextLines; ++i) {
        latex = new TLatex();
        setLatex(i, latex);
        latex->Draw();
    }

    textXsOverPad = {0.22, 0.96};
    textYOverPad = 0.96;
    textAlignsOverPad = {11, 31};
    textFontOverPad = 43;
    textSizeOverPad = 30;
    textOverPadLines = {
            "#sqrt{s_{NN}} = 5.02 TeV",
            "PbPb 404 #mub^{-1}, pp 27.4 pb^{-1}"
    };
    int nTextOverPadLines = textOverPadLines.size();
    for (int i = 0; i < nTextOverPadLines; ++i) {
        latex = new TLatex();
        setLatexOverPad(i, latex);
        latex->Draw();
    }

    textXCMS = 0.25;
    textYCMS = 0.86;
    textAlignCMS = 11;
    textFontCMS = 61;
    textSizeCMS = 0.06299999;
    latex = new TLatex();
    setLatexCMS(latex);
    latex->Draw();

    c->Update();

    TLine* line = new TLine(gPad->GetUxmin(), 0, gPad->GetUxmax(), 0);
    line->SetLineStyle(kDashed);
    line->Draw();

    c->SaveAs(Form("%s.pdf", c->GetName()));
    c->Close();         // do not use Delete() for TCanvas.

    std::cout<<"Closing the input file"<<std::endl;
    input->Close();

    std::cout<<"running zJetPlot_rjz() - END"<<std::endl;
}

void zJetPlot_xjzMean(std::string inputFile)
{
    std::cout<<"running zJetPlot_xjzMean()"<<std::endl;

    TFile* input  = TFile::Open(inputFile.c_str());

    // no horizontal error bars
    gStyle->SetErrorX(0);
    gStyle->SetHatchesLineWidth(3);

    windowWidth = 800;
    windowHeight = 800;
    logX = 0;
    logY = 0;
    leftMargin   = 0.21;
    rightMargin  = 0.03;
    bottomMargin = 0.15;
    topMargin    = 0.06;
    TCanvas* c = new TCanvas(figureNames[k_xjzMean].c_str(), "", windowWidth, windowHeight);
    std::cout<<"preparing canvas : "<< c->GetName() <<std::endl;
    setCanvas(c);
    c->cd();

    xTitle = "p^{Z}_{T} (GeV/c)";
    yTitle = "<x_{jZ}>";
    xTitleSize = 0.0525;
    yTitleSize = 0.0525;
    xTitleOffset = 1.25;
    yTitleOffset = 1.5;
    xTitleFont = 42;
    yTitleFont = 42;

    yMin = 0.6;
    yMax = 1.1;

    enum HISTLABELS {
        k_pp,
        k_pbpb,
        kN_HISTLABELS
    };

    histPaths = {
            "h1D_xjzMean_pp_cent030",
            "h1D_xjzMean_pbpb_cent030"
    };
    markerColors = {kBlack, kBlack};
    markerStyles = {kOpenSquare, kFullCircle};
    markerSizes = {2, 2};
    lineColors = {kBlack, 46};
    lineTransparencies = {1.0, 0.7};
    lineWidths = {2, 3};
    fillColors = {30, 46};
    fillTransparencies = {0.7, 0.7};
    drawOptions = {"e same", "e same"};
    sysPaths = {
            "h1D_sysVar_xjzMean_pp_cent030_rel",
            "h1D_sysVar_xjzMean_pbpb_cent030_rel"
    };
    sysUseRelUnc = {true, true};
    sysColors = {30, 46};
    sysTransparencies = {0.7, 0.7};
    sysFillStyles = {1001, 1001};

    int nHistPaths = histPaths.size();
    std::vector<TH1D*> h1Ds(nHistPaths, 0);
    std::vector<TH1D*> h1DsSys(nHistPaths, 0);
    TGraph* gr = 0;
    TH1D* hTmp = 0;
    for (int i = 0; i < nHistPaths; ++i) {

        h1Ds[i] = (TH1D*)input->Get(histPaths[i].c_str());
        setTH1D(i, h1Ds[i]);

        if (i == 0) {
            hTmp = (TH1D*)h1Ds[i]->Clone(Form("%s_tmpDraw", h1Ds[i]->GetName()));
            hTmp->Draw("e");
        }

        h1DsSys[i] = (TH1D*)input->Get(sysPaths[i].c_str());
        if (h1DsSys[i] != 0) {
            gr = new TGraph();
            setTGraphSys(i, gr);
            drawSysUncBoxes(gr, h1Ds[i], h1DsSys[i], sysUseRelUnc[i]);
        }

        h1Ds[i]->Draw(drawOptions[i].c_str());
    }
    h1Ds[k_pbpb]->Draw("e same");

    legendX1 = 0.6;
    legendY1 = 0.7875;
    legendWidth = 0.44;
    legendHeight = 0.1125;
    legendMargin = 0.15;
    legendEntryTexts = {
            "Smeared pp",
            "PbPb, 0-30 %"
    };
    legendEntryOptions = {
            "pf",
            "pf"
    };
    TLegend* leg = new TLegend();

    hTmp = (TH1D*)h1Ds[k_pbpb]->Clone(Form("%s_tmp", h1Ds[k_pbpb]->GetName()));
    hTmp->SetLineWidth(0);
    leg->AddEntry(hTmp, legendEntryTexts[k_pbpb].c_str(), legendEntryOptions[k_pbpb].c_str());
    hTmp = (TH1D*)h1Ds[k_pp]->Clone(Form("%s_tmp", h1Ds[k_pp]->GetName()));
    hTmp->SetLineWidth(0);
    leg->AddEntry(hTmp, legendEntryTexts[k_pp].c_str(), legendEntryOptions[k_pp].c_str());

    setLegend(leg);
    leg->Draw();

    textX = 0.93;
    textYs = {0.72, 0.6633, 0.6076, 0.5529};
    textAlign = 31;
    textFont = 43;
    textSize = 34;
    textLines = {
            "anti-k_{T} jet R = 0.3",
            "p_{T}^{jet} > 30 GeV/c",
            "|#eta^{jet}| < 1.6",
            "#Delta#phi_{jZ} > #frac{7}{8}#pi"
    };
    int nTextLines = textLines.size();
    TLatex* latex = 0;
    for (int i = 0; i < nTextLines; ++i) {
        latex = new TLatex();
        setLatex(i, latex);
        latex->Draw();
    }

    textXsOverPad = {0.22, 0.96};
    textYOverPad = 0.96;
    textAlignsOverPad = {11, 31};
    textFontOverPad = 43;
    textSizeOverPad = 30;
    textOverPadLines = {
            "#sqrt{s_{NN}} = 5.02 TeV",
            "PbPb 404 #mub^{-1}, pp 27.4 pb^{-1}"
    };
    int nTextOverPadLines = textOverPadLines.size();
    for (int i = 0; i < nTextOverPadLines; ++i) {
        latex = new TLatex();
        setLatexOverPad(i, latex);
        latex->Draw();
    }

    textXCMS = 0.33;
    textYCMS = 0.86;
    textAlignCMS = 11;
    textFontCMS = 61;
    textSizeCMS = 0.06299999;
    latex = new TLatex();
    setLatexCMS(latex);
    latex->Draw();

    c->Update();

    TLine* line = new TLine(gPad->GetUxmin(), 0, gPad->GetUxmax(), 0);
    line->SetLineStyle(kDashed);
    line->Draw();

    c->SaveAs(Form("%s.pdf", c->GetName()));
    c->Close();         // do not use Delete() for TCanvas.

    std::cout<<"Closing the input file"<<std::endl;
    input->Close();

    std::cout<<"running zJetPlot_xjzMean() - END"<<std::endl;
}

void zJetPlot_xjz_pp_Theory(std::string inputFile)
{
    std::cout<<"running zJetPlot_xjz_pp_Theory()"<<std::endl;

    TFile* input  = TFile::Open(inputFile.c_str());

    // no horizontal error bars
    gStyle->SetErrorX(0);
    gStyle->SetHatchesLineWidth(3);

    windowWidth = 800;
    windowHeight = 800;
    logX = 0;
    logY = 0;
    leftMargin   = 0.21;
    rightMargin  = 0.03;
    bottomMargin = 0.15;
    topMargin    = 0.06;
    TCanvas* c = new TCanvas(figureNames[k_xjz_pp_Theory].c_str(), "", windowWidth, windowHeight);
    std::cout<<"preparing canvas : "<< c->GetName() <<std::endl;
    setCanvas(c);
    c->cd();

    xTitle = "x_{jZ} = p^{jet}_{T}/p^{Z}_{T}";
    yTitle = "#frac{1}{N_{Z}} #frac{dN_{jZ}}{dx_{jZ}}";
    xTitleSize = 0.0525;
    yTitleSize = 0.0525;
    xTitleOffset = 1.25;
    yTitleOffset = 1.5;
    xTitleFont = 42;
    yTitleFont = 42;

    yMin = -0.05;
    yMax = 1.3;

    enum HISTLABELS {
        k_pp,
        k_madgraph,
        kN_HISTLABELS
    };

    histPaths = {
            "h1D_xjz_pp",
            "h1D_xjz_madgraph"
    };
    markerColors = {kBlack, kBlack};
    markerStyles = {kFullCircle, kFullSquare};
    markerSizes = {2, 2};
    lineColors = {kBlack, TColor::GetColor("#C71585")};
    lineTransparencies = {1.0, 0.8};
    lineWidths = {4, 5};
    fillColors = {30, 0};
    fillTransparencies = {0.7, 0};
    drawOptions = {"e same", "l hist same"};
    sysPaths = {
            "h1D_sysVar_xjz_pp_rel",
            "NULL"
    };
    sysUseRelUnc = {true, true};
    sysColors = {30, 30};
    sysTransparencies = {0.7, 0.7};
    sysFillStyles = {1001, 1001};

    int nHistPaths = histPaths.size();
    std::vector<TH1D*> h1Ds(nHistPaths, 0);
    std::vector<TH1D*> h1DsSys(nHistPaths, 0);
    TGraph* gr = 0;
    TH1D* hTmp = 0;
    for (int i = 0; i < nHistPaths; ++i) {

        h1Ds[i] = (TH1D*)input->Get(histPaths[i].c_str());
        setTH1D(i, h1Ds[i]);

        if (i == 0) {
            hTmp = (TH1D*)h1Ds[i]->Clone(Form("%s_tmpDraw", h1Ds[i]->GetName()));
            hTmp->Draw("e");
        }
    }

    enum GRAPHLABELS {
        k_jewel,
        k_hybrid,
        k_GLV,
        kN_GRAPHLABELS
    };

    gStyle->SetLineStyleString(11,"15 4 4 4");
    gStyle->SetLineStyleString(14,"20 8");

    graphPaths = {
            "THEORY/gr_xjz_pp_theory_JEWEL",
            "THEORY/gr_xjz_pp_theory_HYBRID",
            "THEORY/gr_xjz_pp_theory_GLV"
    };
    graphMarkerColors = {TColor::GetColor("#1034A6"), TColor::GetColor("#A0522D"), TColor::GetColor("#138808")};
    graphMarkerTransparencies = {0.85, 0.8, 0.7};
    graphMarkerStyles = {kFullStar, kFullCircle, kFullCircle};
    graphMarkerSizes = {3.2, 0, 0};
    graphLineColors = {TColor::GetColor("#1034A6"), TColor::GetColor("#A0522D"), TColor::GetColor("#138808")};
    graphLineTransparencies = {0.85, 0.8, 0.7};
    graphLineWidths = {4, 5, 5};
    graphLineStyles = {kSolid, 14, 11};
    graphFillColors = {0, 0, 0};
    graphFillTransparencies = {0, 0, 0};
    graphDrawOptions = {"l", "lp", "l"};

    int nGraphPaths = graphPaths.size();
    std::vector<TGraphErrors*> graphs(nGraphPaths, 0);
    for (int i = 0; i < nGraphPaths; ++i) {

        graphs[i] = (TGraphErrors*)input->Get(graphPaths[i].c_str());
        setTGraph(i, graphs[i]);
    }

    h1DsSys[k_pp] = (TH1D*)input->Get(sysPaths[k_pp].c_str());
    if (h1DsSys[k_pp] != 0) {
        gr = new TGraph();
        setTGraphSys(k_pp, gr);
        drawSysUncBoxes(gr, h1Ds[k_pp], h1DsSys[k_pp], sysUseRelUnc[k_pp]);
    }
    h1Ds[k_pp]->Draw(drawOptions[k_pp].c_str());

    graphs[k_hybrid]->Draw(graphDrawOptions[k_hybrid].c_str());
    graphs[k_jewel]->Draw(graphDrawOptions[k_jewel].c_str());
    graphs[k_GLV]->Draw(graphDrawOptions[k_GLV].c_str());
    graphs[k_jewel]->Draw("pe");
    h1Ds[k_madgraph]->Draw(drawOptions[k_madgraph].c_str());

    h1Ds[k_pp]->Draw("e same");

    TLegend* leg = 0;
    legendX1 = 0.25;
    legendY1 = 0.77;
    legendWidth = 0.44;
    legendHeight = 0.06;
    legendMargin = 0.15;
    legendEntryTexts = {"pp"};
    legendEntryOptions = {"pf"};
    leg = new TLegend();

    hTmp = (TH1D*)h1Ds[k_pp]->Clone(Form("%s_tmp", h1Ds[k_pp]->GetName()));
    hTmp->SetLineWidth(0);
    leg->AddEntry(hTmp, legendEntryTexts[k_pp].c_str(), legendEntryOptions[k_pp].c_str());

    setLegend(leg);
    leg->Draw();

    legendX1 = 0.61;
    legendY1 = 0.72;
    legendWidth = 0.462;
    legendHeight = 0.18;
    legendMargin = 0.15;
    legendEntryTexts = {"JEWEL ref.", "Hybrid ref.", "GLV ref.", "MG5aMC@NLO"};
    legendEntryOptions = {"lp", "l", "l", "l"};
    leg = new TLegend();

    gr = (TGraphErrors*)graphs[k_jewel]->Clone(Form("%s_tmp", graphs[k_jewel]->GetName()));
    leg->AddEntry(gr, legendEntryTexts[k_jewel].c_str(), legendEntryOptions[k_jewel].c_str());
    gr = (TGraphErrors*)graphs[k_hybrid]->Clone(Form("%s_tmp", graphs[k_hybrid]->GetName()));
    leg->AddEntry(gr, legendEntryTexts[k_hybrid].c_str(), legendEntryOptions[k_hybrid].c_str());
    gr = (TGraphErrors*)graphs[k_GLV]->Clone(Form("%s_tmp", graphs[k_GLV]->GetName()));
    leg->AddEntry(gr, legendEntryTexts[k_GLV].c_str(), legendEntryOptions[k_GLV].c_str());
    hTmp = (TH1D*)h1Ds[k_madgraph]->Clone(Form("%s_tmp", h1Ds[k_madgraph]->GetName()));
    leg->AddEntry(hTmp, legendEntryTexts[3].c_str(), legendEntryOptions[3].c_str());

    setLegend(leg);
    leg->Draw();

    textX = 0.93;
    textYs = {0.58, 0.5247, 0.4704, 0.4171, 0.3648};
    textAlign = 31;
    textFont = 43;
    textSize = 29;
    textLines = {
            "p_{T}^{Z} > 60 GeV/c",
            "anti-k_{T} jet R = 0.3",
            "p_{T}^{jet} > 30 GeV/c",
            "|#eta^{jet}| < 1.6",
            "#Delta#phi_{jZ} > #frac{7}{8}#pi"
    };
    int nTextLines = textLines.size();
    TLatex* latex = 0;
    for (int i = 0; i < nTextLines; ++i) {
        latex = new TLatex();
        setLatex(i, latex);
        latex->Draw();
    }

    textXsOverPad = {0.22, 0.96};
    textYOverPad = 0.96;
    textAlignsOverPad = {11, 31};
    textFontOverPad = 43;
    textSizeOverPad = 30;
    textOverPadLines = {
            "#sqrt{s} = 5.02 TeV",
            "pp 27.4 pb^{-1}"
    };
    int nTextOverPadLines = textOverPadLines.size();
    for (int i = 0; i < nTextOverPadLines; ++i) {
        latex = new TLatex();
        setLatexOverPad(i, latex);
        latex->Draw();
    }

    textXCMS = 0.25;
    textYCMS = 0.86;
    textAlignCMS = 11;
    textFontCMS = 61;
    textSizeCMS = 0.06299999;
    latex = new TLatex();
    setLatexCMS(latex);
    latex->Draw();

    c->Update();

    TLine* line = new TLine(gPad->GetUxmin(), 0, gPad->GetUxmax(), 0);
    line->SetLineStyle(kDashed);
    line->Draw();

    c->SaveAs(Form("%s.pdf", c->GetName()));
    c->Close();         // do not use Delete() for TCanvas.

    std::cout<<"Closing the input file"<<std::endl;
    input->Close();

    std::cout<<"running zJetPlot_xjz_pp_Theory() - END"<<std::endl;
}

void zJetPlot_xjz_pbpb_Theory(std::string inputFile)
{
    std::cout<<"running zJetPlot_xjz_pbpb_Theory()"<<std::endl;

    TFile* input  = TFile::Open(inputFile.c_str());

    // no horizontal error bars
    gStyle->SetErrorX(0);
    gStyle->SetHatchesLineWidth(3);

    windowWidth = 800;
    windowHeight = 800;
    logX = 0;
    logY = 0;
    leftMargin   = 0.21;
    rightMargin  = 0.03;
    bottomMargin = 0.15;
    topMargin    = 0.06;
    TCanvas* c = new TCanvas(figureNames[k_xjz_pbpb_Theory].c_str(), "", windowWidth, windowHeight);
    std::cout<<"preparing canvas : "<< c->GetName() <<std::endl;
    setCanvas(c);
    c->cd();

    xTitle = "x_{jZ} = p^{jet}_{T}/p^{Z}_{T}";
    yTitle = "#frac{1}{N_{Z}} #frac{dN_{jZ}}{dx_{jZ}}";
    xTitleSize = 0.0525;
    yTitleSize = 0.0525;
    xTitleOffset = 1.25;
    yTitleOffset = 1.5;
    xTitleFont = 42;
    yTitleFont = 42;

    yMin = -0.05;
    yMax = 1.3;

    enum HISTLABELS {
        k_pbpb,
        kN_HISTLABELS
    };

    histPaths = {
            "h1D_xjz_pbpb_cent030"
    };
    markerColors = {kBlack};
    markerStyles = {kFullCircle};
    markerSizes = {2};
    lineColors = {46};
    lineTransparencies = {0.7};
    lineWidths = {4};
    fillColors = {46};
    fillTransparencies = {0.7};
    drawOptions = {"e same"};
    sysPaths = {
            "h1D_sysVar_xjz_pbpb_cent030_rel"
    };
    sysUseRelUnc = {true};
    sysColors = {46};
    sysTransparencies = {0.7};
    sysFillStyles = {1001};

    int nHistPaths = histPaths.size();
    std::vector<TH1D*> h1Ds(nHistPaths, 0);
    std::vector<TH1D*> h1DsSys(nHistPaths, 0);
    TGraph* gr = 0;
    TH1D* hTmp = 0;
    for (int i = 0; i < nHistPaths; ++i) {

        h1Ds[i] = (TH1D*)input->Get(histPaths[i].c_str());
        setTH1D(i, h1Ds[i]);

        if (i == 0) {
            hTmp = (TH1D*)h1Ds[i]->Clone(Form("%s_tmpDraw", h1Ds[i]->GetName()));
            hTmp->Draw("e");
        }
    }

    enum GRAPHLABELS {
        k_jewel,
        k_hybrid_coll,
        k_hybrid_rad,
        k_hybrid_strong,
        k_GLV_g2p0,
        k_GLV_g2p2,
        kN_GRAPHLABELS
    };

    gStyle->SetLineStyleString(12,"5 5");
    gStyle->SetLineStyleString(13,"16 4");

    graphPaths = {
            "THEORY/gr_xjz_pbpb_cent030_theory_JEWEL",
            "THEORY/gr_xjz_pbpb_cent030_theory_HYBRID_coll",
            "THEORY/gr_xjz_pbpb_cent030_theory_HYBRID_rad",
            "THEORY/gr_xjz_pbpb_cent030_theory_HYBRID_strong",
            "THEORY/gr_xjz_pbpb_cent030_theory_GLV_g2p0",
            "THEORY/gr_xjz_pbpb_cent030_theory_GLV_g2p2"
    };
    graphMarkerColors = {TColor::GetColor("#1034A6"),
            TColor::GetColor("#A0522D"), TColor::GetColor("#DAA520"), TColor::GetColor("#F4A460"),
            TColor::GetColor("#138808"), TColor::GetColor("#138808")};
    graphMarkerTransparencies = {0.85,
            0.7, 0.7, 0.7,
            0.7, 0.7};
    graphMarkerStyles = {kFullStar,
            kFullCircle, kFullCircle, kFullCircle,
            kFullCircle, kFullCircle};
    graphMarkerSizes = {3.2, 0, 0, 0, 0, 0};
    graphLineColors = {TColor::GetColor("#1034A6"),
            TColor::GetColor("#A0522D"), TColor::GetColor("#DAA520"), TColor::GetColor("#F4A460"),
            TColor::GetColor("#138808"), TColor::GetColor("#138808")};
    graphLineTransparencies = {0.85,
                0.7, 0.7, 0.7,
                0.7, 0.7};
    graphLineWidths = {4, 0, 0, 0, 5, 5};
    graphLineStyles = {kSolid, kSolid, kSolid, kSolid, 12, 13};
    graphFillColors = {0,
            TColor::GetColor("#A0522D"), TColor::GetColor("#DAA520"), TColor::GetColor("#F4A460"),
            0, 0};
    graphFillTransparencies = {0, 0.7, 0.7, 0.7, 0, 0};
    graphDrawOptions = {"lp", "f", "f", "f", "l", "l"};

    int nGraphPaths = graphPaths.size();
    std::vector<TGraphErrors*> graphs(nGraphPaths, 0);
    for (int i = 0; i < nGraphPaths; ++i) {

        graphs[i] = (TGraphErrors*)input->Get(graphPaths[i].c_str());
        setTGraph(i, graphs[i]);
    }

    h1DsSys[k_pbpb] = (TH1D*)input->Get(sysPaths[k_pbpb].c_str());
    if (h1DsSys[k_pbpb] != 0) {
        gr = new TGraph();
        setTGraphSys(k_pbpb, gr);
        drawSysUncBoxes(gr, h1Ds[k_pbpb], h1DsSys[k_pbpb], sysUseRelUnc[k_pbpb]);
    }
    h1Ds[k_pbpb]->Draw(drawOptions[k_pbpb].c_str());

    graphs[k_hybrid_coll]->Draw(graphDrawOptions[k_hybrid_coll].c_str());
    graphs[k_hybrid_rad]->Draw(graphDrawOptions[k_hybrid_rad].c_str());
    graphs[k_hybrid_strong]->Draw(graphDrawOptions[k_hybrid_strong].c_str());
    graphs[k_jewel]->Draw(graphDrawOptions[k_jewel].c_str());
    graphs[k_GLV_g2p0]->Draw(graphDrawOptions[k_GLV_g2p0].c_str());
    graphs[k_GLV_g2p2]->Draw(graphDrawOptions[k_GLV_g2p2].c_str());

    h1Ds[k_pbpb]->Draw("e same");

    TLegend* leg = 0;
    legendX1 = 0.25;
    legendY1 = 0.77;
    legendWidth = 0.44;
    legendHeight = 0.06;
    legendMargin = 0.15;
    legendEntryTexts = {"PbPb, 0-30 %"};
    legendEntryOptions = {"pf"};
    leg = new TLegend();

    hTmp = (TH1D*)h1Ds[k_pbpb]->Clone(Form("%s_tmp", h1Ds[k_pbpb]->GetName()));
    hTmp->SetLineWidth(0);
    leg->AddEntry(hTmp, legendEntryTexts[k_pbpb].c_str(), legendEntryOptions[k_pbpb].c_str());

    setLegend(leg);
    leg->Draw();

    legendEntryTexts = {"JEWEL",
            "dE/dx #alpha T^{2}", "dE/dx #alpha T^{3}", "Strong Coupling",
            "g = 2.0", "g = 2.2"};
    legendEntryOptions = {"lp",
            "f", "f", "f",
            "l", "l"};
    legendX1 = 0.61;
    legendY1 = 0.851;
    legendWidth = 0.484;
    legendHeight = 0.054;
    legendMargin = 0.15;
    leg = new TLegend();

    gr = (TGraphErrors*)graphs[k_jewel]->Clone(Form("%s_tmp", graphs[k_jewel]->GetName()));
    leg->AddEntry(gr, legendEntryTexts[k_jewel].c_str(), legendEntryOptions[k_jewel].c_str());

    setLegend(leg);
    leg->Draw();

    legendX1 = 0.61;
    legendY1 = 0.656;
    legendWidth = 0.484;
    legendHeight = 0.180;
    legendMargin = 0.15;
    leg = new TLegend();

    leg->SetHeader("        Hybrid");
    gr = (TGraphErrors*)graphs[k_hybrid_coll]->Clone(Form("%s_tmp", graphs[k_hybrid_coll]->GetName()));
    leg->AddEntry(gr, legendEntryTexts[k_hybrid_coll].c_str(), legendEntryOptions[k_hybrid_coll].c_str());
    gr = (TGraphErrors*)graphs[k_hybrid_rad]->Clone(Form("%s_tmp", graphs[k_hybrid_rad]->GetName()));
    leg->AddEntry(gr, legendEntryTexts[k_hybrid_rad].c_str(), legendEntryOptions[k_hybrid_rad].c_str());
    gr = (TGraphErrors*)graphs[k_hybrid_strong]->Clone(Form("%s_tmp", graphs[k_hybrid_strong]->GetName()));
    leg->AddEntry(gr, legendEntryTexts[k_hybrid_strong].c_str(), legendEntryOptions[k_hybrid_strong].c_str());

    setLegend(leg);
    leg->Draw();

    legendX1 = 0.61;
    legendY1 = 0.506;
    legendWidth = 0.484;
    legendHeight = 0.135;
    legendMargin = 0.15;
    leg = new TLegend();

    leg->SetHeader("        GLV");
    gr = (TGraphErrors*)graphs[k_GLV_g2p0]->Clone(Form("%s_tmp", graphs[k_GLV_g2p0]->GetName()));
    leg->AddEntry(gr, legendEntryTexts[k_GLV_g2p0].c_str(), legendEntryOptions[k_GLV_g2p0].c_str());
    gr = (TGraphErrors*)graphs[k_GLV_g2p2]->Clone(Form("%s_tmp", graphs[k_GLV_g2p2]->GetName()));
    leg->AddEntry(gr, legendEntryTexts[k_GLV_g2p2].c_str(), legendEntryOptions[k_GLV_g2p2].c_str());

    setLegend(leg);
    leg->Draw();

    textX = 0.93;
    textYs = {0.46, 0.4059, 0.3528, 0.3007, 0.2496};
    textAlign = 31;
    textFont = 43;
    textSize = 29;
    textLines = {
            "p_{T}^{Z} > 60 GeV/c",
            "anti-k_{T} jet R = 0.3",
            "p_{T}^{jet} > 30 GeV/c",
            "|#eta^{jet}| < 1.6",
            "#Delta#phi_{jZ} > #frac{7}{8}#pi"
    };
    int nTextLines = textLines.size();
    TLatex* latex = 0;
    for (int i = 0; i < nTextLines; ++i) {
        latex = new TLatex();
        setLatex(i, latex);
        latex->Draw();
    }

    textXsOverPad = {0.22, 0.96};
    textYOverPad = 0.96;
    textAlignsOverPad = {11, 31};
    textFontOverPad = 43;
    textSizeOverPad = 30;
    textOverPadLines = {
            "#sqrt{s_{NN}} = 5.02 TeV",
            "PbPb 404 #mub^{-1}"
    };
    int nTextOverPadLines = textOverPadLines.size();
    for (int i = 0; i < nTextOverPadLines; ++i) {
        latex = new TLatex();
        setLatexOverPad(i, latex);
        latex->Draw();
    }

    textXCMS = 0.25;
    textYCMS = 0.86;
    textAlignCMS = 11;
    textFontCMS = 61;
    textSizeCMS = 0.06299999;
    latex = new TLatex();
    setLatexCMS(latex);
    latex->Draw();

    c->Update();

    TLine* line = new TLine(gPad->GetUxmin(), 0, gPad->GetUxmax(), 0);
    line->SetLineStyle(kDashed);
    line->Draw();

    c->SaveAs(Form("%s.pdf", c->GetName()));
    c->Close();         // do not use Delete() for TCanvas.

    std::cout<<"Closing the input file"<<std::endl;
    input->Close();

    std::cout<<"running zJetPlot_xjz_pbpb_Theory() - END"<<std::endl;
}

void zJetPlot_dphijz_pp_Theory(std::string inputFile)
{
    std::cout<<"running zJetPlot_dphijz_pp_Theory()"<<std::endl;

    TFile* input  = TFile::Open(inputFile.c_str());

    // no horizontal error bars
    gStyle->SetErrorX(0);
    gStyle->SetHatchesLineWidth(3);

    windowWidth = 800;
    windowHeight = 800;
    logX = 0;
    logY = 0;
    leftMargin   = 0.21;
    rightMargin  = 0.03;
    bottomMargin = 0.15;
    topMargin    = 0.06;
    TCanvas* c = new TCanvas(figureNames[k_dphijz_pp_Theory].c_str(), "", windowWidth, windowHeight);
    std::cout<<"preparing canvas : "<< c->GetName() <<std::endl;
    setCanvas(c);
    c->cd();

    xTitle = "#Delta#phi_{jZ}";
    yTitle = "#frac{1}{N_{Z}} #frac{dN_{jZ}}{d#Delta#phi_{jZ}}";
    xTitleSize = 0.0525;
    yTitleSize = 0.0525;
    xTitleOffset = 1.25;
    yTitleOffset = 1.5;
    xTitleFont = 42;
    yTitleFont = 42;

    yMin = -0.2;
    yMax = 3.1;

    enum HISTLABELS {
        k_pp,
        k_madgraph,
        kN_HISTLABELS
    };

    histPaths = {
            "h1D_dphijz_pp",
            "h1D_dphijz_madgraph"
    };
    markerColors = {kBlack, kBlack};
    markerStyles = {kFullCircle, kFullSquare};
    markerSizes = {2, 2};
    lineColors = {kBlack, TColor::GetColor("#C71585")};
    lineTransparencies = {1.0, 0.8};
    lineWidths = {4, 5};
    fillColors = {30, 0};
    fillTransparencies = {0.7, 0};
    drawOptions = {"e same", "l hist same"};
    sysPaths = {
            "h1D_sysVar_dphijz_pp_diff",
            "NULL"
    };
    sysUseRelUnc = {false, false};
    sysColors = {30, 30};
    sysTransparencies = {0.7, 0.7};
    sysFillStyles = {1001, 1001};

    int nHistPaths = histPaths.size();
    std::vector<TH1D*> h1Ds(nHistPaths, 0);
    std::vector<TH1D*> h1DsSys(nHistPaths, 0);
    TGraph* gr = 0;
    TH1D* hTmp = 0;
    for (int i = 0; i < nHistPaths; ++i) {

        h1Ds[i] = (TH1D*)input->Get(histPaths[i].c_str());
        setTH1D(i, h1Ds[i]);

        if (i == 0) {
            hTmp = (TH1D*)h1Ds[i]->Clone(Form("%s_tmpDraw", h1Ds[i]->GetName()));
            hTmp->Draw("e");
        }
    }

    enum GRAPHLABELS {
        k_jewel,
        k_hybrid,
        kN_GRAPHLABELS
    };

    gStyle->SetLineStyleString(14,"20 8");

    graphPaths = {
            "THEORY/gr_dphijz_pp_theory_JEWEL",
            "THEORY/gr_dphijz_pp_theory_HYBRID"
    };
    graphMarkerColors = {TColor::GetColor("#1034A6"), TColor::GetColor("#A0522D")};
    graphMarkerTransparencies = {0.85, 0.8};
    graphMarkerStyles = {kFullStar, kFullCircle};
    graphMarkerSizes = {3.2, 0};
    graphLineColors = {TColor::GetColor("#1034A6"), TColor::GetColor("#A0522D")};
    graphLineTransparencies = {0.85, 0.8};
    graphLineWidths = {4, 5};
    graphLineStyles = {kSolid, 14};
    graphFillColors = {0, 0};
    graphFillTransparencies = {0, 0};
    graphDrawOptions = {"l", "lp"};

    int nGraphPaths = graphPaths.size();
    std::vector<TGraphErrors*> graphs(nGraphPaths, 0);
    for (int i = 0; i < nGraphPaths; ++i) {

        graphs[i] = (TGraphErrors*)input->Get(graphPaths[i].c_str());
        setTGraph(i, graphs[i]);
    }

    h1DsSys[k_pp] = (TH1D*)input->Get(sysPaths[k_pp].c_str());
    if (h1DsSys[k_pp] != 0) {
        gr = new TGraph();
        setTGraphSys(k_pp, gr);
        drawSysUncBoxes(gr, h1Ds[k_pp], h1DsSys[k_pp], sysUseRelUnc[k_pp]);
    }
    h1Ds[k_pp]->Draw(drawOptions[k_pp].c_str());

    graphs[k_hybrid]->Draw(graphDrawOptions[k_hybrid].c_str());
    graphs[k_jewel]->Draw(graphDrawOptions[k_jewel].c_str());
    graphs[k_jewel]->Draw("pe");
    h1Ds[k_madgraph]->Draw(drawOptions[k_madgraph].c_str());

    h1Ds[k_pp]->Draw("e same");

    TLegend* leg = 0;
    legendX1 = 0.25;
    legendY1 = 0.73;
    legendWidth = 0.44;
    legendHeight = 0.06;
    legendMargin = 0.15;
    legendEntryTexts = {"pp"};
    legendEntryOptions = {"pf"};
    leg = new TLegend();

    hTmp = (TH1D*)h1Ds[k_pp]->Clone(Form("%s_tmp", h1Ds[k_pp]->GetName()));
    hTmp->SetLineWidth(0);
    leg->AddEntry(hTmp, legendEntryTexts[k_pp].c_str(), legendEntryOptions[k_pp].c_str());

    setLegend(leg);
    leg->Draw();

    legendX1 = 0.25;
    legendY1 = 0.585;
    legendWidth = 0.462;
    legendHeight = 0.135;
    legendMargin = 0.15;
    legendEntryTexts = {"JEWEL ref.", "Hybrid ref.", "MG5aMC@NLO"};
    legendEntryOptions = {"lp", "l", "l"};
    leg = new TLegend();

    gr = (TGraphErrors*)graphs[k_jewel]->Clone(Form("%s_tmp", graphs[k_jewel]->GetName()));
    leg->AddEntry(gr, legendEntryTexts[k_jewel].c_str(), legendEntryOptions[k_jewel].c_str());
    gr = (TGraphErrors*)graphs[k_hybrid]->Clone(Form("%s_tmp", graphs[k_hybrid]->GetName()));
    leg->AddEntry(gr, legendEntryTexts[k_hybrid].c_str(), legendEntryOptions[k_hybrid].c_str());
    hTmp = (TH1D*)h1Ds[k_madgraph]->Clone(Form("%s_tmp", h1Ds[k_madgraph]->GetName()));
    leg->AddEntry(hTmp, legendEntryTexts[2].c_str(), legendEntryOptions[2].c_str());

    setLegend(leg);
    leg->Draw();

    textX = 0.25;
    textYs = {0.43, 0.3762, 0.3234, 0.26656};
    textAlign = 11;
    textFont = 43;
    textSize = 29;
    textLines = {
            "p_{T}^{Z} > 60 GeV/c",
            "anti-k_{T} jet R = 0.3",
            "p_{T}^{jet} > 30 GeV/c",
            "|#eta^{jet}| < 1.6"
    };
    int nTextLines = textLines.size();
    TLatex* latex = 0;
    for (int i = 0; i < nTextLines; ++i) {
        latex = new TLatex();
        setLatex(i, latex);
        latex->Draw();
    }

    textXsOverPad = {0.22, 0.96};
    textYOverPad = 0.96;
    textAlignsOverPad = {11, 31};
    textFontOverPad = 43;
    textSizeOverPad = 30;
    textOverPadLines = {
            "#sqrt{s} = 5.02 TeV",
            "pp 27.4 pb^{-1}"
    };
    int nTextOverPadLines = textOverPadLines.size();
    for (int i = 0; i < nTextOverPadLines; ++i) {
        latex = new TLatex();
        setLatexOverPad(i, latex);
        latex->Draw();
    }

    textXCMS = 0.25;
    textYCMS = 0.86;
    textAlignCMS = 11;
    textFontCMS = 61;
    textSizeCMS = 0.06299999;
    latex = new TLatex();
    setLatexCMS(latex);
    latex->Draw();

    c->Update();

    TLine* line = new TLine(gPad->GetUxmin(), 0, gPad->GetUxmax(), 0);
    line->SetLineStyle(kDashed);
    line->Draw();

    c->SaveAs(Form("%s.pdf", c->GetName()));
    c->Close();         // do not use Delete() for TCanvas.

    std::cout<<"Closing the input file"<<std::endl;
    input->Close();

    std::cout<<"running zJetPlot_dphijz_pp_Theory() - END"<<std::endl;
}

void zJetPlot_dphijz_pbpb_Theory(std::string inputFile)
{
    std::cout<<"running zJetPlot_dphijz_pbpb_Theory()"<<std::endl;

    TFile* input  = TFile::Open(inputFile.c_str());

    // no horizontal error bars
    gStyle->SetErrorX(0);
    gStyle->SetHatchesLineWidth(3);

    windowWidth = 800;
    windowHeight = 800;
    logX = 0;
    logY = 0;
    leftMargin   = 0.21;
    rightMargin  = 0.03;
    bottomMargin = 0.15;
    topMargin    = 0.06;
    TCanvas* c = new TCanvas(figureNames[k_dphijz_pbpb_Theory].c_str(), "", windowWidth, windowHeight);
    std::cout<<"preparing canvas : "<< c->GetName() <<std::endl;
    setCanvas(c);
    c->cd();

    xTitle = "#Delta#phi_{jZ}";
    yTitle = "#frac{1}{N_{Z}} #frac{dN_{jZ}}{d#Delta#phi_{jZ}}";
    xTitleSize = 0.0525;
    yTitleSize = 0.0525;
    xTitleOffset = 1.25;
    yTitleOffset = 1.5;
    xTitleFont = 42;
    yTitleFont = 42;

    yMin = -0.2;
    yMax = 3.1;

    enum HISTLABELS {
        k_pbpb,
        kN_HISTLABELS
    };

    histPaths = {
            "h1D_dphijz_pbpb_cent030"
    };
    markerColors = {kBlack};
    markerStyles = {kFullCircle};
    markerSizes = {2};
    lineColors = {46};
    lineTransparencies = {0.7};
    lineWidths = {4};
    fillColors = {46};
    fillTransparencies = {0.7};
    drawOptions = {"e same"};
    sysPaths = {
            "h1D_sysVar_dphijz_pbpb_cent030_diff"
    };
    sysUseRelUnc = {false};
    sysColors = {46};
    sysTransparencies = {0.7};
    sysFillStyles = {1001};

    int nHistPaths = histPaths.size();
    std::vector<TH1D*> h1Ds(nHistPaths, 0);
    std::vector<TH1D*> h1DsSys(nHistPaths, 0);
    TGraph* gr = 0;
    TH1D* hTmp = 0;
    for (int i = 0; i < nHistPaths; ++i) {

        h1Ds[i] = (TH1D*)input->Get(histPaths[i].c_str());
        setTH1D(i, h1Ds[i]);

        if (i == 0) {
            hTmp = (TH1D*)h1Ds[i]->Clone(Form("%s_tmpDraw", h1Ds[i]->GetName()));
            hTmp->Draw("e");
        }
    }

    enum GRAPHLABELS {
        k_jewel,
        k_hybrid_coll,
        k_hybrid_rad,
        k_hybrid_strong,
        kN_GRAPHLABELS
    };

    graphPaths = {
            "THEORY/gr_dphijz_pbpb_cent030_theory_JEWEL",
            "THEORY/gr_dphijz_pbpb_cent030_theory_HYBRID_coll",
            "THEORY/gr_dphijz_pbpb_cent030_theory_HYBRID_rad",
            "THEORY/gr_dphijz_pbpb_cent030_theory_HYBRID_strong"
    };
    graphMarkerColors = {TColor::GetColor("#1034A6"),
            TColor::GetColor("#A0522D"), TColor::GetColor("#DAA520"), TColor::GetColor("#F4A460")};
    graphMarkerTransparencies = {0.85,
            0.7, 0.7, 0.7};
    graphMarkerStyles = {kFullStar,
            kFullCircle, kFullCircle, kFullCircle};
    graphMarkerSizes = {3.2, 0, 0, 0};
    graphLineColors = {TColor::GetColor("#1034A6"),
            TColor::GetColor("#A0522D"), TColor::GetColor("#DAA520"), TColor::GetColor("#F4A460")};
    graphLineTransparencies = {0.85,
                0.7, 0.7, 0.7};
    graphLineWidths = {4, 0, 0, 0};
    graphLineStyles = {kSolid, kSolid, kSolid, kSolid};
    graphFillColors = {0,
            TColor::GetColor("#A0522D"), TColor::GetColor("#DAA520"), TColor::GetColor("#F4A460")};
    graphFillTransparencies = {0, 0.7, 0.7, 0.7};
    graphDrawOptions = {"lp", "f", "f", "f"};

    int nGraphPaths = graphPaths.size();
    std::vector<TGraphErrors*> graphs(nGraphPaths, 0);
    for (int i = 0; i < nGraphPaths; ++i) {

        graphs[i] = (TGraphErrors*)input->Get(graphPaths[i].c_str());
        setTGraph(i, graphs[i]);
    }

    h1DsSys[k_pbpb] = (TH1D*)input->Get(sysPaths[k_pbpb].c_str());
    if (h1DsSys[k_pbpb] != 0) {
        gr = new TGraph();
        setTGraphSys(k_pbpb, gr);
        drawSysUncBoxes(gr, h1Ds[k_pbpb], h1DsSys[k_pbpb], sysUseRelUnc[k_pbpb]);
    }
    h1Ds[k_pbpb]->Draw(drawOptions[k_pbpb].c_str());

    graphs[k_hybrid_coll]->Draw(graphDrawOptions[k_hybrid_coll].c_str());
    graphs[k_hybrid_rad]->Draw(graphDrawOptions[k_hybrid_rad].c_str());
    graphs[k_hybrid_strong]->Draw(graphDrawOptions[k_hybrid_strong].c_str());
    graphs[k_jewel]->Draw(graphDrawOptions[k_jewel].c_str());

    h1Ds[k_pbpb]->Draw("e same");

    TLegend* leg = 0;
    legendX1 = 0.25;
    legendY1 = 0.73;
    legendWidth = 0.44;
    legendHeight = 0.06;
    legendMargin = 0.15;
    legendEntryTexts = {"PbPb, 0-30 %"};
    legendEntryOptions = {"pf"};
    leg = new TLegend();

    hTmp = (TH1D*)h1Ds[k_pbpb]->Clone(Form("%s_tmp", h1Ds[k_pbpb]->GetName()));
    hTmp->SetLineWidth(0);
    leg->AddEntry(hTmp, legendEntryTexts[k_pbpb].c_str(), legendEntryOptions[k_pbpb].c_str());

    setLegend(leg);
    leg->Draw();

    legendEntryTexts = {"JEWEL",
            "dE/dx #alpha T^{2}", "dE/dx #alpha T^{3}", "Strong Coupling"};
    legendEntryOptions = {"lp",
            "f", "f", "f"};
    legendX1 = 0.25;
    legendY1 = 0.6624;
    legendWidth = 0.462;
    legendHeight = 0.0576;
    legendMargin = 0.15;
    leg = new TLegend();

    gr = (TGraphErrors*)graphs[k_jewel]->Clone(Form("%s_tmp", graphs[k_jewel]->GetName()));
    leg->AddEntry(gr, legendEntryTexts[k_jewel].c_str(), legendEntryOptions[k_jewel].c_str());

    setLegend(leg);
    leg->Draw();

    legendX1 = 0.25;
    legendY1 = 0.4704;
    legendWidth = 0.462;
    legendHeight = 0.192;
    legendMargin = 0.15;
    leg = new TLegend();

    leg->SetHeader("      Hybrid");
    gr = (TGraphErrors*)graphs[k_hybrid_coll]->Clone(Form("%s_tmp", graphs[k_hybrid_coll]->GetName()));
    leg->AddEntry(gr, legendEntryTexts[k_hybrid_coll].c_str(), legendEntryOptions[k_hybrid_coll].c_str());
    gr = (TGraphErrors*)graphs[k_hybrid_rad]->Clone(Form("%s_tmp", graphs[k_hybrid_rad]->GetName()));
    leg->AddEntry(gr, legendEntryTexts[k_hybrid_rad].c_str(), legendEntryOptions[k_hybrid_rad].c_str());
    gr = (TGraphErrors*)graphs[k_hybrid_strong]->Clone(Form("%s_tmp", graphs[k_hybrid_strong]->GetName()));
    leg->AddEntry(gr, legendEntryTexts[k_hybrid_strong].c_str(), legendEntryOptions[k_hybrid_strong].c_str());

    setLegend(leg);
    leg->Draw();

    textX = 0.25;
    textYs = {0.43, 0.3762, 0.3234, 0.26656};
    textAlign = 11;
    textFont = 43;
    textSize = 29;
    textLines = {
            "p_{T}^{Z} > 60 GeV/c",
            "anti-k_{T} jet R = 0.3",
            "p_{T}^{jet} > 30 GeV/c",
            "|#eta^{jet}| < 1.6"
    };
    int nTextLines = textLines.size();
    TLatex* latex = 0;
    for (int i = 0; i < nTextLines; ++i) {
        latex = new TLatex();
        setLatex(i, latex);
        latex->Draw();
    }

    textXsOverPad = {0.22, 0.96};
    textYOverPad = 0.96;
    textAlignsOverPad = {11, 31};
    textFontOverPad = 43;
    textSizeOverPad = 30;
    textOverPadLines = {
            "#sqrt{s_{NN}} = 5.02 TeV",
            "PbPb 404 #mub^{-1}"
    };
    int nTextOverPadLines = textOverPadLines.size();
    for (int i = 0; i < nTextOverPadLines; ++i) {
        latex = new TLatex();
        setLatexOverPad(i, latex);
        latex->Draw();
    }

    textXCMS = 0.25;
    textYCMS = 0.86;
    textAlignCMS = 11;
    textFontCMS = 61;
    textSizeCMS = 0.06299999;
    latex = new TLatex();
    setLatexCMS(latex);
    latex->Draw();

    c->Update();

    TLine* line = new TLine(gPad->GetUxmin(), 0, gPad->GetUxmax(), 0);
    line->SetLineStyle(kDashed);
    line->Draw();

    c->SaveAs(Form("%s.pdf", c->GetName()));
    c->Close();         // do not use Delete() for TCanvas.

    std::cout<<"Closing the input file"<<std::endl;
    input->Close();

    std::cout<<"running zJetPlot_dphijz_pbpb_Theory() - END"<<std::endl;
}

void zJetPlot_rjz_pp_Theory(std::string inputFile)
{
    std::cout<<"running zJetPlot_rjz_pp_Theory()"<<std::endl;

    TFile* input  = TFile::Open(inputFile.c_str());

    // no horizontal error bars
    gStyle->SetErrorX(0);
    gStyle->SetHatchesLineWidth(3);

    windowWidth = 800;
    windowHeight = 800;
    logX = 0;
    logY = 0;
    leftMargin   = 0.21;
    rightMargin  = 0.03;
    bottomMargin = 0.15;
    topMargin    = 0.06;
    TCanvas* c = new TCanvas(figureNames[k_rjz_pp_Theory].c_str(), "", windowWidth, windowHeight);
    std::cout<<"preparing canvas : "<< c->GetName() <<std::endl;
    setCanvas(c);
    c->cd();

    xTitle = "p^{Z}_{T} (GeV/c)";
    yTitle = "R_{jZ}";
    xTitleSize = 0.0525;
    yTitleSize = 0.0525;
    xTitleOffset = 1.25;
    yTitleOffset = 1.5;
    xTitleFont = 42;
    yTitleFont = 42;

    yMin = 0;
    yMax = 1;

    enum HISTLABELS {
        k_pp,
        k_madgraph,
        kN_HISTLABELS
    };

    histPaths = {
            "h1D_rjz_pp",
            "h1D_rjz_madgraph"
    };
    markerColors = {kBlack, kBlack};
    markerStyles = {kFullCircle, kFullSquare};
    markerSizes = {2, 2};
    lineColors = {kBlack, TColor::GetColor("#C71585")};
    lineTransparencies = {1.0, 0.8};
    lineWidths = {4, 5};
    fillColors = {30, 0};
    fillTransparencies = {0.7, 0};
    drawOptions = {"e same", "l hist same"};
    sysPaths = {
            "h1D_sysVar_rjz_pp_rel",
            "NULL"
    };
    sysUseRelUnc = {true, true};
    sysColors = {30, 30};
    sysTransparencies = {0.7, 0.7};
    sysFillStyles = {1001, 1001};

    int nHistPaths = histPaths.size();
    std::vector<TH1D*> h1Ds(nHistPaths, 0);
    std::vector<TH1D*> h1DsSys(nHistPaths, 0);
    TGraph* gr = 0;
    TH1D* hTmp = 0;
    for (int i = 0; i < nHistPaths; ++i) {

        h1Ds[i] = (TH1D*)input->Get(histPaths[i].c_str());
        setTH1D(i, h1Ds[i]);

        if (i == 0) {
            hTmp = (TH1D*)h1Ds[i]->Clone(Form("%s_tmpDraw", h1Ds[i]->GetName()));
            hTmp->Draw("e");
        }
    }

    enum GRAPHLABELS {
        k_jewel,
        k_hybrid,
        kN_GRAPHLABELS
    };

    gStyle->SetLineStyleString(14,"20 8");

    graphPaths = {
            "THEORY/gr_rjz_pp_theory_JEWEL",
            "THEORY/gr_rjz_pp_theory_HYBRID"
    };
    graphMarkerColors = {TColor::GetColor("#1034A6"), TColor::GetColor("#A0522D")};
    graphMarkerTransparencies = {0.85, 0.8};
    graphMarkerStyles = {kFullStar, kFullCircle};
    graphMarkerSizes = {3.2, 0};
    graphLineColors = {TColor::GetColor("#1034A6"), TColor::GetColor("#A0522D")};
    graphLineTransparencies = {0.85, 0.8};
    graphLineWidths = {4, 5};
    graphLineStyles = {kSolid, 14};
    graphFillColors = {0, 0};
    graphFillTransparencies = {0, 0};
    graphDrawOptions = {"l", "l"};

    int nGraphPaths = graphPaths.size();
    std::vector<TGraphErrors*> graphs(nGraphPaths, 0);
    for (int i = 0; i < nGraphPaths; ++i) {

        graphs[i] = (TGraphErrors*)input->Get(graphPaths[i].c_str());
        setTGraph(i, graphs[i]);
    }

    h1DsSys[k_pp] = (TH1D*)input->Get(sysPaths[k_pp].c_str());
    if (h1DsSys[k_pp] != 0) {
        gr = new TGraph();
        setTGraphSys(k_pp, gr);
        drawSysUncBoxes(gr, h1Ds[k_pp], h1DsSys[k_pp], sysUseRelUnc[k_pp]);
    }
    h1Ds[k_pp]->Draw(drawOptions[k_pp].c_str());

    graphs[k_hybrid]->Draw(graphDrawOptions[k_hybrid].c_str());
    graphs[k_jewel]->Draw(graphDrawOptions[k_jewel].c_str());
    graphs[k_jewel]->Draw("p");
    h1Ds[k_madgraph]->Draw(drawOptions[k_madgraph].c_str());

    h1Ds[k_pp]->Draw("e same");

    TLegend* leg = 0;
    legendX1 = 0.25;
    legendY1 = 0.73;
    legendWidth = 0.44;
    legendHeight = 0.06;
    legendMargin = 0.15;
    legendEntryTexts = {"pp"};
    legendEntryOptions = {"pf"};
    leg = new TLegend();

    hTmp = (TH1D*)h1Ds[k_pp]->Clone(Form("%s_tmp", h1Ds[k_pp]->GetName()));
    hTmp->SetLineWidth(0);
    leg->AddEntry(hTmp, legendEntryTexts[k_pp].c_str(), legendEntryOptions[k_pp].c_str());

    setLegend(leg);
    leg->Draw();

    legendX1 = 0.61;
    legendY1 = 0.784;
    legendWidth = 0.462;
    legendHeight = 0.126;
    legendMargin = 0.15;
    legendEntryTexts = {"JEWEL ref.", "Hybrid ref.", "MG5aMC@NLO"};
    legendEntryOptions = {"lp", "l", "l"};
    leg = new TLegend();

    gr = (TGraphErrors*)graphs[k_jewel]->Clone(Form("%s_tmp", graphs[k_jewel]->GetName()));
    leg->AddEntry(gr, legendEntryTexts[k_jewel].c_str(), legendEntryOptions[k_jewel].c_str());
    gr = (TGraphErrors*)graphs[k_hybrid]->Clone(Form("%s_tmp", graphs[k_hybrid]->GetName()));
    leg->AddEntry(gr, legendEntryTexts[k_hybrid].c_str(), legendEntryOptions[k_hybrid].c_str());
    hTmp = (TH1D*)h1Ds[k_madgraph]->Clone(Form("%s_tmp", h1Ds[k_madgraph]->GetName()));
    leg->AddEntry(hTmp, legendEntryTexts[2].c_str(), legendEntryOptions[2].c_str());

    setLegend(leg);
    leg->Draw();

    textX = 0.93;
    textYs = {0.37, 0.3168, 0.2646, 0.2134, 0.3648};
    textAlign = 31;
    textFont = 43;
    textSize = 29;
    textLines = {
            "anti-k_{T} jet R = 0.3",
            "p_{T}^{jet} > 30 GeV/c",
            "|#eta^{jet}| < 1.6",
            "#Delta#phi_{jZ} > #frac{7}{8}#pi"
    };
    int nTextLines = textLines.size();
    TLatex* latex = 0;
    for (int i = 0; i < nTextLines; ++i) {
        latex = new TLatex();
        setLatex(i, latex);
        latex->Draw();
    }

    textXsOverPad = {0.22, 0.96};
    textYOverPad = 0.96;
    textAlignsOverPad = {11, 31};
    textFontOverPad = 43;
    textSizeOverPad = 30;
    textOverPadLines = {
            "#sqrt{s} = 5.02 TeV",
            "pp 27.4 pb^{-1}"
    };
    int nTextOverPadLines = textOverPadLines.size();
    for (int i = 0; i < nTextOverPadLines; ++i) {
        latex = new TLatex();
        setLatexOverPad(i, latex);
        latex->Draw();
    }

    textXCMS = 0.25;
    textYCMS = 0.86;
    textAlignCMS = 11;
    textFontCMS = 61;
    textSizeCMS = 0.06299999;
    latex = new TLatex();
    setLatexCMS(latex);
    latex->Draw();

    c->Update();

    c->SaveAs(Form("%s.pdf", c->GetName()));
    c->Close();         // do not use Delete() for TCanvas.

    std::cout<<"Closing the input file"<<std::endl;
    input->Close();

    std::cout<<"running zJetPlot_rjz_pp_Theory() - END"<<std::endl;
}

void zJetPlot_rjz_pbpb_Theory(std::string inputFile)
{
    std::cout<<"running zJetPlot_rjz_pbpb_Theory()"<<std::endl;

    TFile* input  = TFile::Open(inputFile.c_str());

    // no horizontal error bars
    gStyle->SetErrorX(0);
    gStyle->SetHatchesLineWidth(3);

    windowWidth = 800;
    windowHeight = 800;
    logX = 0;
    logY = 0;
    leftMargin   = 0.21;
    rightMargin  = 0.03;
    bottomMargin = 0.15;
    topMargin    = 0.06;
    TCanvas* c = new TCanvas(figureNames[k_rjz_pbpb_Theory].c_str(), "", windowWidth, windowHeight);
    std::cout<<"preparing canvas : "<< c->GetName() <<std::endl;
    setCanvas(c);
    c->cd();

    xTitle = "p^{Z}_{T} (GeV/c)";
    yTitle = "R_{jZ}";
    xTitleSize = 0.0525;
    yTitleSize = 0.0525;
    xTitleOffset = 1.25;
    yTitleOffset = 1.5;
    xTitleFont = 42;
    yTitleFont = 42;

    yMin = 0;
    yMax = 1;

    enum HISTLABELS {
        k_pbpb,
        kN_HISTLABELS
    };

    histPaths = {
            "h1D_rjz_pbpb_cent030"
    };
    markerColors = {kBlack};
    markerStyles = {kFullCircle};
    markerSizes = {2};
    lineColors = {46};
    lineTransparencies = {0.7};
    lineWidths = {4};
    fillColors = {46};
    fillTransparencies = {0.7};
    drawOptions = {"e same"};
    sysPaths = {
            "h1D_sysVar_rjz_pbpb_cent030_rel"
    };
    sysUseRelUnc = {true};
    sysColors = {46};
    sysTransparencies = {0.7};
    sysFillStyles = {1001};

    int nHistPaths = histPaths.size();
    std::vector<TH1D*> h1Ds(nHistPaths, 0);
    std::vector<TH1D*> h1DsSys(nHistPaths, 0);
    TGraph* gr = 0;
    TH1D* hTmp = 0;
    for (int i = 0; i < nHistPaths; ++i) {

        h1Ds[i] = (TH1D*)input->Get(histPaths[i].c_str());
        setTH1D(i, h1Ds[i]);

        if (i == 0) {
            hTmp = (TH1D*)h1Ds[i]->Clone(Form("%s_tmpDraw", h1Ds[i]->GetName()));
            hTmp->Draw("e");
        }
    }

    enum GRAPHLABELS {
        k_hybrid_coll,
        k_hybrid_rad,
        k_hybrid_strong,
        k_jewel,
        kN_GRAPHLABELS
    };

    graphPaths = {
            "THEORY/gr_rjz_pbpb_cent030_theory_HYBRID_coll",
            "THEORY/gr_rjz_pbpb_cent030_theory_HYBRID_rad",
            "THEORY/gr_rjz_pbpb_cent030_theory_HYBRID_strong",
            "THEORY/gr_rjz_pbpb_cent030_theory_JEWEL"
    };
    graphMarkerColors = {TColor::GetColor("#A0522D"), TColor::GetColor("#DAA520"), TColor::GetColor("#F4A460"), TColor::GetColor("#1034A6")};
    graphMarkerTransparencies = {0.7, 0.7, 0.7, 0.85};
    graphMarkerStyles = {kFullCircle, kFullCircle, kFullCircle, kFullStar};
    graphMarkerSizes = {0, 0, 0, 3.2};
    graphLineColors = {TColor::GetColor("#A0522D"), TColor::GetColor("#DAA520"), TColor::GetColor("#F4A460"), TColor::GetColor("#1034A6")};
    graphLineTransparencies = {0.7, 0.7, 0.7, 0.85};
    graphLineWidths = {0, 0, 0, 4};
    graphLineStyles = {kSolid, kSolid, kSolid, kSolid};
    graphFillColors = {TColor::GetColor("#A0522D"), TColor::GetColor("#DAA520"), TColor::GetColor("#F4A460"), 0};
    graphFillTransparencies = {0.7, 0.7, 0.7, 0};
    graphDrawOptions = {"f", "f", "f", "lp"};

    int nGraphPaths = graphPaths.size();
    std::vector<TGraphErrors*> graphs(nGraphPaths, 0);
    for (int i = 0; i < nGraphPaths; ++i) {

        graphs[i] = (TGraphErrors*)input->Get(graphPaths[i].c_str());
        setTGraph(i, graphs[i]);
    }

    h1DsSys[k_pbpb] = (TH1D*)input->Get(sysPaths[k_pbpb].c_str());
    if (h1DsSys[k_pbpb] != 0) {
        gr = new TGraph();
        setTGraphSys(k_pbpb, gr);
        drawSysUncBoxes(gr, h1Ds[k_pbpb], h1DsSys[k_pbpb], sysUseRelUnc[k_pbpb]);
    }
    h1Ds[k_pbpb]->Draw(drawOptions[k_pbpb].c_str());

    graphs[k_hybrid_coll]->Draw(graphDrawOptions[k_hybrid_coll].c_str());
    graphs[k_hybrid_rad]->Draw(graphDrawOptions[k_hybrid_rad].c_str());
    graphs[k_hybrid_strong]->Draw(graphDrawOptions[k_hybrid_strong].c_str());
    graphs[k_jewel]->Draw(graphDrawOptions[k_jewel].c_str());

    h1Ds[k_pbpb]->Draw("e same");

    TLegend* leg = 0;
    legendX1 = 0.25;
    legendY1 = 0.73;
    legendWidth = 0.44;
    legendHeight = 0.06;
    legendMargin = 0.15;
    legendEntryTexts = {"PbPb, 0-30 %"};
    legendEntryOptions = {"pf"};
    leg = new TLegend();

    hTmp = (TH1D*)h1Ds[k_pbpb]->Clone(Form("%s_tmp", h1Ds[k_pbpb]->GetName()));
    hTmp->SetLineWidth(0);
    leg->AddEntry(hTmp, legendEntryTexts[k_pbpb].c_str(), legendEntryOptions[k_pbpb].c_str());

    setLegend(leg);
    leg->Draw();

    legendEntryTexts = {"dE/dx #alpha T^{2}", "dE/dx #alpha T^{3}", "Strong Coupling", "JEWEL"};
    legendEntryOptions = {"f", "f", "f", "lp"};
    legendX1 = 0.61;
    legendY1 = 0.73;
    legendWidth = 0.484;
    legendHeight = 0.18;
    legendMargin = 0.15;
    leg = new TLegend();

    gr = (TGraphErrors*)graphs[k_hybrid_coll]->Clone(Form("%s_tmp", graphs[k_hybrid_coll]->GetName()));
    leg->AddEntry(gr, legendEntryTexts[k_hybrid_coll].c_str(), legendEntryOptions[k_hybrid_coll].c_str());
    gr = (TGraphErrors*)graphs[k_hybrid_rad]->Clone(Form("%s_tmp", graphs[k_hybrid_rad]->GetName()));
    leg->AddEntry(gr, legendEntryTexts[k_hybrid_rad].c_str(), legendEntryOptions[k_hybrid_rad].c_str());
    gr = (TGraphErrors*)graphs[k_hybrid_strong]->Clone(Form("%s_tmp", graphs[k_hybrid_strong]->GetName()));
    leg->AddEntry(gr, legendEntryTexts[k_hybrid_strong].c_str(), legendEntryOptions[k_hybrid_strong].c_str());
    gr = (TGraphErrors*)graphs[k_jewel]->Clone(Form("%s_tmp", graphs[k_jewel]->GetName()));
    leg->AddEntry(gr, legendEntryTexts[k_jewel].c_str(), legendEntryOptions[k_jewel].c_str());

    setLegend(leg);
    leg->Draw();

    textX = 0.93;
    textYs = {0.37, 0.3168, 0.2646, 0.2134};
    textAlign = 31;
    textFont = 43;
    textSize = 29;
    textLines = {
            "anti-k_{T} jet R = 0.3",
            "p_{T}^{jet} > 30 GeV/c",
            "|#eta^{jet}| < 1.6",
            "#Delta#phi_{jZ} > #frac{7}{8}#pi"
    };
    int nTextLines = textLines.size();
    TLatex* latex = 0;
    for (int i = 0; i < nTextLines; ++i) {
        latex = new TLatex();
        setLatex(i, latex);
        latex->Draw();
    }

    textXsOverPad = {0.22, 0.96};
    textYOverPad = 0.96;
    textAlignsOverPad = {11, 31};
    textFontOverPad = 43;
    textSizeOverPad = 30;
    textOverPadLines = {
            "#sqrt{s_{NN}} = 5.02 TeV",
            "PbPb 404 #mub^{-1}"
    };
    int nTextOverPadLines = textOverPadLines.size();
    for (int i = 0; i < nTextOverPadLines; ++i) {
        latex = new TLatex();
        setLatexOverPad(i, latex);
        latex->Draw();
    }

    textXCMS = 0.25;
    textYCMS = 0.86;
    textAlignCMS = 11;
    textFontCMS = 61;
    textSizeCMS = 0.06299999;
    latex = new TLatex();
    setLatexCMS(latex);
    latex->Draw();

    c->Update();

    c->SaveAs(Form("%s.pdf", c->GetName()));
    c->Close();         // do not use Delete() for TCanvas.

    std::cout<<"Closing the input file"<<std::endl;
    input->Close();

    std::cout<<"running zJetPlot_rjz_pbpb_Theory() - END"<<std::endl;
}

void zJetPlot_xjzMean_pp_Theory(std::string inputFile)
{
    std::cout<<"running zJetPlot_xjzMean_pp_Theory()"<<std::endl;

    TFile* input  = TFile::Open(inputFile.c_str());

    // no horizontal error bars
    gStyle->SetErrorX(0);
    gStyle->SetHatchesLineWidth(3);

    windowWidth = 800;
    windowHeight = 800;
    logX = 0;
    logY = 0;
    leftMargin   = 0.21;
    rightMargin  = 0.03;
    bottomMargin = 0.15;
    topMargin    = 0.06;
    TCanvas* c = new TCanvas(figureNames[k_xjzMean_pp_Theory].c_str(), "", windowWidth, windowHeight);
    std::cout<<"preparing canvas : "<< c->GetName() <<std::endl;
    setCanvas(c);
    c->cd();

    xTitle = "p^{Z}_{T} (GeV/c)";
    yTitle = "<x_{jZ}>";
    xTitleSize = 0.0525;
    yTitleSize = 0.0525;
    xTitleOffset = 1.25;
    yTitleOffset = 1.5;
    xTitleFont = 42;
    yTitleFont = 42;

    yMin = 0.6;
    yMax = 1.1;

    enum HISTLABELS {
        k_pp,
        k_madgraph,
        kN_HISTLABELS
    };

    histPaths = {
            "h1D_xjzMean_pp",
            "h1D_xjzMean_madgraph"
    };
    markerColors = {kBlack, kBlack};
    markerStyles = {kFullCircle, kFullSquare};
    markerSizes = {2, 2};
    lineColors = {kBlack, TColor::GetColor("#C71585")};
    lineTransparencies = {1.0, 0.8};
    lineWidths = {4, 5};
    fillColors = {30, 0};
    fillTransparencies = {0.7, 0};
    drawOptions = {"e same", "l hist same"};
    sysPaths = {
            "h1D_sysVar_xjzMean_pp_rel",
            "NULL"
    };
    sysUseRelUnc = {true, true};
    sysColors = {30, 30};
    sysTransparencies = {0.7, 0.7};
    sysFillStyles = {1001, 1001};

    int nHistPaths = histPaths.size();
    std::vector<TH1D*> h1Ds(nHistPaths, 0);
    std::vector<TH1D*> h1DsSys(nHistPaths, 0);
    TGraph* gr = 0;
    TH1D* hTmp = 0;
    for (int i = 0; i < nHistPaths; ++i) {

        h1Ds[i] = (TH1D*)input->Get(histPaths[i].c_str());
        setTH1D(i, h1Ds[i]);

        if (i == 0) {
            hTmp = (TH1D*)h1Ds[i]->Clone(Form("%s_tmpDraw", h1Ds[i]->GetName()));
            hTmp->Draw("e");
        }
    }

    enum GRAPHLABELS {
        k_jewel,
        kN_GRAPHLABELS
    };

    graphPaths = {
            "THEORY/gr_xjzMean_pp_theory_JEWEL"
    };
    graphMarkerColors = {TColor::GetColor("#1034A6")};
    graphMarkerTransparencies = {0.85};
    graphMarkerStyles = {kFullStar};
    graphMarkerSizes = {3.2};
    graphLineColors = {TColor::GetColor("#1034A6")};
    graphLineTransparencies = {0.85};
    graphLineWidths = {4};
    graphLineStyles = {kSolid};
    graphFillColors = {0};
    graphFillTransparencies = {0};
    graphDrawOptions = {"l"};

    int nGraphPaths = graphPaths.size();
    std::vector<TGraphErrors*> graphs(nGraphPaths, 0);
    for (int i = 0; i < nGraphPaths; ++i) {

        graphs[i] = (TGraphErrors*)input->Get(graphPaths[i].c_str());
        setTGraph(i, graphs[i]);
    }

    h1DsSys[k_pp] = (TH1D*)input->Get(sysPaths[k_pp].c_str());
    if (h1DsSys[k_pp] != 0) {
        gr = new TGraph();
        setTGraphSys(k_pp, gr);
        drawSysUncBoxes(gr, h1Ds[k_pp], h1DsSys[k_pp], sysUseRelUnc[k_pp]);
    }
    h1Ds[k_pp]->Draw(drawOptions[k_pp].c_str());

    graphs[k_jewel]->Draw(graphDrawOptions[k_jewel].c_str());
    graphs[k_jewel]->Draw("p");
    h1Ds[k_madgraph]->Draw(drawOptions[k_madgraph].c_str());

    h1Ds[k_pp]->Draw("e same");

    TLegend* leg = 0;
    legendX1 = 0.6;
    legendY1 = 0.84;
    legendWidth = 0.44;
    legendHeight = 0.06;
    legendMargin = 0.15;
    legendEntryTexts = {"pp"};
    legendEntryOptions = {"pf"};
    leg = new TLegend();

    hTmp = (TH1D*)h1Ds[k_pp]->Clone(Form("%s_tmp", h1Ds[k_pp]->GetName()));
    hTmp->SetLineWidth(0);
    leg->AddEntry(hTmp, legendEntryTexts[k_pp].c_str(), legendEntryOptions[k_pp].c_str());

    setLegend(leg);
    leg->Draw();

    legendX1 = 0.60;
    legendY1 = 0.75;
    legendWidth = 0.462;
    legendHeight = 0.09;
    legendMargin = 0.15;
    legendEntryTexts = {"JEWEL ref.", "MG5aMC@NLO"};
    legendEntryOptions = {"lp", "l"};
    leg = new TLegend();

    gr = (TGraphErrors*)graphs[k_jewel]->Clone(Form("%s_tmp", graphs[k_jewel]->GetName()));
    leg->AddEntry(gr, legendEntryTexts[k_jewel].c_str(), legendEntryOptions[k_jewel].c_str());
    hTmp = (TH1D*)h1Ds[k_madgraph]->Clone(Form("%s_tmp", h1Ds[k_madgraph]->GetName()));
    leg->AddEntry(hTmp, legendEntryTexts[1].c_str(), legendEntryOptions[1].c_str());

    setLegend(leg);
    leg->Draw();

    textX = 0.93;
    textYs = {0.7, 0.6435, 0.5879999, 0.5335};
    textAlign = 31;
    textFont = 43;
    textSize = 29;
    textLines = {
            "anti-k_{T} jet R = 0.3",
            "p_{T}^{jet} > 30 GeV/c",
            "|#eta^{jet}| < 1.6",
            "#Delta#phi_{jZ} > #frac{7}{8}#pi"
    };
    int nTextLines = textLines.size();
    TLatex* latex = 0;
    for (int i = 0; i < nTextLines; ++i) {
        latex = new TLatex();
        setLatex(i, latex);
        latex->Draw();
    }

    textXsOverPad = {0.22, 0.96};
    textYOverPad = 0.96;
    textAlignsOverPad = {11, 31};
    textFontOverPad = 43;
    textSizeOverPad = 30;
    textOverPadLines = {
            "#sqrt{s} = 5.02 TeV",
            "pp 27.4 pb^{-1}"
    };
    int nTextOverPadLines = textOverPadLines.size();
    for (int i = 0; i < nTextOverPadLines; ++i) {
        latex = new TLatex();
        setLatexOverPad(i, latex);
        latex->Draw();
    }

    textXCMS = 0.33;
    textYCMS = 0.86;
    textAlignCMS = 11;
    textFontCMS = 61;
    textSizeCMS = 0.06299999;
    latex = new TLatex();
    setLatexCMS(latex);
    latex->Draw();

    c->Update();

    c->SaveAs(Form("%s.pdf", c->GetName()));
    c->Close();         // do not use Delete() for TCanvas.

    std::cout<<"Closing the input file"<<std::endl;
    input->Close();

    std::cout<<"running zJetPlot_xjzMean_pp_Theory() - END"<<std::endl;
}

void zJetPlot_xjzMean_pbpb_Theory(std::string inputFile)
{
    std::cout<<"running zJetPlot_xjzMean_pbpb_Theory()"<<std::endl;

    TFile* input  = TFile::Open(inputFile.c_str());

    // no horizontal error bars
    gStyle->SetErrorX(0);
    gStyle->SetHatchesLineWidth(3);

    windowWidth = 800;
    windowHeight = 800;
    logX = 0;
    logY = 0;
    leftMargin   = 0.21;
    rightMargin  = 0.03;
    bottomMargin = 0.15;
    topMargin    = 0.06;
    TCanvas* c = new TCanvas(figureNames[k_xjzMean_pbpb_Theory].c_str(), "", windowWidth, windowHeight);
    std::cout<<"preparing canvas : "<< c->GetName() <<std::endl;
    setCanvas(c);
    c->cd();

    xTitle = "p^{Z}_{T} (GeV/c)";
    yTitle = "<x_{jZ}>";
    xTitleSize = 0.0525;
    yTitleSize = 0.0525;
    xTitleOffset = 1.25;
    yTitleOffset = 1.5;
    xTitleFont = 42;
    yTitleFont = 42;

    yMin = 0.6;
    yMax = 1.1;

    enum HISTLABELS {
        k_pbpb,
        kN_HISTLABELS
    };

    histPaths = {
            "h1D_xjzMean_pbpb_cent030"
    };
    markerColors = {kBlack};
    markerStyles = {kFullCircle};
    markerSizes = {2};
    lineColors = {46};
    lineTransparencies = {0.7};
    lineWidths = {4};
    fillColors = {46};
    fillTransparencies = {0.7};
    drawOptions = {"e same"};
    sysPaths = {
            "h1D_sysVar_xjzMean_pbpb_cent030_rel"
    };
    sysUseRelUnc = {true};
    sysColors = {46};
    sysTransparencies = {0.7};
    sysFillStyles = {1001};

    int nHistPaths = histPaths.size();
    std::vector<TH1D*> h1Ds(nHistPaths, 0);
    std::vector<TH1D*> h1DsSys(nHistPaths, 0);
    TGraph* gr = 0;
    TH1D* hTmp = 0;
    for (int i = 0; i < nHistPaths; ++i) {

        h1Ds[i] = (TH1D*)input->Get(histPaths[i].c_str());
        setTH1D(i, h1Ds[i]);

        if (i == 0) {
            hTmp = (TH1D*)h1Ds[i]->Clone(Form("%s_tmpDraw", h1Ds[i]->GetName()));
            hTmp->Draw("e");
        }
    }

    enum GRAPHLABELS {
        k_jewel,
        kN_GRAPHLABELS
    };

    graphPaths = {
            "THEORY/gr_xjzMean_pbpb_cent030_theory_JEWEL"
    };
    graphMarkerColors = {TColor::GetColor("#1034A6")};
    graphMarkerTransparencies = {0.85};
    graphMarkerStyles = {kFullStar};
    graphMarkerSizes = {3.2};
    graphLineColors = {TColor::GetColor("#1034A6")};
    graphLineTransparencies = {0.85};
    graphLineWidths = {4};
    graphLineStyles = {kSolid};
    graphFillColors = {0};
    graphFillTransparencies = {0};
    graphDrawOptions = {"lp"};

    int nGraphPaths = graphPaths.size();
    std::vector<TGraphErrors*> graphs(nGraphPaths, 0);
    for (int i = 0; i < nGraphPaths; ++i) {

        graphs[i] = (TGraphErrors*)input->Get(graphPaths[i].c_str());
        setTGraph(i, graphs[i]);
    }

    h1DsSys[k_pbpb] = (TH1D*)input->Get(sysPaths[k_pbpb].c_str());
    if (h1DsSys[k_pbpb] != 0) {
        gr = new TGraph();
        setTGraphSys(k_pbpb, gr);
        drawSysUncBoxes(gr, h1Ds[k_pbpb], h1DsSys[k_pbpb], sysUseRelUnc[k_pbpb]);
    }
    h1Ds[k_pbpb]->Draw(drawOptions[k_pbpb].c_str());

    graphs[k_jewel]->Draw(graphDrawOptions[k_jewel].c_str());

    h1Ds[k_pbpb]->Draw("e same");

    TLegend* leg = 0;
    legendX1 = 0.60;
    legendY1 = 0.84;
    legendWidth = 0.44;
    legendHeight = 0.06;
    legendMargin = 0.15;
    legendEntryTexts = {"PbPb, 0-30 %"};
    legendEntryOptions = {"pf"};
    leg = new TLegend();

    hTmp = (TH1D*)h1Ds[k_pbpb]->Clone(Form("%s_tmp", h1Ds[k_pbpb]->GetName()));
    hTmp->SetLineWidth(0);
    leg->AddEntry(hTmp, legendEntryTexts[k_pbpb].c_str(), legendEntryOptions[k_pbpb].c_str());

    setLegend(leg);
    leg->Draw();

    legendEntryTexts = {"JEWEL"};
    legendEntryOptions = {"lp"};
    legendX1 = 0.60;
    legendY1 = 0.78;
    legendWidth = 0.462;
    legendHeight = 0.06;
    legendMargin = 0.15;
    leg = new TLegend();

    gr = (TGraphErrors*)graphs[k_jewel]->Clone(Form("%s_tmp", graphs[k_jewel]->GetName()));
    leg->AddEntry(gr, legendEntryTexts[k_jewel].c_str(), legendEntryOptions[k_jewel].c_str());

    setLegend(leg);
    leg->Draw();

    textX = 0.93;
    textYs = {0.70, 0.6435, 0.5879999, 0.5335};
    textAlign = 31;
    textFont = 43;
    textSize = 29;
    textLines = {
            "anti-k_{T} jet R = 0.3",
            "p_{T}^{jet} > 30 GeV/c",
            "|#eta^{jet}| < 1.6",
            "#Delta#phi_{jZ} > #frac{7}{8}#pi"
    };
    int nTextLines = textLines.size();
    TLatex* latex = 0;
    for (int i = 0; i < nTextLines; ++i) {
        latex = new TLatex();
        setLatex(i, latex);
        latex->Draw();
    }

    textXsOverPad = {0.22, 0.96};
    textYOverPad = 0.96;
    textAlignsOverPad = {11, 31};
    textFontOverPad = 43;
    textSizeOverPad = 30;
    textOverPadLines = {
            "#sqrt{s_{NN}} = 5.02 TeV",
            "PbPb 404 #mub^{-1}"
    };
    int nTextOverPadLines = textOverPadLines.size();
    for (int i = 0; i < nTextOverPadLines; ++i) {
        latex = new TLatex();
        setLatexOverPad(i, latex);
        latex->Draw();
    }

    textXCMS = 0.33;
    textYCMS = 0.86;
    textAlignCMS = 11;
    textFontCMS = 61;
    textSizeCMS = 0.06299999;
    latex = new TLatex();
    setLatexCMS(latex);
    latex->Draw();

    c->Update();

    c->SaveAs(Form("%s.pdf", c->GetName()));
    c->Close();         // do not use Delete() for TCanvas.

    std::cout<<"Closing the input file"<<std::endl;
    input->Close();

    std::cout<<"running zJetPlot_xjzMean_pbpb_Theory() - END"<<std::endl;
}

void zJetPlot_M_Zee_pbpb(std::string inputFile)
{
    std::cout<<"running zJetPlot_M_Zee_pbpb()"<<std::endl;

    TFile* input  = TFile::Open(inputFile.c_str());

    windowWidth = 800;
    windowHeight = 800;
    logX = 0;
    logY = 0;
    leftMargin   = 0.17;
    rightMargin  = 0.05;
    bottomMargin = 0.15;
    topMargin    = 0.05;
    TCanvas* c = new TCanvas(figureNames[k_M_Zee_pbpb].c_str(), "", windowWidth, windowHeight);
    std::cout<<"preparing canvas : "<< c->GetName() <<std::endl;
    setCanvas(c);
    c->cd();

    xTitle = "M^{ee} (GeV/c^{2})";
    yTitle = "Entries / (2 GeV/c^{2})";
    xTitleSize = 0.042;
    yTitleSize = 0.042;
    xTitleOffset = 1.25;
    yTitleOffset = 1.7;
    xTitleFont = 42;
    yTitleFont = 42;

    yMin = 0;
    yMax = 42.78;

    enum HISTLABELS {
        k_pbpb_mc,
        k_pbpb_data,
        k_pbpb_data_sameCh,
        kN_HISTLABELS
    };

    histPaths = {
            "h1D_M_Zee_pbpbmc_cent030",
            "h1D_M_Zee_pbpb_cent030",
            "h1D_M_Zee_pbpb_cent030_sameCh"
    };
    markerColors = {kBlack, kBlack, kBlack};
    markerStyles = {kFullCircle, kFullCircle, kOpenSquare};
    markerSizes = {1.25, 1.25, 1.25};
    lineColors = {kOrange+7, TColor::GetColor("#000099"), TColor::GetColor("#000099")};
    lineTransparencies = {1.0, 1.0, 1.0};
    lineWidths = {2, 1, 1};
    fillColors = {kOrange-2, -1, -1};
    fillTransparencies = {1.0, 1.0, 1.0};
    drawOptions = {"hist", "e same", "e same"};

    int nHistPaths = histPaths.size();
    std::vector<TH1D*> h1Ds(nHistPaths, 0);
    TH1D* hTmp = 0;
    for (int i = 0; i < nHistPaths; ++i) {

        h1Ds[i] = (TH1D*)input->Get(histPaths[i].c_str());
        setTH1D(i, h1Ds[i]);

        h1Ds[i]->Draw(drawOptions[i].c_str());
    }

    std::vector<int> pairCounts(nHistPaths, 0);
    for (int i = 0; i < nHistPaths; ++i) {

        double massMin_count = 70;
        double massMax_count = 110;
        int binLow_count = -1;
        int binUp_count = -1;

        binLow_count = h1Ds[i]->FindBin(massMin_count);
        binUp_count = h1Ds[i]->FindBin(massMax_count) - 1;

        pairCounts[i] = (int)h1Ds[i]->Integral(binLow_count, binUp_count);
    }

    legendX1 = 0.21;
    legendY1 = 0.76;
    legendWidth = 0.45;
    legendHeight = 0.15;
    legendMargin = 0.15;
    legendEntryTexts = {
            "PYTHIA+HYDJET",
            Form("Opposite sign (%d counts)", pairCounts[k_pbpb_data]),
            Form("Same sign (%d counts)", pairCounts[k_pbpb_data_sameCh])
    };
    legendEntryOptions = {
            "lf",
            "lpf",
            "lpf"
    };
    TLegend* leg = new TLegend();

    hTmp = (TH1D*)h1Ds[k_pbpb_data]->Clone(Form("%s_tmp", h1Ds[k_pbpb_data]->GetName()));
    leg->AddEntry(hTmp, legendEntryTexts[k_pbpb_data].c_str(), legendEntryOptions[k_pbpb_data].c_str());
    hTmp = (TH1D*)h1Ds[k_pbpb_data_sameCh]->Clone(Form("%s_tmp", h1Ds[k_pbpb_data_sameCh]->GetName()));
    leg->AddEntry(hTmp, legendEntryTexts[k_pbpb_data_sameCh].c_str(), legendEntryOptions[k_pbpb_data_sameCh].c_str());
    hTmp = (TH1D*)h1Ds[k_pbpb_mc]->Clone(Form("%s_tmp", h1Ds[k_pbpb_mc]->GetName()));
    leg->AddEntry(hTmp, legendEntryTexts[k_pbpb_mc].c_str(), legendEntryOptions[k_pbpb_mc].c_str());

    setLegend(leg);
    leg->Draw();

    textX = 0.21;
    textYs = {0.69, 0.64};
    textAlign = 11;
    textFont = 43;
    textSize = 30;
    textLines = {
            "p^{e#pm}_{T} > 20 GeV/c",
            "|#eta^{e#pm}| < 2.5"
    };
    int nTextLines = textLines.size();
    TLatex* latex = 0;
    for (int i = 0; i < nTextLines; ++i) {
        latex = new TLatex();
        setLatex(i, latex);
        latex->Draw();
    }

    textX = 0.69;
    textYs = {0.81, 0.76, 0.71};
    textAlign = 11;
    textFont = 43;
    textSize = 30;
    textLines = {
            "Z #rightarrow ee",
            "Cent. 0-30 %",
            "p_{T}^{Z} > 40 GeV/c",
    };
    nTextLines = textLines.size();
    for (int i = 0; i < nTextLines; ++i) {
        latex = new TLatex();
        setLatex(i, latex);
        latex->Draw();
    }

    textXsOverPad = {0.17, 0.95};
    textYOverPad = 0.97;
    textAlignsOverPad = {11, 31};
    textFontOverPad = 43;
    textSizeOverPad = 29;
    textOverPadLines = {
            "PbPb #sqrt{s_{NN}} = 5.02 TeV",
            "404 #mub^{-1}"
    };
    int nTextOverPadLines = textOverPadLines.size();
    for (int i = 0; i < nTextOverPadLines; ++i) {
        latex = new TLatex();
        setLatexOverPad(i, latex);
        latex->Draw();
    }

    textXCMS = 0.69;
    textYCMS = 0.87;
    textAlignCMS = 11;
    textFontCMS = 61;
    textSizeCMS = 0.045;
    latex = new TLatex();
    setLatexCMS(latex);
    latex->Draw();

    c->Update();

    c->SaveAs(Form("%s.pdf", c->GetName()));
    c->Close();         // do not use Delete() for TCanvas.

    std::cout<<"Closing the input file"<<std::endl;
    input->Close();

    std::cout<<"running zJetPlot_M_Zee_pbpb() - END"<<std::endl;
}

void zJetPlot_M_Zmm_pbpb(std::string inputFile)
{
    std::cout<<"running zJetPlot_M_Zmm_pbpb()"<<std::endl;

    TFile* input  = TFile::Open(inputFile.c_str());

    windowWidth = 800;
    windowHeight = 800;
    logX = 0;
    logY = 0;
    leftMargin   = 0.17;
    rightMargin  = 0.05;
    bottomMargin = 0.15;
    topMargin    = 0.05;
    TCanvas* c = new TCanvas(figureNames[k_M_Zmm_pbpb].c_str(), "", windowWidth, windowHeight);
    std::cout<<"preparing canvas : "<< c->GetName() <<std::endl;
    setCanvas(c);
    c->cd();

    xTitle = "M^{#mu#mu} (GeV/c^{2})";
    yTitle = "Entries / (2 GeV/c^{2})";
    xTitleSize = 0.042;
    yTitleSize = 0.042;
    xTitleOffset = 1.25;
    yTitleOffset = 1.7;
    xTitleFont = 42;
    yTitleFont = 42;

    yMin = 0;
    yMax = 164.22;

    enum HISTLABELS {
        k_pbpb_mc,
        k_pbpb_data,
        k_pbpb_data_sameCh,
        kN_HISTLABELS
    };

    histPaths = {
            "h1D_M_Zmm_pbpbmc_cent030",
            "h1D_M_Zmm_pbpb_cent030",
            "h1D_M_Zmm_pbpb_cent030_sameCh"
    };
    markerColors = {kBlack, kBlack, kBlack};
    markerStyles = {kFullCircle, kFullCircle, kOpenSquare};
    markerSizes = {1.25, 1.25, 1.25};
    lineColors = {kOrange+7, TColor::GetColor("#000099"), TColor::GetColor("#000099")};
    lineTransparencies = {1.0, 1.0, 1.0};
    lineWidths = {2, 1, 1};
    fillColors = {kOrange-2, -1, -1};
    fillTransparencies = {1.0, 1.0, 1.0};
    drawOptions = {"hist", "e same", "e same"};

    int nHistPaths = histPaths.size();
    std::vector<TH1D*> h1Ds(nHistPaths, 0);
    TH1D* hTmp = 0;
    for (int i = 0; i < nHistPaths; ++i) {

        h1Ds[i] = (TH1D*)input->Get(histPaths[i].c_str());
        setTH1D(i, h1Ds[i]);

        h1Ds[i]->Draw(drawOptions[i].c_str());
    }

    std::vector<int> pairCounts(nHistPaths, 0);
    for (int i = 0; i < nHistPaths; ++i) {

        double massMin_count = 70;
        double massMax_count = 110;
        int binLow_count = -1;
        int binUp_count = -1;

        binLow_count = h1Ds[i]->FindBin(massMin_count);
        binUp_count = h1Ds[i]->FindBin(massMax_count) - 1;

        pairCounts[i] = (int)h1Ds[i]->Integral(binLow_count, binUp_count);
    }

    legendX1 = 0.21;
    legendY1 = 0.76;
    legendWidth = 0.45;
    legendHeight = 0.15;
    legendMargin = 0.15;
    legendEntryTexts = {
            "PYTHIA+HYDJET",
            Form("Opposite sign (%d counts)", pairCounts[k_pbpb_data]),
            Form("Same sign (%d counts)", pairCounts[k_pbpb_data_sameCh])
    };
    legendEntryOptions = {
            "lf",
            "lpf",
            "lpf"
    };
    TLegend* leg = new TLegend();

    hTmp = (TH1D*)h1Ds[k_pbpb_data]->Clone(Form("%s_tmp", h1Ds[k_pbpb_data]->GetName()));
    leg->AddEntry(hTmp, legendEntryTexts[k_pbpb_data].c_str(), legendEntryOptions[k_pbpb_data].c_str());
    hTmp = (TH1D*)h1Ds[k_pbpb_data_sameCh]->Clone(Form("%s_tmp", h1Ds[k_pbpb_data_sameCh]->GetName()));
    leg->AddEntry(hTmp, legendEntryTexts[k_pbpb_data_sameCh].c_str(), legendEntryOptions[k_pbpb_data_sameCh].c_str());
    hTmp = (TH1D*)h1Ds[k_pbpb_mc]->Clone(Form("%s_tmp", h1Ds[k_pbpb_mc]->GetName()));
    leg->AddEntry(hTmp, legendEntryTexts[k_pbpb_mc].c_str(), legendEntryOptions[k_pbpb_mc].c_str());

    setLegend(leg);
    leg->Draw();

    textX = 0.21;
    textYs = {0.69, 0.64};
    textAlign = 11;
    textFont = 43;
    textSize = 30;
    textLines = {
            "p^{#mu#pm}_{T} > 10 GeV/c",
            "|#eta^{#mu#pm}| < 2.4"
    };
    int nTextLines = textLines.size();
    TLatex* latex = 0;
    for (int i = 0; i < nTextLines; ++i) {
        latex = new TLatex();
        setLatex(i, latex);
        latex->Draw();
    }

    textX = 0.69;
    textYs = {0.81, 0.76, 0.71};
    textAlign = 11;
    textFont = 43;
    textSize = 30;
    textLines = {
            "Z #rightarrow #mu#mu",
            "Cent. 0-30 %",
            "p_{T}^{Z} > 40 GeV/c",
    };
    nTextLines = textLines.size();
    for (int i = 0; i < nTextLines; ++i) {
        latex = new TLatex();
        setLatex(i, latex);
        latex->Draw();
    }

    textXsOverPad = {0.17, 0.95};
    textYOverPad = 0.97;
    textAlignsOverPad = {11, 31};
    textFontOverPad = 43;
    textSizeOverPad = 29;
    textOverPadLines = {
            "PbPb #sqrt{s_{NN}} = 5.02 TeV",
            "393 #mub^{-1}"
    };
    int nTextOverPadLines = textOverPadLines.size();
    for (int i = 0; i < nTextOverPadLines; ++i) {
        latex = new TLatex();
        setLatexOverPad(i, latex);
        latex->Draw();
    }

    textXCMS = 0.69;
    textYCMS = 0.87;
    textAlignCMS = 11;
    textFontCMS = 61;
    textSizeCMS = 0.045;
    latex = new TLatex();
    setLatexCMS(latex);
    latex->Draw();

    c->Update();

    c->SaveAs(Form("%s.pdf", c->GetName()));
    c->Close();         // do not use Delete() for TCanvas.

    std::cout<<"Closing the input file"<<std::endl;
    input->Close();

    std::cout<<"running zJetPlot_M_Zmm_pbpb() - END"<<std::endl;
}

void zJetPlot_bkgSubtraction_dphijz_pbpb(std::string inputFile)
{
    std::cout<<"running zJetPlot_bkgSubtraction_dphijz_pbpb()"<<std::endl;

    TFile* input  = TFile::Open(inputFile.c_str());

    windowWidth = 800;
    windowHeight = 800;
    logX = 0;
    logY = 0;
    leftMargin   = 0.17;
    rightMargin  = 0.05;
    bottomMargin = 0.15;
    topMargin    = 0.05;
    TCanvas* c = new TCanvas(figureNames[k_bkgSubtraction_dphijz_pbpb].c_str(), "", windowWidth, windowHeight);
    std::cout<<"preparing canvas : "<< c->GetName() <<std::endl;
    setCanvas(c);
    c->cd();

    xTitle = "#Delta#phi_{jZ}";
    yTitle = "#frac{1}{N_{Z}} #frac{dN_{jZ}}{d#Delta#phi_{jZ}}";
    xTitleSize = 0.035;
    yTitleSize = 0.035;
    xTitleOffset = 1.25;
    yTitleOffset = 2;
    xTitleFont = 42;
    yTitleFont = 42;

    yMin = -0.5;
    yMax = 3;

    enum HISTLABELS {
        k_rawJet,
        k_bkgJet,
        k_sigJet,
        kN_HISTLABELS
    };

    histPaths = {
            "h1D_dphijz_Zmm_pbpb_cent030_rawJet",
            "h1D_dphijz_Zmm_pbpb_cent030_bkgJet",
            "h1D_dphijz_Zmm_pbpb_cent030_sigJet"
    };
    markerColors = {kBlack, kBlue, kRed};
    markerStyles = {kOpenSquare, kFullTriangleDown, kFullCircle};
    markerSizes = {1.5, 1.5, 1.5};
    lineColors = {kBlack, kBlue, kRed};
    lineTransparencies = {1.0, 1.0, 1.0};
    lineWidths = {1, 1, 1};
    fillColors = {-1, -1, -1};
    fillTransparencies = {1.0, 1.0, 1.0};
    drawOptions = {"e", "e same", "e same"};

    int nHistPaths = histPaths.size();
    std::vector<TH1D*> h1Ds(nHistPaths, 0);
    TH1D* hTmp = 0;
    for (int i = 0; i < nHistPaths; ++i) {

        h1Ds[i] = (TH1D*)input->Get(histPaths[i].c_str());
        setTH1D(i, h1Ds[i]);

        h1Ds[i]->GetXaxis()->CenterTitle(false);
        h1Ds[i]->GetYaxis()->CenterTitle(false);

        h1Ds[i]->Draw(drawOptions[i].c_str());
    }

    legendX1 = 0.21;
    legendY1 = 0.76;
    legendWidth = 0.33;
    legendHeight = 0.15;
    legendMargin = 0.15;
    legendEntryTexts = {"raw jet", "background jet", "signal jet"};
    legendEntryOptions = {"lpf", "lpf", "lpf"};
    TLegend* leg = new TLegend();
    leg->SetTextSize(0.04);

    for (int i = 0; i < nHistPaths; ++i) {
        hTmp = (TH1D*)h1Ds[i]->Clone(Form("%s_tmp", h1Ds[i]->GetName()));
        hTmp->SetMarkerSize(1.5);
        leg->AddEntry(hTmp, legendEntryTexts[i].c_str(), legendEntryOptions[i].c_str());
    }

    setLegend(leg);
    leg->Draw();

    textX = 0.21;
    textYs = {0.71, 0.66, 0.61, 0.5599999};
    textAlign = 11;
    textFont = 43;
    textSize = 28;
    textLines = {
            "Z#rightarrow #mu#mu",
            "p_{T}^{Z} > 60 GeV/c",
            "p_{T}^{jet} > 30 GeV/c",
            "|#eta^{jet}| < 1.6"
    };
    int nTextLines = textLines.size();
    TLatex* latex = 0;
    for (int i = 0; i < nTextLines; ++i) {
        latex = new TLatex();
        setLatex(i, latex);
        latex->Draw();
    }

    textXsOverPad = {0.18, 0.94};
    textYOverPad = 0.97;
    textAlignsOverPad = {11, 31};
    textFontOverPad = 43;
    textSizeOverPad = 24;
    textOverPadLines = {
            "CMS #sqrt{s_{NN}} = 5.02 TeV",
            "393 #mub^{-1} (PbPb)"
    };
    int nTextOverPadLines = textOverPadLines.size();
    for (int i = 0; i < nTextOverPadLines; ++i) {
        latex = new TLatex();
        setLatexOverPad(i, latex);
        latex->Draw();
    }

    c->Update();

    TLine* line = new TLine(gPad->GetUxmin(), 0, gPad->GetUxmax(), 0);
    line->SetLineStyle(kDashed);
    line->Draw();

    c->SaveAs(Form("%s.pdf", c->GetName()));
    c->Close();         // do not use Delete() for TCanvas.

    std::cout<<"Closing the input file"<<std::endl;
    input->Close();

    std::cout<<"running zJetPlot_bkgSubtraction_dphijz_pbpb() - END"<<std::endl;
}

void zJetPlot_xjz_xjg(std::string inputFile, bool isPP)
{
    std::cout<<"running zJetPlot_xjz_xjg()"<<std::endl;

    TFile* input  = TFile::Open(inputFile.c_str());

    // no horizontal error bars
    gStyle->SetErrorX(0);
    gStyle->SetHatchesLineWidth(3);

    windowWidth = 800;
    windowHeight = 800;
    logX = 0;
    logY = 0;
    leftMargin   = 0.21;
    rightMargin  = 0.03;
    bottomMargin = 0.15;
    topMargin    = 0.06;
    std::string figureName = (isPP) ? figureNames[k_xjz_xjg_pp] : figureNames[k_xjz_xjg_pbpb];
    TCanvas* c = new TCanvas(figureName.c_str(), "", windowWidth, windowHeight);
    std::cout<<"preparing canvas : "<< c->GetName() <<std::endl;
    setCanvas(c);
    c->cd();

    xTitle = "x_{jV} = p^{jet}_{T}/p^{V}_{T}";
    yTitle = "#frac{1}{N_{V}} #frac{dN_{jV}}{dx_{jV}}";
    xTitleSize = 0.0525;
    yTitleSize = 0.0525;
    xTitleOffset = 1.25;
    yTitleOffset = 1.5;
    xTitleFont = 42;
    yTitleFont = 42;

    yMin = -0.05;
    yMax = 1;

    enum HISTLABELS {
        k_xjg,
        k_xjz,
        kN_HISTLABELS
    };

    histPaths = {
            "h1D_xjg_pbpb_cent010",
            "h1D_xjz_pbpb_cent030",
    };
    if (isPP) {
        histPaths = {
                "h1D_xjg_pp_cent010",
                "h1D_xjz_pp_cent030",
        };
    }
    markerColors = {kBlack, kBlack};
    markerStyles = {kOpenSquare, kFullCircle};
    markerSizes = {2, 2};
    lineColors = {kBlack, 46};
    lineTransparencies = {1.0, 0.7};
    lineWidths = {2, 3};
    fillColors = {37, 46};
    fillTransparencies = {0.7, 0.7};
    drawOptions = {"e same", "e same"};
    sysPaths = {
            "h1D_sysVar_xjg_pbpb_cent010_rel",
            "h1D_sysVar_xjz_pbpb_cent030_rel",
    };
    if (isPP) {
        sysPaths = {
                "h1D_sysVar_xjg_pp_cent010_rel",
                "h1D_sysVar_xjz_pp_cent030_rel",
        };
    }
    sysUseRelUnc = {true, true};
    sysColors = {37, 46};
    sysTransparencies = {0.7, 0.7};
    sysFillStyles = {1001, 1001};

    int nHistPaths = histPaths.size();
    std::vector<TH1D*> h1Ds(nHistPaths, 0);
    std::vector<TH1D*> h1DsSys(nHistPaths, 0);
    TGraph* gr = 0;
    TH1D* hTmp = 0;
    for (int i = 0; i < nHistPaths; ++i) {

        h1Ds[i] = (TH1D*)input->Get(histPaths[i].c_str());
        setTH1D(i, h1Ds[i]);
        h1Ds[i]->SetStats(false);

        if (i == 0) {
            hTmp = (TH1D*)h1Ds[i]->Clone(Form("%s_tmpDraw", h1Ds[i]->GetName()));
            hTmp->Draw("e");
        }

        h1DsSys[i] = (TH1D*)input->Get(sysPaths[i].c_str());
        if (h1DsSys[i] != 0) {
            gr = new TGraph();
            setTGraphSys(i, gr);
            drawSysUncBoxes(gr, h1Ds[i], h1DsSys[i], sysUseRelUnc[i]);
        }

        h1Ds[i]->Draw(drawOptions[i].c_str());
    }
    h1Ds[HISTLABELS::k_xjz]->Draw("e same");    // redraw to enhance look of error bars
    h1Ds[HISTLABELS::k_xjg]->Draw("e same");    // redraw photon+jet to bring it to foreground

    legendX1 = 0.6;
    legendY1 = 0.6875;
    legendWidth = 0.36;
    legendHeight = 0.22;
    legendMargin = 0.15;
    legendEntryTexts = {
            "#gamma+jet, 0-10 %",
            "Z+jet, 0-30 %",
    };
    if (isPP) {
        legendEntryTexts = {
                "#gamma+jet, Smeared pp",
                "Z+jet, Smeared pp",
        };
    }
    std::vector<std::string> legendEntryTexts2nd = {
            "HIN-16-002",
            "HIN-15-013",
    };
    legendEntryOptions = {
            "pf",
            "pf"
    };
    TLegend* leg = new TLegend();

    hTmp = (TH1D*)h1Ds[HISTLABELS::k_xjz]->Clone(Form("%s_tmp", h1Ds[HISTLABELS::k_xjz]->GetName()));
    hTmp->SetLineWidth(0);
    leg->AddEntry(hTmp, legendEntryTexts[HISTLABELS::k_xjz].c_str(), legendEntryOptions[HISTLABELS::k_xjz].c_str());
    leg->AddEntry(hTmp, legendEntryTexts2nd[HISTLABELS::k_xjz].c_str(), "");

    hTmp = (TH1D*)h1Ds[HISTLABELS::k_xjg]->Clone(Form("%s_tmp", h1Ds[HISTLABELS::k_xjg]->GetName()));
    hTmp->SetLineWidth(0);
    leg->AddEntry(hTmp, legendEntryTexts[HISTLABELS::k_xjg].c_str(), legendEntryOptions[HISTLABELS::k_xjg].c_str());
    leg->AddEntry(hTmp, legendEntryTexts2nd[HISTLABELS::k_xjg].c_str(), "");

    setLegend(leg);
    leg->Draw();

    textX = 0.94;
    textYs = {0.62, 0.57, 0.5133, 0.4576, 0.4029, 0.3492};
    textAlign = 31;
    textFont = 43;
    textSize = 30;
    textLines = {
            "V = Z, #gamma",
            "p_{T}^{V} > 60 GeV/c",
            "anti-k_{T} jet R = 0.3",
            "p_{T}^{jet} > 30 GeV/c",
            "|#eta^{jet}| < 1.6",
            "#Delta#phi_{jV} > #frac{7}{8}#pi"
    };
    int nTextLines = textLines.size();
    TLatex* latex = 0;
    for (int i = 0; i < nTextLines; ++i) {
        latex = new TLatex();
        setLatex(i, latex);
        latex->Draw();
    }

    textXsOverPad = {0.22, 0.96};
    textYOverPad = 0.96;
    textAlignsOverPad = {11, 31};
    textFontOverPad = 43;
    textSizeOverPad = 30;
    textOverPadLines = {
            "#sqrt{s_{NN}} = 5.02 TeV",
            "PbPb 404 #mub^{-1}"
    };
    if (isPP) {
        textOverPadLines = {
                "#sqrt{s_{NN}} = 5.02 TeV",
                "pp 27.4 pb^{-1}"
        };
    }
    int nTextOverPadLines = textOverPadLines.size();
    for (int i = 0; i < nTextOverPadLines; ++i) {
        latex = new TLatex();
        setLatexOverPad(i, latex);
        latex->Draw();
    }

    textXCMS = 0.25;
    textYCMS = 0.86;
    textAlignCMS = 11;
    textFontCMS = 61;
    textSizeCMS = 0.06299999;
    latex = new TLatex();
    setLatexCMS(latex);
    latex->Draw();

    c->Update();

    TLine* line = new TLine(gPad->GetUxmin(), 0, gPad->GetUxmax(), 0);
    line->SetLineStyle(kDashed);
    line->Draw();

    c->SaveAs(Form("%s.pdf", c->GetName()));
    c->Close();         // do not use Delete() for TCanvas.

    std::cout<<"Closing the input file"<<std::endl;
    input->Close();

    std::cout<<"running zJetPlot_xjz_xjg() - END"<<std::endl;
}

void zJetPlot_xjz_xjg_pp(std::string inputFile)
{
    std::cout<<"running zJetPlot_xjz_xjg_pp()"<<std::endl;

    zJetPlot_xjz_xjg(inputFile, true);

    std::cout<<"running zJetPlot_xjz_xjg_pp() - END"<<std::endl;
}

void zJetPlot_xjz_xjg_pbpb(std::string inputFile)
{
    std::cout<<"running zJetPlot_xjz_xjg_pbpb()"<<std::endl;

    zJetPlot_xjz_xjg(inputFile, false);

    std::cout<<"running zJetPlot_xjz_xjg_pbpb() - END"<<std::endl;
}

void zJetPlot_xjzMean_xjgMean(std::string inputFile, bool isPP)
{
    std::cout<<"running zJetPlot_xjzMean_xjgMean()"<<std::endl;

    TFile* input  = TFile::Open(inputFile.c_str());

    // no horizontal error bars
    gStyle->SetErrorX(0);
    gStyle->SetHatchesLineWidth(3);

    windowWidth = 800;
    windowHeight = 800;
    logX = 0;
    logY = 0;
    leftMargin   = 0.21;
    rightMargin  = 0.03;
    bottomMargin = 0.15;
    topMargin    = 0.06;
    std::string figureName = (isPP) ? figureNames[k_xjzMean_xjgMean_pp] : figureNames[k_xjzMean_xjgMean_pbpb];
    TCanvas* c = new TCanvas(figureName.c_str(), "", windowWidth, windowHeight);
    std::cout<<"preparing canvas : "<< c->GetName() <<std::endl;
    setCanvas(c);
    c->cd();

    xTitle = "p^{V}_{T} (GeV/c)";
    yTitle = "<x_{jV}>";
    xTitleSize = 0.0525;
    yTitleSize = 0.0525;
    xTitleOffset = 1.25;
    yTitleOffset = 1.5;
    xTitleFont = 42;
    yTitleFont = 42;

    yMin = 0.6;
    yMax = 1.1;

    enum HISTLABELS {
        k_xjgMean,
        k_xjzMean,
        kN_HISTLABELS
    };

    histPaths = {
            "h1D_xjgMean_pbpb_cent030",
            "h1D_xjzMean_pbpb_cent030",
    };
    if (isPP) {
        histPaths = {
                "h1D_xjgMean_pp_cent030",
                "h1D_xjzMean_pp_cent030",
        };
    }
    markerColors = {kBlack, kBlack};
    markerStyles = {kOpenSquare, kFullCircle};
    markerSizes = {2, 2};
    lineColors = {kBlack, 46};
    lineTransparencies = {1.0, 0.7};
    lineWidths = {2, 3};
    fillColors = {37, 46};
    fillTransparencies = {0.7, 0.7};
    drawOptions = {"e same", "e same"};
    sysPaths = {
            "h1D_sysVar_xjgMean_pbpb_cent030_rel",
            "h1D_sysVar_xjzMean_pbpb_cent030_rel"
    };
    if (isPP) {
        sysPaths = {
                "h1D_sysVar_xjgMean_pp_cent030_rel",
                "h1D_sysVar_xjzMean_pp_cent030_rel",
        };
    }
    sysUseRelUnc = {true, true};
    sysColors = {37, 46};
    sysTransparencies = {0.7, 0.7};
    sysFillStyles = {1001, 1001};

    int nHistPaths = histPaths.size();
    std::vector<TH1D*> h1Ds(nHistPaths, 0);
    std::vector<TH1D*> h1DsSys(nHistPaths, 0);
    TGraph* gr = 0;
    TH1D* hTmp = 0;
    for (int i = 0; i < nHistPaths; ++i) {

        h1Ds[i] = (TH1D*)input->Get(histPaths[i].c_str());
        setTH1D(i, h1Ds[i]);
        h1Ds[i]->SetStats(false);

        if (i == 0) {
            hTmp = (TH1D*)h1Ds[i]->Clone(Form("%s_tmpDraw", h1Ds[i]->GetName()));
            hTmp->Draw("e");
        }

        h1DsSys[i] = (TH1D*)input->Get(sysPaths[i].c_str());
        if (h1DsSys[i] != 0) {
            gr = new TGraph();
            setTGraphSys(i, gr);
            drawSysUncBoxes(gr, h1Ds[i], h1DsSys[i], sysUseRelUnc[i]);
        }

        h1Ds[i]->Draw(drawOptions[i].c_str());
    }
    h1Ds[HISTLABELS::k_xjzMean]->Draw("e same");    // redraw to enhance look of error bars
    h1Ds[HISTLABELS::k_xjgMean]->Draw("e same");    // redraw photon+jet to bring it to foreground

    legendX1 = 0.6;
    legendY1 = 0.6875;
    legendWidth = 0.36;
    legendHeight = 0.22;
    legendMargin = 0.15;
    legendEntryTexts = {
            "#gamma+jet, 0-30 %",
            "Z+jet, 0-30 %",
    };
    if (isPP) {
        legendEntryTexts = {
                "#gamma+jet, Smeared pp",
                "Z+jet, Smeared pp",
        };
    }
    std::vector<std::string> legendEntryTexts2nd = {
            "HIN-16-002",
            "HIN-15-013",
    };
    legendEntryOptions = {
            "pf",
            "pf",
    };
    TLegend* leg = new TLegend();

    hTmp = (TH1D*)h1Ds[HISTLABELS::k_xjzMean]->Clone(Form("%s_tmp", h1Ds[HISTLABELS::k_xjzMean]->GetName()));
    hTmp->SetLineWidth(0);
    leg->AddEntry(hTmp, legendEntryTexts[HISTLABELS::k_xjzMean].c_str(), legendEntryOptions[HISTLABELS::k_xjzMean].c_str());
    leg->AddEntry(hTmp, legendEntryTexts2nd[HISTLABELS::k_xjzMean].c_str(), "");

    hTmp = (TH1D*)h1Ds[HISTLABELS::k_xjgMean]->Clone(Form("%s_tmp", h1Ds[HISTLABELS::k_xjgMean]->GetName()));
    hTmp->SetLineWidth(0);
    leg->AddEntry(hTmp, legendEntryTexts[HISTLABELS::k_xjgMean].c_str(), legendEntryOptions[HISTLABELS::k_xjgMean].c_str());
    leg->AddEntry(hTmp, legendEntryTexts2nd[HISTLABELS::k_xjgMean].c_str(), "");

    setLegend(leg);
    leg->Draw();

    textX = 0.93;
    textYs = {0.64, 0.59, 0.53, 0.47, 0.42};
    textAlign = 31;
    if (isPP) {
        textX = 0.25;
        textYs = {0.44, 0.39, 0.33, 0.27, 0.21};
        textAlign = 11;
    }
    textFont = 43;
    textSize = 30;
    textLines = {
            "V = Z, #gamma",
            "anti-k_{T} jet R = 0.3",
            "p_{T}^{jet} > 30 GeV/c",
            "|#eta^{jet}| < 1.6",
            "#Delta#phi_{jV} > #frac{7}{8}#pi"
    };
    int nTextLines = textLines.size();
    TLatex* latex = 0;
    for (int i = 0; i < nTextLines; ++i) {
        latex = new TLatex();
        setLatex(i, latex);
        latex->Draw();
    }

    textXsOverPad = {0.22, 0.96};
    textYOverPad = 0.96;
    textAlignsOverPad = {11, 31};
    textFontOverPad = 43;
    textSizeOverPad = 30;
    textOverPadLines = {
            "#sqrt{s_{NN}} = 5.02 TeV",
            "PbPb 404 #mub^{-1}"
    };
    if (isPP) {
        textOverPadLines = {
                "#sqrt{s_{NN}} = 5.02 TeV",
                "pp 27.4 pb^{-1}"
        };
    }
    int nTextOverPadLines = textOverPadLines.size();
    for (int i = 0; i < nTextOverPadLines; ++i) {
        latex = new TLatex();
        setLatexOverPad(i, latex);
        latex->Draw();
    }

    textXCMS = 0.33;
    textYCMS = 0.86;
    textAlignCMS = 11;
    textFontCMS = 61;
    textSizeCMS = 0.06299999;
    latex = new TLatex();
    setLatexCMS(latex);
    latex->Draw();

    c->Update();

    TLine* line = new TLine(gPad->GetUxmin(), 0, gPad->GetUxmax(), 0);
    line->SetLineStyle(kDashed);
    line->Draw();

    c->SaveAs(Form("%s.pdf", c->GetName()));
    c->Close();         // do not use Delete() for TCanvas.

    std::cout<<"Closing the input file"<<std::endl;
    input->Close();

    std::cout<<"running zJetPlot_xjzMean_xjgMean() - END"<<std::endl;
}

void zJetPlot_xjzMean_xjgMean_pp(std::string inputFile)
{
    std::cout<<"running zJetPlot_xjzMean_xjgMean_pp()"<<std::endl;

    zJetPlot_xjzMean_xjgMean(inputFile, true);

    std::cout<<"running zJetPlot_xjzMean_xjgMean_pp() - END"<<std::endl;
}

void zJetPlot_xjzMean_xjgMean_pbpb(std::string inputFile)
{
    std::cout<<"running zJetPlot_xjzMean_xjgMean_pbpb()"<<std::endl;

    zJetPlot_xjzMean_xjgMean(inputFile, false);

    std::cout<<"running zJetPlot_xjzMean_xjgMean_pbpb() - END"<<std::endl;
}

void setTH1D(int iHist, TH1D* h)
{
    h->SetTitle("");
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
        zJetPlotPublic(atoi(argv[1]), argv[2]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./zJetPlotPublic.exe <figureIndex> <inputFile>"
                << std::endl;
        return 1;
    }
}
