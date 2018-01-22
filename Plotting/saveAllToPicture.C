/*
 * macro to save all objects to picture files.
 */
#include <TKey.h>
#include <TDirectoryFile.h>
#include <TFile.h>
#include <TH1.h>
#include <TGraphAsymmErrors.h>
#include <TSystem.h>
#include <TString.h>
#include <TCanvas.h>
#include <TTree.h>

#include <string>
#include <iostream>

#include "../Utilities/interface/ArgumentParser.h"
#include "../Utilities/fileUtil.h"
#include "../Utilities/systemUtil.h"

std::string graphicsFormat;
std::vector<std::string> wildCards;

void saveAllToPicture(std::string fileName, std::string directory = "");

void saveAllToPicture(std::string fileName, std::string directory)
{
    std::cout<<"running saveAllToPicture()"<<std::endl;
    TFile* file = new TFile(fileName.c_str(),"READ");
    std::cout<< "begin : saving graphics" <<std::endl;

    std::string directoryToBeSavedIn;
    if(directory == "")   { // no input directory was given.

        TString dirName(file->GetName());
        dirName.ReplaceAll(".root","");
        gSystem->mkdir(dirName,true);
        directoryToBeSavedIn=dirName.Data();
    }
    else {
        directoryToBeSavedIn = directory.c_str();
    }

    // print the input of different options
    std::cout << "graphicsFormat = " << graphicsFormat.c_str() << std::endl;
    int nWildCards = wildCards.size();
    if (nWildCards > 0 && wildCards.at(0).size() > 0) {
        std::cout << "nWildCards = " << nWildCards << std::endl;
        for (int i = 0; i<nWildCards; ++i){
            std::cout << Form("wc[%d] = %s", i, wildCards.at(i).c_str()) << std::endl;
        }
    }

    // canvas to be used as template for pictures
    TCanvas* c = new TCanvas("c", "", 600, 600);
    for (std::vector<std::string>::const_iterator it = wildCards.begin() ; it != wildCards.end(); ++it){

        std::string regexStr = "";
        if((*it).size() > 0) regexStr = wildCard2Regex((*it));

        saveHistogramsToPicture (file, regexStr, graphicsFormat, directoryToBeSavedIn, c);
        saveGraphsToPicture     (file, regexStr, graphicsFormat, directoryToBeSavedIn, c);
        saveCanvasesToPicture   (file, regexStr, graphicsFormat, directoryToBeSavedIn);
    }
    c->Close();

    std::cout<< "end : saving graphics" <<std::endl;

    std::cout<<"Closing the input file."<<std::endl;
    file->Close();
    std::cout<<"running saveAllToPicture() - END"<<std::endl;
}

int main(int argc, char** argv)
{
    std::vector<std::string> argStr = ArgumentParser::ParseParameters(argc, argv);
    int nArgStr = argStr.size();

    std::vector<std::string> argOptions = ArgumentParser::ParseOptions(argc, argv);
    // parse the input of different options
    graphicsFormat = (ArgumentParser::ParseOptionInput(ARGUMENTPARSER::format, argOptions).size() > 0 ?
                      ArgumentParser::ParseOptionInput(ARGUMENTPARSER::format, argOptions).at(0) : "");
    wildCards = ArgumentParser::ParseOptionInput(ARGUMENTPARSER::wildCard, argOptions);

    // set default values
    if (graphicsFormat.size() == 0)  graphicsFormat = "png";
    if (wildCards.size() == 0)  wildCards = {""};

    if (nArgStr == 3)    saveAllToPicture(argStr.at(1), argStr.at(2));
    else if (nArgStr == 2)    saveAllToPicture(argStr.at(1));
    else {
        std::cout << "Usage : \n" <<
                "./saveAllToPicture.exe (--format=<graphicsFormat>) (--wc=<wildcards>) <inputFile> <outputDirectory>"
                << std::endl;
        return 1;
    }
    return 0;
}
