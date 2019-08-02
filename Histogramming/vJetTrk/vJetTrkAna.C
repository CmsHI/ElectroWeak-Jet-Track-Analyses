/*
 * macro to fill histograms for v+jet+track analysis
 */

#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TGraph.h>
#include <TGraphAsymmErrors.h>
#include <TObjArray.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TLine.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TMath.h>
#include <TLorentzVector.h>

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>

#include "../../CorrelationTuple/EventMatcher.h"
#include "../../TreeHeaders/CutConfigurationTree.h"
#include "../../TreeHeaders/ggHiNtuplizerTree.h"
#include "../../TreeHeaders/hiEvtTree.h"
#include "../../TreeHeaders/jetSkimTree.h"
#include "../../TreeHeaders/trackSkimTree.h"
#include "../../TreeHeaders/eventSkimTree.h"
#include "../../TreeHeaders/mixEventSkimTree.h"
#include "../../Utilities/interface/ArgumentParser.h"
#include "../../Utilities/interface/ConfigurationParser.h"
#include "../../Utilities/interface/GraphicsConfigurationParser.h"
#include "../../Utilities/interface/InputConfigurationParser.h"
#include "../../Utilities/interface/HiForestInfoController.h"
#include "../../Utilities/eventUtil.h"
#include "../../Utilities/styleUtil.h"
#include "../../Utilities/th1Util.h"
#include "../../Utilities/fileUtil.h"
#include "../../Utilities/physicsUtil.h"
#include "../../Utilities/vJetTrkUtil.h"
#include "../../Corrections/tracks/2018PbPb_TrackingEfficiencies_Prelim/trackingEfficiency2018PbPb.h"
#include "../../Corrections/tracks/2015/getTrkCorr.h"

///// global variables
/// configuration variables

// input for TTree
std::string treePath;
std::string sampleType;

std::string treePathJetSkim;

// event cuts/weights
int doEventWeight;
std::vector<std::vector<float>> pthatWeights;

// RECO object cuts
bool excludeHI18HEMfailure;

int applyTrkWeights;
bool rotateEvtPlane;

int nPthatWeights;

/// configuration variables - END

std::vector<std::string> argOptions;
///// global variables - END

int readConfiguration(std::string configFile, std::string inputFile);
void printConfiguration();
std::vector<double> parseRanges(std::string rangesStr, int indexMinMax);
std::vector<double> parseRangesMin(std::string rangesStr);
std::vector<double> parseRangesMax(std::string rangesStr);
void vJetTrkAna(std::string configFile, std::string inputFile, std::string outputFile = "vJetTrkAna.root");
void vJetTrkAnaNoLoop(std::string configFile, std::string inputFile, std::string outputFile = "vJetTrkAna.root");

void vJetTrkAna(std::string configFile, std::string inputFile, std::string outputFile)
{
    std::cout<<"running vJetTrkAna()"<<std::endl;
    std::cout<<"configFile  = "<< configFile.c_str() <<std::endl;
    std::cout<<"inputFile   = "<< inputFile.c_str()  <<std::endl;
    std::cout<<"outputFile  = "<< outputFile.c_str() <<std::endl;

    if (readConfiguration(configFile, inputFile) != 0)  return;
    printConfiguration();

    bool isMC = isMCsample(sampleType);
    bool isPbPb = isPbPbsample(sampleType);
    bool isPP = isPPsample(sampleType);

    bool isPbPb15 = isPbPb && (sampleType.find("2015") != std::string::npos);
    bool isPbPb18 = isPbPb && (sampleType.find("2018") != std::string::npos);
    bool isPP15 = isPP && (sampleType.find("2015") != std::string::npos);
    bool isPP17 = isPP && (sampleType.find("2017") != std::string::npos);

    std::cout << "isMC = " << isMC << std::endl;
    std::cout << "isPbPb15 = " << isPbPb15 << std::endl;
    std::cout << "isPbPb18 = " << isPbPb18 << std::endl;
    std::cout << "isPP15 = " << isPP15 << std::endl;
    std::cout << "isPP17 = " << isPP17 << std::endl;

    int collisionType = -1;
    if (isPbPb15) {
        collisionType = COLL::TYPE::kHI;
    }
    else if (isPbPb18) {
        collisionType = COLL::TYPE::kHI2018;
    }
    else if (isPP) {
        collisionType = COLL::TYPE::kPP;
    }

    std::string vType = (ArgumentParser::optionExists("--vType", argOptions)) ?
            ArgumentParser::ParseOptionInputSingle("--vType", argOptions).c_str() : "pho";
    std::string vRG = (ArgumentParser::optionExists("--vRG", argOptions)) ?
            ArgumentParser::ParseOptionInputSingle("--vRG", argOptions).c_str() : "r";

    std::string vPtRangesStr = (ArgumentParser::optionExists("--vPts", argOptions)) ?
            ArgumentParser::ParseOptionInputSingle("--vPts", argOptions).c_str() : "40:-1,60:-1";
    std::vector<double> vPtsMin = parseRangesMin(vPtRangesStr);
    std::vector<double> vPtsMax = parseRangesMax(vPtRangesStr);
    int nVPts = vPtsMin.size();

    double vEtaMin = (ArgumentParser::optionExists("--vEtaMin", argOptions)) ?
            std::atof(ArgumentParser::ParseOptionInputSingle("--vEtaMin", argOptions).c_str()) : 0;
    double vEtaMax = (ArgumentParser::optionExists("--vEtaMax", argOptions)) ?
                std::atof(ArgumentParser::ParseOptionInputSingle("--vEtaMax", argOptions).c_str()) : 1.44;

    std::string jetRG = (ArgumentParser::optionExists("--jetRG", argOptions)) ?
            ArgumentParser::ParseOptionInputSingle("--jetRG", argOptions).c_str() : "r";

    std::string trkRG = (ArgumentParser::optionExists("--trkRG", argOptions)) ?
            ArgumentParser::ParseOptionInputSingle("--trkRG", argOptions).c_str() : "r";

    std::string trkRBS = (ArgumentParser::optionExists("--trkRBS", argOptions)) ?
            ArgumentParser::ParseOptionInputSingle("--trkRBS", argOptions).c_str() : "r";

    std::string trkPtRangesStr = (ArgumentParser::optionExists("--trkPts", argOptions)) ?
            ArgumentParser::ParseOptionInputSingle("--trkPts", argOptions).c_str() : "1:2,2:3,3:4,4:8";
    std::vector<double> trkPtsMin = parseRangesMin(trkPtRangesStr);
    std::vector<double> trkPtsMax = parseRangesMax(trkPtRangesStr);
    int nTrkPts = trkPtsMin.size();

    double trkEtaMin = (ArgumentParser::optionExists("--trkEtaMin", argOptions)) ?
                std::atof(ArgumentParser::ParseOptionInputSingle("--trkEtaMin", argOptions).c_str()) : 0;
    double trkEtaMax = (ArgumentParser::optionExists("--trkEtaMax", argOptions)) ?
                    std::atof(ArgumentParser::ParseOptionInputSingle("--trkEtaMax", argOptions).c_str()) : 2.4;

    double vTrkDphiMin = (ArgumentParser::optionExists("--vTrkDphiMin", argOptions)) ?
                std::atof(ArgumentParser::ParseOptionInputSingle("--vTrkDphiMin", argOptions).c_str()) : 7 / 8;
    double vTrkDphiMax = (ArgumentParser::optionExists("--vTrkDphiMax", argOptions)) ?
                    std::atof(ArgumentParser::ParseOptionInputSingle("--vTrkDphiMax", argOptions).c_str()) : 1.5;

    std::string centRangesStr = (ArgumentParser::optionExists("--cents", argOptions)) ?
            ArgumentParser::ParseOptionInputSingle("--cents", argOptions).c_str() : "0:30,30:100";
    std::vector<double> centsMinTmp = parseRangesMin(centRangesStr);
    std::vector<double> centsMaxTmp = parseRangesMax(centRangesStr);
    std::vector<int> centsMin(centsMinTmp.begin(), centsMinTmp.end());
    std::vector<int> centsMax(centsMaxTmp.begin(), centsMaxTmp.end());
    int nCents = centsMin.size();

    int maxNVtx = (ArgumentParser::optionExists("--maxNVtx", argOptions)) ?
            std::atoi(ArgumentParser::ParseOptionInputSingle("--maxNVtx", argOptions).c_str()) : 0;
    int minNVtx = (ArgumentParser::optionExists("--minNVtx", argOptions)) ?
            std::atoi(ArgumentParser::ParseOptionInputSingle("--minNVtx", argOptions).c_str()) : -1;

    std::cout << "vType = " << vType << std::endl;
    std::cout << "vRG = " << vRG << std::endl;
    std::cout << "nVPts = " << nVPts << std::endl;
    for (int i = 0; i < nVPts; ++i) {
        std::cout << Form("vPts[%d] = [%f, %f)", i, vPtsMin[i], vPtsMax[i]) << std::endl;
    }
    std::cout << "vEtaMin = " << vEtaMin << std::endl;
    std::cout << "vEtaMax = " << vEtaMax << std::endl;

    std::cout << "jetRG = " << jetRG << std::endl;

    std::cout << "trkRG = " << trkRG << std::endl;
    std::cout << "trkRBS = " << trkRBS << std::endl;
    std::cout << "nTrkPts = " << nTrkPts << std::endl;
    for (int i = 0; i < nTrkPts; ++i) {
        std::cout << Form("trkPts[%d] = [%f, %f)", i, trkPtsMin[i], trkPtsMax[i]) << std::endl;
    }
    std::cout << "trkEtaMin = " << trkEtaMin << std::endl;
    std::cout << "trkEtaMax = " << trkEtaMax << std::endl;
    std::cout << "vTrkDphiMin (as fraction of PI) = " << vTrkDphiMin << std::endl;
    std::cout << "vTrkDphiMax (as fraction of PI) = " << vTrkDphiMax << std::endl;

    std::cout << "nCents = " << nCents << std::endl;
    for (int i = 0; i < nCents; ++i) {
        std::cout << Form("cents[%d] = [%d, %d)", i, centsMin[i], centsMax[i]) << std::endl;
    }

    std::cout << "maxNVtx = " << maxNVtx << std::endl;
    std::cout << "minNVtx = " << minNVtx << std::endl;

    std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.c_str());
    std::cout<<"input ROOT files : num = "<<inputFiles.size()<< std::endl;
    std::cout<<"#####"<< std::endl;
    for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
        std::cout<<(*it).c_str()<< std::endl;
    }
    std::cout<<"##### END #####"<< std::endl;

    bool vIsPho = (toLowerCase(vType).find("pho") == 0);
    bool vIsZmm = (toLowerCase(vType).find("zmm") == 0);
    bool vIsZee = (toLowerCase(vType).find("zee") == 0);
    bool vIsZ = vIsZmm || vIsZee;

    bool isRecoV = isRecoObj(vRG);
    bool isRecoJet = isRecoObj(jetRG);
    bool isRecoTrk = isRecoObj(trkRG);

    bool isBkgTrk = isBkgObj(trkRBS);
    bool isSigTrk = isSigObj(trkRBS);

    bool noTrkWeights = (applyTrkWeights == 0);
    bool redoTrkWeights = (applyTrkWeights == 2);

    TrkEff2018PbPb trkEff2018 =  TrkEff2018PbPb("general", false, "Corrections/tracks/2018PbPb_TrackingEfficiencies_Prelim/");

    TrkCorr* trkCorr2015 = 0;
    if (redoTrkWeights) {
        if (isPP15) {
            trkCorr2015 = new TrkCorr("Corrections/tracks/2015/TrkCorr_July22_Iterative_pp_eta2p4/");
        }
        else if (isPbPb15) {
            trkCorr2015 = new TrkCorr("Corrections/tracks/2015/TrkCorr_Jun7_Iterative_PbPb_etaLT2p4/");
        }
    }

    TFile* output = TFile::Open(outputFile.c_str(),"RECREATE");
    output->cd();

    // initialize objects
    TH1::SetDefaultSumw2();

    std::vector<TH2D*> vec_h2D;
    std::vector<TH1D*> vec_h_num;
    std::vector<TH1D*> vec_h_denom;

    TH1D* h_vPt[nCents];
    TH1D* h_vEta[nCents][nVPts];
    TH1D* h_vPhi[nCents][nVPts];
    TH1D* h_vM_os[nCents][nVPts];
    TH1D* h_vM_ss[nCents][nVPts];

    // trig eff
    TH1D* h_trig_num_vPt[nCents];
    TH1D* h_trig_denom_vPt[nCents];

    // reco eff
    TH1D* h_reco_num_vPt[nCents];
    TH1D* h_reco_denom_vPt[nCents];
    TH1D* h_reco_num_vEta[nCents][nVPts];
    TH1D* h_reco_denom_vEta[nCents][nVPts];
    TH1D* h_reco_num_cent[nVPts];
    TH1D* h_reco_denom_cent[nVPts];

    // reco/gen diff
    TH2D* h2_rgVPt_ratio_vs_vPt[nCents];
    TH2D* h2_rgVPhi_diff_vs_vPt[nCents];

    // event observables
    TH1D* h_cent[nVPts];
    TH1D* h_vtxz[nCents][nVPts];
    TH1D* h_dphi_evtPlane_V[nCents][nVPts];
    TH2D* h2_hiHF_vs_vPt[nCents];
    TH2D* h2_rho_vs_vPt[nCents];
    TH2D* h2_PFHFtotE_vs_vPt[nCents];

    TH1D* h_trkPt[nCents][nVPts];
    TH1D* h_trkEta[nCents][nVPts][nTrkPts];
    TH1D* h_trkPhi[nCents][nVPts][nTrkPts];
    TH1D* h_dphi_evtPlane_trk[nCents][nVPts][nTrkPts];

    TH1D* h_dphi[nCents][nVPts][nTrkPts];
    TH1D* h_deta[nCents][nVPts][nTrkPts];
    TH1D* h_dR[nCents][nVPts][nTrkPts];
    TH1D* h_dRneg[nCents][nVPts][nTrkPts];
    TH1D* h_zh[nCents][nVPts][nTrkPts];
    TH1D* h_zh_T[nCents][nVPts][nTrkPts];
    TH1D* h_xivh[nCents][nVPts][nTrkPts];

    TH2D* h2_trkPhi_vs_trkEta[nCents][nVPts][nTrkPts];
    TH2D* h2_dphi_vs_deta[nCents][nVPts][nTrkPts];
    TH2D* h2_dphi_vs_trkPt[nCents][nVPts];
    TH2D* h2_dphi_vs_trkEta[nCents][nVPts][nTrkPts];

    int nBinsX_vPt = 30;
    int nBinsX_trkPt = 60;
    int nBinsX_dphi = 20;
    int nBinsX_eta = 25;

    double xMax_vPt = 150;
    double xMax_trkPt = 30;
    double xMax_phi = TMath::Pi()+1e-12;
    double xMax_eta = 2.5;
    for (int i = 0; i < nCents; ++i) {

        std::string label_cent = Form("cent%d_%d", centsMin[i], centsMax[i]);
        std::string text_range_cent = Form("Cent:%d-%d %%", centsMin[i], centsMax[i]);
        if (!isPbPb) {
            label_cent = "cent0_100";
            text_range_cent = "pp";
        }
        else if (centsMax[i] < centsMin[i]) {
            label_cent = Form("cent%d_100", centsMin[i]);
            text_range_cent = Form("Cent:%d-100 %%", centsMin[i]);
        }

        std::string text_V = vIsPho ? "#gamma" : "Z";
        std::string text_vPt = Form("p^{%s}_{T}", text_V.c_str());
        std::string text_vEta = Form("#eta^{%s}", text_V.c_str());
        std::string text_vEtaAbs = Form("|%s|", text_vEta.c_str());
        std::string text_vPhi = Form("#phi^{%s}", text_V.c_str());
        std::string text_l = "";
        if (vIsZ) {
            text_l = vIsZmm ? "#mu" : "e^{+}";
        }
        std::string text_ll_os = Form("%s^{+}%s^{-}", text_l.c_str(), text_l.c_str());
        std::string text_ll_ss = Form("%s^{#pm}%s^{#pm}", text_l.c_str(), text_l.c_str());
        std::string text_vM_os = Form("M^{%s}", text_ll_os.c_str());
        std::string text_vM_ss = Form("M^{%s}", text_ll_ss.c_str());

        std::string text_range_vEta = Form("%.1f < %s < %.1f", vEtaMin, text_vEtaAbs.c_str(), vEtaMax);
        if (vEtaMax < vEtaMin) {
            text_range_vEta = Form("%.1f < %s", vEtaMin, text_vEtaAbs.c_str());
        }

        std::string name_h_vPt = Form("h_vPt_%s", label_cent.c_str());
        std::string title_h_vPt = Form("%s, %s;%s;", text_range_vEta.c_str(),
                                                     text_range_cent.c_str(),
                                                     text_vPt.c_str());

        h_vPt[i] = 0;
        h_vPt[i] = new TH1D(name_h_vPt.c_str(), title_h_vPt.c_str(), nBinsX_vPt, 0, xMax_vPt);

        std::string name_h_trig_num_vPt = Form("h_trig_num_vPt_%s", label_cent.c_str());
        h_trig_num_vPt[i] = 0;
        h_trig_num_vPt[i] = (TH1D*)h_vPt[i]->Clone(name_h_trig_num_vPt.c_str());
        vec_h_num.push_back(h_trig_num_vPt[i]);

        std::string name_h_trig_denom_vPt = Form("h_trig_denom_vPt_%s", label_cent.c_str());
        h_trig_denom_vPt[i] = 0;
        h_trig_denom_vPt[i] = (TH1D*)h_vPt[i]->Clone(name_h_trig_denom_vPt.c_str());
        vec_h_denom.push_back(h_trig_denom_vPt[i]);

        std::string name_h_reco_num_vPt = Form("h_reco_num_vPt_%s", label_cent.c_str());
        h_reco_num_vPt[i] = 0;
        h_reco_num_vPt[i] = (TH1D*)h_vPt[i]->Clone(name_h_reco_num_vPt.c_str());
        h_reco_num_vPt[i]->SetXTitle(Form("gen %s", h_vPt[i]->GetXaxis()->GetTitle()));
        vec_h_num.push_back(h_reco_num_vPt[i]);

        std::string name_h_reco_denom_vPt = Form("h_reco_denom_vPt_%s", label_cent.c_str());
        h_reco_denom_vPt[i] = 0;
        h_reco_denom_vPt[i] = (TH1D*)h_reco_num_vPt[i]->Clone(name_h_reco_denom_vPt.c_str());
        vec_h_denom.push_back(h_reco_denom_vPt[i]);

        std::string name_h2_rgVPt_ratio_vs_vPt = Form("h2_rgVPt_ratio_vs_vPt_%s", label_cent.c_str());
        std::string title_h2_rgVPt_ratio_vs_vPt = Form("%s, %s;gen %s;p_{T}^{reco} / p_{T}^{gen}", text_range_vEta.c_str(),
                                                     text_range_cent.c_str(),
                                                     text_vPt.c_str());

        h2_rgVPt_ratio_vs_vPt[i] = 0;
        h2_rgVPt_ratio_vs_vPt[i] = new TH2D(name_h2_rgVPt_ratio_vs_vPt.c_str(), title_h2_rgVPt_ratio_vs_vPt.c_str(),
                                                     nBinsX_vPt, 0, xMax_vPt, 60, 0.4, 1.6);
        vec_h2D.push_back(h2_rgVPt_ratio_vs_vPt[i]);

        std::string name_h2_rgVPhi_diff_vs_vPt = Form("h2_rgVPhi_diff_vs_vPt_%s", label_cent.c_str());
        std::string title_h2_rgVPhi_diff_vs_vPt = Form("%s, %s;gen %s;#phi^{reco} - #phi^{gen}", text_range_vEta.c_str(),
                                                     text_range_cent.c_str(),
                                                     text_vPt.c_str());

        h2_rgVPhi_diff_vs_vPt[i] = 0;
        h2_rgVPhi_diff_vs_vPt[i] = new TH2D(name_h2_rgVPhi_diff_vs_vPt.c_str(), title_h2_rgVPhi_diff_vs_vPt.c_str(),
                                                     nBinsX_vPt, 0, xMax_vPt, 80, -0.4, 0.4);
        vec_h2D.push_back(h2_rgVPhi_diff_vs_vPt[i]);

        std::string name_h2_hiHF_vs_vPt = Form("h2_hiHF_vs_vPt_%s", label_cent.c_str());
        std::string title_h2_hiHF_vs_vPt = Form("%s, %s;%s;hiHF", text_range_vEta.c_str(),
                                                     text_range_cent.c_str(),
                                                     text_vPt.c_str());

        h2_hiHF_vs_vPt[i] = 0;
        h2_hiHF_vs_vPt[i] = new TH2D(name_h2_hiHF_vs_vPt.c_str(), title_h2_hiHF_vs_vPt.c_str(), nBinsX_vPt, 0, xMax_vPt, 2000, 0, 8000);
        vec_h2D.push_back(h2_hiHF_vs_vPt[i]);

        std::string name_h2_rho_vs_vPt = Form("h2_rho_vs_vPt_%s", label_cent.c_str());
        std::string title_h2_rho_vs_vPt = Form("%s, %s;%s;rho", text_range_vEta.c_str(),
                                                     text_range_cent.c_str(),
                                                     text_vPt.c_str());

        h2_rho_vs_vPt[i] = 0;
        h2_rho_vs_vPt[i] = new TH2D(name_h2_rho_vs_vPt.c_str(), title_h2_rho_vs_vPt.c_str(), nBinsX_vPt, 0, xMax_vPt, 2000, 0, 400);
        vec_h2D.push_back(h2_rho_vs_vPt[i]);

        std::string name_h2_PFHFtotE_vs_vPt = Form("h2_PFHFtotE_vs_vPt_%s", label_cent.c_str());
        std::string title_h2_PFHFtotE_vs_vPt = Form("%s, %s;%s;total energy of PF HF towers", text_range_vEta.c_str(),
                                                     text_range_cent.c_str(),
                                                     text_vPt.c_str());

        h2_PFHFtotE_vs_vPt[i] = 0;
        h2_PFHFtotE_vs_vPt[i] = new TH2D(name_h2_PFHFtotE_vs_vPt.c_str(), title_h2_PFHFtotE_vs_vPt.c_str(),
                                         nBinsX_vPt, 0, xMax_vPt, 2000, 0, 200000);
        vec_h2D.push_back(h2_PFHFtotE_vs_vPt[i]);
        for (int j = 0; j < nVPts; ++j) {

            std::string label_vPt = Form("vPt%d_%d", (int)(vPtsMin[j]), (int)(vPtsMax[j]));
            std::string text_range_vPt = Form("%d < %s < %d", (int)(vPtsMin[j]), text_vPt.c_str(), (int)(vPtsMax[j]));
            if (vPtsMax[j] < vPtsMin[j]) {
                label_vPt = Form("vPt%d_0", (int)(vPtsMin[j]));
                text_range_vPt = Form("%s > %d", text_vPt.c_str(), (int)(vPtsMin[j]));
            }

            std::string name_h_suffix = Form("%s_%s", label_vPt.c_str(), label_cent.c_str());
            std::string title_h_suffix = Form("%s, %s", text_range_vPt.c_str(), text_range_cent.c_str());

            std::string name_h_vEta = Form("h_vEta_%s", name_h_suffix.c_str());
            std::string title_h_vEta = Form("%s;%s;", title_h_suffix.c_str(), text_vEta.c_str());
            h_vEta[i][j] = 0;
            h_vEta[i][j] = new TH1D(name_h_vEta.c_str(), title_h_vEta.c_str(), nBinsX_eta, -1*xMax_eta, xMax_eta);

            std::string name_h_reco_num_vEta = Form("h_reco_num_vEta_%s", name_h_suffix.c_str());
            h_reco_num_vEta[i][j] = 0;
            h_reco_num_vEta[i][j] = (TH1D*)h_vEta[i][j]->Clone(name_h_reco_num_vEta.c_str());
            h_reco_num_vEta[i][j]->SetXTitle(Form("gen %s", h_vEta[i][j]->GetXaxis()->GetTitle()));
            vec_h_num.push_back(h_reco_num_vEta[i][j]);

            std::string name_h_reco_denom_vEta = Form("h_reco_denom_vEta_%s", name_h_suffix.c_str());
            h_reco_denom_vEta[i][j] = 0;
            h_reco_denom_vEta[i][j] = (TH1D*)h_reco_num_vEta[i][j]->Clone(name_h_reco_denom_vEta.c_str());
            vec_h_denom.push_back(h_reco_denom_vEta[i][j]);

            title_h_suffix = Form("%s, %s, %s", text_range_vPt.c_str(), text_range_vEta.c_str(), text_range_cent.c_str());

            std::string name_h_vPhi = Form("h_vPhi_%s", name_h_suffix.c_str());
            std::string title_h_vPhi = Form("%s;%s;", title_h_suffix.c_str(), text_vPhi.c_str());
            h_vPhi[i][j] = 0;
            h_vPhi[i][j] = new TH1D(name_h_vPhi.c_str(), title_h_vPhi.c_str(), nBinsX_dphi, -1*xMax_phi, xMax_phi);

            std::string name_h_vM_os = Form("h_vM_os_%s", name_h_suffix.c_str());
            std::string title_h_vM_os = Form("%s;%s;", title_h_suffix.c_str(), text_vM_os.c_str());
            h_vM_os[i][j] = 0;
            h_vM_os[i][j] = new TH1D(name_h_vM_os.c_str(), title_h_vM_os.c_str(), 30, 60, 120);

            std::string name_h_vM_ss = Form("h_vM_ss_%s", name_h_suffix.c_str());
            std::string title_h_vM_ss = Form("%s;%s;", title_h_suffix.c_str(), text_vM_ss.c_str());
            h_vM_ss[i][j] = 0;
            h_vM_ss[i][j] = new TH1D(name_h_vM_ss.c_str(), title_h_vM_ss.c_str(), 30, 60, 120);

            if (i == 0) {
                std::string name_h_cent = Form("h_halfHiBin_%s", label_vPt.c_str());
                std::string title_h_cent = Form("%s;Centrality (%%);", text_range_vPt.c_str());

                h_cent[j] = 0;
                h_cent[j] = new TH1D(name_h_cent.c_str(), title_h_cent.c_str(), 20, 0, 100);

                std::string name_h_reco_num_cent = Form("h_reco_num_halfHiBin_%s", label_vPt.c_str());
                h_reco_num_cent[j] = 0;
                h_reco_num_cent[j] = (TH1D*)h_cent[j]->Clone(name_h_reco_num_cent.c_str());
                vec_h_num.push_back(h_reco_num_cent[j]);

                std::string name_h_reco_denom_cent = Form("h_reco_denom_halfHiBin_%s", label_vPt.c_str());
                h_reco_denom_cent[j] = 0;
                h_reco_denom_cent[j] = (TH1D*)h_reco_num_cent[j]->Clone(name_h_reco_denom_cent.c_str());
                vec_h_denom.push_back(h_reco_denom_cent[j]);
            }

            std::string name_h_vtxz = Form("h_vtxz_%s", name_h_suffix.c_str());
            std::string title_h_vtxz = Form("%s;v_{z};", title_h_suffix.c_str());
            h_vtxz[i][j] = 0;
            h_vtxz[i][j] = new TH1D(name_h_vtxz.c_str(), title_h_vtxz.c_str(), 30, -15, 15);


            std::string name_h_dphi_evtPlane_V = Form("h_dphi_evtPlane_V_%s", name_h_suffix.c_str());
            std::string title_h_dphi_evtPlane_V = Form("%s;#Delta#phi_{evtPlane,%s};", title_h_suffix.c_str(), text_V.c_str());

            h_dphi_evtPlane_V[i][j] = 0;
            h_dphi_evtPlane_V[i][j] = new TH1D(name_h_dphi_evtPlane_V.c_str(), title_h_dphi_evtPlane_V.c_str(), nBinsX_dphi, 0, xMax_phi);

            std::string text_trk = "trk";
            std::string text_trkPt = Form("p^{%s}_{T}", text_trk.c_str());
            std::string text_trkEta = Form("#eta^{%s}", text_trk.c_str());
            std::string text_trkEtaAbs = Form("|%s|", text_trkEta.c_str());
            std::string text_trkPhi = Form("#phi^{%s}", text_trk.c_str());

            std::string text_dphi = Form("#Delta#phi_{%s%s}", text_trk.c_str(),
                                                                   text_V.c_str());
            std::string text_defn_dphi = Form("%s = |%s - %s|", text_dphi.c_str(),
                                                                text_trkPhi.c_str(),
                                                                text_vPhi.c_str());

            std::string text_dphineg = Form("#Delta#phi_{%s -%s}", text_trk.c_str(),
                                                                   text_V.c_str());
            std::string text_defn_dphineg = Form("%s = |%s - -%s|", text_dphineg.c_str(),
                                                                text_trkPhi.c_str(),
                                                                text_vPhi.c_str());

            std::string text_range_trkEta = Form("%.1f < %s < %.1f", trkEtaMin, text_trkEtaAbs.c_str(), trkEtaMax);
            if (trkEtaMax < trkEtaMin) {
                text_range_trkEta = Form("%.1f < %s", trkEtaMin, text_trkEtaAbs.c_str());
            }
            std::string text_range_dphi = Form("%.3f #pi < %s < %.3f #pi", vTrkDphiMin, text_dphi.c_str(), vTrkDphiMax);
            if (vTrkDphiMax > 1) {
                text_range_dphi = Form("%s > %.3f #pi", text_dphi.c_str(), vTrkDphiMin);
            }

            title_h_suffix = Form("%s, %s, %s, %s", text_range_vPt.c_str(),
                                                    text_range_trkEta.c_str(),
                                                    text_range_dphi.c_str(),
                                                    text_range_cent.c_str());

            std::string name_h_trkPt = Form("h_trkPt_%s", name_h_suffix.c_str());
            std::string title_h_trkPt = Form("%s;%s;", title_h_suffix.c_str(),
                                                       text_trkPt.c_str());
            h_trkPt[i][j] = 0;
            h_trkPt[i][j] = new TH1D(name_h_trkPt.c_str(), title_h_trkPt.c_str(), nBinsX_trkPt, 0, xMax_trkPt);

            std::string title_h_suffix_dphi = Form("%s, %s, %s", text_range_vPt.c_str(),
                                                    text_range_trkEta.c_str(),
                                                    text_range_cent.c_str());

            std::string name_h2_dphi_vs_trkPt = Form("h2_dphi_vs_trkPt_%s", name_h_suffix.c_str());
            std::string title_h2_dphi_vs_trkPt = Form("%s;%s;%s", title_h_suffix_dphi.c_str(),
                                                        text_trkPt.c_str(),
                                                        text_dphi.c_str());

            h2_dphi_vs_trkPt[i][j] = 0;
            h2_dphi_vs_trkPt[i][j] = new TH2D(name_h2_dphi_vs_trkPt.c_str(), title_h2_dphi_vs_trkPt.c_str(),
                                                      nBinsX_trkPt, 0, xMax_trkPt,
                                                      nBinsX_dphi, 0, xMax_phi);
            vec_h2D.push_back(h2_dphi_vs_trkPt[i][j]);

            for (int k = 0; k < nTrkPts; ++k) {

                double tmpTrkPt = -1;
                tmpTrkPt = trkPtsMin[k];
                std::string label_trkPtMin = Form("%d", (int)(tmpTrkPt));    // 5 --> "5"
                if (std::floor(tmpTrkPt) != tmpTrkPt) {   // 1.4 --> "1p4"
                    label_trkPtMin = Form("%dp%d", (int)(tmpTrkPt), ((int)(tmpTrkPt*10) % 10));
                }

                tmpTrkPt = trkPtsMax[k];
                std::string label_trkPtMax = Form("%d", (int)(tmpTrkPt));    // 5 --> "5"
                if (std::floor(tmpTrkPt) != tmpTrkPt) {   // 1.4 --> "1p4"
                    label_trkPtMax = Form("%dp%d", (int)(tmpTrkPt), ((int)(tmpTrkPt*10) % 10));
                }
                std::string label_trkPt = Form("trkPt%s_%s", label_trkPtMin.c_str(), label_trkPtMax.c_str());

                std::string text_trkPtMin = replaceAll(label_trkPtMin, "p", ".");
                std::string text_trkPtMax = replaceAll(label_trkPtMax, "p", ".");
                std::string text_range_trkPt = Form("%s < %s < %s", text_trkPtMin.c_str(), text_trkPt.c_str(), text_trkPtMax.c_str());
                if (trkPtsMax[k] < trkPtsMin[k]) {
                    label_trkPt = Form("trkPt%s_0", label_trkPtMin.c_str());
                    text_range_trkPt = Form("%s > %s", text_trkPt.c_str(), text_trkPtMin.c_str());
                }

                name_h_suffix = Form("%s_%s_%s", label_vPt.c_str(), label_trkPt.c_str(), label_cent.c_str());
                title_h_suffix = Form("%s, %s, %s, %s", text_range_vPt.c_str(),
                                                        text_range_trkPt.c_str(),
                                                        text_range_dphi.c_str(),
                                                        text_range_cent.c_str());

                std::string name_h_trkEta = Form("h_trkEta_%s", name_h_suffix.c_str());
                std::string title_h_trkEta = Form("%s;%s;", title_h_suffix.c_str(),
                                                            text_trkEta.c_str());
                h_trkEta[i][j][k] = 0;
                h_trkEta[i][j][k] = new TH1D(name_h_trkEta.c_str(), title_h_trkEta.c_str(), nBinsX_eta, -1*xMax_eta, xMax_eta);

                title_h_suffix = Form("%s, %s, %s, %s, %s", text_range_vPt.c_str(),
                                                            text_range_trkPt.c_str(),
                                                            text_range_trkEta.c_str(),
                                                            text_range_dphi.c_str(),
                                                            text_range_cent.c_str());

                std::string name_h_trkPhi = Form("h_trkPhi_%s", name_h_suffix.c_str());
                std::string title_h_trkPhi = Form("%s;%s;", title_h_suffix.c_str(),
                                                            text_trkPhi.c_str());
                h_trkPhi[i][j][k] = 0;
                h_trkPhi[i][j][k] = new TH1D(name_h_trkPhi.c_str(), title_h_trkPhi.c_str(), nBinsX_dphi, -1*xMax_phi, xMax_phi);

                std::string name_h_dphi_evtPlane_trk = Form("h_dphi_evtPlane_trk_%s", name_h_suffix.c_str());
                std::string title_h_dphi_evtPlane_trk = Form("%s;#Delta#phi_{evtPlane,%s};", title_h_suffix.c_str(),
                                                                                             text_trk.c_str());

                h_dphi_evtPlane_trk[i][j][k] = 0;
                h_dphi_evtPlane_trk[i][j][k] = new TH1D(name_h_dphi_evtPlane_trk.c_str(), title_h_dphi_evtPlane_trk.c_str(),
                                                                                            nBinsX_dphi, 0, xMax_phi);

                title_h_suffix_dphi = Form("%s, %s, %s, %s", text_range_vPt.c_str(),
                                                        text_range_trkPt.c_str(),
                                                        text_range_trkEta.c_str(),
                                                        text_range_cent.c_str());

                std::string name_h_dphi = Form("h_dphi_%s", name_h_suffix.c_str());
                std::string title_h_dphi = Form("%s;%s;", title_h_suffix_dphi.c_str(),
                                                          text_defn_dphi.c_str());
                h_dphi[i][j][k] = 0;
                h_dphi[i][j][k] = new TH1D(name_h_dphi.c_str(), title_h_dphi.c_str(), nBinsX_dphi, 0, xMax_phi);

                std::string text_deta = Form("#Delta#eta_{%s%s}", text_trk.c_str(),
                                                                  text_V.c_str());
                std::string text_defn_deta = Form("%s = |%s - %s|", text_deta.c_str(),
                                                                    text_trkEta.c_str(),
                                                                    text_vEta.c_str());

                title_h_suffix = Form("%s, %s, %s, %s, %s", text_range_vPt.c_str(),
                                                            text_range_trkPt.c_str(),
                                                            text_range_trkEta.c_str(),
                                                            text_range_dphi.c_str(),
                                                            text_range_cent.c_str());

                std::string name_h_deta = Form("h_deta_%s_%s_%s", label_vPt.c_str(), label_trkPt.c_str(), label_cent.c_str());
                std::string title_h_deta = Form("%s;%s;", title_h_suffix.c_str(),
                                                          text_defn_deta.c_str());
                h_deta[i][j][k] = 0;
                h_deta[i][j][k] = new TH1D(name_h_deta.c_str(), title_h_deta.c_str(), 20, 0, 4);

                std::string text_dR = Form("#DeltaR_{%s%s}", text_trk.c_str(),
                                                             text_V.c_str());
                std::string text_defn_dR = Form("%s = #sqrt{%s^2 + %s^2}", text_dR.c_str(),
                                                                           text_dphi.c_str(),
                                                                           text_deta.c_str());

                title_h_suffix = Form("%s, %s, %s, %s", text_range_vPt.c_str(),
                                                            text_range_trkPt.c_str(),
                                                            text_range_trkEta.c_str(),
                                                            text_range_cent.c_str());

                std::string name_h_dR = Form("h_dR_%s_%s_%s", label_vPt.c_str(), label_trkPt.c_str(), label_cent.c_str());
                std::string title_h_dR = Form("%s;%s;", title_h_suffix.c_str(),
                                                        text_dR.c_str());
                h_dR[i][j][k] = 0;
                h_dR[i][j][k] = new TH1D(name_h_dR.c_str(), title_h_dR.c_str(), 20, 0, xMax_phi);


                title_h_suffix = Form("%s, %s, %s, %s", text_range_vPt.c_str(),
                                                            text_range_trkPt.c_str(),
                                                            text_range_trkEta.c_str(),
                                                            text_range_cent.c_str());

                std::string text_dRneg = Form("#DeltaR_{%s -%s}", text_trk.c_str(),
                                                                  text_V.c_str());
                std::string text_defn_dRneg = Form("%s = #sqrt{%s^2 + %s^2}", text_dRneg.c_str(),
                                                                           text_dphineg.c_str(),
                                                                           text_deta.c_str());

                std::string name_h_dRneg = Form("h_dRneg_%s_%s_%s", label_vPt.c_str(), label_trkPt.c_str(), label_cent.c_str());
                std::string title_h_dRneg = Form("%s;%s;", title_h_suffix.c_str(),
                                                           text_dRneg.c_str());
                h_dRneg[i][j][k] = 0;
                h_dRneg[i][j][k] = new TH1D(name_h_dRneg.c_str(), title_h_dRneg.c_str(), 20, 0, 1);

                std::string text_zh = Form("z_{%s%s}", text_trk.c_str(),
                                                     text_V.c_str());
                std::string text_defn_zh = Form("%s = %s / %s", text_zh.c_str(),
                                                                text_trkPt.c_str(),
                                                                text_vPt.c_str());

                title_h_suffix = Form("%s, %s, %s, %s, %s", text_range_vPt.c_str(),
                                                            text_range_trkPt.c_str(),
                                                            text_range_trkEta.c_str(),
                                                            text_range_dphi.c_str(),
                                                            text_range_cent.c_str());

                std::string name_h_zh = Form("h_zh_%s_%s_%s", label_vPt.c_str(), label_trkPt.c_str(), label_cent.c_str());
                std::string title_h_zh = Form("%s;%s;", title_h_suffix.c_str(),
                                                        text_defn_zh.c_str());

                int nBins_zh = 10;
                std::vector<double> binsVecTmp = calcBinsLogScale(0.01, 1, nBins_zh);
                double binsArrTmp_zh[nBins_zh+1];
                std::copy(binsVecTmp.begin(), binsVecTmp.end(), binsArrTmp_zh);

                h_zh[i][j][k] = 0;
                h_zh[i][j][k] = new TH1D(name_h_zh.c_str(), title_h_zh.c_str(), nBins_zh, binsArrTmp_zh);

                std::string text_vPt_vec = Form("#bf{p}^{%s}_{T}", text_V.c_str());
                std::string text_trkPt_vec = Form("#bf{p}^{%s}_{T}", text_trk.c_str());
                std::string text_zh_T = Form("z^{%s%s}_{T}", text_trk.c_str(),
                                                             text_V.c_str());
                std::string text_defn_zh_T = Form("%s = ( -(%s #dot %s) / |%s|^{2} )",
                                                                text_zh_T.c_str(),
                                                                text_vPt_vec.c_str(),
                                                                text_trkPt_vec.c_str(),
                                                                text_vPt_vec.c_str());

                std::string name_h_zh_T = Form("h_zh_T_%s_%s_%s", label_vPt.c_str(), label_trkPt.c_str(), label_cent.c_str());
                std::string title_h_zh_T = Form("%s;%s;", title_h_suffix.c_str(),
                                                          text_defn_zh_T.c_str());

                h_zh_T[i][j][k] = 0;
                h_zh_T[i][j][k] = new TH1D(name_h_zh_T.c_str(), title_h_zh_T.c_str(), nBins_zh, binsArrTmp_zh);


                std::string text_xivh = Form("#xi^{%s, %s}_{T}", text_trk.c_str(),
                                                     text_V.c_str());
                std::string text_defn_xivh = Form("%s = ln ( -|%s|^{2} / (%s #dot %s) ) ",
                                                                text_xivh.c_str(),
                                                                text_vPt_vec.c_str(),
                                                                text_vPt_vec.c_str(),
                                                                text_trkPt_vec.c_str());

                std::string name_h_xivh = Form("h_xivh_%s_%s_%s", label_vPt.c_str(), label_trkPt.c_str(), label_cent.c_str());
                std::string title_h_xivh = Form("%s;%s;", title_h_suffix.c_str(),
                                                          text_defn_xivh.c_str());

                int nBins_xivh = 10;

                h_xivh[i][j][k] = 0;
                h_xivh[i][j][k] = new TH1D(name_h_xivh.c_str(), title_h_xivh.c_str(), nBins_xivh, 0, 5);

                std::string name_h2_trkPhi_vs_trkEta = Form("h2_trkPhi_vs_trkEta_%s", name_h_suffix.c_str());
                std::string title_h2_trkPhi_vs_trkEta = Form("%s;%s;%s", title_h_suffix.c_str(),
                                                            text_trkEta.c_str(),
                                                            text_trkPhi.c_str());

                h2_trkPhi_vs_trkEta[i][j][k] = 0;
                h2_trkPhi_vs_trkEta[i][j][k] = new TH2D(name_h2_trkPhi_vs_trkEta.c_str(), title_h2_trkPhi_vs_trkEta.c_str(),
                                                          nBinsX_eta, -1*xMax_eta, xMax_eta,
                                                          nBinsX_dphi, -1*xMax_phi, xMax_phi);
                vec_h2D.push_back(h2_trkPhi_vs_trkEta[i][j][k]);


                std::string name_h2_dphi_vs_deta = Form("h2_dphi_vs_deta_%s", name_h_suffix.c_str());
                std::string title_h2_dphi_vs_deta = Form("%s;%s;%s", title_h_suffix_dphi.c_str(),
                                                            text_deta.c_str(),
                                                            text_dphi.c_str());

                h2_dphi_vs_deta[i][j][k] = 0;
                h2_dphi_vs_deta[i][j][k] = new TH2D(name_h2_dphi_vs_deta.c_str(), title_h2_dphi_vs_deta.c_str(),
                                                          nBinsX_eta, 0, 5,
                                                          nBinsX_dphi, 0, xMax_phi);
                vec_h2D.push_back(h2_dphi_vs_deta[i][j][k]);

                std::string name_h2_dphi_vs_trkEta = Form("h2_dphi_vs_trkEta_%s", name_h_suffix.c_str());
                std::string title_h2_dphi_vs_trkEta = Form("%s;%s;%s", title_h_suffix_dphi.c_str(),
                                                            text_trkEta.c_str(),
                                                            text_dphi.c_str());

                h2_dphi_vs_trkEta[i][j][k] = 0;
                h2_dphi_vs_trkEta[i][j][k] = new TH2D(name_h2_dphi_vs_trkEta.c_str(), title_h2_dphi_vs_trkEta.c_str(),
                                                          nBinsX_eta, -1*xMax_eta,  xMax_eta,
                                                          nBinsX_dphi, 0, xMax_phi);
                vec_h2D.push_back(h2_dphi_vs_trkEta[i][j][k]);
            }
        }
    }

    // modify min/max for analysis
    for (int i = 0; i < nCents; ++i) {
        if (centsMax[i] < centsMin[i]) {
            centsMax[i] = 999999;
        }
    }
    for (int i = 0; i < nVPts; ++i) {
        if (vPtsMax[i] < vPtsMin[i]) {
            vPtsMax[i] = 999999;
        }
    }
    if (vEtaMax < vEtaMin) {
        vEtaMax = 999999;
    }
    for (int i = 0; i < nTrkPts; ++i) {
        if (trkPtsMax[i] < trkPtsMin[i]) {
            trkPtsMax[i] = 999999;
        }
    }
    if (trkEtaMax < trkEtaMin) {
        trkEtaMax = 999999;
    }

    // pointers to particle info
    int nParticles;
    std::vector<float>* p_pt;
    std::vector<float>* p_eta;
    std::vector<float>* p_phi;
    std::vector<int>* p_chg;
    std::vector<int>* p_sube;
    std::vector<float>* p_weight;
    std::vector<int>* p_evtIndex;

    std::vector<int> dummy_vec_I0(150000, 1);
    std::vector<int> dummy_vec_I1(150000, 1);
    std::vector<float> dummy_vec_F1(150000, 1);

    TTree* treeggHiNtuplizer = 0;
    TTree* treeHLT = 0;
    TTree* treeHiEvt = 0;
    TTree* treeJetSkim = 0;
    TTree* treeTrackSkim = 0;
    TTree* treeEvtSkim = 0;
    TTree* treeHiEvtMix = 0;

    std::string treePathHLT = "HltTree";
    std::string treePathHiEvt = "HiTree";
    std::string treePathTrack = "trackSkim";
    std::string treePathEvtSkim = "eventSkim";
    std::string treePathHiEvtMix = "mixEventSkim";

    /*
     * HLT_HIL2Mu20_v1, HLT_HIL2Mu15_v2, HLT_HIL1DoubleMu10_v1, HLT_HIL1DoubleMu0_v1, HLT_HIL3Mu15_v1, HLT_HIL3Mu20_v1
     */

    std::vector<std::string> triggerBranches;
    if (isPP17) {
        if (vIsZmm) {
            triggerBranches = {"HLT_HIL2Mu12_v1", "HLT_HIL3Mu12_v1", "HLT_HIL1DoubleMu0_v1", "HLT_HIL1DoubleMu10_v1"};
        }
        else if (vIsZee) {
            triggerBranches = {"HLT_HIDoublePhoton15_Eta3p1ForPPRef_Mass50to1000_v8",
                               "HLT_HIDoublePhoton15_Eta3p1ForPPRef_Mass50to1000_v9",
                               "HLT_HIEle15_WPLoose_Gsf_v1"
                               "HLT_HIEle20_WPLoose_Gsf_v1",
                               "HLT_HIEle30_WPLoose_Gsf_v1",
                               };
        }
    }
    else if (isPbPb18) {
        if (vIsZmm) {
            triggerBranches = {"HLT_HIL2Mu12_v1", "HLT_HIL3Mu12_v1", "HLT_HIL1DoubleMu0_v1", "HLT_HIL1DoubleMu10_v1"};
        }
        else if (vIsZee) {
            triggerBranches = {"HLT_HIDoubleEle10Gsf_v1", "HLT_HIEle20Gsf_v1"};
        }
    }
    int nTriggerBranches = triggerBranches.size();

    int nFiles = inputFiles.size();
    TFile* fileTmp = 0;

    std::cout << "initial reading to get the number of entries (if there is only one input file) and HiForest info" << std::endl;
    // read the first file only to get the HiForest info
    std::string inputPath = inputFiles.at(0).c_str();
    fileTmp = TFile::Open(inputPath.c_str(), "READ");
    fileTmp->cd();

    bool isvJetTrkSkim = true;
    treeggHiNtuplizer = 0;
    treeggHiNtuplizer = (TTree*)fileTmp->Get("EventTree");
    isvJetTrkSkim = (treeggHiNtuplizer != 0);
    if (!isvJetTrkSkim) {
        treePath = "ggHiNtuplizerGED/EventTree";
        treePathHLT = "hltanalysis/HltTree";
        treePathHiEvt = "hiEvtAnalyzer/HiTree";
        treePathTrack = "ppTrack/trackTree";
        treePathEvtSkim = "NULL";
        treePathHiEvtMix = "NULL";
    }

    if (nFiles == 1) {
        // read one tree only to get the number of entries
        treeggHiNtuplizer = (TTree*)fileTmp->Get(treePath.c_str());
        Long64_t entriesTmp = treeggHiNtuplizer->GetEntries();
        std::cout << "entries = " << entriesTmp << std::endl;
        treeggHiNtuplizer->Delete();
    }

    fileTmp->Close();
    // done with initial reading

    EventMatcher* em = new EventMatcher();
    Long64_t duplicateEntries = 0;

    Long64_t entries = 0;
    Long64_t entriesAnalyzed = 0;
    Long64_t entriesNoMixEvt = 0;

    int nFilesSkipped = 0;
    std::cout<< "Loop : " << treePath.c_str() <<std::endl;
    for (int iFile = 0; iFile < nFiles; ++iFile)  {

        std::string inputPath = inputFiles.at(iFile).c_str();
        std::cout <<"iFile = " << iFile << " , " ;
        std::cout <<"reading input file : " << inputPath.c_str() << std::endl;
        fileTmp = TFile::Open(inputPath.c_str(), "READ");

        // check if the file is usable, if not skip the file.
        if (isGoodFile(fileTmp) != 0) {
            std::cout << "File is not good. skipping file." << std::endl;
            nFilesSkipped++;
            continue;
        }

        treeggHiNtuplizer = (TTree*)fileTmp->Get(treePath.c_str());
        treeggHiNtuplizer->SetBranchStatus("*",0);     // disable all branches
        if (vIsPho) {
            treeggHiNtuplizer->SetBranchStatus("nPho",1);     // enable photon branches
            treeggHiNtuplizer->SetBranchStatus("pho*",1);     // enable photon branches
        }
        else if (vIsZmm) {
            treeggHiNtuplizer->SetBranchStatus("nMu",1);
            treeggHiNtuplizer->SetBranchStatus("mu*",1);
        }
        else if (vIsZee) {
            treeggHiNtuplizer->SetBranchStatus("nEle",1);
            treeggHiNtuplizer->SetBranchStatus("ele*",1);
        }
        treeggHiNtuplizer->SetBranchStatus("rho",1);

        treeHLT = (TTree*)fileTmp->Get(treePathHLT.c_str());
        treeHLT->SetBranchStatus("*",0);     // disable all branches
        int triggerBits[nTriggerBranches];
        for (int iTrig = 0; iTrig < nTriggerBranches; ++iTrig) {
            if (treeHLT->GetBranch(triggerBranches[iTrig].c_str())) {
                treeHLT->SetBranchStatus(triggerBranches[iTrig].c_str(), 1);
                treeHLT->SetBranchAddress(triggerBranches[iTrig].c_str(), &(triggerBits[iTrig]));
            }
            else {
                triggerBits[iTrig] = 0;
            }
        }

        // specify explicitly which branches to use, do not use wildcard
        treeHiEvt = (TTree*)fileTmp->Get(treePathHiEvt.c_str());
        setBranchStatusTreeHiEvt(treeHiEvt, isMC);

        bool anaJets = (treePathJetSkim.size() != 0);
        if (anaJets) {
            treeJetSkim = (TTree*)fileTmp->Get(treePathJetSkim.c_str());
            treeJetSkim->SetBranchStatus("*",0);     // disable all branches
            if (isRecoJet) {
                treeJetSkim->SetBranchStatus("njet",1);
                treeJetSkim->SetBranchStatus("jetpt*",1);
                treeJetSkim->SetBranchStatus("rawpt",1);
                treeJetSkim->SetBranchStatus("ref*",1);
                treeJetSkim->SetBranchStatus("subid",1);
            }
            else {
                treeJetSkim->SetBranchStatus("ngen",1);
                treeJetSkim->SetBranchStatus("gen*",1);
            }
        }

        treeTrackSkim = (TTree*)fileTmp->Get(treePathTrack.c_str());
        treeTrackSkim->SetBranchStatus("*",0);     // disable all branches

        std::string mix_str = (isBkgTrk) ? "_mix" : "";
        if (isRecoTrk) {
            treeTrackSkim->SetBranchStatus(Form("nTrk%s", mix_str.c_str()),1);
            treeTrackSkim->SetBranchStatus(Form("trkPt%s", mix_str.c_str()),1);
            treeTrackSkim->SetBranchStatus(Form("trkEta%s", mix_str.c_str()),1);
            treeTrackSkim->SetBranchStatus(Form("trkPhi%s", mix_str.c_str()),1);
            treeTrackSkim->SetBranchStatus(Form("trkCharge%s", mix_str.c_str()),1);
            treeTrackSkim->SetBranchStatus(Form("trkWeight%s", mix_str.c_str()),1);
            treeTrackSkim->SetBranchStatus(Form("highPurity%s", mix_str.c_str()),1);
            treeTrackSkim->SetBranchStatus(Form("pfHcal%s", mix_str.c_str()),1);
            treeTrackSkim->SetBranchStatus(Form("pfEcal%s", mix_str.c_str()),1);
            treeTrackSkim->SetBranchStatus(Form("evttrk%s", mix_str.c_str()),1);
        }
        else {
            treeTrackSkim->SetBranchStatus(Form("mult%s", mix_str.c_str()),1);
            treeTrackSkim->SetBranchStatus(Form("pt%s", mix_str.c_str()),1);
            treeTrackSkim->SetBranchStatus(Form("eta%s", mix_str.c_str()),1);
            treeTrackSkim->SetBranchStatus(Form("phi%s", mix_str.c_str()),1);
            treeTrackSkim->SetBranchStatus(Form("chg%s", mix_str.c_str()),1);
            treeTrackSkim->SetBranchStatus(Form("sube%s", mix_str.c_str()),1);
            treeTrackSkim->SetBranchStatus(Form("evtgen%s", mix_str.c_str()),1);
        }
        if (maxNVtx > 0 || minNVtx > -1) {
            treeTrackSkim->SetBranchStatus("nVtx",1);
        }

        if (isvJetTrkSkim) {
            treeEvtSkim = (TTree*)fileTmp->Get(treePathEvtSkim.c_str());
            treeEvtSkim->SetBranchStatus("*",0);
            treeEvtSkim->SetBranchStatus("pf_h_HF_totE",1);
            treeEvtSkim->SetBranchStatus("pf_eg_HF_totE",1);

            treeHiEvtMix = (TTree*)fileTmp->Get(treePathHiEvtMix.c_str());
            treeHiEvtMix->SetBranchStatus("*",0);     // disable all branches
            treeHiEvtMix->SetBranchStatus("nmix",1);
            treeHiEvtMix->SetBranchStatus("hiBin_mix",1);
            if (rotateEvtPlane) {
                treeHiEvtMix->SetBranchStatus("hiEvtPlanes_mix",1);
            }
        }

        if (isMC) {
            treeggHiNtuplizer->SetBranchStatus("nMC*",1);     // enable GEN particle branches
            treeggHiNtuplizer->SetBranchStatus("mc*",1);      // enable GEN particle branches

            treeHiEvt->SetBranchStatus("weight", 1);
            treeHiEvt->SetBranchStatus("pthat",1);
        }

        ggHiNtuplizer ggHi;
        ggHi.setupTreeForReading(treeggHiNtuplizer);

        hiEvt hiEvt;
        hiEvt.setupTreeForReading(treeHiEvt);

        jetSkim jets;
        if (anaJets) {
            jets.setupTreeForReading(treeJetSkim);
        }

        trackSkim trks;
        trks.setupTreeForReading(treeTrackSkim);

        eventSkim evtskim;
        mixEventSkim mixEvents;
        if (isvJetTrkSkim) {

            evtskim.setupTreeForReading(treeEvtSkim);
            mixEvents.setupTreeForReading(treeHiEvtMix);
        }

        if (isRecoTrk) {
            p_pt = trks.p_trkPt;
            p_eta = trks.p_trkEta;
            p_phi = trks.p_trkPhi;
            p_chg = trks.p_trkCharge;
            p_sube = &dummy_vec_I1;
            p_weight = trks.p_trkWeight;
            p_evtIndex = &dummy_vec_I0;
            if (isBkgTrk) {
                p_pt = trks.p_trkPt_mix;
                p_eta = trks.p_trkEta_mix;
                p_phi = trks.p_trkPhi_mix;
                p_chg = &dummy_vec_I1;
                p_sube = &dummy_vec_I1;
                p_weight = trks.p_trkWeight_mix;
                p_evtIndex = trks.p_evttrk_mix;
            }
        }
        else {
            p_pt = trks.p_pt;
            p_eta = trks.p_eta;
            p_phi = trks.p_phi;
            p_chg = trks.p_chg;
            p_sube = trks.p_sube;
            p_weight = &dummy_vec_F1;
            p_evtIndex = &dummy_vec_I0;
            if (isBkgTrk) {
                p_pt = trks.p_pt_mix;
                p_eta = trks.p_eta_mix;
                p_phi = trks.p_phi_mix;
                p_chg = trks.p_chg_mix;
                p_sube = trks.p_sube_mix;
                p_weight = &dummy_vec_F1;
                p_evtIndex = trks.p_evtgen_mix;
            }
        }

        Long64_t entriesTmp = treeggHiNtuplizer->GetEntries();
        entries += entriesTmp;
        std::cout << "entries in File = " << entriesTmp << std::endl;
        for (Long64_t j_entry = 0; j_entry < entriesTmp; ++j_entry)
        {
            if (j_entry % 2000 == 0)  {
              std::cout << "current entry = " <<j_entry<<" out of "<<entriesTmp<<" : "<<std::setprecision(2)<<(double)j_entry/entriesTmp*100<<" %"<<std::endl;
            }

            treeggHiNtuplizer->GetEntry(j_entry);
            treeHLT->GetEntry(j_entry);
            treeHiEvt->GetEntry(j_entry);
            if (isvJetTrkSkim) {
                if (anaJets) {
                    treeJetSkim->GetEntry(j_entry);
                }

                treeTrackSkim->GetEntry(j_entry);
                treeEvtSkim->GetEntry(j_entry);
                treeHiEvtMix->GetEntry(j_entry);

                if (isPbPb && mixEvents.nmix == 0)  {
                    std::cout << "WARNING : no mixed event for j_entry = " << j_entry
                              << " , run = " << hiEvt.run << " , hiEvt.lumi = " << hiEvt.lumi << " , hiEvt.evt = " << hiEvt.evt << std::endl;
                    std::cout << "skipping event " << std::endl;
                    entriesNoMixEvt++;
                    continue;  // TODO : remove isPbPb or nmix requirement
                }
                if (maxNVtx > 0 && trks.nVtx > maxNVtx) {
                    continue;
                }
                if (minNVtx > -1 && trks.nVtx < minNVtx) {
                    continue;
                }
            }

            bool eventAdded = em->addEvent(hiEvt.run, hiEvt.lumi, hiEvt.evt, j_entry);
            if(!eventAdded) // this event is duplicate, skip this one.
            {
                duplicateEntries++;
                continue;
            }

            bool passedTrig = (nTriggerBranches == 0);
            for (int iTrig = 0; iTrig < nTriggerBranches; ++iTrig) {
                if (triggerBits[iTrig] > 0) {
                    passedTrig = true;
                    break;
                }
            }

            entriesAnalyzed++;

            double w = 1;
            int hiBin = hiEvt.hiBin;
            int cent = hiBin/2;
            if (doEventWeight > 0) {
                w = hiEvt.weight;
                double vertexWeight = 1;
                if (isPbPb18 && isMC)  {
                    vertexWeight = 1.19357*TMath::Exp(-0.5*TMath::Power((hiEvt.vz-(-22.3784))/39.7459, 2));
                }
                else if (isPbPb15 && isMC)  {
                    vertexWeight = 1.37487*TMath::Exp(-0.5*TMath::Power((hiEvt.vz-0.30709)/7.41379, 2));
                }
                else if (isPP17 && isMC)  {
                    vertexWeight = 1.0/(1.10749*TMath::Exp(-0.5*TMath::Power((hiEvt.vz-(-0.504278))/13.5601, 2)));
                }
                double centWeight = 1;
                if (isPbPb && isMC)  centWeight = findNcoll(hiBin);
                w *= vertexWeight * centWeight;
            }

            if (nPthatWeights > 0) {
                double pthatWeight = 0;
                for (int i = 0; i < nPthatWeights; ++i) {
                    if (hiEvt.pthat >= pthatWeights[0][i] && hiEvt.pthat < pthatWeights[1][i]) {
                        pthatWeight = pthatWeights[2][i];
                        break;
                    }
                }
                w *= pthatWeight;
            }

            double vPt = -1;
            double vEta = -999999;
            double vPhi = -999999;
            double vM = 0;
            bool ll_os = false;     // dilepton opposite charge
            std::vector<float> llEta = {-998877, -998877};
            std::vector<float> llPhi = {-998877, -998877};
            float minDR2_lep_trk = 0.64;

            double genVPt = -1;
            double genVEta = -999999;
            double genVPhi = -999999;
            float maxDR2_reco_gen_V = 0.16;

            if (vIsPho) {
                for (int i = 0; i < ggHi.nPho; ++i) {

                    if (excludeHI18HEMfailure && !ggHi.passedHI18HEMfailurePho(i))  continue;
                }
            }
            else if (vIsZ) {

                double zmassPDG = 91.1876;
                double deltaMass = 999999;

                TLorentzVector vecl1;
                TLorentzVector vecl2;
                TLorentzVector vecll;

                double lMass = -1;
                int pdgCode = -1;
                int nL = 0;
                std::vector<float> *lPt;
                std::vector<float> *lEta;
                std::vector<float> *lPhi;
                std::vector<int>   *lChg;

                double lEtaMax = -1;

                if (vIsZmm) {

                    lMass = 0.105658;
                    pdgCode = 13;
                    nL = ggHi.nMu;
                    lPt = ggHi.muPt;
                    lEta = ggHi.muEta;
                    lPhi = ggHi.muPhi;
                    lChg = ggHi.muCharge;
                    lEtaMax = 2.4;
                }
                else if (vIsZee) {

                    lMass = 0.000511;
                    pdgCode = 11;
                    nL = ggHi.nEle;
                    lPt = ggHi.elePt;
                    lEta = ggHi.eleEta;
                    lPhi = ggHi.elePhi;
                    lChg = ggHi.eleCharge;
                    lEtaMax = 2.1;
                }

                if (!isRecoV) {
                    nL = ggHi.nMC;
                    lPt = ggHi.mcPt;
                    lEta = ggHi.mcEta;
                    lPhi = ggHi.mcPhi;
                    lChg = ggHi.mcPID;
                }
                else if (isMC && isRecoV) {

                    float maxGenVPt = -1;
                    for (int i = 0; i < ggHi.nMC; ++i) {

                        if ((*ggHi.mcPID)[i] != 23)  continue;
                        if ((*ggHi.mcStatus)[i] != 62)  continue;
                        if (!((*ggHi.mcMass)[i] >= 60 && (*ggHi.mcMass)[i] <= 120)) continue;

                        if ((*ggHi.mcPt)[i] > maxGenVPt) {
                            genVPt = (*ggHi.mcPt)[i];
                            genVEta = (*ggHi.mcEta)[i];
                            genVPhi = (*ggHi.mcPhi)[i];

                            maxGenVPt = genVPt;
                        }
                    }
                }

                for (int i = 0; i < nL; ++i) {

                    float l1pt = (*lPt)[i];
                    if (vIsZee && isRecoV) {
                        l1pt *= ggHi.getElePtCorrFactor(i, collisionType, hiBin);
                    }
                    if (!(l1pt > 20)) continue;
                    if (!(std::fabs((*lEta)[i]) < lEtaMax)) continue;

                    if (vIsZmm && isRecoV) {
                        if (!ggHi.passedMuSelection(i, collisionType)) continue;
                    }
                    else if (vIsZee && isRecoV) {
                        if (!ggHi.passedEleSelection(i, collisionType, hiBin)) continue;
                        if (excludeHI18HEMfailure && !ggHi.passedHI18HEMfailureEle(i))  continue;
                    }
                    else if (!isRecoV) {
                        if (std::fabs((*ggHi.mcPID)[i]) != pdgCode) continue;
                        else if (vIsZee) {
                            if (1.4442 < std::fabs((*lEta)[i]) && std::fabs((*lEta)[i]) < 1.566) continue;
                            if (excludeHI18HEMfailure && !ggHi.passedHI18HEMfailureGen(i))  continue;
                        }
                    }

                    for (int j = i+1; j < nL; ++j) {

                        float l2pt = (*lPt)[j];
                        if (vIsZee && isRecoV) {
                            l2pt *= ggHi.getElePtCorrFactor(j, collisionType, hiBin);
                        }
                        if (!(l2pt > 20)) continue;
                        if (!(std::fabs((*lEta)[j]) < lEtaMax)) continue;

                        if (vIsZmm && isRecoV) {
                            if (!ggHi.passedMuSelection(j, collisionType)) continue;
                        }
                        else if (vIsZee && isRecoV) {
                            if (!ggHi.passedEleSelection(j, collisionType, hiBin)) continue;
                            if (excludeHI18HEMfailure && !ggHi.passedHI18HEMfailureEle(j))  continue;
                        }
                        else if (!isRecoV) {
                            if (std::fabs((*ggHi.mcPID)[j]) != pdgCode) continue;
                            if (vIsZee) {
                                if (1.4442 < std::fabs((*lEta)[j]) && std::fabs((*lEta)[j]) < 1.566) continue;
                                if (excludeHI18HEMfailure && !ggHi.passedHI18HEMfailureGen(j))  continue;
                            }
                        }

                        vecl1.SetPtEtaPhiM(l1pt, (*lEta)[i], (*lPhi)[i], lMass);
                        vecl2.SetPtEtaPhiM(l2pt, (*lEta)[j], (*lPhi)[j], lMass);

                        vecll = vecl1 + vecl2;

                        if (!(vecll.M() >= 60 && vecll.M() <= 120)) continue;

                        if (std::fabs(vecll.M() - zmassPDG) < deltaMass) {
                            deltaMass = std::fabs(vecll.M() - zmassPDG);
                            vPt = vecll.Pt();
                            vEta = vecll.Eta();
                            vPhi = vecll.Phi();
                            vM = vecll.M();
                            ll_os = (((*lChg)[i] == -1*(*lChg)[j]));

                            llEta = {(*lEta)[i], (*lEta)[j]};
                            llPhi = {(*lPhi)[i], (*lPhi)[j]};
                        }
                    }
                }
            }

            double wV = w;

            // reco eff
            if (isMC && isRecoV && genVPt > 0) {

                double matchedRG = ((vIsPho || ll_os) && vPt >= 0 && getDR2(genVEta, genVPhi, vEta, vPhi) < maxDR2_reco_gen_V);

                double genVEtaAbs = std::fabs(genVEta);

                for (int i = 0; i < nCents; ++i) {

                    if (isPbPb && !(centsMin[i] <= cent && cent < centsMax[i]))  continue;

                    if (vEtaMin <= genVEtaAbs && genVEtaAbs < vEtaMax) {

                        h_reco_denom_vPt[i]->Fill(genVPt, wV);
                        if (matchedRG) {
                            h_reco_num_vPt[i]->Fill(genVPt, wV);

                            h2_rgVPt_ratio_vs_vPt[i]->Fill(genVPt, vPt / genVPt, wV);
                            h2_rgVPhi_diff_vs_vPt[i]->Fill(genVPt, getDPHI(vPhi, genVPhi), wV);
                        }
                    }

                    for (int j = 0; j < nVPts; ++j) {

                        if (!(vPtsMin[j] <= genVPt && genVPt < vPtsMax[j]))  continue;

                        h_reco_denom_vEta[i][j]->Fill(genVEta, wV);
                        if (matchedRG) {
                            h_reco_num_vEta[i][j]->Fill(genVEta, wV);
                        }
                    }
                }

                for (int j = 0; j < nVPts; ++j) {

                    if (!(vPtsMin[j] <= genVPt && genVPt < vPtsMax[j]))  continue;

                    if (vEtaMin <= genVEtaAbs && genVEtaAbs < vEtaMax) {

                        h_reco_denom_cent[j]->Fill(cent, wV);
                        if (matchedRG) {
                            h_reco_num_cent[j]->Fill(cent, wV);
                        }
                    }

                }
            }

            if (vPt < 0) continue;

            double vEtaAbs = std::fabs(vEta);

            // fill trigger eff histograms
            if (vIsPho || ll_os) {
                for (int i = 0; i < nCents; ++i) {

                    if (isPbPb && !(centsMin[i] <= cent && cent < centsMax[i]))  continue;

                    h_trig_denom_vPt[i]->Fill(vPt, wV);
                    if (passedTrig) {
                        h_trig_num_vPt[i]->Fill(vPt, wV);
                    }
                }
            }

            if (!passedTrig) continue;

            if (vIsPho || ll_os) {
                // opposite charge pairs
                for (int i = 0; i < nCents; ++i) {

                    if (isPbPb && !(centsMin[i] <= cent && cent < centsMax[i]))  continue;

                    if (vEtaMin <= vEtaAbs && vEtaAbs < vEtaMax) {
                        h_vPt[i]->Fill(vPt, wV);
                        h2_hiHF_vs_vPt[i]->Fill(vPt, hiEvt.hiHF, wV);
                        h2_rho_vs_vPt[i]->Fill(vPt, ggHi.rho, wV);
                        if (isvJetTrkSkim) {
                            h2_PFHFtotE_vs_vPt[i]->Fill(vPt, (evtskim.pf_h_HF_totE + evtskim.pf_eg_HF_totE), wV);
                        }
                    }

                    for (int j = 0; j < nVPts; ++j) {

                        if (!(vPtsMin[j] <= vPt && vPt < vPtsMax[j]))  continue;

                        h_vEta[i][j]->Fill(vEta, wV);

                        if (vEtaMin <= vEtaAbs && vEtaAbs < vEtaMax) {

                            h_vPhi[i][j]->Fill(vPhi, wV);
                            h_vM_os[i][j]->Fill(vM, wV);

                            // event observables
                            h_vtxz[i][j]->Fill(hiEvt.vz, wV);
                            h_dphi_evtPlane_V[i][j]->Fill(std::fabs(getDPHI(vPhi, hiEvt.hiEvtPlanes[8])), wV);
                        }
                    }
                }

                for (int j = 0; j < nVPts; ++j) {

                    if (!(vPtsMin[j] <= vPt && vPt < vPtsMax[j]))  continue;

                    if (vEtaMin <= vEtaAbs && vEtaAbs < vEtaMax) {

                        h_cent[j]->Fill(cent, wV);
                    }

                }
            }
            else {
                // same charge pairs
                for (int i = 0; i < nCents; ++i) {

                    if (isPbPb && !(centsMin[i] <= cent && cent < centsMax[i]))  continue;

                    for (int j = 0; j < nVPts; ++j) {

                        if (!(vPtsMin[j] <= vPt && vPt < vPtsMax[j]))  continue;

                        if (vEtaMin <= vEtaAbs && vEtaAbs < vEtaMax) {

                            h_vM_ss[i][j]->Fill(vM, wV);
                        }
                    }
                }
            }

            if (!(vEtaMin <= vEtaAbs && vEtaAbs < vEtaMax))  continue;
            if (vIsZ && !ll_os) continue;

            if (!isvJetTrkSkim) continue;

            TLorentzVector vV;
            vV.SetPtEtaPhiM(vPt, 0, vPhi, vM);

            if (isRecoTrk && !isBkgTrk) {
                nParticles = trks.nTrk;
            }
            else if (isRecoTrk && isBkgTrk) {
                nParticles = trks.nTrk_mix;
            }
            else if (!isRecoTrk && !isBkgTrk) {
                nParticles = trks.mult;
            }
            else {
                nParticles = trks.mult_mix;
            }

            double wMixEvts = wV;
            if (isBkgTrk) {
                wMixEvts *= (1.0 / (double(mixEvents.nmix)));
            }
            for (int i = 0; i < nParticles; ++i) {

                //if (!passedTrkSelection(trks, i, collisionType))  continue;
                if (!isRecoTrk) {
                    if ((*p_chg)[i] == 0)  continue;
                    if (isSigTrk && (*p_sube)[i] != 0)  continue;
                }

                if (!(trkEtaMin <= std::fabs((*p_eta)[i]) && std::fabs((*p_eta)[i]) < trkEtaMax))  continue;

                float t_pt = (*p_pt)[i];
                float t_eta = (*p_eta)[i];
                float t_phi = (*p_phi)[i];

                if (isBkgTrk && rotateEvtPlane) {
                    int iEvt = (*p_evtIndex)[i];
                    t_phi += (hiEvt.hiEvtPlanes[8] - (*mixEvents.p_hiEvtPlanes_mix)[iEvt]);
                    t_phi = correctPhiRange(t_phi);
                }

                if (vIsZ) {
                    if (getDR2(t_eta, t_phi, llEta[0], llPhi[0]) < minDR2_lep_trk)  continue;
                    if (getDR2(t_eta, t_phi, llEta[1], llPhi[1]) < minDR2_lep_trk)  continue;
                }

                double trkWeightTmp = (*p_weight)[i];
                if (isRecoTrk && noTrkWeights) {
                    trkWeightTmp = 1;
                }
                else if (isRecoTrk && redoTrkWeights) {

                    int hiBinTmp = hiBin;
                    if (isBkgTrk) {
                        int iEvt = (*p_evtIndex)[i];
                        hiBinTmp = (*mixEvents.p_hiBin_mix)[iEvt];
                    }

                    if (isPP) {
                        hiBinTmp = 0;
                    }

                    if (isPP15 || isPbPb15) {
                        trkWeightTmp = trkCorr2015->getTrkCorr(t_pt, t_eta, t_phi, hiBinTmp);
                    }
                    else if (isPP17) {
                        trkWeightTmp = 1.10;
                    }
                    else if (isPbPb18) {
                        //trkWeightTmp = trkEff2018.getCorrection(t_pt, t_eta, hiBinTmp);
                        float effTmp = (trkEff2018.getEfficiency(t_pt, t_eta, hiBinTmp, true));
                        trkWeightTmp = (effTmp > 0.001) ? (1.0)/effTmp : 0;
                    }
                }
                double wTrk = trkWeightTmp * wMixEvts;

                for (int iCent = 0; iCent < nCents; ++iCent) {

                    if (isPbPb && !(centsMin[iCent] <= cent && cent < centsMax[iCent]))  continue;

                    for (int iVPt = 0; iVPt < nVPts; ++iVPt) {

                        if (!(vPtsMin[iVPt] <= vPt && vPt < vPtsMax[iVPt]))  continue;

                        float dphi = std::fabs(getDPHI(vPhi, t_phi));
                        double dphiMinTmp = vTrkDphiMin * TMath::Pi();
                        double dphiMaxTmp = vTrkDphiMax * TMath::Pi();

                        if (dphiMinTmp < dphi && dphi <= dphiMaxTmp) {
                            h_trkPt[iCent][iVPt]->Fill(t_pt, wTrk);
                        }

                        h2_dphi_vs_trkPt[iCent][iVPt]->Fill(t_pt, dphi, wTrk);

                        for (int iTrkPt = 0; iTrkPt < nTrkPts; ++iTrkPt) {

                            if (!(trkPtsMin[iTrkPt] <= t_pt && t_pt < trkPtsMax[iTrkPt]))  continue;

                            float deta = std::fabs(vEta - t_eta);
                            float dR = std::sqrt( dphi*dphi + deta*deta );

                            h_dphi[iCent][iVPt][iTrkPt]->Fill(dphi, wTrk);
                            h_dR[iCent][iVPt][iTrkPt]->Fill(dR, wTrk);

                            h2_dphi_vs_deta[iCent][iVPt][iTrkPt]->Fill(deta, dphi, wTrk);
                            h2_dphi_vs_trkEta[iCent][iVPt][iTrkPt]->Fill(t_eta, dphi, wTrk);

                            if (!(dphiMinTmp < dphi && dphi <= dphiMaxTmp)) continue;

                            h_trkEta[iCent][iVPt][iTrkPt]->Fill(t_eta, wTrk);
                            h_trkPhi[iCent][iVPt][iTrkPt]->Fill(t_phi, wTrk);
                            h_dphi_evtPlane_trk[iCent][iVPt][iTrkPt]->Fill(std::fabs(getDPHI(t_phi, hiEvt.hiEvtPlanes[8])), wTrk);

                            h_deta[iCent][iVPt][iTrkPt]->Fill(deta, wTrk);

                            h2_trkPhi_vs_trkEta[iCent][iVPt][iTrkPt]->Fill(t_eta, t_phi, wTrk);

                            float vPhiNeg = (-1*vV).Phi();
                            float dphineg = std::fabs(getDPHI(vPhiNeg, t_phi));
                            float dRneg = std::sqrt( dphineg*dphineg + deta*deta );
                            h_dRneg[iCent][iVPt][iTrkPt]->Fill(dRneg, wTrk);

                            float z_vt = t_pt / vPt;

                            TLorentzVector vTrk;
                            vTrk.SetPtEtaPhiM(t_pt, 0, t_phi, 0);
                            float angle = vV.Angle(vTrk.Vect());
                            float z_vt_T = vTrk.P() * fabs(cos(angle)) / vPt;
                            float xi_vt = log(1.0 / z_vt_T);

                            h_zh[iCent][iVPt][iTrkPt]->Fill(z_vt, wTrk);
                            h_zh_T[iCent][iVPt][iTrkPt]->Fill(z_vt_T, wTrk);
                            h_xivh[iCent][iVPt][iTrkPt]->Fill(xi_vt, wTrk);
                        }

                    }
                }
            }
        }
        fileTmp->Close();
    }
    std::cout<<  "Loop ENDED : " << treePath.c_str() <<std::endl;
    std::cout << "nFilesSkipped = " << nFilesSkipped << std::endl;
    std::cout << "entries            = " << entries << std::endl;
    std::cout << "duplicateEntries   = " << duplicateEntries << std::endl;
    std::cout << "entriesAnalyzed    = " << entriesAnalyzed << std::endl;
    std::cout << "entriesNoMixEvt    = " << entriesNoMixEvt << std::endl;

    output->cd();
    std::cout << "### post loop processing - START ###" << std::endl;

    std::cout << "rebin" << std::endl;
    TH1D* hTmp = 0;
    for (int i = 0; i < nCents; ++i) {
        for (int j = 0; j < nVPts; ++j) {

            std::string tmpName;
            double binW = 0;
            std::vector<double> binsX;
            int nBinsX = 0;

            // rebin trkPt
            //binW = h_trkPt[i][j]->GetBinWidth(1);
            binsX = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 14, 18, 24, xMax_trkPt};
            nBinsX = binsX.size()-1;

            double arr_trkPt[nBinsX+1];
            std::copy(binsX.begin(), binsX.end(), arr_trkPt);

            tmpName = replaceAll(h_trkPt[i][j]->GetName(), "h_trkPt", "h_trkPt_rebin");
            hTmp = (TH1D*)h_trkPt[i][j]->Rebin(nBinsX, tmpName.c_str(), arr_trkPt);
            hTmp->Write("",TObject::kOverwrite);

            for (int k = 0; k < nTrkPts; ++k) {

                // rebin dphi
                binW = h_dphi[i][j][k]->GetBinWidth(1);
                binsX = {0, binW*3, binW*6, binW*9, binW*12, binW*14, binW*16, binW*18, binW*19, binW*20};
                nBinsX = binsX.size()-1;

                double arr_dphi[nBinsX+1];
                std::copy(binsX.begin(), binsX.end(), arr_dphi);

                tmpName = replaceAll(h_dphi[i][j][k]->GetName(), "h_dphi", "h_dphi_rebin");
                hTmp = (TH1D*)h_dphi[i][j][k]->Rebin(nBinsX, tmpName.c_str(), arr_dphi);
                hTmp->Write("",TObject::kOverwrite);
            }
        }
    }

    std::cout << "projections of TH2D to TH1D" << std::endl;
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
    }

    std::cout << "efficiency" << std::endl;
    TGraphAsymmErrors* gTmp = 0;
    hTmp = 0;
    int nVec_h_num = vec_h_num.size();
    int nVec_h_denom = vec_h_denom.size();
    if (nVec_h_num != nVec_h_denom) {
        std::cout << "ERROR : different number of numerator and denominators" << std::endl;
    }
    for (int i = 0; i < nVec_h_num; ++i) {
        std::string tmpName = replaceFirst(vec_h_num[i]->GetName(), "_num_", "_eff_");

        hTmp = (TH1D*)vec_h_num[i]->Clone(tmpName.c_str());
        hTmp->Divide(vec_h_denom[i]);
        hTmp->GetYaxis()->SetTitle("Efficiency");
        hTmp->Write("",TObject::kOverwrite);

        tmpName = replaceFirst(vec_h_num[i]->GetName(), "_num_", "_eff_");
        tmpName = replaceFirst(tmpName, "h_", "g_");

        gTmp = new TGraphAsymmErrors();
        gTmp->SetName(tmpName.c_str());
        gTmp->BayesDivide(vec_h_num[i], vec_h_denom[i]);
        gTmp->SetTitle(vec_h_num[i]->GetTitle());
        gTmp->GetXaxis()->SetTitle(vec_h_num[i]->GetXaxis()->GetTitle());
        gTmp->GetYaxis()->SetTitle("Efficiency");
        gTmp->SetMarkerStyle(kFullCircle);
        gTmp->Write("",TObject::kOverwrite);
    }

    std::cout << "### post loop processing - END ###" << std::endl;

    std::cout<<"Writing the output file."<<std::endl;
    output->Write("",TObject::kOverwrite);
    std::cout<<"Closing the output file."<<std::endl;
    output->Close();
    std::cout<<"running vJetTrkAna() - END"<<std::endl;
}

/*
 * run the macro without going through event loop, things done before and after the loop
 */
void vJetTrkAnaNoLoop(std::string configFile, std::string inputFile, std::string outputFile)
{
    std::cout<<"running vJetTrkAna()"<<std::endl;
    std::cout<<"configFile  = "<< configFile.c_str() <<std::endl;
    std::cout<<"inputFile   = "<< inputFile.c_str()  <<std::endl;
    std::cout<<"outputFile  = "<< outputFile.c_str() <<std::endl;

    if (readConfiguration(configFile, inputFile) != 0)  return;
    printConfiguration();

    TFile* input = TFile::Open(inputFile.c_str(), "READ");

    TFile* output = TFile::Open(outputFile.c_str(),"RECREATE");
    output->cd();

    std::cout<<"Closing the input file."<<std::endl;
    input->Close();
    std::cout<<"Closing the output file."<<std::endl;
    output->Close();
    std::cout<<"running vJetTrkAna() - END"<<std::endl;
}

int main(int argc, char** argv)
{
    std::vector<std::string> argStr = ArgumentParser::ParseParameters(argc, argv);
    int nArgStr = argStr.size();

    argOptions = ArgumentParser::ParseOptions(argc, argv);
    bool noLoop = (findPositionInVector(argOptions, ARGUMENTPARSER::noLoop) >= 0);

    if (nArgStr == 4) {
        if (noLoop) vJetTrkAnaNoLoop(argStr.at(1), argStr.at(2), argStr.at(3));
        else        vJetTrkAna(argStr.at(1), argStr.at(2), argStr.at(3));
        return 0;
    }
    else if (nArgStr == 3) {
        if (noLoop) vJetTrkAnaNoLoop(argStr.at(1), argStr.at(2));
        else        vJetTrkAna(argStr.at(1), argStr.at(2));
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./Histogramming/vJetTrk/vJetTrkAna.exe <configFile> <inputFile> <outputFile>"
                << std::endl;
        return 1;
    }
}

int readConfiguration(std::string configFile, std::string inputFile)
{
    ConfigurationParser confParser;
    confParser.openConfigFile(configFile);

    confParser.parsedKeyWords = InputConfigurationParser::parseKeyWords(inputFile);

    VJT::ewObj = parseEWObj(confParser.ReadConfigValue("ewObj"));

    treePath = confParser.ReadConfigValue("treePath");
    sampleType = confParser.ReadConfigValue("sampleType");

    treePathJetSkim = confParser.ReadConfigValue("treePathJetSkim");

    doEventWeight = confParser.ReadConfigValueInteger("doEventWeight");
    pthatWeights = ConfigurationParser::ParseListTriplet(confParser.ReadConfigValue("pthatWeights"));

    // RECO cuts
    excludeHI18HEMfailure = (confParser.ReadConfigValueInteger("excludeHI18HEMfailure") > 0);

    applyTrkWeights = confParser.ReadConfigValueInteger("applyTrkWeights");

    rotateEvtPlane = (confParser.ReadConfigValueInteger("rotateEvtPlane") > 0);

    nPthatWeights = pthatWeights[0].size();

    return 0;
}

/*
 * print information read from configuration
 * assumes that readConfiguration() is run before
 */
void printConfiguration()
{
    std::cout<<"Configuration :"<<std::endl;
    std::cout << "EW object = " << VJT::ewObjsStr[VJT::ewObj].c_str() << std::endl;
    if (VJT::ewObj < 0 || VJT::ewObj >= VJT::EWOBJS::kN_EWOBJS) {
        std::cout << "ERROR : no valid EW object given" << std::endl;
        std::cout << "ewObj (index for EW object) = " << VJT::ewObj << std::endl;
    }

    std::cout << "treePath = " << treePath.c_str() << std::endl;
    std::cout << "sampleType = " << sampleType.c_str() << std::endl;

    std::cout << "treePathJetSkim = " << treePathJetSkim.c_str() << std::endl;

    std::cout << "doEventWeight = " << doEventWeight << std::endl;

    std::cout << "nPthatWeights = " << nPthatWeights << std::endl;
    for (int i = 0; i < nPthatWeights; ++i) {
        std::cout << Form("pthatWeights[%d] = { ", i);
        std::cout << Form("%.0f, ", pthatWeights[0].at(i));
        std::cout << Form("%f, ", pthatWeights[1].at(i));
        std::cout << Form("%f }", pthatWeights[2].at(i)) << std::endl;;
    }

    std::cout << "excludeHI18HEMfailure = " << excludeHI18HEMfailure << std::endl;

    std::cout << "applyTrkWeights = " << applyTrkWeights << std::endl;

    std::cout << "rotateEvtPlane = " << rotateEvtPlane << std::endl;
}

int parseRecoObj(std::string recoObjStr)
{
    recoObjStr = trim(recoObjStr);
    recoObjStr = toLowerCase(recoObjStr);

    if (recoObjStr == "pho" || recoObjStr == "photon" || recoObjStr == "0") {
        return VJT::EWOBJS::kPhoton;
    }
    else if (recoObjStr == "ele" || recoObjStr == "zBoson" || recoObjStr == "1") {
        return VJT::EWOBJS::kZBoson;
    }
    else {
        return -1;
    }
}

std::vector<double> parseRanges(std::string rangesStr, int indexMinMax)
{
    std::vector<std::string> vecStr = split(rangesStr, ",", false, false);

    std::vector<double> res;
    for (std::vector<std::string>::const_iterator it = vecStr.begin(); it != vecStr.end(); ++it) {
        std::vector<std::string> vecStrTmp = split((*it), ":");

        res.push_back(std::atof(vecStrTmp[indexMinMax].c_str()));
    }

    return res;
}

std::vector<double> parseRangesMin(std::string rangesStr)
{
    return parseRanges(rangesStr, 0);
}

std::vector<double> parseRangesMax(std::string rangesStr)
{
    return parseRanges(rangesStr, 1);
}
