/*
 * macro to draw 2D spectra/distribution histograms.
 * saves histograms to a .root file.
 * if "outputFigureName" is specified and "drawSame" input is set, saves the canvas as picture.
 */

#include <TFile.h>
#include <TTree.h>
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

    /*
     * drawing behavior :
     *      1. If N = # formulas and N = # selections, then N histograms will be drawn,
     *      2. If 1 = # formulas and N = # selections, then N histograms will be drawn with the same formula.
     *      3. If N = # formulas and 1 = # selections, then N histograms will be drawn with the same selection.
     *      4. else, exit.
     */
    // input for mode
    int mode = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_mode];

    // input for TTree
    std::vector<std::string> treePaths = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treePath]);
    std::vector<std::string> treeFriendsPath = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treeFriendPath]);
    std::vector<std::string> treeFriendsPathIndividual = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treeFriendPathIndividual]);
    std::vector<std::string> formulas = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treeFormula]);
    std::string selectionBase = configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treeSelectionBase];
    std::vector<std::string> selections = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treeSelection]);
    std::vector<std::string> selectionSplitter = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treeSelectionSplitter]);
    std::vector<std::string> weights = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH1_weight]);

    // input for TH1
    std::vector<std::string> titles = ConfigurationParser::ParseList(ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH1_title]));
    std::vector<std::string> titlesX = ConfigurationParser::ParseList(ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH1_titleX]));
    std::vector<std::string> titlesY = ConfigurationParser::ParseList(ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH1_titleY]));
    // nBinsx, xLow, xUp, nBinsy, yLow, yUp for a TH2D histogram
    std::vector<std::vector<float>> TH2D_Bins_List = ConfigurationParser::ParseListTH2D_Bins(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH2D_Bins_List]);
    float titleOffsetX = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_titleOffsetX];
    float titleOffsetY = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_titleOffsetY];
    float markerSize = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_markerSize];
    int drawNormalized = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_drawNormalized];
    std::vector<std::string> drawOptions = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_drawOption]);
    std::vector<std::string> markerStyles = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_markerStyle]);
    std::vector<std::string> lineStyles = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_lineStyle]);
    std::vector<std::string> fillStyles = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_fillStyle]);
    std::vector<std::string> colors = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_color]);
    std::vector<std::string> fillColors = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_fillColor]);
    std::vector<std::string> lineColors = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_lineColor]);
    int lineWidth = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_lineWidth];

    // input for TLegend
    std::vector<std::string> legendEntryLabels = ConfigurationParser::ParseList(ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_legendEntryLabel]));
    std::string legendPosition = configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_legendPosition];
    float legendOffsetX = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_legendOffsetX];
    float legendOffsetY = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_legendOffsetY];
    int legendBorderSize = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_legendBorderSize];
    float legendWidth = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_legendWidth];
    float legendHeight = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_legendHeight];
    float legendTextSize = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_legendTextSize];

    // input for text objects
    std::string tmpText = ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_text]);
    std::vector<std::string> textLines = ConfigurationParser::ParseList(tmpText);
    int textFont = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_textFont];
    float textSize = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_textSize];
    std::string textPosition = configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_textPosition];
    float textOffsetX = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_textOffsetX];
    float textOffsetY = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_textOffsetY];

    std::string tmpTextOverPad = ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_textAbovePad]);
    std::vector<std::string> textsOverPad = ConfigurationParser::ParseList(tmpTextOverPad);
    std::vector<std::string> textsOverPadAlignments = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_textAbovePadAlign]);
    int textAbovePadFont = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_textAbovePadFont];
    float textAbovePadSize = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_textAbovePadSize];
    float textAbovePadOffsetX = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_textAbovePadOffsetX];
    float textAbovePadOffsetY = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_textAbovePadOffsetY];

    // input for TLine
    // y-axis positions of the horizontal lines to be drawn
    std::vector<float> TLines_horizontal = ConfigurationParser::ParseListFloat(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TLine_horizontal]);
    std::vector<std::string> lineStyles_horizontal = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_LineStyle_horizontal]);
    // x-axis positions of the vertical lines to be drawn
    std::vector<float> TLines_vertical = ConfigurationParser::ParseListFloat(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TLine_vertical]);
    std::vector<std::string> lineStyles_vertical = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_LineStyle_vertical]);

    // input for TCanvas
    int windowWidth = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_windowWidth];
    int windowHeight = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_windowHeight];
    float leftMargin = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_leftMargin];
    float rightMargin = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_rightMargin];
    float bottomMargin = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_bottomMargin];
    float topMargin = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_topMargin];
    int setLogx = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_setLogx];
    int setLogy = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_setLogy];
    int setLogz = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_setLogz];
      
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
    
    // cut configuration

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
    TFile* output = TFile::Open(outputFile.Data(),"RECREATE");
    output->cd();

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

    // if no mode is specified (which is what happens most of the time), then it is expected that nInputFileArguments = 1.
    // so in that case : 1.) the "TTree*" objects below are effectively 1D, not 2D. 2.) the loops below have effective depth 1, not 2.
    TTree* trees[nTrees][nInputFileArguments];
    TTree* treeFriends[nFriends][nInputFileArguments];
    TTree* treeFriendsIndividual[nFriendsIndividual][nInputFileArguments];
    TTree* treeHiForestInfo[nInputFileArguments];

    Long64_t entries[nInputFileArguments];
    Long64_t entriesSelected[nHistos];
    std::fill_n(entriesSelected, nHistos, 0);

    int nFiles[nInputFileArguments];
    TFile* fileTmp = 0;

    std::cout << "initial reading to get the number of entries (if there is only one input file) and HiForest info" << std::endl;
    for (int iInFileArg = 0; iInFileArg < nInputFileArguments; ++iInFileArg) {

        nFiles[iInFileArg] = inputFiles[iInFileArg].size();
        if (nInputFileArguments > 1) {
            std::cout <<"iInFileArg = " << iInFileArg << " , "<< std::endl;
        }

        // read the first file only to get the HiForest info
        std::string inputPath = inputFiles[iInFileArg].at(0).c_str();
        fileTmp = new TFile(inputPath.c_str(), "READ");

        if (nFiles[iInFileArg] == 1) {
            // read one tree only to get the number of entries
            trees[0][iInFileArg] = (TTree*)fileTmp->Get(treePaths.at(0).c_str());
            entries[iInFileArg] = trees[0][iInFileArg]->GetEntries();
            std::cout << "entries = " << entries[iInFileArg] << std::endl;
        }

        treeHiForestInfo[0] = (TTree*)fileTmp->Get("HiForest/HiForestInfo");
        HiForestInfoController hfic(treeHiForestInfo[0]);
        if (iInFileArg == 0)  std::cout<<"### HiForestInfo Tree ###"<< std::endl;
        else                  std::cout<<"### HiForestInfo Tree, input "<< iInFileArg+1 << " ###" << std::endl;
        hfic.printHiForestInfo();
        std::cout<<"###"<< std::endl;

        fileTmp->Close();
    }

    std::cout << "TTree::Draw()" <<std::endl;
    for (int iInFileArg = 0; iInFileArg < nInputFileArguments; ++iInFileArg) {

        if (nInputFileArguments > 1) {
            std::cout <<"iInFileArg = " << iInFileArg << std::endl;
        }

        entries[iInFileArg] = 0;
        for (int iFile = 0; iFile < nFiles[iInFileArg]; ++iFile) {

            std::string inputPath = inputFiles[iInFileArg].at(iFile).c_str();
            std::cout <<"iFile = " << iFile << " , " ;
            std::cout <<"reading input file : " << inputPath.c_str() << std::endl;
            fileTmp = new TFile(inputPath.c_str(), "READ");

            // check if the file is usable, if not skip the file.
            bool isGood = true;
            if (fileTmp->IsZombie()) {
                isGood = false;
                std::cout << "File is zombie. skipping file." << std::endl;
            }
            if (fileTmp->TestBits(TFile::kRecovered)) {
                isGood = false;
                std::cout << "File has kRecovered flag. skipping file." << std::endl;
            }
            if (!isGood)  continue;

            for (int i=0; i<nTrees; ++i) {
                trees[i][iInFileArg] = (TTree*)fileTmp->Get(treePaths.at(i).c_str());
            }
            for (int i=0; i<nFriends; ++i) {
                treeFriends[i][iInFileArg] = (TTree*)fileTmp->Get(treeFriendsPath.at(i).c_str());
            }
            // add friends
            for (int i=0; i<nTrees; ++i) {
                for (int j=0; j<nFriends; ++j) {
                    trees[i][iInFileArg]->AddFriend(treeFriends[j][iInFileArg], Form("t%d", j));
                }
            }
            for (int i=0; i < nFriendsIndividual; ++i) {
                if (treeFriendsPathIndividual.at(i).compare(CONFIGPARSER::nullInput) != 0) {
                    treeFriendsIndividual[i][iInFileArg] = (TTree*)fileTmp->Get(treeFriendsPathIndividual.at(i).c_str());
                }
            }
            if (nFriendsIndividual > 0) {
                for (int i=0; i<nTrees; ++i) {
                    if (treeFriendsPathIndividual.at(i).compare(CONFIGPARSER::nullInput) != 0) {
                        trees[i][0]->AddFriend(treeFriendsIndividual[i][0], Form("tSelf%d", i));
                    }
                }
            }

            Long64_t entriesTmp = trees[0][iInFileArg]->GetEntries();      // assume all the trees have same number of entries
            entries[iInFileArg] += entriesTmp;

            if (nInputFileArguments == 1)  {
                std::cout << "entries in File = " << entriesTmp << std::endl;
            }
            else {
                std::cout << Form("entries[%d] = ", iInFileArg) << entriesTmp << std::endl;
            }

            output->cd();
            for (int i=0; i<nHistos; ++i) {

                int treeIndex = 0;
                if (nHistosInput == nTrees)  treeIndex = i%nTrees;
                // std::cout << "treePath = " << treePaths.at(treeIndex).c_str() << ", ";

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

                // std::cout << "drawing histogram i = " << i << ", ";

                TCut selectionFinal = selectionBase.c_str();
                selectionFinal = selectionFinal && selection.c_str();
                if (selectionSplit.size() > 0)  selectionFinal = selectionFinal && selectionSplit.c_str();
                Long64_t entriesSelectedTmp = trees[treeIndex][iInFileArg]->GetEntries(selectionFinal.GetTitle());
                // std::cout << "entriesSelected in file = " << entriesSelectedTmp << std::endl;
                entriesSelected[i] += entriesSelectedTmp;

                TCut weight_AND_selection = Form("(%s)*(%s)", weight.c_str(), selectionFinal.GetTitle());
                trees[treeIndex][iInFileArg]->Draw(Form("%s >>+ %s", formula.c_str(), h[i]->GetName()), weight_AND_selection.GetTitle(), "goff");
            }
            fileTmp->Close();
        }
    }
    std::cout << "TTree::Draw() ENDED" <<std::endl;
    for (int i = 0; i < nInputFileArguments; ++i) {

        if (nInputFileArguments == 1)  {
            std::cout << "entries = " << entries[0] << std::endl;
        }
        else {
            std::cout << Form("entries[%d] = ", i) << entries[i] << std::endl;
        }
    }
    std::cout << "### selected entries" << std::endl;
    for (int i = 0; i < nHistos; ++i) {

        std::cout << "TH1D i = " << i << ", ";
        int treeIndex = 0;
        if (nHistosInput == nTrees)  treeIndex = i%nTrees;
        std::cout << "treePath = " << treePaths.at(treeIndex).c_str() << ", ";

        std::cout << "entriesSelected = " << entriesSelected[i] << std::endl;
    }
    std::cout << "###" << std::endl;

    // print info about histograms
    for (int i=0; i<nHistos; ++i) {
        std::cout << "#####" << std::endl;
        std::cout << Form("h[%d]", i) << std::endl;
        std::string summary = summaryTH1(h[i]);
        std::cout << summary.c_str() << std::endl;
    }

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
        TLatex* latex = 0;
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
        TLatex* latexOverPad = 0;
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
        c->Write();

        // save histograms as picture if a figure name is provided.
        // for now 2D canvases are not drawn on top, they are drawn separately.
        if (!outputFigureName.EqualTo("")) {
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
        }

        leg->Delete();
        c->Close();
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
