/*
 * macro to combine zJetHistogram files from dielectron and dimuon channels
 */
#include <TFile.h>
#include <TDirectoryFile.h>
#include <TList.h>
#include <TKey.h>
#include <TCanvas.h>
#include <TCut.h>
#include <TH1D.h>
#include <TMath.h>

#include <vector>
#include <string>
#include <iostream>

#include "../../TreeHeaders/CutConfigurationTree.h"
#include "../../Plotting/commonUtility.h"
#include "../interface/correlationHist.h"
#include "../../Utilities/interface/CutConfigurationParser.h"
#include "../../Utilities/interface/InputConfigurationParser.h"
#include "../../Utilities/eventUtil.h"
#include "../../Utilities/fileUtil.h"
#include "../../Utilities/systemUtil.h"
#include "../../Utilities/th1Util.h"
#include "../../Utilities/bosonJetUtil.h"

void zJetHistogramSum(const TString configFile, const TString inputFileDiEle, const TString inputFileDiMu, const TString outputFile = "zJetHistogramSum.root");

void zJetHistogramSum(const TString configFile, const TString inputFileDiEle, const TString inputFileDiMu, const TString outputFile)
{
    std::cout<<"running zJetHistogramSum()"<<std::endl;
    std::cout<<"configFile = "<< configFile.Data() <<std::endl;
    std::cout<<"inputFileDiEle = "<< inputFileDiEle.Data() <<std::endl;
    std::cout<<"inputFileDiMu  = "<< inputFileDiMu.Data() <<std::endl;
    std::cout<<"outputFile = "<< outputFile.Data() <<std::endl;

    InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

    // input configuration
    int collision;

    // input for TH1
    std::vector<std::string> TH1_paths;      // paths of TH1D histograms

    if (configInput.isValid) {
        collision = configInput.proc[INPUT::kHISTOGRAM].i[INPUT::k_collisionType];

        TH1_paths   = ConfigurationParser::ParseList(configInput.proc[INPUT::kHISTOGRAM].s[INPUT::k_TH1_path]);
    }
    else {
        collision = COLL::kPP;
    }
    const char* collisionName =  getCollisionTypeName((COLL::TYPE)collision).c_str();
    int nTH1_Paths = TH1_paths.size();

    // verbose about input configuration
    std::cout<<"Input Configuration :"<<std::endl;
    std::cout << "collision = " << collisionName << std::endl;
    std::cout << "nTH1_Paths = " << nTH1_Paths << std::endl;
    for (int i = 0; i<nTH1_Paths; ++i) {
            std::cout << Form("TH1_Path[%d] = %s", i, TH1_paths.at(i).c_str()) << std::endl;
    }

    bool isMC = collisionIsMC((COLL::TYPE)collision);
    bool isHI = collisionIsHI((COLL::TYPE)collision);

    // observable bins
    // for zJetHistogramSum.C, the whole purpose of "pt" and "hiBin" bins is to get the number of bins used.
    std::vector<float> bins_pt[2];          // array of vectors for eta bins, each array element is a vector.
    std::vector<int>   bins_hiBin[2];       // array of vectors for hiBin bins, each array element is a vector.

    if (configCuts.isValid) {
        bins_pt[0] = ConfigurationParser::ParseListFloat(
                configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kZBOSON].s[CUTS::ZBO::k_bins_pt_gt]);
        bins_pt[1] = ConfigurationParser::ParseListFloat(
                configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kZBOSON].s[CUTS::ZBO::k_bins_pt_lt]);
        bins_hiBin[0] = ConfigurationParser::ParseListInteger(
                configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_gt]);
        bins_hiBin[1] = ConfigurationParser::ParseListInteger(
                configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_lt]);
    }
    else {

    }
    // default values

    int nBins_pt = bins_pt[0].size();         // assume <myvector>[0] and <myvector>[1] have the same size.
    int nBins_hiBin = bins_hiBin[0].size();     // assume <myvector>[0] and <myvector>[1] have the same size.
    // verbose about cut configuration
    std::cout<<"Cut Configuration :"<<std::endl;
    std::cout << "nBins_pt = " << nBins_pt << std::endl;
    for (int i=0; i<nBins_pt; ++i) {
        std::cout << Form("bins_pt[%d] = [%.1f, %.1f)", i, bins_pt[0].at(i), bins_pt[1].at(i)) << std::endl;
    }
    std::cout << "nBins_hiBin = " << nBins_hiBin << std::endl;
    for (int i=0; i<nBins_hiBin; ++i) {
        std::cout << Form("bins_hiBin[%d] = [%d, %d)", i, bins_hiBin[0].at(i), bins_hiBin[1].at(i)) << std::endl;
    }

    TFile* inputEle = new TFile(inputFileDiEle, "READ");
    TFile* inputMu  = new TFile(inputFileDiMu , "READ");

    TDirectoryFile* dirEle = (TDirectoryFile*)inputEle->GetDirectory(collisionName);
    TDirectoryFile* dirMu  = (TDirectoryFile*)inputMu->GetDirectory(collisionName);

    TList* keysHistoEle = getListOfALLHistograms(dirEle);  // get all histograms from zJet - electron channel file
    TList* keysHistoMu  = getListOfALLHistograms(dirMu);   // get all histograms from zJet - muon     channel file

    TFile* output = new TFile(outputFile, "RECREATE");
    // histograms will be put under a directory whose name is the type of the collision
    if(!output->GetKey(collisionName)) output->mkdir(collisionName, Form("input file DiEle : %s, input file DiMu : %s",
                                                                          inputFileDiEle.Data(), inputFileDiMu.Data()));
    output->cd(collisionName);
    std::cout<<"histograms will be put under directory : " << collisionName << std::endl;

    TTree *configTree = setupConfigurationTreeForWriting(configCuts);

    const double PI = TMath::Pi();
    std::vector<std::string> correlationHistNames {"xjz", "dphi", "dphi_rebin", "dphi_normJZ", "dphi_rebin_normJZ",
                                                          "ptJet", "zM",
                                                          "zPt", "zEta", "zPhi", "jteta", "jtphi",
                                                          "nJet", "hiBin",
                                                          "xjz_binJER", "xjz_binJER2"};
    std::vector<bool>  normbyZcount {true, true, true, true, true,
                                           true, false,
                                           false, false, false, true, true,
                                           true, true,
                                           true, true};
    std::vector<std::string> versionSuffix {"final", "final", "final", "final", "final",
                                                     "final", "final",
                                                     "final", "final", "final", "final", "final",
                                                     "final", "final",
                                                     "final", "final"};
    std::vector<std::string> jetRegion    {"RAW", "RAW", "RAW", "RAW", "RAW",
                                                  "RAW", "RAW",
                                                  "RAW", "RAW", "RAW", "RAW", "RAW",
                                                  "RAW", "RAW",
                                                  "RAW", "RAW"};
    std::vector<std::string> jetRegionBKG {"BKG", "BKG", "BKG", "BKG", "BKG",
                                                  "BKG", "BKG",
                                                  "BKG", "BKG", "BKG", "BKG", "BKG",
                                                  "BKG", "BKG",
                                                  "BKG", "BKG"};
    if (!isHI) {
        std::cout << "There is no jetBKG for PP" <<std::endl;
        std::cout << "for PP : jetSIG = jetRAW" <<std::endl;
    }

    TH1::SetDefaultSumw2();

    int nCorrHist = correlationHistNames.size();
    std::vector<std::string> TH1_pathsEle;
    std::vector<std::string> TH1_pathsEle_jetBKG;
    std::vector<std::string> TH1_pathsMu;
    std::vector<std::string> TH1_pathsMu_jetBKG;
    std::vector<std::string> TH1_pathsEle_nums;   // paths to the histograms containing number of Z events
    std::vector<std::string> TH1_pathsMu_nums;
    for (int i = 0; i < nCorrHist; ++i) {
        std::string correlation = correlationHistNames.at(i).c_str();

        for (int iPt = 0; iPt < nBins_pt ; ++iPt) {
            for (int iHiBin = 0; iHiBin < nBins_hiBin ; ++iHiBin) {
                std::string tmpName = Form("%s_ptBin%d_hiBin%d_jet%s_%s", correlationHistNames.at(i).c_str(), iPt, iHiBin,
                                           jetRegion.at(i).c_str(), versionSuffix.at(i).c_str());
                std::string tmpHistName = Form("h1D_%s", tmpName.c_str());

                TH1_pathsEle.push_back(tmpHistName.c_str());
                TH1_pathsMu.push_back(tmpHistName.c_str());

                if (isHI) {
                    std::string tmpNameBKG = Form("%s_ptBin%d_hiBin%d_jet%s_%s", correlationHistNames.at(i).c_str(), iPt, iHiBin,
                            jetRegionBKG.at(i).c_str(), versionSuffix.at(i).c_str());
                    std::string tmpHistNameBKG = Form("h1D_%s", tmpNameBKG.c_str());

                    TH1_pathsEle_jetBKG.push_back(tmpHistNameBKG.c_str());
                    TH1_pathsMu_jetBKG.push_back(tmpHistNameBKG.c_str());
                }

                std::string tmpHistName_nums = Form("h_nums_ptBin%d_hiBin%d", iPt, iHiBin);
                TH1_pathsEle_nums.push_back(tmpHistName_nums.c_str());
                TH1_pathsMu_nums.push_back(tmpHistName_nums.c_str());
            }
        }
    }
    // special cases
    // add xjz in different Z pt bins for <xjz> calculation
    // add dphi in different Z pt bins for width(dphi) calculation
    const int nPtBins_rjz = 4;
    const int nBins_xjz_mean_ptBin = 4;
    const int offset_ptBins_rjz = 3; // ptBin 40-50 starts from index 3.
    const int offset_ptBins_xjz_mean = offset_ptBins_rjz;
    for (int i = 0; i < nBins_xjz_mean_ptBin; ++i) {

        int iPt = i + offset_ptBins_xjz_mean;
        for (int iHiBin = 0; iHiBin < nBins_hiBin ; ++iHiBin) {
            std::string tmpName = Form("xjz_ptBin%d_hiBin%d_jetRAW_final", iPt, iHiBin);
            std::string tmpHistName = Form("h1D_%s", tmpName.c_str());

            TH1_pathsEle.push_back(tmpHistName.c_str());
            TH1_pathsMu.push_back(tmpHistName.c_str());

            if (isHI) {
                std::string tmpNameBKG = Form("xjz_ptBin%d_hiBin%d_jetBKG_final", iPt, iHiBin);
                std::string tmpHistNameBKG = Form("h1D_%s", tmpNameBKG.c_str());

                TH1_pathsEle_jetBKG.push_back(tmpHistNameBKG.c_str());
                TH1_pathsMu_jetBKG.push_back(tmpHistNameBKG.c_str());
            }

            std::string tmpHistName_nums = Form("h_nums_ptBin%d_hiBin%d", iPt, iHiBin);
            TH1_pathsEle_nums.push_back(tmpHistName_nums.c_str());
            TH1_pathsMu_nums.push_back(tmpHistName_nums.c_str());
        }
        // for width(dphi)
        for (int iHiBin = 0; iHiBin < nBins_hiBin ; ++iHiBin) {
            std::string tmpName = Form("dphi_ptBin%d_hiBin%d_jetRAW_final", iPt, iHiBin);
            std::string tmpHistName = Form("h1D_%s", tmpName.c_str());

            TH1_pathsEle.push_back(tmpHistName.c_str());
            TH1_pathsMu.push_back(tmpHistName.c_str());

            if (isHI) {
                std::string tmpNameBKG = Form("dphi_ptBin%d_hiBin%d_jetBKG_final", iPt, iHiBin);
                std::string tmpHistNameBKG = Form("h1D_%s", tmpNameBKG.c_str());

                TH1_pathsEle_jetBKG.push_back(tmpHistNameBKG.c_str());
                TH1_pathsMu_jetBKG.push_back(tmpHistNameBKG.c_str());
            }

            std::string tmpHistName_nums = Form("h_nums_ptBin%d_hiBin%d", iPt, iHiBin);
            TH1_pathsEle_nums.push_back(tmpHistName_nums.c_str());
            TH1_pathsMu_nums.push_back(tmpHistName_nums.c_str());
        }
    }
    // special cases - END

    int nTH1_pathsEle = TH1_pathsEle.size();
    int nTH1_pathsMu  = TH1_pathsMu.size();
    if (nTH1_pathsEle != nTH1_pathsMu) {
        std::cout << "mismatch of number of histograms to be summed"<< std::endl;
        std::cout << "nTH1_pathsEle = "<< nTH1_pathsEle << std::endl;
        std::cout << "nTH1_pathsMu  = "<< nTH1_pathsMu << std::endl;
        std::cout << "exiting " << std::endl;
        return;
    }
    int nTH1 = 0;
    if (nTH1_pathsEle == nTH1_pathsMu) nTH1 = nTH1_pathsEle;
    std::cout << "nTH1 = " << nTH1 << std::endl;
    TH1D* hEle[nTH1_pathsEle];
    TH1D* hMu [nTH1_pathsMu];
    TH1D* hEle_jetBKG[nTH1_pathsEle];
    TH1D* hMu_jetBKG [nTH1_pathsMu];
    TH1D* hEle_nums[nTH1_pathsEle];
    TH1D* hMu_nums [nTH1_pathsMu];
    double nZeventsEle[nTH1_pathsEle];     // value by which the original histogram was divided
    double nZeventsMu [nTH1_pathsMu];      // value by which the original histogram was divided

    // arrays with <xjz>, <xjz> error, sumofweights values in dielectron channel
    double xjz_mean_Ele[nBins_pt][nBins_hiBin][CORR::kN_CORRFNC];
    double xjz_meanErr_Ele[nBins_pt][nBins_hiBin][CORR::kN_CORRFNC];
    double xjz_weight_Ele[nBins_pt][nBins_hiBin][CORR::kN_CORRFNC];

    // arrays with <xjz>, <xjz> error, sumofweights values in dimuon channel
    double xjz_mean_Mu[nBins_pt][nBins_hiBin][CORR::kN_CORRFNC];
    double xjz_meanErr_Mu[nBins_pt][nBins_hiBin][CORR::kN_CORRFNC];
    double xjz_weight_Mu[nBins_pt][nBins_hiBin][CORR::kN_CORRFNC];

    bool matchZmmZeeRatio = isMC && isHI;  // scale the number of Zmm events to match the Zee/Zmm event ratio in DATA
    if (matchZmmZeeRatio)  {
        std::cout << "scale the number of Zmm events to match the Zee/Zmm event ratio in DATA" << std::endl;
    }
    double ratioZmmZee = 1;
    // dielectron channel
    for (int i=0; i<nTH1_pathsEle; ++i){
        std::string histPath = TH1_pathsEle.at(i).c_str();
        hEle[i] = (TH1D*)keysHistoEle->FindObject(histPath.c_str());

        if (isHI) {
            std::string histPathBKG = TH1_pathsEle_jetBKG.at(i).c_str();
            hEle_jetBKG[i] = (TH1D*)keysHistoEle->FindObject(histPathBKG.c_str());
        }

        // find number of Z events
        std::string histPath_nums = TH1_pathsEle_nums.at(i).c_str();
        hEle_nums[i] = (TH1D*)keysHistoEle->FindObject(histPath_nums.c_str());
        nZeventsEle[i] = double(hEle_nums[i]->GetBinContent(2));     // 2nd bin contains the number of Z events

        // read <xjz>, <xjz> error, sumofweights values
        for (int iPt = 0; iPt < nBins_pt ; ++iPt) {
            if (histPath_nums.find(Form("_ptBin%d", iPt)) == std::string::npos)  continue;
            for (int iHiBin = 0; iHiBin < nBins_hiBin ; ++iHiBin) {
                if (histPath_nums.find(Form("_hiBin%d", iHiBin)) == std::string::npos)  continue;

                int iBinRAW = 4;
                xjz_mean_Ele[iPt][iHiBin][CORR::kRAW] = hEle_nums[i]->GetBinContent(iBinRAW);
                xjz_meanErr_Ele[iPt][iHiBin][CORR::kRAW] = hEle_nums[i]->GetBinError(iBinRAW);
                xjz_weight_Ele[iPt][iHiBin][CORR::kRAW] = hEle_nums[i]->GetBinContent(iBinRAW+3);

                int iBinBKG = 5;
                xjz_mean_Ele[iPt][iHiBin][CORR::kBKG] = hEle_nums[i]->GetBinContent(iBinBKG);
                xjz_meanErr_Ele[iPt][iHiBin][CORR::kBKG] = hEle_nums[i]->GetBinError(iBinBKG);
                xjz_weight_Ele[iPt][iHiBin][CORR::kBKG] = hEle_nums[i]->GetBinContent(iBinBKG+3);

                int iBinSIG = 6;
                xjz_mean_Ele[iPt][iHiBin][CORR::kSIG] = hEle_nums[i]->GetBinContent(iBinSIG);
                xjz_meanErr_Ele[iPt][iHiBin][CORR::kSIG] = hEle_nums[i]->GetBinError(iBinSIG);
                xjz_weight_Ele[iPt][iHiBin][CORR::kSIG] = hEle_nums[i]->GetBinContent(iBinSIG+3);
            }
        }
    }
    // dimuon channel
    for (int i=0; i<nTH1_pathsMu; ++i){
        std::string histPath = TH1_pathsMu.at(i).c_str();
        hMu[i] = (TH1D*)keysHistoMu->FindObject(histPath.c_str());

        if (isHI) {
            std::string histPathBKG = TH1_pathsMu_jetBKG.at(i).c_str();
            hMu_jetBKG[i] = (TH1D*)keysHistoMu->FindObject(histPathBKG.c_str());
        }

        // find number of Z events
        std::string histPath_nums = TH1_pathsMu_nums.at(i).c_str();
        hMu_nums[i] = (TH1D*)keysHistoMu->FindObject(histPath_nums.c_str());
        nZeventsMu[i] = double(hMu_nums[i]->GetBinContent(2));     // 2nd bin contains the number of Z events
        if (matchZmmZeeRatio)  {
            // this scaling is hard coded.
            ratioZmmZee = (164/74.9714);
            nZeventsMu[i] = ratioZmmZee*nZeventsEle[i];
        }

        // read <xjz>, <xjz> error, sumofweights values
        for (int iPt = 0; iPt < nBins_pt ; ++iPt) {
            if (histPath_nums.find(Form("_ptBin%d", iPt)) == std::string::npos)  continue;
            for (int iHiBin = 0; iHiBin < nBins_hiBin ; ++iHiBin) {
                if (histPath_nums.find(Form("_hiBin%d", iHiBin)) == std::string::npos)  continue;

                int iBinRAW = 4;
                xjz_mean_Mu[iPt][iHiBin][CORR::kRAW] = hMu_nums[i]->GetBinContent(iBinRAW);
                xjz_meanErr_Mu[iPt][iHiBin][CORR::kRAW] = hMu_nums[i]->GetBinError(iBinRAW);
                xjz_weight_Mu[iPt][iHiBin][CORR::kRAW] = hMu_nums[i]->GetBinContent(iBinRAW+3);

                int iBinBKG = 5;
                xjz_mean_Mu[iPt][iHiBin][CORR::kBKG] = hMu_nums[i]->GetBinContent(iBinBKG);
                xjz_meanErr_Mu[iPt][iHiBin][CORR::kBKG] = hMu_nums[i]->GetBinError(iBinBKG);
                xjz_weight_Mu[iPt][iHiBin][CORR::kBKG] = hMu_nums[i]->GetBinContent(iBinBKG+3);

                int iBinSIG = 6;
                xjz_mean_Mu[iPt][iHiBin][CORR::kSIG] = hMu_nums[i]->GetBinContent(iBinSIG);
                xjz_meanErr_Mu[iPt][iHiBin][CORR::kSIG] = hMu_nums[i]->GetBinError(iBinSIG);
                xjz_weight_Mu[iPt][iHiBin][CORR::kSIG] = hMu_nums[i]->GetBinContent(iBinSIG+3);
            }
        }
    }

    // calculate <xjz> from sum of dielectron and dimuon channels
    // arrays with <xjz>, <xjz> error, sumofweights values for sum of dielectron and dimuon channels
    double xjz_mean[nBins_pt][nBins_hiBin][CORR::kN_CORRFNC];
    double xjz_meanErr[nBins_pt][nBins_hiBin][CORR::kN_CORRFNC];
    double xjz_weight[nBins_pt][nBins_hiBin][CORR::kN_CORRFNC];
    for (int iPt = 0; iPt < nBins_pt ; ++iPt) {
        for (int iHiBin = 0; iHiBin < nBins_hiBin ; ++iHiBin) {

            // calculate mean of jetRAW
            double wRAW_Ele = xjz_weight_Ele[iPt][iHiBin][CORR::kRAW];
            double meanRAW_Ele = xjz_mean_Ele[iPt][iHiBin][CORR::kRAW];
            double errRAW_Ele = xjz_meanErr_Ele[iPt][iHiBin][CORR::kRAW];
            double wRAW_Mu  = xjz_weight_Mu[iPt][iHiBin][CORR::kRAW];
            double meanRAW_Mu = xjz_mean_Mu[iPt][iHiBin][CORR::kRAW];
            double errRAW_Mu = xjz_meanErr_Mu[iPt][iHiBin][CORR::kRAW];

            double wRAW = (wRAW_Ele + wRAW_Mu);
            double meanRAW = (meanRAW_Ele*wRAW_Ele + meanRAW_Mu*wRAW_Mu) / wRAW;
            double errRAW = TMath::Sqrt(wRAW_Ele*wRAW_Ele*errRAW_Ele*errRAW_Ele + wRAW_Mu*wRAW_Mu*errRAW_Mu*errRAW_Mu) / wRAW;

            // calculate mean of jetBKG
            double wBKG_Ele = xjz_weight_Ele[iPt][iHiBin][CORR::kBKG];
            double meanBKG_Ele = xjz_mean_Ele[iPt][iHiBin][CORR::kBKG];
            double errBKG_Ele = xjz_meanErr_Ele[iPt][iHiBin][CORR::kBKG];
            double wBKG_Mu  = xjz_weight_Mu[iPt][iHiBin][CORR::kBKG];
            double meanBKG_Mu = xjz_mean_Mu[iPt][iHiBin][CORR::kBKG];
            double errBKG_Mu = xjz_meanErr_Mu[iPt][iHiBin][CORR::kBKG];

            double wBKG = (wBKG_Ele + wBKG_Mu);
            double meanBKG = 0;
            double errBKG = 0;
            if (wBKG > 0) {
                meanBKG = (meanBKG_Ele*wBKG_Ele + meanBKG_Mu*wBKG_Mu) / wBKG;
                errBKG = TMath::Sqrt(wBKG_Ele*wBKG_Ele*errBKG_Ele*errBKG_Ele + wBKG_Mu*wBKG_Mu*errBKG_Mu*errBKG_Mu) / wBKG;
            }

            /*
             * should follow this procedure to calculate mean of X_JZ :

              mean_RAW * w_RAW = mean_SIG * w_SIG + mean_BKG * w_BKG
              mean_RAW * w_RAW - mean_BKG * w_BKG = mean_SIG * w_SIG
              by definition : w_SIG = w_RAW - w_BKG
              ==> mean_SIG = (mean_RAW * w_RAW - mean_BKG * w_BKG) / (w_RAW - w_BKG)
             */
            double meanSIG = (meanRAW*wRAW - meanBKG*wBKG) / (wRAW - wBKG);

            /*
             * Var(aX+bY)=a^2*Var(X)+b^2*Var(Y)+2*a*b*Cov(X,Y)
             * ==>
             * w_RAW^2*var_RAW = w_SIG^2*var_SIG+w_BKG^2*var_BKG+2*w_SIG*w_BKG*Cov(SIG,BKG)
             * assume Cov(SIG,BKG) = 0
             * ==>
             * var_SIG = ( w_RAW^2*var_RAW-w_BKG^2*var_BKG ) / (w_RAW - w_BKG)^2
             * err_SIG = sqrt(var_SIG)
             */
            double errSIG = TMath::Sqrt(wRAW*wRAW*errRAW*errRAW - wBKG*wBKG*errBKG*errBKG) / (wRAW - wBKG);

            xjz_mean[iPt][iHiBin][CORR::kRAW] = meanRAW;
            xjz_meanErr[iPt][iHiBin][CORR::kRAW] = errRAW;
            xjz_weight[iPt][iHiBin][CORR::kRAW] = wRAW;

            xjz_mean[iPt][iHiBin][CORR::kBKG] = meanBKG;
            xjz_meanErr[iPt][iHiBin][CORR::kBKG] = errBKG;
            xjz_weight[iPt][iHiBin][CORR::kBKG] = wBKG;

            xjz_mean[iPt][iHiBin][CORR::kSIG] = meanSIG;
            xjz_meanErr[iPt][iHiBin][CORR::kSIG] = errSIG;
            xjz_weight[iPt][iHiBin][CORR::kSIG] = wRAW - wBKG;
        }
    }

    // HISTOGRAM ADDITION AND NORMALIZATION BLOCK
    // skip histograms that contain label "jetSIG" or "norm".
    // sum of individual jet signal or normalized histograms will not be correct.
    // histograms with label "norm" are extracted by the code, giving them as input is not necessary.
    // skip histograms that do not contain label "final"
    TH1D* h[nTH1];      // jetRAW
    TH1D* hNorm[nTH1];
    TH1D* hBKG[nTH1];   // jetBKG
    TH1D* hBKGNorm[nTH1];
    TH1D* hSIGNorm[nTH1];       // there is no unnormalized SIG histogram

    TH1D* h_nums[nTH1];
    // histograms to store numbers
    // 1st bin stores the number of zJet events
    // 2nd bin stores the number of Z events, not zJet event
    // 3rd bin stores the number of mixed MB events
    // 4th bin stores <xjz> of jetRAW correlation, 4th bin error stores <xjz> error of jetRAW correlation
    // 5th bin stores <xjz> of jetBKG correlation, 5th bin error stores <xjz> error of jetBKG correlation
    // 6th bin stores <xjz> of jetSIG correlation, 6th bin error stores <xjz> error of jetSIG correlation
    // 7th bin stores sumofweights of jetRAW xjz correlation
    // 8th bin stores sumofweights of jetBKG xjz correlation
    // 9th bin stores sumofweights of jetSIG xjz correlation : wSIG = wRAW - wBKG
    // they are just a tool to store numbers.

    bool hInitialized[nTH1];
    for (int i = 0; i<nTH1; ++i) {

        hInitialized[i] = false;

        // COMBINE dielectron and dimuon channels

        // special cases
        if (matchZmmZeeRatio) {
            double scaleZmm = ratioZmmZee*hEle[i]->GetSumOfWeights()/(hMu[i]->GetSumOfWeights());
            hMu[i]->Scale(scaleZmm);
            if (isHI)  hMu_jetBKG[i]->Scale(scaleZmm);
        }
        // special cases - END

        hInitialized[i] = true;
        // SUM h_nums histograms
        std::string hNameEle_nums = hEle_nums[i]->GetName();
        std::string hNameMu_nums = hMu_nums[i]->GetName();
        std::string hName_nums = Form("%s", hNameEle_nums.c_str());
        h_nums[i] = (TH1D*)hEle_nums[i]->Clone(hName_nums.c_str());
        h_nums[i]->SetBinContent(1, hEle_nums[i]->GetBinContent(1) + hMu_nums[i]->GetBinContent(1));
        h_nums[i]->SetBinContent(2, nZeventsEle[i]+nZeventsMu[i]);
        h_nums[i]->SetBinContent(3, (hEle_nums[i]->GetBinContent(3) + hMu_nums[i]->GetBinContent(3))/2);
        for (int iPt = 0; iPt < nBins_pt ; ++iPt) {
            if (hName_nums.find(Form("ptBin%d", iPt)) == std::string::npos)  continue;

            for (int iHiBin = 0; iHiBin < nBins_hiBin ; ++iHiBin) {
                if (hName_nums.find(Form("hiBin%d", iHiBin)) == std::string::npos)  continue;

                for (int jCorr = 0; jCorr < CORR::kN_CORRFNC; ++jCorr) {

                    int iBin = 4+jCorr;     // iBin = 4 contains information for jetRAW, iBin = 5 contains information for jetBKG
                    h_nums[i]->SetBinContent(iBin, xjz_mean[iPt][iHiBin][jCorr]);
                    h_nums[i]->SetBinError(iBin, xjz_meanErr[iPt][iHiBin][jCorr]);
                    h_nums[i]->SetBinContent(iBin+3, xjz_weight[iPt][iHiBin][jCorr]);
                }
            }
        }

        // SUM jetRAW histograms
        std::string hNameEle = hEle[i]->GetName();
        std::string hNameMu = hMu[i]->GetName();
        std::string hName = Form("%s", hNameEle.c_str());
        h[i] = (TH1D*)hEle[i]->Clone(hName.c_str());
        h[i]->Add(hMu[i], 1);

        double norm = nZeventsEle[i]+nZeventsMu[i];    // norm = # of Zee events + # of Zmm events

        std::string hNameNorm = Form("%s_norm", hName.c_str());
        hNorm[i] = (TH1D*)h[i]->Clone(hNameNorm.c_str());
        hNorm[i]->Scale(1./norm);

        // SUM jetBKG histograms
        std::string hNameBKG;
        std::string hNameBKGNorm;
        if (isHI) {
            std::string hNameEleBKG = hEle_jetBKG[i]->GetName();
            std::string hNameMuBKG  = hMu_jetBKG[i]->GetName();
            //hNameBKG    = Form("%s_sum", hNameEleBKG.c_str());
            hNameBKG    = Form("%s", hNameEleBKG.c_str());

            int nEventsToMixEle = int(hEle_nums[i]->GetBinContent(3));     // 3rd bin contains the number of number of mixed MB events
            int nEventsToMixMu  = int(hMu_nums[i]->GetBinContent(3));      // 3rd bin contains the number of number of mixed MB events

            hEle_jetBKG[i]->Scale(1./nEventsToMixEle);
            hMu_jetBKG[i]->Scale(1./nEventsToMixMu);

            hBKG[i] = (TH1D*)hEle_jetBKG[i]->Clone(hNameBKG.c_str());
            hBKG[i]->Add(hMu_jetBKG[i], 1);

            hNameBKGNorm = Form("%s_norm", hNameBKG.c_str());
            hBKGNorm[i] = (TH1D*)hBKG[i]->Clone(hNameBKGNorm.c_str());
            hBKGNorm[i]->Scale(1./norm);
        }

        // BACKGROUND SUBTRACTION
        // jetSIG = jetRAW - jetBKG
        std::string hNameSIGNorm = replaceAll(hNameNorm, "_jetRAW", "_jetSIG");
        hSIGNorm[i] = (TH1D*)hNorm[i]->Clone(hNameSIGNorm.c_str());
        if (isHI)  hSIGNorm[i]->Add(hBKGNorm[i], -1);

        std::string tmpTitleY = ((TH1D*)keysHistoEle->FindObject(hNameSIGNorm.c_str()))->GetYaxis()->GetTitle();
        hSIGNorm[i]->GetYaxis()->SetTitle(tmpTitleY.c_str());

        // computations are done.
        // print info about histograms
        std::cout << "#####" << std::endl;
        std::cout << "i = " << i << std::endl;

        std::cout << "#### normalizations ####" << std::endl;
        std::cout << "normEle = " << nZeventsEle[i] << std::endl;
        std::cout << "normMu  = " << nZeventsMu[i] << std::endl;
        std::cout << "norm    = " << norm << std::endl;

        std::cout << "#### dielectron histogram ####" << std::endl;
        std::cout << Form("hEle = %s", hNameEle.c_str()) << std::endl;
        std::string summaryEle = summaryTH1(hEle[i]);
        std::cout << summaryEle.c_str() << std::endl;

        std::cout << "#### dimuon histogram ####" << std::endl;
        std::cout << Form("hMu = %s", hNameMu.c_str()) << std::endl;
        std::string summaryMu = summaryTH1(hMu[i]);
        std::cout << summaryMu.c_str() << std::endl;

        std::cout << "#### SUM histogram ####" << std::endl;
        std::cout << Form("h = %s", hName.c_str()) << std::endl;
        std::string summary = summaryTH1(h[i]);
        std::cout << summary.c_str() << std::endl;

        std::cout << "#### SUM histogram - norm ####" << std::endl;
        std::cout << Form("hNorm = %s", hNameNorm.c_str()) << std::endl;
        std::string summaryNorm = summaryTH1(hNorm[i]);
        std::cout << summaryNorm.c_str() << std::endl;

        if (isHI) {
            std::cout << "#### SUM BKG histogram ####" << std::endl;
            std::cout << Form("hBKG = %s", hNameBKG.c_str()) << std::endl;
            std::string summaryBKG = summaryTH1(hBKG[i]);
            std::cout << summaryBKG.c_str() << std::endl;

            std::cout << "#### SUM BKG histogram - norm ####" << std::endl;
            std::cout << Form("hBKGNorm = %s", hNameBKGNorm.c_str()) << std::endl;
            std::string summaryBKGNorm = summaryTH1(hBKGNorm[i]);
            std::cout << summaryBKGNorm.c_str() << std::endl;
        }

        std::cout << "#### SUM SIG histogram - norm ####" << std::endl;
        std::cout << Form("hSIGNorm = %s", hNameSIGNorm.c_str()) << std::endl;
        std::string summarySIGNorm = summaryTH1(hSIGNorm[i]);
        std::cout << summarySIGNorm.c_str() << std::endl;
    }
    std::cout << "#####" << std::endl;

    // WRITE BLOCK
    TCanvas* c = new TCanvas("cnv","",600,600);
    for(int i = 0; i<nTH1; ++i)
    {
        if (!hInitialized[i])  continue;

        // write h_nums histogram, do not use Canvas for this one.
        h_nums[i]->Write("",TObject::kOverwrite);

        // write unnormalized jetRAW histogram
        std::string hName = h[i]->GetName();
        std::string canvasName = replaceAll(hName, "h1D_", "cnv_");     // replace the prefix in histogram name

        c->SetName(canvasName.c_str());
        c->cd();

        h[i]->Draw("e");
        h[i]->Write("",TObject::kOverwrite);

        c->Write("",TObject::kOverwrite);
        c->Clear();

        // write normalized jetRAW histogram
        std::string hNameNorm = hNorm[i]->GetName();
        canvasName = replaceAll(hNameNorm, "h1D_", "cnv_");     // replace the prefix in histogram name

        c->SetName(canvasName.c_str());
        c->cd();

        hNorm[i]->Draw("e");
        hNorm[i]->Write("",TObject::kOverwrite);

        c->Write("",TObject::kOverwrite);
        c->Clear();

        if (isHI) {
            // write unnormalized jetBKG histogram
            std::string hNameBKG = hBKG[i]->GetName();
            canvasName = replaceAll(hNameBKG, "h1D_", "cnv_");     // replace the prefix in histogram name

            c->SetName(canvasName.c_str());
            c->cd();

            hBKG[i]->Draw("e");
            hBKG[i]->Write("",TObject::kOverwrite);

            c->Write("",TObject::kOverwrite);
            c->Clear();

            // write normalized jetBKG histogram
            std::string hNameBKGNorm = hBKGNorm[i]->GetName();
            canvasName = replaceAll(hNameBKGNorm, "h1D_", "cnv_");     // replace the prefix in histogram name

            c->SetName(canvasName.c_str());
            c->cd();

            hBKGNorm[i]->Draw("e");
            hBKGNorm[i]->Write("",TObject::kOverwrite);

            c->Write("",TObject::kOverwrite);
            c->Clear();
        }

        // write normalized jetSIG histogram (there is no unnormalized jetSIG histogram)
        std::string hNameSIGNorm = hSIGNorm[i]->GetName();
        // special cases
        // normalized by number of Z+jet pairs
        if (hNameSIGNorm.find("_normJZ") != std::string::npos) {
            double integral = hSIGNorm[i]->Integral();
            hSIGNorm[i]->Scale(1./integral);
        }
        // special cases - END
        canvasName = replaceAll(hNameSIGNorm, "h1D_", "cnv_");     // replace the prefix in histogram name

        c->SetName(canvasName.c_str());
        c->cd();

        hSIGNorm[i]->Draw("e");
        hSIGNorm[i]->Write("",TObject::kOverwrite);

        c->Write("",TObject::kOverwrite);
        c->Clear();
    }

    // histograms with "Z pt bins" on x-axis
    std::vector<std::string> correlationHistNames_ptBinAll (ZJET::CORR_NAMES, ZJET::CORR_NAMES + ZJET::kN_ZJETCORR);       // histograms where x-axis is pt bins
    int nCorrHist_ptBinAll = correlationHistNames_ptBinAll.size();
    TH1D* corrHists_ptBinAll[nCorrHist_ptBinAll][nBins_hiBin][CORR::kN_CORRFNC];

    TH1D* rjz_num_Ele_ptBinAll[nBins_hiBin];       // histograms where x-axis is pt bins, y-axis is number of zJet events
    TH1D* rjz_denom_Ele_ptBinAll[nBins_hiBin];     // histograms where x-axis is pt bins, y-axis is number of Z events
    TH1D* rjz_num_Mu_ptBinAll[nBins_hiBin];        // histograms where x-axis is pt bins, y-axis is number of zJet events
    TH1D* rjz_denom_Mu_ptBinAll[nBins_hiBin];      // histograms where x-axis is pt bins, y-axis is number of Z events

    TH1D* rjz_num_ptBinAll[nBins_hiBin];       // histograms where x-axis is pt bins, y-axis is number of zJet events
    TH1D* rjz_denom_ptBinAll[nBins_hiBin];     // histograms where x-axis is pt bins, y-axis is number of Z events
    // R_JZ will be integral of X_JZ
    // R_JZ_zNum will be ( h_rjz_num / h_rjz_denom )

    // recalculate rjz and <xjz>
    // histograms with "Z pt bins" on x-axis
    // corrHists_ptBinAll[0][]  = R_jz
    // corrHists_ptBinAll[1][]  = <X_jz>
    // corrHists_ptBinAll[2][]  = R_jz from number of events
    std::cout<<"####################"<<std::endl;
    std::cout << "#### histograms with Z pt bins on x-axis ####" << std::endl;
    for(int iHiBin=0; iHiBin < nBins_hiBin; ++iHiBin){

        std::cout << "##### iHiBin = " << iHiBin << std::endl;

        std::string subHistName;
        std::string histPath;
        subHistName = Form("%s_ptBinAll_hiBin%d", correlationHistNames_ptBinAll.at(ZJET::kRJZ).c_str(), iHiBin);
        histPath = Form("h1D_%s", subHistName.c_str());
        corrHists_ptBinAll[ZJET::kRJZ][iHiBin][0] = (TH1D*)keysHistoEle->FindObject(histPath.c_str())->Clone(histPath.c_str());
        corrHists_ptBinAll[ZJET::kRJZ][iHiBin][0]->Reset("ICES");   // the histogram above is cloned just to copy axis properties

        for(int i=0; i<nPtBins_rjz; ++i){
            // calculate R_JZ from the integral of X_JZ
            int iPt = i + offset_ptBins_rjz;
            std::string tmpName = Form("xjz_ptBin%d_hiBin%d_jetSIG_final_norm", iPt, iHiBin);
            std::string tmpHistName = Form("h1D_%s", tmpName.c_str());

            // search through histograms that are created by summing dielectron and dimuon channels
            for (int iHist = 0; iHist < nTH1; ++iHist)
            {
                if (tmpHistName.compare(hSIGNorm[iHist]->GetName()) == 0) {

                    int nBinsX = hSIGNorm[iHist]->GetNbinsX();
                    double val;
                    double err;
                    // include the overflow bin in rjz calculation
                    val = hSIGNorm[iHist]->IntegralAndError(1, nBinsX+1, err, "width");

                    corrHists_ptBinAll[ZJET::kRJZ][iHiBin][0]->SetBinContent(i+1, val);
                    corrHists_ptBinAll[ZJET::kRJZ][iHiBin][0]->SetBinError(i+1, err);

                    break;
                }
            }
        }
        // R_JZ has been calculated from sum of electron and muon channels
        std::cout<<"drawing : "<<corrHists_ptBinAll[ZJET::kRJZ][iHiBin][0]->GetName()<<std::endl;
        c->SetName(Form("cnv_%s", subHistName.c_str()));
        c->cd();
        corrHists_ptBinAll[ZJET::kRJZ][iHiBin][0]->Draw("e");
        corrHists_ptBinAll[ZJET::kRJZ][iHiBin][0]->Write("",TObject::kOverwrite);
        corrHists_ptBinAll[ZJET::kRJZ][iHiBin][0]->SetStats(false);     // remove stat box from the canvas, but keep in the histograms.
        c->Write("",TObject::kOverwrite);
        c->Clear();

        // R_JZ calculation using number of events
        subHistName = Form("%s_num_ptBinAll_hiBin%d", correlationHistNames_ptBinAll.at(ZJET::kRJZ).c_str(), iHiBin);
        histPath = Form("h1D_%s", subHistName.c_str());
        rjz_num_Ele_ptBinAll[iHiBin] = (TH1D*)keysHistoEle->FindObject(histPath.c_str());
        rjz_num_Mu_ptBinAll [iHiBin] = (TH1D*)keysHistoMu->FindObject(histPath.c_str());
        // sum number of zJet events in different lepton channels
        rjz_num_ptBinAll[iHiBin] = (TH1D*)rjz_num_Ele_ptBinAll[iHiBin]->Clone(histPath.c_str());
        rjz_num_ptBinAll[iHiBin]->Add(rjz_num_Mu_ptBinAll[iHiBin], 1);

        subHistName = Form("%s_denom_ptBinAll_hiBin%d", correlationHistNames_ptBinAll.at(ZJET::kRJZ).c_str(), iHiBin);
        histPath = Form("h1D_%s", subHistName.c_str());
        rjz_denom_Ele_ptBinAll[iHiBin] = (TH1D*)keysHistoEle->FindObject(histPath.c_str());
        rjz_denom_Mu_ptBinAll [iHiBin] = (TH1D*)keysHistoMu->FindObject(histPath.c_str());
        // sum number of Z events in different lepton channels
        rjz_denom_ptBinAll[iHiBin] = (TH1D*)rjz_denom_Ele_ptBinAll[iHiBin]->Clone(histPath.c_str());
        rjz_denom_ptBinAll[iHiBin]->Add(rjz_denom_Mu_ptBinAll [iHiBin],1);

        subHistName = Form("%s_ptBinAll_hiBin%d", correlationHistNames_ptBinAll.at(ZJET::kRJZ_zNum).c_str(), iHiBin);
        histPath = Form("h1D_%s", subHistName.c_str());
        corrHists_ptBinAll[ZJET::kRJZ_zNum][iHiBin][0] = (TH1D*)keysHistoEle->FindObject(histPath.c_str())->Clone(histPath.c_str());
        corrHists_ptBinAll[ZJET::kRJZ_zNum][iHiBin][0]->Reset("ICES");   // the histogram above is cloned just to copy axis properties
        corrHists_ptBinAll[ZJET::kRJZ_zNum][iHiBin][0]->Add(rjz_num_ptBinAll[iHiBin]);
        corrHists_ptBinAll[ZJET::kRJZ_zNum][iHiBin][0]->Divide(rjz_denom_ptBinAll[iHiBin]);
        // R_JZ has been calculated from sum of electron and muon channels -  using number of events

        // write those histograms without creating a canvas for them
        rjz_num_ptBinAll[iHiBin]->Write("",TObject::kOverwrite);
        rjz_denom_ptBinAll[iHiBin]->Write("",TObject::kOverwrite);

        corrHists_ptBinAll[ZJET::kRJZ_zNum][iHiBin][0]->SetMarkerStyle(kFullCircle);
        corrHists_ptBinAll[ZJET::kRJZ_zNum][iHiBin][0]->SetMarkerColor(kBlack);

        std::cout<<"drawing : "<<corrHists_ptBinAll[ZJET::kRJZ_zNum][iHiBin][0]->GetName()<<std::endl;
        c->SetName(Form("cnv_%s", subHistName.c_str()));
        c->cd();
        corrHists_ptBinAll[ZJET::kRJZ_zNum][iHiBin][0]->Draw("e");
        corrHists_ptBinAll[ZJET::kRJZ_zNum][iHiBin][0]->Write("",TObject::kOverwrite);
        corrHists_ptBinAll[ZJET::kRJZ_zNum][iHiBin][0]->SetStats(false);     // remove stat box from the canvas, but keep in the histograms.
        c->Write("",TObject::kOverwrite);
        c->Clear();

        std::cout << "####" << std::endl;
        std::cout << Form("h = %s", corrHists_ptBinAll[ZJET::kRJZ][iHiBin][0]->GetName()) << std::endl;
        for (int bin = 1; bin <= corrHists_ptBinAll[ZJET::kRJZ][iHiBin][0]->GetNbinsX(); ++bin) {
            std::cout << "bin = " << bin << std::endl;
            std::cout << Form("rjz = %f", corrHists_ptBinAll[ZJET::kRJZ][iHiBin][0]->GetBinContent(bin)) << std::endl;
            std::cout << Form("rjz_zNum = %f", corrHists_ptBinAll[ZJET::kRJZ_zNum][iHiBin][0]->GetBinContent(bin)) << std::endl;
            std::cout << Form("rjz_num = %f", rjz_num_ptBinAll[iHiBin]->GetBinContent(bin)) << std::endl;
            std::cout << Form("rjz_denom = %f", rjz_denom_ptBinAll[iHiBin]->GetBinContent(bin)) << std::endl;
        }

        for(int jCorr = 0; jCorr<CORR::kN_CORRFNC; ++jCorr) {
            // xjz_mean block
            subHistName = Form("%s_ptBinAll_hiBin%d_%s", correlationHistNames_ptBinAll.at(ZJET::kXJZ_mean).c_str(), iHiBin,
                                                         CORR::CORR_JET_LABELS[jCorr].c_str());
            histPath = Form("h1D_%s", subHistName.c_str());
            corrHists_ptBinAll[ZJET::kXJZ_mean][iHiBin][jCorr] = (TH1D*)keysHistoEle->FindObject(histPath.c_str())->Clone(histPath.c_str());
            corrHists_ptBinAll[ZJET::kXJZ_mean][iHiBin][jCorr]->Reset("ICES");   // the histogram above is cloned just to copy axis properties

            for (int i = 0; i < nBins_xjz_mean_ptBin; ++i) {

                int iPt = i + offset_ptBins_xjz_mean;

                double val = xjz_mean[iPt][iHiBin][jCorr];
                double err = xjz_meanErr[iPt][iHiBin][jCorr];
                corrHists_ptBinAll[ZJET::kXJZ_mean][iHiBin][jCorr]->SetBinContent(i+1, val);
                corrHists_ptBinAll[ZJET::kXJZ_mean][iHiBin][jCorr]->SetBinError(i+1, err);
            }

            corrHists_ptBinAll[ZJET::kXJZ_mean][iHiBin][jCorr]->SetMarkerStyle(kFullCircle);
            corrHists_ptBinAll[ZJET::kXJZ_mean][iHiBin][jCorr]->SetMarkerColor(kBlack);

            std::cout<<"drawing : "<<corrHists_ptBinAll[ZJET::kXJZ_mean][iHiBin][jCorr]->GetName() <<std::endl;
            c->SetName(Form("cnv_%s", subHistName.c_str()));
            c->cd();
            corrHists_ptBinAll[ZJET::kXJZ_mean][iHiBin][jCorr]->Draw("e");
            corrHists_ptBinAll[ZJET::kXJZ_mean][iHiBin][jCorr]->Write("",TObject::kOverwrite);
            corrHists_ptBinAll[ZJET::kXJZ_mean][iHiBin][jCorr]->SetStats(false);     // remove stat box from the canvas, but keep in the histograms.
            c->Write("",TObject::kOverwrite);
            c->Clear();

            std::cout << "####" << std::endl;
            std::cout << Form("h = %s", corrHists_ptBinAll[ZJET::kXJZ_mean][iHiBin][jCorr]->GetName()) << std::endl;
            for (int bin = 1; bin <= corrHists_ptBinAll[ZJET::kXJZ_mean][iHiBin][jCorr]->GetNbinsX(); ++bin) {
                std::cout << Form("xjz_mean->GetBinContent(%d) = %f", bin, corrHists_ptBinAll[ZJET::kXJZ_mean][iHiBin][jCorr]->GetBinContent(bin)) << std::endl;
                std::cout << Form("xjz_mean->GetBinError(%d)   = %f", bin, corrHists_ptBinAll[ZJET::kXJZ_mean][iHiBin][jCorr]->GetBinError(bin)) << std::endl;
            }
        }
    }
    std::cout<<"####################"<<std::endl;

    std::cout<<"##### HISTOGRAMS WHICH ARE CALCULATED FROM HISTOGRAMS THAT ARE \"FILLED\" #####"<<std::endl;
    // HISTOGRAMS WHICH ARE MODIFICATIONS OF HISTOGRAMS THAT ARE "FILLED"
    // integral of dphi : CDF (dphi)
    // integral of dphi : CDF (dphi) - with variable binning
    TH1D* h1D_calc;
    TH1D* hTmp = 0;
    TF1*  f1_calc = 0;
    //for (int iPt=0; iPt<nBins_pt; ++iPt) {
    for (int iPt=0; iPt<7; ++iPt) {
        for (int iHiBin=0; iHiBin<nBins_hiBin; ++iHiBin) {
            std::cout<<"##########"<<std::endl;
            for(int iHist = 0; iHist<nCorrHist; ++iHist)
            {
                std::string tmpObservable = correlationHistNames.at(iHist).c_str();
                for (int jCorr = 0; jCorr < CORR::kN_CORRFNC; ++jCorr) {

                    if (jCorr == CORR::kBKG && !isHI)  continue;      // no jet background for non-HI

                    std::string tmpH1D_name = Form("%s_ptBin%d_hiBin%d_%s_final_norm", tmpObservable.c_str(), iPt, iHiBin, CORR::CORR_JET_LABELS[jCorr].c_str());
                    std::string tmpHistName = Form("h1D_%s", tmpH1D_name.c_str());

                    std::string tmpH1D_name_calc = "";
                    std::string tmpHistName_calc = "";

                    // search through histograms that are created by summing dielectron and dimuon channels
                    bool found = false;
                    for (int iHist2 = 0; iHist2 < nTH1; ++iHist2)
                    {
                        if (tmpHistName.compare(hNorm[iHist2]->GetName()) == 0) {
                            hTmp = (TH1D*)hNorm[iHist2]->Clone();
                            found = true;
                        }
                        else if (isHI && tmpHistName.compare(hBKGNorm[iHist2]->GetName()) == 0) {
                            hTmp = (TH1D*)hBKGNorm[iHist2]->Clone();
                            found = true;
                        }
                        else if (tmpHistName.compare(hSIGNorm[iHist2]->GetName()) == 0) {
                            hTmp = (TH1D*)hSIGNorm[iHist2]->Clone();
                            found = true;
                        }
                    }
                    if (!found)  continue;

                    // 1. integral of dphi : CDF (dphi)
                    // 2. integral of dphi : CDF (dphi) - with variable binning
                    if (tmpObservable.compare("dphi") == 0)
                    {
                        // histogram name excluding the "h1D" prefix
                        tmpH1D_name_calc = replaceAll(tmpH1D_name, "dphi_", "dphi_CDF_");
                        tmpHistName_calc = Form("h1D_%s", tmpH1D_name_calc.c_str());

                        h1D_calc = (TH1D*)hTmp->Clone(tmpHistName_calc.c_str());
                        TH1D* hTmp_Cumulative = (TH1D*)hTmp->GetCumulative();
                        for (int iBin = 0; iBin < h1D_calc->GetNbinsX()+1; ++iBin)
                        {
                            h1D_calc->SetBinContent(iBin, hTmp_Cumulative->GetBinContent(iBin));
                            h1D_calc->SetBinError(iBin, hTmp_Cumulative->GetBinError(iBin));
                        }

                        c->SetName(Form("cnv_%s",tmpH1D_name_calc.c_str()));
                        c->cd();
                        h1D_calc->Draw("e");
                        h1D_calc->Write("",TObject::kOverwrite);
                        c->Write("",TObject::kOverwrite);
                        c->Clear();

                        // integral of dphi : CDF (dphi) - with variable binning
                        tmpH1D_name_calc = replaceAll(tmpH1D_name, "dphi_", "dphi_CDF_rebin_");
                        tmpHistName_calc = Form("h1D_%s", tmpH1D_name_calc.c_str());

                        std::vector<double> tmpBins = getTH1xBins(h1D_calc);

                        tmpBins = rebinDphiBins(tmpBins);

                        int NtmpBins = tmpBins.size();
                        double binsRebin[NtmpBins];
                        std::copy(tmpBins.begin(), tmpBins.end(), binsRebin);

                        h1D_calc = (TH1D*)h1D_calc->Rebin(NtmpBins-1, tmpHistName_calc.c_str(), binsRebin); //creates a new variable bin size histogram hnew
                        double firstBinWidth = hTmp->GetBinWidth(1);
                        h1D_calc->Scale(firstBinWidth, "width");

                        c->SetName(Form("cnv_%s",tmpH1D_name_calc.c_str()));
                        c->cd();
                        h1D_calc->Draw("e");
                        h1D_calc->Write("",TObject::kOverwrite);
                        c->Write("",TObject::kOverwrite);
                        c->Clear();
                    }
                    // fit dphi correlation
                    if (tmpObservable.find("dphi") == 0)
                    {
                        std::cout << "fit dphi correlation : " << tmpH1D_name.c_str() << std::endl;

                        // apply fit to dphi, try to extract information about the width of the distribution
                        // fit functions used for "dphi" and "dphi_normJZ" histograms are different.
                        std::string tf1Formula = "pol0";
                        int parIndex = 0;
                        std::vector<double> parInit = {};
                        if (tmpObservable.compare("dphi") == 0 || tmpObservable.compare("dphi_rebin") == 0) {
                            tf1Formula = "[0]+[1]*exp((x-TMath::Pi())/[2])";
                            parIndex = 2;
                            parInit = {0.01, 0.5, 0.3};
                        }
                        else if (tmpObservable.compare("dphi_normJZ") == 0 || tmpObservable.compare("dphi_rebin_normJZ") == 0) {
                            tf1Formula = "exp(-(TMath::Pi()-x)/[0])/([0]*(1-exp(-TMath::Pi()/[0])))";
                            parIndex = 0;
                            parInit = {0.25};
                        }
                        int nParams = parInit.size();
                        double paramsInit[nParams];
                        std::copy(parInit.begin(), parInit.end(), paramsInit);

                        // do fit to the histogram.
                        double xmin = hTmp->GetBinCenter(hTmp->GetNbinsX()-3);
                        f1_calc = new TF1(Form("f1_%s", tmpH1D_name.c_str()), tf1Formula.c_str(), xmin, PI);
                        f1_calc->SetParameters(paramsInit);
                        f1_calc->SetLineColor(kRed);
                        hTmp->Fit(f1_calc->GetName(), "QREM0");
                        hTmp->GetFunction(f1_calc->GetName())->SetBit(TF1::kNotDraw);
                        f1_calc = hTmp->GetFunction(f1_calc->GetName());
                        f1_calc->Write("", TObject::kOverwrite);

                        double val = f1_calc->GetParameter(parIndex);
                        double err = f1_calc->GetParError(parIndex);
                        std::cout << "val = " << val << std::endl;
                        std::cout << "err = " << err << std::endl;
                    }
                }
            }
            std::cout<<"##########"<<std::endl;
        }
    }
    std::cout<<"##########"<<std::endl;
    // width(dphi) in Z pt bins
    std::cout << "width(dphi) in Z pt bins" << std::endl;
    const int nPtBins_dphi_width = nPtBins_rjz;
    const int offset_PtBins_dphi_width = offset_ptBins_rjz;
    for (int iHiBin=0; iHiBin<nBins_hiBin; ++iHiBin) {
        std::cout<<"##########"<<std::endl;
        std::cout<<"##### iHibin = " << iHiBin <<std::endl;
        for(int iHist = 0; iHist<nCorrHist; ++iHist)
        {
            std::string correlation = correlationHistNames.at(iHist).c_str();

            // look for dphi histograms only
            if (correlation.find("dphi") == std::string::npos)  continue;

            for (int jCorr = 0; jCorr < CORR::kN_CORRFNC; ++jCorr) {

                if (jCorr == CORR::kBKG && !isHI)  continue;      // no jet background for non-HI

                // get the histogram template from R_JZ histograms
                std::string tmpHistName_template = Form("h1D_%s_ptBinAll_hiBin%d", correlationHistNames_ptBinAll.at(ZJET::kRJZ).c_str() ,iHiBin);
                std::string tmpH1D_name_calc = Form("%s_width_ptBinAll_hiBin%d_%s", correlation.c_str(), iHiBin, CORR::CORR_JET_LABELS[jCorr].c_str());
                std::string tmpHistName_calc = Form("h1D_%s", tmpH1D_name_calc.c_str());
                std::cout << "## " << tmpHistName_calc.c_str() << std::endl;

                tmpHistName_template = Form("%s/%s", collisionName, tmpHistName_template.c_str());
                h1D_calc = (TH1D*)corrHists_ptBinAll[ZJET::kRJZ][iHiBin][0]->Clone(tmpHistName_calc.c_str());
                h1D_calc->Reset("ICES");   // the histogram above is cloned just to copy axis properties
                h1D_calc->SetYTitle("#sigma (#Delta#phi_{JZ})");

                for(int i=0; i<nPtBins_dphi_width; ++i){

                    int iPt = i + offset_PtBins_dphi_width;

                    std::string tmpName = Form("%s_ptBin%d_hiBin%d_%s_final_norm", correlation.c_str(), iPt, iHiBin, CORR::CORR_JET_LABELS[jCorr].c_str());
                    std::string tmpHistName = Form("h1D_%s", tmpName.c_str());

                    // search through histograms that are created by summing dielectron and dimuon channels
                    bool found = false;
                    for (int iHist2 = 0; iHist2 < nTH1; ++iHist2)
                    {
                        if (jCorr == CORR::kRAW && tmpHistName.compare(hNorm[iHist2]->GetName()) == 0) {
                            hTmp = (TH1D*)hNorm[iHist2]->Clone();
                            found = true;
                        }
                        else if (jCorr == CORR::kBKG && isHI && tmpHistName.compare(hBKGNorm[iHist2]->GetName()) == 0) {
                            hTmp = (TH1D*)hBKGNorm[iHist2]->Clone();
                            found = true;
                        }
                        else if (jCorr == CORR::kSIG && tmpHistName.compare(hSIGNorm[iHist2]->GetName()) == 0) {
                            hTmp = (TH1D*)hSIGNorm[iHist2]->Clone();
                            found = true;
                        }
                    }
                    if (!found)  continue;

                    // apply fit to dphi, try to extract information about the width of the distribution
                    // fit functions used for "dphi" and "dphi_normJZ" histograms are different.
                    std::string tf1Formula = "pol0";
                    int parIndex = 0;
                    std::vector<double> parInit = {};
                    if (correlation.compare("dphi") == 0 || correlation.compare("dphi_rebin") == 0) {
                        tf1Formula = "[0]*exp(-(TMath::Pi()-x)*[1])+[2]";
                        parIndex = 1;
                        parInit = {0, 0.01, 0};
                    }
                    else if (correlation.compare("dphi_normJZ") == 0 || correlation.compare("dphi_rebin_normJZ") == 0) {
                        tf1Formula = "exp(-(TMath::Pi()-x)/[0])/([0]*(1-exp(-TMath::Pi()/[0])))";
                        parIndex = 0;
                        parInit = {0.01};
                    }
                    int nParams = parInit.size();
                    double paramsInit[nParams];
                    std::copy(parInit.begin(), parInit.end(), paramsInit);

                    // do fit to the histogram.
                    f1_calc = new TF1(Form("f1_%s", tmpName.c_str()), tf1Formula.c_str(), 2*PI/3, PI);
                    f1_calc->SetParameters(paramsInit);
                    f1_calc->SetLineColor(kRed);
                    hTmp->Fit(f1_calc->GetName(), "QREM");
                    //corrHists[iHist][iPt][iHiBin].h1D_final_norm[iCorr][jCorr]->GetFunction(f1_calc->GetName())->SetBit(TF1::kNotDraw);
                    f1_calc = hTmp->GetFunction(f1_calc->GetName());
                    f1_calc->Write(f1_calc->GetName(), TObject::kOverwrite);

                    double val = f1_calc->GetParameter(parIndex);
                    double err = f1_calc->GetParError(parIndex);
                    std::cout << "val = " << val << std::endl;
                    std::cout << "err = " << err << std::endl;

                    h1D_calc->SetBinContent(i+1, val);
                    h1D_calc->SetBinError(i+1, err);
                }

                c->SetName(Form("cnv_%s",tmpH1D_name_calc.c_str()));
                c->cd();
                h1D_calc->Draw("e");
                h1D_calc->Write("",TObject::kOverwrite);
                c->Write("",TObject::kOverwrite);
                c->Clear();
            }
        }
    }
    std::cout<<"##### HISTOGRAMS WHICH ARE CALCULATED FROM HISTOGRAMS THAT ARE \"FILLED\" - END #####"<<std::endl;
    // HISTOGRAMS WHICH ARE MODIFICATIONS OF HISTOGRAMS THAT ARE "FILLED" - END

    configTree->Write("",TObject::kOverwrite);

    output->Write("",TObject::kOverwrite);
    inputEle->Close();
    inputMu->Close();

    output->Close();

    std::cout<<"zJetHistogramSum() - END"<<std::endl;
}

int main(int argc, char** argv)
{
    if (argc == 5) {
        zJetHistogramSum(argv[1], argv[2], argv[3], argv[4]);
        return 0;
    }
    else if (argc == 4) {
        zJetHistogramSum(argv[1], argv[2], argv[3]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./zJetHistogramSum.exe <configFile> <inputFileDiEle> <inputFileDiMu> <outputFile>"
                << std::endl;
        return 1;
    }
}
