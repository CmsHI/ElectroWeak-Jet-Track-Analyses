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
#include "../../TreeHeaders/gammaJetTree.h"
#include "../../CutConfigurations/interface/CutConfigurationsParser.h"
#include "../../TreeHeaders/CutConfigurationTree.h"
#include "../../Plotting/commonUtility.h"
#include "../../Utilities/interface/InputConfigurationParser.h"

const long MAXTREESIZE = 200000000000; // set maximum tree size from 10 GB to 100 GB, so that the code does not switch to a new file after 10 GB

void gammaJetSkim(const TString configFile, const TString inputFile, const TString outputFile = "gammaJetSkim.root", const TString minBiasJetSkimFile = "");

void gammaJetSkim(const TString configFile, const TString inputFile, const TString outputFile, const TString minBiasJetSkimFile)
{
       std::cout<<"running gammaJetSkim()"<<std::endl;
       std::cout<<"configFile  = "<< configFile.Data() <<std::endl;
       std::cout<<"inputFile   = "<< inputFile.Data() <<std::endl;
       std::cout<<"outputFile  = "<< outputFile.Data() <<std::endl;
       std::cout<<"minBiasJetSkimFile  = "<< minBiasJetSkimFile.Data() <<std::endl;

       InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
       CutConfiguration configCuts = CutConfigurationsParser::Parse(configFile.Data());

       // input configuration
       int collision;
       if (configInput.isValid) {
           collision = configInput.proc[INPUT::kSKIM].i[INPUT::k_CollisionType];
       }
       else {
           collision = COLL::kPP;
       }
       // verbose about input configuration
       std::cout<<"Input Configuration :"<<std::endl;
       const char* collisionName =  getCollisionTypeName((COLL::TYPE)collision).c_str();
       std::cout << "collision = " << collisionName << std::endl;

       std::string jetCollection;
       float cutPhoEt;
       float cutPhoEta;

       int doMix;
       int nMaxEvents_minBiasMixing;
       int nCentralityBins;
       int nVertexBins;
       int nEventsToMix;
       if (configCuts.isValid) {
           jetCollection = configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].s[CUTS::JET::k_jetCollection].c_str();
           cutPhoEt = configCuts.proc[CUTS::kSKIM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_et];
           cutPhoEta = configCuts.proc[CUTS::kSKIM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_eta];

           doMix = configCuts.proc[CUTS::kSKIM].obj[CUTS::kGAMMAJET].i[CUTS::GJT::k_doMix];
           nMaxEvents_minBiasMixing = configCuts.proc[CUTS::kSKIM].obj[CUTS::kGAMMAJET].i[CUTS::GJT::k_nMaxEvents_minBiasMixing];
           nCentralityBins = configCuts.proc[CUTS::kSKIM].obj[CUTS::kGAMMAJET].i[CUTS::GJT::k_nCentralityBins];
           nVertexBins = configCuts.proc[CUTS::kSKIM].obj[CUTS::kGAMMAJET].i[CUTS::GJT::k_nVertexBins];
           nEventsToMix = configCuts.proc[CUTS::kSKIM].obj[CUTS::kGAMMAJET].i[CUTS::GJT::k_nEventsToMix];
       }
       else {
           jetCollection = "ak4PFJetAnalyzer";
           cutPhoEt = 15;
           cutPhoEta = 1.44;

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

       // verbose about configuration
       std::cout<<"Configuration :"<<std::endl;
       std::cout<<"jetCollection = "<<jetCollection.c_str()<<std::endl;

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

       std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.Data());

       std::cout<<"input ROOT files : num = "<<inputFiles.size()<< std::endl;
       std::cout<<"#####"<< std::endl;
       for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
           std::cout<<(*it).c_str()<< std::endl;
       }
       std::cout<<"##### END #####"<< std::endl;

//       bool isMC = collisionIsMC((COLL::TYPE)collision);
       bool isHI = collisionIsHI((COLL::TYPE)collision);

       TChain* treeHLT   = new TChain("hltanalysis/HltTree");
       TChain* treeggHiNtuplizer  = new TChain("ggHiNtuplizer/EventTree");
       TChain* treeEvent = new TChain("ggHiNtuplizer/EventTree");
       TChain* treeJet   = new TChain(Form("%s/t", jetCollection.c_str()));
       TChain* treeSkim  = new TChain("skimanalysis/HltTree");
       TChain* treeHiEvt;
       bool hasHiEvt = false;
       if (isHI) {
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

       treeEvent->SetBranchStatus("*",0);
       treeEvent->SetBranchStatus("run",1);
       treeEvent->SetBranchStatus("event",1);
       treeEvent->SetBranchStatus("lumis",1);

       treeJet->SetBranchStatus("*",0);        // disable all branches
       treeJet->SetBranchStatus("nref",1);     // enable jet branches
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

       treeSkim->SetBranchAddress("pHBHENoiseFilterResultProducer",&pHBHENoiseFilterResultProducer);
       if (treeSkim->GetBranch("pcollisionEventSelection")) {
           treeSkim->SetBranchAddress("pcollisionEventSelection",&pcollisionEventSelection);
       }
       else {   // overwrite to default
           pcollisionEventSelection = 1;
       }
       if (treeSkim->GetBranch("HBHEIsoNoiseFilterResult")) {

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

       // objects for gamma jet correlations
       ggHiNtuplizer ggHi;
       setupPhotonTree(treeggHiNtuplizer, ggHi);
       Jets jets;
       jets.setupTreeForReading(treeJet);

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

           TRandom3 rand(12345);    // random number seed should be fixed or reproducible
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

       TTree *gammaJetTree = new TTree("gammaJet","leading photon-jet correlations");
       gammaJetTree->SetMaxTreeSize(MAXTREESIZE);
       GammaJet gammajet;
       gammajet.resetAwayRange();
       gammajet.resetConeRange();
       branchGammaJetTree(gammaJetTree, gammajet);

       // mixed-event block
       Jets jetsMBoutput;     // object to write jet trees from MB events
       GammaJet gammajetMB;
       TTree *outputTreeJetMB = new TTree("jetsMB","Jets from minbias events");
       TTree *gammaJetTreeMB  = new TTree("gammaJetMB","leading photon-jet correlations from MB events");
       if (doMix>0) {
           outputTreeJetMB->SetMaxTreeSize(MAXTREESIZE);
           gammaJetTreeMB->SetMaxTreeSize(MAXTREESIZE);

           jetsMBoutput.setupTreeForWritingMB(outputTreeJetMB, false, false);

           gammajetMB.resetAwayRange();
           gammajetMB.resetConeRange();
           branchGammaJetTree(gammaJetTreeMB, gammajetMB);
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

           // photon-jet block
           // find leading photon
           int phoIdx = -1;     // index of the leading photon
           double maxPhoEt = -1;
           for(int i=0; i<ggHi.nPho; ++i)
           {
               bool failedEtCut  = (ggHi.phoEt->at(i) < cutPhoEt) ;
               bool failedEtaCut = (TMath::Abs(ggHi.phoEta->at(i)) > cutPhoEta) ;
               bool failedSpikeRejection = (ggHi.phoSigmaIEtaIEta->at(i) < 0.002 ||
                                            ggHi.pho_swissCrx->at(i)     > 0.9   ||
                                            TMath::Abs(ggHi.pho_seedTime->at(i)) > 3);

               bool failedHoverE = (ggHi.phoHoverE->at(i) > 0.2);      // <0.1 cut is applied after corrections
//               bool failedEnergyRatio = ((float)ggHi.phoSCRawE->at(i)/ggHi.phoE->at(i) < 0.5);

               if (failedEtCut)          continue;
               if (failedEtaCut)         continue;
               if (failedSpikeRejection) continue;
               if (failedHoverE)         continue;
//               if (failedEnergyRatio)    continue;    // actually applied after corrections

               if (ggHi.phoEt->at(i) > maxPhoEt)
               {
                   maxPhoEt = ggHi.phoEt->at(i);
                   phoIdx = i;
               }
           }
           if (phoIdx == -1) continue;
           entriesAnalyzed++;

           // photon-jet correlation
           // leading photon is correlated to each jet in the event.
           makeGammaJetPairs(ggHi, jets, gammajet, phoIdx);

           if(doMix > 0)
           {
               int centBin = hiBin / centBinWidth;
               int vzBin   = (vz+15) / vertexBinWidth;
               jetsMBoutput.nref = 0;

               clearGammaJetPairs(gammajetMB, phoIdx);
               if (nMB[centBin][vzBin] >= nEventsToMix)
               {
                   for (int i=0; i<nEventsToMix; ++i)
                   {
                       Long64_t entryMB = iterMB[centBin][vzBin] % nMB[centBin][vzBin];     // roll back to the beginning if out of range
                       treeJetMB[centBin][vzBin]->GetEntry(entryMB);

                       makeGammaJetPairsMB(ggHi, jetsMB, gammajetMB, phoIdx);

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

               gammaJetTreeMB->Fill();
               outputTreeJetMB->Fill();
           }

           outputTreeHLT->Fill();
           outputTreeggHiNtuplizer->Fill();
           outputTreeJet->Fill();
           if (hasHiEvt) outputTreeHiEvt->Fill();
           outputTreeSkim->Fill();
           
           gammaJetTree->Fill();
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
    
       std::cout << "gammaJetTree->GetEntries() = " << gammaJetTree->GetEntries() << std::endl;
       if (doMix > 0)
       {
           std::cout << "gammaJetTreeMB->GetEntries() = " << gammaJetTreeMB->GetEntries() << std::endl;
           std::cout << "outputTreeJetMB->GetEntries() = " << outputTreeJetMB->GetEntries() << std::endl;
       }
  
       configTree->Write("",TObject::kOverwrite);

       output->Write("",TObject::kOverwrite);
       output->Close();
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
