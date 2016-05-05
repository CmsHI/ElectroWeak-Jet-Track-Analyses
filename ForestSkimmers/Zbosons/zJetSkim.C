#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TRandom3.h>

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

#include "../../CorrelationTuple/EventMatcher.h"
#include "../../TreeHeaders/zJetTree.h"
#include "../../TreeHeaders/CutConfigurationTree.h"
#include "../../Plotting/commonUtility.h"
#include "../../Utilities/interface/CutConfigurationParser.h"
#include "../../Utilities/interface/InputConfigurationParser.h"
#include "../../Utilities/interface/HiForestInfoController.h"
#include "../../Corrections/electrons/electronCorrector.h"
#include "../../Corrections/jets/jetCorrector.h"
#include "../../Corrections/jets/L2L3/L2L3ResidualWFits.h"

const long MAXTREESIZE = 500000000000; // set maximum tree size from 10 GB to 100 GB, so that the code does not switch to a new file after 10 GB

void zJetSkim(const TString configFile, const TString inputFile, const TString outputFile = "zJetSkim.root", const TString minBiasJetSkimFile = "");

void zJetSkim(const TString configFile, const TString inputFile, const TString outputFile, const TString minBiasJetSkimFile)
{
       std::cout<<"running zJetSkim()"<<std::endl;
       std::cout<<"configFile  = "<< configFile.Data() <<std::endl;
       std::cout<<"inputFile   = "<< inputFile.Data() <<std::endl;
       std::cout<<"outputFile  = "<< outputFile.Data() <<std::endl;
       std::cout<<"minBiasJetSkimFile = "<< minBiasJetSkimFile.Data() <<std::endl;

       InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
       CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

       // input configuration
       int collisionType;
       if (configInput.isValid) {
           collisionType = configInput.proc[INPUT::kSKIM].i[INPUT::k_collisionType];
       }
       else {
           collisionType = COLL::kPP;
       }
       // verbose about input configuration
       std::cout<<"Input Configuration :"<<std::endl;
       std::cout << "collisionType = " << collisionType << std::endl;
       const char* collisionName =  getCollisionTypeName((COLL::TYPE)collisionType).c_str();
       std::cout << "collision = " << collisionName << std::endl;

       // cut configuration
       float cut_vz;
       int cut_pcollisionEventSelection;
       int cut_pPAprimaryVertexFilter;
       int cut_pBeamScrapingFilter;

       std::vector<std::string> jetCollections;
       int doCorrectionResidual;
       double energyScaleJet;
       int doCorrectionSmearing;
       int doCorrectionSmearingPhi;
       int jetAlgoSmearing;
       int smearingHiBin;
       int doCorrectionL2L3;
       // electron cuts
       int cut_nEle;
       int doCorrectionEle;
       float energyScaleEle;
       float elePt;
       float eleSigmaIEtaIEta_2012_EB;
       float eleSigmaIEtaIEta_2012_EE;
       float eleHoverE_EB;
       float eleHoverE_EE;
       // muon cuts
       int cut_nMu;
       float muPt;
       float muChi2NDF;
       float muInnerD0;
       float muInnerDz;
       int muMuonHits;
       int muStations;
       int muTrkLayers;
       int muPixelHits;
       // Z cuts
       int doDiElectron;
       int doDiMuon;
       float cutZMassMin;
       float cutZMassMax;
       float cutZPt;
       float cutZEta;
       float smearZ;
       TRandom3 randSmearZ(12345);

       int doMix;
       int nMaxEvents_minBiasMixing;
       int nCentralityBins;
       int nVertexBins;
       int nEventsToMix;
       if (configCuts.isValid) {
           cut_vz = configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].f[CUTS::EVT::k_vz];
           cut_pcollisionEventSelection = configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].i[CUTS::EVT::k_pcollisionEventSelection];
           cut_pPAprimaryVertexFilter = configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].i[CUTS::EVT::k_pPAprimaryVertexFilter];
           cut_pBeamScrapingFilter = configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].i[CUTS::EVT::k_pBeamScrapingFilter];
           
           jetCollections = ConfigurationParser::ParseList(configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].s[CUTS::JET::k_jetCollection]);
           doCorrectionResidual = configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].i[CUTS::JET::k_doCorrectionResidual];
           energyScaleJet = configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].f[CUTS::JET::k_energyScale];
           doCorrectionSmearing = configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].i[CUTS::JET::k_doCorrectionSmearing];
           doCorrectionSmearingPhi = configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].i[CUTS::JET::k_doCorrectionSmearingPhi];
           jetAlgoSmearing = configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].i[CUTS::JET::k_jetAlgoSmearing];
           smearingHiBin = configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].i[CUTS::JET::k_smearingHiBin];
           doCorrectionL2L3 = configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].i[CUTS::JET::k_doCorrectionL2L3];

           cut_nEle = configCuts.proc[CUTS::kSKIM].obj[CUTS::kELECTRON].i[CUTS::ELE::k_nEle];
           doCorrectionEle = configCuts.proc[CUTS::kSKIM].obj[CUTS::kELECTRON].i[CUTS::ELE::k_doCorrection];
           energyScaleEle = configCuts.proc[CUTS::kSKIM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_energyScale];
           elePt = configCuts.proc[CUTS::kSKIM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_elePt];
           eleSigmaIEtaIEta_2012_EB = configCuts.proc[CUTS::kSKIM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleSigmaIEtaIEta_2012_EB];
           eleSigmaIEtaIEta_2012_EE = configCuts.proc[CUTS::kSKIM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleSigmaIEtaIEta_2012_EE];
           eleHoverE_EB = configCuts.proc[CUTS::kSKIM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleHoverE_EB];
           eleHoverE_EE = configCuts.proc[CUTS::kSKIM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleHoverE_EE];

           cut_nMu = configCuts.proc[CUTS::kSKIM].obj[CUTS::kMUON].i[CUTS::MUO::k_nMu];
           muPt = configCuts.proc[CUTS::kSKIM].obj[CUTS::kMUON].f[CUTS::MUO::k_muPt];
           muChi2NDF = configCuts.proc[CUTS::kSKIM].obj[CUTS::kMUON].f[CUTS::MUO::k_muChi2NDF];
           muInnerD0 = configCuts.proc[CUTS::kSKIM].obj[CUTS::kMUON].f[CUTS::MUO::k_muInnerD0];
           muInnerDz = configCuts.proc[CUTS::kSKIM].obj[CUTS::kMUON].f[CUTS::MUO::k_muInnerDz];
           muMuonHits = configCuts.proc[CUTS::kSKIM].obj[CUTS::kMUON].i[CUTS::MUO::k_muMuonHits];
           muStations = configCuts.proc[CUTS::kSKIM].obj[CUTS::kMUON].i[CUTS::MUO::k_muStations];
           muTrkLayers = configCuts.proc[CUTS::kSKIM].obj[CUTS::kMUON].i[CUTS::MUO::k_muTrkLayers];
           muPixelHits = configCuts.proc[CUTS::kSKIM].obj[CUTS::kMUON].i[CUTS::MUO::k_muPixelHits];

           doDiElectron = configCuts.proc[CUTS::kSKIM].obj[CUTS::kZBOSON].i[CUTS::ZBO::k_doDiElectron];
           doDiMuon = configCuts.proc[CUTS::kSKIM].obj[CUTS::kZBOSON].i[CUTS::ZBO::k_doDiMuon];
           cutZMassMin = configCuts.proc[CUTS::kSKIM].obj[CUTS::kZBOSON].f[CUTS::ZBO::k_massMin];
           cutZMassMax = configCuts.proc[CUTS::kSKIM].obj[CUTS::kZBOSON].f[CUTS::ZBO::k_massMax];
           cutZPt = configCuts.proc[CUTS::kSKIM].obj[CUTS::kZBOSON].f[CUTS::ZBO::k_pt];
           cutZEta = configCuts.proc[CUTS::kSKIM].obj[CUTS::kZBOSON].f[CUTS::ZBO::k_eta];
           smearZ = configCuts.proc[CUTS::kSKIM].obj[CUTS::kZBOSON].f[CUTS::ZBO::k_smearZ];

           doMix = configCuts.proc[CUTS::kSKIM].obj[CUTS::kZJET].i[CUTS::ZJT::k_doMix];
           nMaxEvents_minBiasMixing = configCuts.proc[CUTS::kSKIM].obj[CUTS::kZJET].i[CUTS::ZJT::k_nMaxEvents_minBiasMixing];
           nCentralityBins = configCuts.proc[CUTS::kSKIM].obj[CUTS::kZJET].i[CUTS::ZJT::k_nCentralityBins];
           nVertexBins = configCuts.proc[CUTS::kSKIM].obj[CUTS::kZJET].i[CUTS::ZJT::k_nVertexBins];
           nEventsToMix = configCuts.proc[CUTS::kSKIM].obj[CUTS::kZJET].i[CUTS::ZJT::k_nEventsToMix];
       }
       else {
           cut_vz = 15;
           cut_pcollisionEventSelection = 1;
           cut_pPAprimaryVertexFilter = 1;
           cut_pBeamScrapingFilter = 1;

           doCorrectionResidual = 0;
           energyScaleJet = 0;
           doCorrectionSmearing = 0;
           doCorrectionSmearingPhi = 0;
           jetAlgoSmearing = CUTS::JET::k_akPU;
           smearingHiBin = 0;
           doCorrectionL2L3 = 0;

           cut_nEle = 2;
           doCorrectionEle = 0;
           energyScaleEle = 0;
           elePt = 0;
           eleSigmaIEtaIEta_2012_EB = 0.02;
           eleSigmaIEtaIEta_2012_EE = 0.045;
           eleHoverE_EB = 0.2;
           eleHoverE_EE = 0.15;

           cut_nMu = 2;
           muPt = 0;
           muChi2NDF = 10;
           muInnerD0 = 0.2;
           muInnerDz = 0.5;

           muMuonHits = 0;
           muStations = 1;
           muTrkLayers = 5;
           muPixelHits = 0;

           doDiElectron = 1;
           doDiMuon = 0;
           cutZMassMin = 80;
           cutZMassMax = 100;
           cutZPt = 15;
           cutZEta = 1.44;
           smearZ = 0;

           // default : no mixing
           doMix = 0;
           nMaxEvents_minBiasMixing = 0;
           nCentralityBins = 0;
           nVertexBins = 0;
           nEventsToMix = 0;
       }
       int nJetCollections = jetCollections.size();

       if(minBiasJetSkimFile.EqualTo("")) {
           std::cout<<"no minBiasJetSkimFile was provided."<<std::endl;
           std::cout<<"Minimum Bias event mixing will be skipped"<<std::endl;
           doMix = 0;
       }

       bool isMC = collisionIsMC((COLL::TYPE)collisionType);
       bool isHI = collisionIsHI((COLL::TYPE)collisionType);
       bool isPP = collisionIsPP((COLL::TYPE)collisionType);

       // verbose about cut configuration
       std::cout<<"Cut Configuration :"<<std::endl;
       std::cout<<"cut_vz = "<< cut_vz <<std::endl;
       if (isHI) {
           std::cout<<"cut_pcollisionEventSelection = "<< cut_pcollisionEventSelection <<std::endl;
       }
       else {   // PP
           std::cout<<"cut_pPAprimaryVertexFilter = "<< cut_pPAprimaryVertexFilter <<std::endl;
           std::cout<<"cut_pBeamScrapingFilter = "<< cut_pBeamScrapingFilter <<std::endl;
       }
       
       std::cout<<"nJetCollections = "<< nJetCollections <<std::endl;
       for (int i=0; i<nJetCollections; ++i) {
           std::cout << Form("jetCollections[%d] = %s", i, jetCollections.at(i).c_str()) << std::endl;
       }
       std::cout<<"doCorrectionResidual    = "<< doCorrectionResidual <<std::endl;
       std::cout<<"energyScaleJet          = "<< energyScaleJet <<std::endl;
       std::cout<<"doCorrectionSmearing    = "<< doCorrectionSmearing <<std::endl;
       std::cout<<"doCorrectionSmearingPhi = "<< doCorrectionSmearingPhi <<std::endl;
       std::cout<<"jetAlgoSmearing = "<< jetAlgoSmearing <<std::endl;
       std::cout<<"smearingHiBin           = "<< smearingHiBin <<std::endl;
       std::cout<<"doCorrectionL2L3        = "<< doCorrectionL2L3 <<std::endl;

       std::cout<<"doDiElectron = "<<doDiElectron<<std::endl;
       if (doDiElectron > 0) {
           std::cout<<"cut_nEle = "<<cut_nEle<<std::endl;
           std::cout<<"doCorrectionEle = "<<doCorrectionEle<<std::endl;
           std::cout<<"energyScaleEle  = "<<energyScaleEle<<std::endl;
           std::cout<<"elePt = "<<elePt<<std::endl;
           std::cout<<"eleSigmaIEtaIEta_2012_EB = "<<eleSigmaIEtaIEta_2012_EB<<std::endl;
           std::cout<<"eleSigmaIEtaIEta_2012_EE = "<<eleSigmaIEtaIEta_2012_EE<<std::endl;
           std::cout<<"eleHoverE_EB = "<<eleHoverE_EB<<std::endl;
           std::cout<<"eleHoverE_EE = "<<eleHoverE_EE<<std::endl;
       }

       std::cout<<"doDiMuon = "<<doDiMuon<<std::endl;
       if (doDiMuon > 0) {
           std::cout<<"cut_nMu = "<<cut_nMu<<std::endl;
           std::cout<<"muPt = "<<muPt<<std::endl;
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

       std::cout<<"cutZMassMin = "<<cutZMassMin<<std::endl;
       std::cout<<"cutZMassMax = "<<cutZMassMax<<std::endl;
       std::cout<<"cutZPt   = "<<cutZPt<<std::endl;
       std::cout<<"cutZEta  = "<<cutZEta<<std::endl;
       std::cout<<"smearZ  = "<<smearZ<<std::endl;

       std::cout<<"doMix    = "<<doMix<<std::endl;
       if (doMix > 0)
       {
           std::cout<<"nMaxEvents_minBiasMixing = "<< nMaxEvents_minBiasMixing <<std::endl;
           std::cout<<"nCentralityBins          = "<< nCentralityBins <<std::endl;
           std::cout<<"nVertexBins              = "<< nVertexBins <<std::endl;
           std::cout<<"nEventsToMix             = "<< nEventsToMix <<std::endl;
       }

       std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.Data());

       std::cout<<"input ROOT files : num = "<<inputFiles.size()<< std::endl;
       std::cout<<"#####"<< std::endl;
       for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
           std::cout<<(*it).c_str()<< std::endl;
       }
       std::cout<<"##### END #####"<< std::endl;

       TChain* treeHLT   = new TChain("hltanalysis/HltTree");
       TChain* treeggHiNtuplizer  = new TChain("ggHiNtuplizer/EventTree");
       TChain* treeEvent = new TChain("ggHiNtuplizer/EventTree");
       TChain* treeJet[nJetCollections];
       for (int i=0; i<nJetCollections; ++i) {
           treeJet[i] = new TChain(Form("%s/t", jetCollections.at(i).c_str()));
       }
       TChain* treeHiEvt = new TChain("hiEvtAnalyzer/HiTree");
       TChain* treeSkim  = new TChain("skimanalysis/HltTree");
       TChain* treeHiForestInfo = new TChain("HiForest/HiForestInfo");

       for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
          treeHLT->Add((*it).c_str());
          treeggHiNtuplizer->Add((*it).c_str());
          treeEvent->Add((*it).c_str());
          for (int i=0; i<nJetCollections; ++i) {
              treeJet[i]->Add((*it).c_str());
          }
          treeHiEvt->Add((*it).c_str());
          treeSkim->Add((*it).c_str());
          treeHiForestInfo->Add((*it).c_str());
       }

       HiForestInfoController hfic(treeHiForestInfo);
       std::cout<<"### HiForestInfo Tree ###"<< std::endl;
       hfic.printHiForestInfo();
       std::cout<<"###"<< std::endl;

       treeHLT->SetBranchStatus("*",0);     // disable all branches
       treeHLT->SetBranchStatus("HLT_HI*SinglePhoton*Eta*v1*",1);     // enable photon branches
       treeHLT->SetBranchStatus("HLT_HI*DoublePhoton*Eta*v1*",1);     // enable photon branches
       treeHLT->SetBranchStatus("*DoubleMu*",1);                      // enable muon branches
       treeHLT->SetBranchStatus("HLT_HIL1Mu*",1);                     // enable muon branches
       treeHLT->SetBranchStatus("HLT_HIL2Mu*",1);                     // enable muon branches
       treeHLT->SetBranchStatus("HLT_HIL3Mu*",1);                     // enable muon branches

       treeEvent->SetBranchStatus("*",0);
       treeEvent->SetBranchStatus("run",1);
       treeEvent->SetBranchStatus("event",1);
       treeEvent->SetBranchStatus("lumis",1);

       for (int i=0; i<nJetCollections; ++i) {
           treeJet[i]->SetBranchStatus("*",0);        // disable all branches
           treeJet[i]->SetBranchStatus("nref",1);     // enable jet branches
           treeJet[i]->SetBranchStatus("rawpt",1);    // enable jet branches
           treeJet[i]->SetBranchStatus("jtpt",1);     // enable jet branches
           treeJet[i]->SetBranchStatus("jteta",1);    // enable jet branches
           treeJet[i]->SetBranchStatus("jtphi",1);    // enable jet branches
           treeJet[i]->SetBranchStatus("jtpu",1);     // enable jet branches
           treeJet[i]->SetBranchStatus("track*",1);
           treeJet[i]->SetBranchStatus("charged*",1);
           treeJet[i]->SetBranchStatus("photon*",1);
           treeJet[i]->SetBranchStatus("neutral*",1);
           treeJet[i]->SetBranchStatus("eMax*",1);
           treeJet[i]->SetBranchStatus("eSum*",1);
           treeJet[i]->SetBranchStatus("eN*",1);
           treeJet[i]->SetBranchStatus("muMax*",1);
           treeJet[i]->SetBranchStatus("muSum*",1);
           treeJet[i]->SetBranchStatus("muN*",1);
           if (isMC) {
               treeJet[i]->SetBranchStatus("matchedPt",1);
               treeJet[i]->SetBranchStatus("matchedR",1);
               treeJet[i]->SetBranchStatus("beamId1",1);
               treeJet[i]->SetBranchStatus("beamId2",1);
               treeJet[i]->SetBranchStatus("pthat",1);
               treeJet[i]->SetBranchStatus("ngen",1);
               treeJet[i]->SetBranchStatus("gen*",1);
               treeJet[i]->SetBranchStatus("signalChargedSum",1);
               treeJet[i]->SetBranchStatus("signalHardSum",1);
               treeJet[i]->SetBranchStatus("subid",1);
               treeJet[i]->SetBranchStatus("smpt",1);
               treeJet[i]->SetBranchStatus("fr01Chg",1);
               treeJet[i]->SetBranchStatus("fr01EM",1);
               treeJet[i]->SetBranchStatus("fr01",1);
           }
       }

       // specify explicitly which branches to store, do not use wildcard
       treeHiEvt->SetBranchStatus("*",0);
       treeHiEvt->SetBranchStatus("run",1);
       treeHiEvt->SetBranchStatus("evt",1);
       treeHiEvt->SetBranchStatus("lumi",1);
       treeHiEvt->SetBranchStatus("vz",1);
       treeHiEvt->SetBranchStatus("hiBin",1);
       treeHiEvt->SetBranchStatus("hiHF",1);
       treeHiEvt->SetBranchStatus("hiHFplus",1);
       treeHiEvt->SetBranchStatus("hiHFminus",1);
       treeHiEvt->SetBranchStatus("hiHFplusEta4",1);
       treeHiEvt->SetBranchStatus("hiHFminusEta4",1);
       treeHiEvt->SetBranchStatus("hiNevtPlane",1);
       if (isMC) {
           treeHiEvt->SetBranchStatus("Npart",1);
           treeHiEvt->SetBranchStatus("Ncoll",1);
           treeHiEvt->SetBranchStatus("Nhard",1);
           treeHiEvt->SetBranchStatus("ProcessID",1);
           treeHiEvt->SetBranchStatus("pthat",1);
           treeHiEvt->SetBranchStatus("weight",1);
           treeHiEvt->SetBranchStatus("alphaQCD",1);
           treeHiEvt->SetBranchStatus("alphaQED",1);
           treeHiEvt->SetBranchStatus("qScale",1);
           treeHiEvt->SetBranchStatus("npus",1);        // store pileup info
           treeHiEvt->SetBranchStatus("tnpus",1);       // store pileup info
       }

       float vz;
       Int_t hiBin;

       treeHiEvt->SetBranchAddress("vz",&vz);
       treeHiEvt->SetBranchAddress("hiBin",&hiBin);

       // specify explicitly which branches to store, do not use wildcard
       treeSkim->SetBranchStatus("*",0);

       Int_t pcollisionEventSelection;  // this filter is used for HI.
       if (isHI) {
           treeSkim->SetBranchStatus("pcollisionEventSelection",1);
           if (treeSkim->GetBranch("pcollisionEventSelection")) {
               treeSkim->SetBranchAddress("pcollisionEventSelection",&pcollisionEventSelection);
           }
           else {   // overwrite to default
               pcollisionEventSelection = 1;
               std::cout<<"could not get branch : pcollisionEventSelection"<<std::endl;
               std::cout<<"set to default value : pcollisionEventSelection = "<<pcollisionEventSelection<<std::endl;
           }
       }
       else {
           pcollisionEventSelection = 0;    // default value if the collision is not HI, will not be used anyway.
       }
       Int_t pPAprimaryVertexFilter;    // this filter is used for PP.
       if (isPP) {
           treeSkim->SetBranchStatus("pPAprimaryVertexFilter",1);
           if (treeSkim->GetBranch("pPAprimaryVertexFilter")) {
               treeSkim->SetBranchAddress("pPAprimaryVertexFilter",&pPAprimaryVertexFilter);
           }
           else {   // overwrite to default
               pPAprimaryVertexFilter = 1;
               std::cout<<"could not get branch : pPAprimaryVertexFilter"<<std::endl;
               std::cout<<"set to default value : pPAprimaryVertexFilter = "<<pPAprimaryVertexFilter<<std::endl;
           }
       }
       else {
           pPAprimaryVertexFilter = 0;      // default value if the collision is not PP, will not be used anyway.
       }
       Int_t pBeamScrapingFilter;   // this filter is used for PP.
       if (isPP) {
           treeSkim->SetBranchStatus("pBeamScrapingFilter",1);
           if (treeSkim->GetBranch("pBeamScrapingFilter")) {
               treeSkim->SetBranchAddress("pBeamScrapingFilter",&pBeamScrapingFilter);
           }
           else {   // overwrite to default
               pBeamScrapingFilter = 1;
               std::cout<<"could not get branch : pBeamScrapingFilter"<<std::endl;
               std::cout<<"set to default value : pBeamScrapingFilter = "<<pBeamScrapingFilter<<std::endl;
           }
       }
       else {
           pBeamScrapingFilter = 0;     // default value if the collision is not PP, will not be used anyway.
       }

       // event information
       UInt_t run, lumis;
       ULong64_t event;
       treeEvent->SetBranchAddress("run", &run);
       treeEvent->SetBranchAddress("event", &event);
       treeEvent->SetBranchAddress("lumis", &lumis);

       // objects for z-jet correlations
       ggHiNtuplizer ggHi;
       ggHi.setupTreeForReading(treeggHiNtuplizer);    // treeggHiNtuplizer is input
       std::vector<Jets> jets(nJetCollections);
       for (int i=0; i<nJetCollections; ++i) {
           jets.at(i).setupTreeForReading(treeJet[i]);   // treeJet is input
       }

       // correctors
       electronCorrector correctorEle;
       if (doCorrectionEle > 0) {
           if (isHI) {
               std::string pathEB = "Corrections/electrons/weights/BDTG_EB_PbPb_16V.weights.xml";
               std::string pathEE = "Corrections/electrons/weights/BDTG_EE_PbPb_16V.weights.xml";
               correctorEle.initiliazeReader(pathEB.c_str(), pathEE.c_str());
           }
       }

       std::vector<L2L3ResidualWFits> correctorsL2L3(nJetCollections);
       if (isPP && doCorrectionL2L3 > 0)
       {
           for (int i = 0; i < nJetCollections; ++i) {
               correctorsL2L3.at(i).setL2L3Residual(3, 3, false);
           }
       }

       std::vector<int> hiBins_residual{60, 200};
       int nHiBins_residual = hiBins_residual.size();
       std::vector<jetCorrector> correctorsJetResidual(nJetCollections*nHiBins_residual);
       TFile* fileResidual = 0;
       TF1* f1Residual[nHiBins_residual];
       if (doCorrectionResidual > 0 && isHI) {

           std::string pathCorrectionResidual = "Corrections/jets/merged_dgulhan-Pythia8_Dijet30_pp_TuneCUETP8M1_Hydjet_MinBias_5020GeV_RECODEBUG_758_PrivMC_forest_v28_0_20160505_pthat_30_RESIDUALCORR.root";

           std::cout << "pathCorrectionResidual = " << pathCorrectionResidual.c_str() << std::endl;
           fileResidual = new TFile(pathCorrectionResidual.c_str(), "READ");

           f1Residual[0] = (TF1*)((TH1D*)fileResidual->Get("jtRecoOverGenVPt_Inc_Mean_akPu3PF_cent0to30_h"))->GetFunction("f1_p")->Clone("f1Residual_0");
           f1Residual[1] = (TF1*)((TH1D*)fileResidual->Get("jtRecoOverGenVPt_Inc_Mean_akPu3PF_cent30to100_h"))->GetFunction("f1_p")->Clone("f1Residual_1");

           if (fileResidual->IsOpen())  fileResidual->Close();
       }

       std::vector<jetCorrector> correctorsJetJES(nJetCollections);

       std::vector<jetCorrector> correctorsJetSmear(nJetCollections);
       TRandom3 randSmearing(12345);    // random number seed should be fixed or reproducible
       if (doCorrectionSmearing > 0 || doCorrectionSmearingPhi > 0) {
           for (int i = 0; i < nJetCollections; ++i) {
               correctorsJetSmear.at(i).rand = randSmearing;

               // variation of smearing by 15% for pt smearing.
               if (doCorrectionSmearing == 2)  correctorsJetSmear.at(i).sigma_rel_Var = 1.15;
               else if (doCorrectionSmearing == 3)  correctorsJetSmear.at(i).sigma_rel_Var = 0.85;

               // variation of smearing by 15% for phi smearing.
               if (doCorrectionSmearingPhi == 2)  correctorsJetSmear.at(i).sigmaPhi_rel_Var = 1.15;
               else if (doCorrectionSmearingPhi == 3)  correctorsJetSmear.at(i).sigmaPhi_rel_Var = 0.85;

               std::vector<double> CSN_PP = {0.07764, 0.9648, -0.0003191};
               std::vector<double> CSN_phi_PP = {7.72/100000000, 0.1222, 0.5818};
               correctorsJetSmear.at(i).CSN_PP = CSN_PP;
               correctorsJetSmear.at(i).CSN_phi_PP = CSN_phi_PP;

               if (smearingHiBin == 1) {    // smear 0-30 %
                   std::vector<double> CSN_HI = {0.1136, 0.7955, 7.778};
                   std::vector<double> CSN_phi_HI = {-0.01584, 0.03229, 1.954};

                   std::vector<double> CSN_HI_akCs = {0.04991, 1.25, 12.43};
                   std::vector<double> CSN_phi_HI_akCs = {0.001314, 0.07899, 2.034};

                   if (jetAlgoSmearing == CUTS::JET::k_akPU)
                   {
                       correctorsJetSmear.at(i).CSN_HI = CSN_HI;
                       correctorsJetSmear.at(i).CSN_phi_HI = CSN_phi_HI;
                   }
                   else if (jetAlgoSmearing == CUTS::JET::k_akCS)
                   {
                       correctorsJetSmear.at(i).CSN_HI = CSN_HI_akCs;
                       correctorsJetSmear.at(i).CSN_phi_HI = CSN_phi_HI_akCs;
                   }
                   else {
                       std::cout << "jetAlgoSmearing = " << jetAlgoSmearing << " is not a proper value" <<std::endl;
                       std::cout << "exiting"<< std::endl;
                       return;
                   }
               }
               else if (smearingHiBin == 2) {    // smear 30-100 %
                   std::vector<double> CSN_HI = {0.1136, 0.7955, 1.761};
                   std::vector<double> CSN_phi_HI = {0.0168, 2.018/10000000, 1.249};

                   std::vector<double> CSN_HI_akCs = {0.04991, 1.25, 1.907};
                   std::vector<double> CSN_phi_HI_akCs = {-0.006015, 0.07578, 1.234};

                   if (jetAlgoSmearing == CUTS::JET::k_akPU)
                   {
                       correctorsJetSmear.at(i).CSN_HI = CSN_HI;
                       correctorsJetSmear.at(i).CSN_phi_HI = CSN_phi_HI;
                   }
                   else if (jetAlgoSmearing == CUTS::JET::k_akCS)
                   {
                       correctorsJetSmear.at(i).CSN_HI = CSN_HI_akCs;
                       correctorsJetSmear.at(i).CSN_phi_HI = CSN_phi_HI_akCs;
                   }
                   else {
                       std::cout << "jetAlgoSmearing = " << jetAlgoSmearing << " is not a proper value." <<std::endl;
                       std::cout << "exiting"<< std::endl;
                       return;
                   }
               }
           }
       }

       // mixed-event block
       int centBinWidth = 0;
       int vertexBinWidth = 0;
       std::vector<Jets> jetsMB(nJetCollections);     // object to read jet trees from MB events
       TTree* treeJetMB[nCentralityBins][nVertexBins][nJetCollections];
       Long64_t nMB[nCentralityBins][nVertexBins][nJetCollections];
       Long64_t iterMB[nCentralityBins][nVertexBins][nJetCollections];   // index of the tree where the mixing starts
       TFile* inputMB =0;
       if (doMix > 0) {
           centBinWidth = 200/nCentralityBins;  // number of "hiBin"s that a centrality bin covers
           vertexBinWidth = 30/nVertexBins;     // number of "vz"s    that a vertex     bin covers
                                                // accepted vz range is -15 to 15.

           inputMB = new TFile(minBiasJetSkimFile, "READ");

           TRandom3 rand(12345);    // random number seed should be fixed or reproducible
           std::cout <<"Tree initialization for MinBias mixing" << std::endl;
           std::cout <<Form("treeJetMB[%d][%d][%d] is being read", nCentralityBins, nVertexBins, nJetCollections)<< std::endl;
           for(int i=0; i<nCentralityBins; ++i)
           {
               for(int j=0; j<nVertexBins; ++j){
                   for(int k=0; k<nJetCollections; ++k){

                       // in minBiasJetSkimFile, name of a jet tree starts with jetCollection.
                       std::string jetCollection = jetCollections.at(k).c_str();
                       treeJetMB[i][j][k] = (TTree*)inputMB->Get(Form("%s_centBin%d_vzBin%d",jetCollection.c_str(), i, j));
                       if (!treeJetMB[i][j][k]) {
                           std::cout << "jetCollection = " << jetCollection.c_str() << " is not found in minBias jet skim file" <<std::endl;
                           std::cout << "exiting"<< std::endl;
                           return;
                       }
                       nMB[i][j][k] = treeJetMB[i][j][k]->GetEntries();

                       jetsMB.at(k).setupTreeForReading(treeJetMB[i][j][k]);    // all MB jet trees point to jetsMB
                       int primeSeed = 0;
                       if(nMB[i][j][k] != 0) primeSeed = rand.Integer(nMB[i][j][k]); // Integer(imax) Returns a random integer on [0, imax-1].
                       iterMB[i][j][k] = primeSeed;

                       if (nMB[i][j][k] < nEventsToMix){
                           std::cout << "centBin = "<<i<<", vzBin = "<<j<<", jetCollection = "<<jetCollection.c_str()<<std::endl;
                           std::cout << "nMB[centBin][vzBin][jetCollection] = "<<nMB[i][j][k]<<std::endl;
                           std::cout << "nEventsToMix = "<<nEventsToMix<<std::endl;
                           std::cout << "number of MB events in that bin is not enough for mixing" <<std::endl;
                       }
                   }
               }
           }
       }

       TFile* output = new TFile(outputFile,"RECREATE");
       TTree* configTree = setupConfigurationTreeForWriting(configCuts);

       // output tree variables
       TTree* outputTreeHLT    = treeHLT->CloneTree(0);
       outputTreeHLT->SetName("hltTree");
       outputTreeHLT->SetTitle("subbranches of hltanalysis/HltTree");
       TTree* outputTreeggHiNtuplizer = treeggHiNtuplizer->CloneTree(0);
       TTree* outputTreeJet[nJetCollections];
       for (int i=0; i<nJetCollections; ++i) {
           outputTreeJet[i] = treeJet[i]->CloneTree(0);

           // pick a unique, but also not complicated name for jet Trees
           // jet collection names which are complicated will be put into tree title
           std::string treeJetName = jetCollections.at(i).c_str();
           std::string treeJetTitle = jetCollections.at(i).c_str();
           std::string currentTitle = outputTreeJet[i]->GetTitle();
           // do not lose the current title
           if (currentTitle.size() > 0) treeJetTitle = Form("%s - %s", treeJetTitle.c_str(), currentTitle.c_str());

           outputTreeJet[i]->SetName(treeJetName.c_str());
           outputTreeJet[i]->SetTitle(treeJetTitle.c_str());
       }
       TTree* outputTreeHiEvt = treeHiEvt->CloneTree(0);
       outputTreeHiEvt->SetName("HiEvt");
       outputTreeHiEvt->SetTitle("subbranches of hiEvtAnalyzer/HiTree");
       TTree* outputTreeSkim   = treeSkim->CloneTree(0);
       outputTreeSkim->SetName("skim");
       outputTreeSkim->SetTitle("subbranches of skimanalysis/HltTree");
//       TTree* outputTreeHiForestInfo = treeHiForestInfo->CloneTree(0);
//       outputTreeHiForestInfo->SetName("HiForestInfo");
//       outputTreeHiForestInfo->SetTitle("first entry of HiForest/HiForestInfo");

       outputTreeHLT->SetMaxTreeSize(MAXTREESIZE);
       outputTreeggHiNtuplizer->SetMaxTreeSize(MAXTREESIZE);
       outputTreeHiEvt->SetMaxTreeSize(MAXTREESIZE);
       for (int i=0; i<nJetCollections; ++i) {
           outputTreeJet[i]->SetMaxTreeSize(MAXTREESIZE);
       }
       outputTreeSkim->SetMaxTreeSize(MAXTREESIZE);
//       outputTreeHiForestInfo->SetMaxTreeSize(MAXTREESIZE);
//
//       // write HiForestInfo
//       treeHiForestInfo->GetEntry(0);
//       outputTreeHiForestInfo->Fill();

       // trees for diLepton pairs
       TTree* diElectronTree=0;
       // construct dielectron pairs during zJet skim
       if (doDiElectron > 0)
       {
           diElectronTree = new TTree("dielectron","electron pairs");
           diElectronTree->SetMaxTreeSize(MAXTREESIZE);
       }
       TTree* diMuonTree =0;
       // construct dimuon pairs during zJet skim
       if (doDiMuon > 0)
       {
           diMuonTree = new TTree("dimuon","muon pairs");
           diMuonTree->SetMaxTreeSize(MAXTREESIZE);
       }

       // objects for z-jet correlations
       dielectron diEle;
       if (doDiElectron > 0)  diEle.branchDiElectronTree(diElectronTree);     // diElectronTree is output
       dimuon diMu;
       if (doDiMuon > 0) diMu.branchDiMuonTree(diMuonTree);     // diMuonTree is output

       TTree* zJetTree[nJetCollections];
       for (int i=0; i<nJetCollections; ++i) {

           // pick a unique, but also not complicated name for zJet Trees
           // jet collection names which are complicated will be put into tree title
           std::string treezJetName = Form("z_%s", jetCollections.at(i).c_str());
           std::string treezJetTitle = Form("%s : leading z-jet correlations", jetCollections.at(i).c_str());

           zJetTree[i] = new TTree(treezJetName.c_str(),treezJetTitle.c_str());
           zJetTree[i]->SetMaxTreeSize(MAXTREESIZE);
       }
       std::vector<ZJet> zjet(nJetCollections);
       for (int i=0; i<nJetCollections; ++i) {
           zjet.at(i).resetAwayRange();
           zjet.at(i).resetConeRange();
           zjet.at(i).branchZJetTree(zJetTree[i]);
       }

       // mixed-event block
       std::vector<Jets> jetsMBoutput(nJetCollections);     // object to write jet trees from MB events
       std::vector<ZJet> zjetMB(nJetCollections);
       TTree* outputTreeJetMB[nJetCollections];
       TTree* zJetTreeMB[nJetCollections];
       if (doMix > 0) {
           for (int i=0; i<nJetCollections; ++i) {

               // jetMB trees
               // pick a unique, but also not complicated name for jetMB Trees
               // jet collection names which are complicated will be put into tree title
               std::string treeJetMBName = Form("%sMB", jetCollections.at(i).c_str());
               std::string treeJetMBTitle = Form("%s : jets from MB events", jetCollections.at(i).c_str());
               outputTreeJetMB[i] = new TTree(treeJetMBName.c_str(), treeJetMBTitle.c_str());

               outputTreeJetMB[i]->SetMaxTreeSize(MAXTREESIZE);
               jetsMBoutput.at(i).setupTreeForWritingMB(outputTreeJetMB[i], true, false);

               // zJetMB trees
               // pick a unique, but also not complicated name for zJetMB Trees
               // jet collection names which are complicated will be put into tree title
               std::string treezJetMBName = Form("z_%sMB", jetCollections.at(i).c_str());
               std::string treezJetMBTitle = Form("%s : leading z-jet correlations", jetCollections.at(i).c_str());
               zJetTreeMB[i] = new TTree(treezJetMBName.c_str(),treezJetMBTitle.c_str());

               zJetTreeMB[i]->SetMaxTreeSize(MAXTREESIZE);
               zjetMB.at(i).resetAwayRange();
               zjetMB.at(i).resetConeRange();
               zjetMB.at(i).branchZJetTree(zJetTreeMB[i]);
           }
       }

       EventMatcher* em = new EventMatcher();
       Long64_t duplicateEntries = 0;

       const double zMassConst = 91.18;
       Long64_t entries = treeEvent->GetEntries();
       Long64_t entriesPassedEventSelection = 0;
       Long64_t entriesAnalyzed = 0;
       std::cout << "entries = " << entries << std::endl;
       std::cout<< "Loop : ggHiNtuplizer/EventTree" <<std::endl;
       for (Long64_t j_entry=0; j_entry<entries; ++j_entry)
       {
           if (j_entry % 2000 == 0)  {
             std::cout << "current entry = " <<j_entry<<" out of "<<entries<<" : "<<std::setprecision(2)<<(double)j_entry/entries*100<<" %"<<std::endl;
           }

           treeHLT->GetEntry(j_entry);
           treeggHiNtuplizer->GetEntry(j_entry);
           treeEvent->GetEntry(j_entry);
           for (int i=0; i<nJetCollections; ++i) {
               treeJet[i]->GetEntry(j_entry);
           }
           treeSkim->GetEntry(j_entry);
           treeHiEvt->GetEntry(j_entry);

           bool eventAdded = em->addEvent(run,lumis,event,j_entry);
           if(!eventAdded) // this event is duplicate, skip this one.
           {
               duplicateEntries++;
               continue;
           }

           // event selection
           if (!(TMath::Abs(vz) < cut_vz))  continue;
           if (isHI) {
               if ((pcollisionEventSelection < cut_pcollisionEventSelection))  continue;
           }
           else {
               if (pPAprimaryVertexFilter < cut_pPAprimaryVertexFilter || pBeamScrapingFilter < cut_pBeamScrapingFilter)  continue;
           }
           entriesPassedEventSelection++;

           int zIdx = -1;     // index of the leading z

           // dielectron block
           if (doDiElectron > 0) {
               // skip if there are no electron pairs to study
               if(ggHi.nEle < cut_nEle)  continue;

               if(doCorrectionEle > 0)
               {
                   // correct the pt of electrons
                   // note that "elePt" branch of "outputTreeggHiNtuplizer" will be corrected as well.
                   if (isHI)  correctorEle.correctPts(ggHi);
               }
               if (energyScaleEle != 0 && energyScaleEle != 1)
               {
                   correctorEle.applyEnergyScale(ggHi, energyScaleEle);
               }

               // construct dielectron pairs during zJet skim
               diEle.makeDiElectronPairs(ggHi);

               // Zee-jet block
               // find leading z from dielectron
               // double maxZPt = -1;
               double minDiffZMass = 9999;
               for(unsigned int i=0; i<(unsigned)diEle.diEleM_out.size(); ++i)
               {
                   if (smearZ > 0)
                   {
                       diEle.diElePt_out.at(i) *= randSmearZ.Gaus(1, smearZ);
                   }

                   bool failedPtCut  = (diEle.diElePt_out.at(i) < cutZPt) ;
                   bool failedEtaCut = (TMath::Abs(diEle.diEleEta_out.at(i)) > cutZEta) ;
                   bool failedMassWindow = (diEle.diEleM_out.at(i) < cutZMassMin || diEle.diEleM_out.at(i) > cutZMassMax);
                   // bool failedOppositeCh = (diEle.eleCharge_1_out.at(i) == diEle.eleCharge_2_out.at(i));

                   if (failedPtCut)          continue;
                   if (failedEtaCut)         continue;
                   if (failedMassWindow)     continue;
                   // if (failedOppositeCh)     continue;

                   // some extra and rather loose cuts based on eta region
                   // electron 1
                   if (TMath::Abs(diEle.eleEta_1_out.at(i)) < 1.4791) {
                       if (diEle.eleSigmaIEtaIEta_2012_1_out.at(i) > eleSigmaIEtaIEta_2012_EB)  continue;
                       if (diEle.eleHoverE_1_out.at(i) > eleHoverE_EB)                          continue;
                   }
                   else if (TMath::Abs(diEle.eleEta_1_out.at(i)) >= 1.4791 && TMath::Abs(diEle.eleEta_1_out.at(i)) < 2.4) {
                       if (diEle.eleSigmaIEtaIEta_2012_1_out.at(i) > eleSigmaIEtaIEta_2012_EE)  continue;
                       if (diEle.eleHoverE_1_out.at(i) > eleHoverE_EE)                          continue;
                   }
                   if (diEle.elePt_1_out.at(i) <= elePt)  continue;
                   // electron 2
                   if (TMath::Abs(diEle.eleEta_2_out.at(i)) < 1.4791) {
                       if (diEle.eleSigmaIEtaIEta_2012_2_out.at(i) > eleSigmaIEtaIEta_2012_EB)  continue;
                       if (diEle.eleHoverE_2_out.at(i) > eleHoverE_EB)                          continue;
                   }
                   else if (TMath::Abs(diEle.eleEta_2_out.at(i)) >= 1.4791 && TMath::Abs(diEle.eleEta_2_out.at(i)) < 2.4) {
                       if (diEle.eleSigmaIEtaIEta_2012_2_out.at(i) > eleSigmaIEtaIEta_2012_EE)  continue;
                       if (diEle.eleHoverE_2_out.at(i) > eleHoverE_EE)                          continue;
                   }
                   if (diEle.elePt_2_out.at(i) <= elePt)  continue;

                   /*
                   if (diEle.diElePt_out.at(i) > maxZPt)
                   {
                       maxZPt = diEle.diElePt_out.at(i);
                       zIdx = i;
                   }
                   */

                   if (TMath::Abs(diEle.diEleM_out.at(i) - zMassConst) < minDiffZMass)
                   {
                       minDiffZMass = TMath::Abs(diEle.diEleM_out.at(i) - zMassConst);
                       zIdx = i;
                   }
               }
           }

           // dimuon block
           if (doDiMuon > 0) {
               // skip if there are no muon pairs to study
               if(ggHi.nMu < cut_nMu)  continue;

               // construct dimuon pairs during zJet skim
               diMu.makeDiMuonPairs(ggHi);

               // Zmumu-jet block
               // find leading z from dimuon
               // double maxZPt = -1;
               double minDiffZMass = 9999;
               for(unsigned int i=0; i<(unsigned)diMu.diMuM_out.size(); ++i)
               {
                   if (smearZ > 0)
                   {
                       diMu.diMuPt_out.at(i) *= randSmearZ.Gaus(1, smearZ);
                   }

                   bool failedPtCut  = (diMu.diMuPt_out.at(i) < cutZPt) ;
                   bool failedEtaCut = (TMath::Abs(diMu.diMuEta_out.at(i)) > cutZEta) ;
                   bool failedMassWindow = (diMu.diMuM_out.at(i) < cutZMassMin || diMu.diMuM_out.at(i) > cutZMassMax);
                   // bool failedOppositeCh = (diMu.muCharge_1_out.at(i) == diMu.muCharge_2_out.at(i));

                   if (failedPtCut)          continue;
                   if (failedEtaCut)         continue;
                   if (failedMassWindow)     continue;
                   // if (failedOppositeCh)     continue;

                   // some extra and rather loose cuts
                   // muon 1
                   if (diMu.muChi2NDF_1_out.at(i) > muChi2NDF) continue;
                   if (TMath::Abs(diMu.muInnerD0_1_out.at(i)) > muInnerD0) continue;
                   if (TMath::Abs(diMu.muInnerDz_1_out.at(i)) > muInnerDz) continue;
                   if (diMu.muMuonHits_1_out.at(i) < muMuonHits) continue;
                   if (diMu.muStations_1_out.at(i) < muStations) continue;
                   if (diMu.muTrkLayers_1_out.at(i) < muTrkLayers) continue;
                   if (diMu.muPixelHits_1_out.at(i) < muPixelHits) continue;
                   if (diMu.muPt_1_out.at(i) <= muPt)  continue;

                   // muon 2
                   if (diMu.muChi2NDF_2_out.at(i) > muChi2NDF) continue;
                   if (TMath::Abs(diMu.muInnerD0_2_out.at(i)) > muInnerD0) continue;
                   if (TMath::Abs(diMu.muInnerDz_2_out.at(i)) > muInnerDz) continue;
                   if (diMu.muMuonHits_2_out.at(i) < muMuonHits) continue;
                   if (diMu.muStations_2_out.at(i) < muStations) continue;
                   if (diMu.muTrkLayers_2_out.at(i) < muTrkLayers) continue;
                   if (diMu.muPixelHits_2_out.at(i) < muPixelHits) continue;
                   if (diMu.muPt_2_out.at(i) <= muPt)  continue;

                   /*
                   if (diMu.diMuPt_out.at(i) > maxZPt)
                   {
                       maxZPt = diMu.diMuPt_out.at(i);
                       zIdx = i;
                   }
                   */

                   if (TMath::Abs(diMu.diMuM_out.at(i) - zMassConst) < minDiffZMass)
                   {
                       minDiffZMass = TMath::Abs(diMu.diMuM_out.at(i) - zMassConst);
                       zIdx = i;
                   }
               }
           }

           if (zIdx == -1) continue;
           entriesAnalyzed++;

           // jet corrections
           if (doCorrectionResidual > 0 && isHI) {
               for (int i=0; i < nJetCollections; ++i) {
                   for (int iHibin = 0; iHibin < nHiBins_residual; ++iHibin)
                   {
                       if (hiBins_residual.at(iHibin) <= hiBin)
                       {
                           correctorsJetResidual.at(i*nHiBins_residual + iHibin).correctPtsResidual(f1Residual[iHibin], jets.at(i));
                           break;
                       }
                   }
               }
           }
           if (isPP) {

               if (doCorrectionL2L3 > 0)
               {
                   for (int i=0; i<nJetCollections; ++i) {
                       correctorsL2L3.at(i).correctPtsL2L3(jets.at(i));
                   }
               }

               if (doCorrectionSmearing > 0) {
                   for (int i=0; i<nJetCollections; ++i) {
                       correctorsJetSmear.at(i).correctPtsSmearing(jets.at(i));
                   }
               }
               if (doCorrectionSmearingPhi > 0) {
                   for (int i=0; i<nJetCollections; ++i) {
                       correctorsJetSmear.at(i).correctPhisSmearing(jets.at(i));
                   }
               }
           }
           // apply JES after corrections
           if (energyScaleJet != 0 && energyScaleJet != 1)
           {
               for (int i=0; i<nJetCollections; ++i) {
                   correctorsJetJES.at(i).applyEnergyScale(jets.at(i), energyScaleJet);
               }
           }

           for (int i=0; i<nJetCollections; ++i) {
               // z-jet correlation
               // leading z Boson from dielectron is correlated to each jet in the event.
               if (doDiElectron > 0)  zjet.at(i).makeZeeJetPairs(diEle, jets.at(i), zIdx, true);

               // z-jet correlation
               // leading z Boson from dimuon is correlated to each jet in the event.
               if (doDiMuon > 0)  zjet.at(i).makeZmmJetPairs(diMu, jets.at(i), zIdx, true);
           }

           if(doMix > 0)
           {
               int centBin = hiBin / centBinWidth;
               int vzBin   = (vz+15) / vertexBinWidth;
               for (int k = 0; k < nJetCollections; ++k) {
                   jetsMBoutput.at(k).nref = 0;

                   zjetMB.at(k).clearZJetPairs(zIdx);
                   if (nMB[centBin][vzBin][k] >= nEventsToMix)
                   {
                       for (int i=0; i<nEventsToMix; ++i)
                       {
                           Long64_t entryMB = iterMB[centBin][vzBin][k] % nMB[centBin][vzBin][k];     // roll back to the beginning if out of range
                           treeJetMB[centBin][vzBin][k]->GetEntry(entryMB);

                           // jet corrections for MB events
                           if (doCorrectionResidual > 0 && isHI) {

                               for (int iHibin = 0; iHibin < nHiBins_residual; ++iHibin)
                               {
                                   if (hiBins_residual.at(iHibin) <= hiBin)
                                   {
                                       correctorsJetResidual.at(k*nHiBins_residual + iHibin).correctPtsResidual(f1Residual[iHibin], jetsMB.at(k));
                                       break;
                                   }
                               }
                           }
                           if (isPP) {
                               if (doCorrectionSmearing > 0) {
                                   correctorsJetSmear.at(k).correctPtsSmearing(jetsMB.at(k));
                               }
                               if (doCorrectionSmearingPhi > 0) {
                                   correctorsJetSmear.at(k).correctPhisSmearing(jetsMB.at(k));
                               }
                           }
                           // apply JES after corrections
                           if (energyScaleJet != 0 && energyScaleJet != 1)
                           {
                               correctorsJetJES.at(k).applyEnergyScale(jetsMB.at(k), energyScaleJet);
                           }

                           if (doDiElectron > 0) zjetMB.at(k).makeZeeJetPairsMB(diEle, jetsMB.at(k), zIdx, true);
                           if (doDiMuon > 0)     zjetMB.at(k).makeZmmJetPairsMB(diMu,  jetsMB.at(k), zIdx, true);

                           // write jets from minBiasJetSkimFile to outputFile
                           for(int j = 0; j < jetsMB.at(k).nref; ++j)
                           {
                               jetsMBoutput.at(k).rawpt[jetsMBoutput.at(k).nref] = jetsMB.at(k).rawpt[j];
                               jetsMBoutput.at(k).jtpt [jetsMBoutput.at(k).nref] = jetsMB.at(k).jtpt[j];
                               jetsMBoutput.at(k).jteta[jetsMBoutput.at(k).nref] = jetsMB.at(k).jteta[j];
                               jetsMBoutput.at(k).jty  [jetsMBoutput.at(k).nref] = jetsMB.at(k).jty[j];
                               jetsMBoutput.at(k).jtphi[jetsMBoutput.at(k).nref] = jetsMB.at(k).jtphi[j];
                               jetsMBoutput.at(k).jtpu [jetsMBoutput.at(k).nref] = jetsMB.at(k).jtpu[j];
                               jetsMBoutput.at(k).jtm  [jetsMBoutput.at(k).nref] = jetsMB.at(k).jtm[j];
                               // jet ID variables
                               jetsMBoutput.at(k).trackMax [jetsMBoutput.at(k).nref] = jetsMB.at(k).trackMax[j];
                               jetsMBoutput.at(k).trackSum [jetsMBoutput.at(k).nref] = jetsMB.at(k).trackSum[j];
                               jetsMBoutput.at(k).trackN [jetsMBoutput.at(k).nref] = jetsMB.at(k).trackN[j];
                               jetsMBoutput.at(k).chargedMax [jetsMBoutput.at(k).nref] = jetsMB.at(k).chargedMax[j];
                               jetsMBoutput.at(k).chargedSum [jetsMBoutput.at(k).nref] = jetsMB.at(k).chargedSum[j];
                               jetsMBoutput.at(k).chargedN [jetsMBoutput.at(k).nref] = jetsMB.at(k).chargedN[j];
                               jetsMBoutput.at(k).photonMax [jetsMBoutput.at(k).nref] = jetsMB.at(k).photonMax[j];
                               jetsMBoutput.at(k).photonSum [jetsMBoutput.at(k).nref] = jetsMB.at(k).photonSum[j];
                               jetsMBoutput.at(k).photonN [jetsMBoutput.at(k).nref] = jetsMB.at(k).photonN[j];
                               jetsMBoutput.at(k).neutralMax [jetsMBoutput.at(k).nref] = jetsMB.at(k).neutralMax[j];
                               jetsMBoutput.at(k).neutralSum [jetsMBoutput.at(k).nref] = jetsMB.at(k).neutralSum[j];
                               jetsMBoutput.at(k).neutralN [jetsMBoutput.at(k).nref] = jetsMB.at(k).neutralN[j];
                               jetsMBoutput.at(k).eMax [jetsMBoutput.at(k).nref] = jetsMB.at(k).eMax[j];
                               jetsMBoutput.at(k).eSum [jetsMBoutput.at(k).nref] = jetsMB.at(k).eSum[j];
                               jetsMBoutput.at(k).eN [jetsMBoutput.at(k).nref] = jetsMB.at(k).eN[j];
                               jetsMBoutput.at(k).muMax [jetsMBoutput.at(k).nref] = jetsMB.at(k).muMax[j];
                               jetsMBoutput.at(k).muSum [jetsMBoutput.at(k).nref] = jetsMB.at(k).muSum[j];
                               jetsMBoutput.at(k).muN [jetsMBoutput.at(k).nref] = jetsMB.at(k).muN[j];

                               jetsMBoutput.at(k).nref++;
                           }

                           // increase iterator
                           iterMB[centBin][vzBin][k]++;
                           if (iterMB[centBin][vzBin][k] == nMB[centBin][vzBin][k])  iterMB[centBin][vzBin][k] = 0;  // reset if necessary
                       }
                   }
                   else {
                       std::cout << "WARNING : the event lacks necessary number of MB events to mix." << std::endl;
                       std::cout << Form("{run, lumis, event, j_entry} = %d, %d, %llu, %lld", run, lumis, event, j_entry) << std::endl;
                       std::cout << Form("{hiBin, vz} = %d, %f", hiBin, vz) << std::endl;
                       std::cout << "centBin = "<<centBin<<", vzBin = "<<vzBin<<", jetCollection index = "<<k<<std::endl;
                       std::cout << "nMB[centBin][vzBin][jetCollection] = "<<nMB[centBin][vzBin][k]<<std::endl;
                   }
                   jetsMBoutput.at(k).b = -1;   // this branch is not an array.

                   zJetTreeMB[k]->Fill();
                   outputTreeJetMB[k]->Fill();
               }
           }

           outputTreeHLT->Fill();
           outputTreeggHiNtuplizer->Fill();
           for (int i = 0; i < nJetCollections; ++i) {
               outputTreeJet[i]->Fill();
           }
           outputTreeHiEvt->Fill();
           outputTreeSkim->Fill();
           
           if (doDiElectron > 0)  diElectronTree->Fill();
           if (doDiMuon > 0)      diMuonTree->Fill();
           for (int i = 0; i < nJetCollections; ++i) {
               zJetTree[i]->Fill();
           }
       }
       std::cout<<  "Loop ENDED : ggHiNtuplizer/EventTree" <<std::endl;
       std::cout << "entries            = " << entries << std::endl;
       std::cout << "duplicateEntries   = " << duplicateEntries << std::endl;
       std::cout << "entriesPassedEventSelection   = " << entriesPassedEventSelection << std::endl;
       std::cout << "entriesAnalyzed               = " << entriesAnalyzed << std::endl;
       std::cout << "outputTreeHLT->GetEntries()   = " << outputTreeHLT->GetEntries() << std::endl;
       std::cout << "outputTreeggHiNtuplizer->GetEntries()   = " << outputTreeggHiNtuplizer->GetEntries() << std::endl;
       for (int i = 0; i < nJetCollections; ++i) {
           std::cout << Form("outputTreeJet[%d]->GetEntries()   = ", i) << outputTreeJet[i]->GetEntries() << std::endl;
       }
       std::cout << "outputTreeSkim->GetEntries()  = " << outputTreeSkim->GetEntries() << std::endl;
       std::cout << "outputTreeHiEvt->GetEntries() = " << outputTreeHiEvt->GetEntries() << std::endl;
    
       if (doDiElectron > 0)  std::cout << "diElectronTree->GetEntries()  = " << diElectronTree->GetEntries() << std::endl;
       if (doDiMuon > 0)      std::cout << "diMuonTree->GetEntries()  = " << diMuonTree->GetEntries() << std::endl;
       for (int i = 0; i < nJetCollections; ++i) {
           std::cout << Form("zJetTree[%d]->GetEntries() = ", i) << zJetTree[i]->GetEntries() << std::endl;
       }

       if (doMix > 0)
       {
           for (int i = 0; i < nJetCollections; ++i) {
               std::cout << Form("zJetTreeMB[%d]->GetEntries() = ", i) << zJetTreeMB[i]->GetEntries() << std::endl;
               std::cout << Form("outputTreeJetMB[%d]->GetEntries() = ", i) << outputTreeJetMB[i]->GetEntries() << std::endl;
           }
       }
  
       configTree->Write("",TObject::kOverwrite);

       output->Write("",TObject::kOverwrite);
       output->Close();
       if (doMix > 0 && inputMB) inputMB->Close();
}

int main(int argc, char** argv)
{
    if (argc == 5) {
        zJetSkim(argv[1], argv[2], argv[3], argv[4]);
        return 0;
       }
    else if (argc == 4) {
        zJetSkim(argv[1], argv[2], argv[3]);
        return 0;
    }
    else if (argc == 3) {
        zJetSkim(argv[1], argv[2]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./zJetSkim.exe <configFile> <inputFile> <outputFile> (<minBiasJetSkimFile>)"
                << std::endl;
        return 1;
    }
}
