/*
 * macro to save HEP DATA tables (e.g. tables for photon+jet FF and JS at 5.02 TeV)
 */
#include <TFile.h>
#include <TDirectoryFile.h>
#include <TH1.h>

#include <string>
#include <iostream>
#include <vector>
#include <fstream>      // ifstream, ofstream
#include <ios>      // ifstream, ofstream

#include "../Utilities/th1Util.h"
#include "../Utilities/systemUtil.h"

void printHEPData(std::string inputFile, std::string outputFile, std::string hPath, std::string hSysPath, double xMin = 0, double xMax = -1, bool doIndepVar = true);

void printHEPData(std::string inputFile, std::string outputFile, std::string hPath, std::string hSysPath, double xMin, double xMax, bool doIndepVar)
{
   std::cout<<"running printHEPData()"<<std::endl;

   std::cout << "input file  : " << inputFile.c_str() << std::endl;
   std::cout << "output file : " << outputFile.c_str() << std::endl;
   std::cout << "hPath    : " << hPath.c_str() << std::endl;
   std::cout << "hSysPath : " << hSysPath.c_str() << std::endl;
   std::cout << "xMin : " << xMin << std::endl;
   std::cout << "xMax : " << xMax << std::endl;
   std::cout << "doIndepVar : " << doIndepVar << std::endl;

   // TH1 objects
   TH1::SetDefaultSumw2();

   TFile* input = 0;
   input = TFile::Open(inputFile.c_str(), "READ");
   input->cd();

   TH1D* h = 0;
   TH1D* hSys = 0;

   h = 0;
   h = (TH1D*)input->Get(hPath.c_str());
   if (h == 0) {
       std::cout << "histogram not found : " << hPath.c_str() << std::endl;
       std::cout << "Exiting" << std::endl;
       return;
   }
   hSys = 0;
   hSys = (TH1D*)input->Get(hSysPath.c_str());
   if (hSys == 0) {
       std::cout << "histogram not found : " << hSysPath.c_str() << std::endl;
       std::cout << "Exiting" << std::endl;
       return;
   }

   int binFirst = 1;
   int binLast = h->GetNbinsX();
   if (xMax > xMin) {
       binFirst = h->FindBin(xMin);
       binLast = h->FindBin(xMax)-1;
   }

   //std::ios_base::openmode openMode = std::ios_base::out | std::ios_base::trunc;    // default mode, overwrites existing file
   std::ios_base::openmode openMode = std::ios_base::out | std::ios_base::app;    // append to existing file
   std::ofstream outFileStream(outputFile.c_str(), openMode);

   if (!outFileStream.is_open()) {
       std::cout << "output file not open : " << outputFile.c_str() << std::endl;
       std::cout << "Exiting" << std::endl;
   }

   if (doIndepVar) {
       //independent_variables:
       //- header: {name: X-axis}
       outFileStream << "  values:" << std::endl;
       for (int iBin = binFirst; iBin <= binLast; ++iBin) {
           double xLow = h->GetBinLowEdge(iBin);
           xLow = roundToSignificantFigures(xLow, 2, true);
           double xUp = h->GetBinLowEdge(iBin+1);
           xUp = roundToSignificantFigures(xUp, 2, true);
           outFileStream << "  - {high: " << Form("%.3f", xUp) << ", low: " << Form("%.3f", xLow) << "}" << std::endl;
       }
   }

   //dependent_variables:
   //- header: {name: Y-axis}
   outFileStream << "  values:" << std::endl;
   for (int iBin = binFirst; iBin <= binLast; ++iBin) {
       int nSF = 4;
       double y = roundToSignificantFigures(h->GetBinContent(iBin), nSF, false);
       double statErr = roundToSignificantFigures(h->GetBinError(iBin), nSF, false);
       double sysErr = roundToSignificantFigures(hSys->GetBinContent(iBin), nSF, false);
       double cutoff = 5*TMath::Power(10, -1*(nSF+1));
       while (statErr < cutoff || sysErr  < cutoff) {
           nSF++;
           y = roundToSignificantFigures(h->GetBinContent(iBin), nSF, false);
           statErr = roundToSignificantFigures(h->GetBinError(iBin), nSF, false);
           sysErr = roundToSignificantFigures(hSys->GetBinContent(iBin), nSF, false);
           cutoff = 5*TMath::Power(10, -1*(nSF+1));
       }
       //x = roundToPrecision(x, 2);
       outFileStream << "  - value: " << Form("%.*f", nSF, y) << std::endl;
       outFileStream << "    errors:" << std::endl;
       outFileStream << "    - {symerror: " << Form("%.*f", nSF, statErr) << ", label: stat}" << std::endl;
       outFileStream << "    - {symerror: " << Form("%.*f", nSF, sysErr) << ", label: 'sys'}" << std::endl;
   }
   outFileStream.close();

   std::cout<<"Closing the inputFile file."<<std::endl;
   input->Close();

   std::cout<<"running printHEPData() - END"<<std::endl;
}

int main(int argc, char** argv)
{
    if (argc == 8) {
        printHEPData(argv[1], argv[2], argv[3], argv[4], std::atof(argv[5]), std::atof(argv[6]), std::atoi(argv[7]));
        return 0;
    }
    else if (argc == 7) {
        printHEPData(argv[1], argv[2], argv[3], argv[4], std::atof(argv[5]), std::atof(argv[6]));
        return 0;
    }
    else if (argc == 6) {
        printHEPData(argv[1], argv[2], argv[3], argv[4], std::atof(argv[5]));
        return 0;
    }
    else if (argc == 5) {
        printHEPData(argv[1], argv[2], argv[3], argv[4]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./printHEPData.exe <inputFile> <outputFile> <hPath> <hSysPath> <xMin> <xMax> <doIndepVar>"
                << std::endl;
        return 1;
    }
    return 0;
}
