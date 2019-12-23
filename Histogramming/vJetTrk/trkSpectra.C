/*
 * macro to fill histograms of track spectra, to be used for residual track corrections
 */

#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TObjArray.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TMath.h>

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>

#include "../../CorrelationTuple/EventMatcher.h"
#include "../../TreeHeaders/CutConfigurationTree.h"
#include "../../TreeHeaders/hiEvtTree.h"
#include "../../TreeHeaders/skimAnalysisTree.h"
#include "../../TreeHeaders/trackTree.h"
#include "../../TreeHeaders/JetTree.h"
#include "../../TreeHeaders/hiGenParticleTree.h"
#include "../../Utilities/interface/ArgumentParser.h"
#include "../../Utilities/interface/ConfigurationParser.h"
#include "../../Utilities/interface/GraphicsConfigurationParser.h"
#include "../../Utilities/interface/InputConfigurationParser.h"
#include "../../Utilities/interface/HiForestInfoController.h"
#include "../../Utilities/systemUtil.h"
#include "../../Utilities/eventUtil.h"
#include "../../Utilities/styleUtil.h"
#include "../../Utilities/th1Util.h"
#include "../../Utilities/fileUtil.h"
#include "../../Utilities/physicsUtil.h"
#include "../../Utilities/vJetTrkUtil.h"
#include "../../Corrections/tracks/2017pp/trackingEfficiency2017pp.h"
#include "../../Corrections/tracks/2018PbPb/trackingEfficiency2018PbPb.h"

///// global variables
/// configuration variables

/// configuration variables - END

std::vector<std::string> argOptions;
///// global variables - END

void trkSpectra(std::string configFile, std::string inputFile, std::string outputFile = "trkSpectra.root");
void trkSpectraNoLoop(std::string configFile, std::string inputFile, std::string outputFile = "trkSpectra.root");

void trkSpectra(std::string configFile, std::string inputFile, std::string outputFile)
{
    std::cout<<"running trkSpectra()"<<std::endl;
    std::cout<<"configFile  = "<< configFile.c_str() <<std::endl;
    std::cout<<"inputFile   = "<< inputFile.c_str()  <<std::endl;
    std::cout<<"outputFile  = "<< outputFile.c_str() <<std::endl;

    std::string sampleType = (ArgumentParser::optionExists("--sampleType", argOptions)) ?
            ArgumentParser::ParseOptionInputSingle("--sampleType", argOptions).c_str() : "pbpb_2018_data";

    int applyTrkWeights = (ArgumentParser::optionExists("--applyTrkWeights", argOptions)) ?
            std::atoi(ArgumentParser::ParseOptionInputSingle("--applyTrkWeights", argOptions).c_str()) : 1;

    int applyNcollWeights = (ArgumentParser::optionExists("--applyNcollWeights", argOptions)) ?
            std::atoi(ArgumentParser::ParseOptionInputSingle("--applyNcollWeights", argOptions).c_str()) : 1;

    int skipMu = (ArgumentParser::optionExists("--skipMu", argOptions)) ?
            std::atoi(ArgumentParser::ParseOptionInputSingle("--skipMu", argOptions).c_str()) : 0;

    int skipEle = (ArgumentParser::optionExists("--skipEle", argOptions)) ?
            std::atoi(ArgumentParser::ParseOptionInputSingle("--skipEle", argOptions).c_str()) : 0;

    double skipEvtElePt = (ArgumentParser::optionExists("--skipEvtElePt", argOptions)) ?
            std::atof(ArgumentParser::ParseOptionInputSingle("--skipEvtElePt", argOptions).c_str()) : 5;

    double skipEvtMuPt = (ArgumentParser::optionExists("--skipEvtMuPt", argOptions)) ?
            std::atof(ArgumentParser::ParseOptionInputSingle("--skipEvtMuPt", argOptions).c_str()) : 9999;

    double skipEvtTauPt = (ArgumentParser::optionExists("--skipEvtTauPt", argOptions)) ?
            std::atof(ArgumentParser::ParseOptionInputSingle("--skipEvtTauPt", argOptions).c_str()) : -1;

    double skipEvtTauNuPt = (ArgumentParser::optionExists("--skipEvtTauNuPt", argOptions)) ?
            std::atof(ArgumentParser::ParseOptionInputSingle("--skipEvtTauNuPt", argOptions).c_str()) : -1;

    int maxNVtx = (ArgumentParser::optionExists("--maxNVtx", argOptions)) ?
            std::atoi(ArgumentParser::ParseOptionInputSingle("--maxNVtx", argOptions).c_str()) : 0;
    int minNVtx = (ArgumentParser::optionExists("--minNVtx", argOptions)) ?
            std::atoi(ArgumentParser::ParseOptionInputSingle("--minNVtx", argOptions).c_str()) : -1;

    int anajets = (ArgumentParser::optionExists("--anajets", argOptions)) ?
            std::atoi(ArgumentParser::ParseOptionInputSingle("--anajets", argOptions).c_str()) : 0;

    double jetptMin = (ArgumentParser::optionExists("--jetptMin", argOptions)) ?
            std::atof(ArgumentParser::ParseOptionInputSingle("--jetptMin", argOptions).c_str()) : 50;

    std::string th1NamesStr = (ArgumentParser::optionExists("--th1Names", argOptions)) ?
            ArgumentParser::ParseOptionInputSingle("--th1Names", argOptions).c_str() : "phi,phi_vs_eta";
    // runs all histograms if "th1Names" is not specified
    std::vector<std::string> th1Names = split(th1NamesStr, ",", false, false);
    int nTh1Names = th1Names.size();

    std::string centRangesStr = (ArgumentParser::optionExists("--cents", argOptions)) ?
            ArgumentParser::ParseOptionInputSingle("--cents", argOptions).c_str() : "";  // "0:30,30:100"
    std::vector<double> centsMinTmp = parseRangesMin(centRangesStr);
    std::vector<double> centsMaxTmp = parseRangesMax(centRangesStr);
    std::vector<int> centsMin(centsMinTmp.begin(), centsMinTmp.end());
    std::vector<int> centsMax(centsMaxTmp.begin(), centsMaxTmp.end());
    int nCents = centsMin.size();

    std::string trkPtRangesStr = (ArgumentParser::optionExists("--trkPts", argOptions)) ?
            ArgumentParser::ParseOptionInputSingle("--trkPts", argOptions).c_str() : "1:0";  // "1:0,1:2,2:4,4:8"
    std::vector<double> trkPtsMin = parseRangesMin(trkPtRangesStr);
    std::vector<double> trkPtsMax = parseRangesMax(trkPtRangesStr);
    int nTrkPts = trkPtsMin.size();

    std::string trkEtaRangesStr = (ArgumentParser::optionExists("--trkEtas", argOptions)) ?
            ArgumentParser::ParseOptionInputSingle("--trkEtas", argOptions).c_str() : "0:2.4,0:1.0";
    std::vector<double> trkEtasMin = parseRangesMin(trkEtaRangesStr);
    std::vector<double> trkEtasMax = parseRangesMax(trkEtaRangesStr);
    int nTrkEtas = trkEtasMin.size();

    std::cout << "sampleType = " << sampleType << std::endl;
    std::cout << "applyTrkWeights = " << applyTrkWeights << std::endl;
    std::cout << "applyNcollWeights = " << applyNcollWeights << std::endl;
    std::cout << "skipMu  = " << skipMu << std::endl;
    std::cout << "skipEle = " << skipEle << std::endl;
    std::cout << "skipEvtElePt = " << skipEvtElePt << std::endl;
    std::cout << "skipEvtMuPt  = " << skipEvtMuPt << std::endl;
    std::cout << "skipEvtTauPt = " << skipEvtTauPt << std::endl;
    std::cout << "skipEvtTauNuPt = " << skipEvtTauNuPt << std::endl;
    std::cout << "maxNVtx = " << maxNVtx << std::endl;
    std::cout << "minNVtx = " << minNVtx << std::endl;
    std::cout << "anajets  = " << anajets << std::endl;
    std::cout << "jetptMin  = " << jetptMin << std::endl;

    std::cout << "nTh1Names = " << nTh1Names << std::endl;
    for (int i = 0; i < nTh1Names; ++i) {
        std::cout << Form("th1Names[%d] = %s", i, th1Names[i].c_str()) << std::endl;
    }

    std::cout << "nCents = " << nCents << std::endl;
    for (int i = 0; i < nCents; ++i) {
        std::cout << Form("cents[%d] = [%d, %d)", i, centsMin[i], centsMax[i]) << std::endl;
    }

    std::cout << "nTrkPts = " << nTrkPts << std::endl;
    for (int i = 0; i < nTrkPts; ++i) {
        std::cout << Form("trkPts[%d] = [%f, %f)", i, trkPtsMin[i], trkPtsMax[i]) << std::endl;
    }

    std::cout << "nTrkEtas = " << nTrkEtas << std::endl;
    for (int i = 0; i < nTrkEtas; ++i) {
        std::cout << Form("trkEtas[%d] = [%f, %f)", i, trkEtasMin[i], trkEtasMax[i]) << std::endl;
    }

    bool isMC = (toLowerCase(sampleType).find("_mc") != std::string::npos);
    bool isPbPb = (toLowerCase(sampleType).find("pbpb") != std::string::npos);
    bool isPP = (toLowerCase(sampleType).find("pp") != std::string::npos);

    bool isPbPb15 = isPbPb && (sampleType.find("2015") != std::string::npos);
    bool isPbPb18 = isPbPb && (sampleType.find("2018") != std::string::npos);
    bool isPP15 = isPP && (sampleType.find("2015") != std::string::npos);
    bool isPP17 = isPP && (sampleType.find("2017") != std::string::npos);

    bool do_phi = containsElement(th1Names, "phi");
    bool do_phi_vs_eta = containsElement(th1Names, "phi_vs_eta");

    std::cout << "isMC = " << isMC << std::endl;
    std::cout << "isPbPb15 = " << isPbPb15 << std::endl;
    std::cout << "isPbPb18 = " << isPbPb18 << std::endl;
    std::cout << "isPP15 = " << isPP15 << std::endl;
    std::cout << "isPP17 = " << isPP17 << std::endl;

    std::cout << "do_phi = " << do_phi << std::endl;
    std::cout << "do_phi_vs_eta = " << do_phi_vs_eta << std::endl;

    bool shiftHibin = (isPbPb18 && isMC && false);
    if (shiftHibin) {
        std::cout << "shifting hiBin" << std::endl;
    }

    std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.c_str());
    std::cout<<"input ROOT files : num = "<<inputFiles.size()<< std::endl;
    std::cout<<"#####"<< std::endl;
    for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
        std::cout<<(*it).c_str()<< std::endl;
    }
    std::cout<<"##### END #####"<< std::endl;

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

    TrkEff2018PbPb trkEff2018 =  TrkEff2018PbPb("general", true, "Corrections/tracks/2018PbPb/");

    TrkEff2017pp trkEff2017 =  TrkEff2017pp(true, "Corrections/tracks/2017pp/");

    TFile* output = TFile::Open(outputFile.c_str(),"RECREATE");
    output->cd();

    // initialize objects
    TH1::SetDefaultSumw2();

    TH1* hTmp = 0;

    std::vector<TH1*> vec_h;

    std::string text_trk = "trk";
    std::string text_trkPt = Form("p^{%s}_{T}", text_trk.c_str());
    std::string text_trkEta = Form("#eta^{%s}", text_trk.c_str());
    std::string text_trkPhi = Form("#phi^{%s}", text_trk.c_str());

    std::string text_absTrkEta = Form("|%s|", text_trkEta.c_str());

    int nBinsX_phi = 20;
    double xMax_phi = TMath::Pi()+1e-12;

    int nBinsX_eta = 24;
    double xMax_eta = 2.4;

    enum RG {
        k_Gen,
        k_RecoEffCorr,
        k_RecoCorr,
        k_RecoUncorr,
        kN_RG,
    };

    std::string strRG[kN_RG] = {"gen", "trkEffCorr", "trkCorr", "trkUncorr"};

    std::vector<double> wEvtTot(nCents, 0);
    TH1D* h_pt[nCents][nTrkEtas][RG::kN_RG];
    TH1D* h_pt_rebin[nCents][nTrkEtas][RG::kN_RG];
    TH1D* h_pt_rebin_normBinW[nCents][nTrkEtas][RG::kN_RG];

    TH1D* h_eta[nCents][nTrkPts][RG::kN_RG];

    // phi_vs_eta with user-defined bins
    TH2D* h2_phi_vs_eta_var[nCents][nTrkPts][RG::kN_RG];

    double xMax_trkPt = 90;
    std::vector<double> binsX_pt_rebin = {0, 0.5, 1, 2, 3, 4, 5, 7.50, 10.0, 12.0, 15.0, 20.0, 25.0, 30.0, 45.0, 60.0, xMax_trkPt};

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

        for (int j = 0; j < nTrkEtas; ++j) {

            double tmpTrkEta = -1;
            tmpTrkEta = trkEtasMin[j];
            std::string label_trkEtaMin = Form("%d", (int)(tmpTrkEta));    // 5 --> "5"
            if (std::floor(tmpTrkEta) != tmpTrkEta) {   // 1.4 --> "1p4"
                label_trkEtaMin = Form("%dp%d", (int)(tmpTrkEta), ((int)(tmpTrkEta*10) % 10));
            }

            tmpTrkEta = trkEtasMax[j];
            std::string label_trkPtMax = Form("%d", (int)(tmpTrkEta));    // 5 --> "5"
            if (std::floor(tmpTrkEta) != tmpTrkEta) {   // 1.4 --> "1p4"
                label_trkPtMax = Form("%dp%d", (int)(tmpTrkEta), ((int)(tmpTrkEta*10) % 10));
            }
            std::string label_trkEta = Form("eta%s_%s", label_trkEtaMin.c_str(), label_trkPtMax.c_str());

            std::string text_trkEtaMin = replaceAll(label_trkEtaMin, "p", ".");
            std::string text_trkEtaMax = replaceAll(label_trkPtMax, "p", ".");
            std::string text_range_trkEta = Form("%s < %s < %s", text_trkEtaMin.c_str(), text_absTrkEta.c_str(), text_trkEtaMax.c_str());
            if (trkEtasMin[j] == 0) {
                text_range_trkEta = Form("%s < %s", text_absTrkEta.c_str(), text_trkEtaMax.c_str());
            }
            else if (trkEtasMax[j] < trkEtasMin[j]) {
                label_trkEta = Form("eta%s_0", label_trkEtaMin.c_str());
                text_range_trkEta = Form("%s > %s", text_absTrkEta.c_str(), text_trkEtaMin.c_str());
            }

            std::string title_h_pt = Form("%s, %s;%s;", text_range_trkEta.c_str(), text_range_cent.c_str(),
                                                    text_trkPt.c_str());

            std::string name_h_suffix = Form("%s_%s", label_trkEta.c_str(), label_cent.c_str());

            for (int iRG = 0; iRG < RG::kN_RG; ++iRG) {

                std::string name_h_pt = Form("h_pt_%s_%s", strRG[iRG].c_str(), name_h_suffix.c_str());

                h_pt[i][j][iRG] = 0;
                h_pt[i][j][iRG] = new TH1D(name_h_pt.c_str(), title_h_pt.c_str(), 180, 0, xMax_trkPt);

                h_pt[i][j][iRG]->SetMarkerStyle(kFullCircle);
                vec_h.push_back(h_pt[i][j][iRG]);
            }
        }

        for (int j = 0; j < nTrkPts; ++j) {

            double tmpTrkPt = -1;
            tmpTrkPt = trkPtsMin[j];
            std::string label_trkPtMin = Form("%d", (int)(tmpTrkPt));    // 5 --> "5"
            if (std::floor(tmpTrkPt) != tmpTrkPt) {   // 1.4 --> "1p4"
                label_trkPtMin = Form("%dp%d", (int)(tmpTrkPt), ((int)(tmpTrkPt*10) % 10));
            }

            tmpTrkPt = trkPtsMax[j];
            std::string label_trkPtMax = Form("%d", (int)(tmpTrkPt));    // 5 --> "5"
            if (std::floor(tmpTrkPt) != tmpTrkPt) {   // 1.4 --> "1p4"
                label_trkPtMax = Form("%dp%d", (int)(tmpTrkPt), ((int)(tmpTrkPt*10) % 10));
            }
            std::string label_trkPt = Form("pt%s_%s", label_trkPtMin.c_str(), label_trkPtMax.c_str());

            std::string text_trkPtMin = replaceAll(label_trkPtMin, "p", ".");
            std::string text_trkPtMax = replaceAll(label_trkPtMax, "p", ".");
            std::string text_range_trkPt = Form("%s < %s < %s", text_trkPtMin.c_str(), text_trkPt.c_str(), text_trkPtMax.c_str());
            if (trkPtsMin[j] == 0) {
                text_range_trkPt = Form("%s < %s", text_trkPt.c_str(), text_trkPtMax.c_str());
            }
            else if (trkPtsMax[j] < trkPtsMin[j]) {
                label_trkPt = Form("pt%s_0", label_trkPtMin.c_str());
                text_range_trkPt = Form("%s > %s", text_trkPt.c_str(), text_trkPtMin.c_str());
            }

            std::string title_h_eta = Form("%s, %s;%s;", text_range_trkPt.c_str(), text_range_cent.c_str(),
                                                    text_trkEta.c_str());
            std::string title_h2_phi_vs_eta = Form("%s, %s;%s;%s", text_range_trkPt.c_str(), text_range_cent.c_str(),
                                                    text_trkEta.c_str(), text_trkPhi.c_str());

            std::string name_h_suffix = Form("%s_%s", label_trkPt.c_str(), label_cent.c_str());

            for (int iRG = 0; iRG < RG::kN_RG; ++iRG) {

                std::string name_h_eta = Form("h_eta_%s_%s", strRG[iRG].c_str(), name_h_suffix.c_str());

                h_eta[i][j][iRG] = 0;
                h_eta[i][j][iRG] = new TH1D(name_h_eta.c_str(), title_h_eta.c_str(), 12, -2.4, 2.4);

                h_eta[i][j][iRG]->SetMarkerStyle(kFullCircle);
                vec_h.push_back(h_eta[i][j][iRG]);


                std::string name_h2_phi_vs_eta = Form("h2_phi_vs_eta_%s_%s", strRG[iRG].c_str(), name_h_suffix.c_str());

                h2_phi_vs_eta_var[i][j][iRG] = 0;
                h2_phi_vs_eta_var[i][j][iRG] = new TH2D(name_h2_phi_vs_eta.c_str(), title_h2_phi_vs_eta.c_str(),
                                                        12, -2.4, 2.4,
                                                        nBinsX_phi, -1*xMax_phi, xMax_phi);

                h2_phi_vs_eta_var[i][j][iRG]->SetMarkerStyle(kFullCircle);
                vec_h.push_back(h2_phi_vs_eta_var[i][j][iRG]);
            }
        }
    }

    std::vector<double> trkPts_phi = getVecPt4TrkW();
    int nBinsPt_phi = trkPts_phi.size() - 1;

    std::vector<double> trkEtasMin_phi = getVecEta4TrkW();
    int nBinsEta_phi = trkEtasMin_phi.size();
    double widthEta_phi = (2.4*2) / nBinsEta_phi;

    std::vector<int> hiBinsMin_phi = getVecCent4TrkW();
    int nBinsCent_phi = hiBinsMin_phi.size();
    int widthCent_phi = 200 / nBinsCent_phi;

    TH1D* h_trkPhi[nBinsPt_phi][nBinsEta_phi][nBinsCent_phi];

    if (do_phi) {
        for (int i = 0; i < nBinsPt_phi; ++i) {

            double trkPtMin = trkPts_phi[i];
            double trkPtMax = trkPts_phi[i+1];

            std::string text_range_trkPt = Form("%.2f < %s < %.2f", trkPtMin, text_trkPt.c_str(), trkPtMax);

            for (int j = 0; j < nBinsEta_phi; ++j) {

                double trkEtaMin = trkEtasMin_phi[j]-widthEta_phi;
                double trkEtaMax = trkEtasMin_phi[j];

                std::string text_range_trkEta = Form("%.1f < %s < %.1f", trkEtaMin, text_trkEta.c_str(), trkEtaMax);

                for (int k = 0; k < nBinsCent_phi; ++k) {

                    int hiBinMin = hiBinsMin_phi[k]-widthCent_phi;
                    int hiBinMax = hiBinsMin_phi[k];

                    std::string text_range_hiBin = Form("hiBin:%d-%d", hiBinMin, hiBinMax);

                    std::string title_h_suffix = Form("%s, %s, %s",
                                                                text_range_trkPt.c_str(),
                                                                text_range_trkEta.c_str(),
                                                                text_range_hiBin.c_str());

                    std::string title_h_trkPhi = Form("%s;%s;", title_h_suffix.c_str(),
                                                                text_trkPhi.c_str());

                    std::string name_h_suffix = Form("iPt_%d_iEta_%d_iCent_%d", i, j, k);
                    std::string name_h_trkPhi = Form("h_trkPhi_%s", name_h_suffix.c_str());

                    h_trkPhi[i][j][k] = 0;
                    h_trkPhi[i][j][k] = new TH1D(name_h_trkPhi.c_str(), title_h_trkPhi.c_str(), nBinsX_phi, -1*xMax_phi, xMax_phi);

                    h_trkPhi[i][j][k]->SetMarkerStyle(kFullCircle);

                    vec_h.push_back(h_trkPhi[i][j][k]);
                }
            }
        }
    }

    std::vector<double> pts_phieta = getVecPt4TrkWCoarse();
    int nBinsPt_phieta = pts_phieta.size()-1;

    std::vector<int> hiBinsMin_phieta = getVecCent4TrkWCoarse();
    int nBinsCent_phieta = hiBinsMin_phieta.size();

    TH2D* h2_phi_vs_eta[nBinsPt_phieta][nBinsCent_phieta][RG::kN_RG];

    if (do_phi_vs_eta) {
        for (int i = 0; i < nBinsPt_phieta; ++i) {

            double trkPtMin = pts_phieta[i];
            double trkPtMax = pts_phieta[i+1];

            std::string text_range_trkPt = Form("%.2f < %s < %.2f", trkPtMin, text_trkPt.c_str(), trkPtMax);

            for (int k = 0; k < nBinsCent_phieta; ++k) {

                int hiBinMin = (k == 0) ? 0 : (hiBinsMin_phieta[k-1]);
                int hiBinMax = hiBinsMin_phieta[k];

                std::string text_range_hiBin = Form("hiBin:%d-%d", hiBinMin, hiBinMax);

                std::string title_h_suffix = Form("%s, %s", text_range_trkPt.c_str(),
                                                            text_range_hiBin.c_str());

                std::string title_h2_trkPhi_vs_trkEta = Form("%s;%s;%s", title_h_suffix.c_str(),
                                                            text_trkEta.c_str(),
                                                            text_trkPhi.c_str());

                std::string name_h_suffix = Form("iPt_%d_iCent_%d", i, k);

                for (int iRG = 0; iRG < RG::kN_RG; ++iRG) {

                    std::string name_h2_phi_vs_eta = Form("h2_phi_vs_eta_%s_%s", strRG[iRG].c_str(), name_h_suffix.c_str());

                    h2_phi_vs_eta[i][k][iRG] = 0;
                    h2_phi_vs_eta[i][k][iRG] = new TH2D(name_h2_phi_vs_eta.c_str(), title_h2_trkPhi_vs_trkEta.c_str(),
                                                                                    nBinsX_eta, -1*xMax_eta, xMax_eta,
                                                                                    nBinsX_phi, -1*xMax_phi, xMax_phi);

                    h2_phi_vs_eta[i][k][iRG]->SetMarkerStyle(kFullCircle);
                    vec_h.push_back(h2_phi_vs_eta[i][k][iRG]);

                }
            }
        }
    }

    std::vector<double> trkPtsJet = getVecPt4TrkWCoarse2();
    int nBinsTrkPtsJet = trkPtsJet.size()-1;

    std::vector<int> hiBinsMinJet = getVecCent4TrkWCoarse2();
    int nBinsCentJet = hiBinsMinJet.size();

    std::vector<double> absEtasJet = getVecAbsEta4TrkWCoarse();
    int nBinsAbsEtasJet = absEtasJet.size();

    TH2D* h2_jetpt_vs_dR[nBinsTrkPtsJet][nBinsAbsEtasJet][nBinsCentJet][RG::kN_RG];
    TH2D* h2_jetpt_vs_dR_rebin[nBinsTrkPtsJet][nBinsAbsEtasJet][nBinsCentJet][RG::kN_RG];
    //TH2D* h2_rawpt_vs_dR[nBinsTrkPtsJet][nBinsAbsEtasJet][nBinsCentJet][RG::kN_RG];

    if (anajets > 0) {

        for (int i = 0; i < nBinsTrkPtsJet; ++i) {

            double trkPtMin = trkPtsJet[i];
            double trkPtMax = trkPtsJet[i+1];

            std::string text_range_trkPt = Form("%.2f < %s < %.2f", trkPtMin, text_trkPt.c_str(), trkPtMax);

            for (int k = 0; k < nBinsCentJet; ++k) {

                int hiBinMin = (k == 0) ? 0 : (hiBinsMinJet[k-1]);
                int hiBinMax = hiBinsMinJet[k];

                std::string text_range_hiBin = Form("hiBin:%d-%d", hiBinMin, hiBinMax);

                for (int m = 0; m < nBinsAbsEtasJet; ++m) {

                    double absEtaMin = (m == 0) ? 0 : absEtasJet[m-1];
                    double absEtaMax = absEtasJet[m];

                    std::string text_range_trkAbsEta = Form("%.2f < | %s | < %.2f", absEtaMin, text_trkEta.c_str(), absEtaMax);

                    std::string title_h_suffix = Form("%s, %s, %s", text_range_trkPt.c_str(),
                                                                    text_range_trkAbsEta.c_str(),
                                                                    text_range_hiBin.c_str());

                    std::string title_h2_jetpt_vs_dR = Form("%s;%s;%s", title_h_suffix.c_str(),
                                                                "#DeltaR",
                                                                "jet p_{T}");

                    std::string name_h_suffix = Form("iPt_%d_iEta_%d_iCent_%d", i, m, k);

                    for (int iRG = 0; iRG < RG::kN_RG; ++iRG) {

                        std::string name_h2_jetpt_vs_dR = Form("h2_jetpt_vs_dR_%s_%s", strRG[iRG].c_str(), name_h_suffix.c_str());

                        h2_jetpt_vs_dR[i][m][k][iRG] = 0;
                        h2_jetpt_vs_dR[i][m][k][iRG] = new TH2D(name_h2_jetpt_vs_dR.c_str(), title_h2_jetpt_vs_dR.c_str(),
                                                                                        5, 0, 1,
                                                                                        60, 0, 300);

                        h2_jetpt_vs_dR[i][m][k][iRG]->SetMarkerStyle(kFullCircle);
                        vec_h.push_back(h2_jetpt_vs_dR[i][m][k][iRG]);

                        std::string name_h2_jetpt_vs_dR_rebin = Form("h2_jetpt_vs_dR_rebin_%s_%s", strRG[iRG].c_str(), name_h_suffix.c_str());

                        std::vector<double> binsX_jetpt = {0, 10, 20, 30, 40, 50, 70, 100, 150, 200, 300};
                        int nBinsX_jetpt = binsX_jetpt.size()-1;

                        double arr_jetpt[nBinsX_jetpt+1];
                        std::copy(binsX_jetpt.begin(), binsX_jetpt.end(), arr_jetpt);

                        h2_jetpt_vs_dR_rebin[i][m][k][iRG] = 0;
                        h2_jetpt_vs_dR_rebin[i][m][k][iRG] = new TH2D(name_h2_jetpt_vs_dR_rebin.c_str(), title_h2_jetpt_vs_dR.c_str(),
                                                                                        2, 0, 0.8,
                                                                                        nBinsX_jetpt, arr_jetpt);

                        h2_jetpt_vs_dR_rebin[i][m][k][iRG]->SetMarkerStyle(kFullCircle);
                        vec_h.push_back(h2_jetpt_vs_dR_rebin[i][m][k][iRG]);
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
    for (int i = 0; i < nTrkPts; ++i) {
        if (trkPtsMax[i] < trkPtsMin[i]) {
            trkPtsMax[i] = 999999;
        }
    }
    for (int i = 0; i < nTrkEtas; ++i) {
        if (trkEtasMax[i] < trkEtasMin[i]) {
            trkEtasMax[i] = 999999;
        }
    }

    TTree* treeHLT = 0;
    TTree* treeHiEvt = 0;
    TTree* treeSkim = 0;
    TTree* treeTrack = 0;
    TTree* treeHiGenParticle = 0;
    TTree* treeJet = 0;

    std::string treePathHLT = "hltanalysis/HltTree";
    std::string treePathHiEvt = "hiEvtAnalyzer/HiTree";
    std::string treePathSkimAna = "skimanalysis/HltTree";
    std::string treePathTrack = "ppTrack/trackTree";
    std::string treePathGen = "HiGenParticleAna/hi";
    std::string treePathJet = "akPu4CaloJetAnalyzer/t";

    std::vector<std::string> triggerBranches;
    //triggerBranches = {"HLT_HIL2Mu12_v1", "HLT_HIL3Mu12_v1"}; // "HLT_HIL1DoubleMu0_v1", "HLT_HIL1DoubleMu10_v1"
    int nTriggerBranches = triggerBranches.size();

    int nFiles = inputFiles.size();
    TFile* fileTmp = 0;

    std::cout << "initial reading to get the number of entries (if there is only one input file) and HiForest info" << std::endl;
    // read the first file only to get the HiForest info
    std::string inputPath = inputFiles.at(0).c_str();
    fileTmp = TFile::Open(inputPath.c_str(), "READ");
    fileTmp->cd();

    if (nFiles == 1) {
        // read one tree only to get the number of entries
        treeHiEvt = (TTree*)fileTmp->Get(treePathHiEvt.c_str());
        Long64_t entriesTmp = treeHiEvt->GetEntries();
        std::cout << "entries = " << entriesTmp << std::endl;
        treeHiEvt->Delete();
    }

    fileTmp->Close();
    // done with initial reading

    EventMatcher* em = new EventMatcher();
    Long64_t duplicateEntries = 0;

    Long64_t entries = 0;
    Long64_t entriesAnalyzed = 0;

    int nFilesSkipped = 0;
    std::cout<< "Loop : " << treePathHiEvt.c_str() <<std::endl;
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
        treeHiEvt->SetBranchStatus("*", 0);
        treeHiEvt->SetBranchStatus("hiBin", 1);
        treeHiEvt->SetBranchStatus("vz", 1);
        treeHiEvt->SetBranchStatus("run", 1);
        treeHiEvt->SetBranchStatus("lumi", 1);
        treeHiEvt->SetBranchStatus("evt", 1);

        treeSkim = (TTree*)fileTmp->Get(treePathSkimAna.c_str());
        treeSkim->SetBranchStatus("*", 0);

        treeTrack = (TTree*)fileTmp->Get(treePathTrack.c_str());
        treeTrack->SetBranchStatus("*", 0);     // disable all branches

        std::vector<std::string> trkBranches = {
                "nTrk",
                "trkPt",
                "trkEta",
                "trkPhi",
                "highPurity",
                "trkPtError",
                "trkDz1",
                "trkDzError1",
                "trkDxy1",
                "trkDxyError1",
                "trkNHit",
                "trkChi2",
                "trkNdof",
                "trkNlayer",
                "trkAlgo",
                "trkMVA",
                "pfType",
                "pfHcal",
                "pfEcal",
                "nVtx",
        };

        int nTrkBranches = trkBranches.size();
        for (int iTrkBr = 0; iTrkBr < nTrkBranches; ++iTrkBr) {
            treeTrack->SetBranchStatus(trkBranches[iTrkBr].c_str(), 1);
        }

        if (anajets > 0) {
            treeJet = (TTree*)fileTmp->Get(treePathJet.c_str());
            treeJet->SetBranchStatus("*", 0);     // disable all branches

            std::vector<std::string> jetBranches = {
                "nref",
                "jtpt",
                "jteta",
                "jtphi",
                "rawpt",
            };

            int nJetBranches = jetBranches.size();
            for (int iJetBr = 0; iJetBr < nJetBranches; ++iJetBr) {
                treeJet->SetBranchStatus(jetBranches[iJetBr].c_str(), 1);
            }
        }

        if (isMC) {
            treeHiEvt->SetBranchStatus("weight", 1);
            treeHiEvt->SetBranchStatus("pthat",1);

            treeHiGenParticle = (TTree*)fileTmp->Get(treePathGen.c_str());
            treeHiGenParticle->SetBranchStatus("*", 0);     // disable all branches

            std::vector<std::string> genBranches = {
                    "mult",
                    "pt",
                    "eta",
                    "phi",
                    "chg",
                    "pdg",
            };

            int nGenBranches = genBranches.size();
            for (int iTrkBr = 0; iTrkBr < nGenBranches; ++iTrkBr) {
                treeHiGenParticle->SetBranchStatus(genBranches[iTrkBr].c_str(), 1);
            }
        }

        hiEvt hiEvt;
        hiEvt.setupTreeForReading(treeHiEvt);

        skimAnalysis skimAna;
        if (isPbPb15) skimAna.enableBranchesHI(treeSkim);
        else if (isPbPb18) skimAna.enableBranchesHI2018(treeSkim);
        else if (!isPbPb) skimAna.enableBranchesPP(treeSkim);
        skimAna.setupTreeForReading(treeSkim);
        skimAna.checkBranches(treeSkim);    // do the event selection if the branches exist.

        Tracks trks;
        trks.setupTreeForReading(treeTrack);

        Jets jets;
        if (anajets > 0) {
            jets.setupTreeForReading(treeJet);
        }

        hiGenParticle hiGen;
        if (isMC) {
            hiGen.setupTreeForReading(treeHiGenParticle);
        }

        Long64_t entriesTmp = treeHiEvt->GetEntries();
        entries += entriesTmp;
        std::cout << "entries in File = " << entriesTmp << std::endl;
        for (Long64_t j_entry = 0; j_entry < entriesTmp; ++j_entry)
        {
            if (j_entry % 2000 == 0)  {
              std::cout << "current entry = " <<j_entry<<" out of "<<entriesTmp<<" : "<<std::setprecision(2)<<(double)j_entry/entriesTmp*100<<" %"<<std::endl;
            }

            treeHiEvt->GetEntry(j_entry);
            // event selection
            if (!(std::fabs(hiEvt.vz) < 15))  continue;

            treeSkim->GetEntry(j_entry);
            if (isPbPb15 && !skimAna.passedEventSelectionHI())  continue;
            else if (isPbPb18 && !skimAna.passedEventSelectionHI2018())  continue;
            else if (!isPbPb && !skimAna.passedEventSelectionPP())  continue;

            treeHLT->GetEntry(j_entry);
            treeTrack->GetEntry(j_entry);
            if (anajets > 0) {
                treeJet->GetEntry(j_entry);
            }
            if (isMC) {
                treeHiGenParticle->GetEntry(j_entry);
            }

            if (maxNVtx > 0 && trks.nVtx > maxNVtx) {
                continue;
            }
            if (minNVtx > -1 && trks.nVtx < minNVtx) {
                continue;
            }

            bool eventAdded = em->addEvent(hiEvt.run, hiEvt.lumi, hiEvt.evt, j_entry);
            if(!eventAdded) // this event is duplicate, skip this one.
            {
                duplicateEntries++;
                continue;
            }

            double w = 1;
            int hiBin = hiEvt.hiBin;
            if (shiftHibin) {
                if (hiBin < 3) {
                    continue;
                }
                else {
                    hiBin -= 3;
                }
            }

            if (isMC) {
                bool skipEvtEle = false;
                bool skipEvtMu = false;
                bool skipEvtTau = false;
                bool skipEvtTauNu = false;
                for (int i = 0; i < hiGen.mult; ++i) {

                    if (std::fabs((*hiGen.pdg)[i]) == 11 && (*hiGen.pt)[i] > skipEvtElePt) {
                        skipEvtEle = true;
                        break;
                    }
                    if (std::fabs((*hiGen.pdg)[i]) == 13 && (*hiGen.pt)[i] > skipEvtMuPt) {
                        skipEvtMu = true;
                        break;
                    }
                    if (std::fabs((*hiGen.pdg)[i]) == 15 && (*hiGen.pt)[i] > skipEvtTauPt) {
                        skipEvtTau = true;
                        break;
                    }
                    if (std::fabs((*hiGen.pdg)[i]) == 16 && (*hiGen.pt)[i] > skipEvtTauNuPt) {
                        skipEvtTauNu = true;
                        break;
                    }
                }
                if (skipEvtEle) continue;
                if (skipEvtMu) continue;
                if (skipEvtTau) continue;
                if (skipEvtTauNu) continue;
            }

            entriesAnalyzed++;

            int cent = hiBin/2;

            if (isMC) {
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
                if (applyNcollWeights > 0 && isPbPb && isMC)  {
                    centWeight = findNcoll(hiBin);
                }
                w *= vertexWeight * centWeight;
            }

            for (int iCent = 0; iCent < nCents; ++iCent) {

                if (isPbPb && !(centsMin[iCent] <= cent && cent < centsMax[iCent]))  continue;

                wEvtTot[iCent] += w;
            }

            int iCent_phi = getBinCent4TrkW(hiBin, hiBinsMin_phi, nBinsCent_phi);
            int iCent_phieta = getBinCent4TrkW(hiBin, hiBinsMin_phieta, nBinsCent_phieta);

            for (int i = 0; i < trks.nTrk; ++i) {

                if (!passedTrkSelection(trks, i, collisionType))  continue;
                if (!(std::fabs(trks.trkEta[i]) < 2.4))  continue;
                if (skipMu > 0 && trks.pfType[i] == 3) continue;
                if (skipEle > 0 && trks.pfType[i] == 2) continue;

                float t_pt = trks.trkPt[i];
                float t_eta = trks.trkEta[i];
                float t_phi = trks.trkPhi[i];
                float t_absEta = std::fabs(t_eta);

                double trkWeightTmp = 1;
                double trkWeightEffTmp = 1;
                if (applyTrkWeights > 0) {
                    float effTmp = 1;
                    if (isPP17) {
                        trkWeightTmp = trkEff2017.getCorrection(t_pt, t_eta);
                        effTmp = trkEff2017.getEfficiency(t_pt, t_eta, true);
                        if (isMC) {
                            trkWeightTmp *= 0.979;
                            effTmp /= 0.979;
                        }
                    }
                    else if (isPbPb18) {
                        trkWeightTmp = trkEff2018.getCorrection(t_pt, t_eta, hiBin);
                        effTmp = trkEff2018.getEfficiency(t_pt, t_eta, hiBin, true);
                    }
                    trkWeightEffTmp = (effTmp > 0.001) ? (1.0)/effTmp : 0;
                }

                double wTrk = w * trkWeightTmp;
                double wTrkEff = w * trkWeightEffTmp;

                for (int iCent = 0; iCent < nCents; ++iCent) {

                    if (isPbPb && !(centsMin[iCent] <= cent && cent < centsMax[iCent]))  continue;

                    for (int iTrkEta = 0; iTrkEta < nTrkEtas; ++iTrkEta) {

                        if (!(trkEtasMin[iTrkEta] <= t_absEta && t_absEta < trkEtasMax[iTrkEta]))  continue;

                        h_pt[iCent][iTrkEta][RG::k_RecoEffCorr]->Fill(t_pt, wTrkEff);
                        h_pt[iCent][iTrkEta][RG::k_RecoCorr]->Fill(t_pt, wTrk);
                        h_pt[iCent][iTrkEta][RG::k_RecoUncorr]->Fill(t_pt, w);
                    }

                    for (int iTrkPt = 0; iTrkPt < nTrkPts; ++iTrkPt) {

                        if (!(trkPtsMin[iTrkPt] <= t_pt && t_pt < trkPtsMax[iTrkPt]))  continue;

                        h_eta[iCent][iTrkPt][RG::k_RecoEffCorr]->Fill(t_eta, wTrkEff);
                        h_eta[iCent][iTrkPt][RG::k_RecoCorr]->Fill(t_eta, wTrk);
                        h_eta[iCent][iTrkPt][RG::k_RecoUncorr]->Fill(t_eta, w);

                        h2_phi_vs_eta_var[iCent][iTrkPt][RG::k_RecoEffCorr]->Fill(t_eta, t_phi, wTrkEff);
                        h2_phi_vs_eta_var[iCent][iTrkPt][RG::k_RecoCorr]->Fill(t_eta, t_phi, wTrk);
                        h2_phi_vs_eta_var[iCent][iTrkPt][RG::k_RecoUncorr]->Fill(t_eta, t_phi, w);
                    }
                }

                if (do_phi) {
                    int iTrkPt = getBinPt4TrkW(t_pt, trkPts_phi, nBinsPt_phi);
                    int iTrkEta = getBinEta4TrkW(t_eta, trkEtasMin_phi, nBinsEta_phi);
                    if (iTrkPt >= 0 && iTrkEta >= 0 && iCent_phi >= 0) {
                        h_trkPhi[iTrkPt][iTrkEta][iCent_phi]->Fill(t_phi, wTrk);
                    }
                }

                if (do_phi_vs_eta) {
                    int iPt_phieta = getBinPt4TrkW(t_pt, pts_phieta, nBinsPt_phieta);
                    if (iPt_phieta >= 0 && iCent_phieta >= 0) {
                        h2_phi_vs_eta[iPt_phieta][iCent_phieta][RG::k_RecoEffCorr]->Fill(t_eta, t_phi, wTrkEff);
                        h2_phi_vs_eta[iPt_phieta][iCent_phieta][RG::k_RecoCorr]->Fill(t_eta, t_phi, wTrk);
                        h2_phi_vs_eta[iPt_phieta][iCent_phieta][RG::k_RecoUncorr]->Fill(t_eta, t_phi, w);
                    }
                }
            }

            if (isMC) {
                for (int i = 0; i < hiGen.mult; ++i) {

                    if (!(std::fabs((*hiGen.eta)[i]) < 2.4))  continue;
                    if ( ((*hiGen.chg)[i] == 0) )  continue;
                    if (skipMu > 0 && std::fabs((*hiGen.pdg)[i]) == 13) continue;
                    if (skipEle > 0 && std::fabs((*hiGen.pdg)[i]) == 11) continue;

                    for (int iCent = 0; iCent < nCents; ++iCent) {

                        if (isPbPb && !(centsMin[iCent] <= cent && cent < centsMax[iCent]))  continue;

                        for (int iTrkEta = 0; iTrkEta < nTrkEtas; ++iTrkEta) {

                            if (!(trkEtasMin[iTrkEta] <= std::fabs((*hiGen.eta)[i]) &&
                                                         std::fabs((*hiGen.eta)[i]) < trkEtasMax[iTrkEta]))  continue;

                            h_pt[iCent][iTrkEta][RG::k_Gen]->Fill((*hiGen.pt)[i], w);
                        }

                        for (int iTrkPt = 0; iTrkPt < nTrkPts; ++iTrkPt) {

                            if (!(trkPtsMin[iTrkPt] <= (*hiGen.pt)[i] &&
                                                       (*hiGen.pt)[i] < trkPtsMax[iTrkPt]))  continue;

                            h_eta[iCent][iTrkPt][RG::k_Gen]->Fill((*hiGen.eta)[i], w);

                            h2_phi_vs_eta_var[iCent][iTrkPt][RG::k_Gen]->Fill((*hiGen.eta)[i], (*hiGen.phi)[i], w);
                        }
                    }

                    if (do_phi_vs_eta) {
                        int iPt_phieta = getBinPt4TrkW((*hiGen.pt)[i], pts_phieta, nBinsPt_phieta);
                        if (iPt_phieta >= 0 && iCent_phieta >= 0) {
                            h2_phi_vs_eta[iPt_phieta][iCent_phieta][RG::k_Gen]->Fill((*hiGen.eta)[i], (*hiGen.phi)[i], w);
                        }
                    }
                }
            }

            if (anajets > 0) {

                int iCentJet = getBinCent4TrkW(hiBin, hiBinsMinJet, nBinsCentJet);
                if (iCentJet < 0)  continue;

                for (int i = 0; i < trks.nTrk; ++i) {

                     if (!passedTrkSelection(trks, i, collisionType))  continue;
                     if (!(std::fabs(trks.trkEta[i]) < 2.4))  continue;
                     if (skipMu > 0 && trks.pfType[i] == 3) continue;
                     if (skipEle > 0 && trks.pfType[i] == 2) continue;

                     double trkWeightTmp = 1;
                     double trkWeightEffTmp = 1;
                     if (applyTrkWeights > 0) {
                         if (isPP17) {
                             trkWeightTmp = trkEff2017.getCorrection(trks.trkPt[i], trks.trkEta[i]);
                         }
                         else if (isPbPb18) {
                             trkWeightTmp = trkEff2018.getCorrection(trks.trkPt[i], trks.trkEta[i], hiBin);
                             float effTmp = trkEff2018.getEfficiency(trks.trkPt[i], trks.trkEta[i], hiBin, true);
                             trkWeightEffTmp = (effTmp > 0.001) ? (1.0)/effTmp : 0;
                         }
                     }

                     double wTrk = w * trkWeightTmp;
                     double wTrkEff = w * trkWeightEffTmp;

                     double mindR2_jet_trk = 999999;
                     int iJet_mindR = -1;

                     for (int iJ = 0; iJ < jets.nref; ++iJ) {

                         if ( !(std::fabs(jets.jtpt[iJ]) > jetptMin) )  continue;

                         double dR2_jet_trk = getDR2(trks.trkEta[i], trks.trkPhi[i], jets.jteta[iJ], jets.jtphi[iJ]);

                         if (dR2_jet_trk < mindR2_jet_trk) {
                             mindR2_jet_trk = dR2_jet_trk;
                             iJet_mindR = iJ;
                         }
                     }

                     if (iJet_mindR >= 0) {

                         int iTrkPt = getBinPt4TrkW(trks.trkPt[i], trkPtsJet, nBinsTrkPtsJet);
                         int iTrkEta = getBinEta4TrkW(std::fabs(trks.trkEta[i]), absEtasJet, nBinsAbsEtasJet);

                         if (iTrkPt >= 0 && iTrkEta >= 0) {

                             double mindR_jet_trk = std::sqrt(mindR2_jet_trk);
                             h2_jetpt_vs_dR[iTrkPt][iTrkEta][iCentJet][RG::k_RecoEffCorr]->Fill(mindR_jet_trk, jets.jtpt[iJet_mindR], wTrkEff);
                             h2_jetpt_vs_dR[iTrkPt][iTrkEta][iCentJet][RG::k_RecoCorr]->Fill(mindR_jet_trk, jets.jtpt[iJet_mindR], wTrk);
                             h2_jetpt_vs_dR[iTrkPt][iTrkEta][iCentJet][RG::k_RecoUncorr]->Fill(mindR_jet_trk, jets.jtpt[iJet_mindR], w);

                             h2_jetpt_vs_dR_rebin[iTrkPt][iTrkEta][iCentJet][RG::k_RecoEffCorr]->Fill(mindR_jet_trk, jets.jtpt[iJet_mindR], wTrkEff);
                             h2_jetpt_vs_dR_rebin[iTrkPt][iTrkEta][iCentJet][RG::k_RecoCorr]->Fill(mindR_jet_trk, jets.jtpt[iJet_mindR], wTrk);
                             h2_jetpt_vs_dR_rebin[iTrkPt][iTrkEta][iCentJet][RG::k_RecoUncorr]->Fill(mindR_jet_trk, jets.jtpt[iJet_mindR], w);
                         }
                     }
                 }


                if (isMC) {

                    for (int i = 0; i < hiGen.mult; ++i) {

                        if (!(std::fabs((*hiGen.eta)[i]) < 2.4))  continue;
                        if ( ((*hiGen.chg)[i] == 0) )  continue;
                        if (skipMu > 0 && std::fabs((*hiGen.pdg)[i]) == 13) continue;
                        if (skipEle > 0 && std::fabs((*hiGen.pdg)[i]) == 11) continue;

                        double mindR2_jet_trk = 999999;
                        int iJet_mindR = -1;

                        for (int iJ = 0; iJ < jets.nref; ++iJ) {

                            if ( !(std::fabs(jets.jtpt[iJ]) > jetptMin) )  continue;

                            double dR2_jet_trk = getDR2((*hiGen.eta)[i], (*hiGen.phi)[i], jets.jteta[iJ], jets.jtphi[iJ]);

                            if (dR2_jet_trk < mindR2_jet_trk) {
                                mindR2_jet_trk = dR2_jet_trk;
                                iJet_mindR = iJ;
                            }
                        }

                        if (iJet_mindR >= 0) {

                            int iTrkPt = getBinPt4TrkW((*hiGen.pt)[i], trkPtsJet, nBinsTrkPtsJet);
                            int iTrkEta = getBinEta4TrkW(std::fabs((*hiGen.eta)[i]), absEtasJet, nBinsAbsEtasJet);

                            if (iTrkPt >= 0 && iTrkEta >= 0) {

                                double mindR_jet_trk = std::sqrt(mindR2_jet_trk);
                                h2_jetpt_vs_dR[iTrkPt][iTrkEta][iCentJet][RG::k_Gen]->Fill(mindR_jet_trk, jets.jtpt[iJet_mindR], w);

                                h2_jetpt_vs_dR_rebin[iTrkPt][iTrkEta][iCentJet][RG::k_Gen]->Fill(mindR_jet_trk, jets.jtpt[iJet_mindR], w);
                            }
                        }
                    }
                }
            }
        }
        fileTmp->Close();
    }
    std::cout<<  "Loop ENDED : " << treePathHiEvt.c_str() <<std::endl;
    std::cout << "nFilesSkipped = " << nFilesSkipped << std::endl;
    std::cout << "entries            = " << entries << std::endl;
    std::cout << "duplicateEntries   = " << duplicateEntries << std::endl;
    std::cout << "entriesAnalyzed    = " << entriesAnalyzed << std::endl;

    output->cd();
    std::cout << "### post loop processing - START ###" << std::endl;

    std::cout << "rebin" << std::endl;
    for (int i = 0; i < nCents; ++i) {
        for (int j = 0; j < nTrkEtas; ++j) {
            for (int iRG = 0; iRG < RG::kN_RG; ++iRG) {

                std::string tmpName;
                //double binW = h_pt[i][j][iRG]->GetBinWidth(1);
                std::vector<double> binsX;
                int nBinsX = 0;

                binsX = binsX_pt_rebin;
                nBinsX = binsX.size()-1;

                double arr_pt[nBinsX+1];
                std::copy(binsX.begin(), binsX.end(), arr_pt);

                tmpName = replaceAll(h_pt[i][j][iRG]->GetName(), "h_pt", "h_pt_rebin");
                h_pt_rebin[i][j][iRG] = (TH1D*)h_pt[i][j][iRG]->Rebin(nBinsX, tmpName.c_str(), arr_pt);
                h_pt_rebin[i][j][iRG]->Write("",TObject::kOverwrite);

                tmpName = replaceAll(tmpName.c_str(), "h_pt_rebin", "h_pt_rebin_normBinW");
                h_pt_rebin_normBinW[i][j][iRG] = (TH1D*)h_pt_rebin[i][j][iRG]->Clone(tmpName.c_str());
                h_pt_rebin_normBinW[i][j][iRG]->Scale(1, "width");
                h_pt_rebin_normBinW[i][j][iRG]->Write("",TObject::kOverwrite);
            }
        }
    }

    std::cout << "norm" << std::endl;
    for (int iRG = 0; iRG < RG::kN_RG; ++iRG) {
        for (int i = 0; i < nCents; ++i) {

            std::string name_tmp;
            for (int j = 0; j < nTrkEtas; ++j) {

                name_tmp = replaceFirst(h_pt[i][j][iRG]->GetName(), "h_", "h_normEvts_");
                hTmp = (TH1D*)h_pt[i][j][iRG]->Clone(name_tmp.c_str());
                hTmp->Scale(1./wEvtTot[i]);
                hTmp->Write("",TObject::kOverwrite);

                name_tmp = replaceFirst(h_pt_rebin[i][j][iRG]->GetName(), "h_", "h_normEvts_");
                hTmp = (TH1D*)h_pt_rebin[i][j][iRG]->Clone(name_tmp.c_str());
                hTmp->Scale(1./wEvtTot[i]);
                hTmp->Write("",TObject::kOverwrite);

                name_tmp = replaceFirst(h_pt_rebin_normBinW[i][j][iRG]->GetName(), "h_", "h_normEvts_");
                hTmp = (TH1D*)h_pt_rebin_normBinW[i][j][iRG]->Clone(name_tmp.c_str());
                hTmp->Scale(1./wEvtTot[i]);
                hTmp->Write("",TObject::kOverwrite);
            }

            for (int j = 0; j < nTrkPts; ++j) {

                name_tmp = replaceFirst(h_eta[i][j][iRG]->GetName(), "h_", "h_normEvts_");
                hTmp = (TH1D*)h_eta[i][j][iRG]->Clone(name_tmp.c_str());
                hTmp->Scale(1./wEvtTot[i]);
                hTmp->Write("",TObject::kOverwrite);

                name_tmp = replaceFirst(h2_phi_vs_eta_var[i][j][iRG]->GetName(), "h2_", "h2_normEvts_");
                hTmp = (TH2D*)h2_phi_vs_eta_var[i][j][iRG]->Clone(name_tmp.c_str());
                hTmp->Scale(1./wEvtTot[i]);
                hTmp->Write("",TObject::kOverwrite);
            }
        }
    }

    std::cout << "ratio" << std::endl;
    for (int iRG = 1; iRG < RG::kN_RG; ++iRG) {
        for (int i = 0; i < nCents; ++i) {

            int iGen = 0;
            std::string name_tmp;
            std::string str_ratio = Form("_ratio_%s_gen_", strRG[iRG].c_str());

            for (int j = 0; j < nTrkEtas; ++j) {

                name_tmp = replaceFirst(h_pt[i][j][iGen]->GetName(), "_gen_", str_ratio.c_str());
                hTmp = (TH1D*)h_pt[i][j][iRG]->Clone(name_tmp.c_str());
                hTmp->Divide(h_pt[i][j][iGen]);
                hTmp->Write("",TObject::kOverwrite);

                name_tmp = replaceFirst(name_tmp, "_ratio_", "_fullcorr_ratio_");
                hTmp->SetName(name_tmp.c_str());
                setBinErrorsFullCorr4Ratio((TH1D*)hTmp, h_pt[i][j][iRG], h_pt[i][j][iGen]);
                hTmp->Write("",TObject::kOverwrite);

                name_tmp = replaceFirst(h_pt_rebin[i][j][iGen]->GetName(), "_gen_", str_ratio.c_str());
                hTmp = (TH1D*)h_pt_rebin[i][j][iRG]->Clone(name_tmp.c_str());
                hTmp->Divide(h_pt_rebin[i][j][iGen]);
                hTmp->Write("",TObject::kOverwrite);

                name_tmp = replaceFirst(name_tmp, "_ratio_", "_fullcorr_ratio_");
                hTmp->SetName(name_tmp.c_str());
                setBinErrorsFullCorr4Ratio((TH1D*)hTmp, h_pt_rebin[i][j][iRG], h_pt_rebin[i][j][iGen]);
                hTmp->Write("",TObject::kOverwrite);
            }

            for (int j = 0; j < nTrkPts; ++j) {

                name_tmp = replaceFirst(h_eta[i][j][iGen]->GetName(), "_gen_", str_ratio.c_str());
                hTmp = (TH1D*)h_eta[i][j][iRG]->Clone(name_tmp.c_str());
                hTmp->Divide(h_eta[i][j][iGen]);
                hTmp->Write("",TObject::kOverwrite);

                name_tmp = replaceFirst(name_tmp, "_ratio_", "_fullcorr_ratio_");
                hTmp->SetName(name_tmp.c_str());
                setBinErrorsFullCorr4Ratio((TH1D*)hTmp, h_eta[i][j][iRG], h_eta[i][j][iGen]);
                hTmp->Write("",TObject::kOverwrite);

                name_tmp = replaceFirst(h2_phi_vs_eta_var[i][j][iGen]->GetName(), "_gen_", str_ratio.c_str());
                hTmp = (TH2D*)h2_phi_vs_eta_var[i][j][iRG]->Clone(name_tmp.c_str());
                hTmp->Divide(h2_phi_vs_eta_var[i][j][iGen]);
                hTmp->Write("",TObject::kOverwrite);
            }
        }
    }

    std::cout << "### post loop processing - END ###" << std::endl;

    std::cout<<"Writing the output file."<<std::endl;
    output->Write("",TObject::kOverwrite);
    std::cout<<"Closing the output file."<<std::endl;
    output->Close();
    std::cout<<"running trkSpectra() - END"<<std::endl;
}

/*
 * run the macro without going through event loop, things done before and after the loop
 */
void trkSpectraNoLoop(std::string configFile, std::string inputFile, std::string outputFile)
{
    std::cout<<"running trkSpectra()"<<std::endl;
    std::cout<<"configFile  = "<< configFile.c_str() <<std::endl;
    std::cout<<"inputFile   = "<< inputFile.c_str()  <<std::endl;
    std::cout<<"outputFile  = "<< outputFile.c_str() <<std::endl;

    TFile* input = TFile::Open(inputFile.c_str(), "READ");

    TFile* output = TFile::Open(outputFile.c_str(),"RECREATE");
    output->cd();

    std::cout<<"Closing the input file."<<std::endl;
    input->Close();
    std::cout<<"Closing the output file."<<std::endl;
    output->Close();
    std::cout<<"running trkSpectra() - END"<<std::endl;
}

int main(int argc, char** argv)
{
    std::vector<std::string> argStr = ArgumentParser::ParseParameters(argc, argv);
    int nArgStr = argStr.size();

    argOptions = ArgumentParser::ParseOptions(argc, argv);
    bool noLoop = (findPositionInVector(argOptions, ARGUMENTPARSER::noLoop) >= 0);

    if (nArgStr == 4) {
        if (noLoop) trkSpectraNoLoop(argStr.at(1), argStr.at(2), argStr.at(3));
        else        trkSpectra(argStr.at(1), argStr.at(2), argStr.at(3));
        return 0;
    }
    else if (nArgStr == 3) {
        if (noLoop) trkSpectraNoLoop(argStr.at(1), argStr.at(2));
        else        trkSpectra(argStr.at(1), argStr.at(2));
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./Histogramming/vJetTrk/trkSpectra.exe <configFile> <inputFile> <outputFile>"
                << std::endl;
        return 1;
    }
}
