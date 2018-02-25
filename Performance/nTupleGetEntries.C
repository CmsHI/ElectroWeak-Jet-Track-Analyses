/*
 * macro to loop over a file with nTuples and print number of entries that pass the given selections in the given trees
 */

#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TMath.h>

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include "../Utilities/interface/InputConfigurationParser.h"
#include "../Utilities/interface/HiForestInfoController.h"
#include "../Utilities/fileUtil.h"
#include "../Utilities/systemUtil.h"

void nTupleGetEntries(std::string inputFile, std::string argFile);

void nTupleGetEntries(std::string inputFile, std::string argFile)
{
    std::cout<<"running nTupleGetEntries()"<<std::endl;
    std::cout<<"inputFile = "<< inputFile.c_str()  <<std::endl;
    std::cout<<"argFile   = "<< argFile.c_str()  <<std::endl;

    std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.c_str());
    std::cout<<"input ROOT files : num = "<<inputFiles.size()<< std::endl;
    std::cout<<"#####"<< std::endl;
    for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
        std::cout<<(*it).c_str()<< std::endl;
    }
    std::cout<<"##### END #####"<< std::endl;

    std::vector<std::string> argLines = getLines(argFile);
    int nArgLines = argLines.size();

    std::vector<std::string> selections;
    std::vector<std::string> treePaths;
    bool isSelection = false;
    for (int i = 0; i < nArgLines; ++i) {

        if (argLines[i] == "==selections==")  {
            isSelection = true;
            continue;
        }
        else if (argLines[i] == "==trees==")  {
            isSelection = false;
            continue;
        }

        if (isSelection)  selections.push_back(argLines[i]);
        else              treePaths.push_back(argLines[i]);
    }
    int nSelections = selections.size();
    std::vector<Long64_t> selectedEntries(nSelections, 0);
    std::cout<<"#####"<< std::endl;
    std::cout << "nSelections = " << nSelections << std::endl;
    for (int i = 0; i < nSelections; ++i) {
        std::cout << Form("selections[%d] = ", i) << selections[i].c_str() << std::endl;
    }
    std::cout<<"##### END #####"<< std::endl;

    // TTree objects;
    int nTrees = treePaths.size();
    std::vector<TTree*> trees(nTrees, 0);
    std::cout<<"#####"<< std::endl;
    std::cout << "nTrees = " << nTrees << std::endl;
    for (int i = 0; i < nTrees; ++i) {
        std::cout << Form("treePaths[%d] = ", i) << treePaths[i].c_str() << std::endl;
    }
    std::cout<<"##### END #####"<< std::endl;

    int nFiles = inputFiles.size();
    TFile* fileTmp = 0;

    std::cout << "initial reading to get the number of entries (if there is only one input file) and HiForest info" << std::endl;
    // read the first file only to get the HiForest info
    std::string inputPath = inputFiles.at(0).c_str();
    fileTmp = TFile::Open(inputPath.c_str(), "READ");
    fileTmp->cd();

    if (nFiles == 1) {
        // read one tree only to get the number of entries
        trees[0] = (TTree*)fileTmp->Get(treePaths[0].c_str());
        Long64_t entriesTmp = trees[0]->GetEntries();
        std::cout << "entries = " << entriesTmp << std::endl;
        trees[0]->Delete();
    }

    TTree* treeHiForestInfo = 0;
    treeHiForestInfo = (TTree*)fileTmp->Get("HiForest/HiForestInfo");
    HiForestInfoController hfic(treeHiForestInfo);
    std::cout<<"### HiForestInfo Tree ###"<< std::endl;
    hfic.printHiForestInfo();
    std::cout<<"###"<< std::endl;

    fileTmp->Close();
    // done with initial reading

    Long64_t entries = 0;

    std::cout<< "Loop STARTED" << std::endl;
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

        for (int i = 0; i < nTrees; ++i) {
            trees[i] = (TTree*)fileTmp->Get(treePaths[i].c_str());
        }
        for (int i = 1; i < nTrees; ++i) {
            trees[0]->AddFriend(trees[i]);
        }

        Long64_t entriesTmp = trees[0]->GetEntries();
        entries += entriesTmp;
        std::cout << "entries in File = " << entriesTmp << std::endl;

        for (int i = 0; i < nSelections; ++i) {
            selectedEntries[i] += trees[0]->GetEntries(selections[i].c_str());
        }
        fileTmp->Close();
    }
    std::cout<<  "Loop ENDED" <<std::endl;
    std::cout << "entries            = " << entries << std::endl;
    for (int i = 0; i < nSelections; ++i) {
        std::cout << Form("selectedEntries[%d] = ", i) << selectedEntries[i] << std::endl;
    }

    std::cout<<"running nTupleGetEntries() - END"<<std::endl;
}

int main(int argc, char** argv)
{
    if (argc == 3) {
        nTupleGetEntries(argv[1], argv[2]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./nTupleGetEntries.exe <inputFile> <argFile>"
                << std::endl;
        return 1;
    }
}
