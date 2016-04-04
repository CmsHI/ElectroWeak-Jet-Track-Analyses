#include <TFile.h>
#include <TDirectoryFile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TH1D.h>
#include <TF1.h>
#include <TLine.h>
#include <TLatex.h>

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

void zJetSystematics(const TString configFile, const TString inputFile, const TString outputFile = "zJetSystematics.root", const TString outputFigurePrefix = "");

void zJetSystematics(const TString configFile, const TString inputFile, const TString outputFile, const TString outputFigurePrefix)
{
    std::cout<<"running zJetSystematics()"<<std::endl;
    std::cout<<"configFile = "<< configFile.Data() <<std::endl;
    std::cout<<"inputFile = "<< inputFile.Data() <<std::endl;
    std::cout<<"outputFile = "<< outputFile.Data() <<std::endl;
    std::cout<<"outputFigurePrefix = "<< outputFigurePrefix.Data() <<std::endl;

    InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

    int collision;
    // input for TH1
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
        collision = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_collisionType];

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
        collision = COLL::kPP;

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

    int nTextsOverPad = textsOverPad.size();
    int nTextsOverPadAlignments = textsOverPadAlignments.size();

    // verbose about input configuration
    std::cout<<"Input Configuration :"<<std::endl;
    std::string collisionName =  getCollisionTypeName((COLL::TYPE)collision).c_str();
    std::cout << "collision = " << collisionName.c_str() << std::endl;
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

    // bool isMC = collisionIsMC((COLL::TYPE)collision);
    bool isHI = collisionIsHI((COLL::TYPE)collision);

    // cuts in this macro are only used for adding text to the plots
    std::vector<float> bins_pt;
    std::vector<int>   bins_hiBin[2];       // array of vectors for hiBin bins, each array element is a vector.
    // Z Boson cuts
    int doDiElectron;
    int doDiMuon;
    float massMin;
    float massMax;
    // electron cuts
    float elePt;
    // muon cuts
    float muPt;
    // jet cuts
    std::string jetCollection;
    float jetpt;
    float jeteta;
    // zJet cuts
    float awayRange;    // awayRange = 78 means dphi > 7/8 PI
    if (configCuts.isValid) {
        bins_pt = ConfigurationParser::ParseListFloat(
                configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kZBOSON].s[CUTS::ZBO::k_bins_pt_gt]);
        bins_hiBin[0] = ConfigurationParser::ParseListInteger(
                configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_gt]);
        bins_hiBin[1] = ConfigurationParser::ParseListInteger(
                configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_lt]);

        doDiElectron = configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kZBOSON].i[CUTS::ZBO::k_doDiElectron];
        doDiMuon = configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kZBOSON].i[CUTS::ZBO::k_doDiMuon];

        massMin = configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kZBOSON].f[CUTS::ZBO::k_massMin];
        massMax = configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kZBOSON].f[CUTS::ZBO::k_massMax];

        elePt = configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kELECTRON].f[CUTS::ELE::k_elePt];

        muPt = configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kMUON].f[CUTS::MUO::k_muPt];

        jetCollection = configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kJET].s[CUTS::JET::k_jetCollection];
        jetpt  = configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kJET].f[CUTS::JET::k_pt];
        jeteta = configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kJET].f[CUTS::JET::k_eta];

       awayRange = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kZJET].f[CUTS::ZJT::k_awayRange];
    }
    else {
        doDiElectron = 1;
        doDiMuon = 1;

        elePt = 10;

        muPt = 0;

        awayRange = 78;
    }
    // default values

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
    std::cout << "muPt = " << muPt << std::endl;
    std::cout << "jetCollection = "<< jetCollection.c_str() <<std::endl;
    std::cout << "jetpt  = " << jetpt << std::endl;
    std::cout << "jeteta = " << jeteta << std::endl;
    std::cout << "awayRange = " << awayRange << std::endl;

    // inputFile should be a list of ROOT files, that is a ".txt" or ".list" file.
    // the assumed order in inputFile :
    // 1     : nominal results
    // i > 1 : results varied by systematics
    std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.Data());

    int nInputFiles = inputFiles.size();
    std::cout<<"input ROOT files : num = "<<nInputFiles<< std::endl;
    if (nInputFiles < 2 )  {
        std::cout<<"need to provide at least 2 input files" << std::endl;
        std::cout<<"exiting" << std::endl;
        return;
    }
    else {
        std::cout<<"#####"<< std::endl;
        for (int i = 0; i < nInputFiles; ++i) {
            if (i == 0)  std::cout<<"nominal result : "<<inputFiles.at(0).c_str()<< std::endl;
            else         std::cout<<"sys. variation  : "<<inputFiles.at(i).c_str()<< std::endl;
        }
        std::cout<<"##### END #####"<< std::endl;
    }

    TFile* input[nInputFiles];
    bool   inputExists[nInputFiles];
    TDirectoryFile* inputDir[nInputFiles];
    for (int i = 0; i<nInputFiles; ++i) {

        input[i] = new TFile(inputFiles.at(i).c_str(), "READ");
        inputExists[i] = (input[i] && input[i]->IsOpen());
        if (!inputExists[i])  continue;

        inputDir[i] = (TDirectoryFile*)input[i]->GetDirectory(collisionName.c_str());
    }

    TFile* output = new TFile(outputFile, "RECREATE");
    // histograms will be put under a directory whose name contains the type of the collision. ~> FORGET ABOUT THIS APPROACH.
    // std::string dirName = Form("%s_SYS", collisionName.c_str());
    std::string dirName = "SYS";
    if(!output->GetKey(dirName.c_str())) output->mkdir(dirName.c_str(), Form("file with nominal results is %s", inputFiles.at(0).c_str()));
    output->cd(dirName.c_str());
    // output->Delete(Form("%s/*;1", dirName.c_str()));      // delete all systematics histograms from the first cycle.
    // https://root.cern.ch/doc/master/classTDirectory.html#af206b823d1d9a7ecadd1fbcf41b01c25
    std::cout<<"histograms will be put under directory : " << dirName.c_str() << std::endl;

    TTree *configTree = setupConfigurationTreeForWriting(configCuts);

    std::string leptonSymbol = "";
    std::string diLeptonM = "";
    std::string diLeptonPt = "";
    float leptonPt = -1;
    float leptonEta = -1;
    if (doDiElectron > 0) {
        leptonSymbol = "e";
        diLeptonM = "zM";
        diLeptonPt = "zPt";
        leptonPt = elePt;
        leptonEta = 2.5;
    }
    else if (doDiMuon > 0) {
        leptonSymbol = "#mu";
        diLeptonM = "zM";
        diLeptonPt = "zPt";
        leptonPt = muPt;
        leptonEta = 2.4;
    }
    else if (doDiElectron > 0 && doDiMuon > 0) {
        leptonSymbol = "l";
        diLeptonM = "zM";
        diLeptonPt = "zPt";
        leptonPt = muPt;
        leptonEta = 2.4;
    }
    else {
        std::cout<<"None of doDiElectron and doDiMuon options are set."<<std::endl;
        std::cout<<"exiting" << std::endl;
        return;
    }

    std::vector<std::string> correlationHistNames  {"xjz", "dphi", "ptJet", diLeptonM.c_str(), diLeptonPt.c_str(), "rjz", "xjz_mean", "rjz_zNum"};

    std::vector<std::string> versionSuffix {"final_norm", "final_norm", "final_norm", "final", "final_norm","", "", ""};
    std::vector<std::string> jetRegion {"SIG", "SIG", "SIG", "RAW", "RAW", "", "SIG", ""};
    std::vector<std::string> legendPositions {"NE", "NW", "NE", "NW", "NE", "NW", "NE", "NW"};
    std::vector<std::string> textPositions   {"NE", "NW", "NE", "NW", "NE", "NE", "NE", "SE"};
    std::vector<bool> textWriteDphi   {true, false, true, true, true, true, true, true};
    std::vector<std::string> functionFormulas {"([0]+[1]*x)*exp(-[2]*(x-[3]))*(x-[3])", "[0]*exp(-(TMath::Pi()-x)*[1])+[2]", "([0]+[1]*x)*exp(-[2]*(x-[3]))*(x-[3])", "NULL", "([0]+[1]*x)*exp(-[2]*(x-[3]))*(x-[3])", "NULL", "NULL", "NULL"};
    std::vector<std::string> fitOptions {"LL R M", "LL R M", "LL R M", "NULL", "LL R M", "NULL", "NULL", "NULL"};
    std::vector<bool>        calcUncfromFit {false, false, false, false, false, false, false, false};
    std::vector<std::string> functionFormulas_uncTot {"pol1", "pol1", "pol2", "pol1", "pol2", "NULL", "NULL", "NULL"};
    std::vector<int>         rebins {2, 2, 2, 1, 2, 1, 1, 1};

    int nCorrHist = correlationHistNames.size();

    // declaration of graphics objects.
    // the idea is to initialize each Graphics object right before its use, then to delete it right after they are saved/written.
    TCanvas* c;

    TH1::SetDefaultSumw2();
    TH1D* h1D[nInputFiles][nCorrHist];
    TH1D* h1D_ratio[nInputFiles][nCorrHist];    // h1D_ratio = h1D[0][] / h1D[i][]
    TH1D* h1D_diff[nInputFiles][nCorrHist];     // h1D_diff  = h1D[0][] - h1D[i][]
    TH1D* h1D_unc_ratio[nInputFiles][nCorrHist];      // h1D_unc = h1D_ratio - 1, SYS UNC. calculated from ratios
    TH1D* h1D_uncTot_ratio[nCorrHist];                // h1D_uncTot = sqrt( sum (h1D_unc^2) )
    TH1D* h1D_unc_diff[nInputFiles][nCorrHist];      // h1D_unc = h1D_diff, SYS UNC. calculated from differences
    TH1D* h1D_uncTot_diff[nCorrHist];                // h1D_uncTot = sqrt( sum (h1D_unc^2) )
    bool  h1DisValid[nInputFiles][nCorrHist];

    TF1*  fnc[nInputFiles][nCorrHist];
    bool  fncNominalHasbeenFit[nCorrHist];     // once a nominal distribution is fit do not fit it again in the next iteration over iFile.

    TF1*  fnc_uncTot[nCorrHist];          // polynomial fit to final total uncertainty
    TH1D* h1D_unctTot_Fit[nCorrHist];

    for (int i=0; i<nCorrHist; ++i){
        for (int iPt=0; iPt < nBins_pt; ++iPt){
            for (int iHiBin=0; iHiBin < nBins_hiBin; ++iHiBin){

                if (iHiBin > 0 && !isHI)  continue;

                std::string correlation = correlationHistNames.at(i).c_str();
                std::cout<<"##### "<< correlation.c_str() <<" #####"<<std::endl;

                std::string tmpName = Form("%s_ptBin%d_hiBin%d_jet%s_%s", correlationHistNames.at(i).c_str(), iPt, iHiBin,
                        jetRegion.at(i).c_str(), versionSuffix.at(i).c_str());
                // special cases
                if (correlation.compare("rjz") == 0 || correlation.compare("rjz_zNum") == 0 ) {
                    if (iPt > 0)  continue;
                    tmpName = Form("%s_ptBinAll_hiBin%d", correlationHistNames.at(i).c_str(), iHiBin);
                }
                else if (correlation.compare("xjz_mean") == 0) {
                    if (iPt > 0)  continue;
                    tmpName = Form("%s_ptBinAll_hiBin%d_jet%s", correlationHistNames.at(i).c_str(), iHiBin, jetRegion.at(i).c_str());
                }

                std::string tmpHistName = Form("h1D_%s", tmpName.c_str());

                // read histograms
                std::cout<<"reading histogram : "<< tmpHistName.c_str() <<std::endl;
                for (int iFile = 0; iFile < nInputFiles; ++iFile){
                    if (inputExists[iFile])
                        inputDir[iFile]->GetObject(tmpHistName.c_str(), h1D[iFile][i]);

                    h1DisValid[iFile][i] = false;
                    if (h1D[iFile][i])  h1DisValid[iFile][i] = true;
                }

                int zPt = (int)bins_pt.at(iPt);
                fncNominalHasbeenFit[i] = false;
                std::string tmpName_uncTot = Form("%s_uncTot", tmpName.c_str());
                std::string tmpHistName_uncTot = Form("h1D_%s", tmpName_uncTot.c_str());
                std::string tmpHistName_uncTot_Fit = Form("h1D_%s_Fit", tmpName_uncTot.c_str());
                h1D[0][i]->SetTitle("");
                h1D[0][i]->Rebin(rebins.at(i));

                h1D_uncTot_diff[i] = (TH1D*)h1D[0][i]->Clone(tmpHistName_uncTot.c_str());
                h1D_uncTot_diff[i]->Reset("ICES");
                h1D_uncTot_diff[i]->SetYTitle("");
                h1D_unctTot_Fit[i] = (TH1D*)h1D_uncTot_diff[i]->Clone(tmpHistName_uncTot_Fit.c_str());

                h1D_uncTot_ratio[i] = (TH1D*)h1D[0][i]->Clone(Form("%s_ratio", tmpHistName_uncTot.c_str()));
                h1D_uncTot_ratio[i]->Reset("ICES");
                h1D_uncTot_ratio[i]->SetYTitle("");
                // calculate the ratio of the results.
                // nominal histogram is always the denominator.
                for (int iFile = 1; iFile < nInputFiles; ++iFile){
                    if (!h1DisValid[0][i]) continue;
                    if (!h1DisValid[iFile][i]) continue;

                    h1D[iFile][i]->SetTitle("");
                    h1D[iFile][i]->Rebin(rebins.at(i));

                    std::string tmpName_diff = Form("%s_diff_%d", tmpName.c_str(), iFile);
                    std::string tmpHistName_diff = Form("h1D_%s", tmpName_diff.c_str());
                    h1D_diff[iFile][i] = (TH1D*)h1D[0][i]->Clone(tmpHistName_diff.c_str());
                    h1D_diff[iFile][i]->SetYTitle("Nominal - Varied");
                    h1D_diff[iFile][i]->SetTitleOffset(2, "Y");
                    h1D_diff[iFile][i]->Add(h1D[iFile][i], -1);

                    std::string tmpName_ratio = Form("%s_ratio_%d", tmpName.c_str(), iFile);
                    std::string tmpHistName_ratio = Form("h1D_%s", tmpName_ratio.c_str());

                    h1D_ratio[iFile][i] = (TH1D*)h1D[iFile][i]->Clone(tmpHistName_ratio.c_str());
                    if (!calcUncfromFit.at(i)) {
                        // calculate the ratios of the histograms.
                        calcTH1Ratio4SysUnc(h1D_ratio[iFile][i], h1D[0][i]);

                        h1D_ratio[iFile][i]->SetYTitle("Ratio");
                        h1D_ratio[iFile][i]->SetTitleOffset(2, "Y");
                    }
                    else {
                        // initialize fit function
                        std::string tmpFncName = Form("fnc_%s_%d", tmpName.c_str(), iFile);
                        int binFirst = h1D[iFile][i]->GetXaxis()->GetFirst();
                        int binLast  = h1D[iFile][i]->GetXaxis()->GetLast();
                        double TF1_xMin = h1D[iFile][i]->GetXaxis()->GetBinLowEdge(binFirst);
                        double TF1_xMax = h1D[iFile][i]->GetXaxis()->GetBinLowEdge(binLast+1);
                        // special cases
                        if (correlation.compare("xjz") == 0) {
                            TF1_xMin = 0.2;
                        }
                        if (correlation.compare("ptJet") == 0) {
                            TF1_xMin = jetpt;
                        }
                        else if (correlation.compare(diLeptonPt.c_str()) == 0) {
                            TF1_xMin = zPt;
                        }
                        fnc[iFile][i] = new TF1(tmpFncName.c_str(), functionFormulas.at(i).c_str(), TF1_xMin, TF1_xMax);
                        if (correlation.compare("xjz") == 0) {
                            fnc[iFile][i]->SetParameter(3, h1D[iFile][i]->GetMean());
                        }

                        // do fit
                        int fitColor = EColor::kRed;
                        fnc[iFile][i]->SetLineColor(fitColor);
                        h1D[iFile][i]->Fit(fnc[iFile][i]->GetName(), fitOptions.at(i).c_str());

                        if (!fncNominalHasbeenFit[i]) {
                            fnc[0][i] = new TF1(tmpFncName.c_str(), functionFormulas.at(i).c_str(), TF1_xMin, TF1_xMax);
                            if (correlation.compare("xjz") == 0) {
                                fnc[0][i]->SetParameter(3, h1D[0][i]->GetMean());
                            }

                            fnc[0][i]->SetLineColor(fitColor);
                            h1D[0][i]->Fit(fnc[0][i]->GetName(), fitOptions.at(i).c_str());

                            fnc[0][i]->Write(Form("fnc_%s_0", tmpName.c_str()),TObject::kOverwrite);
                            h1D[0][i]->Write(Form("%s_0", tmpHistName.c_str()),TObject::kOverwrite);

                            fncNominalHasbeenFit[i] = true;
                        }
                        // store only the histograms whose uncertainty is calculated by a fit
                        fnc[iFile][i]->Write(tmpFncName.c_str(), TObject::kOverwrite);
                        h1D[iFile][i]->Write(Form("%s_%d", tmpHistName.c_str(), iFile),TObject::kOverwrite);

                        // both nominal and varied histograms are fit at this point.
                        // now calculate ratios of the functions.
                        calcTF1Ratio4SysUnc(h1D_ratio[iFile][i], fnc[0][i], fnc[iFile][i]);

                        h1D_ratio[iFile][i]->SetYTitle("Ratio");
                    }
                    // SYS UNC that is calculated from differences
                    // this will be the SYS UNC. calculation of the analysis
                    std::string tmpName_iFile = Form("%s_%d", tmpName.c_str(), iFile);
                    std::string tmpName_unc_diff = Form("%s_unc_%d", tmpName.c_str(), iFile);
                    std::string tmpHistName_unc_diff = Form("h1D_%s", tmpName_unc_diff.c_str());
                    h1D_unc_diff[iFile][i] = (TH1D*)h1D_diff[iFile][i]->Clone(tmpHistName_unc_diff.c_str());
                    h1D_unc_diff[iFile][i]->SetYTitle("");
                    h1D_unc_diff[iFile][i]->Scale(1./rebins.at(i));
                    addSysUnc(h1D_uncTot_diff[i], h1D_unc_diff[iFile][i]);

                    // SYS UNC that is calculated from ratios
                    // this will NOT be the SYS UNC. calculation of the analysis
                    std::string tmpName_unc_ratio = Form("%s_unc_ratio_%d", tmpName.c_str(), iFile);
                    std::string tmpHistName_unc_ratio = Form("h1D_%s", tmpName_unc_ratio.c_str());
                    h1D_unc_ratio[iFile][i] = (TH1D*)h1D_ratio[iFile][i]->Clone(tmpHistName_unc_ratio.c_str());
                    h1D_unc_ratio[iFile][i]->SetYTitle("");
                    h1D_unc_ratio[iFile][i]->Scale(1./rebins.at(i));
                    subtractIdentity4SysUnc(h1D_unc_ratio[iFile][i]);
                    addSysUnc(h1D_uncTot_ratio[i], h1D_unc_ratio[iFile][i]);

                    for (int iCanvas = 0; iCanvas < 6; ++iCanvas)
                    {
                        if (iCanvas == 5 && iFile < nInputFiles-1)  continue;

                        std::string tmpCnvName = "";
                        if (iCanvas == 0) {         // draw ratio
                            tmpCnvName = Form("cnv_%s", tmpName_iFile.c_str());
                        }
                        if (iCanvas == 1) {         // draw ratio
                            tmpCnvName = Form("cnv_%s", tmpName_diff.c_str());
                        }
                        else if (iCanvas == 2) {    // draw uncertainty
                            tmpCnvName = Form("cnv_%s", tmpName_unc_diff.c_str());
                        }
                        else if (iCanvas == 3) {         // draw ratio
                            tmpCnvName = Form("cnv_%s", tmpName_ratio.c_str());
                        }
                        else if (iCanvas == 4) {    // draw uncertainty
                            tmpCnvName = Form("cnv_%s", tmpName_unc_ratio.c_str());
                        }
                        else if (iCanvas == 5 && iFile == nInputFiles-1) {    // draw total uncertainty after all individual uncertainties are calculated.
                            tmpCnvName = Form("cnv_%s", tmpName_uncTot.c_str());
                        }
                        c = new TCanvas("cnv","",windowWidth,windowHeight);
                        c->SetName(tmpCnvName.c_str());
                        std::cout<<"preparing canvas : "<< c->GetName() <<std::endl;

                        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
                        setCanvasFinal(c, setLogx, setLogy);
                        c->cd();

                        double xmin = -1;
                        double xmax = 1;
                        double yHorizontal = -1;
                        if (iCanvas == 0) {         // draw nominal and varied histograms
                            h1D[iFile][i]->Draw("e same");
                            // h1D[iFile][i]->Write("",TObject::kOverwrite); // they were already written.
                        }
                        if (iCanvas == 1) {         // draw ratio
                            h1D_diff[iFile][i]->Draw("e same");
                            h1D_diff[iFile][i]->Write("",TObject::kOverwrite);

                            // xmin = h1D_ratio[iFile][i]->GetXaxis()->GetBinLowEdge(h1D_ratio[iFile][i]->GetXaxis()->GetFirst());
                            // xmax = h1D_ratio[iFile][i]->GetXaxis()->GetBinLowEdge(h1D_ratio[iFile][i]->GetXaxis()->GetLast()+1);
                            yHorizontal = 1;
                        }
                        else if (iCanvas == 2) {    // draw uncertainty
                            h1D_unc_diff[iFile][i]->Write("",TObject::kOverwrite);

                            h1D_unc_diff[iFile][i]->SetMinimum(-2);
                            h1D_unc_diff[iFile][i]->SetMaximum(2);

                            h1D_unc_diff[iFile][i]->Draw("e same");

                            xmin = h1D_unc_diff[iFile][i]->GetXaxis()->GetBinLowEdge(h1D_unc_diff[iFile][i]->GetXaxis()->GetFirst());
                            xmax = h1D_unc_diff[iFile][i]->GetXaxis()->GetBinLowEdge(h1D_unc_diff[iFile][i]->GetXaxis()->GetLast()+1);
                            yHorizontal = 0;
                        }
                        else if (iCanvas == 3) {         // draw ratio
                            h1D_ratio[iFile][i]->Draw("e same");
                            h1D_ratio[iFile][i]->Write("",TObject::kOverwrite);

                            // xmin = h1D_ratio[iFile][i]->GetXaxis()->GetBinLowEdge(h1D_ratio[iFile][i]->GetXaxis()->GetFirst());
                            // xmax = h1D_ratio[iFile][i]->GetXaxis()->GetBinLowEdge(h1D_ratio[iFile][i]->GetXaxis()->GetLast()+1);
                            yHorizontal = 1;
                        }
                        else if (iCanvas == 4) {    // draw uncertainty
                            h1D_unc_ratio[iFile][i]->Write("",TObject::kOverwrite);

                            h1D_unc_ratio[iFile][i]->SetMinimum(-2);
                            h1D_unc_ratio[iFile][i]->SetMaximum(2);

                            h1D_unc_ratio[iFile][i]->Draw("e same");

                            xmin = h1D_unc_ratio[iFile][i]->GetXaxis()->GetBinLowEdge(h1D_unc_ratio[iFile][i]->GetXaxis()->GetFirst());
                            xmax = h1D_unc_ratio[iFile][i]->GetXaxis()->GetBinLowEdge(h1D_unc_ratio[iFile][i]->GetXaxis()->GetLast()+1);
                            yHorizontal = 0;
                        }
                        else if (iCanvas == 5 && iFile == nInputFiles-1) {    // fit and draw total uncertainty after all individual uncertainties are calculated.

                            // initialize fit function
                            std::string tmpFncName_uncTotLinear = Form("fnc_%s_uncTot_%d", tmpName.c_str(), iFile);
                            int binFirst = h1D_uncTot_diff[i]->GetXaxis()->GetFirst();
                            int binLast  = h1D_uncTot_diff[i]->GetXaxis()->GetLast();
                            double TF1_xMin = h1D_uncTot_diff[i]->GetXaxis()->GetBinLowEdge(binFirst);
                            double TF1_xMax = h1D_uncTot_diff[i]->GetXaxis()->GetBinLowEdge(binLast+1);
                            if (correlation.compare("xjz") == 0) {
                                TF1_xMin = 0.2;
                                TF1_xMax = 1.2;
                            }
                            else if (correlation.compare("dphi") == 0) {
                                TF1_xMin = 1.5;
                            }
                            else if (correlation.compare("ptJet") == 0) {
                                TF1_xMin = jetpt;
                            }
                            else if (correlation.compare(diLeptonPt.c_str()) == 0) {
                                TF1_xMin = zPt;
                            }
                            fnc_uncTot[i] = new TF1(tmpFncName_uncTotLinear.c_str(), functionFormulas_uncTot.at(i).c_str() , TF1_xMin, TF1_xMax);

                            // fit the total uncertainty with a linear function
                            int fitColor = EColor::kRed;
                            fnc_uncTot[i]->SetLineColor(fitColor);
                            if (functionFormulas_uncTot.at(i).compare("NULL") != 0)  h1D_uncTot_diff[i]->Fit(fnc_uncTot[i]->GetName(), "chi2 R M");
                            fnc_uncTot[i]->Write(tmpFncName_uncTotLinear.c_str(), TObject::kOverwrite);

                            fillTH1fromTF1(h1D_unctTot_Fit[i], fnc_uncTot[i]);
                            h1D_unctTot_Fit[i]->Write("",TObject::kOverwrite);

                            h1D_uncTot_diff[i]->SetMinimum(0);
                            h1D_uncTot_diff[i]->SetMaximum(3);

                            h1D_uncTot_diff[i]->Draw("e same");
                            h1D_uncTot_diff[i]->Write("",TObject::kOverwrite);

                            // xmin = h1D_uncTot[i]->GetXaxis()->GetBinLowEdge(h1D_uncTot[i]->GetXaxis()->GetFirst());
                            // xmax = h1D_uncTot[i]->GetXaxis()->GetBinLowEdge(h1D_uncTot[i]->GetXaxis()->GetLast()+1);
                            yHorizontal = 0;
                        }

                        // draw a horizontal line
                        xmin = h1D_diff[iFile][i]->GetXaxis()->GetBinLowEdge(h1D_diff[iFile][i]->GetXaxis()->GetFirst());
                        xmax = h1D_diff[iFile][i]->GetXaxis()->GetBinLowEdge(h1D_diff[iFile][i]->GetXaxis()->GetLast()+1);
                        TLine line_horizontal(xmin, yHorizontal, xmax, yHorizontal);
                        line_horizontal.SetLineStyle(kDashed);
                        if (iCanvas > 0)  line_horizontal.Draw();

                        int cent_low = (int)(bins_hiBin[0].at(iHiBin)/2);
                        int cent_up  = (int)(bins_hiBin[1].at(iHiBin)/2);
                        // put info onto the canvas
                        std::vector<std::string> textLines;
                        std::string collisionText;
                        if (isHI)  collisionText = Form("PbPb, %d-%d %%", cent_low, cent_up);
                        else       collisionText = "pp";
                        textLines.push_back(collisionText.c_str());
                        textLines.push_back(Form("p_{T}^{Z} > %d GeV/c", zPt));
                        textLines.push_back(Form("%d < M^{%s%s} < %d GeV/c^{2}", (int)massMin, leptonSymbol.c_str(), leptonSymbol.c_str(), (int)massMax));
                        textLines.push_back(Form("p^{%s#pm}_{T} > %d GeV/c, |#eta^{%s#pm}| < %.1f  ", leptonSymbol.c_str(), (int)leptonPt,
                                 leptonSymbol.c_str(), leptonEta));
                        textLines.push_back(Form("p_{T}^{Jet} > %d GeV/c,  |#eta^{Jet}| < %.1f", (int)jetpt, jeteta));
                        if (jetCollection.size() > 0)  textLines.push_back(jetCollection.c_str());
                        if (textWriteDphi.at(i))  {
                            if (awayRange > 0) {
                                // awayRange = 78 means dphi > 7/8 PI
                                std::string tmp_dphi = Form("#Delta#phi_{JZ} > #frac{%d}{%d}#pi", int(awayRange/10), int(awayRange)%10);
                                textLines.push_back(tmp_dphi.c_str());
                            }
                        }
                        int nTextLines = textLines.size();
                        std::string textPosition = textPositions.at(i).c_str();

                        TLatex* latex;
                        if (nTextLines > 0) {
                            latex = new TLatex();
                            latex->SetTextFont(textFont);
                            latex->SetTextSize(textSize);
                            setTextAlignment(latex, textPosition.c_str());
                            std::vector<std::pair<float,float>> textCoordinates = calcTextCoordinates(textLines, textPosition.c_str(), c, textOffsetX, textOffsetY);
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

                        c->Write("",TObject::kOverwrite);
                        std::string outputFigureName = c->GetName();
                        if (!outputFigurePrefix.EqualTo("")) {
                            outputFigureName = Form("%s%s", outputFigurePrefix.Data(), c->GetName());
                        }
                        c->SaveAs(Form("%s.png", outputFigureName.c_str()));
                         c->SaveAs(Form("%s.pdf", outputFigureName.c_str()));
                        // c->SaveAs(Form("%s.C",   outputFigureName.c_str()));

                        c->Close();         // do not use Delete() for TCanvas.
                        std::cout<<"##########"<<std::endl;
                    }
                }
            }
        }
    }

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
        zJetSystematics(argv[1], argv[2], argv[3], argv[4]);
        return 0;
    }
    else if (argc == 4) {
        zJetSystematics(argv[1], argv[2], argv[3]);
        return 0;
    }
    else if (argc == 3) {
        zJetSystematics(argv[1], argv[2]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./zJetSystematics.exe <configFile> <inputFile> <outputFile>"
                << std::endl;
        return 1;
    }
}
