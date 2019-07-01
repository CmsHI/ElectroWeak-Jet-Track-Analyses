/*
 * macro to read TTree from input file and write it to output file
 * useful to create subset of NTuples
 */

#include <TFile.h>
#include <TTree.h>
#include <TObject.h>

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>

#include "../Utilities/interface/InputConfigurationParser.h"
#include "../Utilities/interface/HiForestInfoController.h"
#include "../Utilities/fileUtil.h"
#include "../Utilities/systemUtil.h"

void readWriteTTree(std::string inputFile, std::string outputFile, std::string argFile);

void readWriteTTree(std::string inputFile, std::string outputFile, std::string argFile)
{
    std::cout<<"running readWriteTTree()"<<std::endl;
    std::cout<<"inputFile  = "<< inputFile.c_str()  <<std::endl;
    std::cout<<"outputFile = "<< outputFile.c_str() <<std::endl;
    std::cout<<"argFile = "<< argFile.c_str() <<std::endl;

    std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.c_str());
    std::cout<<"input ROOT files : num = "<<inputFiles.size()<< std::endl;
    std::cout<<"#####"<< std::endl;
    for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
        std::cout<<(*it).c_str()<< std::endl;
    }
    std::cout<<"##### END #####"<< std::endl;

    std::vector<std::string> argLines = getLines(argFile);
    int nArgLines = argLines.size();

    std::vector<std::string> treePaths;
    std::vector<std::string> branchLists;
    bool isTreePath = false;
    for (int i = 0; i < nArgLines; ++i) {

        if (argLines[i] == "==trees==")  {
            isTreePath = true;
            continue;
        }
        else if (argLines[i] == "==branches==")  {
            isTreePath = false;
            continue;
        }

        if (isTreePath)  treePaths.push_back(argLines[i]);
        else             branchLists.push_back(argLines[i]);
    }
    int nTrees = treePaths.size();
    std::cout<<"#####"<< std::endl;
    std::cout << "nTrees = " << nTrees << std::endl;
    for (int i = 0; i < nTrees; ++i) {
        std::cout << Form("treePaths[%d] = ", i) << treePaths[i].c_str() << std::endl;
    }
    std::cout<<"##### END #####"<< std::endl;

    int nBranchLists = branchLists.size();
    std::cout<<"#####"<< std::endl;
    std::cout << "nBranchLists = " << nBranchLists << std::endl;
    for (int i = 0; i < nBranchLists; ++i) {
        std::cout << Form("branchLists[%d] = ", i) << branchLists[i].c_str() << std::endl;
    }
    std::cout<<"##### END #####"<< std::endl;

    if (nBranchLists > 0 && nTrees > 0 && nBranchLists != nTrees) {
        std::cout << "Different number of trees and list of branches." << std::endl;
        std::cout << "nTrees = " << nTrees << " , nBranchLists = " << nBranchLists << std::endl;
        std::cout << "exiting." << std::endl;
        return;
    }

    // input trees
    std::vector<TTree*> trees(nTrees, 0);

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

    // extract list of directories
    std::vector<std::string> dirPaths;
    for (int i = 0; i < nTrees; ++i) {
        size_t posLastSlash = treePaths[i].find_last_of("/");
        if (posLastSlash != std::string::npos) {
            dirPaths.push_back(treePaths[i].substr(0, posLastSlash));
        }
        else {
            dirPaths.push_back(".");
        }
    }

    for (int i = 0; i < nTrees; ++i) {
        std::cout << Form("dirPaths[%d] = ", i) << dirPaths[i].c_str() << std::endl;
    }

    std::string writeMode = "RECREATE";
    TFile* output = TFile::Open(outputFile.c_str(), writeMode.c_str());
    output->cd();
    for (int i = 0; i < nTrees; ++i) {
        output->mkdir(dirPaths[i].c_str());
    }

    // output trees
    std::vector<TTree*> outTrees(nTrees, 0);

    Long64_t entries = 0;
    Long64_t entriesWritten = 0;

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

            trees[i]->SetBranchStatus("*", 0);
            std::vector<std::string> branchesTmp = split(branchLists[i], ",", false, false);
            int nBranchesTmp = branchesTmp.size();
            for (int j = 0; j < nBranchesTmp; ++j) {
                std::string branchTmp = trim(branchesTmp[j]);
                trees[i]->SetBranchStatus(branchTmp.c_str(), 1);
            }
        }

        output->cd();

        long MAXTREESIZE = 50000000000;
        for (int i = 0; i < nTrees; ++i) {
            if (iFile == 0)  {
                outTrees[i] = trees[i]->CloneTree(0);
                outTrees[i]->SetDirectory(output->GetDirectory(dirPaths[i].c_str()));
                outTrees[i]->SetMaxTreeSize(MAXTREESIZE);
            }
            else {
                trees[i]->CopyAddresses(outTrees[i]);
            }
        }

        Long64_t entriesTmp = trees[0]->GetEntries();
        entries += entriesTmp;
        std::cout << "entries in File = " << entriesTmp << std::endl;
        for (Long64_t j_entry = 0; j_entry < entriesTmp; ++j_entry)
        {
            if (j_entry % 2000 == 0)  {
              std::cout << "current entry = " <<j_entry<<" out of "<<entriesTmp<<" : "<<std::setprecision(2)<<(double)j_entry/entriesTmp*100<<" %"<<std::endl;
            }

            for (int i = 0; i < nTrees; ++i) {
                trees[i]->GetEntry(j_entry);
            }

            for (int i = 0; i < nTrees; ++i) {
                outTrees[i]->Fill();
            }

            entriesWritten++;
        }

        fileTmp->Close();
    }
    std::cout<<  "Loop ENDED" <<std::endl;
    std::cout << "entries            = " << entries << std::endl;
    std::cout << "entriesWritten     = " << entriesWritten << std::endl;
    for (int i = 0; i < nTrees; ++i) {
        std::cout << Form("outTrees[%d]->GetEntries() ", i) << outTrees[i]->GetEntries() << std::endl;
    }

    std::cout<<"Writing the output file."<<std::endl;
    output->Write("",TObject::kOverwrite);
    std::cout<<"Closing the output file."<<std::endl;
    output->Close();
    std::cout<<"running readWriteTTree() - END"<<std::endl;
}

int main(int argc, char** argv)
{
    if (argc == 4) {
        readWriteTTree(argv[1], argv[2], argv[3]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./readWriteTTree.exe <inputFile> <outputFile> <argFile>"
                << std::endl;
        return 1;
    }
}

