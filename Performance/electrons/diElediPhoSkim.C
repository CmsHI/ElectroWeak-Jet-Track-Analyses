/*
 * macro to analyze and save di-Electron spectrum together with the
 * di-Photon spectrum which consist of photons that match to the electrons one by one.
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
#include "../../TreeHeaders/diphotonTree.h"
#include "../../TreeHeaders/CutConfigurationTree.h"
#include "../../Utilities/interface/InputConfigurationParser.h"
#include "../../Utilities/interface/CutConfigurationParser.h"
#include "../../Utilities/interface/HiForestInfoController.h"
#include "../../Plotting/commonUtility.h"
#include "../../Corrections/electrons/electronCorrector.h"

const long MAXTREESIZE = 500000000000; // set maximum tree size from 10 GB to 100 GB, so that the code does not switch to a new file after 10 GB7

void diElediPhoSkim(const TString configFile, const TString inputFile, const TString outputFile = "diElediPhoSkim.root");

void diElediPhoSkim(const TString configFile, const TString inputFile, const TString outputFile)
{
       std::cout<<"running diElediPhoSkim()"   <<std::endl;
       std::cout<<"configFile  = "<< configFile.Data() <<std::endl;
       std::cout<<"inputFile   = "<< inputFile.Data() <<std::endl;
       std::cout<<"outputFile  = "<< outputFile.Data() <<std::endl;

       InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
       CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

       // input configuration
       std::string treePath;
       if (configInput.isValid) {
           treePath = configInput.proc[INPUT::kSKIM].s[INPUT::k_treePath];
       }
       else {
           treePath = "ggHiNtuplizer/EventTree";
       }
       // set default values
       if (treePath.size() == 0)  treePath = "ggHiNtuplizer/EventTree";

       // verbose about input configuration
       std::cout<<"Input Configuration :"<<std::endl;
       std::cout << "treePath = " << treePath.c_str() << std::endl;

       // cut configuration
       int cut_nPho;

       int cut_nEle;
       int doCorrection;
       if (configCuts.isValid) {
           cut_nPho = configCuts.proc[CUTS::kSKIM].obj[CUTS::kPHOTON].i[CUTS::PHO::k_nPhotons];

           cut_nEle = configCuts.proc[CUTS::kSKIM].obj[CUTS::kELECTRON].i[CUTS::ELE::k_nEle];
           doCorrection = configCuts.proc[CUTS::kSKIM].obj[CUTS::kELECTRON].i[CUTS::ELE::k_doCorrection];
       }
       else {
           cut_nPho = 2;

           cut_nEle = 2;
           doCorrection = 0;
       }

       // verbose about cut configuration
       std::cout<<"Cut Configuration :"<<std::endl;
       std::cout<<"cut_nPho = "<<cut_nPho<<std::endl;

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

       ggHiNtuplizer ggHi;
       ggHi.setupTreeForReading(treeggHiNtuplizer);

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

       // record HiForestInfo
       treeHiForestInfo->GetEntry(0);
       outputTreeHiForestInfo->Fill();

       TTree *diElectronTree = new TTree("dielectron","electron pairs");
       TTree *diPhotonTree = new TTree("diphoton","photon pairs that match to an electron");

       diElectronTree->SetMaxTreeSize(MAXTREESIZE);
       diPhotonTree->SetMaxTreeSize(MAXTREESIZE);

       dielectron diEle;
       diEle.branchDiElectronTree(diElectronTree);

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

           if(doCorrection > 0)
           {
               // correct the pt of electrons
               // note that "elePt" branch of "outputTreeggHiNtuplizer" will be corrected as well.
               corrector.correctPts(ggHi);
           }

           // electron-photon matching
           // at most one photon can match to electron. If there are multiple photons, pick the one with highest energy.
           // based on  https://github.com/CmsHI/HiForestAnalysis/blob/master/PhotonUtilities.C#L154-L186
           std::vector<int> matched_phoIndices;
           for(int i=0; i< ggHi.nEle; ++i) {
               float currentMaxPt=-1;
               int matched_Index =-1;
               for (int j=0; j< ggHi.nPho; ++j) {
                   if (ggHi.phoEt->at(j) < currentMaxPt) continue;
                   if (getDR(ggHi.phoEta->at(j), ggHi.phoPhi->at(j), ggHi.eleEta->at(i), ggHi.elePhi->at(i))>diPho.cutDeltaR) continue;

                   matched_Index=j;
                   currentMaxPt=ggHi.phoEt->at(j);
               }

               matched_phoIndices.push_back(matched_Index);
           }

           diEle.makeDiElectronPairs(ggHi);
           // make the diphoton pairs from photons that matched to the electron pairs
           diPho.clearDiPhotonPairs();
           diPho.nPho_out = ggHi.nPho;
           for(int i=0; i< ggHi.nEle; ++i) {
               for(int j=i+1; j< ggHi.nEle; ++j) {

                   int i1 = matched_phoIndices.at(i);
                   int i2 = matched_phoIndices.at(j);

                   int matched_Index_1 = i;
                   int matched_Index_2 = j;

                   // if there is no photon matched to the electron, still make a diphoton pair albeit dummy
                   // for sake of keeping the indices aligned.
                   if (i1 < 0) {
                       i1 = 0;
                       matched_Index_1 = -1;
                   }
                   if (i2 < 0) {
                       i2 = 0;
                       matched_Index_2 = -1;
                   }

                   diPho.makeDiPhotonPair(ggHi, i1, i2, matched_Index_1, matched_Index_2);
               }
           }

           outputTreeHLT->Fill();
           outputTreeggHiNtuplizer->Fill();
           outputTreeHiEvt->Fill();
           diElectronTree->Fill();
           diPhotonTree->Fill();
       }
       std::cout<< "Loop ENDED : " << treePath.c_str() <<std::endl;
       std::cout << "entries            = " << entries << std::endl;
       std::cout << "duplicateEntries   = " << duplicateEntries << std::endl;
       std::cout << "entriesAnalyzed    = " << entriesAnalyzed << std::endl;
       std::cout << "outputTreeHLT->GetEntries()           = " << outputTreeHLT->GetEntries() << std::endl;
       std::cout << "outputTreeggHiNtuplizer->GetEntries() = " << outputTreeggHiNtuplizer->GetEntries() << std::endl;
       std::cout << "outputTreeHiEvt->GetEntries() = " << outputTreeHiEvt->GetEntries() << std::endl;
       std::cout << "diElectronTree->GetEntries()          = " << diElectronTree->GetEntries() << std::endl;
       std::cout << "diPhotonTree->GetEntries()            = " << diPhotonTree->GetEntries() << std::endl;

       // overwrite existing trees
       outputTreeHLT->Write("", TObject::kOverwrite);
       outputTreeggHiNtuplizer->Write("", TObject::kOverwrite);
       outputTreeHiEvt->Write("", TObject::kOverwrite);
       diElectronTree->Write("", TObject::kOverwrite);
       diPhotonTree->Write("", TObject::kOverwrite);

       configTree->Write("", TObject::kOverwrite);

       output->Write("", TObject::kOverwrite);
       output->Close();
}

int main(int argc, char** argv)
{
    if (argc == 4)
    {
        diElediPhoSkim(argv[1], argv[2], argv[3]);
        return 0;
    }
    else if (argc == 3)
    {
        diElediPhoSkim(argv[1], argv[2]);
        return 0;
    }
    else
    {
        std::cout << "Usage : \n" <<
                "./diElediPhoSkim.exe <configFile> <inputFile> <outputFile>"
                << std::endl;
        return 1;
    }
}
