/*
 * macro to draw spectra/distribution histograms.
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
#include <TPad.h>
#include <TLine.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TMath.h>

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#include "../TreeHeaders/CutConfigurationTree.h"
#include "../Utilities/interface/ArgumentParser.h"
#include "../Utilities/interface/CutConfigurationParser.h"
#include "../Utilities/interface/InputConfigurationParser.h"
#include "../Utilities/interface/GraphicsConfigurationParser.h"
#include "../Utilities/interface/HiForestInfoController.h"
#include "../Utilities/fileUtil.h"
#include "../Utilities/styleUtil.h"
#include "../Utilities/th1Util.h"

///// global variables
/// configuration variables
/*
* drawing behavior :
*      1. If N = # formulas and N = # selections, then N histograms will be drawn,
*      2. If 1 = # formulas and N = # selections, then N histograms will be drawn with the same formula.
*      3. If N = # formulas and 1 = # selections, then N histograms will be drawn with the same selection.
*      4. else, exit.
*/
// input for mode
int mode;

// input for TTree;
std::vector<std::string> treePaths;
std::vector<std::string> treeFriendsPath;
std::vector<std::string> treeFriendsPathIndividual;
std::vector<std::string> formulas;
std::string selectionBase;
std::vector<std::string> selections;
std::vector<std::string> selectionSplitter;
std::vector<std::string> weights;

// input for TH1;
std::vector<std::string> titles;
std::vector<std::string> titlesX;
std::vector<std::string> titlesY;
// nBins, xLow, xUp for the TH1D histogram;
std::vector<CONFIGPARSER::TH1Axis> TH1D_Bins_List;
std::vector<CONFIGPARSER::TH2DAxis> TH2D_Bins_List;
float binsLogScaleX;
float binsLogScaleY;
float titleOffsetX;
float titleOffsetY;
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

// input for TLegend;
std::vector<std::string> legendEntryLabels;
std::string legendPosition;
float legendOffsetX;
float legendOffsetY;
int legendBorderSize;
float legendWidth;
float legendHeight;
float legendTextSize;

// input for text objects;
std::string tmpText;
std::vector<std::string> textLines;
std::vector<int> textLinePadIndices;
std::vector<int> textFonts;
std::vector<float> textSizes;
std::vector<std::string> textPositions;
std::vector<float> textOffsetsX;
std::vector<float> textOffsetsY;

std::string tmpTextOverPad;
std::vector<std::string> textsOverPad;
std::vector<std::string> textsOverPadAlignments;
int textAbovePadFont;
float textAbovePadSize;
float textAbovePadOffsetX;
float textAbovePadOffsetY;

// input for TLine
// y-axis positions of the horizontal lines to be drawn
std::vector<float> TLines_horizontal;
std::vector<std::string> lineStyles_horizontal;
// x-axis positions of the vertical lines to be drawn
std::vector<float> TLines_vertical;
std::vector<std::string> lineStyles_vertical;

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

int nTrees;
int nFriends;
int nFriendsIndividual;
int nFormulas;
int nSelections;
int nSelectionSplitter;
int nWeights;
int nTitles;
int nTitlesX;
int nTitlesY;
int nTH1D_Bins_List;
int nTH2D_Bins_List;
int nDrawOptions;
int nMarkerStyles;
int nLineStyles;
int nFillStyles;
int nColors;
int nFillColors;
int nLineColors;
int nLegendEntryLabels;
int nTextLines;
int nTextFonts;
int nTextSizes;
int nTextPositions;
int nTextOffsetsX;
int nTextOffsetsY;
int nTextsOverPad;
int nTextsOverPadAlignments;
int nTLines_horizontal;
int nLineStyles_horizontal;
int nTLines_vertical;
int nLineStyles_vertical;
/// configuration variables - END
bool noLoop;
enum MODES {
    kTH1D,
    kTH2D,
    kN_MODES
};
std::vector<std::vector<std::string>> inputFiles;
std::vector<std::string> inputSamples;
int nInputSamples;
int nSplits;
int nSelectionsTot;
int nFormulasTot;
std::vector<Long64_t> entries;
std::vector<Long64_t> entriesSelected;
std::vector<Long64_t> entriesBaseSelection;
int nHistos;
int nHistosInput;
int nPads;
std::vector<TH1*> h;
std::vector<TH1*> h_scaleWidth;
std::vector<TH1*> h_normInt;
std::vector<TH1*> h_normEvents;         // normalized by the number of events that pass full selection. Ex : hiBin < 60 && jtpt > 30
std::vector<TH1*> h_normEventsBaseSel;  // normalized by the number of events that pass base selection. Ex : hiBin < 60
std::vector<TH1*> h_nums;      // histograms to store numbers
std::vector<TH1*> h_draw;
// store formula, selection and weight used for each histogram. Helps to look up what was used for a specific histogram.
std::vector<std::string> histFormulas;
std::vector<std::string> histSelections;
std::vector<std::string> histWeights;

std::string outputFigureStr;
///// global variables - END

int readConfiguration(const TString configFile, const TString inputFile = "");
void printConfiguration();
int preLoop(TFile* input = 0, bool makeNew = true);
int postLoop();
void setAndDrawLatex(TPad* pad, int iPad);
void setAndDrawLatexOverPad(TPad* pad);
void setAndDrawLinesHorizontal(TPad* pad);
void setAndDrawLinesVertical(TPad* pad);
void saveAsFigure(TCanvas* c, int iCanvas);
void drawSpectra(const TString configFile, const TString inputFile, const TString outputFile = "drawSpectra.root", const TString outputFigureName = "");
void drawSpectraNoLoop(const TString configFile, const TString inputFile, const TString outputFile = "drawSpectra.root", const TString outputFigureName = "");

void drawSpectra(const TString configFile, const TString inputFile, const TString outputFile, const TString outputFigureName)
{
    std::cout<<"running drawSpectra()"<<std::endl;
    std::cout<<"configFile  = "<< configFile.Data() <<std::endl;
    std::cout<<"inputFile   = "<< inputFile.Data()  <<std::endl;
    std::cout<<"outputFile  = "<< outputFile.Data() <<std::endl;
    outputFigureStr = outputFigureName.Data();

    if (readConfiguration(configFile, inputFile) != 0)  return;
    printConfiguration();

    std::cout<<"Input handling :"<< std::endl;
    inputSamples = InputConfigurationParser::ParseFileArgument(inputFile.Data());
    nInputSamples = inputSamples.size();

    std::cout<<"nInputSamples (number of different input samples) = "<< nInputSamples << std::endl;
    for (int i = 0; i < nInputSamples; ++i) {
        std::cout << Form("inputSamples[%d] = %s", i, inputSamples.at(i).c_str()) << std::endl;
    }

    inputFiles.clear();
    inputFiles.resize(nInputSamples);
    std::cout<<"#####"<< std::endl;
    for (int i = 0; i < nInputSamples; ++i) {

        if (nInputSamples > 1) {
            std::cout<<"###"<< std::endl;
            std::cout<<"inputFileArgument = " << inputSamples.at(i).c_str() << std::endl;
        }

        inputFiles[i] = InputConfigurationParser::ParseFiles(inputSamples.at(i));
        std::cout<<"input ROOT files : num = " << inputFiles[i].size() << std::endl;
        for (std::vector<std::string>::iterator it = inputFiles[i].begin() ; it != inputFiles[i].end(); ++it) {
            std::cout<<(*it).c_str()<< std::endl;
        }
    }
    std::cout<<"##### END #####"<< std::endl;

    // This macro uses TTree::Draw(), the output file must defined before the output objects
    TFile* output = TFile::Open(outputFile.Data(),"RECREATE");
    if (preLoop() != 0) return;

    // so in that case : 1.) the "TTree*" objects below are effectively 1D, not 2D. 2.) the loops below have effective depth 1, not 2.
    TTree* trees[nTrees][nInputSamples];
    TTree* treeFriends[nFriends][nInputSamples];
    TTree* treeFriendsIndividual[nFriendsIndividual][nInputSamples];
    TTree* treeHiForestInfo[nInputSamples];

    entries.clear();
    entries.resize(nInputSamples);
    entriesSelected.clear();
    entriesSelected.resize(nHistos);
    std::fill_n(entriesSelected.begin(), nHistos, 0);
    entriesBaseSelection.clear();
    entriesBaseSelection.resize(nHistos);
    std::fill_n(entriesBaseSelection.begin(), nHistos, 0);

    histFormulas.clear();
    histFormulas.resize(nHistos);
    std::fill_n(histFormulas.begin(), nHistos, "");
    histSelections.clear();
    histSelections.resize(nHistos);
    std::fill_n(histSelections.begin(), nHistos, "");
    histWeights.clear();
    histWeights.resize(nHistos);
    std::fill_n(histWeights.begin(), nHistos, "");

    int nFiles[nInputSamples];
    TFile* fileTmp = 0;

    std::cout << "initial reading to get the number of entries (if there is only one input file) and HiForest info" << std::endl;
    for (int iSample = 0; iSample < nInputSamples; ++iSample) {

        nFiles[iSample] = inputFiles[iSample].size();
        if (nInputSamples > 1) {
            std::cout <<"iSample = " << iSample << " , "<< std::endl;
        }

        // read the first file only to get the HiForest info
        std::string inputPath = inputFiles[iSample].at(0).c_str();
        fileTmp = TFile::Open(inputPath.c_str(), "READ");

        bool treeExists = true;
        if (nFiles[iSample] == 1) {
            // read one tree only to get the number of entries
            trees[0][iSample] = (TTree*)fileTmp->Get(treePaths.at(0).c_str());
            if (!trees[0][iSample]) {
                std::cout << "tree is not found in the path : "<< treePaths.at(0).c_str() <<". skipping file." << std::endl;
                treeExists = false;
            }
            if (treeExists) {
                entries[iSample] = trees[0][iSample]->GetEntries();
                std::cout << "entries = " << entries[iSample] << std::endl;
            }
        }

        if (treeExists) {
            treeHiForestInfo[0] = (TTree*)fileTmp->Get("HiForest/HiForestInfo");
            if (!treeHiForestInfo[0]) {
                std::cout << "HiForest/HiForestInfo tree is not found." << std::endl;
                treeExists = false;
            }
            if (treeExists) {
                HiForestInfoController hfic(treeHiForestInfo[0]);
                if (iSample == 0)  std::cout<<"### HiForestInfo Tree ###"<< std::endl;
                else                  std::cout<<"### HiForestInfo Tree, input "<< iSample+1 << " ###" << std::endl;
                hfic.printHiForestInfo();
                std::cout<<"###"<< std::endl;
            }
        }

        fileTmp->Close();
    }

    std::cout << "TTree::Draw()" <<std::endl;
    for (int iSample = 0; iSample < nInputSamples; ++iSample) {

        if (nInputSamples > 1) {
            std::cout <<"iSample = " << iSample << std::endl;
        }

        entries[iSample] = 0;
        for (int iFile = 0; iFile < nFiles[iSample]; ++iFile) {

            std::string inputPath = inputFiles[iSample].at(iFile).c_str();
            std::cout <<"iFile = " << iFile << " , " ;
            std::cout <<"reading input file : " << inputPath.c_str() << std::endl;
            fileTmp = TFile::Open(inputPath.c_str(), "READ");

            // check if the file is usable, if not skip the file.
            if (isGoodFile(fileTmp) != 0) {
                std::cout << "File is not good. skipping file." << std::endl;
                continue;
            }

            bool treeExists = true;
            for (int i=0; i<nTrees; ++i) {
                trees[i][iSample] = (TTree*)fileTmp->Get(treePaths.at(i).c_str());
                if (!trees[i][iSample]) {
                    std::cout << "tree is not found in the path : "<< treePaths.at(i).c_str() <<". skipping file." << std::endl;
                    treeExists = false;
                }
            }
            for (int i=0; i<nFriends; ++i) {
                treeFriends[i][iSample] = (TTree*)fileTmp->Get(treeFriendsPath.at(i).c_str());
                if (!treeFriends[i][iSample]) {
                    std::cout << "tree is not found in the path : "<< treeFriendsPath.at(i).c_str() <<". skipping file." << std::endl;
                    treeExists = false;
                }
            }
            if (!treeExists)  continue;

            // add friends
            for (int i=0; i<nTrees; ++i) {
                for (int j=0; j<nFriends; ++j) {
                    trees[i][iSample]->AddFriend(treeFriends[j][iSample], Form("t%d", j));
                }
            }
            for (int i=0; i < nFriendsIndividual; ++i) {
                if (treeFriendsPathIndividual.at(i).compare(CONFIGPARSER::nullInput) != 0) {
                    treeFriendsIndividual[i][iSample] = (TTree*)fileTmp->Get(treeFriendsPathIndividual.at(i).c_str());
                }
            }
            if (nFriendsIndividual > 0) {
                for (int i=0; i<nTrees; ++i) {
                    if (treeFriendsPathIndividual.at(i).compare(CONFIGPARSER::nullInput) != 0) {
                        trees[i][0]->AddFriend(treeFriendsIndividual[i][0], Form("tSelf%d", i));
                    }
                }
            }

            Long64_t entriesTmp = trees[0][iSample]->GetEntries();      // assume all the trees have same number of entries
            entries[iSample] += entriesTmp;

            if (nInputSamples == 1)  {
                std::cout << "entries in File = " << entriesTmp << std::endl;
            }
            else {
                std::cout << Form("entries in File of input %d = ", iSample+1) << entriesTmp << std::endl;
            }

            output->cd();
            for (int i=0; i<nHistos; ++i) {

                int treeIndex = 0;
                if (nHistosInput == nTrees)  treeIndex = i%nTrees;
                // std::cout << "treePath = " << treePaths.at(treeIndex).c_str() << ", ";

                if (nInputSamples > 1) {
                    iSample = i%nInputSamples;
                    std::cout << "iSample = " << iSample << ", ";
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
                Long64_t entriesSelectedTmp = trees[treeIndex][iSample]->GetEntries(selectionFinal.GetTitle());
                Long64_t entriesBaseSelectionTmp = trees[treeIndex][iSample]->GetEntries(selectionBase.c_str());
                // std::cout << "entriesSelected in file = " << entriesSelectedTmp << std::endl;
                entriesSelected[i] += entriesSelectedTmp;
                entriesBaseSelection[i] += entriesBaseSelectionTmp;

                if (histFormulas[i] == "") {
                    histFormulas[i] = formula;
                    histSelections[i] = selectionFinal.GetTitle();
                    histWeights[i] = weight;
                }

                TCut weight_AND_selection = Form("(%s)*(%s)", weight.c_str(), selectionFinal.GetTitle());
                trees[treeIndex][iSample]->Draw(Form("%s >>+ %s", formula.c_str(), h[i]->GetName()), weight_AND_selection.GetTitle(), "goff");
                /*
                 * For the case here TTree::Draw() is better than TTree::Project(), since
                 * 1. TTree::Project() is just another call to TTree::Draw() with "goff" option
                 * 2. TTree::Project() does not use "+" for filling histograms.
                 */
            }
            fileTmp->Close();
        }
    }
    std::cout << "TTree::Draw() ENDED" <<std::endl;
    for (int i = 0; i < nInputSamples; ++i) {

        if (nInputSamples == 1)  {
            std::cout << "entries = " << entries[0] << std::endl;
        }
        else {
            std::cout << Form("entries[%d] = ", i) << entries[i] << std::endl;
        }
    }
    std::cout << "### formulas, selections, and weights" << std::endl;
    for (int i = 0; i < nHistos; ++i) {

        std::cout << "TH1 i = " << i << ", ";
        std::cout << "formula = " << histFormulas[i].c_str() << ", ";
        std::cout << "selection = " << histSelections[i].c_str() << ", ";
        std::cout << "weight = " << histWeights[i].c_str() << std::endl;
    }
    std::cout << "###" << std::endl;

    std::cout << "### selected entries" << std::endl;
    for (int i = 0; i < nHistos; ++i) {

        std::cout << "TH1 i = " << i << ", ";
        int treeIndex = 0;
        if (nHistosInput == nTrees)  treeIndex = i%nTrees;
        std::cout << "treePath = " << treePaths.at(treeIndex).c_str() << ", ";
        std::cout << "entriesSelected = " << entriesSelected[i] << ", ";
        std::cout << "entriesBaseSelection = " << entriesBaseSelection[i] << std::endl;

        h_nums[i]->SetBinContent(1, entries[0]);
        h_nums[i]->SetBinContent(2, entriesSelected[i]);
        h_nums[i]->SetBinContent(3, entriesBaseSelection[i]);
    }
    std::cout << "###" << std::endl;

    output->cd();
    postLoop();

    std::cout<<"Closing the output file."<<std::endl;
    output->Close();
    std::cout<<"running drawSpectra() - END"<<std::endl;
}

/*
 * run the macro without going through event loop, things done before and after the loop
 */
void drawSpectraNoLoop(const TString configFile, const TString inputFile, const TString outputFile, const TString outputFigureName)
{
    std::cout<<"running drawSpectra()"<<std::endl;
    std::cout<<"configFile  = "<< configFile.Data() <<std::endl;
    std::cout<<"inputFile   = "<< inputFile.Data()  <<std::endl;
    std::cout<<"outputFile  = "<< outputFile.Data() <<std::endl;
    outputFigureStr = outputFigureName.Data();

    if (readConfiguration(configFile, inputFile) != 0)  return;
    printConfiguration();

    TFile* input = new TFile(inputFile.Data(), "READ");
    if (preLoop(input, false) != 0) return;

    TFile* output = TFile::Open(outputFile.Data(),"RECREATE");
    output->cd();

    postLoop();

    std::cout<<"Closing the input file."<<std::endl;
    input->Close();
    std::cout<<"Closing the output file."<<std::endl;
    output->Close();
    std::cout<<"running drawSpectra() - END"<<std::endl;
}

int main(int argc, char** argv)
{
    std::vector<std::string> argStr = ArgumentParser::ParseParameters(argc, argv);
    int nArgStr = argStr.size();

    std::vector<std::string> argOptions = ArgumentParser::ParseOptions(argc, argv);
    noLoop = (findPositionInVector(argOptions, ARGUMENTPARSER::noLoop) >= 0);

    if (nArgStr == 5) {
        if (noLoop) drawSpectraNoLoop(argStr.at(1), argStr.at(2), argStr.at(3), argStr.at(4));
        else        drawSpectra(argStr.at(1), argStr.at(2), argStr.at(3), argStr.at(4));
        return 0;
    }
    else if (nArgStr == 4) {
        if (noLoop) drawSpectraNoLoop(argStr.at(1), argStr.at(2), argStr.at(3));
        else        drawSpectra(argStr.at(1), argStr.at(2), argStr.at(3));
        return 0;
    }
    else if (nArgStr == 3) {
        if (noLoop) drawSpectraNoLoop(argStr.at(1), argStr.at(2));
        else        drawSpectra(argStr.at(1), argStr.at(2));
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./drawSpectra.exe <configFile> <inputFile> <outputFile> (<outputFigureName>)"
                << std::endl;
        return 1;
    }
}

int readConfiguration(const TString configFile, const TString inputFile)
{
    InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

    if (!configInput.isValid) {
        std::cout << "Input configuration is invalid." << std::endl;
        std::cout << "exiting" << std::endl;
        return -1;
    }
    if (!configCuts.isValid) {
        std::cout << "Cut configuration is invalid." << std::endl;
        std::cout << "exiting" << std::endl;
        return -1;
    }

    InputConfigurationParser::replaceKeyWords(configInput, inputFile.Data());

    // input for mode
    mode = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_mode];

    // input for TTree
    treePaths = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treePath]);
    treeFriendsPath = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treeFriendPath]);
    treeFriendsPathIndividual = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treeFriendPathIndividual]);
    formulas = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treeFormula]);
    selectionBase = configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treeSelectionBase];
    selections = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treeSelection]);
    selectionSplitter = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treeSelectionSplitter]);
    weights = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH1_weight]);

    // input for TH1
    titles = ConfigurationParser::ParseList(ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH1_title]));
    titlesX = ConfigurationParser::ParseList(ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH1_titleX]));
    titlesY = ConfigurationParser::ParseList(ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH1_titleY]));
    // nBins, xLow, xUp for TH1D histograms
    TH1D_Bins_List = ConfigurationParser::ParseListTH1D_Axis(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH1D_Bins_List]);
    // nBinsX, xLow, xUp, nBinsY, yLow, yUp for TH2D histograms
    TH2D_Bins_List = ConfigurationParser::ParseListTH2D_Axis(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH2D_Bins_List]);
    binsLogScaleX = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_binsLogScaleX];
    binsLogScaleY = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_binsLogScaleY];
    titleOffsetX = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_titleOffsetX];
    titleOffsetY = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_titleOffsetY];
    yMin = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_TH1_yMin];
    yMax = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_TH1_yMax];
    markerSize = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_markerSize];
    drawSame = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_drawSame];
    drawNormalized = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_drawNormalized];
    drawOptions = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_drawOption]);
    markerStyles = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_markerStyle]);
    lineStyles = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_lineStyle]);
    fillStyles = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_fillStyle]);
    colors = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_color]);
    fillColors = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_fillColor]);
    lineColors = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_lineColor]);
    lineWidth = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_lineWidth];

    // input for TLegend
    legendEntryLabels = ConfigurationParser::ParseList(ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_legendEntryLabel]));
    legendPosition = configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_legendPosition];
    legendOffsetX = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_legendOffsetX];
    legendOffsetY = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_legendOffsetY];
    legendBorderSize = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_legendBorderSize];
    legendWidth = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_legendWidth];
    legendHeight = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_legendHeight];
    legendTextSize = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_legendTextSize];

    // input for text objects
    tmpText = ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_text]);
    std::vector<std::pair<std::string, int>> textEntries = ConfigurationParser::ParseListOfList(tmpText);
    textLines = ConfigurationParser::getVecString(textEntries);
    textLinePadIndices = ConfigurationParser::getVecIndex(textEntries);
    textFonts = ConfigurationParser::ParseListOrInteger(configInput.proc[INPUT::kPERFORMANCE].str_i[INPUT::k_textFont]);
    textSizes = ConfigurationParser::ParseListOrFloat(configInput.proc[INPUT::kPERFORMANCE].str_f[INPUT::k_textSize]);
    textPositions = ConfigurationParser::ParseListOrString(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_textPosition]);
    textOffsetsX = ConfigurationParser::ParseListOrFloat(configInput.proc[INPUT::kPERFORMANCE].str_f[INPUT::k_textOffsetX]);
    textOffsetsY = ConfigurationParser::ParseListOrFloat(configInput.proc[INPUT::kPERFORMANCE].str_f[INPUT::k_textOffsetY]);

    tmpTextOverPad = ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_textAbovePad]);
    textsOverPad = ConfigurationParser::ParseList(tmpTextOverPad);
    textsOverPadAlignments = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_textAbovePadAlign]);
    textAbovePadFont = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_textAbovePadFont];
    textAbovePadSize = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_textAbovePadSize];
    textAbovePadOffsetX = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_textAbovePadOffsetX];
    textAbovePadOffsetY = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_textAbovePadOffsetY];

    // input for TLine
    // y-axis positions of the horizontal lines to be drawn
    TLines_horizontal = ConfigurationParser::ParseListFloat(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TLine_horizontal]);
    lineStyles_horizontal = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_LineStyle_horizontal]);
    // x-axis positions of the vertical lines to be drawn
    TLines_vertical = ConfigurationParser::ParseListFloat(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TLine_vertical]);
    lineStyles_vertical = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_LineStyle_vertical]);

    // input for TCanvas
    windowWidth = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_windowWidth];
    windowHeight = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_windowHeight];
    leftMargin = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_leftMargin];
    rightMargin = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_rightMargin];
    bottomMargin = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_bottomMargin];
    topMargin = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_topMargin];
    setLogx = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_setLogx];
    setLogy = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_setLogy];
    setLogz = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_setLogz];

    // set default values
    if (selections.size() == 0) selections.push_back("1");
    if (weights.size() == 0)    weights.push_back(INPUT_DEFAULT::TH1_weight.c_str());   // default weight = 1.
    if (titleOffsetX == 0) titleOffsetX = INPUT_DEFAULT::titleOffsetX;
    if (titleOffsetY == 0) titleOffsetY = INPUT_DEFAULT::titleOffsetY;
    if (yMin == 0 && yMax == 0)  yMax = -1;
    else if (yMin <= 0 && setLogy > 0)  yMin = resetTH1axisMin4LogScale(yMin, "y");
    if (drawNormalized >= INPUT_TH1::kN_TYPE_NORM) drawNormalized = INPUT_DEFAULT::drawNormalized;
    if (lineWidth == 0)  lineWidth = INPUT_DEFAULT::lineWidth;

    if (markerSize == 0)  markerSize = INPUT_DEFAULT::markerSize;

    if (textFonts.size() == 0)  textFonts = {INPUT_DEFAULT::textFont};
    if (textSizes.size() == 0)  textSizes = {INPUT_DEFAULT::textSize};

    if (windowWidth  == 0)  windowWidth = INPUT_DEFAULT::windowWidth;
    if (windowHeight == 0)  windowHeight = INPUT_DEFAULT::windowHeight;
    if (leftMargin == 0) leftMargin = INPUT_DEFAULT::leftMargin;
    if (rightMargin == 0) rightMargin = INPUT_DEFAULT::rightMargin;
    if (bottomMargin == 0) bottomMargin = INPUT_DEFAULT::bottomMargin;
    if (topMargin == 0) topMargin = INPUT_DEFAULT::topMargin;

    nTrees = treePaths.size();
    nFriends = treeFriendsPath.size();
    nFriendsIndividual = treeFriendsPathIndividual.size();
    nFormulas = formulas.size();
    nSelections = selections.size();
    nSelectionSplitter = selectionSplitter.size();
    nWeights = weights.size();
    nTitles = titles.size();
    nTitlesX = titlesX.size();
    nTitlesY = titlesY.size();
    nTH1D_Bins_List = TH1D_Bins_List.size();
    nTH2D_Bins_List = TH2D_Bins_List.size();
    nDrawOptions = drawOptions.size();
    nMarkerStyles = markerStyles.size();
    nLineStyles = lineStyles.size();
    nFillStyles = fillStyles.size();
    nColors = colors.size();
    nFillColors = fillColors.size();
    nLineColors = lineColors.size();
    nLegendEntryLabels = legendEntryLabels.size();
    nTextLines = textLines.size();
    nTextFonts = textFonts.size();
    nTextSizes = textSizes.size();
    nTextPositions = textPositions.size();
    nTextOffsetsX = textOffsetsX.size();
    nTextOffsetsY = textOffsetsY.size();
    nTextsOverPad = textsOverPad.size();
    nTextsOverPadAlignments = textsOverPadAlignments.size();
    nTLines_horizontal = TLines_horizontal.size();
    nLineStyles_horizontal = lineStyles_horizontal.size();
    nTLines_vertical = TLines_vertical.size();
    nLineStyles_vertical = lineStyles_vertical.size();

    return 0;
}

/*
 * print information read from input/cut configurations
 * assumes that readConfiguration() is run before
 */
void printConfiguration()
{
    // verbose about input configuration
    std::cout<<"Input Configuration :"<<std::endl;
    std::cout << "mode = " << mode << std::endl;
    if (nInputSamples > 1) {
        // Multiple input samples should be given using the following format
        // inputFile = <inputFile1>,<inputFile2>,...
        // there should be no single space between <inputFile1> and <inputFile2>.
        // the idea is to feed the input samples as a single argument and split them in the macro.
        std::cout << "There are multiple input samples. Entering comparison mode." << std::endl;
        std::cout << "comparison mode : Spectra from multiple input samples are going to be compared." << std::endl;
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
    std::cout << "nTH1D_Bins_List = " << nTH1D_Bins_List << std::endl;
    for (int i=0; i<nTH1D_Bins_List; ++i) {
        std::string strTH1D_Axis = ConfigurationParser::verboseTH1D_Axis(TH1D_Bins_List.at(i));
        std::cout << Form("TH1D_Bins_List[%d] = %s", i, strTH1D_Axis.c_str()) << std::endl;
    }
    std::cout << "nTH2D_Bins_List = " << nTH2D_Bins_List << std::endl;
    for (int i=0; i<nTH2D_Bins_List; ++i) {
        std::string strTH2D_Axis = ConfigurationParser::verboseTH2D_Axis(TH2D_Bins_List.at(i));
        std::cout << Form("TH2D_Bins_List[%d] = %s", i, strTH2D_Axis.c_str()) << std::endl;
    }
    std::cout << "binsLogScaleX = " << binsLogScaleX << std::endl;
    std::cout << "binsLogScaleY = " << binsLogScaleY << std::endl;
    std::cout << "titleOffsetX = " << titleOffsetX << std::endl;
    std::cout << "titleOffsetY = " << titleOffsetY << std::endl;
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
    for (int i = 0; i<nTextLines; ++i) {
        std::cout << Form("textLinePadIndices[%d] = %d", i, textLinePadIndices.at(i)) << std::endl;
    }
    if (nTextLines > 0) {
        std::cout << "nTextFonts = " << nTextFonts << std::endl;
        for (int i = 0; i<nTextFonts; ++i) {
            std::cout << Form("textFonts[%d] = %d", i, textFonts.at(i)) << std::endl;
        }
        std::cout << "nTextSizes = " << nTextSizes << std::endl;
        for (int i = 0; i<nTextSizes; ++i) {
            std::cout << Form("textSizes[%d] = %f", i, textSizes.at(i)) << std::endl;
        }
        std::cout << "nTextPositions = " << nTextPositions << std::endl;
        for (int i = 0; i<nTextPositions; ++i) {
            std::cout << Form("textPositions[%d] = %s", i, textPositions.at(i).c_str()) << std::endl;
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

    // verbose about cut configuration
    std::cout<<"Cut Configuration :"<<std::endl;
}

/*
 * initialize/read/modify objects before the loop.
 * Objects are eg. TH1, TGraph, ...
 */
int preLoop(TFile* input, bool makeNew)
{
    if (!makeNew) {
        // check if the file is available
        if (input == 0 ) return -1;
        else if (!input->IsOpen()) return -1;
        input->cd();
    }

    if (!(mode >= 0 && mode < MODES::kN_MODES)) {
        std::cout<<"mode = "<< mode << " is not a valid mode." << std::endl;
        std::cout<<"exiting"<< std::endl;
        return -1;
    }

    if (nTrees == 1 && nFriendsIndividual > 0) {
        std::cout<<"nTrees = "<< nTrees <<", nFriendsIndividual = " << nFriendsIndividual << std::endl;
        std::cout<<"There is only one tree to be plotted, it does not make sense to use individual friend trees."<< std::endl;
        std::cout<<"exiting"<< std::endl;
        return -1;
    }
    else if (nTrees > 1 && nFriendsIndividual > 0 && nTrees != nFriendsIndividual) {
        std::cout<<"nTrees = "<< nTrees <<", nFriendsIndividual = " << nFriendsIndividual << std::endl;
        std::cout<<"exiting"<< std::endl;
        return -1;
    }

    if (nSelectionSplitter == 1) {
        std::cout << "nSelectionSplitter = "<< nSelectionSplitter << std::endl;
        std::cout << "selectionSplitter has been set to have exactly one selection"<< std::endl;
        std::cout << "selectionSplitter is allowed to be either empty or to have more than one selections"<< std::endl;
        std::cout << "exiting"<< std::endl;
        return -1;
    }
    nSplits = 1;
    if (nSelectionSplitter > 1)  nSplits = nSelectionSplitter;

    nSelectionsTot = nSelections * nSplits;
    nFormulasTot = nFormulas * nSplits;

    nHistos = nFormulasTot;
    if (nFormulas == 1 && nSelections > nFormulas) nHistos = nSelectionsTot;
    else if (nFormulas == 1 && nSelections == 1 && nTrees > nFormulas) nHistos = nTrees * nSplits;
    else if (nFormulas > 1 && nSelections > 1 && nFormulas != nSelections) {
        std::cout << "mismatch of number of formulas and number of selections"<< std::endl;
        std::cout << "nHistos     = "<< nHistos << std::endl;
        std::cout << "nSelections = "<< nSelections << std::endl;
        std::cout << "exiting " << std::endl;
        return -1;
    }
    else if (nFormulas > 1 && nTrees > 1 && nFormulas != nTrees) {
        std::cout << "mismatch of number of formulas and number of trees"<< std::endl;
        std::cout << "nHistos = "<< nHistos << std::endl;
        std::cout << "nTrees  = "<< nTrees << std::endl;
        std::cout << "exiting " << std::endl;
        return -1;
    }
    else if (nSelections > 1 && nTrees > 1 && nSelections != nTrees) {
        std::cout << "mismatch of number of selections and number of trees"<< std::endl;
        std::cout << "nHistos     = "<< nHistos << std::endl;
        std::cout << "nSelections = "<< nSelections << std::endl;
        std::cout << "nTrees      = "<< nTrees << std::endl;
        std::cout << "exiting " << std::endl;
        return -1;
    }
    std::cout << "nHistos = " << nHistos << std::endl;
    nHistosInput = nHistos/nSplits;     // number of histograms without considering selectionSplitter

    TH1::SetDefaultSumw2();
    h.clear();
    h.resize(nHistos);
    h_nums.clear();
    h_nums.resize(nHistos);
    for (int i=0; i<nHistos; ++i) {

        int nBinsX = 0;
        int nBinsY = 0;
        std::vector<double> binsX;
        std::vector<double> binsY;
        float xLow = 0;
        float xUp  = 0;
        float yLow = 0;
        float yUp  = 0;

        int j = 0;
        if (mode == MODES::kTH1D) {

            if (nTH1D_Bins_List == nHistosInput) j = i%nTH1D_Bins_List;

            nBinsX = TH1D_Bins_List[j].nBins;
            binsX = TH1D_Bins_List[j].bins;
            xLow = TH1D_Bins_List[j].xLow;
            xUp = TH1D_Bins_List[j].xUp;
        }
        else if (mode == MODES::kTH2D) {

            if (nTH2D_Bins_List == nHistosInput) j = i%nTH2D_Bins_List;

            nBinsX = TH2D_Bins_List[j].axisX.nBins;
            binsX = TH2D_Bins_List[j].axisX.bins;
            xLow = TH2D_Bins_List[j].axisX.xLow;
            xUp = TH2D_Bins_List[j].axisX.xUp;
            nBinsY = TH2D_Bins_List[j].axisY.nBins;
            binsY = TH2D_Bins_List[j].axisY.bins;
            yLow = TH2D_Bins_List[j].axisY.xLow;
            yUp = TH2D_Bins_List[j].axisY.xUp;
        }

        if (mode == MODES::kTH1D) {
            std::string hName = Form("h_%d", i);
            if (makeNew) {
                double arrX[nBinsX+1];
                std::copy(binsX.begin(), binsX.end(), arrX);
                h[i] = new TH1D(hName.c_str(), "", nBinsX, arrX);
            }
            else
                h[i] = (TH1D*)input->Get(hName.c_str());
        }
        else if (mode == MODES::kTH2D) {
            std::string hName = Form("h2D_%d", i);
            if (makeNew) {
                double arrX[nBinsX+1];
                std::copy(binsX.begin(), binsX.end(), arrX);
                double arrY[nBinsY+1];
                std::copy(binsY.begin(), binsY.end(), arrY);
                h[i] = new TH2D(hName.c_str(), "", nBinsX, arrX, nBinsY, arrY);
            }
            else
                h[i] = (TH2D*)input->Get(hName.c_str());
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
        else if (nTitles == nSplits)  {
            if (titles.at(i/nHistosInput).compare(CONFIGPARSER::nullInput) != 0)  title = titles.at(i/nHistosInput).c_str();
        }

        std::string titleX = "";
        if (nTitlesX == 1) titleX = titlesX.at(0).c_str();
        else if (nTitlesX == nHistosInput) titleX = titlesX.at(i%nTitlesX).c_str();
        else if (nTitlesX == nHistos)      titleX = titlesX.at(i).c_str();

        std::string titleY = "";
        if (nTitlesY == 1) titleY = titlesY.at(0).c_str();
        else if (nTitlesY == nHistosInput) titleY = titlesY.at(i%nTitlesY).c_str();
        else if (nTitlesY == nHistos)      titleY = titlesY.at(i).c_str();

        h[i]->SetTitle(title.c_str());
        h[i]->SetXTitle(titleX.c_str());
        h[i]->SetYTitle(titleY.c_str());

        if (binsLogScaleX > 0) {
            std::vector<double> binsVecTmp = calcBinsLogScale(xLow, xUp, nBinsX);
            double binsArrTmp[nBinsX+1];
            std::copy(binsVecTmp.begin(), binsVecTmp.end(), binsArrTmp);

            h[i]->GetXaxis()->Set(nBinsX, binsArrTmp);
        }
        if (binsLogScaleY > 0 && mode == MODES::kTH2D) {
            std::vector<double> binsVecTmp = calcBinsLogScale(yLow, yUp, nBinsY);
            double binsArrTmp[nBinsY+1];
            std::copy(binsVecTmp.begin(), binsVecTmp.end(), binsArrTmp);

            h[i]->GetYaxis()->Set(nBinsY, binsArrTmp);
        }

        if (yMax > yMin)  h[i]->SetAxisRange(yMin, yMax, "Y");

        std::string h_numsName = Form("%s_nums", h[i]->GetName());
        if (makeNew)
            h_nums[i] = new TH1D(h_numsName.c_str(), h[i]->GetTitle(), 3, 0, 3);
        else
            h_nums[i] = (TH1D*)input->Get(h_numsName.c_str());
    }

    return 0;
}

int postLoop()
{
    // print info about histograms
    for (int i=0; i<nHistos; ++i) {
        std::cout << "#####" << std::endl;
        std::cout << Form("h[%d]", i) << std::endl;
        std::string summary = summaryTH1(h[i]);
        std::cout << summary.c_str() << std::endl;
    }

    h_scaleWidth.clear();
    h_scaleWidth.resize(nHistos);
    h_normInt.clear();
    h_normInt.resize(nHistos);
    h_normEvents.clear();
    h_normEvents.resize(nHistos);
    h_normEventsBaseSel.clear();
    h_normEventsBaseSel.resize(nHistos);
    for (int i=0; i<nHistos; ++i) {
        h[i]->Write("",TObject::kOverwrite);
        h_nums[i]->Write("",TObject::kOverwrite);

        if (mode == MODES::kTH1D) {
            h_scaleWidth[i] = (TH1D*)h[i]->Clone(Form("%s_scaleWidth", h[i]->GetName()));
        }
        else if (mode == MODES::kTH2D) {
            h_scaleWidth[i] = (TH2D*)h[i]->Clone(Form("%s_scaleWidth", h[i]->GetName()));
        }
        h_scaleWidth[i]->Scale(1, "width");
        h_scaleWidth[i]->Write("",TObject::kOverwrite);

        if (mode == MODES::kTH1D) {
            h_normInt[i] = (TH1D*)h[i]->Clone(Form("%s_normInt", h[i]->GetName()));
        }
        else if (mode == MODES::kTH2D) {
            h_normInt[i] = (TH2D*)h[i]->Clone(Form("%s_normInt", h[i]->GetName()));
        }
        h_normInt[i]->Scale(1./h[i]->Integral(), "width");
        h_normInt[i]->Write("",TObject::kOverwrite);

        if (mode == MODES::kTH1D) {
            h_normEvents[i] = (TH1D*)h[i]->Clone(Form("%s_normEvents", h[i]->GetName()));
        }
        else if (mode == MODES::kTH2D) {
            h_normEvents[i] = (TH2D*)h[i]->Clone(Form("%s_normEvents", h[i]->GetName()));
        }
        Long64_t entriesTmp = h_nums[i]->GetBinContent(2);
        h_normEvents[i]->Scale(1./entriesTmp, "width");
        h_normEvents[i]->Write("",TObject::kOverwrite);

        if (mode == MODES::kTH1D) {
            h_normEventsBaseSel[i] = (TH1D*)h[i]->Clone(Form("%s_normEventsBaseSel", h[i]->GetName()));
        }
        else if (mode == MODES::kTH2D) {
            h_normEventsBaseSel[i] = (TH2D*)h[i]->Clone(Form("%s_normEventsBaseSel", h[i]->GetName()));
        }
        Long64_t entriesBaseSelTmp = h_nums[i]->GetBinContent(3);
        h_normEventsBaseSel[i]->Scale(1./entriesBaseSelTmp, "width");
        h_normEventsBaseSel[i]->Write("",TObject::kOverwrite);

        if (mode == MODES::kTH2D) {
            std::vector<TH1D*> hProj(4, 0);
            for (int iProj = 0; iProj < 2; ++iProj) {
                if (iProj == 0) {
                    hProj[0] = (TH1D*)((TH2D*)h[i])->ProjectionX(Form("%s_projX", h[i]->GetName()));
                }
                else
                    hProj[0] = (TH1D*)((TH2D*)h[i])->ProjectionY(Form("%s_projY", h[i]->GetName()));

                hProj[0]->Write("",TObject::kOverwrite);

                hProj[1] = (TH1D*)hProj[0]->Clone(Form("%s_scaleWidth", hProj[0]->GetName()));
                hProj[1]->Scale(1, "width");
                hProj[1]->Write("",TObject::kOverwrite);

                hProj[1] = (TH1D*)hProj[0]->Clone(Form("%s_normInt", hProj[0]->GetName()));
                hProj[1]->Scale(1./hProj[0]->Integral(), "width");
                hProj[1]->Write("",TObject::kOverwrite);

                hProj[1] = (TH1D*)hProj[0]->Clone(Form("%s_normEvents", hProj[0]->GetName()));
                hProj[1]->Scale(1./entriesTmp, "width");
                hProj[1]->Write("",TObject::kOverwrite);

                hProj[2] = (TH1D*)hProj[0]->Clone(Form("%s_mean", hProj[0]->GetName()));
                hProj[3] = (TH1D*)hProj[0]->Clone(Form("%s_stddev", hProj[0]->GetName()));

                std::string projYTitle = (iProj == 0) ? ((TH2D*)h[i])->GetYaxis()->GetTitle() : ((TH2D*)h[i])->GetXaxis()->GetTitle();
                hProj[2]->SetYTitle(Form("< %s >", projYTitle.c_str()));
                hProj[3]->SetYTitle(Form("#sigma( %s )", projYTitle.c_str()));
                for (int iProjBin = 1; iProjBin <= hProj[0]->GetNbinsX(); ++iProjBin) {
                    if (iProj == 0) {
                        hProj[2]->SetBinContent(iProjBin, ((TH2D*)h[i])->ProjectionY("", iProjBin, iProjBin)->GetMean());
                        hProj[2]->SetBinError(iProjBin, ((TH2D*)h[i])->ProjectionY("", iProjBin, iProjBin)->GetMeanError());
                        hProj[3]->SetBinContent(iProjBin, ((TH2D*)h[i])->ProjectionY("", iProjBin, iProjBin)->GetStdDev());
                        hProj[3]->SetBinError(iProjBin, ((TH2D*)h[i])->ProjectionY("", iProjBin, iProjBin)->GetStdDevError());
                    }
                    else {
                        hProj[2]->SetBinContent(iProjBin, ((TH2D*)h[i])->ProjectionX("", iProjBin, iProjBin)->GetMean());
                        hProj[2]->SetBinError(iProjBin, ((TH2D*)h[i])->ProjectionX("", iProjBin, iProjBin)->GetMeanError());
                        hProj[3]->SetBinContent(iProjBin, ((TH2D*)h[i])->ProjectionX("", iProjBin, iProjBin)->GetStdDev());
                        hProj[3]->SetBinError(iProjBin, ((TH2D*)h[i])->ProjectionX("", iProjBin, iProjBin)->GetStdDevError());
                    }
                }

                hProj[2]->Write("",TObject::kOverwrite);
                hProj[3]->Write("",TObject::kOverwrite);
            }
        }
    }
    // histograms are written. After this point changes to the histograms will not be reflected in the output ROOT file.

    // set the style of the histograms for canvases to be written
    for (int i=0; i<nHistos; ++i) {
        h[i]->SetTitleOffset(titleOffsetX,"X");
        h[i]->SetTitleOffset(titleOffsetY,"Y");
        h_scaleWidth[i]->SetTitleOffset(titleOffsetX,"X");
        h_scaleWidth[i]->SetTitleOffset(titleOffsetY,"Y");
        h_normInt[i]->SetTitleOffset(titleOffsetX,"X");
        h_normInt[i]->SetTitleOffset(titleOffsetY,"Y");
        h_normEvents[i]->SetTitleOffset(titleOffsetX,"X");
        h_normEvents[i]->SetTitleOffset(titleOffsetY,"Y");
        h_normEventsBaseSel[i]->SetTitleOffset(titleOffsetX,"X");
        h_normEventsBaseSel[i]->SetTitleOffset(titleOffsetY,"Y");

        // default marker style and color
        h[i]->SetMarkerStyle(kFullCircle);
        h[i]->SetMarkerColor(kBlack);
        h_scaleWidth[i]->SetMarkerStyle(kFullCircle);
        h_scaleWidth[i]->SetMarkerColor(kBlack);
        h_normInt[i]->SetMarkerStyle(kFullCircle);
        h_normInt[i]->SetMarkerColor(kBlack);
        h_normEvents[i]->SetMarkerStyle(kFullCircle);
        h_normEvents[i]->SetMarkerColor(kBlack);
        h_normEventsBaseSel[i]->SetMarkerStyle(kFullCircle);
        h_normEventsBaseSel[i]->SetMarkerColor(kBlack);

        // no stats box in the final plots
        h[i]->SetStats(false);
        h_scaleWidth[i]->SetStats(false);
        h_normInt[i]->SetStats(false);
        h_normEvents[i]->SetStats(false);
        h_normEventsBaseSel[i]->SetStats(false);
    }

    // write canvases
    TCanvas* c = 0;
    for (int i=0; i<nHistos; ++i) {
        std::string cnvName = Form("cnv_%d",i);
        if (mode == MODES::kTH2D)  cnvName = Form("cnv2D_%d",i);
        c = new TCanvas(cnvName.c_str(),"",windowWidth,windowHeight);
        c->SetTitle(h[i]->GetTitle());
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        setCanvasFinal(c, setLogx, setLogy, setLogz);
        c->cd();

        // set the style of the histograms for canvases to be written
        h[i]->SetTitleOffset(titleOffsetX,"X");
        h[i]->SetTitleOffset(titleOffsetY,"Y");
        h[i]->SetStats(false);  // no stats box in the final plots
        if (mode == MODES::kTH1D) {
            h[i]->SetMarkerStyle(kFullCircle);
            h[i]->SetMarkerColor(kBlack);
            h[i]->Draw("e");
        }
        if (mode == MODES::kTH2D) {
            h[i]->Draw("colz");
        }
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.

        // scaled by bin width.
        c = new TCanvas(Form("%s_scaleWidth",cnvName.c_str()),"",windowWidth,windowHeight);
        c->SetTitle(h_scaleWidth[i]->GetTitle());
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        setCanvasFinal(c, setLogx, setLogy, setLogz);
        c->cd();

        // set the style of the histograms for canvases to be written
        h_scaleWidth[i]->SetTitleOffset(titleOffsetX,"X");
        h_scaleWidth[i]->SetTitleOffset(titleOffsetY,"Y");
        h_scaleWidth[i]->SetStats(false);  // no stats box in the final plots
        if (mode == MODES::kTH1D) {
            h_scaleWidth[i]->SetMarkerStyle(kFullCircle);
            h_scaleWidth[i]->SetMarkerColor(kBlack);
            h_scaleWidth[i]->Draw("e");
        }
        if (mode == MODES::kTH2D) {
            h_scaleWidth[i]->Draw("colz");
        }
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.

        // normalized such that integral("width") = 1.
        c = new TCanvas(Form("%s_normInt",cnvName.c_str()),"",windowWidth,windowHeight);
        c->SetTitle(h_normInt[i]->GetTitle());
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        setCanvasFinal(c, setLogx, setLogy, setLogz);
        c->cd();

        // set the style of the histograms for canvases to be written
        h_normInt[i]->SetTitleOffset(titleOffsetX,"X");
        h_normInt[i]->SetTitleOffset(titleOffsetY,"Y");
        h_normInt[i]->SetStats(false);  // no stats box in the final plots
        if (mode == MODES::kTH1D) {
            h_normInt[i]->SetMarkerStyle(kFullCircle);
            h_normInt[i]->SetMarkerColor(kBlack);
            h_normInt[i]->Draw("e");
        }
        if (mode == MODES::kTH2D) {
            h_normInt[i]->Draw("colz");
        }
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.

        // normalized such that integral("width") = total entries / number of events
        c = new TCanvas(Form("%s_normEvents",cnvName.c_str()),"",windowWidth,windowHeight);
        c->SetTitle(h_normEvents[i]->GetTitle());
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        setCanvasFinal(c, setLogx, setLogy, setLogz);
        c->cd();

        // set the style of the histograms for canvases to be written
        h_normEvents[i]->SetTitleOffset(titleOffsetX,"X");
        h_normEvents[i]->SetTitleOffset(titleOffsetY,"Y");
        h_normEvents[i]->SetStats(false);  // no stats box in the final plots
        if (mode == MODES::kTH1D) {
            h_normEvents[i]->SetMarkerStyle(kFullCircle);
            h_normEvents[i]->SetMarkerColor(kBlack);
            h_normEvents[i]->Draw("e");
        }
        if (mode == MODES::kTH2D) {
            h_normEvents[i]->Draw("colz");
        }
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.

        // normalized such that integral("width") = total entries / number of events that pass base selection
        c = new TCanvas(Form("%s_normEventsBaseSel",cnvName.c_str()),"",windowWidth,windowHeight);
        c->SetTitle(h_normEventsBaseSel[i]->GetTitle());
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        setCanvasFinal(c, setLogx, setLogy, setLogz);
        c->cd();

        // set the style of the histograms for canvases to be written
        h_normEventsBaseSel[i]->SetTitleOffset(titleOffsetX,"X");
        h_normEventsBaseSel[i]->SetTitleOffset(titleOffsetY,"Y");
        h_normEventsBaseSel[i]->SetStats(false);  // no stats box in the final plots
        if (mode == MODES::kTH1D) {
            h_normEventsBaseSel[i]->SetMarkerStyle(kFullCircle);
            h_normEventsBaseSel[i]->SetMarkerColor(kBlack);
            h_normEventsBaseSel[i]->Draw("e");
        }
        if (mode == MODES::kTH2D) {
            h_normEventsBaseSel[i]->Draw("colz");
        }
        c->Write("",TObject::kOverwrite);
        c->Close();         // do not use Delete() for TCanvas.
    }
    // write canvases - END

    // set style of the histograms for the canvases to be saved as picture
    for(int i=0; i<nHistos; ++i) {
        bool drawSameAcrossSplits = (drawSame == INPUT_TH1::k_drawSameAcrossSplits);

        std::string drawOption = "";
        if (nDrawOptions == 1) {
            if (drawOptions.at(0).compare(CONFIGPARSER::nullInput) != 0)  drawOption = drawOptions.at(0).c_str();
        }
        else if (!drawSameAcrossSplits && nDrawOptions == nHistosInput) {
            if (drawOptions.at(i%nDrawOptions).compare(CONFIGPARSER::nullInput) != 0)  drawOption = drawOptions.at(i%nDrawOptions).c_str();
        }
        else if (drawSameAcrossSplits && nDrawOptions == nSplits) {
            if (drawOptions.at((i/nHistosInput)%nDrawOptions).compare(CONFIGPARSER::nullInput) != 0)  drawOption = drawOptions.at((i/nHistosInput)%nDrawOptions).c_str();
        }
        // https://root.cern.ch/doc/master/classTObject.html#abe2a97d15738d5de00cd228e0dc21e56
        // TObject::SetDrawOption() is not suitable for the approach here.

        int markerStyle = GRAPHICS::markerStyle;
        if (nMarkerStyles == 1) markerStyle = GraphicsConfigurationParser::ParseMarkerStyle(markerStyles.at(0));
        else if (!drawSameAcrossSplits && nMarkerStyles == nHistosInput) markerStyle = GraphicsConfigurationParser::ParseMarkerStyle(markerStyles.at(i%nMarkerStyles));
        else if (drawSameAcrossSplits && nMarkerStyles == nSplits) markerStyle = GraphicsConfigurationParser::ParseMarkerStyle(markerStyles.at((i/nHistosInput)%nMarkerStyles));
        h[i]->SetMarkerStyle(markerStyle);
        h_scaleWidth[i]->SetMarkerStyle(markerStyle);
        h_normInt[i]->SetMarkerStyle(markerStyle);
        h_normEvents[i]->SetMarkerStyle(markerStyle);
        h_normEventsBaseSel[i]->SetMarkerStyle(markerStyle);

        int lineStyle = GRAPHICS::lineStyle;
        if (nLineStyles == 1)  lineStyle = GraphicsConfigurationParser::ParseLineStyle(lineStyles.at(0));
        else if (!drawSameAcrossSplits && nLineStyles == nHistosInput)  lineStyle = GraphicsConfigurationParser::ParseLineStyle(lineStyles.at(i%nLineStyles));
        else if (drawSameAcrossSplits && nLineStyles == nSplits)  lineStyle = GraphicsConfigurationParser::ParseLineStyle(lineStyles.at((i/nHistosInput)%nLineStyles));
        h[i]->SetLineStyle(lineStyle);
        h_scaleWidth[i]->SetLineStyle(lineStyle);
        h_normInt[i]->SetLineStyle(lineStyle);
        h_normEvents[i]->SetLineStyle(lineStyle);
        h_normEventsBaseSel[i]->SetLineStyle(lineStyle);

        int fillStyle = GRAPHICS::fillStyle;
        if (nFillStyles == 1)  fillStyle = GraphicsConfigurationParser::ParseLineStyle(fillStyles.at(0));
        else if (!drawSameAcrossSplits && nFillStyles == nHistosInput)  fillStyle = GraphicsConfigurationParser::ParseLineStyle(fillStyles.at(i%nFillStyles));
        else if (drawSameAcrossSplits && nFillStyles == nSplits)  fillStyle = GraphicsConfigurationParser::ParseLineStyle(fillStyles.at((i/nHistosInput)%nFillStyles));
        h[i]->SetFillStyle(fillStyle);
        h_scaleWidth[i]->SetFillStyle(fillStyle);
        h_normInt[i]->SetFillStyle(fillStyle);
        h_normEvents[i]->SetFillStyle(fillStyle);
        h_normEventsBaseSel[i]->SetFillStyle(fillStyle);

        int color = GRAPHICS::colors[i];
        if (nColors == 1) color = GraphicsConfigurationParser::ParseColor(colors.at(0));
        else if (!drawSameAcrossSplits && nColors == nHistosInput) color = GraphicsConfigurationParser::ParseColor(colors.at(i%nColors));
        else if (drawSameAcrossSplits && nColors == nSplits) color = GraphicsConfigurationParser::ParseColor(colors.at((i/nHistosInput)%nColors));
        h[i]->SetMarkerColor(color);
        h[i]->SetLineColor(color);
        h_scaleWidth[i]->SetMarkerColor(color);
        h_scaleWidth[i]->SetLineColor(color);
        h_normInt[i]->SetMarkerColor(color);
        h_normInt[i]->SetLineColor(color);
        h_normEvents[i]->SetMarkerColor(color);
        h_normEvents[i]->SetLineColor(color);
        h_normEventsBaseSel[i]->SetMarkerColor(color);
        h_normEventsBaseSel[i]->SetLineColor(color);

        int fillColor = -1;
        if (nFillColors == 1) fillColor = GraphicsConfigurationParser::ParseColor(fillColors.at(0));
        else if (!drawSameAcrossSplits && nFillColors == nHistosInput) fillColor = GraphicsConfigurationParser::ParseColor(fillColors.at(i%nFillColors));
        else if (drawSameAcrossSplits && nFillColors == nSplits) fillColor = GraphicsConfigurationParser::ParseColor(fillColors.at((i/nHistosInput)%nFillColors));
        if (fillColor != -1)
        {
            h[i]->SetFillColor(fillColor);
            h_scaleWidth[i]->SetFillColor(fillColor);
            h_normInt[i]->SetFillColor(fillColor);
            h_normEvents[i]->SetFillColor(fillColor);
            h_normEventsBaseSel[i]->SetFillColor(fillColor);
        }

        int lineColor = -1;
        if (nLineColors == 1) lineColor = GraphicsConfigurationParser::ParseColor(lineColors.at(0));
        else if (!drawSameAcrossSplits && nLineColors == nHistosInput) lineColor = GraphicsConfigurationParser::ParseColor(lineColors.at(i%nLineColors));
        else if (drawSameAcrossSplits && nLineColors == nSplits) lineColor = GraphicsConfigurationParser::ParseColor(lineColors.at((i/nHistosInput)%nLineColors));
        if (nLineColors != -1)
        {
            h[i]->SetLineColor(lineColor);
            h_scaleWidth[i]->SetLineColor(lineColor);
            h_normInt[i]->SetLineColor(lineColor);
            h_normEvents[i]->SetLineColor(lineColor);
            h_normEventsBaseSel[i]->SetLineColor(lineColor);
        }

        if(lineWidth != INPUT_DEFAULT::lineWidth) {
            if (drawOption.find("hist") != std::string::npos) {
                h[i]->SetLineWidth(lineWidth);
                h_scaleWidth[i]->SetLineWidth(lineWidth);
                h_normInt[i]->SetLineWidth(lineWidth);
                h_normEvents[i]->SetLineWidth(lineWidth);
                h_normEventsBaseSel[i]->SetLineWidth(lineWidth);
            }
        }

        h[i]->SetMarkerSize(markerSize);
        h_scaleWidth[i]->SetMarkerSize(markerSize);
        h_normInt[i]->SetMarkerSize(markerSize);
        h_normEvents[i]->SetMarkerSize(markerSize);
        h_normEventsBaseSel[i]->SetMarkerSize(markerSize);
    }

    h_draw.clear();
    h_draw.resize(nHistos);
    for (int i=0; i<nHistos; ++i) {
        if (drawNormalized == INPUT_TH1::k_normInt) {
            h_draw[i] = (TH1D*)h_normInt[i]->Clone(Form("h_%d_draw", i));
        }
        else if (drawNormalized == INPUT_TH1::k_normEvents) {
            h_draw[i] = (TH1D*)h_normEvents[i]->Clone(Form("h_%d_draw", i));
        }
        else if (drawNormalized == INPUT_TH1::k_normEventsBaseSel) {
            h_draw[i] = (TH1D*)h_normEventsBaseSel[i]->Clone(Form("h_%d_draw", i));
        }
        else {
            h_draw[i] = (TH1D*)h_scaleWidth[i]->Clone(Form("h_%d_draw", i));
        }
    }

    nPads = 0;
    if (drawSame == 0) {    // histograms will be plotted separately.
        if (nTextLines > 0)  nPads = textLinePadIndices[nTextLines-1]+1;
        for (int i=0; i<nHistos; ++i) {
            std::string cnvName = Form("cnv_pad%d",i);
            if (mode == MODES::kTH2D)  cnvName = Form("cnv2D_pad%d",i);
            c = new TCanvas(cnvName.c_str(),"",windowWidth,windowHeight);
            setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
            setCanvasFinal(c, setLogx, setLogy, setLogz);
            c->cd();

            std::string drawOption = "";
            if (nDrawOptions == 1)  drawOption = drawOptions.at(0).c_str();
            else if (nDrawOptions == nHistosInput) drawOption = drawOptions.at(i%nDrawOptions).c_str();

            if (mode == MODES::kTH1D) {
                h_draw[i]->SetMarkerColor(kBlack);
                h_draw[i]->SetLineColor(kBlack);
            }
            h_draw[i]->Draw(drawOption.c_str());

            int iPad = 0;
            if (nPads == nHistos) iPad = i;
            else if (nPads == nHistosInput) iPad = i%nPads;
            else if (nPads == nHistos) iPad = i;
            else if (nPads == nSplits) iPad = i/nHistosInput;
            // add Text
            setAndDrawLatex(c, iPad);

            // add Text above the pad
            setAndDrawLatexOverPad(c);

            // add TLine
            setAndDrawLinesHorizontal(c);
            setAndDrawLinesVertical(c);
            c->Write("",TObject::kOverwrite);

            // save histograms as picture if a figure name is provided.
            if (outputFigureStr.size() > 0)  saveAsFigure(c, i);

            c->Close();         // do not use Delete() for TCanvas.
        }
    }
    else if (drawSame > 0) {    // histograms will be plotted to the same canvas

        bool drawSameAcrossSplits = (drawSame == INPUT_TH1::k_drawSameAcrossSplits);
        bool drawSameInsideSplits = (drawSame == INPUT_TH1::k_drawSameInsideSplits);

        nPads = 1;     // default, corresponds to drawSame == INPUT_TH1::k_drawSame
        if (drawSameAcrossSplits)  nPads = nHistosInput;
        if (drawSameInsideSplits)  nPads = nSplits;

        // one must have : nHistosPerCanvas * nCanvasDrawSame = nHistos
        int nHistosPerPad = nHistos;     // default, corresponds to drawSame == INPUT_TH1::k_drawSame
        if (drawSameAcrossSplits)  nHistosPerPad = nSplits;
        if (drawSameInsideSplits)  nHistosPerPad = nHistosInput;

        int iPad = 0;
        bool drawSameFinished = false;
        while (!drawSameFinished)  {

            std::string cnvName = Form("cnv_pad%d", iPad);
            if (mode == MODES::kTH2D)  cnvName = Form("cnv2D_pad%d", iPad);
            c = new TCanvas(cnvName.c_str(),"",windowWidth,windowHeight);
            setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
            setCanvasFinal(c, setLogx, setLogy, setLogz);
            c->cd();

            int histStart = 0;
            if (drawSameAcrossSplits) histStart += iPad;
            if (drawSameInsideSplits) histStart =  iPad * nHistosPerPad;
            // set maximum/minimum of y-axis
            if (yMin > yMax && (mode == MODES::kTH1D)) {
                std::vector<TH1D*> vecTH1Dtmp;
                vecTH1Dtmp.resize(nHistosPerPad);
                vecTH1Dtmp[0] = (TH1D*)h_draw[histStart];

                int histCount = 1;
                int iHist = histStart;
                int increment = 1;
                if (drawSameAcrossSplits)  increment = nHistosInput;
                if (drawSameInsideSplits)  increment = 1;
                while (histCount < nHistosPerPad)
                {
                    iHist += increment;
                    vecTH1Dtmp[histCount] = (TH1D*)h_draw[iHist];
                    histCount++;
                }
                double histMin = getMinimumTH1DContent(vecTH1Dtmp);
                double histMax = getMaximumTH1DContent(vecTH1Dtmp);

                if (setLogy == 0) h_draw[histStart]->SetMinimum(histMin-TMath::Abs(histMin)*0.1);
                h_draw[histStart]->SetMaximum(histMax+TMath::Abs(histMax)*0.25*TMath::Power(10,setLogy));
            }

            TLegend* leg = new TLegend();

            int histCount = 0;
            int iHist = histStart;
            int increment = 1;
            if (drawSameAcrossSplits)  increment = nHistosInput;
            if (drawSameInsideSplits)  increment = 1;
            while (histCount < nHistosPerPad)
            {
                std::string drawOption = "";
                if (nDrawOptions == 1)  drawOption = drawOptions.at(0).c_str();
                else if (nDrawOptions == nHistosPerPad) drawOption = drawOptions.at((iHist/increment)%nDrawOptions).c_str();

                h_draw[iHist]->Draw(Form("%s same", drawOption.c_str()));

                if (nLegendEntryLabels == nHistosPerPad) {
                    std::string label = legendEntryLabels.at((iHist/increment)%nLegendEntryLabels).c_str();
                    if (label.compare(CONFIGPARSER::nullInput) == 0)  continue;

                    std::string legendOption = "lpf";
                    if (drawOption.find("hist") != std::string::npos)  legendOption = "lf";
                    leg->AddEntry(h_draw[iHist], label.c_str(), legendOption.c_str());
                }

                histCount++;
                iHist += increment;
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
            setAndDrawLatex(c, iPad);

            // add Text above the pad
            setAndDrawLatexOverPad(c);

            // add TLine
            setAndDrawLinesHorizontal(c);
            setAndDrawLinesVertical(c);
            c->Write("",TObject::kOverwrite);

            // save histograms as picture if a figure name is provided.
            if (outputFigureStr.size() > 0)  saveAsFigure(c, iPad);

            c->Close();
            if (leg != 0)  leg->Delete();
            iPad++;
            drawSameFinished = (iPad == nPads);
        }
    }

    return 0;
}

void setAndDrawLatex(TPad* pad, int iPad)
{
    bool textExists = (std::find(textLinePadIndices.begin(), textLinePadIndices.end(), iPad) != textLinePadIndices.end());
    if (!textExists)  return;

    TLatex latex;

    std::string textPosition = "";
    if (nTextPositions == 1)  textPosition = textPositions.at(0);
    else if (nTextPositions == nPads)  textPosition = textPositions.at(iPad);
    setTextAlignment(&latex, textPosition);

    float textFont = textFonts.at(0);
    if (nTextFonts == nPads) textFont = textFonts.at(iPad);
    latex.SetTextFont(textFont);

    float textSize = textSizes.at(0);
    if (nTextSizes == nPads) textSize = textSizes.at(iPad);
    latex.SetTextSize(textSize);

    float textOffsetX = textOffsetsX.at(0);
    if (nTextOffsetsX == nPads) textOffsetX = textOffsetsX.at(iPad);

    float textOffsetY = textOffsetsY.at(0);
    if (nTextOffsetsY == nPads) textOffsetY = textOffsetsY.at(iPad);

    std::vector<std::string> textLinesTmp;
    for (int iLine = 0; iLine < nTextLines; ++iLine) {
        if (textLinePadIndices.at(iLine) == iPad)
            textLinesTmp.push_back(textLines.at(iLine).c_str());
    }

    drawTextLines(&latex, pad, textLinesTmp, textPosition, textOffsetX, textOffsetY);
}

void setAndDrawLatexOverPad(TPad* pad)
{
    for (int i = 0; i < nTextsOverPad; ++i) {

        TLatex latex;

        latex.SetTextFont(textAbovePadFont);
        latex.SetTextSize(textAbovePadSize);

        int textOverPadAlignment = GRAPHICS::textAlign;
        if (nTextsOverPadAlignments == 1) textOverPadAlignment = GraphicsConfigurationParser::ParseTextAlign(textsOverPadAlignments.at(0));
        else if (nTextsOverPadAlignments == nTextsOverPad) textOverPadAlignment = GraphicsConfigurationParser::ParseTextAlign(textsOverPadAlignments.at(i));
        latex.SetTextAlign(textOverPadAlignment);

        setTextAbovePad(&latex, pad, textAbovePadOffsetX, textAbovePadOffsetY);

        latex.DrawLatexNDC(latex.GetX(), latex.GetY(), textsOverPad.at(i).c_str());
    }
}

void setAndDrawLinesHorizontal(TPad* pad)
{
    pad->Update();
    for (int i = 0; i<nTLines_horizontal; ++i) {

        double x1 = pad->GetUxmin();
        double x2 = pad->GetUxmax();
        double y1 = pad->GetUymin();
        double y2 = pad->GetUymax();
        if (pad->GetLogy() == 1) {
            y1 = TMath::Power(10, pad->GetUymin());
            y2 = TMath::Power(10, pad->GetUymax());
        }

        if (y1 < TLines_horizontal.at(i) && TLines_horizontal.at(i) < y2) {

            int lineStyle_horizontal = GRAPHICS::lineStyle_horizontal;
            if (nLineStyles_horizontal == 1)
                lineStyle_horizontal = GraphicsConfigurationParser::ParseLineStyle(lineStyles_horizontal.at(0));
            else if (nLineStyles_horizontal == nTLines_horizontal)
                lineStyle_horizontal = GraphicsConfigurationParser::ParseLineStyle(lineStyles_horizontal.at(i));

            TLine line;
            line.SetLineStyle(lineStyle_horizontal);   // https://root.cern.ch/doc/master/TAttLine_8h.html#a7092c0c4616367016b70d54e5c680a69
            line.DrawLine(x1, TLines_horizontal.at(i), x2, TLines_horizontal.at(i));
        }
    }
}

void setAndDrawLinesVertical(TPad* pad)
{
    pad->Update();
    for (int i = 0; i<nTLines_vertical; ++i) {

        double x1 = pad->GetUxmin();
        double x2 = pad->GetUxmax();
        double y1 = pad->GetUymin();
        double y2 = pad->GetUymax();
        if (pad->GetLogy() == 1) {
            y1 = TMath::Power(10, pad->GetUymin());
            y2 = TMath::Power(10, pad->GetUymax());
        }

        if (x1 < TLines_vertical.at(i) && TLines_vertical.at(i) < x2) {

            int lineStyle_vertical = GRAPHICS::lineStyle_vertical;
            if (nLineStyles_vertical == 1)
                lineStyle_vertical = GraphicsConfigurationParser::ParseLineStyle(lineStyles_vertical.at(0));
            else if (nLineStyles_vertical == nTLines_vertical)
                lineStyle_vertical = GraphicsConfigurationParser::ParseLineStyle(lineStyles_vertical.at(i));

            TLine line;
            line.SetLineStyle(lineStyle_vertical);   // https://root.cern.ch/doc/master/TAttLine_8h.html#a7092c0c4616367016b70d54e5c680a69
            line.DrawLine(TLines_vertical.at(i), y1, TLines_vertical.at(i), y2);
        }
    }
}

void saveAsFigure(TCanvas* c, int iCanvas)
{
    std::string outputFigureName = outputFigureStr.c_str();
    if (outputFigureName.find(".") != std::string::npos) {     // file extension is specified
        if (nPads > 1) {
            // modify outputFile name
            // if i=1, then "output.ext" becomes "output_2.ext"
            size_t pos = outputFigureName.find_last_of(".");
            outputFigureName.replace(pos,1, Form("_%d.", iCanvas+1));
        }
        c->SaveAs(outputFigureName.c_str());
    }
    else {  // file extension is NOT specified
        if (nPads > 1) {
            // modify outputFile name
            // if i=1, then "output" becomes "output_2"
            outputFigureName = Form("%s_%d", outputFigureName.c_str(), iCanvas+1);
        }

        c->SaveAs(Form("%s.C", outputFigureName.c_str()));
        c->SaveAs(Form("%s.png", outputFigureName.c_str()));
        c->SaveAs(Form("%s.pdf", outputFigureName.c_str()));
    }
}
