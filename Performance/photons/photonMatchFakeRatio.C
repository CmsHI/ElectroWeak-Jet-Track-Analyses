/*
 * macro to draw eta, GEN Pt, RECO Pt and centrality dependent matching efficiency and fake rate plots for photons.
 * The macro can make 4 types of match ratio histograms
 *  1. x-axis is eta.
 *  2. x-axis is GEN Pt
 *  3. x-axis is RECO Pt.
 *  4. x-axis is hiBin (centrality)
 *  Each match ratio histogram has a corresponding numerator and denominator histogram.
 * saves histograms to a .root file.
 */

#include <TFile.h>
#include <TChain.h>
#include <TH1.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TLine.h>

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include "../../CorrelationTuple/EventMatcher.h"
#include "../../TreeHeaders/CutConfigurationTree.h"
#include "../../TreeHeaders/ggHiNtuplizerTree.h"
#include "../../Utilities/interface/CutConfigurationParser.h"
#include "../../Utilities/interface/InputConfigurationParser.h"
#include "../../Utilities/interface/HiForestInfoController.h"
#include "../../Utilities/styleUtil.h"
#include "../../Utilities/th1Util.h"
#include "../interface/matchRatioHist.h"

enum MATCHRATIO_TYPE {   // matching ratio histogram type
    kMATCH,
    kFAKE
};

void photonMatchFakeRatio(const TString configFile, const TString inputFile, const TString outputFile = "photonMatchFakeRatio.root");

void photonMatchFakeRatio(const TString configFile, const TString inputFile, const TString outputFile)
{
    std::cout<<"running photonMatchFakeRatio()"<<std::endl;
    std::cout<<"configFile  = "<< configFile.Data() <<std::endl;
    std::cout<<"inputFile   = "<< inputFile.Data()  <<std::endl;
    std::cout<<"outputFile  = "<< outputFile.Data() <<std::endl;

    InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

    // input configuration
    // input for TTree
    std::string treePath;
    int collisionType;

    // input for TH1
    std::vector<std::vector<float>> TH1D_Bins_List;      // nBins, xLow, xUp for the TH1D histogram
    float titleOffsetX;
    float titleOffsetY;
    float yMin;         // min. y-axis value of matching efficiency / fake rate histogram, default : 0
    float yMax;         // max. y-axis value of matching efficiency / fake rate histogram, default : 1.2

    // input for TCanvas
    int windowWidth;
    int windowHeight;
    float leftMargin;
    float rightMargin;
    float bottomMargin;
    float topMargin;

    if (configInput.isValid) {
        treePath  = configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treePath];
    
        collisionType = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_collisionType];

        TH1D_Bins_List = ConfigurationParser::ParseListTH1D_Bins(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH1D_Bins_List]);
        titleOffsetX = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_titleOffsetX];
        titleOffsetY = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_titleOffsetY];
        yMin = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_TH1_yMin];
        yMax = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_TH1_yMax];

        windowWidth = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_windowWidth];
        windowHeight = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_windowHeight];
        leftMargin   = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_leftMargin];
        rightMargin  = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_rightMargin];
        bottomMargin = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_bottomMargin];
        topMargin    = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_topMargin];
    }
    else {
        treePath = "";
    
        collisionType = COLL::kPP;

        TH1D_Bins_List.resize(3);
        TH1D_Bins_List[0].push_back(12);     // nBins
        TH1D_Bins_List[1].push_back(-2.4);   // xLow
        TH1D_Bins_List[2].push_back(2.4);    // xUp
        titleOffsetX = 1;
        titleOffsetY = 1;
        yMin = 0;
        yMax = -1;

        windowWidth = 0;
        windowHeight = 0;
        leftMargin = 0.1;
        rightMargin = 0.1;
        bottomMargin = 0.1;
        topMargin = 0.1;
    }
    // set default values
    if (treePath.size() == 0) treePath = "ggHiNtuplizer/EventTree";

    // push TH1D bins until bins for all histogram types are complete.
    //                until list length is 4.
    if (TH1D_Bins_List[0].size() == 0) {
        TH1D_Bins_List[0].push_back(12);     // nBins
        TH1D_Bins_List[1].push_back(-2.4);   // xLow
        TH1D_Bins_List[2].push_back(2.4);    // xUp
    }
    if (TH1D_Bins_List[0].size() == 1) {
        TH1D_Bins_List[0].push_back(40);
        TH1D_Bins_List[1].push_back(0);
        TH1D_Bins_List[2].push_back(120);
    }
    if (TH1D_Bins_List[0].size() == 2) {
        TH1D_Bins_List[0].push_back(40);
        TH1D_Bins_List[1].push_back(0);
        TH1D_Bins_List[2].push_back(120);
    }
    if (TH1D_Bins_List[0].size() == 3) {
        TH1D_Bins_List[0].push_back(50);
        TH1D_Bins_List[1].push_back(0);
        TH1D_Bins_List[2].push_back(200);
    }

    if (titleOffsetX == 0) titleOffsetX = 1.25;
    if (titleOffsetY == 0) titleOffsetY = 1.75;
    if (yMin == 0 && yMax == 0)  yMax = -1;

    if (windowWidth  == 0)  windowWidth = INPUT_DEFAULT::windowWidth;
    if (windowHeight == 0)  windowHeight = INPUT_DEFAULT::windowHeight;
    if (leftMargin == 0) leftMargin = 0.15;
    if (rightMargin == 0) rightMargin = 0.05;
    if (bottomMargin == 0) bottomMargin = INPUT_DEFAULT::bottomMargin;
    if (topMargin == 0) topMargin = INPUT_DEFAULT::topMargin;

    std::string collisionName =  getCollisionTypeName((COLL::TYPE)collisionType).c_str();
    int nTH1D_Bins_List = TH1D_Bins_List[0].size();
    // verbose about input configuration
    std::cout<<"Input Configuration :"<<std::endl;
    std::cout << "treePath = " << treePath.c_str() << std::endl;
    std::cout << "collision = " << collisionName.c_str() << std::endl;

    std::cout << "nTH1D_Bins_List = " << nTH1D_Bins_List << std::endl;  // for this program nTH1D_Bins_List must be 4.
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
    if (yMax < yMin) {
        std::cout << "default yMin and yMax values will be used." << std::endl;
    }

    std::cout << "windowWidth = " << windowWidth << std::endl;
    std::cout << "windowHeight = " << windowHeight << std::endl;
    std::cout << "leftMargin   = " << leftMargin << std::endl;
    std::cout << "rightMargin  = " << rightMargin << std::endl;
    std::cout << "bottomMargin = " << bottomMargin << std::endl;
    std::cout << "topMargin    = " << topMargin << std::endl;

    // input configuration
    std::vector<float> bins_eta[2];         // array of vectors for eta bins, each array element is a vector.
    std::vector<float> bins_genPt[2];       // array of vectors for genPt bins, each array element is a vector, should function also as
                                            // list of pt cuts for GEN-level photons matched to RECO photons
    std::vector<float> bins_recoPt[2];      // array of vectors for recoPt bins, each array element is a vector, should function also as
                                            // list of pt cuts for RECO photons
    std::vector<int>   bins_hiBin[2];       // array of vectors for hiBin bins, each array element is a vector.

    // RECO photon cuts
    float cut_phoHoverE;
    float cut_pho_ecalClusterIsoR4;
    float cut_pho_hcalRechitIsoR4;
    float cut_pho_trackIsoR4PtCut20;
    float cut_phoSigmaIEtaIEta_2012;
    float cut_sumIso;

    // GEN photon cuts
    float cut_mcCalIsoDR04;
    float cut_mcTrkIsoDR04;
    float cut_mcSumIso;

    if (configCuts.isValid) {
        bins_eta[0] = ConfigurationParser::ParseListFloat(
                configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_eta_gt]);
        bins_eta[1] = ConfigurationParser::ParseListFloat(
                configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_eta_lt]);
        bins_genPt[0] = ConfigurationParser::ParseListFloat(
                configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_mcPt_gt]);
        bins_genPt[1] = ConfigurationParser::ParseListFloat(
                configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_mcPt_lt]);
        bins_recoPt[0] = ConfigurationParser::ParseListFloat(
                configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_pt_gt]);
        bins_recoPt[1] = ConfigurationParser::ParseListFloat(
                configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_pt_lt]);
        bins_hiBin[0] = ConfigurationParser::ParseListInteger(
                configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_gt]);
        bins_hiBin[1] = ConfigurationParser::ParseListInteger(
                configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_lt]);

        cut_phoHoverE = configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].f[CUTS::PHO::k_phoHoverE];
        cut_pho_ecalClusterIsoR4 = configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pho_ecalClusterIsoR4];
        cut_pho_hcalRechitIsoR4 = configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pho_hcalRechitIsoR4];
        cut_pho_trackIsoR4PtCut20 = configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pho_trackIsoR4PtCut20];
        cut_phoSigmaIEtaIEta_2012 = configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].f[CUTS::PHO::k_phoSigmaIEtaIEta_2012];
        cut_sumIso = configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].f[CUTS::PHO::k_sumIso];

        cut_mcCalIsoDR04 = configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].f[CUTS::PHO::k_mcCalIsoDR04];
        cut_mcTrkIsoDR04 = configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].f[CUTS::PHO::k_mcTrkIsoDR04];
        cut_mcSumIso = configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].f[CUTS::PHO::k_mcSumIso];
    }
    else {
        bins_eta[0].push_back(0);
        bins_eta[1].push_back(2.4);
        bins_genPt[0].push_back(0);
        bins_genPt[1].push_back(-1);
        bins_recoPt[0].push_back(0);
        bins_recoPt[1].push_back(-1);
        bins_hiBin[0].push_back(0);
        bins_hiBin[1].push_back(-1);

        cut_phoHoverE = 0.1;
        cut_pho_ecalClusterIsoR4 = 4.2;
        cut_pho_hcalRechitIsoR4 = 2.2;
        cut_pho_trackIsoR4PtCut20 = 2;
        cut_phoSigmaIEtaIEta_2012 = 0.01;
        cut_sumIso = 6;

        cut_mcCalIsoDR04 = 5;
        cut_mcTrkIsoDR04 = 5;
        cut_mcSumIso = 0;
    }
    // set default values
    if (bins_eta[0].size() == 0) {
        bins_eta[0].push_back(0);
        bins_eta[1].push_back(2.4);
    }
    if (bins_genPt[0].size() == 0) {
        bins_genPt[0].push_back(0);
        bins_genPt[1].push_back(-1);
    }
    if (bins_recoPt[0].size() == 0) {
        bins_recoPt[0].push_back(0);
        bins_recoPt[1].push_back(-1);
    }
    if (bins_hiBin[0].size() == 0) {
        bins_hiBin[0].push_back(0);
        bins_hiBin[1].push_back(-1);
    }

    if (bins_genPt[1].size() < bins_genPt[0].size()) {
        int diff = (int signed)(bins_genPt[0].size() - bins_genPt[1].size());
        for (int i=0; i < diff ; ++i)
            bins_genPt[1].push_back(-1);
    }
    if (bins_recoPt[1].size() < bins_recoPt[0].size()) {
        int diff = (int signed)(bins_recoPt[0].size() - bins_recoPt[1].size());
        for (int i=0; i < diff; ++i)
            bins_recoPt[1].push_back(-1);
    }

    int nBins_eta = bins_eta[0].size();         // assume <myvector>[0] and <myvector>[1] have the same size.
    int nBins_genPt = bins_genPt[0].size();     // assume <myvector>[0] and <myvector>[1] have the same size.
    int nBins_recoPt = bins_recoPt[0].size();   // assume <myvector>[0] and <myvector>[1] have the same size.
    int nBins_hiBin = bins_hiBin[0].size();     // assume <myvector>[0] and <myvector>[1] have the same size.
    // verbose about cut configuration
    std::cout<<"Cut Configuration :"<<std::endl;
    std::cout << "nBins_eta = " << nBins_eta << std::endl;
    for (int i=0; i<nBins_eta; ++i) {
        std::cout << Form("bins_eta[%d] = [%f, %f)", i, bins_eta[0].at(i), bins_eta[1].at(i)) << std::endl;
    }
    std::cout << "nBins_genPt = " << nBins_genPt << std::endl;
    for (int i=0; i<nBins_genPt; ++i) {
        std::cout << Form("bins_genPt[%d] = [%f, %f)", i, bins_genPt[0].at(i), bins_genPt[1].at(i)) << std::endl;
    }
    std::cout << "nBins_recoPt = " << nBins_recoPt << std::endl;
    for (int i=0; i<nBins_recoPt; ++i) {
        std::cout << Form("bins_recoPt[%d] = [%f, %f)", i, bins_recoPt[0].at(i), bins_recoPt[1].at(i)) << std::endl;
    }
    std::cout << "nBins_hiBin = " << nBins_hiBin << std::endl;
    for (int i=0; i<nBins_hiBin; ++i) {
        std::cout << Form("bins_hiBin[%d] = [%d, %d)", i, bins_hiBin[0].at(i), bins_hiBin[1].at(i)) << std::endl;
    }

    bool isHI = collisionIsHI((COLL::TYPE)collisionType);

    std::cout<<"cut_phoHoverE             = "<< cut_phoHoverE <<std::endl;
    std::cout<<"cut_pho_ecalClusterIsoR4  = "<< cut_pho_ecalClusterIsoR4 <<std::endl;
    std::cout<<"cut_pho_hcalRechitIsoR4   = "<< cut_pho_hcalRechitIsoR4 <<std::endl;
    std::cout<<"cut_pho_trackIsoR4PtCut20 = "<< cut_pho_trackIsoR4PtCut20 <<std::endl;
    std::cout<<"cut_phoSigmaIEtaIEta_2012 = "<< cut_phoSigmaIEtaIEta_2012 <<std::endl;
    std::cout<<"cut_sumIso                = "<< cut_sumIso <<std::endl;

    std::cout<<"cut_mcCalIsoDR04 = "<< cut_mcCalIsoDR04 <<std::endl;
    std::cout<<"cut_mcTrkIsoDR04 = "<< cut_mcTrkIsoDR04 <<std::endl;
    std::cout<<"cut_mcSumIso     = "<< cut_mcSumIso <<std::endl;

    std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.Data());

    std::cout<<"input ROOT files : num = "<<inputFiles.size()<< std::endl;
    std::cout<<"#####"<< std::endl;
    for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
        std::cout<<(*it).c_str()<< std::endl;
    }
    std::cout<<"##### END #####"<< std::endl;

    TChain* treeggHiNtuplizer = new TChain(treePath.c_str());
    TChain* treeHiEvt = new TChain("hiEvtAnalyzer/HiTree");
    TChain* treeHiForestInfo = new TChain("HiForest/HiForestInfo");

    for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
       treeggHiNtuplizer->Add((*it).c_str());
       treeHiEvt->Add((*it).c_str());
       treeHiForestInfo->Add((*it).c_str());
    }

    HiForestInfoController hfic(treeHiForestInfo);
    std::cout<<"### HiForestInfo Tree ###"<< std::endl;
    hfic.printHiForestInfo();
    std::cout<<"###"<< std::endl;

    treeggHiNtuplizer->SetBranchStatus("*",0);     // disable all branches
    treeggHiNtuplizer->SetBranchStatus("run",1);    // enable event information
    treeggHiNtuplizer->SetBranchStatus("event",1);
    treeggHiNtuplizer->SetBranchStatus("lumis",1);

    treeggHiNtuplizer->SetBranchStatus("nPho",1);     // enable photon branches
    treeggHiNtuplizer->SetBranchStatus("pho*",1);     // enable photon branches
    treeggHiNtuplizer->SetBranchStatus("nMC*",1);     // enable GEN particle branches
    treeggHiNtuplizer->SetBranchStatus("mc*",1);      // enable GEN particle branches
    // check existence of genMatching branch
    if (!treeggHiNtuplizer->GetBranch("pho_genMatchedIndex")) {
        std::cout << "WARNING : Branch pho_genMatchedIndex does not exist." <<std::endl;
    }

    // specify explicitly which branches to use, do not use wildcard
    Int_t hiBin;
    treeHiEvt->SetBranchStatus("*",0);
    treeHiEvt->SetBranchStatus("hiBin",1);

    treeHiEvt->SetBranchAddress("hiBin",&hiBin);

    ggHiNtuplizer ggHi;
    ggHi.setupTreeForReading(treeggHiNtuplizer);

    TH1::SetDefaultSumw2();
    // object for set of all possible match/fake ratio histograms
    std::vector<std::string> matchRatioHistType {"matching efficiency", "fake rate"};
    std::vector<std::string> histNamePrefix {"Match", "Fake"};
    int nTypes = matchRatioHistType.size();
    matchRatioHist matchHist[nTypes][MATCHRATIO::kN_MATCHRATIO_DEP][nBins_eta][nBins_genPt][nBins_recoPt][nBins_hiBin];
    matchRatioHist fakeHist [MATCHRATIO::kN_MATCHRATIO_DEP][nBins_eta][nBins_genPt][nBins_recoPt][nBins_hiBin];
    for (int iEta = 0; iEta < nBins_eta; ++iEta) {
        for (int iGenPt = 0; iGenPt < nBins_genPt; ++iGenPt) {
            for (int iRecoPt = 0; iRecoPt < nBins_recoPt; ++iRecoPt) {
                for (int iHibin = 0; iHibin < nBins_hiBin; ++iHibin) {

                    for (int j=0 ; j<nTypes; ++j) {

                    if (iEta > 0 && iGenPt > 0 && iRecoPt > 0 && iHibin > 0)  continue;

                    // histogram ranges
                    for (int iMatchRatio = 0; iMatchRatio < MATCHRATIO::kN_MATCHRATIO_DEP; ++iMatchRatio) {

                        matchHist[j][iMatchRatio][iEta][iGenPt][iRecoPt][iHibin].ranges[MATCHRATIO::kETA][0] = bins_eta[0].at(iEta);
                        matchHist[j][iMatchRatio][iEta][iGenPt][iRecoPt][iHibin].ranges[MATCHRATIO::kETA][1] = bins_eta[1].at(iEta);
                        matchHist[j][iMatchRatio][iEta][iGenPt][iRecoPt][iHibin].ranges[MATCHRATIO::kGENPT][0] = bins_genPt[0].at(iGenPt);
                        matchHist[j][iMatchRatio][iEta][iGenPt][iRecoPt][iHibin].ranges[MATCHRATIO::kGENPT][1] = bins_genPt[1].at(iGenPt);
                        matchHist[j][iMatchRatio][iEta][iGenPt][iRecoPt][iHibin].ranges[MATCHRATIO::kRECOPT][0] = bins_recoPt[0].at(iRecoPt);
                        matchHist[j][iMatchRatio][iEta][iGenPt][iRecoPt][iHibin].ranges[MATCHRATIO::kRECOPT][1] = bins_recoPt[1].at(iRecoPt);
                        matchHist[j][iMatchRatio][iEta][iGenPt][iRecoPt][iHibin].ranges[MATCHRATIO::kHIBIN][0] = bins_hiBin[0].at(iHibin);
                        matchHist[j][iMatchRatio][iEta][iGenPt][iRecoPt][iHibin].ranges[MATCHRATIO::kHIBIN][1] = bins_hiBin[1].at(iHibin);
                    }

                    // for histograms with a particular dependence,
                    // a single index is used in the multidimensional array of matchRatioHist objects is used.
                    // Example : for a histogram with eta dependence (eta is the x-axis), there will not be different histograms
                    // with different eta ranges.
                    // There will be objects like : hist[MATCHRATIO::kETA][0][iGenPt][iRecoPt][iHibin]
                    // but not like : hist[MATCHRATIO::kETA][1][iGenPt][iRecoPt][iHibin]
                    // in general there will be no object hist[someIndex][iEta][iGenPt][iRecoPt][iHibin] such that iEta, iGenpT, iRecoPt, iHibin > 0

                    // initialize histograms with eta dependence
                    if (iEta == 0)  {
                        std::string tmpName = Form("%s_depEta_etaBin%d_genPtBin%d_recoPtBin%d_hiBin%d",
                                histNamePrefix.at(j).c_str(),iEta, iGenPt, iRecoPt, iHibin);
                        matchHist[j][MATCHRATIO::kETA][iEta][iGenPt][iRecoPt][iHibin].name = tmpName.c_str();

                        float nBins = TH1D_Bins_List[0].at(MATCHRATIO::kETA);   // nBins
                        float xLow  = TH1D_Bins_List[1].at(MATCHRATIO::kETA);   // xLow
                        float xUp   = TH1D_Bins_List[2].at(MATCHRATIO::kETA);   // xUp

                        std::string tmpHistName = Form("hNum_%s", tmpName.c_str());
                        matchHist[j][MATCHRATIO::kETA][iEta][iGenPt][iRecoPt][iHibin].hNum =
                                new TH1D(tmpHistName.c_str(), "; photon #eta;", nBins, xLow, xUp);
                        tmpHistName = Form("hDenom_%s", tmpName.c_str());
                        matchHist[j][MATCHRATIO::kETA][iEta][iGenPt][iRecoPt][iHibin].hDenom =
                                (TH1D*)matchHist[j][MATCHRATIO::kETA][iEta][iGenPt][iRecoPt][iHibin].hNum->Clone(tmpHistName.c_str());

                        matchHist[j][MATCHRATIO::kETA][iEta][iGenPt][iRecoPt][iHibin].hNumInitialized = true;
                        matchHist[j][MATCHRATIO::kETA][iEta][iGenPt][iRecoPt][iHibin].hDenomInitialized = true;

                        // set histogram title
                        matchHist[j][MATCHRATIO::kETA][iEta][iGenPt][iRecoPt][iHibin].prepareTitle();
                    }
                    // initialize histograms with gen pt dependence
                    if (iGenPt == 0 && j != kFAKE)  {     // no fake rate histograms with reco pt dependence
                        std::string tmpName = Form("%s_depGenPt_etaBin%d_genPtBin%d_recoPtBin%d_hiBin%d",
                                histNamePrefix.at(j).c_str(),iEta, iGenPt, iRecoPt, iHibin);
                        matchHist[j][MATCHRATIO::kGENPT][iEta][iGenPt][iRecoPt][iHibin].name = tmpName.c_str();

                        float nBins = TH1D_Bins_List[0].at(MATCHRATIO::kGENPT);   // nBins
                        float xLow  = TH1D_Bins_List[1].at(MATCHRATIO::kGENPT);   // xLow
                        float xUp   = TH1D_Bins_List[2].at(MATCHRATIO::kGENPT);   // xUp

                        std::string tmpHistName = Form("hNum_%s", tmpName.c_str());
                        matchHist[j][MATCHRATIO::kGENPT][iEta][iGenPt][iRecoPt][iHibin].hNum =
                                new TH1D(tmpHistName.c_str(), ";Gen p_{T};", nBins, xLow, xUp);
                        tmpHistName = Form("hDenom_%s", tmpName.c_str());
                        matchHist[j][MATCHRATIO::kGENPT][iEta][iGenPt][iRecoPt][iHibin].hDenom =
                                (TH1D*)matchHist[j][MATCHRATIO::kGENPT][iEta][iGenPt][iRecoPt][iHibin].hNum->Clone(tmpHistName.c_str());

                        matchHist[j][MATCHRATIO::kGENPT][iEta][iGenPt][iRecoPt][iHibin].hNumInitialized = true;
                        matchHist[j][MATCHRATIO::kGENPT][iEta][iGenPt][iRecoPt][iHibin].hDenomInitialized = true;

                        // set histogram title
                        matchHist[j][MATCHRATIO::kGENPT][iEta][iGenPt][iRecoPt][iHibin].prepareTitle();
                    }
                    // initialize histograms with reco pt dependence
                    if (iRecoPt == 0 && j != kMATCH)  {     // no matching efficiency histograms with reco pt dependence
                        std::string tmpName = Form("%s_depRecoPt_etaBin%d_genPtBin%d_recoPtBin%d_hiBin%d",
                                histNamePrefix.at(j).c_str(),iEta, iGenPt, iRecoPt, iHibin);
                        matchHist[j][MATCHRATIO::kRECOPT][iEta][iGenPt][iRecoPt][iHibin].name = tmpName.c_str();

                        float nBins = TH1D_Bins_List[0].at(MATCHRATIO::kRECOPT);   // nBins
                        float xLow  = TH1D_Bins_List[1].at(MATCHRATIO::kRECOPT);   // xLow
                        float xUp   = TH1D_Bins_List[2].at(MATCHRATIO::kRECOPT);   // xUp

                        std::string tmpHistName = Form("hNum_%s", tmpName.c_str());
                        matchHist[j][MATCHRATIO::kRECOPT][iEta][iGenPt][iRecoPt][iHibin].hNum =
                                new TH1D(tmpHistName.c_str(), ";Reco p_{T};", nBins, xLow, xUp);
                        tmpHistName = Form("hDenom_%s", tmpName.c_str());
                        matchHist[j][MATCHRATIO::kRECOPT][iEta][iGenPt][iRecoPt][iHibin].hDenom =
                                (TH1D*)matchHist[j][MATCHRATIO::kRECOPT][iEta][iGenPt][iRecoPt][iHibin].hNum->Clone(tmpHistName.c_str());

                        matchHist[j][MATCHRATIO::kRECOPT][iEta][iGenPt][iRecoPt][iHibin].hNumInitialized = true;
                        matchHist[j][MATCHRATIO::kRECOPT][iEta][iGenPt][iRecoPt][iHibin].hDenomInitialized = true;

                        // set histogram title
                        matchHist[j][MATCHRATIO::kRECOPT][iEta][iGenPt][iRecoPt][iHibin].prepareTitle();
                    }
                    // initialize histograms with centrality dependence
                    if (iHibin == 0)  {
                        std::string tmpName = Form("%s_depHiBin_etaBin%d_genPtBin%d_recoPtBin%d_hiBin%d",
                                histNamePrefix.at(j).c_str(),iEta, iGenPt, iRecoPt, iHibin);
                        matchHist[j][MATCHRATIO::kHIBIN][iEta][iGenPt][iRecoPt][iHibin].name = tmpName.c_str();

                        float nBins = TH1D_Bins_List[0].at(MATCHRATIO::kHIBIN);   // nBins
                        float xLow  = TH1D_Bins_List[1].at(MATCHRATIO::kHIBIN);   // xLow
                        float xUp   = TH1D_Bins_List[2].at(MATCHRATIO::kHIBIN);   // xUp

                        std::string tmpHistName = Form("hNum_%s", tmpName.c_str());
                        matchHist[j][MATCHRATIO::kHIBIN][iEta][iGenPt][iRecoPt][iHibin].hNum =
                                new TH1D(tmpHistName.c_str(), ";Hibin;", nBins, xLow, xUp);
                        tmpHistName = Form("hDenom_%s", tmpName.c_str());
                        matchHist[j][MATCHRATIO::kHIBIN][iEta][iGenPt][iRecoPt][iHibin].hDenom =
                                (TH1D*)matchHist[j][MATCHRATIO::kHIBIN][iEta][iGenPt][iRecoPt][iHibin].hNum->Clone(tmpHistName.c_str());

                        matchHist[j][MATCHRATIO::kHIBIN][iEta][iGenPt][iRecoPt][iHibin].hNumInitialized = true;
                        matchHist[j][MATCHRATIO::kHIBIN][iEta][iGenPt][iRecoPt][iHibin].hDenomInitialized = true;

                        // set histogram title
                        matchHist[j][MATCHRATIO::kHIBIN][iEta][iGenPt][iRecoPt][iHibin].prepareTitle();
                    }

                    }
                }
            }
        }
    }

    EventMatcher* em = new EventMatcher();
    Long64_t duplicateEntries = 0;

    Long64_t entries = treeggHiNtuplizer->GetEntries();
    Long64_t entriesAnalyzed = 0;
    std::cout << "entries = " << entries << std::endl;
    std::cout<< "Loop : " << treePath.c_str() <<std::endl;
    for (Long64_t j_entry = 0; j_entry < entries; ++j_entry)
    {
        if (j_entry % 2000 == 0)  {
          std::cout << "current entry = " <<j_entry<<" out of "<<entries<<" : "<<std::setprecision(2)<<(double)j_entry/entries*100<<" %"<<std::endl;
        }

        treeggHiNtuplizer->GetEntry(j_entry);
        treeHiEvt->GetEntry(j_entry);

        bool eventAdded = em->addEvent(ggHi.run, ggHi.lumis, ggHi.event, j_entry);
        if(!eventAdded) // this event is duplicate, skip this one.
        {
            duplicateEntries++;
            continue;
        }

        entriesAnalyzed++;

        // fake rate
        for (int i=0; i<ggHi.nPho; ++i) {

            // selections on RECO particle
            if (!(ggHi.phoSigmaIEtaIEta_2012->at(i) > 0.002 && ggHi.pho_swissCrx->at(i) < 0.9 && TMath::Abs(ggHi.pho_seedTime->at(i)) < 3)) continue;

            if (cut_phoHoverE != 0) {
                if (!(ggHi.phoHoverE->at(i) < cut_phoHoverE))   continue;
            }
            if (cut_pho_ecalClusterIsoR4 != 0) {
                if (!(ggHi.pho_ecalClusterIsoR4->at(i) < cut_pho_ecalClusterIsoR4))   continue;
            }
            if (cut_pho_hcalRechitIsoR4 != 0) {
                if (!(ggHi.pho_hcalRechitIsoR4->at(i) < cut_pho_hcalRechitIsoR4))   continue;
            }
            if (cut_pho_trackIsoR4PtCut20 != 0) {
                if (!(ggHi.pho_trackIsoR4PtCut20->at(i) < cut_pho_trackIsoR4PtCut20))   continue;
            }
            if (cut_phoSigmaIEtaIEta_2012 != 0) {
                if (!(ggHi.phoSigmaIEtaIEta_2012->at(i) < cut_phoSigmaIEtaIEta_2012))   continue;
            }
            if (cut_sumIso != 0) {
                if (!((ggHi.pho_ecalClusterIsoR4->at(i) +
                        ggHi.pho_hcalRechitIsoR4->at(i)  +
                        ggHi.pho_trackIsoR4PtCut20->at(i)) < cut_sumIso))   continue;
            }

            // look for matching GEN particle
            int genMatchedIndex = ggHi.pho_genMatchedIndex->at(i);
            bool matchedToGENPhoton = false;
            double genPt = -1;
            if (genMatchedIndex >= 0)  {
                matchedToGENPhoton = (ggHi.mcPID->at(genMatchedIndex) == 22);
                genPt = ggHi.mcPt->at(genMatchedIndex);
            }

            double eta = ggHi.phoEta->at(i);
            double pt  = ggHi.phoEt->at(i);

            for (int iEta = 0;  iEta < nBins_eta; ++iEta) {
            for (int iGenPt = 0;  iGenPt < nBins_genPt; ++iGenPt) {
            for (int iRecoPt = 0; iRecoPt < nBins_recoPt; ++iRecoPt) {
            for (int iHibin = 0;  iHibin < nBins_hiBin; ++iHibin) {

                if (iEta > 0 && iGenPt > 0 && iRecoPt > 0 && iHibin > 0)  continue;

                if (matchHist[kFAKE][MATCHRATIO::kETA][iEta][iGenPt][iRecoPt][iHibin].hDenomInitialized) {
                    matchHist[kFAKE][MATCHRATIO::kETA][iEta][iGenPt][iRecoPt][iHibin].FillHDenom(eta, eta, genPt, pt, hiBin);
                }

                if (matchHist[kFAKE][MATCHRATIO::kRECOPT][iEta][iGenPt][iRecoPt][iHibin].hDenomInitialized) {
                    matchHist[kFAKE][MATCHRATIO::kRECOPT][iEta][iGenPt][iRecoPt][iHibin].FillHDenom(pt, eta, genPt, pt, hiBin);
                }

                if (matchHist[kFAKE][MATCHRATIO::kHIBIN][iEta][iGenPt][iRecoPt][iHibin].hDenomInitialized) {
                    matchHist[kFAKE][MATCHRATIO::kHIBIN][iEta][iGenPt][iRecoPt][iHibin].FillHDenom(hiBin, eta, genPt, pt, hiBin);
                }

                if (!matchedToGENPhoton) {
                    if (matchHist[kFAKE][MATCHRATIO::kETA][iEta][iGenPt][iRecoPt][iHibin].hNumInitialized) {
                        matchHist[kFAKE][MATCHRATIO::kETA][iEta][iGenPt][iRecoPt][iHibin].FillHNum(eta, eta, genPt, pt, hiBin);
                    }

                    if (matchHist[kFAKE][MATCHRATIO::kRECOPT][iEta][iGenPt][iRecoPt][iHibin].hNumInitialized) {
                        matchHist[kFAKE][MATCHRATIO::kRECOPT][iEta][iGenPt][iRecoPt][iHibin].FillHNum(pt, eta, genPt, pt, hiBin);
                    }

                    if (matchHist[kFAKE][MATCHRATIO::kHIBIN][iEta][iGenPt][iRecoPt][iHibin].hNumInitialized) {
                        matchHist[kFAKE][MATCHRATIO::kHIBIN][iEta][iGenPt][iRecoPt][iHibin].FillHNum(hiBin, eta, genPt, pt, hiBin);
                    }
                }

            }}}}

        }

        // matching efficiency
        for (int i=0; i<ggHi.nMC; ++i) {

            if (ggHi.mcPID->at(i) != 22)   continue;    // consider only GEN particles that are photons
            double genPt = ggHi.mcPt->at(i);
            double eta = ggHi.mcEta->at(i);

            // look for matching RECO particle
            bool matchedToRECOPhoton = false;
            double pt = -1;
            for (int j = 0; j<ggHi.nPho; ++j) {

                if (i == ggHi.pho_genMatchedIndex->at(j)) {
                    matchedToRECOPhoton = true;
                }
            }

            // selections on GEN particle
            if (isHI) {
                if (cut_mcCalIsoDR04 != 0) {
                    if (!(ggHi.mcCalIsoDR04->at(i) < cut_mcCalIsoDR04))   continue;
                }
                if (cut_mcTrkIsoDR04 != 0) {
                    if (!(ggHi.mcTrkIsoDR04->at(i) < cut_mcTrkIsoDR04))   continue;
                }
                if (cut_mcSumIso != 0) {
                    if (!((ggHi.mcCalIsoDR04->at(i) +
                           ggHi.mcTrkIsoDR04->at(i)) < cut_mcSumIso))   continue;
                }
            }
            else {
                if (cut_mcCalIsoDR04 != 0) {
                    if (!(ggHi.mcCalIsoDR04->at(i) < cut_mcCalIsoDR04))   continue;
                }
                if (cut_mcTrkIsoDR04 != 0) {
                    if (!(ggHi.mcTrkIsoDR04->at(i) < cut_mcTrkIsoDR04))   continue;
                }
                if (cut_mcSumIso != 0) {
                    if (!((ggHi.mcCalIsoDR04->at(i) +
                           ggHi.mcTrkIsoDR04->at(i)) < cut_mcSumIso))   continue;
                }
            }

            for (int iEta = 0;  iEta < nBins_eta; ++iEta) {
            for (int iGenPt = 0;  iGenPt < nBins_genPt; ++iGenPt) {
            for (int iRecoPt = 0; iRecoPt < nBins_recoPt; ++iRecoPt) {
            for (int iHibin = 0;  iHibin < nBins_hiBin; ++iHibin) {

                if (iEta > 0 && iGenPt > 0 && iRecoPt > 0 && iHibin > 0)  continue;

                if (matchHist[kMATCH][MATCHRATIO::kETA][iEta][iGenPt][iRecoPt][iHibin].hDenomInitialized) {
                    matchHist[kMATCH][MATCHRATIO::kETA][iEta][iGenPt][iRecoPt][iHibin].FillHDenom(eta, eta, genPt, pt, hiBin);
                }

                if (matchHist[kMATCH][MATCHRATIO::kGENPT][iEta][iGenPt][iRecoPt][iHibin].hDenomInitialized) {
                    matchHist[kMATCH][MATCHRATIO::kGENPT][iEta][iGenPt][iRecoPt][iHibin].FillHDenom(genPt, eta, genPt, pt, hiBin);
                }

                if (matchHist[kMATCH][MATCHRATIO::kHIBIN][iEta][iGenPt][iRecoPt][iHibin].hDenomInitialized) {
                    matchHist[kMATCH][MATCHRATIO::kHIBIN][iEta][iGenPt][iRecoPt][iHibin].FillHDenom(hiBin, eta, genPt, pt, hiBin);
                }

                if (matchedToRECOPhoton) {
                    if (matchHist[kMATCH][MATCHRATIO::kETA][iEta][iGenPt][iRecoPt][iHibin].hNumInitialized) {
                        matchHist[kMATCH][MATCHRATIO::kETA][iEta][iGenPt][iRecoPt][iHibin].FillHNum(eta, eta, genPt, pt, hiBin);
                    }

                    if (matchHist[kMATCH][MATCHRATIO::kGENPT][iEta][iGenPt][iRecoPt][iHibin].hNumInitialized) {
                        matchHist[kMATCH][MATCHRATIO::kGENPT][iEta][iGenPt][iRecoPt][iHibin].FillHNum(genPt, eta, genPt, pt, hiBin);
                    }

                    if (matchHist[kMATCH][MATCHRATIO::kHIBIN][iEta][iGenPt][iRecoPt][iHibin].hNumInitialized) {
                        matchHist[kMATCH][MATCHRATIO::kHIBIN][iEta][iGenPt][iRecoPt][iHibin].FillHNum(hiBin, eta, genPt, pt, hiBin);
                    }
                }

            }}}}

        }
    }
    std::cout<<  "Loop ENDED : " << treePath.c_str() <<std::endl;
    std::cout << "entries            = " << entries << std::endl;
    std::cout << "duplicateEntries   = " << duplicateEntries << std::endl;
    std::cout << "entriesAnalyzed    = " << entriesAnalyzed << std::endl;

    TFile *output = TFile::Open(outputFile.Data(),"RECREATE");
    output->cd();

    // declaration of graphics objects.
    // the idea is to initialize each Graphics object right before its use, then to delete it right after they are saved/written.
    TCanvas* c;
    for (int iEta = 0; iEta < nBins_eta; ++iEta) {
        for (int iGenPt = 0; iGenPt < nBins_genPt; ++iGenPt) {
            for (int iRecoPt = 0; iRecoPt < nBins_recoPt; ++iRecoPt) {
                for (int iHibin = 0; iHibin < nBins_hiBin; ++iHibin) {

                    for (int j=0 ; j<nTypes; ++j) {

                    if (iEta > 0 && iGenPt > 0 && iRecoPt > 0 && iHibin > 0)  continue;

                    // for histograms with a particular dependence,
                    // a single index is used in the multidimensional array of matchRatioHist objects is used.
                    // Example : for an match ratio histogram with eta dependence (eta is the x-axis), there will not be different histograms
                    // with different eta ranges.
                    // There will be objects like : hist[MATCHRATIO::kETA][0][iGenPt][iRecoPt][iHibin]
                    // but not like : hist[MATCHRATIO::kETA][1][iGenPt][iRecoPt][iHibin]
                    // in general there will be no object hist[someIndex][iEta][iGenPt][iRecoPt][iHibin] such that iEta, iGenpT, iRecoPt, iHibin > 0

                    std::vector<int> indices {iEta, iGenPt, iRecoPt, iHibin};
                    std::vector<int> matchRatioDeps {MATCHRATIO::kETA, MATCHRATIO::kGENPT, MATCHRATIO::kRECOPT, MATCHRATIO::kHIBIN};
                    for (int unsigned iMatch = 0; iMatch < matchRatioDeps.size(); ++iMatch) {
                        if (indices.at(iMatch) == 0)
                        {
                            int iMatchDep = matchRatioDeps.at(iMatch);
                            // write histograms with a particular dependence
                            if (matchHist[j][iMatchDep][iEta][iGenPt][iRecoPt][iHibin].hNumInitialized)
                                matchHist[j][iMatchDep][iEta][iGenPt][iRecoPt][iHibin].hNum->Write();
                            if (matchHist[j][iMatchDep][iEta][iGenPt][iRecoPt][iHibin].hDenomInitialized)
                                matchHist[j][iMatchDep][iEta][iGenPt][iRecoPt][iHibin].hDenom->Write();

                            if (!matchHist[j][iMatchDep][iEta][iGenPt][iRecoPt][iHibin].hNumInitialized ||
                                !matchHist[j][iMatchDep][iEta][iGenPt][iRecoPt][iHibin].hDenomInitialized )  continue;

                            matchHist[j][iMatchDep][iEta][iGenPt][iRecoPt][iHibin].calcRatio();

                            if (matchHist[j][iMatchDep][iEta][iGenPt][iRecoPt][iHibin].graphAsymmErrorsInitialized)
                                matchHist[j][iMatchDep][iEta][iGenPt][iRecoPt][iHibin].a->Write();

                            if (matchHist[j][iMatchDep][iEta][iGenPt][iRecoPt][iHibin].hRatioInitialized) {
                                matchHist[j][iMatchDep][iEta][iGenPt][iRecoPt][iHibin].hRatio->Write();

                                if (j == kMATCH)
                                    matchHist[j][iMatchDep][iEta][iGenPt][iRecoPt][iHibin].hRatio->SetYTitle("Matching Efficiency");
                                if (j == kFAKE)
                                    matchHist[j][iMatchDep][iEta][iGenPt][iRecoPt][iHibin].hRatio->SetYTitle("Fake Rate");

                                matchHist[j][iMatchDep][iEta][iGenPt][iRecoPt][iHibin].hRatio->SetMinimum(0);
                                matchHist[j][iMatchDep][iEta][iGenPt][iRecoPt][iHibin].hRatio->SetMaximum(1.2);
                                if (yMax > yMin) {
                                    matchHist[j][iMatchDep][iEta][iGenPt][iRecoPt][iHibin].hRatio->SetMinimum(yMin);
                                    matchHist[j][iMatchDep][iEta][iGenPt][iRecoPt][iHibin].hRatio->SetMaximum(yMax);
                                }
                            }

                            std::string canvasName;
                            std::string name = matchHist[j][iMatchDep][iEta][iGenPt][iRecoPt][iHibin].name.c_str();
                            std::string title = matchHist[j][iMatchDep][iEta][iGenPt][iRecoPt][iHibin].title.c_str();
                            std::string titleX = matchHist[j][iMatchDep][iEta][iGenPt][iRecoPt][iHibin].titleX.c_str();

                            // TGraphAsymmErrors
                            canvasName = Form("cnv_a_%s", name.c_str());
                            c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
                            c->cd();
                            setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);

                            // base histogram on which TGraphAsymmErrors will be plotted.
                            TH1D* h_dummy =(TH1D*)matchHist[j][iMatchDep][iEta][iGenPt][iRecoPt][iHibin].hRatio->Clone("h_dummy");
                            // polishing before drawing the turn on curves
                            h_dummy->Reset();
                            setTH1_efficiency(h_dummy, titleOffsetX, titleOffsetY);
                            h_dummy->SetStats(false);
                            h_dummy->Draw();

                            matchHist[j][iMatchDep][iEta][iGenPt][iRecoPt][iHibin].a->Draw("p e");

                            // draw line y = 1
                            float x1 = h_dummy->GetXaxis()->GetXmin();
                            float x2 = h_dummy->GetXaxis()->GetXmax();
                            TLine line(x1, 1, x2,1);
                            line.SetLineStyle(kDashed);
                            line.Draw();

                            setCanvasFinal(c);
                            c->Write();
                            h_dummy->Delete();
                            c->Close();         // do not use Delete() for TCanvas.

                            // hRatio
                            canvasName = Form("cnv_hRatio_%s", name.c_str());
                            c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
                            c->cd();
                            setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);

                            setTH1_efficiency(matchHist[j][iMatchDep][iEta][iGenPt][iRecoPt][iHibin].hRatio, titleOffsetX, titleOffsetY);
                            matchHist[j][iMatchDep][iEta][iGenPt][iRecoPt][iHibin].hRatio->Draw("e");

                            // draw line y = 1
                            line.Draw();

                            setCanvasFinal(c);
                            c->Write();
                            c->Close();         // do not use Delete() for TCanvas.
                        }
                    }

                    }
                }
            }
        }
    }

    output->Close();
}

int main(int argc, char** argv)
{
    if (argc == 4) {
        photonMatchFakeRatio(argv[1], argv[2], argv[3]);
        return 0;
    }
    else if (argc == 3) {
        photonMatchFakeRatio(argv[1], argv[2]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./photonMatchFakeRatio.exe <configFile> <inputFile> <outputFile>"
                << std::endl;
        return 1;
    }
}
