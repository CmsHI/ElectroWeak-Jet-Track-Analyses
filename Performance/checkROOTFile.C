/*
 * macro to check if ROOT files are good or not.
 */

#include <TFile.h>
#include <TTree.h>

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include "../Utilities/interface/InputConfigurationParser.h"
#include "../Utilities/fileUtil.h"
#include "../Utilities/systemUtil.h"

void checkROOTFile(std::string inputFile);

void checkROOTFile(std::string inputFile)
{
    std::cout<<"running checkROOTFile()"<<std::endl;
    std::cout<<"inputFile = "<< inputFile.c_str()  <<std::endl;

    std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.c_str());
    std::cout<<"input ROOT files : num = "<<inputFiles.size()<< std::endl;
    std::cout<<"#####"<< std::endl;
    for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
        std::cout<<(*it).c_str()<< std::endl;
    }
    std::cout<<"##### END #####"<< std::endl;

    int nFiles = inputFiles.size();
    TFile* fileTmp = 0;

    std::vector<int> indicesNotGood;

    for (int iFile = 0; iFile < nFiles; ++iFile)  {

        std::string inputPath = inputFiles.at(iFile).c_str();
        std::cout <<"iFile = " << iFile << " , " ;
        std::cout <<"reading file : " << inputPath.c_str() << std::endl;
        fileTmp = TFile::Open(inputPath.c_str(), "READ");

        // check if the file is usable, if not skip the file.
        if (isGoodFile(fileTmp) != 0) {
            std::cout << "File is not good." << std::endl;
            indicesNotGood.push_back(iFile);
            continue;
        }

        fileTmp->Close();
    }

    int nNotGood = indicesNotGood.size();
    std::cout<<"nNotGood = " << nNotGood  <<std::endl;
    std::cout<<"Indices of files that are not good"<<std::endl;
    std::cout<<"#####"<< std::endl;
    for (int i = 0; i < nNotGood; ++i) {
        std::cout << indicesNotGood[i] << std::endl;
    }
    std::cout<<"##### END #####"<< std::endl;
    std::cout<<"Names of files that are not good"<<std::endl;
    std::cout<<"#####"<< std::endl;
    for (int i = 0; i < nNotGood; ++i) {
        std::cout << inputFiles[indicesNotGood[i]].c_str() << std::endl;
    }
    std::cout<<"##### END #####"<< std::endl;

    std::cout<<"running checkROOTFile() - END"<<std::endl;
}

int main(int argc, char** argv)
{
    if (argc == 2) {
        checkROOTFile(argv[1]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./checkROOTFile.exe <inputFile>"
                << std::endl;
        return 1;
    }
}
