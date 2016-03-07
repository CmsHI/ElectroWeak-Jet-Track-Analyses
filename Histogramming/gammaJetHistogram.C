#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TCut.h>
#include <TH1D.h>
#include <TMath.h>
#include <TEventList.h>

#include <vector>
#include <string>
#include <iostream>

#include "../TreeHeaders/CutConfigurationTree.h"
#include "../Plotting/commonUtility.h"
#include "interface/correlationHist.h"
#include "../Utilities/eventUtil.h"
#include "../Utilities/interface/CutConfigurationParser.h"
#include "../Utilities/interface/InputConfigurationParser.h"

const std::vector<std::string> correlationHistNames   {"xjg", "dphi", "ptJet"};
const std::vector<std::string> correlationHistFormulas{"xjg", "abs(dphi)", "jtpt"};
const std::vector<std::string> correlationHistTitleX  {"p^{Jet}_{T}/p^{#gamma}_{T}", "#Delta#phi_{J#gamma}", "p^{Jet}_{T}"};
const std::vector<std::string> correlationHistTitleY_final_normalized{"#frac{1}{N_{#gamma}} #frac{dN_{J#gamma}}{dx_{J#gamma}}",
                                                                      "#frac{1}{N_{#gamma}} #frac{dN_{J#gamma}}{d#Delta#phi}",
                                                                      "#frac{1}{N_{#gamma}} #frac{dN_{J#gamma}}{dp^{Jet}_{T}}"};
const std::vector<int>         nBinsx{40, 20,          60};
const std::vector<double>      xlow  {0,  0,           0};
const std::vector<double>      xup   {5,  TMath::Pi(), 300};
const std::vector<double>      xlow_final{0,  0,           0};
const std::vector<double>      xup_final {2,  TMath::Pi(), 200};
const std::vector<bool> isAwaySideJets {true,  false, true};  // whether the observable is plotted for inclusive jets in the away side
const std::vector<bool> isSingleJet    {false, false, false}; // whether the observable is plotted once per event

void gammaJetHistogram(const TString configFile, const TString inputFile, const TString outputFile = "gammaJetHistogram.root");

void gammaJetHistogram(const TString configFile, const TString inputFile, const TString outputFile)
{
    std::cout<<"running gammaJetHistogram()"<<std::endl;
    std::cout<<"configFile  = "<< configFile.Data() <<std::endl;
    std::cout<<"inputFile   = "<< inputFile.Data() <<std::endl;
    std::cout<<"outputFile  = "<< outputFile.Data() <<std::endl;

    InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

    // input configuration
    int collision;
    if (configInput.isValid) {
        collision = configInput.proc[INPUT::kHISTOGRAM].i[INPUT::k_collisionType];
    }
    else {
        collision = COLL::kPP;
    }
    // verbose about input configuration
    std::cout<<"Input Configuration :"<<std::endl;
    const char* collisionName =  getCollisionTypeName((COLL::TYPE)collision).c_str();
    std::cout << "collision = " << collisionName << std::endl;

    bool isMC = collisionIsMC((COLL::TYPE)collision);
    bool isHI = collisionIsHI((COLL::TYPE)collision);

    // observable bins
    std::vector<float> bins_pt[2];          // array of vectors for eta bins, each array element is a vector.
    std::vector<int>   bins_hiBin[2];       // array of vectors for hiBin bins, each array element is a vector.
    // event cuts/weights
    int doEventWeight;
    std::string eventWeight;    // weight to be used for histogram entries
                                // current purpose of this variable is for weighting events from MC samples.
    // photon cuts
    std::string trigger;
    float cut_phoHoverE;
    float cut_pho_ecalClusterIsoR4;
    float cut_pho_hcalRechitIsoR4;
    float cut_pho_trackIsoR4PtCut20;
    float cut_phoSigmaIEtaIEta;
    float cut_sumIso;

    // isolation for PP
    float cut_phoHOverE_EB;         // Barrel
    float cut_phoSigmaIEtaIEta_EB;
    float cut_pfcIso4_EB;
    float cut_pfnIso4_c0_EB;
    float cut_pfnIso4_c1_EB;
    float cut_pfnIso4_c2_EB;
    float cut_pfpIso4_c0_EB;
    float cut_pfpIso4_c1_EB;
    float cut_phoHOverE_EE;         // Endcap
    float cut_phoSigmaIEtaIEta_EE;
    float cut_pfcIso4_EE;
    float cut_pfnIso4_c0_EE;
    float cut_pfnIso4_c1_EE;
    float cut_pfnIso4_c2_EE;
    float cut_pfpIso4_c0_EE;
    float cut_pfpIso4_c1_EE;

    // jet cuts
    std::string jetCollection;
    float cut_jetpt;
    float cut_jeteta;
    int   cut_jetID;
    // gammaJet cuts
    float cut_awayRange;
    float cut_awayRange_lt;
    float cut_dR;
    // process cuts
    int nEventsToMix;
    if (configCuts.isValid) {
        bins_pt[0] = ConfigurationParser::ParseListFloat(
                configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_pt_gt]);
        bins_pt[1] = ConfigurationParser::ParseListFloat(
                configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_pt_lt]);
        bins_hiBin[0] = ConfigurationParser::ParseListInteger(
                configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_gt]);
        bins_hiBin[1] = ConfigurationParser::ParseListInteger(
                configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_lt]);

        doEventWeight = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].i[CUTS::EVT::k_doEventWeight];
        eventWeight = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_eventWeight].c_str();

        trigger = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_trigger_gammaJet].c_str();

        cut_phoHoverE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_phoHoverE];
        cut_pho_ecalClusterIsoR4 = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pho_ecalClusterIsoR4];
        cut_pho_hcalRechitIsoR4 = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pho_hcalRechitIsoR4];
        cut_pho_trackIsoR4PtCut20 = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pho_trackIsoR4PtCut20];
        cut_phoSigmaIEtaIEta = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_phoSigmaIEtaIEta];
        cut_sumIso = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_sumIso];

        // Barrel
        cut_phoHOverE_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_phoHOverE_EB];
        cut_phoSigmaIEtaIEta_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_phoSigmaIEtaIEta_EB];
        cut_pfcIso4_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pfcIso4_EB];
        cut_pfnIso4_c0_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pfnIso4_c0_EB];
        cut_pfnIso4_c1_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pfnIso4_c1_EB];
        cut_pfnIso4_c2_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pfnIso4_c2_EB];
        cut_pfpIso4_c0_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pfpIso4_c0_EB];
        cut_pfpIso4_c1_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pfpIso4_c1_EB];
        // Endcap
        cut_phoHOverE_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_phoHOverE_EE];
        cut_phoSigmaIEtaIEta_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_phoSigmaIEtaIEta_EE];
        cut_pfcIso4_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pfcIso4_EE];
        cut_pfnIso4_c0_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pfnIso4_c0_EE];
        cut_pfnIso4_c1_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pfnIso4_c1_EE];
        cut_pfnIso4_c2_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pfnIso4_c2_EE];
        cut_pfpIso4_c0_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pfpIso4_c0_EE];
        cut_pfpIso4_c1_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pfpIso4_c1_EE];

        jetCollection = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kJET].s[CUTS::JET::k_jetCollection];
        cut_jetpt  = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kJET].f[CUTS::JET::k_pt];
        cut_jeteta = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kJET].f[CUTS::JET::k_eta];
        cut_jetID  = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kJET].i[CUTS::JET::k_jetID];

        cut_awayRange = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kGAMMAJET].f[CUTS::GJT::k_awayRange];
        cut_awayRange_lt = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kGAMMAJET].f[CUTS::GJT::k_awayRange_lt];
        cut_dR = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kGAMMAJET].f[CUTS::GJT::k_dR];

        nEventsToMix = configCuts.proc[CUTS::kSKIM].obj[CUTS::kGAMMAJET].i[CUTS::GJT::k_nEventsToMix];
    }
    else {  // default configuration for photons
        bins_pt[0].push_back(60);
        bins_pt[1].push_back(-1);
        bins_hiBin[0].push_back(0);
        bins_hiBin[0].push_back(0);
        bins_hiBin[1].push_back(200);
        bins_hiBin[1].push_back(60);

        trigger = "HLT_HISinglePhoton20_Eta1p5_v1";

        cut_phoHoverE = 0.1;
        cut_pho_ecalClusterIsoR4 = 4.2;
        cut_pho_hcalRechitIsoR4 = 2.2;
        cut_pho_trackIsoR4PtCut20 = 2;
        cut_phoSigmaIEtaIEta = 0.01;
        cut_sumIso = 6;

        // default photon isolation for PP is "Loose".
        cut_phoHOverE_EB = 0.05;            // Barrel
        cut_phoSigmaIEtaIEta_EB = 0.0102;
        cut_pfcIso4_EB = 3.32;
        cut_pfnIso4_c0_EB = 1.92;
        cut_pfnIso4_c1_EB = 0.014;
        cut_pfnIso4_c2_EB = 0.000019;
        cut_pfpIso4_c0_EB = 0.81;
        cut_pfpIso4_c1_EB = 0.0053;
        cut_phoHOverE_EE = 0.05;            // Endcap
        cut_phoSigmaIEtaIEta_EE = 0.0274;
        cut_pfcIso4_EE = 1.97;
        cut_pfnIso4_c0_EE = 11.86;
        cut_pfnIso4_c1_EE = 0.0139;
        cut_pfnIso4_c2_EE = 0.000025;
        cut_pfpIso4_c0_EE = 0.83;
        cut_pfpIso4_c1_EE = 0.0034;

        jetCollection = "akPu3PFJetAnalyzer";
        cut_jetpt = 40;
        cut_jeteta = 1.6;
        cut_jetID = 0;      // jetID >= 0

        cut_awayRange = 7./8.;
        cut_awayRange_lt = 1;
        cut_dR = 0.4;

        nEventsToMix = 1;
    }
    // default values
    if (eventWeight.size() == 0) eventWeight = "1";
    if (cut_awayRange_lt == 0) cut_awayRange_lt = 1;

    int nBins_pt = bins_pt[0].size();         // assume <myvector>[0] and <myvector>[1] have the same size.
    int nBins_hiBin = bins_hiBin[0].size();     // assume <myvector>[0] and <myvector>[1] have the same size.
    // verbose about cut configuration
    std::cout<<"Cut Configuration :"<<std::endl;
    std::cout << "nBins_pt = " << nBins_pt << std::endl;
    for (int i=0; i<nBins_pt; ++i) {
        std::cout << Form("bins_pt[%d] = [%.1f, %.1f)", i, bins_pt[0].at(i), bins_pt[1].at(i)) << std::endl;
    }
    std::cout << "nBins_hiBin = " << nBins_hiBin << std::endl;
    for (int i=0; i<nBins_hiBin; ++i) {
        std::cout << Form("bins_hiBin[%d] = [%d, %d)", i, bins_hiBin[0].at(i), bins_hiBin[1].at(i)) << std::endl;
    }

    std::cout<<"doEventWeight = "<<doEventWeight<<std::endl;
    if (doEventWeight > 0) {
        std::cout<<"eventWeight = "<<eventWeight.c_str()<<std::endl;
    }

    std::cout<<"trigger    = "<<trigger.c_str()<<std::endl;

    if (isHI) {
        std::cout<<"cut_phoHoverE             = "<< cut_phoHoverE <<std::endl;
        std::cout<<"cut_pho_ecalClusterIsoR4  = "<< cut_pho_ecalClusterIsoR4 <<std::endl;    // not used
        std::cout<<"cut_pho_hcalRechitIsoR4   = "<< cut_pho_hcalRechitIsoR4 <<std::endl;     // not used
        std::cout<<"cut_pho_trackIsoR4PtCut20 = "<< cut_pho_trackIsoR4PtCut20 <<std::endl;   // not used
        std::cout<<"cut_phoSigmaIEtaIEta      = "<< cut_phoSigmaIEtaIEta <<std::endl;
        std::cout<<"cut_sumIso                = "<< cut_sumIso <<std::endl;
    }
    else {
        std::cout<<"Barrel :"<<std::endl;
        std::cout<<"cut_phoHOverE_EB        = "<< cut_phoHOverE_EB <<std::endl;
        std::cout<<"cut_phoSigmaIEtaIEta_EB = "<< cut_phoSigmaIEtaIEta_EB <<std::endl;
        std::cout<<"cut_pfcIso4_EB          = "<< cut_pfcIso4_EB <<std::endl;
        std::cout<<"cut_pfnIso4_c0_EB       = "<< cut_pfnIso4_c0_EB <<std::endl;
        std::cout<<"cut_pfnIso4_c1_EB       = "<< cut_pfnIso4_c1_EB <<std::endl;
        std::cout<<"cut_pfnIso4_c2_EB       = "<< cut_pfnIso4_c2_EB <<std::endl;
        std::cout<<"cut_pfpIso4_c0_EB       = "<< cut_pfpIso4_c0_EB <<std::endl;
        std::cout<<"cut_pfpIso4_c1_EB       = "<< cut_pfpIso4_c1_EB <<std::endl;

        std::cout<<"Endcap :"<<std::endl;
        std::cout<<"cut_phoHOverE_EE        = "<< cut_phoHOverE_EE <<std::endl;
        std::cout<<"cut_phoSigmaIEtaIEta_EE = "<< cut_phoSigmaIEtaIEta_EE <<std::endl;
        std::cout<<"cut_pfcIso4_EE          = "<< cut_pfcIso4_EE <<std::endl;
        std::cout<<"cut_pfnIso4_c0_EE       = "<< cut_pfnIso4_c0_EE <<std::endl;
        std::cout<<"cut_pfnIso4_c1_EE       = "<< cut_pfnIso4_c1_EE <<std::endl;
        std::cout<<"cut_pfnIso4_c2_EE       = "<< cut_pfnIso4_c2_EE <<std::endl;
        std::cout<<"cut_pfpIso4_c0_EE       = "<< cut_pfpIso4_c0_EE <<std::endl;
        std::cout<<"cut_pfpIso4_c1_EE       = "<< cut_pfpIso4_c1_EE <<std::endl;
    }

    std::cout<<"jetCollection             = "<< jetCollection.c_str() <<std::endl;
    std::cout<<"cut_jetpt                 = "<< cut_jetpt <<std::endl;
    std::cout<<"cut_jeteta                = "<< cut_jeteta <<std::endl;
    std::cout<<"cut_jetID                 = "<< cut_jetID <<std::endl;

    std::cout<<"cut_awayRange             = "<< cut_awayRange << " * PI" <<std::endl;
    std::cout<<"cut_awayRange_lt          = "<< cut_awayRange_lt << " * PI" <<std::endl;
    std::cout<<"cut_dR                    = "<< cut_dR <<std::endl;

    std::cout<<"nEventsToMix              = "<< nEventsToMix <<std::endl;

    //set the actual awayRange cut
    cut_awayRange = cut_awayRange * TMath::Pi();
    cut_awayRange_lt = cut_awayRange_lt * TMath::Pi();

    TFile *input = new TFile(inputFile, "READ");
    TTree *tHlt = (TTree*)input->Get("hltTree");
    TTree *tPho = (TTree*)input->Get("EventTree");    // photons
    TTree *tJet = (TTree*)input->Get(jetCollection.c_str());
    TTree *tgj  = (TTree*)input->Get(Form("gamma_%s", jetCollection.c_str()));
    TTree *tHiEvt = (TTree*)input->Get("HiEvt");       // HiEvt tree will be placed in PP forest as well.

    if (!tJet) {
        std::cout<<"following jet collection is not found in the input file : " << jetCollection.c_str() << std::endl;
        std::cout<<"exiting"<< std::endl;
        return;
    }

    TTree *tJetMB;
    TTree *tgjMB;
    // check the existence of HI specific trees in "gammaJetSkim.root" file
    bool hasJetsMB = false;
    bool hasGammaJetMB = false;
    if (isHI) {
        input->GetObject(Form("%sMB", jetCollection.c_str()), tJetMB);
        input->GetObject(Form("gamma_%sMB", jetCollection.c_str()), tgjMB);

        if (tJetMB) hasJetsMB = true;
        if (tgjMB)  hasGammaJetMB = true;
    }
    else {
        tJetMB = 0;
        tgjMB  = 0;
    }

    tgj->AddFriend(tHlt, "Hlt");
    tgj->AddFriend(tPho, "Pho");
    tgj->AddFriend(tJet, "Jet");
    tgj->AddFriend(tHiEvt, "HiEvt");

    // relation of trees from MB mixing block
    if (hasJetsMB && hasGammaJetMB) {
        tgjMB->AddFriend(tHlt, "Hlt");
        tgjMB->AddFriend(tPho, "Pho");
        tgjMB->AddFriend(tJetMB, "Jet");
        tgjMB->AddFriend(tHiEvt, "HiEvt");
    }

    TFile* output = new TFile(outputFile, "UPDATE");
    // histograms will be put under a directory whose name is the type of the collision
    if(!output->GetKey(collisionName)) output->mkdir(collisionName, Form("input file is %s", inputFile.Data()));
    output->cd(collisionName);
    std::cout<<"histograms will be put under directory : " << collisionName << std::endl;

    TTree *configTree = setupConfigurationTreeForWriting(configCuts);

    double purity[nBins_pt][nBins_hiBin];   // fixed for the moment.
    for (int i = 0; i<nBins_pt; ++i){
        for (int j = 0; j<nBins_hiBin; ++j){
            purity[i][j] = 0.75;
        }
    }

    TH1::SetDefaultSumw2();
    int nCorrHist = correlationHistNames.size();
    correlationHist corrHists[nCorrHist][nBins_pt][nBins_hiBin];

    // prepare histogram names for xjg, abs(dphi) and jet pt
    // if the collision is not HI, then cannot split it into hiBins.
    if (!isHI) nBins_hiBin = 1;
    for (int iHist=0; iHist<nCorrHist; ++iHist){
    for (int i=0; i<nBins_pt; ++i){
        for(int j=0; j<nBins_hiBin; ++j){
            corrHists[iHist][i][j].name = Form("%s_ptBin%d_hibin%d", correlationHistNames.at(iHist).c_str(), i, j);

            for (int iCorr = 0; iCorr < CORR::kN_CORRFNC; ++iCorr) {
                for (int jCorr = 0; jCorr < CORR::kN_CORRFNC; ++jCorr) {

                  std::string subHistName = Form("%s_ptBin%d_hibin%d_%s_%s", correlationHistNames.at(iHist).c_str(), i, j,
                                                 CORR::CORR_PHO_LABELS[iCorr].c_str(), CORR::CORR_JET_LABELS[jCorr].c_str());
                  corrHists[iHist][i][j].h1D_name[iCorr][jCorr] = subHistName.c_str();
                  corrHists[iHist][i][j].h1D[iCorr][jCorr] = new TH1D(Form("h1D_%s", subHistName.c_str()),"",
                                                                      nBinsx.at(iHist), xlow.at(iHist), xup.at(iHist));

                  corrHists[iHist][i][j].h1D_titleX[iCorr][jCorr] = correlationHistTitleX.at(iHist).c_str();
                  corrHists[iHist][i][j].h1D_titleY_final_norm[iCorr][jCorr] = correlationHistTitleY_final_normalized.at(iHist).c_str();
                }
            }
        }
    }
    }

    // histograms to store the number of photon events, not photon-Jet event
    // those histograms have a single bin whose content is the number of photon events
    // they are just a tool to store number.
    TH1D* h_nPho[nBins_pt][nBins_hiBin][2];
    for (int i=0; i<nBins_pt; ++i){
        for(int j=0; j<nBins_hiBin; ++j){

            std::string histNamePhoRAW = Form("h_nPho_ptBin%d_hibin%d_%s", i, j, CORR::CORR_PHO_LABELS[CORR::kRAW].c_str());
            h_nPho[i][j][CORR::kRAW] = new TH1D(histNamePhoRAW.c_str(), "", 1, 0, 1);

            std::string histNamePhoBKG = Form("h_nPho_ptBin%d_hibin%d_%s", i, j, CORR::CORR_PHO_LABELS[CORR::kBKG].c_str());
            h_nPho[i][j][CORR::kBKG] = new TH1D(histNamePhoBKG.c_str(), "", 1, 0, 1);
        }
    }

    // histograms with pt bins on x-axis
    std::vector<std::string> correlationHistNames_ptBinAll = {"rjg", "xjg_mean"};       // histograms where x-axis is pt bins
    int nCorrHist_ptBinAll = correlationHistNames_ptBinAll.size();
    correlationHist corrHists_ptBinAll[nCorrHist_ptBinAll][nBins_hiBin];
    // prepare histogram names for rjg and <xjg>
    const int nBins_rjg = 4;
    const int nBins_xjg_mean= 4;
    double bins_rjg[nBins_rjg+1] =           {40, 50, 60, 80, 120};
    double bins_xjg_mean[nBins_xjg_mean+1] = {40, 50, 60, 80, 120};

    for (int j=0; j<nBins_hiBin; ++j){
        for (int iCorr = 0; iCorr < CORR::kN_CORRFNC; ++iCorr) {
            for (int jCorr = 0; jCorr < CORR::kN_CORRFNC; ++jCorr) {

                // rjg
                std::string subHistName;
                subHistName = Form("%s_ptBinAll_hibin%d_%s_%s", correlationHistNames_ptBinAll.at(0).c_str(), j,
                                                               CORR::CORR_PHO_LABELS[iCorr].c_str(), CORR::CORR_JET_LABELS[jCorr].c_str());
                corrHists_ptBinAll[0][j].h1D_name[iCorr][jCorr] = subHistName.c_str();
                corrHists_ptBinAll[0][j].h1D[iCorr][jCorr] = new TH1D(Form("h1D_%s", subHistName.c_str()), "",nBins_rjg, bins_rjg);

                // x_jg_mean
                subHistName = Form("%s_ptBinAll_hibin%d_%s_%s", correlationHistNames_ptBinAll.at(1).c_str(), j,
                                                                               CORR::CORR_PHO_LABELS[iCorr].c_str(), CORR::CORR_JET_LABELS[jCorr].c_str());
                corrHists_ptBinAll[1][j].h1D_name[iCorr][jCorr] = subHistName.c_str();
                corrHists_ptBinAll[1][j].h1D[iCorr][jCorr] = new TH1D(Form("h1D_%s", subHistName.c_str()), "",nBins_xjg_mean, bins_xjg_mean);
            }
        }
    }

    // selections for different signal regions that are applied to every histogram
    // selections for photon regions
    TCut selectionIso = "";
    if (isHI)
    {
        selectionIso = selectionIso && Form("(pho_ecalClusterIsoR4[phoIdx] + pho_hcalRechitIsoR4[phoIdx] + pho_trackIsoR4PtCut20[phoIdx]) < %f", cut_sumIso);
        selectionIso = selectionIso && Form("phoHoverE[phoIdx] < %f", cut_phoHoverE);
    }
    else {  // PP or PA
        // https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedPhotonIdentificationRun2#SPRING15_selections_25_ns
        TCut selectionIso_EB = "";
        TCut selectionIso_EE = "";

        std::string cut_pfpIso4_EB_str = Form("(%f + %f * phoEt[phoIdx])", cut_pfpIso4_c0_EB, cut_pfpIso4_c1_EB);
        std::string cut_pfnIso4_EB_str = Form("(%f + %f * phoEt[phoIdx] + %f * phoEt[phoIdx]*phoEt[phoIdx])",
                                                 cut_pfnIso4_c0_EB, cut_pfnIso4_c1_EB, cut_pfnIso4_c2_EB);
        selectionIso_EB = selectionIso_EB && Form("phoHoverE[phoIdx] < %f", cut_phoHOverE_EB);
        selectionIso_EB = selectionIso_EB && Form("phoSigmaIEtaIEta[phoIdx] < %f", cut_phoSigmaIEtaIEta_EB);
        selectionIso_EB = selectionIso_EB && Form("pfcIso4[phoIdx] < %f", cut_pfcIso4_EB);
        selectionIso_EB = selectionIso_EB && Form("pfnIso4[phoIdx] < %s", cut_pfnIso4_EB_str.c_str());
        selectionIso_EB = selectionIso_EB && Form("pfpIso4[phoIdx] < %s", cut_pfpIso4_EB_str.c_str());

        std::string cut_pfnIso4_EE_str = Form("(%f + %f * phoEt[phoIdx] + %f * phoEt[phoIdx]*phoEt[phoIdx])",
                                                 cut_pfnIso4_c0_EE, cut_pfnIso4_c1_EE, cut_pfnIso4_c2_EE);
        std::string cut_pfpIso4_EE_str = Form("(%f + %f * phoEt[phoIdx])", cut_pfpIso4_c0_EE, cut_pfpIso4_c1_EE);
        selectionIso_EE = selectionIso_EE && Form("phoHoverE[phoIdx] < %f", cut_phoHOverE_EE);
        selectionIso_EE = selectionIso_EE && Form("phoSigmaIEtaIEta[phoIdx] < %f", cut_phoSigmaIEtaIEta_EE);
        selectionIso_EE = selectionIso_EE && Form("pfcIso4[phoIdx] < %f", cut_pfcIso4_EE);
        selectionIso_EE = selectionIso_EE && Form("pfnIso4[phoIdx] < %s", cut_pfnIso4_EE_str.c_str());
        selectionIso_EE = selectionIso_EE && Form("pfpIso4[phoIdx] < %s", cut_pfpIso4_EE_str.c_str());

        float eta_EB = 1.4791;
        float eta_EE = 2.5;
        TCut selection_EB_eta = Form("abs(phoEta[phoIdx]) < %f", eta_EB);
        selectionIso_EB = selectionIso_EB && selection_EB_eta;
        TCut selection_EE_eta = Form("abs(phoEta[phoIdx]) > %f && abs(phoEta[phoIdx]) < %f", eta_EB, eta_EE);
        selectionIso_EE = selectionIso_EE && selection_EE_eta;
        TCut selection_EB_EE = selectionIso_EB || selectionIso_EE;
        selectionIso = selectionIso && selection_EB_EE;
    }

    TCut selectionPhoCORR[CORR::kN_CORRFNC];
    selectionPhoCORR[CORR::kRAW] = "phoSigmaIEtaIEta[phoIdx] < 0.01";
    selectionPhoCORR[CORR::kBKG] = "phoSigmaIEtaIEta[phoIdx] > 0.011 && phoSigmaIEtaIEta[phoIdx] < 0.017";
    // selection for jet regions
    // jet from bkg region are already separated from raw region.
    // no additional selection for jets. just use different trees.
    std::cout<<"####################"<<std::endl;
    std::cout<<"tgj->GetEntries() = "<<tgj->GetEntries()<<std::endl;
    if (trigger.compare("") != 0 && !isMC) {
        std::cout<<"tgj->GetEntries(trigger==1) = "<<tgj->GetEntries(Form("%s == 1",trigger.c_str()))<<std::endl;
    }
    else {
        std::cout<<"tgj->GetEntries(trigger==1) is skipped because either no trigger is specified or the data is coming from MC."<<std::endl;
    }
    std::cout<<"####################"<<std::endl;

    // use "EventList" approach to make the "draw()" shorter
    // store original eventlist
    const char* elist_tmp_name="elist_tmp";
    tgj->Draw(Form(">> %s", elist_tmp_name));
    TEventList* eventlist = ((TEventList*)gDirectory->Get(elist_tmp_name)->Clone("elist_Original"));
    gDirectory->Delete(elist_tmp_name);

    std::string eventlistNames[2][nBins_pt][nBins_hiBin];
    TEventList* elists[2][nBins_pt][nBins_hiBin];
    bool isEventlistCreated[nBins_pt][nBins_hiBin];     // event lists are independent of the correlation histogram.
    // set eventlist once for a given bin, then reuse it for subsequent correlations, do not recalculate.
    for(int i=0; i<nBins_pt; ++i){
        for(int j=0; j<nBins_hiBin; ++j){
            eventlistNames[CORR::kRAW][i][j] = Form("eventlist_PhoRAW_ptBin%d_hibin%d", i, j);  // CORR::kRAW = 0
            eventlistNames[CORR::kBKG][i][j] = Form("eventlist_PhoBKG_ptBin%d_hibin%d", i, j);  // CORR::kBKG = 1
            isEventlistCreated[i][j] = false;
        }
    }

    // HISTOGRAMMING BLOCK
    TCanvas* c = new TCanvas("cnv","",600,600);
    for(int iHist = 0; iHist<nCorrHist; ++iHist)
    {
    std::cout<<"####################"<<std::endl;
    std::cout<<"histogramming : "<<correlationHistNames.at(iHist).c_str()<<std::endl;
    for(int i=0; i<nBins_pt; ++i){
        for(int j=0; j<nBins_hiBin; ++j){

            std::cout<<"##########"<<std::endl;
            std::cout<<Form("histogramming : ptBin%d HiBin%d", i, j)<<std::endl;

            if(j>0 && !isHI) continue;

            // event selection
            TCut selection_event = Form("%s == 1", trigger.c_str());
            if (isMC) selection_event = "1==1";
            if (isHI) {
                selection_event = selection_event && Form("hiBin >= %d && hiBin < %d", bins_hiBin[0].at(j), bins_hiBin[1].at(j));
            }

            // photon cuts were applied in the analysis code
            // photon selection
            TCut selectionPho;
            if (bins_pt[1].at(i) >= 0)  selectionPho = Form("phoEt[phoIdx] >= %f && phoEt[phoIdx] < %f", bins_pt[0].at(i), bins_pt[1].at(i));
            else                        selectionPho = Form("phoEt[phoIdx] >= %f", bins_pt[0].at(i));
            selectionPho = selectionPho && selectionIso;

            // jet selection
            TCut selectionJet = "";
            // special selection
            if (isAwaySideJets.at(iHist)) {  // no awayRange cut for dphi histograms
                selectionJet = selectionJet && Form("abs(dphi) > %f ", cut_awayRange);
                selectionJet = selectionJet && Form("abs(dphi) <= %f ", cut_awayRange_lt);
            }
            selectionJet = selectionJet && Form("dR >= %f", cut_dR);
            selectionJet = selectionJet && Form("jtpt > %f", cut_jetpt);
            selectionJet = selectionJet && Form("abs(jteta) < %f", cut_jeteta);
            selectionJet = selectionJet && Form("jetID >= %d", cut_jetID);
            if (isSingleJet.at(iHist)) {  // select gammaJet events only, do not select inclusive jets
                selectionJet = Form("Max$(%s)>0", selectionJet.GetTitle());
            }

            TCut selection_Barrel = "1";    // no extra selection at the moment
            TCut selection_Endcap = "1";    // no extra selection at the moment

            TCut selection = "";
            selection = selection && selection_event;
            selection = selection && selectionPho;
            selection = selection && selectionJet;

            for (int jCorr = 0; jCorr < CORR::kN_CORRFNC; ++jCorr) {
                    if (jCorr > CORR::kRAW && !isHI)  continue;      // no jet background for non-HI
                    corrHists[iHist][i][j].selections[CORR::kRAW][jCorr] = selection  && selectionPhoCORR[CORR::kRAW];
                    corrHists[iHist][i][j].selections[CORR::kBKG][jCorr] = selection  && selectionPhoCORR[CORR::kBKG];
            }

            std::cout<< "corrHists[iHist][i][j].h1D[CORR::kRAW][CORR::kRAW]->GetName() = " <<  corrHists[iHist][i][j].h1D[CORR::kRAW][CORR::kRAW]->GetName() <<std::endl;
            std::cout<< "corrHists[iHist][i][j].h1D[CORR::kBKG][CORR::kRAW]->GetName() = " <<  corrHists[iHist][i][j].h1D[CORR::kBKG][CORR::kRAW]->GetName() <<std::endl;
            std::cout<< "selections[CORR::kRAW][CORR::kRAW] = " << corrHists[iHist][i][j].selections[CORR::kRAW][CORR::kRAW].GetTitle() <<std::endl;
            std::cout<< "selections[CORR::kBKG][CORR::kRAW] = " << corrHists[iHist][i][j].selections[CORR::kBKG][CORR::kRAW].GetTitle() <<std::endl;

            // use "EventList" approach to make the "draw()" shorter
            if (!isEventlistCreated[i][j]) {
                tgj->Draw(Form(">> %s", eventlistNames[CORR::kRAW][i][j].c_str()), (selection_event && selectionPho && selectionPhoCORR[CORR::kRAW]).GetTitle());
                tgj->Draw(Form(">> %s", eventlistNames[CORR::kBKG][i][j].c_str()), (selection_event && selectionPho && selectionPhoCORR[CORR::kBKG]).GetTitle());
                elists[CORR::kRAW][i][j] = (TEventList*)gDirectory->Get(eventlistNames[CORR::kRAW][i][j].c_str());
                elists[CORR::kBKG][i][j] = (TEventList*)gDirectory->Get(eventlistNames[CORR::kBKG][i][j].c_str());
                isEventlistCreated[i][j] = true;
                // event lists are independent of the correlation histogram.
                // set eventlist once for a given bin, then reuse it for subsequent correlations, do not recalculate.
            }

            // number of events with photons, not necessarily photon-jet events
            tgj->SetEventList(elists[CORR::kRAW][i][j]);
            corrHists[iHist][i][j].nEntriesPho[CORR::kRAW][CORR::kRAW] = tgj->GetEventList()->GetN();
            tgj->SetEventList(eventlist);      // restore the original event list
            tgj->SetEventList(elists[CORR::kBKG][i][j]);
            corrHists[iHist][i][j].nEntriesPho[CORR::kBKG][CORR::kRAW] = tgj->GetEventList()->GetN();
            tgj->SetEventList(eventlist);      // restore the original event list
            // nEntriesPho[][CORR::kRAW] = nEntriesPho[][CORR::kBKG] by definition
            // so no calculation for nEntriesPho[][CORR::kBKG]
            corrHists[iHist][i][j].nEntriesPho[CORR::kRAW][CORR::kBKG] = corrHists[iHist][i][j].nEntriesPho[CORR::kRAW][CORR::kRAW];
            corrHists[iHist][i][j].nEntriesPho[CORR::kBKG][CORR::kBKG] = corrHists[iHist][i][j].nEntriesPho[CORR::kBKG][CORR::kRAW];

            if (h_nPho[i][j][CORR::kRAW]->GetBinContent(1) == 0)  {
                h_nPho[i][j][CORR::kRAW]->SetBinContent(1, corrHists[iHist][i][j].nEntriesPho[CORR::kRAW][CORR::kRAW]);
                h_nPho[i][j][CORR::kRAW]->Write("",TObject::kOverwrite);
            }

            if (h_nPho[i][j][CORR::kBKG]->GetBinContent(1) == 0)  {
                h_nPho[i][j][CORR::kBKG]->SetBinContent(1, corrHists[iHist][i][j].nEntriesPho[CORR::kBKG][CORR::kRAW]);
                h_nPho[i][j][CORR::kBKG]->Write("",TObject::kOverwrite);
            }


            tgj->SetEventList(elists[CORR::kRAW][i][j]);
            corrHists[iHist][i][j].nEntries[CORR::kRAW][CORR::kRAW] = tgj->GetEntries(corrHists[iHist][i][j].selections[CORR::kRAW][CORR::kRAW].GetTitle());
            tgj->SetEventList(eventlist);      // restore the original event list
            tgj->SetEventList(elists[CORR::kBKG][i][j]);
            corrHists[iHist][i][j].nEntries[CORR::kBKG][CORR::kRAW] = tgj->GetEntries(corrHists[iHist][i][j].selections[CORR::kBKG][CORR::kRAW].GetTitle());
            tgj->SetEventList(eventlist);      // restore the original event list

            std::cout<< "nEntries[CORR::kRAW][CORR::kRAW] = " << corrHists[iHist][i][j].nEntries[CORR::kRAW][CORR::kRAW] <<std::endl;
            std::cout<< "nEntries[CORR::kBKG][CORR::kRAW] = " << corrHists[iHist][i][j].nEntries[CORR::kBKG][CORR::kRAW] <<std::endl;
            std::cout<< "nEntriesPho[CORR::kRAW][CORR::kRAW] = " << corrHists[iHist][i][j].nEntriesPho[CORR::kRAW][CORR::kRAW] <<std::endl;
            std::cout<< "nEntriesPho[CORR::kBKG][CORR::kRAW] = " << corrHists[iHist][i][j].nEntriesPho[CORR::kBKG][CORR::kRAW] <<std::endl;

            std::string histoTitle;
            if (isHI) {
              histoTitle = Form("%s , %.0f < p^{#gamma}_{T} < %.0f GeV/c, %d-%d %% ",collisionName , bins_pt[0].at(i), bins_pt[1].at(i), bins_hiBin[0].at(j)/2, bins_hiBin[1].at(j)/2);

              // special cases
              if (bins_pt[0].at(i) <= 0 && bins_pt[1].at(i) < 0 && bins_hiBin[0].at(j) <= 0 && bins_hiBin[1].at(j) >= 200 )   {
                  histoTitle = Form("%s",collisionName );
              }
              else if (bins_pt[0].at(i) <= 0 && bins_pt[1].at(i) < 0)   {
                  histoTitle = Form("%s , %d-%d %%",collisionName , bins_hiBin[0].at(j)/2, bins_hiBin[1].at(j)/2);
              }
              else if (bins_pt[1].at(i) < 0 && bins_hiBin[0].at(j) <= 0 && bins_hiBin[1].at(j) >= 200)   {
                  histoTitle = Form("%s ,  p^{#gamma}_{T} > %.0f GeV/c",collisionName , bins_pt[0].at(i));
              }
              else if (bins_hiBin[0].at(j) <= 0 && bins_hiBin[1].at(j) >= 200)   {
                  histoTitle = Form("%s , %.0f < p^{#gamma}_{T} < %.0f GeV/c",collisionName , bins_pt[0].at(i), bins_pt[1].at(i));
              }
              else if (bins_pt[1].at(i) < 0) {
                  histoTitle = Form("%s , p^{#gamma}_{T} > %.0f GeV/c, %d-%d %% ",collisionName , bins_pt[0].at(i), bins_hiBin[0].at(j)/2, bins_hiBin[1].at(j)/2);
              }
            }
            else {
                histoTitle = Form("%s , %.0f < p^{#gamma}_{T} < %.0f GeV/c",collisionName , bins_pt[0].at(i), bins_pt[1].at(i));

                // special cases
                if (bins_pt[0].at(i) <= 0 && bins_pt[1].at(i) < 0)   {
                    histoTitle = Form("%s",collisionName );
                }
                else if (bins_pt[1].at(i) < 0) {
                    histoTitle = Form("%s , p^{#gamma}_{T} > %.0f GeV/c", collisionName , bins_pt[0].at(i));
                }
            }

            // histograms for RAW and BKG regions
            for (int iCorr = 0; iCorr < CORR::kN_CORRFNC -1; ++iCorr) {
                for (int jCorr = 0; jCorr < CORR::kN_CORRFNC -1; ++jCorr) {

                    if (jCorr == CORR::kBKG && !isHI)  continue;      // no jet background for non-HI

                    std::string titleX = corrHists[iHist][i][j].h1D_titleX[iCorr][jCorr].c_str();
                    corrHists[iHist][i][j].h1D[iCorr][jCorr]->SetTitle(Form("%s;%s;%s",histoTitle.c_str(), titleX.c_str(), "Entries"));
                    corrHists[iHist][i][j].h1D[iCorr][jCorr]->SetMarkerStyle(kFullCircle);
                    corrHists[iHist][i][j].h1D[iCorr][jCorr]->SetMarkerColor(kBlack);

                    // histogram name excluding the "h1D" prefix
                    std::string tmpH1D_name = corrHists[iHist][i][j].h1D_name[iCorr][jCorr].c_str();
                    TCut selectionDraw = corrHists[iHist][i][j].selections[iCorr][jCorr].GetTitle();
                    if (doEventWeight > 0)  selectionDraw = Form("(%s)*(%s)", eventWeight.c_str(), selectionDraw.GetTitle());

                    std::string tmpHistName = corrHists[iHist][i][j].h1D[iCorr][jCorr]->GetName();
                    std::string tmpFormula = correlationHistFormulas.at(iHist).c_str();

                    c->SetName(Form("cnv_%s",tmpH1D_name.c_str()));
                    c->cd();
                    if (jCorr == CORR::kRAW){
                        tgj->SetEventList(elists[iCorr][i][j]);
                        tgj->Draw(Form("%s >> %s", tmpFormula.c_str(), tmpHistName.c_str()), selectionDraw.GetTitle(),"goff");
                        tgj->SetEventList(eventlist);      // restore the original event list
                    }
                    if (jCorr == CORR::kBKG && hasJetsMB && hasGammaJetMB) {
                        tgjMB->SetEventList(elists[iCorr][i][j]);
                        tgjMB->Draw(Form("%s >> %s", tmpFormula.c_str(), tmpHistName.c_str()), selectionDraw.GetTitle(),"goff");
                        tgjMB->SetEventList(eventlist);      // restore the original event list
                    }

                    corrHists[iHist][i][j].h1D[iCorr][jCorr]->Draw("e");
                    corrHists[iHist][i][j].h1D[iCorr][jCorr]->Write("",TObject::kOverwrite);
                    corrHists[iHist][i][j].h1D[iCorr][jCorr]->SetStats(false);     // remove stat box from the canvas, but keep in the histograms.
                    c->Write("",TObject::kOverwrite);
                    c->Clear();

                    // FINAL
                    c->SetName(Form("cnv_%s_final",tmpH1D_name.c_str()));
                    c->cd();
                    corrHists[iHist][i][j].h1D_final[iCorr][jCorr] =
                            (TH1D*)corrHists[iHist][i][j].h1D[iCorr][jCorr]->Clone(Form("%s_final", tmpHistName.c_str()));
                    double tmpXlow = xlow_final.at(iHist);
                    double tmpXup  = xup_final.at(iHist);
                    corrHists[iHist][i][j].h1D_final[iCorr][jCorr]->SetAxisRange(tmpXlow, tmpXup);
                    corrHists[iHist][i][j].h1D_final[iCorr][jCorr]->Draw("e");
                    corrHists[iHist][i][j].h1D_final[iCorr][jCorr]->Write("",TObject::kOverwrite);
                    corrHists[iHist][i][j].h1D_final[iCorr][jCorr]->SetStats(false);  // remove stat box from the canvas, but keep in the histograms.
                    c->Write("",TObject::kOverwrite);
                    c->Clear();

                    // FINAL_NORM
                    c->SetName(Form("cnv_%s_final_norm",tmpH1D_name.c_str()));
                    c->cd();
                    corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr] =
                            (TH1D*)corrHists[iHist][i][j].h1D_final[iCorr][jCorr]->Clone(Form("%s_final_norm", tmpHistName.c_str()));
                    if (jCorr == CORR::kBKG && hasJetsMB && hasGammaJetMB) {   // (hasJetsMB && hasGammaJetMB) ==> isHI
                        // normalize first by the number of mixed events
                        corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->Scale(1./nEventsToMix);
                    }
                    int tmpNEntriesPho = corrHists[iHist][i][j].nEntriesPho[iCorr][jCorr];
                    // normalization is done with photon events, not necessarily by photon-jet events
                    // so the integral of the normalized histogram is R_jg.
                    corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->Scale(1./tmpNEntriesPho);
                    std::string tmpTitleY_final_norm = corrHists[iHist][i][j].h1D_titleY_final_norm[iCorr][jCorr].c_str();
                    corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->GetYaxis()->SetTitle(tmpTitleY_final_norm.c_str());
                    corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->Draw("e");
                    corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->Write("",TObject::kOverwrite);
                    c->Write("",TObject::kOverwrite);
                    c->Clear();
                }
            }

            std::cout<<"making histograms for SIG regions"<<std::endl;
            // calculate SIGSIG histogram,
            // these histograms are ignored : SIGRAW, SIGBKG

            // no background for PP and PA
            if(!isHI){
                // for non-HI, reset jet BKG histograms
                // in that case BKG histograms are not used, but set to empty histograms.
                // so it becomes SIG = RAW
                std::cout<<"collision is "<< collisionName <<std::endl;
                std::cout<<"contribution from BKG region is set to zero."<< std::endl;

                std::string tmpHistName;

                // reset RAWBKG
                tmpHistName = Form("%s_final_norm", corrHists[iHist][i][j].h1D[CORR::kRAW][CORR::kBKG]->GetName());
                corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kBKG] =
                        (TH1D*)corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kRAW]->Clone(tmpHistName.c_str());
                corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kBKG]->Reset();

                // reset BKGBKG
                tmpHistName = Form("%s_final_norm", corrHists[iHist][i][j].h1D[CORR::kBKG][CORR::kBKG]->GetName());
                corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kBKG] =
                        (TH1D*)corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kRAW]->Clone(tmpHistName.c_str());
                corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kBKG]->Reset();
            }

            // first subtract jet BKG, then subtract photon BKG
            // subtract jet BKG
            // RAWSIG = RAWRAW - RAWBKG
            std::string tmpHistNameRAWSIG = Form("%s_final_norm", corrHists[iHist][i][j].h1D[CORR::kRAW][CORR::kSIG]->GetName());
            std::cout<<tmpHistNameRAWSIG.c_str()<<std::endl;
            corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kSIG] =
                                 (TH1D*)corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kRAW]->Clone(tmpHistNameRAWSIG.c_str());
            // do arithmetic if histograms are not empty
            if (corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kBKG]->GetEntries() > 0) {
                corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kSIG]->Add(corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kBKG],-1);
            }

            // BKGSIG = BKGRAW - BKGBKG
            std::string tmpHistNameBKGSIG = Form("%s_final_norm", corrHists[iHist][i][j].h1D[CORR::kBKG][CORR::kSIG]->GetName());
            std::cout<<tmpHistNameBKGSIG.c_str()<<std::endl;
            corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kSIG] =
                                 (TH1D*)corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kRAW]->Clone(tmpHistNameBKGSIG.c_str());
            // do arithmetic if histograms are not empty
            if (corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kBKG]->GetEntries() > 0) {
                corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kSIG]->Add(corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kBKG],-1);
            }

            // subtract photon BKG
            // purity*SIGSIG + (1-purity)*BKGSIG = RAWSIG
            // SIGSIG = 1/purity *  ( RAWSIG - (1-purity) * BKGSIG )
            std::string tmpHistNameSIGSIG = Form("%s_final_norm", corrHists[iHist][i][j].h1D[CORR::kSIG][CORR::kSIG]->GetName());
            std::cout<<tmpHistNameSIGSIG.c_str()<<std::endl;
            corrHists[iHist][i][j].h1D_final_norm[CORR::kSIG][CORR::kSIG] =
                                 (TH1D*)corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kSIG]->Clone(tmpHistNameSIGSIG.c_str());
            // do arithmetic if histograms are not empty
            if (corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kSIG]->GetEntries() > 0) {
                corrHists[iHist][i][j].h1D_final_norm[CORR::kSIG][CORR::kSIG]->Add(corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kSIG],-1*(1-purity[i][j]));
                corrHists[iHist][i][j].h1D_final_norm[CORR::kSIG][CORR::kSIG]->Scale(1./purity[i][j]);
            }
            if (isHI)  std::cout<< "purity[i][j] = " << purity[i][j] << std::endl;

            // FINAL_NORM  RAWSIG
            std::string tmpH1D_nameRAWSIG = corrHists[iHist][i][j].h1D_name[CORR::kRAW][CORR::kSIG].c_str();
            std::cout<<"drawing tmpH1D_nameRAWSIG = "<<tmpH1D_nameRAWSIG.c_str()<<std::endl;
            c->SetName(Form("cnv_%s_final_norm",tmpH1D_nameRAWSIG.c_str()));
            c->cd();
            corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kSIG]->Draw("e");
            corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kSIG]->Write("",TObject::kOverwrite);
            corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kSIG]->SetStats(false);  // remove stat box from the canvas, but keep in the histograms.
            c->Write("",TObject::kOverwrite);
            c->Clear();

            // FINAL_NORM  BKGSIG
            std::string tmpH1D_nameBKGSIG = corrHists[iHist][i][j].h1D_name[CORR::kBKG][CORR::kSIG].c_str();
            std::cout<<"drawing tmpH1D_nameBKGSIG = "<<tmpH1D_nameBKGSIG.c_str()<<std::endl;
            c->SetName(Form("cnv_%s_final_norm",tmpH1D_nameBKGSIG.c_str()));
            c->cd();
            corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kSIG]->Draw("e");
            corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kSIG]->Write("",TObject::kOverwrite);
            corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kSIG]->SetStats(false);  // remove stat box from the canvas, but keep in the histograms.
            c->Write("",TObject::kOverwrite);
            c->Clear();

            // FINAL_NORM  SIGSIG
            std::string tmpH1D_nameSIGSIG = corrHists[iHist][i][j].h1D_name[CORR::kSIG][CORR::kSIG].c_str();
            std::cout<<"drawing tmpH1D_nameSIGSIG = "<<tmpH1D_nameSIGSIG.c_str()<<std::endl;
            c->SetName(Form("cnv_%s_final_norm",tmpH1D_nameSIGSIG.c_str()));
            c->cd();
            corrHists[iHist][i][j].h1D_final_norm[CORR::kSIG][CORR::kSIG]->Draw("e");
            corrHists[iHist][i][j].h1D_final_norm[CORR::kSIG][CORR::kSIG]->Write("",TObject::kOverwrite);
            corrHists[iHist][i][j].h1D_final_norm[CORR::kSIG][CORR::kSIG]->SetStats(false);  // remove stat box from the canvas, but keep in the histograms.
            c->Write("",TObject::kOverwrite);
            c->Clear();

            tgj->SetEventList(eventlist);      // restore the original event list after making the histograms
            if (hasJetsMB && hasGammaJetMB) {
                tgjMB->SetEventList(eventlist);
            }

            std::cout<<Form("histogramming END : ptBin%d HiBin%d", i, j)<<std::endl;
            std::cout<<"##########"<<std::endl;
        }
    }
    std::cout<<"histogramming END : "<<correlationHistNames.at(iHist).c_str()<<std::endl;
    std::cout<<"####################"<<std::endl;
    }

    // histograms with pt bins on x-axis
    // corrHists_ptBinAll[0][]  = R_jg
    // corrHists_ptBinAll[1][]  = <X_jg>
    std::cout<<"####################"<<std::endl;
    for(int j=0; j<nBins_hiBin; ++j){
        for(int iCorr=0; iCorr<CORR::kN_CORRFNC; ++iCorr) {
            for(int jCorr=0; jCorr<CORR::kN_CORRFNC; ++jCorr) {

                // ignore SIGRAW, SIGBKG histograms
                if ((iCorr == CORR::kSIG && (jCorr == CORR::kRAW || jCorr == CORR::kBKG))) continue;

                if(j>0 && !isHI) continue;

                int offset = 2; // ptBin 40-50 starts from index 2.
                // rjg block
                for(int i=0; i<nBins_rjg; ++i){

                    double err;
                    double val = corrHists[0][i+offset][j].h1D_final_norm[iCorr][jCorr]->IntegralAndError(1,
                                                           corrHists[0][i+offset][j].h1D_final_norm[iCorr][jCorr]->GetNbinsX(), err);

                    corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->SetBinContent(i+1, val);
                    corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->SetBinError(i+1, err);
                }

                std::string histoTitle = Form("%s , %d-%d %%",collisionName , bins_hiBin[0].at(j)/2, bins_hiBin[1].at(j)/2);

                corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->SetTitle(Form("%s;p^{#gamma}_{T} (GeV/c); R_{J#gamma}",histoTitle.c_str()));
                corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->SetMarkerStyle(kFullCircle);
                corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->SetMarkerColor(kBlack);

                std::cout<<"drawing : "<<corrHists_ptBinAll[0][j].h1D_name[iCorr][jCorr].c_str()<<std::endl;
                c->SetName(Form("cnv_%s",corrHists_ptBinAll[0][j].h1D_name[iCorr][jCorr].c_str()));
                c->cd();
                corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->Draw("e");
                corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->Write("",TObject::kOverwrite);
                corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->SetStats(false);     // remove stat box from the canvas, but keep in the histograms.
                c->Write("",TObject::kOverwrite);
                c->Clear();

                // xjg_mean block
                for(int i=0; i<nBins_xjg_mean; ++i){

                    double val = corrHists[0][i+offset][j].h1D_final_norm[iCorr][jCorr]->GetMean();
                    double err = corrHists[0][i+offset][j].h1D_final_norm[iCorr][jCorr]->GetMeanError();

                    corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->SetBinContent(i+1, val);
                    corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->SetBinError(i+1, err);
                }

                corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->SetTitle(Form("%s;p^{#gamma}_{T} (GeV/c); <x_{J#gamma}>",histoTitle.c_str()));
                corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->SetMarkerStyle(kFullCircle);
                corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->SetMarkerColor(kBlack);

                std::cout<<"drawing : "<<corrHists_ptBinAll[1][j].h1D_name[iCorr][jCorr].c_str()<<std::endl;
                c->SetName(Form("cnv_%s",corrHists_ptBinAll[1][j].h1D_name[iCorr][jCorr].c_str()));
                c->cd();
                corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->Draw("e");
                corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->Write("",TObject::kOverwrite);
                corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->SetStats(false);     // remove stat box from the canvas, but keep in the histograms.
                c->Write("",TObject::kOverwrite);
                c->Clear();
            }
        }
    }
    std::cout<<"####################"<<std::endl;

    eventlist->Delete();
    for(int i=0; i<nBins_pt; ++i){
        for(int j=0; j<nBins_hiBin; ++j){
            elists[CORR::kRAW][i][j]->Delete();
            elists[CORR::kBKG][i][j]->Delete();
        }
    }

    configTree->Write("",TObject::kOverwrite);

    output->Write("",TObject::kOverwrite);
    input->Close();
    output->Close();
}

int main(int argc, char** argv)
{
    if (argc == 4) {
        gammaJetHistogram(argv[1], argv[2], argv[3]);
        return 0;
    }
    else if (argc == 3) {
        gammaJetHistogram(argv[1], argv[2]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./gammaJetHistogram.exe <configFile> <inputFile> <outputFile>"
                << std::endl;
        return 1;
    }
}
