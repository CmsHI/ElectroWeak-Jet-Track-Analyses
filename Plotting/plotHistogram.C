/*
 * macro to save histograms as picture files.
 * saves histograms to a graphics file.
 * g++ Plotting/plotHistogram.C $(root-config --cflags --libs) -std=c++11 -Werror -Wall -O2 -o Plotting/plotHistogram.exe
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
// ROOT, for interactive graphics.
#include "TVirtualPad.h"
#include "TApplication.h"

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

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

    if (!configInput.isValid) {
        std::cout << "Input configuration is invalid." << std::endl;
        std::cout << "exiting" << std::endl;
        return;
    }
    if (!configCuts.isValid) {
        std::cout << "Cut configuration is invalid." << std::endl;
        std::cout << "exiting" << std::endl;
        return;
    }

    // input for TH1
    // paths of TH1D histograms
    std::vector<std::pair<std::string, int>> TH1_pathsANDindices = ConfigurationParser::ParseListOfList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_TH1_path]);
    std::vector<std::string> TH1_paths = ConfigurationParser::getVecString(TH1_pathsANDindices);
    std::vector<int> TH1_padIndices = ConfigurationParser::getVecIndex(TH1_pathsANDindices);
    std::vector<std::string> titles = ConfigurationParser::ParseListOrString(ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_TH1_title]));
    std::vector<std::string> titlesX = ConfigurationParser::ParseListOrString(ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_TH1_titleX]));
    std::vector<std::string> titlesY = ConfigurationParser::ParseListOrString(ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_TH1_titleY]));
    std::vector<float> titleOffsetsX = ConfigurationParser::ParseListOrFloat(configInput.proc[INPUT::kPLOTTING].str_f[INPUT::k_titleOffsetX]);
    std::vector<float> titleOffsetsY = ConfigurationParser::ParseListOrFloat(configInput.proc[INPUT::kPLOTTING].str_f[INPUT::k_titleOffsetY]);
    std::vector<int> centerTitleX = ConfigurationParser::ParseListOrInteger(configInput.proc[INPUT::kPLOTTING].str_i[INPUT::k_centerTitleX]);
    std::vector<int> centerTitleY = ConfigurationParser::ParseListOrInteger(configInput.proc[INPUT::kPLOTTING].str_i[INPUT::k_centerTitleY]);
    std::vector<float> TH1_scales = ConfigurationParser::ParseListFloat(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_TH1_scale]);
    std::vector<int> TH1_rebins = ConfigurationParser::ParseListInteger(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_TH1_rebin]);
    std::vector<float> TH1_norms = ConfigurationParser::ParseListFloat(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_TH1_norm]);
    std::vector<float> xMin = ConfigurationParser::ParseListOrFloat(configInput.proc[INPUT::kPLOTTING].str_f[INPUT::k_TH1_xMin]);
    std::vector<float> xMax = ConfigurationParser::ParseListOrFloat(configInput.proc[INPUT::kPLOTTING].str_f[INPUT::k_TH1_xMax]);
    std::vector<float> yMin = ConfigurationParser::ParseListOrFloat(configInput.proc[INPUT::kPLOTTING].str_f[INPUT::k_TH1_yMin]);
    std::vector<float> yMax = ConfigurationParser::ParseListOrFloat(configInput.proc[INPUT::kPLOTTING].str_f[INPUT::k_TH1_yMax]);
    std::vector<int> drawNormalized = ConfigurationParser::ParseListOrInteger(configInput.proc[INPUT::kPLOTTING].str_i[INPUT::k_drawNormalized]);
    std::vector<std::string> drawOptions = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_drawOption]);
    std::vector<float> markerSizes = ConfigurationParser::ParseListOrFloat(configInput.proc[INPUT::kPLOTTING].str_f[INPUT::k_markerSize]);
    std::vector<std::string> markerStyles = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_markerStyle]);
    std::vector<std::string> lineStyles = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_lineStyle]);
    std::vector<std::string> fillStyles = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_fillStyle]);
    std::vector<std::string> colors = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_color]);
    std::vector<std::string> fillColors = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_fillColor]);
    std::vector<std::string> lineColors = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_lineColor]);
    std::vector<int> lineWidths = ConfigurationParser::ParseListOrInteger(configInput.proc[INPUT::kPLOTTING].str_i[INPUT::k_lineWidth]);
    std::vector<int> fitTH1 = ConfigurationParser::ParseListOrInteger(configInput.proc[INPUT::kPLOTTING].str_i[INPUT::k_fitTH1]);

    // input for TF1
    std::vector<std::string> TF1_formulas = ConfigurationParser::ParseListTF1Formula(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_TF1]);
    std::vector<std::vector<double>> TF1_ranges = ConfigurationParser::ParseListTF1Range(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_TF1]);
    std::vector<std::string> fitOptions = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_fitOption]);
    std::vector<std::string> fitColors = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_fitColor]);

    // input for TLegend
    std::string tmpLegend = ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_legendEntryLabel]);
    std::vector<std::pair<std::string, int>> legendEntries = ConfigurationParser::ParseListOfList(tmpLegend);
    std::vector<std::string> legendEntryLabels = ConfigurationParser::getVecString(legendEntries);
    std::vector<int> legendEntryPadIndices = ConfigurationParser::getVecIndex(legendEntries);
    std::vector<std::string> legendPositions = ConfigurationParser::ParseListOrString(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_legendPosition]);
    std::vector<float> legendOffsetsX = ConfigurationParser::ParseListOrFloat(configInput.proc[INPUT::kPLOTTING].str_f[INPUT::k_legendOffsetX]);
    std::vector<float> legendOffsetsY = ConfigurationParser::ParseListOrFloat(configInput.proc[INPUT::kPLOTTING].str_f[INPUT::k_legendOffsetY]);
    std::vector<int> legendBorderSizes = ConfigurationParser::ParseListOrInteger(configInput.proc[INPUT::kPLOTTING].str_i[INPUT::k_legendBorderSize]);
    std::vector<float> legendWidths = ConfigurationParser::ParseListOrFloat(configInput.proc[INPUT::kPLOTTING].str_f[INPUT::k_legendWidth]);
    std::vector<float> legendHeights = ConfigurationParser::ParseListOrFloat(configInput.proc[INPUT::kPLOTTING].str_f[INPUT::k_legendHeight]);
    std::vector<float> legendTextSizes = ConfigurationParser::ParseListOrFloat(configInput.proc[INPUT::kPLOTTING].str_f[INPUT::k_legendTextSize]);

    // input for text objects
    std::string tmpText = ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_text]);
    std::vector<std::pair<std::string, int>> textEntries = ConfigurationParser::ParseListOfList(tmpText);
    std::vector<std::string> textLines = ConfigurationParser::getVecString(textEntries);
    std::vector<int> textLinePadIndices = ConfigurationParser::getVecIndex(textEntries);
    std::vector<std::string> textPositions = ConfigurationParser::ParseListOrString(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_textPosition]);
    std::vector<int> textFonts = ConfigurationParser::ParseListOrInteger(configInput.proc[INPUT::kPLOTTING].str_i[INPUT::k_textFont]);
    std::vector<float> textSizes = ConfigurationParser::ParseListOrFloat(configInput.proc[INPUT::kPLOTTING].str_f[INPUT::k_textSize]);
    std::vector<float> textOffsetsX = ConfigurationParser::ParseListOrFloat(configInput.proc[INPUT::kPLOTTING].str_f[INPUT::k_textOffsetX]);
    std::vector<float> textOffsetsY = ConfigurationParser::ParseListOrFloat(configInput.proc[INPUT::kPLOTTING].str_f[INPUT::k_textOffsetY]);

    std::string tmpTextAbovePad = ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_textAbovePad]);
    std::vector<std::pair<std::string, int>> textAbovePadEntries = ConfigurationParser::ParseListOfList(tmpTextAbovePad);
    std::vector<std::string> textsAbovePad = ConfigurationParser::getVecString(textAbovePadEntries);
    std::vector<int> textsAbovePadPadIndices = ConfigurationParser::getVecIndex(textAbovePadEntries);
    std::vector<std::string> textsAbovePadAlignments = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_textAbovePadAlign]);
    std::vector<int> textAbovePadFonts = ConfigurationParser::ParseListOrInteger(configInput.proc[INPUT::kPLOTTING].str_i[INPUT::k_textAbovePadFont]);
    std::vector<float> textAbovePadSizes = ConfigurationParser::ParseListOrFloat(configInput.proc[INPUT::kPLOTTING].str_f[INPUT::k_textAbovePadSize]);
    std::vector<float> textAbovePadOffsetsX = ConfigurationParser::ParseListOrFloat(configInput.proc[INPUT::kPLOTTING].str_f[INPUT::k_textAbovePadOffsetX]);
    std::vector<float> textAbovePadOffsetsY = ConfigurationParser::ParseListOrFloat(configInput.proc[INPUT::kPLOTTING].str_f[INPUT::k_textAbovePadOffsetY]);

    // input for TLine
    // y-axis positions of the horizontal lines to be drawn
    std::vector<std::pair<float, int>> TLines_horizontal_Entries = ConfigurationParser::ParseListOfListFloat(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_TLine_horizontal]);
    std::vector<float> TLines_horizontal = ConfigurationParser::getVecFloat(TLines_horizontal_Entries);
    std::vector<int> TLines_horizontal_PadIndices = ConfigurationParser::getVecIndex(TLines_horizontal_Entries);
    std::vector<std::string> lineStyles_horizontal = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_LineStyle_horizontal]);
    // x-axis positions of the vertical lines to be drawn
    std::vector<std::pair<float, int>> TLines_verticalEntries = ConfigurationParser::ParseListOfListFloat(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_TLine_vertical]);
    std::vector<float> TLines_vertical = ConfigurationParser::getVecFloat(TLines_verticalEntries);
    std::vector<int> TLines_vertical_PadIndices = ConfigurationParser::getVecIndex(TLines_verticalEntries);
    std::vector<std::string> lineStyles_vertical = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_LineStyle_vertical]);

    // input for lower pad
    std::vector<int> drawRatio = ConfigurationParser::ParseListOrInteger(configInput.proc[INPUT::kPLOTTING].str_i[INPUT::k_drawRatio]);
    std::vector<int> drawDiff  = ConfigurationParser::ParseListOrInteger(configInput.proc[INPUT::kPLOTTING].str_i[INPUT::k_drawDiff]);
    std::vector<float> windowHeightFractions = ConfigurationParser::ParseListOrFloat(configInput.proc[INPUT::kPLOTTING].str_f[INPUT::k_windowHeightFraction]);
    std::vector<std::string> titlesY_lowerPad = ConfigurationParser::ParseListOrString(ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_TH1_titleY_lowerPad]));
    std::vector<float> yMin_lowerPad = ConfigurationParser::ParseListOrFloat(configInput.proc[INPUT::kPLOTTING].str_f[INPUT::k_TH1_yMin_lowerPad]);
    std::vector<float> yMax_lowerPad = ConfigurationParser::ParseListOrFloat(configInput.proc[INPUT::kPLOTTING].str_f[INPUT::k_TH1_yMax_lowerPad]);
    // y-axis positions of the horizontal lines to be drawn in the lower pad
    std::vector<std::pair<float, int>> TLines_horizontal_lowerPad_Entries = ConfigurationParser::ParseListOfListFloat(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_TLine_horizontal_lowerPad]);
    std::vector<float> TLines_horizontal_lowerPad = ConfigurationParser::getVecFloat(TLines_horizontal_lowerPad_Entries);
    std::vector<int> TLines_horizontal_lowerPad_PadIndices = ConfigurationParser::getVecIndex(TLines_horizontal_lowerPad_Entries);
    std::vector<std::string> lineStyles_horizontal_lowerPad = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_LineStyle_horizontal_lowerPad]);
    // x-axis positions of the vertical lines to be drawn in the lower pad
    std::vector<std::pair<float, int>> TLines_vertical_lowerPad_Entries = ConfigurationParser::ParseListOfListFloat(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_TLine_vertical_lowerPad]);
    std::vector<float> TLines_vertical_lowerPad = ConfigurationParser::getVecFloat(TLines_vertical_lowerPad_Entries);
    std::vector<int> TLines_vertical_lowerPad_PadIndices = ConfigurationParser::getVecIndex(TLines_vertical_lowerPad_Entries);
    std::vector<std::string> lineStyles_vertical_lowerPad = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_LineStyle_vertical_lowerPad]);
    // input for TF1 in lowerPad
    std::vector<int> fitTH1_lowerPad = ConfigurationParser::ParseListOrInteger(configInput.proc[INPUT::kPLOTTING].str_i[INPUT::k_fitTH1_lowerPad]);
    // input for TF1 in lowerPad
    std::string tmp_TF1_lowerPad = ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_TF1_lowerPad]);
    std::vector<std::string> TF1_formulas_lowerPad = ConfigurationParser::ParseListTF1Formula(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_TF1_lowerPad]);
    std::vector<std::vector<double>> TF1_ranges_lowerPad = ConfigurationParser::ParseListTF1Range(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_TF1_lowerPad]);
    std::vector<int> TF1_lowerPad_Indices = fitTH1_lowerPad;
    std::vector<std::string> fitOptions_lowerPad = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_fitOption_lowerPad]);
    std::vector<std::string> fitColors_lowerPad = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_fitColor_lowerPad]);

    // input for TCanvas
    int windowWidth = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_windowWidth];
    int windowHeight = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_windowHeight];
    float frameWidth = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_frameWidth];
    float frameHeight = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_frameHeight];
    float leftMargin = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_leftMargin];
    float rightMargin = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_rightMargin];
    float bottomMargin = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_bottomMargin];
    float topMargin = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_topMargin];
    float xMargin = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_xMargin];
    float yMargin = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_yMargin];
    int rows = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_rows];
    int columns = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_columns];
    std::vector<int> setLogx = ConfigurationParser::ParseListOrInteger(configInput.proc[INPUT::kPLOTTING].str_i[INPUT::k_setLogx]);
    std::vector<int> setLogy = ConfigurationParser::ParseListOrInteger(configInput.proc[INPUT::kPLOTTING].str_i[INPUT::k_setLogy]);

    // set default values
    if (titleOffsetsX.size() == 0) titleOffsetsX = {INPUT_DEFAULT::titleOffsetX};
    if (titleOffsetsY.size() == 0) titleOffsetsY = {INPUT_DEFAULT::titleOffsetY};

    if (drawNormalized.size() == 0)  drawNormalized = {0};
    if (markerSizes.size() == 0)  markerSizes = {INPUT_DEFAULT::markerSize};
    if (lineWidths.size() == 0)  lineWidths = {INPUT_DEFAULT::lineWidth};
    if (fitTH1.size() == 0)  fitTH1 = {0};

    if (textFonts.size() == 0)  textFonts = {INPUT_DEFAULT::textFont};
    if (textSizes.size() == 0)  textSizes = {INPUT_DEFAULT::textSize};

    if (textOffsetsX.size() == 0)  textOffsetsX = {INPUT_DEFAULT::textOffsetX};
    if (textOffsetsY.size() == 0)  textOffsetsY = {INPUT_DEFAULT::textOffsetY};

    if (textAbovePadFonts.size() == 0)  textAbovePadFonts = {INPUT_DEFAULT::textAbovePadFont};
    if (textAbovePadSizes.size() == 0)  textAbovePadSizes = {INPUT_DEFAULT::textAbovePadSize};

    if (textAbovePadOffsetsX.size() == 0)  textAbovePadOffsetsX = {INPUT_DEFAULT::textAbovePadOffsetX};
    if (textAbovePadOffsetsY.size() == 0)  textAbovePadOffsetsY = {INPUT_DEFAULT::textAbovePadOffsetY};

    if (drawRatio.size() == 0)  drawRatio = {0};
    if (drawDiff.size() == 0)  drawDiff = {0};
    if (windowHeightFractions.size() == 0)  windowHeightFractions = {INPUT_DEFAULT::windowHeightFraction};

    if (fitTH1_lowerPad.size() == 0)  fitTH1_lowerPad = {0};

    if (windowWidth  == 0)  windowWidth = INPUT_DEFAULT::windowWidth;
    if (windowHeight == 0)  windowHeight = INPUT_DEFAULT::windowHeight;
    if (frameWidth  == 0)  frameWidth = INPUT_DEFAULT::frameWidth;
    if (frameHeight == 0)  frameHeight = INPUT_DEFAULT::frameHeight;
    if (leftMargin == 0) leftMargin = INPUT_DEFAULT::leftMargin;
    if (rightMargin == 0) rightMargin = INPUT_DEFAULT::rightMargin;
    if (bottomMargin == 0) bottomMargin = INPUT_DEFAULT::bottomMargin;
    if (topMargin == 0) topMargin = INPUT_DEFAULT::topMargin;
    if (xMargin == 0) xMargin = INPUT_DEFAULT::xMargin;
    if (yMargin == 0) yMargin = INPUT_DEFAULT::yMargin;
    if (rows == 0) rows = INPUT_DEFAULT::rows;
    if (columns == 0) columns = INPUT_DEFAULT::columns;
    if (setLogx.size() == 0)  setLogx = {0};
    int nSetLogx = setLogx.size();
    if (setLogy.size() == 0)  setLogy = {0};
    int nSetLogy = setLogy.size();

    // xMin, xMax
    if (xMin.size() == 0)  xMin = {0};
    int nxMin = xMin.size();
    if (xMax.size() == 0)  xMax = {0};
    int nxMax = xMax.size();
    if (nxMin != nxMax && nxMax > 1 && nxMin > 1) {
        std::cout << "mismatch between the lengths of xMin and xMax lists" << std::endl;
        std::cout << "exiting" << std::endl;
        return;
    }
    int nxMinMax = nxMin;
    if (nxMax > nxMinMax) nxMinMax = nxMax;
    for (int i = 0; i < nxMinMax; ++i) {

        float xMinTmp = xMin.at(0);
        if (nxMin > 1) xMinTmp = xMin.at(i);

        float xMaxTmp = xMax.at(0);
        if (nxMax > 1) xMaxTmp = xMax.at(i);

        int setLogxTmp = setLogx.at(0);
        if (nSetLogx == nxMin && nxMin > 1) setLogxTmp = setLogx.at(i);
        if (xMinTmp == 0 && xMaxTmp == 0)  xMaxTmp = -1;
        else if (xMinTmp <= 0 && setLogxTmp > 0)  xMinTmp = resetTH1axisMin4LogScale(xMinTmp, "x");

        if (nxMin == 1) xMin.at(0) = xMinTmp;
        else if (nxMin > 1) xMin.at(i) = xMinTmp;
        if (nxMax == 1) xMax.at(0) = xMaxTmp;
        else if (nxMax > 1) xMax.at(i) = xMaxTmp;
    }
    // yMin, yMax
    if (yMin.size() == 0)  yMin = {0};
    int nyMin = yMin.size();
    if (yMax.size() == 0)  yMax = {0};
    int nyMax = yMax.size();
    if (nyMin != nyMax && nyMax > 1 && nyMin > 1) {
        std::cout << "mismatch between the lengths of yMin and yMax lists" << std::endl;
        std::cout << "exiting" << std::endl;
        return;
    }
    int nyMinMax = nyMin;
    if (nyMax > nyMinMax) nyMinMax = nyMax;
    for (int i = 0; i < nyMinMax; ++i) {

        float yMinTmp = yMin.at(0);
        if (nyMin > 1) yMinTmp = yMin.at(i);

        float yMaxTmp = yMax.at(0);
        if (nyMax > 1) yMaxTmp = yMax.at(i);

        int setLogyTmp = setLogy.at(0);
        if (nSetLogy == nyMin && nyMin > 1) setLogyTmp = setLogy.at(i);
        if (yMinTmp == 0 && yMaxTmp == 0)  yMaxTmp = -1;
        else if (yMinTmp <= 0 && setLogyTmp > 0)  yMinTmp = resetTH1axisMin4LogScale(yMinTmp, "y");

        if (nyMin == 1) yMin.at(0) = yMinTmp;
        else if (nyMin > 1) yMin.at(i) = yMinTmp;
        if (nyMax == 1) yMax.at(0) = yMaxTmp;
        else if (nyMax > 1) yMax.at(i) = yMaxTmp;
    }
    // yMin, yMax for lowerPad
    if (yMin_lowerPad.size() == 0)  yMin_lowerPad = {0};
    int nyMin_lowerPad = yMin_lowerPad.size();
    if (yMax_lowerPad.size() == 0)  yMax_lowerPad = {0};
    int nyMax_lowerPad = yMax_lowerPad.size();
    if (nyMin_lowerPad != nyMax_lowerPad && nyMax_lowerPad > 1 && nyMin_lowerPad > 1) {
        std::cout << "mismatch between the lengths of yMin_lowerPad and yMax_lowerPad lists" << std::endl;
        std::cout << "exiting" << std::endl;
        return;
    }
    int nyMin_lowerPadMax = nyMin_lowerPad;
    if (nyMax_lowerPad > nyMin_lowerPadMax) nyMin_lowerPadMax = nyMax_lowerPad;
    for (int i = 0; i < nyMin_lowerPadMax; ++i) {

        float yMin_lowerPadTmp = yMin_lowerPad.at(0);
        if (nyMin_lowerPad > 1) yMin_lowerPadTmp = yMin_lowerPad.at(i);

        float yMax_lowerPadTmp = yMax_lowerPad.at(0);
        if (nyMax_lowerPad > 1) yMax_lowerPadTmp = yMax_lowerPad.at(i);

        if (yMin_lowerPadTmp == 0 && yMax_lowerPadTmp == 0)  yMax_lowerPadTmp = -1;

        if (nyMin_lowerPad == 1) yMin_lowerPad.at(0) = yMin_lowerPadTmp;
        else if (nyMin_lowerPad > 1) yMin_lowerPad.at(i) = yMin_lowerPadTmp;
        if (nyMax_lowerPad == 1) yMax_lowerPadTmp = yMax_lowerPad.at(0);
        else if (nyMax_lowerPad > 1) yMax_lowerPadTmp = yMax_lowerPad.at(i);
    }

    int nPads = rows*columns;
    int nTH1_Paths = TH1_paths.size();
    int nTitles = titles.size();
    int nTitlesX = titlesX.size();
    int nTitlesY = titlesY.size();
    int nTitleOffsetX = titleOffsetsX.size();
    int nTitleOffsetY = titleOffsetsY.size();
    int nCenterTitleX = centerTitleX.size();
    int nCenterTitleY = centerTitleY.size();
    int nTH1_scales = TH1_scales.size();
    int nTH1_rebins = TH1_rebins.size();
    int nTH1_norms = TH1_norms.size();
    int nTF1_formulas = TF1_formulas.size();    // assume TF1_formulas.size() = TF1_ranges[0].size()
    int nFitOptions = fitOptions.size();
    int nFitColors = fitColors.size();
    int nDrawNormalized = drawNormalized.size();
    int nDrawOptions = drawOptions.size();
    int nMarkerSizes = markerSizes.size();
    int nMarkerStyles = markerStyles.size();
    int nLineStyles = lineStyles.size();
    int nFillStyles = fillStyles.size();
    int nColors = colors.size();
    int nFillColors = fillColors.size();
    int nLineColors = lineColors.size();
    int nLineWidths = lineWidths.size();
    int nFitTH1 = fitTH1.size();
    int nLegendEntryLabels = legendEntryLabels.size();
    int nLegendPositions = legendPositions.size();
    int nLegendOffsetsX = legendOffsetsX.size();
    int nLegendOffsetsY = legendOffsetsY.size();
    int nLegendBorderSizes = legendBorderSizes.size();
    int nLegendWidths = legendWidths.size();
    int nLegendHeights = legendHeights.size();
    int nLegendTextSizes = legendTextSizes.size();
    int nTextLines = textLines.size();
    int nTextPositions = textPositions.size();
    int nTextFonts = textFonts.size();
    int nTextSizes = textSizes.size();
    int nTextOffsetsX = textOffsetsX.size();
    int nTextOffsetsY = textOffsetsY.size();
    int nTextsAbovePad = textsAbovePad.size();
    int nTextsAbovePadAlignments = textsAbovePadAlignments.size();
    int nTextAbovePadFonts = textAbovePadFonts.size();
    int nTextAbovePadSizes = textAbovePadSizes.size();
    int nTextAbovePadOffsetsX = textAbovePadOffsetsX.size();
    int nTextAbovePadOffsetsY = textAbovePadOffsetsY.size();
    int nTLines_horizontal = TLines_horizontal.size();
    int nLineStyles_horizontal = lineStyles_horizontal.size();
    int nTLines_vertical = TLines_vertical.size();
    int nLineStyles_vertical = lineStyles_vertical.size();
    int nDrawRatio = drawRatio.size();
    int nDrawDiff = drawDiff.size();
    int nWindowHeightFractions = windowHeightFractions.size();
    int nTitlesY_lowerPad = titlesY_lowerPad.size();
    int nTLines_horizontal_lowerPad = TLines_horizontal_lowerPad.size();
    int nLineStyles_horizontal_lowerPad = lineStyles_horizontal_lowerPad.size();
    int nTLines_vertical_lowerPad = TLines_vertical_lowerPad.size();
    int nLineStyles_vertical_lowerPad = lineStyles_vertical_lowerPad.size();
    int nFitTH1_lowerPad = fitTH1_lowerPad.size();
    int nTF1_formulas_lowerPad = TF1_formulas_lowerPad.size();    // assume TF1_formulas.size() = TF1_ranges[0].size()
    int nFitOptions_lowerPad = fitOptions_lowerPad.size();
    int nFitColors_lowerPad = fitColors_lowerPad.size();

    // verbose about input configuration
    std::cout<<"Input Configuration :"<<std::endl;
    std::cout << "nTH1_Paths = " << nTH1_Paths << std::endl;
    for (int i = 0; i<nTH1_Paths; ++i) {
            std::cout << Form("TH1_Path[%d] = %s", i, TH1_paths.at(i).c_str()) << std::endl;
    }
    std::vector<int> TH1s_perPad;
    for (int iPad = 0; iPad < nPads; ++iPad) {
        int nTH1_tmp = 0;
        for (int i = 0; i<nTH1_Paths; ++i) {
            if (TH1_padIndices.at(i) == iPad) nTH1_tmp++;
        }
        TH1s_perPad.push_back(nTH1_tmp);
    }
    int nTH1s_perPad = TH1s_perPad.size();
    if (nPads > 1) {
        for (int i = 0; i<nTH1_Paths; ++i) {
            std::cout << Form("TH1_padIndices[%d] = %d", i, TH1_padIndices.at(i)) << std::endl;
        }
        for (int i = 0; i<nTH1s_perPad; ++i) {
            std::cout << Form("TH1s_perPad[%d] = %d", i, TH1s_perPad.at(i)) << std::endl;
        }
    }
    std::cout << "nTitles = " << nTitles << std::endl;
    for (int i = 0; i<nTitles; ++i) {
        std::cout << Form("titles[%d] = %s", i, titles.at(i).c_str()) << std::endl;
    }
    std::cout << "nTitlesX = " << nTitlesX << std::endl;
    for (int i = 0; i<nTitlesX; ++i) {
        std::cout << Form("titlesX[%d] = %s", i, titlesX.at(i).c_str()) << std::endl;
    }
    std::cout << "nTitlesY = " << nTitlesY << std::endl;
    for (int i = 0; i<nTitlesY; ++i) {
        std::cout << Form("titlesY[%d] = %s", i, titlesY.at(i).c_str()) << std::endl;
    }
    std::cout << "nTitleOffsetX = " << nTitleOffsetX << std::endl;
    for (int i = 0; i<nTitleOffsetX; ++i) {
        std::cout << Form("titleOffsetsX[%d] = %f", i, titleOffsetsX.at(i)) << std::endl;
    }
    std::cout << "nTitleOffsetY = " << nTitleOffsetY << std::endl;
    for (int i = 0; i<nTitleOffsetY; ++i) {
        std::cout << Form("titleOffsetsY[%d] = %f", i, titleOffsetsY.at(i)) << std::endl;
    }
    std::cout << "nCenterTitleX = " << nCenterTitleX << std::endl;
    for (int i = 0; i<nCenterTitleX; ++i) {
        std::cout << Form("centerTitleX[%d] = %d", i, centerTitleX.at(i)) << std::endl;
    }
    std::cout << "nCenterTitleY = " << nCenterTitleY << std::endl;
    for (int i = 0; i<nCenterTitleY; ++i) {
        std::cout << Form("centerTitleY[%d] = %d", i, centerTitleY.at(i)) << std::endl;
    }
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
    std::cout << "nxMin = " << nxMin << std::endl;
    std::cout << "nxMax = " << nxMax << std::endl;
    for (int i = 0; i < nxMinMax; ++i) {
        float xMinTmp = xMin.at(0);
        if (nxMin > 1) xMinTmp = xMin.at(i);
        float xMaxTmp = xMax.at(0);
        if (nxMax > 1) xMaxTmp = xMax.at(i);
        std::cout << Form("(xmin, xmax)[%d] = (%f, %f)", i, xMinTmp, xMaxTmp) << std::endl;
    }
    std::cout << "nyMin = " << nyMin << std::endl;
    std::cout << "nyMax = " << nyMax << std::endl;
    for (int i = 0; i < nyMinMax; ++i) {
        float yMinTmp = yMin.at(0);
        if (nyMin > 1) yMinTmp = yMin.at(i);
        float yMaxTmp = yMax.at(0);
        if (nyMax > 1) yMaxTmp = yMax.at(i);
        std::cout << Form("(yMin, yMax)[%d] = (%f, %f)", i, yMinTmp, yMaxTmp) << std::endl;
    }
    std::cout << "nDrawNormalized = " << nDrawNormalized << std::endl;
    for (int i = 0; i<nDrawNormalized; ++i) {
            std::cout << Form("drawNormalized[%d] = %d", i, drawNormalized.at(i)) << std::endl;
    }
    std::cout << "nDrawOptions   = " << nDrawOptions << std::endl;
    for (int i = 0; i<nDrawOptions; ++i) {
            std::cout << Form("drawOptions[%d] = %s", i, drawOptions.at(i).c_str()) << std::endl;
    }
    std::cout << "nMarkerSizes  = " << nMarkerSizes << std::endl;
    for (int i = 0; i<nMarkerSizes; ++i) {
            std::cout << Form("markerSizes[%d] = %f", i, markerSizes.at(i)) << std::endl;
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
    std::cout << "nLineWidths = " << nLineWidths << std::endl;
    for (int i = 0; i<nLineWidths; ++i) {
            std::cout << Form("lineWidths[%d] = %d", i, lineWidths.at(i)) << std::endl;
    }

    std::cout << "nFitTH1 = " << nFitTH1 << std::endl;
    bool dofitTH1 = false;
    for (int i = 0; i<nFitTH1; ++i) {
        std::cout << Form("fitTH1[%d] = %d", i, fitTH1.at(i)) << std::endl;
        if (fitTH1.at(i) > 0)  dofitTH1 = true;
    }
    if (dofitTH1) {
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
    for (int i = 0; i<nLegendEntryLabels; ++i) {
            std::cout << Form("legendEntryPadIndices[%d] = %d", i, legendEntryPadIndices.at(i)) << std::endl;
    }
    if (nLegendEntryLabels > 0) {
        std::cout << "nLegendPositions    = " << nLegendPositions << std::endl;
        if (nLegendPositions == 0) std::cout<< "No position is provided, legend will not be drawn." <<std::endl;
        for (int i = 0; i < nLegendPositions; ++i) {
            std::cout << Form("legendPositions[%d] = %s", i, legendPositions.at(i).c_str()) << std::endl;
        }
        std::cout << "nLegendOffsetsX = " << nLegendOffsetsX << std::endl;
        for (int i = 0; i < nLegendOffsetsX; ++i) {
            std::cout << Form("legendOffsetsX[%d] = %f", i, legendOffsetsX.at(i)) << std::endl;
        }
        std::cout << "nLegendOffsetsY = " << nLegendOffsetsY << std::endl;
        for (int i = 0; i < nLegendOffsetsY; ++i) {
            std::cout << Form("legendOffsetsY[%d] = %f", i, legendOffsetsY.at(i)) << std::endl;
        }
        std::cout << "nLegendBorderSizes = " << nLegendBorderSizes << std::endl;
        for (int i = 0; i < nLegendBorderSizes; ++i) {
            std::cout << Form("legendBorderSizes[%d] = %d", i, legendBorderSizes.at(i)) << std::endl;
        }
        std::cout << "nLegendWidths = " << nLegendWidths << std::endl;
        for (int i = 0; i < nLegendWidths; ++i) {
            std::cout << Form("legendWidths[%d] = %f", i, legendWidths.at(i)) << std::endl;
        }
        std::cout << "nLegendHeights = " << nLegendHeights << std::endl;
        for (int i = 0; i < nLegendHeights; ++i) {
            std::cout << Form("legendHeights[%d] = %f", i, legendHeights.at(i)) << std::endl;
        }
        std::cout << "nLegendTextSizes = " << nLegendTextSizes << std::endl;
        for (int i = 0; i < nLegendTextSizes; ++i) {
            std::cout << Form("legendTextSizes[%d] = %f", i, legendTextSizes.at(i)) << std::endl;
        }
    }

    std::cout << "nTextLines   = " << nTextLines << std::endl;
    for (int i = 0; i<nTextLines; ++i) {
            std::cout << Form("textLines[%d] = %s", i, textLines.at(i).c_str()) << std::endl;
    }
    if (nTextLines > 0) {

        std::cout << "nTextPositions = " << nTextPositions << std::endl;
        for (int i = 0; i<nTextPositions; ++i) {
                std::cout << Form("textPositions[%d] = %s", i, textPositions.at(i).c_str()) << std::endl;
        }
        std::cout << "nTextFonts = " << nTextFonts << std::endl;
        for (int i = 0; i<nTextFonts; ++i) {
                std::cout << Form("textFonts[%d] = %d", i, textFonts.at(i)) << std::endl;
        }
        std::cout << "nTextSizes = " << nTextSizes << std::endl;
        for (int i = 0; i<nTextSizes; ++i) {
                std::cout << Form("textSizes[%d] = %f", i, textSizes.at(i)) << std::endl;
        }
        std::cout << "nTextOffsetsX = " << nTextOffsetsX << std::endl;
        for (int i = 0; i<nTextOffsetsX; ++i) {
                std::cout << Form("textOffsetsX[%d] = %f", i, textOffsetsX.at(i)) << std::endl;
        }
        std::cout << "nTextOffsetsY = " << nTextOffsetsY << std::endl;
        for (int i = 0; i<nTextOffsetsY; ++i) {
                std::cout << Form("textOffsetsY[%d] = %f", i, textOffsetsY.at(i)) << std::endl;
        }
    }

    std::cout << "nTextsAbovePad = " << nTextsAbovePad << std::endl;
    for (int i = 0; i<nTextsAbovePad; ++i) {
            std::cout << Form("textsAbovePad[%d] = %s", i, textsAbovePad.at(i).c_str()) << std::endl;
    }
    if (nTextsAbovePad > 0) {
        std::cout << "nTextsAbovePadAlignments = " << nTextsAbovePadAlignments << std::endl;
        for (int i = 0; i<nTextsAbovePadAlignments; ++i) {
                std::cout << Form("textsAbovePadAlignments[%d] = %s", i, textsAbovePadAlignments.at(i).c_str()) << std::endl;
        }
        std::cout << "nTextAbovePadFonts = " << nTextAbovePadFonts << std::endl;
        for (int i = 0; i<nTextAbovePadFonts; ++i) {
                std::cout << Form("textAbovePadFonts[%d] = %d", i, textAbovePadFonts.at(i)) << std::endl;
        }
        std::cout << "nTextAbovePadSizes = " << nTextAbovePadSizes << std::endl;
        for (int i = 0; i<nTextAbovePadSizes; ++i) {
                std::cout << Form("textAbovePadSizes[%d] = %f", i, textAbovePadSizes.at(i)) << std::endl;
        }
        std::cout << "nTextAbovePadOffsetsX = " << nTextAbovePadOffsetsX << std::endl;
        for (int i = 0; i<nTextAbovePadOffsetsX; ++i) {
                std::cout << Form("textAbovePadOffsetsX[%d] = %f", i, textAbovePadOffsetsX.at(i)) << std::endl;
        }
        std::cout << "nTextAbovePadOffsetsY = " << nTextAbovePadOffsetsY << std::endl;
        for (int i = 0; i<nTextAbovePadOffsetsY; ++i) {
                std::cout << Form("textAbovePadOffsetsY[%d] = %f", i, textAbovePadOffsetsY.at(i)) << std::endl;
        }
    }

    std::cout << "nTLines_horizontal = " << nTLines_horizontal << std::endl;
    for (int i = 0; i<nTLines_horizontal; ++i) {
            std::cout << Form("TLines_horizontal[%d] = %f", i, TLines_horizontal.at(i)) << std::endl;
    }
    if (nTLines_horizontal > 0) {
        if (nPads > 1) {
            for (int i = 0; i < nTLines_horizontal; ++i) {
                    std::cout << Form("TLines_horizontal_PadIndices[%d] = %d", i, TLines_horizontal_PadIndices.at(i)) << std::endl;
            }
        }
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
        if (nPads > 1) {
            for (int i = 0; i < nTLines_vertical; ++i) {
                    std::cout << Form("TLines_vertical_PadIndices[%d] = %d", i, TLines_vertical_PadIndices.at(i)) << std::endl;
            }
        }
        std::cout << "nLineStyles_vertical = " << nLineStyles_vertical << std::endl;
        for (int i = 0; i<nLineStyles_vertical; ++i) {
            std::cout << Form("lineStyles_vertical[%d] = %s", i, lineStyles_vertical.at(i).c_str()) << std::endl;
        }
    }

    std::cout << "nDrawRatio" << nDrawRatio << std::endl;
    bool doDrawRatio = false;
    for (int i = 0; i < nDrawRatio; ++i) {
        std::cout << Form("drawRatio[%d] = %d", i, drawRatio.at(i)) << std::endl;
        if (drawRatio.at(i) > 0) doDrawRatio = true;
    }
    std::cout << "nDrawDiff" << nDrawDiff << std::endl;
    bool doDrawDiff = false;
    for (int i = 0; i < nDrawDiff; ++i) {
        std::cout << Form("drawDiff[%d] = %d", i, drawDiff.at(i)) << std::endl;
        if (drawDiff.at(i) > 0) doDrawDiff = true;
    }
    if (doDrawRatio || doDrawDiff) {
        bool conflictDrawRatioDiff = false;
        if (nDrawRatio == nDrawDiff) {
            for (int i = 0; i < nDrawRatio; ++i) {
                if (drawRatio.at(i) > 0 && drawDiff.at(i) > 0)  conflictDrawRatioDiff = true;
            }
        }
        else if (nDrawRatio == 1 && drawRatio.at(0) > 0) {
            for (int i = 0; i < nDrawDiff; ++i) {
                if (drawDiff.at(i) > 0)  conflictDrawRatioDiff = true;
            }
        }
        else if (nDrawDiff == 1 && drawDiff.at(0) > 0) {
            for (int i = 0; i < nDrawRatio; ++i) {
                if (drawRatio.at(i) > 0)  conflictDrawRatioDiff = true;
            }
        }
        if (conflictDrawRatioDiff) {
            std::cout << "both drawRatio and drawDiff are set for the same pad. can set at most one of them at a time." << std::endl;
            std::cout << "exiting" << std::endl;
            return;
        }

        std::cout << "nWindowHeightFractions = " << nWindowHeightFractions << std::endl;
        for (int i = 0; i < nWindowHeightFractions; ++i) {
            std::cout << Form("windowHeightFractions[%d] = %f", i, windowHeightFractions.at(i)) << std::endl;
        }
        std::cout << "nTitlesY_lowerPad = " << nTitlesY_lowerPad << std::endl;
        for (int i = 0; i < nTitlesY_lowerPad; ++i) {
            std::cout << Form("titlesY_lowerPad[%d] = %s", i, titlesY_lowerPad.at(i).c_str()) << std::endl;
        }
        std::cout << "nyMin_lowerPad = " << nyMin_lowerPad << std::endl;
        std::cout << "nyMax_lowerPad = " << nyMax_lowerPad << std::endl;
        for (int i = 0; i < nyMin_lowerPadMax; ++i) {
            float yMin_lowerPadTmp = yMin_lowerPad.at(0);
            if (nyMin_lowerPad > 1) yMin_lowerPadTmp = yMin_lowerPad.at(i);
            float yMax_lowerPadTmp = yMax_lowerPad.at(0);
            if (nyMax_lowerPad > 1) yMax_lowerPadTmp = yMax_lowerPad.at(i);
            std::cout << Form("(yMin_lowerPad, yMax_lowerPad)[%d] = (%f, %f)", i, yMin_lowerPadTmp, yMax_lowerPadTmp) << std::endl;
        }
        // lower pad objects
        std::cout << "nTLines_horizontal_lowerPad = " << nTLines_horizontal_lowerPad << std::endl;
        for (int i = 0; i<nTLines_horizontal_lowerPad; ++i) {
                std::cout << Form("TLines_horizontal_lowerPad[%d] = %f", i, TLines_horizontal_lowerPad.at(i)) << std::endl;
        }
        std::cout << "nLineStyles_horizontal_lowerPad = " << nLineStyles_horizontal_lowerPad << std::endl;
        for (int i = 0; i<nLineStyles_horizontal_lowerPad; ++i) {
            std::cout << Form("lineStyles_horizontal_lowerPad[%d] = %s", i, lineStyles_horizontal_lowerPad.at(i).c_str()) << std::endl;
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

        std::cout << "nFitTH1_lowerPad = " << nFitTH1_lowerPad << std::endl;
        bool dofitTH1_lowerPad = false;
        for (int i = 0; i<nFitTH1_lowerPad; ++i) {
            std::cout << Form("fitTH1_lowerPad[%d] = %d", i, fitTH1_lowerPad.at(i)) << std::endl;
            if (fitTH1_lowerPad.at(i) > 0)  dofitTH1_lowerPad = true;
        }
        if (dofitTH1_lowerPad) {
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

    std::cout << "windowWidth  = " << windowWidth << std::endl;
    std::cout << "windowHeight = " << windowHeight << std::endl;
    std::cout << "frameWidth   = " << frameWidth << std::endl;
    std::cout << "frameHeight  = " << frameHeight << std::endl;
    std::cout << "leftMargin   = " << leftMargin << std::endl;
    std::cout << "rightMargin  = " << rightMargin << std::endl;
    std::cout << "bottomMargin = " << bottomMargin << std::endl;
    std::cout << "topMargin    = " << topMargin << std::endl;
    std::cout << "xMargin = " << xMargin << std::endl;
    std::cout << "yMargin = " << yMargin << std::endl;
    std::cout << "rows    = " << rows << std::endl;
    std::cout << "columns = " << columns << std::endl;
    std::cout << "nSetLogx = " << nSetLogx << std::endl;
    for (int i = 0; i<nSetLogx; ++i) {
        std::cout << Form("setLogx[%d] = %d", i, setLogx.at(i)) << std::endl;
    }
    std::cout << "nSetLogy = " << nSetLogy << std::endl;
    for (int i = 0; i<nSetLogy; ++i) {
        std::cout << Form("setLogy[%d] = %d", i, setLogy.at(i)) << std::endl;
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
        int indexPad = TH1_padIndices.at(i);
        std::string inputFile = inputFiles.at(0).c_str();
        if (nInputFiles == nHistos)  inputFile = inputFiles.at(i).c_str();

        f[i] = TFile::Open(inputFile.c_str());
        h[i] = 0;
        h[i] = (TH1D*)f[i]->Get(TH1_Path.c_str());
        if(h[i] == 0){
            std::cout << "No histogram found: " << TH1_Path.c_str() << " file: " << inputFile.c_str() << std::endl;
        }
        h[i]->SetName(Form("h_%d", i));
        h[i]->SetStats(false);

        // print info about histograms
        std::cout << "#####" << std::endl;
        std::cout << Form("h[%d]", i) << std::endl;
        std::cout << Form("TH1_Paths[%d] = %s", i, TH1_Path.c_str()) << std::endl;
        std::cout << Form("inputFiles[%d] = %s", i, inputFile.c_str()) << std::endl;
        std::string summary = summaryTH1(h[i]);
        std::cout << summary.c_str() << std::endl;

        int drawNormalizedTmp = drawNormalized.at(0);
        if (nDrawNormalized == nHistos) drawNormalizedTmp = drawNormalized.at(i);
        if (drawNormalizedTmp == INPUT_TH1::k_normInt)  h[i]->Scale(1./h[i]->Integral());

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
        std::string title = "";
        if (nTitles == 1) title = titles.at(0);
        else if (nTitles == nPads)  title = titles.at(indexPad);
        if (title == CONFIGPARSER::nullInput) h[i]->SetTitle("");   // title is explicitly specified to be empty.
        else if (title.size() > 0)  h[i]->SetTitle(title.c_str());  // title is specified.

        std::string titleX = "";
        if (nTitlesX == 1) titleX = titlesX.at(0);
        else if (nTitlesX == nPads)  titleX = titlesX.at(indexPad);
        if (titleX == CONFIGPARSER::nullInput) h[i]->SetXTitle("");
        else if (titleX.size() > 0) h[i]->SetXTitle(titleX.c_str());

        std::string titleY = "";
        if (nTitlesY == 1) titleY = titlesY.at(0);
        else if (nTitlesY == nPads)  titleY = titlesY.at(indexPad);
        if (titleY == CONFIGPARSER::nullInput) h[i]->SetYTitle("");
        else if (titleY.size() > 0) h[i]->SetYTitle(titleY.c_str());

        std::string drawOption = "";
        if (nDrawOptions == 1) {
            if (drawOptions.at(0) != CONFIGPARSER::nullInput)  drawOption = drawOptions.at(0).c_str();
        }
        else if (nDrawOptions == nHistos) {
            if (drawOptions.at(i) != CONFIGPARSER::nullInput)  drawOption = drawOptions.at(i).c_str();
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

        int lineWidth = lineWidths.at(0);
        if (nLineWidths == nHistos)  lineWidth = lineWidths.at(i);
        if (lineWidth != INPUT_DEFAULT::lineWidth) {
            if (drawOption.find("hist") != std::string::npos)
                h[i]->SetLineWidth(lineWidth);
        }

        float markerSize = markerSizes.at(0);
        if (nMarkerSizes == nHistos) markerSize = markerSizes.at(i);
        h[i]->SetMarkerSize(markerSize);

        float xMinTmp = xMin.at(0);
        if (nxMin == nPads) xMinTmp = xMin.at(indexPad);
        float xMaxTmp = xMax.at(0);
        if (nxMax == nPads) xMaxTmp = xMax.at(indexPad);
        if (xMaxTmp > xMinTmp)       h[i]->SetAxisRange(xMinTmp, xMaxTmp, "X");

        float yMinTmp = yMin.at(0);
        if (nyMin == nPads) yMinTmp = yMin.at(indexPad);
        float yMaxTmp = yMax.at(0);
        if (nyMax == nPads) yMaxTmp = yMax.at(indexPad);
        if (yMaxTmp > yMinTmp)       h[i]->SetAxisRange(yMinTmp, yMaxTmp, "Y");

        float titleOffsetX = titleOffsetsX.at(0);
        if (nTitleOffsetX == nPads)  titleOffsetX = titleOffsetsX.at(indexPad);
        h[i]->SetTitleOffset(titleOffsetX,"X");

        float titleOffsetY = titleOffsetsY.at(0);
        if (nTitleOffsetY == nPads)  titleOffsetY = titleOffsetsY.at(indexPad);
        h[i]->SetTitleOffset(titleOffsetY,"Y");

        int centerTitleXTmp = 0;
        if (nCenterTitleX == 1) centerTitleXTmp = centerTitleX.at(0);
        else if (nCenterTitleX == nPads)  centerTitleXTmp = centerTitleX.at(indexPad);
        if (centerTitleXTmp > 0)  h[i]->GetXaxis()->CenterTitle();

        int centerTitleYTmp = 0;
        if (nCenterTitleY == 1) centerTitleYTmp = centerTitleY.at(0);
        else if (nCenterTitleY == nPads)  centerTitleYTmp = centerTitleY.at(indexPad);
        if (centerTitleYTmp > 0)  h[i]->GetYaxis()->CenterTitle();

        int fitTH1Tmp = fitTH1.at(0);
        if (nFitTH1 == nHistos)  fitTH1Tmp = fitTH1.at(i);
        if (fitTH1Tmp > 0) {
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
                if (fitColors.at(i) != CONFIGPARSER::nullInput)
                    fitColor = GraphicsConfigurationParser::ParseColor(fitColors.at(i));
            }

            if (TF1_formula != CONFIGPARSER::nullInput) {
                TF1 f1(Form("f1_%d", i), TF1_formula.c_str(), TF1_xMin, TF1_xMax);
                f1.SetLineColor(fitColor);

                h[i]->Fit(f1.GetName(), fitOption.c_str());
            }
        }
    }

    TCanvas* c = 0;
    TPad* pads[nPads];
    TPad* pads2[nPads];
    TLegend* legs[nPads];
    for (int iPad = 0; iPad < nPads; ++iPad) {
        legs[iPad] = new TLegend();
    }

    // set canvas and pads
    double normCanvasHeight = calcNormCanvasHeight(rows, bottomMargin, topMargin, yMargin);
    double normCanvasWidth = calcNormCanvasWidth(columns, leftMargin, rightMargin, xMargin);

    c = new TCanvas("cnv","",windowWidth,windowHeight);

    setCanvasSizeMargin(c, normCanvasWidth, normCanvasHeight, leftMargin, rightMargin, bottomMargin, topMargin);
    setCanvasFinal(c);
    c->cd();

    float yMinOffset = 0;
    double windowHeightScale = 1;
    if (doDrawRatio || doDrawDiff) {
        // resize the canvas in along y axis
        float windowHeightFraction = windowHeightFractions.at(0);
        for (int iFrac = 1; iFrac < nWindowHeightFractions; ++iFrac) {
            if (windowHeightFractions.at(iFrac) > windowHeightFraction)
                windowHeightFraction = windowHeightFractions.at(iFrac);
        }
        windowHeightScale = 1 + windowHeightFraction;
        c->SetCanvasSize(c->GetWw(), c->GetWh()*windowHeightScale);

        yMinOffset = windowHeightFraction;
    }

    divideCanvas(c, pads, rows, columns, leftMargin, rightMargin, bottomMargin, topMargin, xMargin, yMargin, frameWidth, frameHeight, yMinOffset);

    for (int iPad = 0; iPad < nPads; ++iPad) {
        int setLogxTmp = setLogx.at(0);
        if (nSetLogx == nPads) setLogxTmp = setLogx.at(iPad);
        int setLogyTmp = setLogy.at(0);
        if (nSetLogy == nPads) setLogyTmp = setLogy.at(iPad);

        setPadFinal(pads[iPad], setLogxTmp, setLogyTmp);
    }

    if (doDrawRatio || doDrawDiff) {

        for (int iPad = 0; iPad < nPads; ++iPad) {
            c->cd();

            std::string padNameTmp = Form("%s_lower", pads[iPad]->GetName());
            double x1_lowerPad = pads[iPad]->GetXlowNDC();
            double y1_lowerPad = pads[iPad]->GetYlowNDC();
            if (iPad + columns < nPads)  y1_lowerPad = pads[iPad+columns]->GetYlowNDC();
            else                         y1_lowerPad = 0;
            double x2_lowerPad = pads[iPad]->GetXlowNDC()+pads[iPad]->GetWNDC();
            double y2_lowerPad = pads[iPad]->GetYlowNDC();

            pads2[iPad] = new TPad(padNameTmp.c_str(), "", x1_lowerPad, y1_lowerPad, x2_lowerPad, y2_lowerPad);
            pads2[iPad]->SetLeftMargin(pads[iPad]->GetLeftMargin());
            pads2[iPad]->SetRightMargin(pads[iPad]->GetRightMargin());
            pads2[iPad]->SetBottomMargin(yMargin * (pads[iPad]->GetAbsHNDC() / pads2[iPad]->GetAbsHNDC()));
            pads2[iPad]->SetTopMargin(0);

            setPadFinal(pads2[iPad], pads[iPad]->GetLogx(), 0);  // do not draw the y-axis in log scale for the ratio histogram.

            pads2[iPad]->Draw();
            pads2[iPad]->cd();
            pads2[iPad]->SetNumber(nPads+iPad+1);
        }
    }
    // set canvas and pads - END

    for (int iPad = 0; iPad < nPads; ++iPad) {

        float yMinTmp = yMin.at(0);
        if (nyMin > 1 && nyMin == nPads) yMinTmp = yMin.at(iPad);
        float yMaxTmp = yMax.at(0);
        if (nyMax > 1 && nyMax == nPads) yMaxTmp = yMax.at(iPad);

        // set maximum/minimum of y-axis
        if (yMinTmp > yMaxTmp) {

            int iStart = std::find(TH1_padIndices.begin(), TH1_padIndices.end(), iPad) - TH1_padIndices.begin();
            int nTH1_perPad = TH1s_perPad.at(iPad);

            double histMin = getMinimumTH1s(h, nTH1_perPad, iStart);
            double histMax = getMaximumTH1s(h, nTH1_perPad, iStart);
            int setLogyTmp = setLogy.at(0);
            if (nSetLogy == nPads) setLogyTmp = setLogy.at(iPad);
            if (setLogyTmp == 0) h[iStart]->SetMinimum(histMin-TMath::Abs(histMin)*0.1);
            h[iStart]->SetMaximum(histMax+TMath::Abs(histMax)*0.1*TMath::Power(10,setLogyTmp));
        }
    }

    int nIndexDrawOptionHist = indexDrawOptionHist.size();
    for (int i = 0; nIndexDrawOptionHist; ++i) {
        // draw first the histograms with drawOption = "hist" so that plots with markers are not overwritten.
        int iHist = indexDrawOptionHist.at(i);

        std::string drawOption = "";
        if (nDrawOptions == 1)  drawOption = drawOptions.at(0).c_str();
        else if (nDrawOptions == nHistos) drawOption = drawOptions.at(iHist).c_str();

        if (!hDrawn[iHist]) {
             int indexPad = TH1_padIndices.at(iHist);
             c->cd(indexPad);
             h[iHist]->Draw(Form("%s same", drawOption.c_str()));
             hDrawn[iHist] = true;
         }
    }

    for (int i = 0; i<nHistos; ++i) {

        int indexPad = TH1_padIndices.at(i);
        c->cd(indexPad+1);

        std::string drawOption = "";
        if (nDrawOptions == 1)  drawOption = drawOptions.at(0).c_str();
        else if (nDrawOptions == nHistos) drawOption = drawOptions.at(i).c_str();

        if (!hDrawn[i]) {
            h[i]->Draw(Form("%s same", drawOption.c_str()));
            hDrawn[i] = true;
        }

        std::string label = legendEntryLabels.at(0).c_str();
        if (nHistos == nLegendEntryLabels) label = legendEntryLabels.at(i).c_str();
        if (label == CONFIGPARSER::nullInput)  continue;

        std::string legendOption = "lpf";
        if (drawOption.find("hist") != std::string::npos)  legendOption = "lf";
        int legIndex = -1;
        if (nHistos == nLegendEntryLabels) legIndex = legendEntryPadIndices.at(i);
        if (legIndex > -1)  legs[legIndex]->AddEntry(h[i], label.c_str(), legendOption.c_str());
    }

    for (int iPad = 0; iPad < nPads; ++iPad) {
        c->cd(iPad+1);

        int nTH1_perPad = TH1s_perPad.at(iPad);
        int iStart = std::find(TH1_padIndices.begin(), TH1_padIndices.end(), iPad) - TH1_padIndices.begin();

        bool legendExists = (std::find(legendEntryPadIndices.begin(), legendEntryPadIndices.end(), iPad) != legendEntryPadIndices.end());
        if (legendExists && nLegendPositions > 0) {    // draw the legend if really a position is provided.

            float legendTextSize = 0;
            if (nLegendTextSizes == 1) legendTextSize = legendTextSizes.at(0);
            else if (nLegendTextSizes == nPads) legendTextSize = legendTextSizes.at(iPad);
            if (legendTextSize != 0)  legs[iPad]->SetTextSize(legendTextSize);

            int legendBorderSize = 0;
            if (nLegendBorderSizes == 1) legendBorderSize = legendBorderSizes.at(0);
            else if (nLegendBorderSizes == nPads) legendBorderSize = legendBorderSizes.at(iPad);
            legs[iPad]->SetBorderSize(legendBorderSize);

            float legendHeight = 0;
            if (nLegendHeights == 1) legendHeight = legendHeights.at(0);
            else if (nLegendHeights == nPads) legendHeight = legendHeights.at(iPad);

            float legendWidth = 0;
            if (nLegendWidths == 1) legendWidth = legendWidths.at(0);
            else if (nLegendWidths == nPads) legendWidth = legendWidths.at(iPad);

            double height = calcTLegendHeight(legs[iPad]);
            double width = calcTLegendWidth(legs[iPad]);
            if (legendHeight != 0)  height = legendHeight;
            if (legendWidth != 0)  width = legendWidth;

            std::string legendPosition = legendPositions.at(0).c_str();
            if (nLegendPositions == nPads)  legendPosition = legendPositions.at(iPad).c_str();

            float legendOffsetX = 0;
            if (nLegendOffsetsX == 1) legendOffsetX = legendOffsetsX.at(0);
            else if (nLegendOffsetsX == nPads) legendOffsetX = legendOffsetsX.at(iPad);

            float legendOffsetY = 0;
            if (nLegendOffsetsY == 1) legendOffsetY = legendOffsetsY.at(0);
            else if (nLegendOffsetsY == nPads) legendOffsetY = legendOffsetsY.at(iPad);

            setLegendPosition(legs[iPad], legendPosition, pads[iPad], height, width, legendOffsetX, legendOffsetY);
            int nLegEntriesTmp = legs[iPad]->GetNRows();
            if (nLegEntriesTmp > 0)  legs[iPad]->Draw();
        }

        // add Text
        TLatex* latex = 0;
        bool textExists = (std::find(textLinePadIndices.begin(), textLinePadIndices.end(), iPad) != textLinePadIndices.end());
        if (textExists && nTextLines > 0) {
            latex = new TLatex();

            std::string textPosition = "";
            if (nTextPositions == 1)  textPosition = textPositions.at(0);
            else if (nTextPositions == nPads)  textPosition = textPositions.at(iPad);
            setTextAlignment(latex, textPosition);

            float textFont = textFonts.at(0);
            if (nTextFonts == nPads) textFont = textFonts.at(iPad);
            latex->SetTextFont(textFont);

            float textSize = textSizes.at(0);
            if (nTextSizes == nPads) textSize = textSizes.at(iPad);
            latex->SetTextSize(textSize);

            float textOffsetX = textOffsetsX.at(0);
            if (nTextOffsetsX == nPads) textOffsetX = textOffsetsX.at(iPad);

            float textOffsetY = textOffsetsY.at(0);
            if (nTextOffsetsY == nPads) textOffsetY = textOffsetsY.at(iPad);

            std::vector<std::string> textLinesTmp;
            for (int iLine = 0; iLine < nTextLines; ++iLine) {
                if (textLinePadIndices.at(iLine) == iPad)
                    textLinesTmp.push_back(textLines.at(iLine).c_str());
            }

            int nTextLinesTmp = textLinesTmp.size();
            std::vector<std::pair<float,float>> textCoordinates = calcTextCoordinates(textLinesTmp, textPosition, c, textOffsetX, textOffsetY);
            for (int i = 0; i<nTextLinesTmp; ++i){
                float x = textCoordinates.at(i).first;
                float y = textCoordinates.at(i).second;
                if (textLinesTmp.at(i) != CONFIGPARSER::nullInput.c_str())
                    latex->DrawLatexNDC(x, y, textLinesTmp.at(i).c_str());
            }
        }

        // add Text above the pad
        TLatex* latexAbovePad = 0;
        bool textAbovePadExists = (std::find(textsAbovePadPadIndices.begin(), textsAbovePadPadIndices.end(), iPad) != textsAbovePadPadIndices.end());
        if (textAbovePadExists && nTextsAbovePad > 0) {
            latexAbovePad = new TLatex();

            int textAbovePadFont = textAbovePadFonts.at(0);
            if (nTextAbovePadFonts == nPads)  textAbovePadFont = textAbovePadFonts.at(iPad);
            latexAbovePad->SetTextFont(textAbovePadFont);

            float textAbovePadSize = textAbovePadSizes.at(0);
            if (nTextAbovePadSizes == nPads)  textAbovePadSize = textAbovePadSizes.at(iPad);
            latexAbovePad->SetTextSize(textAbovePadSize);

            float textAbovePadOffsetX = textAbovePadOffsetsX.at(0);
            if (nTextAbovePadOffsetsX == nPads) textAbovePadOffsetX = textAbovePadOffsetsX.at(iPad);

            float textAbovePadOffsetY = textAbovePadOffsetsX.at(0);
            if (nTextAbovePadOffsetsY == nPads) textAbovePadOffsetY = textAbovePadOffsetsX.at(iPad);

            for (int iText = 0; iText < nTextsAbovePad; ++iText) {
                if (textsAbovePadPadIndices.at(iText) != iPad)  continue;

                int textOverPadAlignment = GRAPHICS::textAlign;
                if (nTextsAbovePadAlignments == 1) textOverPadAlignment = GraphicsConfigurationParser::ParseTextAlign(textsAbovePadAlignments.at(0));
                else if (nTextsAbovePadAlignments == nTextsAbovePad) textOverPadAlignment = GraphicsConfigurationParser::ParseTextAlign(textsAbovePadAlignments.at(iText));

                latexAbovePad->SetTextAlign(textOverPadAlignment);
                setTextAbovePad(latexAbovePad, c, textAbovePadOffsetX, textAbovePadOffsetY);

                latexAbovePad->DrawLatexNDC(latexAbovePad->GetX(), latexAbovePad->GetY(), textsAbovePad.at(iText).c_str());
            }
        }

        // add TLine
        TLine* line_horizontal[nTLines_horizontal];
        bool lineHorizontalExists = (std::find(TLines_horizontal_PadIndices.begin(), TLines_horizontal_PadIndices.end(), iPad) != TLines_horizontal_PadIndices.end());
        if (lineHorizontalExists && nHistos > 0) {
            for (int iLine = 0; iLine < nTLines_horizontal; ++iLine) {
                if (TLines_horizontal_PadIndices.at(iLine) != iPad)  continue;

                // draw horizontal line
                double xmin = h[iStart]->GetXaxis()->GetBinLowEdge(h[iStart]->GetXaxis()->GetFirst());
                double xmax = h[iStart]->GetXaxis()->GetBinLowEdge(h[iStart]->GetXaxis()->GetLast()+1);

                int lineStyle_horizontal = GRAPHICS::lineStyle_horizontal;
                if (nLineStyles_horizontal == 1)
                    lineStyle_horizontal = GraphicsConfigurationParser::ParseLineStyle(lineStyles_horizontal.at(0));
                else if (nLineStyles_horizontal == nTLines_horizontal)
                    lineStyle_horizontal = GraphicsConfigurationParser::ParseLineStyle(lineStyles_horizontal.at(iLine));

                line_horizontal[iLine] = new TLine(xmin, TLines_horizontal.at(iLine), xmax, TLines_horizontal.at(iLine));
                line_horizontal[iLine]->SetLineStyle(lineStyle_horizontal);   // https://root.cern.ch/doc/master/TAttLine_8h.html#a7092c0c4616367016b70d54e5c680a69
                line_horizontal[iLine]->Draw();
            }
        }
        // add TLine
        TLine* line_vertical[nTLines_vertical];
        bool lineVerticalExists = (std::find(TLines_vertical_PadIndices.begin(), TLines_vertical_PadIndices.end(), iPad) != TLines_vertical_PadIndices.end());
        if (lineVerticalExists && nHistos > 0) {
            for (int iLine = 0; iLine<nTLines_vertical; ++iLine) {
                if (TLines_vertical_PadIndices.at(iLine) != iPad)  continue;

                // draw vertical line
                double ymin = h[iStart]->GetMinimum();
                double ymax = h[iStart]->GetMaximum();

                int lineStyle_vertical = GRAPHICS::lineStyle_vertical;
                if (nLineStyles_vertical == 1)
                    lineStyle_vertical = GraphicsConfigurationParser::ParseLineStyle(lineStyles_vertical.at(0));
                else if (nLineStyles_vertical == nTLines_vertical)
                    lineStyle_vertical = GraphicsConfigurationParser::ParseLineStyle(lineStyles_vertical.at(iLine));

                line_vertical[iLine] = new TLine(TLines_vertical.at(iLine), ymin, TLines_vertical.at(iLine), ymax);
                line_vertical[iLine]->SetLineStyle(lineStyle_vertical);   // https://root.cern.ch/doc/master/TAttLine_8h.html#a7092c0c4616367016b70d54e5c680a69
                line_vertical[iLine]->Draw();
            }
        }

        if (doDrawRatio || doDrawDiff) {

            int drawRatioTmp = drawRatio.at(0);
            if (nDrawRatio == nPads)  drawRatioTmp = drawRatio.at(iPad);

            int drawDiffTmp = drawDiff.at(0);
            if (nDrawDiff == nPads)  drawDiffTmp = drawDiff.at(iPad);

            if (drawRatioTmp == 0 && drawDiffTmp == 0)  continue;
            c->cd(nPads+iPad+1);

            // ratio or diff histograms
            int nHistos_lowerPad = 0;
            // ratio histograms will be the division of input histograms at even index by the histograms at odd index
            // diff histograms will be input histograms at even index - input histograms at odd index
            // if input histogram list has odd number of histograms then drawRatio/drawDiff step is skipped.
            // Ex. : TH1_paths = {h1, h2, h3, h4}
            //      ==> h_ratio[0] = h1/h2, h_ratio[1] = h3/h4
            //      ==> h_diff[0] = h1-h2, h_diff[1] = h3-h4
            if (nHistos % 2 == 0)   nHistos_lowerPad = nTH1_perPad/2;

            TH1D* h_lowerPad[nHistos_lowerPad];
            for (int i=0; i<nHistos_lowerPad; ++i) {

                h_lowerPad[i] = (TH1D*)h[iStart+2*i]->Clone(Form("%s_lowerPad", h[iStart+2*i]->GetName()));
                h_lowerPad[i]->Reset();    // start from scratch, reset the undesired properties inherited from the original histogram.
                h_lowerPad[i]->Add(h[iStart+2*i]);

                // do compatibility test before ratio / diff
                double valKolmogorov = h_lowerPad[i]->KolmogorovTest(h[iStart+2*i+1]);
                std::cout << Form("KolmogorovTest(hist %d, hist %d) = %f", iStart+2*i, iStart+2*i+1, valKolmogorov) << std::endl;
                h_lowerPad[i]->Chi2Test(h[iStart+2*i+1],"WW P");

                if (drawRatioTmp > 0)  h_lowerPad[i]->Divide(h[iStart+2*i+1]);
                else if (drawDiffTmp > 0)  h_lowerPad[i]->Add(h[iStart+2*i+1],-1);

                float yMin_lowerPadTmp = yMin_lowerPad.at(0);
                if (nyMin_lowerPad > 1 && nyMin_lowerPad == nPads) yMin_lowerPadTmp = yMin_lowerPad.at(iPad);

                float yMax_lowerPadTmp = yMax_lowerPad.at(0);
                if (nyMax_lowerPad > 1 && nyMax_lowerPad == nPads) yMax_lowerPadTmp = yMax_lowerPad.at(iPad);

                if (yMax_lowerPadTmp > yMin_lowerPadTmp)   h_lowerPad[i]->SetAxisRange(yMin_lowerPadTmp, yMax_lowerPadTmp, "Y");

                for (int iFit = 0; iFit < nTF1_formulas_lowerPad; ++iFit) {

                    if (TF1_lowerPad_Indices.at(iFit)-1 != iPad)  continue;

                    std::string TF1_formula_lowerPad = TF1_formulas_lowerPad.at(iFit).c_str();
                    if (TF1_formula_lowerPad == CONFIGPARSER::nullInput.c_str()) continue;

                    double TF1_xMin_lowerPad = TF1_ranges_lowerPad[0].at(iFit);
                    double TF1_xMax_lowerPad = TF1_ranges_lowerPad[1].at(iFit);

                    std::string fitOption_lowerPad = INPUT_DEFAULT::fitOption.c_str();
                    if (nFitOptions_lowerPad == 1)  fitOption_lowerPad = fitOptions_lowerPad.at(0).c_str();
                    if (nFitOptions_lowerPad == nFitTH1_lowerPad)  fitOption_lowerPad = fitOptions_lowerPad.at(iFit).c_str();

                    int fitColor_lowerPad = INPUT_DEFAULT::fitColor;
                    if (nFitColors == 1) fitColor_lowerPad = GraphicsConfigurationParser::ParseColor(fitColors_lowerPad.at(0));
                    else if (nFitColors_lowerPad == nFitTH1_lowerPad) {
                        if (fitColors_lowerPad.at(iFit) != CONFIGPARSER::nullInput)
                            fitColor_lowerPad = GraphicsConfigurationParser::ParseColor(fitColors_lowerPad.at(iFit));
                    }

                    TF1 f1_lowerPad(Form("f1_%d", iFit), TF1_formula_lowerPad.c_str(), TF1_xMin_lowerPad, TF1_xMax_lowerPad);
                    f1_lowerPad.SetLineColor(fitColor_lowerPad);

                    int fitTH1_lowerPad_Start =
                            int(std::find(TF1_lowerPad_Indices.begin(), TF1_lowerPad_Indices.end(), iPad+1) - TF1_lowerPad_Indices.begin());
                    int iFit_Pad = iFit - fitTH1_lowerPad_Start;
                    h_lowerPad[iFit_Pad]->Fit(f1_lowerPad.GetName(), fitOption_lowerPad.c_str());
                }
            }

            // set maximum/minimum of y-axis in the lower pad
            float yMin_lowerPadTmp = yMin_lowerPad.at(0);
            if (nyMin_lowerPad > 1 && nyMin_lowerPad == nPads) yMin_lowerPadTmp = yMin_lowerPad.at(iPad);

            float yMax_lowerPadTmp = yMax_lowerPad.at(0);
            if (nyMax_lowerPad > 1 && nyMax_lowerPad == nPads) yMax_lowerPadTmp = yMax_lowerPad.at(iPad);

            if (yMin_lowerPadTmp > yMax_lowerPadTmp) {
                double histMin = getMinimumTH1s(h_lowerPad, nHistos_lowerPad);
                double histMax = getMaximumTH1s(h_lowerPad, nHistos_lowerPad);
                h_lowerPad[0]->SetMinimum(histMin-TMath::Abs(histMin)*0.1);
                h_lowerPad[0]->SetMaximum(histMax+TMath::Abs(histMax)*0.1);
            }

            for (int i=0; i<nHistos_lowerPad; ++i) {
                double axisSizeRatio = (c->GetPad(iPad+1)->GetAbsHNDC()/c->GetPad(nPads+iPad+1)->GetAbsHNDC());
                setTH1Ratio(h_lowerPad[i], h[iStart+2*i], axisSizeRatio);

                std::string titleY_lowerPad = "";
                if (nTitlesY_lowerPad == 1) titleY_lowerPad = titlesY_lowerPad.at(0);
                else if (nTitlesY_lowerPad == nPads)  titleY_lowerPad = titlesY_lowerPad.at(iPad);

                if (titleY_lowerPad == CONFIGPARSER::nullInput) h[i]->SetYTitle("");
                else if (titleY_lowerPad.size() > 0) h_lowerPad[i]->SetYTitle(titleY_lowerPad.c_str());

                h_lowerPad[i]->SetMarkerColor(h[iStart+2*i]->GetMarkerColor());
                h_lowerPad[i]->SetMarkerStyle(h[iStart+2*i]->GetMarkerStyle());

                h_lowerPad[i]->Draw("e same");
            }
            // add TLine to the lower pad
            TLine* line_horizontal_lowerPad[nTLines_horizontal_lowerPad];
            bool lineHorizontalLowerPadExists = (std::find(TLines_horizontal_lowerPad_PadIndices.begin(), TLines_horizontal_lowerPad_PadIndices.end(), iPad) != TLines_horizontal_lowerPad_PadIndices.end());
            if (lineHorizontalLowerPadExists && nHistos_lowerPad > 0) {
                for (int iLine = 0; iLine < nTLines_horizontal_lowerPad; ++iLine) {
                    if (TLines_horizontal_lowerPad_PadIndices.at(iLine) != iPad)  continue;

                    // draw horizontal line
                    double xmin_lowerPad = h_lowerPad[0]->GetXaxis()->GetBinLowEdge(h[iStart]->GetXaxis()->GetFirst());
                    double xmax_lowerPad = h_lowerPad[0]->GetXaxis()->GetBinLowEdge(h[iStart]->GetXaxis()->GetLast()+1);

                    int lineStyle_horizontal_lowerPad = GRAPHICS::lineStyle_horizontal;
                    if (nLineStyles_horizontal_lowerPad == 1)
                        lineStyle_horizontal_lowerPad = GraphicsConfigurationParser::ParseLineStyle(lineStyles_horizontal_lowerPad.at(0));
                    else if (nLineStyles_horizontal_lowerPad == nTLines_horizontal_lowerPad)
                        lineStyle_horizontal_lowerPad = GraphicsConfigurationParser::ParseLineStyle(lineStyles_horizontal_lowerPad.at(iLine));

                    line_horizontal_lowerPad[iLine] = new TLine(xmin_lowerPad, TLines_horizontal_lowerPad.at(iLine), xmax_lowerPad, TLines_horizontal_lowerPad.at(iLine));
                    line_horizontal_lowerPad[iLine]->SetLineStyle(lineStyle_horizontal_lowerPad);   // https://root.cern.ch/doc/master/TAttLine_8h.html#a7092c0c4616367016b70d54e5c680a69
                    line_horizontal_lowerPad[iLine]->Draw();
                }
            }
            // add TLine to the lower pad
            TLine* line_vertical_lowerPad[nTLines_vertical_lowerPad];
            double ymin_lowerPad = getMinimumTH1s(h_lowerPad, nHistos_lowerPad);
            double ymax_lowerPad = getMaximumTH1s(h_lowerPad, nHistos_lowerPad);
            bool lineVerticalLowerPadExists = (std::find(TLines_vertical_lowerPad_PadIndices.begin(), TLines_vertical_lowerPad_PadIndices.end(), iPad) != TLines_vertical_lowerPad_PadIndices.end());
            if (lineVerticalLowerPadExists && nHistos_lowerPad > 0) {
                for (int iLine = 0; iLine < nTLines_vertical_lowerPad; ++iLine) {
                    if (TLines_vertical_lowerPad_PadIndices.at(iLine) != iPad)  continue;

                    int lineStyle_vertical_lowerPad = GRAPHICS::lineStyle_vertical;
                    if (nLineStyles_vertical_lowerPad == 1)
                        lineStyle_vertical_lowerPad = GraphicsConfigurationParser::ParseLineStyle(lineStyles_vertical_lowerPad.at(0));
                    else if (nLineStyles_vertical_lowerPad == nTLines_vertical_lowerPad)
                        lineStyle_vertical_lowerPad = GraphicsConfigurationParser::ParseLineStyle(lineStyles_vertical_lowerPad.at(iLine));

                    line_vertical_lowerPad[iLine] = new TLine(TLines_vertical_lowerPad.at(iLine), ymin_lowerPad, TLines_vertical_lowerPad.at(iLine), ymax_lowerPad);
                    line_vertical_lowerPad[iLine]->SetLineStyle(lineStyle_vertical_lowerPad);   // https://root.cern.ch/doc/master/TAttLine_8h.html#a7092c0c4616367016b70d54e5c680a69
                    line_vertical_lowerPad[iLine]->Draw();
                }
            }
        }
        c->cd(iPad+1);
    }
    c->cd(1);

    std::string tmpOutputFile = outputFile.Data();
    if (tmpOutputFile.find(".") != std::string::npos) {     // file extension is specified
        c->SaveAs(tmpOutputFile.c_str());
    }
    else {  // file extension is NOT specified
        c->SaveAs(Form("%s.pdf", tmpOutputFile.c_str()));
    }

    c->Close();         // do not use Delete() for TCanvas.
}

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
