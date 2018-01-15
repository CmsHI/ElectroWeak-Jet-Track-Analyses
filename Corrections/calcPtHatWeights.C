/*
 * macro to calculate the ptHat weights using histograms of ptHat spectra.
 * filePtHat1 contains the continuous ptHat spectra.
 * filePtHat2 contains the ptHat spectra that needs to be assigned weights to become continuous.
 * A single ptHat weight is assigned for ptHatMin <= ptHat < ptHatMax
 */
#include <TFile.h>
#include <TDirectoryFile.h>
#include <TH1D.h>

#include <vector>
#include <string>
#include <iostream>

void calcPtHatWeights(std::string filePtHat1, std::string filePtHat2, std::string hist1 = "h_0", std::string hist2 = "h_0", int ptHatMin = 15, int ptHatMax = 30);

void calcPtHatWeights(std::string filePtHat1, std::string filePtHat2, std::string hist1, std::string hist2, int ptHatMin, int ptHatMax)
{
    std::cout << "running calcPtHatWeights()" << std::endl;
    std::cout << "filePtHat1 = " << filePtHat1.c_str() << std::endl;
    std::cout << "filePtHat2 = " << filePtHat2.c_str() << std::endl;
    std::cout << "hist1 = " << hist1.c_str() << std::endl;
    std::cout << "hist2 = " << hist2.c_str() << std::endl;
    std::cout << "ptHatMin = " << ptHatMin << std::endl;
    std::cout << "ptHatMax = " << ptHatMax << std::endl;

    TFile* file1  = TFile::Open(filePtHat1.c_str(), "READ");
    TFile* file2  = TFile::Open(filePtHat2.c_str(), "READ");

    TH1D* h1 = (TH1D*)file1->Get(hist1.c_str());
    TH1D* h2 = (TH1D*)file2->Get(hist2.c_str());

    int binMin = 0;
    int binMax = 0;

    binMin = (ptHatMin > 0) ? h1->FindBin(ptHatMin) : 0;
    binMax = (ptHatMax > 0) ? h1->FindBin(ptHatMax)-1 : h1->GetNbinsX()+1;
    double tot1 = h1->Integral(binMin, binMax);

    binMin = (ptHatMin > 0) ? h2->FindBin(ptHatMin) : 0;
    binMax = (ptHatMax > 0) ? h2->FindBin(ptHatMax)-1 : h2->GetNbinsX()+1;
    double tot2 = h2->Integral(binMin, binMax);

    double ptHatWeight = tot1 / tot2;

    std::cout << "tot1 = " << tot1 << std::endl;
    std::cout << "tot2 = " << tot2 << std::endl;
    std::cout << "ptHatWeight = " << ptHatWeight << std::endl;

    file1->Close();
    file2->Close();

    std::cout << "running calcPtHatWeights() - END" << std::endl;
}

int main(int argc, char** argv)
{
    if (argc == 7) {
        calcPtHatWeights(argv[1], argv[2], argv[3], argv[4], std::atoi(argv[5]), std::atoi(argv[6]));
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./calcPtHatWeights.exe <filePtHat1> <filePtHat2> <hist1> <hist2> <ptHatMin> <ptHatMax>"
                << std::endl;
        return 1;
    }
}
