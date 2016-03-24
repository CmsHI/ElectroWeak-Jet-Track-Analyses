/*
 * macro to skim jets in minBias events in centrality bins
 */

#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TString.h>

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include "../../CorrelationTuple/EventMatcher.h"
#include "../../TreeHeaders/CutConfigurationTree.h"
#include "../../Utilities/interface/CutConfigurationParser.h"
#include "../../Utilities/interface/InputConfigurationParser.h"
#include "../../Utilities/interface/HiForestInfoController.h"

const long MAXTREESIZE = 200000000000; // set maximum tree size from 10 GB to 100 GB, so that the code does not switch to a new file after 10 GB7

void minBiasJetSkim(const TString configFile, const TString inputFile, const TString outputFile = "minBiasJetSkim.root");

void minBiasJetSkim(const TString configFile, const TString inputFile, const TString outputFile)
{
       std::cout<<"running minBiasJetSkim()"<<std::endl;
       std::cout<<"configFile  = "<< configFile.Data() <<std::endl;
       std::cout<<"inputFile   = "<< inputFile.Data() <<std::endl;
       std::cout<<"outputFile  = "<< outputFile.Data() <<std::endl;

       CutConfiguration config = CutConfigurationParser::Parse(configFile.Data());
       float cut_vz;
       int cut_pcollisionEventSelection;

       std::vector<std::string> jetCollections;
       int nMaxEvents_minBiasMixing;
       int nCentralityBins;
       int nVertexBins;
       if (config.isValid) {
           cut_vz = config.proc[CUTS::kSKIM].obj[CUTS::kEVENT].f[CUTS::EVT::k_vz];
           cut_pcollisionEventSelection = config.proc[CUTS::kSKIM].obj[CUTS::kEVENT].i[CUTS::EVT::k_pcollisionEventSelection];

           jetCollections = ConfigurationParser::ParseList(config.proc[CUTS::kSKIM].obj[CUTS::kJET].s[CUTS::JET::k_jetCollection]);
           nMaxEvents_minBiasMixing = config.proc[CUTS::kSKIM].obj[CUTS::kGAMMAJET].i[CUTS::GJT::k_nMaxEvents_minBiasMixing];
           nCentralityBins = config.proc[CUTS::kSKIM].obj[CUTS::kGAMMAJET].i[CUTS::GJT::k_nCentralityBins];
           nVertexBins = config.proc[CUTS::kSKIM].obj[CUTS::kGAMMAJET].i[CUTS::GJT::k_nVertexBins];
       }
       else {
           cut_vz = 15;
           cut_pcollisionEventSelection = 1;
           nMaxEvents_minBiasMixing = 20000;
           nCentralityBins = 200;    // must divide 200 without remainders
           nVertexBins = 3;         // must divide 15  without remainders
       }
       int nJetCollections = jetCollections.size();

       // verbose about configuration
       std::cout<<"Cut Configuration :"<<std::endl;
       std::cout<<"cut_vz = "<< cut_vz <<std::endl;
       std::cout<<"cut_pcollisionEventSelection = "<< cut_pcollisionEventSelection <<std::endl;

       std::cout<<"nJetCollections = "<< nJetCollections <<std::endl;
       for (int i=0; i<nJetCollections; ++i) {
           std::cout << Form("jetCollections[%d] = %s", i, jetCollections.at(i).c_str()) << std::endl;
       }
       std::cout<<"nMaxEvents_minBiasMixing = "<< nMaxEvents_minBiasMixing <<std::endl;
       std::cout<<"nCentralityBins          = "<< nCentralityBins <<std::endl;
       std::cout<<"nVertexBins              = "<< nVertexBins <<std::endl;

       std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.Data());

       std::cout<<"input ROOT files : num = "<<inputFiles.size()<< std::endl;
       std::cout<<"#####"<< std::endl;
       for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
           std::cout<<(*it).c_str()<< std::endl;
       }
       std::cout<<"##### END #####"<< std::endl;

       TChain* treeHLT   = new TChain("hltanalysis/HltTree");
       TChain* treeEvent = new TChain("ggHiNtuplizer/EventTree");
       TChain* treeJets[nJetCollections];
       for (int i=0; i<nJetCollections; ++i) {
           treeJets[i] = new TChain(Form("%s/t", jetCollections.at(i).c_str()));
       }
       TChain* treeHiEvt = new TChain("hiEvtAnalyzer/HiTree");
       TChain* treeSkim  = new TChain("skimanalysis/HltTree");
       TChain* treeHiForestInfo = new TChain("HiForest/HiForestInfo");

       for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
          treeHLT->Add((*it).c_str());
          treeEvent->Add((*it).c_str());
          for (int i=0; i<nJetCollections; ++i) {
              treeJets[i]->Add((*it).c_str());
          }
          treeHiEvt->Add((*it).c_str());
          treeSkim->Add((*it).c_str());
          if (it == inputFiles.begin())  treeHiForestInfo->Add((*it).c_str());
       }

       HiForestInfoController hfic(treeHiForestInfo);
       std::cout<<"### HiForestInfo Tree ###"<< std::endl;
       hfic.printHiForestInfo();
       std::cout<<"###"<< std::endl;

       treeHLT->SetBranchStatus("*",0);     // disable all branches

       treeEvent->SetBranchStatus("*",0);
       treeEvent->SetBranchStatus("run",1);
       treeEvent->SetBranchStatus("event",1);
       treeEvent->SetBranchStatus("lumis",1);

       for (int i=0; i<nJetCollections; ++i) {
           treeJets[i]->SetBranchStatus("*",0);        // disable all branches
           treeJets[i]->SetBranchStatus("nref",1);     // enable jet branches
           treeJets[i]->SetBranchStatus("rawpt",1);    // enable jet branches
           treeJets[i]->SetBranchStatus("jtpt",1);     // enable jet branches
           treeJets[i]->SetBranchStatus("jteta",1);    // enable jet branches
           treeJets[i]->SetBranchStatus("jtphi",1);    // enable jet branches
           treeJets[i]->SetBranchStatus("jty",1);      // enable jet branches
           treeJets[i]->SetBranchStatus("jtpu",1);     // enable jet branches
           treeJets[i]->SetBranchStatus("jtm",1);      // enable jet branches
           treeJets[i]->SetBranchStatus("track*",1);
           treeJets[i]->SetBranchStatus("charged*",1);
           treeJets[i]->SetBranchStatus("photon*",1);
           treeJets[i]->SetBranchStatus("neutral*",1);
           treeJets[i]->SetBranchStatus("eMax*",1);
           treeJets[i]->SetBranchStatus("eSum*",1);
           treeJets[i]->SetBranchStatus("eN*",1);
           treeJets[i]->SetBranchStatus("muMax*",1);
           treeJets[i]->SetBranchStatus("muSum*",1);
           treeJets[i]->SetBranchStatus("muN*",1);
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

       float vz;
       Int_t hiBin;

       treeHiEvt->SetBranchAddress("vz",&vz);
       treeHiEvt->SetBranchAddress("hiBin",&hiBin);

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

       if (treeSkim->GetBranch("pcollisionEventSelection")) {
           treeSkim->SetBranchAddress("pcollisionEventSelection",&pcollisionEventSelection);
       }
       else {   // overwrite to default
           pcollisionEventSelection = 1;
           std::cout<<"could not get branch : pcollisionEventSelection"<<std::endl;
           std::cout<<"set to default value : pcollisionEventSelection = "<<pcollisionEventSelection<<std::endl;
       }

       // event information
       UInt_t run, lumis;
       ULong64_t event;
       treeEvent->SetBranchAddress("run", &run);
       treeEvent->SetBranchAddress("event", &event);
       treeEvent->SetBranchAddress("lumis", &lumis);

       TFile* output = new TFile(outputFile,"RECREATE");

       TTree *configTree = setupConfigurationTreeForWriting(config);

       int centBinWidth = 200/nCentralityBins;  // number of "hiBin"s that a centrality bin covers
       int vertexBinWidth = 30/nVertexBins;     // number of "vz"s    that a vertex     bin covers
                                                // accepted vz range is -15 to 15.

       TTree *outputTreesHLT[nCentralityBins][nVertexBins];
       TTree *outputTreesEvent[nCentralityBins][nVertexBins];
       TTree *outputTreesJet[nCentralityBins][nVertexBins][nJetCollections];
       TTree *outputTreesHiEvt[nCentralityBins][nVertexBins];
       TTree *outputTreesSkim[nCentralityBins][nVertexBins];

       Long64_t nEntriesFilled[nCentralityBins][nVertexBins];    // number of events read for a centrality bin
       for(int i=0; i<nCentralityBins; ++i)
       {
           for(int j=0; j<nVertexBins; ++j){
               nEntriesFilled[i][j] = 0;

               outputTreesHLT[i][j] = treeHLT->CloneTree(0);
               outputTreesHLT[i][j]->SetName(Form("hltTree_centBin%d_vzBin%d", i, j));
               outputTreesHLT[i][j]->SetTitle(Form("subbranches of hltanalysis/HltTree for centrality bin %d vz bin %d", i, j));
               outputTreesHLT[i][j]->SetMaxTreeSize(MAXTREESIZE);

               outputTreesEvent[i][j] = treeEvent->CloneTree(0);
               outputTreesEvent[i][j]->SetName(Form("EventTree_centBin%d_vzBin%d", i, j));
               outputTreesEvent[i][j]->SetTitle(Form("subbranches of ggHiNtuplizer/EventTree for centrality bin %d vz bin %d", i, j));
               outputTreesEvent[i][j]->SetMaxTreeSize(MAXTREESIZE);

               for(int k=0; k<nJetCollections; ++k){
                   std::string jetCollectionName = jetCollections.at(k).c_str();
                   outputTreesJet[i][j][k] = treeJets[k]->CloneTree(0);
                   outputTreesJet[i][j][k]->SetName(Form("%s_centBin%d_vzBin%d", jetCollectionName.c_str(), i, j));
                   outputTreesJet[i][j][k]->SetTitle(Form("subbranches of %s/t for centrality bin %d vz bin %d", jetCollectionName.c_str(), i, j));
                   outputTreesJet[i][j][k]->SetMaxTreeSize(MAXTREESIZE);
               }

               outputTreesHiEvt[i][j] = treeHiEvt->CloneTree(0);
               outputTreesHiEvt[i][j]->SetName(Form("HiEvt_centBin%d_vzBin%d", i, j));
               outputTreesHiEvt[i][j]->SetTitle(Form("subbranches of hiEvtAnalyzer/HiTree for centrality bin %d vz bin %d", i, j));
               outputTreesHiEvt[i][j]->SetMaxTreeSize(MAXTREESIZE);

               outputTreesSkim[i][j] = treeSkim->CloneTree(0);
               outputTreesSkim[i][j]->SetName(Form("skim_centBin%d_vzBin%d", i, j));
               outputTreesSkim[i][j]->SetTitle(Form("subbranches of skimanalysis/HltTree for centrality bin %d vz bin %d", i, j));
               outputTreesSkim[i][j]->SetMaxTreeSize(MAXTREESIZE);
           }
       }
//       TTree* outputTreeHiForestInfo = treeHiForestInfo->CloneTree(0);
//       outputTreeHiForestInfo->SetName("HiForestInfo");
//       outputTreeHiForestInfo->SetTitle("first entry of HiForest/HiForestInfo");
//
//       outputTreeHiForestInfo->SetMaxTreeSize(MAXTREESIZE);
//
//       // write HiForestInfo
//       treeHiForestInfo->GetEntry(0);
//       outputTreeHiForestInfo->Fill();

       EventMatcher* em = new EventMatcher();
       Long64_t duplicateEntries = 0;

       Long64_t entries = treeEvent->GetEntries();
       Long64_t entriesAnalyzed = 0;
       std::cout << "entries         = " << entries << std::endl;
       std::cout<< "Loop : ggHiNtuplizer/EventTree" <<std::endl;
       for (Long64_t j_entry=0; j_entry<entries; ++j_entry)
       {
           if (j_entry % 20000 == 0)  {
             std::cout << "current entry = " <<j_entry<<" out of "<<entries<<" : "<<std::setprecision(2)<<(double)j_entry/entries*100<<" %"<<std::endl;
           }

           treeHLT->GetEntry(j_entry);
           treeEvent->GetEntry(j_entry);
           for (int i = 0; i<nJetCollections; ++i) {
               treeJets[i]->GetEntry(j_entry);
           }
           treeHiEvt->GetEntry(j_entry);
           treeSkim->GetEntry(j_entry);

           bool eventAdded = em->addEvent(run,lumis,event,j_entry);
           if(!eventAdded) // this event is duplicate, skip this one.
           {
               duplicateEntries++;
               continue;
           }

           // event selection
           if(!(TMath::Abs(vz) < cut_vz)) continue;
           if(pcollisionEventSelection < cut_pcollisionEventSelection) continue;
           entriesAnalyzed++;

           int centBin = hiBin / centBinWidth;
           int vzBin   = (vz+15) / vertexBinWidth;

           if (nEntriesFilled[centBin][vzBin] > nMaxEvents_minBiasMixing) continue;

           outputTreesHLT[centBin][vzBin]->Fill();
           outputTreesEvent[centBin][vzBin]->Fill();
           for (int i = 0; i<nJetCollections; ++i) {
               outputTreesJet[centBin][vzBin][i]->Fill();
           }
           outputTreesHiEvt[centBin][vzBin]->Fill();
           outputTreesSkim[centBin][vzBin]->Fill();

           nEntriesFilled[centBin][vzBin]++;
       }
       std::cout<<  "Loop ENDED : ggHiNtuplizer/EventTree" <<std::endl;
       std::cout << "entries            = " << entries << std::endl;
       std::cout << "duplicateEntries   = " << duplicateEntries << std::endl;
       std::cout << "entriesAnalyzed    = " << entriesAnalyzed << std::endl;
    
       for(int i=0; i<nCentralityBins; ++i){
           for(int j=0; j<nVertexBins; ++j){
               std::cout<<Form("outputTreesJet[%d][%d]->GetEntries() = %lld", i, j, outputTreesEvent[i][j]->GetEntries())<<std::endl;
               if (outputTreesEvent[i][j]->GetEntries() < 100) {
                   std::cout<< "Be careful : less than 100 events were put into centBin = "<<i<<" , vertexBin = "<<j<<std::endl;
               }
           }
       }
  
       configTree->Write("",TObject::kOverwrite);

       output->Write("",TObject::kOverwrite);
       output->Close();
}

int main(int argc, char** argv)
{
    if (argc == 4) {
        minBiasJetSkim(argv[1], argv[2], argv[3]);
        return 0;
    }
    else if (argc == 3) {
        minBiasJetSkim(argv[1], argv[2]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./minBiasJetSkim.exe <configFile> <inputFile> <outputFile>"
                << std::endl;
        return 1;
    }
}
