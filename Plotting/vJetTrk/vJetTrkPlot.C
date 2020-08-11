/*
 * macro to produce plots for v+jet+trk analysis
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
#include <THStack.h>
#include <TLine.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TMath.h>
#include <TStyle.h>
#include <TColor.h>

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

#include "../../Utilities/interface/ConfigurationParser.h"
#include "../../Utilities/interface/InputConfigurationParser.h"
#include "../../Utilities/th1Util.h"
#include "../../Utilities/styleUtil.h"
#include "../../Utilities/systemUtil.h"
#include "../../Utilities/vJetTrkUtil.h"

enum FIGURE{
    k_M_Z,
    k_zTrk,
    kN_FIGURES
};

std::string figureNames[kN_FIGURES] = {"vJetTrk_M_Z", "vJetTrk_zTrk"};

enum OBS {
    vjt_dphi,
    vjt_xivh,
    vjt_trkPt,
    kN_OBS
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
int xTitleFont;
int yTitleFont;
float xTitleSize;
float yTitleSize;
float xTitleOffset;
float yTitleOffset;
int xLabelFont;
int yLabelFont;
float xLabelSize;
float yLabelSize;
float xLabelOffset;
float yLabelOffset;
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
float legendTextSize;
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

void vJetTrkPlot(std::string inputFileList, std::string figInfo);
void vJetTrkPlot_M_Zll(std::vector<TFile*> & inputs, std::string figInfo);
void vJetTrkPlot_zTrk(std::vector<TFile*> & inputs, std::string figInfo);
int parseFigureType(std::string figuretype);
int parseFigureObs(std::string figuretype);
std::string getTextVPt(int vPtMin, int vPtMax);
std::string getTextDphi(double dphiMin);
std::string getObsLabelTrk(std::string obsLbl, int centMin);
int getTheoryColor(std::string pathTh);
double getTheoryColorAlpha(std::string pathTh);
int getTheoryLineWidth(std::string pathTh);
int getTheoryFillStyle(std::string pathTh);
std::string getTheoryDrawOpt(std::string pathTh);
std::string getTheoryLegendLabel(std::string pathTh);
void setTH1D(int iHist, TH1D* h);
void setTGraph(int iGraph, TGraph* gr);
void setTGraphSys(int iSys, TGraph* gr);
void setTHStackfromTH1(THStack* hStk, TH1D* h);
void setCanvas(TCanvas* c);
void setLegend(TLegend* leg);
void setLatex(int iText, TLatex* latex);
void setLatexOverPad(int iText, TLatex* latex);
void setLatexCMS(TLatex* latex);
void setLatexCMSextraLabel(TLatex* latex, std::string text);

void vJetTrkPlot(std::string inputFileList, std::string figInfo)
{
    std::cout << "running vJetTrkCalc()" << std::endl;
    std::cout << "inputFiles = " << inputFileList.c_str()  << std::endl;
    std::cout << "figInfo = " << figInfo.c_str()  << std::endl;

    int figType = parseFigureType(figInfo);
    if (figType < 0) {
        std::cout << "Invalid figure type" << std::endl;
        std::cout << "Exiting." << std::endl;
        return;
    }

    std::cout << "figType = " << figType << std::endl;
    std::cout << "figureName = " << figureNames[figType].c_str() << std::endl;

    std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFileList.c_str());
    int nInputFiles = inputFiles.size();
    std::cout<<"ROOT files containing input TH1 : nFiles = " << nInputFiles << std::endl;
    std::cout<<"#####"<< std::endl;
    for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
        std::cout<<(*it).c_str()<< std::endl;
    }
    std::cout<<"##### END #####"<< std::endl;

    // TH1 objects
    TH1::SetDefaultSumw2();

    std::vector<TFile*> inputs(nInputFiles, 0);
    for (int i = 0; i < nInputFiles; ++i) {
        inputs[i] = 0;
        inputs[i] = TFile::Open(inputFiles[i].c_str(), "READ");
        if (inputs[i] == 0) {
            std::cout << "Cannot read file i : " << i << " with name " << inputFiles[i].c_str() << std::endl;
            std::cout << "Exiting." << std::endl;
            return;
        }
    }

    switch (figType) {
        case FIGURE::k_M_Z:
            vJetTrkPlot_M_Zll(inputs, figInfo);
            break;
        case FIGURE::k_zTrk:
            vJetTrkPlot_zTrk(inputs, figInfo);
            break;
        default:
            break;
    }


    std::cout << "Closing the input files." << std::endl;
    for (int i = 0; i < nInputFiles; ++i) {
        inputs[i]->Close();
    }
    std::cout<<"vJetTrkPlot() - END"<<std::endl;
}

void vJetTrkPlot_M_Zll(std::vector<TFile*> & inputs, std::string figInfo)
{
    std::cout<<"running vJetTrkPlot_M_Zll()"<<std::endl;

    int nInputs = inputs.size();
    if (nInputs != 3) {
        std::cout << "There must be 3 input files for this figure." << std::endl;
        std::cout << "Exiting." << std::endl;
        return;
    }

    bool isMC = isMCsample(figInfo);
    bool isPbPb = isPbPbsample(figInfo);
    bool isPP = isPPsample(figInfo);
    bool isRatio = (toLowerCase(figInfo).find("_ratio") != std::string::npos);
    bool isPull = (toLowerCase(figInfo).find("_pull") != std::string::npos);
    bool isCompare = isRatio || isPull;

    std::cout << "isMC = " << isMC << std::endl;
    std::cout << "isPbPb = " << isPbPb << std::endl;
    std::cout << "isPP = " << isPP << std::endl;
    std::cout << "isRatio = " << isRatio << std::endl;
    std::cout << "isPull = " << isPull << std::endl;

    bool vIsZee = (toLowerCase(figInfo).find("zee") != std::string::npos);

    ////////////////
    windowWidth = 800;
    windowHeight = 800;

    int rows = (isCompare) ? 1 : 1;
    int columns = 1;

    logX = 0;
    logY = 0;
    leftMargin   = 0.17;
    rightMargin  = 0.05;
    bottomMargin = 0.15;
    topMargin    = 0.05;

    float yMargin = 0;
    float xMargin = 0;
    double frameH = 1-bottomMargin-topMargin-yMargin;
    double frameW = 1-leftMargin-rightMargin-xMargin;
    double normCanvasHeight = calcNormCanvasHeight(rows, frameH, bottomMargin, topMargin, yMargin);
    double normCanvasWidth = calcNormCanvasWidth(columns, frameW, leftMargin, rightMargin, xMargin);

    TCanvas* c = 0 ;

    std::string figNameFinal = Form("%s_%s", figureNames[FIGURE::k_M_Z].c_str(), figInfo.c_str());
    c = new TCanvas(figNameFinal.c_str(), "", windowWidth, windowHeight);
    std::cout<<"preparing canvas : "<< c->GetName() <<std::endl;
    c->SetCanvasSize(windowWidth*normCanvasWidth, windowHeight*normCanvasHeight);
    c->SetLeftMargin(leftMargin);
    c->SetRightMargin(rightMargin);
    c->SetBottomMargin(bottomMargin);
    c->SetTopMargin(topMargin);

    TPad* pads[2*columns];
    TPad* pads2[2*columns];
    //divideCanvas(c, pads, rows, columns, leftMargin, rightMargin, bottomMargin, topMargin, xMargin, yMargin, 0);
    divideCanvas(c, pads, rows, columns, leftMargin, rightMargin, bottomMargin, topMargin, xMargin, yMargin, frameW, frameH);
    setCanvas(c);
    c->cd();

    float yMinOffset = 0;
    double windowHeightScale = 1;

    if (isCompare) {
        float windowHeightFraction = 0.35;
        windowHeightScale = 1 + windowHeightFraction;
        c->SetCanvasSize(c->GetWw(), c->GetWh()*windowHeightScale);

        yMinOffset = windowHeightFraction;
        yMargin = 0.05;
    }

    divideCanvas(c, pads, 1, columns, leftMargin, rightMargin, bottomMargin, topMargin, xMargin, yMargin, frameW, frameH, yMinOffset);

    for (int i = 0; i < rows; ++i) {
        pads[i]->SetBorderMode(0);
        pads[i]->SetBorderSize(0);
        pads[i]->SetFrameBorderMode(0);
        pads[i]->SetFrameLineColor(0);

        // put ticks to upper and right part of the axis.
        pads[i]->SetTickx(1);
        pads[i]->SetTicky(1);


        c->cd();

        std::string padNameTmp = Form("%s_lower", pads[i]->GetName());
        double x1_lowerPad = pads[i]->GetXlowNDC();
        double y1_lowerPad = pads[i]->GetYlowNDC();
        if (i + columns < rows*columns)  y1_lowerPad = pads[i+columns]->GetYlowNDC();
        else                         y1_lowerPad = 0;
        double x2_lowerPad = pads[i]->GetXlowNDC()+pads[i]->GetWNDC();
        double y2_lowerPad = pads[i]->GetYlowNDC();

        pads2[i] = new TPad(padNameTmp.c_str(), "", x1_lowerPad, y1_lowerPad, x2_lowerPad, y2_lowerPad);
        pads2[i]->SetLeftMargin(pads[i]->GetLeftMargin());
        pads2[i]->SetRightMargin(pads[i]->GetRightMargin());
        pads2[i]->SetBottomMargin(yMargin * (pads[i]->GetAbsHNDC() / pads2[i]->GetAbsHNDC()));
        pads2[i]->SetTopMargin(0);

        setPadFinal(pads2[i], pads[i]->GetLogx(), 0);  // do not draw the y-axis in log scale for the ratio histogram.

        pads2[i]->Draw();
        pads2[i]->cd();
        pads2[i]->SetNumber(rows*columns+i+1);
    }

    c->cd(1);
    /////////////////

    xTitle = (vIsZee) ? "M^{ee} (GeV/c^{2})": "M^{#mu#mu} (GeV/c^{2})";
    yTitle = "Entries / (2 GeV/c^{2})";
    xTitleFont = 42;
    yTitleFont = 42;
    xTitleSize = 0.042;
    yTitleSize = 0.042;
    xTitleOffset = 1.25;
    yTitleOffset = 1.7;

    xLabelFont = 42;
    yLabelFont = 42;
    xLabelSize = 0.035;
    yLabelSize = 0.035;
    xLabelOffset = 0.005;
    yLabelOffset = 0.005;

    yMin = 0;
    yMax = 0;

    enum HISTLABELS {
        k_mc,
        k_data,
        k_data_sameCh,
        kN_HISTLABELS
    };

    int vPtMin = parseVPtMin(figInfo);
    int vPtMax = parseVPtMax(figInfo);
    std::string strVPt = Form("vPt%d_%d", vPtMin, vPtMax);
    std::string strCent = (isPP) ? "cent0_100" : "cent0_90";
    //std::string strCent = (isPP) ? "cent0_100" : "cent0_100";
    histPaths = {
            Form("h_vM_os_%s_%s", strVPt.c_str(), strCent.c_str()),
            Form("h_vM_os_%s_%s", strVPt.c_str(), strCent.c_str()),
            Form("h_vM_ss_%s_%s", strVPt.c_str(), strCent.c_str()),
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

        h1Ds[i] = (TH1D*)inputs[i]->Get(histPaths[i].c_str());
        setTH1D(i, h1Ds[i]);

        h1Ds[i]->Draw(drawOptions[i].c_str());
    }

    int binMax = h1Ds[k_data]->GetMaximumBin();
    yMax = h1Ds[k_data]->GetBinContent(binMax) * 1.4;

    double intMC = h1Ds[k_mc]->Integral();
    double intData = h1Ds[k_data]->Integral();
    h1Ds[k_mc]->Scale(intData / intMC);

    for (int i = 0; i < nHistPaths; ++i) {
        h1Ds[i]->SetMinimum(yMin);
        h1Ds[i]->SetMaximum(yMax);
        h1Ds[i]->Draw(drawOptions[i].c_str());
    }

    c->Update();

    TLine* line = 0;
    if (isCompare) {

        std::string compareName = (isPull) ? "pull" : "ratio";

        std::string tmpName = Form("%s_%s_mc", h1Ds[k_data]->GetName(), compareName.c_str());
        if (isRatio) {
            hTmp = (TH1D*)h1Ds[k_data]->Clone(tmpName.c_str());
            hTmp->Divide(h1Ds[k_mc]);
        }
        else if (isPull) {
            hTmp = (TH1D*)getResidualHistogram(h1Ds[k_data], h1Ds[k_mc], false);
            hTmp->Divide(h1Ds[k_data]);
            hTmp->SetName(tmpName.c_str());
        }

        setTH1D(k_data, hTmp);

        double lineHorizY = -1;
        if (isRatio) {
            hTmp->SetYTitle("Data / MC");
            hTmp->SetMinimum(0.5);
            hTmp->SetMaximum(1.4999);
            lineHorizY = 1;
        }
        else if (isPull) {
            hTmp->SetYTitle("#frac{Data - MC}{Data}");
            hTmp->SetMinimum(-0.3);
            hTmp->SetMaximum(0.3);
            lineHorizY = 0;
        }

        double axisSizeRatio = (c->GetPad(1)->GetAbsHNDC()/c->GetPad(2)->GetAbsHNDC());
        setTH1Ratio(hTmp, h1Ds[k_data], axisSizeRatio);

        c->cd(2);
        hTmp->Draw("e");

        c->Update();
        line = new TLine(gPad->GetUxmin(), lineHorizY, gPad->GetUxmax(), lineHorizY);
        line->SetLineStyle(kDashed);
        line->Draw();


        c->cd(1);
    }

    std::vector<int> pairCounts(nHistPaths, 0);
    for (int i = 0; i < nHistPaths; ++i) {

        double massMin_count = 60;
        double massMax_count = 120;
        int binLow_count = -1;
        int binUp_count = -1;

        binLow_count = h1Ds[i]->FindBin(massMin_count);
        binUp_count = h1Ds[i]->FindBin(massMax_count) - 1;

        pairCounts[i] = (int)h1Ds[i]->Integral(binLow_count, binUp_count);
    }

    std::cout << Form("Opposite sign (%d counts)", pairCounts[k_data]) << std::endl;
    std::cout << Form("Same sign (%d counts)", pairCounts[k_data_sameCh]) << std::endl;

    legendX1 = 0.21;
    legendY1 = 0.76;
    legendWidth = 0.45;
    legendHeight = 0.15;
    legendMargin = 0.15;
    legendTextSize = -1;
    std::string textMC = (isPP) ? "Pythia" : "Pythia+Hydjet";
    legendEntryTexts = {
            textMC.c_str(),
            //Form("Opposite sign (%d counts)", pairCounts[k_data]),
            //Form("Same sign (%d counts)", pairCounts[k_data_sameCh])
            "Opposite sign",
            "Same sign"
    };

    legendEntryOptions = {
            "lf",
            "lpf",
            "lpf"
    };
    TLegend* leg = new TLegend();

    hTmp = (TH1D*)h1Ds[k_data]->Clone(Form("%s_tmp", h1Ds[k_data]->GetName()));
    leg->AddEntry(hTmp, legendEntryTexts[k_data].c_str(), legendEntryOptions[k_data].c_str());
    hTmp = (TH1D*)h1Ds[k_data_sameCh]->Clone(Form("%s_tmp", h1Ds[k_data_sameCh]->GetName()));
    leg->AddEntry(hTmp, legendEntryTexts[k_data_sameCh].c_str(), legendEntryOptions[k_data_sameCh].c_str());
    hTmp = (TH1D*)h1Ds[k_mc]->Clone(Form("%s_tmp", h1Ds[k_mc]->GetName()));
    leg->AddEntry(hTmp, legendEntryTexts[k_mc].c_str(), legendEntryOptions[k_mc].c_str());

    setLegend(leg);
    leg->Draw();

    textX = 0.21;
    textYs = {0.69, 0.64};
    textAlign = 11;
    textFont = 43;
    textSize = 30;
    std::string textL = (vIsZee) ? "e" : "#mu";
    //std::string textLeta = (vIsZee) ? "2.1" : "2.4";
    textLines = {
            //Form("p^{%s#pm}_{T} > 20 GeV/c", textL.c_str()),
            //Form("|#eta^{%s#pm}| < %s", textL.c_str(), textLeta.c_str())
    };
    int nTextLines = textLines.size();
    TLatex* latex = 0;
    for (int i = 0; i < nTextLines; ++i) {
        latex = new TLatex();
        setLatex(i, latex);
        latex->Draw();
    }

    //textX = 0.69;
    //textYs = {0.71, 0.66, 0.61};
    textX = 0.21;
    textYs = {0.69, 0.63, 0.57, 0.51};
    textAlign = 11;
    textFont = 43;
    textSize = 30;
    std::string textVPt = getTextVPt(vPtMin, vPtMax);
    if (isPP) {
        textLines = {
                Form("Z #rightarrow %s%s", textL.c_str(), textL.c_str()),
                Form("p^{%s#pm}_{T} > 20 GeV/c", textL.c_str()),
                textVPt.c_str(),
        };
    }
    else {
        textLines = {
                Form("Z #rightarrow %s%s", textL.c_str(), textL.c_str()),
                "Cent: 0-90 %",
                Form("p^{%s#pm}_{T} > 20 GeV/c", textL.c_str()),
                textVPt.c_str(),
        };
    }

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
    if (isPP) {
        textOverPadLines = {
                "pp #sqrt{s} = 5.02 TeV",
                "304 pb^{-1}"
        };
    }
    else {
        textOverPadLines = {
                "PbPb #sqrt{s_{NN}} = 5.02 TeV",
                "1.7 nb^{-1}"
        };
    }
    int nTextOverPadLines = textOverPadLines.size();
    for (int i = 0; i < nTextOverPadLines; ++i) {
        latex = new TLatex();
        setLatexOverPad(i, latex);
        latex->Draw();
    }

    textXCMS = 0.64;
    textYCMS = 0.87;
    textAlignCMS = 11;
    textFontCMS = 61;
    textSizeCMS = 0.045;
    latex = new TLatex();
    setLatexCMS(latex);
    latex->Draw();

    bool isPreliminary = false; // false;
    bool isSupplementary = false;  // true;
    if (isPreliminary || isSupplementary) {
        textXCMSpreliminary = textXCMS;
        textYCMSpreliminary = textYCMS-0.05;
        textAlignCMSpreliminary = 11;
        textFontCMSpreliminary = 52;
        textSizeCMSpreliminary = textSizeCMS*1.1;
        latex = new TLatex();
        if (isPreliminary) {
            setLatexCMSextraLabel(latex, "Preliminary");
        }
        else if (isSupplementary) {
            textSizeCMSpreliminary = textSizeCMS*0.95;
            setLatexCMSextraLabel(latex, "Supplementary");
        }
        latex->Draw();
    }

    c->Update();

    c->SaveAs(Form("%s.pdf", c->GetName()));
    c->SaveAs(Form("%s.png", c->GetName()));
    c->SaveAs(Form("%s.C", c->GetName()));
    c->Close();         // do not use Delete() for TCanvas.

    std::cout<<"running vJetTrkPlot_M_Zll() - END"<<std::endl;
}

void vJetTrkPlot_zTrk(std::vector<TFile*> & inputs, std::string figInfo)
{
    std::cout<<"running vJetTrkPlot_zTrk()"<<std::endl;

    bool is_pp_vs_mc = (figInfo.find("pp_vs_mc") != std::string::npos);
    bool is_pbpb_vs_pp = !is_pp_vs_mc;

    bool is_th_scetg = (figInfo.find("th_scetg") != std::string::npos);
    bool is_th_hybrid = (figInfo.find("th_hybrid") != std::string::npos);
    bool is_th_colbt = (figInfo.find("th_colbt") != std::string::npos);
    bool is_th_1curve = (figInfo.find("th_1curve") != std::string::npos);   // select one curve from each model
    bool is_theory = (is_th_scetg || is_th_hybrid || is_th_colbt);

    int iObs = parseFigureObs(figInfo);
    if (iObs < 0) {
        std::cout << "Invalid observable." << std::endl;
        std::cout << "Exiting." << std::endl;
        return;
    }

    bool isErrBS = (figInfo.find("err_bs") != std::string::npos);
    bool isDiff = (figInfo.find("fig_diff") != std::string::npos);

    bool plotTrkPtLogY = true;

    // no horizontal error bars
    gStyle->SetErrorX(0);
    gStyle->SetHatchesLineWidth(2);
    //gStyle->SetHatchesSpacing(0.2);

    windowWidth = 800;
    windowHeight = 700;

    int rows = 2;
    int columns = 1;
    int nCents = countOccurances(figInfo, "_cent");
    columns = nCents;

    logX = 0;
    logY = 0;
    leftMargin   = 0.28;
    rightMargin  = 0.03;
    bottomMargin = 0.20;
    topMargin    = 0.08;

    float yMargin = 0;
    float xMargin = 0;
    double frameH = 1-bottomMargin-topMargin-yMargin;
    double frameW = 1-leftMargin-rightMargin-xMargin;
    double normCanvasHeight = calcNormCanvasHeight(rows, frameH, bottomMargin, topMargin, yMargin);
    double normCanvasWidth = calcNormCanvasWidth(columns, frameW, leftMargin, rightMargin, xMargin);

    TCanvas* c = 0 ;

    c = new TCanvas(Form("%s_%s", figureNames[FIGURE::k_zTrk].c_str(), figInfo.c_str()), "", windowWidth, windowHeight);
    std::cout<<"preparing canvas : "<< c->GetName() <<std::endl;
    c->SetCanvasSize(windowWidth*normCanvasWidth, windowHeight*normCanvasHeight);
    c->SetLeftMargin(leftMargin);
    c->SetRightMargin(rightMargin);
    c->SetBottomMargin(bottomMargin);
    c->SetTopMargin(topMargin);

    TPad* pads[2*columns];
    //divideCanvas(c, pads, rows, columns, leftMargin, rightMargin, bottomMargin, topMargin, xMargin, yMargin, 0);
    divideCanvas(c, pads, rows, columns, leftMargin, rightMargin, bottomMargin, topMargin, xMargin, yMargin, frameW, frameH);

    for (int i = 0; i < 2*columns; ++i) {
        pads[i]->SetBorderMode(0);
        pads[i]->SetBorderSize(0);
        pads[i]->SetFrameBorderMode(0);
        pads[i]->SetFrameLineColor(0);

        // put ticks to upper and right part of the axis.
        pads[i]->SetTickx(1);
        pads[i]->SetTicky(1);
    }

    double dphiMin = parseVTrkDPhiMin(figInfo);

    std::string obsName = "";
    std::string obslabel = "";
    xTitle = "";
    yTitle = "";
    float xMin = 0;
    float xMax = -1;
    yMin = 0;
    yMax = -1;
    if (iObs == OBS::vjt_dphi) {
        obsName = "dphi";
        obslabel = "dphi_rebin";

        xMax = TMath::Pi();

        xTitle = "#Delta#phi_{trk,Z} (rad)";
        yTitle = "#frac{1}{N_{Z}} #frac{dN_{trk,Z}}{d#Delta#phi_{trk,Z}} (1/rad)";
        yMin = -1;
        yMax = 24;
    }
    else if (iObs == OBS::vjt_xivh) {
        obsName = "xivh";
        obslabel = "xivh";

        xTitle = "#xi^{trk,Z}_{T}";
        yTitle = "#frac{1}{N_{Z}} #frac{dN_{trk,Z}}{d#xi^{trk,Z}_{T}}";

        xMax = 4.99;

        yMin = -0.2;
        yMax = 6.0;
        if (dphiMin == 0.5) {
            yMin = -0.4;
            yMax = 10;
        }
        else if (dphiMin == 0.666) {
            yMin = -0.4;
            yMax = 8;
        }
    }
    else if (iObs == OBS::vjt_trkPt) {
        obsName = "trkPt";
        obslabel = "trkPt_rebin";

        xTitle = "p^{trk}_{T} (GeV/c)";
        yTitle = "#frac{1}{N_{Z}} #frac{dN_{trk}}{dp^{trk}_{T}} (1 / GeV/c)";

        xMin = 1;
        xMax = 30;
        yMin = -0.1;
        yMax = 4;
        if (plotTrkPtLogY) {
            yMin = 0.00105;
            yMax = 6;
        }
        if (dphiMin == 0.5) {
            yMin = -0.3;
            yMax = 12;
        }
        else if (dphiMin == 0.666) {
            yMin = -0.2;
            yMax = 10;
        }
    }
    xTitleFont = 43;
    yTitleFont = 43;
    xTitleSize = 50;
    yTitleSize = 50;
    xTitleOffset = 2.2;
    yTitleOffset = 2.9;

    xLabelFont = 43;
    yLabelFont = 43;
    xLabelSize = 45;
    yLabelSize = 45;
    xLabelOffset = 0.006;
    yLabelOffset = 0.008;

    enum HISTLABELS {
        k_hist1,
        k_hist2,
        k_ratio,
        kN_HISTLABELS
    };

    int vPtMin = parseVPtMin(figInfo);
    int vPtMax = parseVPtMax(figInfo);
    std::string strVPt = Form("vPt%d_%d", vPtMin, vPtMax);

    std::vector<int> centMins;
    std::vector<int> centMaxs;
    std::vector<std::string> centTexts;
    std::string tmpFigInfo = figInfo;
    for (int iCent = 0; iCent < nCents; ++iCent) {

        int centMin = parseCentMin(tmpFigInfo);
        int centMax = parseCentMax(tmpFigInfo);

        centMins.push_back(centMin);
        centMaxs.push_back(centMax);

        tmpFigInfo = replaceFirst(tmpFigInfo, Form("_cent%d_%d", centMin, centMax), "");

        centTexts.push_back(Form("Cent: %d-%d%%", centMin, centMax));
    }

    std::vector<double> trkPtMins;
    std::vector<double> trkPtMaxs;
    std::string trkPtLabel;
    std::string trkPtText;
    tmpFigInfo = figInfo;
    if (iObs != vjt_trkPt) {

            double trkPtMin = parseTrkPtMin(tmpFigInfo);
            double trkPtMax = parseTrkPtMax(tmpFigInfo);

            trkPtMins.push_back(trkPtMin);
            trkPtMaxs.push_back(trkPtMax);

            std::string label_trkPtMin = Form("%d", (int)(trkPtMin));    // 5 --> "5"
            if (std::floor(trkPtMin) != trkPtMin) {   // 1.4 --> "1p4"
                label_trkPtMin = Form("%dp%d", (int)(trkPtMin), ((int)(trkPtMin*10) % 10));
            }

            std::string label_trkPtMax = Form("%d", (int)(trkPtMax));    // 5 --> "5"
            if (std::floor(trkPtMax) != trkPtMax) {   // 1.4 --> "1p4"
                label_trkPtMax = Form("%dp%d", (int)(trkPtMax), ((int)(trkPtMax*10) % 10));
            }

            std::string label_trkPt = Form("trkPt%s_%s_", label_trkPtMin.c_str(), label_trkPtMax.c_str());

            trkPtLabel = label_trkPt;

            std::string text_trkPtMin = replaceAll(label_trkPtMin, "p", ".");
            std::string text_trkPtMax = replaceAll(label_trkPtMax, "p", ".");
            std::string text_trkPt = "p^{trk}_{T}";
            std::string text_range_trkPt = Form("%s < %s < %s GeV/c", text_trkPtMin.c_str(), text_trkPt.c_str(), text_trkPtMax.c_str());
            if (trkPtMax < trkPtMin) {
                text_range_trkPt = Form("%s > %s GeV/c", text_trkPt.c_str(), text_trkPtMin.c_str());
            }

            trkPtText = text_range_trkPt;
    }

    std::vector<std::string> graphPathsTh;

    std::string ratiodiffLbl = (isDiff) ? "_diff" : "_ratio";
    std::string tmpTrkPtLbl = (iObs == vjt_trkPt) ? "" : trkPtLabel;
    if (is_pbpb_vs_pp) {

        histPaths.clear();

        for (int iC = 0; iC < nCents; ++iC) {

            std::string centlabel = Form("cent%d_%d", centMins[iC], centMaxs[iC]);
            std::string obslabelTmp = obslabel;
            if (iObs == vjt_trkPt) {
                obslabelTmp = getObsLabelTrk(obslabel, centMins[iC]);
            }

            std::string obslabelTmpPbPb = obslabelTmp;
            if (isErrBS) {
                obslabelTmpPbPb = Form("err_bs_%s", obslabelTmp.c_str());
            }

            histPaths.push_back(Form("h_%s_%s_%s%s_sig", obslabelTmpPbPb.c_str(), strVPt.c_str(), tmpTrkPtLbl.c_str(), centlabel.c_str()));
            histPaths.push_back(Form("h_%s_%s_%scent0_100_sig", obslabelTmp.c_str(), strVPt.c_str(), tmpTrkPtLbl.c_str()));
            histPaths.push_back(Form("h%s_%s_%s_%s%s_sig", ratiodiffLbl.c_str(), obslabelTmpPbPb.c_str(), strVPt.c_str(), tmpTrkPtLbl.c_str(), centlabel.c_str()));

            if (is_theory) {

                if (centMaxs[iC] > 50)  continue;

                std::string str_diff_ratio = isDiff ? "diff" : "ratio";

                if (is_th_scetg) {

                    if (is_th_1curve) {

                        if (centMaxs[iC] > 30)  continue;
                        graphPathsTh.push_back(Form("gr_scetg_g2p0_ratio_%s_%s", obsName.c_str(), centlabel.c_str()));
                    }
                    else {
                        graphPathsTh.push_back(Form("gr_scetg_g1p8_ratio_%s_%s", obsName.c_str(), centlabel.c_str()));
                        graphPathsTh.push_back(Form("gr_scetg_g2p0_ratio_%s_%s", obsName.c_str(), centlabel.c_str()));
                        graphPathsTh.push_back(Form("gr_scetg_g2p2_ratio_%s_%s", obsName.c_str(), centlabel.c_str()));
                    }
                }

                if (is_th_hybrid) {

                    if (is_th_1curve) {

                        if (centMaxs[iC] > 30)  continue;
                        graphPathsTh.push_back(Form("gr_hybrid_wake_full_%s_%s_%s", str_diff_ratio.c_str(), obsName.c_str(), centlabel.c_str()));
                    }
                    else {
                        graphPathsTh.push_back(Form("gr_hybrid_wake_no_%s_%s_%s", str_diff_ratio.c_str(), obsName.c_str(), centlabel.c_str()));
                        graphPathsTh.push_back(Form("gr_hybrid_wake_pos_%s_%s_%s", str_diff_ratio.c_str(), obsName.c_str(), centlabel.c_str()));
                        graphPathsTh.push_back(Form("gr_hybrid_wake_full_%s_%s_%s", str_diff_ratio.c_str(), obsName.c_str(), centlabel.c_str()));
                    }
                }

                if (is_th_colbt) {

                    if (is_th_1curve) {
                        if (centMaxs[iC] > 30)  continue;
                    }
                    graphPathsTh.push_back(Form("gr_colbt_%s_%s_%s", str_diff_ratio.c_str(), obsName.c_str(), centlabel.c_str()));
                }
            }
        }
    }
    else if (is_pp_vs_mc) {

        histPaths = {
                Form("h_%s_%s_%scent0_100_sig", obslabel.c_str(), strVPt.c_str(), tmpTrkPtLbl.c_str()),
                Form("h_%s_%s_%scent0_100_sig", obslabel.c_str(), strVPt.c_str(), tmpTrkPtLbl.c_str()),
                Form("h%s_%s_%s_%scent0_100_sig", ratiodiffLbl.c_str(), obslabel.c_str(), strVPt.c_str(), tmpTrkPtLbl.c_str()),
        };
    }

    markerColors = {kBlack, kBlack, kBlack};
    markerSizes.assign(kN_HISTLABELS, 1.80);
    lineTransparencies.assign(kN_HISTLABELS, 1.0);
    lineWidths.assign(kN_HISTLABELS, 3);
    if (columns == 3) {
        markerSizes.assign(kN_HISTLABELS, 2.00);
        lineWidths.assign(kN_HISTLABELS, 2);
    }
    else if (columns == 4) {
        markerSizes.assign(kN_HISTLABELS, 2.20);
        lineWidths.assign(kN_HISTLABELS, 1);
    }

    std::string ratiodiffLblSys = (isDiff) ? "" : "_ratio";
    //std::string ratiodiffLblSys = (isDiff) ? "_diff" : "_ratio";
    if (is_pbpb_vs_pp) {
        markerStyles = {kFullCircle, kOpenCircle, kFullSquare};
        fillColors = {TColor::GetColor("#ef5253"), TColor::GetColor("#6699cc"), TColor::GetColor("#a09f93")};
        fillTransparencies.assign(kN_HISTLABELS, 0.7);
        lineColors.assign(kN_HISTLABELS, kBlack);
        drawOptions = {"e same", "e same", "e same"};

        sysPaths.clear();

        sysUseRelUnc.clear();

        for (int iC = 0; iC < nCents; ++iC) {

            std::string centlabel = Form("cent%d_%d", centMins[iC], centMaxs[iC]);
            std::string obslabelTmp = obslabel;
            if (iObs == vjt_trkPt) {
                obslabelTmp = getObsLabelTrk(obslabel, centMins[iC]);
            }

            sysPaths.push_back(Form("h_%s_%s_%s%s_sig_systematics", obslabelTmp.c_str(), strVPt.c_str(), tmpTrkPtLbl.c_str(), centlabel.c_str()));
            sysPaths.push_back(Form("h_%s_%s_%scent0_100_sig_systematics", obslabelTmp.c_str(), strVPt.c_str(), tmpTrkPtLbl.c_str()));
            sysPaths.push_back(Form("h%s_%s_%s_%s%s_sig_systematics", ratiodiffLblSys.c_str(), obslabelTmp.c_str(), strVPt.c_str(), tmpTrkPtLbl.c_str(), centlabel.c_str()));
        }

        sysUseRelUnc.assign(kN_HISTLABELS, false);
        sysColors = {TColor::GetColor("#ef5253"), TColor::GetColor("#6699cc"), TColor::GetColor("#a09f93")};
        sysTransparencies.assign(kN_HISTLABELS, 0.7);
        sysFillStyles.assign(kN_HISTLABELS, 1001);
    }
    else if (is_pp_vs_mc) {
        markerStyles = {0, kFullCircle, kFullSquare};
        fillColors = {0, TColor::GetColor("#6699cc"), TColor::GetColor("#299617")};
        fillTransparencies = {0, 0.7, 0.7};
        lineColors = {kViolet, kBlack, kBlack};
        drawOptions = {"hist same", "e same", "e same"};
        sysPaths = {
                Form("NULL"),
                Form("h_%s_%s_%scent0_100_sig_systematics", obslabel.c_str(), strVPt.c_str(), tmpTrkPtLbl.c_str()),
                Form("h%s_%s_%s_%scent0_100_sig_systematics", ratiodiffLblSys.c_str(), obslabel.c_str(), strVPt.c_str(), tmpTrkPtLbl.c_str()),
        };
        sysUseRelUnc.assign(kN_HISTLABELS, false);
        sysColors = fillColors;
        sysTransparencies = fillTransparencies;
        sysFillStyles = {0, 1001, 1001};
    }

    int nHistPaths = histPaths.size();
    std::vector<TH1D*> h1Ds(nHistPaths, 0);
    std::vector<TH1D*> h1DsSys(nHistPaths, 0);
    TGraph* gr = 0;
    TH1D* hTmp = 0;

    TPad* emptyBox = 0;

    for (int i = 0; i < nHistPaths; ++i) {

        int j = (i % kN_HISTLABELS);

        h1Ds[i] = (TH1D*)inputs[j]->Get(histPaths[i].c_str());
        setTH1D(j, h1Ds[i]);

        // set x-axis range
        if (xMax > xMin) {
            //h1Ds[i]->SetAxisRange(xMin + 0.00001, xMax - 0.00001, "X");
            h1Ds[i]->GetXaxis()->SetRangeUser(xMin + 0.00001, xMax - 0.00001);  // (xMin + 0.00001, xMax - 0.00001, "X");
        }

        //h1Ds[i]->SetNdivisions(510, "X");
        //h1Ds[i]->GetXaxis()->SetTicks();
    }

    int nGraphPathsTh = graphPathsTh.size();
    std::vector<TGraph*> graphTh(nGraphPathsTh, 0);

    for (int i = 0; i < nGraphPathsTh; ++i) {
        graphTh[i] = (TGraph*)inputs[0]->Get(graphPathsTh[i].c_str());      // assume theory data is in the first file
    }

    std::string ratioTitleY = "";
    if (is_pbpb_vs_pp) {
        ratioTitleY = "PbPb / pp";
        if (isDiff) {
            ratioTitleY = "PbPb - pp";
        }
    }
    else if (is_pp_vs_mc) {
        ratioTitleY = "MC / Data";
        if (isDiff) {
            ratioTitleY = "MC - Data";
        }
    }

    for (int i = 0; i < nHistPaths; ++i) {

        if (i % 3 == k_ratio) {

            h1Ds[i]->SetYTitle(ratioTitleY.c_str());
            if (is_pp_vs_mc) {
                h1Ds[i]->SetMinimum(0.7);
                h1Ds[i]->SetMaximum(1.3);
            }
            else if (iObs == vjt_dphi) {
                h1Ds[i]->SetMinimum(0.1);
                h1Ds[i]->SetMaximum(3.6);

                if (isDiff) {
                    h1Ds[i]->SetMinimum(-4.4);
                    h1Ds[i]->SetMaximum(7.999);
                }
            }
            else if (iObs == vjt_xivh) {
                h1Ds[i]->SetMinimum(0.1);
                h1Ds[i]->SetMaximum(3.2);
            }
            else if (iObs == vjt_trkPt) {
                h1Ds[i]->SetMinimum(0.2);
                h1Ds[i]->SetMaximum(1.9);
            }
        }
    }

    // draw central values, stat unc, and sys unc
    for (int i = 0; i < nHistPaths; ++i) {

        int j = (i % kN_HISTLABELS);
        int iCol = (i / kN_HISTLABELS);

        c->cd(iCol+1);
        if (plotTrkPtLogY && iObs == vjt_trkPt && j != k_ratio) {
            pads[iCol]->SetLogy(1);
        }

        if (j == k_ratio) c->cd(columns+iCol+1);

        // central values and stat unc
        if (j == 0 || (j == k_ratio)) {
            hTmp = (TH1D*)h1Ds[i]->Clone(Form("%s_tmpDraw", h1Ds[i]->GetName()));
            std::string tmpDrawOpt = replaceAll(drawOptions[j], "same", "");
            hTmp->Draw(tmpDrawOpt.c_str());
        }

        // sys unc
        h1DsSys[i] = (TH1D*)inputs[j+kN_HISTLABELS]->Get(sysPaths[i].c_str());
        if (h1DsSys[i] != 0) {
            gr = new TGraph();
            setTGraphSys(j, gr);
            drawSysUncBoxes(gr, h1Ds[i], h1DsSys[i], sysUseRelUnc[j]);
        }

        if (is_theory) {

            // retrieve the centrality for the current panel
            int panelCentMin = parseCentMin(h1Ds[i]->GetName());
            int panelCentMax = parseCentMax(h1Ds[i]->GetName());

            for (int iTh = 0; iTh < nGraphPathsTh; ++iTh) {

                int thCentMin = parseCentMin(graphTh[iTh]->GetName());
                int thCentMax = parseCentMax(graphTh[iTh]->GetName());

                if ( !((panelCentMin == thCentMin) && (panelCentMax == thCentMax)) ) continue;

                bool is_gr_th_scetg = hasSubstr(graphTh[iTh]->GetName(), "scetg");
                bool is_gr_th_hybrid = hasSubstr(graphTh[iTh]->GetName(), "hybrid");
                bool is_gr_th_colbt = hasSubstr(graphTh[iTh]->GetName(), "colbt");

                int colorTh = getTheoryColor(graphTh[iTh]->GetName());
                double clrThAlpha = getTheoryColorAlpha(graphTh[iTh]->GetName());
                int thLineW = getTheoryLineWidth(graphTh[iTh]->GetName());
                int fillStyleTh = getTheoryFillStyle(graphTh[iTh]->GetName());
                std::string thDrawOpt = getTheoryDrawOpt(graphTh[iTh]->GetName());

                graphTh[iTh]->SetFillColorAlpha(colorTh, clrThAlpha);
                graphTh[iTh]->SetLineColorAlpha(colorTh, clrThAlpha);
                graphTh[iTh]->SetLineWidth(thLineW);
                graphTh[iTh]->SetFillStyle(fillStyleTh);
                graphTh[iTh]->SetDrawOption(thDrawOpt.c_str());

                if (j == k_ratio) {

                    if ((is_th_scetg && is_gr_th_scetg) ||
                        (is_th_hybrid && is_gr_th_hybrid) ||
                        (is_th_colbt && is_gr_th_colbt)
                        ) {

                        graphTh[iTh]->Draw(Form("same %s", thDrawOpt.c_str()));
                    }

                }
            }
        }

        h1Ds[i]->Draw(drawOptions[j].c_str());

        // cosmetics on x-axis labels that are cut-off at panel edges
        if (j == k_ratio && iCol > 0) {

            double xWidth = 0.04;
            double xStart = 0.0;

            double yWidth = 0.07;
            double yStart = bottomMargin + 0.005;
            emptyBox = new TPad("box1", "", c->GetXlowNDC() + xStart,          c->GetYlowNDC() + yStart - yWidth,
                                            c->GetXlowNDC() + xStart + xWidth, c->GetYlowNDC() + yStart);
            //emptyBox->SetFillColor(kRed);
            emptyBox->SetFillColor(kWhite);
            emptyBox->Draw();
        }
        if (j == k_ratio && iCol < columns - 1) {

            if (iObs == vjt_xivh || iObs == vjt_trkPt) {

                double xWidth = 0.04;
                if (iObs == vjt_xivh && iCol == 0) {
                    xWidth = 0.03;
                }
                double xStart = 1.0;

                double yWidth = 0.07;
                double yStart = bottomMargin + 0.005;
                emptyBox = new TPad("box1", "", c->GetXlowNDC() + xStart - xWidth,          c->GetYlowNDC() + yStart - yWidth,
                                                c->GetXlowNDC() + xStart, c->GetYlowNDC() + yStart);
                //emptyBox->SetFillColor(kBlue);
                emptyBox->SetFillColor(kWhite);
                emptyBox->Draw();
            }
        }
    }

    c->cd(1);

    TLegend* leg = 0;

    TLegend* legTh = 0;
    int col_legTh_scetg_drawn = -1;
    int col_legTh_hybrid_drawn = -1;
    int col_legTh_colbt_drawn = -1;

    TLatex* latex = 0;
    TLatex* latex2 = 0;
    TLine* line = 0;
    for (int iCol = 0; iCol < columns; ++iCol) {

        c->cd(iCol+1);

        int iHist1 = (iCol*kN_HISTLABELS)+k_hist1;
        int iHist2 = (iCol*kN_HISTLABELS)+k_hist2;

        if (is_pbpb_vs_pp) {
            if (columns == 1) {
                legendEntryTexts = {
                        Form("PbPb %s", centTexts[iCol].c_str()),
                        "pp"
                };
            }
            else {
                legendEntryTexts = {
                        "PbPb",
                        "pp"
                };
            }
            legendEntryOptions = {
                    "pf",
                    "pf"
            };
        }
        else if (is_pp_vs_mc) {
            legendEntryTexts = {
                    "Madgraph",
                    "pp"
            };
            legendEntryOptions = {
                    "l",
                    "pf"
            };
        }

        legendHeight = 0.18;
        legendMargin = 0.15;
        legendWidth = 0.7;
        legendX1 = 0.04;
        legendY1 = 1.0 - gPad->GetTopMargin() - legendHeight - 0.02;
        legendTextSize = 0.08;
        if (iObs == vjt_dphi) {
            legendX1 = 0.04;
        }
        else if (iObs == vjt_xivh) {
            legendX1 = 0.04;
        }
        else if (iObs == vjt_trkPt) {
            legendX1 = 0.22;
            if (columns > 1) {
                legendX1 -= 0.1;
            }
        }
        legendX1 += (iCol == 0) ? c->GetLeftMargin() : xMargin;

        if (iCol == 0) {
            leg = new TLegend();
            hTmp = (TH1D*)h1Ds[iHist1]->Clone(Form("%s_tmp", h1Ds[iHist1]->GetName()));
            if (legendEntryOptions[k_hist1] == "pf") {
                hTmp->SetLineWidth(0);
            }
            leg->AddEntry(hTmp, legendEntryTexts[k_hist1].c_str(), legendEntryOptions[k_hist1].c_str());
            hTmp = (TH1D*)h1Ds[iHist2]->Clone(Form("%s_tmp", h1Ds[iHist2]->GetName()));
            if (legendEntryOptions[k_hist2] == "pf") {
                hTmp->SetLineWidth(0);
            }
            leg->AddEntry(hTmp, legendEntryTexts[k_hist2].c_str(), legendEntryOptions[k_hist2].c_str());

            setLegend(leg);
            leg->Draw();
        }

        c->cd(iCol+1);

        textAlign = 11;
        textFont = 43;
        textSize = 34 + (columns*2);
        std::string textVPt = getTextVPt(vPtMin, vPtMax);
        textLines = {
                textVPt,
        };
        if (iObs != vjt_trkPt) {
            textLines.push_back(trkPtText);
        }
        std::string textDphi = getTextDphi(dphiMin);
        if (iObs == vjt_xivh || iObs == vjt_trkPt) {
            textLines.push_back(textDphi);
        }
        int nTextLines = textLines.size();

        textX = legendX1 + 0.02;
        float tmpTextY1 = legendY1 - 0.08;

        if (iObs == vjt_trkPt) {
            textX = 0.04 + 0.02;
            textX += (iCol == 0) ? c->GetLeftMargin() : xMargin ;
            tmpTextY1 = 0.2;
        }

        if (iObs == vjt_dphi) {
            textYs.clear();
            textYs.resize(nTextLines, tmpTextY1);
        }
        else if (iObs == vjt_xivh) {
            textYs.clear();
            textYs.resize(nTextLines, tmpTextY1);
        }
        else if (iObs == vjt_trkPt) {
            textYs.clear();
            textYs.resize(nTextLines, tmpTextY1);
        }

        if (iCol == 0) {
            latex = 0;
            for (int i = 0; i < nTextLines; ++i) {
                latex = new TLatex();
                textYs[i] = textYs[0] - i*0.10;
                setLatex(i, latex);
                latex->Draw();
            }
        }

        if (is_theory) {

            // retrieve the centrality for the current panel
            int panelCentMin = parseCentMin(h1Ds[iHist1]->GetName());
            int panelCentMax = parseCentMax(h1Ds[iHist1]->GetName());

            if (is_th_1curve &&
                (col_legTh_scetg_drawn < 0 && col_legTh_hybrid_drawn < 0  && col_legTh_colbt_drawn < 0)) {
                legTh = new TLegend();
            }

            if (is_th_scetg) {

                if (col_legTh_scetg_drawn < 0) {

                    c->cd(columns+iCol+1);

                    std::string legHeaderOrEntry = "SCET_{G}";
                    if (!is_th_1curve) {
                        legTh = new TLegend();
                        legTh->SetHeader(legHeaderOrEntry.c_str());
                    }
                    else {
                        legHeaderOrEntry = "SCET_{G}, g = 2.0";
                    }

                    for (int iTh = 0; iTh < nGraphPathsTh; ++iTh) {

                        if (! hasSubstr(graphTh[iTh]->GetName(), "scetg")) continue;

                        int thCentMin = parseCentMin(graphTh[iTh]->GetName());
                        int thCentMax = parseCentMax(graphTh[iTh]->GetName());

                        if ( !((panelCentMin == thCentMin) && (panelCentMax == thCentMax)) ) continue;

                        std::string lblTh = (!is_th_1curve) ? getTheoryLegendLabel(graphTh[iTh]->GetName()) : legHeaderOrEntry;

                        gr = (TGraph*)graphTh[iTh]->Clone(Form("%s_tmp", graphTh[iTh]->GetName()));
                        legTh->AddEntry(gr, lblTh.c_str(), graphTh[iTh]->GetDrawOption());
                    }

                    if (!is_th_1curve) {
                        legendHeight = 0.24;
                        setLegend(legTh);
                        legTh->SetTextSize(0.048);
                        legTh->Draw();

                        col_legTh_scetg_drawn = iCol;
                    }
                }
            }
            if (is_th_hybrid) {

                if ((col_legTh_hybrid_drawn < 0 && iCol > col_legTh_scetg_drawn) &&
                    ((!isDphi(obslabel) && iCol >= 0) || (isDphi(obslabel) && iCol >= 1) ))
                {

                    c->cd(columns+iCol+1);

                    std::string legHeaderOrEntry = "Hybrid";
                    if (!is_th_1curve) {
                        legTh = new TLegend();
                        legTh->SetHeader(legHeaderOrEntry.c_str());
                    }
                    else {
                        legHeaderOrEntry = "Hybrid, w/ wake";
                    }

                    for (int iTh = 0; iTh < nGraphPathsTh; ++iTh) {

                        if (! hasSubstr(graphTh[iTh]->GetName(), "hybrid")) continue;

                        int thCentMin = parseCentMin(graphTh[iTh]->GetName());
                        int thCentMax = parseCentMax(graphTh[iTh]->GetName());

                        if ( !((panelCentMin == thCentMin) && (panelCentMax == thCentMax)) ) continue;

                        std::string lblTh = (!is_th_1curve) ? getTheoryLegendLabel(graphTh[iTh]->GetName()) : legHeaderOrEntry;

                        gr = (TGraph*)graphTh[iTh]->Clone(Form("%s_tmp", graphTh[iTh]->GetName()));
                        legTh->AddEntry(gr, lblTh.c_str(), "f");
                    }

                    if (!is_th_1curve) {
                        legendHeight = 0.24;
                        setLegend(legTh);
                        legTh->SetTextSize(0.048);
                        legTh->Draw();

                        col_legTh_hybrid_drawn = iCol;
                    }
                }
            }
            if (is_th_colbt) {

                if (col_legTh_colbt_drawn < 0)
                {

                    c->cd(columns+iCol+1);

                    std::string legHeaderOrEntry = "CoLBT";
                    if (!is_th_1curve) {
                        legTh = new TLegend();
                    }

                    for (int iTh = 0; iTh < nGraphPathsTh; ++iTh) {

                        if (! hasSubstr(graphTh[iTh]->GetName(), "colbt")) continue;

                        int thCentMin = parseCentMin(graphTh[iTh]->GetName());
                        int thCentMax = parseCentMax(graphTh[iTh]->GetName());

                        if ( !((panelCentMin == thCentMin) && (panelCentMax == thCentMax)) ) continue;

                        std::string lblTh = (!is_th_1curve) ? getTheoryLegendLabel(graphTh[iTh]->GetName()) : legHeaderOrEntry;

                        gr = (TGraph*)graphTh[iTh]->Clone(Form("%s_tmp", graphTh[iTh]->GetName()));
                        legTh->AddEntry(gr, lblTh.c_str(), "f");
                    }

                    if (!is_th_1curve) {

                        legendHeight = 0.08;
                        legendWidth += 0.06;
                        if (isDphi(obslabel)) {
                            legendY1 += 0.12;
                        }
                        setLegend(legTh);
                        legTh->SetTextSize(0.052);
                        legTh->Draw();

                        col_legTh_colbt_drawn = iCol;
                    }
                }
            }

            if (is_th_1curve &&
                (col_legTh_scetg_drawn < 0 && col_legTh_hybrid_drawn < 0 && col_legTh_colbt_drawn < 0) &&
                legTh->GetNRows() > 0) {

                if (iObs == vjt_dphi) {
                    legendX1 = gPad->GetLeftMargin() + 0.04;
                }
                else if (iObs == vjt_xivh) {
                    legendX1 = gPad->GetLeftMargin() + 0.04;
                }
                else if (iObs == vjt_trkPt) {
                    legendX1 = gPad->GetLeftMargin() + 0.22;
                    if (columns > 1) {
                        legendX1 -= 0.1;
                    }
                }
                legendHeight = 0.08 * legTh->GetNRows();
                legendY1 = 1 - gPad->GetTopMargin() - 0.04 - legendHeight;

                setLegend(legTh);
                legTh->SetTextSize(legendTextSize * 0.8);
                legTh->Draw();

                col_legTh_scetg_drawn = iCol;
                col_legTh_hybrid_drawn = iCol;
                col_legTh_colbt_drawn = iCol;
            }

        }

        c->cd(iCol+1);
        c->Update();

        if (columns > 1) {

            int iCent = iCol;
            textLines = {
                centTexts[iCent].c_str(),
            };

            textAlign = 33;
            textX = 1 - gPad->GetRightMargin() - 0.04;

            nTextLines = textLines.size();
            textYs.clear();
            textYs.resize(nTextLines, 1 - gPad->GetTopMargin() - 0.04);

            latex2 = 0;
            for (int i = 0; i < nTextLines; ++i) {
                latex2 = new TLatex();
                textYs[i] = textYs[0] - i*0.08;
                setLatex(i, latex2);
                latex2->Draw();
            }
        }

        c->Update();
        c->cd();

        if (iCol == columns - 1 ) {

            textAlign = 33;

            nTextLines = textLines.size();
            textX = 1.0 - pads[iCol]->GetRightMargin() * pads[iCol]->GetWNDC();
            textYs.clear();
            textYs.resize(nTextLines, 1.0);
            latex = 0;

            if (is_pbpb_vs_pp) {
                textLines = {"#sqrt{s_{NN}} = 5.02 TeV, PbPb 1.7 nb^{-1}, pp 304 pb^{-1}"};
            }
            else if (is_pp_vs_mc) {
                textLines = {"#sqrt{s} = 5.02 TeV, pp 304 pb^{-1}"};
            }

            latex = new TLatex();
            setLatex(0, latex);
            latex->Draw();
        }

        c->Update();
        c->cd(iCol+1);
        latex = 0;
        if (iCol == 0) {

            textXCMS = pads[iCol]->GetLeftMargin();
            textYCMS = 1.0 - pads[iCol]->GetTopMargin() / 2;
            textAlignCMS = 12;
            textFontCMS = 63;   // 61
            textSizeCMS = 60;   // 0.10
            latex = new TLatex();
            setLatexCMS(latex);
            latex->Draw();

            bool isPreliminary = false; // (columns == 4 && ((iObs != vjt_dphi || isDiff)));
            bool isSupplementary = false; // (columns != 4 || ((iObs == vjt_dphi && !isDiff)));
            if (isPreliminary || isSupplementary) {
                textXCMSpreliminary = textXCMS;
                textYCMSpreliminary = textYCMS-0.05;
                textAlignCMSpreliminary = 11;
                textFontCMSpreliminary = 52;
                textSizeCMSpreliminary = textSizeCMS*1.1;
                latex = new TLatex();
                if (isPreliminary) {
                    setLatexCMSextraLabel(latex, "Preliminary");
                }
                else if (isSupplementary) {
                    textSizeCMSpreliminary = textSizeCMS*0.95;
                    setLatexCMSextraLabel(latex, "Supplementary");
                }
                latex->Draw();
            }
        }

        c->Update();

        c->cd();

        // horizontal dashed lines
        c->cd(iCol+1);
        line = new TLine(gPad->GetUxmin(), 0, gPad->GetUxmax(), 0);
        line->SetLineStyle(kDashed);
        line->Draw();

        c->cd(columns+iCol+1);
        double yLine = (!isDiff) ? 1 : 0;
        line = new TLine(gPad->GetUxmin(), yLine, gPad->GetUxmax(), yLine);
        line->SetLineStyle(kDashed);
        line->Draw();
    }

    c->cd();
    c->Update();
    c->SaveAs(Form("%s.pdf", c->GetName()));
    c->SaveAs(Form("%s.png", c->GetName()));
    c->SaveAs(Form("%s.C", c->GetName()));
    c->Close();         // do not use Delete() for TCanvas.

    std::cout<<"Closing the input file"<<std::endl;

    std::cout<<"running vJetTrkPlot_zTrk() - END"<<std::endl;
}

int parseFigureType(std::string figuretype)
{
    figuretype = trim(figuretype);
    figuretype = toLowerCase(figuretype);

    if (figuretype.find("fig_m_z") != std::string::npos) {
        return FIGURE::k_M_Z;
    }
    else if (figuretype.find("fig_dphi") != std::string::npos ||
             figuretype.find("fig_err_bs_dphi") != std::string::npos ||
             figuretype.find("fig_diff_dphi") != std::string::npos ||
             figuretype.find("fig_diff_err_bs_dphi") != std::string::npos ||
             figuretype.find("fig_xivh") != std::string::npos ||
             figuretype.find("fig_err_bs_xivh") != std::string::npos ||
             figuretype.find("fig_trkpt") != std::string::npos ||
             figuretype.find("fig_err_bs_trkpt") != std::string::npos) {
        return FIGURE::k_zTrk;
    }

    return -1;
}

int parseFigureObs(std::string figuretype)
{
    int res = -1;
    if (figuretype.find("fig_dphi") != std::string::npos ||
            figuretype.find("fig_err_bs_dphi") != std::string::npos ||
            figuretype.find("fig_diff_dphi") != std::string::npos ||
            figuretype.find("fig_diff_err_bs_dphi") != std::string::npos) {
        res = vjt_dphi;
    }
    else if (figuretype.find("fig_xivh") != std::string::npos ||
            figuretype.find("fig_err_bs_xivh") != std::string::npos) {
        res = vjt_xivh;
    }
    else if (figuretype.find("fig_trkPt") != std::string::npos ||
            figuretype.find("fig_err_bs_trkPt") != std::string::npos) {
        res = vjt_trkPt;
    }

    return res;
}

std::string getTextVPt(int vPtMin, int vPtMax)
{
    std::string res = Form("p_{T}^{Z} > %d GeV/c", vPtMin);
    if (vPtMax > 0) {
        res = Form("%d < p_{T}^{Z} < %d GeV/c", vPtMin, vPtMax);
    }

    return res;
}

std::string getTextDphi(double dphiMin)
{
    std::string res = "#Delta#phi_{trk,Z} > #frac{7#pi}{8}";
    if (dphiMin == 0.5) {
        res = "#Delta#phi_{trk,Z} > #frac{#pi}{2}";
    }
    else if (dphiMin == 0.666) {
        res = "#Delta#phi_{trk,Z} > #frac{2#pi}{3}";
    }

    return res;
}

std::string getObsLabelTrk(std::string obsLbl, int centMin)
{
    if (obsLbl.find("trkPt") != std::string::npos && centMin >= 30) {
        return "trkPt_rebin2";
    }
    else {
        return obsLbl;
    }
}

int getTheoryColor(std::string pathTh)
{
    if (pathTh.find("scetg") != std::string::npos) {

        if (pathTh.find("g1p8") != std::string::npos) {
            return TColor::GetColor("#0ECAE3");
        }
        else if (pathTh.find("g2p0") != std::string::npos) {
            //return TColor::GetColor("#138808");
            //return TColor::GetColor("#FF00FF");
            return TColor::GetColor("#139808");
        }
        else if (pathTh.find("g2p2") != std::string::npos) {
            return TColor::GetColor("#E3790E");
        }
    }
    else if (pathTh.find("hybrid") != std::string::npos) {

        if (pathTh.find("wake_no") != std::string::npos) {
            return TColor::GetColor("#0000FF");
        }
        else if (pathTh.find("wake_pos") != std::string::npos) {
            //return TColor::GetColor("#00FF00");
            //return TColor::GetColor("#00b300");
            return TColor::GetColor("#32CD32");
        }
        else if (pathTh.find("wake_full") != std::string::npos) {
            //return TColor::GetColor("#FF0000");
            return TColor::GetColor("#D19019");
        }
    }
    else if (pathTh.find("colbt") != std::string::npos) {

        return kBlue;
    }

    return -1;
}

double getTheoryColorAlpha(std::string pathTh)
{
    if (pathTh.find("scetg") != std::string::npos) {
        return 1.0;
    }
    else if (pathTh.find("hybrid") != std::string::npos) {

        return 1.0;
    }
    else if (pathTh.find("colbt") != std::string::npos) {

        return 0.7;
    }

    return 0.0;
}

int getTheoryLineWidth(std::string pathTh)
{
    if (pathTh.find("scetg") != std::string::npos) {
        return 3;
    }
    else if (pathTh.find("hybrid") != std::string::npos) {

        return 1;
    }
    else if (pathTh.find("colbt") != std::string::npos) {

        return 1;
    }

    return 0;
}

int getTheoryFillStyle(std::string pathTh)
{
    if (pathTh.find("scetg") != std::string::npos) {
        return 1001;
    }
    else if (pathTh.find("hybrid") != std::string::npos) {

        if (pathTh.find("wake_no") != std::string::npos) {
            return 3459;
        }
        else if (pathTh.find("wake_pos") != std::string::npos) {
            return 3454;
        }
        else if (pathTh.find("wake_full") != std::string::npos) {
            return 3545;
        }
    }
    else if (pathTh.find("colbt") != std::string::npos) {
        return 1001;
    }

    return 1001;
}

std::string getTheoryDrawOpt(std::string pathTh)
{
    if (pathTh.find("scetg") != std::string::npos) {

        return "l";
    }
    else if (pathTh.find("hybrid") != std::string::npos) {

        return "l f";
    }
    else if (pathTh.find("colbt") != std::string::npos) {

        return "f";
    }

    return "";
}

std::string getTheoryLegendLabel(std::string pathTh)
{
    if (pathTh.find("scetg") != std::string::npos) {

        if (pathTh.find("g1p8") != std::string::npos) {
            return "g = 1.8";
        }
        else if (pathTh.find("g2p0") != std::string::npos) {
            return "g = 2.0";
        }
        else if (pathTh.find("g2p2") != std::string::npos) {
            return "g = 2.2";
        }
    }
    else if (pathTh.find("hybrid") != std::string::npos) {

        if (pathTh.find("wake_no") != std::string::npos) {
            return "w/o wake";
        }
        else if (pathTh.find("wake_pos") != std::string::npos) {
            return "w/ wake positive only";
        }
        else if (pathTh.find("wake_full") != std::string::npos) {
            return "w/ wake";
        }
    }
    else if (pathTh.find("colbt") != std::string::npos) {

        return "CoLBT";
    }

    return "";
}

void setTH1D(int iHist, TH1D* h)
{
    h->SetTitle("");
    h->SetStats(false);
    h->SetXTitle(xTitle.c_str());
    h->SetYTitle(yTitle.c_str());
    /*
     * Default Font is 42 and sizes are set as percentages. Use 43 to set sizes as pixels
     * https://root-forum.cern.ch/t/histogram-label-sizes-for-several-pads-in-a-canvas/5234/2
     */
    h->SetTitleFont(xTitleFont, "X");
    h->SetTitleFont(yTitleFont, "Y");
    h->SetTitleSize(xTitleSize, "X");
    h->SetTitleSize(yTitleSize, "Y");
    h->SetTitleOffset(xTitleOffset, "X");
    h->SetTitleOffset(yTitleOffset, "Y");
    h->GetXaxis()->CenterTitle();
    h->GetYaxis()->CenterTitle();
    h->SetLabelFont(xLabelFont, "X");
    h->SetLabelFont(yLabelFont, "Y");
    h->SetLabelSize(xLabelSize, "X");
    h->SetLabelSize(yLabelSize, "Y");
    h->SetLabelOffset(xLabelOffset, "X");
    h->SetLabelOffset(yLabelOffset, "Y");
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

void setTHStackfromTH1(THStack* hStk, TH1D* h)
{
    //hStk->Draw();

    hStk->SetHistogram(h);

    hStk->SetMinimum(h->GetMinimum());
    hStk->SetMaximum(h->GetMaximum());

    hStk->SetTitle(h->GetTitle());
    hStk->GetXaxis()->SetTitle(h->GetXaxis()->GetTitle());
    hStk->GetYaxis()->SetTitle(h->GetYaxis()->GetTitle());

    hStk->GetXaxis()->CenterTitle((h->GetXaxis()->GetCenterTitle()));
    hStk->GetYaxis()->CenterTitle((h->GetYaxis()->GetCenterTitle()));

    hStk->GetXaxis()->SetTitleSize(h->GetXaxis()->GetTitleSize());
    hStk->GetYaxis()->SetTitleSize(h->GetYaxis()->GetTitleSize());

    hStk->GetXaxis()->SetTitleOffset(h->GetXaxis()->GetTitleOffset());
    hStk->GetYaxis()->SetTitleOffset(h->GetYaxis()->GetTitleOffset());

    hStk->GetXaxis()->SetTitleFont(h->GetXaxis()->GetTitleFont());
    hStk->GetYaxis()->SetTitleFont(h->GetYaxis()->GetTitleFont());

    hStk->GetXaxis()->SetRangeUser(h->GetXaxis()->GetXmin(), h->GetXaxis()->GetXmax());
    //hStk->GetYaxis()->SetRangeUser(h->GetYaxis()->GetXmin(), h->GetYaxis()->GetXmax());
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

    if (legendTextSize > 0) {
        leg->SetTextSize(legendTextSize);
    }
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
        vJetTrkPlot(argv[1], argv[2]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./vJetTrkPlot.exe <inputFileList> <figInfo>"
                << std::endl;
        return 1;
    }
}
