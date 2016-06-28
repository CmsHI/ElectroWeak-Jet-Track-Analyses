#include <TFile.h>
#include <TDirectoryFile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TH1D.h>
#include <TF1.h>
#include <TFormula.h>
#include <TLine.h>
#include <TLatex.h>

#include <vector>
#include <string>
#include <iostream>

#include "../interface/systematicsHist.h"
#include "../../TreeHeaders/CutConfigurationTree.h"
#include "../../Utilities/interface/CutConfigurationParser.h"
#include "../../Utilities/interface/InputConfigurationParser.h"
#include "../../Utilities/eventUtil.h"
#include "../../Utilities/styleUtil.h"
#include "../../Utilities/systemUtil.h"
#include "../../Utilities/th1Util.h"
#include "../../Utilities/bosonJetUtil.h"

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
    bool isPP = collisionIsPP((COLL::TYPE)collision);

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

    std::vector<std::string> correlationHistNames  {"xjz", "dphi_rebin", "ptJet", diLeptonM.c_str(), diLeptonPt.c_str(), "rjz", "xjz_mean", "rjz_zNum"};

    std::vector<std::string> versionSuffix         {"final_norm", "final_norm", "final_norm", "final", "final_norm","", "", ""};
    std::vector<std::string> jetRegion {"SIG", "SIG", "SIG", "RAW", "RAW", "", "SIG", ""};
    std::vector<std::string> legendPositions {"NE", "NW", "NE", "NW", "NE", "NW", "NE", "NW"};
    std::vector<std::string> textPositions   {"NE", "NW", "NE", "NW", "NE", "NE", "NE", "SE"};
    std::vector<bool> textWriteDphi   {true, false, true, true, true, true, true, true};
    std::vector<std::string> functionFormulas {"([0]+[1]*x)*exp(-[2]*(x-[3]))*(x-[3])", "[0]*exp(-(TMath::Pi()-x)*[1])+[2]", "([0]+[1]*x)*exp(-[2]*(x-[3]))*(x-[3])", "NULL", "([0]+[1]*x)*exp(-[2]*(x-[3]))*(x-[3])", "NULL", "NULL", "NULL"};
    std::vector<std::string> fitOptions {"chi2 R M", "chi2 R M", "chi2 R M", "chi2 R M", "chi2 R M", "chi2 R M", "chi2 R M", "chi2 R M"};
    std::vector<bool>        calcUncfromFit {false, false, false, false, false, false, false, false};
    std::vector<std::string> functionFormulas_uncTot {"pol0", "pol0", "pol2", "pol1", "pol2", "pol1", "pol0", "pol1"};
    std::vector<int>         rebins      {2, 1, 2, 1, 2, 1, 1, 1};

    int nCorrHist = correlationHistNames.size();
    std::vector<bool> vecFalse  (nCorrHist, false);

    std::vector<bool> do_SYS_UNC = vecFalse;
    // select the list of observables for which the SYS. UNC. will be studied.
    std::vector<std::string> correlationHistNamesTMP = {"xjz", "dphi_rebin", "rjz", "xjz_mean"};
    int sizeTMP = correlationHistNamesTMP.size();
    for (int i = 0; i < sizeTMP; ++i) {

        int iTmp = findPositionInVector(correlationHistNames, correlationHistNamesTMP.at(i).c_str());
        if (iTmp > -1)  do_SYS_UNC.at(iTmp) = true;
    }

    // declaration of graphics objects.
    // the idea is to initialize each Graphics object right before its use, then to delete it right after they are saved/written.
    TCanvas* c;

    TH1::SetDefaultSumw2();
    systematicsHist sysHist[nInputFiles][nCorrHist];

    //TH1D* h1D[nInputFiles][nCorrHist];
    // TH1D* h1D_ratio[nInputFiles][nCorrHist];    // h1D_ratio = h1D[0][] / h1D[i][]
    // TH1D* h1D_diff[nInputFiles][nCorrHist];     // h1D_diff  = h1D[0][] - h1D[i][]
    //TH1D* h1D_unc_ratio[nInputFiles][nCorrHist];      // h1D_unc = h1D_ratio - 1, SYS UNC. calculated from ratios
    //TH1D* h1D_uncTot_ratio[nCorrHist];                // h1D_uncTot = sqrt( sum (h1D_unc^2) )
    // TH1D* h1D_unc_diff[nInputFiles][nCorrHist];      // h1D_unc = h1D_diff, SYS UNC. calculated from differences
    // TH1D* h1D_unc_diff_abs[nInputFiles][nCorrHist];      // h1D_unc_diff_abs = abs(h1D_unc_diff)
    // TH1D* h1D_unc_diff_Final[nInputFiles][nCorrHist];
    // bool  h1D_unc_diff_Final_isSet[nInputFiles][nCorrHist];
    // TH1D* h1D_uncTot_diff[nCorrHist];                // h1D_uncTot = sqrt( sum (h1D_unc^2) )

    TF1*  fnc[nInputFiles][nCorrHist];
    bool  fncNominalHasbeenFit[nCorrHist];     // once a nominal distribution is fit do not fit it again in the next iteration over iFile.

    TF1*  fnc_uncTot[nCorrHist];          // polynomial fit to final total uncertainty

    for (int i=0; i<nCorrHist; ++i){
        if (!do_SYS_UNC.at(i))  continue;

        for (int iPt=0; iPt < nBins_pt; ++iPt){
            int zPt = (int)bins_pt.at(iPt);

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

                    if (inputExists[iFile]) {
                        sysHist[iFile][i].name = tmpName.c_str();
                        sysHist[iFile][i].observable = correlation.c_str();

                        inputDir[iFile]->GetObject(tmpHistName.c_str(), sysHist[iFile][i].h1D_varied);
                    }

                    sysHist[iFile][i].isValid = false;
                    if (sysHist[iFile][i].h1D_varied)  {
                        sysHist[iFile][i].isValid = true;

                        // variable size binning for SYS UNC calculation
                        std::vector<double> tmpBins = getTH1xBins(sysHist[iFile][i].h1D_varied);

                        if (correlation.compare("xjz") == 0) {
                            if (isPP) tmpBins = rebinXjBinsPP(tmpBins);
                            else tmpBins = rebinXjBins(tmpBins);
                        }
                        else if (rebins.at(i) > 1) {
                            TH1D* htmp = (TH1D*)sysHist[iFile][i].h1D_varied->Rebin(rebins.at(i), Form("%s_%d_tmpRebin", tmpHistName.c_str(), iFile));
                            tmpBins = getTH1xBins(htmp);
                        }
                        int NtmpBins = tmpBins.size();
                        double binsRebin[NtmpBins];
                        std::copy(tmpBins.begin(), tmpBins.end(), binsRebin);

                        sysHist[iFile][i].h1D_varied = (TH1D*)sysHist[iFile][i].h1D_varied->Rebin(NtmpBins-1, Form("%s_%d", tmpHistName.c_str(), iFile), binsRebin);    //creates a new variable bin size histogram hnew
                        sysHist[iFile][i].h1D_varied->SetTitle("");
                    }
                }
                sysHist[0][i].h1D_nominal = (TH1D*)sysHist[0][i].h1D_varied->Clone(Form("%s_nominal", tmpHistName.c_str()));

                fncNominalHasbeenFit[i] = false;
                std::string tmpName_uncTot = Form("%s_uncTot", tmpName.c_str());
                std::string tmpHistName_uncTot = Form("h1D_%s", tmpName_uncTot.c_str());

                // sysHist[0][i] will contain the total SYS. UNC.
                // sysHist[0][i].h1D_diff  = sqrt( sum over j : (sysHist[j][i].h1D_diff)^2 )
                // sysHist[0][i].h1D_ratio = sqrt( sum over j : (sysHist[j][i].h1D_ratio)^2 )

                std::string tmpName_uncTot_diff = Form("%s_diff", tmpName_uncTot.c_str());
                std::string tmpHistName_uncTot_diff = Form("h1D_%s", tmpName_uncTot_diff.c_str());
                sysHist[0][i].h1D_diff = (TH1D*)sysHist[0][i].h1D_varied->Clone(tmpHistName_uncTot_diff.c_str());

                std::string tmpName_uncTot_ratio = Form("%s_ratio", tmpName_uncTot.c_str());
                std::string tmpHistName_uncTot_ratio = Form("h1D_%s", tmpName_uncTot_ratio.c_str());
                sysHist[0][i].h1D_ratio = (TH1D*)sysHist[0][i].h1D_varied->Clone(tmpHistName_uncTot_ratio.c_str());

                sysHist[0][i].h1D_diff->Reset("ICES");
                // set TH1 style
                std::string titleY_h1D_diff = "Difference = Nominal - Varied";
                std::string titleY_h1D_diff_rel = "Rel. Difference = (Nominal - Varied) / Nominal";
                sysHist[0][i].h1D_diff->SetYTitle(titleY_h1D_diff.c_str());
                setTH1Style4SysUnc(sysHist[0][i].h1D_diff);
                // set TH1 style - END
                std::string tmpName_uncTot_diff_rel = Form("%s_diff_rel", tmpName_uncTot.c_str());
                std::string tmpHistName_uncTot_diff_rel = Form("h1D_%s", tmpName_uncTot_diff_rel.c_str());
                sysHist[0][i].h1D_diff_rel  = (TH1D*)sysHist[0][i].h1D_diff->Clone(tmpHistName_uncTot_diff_rel.c_str());
                sysHist[0][i].h1D_diff_rel->SetYTitle(titleY_h1D_diff_rel.c_str());
                sysHist[0][i].h1D_diff_rel->SetMinimum(0);
                sysHist[0][i].h1D_diff_rel->SetMaximum(2);

                sysHist[0][i].h1D_ratio->Reset("ICES");
                // set TH1 style
                std::string titleY_h1D_ratio = "Ratio = Varied / Nominal";
                std::string titleY_h1D_ratio_rel = "Rel. Ratio = (Varied - Nominal) / Nominal";
                sysHist[0][i].h1D_ratio->SetYTitle(titleY_h1D_ratio.c_str());
                setTH1Style4SysUnc(sysHist[0][i].h1D_ratio);
                // set TH1 style - END
                std::string tmpName_uncTot_ratio_rel = Form("%s_ratio_rel", tmpName_uncTot.c_str());
                std::string tmpHistName_uncTot_ratio_rel = Form("h1D_%s", tmpName_uncTot_ratio_rel.c_str());
                sysHist[0][i].h1D_ratio_rel = (TH1D*)sysHist[0][i].h1D_ratio->Clone(tmpHistName_uncTot_ratio_rel.c_str());
                sysHist[0][i].h1D_ratio_rel->SetYTitle(titleY_h1D_ratio_rel.c_str());
                sysHist[0][i].h1D_ratio_rel->SetMinimum(0);
                sysHist[0][i].h1D_ratio_rel->SetMaximum(2);

                for (int j = 0; j < SYS::kN_SYSPOLFNC; ++j) {

                    // histograms made by bin-by-bin evaluation of fnc_pol
                    std::string tmpName_fnc_pol = Form("fnc_%s_diff_pol_%d", tmpName_uncTot.c_str(), j);
                    std::string tmpHistName_fnc_pol = Form("h1D_%s", tmpName_fnc_pol.c_str());
                    sysHist[0][i].h1D_fnc_pol[j] = (TH1D*)sysHist[0][i].h1D_diff->Clone(tmpHistName_fnc_pol.c_str());
                    sysHist[0][i].h1D_fnc_pol[j]->SetMarkerColor(SYS::POLFNC_COLORS[j]);

                    std::string tmpName_fnc_pol_rel = Form("fnc_%s_diff_pol_%d_rel", tmpName_uncTot.c_str(), j);
                    std::string tmpHistName_fnc_pol_rel = Form("h1D_%s", tmpName_fnc_pol_rel.c_str());
                    sysHist[0][i].h1D_fnc_pol_rel[j] = (TH1D*)sysHist[0][i].h1D_diff->Clone(tmpHistName_fnc_pol_rel.c_str());
                    sysHist[0][i].h1D_fnc_pol_rel[j]->SetMarkerColor(SYS::POLFNC_COLORS[j]);
                    sysHist[0][i].h1D_fnc_pol_rel[j]->SetMarkerStyle(SYS::POLFNC_MARKERSTYLES[j]);
                }
                // calculate the ratio of the results.
                // nominal histogram is always the denominator.

                // the total uncertainty function = sqrt (f1*f1 + f2*f2 + ...)
                // where f1 is the fit to the first SYS. UNC. source.
                // that function will be stored in the "sysHist[0][i]"
                for (int j = 0; j < SYS::kN_SYSPOLFNC; ++j) {

                    double xmin = sysHist[0][i].h1D_diff->GetXaxis()->GetBinLowEdge(sysHist[0][i].h1D_diff->GetXaxis()->GetFirst());
                    double xmax = sysHist[0][i].h1D_diff->GetXaxis()->GetBinLowEdge(sysHist[0][i].h1D_diff->GetXaxis()->GetLast()+1);
                    std::string fnc_pol_name = Form("fnc_%s_pol_%d", tmpName_uncTot.c_str(), j);

                    sysHist[0][i].fnc_pol_formula[j] = "";
                    sysHist[0][i].fnc_pol[j] = new TF1(fnc_pol_name.c_str() , sysHist[0][i].fnc_pol_formula[j].c_str(),
                            xmin, xmax);
                    sysHist[0][i].fnc_pol[j]->SetLineColor(SYS::POLFNC_COLORS[j]);

                    // this function is never fit. It is the sqrt of the sum of squares of functions which are fit.
                    sysHist[0][i].isFit_fnc_pol[j] = false;
                }

                // LOOP over SYSTEMATICS
                TH1D* h1D_temp = 0;
                for (int iFile = 1; iFile < nInputFiles; ++iFile){

                    if (!sysHist[0][i].isValid) continue;
                    if (!sysHist[iFile][i].isValid) continue;

                    // binning of h1D_diff[0][i] was already set before this point.
                    std::vector<double> tmpBins = getTH1xBins(sysHist[0][i].h1D_diff );

                    int NtmpBins = tmpBins.size();
                    double binsRebin[NtmpBins];
                    std::copy(tmpBins.begin(), tmpBins.end(), binsRebin);

                    h1D_temp = (TH1D*)sysHist[iFile][i].h1D_varied->Clone(Form("%s_%d_temp", tmpHistName.c_str(), iFile));

                    std::string tmpName_nominal = Form("%s_nominal_%d", tmpName.c_str(), iFile);
                    std::string tmpHistName_nominal = Form("h1D_%s", tmpName_nominal.c_str());
                    sysHist[iFile][i].h1D_nominal = (TH1D*)sysHist[0][i].h1D_nominal->Clone(tmpHistName_nominal.c_str());
                    std::string tmpName_diff = Form("%s_diff_%d", tmpName.c_str(), iFile);
                    std::string tmpHistName_diff = Form("h1D_%s", tmpName_diff.c_str());
                    sysHist[iFile][i].h1D_diff = (TH1D*)sysHist[iFile][i].h1D_nominal->Clone(tmpHistName_diff.c_str());
                    sysHist[iFile][i].h1D_diff->Add(h1D_temp, -1);
                    // set TH1 style
                    sysHist[iFile][i].h1D_diff->SetYTitle(titleY_h1D_diff.c_str());
                    setTH1Style4SysUnc(sysHist[iFile][i].h1D_diff);
                    // set TH1 style - END

                    // prepare names for relative difference
                    std::string tmpName_diff_rel = Form("%s_diff_rel_%d", tmpName.c_str(), iFile);
                    std::string tmpHistName_diff_rel = Form("h1D_%s", tmpName_diff_rel.c_str());

                    // initial version of h1D_diff  before scaling, abs(), rebinning etc. operations
                    std::string tmpHistName_diff_v1 = Form("%s_v1", sysHist[iFile][i].h1D_diff->GetName());
                    sysHist[iFile][i].h1D_diff_v1 = (TH1D*)sysHist[iFile][i].h1D_diff->Clone(tmpHistName_diff_v1.c_str());

                    std::string tmpName_ratio = Form("%s_ratio_%d", tmpName.c_str(), iFile);
                    std::string tmpHistName_ratio = Form("h1D_%s", tmpName_ratio.c_str());
                    sysHist[iFile][i].h1D_ratio = (TH1D*)h1D_temp->Clone(tmpHistName_ratio.c_str());
                    // calculate the ratios of the histograms.
                    calcTH1Ratio4SysUnc(sysHist[iFile][i].h1D_ratio, sysHist[0][i].h1D_ratio);
                    // set TH1 style
                    sysHist[iFile][i].h1D_ratio->SetYTitle(titleY_h1D_ratio.c_str());
                    setTH1Style4SysUnc(sysHist[iFile][i].h1D_ratio);
                    // set TH1 style - END

                    // prepare names for relative ratio
                    std::string tmpName_ratio_rel = Form("%s_ratio_rel_%d", tmpName.c_str(), iFile);
                    std::string tmpHistName_ratio_rel = Form("h1D_%s", tmpName_ratio_rel.c_str());

                    std::string tmpHistName_ratio_v1 = Form("%s_v1", sysHist[iFile][i].h1D_ratio->GetName());
                    sysHist[iFile][i].h1D_ratio_v1 = (TH1D*)sysHist[iFile][i].h1D_ratio->Clone(tmpHistName_ratio_v1.c_str());

                    if (calcUncfromFit.at(i)) {
                        // initialize fit function
                        std::string tmpFncName = Form("fnc_%s_%d", tmpName.c_str(), iFile);
                        int binFirst = sysHist[iFile][i].h1D_varied->GetXaxis()->GetFirst();
                        int binLast  = sysHist[iFile][i].h1D_varied->GetXaxis()->GetLast();
                        double TF1_xMin = sysHist[iFile][i].h1D_varied->GetXaxis()->GetBinLowEdge(binFirst);
                        double TF1_xMax = sysHist[iFile][i].h1D_varied->GetXaxis()->GetBinLowEdge(binLast+1);
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
                            fnc[iFile][i]->SetParameter(3, sysHist[iFile][i].h1D_varied->GetMean());
                        }

                        // do fit
                        int fitColor = EColor::kRed;
                        fnc[iFile][i]->SetLineColor(fitColor);
                        sysHist[iFile][i].h1D_varied->Fit(fnc[iFile][i]->GetName(), fitOptions.at(i).c_str());

                        if (!fncNominalHasbeenFit[i]) {
                            fnc[0][i] = new TF1(tmpFncName.c_str(), functionFormulas.at(i).c_str(), TF1_xMin, TF1_xMax);
                            if (correlation.compare("xjz") == 0) {
                                fnc[0][i]->SetParameter(3, sysHist[0][i].h1D_varied->GetMean());
                            }

                            fnc[0][i]->SetLineColor(fitColor);
                            sysHist[0][i].h1D_varied->Fit(fnc[0][i]->GetName(), fitOptions.at(i).c_str());

                            fnc[0][i]->Write(Form("fnc_%s_0", tmpName.c_str()),TObject::kOverwrite);
                            sysHist[0][i].h1D_varied->Write(Form("%s_0", tmpHistName.c_str()),TObject::kOverwrite);

                            fncNominalHasbeenFit[i] = true;
                        }
                        // store only the histograms whose uncertainty is calculated by a fit
                        fnc[iFile][i]->Write(tmpFncName.c_str(), TObject::kOverwrite);
                        sysHist[iFile][i].h1D_varied->Write(Form("%s_%d", tmpHistName.c_str(), iFile),TObject::kOverwrite);

                        // both nominal and varied histograms are fit at this point.
                        // now calculate ratios of the functions.
                        calcTF1Ratio4SysUnc(sysHist[iFile][i].h1D_ratio, fnc[0][i], fnc[iFile][i]);

                        sysHist[iFile][i].h1D_ratio->SetYTitle("Ratio");
                    }

                    // SYS UNC that is calculated from differences
                    // this will be the SYS UNC. calculation of the analysis
                    if (rebins.at(i) != 1)      // do scaling only if the histogram is rebinned.
                    {
                        sysHist[iFile][i].h1D_diff->Scale(1, "width");
                        sysHist[iFile][i].h1D_diff->Scale(sysHist[iFile][i].h1D_varied->GetBinWidth(1));   // assumes the original histogram has uniform binning.
                    }

                    // SYS UNC that is calculated from ratios
                    // this will NOT be the SYS UNC. calculation of the analysis
                    if (rebins.at(i) != 1)      // do scaling only if the histogram is rebinned.
                    {
                        sysHist[iFile][i].h1D_ratio->Scale(1, "width");
                        sysHist[iFile][i].h1D_ratio->Scale(sysHist[iFile][i].h1D_varied->GetBinWidth(1));
                    }

                    bool tmpSysValid = false;
                    if (isHI)
                    {
                        // take the maximum of uncertainties : SYS_jes = max(jes106, jes096), iFile = 1,2
                        // take the maximum of uncertainties : SYS_ees = max(eesPlus, eesMinus), iFile = 3,4
                        // take the maximum of uncertainties : SYS_smearResJet = max(SYS_smearResJetPlus, SYS_smearResJetMinus), iFile = 5,6
                        // iFile = 1,2 : jet energy scale
                        // iFile = 3,4 : electron energy scale
                        // iFile = 5,6 : jet energy resolution
                        // iFile = 7   : Z pt resolution
                        // iFile = 8   : centrality reweighting
                        tmpSysValid = (iFile != 1 && iFile != 3 && iFile != 5);
                        if (tmpSysValid)   // wait for iFile = 2, iFile = 4 and iFile = 6
                        {
                            if (iFile == 2 || iFile == 4 || iFile == 6) {

                                calcTH1AbsMax4SysUnc(sysHist[iFile][i].h1D_diff,
                                                     sysHist[iFile-1][i].h1D_diff, sysHist[iFile][i].h1D_diff);

                                // from this moment on, iFile covers the SYS. UNC. coming from iFile-1
                                // do not use iFile-1 any more.
                                // Ex. iFile = 2, 4, 6 cover SYS. UNC. coming from iFile = 1, 3, 5
                            }
                        }
                    }
                    else if (isPP)
                    {
                        // take the maximum of uncertainties : SYS_jes = max(jes106, jes096), iFile = 1,2
                        // take the maximum of uncertainties : SYS_ees = max(eesPlus, eesMinus), iFile = 3,4
                        // take the maximum of uncertainties : SYS_corrJetSmear = max(jetSmearPlus, jetSmearMinus), iFile = 5,6
                        // iFile = 1,2 : jet energy scale
                        // iFile = 3,4 : electron energy scale
                        // iFile = 5,6 : jet smearing
                        if (nInputFiles - 1 > 5) {
                            tmpSysValid = (iFile != 1 && iFile != 3 && iFile != 5);
                            if (tmpSysValid)    // wait for iFile = 2, iFile = 4 and iFile = 6
                            {
                                if (iFile == 2 || iFile == 4 || iFile == 6) {

                                    calcTH1AbsMax4SysUnc(sysHist[iFile][i].h1D_diff,
                                                         sysHist[iFile-1][i].h1D_diff, sysHist[iFile][i].h1D_diff);

                                    // from this moment on, iFile covers the SYS. UNC. coming from iFile-1
                                    // do not use iFile-1 any more.
                                    // Ex. iFile = 2, 4, 6 cover SYS. UNC. coming from iFile = 1, 3, 5
                                }
                            }
                        }
                        else {
                            tmpSysValid = (iFile != 1 && iFile != 3);
                            if (tmpSysValid)   // wait for iFile = 2 and iFile = 4
                            {
                                if (iFile == 2 || iFile == 4) {

                                    calcTH1AbsMax4SysUnc(sysHist[iFile][i].h1D_diff,
                                                         sysHist[iFile-1][i].h1D_diff, sysHist[iFile][i].h1D_diff);
                                }
                            }
                        }
                    }
                    // h1D_unc_diff_Max must have been calculated in the block above.
                    sysHist[iFile][i].isSet = tmpSysValid;
                    if (!sysHist[iFile][i].isSet)  continue;

                    calcTH1Abs4SysUnc(sysHist[iFile][i].h1D_diff);  // h = abs(h)
                    addSysUnc(sysHist[0][i].h1D_diff, sysHist[iFile][i].h1D_diff);

                    // calculate relative unc. histograms
                    sysHist[iFile][i].h1D_diff_rel = (TH1D*)sysHist[iFile][i].h1D_diff->Clone(tmpHistName_diff_rel.c_str());
                    sysHist[iFile][i].h1D_diff_rel->Divide(sysHist[iFile][i].h1D_nominal);
                    calcTH1Abs4SysUnc(sysHist[iFile][i].h1D_diff_rel);  // h = abs(h)
                    addSysUnc(sysHist[0][i].h1D_diff_rel, sysHist[iFile][i].h1D_diff_rel);
                    sysHist[iFile][i].h1D_diff_rel->SetYTitle(titleY_h1D_diff_rel.c_str());
                    sysHist[iFile][i].h1D_diff_rel->SetMinimum(0);
                    sysHist[iFile][i].h1D_diff_rel->SetMaximum(2);

                    sysHist[iFile][i].h1D_ratio_rel = (TH1D*)sysHist[iFile][i].h1D_ratio->Clone(tmpHistName_ratio_rel.c_str());
                    subtractIdentity4SysUnc(sysHist[iFile][i].h1D_ratio_rel);
                    calcTH1Abs4SysUnc(sysHist[iFile][i].h1D_ratio_rel);  // h = abs(h)
                    addSysUnc(sysHist[0][i].h1D_ratio_rel, sysHist[iFile][i].h1D_ratio_rel);
                    sysHist[iFile][i].h1D_ratio_rel->SetYTitle(titleY_h1D_ratio_rel.c_str());
                    sysHist[iFile][i].h1D_ratio_rel->SetMinimum(0);
                    sysHist[iFile][i].h1D_ratio_rel->SetMaximum(2);

                    // make a fit to h1D_diff
                    for (int j = 0; j < SYS::kN_SYSPOLFNC; ++j) {

                        double xmin = sysHist[iFile][i].h1D_diff->GetXaxis()->GetBinLowEdge(sysHist[iFile][i].h1D_diff->GetXaxis()->GetFirst());
                        double xmax = sysHist[iFile][i].h1D_diff->GetXaxis()->GetBinLowEdge(sysHist[iFile][i].h1D_diff->GetXaxis()->GetLast()+1);
                        std::string tmpName_fnc_pol = Form("fnc_%s_pol_%d", tmpName_diff.c_str(), j);

                        sysHist[iFile][i].fnc_pol[j] = new TF1(tmpName_fnc_pol.c_str() , SYS::POLFNC_FORMULAS[j].c_str(),
                                xmin, xmax);
                        sysHist[iFile][i].fnc_pol[j]->SetLineColor(SYS::POLFNC_COLORS[j]);

                        sysHist[iFile][i].h1D_diff->Fit(tmpName_fnc_pol.c_str(), fitOptions.at(0).c_str());
                        sysHist[iFile][i].isFit_fnc_pol[j] = true;

                        std::string tmpFormula = systematicsHist::get_fnc_pol_Formula(sysHist[iFile][i].fnc_pol[j], j);

                        // add this fit function to the total uncertainty function
                        std::string uncTot_diff_Formula = sysHist[0][i].fnc_pol_formula[j].c_str();
                        if (uncTot_diff_Formula.size() == 0) {
                            uncTot_diff_Formula = Form("%s*%s", tmpFormula.c_str(), tmpFormula.c_str());
                        }
                        else {
                            uncTot_diff_Formula = Form("%s + %s*%s", uncTot_diff_Formula.c_str(),
                                    tmpFormula.c_str(), tmpFormula.c_str());
                        }
                        sysHist[0][i].fnc_pol_formula[j] = uncTot_diff_Formula.c_str();
                        sysHist[0][i].fnc_pol[j] = new TF1(sysHist[0][i].fnc_pol[j]->GetName(),
                                sysHist[0][i].fnc_pol_formula[j].c_str(),
                                sysHist[0][i].fnc_pol[j]->GetXmin(), sysHist[0][i].fnc_pol[j]->GetXmax());
                        sysHist[0][i].fnc_pol[j]->SetLineColor(SYS::POLFNC_COLORS[j]);
                        // debug with cout
                        std::cout << "uncTot_diff_Formula = " << sysHist[0][i].fnc_pol_formula[j].c_str() << std::endl;
                        std::cout << "sysHist[0][i].fnc_pol[j] name = " << sysHist[0][i].fnc_pol[j]->GetName() << std::endl;
                        std::cout << "xmin = " << sysHist[0][i].fnc_pol[j]->GetXmin() << std::endl;
                        std::cout << "xmax = " << sysHist[0][i].fnc_pol[j]->GetXmax() << std::endl;

                        // histograms made by bin-by-bin evaluation of fnc_pol
                        std::string tmpHistName_fnc_pol = Form("h1D_%s", tmpName_fnc_pol.c_str());
                        sysHist[iFile][i].h1D_fnc_pol[j] = (TH1D*)sysHist[iFile][i].h1D_diff->Clone(tmpHistName_fnc_pol.c_str());
                        fillTH1fromTF1(sysHist[iFile][i].h1D_fnc_pol[j], sysHist[iFile][i].fnc_pol[j]);
                        sysHist[iFile][i].h1D_fnc_pol[j]->SetMarkerColor(SYS::POLFNC_COLORS[j]);
                        // add h1D_fnc_pol to the total uncertainty histogram
                        addSysUnc(sysHist[0][i].h1D_fnc_pol[j], sysHist[iFile][i].h1D_fnc_pol[j]);

                        std::string tmpName_fnc_pol_rel = Form("fnc_%s_pol_rel_%d", tmpName_diff.c_str(), j);
                        std::string tmpHistName_fnc_pol_rel = Form("h1D_%s", tmpName_fnc_pol_rel.c_str());
                        sysHist[iFile][i].h1D_fnc_pol_rel[j] = (TH1D*)sysHist[iFile][i].h1D_fnc_pol[j]->Clone(tmpHistName_fnc_pol_rel.c_str());
                        sysHist[iFile][i].h1D_fnc_pol_rel[j]->Divide(sysHist[iFile][i].h1D_nominal);
                        // TH1 style
                        sysHist[iFile][i].h1D_fnc_pol_rel[j]->SetMarkerColor(SYS::POLFNC_COLORS[j]);
                        sysHist[iFile][i].h1D_fnc_pol_rel[j]->SetMarkerStyle(SYS::POLFNC_MARKERSTYLES[j]);
                        sysHist[iFile][i].h1D_fnc_pol_rel[j]->GetFunction(tmpName_fnc_pol.c_str())->SetBit(TF1::kNotDraw);
                        // TH1 style - END

                        // add h1D_fnc_pol_rel to the total uncertainty histogram
                        addSysUnc(sysHist[0][i].h1D_fnc_pol_rel[j], sysHist[iFile][i].h1D_fnc_pol_rel[j]);
                    }

                    for (int iCanvas = 0; iCanvas < 4; ++iCanvas)
                    {
                        // this canvas will plot total uncertainty, skip until all the files are processed.
                        if (iCanvas > 1 && iFile < nInputFiles-1)  continue;

                        std::string tmpCnvName = "";
                        if (iCanvas == 0) {         // draw difference
                            tmpCnvName = Form("cnv_%s", tmpName_diff.c_str());
                        }
                        else if (iCanvas == 1) {         // draw relative difference
                            tmpCnvName = Form("cnv_%s", tmpName_diff_rel.c_str());
                        }
                        else if (iCanvas == 2 && iFile == nInputFiles-1) {    // draw total uncertainty after all individual uncertainties are calculated.
                            tmpCnvName = Form("cnv_%s", tmpName_uncTot_diff.c_str());
                        }
                        else if (iCanvas == 3 && iFile == nInputFiles-1) {    // draw total rel. uncertainty after all individual uncertainties are calculated.
                            tmpCnvName = Form("cnv_%s", tmpName_uncTot_diff_rel.c_str());
                        }
                        c = new TCanvas("cnv","",windowWidth,windowHeight);
                        c->SetName(tmpCnvName.c_str());
                        std::cout<<"preparing canvas : "<< c->GetName() <<std::endl;

                        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
                        setCanvasFinal(c, setLogx, setLogy);
                        c->cd();

                        TLegend* leg = new TLegend();
                        double yHorizontal = -1;
                        if (iCanvas == 0) {         // draw difference
                            sysHist[iFile][i].h1D_nominal->SetMarkerColor(kRed);
                            sysHist[iFile][i].h1D_nominal->Draw("e same");
                            sysHist[iFile][i].h1D_varied->SetMarkerColor(kBlue);
                            sysHist[iFile][i].h1D_varied->Draw("e same");

                            sysHist[iFile][i].h1D_diff->Draw("e same");
                            sysHist[iFile][i].h1D_diff->Write("",TObject::kOverwrite);

                            leg = new TLegend(0.6, 0.6, 0.9, 0.85);
                            leg->AddEntry(sysHist[iFile][i].h1D_nominal, "nominal", "lpf");
                            leg->AddEntry(sysHist[iFile][i].h1D_varied, "varied", "lpf");
                            leg->AddEntry(sysHist[iFile][i].h1D_diff, "diff = nominal - varied", "lpf");

                            for (int j = 0; j < SYS::kN_SYSPOLFNC; ++j) {
                                if (sysHist[iFile][i].isFit_fnc_pol[j]) {
                                    sysHist[iFile][i].fnc_pol[j]->Draw("same");
                                    leg->AddEntry(sysHist[iFile][i].fnc_pol[j], SYS::POLFNC_FORMULAS[j].c_str(), "l");
                                }
                            }

                            yHorizontal = 0;
                        }
                        if (iCanvas == 1) {         // draw relative difference
                            sysHist[iFile][i].h1D_diff_rel->Draw("e same");
                            sysHist[iFile][i].h1D_diff_rel->Write("",TObject::kOverwrite);

                            for (int j = 0; j < SYS::kN_SYSPOLFNC; ++j) {
                                if (sysHist[iFile][i].isFit_fnc_pol[j]) {
                                    sysHist[iFile][i].h1D_fnc_pol_rel[j]->Draw("e same");
                                }
                            }

                            yHorizontal = 0;
                        }
                        else if (iCanvas == 2 && iFile == nInputFiles-1) {    // fit and draw total uncertainty after all individual uncertainties are calculated.

                            // initialize fit function
                            std::string tmpFncName_uncTotLinear = Form("fnc_%s_uncTot_%d", tmpName.c_str(), iFile);
                            int binFirst = sysHist[0][i].h1D_diff->GetXaxis()->GetFirst();
                            int binLast  = sysHist[0][i].h1D_diff->GetXaxis()->GetLast();
                            double TF1_xMin = sysHist[0][i].h1D_diff->GetXaxis()->GetBinLowEdge(binFirst);
                            double TF1_xMax = sysHist[0][i].h1D_diff->GetXaxis()->GetBinLowEdge(binLast+1);
                            if (correlation.compare("xjz") == 0) {
                                TF1_xMin = 0.2;
                                TF1_xMax = 1.2;
                            }
                            else if (correlation.compare("dphi_rebin") == 0) {
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
                            fnc_uncTot[i]->SetLineWidth(2);
                            fnc_uncTot[i]->SetLineStyle(kDashed);
                            if (functionFormulas_uncTot.at(i).compare("NULL") != 0)  sysHist[0][i].h1D_diff->Fit(fnc_uncTot[i]->GetName(), "chi2 R M");
                            fnc_uncTot[i]->Write(tmpFncName_uncTotLinear.c_str(), TObject::kOverwrite);

                            double tmpMax = sysHist[0][i].h1D_diff->GetMaximum();
                            sysHist[0][i].h1D_diff->SetMinimum(0);
                            sysHist[0][i].h1D_diff->SetMaximum(tmpMax + 2*TMath::Abs(tmpMax));

                            sysHist[0][i].h1D_diff->Draw("e same");
                            sysHist[0][i].h1D_diff->Write("",TObject::kOverwrite);

                            for (int j = 0; j < SYS::kN_SYSPOLFNC; ++j) {

                                std::cout << "drawing sysHist[0][i].fnc_pol[j]" << std::endl;
                                std::string uncTot_diff_Formula = sysHist[0][i].fnc_pol_formula[j].c_str();
                                sysHist[0][i].fnc_pol_formula[j] = Form("TMath::Sqrt(%s)", uncTot_diff_Formula.c_str());
                                sysHist[0][i].fnc_pol[j] = new TF1(sysHist[0][i].fnc_pol[j]->GetName(),
                                        sysHist[0][i].fnc_pol_formula[j].c_str(),
                                        sysHist[0][i].fnc_pol[j]->GetXmin(), sysHist[0][i].fnc_pol[j]->GetXmax());
                                sysHist[0][i].fnc_pol[j]->SetLineColor(SYS::POLFNC_COLORS[j]);
                                // debug with cout
                                std::cout << "uncTot_diff_Formula = " << sysHist[0][i].fnc_pol_formula[j].c_str() << std::endl;
                                std::cout << "sysHist[0][i].fnc_pol[j] name = " << sysHist[0][i].fnc_pol[j]->GetName() << std::endl;
                                std::cout << "xmin = " << sysHist[0][i].fnc_pol[j]->GetXmin() << std::endl;
                                std::cout << "xmax = " << sysHist[0][i].fnc_pol[j]->GetXmax() << std::endl;

                                sysHist[0][i].fnc_pol[j]->Draw("same");
                            }

                            yHorizontal = 0;
                        }
                        else if (iCanvas == 3 && iFile == nInputFiles-1) {    // fit and draw total uncertainty after all individual uncertainties are calculated.

                            // initialize fit function
                            std::string tmpFncName_uncTotLinear = Form("fnc_%s_uncTot_%d", tmpName.c_str(), iFile);
                            int binFirst = sysHist[0][i].h1D_diff_rel->GetXaxis()->GetFirst();
                            int binLast  = sysHist[0][i].h1D_diff_rel->GetXaxis()->GetLast();
                            double TF1_xMin = sysHist[0][i].h1D_diff_rel->GetXaxis()->GetBinLowEdge(binFirst);
                            double TF1_xMax = sysHist[0][i].h1D_diff_rel->GetXaxis()->GetBinLowEdge(binLast+1);
                            if (correlation.compare("xjz") == 0) {
                                TF1_xMin = 0.2;
                                TF1_xMax = 1.2;
                            }
                            else if (correlation.compare("dphi_rebin") == 0) {
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
                            fnc_uncTot[i]->SetLineWidth(2);
                            fnc_uncTot[i]->SetLineStyle(kDashed);
                            if (functionFormulas_uncTot.at(i).compare("NULL") != 0)  sysHist[0][i].h1D_diff_rel->Fit(fnc_uncTot[i]->GetName(), "chi2 R M");
                            fnc_uncTot[i]->Write(tmpFncName_uncTotLinear.c_str(), TObject::kOverwrite);

                            sysHist[0][i].h1D_diff_rel->Draw("e same");
                            sysHist[0][i].h1D_diff_rel->Write("",TObject::kOverwrite);

                            for (int j = 0; j < SYS::kN_SYSPOLFNC; ++j) {
                                sysHist[0][i].h1D_fnc_pol_rel[j]->Draw("e same");
                            }

                            yHorizontal = 0;
                        }

                        if (leg != 0) {
                            leg->SetBorderSize(0);
                            leg->Draw();
                        }

                        // draw a horizontal line
                        double xmin = -1;
                        double xmax = 1;
                        if (sysHist[0][i].isValid) {
                            xmin = sysHist[0][i].h1D_varied->GetXaxis()->GetBinLowEdge(sysHist[0][i].h1D_varied->GetXaxis()->GetFirst());
                            xmax = sysHist[0][i].h1D_varied->GetXaxis()->GetBinLowEdge(sysHist[0][i].h1D_varied->GetXaxis()->GetLast()+1);
                        }
                        TLine line_horizontal(xmin, yHorizontal, xmax, yHorizontal);
                        line_horizontal.SetLineStyle(kDashed);
                        line_horizontal.Draw();

                        int cent_low = (int)(bins_hiBin[0].at(iHiBin)/2);
                        int cent_up  = (int)(bins_hiBin[1].at(iHiBin)/2);
                        // put info onto the canvas

                        std::vector<std::string> textLines;
                        std::string collisionText;
                        if (isHI)  collisionText = Form("PbPb, %d-%d %%", cent_low, cent_up);
                        else       collisionText = "pp";
                        bool writeText = false;     // decide whether to write more info than collision type
                        textLines.push_back(collisionText.c_str());
                        if (writeText) {
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
                        }
                        int nTextLines = textLines.size();
                        std::string textPosition = textPositions.at(i).c_str();

                        TLatex* latex = 0;
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
