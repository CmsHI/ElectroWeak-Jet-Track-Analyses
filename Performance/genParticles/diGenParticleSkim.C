/*
 * macro to analyze and save di-GenParticle spectrum
 */

#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TMath.h>

#include <vector>
#include <algorithm>    // std::find
#include <string>
#include <iostream>
#include <iomanip>

#include "../../CorrelationTuple/EventMatcher.h"
#include "../../TreeHeaders/diMCTree.h"
#include "../../TreeHeaders/CutConfigurationTree.h"
#include "../../Utilities/interface/InputConfigurationParser.h"
#include "../../Utilities/interface/CutConfigurationParser.h"
#include "../../Utilities/interface/HiForestInfoController.h"

const long MAXTREESIZE = 500000000000; // set maximum tree size from 10 GB to 100 GB, so that the code does not switch to a new file after 10 GB7

void diGenParticleSkim(const TString configFile, const TString inputFile, const TString outputFile = "diGenParticleSkim.root");

void diGenParticleSkim(const TString configFile, const TString inputFile, const TString outputFile)
{
       std::cout<<"running diGenParticleSkim()"   <<std::endl;
       std::cout<<"configFile  = "<< configFile.Data() <<std::endl;
       std::cout<<"inputFile   = "<< inputFile.Data() <<std::endl;
       std::cout<<"outputFile  = "<< outputFile.Data() <<std::endl;

       InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
       CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

       if (!configInput.isValid) {
           std::cout << "Input configuration is invalid." << std::endl;
           std::cout << "exiting" << std::endl;
           return;
       }
       if (!configCuts.isValid) {
           std::cout << "Cut configuration is invalid." << std::endl;
           std::cout << "exiting" << std::endl;
           return;
       }

       // input configuration
       std::string treePath = configInput.proc[INPUT::kSKIM].s[INPUT::k_treePath];

       // set default values
       if (treePath.size() == 0)  treePath = "ggHiNtuplizer/EventTree";

       // verbose about input configuration
       std::cout<<"Input Configuration :"<<std::endl;
       std::cout << "treePath = " << treePath.c_str() << std::endl;

       // cut configuration
       float cut_mcPt = configCuts.proc[CUTS::kSKIM].obj[CUTS::kGENPARTICLE].f[CUTS::GEN::k_mcPt];
       float cut_mcEta = configCuts.proc[CUTS::kSKIM].obj[CUTS::kGENPARTICLE].f[CUTS::GEN::k_mcEta];

       int cut_nMC = configCuts.proc[CUTS::kSKIM].obj[CUTS::kGENPARTICLE].i[CUTS::GEN::k_nMC];

       std::vector<int> mcPIDs  = ConfigurationParser::ParseListInteger(
               configCuts.proc[CUTS::kSKIM].obj[CUTS::kGENPARTICLE].s[CUTS::GEN::k_mcPID_List]);
       std::vector<int> mcMomPIDs  = ConfigurationParser::ParseListInteger(
               configCuts.proc[CUTS::kSKIM].obj[CUTS::kGENPARTICLE].s[CUTS::GEN::k_mcMomPID_List]);

       bool do_cut_mcPt = (cut_mcPt > 0);
       bool do_cut_mcEta = (cut_mcEta > 0);

       int nMcPIDs = mcPIDs.size();
       int nMcMomPIDs = mcMomPIDs.size();
       // verbose about cut configuration
       std::cout<<"Cut Configuration :"<<std::endl;
       std::cout<<"cut_mcPt  = "<< cut_mcPt <<std::endl;
       std::cout<<"cut_mcEta = "<< cut_mcEta <<std::endl;
       std::cout<<"cut_nMC   = "<< cut_nMC <<std::endl;
       std::cout << "nMcPIDs = " << nMcPIDs << std::endl;
       for (int i=0; i<nMcPIDs; ++i) {
           if (i == 0)  std::cout << "mcPIDs = {";
           std::cout << " " << mcPIDs.at(i);
           if (i == nMcPIDs-1)  std::cout << " }" << std::endl;
       }
       std::cout << "nMcMomPIDs = " << nMcMomPIDs << std::endl;
       for (int i=0; i<nMcMomPIDs; ++i) {
           if (i == 0)  std::cout << "mcMomPIDs = {";
           std::cout << " " << mcMomPIDs.at(i);
           if (i == nMcMomPIDs-1)  std::cout << " }" << std::endl;
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
       TChain* treeHiEvt = new TChain("hiEvtAnalyzer/HiTree");
       TChain* treeHiForestInfo = new TChain("HiForest/HiForestInfo");

       for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
          treeHLT->Add((*it).c_str());
          treeggHiNtuplizer->Add((*it).c_str());
          treeHiEvt->Add((*it).c_str());
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
       
       // specify explicitly which branches to store, do not use wildcard
       treeHiEvt->SetBranchStatus("*",0);
       treeHiEvt->SetBranchStatus("run",1);
       treeHiEvt->SetBranchStatus("evt",1);
       treeHiEvt->SetBranchStatus("lumi",1);
       treeHiEvt->SetBranchStatus("vz",1);
       treeHiEvt->SetBranchStatus("hiBin",1);
       treeHiEvt->SetBranchStatus("hiHF",1);
       treeHiEvt->SetBranchStatus("hiNevtPlane",1);
       // include MC branches
       treeHiEvt->SetBranchStatus("Npart",1);
       treeHiEvt->SetBranchStatus("Ncoll",1);
       treeHiEvt->SetBranchStatus("Nhard",1);
       treeHiEvt->SetBranchStatus("ProcessID",1);
       treeHiEvt->SetBranchStatus("pthat",1);
       treeHiEvt->SetBranchStatus("weight",1);
       treeHiEvt->SetBranchStatus("alphaQCD",1);
       treeHiEvt->SetBranchStatus("alphaQED",1);
       treeHiEvt->SetBranchStatus("qScale",1);
       
       ggHiNtuplizer ggHi;
       ggHi.setupTreeForReading(treeggHiNtuplizer);

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

       TTree *diMCTree = new TTree("diMC","gen particle pairs");
       diMCTree->SetMaxTreeSize(MAXTREESIZE);

       diMC diMC;
       diMC.branchDiMCTree(diMCTree);

       EventMatcher* em = new EventMatcher();
       Long64_t duplicateEntries = 0;

       Long64_t entries = treeggHiNtuplizer->GetEntries();
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

           bool eventAdded = em->addEvent(ggHi.run,ggHi.lumis,ggHi.event,j_entry);
           if(!eventAdded) // this event is duplicate, skip this one.
           {
               duplicateEntries++;
               continue;
           }

           if(ggHi.nMC < cut_nMC)  continue;
           entriesAnalyzed++;

           diMC.clearDiMCPairs();
           diMC.nMC_out = ggHi.nMC;
           for(int i=0; i< ggHi.nMC; ++i) {

               if (do_cut_mcPt  && !(ggHi.mcPt->at(i) > cut_mcPt))  continue;
               if (do_cut_mcEta && !(TMath::Abs(ggHi.mcEta->at(i)) < cut_mcEta))  continue;

               for(int j=i+1; j< ggHi.nMC; ++j) {

                   if (do_cut_mcPt  && !(ggHi.mcPt->at(j) > cut_mcPt))  continue;
                   if (do_cut_mcEta && !(TMath::Abs(ggHi.mcEta->at(j)) < cut_mcEta))  continue;

                   // make Gen Particles pairs only from particles whose PID is in the input list.
                   // make pairs by default if no PID list is provided.
                   bool passedPID_1 = true;
                   bool passedPID_2 = true;
                   if (nMcPIDs > 0) {
                       passedPID_1 = std::find(mcPIDs.begin(), mcPIDs.end(), ggHi.mcPID->at(i)) != mcPIDs.end();
                       passedPID_2 = std::find(mcPIDs.begin(), mcPIDs.end(), ggHi.mcPID->at(j)) != mcPIDs.end();
                   }
                   if (!passedPID_1 || !passedPID_2)  continue;

                   // make Gen Particles pairs only from particles whose mother PID is in the input list.
                   bool passedMomPID_1 = true;
                   bool passedMomPID_2 = true;
                   if (nMcMomPIDs > 0) {
                       passedMomPID_1 = std::find(mcMomPIDs.begin(), mcMomPIDs.end(), ggHi.mcMomPID->at(i)) != mcMomPIDs.end();
                       passedMomPID_2 = std::find(mcMomPIDs.begin(), mcMomPIDs.end(), ggHi.mcMomPID->at(j)) != mcMomPIDs.end();
                   }
                   if (!passedMomPID_1 || !passedMomPID_2)  continue;

                   diMC.pushbackDiMCPair(ggHi, i, j);
               }
           }

           outputTreeHLT->Fill();
           outputTreeggHiNtuplizer->Fill();
           outputTreeHiEvt->Fill();
           diMCTree->Fill();
       }
       std::cout<< "Loop ENDED : " << treePath.c_str() <<std::endl;
       std::cout << "entries            = " << entries << std::endl;
       std::cout << "duplicateEntries   = " << duplicateEntries << std::endl;
       std::cout << "entriesAnalyzed    = " << entriesAnalyzed << std::endl;
       std::cout << "outputTreeHLT->GetEntries()           = " << outputTreeHLT->GetEntries() << std::endl;
       std::cout << "outputTreeggHiNtuplizer->GetEntries() = " << outputTreeggHiNtuplizer->GetEntries() << std::endl;
       std::cout << "outputTreeHiEvt->GetEntries() = " << outputTreeHiEvt->GetEntries() << std::endl;
       std::cout << "diMCTree->GetEntries()        = " << diMCTree->GetEntries() << std::endl;

       // overwrite existing trees
       outputTreeHLT->Write("", TObject::kOverwrite);
       outputTreeggHiNtuplizer->Write("", TObject::kOverwrite);
       outputTreeHiEvt->Write("", TObject::kOverwrite);
       diMCTree->Write("", TObject::kOverwrite);

       configTree->Write("", TObject::kOverwrite);

       output->Write("", TObject::kOverwrite);
       output->Close();
}

int main(int argc, char** argv)
{
    if (argc == 4)
    {
        diGenParticleSkim(argv[1], argv[2], argv[3]);
        return 0;
    }
    else if (argc == 3)
    {
        diGenParticleSkim(argv[1], argv[2]);
        return 0;
    }
    else
    {
        std::cout << "Usage : \n" <<
                "./diGenParticleSkim.exe <configFile> <inputFile> <outputFile>"
                << std::endl;
        return 1;
    }
}
