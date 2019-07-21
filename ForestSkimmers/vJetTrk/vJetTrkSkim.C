/*
 * macro to skim HiForest files for v+Jet+Trk analysis
 */

#include <TFile.h>
#include <TTree.h>
#include <TObjArray.h>
#include <TMath.h>
#include <TLorentzVector.h>
#include <TRandom3.h>

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>

#include "../../CorrelationTuple/EventMatcher.h"
#include "../../TreeHeaders/CutConfigurationTree.h"
#include "../../TreeHeaders/ggHiNtuplizerTree.h"
#include "../../TreeHeaders/hiEvtTree.h"
#include "../../TreeHeaders/JetTree.h"
#include "../../TreeHeaders/trackTree.h"
#include "../../TreeHeaders/pfCandTree.h"
#include "../../TreeHeaders/hiGenParticleTree.h"
#include "../../TreeHeaders/skimAnalysisTree.h"
#include "../../TreeHeaders/eventSkimTree.h"
#include "../../TreeHeaders/mixEventSkimTree.h"
#include "../../TreeHeaders/jetSkimTree.h"
#include "../../TreeHeaders/trackSkimTree.h"
#include "../../Utilities/interface/ArgumentParser.h"
#include "../../Utilities/interface/ConfigurationParser.h"
#include "../../Utilities/interface/GraphicsConfigurationParser.h"
#include "../../Utilities/interface/InputConfigurationParser.h"
#include "../../Utilities/interface/HiForestInfoController.h"
#include "../../Utilities/eventUtil.h"
#include "../../Utilities/fileUtil.h"
#include "../../Utilities/physicsUtil.h"
#include "../../Utilities/vJetTrkUtil.h"

struct entryVec
{
    entryVec() : v({}), files({}), current(0) {}
    std::vector<Long64_t> v;
    std::vector<int> files;
    int current;
};

///// global variables
/// configuration variables

// input for TTree
std::string treePath;
std::string sampleType;

std::vector<std::string> jetCollections;

// event cuts/weights
std::vector<std::vector<float>> pthatWeights;

int nMixEvents;

// RECO object cuts
bool excludeHI18HEMfailure;

int nJetCollections;
int nPthatWeights;

/// configuration variables - END
enum ANAOBJS {
    kEWboson,
    kJet,
    kTrk,
    kN_ANAOBJS
};

int nMixFiles;

std::vector<std::string> argOptions;
///// global variables - END

const long MAXTREESIZE = 50000000000; // set maximum tree size from 10 GB to 100 GB, so that the code does not switch to a new file after 10 GB

int readConfiguration(std::string configFile, std::string inputFile);
void printConfiguration();
void vJetTrkSkim(std::string configFile, std::string inputFile, std::string outputFile = "vJetTrkSkim.root", std::string mixFile = "DUMMY");

void vJetTrkSkim(std::string configFile, std::string inputFile, std::string outputFile, std::string mixFile)
{
    std::cout<<"running vJetTrkSkim()"<<std::endl;
    std::cout<<"configFile  = "<< configFile.c_str() <<std::endl;
    std::cout<<"inputFile   = "<< inputFile.c_str()  <<std::endl;
    std::cout<<"outputFile  = "<< outputFile.c_str() <<std::endl;
    std::cout<<"mixFile  = "<< mixFile.c_str() <<std::endl;

    if (readConfiguration(configFile, inputFile) != 0)  return;
    printConfiguration();

    bool isMC = isMCsample(sampleType);
    bool isPbPb = isPbPbsample(sampleType);
    bool isPP = isPPsample(sampleType);

    bool isPbPb15 = isPbPb && (sampleType.find("2015") != std::string::npos);
    bool isPbPb18 = isPbPb && (sampleType.find("2018") != std::string::npos);
    bool isPP17data = !isPbPb && !isMC && (sampleType.find("2017") != std::string::npos);
    bool mixEvents = (nMixEvents > 0);

    int collisionType = -1;
    if (isPbPb15) {
        collisionType = COLL::TYPE::kHI;
    }
    else if (isPbPb18) {
        collisionType = COLL::TYPE::kHI2018;
    }
    else if (isPP) {
        collisionType = COLL::TYPE::kPP;
    }

    std::cout << "isMC = " << isMC << std::endl;
    std::cout << "isPbPb15 = " << isPbPb15 << std::endl;
    std::cout << "isPbPb18 = " << isPbPb18 << std::endl;
    std::cout << "isPP17data = " << isPP17data << std::endl;

    int rndSeed= (ArgumentParser::optionExists("--rndSeed", argOptions)) ?
            std::atof(ArgumentParser::ParseOptionInputSingle("--rndSeed", argOptions).c_str()) : -1;

    std::string vType = (ArgumentParser::optionExists("--vType", argOptions)) ?
            ArgumentParser::ParseOptionInputSingle("--vType", argOptions).c_str() : "pho";

    double vPtMin = (ArgumentParser::optionExists("--vPtMin", argOptions)) ?
            std::atof(ArgumentParser::ParseOptionInputSingle("--vPtMin", argOptions).c_str()) : 10;
    double vEtaMax = (ArgumentParser::optionExists("--vEtaMax", argOptions)) ?
                std::atof(ArgumentParser::ParseOptionInputSingle("--vEtaMax", argOptions).c_str()) : 999999;

    double jetPtMin = (ArgumentParser::optionExists("--jetPtMin", argOptions)) ?
            std::atof(ArgumentParser::ParseOptionInputSingle("--jetPtMin", argOptions).c_str()) : 10;
    double jetEtaMax = (ArgumentParser::optionExists("--jetEtaMax", argOptions)) ?
                std::atof(ArgumentParser::ParseOptionInputSingle("--jetEtaMax", argOptions).c_str()) : 2;

    double trkPtMin = (ArgumentParser::optionExists("--trkPtMin", argOptions)) ?
                std::atof(ArgumentParser::ParseOptionInputSingle("--trkPtMin", argOptions).c_str()) : 0.7;

    double trkEtaMax = (ArgumentParser::optionExists("--trkEtaMax", argOptions)) ?
                    std::atof(ArgumentParser::ParseOptionInputSingle("--trkEtaMax", argOptions).c_str()) : 2.4;

    std::cout << "rndSeed = " << rndSeed << std::endl;

    std::cout << "vType = " << vType << std::endl;
    std::cout << "vPtMin = " << vPtMin << std::endl;
    std::cout << "vEtaMax = " << vEtaMax << std::endl;

    std::cout << "jetPtMin = " << jetPtMin << std::endl;
    std::cout << "jetEtaMax = " << jetEtaMax << std::endl;

    std::cout << "trkPtMin = " << trkPtMin << std::endl;
    std::cout << "trkEtaMax = " << trkEtaMax << std::endl;

    std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.c_str());
    std::cout<<"input ROOT files : num = "<<inputFiles.size()<< std::endl;
    std::cout<<"#####"<< std::endl;
    for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
        std::cout<<(*it).c_str()<< std::endl;
    }
    std::cout<<"##### END #####"<< std::endl;

    std::vector<std::string> mixFileList = InputConfigurationParser::ParseFiles(mixFile.c_str());
    if (mixEvents) {
        std::cout<<"mix ROOT files : num = "<<mixFileList.size()<< std::endl;
        std::cout<<"#####"<< std::endl;
        for (std::vector<std::string>::iterator it = mixFileList.begin() ; it != mixFileList.end(); ++it) {
            std::cout<<(*it).c_str()<< std::endl;
        }
        std::cout<<"##### END #####"<< std::endl;
    }
    else {
        std::cout<<"No event mixing will be done."<< std::endl;
    }

    bool vIsPho = (toLowerCase(vType).find("pho") == 0);
    bool vIsZmm = (toLowerCase(vType).find("zmm") == 0);
    bool vIsZee = (toLowerCase(vType).find("zee") == 0);
    bool vIsZ = vIsZmm || vIsZee;

    bool doTrkVtx = isPP;

    // input trees
    TTree* treeHLT = 0;
    TTree* treeggHiNtuplizer = 0;
    TTree* treeHiEvt = 0;
    std::vector<TTree*> treesJet(nJetCollections, 0);
    TTree* treeTrack = 0;
    TTree* treePFCand = 0;
    TTree* treeHiGenParticle = 0;
    TTree* treeHiForestInfo = 0;
    TTree* treeSkim = 0;

    std::string treePathHLT = "hltanalysis/HltTree";
    std::string treePathHiEvt = "hiEvtAnalyzer/HiTree";
    std::string treePathTrack = "ppTrack/trackTree";
    if (isPbPb15) {
        treePathTrack = "anaTrack/trackTree";
    }
    std::string treePathPFCand = "pfcandAnalyzer/pfTree";
    std::string treePathSkimAna = "skimanalysis/HltTree";
    std::string treePathGen = "HiGenParticleAna/hi";

    int nFiles = inputFiles.size();
    TFile* fileTmp = 0;

    std::cout << "initial reading to get the number of entries (if there is only one input file) and HiForest info" << std::endl;
    // read the first file only to get the HiForest info
    std::string inputPath = inputFiles.at(0).c_str();
    fileTmp = TFile::Open(inputPath.c_str(), "READ");
    fileTmp->cd();

    if (nFiles == 1) {
        // read one tree only to get the number of entries
        treeggHiNtuplizer = (TTree*)fileTmp->Get(treePath.c_str());
        Long64_t entriesTmp = treeggHiNtuplizer->GetEntries();
        std::cout << "entries = " << entriesTmp << std::endl;
        treeggHiNtuplizer->Delete();
    }

    treeHiForestInfo = (TTree*)fileTmp->Get("HiForest/HiForestInfo");
    HiForestInfoController hfic(treeHiForestInfo);
    std::cout<<"### HiForestInfo Tree ###"<< std::endl;
    hfic.printHiForestInfo();
    std::cout<<"###"<< std::endl;

    fileTmp->Close();
    // done with initial reading

    TFile* output = TFile::Open(outputFile.c_str(), "RECREATE");

    // output trees
    TTree* outTreeHLT = 0;
    TTree* outTreeggHiNtuplizer = 0;
    TTree* outTreeHiEvt = 0;
    TTree* eventSkimTree = 0;
    TTree* mixEventSkimTree = 0;
    std::vector<TTree*> jetSkimTrees(nJetCollections, 0);
    TTree* trackSkimTree = 0;

    eventSkimTree= new TTree("eventSkim", "additional event info that is not in hiEvt");
    eventSkimTree->SetMaxTreeSize(MAXTREESIZE);
    eventSkim evtskim;
    evtskim.setupTreeForWriting(eventSkimTree);

    mixEventSkimTree = new TTree("mixEventSkim", "event info about mix events");
    mixEventSkimTree->SetMaxTreeSize(MAXTREESIZE);
    mixEventSkim mixevtskim;
    mixevtskim.setupTreeForWriting(mixEventSkimTree);

    for (int i=0; i<nJetCollections; ++i) {

        // pick a unique, but also not complicated name for zJet Trees
        // jet collection names which are complicated will be put into tree title
        std::string treeJetSkimName = Form("skim_%s", jetCollections.at(i).c_str());
        std::string treeJetSkimTitle = Form("skim of %s", jetCollections.at(i).c_str());

        jetSkimTrees[i] = new TTree(treeJetSkimName.c_str(),treeJetSkimTitle.c_str());
        jetSkimTrees[i]->SetMaxTreeSize(MAXTREESIZE);
    }
    std::vector<jetSkim> jetskims(nJetCollections);
    for (int i=0; i<nJetCollections; ++i) {
        jetskims[i].setupTreeForWriting(jetSkimTrees[i]);
    }

    trackSkimTree = new TTree("trackSkim", "skim of tracks");
    trackSkimTree->SetMaxTreeSize(MAXTREESIZE);
    trackSkim trkskim;
    trkskim.setupTreeForWriting(trackSkimTree);

    // variables for mix event files
    nMixFiles = mixFileList.size();

    std::vector<TFile*> mixFiles(nMixFiles, 0);
    std::vector<bool> isMixFileGood(nMixFiles, false);
    std::vector<TTree*> treeMixHiEvt(nMixFiles, 0);
    std::vector<TTree*> treeMixggHiNtuplizer(nMixFiles, 0);
    std::vector<std::vector<TTree*>> treesMixJet(nMixFiles);
    std::vector<TTree*> treeMixTrack(nMixFiles, 0);
    std::vector<TTree*> treeMixPFCand(nMixFiles, 0);
    std::vector<TTree*> treeMixHiGenParticle(nMixFiles, 0);
    std::vector<TTree*> treeMixSkim(nMixFiles, 0);
    std::vector<TTree*> treeMixEventSkim(nMixFiles, 0);

    std::vector<hiEvt> hiEvtMix(nMixFiles);
    std::vector<ggHiNtuplizer> ggHiMix(nMixFiles);
    std::vector<std::vector<Jets>> jetsMix(nMixFiles);
    std::vector<Tracks> trksMix(nMixFiles);
    std::vector<pfCand> pfMix(nMixFiles);
    std::vector<hiGenParticle> hiGenMix(nMixFiles);
    std::vector<skimAnalysis> skimAnaMix(nMixFiles);
    std::vector<eventSkim> eventSkimMix(nMixFiles);

    std::string treeMixPathEventSkim = "eventSkim";

    std::vector<bool> isForest(nMixFiles);

    // match_hiBin
    static entryVec entriesMixedEvent[VJT::nCentBins][VJT::nVzBins][VJT::nEventPlaneBins];

    if (mixEvents) {

        for (int i = 0; i < nMixFiles; ++i) {
            treesMixJet[i].clear();
            treesMixJet[i].resize(nJetCollections);
            jetsMix[i].clear();
            jetsMix[i].resize(nJetCollections);
        }

        // open mix files
        for (int i = 0; i < nMixFiles; ++i) {

            std::string mixFilePath = mixFileList[i];
            std::cout <<"iFileMix = " << i << " , " ;
            std::cout <<"reading mix file : " << mixFilePath.c_str() << std::endl;
            mixFiles[i] = TFile::Open(mixFilePath.c_str(), "READ");

            // check if the file is usable, if not skip the file.
            if (isGoodFile(mixFiles[i]) != 0) {
                std::cout << "File is not good. skipping file." << std::endl;
                continue;
            }

            isMixFileGood[i] = true;

            // check if mix file is forest or skim
            treeMixEventSkim[i] = 0;
            treeMixEventSkim[i] = (TTree*)mixFiles[i]->Get(treeMixPathEventSkim.c_str());
            isForest[i] = (treeMixEventSkim[i] == 0);

            if (i < 10) {
                std::cout << "isMixForest = " << isForest[i] << std::endl;
            }

            treeMixHiEvt[i] = (TTree*)mixFiles[i]->Get(treePathHiEvt.c_str());
            setBranchStatusTreeHiEvt(treeMixHiEvt[i], isMC);

            treeMixggHiNtuplizer[i] = (TTree*)mixFiles[i]->Get(treePath.c_str());
            treeMixggHiNtuplizer[i]->SetBranchStatus("*", 0);
            treeMixggHiNtuplizer[i]->SetBranchStatus("rho", 1);

            for (int iJ = 0; iJ < nJetCollections; ++iJ) {
                std::string treeMixPathJet = Form("%s/t", jetCollections[iJ].c_str());
                treesMixJet[i][iJ] = (TTree*)mixFiles[i]->Get(treeMixPathJet.c_str());
                setBranchStatusTreeJet(treesMixJet[i][iJ], isMC);
            }
            treeMixTrack[i] = (TTree*)mixFiles[i]->Get(treePathTrack.c_str());
            setBranchStatusTreeTrack(treeMixTrack[i], doTrkVtx);

            treeMixSkim[i] = (TTree*)mixFiles[i]->Get(treePathSkimAna.c_str());
            setBranchStatusTreeSkimAna(treeMixSkim[i]);
            if (isMC) {

                treeMixHiEvt[i]->SetBranchStatus("weight", 1);
                treeMixHiEvt[i]->SetBranchStatus("pthat",1);

                treeMixHiGenParticle[i] = (TTree*)mixFiles[i]->Get(treePathGen.c_str());
                setBranchStatusTreeHiGenParticle(treeMixHiGenParticle[i]);
            }

            hiEvtMix[i].setupTreeForReading(treeMixHiEvt[i]);
            ggHiMix[i].setupTreeForReading(treeMixggHiNtuplizer[i]);
            for (int iJ = 0; iJ < nJetCollections; ++iJ) {
                jetsMix[i][iJ].setupTreeForReading(treesMixJet[i][iJ]);
            }

            trksMix[i].setupTreeForReading(treeMixTrack[i]);
            skimAnaMix[i].setupTreeForReading(treeMixSkim[i]);
            if (isMC) {
                hiGenMix[i].setupTreeForReading(treeMixHiGenParticle[i]);
            }

            if (isForest[i]) {
                treeMixPFCand[i] = (TTree*)mixFiles[i]->Get(treePathPFCand.c_str());
                setBranchStatusTreePFCand(treeMixPFCand[i]);
                pfMix[i].setupTreeForReading(treeMixPFCand[i]);
            }
            else {
                treeMixEventSkim[i] = (TTree*)mixFiles[i]->Get(treeMixPathEventSkim.c_str());
                treeMixEventSkim[i]->SetBranchStatus("*", 1);
                eventSkimMix[i].setupTreeForReading(treeMixEventSkim[i]);
            }

            Long64_t entriesTmp = treeMixHiEvt[i]->GetEntries();
            std::cout << "entries in mix file = " << entriesTmp << std::endl;
        }

        // split mix files and entry indices into event categories
        std::cout <<"### Splitting mix events into event categories "<<std::endl;
        for (int i = 0; i < nMixFiles; ++i) {

            if (!isMixFileGood[i])  continue;

            std::cout <<"iFileMix = " << i << " , " << std::endl;

            Long64_t entriesMixTmp = treeMixHiEvt[i]->GetEntries();
            for (Long64_t j_entry_mix = 0; j_entry_mix < entriesMixTmp; ++j_entry_mix) {

                treeMixHiEvt[i]->GetEntry(j_entry_mix);
                if (!(std::fabs(hiEvtMix[i].vz) < 15))  continue;

                int ivz = getVzBin(hiEvtMix[i].vz);
                int iEventPlane = 0;
                if (isPbPb) {
                    iEventPlane = getEventPlaneBin(hiEvtMix[i].hiEvtPlanes[8]);
                }

                if (ivz < 0) continue;
                if (iEventPlane < 0) continue;

                treeMixSkim[i]->GetEntry(j_entry_mix);
                if (isPbPb15 && !skimAnaMix[i].passedEventSelectionHI())  continue;
                else if (isPbPb18 && !skimAnaMix[i].passedEventSelectionHI2018())  continue;
                else if (!isPbPb && !skimAnaMix[i].passedEventSelectionPP())  continue;

                int iCent = -1;
                if (VJT::mixMethod == VJT::MIXMETHODS::k_match_hiBin) {
                    iCent = getHiBin(hiEvtMix[i].hiBin);
                }
                else if (VJT::mixMethod == VJT::MIXMETHODS::k_match_Npart) {
                    iCent = getNpartBin(hiEvtMix[i].Npart);
                }
                else if (VJT::mixMethod == VJT::MIXMETHODS::k_match_hiHF) {
                    iCent = getHiHFBin(hiEvtMix[i].hiHF);
                }
                else if (VJT::mixMethod == VJT::MIXMETHODS::k_match_rho) {
                    treeMixggHiNtuplizer[i]->GetEntry(j_entry_mix);
                    iCent = getRhoBin(ggHiMix[i].rho);
                }
                else if (VJT::mixMethod == VJT::MIXMETHODS::k_match_PF_HF_totE) {

                    float tmpTotE = -1;
                    if (isForest[i]) {
                        treeMixPFCand[i]->GetEntry(j_entry_mix);
                        std::vector<float> pf_HF_totE = getPFHFtotE(pfMix[i], VJT::etaMin_pf_HF, VJT::etaMax_pf_HF);
                        tmpTotE = pf_HF_totE[0]+pf_HF_totE[1];
                    }
                    else {
                        treeMixEventSkim[i]->GetEntry(j_entry_mix);
                        tmpTotE = eventSkimMix[i].pf_h_HF_totE + eventSkimMix[i].pf_eg_HF_totE;
                    }
                    iCent = getPFHFtotEBin(tmpTotE);
                }
                else if (VJT::mixMethod == VJT::MIXMETHODS::k_match_nVtx) {

                    treeMixTrack[i]->GetEntry(j_entry_mix);
                    iCent = getNVtxBin(trksMix[i].nVtx);
                }

                if (iCent < 0) continue;

                entriesMixedEvent[iCent][ivz][iEventPlane].files.push_back(i);
                entriesMixedEvent[iCent][ivz][iEventPlane].v.push_back(j_entry_mix);
            }
        }
        std::cout <<"### Splitting mix events into event categories - END"<<std::endl;

        for (int iCent = 0; iCent < VJT::nCentBins; ++iCent) {
            for (int iVz = 0; iVz < VJT::nVzBins; ++iVz) {
                for (int iEP = 0; iEP < VJT::nEventPlaneBins; ++iEP) {

                    int nTmp = entriesMixedEvent[iCent][iVz][iEP].v.size();

                    entriesMixedEvent[iCent][iVz][iEP].current = 0;
                    if (rndSeed >= 0) {
                        TRandom3 rand(rndSeed);
                        entriesMixedEvent[iCent][iVz][iEP].current = rand.Integer(nTmp);
                    }

                    if (nTmp < nMixEvents) {
                        std::cout << "WARNING : number of mix events is nTmp = " << nTmp << " < nMixEvents = " << nMixEvents << std::endl;
                        std::cout << "iCent, ivz, iEvtPlane = " << iCent << " " << iVz << " " << iEP << std::endl;
                    }
                }
            }
        }
    }

    EventMatcher* em = new EventMatcher();
    Long64_t duplicateEntries = 0;

    EventMatcher* emMix = new EventMatcher();
    Long64_t duplicateEntriesMix = 0;

    Long64_t entries = 0;
    Long64_t entriesAnalyzed = 0;
    Long64_t entriesSelected = 0;

    int nFilesSkipped = 0;
    std::cout<< "Loop : " << treePath.c_str() <<std::endl;
    for (int iFile = 0; iFile < nFiles; ++iFile)  {

        std::string inputPath = inputFiles.at(iFile).c_str();
        std::cout <<"iFile = " << iFile << " , " ;
        std::cout <<"reading input file : " << inputPath.c_str() << std::endl;
        fileTmp = TFile::Open(inputPath.c_str(), "READ");

        // check if the file is usable, if not skip the file.
        if (isGoodFile(fileTmp) != 0) {
            std::cout << "File is not good. skipping file." << std::endl;
            nFilesSkipped++;
            continue;
        }

        treeHLT = (TTree*)fileTmp->Get(treePathHLT.c_str());
        if (treeHLT == 0) {
            treeHLT = (TTree*)fileTmp->Get("hltanalysisReco/HltTree");
        }

        treeggHiNtuplizer = (TTree*)fileTmp->Get(treePath.c_str());
        treeggHiNtuplizer->SetBranchStatus("*",0);     // disable all branches
        treeggHiNtuplizer->SetBranchStatus("nPho",1);
        treeggHiNtuplizer->SetBranchStatus("pho*",1);     // enable photon branches
        treeggHiNtuplizer->SetBranchStatus("nMu",1);
        treeggHiNtuplizer->SetBranchStatus("mu*",1);
        treeggHiNtuplizer->SetBranchStatus("nEle",1);
        treeggHiNtuplizer->SetBranchStatus("ele*",1);

        treeggHiNtuplizer->SetBranchStatus("rho",1);

        treeHiEvt = (TTree*)fileTmp->Get(treePathHiEvt.c_str());
        setBranchStatusTreeHiEvt(treeHiEvt, isMC);

        for (int i = 0; i < nJetCollections; ++i) {

            treesJet[i] = (TTree*)fileTmp->Get(Form("%s/t", jetCollections[i].c_str()));
            setBranchStatusTreeJet(treesJet[i], isMC);
        }

        treeTrack = (TTree*)fileTmp->Get(treePathTrack.c_str());
        setBranchStatusTreeTrack(treeTrack, doTrkVtx);

        treePFCand = (TTree*)fileTmp->Get(treePathPFCand.c_str());
        setBranchStatusTreePFCand(treePFCand);

        // specify explicitly which branches to use, do not use wildcard
        treeSkim = (TTree*)fileTmp->Get(treePathSkimAna.c_str());
        setBranchStatusTreeSkimAna(treeSkim);

        if (isMC) {
            treeggHiNtuplizer->SetBranchStatus("nMC*",1);     // enable GEN particle branches
            treeggHiNtuplizer->SetBranchStatus("mc*",1);      // enable GEN particle branches

            treeHiEvt->SetBranchStatus("weight", 1);
            treeHiEvt->SetBranchStatus("pthat",1);

            treeHiGenParticle = (TTree*)fileTmp->Get(treePathGen.c_str());
            setBranchStatusTreeHiGenParticle(treeHiGenParticle);
        }

        ggHiNtuplizer ggHi;
        ggHi.setupTreeForReading(treeggHiNtuplizer);

        hiEvt hiEvt;
        hiEvt.setupTreeForReading(treeHiEvt);

        std::vector<Jets> jets(nJetCollections);
        for (int i=0; i<nJetCollections; ++i) {
            jets.at(i).setupTreeForReading(treesJet[i]);
        }

        Tracks trks;
        trks.setupTreeForReading(treeTrack);

        pfCand pf;
        pf.setupTreeForReading(treePFCand);

        hiGenParticle hiGen;
        if (isMC) {
            hiGen.setupTreeForReading(treeHiGenParticle);
        }

        skimAnalysis skimAna;
        if (isPbPb15) skimAna.enableBranchesHI(treeSkim);
        else if (isPbPb18) skimAna.enableBranchesHI2018(treeSkim);
        else if (!isPbPb) skimAna.enableBranchesPP(treeSkim);
        skimAna.setupTreeForReading(treeSkim);
        skimAna.checkBranches(treeSkim);    // do the event selection if the branches exist.

        output->cd();

        if (iFile == 0)  outTreeHLT = treeHLT->CloneTree(0);
        else             treeHLT->CopyAddresses(outTreeHLT);
        //outTreeHLT->SetName("hltTree");
        //outTreeHLT->SetTitle("subbranches of hltanalysis/HltTree");

        if (iFile == 0)  outTreeggHiNtuplizer = treeggHiNtuplizer->CloneTree(0);
        else             treeggHiNtuplizer->CopyAddresses(outTreeggHiNtuplizer);

        if (iFile == 0)  outTreeHiEvt = treeHiEvt->CloneTree(0);
        else             treeHiEvt->CopyAddresses(outTreeHiEvt);

        if (iFile == 0) {
            outTreeHLT->SetMaxTreeSize(MAXTREESIZE);
            outTreeggHiNtuplizer->SetMaxTreeSize(MAXTREESIZE);
            outTreeHiEvt->SetMaxTreeSize(MAXTREESIZE);
        }

        Long64_t entriesTmp = treeggHiNtuplizer->GetEntries();
        entries += entriesTmp;
        std::cout << "entries in File = " << entriesTmp << std::endl;
        for (Long64_t j_entry = 0; j_entry < entriesTmp; ++j_entry)
        {
            if (j_entry % 2000 == 0)  {
              std::cout << "current entry = " <<j_entry<<" out of "<<entriesTmp<<" : "<<std::setprecision(2)<<(double)j_entry/entriesTmp*100<<" %"<<std::endl;
            }

            treeHLT->GetEntry(j_entry);
            treeggHiNtuplizer->GetEntry(j_entry);
            treeHiEvt->GetEntry(j_entry);
            treeSkim->GetEntry(j_entry);
            if (isMC) {
                treeHiGenParticle->GetEntry(j_entry);
            }

            bool eventAdded = em->addEvent(hiEvt.run, hiEvt.lumi, hiEvt.evt, j_entry);
            if(!eventAdded) // this event is duplicate, skip this one.
            {
                duplicateEntries++;
                continue;
            }

            // event selection
            if (!(std::fabs(hiEvt.vz) < 15))  continue;

            if (isPbPb15 && !skimAna.passedEventSelectionHI())  continue;
            else if (isPbPb18 && !skimAna.passedEventSelectionHI2018())  continue;
            else if (!isPbPb && !skimAna.passedEventSelectionPP())  continue;

            entriesAnalyzed++;

            double w = 1;

            int hiBin = hiEvt.hiBin;

            if (nPthatWeights > 0) {
                double pthatWeight = 0;
                for (int i = 0; i < nPthatWeights; ++i) {
                    if (hiEvt.pthat >= pthatWeights[0][i] && hiEvt.pthat < pthatWeights[1][i]) {
                        pthatWeight = pthatWeights[2][i];
                        break;
                    }
                }
                w *= pthatWeight;
            }

            double vPt = -1;
            double vEta = -999999;

            if (vIsPho) {
                for (int i = 0; i < ggHi.nPho; ++i) {

                    if (excludeHI18HEMfailure && !ggHi.passedHI18HEMfailurePho(i))  continue;
                }
            }
            if (vIsZ) {

                double zmassPDG = 91.1876;
                double deltaMass = 999999;

                TLorentzVector vecl1;
                TLorentzVector vecl2;
                TLorentzVector vecll;

                double lMass = -1;
                int nL = 0;
                std::vector<float> *lPt;
                std::vector<float> *lEta;
                std::vector<float> *lPhi;

                if (vIsZmm) {

                    lMass = 0.105658;
                    nL = ggHi.nMu;
                    lPt = ggHi.muPt;
                    lEta = ggHi.muEta;
                    lPhi = ggHi.muPhi;
                }
                else if (vIsZee) {

                    lMass = 0.000511;
                    nL = ggHi.nEle;
                    lPt = ggHi.elePt;
                    lEta = ggHi.eleEta;
                    lPhi = ggHi.elePhi;
                }

                for (int i = 0; i < nL; ++i) {

                    if (!((*lPt)[i] > 20)) continue;

                    if (vIsZmm) {
                        if (!ggHi.passedMuSelection(i, collisionType)) continue;
                    }
                    else if (vIsZee) {
                        if (!ggHi.passedEleSelection(i, collisionType, hiBin)) continue;
                        if (excludeHI18HEMfailure && !ggHi.passedHI18HEMfailureEle(i))  continue;
                    }

                    for (int j = i+1; j < nL; ++j) {

                        if (!((*lPt)[j] > 20)) continue;

                        if (vIsZmm) {
                            if (!ggHi.passedMuSelection(j, collisionType)) continue;
                        }
                        else if (vIsZee) {
                            if (!ggHi.passedEleSelection(j, collisionType, hiBin)) continue;
                            if (excludeHI18HEMfailure && !ggHi.passedHI18HEMfailureEle(j))  continue;
                        }

                        vecl1.SetPtEtaPhiM((*lPt)[i], (*lEta)[i], (*lPhi)[i], lMass);
                        vecl2.SetPtEtaPhiM((*lPt)[j], (*lEta)[j], (*lPhi)[j], lMass);

                        vecll = vecl1 + vecl2;

                        if (!(vecll.M() >= 60 && vecll.M() <= 120)) continue;

                        if (std::fabs(vecll.M() - zmassPDG) < deltaMass) {
                            deltaMass = std::fabs(vecll.M() - zmassPDG);
                            vPt = vecll.Pt();
                            vEta = vecll.Eta();
                        }
                    }
                }
            }

            if (vPt < vPtMin) continue;

            if (!(std::fabs(vEta) < vEtaMax))  continue;

            entriesSelected++;

            for (int i = 0; i < nJetCollections; ++i) {
                treesJet[i]->GetEntry(j_entry);
            }
            treeTrack->GetEntry(j_entry);
            treePFCand->GetEntry(j_entry);

            for (int iJ = 0; iJ < nJetCollections; ++iJ) {
                jetskims[iJ].clearEvent();

                for (int i = 0; i < jets[iJ].nref; ++i) {

                    if (jets[iJ].jtpt[i] < jetPtMin) continue;
                    if (std::fabs(jets[iJ].jteta[i]) > jetEtaMax) continue;

                    jetskims[iJ].jetptCorr.push_back(jets[iJ].jtpt[i]);
                    jetskims[iJ].jetpt.push_back(jets[iJ].jtpt[i]);
                    jetskims[iJ].jeteta.push_back(jets[iJ].jteta[i]);
                    jetskims[iJ].jetphi.push_back(jets[iJ].jtphi[i]);
                    jetskims[iJ].rawpt.push_back(jets[iJ].rawpt[i]);
                    if (isMC) {
                        jetskims[iJ].refpt.push_back(jets[iJ].refpt[i]);
                        jetskims[iJ].refeta.push_back(jets[iJ].refeta[i]);
                        jetskims[iJ].refphi.push_back(jets[iJ].refphi[i]);
                        jetskims[iJ].refparton_flavor.push_back(jets[iJ].refparton_flavor[i]);
                        jetskims[iJ].subid.push_back(jets[iJ].subid[i]);
                    }
                    jetskims[iJ].njet++;

                }

                if (isMC) {
                    jetskims[iJ].ngen = jets[iJ].ngen;
                    for (int i = 0; i < jets[iJ].ngen; ++i) {

                        jetskims[iJ].genpt.push_back(jets[iJ].genpt[i]);
                        jetskims[iJ].geneta.push_back(jets[iJ].geneta[i]);
                        jetskims[iJ].genphi.push_back(jets[iJ].genphi[i]);
                        jetskims[iJ].gensubid.push_back(jets[iJ].gensubid[i]);
                    }
                }
            }

            trkskim.clearEvent();
            for (int i = 0; i < trks.nTrk; ++i) {

                if (!passedTrkSelection(trks, i, collisionType))  continue;
                if (!(trks.trkPt[i] > trkPtMin))  continue;
                if (!(std::fabs(trks.trkEta[i]) < trkEtaMax))  continue;

                trkskim.trkPt.push_back(trks.trkPt[i]);
                trkskim.trkEta.push_back(trks.trkEta[i]);
                trkskim.trkPhi.push_back(trks.trkPhi[i]);
                trkskim.trkCharge.push_back(trks.trkCharge[i]);
                trkskim.trkPtError.push_back(trks.trkPtError[i]);
                trkskim.trkNHit.push_back(trks.trkNHit[i]);
                trkskim.trkNlayer.push_back(trks.trkNlayer[i]);
                trkskim.highPurity.push_back(trks.highPurity[i]);
                trkskim.trkChi2.push_back(trks.trkChi2[i]);
                trkskim.trkNdof.push_back(trks.trkNdof[i]);
                trkskim.trkDxy1.push_back(trks.trkDxy1[i]);
                trkskim.trkDxyError1.push_back(trks.trkDxyError1[i]);
                trkskim.trkDz1.push_back(trks.trkDz1[i]);
                trkskim.trkDzError1.push_back(trks.trkDzError1[i]);
                trkskim.trkAlgo.push_back(trks.trkAlgo[i]);
                if (trks.b_trkMVA != 0) {
                    trkskim.trkMVA.push_back(trks.trkMVA[i]);
                }
                else {
                    trkskim.trkMVA.push_back(-987987);
                }
                trkskim.pfEcal.push_back(trks.pfEcal[i]);
                trkskim.pfHcal.push_back(trks.pfHcal[i]);
                trkskim.trkWeight.push_back(1);
                if (doTrkVtx) {
                    trkskim.trkNVtx.push_back(trks.trkNVtx[i]);
                }
                trkskim.nTrk++;
            }
            if (doTrkVtx) {
                trkskim.nVtx = trks.nVtx;
            }

            evtskim.clearEvent();
            evtskim.rho = ggHi.rho;
            std::vector<float> pf_HF_totE = getPFHFtotE(pf, VJT::etaMin_pf_HF, VJT::etaMax_pf_HF);
            evtskim.pf_h_HF_totE = pf_HF_totE[0];
            evtskim.pf_eg_HF_totE = pf_HF_totE[1];

            if (isMC) {
                for (int i = 0; i < hiGen.mult; ++i) {

                    if (!((*hiGen.pt)[i] > trkPtMin))  continue;
                    if (!(std::fabs((*hiGen.eta)[i]) < trkEtaMax))  continue;

                    trkskim.pt.push_back((*hiGen.pt)[i]);
                    trkskim.eta.push_back((*hiGen.eta)[i]);
                    trkskim.phi.push_back((*hiGen.phi)[i]);
                    trkskim.pdg.push_back((*hiGen.pdg)[i]);
                    trkskim.chg.push_back((*hiGen.chg)[i]);
                    trkskim.sube.push_back((*hiGen.sube)[i]);
                    trkskim.mult++;
                }
            }

            mixevtskim.clearEvent();
            for (int iJ = 0; iJ < nJetCollections; ++iJ) {
                jetskims[iJ].clearMixEvent();
            }
            trkskim.clearMixEvent();

            if (mixEvents) {

                int nMixed = 0;

                int ivz = getVzBin(hiEvt.vz);
                int iEventPlane = 0;
                if (isPbPb) {
                    iEventPlane = -1;
                    iEventPlane = getEventPlaneBin(hiEvt.hiEvtPlanes[8]);
                }

                if (ivz < 0) continue;
                if (iEventPlane < 0) continue;

                std::vector<bool> usedAdjacentCents;

                int iCent = -1;
                if (VJT::mixMethod == VJT::MIXMETHODS::k_match_hiBin) {
                    iCent = getHiBin(hiBin);
                    //iCent += 1;
                    //if (iCent > 199) iCent = 199;
                }
                else if (VJT::mixMethod == VJT::MIXMETHODS::k_match_Npart) {
                    iCent = getNpartBin(hiEvt.Npart);
                }
                else if (VJT::mixMethod == VJT::MIXMETHODS::k_match_rho) {
                    //iCent = getRhoBin(ggHi.rho-1.45);
                    //iCent = getRhoBin(ggHi.rho-1.32);
                    //iCent = getRhoBin(ggHi.rho-1.26);
                    //iCent = getRhoBin(ggHi.rho - getRhoDiff(ggHi.rho));
                    //iCent = getRhoBin(ggHi.rho-0.5);
                    iCent = getRhoBin(ggHi.rho-2.0);
                }
                else if (VJT::mixMethod == VJT::MIXMETHODS::k_match_PF_HF_totE) {

                    //iCent = getPFHFtotEBin((evtskim.pf_h_HF_totE + evtskim.pf_eg_HF_totE) - 682.0);
                    //iCent = getPFHFtotEBin((evtskim.pf_h_HF_totE + evtskim.pf_eg_HF_totE) - 546.0); // 682 - 20%
                    //iCent = getPFHFtotEBin((evtskim.pf_h_HF_totE + evtskim.pf_eg_HF_totE) - 818.0); // 682 + 20%
                    // data
                    iCent = getPFHFtotEBin((evtskim.pf_h_HF_totE + evtskim.pf_eg_HF_totE) - 657.5);
                    //iCent = getPFHFtotEBin((evtskim.pf_h_HF_totE + evtskim.pf_eg_HF_totE) - (657.5+6.46));
                    //iCent = getPFHFtotEBin((evtskim.pf_h_HF_totE + evtskim.pf_eg_HF_totE) - (657.5-6.46));
                }
                if (VJT::mixMethod == VJT::MIXMETHODS::k_match_nVtx) {

                    iCent = getNVtxBin(trks.nVtx-1);
                }

                if (iCent < 0) continue;

                int nAdjacentCents = usedAdjacentCents.size();
                bool centsAdjacentAvail = (nAdjacentCents > 0);

                int nEventsAttempted = 0;

                int iCentStart = iCent;

                int nMixEventsAvail = entriesMixedEvent[iCent][ivz][iEventPlane].v.size();
                // extract the characteristic bins to be used for event mixing

                int iME = entriesMixedEvent[iCent][ivz][iEventPlane].current;

                std::cout << "entriesSelected = "<< entriesSelected << std::endl;
                std::cout << "Start mixing for event with hiBin = " << hiBin << " , vz = " << hiEvt.vz << " , evtPlane = " << hiEvt.hiEvtPlanes[8] << std::endl;
                std::cout << "                       with NPart = " << hiEvt.Npart  << std::endl;
                std::cout << "                       with hiHF = " << hiEvt.hiHF  << std::endl;
                std::cout << "                       with hiHFhit = " << hiEvt.hiHFhit << std::endl;
                std::cout << "                       with rho = " << ggHi.rho << std::endl;
                std::cout << " iCent = " << iCent << " ivz = " << ivz << " , iEvtPlane = " << iEventPlane << std::endl;
                std::cout << " indexMixEvent = " << iME << " nMixEventsAvail = " << nMixEventsAvail << std::endl;

                int nMixEventsTmp = (nMixEventsAvail >= nMixEvents) ? nMixEvents : nMixEventsAvail;
                while (nMixed < nMixEventsTmp) {

                    if (nEventsAttempted == nMixEventsAvail) {

                        std::cout << "Run out of events for iCent = " << iCent << std::endl;

                        centsAdjacentAvail = false;
                        // try events from adjacent cent bins
                        for (int i = 0; i < nAdjacentCents; ++i) {

                            if (!usedAdjacentCents[i]) {

                                int iCentTmp = -1;
                                if (i % 2 == 0) {
                                    iCentTmp = iCentStart + ((i/2) + 1);
                                }
                                else {
                                    iCentTmp = iCentStart - ((i/2) + 1);
                                }

                                usedAdjacentCents[i] = true;

                                if (iCentTmp < 0 || iCentTmp >= VJT::nCentBins)  continue;

                                nMixEventsAvail = entriesMixedEvent[iCentTmp][ivz][iEventPlane].v.size();
                                if (nMixEventsAvail == 0)  continue;

                                iCent = iCentTmp;
                                iME = entriesMixedEvent[iCent][ivz][iEventPlane].current;

                                nEventsAttempted = 0;
                                nMixEventsTmp = (nMixEventsAvail >= nMixEvents) ? nMixEvents : nMixEventsAvail;

                                std::cout << "Try events from adjacent centrality iCent = " << iCent << std::endl;

                                centsAdjacentAvail = true;
                                break;
                            }
                        }

                        if (!centsAdjacentAvail) {

                            entriesMixedEvent[iCent][ivz][iEventPlane].current = iME;
                            std::cout << "End mixing at indexMixEvent = " << iME << std::endl;
                            std::cout << "Mixing ends without reaching desired number of events" << std::endl;
                            break;
                        }
                    }

                    int iMF = entriesMixedEvent[iCent][ivz][iEventPlane].files[iME];
                    Long64_t j_entry_mix = entriesMixedEvent[iCent][ivz][iEventPlane].v[iME];

                    treeMixHiEvt[iMF]->GetEntry(j_entry_mix);
                    treeMixTrack[iMF]->GetEntry(j_entry_mix);

                    nEventsAttempted++;

                    std::cout << " nMixed = " << nMixed << " , iMF = " << iMF << " , j_entry_mix = " << j_entry_mix << std::endl;

                    bool mixEvtAdded;
                    mixEvtAdded = emMix->addEvent(hiEvtMix[iMF].run, hiEvtMix[iMF].lumi, hiEvtMix[iMF].evt, j_entry_mix);
                    if(!mixEvtAdded) // this event is duplicate, skip this one.
                    {
                        std::cout << "WARNING : mix event is being used more than once" << std::endl;
                        std::cout << "Mix event run,lumis,event = " << hiEvtMix[iMF].run << " " << hiEvtMix[iMF].lumi << " " << hiEvtMix[iMF].evt << std::endl;
                        std::cout << "hiBin, ivz, iEvtPlane = " << hiBin << " " << ivz << " " << iEventPlane << std::endl;
                        duplicateEntriesMix++;
                    }

                    treeMixggHiNtuplizer[iMF]->GetEntry(j_entry_mix);
                    for (int i = 0; i < nJetCollections; ++i) {
                        treesMixJet[iMF][i]->GetEntry(j_entry_mix);
                    }
                    if (isMC) {
                        treeMixHiGenParticle[iMF]->GetEntry(j_entry_mix);
                    }

                    mixevtskim.vz_mix.push_back(hiEvtMix[iMF].vz);
                    mixevtskim.hiBin_mix.push_back(hiEvtMix[iMF].hiBin);
                    mixevtskim.hiHF_mix.push_back(hiEvtMix[iMF].hiHF);
                    mixevtskim.hiHFhit_mix.push_back(hiEvtMix[iMF].hiHFhit);
                    mixevtskim.hiEvtPlanes_mix.push_back(hiEvtMix[iMF].hiEvtPlanes[8]);
                    mixevtskim.rho_mix.push_back(ggHiMix[iMF].rho);
                    mixevtskim.run_mix.push_back(hiEvtMix[iMF].run);
                    mixevtskim.evt_mix.push_back(hiEvtMix[iMF].evt);
                    mixevtskim.lumi_mix.push_back(hiEvtMix[iMF].lumi);
                    if (isMC) {
                        mixevtskim.Npart_mix.push_back(hiEvtMix[iMF].Npart);
                        mixevtskim.Ncoll_mix.push_back(hiEvtMix[iMF].Ncoll);
                        mixevtskim.Nhard_mix.push_back(hiEvtMix[iMF].Nhard);
                    }
                    if (isForest[iMF]) {
                        treeMixPFCand[iMF]->GetEntry(j_entry_mix);
                        std::vector<float> pf_HF_totE_mix = getPFHFtotE(pfMix[iMF], VJT::etaMin_pf_HF, VJT::etaMax_pf_HF);
                        mixevtskim.pf_h_HF_totE_mix.push_back(pf_HF_totE_mix[0]);
                        mixevtskim.pf_eg_HF_totE_mix.push_back(pf_HF_totE_mix[1]);
                    }
                    else {
                        treeMixEventSkim[iMF]->GetEntry(j_entry_mix);
                        mixevtskim.pf_h_HF_totE_mix.push_back(eventSkimMix[iMF].pf_h_HF_totE);
                        mixevtskim.pf_eg_HF_totE_mix.push_back(eventSkimMix[iMF].pf_eg_HF_totE);
                    }
                    mixevtskim.nmix++;

                    for (int iJ = 0; iJ < nJetCollections; ++iJ) {

                        for (int i = 0; i < jetsMix[iMF][iJ].nref; ++i) {

                            if (jetsMix[iMF][iJ].jtpt[i] < jetPtMin) continue;
                            if (std::fabs(jetsMix[iMF][iJ].jteta[i]) > jetEtaMax) continue;

                            jetskims[iJ].jetptCorr_mix.push_back(jetsMix[iMF][iJ].jtpt[i]);
                            jetskims[iJ].jetpt_mix.push_back(jetsMix[iMF][iJ].jtpt[i]);
                            jetskims[iJ].jeteta_mix.push_back(jetsMix[iMF][iJ].jteta[i]);
                            jetskims[iJ].jetphi_mix.push_back(jetsMix[iMF][iJ].jtphi[i]);
                            jetskims[iJ].rawpt_mix.push_back(jetsMix[iMF][iJ].rawpt[i]);
                            if (isMC) {
                                jetskims[iJ].refpt_mix.push_back(jetsMix[iMF][iJ].refpt[i]);
                                jetskims[iJ].refeta_mix.push_back(jetsMix[iMF][iJ].refeta[i]);
                                jetskims[iJ].refphi_mix.push_back(jetsMix[iMF][iJ].refphi[i]);
                                jetskims[iJ].subid_mix.push_back(jetsMix[iMF][iJ].subid[i]);
                            }
                            jetskims[iJ].evtjet_mix.push_back(nMixed);
                            jetskims[iJ].njet_mix++;
                        }

                        if (isMC) {

                            for (int i = 0; i < jetsMix[iMF][iJ].ngen; ++i) {

                                jetskims[iJ].genpt_mix.push_back(jetsMix[iMF][iJ].genpt[i]);
                                jetskims[iJ].geneta_mix.push_back(jetsMix[iMF][iJ].geneta[i]);
                                jetskims[iJ].genphi_mix.push_back(jetsMix[iMF][iJ].genphi[i]);
                                jetskims[iJ].gensubid_mix.push_back(jetsMix[iMF][iJ].gensubid[i]);
                                jetskims[iJ].evtgen_mix.push_back(nMixed);
                                jetskims[iJ].ngen_mix++;
                            }
                        }
                    }

                    for (int i = 0; i < trksMix[iMF].nTrk; ++i) {

                        if (!passedTrkSelection(trksMix[iMF], i, collisionType))  continue;
                        if (!(trksMix[iMF].trkPt[i] > trkPtMin))  continue;
                        if (!(std::fabs(trksMix[iMF].trkEta[i]) < trkEtaMax))  continue;

                        trkskim.trkPt_mix.push_back(trksMix[iMF].trkPt[i]);
                        trkskim.trkEta_mix.push_back(trksMix[iMF].trkEta[i]);
                        trkskim.trkPhi_mix.push_back(trksMix[iMF].trkPhi[i]);
                        trkskim.trkWeight_mix.push_back(1);
                        trkskim.evttrk_mix.push_back(nMixed);
                        trkskim.nTrk_mix++;
                    }

                    if (isMC) {
                        for (int i = 0; i < hiGenMix[iMF].mult; ++i) {

                            if (!((*hiGenMix[iMF].pt)[i] > trkPtMin))  continue;
                            if (!(std::fabs((*hiGenMix[iMF].eta)[i]) < trkEtaMax))  continue;

                            trkskim.pt_mix.push_back((*hiGenMix[iMF].pt)[i]);
                            trkskim.eta_mix.push_back((*hiGenMix[iMF].eta)[i]);
                            trkskim.phi_mix.push_back((*hiGenMix[iMF].phi)[i]);
                            trkskim.pdg_mix.push_back((*hiGenMix[iMF].pdg)[i]);
                            trkskim.chg_mix.push_back((*hiGenMix[iMF].chg)[i]);
                            trkskim.sube_mix.push_back((*hiGenMix[iMF].sube)[i]);
                            trkskim.evtgen_mix.push_back(nMixed);
                            trkskim.mult_mix++;
                        }
                    }

                    nMixed++;
                    iME = (iME+1 == nMixEventsAvail) ? 0 : iME+1;
                    if ((nMixed >= nMixEvents) ||
                        (!centsAdjacentAvail && nMixed >= nMixEventsTmp)) {
                        entriesMixedEvent[iCent][ivz][iEventPlane].current = iME;
                        std::cout << "End mixing at indexMixEvent = " << iME << std::endl;
                        break;
                    }
                }
            }

            outTreeHLT->Fill();
            outTreeggHiNtuplizer->Fill();
            outTreeHiEvt->Fill();
            eventSkimTree->Fill();
            mixEventSkimTree->Fill();
            for (int i=0; i<nJetCollections; ++i) {
                jetSkimTrees[i]->Fill();
            }
            trackSkimTree->Fill();
        }
        fileTmp->Close();
    }
    for (int i = 0; i < nMixFiles; ++i) {
        if (isMixFileGood[i])  {
            mixFiles[i]->Close();
        }
    }

    std::cout<<  "Loop ENDED : " << treePath.c_str() <<std::endl;
    std::cout << "nFilesSkipped = " << nFilesSkipped << std::endl;
    std::cout << "entries             = " << entries << std::endl;
    std::cout << "duplicateEntries    = " << duplicateEntries << std::endl;
    std::cout << "duplicateEntriesMix = " << duplicateEntriesMix << std::endl;
    std::cout << "entriesAnalyzed     = " << entriesAnalyzed << std::endl;
    std::cout << "entriesSelected     = " << entriesSelected << std::endl;

    std::cout << "outTreeHLT->GetEntries()           = " << outTreeHLT->GetEntries() << std::endl;
    std::cout << "outTreeggHiNtuplizer->GetEntries() = " << outTreeggHiNtuplizer->GetEntries() << std::endl;
    std::cout << "outTreeHiEvt->GetEntries()         = " << outTreeHiEvt->GetEntries() << std::endl;
    std::cout << "mixEventSkimTree->GetEntries()     = " << mixEventSkimTree->GetEntries() << std::endl;
    for (int i=0; i<nJetCollections; ++i) {
        std::cout << Form("jetSkimTrees[%d]->GetEntries()      = ", i) << jetSkimTrees[i]->GetEntries() << std::endl;
    }
    std::cout << "trackSkimTree->GetEntries()     = " << trackSkimTree->GetEntries() << std::endl;

    output->cd();
    std::cout<<"Writing the output file."<<std::endl;
    output->Write("",TObject::kOverwrite);
    std::cout<<"Closing the output file."<<std::endl;
    output->Close();

    std::cout<<"running vJetTrkSkim() - END"<<std::endl;
}

int main(int argc, char** argv)
{
    std::vector<std::string> argStr = ArgumentParser::ParseParameters(argc, argv);
    int nArgStr = argStr.size();

    argOptions = ArgumentParser::ParseOptions(argc, argv);

    if (nArgStr == 5) {
        vJetTrkSkim(argStr.at(1), argStr.at(2), argStr.at(3), argStr.at(4));
        return 0;
    }
    else if (nArgStr == 4) {
        vJetTrkSkim(argStr.at(1), argStr.at(2), argStr.at(3), argStr.at(4));
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./vJetTrkSkim.exe <configFile> <inputFile> <outputFile> <mixFile>"
                << std::endl;
        return 1;
    }
}

int readConfiguration(std::string configFile, std::string inputFile)
{
    ConfigurationParser confParser;
    confParser.openConfigFile(configFile);

    confParser.parsedKeyWords = InputConfigurationParser::parseKeyWords(inputFile);

    VJT::ewObj = parseEWObj(confParser.ReadConfigValue("ewObj"));

    VJT::mixMethod = parseMixMethod(confParser.ReadConfigValue("mixMethod"));

    treePath = confParser.ReadConfigValue("treePath");
    sampleType = confParser.ReadConfigValue("sampleType");

    jetCollections = ConfigurationParser::ParseList(confParser.ReadConfigValue("jetCollections"));

    // event cuts/weights
    pthatWeights = ConfigurationParser::ParseListTriplet(confParser.ReadConfigValue("pthatWeights"));

    nMixEvents = confParser.ReadConfigValueInteger("nMixEvents");

    // RECO cuts
    excludeHI18HEMfailure = (confParser.ReadConfigValueInteger("excludeHI18HEMfailure") > 0);

    nJetCollections = jetCollections.size();

    nPthatWeights = pthatWeights[0].size();

    return 0;
}

/*
 * print information read from configuration
 * assumes that readConfiguration() is run before
 */
void printConfiguration()
{
    std::cout<<"Configuration :"<<std::endl;
    std::cout << "EW object = " << VJT::ewObjsStr[VJT::ewObj].c_str() << std::endl;
    if (VJT::ewObj < 0 || VJT::ewObj >= VJT::EWOBJS::kN_EWOBJS) {
        std::cout << "ERROR : no valid EW object given" << std::endl;
        std::cout << "ewObj (index for EW object) = " << VJT::ewObj << std::endl;
    }

    std::cout << "mix method = " << VJT::mixMerhodsStr[VJT::mixMethod].c_str() << std::endl;
    if (VJT::mixMethod < 0 || VJT::mixMethod >= VJT::MIXMETHODS::kN_MITMETHODS) {
        std::cout << "ERROR : no valid mix method object given" << std::endl;
        std::cout << "mixMethod (index for mix method) = " << VJT::mixMethod << std::endl;
    }

    std::cout << "treePath = " << treePath.c_str() << std::endl;
    std::cout << "sampleType = " << sampleType.c_str() << std::endl;

    std::cout << "nJetCollections  = " << nJetCollections << std::endl;
    for (int i = 0; i < nJetCollections; ++i) {
        std::cout << Form("jetCollections[%d] = %s", i, jetCollections.at(i).c_str()) << std::endl;
    }

    std::cout << "nPthatWeights = " << nPthatWeights << std::endl;
    for (int i = 0; i < nPthatWeights; ++i) {
        std::cout << Form("pthatWeights[%d] = { ", i);
        std::cout << Form("%.0f, ", pthatWeights[0].at(i));
        std::cout << Form("%f, ", pthatWeights[1].at(i));
        std::cout << Form("%f }", pthatWeights[2].at(i)) << std::endl;;
    }

    std::cout << "nMixEvents = " << nMixEvents << std::endl;

    std::cout << "excludeHI18HEMfailure = " << excludeHI18HEMfailure << std::endl;
}
