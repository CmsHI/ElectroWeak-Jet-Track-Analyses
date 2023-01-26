/*
 * code to draw a 1D function and save it to a ROOT file and picture
 */

#include <TFile.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TF1.h>
#include <TMath.h>

#include <string>
#include <vector>
#include <iostream>

#include "../Utilities/systemUtil.h"
#include "../Utilities/mathUtil.h"

const std::string strFNCHANDLE = "FNCHANDLE";

bool isFncHandle(std::string formula);
void drawTF1(std::string formula, std::string parValues, double xMin, double xMax, std::string outputFile = "drawTF1.root", std::string pictureFileName = "drawTF1.png");

void drawTF1(std::string formula, std::string parValues, double xMin, double xMax, std::string outputFile, std::string pictureFileName)
{
    std::cout << "running drawTF1()" << std::endl;
    std::cout << "formula = " << formula.c_str() << std::endl;
    std::cout << "parValues = " << parValues.c_str() << std::endl;
    std::cout << "xMin = " << xMin << std::endl;
    std::cout << "xMax = " << xMax << std::endl;
    std::cout << "outputFile = " << outputFile.c_str() << std::endl;
    std::cout << "pictureFileName = " << pictureFileName.c_str() << std::endl;

    TFile* output = TFile::Open(outputFile.c_str(),"RECREATE");
    output->cd();

    TF1* f1 = 0;
    if (!isFncHandle(formula)) {
        f1 = new TF1("f1", formula.c_str());
    }
    else {
        std::string fncName = split(formula, ":", false)[1];
        f1 = new TF1("f1", getFncPointer(fncName), 0, 1, getFncNpar(fncName));
    }
    int nPars = f1->GetNpar();

    std::vector<std::string> parValuesVec = split(parValues, ";", false);
    int nParValuesVec = parValuesVec.size();

    if (nPars != nParValuesVec) {
        std::cout << "Function has " << nPars << " parameters" << std::endl;
        std::cout << "Values are provided for " << nParValuesVec << " parameters" << std::endl;
        std::cout << "Exiting" << std::endl;
        return;
    }

    for (int i = 0; i < nPars; ++i) {
        double parTmp = std::atof(parValuesVec[i].c_str());
        std::cout << Form("Par[%d] = %f", i, parTmp) << std::endl;
        f1->SetParameter(i, parTmp);
    }
    std::cout << "ExpFormula = " << f1->GetExpFormula().Data() << std::endl;

    f1->SetRange(xMin, xMax);
    f1->SetLineColor(kRed);

    f1->Write("",TObject::kOverwrite);

    TCanvas* c = new TCanvas("c1", "", 800, 800);
    c->SetMargin(0.15, 0.05, 0.14, 0.06);
    c->SetTickx(1);
    c->SetTicky(1);
    c->SetGrid();
    c->cd();
    //TH1F* hFrame = c->DrawFrame(0.003, 0.0000001, xMax, 5000);
    //c->DrawFrame(xMin, 0.0, xMax, 0.01);

    f1->Draw();
    c->Update();

    c->Write("",TObject::kOverwrite);

    c->SaveAs(Form("%s.png", pictureFileName.c_str()));
    c->SaveAs(Form("%s.pdf", pictureFileName.c_str()));

    std::cout<<"Closing the output file."<<std::endl;
    output->Close();
    std::cout<<"running drawTF1() - END"<<std::endl;
}

int main(int argc, char** argv)
{
    if (argc == 7) {
        drawTF1(argv[1], argv[2], std::atof(argv[3]), std::atof(argv[4]), argv[5], argv[6]);
        return 0;
    }
    else if (argc == 6) {
        drawTF1(argv[1], argv[2], std::atof(argv[3]), std::atof(argv[4]), argv[5]);
        return 0;
    }
    else if (argc == 5) {
        drawTF1(argv[1], argv[2], std::atof(argv[3]), std::atof(argv[4]));
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./drawTF1.exe <formula> <parValues> <xMin> <xMax> <outputFile> <pictureFileName>"
                << std::endl;
        return 1;
    }
}

bool isFncHandle(std::string formula)
{
    return startsWith(formula, strFNCHANDLE.c_str());
}


