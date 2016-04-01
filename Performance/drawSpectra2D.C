/*
 * macro to draw 2D spectra/distribution histograms.
 * saves histograms to a .root file.
 * if "outputFigureName" is specified and "drawSame" input is set, saves the canvas as picture.
 */

#include <TFile.h>
#include <TChain.h>
#include <TH1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCut.h>
#include <TCanvas.h>
#include <TLine.h>
#include <TLegend.h>
#include <TMath.h>

#include <string>
#include <vector>
#include <iostream>

#include "../TreeHeaders/CutConfigurationTree.h"
#include "../Utilities/interface/CutConfigurationParser.h"
#include "../Utilities/interface/InputConfigurationParser.h"
#include "../Utilities/interface/GraphicsConfigurationParser.h"
#include "../Utilities/interface/HiForestInfoController.h"
#include "../Utilities/styleUtil.h"
#include "../Utilities/th1Util.h"

void drawSpectra2D(const TString configFile, const TString inputFile, const TString outputFile = "drawSpectra2D.root", const TString outputFigureName = "");

void drawSpectra2D(const TString configFile, const TString inputFile, const TString outputFile, const TString outputFigureName)
{
    std::cout<<"running drawSpectra2D()"<<std::endl;
    std::cout<<"configFile  = "<< configFile.Data() <<std::endl;
    std::cout<<"inputFile   = "<< inputFile.Data()  <<std::endl;
    std::cout<<"outputFile  = "<< outputFile.Data() <<std::endl;

    InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

    /*
     * drawing behavior :
     *      1. If N = # formulas and N = # selections, then N histograms will be drawn,
     *      2. If 1 = # formulas and N = # selections, then N histograms will be drawn with the same formula.
     *      3. If N = # formulas and 1 = # selections, then N histograms will be drawn with the same selection.
     *      4. else, exit.
     */
    // input for mode
    int mode;

    // input for TTree
    std::vector<std::string> treePaths;
    std::vector<std::string> treeFriendsPath;
    std::vector<std::string> treeFriendsPathIndividual;
    std::vector<std::string> formulas;
    std::string selectionBase;
    std::vector<std::string> selections;
    std::vector<std::string> selectionSplitter;
    std::vector<std::string> weights;

    // input for TH1
    std::vector<std::string>  titles;
    std::vector<std::string>  titlesX;
    std::vector<std::string>  titlesY;
    std::vector<std::vector<float>> TH2D_Bins_List;      // nBinsx, xLow, xUp, nBinsy, yLow, yUp for a TH2D histogram
    float titleOffsetX;
    float titleOffsetY;
    float markerSize;
    int drawNormalized;
    std::vector<std::string> drawOptions;
    std::vector<std::string> markerStyles;
    std::vector<std::string> lineStyles;
    int lineWidth;
    std::vector<std::string> fillStyles;
    std::vector<std::string> colors;
    std::vector<std::string> fillColors;
    std::vector<std::string> lineColors;

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

    // input for TCanvas
    int windowWidth;
    int windowHeight;
    float leftMargin;
    float rightMargin;
    float bottomMargin;
    float topMargin;
    int setLogx;
    int setLogy;
    int setLogz;
    
    if (configInput.isValid) {
        mode = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_mode];

        treePaths = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treePath]);
        treeFriendsPath = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treeFriendPath]);
        treeFriendsPathIndividual = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treeFriendPathIndividual]);
        formulas = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treeFormula]);
        selectionBase = configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treeSelectionBase];
        selections = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treeSelection]);
        selectionSplitter = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treeSelectionSplitter]);
        weights = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH1_weight]);

        titles = ConfigurationParser::ParseList(ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH1_title]));
        titlesX = ConfigurationParser::ParseList(ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH1_titleX]));
        titlesY = ConfigurationParser::ParseList(ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH1_titleY]));
        TH2D_Bins_List = ConfigurationParser::ParseListTH2D_Bins(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH2D_Bins_List]);
        titleOffsetX = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_titleOffsetX];
        titleOffsetY = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_titleOffsetY];
        markerSize = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_markerSize];
        drawNormalized = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_drawNormalized];
        drawOptions = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_drawOption]);
        markerStyles = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_markerStyle]);
        lineStyles = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_lineStyle]);
        fillStyles = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_fillStyle]);
        colors = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_color]);
        fillColors = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_fillColor]);
        lineColors = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_lineColor]);
        lineWidth = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_lineWidth];

        legendEntryLabels = ConfigurationParser::ParseList(ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_legendEntryLabel]));
        legendPosition    = configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_legendPosition];
        legendOffsetX     = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_legendOffsetX];
        legendOffsetY     = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_legendOffsetY];
        legendBorderSize  = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_legendBorderSize];
        legendWidth       = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_legendWidth];
        legendHeight      = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_legendHeight];
        legendTextSize    = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_legendTextSize];

        std::string tmpText = ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_text]);
        textLines = ConfigurationParser::ParseList(tmpText);
        textFont = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_textFont];
        textSize = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_textSize];
        textPosition = configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_textPosition];
        textOffsetX = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_textOffsetX];
        textOffsetY = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_textOffsetY];

        std::string tmpTextOverPad = ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_textAbovePad]);
        textsOverPad = ConfigurationParser::ParseList(tmpTextOverPad);
        textsOverPadAlignments = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_textAbovePadAlign]);
        textAbovePadFont = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_textAbovePadFont];
        textAbovePadSize = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_textAbovePadSize];
        textAbovePadOffsetX = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_textAbovePadOffsetX];
        textAbovePadOffsetY = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_textAbovePadOffsetY];

        TLines_horizontal = ConfigurationParser::ParseListFloat(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TLine_horizontal]);
        lineStyles_horizontal = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_LineStyle_horizontal]);
        TLines_vertical = ConfigurationParser::ParseListFloat(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TLine_vertical]);
        lineStyles_vertical = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_LineStyle_vertical]);

        windowWidth = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_windowWidth];
        windowHeight = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_windowHeight];
        leftMargin   = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_leftMargin];
        rightMargin  = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_rightMargin];
        bottomMargin = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_bottomMargin];
        topMargin    = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_topMargin];
        setLogx = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_setLogx];
        setLogy = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_setLogy];
        setLogz = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_setLogz];
    }
    else {
        mode = 0;

        treePaths.push_back("dummyTREE");
        formulas.push_back("Entry$");
        selectionBase = "";
        selections.push_back("1 == 1");
        weights.push_back("1");

        TH2D_Bins_List.resize(6);
        TH2D_Bins_List[0].push_back(100);    // nBinsx
        TH2D_Bins_List[1].push_back(0);      // xLow
        TH2D_Bins_List[2].push_back(100);    // xUp
        TH2D_Bins_List[3].push_back(100);    // nBinsy
        TH2D_Bins_List[4].push_back(0);      // yLow
        TH2D_Bins_List[5].push_back(100);    // yUp
        titleOffsetX = 1;
        titleOffsetY = 1;
        markerSize = 1;
        drawNormalized = 0;
        lineWidth = 0;
        
        legendEntryLabels.push_back("");
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

        windowWidth = 0;
        windowHeight = 0;
        leftMargin = 0.1;
        rightMargin = 0.1;
        bottomMargin = 0.1;
        topMargin = 0.1;
        setLogx = 0;
        setLogy = 0;
        setLogz = 0;
	textAbovePadFont = 20;
	textAbovePadSize = 18;
    }
    // set default values
    if (selections.size() == 0) selections.push_back("1");
    if (weights.size() == 0)    weights.push_back(INPUT_DEFAULT::TH1_weight.c_str());   // default weight = 1.
    if (titleOffsetX == 0) titleOffsetX = INPUT_DEFAULT::titleOffsetX;
    if (titleOffsetY == 0) titleOffsetY = INPUT_DEFAULT::titleOffsetY;
    if (drawNormalized >= INPUT_TH1::kN_TYPE_NORM) drawNormalized = INPUT_DEFAULT::drawNormalized;
    if (lineWidth == 0)  lineWidth = INPUT_DEFAULT::lineWidth;

    if (markerSize == 0)  markerSize = INPUT_DEFAULT::markerSize;

    if (textFont == 0)  textFont = INPUT_DEFAULT::textFont;
    if (textSize == 0)  textSize = INPUT_DEFAULT::textSize;

    if (windowWidth  == 0)  windowWidth = INPUT_DEFAULT::windowWidth;
    if (windowHeight == 0)  windowHeight = INPUT_DEFAULT::windowHeight;
    if (leftMargin == 0) leftMargin = INPUT_DEFAULT::leftMargin;
    if (rightMargin == 0) rightMargin = INPUT_DEFAULT::rightMargin;
    if (bottomMargin == 0) bottomMargin = INPUT_DEFAULT::bottomMargin;
    if (topMargin == 0) topMargin = INPUT_DEFAULT::topMargin;

    int nTrees = treePaths.size();
    int nFriends = treeFriendsPath.size();
    int nFriendsIndividual = treeFriendsPathIndividual.size();
    int nFormulas = formulas.size();
    int nSelections = selections.size();
    int nSelectionSplitter = selectionSplitter.size();
    int nWeights = weights.size();
    int nTitles = titles.size();
    int nTitlesX = titlesX.size();
    int nTitlesY = titlesY.size();
    int nTH2D_Bins_List = TH2D_Bins_List[0].size();
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

    // verbose about input configuration
    std::cout<<"Input Configuration :"<<std::endl;
    std::cout << "mode = " << mode << std::endl;
    if (mode == INPUT_MODE::k_comparison) {
        // in comparison mode "inputFile" should have the following format
        // inputFile = <inputFile1>,<inputFile2>,...
        // there should be no single space between <inputFile1> and <inputFile2>.
        // the idea is to feed the input samples as a single argument and split them in the macro.
        std::cout << "comparison mode : Spectra from two input samples are going to be compared." << std::endl;
    }
    std::cout << "nTrees = " << nTrees << std::endl;
    for (int i=0; i<nTrees; ++i) {
        std::cout << Form("treePaths[%d] = %s", i, treePaths.at(i).c_str()) << std::endl;
    }
    std::cout << "nFriends = " << nFriends << std::endl;
    for (int i=0; i<nFriends; ++i) {
        std::cout << Form("treeFriendsPath[%d] = %s", i, treeFriendsPath.at(i).c_str()) << std::endl;
    }
    std::cout << "nFriendsIndividual = " << nFriendsIndividual << std::endl;
    for (int i=0; i<nFriendsIndividual; ++i) {
        std::cout << Form("treeFriendsPathIndividual[%d] = %s", i, treeFriendsPathIndividual.at(i).c_str()) << std::endl;
    }
    std::cout << "nFormulas     = " << nFormulas << std::endl;
    for (int i=0; i<nFormulas; ++i) {
        std::cout << Form("formulas[%d]   = %s", i, formulas.at(i).c_str()) << std::endl;
    }
    std::cout << "selectionBase = " << selectionBase.c_str() << std::endl;
    std::cout << "nSelections   = " << nSelections << std::endl;
    for (int i=0; i<nSelections; ++i) {
            std::cout << Form("selections[%d] = %s", i, selections.at(i).c_str()) << std::endl;
    }
    std::cout << "nSelectionSplitter = " << nSelectionSplitter << std::endl;
    for (int i=0; i<nSelectionSplitter; ++i) {
            std::cout << Form("selectionSplitter[%d] = %s", i, selectionSplitter.at(i).c_str()) << std::endl;
    }
    std::cout << "nWeights   = " << nWeights << std::endl;
    for (int i=0; i<nWeights; ++i) {
            std::cout << Form("weights[%d] = %s", i, weights.at(i).c_str()) << std::endl;
    }
    
    std::cout << "nTitles   = " << nTitles << std::endl;
    for (int i=0; i<nTitles; ++i) {
            std::cout << Form("titles[%d] = %s", i, titles.at(i).c_str()) << std::endl;
    }
    std::cout << "nTitlesX   = " << nTitlesX << std::endl;
    for (int i=0; i<nTitlesX; ++i) {
            std::cout << Form("titlesX[%d] = %s", i, titlesX.at(i).c_str()) << std::endl;
    }
    std::cout << "nTitlesY   = " << nTitlesY << std::endl;
    for (int i=0; i<nTitlesY; ++i) {
            std::cout << Form("titlesY[%d] = %s", i, titlesY.at(i).c_str()) << std::endl;
    }
    std::cout << "nTH2D_Bins_List = " << nTH2D_Bins_List << std::endl;
    for (int i=0; i<nTH2D_Bins_List; ++i) {
        std::cout << Form("TH2D_Bins_List[%d] = { ", i);
        std::cout << Form("%.0f, ", TH2D_Bins_List[0].at(i));
        std::cout << Form("%f, ", TH2D_Bins_List[1].at(i));
        std::cout << Form("%f }", TH2D_Bins_List[2].at(i));
        std::cout << " { ";
        std::cout << Form("%.0f, ", TH2D_Bins_List[3].at(i));
        std::cout << Form("%f, ", TH2D_Bins_List[4].at(i));
        std::cout << Form("%f }", TH2D_Bins_List[5].at(i)) << std::endl;;
    }
    std::cout << "titleOffsetX = " << titleOffsetX << std::endl;
    std::cout << "titleOffsetY = " << titleOffsetY << std::endl;
    std::cout << "markerSize = " << markerSize << std::endl;
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

    std::cout << "windowWidth = " << windowWidth << std::endl;
    std::cout << "windowHeight = " << windowHeight << std::endl;
    std::cout << "leftMargin   = " << leftMargin << std::endl;
    std::cout << "rightMargin  = " << rightMargin << std::endl;
    std::cout << "bottomMargin = " << bottomMargin << std::endl;
    std::cout << "topMargin    = " << topMargin << std::endl;
    std::cout << "setLogx = " << setLogx << std::endl;
    std::cout << "setLogy = " << setLogy << std::endl;
    std::cout << "setLogz = " << setLogz << std::endl;
    
    if (configCuts.isValid) {

    }
    else {

    }

    // verbose about cut configuration
    std::cout<<"Cut Configuration :"<<std::endl;

    std::cout<<"Input handling :"<< std::endl;

    std::vector<std::string> inputFileArguments = InputConfigurationParser::ParseFileArgument(inputFile.Data());
    int nInputFileArguments = inputFileArguments.size();
    // if no mode is specified (which is what happens most of the time), then it is expected that nInputFileArguments = 1.
    std::cout<<"nInputFileArguments (number of input file arguments) = "<< nInputFileArguments << std::endl;
    for (int i = 0; i < nInputFileArguments; ++i) {
        std::cout << Form("inputFileArguments[%d] = %s", i, inputFileArguments.at(i).c_str()) << std::endl;
    }

    std::vector<std::vector<std::string>> inputFiles(nInputFileArguments);
    std::cout<<"#####"<< std::endl;
    for (int i = 0; i < nInputFileArguments; ++i) {

        if (nInputFileArguments > 1) {
            std::cout<<"###"<< std::endl;
            std::cout<<"inputFileArgument = " << inputFileArguments.at(i).c_str() << std::endl;
        }

        inputFiles[i] = InputConfigurationParser::ParseFiles(inputFileArguments.at(i));
        std::cout<<"input ROOT files : num = " << inputFiles[i].size() << std::endl;
        for (std::vector<std::string>::iterator it = inputFiles[i].begin() ; it != inputFiles[i].end(); ++it) {
            std::cout<<(*it).c_str()<< std::endl;
        }
    }
    std::cout<<"##### END #####"<< std::endl;

    // check consistency of the input file arguments with the mode
    if (mode == INPUT_MODE::k_noMode && nInputFileArguments > 1) {
        std::cout<<"no specific mode is chosen. more than one input samples are provided."<< std::endl;
        std::cout<<"exiting"<< std::endl;
        return;
    }
    if (mode == INPUT_MODE::k_comparison && nInputFileArguments == 1) {
        std::cout<<"comparison mode is chosen. But only one input sample is provided."<< std::endl;
        std::cout<<"exiting"<< std::endl;
        return;
    }

    if (nTrees == 1 && nFriendsIndividual > 0) {
        std::cout<<"nTrees = "<< nTrees <<", nFriendsIndividual = " << nFriendsIndividual << std::endl;
        std::cout<<"There is only one tree to be plotted, it does not make sense to use individual friend trees."<< std::endl;
        std::cout<<"exiting"<< std::endl;
        return;
    }
    else if (nTrees > 1 && nFriendsIndividual > 0 && nTrees != nFriendsIndividual) {
        std::cout<<"nTrees = "<< nTrees <<", nFriendsIndividual = " << nFriendsIndividual << std::endl;
        std::cout<<"exiting"<< std::endl;
        return;
    }

    // if no mode is specified (which is what happens most of the time), then it is expected that nInputFileArguments = 1.
    // so in that case : 1.) the "TChain*" objects below are effectively 1D, not 2D. 2.) the loops below have effective depth 1, not 2.
    TChain* trees[nTrees][nInputFileArguments];
    TChain* treeFriends[nFriends][nInputFileArguments];
    TChain* treeFriendsIndividual[nFriendsIndividual][nInputFileArguments];
    TChain* treeHiForestInfo[nInputFileArguments];
    for (int iInFileArg = 0; iInFileArg < nInputFileArguments; ++iInFileArg) {
        for (int i=0; i < nTrees; ++i) {
            std::string treePath = treePaths.at(i).c_str();
            trees[i][iInFileArg] = new TChain(treePath.c_str());
        }
        // initialize friend trees
        for (int i=0; i<nFriends; ++i) {
            treeFriends[i][iInFileArg] = new TChain(treeFriendsPath.at(i).c_str());
        }
        // add friends
        for (int i=0; i<nTrees; ++i) {
            for (int j=0; j<nFriends; ++j) {
                trees[i][iInFileArg]->AddFriend(treeFriends[j][iInFileArg], Form("t%d", j));
            }
        }

        // initialize individual friend trees
        for (int i=0; i < nFriendsIndividual; ++i) {
            if (treeFriendsPathIndividual.at(i).compare(CONFIGPARSER::nullInput) != 0)
                treeFriendsIndividual[i][iInFileArg] = new TChain(treeFriendsPathIndividual.at(i).c_str());
        }
        // add individual friends
        if (nFriendsIndividual > 0) {
            for (int i=0; i<nTrees; ++i) {
                if (treeFriendsPathIndividual.at(i).compare(CONFIGPARSER::nullInput) != 0)
                    trees[i][iInFileArg]->AddFriend(treeFriendsIndividual[i][iInFileArg], Form("tSelf%d", i));
            }
        }

        treeHiForestInfo[iInFileArg] = new TChain("HiForest/HiForestInfo");
        for (std::vector<std::string>::iterator it = inputFiles[iInFileArg].begin() ; it != inputFiles[iInFileArg].end(); ++it) {

            for (int i=0; i<nTrees; ++i) {
                trees[i][iInFileArg]->Add((*it).c_str());
            }
            for (int i=0; i<nFriends; ++i) {
                treeFriends[i][iInFileArg]->Add((*it).c_str());
            }
            for (int i=0; i < nFriendsIndividual; ++i) {
                if (treeFriendsPathIndividual.at(i).compare(CONFIGPARSER::nullInput) != 0)
                    treeFriendsIndividual[i][iInFileArg]->Add((*it).c_str());
            }
            treeHiForestInfo[iInFileArg]->Add((*it).c_str());
        }

        HiForestInfoController hfic(treeHiForestInfo[iInFileArg]);
        if (iInFileArg == 0)  std::cout<<"### HiForestInfo Tree ###"<< std::endl;
        else                  std::cout<<"### HiForestInfo Tree, input "<< iInFileArg+1 << " ###" << std::endl;
        hfic.printHiForestInfo();
        std::cout<<"###"<< std::endl;
    }

    if (nSelectionSplitter == 1) {
        std::cout << "nSelectionSplitter = "<< nSelectionSplitter << std::endl;
        std::cout << "selectionSplitter has been set to have exactly one selection"<< std::endl;
        std::cout << "selectionSplitter is allowed to be either empty or to have more than one selections"<< std::endl;
        std::cout << "exiting"<< std::endl;
        return;
    }
    int nSplits = 1;
    if (nSelectionSplitter > 1)  nSplits = nSelectionSplitter;

    int nSelectionsTot = nSelections * nSplits;
    int nFormulasTot = nFormulas * nSplits;

    TH1::SetDefaultSumw2();
    int nHistos = nFormulasTot;
    if (nFormulas == 1 && nSelections > nFormulas) nHistos = nSelectionsTot;
    else if (nFormulas == 1 && nSelections == 1 && nTrees > nFormulas) nHistos = nTrees * nSplits;
    else if (nFormulas > 1 && nSelections > 1 && nFormulas != nSelections) {
        std::cout << "mismatch of number of formulas and number of selections"<< std::endl;
        std::cout << "nHistos     = "<< nHistos << std::endl;
        std::cout << "nSelections = "<< nSelections << std::endl;
        std::cout << "exiting " << std::endl;
        return;
    }
    else if (nFormulas > 1 && nTrees > 1 && nFormulas != nTrees) {
        std::cout << "mismatch of number of formulas and number of trees"<< std::endl;
        std::cout << "nHistos = "<< nHistos << std::endl;
        std::cout << "nTrees  = "<< nTrees << std::endl;
        std::cout << "exiting " << std::endl;
        return;
    }
    else if (nSelections > 1 && nTrees > 1 && nSelections != nTrees) {
        std::cout << "mismatch of number of selections and number of trees"<< std::endl;
        std::cout << "nHistos     = "<< nHistos << std::endl;
        std::cout << "nSelections = "<< nSelections << std::endl;
        std::cout << "nTrees      = "<< nTrees << std::endl;
        std::cout << "exiting " << std::endl;
        return;
    }
    int nHistosInput = nHistos/nSplits;     // number of histograms without considering selectionSplitter
    std::cout << "nHistos = " << nHistos << std::endl;
    TH2D* h[nHistos];
    for (int i=0; i<nHistos; ++i) {
        int nBinsx  = (int)TH2D_Bins_List[0].at(0);
        float xLow = TH2D_Bins_List[1].at(0);
        float xUp  = TH2D_Bins_List[2].at(0);
        int nBinsy  = (int)TH2D_Bins_List[3].at(0);
        float yLow = TH2D_Bins_List[4].at(0);
        float yUp  = TH2D_Bins_List[5].at(0);
        if (nTH2D_Bins_List == nHistosInput) {
            nBinsx = (int)TH2D_Bins_List[0].at(i%nTH2D_Bins_List);
            xLow  = TH2D_Bins_List[1].at(i%nTH2D_Bins_List);
            xUp   = TH2D_Bins_List[2].at(i%nTH2D_Bins_List);
            nBinsy = (int)TH2D_Bins_List[3].at(i%nTH2D_Bins_List);
            yLow  = TH2D_Bins_List[4].at(i%nTH2D_Bins_List);
            yUp   = TH2D_Bins_List[5].at(i%nTH2D_Bins_List);
        }
        std::string title = "";
        if (nTitles == 1)  {
            if (titles.at(0).compare(CONFIGPARSER::nullInput) != 0)  title = titles.at(0).c_str();
        }
        else if (nTitles == nHistosInput)  {
            if (titles.at(i%nTitles).compare(CONFIGPARSER::nullInput) != 0)  title = titles.at(i%nTitles).c_str();
        }
        else if (nTitles == nHistos)  {
            if (titles.at(i).compare(CONFIGPARSER::nullInput) != 0)  title = titles.at(i).c_str();
        }

        std::string titleX = "";
        if (nTitlesX == 1) titleX = titlesX.at(0).c_str();
        else if (nTitlesX == nHistosInput) titleX = titlesX.at(i%nTitlesX).c_str();
        else if (nTitlesX == nHistos)      titleX = titlesX.at(i).c_str();

        std::string titleY = "";
        if (nTitlesY == 1) titleY = titlesY.at(0).c_str();
        else if (nTitlesY == nHistosInput) titleY = titlesY.at(i%nTitlesY).c_str();
        else if (nTitlesY == nHistos)      titleY = titlesY.at(i).c_str();

        h[i] = new TH2D(Form("h2D_%d", i),Form("%s;%s;%s", title.c_str(), titleX.c_str(), titleY.c_str()), nBinsx, xLow, xUp, nBinsy, yLow, yUp);
    }
    Long64_t entries[nInputFileArguments];
    Long64_t entriesSelected[nHistos];
    for (int i = 0; i < nInputFileArguments; ++i) {
        entries[i] = trees[0][i]->GetEntries();      // assume all the trees have same number of entries

        if (nInputFileArguments == 1)  {
            std::cout << "entries = " << entries[0] << std::endl;
        }
        else {
            std::cout << Form("entries[%d] = ", i) << entries[i] << std::endl;
        }
    }
    std::cout << "TTree::Draw()" <<std::endl;
    for (int i=0; i<nHistos; ++i) {

        int treeIndex = 0;
        if (nHistosInput == nTrees)  treeIndex = i%nTrees;
        std::cout << "treePath = " << treePaths.at(treeIndex).c_str() << ", ";

        int iInFileArg = 0;
        if (mode == INPUT_MODE::k_comparison) {
            iInFileArg = i%nInputFileArguments;
            std::cout << "iInFileArg = " << iInFileArg << ", ";
        }

        std::string formula = formulas.at(0).c_str();
        std::string selection = selections.at(0).c_str();
        std::string weight = weights.at(0).c_str();
        if (nHistosInput == nFormulas)  formula = formulas.at(i%nFormulas).c_str();
        if (nHistosInput == nSelections)  selection = selections.at(i%nSelections).c_str();
        if (nHistosInput == nWeights)  weight = weights.at(i%nWeights).c_str();

        std::string selectionSplit = "";
        if (nSelectionSplitter > 1)  selectionSplit = selectionSplitter.at(i/ (nHistos/nSelectionSplitter)).c_str();

        std::cout << "drawing histogram i = " << i << ", ";

        TCut selectionFinal = selectionBase.c_str();
        selectionFinal = selectionFinal && selection.c_str();
        if (selectionSplit.size() > 0)  selectionFinal = selectionFinal && selectionSplit.c_str();
        entriesSelected[i] = trees[treeIndex][iInFileArg]->GetEntries(selectionFinal.GetTitle());
        std::cout << "entriesSelected = " << entriesSelected[i] << std::endl;

        TCut weight_AND_selection = Form("(%s)*(%s)", weight.c_str(), selectionFinal.GetTitle());
        trees[treeIndex][iInFileArg]->Draw(Form("%s >> %s", formula.c_str(), h[i]->GetName()), weight_AND_selection.GetTitle(), "goff");
    }
    std::cout << "TTree::Draw() ENDED" <<std::endl;

    // print info about histograms
    for (int i=0; i<nHistos; ++i) {
        std::cout << "#####" << std::endl;
        std::cout << Form("h[%d]", i) << std::endl;
        std::string summary = summaryTH1(h[i]);
        std::cout << summary.c_str() << std::endl;
    }

    TFile *output = TFile::Open(outputFile.Data(),"RECREATE");
    output->cd();

    TH2D* h_normInt[nHistos];
    TH2D* h_normEvents[nHistos];
    for (int i=0; i<nHistos; ++i) {
        h[i]->Write();

        h_normInt[i] = (TH2D*)h[i]->Clone(Form("%s_normInt", h[i]->GetName()));
        h_normInt[i]->Scale(1./h[i]->Integral());
        h_normInt[i]->Write();

        h_normEvents[i] = (TH2D*)h[i]->Clone(Form("%s_normEvents", h[i]->GetName()));
        h_normEvents[i]->Scale(1./entriesSelected[i]);
        h_normEvents[i]->Write();
    }
    // histograms are written. After this point changes to the histograms will not be reflected in the output ROOT file.

    // set the style of the histograms for canvases to be written
    for (int i=0; i<nHistos; ++i) {
        h[i]->SetTitleOffset(titleOffsetX,"X");
        h[i]->SetTitleOffset(titleOffsetY,"Y");
        h_normInt[i]->SetTitleOffset(titleOffsetX,"X");
        h_normInt[i]->SetTitleOffset(titleOffsetY,"Y");
        h_normEvents[i]->SetTitleOffset(titleOffsetX,"X");
        h_normEvents[i]->SetTitleOffset(titleOffsetY,"Y");

        // default marker style and color
        h[i]->SetMarkerStyle(kFullCircle);
        h[i]->SetMarkerColor(kBlack);
        h_normInt[i]->SetMarkerStyle(kFullCircle);
        h_normInt[i]->SetMarkerColor(kBlack);
        h_normEvents[i]->SetMarkerStyle(kFullCircle);
        h_normEvents[i]->SetMarkerColor(kBlack);

        // no stats box in the final plots
        h[i]->SetStats(false);
        h_normInt[i]->SetStats(false);
        h_normEvents[i]->SetStats(false);
    }

    // write canvases
    TCanvas* c;
    for (int i=0; i<nHistos; ++i) {
        c = new TCanvas(Form("cnv_%d",i),"",windowWidth,windowHeight);
        c->SetTitle(h[i]->GetTitle());
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        setCanvasFinal(c, setLogx, setLogy, setLogz);
        c->cd();

        h[i]->SetTitleOffset(titleOffsetX,"X");
        h[i]->SetTitleOffset(titleOffsetY,"Y");
        h[i]->SetStats(false);
        h[i]->Draw("colz");
        c->Write();
        c->Close();         // do not use Delete() for TCanvas.

        // normalized to 1.
        c = new TCanvas(Form("cnv_%d_normInt",i),"",windowWidth,windowHeight);
        c->SetTitle(h_normInt[i]->GetTitle());
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        setCanvasFinal(c, setLogx, setLogy, setLogz);
        c->cd();

        h_normInt[i]->SetTitleOffset(titleOffsetX,"X");
        h_normInt[i]->SetTitleOffset(titleOffsetY,"Y");
        h_normInt[i]->SetStats(false);
        h_normInt[i]->Draw("colz");
        c->Write();
        c->Close();         // do not use Delete() for TCanvas.
        
        // normalized by number of events
        c = new TCanvas(Form("cnv_%d_normEvents",i),"",windowWidth,windowHeight);
        c->SetTitle(h_normEvents[i]->GetTitle());
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        setCanvasFinal(c, setLogx, setLogy, setLogz);
        c->cd();

        h_normEvents[i]->SetTitleOffset(titleOffsetX,"X");
        h_normEvents[i]->SetTitleOffset(titleOffsetY,"Y");
        h_normEvents[i]->SetStats(false);
        h_normEvents[i]->Draw("colz");
        c->Write();
        c->Close();         // do not use Delete() for TCanvas.
    }
    // canvases are written.

    // set style of the histograms for the canvases to be saved as picture
    for(int i=0; i<nHistos; ++i) {
        std::string drawOption = "colz";
        if (nDrawOptions == 1) {
            if (drawOptions.at(0).compare(CONFIGPARSER::nullInput) != 0)  drawOption = drawOptions.at(0).c_str();
        }
        else if (nDrawOptions == nHistosInput) {
            if (drawOptions.at(i).compare(CONFIGPARSER::nullInput) != 0)  drawOption = drawOptions.at(i%nDrawOptions).c_str();
        }
        // https://root.cern.ch/doc/master/classTObject.html#abe2a97d15738d5de00cd228e0dc21e56
        // TObject::SetDrawOption() is not suitable for the approach here.

        int markerStyle = GRAPHICS::markerStyle;
        if (nMarkerStyles == 1) markerStyle = GraphicsConfigurationParser::ParseMarkerStyle(markerStyles.at(0));
        else if (nMarkerStyles == nHistosInput) markerStyle = GraphicsConfigurationParser::ParseMarkerStyle(markerStyles.at(i%nMarkerStyles));
        h[i]->SetMarkerStyle(markerStyle);
        h_normInt[i]->SetMarkerStyle(markerStyle);
        h_normEvents[i]->SetMarkerStyle(markerStyle);

        int lineStyle = GRAPHICS::lineStyle;
        if (nLineStyles == 1)  lineStyle = GraphicsConfigurationParser::ParseLineStyle(lineStyles.at(0));
        else if (nLineStyles == nHistosInput)  lineStyle = GraphicsConfigurationParser::ParseLineStyle(lineStyles.at(i%nLineStyles));
        h[i]->SetLineStyle(lineStyle);
        h_normInt[i]->SetLineStyle(lineStyle);
        h_normEvents[i]->SetLineStyle(lineStyle);

        int fillStyle = GRAPHICS::fillStyle;
        if (nFillStyles == 1)  fillStyle = GraphicsConfigurationParser::ParseLineStyle(fillStyles.at(0));
        else if (nFillStyles == nHistosInput)  fillStyle = GraphicsConfigurationParser::ParseLineStyle(fillStyles.at(i%nFillStyles));
        h[i]->SetFillStyle(fillStyle);
        h_normInt[i]->SetFillStyle(fillStyle);
        h_normEvents[i]->SetFillStyle(fillStyle);

        int color = GRAPHICS::colors[i];
        if (nColors == 1) color = GraphicsConfigurationParser::ParseColor(colors.at(0));
        else if (nColors == nHistosInput) color = GraphicsConfigurationParser::ParseColor(colors.at(i%nColors));
        h[i]->SetMarkerColor(color);
        h[i]->SetLineColor(color);
        h_normInt[i]->SetMarkerColor(color);
        h_normInt[i]->SetLineColor(color);
        h_normEvents[i]->SetMarkerColor(color);
        h_normEvents[i]->SetLineColor(color);

        int fillColor = -1;
        if (nFillColors == 1) fillColor = GraphicsConfigurationParser::ParseColor(fillColors.at(0));
        else if (nFillColors == nHistosInput) fillColor = GraphicsConfigurationParser::ParseColor(fillColors.at(i%nFillColors));
        if (fillColor != -1)
        {
            h[i]->SetFillColor(fillColor);
            h_normInt[i]->SetFillColor(fillColor);
            h_normEvents[i]->SetFillColor(fillColor);
        }

        int lineColor = -1;
        if (nLineColors == 1) lineColor = GraphicsConfigurationParser::ParseColor(lineColors.at(0));
        else if (nLineColors == nHistosInput) lineColor = GraphicsConfigurationParser::ParseColor(lineColors.at(i%nLineColors));
        if (nLineColors != -1)
        {
            h[i]->SetLineColor(lineColor);
            h_normInt[i]->SetLineColor(lineColor);
            h_normEvents[i]->SetLineColor(lineColor);
        }

        if(lineWidth != INPUT_DEFAULT::lineWidth) {
            if (drawOption.find("hist") != std::string::npos) {
                h[i]->SetLineWidth(lineWidth);
                h_normInt[i]->SetLineWidth(lineWidth);
                h_normEvents[i]->SetLineWidth(lineWidth);
            }
        }

        h[i]->SetMarkerSize(markerSize);
        h_normInt[i]->SetMarkerSize(markerSize);
        h_normEvents[i]->SetMarkerSize(markerSize);
    }

    // save histograms as picture if a figure name is provided.
    // for now 2D canvases are not drawn on top, they are drawn separately.
    if (!outputFigureName.EqualTo("")) {
        TH1D* h_draw[nHistos];
        for (int i=0; i<nHistos; ++i) {
            if (drawNormalized == INPUT_TH1::k_normInt) {
                h_draw[i] = (TH1D*)h_normInt[i]->Clone(Form("h_%d_draw", i));
            }
            else if (drawNormalized == INPUT_TH1::k_normEvents) {
                h_draw[i] = (TH1D*)h_normEvents[i]->Clone(Form("h_%d_draw", i));
            }
            else {  // no normalization
                h_draw[i] = (TH1D*)h[i]->Clone(Form("h_%d_draw", i));
            }
        }

        for (int i = 0; i<nHistos; ++i) {

            c = new TCanvas(Form("cnv_drawSpectra2D_%d", i),"",windowWidth,windowHeight);
			setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
            setCanvasFinal(c, setLogx, setLogy, setLogz);
            c->cd();
            TLegend* leg = new TLegend();

            std::string drawOption = "";
            if (nDrawOptions == 1)  drawOption = drawOptions.at(0).c_str();
            else if (nDrawOptions == nHistosInput) drawOption = drawOptions.at(i%nDrawOptions).c_str();

            h_draw[i]->Draw(drawOption.c_str());

            if (nLegendEntryLabels == nHistosInput) {
                std::string label = legendEntryLabels.at(i%nLegendEntryLabels).c_str();
                std::string legendOption = "lpf";
                if (drawOption.find("hist") != std::string::npos)  legendOption = "lf";
                if (label.compare(CONFIGPARSER::nullInput) != 0)  leg->AddEntry(h_draw[i], label.c_str(), legendOption.c_str());
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
                double ymin = h[i]->GetYaxis()->GetBinLowEdge(h[i]->GetYaxis()->GetFirst());
                double ymax = h[i]->GetYaxis()->GetBinLowEdge(h[i]->GetYaxis()->GetLast()+1);

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
            std::string tmpOutputFigureName = outputFigureName.Data();
            if (tmpOutputFigureName.find(".") != std::string::npos) {     // file extension is specified
                if (nHistos > 1) {
                    // modify outputFile name
                    // if i=1, then "output.ext" becomes "output_2.ext"
                    size_t pos = tmpOutputFigureName.find_last_of(".");
                    tmpOutputFigureName.replace(pos,1, Form("_%d.", i+1));
                }
                c->SaveAs(tmpOutputFigureName.c_str());
            }
            else {  // file extension is NOT specified
                if (nHistos > 1) {
                    // modify outputFile name
                    // if i=1, then "output" becomes "output_2"
                    tmpOutputFigureName = Form("%s_%d", tmpOutputFigureName.c_str(), i+1);
                }

                c->SaveAs(Form("%s.C", tmpOutputFigureName.c_str()));
                c->SaveAs(Form("%s.png", tmpOutputFigureName.c_str()));
                c->SaveAs(Form("%s.pdf", tmpOutputFigureName.c_str()));
            }

            leg->Delete();
            c->Close();
        }
    }


    output->Close();
}

int main(int argc, char** argv)
{
    if (argc == 5) {
        drawSpectra2D(argv[1], argv[2], argv[3], argv[4]);
        return 0;
    }
    else if (argc == 4) {
        drawSpectra2D(argv[1], argv[2], argv[3]);
        return 0;
    }
    else if (argc == 3) {
        drawSpectra2D(argv[1], argv[2]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./drawSpectra2D.exe <configFile> <inputFile> <outputFile> (<outputFigureName>)"
                << std::endl;
        return 1;
    }
}
