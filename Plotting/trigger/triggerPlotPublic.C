/*
 * Stand-alone macro to produce trigger performance plots
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
#include <TGraphAsymmErrors.h>
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
    k_HIRun2018PbPb_Photon_EB_HLTL1_nPaths2,
    k_HIRun2018PbPb_Photon_EB_HLTL1_nPaths3,
    k_HIRun2018PbPb_Photon_EB_HLT_nPaths2,
    k_HIRun2018PbPb_Photon_EB_HLT_nPaths3,
    k_HIRun2018PbPb_Photon_EBEE_HLTL1_nPaths2,
    k_HIRun2018PbPb_Photon_EBEE_HLTL1_nPaths3,
    k_HIRun2018PbPb_Photon_EBEE_HLT_nPaths2,
    k_HIRun2018PbPb_Photon_EBEE_HLT_nPaths3,
    kN_FIGURES
};

std::string figureNames[kN_FIGURES] = {"triggerPlot_HIRun2018PbPb_Photon_EB_HLTL1_nPaths2",
                                       "triggerPlot_HIRun2018PbPb_Photon_EB_HLTL1_nPaths3",
                                       "triggerPlot_HIRun2018PbPb_Photon_EB_HLT_nPaths2",
                                       "triggerPlot_HIRun2018PbPb_Photon_EB_HLT_nPaths3",
                                       "triggerPlot_HIRun2018PbPb_Photon_EBEE_HLTL1_nPaths2",
                                       "triggerPlot_HIRun2018PbPb_Photon_EBEE_HLTL1_nPaths3",
                                       "triggerPlot_HIRun2018PbPb_Photon_EBEE_HLT_nPaths2",
                                       "triggerPlot_HIRun2018PbPb_Photon_EBEE_HLT_nPaths3"};

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
std::vector<std::string> objPaths;
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

void triggerPlotPublic(int figureIndex, std::string inputFile);
void triggerPlotPublicGeneric(std::string inputFile, int figIndex = 0);
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

void triggerPlotPublic(int figureIndex, std::string inputFile)
{
    std::cout<<"running triggerPlotPublic()"<<std::endl;
    std::cout<<"figureIndex = "<< figureIndex <<std::endl;
    std::cout<<"inputFile = "<< inputFile.c_str() <<std::endl;

    switch (figureIndex) {
        case k_HIRun2018PbPb_Photon_EB_HLTL1_nPaths2:
            triggerPlotPublicGeneric(inputFile, k_HIRun2018PbPb_Photon_EB_HLTL1_nPaths2);
            break;
        case k_HIRun2018PbPb_Photon_EB_HLTL1_nPaths3:
            triggerPlotPublicGeneric(inputFile, k_HIRun2018PbPb_Photon_EB_HLTL1_nPaths3);
            break;
        case k_HIRun2018PbPb_Photon_EB_HLT_nPaths2:
            triggerPlotPublicGeneric(inputFile, k_HIRun2018PbPb_Photon_EB_HLT_nPaths2);
            break;
        case k_HIRun2018PbPb_Photon_EB_HLT_nPaths3:
            triggerPlotPublicGeneric(inputFile, k_HIRun2018PbPb_Photon_EB_HLT_nPaths3);
            break;
        case k_HIRun2018PbPb_Photon_EBEE_HLTL1_nPaths2:
            triggerPlotPublicGeneric(inputFile, k_HIRun2018PbPb_Photon_EBEE_HLTL1_nPaths2);
            break;
        case k_HIRun2018PbPb_Photon_EBEE_HLTL1_nPaths3:
            triggerPlotPublicGeneric(inputFile, k_HIRun2018PbPb_Photon_EBEE_HLTL1_nPaths3);
            break;
        case k_HIRun2018PbPb_Photon_EBEE_HLT_nPaths2:
            triggerPlotPublicGeneric(inputFile, k_HIRun2018PbPb_Photon_EBEE_HLT_nPaths2);
            break;
        case k_HIRun2018PbPb_Photon_EBEE_HLT_nPaths3:
            triggerPlotPublicGeneric(inputFile, k_HIRun2018PbPb_Photon_EBEE_HLT_nPaths3);
            break;
        default:
            break;
    }

    std::cout<<"triggerPlotPublic() - END"<<std::endl;
}

void triggerPlotPublicGeneric(std::string inputFile, int figIndex)
{
    std::cout<<"running triggerPlotPublicGeneric()"<<std::endl;

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
    TCanvas* c = new TCanvas(figureNames[figIndex].c_str(), "", windowWidth, windowHeight);
    std::cout<<"preparing canvas : "<< c->GetName() <<std::endl;
    setCanvas(c);
    c->cd();

    bool isHLTL1 = (figIndex == k_HIRun2018PbPb_Photon_EB_HLTL1_nPaths2 ||
                    figIndex == k_HIRun2018PbPb_Photon_EB_HLTL1_nPaths3 ||
                    figIndex == k_HIRun2018PbPb_Photon_EBEE_HLTL1_nPaths2 ||
                    figIndex == k_HIRun2018PbPb_Photon_EBEE_HLTL1_nPaths3);

    xTitle = "p^{#gamma}_{T} (GeV/c)";
    yTitle = (isHLTL1) ? "L1+HLT efficiency" : "HLT efficiency";
    xTitleSize = 0.0525;
    yTitleSize = 0.0525;
    xTitleOffset = 1.25;
    yTitleOffset = 1.5;
    xTitleFont = 42;
    yTitleFont = 42;

    yMin = 0;
    yMax = 1.2;

    enum OBJLABELS {
        k_photon40,
        k_photon50,
        k_photon60,
        kN_OBJLABELS
    };

    bool is_nPaths2 = (figIndex == k_HIRun2018PbPb_Photon_EB_HLTL1_nPaths2 ||
                       figIndex == k_HIRun2018PbPb_Photon_EB_HLT_nPaths2 ||
                       figIndex == k_HIRun2018PbPb_Photon_EBEE_HLTL1_nPaths2 ||
                       figIndex == k_HIRun2018PbPb_Photon_EBEE_HLT_nPaths2);

    bool isEBEE = (figIndex == k_HIRun2018PbPb_Photon_EBEE_HLTL1_nPaths2 ||
                   figIndex == k_HIRun2018PbPb_Photon_EBEE_HLTL1_nPaths3 ||
                   figIndex == k_HIRun2018PbPb_Photon_EBEE_HLT_nPaths2 ||
                   figIndex == k_HIRun2018PbPb_Photon_EBEE_HLT_nPaths3);
    std::string objPathSubStr = (isEBEE) ? "etaBin1_recoPtBin0_centBin0_sumIsoBin1_sieieBin0" : "etaBin1_recoPtBin0_centBin0_sumIsoBin1_sieieBin1";

    if (is_nPaths2) {
        objPaths = {
                Form("gEff_depRecoPt_%s_trig3", objPathSubStr.c_str()),
                Form("gEff_depRecoPt_%s_trig4", objPathSubStr.c_str()),
        };
    }
    else {
        objPaths = {
                Form("gEff_depRecoPt_%s_trig3", objPathSubStr.c_str()),
                Form("gEff_depRecoPt_%s_trig4", objPathSubStr.c_str()),
                Form("gEff_depRecoPt_%s_trig5", objPathSubStr.c_str()),
        };
    }

    graphMarkerColors = {kBlack, kBlue, kRed};
    graphMarkerStyles = {kFullCircle, kFullCircle, kFullCircle};
    graphMarkerSizes = {1.8, 1.8, 1.8};
    graphMarkerTransparencies = {1.0, 1.0, 1.0};
    graphLineColors = graphMarkerColors;
    graphLineTransparencies = {1.0, 1.0, 1.0};
    graphLineWidths = {2, 2, 2};
    graphLineStyles = {kSolid, kSolid, kSolid};
    graphFillColors = graphMarkerColors;
    graphFillTransparencies = {1.0, 1.0, 1.0};
    drawOptions = {"p e same", "p e same", "p e same"};

    markerColors = {kBlack};
    markerStyles = {kFullCircle};
    markerSizes = {2};
    lineColors = markerColors;
    lineTransparencies = {1.0};
    lineWidths = {0};
    fillColors = markerColors;
    fillTransparencies = lineTransparencies;
    // dummy histogram to be used as template for the graph
    TH1D* hTmp = new TH1D("hTmp", "", 100, 20, 109.99);
    setTH1D(0, hTmp);
    hTmp->SetStats(false);
    hTmp->GetXaxis()->SetLabelSize(0.045);
    hTmp->GetXaxis()->SetLabelOffset(0.02);
    hTmp->GetYaxis()->SetLabelSize(0.045);
    hTmp->GetYaxis()->SetLabelOffset(0.01);
    hTmp->Draw("e");

    c->Update();
    TLine* line = new TLine(gPad->GetUxmin(), 1, gPad->GetUxmax(), 1);
    line->SetLineStyle(kDashed);
    line->Draw();

    int nObjPaths = objPaths.size();
    std::vector<TGraphAsymmErrors*> graphs(nObjPaths, 0);
    TGraphAsymmErrors* gTmp = 0;
    for (int i = 0; i < nObjPaths; ++i) {

        graphs[i] = (TGraphAsymmErrors*)input->Get(objPaths[i].c_str());
        setTGraph(i, graphs[i]);

        graphs[i]->Draw(drawOptions[i].c_str());
    }

    legendX1 = 0.6;
    legendY1 = 0.20 + (3-nObjPaths) * 0.16 / 3;
    legendWidth = 0.5;
    legendHeight = (0.16 * nObjPaths / 3);
    legendMargin = 0.10;
    legendEntryTexts = {
            "HLT GED Photon 40",
            "HLT GED Photon 50",
            "HLT GED Photon 60"
    };
    legendEntryOptions = {
            "p e l",
            "p e l",
            "p e l"
    };
    TLegend* leg = new TLegend();

    for (int i = 0; i < nObjPaths; ++i) {
        gTmp = (TGraphAsymmErrors*)graphs[i]->Clone(Form("%s_tmp", graphs[i]->GetName()));
        //gTmp->SetLineWidth(0);
        leg->AddEntry(gTmp, legendEntryTexts[i].c_str(), legendEntryOptions[i].c_str());
    }

    setLegend(leg);
    leg->Draw();

    textX = 0.93;
    textYs = {legendY1+legendHeight+0.04};
    textAlign = 31;
    textFont = 43;
    textSize = 34;
    if (isEBEE) {
        textLines = {
                "|#eta^{#gamma}| < 2.1"
        };
    }
    else {
        textLines = {
                "|#eta^{#gamma}| < 1.44"
        };
    }
    int nTextLines = textLines.size();
    TLatex* latex = 0;
    for (int i = 0; i < nTextLines; ++i) {
        latex = new TLatex();
        setLatex(i, latex);
        latex->Draw();
    }

    textXsOverPad = {0.96};
    textYOverPad = 0.96;
    textAlignsOverPad = {31};
    textFontOverPad = 43;
    textSizeOverPad = 30;
    textOverPadLines = {
            "PbPb, #sqrt{s_{NN}} = 5.02 TeV, 2018"
    };
    int nTextOverPadLines = textOverPadLines.size();
    for (int i = 0; i < nTextOverPadLines; ++i) {
        latex = new TLatex();
        setLatexOverPad(i, latex);
        latex->Draw();
    }

    textXCMS = 0.25;
    textYCMS = 0.87;
    textAlignCMS = 11;
    textFontCMS = 61;
    textSizeCMS = 0.06;
    latex = new TLatex();
    setLatexCMS(latex);
    latex->Draw();

    bool isPreliminary = true;
    if (isPreliminary) {
        latex = new TLatex();
        latex->SetTextFont(52);
        latex->DrawLatexNDC(textXCMS, textYCMS-0.04, "Preliminary");
    }

    c->Update();

    c->SaveAs(Form("%s.C", c->GetName()));
    c->SaveAs(Form("%s.pdf", c->GetName()));
    c->SaveAs(Form("%s.png", c->GetName()));
    c->Close();         // do not use Delete() for TCanvas.

    std::cout<<"Closing the input file"<<std::endl;
    input->Close();

    std::cout<<"running triggerPlotPublicGeneric() - END"<<std::endl;
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
        triggerPlotPublic(atoi(argv[1]), argv[2]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./triggerPlotPublic.exe <figureIndex> <inputFile>"
                << std::endl;
        return 1;
    }
}
