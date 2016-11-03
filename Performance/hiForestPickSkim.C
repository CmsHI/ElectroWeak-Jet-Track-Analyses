/*
 * macro to pick events from HiForest and/or to skim a subset of the HiForest trees.
 *
 */

#include <TKey.h>
#include <TFile.h>
#include <TChain.h>
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

void hiForestPickSkim(const TString configFile, const TString eventList, const TString inputFile, const TString outputFile = "hiForestPickSkim.txt");

void hiForestPickSkim(const TString configFile, const TString eventList, const TString inputFile, const TString outputFile)
{
    std::cout<<"running hiForestPickSkim()"<<std::endl;
    std::cout<<"configFile  = "<< configFile.Data() <<std::endl;
    std::cout<<"inputFile   = "<< inputFile.Data()  <<std::endl;
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

    std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.Data());

    std::cout<<"input ROOT files : num = "<<inputFiles.size()<< std::endl;
    std::cout<<"#####"<< std::endl;
    for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
        std::cout<<(*it).c_str()<< std::endl;
    }
    std::cout<<"##### END #####"<< std::endl;

    TFile* tmpInputFile = new TFile(inputFiles.at(0).c_str(), "READ");
    TList* listTTree = (TList*)getListOfALLKeys(tmpInputFile, "TTree");

    TFile* output = new TFile(outputFile, "RECREATE");
    int nTrees = listTTree->GetSize();

    std::vector<std::string> directoryPaths;
    std::vector<std::string> directoryTitles;

    TChain* inputTrees[nTrees];
    TTree* outputTrees[nTrees];
    int indexHiForestInfo = -1;

    TKey* key = 0;
    for (int iKey = 0; iKey < nTrees; ++iKey)
    {
        key = (TKey*)listTTree->At(iKey);
        std::string keyName = key->GetName();
        outputTrees[iKey] = new TChain(keyName.c_str());

        // get the directory where the TTree is.
        std::string directoryPath = key->GetMotherDir()->GetPath();
        std::string directoryTitle = key->GetMotherDir()->GetTitle();
        std::string filePath = key->GetFile()->GetPath();

        directoryPath = replaceAll(directoryPath, filePath, "");
        directoryPaths.push_back(directoryPath);
        directoryTitles.push_back(directoryTitle);

        std::string treePathTmp = Form("%s/%s", directoryPath.c_str(), keyName.c_str());
        if (treePathTmp == "HiForest/HiForestInfo") indexHiForestInfo = iKey;

        // read input
        inputTrees[iKey] = new TChain(treePathTmp.c_str());
        for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
            inputTrees[iKey]->Add((*it).c_str());
        }

        // prepare output directories / trees
        output->cd();
        if (directoryPath != "") {
            if (!output->GetKey(directoryPath.c_str())) output->mkdir(directoryPath.c_str(), directoryTitle.c_str());

            output->cd(directoryPath.c_str());
        }

        // copy the trees for output
        outputTrees[iKey] = inputTrees[iKey]->CloneTree(0);
    }
    // tree to read event info
    std::string treePathEvent = treePath.c_str();
    TChain* inputTreeEvent = new TChain(treePathEvent.c_str());
    for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
        inputTreeEvent->Add((*it).c_str());
    }
    if (inputTreeEvent == 0) {
        std::cout << Form("Error : tree with event information not found on the path %s", treePathEvent.c_str()) << std::endl;
        std::cout << "exiting" << std::endl;
        return;
    }

    inputTreeEvent->SetBranchStatus("*",0);     // disable all branches
    inputTreeEvent->SetBranchStatus("run",1);    // enable event information
    inputTreeEvent->SetBranchStatus("event",1);
    inputTreeEvent->SetBranchStatus("lumis",1);

    UInt_t    run;
    UInt_t    lumis;
    ULong64_t event;
    inputTreeEvent->SetBranchAddress("run", &run);
    inputTreeEvent->SetBranchAddress("lumis", &lumis);
    inputTreeEvent->SetBranchAddress("event", &event);

    HiForestInfoController hfic(inputTrees[indexHiForestInfo]);
    std::cout<<"### HiForestInfo Tree ###"<< std::endl;
    hfic.printHiForestInfo();
    std::cout<<"###"<< std::endl;

    std::vector<std::string> vecRunLumiEvent = InputConfigurationParser::ParseEvents(eventList.Data());
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

    Long64_t entries = inputTreeEvent->GetEntries();
    Long64_t entriesSelected = 0;
    std::cout << "entries = " << entries << std::endl;
    std::cout<< "Loop : " << treePath.c_str() <<std::endl;

    for (Long64_t j_entry=0; j_entry<entries; ++j_entry)
    {
        if (j_entry % 20000 == 0)  {
            std::cout << "current entry = " <<j_entry<<" out of "<<entries<<" : "<<std::setprecision(2)<<(double)j_entry/entries*100<<" %"<<std::endl;
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
    std::cout<< "Loop ENDED : " << treePath.c_str() <<std::endl;
    std::cout << "entries            = " << entries << std::endl;
    std::cout << "entriesSelected    = " << entriesSelected << std::endl;
    for (int iTree = 0; iTree < nTrees; ++iTree) {

        std::string treePath = outputTrees[iTree]->GetName();
        std::string directoryPath = directoryPaths.at(iTree).c_str();
        if (directoryPath.size() > 0)
            treePath = Form("%s/%s", directoryPath.c_str(), outputTrees[iTree]->GetName());
        std::cout << "Entries = " << outputTrees[iTree]->GetEntries() << Form(" : %s", treePath.c_str()) << std::endl;

        outputTrees[iTree]->Write("", TObject::kOverwrite);
    }

    output->Write("", TObject::kOverwrite);
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
