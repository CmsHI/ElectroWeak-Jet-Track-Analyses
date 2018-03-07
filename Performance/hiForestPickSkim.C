/*
 * macro to pick events from HiForest and/or to skim a subset of the HiForest trees.
 */

#include <TKey.h>
#include <TFile.h>
#include <TTree.h>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>      // ifstream, ofstream
#include <iomanip>

#include "../CorrelationTuple/EventMatcher.h"
#include "../TreeHeaders/CutConfigurationTree.h"
#include "../Utilities/interface/CutConfigurationParser.h"
#include "../Utilities/interface/InputConfigurationParser.h"
#include "../Utilities/interface/HiForestInfoController.h"
#include "../Utilities/fileUtil.h"

void hiForestPickSkim(std::string configFile, std::string eventList, std::string inputFile, std::string outputFile = "hiForestPickSkim.txt");

void hiForestPickSkim(std::string configFile, std::string eventList, std::string inputFile, std::string outputFile)
{
    std::cout<<"running hiForestPickSkim()"<<std::endl;
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
    if (treePath.size() == 0) {
        std::cout << "no tree path is given for the tree containing run, lumi, event information." << std::endl;
        std::string treePathDefault = "ggHiNtuplizer/EventTree";
        std::cout << "set treePath to " << treePathDefault.c_str() << std::endl;
        treePath = treePathDefault.c_str();
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

    std::cout << "### Event list info ###" << std::endl;
    std::vector<std::string> vecRunLumiEvent = InputConfigurationParser::ParseEvents(eventList.c_str());
    int nRunLumiEvent = vecRunLumiEvent.size();
    if (nRunLumiEvent == 0) {
        std::cout << "Event list is empty." << std::endl;
        std::cout << "exiting" << std::endl;
        return;
    }
    std::cout << "nEventsToSelect = " << nRunLumiEvent << std::endl;
    std::cout<<"Event List : run lumi event"<<std::endl;
    std::cout<<"#####"<< std::endl;
    for (std::vector<std::string>::iterator it = vecRunLumiEvent.begin() ; it != vecRunLumiEvent.end(); ++it) {
        std::cout<<(*it).c_str()<< std::endl;
    }
    std::cout<<"##### END #####"<< std::endl;

    // list of events to be picked.
    std::vector<UInt_t> vecRun;
    std::vector<UInt_t> vecLumi;
    std::vector<ULong64_t> vecEvent;
    for (std::vector<std::string>::iterator it = vecRunLumiEvent.begin() ; it != vecRunLumiEvent.end(); ++it) {
        vecRun.push_back(ConfigurationParser::ParseRunNumber((*it)));
        vecLumi.push_back(ConfigurationParser::ParseLumiNumber((*it)));
        vecEvent.push_back(ConfigurationParser::ParseEventNumber((*it)));
    }

    EventMatcher* em = new EventMatcher();
    int nEvents = vecEvent.size();
    for (int i = 0; i < nEvents; ++i) {
        em->addEvent(vecRun[i], vecLumi[i], vecEvent[i], i);
    }

    TFile* fileTmp = 0;
    // read the first file in order to
    // 1. extract the list of trees
    // 2 to get the HiForest info
    std::cout << "initial reading to get the list of trees and HiForest info" << std::endl;
    fileTmp = TFile::Open(inputFiles.at(0).c_str(), "READ");
    TList* listTTree = (TList*)getListOfALLKeys(fileTmp, "TTree");

    TTree* treeHiForestInfo = (TTree*)fileTmp->Get("HiForest/HiForestInfo");
    if (!treeHiForestInfo) {
        std::cout << "HiForest/HiForestInfo tree is not found." << std::endl;
    }
    else {
        HiForestInfoController hfic(treeHiForestInfo);
        std::cout<<"### HiForestInfo Tree ###"<< std::endl;
        hfic.printHiForestInfo();
        std::cout<<"###"<< std::endl;
    }

    TFile* output = new TFile(outputFile.c_str(), "RECREATE");
    int nTrees = listTTree->GetSize();

    std::cout << "nTrees = " << nTrees << std::endl;

    std::vector<std::string> directoryPaths(nTrees);
    std::vector<std::string> directoryTitles(nTrees);
    std::vector<std::string> treePaths(nTrees);

    TTree* inputTrees[nTrees];
    TTree* inputTreeEvent = 0;
    TTree* outputTrees[nTrees];

    // determine the tree paths and initialize output paths
    TKey* key = 0;
    for (int iKey = 0; iKey < nTrees; ++iKey)
    {
        key = (TKey*)listTTree->At(iKey);
        std::string keyName = key->GetName();

        // get the directory where the TTree is.
        std::string directoryPath = key->GetMotherDir()->GetPath();
        std::string directoryTitle = key->GetMotherDir()->GetTitle();
        std::string filePath = key->GetFile()->GetPath();

        directoryPath = replaceAll(directoryPath, filePath, "");
        directoryPaths.at(iKey) = directoryPath;
        directoryTitles.at(iKey) = directoryTitle;

        std::string treePathTmp = Form("%s/%s", directoryPath.c_str(), keyName.c_str());
        treePaths.at(iKey) = treePathTmp;

        // prepare output directories / trees
        output->cd();
        if (directoryPath != "") {
            if (!output->GetKey(directoryPath.c_str())) output->mkdir(directoryPath.c_str(), directoryTitle.c_str());
        }
    }
    fileTmp->Close();

    int nFiles = inputFiles.size();
    Long64_t entries = 0;
    Long64_t entriesSelected = 0;
    std::cout<< "Loop : " << treePath.c_str() <<std::endl;
    for (int iFile = 0; iFile < nFiles; ++iFile) {

        std::string inputPath = inputFiles.at(iFile).c_str();
        std::cout <<"iFile = " << iFile << " , " ;
        std::cout <<"reading input file : " << inputPath.c_str() << std::endl;
        fileTmp = TFile::Open(inputPath.c_str(), "READ");

        // check if the file is usable, if not skip the file.
        if (isGoodFile(fileTmp) != 0) {
            std::cout << "File is not good. skipping file." << std::endl;
            continue;
        }

        bool treeExists = true;
        for (int iTree = 0; iTree < nTrees; ++iTree) {
            std::string treePathTmp = treePaths.at(iTree).c_str();
            inputTrees[iTree] = (TTree*)fileTmp->Get(treePathTmp.c_str());

            if (!inputTrees[iTree]) {
                std::cout << "tree is not found in the path : "<< treePathTmp.c_str() <<". skipping file." << std::endl;
                treeExists = false;
            }
        }
        if (!treeExists)  continue;

        output->cd();
        // copy the trees for output
        for (int iTree = 0; iTree < nTrees; ++iTree) {
            std::string directoryPath = directoryPaths.at(iTree).c_str();

            // directories in output file were initialized before.
            output->cd(directoryPath.c_str());

            if (iFile == 0)  outputTrees[iTree] = inputTrees[iTree]->CloneTree(0);
            else             inputTrees[iTree]->CopyAddresses(outputTrees[iTree]);
        }


        // tree to read event info
        std::string treePathEvent = treePath.c_str();
        inputTreeEvent = (TTree*)fileTmp->Get(treePathEvent.c_str());
        if (!inputTreeEvent) {
            std::cout << Form("Error : tree with event information not found on the path %s", treePathEvent.c_str()) << std::endl;
            std::cout <<". skipping file." << std::endl;
            treeExists = false;
        }
        if (!treeExists)  continue;

        inputTreeEvent->SetBranchStatus("run",1);    // enable event information
        inputTreeEvent->SetBranchStatus("event",1);
        inputTreeEvent->SetBranchStatus("lumis",1);

        UInt_t    run;
        UInt_t    lumis;
        ULong64_t event;
        inputTreeEvent->SetBranchAddress("run", &run);
        inputTreeEvent->SetBranchAddress("lumis", &lumis);
        inputTreeEvent->SetBranchAddress("event", &event);

        Long64_t entriesTmp = inputTreeEvent->GetEntries();
        entries += entriesTmp;
        std::cout << "entries in File = " << entriesTmp << std::endl;

        for (Long64_t j_entry=0; j_entry<entriesTmp; ++j_entry)
        {
            if (j_entry % 20000 == 0)  {
                std::cout << "current entry = " <<j_entry<<" out of "<<entriesTmp<<" : "<<std::setprecision(2)<<(double)j_entry/entriesTmp*100<<" %"<<std::endl;
            }

            inputTreeEvent->GetEntry(j_entry);

            // remove the events that are selected so that a smaller map is checked in the next iteration.
            // if the event cannot be removed, then it is not in the event list.
            if (!em->removeEvent(run, lumis, event))  continue;

            entriesSelected++;
            // read entries only for events that are selected.
            for (int iTree = 0; iTree < nTrees; ++iTree) {
                inputTrees[iTree]->GetEntry(j_entry);
            }

            for (int iTree = 0; iTree < nTrees; ++iTree) {
                outputTrees[iTree]->Fill();
            }
        }

        fileTmp->Close();
    }
    Long64_t entriesNotFound = em->getSize();

    std::cout<< "Loop ENDED : " << treePath.c_str() <<std::endl;
    std::cout << "entries            = " << entries << std::endl;
    std::cout << "entriesSelected    = " << entriesSelected << std::endl;
    std::cout << "entriesNotFound    = " << entriesNotFound << std::endl;
    for (int iTree = 0; iTree < nTrees; ++iTree) {

        std::string treePath = outputTrees[iTree]->GetName();
        std::string directoryPath = directoryPaths.at(iTree).c_str();
        if (directoryPath.size() > 0)
            treePath = Form("%s/%s", directoryPath.c_str(), outputTrees[iTree]->GetName());
        std::cout << "Entries = " << outputTrees[iTree]->GetEntries() << Form(" : %s", treePath.c_str()) << std::endl;

        outputTrees[iTree]->Write("", TObject::kOverwrite);
    }

    output->Write("", TObject::kOverwrite);
    std::cout<<"Closing the output file."<<std::endl;
    output->Close();

    std::cout<<"hiForestPickSkim() - END"   <<std::endl;
}

int main(int argc, char** argv)
{
    if (argc == 5) {
        hiForestPickSkim(argv[1], argv[2], argv[3], argv[4]);
        return 0;
    }
    else if (argc == 4) {
        hiForestPickSkim(argv[1], argv[2], argv[3]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./hiForestPickSkim.exe <configFile> <eventList> <inputFile> <outputFile>"
                << std::endl;
        return 1;
    }
}
