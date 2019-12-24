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
#include <TRandom3.h>

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>

#include "../../CorrelationTuple/EventMatcher.h"
#include "../../TreeHeaders/CutConfigurationTree.h"
#include "../../TreeHeaders/ggHiNtuplizerTree.h"
#include "../../TreeHeaders/hltObjectTree.h"
#include "../../TreeHeaders/l1ObjectTree.h"
#include "../../TreeHeaders/hiEvtTree.h"
#include "../../TreeHeaders/jetSkimTree.h"
#include "../../TreeHeaders/trackSkimTree.h"
#include "../../TreeHeaders/eventSkimTree.h"
#include "../../TreeHeaders/mixEventSkimTree.h"
#include "../../TreeHeaders/hiFJRhoTree.h"
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
#include "../../Corrections/muons/Run2/RoccoR.cc"
#include "../../Corrections/electrons/2017pp/EnergyScaleCorrection.cc"
#include "../../Corrections/tracks/2015/getTrkCorr.h"
#include "../../Corrections/tracks/2017pp/trackingEfficiency2017pp.h"
#include "../../Corrections/tracks/2018PbPb/trackingEfficiency2018PbPb.h"

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

int applyWeightsV;
int applyTrkWeights;
bool rotateEvtPlane;

int nPthatWeights;

/// configuration variables - END

std::vector<std::string> argOptions;
///// global variables - END

int readConfiguration(std::string configFile, std::string inputFile);
void printConfiguration();
int parseRecoObj(std::string recoObjStr);
std::string parseVersionResidualTrkW(std::string path);
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
    bool isPP13tev = isPP && (toLowerCase(sampleType).find("13tev") != std::string::npos);

    bool isMCMG = (isMC && isPbPb18 && (outputFile.find("mg5") != std::string::npos ||
                                        outputFile.find("tt_jets") != std::string::npos ||
                                        outputFile.find("wjets") != std::string::npos));

    std::cout << "isMC = " << isMC << std::endl;
    std::cout << "isPbPb15 = " << isPbPb15 << std::endl;
    std::cout << "isPbPb18 = " << isPbPb18 << std::endl;
    std::cout << "isPP15 = " << isPP15 << std::endl;
    std::cout << "isPP17 = " << isPP17 << std::endl;
    std::cout << "isMCMG = " << isMCMG << std::endl;

    int collisionType = -1;
    if (isPbPb15) {
        collisionType = (isMC) ? COLL::TYPE::kHIMC : COLL::TYPE::kHI;
    }
    else if (isPbPb18) {
        collisionType = (isMC) ? COLL::TYPE::kHIMC2018 : COLL::TYPE::kHI2018;
    }
    else if (isPP15) {
        collisionType = (isMC) ? COLL::TYPE::kPPMC : COLL::TYPE::kPP;
    }
    else if (isPP17) {
        collisionType = (isMC) ? COLL::TYPE::kPPMC2017 : COLL::TYPE::kPP2017;
    }

    std::string anaMode = (ArgumentParser::optionExists("--anaMode", argOptions)) ?
            ArgumentParser::ParseOptionInputSingle("--anaMode", argOptions).c_str() : "v_trk";

    std::string sysMode = (ArgumentParser::optionExists("--sysMode", argOptions)) ?
            ArgumentParser::ParseOptionInputSingle("--sysMode", argOptions).c_str() : "";

    double evtFrac = (ArgumentParser::optionExists("--evtFrac", argOptions)) ?
            std::atof(ArgumentParser::ParseOptionInputSingle("--evtFrac", argOptions).c_str()) : 1;

    int rndEntryStart = (ArgumentParser::optionExists("--rndEntryStart", argOptions)) ?
            std::atoi(ArgumentParser::ParseOptionInputSingle("--rndEntryStart", argOptions).c_str()) : 0;
    bool doRndEntryStart = (rndEntryStart > 0);

    int rndEntry = (ArgumentParser::optionExists("--rndEntry", argOptions)) ?
            std::atoi(ArgumentParser::ParseOptionInputSingle("--rndEntry", argOptions).c_str()) : 0;
    bool doRndEntry = (rndEntry > 0);

    std::string vType = (ArgumentParser::optionExists("--vType", argOptions)) ?
            ArgumentParser::ParseOptionInputSingle("--vType", argOptions).c_str() : "pho";
    std::string vRG = (ArgumentParser::optionExists("--vRG", argOptions)) ?
            ArgumentParser::ParseOptionInputSingle("--vRG", argOptions).c_str() : "r";

    std::string vPtRangesStr = (ArgumentParser::optionExists("--vPts", argOptions)) ?
            ArgumentParser::ParseOptionInputSingle("--vPts", argOptions).c_str() : "40:-1,60:-1";
    std::vector<double> vPtsMin = parseRangesMin(vPtRangesStr);
    std::vector<double> vPtsMax = parseRangesMax(vPtRangesStr);
    int nVPts = vPtsMin.size();

    double vYMin = (ArgumentParser::optionExists("--vYMin", argOptions)) ?
            std::atof(ArgumentParser::ParseOptionInputSingle("--vYMin", argOptions).c_str()) : 0;
    double vYMax = (ArgumentParser::optionExists("--vYMax", argOptions)) ?
                std::atof(ArgumentParser::ParseOptionInputSingle("--vYMax", argOptions).c_str()) : 1.44;

    double lEtaMax = (ArgumentParser::optionExists("--lEtaMax", argOptions)) ?
                    std::atof(ArgumentParser::ParseOptionInputSingle("--lEtaMax", argOptions).c_str()) : -1;

    double lPtScale = (ArgumentParser::optionExists("--lPtScale", argOptions)) ?
                    std::atof(ArgumentParser::ParseOptionInputSingle("--lPtScale", argOptions).c_str()) : 1.0;

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

    double vTrkDetaMin = (ArgumentParser::optionExists("--vTrkDetaMin", argOptions)) ?
                std::atof(ArgumentParser::ParseOptionInputSingle("--vTrkDetaMin", argOptions).c_str()) : 0;
    double vTrkDetaMax = (ArgumentParser::optionExists("--vTrkDetaMax", argOptions)) ?
                    std::atof(ArgumentParser::ParseOptionInputSingle("--vTrkDetaMax", argOptions).c_str()) : -1;

    // deta between trk and leading hadron
    double hTrkDetaMin = (ArgumentParser::optionExists("--hTrkDetaMin", argOptions)) ?
                std::atof(ArgumentParser::ParseOptionInputSingle("--hTrkDetaMin", argOptions).c_str()) : 0;
    double hTrkDetaMax = (ArgumentParser::optionExists("--hTrkDetaMax", argOptions)) ?
                    std::atof(ArgumentParser::ParseOptionInputSingle("--hTrkDetaMax", argOptions).c_str()) : -1;

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

    int nmixMax = (ArgumentParser::optionExists("--nmixMax", argOptions)) ?
            std::atoi(ArgumentParser::ParseOptionInputSingle("--nmixMax", argOptions).c_str()) : 999999;
    int nmixMin = (ArgumentParser::optionExists("--nmixMin", argOptions)) ?
            std::atoi(ArgumentParser::ParseOptionInputSingle("--nmixMin", argOptions).c_str()) : 1;

    int rndVPhi = (ArgumentParser::optionExists("--rndVPhi", argOptions)) ?
            std::atoi(ArgumentParser::ParseOptionInputSingle("--rndVPhi", argOptions).c_str()) : 0;
    bool doRndVPhi = (rndVPhi > 0);

    int genVPtTmp = (ArgumentParser::optionExists("--genVPt", argOptions)) ?
            std::atoi(ArgumentParser::ParseOptionInputSingle("--genVPt", argOptions).c_str()) : 0;
    bool useGenVPt = (genVPtTmp > 0);

    int genVPhiTmp = (ArgumentParser::optionExists("--genVPhi", argOptions)) ?
            std::atoi(ArgumentParser::ParseOptionInputSingle("--genVPhi", argOptions).c_str()) : 0;
    bool useGenVPhi = (genVPhiTmp > 0);

    std::cout << "anaMode = " << anaMode << std::endl;
    std::cout << "sysMode = " << sysMode << std::endl;
    std::cout << "evtFrac = " << evtFrac << std::endl;
    if (doRndEntryStart) {
        std::cout << "rndEntryStart = " << rndEntryStart << std::endl;
    }
    if (doRndEntry) {
        std::cout << "rndEntry = " << rndEntry << std::endl;
    }
    std::cout << "vType = " << vType << std::endl;
    std::cout << "vRG = " << vRG << std::endl;
    std::cout << "nVPts = " << nVPts << std::endl;
    for (int i = 0; i < nVPts; ++i) {
        std::cout << Form("vPts[%d] = [%f, %f)", i, vPtsMin[i], vPtsMax[i]) << std::endl;
    }
    std::cout << "vYMin = " << vYMin << std::endl;
    std::cout << "vYMax = " << vYMax << std::endl;

    std::cout << "lEtaMax = " << lEtaMax << std::endl;
    std::cout << "lPtScale = " << lPtScale << std::endl;

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
    std::cout << "vTrkDetaMin = " << vTrkDetaMin << std::endl;
    std::cout << "vTrkDetaMax = " << vTrkDetaMax << std::endl;

    std::cout << "hTrkDetaMin = " << hTrkDetaMin << std::endl;
    std::cout << "hTrkDetaMax = " << hTrkDetaMax << std::endl;

    std::cout << "nCents = " << nCents << std::endl;
    for (int i = 0; i < nCents; ++i) {
        std::cout << Form("cents[%d] = [%d, %d)", i, centsMin[i], centsMax[i]) << std::endl;
    }

    std::cout << "maxNVtx = " << maxNVtx << std::endl;
    std::cout << "minNVtx = " << minNVtx << std::endl;
    if (nmixMax != 999999) {
        std::cout << "nmixMax = " << nmixMax << std::endl;
    }
    if (nmixMin != 1) {
        std::cout << "nmixMin = " << nmixMin << std::endl;
    }
    if (doRndVPhi) {
        std::cout << "rndVPhi = " << rndVPhi << std::endl;
    }
    if (useGenVPt) {
        std::cout << "genVPt = " << genVPtTmp << std::endl;
    }
    if (useGenVPhi) {
        std::cout << "genVPhi = " << genVPhiTmp << std::endl;
    }

    std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.c_str());
    std::cout<<"input ROOT files : num = "<<inputFiles.size()<< std::endl;
    std::cout<<"#####"<< std::endl;
    for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
        std::cout<<(*it).c_str()<< std::endl;
    }
    std::cout<<"##### END #####"<< std::endl;

    bool anaV = (toLowerCase(anaMode).find("v") == 0);
    bool anaJets = (toLowerCase(anaMode).find("jet") != std::string::npos && treePathJetSkim.size() != 0);
    bool anaTrks = (toLowerCase(anaMode).find("trk") != std::string::npos);
    bool anaNVtx = (isPP || (maxNVtx > 0 || minNVtx > -1));

    bool vIsPho = (toLowerCase(vType).find("pho") == 0);
    bool vIsZmm = (toLowerCase(vType).find("zmm") == 0);
    bool vIsZee = (toLowerCase(vType).find("zee") == 0);
    bool vIsZ = vIsZmm || vIsZee;
    bool vIsSS = (vIsZ && toLowerCase(vType).find("_ss") != std::string::npos);

    bool vIsZtautau_mm = (vIsZmm && toLowerCase(vType).find("tt") != std::string::npos);    // tt = tau+tau
    bool vIsZtautau_ee = (vIsZee && toLowerCase(vType).find("tt") != std::string::npos);

    bool isRecoV = isRecoObj(vRG);
    bool isRecoJet = isRecoObj(jetRG);
    bool isRecoTrk = isRecoObj(trkRG);

    bool isBkgTrk = isBkgObj(trkRBS);
    bool isSigTrk = isSigObj(trkRBS);

    bool isMixTrk = isBkgTrk;

    bool findLeadingTrk = (hTrkDetaMin > 0 || hTrkDetaMax > 0);
    bool needRawTrk = (findLeadingTrk);
    if (isMixTrk && needRawTrk) {
        std::cout << "need raw event particle information as well." << std::endl;
    }

    bool anaTrkID = (anaTrks && isRecoTrk && toLowerCase(anaMode).find("trkid") != std::string::npos);
    bool anaTrkLep = (anaTrks && isRecoTrk && toLowerCase(anaMode).find("trklep") != std::string::npos);
    bool anavTrk_dR = false;
    bool anavTrk_zh = false;

    bool do_sf_weight_nom = (toLowerCase(sysMode).find("sf_weight_nom") != std::string::npos);
    bool do_sys_les = (toLowerCase(sysMode).find("_les") != std::string::npos);
    int lep_sys_index = parseLepSysIndex(sysMode);

    bool doWeightsV = (applyWeightsV > 0);
    bool doWeightsEP = ((applyWeightsV % 10) == 2);
    bool doWeightsPhi0 = ((applyWeightsV % 10) == 3);
    bool doWeightsVcent = ((applyWeightsV > 10) && isPbPb18 && !isMC);

    bool noTrkWeights = (applyTrkWeights == 0);
    bool redoTrkWeights = ((applyTrkWeights % 10) == 2);
    bool doTrkPhiWeights = ((applyTrkWeights > 10) && isPbPb18 && !isMC && isRecoTrk);

    rotateEvtPlane = (rotateEvtPlane && isPbPb);

    if (lEtaMax < 0) {
        lEtaMax = (vIsZee) ? 2.1 : 2.4;
        std::cout << "lEtaMax is set to " << lEtaMax << std::endl;
    }

    bool doNVtxW = (outputFile.find("nVtxW") != std::string::npos && isPP13tev);

    bool doResidualMBTrkW = (outputFile.find("resMBTrkW") != std::string::npos && redoTrkWeights);
    std::cout << "doResidualMBTrkW = " << doResidualMBTrkW << std::endl;

    bool doEffDRW = (outputFile.find("effDRW") != std::string::npos && redoTrkWeights && anaJets);
    std::cout << "doEffDRW = " << doEffDRW << std::endl;

    bool shiftHibin = (isPbPb18 && isMC && anaTrks);
    if (shiftHibin) {
        std::cout << "shifting hiBin" << std::endl;
    }

    double minDR_lep_trk_PFID = 0.0;   // minDR_lep_trk if PF ID is available
    double minDR_lep_trk_noPFID = 0.2;
    std::cout << "minDR_lep_trk_PFID = " << minDR_lep_trk_PFID << std::endl;
    std::cout << "minDR_lep_trk_noPFID = " << minDR_lep_trk_noPFID << std::endl;

    double minDR2_lep_trk_PFID = minDR_lep_trk_PFID*minDR_lep_trk_PFID;
    double minDR2_lep_trk_noPFID = minDR_lep_trk_noPFID*minDR_lep_trk_noPFID;

    // initialize objects
    TH1::SetDefaultSumw2();

    TH1* hTmp = 0;

    TFile* fileWeightsV = 0;
    std::vector<TH2D*> vec_h2D_wV;
    if (doWeightsV) {
        std::string dirWV = "/export/d00/scratch/tatar/EWJTA-out/vJetTrk/zBoson/Histogramming/vReco/";
        std::string fileNameWV = "";
        if (vIsZmm && isPP17)  {
            fileNameWV = "vJetTrkAna_pp_2017_mc_zmm_vr_trk_r_raw_const.root";
        }
        else if (vIsZee && isPP17)  {
            fileNameWV = "vJetTrkAna_pp_2017_mc_zee_vr_trk_r_raw_const.root";
        }
        else if (vIsZmm && isPbPb18)  {
            fileNameWV = "vJetTrkAna_pbpb_2018_mc_zmm_vr_trk_r_raw_const.root";
        }
        else if (vIsZee && isPbPb18)  {
            fileNameWV = "vJetTrkAna_pbpb_2018_mc_zee_vr_trk_r_raw_const.root";
        }

        if (fileNameWV != "") {
            std::string filePathWV = Form("%s/%s", dirWV.c_str(), fileNameWV.c_str());
            std::cout << "reading V correction file : " << filePathWV.c_str() << std::endl;

            fileWeightsV = TFile::Open(filePathWV.c_str(), "READ");

            std::vector<std::string> tmpCents = {"0_100"};
            if (isPbPb) {
                tmpCents = {"0_10", "10_30", "30_50", "50_90"};
            }

            for (std::vector<std::string>::iterator it = tmpCents.begin() ; it != tmpCents.end(); ++it) {

                std::string tmpPath = Form("h2_reco_eff_vPt_vs_vY_cent%s", (*it).c_str());
                vec_h2D_wV.push_back((TH2D*)fileWeightsV->Get(tmpPath.c_str()));
            }
        }
    }

    TFile* fileWeightsEP = 0;
    std::vector<TH2D*> vec_h2D_wEP;
    if (doWeightsEP) {
        std::string dirWEP = "/export/d00/scratch/tatar/EWJTA-out/vJetTrk/zBoson/Histogramming/vReco/wV/";
        std::string fileNameWEP = "";
        if (vIsZmm && isPbPb18)  {
            fileNameWEP = "vJetTrkAna_pbpb_2018_data_zmm_vr_trk_r_raw.root";
        }
        else if (vIsZee && isPbPb18)  {
            fileNameWEP = "vJetTrkAna_pbpb_2018_data_zee_vr_trk_r_raw.root";
        }

        if (fileNameWEP != "") {
            std::string filePathWEP = Form("%s/%s", dirWEP.c_str(), fileNameWEP.c_str());
            std::cout << "reading EP correction file : " << filePathWEP.c_str() << std::endl;

            fileWeightsEP = TFile::Open(filePathWEP.c_str(), "READ");

            std::vector<std::string> tmpCents = {"0_10", "10_30", "30_50", "50_90"};
            //std::vector<std::string> tmpCents = {"0_30", "30_90"};

            for (std::vector<std::string>::iterator it = tmpCents.begin() ; it != tmpCents.end(); ++it) {

                std::string tmpPath = Form("h2_dphi_EPn2_V_vs_vPt_cent%s", (*it).c_str());
                vec_h2D_wEP.push_back((TH2D*)fileWeightsEP->Get(tmpPath.c_str()));
            }
        }
    }

    TFile* fileWeightsPhi0 = 0;
    std::vector<TH1D*> vec_h1D_wPhi0;
    if (doWeightsPhi0) {
        std::string dirWPhi0 = "/export/d00/scratch/tatar/EWJTA-out/vJetTrk/zBoson/Histogramming/test/weights/phi0/";
        std::string fileNameWPhi0 = "";
        std::string tmpStr_mc = (isMCMG) ? "_mg5" : "";
        std::string tmpStr_zll = (vIsZmm) ? "zmm" : "zee";
        std::string tmpStr_vRG = (isRecoV) ? "vr" : "vg";
        fileNameWPhi0 = Form("vJetTrkAna_pbpb_2018_mc%s_%s_%s_trk_g_raw.root", tmpStr_mc.c_str(), tmpStr_zll.c_str(), tmpStr_vRG.c_str());

        if (fileNameWPhi0 != "") {
            std::string filePathWPhi0 = Form("%s/%s", dirWPhi0.c_str(), fileNameWPhi0.c_str());
            std::cout << "reading Phi0 correction file : " << filePathWPhi0.c_str() << std::endl;

            fileWeightsPhi0 = TFile::Open(filePathWPhi0.c_str(), "READ");

            std::vector<std::string> tmpCents = {"0_10", "10_30", "30_50", "50_90"};

            for (std::vector<std::string>::iterator it = tmpCents.begin() ; it != tmpCents.end(); ++it) {

                std::string tmpPath = Form("h_dphi_phi0_V_vPt30_80_cent%s", (*it).c_str());
                vec_h1D_wPhi0.push_back((TH1D*)fileWeightsPhi0->Get(tmpPath.c_str()));
            }
        }
    }

    TFile* fileWeightsVcent1 = 0;
    TFile* fileWeightsVcent2 = 0;
    TH1D* h1D_wVcent1 = 0;
    TH1D* h1D_wVcent2 = 0;
    if (doWeightsVcent) {

        std::string dirWVcent = "/export/d00/scratch/tatar/EWJTA-out/vJetTrk/zBoson/Histogramming/vReco/wV/";
        //std::string filePathWVcent1 = Form("%s/%s", dirWVcent.c_str(), "vJetTrkAna_pbpb_2018_mc_zmm_vr_trk_r_raw.root");
        //std::string filePathWVcent2 = Form("%s/%s", dirWVcent.c_str(), "vJetTrkAna_pbpb_2018_mc_zee_vr_trk_r_raw.root");
        std::string filePathWVcent1 = Form("%s/%s", dirWVcent.c_str(), "vJetTrkAna_pbpb_2018_data_zmm_vr_trk_r_raw.root");
        std::string filePathWVcent2 = Form("%s/%s", dirWVcent.c_str(), "vJetTrkAna_pbpb_2018_data_zee_vr_trk_r_raw.root");

        std::cout << "reading V cent correction files : " << std::endl;
        std::cout << "                                : " << filePathWVcent1.c_str() << std::endl;
        std::cout << "                                : " << filePathWVcent2.c_str() << std::endl;

        fileWeightsVcent1 = TFile::Open(filePathWVcent1.c_str(), "READ");
        fileWeightsVcent2 = TFile::Open(filePathWVcent2.c_str(), "READ");

        h1D_wVcent1 = 0;
        h1D_wVcent1 = (TH1D*)fileWeightsVcent1->Get("h_halfHiBin_vPt30_0");

        h1D_wVcent2 = 0;
        h1D_wVcent2 = (TH1D*)fileWeightsVcent2->Get("h_halfHiBin_vPt30_0");

        h1D_wVcent1->Divide(h1D_wVcent2);
    }

    TFile* fileWeights_nVtx1 = 0;
    TFile* fileWeights_nVtx2 = 0;
    TH1D* h1D_wV_nVtx1 = 0;
    TH1D* h1D_wV_nVtx2 = 0;
    if (doNVtxW) {

        std::string dirWVnVtx = "/export/d00/scratch/tatar/EWJTA-out/vJetTrk/zBoson/Histogramming/pp13tev/vReco/";
        std::string filePathWVnVtx1 = Form("%s/%s", dirWVnVtx.c_str(), "vJetTrkAna_pp_2017f_data_zmm_vr_trk_r_raw_const.root");
        std::string filePathWVnVtx2 = Form("%s/%s", dirWVnVtx.c_str(), "vJetTrkAna_pp_2018_data_zmm_vr_trk_r_raw_const.root");

        std::cout << "reading V nVtx correction files : " << std::endl;
        std::cout << "                                : " << filePathWVnVtx1.c_str() << std::endl;
        std::cout << "                                : " << filePathWVnVtx2.c_str() << std::endl;

        fileWeights_nVtx1 = TFile::Open(filePathWVnVtx1.c_str(), "READ");
        fileWeights_nVtx2 = TFile::Open(filePathWVnVtx2.c_str(), "READ");

        std::string hName_nVtx = "h_nVtx_vPt10_0_cent0_100";

        h1D_wV_nVtx1 = 0;
        h1D_wV_nVtx1 = (TH1D*)fileWeights_nVtx1->Get(hName_nVtx.c_str());
        h1D_wV_nVtx1->Scale(1./h1D_wV_nVtx1->Integral());

        h1D_wV_nVtx2 = 0;
        h1D_wV_nVtx2 = (TH1D*)fileWeights_nVtx2->Get(hName_nVtx.c_str());
        h1D_wV_nVtx2->Scale(1./h1D_wV_nVtx2->Integral());

        h1D_wV_nVtx1->Divide(h1D_wV_nVtx2);
    }

    // lepton corrections
    std::string muonCorrFile = (isPP) ? "RoccoR2017.txt" : "RoccoR2018.txt";
    muonCorrFile = "Corrections/muons/Run2/"+muonCorrFile;
    std::cout << "reading muon correction file : " << muonCorrFile << std::endl;
    RoccoR rc(muonCorrFile);

    EnergyScaleCorrection ec("Corrections/electrons/2017pp/data/Run2017_LowPU_v2", EnergyScaleCorrection::ECALELF);

    // tracking corrections
    TrkEff2018PbPb trkEff2018 =  TrkEff2018PbPb("general", false, "Corrections/tracks/2018PbPb/");

    TrkEff2017pp trkEff2017 =  TrkEff2017pp(false, "Corrections/tracks/2017pp/");

    TrkCorr* trkCorr2015 = 0;
    if (redoTrkWeights) {
        if (isPP15) {
            trkCorr2015 = new TrkCorr("Corrections/tracks/2015/TrkCorr_July22_Iterative_pp_eta2p4/");
        }
        else if (isPbPb15) {
            trkCorr2015 = new TrkCorr("Corrections/tracks/2015/TrkCorr_Jun7_Iterative_PbPb_etaLT2p4/");
        }
    }

    TFile* fileTrkPhiWeights = 0;

    std::vector<double> trkPhiWPts = getVecPt4TrkW();
    std::vector<double> trkPhiWEtas = getVecEta4TrkW();
    std::vector<int> trkPhiWCents = getVecCent4TrkW();
    int nTrkPhiWPt = trkPhiWPts.size() - 1;
    int nTrkPhiWEta = trkPhiWEtas.size();
    int nTrkPhiWCent = trkPhiWCents.size();

    TH1D* h_trkPhi_effcorr[nTrkPhiWPt][nTrkPhiWEta][nTrkPhiWCent];
    if (doTrkPhiWeights) {
        std::string dirTrkPhiW = "/export/d00/scratch/tatar/EWJTA-out/vJetTrk/zBoson/Histogramming/";
        std::string fileNameTrkPhiW = "trkCalc_pbpb_2018_data_mix.root";

        if (fileNameTrkPhiW != "") {
            std::string filePathTrkPhiW = Form("%s/%s", dirTrkPhiW.c_str(), fileNameTrkPhiW.c_str());
            std::cout << "reading trkPhi correction file : " << fileNameTrkPhiW.c_str() << std::endl;

            fileTrkPhiWeights = TFile::Open(filePathTrkPhiW.c_str(), "READ");

            for (int i = 0; i < nTrkPhiWPt; ++i) {
                for (int j = 0; j < nTrkPhiWEta; ++j) {
                    for (int k = 0; k < nTrkPhiWCent; ++k) {
                        std::string name_h_suffix = Form("iPt_%d_iEta_%d_iCent_%d", i, j, k);
                        std::string name_h = Form("h_trkPhi_%s_effcorr", name_h_suffix.c_str());

                        h_trkPhi_effcorr[i][j][k] = 0;
                        h_trkPhi_effcorr[i][j][k] = (TH1D*)fileTrkPhiWeights->Get(name_h.c_str());
                    }
                }
            }
        }
    }

    TFile* fileResMBTrkWeights = 0;

    std::vector<double> resMBTrkWPts = getVecPt4TrkWCoarse();
    std::vector<int> resMBTrkWCents = getVecCent4TrkWCoarse();
    int nResMBTrkWPt = resMBTrkWPts.size() - 1;
    int nResMBTrkWCent = resMBTrkWCents.size();

    TH2D* h_resMBTrk_effcorr[nResMBTrkWPt][nResMBTrkWCent];
    if (doResidualMBTrkW) {
        std::string dirResMBTrkW = "/export/d00/scratch/tatar/EWJTA-out/vJetTrk/zBoson/Histogramming/";
        std::string versionResTrkW = parseVersionResidualTrkW(outputFile);
        if (versionResTrkW.size() > 0) {
            versionResTrkW = "_"+versionResTrkW;
        }

        std::string fileNameResMBTrkW = Form("trkCalc_pbpb_2018_mc_mix%s.root", versionResTrkW.c_str());
        if (!isMixTrk) {
            fileNameResMBTrkW = Form("trkCalc_pbpb_2018_mc_zmm%s.root", versionResTrkW.c_str());
            if (vIsZee) {
                fileNameResMBTrkW = Form("trkCalc_pbpb_2018_mc_mg5%s.root", versionResTrkW.c_str());
            }

            if (outputFile.find("res_zmm") != std::string::npos) {
                fileNameResMBTrkW = Form("trkCalc_pbpb_2018_mc_zmm%s.root", versionResTrkW.c_str());
            }
            else if (outputFile.find("res_mg5") != std::string::npos) {
                fileNameResMBTrkW = Form("trkCalc_pbpb_2018_mc_mg5%s.root", versionResTrkW.c_str());
            }
            else if (outputFile.find("res_zee") != std::string::npos) {
                fileNameResMBTrkW = Form("trkCalc_pbpb_2018_mc_zee%s.root", versionResTrkW.c_str());
            }
        }

        if (fileNameResMBTrkW != "") {
            std::string filePathResMBTrkW = Form("%s/%s", dirResMBTrkW.c_str(), fileNameResMBTrkW.c_str());
            std::cout << "reading ResMBTrk correction file : " << fileNameResMBTrkW.c_str() << std::endl;

            fileResMBTrkWeights = TFile::Open(filePathResMBTrkW.c_str(), "READ");

            for (int i = 0; i < nResMBTrkWPt; ++i) {
                for (int k = 0; k < nResMBTrkWCent; ++k) {
                    std::string name_h_suffix = Form("iPt_%d_iCent_%d", i, k);
                    std::string name_h = Form("h2_phi_vs_eta_gen_trkCorr_ratio_%s", name_h_suffix.c_str());
                    if (outputFile.find("resMBTrkWeff") != std::string::npos) {
                        name_h = Form("h2_phi_vs_eta_gen_trkEffCorr_ratio_%s", name_h_suffix.c_str());
                    }

                    h_resMBTrk_effcorr[i][k] = 0;
                    h_resMBTrk_effcorr[i][k] = (TH2D*)fileResMBTrkWeights->Get(name_h.c_str());
                }
            }
        }
    }

    TFile* fileEffDRW[2][2];

    //std::vector<int> effDRWPts = {1, 2, 3, 4, 8, 12, 20};
    //std::vector<double> effDRWPts = {0.5, 1, 1.5, 2, 3, 4, 8, 12, 20, 60};
    //std::vector<double> effDRWPts = {0.5, 1, 1.2, 2, 3, 4, 8, 12, 20, 60};
    //std::vector<double> effDRWPts = {0.5, 1, 1.3, 2, 3, 4, 8, 12, 20, 60};
    //std::vector<double> effDRWPts = {0.5, 1, 2, 3, 4, 8, 12, 20, 60};
    std::vector<double> effDRWPts = {1, 1.3, 2, 3, 4, 8, 12, 20, 60};
    //std::vector<double> effDRWPts = {1, 2, 3, 4, 8, 12, 20, 60};
    std::vector<int> effDRWCents = {0, 10, 30, 50, 90};
    //std::vector<int> effDRWCents = {0, 10, 30};
    int nEffDRWPt = effDRWPts.size() - 1;
    int nEffDRWCent = effDRWCents.size() - 1;

    double effDR_min_jetpt = 50;
    //double effDR_min_jetpt = 70;
    double mindR2_jet_trk_0 = 999999;
    //double maxdR2_jet_l = 0.16;
    double maxdR2_jet_l = -1;

    TH1D* h_effDR[3][nEffDRWPt][nEffDRWCent][2];
    TH2D* h2_effDR[3][nEffDRWPt][nEffDRWCent][2];
    bool effDR_integrateDR = false;
    int nEffDRWeta = 1;
    if (doEffDRW) {
        //std::string dirEffDRW_gen = "/export/d00/scratch/tatar/EWJTA-out/vJetTrk/zBoson/Histogramming/nMixBins_800_15_1_PFHFtotE_m682/effdRv4/";
        //std::string dirEffDRW_gen = "/export/d00/scratch/tatar/EWJTA-out/vJetTrk/zBoson/Histogramming/nMixBins_800_15_1_PFHFtotE_m682/effdRv5/";
        //std::string dirEffDRW_gen = "/export/d00/scratch/tatar/EWJTA-out/vJetTrk/zBoson/Histogramming/nMixBins_800_15_1_PFHFtotE_m682/effdRv5/vTrkDphiMin0p875/";
        //std::string dirEffDRW_gen = "/export/d00/scratch/tatar/EWJTA-out/vJetTrk/zBoson/Histogramming/nMixBins_800_15_1_PFHFtotE_m682/effdRv7/vTrkDphiMin0p875/";
        //std::string dirEffDRW_gen = "/export/d00/scratch/tatar/EWJTA-out/vJetTrk/zBoson/Histogramming/nMixBins_800_15_1_PFHFtotE_m682/effdRv20/vTrkDphiMin0p5/";
        //std::string dirEffDRW_gen = "/export/d00/scratch/tatar/EWJTA-out/vJetTrk/zBoson/Histogramming/nMixBins_800_15_1_PFHFtotE_m682/effdRv20/vTrkDphiMin0p875/";
        //std::string dirEffDRW_gen = "/export/d00/scratch/tatar/EWJTA-out/vJetTrk/zBoson/Histogramming/nMixBins_800_15_1_PFHFtotE_m682/effdRv22/vTrkDphiMin0p5/";
        //std::string dirEffDRW_gen = "/export/d00/scratch/tatar/EWJTA-out/vJetTrk/zBoson/Histogramming/nMixBins_800_15_1_PFHFtotE_m682/effdRv24/vTrkDphiMin0p5/";
        //std::string dirEffDRW_gen = "/export/d00/scratch/tatar/EWJTA-out/vJetTrk/zBoson/Histogramming/nMixBins_800_15_1_PFHFtotE_m682/effdRv25/vTrkDphiMin0p5/";
        //std::string dirEffDRW_gen = "/export/d00/scratch/tatar/EWJTA-out/vJetTrk/zBoson/Histogramming/nMixBins_800_15_1_PFHFtotE_m682/effdRv27/vTrkDphiMin0p5/";
        //std::string dirEffDRW_gen = "/export/d00/scratch/tatar/EWJTA-out/vJetTrk/zBoson/Histogramming/nMixBins_800_15_1_PFHFtotE_m682/effdRv30/vTrkDphiMin0p0/";
        //std::string dirEffDRW_gen = "/export/d00/scratch/tatar/EWJTA-out/vJetTrk/zBoson/Histogramming/nMixBins_800_15_1_PFHFtotE_m682/effdRv31/vTrkDphiMin0p0/";
        //std::string dirEffDRW_gen = "/export/d00/scratch/tatar/EWJTA-out/vJetTrk/zBoson/Histogramming/nMixBins_800_15_1_PFHFtotE_m682/effdRv32/vTrkDphiMin0p0/";
        std::string dirEffDRW_gen = "/export/d00/scratch/tatar/EWJTA-out/vJetTrk/zBoson/Histogramming/nMixBins_800_15_1_PFHFtotE_m682/effdRv33/vTrkDphiMin0p0/";
        //std::string dirEffDRW_gen_etaBin1 = dirEffDRW_gen;

        std::string fileNameEffDRW_gen = Form("vJetTrkCalc_pbpb_2018_mc_zmm_vr_trk_g_sig.root");
        std::string fileNameEffDRW_reco = Form("vJetTrkCalc_pbpb_2018_mc_zmm_vr_trk_r_bkgsub.root");

        if (nEffDRWeta == 1) {

            std::string filePathEffDRW_gen = Form("%s/%s", dirEffDRW_gen.c_str(), fileNameEffDRW_gen.c_str());

            std::string dirEffDRW_reco = dirEffDRW_gen+"trkgetEff/resMBTrkWeffv8/";
            std::string filePathEffDRW_reco = Form("%s/%s", dirEffDRW_reco.c_str(), fileNameEffDRW_reco.c_str());

            std::cout << "reading effDR correction files - gen  : " << filePathEffDRW_gen.c_str() << std::endl;
            std::cout << "reading effDR correction files - reco : " << filePathEffDRW_reco.c_str() << std::endl;

            fileEffDRW[0][0] = TFile::Open(filePathEffDRW_gen.c_str(), "READ");
            fileEffDRW[1][0] = TFile::Open(filePathEffDRW_reco.c_str(), "READ");
        }
        else if (nEffDRWeta == 2) {
            //std::string dirEffDRW_gen = "/export/d00/scratch/tatar/EWJTA-out/vJetTrk/zBoson/Histogramming/nMixBins_800_15_1_PFHFtotE_m682/effdRv22/vTrkDphiMin0p875/";
            //std::string dirEffDRW_gen = "/export/d00/scratch/tatar/EWJTA-out/vJetTrk/zBoson/Histogramming/nMixBins_800_15_1_PFHFtotE_m682/effdRv23/vTrkDphiMin0p5/";
            std::string dirEffDRW_gen_etaBin1 = dirEffDRW_gen+"trkEtaMax1p0/";
            std::string dirEffDRW_gen_etaBin2 = dirEffDRW_gen+"trkEtaMin1p0Max2p4/";

            //std::string dirEffDRW_reco_etaBin1 = dirEffDRW_gen_etaBin1+"trkgetEff/resMBTrkWeffv7/";
            //std::string dirEffDRW_reco_etaBin2 = dirEffDRW_gen_etaBin2+"trkgetEff/resMBTrkWeffv7/";
            std::string dirEffDRW_reco_etaBin1 = dirEffDRW_gen_etaBin1+"trkgetEff/resMBTrkWeffv8/";
            std::string dirEffDRW_reco_etaBin2 = dirEffDRW_gen_etaBin2+"trkgetEff/resMBTrkWeffv8/";

            std::string filePathEffDRW_gen_etaBin1 = Form("%s/%s", dirEffDRW_gen_etaBin1.c_str(), fileNameEffDRW_gen.c_str());
            std::string filePathEffDRW_reco_etaBin1 = Form("%s/%s", dirEffDRW_reco_etaBin1.c_str(), fileNameEffDRW_reco.c_str());

            std::string filePathEffDRW_gen_etaBin2 = Form("%s/%s", dirEffDRW_gen_etaBin2.c_str(), fileNameEffDRW_gen.c_str());
            std::string filePathEffDRW_reco_etaBin2 = Form("%s/%s", dirEffDRW_reco_etaBin2.c_str(), fileNameEffDRW_reco.c_str());

            std::cout << "reading effDR correction files - gen  etaBin1 : " << filePathEffDRW_gen_etaBin1.c_str() << std::endl;
            std::cout << "reading effDR correction files - reco etaBin1 : " << filePathEffDRW_reco_etaBin1.c_str() << std::endl;

            std::cout << "reading effDR correction files - gen  etaBin2 : " << filePathEffDRW_gen_etaBin2.c_str() << std::endl;
            std::cout << "reading effDR correction files - reco etaBin2 : " << filePathEffDRW_reco_etaBin2.c_str() << std::endl;

            fileEffDRW[0][0] = TFile::Open(filePathEffDRW_gen_etaBin1.c_str(), "READ");
            fileEffDRW[1][0] = TFile::Open(filePathEffDRW_reco_etaBin1.c_str(), "READ");

            fileEffDRW[0][1] = TFile::Open(filePathEffDRW_gen_etaBin2.c_str(), "READ");
            fileEffDRW[1][1] = TFile::Open(filePathEffDRW_reco_etaBin2.c_str(), "READ");
        }

        std::cout << "effDR_min_jetpt = " << effDR_min_jetpt << std::endl;
        std::cout << "mindR2_jet_trk_0 = " << mindR2_jet_trk_0 << std::endl;

        for (int iF = 0; iF < 3; ++iF) {
            for (int iPt = 0; iPt < nEffDRWPt; ++iPt) {
                for (int iC = 0; iC < nEffDRWCent; ++iC) {
                    for (int iEta = 0; iEta < nEffDRWeta; ++iEta) {

                        h_effDR[iF][iPt][iC][iEta] = 0;
                        h2_effDR[iF][iPt][iC][iEta] = 0;

                        double tmpPt1 = effDRWPts[iPt];
                        double tmpPt2 = effDRWPts[iPt+1];

                        std::string label_trkPt1 = Form("%d", (int)(tmpPt1));    // 5 --> "5"
                        if (std::floor(tmpPt1) != tmpPt1) {   // 1.4 --> "1p4"
                            label_trkPt1 = Form("%dp%d", (int)(tmpPt1), ((int)(tmpPt1*10) % 10));
                        }

                        std::string label_trkPt2 = Form("%d", (int)(tmpPt2));    // 5 --> "5"
                        if (std::floor(tmpPt2) != tmpPt2) {   // 1.4 --> "1p4"
                            label_trkPt2 = Form("%dp%d", (int)(tmpPt2), ((int)(tmpPt2*10) % 10));
                        }

                        std::string name_h_suffix_trkPt = Form("trkPt%s_%s", label_trkPt1.c_str(), label_trkPt2.c_str());
                        std::string name_h_suffix_cent = Form("cent%d_%d", effDRWCents[iC], effDRWCents[iC+1]);

                        std::string name_h = "";
                        //name_h = Form("h2_rawpt_vs_xivh_vPt30_0_%s_%s_projY_sig", name_h_suffix_trkPt.c_str(), name_h_suffix_cent.c_str());
                        //name_h = Form("h2_rawpt_vs_dR_vPt30_0_%s_%s_sig", name_h_suffix_trkPt.c_str(), name_h_suffix_cent.c_str());
                        //name_h = Form("h2_rawpt_vs_dR_rebin_vPt30_0_%s_%s_sig", name_h_suffix_trkPt.c_str(), name_h_suffix_cent.c_str());
                        //name_h = Form("h2_jetpt_vs_dR_rebin_vPt30_0_%s_%s_sig", name_h_suffix_trkPt.c_str(), name_h_suffix_cent.c_str());
                        name_h = Form("h2_jetpt_vs_dR_rebin_vPt30_0_%s_%s_projY_sig", name_h_suffix_trkPt.c_str(), name_h_suffix_cent.c_str());

                        if (iF < 2) {
                            //hTmp = (TH1D*)fileEffDRW[iF][iEta]->Get(name_h.c_str());
                            //h_effDR[iF][iPt][iC][iEta]->Rebin(2);
                            //h_effDR[iF][iPt][iC][iEta]->Rebin(6);

                            //hTmp = (TH2D*)fileEffDRW[iF][iEta]->Get(name_h.c_str());

                            //std::vector<double> binsX = {0, 10, 30, 50, 100, 200, 300};
                            //std::vector<double> binsX_pt = {0, 10, 20, 30, 40, 50, 70, 100, 150, 200, 300};
                            //int nBinsX_pt = binsX_pt.size()-1;

                            //double arr_pt[nBinsX_pt+1];
                            //std::copy(binsX_pt.begin(), binsX_pt.end(), arr_pt);

                            //name_h = (iF == 0) ? replaceFirst(name_h, "h2_", "h2_num_rebin_") : replaceFirst(name_h, "h2_", "h2_denom_rebin_");

                            //h_effDR[iF][iPt][iC][iEta] = (TH1D*)hTmp->Rebin(nBinsX_pt, name_h.c_str(), arr_pt);

                            /*
                            h2_effDR[iF][iPt][iC][iEta] = new TH2D(name_h.c_str(), Form("%s:%s:%s", hTmp->GetTitle(), hTmp->GetXaxis()->GetTitle(), hTmp->GetYaxis()->GetTitle()),
                                                                   5, 0, 1, nBinsX_pt, arr_pt);  //hTmp->Rebin(nBinsX_pt, name_h.c_str(), arr_pt);
                            addBinContents((TH2D*)hTmp, h2_effDR[iF][iPt][iC][iEta]);
                            */

                            //h2_effDR[iF][iPt][iC][iEta] = (TH2D*)fileEffDRW[iF][iEta]->Get(name_h.c_str());
                            h_effDR[iF][iPt][iC][iEta] = (TH1D*)fileEffDRW[iF][iEta]->Get(name_h.c_str());
                            if (effDR_integrateDR) {
                                //h_effDR[iF][iPt][iC][iEta] = (TH1D*)h2_effDR[iF][iPt][iC][iEta]->ProjectionY(Form("%s_intDR", name_h.c_str()), 1, 5); // dR1p0
                                //h_effDR[iF][iPt][iC][iEta] = (TH1D*)h2_effDR[iF][iPt][iC][iEta]->ProjectionY(Form("%s_intDR", name_h.c_str()), 1, 2); // dR0p4
                                h_effDR[iF][iPt][iC][iEta] = (TH1D*)h2_effDR[iF][iPt][iC][iEta]->ProjectionY(Form("%s_intDR", name_h.c_str()), 1, 3); // dR0p6
                            }
                        }
                        else {
                            h_effDR[2][iPt][iC][iEta] = (TH1D*)h_effDR[0][iPt][iC][iEta]->Clone(Form("%s_corrDR", name_h.c_str()));
                            h_effDR[2][iPt][iC][iEta]->Divide(h_effDR[1][iPt][iC][iEta]);
                            /*
                            h2_effDR[2][iPt][iC][iEta] = (TH2D*)h2_effDR[0][iPt][iC][iEta]->Clone(Form("%s_corrDR", name_h.c_str()));
                            h2_effDR[2][iPt][iC][iEta]->Divide(h2_effDR[1][iPt][iC][iEta]);
                            */

                            if (effDR_integrateDR) {
                                h_effDR[2][iPt][iC][iEta] = (TH1D*)h_effDR[0][iPt][iC][iEta]->Clone(Form("%s_corrDR", name_h.c_str()));
                                h_effDR[2][iPt][iC][iEta]->Divide(h_effDR[1][iPt][iC][iEta]);
                            }
                        }
                    }
                }
            }
        }
    }

    TFile* output = TFile::Open(outputFile.c_str(),"RECREATE");
    output->cd();

    std::vector<TH2D*> vec_h2D;
    std::vector<TH1*> vec_h_num;
    std::vector<TH1*> vec_h_denom;

    std::vector<TH1*> vec_h_ss;
    std::vector<TH1*> vec_h_os;

    TH1D* h_vPt[nCents];
    TH1D* h_vPt_ss[nCents];
    TH1D* h_vEta[nCents][nVPts];
    TH1D* h_vPhi[nCents][nVPts];
    TH1D* h_vY[nCents][nVPts];
    TH1D* h_vY_ss[nCents][nVPts];
    TH1D* h_vM_os[nCents][nVPts];
    TH1D* h_vM_ss[nCents][nVPts];
    TH1D* h_l1Pt[nCents][nVPts];
    TH1D* h_l1Eta[nCents][nVPts];
    TH1D* h_l1Phi[nCents][nVPts];
    TH1D* h_l1R9[nCents][nVPts];
    TH1D* h_l2Pt[nCents][nVPts];
    TH1D* h_l2Eta[nCents][nVPts];
    TH1D* h_l2Phi[nCents][nVPts];
    TH1D* h_l2R9[nCents][nVPts];
    TH1D* h_detall[nCents][nVPts];
    TH1D* h_dphill[nCents][nVPts];
    TH2D* h2_vPhi_vs_vY[nCents][nVPts];
    TH2D* h2_l1Phi_vs_l1Eta[nCents][nVPts];
    TH2D* h2_l2Phi_vs_l2Eta[nCents][nVPts];

    // trig eff
    TH1D* h_trig_num_vPt[nCents];
    TH1D* h_trig_denom_vPt[nCents];
    TH1D* h_trig_num_vY[nCents][nVPts];
    TH1D* h_trig_denom_vY[nCents][nVPts];

    // reco eff
    TH1D* h_reco_num_vPt[nCents];
    TH1D* h_reco_denom_vPt[nCents];
    TH1D* h_reco_num_vPhi[nCents][nVPts];
    TH1D* h_reco_denom_vPhi[nCents][nVPts];
    TH1D* h_reco_num_vY[nCents][nVPts];
    TH1D* h_reco_denom_vY[nCents][nVPts];
    TH1D* h_reco_num_cent[nVPts];
    TH1D* h_reco_denom_cent[nVPts];
    TH1D* h_reco_num_dphi_phi0_V[nCents][nVPts];
    TH1D* h_reco_denom_dphi_phi0_V[nCents][nVPts];
    TH1D* h_reco_num_dphi_EPn2_V[nCents][nVPts];
    TH1D* h_reco_denom_dphi_EPn2_V[nCents][nVPts];
    TH2D* h2_reco_num_vPt_vs_vY[nCents];
    TH2D* h2_reco_denom_vPt_vs_vY[nCents];

    // reco/gen diff
    TH2D* h2_rgVPt_ratio_vs_vPt[nCents];
    TH2D* h2_rgVPt_ratio_vs_vPtreco[nCents];
    TH2D* h2_rgVPhi_diff_vs_vPt[nCents];
    TH2D* h2_rgVPt_ratio_vs_cent[nVPts];
    TH2D* h2_rgVPt_ratio_vs_vY[nCents][nVPts];
    TH2D* h2_rgVPt_ratio_vs_vM[nCents][nVPts];
    TH2D* h2_rgVPt_ratio_vs_vMreco[nCents][nVPts];

    // event observables
    TH1D* h_cent[nVPts];
    TH1D* h_vtxz[nCents][nVPts];
    TH1D* h_nVtx[nCents][nVPts];
    TH1D* h_dphi_phi0_V[nCents][nVPts];
    TH1D* h_dphi_EPn1_V[nCents][nVPts];
    TH1D* h_dphi_EPn2_V[nCents][nVPts];
    TH1D* h_dphi_EPn3_V[nCents][nVPts];
    TH1D* h_dphi_EPn2_phi0[nCents][nVPts];
    TH1D* h_dphi_EPn2_l1[nCents][nVPts];
    TH1D* h_dphi_EPn2_l2[nCents][nVPts];
    TH2D* h2_hiHF_vs_vPt[nCents];
    TH2D* h2_rho_vs_vPt[nCents];
    TH2D* h2_PFHFtotE_vs_vPt[nCents];
    TH2D* h2_PFHFtotE_vs_vPt_rebin[nCents];
    TH2D* h2_PFHFtotE_vs_Npart[nCents][nVPts];
    TH2D* h2_PFHFtotEmix_vs_Npart[nCents][nVPts];
    TH2D* h2_PFHFtotE_vs_vY[nCents][nVPts];
    TH2D* h2_PFHFtotE_vs_nVtx[nCents][nVPts];
    TH2D* h2_PFHFtotE_diff_rawmix_vs_Npart[nCents][nVPts];
    TH2D* h2_PFHFtotE_eta3to4_vs_vPt[nCents];
    TH2D* h2_PFHFtotE_eta3to4_vs_vPt_rebin[nCents];
    TH2D* h2_PFHFtotE_eta4to5_vs_vPt[nCents];
    TH2D* h2_PFHFtotE_eta4to5_vs_vPt_rebin[nCents];
    TH2D* h2_PFHEtotE_vs_vPt[nCents];
    TH2D* h2_dphi_EPn2_V_vs_vPt[nCents];
    TH2D* h2_hiHF_vs_hiNpix[nVPts];
    TH2D* h2_PFHFtotE_vs_hiNpix[nVPts];

    TH2D* h2_nVtx_vs_run_13tev_pp17[nCents][nVPts];
    TH2D* h2_nVtx_vs_run_13tev_pp18[nCents][nVPts];

    TH1D* h_trkPt[nCents][nVPts];
    TH1D* h_trkEta[nCents][nVPts][nTrkPts];
    TH1D* h_trkPhi[nCents][nVPts][nTrkPts];
    TH1D* h_dphi_EPn1_trk[nCents][nVPts][nTrkPts];
    TH1D* h_dphi_EPn2_trk[nCents][nVPts][nTrkPts];
    TH1D* h_dphi_EPn3_trk[nCents][nVPts][nTrkPts];
    TH1D* h_dphi_EPn2_trk_noDphi[nCents][nVPts][nTrkPts];
    TH1D* h_dphi_phi0_trk[nCents][nVPts][nTrkPts];
    TH1D* h_dphi_phi0_trk_noDphi[nCents][nVPts][nTrkPts];
    TH2D* h2_deta_vs_trkPt[nCents][nVPts];
    TH2D* h2_deta_h1_vs_trkPt[nCents][nVPts];

    TH1D* h_dphi[nCents][nVPts][nTrkPts];
    TH1D* h_deta[nCents][nVPts][nTrkPts];
    TH1D* h_dR[nCents][nVPts][nTrkPts];
    TH1D* h_zh[nCents][nVPts][nTrkPts];
    TH1D* h_zh_T[nCents][nVPts][nTrkPts];
    TH1D* h_xivh[nCents][nVPts][nTrkPts];
    TH1D* h_xitvh[nCents][nVPts][nTrkPts];

    TH1D* h_dphi_leptrk[nCents][nVPts][nTrkPts];
    TH1D* h_dR_leptrk[nCents][nVPts][nTrkPts];

    // observables for Bootstrapping
    bool fillBootStrap = (!isMC && isPbPb);
    TH2D* h2_bs_dphi_rebin[nCents][nVPts][nTrkPts];
    TH2D* h2_bs_xivh[nCents][nVPts][nTrkPts];
    TH2D* h2_bs_trkPt_rebin[nCents][nVPts];
    TH2D* h2_bs_trkPt_rebin2[nCents][nVPts];
    int evtIndex_bs_dphi_rebin[nCents][nVPts][nTrkPts];
    int evtIndex_bs_xivh[nCents][nVPts][nTrkPts];
    int evtIndex_bs_trkPt_rebin[nCents][nVPts];
    int evtIndex_bs_trkPt_rebin2[nCents][nVPts];
    int nEvtsBS = 3000;

    // trk ID
    TH1D* h_trkPtError[nCents][nVPts][nTrkPts];
    TH1D* h_trkPtoErr[nCents][nVPts][nTrkPts];
    TH1D* h_trkDz1[nCents][nVPts][nTrkPts];
    TH1D* h_trkDz1oErr[nCents][nVPts][nTrkPts];
    TH1D* h_trkDxy1[nCents][nVPts][nTrkPts];
    TH1D* h_trkDxy1oErr[nCents][nVPts][nTrkPts];
    TH1D* h_trkNHit[nCents][nVPts][nTrkPts];
    TH1D* h_trkNdof[nCents][nVPts][nTrkPts];
    TH1D* h_trkNlayer[nCents][nVPts][nTrkPts];
    TH1D* h_trkChi2[nCents][nVPts][nTrkPts];
    TH1D* h_trkChi2_Ndof_Nlayer[nCents][nVPts][nTrkPts];
    TH1D* h_trkAlgo[nCents][nVPts][nTrkPts];
    TH1D* h_trkMVA[nCents][nVPts][nTrkPts];

    TH2D* h2_trkPhi_vs_trkEta[nCents][nVPts][nTrkPts];
    TH2D* h2_trkPhi_vs_trkEta_noDphi[nCents][nVPts][nTrkPts];
    TH2D* h2_deta_vs_dphi[nCents][nVPts][nTrkPts];
    TH2D* h2_deta_vs_dphi_rebin1[nCents][nVPts][nTrkPts];
    TH2D* h2_deta_vs_dphi_rebin2[nCents][nVPts][nTrkPts];
    TH2D* h2_deta_h1_vs_dphi[nCents][nVPts][nTrkPts];
    TH2D* h2_dphi_vs_vPt[nCents][nTrkPts];
    TH2D* h2_dphi_vs_trkPt[nCents][nVPts];
    TH2D* h2_dphi_vs_trkEta[nCents][nVPts][nTrkPts];
    TH2D* h2_dphi_vs_trkPhi[nCents][nVPts][nTrkPts];
    TH2D* h2_deta_vs_xivh[nCents][nVPts][nTrkPts];
    TH2D* h2_deta_h1_vs_xivh[nCents][nVPts][nTrkPts];
    TH2D* h2_dphi_vs_detall[nCents][nVPts][nTrkPts];
    TH2D* h2_dphi_vs_dphill[nCents][nVPts][nTrkPts];
    TH2D* h2_dphi_vs_vY[nCents][nVPts][nTrkPts];
    TH2D* h2_dphi_vs_vPhi[nCents][nVPts][nTrkPts];
    TH2D* h2_dphi_vs_detal1[nCents][nVPts][nTrkPts];
    TH2D* h2_dphi_vs_detal2[nCents][nVPts][nTrkPts];
    TH2D* h2_dphi_vs_l1Pt[nCents][nVPts][nTrkPts];
    TH2D* h2_dphi_vs_l2Pt[nCents][nVPts][nTrkPts];
    TH2D* h2_dphi_vs_l1Eta[nCents][nVPts][nTrkPts];
    TH2D* h2_dphi_vs_l2Eta[nCents][nVPts][nTrkPts];
    TH2D* h2_dphi_vs_l1Phi[nCents][nVPts][nTrkPts];
    TH2D* h2_dphi_vs_l2Phi[nCents][nVPts][nTrkPts];
    TH2D* h2_trkPt_vs_trkEta[nCents][nVPts];
    TH2D* h2_trkPt_vs_xivh[nCents][nVPts];
    TH2D* h2_trkPt_rebin_vs_xivh[nCents][nVPts];
    TH2D* h2_vPt_vs_xivh[nCents][nTrkPts];
    TH2D* h2_vReco_vs_vGen_xivh[nCents][nVPts][nTrkPts];

    // multiplicity
    TH2D* h2_wTrkSum_vs_cent[nVPts][nTrkPts];
    TH2D* h2_wTrkSum_vs_cent_noDphi[nVPts][nTrkPts];

    TH2D* h2_jetpt_vs_xivh[nCents][nVPts][nTrkPts];
    TH2D* h2_rawpt_vs_xivh[nCents][nVPts][nTrkPts];
    TH2D* h2_jetpt_vs_dR[nCents][nVPts][nTrkPts];
    TH2D* h2_rawpt_vs_dR[nCents][nVPts][nTrkPts];
    TH2D* h2_jetpt_vs_dR_rebin[nCents][nVPts][nTrkPts];
    TH2D* h2_rawpt_vs_dR_rebin[nCents][nVPts][nTrkPts];

    int nBinsX_vPt = 30;
    int nBinsX_trkPt = 240;
    int nBinsX_dphi = 20;
    int nBinsX_eta = 26;

    double xMax_vPt = 150;
    double xMax_trkPt = 120;
    double xMax_phi = TMath::Pi()+1e-12;
    double xMax_eta = 2.6;

    std::vector<double> binsX_trkPt_rebin = {0, 0.5, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 14, 18, 24, 30, 45, 60, 90, xMax_trkPt};
    std::vector<double> binsX_trkPt_rebin2 = {0, 1, 3, 5, 10, 18, 30, 60, 90, xMax_trkPt};

    // rebin dphi
    double binW_dphi = xMax_phi / nBinsX_dphi;
    std::vector<double> binsX_dphi_rebin = {0, binW_dphi*3, binW_dphi*6, binW_dphi*9, binW_dphi*12, binW_dphi*14, binW_dphi*16, binW_dphi*18, binW_dphi*19, xMax_phi};

    double binW_vPt = xMax_vPt / nBinsX_vPt;
    std::vector<double> binsX_vPt_rebin;
    for (double binLowEdge = 0; binLowEdge < 40; binLowEdge += binW_vPt) {
        binsX_vPt_rebin.push_back(binLowEdge);
    }
    std::vector<double> binsX_vPt_high = {40, 50, 60, 80, 100, 150};
    binsX_vPt_rebin.insert(binsX_vPt_rebin.end(), binsX_vPt_high.begin(), binsX_vPt_high.end());

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
        std::string text_vY = Form("y^{%s}", text_V.c_str());
        std::string text_vYAbs = Form("|%s|", text_vY.c_str());
        std::string text_vEta = Form("#eta^{%s}", text_V.c_str());
        std::string text_vEtaAbs = Form("|%s|", text_vEta.c_str());
        std::string text_vPhi = Form("#phi^{%s}", text_V.c_str());
        std::string text_l = "";
        if (vIsZ) {
            text_l = vIsZmm ? "#mu" : "e";
        }
        else if (vIsPho) {
            text_vY = text_vEta;
            text_vYAbs = text_vEtaAbs;
        }
        std::string text_ll_os = Form("%s^{+}%s^{-}", text_l.c_str(), text_l.c_str());
        std::string text_ll_ss = Form("%s^{#pm}%s^{#pm}", text_l.c_str(), text_l.c_str());
        std::string text_vM_os = Form("M^{%s}", text_ll_os.c_str());
        std::string text_vM_ss = Form("M^{%s}", text_ll_ss.c_str());

        std::string text_range_vY = Form("%.1f < %s < %.1f", vYMin, text_vYAbs.c_str(), vYMax);
        if (vYMax < vYMin) {
            text_range_vY = Form("%.1f < %s", vYMin, text_vYAbs.c_str());
        }

        std::string name_h_vPt = Form("h_vPt_%s", label_cent.c_str());
        std::string title_h_vPt = Form("%s, %s;%s;", text_range_vY.c_str(),
                                                     text_range_cent.c_str(),
                                                     text_vPt.c_str());

        h_vPt[i] = 0;
        h_vPt[i] = new TH1D(name_h_vPt.c_str(), title_h_vPt.c_str(), nBinsX_vPt, 0, xMax_vPt);
        vec_h_os.push_back(h_vPt[i]);

        std::string name_h_vPt_ss = Form("h_vPt_ss_%s", label_cent.c_str());
        h_vPt_ss[i] = 0;
        h_vPt_ss[i] = new TH1D(name_h_vPt_ss.c_str(), title_h_vPt.c_str(), nBinsX_vPt, 0, xMax_vPt);
        vec_h_ss.push_back(h_vPt_ss[i]);

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

        std::string title_h2_reco_num_vPt_vs_vY = Form("%s;%s;%s;", text_range_cent.c_str(),
                                                           text_vY.c_str(),
                                                           text_vPt.c_str());
        std::string name_h2_reco_num_vPt_vs_vY = Form("h2_reco_num_vPt_vs_vY_%s", label_cent.c_str());
        h2_reco_num_vPt_vs_vY[i] = 0;
        h2_reco_num_vPt_vs_vY[i] = new TH2D(name_h2_reco_num_vPt_vs_vY.c_str(), title_h2_reco_num_vPt_vs_vY.c_str(),
                                            nBinsX_eta, -1*xMax_eta, xMax_eta, nBinsX_vPt, 0, xMax_vPt);
        vec_h_num.push_back(h2_reco_num_vPt_vs_vY[i]);
        vec_h2D.push_back(h2_reco_num_vPt_vs_vY[i]);

        std::string name_h2_reco_denom_vPt_vs_vY = Form("h2_reco_denom_vPt_vs_vY_%s", label_cent.c_str());
        h2_reco_denom_vPt_vs_vY[i] = 0;
        h2_reco_denom_vPt_vs_vY[i] = (TH2D*)h2_reco_num_vPt_vs_vY[i]->Clone(name_h2_reco_denom_vPt_vs_vY.c_str());
        vec_h_denom.push_back(h2_reco_denom_vPt_vs_vY[i]);
        vec_h2D.push_back(h2_reco_denom_vPt_vs_vY[i]);

        std::string name_h2_rgVPt_ratio_vs_vPt = Form("h2_rgVPt_ratio_vs_vPt_%s", label_cent.c_str());
        std::string title_h2_rgVPt_ratio_vs_vPt = Form("%s, %s;gen %s;p_{T}^{reco} / p_{T}^{gen}", text_range_vY.c_str(),
                                                     text_range_cent.c_str(),
                                                     text_vPt.c_str());

        h2_rgVPt_ratio_vs_vPt[i] = 0;
        h2_rgVPt_ratio_vs_vPt[i] = new TH2D(name_h2_rgVPt_ratio_vs_vPt.c_str(), title_h2_rgVPt_ratio_vs_vPt.c_str(),
                                                     nBinsX_vPt, 0, xMax_vPt, 60, 0.4, 1.6);
        vec_h2D.push_back(h2_rgVPt_ratio_vs_vPt[i]);

        std::string name_h2_rgVPt_ratio_vs_vPtreco = Form("h2_rgVPt_ratio_vs_vPtreco_%s", label_cent.c_str());
        std::string title_h2_rgVPt_ratio_vs_vPtreco = Form("%s, %s;%s;p_{T}^{reco} / p_{T}^{gen}", text_range_vY.c_str(),
                                                     text_range_cent.c_str(),
                                                     text_vPt.c_str());

        h2_rgVPt_ratio_vs_vPtreco[i] = 0;
        h2_rgVPt_ratio_vs_vPtreco[i] = new TH2D(name_h2_rgVPt_ratio_vs_vPtreco.c_str(), title_h2_rgVPt_ratio_vs_vPtreco.c_str(),
                                                     nBinsX_vPt, 0, xMax_vPt, 60, 0.4, 1.6);
        vec_h2D.push_back(h2_rgVPt_ratio_vs_vPtreco[i]);

        std::string name_h2_rgVPhi_diff_vs_vPt = Form("h2_rgVPhi_diff_vs_vPt_%s", label_cent.c_str());
        std::string title_h2_rgVPhi_diff_vs_vPt = Form("%s, %s;gen %s;#phi^{reco} - #phi^{gen}", text_range_vY.c_str(),
                                                     text_range_cent.c_str(),
                                                     text_vPt.c_str());

        h2_rgVPhi_diff_vs_vPt[i] = 0;
        h2_rgVPhi_diff_vs_vPt[i] = new TH2D(name_h2_rgVPhi_diff_vs_vPt.c_str(), title_h2_rgVPhi_diff_vs_vPt.c_str(),
                                                     nBinsX_vPt, 0, xMax_vPt, 80, -0.4, 0.4);
        vec_h2D.push_back(h2_rgVPhi_diff_vs_vPt[i]);

        std::string name_h2_hiHF_vs_vPt = Form("h2_hiHF_vs_vPt_%s", label_cent.c_str());
        std::string title_h2_hiHF_vs_vPt = Form("%s, %s;%s;hiHF", text_range_vY.c_str(),
                                                     text_range_cent.c_str(),
                                                     text_vPt.c_str());

        h2_hiHF_vs_vPt[i] = 0;
        h2_hiHF_vs_vPt[i] = new TH2D(name_h2_hiHF_vs_vPt.c_str(), title_h2_hiHF_vs_vPt.c_str(), nBinsX_vPt, 0, xMax_vPt, 2000, 0, 8000);
        vec_h2D.push_back(h2_hiHF_vs_vPt[i]);

        std::string name_h2_rho_vs_vPt = Form("h2_rho_vs_vPt_%s", label_cent.c_str());
        std::string title_h2_rho_vs_vPt = Form("%s, %s;%s;rho", text_range_vY.c_str(),
                                                     text_range_cent.c_str(),
                                                     text_vPt.c_str());

        h2_rho_vs_vPt[i] = 0;
        h2_rho_vs_vPt[i] = new TH2D(name_h2_rho_vs_vPt.c_str(), title_h2_rho_vs_vPt.c_str(), nBinsX_vPt, 0, xMax_vPt, 2000, 0, 400);
        vec_h2D.push_back(h2_rho_vs_vPt[i]);

        std::string name_h2_PFHFtotE_vs_vPt = Form("h2_PFHFtotE_vs_vPt_%s", label_cent.c_str());
        std::string title_h2_PFHFtotE_vs_vPt = Form("%s, %s;%s;total energy of PF HF towers", text_range_vY.c_str(),
                                                     text_range_cent.c_str(),
                                                     text_vPt.c_str());

        h2_PFHFtotE_vs_vPt[i] = 0;
        h2_PFHFtotE_vs_vPt[i] = new TH2D(name_h2_PFHFtotE_vs_vPt.c_str(), title_h2_PFHFtotE_vs_vPt.c_str(),
                                         nBinsX_vPt, 0, xMax_vPt, 2000, 0, 200000);
        vec_h2D.push_back(h2_PFHFtotE_vs_vPt[i]);

        int nBinsX_vPt_rebin = binsX_vPt_rebin.size()-1;
        double arr_vPt[nBinsX_vPt_rebin+1];
        std::copy(binsX_vPt_rebin.begin(), binsX_vPt_rebin.end(), arr_vPt);

        std::string name_h2_PFHFtotE_vs_vPt_rebin = Form("h2_PFHFtotE_vs_vPt_rebin_%s", label_cent.c_str());
        std::string title_h2_PFHFtotE_vs_vPt_rebin = Form("%s, %s;%s;total energy of PF HF towers", text_range_vY.c_str(),
                                                     text_range_cent.c_str(),
                                                     text_vPt.c_str());

        h2_PFHFtotE_vs_vPt_rebin[i] = 0;
        h2_PFHFtotE_vs_vPt_rebin[i] = new TH2D(name_h2_PFHFtotE_vs_vPt_rebin.c_str(), title_h2_PFHFtotE_vs_vPt_rebin.c_str(),
                                                              nBinsX_vPt_rebin, arr_vPt, 2000, 0, 200000);
        vec_h2D.push_back(h2_PFHFtotE_vs_vPt_rebin[i]);

        std::string name_h2_PFHFtotE_eta3to4_vs_vPt = Form("h2_PFHFtotE_eta3to4_vs_vPt_%s", label_cent.c_str());
        std::string title_h2_PFHFtotE_eta3to4_vs_vPt = Form("%s, %s;%s;total energy of PF HF towers", text_range_vY.c_str(),
                                                     text_range_cent.c_str(),
                                                     text_vPt.c_str());

        h2_PFHFtotE_eta3to4_vs_vPt[i] = 0;
        h2_PFHFtotE_eta3to4_vs_vPt[i] = new TH2D(name_h2_PFHFtotE_eta3to4_vs_vPt.c_str(), title_h2_PFHFtotE_eta3to4_vs_vPt.c_str(),
                                         nBinsX_vPt, 0, xMax_vPt, 1000, 0, 100000);
        vec_h2D.push_back(h2_PFHFtotE_eta3to4_vs_vPt[i]);

        std::string name_h2_PFHFtotE_eta3to4_vs_vPt_rebin = Form("h2_PFHFtotE_eta3to4_vs_vPt_rebin_%s", label_cent.c_str());
        std::string title_h2_PFHFtotE_eta3to4_vs_vPt_rebin = Form("%s, %s;%s;total energy of PF HF towers", text_range_vY.c_str(),
                                                     text_range_cent.c_str(),
                                                     text_vPt.c_str());

        h2_PFHFtotE_eta3to4_vs_vPt_rebin[i] = 0;
        h2_PFHFtotE_eta3to4_vs_vPt_rebin[i] = new TH2D(name_h2_PFHFtotE_eta3to4_vs_vPt_rebin.c_str(), title_h2_PFHFtotE_eta3to4_vs_vPt_rebin.c_str(),
                                                              nBinsX_vPt_rebin, arr_vPt, 1000, 0, 100000);
        vec_h2D.push_back(h2_PFHFtotE_eta3to4_vs_vPt_rebin[i]);

        std::string name_h2_PFHFtotE_eta4to5_vs_vPt = Form("h2_PFHFtotE_eta4to5_vs_vPt_%s", label_cent.c_str());
        std::string title_h2_PFHFtotE_eta4to5_vs_vPt = Form("%s, %s;%s;total energy of PF HF towers", text_range_vY.c_str(),
                                                     text_range_cent.c_str(),
                                                     text_vPt.c_str());

        h2_PFHFtotE_eta4to5_vs_vPt[i] = 0;
        h2_PFHFtotE_eta4to5_vs_vPt[i] = new TH2D(name_h2_PFHFtotE_eta4to5_vs_vPt.c_str(), title_h2_PFHFtotE_eta4to5_vs_vPt.c_str(),
                                         nBinsX_vPt, 0, xMax_vPt, 1200, 0, 120000);
        vec_h2D.push_back(h2_PFHFtotE_eta4to5_vs_vPt[i]);

        std::string name_h2_PFHFtotE_eta4to5_vs_vPt_rebin = Form("h2_PFHFtotE_eta4to5_vs_vPt_rebin_%s", label_cent.c_str());
        std::string title_h2_PFHFtotE_eta4to5_vs_vPt_rebin = Form("%s, %s;%s;total energy of PF HF towers", text_range_vY.c_str(),
                                                     text_range_cent.c_str(),
                                                     text_vPt.c_str());

        h2_PFHFtotE_eta4to5_vs_vPt_rebin[i] = 0;
        h2_PFHFtotE_eta4to5_vs_vPt_rebin[i] = new TH2D(name_h2_PFHFtotE_eta4to5_vs_vPt_rebin.c_str(), title_h2_PFHFtotE_eta4to5_vs_vPt_rebin.c_str(),
                                                              nBinsX_vPt_rebin, arr_vPt, 1200, 0, 120000);
        vec_h2D.push_back(h2_PFHFtotE_eta4to5_vs_vPt_rebin[i]);

        std::string name_h2_PFHEtotE_vs_vPt = Form("h2_PFHEtotE_vs_vPt_%s", label_cent.c_str());
        std::string title_h2_PFHEtotE_vs_vPt = Form("%s, %s;%s;total energy of PF HF towers", text_range_vY.c_str(),
                                                     text_range_cent.c_str(),
                                                     text_vPt.c_str());

        h2_PFHEtotE_vs_vPt[i] = 0;
        h2_PFHEtotE_vs_vPt[i] = new TH2D(name_h2_PFHEtotE_vs_vPt.c_str(), title_h2_PFHEtotE_vs_vPt.c_str(),
                                         nBinsX_vPt, 0, xMax_vPt, 2000, 0, 20000);
        vec_h2D.push_back(h2_PFHEtotE_vs_vPt[i]);

        std::string name_h2_dphi_EPn2_V_vs_vPt = Form("h2_dphi_EPn2_V_vs_vPt_%s", label_cent.c_str());
        std::string title_h2_dphi_EPn2_V_vs_vPt = Form("%s, %s;%s;#Delta#phi_{EPn2,%s}", text_range_vY.c_str(),
                                                     text_range_cent.c_str(),
                                                     text_vPt.c_str(), text_V.c_str());

        h2_dphi_EPn2_V_vs_vPt[i] = 0;
        h2_dphi_EPn2_V_vs_vPt[i] = new TH2D(name_h2_dphi_EPn2_V_vs_vPt.c_str(), title_h2_dphi_EPn2_V_vs_vPt.c_str(),
                                         nBinsX_vPt, 0, xMax_vPt, nBinsX_dphi, 0, xMax_phi);
        vec_h2D.push_back(h2_dphi_EPn2_V_vs_vPt[i]);

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

            title_h_suffix = Form("%s, %s, %s", text_range_vPt.c_str(), text_range_vY.c_str(), text_range_cent.c_str());

            std::string name_h_vPhi = Form("h_vPhi_%s", name_h_suffix.c_str());
            std::string title_h_vPhi = Form("%s;%s;", title_h_suffix.c_str(), text_vPhi.c_str());
            h_vPhi[i][j] = 0;
            h_vPhi[i][j] = new TH1D(name_h_vPhi.c_str(), title_h_vPhi.c_str(), nBinsX_dphi, -1*xMax_phi, xMax_phi);

            std::string name_h_vY = Form("h_vY_%s", name_h_suffix.c_str());
            std::string title_h_vY = Form("%s;%s;", title_h_suffix.c_str(), text_vY.c_str());
            h_vY[i][j] = 0;
            h_vY[i][j] = new TH1D(name_h_vY.c_str(), title_h_vY.c_str(), nBinsX_eta, -1*xMax_eta, xMax_eta);
            vec_h_os.push_back(h_vY[i][j]);

            std::string name_h_vY_ss = Form("h_vY_ss_%s", name_h_suffix.c_str());
            h_vY_ss[i][j] = 0;
            h_vY_ss[i][j] = new TH1D(name_h_vY_ss.c_str(), title_h_vY.c_str(), nBinsX_eta, -1*xMax_eta, xMax_eta);
            vec_h_ss.push_back(h_vY_ss[i][j]);

            h_l1Pt[i][j] = 0;
            h_l1Pt[i][j] = new TH1D(Form("h_l1Pt_%s", name_h_suffix.c_str()), ";p_T^{l1};", nBinsX_vPt, 0, xMax_vPt);

            h_l1Eta[i][j] = 0;
            h_l1Eta[i][j] = new TH1D(Form("h_l1Eta_%s", name_h_suffix.c_str()), ";#eta^{l1};", nBinsX_eta, -1*xMax_eta, xMax_eta);

            h_l1Phi[i][j] = 0;
            h_l1Phi[i][j] = new TH1D(Form("h_l1Phi_%s", name_h_suffix.c_str()), ";#phi^{l1};", nBinsX_dphi, -1*xMax_phi, xMax_phi);

            h_l1R9[i][j] = 0;
            h_l1R9[i][j] = new TH1D(Form("h_l1R9_%s", name_h_suffix.c_str()), ";l1 R9;", 44, 0, 1.1);

            h_l2Pt[i][j] = 0;
            h_l2Pt[i][j] = new TH1D(Form("h_l2Pt_%s", name_h_suffix.c_str()), ";p_T^{l2};", nBinsX_vPt, 0, xMax_vPt);

            h_l2Eta[i][j] = 0;
            h_l2Eta[i][j] = new TH1D(Form("h_l2Eta_%s", name_h_suffix.c_str()), ";#eta^{l2};", nBinsX_eta, -1*xMax_eta, xMax_eta);

            h_l2Phi[i][j] = 0;
            h_l2Phi[i][j] = new TH1D(Form("h_l2Phi_%s", name_h_suffix.c_str()), ";#phi^{l2};", nBinsX_dphi, -1*xMax_phi, xMax_phi);

            h_l2R9[i][j] = 0;
            h_l2R9[i][j] = new TH1D(Form("h_l2R9_%s", name_h_suffix.c_str()), ";l2 R9;", 44, 0, 1.1);

            h_detall[i][j] = 0;
            h_detall[i][j] = new TH1D(Form("h_detall_%s", name_h_suffix.c_str()), ";deta_ll;", nBinsX_eta, 0, 5.2);

            h_dphill[i][j] = 0;
            h_dphill[i][j] = new TH1D(Form("h_dphill_%s", name_h_suffix.c_str()), ";dphi_ll;", nBinsX_dphi, 0, xMax_phi);

            std::string name_h2_vPhi_vs_vY = Form("h2_vPhi_vs_vY_%s", name_h_suffix.c_str());
            std::string title_h2_vPhi_vs_vY = Form("%s, %s;%s;%s", text_range_vY.c_str(),
                                                         text_range_cent.c_str(),
                                                         text_vY.c_str(), text_vPhi.c_str());

            h2_vPhi_vs_vY[i][j] = 0;
            h2_vPhi_vs_vY[i][j] = new TH2D(name_h2_vPhi_vs_vY.c_str(), title_h2_vPhi_vs_vY.c_str(),
                                             nBinsX_eta, -1*xMax_eta, xMax_eta, nBinsX_dphi, -1*xMax_phi, xMax_phi);
            vec_h2D.push_back(h2_vPhi_vs_vY[i][j]);

            std::string name_h2_l1Phi_vs_l1Eta = Form("h2_l1Phi_vs_l1Eta_%s", name_h_suffix.c_str());
            std::string title_h2_l1Phi_vs_l1Eta = Form("%s, %s;%s;%s", text_range_vY.c_str(),
                                                         text_range_cent.c_str(),
                                                         "#eta^{l1}", "#phi^{l1}");

            h2_l1Phi_vs_l1Eta[i][j] = 0;
            h2_l1Phi_vs_l1Eta[i][j] = new TH2D(name_h2_l1Phi_vs_l1Eta.c_str(), title_h2_l1Phi_vs_l1Eta.c_str(),
                                             nBinsX_eta, -1*xMax_eta, xMax_eta, nBinsX_dphi, -1*xMax_phi, xMax_phi);
            vec_h2D.push_back(h2_l1Phi_vs_l1Eta[i][j]);

            std::string name_h2_l2Phi_vs_l2Eta = Form("h2_l2Phi_vs_l2Eta_%s", name_h_suffix.c_str());
            std::string title_h2_l2Phi_vs_l2Eta = Form("%s, %s;%s;%s", text_range_vY.c_str(),
                                                         text_range_cent.c_str(),
                                                         "#eta^{l2}", "#phi^{l2}");

            h2_l2Phi_vs_l2Eta[i][j] = 0;
            h2_l2Phi_vs_l2Eta[i][j] = new TH2D(name_h2_l2Phi_vs_l2Eta.c_str(), title_h2_l2Phi_vs_l2Eta.c_str(),
                                             nBinsX_eta, -1*xMax_eta, xMax_eta, nBinsX_dphi, -1*xMax_phi, xMax_phi);
            vec_h2D.push_back(h2_l2Phi_vs_l2Eta[i][j]);

            std::string name_h_trig_num_vY = Form("h_trig_num_vY_%s", name_h_suffix.c_str());
            h_trig_num_vY[i][j] = 0;
            h_trig_num_vY[i][j] = (TH1D*)h_vY[i][j]->Clone(name_h_trig_num_vY.c_str());
            vec_h_num.push_back(h_trig_num_vY[i][j]);

            std::string name_h_trig_denom_vY = Form("h_trig_denom_vY_%s", name_h_suffix.c_str());
            h_trig_denom_vY[i][j] = 0;
            h_trig_denom_vY[i][j] = (TH1D*)h_vY[i][j]->Clone(name_h_trig_denom_vY.c_str());
            vec_h_denom.push_back(h_trig_denom_vY[i][j]);

            std::string name_h_reco_num_vPhi = Form("h_reco_num_vPhi_%s", name_h_suffix.c_str());
            h_reco_num_vPhi[i][j] = 0;
            h_reco_num_vPhi[i][j] = (TH1D*)h_vPhi[i][j]->Clone(name_h_reco_num_vPhi.c_str());
            h_reco_num_vPhi[i][j]->SetXTitle(Form("gen %s", h_vPhi[i][j]->GetXaxis()->GetTitle()));
            vec_h_num.push_back(h_reco_num_vPhi[i][j]);

            std::string name_h_reco_denom_vPhi = Form("h_reco_denom_vPhi_%s", name_h_suffix.c_str());
            h_reco_denom_vPhi[i][j] = 0;
            h_reco_denom_vPhi[i][j] = (TH1D*)h_reco_num_vPhi[i][j]->Clone(name_h_reco_denom_vPhi.c_str());
            vec_h_denom.push_back(h_reco_denom_vPhi[i][j]);

            std::string name_h_reco_num_vY = Form("h_reco_num_vY_%s", name_h_suffix.c_str());
            h_reco_num_vY[i][j] = 0;
            h_reco_num_vY[i][j] = (TH1D*)h_vY[i][j]->Clone(name_h_reco_num_vY.c_str());
            h_reco_num_vY[i][j]->SetXTitle(Form("gen %s", h_vY[i][j]->GetXaxis()->GetTitle()));
            vec_h_num.push_back(h_reco_num_vY[i][j]);

            std::string name_h_reco_denom_vY = Form("h_reco_denom_vY_%s", name_h_suffix.c_str());
            h_reco_denom_vY[i][j] = 0;
            h_reco_denom_vY[i][j] = (TH1D*)h_reco_num_vY[i][j]->Clone(name_h_reco_denom_vY.c_str());
            vec_h_denom.push_back(h_reco_denom_vY[i][j]);

            std::string name_h_vM_os = Form("h_vM_os_%s", name_h_suffix.c_str());
            std::string title_h_vM_os = Form("%s;%s;", title_h_suffix.c_str(), text_vM_os.c_str());
            h_vM_os[i][j] = 0;
            h_vM_os[i][j] = new TH1D(name_h_vM_os.c_str(), title_h_vM_os.c_str(), 30, 60, 120);
            vec_h_os.push_back(h_vM_os[i][j]);

            std::string name_h_vM_ss = Form("h_vM_ss_%s", name_h_suffix.c_str());
            std::string title_h_vM_ss = Form("%s;%s;", title_h_suffix.c_str(), text_vM_ss.c_str());
            h_vM_ss[i][j] = 0;
            h_vM_ss[i][j] = new TH1D(name_h_vM_ss.c_str(), title_h_vM_ss.c_str(), 30, 60, 120);
            vec_h_ss.push_back(h_vM_ss[i][j]);

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

                std::string name_h2_rgVPt_ratio_vs_cent = Form("h2_rgVPt_ratio_vs_halfHiBin_%s", label_vPt.c_str());
                std::string title_h2_rgVPt_ratio_vs_cent = Form("%s;Centrality (%%);p_{T}^{reco} / p_{T}^{gen}", text_range_vPt.c_str());

                h2_rgVPt_ratio_vs_cent[j] = 0;
                h2_rgVPt_ratio_vs_cent[j] = new TH2D(name_h2_rgVPt_ratio_vs_cent.c_str(), title_h2_rgVPt_ratio_vs_cent.c_str(),
                                                     40, 0, 100, 60, 0.4, 1.6);
                vec_h2D.push_back(h2_rgVPt_ratio_vs_cent[j]);
            }

            std::string name_h2_rgVPt_ratio_vs_vY = Form("h2_rgVPt_ratio_vs_vY_%s", name_h_suffix.c_str());
            std::string title_h2_rgVPt_ratio_vs_vY = Form("%s;%s;p_{T}^{reco} / p_{T}^{gen}", title_h_suffix.c_str(), text_vY.c_str());
            h2_rgVPt_ratio_vs_vY[i][j] = 0;
            h2_rgVPt_ratio_vs_vY[i][j] = new TH2D(name_h2_rgVPt_ratio_vs_vY.c_str(), title_h2_rgVPt_ratio_vs_vY.c_str(),
                                                  nBinsX_eta, -1*xMax_eta, xMax_eta, 60, 0.4, 1.6);
            vec_h2D.push_back(h2_rgVPt_ratio_vs_vY[i][j]);

            std::string name_h2_rgVPt_ratio_vs_vM = Form("h2_rgVPt_ratio_vs_vM_%s", name_h_suffix.c_str());
            std::string title_h2_rgVPt_ratio_vs_vM = Form("%s;gen %s;p_{T}^{reco} / p_{T}^{gen}", title_h_suffix.c_str(), text_vM_os.c_str());
            h2_rgVPt_ratio_vs_vM[i][j] = 0;
            h2_rgVPt_ratio_vs_vM[i][j] = new TH2D(name_h2_rgVPt_ratio_vs_vM.c_str(), title_h2_rgVPt_ratio_vs_vM.c_str(),
                                                  30, 60, 120, 60, 0.4, 1.6);
            vec_h2D.push_back(h2_rgVPt_ratio_vs_vM[i][j]);

            std::string name_h2_rgVPt_ratio_vs_vMreco = Form("h2_rgVPt_ratio_vs_vMreco_%s", name_h_suffix.c_str());
            std::string title_h2_rgVPt_ratio_vs_vMreco = Form("%s;%s;p_{T}^{reco} / p_{T}^{gen}", title_h_suffix.c_str(), text_vM_os.c_str());
            h2_rgVPt_ratio_vs_vMreco[i][j] = 0;
            h2_rgVPt_ratio_vs_vMreco[i][j] = new TH2D(name_h2_rgVPt_ratio_vs_vMreco.c_str(), title_h2_rgVPt_ratio_vs_vMreco.c_str(),
                                                      30, 60, 120, 60, 0.4, 1.6);
            vec_h2D.push_back(h2_rgVPt_ratio_vs_vMreco[i][j]);

            std::string name_h_vtxz = Form("h_vtxz_%s", name_h_suffix.c_str());
            std::string title_h_vtxz = Form("%s;v_{z};", title_h_suffix.c_str());
            h_vtxz[i][j] = 0;
            h_vtxz[i][j] = new TH1D(name_h_vtxz.c_str(), title_h_vtxz.c_str(), 30, -15, 15);

            if (isPP) {
                std::string name_h_nVtx = Form("h_nVtx_%s", name_h_suffix.c_str());
                std::string title_h_nVtx = Form("%s;nVtx;", title_h_suffix.c_str());
                h_nVtx[i][j] = 0;
                h_nVtx[i][j] = new TH1D(name_h_nVtx.c_str(), title_h_nVtx.c_str(), 100, 0, 100);
            }

            std::string name_h_dphi_phi0_V = Form("h_dphi_phi0_V_%s", name_h_suffix.c_str());
            std::string title_h_dphi_phi0_V = Form("%s;#Delta#phi_{#phi_{0},%s};", title_h_suffix.c_str(), text_V.c_str());

            h_dphi_phi0_V[i][j] = 0;
            h_dphi_phi0_V[i][j] = new TH1D(name_h_dphi_phi0_V.c_str(), title_h_dphi_phi0_V.c_str(), nBinsX_dphi, 0, xMax_phi);

            if (isPbPb) {
                std::string name_h_reco_num_dphi_phi0_V = Form("h_reco_num_dphi_phi0_V_%s", name_h_suffix.c_str());

                h_reco_num_dphi_phi0_V[i][j] = 0;
                h_reco_num_dphi_phi0_V[i][j] = (TH1D*)h_dphi_phi0_V[i][j]->Clone(name_h_reco_num_dphi_phi0_V.c_str());
                h_reco_num_dphi_phi0_V[i][j]->SetXTitle(Form("gen %s", h_dphi_phi0_V[i][j]->GetXaxis()->GetTitle()));
                vec_h_num.push_back(h_reco_num_dphi_phi0_V[i][j]);

                std::string name_h_reco_denom_dphi_phi0_V = Form("h_reco_denom_dphi_phi0_V_%s", name_h_suffix.c_str());
                h_reco_denom_dphi_phi0_V[i][j] = 0;
                h_reco_denom_dphi_phi0_V[i][j] = (TH1D*)h_reco_num_dphi_phi0_V[i][j]->Clone(name_h_reco_denom_dphi_phi0_V.c_str());
                vec_h_denom.push_back(h_reco_denom_dphi_phi0_V[i][j]);
            }

            std::string name_h_dphi_EPn1_V = Form("h_dphi_EPn1_V_%s", name_h_suffix.c_str());
            std::string title_h_dphi_EPn1_V = Form("%s;#Delta#phi_{EPn1,%s};", title_h_suffix.c_str(), text_V.c_str());

            h_dphi_EPn1_V[i][j] = 0;
            h_dphi_EPn1_V[i][j] = new TH1D(name_h_dphi_EPn1_V.c_str(), title_h_dphi_EPn1_V.c_str(), nBinsX_dphi, 0, xMax_phi);

            std::string name_h_dphi_EPn2_V = Form("h_dphi_EPn2_V_%s", name_h_suffix.c_str());
            std::string title_h_dphi_EPn2_V = Form("%s;#Delta#phi_{EPn2,%s};", title_h_suffix.c_str(), text_V.c_str());

            h_dphi_EPn2_V[i][j] = 0;
            h_dphi_EPn2_V[i][j] = new TH1D(name_h_dphi_EPn2_V.c_str(), title_h_dphi_EPn2_V.c_str(), nBinsX_dphi, 0, xMax_phi);

            if (isPbPb) {
                std::string name_h_reco_num_dphi_EPn2_V = Form("h_reco_num_dphi_EPn2_V_%s", name_h_suffix.c_str());

                h_reco_num_dphi_EPn2_V[i][j] = 0;
                h_reco_num_dphi_EPn2_V[i][j] = (TH1D*)h_dphi_EPn2_V[i][j]->Clone(name_h_reco_num_dphi_EPn2_V.c_str());
                h_reco_num_dphi_EPn2_V[i][j]->SetXTitle(Form("gen %s", h_dphi_EPn2_V[i][j]->GetXaxis()->GetTitle()));
                vec_h_num.push_back(h_reco_num_dphi_EPn2_V[i][j]);

                std::string name_h_reco_denom_dphi_EPn2_V = Form("h_reco_denom_dphi_EPn2_V_%s", name_h_suffix.c_str());
                h_reco_denom_dphi_EPn2_V[i][j] = 0;
                h_reco_denom_dphi_EPn2_V[i][j] = (TH1D*)h_reco_num_dphi_EPn2_V[i][j]->Clone(name_h_reco_denom_dphi_EPn2_V.c_str());
                vec_h_denom.push_back(h_reco_denom_dphi_EPn2_V[i][j]);
            }

            std::string name_h_dphi_EPn3_V = Form("h_dphi_EPn3_V_%s", name_h_suffix.c_str());
            std::string title_h_dphi_EPn3_V = Form("%s;#Delta#phi_{EPn3,%s};", title_h_suffix.c_str(), text_V.c_str());

            h_dphi_EPn3_V[i][j] = 0;
            h_dphi_EPn3_V[i][j] = new TH1D(name_h_dphi_EPn3_V.c_str(), title_h_dphi_EPn3_V.c_str(), nBinsX_dphi, 0, xMax_phi);

            std::string name_h_dphi_EPn2_phi0 = Form("h_dphi_EPn2_phi0_%s", name_h_suffix.c_str());
            std::string title_h_dphi_EPn2_phi0 = Form("%s;#Delta#phi_{EPn2,#phi_{0}};", title_h_suffix.c_str());

            h_dphi_EPn2_phi0[i][j] = 0;
            h_dphi_EPn2_phi0[i][j] = new TH1D(name_h_dphi_EPn2_phi0.c_str(), title_h_dphi_EPn2_phi0.c_str(), nBinsX_dphi, 0, xMax_phi);

            std::string name_h_dphi_EPn2_l1 = Form("h_dphi_EPn2_l1_%s", name_h_suffix.c_str());
            std::string title_h_dphi_EPn2_l1 = Form("%s;#Delta#phi_{EPn2,%s};", title_h_suffix.c_str(), "l1");

            h_dphi_EPn2_l1[i][j] = 0;
            h_dphi_EPn2_l1[i][j] = new TH1D(name_h_dphi_EPn2_l1.c_str(), title_h_dphi_EPn2_l1.c_str(), nBinsX_dphi, 0, xMax_phi);

            std::string name_h_dphi_EPn2_l2 = Form("h_dphi_EPn2_l2_%s", name_h_suffix.c_str());
            std::string title_h_dphi_EPn2_l2 = Form("%s;#Delta#phi_{EPn2,%s};", title_h_suffix.c_str(), "l2");

            h_dphi_EPn2_l2[i][j] = 0;
            h_dphi_EPn2_l2[i][j] = new TH1D(name_h_dphi_EPn2_l2.c_str(), title_h_dphi_EPn2_l2.c_str(), nBinsX_dphi, 0, xMax_phi);

            if (isMC && isPbPb) {
                std::string name_h2_PFHFtotE_vs_Npart = Form("h2_PFHFtotE_vs_Npart_%s", name_h_suffix.c_str());
                std::string title_h2_PFHFtotE_vs_Npart = Form("%s;Npart;total energy of PF HF towers", title_h_suffix.c_str());

                h2_PFHFtotE_vs_Npart[i][j] = 0;
                h2_PFHFtotE_vs_Npart[i][j] = new TH2D(name_h2_PFHFtotE_vs_Npart.c_str(), title_h2_PFHFtotE_vs_Npart.c_str(),
                                                          410, 0, 410, 200, 0, 200000);
                vec_h2D.push_back(h2_PFHFtotE_vs_Npart[i][j]);

                std::string name_h2_PFHFtotEmix_vs_Npart = Form("h2_PFHFtotEmix_vs_Npart_%s", name_h_suffix.c_str());
                std::string title_h2_PFHFtotEmix_vs_Npart = Form("%s;Npart;mixed event total energy of PF HF towers", title_h_suffix.c_str());

                h2_PFHFtotEmix_vs_Npart[i][j] = 0;
                h2_PFHFtotEmix_vs_Npart[i][j] = new TH2D(name_h2_PFHFtotEmix_vs_Npart.c_str(), title_h2_PFHFtotEmix_vs_Npart.c_str(),
                                                          410, 0, 410, 200, 0, 200000);
                vec_h2D.push_back(h2_PFHFtotEmix_vs_Npart[i][j]);

                std::string name_h2_PFHFtotE_diff_rawmix_vs_Npart = Form("h2_PFHFtotE_diff_rawmix_vs_Npart_%s", name_h_suffix.c_str());
                std::string title_h2_PFHFtotE_diff_rawmix_vs_Npart = Form("%s;Npart;#Delta(total energy of PF HF towers)_{raw, mix}", title_h_suffix.c_str());

                h2_PFHFtotE_diff_rawmix_vs_Npart[i][j] = 0;
                h2_PFHFtotE_diff_rawmix_vs_Npart[i][j] = new TH2D(name_h2_PFHFtotE_diff_rawmix_vs_Npart.c_str(), title_h2_PFHFtotE_diff_rawmix_vs_Npart.c_str(),
                                                          410, 0, 410, 150, 0, 15000);
                vec_h2D.push_back(h2_PFHFtotE_diff_rawmix_vs_Npart[i][j]);
            }

            std::string name_h2_PFHFtotE_vs_vY = Form("h2_PFHFtotE_vs_vY_%s", name_h_suffix.c_str());
            std::string title_h2_PFHFtotE_vs_vY = Form("%s;%s;total energy of PF HF towers", title_h_suffix.c_str(), text_vY.c_str());

            h2_PFHFtotE_vs_vY[i][j] = 0;
            h2_PFHFtotE_vs_vY[i][j] = new TH2D(name_h2_PFHFtotE_vs_vY.c_str(), title_h2_PFHFtotE_vs_vY.c_str(),
                                               nBinsX_eta, -1*xMax_eta, xMax_eta, 200, 0, 200000);
            vec_h2D.push_back(h2_PFHFtotE_vs_vY[i][j]);

            if (isPP) {
                std::string name_h2_PFHFtotE_vs_nVtx = Form("h2_PFHFtotE_vs_nVtx_%s", name_h_suffix.c_str());
                std::string title_h2_PFHFtotE_vs_nVtx = Form("%s;nVtx;total energy of PF HF towers", title_h_suffix.c_str());

                h2_PFHFtotE_vs_nVtx[i][j] = 0;
                h2_PFHFtotE_vs_nVtx[i][j] = new TH2D(name_h2_PFHFtotE_vs_nVtx.c_str(), title_h2_PFHFtotE_vs_nVtx.c_str(),
                                                          100, 0, 100, 200, 0, 200000);
                vec_h2D.push_back(h2_PFHFtotE_vs_nVtx[i][j]);
            }

            if (i == 0) {
                std::string name_h2_hiHF_vs_hiNpix = Form("h2_hiHF_vs_hiNpix_%s", label_vPt.c_str());
                std::string title_h2_hiHF_vs_hiNpix = Form("%s;hiNpix;hiHF", title_h_suffix.c_str());

                h2_hiHF_vs_hiNpix[j] = 0;
                h2_hiHF_vs_hiNpix[j] = new TH2D(name_h2_hiHF_vs_hiNpix.c_str(), title_h2_hiHF_vs_hiNpix.c_str(),
                                                       40, 0, 120000, 400, 0, 8000);
                vec_h2D.push_back(h2_hiHF_vs_hiNpix[j]);

                std::string name_h2_PFHFtotE_vs_hiNpix = Form("h2_PFHFtotE_vs_hiNpix_%s", label_vPt.c_str());
                std::string title_h2_PFHFtotE_vs_hiNpix = Form("%s;hiNpix;total energy of PF HF towers", title_h_suffix.c_str());

                h2_PFHFtotE_vs_hiNpix[j] = 0;
                h2_PFHFtotE_vs_hiNpix[j] = new TH2D(name_h2_PFHFtotE_vs_hiNpix.c_str(), title_h2_PFHFtotE_vs_hiNpix.c_str(),
                                                       40, 0, 120000, 2000, 0, 200000);
                vec_h2D.push_back(h2_PFHFtotE_vs_hiNpix[j]);
            }

            if (isPP13tev) {

                std::string name_h2_nVtx_vs_run_13tev_pp17 = Form("h2_nVtx_vs_run_13tev_pp17_%s", name_h_suffix.c_str());
                std::string title_h2_nVtx_vs_run_13tev_pp17 = Form("%s;run;nVtx", title_h_suffix.c_str());

                h2_nVtx_vs_run_13tev_pp17[i][j] = 0;
                h2_nVtx_vs_run_13tev_pp17[i][j] = new TH2D(name_h2_nVtx_vs_run_13tev_pp17.c_str(), title_h2_nVtx_vs_run_13tev_pp17.c_str(),
                                                          2000, 305000, 307000, 100, 0, 100);
                vec_h2D.push_back(h2_nVtx_vs_run_13tev_pp17[i][j]);

                std::string name_h2_nVtx_vs_run_13tev_pp18 = Form("h2_nVtx_vs_run_13tev_pp18_%s", name_h_suffix.c_str());
                std::string title_h2_nVtx_vs_run_13tev_pp18 = Form("%s;run;nVtx", title_h_suffix.c_str());

                h2_nVtx_vs_run_13tev_pp18[i][j] = 0;
                h2_nVtx_vs_run_13tev_pp18[i][j] = new TH2D(name_h2_nVtx_vs_run_13tev_pp18.c_str(), title_h2_nVtx_vs_run_13tev_pp18.c_str(),
                                                          2000, 324000, 326000, 100, 0, 100);
                vec_h2D.push_back(h2_nVtx_vs_run_13tev_pp18[i][j]);
            }

            std::string text_trk = "trk";
            std::string text_trkPt = Form("p^{%s}_{T}", text_trk.c_str());
            std::string text_trkEta = Form("#eta^{%s}", text_trk.c_str());
            std::string text_trkEta_h1 = Form("#eta^{%s, lead}", text_trk.c_str());
            std::string text_trkEtaAbs = Form("|%s|", text_trkEta.c_str());
            std::string text_trkPhi = Form("#phi^{%s}", text_trk.c_str());

            std::string text_dphi = Form("#Delta#phi_{%s%s}", text_trk.c_str(),
                                                                   text_V.c_str());
            std::string text_defn_dphi = Form("%s = |%s - %s|", text_dphi.c_str(),
                                                                text_trkPhi.c_str(),
                                                                text_vPhi.c_str());

            std::string text_deta = Form("#Delta#eta_{%s%s}", text_trk.c_str(),
                                                              text_V.c_str());
            std::string text_defn_deta = Form("%s = |%s - %s|", text_deta.c_str(),
                                                                text_trkEta.c_str(),
                                                                text_vEta.c_str());

            std::string text_deta_h1 = Form("#Delta#eta_{%s,leading trk}", text_trk.c_str());
            std::string text_defn_deta_h1 = Form("%s = |%s - %s|", text_deta_h1.c_str(),
                                                                text_trkEta.c_str(),
                                                                text_trkEta_h1.c_str());

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
            std::string text_range_deta = Form("%.1f < %s < %.1f", vTrkDetaMin, text_deta.c_str(), vTrkDetaMax);
            if (vTrkDetaMax < vTrkDetaMin) {
                text_range_deta = Form("%s > %.1f", text_deta.c_str(), vTrkDetaMin);
            }

            title_h_suffix = Form("%s, %s, %s, %s, %s", text_range_vPt.c_str(),
                                                    text_range_trkEta.c_str(),
                                                    text_range_dphi.c_str(),
                                                    text_range_deta.c_str(),
                                                    text_range_cent.c_str());

            if (anaTrks) {

                std::string name_h_trkPt = Form("h_trkPt_%s", name_h_suffix.c_str());
                std::string title_h_trkPt = Form("%s;%s;", title_h_suffix.c_str(),
                        text_trkPt.c_str());
                h_trkPt[i][j] = 0;
                h_trkPt[i][j] = new TH1D(name_h_trkPt.c_str(), title_h_trkPt.c_str(), nBinsX_trkPt, 0, xMax_trkPt);

                std::string title_h_suffix_dphi = Form("%s, %s, %s, %s", text_range_vPt.c_str(),
                        text_range_trkEta.c_str(),
                        text_range_deta.c_str(),
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

                std::string name_h2_deta_vs_trkPt = Form("h2_deta_vs_trkPt_%s", name_h_suffix.c_str());
                std::string title_h2_deta_vs_trkPt = Form("%s;%s;%s;", title_h_suffix.c_str(),
                        text_trkPt.c_str(),
                        text_deta.c_str());

                h2_deta_vs_trkPt[i][j] = 0;
                h2_deta_vs_trkPt[i][j] = new TH2D(name_h2_deta_vs_trkPt.c_str(), title_h2_deta_vs_trkPt.c_str(),
                        nBinsX_trkPt, 0, xMax_trkPt,
                        nBinsX_eta, 0, 5.2);
                vec_h2D.push_back(h2_deta_vs_trkPt[i][j]);

                if (findLeadingTrk) {
                    std::string name_h2_deta_h1_vs_trkPt = Form("h2_deta_h1_vs_trkPt_%s", name_h_suffix.c_str());
                    std::string title_h2_deta_h1_vs_trkPt = Form("%s;%s;%s;", title_h_suffix.c_str(),
                            text_trkPt.c_str(),
                            text_deta_h1.c_str());

                    h2_deta_h1_vs_trkPt[i][j] = 0;
                    h2_deta_h1_vs_trkPt[i][j] = new TH2D(name_h2_deta_h1_vs_trkPt.c_str(), title_h2_deta_h1_vs_trkPt.c_str(),
                            nBinsX_trkPt, 0, xMax_trkPt,
                            nBinsX_eta, 0, 5.2);
                    vec_h2D.push_back(h2_deta_h1_vs_trkPt[i][j]);
                }

                std::string name_h2_trkPt_vs_trkEta = Form("h2_trkPt_vs_trkEta_%s", name_h_suffix.c_str());
                std::string title_h2_trkPt_vs_trkEta = Form("%s;%s;%s", title_h_suffix.c_str(),
                        text_trkEta.c_str(),
                        text_trkPt.c_str());

                h2_trkPt_vs_trkEta[i][j] = 0;
                h2_trkPt_vs_trkEta[i][j] = new TH2D(name_h2_trkPt_vs_trkEta.c_str(), title_h2_trkPt_vs_trkEta.c_str(),
                        nBinsX_eta, -1*xMax_eta, xMax_eta,
                        nBinsX_trkPt, 0, xMax_trkPt);
                vec_h2D.push_back(h2_trkPt_vs_trkEta[i][j]);

                std::string text_vPt_vec = Form("#bf{p}^{%s}_{T}", text_V.c_str());
                std::string text_trkPt_vec = Form("#bf{p}^{%s}_{T}", text_trk.c_str());

                std::string text_xivh = Form("#xi^{%s, %s}_{T}", text_trk.c_str(),
                        text_V.c_str());
                std::string text_defn_xivh = Form("%s = ln ( -|%s|^{2} / (%s #dot %s) ) ",
                        text_xivh.c_str(),
                        text_vPt_vec.c_str(),
                        text_vPt_vec.c_str(),
                        text_trkPt_vec.c_str());

                std::string text_defn_xitvh = Form("#xi = ln ( %s / %s ) ",
                        text_vPt.c_str(),
                        text_trkPt.c_str());

                std::string name_h2_trkPt_vs_xivh = Form("h2_trkPt_vs_xivh_%s", name_h_suffix.c_str());
                std::string title_h2_trkPt_vs_xivh = Form("%s;%s;%s", title_h_suffix.c_str(),
                        text_xivh.c_str(),
                        text_trkPt.c_str());

                int nBins_xivh = 10;

                h2_trkPt_vs_xivh[i][j] = 0;
                h2_trkPt_vs_xivh[i][j] = new TH2D(name_h2_trkPt_vs_xivh.c_str(), title_h2_trkPt_vs_xivh.c_str(),
                        nBins_xivh, 0, 5,
                        nBinsX_trkPt, 0, xMax_trkPt);
                vec_h2D.push_back(h2_trkPt_vs_xivh[i][j]);

                std::string name_h2_trkPt_rebin_vs_xivh = Form("h2_trkPt_rebin_vs_xivh_%s", name_h_suffix.c_str());

                int nBinsX_trkPt_rebin = binsX_trkPt_rebin.size()-1;

                double arrTmp_trkPt[nBinsX_trkPt_rebin+1];
                std::copy(binsX_trkPt_rebin.begin(), binsX_trkPt_rebin.end(), arrTmp_trkPt);

                h2_trkPt_rebin_vs_xivh[i][j] = 0;
                h2_trkPt_rebin_vs_xivh[i][j] = new TH2D(name_h2_trkPt_rebin_vs_xivh.c_str(), title_h2_trkPt_vs_xivh.c_str(),
                        nBins_xivh, 0, 5,
                        nBinsX_trkPt_rebin, arrTmp_trkPt);
                vec_h2D.push_back(h2_trkPt_rebin_vs_xivh[i][j]);

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
                    title_h_suffix = Form("%s, %s, %s, %s, %s", text_range_vPt.c_str(),
                            text_range_trkPt.c_str(),
                            text_range_dphi.c_str(),
                            text_range_deta.c_str(),
                            text_range_cent.c_str());

                    std::string name_h_trkEta = Form("h_trkEta_%s", name_h_suffix.c_str());
                    std::string title_h_trkEta = Form("%s;%s;", title_h_suffix.c_str(),
                            text_trkEta.c_str());
                    h_trkEta[i][j][k] = 0;
                    h_trkEta[i][j][k] = new TH1D(name_h_trkEta.c_str(), title_h_trkEta.c_str(), nBinsX_eta, -1*xMax_eta, xMax_eta);

                    title_h_suffix = Form("%s, %s, %s, %s, %s, %s", text_range_vPt.c_str(),
                            text_range_trkPt.c_str(),
                            text_range_trkEta.c_str(),
                            text_range_dphi.c_str(),
                            text_range_deta.c_str(),
                            text_range_cent.c_str());

                    std::string name_h_trkPhi = Form("h_trkPhi_%s", name_h_suffix.c_str());
                    std::string title_h_trkPhi = Form("%s;%s;", title_h_suffix.c_str(),
                            text_trkPhi.c_str());
                    h_trkPhi[i][j][k] = 0;
                    h_trkPhi[i][j][k] = new TH1D(name_h_trkPhi.c_str(), title_h_trkPhi.c_str(), nBinsX_dphi, -1*xMax_phi, xMax_phi);

                    std::string name_h_dphi_EPn1_trk = Form("h_dphi_EPn1_trk_%s", name_h_suffix.c_str());
                    std::string title_h_dphi_EPn1_trk = Form("%s;#Delta#phi_{EPn1,%s};", title_h_suffix.c_str(),
                            text_trk.c_str());

                    h_dphi_EPn1_trk[i][j][k] = 0;
                    h_dphi_EPn1_trk[i][j][k] = new TH1D(name_h_dphi_EPn1_trk.c_str(), title_h_dphi_EPn1_trk.c_str(),
                            nBinsX_dphi, 0, xMax_phi);

                    std::string name_h_dphi_EPn2_trk = Form("h_dphi_EPn2_trk_%s", name_h_suffix.c_str());
                    std::string title_h_dphi_EPn2_trk = Form("%s;#Delta#phi_{EPn2,%s};", title_h_suffix.c_str(),
                            text_trk.c_str());

                    h_dphi_EPn2_trk[i][j][k] = 0;
                    h_dphi_EPn2_trk[i][j][k] = new TH1D(name_h_dphi_EPn2_trk.c_str(), title_h_dphi_EPn2_trk.c_str(),
                            nBinsX_dphi, 0, xMax_phi);

                    std::string name_h_dphi_EPn3_trk = Form("h_dphi_EPn3_trk_%s", name_h_suffix.c_str());
                    std::string title_h_dphi_EPn3_trk = Form("%s;#Delta#phi_{EPn3,%s};", title_h_suffix.c_str(),
                            text_trk.c_str());

                    h_dphi_EPn3_trk[i][j][k] = 0;
                    h_dphi_EPn3_trk[i][j][k] = new TH1D(name_h_dphi_EPn3_trk.c_str(), title_h_dphi_EPn3_trk.c_str(),
                            nBinsX_dphi, 0, xMax_phi);

                    std::string title_h_suffixNoDphi = Form("%s, %s, %s, %s, %s", text_range_vPt.c_str(),
                            text_range_trkPt.c_str(),
                            text_range_trkEta.c_str(),
                            text_range_deta.c_str(),
                            text_range_cent.c_str());

                    std::string name_h_dphi_EPn2_trk_noDphi = Form("h_dphi_EPn2_trk_noDphi_%s", name_h_suffix.c_str());
                    std::string title_h_dphi_EPn2_trk_noDphi = Form("%s;#Delta#phi_{EPn2,%s};", title_h_suffixNoDphi.c_str(),
                            text_trk.c_str());

                    h_dphi_EPn2_trk_noDphi[i][j][k] = 0;
                    h_dphi_EPn2_trk_noDphi[i][j][k] = new TH1D(name_h_dphi_EPn2_trk_noDphi.c_str(), title_h_dphi_EPn2_trk_noDphi.c_str(),
                            nBinsX_dphi, 0, xMax_phi);

                    std::string name_h_dphi_phi0_trk = Form("h_dphi_phi0_trk_%s", name_h_suffix.c_str());
                    std::string title_h_dphi_phi0_trk = Form("%s;#Delta#phi_{#phi_{0},%s};", title_h_suffix.c_str(),
                            text_trk.c_str());

                    h_dphi_phi0_trk[i][j][k] = 0;
                    h_dphi_phi0_trk[i][j][k] = new TH1D(name_h_dphi_phi0_trk.c_str(), title_h_dphi_phi0_trk.c_str(),
                            nBinsX_dphi, 0, xMax_phi);

                    std::string name_h_dphi_phi0_trk_noDphi = Form("h_dphi_phi0_trk_noDphi_%s", name_h_suffix.c_str());
                    std::string title_h_dphi_phi0_trk_noDphi = Form("%s;#Delta#phi_{#phi_{0},%s};", title_h_suffixNoDphi.c_str(),
                            text_trk.c_str());

                    h_dphi_phi0_trk_noDphi[i][j][k] = 0;
                    h_dphi_phi0_trk_noDphi[i][j][k] = new TH1D(name_h_dphi_phi0_trk_noDphi.c_str(), title_h_dphi_phi0_trk_noDphi.c_str(),
                            nBinsX_dphi, 0, xMax_phi);

                    title_h_suffix_dphi = Form("%s, %s, %s, %s, %s", text_range_vPt.c_str(),
                            text_range_trkPt.c_str(),
                            text_range_trkEta.c_str(),
                            text_range_deta.c_str(),
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

                    title_h_suffix = Form("%s, %s, %s, %s, %s, %s", text_range_vPt.c_str(),
                            text_range_trkPt.c_str(),
                            text_range_trkEta.c_str(),
                            text_range_dphi.c_str(),
                            text_range_deta.c_str(),
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

                    title_h_suffix = Form("%s, %s, %s, %s, %s", text_range_vPt.c_str(),
                            text_range_trkPt.c_str(),
                            text_range_trkEta.c_str(),
                            text_range_deta.c_str(),
                            text_range_cent.c_str());

                    if (anavTrk_dR) {
                        std::string name_h_dR = Form("h_dR_%s_%s_%s", label_vPt.c_str(), label_trkPt.c_str(), label_cent.c_str());
                        std::string title_h_dR = Form("%s;%s;", title_h_suffix.c_str(),
                                text_dR.c_str());
                        h_dR[i][j][k] = 0;
                        h_dR[i][j][k] = new TH1D(name_h_dR.c_str(), title_h_dR.c_str(), 20, 0, xMax_phi);
                    }

                    title_h_suffix = Form("%s, %s, %s, %s, %s", text_range_vPt.c_str(),
                            text_range_trkPt.c_str(),
                            text_range_trkEta.c_str(),
                            text_range_deta.c_str(),
                            text_range_cent.c_str());

                    std::string text_zh = Form("z_{%s%s}", text_trk.c_str(),
                            text_V.c_str());
                    std::string text_defn_zh = Form("%s = %s / %s", text_zh.c_str(),
                            text_trkPt.c_str(),
                            text_vPt.c_str());

                    title_h_suffix = Form("%s, %s, %s, %s, %s, %s", text_range_vPt.c_str(),
                            text_range_trkPt.c_str(),
                            text_range_trkEta.c_str(),
                            text_range_dphi.c_str(),
                            text_range_deta.c_str(),
                            text_range_cent.c_str());

                    if (anavTrk_zh) {
                        std::string name_h_zh = Form("h_zh_%s_%s_%s", label_vPt.c_str(), label_trkPt.c_str(), label_cent.c_str());
                        std::string title_h_zh = Form("%s;%s;", title_h_suffix.c_str(),
                                text_defn_zh.c_str());

                        int nBins_zh = 10;
                        std::vector<double> binsVecTmp = calcBinsLogScale(0.01, 1, nBins_zh);
                        double binsArrTmp_zh[nBins_zh+1];
                        std::copy(binsVecTmp.begin(), binsVecTmp.end(), binsArrTmp_zh);

                        h_zh[i][j][k] = 0;
                        h_zh[i][j][k] = new TH1D(name_h_zh.c_str(), title_h_zh.c_str(), nBins_zh, binsArrTmp_zh);

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
                    }

                    std::string name_h_xivh = Form("h_xivh_%s_%s_%s", label_vPt.c_str(), label_trkPt.c_str(), label_cent.c_str());
                    std::string title_h_xivh = Form("%s;%s;", title_h_suffix.c_str(),
                            text_defn_xivh.c_str());

                    h_xivh[i][j][k] = 0;
                    h_xivh[i][j][k] = new TH1D(name_h_xivh.c_str(), title_h_xivh.c_str(), nBins_xivh, 0, 5);

                    std::string name_h_xitvh = Form("h_xitvh_%s_%s_%s", label_vPt.c_str(), label_trkPt.c_str(), label_cent.c_str());
                    std::string title_h_xitvh = Form("%s;%s;", title_h_suffix.c_str(),
                                                               text_defn_xitvh.c_str());

                    h_xitvh[i][j][k] = 0;
                    h_xitvh[i][j][k] = new TH1D(name_h_xitvh.c_str(), title_h_xitvh.c_str(), nBins_xivh, 0, 5);

                    std::string text_dphi_leptrk = Form("#Delta#phi_{%s,lep}", text_trk.c_str());
                    std::string name_h_dphi_leptrk = Form("h_dphi_leptrk_%s", name_h_suffix.c_str());
                    std::string title_h_dphi_leptrk = Form("%s;%s;", title_h_suffix_dphi.c_str(),
                            text_dphi_leptrk.c_str());

                    h_dphi_leptrk[i][j][k] = 0;
                    h_dphi_leptrk[i][j][k] = new TH1D(name_h_dphi_leptrk.c_str(), title_h_dphi_leptrk.c_str(), nBinsX_dphi, 0, xMax_phi);

                    std::string text_dR_leptrk = Form("#DeltaR_{%s,lep}", text_trk.c_str());
                    std::string name_h_dR_leptrk = Form("h_dR_leptrk_%s", name_h_suffix.c_str());
                    std::string title_h_dR_leptrk = Form("%s;%s;", title_h_suffix_dphi.c_str(),
                            text_dR_leptrk.c_str());

                    h_dR_leptrk[i][j][k] = 0;
                    h_dR_leptrk[i][j][k] = new TH1D(name_h_dR_leptrk.c_str(), title_h_dR_leptrk.c_str(), nBinsX_dphi, 0, xMax_phi);

                    if (fillBootStrap) {

                        std::string name_h2_bs_dphi_rebin = Form("h2_bs_dphi_rebin_%s", name_h_suffix.c_str());
                        std::string title_h2_bs_dphi_rebin = Form("%s;%s;event index", title_h_suffix_dphi.c_str(), text_dphi.c_str());

                        int nBinsX_dphi_rebin = binsX_dphi_rebin.size()-1;

                        double arrTmp_bs_dphi[nBinsX_dphi_rebin+1];
                        std::copy(binsX_dphi_rebin.begin(), binsX_dphi_rebin.end(), arrTmp_bs_dphi);

                        h2_bs_dphi_rebin[i][j][k] = 0;
                        h2_bs_dphi_rebin[i][j][k] = new TH2D(name_h2_bs_dphi_rebin.c_str(), title_h2_bs_dphi_rebin.c_str(),
                                                             nBinsX_dphi_rebin, arrTmp_bs_dphi, nEvtsBS, 0, nEvtsBS);
                        vec_h2D.push_back(h2_bs_dphi_rebin[i][j][k]);
                        evtIndex_bs_dphi_rebin[i][j][k] = -1;

                        std::string name_h2_bs_xivh = Form("h2_bs_xivh_%s", name_h_suffix.c_str());
                        std::string title_h2_bs_xivh = Form("%s;%s;event index", title_h_suffix_dphi.c_str(), text_xivh.c_str());

                        h2_bs_xivh[i][j][k] = 0;
                        h2_bs_xivh[i][j][k] = new TH2D(name_h2_bs_xivh.c_str(), title_h2_bs_xivh.c_str(),
                                                       nBins_xivh, 0, 5, nEvtsBS, 0, nEvtsBS);
                        vec_h2D.push_back(h2_bs_xivh[i][j][k]);
                        evtIndex_bs_xivh[i][j][k] = -1;

                        if (k == 0) {
                            std::string name_h_suffix_trkPt = Form("%s_%s", label_vPt.c_str(), label_cent.c_str());
                            std::string title_h_suffix_trkPt = Form("%s, %s", text_range_vPt.c_str(), text_range_cent.c_str());

                            std::string name_h2_bs_trkPt_rebin = Form("h2_bs_trkPt_rebin_%s", name_h_suffix_trkPt.c_str());
                            std::string title_h2_bs_trkPt_rebin = Form("%s;%s;event index", title_h_suffix_trkPt.c_str(), text_trkPt.c_str());

                            int nBinsX_trkPt_rebin = binsX_trkPt_rebin.size()-1;

                            double arrTmp_bs_trkPt[nBinsX_trkPt_rebin+1];
                            std::copy(binsX_trkPt_rebin.begin(), binsX_trkPt_rebin.end(), arrTmp_bs_trkPt);

                            h2_bs_trkPt_rebin[i][j] = 0;
                            h2_bs_trkPt_rebin[i][j] = new TH2D(name_h2_bs_trkPt_rebin.c_str(), title_h2_bs_trkPt_rebin.c_str(),
                                                               nBinsX_trkPt_rebin, arrTmp_bs_trkPt, nEvtsBS, 0, nEvtsBS);
                            vec_h2D.push_back(h2_bs_trkPt_rebin[i][j]);
                            evtIndex_bs_trkPt_rebin[i][j] = -1;

                            std::string name_h2_bs_trkPt_rebin2 = Form("h2_bs_trkPt_rebin2_%s", name_h_suffix_trkPt.c_str());
                            std::string title_h2_bs_trkPt_rebin2 = Form("%s;%s;event index", title_h_suffix_trkPt.c_str(), text_trkPt.c_str());

                            int nBinsX_trkPt_rebin2 = binsX_trkPt_rebin2.size()-1;

                            double arrTmp_bs_trkPt2[nBinsX_trkPt_rebin2+1];
                            std::copy(binsX_trkPt_rebin2.begin(), binsX_trkPt_rebin2.end(), arrTmp_bs_trkPt2);

                            h2_bs_trkPt_rebin2[i][j] = 0;
                            h2_bs_trkPt_rebin2[i][j] = new TH2D(name_h2_bs_trkPt_rebin2.c_str(), title_h2_bs_trkPt_rebin2.c_str(),
                                                               nBinsX_trkPt_rebin2, arrTmp_bs_trkPt2, nEvtsBS, 0, nEvtsBS);
                            vec_h2D.push_back(h2_bs_trkPt_rebin2[i][j]);
                            evtIndex_bs_trkPt_rebin2[i][j] = -1;
                        }
                    }

                    if (anaTrkID) {
                        h_trkPtError[i][j][k] = new TH1D(Form("h_trkPtError_%s", name_h_suffix.c_str()), ";trkPtError;", 30, 0, 0.15);
                        h_trkPtoErr[i][j][k] = new TH1D(Form("h_trkPtoErr_%s", name_h_suffix.c_str()), ";trkPtError / trkPt;", 40, 0, 0.1);
                        h_trkDz1[i][j][k] = new TH1D(Form("h_trkDz1_%s", name_h_suffix.c_str()), ";trkDz1;", 40, 0, 0.2);
                        h_trkDz1oErr[i][j][k] = new TH1D(Form("h_trkDz1oErr_%s", name_h_suffix.c_str()), ";trkDz1 / trkDzError1;", 61, 0, 3.05);
                        h_trkDxy1[i][j][k] = new TH1D(Form("h_trkDxy1_%s", name_h_suffix.c_str()), ";trkDxy1;", 40, 0, 0.1);
                        h_trkDxy1oErr[i][j][k] = new TH1D(Form("h_trkDxy1oErr_%s", name_h_suffix.c_str()), ";trkDxy1 / trkDxyError1;", 61, 0, 3.05);
                        h_trkNHit[i][j][k] = new TH1D(Form("h_trkNHit_%s", name_h_suffix.c_str()), ";trkNHit;", 50, 0, 50);
                        h_trkNdof[i][j][k] = new TH1D(Form("h_trkNdof_%s", name_h_suffix.c_str()), ";trkNdof;", 70, 0, 70);
                        h_trkNlayer[i][j][k] = new TH1D(Form("h_trkNlayer_%s", name_h_suffix.c_str()), ";trkNlayer;", 22, 0, 22);
                        h_trkChi2[i][j][k] = new TH1D(Form("h_trkChi2_%s", name_h_suffix.c_str()), ";trkChi2;", 40, 0, 120);
                        h_trkChi2_Ndof_Nlayer[i][j][k] = new TH1D(Form("h_trkChi2_Ndof_Nlayer_%s", name_h_suffix.c_str()), ";trkChi2 / trkNdof / trkNlayer;", 40, 0, 0.2);
                        h_trkAlgo[i][j][k] = new TH1D(Form("h_trkAlgo_%s", name_h_suffix.c_str()), ";trkAlgo;", 50, 0, 50);
                        h_trkMVA[i][j][k] = new TH1D(Form("h_trkMVA_%s", name_h_suffix.c_str()), ";trkMVA;", 42, 0, 1.05);
                    }

                    std::string name_h2_trkPhi_vs_trkEta = Form("h2_trkPhi_vs_trkEta_%s", name_h_suffix.c_str());
                    std::string title_h2_trkPhi_vs_trkEta = Form("%s;%s;%s", title_h_suffix.c_str(),
                            text_trkEta.c_str(),
                            text_trkPhi.c_str());

                    h2_trkPhi_vs_trkEta[i][j][k] = 0;
                    h2_trkPhi_vs_trkEta[i][j][k] = new TH2D(name_h2_trkPhi_vs_trkEta.c_str(), title_h2_trkPhi_vs_trkEta.c_str(),
                            nBinsX_eta, -1*xMax_eta, xMax_eta,
                            nBinsX_dphi, -1*xMax_phi, xMax_phi);
                    vec_h2D.push_back(h2_trkPhi_vs_trkEta[i][j][k]);

                    std::string name_h2_trkPhi_vs_trkEta_noDphi = Form("h2_trkPhi_vs_trkEta_noDphi_%s", name_h_suffix.c_str());
                    std::string title_h2_trkPhi_vs_trkEta_noDphi = Form("%s;%s;%s", title_h_suffixNoDphi.c_str(),
                            text_trkEta.c_str(),
                            text_trkPhi.c_str());

                    h2_trkPhi_vs_trkEta_noDphi[i][j][k] = 0;
                    h2_trkPhi_vs_trkEta_noDphi[i][j][k] = new TH2D(name_h2_trkPhi_vs_trkEta_noDphi.c_str(), title_h2_trkPhi_vs_trkEta_noDphi.c_str(),
                            nBinsX_eta, -1*xMax_eta, xMax_eta,
                            nBinsX_dphi, -1*xMax_phi, xMax_phi);
                    vec_h2D.push_back(h2_trkPhi_vs_trkEta_noDphi[i][j][k]);

                    std::string name_h2_deta_vs_dphi = Form("h2_deta_vs_dphi_%s", name_h_suffix.c_str());
                    std::string title_h2_deta_vs_dphi = Form("%s;%s;%s", title_h_suffix_dphi.c_str(),
                            text_dphi.c_str(),
                            text_deta.c_str());

                    h2_deta_vs_dphi[i][j][k] = 0;
                    h2_deta_vs_dphi[i][j][k] = new TH2D(name_h2_deta_vs_dphi.c_str(), title_h2_deta_vs_dphi.c_str(),
                            nBinsX_dphi, 0, xMax_phi,
                            nBinsX_eta, 0, 5.2);
                    vec_h2D.push_back(h2_deta_vs_dphi[i][j][k]);

                    std::string name_h2_deta_vs_dphi_rebin1 = Form("h2_deta_vs_dphi_rebin1_%s", name_h_suffix.c_str());
                    std::string title_h2_deta_vs_dphi_rebin1 = Form("%s;%s;%s", title_h_suffix_dphi.c_str(),
                            text_dphi.c_str(),
                            text_deta.c_str());

                    h2_deta_vs_dphi_rebin1[i][j][k] = 0;
                    h2_deta_vs_dphi_rebin1[i][j][k] = new TH2D(name_h2_deta_vs_dphi_rebin1.c_str(), title_h2_deta_vs_dphi_rebin1.c_str(),
                            nBinsX_dphi/2, 0, xMax_phi,
                            nBinsX_eta/2, 0, 5.2);
                    vec_h2D.push_back(h2_deta_vs_dphi_rebin1[i][j][k]);

                    std::string name_h2_deta_vs_dphi_rebin2 = Form("h2_deta_vs_dphi_rebin2_%s", name_h_suffix.c_str());
                    std::string title_h2_deta_vs_dphi_rebin2 = Form("%s;%s;%s", title_h_suffix_dphi.c_str(),
                            text_dphi.c_str(),
                            text_deta.c_str());

                    h2_deta_vs_dphi_rebin2[i][j][k] = 0;
                    h2_deta_vs_dphi_rebin2[i][j][k] = new TH2D(name_h2_deta_vs_dphi_rebin2.c_str(), title_h2_deta_vs_dphi_rebin2.c_str(),
                            6, 0, xMax_phi,
                            8, 0, 5.2);
                    vec_h2D.push_back(h2_deta_vs_dphi_rebin2[i][j][k]);

                    if (findLeadingTrk) {
                        std::string name_h2_deta_h1_vs_dphi = Form("h2_deta_h1_vs_dphi_%s", name_h_suffix.c_str());
                        std::string title_h2_deta_h1_vs_dphi = Form("%s;%s;%s", title_h_suffix_dphi.c_str(),
                                text_dphi.c_str(),
                                text_deta_h1.c_str());

                        h2_deta_h1_vs_dphi[i][j][k] = 0;
                        h2_deta_h1_vs_dphi[i][j][k] = new TH2D(name_h2_deta_h1_vs_dphi.c_str(), title_h2_deta_h1_vs_dphi.c_str(),
                                nBinsX_dphi, 0, xMax_phi,
                                nBinsX_eta, 0, 5.2);
                        vec_h2D.push_back(h2_deta_h1_vs_dphi[i][j][k]);
                    }

                    if (j == 0) {
                        std::string name_h2_dphi_vs_vPt = Form("h2_dphi_vs_vPt_%s_%s", label_trkPt.c_str(), label_cent.c_str());
                        std::string title_h2_dphi_vs_vPt = Form("%s;%s;%s", title_h_suffix_dphi.c_str(),
                                text_vPt.c_str(),
                                text_dphi.c_str());

                        h2_dphi_vs_vPt[i][k] = 0;
                        h2_dphi_vs_vPt[i][k] = new TH2D(name_h2_dphi_vs_vPt.c_str(), title_h2_dphi_vs_vPt.c_str(),
                                nBinsX_vPt, 0,  xMax_vPt,
                                nBinsX_dphi, 0, xMax_phi);
                        vec_h2D.push_back(h2_dphi_vs_vPt[i][k]);
                    }

                    std::string name_h2_dphi_vs_trkEta = Form("h2_dphi_vs_trkEta_%s", name_h_suffix.c_str());
                    std::string title_h2_dphi_vs_trkEta = Form("%s;%s;%s", title_h_suffix_dphi.c_str(),
                            text_trkEta.c_str(),
                            text_dphi.c_str());

                    h2_dphi_vs_trkEta[i][j][k] = 0;
                    h2_dphi_vs_trkEta[i][j][k] = new TH2D(name_h2_dphi_vs_trkEta.c_str(), title_h2_dphi_vs_trkEta.c_str(),
                            nBinsX_eta, -1*xMax_eta,  xMax_eta,
                            nBinsX_dphi, 0, xMax_phi);
                    vec_h2D.push_back(h2_dphi_vs_trkEta[i][j][k]);

                    std::string name_h2_dphi_vs_trkPhi = Form("h2_dphi_vs_trkPhi_%s", name_h_suffix.c_str());
                    std::string title_h2_dphi_vs_trkPhi = Form("%s;%s;%s", title_h_suffix_dphi.c_str(),
                            text_trkPhi.c_str(),
                            text_dphi.c_str());

                    h2_dphi_vs_trkPhi[i][j][k] = 0;
                    h2_dphi_vs_trkPhi[i][j][k] = new TH2D(name_h2_dphi_vs_trkPhi.c_str(), title_h2_dphi_vs_trkPhi.c_str(),
                            nBinsX_dphi, -1*xMax_phi, xMax_phi,
                            nBinsX_dphi, 0, xMax_phi);
                    vec_h2D.push_back(h2_dphi_vs_trkPhi[i][j][k]);

                    std::string name_h2_dphi_vs_detall = Form("h2_dphi_vs_detall_%s", name_h_suffix.c_str());
                    std::string title_h2_dphi_vs_detall = Form("%s;%s;%s", title_h_suffix_dphi.c_str(),
                            "deta_ll",
                            text_dphi.c_str());

                    h2_dphi_vs_detall[i][j][k] = 0;
                    h2_dphi_vs_detall[i][j][k] = new TH2D(name_h2_dphi_vs_detall.c_str(), title_h2_dphi_vs_detall.c_str(),
                            nBinsX_eta, 0, 5.2,
                            nBinsX_dphi, 0, xMax_phi);
                    vec_h2D.push_back(h2_dphi_vs_detall[i][j][k]);

                    std::string name_h2_dphi_vs_dphill = Form("h2_dphi_vs_dphill_%s", name_h_suffix.c_str());
                    std::string title_h2_dphi_vs_dphill = Form("%s;%s;%s", title_h_suffix_dphi.c_str(),
                            "dphi_ll",
                            text_dphi.c_str());

                    h2_dphi_vs_dphill[i][j][k] = 0;
                    h2_dphi_vs_dphill[i][j][k] = new TH2D(name_h2_dphi_vs_dphill.c_str(), title_h2_dphi_vs_dphill.c_str(),
                            nBinsX_dphi, 0, xMax_phi,
                            nBinsX_dphi, 0, xMax_phi);
                    vec_h2D.push_back(h2_dphi_vs_dphill[i][j][k]);

                    std::string name_h2_dphi_vs_vY = Form("h2_dphi_vs_vY_%s", name_h_suffix.c_str());
                    std::string title_h2_dphi_vs_vY = Form("%s;%s;%s", title_h_suffix_dphi.c_str(),
                            text_vY.c_str(),
                            text_dphi.c_str());

                    h2_dphi_vs_vY[i][j][k] = 0;
                    h2_dphi_vs_vY[i][j][k] = new TH2D(name_h2_dphi_vs_vY.c_str(), title_h2_dphi_vs_vY.c_str(),
                            nBinsX_eta, -2.6, 2.6,
                            nBinsX_dphi, 0, xMax_phi);
                    vec_h2D.push_back(h2_dphi_vs_vY[i][j][k]);

                    std::string name_h2_dphi_vs_vPhi = Form("h2_dphi_vs_vPhi_%s", name_h_suffix.c_str());
                    std::string title_h2_dphi_vs_vPhi = Form("%s;%s;%s", title_h_suffix_dphi.c_str(),
                            text_vPhi.c_str(),
                            text_dphi.c_str());

                    h2_dphi_vs_vPhi[i][j][k] = 0;
                    h2_dphi_vs_vPhi[i][j][k] = new TH2D(name_h2_dphi_vs_vPhi.c_str(), title_h2_dphi_vs_vPhi.c_str(),
                            nBinsX_dphi, -1*xMax_phi, xMax_phi,
                            nBinsX_dphi, 0, xMax_phi);
                    vec_h2D.push_back(h2_dphi_vs_vPhi[i][j][k]);

                    std::string name_h2_dphi_vs_l1Pt = Form("h2_dphi_vs_l1Pt_%s", name_h_suffix.c_str());
                    std::string title_h2_dphi_vs_l1Pt = Form("%s;%s;%s", title_h_suffix_dphi.c_str(),
                            "p_T^{l1}",
                            text_dphi.c_str());

                    if (anaTrkLep) {

                        std::string name_h2_dphi_vs_detal1 = Form("h2_dphi_vs_detal1_%s", name_h_suffix.c_str());
                        std::string title_h2_dphi_vs_detal1 = Form("%s;%s;%s", title_h_suffix_dphi.c_str(),
                                "deta_l1",
                                text_dphi.c_str());

                        h2_dphi_vs_detal1[i][j][k] = 0;
                        h2_dphi_vs_detal1[i][j][k] = new TH2D(name_h2_dphi_vs_detal1.c_str(), title_h2_dphi_vs_detal1.c_str(),
                                nBinsX_eta, 0, 5.2,
                                nBinsX_dphi, 0, xMax_phi);
                        vec_h2D.push_back(h2_dphi_vs_detal1[i][j][k]);

                        std::string name_h2_dphi_vs_detal2 = Form("h2_dphi_vs_detal2_%s", name_h_suffix.c_str());
                        std::string title_h2_dphi_vs_detal2 = Form("%s;%s;%s", title_h_suffix_dphi.c_str(),
                                "deta_l2",
                                text_dphi.c_str());

                        h2_dphi_vs_detal2[i][j][k] = 0;
                        h2_dphi_vs_detal2[i][j][k] = new TH2D(name_h2_dphi_vs_detal2.c_str(), title_h2_dphi_vs_detal2.c_str(),
                                nBinsX_eta, 0, 5.2,
                                nBinsX_dphi, 0, xMax_phi);
                        vec_h2D.push_back(h2_dphi_vs_detal2[i][j][k]);

                        h2_dphi_vs_l1Pt[i][j][k] = 0;
                        h2_dphi_vs_l1Pt[i][j][k] = new TH2D(name_h2_dphi_vs_l1Pt.c_str(), title_h2_dphi_vs_l1Pt.c_str(),
                                nBinsX_vPt, 0, xMax_vPt,
                                nBinsX_dphi, 0, xMax_phi);
                        vec_h2D.push_back(h2_dphi_vs_l1Pt[i][j][k]);

                        std::string name_h2_dphi_vs_l2Pt = Form("h2_dphi_vs_l2Pt_%s", name_h_suffix.c_str());
                        std::string title_h2_dphi_vs_l2Pt = Form("%s;%s;%s", title_h_suffix_dphi.c_str(),
                                "p_T^{l2}",
                                text_dphi.c_str());

                        h2_dphi_vs_l2Pt[i][j][k] = 0;
                        h2_dphi_vs_l2Pt[i][j][k] = new TH2D(name_h2_dphi_vs_l2Pt.c_str(), title_h2_dphi_vs_l2Pt.c_str(),
                                nBinsX_vPt, 0, xMax_vPt,
                                nBinsX_dphi, 0, xMax_phi);
                        vec_h2D.push_back(h2_dphi_vs_l2Pt[i][j][k]);

                        std::string name_h2_dphi_vs_l1Eta = Form("h2_dphi_vs_l1Eta_%s", name_h_suffix.c_str());
                        std::string title_h2_dphi_vs_l1Eta = Form("%s;%s;%s", title_h_suffix_dphi.c_str(),
                                "#eta^{l1}",
                                text_dphi.c_str());

                        h2_dphi_vs_l1Eta[i][j][k] = 0;
                        h2_dphi_vs_l1Eta[i][j][k] = new TH2D(name_h2_dphi_vs_l1Eta.c_str(), title_h2_dphi_vs_l1Eta.c_str(),
                                nBinsX_eta, -2.6, 2.6,
                                nBinsX_dphi, 0, xMax_phi);
                        vec_h2D.push_back(h2_dphi_vs_l1Eta[i][j][k]);

                        std::string name_h2_dphi_vs_l2Eta = Form("h2_dphi_vs_l2Eta_%s", name_h_suffix.c_str());
                        std::string title_h2_dphi_vs_l2Eta = Form("%s;%s;%s", title_h_suffix_dphi.c_str(),
                                "#eta^{l2}",
                                text_dphi.c_str());

                        h2_dphi_vs_l2Eta[i][j][k] = 0;
                        h2_dphi_vs_l2Eta[i][j][k] = new TH2D(name_h2_dphi_vs_l2Eta.c_str(), title_h2_dphi_vs_l2Eta.c_str(),
                                nBinsX_eta, -2.6, 2.6,
                                nBinsX_dphi, 0, xMax_phi);
                        vec_h2D.push_back(h2_dphi_vs_l2Eta[i][j][k]);

                        std::string name_h2_dphi_vs_l1Phi = Form("h2_dphi_vs_l1Phi_%s", name_h_suffix.c_str());
                        std::string title_h2_dphi_vs_l1Phi = Form("%s;%s;%s", title_h_suffix_dphi.c_str(),
                                "#phi^{l1}",
                                text_dphi.c_str());

                        h2_dphi_vs_l1Phi[i][j][k] = 0;
                        h2_dphi_vs_l1Phi[i][j][k] = new TH2D(name_h2_dphi_vs_l1Phi.c_str(), title_h2_dphi_vs_l1Phi.c_str(),
                                nBinsX_dphi, -1*xMax_phi, xMax_phi,
                                nBinsX_dphi, 0, xMax_phi);
                        vec_h2D.push_back(h2_dphi_vs_l1Phi[i][j][k]);

                        std::string name_h2_dphi_vs_l2Phi = Form("h2_dphi_vs_l2Phi_%s", name_h_suffix.c_str());
                        std::string title_h2_dphi_vs_l2Phi = Form("%s;%s;%s", title_h_suffix_dphi.c_str(),
                                "#phi^{l2}",
                                text_dphi.c_str());

                        h2_dphi_vs_l2Phi[i][j][k] = 0;
                        h2_dphi_vs_l2Phi[i][j][k] = new TH2D(name_h2_dphi_vs_l2Phi.c_str(), title_h2_dphi_vs_l2Phi.c_str(),
                                nBinsX_dphi, -1*xMax_phi, xMax_phi,
                                nBinsX_dphi, 0, xMax_phi);
                        vec_h2D.push_back(h2_dphi_vs_l2Phi[i][j][k]);
                    }

                    std::string name_h2_deta_vs_xivh = Form("h2_deta_vs_xivh_%s", name_h_suffix.c_str());
                    std::string title_h2_deta_vs_xivh = Form("%s;%s;%s", title_h_suffix_dphi.c_str(),
                            text_xivh.c_str(),
                            text_deta.c_str());

                    h2_deta_vs_xivh[i][j][k] = 0;
                    h2_deta_vs_xivh[i][j][k] = new TH2D(name_h2_deta_vs_xivh.c_str(), title_h2_deta_vs_xivh.c_str(),
                            nBins_xivh, 0, 5,
                            nBinsX_eta, 0, 5.2);
                    vec_h2D.push_back(h2_deta_vs_xivh[i][j][k]);

                    if (findLeadingTrk) {
                        std::string name_h2_deta_h1_vs_xivh = Form("h2_deta_h1_vs_xivh_%s", name_h_suffix.c_str());
                        std::string title_h2_deta_h1_vs_xivh = Form("%s;%s;%s", title_h_suffix_dphi.c_str(),
                                text_xivh.c_str(),
                                text_deta_h1.c_str());

                        h2_deta_h1_vs_xivh[i][j][k] = 0;
                        h2_deta_h1_vs_xivh[i][j][k] = new TH2D(name_h2_deta_h1_vs_xivh.c_str(), title_h2_deta_h1_vs_xivh.c_str(),
                                nBins_xivh, 0, 5,
                                nBinsX_eta, 0, 5.2);
                        vec_h2D.push_back(h2_deta_h1_vs_xivh[i][j][k]);
                    }

                    if (j == 0) {
                        std::string name_h2_vPt_vs_xivh = Form("h2_vPt_vs_xivh_%s_%s", label_trkPt.c_str(), label_cent.c_str());
                        std::string title_h2_vPt_vs_xivh = Form("%s;%s;%s", title_h_suffix_dphi.c_str(),
                                text_xivh.c_str(),
                                text_vPt.c_str());

                        h2_vPt_vs_xivh[i][k] = 0;
                        h2_vPt_vs_xivh[i][k] = new TH2D(name_h2_vPt_vs_xivh.c_str(), title_h2_vPt_vs_xivh.c_str(),
                                nBins_xivh, 0, 5,
                                nBinsX_vPt, 0,  xMax_vPt);
                        vec_h2D.push_back(h2_vPt_vs_xivh[i][k]);
                    }

                    if (isMC) {

                        std::string name_h2_vReco_vs_vGen_xivh = Form("h2_vReco_vs_vGen_xivh_%s", name_h_suffix.c_str());
                        std::string title_h2_vReco_vs_vGen_xivh = Form("%s;%s %s^{gen};%s %s^{reco}", title_h_suffix_dphi.c_str(),
                                text_xivh.c_str(), text_V.c_str(),
                                text_xivh.c_str(), text_V.c_str());

                        h2_vReco_vs_vGen_xivh[i][j][k] = 0;
                        h2_vReco_vs_vGen_xivh[i][j][k] = new TH2D(name_h2_vReco_vs_vGen_xivh.c_str(), title_h2_vReco_vs_vGen_xivh.c_str(),
                                nBins_xivh, 0, 5,
                                nBins_xivh, 0, 5);
                        vec_h2D.push_back(h2_vReco_vs_vGen_xivh[i][j][k]);
                    }

                    if (i == 0) {
                        std::string name_h_suffix_noCent = Form("%s_%s", label_vPt.c_str(), label_trkPt.c_str());

                        std::string name_h2_wTrkSum_vs_cent = Form("h2_wTrkSum_vs_halfHiBin_%s", name_h_suffix_noCent.c_str());
                        std::string title_h2_wTrkSum_vs_cent = Form("%s;Centrality (%%);N_{particles}", title_h_suffix.c_str());

                        h2_wTrkSum_vs_cent[j][k] = 0;
                        h2_wTrkSum_vs_cent[j][k] = new TH2D(name_h2_wTrkSum_vs_cent.c_str(), title_h2_wTrkSum_vs_cent.c_str(),
                                40, 0, 100,
                                250, 0, 500);
                        vec_h2D.push_back(h2_wTrkSum_vs_cent[j][k]);

                        std::string name_h2_wTrkSum_vs_cent_noDphi = Form("h2_wTrkSum_vs_halfHiBin_noDphi_%s", name_h_suffix_noCent.c_str());
                        std::string title_h2_wTrkSum_vs_cent_noDphi = Form("%s;Centrality (%%);N_{particles}", title_h_suffixNoDphi.c_str());

                        h2_wTrkSum_vs_cent_noDphi[j][k] = 0;
                        h2_wTrkSum_vs_cent_noDphi[j][k] = new TH2D(name_h2_wTrkSum_vs_cent_noDphi.c_str(), title_h2_wTrkSum_vs_cent_noDphi.c_str(),
                                40, 0, 100,
                                500, 0, 2500);
                        vec_h2D.push_back(h2_wTrkSum_vs_cent_noDphi[j][k]);
                    }


                    if (anaJets) {
                        std::string name_h2_jetpt_vs_xivh = Form("h2_jetpt_vs_xivh_%s", name_h_suffix.c_str());
                        std::string title_h2_jetpt_vs_xivh = Form("%s;%s;%s", title_h_suffix.c_str(),
                                text_xivh.c_str(),
                                "jet p_{T}");

                        h2_jetpt_vs_xivh[i][j][k] = 0;
                        h2_jetpt_vs_xivh[i][j][k] = new TH2D(name_h2_jetpt_vs_xivh.c_str(), title_h2_jetpt_vs_xivh.c_str(),
                                nBins_xivh, 0, 5,
                                60, 0, 300);
                        vec_h2D.push_back(h2_jetpt_vs_xivh[i][j][k]);

                        std::string name_h2_rawpt_vs_xivh = Form("h2_rawpt_vs_xivh_%s", name_h_suffix.c_str());
                        std::string title_h2_rawpt_vs_xivh = Form("%s;%s;%s", title_h_suffix.c_str(),
                                text_xivh.c_str(),
                                "raw p_{T}");

                        h2_rawpt_vs_xivh[i][j][k] = 0;
                        h2_rawpt_vs_xivh[i][j][k] = new TH2D(name_h2_rawpt_vs_xivh.c_str(), title_h2_rawpt_vs_xivh.c_str(),
                                nBins_xivh, 0, 5,
                                60, 0, 300);
                        vec_h2D.push_back(h2_rawpt_vs_xivh[i][j][k]);

                        std::string text_dR_jet_trk = Form("#DeltaR_{jet %s}", text_trk.c_str());

                        std::string name_h2_jetpt_vs_dR = Form("h2_jetpt_vs_dR_%s", name_h_suffix.c_str());
                        std::string title_h2_jetpt_vs_dR = Form("%s;%s;%s", title_h_suffix.c_str(),
                                text_dR_jet_trk.c_str(),
                                "jet p_{T}");

                        h2_jetpt_vs_dR[i][j][k] = 0;
                        h2_jetpt_vs_dR[i][j][k] = new TH2D(name_h2_jetpt_vs_dR.c_str(), title_h2_jetpt_vs_dR.c_str(),
                                20, 0, 1,
                                60, 0, 300);
                        vec_h2D.push_back(h2_jetpt_vs_dR[i][j][k]);

                        std::string name_h2_rawpt_vs_dR = Form("h2_rawpt_vs_dR_%s", name_h_suffix.c_str());
                        std::string title_h2_rawpt_vs_dR = Form("%s;%s;%s", title_h_suffix.c_str(),
                                text_dR_jet_trk.c_str(),
                                "raw p_{T}");

                        h2_rawpt_vs_dR[i][j][k] = 0;
                        h2_rawpt_vs_dR[i][j][k] = new TH2D(name_h2_rawpt_vs_dR.c_str(), title_h2_rawpt_vs_dR.c_str(),
                                20, 0, 1,
                                60, 0, 300);
                        vec_h2D.push_back(h2_rawpt_vs_dR[i][j][k]);

                        std::vector<double> binsX_jetpt = {0, 10, 20, 30, 40, 50, 70, 100, 150, 200, 300};
                        int nBinsX_jetpt = binsX_jetpt.size()-1;

                        double arr_jetpt[nBinsX_jetpt+1];
                        std::copy(binsX_jetpt.begin(), binsX_jetpt.end(), arr_jetpt);

                        std::string name_h2_jetpt_vs_dR_rebin = Form("h2_jetpt_vs_dR_rebin_%s", name_h_suffix.c_str());
                        std::string title_h2_jetpt_vs_dR_rebin = Form("%s;%s;%s", title_h_suffix.c_str(),
                                text_dR_jet_trk.c_str(),
                                "jet p_{T}");

                        h2_jetpt_vs_dR_rebin[i][j][k] = 0;
                        h2_jetpt_vs_dR_rebin[i][j][k] = new TH2D(name_h2_jetpt_vs_dR_rebin.c_str(), title_h2_jetpt_vs_dR_rebin.c_str(),
                                5, 0, 1,
                                nBinsX_jetpt, arr_jetpt);
                        vec_h2D.push_back(h2_jetpt_vs_dR_rebin[i][j][k]);

                        std::string name_h2_rawpt_vs_dR_rebin = Form("h2_rawpt_vs_dR_rebin_%s", name_h_suffix.c_str());
                        std::string title_h2_rawpt_vs_dR_rebin = Form("%s;%s;%s", title_h_suffix.c_str(),
                                text_dR_jet_trk.c_str(),
                                "raw p_{T}");

                        h2_rawpt_vs_dR_rebin[i][j][k] = 0;
                        h2_rawpt_vs_dR_rebin[i][j][k] = new TH2D(name_h2_rawpt_vs_dR_rebin.c_str(), title_h2_rawpt_vs_dR_rebin.c_str(),
                                5, 0, 1,
                                nBinsX_jetpt, arr_jetpt);
                        vec_h2D.push_back(h2_rawpt_vs_dR_rebin[i][j][k]);
                    }
                }
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
    if (vYMax < vYMin) {
        vYMax = 999999;
    }
    for (int i = 0; i < nTrkPts; ++i) {
        if (trkPtsMax[i] < trkPtsMin[i]) {
            trkPtsMax[i] = 999999;
        }
    }
    if (trkEtaMax < trkEtaMin) {
        trkEtaMax = 999999;
    }
    if (vTrkDetaMax < vTrkDetaMin) {
        vTrkDetaMax = 999999;
    }

    if (hTrkDetaMax < hTrkDetaMin) {
        hTrkDetaMax = 999999;
    }

    // pointers to jet info
    int nJets = 0;
    std::vector<float>* p_jetpt = 0;
    std::vector<float>* p_jeteta = 0;
    std::vector<float>* p_jetphi = 0;
    std::vector<float>* p_rawpt = 0;
    std::vector<int>* p_subid = 0;
    std::vector<int>* p_evtjet_index = 0;

    // pointers to particle info
    int nParticles;
    std::vector<float>* p_pt;
    std::vector<float>* p_eta;
    std::vector<float>* p_phi;
    std::vector<int>* p_chg;
    std::vector<int>* p_pid;
    std::vector<int>* p_sube;
    std::vector<float>* p_weight;
    std::vector<int>* p_evtIndex;
    std::vector<float>* p_trkPtError;
    std::vector<float>* p_trkDz1;
    std::vector<float>* p_trkDzError1;
    std::vector<float>* p_trkDxy1;
    std::vector<float>* p_trkDxyError1;
    std::vector<unsigned char>* p_trkNHit;
    std::vector<float>* p_trkChi2;
    std::vector<unsigned char>* p_trkNdof;
    std::vector<unsigned char>* p_trkNlayer;
    std::vector<unsigned char>* p_trkAlgo;
    std::vector<float>* p_trkMVA;

    int nPartRaw;
    std::vector<float>* p_raw_pt;
    std::vector<float>* p_raw_eta;
    std::vector<float>* p_raw_phi;
    std::vector<int>* p_raw_chg;
    std::vector<int>* p_raw_pid;
    //std::vector<int>* p_raw_sube;

    std::vector<int> dummy_vec_I0(150000, 0);
    std::vector<int> dummy_vec_I1(150000, 1);
    std::vector<unsigned char> dummy_vec_UC1(150000, 1);
    std::vector<float> dummy_vec_F1(150000, 1);

    TTree* treeggHiNtuplizer = 0;
    TTree* treeHLT = 0;
    TTree* treeL1obj = 0;
    TTree* treeHiEvt = 0;
    TTree* treeJetSkim = 0;
    TTree* treeTrackSkim = 0;
    TTree* treeEvtSkim = 0;
    TTree* treeHiEvtMix = 0;
    TTree* treeHiFJRho = 0;
    std::vector<TTree*> treesHltObj;

    std::string treePathHLT = "HltTree";
    std::string treePathL1obj = "L1UpgradeFlatTree";
    std::string treePathHiEvt = "HiTree";
    std::string treePathTrack = "trackSkim";
    std::string treePathEvtSkim = "eventSkim";
    std::string treePathHiEvtMix = "mixEventSkim";
    std::string treePathHiFJRho = "hiFJRhoFinerBins";

    bool hasTreeHLT = false;

    std::vector<std::string> triggerBranches;

    triggerBranches = {};
    int nTriggerBranches = triggerBranches.size();

    std::vector<std::string> treeNamesHLTObj = getTreeNamesHLTObj(collisionType, vIsZmm);
    if (isPP13tev) {
        treeNamesHLTObj = getTreeNamesHLTObjpp13TeV();
    }
    if (!isRecoV) {
        treeNamesHLTObj = {};
    }
    int nTreesHLTObj = treeNamesHLTObj.size();
    std::vector<double> l1Thresholds = getL1Thresholds(treeNamesHLTObj, (int)collisionType);

    std::cout << "nTreesHLTObj = " << nTreesHLTObj << std::endl;
    for (int i = 0; i < nTreesHLTObj; ++i) {
        std::cout << Form("treeNamesHLTObj[%d] = %s", i, treeNamesHLTObj[i].c_str()) << std::endl;
        std::cout << Form("l1Thresholds[%d] = %f", i, l1Thresholds[i]) << std::endl;
    }

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
        treePathL1obj = "l1object/L1UpgradeFlatTree";
        treePathHiEvt = "hiEvtAnalyzer/HiTree";
        treePathTrack = "ppTrack/trackTree";
        treePathEvtSkim = "NULL";
        treePathHiEvtMix = "NULL";
        treePathHiFJRho = "hiFJRhoAnalyzerFinerBins/t";
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

    TRandom3 randEntryStart;
    if (doRndEntryStart) {
        randEntryStart.SetSeed(rndEntryStart);
    }

    TRandom3 randEntry;
    if (doRndEntry) {
        randEntry.SetSeed(rndEntry);
    }

    TRandom3 randelePt(1234);

    TRandom3 randvPhi;
    if (doRndVPhi) {
        randvPhi.SetSeed(rndVPhi);
    }

    EventMatcher* em = new EventMatcher();
    Long64_t duplicateEntries = 0;

    Long64_t entries = 0;
    Long64_t entriesAnalyzed = 0;
    Long64_t entriesNoMixEvt = 0;
    double wEvtsAll = 0;
    double wEvtsV = 0;

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
            treeggHiNtuplizer->SetBranchStatus("nPho",1);     // enable photon branches
            treeggHiNtuplizer->SetBranchStatus("pho*",1);     // enable photon branches
        }
        treeggHiNtuplizer->SetBranchStatus("rho",1);

        treeHLT = 0;
        treeHLT = (TTree*)fileTmp->Get(treePathHLT.c_str());
        hasTreeHLT = (treeHLT != 0);

        int triggerBits[nTriggerBranches];
        if (hasTreeHLT) {
            treeHLT->SetBranchStatus("*",0);     // disable all branches
            for (int iTrig = 0; iTrig < nTriggerBranches; ++iTrig) {
                if (treeHLT->GetBranch(triggerBranches[iTrig].c_str())) {
                    treeHLT->SetBranchStatus(triggerBranches[iTrig].c_str(), 1);
                    treeHLT->SetBranchAddress(triggerBranches[iTrig].c_str(), &(triggerBits[iTrig]));
                }
                else {
                    triggerBits[iTrig] = 0;
                }
            }
        }

        treesHltObj.clear();
        treesHltObj.resize(nTreesHLTObj, 0);
        for (int iHltObj = 0; iHltObj < nTreesHLTObj; ++iHltObj) {

            std::string treeNameTmp = Form("hltobject/%s", treeNamesHLTObj[iHltObj].c_str());

            treesHltObj[iHltObj] = 0;
            treesHltObj[iHltObj] = (TTree*)fileTmp->Get(treeNameTmp.c_str());

            treesHltObj[iHltObj]->SetBranchStatus("*", 1);
        }

        treeL1obj = 0;
        treeL1obj = (TTree*)fileTmp->Get(treePathL1obj.c_str());
        treeL1obj->SetBranchStatus("*", 0);
        if (vIsPho || vIsZee) {
            treeL1obj->SetBranchStatus("nEGs", 1);
            treeL1obj->SetBranchStatus("eg*", 1);
        }
        else if (vIsZmm) {
            treeL1obj->SetBranchStatus("nMuons", 1);
            treeL1obj->SetBranchStatus("muon*", 1);
        }

        // specify explicitly which branches to use, do not use wildcard
        treeHiEvt = (TTree*)fileTmp->Get(treePathHiEvt.c_str());
        setBranchStatusTreeHiEvt(treeHiEvt, isMC);

        if (anaJets) {
            treeJetSkim = (TTree*)fileTmp->Get(treePathJetSkim.c_str());
            treeJetSkim->SetBranchStatus("*",0);     // disable all branches
            if (isRecoJet) {
                treeJetSkim->SetBranchStatus("njet*",1);
                treeJetSkim->SetBranchStatus("jetpt*",1);
                treeJetSkim->SetBranchStatus("jeteta*",1);
                treeJetSkim->SetBranchStatus("jetphi*",1);
                treeJetSkim->SetBranchStatus("rawpt*",1);
                treeJetSkim->SetBranchStatus("ref*",1);
                treeJetSkim->SetBranchStatus("subid*",1);
                treeJetSkim->SetBranchStatus("evtjet_mix",1);
            }
            else {
                treeJetSkim->SetBranchStatus("ngen",1);
                treeJetSkim->SetBranchStatus("gen*",1);
            }
        }

        bool anaTreeTrack = ((isvJetTrkSkim && anaTrks) || anaNVtx);
        if (anaTreeTrack) {
            treeTrackSkim = (TTree*)fileTmp->Get(treePathTrack.c_str());
            treeTrackSkim->SetBranchStatus("*",0);     // disable all branches

            if (anaTrks) {
                std::string mix_str = (isMixTrk) ? "_mix" : "";
                if (isRecoTrk) {
                    treeTrackSkim->SetBranchStatus(Form("nTrk%s", mix_str.c_str()),1);
                    treeTrackSkim->SetBranchStatus(Form("trkPt%s", mix_str.c_str()),1);
                    treeTrackSkim->SetBranchStatus(Form("trkEta%s", mix_str.c_str()),1);
                    treeTrackSkim->SetBranchStatus(Form("trkPhi%s", mix_str.c_str()),1);
                    treeTrackSkim->SetBranchStatus(Form("trkCharge%s", mix_str.c_str()),1);
                    if (anaTrkID) {
                        treeTrackSkim->SetBranchStatus(Form("trkPtError%s", mix_str.c_str()),1);
                        treeTrackSkim->SetBranchStatus(Form("trkDz1%s", mix_str.c_str()),1);
                        treeTrackSkim->SetBranchStatus(Form("trkDzError1%s", mix_str.c_str()),1);
                        treeTrackSkim->SetBranchStatus(Form("trkDxy1%s", mix_str.c_str()),1);
                        treeTrackSkim->SetBranchStatus(Form("trkDxyError1%s", mix_str.c_str()),1);
                        treeTrackSkim->SetBranchStatus(Form("trkNHit%s", mix_str.c_str()),1);
                        treeTrackSkim->SetBranchStatus(Form("trkChi2%s", mix_str.c_str()),1);
                        treeTrackSkim->SetBranchStatus(Form("trkNdof%s", mix_str.c_str()),1);
                        treeTrackSkim->SetBranchStatus(Form("trkNlayer%s", mix_str.c_str()),1);
                        treeTrackSkim->SetBranchStatus(Form("trkAlgo%s", mix_str.c_str()),1);
                        treeTrackSkim->SetBranchStatus(Form("trkMVA%s", mix_str.c_str()),1);
                    }
                    treeTrackSkim->SetBranchStatus(Form("trkWeight%s", mix_str.c_str()),1);
                    treeTrackSkim->SetBranchStatus(Form("pfType%s", mix_str.c_str()),1);
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
                    treeTrackSkim->SetBranchStatus(Form("pdg%s", mix_str.c_str()),1);
                    treeTrackSkim->SetBranchStatus(Form("sube%s", mix_str.c_str()),1);
                    treeTrackSkim->SetBranchStatus(Form("evtgen%s", mix_str.c_str()),1);
                }

                if (needRawTrk) {
                    if (isRecoTrk) {
                        treeTrackSkim->SetBranchStatus("nTrk",1);
                        treeTrackSkim->SetBranchStatus("trkPt",1);
                        treeTrackSkim->SetBranchStatus("trkEta",1);
                        treeTrackSkim->SetBranchStatus("trkPhi",1);
                        treeTrackSkim->SetBranchStatus("pfType",1);
                    }
                    else {
                        treeTrackSkim->SetBranchStatus("mult",1);
                        treeTrackSkim->SetBranchStatus("pt",1);
                        treeTrackSkim->SetBranchStatus("eta",1);
                        treeTrackSkim->SetBranchStatus("phi",1);
                        treeTrackSkim->SetBranchStatus("chg",1);
                        treeTrackSkim->SetBranchStatus("pdg",1);
                        treeTrackSkim->SetBranchStatus("sube",1);
                    }
                }
            }

            if (anaNVtx) {
                treeTrackSkim->SetBranchStatus("nVtx",1);
            }
        }

        if (isvJetTrkSkim) {
            treeEvtSkim = (TTree*)fileTmp->Get(treePathEvtSkim.c_str());
            treeEvtSkim->SetBranchStatus("*",0);
            treeEvtSkim->SetBranchStatus("pf_*_HF_*",1);
            treeEvtSkim->SetBranchStatus("pf_*_HE_totE",1);

            treeHiEvtMix = (TTree*)fileTmp->Get(treePathHiEvtMix.c_str());
            treeHiEvtMix->SetBranchStatus("*",0);     // disable all branches
            treeHiEvtMix->SetBranchStatus("nmix",1);
            treeHiEvtMix->SetBranchStatus("hiBin_mix",1);
            treeHiEvtMix->SetBranchStatus("pf_*_HF_*",1);
            if (rotateEvtPlane) {
                treeHiEvtMix->SetBranchStatus("hiEvtPlanes_mix",1);
            }
        }

        if (!isMC && isPbPb18 && vIsZee && false) {
            treeHiFJRho = (TTree*)fileTmp->Get(treePathHiFJRho.c_str());
            treeHiFJRho->SetBranchStatus("*", 0);
            treeHiFJRho->SetBranchStatus("etaM*",1);
            treeHiFJRho->SetBranchStatus("rho*",1);
        }

        if (isMC) {
            treeggHiNtuplizer->SetBranchStatus("nMC*",1);     // enable GEN particle branches
            treeggHiNtuplizer->SetBranchStatus("mc*",1);      // enable GEN particle branches

            treeHiEvt->SetBranchStatus("weight", 1);
            treeHiEvt->SetBranchStatus("pthat",1);
            if (isMCMG && !isvJetTrkSkim) {
                treeHiEvt->SetBranchStatus("ttbar_w", 1);
            }
        }

        ggHiNtuplizer ggHi;
        ggHi.setupTreeForReading(treeggHiNtuplizer);

        std::vector<hltObject> hltObjs(nTreesHLTObj);
        for (int iHltObj = 0; iHltObj < nTreesHLTObj; ++iHltObj) {

            hltObjs[iHltObj].reset();
            hltObjs[iHltObj].setupTreeForReading(treesHltObj[iHltObj]);
        }

        l1Object l1Obj;
        l1Obj.setupTreeForReading(treeL1obj);

        hiEvt hiEvt;
        hiEvt.setupTreeForReading(treeHiEvt);

        jetSkim jets;
        if (anaJets) {
            jets.setupTreeForReading(treeJetSkim);
        }

        trackSkim trks;
        if (anaTreeTrack) {
            trks.setupTreeForReading(treeTrackSkim);
        }
        bool has_pfType = (trks.b_pfType != 0 || trks.b_pfType_mix != 0);

        eventSkim evtskim;
        mixEventSkim mixEvents;
        if (isvJetTrkSkim) {

            evtskim.setupTreeForReading(treeEvtSkim);
            mixEvents.setupTreeForReading(treeHiEvtMix);
        }

        hiFJRho hifjrho;
        if (!isMC && isPbPb18 && vIsZee && false) {
            hifjrho.setupTreeForReading(treeHiFJRho);
        }

        if (isRecoTrk) {
            p_pt = trks.p_trkPt;
            p_eta = trks.p_trkEta;
            p_phi = trks.p_trkPhi;
            p_chg = trks.p_trkCharge;
            p_pid = (has_pfType) ? trks.p_pfType : &dummy_vec_I0;
            p_sube = &dummy_vec_I1;
            p_weight = trks.p_trkWeight;
            p_evtIndex = &dummy_vec_I0;

            p_trkPtError = trks.p_trkPtError;
            p_trkDz1 = trks.p_trkDz1;
            p_trkDzError1 = trks.p_trkDzError1;
            p_trkDxy1 = trks.p_trkDxy1;
            p_trkDxyError1 = trks.p_trkDxyError1;
            p_trkNHit = trks.p_trkNHit;
            p_trkChi2 = trks.p_trkChi2;
            p_trkNdof = trks.p_trkNdof;
            p_trkNlayer = trks.p_trkNlayer;
            p_trkAlgo = trks.p_trkAlgo;
            p_trkMVA = trks.p_trkMVA;

            if (isMixTrk) {
                p_pt = trks.p_trkPt_mix;
                p_eta = trks.p_trkEta_mix;
                p_phi = trks.p_trkPhi_mix;
                p_chg = &dummy_vec_I1;
                p_pid = (has_pfType) ? trks.p_pfType_mix : &dummy_vec_I0;
                p_sube = &dummy_vec_I1;
                p_weight = trks.p_trkWeight_mix;
                p_evtIndex = trks.p_evttrk_mix;

                p_trkPtError = trks.p_trkPtError_mix;
                p_trkDz1 = trks.p_trkDz1_mix;
                p_trkDzError1 = trks.p_trkDzError1_mix;
                p_trkDxy1 = trks.p_trkDxy1_mix;
                p_trkDxyError1 = trks.p_trkDxyError1_mix;
                p_trkNHit = trks.p_trkNHit_mix;
                p_trkChi2 = trks.p_trkChi2_mix;
                p_trkNdof = trks.p_trkNdof_mix;
                p_trkNlayer = trks.p_trkNlayer_mix;
                p_trkAlgo = trks.p_trkAlgo_mix;
                p_trkMVA = trks.p_trkMVA_mix;
            }

            p_raw_pt = trks.p_trkPt;
            p_raw_eta = trks.p_trkEta;
            p_raw_phi = trks.p_trkPhi;
            p_raw_chg = &dummy_vec_I0;
            p_raw_pid = (has_pfType) ? trks.p_pfType : &dummy_vec_I0;
            //p_raw_sube = &dummy_vec_I1;
        }
        else {
            p_pt = trks.p_pt;
            p_eta = trks.p_eta;
            p_phi = trks.p_phi;
            p_chg = trks.p_chg;
            p_pid = trks.p_pdg;
            p_sube = trks.p_sube;
            p_weight = &dummy_vec_F1;
            p_evtIndex = &dummy_vec_I0;

            p_trkPtError = &dummy_vec_F1;
            p_trkDz1 = &dummy_vec_F1;
            p_trkDzError1 = &dummy_vec_F1;
            p_trkDxy1 = &dummy_vec_F1;
            p_trkDxyError1 = &dummy_vec_F1;
            p_trkNHit = &dummy_vec_UC1;
            p_trkChi2 = &dummy_vec_F1;
            p_trkNdof = &dummy_vec_UC1;
            p_trkNlayer = &dummy_vec_UC1;
            p_trkAlgo = &dummy_vec_UC1;
            p_trkMVA = &dummy_vec_F1;

            if (isMixTrk) {
                p_pt = trks.p_pt_mix;
                p_eta = trks.p_eta_mix;
                p_phi = trks.p_phi_mix;
                p_chg = trks.p_chg_mix;
                p_pid = trks.p_pdg_mix;
                p_sube = trks.p_sube_mix;
                p_weight = &dummy_vec_F1;
                p_evtIndex = trks.p_evtgen_mix;
            }

            p_raw_pt = trks.p_pt;
            p_raw_eta = trks.p_eta;
            p_raw_phi = trks.p_phi;
            p_raw_chg = trks.p_chg;
            p_raw_pid = trks.p_pdg;
            //p_raw_sube = trks.p_sube;
        }

        bool canUseMixJets = false;
        bool useSigJets = false;
        if (anaJets) {
            p_jetpt = jets.p_jetpt;
            p_jeteta = jets.p_jeteta;
            p_jetphi = jets.p_jetphi;
            p_rawpt = jets.p_rawpt;
            p_subid = jets.p_subid;
            p_evtjet_index = &dummy_vec_I0;

            if (canUseMixJets && isMixTrk) {
                p_jetpt = jets.p_jetpt_mix;
                p_jeteta = jets.p_jeteta_mix;
                p_jetphi = jets.p_jetphi_mix;
                p_rawpt = jets.p_rawpt_mix;
                p_subid = jets.p_subid_mix;
                p_evtjet_index = jets.p_evtjet_mix;
            }
        }

        Long64_t entriesTmp = treeggHiNtuplizer->GetEntries();
        entries += entriesTmp;
        std::cout << "entries in File = " << entriesTmp << std::endl;

        Long64_t entryStart = 0;
        if (doRndEntryStart) {
            entryStart = (Long64_t)(randEntryStart.Uniform(0, entriesTmp));
        }
        std::vector<Long64_t> randEntries;
        Long64_t entryCount = 0;
        for (Long64_t j_entry = entryStart; entryCount < entriesTmp; ++j_entry)
        {
            if (evtFrac < 1 && entryCount > (entriesTmp*evtFrac)) {
                break;
            }

            if (doRndEntry) {
                j_entry = (Long64_t)(randEntry.Uniform(0, entriesTmp));
                while (findInVector(randEntries, j_entry) >= 0) {
                    j_entry = (Long64_t)(randEntry.Uniform(0, entriesTmp));
                }
                randEntries.push_back(j_entry);
            }

            entryCount++;
            j_entry = (j_entry % entriesTmp);

            if (j_entry % 2000 == 0)  {
              std::cout << "current entry = " <<j_entry<<" out of "<<entriesTmp<<" : "<<std::setprecision(4)<<(double)entryCount/entriesTmp*100<<" %"<<std::endl;
            }

            treeggHiNtuplizer->GetEntry(j_entry);
            if (hasTreeHLT) {
                treeHLT->GetEntry(j_entry);
            }
            for (int iHltObj = 0; iHltObj < nTreesHLTObj; ++iHltObj) {
                treesHltObj[iHltObj]->GetEntry(j_entry);
            }
            treeL1obj->GetEntry(j_entry);

            treeHiEvt->GetEntry(j_entry);
            if (!isMC && isPbPb18 && vIsZee && false) {
                treeHiFJRho->GetEntry(j_entry);
            }

            if (anaTreeTrack) {
                treeTrackSkim->GetEntry(j_entry);
            }

            if (isvJetTrkSkim) {
                if (anaJets) {
                    treeJetSkim->GetEntry(j_entry);
                }

                treeEvtSkim->GetEntry(j_entry);
                treeHiEvtMix->GetEntry(j_entry);

                if (isPbPb && anaTrks)  {
                    if ( mixEvents.nmix > nmixMax ) {
                        std::cout << "WARNING : mixEvents.nmix = " << mixEvents.nmix << " is above max. j_entry = " << j_entry
                                  << " , run = " << hiEvt.run << " , hiEvt.lumi = " << hiEvt.lumi << " , hiEvt.evt = " << hiEvt.evt << std::endl;
                        std::cout << "skipping event " << std::endl;
                        entriesNoMixEvt++;
                        continue;
                    }
                    if ( mixEvents.nmix < nmixMin ) {
                        std::cout << "WARNING : mixEvents.nmix = " << mixEvents.nmix << " is below min. j_entry = " << j_entry
                                << " , run = " << hiEvt.run << " , hiEvt.lumi = " << hiEvt.lumi << " , hiEvt.evt = " << hiEvt.evt << std::endl;
                        std::cout << "skipping event " << std::endl;
                        entriesNoMixEvt++;
                        continue;
                    }
                }
            }

            if (anaNVtx) {
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

            double w = 1;
            int hiBin = hiEvt.hiBin;
            int hiBin0 = hiBin;
            if (shiftHibin) {
                if (hiBin < 3) {
                    continue;
                }
                else {
                    hiBin -= 3;
                }
            }

            entriesAnalyzed++;

            int cent = hiBin/2;
            int cent0 = cent;
            if (doEventWeight > 0) {
                w = hiEvt.weight;
                if (isMCMG && !isvJetTrkSkim) {
                    w = (*hiEvt.ttbar_w)[1080];
                }
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

            int nVCand = 0; // number of V candidates per event
            double vPt = -1;
            double vEta = -999999;
            double vPhi = -999999;
            double vY = -999999;
            double vM = 0;
            bool ll_passSign = false;     // dilepton opposite charge
            std::vector<int> llIndex = {-1, -1};
            std::vector<float> llPt = {-998877, -998877};
            std::vector<float> llEta = {-998877, -998877};
            std::vector<float> llPhi = {-998877, -998877};
            float minDR2_lep_trk = (has_pfType) ? minDR2_lep_trk_PFID : minDR2_lep_trk_noPFID;

            double genVPt = -1;
            double genVY = -999999;
            double genVPhi = -999999;
            double genVMass = -1;
            float maxDR2_reco_gen_V = 9999; //0.16;

            if (!anaV) continue;
            wEvtsAll += w;

            if (vIsPho) {
                for (int i = 0; i < ggHi.nPho; ++i) {

                    if (excludeHI18HEMfailure && !ggHi.passedHI18HEMfailurePho(i))  continue;
                }
            }
            else if (vIsZ) {

                double zmassPDG = 91.1876;
                double deltaMass = 999999;
                int pdgV = 23;
                double massMin = 60;
                double massMax = 120;

                TLorentzVector vecl1;
                TLorentzVector vecl2;
                TLorentzVector vecll;

                double lMass = -1;
                int pdgL = -1;
                int nL = 0;
                std::vector<float> *lPt;
                std::vector<float> *lEta;
                std::vector<float> *lPhi;
                std::vector<int>   *lChg;

                double lPtMin = -1;

                if (vIsZmm) {

                    lMass = 0.105658;
                    pdgL = 13;
                    nL = ggHi.nMu;
                    lPt = ggHi.muPt;
                    lEta = ggHi.muEta;
                    lPhi = ggHi.muPhi;
                    lChg = ggHi.muCharge;
                    lPtMin = 20;
                }
                else if (vIsZee) {

                    lMass = 0.000511;
                    pdgL = 11;
                    nL = ggHi.nEle;
                    lPt = ggHi.elePt;
                    lEta = ggHi.eleEta;
                    lPhi = ggHi.elePhi;
                    lChg = ggHi.eleCharge;
                    lPtMin = 20;
                }

                int iMCLepP = -1;
                int iMCLepM = -1;

                if (!isRecoV) {
                    nL = ggHi.nMC;
                    lPt = ggHi.mcPt;
                    lEta = ggHi.mcEta;
                    lPhi = ggHi.mcPhi;
                    lChg = ggHi.mcPID;
                }
                else if (isMC && isRecoV) {

                    if (vIsZtautau_mm || vIsZtautau_ee) {
                        bool hasTauP = false;
                        bool hasTauM = false;
                        for (int i = 0; i < ggHi.nMC; ++i) {
                            if (std::fabs((*ggHi.mcPID)[i]) == pdgL && (*ggHi.mcMomPID)[i] == 15)  hasTauP = true;
                            if (std::fabs((*ggHi.mcPID)[i]) == pdgL && (*ggHi.mcMomPID)[i] == -15)  hasTauM = true;
                        }
                        if ( !(hasTauP && hasTauM) ) continue;
                    }

                    bool hasLepP = false;
                    bool hasLepM = false;
                    for (int i = 0; i < ggHi.nMC; ++i) {

                        if ( !((*ggHi.mcMomPID)[i] == pdgV) ) continue;
                        if ( !(std::fabs((*ggHi.mcEta)[i]) < lEtaMax) ) continue;

                        if ((*ggHi.mcPID)[i] == pdgL) {
                            iMCLepP = i;
                            if ( ((*ggHi.mcPt)[i] > lPtMin) ) hasLepP = true;
                        }
                        else if ((*ggHi.mcPID)[i] == -1*pdgL) {
                            iMCLepM = i;
                            if ( ((*ggHi.mcPt)[i] > lPtMin) ) hasLepM = true;
                        }
                    }

                    if (hasLepP && hasLepM) {
                        for (int i = 0; i < ggHi.nMC; ++i) {

                            if ((*ggHi.mcPID)[i] != pdgV)  continue;
                            if ((*ggHi.mcStatus)[i] != 62)  continue;

                            genVMass = (*ggHi.mcMass)[i];
                            if ( !(genVMass >= massMin && genVMass <= massMax) ) continue;

                            TLorentzVector vecTmp;
                            vecTmp.SetPtEtaPhiM((*ggHi.mcPt)[i], (*ggHi.mcEta)[i], (*ggHi.mcPhi)[i], genVMass);

                            if ( !(std::fabs(vecTmp.Rapidity()) < lEtaMax) ) continue;  // rap needs to be smaller than eta

                            if (std::fabs(genVMass - zmassPDG) < deltaMass) {
                                deltaMass = std::fabs(genVMass - zmassPDG);

                                genVPt = (*ggHi.mcPt)[i];
                                genVPhi = (*ggHi.mcPhi)[i];

                                genVY = vecTmp.Rapidity();
                            }
                        }
                    }
                }

                deltaMass = 999999;
                for (int i = 0; i < nL; ++i) {

                    float l1pt = (*lPt)[i];
                    if (vIsZmm && isRecoV && !do_sys_les) {

                        if (isMC) {
                            double l1ptgen = l1pt;
                            if (iMCLepP > 0 && iMCLepM > 0) {
                                l1ptgen = ((*lChg)[i] < 0) ? (*ggHi.mcPt)[iMCLepP] : (*ggHi.mcPt)[iMCLepM];
                            }
                            l1pt *= rc.kSpreadMC((*lChg)[i], l1pt, (*lEta)[i], (*lPhi)[i], l1ptgen);
                        }
                        else {
                            l1pt *= rc.kScaleDT((*lChg)[i], l1pt, (*lEta)[i], (*lPhi)[i]);
                        }
                    }
                    else if (vIsZee && isRecoV) {
                        if (isPP17) {
                            double lAbsEta = std::fabs((*lEta)[i]);
                            double eleSCEt = (*ggHi.eleSCEn)[i] / std::cosh(lAbsEta);
                            if (isMC) {
                                double smearRnd = randelePt.Gaus(0,1);
                                double smearElePt = ec.smearingSigma(306936, eleSCEt, lAbsEta, (*ggHi.eleR9)[i], 12, 0., 0.);
                                l1pt *= (1.0 + smearElePt * smearRnd);
                            }
                            else {
                                l1pt *= ec.scaleCorr(306936, eleSCEt, lAbsEta, (*ggHi.eleR9)[i]);
                            }
                        }
                        else if (isPbPb18) {
                            l1pt *= ggHi.getElePtCorrFactor(i, collisionType, hiBin0);
                            if (isMC) {
                                double smearWidth = ggHi.getElePtSmearFactor(i, collisionType, hiBin0);
                                l1pt *= randelePt.Gaus(1, smearWidth);
                            }
                        }

                        if (do_sys_les) {
                            l1pt *= getLepSysVar(vIsZmm, lep_sys_index, l1pt, (*lEta)[i], cent0);
                        }
                    }
                    l1pt *= lPtScale;
                    if (!(l1pt > lPtMin)) continue;
                    if (!(std::fabs((*lEta)[i]) < lEtaMax)) continue;

                    if (vIsZmm && isRecoV) {
                        if (!ggHi.passedMuSelection(i, collisionType)) continue;
                        //if (excludeHI18HEMfailure && !ggHi.passedHI18HEMfailureMu(i))  continue;
                    }
                    else if (vIsZee && isRecoV) {
                        double eleRho = (!isMC && isPbPb18 && false) ? ggHi.getHiFJRho4Ele(i, hifjrho) : -1;
                        eleRho = -1;
                        if (!ggHi.passedEleSelection(i, collisionType, hiBin0, eleRho)) continue;
                        if (excludeHI18HEMfailure && !ggHi.passedHI18HEMfailureEle(i))  continue;
                        //std::vector<int> indicesPho = indicesNearPhotons(ggHi, i, 0.8);
                        //int nNearPho = indicesPho.size();
                        //if (nNearPho > 0)  continue;
                        //if ( !((*ggHi.eleR9)[i] > 0.9) ) continue;
                    }
                    else if (!isRecoV) {
                        if (std::fabs((*ggHi.mcPID)[i]) != pdgL) continue;
                        if ( !((*ggHi.mcMomPID)[i] == pdgV) ) continue;
                        else if (vIsZee) {
                            if (std::fabs((*lEta)[i]) > 1.4442 && std::fabs((*lEta)[i]) < 1.566) continue;
                            if (excludeHI18HEMfailure && !ggHi.passedHI18HEMfailureGen(i))  continue;
                        }
                    }

                    for (int j = i+1; j < nL; ++j) {

                        float l2pt = (*lPt)[j];
                        if (vIsZmm && isRecoV && !do_sys_les) {

                            if (isMC) {
                                double l2ptgen = l2pt;
                                if (iMCLepP > 0 && iMCLepM > 0) {
                                    l2ptgen = ((*lChg)[j] < 0) ? (*ggHi.mcPt)[iMCLepP] : (*ggHi.mcPt)[iMCLepM];
                                }
                                l2pt *= rc.kSpreadMC((*lChg)[j], l2pt, (*lEta)[j], (*lPhi)[j], l2ptgen);
                            }
                            else {
                                l2pt *= rc.kScaleDT((*lChg)[j], l2pt, (*lEta)[j], (*lPhi)[j]);
                            }
                        }
                        else if (vIsZee && isRecoV) {
                            if (isPP17) {
                                double lAbsEta = std::fabs((*lEta)[j]);
                                double eleSCEt = (*ggHi.eleSCEn)[j] / std::cosh(lAbsEta);
                                if (isMC) {
                                    double smearRnd = randelePt.Gaus(0,1);
                                    double smearElePt = ec.smearingSigma(306936, eleSCEt, lAbsEta, (*ggHi.eleR9)[j], 12, 0., 0.);
                                    l2pt *= (1.0 + smearElePt * smearRnd);
                                }
                                else {
                                    l2pt *= ec.scaleCorr(306936, eleSCEt, lAbsEta, (*ggHi.eleR9)[j]);
                                }
                            }
                            else if (isPbPb18) {
                                l2pt *= ggHi.getElePtCorrFactor(j, collisionType, hiBin0);
                                if (isMC) {
                                    double smearWidth = ggHi.getElePtSmearFactor(j, collisionType, hiBin0);
                                    l2pt *= randelePt.Gaus(1, smearWidth);
                                }
                            }

                            if (do_sys_les) {
                                l2pt *= getLepSysVar(vIsZmm, lep_sys_index, l2pt, (*lEta)[j], cent0);
                            }
                        }
                        l2pt *= lPtScale;
                        if (!(l2pt > lPtMin)) continue;
                        if (!(std::fabs((*lEta)[j]) < lEtaMax)) continue;

                        if (vIsZmm && isRecoV) {
                            if (!ggHi.passedMuSelection(j, collisionType)) continue;
                            //if (excludeHI18HEMfailure && !ggHi.passedHI18HEMfailureMu(j))  continue;
                        }
                        else if (vIsZee && isRecoV) {
                            double eleRho = (!isMC && isPbPb18 && false) ? ggHi.getHiFJRho4Ele(j, hifjrho) : -1;
                            eleRho = -1;
                            if (!ggHi.passedEleSelection(j, collisionType, hiBin0, eleRho)) continue;
                            if (excludeHI18HEMfailure && !ggHi.passedHI18HEMfailureEle(j))  continue;
                            //std::vector<int> indicesPho = indicesNearPhotons(ggHi, j, 0.8);
                            //int nNearPho = indicesPho.size();
                            //if (nNearPho > 0)  continue;
                            //if ( !((*ggHi.eleR9)[j] > 0.9) ) continue;
                        }
                        else if (!isRecoV) {
                            if (std::fabs((*ggHi.mcPID)[j]) != pdgL) continue;
                            if ( !((*ggHi.mcMomPID)[j] == pdgV) ) continue;
                            if (vIsZee) {
                                if (std::fabs((*lEta)[j]) > 1.4442 && std::fabs((*lEta)[j]) < 1.566) continue;
                                if (excludeHI18HEMfailure && !ggHi.passedHI18HEMfailureGen(j))  continue;
                            }
                        }

                        vecl1.SetPtEtaPhiM(l1pt, (*lEta)[i], (*lPhi)[i], lMass);
                        vecl2.SetPtEtaPhiM(l2pt, (*lEta)[j], (*lPhi)[j], lMass);

                        vecll = vecl1 + vecl2;

                        if (!(vecll.M() >= massMin && vecll.M() <= massMax)) continue;

                        if (vecll.Pt() > 30 && vecll.Pt() < 80 &&
                                std::fabs(vecll.Rapidity()) < 2.4 &&
                                ((*lChg)[i] == -1*(*lChg)[j]) ) {
                            nVCand++;
                        }

                        if (std::fabs(vecll.M() - zmassPDG) < deltaMass) {
                            deltaMass = std::fabs(vecll.M() - zmassPDG);
                            vPt = vecll.Pt();
                            vEta = vecll.Eta();
                            vPhi = vecll.Phi();
                            vY = vecll.Rapidity();
                            vM = vecll.M();
                            ll_passSign =  (vIsSS) ? ((*lChg)[i] == (*lChg)[j]) : ((*lChg)[i] == -1*(*lChg)[j]);

                            if (l1pt >= l2pt) {
                                llIndex = {i, j};
                                llPt = {l1pt, l2pt};
                                llEta = {(*lEta)[i], (*lEta)[j]};
                                llPhi = {(*lPhi)[i], (*lPhi)[j]};
                            }
                            else {
                                llIndex = {j, i};
                                llPt = {l2pt, l1pt};
                                llEta = {(*lEta)[j], (*lEta)[i]};
                                llPhi = {(*lPhi)[j], (*lPhi)[i]};
                            }

                            if (doRndVPhi) {
                                vPhi = randvPhi.Uniform(-1*TMath::Pi(), TMath::Pi());
                            }
                        }
                    }
                }
            }

            bool passedTrig = (nTreesHLTObj == 0);
            if (vIsZ) {

                int nL1obj = 0;
                std::vector<float> *l1objEt;
                std::vector<float> *l1objEta;
                std::vector<float> *l1objPhi;
                if (vIsZmm) {
                    nL1obj = (int)l1Obj.nMuons;
                    l1objEt = l1Obj.muonEt;
                    l1objEta = l1Obj.muonEta;
                    l1objPhi = l1Obj.muonPhi;
                }
                else if (vIsZee) {
                    nL1obj = (int)l1Obj.nEGs;
                    l1objEt = l1Obj.egEt;
                    l1objEta = l1Obj.egEta;
                    l1objPhi = l1Obj.egPhi;
                }

                double max_dR2_L1 = 0.09;
                double max_dR2_HLT = 0.04;

                for (int iHltObj = 0; iHltObj < nTreesHLTObj; ++iHltObj) {

                    bool matchedL1Obj = false;
                    for (int iL1Obj = 0; iL1Obj < nL1obj; ++iL1Obj) {

                        if ( !((*l1objEt)[iL1Obj] >= l1Thresholds[iHltObj]) ) continue;

                        double etaL1 = (*l1objEta)[iL1Obj];
                        double phiL1 = (*l1objPhi)[iL1Obj];

                        if ( !(getDR2(etaL1, phiL1, llEta[0], llPhi[0]) < max_dR2_L1 ||
                               getDR2(etaL1, phiL1, llEta[1], llPhi[1]) < max_dR2_L1) ) continue;

                        matchedL1Obj = true;
                        break;
                    }

                    if (!matchedL1Obj) continue;

                    int nHltObjs = hltObjs[iHltObj].pt->size();
                    for (int iObj = 0; iObj < nHltObjs; ++iObj) {

                        if ( !((*hltObjs[iHltObj].pt)[iObj] > 0) ) continue;

                        double etaHLT = (*hltObjs[iHltObj].eta)[iObj];
                        double phiHLT = (*hltObjs[iHltObj].phi)[iObj];

                        if ( !(getDR2(etaHLT, phiHLT, llEta[0], llPhi[0]) < max_dR2_HLT ||
                               getDR2(etaHLT, phiHLT, llEta[1], llPhi[1]) < max_dR2_HLT) ) continue;

                        passedTrig = true;
                        break;
                    }
                }
            }

            double wV = w;
            if (vIsZ) {
                if (do_sf_weight_nom) {
                    wV *= (getSFweight(vIsZmm, llPt[0], llEta[0], cent0) * getSFweight(vIsZmm, llPt[1], llEta[1], cent0));
                }
                wV *= (getLepSysVar(vIsZmm, lep_sys_index, llPt[0], llEta[0], cent0) * getLepSysVar(vIsZmm, lep_sys_index, llPt[1], llEta[1], cent0));
            }

            double wV_recoeff = 1;
            if (doWeightsV) {
                if (isRecoV) {
                    int iCent = (isPP) ? 0 : getIndex4CentBin(hiBin0);
                    wV_recoeff = getVRecoEffCorrection(vPt, vY, vec_h2D_wV[iCent]);
                }
            }

            if (doWeightsEP) {
                int iCent = (isPP) ? 0 : getIndex4CentBin(hiBin0);
                int binMinTmp = 0;
                int binMaxTmp = -1;
                if (vPt >= 60) {
                    binMinTmp = 13;
                    binMaxTmp = 31;
                }
                else if (vPt >= 40) {
                    binMinTmp = 9;
                    binMaxTmp = 12;
                }
                else if (vPt >= 30) {
                    binMinTmp = 7;
                    binMaxTmp = 8;
                }
                else if (vPt >= 20) {
                    binMinTmp = 5;
                    binMaxTmp = 6;
                }
                else if (vPt >= 10) {
                    binMinTmp = 3;
                    binMaxTmp = 4;
                }

                //int binTmpVPt = ((TH1D*)(vec_h2D_wEP[iCent]->ProjectionX()))->FindBin(vPt);
                //hTmp = (TH1D*)vec_h2D_wEP[iCent]->ProjectionY("hTmp_wEP", binTmpVPt, binTmpVPt);
                //hTmp->Rebin(2);
                hTmp = (TH1D*)vec_h2D_wEP[iCent]->ProjectionY("hTmp_wEP", binMinTmp, binMaxTmp);
                hTmp->Rebin(2);
                hTmp->Scale(1./hTmp->Integral());
                int tmpBinEP = hTmp->FindBin(std::fabs(getDPHI(vPhi, hiEvt.hiEvtPlanes[8])));
                double tmpWEP = hTmp->GetBinContent(tmpBinEP);
                if (tmpWEP > 0.001) {
                    wV *= 1./(tmpWEP);
                }
                hTmp->Delete();
            }

            if (doWeightsPhi0) {
                int iCent = (isPP) ? 0 : getIndex4CentBin(hiBin);

                //int binTmpVPt = ((TH1D*)(vec_h2D_wPhi0[iCent]->ProjectionX()))->FindBin(vPt);
                //hTmp = (TH1D*)vec_h2D_wPhi0[iCent]->ProjectionY("hTmp_wPhi0", binTmpVPt, binTmpVPt);
                //hTmp->Rebin(2);
                hTmp = (TH1D*)vec_h1D_wPhi0[iCent]->Clone("hTmp_wPhi0");
                //hTmp->Rebin(2);
                hTmp->Scale(1./hTmp->Integral());
                int tmpBinPhi0 = hTmp->FindBin(std::fabs(getDPHI(vPhi, hiEvt.phi0)));
                double tmpWPhi0 = hTmp->GetBinContent(tmpBinPhi0);
                if (tmpWPhi0 > 0.00001) {
                    wV *= 1./(tmpWPhi0);
                }
                hTmp->Delete();
            }

            if (doWeightsVcent) {
                int binTmp = h1D_wVcent1->FindBin((hiBin0/2));
                wV *= (h1D_wVcent1->GetBinContent(binTmp));
            }

            if (doNVtxW) {
                int binTmp = h1D_wV_nVtx1->FindBin(trks.nVtx);
                wV *= (h1D_wV_nVtx1->GetBinContent(binTmp));
            }

            if ((useGenVPt || useGenVPhi) && !(genVPt > 0)) continue;

            // reco eff
            double wV_evt = wV;
            wV *= wV_recoeff;
            if (isMC && isRecoV && genVPt > 0) {

                bool matchedRG = ((vIsPho || ll_passSign) && passedTrig && vPt >= 0 && getDR2(genVY, genVPhi, vY, vPhi) < maxDR2_reco_gen_V);

                if (useGenVPt || useGenVPhi) {
                    if (matchedRG) {
                        if (useGenVPt)  vPt = genVPt;
                        if (useGenVPhi)  vPhi = genVPhi;
                    }
                    else {
                        continue;
                    }
                }

                double genVYAbs = std::fabs(genVY);

                double wV_gen = wV_evt;

                for (int i = 0; i < nCents; ++i) {

                    if (isPbPb && !(centsMin[i] <= cent && cent < centsMax[i]))  continue;

                    h2_reco_denom_vPt_vs_vY[i]->Fill(genVY, genVPt, wV_gen);
                    if (matchedRG) {
                        h2_reco_num_vPt_vs_vY[i]->Fill(genVY, genVPt, wV);
                    }

                    if (vYMin <= genVYAbs && genVYAbs < vYMax) {

                        h_reco_denom_vPt[i]->Fill(genVPt, wV_gen);
                        if (matchedRG) {
                            h_reco_num_vPt[i]->Fill(genVPt, wV);

                            h2_rgVPt_ratio_vs_vPt[i]->Fill(genVPt, vPt / genVPt, wV);
                            h2_rgVPt_ratio_vs_vPtreco[i]->Fill(vPt, vPt / genVPt, wV);
                            h2_rgVPhi_diff_vs_vPt[i]->Fill(genVPt, getDPHI(vPhi, genVPhi), wV);
                        }
                    }

                    for (int j = 0; j < nVPts; ++j) {

                        if (!(vPtsMin[j] <= genVPt && genVPt < vPtsMax[j]))  continue;

                        h_reco_denom_vY[i][j]->Fill(genVY, wV_gen);
                        if (matchedRG) {
                            h_reco_num_vY[i][j]->Fill(genVY, wV);

                            h2_rgVPt_ratio_vs_vY[i][j]->Fill(genVY, vPt / genVPt, wV);
                        }

                        if (vYMin <= genVYAbs && genVYAbs < vYMax) {
                            h_reco_denom_vPhi[i][j]->Fill(genVPhi, wV_gen);
                            if (matchedRG) {
                                h_reco_num_vPhi[i][j]->Fill(genVPhi, wV);

                                h2_rgVPt_ratio_vs_vM[i][j]->Fill(genVMass, vPt / genVPt, wV);
                                h2_rgVPt_ratio_vs_vMreco[i][j]->Fill(vM, vPt / genVPt, wV);
                            }

                            if (isPbPb) {
                                h_reco_denom_dphi_phi0_V[i][j]->Fill(std::fabs(getDPHI(genVPhi, hiEvt.phi0)), wV_gen);
                                h_reco_denom_dphi_EPn2_V[i][j]->Fill(std::fabs(getDPHI(genVPhi, hiEvt.hiEvtPlanes[8])), wV_gen);
                                if (matchedRG) {
                                    h_reco_num_dphi_phi0_V[i][j]->Fill(std::fabs(getDPHI(genVPhi, hiEvt.phi0)), wV);
                                    h_reco_num_dphi_EPn2_V[i][j]->Fill(std::fabs(getDPHI(genVPhi, hiEvt.hiEvtPlanes[8])), wV);
                                }
                            }
                        }
                    }
                }

                for (int j = 0; j < nVPts; ++j) {

                    if (!(vPtsMin[j] <= genVPt && genVPt < vPtsMax[j]))  continue;

                    if (vYMin <= genVYAbs && genVYAbs < vYMax) {

                        h_reco_denom_cent[j]->Fill(cent, wV_gen);
                        if (matchedRG) {
                            h_reco_num_cent[j]->Fill(cent, wV);

                            h2_rgVPt_ratio_vs_cent[j]->Fill(cent, vPt / genVPt, wV);
                        }
                    }

                }
            }

            //if (std::fabs(getDPHI(vPhi, hiEvt.hiEvtPlanes[8])) > 1.2 && std::fabs(getDPHI(vPhi, hiEvt.hiEvtPlanes[8])) < 2.2) continue;

            if (vPt < 0) continue;

            if (nVCand > 1 && false) {
                std::cout << "There is nVCand = " << nVCand << std::endl;
            }

            double vYAbs = std::fabs(vY);

            // fill trigger eff histograms
            if (vIsPho || ll_passSign) {
                for (int i = 0; i < nCents; ++i) {

                    if (isPbPb && !(centsMin[i] <= cent && cent < centsMax[i]))  continue;

                    if (vYMin <= vYAbs && vYAbs < vYMax) {
                        h_trig_denom_vPt[i]->Fill(vPt, wV);
                        if (passedTrig) {
                            h_trig_num_vPt[i]->Fill(vPt, wV);
                        }
                        else {
                            if (vPt > 10 && false) {
                                std::cout << "trigger inefficiency for entry : " << j_entry << std::endl;
                                std::cout << "run , event , lumi : " << hiEvt.run << " , " << hiEvt.evt << " , " << hiEvt.lumi << std::endl;
                                std::cout << "hiBin, vPt , vY, vPhi = " << hiBin0 << " , "
                                                                        << vPt << " , "
                                                                        << vY << " , "
                                                                        << vPhi << std::endl;
                            }
                        }
                    }

                    for (int j = 0; j < nVPts; ++j) {

                        if (!(vPtsMin[j] <= vPt && vPt < vPtsMax[j]))  continue;

                        h_trig_denom_vY[i][j]->Fill(vY, wV);
                        if (passedTrig) {
                            h_trig_num_vY[i][j]->Fill(vY, wV);
                        }
                    }
                }
            }

            if (!passedTrig) continue;

            if (vIsPho || ll_passSign) {
                // opposite charge pairs
                for (int i = 0; i < nCents; ++i) {

                    if (isPbPb && !(centsMin[i] <= cent && cent < centsMax[i]))  continue;

                    if (vYMin <= vYAbs && vYAbs < vYMax) {
                        h_vPt[i]->Fill(vPt, wV);
                        h2_hiHF_vs_vPt[i]->Fill(vPt, hiEvt.hiHF, wV);
                        h2_rho_vs_vPt[i]->Fill(vPt, ggHi.rho, wV);
                        if (isvJetTrkSkim) {
                            h2_PFHFtotE_vs_vPt[i]->Fill(vPt, (evtskim.pf_h_HF_totE + evtskim.pf_eg_HF_totE), wV);
                            h2_PFHFtotE_vs_vPt_rebin[i]->Fill(vPt, (evtskim.pf_h_HF_totE + evtskim.pf_eg_HF_totE), wV);
                            h2_PFHFtotE_eta3to4_vs_vPt[i]->Fill(vPt, (evtskim.pf_h_HF_E_eta3to4 + evtskim.pf_eg_HF_E_eta3to4), wV);
                            h2_PFHFtotE_eta3to4_vs_vPt_rebin[i]->Fill(vPt, (evtskim.pf_h_HF_E_eta3to4 + evtskim.pf_eg_HF_E_eta3to4), wV);
                            h2_PFHFtotE_eta4to5_vs_vPt[i]->Fill(vPt, (evtskim.pf_h_HF_E_eta4to5 + evtskim.pf_eg_HF_E_eta4to5), wV);
                            h2_PFHFtotE_eta4to5_vs_vPt_rebin[i]->Fill(vPt, (evtskim.pf_h_HF_E_eta4to5 + evtskim.pf_eg_HF_E_eta4to5), wV);
                            h2_PFHEtotE_vs_vPt[i]->Fill(vPt, (evtskim.pf_h_HE_totE + evtskim.pf_e_HE_totE
                                                                                   + evtskim.pf_mu_HE_totE
                                                                                   + evtskim.pf_gamma_HE_totE
                                                                                   + evtskim.pf_h0_HE_totE), wV);
                        }
                        h2_dphi_EPn2_V_vs_vPt[i]->Fill(vPt, std::fabs(getDPHI(vPhi, hiEvt.hiEvtPlanes[8])), wV);
                    }

                    for (int j = 0; j < nVPts; ++j) {

                        if (!(vPtsMin[j] <= vPt && vPt < vPtsMax[j]))  continue;

                        h_vEta[i][j]->Fill(vEta, wV);
                        h_vY[i][j]->Fill(vY, wV);

                        if (vYMin <= vYAbs && vYAbs < vYMax) {

                            h_vPhi[i][j]->Fill(vPhi, wV);
                            h_vM_os[i][j]->Fill(vM, wV);

                            // event observables
                            h_vtxz[i][j]->Fill(hiEvt.vz, wV);
                            if (anaNVtx) {
                                h_nVtx[i][j]->Fill(trks.nVtx, wV);
                            }
                            if (isMC && isPbPb && isvJetTrkSkim) {
                                double tmpPFHFtotE = (evtskim.pf_h_HF_totE + evtskim.pf_eg_HF_totE);
                                h2_PFHFtotE_vs_Npart[i][j]->Fill(hiEvt.Npart, tmpPFHFtotE, wV);

                                if (mixEvents.nmix > 0) {
                                    double tmpPFHFtotEmix = 0;
                                    for (int iME = 0; iME < mixEvents.nmix; ++iME) {
                                        tmpPFHFtotEmix += ( (*mixEvents.p_pf_h_HF_totE_mix)[iME] + (*mixEvents.p_pf_eg_HF_totE_mix)[iME] );
                                    }
                                    tmpPFHFtotEmix /= (double(mixEvents.nmix));
                                    h2_PFHFtotEmix_vs_Npart[i][j]->Fill(hiEvt.Npart, tmpPFHFtotEmix, wV);

                                    h2_PFHFtotE_diff_rawmix_vs_Npart[i][j]->Fill(hiEvt.Npart, tmpPFHFtotE - tmpPFHFtotEmix, wV);
                                }
                            }

                            h2_PFHFtotE_vs_vY[i][j]->Fill(vY, (evtskim.pf_h_HF_totE + evtskim.pf_eg_HF_totE), wV);
                            if (isPP) {
                                h2_PFHFtotE_vs_nVtx[i][j]->Fill(trks.nVtx, (evtskim.pf_h_HF_totE + evtskim.pf_eg_HF_totE), wV);
                            }

                            if (isPP13tev) {
                                h2_nVtx_vs_run_13tev_pp17[i][j]->Fill(hiEvt.run, trks.nVtx, wV);
                                h2_nVtx_vs_run_13tev_pp18[i][j]->Fill(hiEvt.run, trks.nVtx, wV);
                            }

                            h_dphi_phi0_V[i][j]->Fill(std::fabs(getDPHI(vPhi, hiEvt.phi0)), wV);

                            h_dphi_EPn1_V[i][j]->Fill(std::fabs(getDPHI(vPhi, hiEvt.hiEvtPlanes[2])), wV);

                            h_dphi_EPn2_V[i][j]->Fill(std::fabs(getDPHI(vPhi, hiEvt.hiEvtPlanes[8])), wV);
                            h_dphi_EPn3_V[i][j]->Fill(std::fabs(getDPHI(vPhi, hiEvt.hiEvtPlanes[15])), wV);

                            h_dphi_EPn2_phi0[i][j]->Fill(std::fabs(getDPHI(hiEvt.phi0, hiEvt.hiEvtPlanes[8])), wV);

                            h_dphi_EPn2_l1[i][j]->Fill(std::fabs(getDPHI(llPhi[0], hiEvt.hiEvtPlanes[8])), wV);
                            h_dphi_EPn2_l2[i][j]->Fill(std::fabs(getDPHI(llPhi[1], hiEvt.hiEvtPlanes[8])), wV);

                            h_l1Pt[i][j]->Fill(llPt[0], wV);
                            h_l1Eta[i][j]->Fill(llEta[0], wV);
                            h_l1Phi[i][j]->Fill(llPhi[0], wV);
                            if (vIsZee && isRecoV) {
                                h_l1R9[i][j]->Fill((*ggHi.eleR9)[llIndex[0]], wV);
                            }

                            h_l2Pt[i][j]->Fill(llPt[1], wV);
                            h_l2Eta[i][j]->Fill(llEta[1], wV);
                            h_l2Phi[i][j]->Fill(llPhi[1], wV);
                            if (vIsZee && isRecoV) {
                                h_l2R9[i][j]->Fill((*ggHi.eleR9)[llIndex[1]], wV);
                            }

                            h_detall[i][j]->Fill(std::fabs(llEta[0] - llEta[1]), wV);
                            h_dphill[i][j]->Fill(std::fabs(getDPHI(llPhi[0], llPhi[1])), wV);

                            h2_l1Phi_vs_l1Eta[i][j]->Fill(llEta[0], llPhi[0], wV);
                            h2_l2Phi_vs_l2Eta[i][j]->Fill(llEta[1], llPhi[1], wV);
                        }

                        h2_vPhi_vs_vY[i][j]->Fill(vY, vPhi, wV);
                    }
                }

                for (int j = 0; j < nVPts; ++j) {

                    if (!(vPtsMin[j] <= vPt && vPt < vPtsMax[j]))  continue;

                    if (vYMin <= vYAbs && vYAbs < vYMax) {

                        h_cent[j]->Fill(cent, wV);
                        h2_hiHF_vs_hiNpix[j]->Fill(hiEvt.hiNpix, hiEvt.hiHF, wV);
                        h2_PFHFtotE_vs_hiNpix[j]->Fill(hiEvt.hiNpix, (evtskim.pf_h_HF_totE + evtskim.pf_eg_HF_totE), wV);
                    }

                }
            }
            else {
                // same charge pairs
                for (int i = 0; i < nCents; ++i) {

                    if (isPbPb && !(centsMin[i] <= cent && cent < centsMax[i]))  continue;

                    if (vYMin <= vYAbs && vYAbs < vYMax) {
                        h_vPt_ss[i]->Fill(vPt, wV);
                    }

                    for (int j = 0; j < nVPts; ++j) {

                        if (!(vPtsMin[j] <= vPt && vPt < vPtsMax[j]))  continue;

                        if (vYMin <= vYAbs && vYAbs < vYMax) {

                            h_vM_ss[i][j]->Fill(vM, wV);
                            h_vY_ss[i][j]->Fill(vY, wV);
                        }
                    }
                }
            }

            if (!(vYMin <= vYAbs && vYAbs < vYMax))  continue;
            if (vIsZ && !ll_passSign) continue;
            wEvtsV += wV;

            if (!anaTrks) continue;
            if (!isvJetTrkSkim) continue;

            TLorentzVector vV;
            vV.SetPtEtaPhiM(vPt, 0, vPhi, vM);

            TLorentzVector vVgen;
            vVgen.SetPtEtaPhiM(genVPt, 0, genVPhi, genVMass);

            double h1_pt = -1;
            double h1_eta = -555;
            if (findLeadingTrk) {
                nPartRaw = (isRecoTrk) ? trks.nTrk : trks.mult;

                for (int i = 0; i < nPartRaw; ++i) {

                    if (!isRecoTrk) {
                        if ((*p_raw_chg)[i] == 0)  continue;
                        //if (isSigTrk && (*p_raw_sube)[i] != 0)  continue;
                    }

                    if (!(trkEtaMin <= std::fabs((*p_raw_eta)[i]) && std::fabs((*p_raw_eta)[i]) < trkEtaMax))  continue;
                    if (!(vTrkDetaMin <= std::fabs((*p_raw_eta)[i] - vY) && std::fabs((*p_raw_eta)[i] - vY) < vTrkDetaMax))  continue;

                    float t_raw_pt = (*p_raw_pt)[i];
                    float t_raw_eta = (*p_raw_eta)[i];
                    float t_raw_phi = (*p_raw_phi)[i];

                    if (vIsZ) {
                        if (getDR2(t_raw_eta, t_raw_phi, llEta[0], llPhi[0]) < minDR2_lep_trk)  continue;
                        if (getDR2(t_raw_eta, t_raw_phi, llEta[1], llPhi[1]) < minDR2_lep_trk)  continue;
                    }

                    // lepton rej
                    if (isRecoTrk) {
                        // PF id
                        if ((*p_raw_pid)[i] == 2 || (*p_raw_pid)[i] == 3 || (*p_raw_pid)[i] < 0)  continue;
                    }
                    else {
                        if (std::fabs((*p_raw_pid)[i]) == 11 || std::fabs((*p_raw_pid)[i]) == 13)  continue;
                    }

                    float dphiTmp = std::fabs(getDPHI(vPhi, t_raw_phi));
                    double dphiMinTmp = vTrkDphiMin * TMath::Pi();
                    double dphiMaxTmp = vTrkDphiMax * TMath::Pi();

                    if ( !(dphiMinTmp < dphiTmp && dphiTmp <= dphiMaxTmp) )  continue;

                    if (t_raw_pt > h1_pt) {

                        h1_pt = t_raw_pt;
                        h1_eta = t_raw_eta;
                    }
                }
            }

            if (isRecoTrk && !isMixTrk) {
                nParticles = trks.nTrk;
            }
            else if (isRecoTrk && isMixTrk) {
                nParticles = trks.nTrk_mix;
            }
            else if (!isRecoTrk && !isMixTrk) {
                nParticles = trks.mult;
            }
            else {
                nParticles = trks.mult_mix;
            }

            if (anaJets) {
                nJets = jets.njet;
                if (canUseMixJets && isMixTrk) {
                    nJets = jets.njet_mix;
                }
            }

            double wMixEvts = wV;
            if (isMixTrk) {
                wMixEvts *= (1.0 / (double(mixEvents.nmix)));
            }

            int iTrkPhiWCent = -1;
            if (doTrkPhiWeights) {
                iTrkPhiWCent = getBinCent4TrkW(hiBin0, trkPhiWCents, nTrkPhiWCent);
            }

            int iResMBTrkWCent = -1;
            if (doResidualMBTrkW) {
                iResMBTrkWCent = getBinCent4TrkW(hiBin0, resMBTrkWCents, nResMBTrkWCent);
            }

            int effDR_iC = -1;
            if (doEffDRW) {
                for (int iC = 0; iC < nEffDRWCent; ++iC) {
                    if (cent <= effDRWCents[iC+1]) {
                        effDR_iC = iC;
                        break;
                    }
                }
            }

            if (fillBootStrap) {

                for (int i = 0; i < nCents; ++i) {

                    if (isPbPb && !(centsMin[i] <= cent && cent < centsMax[i]))  continue;

                    for (int j = 0; j < nVPts; ++j) {

                        if (!(vPtsMin[j] <= vPt && vPt < vPtsMax[j]))  continue;

                        for (int k = 0; k < nTrkPts; ++k) {

                            evtIndex_bs_dphi_rebin[i][j][k] += 1;
                            evtIndex_bs_dphi_rebin[i][j][k] = (evtIndex_bs_dphi_rebin[i][j][k] % nEvtsBS);

                            evtIndex_bs_xivh[i][j][k] += 1;
                            evtIndex_bs_xivh[i][j][k] = (evtIndex_bs_xivh[i][j][k] % nEvtsBS);
                        }

                        evtIndex_bs_trkPt_rebin[i][j] += 1;
                        evtIndex_bs_trkPt_rebin[i][j] = (evtIndex_bs_trkPt_rebin[i][j] % nEvtsBS);

                        evtIndex_bs_trkPt_rebin2[i][j] += 1;
                        evtIndex_bs_trkPt_rebin2[i][j] = (evtIndex_bs_trkPt_rebin2[i][j] % nEvtsBS);
                    }
                }
            }

            double wTrkSums[nVPts][nTrkPts];
            double wTrkSums_noDphi[nVPts][nTrkPts];
            for (int j = 0; j < nVPts; ++j) {
                for (int k = 0; k < nTrkPts; ++k) {
                    wTrkSums[j][k] = 0;
                    wTrkSums_noDphi[j][k] = 0;
                }
            }

            for (int i = 0; i < nParticles; ++i) {

                //if (!passedTrkSelection(trks, i, collisionType))  continue;
                if (!isRecoTrk) {
                    if ((*p_chg)[i] == 0)  continue;
                    if (isSigTrk && (*p_sube)[i] != 0)  continue;
                }

                if (!(trkEtaMin <= std::fabs((*p_eta)[i]) && std::fabs((*p_eta)[i]) < trkEtaMax))  continue;
                if (!(vTrkDetaMin <= std::fabs((*p_eta)[i] - vY) && std::fabs((*p_eta)[i] - vY) < vTrkDetaMax))  continue;

                if (findLeadingTrk) {
                    if (!(hTrkDetaMin <= std::fabs((*p_eta)[i] - h1_eta) && std::fabs((*p_eta)[i] - h1_eta) < hTrkDetaMax))  continue;
                }

                float t_pt = (*p_pt)[i];
                float t_eta = (*p_eta)[i];
                float t_phi = (*p_phi)[i];

                double wTrkPhi = 1;
                if (doTrkPhiWeights) {

                    int iTrkPhiWPt = getBinPt4TrkW(t_pt, trkPhiWPts, nTrkPhiWPt);
                    int iTrkPhiWEta = getBinEta4TrkW(t_eta, trkPhiWEtas, nTrkPhiWEta);

                    if (isMixTrk) {
                        int iEvt = (*p_evtIndex)[i];
                        iTrkPhiWCent = getBinCent4TrkW((*mixEvents.p_hiBin_mix)[iEvt], trkPhiWCents, nTrkPhiWCent);
                    }

                    if (iTrkPhiWPt >= 0 && iTrkPhiWEta >= 0 && iTrkPhiWCent >= 0) {
                        wTrkPhi = getTrkPhiEffCorrection(t_phi, h_trkPhi_effcorr[iTrkPhiWPt][iTrkPhiWEta][iTrkPhiWCent]);
                    }
                }

                float t_phi0 = t_phi;
                if (isMixTrk && rotateEvtPlane) {
                    int iEvt = (*p_evtIndex)[i];
                    t_phi += (hiEvt.hiEvtPlanes[8] - (*mixEvents.p_hiEvtPlanes_mix)[iEvt]);
                    t_phi = correctPhiRange(t_phi);
                }

                if (vIsZ) {
                    if (getDR2(t_eta, t_phi, llEta[0], llPhi[0]) < minDR2_lep_trk)  continue;
                    if (getDR2(t_eta, t_phi, llEta[1], llPhi[1]) < minDR2_lep_trk)  continue;
                }

                // lepton rej
                if (isRecoTrk) {
                    // PF id
                    if ((*p_pid)[i] == 2 || (*p_pid)[i] == 3 || (*p_pid)[i] < 0)  continue;
                }
                else {
                    if (std::fabs((*p_pid)[i]) == 11 || std::fabs((*p_pid)[i]) == 13)  continue;
                }

                double trkWeightTmp = (*p_weight)[i];
                if (isRecoTrk && noTrkWeights) {
                    trkWeightTmp = 1;
                }
                else if (isRecoTrk && redoTrkWeights) {

                    int hiBinTmp = hiBin;
                    int hiBin0Tmp = hiBin0;
                    if (isMixTrk) {
                        int iEvt = (*p_evtIndex)[i];
                        hiBinTmp = (*mixEvents.p_hiBin_mix)[iEvt];
                        hiBin0Tmp = hiBinTmp;
                        if (shiftHibin) {
                            hiBinTmp = (hiBinTmp < 3) ? 0 : hiBinTmp-3;
                        }
                    }

                    if (isPP) {
                        hiBinTmp = 0;
                    }

                    float tmpResCorr = 1;
                    if (isPP15 || isPbPb15) {
                        trkWeightTmp = trkCorr2015->getTrkCorr(t_pt, t_eta, t_phi, hiBin0Tmp);
                    }
                    else if (isPP17) {
                        trkWeightTmp = 0.979*trkEff2017.getCorrection(t_pt, t_eta);
                    }
                    else if (isPbPb18) {
                        //trkWeightTmp = trkEff2018.getCorrection(t_pt, t_eta, hiBinTmp);
                        float effTmp = trkEff2018.getEfficiency(t_pt, t_eta, hiBin0Tmp, true);
                        trkWeightTmp = (effTmp > 0.001) ? (1.0)/effTmp : 0;

                        if (doResidualMBTrkW) {

                            int iResMBTrkWPt = -1;
                            if ((t_pt < 4) || (hiBin0Tmp < 100 && t_pt < 60)) {
                                iResMBTrkWPt = getBinPt4TrkW(t_pt, resMBTrkWPts, nResMBTrkWPt);
                            }

                            if (isMixTrk) {
                                iResMBTrkWCent = getBinCent4TrkW(hiBin0Tmp, resMBTrkWCents, nResMBTrkWCent);
                            }

                            if (iResMBTrkWPt >= 0 && iResMBTrkWCent >= 0) {
                                tmpResCorr = 1.0/(getTrkEtaPhiEffCorrection(t_eta, t_phi0, h_resMBTrk_effcorr[iResMBTrkWPt][iResMBTrkWCent]));
                            }
                        }

                        trkWeightTmp /= tmpResCorr;
                    }
                    if (doEffDRW) {

                        int effDR_iP = -1;
                        for (int iP = 0; iP < nEffDRWPt; ++iP) {
                            if (effDRWPts[iP] < t_pt && t_pt <= effDRWPts[iP+1]) {
                                effDR_iP = iP;
                                break;
                            }
                        }

                        int effDR_iEta = 0;
                        if (nEffDRWeta == 2) {
                            effDR_iEta = (std::fabs(t_eta) < 1.0) ? 0 : 1;
                            if (std::fabs(t_eta) >= 2.4) {
                                effDR_iEta = -1;
                            }
                        }

                        double mindR2_jet_trk = mindR2_jet_trk_0;
                        int iJet_mindR = -1;

                        int iEvt = (*p_evtIndex)[i];

                        int nJetsTmp = (effDR_iP >= 0 && effDR_iC >= 0 && effDR_iEta >= 0) ? nJets : 0;
                        for (int iJet = 0; iJet < nJetsTmp; ++iJet) {

                            if (canUseMixJets && isMixTrk && iEvt != (*p_evtjet_index)[iJet]) continue;
                            if ( !((*p_jetpt)[iJet] > effDR_min_jetpt) ) continue;

                            double dR2_jet_trk = getDR2(t_eta, t_phi, (*p_jeteta)[iJet], (*p_jetphi)[iJet]);

                            if (dR2_jet_trk < mindR2_jet_trk) {
                                mindR2_jet_trk = dR2_jet_trk;
                                iJet_mindR = iJet;
                            }
                        }

                        if (iJet_mindR >= 0) {
                            if (getDR2(llEta[0], llPhi[0], (*p_jeteta)[iJet_mindR], (*p_jetphi)[iJet_mindR]) < maxdR2_jet_l ||
                                getDR2(llEta[1], llPhi[1], (*p_jeteta)[iJet_mindR], (*p_jetphi)[iJet_mindR]) < maxdR2_jet_l) {
                                iJet_mindR = -1;
                            }
                        }

                        double tmpCorrDR = 1;
                        if (iJet_mindR >= 0) {
                            //int binTmpPt = h_effDR[2][effDR_iP][effDR_iC][effDR_iEta]->FindBin((*p_jetpt)[iJet_mindR]);
                            //int binTmpPt = h_effDR[2][effDR_iP][effDR_iC][effDR_iEta]->FindBin((*p_rawpt)[iJet_mindR]);
                            //tmpCorrDR = h_effDR[2][effDR_iP][effDR_iC][effDR_iEta]->GetBinContent(binTmpPt);

                            //double mindR_jet_trk = std::sqrt(mindR2_jet_trk);
                            //int binTmpDR = h2_effDR[2][effDR_iP][effDR_iC][effDR_iEta]->GetXaxis()->FindBin(mindR_jet_trk);
                            //int binTmpPt = h2_effDR[2][effDR_iP][effDR_iC][effDR_iEta]->GetYaxis()->FindBin((*p_jetpt)[iJet_mindR]);
                            //tmpCorrDR = h2_effDR[2][effDR_iP][effDR_iC][effDR_iEta]->GetBinContent(binTmpDR, binTmpPt);

                            int binTmpPt = h_effDR[2][effDR_iP][effDR_iC][effDR_iEta]->FindBin((*p_jetpt)[iJet_mindR]);
                            tmpCorrDR = h_effDR[2][effDR_iP][effDR_iC][effDR_iEta]->GetBinContent(binTmpPt);

                            if (effDR_integrateDR) {
                                binTmpPt = h_effDR[2][effDR_iP][effDR_iC][effDR_iEta]->FindBin((*p_jetpt)[iJet_mindR]);
                                tmpCorrDR = h_effDR[2][effDR_iP][effDR_iC][effDR_iEta]->GetBinContent(binTmpPt);
                            }

                            if (tmpCorrDR < 0.01) tmpCorrDR = 0.01;
                            //else if (tmpCorrDR > 2.5) tmpCorrDR = 2.5;
                        }

                        trkWeightTmp *= tmpCorrDR;
                    }
                }
                double wTrk = trkWeightTmp * wMixEvts * wTrkPhi;

                double dphiMinTmp = vTrkDphiMin * TMath::Pi();
                double dphiMaxTmp = vTrkDphiMax * TMath::Pi();

                for (int iCent = 0; iCent < nCents; ++iCent) {

                    if (isPbPb && !(centsMin[iCent] <= cent && cent < centsMax[iCent]))  continue;

                    for (int iVPt = 0; iVPt < nVPts; ++iVPt) {

                        if (!(vPtsMin[iVPt] <= vPt && vPt < vPtsMax[iVPt]))  continue;

                        float dphi = std::fabs(getDPHI(vPhi, t_phi));

                        float deta = std::fabs(vY - t_eta);
                        float deta_h1 = std::fabs(h1_eta - t_eta);

                        if (dphiMinTmp < dphi && dphi <= dphiMaxTmp) {
                            h_trkPt[iCent][iVPt]->Fill(t_pt, wTrk);
                            h2_deta_vs_trkPt[iCent][iVPt]->Fill(t_pt, deta, wTrk);
                            if (findLeadingTrk) {
                                h2_deta_h1_vs_trkPt[iCent][iVPt]->Fill(t_pt, deta_h1, wTrk);
                            }

                            if (fillBootStrap) {
                                h2_bs_trkPt_rebin[iCent][iVPt]->Fill(t_pt, evtIndex_bs_trkPt_rebin[iCent][iVPt], wTrk);
                                h2_bs_trkPt_rebin2[iCent][iVPt]->Fill(t_pt, evtIndex_bs_trkPt_rebin2[iCent][iVPt], wTrk);
                            }
                        }
                        h2_dphi_vs_trkPt[iCent][iVPt]->Fill(t_pt, dphi, wTrk);

                        h2_trkPt_vs_trkEta[iCent][iVPt]->Fill(t_eta, t_pt, wTrk);

                        TLorentzVector vTrk;
                        vTrk.SetPtEtaPhiM(t_pt, 0, t_phi, 0);
                        float angle = vV.Angle(vTrk.Vect());
                        float z_vt_T = vTrk.P() * fabs(cos(angle)) / vPt;
                        float xi_vt = log(1.0 / z_vt_T);
                        float xit_vt = log(vPt / t_pt);

                        h2_trkPt_vs_xivh[iCent][iVPt]->Fill(xi_vt, t_pt, wTrk);
                        h2_trkPt_rebin_vs_xivh[iCent][iVPt]->Fill(xi_vt, t_pt, wTrk);

                        for (int iTrkPt = 0; iTrkPt < nTrkPts; ++iTrkPt) {

                            if (!(trkPtsMin[iTrkPt] <= t_pt && t_pt < trkPtsMax[iTrkPt]))  continue;

                            h_dphi[iCent][iVPt][iTrkPt]->Fill(dphi, wTrk);
                            if (anavTrk_dR) {
                                float dR = std::sqrt( dphi*dphi + deta*deta );
                                h_dR[iCent][iVPt][iTrkPt]->Fill(dR, wTrk);
                            }

                            h_dphi_leptrk[iCent][iVPt][iTrkPt]->Fill(std::fabs(getDPHI(llPhi[0], t_phi)), wTrk);

                            h_dR_leptrk[iCent][iVPt][iTrkPt]->Fill(std::fabs(getDR(llEta[0], llPhi[0], t_eta, t_phi)), wTrk);

                            h2_deta_vs_dphi[iCent][iVPt][iTrkPt]->Fill(dphi, deta, wTrk);
                            h2_deta_vs_dphi_rebin1[iCent][iVPt][iTrkPt]->Fill(dphi, deta, wTrk);
                            h2_deta_vs_dphi_rebin2[iCent][iVPt][iTrkPt]->Fill(dphi, deta, wTrk);
                            if (findLeadingTrk) {
                                h2_deta_h1_vs_dphi[iCent][iVPt][iTrkPt]->Fill(dphi, deta_h1, wTrk);
                            }
                            if (iVPt == 0) {
                                h2_dphi_vs_vPt[iCent][iTrkPt]->Fill(vPt, dphi, wTrk);
                            }
                            h2_dphi_vs_trkEta[iCent][iVPt][iTrkPt]->Fill(t_eta, dphi, wTrk);
                            h2_dphi_vs_trkPhi[iCent][iVPt][iTrkPt]->Fill(t_phi, dphi, wTrk);
                            h2_dphi_vs_detall[iCent][iVPt][iTrkPt]->Fill(std::fabs((llEta[0] - llEta[1])), dphi, wTrk);
                            h2_dphi_vs_dphill[iCent][iVPt][iTrkPt]->Fill(std::fabs(getDPHI(llPhi[0], llPhi[1])), dphi, wTrk);
                            h2_dphi_vs_vY[iCent][iVPt][iTrkPt]->Fill(vY, dphi, wTrk);
                            h2_dphi_vs_vPhi[iCent][iVPt][iTrkPt]->Fill(vPhi, dphi, wTrk);
                            if (anaTrkLep) {
                                h2_dphi_vs_detal1[iCent][iVPt][iTrkPt]->Fill(std::fabs((llEta[0] - t_eta)), dphi, wTrk);
                                h2_dphi_vs_detal2[iCent][iVPt][iTrkPt]->Fill(std::fabs((llEta[1] - t_eta)), dphi, wTrk);
                                h2_dphi_vs_l1Pt[iCent][iVPt][iTrkPt]->Fill(llPt[0], dphi, wTrk);
                                h2_dphi_vs_l2Pt[iCent][iVPt][iTrkPt]->Fill(llPt[1], dphi, wTrk);
                                h2_dphi_vs_l1Eta[iCent][iVPt][iTrkPt]->Fill(llEta[0], dphi, wTrk);
                                h2_dphi_vs_l2Eta[iCent][iVPt][iTrkPt]->Fill(llEta[1], dphi, wTrk);
                                h2_dphi_vs_l1Phi[iCent][iVPt][iTrkPt]->Fill(llPhi[0], dphi, wTrk);
                                h2_dphi_vs_l2Phi[iCent][iVPt][iTrkPt]->Fill(llPhi[1], dphi, wTrk);
                            }

                            h_dphi_EPn2_trk_noDphi[iCent][iVPt][iTrkPt]->Fill(std::fabs(getDPHI(t_phi, hiEvt.hiEvtPlanes[8])), wTrk);
                            h_dphi_phi0_trk_noDphi[iCent][iVPt][iTrkPt]->Fill(std::fabs(getDPHI(t_phi, hiEvt.phi0)), wTrk);
                            h2_trkPhi_vs_trkEta_noDphi[iCent][iVPt][iTrkPt]->Fill(t_eta, t_phi, wTrk);

                            if (fillBootStrap) {
                                h2_bs_dphi_rebin[iCent][iVPt][iTrkPt]->Fill(dphi, evtIndex_bs_dphi_rebin[iCent][iVPt][iTrkPt], wTrk);
                            }

                            if (!(dphiMinTmp < dphi && dphi <= dphiMaxTmp)) continue;

                            h_trkEta[iCent][iVPt][iTrkPt]->Fill(t_eta, wTrk);
                            h_trkPhi[iCent][iVPt][iTrkPt]->Fill(t_phi, wTrk);
                            h_dphi_EPn1_trk[iCent][iVPt][iTrkPt]->Fill(std::fabs(getDPHI(t_phi, hiEvt.hiEvtPlanes[2])), wTrk);
                            h_dphi_EPn2_trk[iCent][iVPt][iTrkPt]->Fill(std::fabs(getDPHI(t_phi, hiEvt.hiEvtPlanes[8])), wTrk);
                            h_dphi_EPn3_trk[iCent][iVPt][iTrkPt]->Fill(std::fabs(getDPHI(t_phi, hiEvt.hiEvtPlanes[15])), wTrk);
                            h_dphi_phi0_trk[iCent][iVPt][iTrkPt]->Fill(std::fabs(getDPHI(t_phi, hiEvt.phi0)), wTrk);

                            h_deta[iCent][iVPt][iTrkPt]->Fill(deta, wTrk);

                            h2_trkPhi_vs_trkEta[iCent][iVPt][iTrkPt]->Fill(t_eta, t_phi, wTrk);

                            if (anaTrkID) {
                                h_trkPtError[iCent][iVPt][iTrkPt]->Fill((*p_trkPtError)[i], wTrk);
                                h_trkPtoErr[iCent][iVPt][iTrkPt]->Fill((*p_trkPtError)[i] / t_pt, wTrk);
                                h_trkDz1[iCent][iVPt][iTrkPt]->Fill(std::fabs((*p_trkDz1)[i]), wTrk);
                                h_trkDz1oErr[iCent][iVPt][iTrkPt]->Fill(std::fabs((*p_trkDz1)[i] / (*p_trkDzError1)[i]), wTrk);
                                h_trkDxy1[iCent][iVPt][iTrkPt]->Fill(std::fabs((*p_trkDxy1)[i]), wTrk);
                                h_trkDxy1oErr[iCent][iVPt][iTrkPt]->Fill(std::fabs((*p_trkDxy1)[i] / (*p_trkDxyError1)[i]), wTrk);
                                h_trkNHit[iCent][iVPt][iTrkPt]->Fill((int)((*p_trkNHit)[i]), wTrk);
                                h_trkNdof[iCent][iVPt][iTrkPt]->Fill((int)((*p_trkNdof)[i]), wTrk);
                                h_trkNlayer[iCent][iVPt][iTrkPt]->Fill((int)((*p_trkNlayer)[i]), wTrk);
                                h_trkChi2[iCent][iVPt][iTrkPt]->Fill((*p_trkChi2)[i], wTrk);
                                h_trkChi2_Ndof_Nlayer[iCent][iVPt][iTrkPt]->Fill(((*p_trkChi2)[i] / (*p_trkNdof)[i] / (*p_trkNlayer)[i]), wTrk);
                                h_trkAlgo[iCent][iVPt][iTrkPt]->Fill((int)((*p_trkAlgo)[i]), wTrk);
                                h_trkMVA[iCent][iVPt][iTrkPt]->Fill((*p_trkMVA)[i], wTrk);
                            }

                            if (anavTrk_zh) {
                                float z_vt = t_pt / vPt;
                                h_zh[iCent][iVPt][iTrkPt]->Fill(z_vt, wTrk);
                                h_zh_T[iCent][iVPt][iTrkPt]->Fill(z_vt_T, wTrk);
                            }
                            h_xivh[iCent][iVPt][iTrkPt]->Fill(xi_vt, wTrk);
                            h_xitvh[iCent][iVPt][iTrkPt]->Fill(xit_vt, wTrk);

                            h2_deta_vs_xivh[iCent][iVPt][iTrkPt]->Fill(xi_vt, deta, wTrk);
                            if (findLeadingTrk) {
                                h2_deta_h1_vs_xivh[iCent][iVPt][iTrkPt]->Fill(xi_vt, deta_h1, wTrk);
                            }

                            if (iVPt == 0) {
                                h2_vPt_vs_xivh[iCent][iTrkPt]->Fill(xi_vt, vPt, wTrk);
                            }

                            if (isMC) {
                                float angle_gen = vVgen.Angle(vTrk.Vect());
                                float z_vt_T_gen = vTrk.P() * fabs(cos(angle_gen)) / genVPt;
                                float xi_vt_gen = log(1.0 / z_vt_T_gen);

                                h2_vReco_vs_vGen_xivh[iCent][iVPt][iTrkPt]->Fill(xi_vt_gen, xi_vt, wTrk);
                            }

                            if (fillBootStrap) {
                                h2_bs_xivh[iCent][iVPt][iTrkPt]->Fill(xi_vt, evtIndex_bs_xivh[iCent][iVPt][iTrkPt], wTrk);
                            }

                            if (anaJets) {
                                double mindR2_jet_trk = mindR2_jet_trk_0;
                                int iJet_mindR = -1;

                                int iEvt = (*p_evtIndex)[i];

                                for (int iJet = 0; iJet < nJets; ++iJet) {

                                    if ( !((*p_jetpt)[iJet] > effDR_min_jetpt) )  continue;

                                    if (canUseMixJets && isMixTrk && iEvt != (*p_evtjet_index)[iJet]) continue;
                                    if (useSigJets && (*p_subid)[iJet] != 0 ) continue;

                                    double dR2_jet_trk = getDR2(t_eta, t_phi, (*p_jeteta)[iJet], (*p_jetphi)[iJet]);

                                    if (dR2_jet_trk < mindR2_jet_trk) {
                                        mindR2_jet_trk = dR2_jet_trk;
                                        iJet_mindR = iJet;
                                    }
                                }

                                if (iJet_mindR >= 0) {
                                    if (getDR2(llEta[0], llPhi[0], (*p_jeteta)[iJet_mindR], (*p_jetphi)[iJet_mindR]) < maxdR2_jet_l ||
                                        getDR2(llEta[1], llPhi[1], (*p_jeteta)[iJet_mindR], (*p_jetphi)[iJet_mindR]) < maxdR2_jet_l) {
                                        iJet_mindR = -1;
                                    }
                                }

                                if (iJet_mindR >= 0) {
                                    h2_jetpt_vs_xivh[iCent][iVPt][iTrkPt]->Fill(xi_vt, (*p_jetpt)[iJet_mindR], wTrk);
                                    h2_rawpt_vs_xivh[iCent][iVPt][iTrkPt]->Fill(xi_vt, (*p_rawpt)[iJet_mindR], wTrk);

                                    double mindR_jet_trk = std::sqrt(mindR2_jet_trk);
                                    h2_jetpt_vs_dR[iCent][iVPt][iTrkPt]->Fill(mindR_jet_trk, (*p_jetpt)[iJet_mindR], wTrk);
                                    h2_rawpt_vs_dR[iCent][iVPt][iTrkPt]->Fill(mindR_jet_trk, (*p_rawpt)[iJet_mindR], wTrk);
                                    h2_jetpt_vs_dR_rebin[iCent][iVPt][iTrkPt]->Fill(mindR_jet_trk, (*p_jetpt)[iJet_mindR], wTrk);
                                    h2_rawpt_vs_dR_rebin[iCent][iVPt][iTrkPt]->Fill(mindR_jet_trk, (*p_rawpt)[iJet_mindR], wTrk);
                                }
                            }
                        }

                    }
                }

                for (int iVPt = 0; iVPt < nVPts; ++iVPt) {

                    if (!(vPtsMin[iVPt] <= vPt && vPt < vPtsMax[iVPt]))  continue;

                    for (int iTrkPt = 0; iTrkPt < nTrkPts; ++iTrkPt) {

                        if (!(trkPtsMin[iTrkPt] <= t_pt && t_pt < trkPtsMax[iTrkPt]))  continue;
                        wTrkSums_noDphi[iVPt][iTrkPt] += wTrk;

                        float dphi = std::fabs(getDPHI(vPhi, t_phi));
                        if (!(dphiMinTmp < dphi && dphi <= dphiMaxTmp)) continue;
                        wTrkSums[iVPt][iTrkPt] += wTrk;
                    }
                }
            }
            for (int j = 0; j < nVPts; ++j) {

                if (!(vPtsMin[j] <= vPt && vPt < vPtsMax[j]))  continue;

                for (int k = 0; k < nTrkPts; ++k) {
                    h2_wTrkSum_vs_cent[j][k]->Fill(cent, (wTrkSums[j][k] / wV), wV);
                    h2_wTrkSum_vs_cent_noDphi[j][k]->Fill(cent, (wTrkSums_noDphi[j][k] / wV), wV);
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

    std::cout << "wEvtsAll  = " << wEvtsAll << std::endl;
    std::cout << "wEvtsV    = " << wEvtsV << std::endl;

    output->cd();
    std::cout << "### post loop processing - START ###" << std::endl;

    std::cout << "norm" << std::endl;
    TH1D* h1DTmp = 0;
    for (int i = 0; i < nCents; ++i) {
        std::string hNameTmp;

        hNameTmp = replaceFirst((h_vPt[i]->GetName()), "h_", "h_normEvts_");
        h1DTmp = (TH1D*)h_vPt[i]->Clone(hNameTmp.c_str());
        h1DTmp->Scale(1./wEvtsAll);
        h1DTmp->Write("",TObject::kOverwrite);

        hNameTmp = replaceFirst((h_vPt_ss[i]->GetName()), "h_", "h_normEvts_");
        h1DTmp = (TH1D*)h_vPt_ss[i]->Clone(hNameTmp.c_str());
        h1DTmp->Scale(1./wEvtsAll);
        h1DTmp->Write("",TObject::kOverwrite);

        for (int j = 0; j < nVPts; ++j) {
            hNameTmp = replaceFirst((h_vM_os[i][j]->GetName()), "h_", "h_normEvts_");
            h1DTmp = (TH1D*)h_vM_os[i][j]->Clone(hNameTmp.c_str());
            h1DTmp->Scale(1./wEvtsAll);
            h1DTmp->Write("",TObject::kOverwrite);

            hNameTmp = replaceFirst((h_vM_ss[i][j]->GetName()), "h_", "h_normEvts_");
            h1DTmp = (TH1D*)h_vM_ss[i][j]->Clone(hNameTmp.c_str());
            h1DTmp->Scale(1./wEvtsAll);
            h1DTmp->Write("",TObject::kOverwrite);

            hNameTmp = replaceFirst((h_vY[i][j]->GetName()), "h_", "h_normEvts_");
            h1DTmp = (TH1D*)h_vY[i][j]->Clone(hNameTmp.c_str());
            h1DTmp->Scale(1./wEvtsAll);
            h1DTmp->Write("",TObject::kOverwrite);

            hNameTmp = replaceFirst((h_vY_ss[i][j]->GetName()), "h_", "h_normEvts_");
            h1DTmp = (TH1D*)h_vY_ss[i][j]->Clone(hNameTmp.c_str());
            h1DTmp->Scale(1./wEvtsAll);
            h1DTmp->Write("",TObject::kOverwrite);
        }
    }

    std::cout << "rebin" << std::endl;
    for (int i = 0; i < nCents; ++i) {
        std::string tmpName;
        std::vector<double> binsX;
        int nBinsX = 0;

        binsX = binsX_vPt_rebin;
        nBinsX = binsX.size()-1;

        double arr_vPt[nBinsX+1];
        std::copy(binsX.begin(), binsX.end(), arr_vPt);

        tmpName = replaceAll(h_vPt[i]->GetName(), "h_vPt", "h_vPt_rebin");
        h1DTmp = (TH1D*)h_vPt[i]->Rebin(nBinsX, tmpName.c_str(), arr_vPt);
        h1DTmp->Write("",TObject::kOverwrite);
        vec_h_os.push_back(h1DTmp);

        tmpName = replaceAll(tmpName.c_str(), "h_vPt_rebin", "h_vPt_rebin_normBinW");
        h1DTmp->SetName(tmpName.c_str());
        h1DTmp->Scale(1, "width");
        h1DTmp->Write("",TObject::kOverwrite);

        tmpName = replaceAll(h_vPt_ss[i]->GetName(), "h_vPt_ss", "h_vPt_ss_rebin");
        h1DTmp = (TH1D*)h_vPt_ss[i]->Rebin(nBinsX, tmpName.c_str(), arr_vPt);
        h1DTmp->Write("",TObject::kOverwrite);
        vec_h_ss.push_back(h1DTmp);

        tmpName = replaceAll(tmpName.c_str(), "h_vPt_ss_rebin", "h_vPt_ss_rebin_normBinW");
        h1DTmp->SetName(tmpName.c_str());
        h1DTmp->Scale(1, "width");
        h1DTmp->Write("",TObject::kOverwrite);

        for (int j = 0; j < nVPts; ++j) {

            if (anaTrks) {
                // rebin trkPt
                //binW = h_trkPt[i][j]->GetBinWidth(1);
                binsX = binsX_trkPt_rebin;
                nBinsX = binsX.size()-1;

                double arr_trkPt[nBinsX+1];
                std::copy(binsX.begin(), binsX.end(), arr_trkPt);

                tmpName = replaceAll(h_trkPt[i][j]->GetName(), "h_trkPt", "h_trkPt_rebin");
                h1DTmp = (TH1D*)h_trkPt[i][j]->Rebin(nBinsX, tmpName.c_str(), arr_trkPt);
                h1DTmp->Write("",TObject::kOverwrite);

                // 2nd rebin of trkPt
                binsX = binsX_trkPt_rebin2;
                nBinsX = binsX.size()-1;

                double arr_trkPt2[nBinsX+1];
                std::copy(binsX.begin(), binsX.end(), arr_trkPt2);

                tmpName = replaceAll(h_trkPt[i][j]->GetName(), "h_trkPt", "h_trkPt_rebin2");
                h1DTmp = (TH1D*)h_trkPt[i][j]->Rebin(nBinsX, tmpName.c_str(), arr_trkPt2);
                h1DTmp->Write("",TObject::kOverwrite);

                for (int k = 0; k < nTrkPts; ++k) {

                    // rebin dphi
                    binsX = binsX_dphi_rebin;
                    nBinsX = binsX.size()-1;

                    double arr_dphi[nBinsX+1];
                    std::copy(binsX.begin(), binsX.end(), arr_dphi);

                    tmpName = replaceAll(h_dphi[i][j][k]->GetName(), "h_dphi", "h_dphi_rebin");
                    h1DTmp = (TH1D*)h_dphi[i][j][k]->Rebin(nBinsX, tmpName.c_str(), arr_dphi);
                    h1DTmp->Write("",TObject::kOverwrite);
                }
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

        hTmp = (TH1*)vec_h_num[i]->Clone(tmpName.c_str());
        hTmp->Divide(vec_h_denom[i]);
        if (hTmp->InheritsFrom("TH1D")) {
            hTmp->GetYaxis()->SetTitle("Efficiency");
        }
        hTmp->Write("",TObject::kOverwrite);

        if (hTmp->InheritsFrom("TH1D")) {
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
    }

    std::cout << "SS / OS ratio" << std::endl;
    hTmp = 0;
    int nVec_h_ss = vec_h_ss.size();
    int nVec_h_os = vec_h_os.size();
    if (nVec_h_ss != nVec_h_os) {
        std::cout << "ERROR : different number of numerator and denominators" << std::endl;
    }
    for (int i = 0; i < nVec_h_ss; ++i) {
        std::string tmpName = vec_h_ss[i]->GetName();

        tmpName = replaceAll(tmpName, "_ss_", "_");
        tmpName = replaceFirst(tmpName, "h_", "h_ratio_ss_os_");

        hTmp = (TH1*)vec_h_ss[i]->Clone(tmpName.c_str());
        hTmp->Divide(vec_h_os[i]);
        if (hTmp->InheritsFrom("TH1D")) {
            hTmp->GetYaxis()->SetTitle("Same sign / Opposite sign");
        }
        hTmp->Write("",TObject::kOverwrite);
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

    applyWeightsV = confParser.ReadConfigValueInteger("applyWeightsV");

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

    std::cout << "applyWeightsV = " << applyWeightsV << std::endl;

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

std::string parseVersionResidualTrkW(std::string path)
{
    std::string substr = "";
    if (path.find("resMBTrkWeffv") != std::string::npos) {
        substr = "resMBTrkWeffv";
    }
    else if (path.find("resMBTrkWv") != std::string::npos) {
        substr = "resMBTrkWv";
    }

    int len = substr.size();
    if (len == 0) {
        return "";
    }

    std::string versionNum = path.substr(path.find(substr) + len, 2);
    if (std::atoi(versionNum.c_str()) < 10) {
        versionNum = path.substr(path.find(substr) + len, 1);
    }

    return "v"+versionNum;
}
