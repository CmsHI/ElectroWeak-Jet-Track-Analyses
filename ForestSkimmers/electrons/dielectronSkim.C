/*
 * macro to analyze and save di-Electron spectrum
 */

#include <TFile.h>
#include <TTree.h>

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

#include "../../CorrelationTuple/EventMatcher.h"
#include "../../TreeHeaders/dielectronTree.h"
#include "../../CutConfigurations/interface/CutConfigurationsParser.h"
#include "../../TreeHeaders/CutConfigurationTree.h"

const long MAXTREESIZE = 200000000000; // set maximum tree size from 10 GB to 100 GB, so that the code does not switch to a new file after 10 GB7

void dielectronSkim(const char* configFile, const char* inputFile, const char* outputFile = "dielectronSkim.root");

void dielectronSkim(const char* configFile, const char* inputFile, const char* outputFile)
{
       std::cout<<"running dielectronSkim()"   <<std::endl;
       std::cout<<"configFile = "<< configFile  <<std::endl;
       std::cout<<"inputFile  = "<< inputFile  <<std::endl;
       std::cout<<"outputFile = "<< outputFile <<std::endl;

       std::string eventTreePath = "ggHiNtuplizer/EventTree";

       TFile* input = new TFile(inputFile, "READ");

       TTree* treeHLT            = (TTree*)input->Get("hltanalysis/HltTree");
       TTree* ggHiNtuplizerTree  = (TTree*)input->Get("ggHiNtuplizer/EventTree");

       treeHLT->SetBranchStatus("*",0);     // disable all branches
       treeHLT->SetBranchStatus("HLT_HI*SinglePhoton*Eta*v1*",1);     // enable photon branches
       treeHLT->SetBranchStatus("HLT_HI*DoublePhoton*Eta*v1*",1);     // enable photon branches
       treeHLT->SetBranchStatus("*DoubleMu*",1);                      // enable muon branches

       ggHiNtuplizer ggHi;
       setupPhotonTree(ggHiNtuplizerTree, ggHi);

       TFile* output = new TFile(outputFile,"UPDATE");

       CutConfiguration config = CutConfigurationsParser::Parse(configFile);
       TTree* configTree = setupConfigurationTreeForWriting(config);
       int cut_nEle;
       if (config.isValid) {
           cut_nEle = config.proc[CUTS::kSKIM].obj[CUTS::kELECTRON].i[CUTS::ELE::k_nEle];
       }
       else {
           cut_nEle = 2;
       }

       std::cout<<"cut_nEle = "<<cut_nEle<<std::endl;

       // output tree variables
       TTree *outputTreeHLT           = treeHLT->CloneTree(0);
       TTree *outputTreeggHiNtuplizer = ggHiNtuplizerTree->CloneTree(0);

       TTree *diElectronTree = new TTree("dielectron","electron pairs");
       diElectronTree->SetMaxTreeSize(MAXTREESIZE);

       dielectron diEle;
       branchDiElectronTree(diElectronTree, diEle);

       EventMatcher* em = new EventMatcher();
       Long64_t duplicateEntries = 0;

       Long64_t entries = ggHiNtuplizerTree->GetEntries();
       Long64_t entriesAnalyzed = 0;
       std::cout << "entries         = " << entries << std::endl;
       std::cout<< "Loop : " << eventTreePath.c_str() <<std::endl;
       for (int j_entry=0; j_entry<entries; ++j_entry)
       {
           if (j_entry % 20000 == 0)  {
             std::cout << "current entry = " <<j_entry<<" out of "<<entries<<" : "<<std::setprecision(2)<<(double)j_entry/entries*100<<" %"<<std::endl;
           }

           treeHLT->GetEntry(j_entry);
           ggHiNtuplizerTree->GetEntry(j_entry);

           bool eventAdded = em->addEvent(ggHi.run,ggHi.lumis,ggHi.event,j_entry);
           if(!eventAdded) // this event is duplicate, skip this one.
           {
               duplicateEntries++;
               continue;
           }

           // skip if there are no electron to study
           if(ggHi.nEle < cut_nEle)  continue;
           entriesAnalyzed++;

           makeDiElectronPairs(ggHi,diEle);

           outputTreeHLT->Fill();
           outputTreeggHiNtuplizer->Fill();
           diElectronTree->Fill();
       }
       std::cout<< "Loop ENDED : " << eventTreePath.c_str() <<std::endl;
       std::cout << "entries            = " << entries << std::endl;
       std::cout << "duplicateEntries   = " << duplicateEntries << std::endl;
       std::cout << "entriesAnalyzed    = " << entriesAnalyzed << std::endl;
       std::cout << "outputTreeHLT->GetEntries()           = " << outputTreeHLT->GetEntries() << std::endl;
       std::cout << "outputTreeggHiNtuplizer->GetEntries() = " << outputTreeggHiNtuplizer->GetEntries() << std::endl;
       std::cout << "diElectronTree->GetEntries()          = " << diElectronTree->GetEntries() << std::endl;

       // overwrite existing trees
       outputTreeHLT->Write("", TObject::kOverwrite);
       outputTreeggHiNtuplizer->Write("", TObject::kOverwrite);
       diElectronTree->Write("", TObject::kOverwrite);

       configTree->Write("", TObject::kOverwrite);

       output->Write("", TObject::kOverwrite);
       output->Close();
       input->Close();
}

int main(int argc, char** argv)
{
    if (argc == 4)
    {
        dielectronSkim(argv[1], argv[2], argv[3]);
        return 0;
    }
    else if (argc == 3)
    {
        dielectronSkim(argv[1], argv[2]);
        return 0;
    }
    else
    {
        std::cout << "Usage : \n" <<
                "./dielectronSkim.exe <configFile> <inputFile> <outputFile>"
                << std::endl;
        return 1;
    }
}
