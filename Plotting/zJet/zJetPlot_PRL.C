#include <TFile.h>
#include <TDirectoryFile.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TLegend.h>
#include <TLine.h>
#include <TBox.h>
#include <TH1D.h>
#include <TF1.h>
#include <TAxis.h>
#include <TGaxis.h>
#include <TLatex.h>
#include <TMath.h>
#include <TStyle.h>
#include <TArrow.h>

#include <vector>
#include <string>
#include <iostream>

#include "../../TreeHeaders/CutConfigurationTree.h"
#include "../../Utilities/interface/CutConfigurationParser.h"
#include "../../Utilities/interface/InputConfigurationParser.h"
#include "../../Utilities/eventUtil.h"
#include "../../Utilities/styleUtil.h"
#include "../../Utilities/systemUtil.h"
#include "../../Utilities/th1Util.h"
#include "../../Utilities/tgraphUtil.h"
#include "../../Data/zBosons/zJetData.h"

void zJetPlot_PRL(const TString configFile, const TString inputFile, const TString outputFile = "zJetPlot_PRL.root", const TString outputFigurePrefix = "");
std::string parseMCreference(int iColl, std::string fileName);
void setTH1(TH1 *h, COLL::TYPE collisionType);
void setTH1_correlation(std::string correlation, TH1* h);
void setTH1_xjz(TH1* h);
void setTH1_dphi(TH1* h);
void setTH1_ptJet(TH1* h);
void setTH1_diLeptonM(TH1* h);
void setTH1_xjz_mean(TH1* h);
void setTH1_rjz(TH1* h);
bool is_xjz(std::string correlation);
bool is_dphi(std::string correlation);

void zJetPlot_PRL(const TString configFile, const TString inputFile, const TString outputFile, const TString outputFigurePrefix)
{
    std::cout<<"running zJetPlot_PRL()"<<std::endl;
    std::cout<<"configFile = "<< configFile.Data() <<std::endl;
    std::cout<<"inputFile = "<< inputFile.Data() <<std::endl;
    std::cout<<"outputFile = "<< outputFile.Data() <<std::endl;
    std::cout<<"outputFigurePrefix = "<< outputFigurePrefix.Data() <<std::endl;

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

    // input for mode
    /* mode = 0 : plot no systematics and no theory model
     * mode = 1 : plot systematics, but no theory model
     * mode = 2 : plot no systematics, but theory model
     * mode = 3 : plot systematics and theory model
     * mode = 4 : plot HI only, plot systematics and theory model
     * mode = 5 : plot PP only, plot systematics and theory model
     * mode = 6 : plot PP only, plot systematics, theory model and PPMC
     */
    int mode = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_mode];

    // input for TH1
    std::vector<std::string> TH1_paths = ConfigurationParser::ParseList(configInput.proc[INPUT::kHISTOGRAM].s[INPUT::k_TH1_path]);   // paths of TH1D histograms
    float markerSize = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_markerSize];

    // input for TLegend
    float legendOffsetX = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_legendOffsetX];
    float legendOffsetY = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_legendOffsetY];
    int legendBorderSize = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_legendBorderSize];
    float legendWidth = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_legendWidth];
    float legendHeight = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_legendHeight];
    float legendTextSize = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_legendTextSize];

    // input for text objects
    int textFont = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_textFont];
    float textSize = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_textSize];
    float textOffsetX = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_textOffsetX];
    float textOffsetY = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_textOffsetY];

    std::string tmpTextOverPad = ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_textAbovePad]);
    std::vector<std::string> textsOverPad = ConfigurationParser::ParseList(tmpTextOverPad);
    std::vector<std::string> textsOverPadAlignments = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_textAbovePadAlign]);
    int textAbovePadFont = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_textAbovePadFont];
    float textAbovePadSize = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_textAbovePadSize];
    float textAbovePadOffsetX = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_textAbovePadOffsetX];
    float textAbovePadOffsetY = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_textAbovePadOffsetY];

    // input for TCanvas
    int windowWidth = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_windowWidth];
    int windowHeight = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_windowHeight];
    float leftMargin = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_leftMargin];
    float rightMargin = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_rightMargin];
    float bottomMargin = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_bottomMargin];
    float topMargin = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_topMargin];
    int setLogx = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_setLogx];
    int setLogy = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_setLogy];

    // set default values
    if (markerSize == 0)  markerSize = INPUT_DEFAULT::markerSize;

    if (textFont == 0)  textFont = INPUT_DEFAULT::textFont;
    if (textSize == 0)  textSize = INPUT_DEFAULT::textSize;

    if (textAbovePadFont == 0)  textAbovePadFont = INPUT_DEFAULT::textAbovePadFont;
    if (textAbovePadSize == 0)  textAbovePadSize = INPUT_DEFAULT::textAbovePadSize;

    if (windowWidth  == 0)  windowWidth = INPUT_DEFAULT::windowWidth;
    if (windowHeight == 0)  windowHeight = INPUT_DEFAULT::windowHeight;
    if (leftMargin == 0) leftMargin = INPUT_DEFAULT::leftMargin;
    if (rightMargin == 0) rightMargin = INPUT_DEFAULT::rightMargin;
    if (bottomMargin == 0) bottomMargin = INPUT_DEFAULT::bottomMargin;
    if (topMargin == 0) topMargin = INPUT_DEFAULT::topMargin;

    int nTH1_Paths = TH1_paths.size();

    int nTextsOverPad = textsOverPad.size();
    int nTextsOverPadAlignments = textsOverPadAlignments.size();

    // verbose about input configuration
    std::cout<<"Input Configuration :"<<std::endl;
    std::cout << "mode = " << mode << std::endl;
    std::cout << "nTH1_Paths = " << nTH1_Paths << std::endl;
    for (int i = 0; i<nTH1_Paths; ++i) {
            std::cout << Form("TH1_Path[%d] = %s", i, TH1_paths.at(i).c_str()) << std::endl;
    }
    std::cout << "markerSize = " << markerSize << std::endl;

    std::cout << "legendOffsetX    = " << legendOffsetX << std::endl;
    std::cout << "legendOffsetY    = " << legendOffsetY << std::endl;
    std::cout << "legendBorderSize = " << legendBorderSize << std::endl;
    std::cout << "legendWidth      = " << legendWidth << std::endl;
    std::cout << "legendHeight     = " << legendHeight << std::endl;
    std::cout << "legendTextSize   = " << legendTextSize << std::endl;

    std::cout << "textFont = " << textFont << std::endl;
    std::cout << "textSize = " << textSize << std::endl;
    std::cout << "textOffsetX  = " << textOffsetX << std::endl;
    std::cout << "textOffsetY  = " << textOffsetY << std::endl;

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

    std::cout << "windowWidth = " << windowWidth << std::endl;
    std::cout << "windowHeight = " << windowHeight << std::endl;
    std::cout << "leftMargin   = " << leftMargin << std::endl;
    std::cout << "rightMargin  = " << rightMargin << std::endl;
    std::cout << "bottomMargin = " << bottomMargin << std::endl;
    std::cout << "topMargin    = " << topMargin << std::endl;
    std::cout << "setLogx  = " << setLogx << std::endl;
    std::cout << "setLogy  = " << setLogy << std::endl;

    // cuts in this macro are only used for adding text to the plots
    std::vector<float> bins_pt = ConfigurationParser::ParseListFloat(
            configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kZBOSON].s[CUTS::ZBO::k_bins_pt_gt]);

    std::vector<int> bins_hiBin[2];       // array of vectors for hiBin bins, each array element is a vector.
    bins_hiBin[0] = ConfigurationParser::ParseListInteger(
            configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_gt]);
    bins_hiBin[1] = ConfigurationParser::ParseListInteger(
            configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_lt]);

    // Z Boson cuts
    int doDiElectron = configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kZBOSON].i[CUTS::ZBO::k_doDiElectron];
    int doDiMuon = configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kZBOSON].i[CUTS::ZBO::k_doDiMuon];
    float massMin = configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kZBOSON].f[CUTS::ZBO::k_massMin];
    float massMax = configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kZBOSON].f[CUTS::ZBO::k_massMax];

    // electron cuts
    float elePt = configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kELECTRON].f[CUTS::ELE::k_elePt];
    float eleEta = configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleEta];

    // muon cuts
    float muPt = configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kMUON].f[CUTS::MUO::k_muPt];
    float muEta = configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kMUON].f[CUTS::MUO::k_muEta];

    // jet cuts
    std::string jetCollection = configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kJET].s[CUTS::JET::k_jetCollection];
    float jetpt = configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kJET].f[CUTS::JET::k_pt];
    float jeteta = configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kJET].f[CUTS::JET::k_eta];

    // zJet cuts
    // awayRange = 78 means dphi > 7/8 PI
    float awayRange = configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kZJET].f[CUTS::ZJT::k_awayRange];

    int nBins_pt = bins_pt.size();
    int nBins_hiBin = bins_hiBin[0].size();     // assume <myvector>[0] and <myvector>[1] have the same size.
    // verbose about cut configuration
    std::cout<<"Cut Configuration :"<<std::endl;
    std::cout << "nBins_zPt = " << nBins_pt << std::endl;
    for (int i = 0; i<nBins_pt; ++i) {
            std::cout << Form("bins_zPt[%d] = %d", i, (int)bins_pt.at(i)) << std::endl;
    }
    std::cout << "nBins_hiBin = " << nBins_hiBin << std::endl;
    for (int i=0; i<nBins_hiBin; ++i) {
        std::cout << Form("bins_hiBin[%d] = [%d, %d)", i, bins_hiBin[0].at(i), bins_hiBin[1].at(i)) << std::endl;
    }
    std::cout<<"doDiElectron = "<<doDiElectron<<std::endl;
    std::cout<<"doDiMuon     = "<<doDiMuon<<std::endl;
    std::cout << "massMin = " << massMin << std::endl;
    std::cout << "massMax = " << massMax << std::endl;
    std::cout << "elePt = " << elePt << std::endl;
    std::cout << "eleEta = " << eleEta << std::endl;
    std::cout << "muPt = " << muPt << std::endl;
    std::cout << "muEta = " << muEta << std::endl;
    std::cout << "jetCollection = "<< jetCollection.c_str() <<std::endl;
    std::cout << "jetpt  = " << jetpt << std::endl;
    std::cout << "jeteta = " << jeteta << std::endl;
    std::cout << "awayRange = " << awayRange << std::endl;

    // inputFile should be a list of ROOT files, that is a ".txt" or ".list" file.
    // the assumed order in inputFile :
    // 1. HI DATA
    // 2. HI MC
    // 3. PP DATA
    // 4. PP MC
    std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.Data());

    bool plotSystematics = (mode == 1 || mode == 3 || mode == 4 || mode == 5 || mode == 6);
    std::cout << "plotSystematics = " << plotSystematics << std::endl;
    bool plotTheory =  (mode == 2 || mode == 3 || mode == 4 || mode == 5 || mode == 6);
    bool plotTheoryHI = (mode == 2 || mode == 3 || mode == 4);
    bool plotTheoryPP = (mode == 2 || mode == 3 || mode == 5 || mode == 6);
    std::cout << "plotTheory = " << plotTheory << std::endl;
    std::cout << "plotTheoryHI = " << plotTheoryHI << std::endl;
    std::cout << "plotTheoryPP = " << plotTheoryPP << std::endl;

    int nInputFiles = inputFiles.size();
    std::vector<int> collisionTypes    {COLL::kHI,   COLL::kHIMC, COLL::kPP,   COLL::kPPMC};
    int nCollisions = collisionTypes.size();
    std::cout<<"input ROOT files : num = "<<nInputFiles<< std::endl;
    if (nInputFiles < 4 && !plotSystematics)  {
        std::cout<<"need to provide 4 input files, provide non-existing files as DUMMY.root" << std::endl;
        std::cout<<"exiting" << std::endl;
        return;
    }
    else {
        std::cout<<"#####"<< std::endl;
        std::cout<<"HI DATA : "<<inputFiles.at(0).c_str()<< std::endl;
        std::cout<<"HI MC   : "<<inputFiles.at(1).c_str()<< std::endl;
        std::cout<<"PP DATA : "<<inputFiles.at(2).c_str()<< std::endl;
        std::cout<<"PP MC   : "<<inputFiles.at(3).c_str()<< std::endl;
        std::cout<<"##### END #####"<< std::endl;
    }

    TFile* input[nCollisions];
    bool   inputExists[nCollisions];
    TDirectoryFile* inputDir[nCollisions];

    TFile* inputSys[nCollisions];
    bool   inputSysExists[nCollisions];
    TDirectoryFile* inputSysDir[nCollisions];

    std::string mcReferenceHI = "MC";
    std::string mcReferencePP = "MC";

    for (int i = 0; i<nCollisions; ++i) {

        inputExists[i] = false;
        inputSysExists[i] = false;

        input[i] = new TFile(inputFiles.at(i).c_str(), "READ");
        inputExists[i] = (input[i] && input[i]->IsOpen());
        if (!inputExists[i])  continue;

        std::string collisionName = getCollisionTypeName((COLL::TYPE)collisionTypes.at(i));
        inputDir[i] = (TDirectoryFile*)input[i]->GetDirectory(collisionName.c_str());

        if (i == COLL::kHIMC && inputExists[i])
        {
            std::string fileName = input[i]->GetName();
            std::string tmpMcReference = parseMCreference(COLL::kHIMC, fileName);
            if (tmpMcReference.size() > 0)  mcReferenceHI = tmpMcReference.c_str();
        }

        if (i == COLL::kPPMC && inputExists[i])
        {
            std::string fileName = input[i]->GetName();
            std::string tmpMcReference = parseMCreference(COLL::kPPMC, fileName);
            if (tmpMcReference.size() > 0)  mcReferencePP = tmpMcReference.c_str();
        }

        if (plotSystematics) {
            inputSys[i] = new TFile(inputFiles.at(i+nCollisions).c_str(), "READ");
            inputSysExists[i] = (inputSys[i] && inputSys[i]->IsOpen());
            if (!inputSysExists[i])  continue;

            std::string dirName = "SYS";
            inputSysDir[i] = (TDirectoryFile*)inputSys[i]->GetDirectory(dirName.c_str());
        }
    }

    // special cases
    if (!inputExists[COLL::kHI] && inputExists[COLL::kHIMC]) {
        inputDir[COLL::kHIMC] = (TDirectoryFile*)input[COLL::kHIMC]->GetDirectory("PPMC");
    }
    // special cases - END
    bool isUnsmearedPP = (std::string(input[COLL::kPP]->GetName()).find("noCorrJetSmearALL") != std::string::npos);

    TFile* output = new TFile(outputFile, "RECREATE");
    TTree *configTree = setupConfigurationTreeForWriting(configCuts);

    std::string leptonSymbol = "";
    std::string leptonSymbolele = "e";
    std::string leptonSymbolmu = "#mu";
    std::string diLeptonPt = "zPt";
    float leptonPt = -1;
    float leptonEta = -1;
    if (doDiElectron > 0) {
        leptonSymbol = leptonSymbolele.c_str();
        leptonPt = elePt;
        leptonEta = eleEta;
    }
    else if (doDiMuon > 0) {
        leptonSymbol = leptonSymbolmu.c_str();
        leptonPt = muPt;
        leptonEta = muEta;
    }
    else {
        std::cout<<"None of doDiElectron and doDiMuon options are set."<<std::endl;
        std::cout<<"exiting" << std::endl;
        return;
    }
    if (doDiElectron > 0 && doDiMuon > 0) {
        leptonSymbol = "l";
        leptonPt = muPt;
        leptonEta = 2.4;
    }

    const std::vector<std::string> legendEntryLabels_const {"PbPb DATA", "PbPb MC", "pp DATA", "pp MC"};

    std::vector<std::string> correlationHistNames   {"xjz", "xjz", "xjz_rebin", "dphi_rebin", "dphi_rebin", "dphi_rebin", "dphi_rebin", "dphi_rebin_normJZ", "ptJet", "zM", "zPt",
                                                     "rjz", "xjz_mean", "rjz_zNum", "xjz_mean", "iaa", "iaa_ptBin5", "iaa_ptBin6", "xjz_binJER", "xjz_binJER2", "xjz_binJER", "rjz_ratio", "xjz_mean_ratio",
                                                     "zEta", "zPhi", "jteta", "jtphi", "ptJet"};

    std::vector<std::string> versionSuffix {"final_norm", "final_norm", "final_norm", "final_norm", "final_norm", "final_norm", "final_norm", "final_norm", "final_norm", "final", "final_norm",
                                            "", "", "", "", "final_norm", "final_norm", "final_norm", "final_norm", "final_norm", "final_norm", "", "",
                                            "final_norm", "final_norm", "final_norm", "final_norm", "final_norm"};
    std::vector<std::string> jetRegion {"SIG", "SIG", "SIG", "SIG", "SIG", "SIG", "SIG", "SIG", "SIG", "RAW", "RAW",
                                           "", "SIG", "", "RAW", "SIG", "SIG", "SIG", "SIG", "SIG", "SIG", "", "SIG",
                                           "RAW", "RAW", "RAW", "RAW", "RAW"};
    std::vector<std::string> legendPositions {"NE", "NW", "NE", "NW", "NW", "NW", "NW", "NW", "NW", "NE", "NE",
                                            "NW", "NE", "NW", "NE", "NW", "NW", "NW", "NE", "NE", "NE", "NW", "NW",
                                            "NW", "NW", "NW", "NW", "NE"};
    std::vector<std::string> textPositions   {"NE", "NE", "NE", "NW", "NW", "NW", "NW", "NW", "NE", "NW", "NE",
                                              "SE", "NE", "SE", "NE", "NE", "NE", "NE", "NE", "NE", "NE", "NE", "NE",
                                              "NE", "NE", "NE", "NE", "NE" };
    std::vector<bool> textWriteZPt    {true, true, true, true,  true,  true,  true,  true,  true, true, true,
                                              false, false, false, false, true, true, true, true, true, true, false, false,
                                              true, true, true, true, true};
    std::vector<bool> textWriteDphi   {true, true, true, false, false, false, false, false, true, true, true,
                                              true, true, true, true, true, true, true, true, true, true, true, true,
                                              true, true, true, true, true};
    std::vector<bool> textWritejetPtEtaSameLine
                                      {false, false, false, false, false, false, false, false, false, false, true,
                                              false, false, false, false, false, false, false, false, false, false, false, false,
                                              true, true, true, true, false};
    std::vector<std::string> drawOptionsMC   {"hist", "hist", "hist", "hist", "hist", "hist", "hist", "hist", "hist", "hist", "hist",
                                              "e", "e", "e", "e", "e", "e", "e", "hist", "hist", "hist", "e", "e",
                                              "hist", "hist", "hist", "hist", "hist"};
    std::vector<std::string> legEntriesMC    {"lf",   "lf",   "lf",   "lf",   "lf",   "lf",   "lf",   "lf",   "lf",   "lf",   "lf",
                                              "lfp", "lfp", "lfp", "lfp", "lfp", "lfp", "lfp", "lf", "lf", "lf", "lfp", "lfp",
                                              "lfp", "lfp", "lfp", "lfp", "lfp"};

    // decided on which collision to be plotted based on the existence of the histogram files
    int nCorrHist = correlationHistNames.size();
    std::vector<bool> vecTrue   (nCorrHist, true);
    std::vector<bool> vecFalse  (nCorrHist, false);

    std::vector<bool> do_PLOT = vecFalse;
    // select the list of observables for which plots will be made.
    std::vector<std::string> correlationHistNamesTODO = {"xjz", "dphi_rebin", "dphi_rebin_normJZ", "rjz", "xjz_mean", "xjz_binJER", "xjz_binJER2", "ptJet", "iaa", "iaa_ptBin5", "iaa_ptBin6", "rjz_ratio", "xjz_mean_ratio"};  // , "zM", "zPt", "zEta", "zPhi"
    int sizeTODO = correlationHistNamesTODO.size();
    for (int i = 0; i < sizeTODO; ++i) {

        int iTmp = findPositionInVector(correlationHistNames, correlationHistNamesTODO.at(i).c_str());
        if (iTmp > -1)  do_PLOT.at(iTmp) = true;
    }

    std::vector<int>   plotLogY  (nCorrHist, 0);
    std::vector<bool>  plotRatio (nCorrHist, false);
    // special cases
    int index_xjz_ratio = 1;
    do_PLOT.at(index_xjz_ratio) = true;
    plotRatio.at(index_xjz_ratio) = true;
    int index_dphi_rebin_logY = 4;
    do_PLOT.at(index_dphi_rebin_logY) = true;
    plotLogY.at(index_dphi_rebin_logY) = 1;         // plot second dphi plot in log scale.
    int index_dphi_rebin_ratio = 5;
    do_PLOT.at(index_dphi_rebin_ratio) = true;
    plotRatio.at(index_dphi_rebin_ratio) = true;    // plot 3rd dphi plot with pp/PbPb ratio.
    int index_dphi_rebin_logY_ratio = 6;
    do_PLOT.at(index_dphi_rebin_logY_ratio) = true;
    plotLogY.at(index_dphi_rebin_logY_ratio) = 1;         // plot 4th dphi plot in log scale.
    plotRatio.at(index_dphi_rebin_logY_ratio) = true;     // plot 4th dphi plot with pp/PbPb ratio.
    int index_xjz_binJER_ratio = 20;
    do_PLOT.at(index_xjz_binJER_ratio) = true;
    plotRatio.at(index_xjz_binJER_ratio) = true;        // plot one of the xjz plots with pp/PbPb ratio.
    // special cases - END

    std::vector<bool> plotHI = vecFalse;
    std::vector<bool> plotHIMC = vecFalse;
    std::vector<bool> scaleHIMC = vecFalse;
    std::vector<bool> plotPP = vecFalse;
    std::vector<bool> plotPP_hist = vecFalse;
    std::vector<bool> plotPPMC = vecFalse;
    std::vector<bool> scalePPMC = vecFalse;

    if (inputExists[COLL::kHI])
    {
        plotHI = vecTrue;
    }
    if (inputExists[COLL::kHIMC])
    {
        plotHIMC = vecTrue;

        scaleHIMC = vecFalse;
        int iTmp = findPositionInVector(correlationHistNames, "zM");
        if (iTmp > -1)  scaleHIMC.at(iTmp) = true;
    }
    if (inputExists[COLL::kPP])
    {
        plotPP = vecTrue;

        std::vector<std::string> correlationHistNamesTmp =
        {"zM", "zPt", "zEta", "zPhi", "jteta", "jtphi", "iaa", "iaa_ptBin5", "iaa_ptBin6", "rjz_ratio", "xjz_mean_ratio"};

        int sizeTmp = correlationHistNamesTmp.size();
        for (int i = 0; i < sizeTmp; ++i) {

            int iTmp = findPositionInVector(correlationHistNames, correlationHistNamesTmp.at(i).c_str());
            if (iTmp > -1 && plotHI.at(iTmp))  plotPP.at(iTmp) = false;
        }

        if (inputExists[COLL::kHIMC])  plotPP_hist = vecFalse;
    }
    if (inputExists[COLL::kPPMC])
    {
        plotPPMC = vecTrue;

        scalePPMC = vecFalse;
        int iTmp = findPositionInVector(correlationHistNames, "zM");
        if (iTmp > -1)  scalePPMC.at(iTmp) = true;
    }

    // plot pp as histogram if only PbPb and pp data is plotted.
    if (inputExists[COLL::kHI] && inputExists[COLL::kPP] && !inputExists[COLL::kHIMC] && !inputExists[COLL::kPPMC])
    {

        std::vector<std::string> correlationHistNamesTmp =
        {};

        int sizeTmp = correlationHistNamesTmp.size();
        for (int i = 0; i < sizeTmp; ++i) {

            int iTmp = findPositionInVector(correlationHistNames, correlationHistNamesTmp.at(i).c_str());
            if (iTmp > -1)  plotPP_hist.at(iTmp) = true;
        }
    }

    // special cases
    if (!inputExists[COLL::kHI])
    {
        for (int i = 0; i < nTextsOverPad; ++i)
        {
            textsOverPad.at(i) = replaceAll(textsOverPad.at(i), "s_{NN}", "s");
            textsOverPad.at(i) = replaceAll(textsOverPad.at(i), "404 #mub^{-1}, ", "");
            textsOverPad.at(i) = replaceAll(textsOverPad.at(i), "PbPb", "");
        }
    }

    // systematics
    std::vector<bool> plotSYSfromFIT[nCollisions];
    plotSYSfromFIT[COLL::kHI] = vecFalse;
    plotSYSfromFIT[COLL::kHIMC] = vecFalse;
    plotSYSfromFIT[COLL::kPP] = vecFalse;
    plotSYSfromFIT[COLL::kPPMC] = vecFalse;

    for (int i=0; i<nCorrHist; ++i) {
        std::vector<std::string> correlationHistNamesTmp =
        {"xjz", "xjz_rebin", "dphi", "dphi_rebin"};

        int nTMP = correlationHistNamesTmp.size();
        for (int i2 = 0; i2 < nTMP ; ++i2) {
            if (correlationHistNamesTmp.at(i2) == correlationHistNames.at(i))
            {
                plotSYSfromFIT[COLL::kHI].at(i) = false;
                plotSYSfromFIT[COLL::kPP].at(i) = false;
            }
        }
    }

    if (plotTheoryHI && !plotTheoryPP) plotPP = vecFalse;
    if (plotTheoryPP && !plotTheoryHI) plotHI = vecFalse;

    // declaration of graphics objects.
    // the idea is to initialize each Graphics object right before its use, then to delete it right after they are saved/written.
    TCanvas* c = 0;
    TPad* pad = 0;
    TPad* pad2 = 0;
    TLegend* leg = 0;

    double falpha = 0.70;
    int kFSolid_ColorAlpha = 1001;  // transparency does not work with kFSolid, must use 1001.

    TBox* box = new TBox();
    int boxColor = 46;
    box->SetFillColorAlpha(boxColor, falpha);
    box->SetFillStyle(kFSolid_ColorAlpha);

    TBox* boxPP = new TBox();
    int boxColorPP = 30;
    boxPP->SetFillColorAlpha(boxColorPP, falpha);
    boxPP->SetFillStyle(kFSolid_ColorAlpha);

    TGraph* gr = new TGraph();
    gr->SetFillColorAlpha(boxColor, falpha);
    gr->SetFillStyle(kFSolid_ColorAlpha);

    TGraph* grPP = new TGraph();
    grPP->SetFillColorAlpha(boxColorPP, falpha);
    grPP->SetFillStyle(kFSolid_ColorAlpha);

    TH1::SetDefaultSumw2();
    TH1D* h1D[nInputFiles][nCorrHist];
    bool  h1DisValid[nInputFiles][nCorrHist];

    TH1D* h_nums[nInputFiles][nCorrHist];
    bool  h_numsIsValid[nInputFiles][nCorrHist];

    TH1D* h1DSys[nInputFiles][nCorrHist];
    bool  h1DSysIsValid[nInputFiles][nCorrHist];

    // no horizontal error bars
    gStyle->SetErrorX(0);
    gStyle->SetHatchesLineWidth(3);

    bool useRelUnc = false;

    TH1D* hTmp = 0;
    TH1D* hLegend = 0;
    TLine* lineTmp = 0;
    TH1D* hTmp_lowerPad = 0;

    TArrow* arrow = 0;
    TArrow* arrowPP = 0;
    TLine*  line = 0;
    TLine*  linePP = 0;
    for (int i=0; i<nCorrHist; ++i){
        for (int iPt=0; iPt < nBins_pt; ++iPt){
            for (int iHiBin=0; iHiBin < nBins_hiBin; ++iHiBin){

                if (!do_PLOT.at(i))  continue;

                std::string correlation = correlationHistNames.at(i).c_str();
                std::cout<<"##### "<< correlation.c_str() <<" #####"<<std::endl;

                std::string tmpName = Form("%s_ptBin%d_hiBin%d_jet%s_%s", correlation.c_str(), iPt, iHiBin,
                        jetRegion.at(i).c_str(), versionSuffix.at(i).c_str());
                // special cases
                if (correlation == "rjz" || correlation == "rjz_zNum" ) {
                    if (iPt > 0)  continue;
                    tmpName = Form("%s_ptBinAll_hiBin%d", correlation.c_str(), iHiBin);
                }
                else if (correlation == "xjz_mean") {
                    if (iPt > 0)  continue;
                    tmpName = Form("%s_ptBinAll_hiBin%d_jet%s", correlation.c_str(), iHiBin, jetRegion.at(i).c_str());
                }
                else if (correlation == "rjz_ratio") {
                    if (iPt > 0)  continue;
                    tmpName = Form("%s_ptBinAll_hiBin%d", correlation.c_str(), iHiBin);
                }
                else if (correlation == "xjz_mean_ratio") {
                    if (iPt > 0)  continue;
                    tmpName = Form("%s_ptBinAll_hiBin%d_jet%s", correlation.c_str(), iHiBin, jetRegion.at(i).c_str());
                }
                else if (correlation == "iaa_ptBin5" || correlation == "iaa_ptBin6") {
                    if (iPt > 0)  continue;
                    if (correlation == "iaa_ptBin5" ) {
                        tmpName = Form("%s_ptBin%d_hiBin%d_jet%s_%s", "iaa", 5, iHiBin,
                                jetRegion.at(i).c_str(), versionSuffix.at(i).c_str());
                    }
                    if (correlation == "iaa_ptBin6" ) {
                        tmpName = Form("%s_ptBin%d_hiBin%d_jet%s_%s", "iaa", 6, iHiBin,
                                jetRegion.at(i).c_str(), versionSuffix.at(i).c_str());
                    }
                }
                // special cases - END

                std::string tmpHistName = Form("h1D_%s", tmpName.c_str());
                std::string tmpHistName_nums = Form("h_nums_ptBin%d_hiBin%d", iPt, iHiBin);

                // no centrality bin for PP or PPMC
                std::string tmpNamePP = replaceAll(tmpName, Form("_hiBin%d", iHiBin), "_hiBin0");
                std::string tmpHistNamePP = Form("h1D_%s", tmpNamePP.c_str());
                std::string tmpHistName_numsPP = replaceAll(tmpHistName_nums, Form("_hiBin%d", iHiBin), "_hiBin0");

                // read histograms
                std::cout<<"reading histogram : "<< tmpHistName.c_str() <<std::endl;
                for (int iColl = 0; iColl < nCollisions; ++iColl){
                    std::string tmpNameColl = tmpName.c_str();
                    std::string tmpHistNameColl = tmpHistName.c_str();

                    if (iColl == COLL::kPP || iColl == COLL::kPPMC) {
                        tmpNameColl = tmpNamePP.c_str();
                        tmpHistNameColl = tmpHistNamePP.c_str();
                        tmpHistName_nums = tmpHistName_numsPP.c_str();
                    }

                    // read h1D
                    h1D[iColl][i] = 0;
                    if (inputExists[iColl])
                        inputDir[iColl]->GetObject(tmpHistNameColl.c_str(), h1D[iColl][i]);

                    h1DisValid[iColl][i] = false;
                    if (h1D[iColl][i])  h1DisValid[iColl][i] = true;

                    // read h_nums
                    if (inputExists[iColl])
                        inputDir[iColl]->GetObject(tmpHistName_nums.c_str(), h_nums[iColl][i]);

                    h_numsIsValid[iColl][i] = false;
                    if (h_nums[iColl][i])  h_numsIsValid[iColl][i] = true;

                    h1DSysIsValid[iColl][i] = false;
                    if (plotSystematics) {
                        useRelUnc = false;

                        std::string tmpHistSysName = Form("h1D_fnc_%s_uncTot_diff_pol_0", tmpNameColl.c_str());
                        if (correlation == "xjz")  {
                            tmpHistSysName = Form("h1D_%s_uncTot_diff_rel", tmpNameColl.c_str());
                            useRelUnc = true;
                        }
                        if (correlation.find("xjz_binJER") == 0)  {
                            // use the systematics calculated from "xjz", NOT "xjz_binJER" or "xjz_binJER2"
                            tmpHistSysName = Form("h1D_%s_uncTot_diff_rel", replaceAll(tmpNameColl.c_str(), correlation.c_str(), "xjz").c_str());
                            useRelUnc = true;
                        }
                        if (correlation == "dphi_rebin" || correlation == "dphi_rebin_normJZ")  {
                            tmpHistSysName = Form("h1D_%s_uncTot_diff", tmpNameColl.c_str());
                            if (iColl == COLL::kHI)  tmpHistSysName = Form("h1D_fnc_%s_uncTot_diff_pol_2", tmpNameColl.c_str());
                        }
                        if (correlation == "rjz" || correlation == "xjz_mean")  {
                            tmpHistSysName = Form("h1D_fnc_%s_uncTot_diff_pol_1_rel", tmpNameColl.c_str());
                            useRelUnc = true;
                        }
                        if (correlation == "ptJet")  {
                            tmpHistSysName = Form("h1D_%s_uncTot_diff_rel", tmpNameColl.c_str());
                            useRelUnc = true;
                        }
                        if (correlation.find("iaa") == 0)  {
                            std::string tmpNameColl2 = replaceAll(tmpNameColl,  "_ptBin5", "_ptBin0");
                            tmpNameColl2 = replaceAll(tmpNameColl2, "_ptBin6", "_ptBin0");
                            tmpHistSysName = Form("h1D_%s_uncTot_diff_rel", tmpNameColl2.c_str()); // use iaa systematics for iaa_ptBin5 and iaa_ptBin6
                            useRelUnc = true;
                        }

                        if (plotSYSfromFIT[iColl].at(i))  tmpHistSysName = Form("h1D_%s_uncTot_Fit", tmpNameColl.c_str());

                        if (inputSysExists[iColl])
                            inputSysDir[iColl]->GetObject(tmpHistSysName.c_str(), h1DSys[iColl][i]);

                        if (h1DSys[iColl][i])  h1DSysIsValid[iColl][i] = true;
                    }
                }

                // set canvas
                c = new TCanvas("cnv","",windowWidth,windowHeight);
                std::string tmpCnvName = Form("cnv_%s", tmpName.c_str());
                setLogy = plotLogY.at(i);
                if (setLogy > 0) tmpCnvName = Form("%s_logY", tmpCnvName.c_str());
                if (plotRatio.at(i)) tmpCnvName = Form("%s_ratio", tmpCnvName.c_str());

                c->SetName(tmpCnvName.c_str());
                std::cout<<"preparing canvas : "<< c->GetName() <<std::endl;

                setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
                setCanvasFinal(c, setLogx, setLogy);
                c->cd();
                double windowHeightScale = 1;
                double windowHeightFraction = 0.25;
                if (plotRatio.at(i)) {
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

                leg = new TLegend();
                // make legend transparent
                leg->SetFillColor(-1);
                leg->SetFillStyle(4000);

                bool ish1D_xjz = is_xjz(correlation);
                bool ish1D_dphi = is_dphi(correlation);

                if (h1DisValid[COLL::kHI][i] && plotHI.at(i)) {
                    setTH1(h1D[COLL::kHI][i], COLL::kHI);
                    h1D[COLL::kHI][i]->SetMarkerSize(markerSize);
                    setTH1_correlation(correlation, h1D[COLL::kHI][i]);

                    if (h1DSysIsValid[COLL::kHI][i]) {
                        h1D[COLL::kHI][i]->SetFillColor(boxColor);
                        h1D[COLL::kHI][i]->SetFillColorAlpha(boxColor, falpha);
                    }

                    // for TLegend purposes only
                    hLegend = (TH1D*)h1D[COLL::kHI][i]->Clone(Form("%s_legend", h1D[COLL::kHI][i]->GetName()));
                    hLegend->SetLineWidth(0);
                    int cent_low = (int)(bins_hiBin[0].at(iHiBin)/2);
                    int cent_up  = (int)(bins_hiBin[1].at(iHiBin)/2);
                    leg->AddEntry(hLegend, Form("PbPb, %d-%d %%", cent_low, cent_up), "pf");
                    
                    // special cases
                    if (correlation == "ptJet" && jetRegion.at(i) == "RAW" && !plotHIMC.at(i))
                    {
                        std::string hTmpName = replaceAll(h1D[COLL::kHI][i]->GetName(), "_jetRAW", "_jetBKG");
                        inputDir[COLL::kHI]->GetObject(hTmpName.c_str(), hTmp);
                        setTH1(hTmp, COLL::kHI);

                        hTmp->SetMarkerStyle(kOpenCircle);
                        hTmp->SetMarkerSize(markerSize);

                        leg->AddEntry(hTmp, Form("PbPb MB, %d-%d %%", cent_low, cent_up), "lpf");
                    }
                }
                if (h1DisValid[COLL::kPP][i] && plotPP.at(i)) {
                    setTH1(h1D[COLL::kPP][i], COLL::kPP);
                    h1D[COLL::kPP][i]->SetMarkerSize(markerSize);
                    setTH1_correlation(correlation, h1D[COLL::kPP][i]);

                    if (!plotHI.at(i))   h1D[COLL::kPP][i]->SetMarkerStyle(kFullCircle);

                    std::string ppEntry = "pp";
                    if (isUnsmearedPP) ppEntry = "pp";
                    else if (plotHI.at(i) || plotTheoryPP) ppEntry = "Smeared pp";
                    if (h1DSysIsValid[COLL::kPP][i]) {
                        h1D[COLL::kPP][i]->SetFillColor(boxColorPP);
                        h1D[COLL::kPP][i]->SetFillColorAlpha(boxColorPP, falpha);
                    }

                    // for TLegend purposes only
                    hLegend = (TH1D*)h1D[COLL::kPP][i]->Clone(Form("%s_legend", h1D[COLL::kPP][i]->GetName()));
                    hLegend->SetLineWidth(0);
                    leg->AddEntry(hLegend, ppEntry.c_str(), "pf");
                }
                std::cout << "ipt = " << iPt << std::endl;
                std::cout << "ihibin = " << iHiBin << std::endl;
                std::cout << "h1DisValid[COLL::kHIMC][i] = " << h1DisValid[COLL::kHIMC][i] << std::endl;
                std::cout << "plotHIMC = " << plotHIMC.at(i) << std::endl;
                if (h1DisValid[COLL::kHIMC][i] && plotHIMC.at(i)) {

                    if (scaleHIMC.at(i))  h1D[COLL::kHIMC][i]->Scale(h1D[COLL::kHI][i]->Integral() / h1D[COLL::kHIMC][i]->Integral());

                    setTH1(h1D[COLL::kHIMC][i], COLL::kHIMC);
                    h1D[COLL::kHIMC][i]->SetMarkerSize(markerSize);
                    setTH1_correlation(correlation, h1D[COLL::kHIMC][i]);

                    std::string legeEntryMC = legEntriesMC.at(i).c_str();
                    leg->AddEntry(h1D[COLL::kHIMC][i], Form("%s", mcReferenceHI.c_str()), legeEntryMC.c_str());
                }
                if (h1DisValid[COLL::kPPMC][i] && plotPPMC.at(i)) {

                    if (scalePPMC.at(i))  h1D[COLL::kPPMC][i]->Scale(h1D[COLL::kPP][i]->Integral() / h1D[COLL::kPPMC][i]->Integral());

                    setTH1(h1D[COLL::kPPMC][i], COLL::kPPMC);
                    h1D[COLL::kPPMC][i]->SetMarkerSize(markerSize);
                    setTH1_correlation(correlation, h1D[COLL::kPPMC][i]);

                    std::string legeEntryMC = legEntriesMC.at(i).c_str();
                    leg->AddEntry(h1D[COLL::kPPMC][i], Form("%s", mcReferencePP.c_str()), legeEntryMC.c_str());
                }

                // set maximum/minimum of x-axis
                double xmin = 0;
                double xmax = -1;
                // set maximum/minimum of y-axis
                double histMin = 0;
                double histMax = -1;
                for (int iColl = 0; iColl < 3; ++iColl) {

                    if (!h1DisValid[iColl][i]) continue;
                    if (iColl == COLL::kHI && !plotHI.at(i))  continue;
                    if (iColl == COLL::kHIMC && !plotHIMC.at(i))  continue;
                    if (iColl == COLL::kPP && !plotPP.at(i))  continue;
                    if (iColl == COLL::kPPMC && !plotPPMC.at(i))  continue;

                    if (h1D[iColl][i]->GetMinimum() < histMin)   histMin = h1D[iColl][i]->GetMinimum();
                    if (h1D[iColl][i]->GetMaximum() > histMax)   histMax = h1D[iColl][i]->GetMaximum();

                    xmin = h1D[iColl][i]->GetXaxis()->GetBinLowEdge(h1D[iColl][i]->GetXaxis()->GetFirst());
                    xmax = h1D[iColl][i]->GetXaxis()->GetBinLowEdge(h1D[iColl][i]->GetXaxis()->GetLast()+1);
                }
                if (setLogy == 0) histMin = histMin-TMath::Abs(histMin)*0.3;

                if (ish1D_xjz)  {
                    histMax = 1.0;
                    if (plotTheory)  histMax = 1.3;
                    if (plotTheory && plotPPMC.at(i))  histMax = 1.2;
                    histMin = -0.05;
                    // zPt > 40
                    if (iPt == 1 ) {
                        histMax = 1.5;
                        histMin = -0.05;
                    }
                    // zPt > 50
                    if (iPt == 2 )  {
                        histMax = 1.5;
                        histMin = -0.05;
                    }
                }
                else if (correlation == "dphi_rebin") {
                    histMax = 3;
                    histMin = -0.2;
                    if (plotTheory)  histMax = 3.1;
                    if (plotTheory && plotPPMC.at(i))  histMax = 2.8;
                    // zPt > 40
                    if (iPt == 1 )  {
                        histMax = 3;
                        histMin = -0.2;
                    }

                    if (setLogy > 0) histMax = 5;
                }
                else if (correlation == "dphi_rebin_normJZ") { histMax = 0.76; histMin = -0.05; }
                else if (correlation == "rjz")       {
                    histMax = 1; histMin = 0;
                    if (plotTheory)  histMax = 1.0;
                }
                else if (correlation == "xjz_mean")  {histMax = 1.1; histMin = 0.6;}
                else if (correlation == "rjz_zNum")  histMax = 1.1;
                else if (correlation == "ptJet")  histMax = 0.02;
                else if (correlation == "jteta")     histMax = histMax+TMath::Abs(histMax)*0.4*TMath::Power(10,setLogy);
                else if (correlation == "jtphi")     histMax = histMax+TMath::Abs(histMax)*0.6*TMath::Power(10,setLogy);
                else if (correlation.find("iaa") == 0)       {histMax = 3.0; histMin = 0;}
                else if (correlation.find("rjz_ratio") == 0)       {histMax = 2.0; histMin = 0;}
                else if (correlation.find("xjz_mean_ratio") == 0)       {histMax = 1.6; histMin = 0.5;}
                else    histMax = histMax+TMath::Abs(histMax)*0.6*TMath::Power(10,setLogy);

                if (setLogy > 0)  histMin = 0.01;

                if (h1DisValid[COLL::kPP][i] && plotPP_hist.at(i)) {
                    h1D[COLL::kPP][i]->SetMinimum(histMin);
                    h1D[COLL::kPP][i]->SetMaximum(histMax);

                    h1D[COLL::kPP][i]->SetFillColor(90);
                    h1D[COLL::kPP][i]->SetLineWidth(2);
                    h1D[COLL::kPP][i]->Draw("hist same");
                }
                if (h1DisValid[COLL::kHIMC][i] && plotHIMC.at(i)) {
                    h1D[COLL::kHIMC][i]->SetMinimum(histMin);
                    h1D[COLL::kHIMC][i]->SetMaximum(histMax);

                    std::string drawOptionMC = drawOptionsMC.at(i).c_str();
                    h1D[COLL::kHIMC][i]->Draw(Form("%s same", drawOptionMC.c_str()));  // first draw "hist" option
                    std::string writeName = Form("%s_HIMC", h1D[COLL::kHIMC][i]->GetName());
                    h1D[COLL::kHIMC][i]->Write(writeName.c_str(), TObject::kOverwrite);
                }
                if (h1DisValid[COLL::kPPMC][i] && plotPPMC.at(i)) {
                    h1D[COLL::kPPMC][i]->SetMinimum(histMin);
                    h1D[COLL::kPPMC][i]->SetMaximum(histMax);

                    std::string drawOptionMC = drawOptionsMC.at(i).c_str();
                    h1D[COLL::kPPMC][i]->Draw(Form("%s same", drawOptionMC.c_str()));  // first draw "hist" option
                    std::string writeName = Form("%s_PPMC", h1D[COLL::kPPMC][i]->GetName());
                    h1D[COLL::kPPMC][i]->Write(writeName.c_str(), TObject::kOverwrite);
                }
                if (h1DisValid[COLL::kPP][i] && plotPP.at(i)) {
                    h1D[COLL::kPP][i]->SetMinimum(histMin);
                    h1D[COLL::kPP][i]->SetMaximum(histMax);

                    if (h1DSysIsValid[COLL::kPP][i]) {
                        h1D[COLL::kPP][i]->Draw("e same");
                        if (!(ish1D_xjz || ish1D_dphi) && !plotTheoryPP)  drawSysUncBoxes(grPP, h1D[COLL::kPP][i], h1DSys[COLL::kPP][i], useRelUnc);
                        else if (plotTheoryPP) drawSysUncBoxes(grPP, h1D[COLL::kPP][i], h1DSys[COLL::kPP][i], useRelUnc);
                    }
                    h1D[COLL::kPP][i]->Draw("e same");
                    std::string writeName = Form("%s_PP", h1D[COLL::kPP][i]->GetName());
                    h1D[COLL::kPP][i]->Write(writeName.c_str(), TObject::kOverwrite);
                }
                if (h1DisValid[COLL::kHI][i] && plotHI.at(i)) {
                    h1D[COLL::kHI][i]->SetMinimum(histMin);
                    h1D[COLL::kHI][i]->SetMaximum(histMax);

                    if (h1DSysIsValid[COLL::kHI][i]) {
                        h1D[COLL::kHI][i]->Draw("e same");
                        drawSysUncBoxes(gr, h1D[COLL::kHI][i], h1DSys[COLL::kHI][i], useRelUnc);

                        h1D[COLL::kHI][i]->SetLineWidth(3);
                        h1D[COLL::kHI][i]->SetLineColor(h1D[COLL::kHI][i]->GetFillColor());
                    }
                    h1D[COLL::kHI][i]->Draw("e same");
                    std::string writeName = Form("%s_HI", h1D[COLL::kHI][i]->GetName());
                    h1D[COLL::kHI][i]->Write(writeName.c_str(), TObject::kOverwrite);

                    // special cases
                    if (correlation == "ptJet" && jetRegion.at(i) == "RAW" && !plotHIMC.at(i))
                    {
                        if (hTmp) {
                            hTmp->Draw("e same");
                            hTmp->Write("", TObject::kOverwrite);
                        }
                    }
                    if ((ish1D_xjz || ish1D_dphi) && !plotTheoryPP)
                    {
                        // redraw pp data so that pp points are not hidden in the background.
                        if (h1DisValid[COLL::kPP][i] && plotPP.at(i)) {
                            if (h1DSysIsValid[COLL::kPP][i]) {
                                h1D[COLL::kPP][i]->Draw("e same");
                                drawSysUncBoxes(grPP, h1D[COLL::kPP][i], h1DSys[COLL::kPP][i], useRelUnc);
                            }
                            h1D[COLL::kPP][i]->Draw("e same");
                        }
                    }
                }

                // put info onto the canvas
                std::vector<std::string> textLines;
                int zPt = (int)bins_pt.at(iPt);
                std::string zPtLine = Form("p_{T}^{Z} > %d GeV/c", zPt);
                if (correlation == "iaa_ptBin5")  zPtLine = "60 < p_{T}^{Z} < 80 GeV/c";
                if (correlation == "iaa_ptBin6")  zPtLine = "p_{T}^{Z} > 80 GeV/c";
                if (textWriteZPt.at(i))  textLines.push_back(zPtLine.c_str());

                bool writeMassRangeInfo = false;
                if (writeMassRangeInfo) {
                    textLines.push_back(Form("%d < M^{%s%s} < %d GeV/c^{2}", (int)massMin, leptonSymbol.c_str(), leptonSymbol.c_str(), (int)massMax));
                }

                bool writeLeptonInfo = !(ish1D_xjz ||
                                         ish1D_dphi ||
                                         correlation == "rjz" || correlation == "rjz_zNum" ||
                                         correlation == "xjz_mean" || correlation.find("iaa") == 0 || correlation.find("ptJet") == 0 ||
                                         correlation == "rjz_ratio" || correlation == "xjz_mean_ratio");
                if (writeLeptonInfo) {
                    bool writeEleMuInfo = true;
                    if (writeEleMuInfo)
                    {
                        textLines.push_back(Form("p^{%s#pm}_{T} > %d GeV/c, |#eta^{%s#pm}| < %.1f", leptonSymbolmu.c_str(), (int)muPt,
                                leptonSymbolmu.c_str(), muEta));
                        textLines.push_back(Form("p^{%s#pm}_{T} > %d GeV/c, |#eta^{%s#pm}| < %.1f", leptonSymbolele.c_str(), (int)elePt,
                                leptonSymbolele.c_str(), eleEta));
                    }
                    else
                    {
                        textLines.push_back(Form("p^{%s#pm}_{T} > %d GeV/c, |#eta^{%s#pm}| < %.1f", leptonSymbol.c_str(), (int)leptonPt,
                                leptonSymbol.c_str(), leptonEta));
                    }
                }

                double jetRadius = 0.3;
                textLines.push_back(Form("anti-k_{T} jet R = %.1f", jetRadius));

                if (!textWritejetPtEtaSameLine.at(i)) {
                    textLines.push_back(Form("p_{T}^{jet} > %d GeV/c", (int)jetpt));
                    textLines.push_back(Form("|#eta^{jet}| < %.1f",     jeteta));
                }
                else  textLines.push_back(Form("p_{T}^{jet} > %d GeV/c, |#eta^{jet}| < %.1f", (int)jetpt, jeteta));

                if (jetCollection.size() > 0)  textLines.push_back(jetCollection.c_str());
                if (textWriteDphi.at(i))  {
                    if (awayRange > 0) {
                        // awayRange = 78 means dphi > 7/8 PI
                        std::string tmp_dphi = Form("#Delta#phi_{jZ} > #frac{%d}{%d}#pi", int(awayRange/10), int(awayRange)%10);
                        textLines.push_back(tmp_dphi.c_str());
                    }
                }

                int nTextLines = textLines.size();
                std::string textPosition = textPositions.at(i).c_str();

                bool drawLatexCMS = plotSystematics;
                bool drawLatexCMSExtra = drawLatexCMS && false;

                TLatex* latex = 0;
                if (nTextLines > 0) {
                    latex = new TLatex();
                    latex->SetTextFont(textFont);
                    latex->SetTextSize(textSize);
                    setTextAlignment(latex, textPosition.c_str());
                    float tmpTextOffsetY = textOffsetY;

                    if (textPosition != legendPositions.at(i)) {
                          if (textPosition.find("N") != std::string::npos)  tmpTextOffsetY = 0.08;
                    }
                    else if (legendPositions.at(i) == "NW" && drawLatexCMS)  {   // leave space for latexCMS
                        tmpTextOffsetY = textOffsetY+0.11;
                        if (!drawLatexCMSExtra && ish1D_xjz)  {
                            tmpTextOffsetY = textOffsetY+0.07;
                        }
                    }
                    if (ish1D_xjz && plotTheory) {   // push text below to leave space for VITEV legend
                        tmpTextOffsetY = tmpTextOffsetY+0.14;
                        if (plotPPMC.at(i))  tmpTextOffsetY = tmpTextOffsetY-0.06;
                    }
                    if (correlation.find("dphi") == 0 && plotTheory && !(plotTheoryHI && plotTheoryPP)) {   // push text below to leave space for JEWEL legend
                        tmpTextOffsetY = tmpTextOffsetY+0.14;
                    }
                    else if (correlation.find("dphi") == 0 && plotTheory) {   // push text below to leave space for JEWEL legend
                        tmpTextOffsetY = tmpTextOffsetY+0.18;
                    }
                    std::vector<std::pair<float,float>> textCoordinates = calcTextCoordinates(textLines, textPosition.c_str(), c, textOffsetX, tmpTextOffsetY);
                    for (int i = 0; i<nTextLines; ++i){
                        float x = textCoordinates.at(i).first;
                        float y = textCoordinates.at(i).second*(1-i*0.01);
                        std::string textLine = textLines.at(i).c_str();
                        if (ish1D_dphi && textLine.find("|#eta^{jet}|") == 0) {
                            // make sure the superscript in eta^{jet} does not overlap with the line above.
                            y = textCoordinates.at(i).second*(1-i*0.016);
                        }
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

                ////////// latexCMS //////////
                if (drawLatexCMS) {
                    // if theory models are plotted, then there is no place to add big CMS preliminary Latex.
                    TLatex* latexCMS = new TLatex();
                    TString cmsText     = "CMS";
                    float cmsTextFont   = 61;  // default is helvetic-bold

                    TString extraText   = "Preliminary";
                    if (!drawLatexCMSExtra)  extraText   = "";
                    float extraTextFont = 52;  // default is helvetica-italics

                    // text sizes and text offsets with respect to the top frame
                    // in unit of the top margin size
                    float cmsTextSize      = 1.05;
                    // float cmsTextOffset    = 0.1;  // only used in outOfFrame version
                    // ratio of "CMS" and extra text size
                    float extraOverCmsTextSize  = 0.76;

                    latexCMS->SetTextFont(cmsTextFont);
                    latexCMS->SetTextSize(cmsTextSize*pad->GetTopMargin());
                    setTextAlignment(latexCMS, "NW");
                    float posX = c->GetLeftMargin() + 0.04;
                    float posY = 1 - c->GetTopMargin() - 0.08;
                    if (correlation == "xjz_mean") {    // shift latexCMS to right to avoid overlap with data points
                        posX += 0.08;
                    }
                    latexCMS->DrawLatexNDC(posX, posY, cmsText);
                    if( drawLatexCMSExtra )
                      {
                        latexCMS->SetTextFont(extraTextFont);
                        latexCMS->SetTextSize(extraOverCmsTextSize*cmsTextSize*c->GetTopMargin());
                        latexCMS->DrawLatexNDC(posX, posY- 1.2*cmsTextSize*c->GetTopMargin(), extraText);
                      }
                }
                ////////// latexCMS - END //////////

                if (legendTextSize != 0)  leg->SetTextSize(legendTextSize);
                leg->SetBorderSize(legendBorderSize);
                double height = calcTLegendHeight(leg);
                double width = calcTLegendWidth(leg);
                if (legendHeight != 0)  height = legendHeight;
                if (legendWidth != 0)  width = legendWidth;
                std::string legendPosition = legendPositions.at(i).c_str();
                float tmpLegendOffsetY = legendOffsetY;
                float tmpLegendOffsetX = legendOffsetX;
                if (ish1D_xjz && (plotTheory && !plotTheoryPP))  legendPosition = "NW";
                if (legendPosition == "NE") tmpLegendOffsetX -= 0.11;
                if (legendPosition == "NW" && drawLatexCMS)  {   // leave space for latexCMS, shift legend down
                    tmpLegendOffsetY = legendOffsetY+0.11;
                    if (!drawLatexCMSExtra && (ish1D_xjz))  {
                        tmpLegendOffsetY = legendOffsetY+0.07;
                    }
                }

                setLegendPosition(leg, legendPosition.c_str(), c, height, width, tmpLegendOffsetX, tmpLegendOffsetY);
                leg->Draw();

                ////////// plot theory model //////////
                if (plotTheory && (iHiBin == 1 && iPt == 0)) {
                    TGraph* gr = new TGraph();
                    TGraphErrors* grErr = new TGraphErrors();

                    TLegend* legTheory = (TLegend*)leg->Clone("legTheory");
                    legTheory->Clear();
                    double widthFactor = 1.0;
                    double heightFactor = 1.0;

                    // hybrid model
                    double falpha_theory = 0.7;
                    double falpha_theory_jewel = 0.7;
                    double falpha_theory_PP = 0.7;
                    int kFSolid_ColorAlpha_theory = kFSolid_ColorAlpha;

                    std::vector<double> x_hybrid[HYBRID::kN_MODEL];
                    std::vector<double> ymin_hybrid[HYBRID::kN_MODEL];
                    std::vector<double> ymax_hybrid[HYBRID::kN_MODEL];
                    std::vector<double> xPP;
                    std::vector<double> yPP;
                    std::vector<double> yerrPP;
                    if (ish1D_xjz) {
                        for (int iModel = 0; iModel < HYBRID::kN_MODEL; ++iModel) {
                            x_hybrid[iModel] = HYBRID::x_xjz;
                            ymin_hybrid[iModel] = HYBRID::ymin_xjz[iModel];
                            ymax_hybrid[iModel] = HYBRID::ymax_xjz[iModel];
                        }
                        xPP =  HYBRID::x_xjz;
                        yPP = HYBRID::y_xjz_PP;
                        yerrPP = HYBRID::yerr_xjz_PP;
                    }
                    else if (ish1D_dphi) {
                        for (int iModel = 0; iModel < HYBRID::kN_MODEL; ++iModel) {
                            x_hybrid[iModel] = HYBRID::x_dphi;
                            ymin_hybrid[iModel] = HYBRID::ymin_dphi[iModel];
                            ymax_hybrid[iModel] = HYBRID::ymax_dphi[iModel];
                        }
                        xPP =  HYBRID::x_dphi;
                        yPP = HYBRID::y_dphi_PP;
                        yerrPP = HYBRID::yerr_dphi_PP;
                    }
                    else if (correlation == "rjz") {
                        for (int iModel = 0; iModel < HYBRID::kN_MODEL; ++iModel) {
                            x_hybrid[iModel] = HYBRID::x_rjz;
                            ymin_hybrid[iModel] = HYBRID::ymin_rjz[iModel];
                            ymax_hybrid[iModel] = HYBRID::ymax_rjz[iModel];
                        }
                        xPP =  HYBRID::x_rjz;
                        yPP = HYBRID::y_rjz_PP;
                        yerrPP = HYBRID::yerr_rjz_PP;
                    }
                    else if (correlation == "iaa_ptBin5") {
                        for (int iModel = 0; iModel < HYBRID::kN_MODEL; ++iModel) {
                            x_hybrid[iModel] = HYBRID::x_IAA_ptBin5[iModel];
                            ymin_hybrid[iModel] = HYBRID::ymin_IAA_ptBin5[iModel];
                            ymax_hybrid[iModel] = HYBRID::ymax_IAA_ptBin5[iModel];
                        }
                    }
                    else if (correlation == "iaa_ptBin6") {
                        for (int iModel = 0; iModel < HYBRID::kN_MODEL; ++iModel) {
                            x_hybrid[iModel] = HYBRID::x_IAA_ptBin6[iModel];
                            ymin_hybrid[iModel] = HYBRID::ymin_IAA_ptBin6[iModel];
                            ymax_hybrid[iModel] = HYBRID::ymax_IAA_ptBin6[iModel];
                        }
                    }
                    if (x_hybrid[0].size() > 0 && plotTheoryHI) {

                        gr->SetLineWidth(0);
                        // models for PbPb
                        for (int iModel = 0; iModel < HYBRID::kN_MODEL; ++iModel) {

                            setTGraphBand(gr, x_hybrid[iModel], ymin_hybrid[iModel], ymax_hybrid[iModel]);
                            gr->SetFillColorAlpha(HYBRID::fillColors[iModel], falpha_theory);
                            gr->SetFillStyle(kFSolid_ColorAlpha_theory);
                            gr->DrawClone("f");
                            legTheory->AddEntry(gr->Clone(), HYBRID::legendEntries[iModel].c_str(), "f");
                        }
                    }
                    if (xPP.size() > 0 && plotTheoryPP)
                    {
                        gr->SetLineWidth(0);

                        // pp ref
                        setTGraphErrors(grErr, xPP, yPP, yerrPP);
                        grErr->SetMarkerColorAlpha(HYBRID::fillColorPP, falpha_theory_PP);
                        //grErr->SetMarkerStyle(kFullTriangleUp);
                        //grErr->SetMarkerSize(2.5);
                        grErr->SetMarkerSize(0);
                        grErr->SetLineColorAlpha(HYBRID::fillColorPP, falpha_theory_PP);
                        grErr->SetLineWidth(3);     // 2
                        std::string ppRefPlotOption = "lp";
                        if (correlation == "rjz" || correlation == "xjz_mean") ppRefPlotOption = "lp";   // "p"
                        grErr->DrawClone(ppRefPlotOption.c_str());
                        legTheory->AddEntry(grErr->Clone(), HYBRID::legendEntryPP.c_str(), ppRefPlotOption.c_str());
                    }

                    // JEWEL
                    std::vector<double> x_jewel;
                    std::vector<double> y_jewel;
                    std::vector<double> yerr_jewel;
                    std::vector<double> xPP_jewel;
                    std::vector<double> yPP_jewel;
                    std::vector<double> yerrPP_jewel;
                    if (ish1D_xjz) {
                        x_jewel = JEWEL::x_xjz;
                        xPP_jewel =  JEWEL::x_xjz_PP;
                        y_jewel = JEWEL::y_xjz;
                        yerr_jewel = JEWEL::yerr_xjz;
                        yPP_jewel = JEWEL::y_xjz_PP;
                        yerrPP_jewel = JEWEL::yerr_xjz_PP;
                    }
                    else if (ish1D_dphi) {
                        x_jewel = JEWEL::x_dphi;
                        xPP_jewel =  JEWEL::x_dphi;
                        y_jewel = JEWEL::y_dphi;
                        yerr_jewel = JEWEL::yerr_dphi;
                        yPP_jewel = JEWEL::y_dphi_PP;
                        yerrPP_jewel = JEWEL::yerr_dphi_PP;
                    }
                    else if (correlation == "rjz") {
                        x_jewel = JEWEL::x_rjz;
                        xPP_jewel =  JEWEL::x_rjz;
                        y_jewel = JEWEL::y_rjz;
                        yerr_jewel = JEWEL::yerr_rjz;
                        yPP_jewel = JEWEL::y_rjz_PP;
                        yerrPP_jewel = JEWEL::yerr_rjz_PP;
                    }
                    else if (correlation == "xjz_mean") {
                        x_jewel = JEWEL::x_xjz_mean;
                        xPP_jewel =  JEWEL::x_xjz_mean;
                        y_jewel = JEWEL::y_xjz_mean;
                        yerr_jewel = JEWEL::yerr_xjz_mean;
                        yPP_jewel = JEWEL::y_xjz_mean_PP;
                        yerrPP_jewel = JEWEL::yerr_xjz_mean_PP;
                    }
                    if (x_jewel.size() > 0 && plotTheoryHI) {
                        gr->SetLineWidth(0);

                        // model for PbPb
                        grErr = new TGraphErrors();
                        setTGraphErrors(grErr, x_jewel, y_jewel, yerr_jewel);
                        grErr->SetMarkerColorAlpha(JEWEL::color, falpha_theory_jewel);
                        //grErr->SetMarkerStyle(kFullCross);
                        //grErr->SetMarkerSize(2.5);
                        grErr->SetMarkerSize(0);
                        grErr->SetLineColorAlpha(JEWEL::color, falpha_theory_jewel);
                        grErr->SetLineWidth(3);     // 2
                        std::string plotOption = "lp";
                        grErr->DrawClone(plotOption.c_str());
                        legTheory->AddEntry(grErr->Clone(), JEWEL::legendEntry.c_str(), plotOption.c_str());
                    }
                    if (xPP_jewel.size() > 0 && plotTheoryPP) {
                        gr->SetLineWidth(0);

                        // pp ref
                        grErr = new TGraphErrors();
                        setTGraphErrors(grErr, xPP_jewel, yPP_jewel, yerrPP_jewel);
                        grErr->SetMarkerColorAlpha(JEWEL::colorPP, falpha_theory_PP);
                        //grErr->SetMarkerStyle(kFullTriangleDown);
                        //grErr->SetMarkerSize(2.5);
                        grErr->SetMarkerSize(0);
                        grErr->SetLineColorAlpha(JEWEL::colorPP, falpha_theory_PP);
                        grErr->SetLineWidth(3);      // 2
                        std::string plotOption = "lp";
                        if (correlation == "rjz" || correlation == "xjz_mean") plotOption = "lp";   // "p"
                        grErr->DrawClone(plotOption.c_str());
                        legTheory->AddEntry(grErr->Clone(), JEWEL::legendEntryPP.c_str(), plotOption.c_str());
                    }

                    // VITEV
                    std::vector<double> x_VITEV[HYBRID::kN_MODEL];
                    std::vector<double> y_VITEV[HYBRID::kN_MODEL];
                    std::vector<double> xPP_VITEV;
                    std::vector<double> yPP_VITEV;
                    if (ish1D_xjz) {
                        for (int iModel = 0; iModel < VITEV::kN_MODEL; ++iModel) {
                            x_VITEV[iModel] = VITEV::x_xjz;
                            y_VITEV[iModel] = VITEV::y_xjz[iModel];
                        }
                        xPP_VITEV = VITEV::x_xjz;
                        yPP_VITEV = VITEV::y_xjz_PP;
                    }
                    if (x_VITEV[0].size() > 0 && plotTheoryHI) {

                        gr->SetLineWidth(0);
                        // models for PbPb
                        for (int iModel = 0; iModel < VITEV::kN_MODEL; ++iModel) {

                            grErr = new TGraphErrors();
                            int n_x = x_VITEV[iModel].size();
                            setTGraphErrors(grErr, x_VITEV[iModel], y_VITEV[iModel], std::vector<double>(n_x, 0));
                            grErr->SetMarkerColorAlpha(VITEV::fillColors[iModel], falpha_theory);     // HYBRID::fillColorPP
                            grErr->SetMarkerSize(0);
                            grErr->SetLineColorAlpha(VITEV::fillColors[iModel], falpha_theory);   // HYBRID::fillColorPP
                            grErr->SetLineWidth(3);     // 2
                            grErr->SetLineStyle(kDashed);
                            std::string plotOption = "lp";
                            grErr->DrawClone(plotOption.c_str());
                            legTheory->AddEntry(grErr->Clone(), VITEV::legendEntries[iModel].c_str(), plotOption.c_str());
                        }
                    }
                    if (xPP_VITEV.size() > 0 && plotTheoryPP)
                    {
                        gr->SetLineWidth(0);

                        // pp ref
                        grErr = new TGraphErrors();
                        int n_x = xPP_VITEV.size();
                        setTGraphErrors(grErr, xPP_VITEV, yPP_VITEV, std::vector<double>(n_x, 0));
                        grErr->SetMarkerColorAlpha(VITEV::fillColorPP, falpha_theory);     // HYBRID::fillColorPP
                        grErr->SetMarkerSize(0);
                        grErr->SetLineColorAlpha(VITEV::fillColorPP, falpha_theory);   // HYBRID::fillColorPP
                        grErr->SetLineWidth(3);     // 2
                        grErr->SetLineStyle(kDashed);
                        std::string plotOption = "lp";
                        grErr->DrawClone(plotOption.c_str());
                        legTheory->AddEntry(grErr->Clone(), VITEV::legendEntryPP.c_str(), plotOption.c_str());
                    }

                    // set "legTheory" properly by taking "leg" as reference
                    heightFactor = 1.0;                      // by default use the same height per entry
                    if (plotTheoryHI) {
                        heightFactor = 1.1*0.75;
                        if (ish1D_xjz) heightFactor = 0.75;
                    }
                    else if (plotTheoryPP)  {
                        if (ish1D_dphi) heightFactor = 1.9*0.75;
                        else if (ish1D_xjz) heightFactor = 0.75;
                        else heightFactor = 1.2;
                    }
                    double heightTheory = (height*legTheory->GetNRows()/leg->GetNRows()*heightFactor);

                    if (correlation == "xjz_mean") heightTheory = height;

                    // put "legTheory" below "leg"
                    legTheory->SetX1(leg->GetX1NDC());
                    legTheory->SetY2(leg->GetY1NDC());
                    if ((correlation == "rjz") || (ish1D_xjz && plotTheoryHI)) {
                        // put "legTheory" to the right of "leg"*
                        legTheory->SetX1(leg->GetX1NDC()+0.33);
                        if (correlation == "rjz") legTheory->SetY2(1 - c->GetTopMargin() - 0.03);    // push a little bit upwards.
                        else if ((ish1D_xjz && plotTheoryHI)) legTheory->SetY2(1 - c->GetTopMargin() - 0.04);    // push a little bit upwards.
                    }
                    widthFactor = 1.0;
                    if (plotTheoryHI)  widthFactor = 0.83;
                    else if (plotTheoryPP)  widthFactor = 1.2;

                    if (ish1D_dphi || correlation == "xjz_mean" || (ish1D_xjz && plotTheoryPP)) widthFactor = 1.0;

                    legTheory->SetX2(legTheory->GetX1NDC() + legendWidth*widthFactor);
                    legTheory->SetY1(legTheory->GetY2NDC() - heightTheory);

                    legTheory->Draw();
                }
                // replot data points after theory plots
                if (plotTheory) {
                    if (h1DisValid[COLL::kPP][i] && plotPP.at(i)) {

                        h1D[COLL::kPP][i]->Draw("e same");
                    }
                    if (h1DisValid[COLL::kHI][i] && plotHI.at(i)) {

                        h1D[COLL::kHI][i]->Draw("e same");
                    }
                    if ((ish1D_xjz || ish1D_dphi) && !plotTheoryPP)
                    {
                        // redraw pp data so that pp points are not hidden in the background.
                        if (h1DisValid[COLL::kPP][i] && plotPP.at(i)) {
                            h1D[COLL::kPP][i]->Draw("e same");
                        }
                    }
                }
                ////////// plot theory model - END //////////

                // special cases
                // draw a horizontal line at y = 0 for xjz and dphi.
                if (ish1D_xjz || ish1D_dphi) {
                    lineTmp = new TLine(xmin, 0, xmax, 0);
                    lineTmp->SetLineStyle(kDashed);   // https://root.cern.ch/doc/master/TAttLine_8h.html#a7092c0c4616367016b70d54e5c680a69
                    lineTmp->Draw();
                }
                if (correlation.find("iaa") == 0  || correlation == "rjz_ratio" || correlation == "xjz_mean_ratio") {
                    lineTmp = new TLine(xmin, 1, xmax, 1);
                    lineTmp->SetLineStyle(kDashed);   // https://root.cern.ch/doc/master/TAttLine_8h.html#a7092c0c4616367016b70d54e5c680a69
                    lineTmp->Draw();
                }
                // draw an arrow for mean of xjz
                bool drawArrow_xjz = false;
                if (ish1D_xjz && drawArrow_xjz) {

                    arrow = new TArrow();
                    arrow->SetLineColor(boxColor);
                    arrow->SetFillColor(boxColor);
                    arrow->SetLineWidth(3*1.5);
                    arrow->SetArrowSize(0.03);

                    arrowPP = (TArrow*)arrow->Clone();
                    arrowPP->SetLineColor(boxColorPP);
                    //arrowPP->SetFillColor(boxColorPP);
                    arrowPP->SetFillColor(-1);

                    // horizontal line for statistical uncertainty on the mean
                    line = new TLine();
                    line->SetLineWidth(3*1.5);

                    double xArrow = 0;
                    double xErr = 0;
                    double yMaxArrow = 0;
                    double yLine = 0;
                    if (h_numsIsValid[COLL::kHI][i] && plotHI.at(i)) {
                        xArrow = h_nums[COLL::kHI][i]->GetBinContent(6);
                        xErr = h_nums[COLL::kHI][i]->GetBinError(6);
                        yMaxArrow = h1D[COLL::kHI][i]->GetMaximum()*0.15*1.2;
                        yLine = yMaxArrow/2;
                        line->SetLineColor(h1D[COLL::kHI][i]->GetLineColor());

                        // arrow->DrawArrow(xArrow, yMaxArrow, xArrow, 0);
                        arrow->DrawLine(xArrow, yMaxArrow, xArrow, 0);  // do not draw an arrow, but a line
                        line->DrawLine(xArrow-xErr, yLine, xArrow+xErr, yLine);
                    }

                    linePP = (TLine*)line->Clone();
                    double xArrowPP = 0;
                    double xErrPP = 0;
                    double yMaxArrowPP = 0;
                    double yLinePP = 0;
                    if (h_numsIsValid[COLL::kPP][i] && plotPP.at(i)) {
                        xArrowPP = h_nums[COLL::kPP][i]->GetBinContent(6);
                        xErrPP = h_nums[COLL::kPP][i]->GetBinError(6);
                        yMaxArrowPP = h1D[COLL::kPP][i]->GetMaximum()*0.15*1.2;
                        yLinePP = yMaxArrowPP/2;
                        linePP->SetLineColor(h1D[COLL::kPP][i]->GetLineColor());

                        // arrowPP->DrawArrow(xArrowPP, yMaxArrowPP, xArrowPP, 0);
                        arrowPP->DrawLine(xArrowPP, yMaxArrowPP, xArrowPP, 0);  // do not draw an arrow, but a line
                        linePP->DrawLine(xArrowPP-xErrPP, yLinePP, xArrowPP+xErrPP, yLinePP);
                    }
                }

                if (plotRatio.at(i)) {
                    c->cd(2);

                    bool plotLowerPad_HI_PP = false;
                    bool plotLowerPad_PP_MC = false;
                    // ratio histogram
                    double yMin = 0;
                    double yMax = -1;
                    if (ish1D_dphi)
                    {
                        plotLowerPad_HI_PP = (h1DisValid[COLL::kHI][i] && plotHI.at(i)) && (h1DisValid[COLL::kPP][i] && plotPP.at(i));
                        plotLowerPad_PP_MC = (h1DisValid[COLL::kPPMC][i] && plotPPMC.at(i)) && (h1DisValid[COLL::kPP][i] && plotPP.at(i));
                        if (plotLowerPad_HI_PP) yMin = 0;
                        if (plotLowerPad_PP_MC) yMin = 0.2;
                        yMax = 1.6;
                    }
                    if (ish1D_xjz) {
                        plotLowerPad_HI_PP = (h1DisValid[COLL::kHI][i] && plotHI.at(i)) && (h1DisValid[COLL::kPP][i] && plotPP.at(i));
                        plotLowerPad_PP_MC = (h1DisValid[COLL::kPPMC][i] && plotPPMC.at(i)) && (h1DisValid[COLL::kPP][i] && plotPP.at(i));
                        yMin = 0;
                        yMax = 2;
                    }

                    if (plotLowerPad_HI_PP)
                    {
                        std::string tmpName = h1D[COLL::kHI][i]->GetName();
                        hTmp = (TH1D*)h1D[COLL::kHI][i]->Clone();
                        hTmp_lowerPad = (TH1D*)h1D[COLL::kHI][i]->Clone(Form("%s_ratio", tmpName.c_str()));
                        hTmp_lowerPad->Divide(h1D[COLL::kPP][i]);

                        setTH1(hTmp_lowerPad, COLL::kHI);

                        double axisSizeRatio = (c->GetPad(1)->GetAbsHNDC()/c->GetPad(2)->GetAbsHNDC());
                        setTH1Ratio(hTmp_lowerPad, hTmp, axisSizeRatio);

                        hTmp_lowerPad->SetAxisRange(yMin, yMax, "Y");
                        hTmp_lowerPad->SetYTitle("PbPb / pp");

                        hTmp_lowerPad->Draw("e same");

                        lineTmp = new TLine(xmin, 1, xmax, 1);
                        lineTmp->SetLineStyle(kDashed);   // https://root.cern.ch/doc/master/TAttLine_8h.html#a7092c0c4616367016b70d54e5c680a69
                        lineTmp->Draw();
                    }

                    if (plotLowerPad_PP_MC)
                    {
                        std::string tmpName = h1D[COLL::kPP][i]->GetName();
                        hTmp = (TH1D*)h1D[COLL::kPP][i]->Clone();
                        hTmp_lowerPad = (TH1D*)h1D[COLL::kPP][i]->Clone(Form("%s_ratio", tmpName.c_str()));
                        hTmp_lowerPad->Divide(h1D[COLL::kPPMC][i]);

                        setTH1(hTmp_lowerPad, COLL::kHI);

                        double axisSizeRatio = (c->GetPad(1)->GetAbsHNDC()/c->GetPad(2)->GetAbsHNDC());
                        setTH1Ratio(hTmp_lowerPad, hTmp, axisSizeRatio);

                        hTmp_lowerPad->SetAxisRange(yMin, yMax, "Y");
                        hTmp_lowerPad->SetYTitle("DATA / MC");

                        hTmp_lowerPad->Draw("e same");

                        lineTmp = new TLine(xmin, 1, xmax, 1);
                        lineTmp->SetLineStyle(kDashed);   // https://root.cern.ch/doc/master/TAttLine_8h.html#a7092c0c4616367016b70d54e5c680a69
                        lineTmp->Draw();
                    }
                }
                c->cd(1);

                c->Write("",TObject::kOverwrite);
                std::string outputFigureName = c->GetName();
                if (!outputFigurePrefix.EqualTo("")) {
                    outputFigureName = Form("%s%s", outputFigurePrefix.Data(), c->GetName());
                }
                c->SaveAs(Form("%s.png", outputFigureName.c_str()));
                c->Print(Form("%s.pdf", outputFigureName.c_str()));
                c->SaveAs(Form("%s.C",   outputFigureName.c_str()));

                c->cd();
                leg->Delete();
                c->Close();         // do not use Delete() for TCanvas.
                std::cout<<"##########"<<std::endl;
            }
        }
    }
    configTree->Write("",TObject::kOverwrite);

    output->Write("",TObject::kOverwrite);
    std::cout<<"Closing the input files."<<std::endl;
    for (int i = 0; i<nInputFiles; ++i) {
        if (inputExists[i])  {
            inputDir[i]->Close();
            input[i]->Close();
        }
    }
    std::cout<<"Closing the output file."<<std::endl;
    output->Close();

    std::cout<<"zJetPlot_PRL() - END"<<std::endl;
}

int main(int argc, char** argv)
{
    if (argc == 5) {
        zJetPlot_PRL(argv[1], argv[2], argv[3], argv[4]);
        return 0;
    }
    else if (argc == 4) {
        zJetPlot_PRL(argv[1], argv[2], argv[3]);
        return 0;
    }
    else if (argc == 3) {
        zJetPlot_PRL(argv[1], argv[2]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./zJetPlot_PRL.exe <configFile> <inputFile> <outputFile>"
                << std::endl;
        return 1;
    }
}

std::string parseMCreference(int iColl, std::string fileName) {

    std::string mcReference = "";

    if (iColl == COLL::kHIMC) {
        if (fileName.find("Pythia") != std::string::npos && fileName.find("Hydjet") != std::string::npos)
            mcReference = "Pythia+Hydjet";
        else if (fileName.find("Pythia") != std::string::npos)  mcReference = "Pythia";

        if (fileName.find("Pyquen") != std::string::npos && fileName.find("Hydjet") != std::string::npos)
            mcReference = "Pyquen+Hydjet";
        else if (fileName.find("Pyquen") != std::string::npos)  mcReference = "Pyquen";
    }
    else if (iColl == COLL::kPPMC) {
        if (fileName.find("Pythia") != std::string::npos)  mcReference = "Pythia";
        if (fileName.find("Pyquen") != std::string::npos)  mcReference = "Pyquen";
        //if (fileName.find("DYJetsToLL") != std::string::npos)  mcReference = "Madgraph";
    }
    if (fileName.find("DYJetsToLL") != std::string::npos)  mcReference = "Madgraph";

    return mcReference;
}

void setTH1(TH1 *h, COLL::TYPE collisionType) {

    h->GetXaxis()->CenterTitle();
    h->GetYaxis()->CenterTitle();

    double titleSize = 0.035*1.25*1.2;
    h->SetTitleSize(titleSize, "X");
    h->SetTitleSize(titleSize, "Y");

    h->SetTitleOffset(1.25, "X");
    h->SetTitleOffset(1.5,"Y");

    h->SetTitle("");
    h->SetStats(false);

    if (collisionType == COLL::kHI)
    {
        h->SetLineWidth(2);
        h->SetLineColor(kBlack);
        h->SetMarkerStyle(kFullCircle);
        h->SetMarkerColor(kBlack);
    }
    else if (collisionType == COLL::kHIMC)
    {
        h->SetLineWidth(3);
        h->SetLineColor(kOrange+7);     // kOrange=800
        h->SetFillColor(90);            // Higgs style
        h->SetMarkerColor(kOrange-2);

        h->SetLineColor(kBlue+2);     // kOrange=800
        h->SetFillColor(kBlue);            // Higgs style
        h->SetFillColorAlpha(kBlue, 1);
        h->SetFillStyle(kFHatched1);
        
        h->SetMarkerColor(h->GetLineColor());
        h->SetMarkerStyle(kFullTriangleDown);
    }
    else if (collisionType == COLL::kPP) {
        h->SetLineWidth(2);
        h->SetLineColor(kBlack);
        h->SetMarkerStyle(kOpenSquare);
        h->SetMarkerColor(kBlack);
    }
    else if (collisionType == COLL::kPPMC) {
        h->SetLineWidth(3);
        h->SetLineColor(kOrange+7);     // kOrange=800
        //h->SetFillColorAlpha(90, 0.55);         // Higgs style
        h->SetFillColorAlpha(kOrange+7, 1);
        h->SetFillStyle(kFHatched1+1);
        h->SetMarkerColor(h->GetLineColor());
        h->SetMarkerStyle(kFullTriangleUp);
    }
}

void setTH1_correlation(std::string correlation, TH1* h)
{
    if (is_xjz(correlation)) setTH1_xjz(h);
    if (is_dphi(correlation)) setTH1_dphi(h);
    if (correlation == "xjz_mean") setTH1_xjz_mean(h);
    if (correlation == "rjz") setTH1_rjz(h);
    if (correlation == "ptJet" || correlation.find("iaa") == 0) setTH1_ptJet(h);
    if (correlation == "zM") setTH1_diLeptonM(h);
}

void setTH1_xjz(TH1* h) {

    h->SetXTitle("x_{jZ} = p^{jet}_{T}/p^{Z}_{T}");
    h->SetYTitle("#frac{1}{N_{Z}} #frac{dN_{jZ}}{dx_{jZ}}");
}

void setTH1_dphi(TH1* h) {

    h->SetXTitle("#Delta#phi_{jZ}");
    h->SetYTitle("#frac{1}{N_{Z}} #frac{dN_{jZ}}{d#Delta#phi_{jZ}}");
}

void setTH1_ptJet(TH1* h) {

    h->SetXTitle("p^{jet}_{T} (GeV/c)");
}

void setTH1_diLeptonM(TH1* h) {

    h->SetMinimum(0);      // fix Y-Axis range to 0-0.2.
}

void setTH1_xjz_mean(TH1* h) {

    h->SetYTitle("<x_{jZ}>");
}

void setTH1_rjz(TH1* h) {

    h->SetYTitle("R_{jZ}");
}

bool is_xjz(std::string correlation)
{
    return (correlation == "xjz" || correlation == "xjz_rebin" ||
                                   correlation == "xjz_binJER" || correlation == "xjz_binJER2");
}

bool is_dphi(std::string correlation)
{
    return (correlation == "dphi" || correlation == "dphi_rebin");
}
