/*
 * macro to save histograms as picture files.
 * saves histograms to a graphics file.
 */

#include <TFile.h>
#include <TH1.h>
#include <TH1D.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TLine.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TMath.h>

#include <string>
#include <vector>
#include <iostream>

#include "../TreeHeaders/CutConfigurationTree.h"
#include "../Utilities/interface/CutConfigurationParser.h"
#include "../Utilities/interface/InputConfigurationParser.h"
#include "../Utilities/interface/GraphicsConfigurationParser.h"
#include "../Utilities/styleUtil.h"
#include "../Utilities/th1Util.h"

void plotHistogram(const TString configFile, const TString inputFile, const TString outputFile = "plotHistogram");

void plotHistogram(const TString configFile, const TString inputFile, const TString outputFile)
{
    std::cout<<"running plotHistogram()"<<std::endl;
    std::cout<<"configFile  = "<< configFile.Data() <<std::endl;
    std::cout<<"inputFile   = "<< inputFile.Data()  <<std::endl;
    std::cout<<"outputFile  = "<< outputFile.Data() <<std::endl;

    InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

    // input for TH1
    std::vector<std::string> TH1_paths;      // paths of TH1D histograms
    std::string title;
    std::string titleX;
    std::string titleY;
    float titleOffsetX;
    float titleOffsetY;
    std::vector<float> TH1_scales;
    std::vector<int> TH1_rebins;
    std::vector<float> TH1_norms;
    float xMin;
    float xMax;
    float yMin;
    float yMax;
    float markerSize;
    int drawSame;
    int drawNormalized;
    std::vector<std::string> drawOptions;
    std::vector<std::string> markerStyles;
    std::vector<std::string> lineStyles;
    std::vector<std::string> fillStyles;
    std::vector<std::string> colors;
    std::vector<std::string> fillColors;
    std::vector<std::string> lineColors;
    int lineWidth;
    int fitTH1;

    // input for TF1
    std::vector<std::string> TF1_formulas;
    std::vector<std::vector<double>> TF1_ranges;
    std::vector<std::string> fitOptions;
    std::vector<std::string> fitColors;

    // input for TLegend
    std::vector<std::string> legendEntryLabels;
    std::string legendPosition;
    float legendOffsetX;
    float legendOffsetY;
    int legendBorderSize;
    float legendWidth;
    float legendHeight;
    float legendTextSize;

    // input for text objects
    std::vector<std::string> textLines;
    int textFont;
    float textSize;
    std::string textPosition;
    float textOffsetX;
    float textOffsetY;

    std::vector<std::string> textsOverPad;
    std::vector<std::string> textsOverPadAlignments;
    int textAbovePadFont;
    float textAbovePadSize;
    float textAbovePadOffsetX;
    float textAbovePadOffsetY;

    // input for TLine
    std::vector<float> TLines_horizontal;               // y-axis positions of the horizontal lines to be drawn
    std::vector<std::string> lineStyles_horizontal;     // styles of the horizontal lines to be drawn
    std::vector<float> TLines_vertical;                 // x-axis positions of the vertical lines to be drawn
    std::vector<std::string> lineStyles_vertical;       // styles of the vertical lines to be drawn

    // input for lower pad
    int drawRatio;
    int drawDiff;
    float windowHeightFraction;
    std::string titleY_lowerPad;
    std::vector<float> TLines_horizontal_lowerPad;               // y-axis positions of the horizontal lines to be drawn in the lower pad
    std::vector<std::string> lineStyles_horizontal_lowerPad;     // styles of the horizontal lines to be drawn in the lower pad
    std::vector<float> TLines_vertical_lowerPad;                 // x-axis positions of the vertical lines to be drawn in the lower pad
    std::vector<std::string> lineStyles_vertical_lowerPad;       // styles of the vertical lines to be drawn in the lower pad
    // input for TF1 in lowerPad
    int fitTH1_lowerPad;
    std::vector<std::string> TF1_formulas_lowerPad;
    std::vector<std::vector<double>> TF1_ranges_lowerPad;
    std::vector<std::string> fitOptions_lowerPad;
    std::vector<std::string> fitColors_lowerPad;

    // input for TCanvas
    int windowWidth;
    int windowHeight;
    float leftMargin;
    float rightMargin;
    float bottomMargin;
    float topMargin;
    int setLogx;
    int setLogy;

    if (configInput.isValid) {
        TH1_paths   = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_TH1_path]);
        title = ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_TH1_title]);
        titleX = ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_TH1_titleX]);
        titleY = ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_TH1_titleY]);
        titleOffsetX = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_titleOffsetX];
        titleOffsetY = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_titleOffsetY];
        TH1_scales  = ConfigurationParser::ParseListFloat(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_TH1_scale]);
        TH1_rebins  = ConfigurationParser::ParseListInteger(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_TH1_rebin]);
        TH1_norms = ConfigurationParser::ParseListFloat(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_TH1_norm]);
        xMin = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_TH1_xMin];
        xMax = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_TH1_xMax];
        yMin = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_TH1_yMin];
        yMax = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_TH1_yMax];
        markerSize = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_markerSize];
        drawSame = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_drawSame];
        drawNormalized = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_drawNormalized];
        drawOptions = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_drawOption]);
        markerStyles = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_markerStyle]);
        lineStyles = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_lineStyle]);
        fillStyles = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_fillStyle]);
        colors = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_color]);
        fillColors = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_fillColor]);
        lineColors = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_lineColor]);
        lineWidth = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_lineWidth];
        fitTH1 = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_fitTH1];

        TF1_formulas = ConfigurationParser::ParseListTF1Formula(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_TF1]);
        TF1_ranges   = ConfigurationParser::ParseListTF1Range(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_TF1]);
        fitOptions = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_fitOption]);
        fitColors = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_fitColor]);

        legendEntryLabels = ConfigurationParser::ParseList(ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_legendEntryLabel]));
        legendPosition    = configInput.proc[INPUT::kPLOTTING].s[INPUT::k_legendPosition];
        legendOffsetX     = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_legendOffsetX];
        legendOffsetY     = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_legendOffsetY];
        legendBorderSize  = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_legendBorderSize];
        legendWidth       = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_legendWidth];
        legendHeight      = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_legendHeight];
        legendTextSize    = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_legendTextSize];

        std::string tmpText = ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_text]);
        textLines = ConfigurationParser::ParseList(tmpText);
        textFont = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_textFont];
        textSize = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_textSize];
        textPosition = configInput.proc[INPUT::kPLOTTING].s[INPUT::k_textPosition];
        textOffsetX = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_textOffsetX];
        textOffsetY = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_textOffsetY];

        std::string tmpTextOverPad = ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_textAbovePad]);
        textsOverPad = ConfigurationParser::ParseList(tmpTextOverPad);
        textsOverPadAlignments = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_textAbovePadAlign]);
        textAbovePadFont = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_textAbovePadFont];
        textAbovePadSize = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_textAbovePadSize];
        textAbovePadOffsetX = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_textAbovePadOffsetX];
        textAbovePadOffsetY = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_textAbovePadOffsetY];

        TLines_horizontal = ConfigurationParser::ParseListFloat(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_TLine_horizontal]);
        lineStyles_horizontal = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_LineStyle_horizontal]);
        TLines_vertical = ConfigurationParser::ParseListFloat(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_TLine_vertical]);
        lineStyles_vertical = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_LineStyle_vertical]);

        // lower pad objects
        drawRatio = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_drawRatio];
        drawDiff  = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_drawDiff];
        windowHeightFraction = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_windowHeightFraction];
        titleY_lowerPad = ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_TH1_titleY_lowerPad]);
        TLines_horizontal_lowerPad = ConfigurationParser::ParseListFloat(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_TLine_horizontal_lowerPad]);
        lineStyles_horizontal_lowerPad = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_LineStyle_horizontal_lowerPad]);
        TLines_vertical_lowerPad = ConfigurationParser::ParseListFloat(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_TLine_vertical_lowerPad]);
        lineStyles_vertical_lowerPad = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_LineStyle_vertical_lowerPad]);
        // input for TF1 in lowerPad
        fitTH1_lowerPad = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_fitTH1_lowerPad];
        TF1_formulas_lowerPad = ConfigurationParser::ParseListTF1Formula(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_TF1_lowerPad]);
        TF1_ranges_lowerPad = ConfigurationParser::ParseListTF1Range(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_TF1_lowerPad]);
        fitOptions_lowerPad = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_fitOption_lowerPad]);
        fitColors_lowerPad = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_fitColor_lowerPad]);

        windowWidth = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_windowWidth];
        windowHeight = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_windowHeight];
        leftMargin   = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_leftMargin];
        rightMargin  = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_rightMargin];
        bottomMargin = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_bottomMargin];
        topMargin    = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_topMargin];
        setLogx = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_setLogx];
        setLogy = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_setLogy];
    }
    else {
        title = "";
        titleX = "";
        titleY = "";
        titleOffsetX = 1;
        titleOffsetY = 1;
        xMin = 0;
        xMax = -1;
        yMin = 0;
        yMax = -1;
        markerSize = 1;
        drawSame = 0;
        drawNormalized = 0;
        lineWidth = 0;
        fitTH1 = 0;

        drawRatio = 0;
        drawDiff = 0;
        windowHeightFraction = 0;
        titleY_lowerPad = "";

        legendPosition = "";
        legendOffsetX = 0;
        legendOffsetY = 0;
        legendBorderSize = 0;
        legendWidth = 0;
        legendHeight = 0;
        legendTextSize = 0;

        textFont = 0;
        textSize = 0;
        textPosition = "";
        textOffsetX = 0;
        textOffsetY = 0;

        textAbovePadFont = 0;
        textAbovePadSize = 0;
        textAbovePadOffsetX = 0;
        textAbovePadOffsetY = 0;

        drawRatio = 0;
        drawDiff = 0;
        windowHeightFraction = 0;
        fitTH1_lowerPad = 0;

        windowWidth = 0;
        windowHeight = 0;
        leftMargin = 0.1;
        rightMargin = 0.1;
        bottomMargin = 0.1;
        topMargin = 0.1;
        setLogx = 0;
        setLogy = 0;
    }
    // set default values
    if (titleOffsetX == 0) titleOffsetX = INPUT_DEFAULT::titleOffsetX;
    if (titleOffsetY == 0) titleOffsetY = INPUT_DEFAULT::titleOffsetY;
    if (xMin == 0 && xMax == 0)  xMax = -1;
    else if (xMin <= 0 && setLogx > 0)  xMin = resetTH1axisMin4LogScale(xMin, "x");
    if (yMin == 0 && yMax == 0)  yMax = -1;
    else if (yMin <= 0 && setLogy > 0)  yMin = resetTH1axisMin4LogScale(yMin, "y");
    if (lineWidth == 0)  lineWidth = INPUT_DEFAULT::lineWidth;

    if (markerSize == 0)  markerSize = INPUT_DEFAULT::markerSize;

    if (textFont == 0)  textFont = INPUT_DEFAULT::textFont;
    if (textSize == 0)  textSize = INPUT_DEFAULT::textSize;

    if (textAbovePadFont == 0)  textAbovePadFont = INPUT_DEFAULT::textAbovePadFont;
    if (textAbovePadSize == 0)  textAbovePadSize = INPUT_DEFAULT::textAbovePadSize;

    if ((drawRatio > 0 || drawDiff > 0) && windowHeightFraction <= 0)  windowHeightFraction = INPUT_DEFAULT::windowHeightFraction;

    if (windowWidth  == 0)  windowWidth = INPUT_DEFAULT::windowWidth;
    if (windowHeight == 0)  windowHeight = INPUT_DEFAULT::windowHeight;
    if (leftMargin == 0) leftMargin = INPUT_DEFAULT::leftMargin;
    if (rightMargin == 0) rightMargin = INPUT_DEFAULT::rightMargin;
    if (bottomMargin == 0) bottomMargin = INPUT_DEFAULT::bottomMargin;
    if (topMargin == 0) topMargin = INPUT_DEFAULT::topMargin;

    int nTH1_Paths = TH1_paths.size();
    int nTH1_scales = TH1_scales.size();
    int nTH1_rebins = TH1_rebins.size();
    int nTH1_norms = TH1_norms.size();
    int nTF1_formulas = TF1_formulas.size();    // assume TF1_formulas.size() = TF1_ranges[0].size()
    int nFitOptions = fitOptions.size();
    int nFitColors = fitColors.size();
    int nDrawOptions = drawOptions.size();
    int nMarkerStyles = markerStyles.size();
    int nLineStyles = lineStyles.size();
    int nFillStyles = fillStyles.size();
    int nColors = colors.size();
    int nFillColors = fillColors.size();
    int nLineColors = lineColors.size();
    int nLegendEntryLabels = legendEntryLabels.size();
    int nTextLines = textLines.size();
    int nTextsOverPad = textsOverPad.size();
    int nTextsOverPadAlignments = textsOverPadAlignments.size();
    int nTLines_horizontal = TLines_horizontal.size();
    int nLineStyles_horizontal = lineStyles_horizontal.size();
    int nTLines_vertical = TLines_vertical.size();
    int nLineStyles_vertical = lineStyles_vertical.size();
    int nTLines_horizontal_lowerPad = TLines_horizontal_lowerPad.size();
    int nLineStyles_horizontal_lowerPad = lineStyles_horizontal_lowerPad.size();
    int nTLines_vertical_lowerPad = TLines_vertical_lowerPad.size();
    int nLineStyles_vertical_lowerPad = lineStyles_vertical_lowerPad.size();
    int nTF1_formulas_lowerPad = TF1_formulas_lowerPad.size();    // assume TF1_formulas.size() = TF1_ranges[0].size()
    int nFitOptions_lowerPad = fitOptions_lowerPad.size();
    int nFitColors_lowerPad = fitColors_lowerPad.size();

    // verbose about input configuration
    std::cout<<"Input Configuration :"<<std::endl;
    std::cout << "nTH1_Paths = " << nTH1_Paths << std::endl;
    for (int i = 0; i<nTH1_Paths; ++i) {
            std::cout << Form("TH1_Path[%d] = %s", i, TH1_paths.at(i).c_str()) << std::endl;
    }
    std::cout << "title  = " << title.c_str() << std::endl;
    std::cout << "titleX = " << titleX.c_str() << std::endl;
    std::cout << "titleY = " << titleY.c_str() << std::endl;
    std::cout << "titleOffsetX = " << titleOffsetX << std::endl;
    std::cout << "titleOffsetY = " << titleOffsetY << std::endl;
    std::cout << "nTH1_scales  = " << nTH1_scales << std::endl;
    for (int i = 0; i<nTH1_scales; ++i) {
            std::cout << Form("TH1_scales[%d] = %f", i, TH1_scales.at(i)) << std::endl;
    }
    std::cout << "nTH1_rebins  = " << nTH1_rebins << std::endl;
    for (int i = 0; i<nTH1_rebins; ++i) {
            std::cout << Form("TH1_rebins[%d] = %d", i, TH1_rebins.at(i)) << std::endl;
    }
    std::cout << "nTH1_normalizations  = " << nTH1_norms << std::endl;
    for (int i = 0; i<nTH1_norms; ++i) {
            std::cout << Form("TH1_normalizations[%d] = %f", i, TH1_norms.at(i)) << std::endl;
    }
    std::cout << "xMin = " << xMin << std::endl;
    std::cout << "xMax = " << xMax << std::endl;
    std::cout << "yMin = " << yMin << std::endl;
    std::cout << "yMax = " << yMax << std::endl;
    std::cout << "markerSize = " << markerSize << std::endl;
    std::cout << "drawSame = " << drawSame << std::endl;
    std::cout << "drawNormalized = " << drawNormalized << std::endl;
    std::cout << "nDrawOptions   = " << nDrawOptions << std::endl;
    for (int i = 0; i<nDrawOptions; ++i) {
            std::cout << Form("drawOptions[%d] = %s", i, drawOptions.at(i).c_str()) << std::endl;
    }
    std::cout << "nMarkerStyles  = " << nMarkerStyles << std::endl;
    for (int i = 0; i<nMarkerStyles; ++i) {
            std::cout << Form("markerStyles[%d] = %s", i, markerStyles.at(i).c_str()) << std::endl;
    }
    std::cout << "nLineStyles   = " << nLineStyles << std::endl;
    for (int i = 0; i<nLineStyles; ++i) {
            std::cout << Form("lineStyles[%d] = %s", i, lineStyles.at(i).c_str()) << std::endl;
    }
    std::cout << "nFillStyles   = " << nFillStyles << std::endl;
    for (int i = 0; i<nFillStyles; ++i) {
            std::cout << Form("fillStyles[%d] = %s", i, fillStyles.at(i).c_str()) << std::endl;
    }
    std::cout << "nColors   = " << nColors << std::endl;
    for (int i = 0; i<nColors; ++i) {
            std::cout << Form("colors[%d] = %s", i, colors.at(i).c_str()) << std::endl;
    }
    std::cout << "nFillColors = " << nFillColors << std::endl;
    for (int i = 0; i<nFillColors; ++i) {
            std::cout << Form("fillColors[%d] = %s", i, fillColors.at(i).c_str()) << std::endl;
    }
    std::cout << "nLineColors = " << nLineColors << std::endl;
    for (int i = 0; i<nLineColors; ++i) {
            std::cout << Form("lineColors[%d] = %s", i, lineColors.at(i).c_str()) << std::endl;
    }
    std::cout << "lineWidth = " << lineWidth << std::endl;

    std::cout << "fitTH1 = " << fitTH1 << std::endl;
    if (fitTH1 > 0) {
        std::cout << "nTF1_formulas = " << nTF1_formulas << std::endl;
        std::cout << "nFitOptions = " << nFitOptions << std::endl;
        std::cout << "nFitColors = " << nFitColors << std::endl;
        for (int i = 0; i<nTF1_formulas; ++i) {
                std::cout << Form("TF1_list[%d] = { ", i);
                std::cout << Form("%s, ", TF1_formulas.at(i).c_str());
                std::cout << Form("%f, ", TF1_ranges[0].at(i));
                std::cout << Form("%f }", TF1_ranges[1].at(i));

                if (nFitOptions == nTF1_formulas) {
                    std::cout << Form(", fitOptions[%d] = ", i) << fitOptions.at(i).c_str();
                }
                if (nFitColors == nTF1_formulas) {
                    std::cout << Form(", fitColors[%d] = ", i) << fitColors.at(i).c_str();
                }

                std::cout<<std::endl;
        }
        if (nFitOptions != nTF1_formulas) {
            for (int i = 0; i<nFitOptions; ++i) {
                std::cout << Form("fitOptions[%d] = %s", i, fitOptions.at(i).c_str()) << std::endl;
            }
        }
        if (nFitColors != nTF1_formulas) {
            for (int i = 0; i<nFitColors; ++i) {
                std::cout << Form("fitColors[%d] = %s", i, fitColors.at(i).c_str()) << std::endl;
            }
        }
    }

    std::cout << "nLegendEntryLabels   = " << nLegendEntryLabels << std::endl;
    for (int i = 0; i<nLegendEntryLabels; ++i) {
            std::cout << Form("legendEntryLabels[%d] = %s", i, legendEntryLabels.at(i).c_str()) << std::endl;
    }
    if (nLegendEntryLabels > 0) {
        std::cout << "legendPosition   = " << legendPosition.c_str() << std::endl;
        if (legendPosition.size() == 0) std::cout<< "No position is provided, legend will not be drawn." <<std::endl;
        std::cout << "legendOffsetX    = " << legendOffsetX << std::endl;
        std::cout << "legendOffsetY    = " << legendOffsetY << std::endl;
        std::cout << "legendBorderSize = " << legendBorderSize << std::endl;
        std::cout << "legendWidth      = " << legendWidth << std::endl;
        std::cout << "legendHeight     = " << legendHeight << std::endl;
        std::cout << "legendTextSize   = " << legendTextSize << std::endl;
    }

    std::cout << "nTextLines   = " << nTextLines << std::endl;
    for (int i = 0; i<nTextLines; ++i) {
            std::cout << Form("textLines[%d] = %s", i, textLines.at(i).c_str()) << std::endl;
    }
    if (nTextLines > 0) {
        std::cout << "textFont = " << textFont << std::endl;
        std::cout << "textSize = " << textSize << std::endl;
        std::cout << "textPosition = " << textPosition << std::endl;
        std::cout << "textOffsetX  = " << textOffsetX << std::endl;
        std::cout << "textOffsetY  = " << textOffsetY << std::endl;
    }

    std::cout << "nTextsOverPad = " << nTextsOverPad << std::endl;
    for (int i = 0; i<nTextsOverPad; ++i) {
            std::cout << Form("textsOverPad[%d] = %s", i, textsOverPad.at(i).c_str()) << std::endl;
    }
    if (nTextsOverPad > 0) {
        std::cout << "nTextsOverPadAlignments = " << nTextsOverPadAlignments << std::endl;
        for (int i = 0; i<nTextsOverPadAlignments; ++i) {
                std::cout << Form("textsOverPadAlignments[%d] = %s", i, textsOverPadAlignments.at(i).c_str()) << std::endl;
        }
        std::cout << "textAbovePadFont = " << textAbovePadFont << std::endl;
        std::cout << "textAbovePadSize = " << textAbovePadSize << std::endl;
        std::cout << "textAbovePadOffsetX  = " << textAbovePadOffsetX << std::endl;
        std::cout << "textAbovePadOffsetY  = " << textAbovePadOffsetY << std::endl;
    }

    std::cout << "nTLines_horizontal = " << nTLines_horizontal << std::endl;
    for (int i = 0; i<nTLines_horizontal; ++i) {
            std::cout << Form("TLines_horizontal[%d] = %f", i, TLines_horizontal.at(i)) << std::endl;
    }
    if (nTLines_horizontal > 0) {
        std::cout << "nLineStyles_horizontal = " << nLineStyles_horizontal << std::endl;
        for (int i = 0; i<nLineStyles_horizontal; ++i) {
            std::cout << Form("lineStyles_horizontal[%d] = %s", i, lineStyles_horizontal.at(i).c_str()) << std::endl;
        }
    }
    std::cout << "nTLines_vertical = " << nTLines_vertical << std::endl;
    for (int i = 0; i<nTLines_vertical; ++i) {
            std::cout << Form("TLines_vertical[%d] = %f", i, TLines_vertical.at(i)) << std::endl;
    }
    if (nTLines_vertical > 0) {
        std::cout << "nLineStyles_vertical = " << nLineStyles_vertical << std::endl;
        for (int i = 0; i<nLineStyles_vertical; ++i) {
            std::cout << Form("lineStyles_vertical[%d] = %s", i, lineStyles_vertical.at(i).c_str()) << std::endl;
        }
    }

    std::cout << "drawRatio = " << drawRatio << std::endl;
    std::cout << "drawDiff  = " << drawDiff << std::endl;
    if (drawRatio > 0 && drawDiff > 0) {
        std::cout << "both drawRatio and drawDiff are set. can set at most one of them at a time." << std::endl;
        std::cout << "exiting" << std::endl;
        return;
    }
    else if (drawRatio > 0 || drawDiff > 0) {
        std::cout << "windowHeightFraction  = " << windowHeightFraction << std::endl;
        std::cout << "titleYRatio = " << titleY_lowerPad.c_str() << std::endl;
        // lower pad objects
        if (nTLines_horizontal_lowerPad > 0) {
            std::cout << "nLineStyles_horizontal_lowerPad = " << nLineStyles_horizontal_lowerPad << std::endl;
            for (int i = 0; i<nLineStyles_horizontal_lowerPad; ++i) {
                std::cout << Form("lineStyles_horizontal_lowerPad[%d] = %s", i, lineStyles_horizontal_lowerPad.at(i).c_str()) << std::endl;
            }
        }
        std::cout << "nTLines_vertical_lowerPad = " << nTLines_vertical_lowerPad << std::endl;
        for (int i = 0; i<nTLines_vertical_lowerPad; ++i) {
                std::cout << Form("TLines_vertical_lowerPad[%d] = %f", i, TLines_vertical_lowerPad.at(i)) << std::endl;
        }
        if (nTLines_vertical_lowerPad > 0) {
            std::cout << "nLineStyles_vertical_lowerPad = " << nLineStyles_vertical_lowerPad << std::endl;
            for (int i = 0; i<nLineStyles_vertical_lowerPad; ++i) {
                std::cout << Form("lineStyles_vertical_lowerPad[%d] = %s", i, lineStyles_vertical_lowerPad.at(i).c_str()) << std::endl;
            }
        }

        if (fitTH1_lowerPad > 0) {
            std::cout << "nTF1_formulas_lowerPad = " << nTF1_formulas_lowerPad << std::endl;
            std::cout << "nFitOptions_lowerPad = " << nFitOptions_lowerPad << std::endl;
            std::cout << "nFitColors_lowerPad = " << nFitColors_lowerPad << std::endl;
            for (int i = 0; i<nTF1_formulas_lowerPad; ++i) {
                    std::cout << Form("TF1_list_lowerPad[%d] = { ", i);
                    std::cout << Form("%s, ", TF1_formulas_lowerPad.at(i).c_str());
                    std::cout << Form("%f, ", TF1_ranges_lowerPad[0].at(i));
                    std::cout << Form("%f }", TF1_ranges_lowerPad[1].at(i));

                    if (nFitOptions_lowerPad == nTF1_formulas_lowerPad) {
                        std::cout << Form(", fitOptions_lowerPad[%d] = ", i) << fitOptions_lowerPad.at(i).c_str();
                    }
                    if (nFitColors_lowerPad == nTF1_formulas_lowerPad) {
                        std::cout << Form(", fitColors_lowerPad[%d] = ", i) << fitColors_lowerPad.at(i).c_str();
                    }

                    std::cout<<std::endl;
            }
            if (nFitOptions_lowerPad != nTF1_formulas_lowerPad) {
                for (int i = 0; i<nFitOptions_lowerPad; ++i) {
                    std::cout << Form("fitOptions_lowerPad[%d] = %s", i, fitOptions_lowerPad.at(i).c_str()) << std::endl;
                }
            }
            if (nFitColors_lowerPad != nTF1_formulas_lowerPad) {
                for (int i = 0; i<nFitColors_lowerPad; ++i) {
                    std::cout << Form("fitColors_lowerPad[%d] = %s", i, fitColors_lowerPad.at(i).c_str()) << std::endl;
                }
            }
        }
    }

    std::cout << "windowWidth = " << windowWidth << std::endl;
    std::cout << "windowHeight = " << windowHeight << std::endl;
    std::cout << "leftMargin   = " << leftMargin << std::endl;
    std::cout << "rightMargin  = " << rightMargin << std::endl;
    std::cout << "bottomMargin = " << bottomMargin << std::endl;
    std::cout << "topMargin    = " << topMargin << std::endl;
    std::cout << "setLogx  = " << setLogx << std::endl;
    std::cout << "setLogy  = " << setLogy << std::endl;

    if (configCuts.isValid) {

    }
    else {

    }

    // verbose about cut configuration
    std::cout<<"Cut Configuration :"<<std::endl;

    std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.Data());

    int nInputFiles = inputFiles.size();
    std::cout<<"input ROOT files : num = "<<nInputFiles<< std::endl;
    if (nInputFiles == 1) {
        std::cout<<"all histograms will be taken from one file."<< std::endl;
    }
    std::cout<<"#####"<< std::endl;
    for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
        std::cout<<(*it).c_str()<< std::endl;
    }
    std::cout<<"##### END #####"<< std::endl;

    int nHistos = TH1_paths.size();
    std::cout << "nHistos = " << nHistos << std::endl;
    if (nHistos > 1 && nInputFiles > 1 && nHistos != nInputFiles) {
        std::cout << "mismatch of number of histograms and number of input files"<< std::endl;
        std::cout << "nHistos     = "<< nHistos << std::endl;
        std::cout << "nInputFiles = "<< nInputFiles << std::endl;
        std::cout << "exiting " << std::endl;
        return;
    }

    // extra
    std::vector<int> indexDrawOptionHist;   // book histograms with drawOption = "hist" and draw them first
                                            // so that plots with markers are not overwritten.
    bool hDrawn[nHistos];       // true if the histogram is already drawn.

    TFile* f[nHistos];
    TH1::SetDefaultSumw2();
    TH1D* h[nHistos];
    for (int i=0; i<nHistos; ++i) {
        hDrawn[i] = false;

        std::string TH1_Path = TH1_paths.at(i).c_str();
        std::string inputFile = inputFiles.at(0).c_str();
        if (nInputFiles == nHistos)  inputFile = inputFiles.at(i).c_str();

        f[i] = TFile::Open(inputFile.c_str());
        h[i] = (TH1D*)f[i]->Get(TH1_Path.c_str());
        h[i]->SetName(Form("h_%d", i));
        h[i]->SetStats(false);

        // print info about histograms
        std::cout << "#####" << std::endl;
        std::cout << Form("h[%d]", i) << std::endl;
        std::cout << Form("TH1_Paths[%d] = %s", i, TH1_Path.c_str()) << std::endl;
        std::cout << Form("inputFiles[%d] = %s", i, inputFile.c_str()) << std::endl;
        std::string summary = summaryTH1(h[i]);
        std::cout << summary.c_str() << std::endl;

        if (drawNormalized == INPUT_TH1::k_normInt)  h[i]->Scale(1./h[i]->Integral());

        float scale = 1;
        if (nTH1_scales == 1)  scale = TH1_scales.at(0);
        else if (nTH1_scales == nHistos)  scale = TH1_scales.at(i);
        if (scale !=1 )        h[i]->Scale(scale);

        int rebin = 1;
        if (nTH1_rebins == 1)  rebin = TH1_rebins.at(0);
        else if (nTH1_rebins == nHistos)  rebin = TH1_rebins.at(i);
        if (rebin != 1)        h[i]->Rebin(rebin);

        // apply the normalization if it is positive.
        float normalization = 0;
        if (nTH1_norms == 1)  normalization = TH1_norms.at(0);
        else if (nTH1_norms == nHistos)  normalization = TH1_norms.at(i);
        if (normalization > 0)  h[i]->Scale(normalization/h[i]->Integral());

        // overwrite histogram titles, if there are explicitly specified titles
        if (title.compare(CONFIGPARSER::nullInput) == 0) h[i]->SetTitle("");   // title is explicitly specified to be empty.
        else if (title.size() > 0)  h[i]->SetTitle(title.c_str());  // title is specified.

        if (titleX.compare(CONFIGPARSER::nullInput) == 0) h[i]->SetXTitle("");
        else if (titleX.size() > 0) h[i]->SetXTitle(titleX.c_str());

        if (titleY.compare(CONFIGPARSER::nullInput) == 0) h[i]->SetYTitle("");
        else if (titleY.size() > 0) h[i]->SetYTitle(titleY.c_str());

        std::string drawOption = "";
        if (nDrawOptions == 1) {
            if (drawOptions.at(0).compare(CONFIGPARSER::nullInput) != 0)  drawOption = drawOptions.at(0).c_str();
        }
        else if (nDrawOptions == nHistos) {
            if (drawOptions.at(i).compare(CONFIGPARSER::nullInput) != 0)  drawOption = drawOptions.at(i).c_str();
        }
        if (drawOption.size() > 0)  drawOptions.at(i) = drawOption.c_str();     // overwrite drawing options
        if (drawOption.find("hist") != std::string::npos) {
            indexDrawOptionHist.push_back(i);   // book histograms with drawOption = "hist" and draw them first
        }
        // https://root.cern.ch/doc/master/classTObject.html#abe2a97d15738d5de00cd228e0dc21e56
        // TObject::SetDrawOption() is not suitable for the approach here.

        int markerStyle = GRAPHICS::markerStyle;
        if (nMarkerStyles == 1) markerStyle = GraphicsConfigurationParser::ParseMarkerStyle(markerStyles.at(0));
        else if (nMarkerStyles == nHistos) markerStyle = GraphicsConfigurationParser::ParseMarkerStyle(markerStyles.at(i));
        h[i]->SetMarkerStyle(markerStyle);

        int lineStyle = GRAPHICS::lineStyle;
        if (nLineStyles == 1)  lineStyle = GraphicsConfigurationParser::ParseLineStyle(lineStyles.at(0));
        else if (nLineStyles == nHistos)  lineStyle = GraphicsConfigurationParser::ParseLineStyle(lineStyles.at(i));
        h[i]->SetLineStyle(lineStyle);

        int fillStyle = -1;
        if (nFillStyles == 1)  fillStyle = GraphicsConfigurationParser::ParseFillStyle(fillStyles.at(0));
        else if (nFillStyles == nHistos)  fillStyle = GraphicsConfigurationParser::ParseFillStyle(fillStyles.at(i));
        if (fillStyle != -1) h[i]->SetFillStyle(fillStyle);

        int color = GRAPHICS::colors[i];
        if (nColors == 1) color = GraphicsConfigurationParser::ParseColor(colors.at(0));
        else if (nColors == nHistos) color = GraphicsConfigurationParser::ParseColor(colors.at(i));
        h[i]->SetMarkerColor(color);
        h[i]->SetLineColor(color);

        int fillColor = -1;
        if (nFillColors == 1) fillColor = GraphicsConfigurationParser::ParseColor(fillColors.at(0));
        else if (nFillColors == nHistos) fillColor = GraphicsConfigurationParser::ParseColor(fillColors.at(i));
        if (fillColor != -1)  h[i]->SetFillColor(fillColor);

        int lineColor = -1;
        if (nLineColors == 1) lineColor = GraphicsConfigurationParser::ParseColor(lineColors.at(0));
        else if (nLineColors == nHistos) lineColor = GraphicsConfigurationParser::ParseColor(lineColors.at(i));
        if (lineColor != -1)  h[i]->SetLineColor(lineColor);

        if(lineWidth != INPUT_DEFAULT::lineWidth) {
            if (drawOption.find("hist") != std::string::npos)
                h[i]->SetLineWidth(lineWidth);
        }

        h[i]->SetMarkerSize(markerSize);

        if (xMax > xMin)       h[i]->SetAxisRange(xMin, xMax, "X");
        if (yMax > yMin)       h[i]->SetAxisRange(yMin, yMax, "Y");

        h[i]->SetTitleOffset(titleOffsetX,"X");
        h[i]->SetTitleOffset(titleOffsetY,"Y");

        if (fitTH1 > 0) {
            std::string TF1_formula = "";
            double TF1_xMin = 0;
            double TF1_xMax = 0;
            if (nTF1_formulas == 1)  {
                TF1_formula = TF1_formulas.at(0).c_str();
                TF1_xMin = TF1_ranges[0].at(0);
                TF1_xMax = TF1_ranges[1].at(0);
            }
            else if (nTF1_formulas == nHistos)  {
                TF1_formula = TF1_formulas.at(i).c_str();
                TF1_xMin = TF1_ranges[0].at(i);
                TF1_xMax = TF1_ranges[1].at(i);
            }
            std::string fitOption = INPUT_DEFAULT::fitOption.c_str();
            if (nFitOptions == 1)  fitOption = fitOptions.at(0).c_str();
            if (nFitOptions == nHistos)  fitOption = fitOptions.at(i).c_str();

            int fitColor = INPUT_DEFAULT::fitColor;
            if (nFitColors == 1) fitColor = GraphicsConfigurationParser::ParseColor(fitColors.at(0));
            else if (nFitColors == nHistos) {
                if (fitColors.at(i).compare(CONFIGPARSER::nullInput) != 0)
                    fitColor = GraphicsConfigurationParser::ParseColor(fitColors.at(i));
            }

            TF1 f1(Form("f1_%d", i), TF1_formula.c_str(), TF1_xMin, TF1_xMax);
            f1.SetLineColor(fitColor);

            h[i]->Fit(f1.GetName(), fitOption.c_str());
        }
    }

    // save canvases
    TCanvas* c;
    TPad* pad;
    TPad* pad2;
    if (drawSame == 0) {    // histograms will be plotted separately.
        for (int i=0; i<nHistos; ++i) {

            c = new TCanvas(Form("cnv_%d",i),"",windowWidth,windowHeight);
            c->SetTitle(h[i]->GetTitle());
            setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
            setCanvasFinal(c, setLogx, setLogy);
            c->cd();

            std::string drawOption = "";
            if (nDrawOptions == 1)  drawOption = drawOptions.at(0).c_str();
            else if (nDrawOptions == nHistos) drawOption = drawOptions.at(i).c_str();

            h[i]->Draw(drawOption.c_str());

            // add Text
            TLatex* latex;
            if (nTextLines > 0) {
                latex = new TLatex();
                latex->SetTextFont(textFont);
                latex->SetTextSize(textSize);
                setTextAlignment(latex, textPosition);
                std::vector<std::pair<float,float>> textCoordinates = calcTextCoordinates(textLines, textPosition, c, textOffsetX, textOffsetY);
                for (int i = 0; i<nTextLines; ++i){
                    float x = textCoordinates.at(i).first;
                    float y = textCoordinates.at(i).second;
                    latex->DrawLatexNDC(x, y, textLines.at(i).c_str());
                }
            }

            // add Text above the pad
            TLatex* latexOverPad;
            if (nTextsOverPad > 0) {
                latexOverPad = new TLatex();
                latexOverPad->SetTextFont(textAbovePadFont);
                latexOverPad->SetTextSize(textAbovePadSize);
                for (int i = 0; i < nTextsOverPad; ++i) {
                    int textOverPadAlignment = GRAPHICS::textAlign;
                    if (nTextsOverPadAlignments == 1) textOverPadAlignment = GraphicsConfigurationParser::ParseTextAlign(textsOverPadAlignments.at(0));
                    else if (nTextsOverPadAlignments == nTextsOverPad) textOverPadAlignment = GraphicsConfigurationParser::ParseTextAlign(textsOverPadAlignments.at(i));

                    latexOverPad->SetTextAlign(textOverPadAlignment);
                    setTextAbovePad(latexOverPad, c, textAbovePadOffsetX, textAbovePadOffsetY);

                    latexOverPad->DrawLatexNDC(latexOverPad->GetX(), latexOverPad->GetY(), textsOverPad.at(i).c_str());
                }
            }

            // add TLine
            TLine* line_horizontal[nTLines_horizontal];
            for (int iLine = 0; iLine<nTLines_horizontal; ++iLine) {
                // draw horizontal line
                double xmin = h[i]->GetXaxis()->GetBinLowEdge(h[i]->GetXaxis()->GetFirst());
                double xmax = h[i]->GetXaxis()->GetBinLowEdge(h[i]->GetXaxis()->GetLast()+1);

                int lineStyle_horizontal = GRAPHICS::lineStyle_horizontal;
                if (nLineStyles_horizontal == 1)
                    lineStyle_horizontal = GraphicsConfigurationParser::ParseLineStyle(lineStyles_horizontal.at(0));
                else if (nLineStyles_horizontal == nTLines_horizontal)
                    lineStyle_horizontal = GraphicsConfigurationParser::ParseLineStyle(lineStyles_horizontal.at(iLine));

                line_horizontal[iLine] = new TLine(xmin, TLines_horizontal.at(iLine), xmax, TLines_horizontal.at(iLine));
                line_horizontal[iLine]->SetLineStyle(lineStyle_horizontal);   // https://root.cern.ch/doc/master/TAttLine_8h.html#a7092c0c4616367016b70d54e5c680a69
                line_horizontal[iLine]->Draw();
            }
            // add TLine
            TLine* line_vertical[nTLines_vertical];
            for (int iLine = 0; iLine<nTLines_vertical; ++iLine) {
                // draw vertical line
                double ymin = h[i]->GetMinimum();
                double ymax = h[i]->GetMaximum();

                int lineStyle_vertical = GRAPHICS::lineStyle_vertical;
                if (nLineStyles_vertical == 1)
                    lineStyle_vertical = GraphicsConfigurationParser::ParseLineStyle(lineStyles_vertical.at(0));
                else if (nLineStyles_vertical == nTLines_vertical)
                    lineStyle_vertical = GraphicsConfigurationParser::ParseLineStyle(lineStyles_vertical.at(iLine));

                line_vertical[iLine] = new TLine(TLines_vertical.at(iLine), ymin, TLines_vertical.at(iLine), ymax);
                line_vertical[iLine]->SetLineStyle(lineStyle_vertical);   // https://root.cern.ch/doc/master/TAttLine_8h.html#a7092c0c4616367016b70d54e5c680a69
                line_vertical[iLine]->Draw();
            }

            // saving histogram
            std::string tmpOutputFile = outputFile.Data();
            if (tmpOutputFile.find(".") != std::string::npos) {     // file extension is specified
                if (nHistos > 1) {
                    // modify outputFile name
                    // if i=1, then "output.ext" becomes "output_2.ext"
                    size_t pos = tmpOutputFile.find_last_of(".");
                    tmpOutputFile.replace(pos,1, Form("_%d.", i+1));
                }
                c->SaveAs(tmpOutputFile.c_str());
            }
            else {  // file extension is NOT specified
                if (nHistos > 1) {
                    // modify outputFile name
                    // if i=1, then "output" becomes "output_2"
                    tmpOutputFile = Form("%s_%d", tmpOutputFile.c_str(), i+1);
                }
                c->SaveAs(Form("%s.C", tmpOutputFile.c_str()));
                c->SaveAs(Form("%s.png", tmpOutputFile.c_str()));
                c->SaveAs(Form("%s.pdf", tmpOutputFile.c_str()));
            }
            c->Close();         // do not use Delete() for TCanvas.
        }
    }
    else if (drawSame > 0) {
        // set canvas and pads
        c = new TCanvas("cnv","",windowWidth,windowHeight);
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        setCanvasFinal(c, setLogx, setLogy);
        c->cd();
        double windowHeightScale = 1;
        if (drawRatio > 0 || drawDiff > 0) {
            windowHeightScale = 1 + windowHeightFraction;

            pad2 = new TPad("pad2", "",0, 0, 1, windowHeightFraction);
            pad2->SetLeftMargin(leftMargin);
            pad2->SetRightMargin(rightMargin);
            setPadFinal(pad2, setLogx, 0);  // do not draw the y-axis in log scale for the ratio histogram.

            pad2->Draw();
            pad2->SetNumber(2);
        }
        c->SetCanvasSize(c->GetWw(), c->GetWh()*windowHeightScale);

        pad = new TPad("pad", "",0, windowHeightScale-1, 1, 1);
        pad->SetMargin(leftMargin, rightMargin, bottomMargin, topMargin);
        setPadFinal(pad, setLogx, setLogy);

        pad->Draw();
        pad->SetNumber(1);
        c->cd(1);
        // set canvas and pads - END

        TLegend* leg = new TLegend();

        // set maximum/minimum of y-axis
        if (yMin > yMax) {
            double histMin = getMinimumTH1s(h, nHistos);
            double histMax = getMaximumTH1s(h, nHistos);
            if (setLogy == 0) h[0]->SetMinimum(histMin-TMath::Abs(histMin)*0.1);
            h[0]->SetMaximum(histMax+TMath::Abs(histMax)*0.1*TMath::Power(10,setLogy));
        }

        for (int i = 0; (unsigned)i<indexDrawOptionHist.size(); ++i) {
            // draw first the histograms with drawOption = "hist" so that plots with markers are not overwritten.
            int iHist = indexDrawOptionHist.at(i);

            std::string drawOption = "";
            if (nDrawOptions == 1)  drawOption = drawOptions.at(0).c_str();
            else if (nDrawOptions == nHistos) drawOption = drawOptions.at(iHist).c_str();

            if (!hDrawn[iHist]) {
                 h[iHist]->Draw(Form("%s same", drawOption.c_str()));
                 hDrawn[iHist] = true;
             }
        }

        for (int i = 0; i<nHistos; ++i) {

            std::string drawOption = "";
            if (nDrawOptions == 1)  drawOption = drawOptions.at(0).c_str();
            else if (nDrawOptions == nHistos) drawOption = drawOptions.at(i).c_str();

            if (!hDrawn[i]) {
                h[i]->Draw(Form("%s same", drawOption.c_str()));
                hDrawn[i] = true;
            }

            std::string label = legendEntryLabels.at(0).c_str();
            if (nHistos == nLegendEntryLabels) label = legendEntryLabels.at(i).c_str();
            if (label.compare(CONFIGPARSER::nullInput) == 0)  continue;

            std::string legendOption = "lpf";
            if (drawOption.find("hist") != std::string::npos)  legendOption = "lf";
            leg->AddEntry(h[i], label.c_str(), legendOption.c_str());
        }

        if (legendTextSize != 0)  leg->SetTextSize(legendTextSize);
        leg->SetBorderSize(legendBorderSize);
        double height = calcTLegendHeight(leg);
        double width = calcTLegendWidth(leg);
        if (legendHeight != 0)  height = legendHeight;
        if (legendWidth != 0)  width = legendWidth;
        if (legendPosition.size() > 0) {    // draw the legend if really a position is provided.
            setLegendPosition(leg, legendPosition, c, height, width, legendOffsetX, legendOffsetY);
            leg->Draw();
        }

        // add Text
        TLatex* latex;
        if (nTextLines > 0) {
            latex = new TLatex();
            latex->SetTextFont(textFont);
            latex->SetTextSize(textSize);
            setTextAlignment(latex, textPosition);
            std::vector<std::pair<float,float>> textCoordinates = calcTextCoordinates(textLines, textPosition, c, textOffsetX, textOffsetY);
            for (int i = 0; i<nTextLines; ++i){
                float x = textCoordinates.at(i).first;
                float y = textCoordinates.at(i).second;
                latex->DrawLatexNDC(x, y, textLines.at(i).c_str());
            }
        }

        // add Text above the pad
        TLatex* latexOverPad;
        if (nTextsOverPad > 0) {
            latexOverPad = new TLatex();
            latexOverPad->SetTextFont(textAbovePadFont);
            latexOverPad->SetTextSize(textAbovePadSize);
            for (int i = 0; i < nTextsOverPad; ++i) {
                int textOverPadAlignment = GRAPHICS::textAlign;
                if (nTextsOverPadAlignments == 1) textOverPadAlignment = GraphicsConfigurationParser::ParseTextAlign(textsOverPadAlignments.at(0));
                else if (nTextsOverPadAlignments == nTextsOverPad) textOverPadAlignment = GraphicsConfigurationParser::ParseTextAlign(textsOverPadAlignments.at(i));

                latexOverPad->SetTextAlign(textOverPadAlignment);
                setTextAbovePad(latexOverPad, c, textAbovePadOffsetX, textAbovePadOffsetY);

                latexOverPad->DrawLatexNDC(latexOverPad->GetX(), latexOverPad->GetY(), textsOverPad.at(i).c_str());
            }
        }

        // add TLine
        TLine* line_horizontal[nTLines_horizontal];
        for (int i = 0; i<nTLines_horizontal; ++i) {
            if (nHistos > 0) {
                // draw horizontal line
                double xmin = h[0]->GetXaxis()->GetBinLowEdge(h[0]->GetXaxis()->GetFirst());
                double xmax = h[0]->GetXaxis()->GetBinLowEdge(h[0]->GetXaxis()->GetLast()+1);

                int lineStyle_horizontal = GRAPHICS::lineStyle_horizontal;
                if (nLineStyles_horizontal == 1)
                    lineStyle_horizontal = GraphicsConfigurationParser::ParseLineStyle(lineStyles_horizontal.at(0));
                else if (nLineStyles_horizontal == nTLines_horizontal)
                    lineStyle_horizontal = GraphicsConfigurationParser::ParseLineStyle(lineStyles_horizontal.at(i));

                line_horizontal[i] = new TLine(xmin, TLines_horizontal.at(i), xmax, TLines_horizontal.at(i));
                line_horizontal[i]->SetLineStyle(lineStyle_horizontal);   // https://root.cern.ch/doc/master/TAttLine_8h.html#a7092c0c4616367016b70d54e5c680a69
                line_horizontal[i]->Draw();
            }
        }
        // add TLine
        TLine* line_vertical[nTLines_vertical];
        for (int i = 0; i<nTLines_vertical; ++i) {
            if (nHistos > 0) {
                // draw vertical line
                double ymin = h[0]->GetMinimum();
                double ymax = h[0]->GetMaximum();

                int lineStyle_vertical = GRAPHICS::lineStyle_vertical;
                if (nLineStyles_vertical == 1)
                    lineStyle_vertical = GraphicsConfigurationParser::ParseLineStyle(lineStyles_vertical.at(0));
                else if (nLineStyles_vertical == nTLines_vertical)
                    lineStyle_vertical = GraphicsConfigurationParser::ParseLineStyle(lineStyles_vertical.at(i));

                line_vertical[i] = new TLine(TLines_vertical.at(i), ymin, TLines_vertical.at(i), ymax);
                line_vertical[i]->SetLineStyle(lineStyle_vertical);   // https://root.cern.ch/doc/master/TAttLine_8h.html#a7092c0c4616367016b70d54e5c680a69
                line_vertical[i]->Draw();
            }
        }

        if (drawRatio > 0 || drawDiff > 0) {
            c->cd(2);

            // ratio histograms
            int nHistos_lowerPad = 0;
            // ratio histograms will be the division of input histograms at even index by the histograms at odd index
            // if input histogram list has odd number of histograms then drawRatio step is skipped.
            // Ex. : TH1_paths = {h1, h2, h3, h4}
            //      ==> h_ratio[0] = h1/h2, h_ratio[1] = h3/h4
            if (nHistos % 2 == 0)   nHistos_lowerPad = nHistos/2;

            TH1D* h_lowerPad[nHistos_lowerPad];
            for (int i=0; i<nHistos_lowerPad; ++i) {

                h_lowerPad[i] = (TH1D*)h[2*i]->Clone(Form("%s_lowerPad", h[2*i]->GetName()));
                h_lowerPad[i]->Reset();    // start from scratch, reset the undesired properties inherited from the original histogram.
                h_lowerPad[i]->Add(h[2*i]);
                if (drawRatio > 0)  h_lowerPad[i]->Divide(h[2*i+1]);
                else if (drawDiff > 0)  h_lowerPad[i]->Add(h[2*i+1],-1);

                if (fitTH1_lowerPad > 0) {
                    std::string TF1_formula_lowerPad = "";
                    double TF1_xMin_lowerPad = 0;
                    double TF1_xMax_lowerPad = 0;
                    if (nTF1_formulas_lowerPad == 1)  {
                        TF1_formula_lowerPad = TF1_formulas_lowerPad.at(0).c_str();
                        TF1_xMin_lowerPad = TF1_ranges_lowerPad[0].at(0);
                        TF1_xMax_lowerPad = TF1_ranges_lowerPad[1].at(0);
                    }
                    else if (nTF1_formulas_lowerPad == nHistos_lowerPad)  {
                        TF1_formula_lowerPad = TF1_formulas_lowerPad.at(i).c_str();
                        TF1_xMin_lowerPad = TF1_ranges_lowerPad[0].at(i);
                        TF1_xMax_lowerPad = TF1_ranges_lowerPad[1].at(i);
                    }
                    std::string fitOption_lowerPad = INPUT_DEFAULT::fitOption.c_str();
                    if (nFitOptions_lowerPad == 1)  fitOption_lowerPad = fitOptions_lowerPad.at(0).c_str();
                    if (nFitOptions_lowerPad == nHistos_lowerPad)  fitOption_lowerPad = fitOptions_lowerPad.at(i).c_str();

                    int fitColor_lowerPad = INPUT_DEFAULT::fitColor;
                    if (nFitColors == 1) fitColor_lowerPad = GraphicsConfigurationParser::ParseColor(fitColors_lowerPad.at(0));
                    else if (nFitColors_lowerPad == nHistos_lowerPad) {
                        if (fitColors_lowerPad.at(i).compare(CONFIGPARSER::nullInput) != 0)
                            fitColor_lowerPad = GraphicsConfigurationParser::ParseColor(fitColors_lowerPad.at(i));
                    }

                    TF1 f1_lowerPad(Form("f1_%d", i), TF1_formula_lowerPad.c_str(), TF1_xMin_lowerPad, TF1_xMax_lowerPad);
                    f1_lowerPad.SetLineColor(fitColor_lowerPad);

                    h_lowerPad[i]->Fit(f1_lowerPad.GetName(), fitOption_lowerPad.c_str());
                }
            }

            for (int i=0; i<nHistos_lowerPad; ++i) {
                double axisSizeRatio = (c->GetPad(1)->GetAbsHNDC()/c->GetPad(2)->GetAbsHNDC());
                setTH1Ratio(h_lowerPad[i], h[2*i], axisSizeRatio);
                h_lowerPad[i]->SetYTitle(titleY_lowerPad.c_str());

                h_lowerPad[i]->SetMarkerColor(GRAPHICS::colors[i]);
                h_lowerPad[i]->Draw("e same");
            }
            // add TLine to the lower pad
            TLine* line_horizontal_lowerPad[nTLines_horizontal_lowerPad];
            for (int i = 0; i<nTLines_horizontal_lowerPad; ++i) {
                if (nHistos_lowerPad > 0) {
                    // draw horizontal line
                    double xmin = h_lowerPad[0]->GetXaxis()->GetBinLowEdge(h_lowerPad[0]->GetXaxis()->GetFirst());
                    double xmax = h_lowerPad[0]->GetXaxis()->GetBinLowEdge(h_lowerPad[0]->GetXaxis()->GetLast()+1);

                    int lineStyle_horizontal_lowerPad = GRAPHICS::lineStyle_horizontal;
                    if (nLineStyles_horizontal_lowerPad == 1)
                        lineStyle_horizontal_lowerPad = GraphicsConfigurationParser::ParseLineStyle(lineStyles_horizontal_lowerPad.at(0));
                    else if (nLineStyles_horizontal_lowerPad == nTLines_horizontal_lowerPad)
                        lineStyle_horizontal_lowerPad = GraphicsConfigurationParser::ParseLineStyle(lineStyles_horizontal_lowerPad.at(i));

                    line_horizontal_lowerPad[i] = new TLine(xmin, TLines_horizontal_lowerPad.at(i), xmax, TLines_horizontal_lowerPad.at(i));
                    line_horizontal_lowerPad[i]->SetLineStyle(lineStyle_horizontal_lowerPad);   // https://root.cern.ch/doc/master/TAttLine_8h.html#a7092c0c4616367016b70d54e5c680a69
                    line_horizontal_lowerPad[i]->Draw();
                }
            }
            // add TLine to the lower pad
            TLine* line_vertical_lowerPad[nTLines_vertical_lowerPad];
            double ymin_lowerPad = getMinimumTH1s(h_lowerPad, nHistos_lowerPad);
            double ymax_lowerPad = getMaximumTH1s(h_lowerPad, nHistos_lowerPad);
            for (int i = 0; i<nTLines_vertical_lowerPad; ++i) {
                if (nHistos_lowerPad > 0) {
                    // draw vertical line

                    int lineStyle_vertical_lowerPad = GRAPHICS::lineStyle_vertical;
                    if (nLineStyles_vertical_lowerPad == 1)
                        lineStyle_vertical_lowerPad = GraphicsConfigurationParser::ParseLineStyle(lineStyles_vertical_lowerPad.at(0));
                    else if (nLineStyles_vertical_lowerPad == nTLines_vertical_lowerPad)
                        lineStyle_vertical_lowerPad = GraphicsConfigurationParser::ParseLineStyle(lineStyles_vertical_lowerPad.at(i));

                    line_vertical_lowerPad[i] = new TLine(TLines_vertical_lowerPad.at(i), ymin_lowerPad, TLines_vertical_lowerPad.at(i), ymax_lowerPad);
                    line_vertical_lowerPad[i]->SetLineStyle(lineStyle_vertical_lowerPad);   // https://root.cern.ch/doc/master/TAttLine_8h.html#a7092c0c4616367016b70d54e5c680a69
                    line_vertical_lowerPad[i]->Draw();
                }
            }
        }
        c->cd(1);

        // saving histograms
        std::string tmpOutputFile = outputFile.Data();
        if (tmpOutputFile.find(".") != std::string::npos) {     // file extension is specified
            c->SaveAs(tmpOutputFile.c_str());
        }
        else {  // file extension is NOT specified

            c->SaveAs(Form("%s.C", tmpOutputFile.c_str()));
            c->SaveAs(Form("%s.png", tmpOutputFile.c_str()));
            c->SaveAs(Form("%s.pdf", tmpOutputFile.c_str()));
        }
        pad->Close();
        c->Close();
    }
}

/*
 * g++ Plotting/plotHistogram.C $(root-config --cflags --libs) -std=c++11 -Werror -Wall -O2 -o Plotting/plotHistogram.exe
 */
int main(int argc, char** argv)
{
    if (argc == 4) {
        plotHistogram(argv[1], argv[2], argv[3]);
        return 0;
    }
    else if (argc == 3) {
        plotHistogram(argv[1], argv[2]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./plotHistogram.exe <configFile> <inputFile> <outputFile>"
                << std::endl;
        return 1;
    }
}
