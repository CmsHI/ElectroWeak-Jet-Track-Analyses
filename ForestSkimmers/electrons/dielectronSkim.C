/*
 * macro to analyze and save di-Electron spectrum
 */

#include <TFile.h>
#include <TTree.h>
#include <TChain.h>

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

#include "../../CorrelationTuple/EventMatcher.h"
#include "../../TreeHeaders/dielectronTree.h"
#include "../../TreeHeaders/hiEvtTree.h"
#include "../../TreeHeaders/CutConfigurationTree.h"
#include "../../Utilities/interface/InputConfigurationParser.h"
#include "../../Utilities/interface/CutConfigurationParser.h"
#include "../../Utilities/interface/HiForestInfoController.h"
#include "../../Corrections/electrons/electronCorrector.h"

const long MAXTREESIZE = 500000000000; // set maximum tree size from 10 GB to 100 GB, so that the code does not switch to a new file after 10 GB7

void dielectronSkim(const TString configFile, const TString inputFile, const TString outputFile = "dielectronSkim.root");

void dielectronSkim(const TString configFile, const TString inputFile, const TString outputFile)
{
       std::cout<<"running dielectronSkim()"   <<std::endl;
       std::cout<<"configFile  = "<< configFile.Data() <<std::endl;
       std::cout<<"inputFile   = "<< inputFile.Data() <<std::endl;
       std::cout<<"outputFile  = "<< outputFile.Data() <<std::endl;

       InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
       CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

       // input configuration
       int isMC;
       std::string treePath;
       if (configInput.isValid) {
           isMC = configInput.proc[INPUT::kSKIM].i[INPUT::k_isMC];
           treePath = configInput.proc[INPUT::kSKIM].s[INPUT::k_treePath];
       }
       else {
           isMC = 0;
           treePath = "ggHiNtuplizer/EventTree";
       }
       // set default values
       if (treePath.size() == 0)  treePath = "ggHiNtuplizer/EventTree";

       // verbose about input configuration
       std::cout<<"Input Configuration :"<<std::endl;
       std::cout << "isMC = " << isMC << std::endl;
       std::cout << "treePath = " << treePath.c_str() << std::endl;

       // cut configuration
       int cut_nEle;
       int doCorrection;
       if (configCuts.isValid) {
           cut_nEle = configCuts.proc[CUTS::kSKIM].obj[CUTS::kELECTRON].i[CUTS::ELE::k_nEle];
           doCorrection = configCuts.proc[CUTS::kSKIM].obj[CUTS::kELECTRON].i[CUTS::ELE::k_doCorrection];
       }
       else {
           cut_nEle = 2;
           doCorrection = 0;
       }

       // verbose about cut configuration
       std::cout<<"Cut Configuration :"<<std::endl;
       std::cout<<"cut_nEle = "<<cut_nEle<<std::endl;
       std::cout<<"doCorrection = "<<doCorrection<<std::endl;

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

       hiEvt hiEvt;
       hiEvt.setupTreeForReading(treeHiEvt);

       electronCorrector corrector;
       if (doCorrection) {
           std::string pathEB = "Corrections/electrons/weights/BDTG_EB_PbPb.weights.xml";
           std::string pathEE = "Corrections/electrons/weights/BDTG_EE_PbPb.weights.xml";
           corrector.initiliazeReader(pathEB.c_str(), pathEE.c_str());
       }

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

       TTree *diElectronTree = new TTree("dielectron","electron pairs");
       diElectronTree->SetMaxTreeSize(MAXTREESIZE);

       dielectron diEle;
       diEle.branchDiElectronTree(diElectronTree);

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

           // skip if there are no electron pairs to study
           if(ggHi.nEle < cut_nEle)  continue;
           entriesAnalyzed++;

           if(doCorrection > 0)
           {
               // correct the pt of electrons
               // note that "elePt" branch of "outputTreeggHiNtuplizer" will be corrected as well.
               corrector.correctPtsregressionTMVA(ggHi, hiEvt.hiBin);
           }

           diEle.makeDiElectronPairs(ggHi);

           outputTreeHLT->Fill();
           outputTreeggHiNtuplizer->Fill();
           outputTreeHiEvt->Fill();
           diElectronTree->Fill();
       }
       std::cout<< "Loop ENDED : " << treePath.c_str() <<std::endl;
       std::cout << "entries            = " << entries << std::endl;
       std::cout << "duplicateEntries   = " << duplicateEntries << std::endl;
       std::cout << "entriesAnalyzed    = " << entriesAnalyzed << std::endl;
       std::cout << "outputTreeHLT->GetEntries()           = " << outputTreeHLT->GetEntries() << std::endl;
       std::cout << "outputTreeggHiNtuplizer->GetEntries() = " << outputTreeggHiNtuplizer->GetEntries() << std::endl;
       std::cout << "outputTreeHiEvt->GetEntries() = " << outputTreeHiEvt->GetEntries() << std::endl;
       std::cout << "diElectronTree->GetEntries()          = " << diElectronTree->GetEntries() << std::endl;

       // overwrite existing trees
       outputTreeHLT->Write("", TObject::kOverwrite);
       outputTreeggHiNtuplizer->Write("", TObject::kOverwrite);
       outputTreeHiEvt->Write("", TObject::kOverwrite);
       diElectronTree->Write("", TObject::kOverwrite);

       configTree->Write("", TObject::kOverwrite);

       output->Write("", TObject::kOverwrite);
       output->Close();
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
