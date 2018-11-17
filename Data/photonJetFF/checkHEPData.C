/*
 * macro to check if the values in HEPdata agree with the original data
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

#include "../../Utilities/interface/InputConfigurationParser.h"
#include "../../Utilities/interface/HiForestInfoController.h"
#include "../../Utilities/fileUtil.h"
#include "../../Utilities/systemUtil.h"

void checkHEPData(std::string inputFile1, std::string histPath1, std::string inputFile2, std::string histPath2, double xMin = 0, double xMax = -1);

void checkHEPData(std::string inputFile1, std::string histPath1, std::string inputFile2, std::string histPath2, double xMin, double xMax)
{
    std::cout<<"running checkHEPData()"<<std::endl;
    std::cout<<"inputFile1  = "<< inputFile1.c_str()  <<std::endl;
    std::cout<<"histPath1   = "<< histPath1.c_str()  <<std::endl;
    std::cout<<"inputFile2  = "<< inputFile2.c_str()  <<std::endl;
    std::cout<<"histPath2   = "<< histPath2.c_str()  <<std::endl;

    // TH1 objects
    TH1::SetDefaultSumw2();

    TFile* input1 = 0;
    input1 = TFile::Open(inputFile1.c_str(), "READ");
    input1->cd();
    TH1D* h1 = (TH1D*)input1->Get(histPath1.c_str());

    TFile* input2 = 0;
    input2 = TFile::Open(inputFile2.c_str(), "READ");
    input2->cd();
    TH1D* h2 = (TH1D*)input2->Get(histPath2.c_str());

    // create path for statistical error in HEPData file
    std::string histPath2StatErr = Form("%s_e1", histPath2.c_str());
    TH1D* h2StatErr = (TH1D*)input2->Get(histPath2StatErr.c_str());

    int binFirst = 1;
    int binLast = h1->GetNbinsX();
    if (xMax > xMin) {
        binFirst = h1->FindBin(xMin);
        binLast = h1->FindBin(xMax)-1;
    }

    for (int iBin = binFirst; iBin <= binLast; ++iBin) {
        double val1 = h1->GetBinContent(iBin);
        int iBin2 = h2->FindBin(h1->GetBinCenter(iBin));
        double val2 = h2->GetBinContent(iBin2);
        double normDiffVal = (val1 - val2) / val1;

        std::cout << "iBin = " << iBin << ", bin center = " << h1->GetBinCenter(iBin) << std::endl;
        std::cout << "val1 = " << val1 << ", val2 = " << val2 << std::endl;
        std::cout << "normDiffVal = " << normDiffVal << std::endl;

        double diffLimit = 0.001;
        if (TMath::Abs(normDiffVal) > diffLimit) {
            std::cout << "WARNING : Values do NOT agree by more than " << diffLimit;
            std::cout << " ( normDiffVal = " << normDiffVal << " )" << std::endl;
        }

        double errStat1 = h1->GetBinError(iBin);
        double errStat2 = h2StatErr->GetBinContent(iBin2);
        double normDiffErrStat = (errStat1 - errStat2) / errStat1;

        std::cout << "errStat1 = " << errStat1 << ", errStat2 = " << errStat2 << std::endl;
        std::cout << "normDiffErrStat = " << normDiffErrStat << std::endl;
        if (TMath::Abs(normDiffErrStat) > diffLimit) {
            std::cout << "WARNING : Statistical errors do NOT agree by more than " << diffLimit;
            std::cout << " ( normDiffErrStat = " << normDiffErrStat << " )" << std::endl;
        }
    }

    std::cout<<"Closing the input files."<<std::endl;
    input1->Close();
    input2->Close();
    std::cout<<"running checkHEPData() - END"<<std::endl;
}

int main(int argc, char** argv)
{
    if (argc == 7) {
        checkHEPData(argv[1], argv[2], argv[3], argv[4], std::atof(argv[5]), std::atof(argv[6]));
        return 0;
    }
    else if (argc == 5) {
        checkHEPData(argv[1], argv[2], argv[3], argv[4]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./checkHEPData.exe <inputFile1> <histPath1> <inputFile2> <histPath2> <xMin> <xMax>"
                << std::endl;
        return 1;
    }
}
