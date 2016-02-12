/*
 * macro to draw spectra/distribution histograms.
 * saves histograms to a .root file.
 * if "outputFigureName" is specified and "drawSame" input is set, saves the canvas as picture.
 */

#include <TFile.h>
#include <TChain.h>
#include <TH1.h>
#include <TH1D.h>
#include <TCut.h>
#include <TCanvas.h>
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

void drawSpectra(const TString configFile, const TString inputFile, const TString outputFile = "drawSpectra.root", const TString outputFigureName = "");

void drawSpectra(const TString configFile, const TString inputFile, const TString outputFile, const TString outputFigureName)
{
    std::cout<<"running drawSpectra()"<<std::endl;
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
    // input for TTree
    std::string treePath;
    std::vector<std::string> treeFriendsPath;
    std::vector<std::string> formulas;
    std::string selectionBase;
    std::vector<std::string> selections;
    std::vector<std::string> selectionSplitter;
    std::vector<std::string> weights;

    // input for TH1
    std::vector<std::string>  titles;
    std::vector<std::string>  titlesX;
    std::vector<std::string>  titlesY;
    std::vector<std::vector<float>> TH1D_Bins_List;      // nBins, xLow, xUp for the TH1D histogram
    float titleOffsetX;
    float titleOffsetY;
    float yMin;
    float yMax;
    int drawSame;
    int drawNormalized;
    std::vector<std::string> drawOptions;
    std::vector<std::string> markerStyles;
    std::vector<std::string> lineStyles;
    std::vector<std::string> fillStyles;
    std::vector<std::string> colors;

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
        treePath  = configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treePath];
        treeFriendsPath = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treeFriends_List]);
        formulas = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treeFormula]);
        selectionBase = configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treeSelectionBase];
        selections = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treeSelection]);
        selectionSplitter = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treeSelectionSplitter]);
        weights = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH1_weight]);

        titles = ConfigurationParser::ParseList(ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH1_title]));
        titlesX = ConfigurationParser::ParseList(ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH1_titleX]));
        titlesY = ConfigurationParser::ParseList(ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH1_titleY]));
        TH1D_Bins_List = ConfigurationParser::ParseListTH1D_Bins(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH1D_Bins_List]);
        titleOffsetX = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_titleOffsetX];
        titleOffsetY = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_titleOffsetY];
        yMin = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_TH1_yMin];
        yMax = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_TH1_yMax];
        drawSame = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_drawSame];
        drawNormalized = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_drawNormalized];
        drawOptions = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_drawOption]);
        markerStyles = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_markerStyle]);
        lineStyles = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_lineStyle]);
        fillStyles = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_fillStyle]);
        colors = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_color]);

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

        windowWidth = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_windowWidth];
        windowHeight = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_windowHeight];
        leftMargin   = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_leftMargin];
        rightMargin  = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_rightMargin];
        bottomMargin = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_bottomMargin];
        topMargin    = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_topMargin];
        setLogx = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_setLogx];
        setLogy = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_setLogy];
    }
    else {
        treePath = "";
        formulas.push_back("Entry$");
        selectionBase = "";
        selections.push_back("1 == 1");
        weights.push_back("1");

        TH1D_Bins_List.resize(3);
        TH1D_Bins_List[0].push_back(100);    // nBins
        TH1D_Bins_List[1].push_back(0);      // xLow
        TH1D_Bins_List[2].push_back(100);    // xUp
        titleOffsetX = 1;
        titleOffsetY = 1;
        yMin = 0;
        yMax = -1;
        drawSame = 0;
        drawNormalized = 0;
        
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
    }
    // set default values
    if (selections.size() == 0) selections.push_back("1");
    if (weights.size() == 0)    weights.push_back(INPUT_DEFAULT::TH1_weight.c_str());   // default weight = 1.
    if (titleOffsetX == 0) titleOffsetX = INPUT_DEFAULT::titleOffsetX;
    if (titleOffsetY == 0) titleOffsetY = INPUT_DEFAULT::titleOffsetY;
    if (yMin == 0 && yMax == 0)  yMax = -1;
    else if (yMin <= 0 && setLogy > 0)  yMin = resetTH1axisMin4LogScale(yMin, "y");
    if (drawNormalized >= INPUT_TH1::kN_TYPE_NORM) drawNormalized = INPUT_DEFAULT::drawNormalized;

    if (textFont == 0)  textFont = INPUT_DEFAULT::textFont;
    if (textSize == 0)  textSize = INPUT_DEFAULT::textSize;

    if (windowWidth  == 0)  windowWidth = INPUT_DEFAULT::windowWidth;
    if (windowHeight == 0)  windowHeight = INPUT_DEFAULT::windowHeight;
    if (leftMargin == 0) leftMargin = INPUT_DEFAULT::leftMargin;
    if (rightMargin == 0) rightMargin = INPUT_DEFAULT::rightMargin;
    if (bottomMargin == 0) bottomMargin = INPUT_DEFAULT::bottomMargin;
    if (topMargin == 0) topMargin = INPUT_DEFAULT::topMargin;

    int nFriends = treeFriendsPath.size();
    int nFormulas = formulas.size();
    int nSelections = selections.size();
    int nSelectionSplitter = selectionSplitter.size();
    int nWeights = weights.size();
    int nTitles = titles.size();
    int nTitlesX = titlesX.size();
    int nTitlesY = titlesY.size();
    int nTH1D_Bins_List = TH1D_Bins_List[0].size();
    int nDrawOptions = drawOptions.size();
    int nMarkerStyles = markerStyles.size();
    int nLineStyles = lineStyles.size();
    int nFillStyles = fillStyles.size();
    int nColors = colors.size();
    int nLegendEntryLabels = legendEntryLabels.size();
    int nTextLines = textLines.size();
    // verbose about input configuration
    std::cout<<"Input Configuration :"<<std::endl;
    std::cout << "treePath = " << treePath.c_str() << std::endl;
    std::cout << "nFriends = " << nFriends << std::endl;
    for (int i=0; i<nFriends; ++i) {
        std::cout << Form("treeFriends[%d] = %s", i, treeFriendsPath.at(i).c_str()) << std::endl;
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
    std::cout << "nTH1D_Bins_List = " << nTH1D_Bins_List << std::endl;
    for (int i=0; i<nTH1D_Bins_List; ++i) {
        std::cout << Form("TH1D_Bins_List[%d] = { ", i);
        std::cout << Form("%.0f, ", TH1D_Bins_List[0].at(i));
        std::cout << Form("%f, ", TH1D_Bins_List[1].at(i));
        std::cout << Form("%f }", TH1D_Bins_List[2].at(i)) << std::endl;;
    }
    std::cout << "titleOffsetX = " << titleOffsetX << std::endl;
    std::cout << "titleOffsetY = " << titleOffsetY << std::endl;
    std::cout << "yMin = " << yMin << std::endl;
    std::cout << "yMax = " << yMax << std::endl;
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

    std::cout << "windowWidth = " << windowWidth << std::endl;
    std::cout << "windowHeight = " << windowHeight << std::endl;
    std::cout << "leftMargin   = " << leftMargin << std::endl;
    std::cout << "rightMargin  = " << rightMargin << std::endl;
    std::cout << "bottomMargin = " << bottomMargin << std::endl;
    std::cout << "topMargin    = " << topMargin << std::endl;
    std::cout << "setLogx = " << setLogx << std::endl;
    std::cout << "setLogy = " << setLogy << std::endl;

    if (configCuts.isValid) {

    }
    else {

    }

    // verbose about cut configuration
    std::cout<<"Cut Configuration :"<<std::endl;

    std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.Data());

    std::cout<<"input ROOT files : num = "<<inputFiles.size()<< std::endl;
    std::cout<<"#####"<< std::endl;
    for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
        std::cout<<(*it).c_str()<< std::endl;
    }
    std::cout<<"##### END #####"<< std::endl;

    TChain* tree = new TChain(treePath.c_str());
    TChain* treeFriends[nFriends];
    for (int i=0; i<nFriends; ++i) {
        treeFriends[i] = new TChain(treeFriendsPath.at(i).c_str());
        tree->AddFriend(treeFriends[i], Form("t%d", i));
    }

    for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
       tree->Add((*it).c_str());
       for (int i=0; i<nFriends; ++i) {
           treeFriends[i]->Add((*it).c_str());
       }
    }

    if (nSelectionSplitter == 1) {
        std::cout << "nSelectionMultiplier = "<< nSelectionSplitter << std::endl;
        std::cout << "selectionMultiplier has been set to have exactly one selection"<< std::endl;
        std::cout << "selectionMultiplier is allowed to be either empty or to have more than one selections"<< std::endl;
        std::cout << "exiting"<< std::endl;
        return;
    }
    int nSplits = 1;
    if (nSelectionSplitter > 1)  nSplits = nSelectionSplitter;

    int nSelectionsTot = nSelections * nSplits;
    int nFormulasTot = nFormulas * nSplits;

    TH1::SetDefaultSumw2();
    int nHistos = nFormulasTot;
    int nHistosInput = nHistos/nSplits;     // number of histograms without considering selectionSplitter
    if (nFormulasTot == 1 && nSelectionsTot > nFormulas) nHistos = nSelectionsTot;
    else if (nFormulasTot > 1 && nSelectionsTot > 1 && nFormulasTot != nSelectionsTot) {
        std::cout << "mismatch of number of formulas and number of selections"<< std::endl;
        std::cout << "nHistos     = "<< nHistos << std::endl;
        std::cout << "nSelections = "<< nSelections << std::endl;
        std::cout << "exiting " << std::endl;
        return;
    }
    std::cout << "nHistos = " << nHistos << std::endl;
    TH1D* h[nHistos];
    for (int i=0; i<nHistos; ++i) {
        int nBins  = (int)TH1D_Bins_List[0].at(0);
        float xLow = TH1D_Bins_List[1].at(0);
        float xUp  = TH1D_Bins_List[2].at(0);
        if (nTH1D_Bins_List == nHistosInput) {
            nBins = (int)TH1D_Bins_List[0].at(i%nTH1D_Bins_List);
            xLow  = TH1D_Bins_List[1].at(i%nTH1D_Bins_List);
            xUp   = TH1D_Bins_List[2].at(i%nTH1D_Bins_List);
        }
        std::string title = "";
        if (nTitles == 1) title = titles.at(0).c_str();
        else if (nTitles == nHistosInput) title = titles.at(i%nTitles).c_str();

        std::string titleX = "";
        if (nTitlesX == 1) titleX = titlesX.at(0).c_str();
        else if (nTitlesX == nHistosInput) titleX = titlesX.at(i%nTitlesX).c_str();

        std::string titleY = "";
        if (nTitlesY == 1) titleY = titlesY.at(0).c_str();
        else if (nTitlesY == nHistosInput) titleY = titlesY.at(i%nTitlesY).c_str();

        h[i] = new TH1D(Form("h_%d", i),Form("%s;%s;%s", title.c_str(), titleX.c_str(), titleY.c_str()), nBins, xLow, xUp);

        if (yMax > yMin)  h[i]->SetAxisRange(yMin, yMax, "Y");
    }

    Long64_t entries = tree->GetEntries();
    Long64_t entriesSelected[nHistos];
    std::cout << "entries = " << entries << std::endl;
    std::cout << "TTree::Draw() : " << treePath.c_str() <<std::endl;
    for (int i=0; i<nHistos; ++i) {

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
        entriesSelected[i] = tree->GetEntries(selectionFinal.GetTitle());
        std::cout << "entriesSelected = " << entriesSelected[i] << std::endl;

        TCut weight_AND_selection = Form("(%s)*(%s)", weight.c_str(), selectionFinal.GetTitle());
        tree->Draw(Form("%s >> %s", formula.c_str(), h[i]->GetName()), weight_AND_selection.GetTitle(), "goff");
    }
    std::cout <<  "TTree::Draw() ENDED : " << treePath.c_str() <<std::endl;
    std::cout << "entries = " << entries << std::endl;

    // print info about histograms
    for (int i=0; i<nHistos; ++i) {
        std::cout << "#####" << std::endl;
        std::cout << Form("h[%d]", i) << std::endl;
        std::string summary = summaryTH1(h[i]);
        std::cout << summary.c_str() << std::endl;
    }

    TFile *output = TFile::Open(outputFile.Data(),"RECREATE");
    output->cd();

    TH1D* h_normInt[nHistos];
    TH1D* h_normEvents[nHistos];
    for (int i=0; i<nHistos; ++i) {
        h[i]->Write();

        h_normInt[i] = (TH1D*)h[i]->Clone(Form("%s_normInt", h[i]->GetName()));
        h_normInt[i]->Scale(1./h[i]->Integral());
        h_normInt[i]->Write();

        h_normEvents[i] = (TH1D*)h[i]->Clone(Form("%s_normEvents", h[i]->GetName()));
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
        setCanvasFinal(c, setLogx, setLogy);
        c->cd();

        // set the style of the histograms for canvases to be written
        h[i]->SetTitleOffset(titleOffsetX,"X");
        h[i]->SetTitleOffset(titleOffsetY,"Y");
        h[i]->SetMarkerStyle(kFullCircle);
        h[i]->SetMarkerColor(kBlack);
        h[i]->SetStats(false);              // no stats box in the final plots
        h[i]->Draw("e");
        c->Write();
        c->Close();         // do not use Delete() for TCanvas.

        // normalized to 1.
        c = new TCanvas(Form("cnv_%d_normInt",i),"",windowWidth,windowHeight);
        c->SetTitle(h_normInt[i]->GetTitle());
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        setCanvasFinal(c, setLogx, setLogy);
        c->cd();

        // set the style of the histograms for canvases to be written
        h_normInt[i]->SetTitleOffset(titleOffsetX,"X");
        h_normInt[i]->SetTitleOffset(titleOffsetY,"Y");
        h_normInt[i]->SetMarkerStyle(kFullCircle);
        h_normInt[i]->SetMarkerColor(kBlack);
        h_normInt[i]->SetStats(false);  // no stats box in the final plots
        h_normInt[i]->Draw("e");
        c->Write();
        c->Close();         // do not use Delete() for TCanvas.

        // normalized by number of events
        c = new TCanvas(Form("cnv_%d_normEvents",i),"",windowWidth,windowHeight);
        c->SetTitle(h_normEvents[i]->GetTitle());
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        setCanvasFinal(c, setLogx, setLogy);
        c->cd();

        // set the style of the histograms for canvases to be written
        h_normEvents[i]->SetTitleOffset(titleOffsetX,"X");
        h_normEvents[i]->SetTitleOffset(titleOffsetY,"Y");
        h_normEvents[i]->SetMarkerStyle(kFullCircle);
        h_normEvents[i]->SetMarkerColor(kBlack);
        h_normEvents[i]->SetStats(false);   // no stats box in the final plots
        h_normEvents[i]->Draw("e");
        c->Write();
        c->Close();         // do not use Delete() for TCanvas.
    }
    // canvases are written.

    // set style of the histograms for the canvases to be saved as picture
    for(int i=0; i<nHistos; ++i) {
        std::string drawOption = "";
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
    }

    // save histograms as picture if a figure name is provided.
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

        if (drawSame == 0) {    // histograms will be plotted separately.
            for (int i=0; i<nHistos; ++i) {
                c = new TCanvas(Form("cnv_%d",i),"",windowWidth,windowHeight);
                setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
                setCanvasFinal(c, setLogx, setLogy);
                c->cd();

                std::string drawOption = "";
                if (nDrawOptions == 1)  drawOption = drawOptions.at(0).c_str();
                else if (nDrawOptions == nHistosInput) drawOption = drawOptions.at(i%nDrawOptions).c_str();

                h_draw[i]->SetMarkerColor(kBlack);
                h_draw[i]->SetLineColor(kBlack);
                h_draw[i]->Draw(drawOption.c_str());

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
                c->Close();         // do not use Delete() for TCanvas.
            }
        }
        else if (drawSame > 0) {    // histograms will be plotted to the same canvas
            c = new TCanvas("cnv_drawSpectra","",windowWidth,windowHeight);
            setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
            setCanvasFinal(c, setLogx, setLogy);
            c->cd();

            // set maximum/minimum of y-axis
            if (yMin > yMax) {
                double histMin = h_draw[0]->GetMinimum();
                double histMax = h_draw[0]->GetMaximum();
                for (int i = 1; i<nHistos; ++i) {
                    if (h_draw[i]->GetMinimum() < histMin)   histMin = h_draw[i]->GetMinimum();
                    if (h_draw[i]->GetMaximum() > histMax)   histMax = h_draw[i]->GetMaximum();
                }
                if (setLogy == 0) h_draw[0]->SetMinimum(histMin-TMath::Abs(histMin)*0.1);
                h_draw[0]->SetMaximum(histMax+TMath::Abs(histMax)*0.1*TMath::Power(10,setLogy));
            }

            TLegend* leg = new TLegend();
            for (int i = 0; i<nHistos; ++i) {

                std::string drawOption = "";
                if (nDrawOptions == 1)  drawOption = drawOptions.at(0).c_str();
                else if (nDrawOptions == nHistosInput) drawOption = drawOptions.at(i%nDrawOptions).c_str();

                h_draw[i]->Draw(Form("%s same", drawOption.c_str()));

                if (nLegendEntryLabels == nHistosInput) {
                    std::string label = legendEntryLabels.at(i%nLegendEntryLabels).c_str();
                    if (label.compare(CONFIGPARSER::nullInput) == 0)  continue;

                    std::string legendOption = "lpf";
                    if (drawOption.find("hist") != std::string::npos)  legendOption = "lf";
                    leg->AddEntry(h_draw[i], label.c_str(), legendOption.c_str());
                }
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

            c->Write();
            // saving histograms
            std::string tmpOutputFigureName = outputFigureName.Data();
            if (tmpOutputFigureName.find(".") != std::string::npos) {     // file extension is specified
                c->SaveAs(tmpOutputFigureName.c_str());
            }
            else {  // file extension is NOT specified

                c->SaveAs(Form("%s.C", tmpOutputFigureName.c_str()));
                c->SaveAs(Form("%s.png", tmpOutputFigureName.c_str()));
                c->SaveAs(Form("%s.pdf", tmpOutputFigureName.c_str()));
            }
            c->Close();
        }
    }

    output->Close();
}

int main(int argc, char** argv)
{
    if (argc == 5) {
        drawSpectra(argv[1], argv[2], argv[3], argv[4]);
        return 0;
    }
    else if (argc == 4) {
        drawSpectra(argv[1], argv[2], argv[3]);
        return 0;
    }
    else if (argc == 3) {
        drawSpectra(argv[1], argv[2]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./drawSpectra.exe <configFile> <inputFile> <outputFile> (<outputFigureName>)"
                << std::endl;
        return 1;
    }
}
