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
void processTH1(std::string inputFile, std::string outputFile = "processTH1.root");

void processTH1(std::string inputFile, std::string outputFile)
{
    std::cout<<"running processTH1()"<<std::endl;
    std::cout<<"inputFile   = "<< inputFile.c_str()  <<std::endl;
    std::cout<<"outputFile  = "<< outputFile.c_str() <<std::endl;

    // TH1 objects
    TH1::SetDefaultSumw2();

    TFile* input = 0;
    input = TFile::Open(inputFile.c_str(), "READ");
    input->cd();

    TFile* output = TFile::Open(outputFile.c_str(),"RECREATE");
    output->cd();

    TH1D* hIn = 0;
    //TH2D* h2DIn = 0;

    TH1D* hOut = 0;
    //TH2D* h2DOut = 0;

    std::vector<std::string> hInputPaths = {
            "hnPhoJet_ppdata_recoreco_100_200",
            "hnPhoJet_pbpbdata_recoreco_100_200",
            "hnPhoJet_pbpbdata_recoreco_0_20",
            "hnPhoJetjetmix_pbpbdata_recoreco_100_200",
            "hnPhoJetjetmix_pbpbdata_recoreco_0_20"
    };

    int nHInputPaths = hInputPaths.size();
    for (int i = 0; i < nHInputPaths; ++i) {
        std::string hInputPath = hInputPaths[i];
        hIn = 0;
        hIn = (TH1D*)input->Get(hInputPath.c_str());
        if (hIn == 0) continue;

        std::string hOutputPath = replaceAll(hInputPath, "_recoreco", "");
        hOut = (TH1D*)hIn->Clone(hOutputPath.c_str());
        setTH1D(hOut);
        hOut->Scale(1.0 / hOut->Integral());
        hOut->Write("",TObject::kOverwrite);
    }

    std::cout<<"Closing the output file."<<std::endl;
    output->Close();
    std::cout<<"running processTH1() - END"<<std::endl;
}

int main(int argc, char** argv)
{
    if (argc == 3) {
        processTH1(argv[1], argv[2]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./Histogramming/processTH1.exe <inputFile> <outputFile>"
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
