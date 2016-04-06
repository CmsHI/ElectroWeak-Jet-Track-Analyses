#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TRandom3.h>

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

#include "../../CorrelationTuple/EventMatcher.h"
#include "../../TreeHeaders/ggHiNtuplizerTree.h"
#include "../../TreeHeaders/CutConfigurationTree.h"
#include "../../Plotting/commonUtility.h"
#include "../../Utilities/interface/CutConfigurationParser.h"
#include "../../Utilities/interface/InputConfigurationParser.h"
#include "../../Utilities/interface/HiForestInfoController.h"

const long MAXTREESIZE = 2000000000000; // set maximum tree size from 10 GB to 1862 GB, so that the code does not switch to a new file after 10 GB

void photonRaaSkim(const TString configFile, const TString inputFile, const TString outputFile = "photonRaaSkim.root", COLL::TYPE colli = COLL::kPP);
float xSecCal(const char* fname_lowestPthat, TChain* mergedTree, float pthat_i, float pthat_f);

void photonRaaSkim(const TString configFile, const TString inputFile, const TString outputFile, COLL::TYPE colli)
{
    std::cout<<"running photonRaaSkim()"<<std::endl;
    std::cout<<"configFile  = "<< configFile.Data() <<std::endl;
    std::cout<<"inputFile   = "<< inputFile.Data() <<std::endl;
    std::cout<<"outputFile  = "<< outputFile.Data() <<std::endl;

    InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

    // input configuration
/*    int collisionType;
    if (configInput.isValid) {
        collisionType = configInput.proc[INPUT::kSKIM].i[INPUT::k_collisionType];
    }
    else {
        collisionType = COLL::kPP;
    }
*/
    // verbose about input configuration
    int collisionType = colli;
    std::cout<<"Input Configuration :"<<std::endl;
    std::cout << "collisionType = " << collisionType << std::endl;
    const char* collisionName =  getCollisionTypeName((COLL::TYPE)collisionType).c_str();
    std::cout << "collision = " << collisionName << std::endl;

    // cut configuration
    float cut_vz;
    int cut_pcollisionEventSelection;
    int cut_pPAprimaryVertexFilter;
    int cut_pBeamScrapingFilter;

    float cutPhoEt;
    float cutPhoEta;

    if (configCuts.isValid) {
        cut_vz = configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].f[CUTS::EVT::k_vz];
        cut_pcollisionEventSelection = configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].i[CUTS::EVT::k_pcollisionEventSelection];
        cut_pPAprimaryVertexFilter = configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].i[CUTS::EVT::k_pPAprimaryVertexFilter];
        cut_pBeamScrapingFilter = configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].i[CUTS::EVT::k_pBeamScrapingFilter];

        cutPhoEt = configCuts.proc[CUTS::kSKIM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_et];
        cutPhoEta = configCuts.proc[CUTS::kSKIM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_eta];

    }
    else {
        cut_vz = 15;
        cut_pcollisionEventSelection = 1;
        cut_pPAprimaryVertexFilter = 1;
        cut_pBeamScrapingFilter = 1;

        cutPhoEt = 15;
        cutPhoEta = 1.44;
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

    std::cout<<"cutPhoEt  = "<<cutPhoEt<<std::endl;
    std::cout<<"cutPhoEta = "<<cutPhoEta<<std::endl;

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
    TChain* treeGen=0;
    if(isMC) treeGen  = new TChain("HiGenParticleAna/hi");

    // pthatWeight Calculation block! 
    int nPthat = 5;
    float pthatCut[nPthat+1];
    const char* lowestPthatFileName="";
    int nfiles = 0;
    for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
        treeHLT->Add((*it).c_str());
        treeggHiNtuplizer->Add((*it).c_str());
        treeHiEvt->Add((*it).c_str());
        treeSkim->Add((*it).c_str());
        treeHiForestInfo->Add((*it).c_str());
        if(isMC) treeGen->Add((*it).c_str());
        if(isMC && (nfiles==0)) { 
            lowestPthatFileName = (*it).c_str(); 
            TString str(lowestPthatFileName);
            cout << "lowestPthatFileName = " << lowestPthatFileName << endl;
            if(str.Contains("15")) {
                float temp[] = {15,30,50,80,120,9999};          
                for(int j=0;j<nPthat+1;j++){
                    pthatCut[j] = temp[j];
                }
            } else if(str.Contains("30")) {
                float temp[] = {30,50,80,120,170,9999};          
                for(int j=0;j<nPthat+1;j++){
                    pthatCut[j] = temp[j];
                }
            }
        }
        nfiles++;
    }

    float tmpWeight[nPthat];
    if(isMC) {
        for(int j=0; j<nPthat ; j++){
            tmpWeight[j] = xSecCal(lowestPthatFileName,treeHiEvt, pthatCut[j], pthatCut[j+1]);
            cout << collisionName << ", pthatWeight of " << pthatCut[j] << " to " << pthatCut[j+1] << " = " << tmpWeight[j] << endl;
        }
    }
/*
    HiForestInfoController hfic(treeHiForestInfo);
    std::cout<<"### HiForestInfo Tree ###"<< std::endl;
    hfic.printHiForestInfo();
    std::cout<<"###"<< std::endl;
*/
    treeHLT->SetBranchStatus("*",0);     // disable all branches
    treeHLT->SetBranchStatus("HLT_HI*SinglePhoton*Eta*",1);     // enable photon branches
    treeHLT->SetBranchStatus("HLT_HI*DoublePhoton*Eta*",1);     // enable photon branches

    float vz;
    Int_t hiBin;
    UInt_t run, lumis;
    ULong64_t event;
    float pthat, pthatWeight;
    treeHiEvt->SetBranchAddress("vz",&vz);
    treeHiEvt->SetBranchAddress("hiBin",&hiBin);
    treeHiEvt->SetBranchAddress("run", &run);
    treeHiEvt->SetBranchAddress("evt", &event);
    treeHiEvt->SetBranchAddress("lumi", &lumis);
    if(isMC) {
        //treeHiEvt->Branch("pthatWeight", &pthatWeight, "pthatWeight/F");
        treeHiEvt->SetBranchAddress("pthat", &pthat);
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

    // objects for z-jet correlations
    ggHiNtuplizer ggHi;
    ggHi.setupTreeForReading(treeggHiNtuplizer);    // treeggHiNtuplizer is input
    treeggHiNtuplizer->SetBranchStatus("*",0);
    treeggHiNtuplizer->SetBranchStatus("run",1);
    treeggHiNtuplizer->SetBranchStatus("event",1);
    treeggHiNtuplizer->SetBranchStatus("lumis",1);
    treeggHiNtuplizer->SetBranchStatus("nPho",1);
    treeggHiNtuplizer->SetBranchStatus("pho*",1);
    treeggHiNtuplizer->SetBranchStatus("pf*",1);
    //treeggHiNtuplizer->SetBranchStatus("tower*",1);
    if(isMC) treeggHiNtuplizer->SetBranchStatus("mc*",1);



    TFile* output = TFile::Open(outputFile,"RECREATE");
    TTree* configTree = setupConfigurationTreeForWriting(configCuts);

    // output tree variables
    TTree *outputTreeHLT=0, *outputTreeggHiNtuplizer=0, 
          *outputTreeHiEvt=0, *outputTreeSkim=0, *outputTreeHiForestInfo=0, *outputTreeGen=0;

    // output tree variables
    outputTreeHLT    = treeHLT->CloneTree(0);
    outputTreeggHiNtuplizer = treeggHiNtuplizer->CloneTree(0);
    outputTreeHiEvt = treeHiEvt->CloneTree(0);
    outputTreeSkim   = treeSkim->CloneTree(0);
    outputTreeHiForestInfo = treeHiForestInfo->CloneTree(0);
    if(isMC) outputTreeGen = treeGen ->CloneTree(0);

    outputTreeSkim->SetName("skimTree");
    outputTreeHLT->SetMaxTreeSize(MAXTREESIZE);
    outputTreeggHiNtuplizer->SetMaxTreeSize(MAXTREESIZE);
    outputTreeHiEvt->SetMaxTreeSize(MAXTREESIZE);
    outputTreeSkim->SetMaxTreeSize(MAXTREESIZE);
    outputTreeHiForestInfo->SetMaxTreeSize(MAXTREESIZE);
    if(isMC) outputTreeGen->SetMaxTreeSize(MAXTREESIZE);

    if(isMC) outputTreeHiEvt->Branch("pthatWeight", &pthatWeight, "pthatWeight/F");
  

    /////// Event Matching for DATA ///////
    EventMatcher* em = new EventMatcher();
    Long64_t duplicateEntries = 0;
    Long64_t entriesPassedEventSelection =0;
    Long64_t entriesAnalyzed =0;
    Long64_t entriesSpikeRejected=0;
    Long64_t entries = treeggHiNtuplizer->GetEntries();
    std::cout << "entries = " << entries << std::endl;
    std::cout<< "Loop : ggHiNtuplizer/EventTree" <<std::endl;
    //for (Long64_t j_entry=10000; j_entry<10100; ++j_entry)
    for (Long64_t j_entry=0; j_entry<entries; ++j_entry)
    {
        if (j_entry % 2000 == 0)  {
            std::cout << "current entry = " <<j_entry<<" out of "<<entries<<" : "<<std::setprecision(2)<<(double)j_entry/entries*100<<" %"<<std::endl;
        }

        treeHLT->GetEntry(j_entry);
        treeggHiNtuplizer->GetEntry(j_entry);
        treeSkim->GetEntry(j_entry);
        treeHiEvt->GetEntry(j_entry);
        treeHiForestInfo->GetEntry(j_entry);
        if(isMC) treeGen->GetEntry(j_entry);

        bool eventAdded = em->addEvent(run,lumis,event,j_entry);
        //std::cout << run << " " << lumis << " " << event << " " << j_entry << std::endl;
        if(!eventAdded) // this event is duplicate, skip this one.
        {
            duplicateEntries++;
            continue;
        }

        if(isMC) {
            if((pthat>=pthatCut[0]) && (pthat<pthatCut[1])) pthatWeight = tmpWeight[0];
            else if((pthat>=pthatCut[1]) && (pthat<pthatCut[2])) pthatWeight = tmpWeight[1];
            else if((pthat>=pthatCut[2]) && (pthat<pthatCut[3])) pthatWeight = tmpWeight[2];
            else if((pthat>=pthatCut[3]) && (pthat<pthatCut[4])) pthatWeight = tmpWeight[3];
            else if((pthat>=pthatCut[4]) && (pthat<pthatCut[5])) pthatWeight = tmpWeight[4];
            else continue;
        }
        // event selection
        if (!(TMath::Abs(vz) < cut_vz))  continue;
        if (isHI) {
            if ((pcollisionEventSelection < cut_pcollisionEventSelection))  continue;
        }
        else {
            if (pPAprimaryVertexFilter < cut_pPAprimaryVertexFilter || pBeamScrapingFilter < cut_pBeamScrapingFilter)  continue;
        }
        entriesPassedEventSelection++;

        // photon block
        // find leading photon
        int phoIdx = -1;     // index of the leading photon
        double maxPhoEt = -1;
        for(int i=0; i<ggHi.nPho; ++i)
        {
            bool failedEtCut  = (ggHi.phoEt->at(i) < cutPhoEt) ;
            bool failedEtaCut = (TMath::Abs(ggHi.phoEta->at(i)) > cutPhoEta) ;
            bool failedSpikeRejection;
            bool failedHotSpotRejection;
            //if (isHI) {
            failedSpikeRejection =( (ggHi.phoEta->at(i)<1.44) && 
                    (ggHi.phoSigmaIEtaIEta->at(i) < 0.002 ||
                     ggHi.pho_swissCrx->at(i)     > 0.9   ||
                     TMath::Abs(ggHi.pho_seedTime->at(i)) > 3) );
            // }
            // else {
            //     failedSpikeRejection = (ggHi.phoSigmaIEtaIEta->at(i) < 0.002);
            // }

            failedHotSpotRejection = (
                (ggHi.phoE3x3->at(i)/ggHi.phoE5x5->at(i) > 2./3.-0.03 && ggHi.phoE3x3->at(i)/ggHi.phoE5x5->at(i) < 2./3.+0.03) &&
                (ggHi.phoE1x5->at(i)/ggHi.phoE5x5->at(i) > 1./3.-0.03 && ggHi.phoE1x5->at(i)/ggHi.phoE5x5->at(i) < 1./3.+0.03) &&
                (ggHi.phoE2x5->at(i)/ggHi.phoE5x5->at(i) > 2./3.-0.03 && ggHi.phoE2x5->at(i)/ggHi.phoE5x5->at(i) < 2./3.+0.03) );
            
            bool failedHoverE = (ggHi.phoHoverE->at(i) > 0.2);      // <0.1 cut is applied after corrections
            //               bool failedEnergyRatio = ((float)ggHi.phoSCRawE->at(i)/ggHi.phoE->at(i) < 0.5);

            if (failedEtCut)          continue;
            if (failedEtaCut)         continue;
            if (failedSpikeRejection) continue;
            if (failedHotSpotRejection) {entriesSpikeRejected++; continue;}
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

        outputTreeHLT->Fill();
        outputTreeggHiNtuplizer->Fill();
        outputTreeHiEvt->Fill();
        outputTreeSkim->Fill();
        outputTreeHiForestInfo->Fill();
        if(isMC) outputTreeGen->Fill();
    }// event loop closed here

    std::cout<<  "Loop ENDED : ggHiNtuplizer/EventTree" <<std::endl;
    std::cout << "entries            = " << entries << std::endl;
    std::cout << "duplicateEntries   = " << duplicateEntries << std::endl;
    std::cout << "entriesPassedEventSelection   = " << entriesPassedEventSelection << std::endl;
    std::cout << "entriesAnalyzed               = " << entriesAnalyzed << std::endl;
    std::cout << "entriesSpikeRejected          = " << entriesSpikeRejected << std::endl;
    std::cout << "outputTreeHLT->GetEntries()   = " << outputTreeHLT->GetEntries() << std::endl;
    std::cout << "outputTreeggHiNtuplizer->GetEntries()   = " << outputTreeggHiNtuplizer->GetEntries() << std::endl;
    std::cout << "outputTreeSkim->GetEntries()  = " << outputTreeSkim->GetEntries() << std::endl;
    std::cout << "outputTreeHiEvt->GetEntries() = " << outputTreeHiEvt->GetEntries() << std::endl;
    std::cout << "outputTreeHiForestInfo->GetEntries() = " << outputTreeHiForestInfo->GetEntries() << std::endl;
    if(isMC) std::cout << "outputTreeGen->GetEntries() = " << outputTreeGen->GetEntries() << std::endl;


    output->cd();
    configTree->Write("",TObject::kOverwrite);
    output->Write("",TObject::kOverwrite);
    output->Close();
}

int main(int argc, char** argv)
{
    if (argc == 4) {
        photonRaaSkim(argv[1], argv[2], argv[3]);
        return 0;
    }
    else if (argc == 3) {
        photonRaaSkim(argv[1], argv[2]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
            "./photonRaaSkim.exe <configFile> <inputFile> <outputFile>"
            << std::endl;
        return 1;
    }
}

float xSecCal(const char* fname_lowestPthat, TChain* mergedTree, float pthat_i, float pthat_f){
    const int nFile = 2;
    int entries[nFile];
    for(int i=0; i<nFile ; i++){
        entries[i]=0.0;
    }
    TFile* fin = new TFile(fname_lowestPthat);
    TTree* tlowest = (TTree*) fin -> Get("hiEvtAnalyzer/HiTree");
    Float_t pthat_low, pthat_merged;
    TBranch *b_pthat_low, *b_pthat_merged;
    tlowest->SetBranchAddress("pthat",&pthat_low, &b_pthat_low);
    entries[0] = tlowest->GetEntries(Form("pthat>= %.3f && pthat< %.3f", pthat_i, pthat_f));
    cout << "entries from the lowest pthat sample : " << entries[0] << endl;

    mergedTree->SetBranchAddress("pthat",&pthat_merged, &b_pthat_merged);
    entries[1] = mergedTree->GetEntries(Form("pthat>= %.3f && pthat< %.3f", pthat_i, pthat_f));
    cout << "entries from the merged sample : " << entries[1] << endl;

    float weight = (double)entries[0]/(double)entries[1];
    return weight;
}

