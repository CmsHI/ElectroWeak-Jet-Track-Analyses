#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TMath.h>
#include <TRandom3.h>

#include <vector>
#include <string>
#include <iostream>

#include "interface/correlationHist.h"
#include "../TreeHeaders/CutConfigurationTree.h"
#include "../TreeHeaders/JetTree.h"
#include "../TreeHeaders/gammaJetTree.h"
#include "../TreeHeaders/hiEvtTree.h"
#include "../TreeHeaders/ggHiNtuplizerTree.h"
#include "../Utilities/physicsUtil.h"
#include "../Utilities/eventUtil.h"
#include "../Corrections/jets/jetCorrector.h"
#include "../Utilities/interface/CutConfigurationParser.h"
#include "../Utilities/interface/InputConfigurationParser.h"

const std::vector<std::string> correlationHistNames {
    "xjg", "dphi", "ptJet"
};
const std::vector<std::string> correlationHistTitleX {
    "p^{Jet}_{T}/p^{#gamma}_{T}", "#Delta#phi_{J#gamma}", "p^{Jet}_{T}"
};
const std::vector<std::string> correlationHistTitleY_final_normalized {
    "#frac{1}{N_{#gamma}} #frac{dN_{J#gamma}}{dx_{J#gamma}}",
    "#frac{1}{N_{#gamma}} #frac{dN_{J#gamma}}{d#Delta#phi}",
    "#frac{1}{N_{#gamma}} #frac{dN_{J#gamma}}{dp^{Jet}_{T}}"
};
const std::vector<int>         nBinsx {16, 20,          30};
const std::vector<double>      xlow   {0,  0,           0};
const std::vector<double>      xup    {2,  TMath::Pi(), 300};

int getResolutionBin(int hiBin);
int getResolutionBinPP(int smearBin);

int gammaJetHistogram(const TString configFile, const TString inputFile, const TString outputFile, const int nJobs = -1, const int jobNum = -1, const float variation = 1);

int gammaJetHistogram(const TString configFile, const TString inputFile, const TString outputFile, const int nJobs, const int jobNum, const float variation) {
    TH1::SetDefaultSumw2();

    std::cout << "running gammaJetHistogram()" << std::endl;
    std::cout << "configFile  = " << configFile.Data() << std::endl;
    std::cout << "inputFile   = " << inputFile.Data() << std::endl;
    std::cout << "outputFile  = " << outputFile.Data() << std::endl;

    InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

    // input configuration
    if (!configInput.isValid) {
        std::cout << "Invalid input configuration" << std::endl;
        return 1;
    } else if (!configCuts.isValid) {
        std::cout << "Invalid cut configuration" << std::endl;
        return 1;
    }

    // input configuration
    // verbose about input configuration
    std::cout << "Input Configuration :" << std::endl;

    const int collision = configInput.proc[INPUT::kHISTOGRAM].i[INPUT::k_collisionType];
    const char* collisionName = getCollisionTypeName((COLL::TYPE)collision).c_str();
    std::cout << "collision = " << collisionName << std::endl;

    const bool isMC = collisionIsMC((COLL::TYPE)collision);
    const bool isHI = collisionIsHI((COLL::TYPE)collision);

    // observable bins
    std::vector<float> bins_pt[2];          // array of vectors for eta bins, each array element is a vector.
    std::vector<int>   bins_hiBin[2];       // array of vectors for hiBin bins, each array element is a vector.

    bins_pt[0] = ConfigurationParser::ParseListFloat(configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_pt_gt]);
    bins_pt[1] = ConfigurationParser::ParseListFloat(configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_pt_lt]);
    bins_hiBin[0] = ConfigurationParser::ParseListInteger(configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_gt]);
    bins_hiBin[1] = ConfigurationParser::ParseListInteger(configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_lt]);

    const int nBins_pt = bins_pt[0].size();         // assume <myvector>[0] and <myvector>[1] have the same size.
    const int nBins_hiBin = bins_hiBin[0].size();     // assume <myvector>[0] and <myvector>[1] have the same size.

    const int nSmearBins = configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].i[CUTS::JET::k_nSmearBins];

    // event cuts/weights
    // photon cuts
    const std::string trigger = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_trigger_gammaJet].c_str();
    const float cut_phoHoverE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_phoHoverE];
    const float cut_phoSigmaIEtaIEta = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_phoSigmaIEtaIEta];
    const float cut_sumIso = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_sumIso];

    // jet cuts
    const std::string jetCollection = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kJET].s[CUTS::JET::k_jetCollection];
    const float cut_jetpt = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kJET].f[CUTS::JET::k_pt];
    const float cut_jeteta = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kJET].f[CUTS::JET::k_eta];
    const int   cut_jetID = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kJET].i[CUTS::JET::k_jetID];

    // gamma-jet cuts
    const float cut_awayRange = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kGAMMAJET].f[CUTS::GJT::k_awayRange] * TMath::Pi();
    const float cut_dR = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kGAMMAJET].f[CUTS::GJT::k_dR];

    // process cuts
    const bool doPhotonEnergyScaleSystematics = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].i[CUTS::PHO::k_doPhotonEnergyScaleSystematics];
    const bool doElectronRejection = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].i[CUTS::PHO::k_doElectronRejection];
    const bool doPhotonIsolationSys = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].i[CUTS::PHO::k_doPhotonIsolationSys];
    const bool useCorrectedSumIso = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].i[CUTS::PHO::k_useCorrectedSumIso];
    const float smearingRes = configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].f[CUTS::JET::k_smearingRes];
    const float energyScale = configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].f[CUTS::JET::k_energyScale];

    const bool doResolutionSmearing = (smearingRes > 0);
    const int nsmear = doResolutionSmearing ? 100 : 1;

    const bool doJetEnergyScaling = energyScale != 0;

    TF1* f_JES_Q[nBins_hiBin] = {0};
    f_JES_Q[0] = new TF1("f_JES_Q_0", "0.011180+0.195313/sqrt(x)", 30, 300);
    f_JES_Q[1] = new TF1("f_JES_Q_1", "0.011180+0.195313/sqrt(x)", 30, 300);
    f_JES_Q[2] = new TF1("f_JES_Q_2", "0.014454+0.089004/sqrt(x)", 30, 300);
    f_JES_Q[3] = new TF1("f_JES_Q_3", "0.011180+0.195313/sqrt(x)", 30, 300);
    f_JES_Q[4] = new TF1("f_JES_Q_4", "0.014200+0.127950/sqrt(x)", 30, 300);
    f_JES_Q[5] = new TF1("f_JES_Q_5", "0.014454+0.089004/sqrt(x)", 30, 300);
    f_JES_Q[6] = new TF1("f_JES_Q_6", "0.010469+0.084808/sqrt(x)", 30, 300);
    TF1* f_JES_G[nBins_hiBin] = {0};
    f_JES_G[0] = new TF1("f_JES_G_0", "0.021607+0.458346/sqrt(x)", 30, 300);
    f_JES_G[1] = new TF1("f_JES_G_1", "0.021607+0.458346/sqrt(x)", 30, 300);
    f_JES_G[2] = new TF1("f_JES_G_2", "0.021607+0.295396/sqrt(x)", 30, 300);
    f_JES_G[3] = new TF1("f_JES_G_3", "0.021607+0.458346/sqrt(x)", 30, 300);
    f_JES_G[4] = new TF1("f_JES_G_4", "0.023489+0.313111/sqrt(x)", 30, 300);
    f_JES_G[5] = new TF1("f_JES_G_5", "0.021607+0.295396/sqrt(x)", 30, 300);
    f_JES_G[6] = new TF1("f_JES_G_6", "0.021607+0.213359/sqrt(x)", 30, 300);

    TRandom3 rand(12345);

    // verbose about cut configuration
    std::cout << "Cut Configuration :" << std::endl;
    std::cout << "nBins_pt = " << nBins_pt << std::endl;
    for (int i = 0; i < nBins_pt; ++i)
        std::cout << Form("bins_pt[%d] = [%.1f, %.1f)", i, bins_pt[0][i], bins_pt[1][i]) << std::endl;
    std::cout << "nBins_hiBin = " << nBins_hiBin << std::endl;
    for (int i = 0; i < nBins_hiBin; ++i)
        std::cout << Form("bins_hiBin[%d] = [%d, %d)", i, bins_hiBin[0][i], bins_hiBin[1][i]) << std::endl;

    std::cout << "trigger    = " << trigger.c_str() << std::endl;

    std::cout << "cut_phoHoverE             = " << cut_phoHoverE << std::endl;
    std::cout << "cut_phoSigmaIEtaIEta      = " << cut_phoSigmaIEtaIEta << std::endl;
    std::cout << "cut_sumIso                = " << cut_sumIso << std::endl;
    std::cout << "useCorrectedSumIso        = " << useCorrectedSumIso << std::endl;

    std::cout << "jetCollection             = " << jetCollection.c_str() << std::endl;
    std::cout << "cut_jetpt                 = " << cut_jetpt << std::endl;
    std::cout << "cut_jeteta                = " << cut_jeteta << std::endl;
    std::cout << "cut_jetID                 = " << cut_jetID << std::endl;

    std::cout << "cut_awayRange             = " << cut_awayRange << std::endl;
    std::cout << "cut_dR                    = " << cut_dR << std::endl;

    std::cout << "doPhotonEnergyScaleSystematics = " << doPhotonEnergyScaleSystematics << std::endl;
    std::cout << "doElectronRejection = " << doElectronRejection << std::endl;
    std::cout << "doPhotonIsolationSys = " << doPhotonIsolationSys << std::endl;
    std::cout << "useCorrectedSumIso = " << useCorrectedSumIso << std::endl;
    std::cout << "doResolutionSmearing = " << doResolutionSmearing << std::endl;
    if (doResolutionSmearing) std::cout << "smearingRes = " << smearingRes << std::endl;
    std::cout << "doJetEnergyScaling = " << doJetEnergyScaling << std::endl;
    if (doJetEnergyScaling) std::cout << "energyScale = " << energyScale << std::endl;

    /// Input Bookkeeping block ///
    TFile* input = TFile::Open(inputFile);

    TTree* tHlt = (TTree*)input->Get("hltTree");
    tHlt->SetBranchStatus("*", 0);
    tHlt->SetBranchStatus("HLT_HISinglePhoton40_Eta1p5_v1", 1);
    tHlt->SetBranchStatus("HLT_HISinglePhoton40_Eta1p5_v2", 1);
    tHlt->SetBranchStatus("HLT_HISinglePhoton50_Eta3p1_v1", 1);
    tHlt->SetBranchStatus("HLT_HISinglePhoton50_Eta3p1_v2", 1);
    tHlt->SetBranchStatus("HLT_HISinglePhoton40_Eta1p5ForPPRef_v1", 1);

    TTree* tPho = (TTree*)input->Get("EventTree");    // photons
    tPho->SetBranchStatus("*", 0);
    tPho->SetBranchStatus("phoEt", 1);
    tPho->SetBranchStatus("phoEtCorrected", 1);
    tPho->SetBranchStatus("phoEtCorrected_sys", 1);
    tPho->SetBranchStatus("phoEta", 1);
    tPho->SetBranchStatus("phoPhi", 1);
    tPho->SetBranchStatus("phoSigmaIEtaIEta_2012", 1);
    tPho->SetBranchStatus("pho_ecalClusterIsoR4", 1);
    tPho->SetBranchStatus("pho_hcalRechitIsoR4", 1);
    tPho->SetBranchStatus("pho_trackIsoR4PtCut20", 1);
    tPho->SetBranchStatus("pho_sumIsoCorrected", 1);
    tPho->SetBranchStatus("phoHoverE", 1);
    tPho->SetBranchStatus("phoE3x3", 1);
    tPho->SetBranchStatus("phoE5x5", 1);
    tPho->SetBranchStatus("phoE1x5", 1);
    tPho->SetBranchStatus("phoE2x5", 1);
    if (doElectronRejection) {
        tPho->SetBranchStatus("nEle", 1);
        tPho->SetBranchStatus("elePt", 1);
        tPho->SetBranchStatus("eleEta", 1);
        tPho->SetBranchStatus("elePhi", 1);
        tPho->SetBranchStatus("eleEoverP", 1);
    }
    if (isMC && doPhotonIsolationSys) {
        tPho->SetBranchStatus("nMC", 1);
        tPho->SetBranchStatus("mcPID", 1);
        tPho->SetBranchStatus("mcEta", 1);
        tPho->SetBranchStatus("mcPhi", 1);
        tPho->SetBranchStatus("mcCalIsoDR03", 1);
        tPho->SetBranchStatus("mcCalIsoDR04", 1);
        tPho->SetBranchStatus("pho_genMatchedIndex", 1);
    }

    TTree* tJet = (TTree*)input->Get(jetCollection.c_str());
    if (!tJet) {
        std::cout << "following jet collection is not found in the input file : " << jetCollection.c_str() << std::endl;
        std::cout << "exiting" << std::endl;
        return 1;
    }

    tJet->SetBranchStatus("*", 0);
    tJet->SetBranchStatus("nref", 1);
    tJet->SetBranchStatus("jtpt", 1);
    tJet->SetBranchStatus("jtpt_smeared", 1);
    // tJet->SetBranchStatus("jteta", 1);

    TTree* tHiEvt = (TTree*)input->Get("HiEvt");       // HiEvt tree will be placed in PP forest as well.

    ggHiNtuplizer pho;
    pho.setupTreeForReading(tPho);

    Jets jet;
    jet.setupTreeForReading(tJet);

    hiEvt evt;
    evt.setupTreeForReading(tHiEvt);

    Int_t HLT_HISinglePhoton40_Eta1p5_v1, HLT_HISinglePhoton40_Eta1p5_v2;
    Int_t HLT_HISinglePhoton50_Eta3p1_v1, HLT_HISinglePhoton50_Eta3p1_v2;
    Int_t HLT_HISinglePhoton40_Eta1p5ForPPRef_v1;
    tHlt->SetBranchAddress("HLT_HISinglePhoton40_Eta1p5_v1", &HLT_HISinglePhoton40_Eta1p5_v1);
    tHlt->SetBranchAddress("HLT_HISinglePhoton40_Eta1p5_v2", &HLT_HISinglePhoton40_Eta1p5_v2);
    tHlt->SetBranchAddress("HLT_HISinglePhoton50_Eta3p1_v1", &HLT_HISinglePhoton50_Eta3p1_v1);
    tHlt->SetBranchAddress("HLT_HISinglePhoton50_Eta3p1_v2", &HLT_HISinglePhoton50_Eta3p1_v2);
    tHlt->SetBranchAddress("HLT_HISinglePhoton40_Eta1p5ForPPRef_v1", &HLT_HISinglePhoton40_Eta1p5ForPPRef_v1);

    TTree* gammaJetTree[nSmearBins + 1];
    gammaJetTree[0] = (TTree*)input->Get(Form("gamma_%s", jetCollection.c_str()));
    for (int i = 0; i < nSmearBins; i++)
        gammaJetTree[i + 1] = (TTree*)input->Get(Form("gamma_%s_smearBin%i", jetCollection.c_str(), i));

    GammaJet gammaJet[nSmearBins + 1];
    for (int i = 0; i < nSmearBins + 1; i++)
        gammaJet[i].setupGammaJetTree(gammaJetTree[i]);

    TTree* jetTreeMB = 0;
    TTree* gammaJetTreeMB = 0;
    // check the existence of HI specific trees in "gammaJetSkim.root" file
    bool hasJetsMB = false;
    bool hasGammaJetMB = false;
    if (isHI) {
        input->GetObject(Form("%sMB", jetCollection.c_str()), jetTreeMB);
        input->GetObject(Form("gamma_%sMB", jetCollection.c_str()), gammaJetTreeMB);

        if (jetTreeMB)
            hasJetsMB = true;
        if (gammaJetTreeMB)
            hasGammaJetMB = true;
    }

    Jets jetMB;
    GammaJet gammaJetMB;
    if (hasJetsMB && hasGammaJetMB) {
        jetTreeMB->SetBranchStatus("*", 0);
        jetTreeMB->SetBranchStatus("jtpt", 1);
        jetTreeMB->SetBranchStatus("nref", 1);
        // jetTreeMB->SetBranchStatus("jteta", 1);

        jetMB.setupTreeForReading(jetTreeMB);
        gammaJetMB.setupGammaJetTree(gammaJetTreeMB);
    }

    // smearing set up block
    // ONLY used for JER systematic
    // WARNING make super sure this matches the values in gammaJetSkim.C !!
    // otherwise JER systematic will be off
    jetCorrector resolutionJetSmear[6];

    // smear 0-10 %
    std::vector<double> CSN_HI_cent0010 = {0.06, 1.23, 8.38};
    std::vector<double> CSN_phi_HI_cent0010 = {-3.18781 / 10000000, 0.125911, 2.23898};
    // smear 10-30 %
    std::vector<double> CSN_HI_cent1030 = {0.06, 1.23, 5.88};
    std::vector<double> CSN_phi_HI_cent1030 = {1.14344 / 100000, 0.179847, 1.56128};
    // smear 30-50 %
    std::vector<double> CSN_HI_cent3050 = {0.06, 1.23, 3.24};
    std::vector<double> CSN_phi_HI_cent3050 = {0.0145775, 0.1222, 1.21751};
    // smear 50-100 %
    std::vector<double> CSN_HI_cent50100 = {0.06, 1.23, 0};
    std::vector<double> CSN_phi_HI_cent50100 = {-0.0073078, 0.168879, 0.798885};

    // 0-30 and 30-100 are necessary only for smeared pp JER computation
    // smear 0-30 %
    std::vector<double> CSN_HI_cent0030 = {0.06, 1.23, 7.38};
    std::vector<double> CSN_phi_HI_cent0030 = {-1.303 / 1000000, 0.1651, 1.864};
    // smear 30-100 %
    std::vector<double> CSN_HI_cent30100 = {0.06, 1.23, 2.1};
    std::vector<double> CSN_phi_HI_cent30100 = {-2.013 / 100000000, 0.1646, 1.04};


    for (int i = 0; i < 6; ++i) {
        resolutionJetSmear[i].rand = rand;

        switch (i) {
            case 0: //0-10
                resolutionJetSmear[i].CSN_HI = CSN_HI_cent0010;
                resolutionJetSmear[i].CSN_phi_HI = CSN_phi_HI_cent0010;
                break;
            case 1: //10-30
                resolutionJetSmear[i].CSN_HI = CSN_HI_cent1030;
                resolutionJetSmear[i].CSN_phi_HI = CSN_phi_HI_cent1030;
                break;
            case 2: //30-50
                resolutionJetSmear[i].CSN_HI = CSN_HI_cent3050;
                resolutionJetSmear[i].CSN_phi_HI = CSN_phi_HI_cent3050;
                break;
            case 3: //50-100
                resolutionJetSmear[i].CSN_HI = CSN_HI_cent50100;
                resolutionJetSmear[i].CSN_phi_HI = CSN_phi_HI_cent50100;
                break;
            case 4: // 0-30, for smeared pp only
                resolutionJetSmear[i].CSN_HI = CSN_HI_cent0030;
                resolutionJetSmear[i].CSN_phi_HI = CSN_phi_HI_cent0030;
                break;
            case 5: // 30-100, for smeared pp only
                resolutionJetSmear[i].CSN_HI = CSN_HI_cent30100;
                resolutionJetSmear[i].CSN_phi_HI = CSN_phi_HI_cent30100;
                break;
        }
    }

    /// End Input Bookkeeping block //

    TFile* output = TFile::Open(outputFile, "RECREATE");
    // histograms will be put under a directory whose name is the type of the collision
    if (!output->GetKey(collisionName))
        output->mkdir(collisionName, Form("input file is %s", inputFile.Data()));
    output->cd(collisionName);
    std::cout << "histograms will be put under directory : " << collisionName << std::endl;

    TTree* configTree = setupConfigurationTreeForWriting(configCuts);

    int nCorrHist = correlationHistNames.size();
    correlationHist corrHists[nCorrHist][nBins_pt][nBins_hiBin];

    // prepare histogram names for xjg, abs(dphi) and jet pt
    for (int iHist = 0; iHist < nCorrHist; ++iHist) {
        for (int i = 0; i < nBins_pt; ++i) {
            for (int j = 0; j < nBins_hiBin; ++j) {
                corrHists[iHist][i][j].name = Form("%s_ptBin%d_hiBin%d", correlationHistNames[iHist].c_str(), i, j);

                for (int iCorr = 0; iCorr < CORR::kN_CORRFNC; ++iCorr) {
                    for (int jCorr = 0; jCorr < CORR::kN_CORRFNC; ++jCorr) {
                        std::string subHistName = Form("%s_ptBin%d_hiBin%d_%s_%s", correlationHistNames[iHist].c_str(), i, j,
                                                       CORR::CORR_PHO_LABELS[iCorr].c_str(), CORR::CORR_JET_LABELS[jCorr].c_str());
                        corrHists[iHist][i][j].h1D_name[iCorr][jCorr] = subHistName.c_str();
                        corrHists[iHist][i][j].h1D[iCorr][jCorr] = new TH1D(Form("h1D_%s", subHistName.c_str()), "",
                                nBinsx[iHist], xlow[iHist], xup[iHist]);

                        corrHists[iHist][i][j].h1D_titleX[iCorr][jCorr] = correlationHistTitleX[iHist].c_str();
                        corrHists[iHist][i][j].h1D_titleY_final_norm[iCorr][jCorr] = correlationHistTitleY_final_normalized[iHist].c_str();
                    }
                }
            }
        }
    }

    // histograms to store the number of photon events, not photon-Jet event
    // those histograms have a single bin whose content is the number of photon events
    // they are just a tool to store number.
    TH1D* h_nPho[nBins_pt][nBins_hiBin][2];
    for (int i = 0; i < nBins_pt; ++i) {
        for (int j = 0; j < nBins_hiBin; ++j) {
            std::string histNamePhoRAW = Form("h_nPho_ptBin%d_hiBin%d_%s", i, j, CORR::CORR_PHO_LABELS[CORR::kRAW].c_str());
            h_nPho[i][j][CORR::kRAW] = new TH1D(histNamePhoRAW.c_str(), "", 1, 0, 1);

            std::string histNamePhoBKG = Form("h_nPho_ptBin%d_hiBin%d_%s", i, j, CORR::CORR_PHO_LABELS[CORR::kBKG].c_str());
            h_nPho[i][j][CORR::kBKG] = new TH1D(histNamePhoBKG.c_str(), "", 1, 0, 1);
        }
    }

    // selection for jet regions
    // jet from bkg region are already separated from raw region.
    // no additional selection for jets. just use different trees.
    std::cout << "####################" << std::endl;
    std::cout << "gammaJetTree->GetEntries() = " << gammaJetTree[0]->GetEntries() << std::endl;
    if (trigger.compare("") != 0 && !isMC)
        std::cout << "gammaJetTree->GetEntries(trigger==1) = " << tHlt->GetEntries(Form("%s == 1", trigger.c_str())) << std::endl;
    else
        std::cout << "gammaJetTree->GetEntries(trigger==1) is skipped because either no trigger is specified or the data is coming from MC." << std::endl;
    std::cout << "####################" << std::endl;

    long long nentries = gammaJetTree[0]->GetEntries();
    long long firstEntry = 0;
    long long lastEntry = nentries;
    std::cout << "Total Entries: " << nentries << std::endl;

    if (nJobs != -1) {
        if (jobNum >= nJobs) {
            std::cout << "jobNum > nJobs, invalid configuration, aborting" << std::endl;
            return 1;
        }

        firstEntry = floor(nentries / nJobs) * jobNum;
        lastEntry = floor(nentries / nJobs) * (jobNum + 1);
        if (jobNum == nJobs - 1)
            lastEntry = nentries;

        std::cout << "For this job " << jobNum << std::endl;
        std::cout << "First Entry: " << firstEntry << std::endl;
        std::cout << "Final Entry: " << lastEntry << std::endl;
    }

    for (long long jentry = firstEntry; jentry < lastEntry; jentry++) {
        if (jentry % 2000 == 0 && nJobs == -1)
            printf("current entry = %lli out of %lli : %3f%%\n", jentry, nentries, (double)jentry / nentries * 100);

        tHlt->GetEntry(jentry);
        // event selection
        if (!isMC && !HLT_HISinglePhoton40_Eta1p5_v1)
            continue;
        if (isHI && isMC && !HLT_HISinglePhoton40_Eta1p5_v2)
            continue;
        if (!isHI && isMC && !HLT_HISinglePhoton40_Eta1p5ForPPRef_v1)
            continue;

        tPho->GetEntry(jentry);
        gammaJetTree[0]->GetEntry(jentry);

        // eta cut moved to skim step
        // if (TMath::Abs((*pho.phoEta)[gammaJet[0].phoIdx]) > 1.44) continue;

        // noise cut
        if (((*pho.phoE3x3)[gammaJet[0].phoIdx] / (*pho.phoE5x5)[gammaJet[0].phoIdx] > 2. / 3. - 0.03 &&
                (*pho.phoE3x3)[gammaJet[0].phoIdx] / (*pho.phoE5x5)[gammaJet[0].phoIdx] < 2. / 3. + 0.03) &&
                ((*pho.phoE1x5)[gammaJet[0].phoIdx] / (*pho.phoE5x5)[gammaJet[0].phoIdx] > 1. / 3. - 0.03 &&
                 (*pho.phoE1x5)[gammaJet[0].phoIdx] / (*pho.phoE5x5)[gammaJet[0].phoIdx] < 1. / 3. + 0.03) &&
                ((*pho.phoE2x5)[gammaJet[0].phoIdx] / (*pho.phoE5x5)[gammaJet[0].phoIdx] > 2. / 3. - 0.03 &&
                 (*pho.phoE2x5)[gammaJet[0].phoIdx] / (*pho.phoE5x5)[gammaJet[0].phoIdx] < 2. / 3. + 0.03)) continue;

        // isolation cut
        if (useCorrectedSumIso) {
            if ((*pho.pho_sumIsoCorrected)[gammaJet[0].phoIdx] > cut_sumIso) {
                continue;
            }
        } else if (((*pho.pho_ecalClusterIsoR4)[gammaJet[0].phoIdx] +
                    (*pho.pho_hcalRechitIsoR4)[gammaJet[0].phoIdx] +
                    (*pho.pho_trackIsoR4PtCut20)[gammaJet[0].phoIdx]) > cut_sumIso) {
            continue;
        }

        if ((*pho.phoHoverE)[gammaJet[0].phoIdx] > 0.1)
            continue;

        bool isSignalPho = ((*pho.phoSigmaIEtaIEta_2012)[gammaJet[0].phoIdx] < cut_phoSigmaIEtaIEta);
        bool isBkgPho = ((*pho.phoSigmaIEtaIEta_2012)[gammaJet[0].phoIdx] > 0.011 &&
                         (*pho.phoSigmaIEtaIEta_2012)[gammaJet[0].phoIdx] < 0.017);

        if (!(isSignalPho || isBkgPho))
            continue;

        int phoType = isSignalPho ? CORR::kRAW : CORR::kBKG;

        // reco electron rejection part
        bool isEle = false;
        if (doElectronRejection) {
            float eleEpTemp = 100.0;
            for (int ie = 0; ie < pho.nEle; ++ie) {
                if ((*pho.elePt)[ie] < 10)
                    continue;
                if (abs((*pho.eleEta)[ie] - (*pho.phoEta)[gammaJet[0].phoIdx]) > 0.03) // deta
                    continue;
                if (abs(getDPHI((*pho.elePhi)[ie], (*pho.phoPhi)[gammaJet[0].phoIdx])) > 0.03) // dphi
                    continue;
                if (eleEpTemp < pho.eleEoverP->at(ie))
                    continue;

                isEle = true;
                break;
            }
        }
        if (isEle)
            continue;

        // photon isolation systematic part (gen matching & genIso condition)
        if (isMC && doPhotonIsolationSys) {
            if (!(((*pho.pho_genMatchedIndex)[gammaJet[0].phoIdx] != -1) && (pho.mcCalIsoDR04->at((*pho.pho_genMatchedIndex)[gammaJet[0].phoIdx]) < 5.0))) {
                // std::cout << "failedGenIso : mcCalIso = " << (pho.mcCalIsoDR04->at((*pho.pho_genMatchedIndex)[gammaJet[0].phoIdx]))<< std::endl;
                continue;
            }
        }

        tJet->GetEntry(jentry);
        tHiEvt->GetEntry(jentry);

        float weight = isMC ? evt.weight : 1;
        float phoEt = doPhotonEnergyScaleSystematics ? (*pho.phoEtCorrected_sys)[gammaJet[0].phoIdx] : (*pho.phoEtCorrected)[gammaJet[0].phoIdx];

        // handle nEntriesPho separate from jet loop
        for (int i = 0; i < nBins_pt; ++i) {
            if (phoEt <  bins_pt[0][i] || phoEt >= bins_pt[1][i])
                continue;
            for (int j = 0; j < nBins_hiBin; ++j) {
                if (isHI && (evt.hiBin <  bins_hiBin[0][j] || evt.hiBin >= bins_hiBin[1][j]))
                    continue;
                for (int iHist = 0; iHist < nCorrHist; iHist++) {
                    corrHists[iHist][i][j].nEntriesPho[phoType][CORR::kRAW] += weight;
                    ++corrHists[iHist][i][j].nEntriesPhoRaw[phoType][CORR::kRAW];
                }
            }
        }

        weight /= nsmear;

        for (int j = 0; j < nBins_hiBin; ++j) {
            int smearBin = 0;
            if (isHI) {
                if (evt.hiBin <  bins_hiBin[0][j] || evt.hiBin >= bins_hiBin[1][j])
                    continue;
            } else {
                if (j) {
                    // change smearBin to select the smeared jtpt and jtphi corresponding to the centrality bin
                    // 0 is unsmeared jtpt
                    smearBin = j;
                }

                if (smearBin)
                    gammaJetTree[smearBin]->GetEntry(jentry);
            }

            for (int ijet = 0; ijet < jet.nref; ijet++) {
                for (int ismear = 0; ismear < nsmear; ismear++) {
                    float jetpt = (*jet.jtpt_smeared)[smearBin][ijet];

                    // JER systematics
                    float JER_factor = 1;
                    if (doResolutionSmearing) {
                        float smear_factor = 1 + smearingRes;
                        int resolutionBin = 0;
                        if (isHI) {
                            resolutionBin = getResolutionBin(evt.hiBin);
                        } else {
                            resolutionBin = getResolutionBinPP(smearBin);
                        }
                        float initialResolution = resolutionJetSmear[resolutionBin].getResolutionHI(jetpt);
                        JER_factor = rand.Gaus(1, smear_factor * initialResolution * sqrt(smear_factor * smear_factor - 1));
                        JER_factor = 1 + variation * (JER_factor - 1);
                        jetpt *= JER_factor;
                    }

                    // JES systematics
                    float JES_factor = 1 + energyScale;
                    if (doJetEnergyScaling) {
                        if (isHI) {
                            float flavour_factor = 0;
                            if (energyScale > 0) flavour_factor = f_JES_Q[j]->Eval(jetpt);
                            if (energyScale < 0) flavour_factor = f_JES_G[j]->Eval(jetpt);
                            JES_factor = 1 + (energyScale / TMath::Abs(energyScale)) *
                                TMath::Sqrt(energyScale * energyScale + flavour_factor * flavour_factor);
                        }
                        JES_factor = 1 + variation * (JES_factor - 1);
                        jetpt *= JES_factor;
                    }

                    // jet cuts
                    if ((*gammaJet[smearBin].dR)[ijet] < cut_dR) continue;
                    if (jetpt < cut_jetpt) continue;
                    // jteta cut moved to skim
                    // if (TMath::Abs(jet.jteta[ijet]) > cut_jeteta) continue;
                    if ((*gammaJet[0].jetID)[ijet] < cut_jetID) continue;

                    for (int i = 0; i < nBins_pt; ++i) {
                        if (phoEt <  bins_pt[0][i] || phoEt >= bins_pt[1][i])
                            continue;

                        // fill histograms
                        // dphi = 1
                        corrHists[1][i][j].h1D[phoType][CORR::kRAW]->Fill(TMath::Abs((*gammaJet[smearBin].dphi)[ijet]), weight);
                        corrHists[1][i][j].nEntries[phoType][CORR::kRAW] += weight;
                        ++corrHists[1][i][j].nEntriesRaw[phoType][CORR::kRAW];

                        // apply dphi cuts now
                        if (TMath::Abs((*gammaJet[smearBin].dphi)[ijet]) <= cut_awayRange)
                            continue;

                        // xjg = 0, jtpt = 2
                        float xjg = doPhotonEnergyScaleSystematics ? (*gammaJet[smearBin].xjgCorrected_sys)[ijet] : (*gammaJet[smearBin].xjgCorrected)[ijet];
                        // JER, JES systematics
                        if (doResolutionSmearing) { xjg *= JER_factor; }
                        if (doJetEnergyScaling) { xjg *= JES_factor; }

                        corrHists[0][i][j].h1D[phoType][CORR::kRAW]->Fill(xjg, weight);
                        corrHists[0][i][j].nEntries[phoType][CORR::kRAW] += weight;
                        ++corrHists[0][i][j].nEntriesRaw[phoType][CORR::kRAW];
                        corrHists[2][i][j].h1D[phoType][CORR::kRAW]->Fill(jetpt, weight);
                        corrHists[2][i][j].nEntries[phoType][CORR::kRAW] += weight;
                        ++corrHists[2][i][j].nEntriesRaw[phoType][CORR::kRAW];
                    }
                }
            }
        }

        if (hasJetsMB && hasGammaJetMB) {
            jetTreeMB->GetEntry(jentry);
            gammaJetTreeMB->GetEntry(jentry);

            for (int j = 0; j < nBins_hiBin; ++j) {
                if (evt.hiBin <  bins_hiBin[0][j] || evt.hiBin >= bins_hiBin[1][j])
                    continue;

                for (int ijet = 0; ijet < jetMB.nref; ijet++) {
                    for (int ismear = 0; ismear < nsmear; ismear++) {
                        float jetpt = jetMB.jtpt[ijet];

                        // JER systematics
                        float JER_factor = 1;
                        if (doResolutionSmearing) {
                            float smear_factor = 1 + smearingRes;
                            int resolutionBin = 0;
                            resolutionBin = getResolutionBin(evt.hiBin);
                            float initialResolution = resolutionJetSmear[resolutionBin].getResolutionHI(jetpt);
                            JER_factor = rand.Gaus(1, smear_factor * initialResolution * sqrt(smear_factor * smear_factor - 1));
                            jetpt *= JER_factor;
                        }

                        // JES systematics
                        float JES_factor = 1 + energyScale;
                        if (doJetEnergyScaling) {
                            if (isHI) {
                                float flavour_factor = 0;
                                if (energyScale > 0) flavour_factor = f_JES_Q[j]->Eval(jetpt);
                                if (energyScale < 0) flavour_factor = f_JES_G[j]->Eval(jetpt);
                                JES_factor = 1 + (energyScale / TMath::Abs(energyScale)) *
                                    TMath::Sqrt(energyScale * energyScale + flavour_factor * flavour_factor);
                            }
                            jetpt *= JES_factor;
                        }

                        // jet cuts
                        if ((*gammaJetMB.dR)[ijet] < cut_dR) continue;
                        if (jetpt < cut_jetpt) continue;
                        // jteta cut applied in skim
                        // if (TMath::Abs(jetMB.jteta[ijet]) > cut_jeteta) continue;
                        if ((*gammaJetMB.jetID)[ijet] < cut_jetID) continue;

                        for (int i = 0; i < nBins_pt; ++i) {
                            if ((*pho.phoEtCorrected)[gammaJetMB.phoIdx] <  bins_pt[0][i] ||
                                    (*pho.phoEtCorrected)[gammaJetMB.phoIdx] >= bins_pt[1][i]) continue;

                            // fill histograms
                            // dphi = 1
                            corrHists[1][i][j].h1D[phoType][CORR::kBKG]->Fill(TMath::Abs((*gammaJetMB.dphi)[ijet]), weight);
                            corrHists[1][i][j].nEntries[phoType][CORR::kBKG] += weight;
                            ++corrHists[1][i][j].nEntriesRaw[phoType][CORR::kBKG];

                            // apply dphi cuts now
                            if (TMath::Abs((*gammaJetMB.dphi)[ijet]) <= cut_awayRange)
                                continue;

                            // xjg = 0, jtpt = 2
                            float xjg = doPhotonEnergyScaleSystematics ? (*gammaJetMB.xjgCorrected_sys)[ijet] : (*gammaJetMB.xjgCorrected)[ijet];

                            // JER, JES systematics
                            if (doResolutionSmearing) { xjg *= JER_factor; }
                            if (doJetEnergyScaling) { xjg *= JES_factor; }

                            corrHists[0][i][j].h1D[phoType][CORR::kBKG]->Fill(xjg, weight);
                            corrHists[0][i][j].nEntries[phoType][CORR::kBKG] += weight;
                            ++corrHists[0][i][j].nEntriesRaw[phoType][CORR::kBKG];
                            corrHists[2][i][j].h1D[phoType][CORR::kBKG]->Fill(jetpt, weight);
                            corrHists[2][i][j].nEntries[phoType][CORR::kBKG] += weight;
                            ++corrHists[2][i][j].nEntriesRaw[phoType][CORR::kBKG];
                        }
                    }
                }
            }
        }
    }

    /// Histogram arithmetic (no reading I/O)
    for (int iHist = 0; iHist < nCorrHist; iHist++) {
        for (int i = 0; i < nBins_pt; ++i) {
            for (int j = 0; j < nBins_hiBin; ++j) {
                corrHists[iHist][i][j].nEntriesPho[CORR::kRAW][CORR::kBKG] = corrHists[iHist][i][j].nEntriesPho[CORR::kRAW][CORR::kRAW];
                corrHists[iHist][i][j].nEntriesPhoRaw[CORR::kRAW][CORR::kBKG] = corrHists[iHist][i][j].nEntriesPhoRaw[CORR::kRAW][CORR::kRAW];
                corrHists[iHist][i][j].nEntriesPho[CORR::kBKG][CORR::kBKG] = corrHists[iHist][i][j].nEntriesPho[CORR::kBKG][CORR::kRAW];
                corrHists[iHist][i][j].nEntriesPhoRaw[CORR::kBKG][CORR::kBKG] = corrHists[iHist][i][j].nEntriesPhoRaw[CORR::kBKG][CORR::kRAW];

                if (h_nPho[i][j][CORR::kRAW]->GetBinContent(1) == 0) {
                    h_nPho[i][j][CORR::kRAW]->SetBinContent(1, corrHists[iHist][i][j].nEntriesPho[CORR::kRAW][CORR::kRAW]);
                    h_nPho[i][j][CORR::kRAW]->Write("", TObject::kOverwrite);
                }

                if (h_nPho[i][j][CORR::kBKG]->GetBinContent(1) == 0) {
                    h_nPho[i][j][CORR::kBKG]->SetBinContent(1, corrHists[iHist][i][j].nEntriesPho[CORR::kBKG][CORR::kRAW]);
                    h_nPho[i][j][CORR::kBKG]->Write("", TObject::kOverwrite);
                }

                printf("ptBin: %i, hiBin: %i, nEntries[CORR::kRAW][CORR::kRAW] = %f\n", i, j, corrHists[iHist][i][j].nEntries[CORR::kRAW][CORR::kRAW]);
                printf("ptBin: %i, hiBin: %i, nEntries[CORR::kBKG][CORR::kRAW] = %f\n", i, j, corrHists[iHist][i][j].nEntries[CORR::kBKG][CORR::kRAW]);
                printf("ptBin: %i, hiBin: %i, nEntriesRaw[CORR::kRAW][CORR::kRAW] = %f\n", i, j, corrHists[iHist][i][j].nEntriesRaw[CORR::kRAW][CORR::kRAW]);
                printf("ptBin: %i, hiBin: %i, nEntriesRaw[CORR::kBKG][CORR::kRAW] = %f\n", i, j, corrHists[iHist][i][j].nEntriesRaw[CORR::kBKG][CORR::kRAW]);
                printf("ptBin: %i, hiBin: %i, nEntriesPho[CORR::kRAW][CORR::kRAW] = %f\n", i, j, corrHists[iHist][i][j].nEntriesPho[CORR::kRAW][CORR::kRAW]);
                printf("ptBin: %i, hiBin: %i, nEntriesPho[CORR::kBKG][CORR::kRAW] = %f\n", i, j, corrHists[iHist][i][j].nEntriesPho[CORR::kBKG][CORR::kRAW]);
                printf("ptBin: %i, hiBin: %i, nEntriesPhoRaw[CORR::kRAW][CORR::kRAW] = %f\n", i, j, corrHists[iHist][i][j].nEntriesPhoRaw[CORR::kRAW][CORR::kRAW]);
                printf("ptBin: %i, hiBin: %i, nEntriesPhoRaw[CORR::kBKG][CORR::kRAW] = %f\n", i, j, corrHists[iHist][i][j].nEntriesPhoRaw[CORR::kBKG][CORR::kRAW]);

                std::string histoTitle;
                if (bins_pt[1][i] == 9999 && bins_hiBin[0][j] <= 0 && bins_hiBin[1][j] >= 200)
                    histoTitle = Form("%s , p^{#gamma}_{T} > %.0f GeV/c", collisionName, bins_pt[0][i]);
                else if (bins_hiBin[0][j] <= 0 && bins_hiBin[1][j] >= 200)
                    histoTitle = Form("%s , %.0f < p^{#gamma}_{T} < %.0f GeV/c", collisionName, bins_pt[0][i], bins_pt[1][i]);
                else if (bins_pt[1][i] == 9999)
                    histoTitle = Form("%s , p^{#gamma}_{T} > %.0f GeV/c, %d-%d %% ", collisionName, bins_pt[0][i], bins_hiBin[0][j] / 2, bins_hiBin[1][j] / 2);
                else
                    histoTitle = Form("%s , %.0f < p^{#gamma}_{T} < %.0f GeV/c, %d-%d %% ", collisionName, bins_pt[0][i], bins_pt[1][i], bins_hiBin[0][j] / 2, bins_hiBin[1][j] / 2);

                // histograms for RAW and BKG regions
                for (int iCorr = 0; iCorr < CORR::kN_CORRFNC - 1; ++iCorr) {
                    for (int jCorr = 0; jCorr < CORR::kN_CORRFNC - 1; ++jCorr) {
                        if (jCorr == CORR::kBKG && !isHI) continue;      // no jet background for non-HI

                        std::string titleX = corrHists[iHist][i][j].h1D_titleX[iCorr][jCorr].c_str();
                        corrHists[iHist][i][j].h1D[iCorr][jCorr]->SetTitle(Form("%s;%s;%s", histoTitle.c_str(), titleX.c_str(), "Entries"));
                        corrHists[iHist][i][j].h1D[iCorr][jCorr]->SetMarkerStyle(kFullCircle);
                        corrHists[iHist][i][j].h1D[iCorr][jCorr]->SetMarkerColor(kBlack);

                        // histogram name excluding the "h1D" prefix
                        std::string tmpH1D_name = corrHists[iHist][i][j].h1D_name[iCorr][jCorr].c_str();
                        std::string tmpHistName = corrHists[iHist][i][j].h1D[iCorr][jCorr]->GetName();

                        corrHists[iHist][i][j].h1D[iCorr][jCorr]->Write("", TObject::kOverwrite);
                    }
                }
            }
        }
    }

    configTree->Write("", TObject::kOverwrite);

    output->Write("", TObject::kOverwrite);
    input->Close();
    output->Close();

    return 0;
}

int getResolutionBin(int hiBin) {
    int boundaries[4] = {20, 60, 100, 200};
    for (int i = 0; i < 4; ++i) {
        if (hiBin < boundaries[i])
            return i;
    }
    std::cout << "Warning, getResolutionBin out of bounds." << std::endl;
    return 0;
}

int getResolutionBinPP(int smearBin) {
    switch (smearBin) {
        case 0: // no smearing? make something up...
            return 0;
        case 1: // 0-30%
            return 4;
        case 2: // 30-100%
            return 5;
        case 3: // 0-10%
            return 0;
        case 4: // 10-30%
            return 1;
        case 5: // 30-50%
            return 2;
        case 6: // 50-100%
            return 3;
        case 7: //sys ? make something up...
            return 0;
    }
    std::cout << "Warning, getResolutionBinPP out of bounds." << std::endl;
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 7)
        return gammaJetHistogram(argv[1], argv[2], argv[3], atoi(argv[4]), atoi(argv[5]), atof(argv[6]));
    else if (argc == 6)
        return gammaJetHistogram(argv[1], argv[2], argv[3], atoi(argv[4]), atoi(argv[5]));
    else if (argc == 4)
        return gammaJetHistogram(argv[1], argv[2], argv[3]);
    else
        printf("Usage : \n"
               "./gammaJetHistogram.exe <configFile> <inputFile> <outputFile>\n");

    return 1;
}
