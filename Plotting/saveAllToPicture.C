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

#include "../Utilities/fileUtil.h"

void saveAllToPicture(std::string fileName, std::string pictureType = "png", std::string directory = "");

void saveAllToPicture(std::string fileName, std::string pictureType, std::string directory)
{
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

   saveAllHistogramsToPicture (file, pictureType.c_str(), directoryToBeSavedIn);
   saveAllGraphsToPicture     (file, pictureType.c_str(), directoryToBeSavedIn);
   saveAllCanvasesToPicture   (file, pictureType.c_str(), directoryToBeSavedIn);

   std::cout<< "end : saving graphics" <<std::endl;

   file->Close();
   std::cout<< "input file has been closed" <<std::endl;
}

int main(int argc, char** argv)
{
    if (argc == 4)    saveAllToPicture(argv[1], argv[2], argv[3]);
    else if (argc == 3)    saveAllToPicture(argv[1], argv[2]);
    else if (argc == 2)    saveAllToPicture(argv[1]);
    else {
        std::cout << "Usage : \n" <<
                "./saveAllToPicture.exe <inputFile> <pictureType> <outputDirectory>"
                << std::endl;
        return 1;
    }
    return 0;
}
