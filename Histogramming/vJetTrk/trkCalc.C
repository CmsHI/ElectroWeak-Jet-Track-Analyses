/*
 * macro to do calculations for track spectra, to be used for residual track corrections
 */

#include <TFile.h>
#include <TDirectoryFile.h>
#include <TH1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TObject.h>
#include <TObjArray.h>
#include <TList.h>
#include <TKey.h>
#include <TMath.h>

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>

#include "../../Utilities/interface/ArgumentParser.h"
#include "../../Utilities/interface/ConfigurationParser.h"
#include "../../Utilities/interface/InputConfigurationParser.h"
#include "../../Utilities/eventUtil.h"
#include "../../Utilities/th1Util.h"
#include "../../Utilities/fileUtil.h"
#include "../../Utilities/systemUtil.h"
#include "../../Utilities/physicsUtil.h"
#include "../../Utilities/vJetTrkUtil.h"

///// global variables
std::vector<std::string> argOptions;
///// global variables - END

void setTH1(TH1* h);
void trkCalc(std::string inputFileList, std::string outputFile = "trkCalc.root");

void trkCalc(std::string inputFileList, std::string outputFile)
{
    std::cout << "running trkCalc()" << std::endl;
    std::cout << "inputFiles = " << inputFileList.c_str()  << std::endl;
    std::cout << "outputFile = " << outputFile.c_str() << std::endl;

    std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFileList.c_str());
    std::cout<<"ROOT files containing input TH1 : num = "<<inputFiles.size()<< std::endl;
    std::cout<<"#####"<< std::endl;
    for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
        std::cout<<(*it).c_str()<< std::endl;
    }
    std::cout<<"##### END #####"<< std::endl;

    // TH1 objects
    TH1::SetDefaultSumw2();

    int nInputFiles = inputFiles.size();

    std::cout << "nInputFiles = " << nInputFiles << std::endl;
    for (int i = 0; i < nInputFiles; ++i) {
        std::cout << Form("inputFiles[%d] = %s", i, inputFiles.at(i).c_str()) << std::endl;
    }

    std::vector<TFile*> inputs(nInputFiles, 0);
    for (int i = 0; i < nInputFiles; ++i) {
        inputs[i] = 0;
        inputs[i] = TFile::Open(inputFiles[i].c_str(), "READ");
    }

    std::vector<TList*> keyList(nInputFiles, 0);
    std::vector<std::string> inputObjs;

    TKey* key = 0;
    TIter* iter = 0;

    std::vector<TH1*> hIn;

    std::string writeMode = "RECREATE";
    TFile* output = TFile::Open(outputFile.c_str(), writeMode.c_str());
    output->cd();

    TH1* hTmp = 0;
    TH1* hTmp2 = 0;
    TH1* hOut = 0;

    TH2D* h2DTmp = 0;
    TH2D* h2DTmp2 = 0;
    TH2D* h2DTmp3 = 0;

    /*
    std::vector<double> trkPts = { 0.20, 0.250, 0.30, 0.350, 0.40, 0.450, 0.50, 0.550, 0.60, 0.650, 0.70, 0.750, 0.80, 0.850, 0.90, 0.950, 1.0, 1.05, 1.10, 1.15, 1.20, 1.30, 1.40, 1.50, 1.60, 1.70, 1.80, 1.90, 2.0, 2.50, 3.0, 4.0, 5.0, 7.50, 10.0, 12.0, 15.0, 20.0, 25.0, 30.0, 45.0, 60.0, 90.0, 120.0, 180.0, 300.0 };
    int nBinsPt = trkPts.size() - 1;
    int nBinsEta = 12;
    int nBinsCent = 20;

    for (int i = 0; i < nBinsPt; ++i) {
        for (int j = 0; j < nBinsEta; ++j) {
            for (int k = 0; k < nBinsCent; ++k) {
                std::string name_h_suffix = Form("iPt_%d_iEta_%d_iCent_%d", i, j, k);
                std::string name_h = Form("h_trkPhi_%s", name_h_suffix.c_str());
            }
        }
    }
    */

    keyList[0] = getListOfALLKeys(inputs[0]);

    for (int iFile = 0; iFile < nInputFiles; ++iFile) {

        keyList[iFile] = getListOfALLKeys(inputs[iFile]);

        std::cout << "Number of keys in iFile " << iFile << " = " << keyList[iFile]->GetSize() << std::endl;
    }

    key = 0;
    iter = new TIter(keyList[0]);

    hIn.clear();
    hIn.assign(nInputFiles, 0);

    // uniformization of trkPhi
    while ((key=(TKey*)iter->Next()))
    {
        std::string objName = key->ReadObj()->GetName();
        if (objName.find("h_trkPhi") == 0) {

            if ( !(key->ReadObj()->InheritsFrom("TH1D")) )  continue;

            hIn[0] = 0;
            hIn[0] = (TH1*)key->ReadObj();

            hTmp = (TH1*)hIn[0]->Clone(Form("%s_tmp", objName.c_str()));

            bool otherFileContains = true;
            for (int iFile = 1; iFile < nInputFiles; ++iFile) {

                if (keyList[iFile]->Contains(objName.c_str())) {
                    hIn[iFile] = (TH1*)inputs[iFile]->Get(objName.c_str());
                    hTmp->Add(hIn[iFile]);
                }
                else {
                    otherFileContains = false;
                    break;
                }
            }
            if (!otherFileContains) continue;

            setTH1(hTmp);
            hTmp->SetName(Form("%s_raw", objName.c_str()));
            hTmp->Write("",TObject::kOverwrite);

            double tmpInt = hTmp->Integral();
            hTmp2 = (TH1*)hTmp->Clone(Form("%s_uniform", objName.c_str()));
            if (tmpInt > 0) {
                hTmp2->Divide(hTmp2);
                int tmpN = 0;
                for (int iBin = 1; iBin <= hTmp2->GetNbinsX(); ++iBin) {
                    if (hTmp2->GetBinContent(iBin) > 0) {
                        tmpN++;
                    }
                }
                hTmp2->Scale((double(tmpInt) / tmpN));
            }
            hTmp2->Write("",TObject::kOverwrite);

            hOut = (TH1*)hTmp2->Clone(Form("%s_effcorr", objName.c_str()));
            hOut->Divide(hTmp);
            hOut->Write("",TObject::kOverwrite);
            std::cout << "wrote object : " << hOut->GetName() << std::endl;
        }
        else if (objName.find("h2_phi_vs_eta_gen_") == 0 ||
                 objName.find("h2_jetpt_vs_dR_gen_") == 0 ||
                 objName.find("h2_jetpt_vs_dR_rebin_gen_") == 0) {

            if ( !(key->ReadObj()->InheritsFrom("TH2D")) )  continue;

            hIn[0] = 0;
            hIn[0] = (TH2D*)key->ReadObj();

            //h2DTmp = (TH2D*)hIn[0]->Clone(Form("%s_tmp", objName.c_str()));
            h2DTmp = (TH2D*)hIn[0];

            std::string objNameTrkCorr = replaceAll(objName, "_gen_", "_trkCorr_");
            h2DTmp2 = (TH2D*)inputs[0]->Get(objNameTrkCorr.c_str());

            std::string objNameTrkEffCorr = replaceAll(objName, "_gen_", "_trkEffCorr_");
            h2DTmp3 = (TH2D*)inputs[0]->Get(objNameTrkEffCorr.c_str());

            bool otherFileContains = true;
            for (int iFile = 1; iFile < nInputFiles; ++iFile) {

                if (keyList[iFile]->Contains(objName.c_str())) {
                    hIn[iFile] = (TH2D*)inputs[iFile]->Get(objName.c_str());
                    h2DTmp->Add(hIn[iFile]);

                    hIn[iFile] = (TH2D*)inputs[iFile]->Get(objNameTrkCorr.c_str());
                    h2DTmp2->Add(hIn[iFile]);

                    hIn[iFile] = (TH2D*)inputs[iFile]->Get(objNameTrkEffCorr.c_str());
                    h2DTmp3->Add(hIn[iFile]);
                }
                else {
                    otherFileContains = false;
                    break;
                }
            }
            if (!otherFileContains) continue;

            setTH1(h2DTmp);
            h2DTmp->Write("",TObject::kOverwrite);

            setTH1(h2DTmp2);
            h2DTmp2->Write("",TObject::kOverwrite);

            setTH1(h2DTmp3);
            h2DTmp3->Write("",TObject::kOverwrite);

            std::string objNameRatio = replaceAll(objName, "_gen_", "_gen_trkCorr_ratio_");
            hOut = (TH2D*)h2DTmp->Clone(objNameRatio.c_str());
            hOut->Divide(h2DTmp2);
            hOut->Write("",TObject::kOverwrite);
            std::cout << "wrote object : " << hOut->GetName() << std::endl;

            objNameRatio = replaceAll(objName, "_gen_", "_gen_trkEffCorr_ratio_");
            hOut = (TH2D*)h2DTmp->Clone(objNameRatio.c_str());
            hOut->Divide(h2DTmp3);
            hOut->Write("",TObject::kOverwrite);
            std::cout << "wrote object : " << hOut->GetName() << std::endl;
        }
    }

    output->Write("",TObject::kOverwrite);
    std::cout << "Closing the output file." << std::endl;
    output->Close();
    std::cout << "running trkCalc() - END" << std::endl;
}

int main(int argc, char** argv)
{

    std::vector<std::string> argStr = ArgumentParser::ParseParameters(argc, argv);
    int nArgStr = argStr.size();

    argOptions = ArgumentParser::ParseOptions(argc, argv);

    if (nArgStr == 3) {
        trkCalc(argStr.at(1), argStr.at(2));
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./Histogramming/vJetTrk/trkCalc.exe <inputFiles> <outputFile>"
                << std::endl;
        return 1;
    }
}

void setTH1(TH1* h)
{
    h->SetMarkerStyle(kFullCircle);
    h->SetMarkerColor(kBlack);

    h->SetTitleOffset(1.25, "X");
    h->SetTitleOffset(1.5, "Y");
}

