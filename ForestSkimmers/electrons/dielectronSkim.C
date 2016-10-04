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
       int collisionType = configInput.proc[INPUT::kSKIM].i[INPUT::k_collisionType];
       std::string treePath = configInput.proc[INPUT::kSKIM].s[INPUT::k_treePath];

       // set default values
       if (treePath.size() == 0)  treePath = "ggHiNtuplizer/EventTree";

       // verbose about input configuration
       std::cout<<"Input Configuration :"<<std::endl;
       std::cout << "collisionType = " << collisionType << std::endl;
       const char* collisionName =  getCollisionTypeName((COLL::TYPE)collisionType).c_str();
       std::cout << "collision = " << collisionName << std::endl;
       std::cout << "treePath = " << treePath.c_str() << std::endl;

       // cut configuration
       float cut_vz = configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].f[CUTS::EVT::k_vz];
       int cut_pcollisionEventSelection = configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].i[CUTS::EVT::k_pcollisionEventSelection];
       int cut_pPAprimaryVertexFilter = configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].i[CUTS::EVT::k_pPAprimaryVertexFilter];
       int cut_pBeamScrapingFilter = configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].i[CUTS::EVT::k_pBeamScrapingFilter];

       int cut_nEle = configCuts.proc[CUTS::kSKIM].obj[CUTS::kELECTRON].i[CUTS::ELE::k_nEle];
       int doCorrection = configCuts.proc[CUTS::kSKIM].obj[CUTS::kELECTRON].i[CUTS::ELE::k_doCorrection];

       // bool isMC = collisionIsMC((COLL::TYPE)collisionType);
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
       treeHiEvt->SetBranchStatus("*",1);

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

       electronCorrector corrector;
       if (doCorrection) {
           if (isHI) {
               std::string pathEB = "Corrections/electrons/weights/BDTG_EB_PbPb.weights.xml";
               std::string pathEE = "Corrections/electrons/weights/BDTG_EE_PbPb.weights.xml";
               corrector.initiliazeReader(pathEB.c_str(), pathEE.c_str());
           }
           else if (isPP) {
               std::string path = "Corrections/electrons/weights/gbrmva_pp_16V.root";
               corrector.initRegressionGBR(path);
           }
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

       TTree *diElectronTree = new TTree("dielectron","electron pairs");
       diElectronTree->SetMaxTreeSize(MAXTREESIZE);

       dielectron diEle;
       diEle.branchDiElectronTree(diElectronTree);

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

           // skip if there are no electron pairs to study
           if(ggHi.nEle < cut_nEle)  continue;
           entriesAnalyzed++;

           if(doCorrection > 0)
           {
               // correct the pt of electrons
               // note that "elePt" branch of "outputTreeggHiNtuplizer" will be corrected as well.
               if (isHI)  corrector.correctPtsregressionTMVA(ggHi, hiEvt.hiBin);
               else if (isPP) corrector.correctPtsregressionGBR(ggHi);
           }

           diEle.makeDiElectronPairs(ggHi);

           outputTreeHLT->Fill();
           outputTreeggHiNtuplizer->Fill();
           outputTreeHiEvt->Fill();
           outputTreeSkim->Fill();
           diElectronTree->Fill();
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
       std::cout << "diElectronTree->GetEntries()          = " << diElectronTree->GetEntries() << std::endl;

       // overwrite existing trees
       outputTreeHLT->Write("", TObject::kOverwrite);
       outputTreeggHiNtuplizer->Write("", TObject::kOverwrite);
       outputTreeHiEvt->Write("", TObject::kOverwrite);
       diElectronTree->Write("", TObject::kOverwrite);

       configTree->Write("", TObject::kOverwrite);

       output->Write("", TObject::kOverwrite);
       output->Close();

       std::cout<<"dielectronSkim() - END"   <<std::endl;
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
