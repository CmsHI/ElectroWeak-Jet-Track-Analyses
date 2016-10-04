/*
 * macro to analyze and save di-Photon spectrum
 */

#include <TFile.h>
#include <TTree.h>
#include <TChain.h>

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

#include "../../CorrelationTuple/EventMatcher.h"
#include "../../TreeHeaders/diphotonTree.h"
#include "../../TreeHeaders/CutConfigurationTree.h"
#include "../../Utilities/interface/InputConfigurationParser.h"
#include "../../Utilities/interface/CutConfigurationParser.h"
#include "../../Utilities/interface/HiForestInfoController.h"

const long MAXTREESIZE = 500000000000; // set maximum tree size from 10 GB to 100 GB, so that the code does not switch to a new file after 10 GB7

void diphotonSkim(const TString configFile, const TString inputFile, const TString outputFile = "diphotonSkim.root");

void diphotonSkim(const TString configFile, const TString inputFile, const TString outputFile)
{
       std::cout<<"running diphotonSkim()"   <<std::endl;
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
       int isMC = configInput.proc[INPUT::kSKIM].i[INPUT::k_isMC];
       std::string treePath = configInput.proc[INPUT::kSKIM].s[INPUT::k_treePath];

       // set default values
       if (treePath.size() == 0)  treePath = "ggHiNtuplizer/EventTree";

       // verbose about input configuration
       std::cout<<"Input Configuration :"<<std::endl;
       std::cout << "isMC = " << isMC << std::endl;
       std::cout << "treePath = " << treePath.c_str() << std::endl;

       // cut configuration
       int cut_nPho = configCuts.proc[CUTS::kSKIM].obj[CUTS::kPHOTON].i[CUTS::PHO::k_nPhotons];
       float cut_phoSigmaIEtaIEta_spike = configCuts.proc[CUTS::kSKIM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_phoSigmaIEtaIEta_spike];
       float cut_pho_swissCrx = configCuts.proc[CUTS::kSKIM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pho_swissCrx];
       float cut_pho_seedTime = configCuts.proc[CUTS::kSKIM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pho_seedTime];

       int cut_nEle = configCuts.proc[CUTS::kSKIM].obj[CUTS::kELECTRON].i[CUTS::ELE::k_nEle];

       // verbose about cut configuration
       std::cout<<"Cut Configuration :"<<std::endl;
       std::cout<<"cut_nPho = "<<cut_nPho<<std::endl;
       std::cout<<"cut_phoSigmaIEtaIEta_spike = "<<cut_phoSigmaIEtaIEta_spike<<std::endl;   // not used
       std::cout<<"cut_pho_swissCrx           = "<<cut_pho_swissCrx<<std::endl;             // not used
       std::cout<<"cut_pho_seedTime           = "<<cut_pho_seedTime<<std::endl;             // not used

       std::cout<<"cut_nEle = "<<cut_nEle<<std::endl;

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
       if (isMC > 0) {
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

       // record HiForestInfo
       treeHiForestInfo->GetEntry(0);
       outputTreeHiForestInfo->Fill();

       TTree *diPhotonTree = new TTree("diphoton","photon pairs that match to an electron");
       diPhotonTree->SetMaxTreeSize(MAXTREESIZE);

       diphoton diPho;
       diPho.branchDiPhotonTree(diPhotonTree);

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

           // skip if there are no photons or electrons to study
           if(ggHi.nPho < cut_nPho || ggHi.nEle < cut_nEle)  continue;
           entriesAnalyzed++;

           diPho.makeDiPhotonPairs(ggHi);

           outputTreeHLT->Fill();
           outputTreeggHiNtuplizer->Fill();
           outputTreeHiEvt->Fill();
           diPhotonTree->Fill();
       }
       std::cout<< "Loop ENDED : " << treePath.c_str() <<std::endl;
       std::cout << "entries            = " << entries << std::endl;
       std::cout << "duplicateEntries   = " << duplicateEntries << std::endl;
       std::cout << "entriesAnalyzed    = " << entriesAnalyzed << std::endl;
       std::cout << "outputTreeHLT->GetEntries()           = " << outputTreeHLT->GetEntries() << std::endl;
       std::cout << "outputTreeggHiNtuplizer->GetEntries() = " << outputTreeggHiNtuplizer->GetEntries() << std::endl;
       std::cout << "outputTreeHiEvt->GetEntries() = " << outputTreeHiEvt->GetEntries() << std::endl;
       std::cout << "diPhotonTree->GetEntries()            = " << diPhotonTree->GetEntries() << std::endl;

       // overwrite existing trees
       outputTreeHLT->Write("", TObject::kOverwrite);
       outputTreeggHiNtuplizer->Write("", TObject::kOverwrite);
       outputTreeHiEvt->Write("", TObject::kOverwrite);
       diPhotonTree->Write("", TObject::kOverwrite);

       configTree->Write("", TObject::kOverwrite);

       output->Write("", TObject::kOverwrite);
       output->Close();

       std::cout<<"diphotonSkim() - END"   <<std::endl;
}

int main(int argc, char** argv)
{
    if (argc == 4)
    {
        diphotonSkim(argv[1], argv[2], argv[3]);
        return 0;
    }
    else if (argc == 3)
    {
        diphotonSkim(argv[1], argv[2]);
        return 0;
    }
    else
    {
        std::cout << "Usage : \n" <<
                "./diphotonSkim.exe <configFile> <inputFile> <outputFile>"
                << std::endl;
        return 1;
    }
}
