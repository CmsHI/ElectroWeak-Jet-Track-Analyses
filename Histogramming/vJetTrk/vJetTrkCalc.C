/*
 * macro to do calculations for v+jet+track analysis
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
std::string parsePathTH1vPt(std::string histPath);
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
    bool doSCALEBINW = containsElement(operationList, "SCALEBINW");
    bool doBKGSUB = containsElement(operationList, "BKGSUB");
    bool doSBSUB = containsElement(operationList, "SBSUB");
    bool doNORMV = containsElement(operationList, "NORMV");
    bool doMERGE = containsElement(operationList, "MERGE");
    bool doRATIO = containsElement(operationList, "RATIO");

    std::cout << "doSCALEBINW = " << doSCALEBINW << std::endl;
    std::cout << "doBKGSUB = " << doBKGSUB << std::endl;
    std::cout << "doSBSUB = " << doSBSUB << std::endl;
    std::cout << "doNORMV = " << doNORMV << std::endl;
    std::cout << "doMERGE = " << doMERGE << std::endl;
    std::cout << "doRATIO = " << doRATIO << std::endl;

    if (doMERGE && (doSCALEBINW || doBKGSUB || doSBSUB || doNORMV || doRATIO)) {
        std::cout << "MERGE cannot be combined with other operations" << std::endl;
        std::cout << "Exiting" << std::endl;
        return;
    }
    else if (doRATIO && (doSCALEBINW || doBKGSUB || doSBSUB || doNORMV || doMERGE)) {
        std::cout << "RATIO cannot be combined with other operations" << std::endl;
        std::cout << "Exiting" << std::endl;
        return;
    }
    else if (doBKGSUB && doSBSUB) {
        std::cout << "BKGSUB and SBSUB cannot be combined" << std::endl;
        std::cout << "Exiting" << std::endl;
        return;
    }

    bool doSUB = (doBKGSUB || doSBSUB);

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

    std::vector<TList*> keyList(nInputFiles, 0);

    TKey* key = 0;
    TIter* iter = 0;

    std::vector<TH1*> hIn(nInputObjs, 0);

    TFile* output = TFile::Open(outputFile.c_str(), writeMode.c_str());
    output->cd();

    TH1* hTmp = 0;
    TH1* hTmpBkg = 0;
    TH1* hOut = 0;

    if (doSUB || doRATIO) {
        if (nInputFiles != 2) {
            std::cout << "There must be 2 input files for SUB (RATIO) operation : "
                    "       one RAW (NUMERATOR), one BKG (DENOMINATOR) file" << std::endl;
            std::cout << "Exiting." << std::endl;
            return;
        }
        if (nInputObjs % 2 != 0) {
            std::cout << "Input objects must come in pairs for SUB (RATIO) operation : "
                    "       one RAW (NUMERATOR), one BKG (DENOMINATOR) object" << std::endl;
            std::cout << "Exiting." << std::endl;
            return;
        }

        for (int i = 0; i < nInputObjs; ++i) {
            // i % 2 == 0 --> raw or numerator
            // i % 2 == 1 --> bkg or denominator

            hIn[i] = 0;
            int iFile = (i % 2 == 0) ? 0 : 1;
            hIn[i] = (TH1*)inputs[iFile]->Get(inputObjs[i].c_str());
        }
    }

    if (doSUB) {

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
                hTmp = (TH1*)inputs[0]->Get(histPath_vPt.c_str());

                int binMin = hTmp->GetXaxis()->FindBin(parseVPtMin(tmpName));
                int binMax = hTmp->GetXaxis()->FindBin(parseVPtMax(tmpName)) - 1;
                if (binMax < binMin) {
                    binMax = hTmp->GetNbinsX() + 1;
                }
                nV = hTmp->Integral(binMin, binMax);
            }

            setTH1(hIn[iRaw]);
            setTH1(hIn[iBkg]);

            if (doBKGSUB) {
                hTmpBkg = (TH1*)hIn[iBkg]->Clone(Form("%s_tmpBkg", hIn[iBkg]->GetName()));

                hOut = (TH1*)hIn[iRaw]->Clone(Form("%s_sig", hIn[iRaw]->GetName()));
                hOut->Add(hTmpBkg, -1);

                // write objects
                hTmp = (TH1*)hIn[iRaw]->Clone(Form("%s_raw", hIn[iRaw]->GetName()));
                hTmp->Scale(1.0 / nV);
                if (doSCALEBINW) {
                    hTmp->Scale(1.0, "width");
                }
                hTmp->Write("", TObject::kOverwrite);

                hTmp = (TH1*)hTmpBkg->Clone(Form("%s_bkg", hIn[iBkg]->GetName()));
                hTmpBkg->Delete();
                hTmp->Scale(1.0 / nV);
                if (doSCALEBINW) {
                    hTmp->Scale(1.0, "width");
                }
                hTmp->Write("", TObject::kOverwrite);

                hOut->Scale(1.0 / nV);
                if (doSCALEBINW) {
                    hOut->Scale(1.0, "width");
                }
                hOut->Write("",TObject::kOverwrite);
            }
            else if (doSBSUB) {
                // assume x-axis contains dphi, trkPt, xivh
                // assume y-axis contains deta

                if (!(hIn[iRaw]->InheritsFrom("TH2D"))) {
                    continue;
                }
                if (std::string(hIn[iRaw]->GetName()).find("h2_deta_vs_") == std::string::npos) {
                    std::cout << "deta is not on y-axis : " << inputObjs[i].c_str() << std::endl;
                    std::cout << "skipping calculation involving this object" << std::endl;
                    continue;
                }

                hTmp = (TH1*)hIn[iRaw]->Clone(Form("%s_raw", hIn[iRaw]->GetName()));
                hTmp->Write("", TObject::kOverwrite);

                hTmp = (TH1*)hIn[iBkg]->Clone(Form("%s_bkg", hIn[iBkg]->GetName()));
                hTmp->Write("", TObject::kOverwrite);

                hTmpBkg = (TH1*)hIn[iBkg]->Clone(Form("%s_tmpBkg", hIn[iBkg]->GetName()));

                // first bin along x-axis and y-axis contains the BKG normalization.
                double normBKG = hTmpBkg->GetBinContent(1, 1);

                // raw corrected for acceptance
                hTmp = (TH1*)hIn[iRaw]->Clone(Form("%s_raw_accCorr", hIn[iRaw]->GetName()));
                //hTmp->Divide(hTmpBkg);
                for (int iBinX = 1; iBinX <= hTmp->GetNbinsX(); ++iBinX) {
                    for (int iBinY = 1; iBinY <= hTmp->GetNbinsY(); ++iBinY) {
                        double binC = hTmp->GetBinContent(iBinX, iBinY) / hTmpBkg->GetBinContent(iBinX, iBinY);
                        double binErr = hTmp->GetBinError(iBinX, iBinY) / hTmpBkg->GetBinContent(iBinX, iBinY);
                        hTmp->SetBinContent(iBinX, iBinY, binC);
                        hTmp->SetBinError(iBinX, iBinY, binErr);
                    }
                }
                hTmpBkg->Delete();
                hTmp->Scale(normBKG);
                hTmp->Scale(1.0 / nV);
                hTmp->Write("", TObject::kOverwrite);

                // sr = short-range
                // lr = long-range
                double detaMinSR = 0;
                double detaMaxSR = 1.2;

                double detaMinLR = 2.0;
                double detaMaxLR = 3.2;

                int binDetaMinSR = hTmp->GetYaxis()->FindBin(detaMinSR);
                int binDetaMaxSR = hTmp->GetYaxis()->FindBin(detaMaxSR);

                int binDetaMinLR = hTmp->GetYaxis()->FindBin(detaMinLR);
                int binDetaMaxLR = hTmp->GetYaxis()->FindBin(detaMaxLR);

                double detaSR = hTmp->GetYaxis()->GetBinLowEdge(binDetaMaxSR+1) - hTmp->GetYaxis()->GetBinLowEdge(binDetaMinSR);
                double detaLR = hTmp->GetYaxis()->GetBinLowEdge(binDetaMaxLR+1) - hTmp->GetYaxis()->GetBinLowEdge(binDetaMinLR);
                double normLRtoSR = detaSR / detaLR;

                std::string name1D = replaceFirst(hIn[iRaw]->GetName(), "h2_deta_vs_", "h_");

                // lr
                hTmpBkg = (TH1D*)(((TH2D*)hTmp)->ProjectionX(Form("%s_bkg", name1D.c_str()), binDetaMinLR, binDetaMaxLR));
                hTmpBkg->Scale(normLRtoSR);

                // sr
                hOut = (TH1D*)(((TH2D*)hTmp)->ProjectionX(Form("%s_raw", name1D.c_str()), binDetaMinSR, binDetaMaxSR));

                if (doSCALEBINW) {
                    hTmpBkg->Scale(1.0, "width");
                    hOut->Scale(1.0, "width");
                }
                hTmpBkg->Write("", TObject::kOverwrite);
                hOut->Write("", TObject::kOverwrite);

                hOut->SetName(Form("%s_sig", name1D.c_str()));
                hOut->Add(hTmpBkg, -1);
                hOut->Write("", TObject::kOverwrite);

                // rebin
                if (name1D.find("h_dphi_") == 0) {
                    double binW = hOut->GetBinWidth(1);
                    std::vector<double> binsX = {0, binW*3, binW*6, binW*9, binW*12, binW*14, binW*16, binW*18, binW*19, binW*20};
                    int nBinsX = binsX.size()-1;

                    double arr_dphi[nBinsX+1];
                    std::copy(binsX.begin(), binsX.end(), arr_dphi);

                    name1D = replaceAll(name1D, "h_dphi", "h_dphi_rebin");
                    hTmp = (TH1D*)hOut->Rebin(nBinsX, Form("%s_sig", name1D.c_str()), arr_dphi);
                    hTmp->Scale(binW, "width");
                    hTmp->Write("",TObject::kOverwrite);
                }
                else if (name1D.find("h_trkPt_") == 0) {
                    double binW = 0.5;
                    double xMax_trkPt = 30;
                    std::vector<double> binsX = {0, 0.5, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 14, 18, 24, xMax_trkPt};

                    int nBinsX = binsX.size()-1;

                    double tmpArr[nBinsX+1];
                    std::copy(binsX.begin(), binsX.end(), tmpArr);

                    name1D = replaceAll(name1D, "h_trkPt", "h_trkPt_rebin");
                    hTmp = (TH1D*)hOut->Rebin(nBinsX, Form("%s_sig", name1D.c_str()), tmpArr);
                    hTmp->Scale(binW, "width");
                    hTmp->Write("",TObject::kOverwrite);
                }
            }
        }
    }
    else if (doRATIO) {
        for (int i = 0; i < nInputObjs; i+=2) {

            int iNum = i;
            int iDnm = i+1;

            if (hIn[iNum] == 0) {
                std::cout << "Object not found : " << inputObjs[i].c_str() << std::endl;
                std::cout << "relevant file : " << inputFiles[0].c_str() << std::endl;
                std::cout << "skipping calculation involving this object" << std::endl;
                continue;
            }
            if (hIn[iDnm] == 0) {
                std::cout << "Object not found : " << inputObjs[i].c_str() << std::endl;
                std::cout << "relevant file : " << inputFiles[1].c_str() << std::endl;
                std::cout << "skipping calculation involving this object" << std::endl;
                continue;
            }

            setTH1(hIn[iNum]);
            setTH1(hIn[iDnm]);

            hTmp = (TH1*)hIn[iDnm]->Clone(Form("%s_tmpDenom", hIn[iDnm]->GetName()));

            std::string tmpName;
            std::string strOld = (hIn[iNum]->InheritsFrom("TH2D")) ? "h2_" : "h_";
            tmpName = replaceFirst(hIn[iNum]->GetName(), strOld, strOld+"ratio_");

            hOut = (TH1*)hIn[iNum]->Clone(tmpName.c_str());
            hOut->Divide(hTmp);
            hOut->SetYTitle("PbPb / pp");

            hTmp->Delete();

            // write objects
            tmpName = replaceFirst(hIn[iNum]->GetName(), strOld, strOld+"num_");
            hTmp = (TH1*)hIn[iNum]->Clone(tmpName.c_str());
            hTmp->Write("", TObject::kOverwrite);

            tmpName = replaceFirst(hIn[iDnm]->GetName(), strOld, strOld+"denom_");
            hTmp = (TH1*)hIn[iDnm]->Clone(tmpName.c_str());
            hTmp->Write("", TObject::kOverwrite);

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
            hIn[i] = (TH1*)inputs[0]->Get(inputObjs[i].c_str());
        }

        for (int i = 0; i < nInputObjs; ++i) {

            if (hIn[i] == 0) {
                std::cout << "Object not found : " << inputObjs[i].c_str() << std::endl;
                std::cout << "relevant file : " << inputFiles[0].c_str() << std::endl;
                std::cout << "skipping calculation involving this object" << std::endl;
                continue;
            }

            std::string tmpName = hIn[i]->GetName();
            hOut = (TH1*)hIn[i]->Clone(tmpName.c_str());

            std::string histPath_vPt = parsePathTH1vPt(tmpName);
            hTmp = (TH1*)inputs[0]->Get(histPath_vPt.c_str());

            int binMin = hTmp->GetXaxis()->FindBin(parseVPtMin(tmpName));
            int binMax = hTmp->GetXaxis()->FindBin(parseVPtMax(tmpName)) - 1;
            if (binMax < binMin) {
                binMax = hTmp->GetNbinsX() + 1;
            }
            double nV = hTmp->Integral(binMin, binMax);

            hOut->Scale(1.0 / nV);
            if (doSCALEBINW) {
                hOut->Scale(1.0, "width");
            }
            hOut->Write("",TObject::kOverwrite);

            // if no bkg, then this is sig
            hOut->SetName(Form("%s_sig", hOut->GetName()));
            hOut->Write("",TObject::kOverwrite);
        }
    }
    else if (doMERGE) {
        if (nInputFiles != 2) {
            std::cout << "There must be 2 input files for MERGE operation : one for Zmm, one for Zee" << std::endl;
            std::cout << "Exiting." << std::endl;
            return;
        }

        // assume that first file is for Zmm, second for Zee

        keyList[0] = getListOfALLKeys(inputs[0]);
        keyList[1] = getListOfALLKeys(inputs[1]);

        std::cout << "Number of keys in file 1 = " << keyList[0]->GetSize() << std::endl;
        std::cout << "Number of keys in file 2 = " << keyList[1]->GetSize() << std::endl;

        key = 0;
        iter = new TIter(keyList[0]);

        std::vector<TH2D*> vec_h2D;

        double w1 = 1;
        double w2 = 1;

        hIn.clear();
        hIn.assign(2, 0);

        while ((key=(TKey*)iter->Next()))
        {
            if (!(key->ReadObj()->InheritsFrom("TH1D") || key->ReadObj()->InheritsFrom("TH2D")))  continue;

            hIn[0] = 0;
            hIn[0] = (TH1*)key->ReadObj();

            std::string objName = hIn[0]->GetName();

            if (!keyList[1]->Contains(objName.c_str()))  continue;

            hIn[1] = (TH1*)inputs[1]->Get(objName.c_str());

            hIn[0]->Scale(w1);
            hIn[1]->Scale(w2);

            hOut = 0;
            hOut = (TH1*)hIn[0]->Clone();
            hOut->Add(hIn[1]);

            if (hOut->InheritsFrom("TH2D")) {
                vec_h2D.push_back((TH2D*)hOut);
            }

            hOut->Write("",TObject::kOverwrite);
            std::cout << "wrote object : " << hOut->GetName() << std::endl;
        }

        int nVec_h2D = vec_h2D.size();
        for (int i = 0; i < nVec_h2D; ++i) {

            std::vector<TH1D*> hProj(3, 0);
            for (int iProj = 0; iProj < 2; ++iProj) {
                if (iProj == 0) {
                    hProj[0] = (TH1D*)((TH2D*)vec_h2D[i])->ProjectionX(Form("%s_projX", vec_h2D[i]->GetName()));
                }
                else {
                    hProj[0] = (TH1D*)((TH2D*)vec_h2D[i])->ProjectionY(Form("%s_projY", vec_h2D[i]->GetName()));
                }

                hProj[0]->Write("",TObject::kOverwrite);

                hProj[1] = (TH1D*)hProj[0]->Clone(Form("%s_mean", hProj[0]->GetName()));
                hProj[2] = (TH1D*)hProj[0]->Clone(Form("%s_stddev", hProj[0]->GetName()));

                std::string projYTitle = (iProj == 0) ? ((TH2D*)vec_h2D[i])->GetYaxis()->GetTitle() : ((TH2D*)vec_h2D[i])->GetXaxis()->GetTitle();
                hProj[1]->SetYTitle(Form("< %s >", projYTitle.c_str()));
                hProj[2]->SetYTitle(Form("#sigma( %s )", projYTitle.c_str()));

                setBinsFromTH2sliceMean(hProj[1], (TH2D*)vec_h2D[i], (iProj == 0));
                setBinsFromTH2sliceStdDev(hProj[2], (TH2D*)vec_h2D[i], (iProj == 0));

                hProj[1]->Write("",TObject::kOverwrite);
                hProj[2]->Write("",TObject::kOverwrite);
            }

            std::cout << "wrote 1D projections for : " << vec_h2D[i]->GetName() << std::endl;
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

void setTH1(TH1* h)
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
    histPath = replaceAll(histPath, "_projX", "");
    histPath = replaceAll(histPath, "_projY", "");

    size_t pos = histPath.find("_cent");
    std::string strCent = histPath.substr(pos);

    std::string res = Form("h_vPt%s", strCent.c_str());

    return res;
}
