#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TRandom3.h>

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

#include "../../CorrelationTuple/EventMatcher.h"
#include "../../TreeHeaders/gammaJetTree.h"
#include "../../TreeHeaders/CutConfigurationTree.h"
#include "../../Plotting/commonUtility.h"
#include "../../Utilities/interface/CutConfigurationParser.h"
#include "../../Utilities/interface/InputConfigurationParser.h"
#include "../../Utilities/interface/HiForestInfoController.h"
#include "../../Corrections/jets/jetCorrector.h"
#include "../../Corrections/jets/L2L3/L2L3ResidualWFits.h"

const long MAXTREESIZE = 2000000000000; // set maximum tree size from 10 GB to 1862 GB, so that the code does not switch to a new file after 10 GB

void gammaJetSkim(const TString configFile, const TString inputFile, const TString outputFile = "gammaJetSkim.root", const TString minBiasJetSkimFile = "");

void gammaJetSkim(const TString configFile, const TString inputFile, const TString outputFile, const TString minBiasJetSkimFile)
{
  std::cout << "running gammaJetSkim()" << std::endl;
  std::cout << "configFile  = " << configFile.Data() << std::endl;
  std::cout << "inputFile   = " << inputFile.Data() << std::endl;
  std::cout << "outputFile  = " << outputFile.Data() << std::endl;
  std::cout << "minBiasJetSkimFile = " << minBiasJetSkimFile.Data() << std::endl;

  InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
  CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

  // input configuration
  if (!configInput.isValid) {
    std::cout << "Input Configuration invalid" << std::endl;
    return;
  }

  int collisionType = configInput.proc[INPUT::kSKIM].i[INPUT::k_collisionType];
  // verbose about input configuration
  std::cout << "Input Configuration :" << std::endl;
  std::cout << "collisionType = " << collisionType << std::endl;
  const char* collisionName =  getCollisionTypeName((COLL::TYPE)collisionType).c_str();
  std::cout << "collision = " << collisionName << std::endl;

  // cut configuration
  float cut_vz;
  int cut_pcollisionEventSelection;
  int cut_pPAprimaryVertexFilter;
  int cut_pBeamScrapingFilter;

  std::vector<std::string> jetCollections;
  float cutPhoEt;
  float cutPhoEta;
  float cutJetPt; // applied to any of the smeared values in the case of smearing.
  float cutJetEta;

  int doMix;
  int nMaxEvents_minBiasMixing;
  int nCentralityBins;
  int nVertexBins;
  int nEventsToMix;
  int doEventWeight;
  bool doCorrectionSmearing;
  int nSmear;
  int doCorrectionL2L3;
  std::vector<float> mcFileWeights;

  if (!configCuts.isValid) {
    std::cout << "Invalid Configuration File" << std::endl;
    return;
  }
  cut_vz = configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].f[CUTS::EVT::k_vz];
  cut_pcollisionEventSelection = configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].i[CUTS::EVT::k_pcollisionEventSelection];
  cut_pPAprimaryVertexFilter = configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].i[CUTS::EVT::k_pPAprimaryVertexFilter];
  cut_pBeamScrapingFilter = configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].i[CUTS::EVT::k_pBeamScrapingFilter];

  jetCollections = ConfigurationParser::ParseList(configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].s[CUTS::JET::k_jetCollection]);

  cutPhoEt = configCuts.proc[CUTS::kSKIM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_et];
  cutPhoEta = configCuts.proc[CUTS::kSKIM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_eta];

  cutJetPt = configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].f[CUTS::JET::k_pt];
  cutJetEta = configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].f[CUTS::JET::k_eta];

  doMix = configCuts.proc[CUTS::kSKIM].obj[CUTS::kGAMMAJET].i[CUTS::GJT::k_doMix];
  nMaxEvents_minBiasMixing = configCuts.proc[CUTS::kSKIM].obj[CUTS::kGAMMAJET].i[CUTS::GJT::k_nMaxEvents_minBiasMixing];
  nCentralityBins = configCuts.proc[CUTS::kSKIM].obj[CUTS::kGAMMAJET].i[CUTS::GJT::k_nCentralityBins];
  nVertexBins = configCuts.proc[CUTS::kSKIM].obj[CUTS::kGAMMAJET].i[CUTS::GJT::k_nVertexBins];
  nEventsToMix = configCuts.proc[CUTS::kSKIM].obj[CUTS::kGAMMAJET].i[CUTS::GJT::k_nEventsToMix];
  doCorrectionSmearing = configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].i[CUTS::JET::k_doCorrectionSmearing];
  nSmear = configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].i[CUTS::JET::k_nSmear];
  doEventWeight = configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].i[CUTS::EVT::k_doEventWeight];
  mcFileWeights = ConfigurationParser::ParseListFloat(configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].s[CUTS::EVT::k_eventWeight]);
  doCorrectionL2L3 = configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].i[CUTS::JET::k_doCorrectionL2L3];

  for (unsigned i = 0; i < mcFileWeights.size(); ++i)
    std::cout << mcFileWeights[i] << " ";
  std::cout << std::endl;

  int nJetCollections = jetCollections.size();

  if (minBiasJetSkimFile.EqualTo("")) {
    std::cout << "no minBiasJetSkimFile was provided." << std::endl;
    std::cout << "Minimum Bias event mixing will be skipped" << std::endl;
    doMix = 0;
  }

  bool isMC = collisionIsMC((COLL::TYPE)collisionType);
  bool isHI = collisionIsHI((COLL::TYPE)collisionType);

  // verbose about cut configuration
  std::cout << "Cut Configuration :" << std::endl;
  std::cout << "cut_vz = " << cut_vz << std::endl;
  if (isHI) {
    std::cout << "cut_pcollisionEventSelection = " << cut_pcollisionEventSelection << std::endl;
  } else {   // PP
    std::cout << "cut_pPAprimaryVertexFilter = " << cut_pPAprimaryVertexFilter << std::endl;
    std::cout << "cut_pBeamScrapingFilter = " << cut_pBeamScrapingFilter << std::endl;
  }

  std::cout << "nJetCollections = " << nJetCollections << std::endl;
  for (int i=0; i<nJetCollections; ++i) {
    std::cout << Form("jetCollections[%d] = %s", i, jetCollections[i].c_str()) << std::endl;
  }

  std::cout << "cutPhoEt  = " << cutPhoEt << std::endl;
  std::cout << "cutPhoEta = " << cutPhoEta << std::endl;

  std::cout << "doMix     = " << doMix << std::endl;
  if (doMix > 0) {
    std::cout << "nMaxEvents_minBiasMixing = " << nMaxEvents_minBiasMixing << std::endl;
    std::cout << "nCentralityBins          = " << nCentralityBins << std::endl;
    std::cout << "nVertexBins              = " << nVertexBins << std::endl;
    std::cout << "nEventsToMix             = " << nEventsToMix << std::endl;
  }

  TFile* weightsFile = TFile::Open(configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].s[CUTS::EVT::k_weights_file].c_str(), "READ");
  TH2D* h_weights_PbPb=0;
  TH1D* h_weights_pp=0;
  if (isMC) {
    if (isHI) {
      h_weights_PbPb = (TH2D*)weightsFile->Get("h_weights");
    } else {
      h_weights_pp = (TH1D*)weightsFile->Get("h_weights");
    }
  }

  std::vector<L2L3ResidualWFits> correctorsL2L3(nJetCollections);
  if (doCorrectionL2L3 > 0) {
    for (int i = 0; i < nJetCollections; ++i) {
      correctorsL2L3[i].setL2L3Residual(3, 3, false);
    }
  }

  // mixed-event block
  int centBinWidth = 0;
  int vertexBinWidth = 0;
  std::vector<Jets> jetsMB(nJetCollections);     // object to read jet trees from MB events
  TTree* treeJetMB[nCentralityBins][nVertexBins][nJetCollections];
  Long64_t nMB[nCentralityBins][nVertexBins][nJetCollections];
  Long64_t iterMB[nCentralityBins][nVertexBins][nJetCollections];   // index of the tree where the mixing starts
  TFile* inputMB = 0;
  if (doMix > 0) {
    centBinWidth = 200/nCentralityBins;  // number of "hiBin"s that a centrality bin covers
    vertexBinWidth = 30/nVertexBins;     // number of "vz"s    that a vertex     bin covers
    // accepted vz range is -15 to 15.

    inputMB = TFile::Open(minBiasJetSkimFile, "READ");

    TRandom3 rand(12345);    // random number seed should be fixed or reproducible
    std::cout << "Tree initialization for MinBias mixing" << std::endl;
    std::cout << Form("treeJetMB[%d][%d][%d] is being read", nCentralityBins, nVertexBins, nJetCollections) << std::endl;
    for (int i=0; i<nCentralityBins; ++i) {
      for (int j=0; j<nVertexBins; ++j) {
        for (int k=0; k<nJetCollections; ++k) {
          // in minBiasJetSkimFile, name of a jet tree starts with jetCollection.
          std::string jetCollection = jetCollections[k].c_str();
          treeJetMB[i][j][k] = (TTree*)inputMB->Get(Form("%s_centBin%d_vzBin%d", jetCollection.c_str(), i, j));
          if (!treeJetMB[i][j][k]) {
            std::cout << "jetCollection = " << jetCollection.c_str() << " is not found in minBias jet skim file" << std::endl;
            std::cout << "exiting" << std::endl;
            return;
          }
          nMB[i][j][k] = treeJetMB[i][j][k]->GetEntries();

          jetsMB[k].setupTreeForReading(treeJetMB[i][j][k]);    // all MB jet trees point to jetsMB
          int primeSeed = 0;
          if (nMB[i][j][k] != 0) primeSeed = rand.Integer(nMB[i][j][k]); // Integer(imax) Returns a random integer on [0, imax-1].
          iterMB[i][j][k] = primeSeed;

          if (nMB[i][j][k] < nEventsToMix) {
            std::cout << "centBin = " << i << ", vzBin = " << j << ", jetCollection = " << jetCollection.c_str() << std::endl;
            std::cout << "nMB[centBin][vzBin][jetCollection] = " << nMB[i][j][k] << std::endl;
            std::cout << "nEventsToMix = " << nEventsToMix << std::endl;
            std::cout << "number of MB events in that bin is not enough for mixing" << std::endl;
          }
        }
      }
    }
  }

  //smearing set up block
  jetCorrector correctorsJetSmear[7][nJetCollections];
  if (doCorrectionSmearing) {
    TRandom3 randSmearing(12345);    // random number seed should be fixed or reproducible

    // pp resolution
    std::vector<double> CSN_PP = {0.07764, 0.9648, -0.0003191};
    std::vector<double> CSN_phi_PP = {7.72/100000000, 0.1222, 0.5818};

    // smear 0-30 %
    std::vector<double> CSN_HI_cent0030 = {0.08624, 1.129, 7.853};
    std::vector<double> CSN_phi_HI_cent0030 = {-1.303/1000000, 0.1651, 1.864};
    // smear 30-100 %
    std::vector<double> CSN_HI_cent30100 = {0.0623, 1.059, 4.245};
    std::vector<double> CSN_phi_HI_cent30100 = {-2.013/100000000, 0.1646, 1.04};

    for (int j = 0; j < 7; ++j) {
      for (int i = 0; i < nJetCollections; ++i) {
        correctorsJetSmear[j][i].rand = randSmearing;
        correctorsJetSmear[j][i].CSN_PP = CSN_PP;
        correctorsJetSmear[j][i].CSN_phi_PP = CSN_phi_PP;
        correctorsJetSmear[j][i].smearingBranchIndex = j+1;

        switch (j) {
        case 0: //0-30
        case 2: //0-10
        case 3: //10-30
          correctorsJetSmear[j][i].CSN_HI = CSN_HI_cent0030;
          correctorsJetSmear[j][i].CSN_phi_HI = CSN_phi_HI_cent0030;
          break;
        case 1: //30-100
        case 4: //30-50
        case 5: //50-100
        case 6: //sys
          correctorsJetSmear[j][i].CSN_HI = CSN_HI_cent30100;
          correctorsJetSmear[j][i].CSN_phi_HI = CSN_phi_HI_cent30100;
          break;
        }
      }
    }
  }

  TFile* output = TFile::Open(outputFile, "RECREATE");
  TTree* configTree = setupConfigurationTreeForWriting(configCuts);

  // output tree variables
  TTree *outputTreeHLT = 0, *outputTreeggHiNtuplizer = 0, *outputTreeJet[nJetCollections] = {0},
    *outputTreeHiEvt = 0, *outputTreeSkim = 0;

  TTree* gammaJetTree[nJetCollections][8];
  GammaJet gammajet[nJetCollections][8];

  std::vector<Jets> outputJets(nJetCollections);
  for (int i=0; i<nJetCollections; ++i) {
    std::string treeJetName = jetCollections[i].c_str();
    std::string treeJetTitle = jetCollections[i].c_str();
    // do not lose the current title
    outputTreeJet[i] = new TTree(treeJetName.c_str(), treeJetTitle.c_str());
    outputTreeJet[i]->SetMaxTreeSize(MAXTREESIZE);
    outputJets[i].setupTreeForWriting(outputTreeJet[i]);
  }

  for (int i=0; i<nJetCollections; ++i) {
    for (int j=0; j<8; ++j) {
      // pick a unique, but also not complicated name for gammaJet Trees
      // jet collection names which are complicated will be put into tree title
      std::string treegammaJetName, treegammaJetTitle;
      if (j==0) {
        treegammaJetName = Form("gamma_%s", jetCollections[i].c_str());
      } else {
        treegammaJetName = Form("gamma_%s_smearBin%i", jetCollections[i].c_str(), j-1);
      }
      treegammaJetTitle = Form("%s : leading photon-jet correlations", jetCollections[i].c_str());

      gammaJetTree[i][j] = new TTree(treegammaJetName.c_str(), treegammaJetTitle.c_str());
      gammaJetTree[i][j]->SetMaxTreeSize(MAXTREESIZE);

      gammajet[i][j].resetAwayRange();
      gammajet[i][j].resetConeRange();
      gammajet[i][j].branchGammaJetTree(gammaJetTree[i][j]);
      if (!doCorrectionSmearing) break;
    }
  }

  // mixed-event block
  std::vector<Jets> jetsMBoutput(nJetCollections);     // object to write jet trees from MB events
  std::vector<GammaJet> gammajetMB(nJetCollections);
  TTree* outputTreeJetMB[nJetCollections];
  TTree* gammaJetTreeMB[nJetCollections];
  if (doMix > 0) {
    for (int i=0; i<nJetCollections; ++i) {
      // jetMB trees
      // pick a unique, but also not complicated name for jetMB Trees
      // jet collection names which are complicated will be put into tree title
      std::string treeJetMBName = Form("%sMB", jetCollections[i].c_str());
      std::string treeJetMBTitle = Form("%s : jets from MB events", jetCollections[i].c_str());
      outputTreeJetMB[i] = new TTree(treeJetMBName.c_str(), treeJetMBTitle.c_str());

      outputTreeJetMB[i]->SetMaxTreeSize(MAXTREESIZE);
      jetsMBoutput[i].setupTreeForWritingMB(outputTreeJetMB[i], true, false);

      // gammaJetMB trees
      // pick a unique, but also not complicated name for gammaJetMB Trees
      // jet collection names which are complicated will be put into tree title
      std::string treegammaJetMBName = Form("gamma_%sMB", jetCollections[i].c_str());
      std::string treegammaJetMBTitle = Form("%s : leading photon-jet correlations", jetCollections[i].c_str());
      gammaJetTreeMB[i] = new TTree(treegammaJetMBName.c_str(), treegammaJetMBTitle.c_str());

      gammaJetTreeMB[i]->SetMaxTreeSize(MAXTREESIZE);
      gammajetMB[i].resetAwayRange();
      gammajetMB[i].resetConeRange();
      gammajetMB[i].branchGammaJetTree(gammaJetTreeMB[i]);
    }
  }

  EventMatcher* em = new EventMatcher();
  Long64_t duplicateEntries = 0;
  Long64_t Tentries = 0;
  Long64_t entriesPassedEventSelection = 0;
  Long64_t entriesAnalyzed = 0;

  std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.Data());

  for (unsigned i = 0; i < inputFiles.size(); ++i)
    std::cout << inputFiles[i] << " ";
  std::cout << std::endl;

  std::cout << "input ROOT files : num = " << inputFiles.size() << std::endl;
  std::cout << "#####" << std::endl;
  for (std::vector<std::string>::iterator it = inputFiles.begin(); it != inputFiles.end(); ++it) {
    std::cout << (*it).c_str() << std::endl;

    TFile *inFile = TFile::Open((*it).c_str());
    inFile->cd();
    TTree* treeHLT   = (TTree*) inFile->Get("hltanalysis/HltTree");
    TTree* treeggHiNtuplizer = 0;
    if (isHI) {
      treeggHiNtuplizer = (TTree*)inFile->Get("ggHiNtuplizer/EventTree");
    } else {
      treeggHiNtuplizer = (TTree*)inFile->Get("ggHiNtuplizerGED/EventTree");
    }
    TTree* treeJet[nJetCollections];
    for (int i=0; i<nJetCollections; ++i) {
      treeJet[i] = (TTree*) inFile->Get(Form("%s/t", jetCollections[i].c_str()));
    }
    TTree* treeHiEvt = (TTree*)inFile->Get("hiEvtAnalyzer/HiTree");
    TTree* treeSkim  = (TTree*)inFile->Get("skimanalysis/HltTree");

    treeHLT->SetBranchStatus("*", 0);     // disable all branches
    treeHLT->SetBranchStatus("HLT_HISinglePhoton*_Eta*_v*", 1);     // enable photon branches
    treeHLT->SetBranchStatus("HLT_HIDoublePhoton*_Eta*_v*", 1);     // enable photon branches

    // objects for gamma-jet correlations
    ggHiNtuplizer ggHi;
    ggHi.setupTreeForReading(treeggHiNtuplizer);    // treeggHiNtuplizer is input
    std::vector<Jets> jets(nJetCollections);
    for (int i=0; i<nJetCollections; ++i) {
      jets[i].setupTreeForReading(treeJet[i]);   // treeJet is input
    }

    for (int i=0; i<nJetCollections; ++i) {
      treeJet[i]->SetBranchStatus("*", 0);        // disable all branches
      treeJet[i]->SetBranchStatus("nref", 1);     // enable jet branches
      treeJet[i]->SetBranchStatus("rawpt", 1);    // enable jet branches
      treeJet[i]->SetBranchStatus("jtpt", 1);     // enable jet branches
      treeJet[i]->SetBranchStatus("jteta", 1);     // enable jet branches
      treeJet[i]->SetBranchStatus("jtphi", 1);     // enable jet branches
      treeJet[i]->SetBranchStatus("track*", 1);
      treeJet[i]->SetBranchStatus("charged*", 1);
      treeJet[i]->SetBranchStatus("photon*", 1);
      treeJet[i]->SetBranchStatus("neutral*", 1);
      treeJet[i]->SetBranchStatus("eMax*", 1);
      treeJet[i]->SetBranchStatus("eSum*", 1);
      treeJet[i]->SetBranchStatus("eN*", 1);
      treeJet[i]->SetBranchStatus("muMax*", 1);
      treeJet[i]->SetBranchStatus("muSum*", 1);
      treeJet[i]->SetBranchStatus("muN*", 1);
      if (isMC) {
        treeJet[i]->SetBranchStatus("matchedPt", 1);
        treeJet[i]->SetBranchStatus("matchedR", 1);
        treeJet[i]->SetBranchStatus("beamId1", 1);
        treeJet[i]->SetBranchStatus("beamId2", 1);
        treeJet[i]->SetBranchStatus("pthat", 1);
        treeJet[i]->SetBranchStatus("ngen", 1);
        treeJet[i]->SetBranchStatus("gen*", 1);
        treeJet[i]->SetBranchStatus("signalChargedSum", 1);
        treeJet[i]->SetBranchStatus("signalHardSum", 1);
        treeJet[i]->SetBranchStatus("subid", 1);
        treeJet[i]->SetBranchStatus("smpt", 1);
        treeJet[i]->SetBranchStatus("fr01Chg", 1);
        treeJet[i]->SetBranchStatus("fr01EM", 1);
        treeJet[i]->SetBranchStatus("fr01", 1);
      }
    }

    // specify explicitly which branches to store, do not use wildcard
    treeHiEvt->SetBranchStatus("*", 0);
    treeHiEvt->SetBranchStatus("run", 1);
    treeHiEvt->SetBranchStatus("evt", 1);
    treeHiEvt->SetBranchStatus("lumi", 1);
    treeHiEvt->SetBranchStatus("vz", 1);
    treeHiEvt->SetBranchStatus("hiBin", 1);
    treeHiEvt->SetBranchStatus("hiHF", 1);
    treeHiEvt->SetBranchStatus("hiHFplus", 1);
    treeHiEvt->SetBranchStatus("hiHFminus", 1);
    treeHiEvt->SetBranchStatus("hiHFplusEta4", 1);
    treeHiEvt->SetBranchStatus("hiHFminusEta4", 1);
    treeHiEvt->SetBranchStatus("hiNevtPlane", 1);
    if (isMC) {
      treeHiEvt->SetBranchStatus("Npart", 1);
      treeHiEvt->SetBranchStatus("Ncoll", 1);
      treeHiEvt->SetBranchStatus("Nhard", 1);
      treeHiEvt->SetBranchStatus("ProcessID", 1);
      treeHiEvt->SetBranchStatus("pthat", 1);
      //treeHiEvt->SetBranchStatus("weight", 1); // set a new weight, don't use the old one
      treeHiEvt->SetBranchStatus("alphaQCD", 1);
      treeHiEvt->SetBranchStatus("alphaQED", 1);
      treeHiEvt->SetBranchStatus("qScale", 1);
    }

    float vz;
    Int_t hiBin;
    UInt_t run, lumis;
    ULong64_t event;

    treeHiEvt->SetBranchAddress("vz", &vz);
    treeHiEvt->SetBranchAddress("hiBin", &hiBin);
    treeHiEvt->SetBranchAddress("run", &run);
    treeHiEvt->SetBranchAddress("evt", &event);
    treeHiEvt->SetBranchAddress("lumi", &lumis);

    // specify explicitly which branches to store, do not use wildcard
    treeSkim->SetBranchStatus("*", 0);

    Int_t pcollisionEventSelection;  // this filter is used for HI.
    if (isHI) {
      treeSkim->SetBranchStatus("pcollisionEventSelection", 1);
      if (treeSkim->GetBranch("pcollisionEventSelection")) {
        treeSkim->SetBranchAddress("pcollisionEventSelection", &pcollisionEventSelection);
      } else {   // overwrite to default
        pcollisionEventSelection = 1;
        std::cout << "could not get branch : pcollisionEventSelection" << std::endl;
        std::cout << "set to default value : pcollisionEventSelection = " << pcollisionEventSelection << std::endl;
      }
    } else {
      pcollisionEventSelection = 0;    // default value if the collision is not HI, will not be used anyway.
    }
    Int_t pPAprimaryVertexFilter;    // this filter is used for PP.
    if (!isHI) {
      treeSkim->SetBranchStatus("pPAprimaryVertexFilter", 1);
      if (treeSkim->GetBranch("pPAprimaryVertexFilter")) {
        treeSkim->SetBranchAddress("pPAprimaryVertexFilter", &pPAprimaryVertexFilter);
      } else {   // overwrite to default
        pPAprimaryVertexFilter = 1;
        std::cout << "could not get branch : pPAprimaryVertexFilter" << std::endl;
        std::cout << "set to default value : pPAprimaryVertexFilter = " << pPAprimaryVertexFilter << std::endl;
      }
    } else {
      pPAprimaryVertexFilter = 0;      // default value if the collision is not PP, will not be used anyway.
    }
    Int_t pBeamScrapingFilter;   // this filter is used for PP.
    if (!isHI) {
      treeSkim->SetBranchStatus("pBeamScrapingFilter", 1);
      if (treeSkim->GetBranch("pBeamScrapingFilter")) {
        treeSkim->SetBranchAddress("pBeamScrapingFilter", &pBeamScrapingFilter);
      } else {   // overwrite to default
        pBeamScrapingFilter = 1;
        std::cout << "could not get branch : pBeamScrapingFilter" << std::endl;
        std::cout << "set to default value : pBeamScrapingFilter = " << pBeamScrapingFilter << std::endl;
      }
    } else {
      pBeamScrapingFilter = 0;     // default value if the collision is not PP, will not be used anyway.
    }

    float eventWeight;

    if (it == inputFiles.begin()) {
      output->cd();
      outputTreeHLT = treeHLT->CloneTree(0);
      outputTreeHLT->SetName("hltTree");
      outputTreeHLT->SetTitle("subbranches of hltanalysis/HltTree");
      outputTreeggHiNtuplizer = treeggHiNtuplizer->CloneTree(0);
      outputTreeHiEvt = treeHiEvt->CloneTree(0);
      outputTreeHiEvt->SetName("HiEvt");
      outputTreeHiEvt->SetTitle("subbranches of hiEvtAnalyzer/HiTree");
      if (doEventWeight) {
        outputTreeHiEvt->Branch("weight", &eventWeight, "weight/F");
      }
      outputTreeSkim = treeSkim->CloneTree(0);
      outputTreeSkim->SetName("skim");
      outputTreeSkim->SetTitle("subbranches of skimanalysis/HltTree");

      outputTreeHLT->SetMaxTreeSize(MAXTREESIZE);
      outputTreeggHiNtuplizer->SetMaxTreeSize(MAXTREESIZE);
      outputTreeHiEvt->SetMaxTreeSize(MAXTREESIZE);
      outputTreeSkim->SetMaxTreeSize(MAXTREESIZE);
      inFile->cd();
    } else {
      output->cd();
      treeHLT->CopyAddresses(outputTreeHLT);
      treeggHiNtuplizer->CopyAddresses(outputTreeggHiNtuplizer);
      treeHiEvt->CopyAddresses(outputTreeHiEvt);
      treeSkim->CopyAddresses(outputTreeSkim);
      inFile->cd();
    }

    Long64_t entries = treeggHiNtuplizer->GetEntries();
    Tentries += entries;
    std::cout << "entries = " << entries << std::endl;
    std::cout << "Loop : ggHiNtuplizer/EventTree" << std::endl;
    for (Long64_t j_entry=0; j_entry<entries; ++j_entry) {
      if (j_entry % 2000 == 0) {
        std::cout << "current entry = " << j_entry << " out of " << entries << " : " << std::setprecision(2) << (double)j_entry/entries*100 << " %" << std::endl;
      }

      treeHLT->GetEntry(j_entry);
      treeggHiNtuplizer->GetEntry(j_entry);
      for (int i=0; i<nJetCollections; ++i) {
        treeJet[i]->GetEntry(j_entry);
      }
      treeSkim->GetEntry(j_entry);
      treeHiEvt->GetEntry(j_entry);
      if (doEventWeight) {
        eventWeight = mcFileWeights[it - inputFiles.begin()];
        if (isHI) {
          eventWeight *= h_weights_PbPb->GetBinContent(h_weights_PbPb->FindBin(hiBin, vz));
        } else {
          eventWeight *= h_weights_pp->GetBinContent(h_weights_pp->FindBin(vz));
        }
      }

      bool eventAdded = em->addEvent(run, lumis, event, j_entry);
      if (!eventAdded) { // this event is duplicate, skip this one.
        duplicateEntries++;
        continue;
      }

      // event selection
      if (!(TMath::Abs(vz) < cut_vz))  continue;
      if (isHI) {
        if ((pcollisionEventSelection < cut_pcollisionEventSelection))  continue;
      } else {
        if (pPAprimaryVertexFilter < cut_pPAprimaryVertexFilter || pBeamScrapingFilter < cut_pBeamScrapingFilter)  continue;
      }
      entriesPassedEventSelection++;

      // photon-jet block
      // find leading photon
      int phoIdx = -1;     // index of the leading photon
      double maxPhoEt = -1;
      for (int i=0; i<ggHi.nPho; ++i) {
        bool failedNoiseCut;
        failedNoiseCut = (((*ggHi.phoE3x3)[phoIdx]/(*ggHi.phoE5x5)[phoIdx] > 2/3-0.03 &&
                           (*ggHi.phoE3x3)[phoIdx]/(*ggHi.phoE5x5)[phoIdx] < 2/3+0.03) &&
                          ((*ggHi.phoE1x5)[phoIdx]/(*ggHi.phoE5x5)[phoIdx] > 1/3-0.03 &&
                           (*ggHi.phoE1x5)[phoIdx]/(*ggHi.phoE5x5)[phoIdx] < 1/3+0.03) &&
                          ((*ggHi.phoE2x5)[phoIdx]/(*ggHi.phoE5x5)[phoIdx] > 2/3-0.03 &&
                           (*ggHi.phoE2x5)[phoIdx]/(*ggHi.phoE5x5)[phoIdx] < 2/3+0.03));
        if (failedNoiseCut) {
          phoIdx = -1;
          break;
        }
        bool failedEtCut = (ggHi.phoEt->at(i) < cutPhoEt) ;
        if (failedEtCut)
          continue;
        bool failedEtaCut = (TMath::Abs(ggHi.phoEta->at(i)) > cutPhoEta) ;
        if (failedEtaCut)
          continue;
        bool failedSpikeRejection;
        failedSpikeRejection = (ggHi.phoSigmaIEtaIEta->at(i) < 0.002 ||
                                ggHi.pho_swissCrx->at(i)     > 0.9   ||
                                TMath::Abs(ggHi.pho_seedTime->at(i)) > 3);
        if (failedSpikeRejection)
          continue;
        bool failedHoverE = (ggHi.phoHoverE->at(i) > 0.2);      // <0.1 cut is applied after corrections
        if (failedHoverE)
          continue;

        if (ggHi.phoEt->at(i) > maxPhoEt) {
          maxPhoEt = ggHi.phoEt->at(i);
          phoIdx = i;
        }
      }
      if (phoIdx == -1)
        continue;
      bool failedLooseIso = ((ggHi.pho_ecalClusterIsoR4->at(phoIdx) + ggHi.pho_hcalRechitIsoR4->at(phoIdx) + ggHi.pho_trackIsoR4PtCut20->at(phoIdx)) > 30.0);
      if (failedLooseIso)
        continue;

      entriesAnalyzed++;

      for (int i=0; i<nJetCollections; ++i) {
        outputJets[i].nref = 0;
        if (doCorrectionL2L3 > 0) {
          correctorsL2L3[i].correctPtsL2L3(jets[i]);
        }

        if (doCorrectionSmearing) {
          jets[i].replicateJets(nSmear);
          for (int j=0; j<=7; ++j) {
            if (j != 0) {
              correctorsJetSmear[j-1][i].applyPtsSmearing(jets[i]);
              correctorsJetSmear[j-1][i].applyPhisSmearing(jets[i]);
            }
          }
        }

        // scrape some jets.
        for (int j = 0; j < jets[i].nref; ++j) {
          if (TMath::Abs(jets[i].jteta[j]) > cutJetEta) continue;
          if (doCorrectionSmearing) {
            if ((jets[i].jtpt[j] < cutJetPt) &&
               (jets[i].jtpt_smeared_0_30[j] < cutJetPt) &&
               (jets[i].jtpt_smeared_30_100[j] < cutJetPt) &&
               (jets[i].jtpt_smeared_0_10[j] < cutJetPt) &&
               (jets[i].jtpt_smeared_10_30[j] < cutJetPt) &&
               (jets[i].jtpt_smeared_30_50[j] < cutJetPt) &&
               (jets[i].jtpt_smeared_50_100[j] < cutJetPt) &&
               (jets[i].jtpt_smeared_sys[j] < cutJetPt)) continue;
          } else {
            if ((jets[i].jtpt[j] < cutJetPt)) continue;
          }

          outputJets[i].rawpt[outputJets[i].nref] = jets[i].rawpt[j];
          outputJets[i].jtpt[outputJets[i].nref] = jets[i].jtpt[j];
          outputJets[i].jteta[outputJets[i].nref] = jets[i].jteta[j];
          outputJets[i].jty[outputJets[i].nref] = jets[i].jty[j];
          outputJets[i].jtphi[outputJets[i].nref] = jets[i].jtphi[j];
          outputJets[i].jtm[outputJets[i].nref] = jets[i].jtm[j];
          outputJets[i].trackMax[outputJets[i].nref] = jets[i].trackMax[j];
          outputJets[i].trackSum[outputJets[i].nref] = jets[i].trackSum[j];
          outputJets[i].trackN[outputJets[i].nref] = jets[i].trackN[j];
          outputJets[i].trackHardSum[outputJets[i].nref] = jets[i].trackHardSum[j];
          outputJets[i].trackHardN[outputJets[i].nref] = jets[i].trackHardN[j];
          outputJets[i].chargedMax[outputJets[i].nref] = jets[i].chargedMax[j];
          outputJets[i].chargedN[outputJets[i].nref] = jets[i].chargedN[j];
          outputJets[i].chargedSum[outputJets[i].nref] = jets[i].chargedSum[j];
          outputJets[i].chargedHardSum[outputJets[i].nref] = jets[i].chargedHardSum[j];
          outputJets[i].chargedHardN[outputJets[i].nref] = jets[i].chargedHardN[j];
          outputJets[i].photonMax[outputJets[i].nref] = jets[i].photonMax[j];
          outputJets[i].photonSum[outputJets[i].nref] = jets[i].photonSum[j];
          outputJets[i].photonN[outputJets[i].nref] = jets[i].photonN[j];
          outputJets[i].photonHardSum[outputJets[i].nref] = jets[i].photonHardSum[j];
          outputJets[i].photonHardN[outputJets[i].nref] = jets[i].photonHardN[j];
          outputJets[i].neutralMax[outputJets[i].nref] = jets[i].neutralMax[j];
          outputJets[i].neutralSum[outputJets[i].nref] = jets[i].neutralSum[j];
          outputJets[i].neutralN[outputJets[i].nref] = jets[i].neutralN[j];
          outputJets[i].eMax[outputJets[i].nref] = jets[i].eMax[j];
          outputJets[i].eSum[outputJets[i].nref] = jets[i].eSum[j];
          outputJets[i].eN[outputJets[i].nref] = jets[i].eN[j];
          outputJets[i].muMax[outputJets[i].nref] = jets[i].muMax[j];
          outputJets[i].muSum[outputJets[i].nref] = jets[i].muSum[j];
          outputJets[i].muN[outputJets[i].nref] = jets[i].muN[j];

          if (isMC) {
            outputJets[i].genChargedSum[outputJets[i].nref] = jets[i].genChargedSum[j];
            outputJets[i].genHardSum[outputJets[i].nref] = jets[i].genHardSum[j];
            outputJets[i].signalChargedSum[outputJets[i].nref] = jets[i].signalChargedSum[j];
            outputJets[i].signalHardSum[outputJets[i].nref] = jets[i].signalHardSum[j];
            outputJets[i].subid[outputJets[i].nref] = jets[i].subid[j];
            // don't include the full gen jet tree (might regret this)
          }

          if (doCorrectionSmearing) {
            outputJets[i].jtpt_smeared_0_30[outputJets[i].nref] = jets[i].jtpt_smeared_0_30[j];
            outputJets[i].jtpt_smeared_30_100[outputJets[i].nref] = jets[i].jtpt_smeared_30_100[j];
            outputJets[i].jtpt_smeared_0_10[outputJets[i].nref] = jets[i].jtpt_smeared_0_10[j];
            outputJets[i].jtpt_smeared_10_30[outputJets[i].nref] = jets[i].jtpt_smeared_10_30[j];
            outputJets[i].jtpt_smeared_30_50[outputJets[i].nref] = jets[i].jtpt_smeared_30_50[j];
            outputJets[i].jtpt_smeared_50_100[outputJets[i].nref] = jets[i].jtpt_smeared_50_100[j];
            outputJets[i].jtpt_smeared_sys[outputJets[i].nref] = jets[i].jtpt_smeared_sys[j];

            outputJets[i].jtphi_smeared_0_30[outputJets[i].nref] = jets[i].jtphi_smeared_0_30[j];
            outputJets[i].jtphi_smeared_30_100[outputJets[i].nref] = jets[i].jtphi_smeared_30_100[j];
            outputJets[i].jtphi_smeared_0_10[outputJets[i].nref] = jets[i].jtphi_smeared_0_10[j];
            outputJets[i].jtphi_smeared_10_30[outputJets[i].nref] = jets[i].jtphi_smeared_10_30[j];
            outputJets[i].jtphi_smeared_30_50[outputJets[i].nref] = jets[i].jtphi_smeared_30_50[j];
            outputJets[i].jtphi_smeared_50_100[outputJets[i].nref] = jets[i].jtphi_smeared_50_100[j];
            outputJets[i].jtphi_smeared_sys[outputJets[i].nref] = jets[i].jtphi_smeared_sys[j];
          }
          outputJets[i].nref++;
        }
        for (int j =0; j<=7; ++j) {
          gammajet[i][j].makeGammaJetPairs(ggHi, outputJets[i], phoIdx, j);
          if (!doCorrectionSmearing) break;
        }
      }

      if (doMix > 0) {
        int centBin = hiBin / centBinWidth;
        int vzBin   = (vz+15) / vertexBinWidth;
        for (int k = 0; k < nJetCollections; ++k) {
          jetsMBoutput[k].nref = 0;

          gammajetMB[k].clearGammaJetPairs(phoIdx);
          if (nMB[centBin][vzBin][k] >= nEventsToMix) {
            for (int i=0; i<nEventsToMix; ++i) {
              Long64_t entryMB = iterMB[centBin][vzBin][k] % nMB[centBin][vzBin][k];     // roll back to the beginning if out of range
              treeJetMB[centBin][vzBin][k]->GetEntry(entryMB);

              if (doCorrectionL2L3 > 0) {
                correctorsL2L3[k].correctPtsL2L3(jetsMB[k]);
              }

              // write jets from minBiasJetSkimFile to outputFile
              for (int j = 0; j < jetsMB[k].nref; ++j) {
                if (TMath::Abs(jetsMB[k].jteta[j]) > cutJetEta) continue;
                if ((jetsMB[k].jtpt[j] < cutJetPt)) continue;

                jetsMBoutput[k].rawpt[jetsMBoutput[k].nref] = jetsMB[k].rawpt[j];
                jetsMBoutput[k].jtpt [jetsMBoutput[k].nref] = jetsMB[k].jtpt[j];
                jetsMBoutput[k].jteta[jetsMBoutput[k].nref] = jetsMB[k].jteta[j];
                jetsMBoutput[k].jty  [jetsMBoutput[k].nref] = jetsMB[k].jty[j];
                jetsMBoutput[k].jtphi[jetsMBoutput[k].nref] = jetsMB[k].jtphi[j];
                jetsMBoutput[k].jtm  [jetsMBoutput[k].nref] = jetsMB[k].jtm[j];
                jetsMBoutput[k].nref++;
              }

              // increase iterator
              iterMB[centBin][vzBin][k]++;
              if (iterMB[centBin][vzBin][k] == nMB[centBin][vzBin][k]) iterMB[centBin][vzBin][k] = 0;  // reset if necessary
            }
          } else {
            std::cout << "WARNING : the event lacks necessary number of MB events to mix." << std::endl;
            std::cout << Form("{run, lumis, event, j_entry} = %d, %d, %llu, %lld", run, lumis, event, j_entry) << std::endl;
            std::cout << Form("{hiBin, vz} = %d, %f", hiBin, vz) << std::endl;
            std::cout << "centBin = " << centBin << ", vzBin = " << vzBin << ", jetCollection index = " << k << std::endl;
            std::cout << "nMB[centBin][vzBin][jetCollection] = " << nMB[centBin][vzBin][k] << std::endl;
          }
          jetsMBoutput[k].b = -1;   // this branch is not an array.
          gammajetMB[k].makeGammaJetPairsMB(ggHi, jetsMBoutput[k], phoIdx);

          gammaJetTreeMB[k]->Fill();
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

      for (int i = 0; i < nJetCollections; ++i) {
        for (int j=0; j<=7; ++j) {
          gammaJetTree[i][j]->Fill();
          if (!doCorrectionSmearing) break;
        }
      }
    }
    inFile->Close();
  } // files loop

  std::cout <<  "Loop ENDED : ggHiNtuplizer/EventTree" << std::endl;
  std::cout << "entries            = " << Tentries << std::endl;
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

  for (int i = 0; i < nJetCollections; ++i) {
    for (int j = 0; j<=7; ++j) {
      std::cout << Form("gammaJetTree[%d][%d]->GetEntries() = ", i, j) << gammaJetTree[i][j]->GetEntries() << std::endl;
      if (!doCorrectionSmearing) break;
    }
  }

  if (doMix > 0) {
    for (int i = 0; i < nJetCollections; ++i) {
      std::cout << Form("gammaJetTreeMB[%d]->GetEntries() = ", i) << gammaJetTreeMB[i]->GetEntries() << std::endl;
      std::cout << Form("outputTreeJetMB[%d]->GetEntries() = ", i) << outputTreeJetMB[i]->GetEntries() << std::endl;
    }
  }

  output->cd();
  configTree->Write("", TObject::kOverwrite);

  output->Write("", TObject::kOverwrite);
  output->Close();
  if (doMix > 0 && inputMB) inputMB->Close();
}

int main(int argc, char** argv)
{
  if (argc == 5) {
    gammaJetSkim(argv[1], argv[2], argv[3], argv[4]);
    return 0;
  }
  else if (argc == 4) {
    gammaJetSkim(argv[1], argv[2], argv[3]);
    return 0;
  }
  else if (argc == 3) {
    gammaJetSkim(argv[1], argv[2]);
    return 0;
  }
  else {
    std::cout << "Usage : \n" <<
      "./gammaJetSkim.exe <configFile> <inputFile> <outputFile> (<minBiasJetSkimFile>)"
              << std::endl;
    return 1;
  }
}
