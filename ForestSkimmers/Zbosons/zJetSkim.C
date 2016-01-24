#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TH1.h>
#include <TH1D.h>
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

const long MAXTREESIZE = 500000000000; // set maximum tree size from 10 GB to 100 GB, so that the code does not switch to a new file after 10 GB

void zJetSkim(const TString configFile, const TString inputFile, const TString outputFile = "zJetSkim.root", const TString minBiasJetSkimFile = "");

void zJetSkim(const TString configFile, const TString inputFile, const TString outputFile, const TString minBiasJetSkimFile)
{
       std::cout<<"running zJetSkim()"<<std::endl;
       std::cout<<"configFile  = "<< configFile.Data() <<std::endl;
       std::cout<<"inputFile   = "<< inputFile.Data() <<std::endl;
       std::cout<<"outputFile  = "<< outputFile.Data() <<std::endl;
       std::cout<<"minBiasJetSkimFile  = "<< minBiasJetSkimFile.Data() <<std::endl;

       InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
       CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

       // input configuration
       int collision;
       if (configInput.isValid) {
           collision = configInput.proc[INPUT::kSKIM].i[INPUT::k_collisionType];
       }
       else {
           collision = COLL::kPP;
       }
       // verbose about input configuration
       std::cout<<"Input Configuration :"<<std::endl;
       const char* collisionName =  getCollisionTypeName((COLL::TYPE)collision).c_str();
       std::cout << "collision = " << collisionName << std::endl;

       std::string jetCollection;
       int cut_nEle;
       float elePt;
       float eleSigmaIEtaIEta_2012_EB;
       float eleSigmaIEtaIEta_2012_EE;
       float eleHoverE_EB;
       float eleHoverE_EE;
       float cutZMassMin;
       float cutZMassMax;
       float cutZPt;
       float cutZEta;

       int doMix;
       int nMaxEvents_minBiasMixing;
       int nCentralityBins;
       int nVertexBins;
       int nEventsToMix;
       if (configCuts.isValid) {
           jetCollection = configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].s[CUTS::JET::k_jetCollection].c_str();
           cut_nEle = configCuts.proc[CUTS::kSKIM].obj[CUTS::kELECTRON].i[CUTS::ELE::k_nEle];
           elePt = configCuts.proc[CUTS::kSKIM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_elePt];
           eleSigmaIEtaIEta_2012_EB = configCuts.proc[CUTS::kSKIM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleSigmaIEtaIEta_2012_EB];
           eleSigmaIEtaIEta_2012_EE = configCuts.proc[CUTS::kSKIM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleSigmaIEtaIEta_2012_EE];
           eleHoverE_EB = configCuts.proc[CUTS::kSKIM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleHoverE_EB];
           eleHoverE_EE = configCuts.proc[CUTS::kSKIM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleHoverE_EE];
           cutZMassMin = configCuts.proc[CUTS::kSKIM].obj[CUTS::kZBOSON].f[CUTS::ZBO::k_massMin];
           cutZMassMax = configCuts.proc[CUTS::kSKIM].obj[CUTS::kZBOSON].f[CUTS::ZBO::k_massMax];
           cutZPt = configCuts.proc[CUTS::kSKIM].obj[CUTS::kZBOSON].f[CUTS::ZBO::k_pt];
           cutZEta = configCuts.proc[CUTS::kSKIM].obj[CUTS::kZBOSON].f[CUTS::ZBO::k_eta];

           doMix = configCuts.proc[CUTS::kSKIM].obj[CUTS::kZJET].i[CUTS::ZJT::k_doMix];
           nMaxEvents_minBiasMixing = configCuts.proc[CUTS::kSKIM].obj[CUTS::kZJET].i[CUTS::ZJT::k_nMaxEvents_minBiasMixing];
           nCentralityBins = configCuts.proc[CUTS::kSKIM].obj[CUTS::kZJET].i[CUTS::ZJT::k_nCentralityBins];
           nVertexBins = configCuts.proc[CUTS::kSKIM].obj[CUTS::kZJET].i[CUTS::ZJT::k_nVertexBins];
           nEventsToMix = configCuts.proc[CUTS::kSKIM].obj[CUTS::kZJET].i[CUTS::ZJT::k_nEventsToMix];
       }
       else {
           jetCollection = "ak4PFJetAnalyzer";
           cut_nEle = 2;
           elePt = 0;
           eleSigmaIEtaIEta_2012_EB = 0.02;
           eleSigmaIEtaIEta_2012_EE = 0.045;
           eleHoverE_EB = 0.2;
           eleHoverE_EE = 0.15;

           cutZMassMin = 80;
           cutZMassMax = 100;
           cutZPt = 15;
           cutZEta = 1.44;

           // default : no mixing
           doMix = 0;
           nMaxEvents_minBiasMixing = 0;
           nCentralityBins = 0;
           nVertexBins = 0;
           nEventsToMix = 0;
       }

       if(minBiasJetSkimFile.EqualTo("")) {
           std::cout<<"no minBiasJetSkimFile was provided."<<std::endl;
           std::cout<<"Minimum Bias event mixing will be skipped"<<std::endl;
           doMix = 0;
       }

       // verbose about cut configuration
       std::cout<<"Configuration :"<<std::endl;
       std::cout<<"jetCollection = "<<jetCollection.c_str()<<std::endl;

       std::cout<<"cut_nEle = "<<cut_nEle<<std::endl;
       std::cout<<"elePt = "<<elePt<<std::endl;
       std::cout<<"eleSigmaIEtaIEta_2012_EB = "<<eleSigmaIEtaIEta_2012_EB<<std::endl;
       std::cout<<"eleSigmaIEtaIEta_2012_EE = "<<eleSigmaIEtaIEta_2012_EE<<std::endl;
       std::cout<<"eleHoverE_EB = "<<eleHoverE_EB<<std::endl;
       std::cout<<"eleHoverE_EE = "<<eleHoverE_EE<<std::endl;
       std::cout<<"cutZMassMin = "<<cutZMassMin<<std::endl;
       std::cout<<"cutZMassMax = "<<cutZMassMax<<std::endl;
       std::cout<<"cutZPt   = "<<cutZPt<<std::endl;
       std::cout<<"cutZEta  = "<<cutZEta<<std::endl;

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

       bool isMC = collisionIsMC((COLL::TYPE)collision);
       bool isHI = collisionIsHI((COLL::TYPE)collision);

       TChain* treeHLT   = new TChain("hltanalysis/HltTree");
       TChain* treeggHiNtuplizer  = new TChain("ggHiNtuplizer/EventTree");
       TChain* treeEvent = new TChain("ggHiNtuplizer/EventTree");
       TChain* treeJet   = new TChain(Form("%s/t", jetCollection.c_str()));
       TChain* treeSkim  = new TChain("skimanalysis/HltTree");
       TChain* treeHiEvt;
       bool hasHiEvt = false;
       if (isHI || isMC) {
           treeHiEvt = new TChain("hiEvtAnalyzer/HiTree");
           hasHiEvt  = true;
       }
       else {
           treeHiEvt = 0;
           hasHiEvt  = false;
       }
       if (doMix > 0 && !hasHiEvt)
       {
           std::cout<<"mixing is requested in input. But the input file does not have hiEvtAnalyzer/HiTree"<<std::endl;
           doMix = 0;
           std::cout<<"mixing is disabled : doMix is set to "<< doMix <<std::endl;
       }

       for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
          treeHLT->Add((*it).c_str());
          treeggHiNtuplizer->Add((*it).c_str());
          treeEvent->Add((*it).c_str());
          treeJet->Add((*it).c_str());
          treeSkim->Add((*it).c_str());
          if(hasHiEvt) treeHiEvt->Add((*it).c_str());
       }

       treeHLT->SetBranchStatus("*",0);     // disable all branches
       treeHLT->SetBranchStatus("HLT_HI*SinglePhoton*Eta*v1*",1);     // enable photon branches
       treeHLT->SetBranchStatus("HLT_HI*DoublePhoton*Eta*v1*",1);     // enable photon branches
       treeHLT->SetBranchStatus("*DoubleMu*",1);                      // enable muon branches

       treeEvent->SetBranchStatus("*",0);
       treeEvent->SetBranchStatus("run",1);
       treeEvent->SetBranchStatus("event",1);
       treeEvent->SetBranchStatus("lumis",1);

       treeJet->SetBranchStatus("*",0);        // disable all branches
       treeJet->SetBranchStatus("nref",1);     // enable jet branches
       treeJet->SetBranchStatus("rawpt",1);    // enable jet branches
       treeJet->SetBranchStatus("jtpt",1);     // enable jet branches
       treeJet->SetBranchStatus("jteta",1);     // enable jet branches
       treeJet->SetBranchStatus("jtphi",1);     // enable jet branches
       treeJet->SetBranchStatus("track*",1);
       treeJet->SetBranchStatus("charged*",1);
       treeJet->SetBranchStatus("photon*",1);
       treeJet->SetBranchStatus("neutral*",1);
       treeJet->SetBranchStatus("eMax*",1);
       treeJet->SetBranchStatus("eSum*",1);
       treeJet->SetBranchStatus("eN*",1);
       treeJet->SetBranchStatus("muMax*",1);
       treeJet->SetBranchStatus("muSum*",1);
       treeJet->SetBranchStatus("muN*",1);
       if (isMC) {
           treeJet->SetBranchStatus("matchedPt",1);
           treeJet->SetBranchStatus("matchedR",1);
           treeJet->SetBranchStatus("beamId1",1);
           treeJet->SetBranchStatus("beamId2",1);
           treeJet->SetBranchStatus("pthat",1);
           treeJet->SetBranchStatus("ngen",1);
           treeJet->SetBranchStatus("gen*",1);
           treeJet->SetBranchStatus("signalChargedSum",1);
           treeJet->SetBranchStatus("signalHardSum",1);
           treeJet->SetBranchStatus("subid",1);
           treeJet->SetBranchStatus("smpt",1);
           treeJet->SetBranchStatus("fr01Chg",1);
           treeJet->SetBranchStatus("fr01EM",1);
           treeJet->SetBranchStatus("fr01",1);
       }

       // specify explicitly which branches to store, do not use wildcard
       float vz;
       Int_t hiBin;
       if (hasHiEvt) {
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
           }

           treeHiEvt->SetBranchAddress("vz",&vz);
           treeHiEvt->SetBranchAddress("hiBin",&hiBin);
       }
       else {   // overwrite to default
           vz = 0;
           hiBin = 200;
       }

       // specify explicitly which branches to store, do not use wildcard
       treeSkim->SetBranchStatus("*",0);
       treeSkim->SetBranchStatus("ana_step",1);
       treeSkim->SetBranchStatus("pcollisionEventSelection",1);
       treeSkim->SetBranchStatus("pHBHENoiseFilterResultProducer",1);
       treeSkim->SetBranchStatus("HBHENoiseFilterResultRun1",1);
       treeSkim->SetBranchStatus("HBHENoiseFilterResultRun2Loose",1);
       treeSkim->SetBranchStatus("HBHENoiseFilterResultRun2Tight",1);
       treeSkim->SetBranchStatus("HBHENoiseFilterResult",1);
       treeSkim->SetBranchStatus("HBHEIsoNoiseFilterResult",1);

       Int_t pcollisionEventSelection;
       Int_t pHBHENoiseFilterResultProducer;
       Int_t HBHEIsoNoiseFilterResult;

       if (treeSkim->GetBranch("pcollisionEventSelection")) {
           treeSkim->SetBranchAddress("pcollisionEventSelection",&pcollisionEventSelection);
       }
       else {   // overwrite to default
           pcollisionEventSelection = 1;
       }
       if (treeSkim->GetBranch("pHBHENoiseFilterResultProducer")) {
           treeSkim->SetBranchAddress("pHBHENoiseFilterResultProducer",&pHBHENoiseFilterResultProducer);
       }
       else {   // overwrite to default
           pHBHENoiseFilterResultProducer = 1;
       }
       if (treeSkim->GetBranch("HBHEIsoNoiseFilterResult")) {
           treeSkim->SetBranchAddress("HBHEIsoNoiseFilterResult",&HBHEIsoNoiseFilterResult);
       }
       else {   // overwrite to default
           HBHEIsoNoiseFilterResult = 1;
       }

       // event information
       Int_t run, lumis;
       Long64_t event;
       treeEvent->SetBranchAddress("run", &run);
       treeEvent->SetBranchAddress("event", &event);
       treeEvent->SetBranchAddress("lumis", &lumis);

       ggHiNtuplizer ggHi;
       ggHi.setupTreeForReading(treeggHiNtuplizer);    // treeggHiNtuplizer is input
       Jets jets;
       jets.setupTreeForReading(treeJet);               // treeJet is input

       // mixed-event block
       int centBinWidth = 0;
       int vertexBinWidth = 0;
       Jets jetsMB;     // object to read jet trees from MB events
       TChain *treeJetMB[nCentralityBins][nVertexBins];
       Long64_t nMB[nCentralityBins][nVertexBins];
       Long64_t iterMB[nCentralityBins][nVertexBins];   // index of the tree where the mixing starts
       if (doMix > 0) {
           centBinWidth = 200/nCentralityBins;  // number of "hiBin"s that a centrality bin covers
           vertexBinWidth = 30/nVertexBins;     // number of "vz"s    that a vertex     bin covers
                                                    // accepted vz range is -15 to 15.

           TRandom3 rand(12349);    // random number seed should be fixed or reproducible
           std::cout <<"Tree initialization for MinBias mixing" << std::endl;
           std::cout <<Form("treeJetMB[%d][%d] is being read", nCentralityBins, nVertexBins)<< std::endl;
           for(int i=0; i<nCentralityBins; ++i)
           {
               for(int j=0; j<nVertexBins; ++j){
                   treeJetMB[i][j] = new TChain(Form("jets_centBin%d_vzBin%d",i,j));
                   treeJetMB[i][j]->Add(minBiasJetSkimFile.Data());
                   nMB[i][j] = treeJetMB[i][j]->GetEntries();

                   jetsMB.setupTreeForReading(treeJetMB[i][j]);    // all MB jet trees point to jetsMB
                   int primeSeed = rand.Integer(10000); // Integer(imax) Returns a random integer on [0, imax-1].

                   if(nMB[i][j] != 0) iterMB[i][j] = primeSeed%(nMB[i][j]);
                   else               iterMB[i][j] = 0;

                   if (nMB[i][j] < nEventsToMix){
                       std::cout << "centBin = "<<i<<", vzBin = "<<j<<std::endl;
                       std::cout << "nMB[centBin][vzBin] = "<<nMB[i][j]<<std::endl;
                       std::cout << "nEventsToMix = "<<nEventsToMix<<std::endl;
                       std::cout << "number of MB events in that bin is not enough for mixing" <<std::endl;
                   }
               }
           }
       }

       TFile* output = new TFile(outputFile,"RECREATE");
       TTree *configTree = setupConfigurationTreeForWriting(configCuts);
       // output tree variables

       TTree *outputTreeHLT    = treeHLT->CloneTree(0);
       outputTreeHLT->SetName("hltTree");
       outputTreeHLT->SetTitle("subbranches of hltanalysis/HltTree");
       TTree *outputTreeggHiNtuplizer = treeggHiNtuplizer->CloneTree(0);
       TTree *outputTreeJet    = treeJet->CloneTree(0);
       outputTreeJet->SetName("jets");
       TTree *outputTreeHiEvt;
       if (hasHiEvt) {
           outputTreeHiEvt  = treeHiEvt->CloneTree(0);
           outputTreeHiEvt->SetName("HiEvt");
           outputTreeHiEvt->SetTitle("subbranches of hiEvtAnalyzer/HiTree");
           outputTreeHiEvt->SetMaxTreeSize(MAXTREESIZE);
       }
       else {
           outputTreeHiEvt = 0;
       }
       TTree *outputTreeSkim   = treeSkim->CloneTree(0);
       outputTreeSkim->SetName("skim");
       outputTreeSkim->SetTitle("subbranches of skimanalysis/HltTree");

       outputTreeHLT->SetMaxTreeSize(MAXTREESIZE);
       outputTreeggHiNtuplizer->SetMaxTreeSize(MAXTREESIZE);
       outputTreeJet->SetMaxTreeSize(MAXTREESIZE);
       outputTreeSkim->SetMaxTreeSize(MAXTREESIZE);

       // construct dielectron pairs during zJet skim
       TTree *diElectronTree = new TTree("dielectron","electron pairs");
       diElectronTree->SetMaxTreeSize(MAXTREESIZE);

       // objects for z-jet correlations
       dielectron diEle;
       diEle.branchDiElectronTree(diElectronTree);     // diElectronTree is output

       TTree *zJetTree = new TTree("zJet","leading z-jet correlations");
       zJetTree->SetMaxTreeSize(MAXTREESIZE);
       ZJet zjet;
       zjet.resetAwayRange();
       zjet.resetConeRange();
       zjet.branchZJetTree(zJetTree);

       // mixed-event block
       Jets jetsMBoutput;     // object to write jet trees from MB events
       ZJet zjetMB;
       TTree *outputTreeJetMB = new TTree("jetsMB","Jets from minbias events");
       TTree *zJetTreeMB  = new TTree("zJetMB","leading z-jet correlations from MB events");
       if (doMix>0) {
           outputTreeJetMB->SetMaxTreeSize(MAXTREESIZE);
           zJetTreeMB->SetMaxTreeSize(MAXTREESIZE);

           jetsMBoutput.setupTreeForWritingMB(outputTreeJetMB, true, false);

           zjetMB.resetAwayRange();
           zjetMB.resetConeRange();
           zjetMB.branchZJetTree(zJetTreeMB);
       }

       EventMatcher* em = new EventMatcher();
       Long64_t duplicateEntries = 0;

       Long64_t entries = treeEvent->GetEntries();
       Long64_t entriesPassedEventSelection = 0;
       Long64_t entriesAnalyzed = 0;
       std::cout << "entries         = " << entries << std::endl;
       std::cout<< "Loop : ggHiNtuplizer/EventTree" <<std::endl;
       for (Long64_t j_entry=0; j_entry<entries; ++j_entry)
       {
           if (j_entry % 2000 == 0)  {
             std::cout << "current entry = " <<j_entry<<" out of "<<entries<<" : "<<std::setprecision(2)<<(double)j_entry/entries*100<<" %"<<std::endl;
           }

           treeHLT->GetEntry(j_entry);
           treeggHiNtuplizer->GetEntry(j_entry);
           treeEvent->GetEntry(j_entry);
           treeJet->GetEntry(j_entry);
           treeSkim->GetEntry(j_entry);
           if(hasHiEvt){
               treeHiEvt->GetEntry(j_entry);
           }

           bool eventAdded = em->addEvent(run,lumis,event,j_entry);
           if(!eventAdded) // this event is duplicate, skip this one.
           {
               duplicateEntries++;
               continue;
           }

           // event selection
           if(!(TMath::Abs(vz) < 15 && pcollisionEventSelection == 1 && HBHEIsoNoiseFilterResult == 1)) continue;
//           if(!(TMath::Abs(vz) < 15 && pcollisionEventSelection == 1)) continue;
           entriesPassedEventSelection++;

           // skip if there are no electron pairs to study
           if(ggHi.nEle < cut_nEle)  continue;

           // construct dielectron pairs during zJet skim
           diEle.makeDiElectronPairs(ggHi);

           // z-jet block
           // find leading z
           int zIdx = -1;     // index of the leading z
           double maxZPt = -1;
           for(unsigned int i=0; i<(unsigned)diEle.diEleM_out.size(); ++i)
           {
               bool failedPtCut  = (diEle.diElePt_out.at(i) < cutZPt) ;
               bool failedEtaCut = (TMath::Abs(diEle.diEleEta_out.at(i)) > cutZEta) ;
               bool failedMassWindow = (diEle.diEleM_out.at(i) < cutZMassMin || diEle.diEleM_out.at(i) > cutZMassMax);
               bool failedOppositeCh = (diEle.eleCharge_1_out.at(i) == diEle.eleCharge_2_out.at(i));

               if (failedPtCut)          continue;
               if (failedEtaCut)         continue;
               if (failedMassWindow)     continue;
               if (failedOppositeCh)     continue;

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

               if (diEle.diElePt_out.at(i) > maxZPt)
               {
                   maxZPt = diEle.diElePt_out.at(i);
                   zIdx = i;
               }
           }
           if (zIdx == -1) continue;
           entriesAnalyzed++;

           // z-jet correlation
           // leading z Boson is correlated to each jet in the event.
           zjet.makeZJetPairs(diEle, jets, zIdx, true);

           if(doMix > 0)
           {
               int centBin = hiBin / centBinWidth;
               int vzBin   = (vz+15) / vertexBinWidth;
               jetsMBoutput.nref = 0;

               zjetMB.clearZJetPairs(zIdx);
               if (nMB[centBin][vzBin] >= nEventsToMix)
               {
                   for (int i=0; i<nEventsToMix; ++i)
                   {
                       Long64_t entryMB = iterMB[centBin][vzBin] % nMB[centBin][vzBin];     // roll back to the beginning if out of range
                       treeJetMB[centBin][vzBin]->GetEntry(entryMB);

                       zjetMB.makeZJetPairsMB(diEle, jetsMB, zIdx, true);

                       // write jets from minBiasJetSkimFile to outputFile
                       for(int j = 0; j < jetsMB.nref; ++j)
                       {
                           jetsMBoutput.rawpt[jetsMBoutput.nref] = jetsMB.rawpt[j];
                           jetsMBoutput.jtpt [jetsMBoutput.nref] = jetsMB.jtpt[j];
                           jetsMBoutput.jteta[jetsMBoutput.nref] = jetsMB.jteta[j];
                           jetsMBoutput.jty  [jetsMBoutput.nref] = jetsMB.jty[j];
                           jetsMBoutput.jtphi[jetsMBoutput.nref] = jetsMB.jtphi[j];
                           jetsMBoutput.jtm  [jetsMBoutput.nref] = jetsMB.jtm[j];
                           jetsMBoutput.nref++;
                       }

                       // increase iterator
                       iterMB[centBin][vzBin]++;
                       if (iterMB[centBin][vzBin] == nMB[centBin][vzBin])  iterMB[centBin][vzBin] = 0;  // reset if necessary
                   }
               }
               jetsMBoutput.b = -1;   // this branch is not an array.

               zJetTreeMB->Fill();
               outputTreeJetMB->Fill();
           }

           outputTreeHLT->Fill();
           outputTreeggHiNtuplizer->Fill();
           outputTreeJet->Fill();
           if (hasHiEvt) outputTreeHiEvt->Fill();
           outputTreeSkim->Fill();
           
           diElectronTree->Fill();
           zJetTree->Fill();
       }
       std::cout<<  "Loop ENDED : ggHiNtuplizer/EventTree" <<std::endl;
       std::cout << "entries            = " << entries << std::endl;
       std::cout << "duplicateEntries   = " << duplicateEntries << std::endl;
       std::cout << "entriesPassedEventSelection   = " << entriesPassedEventSelection << std::endl;
       std::cout << "entriesAnalyzed               = " << entriesAnalyzed << std::endl;
       std::cout << "outputTreeHLT->GetEntries()   = " << outputTreeHLT->GetEntries() << std::endl;
       std::cout << "outputTreeggHiNtuplizer->GetEntries()   = " << outputTreeggHiNtuplizer->GetEntries() << std::endl;
       std::cout << "outputTreeJet->GetEntries()   = " << outputTreeJet->GetEntries() << std::endl;
       std::cout << "outputTreeSkim->GetEntries()  = " << outputTreeSkim->GetEntries() << std::endl;
       if (hasHiEvt) std::cout << "outputTreeHiEvt->GetEntries() = " << outputTreeHiEvt->GetEntries() << std::endl;
    
       std::cout << "zJetTree->GetEntries() = " << zJetTree->GetEntries() << std::endl;
       if (doMix > 0)
       {
           std::cout << "zJetTreeMB->GetEntries() = " << zJetTreeMB->GetEntries() << std::endl;
           std::cout << "outputTreeJetMB->GetEntries() = " << outputTreeJetMB->GetEntries() << std::endl;
       }
  
       configTree->Write("",TObject::kOverwrite);

       output->Write("",TObject::kOverwrite);
       output->Close();
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
