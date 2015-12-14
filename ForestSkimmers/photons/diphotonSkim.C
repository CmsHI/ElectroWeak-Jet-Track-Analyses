/*
 * macro to analyze and save di-Photon spectrum
 */

#include <TFile.h>
#include <TTree.h>

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

#include "../../CorrelationTuple/EventMatcher.h"
#include "../../TreeHeaders/diphotonTree.h"
#include "../../CutConfigurations/interface/CutConfigurationsParser.h"
#include "../../TreeHeaders/CutConfigurationTree.h"

const long MAXTREESIZE = 500000000000; // set maximum tree size from 10 GB to 100 GB, so that the code does not switch to a new file after 10 GB7

void diphotonSkim(const TString configFile, const TString inputFile, const TString outputFile = "diphotonSkim.root");

void diphotonSkim(const TString configFile, const TString inputFile, const TString outputFile)
{
       std::cout<<"running diphotonSkim()"   <<std::endl;
       std::cout<<"configFile  = "<< configFile.Data() <<std::endl;
       std::cout<<"inputFile   = "<< inputFile.Data() <<std::endl;
       std::cout<<"outputFile  = "<< outputFile.Data() <<std::endl;

       std::string eventTreePath = "ggHiNtuplizer/EventTree";

       TFile* input = new TFile(inputFile.Data(), "READ");

       TTree* treeHLT            = (TTree*)input->Get("hltanalysis/HltTree");
       TTree* ggHiNtuplizerTree  = (TTree*)input->Get("ggHiNtuplizer/EventTree");

       treeHLT->SetBranchStatus("*",0);     // disable all branches
       treeHLT->SetBranchStatus("HLT_HI*SinglePhoton*Eta*v1*",1);     // enable photon branches
       treeHLT->SetBranchStatus("HLT_HI*DoublePhoton*Eta*v1*",1);     // enable photon branches
       treeHLT->SetBranchStatus("*DoubleMu*",1);                      // enable muon branches

       ggHiNtuplizer ggHi;
       setupPhotonTree(ggHiNtuplizerTree, ggHi);

       TFile* output = new TFile(outputFile.Data(),"UPDATE");

       CutConfiguration config = CutConfigurationsParser::Parse(configFile.Data());
       TTree* configTree = setupConfigurationTreeForWriting(config);
       int cut_nPho;
       int cut_nEle;
       if (config.isValid) {
           cut_nPho = config.proc[CUTS::kSKIM].obj[CUTS::kPHOTON].i[CUTS::PHO::k_nPhotons];
           cut_nEle = config.proc[CUTS::kSKIM].obj[CUTS::kELECTRON].i[CUTS::ELE::k_nEle];
       }
       else {
           cut_nPho = 2;
           cut_nEle = 2;
       }

       std::cout<<"cut_nPho = "<<cut_nPho<<std::endl;
       std::cout<<"cut_nEle = "<<cut_nEle<<std::endl;

       // output tree variables
       TTree *outputTreeHLT           = treeHLT->CloneTree(0);
       TTree *outputTreeggHiNtuplizer = ggHiNtuplizerTree->CloneTree(0);
       outputTreeggHiNtuplizer->SetMaxTreeSize(MAXTREESIZE);

       TTree *diPhotonTree = new TTree("diphoton","photon pairs that match to an electron");
       diPhotonTree->SetMaxTreeSize(MAXTREESIZE);

       diphoton diPho;
       branchDiPhotonTree(diPhotonTree, diPho);

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

           // skip if there are no photons or electrons to study
           if(ggHi.nPho < cut_nPho || ggHi.nEle < cut_nEle)  continue;
           entriesAnalyzed++;

           makeDiPhotonPairs(ggHi, diPho);

           outputTreeHLT->Fill();
           outputTreeggHiNtuplizer->Fill();
           diPhotonTree->Fill();
       }
       std::cout<< "Loop ENDED : " << eventTreePath.c_str() <<std::endl;
       std::cout << "entries            = " << entries << std::endl;
       std::cout << "duplicateEntries   = " << duplicateEntries << std::endl;
       std::cout << "entriesAnalyzed    = " << entriesAnalyzed << std::endl;
       std::cout << "outputTreeHLT->GetEntries()           = " << outputTreeHLT->GetEntries() << std::endl;
       std::cout << "outputTreeggHiNtuplizer->GetEntries() = " << outputTreeggHiNtuplizer->GetEntries() << std::endl;
       std::cout << "diPhotonTree->GetEntries()            = " << diPhotonTree->GetEntries() << std::endl;

       // overwrite existing trees
       outputTreeHLT->Write("", TObject::kOverwrite);
       outputTreeggHiNtuplizer->Write("", TObject::kOverwrite);
       diPhotonTree->Write("", TObject::kOverwrite);

       configTree->Write("", TObject::kOverwrite);

       output->Write("", TObject::kOverwrite);
       output->Close();
       input->Close();
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
