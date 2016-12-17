/*
 * macro to draw photon trigger observables such efficiency, miss rate, inefficiency, energy scale wrt. HLT objects.
 * saves histograms to a .root file.
 * if "outputFigureName" is specified, saves the canvas that includes all turn on curves as picture.
 */

#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLine.h>
#include <TLatex.h>
#include <TGraphAsymmErrors.h>
#include <TMath.h>

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include "../../CorrelationTuple/EventMatcher.h"
#include "../../TreeHeaders/CutConfigurationTree.h"
#include "../../TreeHeaders/ggHiNtuplizerTree.h"
#include "../../TreeHeaders/hltObjectTree.h"
#include "../../Utilities/interface/CutConfigurationParser.h"
#include "../../Utilities/interface/InputConfigurationParser.h"
#include "../../Utilities/interface/HiForestInfoController.h"
#include "../../Utilities/fileUtil.h"
#include "../../Utilities/styleUtil.h"
#include "../../Plotting/commonUtility.h"

void photonTrigger(const TString configFile, const TString inputFile, const TString outputFile = "photonTrigger.root", const TString outputFigureName = "");
int setBranchAdressTrigger(TTree* tree, std::string branch, int & branchVal);
std::string getHLTObjectName(std::string triggerName);
std::string prepareTextEta(float etaLow, float etaUp);
float extractPtThreshold(std::string triggerName);
float extractEtaThreshold(std::string triggerName);
float getOffset4PtThreshold(float ptThreshold);
int ecalRegion(float eta);
void setTH1_photonTrigger(TH1* h, TCanvas* c = 0);
int getBin4IntegralFraction(TH2D* h2D, float fraction, std::string direction = "X");

const float ECAL_boundary_1 = 1.4442;
const float ECAL_boundary_2 = 1.566;

void photonTrigger(const TString configFile, const TString inputFile, const TString outputFile, const TString outputFigureName)
{
    std::cout<<"running photonTrigger()"<<std::endl;
    std::cout<<"configFile  = "<< configFile.Data() <<std::endl;
    std::cout<<"inputFile   = "<< inputFile.Data()  <<std::endl;
    std::cout<<"outputFile  = "<< outputFile.Data() <<std::endl;

    bool doEcalNoiseMask= false;
    std::cout << "doEcalNoiseMask = " << doEcalNoiseMask << std::endl;

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

    // input configuration

    /*
     * mode = 0 : default mode
     * mode = 1 : default mode + do HLT energy scale
     * mode = 2 : default mode + do HLT energy scale + do HLT energy scale as fnc. of photon variables
     */
    // input for mode
    int mode = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_mode];

    // input for TTree
    std::string treePath = configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treePath];

    int collisionType = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_collisionType];

    // input for TH1
    // nBins, xLow, xUp for the TH1D histogram
    std::vector<float> TH1D_Bins = ConfigurationParser::ParseListFloat(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_TH1D_Bins_List]);
    float yMin = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_TH1_yMin];
    float yMax = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_TH1_yMax];
    // nBinsx, xLow, xUp, nBinsy, yLow, yUp for a TH2D histogram :
    // 1st bin List is for HLT energy scale histogram
    std::vector<std::vector<float>> TH2D_Bins_List = ConfigurationParser::ParseListTH2D_Bins(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_TH2D_Bins_List]);
    float titleOffsetX = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_titleOffsetX];
    float titleOffsetY = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_titleOffsetY];
    float markerSize = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_markerSize];
    int lineWidth = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_lineWidth];

    // input for TLegend
    std::string legendPosition = configInput.proc[INPUT::kPLOTTING].s[INPUT::k_legendPosition];

    // input for TCanvas
    int windowWidth = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_windowWidth];
    int windowHeight = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_windowHeight];
    float leftMargin = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_leftMargin];
    float rightMargin = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_rightMargin];
    float bottomMargin = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_bottomMargin];
    float topMargin = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_topMargin];

    int nTH1D_Bins = TH1D_Bins.size();
    int nBins = (int)TH1D_Bins.at(0);
    float xLow = TH1D_Bins.at(1);
    float xUp  = TH1D_Bins.at(2);
    int nTH2D_Bins_List = TH2D_Bins_List[0].size();

    // set default values
    if (nTH1D_Bins == 0) nTH1D_Bins = 30;
    if (xUp == 0)        xUp = 60;
    if (nTH2D_Bins_List == 0) {
        TH2D_Bins_List[0].push_back(nTH1D_Bins);
        TH2D_Bins_List[1].push_back(xLow);
        TH2D_Bins_List[2].push_back(xUp);
        TH2D_Bins_List[3].push_back(40);
        TH2D_Bins_List[4].push_back(0.4);
        TH2D_Bins_List[5].push_back(2.2);
    }

    if (titleOffsetX == 0) titleOffsetX = INPUT_DEFAULT::titleOffsetX;
    if (titleOffsetY == 0) titleOffsetY = INPUT_DEFAULT::titleOffsetY;

    if (markerSize == 0)  markerSize = INPUT_DEFAULT::markerSize;
    if (lineWidth == 0)  lineWidth = INPUT_DEFAULT::lineWidth;

    if (windowWidth  == 0)  windowWidth = INPUT_DEFAULT::windowWidth;
    if (windowHeight == 0)  windowHeight = INPUT_DEFAULT::windowHeight;
    if (leftMargin == 0) leftMargin = INPUT_DEFAULT::leftMargin;
    if (rightMargin == 0) rightMargin = INPUT_DEFAULT::rightMargin;
    if (bottomMargin == 0) bottomMargin = INPUT_DEFAULT::bottomMargin;
    if (topMargin == 0) topMargin = INPUT_DEFAULT::topMargin;

    // verbose about input configuration
    std::cout<<"Input Configuration :"<<std::endl;
    std::cout << "mode = " << mode << std::endl;
    std::cout << "treePath = " << treePath.c_str() << std::endl;
    std::cout << "collisionType = " << collisionType << std::endl;
    const char* collisionName =  getCollisionTypeName((COLL::TYPE)collisionType).c_str();
    std::cout << "collision = " << collisionName << std::endl;
    std::cout << "nTH1D_Bins = " << nTH1D_Bins << std::endl;
    std::cout << "nBins = " << nBins << std::endl;
    std::cout << "xLow  = " << xLow << std::endl;
    std::cout << "xUp   = " << xUp << std::endl;
    if (yMin != 0)  std::cout << "yMin = " << yMin << std::endl;
    if (yMax != 0)  std::cout << "yMax = " << yMax << std::endl;
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
    std::cout << "lineWidth = " << lineWidth << std::endl;
    std::cout << "legendPosition = " << legendPosition.c_str() << std::endl;
    std::cout << "windowWidth  = " << windowWidth << std::endl;
    std::cout << "windowHeight = " << windowHeight << std::endl;
    std::cout << "leftMargin   = " << leftMargin << std::endl;
    std::cout << "rightMargin  = " << rightMargin << std::endl;
    std::cout << "bottomMargin = " << bottomMargin << std::endl;
    std::cout << "topMargin    = " << topMargin << std::endl;

    //bool isMC = collisionIsMC((COLL::TYPE)collisionType);
    bool isHI = collisionIsHI((COLL::TYPE)collisionType);
    bool isPP = collisionIsPP((COLL::TYPE)collisionType);

    // cut configuration
    // triggers that go into numerator
    std::vector<std::pair<std::string, int>> triggerBranchesNumANDindices = ConfigurationParser::ParseListOfList(
            configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_triggerNum_List]);
    std::vector<std::string> triggerBranchesNum = ConfigurationParser::getVecString(triggerBranchesNumANDindices);
    std::vector<int> triggerBranchesNumIndices = ConfigurationParser::getVecIndex(triggerBranchesNumANDindices);
    // triggers that go into denominator
    // global denominator : list of triggers that define the subset of events to consider.
    std::vector<std::string> triggerBranchesDenomGlobal = ConfigurationParser::ParseList(
            configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_triggerDenomGlobal_List]);
    // individual denominators
    std::vector<std::pair<std::string, int>> triggerBranchesDenomANDindices = ConfigurationParser::ParseListOfList(
            configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_triggerDenom_List]);
    std::vector<std::string> triggerBranchesDenom = ConfigurationParser::getVecString(triggerBranchesDenomANDindices);
    std::vector<int> triggerBranchesDenomIndices = ConfigurationParser::getVecIndex(triggerBranchesDenomANDindices);
    // prescales to check in numerators and denominators
    std::vector<std::string> prescaleBranchesNum = ConfigurationParser::ParseList(
            configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_triggerNumPrescl_List]);
    // prescales to check only in denominators
    std::vector<std::string> prescaleBranchesDenom = ConfigurationParser::ParseList(
            configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_triggerDenomPrescl_List]);
    // These triggers will be "OR"ed.
    // This vector must be empty not to use any triggers in the denominator.

    std::vector<float> bins_eta[2];     // array of vectors for eta bins, each array element is a vector.
    std::vector<float> bins_pt[2];      // array of vectors for recoPt bins, each array element is a vector, should function also as
                                        // list of pt cuts for RECO photons

    bins_eta[0] = ConfigurationParser::ParseListFloat(
            configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_eta_gt]);
    bins_eta[1] = ConfigurationParser::ParseListFloat(
            configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_eta_lt]);
    bins_pt[0] = ConfigurationParser::ParseListFloat(
            configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_pt_gt]);
    bins_pt[1] = ConfigurationParser::ParseListFloat(
            configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_pt_lt]);

    // RECO photon cuts
    // 1st element is EB cut
    // 2dn element is EE cut
    // if there is single element, then it is applied both for EB and EE
    std::vector<float> cut_phoHoverE = ConfigurationParser::ParseListOrFloat(
            configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].str_f[CUTS::PHO::k_phoHoverE]);
    std::vector<float> cut_pho_ecalClusterIsoR4 = ConfigurationParser::ParseListOrFloat(
            configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].str_f[CUTS::PHO::k_pho_ecalClusterIsoR4]);
    std::vector<float> cut_pho_hcalRechitIsoR4 = ConfigurationParser::ParseListOrFloat(
            configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].str_f[CUTS::PHO::k_pho_hcalRechitIsoR4]);
    std::vector<float> cut_pho_trackIsoR4PtCut20 = ConfigurationParser::ParseListOrFloat(
            configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].str_f[CUTS::PHO::k_pho_trackIsoR4PtCut20]);
    std::vector<float> cut_sumIso = ConfigurationParser::ParseListOrFloat(
            configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].str_f[CUTS::PHO::k_sumIso]);
    std::vector<float> cut_phoSigmaIEtaIEta_2012 = ConfigurationParser::ParseListOrFloat(
            configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].str_f[CUTS::PHO::k_phoSigmaIEtaIEta_2012]);

    // set default values
    if (triggerBranchesDenom.size() == 0) {
        triggerBranchesDenom.push_back("");
        triggerBranchesDenomIndices.push_back(0);
    }
    if (prescaleBranchesNum.size() == 0) prescaleBranchesNum.push_back("");
    if (prescaleBranchesDenom.size() == 0) prescaleBranchesDenom.push_back("");

    if (bins_eta[0].size() == 0) {
        bins_eta[0] = {0,   0,   0,    1.48};
        bins_eta[1] = {3.1, 2.1, 1.44, 2.4};
    }
    if (bins_pt[0].size() == 0) {
        bins_pt[0] = {0};
        bins_pt[1] = {-1};
    }

    if (bins_pt[1].size() < bins_pt[0].size()) {
        int diff = (int signed)(bins_pt[0].size() - bins_pt[1].size());
        for (int i=0; i < diff; ++i)
            bins_pt[1].push_back(-1);
    }

    int nTriggersNum = triggerBranchesNum.size();
    int nNum = triggerBranchesNumIndices.at(nTriggersNum-1)+1;
    int nTriggersDenomGlobal = triggerBranchesDenomGlobal.size();
    int nTriggersDenom = triggerBranchesDenom.size();
    int nDenom = triggerBranchesDenomIndices.at(nTriggersDenom-1)+1;
    int nPrescaleNum = prescaleBranchesNum.size();
    int nPrescaleDenom = prescaleBranchesDenom.size();
    int nBins_eta = bins_eta[0].size();   // assume <myvector>[0] and <myvector>[1] have the same size.
    int nBins_pt = bins_pt[0].size();     // assume <myvector>[0] and <myvector>[1] have the same size.
    int nCut_phoHoverE = cut_phoHoverE.size();
    int nCut_pho_ecalClusterIsoR4 = cut_pho_ecalClusterIsoR4.size();
    int nCut_pho_hcalRechitIsoR4 = cut_pho_hcalRechitIsoR4.size();
    int nCut_pho_trackIsoR4PtCut20 = cut_pho_trackIsoR4PtCut20.size();
    int nCut_sumIso = cut_sumIso.size();
    int nCut_phoSigmaIEtaIEta_2012 = cut_phoSigmaIEtaIEta_2012.size();

    // indices of triggers for each numerator
    std::vector<int> triggerIndices_perNum[nNum];
    for (int i = 0; i < nNum; ++i) {
        for (int iTrig = 0; iTrig < nTriggersNum; ++iTrig ) {
            if (i == triggerBranchesNumIndices.at(iTrig)) {
                triggerIndices_perNum[i].push_back(iTrig);
            }
        }
    }
    // indices of triggers for each denominator
    std::vector<int> triggerIndices_perDenom[nDenom];
    for (int i = 0; i < nDenom; ++i) {
        for (int iTrig = 0; iTrig < nTriggersDenom; ++iTrig ) {
            if (i == triggerBranchesDenomIndices.at(iTrig)) {
                triggerIndices_perDenom[i].push_back(iTrig);
            }
        }
    }

    // identify the set of unique triggers to read
    // one trigger can be used by multiple numerators/denominators, make sure each trigger is set only once.
    std::vector<std::string> triggersToReadNum = vectorUnique(triggerBranchesNum);
    int nTriggersToReadNum = triggersToReadNum.size();
    std::vector<int> indicesToReadNum = positionsInVector(triggersToReadNum, triggerBranchesNum);

    std::vector<std::string> triggersToReadDenom = vectorUnique(triggerBranchesDenom);
    int nTriggersToReadDenom = triggersToReadDenom.size();
    std::vector<int> indicesToReadDenom = positionsInVector(triggersToReadDenom, triggerBranchesDenom);

    // identify the set of unique prescales to read
    // one prescale can be used by multiple numerators/denominators, make sure each prescale is set only once.
    std::vector<std::string> prescalesToReadNum = vectorUnique(prescaleBranchesNum);
    int nPrescalesToReadNum = prescalesToReadNum.size();
    std::vector<int> indicesPrescaleToReadNum = positionsInVector(prescalesToReadNum, prescaleBranchesNum);

    std::vector<std::string> prescalesToReadDenom = vectorUnique(prescaleBranchesDenom);
    int nPrescalesToReadDenom = prescalesToReadDenom.size();
    std::vector<int> indicesPrescaleToReadDenom = positionsInVector(prescalesToReadDenom, prescaleBranchesDenom);

    // verbose about cut configuration
    std::cout<<"Cut Configuration :"<<std::endl;
    std::cout << "nNum = " << nNum << std::endl;
    std::cout << "nTriggersNum = " << nTriggersNum << std::endl;
    for (int i=0; i<nNum; ++i) {
        std::cout << Form("num[%d] = ", i);
        int nTriggersPerNum = triggerIndices_perNum[i].size();
        for (int j = 0; j < nTriggersPerNum; ++j ) {
            if (j > 0)  std::cout << " , " << triggerBranchesNum.at(triggerIndices_perNum[i][j]).c_str();
            else        std::cout << triggerBranchesNum.at(triggerIndices_perNum[i][j]).c_str();
        }
        std::cout << std::endl;
    }
    std::cout << "nTriggersDenomGlobal = " << nTriggersDenomGlobal << std::endl;
    for (int i=0; i<nTriggersDenomGlobal; ++i) {
        std::cout << Form("triggerBranchesDenomGlobal[%d] = ", i) << triggerBranchesDenomGlobal.at(i).c_str() << std::endl;
    }
    std::cout << "nDenom = " << nDenom << std::endl;
    std::cout << "nTriggersDenom = " << nTriggersDenom << std::endl;
    for (int i=0; i<nDenom; ++i) {
        std::cout << Form("denom[%d] = ", i);
        int nTriggersPerDenom = triggerIndices_perDenom[i].size();
        for (int j = 0; j < nTriggersPerDenom; ++j ) {
            if (j > 0)  std::cout << " , " << triggerBranchesDenom.at(triggerIndices_perDenom[i][j]).c_str();
            else        std::cout << triggerBranchesDenom.at(triggerIndices_perDenom[i][j]).c_str();
        }
        std::cout << std::endl;
    }
    if (nNum > 1 && nDenom > 1 && nNum != nDenom) {
        std::cout << "mismatch in the number of numerators and denominators :" << std::endl;
        std::cout << "nNum = " << nNum << "and nDenom = " << nDenom << std::endl;
        std::cout << "exiting" << std::endl;
        return;
    }

    std::cout << "nPrescaleNum = " << nPrescaleNum << std::endl;
    for (int i=0; i<nPrescaleNum; ++i) {
        std::cout << Form("prescaleBranchesNum[%d] = ", i) << prescaleBranchesNum.at(i).c_str() << std::endl;
        if (prescaleBranchesNum.at(i).size() > 0 && prescaleBranchesNum.at(i).find("Prescl") == std::string::npos) {
            std::cout << "Warning : Branch name does not contain 'Prescl'" << std::endl;
        }
    }
    if (nPrescaleNum > 1 && nPrescaleNum != nNum)
    {
        std::cout << "mismatch in the number of numerators and prescales to be used for numerators :" << std::endl;
        std::cout << "nNum = " << nNum << "and nPrescaleNum = " << nPrescaleNum << std::endl;
        std::cout << "exiting" << std::endl;
        return;
    }
    std::cout << "nPrescaleDenom = " << nPrescaleDenom << std::endl;
    for (int i=0; i<nPrescaleDenom; ++i) {
        std::cout << Form("prescaleBranchesDenom[%d] = ", i) << prescaleBranchesDenom.at(i).c_str() << std::endl;
        if (prescaleBranchesDenom.at(i).size() > 0 && prescaleBranchesDenom.at(i).find("Prescl") == std::string::npos) {
            std::cout << "Warning : Branch name does not contain 'Prescl'" << std::endl;
        }
    }
    if (nPrescaleDenom > 1 && nPrescaleDenom != nDenom)
    {
        std::cout << "mismatch in the number of denominators and prescales to be used for denominators :" << std::endl;
        std::cout << "nNum = " << nDenom << "and nPrescaleDenom = " << nPrescaleDenom << std::endl;
        std::cout << "exiting" << std::endl;
        return;
    }

    std::cout << "nBins_eta = " << nBins_eta << std::endl;
    for (int i=0; i<nBins_eta; ++i) {
        std::cout << Form("bins_eta[%d] = [%f, %f)", i, bins_eta[0].at(i), bins_eta[1].at(i)) << std::endl;
    }
    std::cout << "nBins_pt = " << nBins_pt << std::endl;
    for (int i=0; i<nBins_pt; ++i) {
        std::cout << Form("bins_recoPt[%d] = [%f, %f)", i, bins_pt[0].at(i), bins_pt[1].at(i)) << std::endl;
    }
    std::cout << "nCut_phoHoverE = " << nCut_phoHoverE << std::endl;
    for (int i=0; i<nCut_phoHoverE; ++i) {
        std::cout << Form("cut_phoHoverE[%d] = ", i) << cut_phoHoverE.at(i) << std::endl;
    }
    std::cout << "nCut_pho_ecalClusterIsoR4 = " << nCut_pho_ecalClusterIsoR4 << std::endl;
    for (int i=0; i<nCut_pho_ecalClusterIsoR4; ++i) {
        std::cout << Form("cut_pho_ecalClusterIsoR4[%d] = ", i) << cut_pho_ecalClusterIsoR4.at(i) << std::endl;
    }
    std::cout << "nCut_pho_hcalRechitIsoR4 = " << nCut_pho_hcalRechitIsoR4 << std::endl;
    for (int i=0; i<nCut_pho_hcalRechitIsoR4; ++i) {
        std::cout << Form("cut_pho_hcalRechitIsoR4[%d] = ", i) << cut_pho_hcalRechitIsoR4.at(i) << std::endl;
    }
    std::cout << "nCut_pho_trackIsoR4PtCut20 = " << nCut_pho_trackIsoR4PtCut20 << std::endl;
    for (int i=0; i<nCut_pho_trackIsoR4PtCut20; ++i) {
        std::cout << Form("cut_pho_trackIsoR4PtCut20[%d] = ", i) << cut_pho_trackIsoR4PtCut20.at(i) << std::endl;
    }
    std::cout << "nCut_sumIso = " << nCut_sumIso << std::endl;
    for (int i=0; i<nCut_sumIso; ++i) {
        std::cout << Form("cut_sumIso[%d] = ", i) << cut_sumIso.at(i) << std::endl;
    }
    std::cout << "nCut_phoSigmaIEtaIEta_2012 = " << nCut_phoSigmaIEtaIEta_2012 << std::endl;
    for (int i=0; i<nCut_phoSigmaIEtaIEta_2012; ++i) {
        std::cout << Form("cut_phoSigmaIEtaIEta_2012[%d] = ", i) << cut_phoSigmaIEtaIEta_2012.at(i) << std::endl;
    }

    std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.Data());

    std::cout<<"input ROOT files : num = "<<inputFiles.size()<< std::endl;
    std::cout<<"#####"<< std::endl;
    for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
        std::cout<<(*it).c_str()<< std::endl;
    }
    std::cout<<"##### END #####"<< std::endl;

    if (isHI && treePath != "ggHiNtuplizer/EventTree") {
        std::cout << "WARNING : Collision is HI. But the photon tree is not set to ggHiNtuplizer/EventTree" << std::endl;
    }
    else if (isPP && treePath != "ggHiNtuplizerGED/EventTree") {
        std::cout << "WARNING : Collision is PP. But the photon tree is not set to ggHiNtuplizerGED/EventTree" << std::endl;
    }

    enum EFF_DEP {   // efficiency dependencies
        kPT,
        kETA,
        kN_EFF_DEP
    };

    TH1::SetDefaultSumw2();
    TH1D* hTmp = 0;
    TH2D* h2DTmp = 0;

    TH1D* h[kN_EFF_DEP][nBins_eta][nNum];     // denominator
    TH1D* h_Num[kN_EFF_DEP][nBins_eta][nNum];  // numerator

    bool h_Init[kN_EFF_DEP][nBins_eta][nNum];     // denominator
    bool h_Num_Init[kN_EFF_DEP][nBins_eta][nNum];  // numerator
    for (int iDep = 0; iDep < kN_EFF_DEP; ++iDep) {
        for (int iEta = 0; iEta < nBins_eta; ++iEta){
            for (int iNum = 0; iNum < nNum; ++iNum) {
                h_Init[iDep][iEta][iNum] = false;
                h_Num_Init[iDep][iEta][iNum] = false;
            }
        }
    }

    // prepare titles for numerators
    std::vector<std::string> titlesNum(nNum);
    for (int iNum = 0; iNum < nNum; ++iNum)
    {
        int iTrig0 = triggerIndices_perNum[iNum][0];
        std::string titleNum = triggerBranchesNum.at(iTrig0).c_str();
        int nTriggersInNum = triggerIndices_perNum[iNum].size();
        for (int iTrig = 1; iTrig < nTriggersInNum; ++iTrig) {
            int j = triggerIndices_perNum[iNum][iTrig];
            titleNum += Form(" || %s", triggerBranchesNum.at(j).c_str());
        }

        titlesNum.at(iNum) = titleNum;
    }

    // prepare titles for denominators
    std::vector<std::string> titlesDenom(nDenom);
    for (int iDenom = 0; iDenom < nDenom; ++iDenom)
    {
        int iTrig0 = triggerIndices_perDenom[iDenom][0];
        std::string titleDenom = triggerBranchesDenom.at(iTrig0).c_str();
        int nTriggersInDenom = triggerIndices_perDenom[iDenom].size();
        for (int iTrig = 1; iTrig < nTriggersInDenom; ++iTrig) {
            int j = triggerIndices_perDenom[iDenom][iTrig];
            titleDenom += Form(" || %s", triggerBranchesDenom.at(j).c_str());
        }
        if (titleDenom.size() == 0)  titleDenom = "Denominator";

        titlesDenom.at(iDenom) = titleDenom;
    }

    for (int iEta = 0; iEta < nBins_eta; ++iEta){

        float etaLow = bins_eta[0].at(iEta);
        float etaUp = bins_eta[1].at(iEta);
        std::string tmpTitleEta = prepareTextEta(etaLow, etaUp);

        for (int iDenom = 0; iDenom < nDenom; ++iDenom) {
            std::string tmpName = Form("etaBin%d", iEta);
            std::string tmpHistName = Form("h_pt_denom%d_%s", iDenom, tmpName.c_str());
            std::string tmpHistTitle = Form("%s, %s", titlesDenom.at(iDenom).c_str(), tmpTitleEta.c_str());

            h_Init[kPT][iEta][iDenom] = true;
            h[kPT][iEta][iDenom] = new TH1D(tmpHistName.c_str(), tmpHistTitle.c_str(), nBins, xLow, xUp);
            h[kPT][iEta][iDenom]->SetXTitle("p_{T}^{#gamma} (GeV/c)");
            h[kPT][iEta][iDenom]->SetMarkerStyle(kFullCircle);
        }

        for (int iNum = 0; iNum < nNum; ++iNum)
        {
            int iDenom = iNum;
            if (nDenom == 1) iDenom = 0;

            h_Num_Init[kPT][iEta][iNum] = true;
            std::string tmpHistName = replaceAll(h[kPT][iEta][iDenom]->GetName(), Form("_denom%d", iDenom), "");
            h_Num[kPT][iEta][iNum] = (TH1D*)h[kPT][iEta][iDenom]->Clone(Form("%s_Num%d", tmpHistName.c_str(), iNum));

            std::string tmpHistTitle = Form("%s, %s", titlesNum.at(iNum).c_str(), tmpTitleEta.c_str());
            h_Num[kPT][iEta][iNum]->SetTitle(tmpHistTitle.c_str());
        }
    }

    std::vector<float> ptThresholdsOrig(nNum);
    std::vector<float> ptThresholds(nNum);
    for (int iNum = 0; iNum < nNum; ++iNum)
    {
        int iDenom = iNum;
        if (nDenom == 1) iDenom = 0;

        h_Init[kETA][0][iNum] = true;
        h[kETA][0][iNum] = new TH1D(Form("h_eta_denom%d_Num%d", iDenom, iNum), titlesDenom.at(iDenom).c_str(), 20, -2.4, 2.4);  // denominator
        h[kETA][0][iNum]->SetXTitle("|#eta|");
        h[kETA][0][iNum]->SetMarkerStyle(kFullCircle);

        h_Num_Init[kETA][0][iNum] = true;
        std::string tmpHistName = replaceAll(h[kETA][0][iNum]->GetName(), Form("_denom%d", iDenom), "");
        h_Num[kETA][0][iNum] = (TH1D*)h[kETA][0][iNum]->Clone(tmpHistName.c_str());

        // 1st trigger in the numerator is used for extracting the threshold
        int iTrig0 = triggerIndices_perNum[iNum][0];
        float ptThreshold = extractPtThreshold(triggerBranchesNum.at(iTrig0).c_str());
        ptThresholdsOrig[iNum] = ptThreshold;
        ptThresholds[iNum] = ptThreshold + getOffset4PtThreshold(ptThreshold);

        std::string tmpHistTitle = titlesNum.at(iNum).c_str();
        tmpHistTitle += Form(" , p_{T} > %.0f", ptThresholds[iNum]);
        h_Num[kETA][0][iNum]->SetTitle(tmpHistTitle.c_str());
    }

    enum TH2D_TYPE {   // TH2D plot types
        kETAPHI,
        kHLTSCALE,
        kN_TH2D_TYPE
    };

    // efficiency plots as func. of etaphi
    TH2D* h2D_etaphi[nNum];      // denominator
    TH2D* h2D_etaphi_Num[nNum];  // numerator
    for (int iNum = 0; iNum < nNum; ++iNum) {

        int iDenom = iNum;
        if (nDenom == 1)  iDenom = 0;

        h2D_etaphi[iNum] = new TH2D(Form("h2D_etaphi_denom%d_Num%d", iDenom, iNum), titlesDenom.at(iDenom).c_str(), 40, -3.2, 3.2, 40, -TMath::Pi(), TMath::Pi());
        h2D_etaphi[iNum]->SetXTitle("#eta");
        h2D_etaphi[iNum]->SetYTitle("#phi");

        std::string tmpHistName = replaceAll(h2D_etaphi[iNum]->GetName(), Form("_denom%d", iDenom), "");
        h2D_etaphi_Num[iNum] = (TH2D*)h2D_etaphi[iNum]->Clone(tmpHistName.c_str());

        std::string tmpHistTitle = titlesNum.at(iNum).c_str();
        tmpHistTitle += Form(" , p_{T} > %.0f", ptThresholds[iNum]);
        h2D_etaphi_Num[iNum]->SetTitle(tmpHistTitle.c_str());
    }

    // HLT energy scale as func. of offline pt
    TH2D* h2D_scaleHLT[nTriggersNum][2];
    for (int iNum = 0; iNum < nTriggersNum; ++iNum) {

        std::string titleNum = triggerBranchesNum.at(iNum).c_str();

        int nBinsx  = (int)TH2D_Bins_List[0].at(0);
        float xLowTmp = TH2D_Bins_List[1].at(0);
        float xUpTmp  = TH2D_Bins_List[2].at(0);
        int nBinsy  = (int)TH2D_Bins_List[3].at(0);
        float yLowTmp = TH2D_Bins_List[4].at(0);
        float yUpTmp  = TH2D_Bins_List[5].at(0);

        h2D_scaleHLT[iNum][0] = new TH2D(Form("h2D_scaleHLT_Num%d_EB", iNum), "", nBinsx, xLowTmp, xUpTmp, nBinsy, yLowTmp, yUpTmp);
        h2D_scaleHLT[iNum][0]->SetTitle(Form("%s - |#eta| < 1.44", titleNum.c_str()));
        h2D_scaleHLT[iNum][0]->SetXTitle("offline p_{T}^{#gamma} (GeV/c)");
        h2D_scaleHLT[iNum][0]->SetYTitle(Form("HLT p_{T}/offline p_{T}"));

        h2D_scaleHLT[iNum][1] = (TH2D*)h2D_scaleHLT[iNum][0]->Clone(Form("h2D_scaleHLT_Num%d_EE", iNum));
        h2D_scaleHLT[iNum][1]->SetTitle(Form("%s - 1.56 < |#eta| < 2.4", titleNum.c_str()));
    }

    enum PHOVAR_DEP {   // TH2D plot types
        kSCEtaWidth,
        kSCPhiWidth,
        kSCBrem,
        kR9,
        ksumIso,
        kN_PHOVAR_DEP
    };
    std::vector<std::string> phoVar_Dep_xAxisTitles {"SCEtaWidth", "SCPhiWidth", "SCBrem", "R9", "sumIso"};
    std::vector<int> phoVar_Dep_nBinsX    {40, 40, 40, 40,  40};
    std::vector<double> phoVar_Dep_xLow   {0,   0,  0,  0, -20};
    std::vector<double> phoVar_Dep_xUp    {0.03, 0.05,  20,  1, 20};

    // HLT energy scale as func. of offline pt
    TH2D* h2D_scaleHLT_phoVar[nTriggersNum][kN_PHOVAR_DEP][2];
    for (int iNum = 0; iNum < nTriggersNum; ++iNum) {
        for (int iDep = 0; iDep < kN_PHOVAR_DEP; ++iDep) {

            std::string xTitle = phoVar_Dep_xAxisTitles.at(iDep);
            std::string titleNum = triggerBranchesNum.at(iNum).c_str();

            int nBinsx  = (int)phoVar_Dep_nBinsX.at(iDep);
            float xLowTmp = phoVar_Dep_xLow.at(iDep);
            float xUpTmp  = phoVar_Dep_xUp.at(iDep);
            int nBinsy  = (int)TH2D_Bins_List[3].at(0);
            float yLowTmp = TH2D_Bins_List[4].at(0);
            float yUpTmp  = TH2D_Bins_List[5].at(0);

            h2D_scaleHLT_phoVar[iNum][iDep][0] = new TH2D(Form("h2D_scaleHLT_%s_Num%d_EB", xTitle.c_str(), iNum), "", nBinsx, xLowTmp, xUpTmp, nBinsy, yLowTmp, yUpTmp);
            h2D_scaleHLT_phoVar[iNum][iDep][0]->SetTitle(Form("%s - |#eta| < 1.44", titleNum.c_str()));
            h2D_scaleHLT_phoVar[iNum][iDep][0]->SetXTitle(xTitle.c_str());
            h2D_scaleHLT_phoVar[iNum][iDep][0]->SetYTitle(Form("HLT p_{T}/offline p_{T}"));

            h2D_scaleHLT_phoVar[iNum][iDep][1] = (TH2D*)h2D_scaleHLT_phoVar[iNum][iDep][0]->Clone(Form("h2D_scaleHLT_%s_Num%d_EE", xTitle.c_str(), iNum));
            h2D_scaleHLT_phoVar[iNum][iDep][1]->SetTitle(Form("%s - 1.56 < |#eta| < 2.4", titleNum.c_str()));
            // special cases
            if (iDep == PHOVAR_DEP::kSCEtaWidth)  {
                xUpTmp = 0.05;
                h2D_scaleHLT_phoVar[iNum][iDep][1]->GetXaxis()->Set(nBins, xLowTmp, xUpTmp);
            }
        }
    }

    TTree* treeHLT = 0;
    TTree* treePhoton = 0;
    TTree* treeHiForestInfo = 0;
    TTree* treeHLTObject[nTriggersNum];

    bool doHLTObject = (mode == 1 || mode == 2);
    bool doHLTObject_phoVar = (mode == 2);
    std::cout << "doHLTObject = " << doHLTObject << std::endl;
    std::cout << "doHLTObject_phoVar = " << doHLTObject_phoVar << std::endl;
    std::vector<std::string> hltObjectPaths(nTriggersNum);
    if (doHLTObject) {
        for (int i = 0; i < nTriggersNum; ++i) {

            std::string hltObjectPath = "";
            std::string hltObjectName = getHLTObjectName(triggerBranchesNum.at(i).c_str());
            if (hltObjectName.size() > 0)
                hltObjectPath = Form("hltobject/%s", hltObjectName.c_str());

            hltObjectPaths.at(i) = hltObjectPath;
            std::cout << Form("hltObjectPath[%d] = ", i) << hltObjectPath.c_str() << std::endl;
        }
    }

    int nFiles = inputFiles.size();
    TFile* fileTmp = 0;

    std::cout << "initial reading to get the number of entries (if there is only one input file) and HiForest info" << std::endl;
    // read the first file only to get the HiForest info
    std::string inputPath = inputFiles.at(0).c_str();
    fileTmp = TFile::Open(inputPath.c_str(), "READ");

    if (nFiles == 1) {
        // read one tree only to get the number of entries
        treePhoton = (TTree*)fileTmp->Get(treePath.c_str());
        Long64_t entriesTmp = treePhoton->GetEntries();
        std::cout << "entries = " << entriesTmp << std::endl;
        treePhoton->Delete();
    }

    treeHiForestInfo = (TTree*)fileTmp->Get("HiForest/HiForestInfo");
    HiForestInfoController hfic(treeHiForestInfo);
    std::cout<<"### HiForestInfo Tree ###"<< std::endl;
    hfic.printHiForestInfo();
    std::cout<<"###"<< std::endl;

    fileTmp->Close();
    // done with initial reading

    EventMatcher* em = new EventMatcher();
    Long64_t duplicateEntries = 0;

    Long64_t entries = 0;
    Long64_t entriesPassedDenomGlobal = 0;
    Long64_t entriesAnalyzed = 0;
    std::cout<< "Loop : " << treePath.c_str() <<std::endl;
    for (int iFile = 0; iFile < nFiles; ++iFile) {

        std::string inputPath = inputFiles.at(iFile).c_str();
        std::cout <<"iFile = " << iFile << " , " ;
        std::cout <<"reading input file : " << inputPath.c_str() << std::endl;
        fileTmp = TFile::Open(inputPath.c_str(), "READ");

        // check if the file is usable, if not skip the file.
        if (isGoodFile(fileTmp) != 0) {
            std::cout << "File is not good. skipping file." << std::endl;
            continue;
        }

        std::string treePathHLT = "hltanalysis/HltTree";
        treeHLT = (TTree*)fileTmp->Get(treePathHLT.c_str());
        if (!treeHLT) {
            std::cout << "tree is not found in the path : "<< "hltanalysis/HltTree" <<". skipping file." << std::endl;
            continue;
        }
        treeHLT->SetBranchStatus("*",0);     // disable all branches
        bool passedNum[nNum];
        Int_t triggersNum[nTriggersToReadNum];
        for (int i=0; i < nTriggersToReadNum; ++i) {

            std::string branch = triggersToReadNum.at(i).c_str();
            int branchSetFlag = setBranchAdressTrigger(treeHLT, branch, triggersNum[i]);
            if (branchSetFlag == -1) {
                std::cout<<"set branch addresses for triggers that go into numerator"<<std::endl;
                std::cout<<"Following branch is not found : " << branch.c_str() <<std::endl;
            }
        }

        Int_t triggersDenomGlobal[nTriggersDenomGlobal];
        for (int i=0; i < nTriggersDenomGlobal; ++i) {

            std::string branch = triggerBranchesDenomGlobal.at(i).c_str();
            int branchSetFlag = setBranchAdressTrigger(treeHLT, branch, triggersDenomGlobal[i]);
            if (branchSetFlag == -1) {
                std::cout<<"set branch addresses for triggers that go into denominator"<<std::endl;
                std::cout<<"Following branch is not found : " << branch.c_str() <<std::endl;
            }
        }

        bool passedDenom[nNum];
        Int_t triggersDenom[nTriggersToReadDenom];
        for (int i=0; i < nTriggersToReadDenom; ++i) {

            std::string branch = triggersToReadDenom.at(i).c_str();
            int branchSetFlag = setBranchAdressTrigger(treeHLT, branch, triggersDenom[i]);
            if (branchSetFlag == -1) {
                std::cout<<"set branch addresses for triggers that go into denominator"<<std::endl;
                std::cout<<"Following branch is not found : " << branch.c_str() <<std::endl;
            }
        }

        bool noPrescaleNum[nPrescaleNum];
        Int_t prescalesNum[nPrescalesToReadNum];
        for (int i=0; i < nPrescalesToReadNum; ++i) {

            std::string branch = prescalesToReadNum.at(i).c_str();
            int branchSetFlag = setBranchAdressTrigger(treeHLT, branch, prescalesNum[i]);
            if (branchSetFlag == -1) {
                std::cout<<"set branch addresses for prescale values that go into numerator"<<std::endl;
                std::cout<<"Following branch is not found : " << branch.c_str() <<std::endl;
            }
        }

        bool noPrescaleDenom[nPrescaleDenom];
        Int_t prescalesDenom[nPrescalesToReadDenom];
        for (int i=0; i < nPrescalesToReadDenom; ++i) {

            std::string branch = prescalesToReadDenom.at(i).c_str();
            int branchSetFlag = setBranchAdressTrigger(treeHLT, branch, prescalesDenom[i]);
            if (branchSetFlag == -1) {
                std::cout<<"set branch addresses for prescale values that go into denominator"<<std::endl;
                std::cout<<"Following branch is not found : " << branch.c_str() <<std::endl;
            }
        }

        treePhoton = (TTree*)fileTmp->Get(treePath.c_str());
        if (!treePhoton) {
            std::cout << "tree is not found in the path : "<< treePath.c_str() <<". skipping file." << std::endl;
            continue;
        }
        treePhoton->SetBranchStatus("*",0);     // disable all branches
        treePhoton->SetBranchStatus("run",1);    // enable event information
        treePhoton->SetBranchStatus("event",1);
        treePhoton->SetBranchStatus("lumis",1);

        treePhoton->SetBranchStatus("nPho",1);     // enable photon branches
        treePhoton->SetBranchStatus("pho*",1);     // enable photon branches

        ggHiNtuplizer ggHi;
        ggHi.setupTreeForReading(treePhoton);

        bool existsHLTObject[nTriggersNum];
        hltObject hltObj[nTriggersNum];
        if (doHLTObject) {
            for (int iHLT = 0; iHLT < nTriggersNum; ++iHLT){
                treeHLTObject[iHLT] = 0;

                std::string hltObjectPath = hltObjectPaths[iHLT].c_str();
                existsHLTObject[iHLT] = (hltObjectPath.size() > 0);

                if (existsHLTObject[iHLT]) {
                    treeHLTObject[iHLT] = (TTree*)fileTmp->Get(hltObjectPath.c_str());
                    existsHLTObject[iHLT] = (bool)treeHLTObject[iHLT];

                    if (existsHLTObject[iHLT]) {
                        treeHLTObject[iHLT]->SetBranchStatus("*",0);     // disable all branches
                        treeHLTObject[iHLT]->SetBranchStatus("pt",1);
                        treeHLTObject[iHLT]->SetBranchStatus("eta",1);
                        treeHLTObject[iHLT]->SetBranchStatus("phi",1);

                        hltObj[iHLT].setupTreeForReading(treeHLTObject[iHLT]);
                    }
                    else {
                        std::cout << "tree is not found in the path : "<< hltObjectPath.c_str() <<". skipping the tree." << std::endl;
                    }
                }
            }
        }

        Long64_t entriesTmp = treePhoton->GetEntries();
        entries += entriesTmp;
        std::cout << "entries in File = " << entriesTmp << std::endl;

        // make sure the the trees have same number of entries
        bool equalEntries = true;
        equalEntries &= (entriesTmp == treeHLT->GetEntries());
        if (doHLTObject) {
            for (int iHLT = 0; iHLT < nTriggersNum; ++iHLT){
                if (existsHLTObject[iHLT])
                    equalEntries &= (entriesTmp == treeHLTObject[iHLT]->GetEntries());
            }
        }
        if (!equalEntries) {
            std::cout << "Trees in the file have non-equal number of entries." << std::endl;
            std::cout << "Entries in " << treePath.c_str() << " = " << entriesTmp << std::endl;
            std::cout << "Entries in " << treePathHLT.c_str() << " = " << treeHLT->GetEntries() << std::endl;
            if (doHLTObject) {
                for (int iHLT = 0; iHLT < nTriggersNum; ++iHLT){
                    if (existsHLTObject[iHLT])
                        std::cout << "Entries in " << treeHLTObject[iHLT]->GetName() << " = "
                            << treeHLTObject[iHLT]->GetEntries() << std::endl;
                }
            }
            std::cout << "skipping file." << std::endl;
            continue;
        }

        for (Long64_t j_entry = 0; j_entry < entriesTmp; ++j_entry)
        {
            if (j_entry % 20000 == 0)  {
              std::cout << "current entry = " <<j_entry<<" out of "<<entriesTmp<<" : "<<std::setprecision(2)<<(double)j_entry/entriesTmp*100<<" %"<<std::endl;
            }

            treeHLT->GetEntry(j_entry);
            treePhoton->GetEntry(j_entry);

            bool eventAdded = em->addEvent(ggHi.run, ggHi.lumis, ggHi.event, j_entry);
            if(!eventAdded) // this event is duplicate, skip this one.
            {
                duplicateEntries++;
                continue;
            }

            bool passedDenomGlobal = false;
            if (nTriggersDenomGlobal == 0) passedDenomGlobal = true;
            else {
                // triggers in the denominator are "OR"ed.
                for (int i = 0; i<nTriggersDenomGlobal; ++i)
                {
                    if (triggersDenomGlobal[i] == 1) {
                        passedDenomGlobal = true;
                        break;
                    }
                }
            }
            if (!passedDenomGlobal) continue;
            entriesPassedDenomGlobal++;

            bool passedAnyDenom = false;
            if (nDenom == 0) passedAnyDenom = true;
            for (int i = 0; i < nDenom; ++i) {
                passedDenom[i] = false;
                // loop over trigger for this denominator
                int nTriggersInDenom = triggerIndices_perDenom[i].size();
                if (nTriggersInDenom == 0)  passedDenom[i] = true;
                else {
                    for (int iTrig = 0; iTrig < nTriggersInDenom; ++iTrig) {

                        int jTrig = indicesToReadDenom[triggerIndices_perDenom[i][iTrig]];
                        // triggers in the numerator are "OR"ed.
                        if (triggersDenom[jTrig] > 0) {
                            passedDenom[i] = true;
                            break;
                        }
                    }
                }
                passedAnyDenom |= passedDenom[i];
            }
            if (!passedAnyDenom) continue;

            for (int i = 0; i < nNum; ++i) {
                passedNum[i] = false;
                // loop over trigger for this numerator
                int nTriggersInNum = triggerIndices_perNum[i].size();
                if (nTriggersInNum == 0)  passedNum[i] = true;
                for (int iTrig = 0; iTrig < nTriggersInNum; ++iTrig) {

                    int jTrig = indicesToReadNum[triggerIndices_perNum[i][iTrig]];
                    // triggers in the numerator are "OR"ed.
                    if (triggersNum[jTrig] > 0) {
                        passedNum[i] = true;
                        break;
                    }
                }
            }

            // prescales for denominator
            for (int i = 0; i < nPrescaleDenom; ++i) {
                noPrescaleDenom[i] = false;
                // check if this numerator has prescale = 1
                int iPrescale = indicesPrescaleToReadDenom[i];
                noPrescaleDenom[i] = (prescalesDenom[iPrescale] == 1);
            }

            // prescales for numerator
            for (int i = 0; i < nPrescaleNum; ++i) {
                noPrescaleNum[i] = false;
                // check if this numerator has prescale = 1
                int iPrescale = indicesPrescaleToReadNum[i];
                noPrescaleNum[i] = (prescalesNum[iPrescale] == 1);
            }

            int phoIndex = -1;
            float maxPt = -1;
            float phoEta = -999;
            float phoPhi = -999;
            int subdet = -1;
            for (int i=0; i<ggHi.nPho; ++i) {

                if (!((*ggHi.phoSigmaIEtaIEta)[i] > 0.002 && (*ggHi.pho_swissCrx)[i] < 0.9 && TMath::Abs((*ggHi.pho_seedTime)[i]) < 3)) continue;
                if (doEcalNoiseMask) {
                    if ((((*ggHi.phoE3x3)[i])/((*ggHi.phoE5x5)[i]) > 2./3.-0.03 &&
                            ((*ggHi.phoE3x3)[i])/((*ggHi.phoE5x5)[i]) < 2./3.+0.03) &&
                        (((*ggHi.phoE1x5)[i])/((*ggHi.phoE5x5)[i]) > 1./3.-0.03 &&
                            ((*ggHi.phoE1x5)[i])/((*ggHi.phoE5x5)[i]) < 1./3.+0.03) &&
                        (((*ggHi.phoE2x5)[i])/((*ggHi.phoE5x5)[i]) > 2./3.-0.03 &&
                            ((*ggHi.phoE2x5)[i])/((*ggHi.phoE5x5)[i]) < 2./3.+0.03)) continue;
                }

                // subdet = 0 if EB
                // subdet = 1 if EE
                int subdetTmp = ecalRegion((*ggHi.phoEta)[i]);
                if (subdetTmp == -1)  continue;

                // hOverE
                if (nCut_phoHoverE > 0) {
                    float tmpCut_phoHoverE = cut_phoHoverE[0];
                    if (nCut_phoHoverE == 2)
                        tmpCut_phoHoverE = cut_phoHoverE[subdetTmp];

                    if (!((*ggHi.phoHoverE)[i] < tmpCut_phoHoverE))  continue;
                }
                // ecal Iso
                if (nCut_pho_ecalClusterIsoR4 > 0) {
                    float tmpCut_pho_ecalClusterIsoR4 = cut_pho_ecalClusterIsoR4[0];
                    if (nCut_pho_ecalClusterIsoR4 == 2)
                        tmpCut_pho_ecalClusterIsoR4 = cut_pho_ecalClusterIsoR4[subdetTmp];

                    if (!((*ggHi.pho_ecalClusterIsoR4)[i] < tmpCut_pho_ecalClusterIsoR4))  continue;
                }
                // hcal Iso
                if (nCut_pho_hcalRechitIsoR4 > 0) {
                    float tmpCut_pho_hcalRechitIsoR4 = cut_pho_hcalRechitIsoR4[0];
                    if (nCut_pho_hcalRechitIsoR4 == 2)
                        tmpCut_pho_hcalRechitIsoR4 = cut_pho_hcalRechitIsoR4[subdetTmp];

                    if (!((*ggHi.pho_hcalRechitIsoR4)[i] < tmpCut_pho_hcalRechitIsoR4))  continue;
                }
                // track Iso
                if (nCut_pho_trackIsoR4PtCut20 > 0) {
                    float tmpCut_pho_trackIsoR4PtCut20 = cut_pho_trackIsoR4PtCut20[0];
                    if (nCut_pho_trackIsoR4PtCut20 == 2)
                        tmpCut_pho_trackIsoR4PtCut20 = cut_pho_trackIsoR4PtCut20[subdetTmp];

                    if (!((*ggHi.pho_trackIsoR4PtCut20)[i] < tmpCut_pho_trackIsoR4PtCut20))  continue;
                }
                // sumIso
                if (nCut_sumIso > 0) {
                    float tmpCut_sumIso = cut_sumIso[0];
                    if (nCut_sumIso == 2)
                        tmpCut_sumIso = cut_sumIso[subdetTmp];

                    if (!(((*ggHi.pho_ecalClusterIsoR4)[i] +
                            (*ggHi.pho_hcalRechitIsoR4)[i]  +
                            (*ggHi.pho_trackIsoR4PtCut20)[i]) < tmpCut_sumIso))   continue;
                }
                // phoSigmaIEtaIEta
                if (nCut_phoSigmaIEtaIEta_2012 > 0) {
                    float tmpCut_phoSigmaIEtaIEta_2012 = cut_phoSigmaIEtaIEta_2012[0];
                    if (nCut_phoSigmaIEtaIEta_2012 == 2)
                        tmpCut_phoSigmaIEtaIEta_2012 = cut_phoSigmaIEtaIEta_2012[subdetTmp];

                    if (!((*ggHi.phoSigmaIEtaIEta_2012)[i] < tmpCut_phoSigmaIEtaIEta_2012))  continue;
                }

                if ((*ggHi.phoEt)[i] > maxPt) {
                    phoIndex = i;
                    maxPt = (*ggHi.phoEt)[i];
                    phoEta = (*ggHi.phoEta)[i];
                    phoPhi = (*ggHi.phoPhi)[i];
                    subdet = subdetTmp;
                }
            }

            // leading photon goes into histograms
            if(maxPt == -1) continue;
            entriesAnalyzed++;

            // fill h[kPT][iEta][]
            for (int iEta = 0; iEta < nBins_eta; ++iEta)  {
                if (bins_eta[0][iEta] <= phoEta && (bins_eta[1][iEta] == -1 || bins_eta[1][iEta] > phoEta)) {

                    for (int i = 0; i < nDenom; ++i) {

                        int iPreScaleDenom = i;
                        if (nPrescaleDenom == 1) iPreScaleDenom = 0;

                        // this block will work only if nDenom = nNum, does not work if nDenom = 1 < nNum
                        int iPreScale = i;
                        if (nPrescaleNum == 1) iPreScale = 0;

                        if (passedDenom[i] && noPrescaleDenom[iPreScaleDenom] && noPrescaleNum[iPreScale]) {
                            h[kPT][iEta][i]->Fill(maxPt);
                        }
                    }
                    for (int i = 0; i < nNum; ++i) {

                        int iDenom = i;
                        if (nDenom == 1) iDenom = 0;

                        int iPreScaleDenom = iDenom;
                        if (nPrescaleDenom == 1) iPreScaleDenom = 0;

                        int iPreScale = i;
                        if (nPrescaleNum == 1) iPreScale = 0;

                        if (!(passedDenom[i] && noPrescaleDenom[iPreScaleDenom] && noPrescaleNum[iPreScale]))  continue;

                        if (passedNum[i]) {
                            h_Num[kPT][iEta][i]->Fill(maxPt);
                        }
                    }
                }
            }

            // fill h[kETA][0][]
            for (int i = 0; i < nNum; ++i) {
                if (maxPt > ptThresholds[i]) {

                    int iDenom = i;
                    if (nDenom == 1) iDenom = 0;

                    int iPreScaleDenom = iDenom;
                    if (nPrescaleDenom == 1) iPreScaleDenom = 0;

                    int iPreScale = i;
                    if (nPrescaleNum == 1) iPreScale = 0;

                    if (passedDenom[iDenom] && noPrescaleDenom[iPreScaleDenom] && noPrescaleNum[iPreScale]) {
                        h[kETA][0][i]->Fill(phoEta);
                    }
                    else continue;

                    if (passedNum[i]) {
                        h_Num[kETA][0][i]->Fill(phoEta);
                    }
                }
            }

            // fill h2D_etaphi[]
            for (int i = 0; i < nNum; ++i) {
                if (maxPt > ptThresholds[i]) {

                    int iDenom = i;
                    if (nDenom == 1) iDenom = 0;

                    int iPreScaleDenom = iDenom;
                    if (nPrescaleDenom == 1) iPreScaleDenom = 0;

                    int iPreScale = i;
                    if (nPrescaleNum == 1) iPreScale = 0;

                    if (passedDenom[iDenom] && noPrescaleDenom[iPreScaleDenom] && noPrescaleNum[iPreScale]) {
                        h2D_etaphi[i]->Fill(phoEta, phoPhi);
                    }
                    else continue;

                    if (passedNum[i]) {
                        h2D_etaphi_Num[i]->Fill(phoEta, phoPhi);
                    }
                }
            }

            // HLT energy scale
            // it is not necessary to require noPrescale for this case
            if (doHLTObject) {
                for (int iHLT = 0; iHLT < nTriggersNum; ++iHLT){

                    // do HLT scale for events whose denominator passes
                    int iNum = triggerBranchesNumIndices[iHLT];
                    int iDenom = iNum;
                    if (nDenom == 1) iDenom = 0;
                    if (!passedDenom[iDenom])  continue;

                    if (existsHLTObject[iHLT]) {
                        treeHLTObject[iHLT]->GetEntry(j_entry);

                        int nObjects = hltObj[iHLT].pt->size();
                        float pt_HLT = -1;
                        float eta_HLT = -999;
                        float phi_HLT = -999;
                        for (int iObj = 0; iObj < nObjects; ++iObj) {
                            if ((*hltObj[iHLT].pt)[iObj] > pt_HLT) {
                                pt_HLT = (*hltObj[iHLT].pt)[iObj];
                                eta_HLT = (*hltObj[iHLT].eta)[iObj];
                                phi_HLT = (*hltObj[iHLT].phi)[iObj];
                            }
                        }
                        if (pt_HLT > -1) {
                            double dR = getDR(phoEta, phoPhi, eta_HLT, phi_HLT);
                            if (dR < 0.1) {
                                h2D_scaleHLT[iHLT][subdet]->Fill(maxPt, pt_HLT/maxPt);

                                if (doHLTObject_phoVar) {
                                    h2D_scaleHLT_phoVar[iHLT][PHOVAR_DEP::kSCEtaWidth][subdet]->Fill(
                                            (*ggHi.phoSCEtaWidth)[phoIndex], pt_HLT/maxPt);
                                    h2D_scaleHLT_phoVar[iHLT][PHOVAR_DEP::kSCPhiWidth][subdet]->Fill(
                                            (*ggHi.phoSCPhiWidth)[phoIndex], pt_HLT/maxPt);
                                    h2D_scaleHLT_phoVar[iHLT][PHOVAR_DEP::kSCBrem][subdet]->Fill(
                                            (*ggHi.phoSCBrem)[phoIndex], pt_HLT/maxPt);
                                    h2D_scaleHLT_phoVar[iHLT][PHOVAR_DEP::kR9][subdet]->Fill(
                                            (*ggHi.phoR9)[phoIndex], pt_HLT/maxPt);
                                    h2D_scaleHLT_phoVar[iHLT][PHOVAR_DEP::ksumIso][subdet]->Fill(
                                            ((*ggHi.pho_ecalClusterIsoR4)[phoIndex] + (*ggHi.pho_hcalRechitIsoR4)[phoIndex] + (*ggHi.pho_trackIsoR4PtCut20)[phoIndex]),
                                            pt_HLT/maxPt);
                                }
                            }
                        }
                    }
                }
            }
        }
        fileTmp->Close();
    }
    std::cout<<  "Loop ENDED : " << treePath.c_str() <<std::endl;
    std::cout << "entries                  = " << entries << std::endl;
    std::cout << "duplicateEntries         = " << duplicateEntries << std::endl;
    std::cout << "entriesPassedDenomGlobal = " << entriesPassedDenomGlobal << std::endl;
    std::cout << "entriesAnalyzed          = " << entriesAnalyzed << std::endl;

    TFile* output = TFile::Open(outputFile.Data(),"RECREATE");
    output->cd();

    // save histograms
    for (int iDep = 0; iDep < kN_EFF_DEP; ++iDep) {
        for (int iEta = 0; iEta < nBins_eta; ++iEta)  {
            for (int i = 0; i < nNum; ++i) {

                if (h_Init[iDep][iEta][i])  h[iDep][iEta][i]->Write("",TObject::kOverwrite);
                if (h_Num_Init[iDep][iEta][i])  h_Num[iDep][iEta][i]->Write("",TObject::kOverwrite);
            }
        }
    }

    TH2D* h2D_etaphi_Num_ratio[nNum];
    for (int i = 0; i < nNum; ++i) {
        std::string tmpHistName = Form("%s_ratio", h2D_etaphi_Num[i]->GetName());
        h2D_etaphi_Num_ratio[i] = (TH2D*)h2D_etaphi_Num[i]->Clone(tmpHistName.c_str());
        std::string tmpHistTitle = h2D_etaphi_Num_ratio[i]->GetTitle();
        tmpHistTitle += " - Eff.";
        h2D_etaphi_Num_ratio[i]->SetTitle(tmpHistTitle.c_str());

        h2D_etaphi_Num_ratio[i]->Divide(h2D_etaphi[i]);

        h2D_etaphi_Num_ratio[i]->SetMaximum(1);
        h2D_etaphi_Num_ratio[i]->SetMinimum(0);
    }

    for (int i = 0; i < nNum; ++i) {
        h2D_etaphi[i]->Write("",TObject::kOverwrite);
        h2D_etaphi_Num[i]->Write("",TObject::kOverwrite);
        h2D_etaphi_Num_ratio[i]->Write("",TObject::kOverwrite);
    }

    for (int i = 0; i < nTriggersNum; ++i) {
        for (int iSubdet = 0; iSubdet < 2; ++iSubdet) {
            h2D_scaleHLT[i][iSubdet]->Write("",TObject::kOverwrite);
        }
    }

    if (doHLTObject_phoVar) {
        for (int i = 0; i < nTriggersNum; ++i) {
            for (int iDep = 0; iDep < PHOVAR_DEP::kN_PHOVAR_DEP; ++iDep) {
                for (int iSubdet = 0; iSubdet < 2; ++iSubdet) {
                    h2D_scaleHLT_phoVar[i][iDep][iSubdet]->Write("",TObject::kOverwrite);
                }
            }
        }
    }

    // create 1D versions of energy scale histograms
    TH1D* h_scaleHLT_projY[nTriggersNum][2];
    TH1D* h_scaleHLT_eScale[nTriggersNum][2];
    TH1D* h_scaleHLT_eRes[nTriggersNum][2];
    for (int i = 0; i < nTriggersNum; ++i) {
        for (int iSubdet = 0; iSubdet < 2; ++iSubdet) {

            std::string tmpRefName = h2D_scaleHLT[i][iSubdet]->GetName();
            std::string tmpTitle = h2D_scaleHLT[i][iSubdet]->GetTitle();
            double tmpYmin = h2D_scaleHLT[i][iSubdet]->GetYaxis()->GetXmin();
            double tmpYmax = h2D_scaleHLT[i][iSubdet]->GetYaxis()->GetXmax();
            std::string tmpHistName = "";
            std::string yTitle = "";
            // h_scaleHLT_projY[][]
            tmpHistName = replaceAll(tmpRefName, "h2D_", "h_");
            tmpHistName += "_projY";
            h_scaleHLT_projY[i][iSubdet] = (TH1D*)h2D_scaleHLT[i][iSubdet]->ProjectionY(tmpHistName.c_str());
            h_scaleHLT_projY[i][iSubdet]->SetMarkerStyle(kFullCircle);

            h_scaleHLT_projY[i][iSubdet]->Write("",TObject::kOverwrite);

            // h_scaleHLT_eScale[][]
            TObjArray aSlices;
            int nBinsX = h2D_scaleHLT[i][iSubdet]->GetXaxis()->GetNbins();
            int binsXmerge = nBinsX/4;
            h2DTmp = (TH2D*)h2D_scaleHLT[i][iSubdet]->RebinX(binsXmerge, Form("%s_tmp", tmpRefName.c_str()));
            h2DTmp->FitSlicesY(0,0,-1,0,"Q LL m", &aSlices);

            tmpHistName = replaceAll(tmpRefName, "h2D_", "h_eScale_");
            h_scaleHLT_eScale[i][iSubdet] = (TH1D*)aSlices.At(1)->Clone(tmpHistName.c_str());
            h_scaleHLT_eScale[i][iSubdet]->SetTitle(tmpTitle.c_str());
            yTitle = Form ("< %s >", h2D_scaleHLT[i][iSubdet]->GetYaxis()->GetTitle());
            h_scaleHLT_eScale[i][iSubdet]->SetYTitle(yTitle.c_str());
            h_scaleHLT_eScale[i][iSubdet]->SetAxisRange(tmpYmin, tmpYmax, "Y");
            h_scaleHLT_eScale[i][iSubdet]->SetMarkerStyle(kFullCircle);
            h_scaleHLT_eScale[i][iSubdet]->Write("",TObject::kOverwrite);

            // h_scaleHLT_eRes[][]
            tmpHistName = replaceAll(tmpRefName, "h2D_", "h_eRes_");
            h_scaleHLT_eRes[i][iSubdet] = (TH1D*)aSlices.At(2)->Clone(tmpHistName.c_str());
            h_scaleHLT_eRes[i][iSubdet]->SetTitle(tmpTitle.c_str());
            yTitle = Form ("#sigma(  %s )", h2D_scaleHLT[i][iSubdet]->GetYaxis()->GetTitle());
            h_scaleHLT_eRes[i][iSubdet]->SetYTitle(yTitle.c_str());
            h_scaleHLT_eRes[i][iSubdet]->SetAxisRange(0, 0.45, "Y");
            h_scaleHLT_eRes[i][iSubdet]->SetMarkerStyle(kFullCircle);
            h_scaleHLT_eRes[i][iSubdet]->Write("",TObject::kOverwrite);
        }
    }

    // create efficiency graphs
    TGraphAsymmErrors* a[kN_EFF_DEP][nBins_eta][nNum];
    bool a_Init[kN_EFF_DEP][nBins_eta][nNum];
    for (int iDep = 0; iDep < kN_EFF_DEP; ++iDep) {
        for (int iEta = 0; iEta < nBins_eta; ++iEta)  {
            for (int i = 0; i < nNum; ++i) {

                a_Init[iDep][iEta][i] = false;

                int iDenom = i;
                if (nDenom == 1 && iDep == EFF_DEP::kPT)  iDenom = 0;
                if (!h_Init[iDep][iEta][iDenom])  continue;
                if (!h_Num_Init[iDep][iEta][i]) continue;

                std::string tmpHistName = h_Num[iDep][iEta][i]->GetName();
                std::string tmpGraphName = Form("a_%s", tmpHistName.substr(2).c_str());
                std::string tmpTitle = h_Num[iDep][iEta][i]->GetTitle();

                a_Init[iDep][iEta][i] = true;
                a[iDep][iEta][i] = new TGraphAsymmErrors();
                a[iDep][iEta][i]->SetName(tmpGraphName.c_str());
                a[iDep][iEta][i]->SetTitle(tmpTitle.c_str());
                a[iDep][iEta][i]->GetYaxis()->SetTitle("Efficiency");
                a[iDep][iEta][i]->BayesDivide(h_Num[iDep][iEta][i], h[iDep][iEta][iDenom]);
                a[iDep][iEta][i]->SetMarkerStyle(kFullCircle);

                a[iDep][iEta][i]->Write("",TObject::kOverwrite);
            }
        }
    }

    ///// plotting /////
    TCanvas* c = 0;
    TLegend* leg = 0;
    TLine* line = 0;
    TLatex* latex = 0;
    // assume not more than 13 curves are drawn.

    for (int iDep = 0; iDep < kN_EFF_DEP; ++iDep) {
        for (int iEta = 0; iEta < nBins_eta; ++iEta)  {

            c = new TCanvas("cvn_photonTrigger","",windowWidth,windowHeight);
            setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
            setCanvasFinal(c);
            c->cd();

            for (int i = 0; i < nNum; ++i) {

                if (a_Init[iDep][iEta][i])  {
                    std::string tmpGraphName = a[iDep][iEta][i]->GetName();
                    std::string tmpCanvasName = Form("cvn_TrigEff_%s", tmpGraphName.substr(2).c_str());
                    std::string strRemoveTmp = Form("_Num%d", i);
                    tmpCanvasName = replaceAll(tmpCanvasName, strRemoveTmp, "");

                    c->SetName(tmpCanvasName.c_str());

                    break;      // canvas name is set, rest of the loop is not necessary.
                }
            }

            // base histogram on which other histograms will be plotted.
            hTmp = (TH1D*)h[iDep][0][0]->Clone("h_dummy");
            // cosmetics before drawing the turn on curves
            hTmp->Reset();
            hTmp->SetTitle("");
            hTmp->SetYTitle("Efficiency");
            hTmp->SetMinimum(yMin);  // default is 0.
            float yMaxTmp = yMax;
            if (yMaxTmp == 0) {
                if (nNum > 4)  yMaxTmp = 1.6;
                else if (nNum > 2)  yMaxTmp = 1.4;
                else  yMaxTmp = 1.3;
            }
            hTmp->SetMaximum(yMaxTmp);
            setTH1_photonTrigger(hTmp, c);
            hTmp->SetTitleOffset(titleOffsetX, "X");
            hTmp->SetTitleOffset(titleOffsetY, "Y");
            // scale title size with the size of Canvas
            hTmp->Draw();

            // draw line y = 1
            line = new TLine(hTmp->GetXaxis()->GetXmin(), 1, hTmp->GetXaxis()->GetXmax(),1);
            line->SetLineStyle(kDashed);
            line->DrawClone();

            if (iDep == EFF_DEP::kETA) {
                // draw lines for ECAL transition region
                std::vector<float> lineXvalues {-1*ECAL_boundary_1, ECAL_boundary_1, -1*ECAL_boundary_2, ECAL_boundary_2};
                for (std::vector<float>::const_iterator itLine = lineXvalues.begin(); itLine !=lineXvalues.end(); ++itLine) {

                    line = new TLine((*itLine), 0, (*itLine), 1);
                    line->SetLineStyle(kDashed);
                    line->SetLineWidth(lineWidth);
                    line->DrawClone();
                }
            }

            leg = new TLegend();
            // leg->SetHeader("Triggers");

            for (int i = 0; i < nNum; ++i) {

                if (!a_Init[iDep][iEta][i])  continue;

                a[iDep][iEta][i]->SetMarkerColor(GRAPHICS::colors[i]);
                a[iDep][iEta][i]->SetLineColor(GRAPHICS::colors[i]);
                a[iDep][iEta][i]->SetMarkerSize(markerSize);
                a[iDep][iEta][i]->Draw("p e");

                if (iDep == EFF_DEP::kPT) {
                    line = new TLine(ptThresholdsOrig[i], 0, ptThresholdsOrig[i], 1);
                    line->SetLineStyle(kDashed);
                    line->SetLineColor(a[iDep][iEta][i]->GetMarkerColor());
                    line->SetLineWidth(lineWidth);
                    line->DrawClone();
                }

                int iTrig0 = triggerIndices_perNum[i][0];
                // 1st trigger in the numerator goes into the legend
                std::string legEntry = triggerBranchesNum.at(iTrig0);
                if (iDep == EFF_DEP::kETA)  legEntry += Form(" , p_{T} > %.0f", ptThresholds[i]);
                leg->AddEntry(a[iDep][iEta][i], legEntry.c_str(), "lp");
            }

            double height = calcTLegendHeight(leg);
            double width = calcTLegendWidth(leg);
            setLegendPosition(leg, legendPosition, c, height, width);
            leg->Draw();

            // print eta cut
            if (iDep == EFF_DEP::kPT) {
                latex = new TLatex();

                setTextAlignment(latex, "NE");

                std::string textEta = prepareTextEta(bins_eta[0].at(iEta), bins_eta[1].at(iEta));
                std::vector<std::string> textLinesTmp;
                textLinesTmp.push_back(textEta);

                int nTextLinesTmp = textLinesTmp.size();
                float textOffsetXTmp = 0.08;
                float textOffsetYTmp = 0.08;
                std::vector<std::pair<float,float>> textCoordinates = calcTextCoordinates(textLinesTmp, "NE", c, textOffsetXTmp, textOffsetYTmp);
                for (int i = 0; i<nTextLinesTmp; ++i){
                    float x = textCoordinates.at(i).first;
                    float y = textCoordinates.at(i).second;
                    if (textLinesTmp.at(i) != CONFIGPARSER::nullInput.c_str())
                        latex->DrawLatexNDC(x, y, textLinesTmp.at(i).c_str());
                }
            }

            c->Write("",TObject::kOverwrite);

            // save canvas as picture if a figure name is provided.
            if (! outputFigureName.EqualTo("")) {

                c->SaveAs(Form("%s.C", outputFigureName.Data()));
                c->SaveAs(Form("%s.png", outputFigureName.Data()));
                c->SaveAs(Form("%s.pdf", outputFigureName.Data()));
            }
            c->Close();
        }
    }

    for (int i = 0; i < nNum; ++i) {
        c = new TCanvas("cnv_","",windowWidth,windowHeight);
        // do not put margin for the TH2D canvas
        setCanvasFinal(c);
        c->cd();

        std::string tmpHistName = h2D_etaphi_Num_ratio[i]->GetName();
        std::string tmpCanvasName = Form("cvn_%s", tmpHistName.substr(4).c_str());
        c->SetName(tmpCanvasName.c_str());

        // do not offset title for the TH2D canvas
        setTH1_photonTrigger(h2D_etaphi_Num_ratio[i], c);
        h2D_etaphi_Num_ratio[i]->Draw("colz");

        // draw lines for ECAL transition region
        std::vector<float> lineXvalues {-1*ECAL_boundary_1, ECAL_boundary_1, -1*ECAL_boundary_2, ECAL_boundary_2};
        for (std::vector<float>::const_iterator itLine = lineXvalues.begin(); itLine !=lineXvalues.end(); ++itLine) {

            double yMin_h2D = h2D_etaphi_Num_ratio[i]->GetYaxis()->GetXmin();
            double yMax_h2D = h2D_etaphi_Num_ratio[i]->GetYaxis()->GetXmax();

            line = new TLine((*itLine), yMin_h2D, (*itLine), yMax_h2D);
            line->SetLineStyle(kDashed);
            line->SetLineWidth(lineWidth);
            line->DrawClone();
        }

        c->Write("",TObject::kOverwrite);

        // save canvas as picture if a figure name is provided.
        if (! outputFigureName.EqualTo("")) {

            c->SaveAs(Form("%s.C", outputFigureName.Data()));
            c->SaveAs(Form("%s.png", outputFigureName.Data()));
            c->SaveAs(Form("%s.pdf", outputFigureName.Data()));
        }
        c->Close();
    }

    // h2D_scaleHLT[][]
    for (int i = 0; i < nTriggersNum; ++i) {
        for (int iSubdet = 0; iSubdet < 2; ++iSubdet) {
        c = new TCanvas("cnv_","",windowWidth,windowHeight);
        setCanvasMargin(c, leftMargin, 0.15, bottomMargin, 0.15);
        setCanvasFinal(c);
        c->cd();

        std::string tmpHistName = h2D_scaleHLT[i][iSubdet]->GetName();
        std::string tmpCanvasName = Form("cvn_%s", tmpHistName.substr(4).c_str());
        c->SetName(tmpCanvasName.c_str());

        // do not offset title for the TH2D canvas
        setTH1_photonTrigger(h2D_scaleHLT[i][iSubdet], c);
        h2D_scaleHLT[i][iSubdet]->SetTitleOffset(1.25, "X");
        h2D_scaleHLT[i][iSubdet]->SetTitleOffset(1.25, "Y");
        // h2D_scaleHLT[i][iSubdet]->Scale(1./(double)h2D_scaleHLT[i][iSubdet]->Integral());
        h2D_scaleHLT[i][iSubdet]->Draw("colz");

        // draw line at energy scale = 1
        double xMin_h2D = h2D_scaleHLT[i][iSubdet]->GetXaxis()->GetXmin();
        double xMax_h2D = h2D_scaleHLT[i][iSubdet]->GetXaxis()->GetXmax();

        line = new TLine(xMin_h2D, 1, xMax_h2D, 1);
        line->SetLineStyle(kDashed);
        line->SetLineWidth(lineWidth);
        line->DrawClone();

        // draw line above which the integral is the 95% of the total integral
        std::vector<float> fractions{0.95, 0.99};
        for (std::vector<float>::const_iterator itFrac = fractions.begin(); itFrac !=fractions.end(); ++itFrac) {

            int binFrac = getBin4IntegralFraction(h2D_scaleHLT[i][iSubdet], (*itFrac), "Y");
            if (binFrac < 0) continue;
            double yFrac = h2D_scaleHLT[i][iSubdet]->GetYaxis()->GetBinLowEdge(binFrac);

            line = new TLine(xMin_h2D, yFrac, xMax_h2D, yFrac);
            line->SetLineStyle(kDotted);
            line->SetLineWidth(lineWidth);
            line->DrawClone();
        }

        // extract fraction of entries with energy scale < 1.
        double integral = h2D_scaleHLT[i][iSubdet]->Integral();
        double binYequals1 = h2D_scaleHLT[i][iSubdet]->GetYaxis()->FindBin(1);
        double fractionBelow1 = h2D_scaleHLT[i][iSubdet]->Integral(0, -1, 1, binYequals1) / integral;

        latex = new TLatex();

        setTextAlignment(latex, "NE");

        std::string textTmp = Form ("#frac{y < 1}{total} = %.0f%%", fractionBelow1*100);
        std::vector<std::string> textLinesTmp;
        textLinesTmp.push_back(textTmp);

        latex->SetTextSize(latex->GetTextSize()*0.5);
        float textOffsetXTmp = 0.04;
        float textOffsetYTmp = 0.08;
        drawTextLines(latex, c, textLinesTmp, "NE", textOffsetXTmp, textOffsetYTmp);

        c->Write("",TObject::kOverwrite);

        // save canvas as picture if a figure name is provided.
        if (! outputFigureName.EqualTo("")) {

            c->SaveAs(Form("%s.C", outputFigureName.Data()));
            c->SaveAs(Form("%s.png", outputFigureName.Data()));
            c->SaveAs(Form("%s.pdf", outputFigureName.Data()));
        }
        c->Close();
    }
    }

    if (doHLTObject_phoVar) {
    for (int i = 0; i < nTriggersNum; ++i) {
        for (int iDep = 0; iDep < PHOVAR_DEP::kN_PHOVAR_DEP; ++iDep) {
            for (int iSubdet = 0; iSubdet < 2; ++iSubdet) {
                c = new TCanvas("cnv_","",windowWidth,windowHeight);
                setCanvasMargin(c, leftMargin, 0.15, bottomMargin, 0.15);
                setCanvasFinal(c);
                c->cd();

                std::string tmpHistName = h2D_scaleHLT_phoVar[i][iDep][iSubdet]->GetName();
                std::string tmpCanvasName = Form("cvn_%s", tmpHistName.substr(4).c_str());
                c->SetName(tmpCanvasName.c_str());

                // do not offset title for the TH2D canvas
                setTH1_photonTrigger(h2D_scaleHLT_phoVar[i][iDep][iSubdet], c);
                h2D_scaleHLT_phoVar[i][iDep][iSubdet]->SetTitleOffset(1.25, "X");
                h2D_scaleHLT_phoVar[i][iDep][iSubdet]->SetTitleOffset(1.25, "Y");
                // h2D_scaleHLT[i][iSubdet]->Scale(1./(double)h2D_scaleHLT[i][iSubdet]->Integral());
                h2D_scaleHLT_phoVar[i][iDep][iSubdet]->Draw("colz");

                // draw line at energy scale = 1
                double xMin_h2D = h2D_scaleHLT_phoVar[i][iDep][iSubdet]->GetXaxis()->GetXmin();
                double xMax_h2D = h2D_scaleHLT_phoVar[i][iDep][iSubdet]->GetXaxis()->GetXmax();

                line = new TLine(xMin_h2D, 1, xMax_h2D, 1);
                line->SetLineStyle(kDashed);
                line->SetLineWidth(lineWidth);
                line->DrawClone();

                // draw line above which the integral is the 95% of the total integral
                std::vector<float> fractions{0.95, 0.99};
                for (std::vector<float>::const_iterator itFrac = fractions.begin(); itFrac !=fractions.end(); ++itFrac) {

                    int binFrac = getBin4IntegralFraction(h2D_scaleHLT_phoVar[i][iDep][iSubdet], (*itFrac), "Y");
                    if (binFrac < 0) continue;
                    double yFrac = h2D_scaleHLT_phoVar[i][iDep][iSubdet]->GetYaxis()->GetBinLowEdge(binFrac);

                    line = new TLine(xMin_h2D, yFrac, xMax_h2D, yFrac);
                    line->SetLineStyle(kDotted);
                    line->SetLineWidth(lineWidth);
                    line->DrawClone();
                }

                // extract fraction of entries with energy scale < 1.
                double integral = h2D_scaleHLT_phoVar[i][iDep][iSubdet]->Integral();
                double binYequals1 = h2D_scaleHLT_phoVar[i][iDep][iSubdet]->GetYaxis()->FindBin(1);
                double fractionBelow1 = h2D_scaleHLT_phoVar[i][iDep][iSubdet]->Integral(0, -1, 1, binYequals1) / integral;

                latex = new TLatex();

                setTextAlignment(latex, "NE");

                std::string textTmp = Form ("#frac{y < 1}{total} = %.0f%%", fractionBelow1*100);
                std::vector<std::string> textLinesTmp;
                textLinesTmp.push_back(textTmp);

                latex->SetTextSize(latex->GetTextSize()*0.5);
                float textOffsetXTmp = 0.04;
                float textOffsetYTmp = 0.08;
                drawTextLines(latex, c, textLinesTmp, "NE", textOffsetXTmp, textOffsetYTmp);

                c->Write("",TObject::kOverwrite);

                // save canvas as picture if a figure name is provided.
                if (! outputFigureName.EqualTo("")) {

                    c->SaveAs(Form("%s.C", outputFigureName.Data()));
                    c->SaveAs(Form("%s.png", outputFigureName.Data()));
                    c->SaveAs(Form("%s.pdf", outputFigureName.Data()));
                }
                c->Close();
            }
        }
    }
    }

    // h_scaleHLT_projY[][]
    for (int i = 0; i < nTriggersNum; ++i) {
        for (int iSubdet = 0; iSubdet < 2; ++iSubdet) {
        c = new TCanvas("cnv_","",windowWidth,windowHeight);
        setCanvasMargin(c, leftMargin, 0.15, bottomMargin, 0.15);
        setCanvasFinal(c);
        c->cd();

        std::string tmpHistName = h_scaleHLT_projY[i][iSubdet]->GetName();
        std::string tmpCanvasName = Form("cvn_%s", tmpHistName.substr(2).c_str());
        c->SetName(tmpCanvasName.c_str());

        // do not offset title for the TH2D canvas
        setTH1_photonTrigger(h_scaleHLT_projY[i][iSubdet], c);
        h_scaleHLT_projY[i][iSubdet]->SetTitleOffset(1.25, "X");
        h_scaleHLT_projY[i][iSubdet]->SetTitleOffset(1.25, "Y");
        h_scaleHLT_projY[i][iSubdet]->Draw("e");

        c->Write("",TObject::kOverwrite);

        // save canvas as picture if a figure name is provided.
        if (! outputFigureName.EqualTo("")) {

            c->SaveAs(Form("%s.C", outputFigureName.Data()));
            c->SaveAs(Form("%s.png", outputFigureName.Data()));
            c->SaveAs(Form("%s.pdf", outputFigureName.Data()));
        }
        c->Close();
    }
    }

    //TH1D* h_scaleHLT_eScale[nTriggersNum][2];
    for (int i = 0; i < nTriggersNum; ++i) {
        for (int iSubdet = 0; iSubdet < 2; ++iSubdet) {
        c = new TCanvas("cnv_","",windowWidth,windowHeight);
        setCanvasMargin(c, leftMargin, 0.15, bottomMargin, 0.15);
        setCanvasFinal(c);
        c->cd();

        std::string tmpHistName = h_scaleHLT_eScale[i][iSubdet]->GetName();
        std::string tmpCanvasName = Form("cvn_%s", tmpHistName.substr(2).c_str());
        c->SetName(tmpCanvasName.c_str());

        // do not offset title for the TH2D canvas
        setTH1_photonTrigger(h_scaleHLT_eScale[i][iSubdet], c);
        h_scaleHLT_eScale[i][iSubdet]->SetTitleOffset(1.25, "X");
        h_scaleHLT_eScale[i][iSubdet]->SetTitleOffset(1.25, "Y");
        h_scaleHLT_eScale[i][iSubdet]->Draw("e");

        // draw line at energy scale = 1
        double xMin_h2D = h_scaleHLT_eScale[i][iSubdet]->GetXaxis()->GetXmin();
        double xMax_h2D = h_scaleHLT_eScale[i][iSubdet]->GetXaxis()->GetXmax();

        line = new TLine(xMin_h2D, 1, xMax_h2D, 1);
        line->SetLineStyle(kDashed);
        line->SetLineWidth(lineWidth);
        line->DrawClone();

        c->Write("",TObject::kOverwrite);

        // save canvas as picture if a figure name is provided.
        if (! outputFigureName.EqualTo("")) {

            c->SaveAs(Form("%s.C", outputFigureName.Data()));
            c->SaveAs(Form("%s.png", outputFigureName.Data()));
            c->SaveAs(Form("%s.pdf", outputFigureName.Data()));
        }
        c->Close();
    }
    }

    //TH1D* h_scaleHLT_eRes[nTriggersNum][2];
    for (int i = 0; i < nTriggersNum; ++i) {
        for (int iSubdet = 0; iSubdet < 2; ++iSubdet) {
        c = new TCanvas("cnv_","",windowWidth,windowHeight);
        setCanvasMargin(c, leftMargin, 0.15, bottomMargin, 0.15);
        setCanvasFinal(c);
        c->cd();

        std::string tmpHistName = h_scaleHLT_eRes[i][iSubdet]->GetName();
        std::string tmpCanvasName = Form("cvn_%s", tmpHistName.substr(2).c_str());
        c->SetName(tmpCanvasName.c_str());

        // do not offset title for the TH2D canvas
        setTH1_photonTrigger(h_scaleHLT_eRes[i][iSubdet], c);
        h_scaleHLT_eRes[i][iSubdet]->SetTitleOffset(1.25, "X");
        h_scaleHLT_eRes[i][iSubdet]->SetTitleOffset(1.25, "Y");
        h_scaleHLT_eRes[i][iSubdet]->Draw("e");

        c->Write("",TObject::kOverwrite);

        // save canvas as picture if a figure name is provided.
        if (! outputFigureName.EqualTo("")) {

            c->SaveAs(Form("%s.C", outputFigureName.Data()));
            c->SaveAs(Form("%s.png", outputFigureName.Data()));
            c->SaveAs(Form("%s.pdf", outputFigureName.Data()));
        }
        c->Close();
    }
    }

    std::cout<<"Closing the output file."<<std::endl;
    output->Close();
    std::cout<<"running photonTrigger() - END"<<std::endl;
}

int main(int argc, char** argv)
{
    if (argc == 5) {
        photonTrigger(argv[1], argv[2], argv[3], argv[4]);
        return 0;
    }
    else if (argc == 4) {
        photonTrigger(argv[1], argv[2], argv[3]);
        return 0;
    }
    else if (argc == 3) {
        photonTrigger(argv[1], argv[2]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./photonTrigger.exe <configFile> <inputFile> <outputFile> (<outputFigureName>)"
                << std::endl;
        return 1;
    }
}

int setBranchAdressTrigger(TTree* tree, std::string branch, int & branchVal)
{
    if (branch.size() == 0 || branch == CONFIGPARSER::nullInput) {
        branchVal = 1;
        return -2;
    }
    else if (tree->GetBranch(branch.c_str())) {
        tree->SetBranchStatus(branch.c_str(),1);
        tree->SetBranchAddress(branch.c_str(),&(branchVal));
        return 0;
    }
    else {
        std::cout<<"could not GetBranch() : "<<branch.c_str()<<std::endl;
        std::cout<<"default value set to  : "<<branch.c_str()<<" = 1"<<std::endl;
        branchVal = 1;
        return -1;
    }
}

/*
 * return the TTree* name of HLT object corresponding to the trigger bit
 * return empty string if nothing is found
 */
std::string getHLTObjectName(std::string triggerName)
{
    // should be an HLT object
    if (triggerName.find("HLT") != 0) return "";

    // trigger name ends with _v1, _v2, _v3, etc.
    // HLT object tree ends with _v. trim the number at the end
    // Ex. HLT bit is HLT_HISinglePhoton20_Eta1p5_v1
    // HLT object is HLT_HISinglePhoton20_Eta1p5_v
    std::string suffix = "_v";
    int suffixLen = suffix.size();

    return triggerName.substr(0, triggerName.rfind(suffix)+suffixLen);
}

std::string prepareTextEta(float etaLow, float etaUp)
{
    std::string str = "";
    if (etaLow <= 0 && etaUp > 0)
        str  = Form("|#eta| < %.2f", etaUp);
    else if (etaLow > 0 && etaUp > 0)
        str  = Form("%.2f < |#eta| < %.2f", etaLow, etaUp);

    return str;
}

/*
 * extract pt threshold from the name of trigger
 * Ex. "HLT_HISinglePhoton15_Eta3p1_v5" should return 15
 */
float extractPtThreshold(std::string triggerName)
{
    std::string strSub = "Photon";
    if (triggerName.find("L1") == 0) {
        // this is L1 trigger, look for EG
        strSub = "EG";
    }
    int len = strSub.size();
    size_t indexStart = triggerName.find(strSub.c_str()) + len;

    std::string strXX = triggerName.substr(indexStart, 2);

    std::istringstream sin(strXX);

    float res;
    sin >> res;

    return res;
}

/*
 * extract eta threshold from the name of trigger
 * Ex. "HLT_HISinglePhoton15_Eta3p1_v5" should return 3.1
 */
float extractEtaThreshold(std::string triggerName)
{
    std::string strSub = "Eta";
    int len = strSub.size();
    size_t indexStart = triggerName.find(strSub.c_str()) + len;

    std::string strXpY_X = triggerName.substr(indexStart, 1);
    std::string strXpY_Y = triggerName.substr(indexStart+2, 1);

    std::istringstream sinX(strXpY_X);
    std::istringstream sinY(strXpY_Y);

    float resX, resY;
    sinX >> resX;
    sinY >> resY;

    return resX+(0.1*resY);
}

/*
 * the offset will be used for trigger efficiency as func. of eta, eta-phi
 */
float getOffset4PtThreshold(float ptThreshold)
{
    if (ptThreshold >= 40)  return 5;
    else if (ptThreshold >= 20)  return 4;
    else if (ptThreshold >= 10)  return 3;
    else if (ptThreshold >= 5)  return 2;
    return 0;
}

/*
 * return 0 if EB
 * return 1 if EB
 * return -1 otherwise
 */
int ecalRegion(float eta)
{
    if (TMath::Abs(eta) < ECAL_boundary_1)  return 0;
    else if (TMath::Abs(eta) > ECAL_boundary_2) return 1;

    return -1;
}

void setTH1_photonTrigger(TH1* h, TCanvas* c)
{
    h->SetStats(false);
    h->GetXaxis()->CenterTitle();
    h->GetYaxis()->CenterTitle();
    // scale title size with the size of Canvas
    if (c != 0) {
        float windowHeight = c->GetWh();
        float titleSizeX = h->GetTitleSize("X")*windowHeight/((float)INPUT_DEFAULT::windowHeight);
        float titleSizeY = h->GetTitleSize("Y")*windowHeight/((float)INPUT_DEFAULT::windowHeight);
        h->SetTitleSize(titleSizeX, "X");
        h->SetTitleSize(titleSizeY, "Y");
    }
}

/*
 *  return the bin after which the integral of the histogram is the "fraction" of the integral over all bins
 *  If the fraction falls between the bin edges (likely to be the case), pick the lower bin.
 *  Technically, it is the highest bin such that (integral after the bin) >= fraction * (integral over all bins)
 *  "direction" is the axis along which the integral is calculated.
 */
int getBin4IntegralFraction(TH2D* h2D, float fraction, std::string direction)
{
    int binFirst = -1;
    int binLast = -1;
    int nBins = -1;
    if (direction == "X") {
        binFirst = h2D->GetYaxis()->GetFirst();
        binLast = h2D->GetYaxis()->GetLast();

        nBins = h2D->GetXaxis()->GetNbins();
    }
    else if (direction == "Y") {
        binFirst = h2D->GetXaxis()->GetFirst();
        binLast = h2D->GetXaxis()->GetLast();

        nBins = h2D->GetYaxis()->GetNbins();
    }
    else return -1;

    double integral = h2D->Integral();
    int bin = -1;
    for (int iBin = nBins; iBin > 0; --iBin) {
        double integralTmp = -1;
        if (direction == "X")  integralTmp = h2D->Integral(iBin, nBins, binFirst, binLast);
        else if (direction == "Y")  integralTmp = h2D->Integral(binFirst, binLast, iBin, nBins);
        if (integralTmp/integral >= fraction)  {
            bin = iBin;
            break;
        }
    }

    return bin;
}
