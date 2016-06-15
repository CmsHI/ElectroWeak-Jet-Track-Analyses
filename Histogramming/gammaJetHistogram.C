#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TCut.h>
#include <TH1D.h>
#include <TMath.h>

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
// #include <ctime>

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
#include "PhotonPurity.h"

const std::vector<std::string> correlationHistNames   {"xjg", "dphi", "ptJet"};
const std::vector<std::string> correlationHistTitleX  {"p^{Jet}_{T}/p^{#gamma}_{T}", "#Delta#phi_{J#gamma}", "p^{Jet}_{T}"};
const std::vector<std::string> correlationHistTitleY_final_normalized{"#frac{1}{N_{#gamma}} #frac{dN_{J#gamma}}{dx_{J#gamma}}",
        "#frac{1}{N_{#gamma}} #frac{dN_{J#gamma}}{d#Delta#phi}",
        "#frac{1}{N_{#gamma}} #frac{dN_{J#gamma}}{dp^{Jet}_{T}}"};
const std::vector<int>         nBinsx{16, 20,          30};
const std::vector<double>      xlow  {0,  0,           0};
const std::vector<double>      xup   {2,  TMath::Pi(), 300};
const std::vector<double>      xlow_final{0,  0,           0};
const std::vector<double>      xup_final {2,  TMath::Pi(), 200};

const int nSmearBins = 8;

float smeared_jtpt(Jets j, int ijet, int smearingBinIndex){
    switch(smearingBinIndex){
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

void gammaJetHistogram(const TString configFile, const TString inputFile, const TString outputFile = "gammaJetHistogram.root");

void gammaJetHistogram(const TString configFile, const TString inputFile, const TString outputFile)
{
    TH1::SetDefaultSumw2();

    std::cout<<"running gammaJetHistogram()"<<std::endl;
    std::cout<<"configFile  = "<< configFile.Data() <<std::endl;
    std::cout<<"inputFile   = "<< inputFile.Data() <<std::endl;
    std::cout<<"outputFile  = "<< outputFile.Data() <<std::endl;

    InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());
    if(!configCuts.isValid){
        std::cout << "Invalid configfile, terminating." << std::endl;
        return;
    }

    // input configuration
    int collision;
    collision = configInput.proc[INPUT::kHISTOGRAM].i[INPUT::k_collisionType];
    // verbose about input configuration
    std::cout<<"Input Configuration :"<<std::endl;
    const char* collisionName =  getCollisionTypeName((COLL::TYPE)collision).c_str();
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
    int nEventsToMix;
    int nSmear;
    bins_pt[0] = ConfigurationParser::ParseListFloat(
        configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_pt_gt]);
    bins_pt[1] = ConfigurationParser::ParseListFloat(
        configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_pt_lt]);
    bins_hiBin[0] = ConfigurationParser::ParseListInteger(
        configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_gt]);
    bins_hiBin[1] = ConfigurationParser::ParseListInteger(
        configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_lt]);

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

    nEventsToMix = configCuts.proc[CUTS::kSKIM].obj[CUTS::kGAMMAJET].i[CUTS::GJT::k_nEventsToMix];
    nSmear = configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].i[CUTS::JET::k_nSmear];

    if (eventWeight.size() == 0) eventWeight = "1";
    if (cut_awayRange_lt == 0) cut_awayRange_lt = 1;

    int nBins_pt = bins_pt[0].size();         // assume <myvector>[0] and <myvector>[1] have the same size.
    int nBins_hiBin = bins_hiBin[0].size();     // assume <myvector>[0] and <myvector>[1] have the same size.
    // verbose about cut configuration
    std::cout<<"Cut Configuration :"<<std::endl;
    std::cout << "nBins_pt = " << nBins_pt << std::endl;
    for (int i=0; i<nBins_pt; ++i) {
        std::cout << Form("bins_pt[%d] = [%.1f, %.1f)", i, bins_pt[0][i], bins_pt[1][i]) << std::endl;
    }
    std::cout << "nBins_hiBin = " << nBins_hiBin << std::endl;
    for (int i=0; i<nBins_hiBin; ++i) {
        std::cout << Form("bins_hiBin[%d] = [%d, %d)", i, bins_hiBin[0][i], bins_hiBin[1][i]) << std::endl;
    }

    std::cout<<"doEventWeight = "<<doEventWeight<<std::endl;
    if (doEventWeight > 0) {
        std::cout<<"eventWeight = "<<eventWeight.c_str()<<std::endl;
    }

    std::cout<<"trigger    = "<<trigger.c_str()<<std::endl;

    // if (isHI || !usePPstyleIso) {
        std::cout<<"cut_phoHoverE             = "<< cut_phoHoverE <<std::endl;
        std::cout<<"cut_phoSigmaIEtaIEta      = "<< cut_phoSigmaIEtaIEta <<std::endl;
        std::cout<<"cut_sumIso                = "<< cut_sumIso <<std::endl;
    // }
    // else {
    //     std::cout << "pp style isolation" << std::endl;
    //     std::cout<<"Barrel :"<<std::endl;
    //     std::cout<<"cut_phoHOverE_EB        = "<< cut_phoHOverE_EB <<std::endl;
    //     std::cout<<"cut_pfcIso4_EB          = "<< cut_pfcIso4_EB <<std::endl;
    //     std::cout<<"cut_pfnIso4_c0_EB       = "<< cut_pfnIso4_c0_EB <<std::endl;
    //     std::cout<<"cut_pfnIso4_c1_EB       = "<< cut_pfnIso4_c1_EB <<std::endl;
    //     std::cout<<"cut_pfnIso4_c2_EB       = "<< cut_pfnIso4_c2_EB <<std::endl;
    //     std::cout<<"cut_pfpIso4_c0_EB       = "<< cut_pfpIso4_c0_EB <<std::endl;
    //     std::cout<<"cut_pfpIso4_c1_EB       = "<< cut_pfpIso4_c1_EB <<std::endl;

    //     std::cout<<"Endcap :"<<std::endl;
    //     std::cout<<"cut_phoHOverE_EE        = "<< cut_phoHOverE_EE <<std::endl;
    //     std::cout<<"cut_pfcIso4_EE          = "<< cut_pfcIso4_EE <<std::endl;
    //     std::cout<<"cut_pfnIso4_c0_EE       = "<< cut_pfnIso4_c0_EE <<std::endl;
    //     std::cout<<"cut_pfnIso4_c1_EE       = "<< cut_pfnIso4_c1_EE <<std::endl;
    //     std::cout<<"cut_pfnIso4_c2_EE       = "<< cut_pfnIso4_c2_EE <<std::endl;
    //     std::cout<<"cut_pfpIso4_c0_EE       = "<< cut_pfpIso4_c0_EE <<std::endl;
    //     std::cout<<"cut_pfpIso4_c1_EE       = "<< cut_pfpIso4_c1_EE <<std::endl;
    // }

    std::cout<<"jetCollection             = "<< jetCollection.c_str() <<std::endl;
    std::cout<<"cut_jetpt                 = "<< cut_jetpt <<std::endl;
    std::cout<<"cut_jeteta                = "<< cut_jeteta <<std::endl;
    std::cout<<"cut_jetID                 = "<< cut_jetID <<std::endl;

    std::cout<<"cut_awayRange             = "<< cut_awayRange << " * PI" <<std::endl;
    std::cout<<"cut_awayRange_lt          = "<< cut_awayRange_lt << " * PI" <<std::endl;
    std::cout<<"cut_dR                    = "<< cut_dR <<std::endl;

    std::cout<<"nEventsToMix              = "<< nEventsToMix <<std::endl;

    //set the actual awayRange cut
    cut_awayRange = cut_awayRange * TMath::Pi();
    cut_awayRange_lt = cut_awayRange_lt * TMath::Pi();

    /// Input Bookkeeping block ///
    TFile *input = TFile::Open(inputFile);
    TTree *tHlt = (TTree*)input->Get("hltTree");
    TTree *tPho = (TTree*)input->Get("EventTree");    // photons
    tPho->SetBranchStatus("*",0);
    tPho->SetBranchStatus("phoEt",1);
    tPho->SetBranchStatus("phoEta",1);
    tPho->SetBranchStatus("phoSigmaIEtaIEta_2012",1);
    tPho->SetBranchStatus("pho_ecalClusterIsoR4",1);
    tPho->SetBranchStatus("pho_hcalRechitIsoR4",1);
    tPho->SetBranchStatus("pho_trackIsoR4PtCut20",1);
    tPho->SetBranchStatus("phoHoverE",1);
    // tPho->SetBranchStatus("phoE3x3",1);
    // tPho->SetBranchStatus("phoE5x5",1);
    // tPho->SetBranchStatus("phoE1x5",1);
    // tPho->SetBranchStatus("phoE2x5",1);
    TTree *tJet = (TTree*)input->Get(jetCollection.c_str());
    tJet->SetBranchStatus("*",0);
    tJet->SetBranchStatus("nref",1);
    tJet->SetBranchStatus("jtpt",1);
    tJet->SetBranchStatus("jtpt_smeared_0_30",1);
    tJet->SetBranchStatus("jtpt_smeared_30_100",1);
    tJet->SetBranchStatus("jtpt_smeared_0_10",1);
    tJet->SetBranchStatus("jtpt_smeared_10_30",1);
    tJet->SetBranchStatus("jtpt_smeared_30_50",1);
    tJet->SetBranchStatus("jtpt_smeared_50_100",1);
    tJet->SetBranchStatus("jtpt_smeared_sys",1);
    //tJet->SetBranchStatus("jteta",1);
    TTree *tgj[nSmearBins];
    for(int smearingBinIndex = 0; smearingBinIndex<nSmearBins; smearingBinIndex++)
    {
        if(smearingBinIndex == 0) {
            tgj[smearingBinIndex]  = (TTree*)input->Get(Form("gamma_%s", jetCollection.c_str()));
        } else {
            tgj[smearingBinIndex] = (TTree*)input->Get(Form("gamma_%s_smearBin%i", jetCollection.c_str(), smearingBinIndex-1));
        }
    }
    TTree *tHiEvt = (TTree*)input->Get("HiEvt");       // HiEvt tree will be placed in PP forest as well.

    if (!tJet) {
        std::cout<<"following jet collection is not found in the input file : " << jetCollection.c_str() << std::endl;
        std::cout<<"exiting"<< std::endl;
        return;
    }

    GammaJet gj[nSmearBins];
    for(int smearingBinIndex = 0; smearingBinIndex < nSmearBins; smearingBinIndex++)
    {
        if(tgj[smearingBinIndex]){
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
    tHlt->SetBranchStatus("HLT_HISinglePhoton40_Eta1p5ForPPRef_v1",1);
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
    }
    else {
        tJetMB = 0;
        tgjMB  = 0;
    }

    Jets jetMB;
    GammaJet gjMB;
    if(hasJetsMB && hasGammaJetMB){
        tJetMB->SetBranchStatus("*",0);
        tJetMB->SetBranchStatus("jtpt",1);
        tJetMB->SetBranchStatus("nref",1);
        //tJetMB->SetBranchStatus("jteta",1);

        jetMB.setupTreeForReading(tJetMB);
        gjMB.setupGammaJetTree(tgjMB);
    }

    /// End Input Bookkeeping block //

    /// Purity Calculation Block ///
    // these lists of constants should really be in the conf. Cheat for time for now.
    const float tempPbPbPurity[56] = {0.676347, 0.686488, 0.756964, 0.675387, 0.703763, 0.752848, 0.787882, 0.712814, 0.707078, 0.784051, 0.675911, 0.72968, 0.76778, 0.844154, 0.663561, 0.678369, 0.746681, 0.651645, 0.693355, 0.742194, 0.776277, 0.680703, 0.679884, 0.757597, 0.662675, 0.689962, 0.752508, 0.769309, 0.703061, 0.693244, 0.780901, 0.639828, 0.713101, 0.760176, 0.841252, 0.73165, 0.727308, 0.788082, 0.711999, 0.738944, 0.771933, 0.855741, 0.720931, 0.713083, 0.77815, 0.682542, 0.708191, 0.759067, 0.849453, 0.744423, 0.735621, 0.803791, 0.691025, 0.763615, 0.778766, 0.886822};

    const float tempPbPbMCPurity[56] = {0.676347, 0.686488, 0.756964, 0.675387, 0.703763, 0.752848, 0.787882, 0.712814, 0.707078, 0.784051, 0.675911, 0.72968, 0.76778, 0.844154, 0.663561, 0.678369, 0.746681, 0.651645, 0.693355, 0.742194, 0.776277, 0.680703, 0.679884, 0.757597, 0.662675, 0.689962, 0.752508, 0.769309, 0.703061, 0.693244, 0.780901, 0.639828, 0.713101, 0.760176, 0.841252, 0.73165, 0.727308, 0.788082, 0.711999, 0.738944, 0.771933, 0.855741, 0.720931, 0.713083, 0.77815, 0.682542, 0.708191, 0.759067, 0.849453, 0.744423, 0.735621, 0.803791, 0.691025, 0.763615, 0.778766, 0.886822};

    const float tempPPPurity[56] = {0.676347, 0.686488, 0.756964, 0.675387, 0.703763, 0.752848, 0.787882, 0.712814, 0.707078, 0.784051, 0.675911, 0.72968, 0.76778, 0.844154, 0.663561, 0.678369, 0.746681, 0.651645, 0.693355, 0.742194, 0.776277, 0.680703, 0.679884, 0.757597, 0.662675, 0.689962, 0.752508, 0.769309, 0.703061, 0.693244, 0.780901, 0.639828, 0.713101, 0.760176, 0.841252, 0.73165, 0.727308, 0.788082, 0.711999, 0.738944, 0.771933, 0.855741, 0.720931, 0.713083, 0.77815, 0.682542, 0.708191, 0.759067, 0.849453, 0.744423, 0.735621, 0.803791, 0.691025, 0.763615, 0.778766, 0.886822};

    const float tempPPMCPurity[56] = {0.676347, 0.686488, 0.756964, 0.675387, 0.703763, 0.752848, 0.787882, 0.712814, 0.707078, 0.784051, 0.675911, 0.72968, 0.76778, 0.844154, 0.663561, 0.678369, 0.746681, 0.651645, 0.693355, 0.742194, 0.776277, 0.680703, 0.679884, 0.757597, 0.662675, 0.689962, 0.752508, 0.769309, 0.703061, 0.693244, 0.780901, 0.639828, 0.713101, 0.760176, 0.841252, 0.73165, 0.727308, 0.788082, 0.711999, 0.738944, 0.771933, 0.855741, 0.720931, 0.713083, 0.77815, 0.682542, 0.708191, 0.759067, 0.849453, 0.744423, 0.735621, 0.803791, 0.691025, 0.763615, 0.778766, 0.886822};
    
    double purity[nBins_pt][nBins_hiBin];   // fixed for the moment.
    for (int i = 0; i<nBins_pt; ++i){
        for (int j = 0; j<nBins_hiBin; ++j){
            if(isHI && !isMC){
                purity[i][j] = tempPbPbPurity[i*nBins_hiBin+j];
            } else if (isHI && isMC) {
                purity[i][j] = tempPbPbMCPurity[i*nBins_hiBin+j];
            } else if (!isHI && !isMC) {
                purity[i][j] = tempPPPurity[i*nBins_hiBin+j];
            } else {
                purity[i][j] = tempPPMCPurity[i*nBins_hiBin+j];
            }
        }
    }
    /// End Purity Block ///

    TFile* output = TFile::Open(outputFile, "RECREATE");
    // histograms will be put under a directory whose name is the type of the collision
    if(!output->GetKey(collisionName)) output->mkdir(collisionName, Form("input file is %s", inputFile.Data()));
    output->cd(collisionName);
    std::cout<<"histograms will be put under directory : " << collisionName << std::endl;

    TTree *configTree = setupConfigurationTreeForWriting(configCuts);


    int nCorrHist = correlationHistNames.size();
    correlationHist corrHists[nCorrHist][nBins_pt][nBins_hiBin];

    // prepare histogram names for xjg, abs(dphi) and jet pt
    // if the collision is not HI, then cannot split it into hiBins.
    //if (!isHI) nBins_hiBin = 1;
    for (int iHist=0; iHist<nCorrHist; ++iHist){
        for (int i=0; i<nBins_pt; ++i){
            for(int j=0; j<nBins_hiBin; ++j){
                corrHists[iHist][i][j].name = Form("%s_ptBin%d_hiBin%d", correlationHistNames[iHist].c_str(), i, j);

                for (int iCorr = 0; iCorr < CORR::kN_CORRFNC; ++iCorr) {
                    for (int jCorr = 0; jCorr < CORR::kN_CORRFNC; ++jCorr) {

                        std::string subHistName = Form("%s_ptBin%d_hiBin%d_%s_%s", correlationHistNames[iHist].c_str(), i, j,
                                                       CORR::CORR_PHO_LABELS[iCorr].c_str(), CORR::CORR_JET_LABELS[jCorr].c_str());
                        corrHists[iHist][i][j].h1D_name[iCorr][jCorr] = subHistName.c_str();
                        corrHists[iHist][i][j].h1D[iCorr][jCorr] = new TH1D(Form("h1D_%s", subHistName.c_str()),"",
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
    for (int i=0; i<nBins_pt; ++i){
        for(int j=0; j<nBins_hiBin; ++j){

            std::string histNamePhoRAW = Form("h_nPho_ptBin%d_hiBin%d_%s", i, j, CORR::CORR_PHO_LABELS[CORR::kRAW].c_str());
            h_nPho[i][j][CORR::kRAW] = new TH1D(histNamePhoRAW.c_str(), "", 1, 0, 1);

            std::string histNamePhoBKG = Form("h_nPho_ptBin%d_hiBin%d_%s", i, j, CORR::CORR_PHO_LABELS[CORR::kBKG].c_str());
            h_nPho[i][j][CORR::kBKG] = new TH1D(histNamePhoBKG.c_str(), "", 1, 0, 1);
        }
    }

    // histograms with pt bins on x-axis
    std::vector<std::string> correlationHistNames_ptBinAll = {"rjg", "xjg_mean"};       // histograms where x-axis is pt bins
    int nCorrHist_ptBinAll = correlationHistNames_ptBinAll.size();
    correlationHist corrHists_ptBinAll[nCorrHist_ptBinAll][nBins_hiBin];
    // prepare histogram names for rjg and <xjg>
    const int nBins_rjg = 4;
    const int nBins_xjg_mean= 4;
    double bins_rjg[nBins_rjg+1] =           {40, 50, 60, 80, 120};
    double bins_xjg_mean[nBins_xjg_mean+1] = {40, 50, 60, 80, 120};

    for (int j=0; j<nBins_hiBin; ++j){
        for (int iCorr = 0; iCorr < CORR::kN_CORRFNC; ++iCorr) {
            for (int jCorr = 0; jCorr < CORR::kN_CORRFNC; ++jCorr) {

                // rjg
                std::string subHistName;
                subHistName = Form("%s_ptBinAll_hiBin%d_%s_%s", correlationHistNames_ptBinAll[0].c_str(), j,
                                   CORR::CORR_PHO_LABELS[iCorr].c_str(), CORR::CORR_JET_LABELS[jCorr].c_str());
                corrHists_ptBinAll[0][j].h1D_name[iCorr][jCorr] = subHistName.c_str();
                corrHists_ptBinAll[0][j].h1D[iCorr][jCorr] = new TH1D(Form("h1D_%s", subHistName.c_str()), "",nBins_rjg, bins_rjg);

                // x_jg_mean
                subHistName = Form("%s_ptBinAll_hiBin%d_%s_%s", correlationHistNames_ptBinAll[1].c_str(), j,
                                   CORR::CORR_PHO_LABELS[iCorr].c_str(), CORR::CORR_JET_LABELS[jCorr].c_str());
                corrHists_ptBinAll[1][j].h1D_name[iCorr][jCorr] = subHistName.c_str();
                corrHists_ptBinAll[1][j].h1D[iCorr][jCorr] = new TH1D(Form("h1D_%s", subHistName.c_str()), "",nBins_xjg_mean, bins_xjg_mean);
            }
        }
    }

    // histograms with centrality on the x-axis
    std::vector<std::string> correlationHistNames_centBinAll = {"rjg", "xjg_mean"};
    int nCorrHist_centBinAll = correlationHistNames_centBinAll.size();
    correlationHist corrHists_centBinAll[nCorrHist_centBinAll][nBins_pt];
    // prepare histogram names for rjg and <xjg>
    const int nBins_rjg_cent = 4;
    const int nBins_xjg_mean_cent= 4;
    double bins_rjg_cent[nBins_rjg+1] =           {0, 20, 60, 100, 200};
    double bins_xjg_mean_cent[nBins_xjg_mean+1] = {0, 20, 60, 100, 200};

    for (int j=0; j<nBins_pt; ++j){
        for (int iCorr = 0; iCorr < CORR::kN_CORRFNC; ++iCorr) {
            for (int jCorr = 0; jCorr < CORR::kN_CORRFNC; ++jCorr) {

                // rjg
                std::string subHistName;
                subHistName = Form("%s_centBinAll_ptBin%d_%s_%s", correlationHistNames_centBinAll[0].c_str(), j,
                                   CORR::CORR_PHO_LABELS[iCorr].c_str(), CORR::CORR_JET_LABELS[jCorr].c_str());
                corrHists_centBinAll[0][j].h1D_name[iCorr][jCorr] = subHistName.c_str();
                corrHists_centBinAll[0][j].h1D[iCorr][jCorr] = new TH1D(Form("h1D_%s", subHistName.c_str()), "",nBins_rjg_cent, bins_rjg_cent);

                // x_jg_mean
                subHistName = Form("%s_centBinAll_ptBin%d_%s_%s", correlationHistNames_centBinAll[1].c_str(), j,
                                   CORR::CORR_PHO_LABELS[iCorr].c_str(), CORR::CORR_JET_LABELS[jCorr].c_str());
                corrHists_centBinAll[1][j].h1D_name[iCorr][jCorr] = subHistName.c_str();
                corrHists_centBinAll[1][j].h1D[iCorr][jCorr] = new TH1D(Form("h1D_%s", subHistName.c_str()), "",nBins_xjg_mean_cent, bins_xjg_mean_cent);
            }
        }
    }


    // selection for jet regions
    // jet from bkg region are already separated from raw region.
    // no additional selection for jets. just use different trees.
    std::cout<<"####################"<<std::endl;
    std::cout<<"tgj->GetEntries() = "<<tgj[0]->GetEntries()<<std::endl;
    if (trigger.compare("") != 0 && !isMC) {
        std::cout<<"tgj->GetEntries(trigger==1) = "<<tgj[0]->GetEntries(Form("%s == 1",trigger.c_str()))<<std::endl;
    }
    else {
        std::cout<<"tgj->GetEntries(trigger==1) is skipped because either no trigger is specified or the data is coming from MC."<<std::endl;
    }
    std::cout<<"####################"<<std::endl;

    //time_t startTime = time(0);
    // double phoCutInt = 0;
    // double jetCutInt = 0;
    long long nentries = tgj[0]->GetEntries();
    for(long long jentry = 0; jentry < nentries; jentry++)
    {
        if (jentry % 2000 == 0)  {
            std::cout << "current entry = " <<jentry<<" out of "<<nentries<<" : "<<std::setprecision(2)<<(double)jentry/nentries*100<<" %"<<std::endl;
        }

        // time_t phoCutStart = time(0);
        tHlt->GetEntry(jentry);
        // event selection
        if(!isMC && !HLT_HISinglePhoton40_Eta1p5_v1) continue;
        if(isHI && isMC && !HLT_HISinglePhoton40_Eta1p5_v2) continue;
        if(!isHI && isMC && !HLT_HISinglePhoton40_Eta1p5ForPPRef_v1) continue;

        tPho->GetEntry(jentry);
        tgj[0]->GetEntry(jentry);

        // eta cut moved to skim step
        // eta cut
        //if(TMath::Abs((*pho.phoEta)[gj[0].phoIdx]) > 1.44) continue;

        // noise cut moved to skim step
        // // noise cut
        // if(((*pho.phoE3x3)[gj[0].phoIdx]/(*pho.phoE5x5)[gj[0].phoIdx] > 2/3-0.03 &&
        //     (*pho.phoE3x3)[gj[0].phoIdx]/(*pho.phoE5x5)[gj[0].phoIdx] < 2/3+0.03) &&
        //    ((*pho.phoE1x5)[gj[0].phoIdx]/(*pho.phoE5x5)[gj[0].phoIdx] > 1/3-0.03 &&
        //     (*pho.phoE1x5)[gj[0].phoIdx]/(*pho.phoE5x5)[gj[0].phoIdx] < 1/3+0.03) &&
        //    ((*pho.phoE2x5)[gj[0].phoIdx]/(*pho.phoE5x5)[gj[0].phoIdx] > 2/3-0.03 &&
        //     (*pho.phoE2x5)[gj[0].phoIdx]/(*pho.phoE5x5)[gj[0].phoIdx] < 2/3+0.03)) continue;

        // isolation cut
        if(((*pho.pho_ecalClusterIsoR4)[gj[0].phoIdx] +
            (*pho.pho_hcalRechitIsoR4)[gj[0].phoIdx] +
            (*pho.pho_trackIsoR4PtCut20)[gj[0].phoIdx] ) > cut_sumIso) continue;
        if((*pho.phoHoverE)[gj[0].phoIdx] > 0.1) continue;

        bool isSignalPho = ((*pho.phoSigmaIEtaIEta_2012)[gj[0].phoIdx] < cut_phoSigmaIEtaIEta);
        bool isBkgPho = ((*pho.phoSigmaIEtaIEta_2012)[gj[0].phoIdx] > 0.011 &&
                         (*pho.phoSigmaIEtaIEta_2012)[gj[0].phoIdx] < 0.017);

        if(!(isSignalPho || isBkgPho)) continue;

        int phoType = (isSignalPho ? CORR::kRAW : CORR::kBKG);
        // phoCutInt += difftime(time(0),phoCutStart);

        tJet->GetEntry(jentry);
        tHiEvt->GetEntry(jentry);

        float weight = 1;
        if(isMC){
            weight = evt.weight;
        }

        // handle nEntriesPho separate from jet loop
        for (int i=0; i<nBins_pt; ++i){
            if((*pho.phoEt)[gj[0].phoIdx] <  bins_pt[0][i] ||
               (*pho.phoEt)[gj[0].phoIdx] >= bins_pt[1][i]) continue;
            for(int j=0; j<nBins_hiBin; ++j){
                if(isHI){
                    if(evt.hiBin <  bins_hiBin[0][j] ||
                       evt.hiBin >= bins_hiBin[1][j]) continue;
                }
                for(int iHist = 0; iHist < nCorrHist; iHist++){
                    corrHists[iHist][i][j].nEntriesPho[phoType][CORR::kRAW] += weight;
                }
            }
        }

        // time_t jetCutStart = time(0);
        bool gotEntry[nSmearBins] = {false}; // only call getEntry once per event for smear trees
        gotEntry[0] = true;
        for(int ijet = 0; ijet < jet.nref; ijet++){
            // jet cuts
            //jteta cut moved to skim
            //if(TMath::Abs(jet.jteta[ijet]) > cut_jeteta) continue;
            if((*gj[0].jetID)[ijet] < cut_jetID) continue;

            for(int j=0; j<nBins_hiBin; ++j){
                int smearingBinIndex = 0;
                if(isHI){
                    if(evt.hiBin <  bins_hiBin[0][j] ||
                       evt.hiBin >= bins_hiBin[1][j]) continue;
                } else {
                    if(!gotEntry[j]){
                        tgj[j]->GetEntry(jentry);
                        gotEntry[j] = true;
                    }
                    smearingBinIndex = j;
                }

                if(smeared_jtpt(jet, ijet, smearingBinIndex) < cut_jetpt) continue;
                if((*gj[smearingBinIndex].dR)[ijet] < cut_dR) continue;

                for (int i=0; i<nBins_pt; ++i){
                    if((*pho.phoEt)[gj[0].phoIdx] <  bins_pt[0][i] ||
                       (*pho.phoEt)[gj[0].phoIdx] >= bins_pt[1][i]) continue;

                    // fill histograms
                    // dphi = 1
                    corrHists[1][i][j].h1D[phoType][CORR::kRAW]->Fill(TMath::Abs((*gj[smearingBinIndex].dphi)[ijet]), weight);
                    corrHists[1][i][j].nEntries[phoType][CORR::kRAW] += weight;
                    //apply dphi cuts now
                    if(TMath::Abs((*gj[smearingBinIndex].dphi)[ijet]) <= cut_awayRange) continue;
                    // xjg = 0
                    // jtpt = 2
                    corrHists[0][i][j].h1D[phoType][CORR::kRAW]->Fill((*gj[smearingBinIndex].xjg)[ijet], weight);
                    corrHists[0][i][j].nEntries[phoType][CORR::kRAW] += weight;
                    corrHists[2][i][j].h1D[phoType][CORR::kRAW]->Fill(smeared_jtpt(jet, ijet, smearingBinIndex), weight);
                    corrHists[2][i][j].nEntries[phoType][CORR::kRAW] += weight;
                }
            }
        }
        // jetCutInt += difftime(time(0), jetCutStart);

        if(hasJetsMB && hasGammaJetMB) {
            tJetMB->GetEntry(jentry);
            tgjMB->GetEntry(jentry);

            for(int ijet = 0; ijet < jetMB.nref; ijet++){
                // jet cuts
                if((*gjMB.dR)[ijet] < cut_dR) continue;
                //jteta cut moved to skim
                //if(TMath::Abs(jetMB.jteta[ijet]) > cut_jeteta) continue;
                if(smeared_jtpt(jetMB, ijet, 0) < cut_jetpt) continue;
                if((*gjMB.jetID)[ijet] < cut_jetID) continue;

                for (int i=0; i<nBins_pt; ++i){
                    if((*pho.phoEt)[gjMB.phoIdx] <  bins_pt[0][i] ||
                       (*pho.phoEt)[gjMB.phoIdx] >= bins_pt[1][i]) continue;
                    for(int j=0; j<nBins_hiBin; ++j){
                        if(evt.hiBin <  bins_hiBin[0][j] ||
                           evt.hiBin >= bins_hiBin[1][j]) continue;

                        // fill histograms
                        // dphi = 1
                        corrHists[1][i][j].h1D[phoType][CORR::kBKG]->Fill(TMath::Abs((*gjMB.dphi)[ijet]), weight);
                        corrHists[1][i][j].nEntries[phoType][CORR::kBKG] += weight;
                        //apply dphi cuts now
                        if(TMath::Abs((*gjMB.dphi)[ijet]) <= cut_awayRange) continue;
                        // xjg = 0
                        // jtpt = 2
                        corrHists[0][i][j].h1D[phoType][CORR::kBKG]->Fill((*gjMB.xjg)[ijet], weight);
                        corrHists[0][i][j].nEntries[phoType][CORR::kBKG] += weight;
                        corrHists[2][i][j].h1D[phoType][CORR::kBKG]->Fill(smeared_jtpt(jetMB, ijet, 0), weight);
                        corrHists[2][i][j].nEntries[phoType][CORR::kBKG] += weight;
                    }
                }
            }
        }
    }

    // std::cout << "Photon Cuts Time: " << phoCutInt << std::endl;
    // std::cout << "Jet Cuts/Loop Time: " << jetCutInt << std::endl;

    /// Histogram arithmetic (no reading I/O)
    TCanvas* c = new TCanvas("cnv","",600,600);
    for(int iHist = 0; iHist < nCorrHist; iHist++){
        for (int i=0; i<nBins_pt; ++i){
            for(int j=0; j<nBins_hiBin; ++j){
                corrHists[iHist][i][j].nEntriesPho[CORR::kRAW][CORR::kBKG] = corrHists[iHist][i][j].nEntriesPho[CORR::kRAW][CORR::kRAW];
                corrHists[iHist][i][j].nEntriesPho[CORR::kBKG][CORR::kBKG] = corrHists[iHist][i][j].nEntriesPho[CORR::kBKG][CORR::kRAW];

                if (h_nPho[i][j][CORR::kRAW]->GetBinContent(1) == 0)  {
                    h_nPho[i][j][CORR::kRAW]->SetBinContent(1, corrHists[iHist][i][j].nEntriesPho[CORR::kRAW][CORR::kRAW]);
                    h_nPho[i][j][CORR::kRAW]->Write("",TObject::kOverwrite);
                }

                if (h_nPho[i][j][CORR::kBKG]->GetBinContent(1) == 0)  {
                    h_nPho[i][j][CORR::kBKG]->SetBinContent(1, corrHists[iHist][i][j].nEntriesPho[CORR::kBKG][CORR::kRAW]);
                    h_nPho[i][j][CORR::kBKG]->Write("",TObject::kOverwrite);
                }

                std::cout<< "nEntries[CORR::kRAW][CORR::kRAW] = " << corrHists[iHist][i][j].nEntries[CORR::kRAW][CORR::kRAW] <<std::endl;
                std::cout<< "nEntries[CORR::kBKG][CORR::kRAW] = " << corrHists[iHist][i][j].nEntries[CORR::kBKG][CORR::kRAW] <<std::endl;
                std::cout<< "nEntriesPho[CORR::kRAW][CORR::kRAW] = " << corrHists[iHist][i][j].nEntriesPho[CORR::kRAW][CORR::kRAW] <<std::endl;
                std::cout<< "nEntriesPho[CORR::kBKG][CORR::kRAW] = " << corrHists[iHist][i][j].nEntriesPho[CORR::kBKG][CORR::kRAW] <<std::endl;

                std::string histoTitle;
                //if (isHI) {
                    histoTitle = Form("%s , %.0f < p^{#gamma}_{T} < %.0f GeV/c, %d-%d %% ",collisionName , bins_pt[0][i], bins_pt[1][i], bins_hiBin[0][j]/2, bins_hiBin[1][j]/2);

                    // special cases
                    if (bins_pt[0][i] <= 0 && bins_pt[1][i] < 0 && bins_hiBin[0][j] <= 0 && bins_hiBin[1][j] >= 200 )   {
                        histoTitle = Form("%s",collisionName );
                    }
                    else if (bins_pt[0][i] <= 0 && bins_pt[1][i] < 0)   {
                        histoTitle = Form("%s , %d-%d %%",collisionName , bins_hiBin[0][j]/2, bins_hiBin[1][j]/2);
                    }
                    else if (bins_pt[1][i] < 0 && bins_hiBin[0][j] <= 0 && bins_hiBin[1][j] >= 200)   {
                        histoTitle = Form("%s ,  p^{#gamma}_{T} > %.0f GeV/c",collisionName , bins_pt[0][i]);
                    }
                    else if (bins_hiBin[0][j] <= 0 && bins_hiBin[1][j] >= 200)   {
                        histoTitle = Form("%s , %.0f < p^{#gamma}_{T} < %.0f GeV/c",collisionName , bins_pt[0][i], bins_pt[1][i]);
                    }
                    else if (bins_pt[1][i] < 0) {
                        histoTitle = Form("%s , p^{#gamma}_{T} > %.0f GeV/c, %d-%d %% ",collisionName , bins_pt[0][i], bins_hiBin[0][j]/2, bins_hiBin[1][j]/2);
                    }
                    //}
                // else {
                //     histoTitle = Form("%s , %.0f < p^{#gamma}_{T} < %.0f GeV/c",collisionName , bins_pt[0][i], bins_pt[1][i]);

                //     // special cases
                //     if (bins_pt[0][i] <= 0 && bins_pt[1][i] < 0)   {
                //         histoTitle = Form("%s",collisionName );
                //     }
                //     else if (bins_pt[1][i] < 0) {
                //         histoTitle = Form("%s , p^{#gamma}_{T} > %.0f GeV/c", collisionName , bins_pt[0][i]);
                //     }
                // }

                // histograms for RAW and BKG regions
                for (int iCorr = 0; iCorr < CORR::kN_CORRFNC -1; ++iCorr) {
                    for (int jCorr = 0; jCorr < CORR::kN_CORRFNC -1; ++jCorr) {

                        if (jCorr == CORR::kBKG && !isHI)  continue;      // no jet background for non-HI

                        std::string titleX = corrHists[iHist][i][j].h1D_titleX[iCorr][jCorr].c_str();
                        corrHists[iHist][i][j].h1D[iCorr][jCorr]->SetTitle(Form("%s;%s;%s",histoTitle.c_str(), titleX.c_str(), "Entries"));
                        corrHists[iHist][i][j].h1D[iCorr][jCorr]->SetMarkerStyle(kFullCircle);
                        corrHists[iHist][i][j].h1D[iCorr][jCorr]->SetMarkerColor(kBlack);

                        // histogram name excluding the "h1D" prefix
                        std::string tmpH1D_name = corrHists[iHist][i][j].h1D_name[iCorr][jCorr].c_str();
                        std::string tmpHistName = corrHists[iHist][i][j].h1D[iCorr][jCorr]->GetName();

                        c->SetName(Form("cnv_%s",tmpH1D_name.c_str()));
                        c->cd();
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
                        double tmpXlow = xlow_final[iHist];
                        double tmpXup  = xup_final[iHist];
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
                        if (jCorr == CORR::kBKG && hasJetsMB && hasGammaJetMB) {   // (hasJetsMB && hasGammaJetMB) ==> isHI
                            // normalize first by the number of mixed events
                            corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->Scale(1./nEventsToMix);
                        }
                        if (nSmear > 0 && nSmear != 1) {
                            //first correct actual bin value
                            corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->Scale(1./nSmear);
                            //then increase statistical bin error by 10 to account for 100 "fake" smearing
                            for(int ibin = 1;
                                ibin <= corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->GetNbinsX();
                                ibin++)
                            {
                                corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->SetBinError
                                    (ibin,
                                     TMath::Sqrt(nSmear)*
                                     corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->GetBinError(ibin));
                            }
                        }
                        int tmpNEntriesPho = corrHists[iHist][i][j].nEntriesPho[iCorr][jCorr];
                        // normalization is done with photon events, not necessarily by photon-jet events
                        // so the integral of the normalized histogram is R_jg.
                        corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->Scale(1./tmpNEntriesPho);
                        std::string tmpTitleY_final_norm = corrHists[iHist][i][j].h1D_titleY_final_norm[iCorr][jCorr].c_str();
                        corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->GetYaxis()->SetTitle(tmpTitleY_final_norm.c_str());
                        corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->Draw("e");
                        corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->Write("",TObject::kOverwrite);
                        c->Write("",TObject::kOverwrite);
                        c->Clear();
                    }
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

                    // reset BKGBKG
                    tmpHistName = Form("%s_final_norm", corrHists[iHist][i][j].h1D[CORR::kBKG][CORR::kBKG]->GetName());
                    corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kBKG] =
                        (TH1D*)corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kRAW]->Clone(tmpHistName.c_str());
                    corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kBKG]->Reset();
                }

                // first subtract jet BKG, then subtract photon BKG
                // subtract jet BKG
                // RAWSIG = RAWRAW - RAWBKG
                std::string tmpHistNameRAWSIG = Form("%s_final_norm", corrHists[iHist][i][j].h1D[CORR::kRAW][CORR::kSIG]->GetName());
                std::cout<<tmpHistNameRAWSIG.c_str()<<std::endl;
                corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kSIG] =
                    (TH1D*)corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kRAW]->Clone(tmpHistNameRAWSIG.c_str());
                // do arithmetic if histograms are not empty
                if (corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kBKG]->GetEntries() > 0) {
                    corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kSIG]->Add(corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kBKG],-1);
                }

                // BKGSIG = BKGRAW - BKGBKG
                std::string tmpHistNameBKGSIG = Form("%s_final_norm", corrHists[iHist][i][j].h1D[CORR::kBKG][CORR::kSIG]->GetName());
                std::cout<<tmpHistNameBKGSIG.c_str()<<std::endl;
                corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kSIG] =
                    (TH1D*)corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kRAW]->Clone(tmpHistNameBKGSIG.c_str());
                // do arithmetic if histograms are not empty
                if (corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kBKG]->GetEntries() > 0) {
                    corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kSIG]->Add(corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kBKG],-1);
                }

                // subtract photon BKG
                // purity*SIGSIG + (1-purity)*BKGSIG = RAWSIG
                // SIGSIG = 1/purity *  ( RAWSIG - (1-purity) * BKGSIG )
                std::string tmpHistNameSIGSIG = Form("%s_final_norm", corrHists[iHist][i][j].h1D[CORR::kSIG][CORR::kSIG]->GetName());
                std::cout<<tmpHistNameSIGSIG.c_str()<<std::endl;
                corrHists[iHist][i][j].h1D_final_norm[CORR::kSIG][CORR::kSIG] =
                    (TH1D*)corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kSIG]->Clone(tmpHistNameSIGSIG.c_str());
                // do arithmetic if histograms are not empty
                if (corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kSIG]->GetEntries() > 0) {
                    corrHists[iHist][i][j].h1D_final_norm[CORR::kSIG][CORR::kSIG]->Add(corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kSIG],-1*(1-purity[i][j]));
                    corrHists[iHist][i][j].h1D_final_norm[CORR::kSIG][CORR::kSIG]->Scale(1./purity[i][j]);
                }
                std::cout<< "purity[i][j] = " << purity[i][j] << std::endl;

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

                // FINAL_NORM  BKGSIG
                std::string tmpH1D_nameBKGSIG = corrHists[iHist][i][j].h1D_name[CORR::kBKG][CORR::kSIG].c_str();
                std::cout<<"drawing tmpH1D_nameBKGSIG = "<<tmpH1D_nameBKGSIG.c_str()<<std::endl;
                c->SetName(Form("cnv_%s_final_norm",tmpH1D_nameBKGSIG.c_str()));
                c->cd();
                corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kSIG]->Draw("e");
                corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kSIG]->Write("",TObject::kOverwrite);
                corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kSIG]->SetStats(false);  // remove stat box from the canvas, but keep in the histograms.
                c->Write("",TObject::kOverwrite);
                c->Clear();

                // FINAL_NORM  SIGSIG
                std::string tmpH1D_nameSIGSIG = corrHists[iHist][i][j].h1D_name[CORR::kSIG][CORR::kSIG].c_str();
                std::cout<<"drawing tmpH1D_nameSIGSIG = "<<tmpH1D_nameSIGSIG.c_str()<<std::endl;
                c->SetName(Form("cnv_%s_final_norm",tmpH1D_nameSIGSIG.c_str()));
                c->cd();
                corrHists[iHist][i][j].h1D_final_norm[CORR::kSIG][CORR::kSIG]->Draw("e");
                corrHists[iHist][i][j].h1D_final_norm[CORR::kSIG][CORR::kSIG]->Write("",TObject::kOverwrite);
                corrHists[iHist][i][j].h1D_final_norm[CORR::kSIG][CORR::kSIG]->SetStats(false);  // remove stat box from the canvas, but keep in the histograms.
                c->Write("",TObject::kOverwrite);
                c->Clear();

                std::cout<<Form("histogramming END : ptBin%d HiBin%d", i, j)<<std::endl;
                std::cout<<"##########"<<std::endl;
            }
        }
        std::cout<<"histogramming END : "<<correlationHistNames[iHist].c_str()<<std::endl;
        std::cout<<"####################"<<std::endl;
    }

    // histograms with pt bins on x-axis
    // corrHists_ptBinAll[0][]  = R_jg
    // corrHists_ptBinAll[1][]  = <X_jg>
    std::cout<<"####################"<<std::endl;
    for(int j=0; j<nBins_hiBin; ++j){
        for(int iCorr=0; iCorr<CORR::kN_CORRFNC; ++iCorr) {
            for(int jCorr=0; jCorr<CORR::kN_CORRFNC; ++jCorr) {

                // ignore SIGRAW, SIGBKG histograms
                if ((iCorr == CORR::kSIG && (jCorr == CORR::kRAW || jCorr == CORR::kBKG))) continue;

                //if(j>0) continue;

                int offset = 2; // ptBin 40-50 starts from index 2.
                // rjg block
                for(int i=0; i<nBins_rjg; ++i){

                    double err;
                    double val = corrHists[0][i+offset][j].h1D_final_norm[iCorr][jCorr]->IntegralAndError(1,
                                                                                                          corrHists[0][i+offset][j].h1D_final_norm[iCorr][jCorr]->GetNbinsX(), err);

                    corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->SetBinContent(i+1, val);
                    corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->SetBinError(i+1, err);
                }

                std::string histoTitle = Form("%s , %d-%d %%",collisionName , bins_hiBin[0][j]/2, bins_hiBin[1][j]/2);

                corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->SetTitle(Form("%s;p^{#gamma}_{T} (GeV/c); R_{J#gamma}",histoTitle.c_str()));
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

                // xjg_mean block
                for(int i=0; i<nBins_xjg_mean; ++i){

                    double val = corrHists[0][i+offset][j].h1D_final_norm[iCorr][jCorr]->GetMean();
                    double err = corrHists[0][i+offset][j].h1D_final_norm[iCorr][jCorr]->GetMeanError();

                    corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->SetBinContent(i+1, val);
                    corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->SetBinError(i+1, err);
                }

                corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->SetTitle(Form("%s;p^{#gamma}_{T} (GeV/c); <x_{J#gamma}>",histoTitle.c_str()));
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
    }
    std::cout<<"####################"<<std::endl;

    // histograms with centrality bins on x-axis
    // corrHists_centBinAll[0][]  = R_jg
    // corrHists_centBinAll[1][]  = <X_jg>
    std::cout<<"####################"<<std::endl;
    for(int j=0; j<nBins_pt; ++j){
        for(int iCorr=0; iCorr<CORR::kN_CORRFNC; ++iCorr) {
            for(int jCorr=0; jCorr<CORR::kN_CORRFNC; ++jCorr) {

                // ignore SIGRAW, SIGBKG histograms
                if ((iCorr == CORR::kSIG && (jCorr == CORR::kRAW || jCorr == CORR::kBKG))) continue;

                //if(j>0 && !isHI) continue;

                int offset = 6; // hiBin 0-10 starts from index 6.
                // rjg block
                for(int i=0; i<nBins_rjg_cent; ++i){

                    double err;
                    double val = corrHists[0][j][i+offset].h1D_final_norm[iCorr][jCorr]->IntegralAndError(1,
                                                                                                          corrHists[0][j][i+offset].h1D_final_norm[iCorr][jCorr]->GetNbinsX(), err);

                    corrHists_centBinAll[0][j].h1D[iCorr][jCorr]->SetBinContent(i+1, val);
                    corrHists_centBinAll[0][j].h1D[iCorr][jCorr]->SetBinError(i+1, err);
                }

                std::string histoTitle = Form("%s , %lf-%lf %%",collisionName , bins_pt[0][j], bins_pt[1][j]);

                corrHists_centBinAll[0][j].h1D[iCorr][jCorr]->SetTitle(Form("%s;Centrality Bin; R_{J#gamma}",histoTitle.c_str()));
                corrHists_centBinAll[0][j].h1D[iCorr][jCorr]->SetMarkerStyle(kFullCircle);
                corrHists_centBinAll[0][j].h1D[iCorr][jCorr]->SetMarkerColor(kBlack);

                std::cout<<"drawing : "<<corrHists_centBinAll[0][j].h1D_name[iCorr][jCorr].c_str()<<std::endl;
                c->SetName(Form("cnv_%s",corrHists_centBinAll[0][j].h1D_name[iCorr][jCorr].c_str()));
                c->cd();
                corrHists_centBinAll[0][j].h1D[iCorr][jCorr]->Draw("e");
                corrHists_centBinAll[0][j].h1D[iCorr][jCorr]->Write("",TObject::kOverwrite);
                corrHists_centBinAll[0][j].h1D[iCorr][jCorr]->SetStats(false);     // remove stat box from the canvas, but keep in the histograms.
                c->Write("",TObject::kOverwrite);
                c->Clear();

                // xjg_mean block
                for(int i=0; i<nBins_xjg_mean_cent; ++i){

                    double val = corrHists[0][j][i+offset].h1D_final_norm[iCorr][jCorr]->GetMean();
                    double err = corrHists[0][j][i+offset].h1D_final_norm[iCorr][jCorr]->GetMeanError();

                    corrHists_centBinAll[1][j].h1D[iCorr][jCorr]->SetBinContent(i+1, val);
                    corrHists_centBinAll[1][j].h1D[iCorr][jCorr]->SetBinError(i+1, err);
                }

                corrHists_centBinAll[1][j].h1D[iCorr][jCorr]->SetTitle(Form("%s;Centrality Bin; <x_{J#gamma}>",histoTitle.c_str()));
                corrHists_centBinAll[1][j].h1D[iCorr][jCorr]->SetMarkerStyle(kFullCircle);
                corrHists_centBinAll[1][j].h1D[iCorr][jCorr]->SetMarkerColor(kBlack);

                std::cout<<"drawing : "<<corrHists_centBinAll[1][j].h1D_name[iCorr][jCorr].c_str()<<std::endl;
                c->SetName(Form("cnv_%s",corrHists_centBinAll[1][j].h1D_name[iCorr][jCorr].c_str()));
                c->cd();
                corrHists_centBinAll[1][j].h1D[iCorr][jCorr]->Draw("e");
                corrHists_centBinAll[1][j].h1D[iCorr][jCorr]->Write("",TObject::kOverwrite);
                corrHists_centBinAll[1][j].h1D[iCorr][jCorr]->SetStats(false);     // remove stat box from the canvas, but keep in the histograms.
                c->Write("",TObject::kOverwrite);
                c->Clear();
            }
        }
    }
    std::cout<<"####################"<<std::endl;

    configTree->Write("",TObject::kOverwrite);

    output->Write("",TObject::kOverwrite);
    input->Close();
    output->Close();
}

int main(int argc, char** argv)
{
    if (argc == 4) {
        gammaJetHistogram(argv[1], argv[2], argv[3]);
        return 0;
    }
    else if (argc == 3) {
        gammaJetHistogram(argv[1], argv[2]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
            "./gammaJetHistogram.exe <configFile> <inputFile> <outputFile>"
                  << std::endl;
        return 1;
    }
}
