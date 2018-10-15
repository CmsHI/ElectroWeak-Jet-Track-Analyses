/*
 * Stand-alone macro to produce CMS projection plots for Z+jet and photon+jet FF observables.
 */
#include <TFile.h>
#include <TDirectoryFile.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TAxis.h>
#include <TGaxis.h>
#include <TH1.h>
#include <TH1D.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TF1.h>
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
    k_xjz_Theory,
    k_xjz_Theory_MergedUnc,
    k_xjz_pPb,
    k_xjz_pPb_multBins,
    k_xjz_pp_multBins,
    k_xjg_pPb_multBins,
    k_xjg_pp_multBins,
    k_xijet,
    k_xigamma,
    k_xijet_MergedUnc,
    k_xigamma_MergedUnc,
    k_xijet_ratio,
    k_xigamma_ratio,
    k_xijet_ratioOnly,
    k_xigamma_ratioOnly,
    k_js_ratioOnly,
    k_js_ratioOnly_fit_pol2,
    k_js_ratioOnly_fit_pol3,
    kN_FIGURES
};

std::string figureNames[kN_FIGURES] = {
        "projection_xjz",
        "projection_xjz_Theory",
        "projection_xjz_Theory_MergedUnc",
        "projection_xjz_pPb",
        "projection_xjz_pPb_multBins",
        "projection_xjz_pp_multBins",
        "projection_xjg_pPb_multBins",
        "projection_xjg_pp_multBins",
        "projection_xijet",
        "projection_xigamma",
        "projection_xijet_MergedUnc",
        "projection_xigamma_MergedUnc",
        "projection_xijet_ratio",
        "projection_xigamma_ratio",
        "projection_xijet_ratioOnly",
        "projection_xigamma_ratioOnly",
        "projection_js_ratioOnly",
        "projection_js_ratioOnly_fit_pol2",
        "projection_js_ratioOnly_fit_pol3",};

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
std::vector<std::string> histPathsStatUnc;      // paths to hist whose error carry the stats unc
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
// TLatex CMS Projection
float textXCMSProj;
float textYCMSProj;
int textAlignCMSProj;
int textFontCMSProj;
float textSizeCMSProj;

double projectedLumiPBPB = 10;   // nb
double currentLumiPBPB = 0.404;  // nb
double statsIncreasePBPB = projectedLumiPBPB / currentLumiPBPB;
double projectedLumiPP = 200;     // pb
double projectedLumiPPB = 2;     // pb
double currentLumiPP = 27.4;     // pb
double statsIncreasePPB = (projectedLumiPPB*208) / currentLumiPP;     // assuming pPb is 5 TeV
double statsIncreasePP = projectedLumiPP / currentLumiPP;

void vJetPlotProjection(int figureIndex, std::string inputFile, double sysReduction = 0);
void projectionPlot_xjz(std::string inputFile, double sysReduction = 0);
void projectionPlot_xjz_Theory(std::string inputFile, double sysReduction = 0);
void projectionPlot_xjz_Theory_MergedUnc(std::string inputFile, double sysReduction = 0);
void projectionPlot_xjz_pPb(std::string inputFile, double sysReduction = 0);
void projectionPlot_xjz_pPb_multBins(std::string inputFile, double sysReduction = 0);
void projectionPlot_xjz_pp_multBins(std::string inputFile, double sysReduction = 0);
void projectionPlot_xjg_pPb_multBins(std::string inputFile, double sysReduction = 0);
void projectionPlot_xjg_pp_multBins(std::string inputFile, double sysReduction = 0);
void projectionPlot_xjv_multBins(std::string inputFile, double sysReduction = 0, int iCollision = 0, int iszjet = 1);
void projectionPlot_xi(std::string inputFile, bool isxijet = true, double sysReduction = 0);
void projectionPlot_xi_MergedUnc(std::string inputFile, bool isxijet = true, double sysReduction = 0);
void projectionPlot_xi_ratio(std::string inputFile, bool isxijet = true, double sysReduction = 0);
void projectionPlot_xi_ratioOnly(std::string inputFile, bool isxijet = true, double sysReduction = 0);
void projectionPlot_js_ratioOnly(std::string inputFile, double sysReduction = 0, int iFitFnc = -1);
void setTH1D(int iHist, TH1D* h);
void setTGraph(int iGraph, TGraph* gr);
void setTGraphSys(int iSys, TGraph* gr);
void setCanvas(TCanvas* c);
void setLegend(TLegend* leg);
void setLatex(int iText, TLatex* latex);
void setLatexOverPad(int iText, TLatex* latex);
void setLatexCMSProj(TLatex* latex, std::string text = "CMS");
double calcNormCanvasWidth(int columns = 1, float leftMargin = 0.1, float rightMargin = 0.1, float xMargin = 0.01);
double calcNormCanvasHeight(int rows = 1, float bottomMargin = 0.1, float topMargin = 0.1, float yMargin = 0.01);
void divideCanvas(TCanvas* c, int rows = 1, int columns = 1, float leftMargin = 0.1, float rightMargin = 0.1, float bottomMargin = 0.1, float topMargin = 0.1, float xMargin = 0.01, float yMargin = 0.01, float yMinOffSet = 0);
void divideCanvas(TCanvas* c, TPad* pads[], int rows = 1, int columns = 1, float leftMargin = 0.1, float rightMargin = 0.1, float bottomMargin = 0.1, float topMargin = 0.1, float xMargin = 0.01, float yMargin = 0.01, float yMinOffSet = 0);
void setSysUncBox(TGraph* gr, TH1* h, TH1* hSys, int bin, bool doRelUnc = false, double binWidth = -1, double binWidthScale = 1);
void drawSysUncBoxes(TGraph* gr, TH1* h, TH1* hSys, bool doRelUnc = false, double binWidth = -1, double binWidthScale = 1);
void scaleBinErrors(TH1* h, double scale);
void fillTH1fromTF1(TH1* h, TF1* f, int binFirst, int binLast);
void mergeUncWithErrorBar(TH1* h, TH1* hSys, bool doRelUnc = false);
void setBinErrors(TH1* h, std::vector<double> binErrors);
std::vector<double> getBinErrors(TH1* h);

void vJetPlotProjection(int figureIndex, std::string inputFile, double sysReduction)
{
    std::cout<<"running vJetPlotProjection()"<<std::endl;
    std::cout<<"figureIndex = "<< figureIndex <<std::endl;
    std::cout<<"inputFile = "<< inputFile.c_str() <<std::endl;

    switch (figureIndex) {
        case k_xjz:
            projectionPlot_xjz(inputFile, sysReduction);
            break;
        case k_xjz_Theory:
            projectionPlot_xjz_Theory(inputFile, sysReduction);
            break;
        case k_xjz_Theory_MergedUnc:
            projectionPlot_xjz_Theory_MergedUnc(inputFile, sysReduction);
            break;
        case k_xjz_pPb:
            projectionPlot_xjz_pPb(inputFile, sysReduction);
            break;
        case k_xjz_pPb_multBins:
            projectionPlot_xjz_pPb_multBins(inputFile, sysReduction);
            break;
        case k_xjz_pp_multBins:
            projectionPlot_xjz_pp_multBins(inputFile, sysReduction);
            break;
        case k_xjg_pPb_multBins:
            projectionPlot_xjg_pPb_multBins(inputFile, sysReduction);
            break;
        case k_xjg_pp_multBins:
            projectionPlot_xjg_pp_multBins(inputFile, sysReduction);
            break;
        case k_xijet:
            projectionPlot_xi(inputFile, true, sysReduction);
            break;
        case k_xigamma:
            projectionPlot_xi(inputFile, false, sysReduction);
            break;
        case k_xijet_MergedUnc:
            projectionPlot_xi_MergedUnc(inputFile, true, sysReduction);
            break;
        case k_xigamma_MergedUnc:
            projectionPlot_xi_MergedUnc(inputFile, false, sysReduction);
            break;
        case k_xijet_ratio:
            projectionPlot_xi_ratio(inputFile, true, sysReduction);
            break;
        case k_xigamma_ratio:
            projectionPlot_xi_ratio(inputFile, false, sysReduction);
            break;
        case k_xijet_ratioOnly:
            projectionPlot_xi_ratioOnly(inputFile, true, sysReduction);
            break;
        case k_xigamma_ratioOnly:
            projectionPlot_xi_ratioOnly(inputFile, false, sysReduction);
            break;
        case k_js_ratioOnly:
            projectionPlot_js_ratioOnly(inputFile, sysReduction);
            break;
        case k_js_ratioOnly_fit_pol2:
            projectionPlot_js_ratioOnly(inputFile, sysReduction, 0);
            break;
        case k_js_ratioOnly_fit_pol3:
            projectionPlot_js_ratioOnly(inputFile, sysReduction, 1);
            break;
        default:
            break;
    }

    std::cout<<"vJetPlotProjection() - END"<<std::endl;
}

void projectionPlot_xjz(std::string inputFile, double sysReduction)
{
    std::cout<<"running projectionPlot_xjz()"<<std::endl;

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
    TCanvas* c = 0;
    if (sysReduction == 0)
        c = new TCanvas(figureNames[k_xjz].c_str(), "", windowWidth, windowHeight);
    else if (sysReduction == -1)
        c = new TCanvas(Form("%s_noSys", figureNames[k_xjz].c_str()), "", windowWidth, windowHeight);
    else
        c = new TCanvas(Form("%s_sysReduced%dPrct", figureNames[k_xjz].c_str(), (int)(sysReduction*100)), "", windowWidth, windowHeight);
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

    yMin = 0;
    yMax = 1;

    enum HISTLABELS {
        k_pbpb,
        kN_HISTLABELS
    };

    histPaths = {
            "h1D_xjz_pbpb_cent0100"
    };
    markerColors = {kBlack};
    markerStyles = {kFullCircle};
    markerSizes = {1.70};
    lineColors = {kBlack};
    lineTransparencies = {1.0};
    lineWidths = {3};
    fillColors = {46};
    if (sysReduction == -1) fillColors = {0};
    fillTransparencies = {1.0};
    drawOptions = {"e same"};
    sysPaths = {
            "h1D_sysVar_xjz_pbpb_cent0100_rel",
    };
    if (sysReduction == -1) {
        sysPaths = {
                "NULL",
        };
    }
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
        scaleBinErrors(h1Ds[i], 1./TMath::Sqrt(statsIncreasePBPB));
    }
    // modify bin contents for projection
    double sf = 0.96;
    int iBin = 3;
    h1Ds[k_pbpb]->SetBinContent(iBin, h1Ds[k_pbpb]->GetBinContent(iBin) * sf);
    h1Ds[k_pbpb]->SetBinError(iBin, h1Ds[k_pbpb]->GetBinError(iBin) * TMath::Sqrt(sf));
    sf = 0.9;
    iBin = 5;
    h1Ds[k_pbpb]->SetBinContent(iBin, h1Ds[k_pbpb]->GetBinContent(iBin) * sf);
    h1Ds[k_pbpb]->SetBinError(iBin, h1Ds[k_pbpb]->GetBinError(iBin) * TMath::Sqrt(sf));
    sf = 0.6;
    iBin = 8;
    h1Ds[k_pbpb]->SetBinContent(iBin, h1Ds[k_pbpb]->GetBinContent(iBin) * sf);
    h1Ds[k_pbpb]->SetBinError(iBin, h1Ds[k_pbpb]->GetBinError(iBin) * TMath::Sqrt(sf));
    // add content to last bin
    sf = 0.6;
    iBin = 10;
    h1Ds[k_pbpb]->SetBinContent(iBin, h1Ds[k_pbpb]->GetBinContent(iBin-1) * sf);
    h1Ds[k_pbpb]->SetBinError(iBin, h1Ds[k_pbpb]->GetBinError(iBin-1) * TMath::Sqrt(sf));

    // draw histograms
    for (int i = 0; i < nHistPaths; ++i) {

        if (i == 0) {
            hTmp = (TH1D*)h1Ds[i]->Clone(Form("%s_tmpDraw", h1Ds[i]->GetName()));
            hTmp->Draw("e");
        }

        h1DsSys[i] = (TH1D*)input->Get(sysPaths[i].c_str());
        if (h1DsSys[i] != 0) {
            if (sysReduction >= 0)
                h1DsSys[i]->Scale(1-sysReduction);
            gr = new TGraph();
            setTGraphSys(i, gr);
            drawSysUncBoxes(gr, h1Ds[i], h1DsSys[i], sysUseRelUnc[i]);
        }

        h1Ds[i]->Draw(drawOptions[i].c_str());
    }

    /*
    legendX1 = 0.6;
    legendY1 = 0.7875;
    legendWidth = 0.44;
    legendHeight = 0.1125;
    legendMargin = 0.15;
    legendEntryTexts = {
            "PbPb, 0-30 %",
    };
    legendEntryOptions = {
            "pf",
    };
    TLegend* leg = new TLegend();

    hTmp = (TH1D*)h1Ds[k_pbpb]->Clone(Form("%s_tmp", h1Ds[k_pbpb]->GetName()));
    hTmp->SetLineWidth(0);
    leg->AddEntry(hTmp, legendEntryTexts[k_pbpb].c_str(), legendEntryOptions[k_pbpb].c_str());

    setLegend(leg);
    leg->Draw();
    */

    textAlign = 31;
    textFont = 43;
    textSize = 34;
    textLines = {
            "Cent. 0-100 %",
            "p_{T}^{Z} > 60 GeV/c",
            "anti-k_{T} jet R = 0.3",
            "p_{T}^{jet} > 30 GeV/c",
            "|#eta^{jet}| < 1.6",
            "#Delta#phi_{jZ} > #frac{7}{8}#pi"
    };
    int nTextLines = textLines.size();
    textX = 0.93;
    textYs.resize(nTextLines, 0.80);
    TLatex* latex = 0;
    for (int i = 0; i < nTextLines; ++i) {
        latex = new TLatex();
        textYs[i] = textYs[0] - i*0.056;
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
            "PbPb 10 nb^{-1}"
    };
    int nTextOverPadLines = textOverPadLines.size();
    for (int i = 0; i < nTextOverPadLines; ++i) {
        latex = new TLatex();
        setLatexOverPad(i, latex);
        latex->Draw();
    }

    textXCMSProj = 0.25;
    textYCMSProj = 0.86;
    textAlignCMSProj = 11;
    textFontCMSProj = 61;
    textSizeCMSProj = 0.06;
    latex = new TLatex();
    setLatexCMSProj(latex, "CMS");
    latex->Draw();

    textXCMSProj = 0.38;
    textYCMSProj = 0.86;
    textAlignCMSProj = 11;
    textFontCMSProj = 52;
    textSizeCMSProj = 0.05;
    latex = new TLatex();
    setLatexCMSProj(latex, "Projection");
    latex->Draw();

    c->Update();

    c->SaveAs(Form("%s.pdf", c->GetName()));
    c->Close();         // do not use Delete() for TCanvas.

    std::cout<<"Closing the input file"<<std::endl;
    input->Close();

    std::cout<<"running projectionPlot_xjz() - END"<<std::endl;
}

void projectionPlot_xjz_Theory(std::string inputFile, double sysReduction)
{
    std::cout<<"running projectionPlot_xjz_Theory()"<<std::endl;

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
    TCanvas* c = 0;
    if (sysReduction == 0)
        c = new TCanvas(figureNames[k_xjz_Theory].c_str(), "", windowWidth, windowHeight);
    else if (sysReduction == -1)
        c = new TCanvas(Form("%s_noSys", figureNames[k_xjz_Theory].c_str()), "", windowWidth, windowHeight);
    else
        c = new TCanvas(Form("%s_sysReduced%dPrct", figureNames[k_xjz_Theory].c_str(), (int)(sysReduction*100)), "", windowWidth, windowHeight);
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

    yMin = 0;
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
    markerSizes = {1.70};
    lineColors = {kBlack};
    lineTransparencies = {1.0};
    lineWidths = {3};
    fillColors = {46};
    if (sysReduction == -1) fillColors = {0};
    fillTransparencies = {0.7};
    drawOptions = {"e same"};
    sysPaths = {
            "h1D_sysVar_xjz_pbpb_cent030_rel",
    };
    if (sysReduction == -1) {
        sysPaths = {
                "NULL",
        };
    }
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
    }
    // modify bin contents for projection, use a fit function for that.
    TF1* f1 = 0;
    double f1_xMin = 0.1;
    double f1_xMax = 1.9;
    // pol2 x Gaussian
    //f1_xMin = 0.3;
    //f1_xMax = 1.9;
    //f1 = new TF1("f1","([0]+[1]*x+[2]*x*x)*exp(-[3]*(x-[4])*(x-[4]))", f1_xMin, f1_xMax);
    // pol3 x Gaussian
    f1 = new TF1("f1","([0]+[1]*x+[2]*x*x+[3]*x*x*x)*exp(-[4]*(x-[5])*(x-[5]))", f1_xMin, f1_xMax);
    h1Ds[k_pbpb]->Fit(f1, "M N R");
    for (int iBin = h1Ds[k_pbpb]->FindBin(f1_xMin); iBin <= h1Ds[k_pbpb]->FindBin(f1_xMax); ++iBin) {
        double binContent = h1Ds[k_pbpb]->GetBinContent(iBin);
        double binError = h1Ds[k_pbpb]->GetBinError(iBin);
        double val = f1->Eval(h1Ds[k_pbpb]->GetBinCenter(iBin));

        h1Ds[k_pbpb]->SetBinContent(iBin, val);
        h1Ds[k_pbpb]->SetBinError(iBin, binError * TMath::Sqrt(val/binContent));
        if (binError == 0)
            h1Ds[k_pbpb]->SetBinError(iBin, h1Ds[k_pbpb]->GetBinError(iBin-1) * TMath::Sqrt(val/h1Ds[k_pbpb]->GetBinContent(iBin-1)));
    }

    // draw histograms
    for (int i = 0; i < nHistPaths; ++i) {

        scaleBinErrors(h1Ds[i], 1./TMath::Sqrt(statsIncreasePBPB));
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
        if (sysReduction >= 0)
            h1DsSys[k_pbpb]->Scale(1-sysReduction);
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
    legendY1 = 0.78;
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
            "PbPb 10 nb^{-1}"
    };
    int nTextOverPadLines = textOverPadLines.size();
    for (int i = 0; i < nTextOverPadLines; ++i) {
        latex = new TLatex();
        setLatexOverPad(i, latex);
        latex->Draw();
    }

    textXCMSProj = 0.25;
    textYCMSProj = 0.87;
    textAlignCMSProj = 11;
    textFontCMSProj = 61;
    textSizeCMSProj = 0.06;
    latex = new TLatex();
    setLatexCMSProj(latex, "CMS");
    latex->Draw();

    textXCMSProj = 0.38;
    textYCMSProj = 0.87;
    textAlignCMSProj = 11;
    textFontCMSProj = 52;
    textSizeCMSProj = 0.05;
    latex = new TLatex();
    setLatexCMSProj(latex, "Projection");
    latex->Draw();

    c->Update();

    c->SaveAs(Form("%s.pdf", c->GetName()));
    c->Close();         // do not use Delete() for TCanvas.

    std::cout<<"Closing the input file"<<std::endl;
    input->Close();

    std::cout<<"running projectionPlot_xjz_Theory() - END"<<std::endl;
}

void projectionPlot_xjz_Theory_MergedUnc(std::string inputFile, double sysReduction)
{
    std::cout<<"running projectionPlot_xjz_Theory_MergedUnc()"<<std::endl;

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
    TCanvas* c = 0;
    if (sysReduction == 0)
        c = new TCanvas(figureNames[k_xjz_Theory_MergedUnc].c_str(), "", windowWidth, windowHeight);
    else if (sysReduction == -1)
        c = new TCanvas(Form("%s_noSys", figureNames[k_xjz_Theory_MergedUnc].c_str()), "", windowWidth, windowHeight);
    else
        c = new TCanvas(Form("%s_sysReduced%dPrct", figureNames[k_xjz_Theory_MergedUnc].c_str(), (int)(sysReduction*100)), "", windowWidth, windowHeight);
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

    yMin = 0;
    yMax = 1.30;

    enum HISTLABELS {
        k_pbpb,
        kN_HISTLABELS
    };

    histPaths = {
            "h1D_xjz_pbpb_cent030"
    };
    markerColors = {kBlack};
    markerStyles = {kFullCircle};
    markerSizes = {1.70};
    lineColors = {kBlack};
    lineTransparencies = {1.0};
    lineWidths = {0};
    fillColors = {0};
    if (sysReduction == -1) fillColors = {0};
    fillTransparencies = {0.75};
    drawOptions = {"e same"};
    sysPaths = {
            "h1D_sysVar_xjz_pbpb_cent030_rel"
    };
    if (sysReduction == -1) {
        sysPaths = {
                "NULL"
        };
    }
    sysUseRelUnc = {true, true};
    sysColors = {TColor::GetColor("#a09f93"), TColor::GetColor("#ad33ff")};
    sysTransparencies = {0.8, 0.8};
    sysFillStyles = {1001, 3254};

    int nHistPaths = histPaths.size();
    std::vector<TH1D*> h1Ds(nHistPaths, 0);
    std::vector<TH1D*> h1DsSys(nHistPaths, 0);
    TGraph* gr = 0;
    TH1D* hTmp = 0;
    for (int i = 0; i < nHistPaths; ++i) {

        h1Ds[i] = (TH1D*)input->Get(histPaths[i].c_str());
        setTH1D(i, h1Ds[i]);
    }
    // modify bin contents for projection, use a fit function for that.
    TF1* f1 = 0;
    double f1_xMin = 0.1;
    double f1_xMax = 1.9;
    // pol2 x Gaussian
    //f1_xMin = 0.3;
    //f1_xMax = 1.9;
    //f1 = new TF1("f1","([0]+[1]*x+[2]*x*x)*exp(-[3]*(x-[4])*(x-[4]))", f1_xMin, f1_xMax);
    // pol3 x Gaussian
    f1 = new TF1("f1","([0]+[1]*x+[2]*x*x+[3]*x*x*x)*exp(-[4]*(x-[5])*(x-[5]))", f1_xMin, f1_xMax);
    h1Ds[k_pbpb]->Fit(f1, "M N R");
    for (int iBin = h1Ds[k_pbpb]->FindBin(f1_xMin); iBin <= h1Ds[k_pbpb]->FindBin(f1_xMax); ++iBin) {
        double binContent = h1Ds[k_pbpb]->GetBinContent(iBin);
        double binError = h1Ds[k_pbpb]->GetBinError(iBin);
        double val = f1->Eval(h1Ds[k_pbpb]->GetBinCenter(iBin));

        h1Ds[k_pbpb]->SetBinContent(iBin, val);
        h1Ds[k_pbpb]->SetBinError(iBin, binError * TMath::Sqrt(val/binContent));
        if (binError == 0)
            h1Ds[k_pbpb]->SetBinError(iBin, h1Ds[k_pbpb]->GetBinError(iBin-1) * TMath::Sqrt(val/h1Ds[k_pbpb]->GetBinContent(iBin-1)));
    }

    // draw histograms
    for (int i = 0; i < nHistPaths; ++i) {

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

    std::vector<TH1D*> h1DsMergedUncCurrent(nHistPaths, 0);
    std::vector<TH1D*> h1DsMergedUncProjection(nHistPaths, 0);

    h1DsMergedUncCurrent[k_pbpb] =(TH1D*)h1Ds[k_pbpb]->Clone(Form("%s_MergedUncCurrent", h1Ds[k_pbpb]->GetName()));
    h1DsMergedUncProjection[k_pbpb] =(TH1D*)h1Ds[k_pbpb]->Clone(Form("%s_MergedUncProjection", h1Ds[k_pbpb]->GetName()));

    if (h1DsSys[k_pbpb] != 0) {
        // prepare and draw current uncertainty
        mergeUncWithErrorBar(h1DsMergedUncCurrent[k_pbpb], h1DsSys[k_pbpb], sysUseRelUnc[k_pbpb]);

        for (int iBin = 1; iBin <= h1DsMergedUncCurrent[k_pbpb]->GetNbinsX(); ++iBin) {
            double binContent = h1DsMergedUncCurrent[k_pbpb]->GetBinContent(iBin);
            double binError = h1DsMergedUncCurrent[k_pbpb]->GetBinError(iBin);
            if (sysUseRelUnc[k_pbpb])
                h1DsMergedUncCurrent[k_pbpb]->SetBinContent(iBin, binError/binContent);
            else
                h1DsMergedUncCurrent[k_pbpb]->SetBinContent(iBin, binError);
        }

        gr = new TGraph();
        setTGraphSys(k_pbpb, gr);
        gr->SetLineWidth(3);
        drawSysUncBoxes(gr, h1Ds[k_pbpb], h1DsMergedUncCurrent[k_pbpb], sysUseRelUnc[k_pbpb]);

        // prepare and draw projected uncertainty
        scaleBinErrors(h1DsMergedUncProjection[k_pbpb], 1./TMath::Sqrt(statsIncreasePBPB));
        if (sysReduction >= 0) {
            h1DsSys[k_pbpb]->Scale(1-sysReduction);
        }
        mergeUncWithErrorBar(h1DsMergedUncProjection[k_pbpb], h1DsSys[k_pbpb], sysUseRelUnc[k_pbpb]);

        for (int iBin = 1; iBin <= h1DsMergedUncProjection[k_pbpb]->GetNbinsX(); ++iBin) {
            double binContent = h1DsMergedUncProjection[k_pbpb]->GetBinContent(iBin);
            double binError = h1DsMergedUncProjection[k_pbpb]->GetBinError(iBin);
            if (sysUseRelUnc[k_pbpb])
                h1DsMergedUncProjection[k_pbpb]->SetBinContent(iBin, binError/binContent);
            else
                h1DsMergedUncProjection[k_pbpb]->SetBinContent(iBin, binError);
        }

        gr = new TGraph();
        setTGraphSys(k_pbpb+1, gr);
        gr->SetLineWidth(3);
        drawSysUncBoxes(gr, h1Ds[k_pbpb], h1DsMergedUncProjection[k_pbpb], sysUseRelUnc[k_pbpb]);
    }
    h1Ds[k_pbpb]->Draw(drawOptions[k_pbpb].c_str());

    graphs[k_hybrid_coll]->Draw(graphDrawOptions[k_hybrid_coll].c_str());
    graphs[k_hybrid_rad]->Draw(graphDrawOptions[k_hybrid_rad].c_str());
    graphs[k_hybrid_strong]->Draw(graphDrawOptions[k_hybrid_strong].c_str());
    graphs[k_jewel]->Draw(graphDrawOptions[k_jewel].c_str());
    graphs[k_GLV_g2p0]->Draw(graphDrawOptions[k_GLV_g2p0].c_str());
    graphs[k_GLV_g2p2]->Draw(graphDrawOptions[k_GLV_g2p2].c_str());

    h1Ds[k_pbpb]->Draw(drawOptions[k_pbpb].c_str());

    TLegend* leg = 0;
    legendX1 = 0.25;
    legendY1 = 0.724;
    legendWidth = 0.44;
    legendHeight = 0.14;
    legendMargin = 0.15;
    legendEntryTexts = {"PbPb, 0-30 %", "Current Unc.", "Projected Unc."};
    legendEntryOptions = {"p", "f", "f"};
    leg = new TLegend();

    hTmp = (TH1D*)h1Ds[k_pbpb]->Clone(Form("%s_tmp", h1Ds[k_pbpb]->GetName()));
    hTmp->SetLineWidth(0);
    leg->AddEntry(hTmp, legendEntryTexts[k_pbpb].c_str(), legendEntryOptions[k_pbpb].c_str());

    hTmp = (TH1D*)h1Ds[k_pbpb]->Clone(Form("%s_tmp2", h1Ds[k_pbpb]->GetName()));
    hTmp->SetLineWidth(0);
    hTmp->SetFillColor(sysColors[k_pbpb]);
    hTmp->SetFillColorAlpha(sysColors[k_pbpb], sysTransparencies[k_pbpb]);
    hTmp->SetFillStyle(sysFillStyles[k_pbpb]);
    leg->AddEntry(hTmp, legendEntryTexts[1].c_str(), legendEntryOptions[1].c_str());

    hTmp = (TH1D*)h1Ds[k_pbpb]->Clone(Form("%s_tmp3", h1Ds[k_pbpb]->GetName()));
    hTmp->SetLineWidth(0);
    hTmp->SetFillColor(sysColors[k_pbpb+1]);
    hTmp->SetFillColorAlpha(sysColors[k_pbpb+1], sysTransparencies[k_pbpb+1]);
    hTmp->SetFillStyle(sysFillStyles[k_pbpb+1]);
    leg->AddEntry(hTmp, legendEntryTexts[2].c_str(), legendEntryOptions[2].c_str());

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
            "PbPb 10 nb^{-1}"
    };
    int nTextOverPadLines = textOverPadLines.size();
    for (int i = 0; i < nTextOverPadLines; ++i) {
        latex = new TLatex();
        setLatexOverPad(i, latex);
        latex->Draw();
    }

    textXCMSProj = 0.25;
    textYCMSProj = 0.87;
    textAlignCMSProj = 11;
    textFontCMSProj = 61;
    textSizeCMSProj = 0.06;
    latex = new TLatex();
    setLatexCMSProj(latex, "CMS");
    latex->Draw();

    textXCMSProj = 0.38;
    textYCMSProj = 0.87;
    textAlignCMSProj = 11;
    textFontCMSProj = 52;
    textSizeCMSProj = 0.05;
    latex = new TLatex();
    setLatexCMSProj(latex, "Projection");
    latex->Draw();

    c->Update();

    c->SaveAs(Form("%s.pdf", c->GetName()));
    c->Close();         // do not use Delete() for TCanvas.

    std::cout<<"Closing the input file"<<std::endl;
    input->Close();

    std::cout<<"running projectionPlot_xjz_Theory_MergedUnc() - END"<<std::endl;
}

/*
 * xjz projection for pPb based on pp data
 */
void projectionPlot_xjz_pPb(std::string inputFile, double sysReduction)
{
    std::cout<<"running projectionPlot_xjz_pPb()"<<std::endl;

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
    TCanvas* c = 0;
    if (sysReduction == 0)
        c = new TCanvas(figureNames[k_xjz_pPb].c_str(), "", windowWidth, windowHeight);
    else if (sysReduction == -1)
        c = new TCanvas(Form("%s_noSys", figureNames[k_xjz_pPb].c_str()), "", windowWidth, windowHeight);
    else
        c = new TCanvas(Form("%s_sysReduced%dPrct", figureNames[k_xjz_pPb].c_str(), (int)(sysReduction*100)), "", windowWidth, windowHeight);
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

    yMin = 0;
    yMax = 1;

    enum HISTLABELS {
        k_pp,
        kN_HISTLABELS
    };

    histPaths = {
            "h1D_xjz_pp"
    };
    markerColors = {kBlack};
    markerStyles = {kFullCircle};
    markerSizes = {1.70};
    lineColors = {kBlack};
    lineTransparencies = {1.0};
    lineWidths = {3};
    fillColors = {46};
    if (sysReduction == -1) fillColors = {0};
    fillTransparencies = {1.0};
    drawOptions = {"e same"};
    sysPaths = {
            "h1D_sysVar_xjz_pp_rel",
    };
    if (sysReduction == -1) {
        sysPaths = {
                "NULL",
        };
    }
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
        scaleBinErrors(h1Ds[i], 1./TMath::Sqrt(statsIncreasePPB));
    }

    // draw histograms
    for (int i = 0; i < nHistPaths; ++i) {

        if (i == 0) {
            hTmp = (TH1D*)h1Ds[i]->Clone(Form("%s_tmpDraw", h1Ds[i]->GetName()));
            hTmp->Draw("e");
        }

        h1DsSys[i] = (TH1D*)input->Get(sysPaths[i].c_str());
        if (h1DsSys[i] != 0) {
            if (sysReduction >= 0)
                h1DsSys[i]->Scale(1-sysReduction);
            gr = new TGraph();
            setTGraphSys(i, gr);
            drawSysUncBoxes(gr, h1Ds[i], h1DsSys[i], sysUseRelUnc[i]);
        }

        h1Ds[i]->Draw(drawOptions[i].c_str());
    }

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
    textX = 0.93;
    textYs.resize(nTextLines, 0.80);
    TLatex* latex = 0;
    for (int i = 0; i < nTextLines; ++i) {
        latex = new TLatex();
        textYs[i] = textYs[0] - i*0.056;
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
            "pPb 2 pb^{-1}"
    };
    int nTextOverPadLines = textOverPadLines.size();
    for (int i = 0; i < nTextOverPadLines; ++i) {
        latex = new TLatex();
        setLatexOverPad(i, latex);
        latex->Draw();
    }

    textXCMSProj = 0.25;
    textYCMSProj = 0.86;
    textAlignCMSProj = 11;
    textFontCMSProj = 61;
    textSizeCMSProj = 0.06;
    latex = new TLatex();
    setLatexCMSProj(latex, "CMS");
    latex->Draw();

    textXCMSProj = 0.38;
    textYCMSProj = 0.86;
    textAlignCMSProj = 11;
    textFontCMSProj = 52;
    textSizeCMSProj = 0.05;
    latex = new TLatex();
    setLatexCMSProj(latex, "Projection");
    latex->Draw();

    c->Update();

    c->SaveAs(Form("%s.pdf", c->GetName()));
    c->Close();         // do not use Delete() for TCanvas.

    std::cout<<"Closing the input file"<<std::endl;
    input->Close();

    std::cout<<"running projectionPlot_xjz_pPb() - END"<<std::endl;
}

void projectionPlot_xjz_pPb_multBins(std::string inputFile, double sysReduction)
{
    std::cout<<"running projectionPlot_xjz_pPb_multBins()"<<std::endl;

    projectionPlot_xjv_multBins(inputFile, sysReduction, 1, 1);

    std::cout<<"running projectionPlot_xjz_pPb_multBins() - END"<<std::endl;
}

void projectionPlot_xjz_pp_multBins(std::string inputFile, double sysReduction)
{
    std::cout<<"running projectionPlot_xjz_pp_multBins()"<<std::endl;

    projectionPlot_xjv_multBins(inputFile, sysReduction, 0, 1);

    std::cout<<"running projectionPlot_xjz_pp_multBins() - END"<<std::endl;
}

void projectionPlot_xjg_pPb_multBins(std::string inputFile, double sysReduction)
{
    std::cout<<"running projectionPlot_xjg_pPb_multBins()"<<std::endl;

    projectionPlot_xjv_multBins(inputFile, sysReduction, 1, 0);

    std::cout<<"running projectionPlot_xjg_pPb_multBins() - END"<<std::endl;
}

void projectionPlot_xjg_pp_multBins(std::string inputFile, double sysReduction)
{
    std::cout<<"running projectionPlot_xjg_pp_multBins()"<<std::endl;

    projectionPlot_xjv_multBins(inputFile, sysReduction, 0, 0);

    std::cout<<"running projectionPlot_xjg_pp_multBins() - END"<<std::endl;
}

/*
 * xjz and xjg projections for pp or pPb in multiplicity bins
 * Central values are based on Pythia
 * Stats unc are based on pp data
 * Sys unc are based on pp xjz results
 * if iszjet = 1, then make xjz plot. Else make xjg plot
 */
void projectionPlot_xjv_multBins(std::string inputFile, double sysReduction, int iCollision, int iszjet)
{
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
    TCanvas* c = 0;

    int iFig = -1;
    if (iCollision == 0)
        iFig = (iszjet == 1) ? k_xjz_pp_multBins : k_xjg_pp_multBins;
    else if (iCollision == 1)
        iFig = k_xjz_pPb_multBins;

    if (sysReduction == 0)
        c = new TCanvas(figureNames[iFig].c_str(), "", windowWidth, windowHeight);
    else if (sysReduction == -1)
        c = new TCanvas(Form("%s_noSys", figureNames[iFig].c_str()), "", windowWidth, windowHeight);
    else
        c = new TCanvas(Form("%s_sysReduced%dPrct", figureNames[iFig].c_str(), (int)(sysReduction*100)), "", windowWidth, windowHeight);
    std::cout<<"preparing canvas : "<< c->GetName() <<std::endl;
    setCanvas(c);
    c->cd();

    std::string vStr = (iszjet == 1) ? "Z" : "#gamma";

    xTitle = Form("x_{j%s} = p^{jet}_{T}/p^{%s}_{T}", vStr.c_str(), vStr.c_str());
    yTitle = Form("#frac{1}{N_{%s}} #frac{dN_{j%s}}{dx_{j%s}}", vStr.c_str(), vStr.c_str(), vStr.c_str());
    xTitleSize = 0.0525;
    yTitleSize = 0.0525;
    xTitleOffset = 1.25;
    yTitleOffset = 1.5;
    xTitleFont = 42;
    yTitleFont = 42;

    yMin = 0;
    yMax = 1.4;

    enum HISTLABELS {
        k_pythia_mult_lt5xmeanNch,
        k_pythia_mult_5xmeanNch,
        k_pythia_mult_10xmeanNch,
        kN_HISTLABELS
    };

    std::vector<double> eventFractionsInData =
    {
            1-0.012,
            0.012,
            0.00091
    };

    if (iszjet == 1) {
        histPaths = {
                "h1D_xjz_pythia_mult_lt5xmeanNch",
                "h1D_xjz_pythia_mult_5xmeanNch",
                "h1D_xjz_pythia_mult_10xmeanNch_rebin"
        };
        histPathsStatUnc = {
                "h1D_xjz_pp",
                "h1D_xjz_pp",
                "h1D_xjz_pp_rebin"
        };
    }
    else {
        histPaths = {
                "hxjg_nlt5",
                "hxjg_ngt5lt7",
                "hxjg_ngt7lt10"
        };
        histPathsStatUnc = {
                "hxjg_nlt5",
                "hxjg_ngt5lt7",
                "hxjg_ngt7lt10"
        };
    }

    markerColors = {kBlack, kBlue, kRed};
    markerStyles = {kFullCircle, kFullSquare, kFullCross};
    markerSizes = {1.70, 1.70, 1.70};
    lineColors = {kBlack, kBlue, kRed};
    lineTransparencies = {1.0, 1.0, 1.0};
    lineWidths = {3, 3, 3};
    fillColors = {kBlack, kBlue, kRed};
    if (sysReduction == -1) fillColors = {0, 0, 0};
    fillTransparencies = {0.4, 0.4, 0.4};
    drawOptions = {"e same", "e same", "e same"};
    if (iszjet == 1) {
        sysPaths = {
                "h1D_sysVar_xjz_pp_rel",
                "h1D_sysVar_xjz_pp_rel",
                "h1D_sysVar_xjz_pp_rel"
        };
    }
    else {
        sysPaths = {
                "hxjg_nlt5_sys",
                "hxjg_ngt5lt7_sys",
                "hxjg_ngt7lt10_sys"
        };
    }
    if (sysReduction == -1) {
        sysPaths = {
                "NULL",
                "NULL",
                "NULL"
        };
    }
    sysUseRelUnc = {true, true, true};
    if (iszjet != 1) sysUseRelUnc = {false, false, false};
    sysColors = {kBlack, kBlue, kRed};
    sysTransparencies = {0.4, 0.4, 0.4};
    sysFillStyles = {1001, 1001, 1001};

    int nHistPaths = histPaths.size();
    std::vector<TH1D*> h1Ds(nHistPaths, 0);
    std::vector<TH1D*> h1DsStatUnc(nHistPaths, 0);
    std::vector<TH1D*> h1DsSys(nHistPaths, 0);
    TGraph* gr = 0;
    TH1D* hTmp = 0;
    for (int i = 0; i < nHistPaths; ++i) {

        if (iszjet == 1 && i == k_pythia_mult_10xmeanNch) continue;

        h1Ds[i] = (TH1D*)input->Get(histPaths[i].c_str());
        setTH1D(i, h1Ds[i]);
        h1DsStatUnc[i] = (TH1D*)input->Get(histPathsStatUnc[i].c_str());
        std::vector<double> errorBars = getBinErrors(h1DsStatUnc[i]);
        setBinErrors(h1Ds[i], errorBars);

        if (iszjet == 1) {
            double eventFraction = eventFractionsInData[i];
            double statsIncrease = 0;
            if (iCollision == 0)
                statsIncrease = statsIncreasePP;
            else if (iCollision == 1)
                statsIncrease = statsIncreasePPB;
            scaleBinErrors(h1Ds[i], 1./TMath::Sqrt(statsIncrease*eventFraction));
        }
        else {
            // xjg histograms in the file are for a projection of 200/pb pp.
            // OR xjg histograms in the file contains stats error of 200/pb pp
            double statsIncrease = 0;
            if (iCollision == 0)
                statsIncrease = projectedLumiPP / 200;
            else if (iCollision == 1)
                statsIncrease = (projectedLumiPPB*208) / 200;
            scaleBinErrors(h1Ds[i], 1./TMath::Sqrt(statsIncrease));
        }
    }

    // draw histograms
    // draw in reverse order so that the histograms with larger error bar appears first and stay underneath the other
    for (int i = nHistPaths-1; i >= 0; --i) {

        if (iszjet == 1 && i == k_pythia_mult_10xmeanNch) continue;

        if ((iszjet == 1 && i == nHistPaths-2) ||  (iszjet != 1 && i == nHistPaths-1)) {
            hTmp = (TH1D*)h1Ds[i]->Clone(Form("%s_tmpDraw", h1Ds[i]->GetName()));
            hTmp->Draw("e");
        }

        if (input->Get(sysPaths[i].c_str())) {
            h1DsSys[i] = (TH1D*)(input->Get(sysPaths[i].c_str())->Clone(Form("hSys_%d", i)));
            if (sysReduction >= 0)
                h1DsSys[i]->Scale(1-sysReduction);
            gr = new TGraph();
            setTGraphSys(i, gr);
            drawSysUncBoxes(gr, h1Ds[i], h1DsSys[i], sysUseRelUnc[i]);
            //drawSysUncBoxes(gr, h1DsStatUnc[0], h1DsSys[i], sysUseRelUnc[i]);
        }

        h1Ds[i]->Draw(drawOptions[i].c_str());
    }

    legendX1 = 0.66;
    legendY1 = 0.76;
    legendWidth = 0.52;
    legendHeight = (iszjet == 1) ? 0.16 * 2/3 : 0.16;
    legendMargin = 0.15;
    legendEntryTexts = {
            "< 5 <N_{ch}>",
            "5-7 <N_{ch}>",
            "7-10 <N_{ch}>"
    };
    legendEntryOptions = {
            "pf",
            "pf",
            "pf"
    };
    TLegend* leg = new TLegend();

    for (int i = 0; i < nHistPaths; ++i) {

        if (iszjet == 1 && i == k_pythia_mult_10xmeanNch) continue;

        hTmp = (TH1D*)h1Ds[i]->Clone(Form("%s_tmp", h1Ds[i]->GetName()));
        hTmp->SetLineWidth(0);
        leg->AddEntry(hTmp, legendEntryTexts[i].c_str(), legendEntryOptions[i].c_str());
    }

    setLegend(leg);
    leg->Draw();

    textAlign = 31;
    textFont = 43;
    textSize = 32;
    if (iszjet == 1) {
        textLines = {
                Form("p_{T}^{%s} > 60 GeV/c", vStr.c_str()),
                "anti-k_{T} jet R = 0.3",
                "p_{T}^{jet} > 30 GeV/c",
                "|#eta^{jet}| < 1.6",
                Form("#Delta#phi_{j%s} > #frac{7}{8}#pi", vStr.c_str())
        };
    }
    else {
        textLines = {
                Form("p_{T}^{%s} > 60 GeV/c", vStr.c_str()),
                Form("|#eta^{%s}| < 1.44", vStr.c_str()),
                "anti-k_{T} jet R = 0.3",
                "p_{T}^{jet} > 30 GeV/c",
                "|#eta^{jet}| < 1.6",
                Form("#Delta#phi_{j%s} > #frac{7}{8}#pi", vStr.c_str())
        };
    }
    int nTextLines = textLines.size();
    textX = 0.93;
    textYs.resize(nTextLines, 0.70);
    TLatex* latex = 0;
    for (int i = 0; i < nTextLines; ++i) {
        latex = new TLatex();
        textYs[i] = textYs[0] - i*0.056;
        setLatex(i, latex);
        latex->Draw();
    }

    std::string collisionText = "";
    if (iCollision == 0)
        collisionText = Form("pp %d pb^{-1}", (int)projectedLumiPP);
    else if (iCollision == 1)
        collisionText = Form("pPb %d pb^{-1}", (int)projectedLumiPPB);

    textXsOverPad = {0.96};  //{0.22, 0.96};
    textYOverPad = 0.96;
    textAlignsOverPad = {31};  // {11, 31};
    textFontOverPad = 43;
    textSizeOverPad = 30;
    textOverPadLines = {
            //"#sqrt{s_{NN}} = 5.02 TeV",
            collisionText.c_str()
    };
    int nTextOverPadLines = textOverPadLines.size();
    for (int i = 0; i < nTextOverPadLines; ++i) {
        latex = new TLatex();
        setLatexOverPad(i, latex);
        latex->Draw();
    }

    textXCMSProj = 0.25;
    textYCMSProj = 0.86;
    textAlignCMSProj = 11;
    textFontCMSProj = 61;
    textSizeCMSProj = 0.06;
    latex = new TLatex();
    setLatexCMSProj(latex, "CMS");
    latex->Draw();

    textXCMSProj = 0.38;
    textYCMSProj = 0.86;
    textAlignCMSProj = 11;
    textFontCMSProj = 52;
    textSizeCMSProj = 0.05;
    latex = new TLatex();
    setLatexCMSProj(latex, "Projection");
    latex->Draw();

    c->Update();

    c->SaveAs(Form("%s.pdf", c->GetName()));
    c->Close();         // do not use Delete() for TCanvas.

    std::cout<<"Closing the input file"<<std::endl;
    input->Close();
}

void projectionPlot_xi(std::string inputFile, bool isxijet, double sysReduction)
{
    std::cout<<"running projectionPlot_xi()"<<std::endl;

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
    TCanvas* c = 0 ;

    int xiIndex = isxijet ? k_xijet : k_xigamma;
    if (sysReduction == 0)
        c = new TCanvas(figureNames[xiIndex].c_str(), "", windowWidth, windowHeight);
    else if (sysReduction == -1)
        c = new TCanvas(Form("%s_noSys", figureNames[xiIndex].c_str()), "", windowWidth, windowHeight);
    else
        c = new TCanvas(Form("%s_sysReduced%dPrct", figureNames[xiIndex].c_str(), (int)(sysReduction*100)), "", windowWidth, windowHeight);
    std::cout<<"preparing canvas : "<< c->GetName() <<std::endl;
    setCanvas(c);
    c->cd();

    xTitle = "#xi^{jet}";
    yTitle = "#frac{1}{N^{jet}} #frac{dN^{trk}}{d#xi^{jet}}";
    if (!isxijet) {
        xTitle = "#xi^{#gamma}_{T}";
        yTitle = "#frac{1}{N^{jet}} #frac{dN^{trk}}{d#xi^{#gamma}_{T}}";
    }
    xTitleSize = 0.0525;
    yTitleSize = 0.0525;
    xTitleOffset = 1.25;
    yTitleOffset = 1.5;
    xTitleFont = 42;
    yTitleFont = 42;

    yMin = 0;
    yMax = 5.5;

    enum HISTLABELS {
        k_pbpb,
        k_pp,
        kN_HISTLABELS
    };

    histPaths = {
            "hff_final_pbpbdata_recoreco_0_20",
            "hff_final_ppdata_srecoreco_0_20"
    };
    markerColors = {kBlack, kBlack};
    markerStyles = {kFullCircle, kOpenCircle};
    markerSizes = {1.70, 1.70};
    lineColors = {kBlack, kBlack};
    lineTransparencies = {1.0, 1.0};
    lineWidths = {3, 3};
    fillColors = {TColor::GetColor("#ef5253"), TColor::GetColor("#6699cc")};
    if (sysReduction == -1) fillColors = {0, 0};
    fillTransparencies = {0.7, 0.7};
    drawOptions = {"e same", "e same"};
    sysPaths = {
            "hff_final_pbpbdata_recoreco_0_20_systematics",
            "hff_final_ppdata_srecoreco_0_20_systematics"
    };
    if (sysReduction == -1) {
        sysPaths = {
                "NULL",
                "NULL"
        };
    }
    sysUseRelUnc = {false, false};
    sysColors = {TColor::GetColor("#ef5253"), TColor::GetColor("#6699cc")};
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
        scaleBinErrors(h1Ds[i], 1./TMath::Sqrt(statsIncreasePBPB));

        // set x-axis range
        h1Ds[i]->SetAxisRange(0.5 + 0.001, 4.5 - 0.001, "X");
    }

    // draw histograms
    for (int i = 0; i < nHistPaths; ++i) {

        if (i == 0) {
            hTmp = (TH1D*)h1Ds[i]->Clone(Form("%s_tmpDraw", h1Ds[i]->GetName()));
            hTmp->Draw("e");
        }

        h1DsSys[i] = (TH1D*)input->Get(sysPaths[i].c_str());
        if (h1DsSys[i] != 0) {
            if (sysReduction >= 0)
                h1DsSys[i]->Scale(1-sysReduction);
            gr = new TGraph();
            setTGraphSys(i, gr);
            drawSysUncBoxes(gr, h1Ds[i], h1DsSys[i], sysUseRelUnc[i]);
        }

        h1Ds[i]->Draw(drawOptions[i].c_str());
    }

    legendX1 = 0.24;
    legendY1 = 0.72;
    legendWidth = 0.54;
    legendHeight = 0.12;
    legendMargin = 0.15;
    legendEntryTexts = {
            "PbPb Cent. 0-10 %",
            "pp (smeared)"
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

    textAlign = 11;
    textFont = 43;
    textSize = 26;
    textLines = {
            "p_{T}^{trk} > 1 GeV/c",
            "anti-k_{T} jet R = 0.3",
            "p_{T}^{jet} > 30 GeV/c",
            "|#eta^{jet}| < 1.6",
            "p_{T}^{#gamma} > 60 GeV/c",
            "|#eta^{#gamma}| < 1.44",
            "#Delta#phi_{j#gamma} > #frac{7#pi}{8}"
    };
    int nTextLines = textLines.size();
    textX = 0.26;
    textYs.resize(nTextLines, 0.68);
    TLatex* latex = 0;
    for (int i = 0; i < nTextLines; ++i) {
        latex = new TLatex();
        textYs[i] = textYs[0] - i*0.056;
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
            "PbPb 10 nb^{-1}, pp 650 pb^{-1}"
    };
    int nTextOverPadLines = textOverPadLines.size();
    for (int i = 0; i < nTextOverPadLines; ++i) {
        latex = new TLatex();
        setLatexOverPad(i, latex);
        latex->Draw();
    }

    textXCMSProj = 0.25;
    textYCMSProj = 0.86;
    textAlignCMSProj = 11;
    textFontCMSProj = 61;
    textSizeCMSProj = 0.06;
    latex = new TLatex();
    setLatexCMSProj(latex, "CMS");
    latex->Draw();

    textXCMSProj = 0.38;
    textYCMSProj = 0.86;
    textAlignCMSProj = 11;
    textFontCMSProj = 52;
    textSizeCMSProj = 0.05;
    latex = new TLatex();
    setLatexCMSProj(latex, "Projection");
    latex->Draw();

    c->Update();

    c->SaveAs(Form("%s.pdf", c->GetName()));
    c->Close();         // do not use Delete() for TCanvas.

    std::cout<<"Closing the input file"<<std::endl;
    input->Close();

    std::cout<<"running projectionPlot_xi() - END"<<std::endl;
}

void projectionPlot_xi_MergedUnc(std::string inputFile, bool isxijet, double sysReduction)
{
    std::cout<<"running projectionPlot_xi_MergedUnc()"<<std::endl;

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
    TCanvas* c = 0 ;

    int xiIndex = isxijet ? k_xijet_MergedUnc : k_xigamma_MergedUnc;
    if (sysReduction == 0)
        c = new TCanvas(figureNames[xiIndex].c_str(), "", windowWidth, windowHeight);
    else if (sysReduction == -1)
        c = new TCanvas(Form("%s_noSys", figureNames[xiIndex].c_str()), "", windowWidth, windowHeight);
    else
        c = new TCanvas(Form("%s_sysReduced%dPrct", figureNames[xiIndex].c_str(), (int)(sysReduction*100)), "", windowWidth, windowHeight);
    std::cout<<"preparing canvas : "<< c->GetName() <<std::endl;
    setCanvas(c);
    c->cd();

    xTitle = "#xi^{jet}";
    yTitle = "#frac{1}{N^{jet}} #frac{dN^{trk}}{d#xi^{jet}}";
    if (!isxijet) {
        xTitle = "#xi^{#gamma}_{T}";
        yTitle = "#frac{1}{N^{jet}} #frac{dN^{trk}}{d#xi^{#gamma}_{T}}";
    }
    xTitleSize = 0.0525;
    yTitleSize = 0.0525;
    xTitleOffset = 1.25;
    yTitleOffset = 1.5;
    xTitleFont = 42;
    yTitleFont = 42;

    yMin = 0;
    yMax = 5.5;

    enum HISTLABELS {
        k_pbpb,
        k_pp,
        kN_HISTLABELS
    };

    histPaths = {
            "hff_final_pbpbdata_recoreco_0_20",
            "hff_final_ppdata_srecoreco_0_20"
    };
    markerColors = {kBlack, kBlack};
    markerStyles = {kFullCircle, kOpenCircle};
    markerSizes = {1.70, 1.70};
    lineColors = {kBlack, 45};
    lineTransparencies = {1.0, 1.0};
    lineWidths = {0, 0};
    fillColors = {0, 0};
    if (sysReduction == -1) fillColors = {0, 0};
    fillTransparencies = {0.8, 0.8};
    drawOptions = {"e same", "e same"};
    sysPaths = {
            "hff_final_pbpbdata_recoreco_0_20_systematics",
            "hff_final_ppdata_srecoreco_0_20_systematics"
    };
    if (sysReduction == -1) {
        sysPaths = {
                "NULL",
                "NULL"
        };
    }
    sysUseRelUnc = {false, false, false, false};
    sysColors = {TColor::GetColor("#a09f93"), 43, TColor::GetColor("#ad33ff"), TColor::GetColor("#29a329")};
    sysTransparencies = {0.8, 0.8, 0.8, 0.8};
    sysFillStyles = {1001, 1001, 3254, 3245};

    int nHistPaths = histPaths.size();
    std::vector<TH1D*> h1Ds(nHistPaths, 0);
    std::vector<TH1D*> h1DsSys(nHistPaths, 0);
    TGraph* gr = 0;
    TH1D* hTmp = 0;
    for (int i = 0; i < nHistPaths; ++i) {

        h1Ds[i] = (TH1D*)input->Get(histPaths[i].c_str());
        setTH1D(i, h1Ds[i]);

        // set x-axis range
        h1Ds[i]->SetAxisRange(0.5 + 0.001, 4.5 - 0.001, "X");
    }

    for (int i = 0; i < nHistPaths; ++i) {
        h1DsSys[i] = (TH1D*)input->Get(sysPaths[i].c_str());
    }

    std::vector<TH1D*> h1DsMergedUncCurrent(nHistPaths, 0);
    std::vector<TH1D*> h1DsMergedUncProjection(nHistPaths, 0);

    for (int i = 0; i < nHistPaths; ++i) {

        h1DsMergedUncCurrent[i] =(TH1D*)h1Ds[i]->Clone(Form("%s_MergedUncCurrent", h1Ds[i]->GetName()));
        h1DsMergedUncProjection[i] =(TH1D*)h1Ds[i]->Clone(Form("%s_MergedUncProjection", h1Ds[i]->GetName()));
    }

    // draw histograms
    for (int i = 0; i < nHistPaths; ++i) {

        if (i == 0) {
            hTmp = (TH1D*)h1Ds[i]->Clone(Form("%s_tmpDraw", h1Ds[i]->GetName()));
            hTmp->Draw("e");
        }

        if (h1DsSys[i] != 0) {
            // prepare and draw current uncertainty
            mergeUncWithErrorBar(h1DsMergedUncCurrent[i], h1DsSys[i], sysUseRelUnc[i]);

            for (int iBin = 1; iBin <= h1DsMergedUncCurrent[i]->GetNbinsX(); ++iBin) {
                double binContent = h1DsMergedUncCurrent[i]->GetBinContent(iBin);
                double binError = h1DsMergedUncCurrent[i]->GetBinError(iBin);
                if (sysUseRelUnc[i])
                    h1DsMergedUncCurrent[i]->SetBinContent(iBin, binError/binContent);
                else
                    h1DsMergedUncCurrent[i]->SetBinContent(iBin, binError);
            }

            gr = new TGraph();
            setTGraphSys(i, gr);
            gr->SetLineWidth(3);
            drawSysUncBoxes(gr, h1Ds[i], h1DsMergedUncCurrent[i], sysUseRelUnc[i]);

            // prepare and draw projected uncertainty
            scaleBinErrors(h1DsMergedUncProjection[i], 1./TMath::Sqrt(statsIncreasePBPB));
            if (sysReduction >= 0) {
                h1DsSys[i]->Scale(1-sysReduction);
            }
            mergeUncWithErrorBar(h1DsMergedUncProjection[i], h1DsSys[i], sysUseRelUnc[i]);

            for (int iBin = 1; iBin <= h1DsMergedUncProjection[i]->GetNbinsX(); ++iBin) {
                double binContent = h1DsMergedUncProjection[i]->GetBinContent(iBin);
                double binError = h1DsMergedUncProjection[i]->GetBinError(iBin);
                if (sysUseRelUnc[i])
                    h1DsMergedUncProjection[i]->SetBinContent(iBin, binError/binContent);
                else
                    h1DsMergedUncProjection[i]->SetBinContent(iBin, binError);
            }

            gr = new TGraph();
            setTGraphSys(i+2, gr);
            gr->SetLineWidth(3);
            drawSysUncBoxes(gr, h1Ds[i], h1DsMergedUncProjection[i], sysUseRelUnc[i]);
        }

        h1Ds[i]->Draw(drawOptions[i].c_str());
    }

    TLegend* leg = 0;

    legendX1 = 0.24;
    legendY1 = 0.70;
    legendWidth = 0.54;
    legendHeight = 0.14;
    legendMargin = 0.15;
    legendEntryTexts = {"PbPb Cent. 0-10 %", "Current Unc.", "Projected Unc."};
    legendEntryOptions = {"p", "f", "f"};
    leg = new TLegend();
    hTmp = (TH1D*)h1Ds[k_pbpb]->Clone(Form("%s_tmp", h1Ds[k_pbpb]->GetName()));
    hTmp->SetLineWidth(0);
    leg->AddEntry(hTmp, legendEntryTexts[k_pbpb].c_str(), legendEntryOptions[k_pbpb].c_str());

    hTmp = (TH1D*)h1Ds[k_pbpb]->Clone(Form("%s_tmp2", h1Ds[k_pbpb]->GetName()));
    hTmp->SetLineWidth(0);
    hTmp->SetFillColor(sysColors[k_pbpb]);
    hTmp->SetFillColorAlpha(sysColors[k_pbpb], sysTransparencies[k_pbpb]);
    hTmp->SetFillStyle(sysFillStyles[k_pbpb]);
    leg->AddEntry(hTmp, legendEntryTexts[1].c_str(), legendEntryOptions[1].c_str());

    hTmp = (TH1D*)h1Ds[k_pbpb]->Clone(Form("%s_tmp3", h1Ds[k_pbpb]->GetName()));
    hTmp->SetLineWidth(0);
    hTmp->SetFillColor(sysColors[k_pbpb+2]);
    hTmp->SetFillColorAlpha(sysColors[k_pbpb+2], sysTransparencies[k_pbpb+2]);
    hTmp->SetFillStyle(sysFillStyles[k_pbpb+2]);
    leg->AddEntry(hTmp, legendEntryTexts[2].c_str(), legendEntryOptions[2].c_str());

    setLegend(leg);
    leg->Draw();

    legendX1 = 0.62;
    legendY1 = 0.70;
    legendWidth = 0.54;
    legendHeight = 0.14;
    legendMargin = 0.15;
    legendEntryTexts = {"pp (smeared)", "Current Unc.", "Projected Unc."};
    legendEntryOptions = {"p", "f", "f"};
    leg = new TLegend();
    hTmp = (TH1D*)h1Ds[k_pp]->Clone(Form("%s_tmp", h1Ds[k_pbpb]->GetName()));
    hTmp->SetLineWidth(0);
    leg->AddEntry(hTmp, legendEntryTexts[k_pbpb].c_str(), legendEntryOptions[k_pbpb].c_str());

    hTmp = (TH1D*)h1Ds[k_pp]->Clone(Form("%s_tmp2", h1Ds[k_pbpb]->GetName()));
    hTmp->SetLineWidth(0);
    hTmp->SetFillColor(sysColors[k_pp]);
    hTmp->SetFillColorAlpha(sysColors[k_pp], sysTransparencies[k_pp]);
    hTmp->SetFillStyle(sysFillStyles[k_pp]);
    leg->AddEntry(hTmp, legendEntryTexts[1].c_str(), legendEntryOptions[1].c_str());

    hTmp = (TH1D*)h1Ds[k_pp]->Clone(Form("%s_tmp3", h1Ds[k_pbpb]->GetName()));
    hTmp->SetLineWidth(0);
    hTmp->SetFillColor(sysColors[k_pp+2]);
    hTmp->SetFillColorAlpha(sysColors[k_pp+2], sysTransparencies[k_pp+2]);
    hTmp->SetFillStyle(sysFillStyles[k_pp+2]);
    leg->AddEntry(hTmp, legendEntryTexts[2].c_str(), legendEntryOptions[2].c_str());

    setLegend(leg);
    leg->Draw();

    textAlign = 11;
    textFont = 43;
    textSize = 26;
    textLines = {
            "p_{T}^{trk} > 1 GeV/c",
            "anti-k_{T} jet R = 0.3",
            "p_{T}^{jet} > 30 GeV/c",
            "|#eta^{jet}| < 1.6",
            "p_{T}^{#gamma} > 60 GeV/c",
            "|#eta^{#gamma}| < 1.44",
            "#Delta#phi_{j#gamma} > #frac{7#pi}{8}"
    };
    int nTextLines = textLines.size();
    textX = 0.26;
    textYs.resize(nTextLines, 0.65);
    TLatex* latex = 0;
    for (int i = 0; i < nTextLines; ++i) {
        latex = new TLatex();
        textYs[i] = textYs[0] - i*0.056;
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
            "PbPb 10 nb^{-1}, pp 650 pb^{-1}"
    };
    int nTextOverPadLines = textOverPadLines.size();
    for (int i = 0; i < nTextOverPadLines; ++i) {
        latex = new TLatex();
        setLatexOverPad(i, latex);
        latex->Draw();
    }

    textXCMSProj = 0.25;
    textYCMSProj = 0.86;
    textAlignCMSProj = 11;
    textFontCMSProj = 61;
    textSizeCMSProj = 0.06;
    latex = new TLatex();
    setLatexCMSProj(latex, "CMS");
    latex->Draw();

    textXCMSProj = 0.38;
    textYCMSProj = 0.86;
    textAlignCMSProj = 11;
    textFontCMSProj = 52;
    textSizeCMSProj = 0.05;
    latex = new TLatex();
    setLatexCMSProj(latex, "Projection");
    latex->Draw();

    c->Update();

    c->SaveAs(Form("%s.pdf", c->GetName()));
    c->Close();         // do not use Delete() for TCanvas.

    std::cout<<"Closing the input file"<<std::endl;
    input->Close();

    std::cout<<"running projectionPlot_xi_MergedUnc() - END"<<std::endl;
}

void projectionPlot_xi_ratio(std::string inputFile, bool isxijet, double sysReduction)
{
    std::cout<<"running projectionPlot_xi_ratio()"<<std::endl;

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
    rightMargin  = 0.03;
    bottomMargin = 0.17;
    topMargin    = 0.04;
    TCanvas* c = 0 ;

    int xiIndex = isxijet ? k_xijet_ratio : k_xigamma_ratio;
    if (sysReduction == 0)
        c = new TCanvas(figureNames[xiIndex].c_str(), "", windowWidth, windowHeight);
    else if (sysReduction == -1)
        c = new TCanvas(Form("%s_noSys", figureNames[xiIndex].c_str()), "", windowWidth, windowHeight);
    else
        c = new TCanvas(Form("%s_sysReduced%dPrct", figureNames[xiIndex].c_str(), (int)(sysReduction*100)), "", windowWidth, windowHeight);
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

    xTitle = "#xi^{jet}";
    yTitle = "#frac{1}{N^{jet}} #frac{dN^{trk}}{d#xi^{jet}}";
    if (!isxijet) {
        xTitle = "#xi^{#gamma}_{T}";
        yTitle = "#frac{1}{N^{jet}} #frac{dN^{trk}}{d#xi^{#gamma}_{T}}";
    }
    xTitleSize = 0.056;
    yTitleSize = 0.056;
    xTitleOffset = 1.25;
    yTitleOffset = 1.50;
    xTitleFont = 42;
    yTitleFont = 42;

    yMin = 0;
    yMax = 4.2;

    enum HISTLABELS {
        k_pbpb,
        k_pp,
        k_ratio,
        kN_HISTLABELS
    };

    histPaths = {
            "hff_final_pbpbdata_recoreco_0_20",
            "hff_final_ppdata_srecoreco_0_20",
            "hff_final_ratio_0_20"
    };
    markerColors = {kBlack, kBlack, kBlack};
    markerStyles = {kFullCircle, kOpenCircle, kFullSquare};
    markerSizes = {1.70, 1.70, 1.70};
    lineColors = {kBlack, kBlack, kBlack};
    lineTransparencies = {1.0, 1.0, 1.0};
    lineWidths = {3, 3, 3};
    fillColors = {TColor::GetColor("#ef5253"), TColor::GetColor("#6699cc"), TColor::GetColor("#a09f93")};
    if (sysReduction == -1) fillColors = {0, 0, 0};
    fillTransparencies = {0.7, 0.7, 0.7};
    drawOptions = {"e same", "e same", "e same"};
    sysPaths = {
            "hff_final_pbpbdata_recoreco_0_20_systematics",
            "hff_final_ppdata_srecoreco_0_20_systematics",
            "hff_final_ratio_0_20_systematics"
    };
    if (sysReduction == -1) {
        sysPaths = {
                "NULL",
                "NULL",
                "NULL",
        };
    }
    sysUseRelUnc = {false, false, false};
    sysColors = {TColor::GetColor("#ef5253"), TColor::GetColor("#6699cc"), TColor::GetColor("#a09f93")};
    sysTransparencies = {0.7, 0.7, 0.7};
    sysFillStyles = {1001, 1001, 1001};

    int nHistPaths = histPaths.size();
    std::vector<TH1D*> h1Ds(nHistPaths, 0);
    std::vector<TH1D*> h1DsSys(nHistPaths, 0);
    TGraph* gr = 0;
    TH1D* hTmp = 0;
    for (int i = 0; i < nHistPaths; ++i) {

        h1Ds[i] = (TH1D*)input->Get(histPaths[i].c_str());
        setTH1D(i, h1Ds[i]);
        scaleBinErrors(h1Ds[i], 1./TMath::Sqrt(statsIncreasePBPB));

        // set x-axis range
        h1Ds[i]->SetAxisRange(0.5 + 0.001, 4.5 - 0.001, "X");

        h1Ds[i]->GetXaxis()->SetLabelSize(h1Ds[i]->GetXaxis()->GetLabelSize()*1.4);
        h1Ds[i]->GetYaxis()->SetLabelSize(h1Ds[i]->GetYaxis()->GetLabelSize()*1.4);

        h1Ds[i]->GetXaxis()->SetLabelOffset(h1Ds[i]->GetXaxis()->GetLabelOffset()*2.5);
        h1Ds[i]->GetYaxis()->SetLabelOffset(h1Ds[i]->GetYaxis()->GetLabelOffset()*2);
    }
    h1Ds[k_ratio]->SetYTitle("PbPb / pp");
    h1Ds[k_ratio]->SetMinimum(0.4);
    h1Ds[k_ratio]->SetMaximum(2.0);
    if (!isxijet) {
        h1Ds[k_ratio]->SetMinimum(0);
        h1Ds[k_ratio]->SetMaximum(3);
    }

    // smooth the pbpb / pp
    TF1* f1 = 0;
    std::vector<TH1D*> h1DsFit(nHistPaths, 0);
    h1DsFit[k_ratio] = (TH1D*)h1Ds[k_ratio]->Clone(Form("%s_Fit", h1Ds[k_ratio]->GetName()));
    f1 = new TF1(Form("%s_tmpF1", h1Ds[k_ratio]->GetName()), "pol3", 0.75, 3.75);
    h1DsFit[k_ratio]->Fit(f1, "N M R 0");
    if (isxijet) {
        fillTH1fromTF1(h1Ds[k_ratio], f1, 2, 9);
    }
    else {
        fillTH1fromTF1(h1Ds[k_ratio], f1, 3, 8);
    }
    hTmp = (TH1D*)h1Ds[k_ratio]->Clone(Form("%s_sf", h1Ds[k_ratio]->GetName()));
    hTmp->Divide(h1DsFit[k_ratio]);
    // modify pbpb distribution using the smoothed pbpb / pp ratio
    for (int iBin = 1; iBin <= hTmp->GetNbinsX(); ++iBin) {
        h1Ds[k_pbpb]->SetBinContent(iBin, h1Ds[k_pbpb]->GetBinContent(iBin)*hTmp->GetBinContent(iBin));
        h1Ds[k_pbpb]->SetBinError(iBin, h1Ds[k_pbpb]->GetBinError(iBin)*TMath::Sqrt(hTmp->GetBinContent(iBin)));
    }

    // draw histograms
    for (int i = 0; i < nHistPaths; ++i) {

        c->cd(1);
        if (i == k_ratio) c->cd(2);

        if (i == 0 || i == k_ratio) {
            hTmp = (TH1D*)h1Ds[i]->Clone(Form("%s_tmpDraw", h1Ds[i]->GetName()));
            hTmp->Draw("e");
        }

        h1DsSys[i] = (TH1D*)input->Get(sysPaths[i].c_str());
        if (h1DsSys[i] != 0) {
            if (sysReduction >= 0)
                h1DsSys[i]->Scale(1-sysReduction);
            gr = new TGraph();
            setTGraphSys(i, gr);
            drawSysUncBoxes(gr, h1Ds[i], h1DsSys[i], sysUseRelUnc[i]);
        }

        h1Ds[i]->Draw(drawOptions[i].c_str());
    }

    c->cd(1);
    legendX1 = 0.24;
    legendY1 = 0.70;
    legendWidth = 0.54;
    legendHeight = 0.16;
    legendMargin = 0.15;
    legendEntryTexts = {
            "PbPb Cent. 0-10 %",
            "pp (smeared)"
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

    c->cd(2);
    textAlign = 11;
    textFont = 43;
    textSize = 26;
    textLines = {
            "p_{T}^{trk} > 1 GeV/c",
            "anti-k_{T} jet R = 0.3",
            "p_{T}^{jet} > 30 GeV/c",
            "|#eta^{jet}| < 1.6",
            "p_{T}^{#gamma} > 60 GeV/c",
            "|#eta^{#gamma}| < 1.44",
            "#Delta#phi_{j#gamma} > #frac{7#pi}{8}"
    };
    int nTextLines = textLines.size();
    textX = 0.26;
    textYs.resize(nTextLines, 0.93);
    TLatex* latex = 0;
    for (int i = 0; i < nTextLines; ++i) {
        latex = new TLatex();
        textYs[i] = textYs[0] - i*0.065;
        setLatex(i, latex);
        latex->Draw();
    }

    c->Update();
    c->cd(1);
    textAlign = 11;
    textFont = 43;
    textSize = 30;
    textLines = {
            "#sqrt{s_{NN}} = 5.02 TeV",
            "PbPb 10 nb^{-1}",
            "pp 650 pb^{-1}"
    };
    nTextLines = textLines.size();
    textX = 0.26;
    textYs.clear();
    textYs.resize(nTextLines, 0.62);
    latex = 0;
    for (int i = 0; i < nTextLines; ++i) {
        latex = new TLatex();
        textYs[i] = textYs[0] - i*0.08;
        setLatex(i, latex);
        latex->Draw();
    }

    textXCMSProj = 0.25;
    textYCMSProj = 0.88;
    textAlignCMSProj = 11;
    textFontCMSProj = 61;
    textSizeCMSProj = 0.07;
    latex = new TLatex();
    setLatexCMSProj(latex, "CMS");
    latex->Draw();

    textXCMSProj = 0.38;
    textYCMSProj = 0.88;
    textAlignCMSProj = 11;
    textFontCMSProj = 52;
    textSizeCMSProj = 0.06;
    latex = new TLatex();
    setLatexCMSProj(latex, "Projection");
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
    line->SetLineStyle(kDashed);
    line->Draw();

    c->cd();
    c->Update();
    c->SaveAs(Form("%s.pdf", c->GetName()));
    c->Close();         // do not use Delete() for TCanvas.

    std::cout<<"Closing the input file"<<std::endl;
    input->Close();

    std::cout<<"running projectionPlot_xi_ratio() - END"<<std::endl;
}

void projectionPlot_xi_ratioOnly(std::string inputFile, bool isxijet, double sysReduction)
{
    std::cout<<"running projectionPlot_xi_ratioOnly()"<<std::endl;

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
    TCanvas* c = 0 ;

    int xiIndex = isxijet ? k_xijet_ratioOnly : k_xigamma_ratioOnly;
    if (sysReduction == 0)
        c = new TCanvas(figureNames[xiIndex].c_str(), "", windowWidth, windowHeight);
    else if (sysReduction == -1)
        c = new TCanvas(Form("%s_noSys", figureNames[xiIndex].c_str()), "", windowWidth, windowHeight);
    else
        c = new TCanvas(Form("%s_sysReduced%dPrct", figureNames[xiIndex].c_str(), (int)(sysReduction*100)), "", windowWidth, windowHeight);

    std::cout<<"preparing canvas : "<< c->GetName() <<std::endl;
    setCanvas(c);
    c->cd();

    xTitle = "#xi^{jet}";
    if (!isxijet) {
        xTitle = "#xi^{#gamma}_{T}";
    }
    yTitle = "PbPb / pp";
    xTitleSize = 0.0525;
    yTitleSize = 0.0525;
    xTitleOffset = 1.25;
    yTitleOffset = 1.5;
    xTitleFont = 42;
    yTitleFont = 42;

    yMin = 0.4;
    yMax = 2.0;
    if (!isxijet) {
        yMin = 0;
        yMax = 3;
    }

    enum HISTLABELS {
        k_ratio,
        kN_HISTLABELS
    };

    histPaths = {
            "hff_final_ratio_0_20"
    };
    markerColors = {kBlack};
    markerStyles = {kFullSquare};
    markerSizes = {1.70};
    lineColors = {kBlack};
    lineTransparencies = {1.0};
    lineWidths = {3};
    fillColors = {TColor::GetColor("#a09f93")};
    if (sysReduction == -1) fillColors = {0};
    fillTransparencies = {0.7};
    drawOptions = {"e same"};
    sysPaths = {
            "hff_final_ratio_0_20_systematics"
    };
    if (sysReduction == -1) {
        sysPaths = {
                "NULL",
        };
    }
    sysUseRelUnc = {false};
    sysColors = {TColor::GetColor("#a09f93")};
    sysTransparencies = {0.7};
    sysFillStyles = {1001};

    int nHistPaths = histPaths.size();
    std::vector<TH1D*> h1Ds(nHistPaths, 0);
    std::vector<TH1D*> h1DsSys(nHistPaths, 0);
    TGraph* gr = 0;
    TH1D* hTmp = 0;
    TF1* f1 = 0;
    std::vector<TH1D*> h1DsFit(nHistPaths, 0);
    for (int i = 0; i < nHistPaths; ++i) {

        h1Ds[i] = (TH1D*)input->Get(histPaths[i].c_str());
        setTH1D(i, h1Ds[i]);
        scaleBinErrors(h1Ds[i], 1./TMath::Sqrt(statsIncreasePBPB));

        // set x-axis range
        h1Ds[i]->SetAxisRange(0.5 + 0.001, 4.5 - 0.001, "X");

        h1DsFit[i] = (TH1D*)h1Ds[i]->Clone(Form("%s_Fit", h1Ds[i]->GetName()));
        f1 = new TF1(Form("%s_tmpF1", h1Ds[i]->GetName()), "pol3", 0.75, 3.75);
        h1DsFit[i]->Fit(f1, "N M R 0");
        if (isxijet) {
            fillTH1fromTF1(h1Ds[i], f1, 2, 9);
        }
        else {
            fillTH1fromTF1(h1Ds[i], f1, 3, 8);
        }
    }

    // draw histograms
    for (int i = 0; i < nHistPaths; ++i) {

        if (i == 0) {
            hTmp = (TH1D*)h1Ds[i]->Clone(Form("%s_tmpDraw", h1Ds[i]->GetName()));
            hTmp->Draw("e");
        }

        h1DsSys[i] = (TH1D*)input->Get(sysPaths[i].c_str());
        if (h1DsSys[i] != 0) {
            if (sysReduction >= 0)
                h1DsSys[i]->Scale(1-sysReduction);
            gr = new TGraph();
            setTGraphSys(i, gr);
            drawSysUncBoxes(gr, h1Ds[i], h1DsSys[i], sysUseRelUnc[i]);
        }

        h1Ds[i]->Draw(drawOptions[i].c_str());
    }

    /*
    legendX1 = 0.24;
    legendY1 = 0.72;
    legendWidth = 0.54;
    legendHeight = 0.12;
    legendMargin = 0.15;
    legendEntryTexts = {
            "PbPb / pp"
    };
    legendEntryOptions = {
            "pf"
    };
    TLegend* leg = new TLegend();

    hTmp = (TH1D*)h1Ds[k_ratio]->Clone(Form("%s_tmp", h1Ds[k_ratio]->GetName()));
    hTmp->SetLineWidth(0);
    leg->AddEntry(hTmp, legendEntryTexts[k_ratio].c_str(), legendEntryOptions[k_ratio].c_str());

    setLegend(leg);
    leg->Draw();
    */

    textAlign = 11;
    textFont = 43;
    textSize = 26;
    textLines = {
            "PbPb Cent. 0-10 %",
            "p_{T}^{trk} > 1 GeV/c",
            "anti-k_{T} jet R = 0.3",
            "p_{T}^{jet} > 30 GeV/c, |#eta^{jet}| < 1.6",
            "p_{T}^{#gamma} > 60 GeV/c, |#eta^{#gamma}| < 1.44",
            "#Delta#phi_{j#gamma} > #frac{7#pi}{8}"
    };
    int nTextLines = textLines.size();
    textX = 0.25;
    textYs.resize(nTextLines, 0.80);
    TLatex* latex = 0;
    for (int i = 0; i < nTextLines; ++i) {
        latex = new TLatex();
        textYs[i] = textYs[0] - i*0.056;
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
            "PbPb 10 nb^{-1}, pp 650 pb^{-1}"
    };
    int nTextOverPadLines = textOverPadLines.size();
    for (int i = 0; i < nTextOverPadLines; ++i) {
        latex = new TLatex();
        setLatexOverPad(i, latex);
        latex->Draw();
    }

    textXCMSProj = 0.25;
    textYCMSProj = 0.86;
    textAlignCMSProj = 11;
    textFontCMSProj = 61;
    textSizeCMSProj = 0.06;
    latex = new TLatex();
    setLatexCMSProj(latex, "CMS");
    latex->Draw();

    textXCMSProj = 0.38;
    textYCMSProj = 0.86;
    textAlignCMSProj = 11;
    textFontCMSProj = 52;
    textSizeCMSProj = 0.05;
    latex = new TLatex();
    setLatexCMSProj(latex, "Projection");
    latex->Draw();

    c->Update();

    TLine* line = new TLine(gPad->GetUxmin(), 1, gPad->GetUxmax(), 1);
    line->SetLineStyle(kDashed);
    line->Draw();

    c->SaveAs(Form("%s.pdf", c->GetName()));
    c->Close();         // do not use Delete() for TCanvas.

    std::cout<<"Closing the input file"<<std::endl;
    input->Close();

    std::cout<<"running projectionPlot_xi_ratioOnly() - END"<<std::endl;
}

void projectionPlot_js_ratioOnly(std::string inputFile, double sysReduction, int iFitFnc)
{
    std::cout<<"running projectionPlot_js_ratioOnly()"<<std::endl;

    TFile* input  = TFile::Open(inputFile.c_str());

    // no horizontal error bars
    gStyle->SetErrorX(0);
    gStyle->SetHatchesLineWidth(3);

    windowWidth = 800;
    windowHeight = 800;
    logX = 0;
    logY = 0;
    leftMargin   = 0.21;
    rightMargin  = 0.06;
    bottomMargin = 0.15;
    topMargin    = 0.12;
    TCanvas* c = 0 ;

    int jsIndex = k_js_ratioOnly;
    if (iFitFnc == 0) {
        jsIndex = k_js_ratioOnly_fit_pol2;
    }
    else if (iFitFnc == 1) {
        jsIndex = k_js_ratioOnly_fit_pol3;
    }
    if (sysReduction == 0)
        c = new TCanvas(figureNames[jsIndex].c_str(), "", windowWidth, windowHeight);
    else if (sysReduction == -1)
        c = new TCanvas(Form("%s_noSys", figureNames[jsIndex].c_str()), "", windowWidth, windowHeight);
    else
        c = new TCanvas(Form("%s_sysReduced%dPrct", figureNames[jsIndex].c_str(), (int)(sysReduction*100)), "", windowWidth, windowHeight);

    std::cout<<"preparing canvas : "<< c->GetName() <<std::endl;
    setCanvas(c);
    c->cd();

    xTitle = "r";
    yTitle = "#rho(r)_{PbPb} / #rho(r)_{pp}";
    xTitleSize = 0.09;
    yTitleSize = 0.09;
    xTitleOffset = 0.80;
    yTitleOffset = 0.98;
    xTitleFont = 42;
    yTitleFont = 42;

    yMin = 0.6;
    yMax = 3.4;

    enum HISTLABELS {
        k_ratio,
        kN_HISTLABELS
    };

    histPaths = {
            "hjs_final_ratio_0_20"
    };
    markerColors = {TColor::GetColor("#e60040")};
    markerStyles = {kFullSquare};
    markerSizes = {3.0};
    lineColors = {kBlack};
    lineTransparencies = {1.0};
    lineWidths = {3};
    fillColors = {TColor::GetColor("#e60040")};
    if (sysReduction == -1) fillColors = {0};
    fillTransparencies = {0.7};
    drawOptions = {"e same"};
    sysPaths = {
            "hjs_final_ratio_0_20_systematics"
    };
    if (sysReduction == -1) {
        sysPaths = {
                "NULL",
        };
    }
    sysUseRelUnc = {false};
    sysColors = {TColor::GetColor("#e60040")};
    sysTransparencies = {0.4};
    sysFillStyles = {1001};

    int nHistPaths = histPaths.size();
    std::vector<TH1D*> h1Ds(nHistPaths, 0);
    std::vector<TH1D*> h1DsSys(nHistPaths, 0);
    TGraph* gr = 0;
    TH1D* hTmp = 0;
    TF1* f1 = 0;
    std::vector<TH1D*> h1DsFit(nHistPaths, 0);
    for (int i = 0; i < nHistPaths; ++i) {

        h1Ds[i] = (TH1D*)input->Get(histPaths[i].c_str());
        setTH1D(i, h1Ds[i]);
        h1Ds[i]->SetNdivisions(504, "X");
        h1Ds[i]->SetNdivisions(505, "Y");

        h1Ds[i]->GetXaxis()->SetLabelSize(0.08);
        h1Ds[i]->GetXaxis()->SetTickSize(0.03);

        h1Ds[i]->GetYaxis()->SetLabelSize(0.08);
        h1Ds[i]->GetYaxis()->SetTickSize(0.03);

        scaleBinErrors(h1Ds[i], 1./TMath::Sqrt(statsIncreasePBPB));

        // set x-axis range
        h1Ds[i]->SetAxisRange(0 + 0.001, 0.3 - 0.001, "X");

        h1DsFit[i] = (TH1D*)h1Ds[i]->Clone(Form("%s_Fit", h1Ds[i]->GetName()));
        if (iFitFnc > -1) {
            std::string formulaTmp = "";
            if (iFitFnc == 0) {
                formulaTmp = "pol2";
            }
            else if (iFitFnc == 1) {
                formulaTmp = "pol3";
            }

            if (formulaTmp != "") {
                f1 = new TF1(Form("%s_tmpF1", h1Ds[i]->GetName()), formulaTmp.c_str(), 0.0, 0.3);
                h1DsFit[i]->Fit(f1, "N M R 0 L");
                fillTH1fromTF1(h1Ds[i], f1, h1Ds[i]->FindBin(0), h1Ds[i]->FindBin(0.3)-1);
            }
        }
    }

    // draw histograms
    for (int i = 0; i < nHistPaths; ++i) {

        if (i == 0) {
            hTmp = (TH1D*)h1Ds[i]->Clone(Form("%s_tmpDraw", h1Ds[i]->GetName()));
            hTmp->Draw("e");
        }

        h1DsSys[i] = (TH1D*)input->Get(sysPaths[i].c_str());
        if (h1DsSys[i] != 0) {
            if (sysReduction >= 0)
                h1DsSys[i]->Scale(1-sysReduction);
            gr = new TGraph();
            setTGraphSys(i, gr);
            drawSysUncBoxes(gr, h1Ds[i], h1DsSys[i], sysUseRelUnc[i]);
        }

        h1Ds[i]->Draw(drawOptions[i].c_str());
    }

    /*
    legendX1 = 0.24;
    legendY1 = 0.72;
    legendWidth = 0.54;
    legendHeight = 0.12;
    legendMargin = 0.15;
    legendEntryTexts = {
            "PbPb / pp"
    };
    legendEntryOptions = {
            "pf"
    };
    TLegend* leg = new TLegend();

    hTmp = (TH1D*)h1Ds[k_ratio]->Clone(Form("%s_tmp", h1Ds[k_ratio]->GetName()));
    hTmp->SetLineWidth(0);
    leg->AddEntry(hTmp, legendEntryTexts[k_ratio].c_str(), legendEntryOptions[k_ratio].c_str());

    setLegend(leg);
    leg->Draw();
    */

    textAlign = 11;
    textFont = 43;
    textSize = 40;
    textLines = {
            "p_{T}^{#gamma} > 60 GeV/c",
            "anti-k_{T} jet R = 0.3",
            "p_{T}^{jet} > 30 GeV/c",
            "#Delta#phi_{j#gamma} > #frac{7#pi}{8}"
    };
    int nTextLines = textLines.size();
    textX = 0.25;
    textYs.resize(nTextLines, 0.62);
    TLatex* latex = 0;
    for (int i = 0; i < nTextLines; ++i) {
        latex = new TLatex();
        textYs[i] = textYs[0] - i*0.07;
        setLatex(i, latex);
        latex->Draw();
    }

    // centrality info
    textAlign = 11;
    textFont = 62;
    textSize = 0.06;
    textLines = {
            "Cent. 0 - 10%"
    };
    nTextLines = textLines.size();
    textX = 0.56;
    textYs.clear();
    textYs.resize(nTextLines, 0.80);
    for (int i = 0; i < nTextLines; ++i) {
        latex = new TLatex();
        textYs[i] = textYs[0] - i*0.07;
        setLatex(i, latex);
        latex->Draw();
    }

    textXsOverPad = {0.22, 0.22};
    textYOverPad = -1;
    textAlignsOverPad = {11, 11};
    textFontOverPad = 43;
    textSizeOverPad = 40;
    textOverPadLines = {
            "#sqrt{s_{NN}} = 5.02 TeV",
            Form("PbPb %s nb^{-1}, pp 650 pb^{-1}", (int)projectedLumiPBPB)
    };
    int nTextOverPadLines = textOverPadLines.size();
    for (int i = 0; i < nTextOverPadLines; ++i) {
        latex = new TLatex();
        textYOverPad = 0.96 - i*0.065;
        setLatexOverPad(i, latex);
        latex->Draw();
    }

    textXCMSProj = 0.24;
    textYCMSProj = 0.80;
    textAlignCMSProj = 11;
    textFontCMSProj = 61;
    textSizeCMSProj = 0.08;
    latex = new TLatex();
    setLatexCMSProj(latex, "CMS");
    latex->Draw();

    textXCMSProj = 0.24;
    textYCMSProj = 0.74;
    textAlignCMSProj = 11;
    textFontCMSProj = 52;
    textSizeCMSProj = 0.06;
    latex = new TLatex();
    setLatexCMSProj(latex, "Projection");
    latex->Draw();

    c->Update();

    TLine* line = new TLine(gPad->GetUxmin(), 1, gPad->GetUxmax(), 1);
    line->SetLineStyle(kDashed);
    line->Draw();

    c->SaveAs(Form("%s.pdf", c->GetName()));
    c->Close();         // do not use Delete() for TCanvas.

    std::cout<<"Closing the input file"<<std::endl;
    input->Close();

    std::cout<<"running projectionPlot_js_ratioOnly() - END"<<std::endl;
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
    h->SetStats(false);
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

void scaleBinErrors(TH1* h, double scale)
{
    int nBins = h->GetNbinsX();
    for ( int i = 0; i <= nBins+1; i++)
    {
        h->SetBinError(i, h->GetBinError(i)*scale);
    }
}

void fillTH1fromTF1(TH1* h, TF1* f, int binFirst, int binLast)
{
    for ( int i = binFirst; i <= binLast; i++)
    {
        double x = h->GetBinCenter(i);
        double y = f->Eval(x);

        double binContent = h->GetBinContent(i);
        double binError = h->GetBinError(i);

        h->SetBinContent(i, y);
        h->SetBinError(i, binError * TMath::Sqrt(y/binContent));
    }
}

void mergeUncWithErrorBar(TH1* h, TH1* hSys, bool doRelUnc)
{
    if (hSys == 0) return;

    for ( int bin = 1; bin <= h->GetNbinsX(); bin++)
    {
        double val = h->GetBinContent(bin);
        double x   = h->GetBinCenter(bin);
        int binSys = hSys->FindBin(x);

        double error = TMath::Abs(hSys->GetBinContent(binSys));             // if the uncertainty is calculated using differences
        if (doRelUnc) error = TMath::Abs(val * hSys->GetBinContent(binSys));    // if the uncertainty is calculated using ratios

        double errorBar = TMath::Sqrt(h->GetBinError(bin)*h->GetBinError(bin) + error*error);

        h->SetBinError(bin, errorBar);
    }
}

/*
 * function to set bin errors of TH1 histogram.
 * includes underflow and overflow bins, size of binErrors must be nBins + 2.
 * avoids looping over the bins in the main program.
 * helps to keep the code clean.
 */
void setBinErrors(TH1* h, std::vector<double> binErrors)
{
    int nBins = h->GetNbinsX();
    int nVec  = binErrors.size();
    if (nBins+2 != nVec)  return;

    for ( int i = 0; i <= nBins+1; ++i)
    {
        h->SetBinError(i, binErrors.at(i));
    }
}

/*
 * returns the bin errors including underflow and overflow bins
 * size of the vector is nBins+2.
 */
std::vector<double> getBinErrors(TH1* h)
{
    std::vector<double> res;
    int nBins = h->GetNbinsX();
    for ( int i = 0; i <= nBins+1; ++i)
    {
        res.push_back(h->GetBinError(i));
    }

    return res;
}

int main(int argc, char** argv)
{
    if (argc == 4) {
        vJetPlotProjection(atoi(argv[1]), argv[2], std::atof(argv[3]));
        return 0;
    }
    else if (argc == 3) {
        vJetPlotProjection(atoi(argv[1]), argv[2]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./vJetPlotProjection.exe <figureIndex> <inputFile> <sysReduction>"
                << std::endl;
        return 1;
    }
}
