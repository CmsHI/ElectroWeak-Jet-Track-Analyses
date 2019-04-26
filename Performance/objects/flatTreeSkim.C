/*
 * code to make a flat TTree skim of electron/photon, and event information
 */

#include <TFile.h>
#include <TTree.h>
#include <TObjArray.h>
#include <TMath.h>
#include <TLorentzVector.h>

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>

#include "../../CorrelationTuple/EventMatcher.h"
#include "../../TreeHeaders/ggHiNtuplizerTree.h"
#include "../../TreeHeaders/hiEvtTree.h"
#include "../../TreeHeaders/skimAnalysisTree.h"
#include "../../TreeHeaders/ggHiFlatTree.h"
#include "../../Utilities/interface/ArgumentParser.h"
#include "../../Utilities/interface/ConfigurationParser.h"
#include "../../Utilities/interface/InputConfigurationParser.h"
#include "../../Utilities/interface/HiForestInfoController.h"
#include "../../Utilities/eventUtil.h"
#include "../../Utilities/fileUtil.h"
#include "../../Utilities/physicsUtil.h"

///// global variables
/// configuration variables

// TTree
std::string inputTreePath;
std::string outputTreePath;

int collisionType;

std::string collisionName;

// event cuts/weights
std::vector<std::vector<float>> pthatWeights;

// object cuts
float ptMin;
float ptMax;
float etaMin;
float etaMax;

int nPthatWeights;

/// configuration variables - END
enum RECOOBJS {
    kPhoton,
    kElectron,
    kN_RECOOBJS
};
const std::string recoObjsStr[kN_RECOOBJS] = {"photon", "electron"};
int recoObj;
///// global variables - END

int readConfiguration(std::string configFile, std::string inputFile);
void printConfiguration();
int parseRecoObj(std::string recoObjStr);
void flatTreeSkim(std::string configFile, std::string inputFile, std::string outputFile = "flatTreeSkim.root");

void flatTreeSkim(std::string configFile, std::string inputFile, std::string outputFile)
{
    std::cout<<"running flatTreeSkim()"<<std::endl;
    std::cout<<"configFile  = "<< configFile.c_str() <<std::endl;
    std::cout<<"inputFile   = "<< inputFile.c_str()  <<std::endl;
    std::cout<<"outputFile  = "<< outputFile.c_str() <<std::endl;

    if (readConfiguration(configFile, inputFile) != 0)  return;
    printConfiguration();

    std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.c_str());
    std::cout<<"input ROOT files : num = "<<inputFiles.size()<< std::endl;
    std::cout<<"#####"<< std::endl;
    for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
        std::cout<<(*it).c_str()<< std::endl;
    }
    std::cout<<"##### END #####"<< std::endl;

    bool isMC = collisionIsMC((COLL::TYPE)collisionType);
    bool isHI15 = collisionIsHI((COLL::TYPE)collisionType);
    bool isHI18 = collisionIsHI2018((COLL::TYPE)collisionType);
    //bool isHI = (isHI15 || isHI18);
    bool isPP = collisionIsPP((COLL::TYPE)collisionType);

    if (!isMC) {
        std::cout << "This macro runs on simulation samples only." << std::endl;
        std::cout << "Change the collisionType to a simulated collisions." << std::endl;
        std::cout << "exiting" << std::endl;
        return;
    }

    TFile* output = TFile::Open(outputFile.c_str(),"UPDATE");
    output->cd();

    TTree* outputTree = new TTree(outputTreePath.c_str(), Form("skim of %s", inputTreePath.c_str()));

    ggHiFlat ggHiOut;
    ggHiOut.doEle = (recoObj == RECOOBJS::kElectron);
    ggHiOut.doPho = (recoObj == RECOOBJS::kPhoton);
    ggHiOut.doMC = (recoObj == RECOOBJS::kPhoton && isMC);
    ggHiOut.setupTreeForWriting(outputTree);

    int nFiles = inputFiles.size();
    TFile* fileTmp = 0;
    std::cout << "initial reading to get the number of entries (if there is only one input file) and HiForest info" << std::endl;
    // read the first file only to get the HiForest info
    std::string inputPath = inputFiles.at(0).c_str();
    fileTmp = TFile::Open(inputPath.c_str(), "READ");
    fileTmp->cd();

    TTree* treeggHiNtuplizer = 0;
    TTree* treeHiEvt = 0;
    TTree* treeHiForestInfo = 0;
    TTree* treeSkim = 0;

    if (nFiles == 1) {
        // read one tree only to get the number of entries
        treeggHiNtuplizer = (TTree*)fileTmp->Get(inputTreePath.c_str());
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

    EventMatcher* em = new EventMatcher();
    Long64_t duplicateEntries = 0;

    Long64_t entries = 0;
    Long64_t entriesAnalyzed = 0;
    Long64_t objectsSkimmed = 0;

    std::cout<< "Loop : " << inputTreePath.c_str() <<std::endl;
    for (int iFile = 0; iFile < nFiles; ++iFile)  {

        std::string inputPath = inputFiles.at(iFile).c_str();
        std::cout <<"iFile = " << iFile << " , " ;
        std::cout <<"reading input file : " << inputPath.c_str() << std::endl;
        fileTmp = TFile::Open(inputPath.c_str(), "READ");

        // check if the file is usable, if not skip the file.
        if (isGoodFile(fileTmp) != 0) {
            std::cout << "File is not good. skipping file." << std::endl;
            continue;
        }

        treeggHiNtuplizer = (TTree*)fileTmp->Get(inputTreePath.c_str());
        treeggHiNtuplizer->SetBranchStatus("*",0);     // disable all branches
        if (recoObj == RECOOBJS::kPhoton) {
            treeggHiNtuplizer->SetBranchStatus("nPho",1);     // enable photon branches
            treeggHiNtuplizer->SetBranchStatus("pho*",1);     // enable photon branches
            treeggHiNtuplizer->SetBranchStatus("pf*",1);     // enable photon branches
        }
        else if (recoObj == RECOOBJS::kElectron) {
            treeggHiNtuplizer->SetBranchStatus("nEle",1);
            treeggHiNtuplizer->SetBranchStatus("ele*",1);
        }
        treeggHiNtuplizer->SetBranchStatus("nMC*",1);     // enable GEN particle branches
        treeggHiNtuplizer->SetBranchStatus("mc*",1);      // enable GEN particle branches
        // check existence of genMatching branch
        if (!treeggHiNtuplizer->GetBranch("pho_genMatchedIndex")) {
            std::cout << "WARNING : Branch pho_genMatchedIndex does not exist." <<std::endl;
        }

        // specify explicitly which branches to use, do not use wildcard
        treeHiEvt = (TTree*)fileTmp->Get("hiEvtAnalyzer/HiTree");
        treeHiEvt->SetBranchStatus("*",0);     // disable all branches
        treeHiEvt->SetBranchStatus("run",1);   // enable event information
        treeHiEvt->SetBranchStatus("evt",1);
        treeHiEvt->SetBranchStatus("lumi",1);
        treeHiEvt->SetBranchStatus("vz",1);
        treeHiEvt->SetBranchStatus("hiBin",1);
        bool doEventWeight = (nPthatWeights > 0);
        if (doEventWeight) {
            treeHiEvt->SetBranchStatus("weight", 1);
        }
        if (nPthatWeights > 0) {
            treeHiEvt->SetBranchStatus("pthat",1);
        }

        treeSkim = (TTree*)fileTmp->Get("skimanalysis/HltTree");
        treeSkim->SetBranchStatus("*",0);     // disable all branches

        ggHiNtuplizer ggHi;
        ggHi.setupTreeForReading(treeggHiNtuplizer);

        hiEvt hiEvt;
        hiEvt.setupTreeForReading(treeHiEvt);

        skimAnalysis skimAna;
        if (isHI15) skimAna.enableBranchesHI(treeSkim);
        else if (isHI18) skimAna.enableBranchesHI2018(treeSkim);
        else if (isPP) skimAna.enableBranchesPP(treeSkim);
        skimAna.setupTreeForReading(treeSkim);
        skimAna.checkBranches(treeSkim);    // do the event selection if the branches exist.

        Long64_t entriesTmp = treeggHiNtuplizer->GetEntries();
        entries += entriesTmp;
        std::cout << "entries in File = " << entriesTmp << std::endl;
        for (Long64_t j_entry = 0; j_entry < entriesTmp; ++j_entry)
        {
            if (j_entry % 2000 == 0)  {
                std::cout << "current entry = " <<j_entry<<" out of "<<entriesTmp<<" : "<<std::setprecision(2)<<(double)j_entry/entriesTmp*100<<" %"<<std::endl;
            }

            treeggHiNtuplizer->GetEntry(j_entry);
            treeHiEvt->GetEntry(j_entry);
            treeSkim->GetEntry(j_entry);

            bool eventAdded = em->addEvent(hiEvt.run, hiEvt.lumi, hiEvt.evt, j_entry);
            if(!eventAdded) // this event is duplicate, skip this one.
            {
                duplicateEntries++;
                continue;
            }

            // event selection
            if (!(TMath::Abs(hiEvt.vz) < 15))  continue;

            if (isHI15 && !skimAna.passedEventSelectionHI())  continue;
            else if (isHI18 && !skimAna.passedEventSelectionHI2018())  continue;
            else if (isPP && !skimAna.passedEventSelectionPP())  continue;

            entriesAnalyzed++;

            double w = 1;
            /*
            int hiBin = hiEvt.hiBin;
            int cent = hiBin/2;
             */
            if (doEventWeight) {
                w = hiEvt.weight;
                /*
                double vertexWeight = 1;
                if (isHI && isMC)  vertexWeight = 1.37487*TMath::Exp(-0.5*TMath::Power((hiEvt.vz-0.30709)/7.41379, 2));  // 02.04.2016
                double centWeight = 1;
                if (isHI && isMC)  centWeight = findNcoll(hiBin);
                w *= vertexWeight * centWeight;
                 */
            }

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

            if (recoObj == RECOOBJS::kPhoton) {
                for (int i=0; i<ggHi.nPho; ++i) {

                    ggHiOut.clearEntry();
                    ggHiOut.weight = w;
                    ggHiOut.hiBin = hiEvt.hiBin;

                    if (!((*ggHi.phoEt)[i] >= ptMin))  continue;
                    if (ptMax > 0 && !((*ggHi.phoEt)[i] < ptMax))  continue;

                    if (!(TMath::Abs((*ggHi.phoEta)[i]) >= etaMin))  continue;
                    if (etaMax > 0 && !(TMath::Abs((*ggHi.phoEta)[i]) < etaMax))  continue;

                    ggHiOut.copyPho(ggHi, i);

                    int genMatchedIndex = (*ggHi.pho_genMatchedIndex)[i];
                    if (genMatchedIndex >= 0) {
                        ggHiOut.copyGen(ggHi, genMatchedIndex);
                    }

                    outputTree->Fill();
                    objectsSkimmed++;
                }
            }
        }
        fileTmp->Close();
    }
    std::cout<<  "Loop ENDED : " << inputTreePath.c_str() <<std::endl;
    std::cout << "entries            = " << entries << std::endl;
    std::cout << "duplicateEntries   = " << duplicateEntries << std::endl;
    std::cout << "entriesAnalyzed    = " << entriesAnalyzed << std::endl;
    std::cout << "objectsSkimmed    = " << objectsSkimmed << std::endl;

    // overwrite existing trees
    output->cd();
    outputTree->Write("", TObject::kOverwrite);

    std::cout<<"Writing the output file."<<std::endl;
    output->Write("",TObject::kOverwrite);
    std::cout<<"Closing the output file."<<std::endl;
    output->Close();
    std::cout<<"running flatTreeSkim() - END"<<std::endl;
}

int main(int argc, char** argv)
{
    std::vector<std::string> argStr = ArgumentParser::ParseParameters(argc, argv);
    int nArgStr = argStr.size();

    std::vector<std::string> argOptions = ArgumentParser::ParseOptions(argc, argv);

    if (nArgStr == 4) {
        flatTreeSkim(argStr.at(1), argStr.at(2), argStr.at(3));
        return 0;
    }
    else if (nArgStr == 3) {
        flatTreeSkim(argStr.at(1), argStr.at(2));
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./flatTreeSkim.exe <configFile> <inputFile> <outputFile>"
                << std::endl;
        return 1;
    }
}

int readConfiguration(std::string configFile, std::string inputFile)
{
    ConfigurationParser confParser;
    confParser.openConfigFile(configFile);

    confParser.parsedKeyWords = InputConfigurationParser::parseKeyWords(inputFile);

    recoObj = parseRecoObj(confParser.ReadConfigValue("recoObj"));

    // TTree
    inputTreePath = confParser.ReadConfigValue("inputTreePath");
    outputTreePath = confParser.ReadConfigValue("outputTreePath");

    collisionType = confParser.ReadConfigValueInteger("collisionType");

    // event cuts/weights
    pthatWeights = ConfigurationParser::ParseListTriplet(confParser.ReadConfigValue("pthatWeights"));

    // object cuts
    ptMin = confParser.ReadConfigValueFloat("ptMin");
    ptMax = confParser.ReadConfigValueFloat("ptMax");
    etaMin = confParser.ReadConfigValueFloat("etaMin");
    etaMax = confParser.ReadConfigValueFloat("etaMax");

    nPthatWeights = pthatWeights[0].size();

    // set default values
    if (inputTreePath.size() == 0) inputTreePath = "ggHiNtuplizer/EventTree";
    if (outputTreePath.size() == 0) outputTreePath = "treeSignal";

    if (ptMax == 0)  ptMax = -1;
    if (etaMax == 0)  etaMax = -1;

    collisionName = getCollisionTypeName((COLL::TYPE)collisionType).c_str();

    return 0;
}

/*
 * print information read from input/cut configurations
 * assumes that readConfiguration() is run before
 */
void printConfiguration()
{
    std::cout<<"Configuration :"<<std::endl;
    std::cout << "reco object = " << recoObjsStr[recoObj].c_str() << std::endl;
    if (recoObj < 0 || recoObj >= RECOOBJS::kN_RECOOBJS) {
        std::cout << "ERROR : no valid reco object given" << std::endl;
        std::cout << "recoObj (index for reco object) = " << recoObj << std::endl;
    }

    std::cout << "inputTreePath = " << inputTreePath.c_str() << std::endl;
    std::cout << "outputTreePath = " << outputTreePath.c_str() << std::endl;

    std::cout << "collision = " << collisionName.c_str() << std::endl;

    std::cout << "nPthatWeights = " << nPthatWeights << std::endl;
    for (int i = 0; i < nPthatWeights; ++i) {
        std::cout << Form("pthatWeights[%d] = { ", i);
        std::cout << Form("%.0f, ", pthatWeights[0].at(i));
        std::cout << Form("%f, ", pthatWeights[1].at(i));
        std::cout << Form("%f }", pthatWeights[2].at(i)) << std::endl;;
    }

    std::cout << "ptMin = " << ptMin << std::endl;
    std::cout << "ptMax = " << ptMax << std::endl;
    std::cout << "etaMin = " << etaMin << std::endl;
    std::cout << "etaMax = " << etaMax << std::endl;
}

int parseRecoObj(std::string recoObjStr)
{
    recoObjStr = trim(recoObjStr);
    recoObjStr = toLowerCase(recoObjStr);

    if (recoObjStr == "pho" || recoObjStr == "photon" || recoObjStr == "0") {
        return RECOOBJS::kPhoton;
    }
    else if (recoObjStr == "ele" || recoObjStr == "electron" || recoObjStr == "1") {
        return RECOOBJS::kElectron;
    }
    else {
        return -1;
    }
}


