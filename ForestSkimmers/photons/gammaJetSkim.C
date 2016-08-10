#include <TFile.h>
#include <TTree.h>
#include <TRandom3.h>

#include <vector>
#include <string>
#include <iostream>

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

int gammaJetSkim(const TString configFile, const TString inputFile, const TString outputFile, const TString minBiasJetSkimFile = "", const int nJobs=-1, const int jobNum=-1) {
  std::cout << "running gammaJetSkim()" << std::endl;
  std::cout << "configFile  = " << configFile.Data() << std::endl;
  std::cout << "inputFile   = " << inputFile.Data() << std::endl;
  std::cout << "outputFile  = " << outputFile.Data() << std::endl;
  std::cout << "minBiasJetSkimFile = " << minBiasJetSkimFile.Data() << std::endl;

  const InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
  const CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

  // input configuration
  if (!configInput.isValid) {
    std::cout << "Invalid input configuration" << std::endl;
    return 1;
  }

  if (!configCuts.isValid) {
    std::cout << "Invalid cut configuration" << std::endl;
    return 1;
  }

  const int collisionType = configInput.proc[INPUT::kSKIM].i[INPUT::k_collisionType];
  // verbose about input configuration
  std::cout << "Input Configuration :" << std::endl;
  std::cout << "collisionType = " << collisionType << std::endl;
  std::cout << "collision = " << getCollisionTypeName((COLL::TYPE)collisionType).c_str() << std::endl;

  // cut configuration
  std::vector<std::string> jetCollections;
  std::vector<float> mcPthatWeights;

  const float cut_vz = configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].f[CUTS::EVT::k_vz];
  const float cut_pcollisionEventSelection = configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].i[CUTS::EVT::k_pcollisionEventSelection];
  const int cut_pPAprimaryVertexFilter = configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].i[CUTS::EVT::k_pPAprimaryVertexFilter];
  const int cut_pBeamScrapingFilter = configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].i[CUTS::EVT::k_pBeamScrapingFilter];

  jetCollections = ConfigurationParser::ParseList(configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].s[CUTS::JET::k_jetCollection]);

  const float cutPhoEt = configCuts.proc[CUTS::kSKIM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_et];
  const float cutPhoEta = configCuts.proc[CUTS::kSKIM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_eta];

  const float cutJetPt = configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].f[CUTS::JET::k_pt];
  const float cutJetEta = configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].f[CUTS::JET::k_eta];

  const int doMix = configCuts.proc[CUTS::kSKIM].obj[CUTS::kGAMMAJET].i[CUTS::GJT::k_doMix];
  const int nMaxEvents_minBiasMixing = configCuts.proc[CUTS::kSKIM].obj[CUTS::kGAMMAJET].i[CUTS::GJT::k_nMaxEvents_minBiasMixing];
  const int nCentralityBins = configCuts.proc[CUTS::kSKIM].obj[CUTS::kGAMMAJET].i[CUTS::GJT::k_nCentralityBins];
  const int nVertexBins = configCuts.proc[CUTS::kSKIM].obj[CUTS::kGAMMAJET].i[CUTS::GJT::k_nVertexBins];
  const int nEventPlaneBins = configCuts.proc[CUTS::kSKIM].obj[CUTS::kGAMMAJET].i[CUTS::GJT::k_nEventPlaneBins];
  const int nEventsToMix = configCuts.proc[CUTS::kSKIM].obj[CUTS::kGAMMAJET].i[CUTS::GJT::k_nEventsToMix];
  const float smearingResJet = configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].f[CUTS::JET::k_smearingRes];
  const float smearingResJetPhi = configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].f[CUTS::JET::k_smearingResPhi];
  const int doCorrectionSmearing = configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].i[CUTS::JET::k_doCorrectionSmearing];
  const int nSmear = configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].i[CUTS::JET::k_nSmear];
  const int nSmearBins = configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].i[CUTS::JET::k_nSmearBins];
  const int doEventWeight = configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].i[CUTS::EVT::k_doEventWeight];
  mcPthatWeights = ConfigurationParser::ParseListFloat(configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].s[CUTS::EVT::k_eventWeight]);
  const int doCorrectionL2L3 = configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].i[CUTS::JET::k_doCorrectionL2L3];
  const float energyScaleJet = configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].f[CUTS::JET::k_energyScale];
  const int doResidualCorrection = configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].i[CUTS::JET::k_doResidualCorrection];
  const std::string jetResidualCorrectionFile = configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].s[CUTS::JET::k_residualCorrectionFile];

  for (std::size_t i=0; i<mcPthatWeights.size(); ++i)
    std::cout << mcPthatWeights[i] << " ";
  std::cout << std::endl;

  const int nJetCollections = jetCollections.size();

  if (minBiasJetSkimFile.EqualTo("") && doMix) {
    std::cout << "no minBiasJetSkimFile was provided." << std::endl;
    std::cout << "But mixing was requested, aborting." << std::endl;
    return 1;
  }

  const bool isMC = collisionIsMC((COLL::TYPE)collisionType);
  const bool isHI = collisionIsHI((COLL::TYPE)collisionType);

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
  for (int i=0; i<nJetCollections; ++i)
    std::cout << Form("jetCollections[%d] = %s", i, jetCollections[i].c_str()) << std::endl;

  std::cout << "cutPhoEt  = " << cutPhoEt << std::endl;
  std::cout << "cutPhoEta = " << cutPhoEta << std::endl;

  std::cout << "doMix     = " << doMix << std::endl;
  if (doMix > 0) {
    std::cout << "nMaxEvents_minBiasMixing = " << nMaxEvents_minBiasMixing << std::endl;
    std::cout << "nCentralityBins          = " << nCentralityBins << std::endl;
    std::cout << "nVertexBins              = " << nVertexBins << std::endl;
    std::cout << "nEventPlaneBins          = " << nEventPlaneBins << std::endl;
    std::cout << "nEventsToMix             = " << nEventsToMix << std::endl;
  }

  std::cout << "smearingResJet          = " << smearingResJet << std::endl;
  std::cout << "smearingResJetPhi       = " << smearingResJetPhi << std::endl;
  std::cout << "doCorrectionSmearing    = " << doCorrectionSmearing << std::endl;

  std::cout << "doResidualCorrection    = " << doResidualCorrection << std::endl;
  std::cout << "jetResidualCorrectionFile = " << jetResidualCorrectionFile << std::endl;

  TFile* weightsFile = TFile::Open(configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].s[CUTS::EVT::k_weights_file].c_str(), "READ");
  TH2D* h_weights_PbPb = 0;
  TH1D* h_weights_pp = 0;
  if (isMC) {
    if (isHI)
      h_weights_PbPb = (TH2D*)weightsFile->Get("h_weights");
    else
      h_weights_pp = (TH1D*)weightsFile->Get("h_weights");
  }

  std::vector<L2L3ResidualWFits> correctorsL2L3(nJetCollections);
  if (doCorrectionL2L3 > 0) {
    for (int i=0; i<nJetCollections; ++i) {
      correctorsL2L3[i].setL2L3Residual(3, 3, false);
    }
  }

  TF1 *jetResidualFunction[4];
  if(doResidualCorrection){
    if(isHI){
      TFile *jetResidualFile = TFile::Open(jetResidualCorrectionFile.c_str());
      jetResidualFunction[3] = ((TH1F*)jetResidualFile->Get("resCorr_cent50to100_h"))->GetFunction("f1_p");
      jetResidualFunction[2] = ((TH1F*)jetResidualFile->Get("resCorr_cent30to50_h"))->GetFunction("f1_p");
      jetResidualFunction[1] = ((TH1F*)jetResidualFile->Get("resCorr_cent10to30_h"))->GetFunction("f1_p");
      jetResidualFunction[0] = ((TH1F*)jetResidualFile->Get("resCorr_cent0to10_h"))->GetFunction("f1_p");
      //jetResidualFile->Close();
    } else {
      jetResidualFunction[0] = new TF1("f1_p","(1+.5/x)",5,300);
    }
  }

  // mixed-event block
  int centBinWidth = 0;
  int vertexBinWidth = 0;
  float eventPlaneBinWidth = 0;

  std::vector<Jets> jetsMB(nJetCollections);

  TTree* treeJetMB[nCentralityBins][nVertexBins][nEventPlaneBins][nJetCollections];
  Long64_t nMB[nCentralityBins][nVertexBins][nEventPlaneBins][nJetCollections];
  Long64_t iterMB[nCentralityBins][nVertexBins][nEventPlaneBins][nJetCollections];   // index of the tree where the mixing starts

  TFile* inputMB = 0;

  if (doMix > 0) {
    centBinWidth = 200/nCentralityBins;  // number of "hiBin"s that a centrality bin covers
    vertexBinWidth = 30/nVertexBins;     // number of "vz"s    that a vertex     bin covers, accepted vz range is -15 to 15.
    eventPlaneBinWidth = TMath::Pi()/nEventPlaneBins;     // number of angles that a evtplane bin covers

    inputMB = TFile::Open(minBiasJetSkimFile, "READ");

    int seed = 0;
    if(nJobs == -1)
      seed = 12345;
    else
      seed = jobNum;
    
    TRandom3 rand(seed);    // random number seed should be fixed or reproducible

    std::cout << "Tree initialization for MinBias mixing" << std::endl;
    std::cout << Form("treeJetMB[%d][%d][%d][%d] is being read", nCentralityBins, nVertexBins, nEventPlaneBins, nJetCollections) << std::endl;

    for (int i=0; i<nCentralityBins; ++i) {
      for (int j=0; j<nVertexBins; ++j) {
        for (int k=0; k<nEventPlaneBins; ++k) {
          for (int l=0; l<nJetCollections; ++l) {
            // in minBiasJetSkimFile, name of a jet tree starts with jetCollection.
            std::string jetCollection = jetCollections[l].c_str();
            treeJetMB[i][j][k][l] = (TTree*)inputMB->Get(Form("%s_centBin%d_vzBin%d_evPlaneBin%d", jetCollection.c_str(), i, j, k));
            if (!treeJetMB[i][j][k][l]) {
              printf("jetCollection: %s with centBin: %i, vzBin: %i, evtPlaneBin: %i cannot be found in the MinBias jet skim file.\n", jetCollection.c_str(), i, j, k);
              printf("Config: nCentralityBins: %i, nVertexBins: %i, nEventPlaneBins: %i\n", nCentralityBins, nVertexBins, nEventPlaneBins);
              printf("Exiting...\n");
              return 1;
            }
            treeJetMB[i][j][k][l]->SetBranchStatus("*", 0);        // disable all branches
            treeJetMB[i][j][k][l]->SetBranchStatus("nref", 1);     // enable jet branches
            treeJetMB[i][j][k][l]->SetBranchStatus("rawpt", 1);    // enable jet branches
            treeJetMB[i][j][k][l]->SetBranchStatus("jtpt", 1);     // enable jet branches
            treeJetMB[i][j][k][l]->SetBranchStatus("jteta", 1);     // enable jet branches
            treeJetMB[i][j][k][l]->SetBranchStatus("jtphi", 1);     // enable jet branches
            treeJetMB[i][j][k][l]->SetBranchStatus("track*", 1);
            treeJetMB[i][j][k][l]->SetBranchStatus("charged*", 1);
            treeJetMB[i][j][k][l]->SetBranchStatus("photon*", 1);
            treeJetMB[i][j][k][l]->SetBranchStatus("neutral*", 1);
            treeJetMB[i][j][k][l]->SetBranchStatus("eMax*", 1);
            treeJetMB[i][j][k][l]->SetBranchStatus("eSum*", 1);
            treeJetMB[i][j][k][l]->SetBranchStatus("eN*", 1);
            treeJetMB[i][j][k][l]->SetBranchStatus("muMax*", 1);
            treeJetMB[i][j][k][l]->SetBranchStatus("muSum*", 1);
            treeJetMB[i][j][k][l]->SetBranchStatus("muN*", 1);

            nMB[i][j][k][l] = treeJetMB[i][j][k][l]->GetEntries();

            jetsMB[l].setupTreeForReading(treeJetMB[i][j][k][l]);    // all MB jet trees point to jetsMB
            int primeSeed = 0;
            if (nMB[i][j][k][l] != 0)
              primeSeed = rand.Integer(nMB[i][j][k][l]); // Integer(imax) Returns a random integer on [0, imax-1].
            iterMB[i][j][k][l] = primeSeed;

            if (nMB[i][j][k][l] < nEventsToMix) {
              std::cout << "centBin = " << i << ", vzBin = " << j << ", eventPlane = " << k << ", jetCollection = " << jetCollection.c_str() << std::endl;
              std::cout << "nMB[centBin][vzBin][EventPlaneBin][jetCollection] = " << nMB[i][j][k][l] << std::endl;
              std::cout << "nEventsToMix = " << nEventsToMix << std::endl;
              std::cout << "number of MB events in that bin is not enough for mixing" << std::endl;
            }
          }
        }
      }
    }
  }

  std::vector<jetCorrector> correctorsJetJES(nJetCollections);

  // smearing set up block
  jetCorrector correctorsJetSmear[nJetCollections][nSmearBins+1];

  TRandom3 randSmearing(12345);    // random number seed should be fixed or reproducible

  // pp resolution
  std::vector<double> CSN_PP = {0.06, 0.91, 0};
  std::vector<double> CSN_phi_PP = {7.72/100000000, 0.1222, 0.5818};

  // smear 0-30 %
  std::vector<double> CSN_HI_cent0030 = {0.06, 1.23, 7.38};
  std::vector<double> CSN_phi_HI_cent0030 = {-1.303/1000000, 0.1651, 1.864};
  // smear 30-100 %
  std::vector<double> CSN_HI_cent30100 = {0.06, 1.23, 2.1};
  std::vector<double> CSN_phi_HI_cent30100 = {-2.013/100000000, 0.1646, 1.04};

  // smear 0-10 %
  std::vector<double> CSN_HI_cent0010 = {0.06, 1.23, 8.38};
  std::vector<double> CSN_phi_HI_cent0010 = {-3.18781/10000000, 0.125911, 2.23898};
  // smear 10-30 %
  std::vector<double> CSN_HI_cent1030 = {0.06, 1.23, 5.88};
  std::vector<double> CSN_phi_HI_cent1030 = {1.14344/100000, 0.179847, 1.56128};
  // smear 30-50 %
  std::vector<double> CSN_HI_cent3050 = {0.06, 1.23, 3.24};
  std::vector<double> CSN_phi_HI_cent3050 = {0.0145775, 0.1222, 1.21751};
  // smear 50-100 %
  std::vector<double> CSN_HI_cent50100 = {0.06, 1.23, 0};
  std::vector<double> CSN_phi_HI_cent50100 = {-0.0073078, 0.168879, 0.798885};

  for (int i=0; i<nJetCollections; ++i) {
    for (int j=0; j<nSmearBins; ++j) {
      correctorsJetSmear[i][j].rand = randSmearing;
      correctorsJetSmear[i][j].CSN_PP = CSN_PP;
      correctorsJetSmear[i][j].CSN_phi_PP = CSN_phi_PP;

      switch (j) {
      case 0: //0-30
        correctorsJetSmear[i][j].CSN_HI = CSN_HI_cent0030;
        correctorsJetSmear[i][j].CSN_phi_HI = CSN_phi_HI_cent0030;
        break;
      case 1: //30-100
        correctorsJetSmear[i][j].CSN_HI = CSN_HI_cent30100;
        correctorsJetSmear[i][j].CSN_phi_HI = CSN_phi_HI_cent30100;
        break;
      case 2: //0-10
        correctorsJetSmear[i][j].CSN_HI = CSN_HI_cent0010;
        correctorsJetSmear[i][j].CSN_phi_HI = CSN_phi_HI_cent0010;
        break;
      case 3: //10-30
        correctorsJetSmear[i][j].CSN_HI = CSN_HI_cent1030;
        correctorsJetSmear[i][j].CSN_phi_HI = CSN_phi_HI_cent1030;
        break;
      case 4: //30-50
        correctorsJetSmear[i][j].CSN_HI = CSN_HI_cent3050;
        correctorsJetSmear[i][j].CSN_phi_HI = CSN_phi_HI_cent3050;
        break;
      case 5: //50-100
        correctorsJetSmear[i][j].CSN_HI = CSN_HI_cent50100;
        correctorsJetSmear[i][j].CSN_phi_HI = CSN_phi_HI_cent50100;
        break;
      case 6: //sys
        correctorsJetSmear[i][j].CSN_HI = CSN_HI_cent30100;
        correctorsJetSmear[i][j].CSN_phi_HI = CSN_phi_HI_cent30100;
        break;
      }
    }
    if(nSmearBins == 0){
      correctorsJetSmear[i][0].rand = randSmearing;
    }
  }

  TFile* output = TFile::Open(outputFile, "RECREATE");
  TTree* configTree = setupConfigurationTreeForWriting(configCuts);

  // output tree variables
  TTree *outputTreeHLT = 0, *outputTreeggHiNtuplizer = 0, *outputTreeHiEvt = 0, *outputTreeSkim = 0;

  TTree* outputTreeJet[nJetCollections] = {0};
  std::vector<Jets> outputJets(nJetCollections);

  TTree* gammaJetTree[nJetCollections][nSmearBins+1];
  GammaJet gammaJet[nJetCollections][nSmearBins+1];

  std::vector< std::vector< std::vector<float> > > jtpt_smeared(nJetCollections, std::vector< std::vector<float> >(nSmearBins+1, std::vector<float>()));
  std::vector< std::vector< std::vector<float> > > jtphi_smeared(nJetCollections, std::vector< std::vector<float> >(nSmearBins+1, std::vector<float>()));
  std::vector< std::vector< std::vector<float> > > jtpt_smeared_output(nJetCollections, std::vector< std::vector<float> >(nSmearBins+1, std::vector<float>()));
  std::vector< std::vector< std::vector<float> > > jtphi_smeared_output(nJetCollections, std::vector< std::vector<float> >(nSmearBins+1, std::vector<float>()));

  for (int i=0; i<nJetCollections; ++i) {
    std::string treeJetName = jetCollections[i].c_str();
    std::string treeJetTitle = jetCollections[i].c_str();
    // do not lose the current title
    outputTreeJet[i] = new TTree(treeJetName.c_str(), treeJetTitle.c_str());
    outputTreeJet[i]->SetMaxTreeSize(MAXTREESIZE);
    outputJets[i].setupTreeForWriting(outputTreeJet[i]);

    outputTreeJet[i]->Branch("jtpt_smeared", &(jtpt_smeared_output[i]));
    outputTreeJet[i]->Branch("jtphi_smeared", &(jtphi_smeared_output[i]));

    // pick a unique, but also not complicated name for gammaJet Trees
    // jet collection names which are complicated will be put into tree title
    std::string gammaJetTreeTitle = Form("%s : leading photon-jet correlations", jetCollections[i].c_str());

    for (int j=0; j<nSmearBins+1; ++j) {
      std::string gammaJetTreeName = Form("gamma_%s", jetCollections[i].c_str());
      if (j)
        gammaJetTreeName = Form("gamma_%s_smearBin%i", jetCollections[i].c_str(), j-1);

      gammaJetTree[i][j] = new TTree(gammaJetTreeName.c_str(), gammaJetTreeTitle.c_str());
      gammaJetTree[i][j]->SetMaxTreeSize(MAXTREESIZE);

      gammaJet[i][j].resetAwayRange();
      gammaJet[i][j].resetConeRange();
      gammaJet[i][j].branchGammaJetTree(gammaJetTree[i][j]);
    }
  }

  // mixed-event block
  TTree* outputTreeJetMB[nJetCollections];
  TTree* gammaJetTreeMB[nJetCollections];
  std::vector<Jets> jetsMBoutput(nJetCollections);     // object to write jet trees from MB events
  std::vector<GammaJet> gammaJetMB(nJetCollections);

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
      std::string gammaJetTreeMBName = Form("gamma_%sMB", jetCollections[i].c_str());
      std::string gammaJetTreeMBTitle = Form("%s : leading photon-jet correlations", jetCollections[i].c_str());
      gammaJetTreeMB[i] = new TTree(gammaJetTreeMBName.c_str(), gammaJetTreeMBTitle.c_str());

      gammaJetTreeMB[i]->SetMaxTreeSize(MAXTREESIZE);
      gammaJetMB[i].resetAwayRange();
      gammaJetMB[i].resetConeRange();
      gammaJetMB[i].branchGammaJetTree(gammaJetTreeMB[i]);
    }
  }

  EventMatcher* em = new EventMatcher();
  Long64_t duplicateEntries = 0;
  Long64_t totalEntries = 0;
  Long64_t entriesPassedEventSelection = 0;
  Long64_t entriesAnalyzed = 0;

  std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.Data());

  for (std::size_t i=0; i<inputFiles.size(); ++i)
    std::cout << inputFiles[i] << std::endl;
  std::cout << "input ROOT files : num = " << inputFiles.size() << std::endl;
  std::cout << "#####" << std::endl;

  std::vector<std::string>::iterator itFirst = inputFiles.begin();
  std::vector<std::string>::iterator itEnd = inputFiles.end();
  
  if (inputFiles.size() > 1 && nJobs != -1){
    if (jobNum >= nJobs) {
      std::cout << "jobNum > nJobs, invalid configuration, aborting" << std::endl;
      return 1;
    }

    int totFiles = inputFiles.size();
    itFirst = inputFiles.begin() + floor(totFiles/nJobs)*jobNum;
    itEnd = inputFiles.begin() + floor(totFiles/nJobs)*(jobNum+1);
    if (jobNum == nJobs-1)
      itEnd = inputFiles.end();

    std::cout << "For this job " << jobNum << std::endl;
    std::cout << "First Entry: " << floor(totFiles/nJobs)*jobNum << std::endl;
    std::cout << "Final Entry: " << floor(totFiles/nJobs)*(jobNum+1)<< std::endl;
  }

  for (std::vector<std::string>::iterator it = itFirst; it != itEnd; ++it) {
    std::cout << (*it).c_str() << std::endl;

    TFile *inFile = TFile::Open((*it).c_str());
    inFile->cd();

    TTree* treeHLT = (TTree*) inFile->Get("hltanalysis/HltTree");
    TTree* treeggHiNtuplizer = 0;
    if (isHI)
      treeggHiNtuplizer = (TTree*)inFile->Get("ggHiNtuplizer/EventTree");
    else
      treeggHiNtuplizer = (TTree*)inFile->Get("ggHiNtuplizerGED/EventTree");

    TTree* treeJet[nJetCollections];
    for (int i=0; i<nJetCollections; ++i)
      treeJet[i] = (TTree*) inFile->Get(Form("%s/t", jetCollections[i].c_str()));
    TTree* treeHiEvt = (TTree*)inFile->Get("hiEvtAnalyzer/HiTree");
    TTree* treeSkim  = (TTree*)inFile->Get("skimanalysis/HltTree");

    treeHLT->SetBranchStatus("*", 0);     // disable all branches
    treeHLT->SetBranchStatus("HLT_HISinglePhoton*_Eta*_v*", 1);     // enable photon branches
    treeHLT->SetBranchStatus("HLT_HIDoublePhoton*_Eta*_v*", 1);     // enable photon branches

    // objects for gamma-jet correlations
    ggHiNtuplizer ggHi;
    ggHi.setupTreeForReading(treeggHiNtuplizer);    // treeggHiNtuplizer is input
    std::vector<Jets> jets(nJetCollections);

    for (int i=0; i<nJetCollections; ++i)
      jets[i].setupTreeForReading(treeJet[i]);   // treeJet is input

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
    // treeHiEvt->SetBranchStatus("hiNevtPlane", 1);
    treeHiEvt->SetBranchStatus("hiEvtPlanes", 1);
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
    Float_t hiEvtPlanes[29];   //[hiNevtPlane]
    UInt_t run, lumis;
    ULong64_t event;
    float pthat;

    treeHiEvt->SetBranchAddress("vz", &vz);
    treeHiEvt->SetBranchAddress("hiBin", &hiBin);
    treeHiEvt->SetBranchAddress("hiEvtPlanes", &hiEvtPlanes);
    treeHiEvt->SetBranchAddress("run", &run);
    treeHiEvt->SetBranchAddress("evt", &event);
    treeHiEvt->SetBranchAddress("lumi", &lumis);
    if (isMC)
      treeHiEvt->SetBranchAddress("pthat", &pthat);

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

    Int_t HBHENoiseFilterResultRun2Loose;
    if (!isMC) {
      treeSkim->SetBranchStatus("HBHENoiseFilterResultRun2Loose", 1);
      if (treeSkim->GetBranch("HBHENoiseFilterResultRun2Loose")) {
        treeSkim->SetBranchAddress("HBHENoiseFilterResultRun2Loose", &HBHENoiseFilterResultRun2Loose);
      } else {   // overwrite to default
        HBHENoiseFilterResultRun2Loose = 1;
        std::cout << "could not get branch : HBHENoiseFilterResultRun2Loose" << std::endl;
        std::cout << "set to default value : HBHENoiseFilterResultRun2Loose = " << HBHENoiseFilterResultRun2Loose << std::endl;
      }
    } else {
      HBHENoiseFilterResultRun2Loose = 0;
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

    float eventWeight = 1;

    if (it == itFirst) {
      output->cd();
      outputTreeHLT = treeHLT->CloneTree(0);
      outputTreeHLT->SetName("hltTree");
      outputTreeHLT->SetTitle("subbranches of hltanalysis/HltTree");
      outputTreeggHiNtuplizer = treeggHiNtuplizer->CloneTree(0);
      outputTreeHiEvt = treeHiEvt->CloneTree(0);
      outputTreeHiEvt->SetName("HiEvt");
      outputTreeHiEvt->SetTitle("subbranches of hiEvtAnalyzer/HiTree");
      if (doEventWeight)
        outputTreeHiEvt->Branch("weight", &eventWeight, "weight/F");
      outputTreeHiEvt->Branch("pcollisionEventSelection", &pcollisionEventSelection, "pcollisionEventSelection/I");
      outputTreeHiEvt->Branch("pPAprimaryVertexFilter", &pPAprimaryVertexFilter, "pPAprimaryVertexFilter/I");
      outputTreeHiEvt->Branch("pBeamScrapingFilter", &pBeamScrapingFilter, "pBeamScrapingFilter/I");
      outputTreeHiEvt->Branch("HBHENoiseFilterResultRun2Loose", &HBHENoiseFilterResultRun2Loose, "HBHENoiseFilterResultRun2Loose/I");
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

    Long64_t nentries = treeggHiNtuplizer->GetEntries();
    long long firstEntry = 0;
    long long lastEntry = nentries;
    std::cout << "Total Entries: " << nentries << std::endl;

    if (inputFiles.size() == 1 && nJobs != -1) {
        if (jobNum >= nJobs) {
            std::cout << "jobNum > nJobs, invalid configuration, aborting" << std::endl;
            return 1;
        }

        firstEntry = floor(nentries/nJobs)*jobNum;
        lastEntry = floor(nentries/nJobs)*(jobNum+1);
        if (jobNum == nJobs-1)
            lastEntry = nentries;

        std::cout << "For this job " << jobNum << std::endl;
        std::cout << "First Entry: " << firstEntry << std::endl;
        std::cout << "Final Entry: " << lastEntry << std::endl;
    }

    totalEntries += nentries;
    for (long long jentry = firstEntry; jentry < lastEntry; jentry++) {
      if (jentry % 2000 == 0)
        printf("current entry = %lli out of %lli : %.1f%%\n", jentry, nentries, jentry*100.0/nentries);

      treeHLT->GetEntry(jentry);
      treeggHiNtuplizer->GetEntry(jentry);
      for (int i=0; i<nJetCollections; ++i)
        treeJet[i]->GetEntry(jentry);
      treeSkim->GetEntry(jentry);
      treeHiEvt->GetEntry(jentry);

      if (doEventWeight) {
        if (pthat >= 14.99 && pthat < 30.) {
          eventWeight = mcPthatWeights[0];
        } else if (pthat >= 30. && pthat < 50.) {
          eventWeight = mcPthatWeights[1];
        } else if (pthat >= 50. && pthat < 80.) {
          eventWeight = mcPthatWeights[2];
        } else if (pthat >= 80. && pthat < 120.) {
          eventWeight = mcPthatWeights[3];
        } else if (pthat >= 120.) {
          eventWeight = mcPthatWeights[4];
        } else {
          eventWeight = 0;
          std::cout << "ERROR: bad pthat value: " << pthat << std::endl;
        }

        if (isHI)
          eventWeight *= h_weights_PbPb->GetBinContent(h_weights_PbPb->FindBin(hiBin, vz));
        else
          eventWeight *= h_weights_pp->GetBinContent(h_weights_pp->FindBin(vz));
      }

      bool eventAdded = em->addEvent(run, lumis, event, jentry);
      if (!eventAdded) { // this event is duplicate, skip this one.
        duplicateEntries++;
        continue;
      }

      // event selection
      if (!(TMath::Abs(vz) < cut_vz))
        continue;
      if (isHI) {
        if ((pcollisionEventSelection < cut_pcollisionEventSelection))
          continue;
        if (!isMC && (HBHENoiseFilterResultRun2Loose < cut_pcollisionEventSelection))
          continue; // re-use config value...
      } else {
        if (pPAprimaryVertexFilter < cut_pPAprimaryVertexFilter || pBeamScrapingFilter < cut_pBeamScrapingFilter)
          continue;
      }

      entriesPassedEventSelection++;

      // photon-jet block
      // find leading photon
      int phoIdx = -1;     // index of the leading photon
      double maxPhoEt = -1;

      for (int i=0; i<ggHi.nPho; ++i) {
        bool failedEtCut = (ggHi.phoEt->at(i) < cutPhoEt);
        if (failedEtCut)
          continue;
        bool failedEtaCut = (TMath::Abs(ggHi.phoEta->at(i)) > cutPhoEta);
        if (failedEtaCut)
          continue;
        bool failedSpikeRejection;
        failedSpikeRejection = (ggHi.phoSigmaIEtaIEta->at(i) < 0.002 ||
                                ggHi.pho_swissCrx->at(i)     > 0.9   ||
                                TMath::Abs(ggHi.pho_seedTime->at(i)) > 3);
        if (failedSpikeRejection)
          continue;

        if (ggHi.phoEt->at(i) > maxPhoEt) {
          maxPhoEt = ggHi.phoEt->at(i);
          phoIdx = i;
        }
      }

      if (phoIdx == -1)
        continue;

      entriesAnalyzed++;

      for (int i=0; i<nJetCollections; ++i) {
        outputJets[i].nref = 0;

        //can't use helper functions because of centrality dependence
        //so much loop over jet collections manually
        if(doResidualCorrection){
          int centBin = 0;
          if(isHI){
            if(hiBin >= 100)
              centBin = 3;
            else if (hiBin >= 60)
              centBin = 2;
            else if (hiBin >= 20)
              centBin = 1;
            else
              centBin = 0;
          }
          double xmin, xmax;
          jetResidualFunction[centBin]->GetRange(xmin,xmax);
          for (int k=0; k<jets[i].nref; ++k) {
            if(jets[i].jtpt[k]<xmin || jets[i].jtpt[k]>xmax) continue;
            jets[i].jtpt[k] /= jetResidualFunction[centBin]->Eval(jets[i].jtpt[k]);
          }
        }
        
        if (doCorrectionL2L3 > 0)
          correctorsL2L3[i].correctPtsL2L3(jets[i]);

        // apply JES after corrections
        if (energyScaleJet > 0 && energyScaleJet != 1)
          correctorsJetJES.at(i).applyEnergyScale(jets[i], energyScaleJet);

        if (smearingResJetPhi > 0)
          correctorsJetSmear[i][0].applyPhisResolution(jets[i], smearingResJetPhi);
        if (smearingResJet > 0)
          correctorsJetSmear[i][0].applyPtsResolution(jets[i], smearingResJet);

        if (nSmearBins)
          jets[i].replicateJets(nSmear);

        for (int j=0; j<nSmearBins+1; ++j) {
          jtpt_smeared[i][j].clear();
          jtpt_smeared[i][j].reserve(nSmear * jets[i].nref);
          jtpt_smeared_output[i][j].clear();
          jtpt_smeared_output[i][j].reserve(nSmear * jets[i].nref);

          jtphi_smeared[i][j].clear();
          jtphi_smeared[i][j].reserve(nSmear * jets[i].nref);
          jtphi_smeared_output[i][j].clear();
          jtphi_smeared_output[i][j].reserve(nSmear * jets[i].nref);
        }

        // copy jtpt, jtphi to *_smeared[i][0]
        jtpt_smeared[i][0].assign(jets[i].jtpt, jets[i].jtpt + jets[i].nref);
        jtphi_smeared[i][0].assign(jets[i].jtphi, jets[i].jtphi + jets[i].nref);
        for (int j=0; j<nSmearBins; ++j) {
          correctorsJetSmear[i][j].applyPtSmearingToVector(jets[i], &(jtpt_smeared[i][j+1]));
          correctorsJetSmear[i][j].applyPhiSmearingToVector(jets[i], &(jtphi_smeared[i][j+1]));
        }

        // scrape some jets.
        for (int k=0; k<jets[i].nref; ++k) {
          if (TMath::Abs(jets[i].jteta[k]) > cutJetEta)
            continue;

          if (!(jets[i].jtpt[k] > 5))
            continue;
          int belowJetPtCut = 1;
          for (int j=0; j<nSmearBins+1; ++j)
            belowJetPtCut &= jtpt_smeared[i][j][k] < cutJetPt;
          if (belowJetPtCut)
            continue;

          outputJets[i].rawpt[outputJets[i].nref] = jets[i].rawpt[k];
          outputJets[i].jtpt[outputJets[i].nref] = jets[i].jtpt[k];
          outputJets[i].jteta[outputJets[i].nref] = jets[i].jteta[k];
          outputJets[i].jty[outputJets[i].nref] = jets[i].jty[k];
          outputJets[i].jtphi[outputJets[i].nref] = jets[i].jtphi[k];
          outputJets[i].jtm[outputJets[i].nref] = jets[i].jtm[k];
          outputJets[i].trackMax[outputJets[i].nref] = jets[i].trackMax[k];
          outputJets[i].trackSum[outputJets[i].nref] = jets[i].trackSum[k];
          outputJets[i].trackN[outputJets[i].nref] = jets[i].trackN[k];
          outputJets[i].trackHardSum[outputJets[i].nref] = jets[i].trackHardSum[k];
          outputJets[i].trackHardN[outputJets[i].nref] = jets[i].trackHardN[k];
          outputJets[i].chargedMax[outputJets[i].nref] = jets[i].chargedMax[k];
          outputJets[i].chargedN[outputJets[i].nref] = jets[i].chargedN[k];
          outputJets[i].chargedSum[outputJets[i].nref] = jets[i].chargedSum[k];
          outputJets[i].chargedHardSum[outputJets[i].nref] = jets[i].chargedHardSum[k];
          outputJets[i].chargedHardN[outputJets[i].nref] = jets[i].chargedHardN[k];
          outputJets[i].photonMax[outputJets[i].nref] = jets[i].photonMax[k];
          outputJets[i].photonSum[outputJets[i].nref] = jets[i].photonSum[k];
          outputJets[i].photonN[outputJets[i].nref] = jets[i].photonN[k];
          outputJets[i].photonHardSum[outputJets[i].nref] = jets[i].photonHardSum[k];
          outputJets[i].photonHardN[outputJets[i].nref] = jets[i].photonHardN[k];
          outputJets[i].neutralMax[outputJets[i].nref] = jets[i].neutralMax[k];
          outputJets[i].neutralSum[outputJets[i].nref] = jets[i].neutralSum[k];
          outputJets[i].neutralN[outputJets[i].nref] = jets[i].neutralN[k];
          outputJets[i].eMax[outputJets[i].nref] = jets[i].eMax[k];
          outputJets[i].eSum[outputJets[i].nref] = jets[i].eSum[k];
          outputJets[i].eN[outputJets[i].nref] = jets[i].eN[k];
          outputJets[i].muMax[outputJets[i].nref] = jets[i].muMax[k];
          outputJets[i].muSum[outputJets[i].nref] = jets[i].muSum[k];
          outputJets[i].muN[outputJets[i].nref] = jets[i].muN[k];

          if (isMC) {
            outputJets[i].genChargedSum[outputJets[i].nref] = jets[i].genChargedSum[k];
            outputJets[i].genHardSum[outputJets[i].nref] = jets[i].genHardSum[k];
            outputJets[i].signalChargedSum[outputJets[i].nref] = jets[i].signalChargedSum[k];
            outputJets[i].signalHardSum[outputJets[i].nref] = jets[i].signalHardSum[k];
            outputJets[i].subid[outputJets[i].nref] = jets[i].subid[k];
            // don't include the full gen jet tree (might regret this)
          }

          for (int j=0; j<nSmearBins+1; ++j) {
            jtpt_smeared_output[i][j].push_back(jtpt_smeared[i][j][k]);
            jtphi_smeared_output[i][j].push_back(jtphi_smeared[i][j][k]);
          }

          outputJets[i].nref++;
        }

        for (int j=0; j<nSmearBins+1; ++j)
          gammaJet[i][j].makeGammaJetPairsSmeared(ggHi, outputJets[i], phoIdx, &(jtpt_smeared_output[i][j]), &(jtphi_smeared_output[i][j]));
      }

      if (doMix > 0) {
        const int centBin = hiBin / centBinWidth;
        const int vzBin   = (vz+15) / vertexBinWidth;
        const int evplaneBin = (hiEvtPlanes[8]+(TMath::Pi()/2.)) / eventPlaneBinWidth;
        for (int i=0; i<nJetCollections; ++i) {
          jetsMBoutput[i].nref = 0;

          if (nMB[centBin][vzBin][evplaneBin][i] >= nEventsToMix) {
            for (int n=0; n<nEventsToMix; ++n) {
              const Long64_t entryMB = iterMB[centBin][vzBin][evplaneBin][i] % nMB[centBin][vzBin][evplaneBin][i];     // roll back to the beginning if out of range
              treeJetMB[centBin][vzBin][evplaneBin][i]->GetEntry(entryMB);

              //can't use helper functions because of centrality dependence
              //so much loop over jet collections manually
              if(doResidualCorrection){
                int rcentBin = 0;
                if(isHI){
                  if(hiBin >= 100)
                    rcentBin = 3;
                  else if (hiBin >= 60)
                    rcentBin = 2;
                  else if (hiBin >= 20)
                    rcentBin = 1;
                  else
                    rcentBin = 0;
                }

                double xmin, xmax;
                jetResidualFunction[rcentBin]->GetRange(xmin,xmax);
                for (int k=0; k<jetsMB[i].nref; ++k) {
                  if(jetsMB[i].jtpt[k]<xmin || jetsMB[i].jtpt[k]>xmax) continue;
                  jetsMB[i].jtpt[k] /= jetResidualFunction[rcentBin]->Eval(jetsMB[i].jtpt[k]);
                }
              }

              if (doCorrectionL2L3 > 0)
                correctorsL2L3[i].correctPtsL2L3(jetsMB[i]);

              // apply JES after corrections
              if (energyScaleJet > 0 && energyScaleJet != 1)
                correctorsJetJES.at(i).applyEnergyScale(jetsMB[i], energyScaleJet);

              if (smearingResJetPhi > 0)
                correctorsJetSmear[i][0].applyPhisResolution(jetsMB[i], smearingResJetPhi);
              if (smearingResJet > 0)
                correctorsJetSmear[i][0].applyPtsResolution(jetsMB[i], smearingResJet);

              // write jets from minBiasJetSkimFile to outputFile
              for (int k=0; k<jetsMB[i].nref; ++k) {
                if (TMath::Abs(jetsMB[i].jteta[k]) > cutJetEta)
                  continue;
                if ((jetsMB[i].jtpt[k] < cutJetPt))
                  continue;

                jetsMBoutput[i].rawpt[jetsMBoutput[i].nref] = jetsMB[i].rawpt[k];
                jetsMBoutput[i].jtpt[jetsMBoutput[i].nref] = jetsMB[i].jtpt[k];
                jetsMBoutput[i].jteta[jetsMBoutput[i].nref] = jetsMB[i].jteta[k];
                jetsMBoutput[i].jty[jetsMBoutput[i].nref] = jetsMB[i].jty[k];
                jetsMBoutput[i].jtphi[jetsMBoutput[i].nref] = jetsMB[i].jtphi[k];
                jetsMBoutput[i].jtm[jetsMBoutput[i].nref] = jetsMB[i].jtm[k];
                jetsMBoutput[i].trackMax[jetsMBoutput[i].nref] = jetsMB[i].trackMax[k];
                jetsMBoutput[i].trackSum[jetsMBoutput[i].nref] = jetsMB[i].trackSum[k];
                jetsMBoutput[i].trackN[jetsMBoutput[i].nref] = jetsMB[i].trackN[k];
                jetsMBoutput[i].trackHardSum[jetsMBoutput[i].nref] = jetsMB[i].trackHardSum[k];
                jetsMBoutput[i].trackHardN[jetsMBoutput[i].nref] = jetsMB[i].trackHardN[k];
                jetsMBoutput[i].chargedMax[jetsMBoutput[i].nref] = jetsMB[i].chargedMax[k];
                jetsMBoutput[i].chargedN[jetsMBoutput[i].nref] = jetsMB[i].chargedN[k];
                jetsMBoutput[i].chargedSum[jetsMBoutput[i].nref] = jetsMB[i].chargedSum[k];
                jetsMBoutput[i].chargedHardSum[jetsMBoutput[i].nref] = jetsMB[i].chargedHardSum[k];
                jetsMBoutput[i].chargedHardN[jetsMBoutput[i].nref] = jetsMB[i].chargedHardN[k];
                jetsMBoutput[i].photonMax[jetsMBoutput[i].nref] = jetsMB[i].photonMax[k];
                jetsMBoutput[i].photonSum[jetsMBoutput[i].nref] = jetsMB[i].photonSum[k];
                jetsMBoutput[i].photonN[jetsMBoutput[i].nref] = jetsMB[i].photonN[k];
                jetsMBoutput[i].photonHardSum[jetsMBoutput[i].nref] = jetsMB[i].photonHardSum[k];
                jetsMBoutput[i].photonHardN[jetsMBoutput[i].nref] = jetsMB[i].photonHardN[k];
                jetsMBoutput[i].neutralMax[jetsMBoutput[i].nref] = jetsMB[i].neutralMax[k];
                jetsMBoutput[i].neutralSum[jetsMBoutput[i].nref] = jetsMB[i].neutralSum[k];
                jetsMBoutput[i].neutralN[jetsMBoutput[i].nref] = jetsMB[i].neutralN[k];
                jetsMBoutput[i].eMax[jetsMBoutput[i].nref] = jetsMB[i].eMax[k];
                jetsMBoutput[i].eSum[jetsMBoutput[i].nref] = jetsMB[i].eSum[k];
                jetsMBoutput[i].eN[jetsMBoutput[i].nref] = jetsMB[i].eN[k];
                jetsMBoutput[i].muMax[jetsMBoutput[i].nref] = jetsMB[i].muMax[k];
                jetsMBoutput[i].muSum[jetsMBoutput[i].nref] = jetsMB[i].muSum[k];
                jetsMBoutput[i].muN[jetsMBoutput[i].nref] = jetsMB[i].muN[k];
                jetsMBoutput[i].nref++;
              }

              // increase iterator
              iterMB[centBin][vzBin][evplaneBin][i]++;
              if (iterMB[centBin][vzBin][evplaneBin][i] == nMB[centBin][vzBin][evplaneBin][i])
                iterMB[centBin][vzBin][evplaneBin][i] = 0;  // reset if necessary
            }
            treeJetMB[centBin][vzBin][evplaneBin][i]->DropBaskets();
          } else {
            std::cout << "WARNING : the event lacks necessary number of MB events to mix." << std::endl;
            std::cout << Form("{run, lumis, event, jentry} = %d, %d, %llu, %lld", run, lumis, event, jentry) << std::endl;
            std::cout << Form("{hiBin, vz, eventPlane} = %d, %f, %f", hiBin, vz, hiEvtPlanes[8]) << std::endl;
            printf("centBin = %i, vzBin = %i, evplaneBin = %i, jetCollection index = %i\n", centBin, vzBin, evplaneBin, i);
            std::cout << "nMB[centBin][vzBin][evplaneBin][jetCollection] = " << nMB[centBin][vzBin][evplaneBin][i] << std::endl;
          }

          jetsMBoutput[i].b = -1;   // this branch is not an array.

          gammaJetMB[i].makeGammaJetPairsMB(ggHi, jetsMBoutput[i], phoIdx);

          gammaJetTreeMB[i]->Fill();
          outputTreeJetMB[i]->Fill();
        }
      }

      outputTreeHLT->Fill();
      outputTreeggHiNtuplizer->Fill();
      for (int i=0; i<nJetCollections; ++i)
        outputTreeJet[i]->Fill();
      outputTreeHiEvt->Fill();
      outputTreeSkim->Fill();

      for (int i=0; i<nJetCollections; ++i){
        //treeJet[i]->DropBaskets();
        for (int j=0; j<nSmearBins+1; ++j)
          gammaJetTree[i][j]->Fill();
      }
    }
    inFile->Close();
  } // files loop

  std::cout << "Loop ENDED : ggHiNtuplizer/EventTree" << std::endl;
  std::cout << "entries            = " << totalEntries << std::endl;
  std::cout << "duplicateEntries   = " << duplicateEntries << std::endl;
  std::cout << "entriesPassedEventSelection   = " << entriesPassedEventSelection << std::endl;
  std::cout << "entriesAnalyzed               = " << entriesAnalyzed << std::endl;
  std::cout << "outputTreeHLT->GetEntries()   = " << outputTreeHLT->GetEntries() << std::endl;
  std::cout << "outputTreeggHiNtuplizer->GetEntries()   = " << outputTreeggHiNtuplizer->GetEntries() << std::endl;
  for (int i=0; i<nJetCollections; ++i)
    std::cout << Form("outputTreeJet[%d]->GetEntries()   = ", i) << outputTreeJet[i]->GetEntries() << std::endl;
  std::cout << "outputTreeSkim->GetEntries()  = " << outputTreeSkim->GetEntries() << std::endl;
  std::cout << "outputTreeHiEvt->GetEntries() = " << outputTreeHiEvt->GetEntries() << std::endl;

  for (int i=0; i<nJetCollections; ++i)
    for (int j=0; j<nSmearBins+1; ++j)
      std::cout << Form("gammaJetTree[%d][%d]->GetEntries() = ", i, j) << gammaJetTree[i][j]->GetEntries() << std::endl;

  if (doMix > 0) {
    for (int i=0; i<nJetCollections; ++i) {
      std::cout << Form("gammaJetTreeMB[%d]->GetEntries() = ", i) << gammaJetTreeMB[i]->GetEntries() << std::endl;
      std::cout << Form("outputTreeJetMB[%d]->GetEntries() = ", i) << outputTreeJetMB[i]->GetEntries() << std::endl;
    }
  }

  output->cd();
  configTree->Write("", TObject::kOverwrite);

  output->Write("", TObject::kOverwrite);
  output->Close();
  if (doMix > 0 && inputMB)
    inputMB->Close();

  std::cout << "gammaJetSkim() - END" << std::endl;

  return 0;
}

int main(int argc, char** argv) {
  if (argc == 7)
    return gammaJetSkim(argv[1], argv[2], argv[3], argv[4], atoi(argv[5]), atoi(argv[6]));
  else if (argc == 5)
    return gammaJetSkim(argv[1], argv[2], argv[3], argv[4]);
  else if (argc == 4)
    return gammaJetSkim(argv[1], argv[2], argv[3]);
  else
    printf("Usage : \n"
           "./gammaJetSkim.exe <configFile> <inputFile> <outputFile> (<minBiasJetSkimFile>)\n");

  return 1;
}
