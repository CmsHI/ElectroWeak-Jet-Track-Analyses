#include <TFile.h>
#include <TDirectoryFile.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TLegend.h>
#include <TLine.h>
#include <TBox.h>
#include <TH1D.h>
#include <TF1.h>
#include <TLatex.h>
#include <TMath.h>
#include "TStyle.h"
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

void gammaJetPlot(const TString configFile, const TString inputFile, const TString outputFile = "gammaJetPlot.root", const TString outputFigurePrefix = "");
void setTH1_diLepton(TH1* h);
void setTH1StyleSample(TH1 *h, COLL::TYPE collisionType);
void setTH1_xjg(TH1* h);
void setTH1_dphi(TH1* h);
void setTH1_dphi_width(TH1* h);
void setTH1_IAA(TH1* h);

void gammaJetPlot(const TString configFile, const TString inputFile, const TString outputFile, const TString outputFigurePrefix)
{
    std::cout<<"running gammaJetPlot()"<<std::endl;
    std::cout<<"configFile = "<< configFile.Data() <<std::endl;
    std::cout<<"inputFile = "<< inputFile.Data() <<std::endl;
    std::cout<<"outputFile = "<< outputFile.Data() <<std::endl;
    std::cout<<"outputFigurePrefix = "<< outputFigurePrefix.Data() <<std::endl;

    InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

    // input for mode
    /* mode = 0 : plot no systematics and no theory model
     * mode = 1 : plot systematics, but no theory model
     * mode = 2 : plot no systematics, but theory model
     * mode = 3 : plot systematics and theory model
     */
    int mode;

    // input for TH1
    std::vector<std::string> TH1_paths;      // paths of TH1D histograms
    float markerSize;

    // input for TLegend
    float legendOffsetX;
    float legendOffsetY;
    int legendBorderSize;
    float legendWidth;
    float legendHeight;
    float legendTextSize;

    // input for text objects
    int textFont;
    float textSize;
    float textOffsetX;
    float textOffsetY;

    std::vector<std::string> textsOverPad;
    std::vector<std::string> textsOverPadAlignments;
    int textAbovePadFont;
    float textAbovePadSize;
    float textAbovePadOffsetX;
    float textAbovePadOffsetY;

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

        mode  = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_mode];

        TH1_paths   = ConfigurationParser::ParseList(configInput.proc[INPUT::kHISTOGRAM].s[INPUT::k_TH1_path]);
        markerSize = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_markerSize];

        legendOffsetX     = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_legendOffsetX];
        legendOffsetY     = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_legendOffsetY];
        legendBorderSize  = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_legendBorderSize];
        legendWidth       = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_legendWidth];
        legendHeight      = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_legendHeight];
        legendTextSize    = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_legendTextSize];

        textFont = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_textFont];
        textSize = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_textSize];
        textOffsetX = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_textOffsetX];
        textOffsetY = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_textOffsetY];

        std::string tmpTextOverPad = ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_textAbovePad]);
        textsOverPad = ConfigurationParser::ParseList(tmpTextOverPad);
        textsOverPadAlignments = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_textAbovePadAlign]);
        textAbovePadFont = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_textAbovePadFont];
        textAbovePadSize = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_textAbovePadSize];
        textAbovePadOffsetX = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_textAbovePadOffsetX];
        textAbovePadOffsetY = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_textAbovePadOffsetY];

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
        mode = 0;

        markerSize = 1;

        legendOffsetX = 0;
        legendOffsetY = 0;
        legendBorderSize = 0;
        legendWidth = 0;
        legendHeight = 0;
        legendTextSize = 0;

        textFont = 0;
        textSize = 0;
        textOffsetX = 0;
        textOffsetY = 0;

        textAbovePadFont = 0;
        textAbovePadSize = 0;
        textAbovePadOffsetX = 0;
        textAbovePadOffsetY = 0;

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
    std::vector<float> bins_pt[2];
    std::vector<int>   bins_hiBin[2];       // array of vectors for hiBin bins, each array element is a vector.
    // photon cuts
    // jet cuts
    std::string jetCollection;
    float jetpt;
    float jeteta;
    // zJet cuts
    float awayRange;    // awayRange = 78 means dphi > 7/8 PI
    if (configCuts.isValid) {
        bins_pt[0] = ConfigurationParser::ParseListFloat(
                configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_pt_gt]);
        bins_pt[1] = ConfigurationParser::ParseListFloat(
                configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_pt_lt]);

        bins_hiBin[0] = ConfigurationParser::ParseListInteger(
                configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_gt]);
        bins_hiBin[1] = ConfigurationParser::ParseListInteger(
                configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_lt]);

        jetCollection = configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kJET].s[CUTS::JET::k_jetCollection];
        jetpt  = configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kJET].f[CUTS::JET::k_pt];
        jeteta = configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kJET].f[CUTS::JET::k_eta];

       awayRange = configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kZJET].f[CUTS::ZJT::k_awayRange];
    }
    else {
        jetpt = 0;
        jeteta = 0;

        awayRange = 78;
    }
    // default values

    int nBins_pt = bins_pt[0].size();
    int nBins_hiBin = bins_hiBin[0].size();     // assume <myvector>[0] and <myvector>[1] have the same size.
    // verbose about cut configuration
    std::cout<<"Cut Configuration :"<<std::endl;
    std::cout << "nBins_pt = " << nBins_pt << std::endl;
    for (int i = 0; i<nBins_pt; ++i) {
        std::cout << Form("bins_pt[%d] = [%f, %f)", i, bins_pt[0].at(i), bins_pt[1].at(i)) << std::endl;
    }
    std::cout << "nBins_hiBin = " << nBins_hiBin << std::endl;
    for (int i=0; i<nBins_hiBin; ++i) {
        std::cout << Form("bins_hiBin[%d] = [%d, %d)", i, bins_hiBin[0].at(i), bins_hiBin[1].at(i)) << std::endl;
    }
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

    bool plotSystematics = (mode == 1 || mode == 3);
    std::cout << "plotSystematics = " << plotSystematics << std::endl;
    bool plotTheory =  (mode == 2 || mode == 3);
    std::cout << "plotTheory = " << plotTheory << std::endl;

    int nInputFiles = inputFiles.size();
    int nCollisions = 4;
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

    std::vector<int> collisionTypes    {COLL::kHI,   COLL::kHIMC, COLL::kPP,   COLL::kPPMC};
    int nCollisionTypes = collisionTypes.size();
    for (int i = 0; i<nCollisions; ++i) {

        input[i] = new TFile(inputFiles.at(i).c_str(), "READ");
        inputExists[i] = (input[i] && input[i]->IsOpen());
        if (!inputExists[i])  continue;

        std::string collisionName = getCollisionTypeName((COLL::TYPE)collisionTypes.at(i));
        //inputDir[i] = (TDirectoryFile*)input[i]->GetDirectory(collisionName.c_str());
        // switch histogram naming convention
        inputDir[i] = (TDirectoryFile*)input[i]->GetDirectory("");

        if (i == COLL::kHIMC && inputExists[i])
        {
            // std::string tmpFileName = input[i]->GetName();
            // if (tmpFileName.find("Pythia") != std::string::npos && tmpFileName.find("Hydjet") != std::string::npos)
            mcReferenceHI = "Pythia+Hydjet";
            // else if (tmpFileName.find("Pythia") != std::string::npos)  mcReferenceHI = "Pythia";

            // if (tmpFileName.find("Pyquen") != std::string::npos && tmpFileName.find("Hydjet") != std::string::npos)
            //     mcReferenceHI = "Pyquen+Hydjet";
            // else if (tmpFileName.find("Pyquen") != std::string::npos)  mcReferenceHI = "Pyquen";
        }

        if (i == COLL::kPPMC && inputExists[i])
        {
            // std::string tmpFileName = input[i]->GetName();
            // if (tmpFileName.find("Pythia") != std::string::npos)  mcReferencePP = "Pythia";
            // if (tmpFileName.find("Pyquen") != std::string::npos)  mcReferencePP = "Pyquen";
            // if (tmpFileName.find("DYJetsToLL") != std::string::npos)  mcReferencePP = "Madgraph";
            mcReferencePP = "Pythia";
        }

        if (plotSystematics) {
            inputSys[i] = new TFile(inputFiles.at(i+nCollisions).c_str(), "READ");
            inputSysExists[i] = (inputSys[i] && inputSys[i]->IsOpen());
            if (!inputSysExists[i])  continue;

            std::string dirName = "SYS";
            inputSysDir[i] = (TDirectoryFile*)inputSys[i]->GetDirectory(dirName.c_str());
        }
    }

    TFile* output = new TFile(outputFile, "RECREATE");
    TTree *configTree = setupConfigurationTreeForWriting(configCuts);

    std::string h1D_suffix_HI   = "PbPb_Data";
    std::string h1D_suffix_HIMC = "PbPb_MC";
    std::string h1D_suffix_PP   = "pp_Data";
    std::string h1D_suffix_PPMC = "pp_MC";
    std::string h1D_suffix = h1D_suffix_HI.c_str();     // will get updated where appropriate

    const std::vector<std::string> legendEntryLabels_const {"PbPb DATA", "PbPb MC", "pp DATA", "pp MC"};
    const std::vector<std::string> h1D_suffices {"PbPb_Data", "PbPb_MC", "pp_Data", "pp_MC"};

    std::vector<std::string> correlationHistNames  {"xjg", "dphi", "ptJet", "rjg_ptBinAll", "rjg_centBinAll",
                                                                            "xjg_mean_ptBinAll", "xjg_mean_centBinAll",
                                                                            "dphi_width_ptBinAll", "dphi_width_centBinAll",
                                                                            "iaa"};

    std::vector<std::string> versionSuffix         {h1D_suffix, h1D_suffix, h1D_suffix, h1D_suffix, h1D_suffix,
                                                                                        h1D_suffix, h1D_suffix,
                                                                                        h1D_suffix, h1D_suffix,
                                                                                        "rebin"};
    //std::vector<std::string> phoRegion {"SIG", "SIG", "SIG", "SIG", "SIG", "SIG"};
    //std::vector<std::string> jetRegion {"SIG", "SIG", "SIG", "SIG", "SIG", "SIG"};
    std::vector<std::string> legendPositions {"NW", "NW", "NE", "NW", "NE",
                                                                "NE", "NE",
                                                                "NE", "NE",
                                                                "NE"};
    std::vector<std::string> textPositions   {"NE", "NW", "SE", "SE", "SE",
                                                                "NE", "SE",
                                                                "SW", "SW",
                                                                "NE"};
    std::vector<bool> textWriteHiBin    {true, true, true, true, false,
                                                           true, false,
                                                           true, false,
                                                           true};
    std::vector<bool> textWritephoPt    {true, true, false, false, true,
                                                            false, true,
                                                            false, true,
                                                            true};
    std::vector<bool> textWriteDphi     {true, false, true, true, true,
                                                            true, true,
                                                            false, false,
                                                            true};
    std::vector<bool> textWritejetPtEtaSeparate
                                        {true, true, false, false, false,
                                                            false, false,
                                                            false, false,
                                                            false};
    std::vector<std::string> drawOptionsMC   {"hist", "hist", "hist", "hist", "hist",
                                                                      "hist", "hist",
                                                                      "hist", "hist",
                                                                      "hist"};
    std::vector<std::string> legEntriesMC    {"lf",   "lf",   "lf",  "lf",   "lf",
                                                                     "lf",   "lf",
                                                                     "lf",   "lf",
                                                                     "lf"};

    // decided on which collision to be plotted based on the existence of the histogram files
    int nCorrHist = correlationHistNames.size();
    std::vector<bool> vecTrue   (nCorrHist, true);
    std::vector<bool> vecFalse  (nCorrHist, false);

    std::vector<int>   plotLogY  (nCorrHist, 0);
    std::vector<bool>  plotRatio (nCorrHist, false);
//    int index_xjz_ratio = 1;
//    plotRatio.at(index_xjz_ratio) = true;
//    int index_dphi_rebin_logY = 4;
//    plotLogY.at(index_dphi_rebin_logY) = 1;         // plot second dphi plot in log scale.
//    int index_dphi_rebin_ratio = 5;
//    plotRatio.at(index_dphi_rebin_ratio) = true;    // plot 3rd    dphi with pp/PbPb ratio.
//    int index_dphi_rebin_logY_ratio = 6;
//    plotLogY.at(index_dphi_rebin_logY_ratio) = 1;         // plot second dphi plot in log scale.
//    plotRatio.at(index_dphi_rebin_logY_ratio) = true;         // plot second dphi plot in log scale.

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
    }
    if (inputExists[COLL::kPP])
    {
        plotPP = vecTrue;

        std::vector<std::string> correlationHistNamesTMP =
        {"ptJet", "jteta", "jtphi", "iaa"};

        int sizeTMP = correlationHistNamesTMP.size();
        for (int i = 0; i < sizeTMP; ++i) {

            int iTmp = findPositionInVector(correlationHistNames, correlationHistNamesTMP.at(i).c_str());
            if (iTmp > -1 && plotHI.at(iTmp))  plotPP.at(iTmp) = false;
        }

        if (inputExists[COLL::kHIMC])  plotPP_hist = vecFalse;
    }
    if (inputExists[COLL::kPPMC] && !inputExists[COLL::kHIMC])
    {
        plotPPMC = vecTrue;

        scalePPMC = vecFalse;
    }

    // plot pp with histogram if only PbPb and pp data is plotted.
    if (inputExists[COLL::kHI] && inputExists[COLL::kPP] && !inputExists[COLL::kHIMC] && !inputExists[COLL::kPPMC])
    {
        std::vector<std::string> correlationHistNamesTMP =
        {};

        int sizeTMP = correlationHistNamesTMP.size();
        for (int i = 0; i < sizeTMP; ++i) {

            int iTmp = findPositionInVector(correlationHistNames, correlationHistNamesTMP.at(i).c_str());
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
    std::vector<bool> plotSYSfromFIT[nCollisionTypes];
    plotSYSfromFIT[COLL::kHI] = vecFalse;
    plotSYSfromFIT[COLL::kHIMC] = vecFalse;
    plotSYSfromFIT[COLL::kPP] = vecFalse;
    plotSYSfromFIT[COLL::kPPMC] = vecFalse;

    /*
    for (int i=0; i<nCorrHist; ++i) {
        std::vector<std::string> correlationHistNamesTMP =
        {"xjg", "dphi"};

        int nTMP = correlationHistNamesTMP.size();
        for (int i2 = 0; i2 < nTMP ; ++i2) {
            if (correlationHistNamesTMP.at(i2).compare(correlationHistNames.at(i).c_str()) == 0)
            {
                plotSYSfromFIT[COLL::kHI].at(i) = false;
                plotSYSfromFIT[COLL::kPP].at(i) = false;
            }
        }
    }
    */

    // declaration of graphics objects.
    // the idea is to initialize each Graphics object right before its use, then to delete it right after they are saved/written.
    TCanvas* c;
    TPad* pad = 0;
    TPad* pad2 = 0;
    TLegend* leg = 0;

    double falpha = 0.70;
    int kFSolid_ColorAlpha = 1001;  // transparency does not work with kFSolid, must use 1001.

    TBox* box = new TBox();
    int boxColor = 46;      // Z+jet color = 46
    box->SetFillColorAlpha(boxColor, falpha);
    box->SetFillStyle(kFSolid_ColorAlpha);

    TBox* boxPP = new TBox();
    int boxColorPP = 30;      // Z+jet color = 30
    boxPP->SetFillColorAlpha(boxColorPP, falpha);
    boxPP->SetFillStyle(kFSolid_ColorAlpha);

    TH1::SetDefaultSumw2();
    TH1D* h1D[nInputFiles][nCorrHist];
    bool  h1DisValid[nInputFiles][nCorrHist];

    TH1D* h1DSys[nInputFiles][nCorrHist];
    bool  h1DSysIsValid[nInputFiles][nCorrHist];

    // no horizontal error bars
    gStyle->SetErrorX(0);

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

                std::string correlation = correlationHistNames.at(i).c_str();
                std::cout<<"##### "<< correlation.c_str() <<" #####"<<std::endl;

                /*
                std::string tmpName = Form("%s_ptBin%d_hiBin%d_pho%s_jet%s_%s", correlationHistNames.at(i).c_str(), iPt, iHiBin,
                        phoRegion.at(i).c_str(), jetRegion.at(i).c_str(), versionSuffix.at(i).c_str());
                // special cases
                if (correlation.compare("rjg_ptBinAll") == 0 || correlation.compare("xjg_mean_ptBinAll") == 0 ) {
                    if (iPt > 0)  continue;
                    tmpName = Form("%s_hiBin%d_pho%s_jet%s", correlationHistNames.at(i).c_str(),
                            iHiBin, phoRegion.at(i).c_str(), jetRegion.at(i).c_str());
                }
                else if (correlation.compare("rjg_centBinAll") == 0 || correlation.compare("xjg_mean_centBinAll") == 0 ) {
                    if (iHiBin > 0)  continue;
                    tmpName = Form("%s_ptBin%d_pho%s_jet%s", correlationHistNames.at(i).c_str(),
                            iPt, phoRegion.at(i).c_str(), jetRegion.at(i).c_str());
                }
                */
                std::string tmpName = Form("%s_ptBin%d_hiBin%d_%s", correlationHistNames.at(i).c_str(), iPt, iHiBin,
                                                                    versionSuffix.at(i).c_str());
                // special cases
                if (correlation.find("ptBinAll") != std::string::npos) {
                    if (iPt > 0)  continue;
                    tmpName = Form("%s_hiBin%d_%s", correlationHistNames.at(i).c_str(),
                            iHiBin, versionSuffix.at(i).c_str());
                }
                else if (correlation.find("centBinAll") != std::string::npos) {
                    if (iHiBin > 0)  continue;
                    tmpName = Form("%s_ptBin%d_%s", correlationHistNames.at(i).c_str(),
                            iPt, versionSuffix.at(i).c_str());
                }

                std::string tmpHistName = Form("h1D_%s", tmpName.c_str());

//                // no centrality bin for PP or PPMC
//                std::string tmpNamePP = replaceAll(tmpName, Form("_hiBin%d", iHiBin), "_hiBin0");
//                std::string tmpHistNamePP = Form("h1D_%s", tmpNamePP.c_str());

                // read histograms
                std::cout<<"reading histogram : "<< tmpHistName.c_str() <<std::endl;
                for (int iColl = 0; iColl < nCollisions; ++iColl){
                    std::string tmpNameColl = tmpName.c_str();
                    std::string tmpHistNameColl = tmpHistName.c_str();

                    if (iColl == COLL::kPP) {
                        std::string tmpNamePP = replaceAll(tmpName.c_str(), h1D_suffix, h1D_suffix_PP);
                        std::string tmpHistNamePP = Form("h1D_%s", tmpNamePP.c_str());

                        tmpNameColl = tmpNamePP.c_str();
                        tmpHistNameColl = tmpHistNamePP.c_str();
                    }
                    if (iColl == COLL::kPPMC) {
                        std::string tmpNamePP = replaceAll(tmpName.c_str(), h1D_suffix, h1D_suffix_PPMC);
                        std::string tmpHistNamePP = Form("h1D_%s", tmpNamePP.c_str());

                        tmpNameColl = tmpNamePP.c_str();
                        tmpHistNameColl = tmpHistNamePP.c_str();
                    }
                    if (iColl == COLL::kHIMC) {
                        std::string tmpNamePP = replaceAll(tmpName.c_str(), h1D_suffix, h1D_suffix_HIMC);
                        std::string tmpHistNamePP = Form("h1D_%s", tmpNamePP.c_str());

                        tmpNameColl = tmpNamePP.c_str();
                        tmpHistNameColl = tmpHistNamePP.c_str();
                    }

                    if (inputExists[iColl])
                        inputDir[iColl]->GetObject(tmpHistNameColl.c_str(), h1D[iColl][i]);

                    h1DisValid[iColl][i] = false;
                    if (h1D[iColl][i])  h1DisValid[iColl][i] = true;

                    h1DSysIsValid[iColl][i] = false;
                    if (plotSystematics) {
                        std::string tmpHistSysName = Form("h1D_fnc_%s_uncTot_diff_pol_1", tmpNameColl.c_str());
                        if (correlation.compare("xjg") == 0)  tmpHistSysName = Form("h1D_fnc_%s_uncTot_diff_pol_3", tmpNameColl.c_str());
                        if (correlation.compare("dphi") == 0)  tmpHistSysName = Form("h1D_fnc_%s_uncTot_diff_pol_1", tmpNameColl.c_str());

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

                if (h1DisValid[COLL::kHI][i] && plotHI.at(i)) {
                    setTH1StyleSample(h1D[COLL::kHI][i], COLL::kHI);
                    setTH1_diLepton(h1D[COLL::kHI][i]);
                    if (correlation.compare("xjg") == 0 || correlation.compare("xjg_rebin") == 0) setTH1_xjg(h1D[COLL::kHI][i]);
                    if (correlation.compare("dphi") == 0 || correlation.compare("dphi_rebin") == 0) setTH1_dphi(h1D[COLL::kHI][i]);
                    if (correlation.compare("dphi_width_ptBinAll") == 0 || correlation.compare("dphi_width_centBinAll") == 0)
                               setTH1_dphi_width(h1D[COLL::kHI][i]);
                    if (correlation.compare("iaa") == 0) setTH1_IAA(h1D[COLL::kHI][i]);

                    h1D[COLL::kHI][i]->SetMarkerSize(markerSize);

                    int cent_low = (int)(bins_hiBin[0].at(iHiBin)/2);
                    int cent_up  = (int)(bins_hiBin[1].at(iHiBin)/2);
                    if (h1DSysIsValid[COLL::kHI][i]) {  // for TLegend purposes only
                        h1D[COLL::kHI][i]->SetFillColor(boxColor);
                        h1D[COLL::kHI][i]->SetFillStyle(kFSolid_ColorAlpha);
                    }
                    hLegend = (TH1D*)h1D[COLL::kHI][i]->Clone(Form("%s_legend", h1D[COLL::kHI][i]->GetName()));
                    hLegend->SetLineWidth(0);
                    std::string legEntryStr = Form("PbPb, %d-%d %%", cent_low, cent_up);
                    if (!textWriteHiBin.at(i))  legEntryStr = "PbPb";
                    bool addLegEntry = (correlation.compare("iaa") != 0);   // do not put I_AA as legend entry
                    if (addLegEntry)  leg->AddEntry(hLegend, legEntryStr.c_str(), "pf");

                    // special cases
                    /*
                    if (correlation.compare("ptJet") == 0 && jetRegion.at(i).compare("RAW") == 0 && !plotHIMC.at(i))
                    {
                        std::string hTmpName = replaceAll(h1D[COLL::kHI][i]->GetName(), "_jetRAW", "_jetBKG");
                        inputDir[COLL::kHI]->GetObject(hTmpName.c_str(), hTmp);
                        setTH1StyleSample(hTmp, COLL::kHI);

                        hTmp->SetMarkerStyle(kOpenCircle);
                        hTmp->SetMarkerSize(markerSize);

                        std::string legEntryStrMB = Form("PbPb MB, %d-%d %%", cent_low, cent_up);
                        if (!textWriteHiBin.at(i))  legEntryStrMB = "PbPb MB";
                        leg->AddEntry(hTmp, legEntryStrMB.c_str(), "lpf");
                    }
                    */
                }
                if (h1DisValid[COLL::kPP][i] && plotPP.at(i)) {
                    setTH1StyleSample(h1D[COLL::kPP][i], COLL::kPP);
                    setTH1_diLepton(h1D[COLL::kPP][i]);
                    if (correlation.compare("xjg") == 0 || correlation.compare("xjg_rebin") == 0) setTH1_xjg(h1D[COLL::kPP][i]);
                    if (correlation.compare("dphi") == 0 || correlation.compare("dphi_rebin") == 0) setTH1_dphi(h1D[COLL::kPP][i]);
                    if (correlation.compare("dphi_width_ptBinAll") == 0 || correlation.compare("dphi_width_centBinAll") == 0)
                               setTH1_dphi_width(h1D[COLL::kPP][i]);
                    if (correlation.compare("iaa") == 0) setTH1_IAA(h1D[COLL::kPP][i]);

                    if (correlation.compare("jteta") == 0)
                    {
                        int nBins3 = h1D[COLL::kPP][i]->GetNbinsX();
                        for (int i3 = 0; i3 < nBins3 ;++i3) {
                            h1D[COLL::kPP][i]->SetBinContent(i3, h1D[COLL::kPP][i]->GetBinContent(i3)/25);
                            h1D[COLL::kPP][i]->SetBinError(i3,   h1D[COLL::kPP][i]->GetBinError(i3)/5);
                        }
                    }

                    h1D[COLL::kPP][i]->SetMarkerSize(markerSize);
                    if (!plotHI.at(i))   h1D[COLL::kPP][i]->SetMarkerStyle(kFullCircle);

                    std::string ppEntry = "pp";
                    if (plotHI.at(i)) ppEntry = "smeared pp";
                    if (h1DSysIsValid[COLL::kPP][i]) {  // for TLegend purposes only
                        h1D[COLL::kPP][i]->SetFillColor(boxColorPP);
                        h1D[COLL::kPP][i]->SetFillStyle(kFSolid_ColorAlpha);
                    }

                    hLegend = (TH1D*)h1D[COLL::kPP][i]->Clone(Form("%s_legend", h1D[COLL::kPP][i]->GetName()));
                    hLegend->SetLineWidth(0);
                    leg->AddEntry(hLegend, ppEntry.c_str(), "pf");
                }
                if (h1DisValid[COLL::kHIMC][i] && plotHIMC.at(i)) {
                    // DO NOT scale MC histogram
                    if (scaleHIMC.at(i))  h1D[COLL::kHIMC][i]->Scale(h1D[COLL::kHI][i]->Integral() / h1D[COLL::kHIMC][i]->Integral());

                    setTH1StyleSample(h1D[COLL::kHIMC][i], COLL::kHIMC);
                    setTH1_diLepton(h1D[COLL::kHIMC][i]);
                    if (correlation.compare("xjg") == 0 || correlation.compare("xjg_rebin") == 0) setTH1_xjg(h1D[COLL::kHIMC][i]);
                    if (correlation.compare("dphi") == 0 || correlation.compare("dphi_rebin") == 0) setTH1_dphi(h1D[COLL::kHIMC][i]);
                    if (correlation.compare("dphi_width_ptBinAll") == 0 || correlation.compare("dphi_width_centBinAll") == 0)
                               setTH1_dphi_width(h1D[COLL::kHIMC][i]);
                    if (correlation.compare("iaa")) setTH1_IAA(h1D[COLL::kHIMC][i]);

                    h1D[COLL::kHIMC][i]->SetMarkerSize(markerSize);

                    std::string legeEntryMC = legEntriesMC.at(i).c_str();
                    leg->AddEntry(h1D[COLL::kHIMC][i], Form("%s", mcReferenceHI.c_str()), legeEntryMC.c_str());
                }
                if (h1DisValid[COLL::kPPMC][i] && plotPPMC.at(i)) {
                    // DO NOT scale MC histogram
                    if (scalePPMC.at(i))  h1D[COLL::kPPMC][i]->Scale(h1D[COLL::kPP][i]->Integral() / h1D[COLL::kPPMC][i]->Integral());

                    setTH1StyleSample(h1D[COLL::kPPMC][i], COLL::kPPMC);
                    setTH1_diLepton(h1D[COLL::kPPMC][i]);
                    if (correlation.compare("xjg") == 0 || correlation.compare("xjg_rebin") == 0) setTH1_xjg(h1D[COLL::kPPMC][i]);
                    if (correlation.compare("dphi") == 0 || correlation.compare("dphi_rebin") == 0) setTH1_dphi(h1D[COLL::kPPMC][i]);
                    if (correlation.compare("dphi_width_ptBinAll") == 0 || correlation.compare("dphi_width_centBinAll") == 0)
                               setTH1_dphi_width(h1D[COLL::kPPMC][i]);
                    if (correlation.compare("iaa") == 0) setTH1_IAA(h1D[COLL::kPPMC][i]);

                    h1D[COLL::kPPMC][i]->SetMarkerSize(markerSize);

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

                if (correlation.compare("xjg") == 0)  {
                    histMax = 1.6;
                    histMin = -0.05;
                    if (iPt > 4 )  {
                        histMax = 2;
                        histMin = -0.05;
                    }
                }
                else if (correlation.compare("dphi") == 0)  {
                    histMax = 0.5;
                    histMin = -0.05;
                }
                else if (correlation.compare("rjg_ptBinAll") == 0 || correlation.compare("rjg_centBinAll") == 0)       {
                    histMax = 1; histMin = 0;
                    if (iPt > 4)  {
                        histMax = 1.2; histMin = 0.2;
                    }
                }
                else if (correlation.compare("xjg_mean_ptBinAll") == 0 || correlation.compare("xjg_mean_centBinAll") == 0)       {
                    histMax = 1.2; histMin = 0.6;
                }
                else if (correlation.compare("dphi_width_ptBinAll") == 0 || correlation.compare("dphi_width_centBinAll") == 0)       {
                    histMax = 0.5; histMin = 0;
                }
                else if (correlation.compare("iaa") == 0)       {
                    histMax = 3; histMin = 0;
                }
                else if (correlation.compare("xjg_mean") == 0)  {histMax = 1.1; histMin = 0.6;}
                else if (correlation.compare("rjg_zNum") == 0)  histMax = 1.1;
                else if (correlation.compare("jteta") == 0)     histMax = histMax+TMath::Abs(histMax)*0.4*TMath::Power(10,setLogy);
                else if (correlation.compare("jtphi") == 0)     histMax = histMax+TMath::Abs(histMax)*0.6*TMath::Power(10,setLogy);
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
                    h1D[COLL::kHIMC][i]->Write();
                }
                if (h1DisValid[COLL::kPPMC][i] && plotPPMC.at(i)) {
                    h1D[COLL::kPPMC][i]->SetMinimum(histMin);
                    h1D[COLL::kPPMC][i]->SetMaximum(histMax);

                    std::string drawOptionMC = drawOptionsMC.at(i).c_str();
                    h1D[COLL::kPPMC][i]->Draw(Form("%s same", drawOptionMC.c_str()));  // first draw "hist" option
                    h1D[COLL::kPPMC][i]->Write();
                }
                if (h1DisValid[COLL::kPP][i] && plotPP.at(i)) {
                    h1D[COLL::kPP][i]->SetMinimum(histMin);
                    h1D[COLL::kPP][i]->SetMaximum(histMax);

                    if (h1DSysIsValid[COLL::kPP][i]) {
                        h1D[COLL::kPP][i]->Draw("e same");
                        drawSysUncBoxes(boxPP, h1D[COLL::kPP][i], h1DSys[COLL::kPP][i]);
                    }
                    h1D[COLL::kPP][i]->Draw("e same");
                    h1D[COLL::kPP][i]->Write();
                }
                if (h1DisValid[COLL::kHI][i] && plotHI.at(i)) {
                    h1D[COLL::kHI][i]->SetMinimum(histMin);
                    h1D[COLL::kHI][i]->SetMaximum(histMax);

                    if (h1DSysIsValid[COLL::kHI][i]) {
                        h1D[COLL::kHI][i]->Draw("e same");
                        drawSysUncBoxes(box, h1D[COLL::kHI][i], h1DSys[COLL::kHI][i]);

                        h1D[COLL::kHI][i]->SetLineWidth(3);
                        h1D[COLL::kHI][i]->SetLineColor(h1D[COLL::kHI][i]->GetFillColor());
                    }
                    h1D[COLL::kHI][i]->Draw("e same");
                    h1D[COLL::kHI][i]->Write();

                    // special cases
                    /*
                    if (correlation.compare("ptJet") == 0 && jetRegion.at(i).compare("RAW") == 0 && !plotHIMC.at(i))
                    {
                        if (hTmp) {
                            hTmp->Draw("e same");
                            hTmp->Write();
                        }
                    }
                    */
                }

                // put info onto the canvas
                std::vector<std::string> textLines;

                if (correlation.compare("iaa") == 0) {
                    int cent_low = (int)(bins_hiBin[0].at(iHiBin)/2);
                    int cent_up  = (int)(bins_hiBin[1].at(iHiBin)/2);
                    std::string tmpTextLine = Form("%d-%d %%", cent_low, cent_up);
                    textLines.push_back(tmpTextLine.c_str());
                }

                if (textWritephoPt.at(i))  {
                    int phoPt1 = (int)bins_pt[0].at(iPt);
                    int phoPt2 = (int)bins_pt[1].at(iPt);
                    if (phoPt2 > 0 && phoPt2 < 9999)  textLines.push_back(Form("%d < p_{T}^{#gamma} < %d GeV/c", phoPt1, phoPt2));
                    else                              textLines.push_back(Form("p_{T}^{#gamma} > %d GeV/c", phoPt1));
                }

                double jetRadius = 0.3;
                textLines.push_back(Form("anti-k_{T} Jet R = %.1f", jetRadius));

                if (textWritejetPtEtaSeparate.at(i)) {
                    textLines.push_back(Form("p_{T}^{Jet} > %d GeV/c", (int)jetpt));
                    textLines.push_back(Form("|#eta^{Jet}| < %.1f",     jeteta));
                }
                else  textLines.push_back(Form("p_{T}^{Jet} > %d GeV/c, |#eta^{Jet}| < %.1f", (int)jetpt, jeteta));
                if (jetCollection.size() > 0)  textLines.push_back(jetCollection.c_str());
                if (textWriteDphi.at(i))  {
                    if (awayRange > 0) {
                        // awayRange = 78 means dphi > 7/8 PI
                        std::string tmp_dphi = Form("#Delta#phi_{J#gamma} > #frac{%d}{%d}#pi", int(awayRange/10), int(awayRange)%10);
                        textLines.push_back(tmp_dphi.c_str());
                    }
                }
                int nTextLines = textLines.size();
                std::string textPosition = textPositions.at(i).c_str();

                TLatex* latex = 0;
                if (nTextLines > 0) {
                    latex = new TLatex();
                    latex->SetTextFont(textFont);
                    latex->SetTextSize(textSize);
                    setTextAlignment(latex, textPosition.c_str());
                    float tmpTextOffsetY = textOffsetY;
                    if (textPosition.compare(legendPositions.at(i).c_str()) != 0) {
                          if (textPosition.find("N") != std::string::npos)  tmpTextOffsetY = 0.08;
                    }
                    else if (legendPositions.at(i).compare("NW") == 0)  {   // leave space for latexCMS
                        tmpTextOffsetY = textOffsetY+0.11;
                    }
                    if (correlation.compare("iaa") == 0)  tmpTextOffsetY = textOffsetY - 0.12;
                    std::vector<std::pair<float,float>> textCoordinates = calcTextCoordinates(textLines, textPosition.c_str(), c, textOffsetX, tmpTextOffsetY);
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

                ////////// latexCMS //////////
                bool drawLatexCMS = !plotTheory;
                if (drawLatexCMS) {      // if theory models are plotted, then there is no place to add big CMS preliminary Latex.
                    TLatex* latexCMS = new TLatex();
                    TString cmsText     = "CMS";
                    float cmsTextFont   = 61;  // default is helvetic-bold

                    bool writeExtraText = true;
                    TString extraText   = "Preliminary";
                    float extraTextFont = 52;  // default is helvetica-italics

                    // text sizes and text offsets with respect to the top frame
                    // in unit of the top margin size
                    float cmsTextSize      = 0.75;
    //                float cmsTextOffset    = 0.1;  // only used in outOfFrame version
                    // ratio of "CMS" and extra text size
                    float extraOverCmsTextSize  = 0.76;

                    float relExtraDY = 1.2;

                    float scaleLatexCMS = 1.4;
                    latexCMS->SetTextFont(cmsTextFont);
                    latexCMS->SetTextSize(cmsTextSize*pad->GetTopMargin()*scaleLatexCMS);
                    setTextAlignment(latexCMS, "NW");
                    float posX_ = c->GetLeftMargin() + 0.04;
                    float posY_ = 1 - c->GetTopMargin() - 0.08;
                    if (correlation.compare("xjg_mean") == 0) {
                        posX_ += 0.08;
                    }
                    latexCMS->DrawLatexNDC(posX_, posY_, cmsText);
                    if( writeExtraText )
                      {
                        latexCMS->SetTextFont(extraTextFont);
                        latexCMS->SetTextSize(extraOverCmsTextSize*cmsTextSize*c->GetTopMargin()*scaleLatexCMS);
                        latexCMS->DrawLatexNDC(posX_, posY_- relExtraDY*cmsTextSize*c->GetTopMargin(), extraText);
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
                if (legendPosition.compare("NW") == 0 && drawLatexCMS)  {   // leave space for latexCMS
                    tmpLegendOffsetY = legendOffsetY+0.11;
                }
                setLegendPosition(leg, legendPosition.c_str(), c, height, width, legendOffsetX, tmpLegendOffsetY);
                leg->Draw();

                // special cases
                // draw a horizontal line at y = 0 for xjz and dphi.
                if (correlation.compare("xjg") == 0 || correlation.compare("dphi") == 0) {
                    lineTmp = new TLine(xmin, 0, xmax, 0);
                    lineTmp->SetLineStyle(kDashed);   // https://root.cern.ch/doc/master/TAttLine_8h.html#a7092c0c4616367016b70d54e5c680a69
                    lineTmp->Draw();
                }
                // draw a horizontal line at y = 1 for I_AA
                if (correlation.compare("iaa") == 0) {
                    lineTmp = new TLine(xmin, 1, xmax, 1);
                    lineTmp->SetLineStyle(kDashed);   // https://root.cern.ch/doc/master/TAttLine_8h.html#a7092c0c4616367016b70d54e5c680a69
                    lineTmp->Draw();
                }
                // draw an arrow for mean of xjz
                if (correlation.compare("xjg") == 0) {
                    arrow = new TArrow();
                    arrow->SetLineColor(boxColor);
                    arrow->SetFillColor(boxColor);
                    arrow->SetLineWidth(3);
                    arrow->SetArrowSize(0.03);

                    arrowPP = (TArrow*)arrow->Clone();
                    arrowPP->SetLineColor(boxColorPP);
                    arrowPP->SetFillColor(boxColorPP);

                    double yMaxArrow = h1D[COLL::kHI][i]->GetMaximum()*0.15;
                    //double yMaxArrow = h1D[COLL::kHI][i]->GetMinimum();
                    double xArrow = h1D[COLL::kHI][i]->GetMean();
                    double xArrowPP = h1D[COLL::kPP][i]->GetMean();
                    double xErr = h1D[COLL::kHI][i]->GetMeanError();
                    double xErrPP = h1D[COLL::kPP][i]->GetMeanError();

                    // horizontal line for statistical uncertainty on the mean
                    line = new TLine();
                    line->SetLineColor(h1D[COLL::kHI][i]->GetLineColor());
                    line->SetLineWidth(3);
                    double yLine = yMaxArrow/2;

                    linePP = (TLine*)line->Clone();
                    linePP->SetLineColor(h1D[COLL::kPP][i]->GetLineColor());
                    double yLinePP = yLine;

                    arrow->DrawArrow(xArrow, yMaxArrow, xArrow, 0);
                    arrowPP->DrawArrow(xArrowPP, yMaxArrow, xArrowPP, 0);
                    line->DrawLine(xArrow-xErr, yLine, xArrow+xErr, yLine);
                    linePP->DrawLine(xArrowPP-xErrPP, yLinePP, xArrowPP+xErrPP, yLinePP);
                }

                if (plotRatio.at(i)) {
                            c->cd(2);

                            bool plotLowerPad_HI_PP = false;
                            bool plotLowerPad_PP_MC = false;
                            // ratio histogram
                            double yMin = 0;
                            double yMax = -1;
                            if (correlation.compare("dphi_rebin") == 0)
                            {
                                plotLowerPad_HI_PP = (h1DisValid[COLL::kHI][i] && plotHI.at(i)) && (h1DisValid[COLL::kPP][i] && plotPP.at(i));
                                plotLowerPad_PP_MC = (h1DisValid[COLL::kPPMC][i] && plotPPMC.at(i)) && (h1DisValid[COLL::kPP][i] && plotPP.at(i));
                                if (plotLowerPad_HI_PP) yMin = 0;
                                if (plotLowerPad_PP_MC) yMin = 0.2;
                                yMax = 1.6;
                            }
                            if (correlation.compare("xjg") == 0) {
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

                                setTH1StyleSample(hTmp_lowerPad, COLL::kHI);

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

                                setTH1StyleSample(hTmp_lowerPad, COLL::kHI);

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
    std::cout<<"##### END #####"<<std::endl;

    configTree->Write("",TObject::kOverwrite);

    output->Write("",TObject::kOverwrite);
    for (int i = 0; i<nInputFiles; ++i) {
        if (inputExists[i])  {
            inputDir[i]->Close();
            input[i]->Close();
        }
    }
    output->Close();
}

int main(int argc, char** argv)
{
    if (argc == 5) {
        gammaJetPlot(argv[1], argv[2], argv[3], argv[4]);
        return 0;
    }
    else if (argc == 4) {
        gammaJetPlot(argv[1], argv[2], argv[3]);
        return 0;
    }
    else if (argc == 3) {
        gammaJetPlot(argv[1], argv[2]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./gammaJetPlot.exe <configFile> <inputFile> <outputFile>"
                << std::endl;
        return 1;
    }
}

void setTH1_diLepton(TH1* h) {

    h->SetTitleOffset(1.25, "X");
    h->SetTitleOffset(1.5,"Y");

    h->SetTitle("");
    h->SetStats(false);
}

void setTH1StyleSample(TH1 *h, COLL::TYPE collisionType) {

    h->GetXaxis()->CenterTitle();
    h->GetYaxis()->CenterTitle();

    double titleSize = 0.035*1.25;
    h->SetTitleSize(titleSize, "X");
    h->SetTitleSize(titleSize, "Y");

    if (collisionType == COLL::kHI)
    {
        h->SetLineWidth(2);
        h->SetLineColor(kBlack);
        h->SetMarkerStyle(kFullCircle);
        h->SetMarkerColor(kBlack);
    }
    else if (collisionType == COLL::kHIMC)
    {
        h->SetLineWidth(2);
        h->SetLineColor(kOrange+7);     // kOrange=800
        h->SetFillColor(90);            // Higgs style
        h->SetMarkerColor(kOrange-2);
    }
    else if (collisionType == COLL::kPP) {
        h->SetLineWidth(2);
        h->SetLineColor(kBlack);
        h->SetMarkerStyle(kOpenCircle);
        h->SetMarkerColor(kBlack);
    }
    else if (collisionType == COLL::kPPMC) {
        h->SetLineWidth(2);
        h->SetLineColor(kOrange+7);     // kOrange=800
        h->SetFillColor(90);            // Higgs style
        h->SetMarkerColor(kOrange-2);
    }
}

void setTH1_xjg(TH1* h) {

    h->SetXTitle("x_{J#gamma} = p^{Jet}_{T}/p^{#gamma}_{T}");
}

void setTH1_dphi(TH1* h) {

    h->SetYTitle("#frac{1}{N_{J#gamma}} #frac{dN_{J#gamma}}{d#Delta#phi_{J#gamma}}");
}

void setTH1_dphi_width(TH1* h) {

    h->SetYTitle("#sigma (#Delta#phi_{J#gamma})");
}

void setTH1_IAA(TH1* h) {

    h->SetXTitle("p^{Jet}_{T} (GeV/c)");
    h->SetYTitle("Jet I_{AA}");
}

