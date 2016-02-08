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

void zJetHistogram(const TString configFile, const TString inputFile, const TString outputFile = "zJetHistogram.root");

void zJetHistogram(const TString configFile, const TString inputFile, const TString outputFile)
{
    std::cout<<"running zJetHistogram()"<<std::endl;
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
    // Z Boson cuts
    int doDiElectron;
    int doDiMuon;
    float massMin;
    float massMax;
    // electron cuts
    std::string triggerEle;
    float elePt;
    float eleSigmaIEtaIEta_2012_EB;
    float eledEtaAtVtx_abs_EB;
    float eledPhiAtVtx_abs_EB;
    float eleHoverE_EB;
    float eleEoverPInv_EB;
    float eleD0_abs_EB;
    float eleDz_abs_EB;
    int   eleMissHits_EB;
    float eleSigmaIEtaIEta_2012_EE;
    float eledEtaAtVtx_abs_EE;
    float eledPhiAtVtx_abs_EE;
    float eleHoverE_EE;
    float eleEoverPInv_EE;
    float eleD0_abs_EE;
    float eleDz_abs_EE;
    int   eleMissHits_EE;
    // muon cuts
    std::string triggerMu;
    float muPt;
    float muChi2NDF;
    float muInnerD0;
    float muInnerDz;
    int muMuonHits;
    int muStations;
    int muTrkLayers;
    int muPixelHits;

    // jet cuts
    float cut_jetpt;
    float cut_jeteta;
    int   cut_jetID;
    // zJet cuts
    float cut_awayRange;
    float cut_awayRange_lt;
    float cut_dR;
    // process cuts
    int nEventsToMix;
    if (configCuts.isValid) {
        bins_pt[0] = ConfigurationParser::ParseListFloat(
                configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kZBOSON].s[CUTS::ZBO::k_bins_pt_gt]);
        bins_pt[1] = ConfigurationParser::ParseListFloat(
                configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kZBOSON].s[CUTS::ZBO::k_bins_pt_lt]);
        bins_hiBin[0] = ConfigurationParser::ParseListInteger(
                configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_gt]);
        bins_hiBin[1] = ConfigurationParser::ParseListInteger(
                configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_lt]);

        doEventWeight = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].i[CUTS::EVT::k_doEventWeight];
        eventWeight = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_eventWeight].c_str();

        doDiElectron = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kZBOSON].i[CUTS::ZBO::k_doDiElectron];
        doDiMuon = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kZBOSON].i[CUTS::ZBO::k_doDiMuon];
        massMin = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kZBOSON].f[CUTS::ZBO::k_massMin];
        massMax = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kZBOSON].f[CUTS::ZBO::k_massMax];

        triggerEle = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].s[CUTS::ELE::k_trigger].c_str();
        elePt = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_elePt];

        // Barrel
        eleSigmaIEtaIEta_2012_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleSigmaIEtaIEta_2012_EB];
        eledEtaAtVtx_abs_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eledEtaAtVtx_abs_EB];
        eledPhiAtVtx_abs_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eledPhiAtVtx_abs_EB];
        eleHoverE_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleHoverE_EB];
        eleEoverPInv_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleEoverPInv_EB];
        eleD0_abs_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleD0_abs_EB];
        eleDz_abs_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleDz_abs_EB];
        eleMissHits_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].i[CUTS::ELE::k_eleMissHits_EB];

        // Endcap
        eleSigmaIEtaIEta_2012_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleSigmaIEtaIEta_2012_EE];
        eledEtaAtVtx_abs_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eledEtaAtVtx_abs_EE];
        eledPhiAtVtx_abs_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eledPhiAtVtx_abs_EE];
        eleHoverE_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleHoverE_EE];
        eleEoverPInv_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleEoverPInv_EE];
        eleD0_abs_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleD0_abs_EE];
        eleDz_abs_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleDz_abs_EE];
        eleMissHits_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].i[CUTS::ELE::k_eleMissHits_EE];

        triggerMu = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kMUON].s[CUTS::MUO::k_trigger].c_str();
        muPt = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kMUON].f[CUTS::MUO::k_muPt];

        muChi2NDF = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kMUON].f[CUTS::MUO::k_muChi2NDF];
        muInnerD0 = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kMUON].f[CUTS::MUO::k_muInnerD0];
        muInnerDz = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kMUON].f[CUTS::MUO::k_muInnerDz];
        muMuonHits = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kMUON].i[CUTS::MUO::k_muMuonHits];
        muStations = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kMUON].i[CUTS::MUO::k_muStations];
        muTrkLayers = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kMUON].i[CUTS::MUO::k_muTrkLayers];
        muPixelHits = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kMUON].i[CUTS::MUO::k_muPixelHits];

        cut_jetpt  = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kJET].f[CUTS::JET::k_pt];
        cut_jeteta = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kJET].f[CUTS::JET::k_eta];
        cut_jetID  = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kJET].i[CUTS::JET::k_jetID];

        cut_awayRange = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kZJET].f[CUTS::ZJT::k_awayRange];
        cut_awayRange_lt = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kZJET].f[CUTS::ZJT::k_awayRange_lt];
        cut_dR = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kZJET].f[CUTS::ZJT::k_dR];

        nEventsToMix = configCuts.proc[CUTS::kSKIM].obj[CUTS::kZJET].i[CUTS::ZJT::k_nEventsToMix];
    }
    else {  // default configuration
        bins_pt[0].push_back(60);
        bins_pt[1].push_back(999999);
        bins_hiBin[0].push_back(0);
        bins_hiBin[0].push_back(0);
        bins_hiBin[1].push_back(200);
        bins_hiBin[1].push_back(60);

        doDiElectron = 1;
        doDiMuon = 0;
        massMin = 60;
        massMax = 120;

        triggerEle = "HLT_HIDoublePhoton15_Eta2p5_Mass50_1000_R9SigmaHECut_v1";
        elePt = 10;

        // Barrel
        eleSigmaIEtaIEta_2012_EB = 0.012;
        eledEtaAtVtx_abs_EB = 0.0126;
        eledPhiAtVtx_abs_EB = 0.107;
        eleHoverE_EB = 0.186;
        eleEoverPInv_EB = 0.239;
        eleD0_abs_EB = 0.0621;
        eleDz_abs_EB = 0.613;
        eleMissHits_EB = 2;

        // Endcap
        eleSigmaIEtaIEta_2012_EE = 0.0339;
        eledEtaAtVtx_abs_EE = 0.0109;
        eledPhiAtVtx_abs_EE = 0.219;
        eleHoverE_EE = 0.0962;
        eleEoverPInv_EE = 0.141;
        eleD0_abs_EE = 0.279;
        eleDz_abs_EE = 0.947;
        eleMissHits_EE = 3;

        muPt = 0;
        muChi2NDF = 10;
        muInnerD0 = 0.2;
        muInnerDz = 0.5;

        muMuonHits = 0;
        muStations = 1;
        muTrkLayers = 5;
        muPixelHits = 0;

        cut_jetpt = 40;
        cut_jeteta = 1.6;
        cut_jetID = 0;      // jetID >= 0

        cut_awayRange = 2./3.;
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

    std::cout<<"massMin = "<<massMin<<std::endl;
    std::cout<<"massMax = "<<massMax<<std::endl;

    std::cout<<"doDiElectron = "<<doDiElectron<<std::endl;
    if (doDiElectron > 0) {
        std::cout<<"triggerEle = "<<triggerEle.c_str()<<std::endl;
        std::cout<<"elePt      = "<<elePt<<std::endl;

        std::cout<<"Barrel :"<<std::endl;
        std::cout<<"eleSigmaIEtaIEta_2012_EB = "<<eleSigmaIEtaIEta_2012_EB<<std::endl;
        std::cout<<"eledEtaAtVtx_abs_EB      = "<<eledEtaAtVtx_abs_EB<<std::endl;
        std::cout<<"eledPhiAtVtx_abs_EB      = "<<eledPhiAtVtx_abs_EB<<std::endl;
        std::cout<<"eleHoverE_EB             = "<<eleHoverE_EB<<std::endl;
        std::cout<<"eleEoverPInv_EB          = "<<eleEoverPInv_EB<<std::endl;
        std::cout<<"eleD0_abs_EB             = "<<eleD0_abs_EB<<std::endl;
        std::cout<<"eleDz_abs_EB             = "<<eleDz_abs_EB<<std::endl;
        std::cout<<"eleMissHits_EB           = "<<eleMissHits_EB<<std::endl;

        std::cout<<"Endcap :"<<std::endl;
        std::cout<<"eleSigmaIEtaIEta_2012_EE = "<<eleSigmaIEtaIEta_2012_EE<<std::endl;
        std::cout<<"eledEtaAtVtx_abs_EE      = "<<eledEtaAtVtx_abs_EE<<std::endl;
        std::cout<<"eledPhiAtVtx_abs_EE      = "<<eledPhiAtVtx_abs_EE<<std::endl;
        std::cout<<"eleHoverE_EE             = "<<eleHoverE_EE<<std::endl;
        std::cout<<"eleEoverPInv_EE          = "<<eleEoverPInv_EE<<std::endl;
        std::cout<<"eleD0_abs_EE             = "<<eleD0_abs_EE<<std::endl;
        std::cout<<"eleDz_abs_EE             = "<<eleDz_abs_EE<<std::endl;
        std::cout<<"eleMissHits_EE           = "<<eleMissHits_EE<<std::endl;
    }

    std::cout<<"doDiMuon = "<<doDiMuon<<std::endl;
    if (doDiMuon > 0) {
        std::cout<<"triggerMu = "<<triggerMu.c_str()<<std::endl;
        std::cout<<"muPt      = "<<muPt<<std::endl;
        std::cout<<"muChi2NDF = "<<muChi2NDF<<std::endl;
        std::cout<<"muInnerD0 = "<<muInnerD0<<std::endl;
        std::cout<<"muInnerDz = "<<muInnerDz<<std::endl;
        std::cout<<"muMuonHits = "<<muMuonHits<<std::endl;
        std::cout<<"muStations = "<<muStations<<std::endl;
        std::cout<<"muTrkLayers = "<<muTrkLayers<<std::endl;
        std::cout<<"muPixelHits = "<<muPixelHits<<std::endl;
    }

    if (doDiElectron > 0 && doDiMuon > 0) {
         std::cout<<"WARNING : Both of doDiElectron and doDiMuon options are set."<<std::endl;
         std::cout<<"switch to default option : doDiElectron = 1, doDiMuon = 0"<<std::endl;
         doDiElectron = 1;
         doDiMuon = 0;
     }

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

    std::string trigger = "";
    std::string leptonTag = "";         // acronym used for the lepton in the tree branches
    std::string diLeptonM = "";         // name of the branch containing diLepton mass
    std::string diLeptonPt = "";
    std::string diLeptonTreePath = "";
    std::string leptonSymbol = "";
    if (doDiElectron > 0) {
        trigger = triggerEle.c_str();
        leptonTag = "Ele";
        diLeptonM = "diEleM";
        diLeptonPt = "diElePt";
        diLeptonTreePath = "dielectron";
        leptonSymbol = "e";
    }
    else if (doDiMuon > 0) {
        trigger = triggerMu.c_str();
        leptonTag = "Mu";
        diLeptonM = "diMuM";
        diLeptonPt = "diMuPt";
        diLeptonTreePath = "dimuon";
        leptonSymbol = "#mu";
    }

    std::string dileptonMFormula = Form("%s[zIdx]", diLeptonM.c_str());
    std::string dileptonPtFormula = Form("%s[zIdx]", diLeptonPt.c_str());
    std::string dileptonMtitleX = Form("M^{%s%s} (GeV/c^{2})", leptonSymbol.c_str(), leptonSymbol.c_str());

    std::vector<std::string> correlationHistNames   {"xjz", "dphi", "ptJet", diLeptonM.c_str(), diLeptonPt.c_str()};
    std::vector<std::string> correlationHistFormulas{"xjz", "abs(dphi)", "jtpt", dileptonMFormula.c_str(), dileptonPtFormula.c_str()};
    std::vector<std::string> correlationHistTitleX  {"p^{Jet}_{T}/p^{Z}_{T}", "#Delta#phi_{JZ}", "p^{Jet}_{T}", dileptonMtitleX.c_str(), "p^{Z}_{T}"};
    std::vector<std::string> correlationHistTitleY_final_normalized{"#frac{1}{N_{Z}} #frac{dN_{JZ}}{dx_{JZ}}",
                                                                          "#frac{1}{N_{JZ}} #frac{dN_{JZ}}{d#Delta#phi}",
                                                                          "#frac{1}{N_{Z}} #frac{dN_{JZ}}{dp^{Jet}_{T}}",
                                                                          "Entries / (2 GeV/c^{2})",
                                                                          "#frac{1}{N_{JZ}} #frac{dN_{JZ}}{dp^{Z}_{T}}"};
    std::vector<int>         nBinsx{40, 20,          100, 30, 100};
    std::vector<double>      xlow  {0,  0,           0,   60, 0};
    std::vector<double>      xup   {5,  TMath::Pi(), 300, 120, 300};
    std::vector<double>      xlow_final{0,  0,           0,   60, 0};
    std::vector<double>      xup_final {2,  TMath::Pi(), 150, 120, 150};

    TFile *input = new TFile(inputFile, "READ");
    TTree *tHlt = (TTree*)input->Get("hltTree");
    TTree *tdiLepton = (TTree*)input->Get(diLeptonTreePath.c_str());
    TTree *tJet = (TTree*)input->Get("jets");
    TTree *tzj  = (TTree*)input->Get("zJet");
    TTree *tHiEvt = (TTree*)input->Get("HiEvt");       // HiEvt tree will be placed in PP forest as well.
    TTree *tJetMB;
    TTree *tzjMB;

    // check the existence of HI specific trees in "zJetSkim.root" file
    bool hasJetsMB = false;
    bool hasZJetMB = false;
    if (isHI) {
        input->GetObject("jetsMB",tJetMB);
        input->GetObject("zJetMB",tzjMB);

        if (tJetMB) hasJetsMB = true;
        if (tzjMB)  hasZJetMB = true;
    }
    else {
        tJetMB = 0;
        tzjMB  = 0;
    }

    tzj->AddFriend(tHlt, "Hlt");
    tzj->AddFriend(tdiLepton, "diLepton");
    tzj->AddFriend(tJet, "Jet");
    tzj->AddFriend(tHiEvt, "HiEvt");

    // relation of trees from MB mixing block
    if (hasJetsMB && hasZJetMB) {
        tzjMB->AddFriend(tHlt, "Hlt");
        tzjMB->AddFriend(tdiLepton, "diLepton");
        tzjMB->AddFriend(tJetMB, "Jet");
        tzjMB->AddFriend(tHiEvt, "HiEvt");
    }

    TFile* output = new TFile(outputFile, "UPDATE");
    // histograms will be put under a directory whose name is the type of the collision
    if(!output->GetKey(collisionName)) output->mkdir(collisionName, Form("input file is %s", inputFile.Data()));
    output->cd(collisionName);
    std::cout<<"histograms will be put under directory : " << collisionName << std::endl;

    TTree *configTree = setupConfigurationTreeForWriting(configCuts);

    // gammaJet analysis has purity calculation. no purity calculation for zJet.

    TH1::SetDefaultSumw2();
    int nCorrHist = correlationHistNames.size();
    correlationHist corrHists[nCorrHist][nBins_pt][nBins_hiBin];

    // prepare histogram names for xjg, abs(dphi) and jet pt
    // if the collision is not HI, then cannot split it into hiBins.
    if (!isHI) nBins_hiBin = 1;
    for (int iHist=0; iHist<nCorrHist; ++iHist){
    for (int i=0; i<nBins_pt; ++i){
        for(int j=0; j<nBins_hiBin; ++j){
            corrHists[iHist][i][j].name = Form("%s_ptBin%d_HiBin%d", correlationHistNames.at(iHist).c_str(), i, j);

            int iCorr = 0;
            for (int jCorr = 0; jCorr < CORR::kN_CORRFNC; ++jCorr) {

                std::string subHistName = Form("%s_ptBin%d_HiBin%d_%s", correlationHistNames.at(iHist).c_str(), i, j,
                        CORR::CORR_JET_LABELS[jCorr].c_str());
                corrHists[iHist][i][j].h1D_name[iCorr][jCorr] = subHistName.c_str();
                corrHists[iHist][i][j].h1D[iCorr][jCorr] = new TH1D(Form("h1D_%s", subHistName.c_str()),"",
                        nBinsx.at(iHist), xlow.at(iHist), xup.at(iHist));

                corrHists[iHist][i][j].h1D_titleX[iCorr][jCorr] = correlationHistTitleX.at(iHist).c_str();
                corrHists[iHist][i][j].h1D_titleY_final_norm[iCorr][jCorr] = correlationHistTitleY_final_normalized.at(iHist).c_str();
            }
        }
    }
    }

    // histograms with pt bins on x-axis
    std::vector<std::string> correlationHistNames_ptBinAll = {"rjz", "xjz_mean"};       // histograms where x-axis is pt bins
    int nCorrHist_ptBinAll = correlationHistNames_ptBinAll.size();
    correlationHist corrHists_ptBinAll[nCorrHist_ptBinAll][nBins_hiBin];
    // prepare histogram names for rjz and <xjz>
    const int nBins_rjz = 4;
    const int nBins_xjz_mean= 4;
    double bins_rjz[nBins_rjz+1] =           {40, 50, 60, 80, 120};
    double bins_xjz_mean[nBins_xjz_mean+1] = {40, 50, 60, 80, 120};

    for (int j=0; j<nBins_hiBin; ++j){
        int iCorr = 0;
        for (int jCorr = 0; jCorr < CORR::kN_CORRFNC; ++jCorr) {

            // rjz
            std::string subHistName;
            subHistName = Form("%s_ptBinAll_HiBin%d_%s", correlationHistNames_ptBinAll.at(0).c_str(), j,
                    CORR::CORR_JET_LABELS[jCorr].c_str());
            corrHists_ptBinAll[0][j].h1D_name[iCorr][jCorr] = subHistName.c_str();
            corrHists_ptBinAll[0][j].h1D[iCorr][jCorr] = new TH1D(Form("h1D_%s", subHistName.c_str()), "",nBins_rjz, bins_rjz);

            // x_jz_mean
            subHistName = Form("%s_ptBinAll_HiBin%d_%s", correlationHistNames_ptBinAll.at(1).c_str(), j,
                    CORR::CORR_JET_LABELS[jCorr].c_str());
            corrHists_ptBinAll[1][j].h1D_name[iCorr][jCorr] = subHistName.c_str();
            corrHists_ptBinAll[1][j].h1D[iCorr][jCorr] = new TH1D(Form("h1D_%s", subHistName.c_str()), "",nBins_xjz_mean, bins_xjz_mean);
        }
    }

    // selections for different signal regions that are applied to every histogram
    // selections for dielectron regions
    TCut selectionDiele =  "";
    selectionDiele = selectionDiele && Form("elePt_1[zIdx] > %f && elePt_2[zIdx] > %f", elePt, elePt);

    TCut selection_ele[2];
    TCut selection_ele_EB[2];
    TCut selection_ele_EE[2];
    for (int iEle=0; iEle<2; ++iEle) {
        selection_ele_EB[iEle] = "";
        selection_ele_EE[iEle] = "";

        selection_ele_EB[iEle] = selection_ele_EB[iEle] && Form("eleSigmaIEtaIEta_2012_%d[zIdx] < %f" ,iEle+1 ,eleSigmaIEtaIEta_2012_EB);
        selection_ele_EB[iEle] = selection_ele_EB[iEle] && Form("abs(eledEtaAtVtx_%d[zIdx]) < %f" ,iEle+1 ,eledEtaAtVtx_abs_EB);
        selection_ele_EB[iEle] = selection_ele_EB[iEle] && Form("abs(eledPhiAtVtx_%d[zIdx]) < %f" ,iEle+1 ,eledPhiAtVtx_abs_EB);
        selection_ele_EB[iEle] = selection_ele_EB[iEle] && Form("eleHoverE_%d[zIdx] < %f" ,iEle+1 ,eleHoverE_EB);
        selection_ele_EB[iEle] = selection_ele_EB[iEle] && Form("eleEoverPInv_%d[zIdx] < %f" ,iEle+1 ,eleEoverPInv_EB);
        selection_ele_EB[iEle] = selection_ele_EB[iEle] && Form("abs(eleD0_%d[zIdx]) < %f" ,iEle+1 ,eleD0_abs_EB);
        selection_ele_EB[iEle] = selection_ele_EB[iEle] && Form("abs(eleDz_%d[zIdx]) < %f" ,iEle+1 ,eleDz_abs_EB);
        selection_ele_EB[iEle] = selection_ele_EB[iEle] && Form("eleMissHits_%d[zIdx] <= %d" ,iEle+1 ,eleMissHits_EB);

        selection_ele_EE[iEle] = selection_ele_EE[iEle] && Form("eleSigmaIEtaIEta_2012_%d[zIdx] < %f" ,iEle+1 ,eleSigmaIEtaIEta_2012_EE);
        selection_ele_EE[iEle] = selection_ele_EE[iEle] && Form("abs(eledEtaAtVtx_%d[zIdx]) < %f" ,iEle+1 ,eledEtaAtVtx_abs_EE);
        selection_ele_EE[iEle] = selection_ele_EE[iEle] && Form("abs(eledPhiAtVtx_%d[zIdx]) < %f" ,iEle+1 ,eledPhiAtVtx_abs_EE);
        selection_ele_EE[iEle] = selection_ele_EE[iEle] && Form("eleHoverE_%d[zIdx] < %f" ,iEle+1 ,eleHoverE_EE);
        selection_ele_EE[iEle] = selection_ele_EE[iEle] && Form("eleEoverPInv_%d[zIdx] < %f" ,iEle+1 ,eleEoverPInv_EE);
        selection_ele_EE[iEle] = selection_ele_EE[iEle] && Form("abs(eleD0_%d[zIdx]) < %f" ,iEle+1 ,eleD0_abs_EE);
        selection_ele_EE[iEle] = selection_ele_EE[iEle] && Form("abs(eleDz_%d[zIdx]) < %f" ,iEle+1 ,eleDz_abs_EE);
        selection_ele_EE[iEle] = selection_ele_EE[iEle] && Form("eleMissHits_%d[zIdx] <= %d" ,iEle+1 ,eleMissHits_EE);

        TCut selection_EB_eta = Form("abs(eleEta_%d[zIdx]) < %f" ,iEle+1 ,1.4791);
        TCut selection_EE_eta = Form("abs(eleEta_%d[zIdx]) >= %f && abs(eleEta_%d[zIdx]) < %f" ,iEle+1 ,1.4791 ,iEle+1 ,2.4);

        selection_ele_EB[iEle] = selection_ele_EB[iEle] && selection_EB_eta;
        selection_ele_EE[iEle] = selection_ele_EE[iEle] && selection_EE_eta;
        selection_ele[iEle] = selection_ele_EB[iEle] || selection_ele_EE[iEle];
    }
    selectionDiele = selectionDiele && selection_ele[0] && selection_ele[1];

    // selections for dimuon
    TCut selectionDimu =  "";
    selectionDimu = selectionDimu && Form("muPt_1[zIdx] > %f && muPt_2[zIdx] > %f", muPt, muPt);
    TCut selection_mu[2];
    for (int iMu=0; iMu<2; ++iMu) {
        selection_mu[iMu] = "";

        selection_mu[iMu] = selection_mu[iMu] && Form("muChi2NDF_%d[zIdx] < %f", iMu+1, muChi2NDF);
        selection_mu[iMu] = selection_mu[iMu] && Form("abs(muInnerD0_%d[zIdx]) < %f", iMu+1, muInnerD0);
        selection_mu[iMu] = selection_mu[iMu] && Form("abs(muInnerDz_%d[zIdx]) < %f", iMu+1, muInnerDz);
        selection_mu[iMu] = selection_mu[iMu] && Form("muMuonHits_%d[zIdx] > %d", iMu+1, muMuonHits);
        selection_mu[iMu] = selection_mu[iMu] && Form("muStations_%d[zIdx] > %d", iMu+1, muStations);
        selection_mu[iMu] = selection_mu[iMu] && Form("muTrkLayers_%d[zIdx] > %d", iMu+1, muTrkLayers);
        selection_mu[iMu] = selection_mu[iMu] && Form("muPixelHits_%d[zIdx] > %d", iMu+1, muPixelHits);

        TCut selection_mu_eta = Form("abs(muEta_%d[zIdx]) < %f", iMu+1 ,2.4);
        selection_mu[iMu] = selection_mu[iMu] && selection_mu_eta;
    }
    selectionDimu = selectionDimu && selection_mu[0] && selection_mu[1];

    TCut selectionLepton = "";
    if (doDiElectron > 0) selectionLepton = selectionDiele;
    else if (doDiMuon > 0) selectionLepton = selectionDimu;

    // selection for jet regions
    // jet from bkg region are already separated from raw region.
    // no additional selection for jets. just use different trees.
    std::cout<<"####################"<<std::endl;
    std::cout<<"tzj->GetEntries() = "<<tzj->GetEntries()<<std::endl;
    if (trigger.compare("") != 0 && !isMC) {
        std::cout<<"tzj->GetEntries(trigger==1) = "<<tzj->GetEntries(Form("%s == 1",trigger.c_str()))<<std::endl;
    }
    else {
        std::cout<<"tzj->GetEntries(trigger==1) is skipped because either no trigger is specified or the data is coming from MC."<<std::endl;
    }
    std::cout<<"####################"<<std::endl;

    // use "EventList" approach to make the "draw()" shorter
    // store original eventlist
    const char* elist_tmp_name="elist_tmp";
    tzj->Draw(Form(">> %s", elist_tmp_name));
    TEventList* eventlist = ((TEventList*)gDirectory->Get(elist_tmp_name)->Clone("elist_Original"));
    gDirectory->Delete(elist_tmp_name);

    int nCorrZ = CORR::kRAW + 1;
    std::string eventlistNames[nCorrZ][nBins_pt][nBins_hiBin];
    TEventList* elists[nCorrZ][nBins_pt][nBins_hiBin];
    bool isEventlistCreated[nBins_pt][nBins_hiBin];     // event lists are independent of the correlation histogram.
    // set eventlist once for a given bin, then reuse it for subsequent correlations, do not recalculate.
    for(int i=0; i<nBins_pt; ++i){
        for(int j=0; j<nBins_hiBin; ++j){
            eventlistNames[CORR::kRAW][i][j] = Form("eventlist_ptBin%d_HiBin%d", i, j);  // CORR::kRAW = 0
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

            // Z cuts were applied in the analysis code
            // Z selection
            TCut selectionZ;
            if (bins_pt[1].at(i) >= 0)  selectionZ = Form("%s[zIdx] >= %f && %s[zIdx] < %f", diLeptonPt.c_str(), bins_pt[0].at(i),
                                                                                             diLeptonPt.c_str(), bins_pt[1].at(i));
            else                        selectionZ = Form("%s[zIdx] >= %f", diLeptonPt.c_str(), bins_pt[0].at(i));
            selectionZ = selectionZ && Form("%s[zIdx] >= %f && %s[zIdx] <= %f", diLeptonM.c_str(), massMin,
                                                                                diLeptonM.c_str(), massMax);
            selectionZ = selectionZ && selectionLepton;

            // jet selection
            TCut selectionJet = "";
            // special selection
            if (correlationHistNames.at(iHist).compare("dphi") != 0) {  // no awayRange cut for dphi histograms
                selectionJet = selectionJet && Form("abs(dphi) > %f ", cut_awayRange);
                selectionJet = selectionJet && Form("abs(dphi) <= %f ", cut_awayRange_lt);
            }
            selectionJet = selectionJet && Form("dR_ele_1 >= %f", cut_dR);
            selectionJet = selectionJet && Form("dR_ele_2 >= %f", cut_dR);
            selectionJet = selectionJet && Form("jtpt > %f", cut_jetpt);
            selectionJet = selectionJet && Form("abs(jteta) < %f", cut_jeteta);
            selectionJet = selectionJet && Form("jetID >= %d", cut_jetID);
            // special selection
            if (correlationHistNames.at(iHist).compare(diLeptonM.c_str()) == 0) {  // select zJet events only, do not select inclusive jets
                selectionJet = Form("Max$(%s)>0", selectionJet.GetTitle());
            }
            if (correlationHistNames.at(iHist).compare(diLeptonPt.c_str()) == 0) {  // select zJet events only, do not select inclusive jets
                selectionJet = Form("Max$(%s)>0", selectionJet.GetTitle());
            }

            TCut selection_Barrel = "1";    // no extra selection at the moment
            TCut selection_Endcap = "1";    // no extra selection at the moment

            TCut selection = "";
            selection = selection && selection_event;
            selection = selection && selectionZ;
            selection = selection && selectionJet;

            for (int jCorr = 0; jCorr < CORR::kN_CORRFNC; ++jCorr) {
                    if (jCorr > CORR::kRAW && !isHI)  continue;      // no jet background for non-HI
                    corrHists[iHist][i][j].selections[CORR::kRAW][jCorr] = selection;
            }

            std::cout<< "corrHists[iHist][i][j].h1D[CORR::kRAW][CORR::kRAW]->GetName() = " <<  corrHists[iHist][i][j].h1D[CORR::kRAW][CORR::kRAW]->GetName() <<std::endl;
            std::cout<< "selections[CORR::kRAW][CORR::kRAW] = " << corrHists[iHist][i][j].selections[CORR::kRAW][CORR::kRAW].GetTitle() <<std::endl;

            // use "EventList" approach to make the "draw()" shorter
            if (!isEventlistCreated[i][j]) {
                tzj->Draw(Form(">> %s", eventlistNames[CORR::kRAW][i][j].c_str()), (selection_event && selectionZ).GetTitle());
                elists[CORR::kRAW][i][j] = (TEventList*)gDirectory->Get(eventlistNames[CORR::kRAW][i][j].c_str());
                isEventlistCreated[i][j] = true;
                // event lists are independent of the correlation histogram.
                // set eventlist once for a given bin, then reuse it for subsequent correlations, do not recalculate.
            }

            // number of events with Z bosons, not necessarily z-jet events
            tzj->SetEventList(elists[CORR::kRAW][i][j]);
            corrHists[iHist][i][j].nEntriesPho[CORR::kRAW][CORR::kRAW] = tzj->GetEventList()->GetN();
            tzj->SetEventList(eventlist);      // restore the original event list
            // nEntriesPho[][CORR::kRAW] = nEntriesPho[][CORR::kBKG] by definition
            // so no calculation for nEntriesPho[][CORR::kBKG]
            corrHists[iHist][i][j].nEntriesPho[CORR::kRAW][CORR::kBKG] = corrHists[iHist][i][j].nEntriesPho[CORR::kRAW][CORR::kRAW];

            tzj->SetEventList(elists[CORR::kRAW][i][j]);
            corrHists[iHist][i][j].nEntries[CORR::kRAW][CORR::kRAW] = tzj->GetEntries(corrHists[iHist][i][j].selections[CORR::kRAW][CORR::kRAW].GetTitle());
            tzj->SetEventList(eventlist);      // restore the original event list

            std::cout<< "nEntries[CORR::kRAW][CORR::kRAW] = " << corrHists[iHist][i][j].nEntries[CORR::kRAW][CORR::kRAW] <<std::endl;
            std::cout<< "nEntriesZ[CORR::kRAW][CORR::kRAW] = " << corrHists[iHist][i][j].nEntriesPho[CORR::kRAW][CORR::kRAW] <<std::endl;

            std::string histoTitle;
            if (isHI) {
              histoTitle = Form("%s , %.0f < p^{Z}_{T} < %.0f GeV/c, %d-%d %% ",collisionName , bins_pt[0].at(i), bins_pt[1].at(i), bins_hiBin[0].at(j)/2, bins_hiBin[1].at(j)/2);

              // special cases
              if (bins_pt[0].at(i) <= 0 && bins_pt[1].at(i) < 0 && bins_hiBin[0].at(j) <= 0 && bins_hiBin[1].at(j) >= 200 )   {
                  histoTitle = Form("%s",collisionName );
              }
              else if (bins_pt[0].at(i) <= 0 && bins_pt[1].at(i) < 0)   {
                  histoTitle = Form("%s , %d-%d %%",collisionName , bins_hiBin[0].at(j)/2, bins_hiBin[1].at(j)/2);
              }
              else if (bins_pt[1].at(i) < 0 && bins_hiBin[0].at(j) <= 0 && bins_hiBin[1].at(j) >= 200)   {
                  histoTitle = Form("%s ,  p^{Z}_{T} > %.0f GeV/c",collisionName , bins_pt[0].at(i));
              }
              else if (bins_hiBin[0].at(j) <= 0 && bins_hiBin[1].at(j) >= 200)   {
                  histoTitle = Form("%s , %.0f < p^{Z}_{T} < %.0f GeV/c",collisionName , bins_pt[0].at(i), bins_pt[1].at(i));
              }
              else if (bins_pt[1].at(i) < 0) {
                  histoTitle = Form("%s , p^{Z}_{T} > %.0f GeV/c, %d-%d %% ",collisionName , bins_pt[0].at(i), bins_hiBin[0].at(j)/2, bins_hiBin[1].at(j)/2);
              }
            }
            else {
                histoTitle = Form("%s , %.0f < p^{Z}_{T} < %.0f GeV/c",collisionName , bins_pt[0].at(i), bins_pt[1].at(i));

                // special cases
                if (bins_pt[0].at(i) <= 0 && bins_pt[1].at(i) < 0)   {
                    histoTitle = Form("%s",collisionName );
                }
                else if (bins_pt[1].at(i) < 0) {
                    histoTitle = Form("%s , p^{Z}_{T} > %.0f GeV/c", collisionName , bins_pt[0].at(i));
                }
            }

            // histograms for RAW and BKG regions
            int iCorr = 0;
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
                    tzj->SetEventList(elists[iCorr][i][j]);
                    tzj->Draw(Form("%s >> %s", tmpFormula.c_str(), tmpHistName.c_str()), selectionDraw.GetTitle(),"goff");
                    tzj->SetEventList(eventlist);      // restore the original event list
                }
                if (jCorr == CORR::kBKG && hasJetsMB && hasZJetMB) {
                    tzjMB->SetEventList(elists[iCorr][i][j]);
                    tzjMB->Draw(Form("%s >> %s", tmpFormula.c_str(), tmpHistName.c_str()), selectionDraw.GetTitle(),"goff");
                    tzjMB->SetEventList(eventlist);      // restore the original event list
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
                if (jCorr == CORR::kBKG && hasJetsMB && hasZJetMB) {   // (hasJetsMB && hasZJetMB) ==> isHI
                    // normalize first by the number of mixed events
                    corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->Scale(1./nEventsToMix);
                }
                int tmpNEntriesZ = corrHists[iHist][i][j].nEntriesPho[iCorr][jCorr];
                // normalization is done with Z boson events, not necessarily by Z-jet events
                // so the integral of the normalized histogram is R_jg.
                corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->Scale(1./tmpNEntriesZ);
                std::string tmpTitleY_final_norm = corrHists[iHist][i][j].h1D_titleY_final_norm[iCorr][jCorr].c_str();
                corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->GetYaxis()->SetTitle(tmpTitleY_final_norm.c_str());
                corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->Draw("e");
                corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->Write("",TObject::kOverwrite);
                c->Write("",TObject::kOverwrite);
                c->Clear();
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

                // there is no BKGBKG
            }

            // first subtract jet BKG, there is no Z BKG
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

            // there is no BKGRAW, BKGBKG, BKGSIG

            // there is no Z BKG

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

            // there is no FINAL_NORM  BKGSIG
            std::cout<<"there is no tmpH1D_nameBKGSIG "<<std::endl;

            // FINAL_NORM  SIGSIG
            std::cout<<"there is no tmpH1D_nameSIGSIG "<<std::endl;

            tzj->SetEventList(eventlist);      // restore the original event list after making the histograms
            if (hasJetsMB && hasZJetMB) {
                tzjMB->SetEventList(eventlist);
            }

            std::cout<<Form("histogramming END : ptBin%d HiBin%d", i, j)<<std::endl;
            std::cout<<"##########"<<std::endl;
        }
    }
    std::cout<<"histogramming END : "<<correlationHistNames.at(iHist).c_str()<<std::endl;
    std::cout<<"####################"<<std::endl;
    }

    // histograms with pt bins on x-axis
    // corrHists_ptBinAll[0][]  = R_jz
    // corrHists_ptBinAll[1][]  = <X_jz>
    std::cout<<"####################"<<std::endl;
    for(int j=0; j<nBins_hiBin; ++j){
        int iCorr=0;
        for(int jCorr=0; jCorr<CORR::kN_CORRFNC; ++jCorr) {

            if(j>0 && !isHI) continue;

            int offset = 3; // ptBin 40-50 starts from index 3.
            // rjg block
            for(int i=0; i<nBins_rjz; ++i){

                double err;
                double val = corrHists[0][i+offset][j].h1D_final_norm[iCorr][jCorr]->IntegralAndError(1,
                        corrHists[0][i+offset][j].h1D_final_norm[iCorr][jCorr]->GetNbinsX(), err);

                corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->SetBinContent(i+1, val);
                corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->SetBinError(i+1, err);
            }

            std::string histoTitle = Form("%s , %d-%d %%",collisionName , bins_hiBin[0].at(j)/2, bins_hiBin[1].at(j)/2);

            corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->SetTitle(Form("%s;p^{Z}_{T} (GeV/c); R_{JZ}",histoTitle.c_str()));
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

            // xjz_mean block
            for(int i=0; i<nBins_xjz_mean; ++i){

                double val = corrHists[0][i+offset][j].h1D_final_norm[iCorr][jCorr]->GetMean();
                double err = corrHists[0][i+offset][j].h1D_final_norm[iCorr][jCorr]->GetMeanError();

                corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->SetBinContent(i+1, val);
                corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->SetBinError(i+1, err);
            }

            corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->SetTitle(Form("%s;p^{Z}_{T} (GeV/c); <x_{JZ}>",histoTitle.c_str()));
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
    std::cout<<"####################"<<std::endl;

    eventlist->Delete();
    for(int i=0; i<nBins_pt; ++i){
        for(int j=0; j<nBins_hiBin; ++j){
            elists[CORR::kRAW][i][j]->Delete();
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
        zJetHistogram(argv[1], argv[2], argv[3]);
        return 0;
    }
    else if (argc == 3) {
        zJetHistogram(argv[1], argv[2]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./zJetHistogram.exe <configFile> <inputFile> <outputFile>"
                << std::endl;
        return 1;
    }
}
