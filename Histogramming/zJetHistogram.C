#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TCut.h>
#include <TH1D.h>
#include <TMath.h>
#include <TEventList.h>

#include <vector>
#include <string>
#include <iostream>

#include "../TreeHeaders/CutConfigurationTree.h"
#include "../Plotting/commonUtility.h"
#include "interface/correlationHist.h"
#include "../Utilities/eventUtil.h"
#include "../Utilities/interface/CutConfigurationParser.h"
#include "../Utilities/interface/InputConfigurationParser.h"

const std::vector<std::string> correlationHistNames   {"xjz", "dphi", "ptJet", "diEleM"};
const std::vector<std::string> correlationHistFormulas{"xjz", "abs(dphi)", "jtpt", "diEleM[zIdx]"};
const std::vector<std::string> correlationHistTitleX  {"p^{Jet}_{T}/p^{Z}_{T}", "#Delta#phi_{JZ}", "p^{Jet}_{T}", "M^{ee} (GeV/c^{2})"};
const std::vector<std::string> correlationHistTitleY_final_normalized{"#frac{1}{N_{Z}}#frac{dN_{JZ}}{dx_{JZ}}",
                                                                      "#frac{1}{N_{JZ}}#frac{dN_{JZ}}{d#Delta#phi}",
                                                                      "#frac{1}{N_{Z}}#frac{dN_{JZ}}{dp^{Jet}_{T}}",
                                                                      "Entries / (2 GeV/c^{2})"};
const std::vector<int>         nBinsx{40, 20,          300, 30};
const std::vector<double>      xlow  {0,  0,           0,   60};
const std::vector<double>      xup   {5,  TMath::Pi(), 300, 120};
const std::vector<double>      xlow_final{0,  0,           0,   60};
const std::vector<double>      xup_final {2,  TMath::Pi(), 150, 120};

void zJetHistogram(const TString configFile, const TString inputFile, const TString outputFile = "zJetHistogram.root");

void zJetHistogram(const TString configFile, const TString inputFile, const TString outputFile)
{
    std::cout<<"running zJetHistogram()"<<std::endl;
    std::cout<<"configFile  = "<< configFile.Data() <<std::endl;
    std::cout<<"inputFile   = "<< inputFile.Data() <<std::endl;
    std::cout<<"outputFile  = "<< outputFile.Data() <<std::endl;

    InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

    // input configuration
    int collision;
    if (configInput.isValid) {
        collision = configInput.proc[INPUT::kHISTOGRAM].i[INPUT::k_CollisionType];
    }
    else {
        collision = COLL::kPP;
    }
    // verbose about input configuration
    std::cout<<"Input Configuration :"<<std::endl;
    const char* collisionName =  getCollisionTypeName((COLL::TYPE)collision).c_str();
    std::cout << "collision = " << collisionName << std::endl;

    bool isMC = collisionIsMC((COLL::TYPE)collision);
    bool isHI = collisionIsHI((COLL::TYPE)collision);

    // observable bins
    std::vector<float> bins_pt[2];          // array of vectors for eta bins, each array element is a vector.
    std::vector<int>   bins_hiBin[2];       // array of vectors for hiBin bins, each array element is a vector.
    // Z Boson cuts
    std::string str_trigger;
    float eleSigmaIEtaIEta_2012_EB;
    float eledEtaAtVtx_abs_EB;
    float eledPhiAtVtx_abs_EB;
    float eleHoverE_EB;
    float eleEoverPInv_EB;
    float eleD0_abs_EB;
    float eleDz_abs_EB;
    int   eleMissHits_EB;
    float eleSigmaIEtaIEta_2012_EE;
    float eledEtaAtVtx_abs_EE;
    float eledPhiAtVtx_abs_EE;
    float eleHoverE_EE;
    float eleEoverPInv_EE;
    float eleD0_abs_EE;
    float eleDz_abs_EE;
    int   eleMissHits_EE;

    // jet cuts
    float cut_jetpt;
    float cut_jeteta;
    int   cut_jetID;
    // zJet cuts
    float cut_awayRange;
    float cut_dR;
    // process cuts
    int nEventsToMix;
    if (configCuts.isValid) {
        bins_pt[0] = ConfigurationParser::ParseListFloat(
                configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kZBOSON].s[CUTS::ZBO::k_bins_pt_gt]);
        bins_pt[1] = ConfigurationParser::ParseListFloat(
                configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kZBOSON].s[CUTS::ZBO::k_bins_pt_lt]);
        bins_hiBin[0] = ConfigurationParser::ParseListInteger(
                configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_gt]);
        bins_hiBin[1] = ConfigurationParser::ParseListInteger(
                configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_lt]);

        str_trigger = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].s[CUTS::ELE::k_trigger].c_str();

        // Barrel
        eleSigmaIEtaIEta_2012_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleSigmaIEtaIEta_2012_EB];
        eledEtaAtVtx_abs_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eledEtaAtVtx_abs_EB];
        eledPhiAtVtx_abs_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eledPhiAtVtx_abs_EB];
        eleHoverE_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleHoverE_EB];
        eleEoverPInv_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleEoverPInv_EB];
        eleD0_abs_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleD0_abs_EB];
        eleDz_abs_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleDz_abs_EB];
        eleMissHits_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].i[CUTS::ELE::k_eleMissHits_EB];

        // Endcap
        eleSigmaIEtaIEta_2012_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleSigmaIEtaIEta_2012_EE];
        eledEtaAtVtx_abs_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eledEtaAtVtx_abs_EE];
        eledPhiAtVtx_abs_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eledPhiAtVtx_abs_EE];
        eleHoverE_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleHoverE_EE];
        eleEoverPInv_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleEoverPInv_EE];
        eleD0_abs_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleD0_abs_EE];
        eleDz_abs_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleDz_abs_EE];
        eleMissHits_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].i[CUTS::ELE::k_eleMissHits_EE];

        cut_jetpt  = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kJET].f[CUTS::JET::k_pt];
        cut_jeteta = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kJET].f[CUTS::JET::k_eta];
        cut_jetID  = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kJET].i[CUTS::JET::k_jetID];

        cut_awayRange = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kZJET].f[CUTS::GJT::k_awayRange];
        cut_dR = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kZJET].f[CUTS::GJT::k_dR];

        nEventsToMix = configCuts.proc[CUTS::kSKIM].obj[CUTS::kZJET].i[CUTS::GJT::k_nEventsToMix];
    }
    else {  // default configuration
        bins_pt[0].push_back(60);
        bins_pt[1].push_back(999999);
        bins_hiBin[0].push_back(0);
        bins_hiBin[0].push_back(0);
        bins_hiBin[1].push_back(200);
        bins_hiBin[1].push_back(60);

        str_trigger = "HLT_HIDoublePhoton15_Eta2p5_Mass50_1000_R9SigmaHECut_v1";

        // Barrel
        eleSigmaIEtaIEta_2012_EB = 0.012;
        eledEtaAtVtx_abs_EB = 0.0126;
        eledPhiAtVtx_abs_EB = 0.107;
        eleHoverE_EB = 0.186;
        eleEoverPInv_EB = 0.239;
        eleD0_abs_EB = 0.0621;
        eleDz_abs_EB = 0.613;
        eleMissHits_EB = 2;

        // Endcap
        eleSigmaIEtaIEta_2012_EE = 0.0339;
        eledEtaAtVtx_abs_EE = 0.0109;
        eledPhiAtVtx_abs_EE = 0.219;
        eleHoverE_EE = 0.0962;
        eleEoverPInv_EE = 0.141;
        eleD0_abs_EE = 0.279;
        eleDz_abs_EE = 0.947;
        eleMissHits_EE = 3;

        cut_jetpt = 40;
        cut_jeteta = 1.6;
        cut_jetID = 0;      // jetID >= 0

        cut_awayRange = 2./3.;
        cut_dR = 0.4;

        nEventsToMix = 1;
    }
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

    std::cout<<"trigger    = "<<str_trigger.c_str()<<std::endl;

    std::cout<<"Barrel :"<<std::endl;
    std::cout<<"eleSigmaIEtaIEta_2012_EB = "<<eleSigmaIEtaIEta_2012_EB<<std::endl;
    std::cout<<"eledEtaAtVtx_abs_EB      = "<<eledEtaAtVtx_abs_EB<<std::endl;
    std::cout<<"eledPhiAtVtx_abs_EB      = "<<eledPhiAtVtx_abs_EB<<std::endl;
    std::cout<<"eleHoverE_EB             = "<<eleHoverE_EB<<std::endl;
    std::cout<<"eleEoverPInv_EB          = "<<eleEoverPInv_EB<<std::endl;
    std::cout<<"eleD0_abs_EB             = "<<eleD0_abs_EB<<std::endl;
    std::cout<<"eleDz_abs_EB             = "<<eleDz_abs_EB<<std::endl;
    std::cout<<"eleMissHits_EB           = "<<eleMissHits_EB<<std::endl;

    std::cout<<"Endcap :"<<std::endl;
    std::cout<<"eleSigmaIEtaIEta_2012_EE = "<<eleSigmaIEtaIEta_2012_EE<<std::endl;
    std::cout<<"eledEtaAtVtx_abs_EE      = "<<eledEtaAtVtx_abs_EE<<std::endl;
    std::cout<<"eledPhiAtVtx_abs_EE      = "<<eledPhiAtVtx_abs_EE<<std::endl;
    std::cout<<"eleHoverE_EE             = "<<eleHoverE_EE<<std::endl;
    std::cout<<"eleEoverPInv_EE          = "<<eleEoverPInv_EE<<std::endl;
    std::cout<<"eleD0_abs_EE             = "<<eleD0_abs_EE<<std::endl;
    std::cout<<"eleDz_abs_EE             = "<<eleDz_abs_EE<<std::endl;
    std::cout<<"eleMissHits_EE           = "<<eleMissHits_EE<<std::endl;

    std::cout<<"cut_jetpt                 = "<< cut_jetpt <<std::endl;
    std::cout<<"cut_jeteta                = "<< cut_jeteta <<std::endl;
    std::cout<<"cut_jetID                 = "<< cut_jetID <<std::endl;

    std::cout<<"cut_awayRange             = "<< cut_awayRange << " * PI" <<std::endl;
    std::cout<<"cut_dR                    = "<< cut_dR <<std::endl;

    std::cout<<"nEventsToMix              = "<< nEventsToMix <<std::endl;

    //set real awayRange cut
    cut_awayRange = cut_awayRange * TMath::Pi();

    TFile *input = new TFile(inputFile, "READ");
    TTree *tHlt = (TTree*)input->Get("hltTree");
    TTree *tdiEle = (TTree*)input->Get("dielectron");
    TTree *tJet = (TTree*)input->Get("jets");
    TTree *tzj  = (TTree*)input->Get("zJet");
    TTree *tJetMB;
    TTree *tzjMB;
    TTree *tHiEvt;

    // check the existence of HI specific trees in "zJetSkim.root" file
    bool hasJetsMB = false;
    bool hasZJetMB = false;
    if (isHI) {
        input->GetObject("jetsMB",tJetMB);
        input->GetObject("zJetMB",tzjMB);

        if (tJetMB) hasJetsMB = true;
        if (tzjMB)  hasZJetMB = true;
    }
    else {
        tJetMB = 0;
        tzjMB  = 0;
    }
    bool hasHiEvt = false;
    if (isHI || isMC) {
        input->GetObject("HiEvt",tHiEvt);

        if (tHiEvt) hasHiEvt = true;
    }
    else {
        tHiEvt = 0;
    }
    bool hasEventWeight = false;
    if (hasHiEvt) {
        if (tHiEvt->GetBranch("weight"))    hasEventWeight = true;
    }
    std::cout << "hasEventWeight = " << hasEventWeight <<std::endl;
    if (hasEventWeight)
        std::cout << "Events will be weighted, during drawing step selections will be multiplied with the weight" <<std::endl;

    tzj->AddFriend(tHlt, "Hlt");
    tzj->AddFriend(tdiEle, "diEle");
    tzj->AddFriend(tJet, "Jet");
    if (hasHiEvt) {
        tzj->AddFriend(tHiEvt, "HiEvt");
    }

    // relation of trees from MB mixing block
    if (hasJetsMB && hasZJetMB) {
        tzjMB->AddFriend(tHlt, "Hlt");
        tzjMB->AddFriend(tdiEle, "diEle");
        tzjMB->AddFriend(tJetMB, "Jet");
        if (hasHiEvt) {
            tzjMB->AddFriend(tHiEvt, "HiEvt");
        }
    }

    TFile* output = new TFile(outputFile, "UPDATE");
    // histograms will be put under a directory whose name is the type of the collision
    if(!output->GetKey(collisionName)) output->mkdir(collisionName, Form("input file is %s", inputFile.Data()));
    output->cd(collisionName);
    std::cout<<"histograms will be put under directory : " << collisionName << std::endl;

    TTree *configTree = setupConfigurationTreeForWriting(configCuts);

    // gammaJet analysis has purity calculation. no purity calculation for zJet.

    TH1::SetDefaultSumw2();
    int nCorrHist = correlationHistNames.size();
    correlationHist corrHists[nCorrHist][nBins_pt][nBins_hiBin];

    // prepare histogram names for xjg, abs(dphi) and jet pt
    // if the collision is not HI, then cannot split it into hiBins.
    if (!isHI) nBins_hiBin = 1;
    for (int iHist=0; iHist<nCorrHist; ++iHist){
    for (int i=0; i<nBins_pt; ++i){
        for(int j=0; j<nBins_hiBin; ++j){
            corrHists[iHist][i][j].name = Form("%s_ptBin%d_HiBin%d", correlationHistNames.at(iHist).c_str(), i, j);

            int iCorr = 0;
            for (int jCorr = 0; jCorr < CORR::kN_CORRFNC; ++jCorr) {

                std::string subHistName = Form("%s_ptBin%d_HiBin%d_%s", correlationHistNames.at(iHist).c_str(), i, j,
                        CORR::CORR_JET_LABELS[jCorr].c_str());
                corrHists[iHist][i][j].h1D_name[iCorr][jCorr] = subHistName.c_str();
                corrHists[iHist][i][j].h1D[iCorr][jCorr] = new TH1D(Form("h1D_%s", subHistName.c_str()),"",
                        nBinsx.at(iHist), xlow.at(iHist), xup.at(iHist));

                corrHists[iHist][i][j].h1D_titleX[iCorr][jCorr] = correlationHistTitleX.at(iHist).c_str();
                corrHists[iHist][i][j].h1D_titleY_final_norm[iCorr][jCorr] = correlationHistTitleY_final_normalized.at(iHist).c_str();
            }
        }
    }
    }

    // histograms with pt bins on x-axis
    std::vector<std::string> correlationHistNames_ptBinAll = {"rjz", "xjz_mean"};       // histograms where x-axis is pt bins
    int nCorrHist_ptBinAll = correlationHistNames_ptBinAll.size();
    correlationHist corrHists_ptBinAll[nCorrHist_ptBinAll][nBins_hiBin];
    // prepare histogram names for rjz and <xjz>
    const int nBins_rjz = 4;
    const int nBins_xjz_mean= 4;
    double bins_rjz[nBins_rjz+1] =           {40, 50, 60, 80, 120};
    double bins_xjz_mean[nBins_xjz_mean+1] = {40, 50, 60, 80, 120};

    for (int j=0; j<nBins_hiBin; ++j){
        int iCorr = 0;
        for (int jCorr = 0; jCorr < CORR::kN_CORRFNC; ++jCorr) {

            // rjz
            std::string subHistName;
            subHistName = Form("%s_ptBinAll_HiBin%d_%s", correlationHistNames_ptBinAll.at(0).c_str(), j,
                    CORR::CORR_JET_LABELS[jCorr].c_str());
            corrHists_ptBinAll[0][j].h1D_name[iCorr][jCorr] = subHistName.c_str();
            corrHists_ptBinAll[0][j].h1D[iCorr][jCorr] = new TH1D(Form("h1D_%s", subHistName.c_str()), "",nBins_rjz, bins_rjz);

            // x_jz_mean
            subHistName = Form("%s_ptBinAll_HiBin%d_%s", correlationHistNames_ptBinAll.at(1).c_str(), j,
                    CORR::CORR_JET_LABELS[jCorr].c_str());
            corrHists_ptBinAll[1][j].h1D_name[iCorr][jCorr] = subHistName.c_str();
            corrHists_ptBinAll[1][j].h1D[iCorr][jCorr] = new TH1D(Form("h1D_%s", subHistName.c_str()), "",nBins_xjz_mean, bins_xjz_mean);
        }
    }

    // selections for different signal regions that are applied to every histogram
    // selections for dielectron regions
    TCut selectionDiele =  "";
    TCut selection_EB = "";
    TCut selection_EE = "";
    selection_EB = selection_EB && Form("eleSigmaIEtaIEta_2012_1[zIdx] < %f && eleSigmaIEtaIEta_2012_2[zIdx] < %f", eleSigmaIEtaIEta_2012_EB, eleSigmaIEtaIEta_2012_EB);
    selection_EB = selection_EB && Form("abs(eledEtaAtVtx_1[zIdx]) < %f && abs(eledEtaAtVtx_2[zIdx]) < %f", eledEtaAtVtx_abs_EB, eledEtaAtVtx_abs_EB);
    selection_EB = selection_EB && Form("abs(eledPhiAtVtx_1[zIdx]) < %f && abs(eledPhiAtVtx_2[zIdx]) < %f", eledPhiAtVtx_abs_EB, eledPhiAtVtx_abs_EB);
    selection_EB = selection_EB && Form("eleHoverE_1[zIdx] < %f && eleHoverE_2[zIdx] < %f", eleHoverE_EB, eleHoverE_EB);
    selection_EB = selection_EB && Form("eleEoverPInv_1[zIdx] < %f && eleEoverPInv_2[zIdx] < %f", eleEoverPInv_EB, eleEoverPInv_EB);
    selection_EB = selection_EB && Form("abs(eleD0_1[zIdx]) < %f && abs(eleD0_2[zIdx]) < %f", eleD0_abs_EB, eleD0_abs_EB);
    selection_EB = selection_EB && Form("abs(eleDz_1[zIdx]) < %f && abs(eleDz_2[zIdx]) < %f", eleDz_abs_EB, eleDz_abs_EB);
    selection_EB = selection_EB && Form("eleMissHits_1[zIdx] <= %d && eleMissHits_2[zIdx] <= %d", eleMissHits_EB, eleMissHits_EB);

    selection_EE = selection_EE && Form("eleSigmaIEtaIEta_2012_1[zIdx] < %f && eleSigmaIEtaIEta_2012_2[zIdx] < %f", eleSigmaIEtaIEta_2012_EE, eleSigmaIEtaIEta_2012_EE);
    selection_EE = selection_EE && Form("abs(eledEtaAtVtx_1[zIdx]) < %f && abs(eledEtaAtVtx_2[zIdx]) < %f", eledEtaAtVtx_abs_EE, eledEtaAtVtx_abs_EE);
    selection_EE = selection_EE && Form("abs(eledPhiAtVtx_1[zIdx]) < %f && abs(eledPhiAtVtx_2[zIdx]) < %f", eledPhiAtVtx_abs_EE, eledPhiAtVtx_abs_EE);
    selection_EE = selection_EE && Form("eleHoverE_1[zIdx] < %f && eleHoverE_2[zIdx] < %f", eleHoverE_EE, eleHoverE_EE);
    selection_EE = selection_EE && Form("eleEoverPInv_1[zIdx] < %f && eleEoverPInv_2[zIdx] < %f", eleEoverPInv_EE, eleEoverPInv_EE);
    selection_EE = selection_EE && Form("abs(eleD0_1[zIdx]) < %f && abs(eleD0_2[zIdx]) < %f", eleD0_abs_EE, eleD0_abs_EE);
    selection_EE = selection_EE && Form("abs(eleDz_1[zIdx]) < %f && abs(eleDz_2[zIdx]) < %f", eleDz_abs_EE, eleDz_abs_EE);
    selection_EE = selection_EE && Form("eleMissHits_1[zIdx] <= %d && eleMissHits_2[zIdx] <= %d", eleMissHits_EE, eleMissHits_EE);

    TCut selection_EB_eta = Form(" abs(eleEta_1[zIdx]) < %f && abs(eleEta_2[zIdx]) < %f", 1.4791, 1.4791);
    selection_EB = selection_EB && selection_EB_eta;
    TCut selection_EE_eta = Form(" abs(eleEta_1[zIdx]) >= %f && abs(eleEta_2[zIdx]) >= %f", 1.4791, 1.4791);
    selection_EE_eta = selection_EE_eta && Form(" abs(eleEta_1[zIdx]) < %f && abs(eleEta_2[zIdx]) < %f", 2.4, 2.4);
    selection_EE = selection_EE && selection_EE_eta;
    TCut selection_EB_EE = selection_EB || selection_EE;
    selectionDiele = selectionDiele && selection_EB_EE;

    if (isMC) {
        selectionDiele = selectionDiele && "1 == 1";    // gen particle specific selection
    }

    // selection for jet regions
    // jet from bkg region are already separated from raw region.
    // no additional selection for jets. just use different trees.
    std::cout<<"####################"<<std::endl;
    std::cout<<"tzj->GetEntries() = "<<tzj->GetEntries()<<std::endl;
    if (str_trigger.compare("") != 0 && !isMC) {
        std::cout<<"tzj->GetEntries(trigger==1) = "<<tzj->GetEntries(Form("%s == 1",str_trigger.c_str()))<<std::endl;
    }
    else {
        std::cout<<"tzj->GetEntries(trigger==1) is skipped because either no trigger is specified or the data is coming from MC."<<std::endl;
    }
    std::cout<<"####################"<<std::endl;

    // use "EventList" approach to make the "draw()" shorter
    // store original eventlist
    const char* elist_tmp_name="elist_tmp";
    tzj->Draw(Form(">> %s", elist_tmp_name));
    TEventList* eventlist = ((TEventList*)gDirectory->Get(elist_tmp_name)->Clone("elist_Original"));
    gDirectory->Delete(elist_tmp_name);

    int nCorrZ = CORR::kRAW + 1;
    std::string eventlistNames[nCorrZ][nBins_pt][nBins_hiBin];
    TEventList* elists[nCorrZ][nBins_pt][nBins_hiBin];
    bool isEventlistSet[nBins_pt][nBins_hiBin];     // event lists are independent of the correlation histogram.
    // set eventlist once for a given bin, then reuse it for subsequent correlations, do not recalculate.
    for(int i=0; i<nBins_pt; ++i){
        for(int j=0; j<nBins_hiBin; ++j){
            eventlistNames[CORR::kRAW][i][j] = Form("eventlist_ptBin%d_HiBin%d", i, j);  // CORR::kRAW = 0
            isEventlistSet[i][j] = false;
        }
    }

    // HISTOGRAMMING BLOCK
    TCanvas* c = new TCanvas("cnv","",600,600);
    for(int iHist = 0; iHist<nCorrHist; ++iHist)
    {
    std::cout<<"####################"<<std::endl;
    std::cout<<"histogramming : "<<correlationHistNames.at(iHist).c_str()<<std::endl;
    for(int i=0; i<nBins_pt; ++i){
        for(int j=0; j<nBins_hiBin; ++j){

            std::cout<<"##########"<<std::endl;
            std::cout<<Form("histogramming : ptBin%d HiBin%d", i, j)<<std::endl;

            if(j>0 && !isHI && !hasHiEvt) continue;

            // event selection
            TCut selection_event = Form("%s == 1", str_trigger.c_str());
            if (isMC) selection_event = "1==1";
            if (isHI) {
                selection_event = selection_event && Form("hiBin >= %d && hiBin < %d", bins_hiBin[0].at(j), bins_hiBin[1].at(j));
            }

            // Z cuts were applied in the analysis code
            // Z selection
            TCut selectionZ;
            if (bins_pt[1].at(i) >= 0)  selectionZ = Form("diElePt[zIdx] >= %f && diElePt[zIdx] < %f", bins_pt[0].at(i), bins_pt[1].at(i));
            else                        selectionZ = Form("diElePt[zIdx] >= %f", bins_pt[0].at(i));
            selectionZ = selectionZ && selectionDiele;

            // jet selection
            TCut selectionJet = "";
            // special selection
            if (correlationHistNames.at(iHist).compare("dphi") != 0) {  // no awayRange cut for dphi histograms
                selectionJet = selectionJet && Form("abs(dphi) > %f ", cut_awayRange);
            }
            selectionJet = selectionJet && Form("dR_ele_1 >= %f", cut_dR);
            selectionJet = selectionJet && Form("dR_ele_2 >= %f", cut_dR);
            selectionJet = selectionJet && Form("jtpt > %f", cut_jetpt);
            selectionJet = selectionJet && Form("abs(jteta) < %f", cut_jeteta);
            selectionJet = selectionJet && Form("jetID >= %d", cut_jetID);

            TCut selection_Barrel = "1";    // no extra selection at the moment
            TCut selection_Endcap = "1";    // no extra selection at the moment

            TCut selection = "";
            selection = selection && selection_event;
            selection = selection && selectionZ;
            selection = selection && selectionJet;

            for (int jCorr = 0; jCorr < CORR::kN_CORRFNC; ++jCorr) {
                    if (jCorr > CORR::kRAW && !isHI)  continue;      // no jet background for non-HI
                    corrHists[iHist][i][j].selections[CORR::kRAW][jCorr] = selection;
            }

            std::cout<< "corrHists[iHist][i][j].h1D[CORR::kRAW][CORR::kRAW]->GetName() = " <<  corrHists[iHist][i][j].h1D[CORR::kRAW][CORR::kRAW]->GetName() <<std::endl;
            std::cout<< "selections[CORR::kRAW][CORR::kRAW] = " << corrHists[iHist][i][j].selections[CORR::kRAW][CORR::kRAW].GetTitle() <<std::endl;

            // use "EventList" approach to make the "draw()" shorter
            if (!isEventlistSet[i][j]) {
                tzj->Draw(Form(">> %s", eventlistNames[CORR::kRAW][i][j].c_str()), (selection_event && selectionZ).GetTitle());
                elists[CORR::kRAW][i][j] = (TEventList*)gDirectory->Get(eventlistNames[CORR::kRAW][i][j].c_str());
                isEventlistSet[i][j] = true;
                // event lists are independent of the correlation histogram.
                // set eventlist once for a given bin, then reuse it for subsequent correlations, do not recalculate.
            }

            // number of events with Z bosons, not necessarily z-jet events
            tzj->SetEventList(elists[CORR::kRAW][i][j]);
            corrHists[iHist][i][j].nEntriesPho[CORR::kRAW][CORR::kRAW] = tzj->GetEventList()->GetN();
            // nEntriesPho[][CORR::kRAW] = nEntriesPho[][CORR::kBKG] by definition
            // so no calculation for nEntriesPho[][CORR::kBKG]
            corrHists[iHist][i][j].nEntriesPho[CORR::kRAW][CORR::kBKG] = corrHists[iHist][i][j].nEntriesPho[CORR::kRAW][CORR::kRAW];

            tzj->SetEventList(elists[CORR::kRAW][i][j]);
            corrHists[iHist][i][j].nEntries[CORR::kRAW][CORR::kRAW] = tzj->GetEntries(corrHists[iHist][i][j].selections[CORR::kRAW][CORR::kRAW].GetTitle());

            std::cout<< "nEntries[CORR::kRAW][CORR::kRAW] = " << corrHists[iHist][i][j].nEntries[CORR::kRAW][CORR::kRAW] <<std::endl;
            std::cout<< "nEntriesPho[CORR::kRAW][CORR::kRAW] = " << corrHists[iHist][i][j].nEntriesPho[CORR::kRAW][CORR::kRAW] <<std::endl;

            std::string histoTitle;
            if (isHI) {
              histoTitle = Form("%s , %.0f < p^{Z}_{T} < %.0f GeV/c, %d-%d %% ",collisionName , bins_pt[0].at(i), bins_pt[1].at(i), bins_hiBin[0].at(j)/2, bins_hiBin[1].at(j)/2);

              // special cases
              if (bins_pt[0].at(i) <= 0 && bins_pt[1].at(i) < 0 && bins_hiBin[0].at(j) <= 0 && bins_hiBin[1].at(j) >= 200 )   {
                  histoTitle = Form("%s",collisionName );
              }
              else if (bins_pt[0].at(i) <= 0 && bins_pt[1].at(i) < 0)   {
                  histoTitle = Form("%s , %d-%d %%",collisionName , bins_hiBin[0].at(j)/2, bins_hiBin[1].at(j)/2);
              }
              else if (bins_hiBin[0].at(j) <= 0 && bins_hiBin[1].at(j) >= 200)   {
                  histoTitle = Form("%s , %.0f < p^{Z}_{T} < %.0f GeV/c",collisionName , bins_pt[0].at(i), bins_pt[1].at(i));
              }
              else if (bins_pt[1].at(i) < 0) {
                  histoTitle = Form("%s , p^{Z}_{T} > %.0f GeV/c, %d-%d %% ",collisionName , bins_pt[0].at(i), bins_hiBin[0].at(j)/2, bins_hiBin[1].at(j)/2);
              }
            }
            else {
                histoTitle = Form("%s , %.0f < p^{Z}_{T} < %.0f GeV/c",collisionName , bins_pt[0].at(i), bins_pt[1].at(i));

                // special cases
                if (bins_pt[0].at(i) <= 0 && bins_pt[1].at(i) < 0)   {
                    histoTitle = Form("%s",collisionName );
                }
                else if (bins_pt[1].at(i) < 0) {
                    histoTitle = Form("%s , p^{Z}_{T} > %.0f GeV/c", collisionName , bins_pt[0].at(i));
                }
            }

            // histograms for RAW and BKG regions
            int iCorr = 0;
            for (int jCorr = 0; jCorr < CORR::kN_CORRFNC -1; ++jCorr) {

                if (jCorr == CORR::kBKG && !isHI)  continue;      // no jet background for non-HI

                std::string titleX = corrHists[iHist][i][j].h1D_titleX[iCorr][jCorr].c_str();
                corrHists[iHist][i][j].h1D[iCorr][jCorr]->SetTitle(Form("%s;%s;%s",histoTitle.c_str(), titleX.c_str(), "Entries"));
                corrHists[iHist][i][j].h1D[iCorr][jCorr]->SetMarkerStyle(kFullCircle);
                corrHists[iHist][i][j].h1D[iCorr][jCorr]->SetMarkerColor(kBlack);

                // histogram name excluding the "h1D" prefix
                std::string tmpH1D_name = corrHists[iHist][i][j].h1D_name[iCorr][jCorr].c_str();
                TCut selectionDraw = corrHists[iHist][i][j].selections[iCorr][jCorr].GetTitle();
                if (hasEventWeight)  selectionDraw = Form("(HiEvt.weight)*(%s)", selectionDraw.GetTitle());

                std::string tmpHistName = corrHists[iHist][i][j].h1D[iCorr][jCorr]->GetName();
                std::string tmpFormula = correlationHistFormulas.at(iHist).c_str();

                c->SetName(Form("cnv_%s",tmpH1D_name.c_str()));
                c->cd();
                if (jCorr == CORR::kRAW){
                    tzj->SetEventList(elists[iCorr][i][j]);
                    tzj->Draw(Form("%s >> %s", tmpFormula.c_str(), tmpHistName.c_str()), selectionDraw.GetTitle(),"goff");
                }
                if (jCorr == CORR::kBKG && hasJetsMB && hasZJetMB) {
                    tzjMB->SetEventList(elists[iCorr][i][j]);
                    tzjMB->Draw(Form("%s >> %s", tmpFormula.c_str(), tmpHistName.c_str()), selectionDraw.GetTitle(),"goff");
                }

                corrHists[iHist][i][j].h1D[iCorr][jCorr]->Draw("e");
                corrHists[iHist][i][j].h1D[iCorr][jCorr]->Write("",TObject::kOverwrite);
                corrHists[iHist][i][j].h1D[iCorr][jCorr]->SetStats(false);     // remove stat box from the canvas, but keep in the histograms.
                c->Write("",TObject::kOverwrite);
                c->Clear();

                // FINAL
                c->SetName(Form("cnv_%s_final",tmpH1D_name.c_str()));
                c->cd();
                corrHists[iHist][i][j].h1D_final[iCorr][jCorr] =
                        (TH1D*)corrHists[iHist][i][j].h1D[iCorr][jCorr]->Clone(Form("%s_final", tmpHistName.c_str()));
                double tmpXlow = xlow_final.at(iHist);
                double tmpXup  = xup_final.at(iHist);
                corrHists[iHist][i][j].h1D_final[iCorr][jCorr]->SetAxisRange(tmpXlow, tmpXup);
                corrHists[iHist][i][j].h1D_final[iCorr][jCorr]->Draw("e");
                corrHists[iHist][i][j].h1D_final[iCorr][jCorr]->Write("",TObject::kOverwrite);
                corrHists[iHist][i][j].h1D_final[iCorr][jCorr]->SetStats(false);  // remove stat box from the canvas, but keep in the histograms.
                c->Write("",TObject::kOverwrite);
                c->Clear();

                // FINAL_NORM
                c->SetName(Form("cnv_%s_final_norm",tmpH1D_name.c_str()));
                c->cd();
                corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr] =
                        (TH1D*)corrHists[iHist][i][j].h1D_final[iCorr][jCorr]->Clone(Form("%s_final_norm", tmpHistName.c_str()));
                if (jCorr == CORR::kBKG && hasJetsMB && hasZJetMB) {   // (hasJetsMB && hasZJetMB) ==> isHI
                    // normalize first by the number of mixed events
                    corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->Scale(1./nEventsToMix);
                }
                int tmpNEntriesZ = corrHists[iHist][i][j].nEntriesPho[iCorr][jCorr];
                // normalization is done with Z boson events, not necessarily by Z-jet events
                // so the integral of the normalized histogram is R_jg.
                corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->Scale(1./tmpNEntriesZ);
                std::string tmpTitleY_final_norm = corrHists[iHist][i][j].h1D_titleY_final_norm[iCorr][jCorr].c_str();
                corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->GetYaxis()->SetTitle(tmpTitleY_final_norm.c_str());
                corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->Draw("e");
                corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->Write("",TObject::kOverwrite);
                c->Write("",TObject::kOverwrite);
                c->Clear();
            }

            std::cout<<"making histograms for SIG regions"<<std::endl;
            // calculate SIGSIG histogram,
            // these histograms are ignored : SIGRAW, SIGBKG

            // no background for PP and PA
            if(!isHI){
                // for non-HI, reset jet BKG histograms
                // in that case BKG histograms are not used, but set to empty histograms.
                // so it becomes SIG = RAW
                std::cout<<"collision is "<< collisionName <<std::endl;
                std::cout<<"contribution from BKG region is set to zero."<< std::endl;

                std::string tmpHistName;

                // reset RAWBKG
                tmpHistName = Form("%s_final_norm", corrHists[iHist][i][j].h1D[CORR::kRAW][CORR::kBKG]->GetName());
                corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kBKG] =
                        (TH1D*)corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kRAW]->Clone(tmpHistName.c_str());
                corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kBKG]->Reset();

                // there is no BKGBKG
            }

            // first subtract jet BKG, there is no Z BKG
            // subtract jet BKG
            // RAWSIG = RAWRAW - RAWBKG
            std::string tmpHistNameRAWSIG = Form("%s_final_norm", corrHists[iHist][i][j].h1D[CORR::kRAW][CORR::kSIG]->GetName());
            std::cout<<tmpHistNameRAWSIG.c_str()<<std::endl;
            corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kSIG] =
                                 (TH1D*)corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kRAW]->Clone(tmpHistNameRAWSIG.c_str());
            corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kSIG]->Add(corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kBKG],-1);

            // there is no BKGRAW, BKGBKG, BKGSIG

            // there is no Z BKG

            // FINAL_NORM  RAWSIG
            std::string tmpH1D_nameRAWSIG = corrHists[iHist][i][j].h1D_name[CORR::kRAW][CORR::kSIG].c_str();
            std::cout<<"drawing tmpH1D_nameRAWSIG = "<<tmpH1D_nameRAWSIG.c_str()<<std::endl;
            c->SetName(Form("cnv_%s_final_norm",tmpH1D_nameRAWSIG.c_str()));
            c->cd();
            corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kSIG]->Draw("e");
            corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kSIG]->Write("",TObject::kOverwrite);
            corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kSIG]->SetStats(false);  // remove stat box from the canvas, but keep in the histograms.
            c->Write("",TObject::kOverwrite);
            c->Clear();

            // there is no FINAL_NORM  BKGSIG
            std::cout<<"there is no tmpH1D_nameBKGSIG "<<std::endl;

            // FINAL_NORM  SIGSIG
            std::cout<<"there is no tmpH1D_nameSIGSIG "<<std::endl;

            tzj->SetEventList(eventlist);      // restore the original event list after making the histograms
            if (hasJetsMB && hasZJetMB) {
                tzjMB->SetEventList(eventlist);
            }

            std::cout<<Form("histogramming END : ptBin%d HiBin%d", i, j)<<std::endl;
            std::cout<<"##########"<<std::endl;
        }
    }
    std::cout<<"histogramming END : "<<correlationHistNames.at(iHist).c_str()<<std::endl;
    std::cout<<"####################"<<std::endl;
    }

    // histograms with pt bins on x-axis
    // corrHists_ptBinAll[0][]  = R_jz
    // corrHists_ptBinAll[1][]  = <X_jz>
    std::cout<<"####################"<<std::endl;
    for(int j=0; j<nBins_hiBin; ++j){
        int iCorr=0;
        for(int jCorr=0; jCorr<CORR::kN_CORRFNC; ++jCorr) {

            if(j>0 && !isHI) continue;

            int offset = 3; // ptBin 40-50 starts from index 3.
            // rjg block
            for(int i=0; i<nBins_rjz; ++i){

                double err;
                double val = corrHists[0][i+offset][j].h1D_final_norm[iCorr][jCorr]->IntegralAndError(1,
                        corrHists[0][i+offset][j].h1D_final_norm[iCorr][jCorr]->GetNbinsX(), err);

                corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->SetBinContent(i+1, val);
                corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->SetBinError(i+1, err);
            }

            std::string histoTitle = Form("%s , %d-%d %%",collisionName , bins_hiBin[0].at(j)/2, bins_hiBin[1].at(j)/2);

            corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->SetTitle(Form("%s;p^{Z}_{T} (GeV/c); R_{JZ}",histoTitle.c_str()));
            corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->SetMarkerStyle(kFullCircle);
            corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->SetMarkerColor(kBlack);

            std::cout<<"drawing : "<<corrHists_ptBinAll[0][j].h1D_name[iCorr][jCorr].c_str()<<std::endl;
            c->SetName(Form("cnv_%s",corrHists_ptBinAll[0][j].h1D_name[iCorr][jCorr].c_str()));
            c->cd();
            corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->Draw("e");
            corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->Write("",TObject::kOverwrite);
            corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->SetStats(false);     // remove stat box from the canvas, but keep in the histograms.
            c->Write("",TObject::kOverwrite);
            c->Clear();

            // xjz_mean block
            for(int i=0; i<nBins_xjz_mean; ++i){

                double val = corrHists[0][i+offset][j].h1D_final_norm[iCorr][jCorr]->GetMean();
                double err = corrHists[0][i+offset][j].h1D_final_norm[iCorr][jCorr]->GetMeanError();

                corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->SetBinContent(i+1, val);
                corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->SetBinError(i+1, err);
            }

            corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->SetTitle(Form("%s;p^{Z}_{T} (GeV/c); <x_{JZ}>",histoTitle.c_str()));
            corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->SetMarkerStyle(kFullCircle);
            corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->SetMarkerColor(kBlack);

            std::cout<<"drawing : "<<corrHists_ptBinAll[1][j].h1D_name[iCorr][jCorr].c_str()<<std::endl;
            c->SetName(Form("cnv_%s",corrHists_ptBinAll[1][j].h1D_name[iCorr][jCorr].c_str()));
            c->cd();
            corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->Draw("e");
            corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->Write("",TObject::kOverwrite);
            corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->SetStats(false);     // remove stat box from the canvas, but keep in the histograms.
            c->Write("",TObject::kOverwrite);
            c->Clear();
        }
    }
    std::cout<<"####################"<<std::endl;

    eventlist->Delete();
    for(int i=0; i<nBins_pt; ++i){
        for(int j=0; j<nBins_hiBin; ++j){
            elists[CORR::kRAW][i][j]->Delete();
        }
    }

    configTree->Write("",TObject::kOverwrite);

    output->Write("",TObject::kOverwrite);
    input->Close();
    output->Close();
}

int main(int argc, char** argv)
{
    if (argc == 4) {
        zJetHistogram(argv[1], argv[2], argv[3]);
        return 0;
    }
    else if (argc == 3) {
        zJetHistogram(argv[1], argv[2]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./zJetHistogram.exe <configFile> <inputFile> <outputFile>"
                << std::endl;
        return 1;
    }
}
