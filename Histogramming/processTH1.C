/*
 * template macro to read TH1 from input file, process it, and write to output file
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

#include "../Utilities/fileUtil.h"
#include "../Utilities/systemUtil.h"
#include "../Utilities/th1Util.h"

void setTH1D(TH1D* h);
void processTH1(std::string inputFiles, std::string outputFile = "processTH1.root", std::string writeMode = "RECREATE", std::string hInPaths = "", std::string hOutPath = "", std::string operation = "add");

void processTH1(std::string inputFiles, std::string outputFile, std::string writeMode, std::string hInPaths, std::string hOutPath, std::string operation)
{
    std::cout << "running processTH1()" << std::endl;
    std::cout << "inputFiles = " << inputFiles.c_str()  << std::endl;
    std::cout << "outputFile = " << outputFile.c_str() << std::endl;
    std::cout << "writeMode  = " << writeMode.c_str() << std::endl;
    std::cout << "hInPaths  = " << hInPaths.c_str() << std::endl;
    std::cout << "hOutPath = " << hOutPath.c_str() << std::endl;
    std::cout << "operation = " << operation.c_str() << std::endl;

    operation = toUpperCase(operation);

    // TH1 objects
    TH1::SetDefaultSumw2();

    std::vector<std::string> inputFilePaths = split(inputFiles, ",", false, false);
    int nInputFiles = inputFilePaths.size();

    std::vector<std::string> hInputPaths = split(hInPaths, ",", false, false);
    int nInputHist = hInputPaths.size();

    std::vector<std::string> strParams;
    if (operation.find("SCALE") != std::string::npos) {
        strParams = split(operation, ":", false, false);
        operation = "SCALE";
    }
    else if (operation.find("PROJ") == 0) {
        strParams = split(operation, ":", false, false);
        operation = "PROJ";
        if (strParams.size() == 2) {
            strParams.push_back("0");
            strParams.push_back("-1");
        }
        else if (strParams.size() == 3) {
            strParams.push_back("-1");
        }
    }
    else if (operation.find("MEANPROJ") == 0) {
        strParams = split(operation, ":", false, false);
        operation = "MEANPROJ";
    }
    else if (operation.find("STDDEVPROJ") == 0) {
        strParams = split(operation, ":", false, false);
        operation = "STDDEVPROJ";
    }

    if (nInputFiles != 1 && nInputHist != 1 && nInputFiles != nInputHist) {
        std::cout << "Mismatch in number of input files and number of input histograms." << std::endl;
        std::cout << "nInputFiles = " << nInputFiles << std::endl;
        std::cout << "nInputHist = " << nInputHist << std::endl;
        std::cout << "Exiting." << std::endl;
        return;
    }
    if (nInputHist > 1) {
        if (operation == "SCALE" || operation == "UNITNORM" || operation.find("PROJ") != std::string::npos) {
            std::cout << "There should be only one input histogram if operation is " << operation.c_str() << "." << std::endl;
            std::cout << "nInputHist = " << nInputHist << std::endl;
            std::cout << "Exiting." << std::endl;
            return;
        }
    }

    std::cout << "nInputFiles = " << nInputFiles << std::endl;
    for (int i = 0; i < nInputFiles; ++i) {
        std::cout << Form("inputFiles[%d] = %s", i, inputFilePaths.at(i).c_str()) << std::endl;
    }

    std::cout << "nInputHist = " << nInputHist << std::endl;
    for (int i = 0; i < nInputHist; ++i) {
        std::cout << Form("hInPaths[%d] = %s", i, hInputPaths.at(i).c_str()) << std::endl;
    }

    std::vector<TFile*> inputs(nInputFiles, 0);
    for (int i = 0; i < nInputFiles; ++i) {
        inputs[i] = 0;
        inputs[i] = TFile::Open(inputFilePaths.at(i).c_str(), "READ");
    }

    std::vector<TH1*> hInVec(nInputHist, 0);
    for (int i = 0; i < nInputHist; ++i) {

        hInVec[i] = 0;
        int iFile = (nInputFiles == 1) ? 0 : i;
        hInVec[i] = (TH1*)inputs[iFile]->Get(hInputPaths[i].c_str());
        if (hInVec[i] == 0) continue;
    }

    TFile* output = TFile::Open(outputFile.c_str(), writeMode.c_str());
    output->cd();

    TH2D* h2Dtmp = 0;
    TH1D* hOut = 0;
    if (operation == "ADD") {
        hOut = (TH1D*)hInVec[0]->Clone(hOutPath.c_str());
        for (int i = 1; i < nInputHist; ++i) {
            hOut->Add(hInVec[i]);
        }
    }
    else if (operation == "SUB") {
        hOut = (TH1D*)hInVec[0]->Clone(hOutPath.c_str());
        for (int i = 1; i < nInputHist; ++i) {
            hOut->Add(hInVec[i], -1);
        }
    }
    else if (operation == "MUL") {
        hOut = (TH1D*)hInVec[0]->Clone(hOutPath.c_str());
        for (int i = 1; i < nInputHist; ++i) {
            hOut->Multiply(hInVec[i]);
        }
    }
    else if (operation == "DIV") {
        hOut = (TH1D*)hInVec[0]->Clone(hOutPath.c_str());
        for (int i = 1; i < nInputHist; ++i) {
            hOut->Divide(hInVec[i]);
        }
    }
    else if (operation == "SCALE") {
        hOut = (TH1D*)hInVec[0]->Clone(hOutPath.c_str());
        double paramTmp = std::atof(strParams[1].c_str());
        hOut->Scale(paramTmp);
    }
    else if (operation == "UNITNORM") {
        hOut = (TH1D*)hInVec[0]->Clone(hOutPath.c_str());
        hOut->Scale(1.0 / hOut->Integral(), "width");
    }
    else if (operation == "PROJ") {
        h2Dtmp = (TH2D*)hInVec[0]->Clone(Form("%s_tmp", hOutPath.c_str()));

        std::string axisStr = strParams[1];
        int bin1 = std::atoi(strParams[2].c_str());
        int bin2 = std::atoi(strParams[3].c_str());
        if (axisStr == "X") {
            hOut = (TH1D*)(h2Dtmp->ProjectionX(hOutPath.c_str(), bin1, bin2));
        }
        else if (axisStr == "Y") {
            hOut = (TH1D*)(h2Dtmp->ProjectionY(hOutPath.c_str(), bin1, bin2));
        }
    }
    else if (operation == "MEANPROJ" || operation == "STDDEVPROJ") {
        h2Dtmp = (TH2D*)hInVec[0]->Clone(Form("%s_tmp", hOutPath.c_str()));

        std::string titleY = "";
        std::string axisStr = strParams[1];
        if (axisStr == "X") {
            hOut = (TH1D*)(h2Dtmp->ProjectionX(hOutPath.c_str()));
            titleY = h2Dtmp->GetYaxis()->GetTitle();
        }
        else if (axisStr == "Y") {
            hOut = (TH1D*)(h2Dtmp->ProjectionY(hOutPath.c_str()));
            titleY = h2Dtmp->GetXaxis()->GetTitle();
        }

        if (operation == "MEANPROJ") {
            hOut->SetYTitle(Form("< %s >", titleY.c_str()));
            setBinsFromTH2sliceMean(hOut, h2Dtmp, (axisStr == "X"));
        }
        else if (operation == "STDDEVPROJ") {
            hOut->SetYTitle(Form("#sigma( %s )", titleY.c_str()));
            setBinsFromTH2sliceStdDev(hOut, h2Dtmp, (axisStr == "X"));
        }
    }
    else {
        hOut = (TH1D*)hInVec[0]->Clone(hOutPath.c_str());
    }

    setTH1D(hOut);
    hOut->Write("",TObject::kOverwrite);

    std::cout << "Closing the output file." << std::endl;
    output->Close();
    std::cout << "running processTH1() - END" << std::endl;
}

int main(int argc, char** argv)
{
    if (argc == 7) {
        processTH1(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]);
        return 0;
    }
    else if (argc == 6) {
        processTH1(argv[1], argv[2], argv[3], argv[4], argv[5]);
        return 0;
    }
    else if (argc == 5) {
        processTH1(argv[1], argv[2], argv[3], argv[4]);
        return 0;
    }
    else if (argc == 4) {
        processTH1(argv[1], argv[2], argv[3]);
        return 0;
    }
    else if (argc == 3) {
        processTH1(argv[1], argv[2]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./Histogramming/processTH1.exe <inputFiles> <outputFile> <writeMode> <hInPaths> <hOutPath> <operation>"
                << std::endl;
        return 1;
    }
}

void setTH1D(TH1D* h)
{
    h->SetMarkerStyle(kFullCircle);
    h->SetMarkerColor(kBlack);

    h->SetTitleOffset(1.25, "X");
    h->SetTitleOffset(1.5, "Y");
}
