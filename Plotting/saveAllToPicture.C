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

std::vector<std::string> argOptions;

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

    // parse the input of different options
    std::string graphicsFormat = (ArgumentParser::ParseOptionInputSingle(ARGUMENTPARSER::format, argOptions).size() > 0) ?
            ArgumentParser::ParseOptionInputSingle(ARGUMENTPARSER::format, argOptions) : "png";
    std::vector<std::string> wildCards = ArgumentParser::ParseOptionInput(ARGUMENTPARSER::wildCard, argOptions);

    bool overwriteStoredCanvas = (findPositionInVector(argOptions, "--overwriteStoredCanvas") != -1);

    // window width
    int ww = (ArgumentParser::ParseOptionInputSingle("--ww", argOptions).size() > 0) ?
            std::atoi(ArgumentParser::ParseOptionInputSingle("--ww", argOptions).c_str()) : 600;
    // window height
    int wh = (ArgumentParser::ParseOptionInputSingle("--wh", argOptions).size() > 0) ?
            std::atoi(ArgumentParser::ParseOptionInputSingle("--wh", argOptions).c_str()) : 600;

    // margins
    double lmargin = (ArgumentParser::ParseOptionInputSingle("--lmargin", argOptions).size() > 0) ?
            std::atof(ArgumentParser::ParseOptionInputSingle("--lmargin", argOptions).c_str()) : 0.1;
    double rmargin = (ArgumentParser::ParseOptionInputSingle("--rmargin", argOptions).size() > 0) ?
            std::atof(ArgumentParser::ParseOptionInputSingle("--rmargin", argOptions).c_str()) : 0.1;
    double bmargin = (ArgumentParser::ParseOptionInputSingle("--bmargin", argOptions).size() > 0) ?
            std::atof(ArgumentParser::ParseOptionInputSingle("--bmargin", argOptions).c_str()) : 0.1;
    double tmargin = (ArgumentParser::ParseOptionInputSingle("--tmargin", argOptions).size() > 0) ?
            std::atof(ArgumentParser::ParseOptionInputSingle("--tmargin", argOptions).c_str()) : 0.1;

    double ymin = (ArgumentParser::ParseOptionInputSingle("--ymin", argOptions).size() > 0) ?
            std::atof(ArgumentParser::ParseOptionInputSingle("--ymin", argOptions).c_str()) : -999999;
    double ymax = (ArgumentParser::ParseOptionInputSingle("--ymax", argOptions).size() > 0) ?
            std::atof(ArgumentParser::ParseOptionInputSingle("--ymax", argOptions).c_str()) : -999999;

    int logx = (ArgumentParser::ParseOptionInputSingle("--logx", argOptions).size() > 0) ?
            std::atoi(ArgumentParser::ParseOptionInputSingle("--logx", argOptions).c_str()) : 0;
    int logy = (ArgumentParser::ParseOptionInputSingle("--logy", argOptions).size() > 0) ?
            std::atoi(ArgumentParser::ParseOptionInputSingle("--logy", argOptions).c_str()) : 0;
    int logz = (ArgumentParser::ParseOptionInputSingle("--logz", argOptions).size() > 0) ?
            std::atoi(ArgumentParser::ParseOptionInputSingle("--logz", argOptions).c_str()) : 0;

    // set default values
    if (wildCards.size() == 0)  wildCards = {""};

    // print the input of different options
    std::cout << "graphicsFormat = " << graphicsFormat.c_str() << std::endl;
    int nWildCards = wildCards.size();
    if (nWildCards > 0 && wildCards.at(0).size() > 0) {
        std::cout << "nWildCards = " << nWildCards << std::endl;
        for (int i = 0; i<nWildCards; ++i){
            std::cout << Form("wc[%d] = %s", i, wildCards.at(i).c_str()) << std::endl;
        }
    }

    if (overwriteStoredCanvas)
        std::cout << "overwriting stored canvases" << std::endl;

    std::cout << "ww = " << ww << std::endl;
    std::cout << "wh = " << wh << std::endl;

    std::cout << "lmargin = " << lmargin << std::endl;
    std::cout << "rmargin = " << rmargin << std::endl;
    std::cout << "bmargin = " << bmargin << std::endl;
    std::cout << "tmargin = " << tmargin << std::endl;

    std::cout << "ymin = " << ymin << std::endl;
    std::cout << "ymax = " << ymax << std::endl;

    std::cout << "logx = " << logx << std::endl;
    std::cout << "logy = " << logy << std::endl;
    std::cout << "logz = " << logz << std::endl;

    // TH1D to contain reference properties for histograms
    TH1D* h1D = new TH1D("h1D", "", 1, 0, 1);
    h1D->SetMinimum(ymin);
    h1D->SetMaximum(ymax);

    // canvas to be used as template for pictures
    TCanvas* c = new TCanvas("c", "", ww, wh);
    c->SetLogx(logx);
    c->SetLogy(logy);
    c->SetLogz(logz);
    c->SetMargin(lmargin, rmargin, bmargin, tmargin);

    TCanvas* c2 = 0;
    if (overwriteStoredCanvas) c2 = (TCanvas*)c->Clone("c2");
    for (std::vector<std::string>::const_iterator it = wildCards.begin() ; it != wildCards.end(); ++it){

        std::string regexStr = "";
        if((*it).size() > 0) regexStr = wildCard2Regex((*it));

        saveHistogramsToPicture (file, regexStr, graphicsFormat, directoryToBeSavedIn, h1D, c);
        saveGraphsToPicture     (file, regexStr, graphicsFormat, directoryToBeSavedIn, c);
        saveCanvasesToPicture   (file, regexStr, graphicsFormat, directoryToBeSavedIn, h1D, c2);
    }
    c->Close();
    if (c2) c2->Close();

    std::cout<< "end : saving graphics" <<std::endl;

    std::cout<<"Closing the input file."<<std::endl;
    file->Close();
    std::cout<<"running saveAllToPicture() - END"<<std::endl;
}

int main(int argc, char** argv)
{
    std::vector<std::string> argStr = ArgumentParser::ParseParameters(argc, argv);
    int nArgStr = argStr.size();

    argOptions = ArgumentParser::ParseOptions(argc, argv);

    if (nArgStr == 3)    saveAllToPicture(argStr.at(1), argStr.at(2));
    else if (nArgStr == 2)    saveAllToPicture(argStr.at(1));
    else {
        std::cout << "Usage : \n" <<
                "./saveAllToPicture.exe <inputFile> [<outputDirectory>] [options]"
                << std::endl;
        std::cout << "Options are" << std::endl;
        std::cout << "--format=<graphicsFormat>" << std::endl;
        std::cout << "--wc=<wildcards>" << std::endl;
        std::cout << "--ww=<window width>" << std::endl;
        std::cout << "--wh=<window height>" << std::endl;
        std::cout << "--lmargin=<left margin>, Similarly rmargin, bmargin, tmargin for right, bottom, and top margins" << std::endl;
        std::cout << "--logx=<log scale of x-axis>, Similarly for logy and logz" << std::endl;
        std::cout << "--ymin=<minimum value for y-axis>, Similarly for ymax" << std::endl;
        std::cout << "--overwriteStoredCanvas is needed to modify a stored canvas" << std::endl;
        return 1;
    }
    return 0;
}
