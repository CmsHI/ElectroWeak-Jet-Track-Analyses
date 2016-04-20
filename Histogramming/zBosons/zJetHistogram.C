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

#include "../../TreeHeaders/CutConfigurationTree.h"
#include "../../TreeHeaders/zJetTree.h"
#include "../../Plotting/commonUtility.h"
#include "../interface/correlationHist.h"
#include "../../Performance/interface/ratioHist.h"
#include "../../Utilities/eventUtil.h"
#include "../../Utilities/interface/InputConfigurationParser.h"
#include "../../Utilities/interface/CutConfigurationParser.h"
#include "../../Corrections/jets/jetCorrector.h"

void zJetHistogram(const TString configFile, const TString inputFile, const TString outputFile = "zJetHistogram.root");
std::string makeHistTitle(bool isHI, std::string collisionName, float ptBinLow, float ptBinUp, int hiBinLow, int hiBinUp);

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
    std::vector<std::string> triggersEle;   // triggers will be "OR"ed
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
    std::vector<std::string> triggersMu;    // triggers will be "OR"ed
    float muPt;
    float muChi2NDF;
    float muInnerD0;
    float muInnerDz;
    int muMuonHits;
    int muStations;
    int muTrkLayers;
    int muPixelHits;

    // jet cuts
    std::string jetCollection;
    int doCorrectionMatchingEff;
    int doCorrectionJetID;
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

        triggersEle = ConfigurationParser::ParseList(configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].s[CUTS::ELE::k_trigger].c_str());
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

        triggersMu = ConfigurationParser::ParseList(configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kMUON].s[CUTS::MUO::k_trigger]);
        muPt = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kMUON].f[CUTS::MUO::k_muPt];

        muChi2NDF = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kMUON].f[CUTS::MUO::k_muChi2NDF];
        muInnerD0 = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kMUON].f[CUTS::MUO::k_muInnerD0];
        muInnerDz = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kMUON].f[CUTS::MUO::k_muInnerDz];
        muMuonHits = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kMUON].i[CUTS::MUO::k_muMuonHits];
        muStations = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kMUON].i[CUTS::MUO::k_muStations];
        muTrkLayers = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kMUON].i[CUTS::MUO::k_muTrkLayers];
        muPixelHits = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kMUON].i[CUTS::MUO::k_muPixelHits];

        jetCollection = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kJET].s[CUTS::JET::k_jetCollection];
        doCorrectionMatchingEff = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kJET].i[CUTS::JET::k_doCorrectionMatchingEfficiency];
        doCorrectionJetID = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kJET].i[CUTS::JET::k_doCorrectionJetID];
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

        doEventWeight = 0;

        doDiElectron = 1;
        doDiMuon = 0;
        massMin = 60;
        massMax = 120;

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

        jetCollection = "akPu3PFJetAnalyzer";
        doCorrectionMatchingEff = 0;
        doCorrectionJetID = 0;
        cut_jetpt = 40;
        cut_jeteta = 1.6;
        cut_jetID = 0;      // jetID >= 0

        cut_awayRange = 2./3.;
        cut_awayRange_lt = 1;
        cut_dR = 0.4;

        nEventsToMix = 1;
        doEventWeight = 0;
    }
    // default values
    if (eventWeight.size() == 0) eventWeight = "1";
    if (cut_awayRange_lt == 0) cut_awayRange_lt = 1;

    int nBins_pt = bins_pt[0].size();         // assume <myvector>[0] and <myvector>[1] have the same size.
    int nBins_hiBin = bins_hiBin[0].size();     // assume <myvector>[0] and <myvector>[1] have the same size.
    int nTriggersEle = triggersEle.size();
    int nTriggersMu  = triggersMu.size();

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
        std::cout<<"nTriggersEle = "<< nTriggersEle <<std::endl;
        for (int i=0; i<nTriggersEle; ++i) {
            std::cout << Form("triggersEle[%d] = %s", i, triggersEle.at(i).c_str()) << std::endl;
        }

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
        std::cout<<"nTriggersMu = "<< nTriggersMu <<std::endl;
        for (int i=0; i<nTriggersMu; ++i) {
            std::cout << Form("triggersMu[%d] = %s", i, triggersMu.at(i).c_str()) << std::endl;
        }

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

    std::cout<<"jetCollection             = "<< jetCollection.c_str() <<std::endl;
    std::cout<<"doCorrectionMatchingEff   = "<< doCorrectionMatchingEff <<std::endl;
    std::cout<<"doCorrectionJetID         = "<< doCorrectionJetID <<std::endl;
    std::cout<<"cut_jetpt                 = "<< cut_jetpt <<std::endl;
    std::cout<<"cut_jeteta                = "<< cut_jeteta <<std::endl;
    std::cout<<"cut_jetID                 = "<< cut_jetID <<std::endl;

    std::cout<<"cut_awayRange             = "<< cut_awayRange << " * PI" <<std::endl;
    std::cout<<"cut_awayRange_lt          = "<< cut_awayRange_lt << " * PI" <<std::endl;
    std::cout<<"cut_dR                    = "<< cut_dR <<std::endl;

    std::cout<<"nEventsToMix              = "<< nEventsToMix <<std::endl;

    // WARNINGS
    if (doCorrectionJetID > 0 && cut_jetID == 0) {
        std::cout<<"WARNING : jet ID corrections is set. But application of jetID is turned off." <<std::endl;
        std::cout<<"          jet ID correction will not be applied." <<std::endl;
    }

    //set the actual awayRange cut
    cut_awayRange = cut_awayRange * TMath::Pi();
    cut_awayRange_lt = cut_awayRange_lt * TMath::Pi();

    std::vector<std::string> triggers;
    std::string diLeptonM = "";         // name of the branch containing diLepton mass
    std::string diLeptonPt = "";
    std::string diLeptonTreePath = "";
    std::string leptonSymbol = "";
    if (doDiElectron > 0) {
        triggers = triggersEle;
        diLeptonM = "diEleM";
        diLeptonPt = "diElePt";
        diLeptonTreePath = "dielectron";
        leptonSymbol = "e";
    }
    else if (doDiMuon > 0) {
        triggers = triggersMu;
        diLeptonM = "diMuM";
        diLeptonPt = "diMuPt";
        diLeptonTreePath = "dimuon";
        leptonSymbol = "#mu";
    }

    std::string dileptonMFormula = Form("%s[zIdx]", diLeptonM.c_str());
    std::string dileptonPtFormula = Form("%s[zIdx]", diLeptonPt.c_str());
    std::string dileptonMtitleX = Form("M^{%s%s} (GeV/c^{2})", leptonSymbol.c_str(), leptonSymbol.c_str());

    // READ TREES
    TFile *input = new TFile(inputFile, "READ");
    TTree *tHlt = (TTree*)input->Get("hltTree");
    TTree *tdiLepton = (TTree*)input->Get(diLeptonTreePath.c_str());
    TTree *tJet = (TTree*)input->Get(jetCollection.c_str());
    TTree *tzj  = (TTree*)input->Get(Form("z_%s", jetCollection.c_str()));
    TTree *tHiEvt = (TTree*)input->Get("HiEvt");       // HiEvt tree will be placed in PP forest as well.

    if (!tJet) {
        std::cout<<"following jet collection is not found in the input file : " << jetCollection.c_str() << std::endl;
        std::cout<<"exiting"<< std::endl;
        return;
    }

    TTree *tJetMB = 0;
    TTree *tzjMB = 0;
    // check the existence of HI specific trees in "zJetSkim.root" file
    bool hasJetsMB = false;
    bool hasZJetMB = false;
    if (isHI) {
        input->GetObject(Form("%sMB", jetCollection.c_str()), tJetMB);
        input->GetObject(Form("z_%sMB", jetCollection.c_str()), tzjMB);

        if (tJetMB) hasJetsMB = true;
        if (tzjMB)  hasZJetMB = true;
    }
    else {
        tJetMB = 0;
        tzjMB  = 0;
    }
    // READ TREES - END

    // SET BRANCHES ADRESSESS
    // hltTree
    tHlt->SetBranchStatus("*", 0);
    int nTriggers = triggers.size();
    Int_t triggerBits[nTriggers];
    for (int i = 0; i < nTriggers; ++i) {
        tHlt->SetBranchStatus(triggers.at(i).c_str(), 1);
        tHlt->SetBranchAddress(triggers.at(i).c_str(), &triggerBits[i]);
    }

    // HiEvt
    tHiEvt->SetBranchStatus("*", 0);

    float vz;
    Int_t hiBin;
    tHiEvt->SetBranchStatus("vz", 1);
    tHiEvt->SetBranchStatus("hiBin", 1);
    tHiEvt->SetBranchAddress("vz", &vz);
    tHiEvt->SetBranchAddress("hiBin", &hiBin);
    float weight_HiEvt;
    if (doEventWeight > 0) {
        tHiEvt->SetBranchStatus("weight", 1);
        tHiEvt->SetBranchAddress("weight", &weight_HiEvt);
    }
    else {
        weight_HiEvt = 1;
    }

    // dilepton tree
    dielectron diEle;
    if (doDiElectron > 0)  diEle.setupDiElectronTree(tdiLepton);     // diElectronTree is output
    dimuon diMu;
    if (doDiMuon > 0) diMu.setupDiMuonTree(tdiLepton);     // diMuonTree is output

    // jet trees
    std::vector<Jets> jets(2);   // i = 0 : RAW,  i = 1 BKG
    jets[CORR::kRAW].setupTreeForReading(tJet);
    // zJet trees
    std::vector<ZJet> zJets(2);
    zJets[CORR::kRAW].setupZJetTree(tzj);

    if (hasJetsMB)  jets[CORR::kBKG].setupTreeForReading(tJetMB);
    if (hasZJetMB)  zJets[CORR::kBKG].setupZJetTree(tzjMB);
    // SET BRANCHES ADRESSESS - END

    // correctors
    jetCorrector correctorJet;

    TFile* output = new TFile(outputFile, "UPDATE");
    // histograms will be put under a directory whose name is the type of the collision
    if(!output->GetKey(collisionName)) output->mkdir(collisionName, Form("input file is %s", inputFile.Data()));
    output->cd(collisionName);
    std::cout<<"histograms will be put under directory : " << collisionName << std::endl;

    TTree *configTree = setupConfigurationTreeForWriting(configCuts);

    // gammaJet analysis has purity calculation. no purity calculation for zJet.

    // zJet correlation objects
    std::vector<std::string> correlationHistNames   {"xjz", "dphi", "ptJet", "zM", "zPt", "zEta", "zPhi", "jteta", "jtphi", "nJet"};
    //std::vector<std::string> correlationHistFormulas{"xjz", "abs(dphi)", "jtpt", dileptonMFormula.c_str(), dileptonPtFormula.c_str()};
    std::vector<std::string> correlationHistTitleX  {"p^{Jet}_{T}/p^{Z}_{T}", "#Delta#phi_{JZ}", "p^{Jet}_{T}", dileptonMtitleX.c_str(),
                                                        "p^{Z}_{T}", "#eta^{Z}", "#phi^{Z}", "#eta^{Jet}", "#phi^{Jet}", "N^{Jet}"};
    std::vector<std::string> correlationHistTitleY_final_normalized{"#frac{1}{N_{Z}} #frac{dN_{JZ}}{dx_{JZ}}",
                                                                          "#frac{1}{N_{Z}} #frac{dN_{JZ}}{d#Delta#phi}",
                                                                          "#frac{1}{N_{Z}} #frac{dN_{JZ}}{dp^{Jet}_{T}}",
                                                                          "Entries / (2 GeV/c^{2})",
                                                                          "#frac{1}{N_{Z}} #frac{dN_{JZ}}{dp^{Z}_{T}}",
                                                                          "#frac{1}{N_{Z}} #frac{dN_{JZ}}{d#eta^{Z}}",
                                                                          "#frac{1}{N_{Z}} #frac{dN_{JZ}}{d#phi^{Z}}",
                                                                          "#frac{1}{N_{Z}} #frac{dN_{JZ}}{d#eta^{Jet}}",
                                                                          "#frac{1}{N_{Z}} #frac{dN_{JZ}}{d#phi^{Jet}}",
                                                                          "#frac{1}{N_{Z}} #frac{dN_{JZ}}{dN^{Jet}}",};
    std::vector<int>         nBinsx{16, 20,          30, 30, 30, 20, 20, 20, 20, 8};
    std::vector<double>      xlow  {0,  0,           0,   60, 0, -4, -TMath::Pi(), -4, -TMath::Pi(), 0};
    std::vector<double>      xup   {2,  TMath::Pi(), 300, 120, 300, 4, TMath::Pi(), 4, TMath::Pi(),  8};
    std::vector<double>      xlow_final{0,  0,           0,   60, 0, -3, -TMath::Pi(), -3, -TMath::Pi(), 0};
    std::vector<double>      xup_final {2,  TMath::Pi(), 200, 120, 200, 3, TMath::Pi(), 3, TMath::Pi(),  8};
    std::vector<bool> doAwaySideJets {true,  false, true, false, false, false, false, true, true, true};  // whether the observable is plotted for inclusive jets in the away side
    std::vector<bool> doSingleJet    {false, false, false, true, true, true, true, false, false, false};   // whether the observable is plotted once per event

    TH1::SetDefaultSumw2();
    int nCorrHist = correlationHistNames.size();
    correlationHist corrHists[nCorrHist][nBins_pt][nBins_hiBin];

    // prepare histogram names for xjg, abs(dphi) and jet pt
    // if the collision is not HI, then cannot split it into hiBins.
    if (!isHI) nBins_hiBin = 1;
    for (int iHist=0; iHist<nCorrHist; ++iHist){
        for (int i=0; i<nBins_pt; ++i){
            for(int j=0; j<nBins_hiBin; ++j){
                corrHists[iHist][i][j].name = Form("%s_ptBin%d_hiBin%d", correlationHistNames.at(iHist).c_str(), i, j);

                int iCorr = 0;
                for (int jCorr = 0; jCorr < CORR::kN_CORRFNC; ++jCorr) {

                    std::string subHistName = Form("%s_ptBin%d_hiBin%d_%s", correlationHistNames.at(iHist).c_str(), i, j,
                            CORR::CORR_JET_LABELS[jCorr].c_str());
                    corrHists[iHist][i][j].h1D_name[iCorr][jCorr] = subHistName.c_str();
                    corrHists[iHist][i][j].h1D_titleX[iCorr][jCorr] = correlationHistTitleX.at(iHist).c_str();
                    corrHists[iHist][i][j].h1D_titleY_final_norm[iCorr][jCorr] = correlationHistTitleY_final_normalized.at(iHist).c_str();

                    corrHists[iHist][i][j].h1D[iCorr][jCorr] = new TH1D(Form("h1D_%s", subHistName.c_str()),"",
                            nBinsx.at(iHist), xlow.at(iHist), xup.at(iHist));
                    int nBinx_final = int((xup_final.at(iHist)-xlow_final.at(iHist))/(xup.at(iHist)-xlow.at(iHist))*nBinsx.at(iHist));
                    corrHists[iHist][i][j].h1D_final[iCorr][jCorr] = new TH1D(Form("h1D_%s_final", subHistName.c_str()),"",
                            nBinx_final, xlow_final.at(iHist), xup_final.at(iHist));

                    // HISTO TITLE
                    std::string histoTitle = makeHistTitle(isHI, collisionName, bins_pt[0].at(i), bins_pt[1].at(i), bins_hiBin[0].at(j), bins_hiBin[1].at(j));

                    std::string titleX = corrHists[iHist][i][j].h1D_titleX[iCorr][jCorr].c_str();
                    std::string titleY_final_norm = corrHists[iHist][i][j].h1D_titleY_final_norm[iCorr][jCorr].c_str();
                    corrHists[iHist][i][j].h1D[iCorr][jCorr]->SetTitle(Form("%s;%s;%s",histoTitle.c_str(), titleX.c_str(), "Entries"));
                    corrHists[iHist][i][j].h1D_final[iCorr][jCorr]->SetTitle(Form("%s;%s;%s",histoTitle.c_str(), titleX.c_str(), "Entries"));
                }
            }
        }
    }

    // histograms to store numbers
    // 1st bin stores the number of zJet events
    // 2nd bin stores the number of Z events, not zJet event
    // 3rd bin stores the number of mixed MB events
    // they are just a tool to store numbers.
    TH1D* h_nums[nBins_pt][nBins_hiBin];
    double entriesZJet[nBins_pt][nBins_hiBin];
    double entriesZ[nBins_pt][nBins_hiBin];
    Long64_t entriesZJetRaw[nBins_pt][nBins_hiBin];
    Long64_t entriesZRaw[nBins_pt][nBins_hiBin];
    for (int i=0; i<nBins_pt; ++i){
        for(int j=0; j<nBins_hiBin; ++j){

            std::string histName = Form("h_nums_ptBin%d_hiBin%d", i, j);
            h_nums[i][j] = new TH1D(histName.c_str(), "", 3, 0, 3);

            entriesZJet[i][j] = 0;
            entriesZ[i][j] = 0;
            entriesZJetRaw[i][j] = 0;
            entriesZRaw[i][j] = 0;
        }
    }

    // histograms with pt bins on x-axis
    std::vector<std::string> correlationHistNames_ptBinAll = {"rjz", "xjz_mean", "rjz_zNum"};       // histograms where x-axis is pt bins
    int nCorrHist_ptBinAll = correlationHistNames_ptBinAll.size();
    correlationHist corrHists_ptBinAll[nCorrHist_ptBinAll][nBins_hiBin];
    // prepare histogram names for rjz and <xjz>
    const int nBins_rjz = 4;
    const int nBins_xjz_mean = 4;
    double bins_rjz[nBins_rjz+1] =           {40, 50, 60, 80, 120};
    double bins_xjz_mean[nBins_xjz_mean+1] = {40, 50, 60, 80, 120};
    correlationHist rjz_num[nBins_hiBin];       // histograms where x-axis is pt bins, y-axis is number of zJet events
    correlationHist rjz_denom[nBins_hiBin];     // histograms where x-axis is pt bins, y-axis is number of Z events
    // R_jz will be ( h_rjz_num / h_rjz_denom )
    for (int j=0; j<nBins_hiBin; ++j){
        int iCorr = 0;
        // rjz
        std::string subHistName;
        // R_JZ
        subHistName = Form("%s_ptBinAll_hiBin%d", correlationHistNames_ptBinAll.at(0).c_str(), j);
        corrHists_ptBinAll[0][j].h1D_name[iCorr][0] = subHistName.c_str();
        corrHists_ptBinAll[0][j].h1D[iCorr][0] = new TH1D(Form("h1D_%s", subHistName.c_str()), "",nBins_rjz, bins_rjz);

        std::string histoTitle = Form("%s , %d-%d %%",collisionName , bins_hiBin[0].at(j)/2, bins_hiBin[1].at(j)/2);
        corrHists_ptBinAll[0][j].h1D[iCorr][0]->SetTitle(Form("%s;p^{Z}_{T} (GeV/c); R_{JZ}",histoTitle.c_str()));

        // R_JZ using number of events
        subHistName = Form("%s_ptBinAll_hiBin%d", correlationHistNames_ptBinAll.at(2).c_str(), j);
        corrHists_ptBinAll[2][j].h1D_name[iCorr][0] = subHistName.c_str();
        corrHists_ptBinAll[2][j].h1D[iCorr][0] = new TH1D(Form("h1D_%s", subHistName.c_str()), "",nBins_rjz, bins_rjz);

        histoTitle = Form("%s , %d-%d %%",collisionName , bins_hiBin[0].at(j)/2, bins_hiBin[1].at(j)/2);
        corrHists_ptBinAll[2][j].h1D[iCorr][0]->SetTitle(Form("%s;p^{Z}_{T} (GeV/c); R_{JZ}",histoTitle.c_str()));

        subHistName = Form("%s_num_ptBinAll_hiBin%d", correlationHistNames_ptBinAll.at(0).c_str(), j);
        rjz_num[j].h1D_name[iCorr][0] = subHistName.c_str();
        rjz_num[j].h1D[iCorr][0] = (TH1D*)corrHists_ptBinAll[0][j].h1D[iCorr][0]->Clone(Form("h1D_%s", subHistName.c_str()));

        subHistName = Form("%s_denom_ptBinAll_hiBin%d", correlationHistNames_ptBinAll.at(0).c_str(), j);
        rjz_denom[j].h1D_name[iCorr][0] = subHistName.c_str();
        rjz_denom[j].h1D[iCorr][0] = (TH1D*)corrHists_ptBinAll[0][j].h1D[iCorr][0]->Clone(Form("h1D_%s", subHistName.c_str()));

        for (int jCorr = 0; jCorr < CORR::kN_CORRFNC; ++jCorr) {
            // x_jz_mean
            subHistName = Form("%s_ptBinAll_hiBin%d_%s", correlationHistNames_ptBinAll.at(1).c_str(), j,
                    CORR::CORR_JET_LABELS[jCorr].c_str());
            corrHists_ptBinAll[1][j].h1D_name[iCorr][jCorr] = subHistName.c_str();
            corrHists_ptBinAll[1][j].h1D[iCorr][jCorr] = new TH1D(Form("h1D_%s", subHistName.c_str()), "",nBins_xjz_mean, bins_xjz_mean);
            corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->SetTitle(Form("%s;p^{Z}_{T} (GeV/c); <x_{JZ}>",histoTitle.c_str()));
        }
    }

    // histograms for performance studies : correction
    ratioHist ratioHist_jetID[nBins_pt][nBins_hiBin];
    TFile* fileMC_num;
    TFile* fileMC_denom;
    if (doCorrectionJetID > 0) {
        std::cout<<"##### histograms for jet ID efficiency correction #####"<<std::endl;

        std::string pathFileMC_num   = "zJetHistogram_Pythia8_Z30mumuJet_Hydjet_MB_0104_vJetIDCorr.root";
        std::string pathFileMC_denom = "zJetHistogram_Pythia8_Z30mumuJet_Hydjet_MB_0104_SYS_noJetID_vJetIDCorr.root";

        std::cout << "pathFileMC_num   = " << pathFileMC_num.c_str() << std::endl;
        std::cout << "pathFileMC_denom = " << pathFileMC_denom.c_str() << std::endl;

        fileMC_num   = new TFile(pathFileMC_num.c_str(), "READ");
        fileMC_denom = new TFile(pathFileMC_denom.c_str(), "READ");

        output->cd(collisionName);

        for (int i=0; i<nBins_pt; ++i){
            for(int j=0; j<nBins_hiBin; ++j){

                int indexPtJet = 2;
                ratioHist_jetID[i][j].name = Form("%s_ptBin%d_hiBin%d_%s_final_norm", correlationHistNames.at(indexPtJet).c_str(), i, j,
                        CORR::CORR_JET_LABELS[CORR::kSIG].c_str());

                ratioHist_jetID[i][j].hNum = (TH1D*)fileMC_num->Get(Form("HIMC/h1D_%s", ratioHist_jetID[i][j].name.c_str()))->Clone();
                ratioHist_jetID[i][j].hNum->SetName(Form("h_num_%s", ratioHist_jetID[i][j].name.c_str()));

                ratioHist_jetID[i][j].hDenom = (TH1D*)fileMC_denom->Get(Form("HIMC/h1D_%s", ratioHist_jetID[i][j].name.c_str()))->Clone();
                ratioHist_jetID[i][j].hDenom->SetName(Form("h_denom_%s", ratioHist_jetID[i][j].name.c_str()));

                ratioHist_jetID[i][j].hNumInitialized = true;
                ratioHist_jetID[i][j].hDenomInitialized = true;

                ratioHist_jetID[i][j].hRatio = (TH1D*)ratioHist_jetID[i][j].hNum->Clone(Form("h_Ratio_%s", ratioHist_jetID[i][j].name.c_str()));
                ratioHist_jetID[i][j].hRatio->Divide(ratioHist_jetID[i][j].hDenom);

                ratioHist_jetID[i][j].hRatioInitialized = true;
            }
        }

        // write histograms for performance studies
        std::cout<<"##### write histograms for jet ID efficiency correction #####"<<std::endl;
        for (int i=0; i<nBins_pt; ++i){
            for(int j=0; j<nBins_hiBin; ++j){

                if (!ratioHist_jetID[i][j].hRatioInitialized)  continue;

                ratioHist_jetID[i][j].hRatio->SetMarkerStyle(kFullCircle);
                ratioHist_jetID[i][j].hRatio->SetMarkerColor(kBlack);
                ratioHist_jetID[i][j].hRatio->Write("",TObject::kOverwrite);
                ratioHist_jetID[i][j].hRatio->SetStats(false);
            }
        }

        if (fileMC_num->IsOpen()) fileMC_num->Close();
        if (fileMC_denom->IsOpen()) fileMC_denom->Close();
    }

    Long64_t entries = tzj->GetEntries();
    Long64_t entriesPassedEventSelection = 0;
    Long64_t entriesPassedLeptonSelection = 0;
    Long64_t entriesPassedZmassSelection = 0;
    Long64_t entriesAnalyzed = 0;
    std::cout<<"####################"<<std::endl;
    std::cout<<"tzj->GetEntries() = "<<entries<<std::endl;
    if (nTriggers != 0 && !isMC) {
        std::string triggerStr = triggers.at(0).c_str();
        for (int i = 1; i < nTriggers; ++i) {
            triggerStr = Form ("%s || %s", triggerStr.c_str(), triggers.at(i).c_str());
        }
        std::cout<<"tHlt->GetEntries(triggerStr==1) = "<<tHlt->GetEntries(Form("( %s ) == 1", triggerStr.c_str()))<<std::endl;
    }
    else {
        std::cout<<"tHlt->GetEntries(triggerStr==1) is skipped because either no trigger is specified or the data is coming from MC."<<std::endl;
    }
    std::cout<<"####################"<<std::endl;

    std::cout<< "Loop : tzj" <<std::endl;
    for (Long64_t j_entry=0; j_entry<entries; ++j_entry)
    {
        tHlt->GetEntry(j_entry);
        tdiLepton->GetEntry(j_entry);
        tJet->GetEntry(j_entry);
        tzj->GetEntry(j_entry);
        tHiEvt->GetEntry(j_entry);
        if (hasJetsMB)  tJetMB->GetEntry(j_entry);
        if (hasZJetMB)  tzjMB->GetEntry(j_entry);

        // event selection
        bool passedTrigger = false;      // triggers will be "OR"ed
        if (nTriggers == 0) passedTrigger = true;
        for (int i = 0; i < nTriggers; ++i)
        {
            if (triggerBits[i] > 0)  passedTrigger = true;
        }
        if (!passedTrigger)  continue;

        entriesPassedEventSelection++;

        // Z selection
        int zIdx = zJets[CORR::kRAW].zIdx;     // index of the leading z

        // dielectron block
        if (doDiElectron > 0)
        {
            // pt selection
            if (!(diEle.elePt_1->at(zIdx) > elePt && diEle.elePt_2->at(zIdx) > elePt))  continue;

            // selection of electron 1 based on ECAL regions
            if (TMath::Abs(diEle.eleSCEta_1->at(zIdx)) < 1.4442)
            {
                if (!(diEle.eleSigmaIEtaIEta_2012_1->at(zIdx) < eleSigmaIEtaIEta_2012_EB)) continue;
                if (!(TMath::Abs(diEle.eledEtaAtVtx_1->at(zIdx)) < eledEtaAtVtx_abs_EB)) continue;
                if (!(TMath::Abs(diEle.eledPhiAtVtx_1->at(zIdx)) < eledPhiAtVtx_abs_EB)) continue;
                if (!(diEle.eleHoverE_1->at(zIdx) < eleHoverE_EB)) continue;
                if (!(diEle.eleEoverPInv_1->at(zIdx) < eleEoverPInv_EB)) continue;
                if (!(TMath::Abs(diEle.eleD0_1->at(zIdx)) < eleD0_abs_EB)) continue;
                if (!(TMath::Abs(diEle.eleDz_1->at(zIdx)) < eleDz_abs_EB)) continue;
                if (!(diEle.eleMissHits_1->at(zIdx) <= eleMissHits_EB)) continue;
            }
            else if (TMath::Abs(diEle.eleSCEta_1->at(zIdx)) > 1.566 && TMath::Abs(diEle.eleSCEta_1->at(zIdx)) < 2.5)
            {
                if (!(diEle.eleSigmaIEtaIEta_2012_1->at(zIdx) < eleSigmaIEtaIEta_2012_EE)) continue;
                if (!(TMath::Abs(diEle.eledEtaAtVtx_1->at(zIdx)) < eledEtaAtVtx_abs_EE)) continue;
                if (!(TMath::Abs(diEle.eledPhiAtVtx_1->at(zIdx)) < eledPhiAtVtx_abs_EE)) continue;
                if (!(diEle.eleHoverE_1->at(zIdx) < eleHoverE_EE)) continue;
                if (!(diEle.eleEoverPInv_1->at(zIdx) < eleEoverPInv_EE)) continue;
                if (!(TMath::Abs(diEle.eleD0_1->at(zIdx)) < eleD0_abs_EE)) continue;
                if (!(TMath::Abs(diEle.eleDz_1->at(zIdx)) < eleDz_abs_EE)) continue;
                if (!(diEle.eleMissHits_1->at(zIdx) <= eleMissHits_EE)) continue;
            }
            else  {
                continue;
            }

            // selection of electron 2 based on ECAL regions
            if (TMath::Abs(diEle.eleSCEta_2->at(zIdx)) < 1.4442)
            {
                if (!(diEle.eleSigmaIEtaIEta_2012_2->at(zIdx) < eleSigmaIEtaIEta_2012_EB)) continue;
                if (!(TMath::Abs(diEle.eledEtaAtVtx_2->at(zIdx)) < eledEtaAtVtx_abs_EB)) continue;
                if (!(TMath::Abs(diEle.eledPhiAtVtx_2->at(zIdx)) < eledPhiAtVtx_abs_EB)) continue;
                if (!(diEle.eleHoverE_2->at(zIdx) < eleHoverE_EB)) continue;
                if (!(diEle.eleEoverPInv_2->at(zIdx) < eleEoverPInv_EB)) continue;
                if (!(TMath::Abs(diEle.eleD0_2->at(zIdx)) < eleD0_abs_EB)) continue;
                if (!(TMath::Abs(diEle.eleDz_2->at(zIdx)) < eleDz_abs_EB)) continue;
                if (!(diEle.eleMissHits_2->at(zIdx) <= eleMissHits_EB)) continue;
            }
            else if (TMath::Abs(diEle.eleSCEta_2->at(zIdx)) > 1.566 && TMath::Abs(diEle.eleSCEta_2->at(zIdx)) < 2.5)
            {
                if (!(diEle.eleSigmaIEtaIEta_2012_2->at(zIdx) < eleSigmaIEtaIEta_2012_EE)) continue;
                if (!(TMath::Abs(diEle.eledEtaAtVtx_2->at(zIdx)) < eledEtaAtVtx_abs_EE)) continue;
                if (!(TMath::Abs(diEle.eledPhiAtVtx_2->at(zIdx)) < eledPhiAtVtx_abs_EE)) continue;
                if (!(diEle.eleHoverE_2->at(zIdx) < eleHoverE_EE)) continue;
                if (!(diEle.eleEoverPInv_2->at(zIdx) < eleEoverPInv_EE)) continue;
                if (!(TMath::Abs(diEle.eleD0_2->at(zIdx)) < eleD0_abs_EE)) continue;
                if (!(TMath::Abs(diEle.eleDz_2->at(zIdx)) < eleDz_abs_EE)) continue;
                if (!(diEle.eleMissHits_2->at(zIdx) <= eleMissHits_EE)) continue;
            }
            else  {
                continue;
            }

        }

        // dimuon block
        if (doDiMuon > 0)
        {
            // pt selection
            if (!(diMu.muPt_1->at(zIdx) > muPt && diMu.muPt_2->at(zIdx) > muPt))  continue;

            // selection of muon 1
            if (TMath::Abs(diMu.muEta_1->at(zIdx)) < 2.4)
            {
                if (!(diMu.muChi2NDF_1->at(zIdx) < muChi2NDF)) continue;
                if (!(TMath::Abs(diMu.muInnerD0_1->at(zIdx)) < muInnerD0)) continue;
                if (!(TMath::Abs(diMu.muInnerDz_1->at(zIdx)) < muInnerDz)) continue;
                if (!(diMu.muMuonHits_1->at(zIdx) > muMuonHits)) continue;
                if (!(diMu.muStations_1->at(zIdx) > muStations)) continue;
                if (!(diMu.muTrkLayers_1->at(zIdx) > muTrkLayers)) continue;
                if (!(diMu.muPixelHits_1->at(zIdx) > muPixelHits)) continue;
            }
            else  {
                continue;
            }

            // selection of muon 2
            if (TMath::Abs(diMu.muEta_2->at(zIdx)) < 2.4)
            {
                if (!(diMu.muChi2NDF_2->at(zIdx) < muChi2NDF)) continue;
                if (!(TMath::Abs(diMu.muInnerD0_2->at(zIdx)) < muInnerD0)) continue;
                if (!(TMath::Abs(diMu.muInnerDz_2->at(zIdx)) < muInnerDz)) continue;
                if (!(diMu.muMuonHits_2->at(zIdx) > muMuonHits)) continue;
                if (!(diMu.muStations_2->at(zIdx) > muStations)) continue;
                if (!(diMu.muTrkLayers_2->at(zIdx) > muTrkLayers)) continue;
                if (!(diMu.muPixelHits_2->at(zIdx) > muPixelHits)) continue;
            }
            else  {
                continue;
            }
        }

        entriesPassedLeptonSelection++;

        // Z block
        float zMass = -1;
        float zPt = -1;
        float zEta = -1;
        float zPhi = -1;
        if (doDiElectron > 0) {
            zMass = diEle.diEleM->at(zIdx);
            zPt = diEle.diElePt->at(zIdx);
            zEta = diEle.diEleEta->at(zIdx);
            zPhi = diEle.diElePhi->at(zIdx);
        }
        if (doDiMuon     > 0) {
            zMass = diMu.diMuM->at(zIdx);
            zPt = diMu.diMuPt->at(zIdx);
            zEta = diMu.diMuEta->at(zIdx);
            zPhi = diMu.diMuPhi->at(zIdx);
        }

        if (!(zMass >= massMin && zMass <= massMax))  continue;
        entriesPassedZmassSelection++;

        double eventWeightAll = 1;
        double vertexWeight = 1;
        double centWeight = 1;
        if (doEventWeight > 0) {
            // 1.3649*exp(-0.5*((HiEvt.vz-0.62814)/7.7109)^2)
            //vertexWeight = 1.3649*TMath::Exp(-0.5*TMath::Power((vz-0.62814)/7.7109, 2));
            vertexWeight = 1.37487*TMath::Exp(-0.5*TMath::Power((vz-0.30709)/7.41379, 2));  // 02.04.2016
            if (isHI)  centWeight = findNcoll(hiBin);
            eventWeightAll = weight_HiEvt * vertexWeight * centWeight;
        }

        // zJet correlation block
        // bool doneSingleJet = false;     // flag to keep track of histograms that are filled only once per zJet event, e.g. : zM, zPt
        // bool doneEventCount = false;
        for (int iCorr = 0; iCorr < CORR::kN_CORRFNC - 1; ++iCorr)
        {
            if (iCorr == CORR::kBKG && (!hasJetsMB || !hasZJetMB))  continue;

            // pt Bin and hiBin selections
            for(int iPt=0; iPt<nBins_pt; ++iPt){
                for(int iHiBin=0; iHiBin<nBins_hiBin; ++iHiBin){

                    bool passedZPt;
                    if (bins_pt[1].at(iPt) >= 0) passedZPt = ( zPt >= bins_pt[0].at(iPt) && zPt < bins_pt[1].at(iPt) );
                    else                         passedZPt = ( zPt >= bins_pt[0].at(iPt) );
                    if (!passedZPt) continue;

                    if (isHI) {
                        bool passedHiBin = (hiBin >= bins_hiBin[0].at(iHiBin) && hiBin < bins_hiBin[1].at(iHiBin));
                        if (!passedHiBin) continue;
                    }

                    if (iCorr == CORR::kRAW)  {
                        if (isMC)  entriesZ[iPt][iHiBin] += eventWeightAll;
                        else       entriesZ[iPt][iHiBin]++;
                        entriesZRaw[iPt][iHiBin]++;
                    }

                    bool isZJetEvent = false;
                    int nJet = 0;
                    for (int i = 0; i < jets[iCorr].nref; ++i)
                    {
                        // selection on jet properties
                        if (!(jets[iCorr].jtpt[i] > cut_jetpt))  continue;
                        if (!(TMath::Abs(jets[iCorr].jteta[i]) < cut_jeteta))  continue;
                        // selection on zJet properties
                        bool isAwaySideJet = false;
                        if (TMath::Abs(zJets[iCorr].dphi->at(i)) > cut_awayRange &&
                                TMath::Abs(zJets[iCorr].dphi->at(i)) <= cut_awayRange_lt)  isAwaySideJet = true;
                        if (!(zJets[iCorr].dR_ele_1->at(i) > cut_dR))  continue;
                        if (!(zJets[iCorr].dR_ele_2->at(i) > cut_dR))  continue;
                        // if (!(zJets[iCorr].jetID->at(i) >= cut_jetID))  continue;

                        bool passedJetID = true;
                        if (cut_jetID > 0) {
                            passedJetID = ((jets[iCorr].neutralMax[i]/jets[iCorr].rawpt[i])*(0.085)+
                                    jets[iCorr].photonMax[i]/jets[iCorr].rawpt[i]*(-0.337)+
                                    jets[iCorr].chargedMax[i]/jets[iCorr].rawpt[i]*0.584+
                                    jets[iCorr].neutralSum[i]/jets[iCorr].rawpt[i]*(-0.454)+
                                    jets[iCorr].photonSum[i]/jets[iCorr].rawpt[i]*(-0.127)+
                                    jets[iCorr].chargedSum[i]/jets[iCorr].rawpt[i]*(-0.239)+
                                    jets[iCorr].jtpu[i]/jets[iCorr].rawpt[i]*(-0.184)+0.173 > -0.37);
                            if (!passedJetID)  continue;
                        }

                        if (isAwaySideJet && iCorr == CORR::kRAW) isZJetEvent = true;
                        nJet++;

                        double w = 1;
                        w = eventWeightAll;
                        if (doCorrectionMatchingEff > 0) {
                            if (isHI) {
                                w *= correctorJet.getMatchingEfficiencyCorrection(jets[iCorr], i);
                            }
                        }
                        if (doCorrectionJetID > 0 && cut_jetID > 0 && jets[iCorr].jtpt[i] < 100) {
                            if (ratioHist_jetID[iPt][iHiBin].hRatioInitialized) {

                                int bin = ratioHist_jetID[iPt][iHiBin].hRatio->FindBin(jets[iCorr].jtpt[i]);

                                bool validBin = (bin > 0 && bin <= ratioHist_jetID[iPt][iHiBin].hRatio->GetNbinsX());

                                double val= ratioHist_jetID[iPt][iHiBin].hRatio->GetBinContent(bin);

                                if (validBin && val > 0 && val <= 1) {
                                    w *= (1/val);
                                }
                            }
                        }

                        // histograms for zJet correlations
                        for(int iHist = 0; iHist<nCorrHist; ++iHist)
                        {
                            if (doAwaySideJets.at(iHist) && !isAwaySideJet)  continue;

                            std::string tmpObservable = correlationHistNames.at(iHist).c_str();
                            if (tmpObservable.compare("xjz") == 0) {
                                corrHists[iHist][iPt][iHiBin].h1D[CORR::kRAW][iCorr]->Fill(zJets[iCorr].xjz->at(i), w);
                                corrHists[iHist][iPt][iHiBin].h1D_final[CORR::kRAW][iCorr]->Fill(zJets[iCorr].xjz->at(i), w);
                            }
                            else if (tmpObservable.compare("dphi") == 0) {
                                corrHists[iHist][iPt][iHiBin].h1D[CORR::kRAW][iCorr]->Fill(TMath::Abs(zJets[iCorr].dphi->at(i)), w);
                                corrHists[iHist][iPt][iHiBin].h1D_final[CORR::kRAW][iCorr]->Fill(TMath::Abs(zJets[iCorr].dphi->at(i)), w);
                            }
                            else if (tmpObservable.compare("ptJet") == 0) {
                                corrHists[iHist][iPt][iHiBin].h1D[CORR::kRAW][iCorr]->Fill(jets[iCorr].jtpt[i], w);
                                corrHists[iHist][iPt][iHiBin].h1D_final[CORR::kRAW][iCorr]->Fill(jets[iCorr].jtpt[i], w);
                            }
                            else if (tmpObservable.compare("jteta") == 0) {
                                corrHists[iHist][iPt][iHiBin].h1D[CORR::kRAW][iCorr]->Fill(jets[iCorr].jteta[i], w);
                                corrHists[iHist][iPt][iHiBin].h1D_final[CORR::kRAW][iCorr]->Fill(jets[iCorr].jteta[i], w);
                            }
                            else if (tmpObservable.compare("jtphi") == 0) {
                                corrHists[iHist][iPt][iHiBin].h1D[CORR::kRAW][iCorr]->Fill(jets[iCorr].jtphi[i], w);
                                corrHists[iHist][iPt][iHiBin].h1D_final[CORR::kRAW][iCorr]->Fill(jets[iCorr].jtphi[i], w);
                            }
                            else if (tmpObservable.compare("nJet") == 0) {
                                corrHists[iHist][iPt][iHiBin].h1D[CORR::kRAW][iCorr]->Fill(nJet, w);
                                corrHists[iHist][iPt][iHiBin].h1D_final[CORR::kRAW][iCorr]->Fill(nJet, w);
                            }
                        }
                    } // i

                    if (iCorr == CORR::kRAW) {
                        // histograms that are filled only once per zJet event
                        // in particular, histograms for Z in zJet events
                        if (isZJetEvent)
                        {
                            // use weighted number of events for MC
                            if (isMC)  entriesZJet[iPt][iHiBin] += eventWeightAll;
                            else       entriesZJet[iPt][iHiBin]++;
                            entriesZJetRaw[iPt][iHiBin]++;

                            double w = 1;
                            w = eventWeightAll;

                            for(int iHist = 0; iHist<nCorrHist; ++iHist)
                            {
                                if (!doSingleJet.at(iHist))  continue;

                                std::string tmpObservable = correlationHistNames.at(iHist).c_str();
                                if (tmpObservable.compare("zM") == 0) {
                                    corrHists[iHist][iPt][iHiBin].h1D[CORR::kRAW][CORR::kRAW]->Fill(zMass, w);
                                    corrHists[iHist][iPt][iHiBin].h1D_final[CORR::kRAW][CORR::kRAW]->Fill(zMass, w);
                                }
                                else if (tmpObservable.compare("zPt") == 0) {
                                    corrHists[iHist][iPt][iHiBin].h1D[CORR::kRAW][CORR::kRAW]->Fill(zPt, w);
                                    corrHists[iHist][iPt][iHiBin].h1D_final[CORR::kRAW][CORR::kRAW]->Fill(zPt, w);
                                }
                                else if (tmpObservable.compare("zEta") == 0) {
                                    corrHists[iHist][iPt][iHiBin].h1D[CORR::kRAW][CORR::kRAW]->Fill(zEta, w);
                                    corrHists[iHist][iPt][iHiBin].h1D_final[CORR::kRAW][CORR::kRAW]->Fill(zEta, w);
                                }
                                else if (tmpObservable.compare("zPhi") == 0) {
                                    corrHists[iHist][iPt][iHiBin].h1D[CORR::kRAW][CORR::kRAW]->Fill(zPhi, w);
                                    corrHists[iHist][iPt][iHiBin].h1D_final[CORR::kRAW][CORR::kRAW]->Fill(zPhi, w);
                                }
                            }
                        }
                    }

                }  // ihiBin
            } // iPt
        } // iCorr
        entriesAnalyzed++;
    }
    std::cout<<  "Loop ENDED : tzj" <<std::endl;
    std::cout << "entries            = " << entries << std::endl;
    std::cout << "entriesPassedEventSelection   = " << entriesPassedEventSelection << std::endl;
    std::cout << "entriesPassedLeptonSelection  = " << entriesPassedLeptonSelection << std::endl;
    std::cout << "entriesPassedZmassSelection   = " << entriesPassedZmassSelection << std::endl;
    std::cout << "entriesAnalyzed               = " << entriesAnalyzed << std::endl;

    // WRITE HISTOGRAM AND CANVAS
    TCanvas* c = new TCanvas("cnv","",600,600);
    for (int iPt=0; iPt<nBins_pt; ++iPt) {
        for (int iHiBin=0; iHiBin<nBins_hiBin; ++iHiBin) {
            std::cout<<"##########"<<std::endl;
            std::cout << Form ("ptBin%d HiBin%d :", iPt, iHiBin)<<std::endl;
            std::cout << "entriesZJet = " << entriesZJet[iPt][iHiBin] << std::endl;
            std::cout << "entriesZ = " << entriesZ[iPt][iHiBin] << std::endl;
            std::cout << "entriesZJetRaw = " << entriesZJetRaw[iPt][iHiBin] << std::endl;
            std::cout << "entriesZRaw = " << entriesZRaw[iPt][iHiBin] << std::endl;

            h_nums[iPt][iHiBin]->SetBinContent(1, entriesZJet[iPt][iHiBin]);
            h_nums[iPt][iHiBin]->SetBinContent(2, entriesZ[iPt][iHiBin]);
            h_nums[iPt][iHiBin]->SetBinContent(3, nEventsToMix);
            h_nums[iPt][iHiBin]->Write("",TObject::kOverwrite);

            // write histograms for RAW and BKG regions
            int iCorr = CORR::kRAW;
            for(int iHist = 0; iHist<nCorrHist; ++iHist)
            {
                for (int jCorr = 0; jCorr < CORR::kN_CORRFNC -1; ++jCorr) {

                    if (jCorr == CORR::kBKG && !isHI)  continue;      // no jet background for non-HI

                    corrHists[iHist][iPt][iHiBin].h1D[iCorr][jCorr]->SetMarkerStyle(kFullCircle);
                    corrHists[iHist][iPt][iHiBin].h1D[iCorr][jCorr]->SetMarkerColor(kBlack);

                    // histogram name excluding the "h1D" prefix
                    std::string tmpH1D_name = corrHists[iHist][iPt][iHiBin].h1D_name[iCorr][jCorr].c_str();

                    c->SetName(Form("cnv_%s",tmpH1D_name.c_str()));
                    c->cd();
                    corrHists[iHist][iPt][iHiBin].h1D[iCorr][jCorr]->Draw("e");
                    corrHists[iHist][iPt][iHiBin].h1D[iCorr][jCorr]->Write("",TObject::kOverwrite);
                    corrHists[iHist][iPt][iHiBin].h1D[iCorr][jCorr]->SetStats(false);     // remove stat box from the canvas, but keep in the histograms.
                    c->Write("",TObject::kOverwrite);
                    c->Clear();

                    // FINAL
                    corrHists[iHist][iPt][iHiBin].h1D_final[iCorr][jCorr]->SetMarkerStyle(kFullCircle);
                    corrHists[iHist][iPt][iHiBin].h1D_final[iCorr][jCorr]->SetMarkerColor(kBlack);
                    c->SetName(Form("cnv_%s_final",tmpH1D_name.c_str()));
                    c->cd();
                    corrHists[iHist][iPt][iHiBin].h1D_final[iCorr][jCorr]->Draw("e");
                    corrHists[iHist][iPt][iHiBin].h1D_final[iCorr][jCorr]->Write("",TObject::kOverwrite);
                    corrHists[iHist][iPt][iHiBin].h1D_final[iCorr][jCorr]->SetStats(false);  // remove stat box from the canvas, but keep in the histograms.
                    c->Write("",TObject::kOverwrite);
                    c->Clear();

                    // FINAL_NORM
                    std::string tmpHistName = corrHists[iHist][iPt][iHiBin].h1D[iCorr][jCorr]->GetName();
                    corrHists[iHist][iPt][iHiBin].h1D_final_norm[iCorr][jCorr] =
                            (TH1D*)corrHists[iHist][iPt][iHiBin].h1D_final[iCorr][jCorr]->Clone(Form("%s_final_norm", tmpHistName.c_str()));
                    if (jCorr == CORR::kBKG && hasJetsMB && hasZJetMB) {   // (hasJetsMB && hasZJetMB) ==> isHI
                        // normalize first by the number of mixed events
                        corrHists[iHist][iPt][iHiBin].h1D_final_norm[iCorr][jCorr]->Scale(1./nEventsToMix);
                    }
                    int tmpNEntriesZ = entriesZ[iPt][iHiBin];
                    // normalization is done with Z boson events, not necessarily by Z-jet events
                    corrHists[iHist][iPt][iHiBin].h1D_final_norm[iCorr][jCorr]->Scale(1./tmpNEntriesZ);
                    std::string tmpTitleY_final_norm = corrHists[iHist][iPt][iHiBin].h1D_titleY_final_norm[iCorr][jCorr].c_str();
                    corrHists[iHist][iPt][iHiBin].h1D_final_norm[iCorr][jCorr]->GetYaxis()->SetTitle(tmpTitleY_final_norm.c_str());

                    c->SetName(Form("cnv_%s_final_norm",tmpH1D_name.c_str()));
                    c->cd();
                    corrHists[iHist][iPt][iHiBin].h1D_final_norm[iCorr][jCorr]->Draw("e");
                    corrHists[iHist][iPt][iHiBin].h1D_final_norm[iCorr][jCorr]->Write("",TObject::kOverwrite);
                    c->Write("",TObject::kOverwrite);
                    c->Clear();
                }

                // calculate SIGSIG histogram,
                // these histograms are ignored : SIGRAW, SIGBKG

                // no background for PP and PA
                if(!isHI){
                    // for non-HI, reset jet BKG histograms
                    // in that case BKG histograms are not used, but set to empty histograms.
                    // so it becomes SIG = RAW

                    std::string tmpHistName;

                    // reset RAWBKG
                    tmpHistName = Form("%s_final_norm", corrHists[iHist][iPt][iHiBin].h1D[CORR::kRAW][CORR::kBKG]->GetName());
                    corrHists[iHist][iPt][iHiBin].h1D_final_norm[CORR::kRAW][CORR::kBKG] =
                            (TH1D*)corrHists[iHist][iPt][iHiBin].h1D_final_norm[CORR::kRAW][CORR::kRAW]->Clone(tmpHistName.c_str());
                    corrHists[iHist][iPt][iHiBin].h1D_final_norm[CORR::kRAW][CORR::kBKG]->Reset();

                    // there is no BKGBKG
                }

                // first subtract jet BKG, there is no Z BKG
                // subtract jet BKG
                // RAWSIG = RAWRAW - RAWBKG
                std::string tmpHistNameRAWSIG = Form("%s_final_norm", corrHists[iHist][iPt][iHiBin].h1D[CORR::kRAW][CORR::kSIG]->GetName());
                corrHists[iHist][iPt][iHiBin].h1D_final_norm[CORR::kRAW][CORR::kSIG] =
                                     (TH1D*)corrHists[iHist][iPt][iHiBin].h1D_final_norm[CORR::kRAW][CORR::kRAW]->Clone(tmpHistNameRAWSIG.c_str());
                // do arithmetic if histograms are not empty
                if (corrHists[iHist][iPt][iHiBin].h1D_final_norm[CORR::kRAW][CORR::kBKG]->GetEntries() > 0) {
                    corrHists[iHist][iPt][iHiBin].h1D_final_norm[CORR::kRAW][CORR::kSIG]->Add(corrHists[iHist][iPt][iHiBin].h1D_final_norm[CORR::kRAW][CORR::kBKG],-1);
                }

                // there is no BKGRAW, BKGBKG, BKGSIG

                // there is no Z BKG

                // FINAL_NORM  RAWSIG
                std::string tmpH1D_nameRAWSIG = corrHists[iHist][iPt][iHiBin].h1D_name[CORR::kRAW][CORR::kSIG].c_str();
                c->SetName(Form("cnv_%s_final_norm",tmpH1D_nameRAWSIG.c_str()));
                c->cd();
                corrHists[iHist][iPt][iHiBin].h1D_final_norm[CORR::kRAW][CORR::kSIG]->Draw("e");
                corrHists[iHist][iPt][iHiBin].h1D_final_norm[CORR::kRAW][CORR::kSIG]->Write("",TObject::kOverwrite);
                corrHists[iHist][iPt][iHiBin].h1D_final_norm[CORR::kRAW][CORR::kSIG]->SetStats(false);  // remove stat box from the canvas, but keep in the histograms.
                c->Write("",TObject::kOverwrite);
                c->Clear();

                // there is no FINAL_NORM  BKGSIG

                // FINAL_NORM  SIGSIG
            }
            std::cout<<"##########"<<std::endl;
        }
    }

    // histograms with pt bins on x-axis
    // corrHists_ptBinAll[0][]  = R_jz
    // corrHists_ptBinAll[1][]  = <X_jz>
    std::cout<<"####################"<<std::endl;
    for(int iHiBin=0; iHiBin<nBins_hiBin; ++iHiBin){

        int iCorr = CORR::kRAW;

        int offset = 3; // ptBin 40-50 starts from index 3.
        if (nBins_pt < offset + nBins_rjz) break;

        // rjg block
        for(int i=0; i<nBins_rjz; ++i){
            // calculate R_JZ from the integral of R_JZ
            int nBinsX = corrHists[0][i+offset][iHiBin].h1D_final_norm[iCorr][0]->GetNbinsX();
            double val;
            double err;
            val = corrHists[0][i+offset][iHiBin].h1D_final_norm[iCorr][0]->IntegralAndError(1, nBinsX, err);

            corrHists_ptBinAll[0][iHiBin].h1D[iCorr][0]->SetBinContent(i+1, val);
            corrHists_ptBinAll[0][iHiBin].h1D[iCorr][0]->SetBinError(i+1, err);

            // h_nums[][] are histograms to store numbers
            // 1st bin stores the number of zJet events
            // 2nd bin stores the number of Z events, not zJet event
            // R_JZ calculation using number of events
            rjz_num[iHiBin].h1D[iCorr][0]->SetBinContent(i+1, h_nums[i+offset][iHiBin]->GetBinContent(1));
            rjz_num[iHiBin].h1D[iCorr][0]->SetBinError(i+1, TMath::Sqrt(h_nums[i+offset][iHiBin]->GetBinContent(1)));
            rjz_denom[iHiBin].h1D[iCorr][0]->SetBinContent(i+1, h_nums[i+offset][iHiBin]->GetBinContent(2));
            rjz_denom[iHiBin].h1D[iCorr][0]->SetBinError(i+1, TMath::Sqrt(h_nums[i+offset][iHiBin]->GetBinContent(2)));
        }
        corrHists_ptBinAll[2][iHiBin].h1D[iCorr][0]->Add(rjz_num[iHiBin].h1D[iCorr][0]);
        corrHists_ptBinAll[2][iHiBin].h1D[iCorr][0]->Divide(rjz_denom[iHiBin].h1D[iCorr][0]);

        // write those histograms without creating a canvas for them
        rjz_num[iHiBin].h1D[iCorr][0]->Write("",TObject::kOverwrite);
        rjz_denom[iHiBin].h1D[iCorr][0]->Write("",TObject::kOverwrite);

        // R_JZ
        corrHists_ptBinAll[0][iHiBin].h1D[iCorr][0]->SetMarkerStyle(kFullCircle);
        corrHists_ptBinAll[0][iHiBin].h1D[iCorr][0]->SetMarkerColor(kBlack);

        std::cout<<"drawing : "<<corrHists_ptBinAll[0][iHiBin].h1D_name[iCorr][0].c_str()<<std::endl;
        c->SetName(Form("cnv_%s",corrHists_ptBinAll[0][iHiBin].h1D_name[iCorr][0].c_str()));
        c->cd();
        corrHists_ptBinAll[0][iHiBin].h1D[iCorr][0]->Draw("e");
        corrHists_ptBinAll[0][iHiBin].h1D[iCorr][0]->Write("",TObject::kOverwrite);
        corrHists_ptBinAll[0][iHiBin].h1D[iCorr][0]->SetStats(false);     // remove stat box from the canvas, but keep in the histograms.
        c->Write("",TObject::kOverwrite);
        c->Clear();

        // R_JZ using number of events
        corrHists_ptBinAll[2][iHiBin].h1D[iCorr][0]->SetMarkerStyle(kFullCircle);
        corrHists_ptBinAll[2][iHiBin].h1D[iCorr][0]->SetMarkerColor(kBlack);

        std::cout<<"drawing : "<<corrHists_ptBinAll[2][iHiBin].h1D_name[iCorr][0].c_str()<<std::endl;
        c->SetName(Form("cnv_%s",corrHists_ptBinAll[2][iHiBin].h1D_name[iCorr][0].c_str()));
        c->cd();
        corrHists_ptBinAll[2][iHiBin].h1D[iCorr][0]->Draw("e");
        corrHists_ptBinAll[2][iHiBin].h1D[iCorr][0]->Write("",TObject::kOverwrite);
        corrHists_ptBinAll[2][iHiBin].h1D[iCorr][0]->SetStats(false);     // remove stat box from the canvas, but keep in the histograms.
        c->Write("",TObject::kOverwrite);
        c->Clear();

        // do not calculate xjz_mean directly from jetSIG any more.
        // array of <x_jz> mean and weights for RAW and BKG correlations
        // [0][i] : <x_jz> for jetRAW in ptBin i
        // [1][i] : weight of <x_jz> for jetRAW in ptBin i
        // [2][i] : error  of <x_jz> for jetRAW in ptBin i
        // [3][i] : <x_jz> for jetBKG in ptBin i
        // [4][i] : weight of <x_jz> for jetBKG in ptBin i
        // [5][i] : error  of <x_jz> for jetBKG in ptBin i
        /*
         * should follow this procedure to calculate mean of X_JZ :

           mean_RAW * w_RAW = mean_SIG * w_SIG + mean_BKG * w_BKG
           mean_RAW * w_RAW - mean_BKG * w_BKG = mean_SIG * w_SIG
           by definition : w_SIG = w_RAW - w_BKG
           ==> mean_SIG = (mean_RAW * w_RAW - mean_BKG * w_BKG) / (w_RAW - w_BKG)

           so it will not be ratio of weigthed sums, but ratio of weighted differences.
           this approach should avoid the case of negative entries.
         */
        std::vector<double> meansANDweights[nBins_xjz_mean];
        for(int jCorr = 0; jCorr<CORR::kN_CORRFNC; ++jCorr) {
            if((iHiBin > 0 || jCorr > 0) && !isHI) continue;
            // xjz_mean block
            for(int i=0; i<nBins_xjz_mean; ++i){

                if (jCorr == CORR::kRAW || jCorr == CORR::kBKG) {
                    double val = corrHists[0][i+offset][iHiBin].h1D_final_norm[iCorr][jCorr]->GetMean();
                    double sumWeights = corrHists[0][i+offset][iHiBin].h1D_final_norm[iCorr][jCorr]->GetSumOfWeights();
                    double err = corrHists[0][i+offset][iHiBin].h1D_final_norm[iCorr][jCorr]->GetMeanError();

                    meansANDweights[i].push_back(val);
                    meansANDweights[i].push_back(sumWeights);
                    meansANDweights[i].push_back(err);

                    corrHists_ptBinAll[1][iHiBin].h1D[iCorr][jCorr]->SetBinContent(i+1, val);
                    corrHists_ptBinAll[1][iHiBin].h1D[iCorr][jCorr]->SetBinError(i+1, err);
                }
                else if (jCorr == CORR::kSIG) {
                    // mean_SIG = (mean_RAW * w_RAW - mean_BKG * w_BKG) / (w_RAW - w_BKG)
                    double sumWeightsSIG = meansANDweights[i].at(1) - meansANDweights[i].at(4);
                    double meanSIG = (meansANDweights[i].at(0)*meansANDweights[i].at(1) - meansANDweights[i].at(3)*meansANDweights[i].at(4)) / sumWeightsSIG;
                    double errSIG = TMath::Sqrt(meansANDweights[i].at(2)*meansANDweights[i].at(2)*meansANDweights[i].at(1)*meansANDweights[i].at(1)
                                              - meansANDweights[i].at(5)*meansANDweights[i].at(5)*meansANDweights[i].at(4)*meansANDweights[i].at(4)) / sumWeightsSIG;
                    /*
                     * Var(aX+bY)=a^2*Var(X)+b^2*Var(Y)+2*a*b*Cov(X,Y)
                     * ==>
                     * w_RAW^2*var_RAW = w_SIG^2*var_SIG+w_BKG^2*var_BKG+2*w_SIG*w_BKG*Cov(SIG,BKG)
                     * assume Cov(SIG,BKG) = 0
                     * ==>
                     * var_SIG = ( w_RAW^2*var_RAW-w_BKG^2*var_BKG ) / (w_RAW - w_BKG)
                     */

                    corrHists_ptBinAll[1][iHiBin].h1D[iCorr][jCorr]->SetBinContent(i+1, meanSIG);
                    corrHists_ptBinAll[1][iHiBin].h1D[iCorr][jCorr]->SetBinError(i+1, errSIG);
                }
            }

            corrHists_ptBinAll[1][iHiBin].h1D[iCorr][jCorr]->SetMarkerStyle(kFullCircle);
            corrHists_ptBinAll[1][iHiBin].h1D[iCorr][jCorr]->SetMarkerColor(kBlack);

            std::cout<<"drawing : "<<corrHists_ptBinAll[1][iHiBin].h1D_name[iCorr][jCorr].c_str()<<std::endl;
            c->SetName(Form("cnv_%s",corrHists_ptBinAll[1][iHiBin].h1D_name[iCorr][jCorr].c_str()));
            c->cd();
            corrHists_ptBinAll[1][iHiBin].h1D[iCorr][jCorr]->Draw("e");
            corrHists_ptBinAll[1][iHiBin].h1D[iCorr][jCorr]->Write("",TObject::kOverwrite);
            corrHists_ptBinAll[1][iHiBin].h1D[iCorr][jCorr]->SetStats(false);     // remove stat box from the canvas, but keep in the histograms.
            c->Write("",TObject::kOverwrite);
            c->Clear();
        }
    }
    std::cout<<"####################"<<std::endl;

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

std::string makeHistTitle(bool isHI, std::string collisionName, float ptBinLow, float ptBinUp, int hiBinLow, int hiBinUp)
{
    std::string histoTitle;
    if (isHI) {
        histoTitle = Form("%s , %.0f < p^{Z}_{T} < %.0f GeV/c, %d-%d %% ",collisionName.c_str() , ptBinLow, ptBinUp, hiBinLow/2, hiBinUp/2);

        // special cases
        if (ptBinLow <= 0 && ptBinUp < 0 && hiBinLow <= 0 && hiBinUp >= 200 )   {
            histoTitle = Form("%s",collisionName.c_str() );
        }
        else if (ptBinLow <= 0 && ptBinUp < 0)   {
            histoTitle = Form("%s , %d-%d %%",collisionName.c_str() , hiBinLow/2, hiBinUp/2);
        }
        else if (ptBinUp < 0 && hiBinLow <= 0 && hiBinUp >= 200)   {
            histoTitle = Form("%s ,  p^{Z}_{T} > %.0f GeV/c",collisionName.c_str() , ptBinLow);
        }
        else if (hiBinLow <= 0 && hiBinUp >= 200)   {
            histoTitle = Form("%s , %.0f < p^{Z}_{T} < %.0f GeV/c",collisionName.c_str() , ptBinLow, ptBinUp);
        }
        else if (ptBinUp < 0) {
            histoTitle = Form("%s , p^{Z}_{T} > %.0f GeV/c, %d-%d %% ",collisionName.c_str() , ptBinLow, hiBinLow/2, hiBinUp/2);
        }
    }
    else {
        histoTitle = Form("%s , %.0f < p^{Z}_{T} < %.0f GeV/c",collisionName.c_str() , ptBinLow, ptBinUp);

        // special cases
        if (ptBinLow <= 0 && ptBinUp < 0)   {
            histoTitle = Form("%s",collisionName.c_str() );
        }
        else if (ptBinUp < 0) {
            histoTitle = Form("%s , p^{Z}_{T} > %.0f GeV/c", collisionName.c_str() , ptBinLow);
        }
    }

    return histoTitle;
}
