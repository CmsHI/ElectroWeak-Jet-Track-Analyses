/*
 * macro to print Run, Event, Lumi numbers for a given event selection.
 */

#include <TFile.h>
#include <TChain.h>
#include <TH1.h>
#include <TCut.h>
#include <TEventList.h>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>      // ifstream, ofstream

#include "../TreeHeaders/CutConfigurationTree.h"
#include "../Utilities/interface/CutConfigurationParser.h"
#include "../Utilities/interface/InputConfigurationParser.h"

void printRunLumiEvent(const TString configFile, const TString inputFile, const TString outputFile = "printRunLumiEvent.txt");

void printRunLumiEvent(const TString configFile, const TString inputFile, const TString outputFile)
{
    std::cout<<"running printRunLumiEvent()"<<std::endl;
    std::cout<<"configFile  = "<< configFile.Data() <<std::endl;
    std::cout<<"inputFile   = "<< inputFile.Data()  <<std::endl;
    std::cout<<"outputFile  = "<< outputFile.Data() <<std::endl;

    InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

    std::string formula;    // formula for run,lumi,event
    std::string selection;

    std::string treePath;
    std::vector<std::string> treeFriendsPath;

    if (configInput.isValid) {
        formula = configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treeFormula];
        selection = configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treeSelection];

        treePath  = configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treePath];
        treeFriendsPath = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treeFriendPath]);
    }
    else {
        formula = "";
        selection = "0";

        treePath = "";
    }
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

    if (configCuts.isValid) {

    }
    else {

    }

    // verbose about cut configuration
    std::cout<<"Cut Configuration :"<<std::endl;

    std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.Data());

    std::cout<<"input ROOT files : num = "<<inputFiles.size()<< std::endl;
    std::cout<<"#####"<< std::endl;
    for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
        std::cout<<(*it).c_str()<< std::endl;
    }
    std::cout<<"##### END #####"<< std::endl;

    TChain* tree = new TChain(treePath.c_str());
    TChain* treeFriends[nFriends];
    for (int i=0; i<nFriends; ++i) {
        treeFriends[i] = new TChain(treeFriendsPath.at(i).c_str());
        tree->AddFriend(treeFriends[i], Form("t%d", i));
    }

    for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
       tree->Add((*it).c_str());
       for (int i=0; i<nFriends; ++i) {
           treeFriends[i]->Add((*it).c_str());
       }
    }

    // select the list of events
    std::string elistName = "elist_printRunLumiEvent";
    tree->Draw(Form(">> %s", elistName.c_str()), selection.c_str());
    TEventList* elist = (TEventList*)gDirectory->Get(elistName.c_str());
    tree->SetEventList(elist);

    Long64_t entries = tree->GetEntries();
    std::cout << "entries = " << entries << std::endl;
    std::cout << "TTree::Draw() : " << treePath.c_str() <<std::endl;
    tree->Draw(formula.c_str(),"","goff");

    std::cout <<  "TTree::Draw() ENDED : " << treePath.c_str() <<std::endl;

    int nentries = (Int_t)tree->GetSelectedRows();
    std::vector<UInt_t> vecRun   (tree->GetV1(), tree->GetV1()+nentries);
    std::vector<UInt_t> vecLumis (tree->GetV2(), tree->GetV2()+nentries);
    std::vector<UInt_t> vecEvent (tree->GetV3(), tree->GetV3()+nentries);

    std::cout << "selected entries = " << nentries << std::endl;

    std::cout << "opening file : " << outputFile.Data() << std::endl;
    std::ofstream outFileStream(outputFile.Data());
    if (outFileStream.is_open()) {
        outFileStream << "# run lumis event" << std::endl;
        for (int i = 0; i<nentries; ++i) {
            outFileStream << vecRun.at(i) << " " << vecLumis.at(i) << " " << vecEvent.at(i) << std::endl;
        }
    }
    outFileStream.close();
    std::cout << "wrote file   : " << outputFile.Data() << std::endl;

    elist->Delete();
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
