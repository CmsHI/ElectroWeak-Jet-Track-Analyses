/*
 * macro to do calculations for v+jet+track analysis
 */

#include <TFile.h>
#include <TH1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TObjArray.h>
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

void setTH1D(TH1D* h);
std::string parsePathTH1vPt(std::string histPath);
double parseVPtMin(std::string histPath);
double parseVPtMax(std::string histPath);
void vJetTrkCalc(std::string inputFileList, std::string inputObjList, std::string outputFile = "vJetTrkCalc.root", std::string writeMode = "RECREATE", std::string operations = "add");

void vJetTrkCalc(std::string inputFileList, std::string inputObjList, std::string outputFile, std::string writeMode, std::string operations)
{
    std::cout << "running vJetTrkCalc()" << std::endl;
    std::cout << "inputFiles = " << inputFileList.c_str()  << std::endl;
    std::cout << "inputObjList = " << inputObjList.c_str()  << std::endl;
    std::cout << "outputFile = " << outputFile.c_str() << std::endl;
    std::cout << "writeMode  = " << writeMode.c_str() << std::endl;
    std::cout << "operations = " << operations.c_str() << std::endl;

    operations = toUpperCase(operations);
    std::vector<std::string> operationList = split(operations, ",", false, false);
    bool doBKGSUB = containsElement(operationList, "BKGSUB");
    bool doNORMV = containsElement(operationList, "NORMV");

    std::cout << "doBKGSUB = " << doBKGSUB << std::endl;
    std::cout << "doNORMV = " << doNORMV << std::endl;

    std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFileList.c_str());
    std::cout<<"ROOT files containing input TH1 : num = "<<inputFiles.size()<< std::endl;
    std::cout<<"#####"<< std::endl;
    for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
        std::cout<<(*it).c_str()<< std::endl;
    }
    std::cout<<"##### END #####"<< std::endl;

    std::vector<std::string> inputObjs = InputConfigurationParser::ParseFiles(inputObjList.c_str());
    std::cout<<"list of distinguishing names or strings for input objects : num = "<<inputObjs.size()<< std::endl;
    std::cout<<"#####"<< std::endl;
    for (std::vector<std::string>::iterator it = inputObjs.begin() ; it != inputObjs.end(); ++it) {
        std::cout<<(*it).c_str()<< std::endl;
    }
    std::cout<<"##### END #####"<< std::endl;

    // TH1 objects
    TH1::SetDefaultSumw2();

    int nInputFiles = inputFiles.size();

    int nInputObjs = inputObjs.size();

    std::cout << "nInputFiles = " << nInputFiles << std::endl;
    for (int i = 0; i < nInputFiles; ++i) {
        std::cout << Form("inputFiles[%d] = %s", i, inputFiles.at(i).c_str()) << std::endl;
    }

    std::cout << "nInputHist = " << nInputObjs << std::endl;
    for (int i = 0; i < nInputObjs; ++i) {
        std::cout << Form("inputObjs[%d] = %s", i, inputObjs.at(i).c_str()) << std::endl;
    }

    std::vector<TFile*> inputs(nInputFiles, 0);
    for (int i = 0; i < nInputFiles; ++i) {
        inputs[i] = 0;
        inputs[i] = TFile::Open(inputFiles[i].c_str(), "READ");
    }

    std::vector<TH1*> hIn(nInputObjs, 0);

    TFile* output = TFile::Open(outputFile.c_str(), writeMode.c_str());
    output->cd();

    TH1D* hTmp = 0;
    TH1D* hTmpBkg = 0;
    TH1D* hOut = 0;

    if (doBKGSUB) {
        if (nInputFiles != 2) {
            std::cout << "There must be 2 input files for BKGSUB operation : one RAW, one BKG object" << std::endl;
            std::cout << "Exiting." << std::endl;
            return;
        }
        if (nInputObjs % 2 != 0) {
            std::cout << "Input objects must come in pairs for BKGSUB operation : one RAW, one BKG object" << std::endl;
            std::cout << "Exiting." << std::endl;
            return;
        }

        for (int i = 0; i < nInputObjs; ++i) {
            // i % 2 == 0 --> raw
            // i % 2 == 1 --> bkg

            hIn[i] = 0;
            int iFile = (i % 2 == 0) ? 0 : 1;
            hIn[i] = (TH1D*)inputs[iFile]->Get(inputObjs[i].c_str());
        }

        for (int i = 0; i < nInputObjs; i+=2) {

            int iRaw = i;
            int iBkg = i+1;

            if (hIn[iRaw] == 0) {
                std::cout << "Object not found : " << inputObjs[i].c_str() << std::endl;
                std::cout << "relevant file : " << inputFiles[0].c_str() << std::endl;
                std::cout << "skipping calculation involving this object" << std::endl;
                continue;
            }
            if (hIn[iBkg] == 0) {
                std::cout << "Object not found : " << inputObjs[i].c_str() << std::endl;
                std::cout << "relevant file : " << inputFiles[1].c_str() << std::endl;
                std::cout << "skipping calculation involving this object" << std::endl;
                continue;
            }

            double nV = 1;
            if (doNORMV) {

                std::string tmpName = hIn[iRaw]->GetName();
                std::string histPath_vPt = parsePathTH1vPt(tmpName);
                hTmp = (TH1D*)inputs[0]->Get(histPath_vPt.c_str());

                int binMin = hTmp->FindBin(parseVPtMin(tmpName));
                int binMax = hTmp->FindBin(parseVPtMax(tmpName)) - 1;
                if (binMax < binMin) {
                    binMax = hTmp->GetNbinsX() + 1;
                }
                nV = hTmp->Integral(binMin, binMax);
            }

            setTH1D((TH1D*)hIn[iRaw]);
            setTH1D((TH1D*)hIn[iBkg]);

            hTmpBkg = (TH1D*)hIn[iBkg]->Clone(Form("%s_tmpBkg", hIn[iBkg]->GetName()));

            hOut = (TH1D*)hIn[iRaw]->Clone(Form("%s_sig", hIn[iRaw]->GetName()));
            hOut->Add(hTmpBkg, -1);

            // write objects
            hTmp = (TH1D*)hIn[iRaw]->Clone(Form("%s_raw", hIn[iRaw]->GetName()));
            hTmp->Scale(1.0 / nV);
            hTmp->Write("", TObject::kOverwrite);

            hTmp = (TH1D*)hTmpBkg->Clone(Form("%s_bkg", hIn[iBkg]->GetName()));
            hTmp->Scale(1.0 / nV);
            hTmp->Write("", TObject::kOverwrite);

            hOut->Scale(1.0 / nV);
            hOut->Write("",TObject::kOverwrite);
        }
    }
    else if (doNORMV) {
        if (nInputFiles != 1) {
            std::cout << "There must be 1 input file if operation is only NORMV" << std::endl;
            std::cout << "Exiting." << std::endl;
            return;
        }

        for (int i = 0; i < nInputObjs; ++i) {

            hIn[i] = 0;
            hIn[i] = (TH1D*)inputs[0]->Get(inputObjs[i].c_str());
        }

        for (int i = 0; i < nInputObjs; ++i) {

            if (hIn[i] == 0) {
                std::cout << "Object not found : " << inputObjs[i].c_str() << std::endl;
                std::cout << "relevant file : " << inputFiles[0].c_str() << std::endl;
                std::cout << "skipping calculation involving this object" << std::endl;
                continue;
            }

            std::string tmpName = hIn[i]->GetName();
            hOut = (TH1D*)hIn[i]->Clone(tmpName.c_str());

            std::string histPath_vPt = parsePathTH1vPt(tmpName);
            hTmp = (TH1D*)inputs[0]->Get(histPath_vPt.c_str());

            int binMin = hTmp->FindBin(parseVPtMin(tmpName));
            int binMax = hTmp->FindBin(parseVPtMax(tmpName)) - 1;
            if (binMax < binMin) {
                binMax = hTmp->GetNbinsX() + 1;
            }
            double nV = hTmp->Integral(binMin, binMax);

            hOut->Scale(1.0 / nV);
            hOut->Write("",TObject::kOverwrite);
        }
    }

    output->Write("",TObject::kOverwrite);
    std::cout << "Closing the output file." << std::endl;
    output->Close();
    std::cout << "running vJetTrkCalc() - END" << std::endl;
}

int main(int argc, char** argv)
{

    std::vector<std::string> argStr = ArgumentParser::ParseParameters(argc, argv);
    int nArgStr = argStr.size();

    argOptions = ArgumentParser::ParseOptions(argc, argv);

    if (nArgStr == 6) {
        vJetTrkCalc(argStr.at(1), argStr.at(2), argStr.at(3), argStr.at(4), argStr.at(5));
        return 0;
    }
    else if (nArgStr == 5) {
        vJetTrkCalc(argStr.at(1), argStr.at(2), argStr.at(3), argStr.at(4));
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./Histogramming/vJetTrk/vJetTrkCalc.exe <inputFiles> <outputFile> <writeMode> <operation>"
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

/*
 * parse the path for vPt TH1 corresponding to given TH1 path
 * Example : returns "h_vPt_cent0_30" if given "h_dphi_vPt20_40_trkPt1_2_cent0_30"
 */
std::string parsePathTH1vPt(std::string histPath)
{
    size_t pos = histPath.find("_cent");
    std::string strCent = histPath.substr(pos);

    std::string res = Form("h_vPt%s", strCent.c_str());

    return res;
}

/*
 * parse the min vPt used when filling the given TH1
 * Example : returns 20 if given "h_dphi_vPt20_40_trkPt1_2_cent0_30"
 */
double parseVPtMin(std::string histPath)
{
    // histPath = "h_dphi_vPt20_40_trkPt1_2_cent0_30"
    size_t pos = histPath.find("vPt");
    std::string strTmp = histPath.substr(pos + std::string("vPt").size());
    // strTmp = "20_40_trkPt1_2_cent0_30"

    pos = strTmp.find("_");
    strTmp = strTmp.substr(0,pos);
    // strTmp = "20"

    return std::atof(strTmp.c_str());
}

/*
 * parse the max vPt used when filling the given TH1
 * Example : returns 40 if given "h_dphi_vPt20_40_trkPt1_2_cent0_30"
 */
double parseVPtMax(std::string histPath)
{
    // histPath = "h_dphi_vPt20_40_trkPt1_2_cent0_30"
    size_t pos = histPath.find("vPt");
    std::string strTmp = histPath.substr(pos + std::string("vPt").size());
    // strTmp = "20_40_trkPt1_2_cent0_30"

    pos = strTmp.find("_");
    strTmp = strTmp.substr(pos+1);
    // strTmp = "40_trkPt1_2_cent0_30"

    pos = strTmp.find("_");
    strTmp = strTmp.substr(0,pos);
    // strTmp = "40"

    return std::atof(strTmp.c_str());
}
