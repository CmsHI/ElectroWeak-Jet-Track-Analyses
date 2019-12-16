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
#include <TRandom3.h>

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
TH1* calcTH1BootStrap(TH2D* h2D_bs);
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
    bool doSAMESIGNSUB = containsElement(operationList, "SAMESIGNSUB");
    bool doNORMV = containsElement(operationList, "NORMV");
    bool doMERGE = containsElement(operationList, "MERGE");
    bool doRATIO = containsElement(operationList, "RATIO");
    bool doDIFF = containsElement(operationList, "DIFF");
    bool doPULL = containsElement(operationList, "PULL");
    bool doBOOTSTRAP = containsElement(operationList, "BOOTSTRAP");

    std::cout << "doSCALEBINW = " << doSCALEBINW << std::endl;
    std::cout << "doBKGSUB = " << doBKGSUB << std::endl;
    std::cout << "doSBSUB = " << doSBSUB << std::endl;
    std::cout << "doSAMESIGNSUB = " << doSAMESIGNSUB << std::endl;
    std::cout << "doNORMV = " << doNORMV << std::endl;
    std::cout << "doMERGE = " << doMERGE << std::endl;
    std::cout << "doRATIO = " << doRATIO << std::endl;
    std::cout << "doDIFF = " << doDIFF << std::endl;
    std::cout << "doPULL = " << doPULL << std::endl;
    std::cout << "doBOOTSTRAP = " << doBOOTSTRAP << std::endl;

    if (doMERGE && (doSCALEBINW || doBKGSUB || doSBSUB || doSAMESIGNSUB || doNORMV || doRATIO || doDIFF || doPULL)) {
        std::cout << "MERGE cannot be combined with other operations" << std::endl;
        std::cout << "Exiting" << std::endl;
        return;
    }
    else if (doRATIO && (doSCALEBINW || doBKGSUB || doSBSUB || doSAMESIGNSUB || doNORMV || doMERGE || doDIFF || doPULL)) {
        std::cout << "RATIO cannot be combined with other operations" << std::endl;
        std::cout << "Exiting" << std::endl;
        return;
    }
    else if (doDIFF && (doSCALEBINW || doBKGSUB || doSBSUB || doSAMESIGNSUB || doNORMV || doMERGE || doRATIO || doPULL)) {
        std::cout << "DIFF cannot be combined with other operations" << std::endl;
        std::cout << "Exiting" << std::endl;
        return;
    }
    else if (doPULL && (doSCALEBINW || doBKGSUB || doSBSUB || doSAMESIGNSUB || doNORMV || doMERGE || doRATIO || doDIFF)) {
        std::cout << "PULL cannot be combined with other operations" << std::endl;
        std::cout << "Exiting" << std::endl;
        return;
    }
    else if ((doBKGSUB && doSBSUB) || (doBKGSUB && doSAMESIGNSUB) || (doSBSUB && doSAMESIGNSUB)) {
        std::cout << "BKGSUB, SBSUB, and SAMESIGNSUB cannot be combined" << std::endl;
        std::cout << "Exiting" << std::endl;
        return;
    }

    bool doSUB = (doBKGSUB || doSBSUB || doSAMESIGNSUB);

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
    int nInputFilePairs = nInputFiles / 2;

    int nInputObjs = inputObjs.size();
    int nInputObjPairs = nInputObjs / 2;

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

    std::vector<TH1*> hIn[nInputFiles];
    hIn[0].clear();
    hIn[0].resize(nInputObjs, 0);

    TFile* output = TFile::Open(outputFile.c_str(), writeMode.c_str());
    output->cd();

    TH1* hTmp = 0;
    TH1* hTmp2 = 0;
    TH1* hTmpBkg = 0;
    TH1* hOut = 0;

    TH2D* h2DTmp = 0;

    if (doSUB || doRATIO || doDIFF) {
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

            hIn[0][i] = 0;
            int iFile = (i % 2 == 0) ? 0 : 1;
            hIn[0][i] = (TH1*)inputs[iFile]->Get(inputObjs[i].c_str());
        }
    }
    else if (doPULL) {
        if (nInputFiles % 2 != 0) {
            std::cout << "Input files must come in pairs for PULL operation : "
                    "       one EXPERIMENT, one TRUTH object" << std::endl;
            std::cout << "Exiting." << std::endl;
            return;
        }
        if (nInputObjs % 2 != 0) {
            std::cout << "Input objects must come in pairs for PULL operation : "
                    "       one EXPERIMENT, one TRUTH object" << std::endl;
            std::cout << "Exiting." << std::endl;
            return;
        }

        for (int iFile = 0; iFile < nInputFiles; ++iFile) {
            hIn[iFile].clear();
            hIn[iFile].resize(nInputObjPairs, 0);
        }

        for (int i = 0; i < nInputObjs; ++i) {
            // i % 2 == 0 --> experiment
            // i % 2 == 1 --> truth

            int iPair = i / 2;

            for (int iFile = 0; iFile < nInputFiles; ++iFile) {

                if (i % 2 == 0 && iFile % 2 == 0) {
                    hIn[iFile][iPair] = 0;
                    hIn[iFile][iPair] = (TH1*)inputs[iFile]->Get(inputObjs[i].c_str());
                }
                else if (i % 2 == 1 && iFile % 2 == 1) {
                    hIn[iFile][iPair] = 0;
                    hIn[iFile][iPair] = (TH1*)inputs[iFile]->Get(inputObjs[i].c_str());
                }
            }
        }
    }

    if (doSUB) {

        std::vector<std::string> hPaths_vPt;

        for (int i = 0; i < nInputObjs; i+=2) {

            int iRaw = i;
            int iBkg = i+1;

            if (hIn[0][iRaw] == 0) {
                std::cout << "Object not found : " << inputObjs[i].c_str() << std::endl;
                std::cout << "relevant file : " << inputFiles[0].c_str() << std::endl;
                std::cout << "skipping calculation involving this object" << std::endl;
                continue;
            }
            if (hIn[0][iBkg] == 0) {
                std::cout << "Object not found : " << inputObjs[i].c_str() << std::endl;
                std::cout << "relevant file : " << inputFiles[1].c_str() << std::endl;
                std::cout << "skipping calculation involving this object" << std::endl;
                continue;
            }

            std::string tmpName_vPt = hIn[0][iRaw]->GetName();
            std::string histPath_vPt = parsePathTH1vPt(tmpName_vPt);
            if (doSAMESIGNSUB) {
                //std::cout << "here 0 : " << histPath_vPt << std::endl;
                histPath_vPt = replaceAll(histPath_vPt, "_sig", "");
            }

            hTmp = (TH1*)inputs[0]->Get(histPath_vPt.c_str());
            if (doSAMESIGNSUB) {
                hTmpBkg = (TH1*)inputs[1]->Get(histPath_vPt.c_str());
            }

            if (!containsElement(hPaths_vPt, histPath_vPt)) {

                hPaths_vPt.push_back(histPath_vPt);
                if (doSAMESIGNSUB) {
                    hTmp->SetName(Form("%s_os", histPath_vPt.c_str()));
                }
                hTmp->Write("", TObject::kOverwrite);

                if (doSAMESIGNSUB) {
                    hTmpBkg->SetName(Form("%s_ss", histPath_vPt.c_str()));
                    hTmpBkg->Write("", TObject::kOverwrite);
                }
            }

            double nV = 1;
            if (doNORMV) {
                int binMin = hTmp->GetXaxis()->FindBin(parseVPtMin(tmpName_vPt));
                int binMax = hTmp->GetXaxis()->FindBin(parseVPtMax(tmpName_vPt)) - 1;
                if (binMax < binMin) {
                    binMax = hTmp->GetNbinsX() + 1;
                }
                nV = hTmp->Integral(binMin, binMax);

                if (doSAMESIGNSUB) {
                    nV -= hTmpBkg->Integral(binMin, binMax);
                }
            }

            setTH1(hIn[0][iRaw]);
            setTH1(hIn[0][iBkg]);

            if (doBKGSUB || doSAMESIGNSUB) {
                hTmpBkg = (TH1*)hIn[0][iBkg]->Clone(Form("%s_tmpBkg", hIn[0][iBkg]->GetName()));

                std::string tmpNameRaw = hIn[0][iRaw]->GetName();
                std::string tmpNameBkg = hIn[0][iBkg]->GetName();
                if (doSAMESIGNSUB) {
                    tmpNameRaw = replaceAll(tmpNameRaw, "_sig", "");
                    tmpNameBkg = replaceAll(tmpNameBkg, "_sig", "");
                }
                hOut = (TH1*)hIn[0][iRaw]->Clone(Form("%s_sig", tmpNameRaw.c_str()));
                hOut->Add(hTmpBkg, -1);

                // write objects
                hTmp = (TH1*)hIn[0][iRaw]->Clone(Form("%s_raw", tmpNameRaw.c_str()));
                hTmp->Scale(1.0 / nV);
                if (doSCALEBINW) {
                    hTmp->Scale(1.0, "width");
                }
                hTmp->Write("", TObject::kOverwrite);

                hTmp2 = (TH1*)hTmpBkg->Clone(Form("%s_bkg", tmpNameBkg.c_str()));
                hTmpBkg->Delete();
                hTmp2->Scale(1.0 / nV);
                if (doSCALEBINW) {
                    hTmp2->Scale(1.0, "width");
                }
                hTmp2->Write("", TObject::kOverwrite);

                hOut->Scale(1.0 / nV);
                if (doSCALEBINW) {
                    hOut->Scale(1.0, "width");
                }
                hOut->Write("",TObject::kOverwrite);
                //hOut->Delete();

                if (doBOOTSTRAP && isBootStrapHist(tmpNameRaw)
                                && hIn[0][iRaw]->InheritsFrom("TH2D")) {

                    hTmp->Write("",TObject::kOverwrite);
                    hTmp = (TH1*)calcTH1BootStrap((TH2D*)hTmp);      // raw
                    hTmp->Write("",TObject::kOverwrite);

                    hTmp2->Write("",TObject::kOverwrite);
                    hTmp2 = (TH1*)calcTH1BootStrap((TH2D*)hTmp2);     // bkg
                    hTmp2->Write("",TObject::kOverwrite);

                    tmpNameRaw = hTmp->GetName();
                    //hOut = (TH1*)hTmp->Clone(replaceAll(tmpNameRaw, "_raw", "_sig").c_str());
                    hTmp->SetName(replaceAll(tmpNameRaw, "_raw", "_sig").c_str());
                    hTmp->Add(hTmp2, -1);
                    hTmp->Write("",TObject::kOverwrite);

                    std::string tmpNameSig2 = replaceAll(hOut->GetName(), "_sig", "_sig2");
                    hOut->SetName(tmpNameSig2.c_str());
                    hOut = (TH1*)calcTH1BootStrap((TH2D*)hOut);     // sig2
                    //tmpNameSig2 = replaceFirst(hOut->GetName(), "h_err_bs_diff_", "h_");
                    //hOut->SetName(tmpNameSig2.c_str());
                    hOut->Write("",TObject::kOverwrite);
                }
            }
            else if (doSBSUB) {
                // assume x-axis contains dphi, trkPt, xivh
                // assume y-axis contains deta

                if (!(hIn[0][iRaw]->InheritsFrom("TH2D"))) {
                    continue;
                }

                std::string strSB = "h2_deta_vs_";
                //std::string strSB = "h2_deta_h1_vs_";

                if (std::string(hIn[0][iRaw]->GetName()).find(strSB.c_str()) == std::string::npos) {
                    std::cout << "deta is not on y-axis : " << inputObjs[i].c_str() << std::endl;
                    std::cout << "skipping calculation involving this object" << std::endl;
                    continue;
                }

                hTmp = (TH1*)hIn[0][iRaw]->Clone(Form("%s_raw", hIn[0][iRaw]->GetName()));
                hTmp->Scale(1.0 / nV);
                hTmp->Write("", TObject::kOverwrite);

                hTmp = (TH1*)hIn[0][iBkg]->Clone(Form("%s_bkg", hIn[0][iBkg]->GetName()));
                hTmp->Scale(1.0 / nV);
                hTmp->Write("", TObject::kOverwrite);

                hTmpBkg = (TH1*)hIn[0][iBkg]->Clone(Form("%s_tmpBkg", hIn[0][iBkg]->GetName()));

                // bin with deta=0 contains the BKG normalization.
                //double normBKG = hTmpBkg->GetBinContent(1, 1);
                double normBKG = ((TH2D*)hTmpBkg)->ProjectionX("", 1, 1)->Integral();
                normBKG /= ((TH2D*)hTmpBkg)->ProjectionX("", 1, 1)->GetNbinsX();

                // raw corrected for acceptance
                hTmp = (TH1*)hIn[0][iRaw]->Clone(Form("%s_raw_accCorr", hIn[0][iRaw]->GetName()));
                //hTmp->Divide(hTmpBkg);
                for (int iBinX = 1; iBinX <= hTmp->GetNbinsX(); ++iBinX) {
                    for (int iBinY = 1; iBinY <= hTmp->GetNbinsY(); ++iBinY) {

                        double binC = hTmp->GetBinContent(iBinX, iBinY);
                        double binErr = hTmp->GetBinError(iBinX, iBinY);

                        double bkgBinC = hTmpBkg->GetBinContent(iBinX, iBinY);
                        double bkgBinRefC = hTmpBkg->GetBinContent(iBinX, 1);

                        double accCorr = 1;
                        if (bkgBinC > 0) {
                            accCorr = bkgBinRefC / bkgBinC;
                        }

                        binC *= accCorr;
                        binErr *= accCorr;

                        hTmp->SetBinContent(iBinX, iBinY, binC);
                        hTmp->SetBinError(iBinX, iBinY, binErr);
                    }
                }
                hTmpBkg->Delete();
                //hTmp->Scale(normBKG);
                hTmp->Scale(1.0 / nV);
                hTmp->Write("", TObject::kOverwrite);

                // sr = short-range
                // lr = long-range
                double detaMinSR = 0;
                double detaMaxSR = 1.0;
                //double detaMaxSR = 0.5;

                //double detaMinLR = 1.5;
                //double detaMaxLR = 3.5;
                //double detaMinLR = 1.5;
                //double detaMaxLR = 3.5;
                double detaMinLR = 2.0;
                double detaMaxLR = 4.0;

                int binDetaMinSR = hTmp->GetYaxis()->FindBin(detaMinSR);
                int binDetaMaxSR = hTmp->GetYaxis()->FindBin(detaMaxSR);

                int binDetaMinLR = hTmp->GetYaxis()->FindBin(detaMinLR);
                int binDetaMaxLR = hTmp->GetYaxis()->FindBin(detaMaxLR);

                double detaSR = hTmp->GetYaxis()->GetBinLowEdge(binDetaMaxSR+1) - hTmp->GetYaxis()->GetBinLowEdge(binDetaMinSR);
                double detaLR = hTmp->GetYaxis()->GetBinLowEdge(binDetaMaxLR+1) - hTmp->GetYaxis()->GetBinLowEdge(binDetaMinLR);
                double normLRtoSR = detaSR / detaLR;

                std::string name1D = replaceFirst(hIn[0][iRaw]->GetName(), strSB.c_str(), "h_");

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
    else if (doRATIO || doDIFF) {
        for (int i = 0; i < nInputObjs; i+=2) {

            int iNum = i;
            int iDnm = i+1;

            if (hIn[0][iNum] == 0) {
                std::cout << "Object not found : " << inputObjs[i].c_str() << std::endl;
                std::cout << "relevant file : " << inputFiles[0].c_str() << std::endl;
                std::cout << "skipping calculation involving this object" << std::endl;
                continue;
            }
            if (hIn[0][iDnm] == 0) {
                std::cout << "Object not found : " << inputObjs[i].c_str() << std::endl;
                std::cout << "relevant file : " << inputFiles[1].c_str() << std::endl;
                std::cout << "skipping calculation involving this object" << std::endl;
                continue;
            }

            setTH1(hIn[0][iNum]);
            setTH1(hIn[0][iDnm]);

            hTmp = (TH1*)hIn[0][iDnm]->Clone(Form("%s_tmpDenom", hIn[0][iDnm]->GetName()));

            std::string tmpName;
            std::string strOld = (hIn[0][iNum]->InheritsFrom("TH2D")) ? "h2_" : "h_";
            if (doRATIO) {
                tmpName = replaceFirst(hIn[0][iNum]->GetName(), strOld, strOld+"ratio_");
            }
            else if (doDIFF) {
                tmpName = replaceFirst(hIn[0][iNum]->GetName(), strOld, strOld+"diff_");
            }

            hOut = (TH1*)hIn[0][iNum]->Clone(tmpName.c_str());
            if (doRATIO) {
                hOut->Divide(hTmp);
                hOut->SetYTitle("PbPb / pp");
            }
            else if (doDIFF) {
                hOut->Add(hTmp, -1);
                hOut->SetYTitle("PbPb - pp");
            }

            hTmp->Delete();

            // write objects
            tmpName = replaceFirst(hIn[0][iNum]->GetName(), strOld, strOld+"num_");
            hTmp = (TH1*)hIn[0][iNum]->Clone(tmpName.c_str());
            hTmp->Write("", TObject::kOverwrite);

            tmpName = replaceFirst(hIn[0][iDnm]->GetName(), strOld, strOld+"denom_");
            hTmp = (TH1*)hIn[0][iDnm]->Clone(tmpName.c_str());
            hTmp->Write("", TObject::kOverwrite);

            hOut->Write("",TObject::kOverwrite);
        }
    }
    else if (doPULL) {

        int nExperiments = nInputFilePairs;

        for (int i = 0; i < nInputObjPairs; ++i) {

            for (int iExp = 0; iExp < nExperiments; ++iExp) {

                int iObjExp = iExp*2;
                int iObjTrue = iExp*2 + 1;

                if (hIn[iObjExp][i] == 0) {
                    std::cout << "Object not found : " << inputObjs[i*2].c_str() << std::endl;
                    std::cout << "relevant file : " << inputFiles[iObjExp].c_str() << std::endl;
                    std::cout << "skipping calculation involving this object" << std::endl;
                    continue;
                }
                if (hIn[iObjTrue][i] == 0) {
                    std::cout << "Object not found : " << inputObjs[i*2+1].c_str() << std::endl;
                    std::cout << "relevant file : " << inputFiles[iObjTrue].c_str() << std::endl;
                    std::cout << "skipping calculation involving this object" << std::endl;
                    continue;
                }

                setTH1(hIn[iObjExp][i]);
                setTH1(hIn[iObjTrue][i]);

                std::string tmpName;
                std::string strOld = (hIn[iObjExp][i]->InheritsFrom("TH2D")) ? "h2_" : "h_";
                tmpName = replaceFirst(hIn[iObjExp][i]->GetName(), strOld, strOld+Form("pull_%d_", iExp));

                hOut = (TH1*)getPullHistogram(hIn[iObjExp][i], hIn[iObjTrue][i]);
                hOut->SetName(tmpName.c_str());
                hOut->SetYTitle("pull");

                // write objects
                tmpName = replaceFirst(hIn[iObjExp][i]->GetName(), strOld, strOld+Form("exp_%d_", iExp));
                hTmp = (TH1*)hIn[iObjExp][i]->Clone(tmpName.c_str());
                hTmp->Write("", TObject::kOverwrite);

                tmpName = replaceFirst(hIn[iObjTrue][i]->GetName(), strOld, strOld+Form("true_%d_", iExp));
                hTmp = (TH1*)hIn[iObjTrue][i]->Clone(tmpName.c_str());
                hTmp->Write("", TObject::kOverwrite);

                hOut->Write("",TObject::kOverwrite);

                if (iExp == 0) {
                    std::vector<double> binsXTmp = getTH1xBins(hIn[iObjExp][i]);
                    int nBinsX = binsXTmp.size()-1;

                    double arr[nBinsX+1];
                    std::copy(binsXTmp.begin(), binsXTmp.end(), arr);

                    tmpName = replaceFirst(hIn[iObjExp][i]->GetName(), strOld, "h2_pull_vs_");
                    h2DTmp = 0;
                    h2DTmp = new TH2D(tmpName.c_str(), "", nBinsX, arr, 100, -4, 4);
                    h2DTmp->SetTitle(hIn[iObjExp][i]->GetTitle());
                    h2DTmp->SetXTitle(hIn[iObjExp][i]->GetXaxis()->GetTitle());
                    h2DTmp->SetYTitle("pull");
                }

                int nBinsX = hOut->GetNbinsX();
                for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {

                    double binXCenter = hOut->GetBinCenter(iBinX);
                    double binContent = hOut->GetBinContent(iBinX);

                    h2DTmp->Fill(binXCenter, binContent);
                }

                if (iExp == nExperiments - 1) {
                    h2DTmp->Write("",TObject::kOverwrite);

                    for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {

                        tmpName = Form("%s_projY_bin_%d", h2DTmp->GetName(), iBinX);
                        hTmp = (TH1D*)h2DTmp->ProjectionY(tmpName.c_str(), iBinX, iBinX);
                        hTmp->Write("",TObject::kOverwrite);
                    }

                    std::vector<TH1D*> hProj(3, 0);
                    for (int iProj = 0; iProj < 1; ++iProj) {
                        if (iProj == 0) {
                            hProj[0] = (TH1D*)((TH2D*)h2DTmp)->ProjectionX(Form("%s_projX", h2DTmp->GetName()));
                        }
                        else {
                            hProj[0] = (TH1D*)((TH2D*)h2DTmp)->ProjectionY(Form("%s_projY", h2DTmp->GetName()));
                        }

                        hProj[0]->Write("",TObject::kOverwrite);

                        hProj[1] = (TH1D*)hProj[0]->Clone(Form("%s_mean", hProj[0]->GetName()));
                        hProj[2] = (TH1D*)hProj[0]->Clone(Form("%s_stddev", hProj[0]->GetName()));

                        std::string projYTitle = (iProj == 0) ? ((TH2D*)h2DTmp)->GetYaxis()->GetTitle() : ((TH2D*)h2DTmp)->GetXaxis()->GetTitle();
                        hProj[1]->SetYTitle(Form("< %s >", projYTitle.c_str()));
                        hProj[2]->SetYTitle(Form("#sigma( %s )", projYTitle.c_str()));

                        setBinsFromTH2sliceMean(hProj[1], (TH2D*)h2DTmp, (iProj == 0));
                        setBinsFromTH2sliceStdDev(hProj[2], (TH2D*)h2DTmp, (iProj == 0));

                        hProj[1]->Write("",TObject::kOverwrite);
                        hProj[2]->Write("",TObject::kOverwrite);
                    }
                }
            }
        }
    }
    else if (doNORMV) {
        if (nInputFiles != 1) {
            std::cout << "There must be 1 input file if operation is only NORMV" << std::endl;
            std::cout << "Exiting." << std::endl;
            return;
        }

        for (int i = 0; i < nInputObjs; ++i) {

            hIn[0][i] = 0;
            hIn[0][i] = (TH1*)inputs[0]->Get(inputObjs[i].c_str());
        }

        for (int i = 0; i < nInputObjs; ++i) {

            if (hIn[0][i] == 0) {
                std::cout << "Object not found : " << inputObjs[i].c_str() << std::endl;
                std::cout << "relevant file : " << inputFiles[0].c_str() << std::endl;
                std::cout << "skipping calculation involving this object" << std::endl;
                continue;
            }

            std::string tmpName = hIn[0][i]->GetName();
            hOut = (TH1*)hIn[0][i]->Clone(tmpName.c_str());

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

            if (doBOOTSTRAP && isBootStrapHist(tmpName)) {

                hTmp = (TH1*)inputs[0]->Get(tmpName.c_str());
                if ( !(hTmp->InheritsFrom("TH2D")) ) continue;

                hTmp->Scale(1.0 / nV);
                if (doSCALEBINW) {
                    hTmp->Scale(1.0, "width");
                }

                calcTH1BootStrap((TH2D*)hTmp);
            }
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

        hIn[0].clear();
        hIn[0].assign(2, 0);

        while ((key=(TKey*)iter->Next()))
        {
            if (!(key->ReadObj()->InheritsFrom("TH1D") || key->ReadObj()->InheritsFrom("TH2D")))  continue;

            hIn[0][0] = 0;
            hIn[0][0] = (TH1*)key->ReadObj();

            std::string objName = hIn[0][0]->GetName();

            if (!keyList[1]->Contains(objName.c_str()))  continue;

            hIn[0][1] = (TH1*)inputs[1]->Get(objName.c_str());

            hIn[0][0]->Scale(w1);
            hIn[0][1]->Scale(w2);

            hOut = 0;
            hOut = (TH1*)hIn[0][0]->Clone();
            hOut->Add(hIn[0][1]);

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

TH1* calcTH1BootStrap(TH2D* h2D_bs)
{

    std::string  tmpName = h2D_bs->GetName();
    int nBinsY = h2D_bs->GetYaxis()->GetNbins();

    TH1D* hTmp_bs = 0;
    TH1* hTmp2_bs = 0;
    TH1* hOut_bs_diff = 0;

    // find number of events, ie the last bin that is filled
    hTmp_bs = (TH1D*)(h2D_bs->ProjectionY(Form("%s_projYtmp", tmpName.c_str())));
    int nEvtsY = 0;
    for (int iEvtY = nBinsY; iEvtY >= 1; --iEvtY) {

        if (hTmp_bs->GetBinError(iEvtY) > 0)  {
            nEvtsY = iEvtY;
            break;
        }
    }
    hTmp_bs->Delete();

    hTmp_bs = (TH1D*)(h2D_bs->ProjectionX(Form("%s_projXtmp", tmpName.c_str())));
    std::vector<double> binsX = getTH1xBins(hTmp_bs);

    int nBinsX = binsX.size()-1;

    double tmpArr[nBinsX+1];
    std::copy(binsX.begin(), binsX.end(), tmpArr);

    std::string tmpNameOut;

    double yMin_bs_diff = -1;
    double yMax_bs_diff = -1;
    std::string tmpLabel = replaceFirst(tmpName, "h2_bs_", "");
    if (isDphi(tmpLabel)) {
        yMin_bs_diff = -10;
        yMax_bs_diff = 10;
    }
    else if (isXivh(tmpLabel)) {
        yMin_bs_diff = -5;
        yMax_bs_diff = 5;
    }
    else if (isTrkPt(tmpLabel)) {
        yMin_bs_diff = -2.5;
        yMax_bs_diff = 2.5;
    }

    tmpNameOut = replaceFirst(tmpName.c_str(), "h2_bs_", "h2_bs_samples_real_diff_vs_");
    hOut_bs_diff = new TH2D(tmpNameOut.c_str(), Form("%s;%s;", h2D_bs->GetTitle(), h2D_bs->GetXaxis()->GetTitle()),
            nBinsX, tmpArr, 1000, yMin_bs_diff, yMax_bs_diff);

    int nSamples = 400;
    TRandom3 randEvt(12345);

    std::cout << "tmpName = " << tmpName << std::endl;
    std::cout << "nEvtsY = " << nEvtsY << std::endl;

    // there are nEvtsY samples. Each sample has nEvtsY many events.

    std::vector<double> f_bs[nBinsX];
    for (int iSample = 0; iSample < nSamples; ++iSample) {

        hTmp2_bs = (TH1D*)(h2D_bs->ProjectionX(Form("%s_projX_iSample_%d", tmpName.c_str(), iSample)));
        hTmp2_bs->Reset();
        for (int iEvt = 1; iEvt <= nEvtsY; ++iEvt) {

            int evtNum = randEvt.Uniform(1, nEvtsY+1);

            hTmp2_bs->Add(h2D_bs->ProjectionX("", evtNum, evtNum));
        }

        for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {

            double yReal = hTmp_bs->GetBinContent(iBinX);
            double ySample = hTmp2_bs->GetBinContent(iBinX);

            double binCenter = hTmp_bs->GetBinCenter(iBinX);

            hOut_bs_diff->Fill(binCenter, ySample - yReal);

            f_bs[iBinX-1].push_back(ySample);
        }
        hTmp2_bs->Delete();
    }

    hOut_bs_diff->Write("",TObject::kOverwrite);

    std::vector<TH1D*> vec_bs_out;

    h2D_bs = (TH2D*)hOut_bs_diff;

    std::vector<TH1D*> hProj(3, 0);
    hProj[0] = (TH1D*)((TH2D*)h2D_bs)->ProjectionX(Form("%s_projX", h2D_bs->GetName()));

    hProj[1] = (TH1D*)hProj[0]->Clone(Form("%s_mean", hProj[0]->GetName()));
    hProj[2] = (TH1D*)hProj[0]->Clone(Form("%s_stddev", hProj[0]->GetName()));

    std::string projYTitle = ((TH2D*)h2D_bs)->GetYaxis()->GetTitle();
    hProj[1]->SetYTitle(Form("< %s >", projYTitle.c_str()));
    hProj[2]->SetYTitle(Form("#sigma( %s )", projYTitle.c_str()));

    setBinsFromTH2sliceMean(hProj[1], (TH2D*)h2D_bs, (true));
    setBinsFromTH2sliceStdDev(hProj[2], (TH2D*)h2D_bs, (true));

    // multiply "sample/real" ratios by "real"
    std::string tmpSubStr = "diff";

    tmpNameOut = replaceFirst(tmpName.c_str(), "h2_bs_", Form("h_bs_%s_samples_tot_vs_", tmpSubStr.c_str()));
    hProj[0]->SetName(tmpNameOut.c_str());
    hProj[0]->Scale(((double)1.0/nSamples));

    tmpNameOut = replaceFirst(tmpName.c_str(), "h2_bs_", Form("h_bs_%s_samples_mean_vs_", tmpSubStr.c_str()));
    hProj[1]->SetName(tmpNameOut.c_str());

    tmpNameOut = replaceFirst(tmpName.c_str(), "h2_bs_", Form("h_bs_%s_samples_err_vs_", tmpSubStr.c_str()));
    hProj[2]->SetName(tmpNameOut.c_str());

    tmpNameOut = replaceFirst(tmpName, "h2_bs_", Form("h_err_bs_%s_", tmpSubStr.c_str()));
    hTmp_bs->SetName(tmpNameOut.c_str());

    for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {

        double yReal = hTmp_bs->GetBinContent(iBinX);
        double tmpY = hProj[0]->GetBinContent(iBinX);
        double tmpErr = hProj[0]->GetBinError(iBinX);

        hProj[0]->SetBinContent(iBinX, yReal + tmpY);
        hProj[0]->SetBinError(iBinX, tmpErr);

        tmpY = hProj[1]->GetBinContent(iBinX);
        tmpErr = hProj[1]->GetBinError(iBinX);
        hProj[1]->SetBinContent(iBinX, yReal + tmpY);
        hProj[1]->SetBinError(iBinX, tmpErr);

        hTmp_bs->SetBinError(iBinX, hProj[2]->GetBinContent(iBinX));
    }

    hProj[0]->Write("",TObject::kOverwrite);
    hProj[1]->Write("",TObject::kOverwrite);
    hProj[2]->Write("",TObject::kOverwrite);

    std::cout << "wrote bs samples tot,mean,err for : " << h2D_bs->GetName() << std::endl;

    hTmp_bs->Write("",TObject::kOverwrite);
    vec_bs_out.push_back(hTmp_bs);

    tmpNameOut = replaceFirst(tmpName, "h2_bs_", "h_err_bs_");
    hTmp_bs->SetName(tmpNameOut.c_str());

    for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {

        int nSamplesTmp = f_bs[iBinX-1].size();

        double tmpArr[nSamplesTmp];
        std::copy(f_bs[iBinX-1].begin(), f_bs[iBinX-1].end(), tmpArr);

        double tmpErr = TMath::StdDev(nSamplesTmp, tmpArr);
        hTmp_bs->SetBinError(iBinX, tmpErr);
    }

    hTmp_bs->Write("",TObject::kOverwrite);

    //return vec_bs_out[k_bs_diff];
    return hTmp_bs;
}
