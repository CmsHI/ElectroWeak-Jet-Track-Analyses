/*
 * macro to print Run, Event, Lumi numbers for a given event selection.
 *
 * 09.06.2016 : quit EventList approach for now. I experienced some problems with reading the EventList from gDirectory. That happened when "tree" was not in the top directory, if "tree" is in top directory, then there is no problem.
 *      Example :
 *         input.performance.treePath = EventTree --> "tree" is in top directory --> EventList works
 *         input.performance.treePath = ggHiNtuplizer/EventTree --> "tree" is not in top directory --> EventList does not work
 *         --> gives : "Error in <TChain::SetEntryList>: No list found for the trees in this chain"
 *
 */

#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TCut.h>
#include <TEventList.h>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>      // ifstream, ofstream
#include <ios>      // ifstream, ofstream

#include "../TreeHeaders/CutConfigurationTree.h"
#include "../Utilities/interface/CutConfigurationParser.h"
#include "../Utilities/interface/InputConfigurationParser.h"
#include "../Utilities/interface/HiForestInfoController.h"
#include "../Utilities/fileUtil.h"

void printRunLumiEvent(std::string configFile, std::string inputFile, std::string outputFile = "printRunLumiEvent.txt");

void printRunLumiEvent(std::string configFile, std::string inputFile, std::string outputFile)
{
    std::cout<<"running printRunLumiEvent()"<<std::endl;
    std::cout<<"configFile  = "<< configFile.c_str() <<std::endl;
    std::cout<<"inputFile   = "<< inputFile.c_str()  <<std::endl;
    std::cout<<"outputFile  = "<< outputFile.c_str() <<std::endl;

    InputConfiguration configInput = InputConfigurationParser::Parse(configFile.c_str());
    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.c_str());

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

    // formula for run,lumi,event
    std::string formula = configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treeFormula];
    std::string selection = configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treeSelection];

    std::string treePath = configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treePath];
    std::vector<std::string> treeFriendsPath = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treeFriendPath]);

    if (formula.size() == 0) formula = "run:lumis:event";   // default formula, taken from ggHiNtuplizer/EventTree

    int nFriends = treeFriendsPath.size();
    // verbose about input configuration
    std::cout << "formula = " << formula.c_str() << std::endl;
    std::cout << "selection = " << selection.c_str() << std::endl;
    std::cout << "treePath = " << treePath.c_str() << std::endl;
    std::cout << "nFriends = " << nFriends << std::endl;
    for (int i=0; i<nFriends; ++i) {
        std::cout << Form("treeFriends[%d] = %s", i, treeFriendsPath.at(i).c_str()) << std::endl;
    }

    // cut configuration

    // verbose about cut configuration
    std::cout<<"Cut Configuration :"<<std::endl;

    std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.c_str());

    std::cout<<"input ROOT files : num = "<<inputFiles.size()<< std::endl;
    std::cout<<"#####"<< std::endl;
    for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
        std::cout<<(*it).c_str()<< std::endl;
    }
    std::cout<<"##### END #####"<< std::endl;

    TTree* tree;
    TTree* treeFriends[nFriends];
    TTree* treeHiForestInfo;

    int nFiles = inputFiles.size();
    TFile* fileTmp = 0;
    Long64_t entries = 0;
    Long64_t entriesSelected = 0;

    std::cout << "initial reading to get the number of entries (if there is only one input file) and HiForest info" << std::endl;
    // read the first file only to get the HiForest info
    std::string inputPath = inputFiles.at(0).c_str();
    fileTmp = new TFile(inputPath.c_str(), "READ");

    if (nFiles == 1) {
        // read one tree only to get the number of entries
        tree = (TTree*)fileTmp->Get(treePath.c_str());
        entries = tree->GetEntries();
        std::cout << "entries = " << entries << std::endl;
    }

    treeHiForestInfo = (TTree*)fileTmp->Get("HiForest/HiForestInfo");
    HiForestInfoController hfic(treeHiForestInfo);
    std::cout<<"### HiForestInfo Tree ###"<< std::endl;
    hfic.printHiForestInfo();
    std::cout<<"###"<< std::endl;

    fileTmp->Close();
    // done with initial reading

    std::cout << "TTree::Draw() : " << treePath.c_str() <<std::endl;
    for (int iFile = 0; iFile < nFiles; ++iFile) {

        std::string inputPath = inputFiles.at(iFile).c_str();
        std::cout <<"iFile = " << iFile << " , " ;
        std::cout <<"reading input file : " << inputPath.c_str() << std::endl;
        fileTmp = new TFile(inputPath.c_str(), "READ");

        // check if the file is usable, if not skip the file.
        if (isGoodFile(fileTmp) != 0) {
            std::cout << "File is not good. skipping file." << std::endl;
            continue;
        }

       tree = (TTree*)fileTmp->Get(treePath.c_str());
       for (int i=0; i<nFriends; ++i) {
           treeFriends[i] =(TTree*)fileTmp->Get(treeFriendsPath.at(i).c_str());
           tree->AddFriend(treeFriends[i], Form("t%d", i));
       }

       Long64_t entriesTmp = tree->GetEntries();
       entries += entriesTmp;
       std::cout << "entries in File = " << entriesTmp << std::endl;
       tree->Draw(formula.c_str(), Form("Sum$(%s) > 0", selection.c_str()), "goff");

       int nentriesTmp = (Int_t)tree->GetSelectedRows();
       entriesSelected += nentriesTmp;
       std::cout << "selected entries in File = " << nentriesTmp << std::endl;
       std::vector<UInt_t> vecRun   (tree->GetV1(), tree->GetV1()+nentriesTmp);
       std::vector<UInt_t> vecLumis (tree->GetV2(), tree->GetV2()+nentriesTmp);
       std::vector<UInt_t> vecEvent (tree->GetV3(), tree->GetV3()+nentriesTmp);

       // std::cout << "opening file : " << outputFile.Data() << std::endl;
       std::ios_base::openmode openMode = std::ios_base::out | std::ios_base::trunc;    // default mode, overwrites existing file
       if (iFile > 0)  openMode = std::ios_base::app | std::ios_base::out;      // append mode
       std::ofstream outFileStream(outputFile.c_str(), openMode);

       if (outFileStream.is_open()) {
           if (iFile == 0) outFileStream << "# run lumis event" << std::endl;
           for (int i = 0; i<nentriesTmp; ++i) {
               outFileStream << vecRun.at(i) << " " << vecLumis.at(i) << " " << vecEvent.at(i) << std::endl;
               std::cout << vecRun.at(i) << " " << vecLumis.at(i) << " " << vecEvent.at(i) << std::endl;
           }
       }
       outFileStream.close();

       fileTmp->Close();
    }
    std::cout << "TTree::Draw() ENDED : " << treePath.c_str() <<std::endl;
    std::cout << "entries = " << entries <<std::endl;
    std::cout << "entriesSelected = " << entriesSelected <<std::endl;
    std::cout << "wrote file   : " << outputFile.c_str() << std::endl;
}

int main(int argc, char** argv)
{
    if (argc == 4) {
        printRunLumiEvent(argv[1], argv[2], argv[3]);
        return 0;
    }
    else if (argc == 3) {
        printRunLumiEvent(argv[1], argv[2]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./printRunLumiEvent.exe <configFile> <inputFile> <outputFile>"
                << std::endl;
        return 1;
    }
}
