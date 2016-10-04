/*
 * macro to draw eta, GEN Pt, RECO Pt and centrality dependent photon energy scale plots.
 * The macro can make 4 types of 1D energy scale  histograms
 *  1. x-axis is eta.
 *  2. x-axis is GEN Pt
 *  3. x-axis is RECO Pt.
 *  4. x-axis is hiBin (centrality)
 *  Each 1D energy scale histogram has a corresponding 1D energy width (resolution) histogram.
 *  The macro provides a RECO Pt vs. GEN Pt correlation histogram for each eta and centrality bin.
 * saves histograms to a .root file.
 */

#include <TFile.h>
#include <TChain.h>
#include <TH1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TObjArray.h>
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
#include "../interface/energyScaleHist.h"


void photonEnergyScale(const TString configFile, const TString inputFile, const TString outputFile = "photonEnergyScale.root");

void photonEnergyScale(const TString configFile, const TString inputFile, const TString outputFile)
{
    std::cout<<"running photonEnergyScale()"<<std::endl;
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

    // input configuration
    // input for TTree
    std::string treePath  = configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treePath];
    int collisionType = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_collisionType];

    // input for TH1
    // nBins, xLow, xUp for the TH1D histogram
    // this bin list will be used for histograms where x-axis is eta.
    std::vector<std::vector<float>> TH1D_Bins_List = ConfigurationParser::ParseListTH1D_Bins(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH1D_Bins_List]);
    // nBinsx, xLow, xUp, nBinsy, yLow, yUp for a TH2D histogram
    // this bin list will be used for gen pt - reco pt correlation histogram.
    std::vector<std::vector<float>> TH2D_Bins_List = ConfigurationParser::ParseListTH2D_Bins(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH2D_Bins_List]);
    float titleOffsetX = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_titleOffsetX];
    float titleOffsetY = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_titleOffsetY];
    // min. y-axis value of energy Scale histogram default : 0.8
    float yMin = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_TH1_yMin];
    // max. y-axis value of energy Scale histogram default : 1.5
    float yMax = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_TH1_yMax];

    // input for TCanvas
    int windowWidth = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_windowWidth];
    int windowHeight = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_windowHeight];
    float leftMargin   = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_leftMargin];
    float rightMargin  = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_rightMargin];
    float bottomMargin = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_bottomMargin];
    float topMargin    = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_topMargin];

    // set default values
    if (treePath.size() == 0) treePath = "ggHiNtuplizer/EventTree";
    
    if (TH1D_Bins_List[0].size() == 0) {
        TH1D_Bins_List[0].push_back(12);     // nBins
        TH1D_Bins_List[1].push_back(-2.4);   // xLow
        TH1D_Bins_List[2].push_back(2.4);    // xUp
    }
    if (TH2D_Bins_List[0].size() == 0) {
        TH2D_Bins_List[0].push_back(120);    // nBinsx
        TH2D_Bins_List[1].push_back(0);      // xLow
        TH2D_Bins_List[2].push_back(120);    // xUp
        TH2D_Bins_List[3].push_back(120);    // nBinsy
        TH2D_Bins_List[4].push_back(0);      // yLow
        TH2D_Bins_List[5].push_back(120);    // yUp
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
    int nTH2D_Bins_List = TH2D_Bins_List[0].size();
    // verbose about input configuration
    std::cout<<"Input Configuration :"<<std::endl;
    std::cout << "treePath = " << treePath.c_str() << std::endl;
    std::cout << "collision = " << collisionName.c_str() << std::endl;

    std::cout << "nTH1D_Bins_List = " << nTH1D_Bins_List << std::endl;  // for this program nTH1D_Bins_List must be 1.
    for (int i=0; i<nTH1D_Bins_List; ++i) {
        std::cout << Form("TH1D_Bins_List[%d] = { ", i);
        std::cout << Form("%.0f, ", TH1D_Bins_List[0].at(i));
        std::cout << Form("%f, ", TH1D_Bins_List[1].at(i));
        std::cout << Form("%f }", TH1D_Bins_List[2].at(i)) << std::endl;;
    }
    std::cout << "nTH2D_Bins_List = " << nTH2D_Bins_List << std::endl;  // for this program nTH2D_Bins_List must be 1.
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

    // RECO photon cuts
    float cut_phoHoverE = configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].f[CUTS::PHO::k_phoHoverE];
    float cut_pho_ecalClusterIsoR4 = configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pho_ecalClusterIsoR4];
    float cut_pho_hcalRechitIsoR4 = configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pho_hcalRechitIsoR4];
    float cut_pho_trackIsoR4PtCut20 = configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pho_trackIsoR4PtCut20];
    float cut_phoSigmaIEtaIEta_2012 = configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].f[CUTS::PHO::k_phoSigmaIEtaIEta_2012];
    float cut_sumIso = configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].f[CUTS::PHO::k_sumIso];

    // GEN photon cuts
    float cut_mcCalIsoDR04 = configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].f[CUTS::PHO::k_mcCalIsoDR04];
    float cut_mcTrkIsoDR04 = configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].f[CUTS::PHO::k_mcTrkIsoDR04];
    float cut_mcSumIso = configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].f[CUTS::PHO::k_mcSumIso];

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
    // object for set of all possible energy scale histograms
    energyScaleHist hist[ENERGYSCALE::kN_ENERGYSCALE_DEP][nBins_eta][nBins_genPt][nBins_recoPt][nBins_hiBin];
    for (int iEta = 0; iEta < nBins_eta; ++iEta) {
        for (int iGenPt = 0; iGenPt < nBins_genPt; ++iGenPt) {
            for (int iRecoPt = 0; iRecoPt < nBins_recoPt; ++iRecoPt) {
                for (int iHibin = 0; iHibin < nBins_hiBin; ++iHibin) {

                    if (iEta > 0 && iGenPt > 0 && iRecoPt > 0 && iHibin > 0)  continue;

                    // histogram ranges
                    for (int iEScale = 0; iEScale < ENERGYSCALE::kN_ENERGYSCALE_DEP; ++iEScale) {

                        hist[iEScale][iEta][iGenPt][iRecoPt][iHibin].ranges[ENERGYSCALE::kETA][0] = bins_eta[0].at(iEta);
                        hist[iEScale][iEta][iGenPt][iRecoPt][iHibin].ranges[ENERGYSCALE::kETA][1] = bins_eta[1].at(iEta);
                        hist[iEScale][iEta][iGenPt][iRecoPt][iHibin].ranges[ENERGYSCALE::kGENPT][0] = bins_genPt[0].at(iGenPt);
                        hist[iEScale][iEta][iGenPt][iRecoPt][iHibin].ranges[ENERGYSCALE::kGENPT][1] = bins_genPt[1].at(iGenPt);
                        hist[iEScale][iEta][iGenPt][iRecoPt][iHibin].ranges[ENERGYSCALE::kRECOPT][0] = bins_recoPt[0].at(iRecoPt);
                        hist[iEScale][iEta][iGenPt][iRecoPt][iHibin].ranges[ENERGYSCALE::kRECOPT][1] = bins_recoPt[1].at(iRecoPt);
                        hist[iEScale][iEta][iGenPt][iRecoPt][iHibin].ranges[ENERGYSCALE::kHIBIN][0] = bins_hiBin[0].at(iHibin);
                        hist[iEScale][iEta][iGenPt][iRecoPt][iHibin].ranges[ENERGYSCALE::kHIBIN][1] = bins_hiBin[1].at(iHibin);
                    }

                    // for histograms with a particular dependence,
                    // a single index is used in the multidimensional array of energyScaleHist objects is used.
                    // Example : for an energy scale histogram with eta dependence (eta is the x-axis), there will not be different histograms
                    // with different eta ranges.
                    // There will be objects like : hist[ENERGYSCALE::kETA][0][iGenPt][iRecoPt][iHibin]
                    // but not like : hist[ENERGYSCALE::kETA][1][iGenPt][iRecoPt][iHibin]
                    // in general there will be no object hist[someIndex][iEta][iGenPt][iRecoPt][iHibin] such that iEta, iGenpT, iRecoPt, iHibin > 0

                    int nBinsx = TH2D_Bins_List[0].at(0);    // nBinsx
                    float xLow = TH2D_Bins_List[1].at(0);    // xLow
                    float xUp  = TH2D_Bins_List[2].at(0);    // xUp
                    int nBinsy = TH2D_Bins_List[3].at(0);    // nBinsy
                    float yLow = TH2D_Bins_List[4].at(0);    // yLow
                    float yUp  = TH2D_Bins_List[5].at(0);    // yUp

                    // initialize histograms with eta dependence
                    if (iEta == 0)  {
                        std::string tmpName = Form("depEta_etaBin%d_genPtBin%d_recoPtBin%d_hiBin%d", iEta, iGenPt, iRecoPt, iHibin);
                        hist[ENERGYSCALE::kETA][iEta][iGenPt][iRecoPt][iHibin].name = tmpName.c_str();

                        float nBinsEta = TH1D_Bins_List[0].at(0);   // nBins
                        float xLowEta  = TH1D_Bins_List[1].at(0);   // xLow
                        float xUpEta   = TH1D_Bins_List[2].at(0);   // xUp
                        std::string tmpHistName = Form("h2D_%s", tmpName.c_str());
                        std::string tmpHistName1D = Form("h_%s", tmpName.c_str());
                        hist[ENERGYSCALE::kETA][iEta][iGenPt][iRecoPt][iHibin].h2D =
                                new TH2D(tmpHistName.c_str(), ";photon #eta;Reco p_{T} / Gen p_{T}", nBinsEta, xLowEta, xUpEta, 100, 0, 2);
                        hist[ENERGYSCALE::kETA][iEta][iGenPt][iRecoPt][iHibin].h =
                                new TH1D(tmpHistName1D.c_str(), ";Reco p_{T} / Gen p_{T};", 100, 0, 2);

                        hist[ENERGYSCALE::kETA][iEta][iGenPt][iRecoPt][iHibin].h2Dinitialized = true;
                        hist[ENERGYSCALE::kETA][iEta][iGenPt][iRecoPt][iHibin].hInitialized = true;

                        // set histogram title
                        hist[ENERGYSCALE::kETA][iEta][iGenPt][iRecoPt][iHibin].prepareTitle();
                    }
                    // initialize histograms with gen pt dependence
                    if (iGenPt == 0 && nBins_genPt > 1)  {
                        std::string tmpName = Form("depGenPt_etaBin%d_genPtBin%d_recoPtBin%d_hiBin%d", iEta, iGenPt, iRecoPt, iHibin);
                        hist[ENERGYSCALE::kGENPT][iEta][iGenPt][iRecoPt][iHibin].name = tmpName.c_str();

                        // pt dependent histograms do not have uniform binning. They are binned using the pt gt/lt list.
                        double arr[nBins_genPt];
                        std::copy(bins_genPt[0].begin(), bins_genPt[0].end(), arr);
                        std::string tmpHistName = Form("h2D_%s", tmpName.c_str());
                        std::string tmpHistName1D = Form("h_%s", tmpName.c_str());
                        std::string tmpHistNameCorr = Form("h2Dcorr_%s", tmpName.c_str());
                        hist[ENERGYSCALE::kGENPT][iEta][iGenPt][iRecoPt][iHibin].h2D =
                                new TH2D(tmpHistName.c_str(), ";Gen p_{T};Reco p_{T} / Gen p_{T}", nBins_genPt-1, arr, 100, 0, 2);
                        hist[ENERGYSCALE::kGENPT][iEta][iGenPt][iRecoPt][iHibin].h =
                                new TH1D(tmpHistName1D.c_str(), ";Reco p_{T} / Gen p_{T};", 100, 0, 2);
                        hist[ENERGYSCALE::kGENPT][iEta][iGenPt][iRecoPt][iHibin].h2Dcorr =
                                new TH2D(tmpHistNameCorr.c_str(), ";Gen p_{T};Reco p_{T}", nBinsx, xLow, xUp, nBinsy, yLow, yUp);
                        // h2Dcorr will be used only by hist[ENERGYSCALE::kGENPT] object.
                        // By definition, hist[ENERGYSCALE::kEta] and hist[ENERGYSCALE::kHIBIN] objects would be redundant.

                        hist[ENERGYSCALE::kGENPT][iEta][iGenPt][iRecoPt][iHibin].h2Dinitialized = true;
                        hist[ENERGYSCALE::kGENPT][iEta][iGenPt][iRecoPt][iHibin].hInitialized = true;
                        hist[ENERGYSCALE::kGENPT][iEta][iGenPt][iRecoPt][iHibin].h2DcorrInitialized = true;

                        // set histogram title
                        hist[ENERGYSCALE::kGENPT][iEta][iGenPt][iRecoPt][iHibin].prepareTitle();
                    }
                    // initialize histograms with reco pt dependence
                    if (iRecoPt == 0 && nBins_recoPt > 1)  {
                        std::string tmpName = Form("depRecoPt_etaBin%d_genPtBin%d_recoPtBin%d_hiBin%d", iEta, iGenPt, iRecoPt, iHibin);
                        hist[ENERGYSCALE::kRECOPT][iEta][iGenPt][iRecoPt][iHibin].name = tmpName.c_str();

                        // pt dependent histograms do not have uniform binning. They are binned using the pt gt/lt list.
                        double arr[nBins_recoPt];
                        std::copy(bins_recoPt[0].begin(), bins_recoPt[0].end(), arr);
                        std::string tmpHistName = Form("h2D_%s", tmpName.c_str());
                        std::string tmpHistName1D = Form("h_%s", tmpName.c_str());
                        hist[ENERGYSCALE::kRECOPT][iEta][iGenPt][iRecoPt][iHibin].h2D =
                                new TH2D(tmpHistName.c_str(), ";Reco p_{T};Reco p_{T} / Gen p_{T}", nBins_recoPt-1, arr, 100, 0, 2);
                        hist[ENERGYSCALE::kRECOPT][iEta][iGenPt][iRecoPt][iHibin].h =
                                new TH1D(tmpHistName1D.c_str(), ";Reco p_{T} / Gen p_{T};", 100, 0, 2);

                        hist[ENERGYSCALE::kRECOPT][iEta][iGenPt][iRecoPt][iHibin].h2Dinitialized = true;
                        hist[ENERGYSCALE::kRECOPT][iEta][iGenPt][iRecoPt][iHibin].hInitialized = true;

                        // set histogram title
                        hist[ENERGYSCALE::kRECOPT][iEta][iGenPt][iRecoPt][iHibin].prepareTitle();
                    }
                    // initialize histograms with centrality dependence
                    if (iHibin == 0 && nBins_hiBin > 1)  {
                        std::string tmpName = Form("depHiBin_etaBin%d_genPtBin%d_recoPtBin%d_hiBin%d", iEta, iGenPt, iRecoPt, iHibin);
                        hist[ENERGYSCALE::kHIBIN][iEta][iGenPt][iRecoPt][iHibin].name = tmpName.c_str();

                        // centrality dependent histograms do not have uniform binning. They are binned using the pt gt/lt list.
                        double arr[nBins_hiBin];
                        std::copy(bins_hiBin[0].begin(), bins_hiBin[0].end(), arr);
                        std::string tmpHistName = Form("h2D_%s", tmpName.c_str());
                        std::string tmpHistName1D = Form("h_%s", tmpName.c_str());
                        hist[ENERGYSCALE::kHIBIN][iEta][iGenPt][iRecoPt][iHibin].h2D =
                                new TH2D(tmpHistName.c_str(), ";Hibin;Reco p_{T} / Gen p_{T}", nBins_hiBin-1, arr, 100, 0, 2);
                        hist[ENERGYSCALE::kHIBIN][iEta][iGenPt][iRecoPt][iHibin].h =
                                new TH1D(tmpHistName1D.c_str(), ";Reco p_{T} / Gen p_{T};", 100, 0, 2);

                        hist[ENERGYSCALE::kHIBIN][iEta][iGenPt][iRecoPt][iHibin].h2Dinitialized = true;
                        hist[ENERGYSCALE::kHIBIN][iEta][iGenPt][iRecoPt][iHibin].hInitialized = true;

                        // set histogram title
                        hist[ENERGYSCALE::kHIBIN][iEta][iGenPt][iRecoPt][iHibin].prepareTitle();
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

        // energy scale
        for (int i=0; i<ggHi.nPho; ++i) {

            // selections on GEN particle
            int genMatchedIndex = ggHi.pho_genMatchedIndex->at(i);
            if (genMatchedIndex < 0)   continue;    // is matched
            if (ggHi.mcPID->at(genMatchedIndex) != 22)   continue;    // is matched to a photon

            double genPt = ggHi.mcPt->at(genMatchedIndex);
            if (genPt <= 0)   continue;

            if (isHI) {
                if (cut_mcCalIsoDR04 != 0) {
                    if (!(ggHi.mcCalIsoDR04->at(genMatchedIndex) < cut_mcCalIsoDR04))   continue;
                }
                if (cut_mcTrkIsoDR04 != 0) {
                    if (!(ggHi.mcTrkIsoDR04->at(genMatchedIndex) < cut_mcTrkIsoDR04))   continue;
                }
                if (cut_mcSumIso != 0) {
                    if (!((ggHi.mcCalIsoDR04->at(genMatchedIndex) +
                           ggHi.mcTrkIsoDR04->at(genMatchedIndex)) < cut_mcSumIso))   continue;
                }
            }
            else {
                if (cut_mcCalIsoDR04 != 0) {
                    if (!(ggHi.mcCalIsoDR04->at(genMatchedIndex) < cut_mcCalIsoDR04))   continue;
                }
                if (cut_mcTrkIsoDR04 != 0) {
                    if (!(ggHi.mcTrkIsoDR04->at(genMatchedIndex) < cut_mcTrkIsoDR04))   continue;
                }
                if (cut_mcSumIso != 0) {
                    if (!((ggHi.mcCalIsoDR04->at(genMatchedIndex) +
                           ggHi.mcTrkIsoDR04->at(genMatchedIndex)) < cut_mcSumIso))   continue;
                }
            }

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

            double eta = ggHi.phoEta->at(i);
            double pt  = ggHi.phoEt->at(i);
            double energyScale = pt/genPt;

            for (int iEta = 0;  iEta < nBins_eta; ++iEta) {
            for (int iGenPt = 0;  iGenPt < nBins_genPt; ++iGenPt) {
            for (int iRecoPt = 0; iRecoPt < nBins_recoPt; ++iRecoPt) {
            for (int iHibin = 0;  iHibin < nBins_hiBin; ++iHibin) {

                if (iEta > 0 && iGenPt > 0 && iRecoPt > 0 && iHibin > 0)  continue;

                if (hist[ENERGYSCALE::kETA][iEta][iGenPt][iRecoPt][iHibin].h2Dinitialized) {
                    hist[ENERGYSCALE::kETA][iEta][iGenPt][iRecoPt][iHibin].FillH2D(energyScale, eta, eta, genPt, pt, hiBin);
                }
                if (hist[ENERGYSCALE::kETA][iEta][iGenPt][iRecoPt][iHibin].hInitialized) {
                    hist[ENERGYSCALE::kETA][iEta][iGenPt][iRecoPt][iHibin].FillH(energyScale, eta, genPt, pt, hiBin);
                }

                if (hist[ENERGYSCALE::kGENPT][iEta][iGenPt][iRecoPt][iHibin].h2Dinitialized) {
                    hist[ENERGYSCALE::kGENPT][iEta][iGenPt][iRecoPt][iHibin].FillH2D(energyScale, genPt, eta, genPt, pt, hiBin);
                }
                if (hist[ENERGYSCALE::kGENPT][iEta][iGenPt][iRecoPt][iHibin].hInitialized) {
                    hist[ENERGYSCALE::kGENPT][iEta][iGenPt][iRecoPt][iHibin].FillH(energyScale, eta, genPt, pt, hiBin);
                }
                if (hist[ENERGYSCALE::kGENPT][iEta][iGenPt][iRecoPt][iHibin].h2DcorrInitialized) {
                    hist[ENERGYSCALE::kGENPT][iEta][iGenPt][iRecoPt][iHibin].FillH2Dcorr(genPt, pt, eta, hiBin);
                }

                if (hist[ENERGYSCALE::kRECOPT][iEta][iGenPt][iRecoPt][iHibin].h2Dinitialized) {
                    hist[ENERGYSCALE::kRECOPT][iEta][iGenPt][iRecoPt][iHibin].FillH2D(energyScale, pt, eta, genPt, pt, hiBin);
                }
                if (hist[ENERGYSCALE::kRECOPT][iEta][iGenPt][iRecoPt][iHibin].hInitialized) {
                    hist[ENERGYSCALE::kRECOPT][iEta][iGenPt][iRecoPt][iHibin].FillH(energyScale, eta, genPt, pt, hiBin);
                }

                if (hist[ENERGYSCALE::kHIBIN][iEta][iGenPt][iRecoPt][iHibin].h2Dinitialized) {
                    hist[ENERGYSCALE::kHIBIN][iEta][iGenPt][iRecoPt][iHibin].FillH2D(energyScale, hiBin, eta, genPt, pt, hiBin);
                }
                if (hist[ENERGYSCALE::kHIBIN][iEta][iGenPt][iRecoPt][iHibin].hInitialized) {
                    hist[ENERGYSCALE::kHIBIN][iEta][iGenPt][iRecoPt][iHibin].FillH(energyScale, eta, genPt, pt, hiBin);
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

                    if (iEta > 0 && iGenPt > 0 && iRecoPt > 0 && iHibin > 0)  continue;

                    // for histograms with a particular dependence,
                    // a single index is used in the multidimensional array of energyScaleHist objects is used.
                    // Example : for an energy scale histogram with eta dependence (eta is the x-axis), there will not be different histograms
                    // with different eta ranges.
                    // There will be objects like : hist[ENERGYSCALE::kETA][0][iGenPt][iRecoPt][iHibin]
                    // but not like : hist[ENERGYSCALE::kETA][1][iGenPt][iRecoPt][iHibin]
                    // in general there will be no object hist[someIndex][iEta][iGenPt][iRecoPt][iHibin] such that iEta, iGenpT, iRecoPt, iHibin > 0

                    std::vector<int> indices {iEta, iGenPt, iRecoPt, iHibin};
                    std::vector<int> eScale {ENERGYSCALE::kETA, ENERGYSCALE::kGENPT, ENERGYSCALE::kRECOPT, ENERGYSCALE::kHIBIN};
                    for (int unsigned iEScale = 0; iEScale < eScale.size(); ++iEScale) {
                        if (indices.at(iEScale) == 0)
                        {
                            int eScaleDep = eScale.at(iEScale);
                            // write histograms with a particular dependence
                            if (hist[eScaleDep][iEta][iGenPt][iRecoPt][iHibin].hInitialized) {
                                hist[eScaleDep][iEta][iGenPt][iRecoPt][iHibin].h->SetMarkerStyle(kFullCircle);
                                hist[eScaleDep][iEta][iGenPt][iRecoPt][iHibin].h->Write();
                            }
                            if (hist[eScaleDep][iEta][iGenPt][iRecoPt][iHibin].h2DcorrInitialized)
                                hist[eScaleDep][iEta][iGenPt][iRecoPt][iHibin].h2Dcorr->Write();

                            if (!hist[eScaleDep][iEta][iGenPt][iRecoPt][iHibin].h2Dinitialized) continue;

                            std::string canvasName = "";
                            canvasName = replaceAll(hist[eScaleDep][iEta][iGenPt][iRecoPt][iHibin].h2D->GetName(), "h2D", "cnv2D");
                            c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
                            c->cd();
                            setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
                            hist[eScaleDep][iEta][iGenPt][iRecoPt][iHibin].h2D->SetStats(false);
                            hist[eScaleDep][iEta][iGenPt][iRecoPt][iHibin].h2D->Draw("colz");
                            hist[eScaleDep][iEta][iGenPt][iRecoPt][iHibin].h2D->Write();
                            setCanvasFinal(c);
                            c->Write();
                            c->Close();         // do not use Delete() for TCanvas.

                            TObjArray aSlices;
                            hist[eScaleDep][iEta][iGenPt][iRecoPt][iHibin].h2D->FitSlicesY(0,0,-1,0,"Q LL m", &aSlices);

                            std::string name = hist[eScaleDep][iEta][iGenPt][iRecoPt][iHibin].name.c_str();
                            std::string title = hist[eScaleDep][iEta][iGenPt][iRecoPt][iHibin].title.c_str();
                            std::string titleX = hist[eScaleDep][iEta][iGenPt][iRecoPt][iHibin].titleX.c_str();
                            // energy scale
                            canvasName = Form("cnv_eScale_%s", name.c_str());
                            c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
                            c->cd();
                            setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);

                            hist[eScaleDep][iEta][iGenPt][iRecoPt][iHibin].h1D[0] = (TH1D*)aSlices.At(1)->Clone(Form("h1D_eScale_%s", name.c_str()));
                            hist[eScaleDep][iEta][iGenPt][iRecoPt][iHibin].h1D[0]->SetTitle(title.c_str());
                            hist[eScaleDep][iEta][iGenPt][iRecoPt][iHibin].h1D[0]->SetXTitle(titleX.c_str());
                            setTH1_energyScale(hist[eScaleDep][iEta][iGenPt][iRecoPt][iHibin].h1D[0], titleOffsetX, titleOffsetY);
                            if (yMax > yMin)
                                hist[eScaleDep][iEta][iGenPt][iRecoPt][iHibin].h1D[0]->SetAxisRange(yMin, yMax, "Y");
                            hist[eScaleDep][iEta][iGenPt][iRecoPt][iHibin].h1D[0]->Draw("e");
                            hist[eScaleDep][iEta][iGenPt][iRecoPt][iHibin].h1D[0]->Write();

                            // draw line y = 1
                            float x1 = hist[eScaleDep][iEta][iGenPt][iRecoPt][iHibin].h1D[0]->GetXaxis()->GetXmin();
                            float x2 = hist[eScaleDep][iEta][iGenPt][iRecoPt][iHibin].h1D[0]->GetXaxis()->GetXmax();
                            TLine line(x1, 1, x2,1);
                            line.SetLineStyle(kDashed);
                            line.Draw();
                            setCanvasFinal(c);
                            c->Write();
                            c->Close();         // do not use Delete() for TCanvas.

                            // width of energy scale
                            canvasName = Form("cnv_eRes_%s", name.c_str());
                            c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
                            c->cd();
                            setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
                            hist[eScaleDep][iEta][iGenPt][iRecoPt][iHibin].h1D[1] = (TH1D*)aSlices.At(2)->Clone(Form("h1D_eRes_%s", name.c_str()));
                            hist[eScaleDep][iEta][iGenPt][iRecoPt][iHibin].h1D[1]->SetTitle(title.c_str());
                            hist[eScaleDep][iEta][iGenPt][iRecoPt][iHibin].h1D[1]->SetXTitle(titleX.c_str());
                            setTH1_energyWidth(hist[eScaleDep][iEta][iGenPt][iRecoPt][iHibin].h1D[1], titleOffsetX, titleOffsetY);
                            hist[eScaleDep][iEta][iGenPt][iRecoPt][iHibin].h1D[1]->Draw("e");
                            hist[eScaleDep][iEta][iGenPt][iRecoPt][iHibin].h1D[1]->Write();
                            setCanvasFinal(c);
                            c->Write();
                            c->Close();         // do not use Delete() for TCanvas.
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
        photonEnergyScale(argv[1], argv[2], argv[3]);
        return 0;
    }
    else if (argc == 3) {
        photonEnergyScale(argv[1], argv[2]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./photonEnergyScale.exe <configFile> <inputFile> <outputFile>"
                << std::endl;
        return 1;
    }
}

