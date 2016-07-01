#include <TFile.h>
#include <TTree.h>
#include <TCut.h>
#include <TH1D.h>
#include <TMath.h>

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

#include "../TreeHeaders/CutConfigurationTree.h"
#include "../TreeHeaders/JetTree.h"
#include "../TreeHeaders/gammaJetTree.h"
#include "../TreeHeaders/hiEvtTree.h"
#include "../TreeHeaders/ggHiNtuplizerTree.h"
#include "../Plotting/commonUtility.h"
#include "interface/correlationHist.h"
#include "../Utilities/eventUtil.h"
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
// const std::vector<double>      xlow_final {0,  0,           0};
// const std::vector<double>      xup_final  {2,  TMath::Pi(), 200};

const int nSmearBins = 8; // should come from config...

float smeared_jtpt(Jets j, int ijet, int smearingBinIndex) {
    switch (smearingBinIndex) {
    case 0:
        return j.jtpt[ijet];
    case 1:
        return j.jtpt_smeared_0_30[ijet];
    case 2:
        return j.jtpt_smeared_30_100[ijet];
    case 3:
        return j.jtpt_smeared_0_10[ijet];
    case 4:
        return j.jtpt_smeared_10_30[ijet];
    case 5:
        return j.jtpt_smeared_30_50[ijet];
    case 6:
        return j.jtpt_smeared_50_100[ijet];
    case 7:
        return j.jtpt_smeared_sys[ijet];
    default:
        return j.jtpt[ijet];
    }
}

void gammaJetHistogram(const TString configFile, const TString inputFile, const TString outputFile, const int nJobs = -1, const int jobNum = -1);

void gammaJetHistogram(const TString configFile, const TString inputFile, const TString outputFile, const int nJobs, const int jobNum)
{
    TH1::SetDefaultSumw2();

    std::cout << "running gammaJetHistogram()" << std::endl;
    std::cout << "configFile  = " << configFile.Data() << std::endl;
    std::cout << "inputFile   = " << inputFile.Data() << std::endl;
    std::cout << "outputFile  = " << outputFile.Data() << std::endl;

    InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());
    if (!configCuts.isValid) {
        std::cout << "Invalid configfile, terminating." << std::endl;
        return;
    }

    // input configuration
    int collision;
    collision = configInput.proc[INPUT::kHISTOGRAM].i[INPUT::k_collisionType];
    // verbose about input configuration
    std::cout << "Input Configuration :" << std::endl;
    const char* collisionName = getCollisionTypeName((COLL::TYPE)collision).c_str();
    std::cout << "collision = " << collisionName << std::endl;

    bool isMC = collisionIsMC((COLL::TYPE)collision);
    bool isHI = collisionIsHI((COLL::TYPE)collision);

    // observable bins
    std::vector<float> bins_pt[2];          // array of vectors for eta bins, each array element is a vector.
    std::vector<int>   bins_hiBin[2];       // array of vectors for hiBin bins, each array element is a vector.
    // event cuts/weights
    int doEventWeight;
    std::string eventWeight;    // weight to be used for histogram entries
                                // current purpose of this variable is for weighting events from MC samples.
    // photon cuts
    std::string trigger;
    float cut_phoHoverE;
    float cut_phoSigmaIEtaIEta;
    float cut_sumIso;

    // bool usePPstyleIso;

    // // isolation for PP
    // float cut_phoHOverE_EB;         // Barrel
    // float cut_pfcIso4_EB;
    // float cut_pfnIso4_c0_EB;
    // float cut_pfnIso4_c1_EB;
    // float cut_pfnIso4_c2_EB;
    // float cut_pfpIso4_c0_EB;
    // float cut_pfpIso4_c1_EB;
    // float cut_phoHOverE_EE;         // Endcap
    // float cut_pfcIso4_EE;
    // float cut_pfnIso4_c0_EE;
    // float cut_pfnIso4_c1_EE;
    // float cut_pfnIso4_c2_EE;
    // float cut_pfpIso4_c0_EE;
    // float cut_pfpIso4_c1_EE;

    // jet cuts
    std::string jetCollection;
    float cut_jetpt;
    float cut_jeteta;
    int   cut_jetID;
    // gammaJet cuts
    float cut_awayRange;
    float cut_awayRange_lt;
    float cut_dR;
    // process cuts
    // int nEventsToMix;
    // int nSmear;
    bool useUncorrectedPhotonEnergy;
    bool doElectronRejection;

    bins_pt[0] = ConfigurationParser::ParseListFloat(
        configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_pt_gt]);
    bins_pt[1] = ConfigurationParser::ParseListFloat(
        configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_pt_lt]);
    bins_hiBin[0] = ConfigurationParser::ParseListInteger(
        configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_gt]);
    bins_hiBin[1] = ConfigurationParser::ParseListInteger(
        configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_lt]);

    int dphi_check = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].i[CUTS::EVT::k_dphi_check];

    doEventWeight = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].i[CUTS::EVT::k_doEventWeight];
    eventWeight = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_eventWeight].c_str();

    trigger = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_trigger_gammaJet].c_str();
    // trigger is named differently in MC, hardcode for now :(
    std::string triggerMC_forPurity = "(HLT_HISinglePhoton40_Eta1p5_v2)";
    std::string triggerMC_forPurity_pp = "(HLT_HISinglePhoton40_Eta1p5ForPPRef_v1)";

    cut_phoHoverE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_phoHoverE];
    cut_phoSigmaIEtaIEta = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_phoSigmaIEtaIEta];
    cut_sumIso = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_sumIso];

    // usePPstyleIso = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].i[CUTS::PHO::k_usePPstyleIso];
    // // Barrel
    // cut_phoHOverE_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_phoHOverE_EB];
    // cut_pfcIso4_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pfcIso4_EB];
    // cut_pfnIso4_c0_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pfnIso4_c0_EB];
    // cut_pfnIso4_c1_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pfnIso4_c1_EB];
    // cut_pfnIso4_c2_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pfnIso4_c2_EB];
    // cut_pfpIso4_c0_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pfpIso4_c0_EB];
    // cut_pfpIso4_c1_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pfpIso4_c1_EB];
    // // Endcap
    // cut_phoHOverE_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_phoHOverE_EE];
    // cut_pfcIso4_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pfcIso4_EE];
    // cut_pfnIso4_c0_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pfnIso4_c0_EE];
    // cut_pfnIso4_c1_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pfnIso4_c1_EE];
    // cut_pfnIso4_c2_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pfnIso4_c2_EE];
    // cut_pfpIso4_c0_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pfpIso4_c0_EE];
    // cut_pfpIso4_c1_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pfpIso4_c1_EE];

    jetCollection = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kJET].s[CUTS::JET::k_jetCollection];
    cut_jetpt  = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kJET].f[CUTS::JET::k_pt];
    cut_jeteta = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kJET].f[CUTS::JET::k_eta];
    cut_jetID  = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kJET].i[CUTS::JET::k_jetID];

    cut_awayRange = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kGAMMAJET].f[CUTS::GJT::k_awayRange];
    cut_awayRange_lt = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kGAMMAJET].f[CUTS::GJT::k_awayRange_lt];
    cut_dR = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kGAMMAJET].f[CUTS::GJT::k_dR];

    useUncorrectedPhotonEnergy = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].i[CUTS::PHO::k_useUncorrectedPhotonEnergy];
    doElectronRejection = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].i[CUTS::PHO::k_doElectronRejection];
    // nEventsToMix = configCuts.proc[CUTS::kSKIM].obj[CUTS::kGAMMAJET].i[CUTS::GJT::k_nEventsToMix];
    // nSmear = configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].i[CUTS::JET::k_nSmear];

    if (eventWeight.size() == 0) eventWeight = "1";
    if (cut_awayRange_lt == 0) cut_awayRange_lt = 1;

    int nBins_pt = bins_pt[0].size();         // assume <myvector>[0] and <myvector>[1] have the same size.
    int nBins_hiBin = bins_hiBin[0].size();     // assume <myvector>[0] and <myvector>[1] have the same size.
    // verbose about cut configuration
    std::cout << "Cut Configuration :" << std::endl;
    std::cout << "nBins_pt = " << nBins_pt << std::endl;
    for (int i=0; i<nBins_pt; ++i) {
        std::cout << Form("bins_pt[%d] = [%.1f, %.1f)", i, bins_pt[0][i], bins_pt[1][i]) << std::endl;
    }
    std::cout << "nBins_hiBin = " << nBins_hiBin << std::endl;
    for (int i=0; i<nBins_hiBin; ++i) {
        std::cout << Form("bins_hiBin[%d] = [%d, %d)", i, bins_hiBin[0][i], bins_hiBin[1][i]) << std::endl;
    }

    std::cout << "doEventWeight = " << doEventWeight << std::endl;
    if (doEventWeight > 0) {
        std::cout << "eventWeight = " << eventWeight.c_str() << std::endl;
    }

    std::cout << "trigger    = " << trigger.c_str() << std::endl;

    // if (isHI || !usePPstyleIso) {
        std::cout << "cut_phoHoverE             = " << cut_phoHoverE << std::endl;
        std::cout << "cut_phoSigmaIEtaIEta      = " << cut_phoSigmaIEtaIEta << std::endl;
        std::cout << "cut_sumIso                = " << cut_sumIso << std::endl;
    // }
    // else {
    //     std::cout << "pp style isolation" << std::endl;
    //     std::cout << "Barrel :" << std::endl;
    //     std::cout << "cut_phoHOverE_EB        = " << cut_phoHOverE_EB << std::endl;
    //     std::cout << "cut_pfcIso4_EB          = " << cut_pfcIso4_EB << std::endl;
    //     std::cout << "cut_pfnIso4_c0_EB       = " << cut_pfnIso4_c0_EB << std::endl;
    //     std::cout << "cut_pfnIso4_c1_EB       = " << cut_pfnIso4_c1_EB << std::endl;
    //     std::cout << "cut_pfnIso4_c2_EB       = " << cut_pfnIso4_c2_EB << std::endl;
    //     std::cout << "cut_pfpIso4_c0_EB       = " << cut_pfpIso4_c0_EB << std::endl;
    //     std::cout << "cut_pfpIso4_c1_EB       = " << cut_pfpIso4_c1_EB << std::endl;

    //     std::cout << "Endcap :" << std::endl;
    //     std::cout << "cut_phoHOverE_EE        = " << cut_phoHOverE_EE << std::endl;
    //     std::cout << "cut_pfcIso4_EE          = " << cut_pfcIso4_EE << std::endl;
    //     std::cout << "cut_pfnIso4_c0_EE       = " << cut_pfnIso4_c0_EE << std::endl;
    //     std::cout << "cut_pfnIso4_c1_EE       = " << cut_pfnIso4_c1_EE << std::endl;
    //     std::cout << "cut_pfnIso4_c2_EE       = " << cut_pfnIso4_c2_EE << std::endl;
    //     std::cout << "cut_pfpIso4_c0_EE       = " << cut_pfpIso4_c0_EE << std::endl;
    //     std::cout << "cut_pfpIso4_c1_EE       = " << cut_pfpIso4_c1_EE << std::endl;
    // }

    std::cout << "jetCollection             = " << jetCollection.c_str() << std::endl;
    std::cout << "cut_jetpt                 = " << cut_jetpt << std::endl;
    std::cout << "cut_jeteta                = " << cut_jeteta << std::endl;
    std::cout << "cut_jetID                 = " << cut_jetID << std::endl;

    std::cout << "cut_awayRange             = " << cut_awayRange << " * PI" << std::endl;
    std::cout << "cut_awayRange_lt          = " << cut_awayRange_lt << " * PI" << std::endl;
    std::cout << "cut_dR                    = " << cut_dR << std::endl;

    // std::cout << "nEventsToMix              = " << nEventsToMix << std::endl;

    // set the actual awayRange cut
    cut_awayRange = cut_awayRange * TMath::Pi();
    cut_awayRange_lt = cut_awayRange_lt * TMath::Pi();

    /// Input Bookkeeping block ///
    TFile *input = TFile::Open(inputFile);
    TTree *tHlt = (TTree*)input->Get("hltTree");
    TTree *tPho = (TTree*)input->Get("EventTree");    // photons
    tPho->SetBranchStatus("*", 0);
    tPho->SetBranchStatus("phoEtCorrected", 1);
    tPho->SetBranchStatus("phoEta", 1);
    tPho->SetBranchStatus("phoPhi", 1);
    tPho->SetBranchStatus("phoSigmaIEtaIEta_2012", 1);
    tPho->SetBranchStatus("pho_ecalClusterIsoR4", 1);
    tPho->SetBranchStatus("pho_hcalRechitIsoR4", 1);
    tPho->SetBranchStatus("pho_trackIsoR4PtCut20", 1);
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
    TTree *tJet = (TTree*)input->Get(jetCollection.c_str());
    tJet->SetBranchStatus("*", 0);
    tJet->SetBranchStatus("nref", 1);
    tJet->SetBranchStatus("jtpt", 1);
    tJet->SetBranchStatus("jtpt_smeared_0_30", 1);
    tJet->SetBranchStatus("jtpt_smeared_30_100", 1);
    tJet->SetBranchStatus("jtpt_smeared_0_10", 1);
    tJet->SetBranchStatus("jtpt_smeared_10_30", 1);
    tJet->SetBranchStatus("jtpt_smeared_30_50", 1);
    tJet->SetBranchStatus("jtpt_smeared_50_100", 1);
    tJet->SetBranchStatus("jtpt_smeared_sys", 1);
    //tJet->SetBranchStatus("jteta", 1);

    TTree *tgj[nSmearBins];
    for (int smearingBinIndex = 0; smearingBinIndex<nSmearBins; smearingBinIndex++) {
        if (smearingBinIndex == 0) {
            tgj[smearingBinIndex] = (TTree*)input->Get(Form("gamma_%s", jetCollection.c_str()));
        } else {
            tgj[smearingBinIndex] = (TTree*)input->Get(Form("gamma_%s_smearBin%i", jetCollection.c_str(), smearingBinIndex-1));
        }
    }
    TTree *tHiEvt = (TTree*)input->Get("HiEvt");       // HiEvt tree will be placed in PP forest as well.

    if (!tJet) {
        std::cout << "following jet collection is not found in the input file : " << jetCollection.c_str() << std::endl;
        std::cout << "exiting" << std::endl;
        return;
    }

    GammaJet gj[nSmearBins];
    for (int smearingBinIndex = 0; smearingBinIndex < nSmearBins; smearingBinIndex++) {
        if (tgj[smearingBinIndex]) {
            gj[smearingBinIndex].setupGammaJetTree(tgj[smearingBinIndex]);
        }
    }
    ggHiNtuplizer pho;
    pho.setupTreeForReading(tPho);
    Jets jet;
    jet.setupTreeForReading(tJet);
    hiEvt evt;
    evt.setupTreeForReading(tHiEvt);
    Int_t HLT_HISinglePhoton40_Eta1p5_v1, HLT_HISinglePhoton40_Eta1p5_v2;
    Int_t HLT_HISinglePhoton50_Eta3p1_v1, HLT_HISinglePhoton50_Eta3p1_v2;
    Int_t HLT_HISinglePhoton40_Eta1p5ForPPRef_v1;
    tHlt->SetBranchStatus("*", 0);
    tHlt->SetBranchStatus("HLT_HISinglePhoton40_Eta1p5_v1", 1);
    tHlt->SetBranchStatus("HLT_HISinglePhoton40_Eta1p5_v2", 1);
    tHlt->SetBranchStatus("HLT_HISinglePhoton50_Eta3p1_v1", 1);
    tHlt->SetBranchStatus("HLT_HISinglePhoton50_Eta3p1_v2", 1);
    tHlt->SetBranchStatus("HLT_HISinglePhoton40_Eta1p5ForPPRef_v1", 1);
    tHlt->SetBranchAddress("HLT_HISinglePhoton40_Eta1p5_v1", &HLT_HISinglePhoton40_Eta1p5_v1);
    tHlt->SetBranchAddress("HLT_HISinglePhoton40_Eta1p5_v2", &HLT_HISinglePhoton40_Eta1p5_v2);
    tHlt->SetBranchAddress("HLT_HISinglePhoton50_Eta3p1_v1", &HLT_HISinglePhoton50_Eta3p1_v1);
    tHlt->SetBranchAddress("HLT_HISinglePhoton50_Eta3p1_v2", &HLT_HISinglePhoton50_Eta3p1_v2);
    tHlt->SetBranchAddress("HLT_HISinglePhoton40_Eta1p5ForPPRef_v1", &HLT_HISinglePhoton40_Eta1p5ForPPRef_v1);

    TTree *tJetMB;
    TTree *tgjMB;
    // check the existence of HI specific trees in "gammaJetSkim.root" file
    bool hasJetsMB = false;
    bool hasGammaJetMB = false;
    if (isHI) {
        input->GetObject(Form("%sMB", jetCollection.c_str()), tJetMB);
        input->GetObject(Form("gamma_%sMB", jetCollection.c_str()), tgjMB);

        if (tJetMB) hasJetsMB = true;
        if (tgjMB)  hasGammaJetMB = true;
    } else {
        tJetMB = 0;
        tgjMB  = 0;
    }

    Jets jetMB;
    GammaJet gjMB;
    if (hasJetsMB && hasGammaJetMB) {
        tJetMB->SetBranchStatus("*", 0);
        tJetMB->SetBranchStatus("jtpt", 1);
        tJetMB->SetBranchStatus("nref", 1);
        // tJetMB->SetBranchStatus("jteta", 1);

        jetMB.setupTreeForReading(tJetMB);
        gjMB.setupGammaJetTree(tgjMB);
    }

    /// End Input Bookkeeping block //

    TFile* output = TFile::Open(outputFile, "RECREATE");
    // histograms will be put under a directory whose name is the type of the collision
    if (!output->GetKey(collisionName)) output->mkdir(collisionName, Form("input file is %s", inputFile.Data()));
    output->cd(collisionName);
    std::cout << "histograms will be put under directory : " << collisionName << std::endl;

    TTree *configTree = setupConfigurationTreeForWriting(configCuts);

    int nCorrHist = correlationHistNames.size();
    correlationHist corrHists[nCorrHist][nBins_pt][nBins_hiBin];

    // prepare histogram names for xjg, abs(dphi) and jet pt
    // if the collision is not HI, then cannot split it into hiBins.
    // if (!isHI) nBins_hiBin = 1;
    for (int iHist=0; iHist<nCorrHist; ++iHist) {
        for (int i=0; i<nBins_pt; ++i) {
            for (int j=0; j<nBins_hiBin; ++j) {
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
    for (int i=0; i<nBins_pt; ++i) {
        for (int j=0; j<nBins_hiBin; ++j) {
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
    std::cout << "tgj->GetEntries() = " << tgj[0]->GetEntries() << std::endl;
    if (trigger.compare("") != 0 && !isMC) {
        std::cout << "tgj->GetEntries(trigger==1) = " << tHlt->GetEntries(Form("%s == 1", trigger.c_str())) << std::endl;
    } else {
        std::cout << "tgj->GetEntries(trigger==1) is skipped because either no trigger is specified or the data is coming from MC." << std::endl;
    }
    std::cout << "####################" << std::endl;

    long long nentries = tgj[0]->GetEntries();
    long long firstEntry = 0;
    long long lastEntry = nentries;
    std::cout << "Total Entries: " << nentries << std::endl;
    if (nJobs != -1) {
        if (jobNum >= nJobs) {
            std::cout << "jobNum > nJobs, invalid configuration, aborting" << std::endl;
            return;
        }
        firstEntry = floor(nentries/nJobs)*jobNum;
        lastEntry = floor(nentries/nJobs)*(jobNum+1);
        if (jobNum == nJobs-1) {
            lastEntry = nentries;
        }
        std::cout << "For this job " << jobNum << std::endl;
        std::cout << "First Entry: " << firstEntry << std::endl;
        std::cout << "Final Entry: " << lastEntry << std::endl;
    }
    for (long long jentry = firstEntry; jentry < lastEntry; jentry++) {
        if (jentry % 2000 == 0 && nJobs == -1) {
            // std::cout << "current entry = " << jentry << " out of " << nentries << " : " << std::setprecision(2) << (double)jentry/nentries*100 << " %" << std::endl;
            printf("current entry = %lli out of %lli : %3f%%\n", jentry, nentries, (double)jentry/nentries*100);
        }

        tHlt->GetEntry(jentry);
        // event selection
        if (!isMC && !HLT_HISinglePhoton40_Eta1p5_v1) continue;
        if (isHI && isMC && !HLT_HISinglePhoton40_Eta1p5_v2) continue;
        if (!isHI && isMC && !HLT_HISinglePhoton40_Eta1p5ForPPRef_v1) continue;

        tPho->GetEntry(jentry);
        tgj[0]->GetEntry(jentry);

        // eta cut moved to skim step
        // eta cut
        // if (TMath::Abs((*pho.phoEta)[gj[0].phoIdx]) > 1.44) continue;

        // noise cut moved to skim step
        // // noise cut
        if (((*pho.phoE3x3)[gj[0].phoIdx]/(*pho.phoE5x5)[gj[0].phoIdx] > 2./3.-0.03 &&
             (*pho.phoE3x3)[gj[0].phoIdx]/(*pho.phoE5x5)[gj[0].phoIdx] < 2./3.+0.03) &&
            ((*pho.phoE1x5)[gj[0].phoIdx]/(*pho.phoE5x5)[gj[0].phoIdx] > 1./3.-0.03 &&
             (*pho.phoE1x5)[gj[0].phoIdx]/(*pho.phoE5x5)[gj[0].phoIdx] < 1./3.+0.03) &&
            ((*pho.phoE2x5)[gj[0].phoIdx]/(*pho.phoE5x5)[gj[0].phoIdx] > 2./3.-0.03 &&
             (*pho.phoE2x5)[gj[0].phoIdx]/(*pho.phoE5x5)[gj[0].phoIdx] < 2./3.+0.03)) continue;

        // isolation cut
        if (((*pho.pho_ecalClusterIsoR4)[gj[0].phoIdx] +
             (*pho.pho_hcalRechitIsoR4)[gj[0].phoIdx] +
             (*pho.pho_trackIsoR4PtCut20)[gj[0].phoIdx]) > cut_sumIso) continue;
        if ((*pho.phoHoverE)[gj[0].phoIdx] > 0.1) continue;

        bool isSignalPho = ((*pho.phoSigmaIEtaIEta_2012)[gj[0].phoIdx] < cut_phoSigmaIEtaIEta);
        bool isBkgPho = ((*pho.phoSigmaIEtaIEta_2012)[gj[0].phoIdx] > 0.011 &&
                         (*pho.phoSigmaIEtaIEta_2012)[gj[0].phoIdx] < 0.017);

        if (!(isSignalPho || isBkgPho)) continue;

        int phoType = (isSignalPho ? CORR::kRAW : CORR::kBKG);

        // reco electron rejection part
        bool isEle = false;
        if (doElectronRejection) {
            float eleEpTemp = 100.0;
            for(int ie=0; ie<pho.nEle; ++ie){
                if ( (*pho.elePt)[ie] < 10 ) continue;
                if ( abs( (*pho.eleEta)[ie] - (*pho.phoEta)[gj[0].phoIdx] ) > 0.03 ) continue;
                double dphi = getDPHI((*pho.elePhi)[ie], (*pho.phoPhi)[gj[0].phoIdx]);
                //float dphi = pho.elePhi->at(ie) - (*pho.phoPhi)[gj[0].phoIdx];
                //if ( dphi >  3.141592 ) dphi = dphi - 2* 3.141592;
                //if ( dphi < -3.141592 ) dphi = dphi + 2* 3.141592;
                if ( abs(dphi) > 0.03 )  continue;
                if ( eleEpTemp < pho.eleEoverP->at(ie) )  continue;
                eleEpTemp = pho.eleEoverP->at(ie);
                isEle = true;
            }
        }
        if (isEle) continue;

        tJet->GetEntry(jentry);
        tHiEvt->GetEntry(jentry);

        float weight = 1;
        if (isMC) {
            weight = evt.weight;
        }

        float phoEt = (*pho.phoEtCorrected)[gj[0].phoIdx];
        if (useUncorrectedPhotonEnergy)
            phoEt = (*pho.phoEt)[gj[0].phoIdx];

        // handle nEntriesPho separate from jet loop
        for (int i=0; i<nBins_pt; ++i) {
            if (phoEt <  bins_pt[0][i] ||
                phoEt >= bins_pt[1][i]) continue;
            for (int j=0; j<nBins_hiBin; ++j) {
                if (isHI) {
                    if (evt.hiBin <  bins_hiBin[0][j] ||
                        evt.hiBin >= bins_hiBin[1][j]) continue;
                }
                for (int iHist = 0; iHist < nCorrHist; iHist++) {
                    corrHists[iHist][i][j].nEntriesPho[phoType][CORR::kRAW] += weight;
                }
            }
        }

        bool gotEntry[nSmearBins] = {false}; // only call getEntry once per event for smear trees
        gotEntry[0] = true;
        for (int ijet = 0; ijet < jet.nref; ijet++) {
            // jet cuts
            // jteta cut moved to skim
            // if (TMath::Abs(jet.jteta[ijet]) > cut_jeteta) continue;
            if ((*gj[0].jetID)[ijet] < cut_jetID) continue;

            for (int j=0; j<nBins_hiBin; ++j) {
                int smearingBinIndex = 0;
                if (isHI) {
                    if (evt.hiBin <  bins_hiBin[0][j] ||
                        evt.hiBin >= bins_hiBin[1][j]) continue;
                } else {
                    if (!gotEntry[j]) {
                        tgj[j]->GetEntry(jentry);
                        gotEntry[j] = true;
                    }
                    smearingBinIndex = j;
                }

                if (smeared_jtpt(jet, ijet, smearingBinIndex) < cut_jetpt) continue;
                if ((*gj[smearingBinIndex].dR)[ijet] < cut_dR) continue;

                for (int i=0; i<nBins_pt; ++i) {
                    if (phoEt <  bins_pt[0][i] ||
                        phoEt >= bins_pt[1][i]) continue;

                    // fill histograms
                    // dphi = 1
                    corrHists[1][i][j].h1D[phoType][CORR::kRAW]->Fill(TMath::Abs((*gj[smearingBinIndex].dphi)[ijet]), weight);
                    corrHists[1][i][j].nEntries[phoType][CORR::kRAW] += weight;
                    //apply dphi cuts now
                    if (TMath::Abs((*gj[smearingBinIndex].dphi)[ijet]) <= cut_awayRange) continue;
                    // xjg = 0
                    // jtpt = 2
                    float xjg = (*gj[smearingBinIndex].xjgCorrected)[ijet];
                    if (useUncorrectedPhotonEnergy)
                        xjg = (*gj[smearingBinIndex].xjg)[ijet];
                    corrHists[0][i][j].h1D[phoType][CORR::kRAW]->Fill(xjg, weight);
                    corrHists[0][i][j].nEntries[phoType][CORR::kRAW] += weight;
                    corrHists[2][i][j].h1D[phoType][CORR::kRAW]->Fill(smeared_jtpt(jet, ijet, smearingBinIndex), weight);
                    corrHists[2][i][j].nEntries[phoType][CORR::kRAW] += weight;
                }
            }
        }

        if (dphi_check && isHI) {
            for (int ijet = 0; ijet < jet.nref; ijet++) {
                for (int j=0; j<nBins_hiBin; ++j) {
                    if (evt.hiBin <  bins_hiBin[0][j] ||
                        evt.hiBin >= bins_hiBin[1][j]) continue;

                    if (jet.jtpt[ijet] < cut_jetpt) continue;
                    if ((*gj[0].dR)[ijet] < cut_dR) continue;

                    for (int i=0; i<nBins_pt; ++i) {
                        if ((*pho.phoEtCorrected)[gj[0].phoIdx] <  bins_pt[0][i] ||
                            (*pho.phoEtCorrected)[gj[0].phoIdx] >= bins_pt[1][i]) continue;

                        float lower_sideband = 1;
                        float upper_sideband = TMath::Pi()/2;
                        float sideband_width = upper_sideband - lower_sideband;

                        // select dphi sideband region
                        if (TMath::Abs((*gj[0].dphi)[ijet]) > upper_sideband || TMath::Abs((*gj[0].dphi)[ijet]) < lower_sideband) continue;

                        corrHists[1][i][j].h1D[phoType][CORR::kBKG]->Fill((TMath::Abs((*gj[0].dphi)[ijet])-lower_sideband)*(TMath::Pi()/sideband_width), weight*40*(TMath::Pi()/sideband_width));
                        corrHists[1][i][j].nEntries[phoType][CORR::kBKG] += weight*40*(TMath::Pi()/sideband_width);

                        corrHists[0][i][j].h1D[phoType][CORR::kBKG]->Fill((*gj[0].xjgCorrected)[ijet], weight*40*(TMath::Pi()/8/sideband_width));
                        corrHists[0][i][j].nEntries[phoType][CORR::kBKG] += weight*40*(TMath::Pi()/8/sideband_width);
                        corrHists[2][i][j].h1D[phoType][CORR::kBKG]->Fill(jet.jtpt[ijet], weight*40*(TMath::Pi()/8/sideband_width));
                        corrHists[2][i][j].nEntries[phoType][CORR::kBKG] += weight*40*(TMath::Pi()/8/sideband_width);
                    }
                }
            }
        } else {
            if (hasJetsMB && hasGammaJetMB) {
                tJetMB->GetEntry(jentry);
                tgjMB->GetEntry(jentry);

                for (int ijet = 0; ijet < jetMB.nref; ijet++) {
                    // jet cuts
                    if ((*gjMB.dR)[ijet] < cut_dR) continue;
                    // jteta cut moved to skim
                    // if (TMath::Abs(jetMB.jteta[ijet]) > cut_jeteta) continue;
                    if (smeared_jtpt(jetMB, ijet, 0) < cut_jetpt) continue;
                    if ((*gjMB.jetID)[ijet] < cut_jetID) continue;

                    for (int i=0; i<nBins_pt; ++i) {
                        if ((*pho.phoEtCorrected)[gjMB.phoIdx] <  bins_pt[0][i] ||
                            (*pho.phoEtCorrected)[gjMB.phoIdx] >= bins_pt[1][i]) continue;
                        for (int j=0; j<nBins_hiBin; ++j) {
                            if (evt.hiBin <  bins_hiBin[0][j] ||
                                evt.hiBin >= bins_hiBin[1][j]) continue;

                            // fill histograms
                            // dphi = 1
                            corrHists[1][i][j].h1D[phoType][CORR::kBKG]->Fill(TMath::Abs((*gjMB.dphi)[ijet]), weight);
                            corrHists[1][i][j].nEntries[phoType][CORR::kBKG] += weight;
                            // apply dphi cuts now
                            if (TMath::Abs((*gjMB.dphi)[ijet]) <= cut_awayRange) continue;
                            // xjg = 0
                            // jtpt = 2
                            float xjg = (*gjMB.xjgCorrected)[ijet];
                            if (useUncorrectedPhotonEnergy)
                                xjg = (*gjMB.xjg)[ijet];

                            corrHists[0][i][j].h1D[phoType][CORR::kBKG]->Fill(xjg, weight);
                            corrHists[0][i][j].nEntries[phoType][CORR::kBKG] += weight;
                            corrHists[2][i][j].h1D[phoType][CORR::kBKG]->Fill(smeared_jtpt(jetMB, ijet, 0), weight);
                            corrHists[2][i][j].nEntries[phoType][CORR::kBKG] += weight;
                        }
                    }
                }
            }
        }
    }

    /// Histogram arithmetic (no reading I/O)
    for (int iHist = 0; iHist < nCorrHist; iHist++) {
        for (int i=0; i<nBins_pt; ++i) {
            for (int j=0; j<nBins_hiBin; ++j) {
                corrHists[iHist][i][j].nEntriesPho[CORR::kRAW][CORR::kBKG] = corrHists[iHist][i][j].nEntriesPho[CORR::kRAW][CORR::kRAW];
                corrHists[iHist][i][j].nEntriesPho[CORR::kBKG][CORR::kBKG] = corrHists[iHist][i][j].nEntriesPho[CORR::kBKG][CORR::kRAW];

                if (h_nPho[i][j][CORR::kRAW]->GetBinContent(1) == 0) {
                    h_nPho[i][j][CORR::kRAW]->SetBinContent(1, corrHists[iHist][i][j].nEntriesPho[CORR::kRAW][CORR::kRAW]);
                    h_nPho[i][j][CORR::kRAW]->Write("", TObject::kOverwrite);
                }

                if (h_nPho[i][j][CORR::kBKG]->GetBinContent(1) == 0) {
                    h_nPho[i][j][CORR::kBKG]->SetBinContent(1, corrHists[iHist][i][j].nEntriesPho[CORR::kBKG][CORR::kRAW]);
                    h_nPho[i][j][CORR::kBKG]->Write("", TObject::kOverwrite);
                }

                std::cout << "nEntries[CORR::kRAW][CORR::kRAW] = " << corrHists[iHist][i][j].nEntries[CORR::kRAW][CORR::kRAW] << std::endl;
                std::cout << "nEntries[CORR::kBKG][CORR::kRAW] = " << corrHists[iHist][i][j].nEntries[CORR::kBKG][CORR::kRAW] << std::endl;
                std::cout << "nEntriesPho[CORR::kRAW][CORR::kRAW] = " << corrHists[iHist][i][j].nEntriesPho[CORR::kRAW][CORR::kRAW] << std::endl;
                std::cout << "nEntriesPho[CORR::kBKG][CORR::kRAW] = " << corrHists[iHist][i][j].nEntriesPho[CORR::kBKG][CORR::kRAW] << std::endl;

                std::string histoTitle;
                // if (isHI) {
                    histoTitle = Form("%s , %.0f < p^{#gamma}_{T} < %.0f GeV/c, %d-%d %% ", collisionName , bins_pt[0][i], bins_pt[1][i], bins_hiBin[0][j]/2, bins_hiBin[1][j]/2);

                    // special cases
                    if (bins_pt[0][i] <= 0 && bins_pt[1][i] < 0 && bins_hiBin[0][j] <= 0 && bins_hiBin[1][j] >= 200) {
                        histoTitle = Form("%s", collisionName );
                    }
                    else if (bins_pt[0][i] <= 0 && bins_pt[1][i] < 0) {
                        histoTitle = Form("%s , %d-%d %%", collisionName , bins_hiBin[0][j]/2, bins_hiBin[1][j]/2);
                    }
                    else if (bins_pt[1][i] < 0 && bins_hiBin[0][j] <= 0 && bins_hiBin[1][j] >= 200) {
                        histoTitle = Form("%s , p^{#gamma}_{T} > %.0f GeV/c", collisionName , bins_pt[0][i]);
                    }
                    else if (bins_hiBin[0][j] <= 0 && bins_hiBin[1][j] >= 200) {
                        histoTitle = Form("%s , %.0f < p^{#gamma}_{T} < %.0f GeV/c", collisionName , bins_pt[0][i], bins_pt[1][i]);
                    }
                    else if (bins_pt[1][i] < 0) {
                        histoTitle = Form("%s , p^{#gamma}_{T} > %.0f GeV/c, %d-%d %% ", collisionName , bins_pt[0][i], bins_hiBin[0][j]/2, bins_hiBin[1][j]/2);
                    }
                // } else {
                //     histoTitle = Form("%s , %.0f < p^{#gamma}_{T} < %.0f GeV/c", collisionName , bins_pt[0][i], bins_pt[1][i]);

                //     // special cases
                //     if (bins_pt[0][i] <= 0 && bins_pt[1][i] < 0) {
                //         histoTitle = Form("%s", collisionName );
                //     }
                //     else if (bins_pt[1][i] < 0) {
                //         histoTitle = Form("%s , p^{#gamma}_{T} > %.0f GeV/c", collisionName , bins_pt[0][i]);
                //     }
                // }

                // histograms for RAW and BKG regions
                for (int iCorr = 0; iCorr < CORR::kN_CORRFNC-1; ++iCorr) {
                    for (int jCorr = 0; jCorr < CORR::kN_CORRFNC-1; ++jCorr) {
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
}

int main(int argc, char** argv)
{
    if (argc == 6) {
        gammaJetHistogram(argv[1], argv[2], argv[3], atoi(argv[4]), atoi(argv[5]));
        return 0;
    } else if (argc == 4) {
        gammaJetHistogram(argv[1], argv[2], argv[3]);
        return 0;
    } else {
        std::cout << "Usage : \n" <<
            "./gammaJetHistogram.exe <configFile> <inputFile> <outputFile>"
                  << std::endl;
        return 1;
    }
}
