/*
 * macro to analyze and save di-Muon spectrum
 */

#include <TFile.h>
#include <TTree.h>
#include <TChain.h>

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

#include "../../CorrelationTuple/EventMatcher.h"
#include "../../TreeHeaders/dimuonTree.h"
#include "../../TreeHeaders/hiEvtTree.h"
#include "../../TreeHeaders/CutConfigurationTree.h"
#include "../../Utilities/interface/InputConfigurationParser.h"
#include "../../Utilities/interface/CutConfigurationParser.h"
#include "../../Utilities/interface/HiForestInfoController.h"

const long MAXTREESIZE = 500000000000; // set maximum tree size from 10 GB to 100 GB, so that the code does not switch to a new file after 10 GB7

void dimuonSkim(const TString configFile, const TString inputFile, const TString outputFile = "dimuonSkim.root");

void dimuonSkim(const TString configFile, const TString inputFile, const TString outputFile)
{
       std::cout<<"running dimuonSkim()"   <<std::endl;
       std::cout<<"configFile  = "<< configFile.Data() <<std::endl;
       std::cout<<"inputFile   = "<< inputFile.Data() <<std::endl;
       std::cout<<"outputFile  = "<< outputFile.Data() <<std::endl;

       InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
       CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

       // input configuration
       int collisionType;
       std::string treePath;
       if (configInput.isValid) {
           collisionType = configInput.proc[INPUT::kSKIM].i[INPUT::k_collisionType];
           treePath = configInput.proc[INPUT::kSKIM].s[INPUT::k_treePath];
       }
       else {
           collisionType = COLL::kPP;
           treePath = "ggHiNtuplizer/EventTree";
       }
       // set default values
       if (treePath.size() == 0)  treePath = "ggHiNtuplizer/EventTree";

       // verbose about input configuration
       std::cout<<"Input Configuration :"<<std::endl;
       std::cout << "collisionType = " << collisionType << std::endl;
       const char* collisionName =  getCollisionTypeName((COLL::TYPE)collisionType).c_str();
       std::cout << "collision = " << collisionName << std::endl;
       std::cout << "treePath = " << treePath.c_str() << std::endl;

       // cut configuration
       float cut_vz;
       int cut_pcollisionEventSelection;
       int cut_pPAprimaryVertexFilter;
       int cut_pBeamScrapingFilter;

       int cut_nMu;
       if (configCuts.isValid) {
           cut_vz = configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].f[CUTS::EVT::k_vz];
           cut_pcollisionEventSelection = configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].i[CUTS::EVT::k_pcollisionEventSelection];
           cut_pPAprimaryVertexFilter = configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].i[CUTS::EVT::k_pPAprimaryVertexFilter];
           cut_pBeamScrapingFilter = configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].i[CUTS::EVT::k_pBeamScrapingFilter];

           cut_nMu = configCuts.proc[CUTS::kSKIM].obj[CUTS::kMUON].i[CUTS::MUO::k_nMu];
       }
       else {
           cut_vz = 15;
           cut_pcollisionEventSelection = 1;
           cut_pPAprimaryVertexFilter = 1;
           cut_pBeamScrapingFilter = 1;

           cut_nMu = 2;
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

       std::cout<<"cut_nMu = "<<cut_nMu<<std::endl;

       std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.Data());

       std::cout<<"input ROOT files : num = "<<inputFiles.size()<< std::endl;
       std::cout<<"#####"<< std::endl;
       for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
           std::cout<<(*it).c_str()<< std::endl;
       }
       std::cout<<"##### END #####"<< std::endl;

       TChain* treeHLT   = new TChain("hltanalysis/HltTree");
       TChain* treeggHiNtuplizer  = new TChain("ggHiNtuplizer/EventTree");
       TChain* treeHiEvt = new TChain("hiEvtAnalyzer/HiTree");
       TChain* treeSkim  = new TChain("skimanalysis/HltTree");
       TChain* treeHiForestInfo = new TChain("HiForest/HiForestInfo");

       for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
          treeHLT->Add((*it).c_str());
          treeggHiNtuplizer->Add((*it).c_str());
          treeHiEvt->Add((*it).c_str());
          treeSkim->Add((*it).c_str());
          treeHiForestInfo->Add((*it).c_str());
       }

       HiForestInfoController hfic(treeHiForestInfo);
       std::cout<<"### HiForestInfo Tree ###"<< std::endl;
       hfic.printHiForestInfo();
       std::cout<<"###"<< std::endl;

       treeHLT->SetBranchStatus("*",0);     // disable all branches
       treeHLT->SetBranchStatus("HLT_HI*SinglePhoton*Eta*",1);     // enable photon branches
       treeHLT->SetBranchStatus("HLT_HI*DoublePhoton*Eta*",1);     // enable photon branches
       treeHLT->SetBranchStatus("*DoubleMu*",1);                      // enable muon branches
       treeHLT->SetBranchStatus("HLT_HIL1Mu*",1);                     // enable muon branches
       treeHLT->SetBranchStatus("HLT_HIL2Mu*",1);                     // enable muon branches
       treeHLT->SetBranchStatus("HLT_HIL3Mu*",1);                     // enable muon branches
       
       // specify explicitly which branches to store, do not use wildcard
       treeHiEvt->SetBranchStatus("*",0);
       treeHiEvt->SetBranchStatus("run",1);
       treeHiEvt->SetBranchStatus("evt",1);
       treeHiEvt->SetBranchStatus("lumi",1);
       treeHiEvt->SetBranchStatus("vz",1);
       treeHiEvt->SetBranchStatus("hiBin",1);
       treeHiEvt->SetBranchStatus("hiHF",1);
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

       ggHiNtuplizer ggHi;
       ggHi.setupTreeForReading(treeggHiNtuplizer);
       
       hiEvt hiEvt;
       hiEvt.setupTreeForReading(treeHiEvt);

       TFile* output = new TFile(outputFile,"RECREATE");
       TTree *configTree = setupConfigurationTreeForWriting(configCuts);

       // output tree variables
       TTree *outputTreeHLT           = treeHLT->CloneTree(0);
       outputTreeHLT->SetName("hltTree");
       outputTreeHLT->SetTitle("subbranches of hltanalysis/HltTree");
       TTree *outputTreeggHiNtuplizer = treeggHiNtuplizer->CloneTree(0);
       TTree *outputTreeHiEvt = treeHiEvt->CloneTree(0);
       outputTreeHiEvt->SetName("HiEvt");
       outputTreeHiEvt->SetTitle("subbranches of hiEvtAnalyzer/HiTree");
       TTree* outputTreeSkim  = treeSkim->CloneTree(0);
       outputTreeSkim->SetName("skim");
       outputTreeSkim->SetTitle("subbranches of skimanalysis/HltTree");
       TTree* outputTreeHiForestInfo = treeHiForestInfo->CloneTree(0);
       outputTreeHiForestInfo->SetName("HiForestInfo");
       outputTreeHiForestInfo->SetTitle("first entry of HiForest/HiForestInfo");
       
       outputTreeHLT->SetMaxTreeSize(MAXTREESIZE);
       outputTreeggHiNtuplizer->SetMaxTreeSize(MAXTREESIZE);
       outputTreeHiEvt->SetMaxTreeSize(MAXTREESIZE);
       outputTreeHiForestInfo->SetMaxTreeSize(MAXTREESIZE);

       // write HiForestInfo
       treeHiForestInfo->GetEntry(0);
       outputTreeHiForestInfo->Fill();

       TTree *diMuonTree = new TTree("dimuon","muon pairs");
       diMuonTree->SetMaxTreeSize(MAXTREESIZE);

       dimuon diMu;
       diMu.branchDiMuonTree(diMuonTree);

       EventMatcher* em = new EventMatcher();
       Long64_t duplicateEntries = 0;

       Long64_t entries = treeggHiNtuplizer->GetEntries();
       Long64_t entriesPassedEventSelection = 0;
       Long64_t entriesAnalyzed = 0;
       std::cout << "entries = " << entries << std::endl;
       std::cout<< "Loop : " << treePath.c_str() <<std::endl;
       for (Long64_t j_entry=0; j_entry<entries; ++j_entry)
       {
           if (j_entry % 20000 == 0)  {
             std::cout << "current entry = " <<j_entry<<" out of "<<entries<<" : "<<std::setprecision(2)<<(double)j_entry/entries*100<<" %"<<std::endl;
           }

           treeHLT->GetEntry(j_entry);
           treeggHiNtuplizer->GetEntry(j_entry);
           treeHiEvt->GetEntry(j_entry);
           treeSkim->GetEntry(j_entry);

           bool eventAdded = em->addEvent(ggHi.run,ggHi.lumis,ggHi.event,j_entry);
           if(!eventAdded) // this event is duplicate, skip this one.
           {
               duplicateEntries++;
               continue;
           }

           // event selection
           if (!(TMath::Abs(hiEvt.vz) < cut_vz))  continue;
           if (isHI) {
               if ((pcollisionEventSelection < cut_pcollisionEventSelection))  continue;
           }
           else {
               if (pPAprimaryVertexFilter < cut_pPAprimaryVertexFilter || pBeamScrapingFilter < cut_pBeamScrapingFilter)  continue;
           }
           entriesPassedEventSelection++;

           // skip if there are no muon pairs to study
           if(ggHi.nMu < cut_nMu)  continue;
           entriesAnalyzed++;

           diMu.makeDiMuonPairs(ggHi);

           outputTreeHLT->Fill();
           outputTreeggHiNtuplizer->Fill();
           outputTreeHiEvt->Fill();
           outputTreeSkim->Fill();
           diMuonTree->Fill();
       }
       std::cout<< "Loop ENDED : " << treePath.c_str() <<std::endl;
       std::cout << "entries            = " << entries << std::endl;
       std::cout << "duplicateEntries   = " << duplicateEntries << std::endl;
       std::cout << "entriesPassedEventSelection   = " << entriesPassedEventSelection << std::endl;
       std::cout << "entriesAnalyzed    = " << entriesAnalyzed << std::endl;
       std::cout << "outputTreeHLT->GetEntries()           = " << outputTreeHLT->GetEntries() << std::endl;
       std::cout << "outputTreeggHiNtuplizer->GetEntries() = " << outputTreeggHiNtuplizer->GetEntries() << std::endl;
       std::cout << "outputTreeHiEvt->GetEntries() = " << outputTreeHiEvt->GetEntries() << std::endl;
       std::cout << "outputTreeSkim->GetEntries()  = " << outputTreeSkim->GetEntries() << std::endl;
       std::cout << "diMuonTree->GetEntries()          = " << diMuonTree->GetEntries() << std::endl;

       // overwrite existing trees
       outputTreeHLT->Write("", TObject::kOverwrite);
       outputTreeggHiNtuplizer->Write("", TObject::kOverwrite);
       outputTreeHiEvt->Write("", TObject::kOverwrite);
       diMuonTree->Write("", TObject::kOverwrite);

       configTree->Write("", TObject::kOverwrite);

       output->Write("", TObject::kOverwrite);
       output->Close();

       std::cout<<"dimuonSkim() - END"   <<std::endl;
}

int main(int argc, char** argv)
{
    if (argc == 4)
    {
        dimuonSkim(argv[1], argv[2], argv[3]);
        return 0;
    }
    else if (argc == 3)
    {
        dimuonSkim(argv[1], argv[2]);
        return 0;
    }
    else
    {
        std::cout << "Usage : \n" <<
                "./dimuonSkim.exe <configFile> <inputFile> <outputFile>"
                << std::endl;
        return 1;
    }
}
