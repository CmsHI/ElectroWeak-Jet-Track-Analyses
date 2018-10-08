/*
 * template macro to read TH1 from input file, process them, and write to output file
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

void setTH1D(TH1D* h);
void processTH1(std::string inputFile, std::string outputFile = "processTH1.root", std::string writeMode = "RECREATE");

void processTH1(std::string inputFile, std::string outputFile, std::string writeMode)
{
    std::cout<<"running processTH1()"<<std::endl;
    std::cout<<"inputFile  = "<< inputFile.c_str()  <<std::endl;
    std::cout<<"outputFile = "<< outputFile.c_str() <<std::endl;
    std::cout<<"writeMode  = "<< writeMode.c_str() <<std::endl;

    // TH1 objects
    TH1::SetDefaultSumw2();

    TFile* input = 0;
    input = TFile::Open(inputFile.c_str(), "READ");
    input->cd();

    TFile* output = TFile::Open(outputFile.c_str(), writeMode.c_str());
    output->cd();

    TH1D* hIn = 0;
    //TH2D* h2DIn = 0;

    TH1D* hOut = 0;
    //TH2D* h2DOut = 0;

    std::vector<std::string> hInputPaths = {
            "h1D_xjz_pp",
            "h1D_xjz_pbpb_cent030"
    };

    int nHInputPaths = hInputPaths.size();
    for (int i = 0; i < nHInputPaths; ++i) {
        std::string hInputPath = hInputPaths[i];
        hIn = 0;
        hIn = (TH1D*)input->Get(hInputPath.c_str());
        if (hIn == 0) continue;

        std::string hOutputPath = Form("%s_unitNorm", hInputPath.c_str());
        hOut = (TH1D*)hIn->Clone(hOutputPath.c_str());
        setTH1D(hOut);
        hOut->Scale(1.0 / hOut->Integral(), "width");
        hOut->Write("",TObject::kOverwrite);
    }

    std::cout<<"Closing the output file."<<std::endl;
    output->Close();
    std::cout<<"running processTH1() - END"<<std::endl;
}

int main(int argc, char** argv)
{
    if (argc == 4) {
        processTH1(argv[1], argv[2], argv[3]);
        return 0;
    }
    else if (argc == 3) {
        processTH1(argv[1], argv[2]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./Histogramming/processTH1.exe <inputFile> <outputFile> <writeMode>"
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
