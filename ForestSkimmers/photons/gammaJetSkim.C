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
  std::cout<<"running gammaJetSkim()"<<std::endl;
  std::cout<<"configFile  = "<< configFile.Data() <<std::endl;
  std::cout<<"inputFile   = "<< inputFile.Data() <<std::endl;
  std::cout<<"outputFile  = "<< outputFile.Data() <<std::endl;
  std::cout<<"minBiasJetSkimFile = "<< minBiasJetSkimFile.Data() <<std::endl;

  InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
  CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

  // input configuration
  if (!configInput.isValid) {
    std::cout << "Input Configuration invalid" << std::endl;
    return;
  }

  int collisionType = configInput.proc[INPUT::kSKIM].i[INPUT::k_collisionType];
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

  for(unsigned i = 0; i < mcFileWeights.size(); ++i)
    std::cout << mcFileWeights.at(i) << " ";
  std::cout << std::endl;
						      
  int nJetCollections = jetCollections.size();

  if(minBiasJetSkimFile.EqualTo("")) {
    std::cout<<"no minBiasJetSkimFile was provided."<<std::endl;
    std::cout<<"Minimum Bias event mixing will be skipped"<<std::endl;
    doMix = 0;
  }

  bool isMC = collisionIsMC((COLL::TYPE)collisionType);
  bool isHI = collisionIsHI((COLL::TYPE)collisionType);

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

  std::cout<<"cutPhoEt  = "<<cutPhoEt<<std::endl;
  std::cout<<"cutPhoEta = "<<cutPhoEta<<std::endl;

  std::cout<<"doMix     = "<<doMix<<std::endl;
  if (doMix > 0)
  {
    std::cout<<"nMaxEvents_minBiasMixing = "<< nMaxEvents_minBiasMixing <<std::endl;
    std::cout<<"nCentralityBins          = "<< nCentralityBins <<std::endl;
    std::cout<<"nVertexBins              = "<< nVertexBins <<std::endl;
    std::cout<<"nEventsToMix             = "<< nEventsToMix <<std::endl;
  }


  std::vector<L2L3ResidualWFits> correctorsL2L3(nJetCollections);
  if (doCorrectionL2L3 > 0)
  {
    for (int i = 0; i < nJetCollections; ++i) {
      correctorsL2L3.at(i).setL2L3Residual(3, 3, false);
    }
  }

  // mixed-event block
  int centBinWidth = 0;
  int vertexBinWidth = 0;
  std::vector<Jets> jetsMB(nJetCollections);     // object to read jet trees from MB events
  TTree* treeJetMB[nCentralityBins][nVertexBins][nJetCollections];
  Long64_t nMB[nCentralityBins][nVertexBins][nJetCollections];
  Long64_t iterMB[nCentralityBins][nVertexBins][nJetCollections];   // index of the tree where the mixing starts
  TFile* inputMB=0;
  if (doMix > 0) {
    centBinWidth = 200/nCentralityBins;  // number of "hiBin"s that a centrality bin covers
    vertexBinWidth = 30/nVertexBins;     // number of "vz"s    that a vertex     bin covers
    // accepted vz range is -15 to 15.

    inputMB = TFile::Open(minBiasJetSkimFile, "READ");

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

  //smearing set up block
  jetCorrector correctorsJetSmear[7][nJetCollections];
  if(doCorrectionSmearing){
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

    for(int j = 0; j < 7; ++j){
      for (int i = 0; i < nJetCollections; ++i) {
	correctorsJetSmear[j][i].rand = randSmearing;
	correctorsJetSmear[j][i].CSN_PP = CSN_PP;
	correctorsJetSmear[j][i].CSN_phi_PP = CSN_phi_PP;
	correctorsJetSmear[j][i].smearingBranchIndex = j+1;
      
	switch(j){
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
						      
  TFile* output = TFile::Open(outputFile,"RECREATE");
  TTree* configTree = setupConfigurationTreeForWriting(configCuts);

  // output tree variables
  TTree *outputTreeHLT=0, *outputTreeggHiNtuplizer=0, *outputTreeJet[nJetCollections]={0},
    *outputTreeHiEvt=0, *outputTreeSkim=0;

  TTree* gammaJetTree[nJetCollections][8];
  GammaJet gammajet[nJetCollections][8];

  std::vector<Jets> outputJets(nJetCollections);
  for (int i=0; i<nJetCollections; ++i) {
    std::string treeJetName = jetCollections.at(i).c_str();
    std::string treeJetTitle = jetCollections.at(i).c_str();
    // do not lose the current title
    outputTreeJet[i] = new TTree(treeJetName.c_str(), treeJetTitle.c_str());
    outputTreeJet[i]->SetMaxTreeSize(MAXTREESIZE);
    outputJets.at(i).setupTreeForWriting(outputTreeJet[i]);   
  }

  for (int i=0; i<nJetCollections; ++i) {
    for (int j=0; j<=7; ++j) {
      // pick a unique, but also not complicated name for gammaJet Trees
      // jet collection names which are complicated will be put into tree title
      std::string treegammaJetName, treegammaJetTitle;
      if(j==0){
	treegammaJetName = Form("gamma_%s", jetCollections.at(i).c_str());
      } else {
	treegammaJetName = Form("gamma_%s_smearBin%i", jetCollections.at(i).c_str(), j-1);
      }
      treegammaJetTitle = Form("%s : leading photon-jet correlations", jetCollections.at(i).c_str());

      gammaJetTree[i][j] = new TTree(treegammaJetName.c_str(),treegammaJetTitle.c_str());
      gammaJetTree[i][j]->SetMaxTreeSize(MAXTREESIZE);

      gammajet[i][j].resetAwayRange();
      gammajet[i][j].resetConeRange();
      gammajet[i][j].branchGammaJetTree(gammaJetTree[i][j]);
      if(!doCorrectionSmearing) break;
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
      std::string treeJetMBName = Form("%sMB", jetCollections.at(i).c_str());
      std::string treeJetMBTitle = Form("%s : jets from MB events", jetCollections.at(i).c_str());
      outputTreeJetMB[i] = new TTree(treeJetMBName.c_str(), treeJetMBTitle.c_str());

      outputTreeJetMB[i]->SetMaxTreeSize(MAXTREESIZE);
      jetsMBoutput.at(i).setupTreeForWritingMB(outputTreeJetMB[i], true, false);

      // gammaJetMB trees
      // pick a unique, but also not complicated name for gammaJetMB Trees
      // jet collection names which are complicated will be put into tree title
      std::string treegammaJetMBName = Form("gamma_%sMB", jetCollections.at(i).c_str());
      std::string treegammaJetMBTitle = Form("%s : leading photon-jet correlations", jetCollections.at(i).c_str());
      gammaJetTreeMB[i] = new TTree(treegammaJetMBName.c_str(),treegammaJetMBTitle.c_str());

      gammaJetTreeMB[i]->SetMaxTreeSize(MAXTREESIZE);
      gammajetMB.at(i).resetAwayRange();
      gammajetMB.at(i).resetConeRange();
      gammajetMB.at(i).branchGammaJetTree(gammaJetTreeMB[i]);
    }
  }

  EventMatcher* em = new EventMatcher();
  Long64_t duplicateEntries = 0;
  Long64_t Tentries = 0;
  Long64_t entriesPassedEventSelection =0;
  Long64_t entriesAnalyzed =0;

  std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.Data());

  for(unsigned i = 0; i < inputFiles.size(); ++i)
    std::cout << inputFiles.at(i) << " ";
  std::cout << std::endl;
						      
  std::cout<<"input ROOT files : num = "<<inputFiles.size()<< std::endl;
  std::cout<<"#####"<< std::endl;
  for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
    std::cout<<(*it).c_str()<< std::endl;

    TFile *inFile = TFile::Open((*it).c_str());
    inFile->cd();
    TTree* treeHLT   = (TTree*) inFile->Get("hltanalysis/HltTree");
    TTree* treeggHiNtuplizer =0;
    if(isHI) {
      treeggHiNtuplizer = (TTree*) inFile->Get("ggHiNtuplizer/EventTree");
    } else {
      treeggHiNtuplizer = (TTree*) inFile->Get("ggHiNtuplizerGED/EventTree");
    }
    TTree* treeJet[nJetCollections];
    for (int i=0; i<nJetCollections; ++i) {
      treeJet[i] = (TTree*) inFile->Get(Form("%s/t", jetCollections.at(i).c_str()));
    }
    TTree* treeHiEvt = (TTree*) inFile->Get("hiEvtAnalyzer/HiTree");
    TTree* treeSkim  = (TTree*) inFile->Get("skimanalysis/HltTree");

    treeHLT->SetBranchStatus("*",0);     // disable all branches
    treeHLT->SetBranchStatus("HLT_HISinglePhoton*_Eta*_v*",1);     // enable photon branches
    treeHLT->SetBranchStatus("HLT_HIDoublePhoton*_Eta*_v*",1);     // enable photon branches

    // objects for gamma-jet correlations
    ggHiNtuplizer ggHi;
    ggHi.setupTreeForReading(treeggHiNtuplizer);    // treeggHiNtuplizer is input
    std::vector<Jets> jets(nJetCollections);
    for (int i=0; i<nJetCollections; ++i) {
      jets.at(i).setupTreeForReading(treeJet[i]);   // treeJet is input
    }

    for (int i=0; i<nJetCollections; ++i) {
      treeJet[i]->SetBranchStatus("*",0);        // disable all branches
      treeJet[i]->SetBranchStatus("nref",1);     // enable jet branches
      treeJet[i]->SetBranchStatus("rawpt",1);    // enable jet branches
      treeJet[i]->SetBranchStatus("jtpt",1);     // enable jet branches
      treeJet[i]->SetBranchStatus("jteta",1);     // enable jet branches
      treeJet[i]->SetBranchStatus("jtphi",1);     // enable jet branches
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
      //treeHiEvt->SetBranchStatus("weight",1); // set a new weight, don't use the old one
      treeHiEvt->SetBranchStatus("alphaQCD",1);
      treeHiEvt->SetBranchStatus("alphaQED",1);
      treeHiEvt->SetBranchStatus("qScale",1);
    }

    float vz;
    Int_t hiBin;
    UInt_t run, lumis;
    ULong64_t event;

    treeHiEvt->SetBranchAddress("vz",&vz);
    treeHiEvt->SetBranchAddress("hiBin",&hiBin);
    treeHiEvt->SetBranchAddress("run", &run);
    treeHiEvt->SetBranchAddress("evt", &event);
    treeHiEvt->SetBranchAddress("lumi", &lumis);

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
    if (!isHI) {
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
    if (!isHI) {
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

    float eventWeight;

    if(it == inputFiles.begin()) {
      output->cd();
      outputTreeHLT    = treeHLT->CloneTree(0);
      outputTreeHLT->SetName("hltTree");
      outputTreeHLT->SetTitle("subbranches of hltanalysis/HltTree");
      outputTreeggHiNtuplizer = treeggHiNtuplizer->CloneTree(0);
      outputTreeHiEvt = treeHiEvt->CloneTree(0);
      outputTreeHiEvt->SetName("HiEvt");
      outputTreeHiEvt->SetTitle("subbranches of hiEvtAnalyzer/HiTree");
      if(doEventWeight){
	outputTreeHiEvt->Branch("weight",&eventWeight,"weight/F");
      }
      outputTreeSkim   = treeSkim->CloneTree(0);
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
    std::cout<< "Loop : ggHiNtuplizer/EventTree" <<std::endl;
    for (Long64_t j_entry=0; j_entry<entries; ++j_entry)
    {
      if (j_entry % 2000 == 0)  {
	std::cout << "current entry = " <<j_entry<<" out of "<<entries<<" : "<<std::setprecision(2)<<(double)j_entry/entries*100<<" %"<<std::endl;
      }

      treeHLT->GetEntry(j_entry);
      treeggHiNtuplizer->GetEntry(j_entry);
      for (int i=0; i<nJetCollections; ++i) {
	treeJet[i]->GetEntry(j_entry);
      }
      treeSkim->GetEntry(j_entry);
      treeHiEvt->GetEntry(j_entry);
      if(doEventWeight){
	eventWeight = mcFileWeights.at(it - inputFiles.begin());
      }

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

      // photon-jet block
      // find leading photon
      int phoIdx = -1;     // index of the leading photon
      double maxPhoEt = -1;
      for(int i=0; i<ggHi.nPho; ++i)
      {
	bool failedEtCut  = (ggHi.phoEt->at(i) < cutPhoEt) ;
	bool failedEtaCut = (TMath::Abs(ggHi.phoEta->at(i)) > cutPhoEta) ;
	bool failedSpikeRejection;
	failedSpikeRejection = (ggHi.phoSigmaIEtaIEta->at(i) < 0.002 ||
				ggHi.pho_swissCrx->at(i)     > 0.9   ||
				TMath::Abs(ggHi.pho_seedTime->at(i)) > 3);
	bool failedHoverE = (ggHi.phoHoverE->at(i) > 0.2);      // <0.1 cut is applied after corrections

	if (failedEtCut)          continue;
	if (failedEtaCut)         continue;
	if (failedSpikeRejection) continue;
	if (failedHoverE)         continue;

	if (ggHi.phoEt->at(i) > maxPhoEt)
	{
	  maxPhoEt = ggHi.phoEt->at(i);
	  phoIdx = i;
	}
      }
      if (phoIdx == -1) continue;
      entriesAnalyzed++;

      for (int i=0; i<nJetCollections; ++i) {
	outputJets.at(i).nref = 0;
	if (doCorrectionL2L3 > 0)
	{
	  correctorsL2L3.at(i).correctPtsL2L3(jets.at(i));
	}
 
	if(doCorrectionSmearing){
	  jets.at(i).replicateJets(nSmear);
	  for (int j =0; j<=7; ++j) {
	    if(j !=0 ){
	      correctorsJetSmear[j-1][i].correctPtsSmearing(jets.at(i)); 
	      correctorsJetSmear[j-1][i].correctPhisSmearing(jets.at(i));
	    }
	  }
	}

	// scrape some jets.
	for(int j = 0; j < jets.at(i).nref; ++j){
	  if(TMath::Abs(jets.at(i).jteta[j]) > cutJetEta) continue;
	  if(doCorrectionSmearing){
	    if((jets.at(i).jtpt[j] < cutJetPt) &&
	       (jets.at(i).jtpt_smeared_0_30[j] < cutJetPt) &&
	       (jets.at(i).jtpt_smeared_30_100[j] < cutJetPt) &&
	       (jets.at(i).jtpt_smeared_0_10[j] < cutJetPt) &&
	       (jets.at(i).jtpt_smeared_10_30[j] < cutJetPt) &&
	       (jets.at(i).jtpt_smeared_30_50[j] < cutJetPt) &&
	       (jets.at(i).jtpt_smeared_50_100[j] < cutJetPt) &&
	       (jets.at(i).jtpt_smeared_sys[j] < cutJetPt)) continue;
	  } else {
	    if((jets.at(i).jtpt[j] < cutJetPt)) continue;
	  }
	  
	  outputJets.at(i).rawpt[outputJets.at(i).nref] = jets.at(i).rawpt[j];
	  outputJets.at(i).jtpt[outputJets.at(i).nref] = jets.at(i).jtpt[j];
	  outputJets.at(i).jteta[outputJets.at(i).nref] = jets.at(i).jteta[j];
	  outputJets.at(i).jty[outputJets.at(i).nref] = jets.at(i).jty[j];
	  outputJets.at(i).jtphi[outputJets.at(i).nref] = jets.at(i).jtphi[j];
	  outputJets.at(i).jtm[outputJets.at(i).nref] = jets.at(i).jtm[j];
	  outputJets.at(i).trackMax[outputJets.at(i).nref] = jets.at(i).trackMax[j];
	  outputJets.at(i).trackSum[outputJets.at(i).nref] = jets.at(i).trackSum[j];
	  outputJets.at(i).trackN[outputJets.at(i).nref] = jets.at(i).trackN[j];
	  outputJets.at(i).trackHardSum[outputJets.at(i).nref] = jets.at(i).trackHardSum[j];
	  outputJets.at(i).trackHardN[outputJets.at(i).nref] = jets.at(i).trackHardN[j];
	  outputJets.at(i).chargedMax[outputJets.at(i).nref] = jets.at(i).chargedMax[j];
	  outputJets.at(i).chargedN[outputJets.at(i).nref] = jets.at(i).chargedN[j];
	  outputJets.at(i).chargedSum[outputJets.at(i).nref] = jets.at(i).chargedSum[j];
	  outputJets.at(i).chargedHardSum[outputJets.at(i).nref] = jets.at(i).chargedHardSum[j];
	  outputJets.at(i).chargedHardN[outputJets.at(i).nref] = jets.at(i).chargedHardN[j];
	  outputJets.at(i).photonMax[outputJets.at(i).nref] = jets.at(i).photonMax[j];
	  outputJets.at(i).photonSum[outputJets.at(i).nref] = jets.at(i).photonSum[j];
	  outputJets.at(i).photonN[outputJets.at(i).nref] = jets.at(i).photonN[j];
	  outputJets.at(i).photonHardSum[outputJets.at(i).nref] = jets.at(i).photonHardSum[j];
	  outputJets.at(i).photonHardN[outputJets.at(i).nref] = jets.at(i).photonHardN[j];
	  outputJets.at(i).neutralMax[outputJets.at(i).nref] = jets.at(i).neutralMax[j];
	  outputJets.at(i).neutralSum[outputJets.at(i).nref] = jets.at(i).neutralSum[j];
	  outputJets.at(i).neutralN[outputJets.at(i).nref] = jets.at(i).neutralN[j];
	  outputJets.at(i).eMax[outputJets.at(i).nref] = jets.at(i).eMax[j];
	  outputJets.at(i).eSum[outputJets.at(i).nref] = jets.at(i).eSum[j];
	  outputJets.at(i).eN[outputJets.at(i).nref] = jets.at(i).eN[j];
	  outputJets.at(i).muMax[outputJets.at(i).nref] = jets.at(i).muMax[j];
	  outputJets.at(i).muSum[outputJets.at(i).nref] = jets.at(i).muSum[j];
	  outputJets.at(i).muN[outputJets.at(i).nref] = jets.at(i).muN[j];

	  if(isMC){
	    outputJets.at(i).genChargedSum[outputJets.at(i).nref] = jets.at(i).genChargedSum[j];
	    outputJets.at(i).genHardSum[outputJets.at(i).nref] = jets.at(i).genHardSum[j];
	    outputJets.at(i).signalChargedSum[outputJets.at(i).nref] = jets.at(i).signalChargedSum[j];
	    outputJets.at(i).signalHardSum[outputJets.at(i).nref] = jets.at(i).signalHardSum[j];
	    outputJets.at(i).subid[outputJets.at(i).nref] = jets.at(i).subid[j];
	    // don't include the full gen jet tree (might regret this)
	  }
	  
	  if(doCorrectionSmearing){
	    outputJets.at(i).jtpt_smeared_0_30[outputJets.at(i).nref] = jets.at(i).jtpt_smeared_0_30[j];
	    outputJets.at(i).jtpt_smeared_30_100[outputJets.at(i).nref] = jets.at(i).jtpt_smeared_30_100[j];
	    outputJets.at(i).jtpt_smeared_0_10[outputJets.at(i).nref] = jets.at(i).jtpt_smeared_0_10[j];
	    outputJets.at(i).jtpt_smeared_10_30[outputJets.at(i).nref] = jets.at(i).jtpt_smeared_10_30[j];
	    outputJets.at(i).jtpt_smeared_30_50[outputJets.at(i).nref] = jets.at(i).jtpt_smeared_30_50[j];
	    outputJets.at(i).jtpt_smeared_50_100[outputJets.at(i).nref] = jets.at(i).jtpt_smeared_50_100[j];
	    outputJets.at(i).jtpt_smeared_sys[outputJets.at(i).nref] = jets.at(i).jtpt_smeared_sys[j];

	    outputJets.at(i).jtphi_smeared_0_30[outputJets.at(i).nref] = jets.at(i).jtphi_smeared_0_30[j];
	    outputJets.at(i).jtphi_smeared_30_100[outputJets.at(i).nref] = jets.at(i).jtphi_smeared_30_100[j];
	    outputJets.at(i).jtphi_smeared_0_10[outputJets.at(i).nref] = jets.at(i).jtphi_smeared_0_10[j];
	    outputJets.at(i).jtphi_smeared_10_30[outputJets.at(i).nref] = jets.at(i).jtphi_smeared_10_30[j];
	    outputJets.at(i).jtphi_smeared_30_50[outputJets.at(i).nref] = jets.at(i).jtphi_smeared_30_50[j];
	    outputJets.at(i).jtphi_smeared_50_100[outputJets.at(i).nref] = jets.at(i).jtphi_smeared_50_100[j];
	    outputJets.at(i).jtphi_smeared_sys[outputJets.at(i).nref] = jets.at(i).jtphi_smeared_sys[j];
	  }
	  outputJets.at(i).nref++;
	}
	for (int j =0; j<=7; ++j) {
	  gammajet[i][j].makeGammaJetPairs(ggHi, outputJets.at(i), phoIdx, j);
	  if(!doCorrectionSmearing) break;
	}
      }

      if(doMix > 0)
      {
	int centBin = hiBin / centBinWidth;
	int vzBin   = (vz+15) / vertexBinWidth;
	for (int k = 0; k < nJetCollections; ++k) {
	  jetsMBoutput.at(k).nref = 0;

	  gammajetMB.at(k).clearGammaJetPairs(phoIdx);
	  if (nMB[centBin][vzBin][k] >= nEventsToMix)
	  {
	    for (int i=0; i<nEventsToMix; ++i)
	    {
	      Long64_t entryMB = iterMB[centBin][vzBin][k] % nMB[centBin][vzBin][k];     // roll back to the beginning if out of range
	      treeJetMB[centBin][vzBin][k]->GetEntry(entryMB);

	      if (doCorrectionL2L3 > 0)
	      {
		correctorsL2L3.at(k).correctPtsL2L3(jetsMB.at(k));
	      }

	      gammajetMB.at(k).makeGammaJetPairsMB(ggHi, jetsMB.at(k), phoIdx);

	      // write jets from minBiasJetSkimFile to outputFile
	      for(int j = 0; j < jetsMB.at(k).nref; ++j)
	      {
		if(TMath::Abs(jetsMB.at(k).jteta[j]) > cutJetEta) continue;
		if((jetsMB.at(k).jtpt[j] < cutJetPt)) continue;

		jetsMBoutput.at(k).rawpt[jetsMBoutput.at(k).nref] = jetsMB.at(k).rawpt[j];
		jetsMBoutput.at(k).jtpt [jetsMBoutput.at(k).nref] = jetsMB.at(k).jtpt[j];
		jetsMBoutput.at(k).jteta[jetsMBoutput.at(k).nref] = jetsMB.at(k).jteta[j];
		jetsMBoutput.at(k).jty  [jetsMBoutput.at(k).nref] = jetsMB.at(k).jty[j];
		jetsMBoutput.at(k).jtphi[jetsMBoutput.at(k).nref] = jetsMB.at(k).jtphi[j];
		jetsMBoutput.at(k).jtm  [jetsMBoutput.at(k).nref] = jetsMB.at(k).jtm[j];
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
	for(int j = 0; j <=7; ++j) {
	  gammaJetTree[i][j]->Fill();
	  if(!doCorrectionSmearing) break;
	}
      }
    }
    inFile->Close();
  } // files loop
  std::cout<<  "Loop ENDED : ggHiNtuplizer/EventTree" <<std::endl;
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
      std::cout << Form("gammaJetTree[%d][%d]->GetEntries() = ", i,j) << gammaJetTree[i][j]->GetEntries() << std::endl;
      if(!doCorrectionSmearing) break;
    }
  }

  if (doMix > 0)
  {
    for (int i = 0; i < nJetCollections; ++i) {
      std::cout << Form("gammaJetTreeMB[%d]->GetEntries() = ", i) << gammaJetTreeMB[i]->GetEntries() << std::endl;
      std::cout << Form("outputTreeJetMB[%d]->GetEntries() = ", i) << outputTreeJetMB[i]->GetEntries() << std::endl;
    }
  }

  output->cd();
  configTree->Write("",TObject::kOverwrite);

  output->Write("",TObject::kOverwrite);
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
