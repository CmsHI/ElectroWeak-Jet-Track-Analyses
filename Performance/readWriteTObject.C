/*
 * macro to read object from input file and write it to output file
 */

#include <TFile.h>
#include <TObject.h>

#include <string>
#include <vector>
#include <iostream>

#include "../Utilities/systemUtil.h"

void readWriteTObject(std::string inputFile, std::string outputFile, std::string inputObj, std::string outputObj, std::string writeMode = "RECREATE");

void readWriteTObject(std::string inputFile, std::string outputFile, std::string inputObj, std::string outputObj, std::string writeMode)
{
    std::cout<<"running readWriteTObject()"<<std::endl;
    std::cout<<"inputFile  = "<< inputFile.c_str()  <<std::endl;
    std::cout<<"outputFile = "<< outputFile.c_str() <<std::endl;
    std::cout<<"inputObj  = "<< inputObj.c_str() <<std::endl;
    std::cout<<"outputObj = "<< outputObj.c_str() <<std::endl;
    std::cout<<"writeMode = "<< writeMode.c_str() <<std::endl;

    TFile* input = 0;
    input = TFile::Open(inputFile.c_str(), "READ");
    input->cd();

    TFile* output = TFile::Open(outputFile.c_str(), writeMode.c_str());
    output->cd();

    TObject* objIn = 0;
    TObject* objOut = 0;

    objIn = (TObject*)input->Get(inputObj.c_str());
    if (objIn == 0) {
        std::cout<<"The object does not exist in input file : "<<inputObj.c_str()<<std::endl;
        std::cout<<"Exiting."<<std::endl;
        return;
    }

    objOut = objIn->Clone(outputObj.c_str());
    objOut->Write(outputObj.c_str(), TObject::kOverwrite);

    std::cout<<"Closing the output file."<<std::endl;
    output->Close();
    std::cout<<"running readWriteTObject() - END"<<std::endl;
}

int main(int argc, char** argv)
{
    if (argc == 6) {
        readWriteTObject(argv[1], argv[2], argv[3], argv[4], argv[5]);
        return 0;
    }
    else if (argc == 5) {
        readWriteTObject(argv[1], argv[2], argv[3], argv[4]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./readWriteTObject.exe <inputFile> <outputFile> <inputObj> <outputObj> <writeMode>"
                << std::endl;
        return 1;
    }
}

