// this code is inherited from gammaJetSkim.C by Yeonju Go
// 2017 Mar 29 modification : pho_isEle and pho_is2015Noise are calculated in nPho loop. so all the photons have this info.  

#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TRandom3.h>

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

#include "../../CorrelationTuple/EventMatcher.h"
#include "../../TreeHeaders/ggHiNtuplizerTreePhotonRAA.h"
#include "../../TreeHeaders/CutConfigurationTree.h"
//#include "../../Utilities/commonUtility.h"
#include "../../Plotting/commonUtility.h"
#include "../../Utilities/interface/CutConfigurationParser.h"
#include "../../Utilities/interface/InputConfigurationParser.h"
#include "../../Utilities/interface/HiForestInfoController.h"

const long MAXTREESIZE = 2000000000000; // set maximum tree size from 10 GB to 1862 GB, so that the code does not switch to a new file after 10 GB

double getAngleToEP(double angle);

int photonRaaSkim(const TString configFile, const TString inputFile, const TString outputFile = "photonRaaSkim.root", const int isEmEnr=0, const int isHighPhotonPD=false, const int nJobs=-1, const int jobNum=-1) {
    std::cout<<"running photonRaaSkim()"<<std::endl;
    std::cout<<"configFile  = "<< configFile.Data() <<std::endl;
    std::cout<<"inputFile   = "<< inputFile.Data() <<std::endl;
    std::cout<<"outputFile  = "<< outputFile.Data() <<std::endl;

    const InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
    const CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

    // input configuration
    if (!configInput.isValid) {
        std::cout << "Invalid input configuration" << std::endl;
        return 1;
    } else if (!configCuts.isValid) {
        std::cout << "Invalid cut configuration" << std::endl;
        return 1;
    }

    const int collisionType = configInput.proc[INPUT::kSKIM].i[INPUT::k_collisionType];
    // verbose about input configuration
    std::cout << "Input Configuration :" << std::endl;
    std::cout << "collisionType = " << collisionType << std::endl;
    std::cout << "collision = " << getCollisionTypeName((COLL::TYPE)collisionType).c_str() << std::endl;

    // cut configuration

    std::vector<float> mcPthatWeights = ConfigurationParser::ParseListFloat(configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].s[CUTS::EVT::k_eventWeight]);
    //std::vector<float> energyScaleSys = ConfigurationParser::ParseListFloat(configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].s[CUTS::EVT::k_eventWeight]);
    const float cut_vz = configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].f[CUTS::EVT::k_vz];
    const float cut_pcollisionEventSelection = configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].i[CUTS::EVT::k_pcollisionEventSelection];
    const int cut_pPAprimaryVertexFilter = configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].i[CUTS::EVT::k_pPAprimaryVertexFilter];
    const int cut_pBeamScrapingFilter = configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].i[CUTS::EVT::k_pBeamScrapingFilter];

    const float cutPhoEt = configCuts.proc[CUTS::kSKIM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_et];
    const float cutPhoEta = configCuts.proc[CUTS::kSKIM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_eta];

    const int doEventWeight = configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].i[CUTS::EVT::k_doEventWeight];
    const bool isMC = collisionIsMC((COLL::TYPE)collisionType);
    const bool isHI = collisionIsHI((COLL::TYPE)collisionType);

    const string reco_algo = configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].s[CUTS::EVT::k_reco_algo].c_str();

    // binning for photon energy correction
    std::vector<int> centBins[2];
    centBins[0] = ConfigurationParser::ParseListInteger(configCuts.proc[CUTS::kCORRECTION].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_gt]);
    centBins[1] = ConfigurationParser::ParseListInteger(configCuts.proc[CUTS::kCORRECTION].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_lt]);
    int nCentBins = centBins[0].size();
    cout << "nCentBins for energy correction = " << nCentBins << endl;

    std::vector<float> etaBins[2];
    etaBins[0] = ConfigurationParser::ParseListFloat(configCuts.proc[CUTS::kCORRECTION].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_eta_gt]);
    etaBins[1] = ConfigurationParser::ParseListFloat(configCuts.proc[CUTS::kCORRECTION].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_eta_lt]);
    int nEtaBins = etaBins[0].size();

    TFile* energyCorrectionFile = 0;
    TF1* photonEnergyCorrections[nCentBins][nEtaBins] = {0};
    TF1* photonEnergyCorrections_res_sig[nCentBins][nEtaBins] = {0};
    TF1* photonEnergyCorrections_res_rms[nCentBins][nEtaBins] = {0};
    TF1* photonEnergyCorrections_pp[nEtaBins] = {0};
    TF1* photonEnergyCorrections_res_sig_pp[nEtaBins] = {0};
    TF1* photonEnergyCorrections_res_rms_pp[nEtaBins] = {0};
    if (isHI) {
        energyCorrectionFile = TFile::Open(configCuts.proc[CUTS::kCORRECTION].obj[CUTS::kPHOTON].s[CUTS::PHO::k_energy_correction_file].c_str());
        for (int i=0; i<nCentBins; ++i){
            for (int j=0; j<nEtaBins; ++j){
                photonEnergyCorrections[i][j] = (TF1*)energyCorrectionFile->Get(Form("f_mean_gaus_cent%i_eta%i", i, j));
                photonEnergyCorrections_res_rms[i][j] = (TF1*)energyCorrectionFile->Get(Form("f_rms_cent%i_eta%i", i, j));
                photonEnergyCorrections_res_sig[i][j] = (TF1*)energyCorrectionFile->Get(Form("f_sig_gaus_cent%i_eta%i", i, j));
            }
        }
    } else {
        energyCorrectionFile = TFile::Open(configCuts.proc[CUTS::kCORRECTION].obj[CUTS::kPHOTON].s[CUTS::PHO::k_energy_correction_file_pp].c_str());
        for (int i=0; i<nEtaBins; ++i){
            photonEnergyCorrections_pp[i] = (TF1*)energyCorrectionFile->Get(Form("f_mean_gaus_cent0_eta%i", i));
            photonEnergyCorrections_res_rms_pp[i] = (TF1*)energyCorrectionFile->Get(Form("f_rms_cent0_eta%i", i));
            photonEnergyCorrections_res_sig_pp[i] = (TF1*)energyCorrectionFile->Get(Form("f_sig_gaus_cent0_eta%i", i));
        }
    }

    int nCentBins_sumIso = 5; 
    int centBins_sumIso[2][nCentBins_sumIso] = {{0, 20, 60, 100, 140},{20, 60, 100, 140, 200}};
    TFile* sumIsoCorrectionFile = TFile::Open(configCuts.proc[CUTS::kCORRECTION].obj[CUTS::kPHOTON].s[CUTS::PHO::k_sumiso_correction_file].c_str());
    TH1D* sumIsoCorrections[nCentBins_sumIso] = {0};
    for (int i=0; i<nCentBins_sumIso; ++i)
        sumIsoCorrections[i] = (TH1D*)sumIsoCorrectionFile->Get(Form("sumIsoCorrections_cent%i", i));

    // mc pthat weighting
    for (std::size_t i=0; i<mcPthatWeights.size(); ++i)
        std::cout << mcPthatWeights[i] << " ";
    std::cout << std::endl;

    // verbose about cut configuration
    std::cout<<"Cut Configuration :"<<std::endl;
    std::cout<<"cut_vz = "<< cut_vz <<std::endl;
    if (isHI) {
        std::cout<<"cut_pcollisionEventSelection = "<< cut_pcollisionEventSelection <<std::endl;
    }
    else {   // PP
        std::cout<<"cut_pPAprimaryVertexFilter = "<< cut_pPAprimaryVertexFilter <<std::endl;
        std::cout<<"cut_pBeamScrapingFilter = "<< cut_pBeamScrapingFilter <<std::endl;
    }

    std::cout<<"cutPhoEt  = "<<cutPhoEt<<std::endl;
    std::cout<<"cutPhoEta = "<<cutPhoEta<<std::endl;

    // vertex and centrality? reweighting file
    TFile* weightsFile = TFile::Open(configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].s[CUTS::EVT::k_weights_file].c_str(), "READ");
    TH1D* hweights_vz = 0;
    TH1D* hweights_cent = 0;
    if (isMC) {
        hweights_vz = (TH1D*)weightsFile->Get("hvz");
        if (isHI)
            hweights_cent = (TH1D*)weightsFile->Get("hcent");
    }

    //output file setting
    TFile* output = TFile::Open(outputFile, "RECREATE");
    TTree* configTree = setupConfigurationTreeForWriting(configCuts);
    // output tree variables
    TTree *outputTreeHLT = 0, *outputTreeggHiNtuplizer = 0, *outputTreeHiEvt = 0, *outputTreeSkim = 0;

    //event matching
    EventMatcher* em = new EventMatcher();
    Long64_t duplicateEntries = 0;
    Long64_t totalEntries = 0;
    Long64_t entriesPassedEventSelection = 0;
    Long64_t entriesAnalyzed = 0;

    std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.Data());

    for (std::size_t i=0; i<inputFiles.size(); ++i) 
        std::cout << inputFiles[i] << std::endl;
    std::cout << "input ROOT files : num = " << inputFiles.size() << std::endl;
    std::cout << "#####" << std::endl;

    std::vector<std::string>::iterator itFirst = inputFiles.begin();
    std::vector<std::string>::iterator itEnd = inputFiles.end();

    if (inputFiles.size() > 1 && nJobs != -1) {
        if (jobNum >= nJobs) {
            std::cout << "jobNum > nJobs, invalid configuration, aborting" << std::endl;
            return 1;
        } else if ((unsigned)nJobs > inputFiles.size()) {
            std::cout << "More jobs defined than input files, invalid settings." << std::endl;
            std::cout << "Number of files: " << inputFiles.size() << " . Number of jobs: " << nJobs << std::endl;
            return 1;
        }

        int totFiles = inputFiles.size();
        itFirst = inputFiles.begin() + floor((float)totFiles*(float)jobNum/(float)nJobs);
        itEnd = inputFiles.begin() + floor((float)totFiles*(float)(jobNum+1)/(float)nJobs);
        if (jobNum == nJobs-1)
            itEnd = inputFiles.end();

        std::cout << "For this job " << jobNum << std::endl;
        std::cout << "First Entry: " << floor((float)totFiles*(float)jobNum/(float)nJobs) << std::endl;
        std::cout << "Final Entry: " << floor((float)totFiles*(float)(jobNum+1)/(float)nJobs) << std::endl;
    }


    float eventWeight;
    float ncoll_data;
    std::vector<float> pho_genPt, pho_genEta, pho_genPhi, pho_genE, pho_genEt, pho_genCalIsoDR03, pho_genCalIsoDR04, pho_genTrkIsoDR03, pho_genTrkIsoDR04;
    std::vector<int> pho_genPID, pho_genStatus, pho_genMomPID;
    std::vector<float> phoEtCorrected, phoEtCorrected_sys, pho_sumIso, pho_sumIsoCorrected;
    std::vector<int> phoPreScale;
    std::vector<float> phoEtCorrected_resSys_rms, phoEtCorrected_resSys_sig;
    std::vector<float> phoEtCorrected_resSys_rms2, phoEtCorrected_resSys_sig2;
    std::vector<float> phoEtCorrected_resSys_up, phoEtCorrected_resSys_down;
    std::vector<int> pho_isEle, pho_is2015Noise; 
   
    // trigger info for prescale 
    // pbpb
    Int_t HLT_HISinglePhoton10_Eta1p5_v1; 
    Int_t HLT_HISinglePhoton15_Eta1p5_v1; 
    Int_t HLT_HISinglePhoton20_Eta1p5_v1; 
    Int_t HLT_HISinglePhoton30_Eta1p5_v1; 
    Int_t HLT_HISinglePhoton40_Eta1p5_v1; 
    Int_t HLT_HISinglePhoton10_Eta1p5_v2; 
    Int_t HLT_HISinglePhoton15_Eta1p5_v2; 
    Int_t HLT_HISinglePhoton20_Eta1p5_v2; 
    Int_t HLT_HISinglePhoton10_Eta1p5_v1_Prescl; 
    Int_t HLT_HISinglePhoton15_Eta1p5_v1_Prescl; 
    Int_t HLT_HISinglePhoton20_Eta1p5_v1_Prescl; 
    Int_t HLT_HISinglePhoton30_Eta1p5_v1_Prescl; 
    Int_t HLT_HISinglePhoton40_Eta1p5_v1_Prescl; 
    Int_t HLT_HISinglePhoton10_Eta1p5_v2_Prescl; 
    Int_t HLT_HISinglePhoton15_Eta1p5_v2_Prescl; 
    Int_t HLT_HISinglePhoton20_Eta1p5_v2_Prescl; 
    Int_t L1_MinimumBiasHF1_AND; 
    Int_t L1_MinimumBiasHF2_AND; 
    Int_t L1_SingleEG7_BptxAND; 
    Int_t L1_SingleEG21_BptxAND; 
    Int_t L1_MinimumBiasHF1_AND_Prescl; 
    Int_t L1_MinimumBiasHF2_AND_Prescl; 
    Int_t L1_SingleEG7_BptxAND_Prescl; 
    Int_t L1_SingleEG21_BptxAND_Prescl; 

    // pp
    Int_t L1_SingleEG5_BptxAND; 
    Int_t L1_SingleEG12_BptxAND; 
    Int_t L1_SingleEG20_BptxAND; 
    Int_t L1_SingleEG5_BptxAND_Prescl; 
    Int_t L1_SingleEG12_BptxAND_Prescl; 
    Int_t L1_SingleEG20_BptxAND_Prescl; 

    //Int_t pho_isEle, pho_is2015Noise;
    Int_t pcollisionEventSelection;  // this filter is used for HI.
    Int_t HBHENoiseFilterResultRun2Loose;
    Int_t pPAprimaryVertexFilter;    // this filter is used for PP.
    Int_t pBeamScrapingFilter;   // this filter is used for PP.
    TRandom3 *r3=new TRandom3();
    //int ii=0;
    for (std::vector<std::string>::iterator it = itFirst; it != itEnd; ++it) {
        std::cout << (*it).c_str() << std::endl;
        //ii++;
        //std::cout << ii << std::endl;
        //if(ii<100) continue;

        TFile *inFile = TFile::Open((*it).c_str());
        inFile->cd();

        TTree* treeHLT = (TTree*)inFile->Get("hltanalysis/HltTree");
        TTree* treeggHiNtuplizer = 0;
        treeggHiNtuplizer = (TTree*)inFile->Get(Form("ggHiNtuplizer%s/EventTree",reco_algo.data()));
        //std::cout << "ggHiNtuplizer" << reco_algo << " is being processed" << endl;
       // if (isHI)
       //     treeggHiNtuplizer = (TTree*)inFile->Get("ggHiNtuplizer/EventTree");
       // else 
       //     treeggHiNtuplizer = (TTree*)inFile->Get("ggHiNtuplizerGED/EventTree");

        TTree* treeHiEvt = (TTree*)inFile->Get("hiEvtAnalyzer/HiTree");
        TTree* treeSkim  = (TTree*)inFile->Get("skimanalysis/HltTree");

        treeHLT->SetBranchStatus("*", 0);     // disable all branches
        treeHLT->SetBranchStatus("HLT_HISinglePhoton*_Eta*_v*", 1);     // enable photon branches
        treeHLT->SetBranchStatus("HLT_HIDoublePhoton*_Eta*_v*", 1);     // enable photon branches
        treeHLT->SetBranchStatus("L1_MinimumBiasHF1_AND*", 1);     // enable photon branches
        treeHLT->SetBranchStatus("L1_MinimumBiasHF2_AND*", 1);     // enable photon branches
        treeHLT->SetBranchStatus("L1_SingleEG*_BptxAND*", 1);     // enable photon branches
        if(!isMC){
            if (treeHLT->GetBranch("HLT_HISinglePhoton10_Eta1p5_v1")) 
                treeHLT->SetBranchAddress("HLT_HISinglePhoton10_Eta1p5_v1", &HLT_HISinglePhoton10_Eta1p5_v1);
            if (treeHLT->GetBranch("HLT_HISinglePhoton15_Eta1p5_v1")) 
                treeHLT->SetBranchAddress("HLT_HISinglePhoton15_Eta1p5_v1", &HLT_HISinglePhoton15_Eta1p5_v1);
            if (treeHLT->GetBranch("HLT_HISinglePhoton20_Eta1p5_v1")) 
                treeHLT->SetBranchAddress("HLT_HISinglePhoton20_Eta1p5_v1", &HLT_HISinglePhoton20_Eta1p5_v1);
            if (treeHLT->GetBranch("HLT_HISinglePhoton30_Eta1p5_v1")) 
                treeHLT->SetBranchAddress("HLT_HISinglePhoton30_Eta1p5_v1", &HLT_HISinglePhoton30_Eta1p5_v1);
            if (treeHLT->GetBranch("HLT_HISinglePhoton40_Eta1p5_v1")) 
                treeHLT->SetBranchAddress("HLT_HISinglePhoton40_Eta1p5_v1", &HLT_HISinglePhoton40_Eta1p5_v1);
            if (treeHLT->GetBranch("HLT_HISinglePhoton10_Eta1p5_v2")) 
                treeHLT->SetBranchAddress("HLT_HISinglePhoton10_Eta1p5_v2", &HLT_HISinglePhoton10_Eta1p5_v2);
            if (treeHLT->GetBranch("HLT_HISinglePhoton15_Eta1p5_v2")) 
                treeHLT->SetBranchAddress("HLT_HISinglePhoton15_Eta1p5_v2", &HLT_HISinglePhoton15_Eta1p5_v2);
            if (treeHLT->GetBranch("HLT_HISinglePhoton20_Eta1p5_v2")) 
                treeHLT->SetBranchAddress("HLT_HISinglePhoton20_Eta1p5_v2", &HLT_HISinglePhoton20_Eta1p5_v2);
            if (treeHLT->GetBranch("HLT_HISinglePhoton10_Eta1p5_v1_Prescl")) 
                treeHLT->SetBranchAddress("HLT_HISinglePhoton10_Eta1p5_v1_Prescl", &HLT_HISinglePhoton10_Eta1p5_v1_Prescl);
            if (treeHLT->GetBranch("HLT_HISinglePhoton15_Eta1p5_v1_Prescl")) 
                treeHLT->SetBranchAddress("HLT_HISinglePhoton15_Eta1p5_v1_Prescl", &HLT_HISinglePhoton15_Eta1p5_v1_Prescl);
            if (treeHLT->GetBranch("HLT_HISinglePhoton20_Eta1p5_v1_Prescl")) 
                treeHLT->SetBranchAddress("HLT_HISinglePhoton20_Eta1p5_v1_Prescl", &HLT_HISinglePhoton20_Eta1p5_v1_Prescl);
            if (treeHLT->GetBranch("HLT_HISinglePhoton30_Eta1p5_v1_Prescl")) 
                treeHLT->SetBranchAddress("HLT_HISinglePhoton30_Eta1p5_v1_Prescl", &HLT_HISinglePhoton30_Eta1p5_v1_Prescl);
            if (treeHLT->GetBranch("HLT_HISinglePhoton40_Eta1p5_v1_Prescl")) 
                treeHLT->SetBranchAddress("HLT_HISinglePhoton40_Eta1p5_v1_Prescl", &HLT_HISinglePhoton40_Eta1p5_v1_Prescl);
            if (treeHLT->GetBranch("HLT_HISinglePhoton10_Eta1p5_v2_Prescl")) 
                treeHLT->SetBranchAddress("HLT_HISinglePhoton10_Eta1p5_v2_Prescl", &HLT_HISinglePhoton10_Eta1p5_v2_Prescl);
            if (treeHLT->GetBranch("HLT_HISinglePhoton15_Eta1p5_v2_Prescl")) 
                treeHLT->SetBranchAddress("HLT_HISinglePhoton15_Eta1p5_v2_Prescl", &HLT_HISinglePhoton15_Eta1p5_v2_Prescl);
            if (treeHLT->GetBranch("HLT_HISinglePhoton20_Eta1p5_v2_Prescl")) 
                treeHLT->SetBranchAddress("HLT_HISinglePhoton20_Eta1p5_v2_Prescl", &HLT_HISinglePhoton20_Eta1p5_v2_Prescl);
    
            if (treeHLT->GetBranch("L1_MinimumBiasHF1_AND")) 
                treeHLT->SetBranchAddress("L1_MinimumBiasHF1_AND", &L1_MinimumBiasHF1_AND);
            if (treeHLT->GetBranch("L1_MinimumBiasHF2_AND")) 
                treeHLT->SetBranchAddress("L1_MinimumBiasHF2_AND", &L1_MinimumBiasHF2_AND);
            if (treeHLT->GetBranch("L1_SingleEG7_BptxAND")) 
                treeHLT->SetBranchAddress("L1_SingleEG7_BptxAND", &L1_SingleEG7_BptxAND);
            if (treeHLT->GetBranch("L1_SingleEG21_BptxAND")) 
                treeHLT->SetBranchAddress("L1_SingleEG21_BptxAND", &L1_SingleEG21_BptxAND);
            if (treeHLT->GetBranch("L1_MinimumBiasHF1_AND_Prescl")) 
                treeHLT->SetBranchAddress("L1_MinimumBiasHF1_AND_Prescl", &L1_MinimumBiasHF1_AND_Prescl);
            if (treeHLT->GetBranch("L1_MinimumBiasHF2_AND_Prescl")) 
                treeHLT->SetBranchAddress("L1_MinimumBiasHF2_AND_Prescl", &L1_MinimumBiasHF2_AND_Prescl);
            if (treeHLT->GetBranch("L1_SingleEG7_BptxAND_Prescl")) 
                treeHLT->SetBranchAddress("L1_SingleEG7_BptxAND_Prescl", &L1_SingleEG7_BptxAND_Prescl);
            if (treeHLT->GetBranch("L1_SingleEG21_BptxAND_Prescl")) 
                treeHLT->SetBranchAddress("L1_SingleEG21_BptxAND_Prescl", &L1_SingleEG21_BptxAND_Prescl);
           
            if (treeHLT->GetBranch("L1_SingleEG5_BptxAND")) 
                treeHLT->SetBranchAddress("L1_SingleEG5_BptxAND", &L1_SingleEG5_BptxAND);
            if (treeHLT->GetBranch("L1_SingleEG12_BptxAND")) 
                treeHLT->SetBranchAddress("L1_SingleEG12_BptxAND", &L1_SingleEG12_BptxAND);
            if (treeHLT->GetBranch("L1_SingleEG20_BptxAND")) 
                treeHLT->SetBranchAddress("L1_SingleEG20_BptxAND", &L1_SingleEG20_BptxAND);
            if (treeHLT->GetBranch("L1_SingleEG5_BptxAND_Prescl")) 
                treeHLT->SetBranchAddress("L1_SingleEG5_BptxAND_Prescl", &L1_SingleEG5_BptxAND_Prescl);
            if (treeHLT->GetBranch("L1_SingleEG12_BptxAND_Prescl")) 
                treeHLT->SetBranchAddress("L1_SingleEG12_BptxAND_Prescl", &L1_SingleEG12_BptxAND_Prescl);
            if (treeHLT->GetBranch("L1_SingleEG20_BptxAND_Prescl")) 
                treeHLT->SetBranchAddress("L1_SingleEG20_BptxAND_Prescl", &L1_SingleEG20_BptxAND_Prescl);
        }

        // objects for gamma-jet correlations
        ggHiNtuplizer ggHi;
        ggHi.setupTreeForReading(treeggHiNtuplizer);    // treeggHiNtuplizer is input
        ggHi.pho_genPID         = &pho_genPID;
        ggHi.pho_genStatus      = &pho_genStatus;
        ggHi.pho_genPt          = &pho_genPt;
        ggHi.pho_genEta         = &pho_genEta;
        ggHi.pho_genPhi         = &pho_genPhi;
        ggHi.pho_genE           = &pho_genE;
        ggHi.pho_genEt          = &pho_genEt;
        ggHi.pho_genMomPID      = &pho_genMomPID;
        ggHi.pho_genCalIsoDR03  = &pho_genCalIsoDR03;
        ggHi.pho_genCalIsoDR04  = &pho_genCalIsoDR04;
        ggHi.pho_genTrkIsoDR03  = &pho_genTrkIsoDR03;
        ggHi.pho_genTrkIsoDR04  = &pho_genTrkIsoDR04;
        ggHi.phoPreScale= &phoPreScale;
        ggHi.phoEtCorrected = &phoEtCorrected;
        ggHi.phoEtCorrected_sys = &phoEtCorrected_sys;
        ggHi.pho_sumIsoCorrected = &pho_sumIsoCorrected;
        ggHi.pho_sumIso= &pho_sumIso;
        ggHi.pho_isEle = &pho_isEle;
        ggHi.pho_is2015Noise = &pho_is2015Noise;

        // specify explicitly which branches to store, do not use wildcard
        treeHiEvt->SetBranchStatus("*", 0);
        treeHiEvt->SetBranchStatus("run", 1);
        treeHiEvt->SetBranchStatus("evt", 1);
        treeHiEvt->SetBranchStatus("lumi", 1);
        treeHiEvt->SetBranchStatus("vz", 1);
        treeHiEvt->SetBranchStatus("hiBin", 1);
        treeHiEvt->SetBranchStatus("hiHF", 1);
        treeHiEvt->SetBranchStatus("hiHFplus", 1);
        treeHiEvt->SetBranchStatus("hiHFminus", 1);
        treeHiEvt->SetBranchStatus("hiHFplusEta4", 1);
        treeHiEvt->SetBranchStatus("hiHFminusEta4", 1);
        // treeHiEvt->SetBranchStatus("hiNevtPlane", 1);
        if(isHI) treeHiEvt->SetBranchStatus("hiEvtPlanes", 1);
        if (isMC) {
            if (treeHiEvt->GetBranch("Npart")) treeHiEvt->SetBranchStatus("Npart", 1);
            if (treeHiEvt->GetBranch("Ncoll"))treeHiEvt->SetBranchStatus("Ncoll", 1);
            if (treeHiEvt->GetBranch("Nhard"))treeHiEvt->SetBranchStatus("Nhard", 1);
            if (treeHiEvt->GetBranch("ProcessID"))treeHiEvt->SetBranchStatus("ProcessID", 1);
            if (treeHiEvt->GetBranch("pthat"))treeHiEvt->SetBranchStatus("pthat", 1);
            if (treeHiEvt->GetBranch("alphaQCD"))treeHiEvt->SetBranchStatus("alphaQCD", 1);
            if (treeHiEvt->GetBranch("alphaQED"))treeHiEvt->SetBranchStatus("alphaQED", 1);
            if (treeHiEvt->GetBranch("aScale"))treeHiEvt->SetBranchStatus("qScale", 1);
        }

        float vz;
        Int_t hiBin;
        Float_t hiEvtPlanes[29];   //[hiNevtPlane]
        UInt_t run, lumis;
        ULong64_t event;
        float pthat;

        treeHiEvt->SetBranchAddress("vz", &vz);
        treeHiEvt->SetBranchAddress("hiBin", &hiBin);
        if(isHI) treeHiEvt->SetBranchAddress("hiEvtPlanes", &hiEvtPlanes);
        treeHiEvt->SetBranchAddress("run", &run);
        treeHiEvt->SetBranchAddress("evt", &event);
        treeHiEvt->SetBranchAddress("lumi", &lumis);
        if (isMC)
            treeHiEvt->SetBranchAddress("pthat", &pthat);

        // specify explicitly which branches to store, do not use wildcard
        treeSkim->SetBranchStatus("*", 0);

        if (isHI) {
            treeSkim->SetBranchStatus("pcollisionEventSelection", 1);
            if (treeSkim->GetBranch("pcollisionEventSelection")) {
                treeSkim->SetBranchAddress("pcollisionEventSelection", &pcollisionEventSelection);
            } else {   // overwrite to default
                pcollisionEventSelection = 1;
                std::cout << "could not get branch : pcollisionEventSelection" << std::endl;
                std::cout << "set to default value : pcollisionEventSelection = " << pcollisionEventSelection << std::endl;
            }
        } else {
            pcollisionEventSelection = 0;    // default value if the collision is not HI, will not be used anyway.
        }

        if (!isMC) {
            treeSkim->SetBranchStatus("HBHENoiseFilterResultRun2Loose", 1);
            if (treeSkim->GetBranch("HBHENoiseFilterResultRun2Loose")) {
                treeSkim->SetBranchAddress("HBHENoiseFilterResultRun2Loose", &HBHENoiseFilterResultRun2Loose);
            } else {   // overwrite to default
                HBHENoiseFilterResultRun2Loose = 1;
                std::cout << "could not get branch : HBHENoiseFilterResultRun2Loose" << std::endl;
                std::cout << "set to default value : HBHENoiseFilterResultRun2Loose = " << HBHENoiseFilterResultRun2Loose << std::endl;
            }
        } else {
            HBHENoiseFilterResultRun2Loose = 0;
        }

        if (!isHI) {
            treeSkim->SetBranchStatus("pPAprimaryVertexFilter", 1);
            if (treeSkim->GetBranch("pPAprimaryVertexFilter")) {
                treeSkim->SetBranchAddress("pPAprimaryVertexFilter", &pPAprimaryVertexFilter);
            } else {   // overwrite to default
                pPAprimaryVertexFilter = 1;
                std::cout << "could not get branch : pPAprimaryVertexFilter" << std::endl;
                std::cout << "set to default value : pPAprimaryVertexFilter = " << pPAprimaryVertexFilter << std::endl;
            }
        } else {
            pPAprimaryVertexFilter = 0;      // default value if the collision is not PP, will not be used anyway.
        }

        if (!isHI) {
            treeSkim->SetBranchStatus("pBeamScrapingFilter", 1);
            if (treeSkim->GetBranch("pBeamScrapingFilter")) {
                treeSkim->SetBranchAddress("pBeamScrapingFilter", &pBeamScrapingFilter);
            } else {   // overwrite to default
                pBeamScrapingFilter = 1;
                std::cout << "could not get branch : pBeamScrapingFilter" << std::endl;
                std::cout << "set to default value : pBeamScrapingFilter = " << pBeamScrapingFilter << std::endl;
            }
        } else {
            pBeamScrapingFilter = 0;     // default value if the collision is not PP, will not be used anyway.
        }


        if (it == itFirst) {
            output->cd();
            outputTreeHLT = treeHLT->CloneTree(0);
            outputTreeHLT->SetName("hltTree");
            outputTreeHLT->SetTitle("subbranches of hltanalysis/HltTree");
            outputTreeggHiNtuplizer = treeggHiNtuplizer->CloneTree(0);
            outputTreeggHiNtuplizer->SetMaxTreeSize(MAXTREESIZE);
            if(isMC){
                outputTreeggHiNtuplizer->Branch("pho_genPID", &pho_genPID);
                outputTreeggHiNtuplizer->Branch("pho_genStatus", &pho_genStatus);
                outputTreeggHiNtuplizer->Branch("pho_genPt", &pho_genPt);
                outputTreeggHiNtuplizer->Branch("pho_genEta", &pho_genEta);
                outputTreeggHiNtuplizer->Branch("pho_genPhi", &pho_genPhi);
                outputTreeggHiNtuplizer->Branch("pho_genE", &pho_genE);
                outputTreeggHiNtuplizer->Branch("pho_genEt", &pho_genEt);
                outputTreeggHiNtuplizer->Branch("pho_genMomPID", &pho_genMomPID);
                outputTreeggHiNtuplizer->Branch("pho_genCalIsoDR03", &pho_genCalIsoDR03);
                outputTreeggHiNtuplizer->Branch("pho_genCalIsoDR04", &pho_genCalIsoDR04);
                outputTreeggHiNtuplizer->Branch("pho_genTrkIsoDR03", &pho_genTrkIsoDR03);
                outputTreeggHiNtuplizer->Branch("pho_genTrkIsoDR04", &pho_genTrkIsoDR04);
            }
            outputTreeggHiNtuplizer->Branch("phoPreScale", &phoPreScale);
            outputTreeggHiNtuplizer->Branch("phoEtCorrected", &phoEtCorrected);
            outputTreeggHiNtuplizer->Branch("phoEtCorrected_sys", &phoEtCorrected_sys);
            outputTreeggHiNtuplizer->Branch("phoEtCorrected_resSys_rms", &phoEtCorrected_resSys_rms);
            outputTreeggHiNtuplizer->Branch("phoEtCorrected_resSys_rms2", &phoEtCorrected_resSys_rms2);
            outputTreeggHiNtuplizer->Branch("phoEtCorrected_resSys_sig", &phoEtCorrected_resSys_sig);
            outputTreeggHiNtuplizer->Branch("phoEtCorrected_resSys_sig2", &phoEtCorrected_resSys_sig2);
            outputTreeggHiNtuplizer->Branch("phoEtCorrected_resSys_up", &phoEtCorrected_resSys_up);
            outputTreeggHiNtuplizer->Branch("phoEtCorrected_resSys_down", &phoEtCorrected_resSys_down);
            outputTreeggHiNtuplizer->Branch("pho_sumIsoCorrected", &pho_sumIsoCorrected);
            outputTreeggHiNtuplizer->Branch("pho_sumIso", &pho_sumIso);
            outputTreeggHiNtuplizer->Branch("pho_isEle", &pho_isEle);
            outputTreeggHiNtuplizer->Branch("pho_is2015Noise", &pho_is2015Noise);
            outputTreeHiEvt = treeHiEvt->CloneTree(0);
            outputTreeHiEvt->SetName("HiEvt");
            outputTreeHiEvt->SetTitle("subbranches of hiEvtAnalyzer/HiTree");
            if (doEventWeight)
                outputTreeHiEvt->Branch("weight", &eventWeight, "weight/F");
            if (!isMC)
                outputTreeHiEvt->Branch("Ncoll", &ncoll_data, "Ncoll/F");
            outputTreeSkim = treeSkim->CloneTree(0);
            outputTreeSkim->SetName("skim");
            outputTreeSkim->SetTitle("subbranches of skimanalysis/HltTree");
            if(isHI) { outputTreeSkim->Branch("pcollisionEventSelection", &pcollisionEventSelection, "pcollisionEventSelection/I"); }
            else {
                outputTreeSkim->Branch("pPAprimaryVertexFilter", &pPAprimaryVertexFilter, "pPAprimaryVertexFilter/I");
                outputTreeSkim->Branch("pBeamScrapingFilter", &pBeamScrapingFilter, "pBeamScrapingFilter/I");
            }
            outputTreeSkim->Branch("HBHENoiseFilterResultRun2Loose", &HBHENoiseFilterResultRun2Loose, "HBHENoiseFilterResultRun2Loose/I");

            outputTreeHLT->SetMaxTreeSize(MAXTREESIZE);
            outputTreeHiEvt->SetMaxTreeSize(MAXTREESIZE);
            outputTreeSkim->SetMaxTreeSize(MAXTREESIZE);
            inFile->cd();
        } else {
            output->cd();
            treeHLT->CopyAddresses(outputTreeHLT);
            treeggHiNtuplizer->CopyAddresses(outputTreeggHiNtuplizer);
            treeHiEvt->CopyAddresses(outputTreeHiEvt);
            treeSkim->CopyAddresses(outputTreeSkim);
            inFile->cd();
        }

        Long64_t nentries = treeggHiNtuplizer->GetEntries();
        long long firstEntry = 0;
        long long lastEntry = nentries;
        std::cout << "Total Entries: " << nentries << std::endl;

        if (inputFiles.size() == 1 && nJobs != -1) {
            if (jobNum >= nJobs) {
                std::cout << "jobNum > nJobs, invalid configuration, aborting" << std::endl;
                return 1;
            }

            firstEntry = floor((float)nentries*(float)jobNum/(float)nJobs);
            lastEntry = floor((float)nentries*(float)(jobNum+1)/(float)nJobs);
            if (jobNum == nJobs-1)
                lastEntry = nentries;

            std::cout << "For this job " << jobNum << std::endl;
            std::cout << "First Entry: " << firstEntry << std::endl;
            std::cout << "Final Entry: " << lastEntry << std::endl;
        }

        totalEntries += nentries;
        //for (long long jentry = firstEntry; jentry < 100; jentry++) {
        for (long long jentry = firstEntry; jentry < lastEntry; jentry++) {
            if (jentry % 2000 == 0)
                printf("current entry = %lli out of %lli : %.1f%%\n", jentry, nentries, jentry*100.0/nentries);
            if(isMC){
                pho_genPID.clear();       
                pho_genStatus.clear();    
                pho_genPt.clear();        
                pho_genEta.clear();       
                pho_genPhi.clear();       
                pho_genE.clear();         
                pho_genEt.clear();        
                pho_genMomPID.clear();    
                pho_genCalIsoDR03.clear();
                pho_genCalIsoDR04.clear();
                pho_genTrkIsoDR03.clear();
                pho_genTrkIsoDR04.clear();
            }
            phoPreScale.clear();
            phoEtCorrected.clear();
            phoEtCorrected_sys.clear();
            phoEtCorrected_resSys_rms.clear();
            phoEtCorrected_resSys_rms2.clear();
            phoEtCorrected_resSys_sig.clear();
            phoEtCorrected_resSys_sig2.clear();
            phoEtCorrected_resSys_up.clear();
            phoEtCorrected_resSys_down.clear();
            pho_sumIsoCorrected.clear();
            pho_sumIso.clear();
            pho_isEle.clear();
            pho_is2015Noise.clear();

            treeHLT->GetEntry(jentry);
            treeggHiNtuplizer->GetEntry(jentry);
            treeSkim->GetEntry(jentry);
            treeHiEvt->GetEntry(jentry);

            eventWeight = 1;
            if (doEventWeight && isEmEnr==0) {
                if (pthat >= 14.99 && pthat < 30.) {
                    eventWeight = mcPthatWeights[0];
                } else if (pthat >= 30. && pthat < 50.) {
                    eventWeight = mcPthatWeights[1];
                } else if (pthat >= 50. && pthat < 80.) {
                    eventWeight = mcPthatWeights[2];
                } else if (pthat >= 80. && pthat < 120.) {
                    eventWeight = mcPthatWeights[3];
                } else if (pthat >= 120.) {
                    eventWeight = mcPthatWeights[4];
                } else {
                    eventWeight = 0;
                    std::cout << "ERROR: bad pthat value: " << pthat << std::endl;
                }
                
                eventWeight *= hweights_vz->GetBinContent(hweights_vz->FindBin(vz));
                if (isHI)
                    eventWeight *= hweights_cent->GetBinContent(hweights_cent->FindBin(hiBin));

            } else if(doEventWeight && isEmEnr==1) {
                if (pthat >= 29.99 && pthat < 50.) {
                    eventWeight = mcPthatWeights[0];
                } else if (pthat >= 50. && pthat < 80.) {
                    eventWeight = mcPthatWeights[1];
                } else if (pthat >= 80. && pthat < 120.) {
                    eventWeight = mcPthatWeights[2];
                } else if (pthat >= 120. && pthat < 170.) {
                    eventWeight = mcPthatWeights[3];
                } else if (pthat >= 170.) {
                    eventWeight = mcPthatWeights[4];
                } else {
                    eventWeight = 0;
                    std::cout << "ERROR: bad pthat value: " << pthat << std::endl;
                }
                
                eventWeight *= hweights_vz->GetBinContent(hweights_vz->FindBin(vz));
                if (isHI)
                    eventWeight *= hweights_cent->GetBinContent(hweights_cent->FindBin(hiBin));

            }

            bool eventAdded = em->addEvent(run, lumis, event, jentry);
            if (!eventAdded) { // this event is duplicate, skip this one.
                duplicateEntries++;
                continue;
            }

            // event selection
            if (!(TMath::Abs(vz) < cut_vz))
                continue;
            if (isHI) {
                if ((pcollisionEventSelection < cut_pcollisionEventSelection))
                    continue;
                if (!isMC && (HBHENoiseFilterResultRun2Loose < cut_pcollisionEventSelection))
                    continue; // re-use config value...
            } else {
                if (pPAprimaryVertexFilter < cut_pPAprimaryVertexFilter || pBeamScrapingFilter < cut_pBeamScrapingFilter)
                    continue;
            }

            entriesPassedEventSelection++;

            // find leading photon
            int phoIdx = -1;     // index of the leading photon
            double maxPhoEt = -1;

            for (int i=0; i<ggHi.nPho; ++i) {
                int ieta = TMath::Abs((*ggHi.phoEta)[i]) < 1.44 ? 0 : 1;

                // apply corrections to every photon
                double sumIso = (*ggHi.pho_ecalClusterIsoR4)[i] + (*ggHi.pho_hcalRechitIsoR4)[i] + (*ggHi.pho_trackIsoR4PtCut20)[i];
                double phoEt_corrected = 0;
                double resCorr1 = 0; 
                double resCorr2 = 0;
                double sigCorr1 = 0;
                double sigCorr2 = 0;
                double resUp = 0;
                double resDown = 0;
                if (isHI) {
                    int icent = 0;
                    for (; hiBin>=centBins[1][icent] && icent<nCentBins; ++icent);

                    //cout << "hiBin = " << hiBin << " :: " << centBins[0][icent] << " <  centrality < " << centBins[1][icent] << endl;
                    if ((*ggHi.phoEt)[i] > 10)
                        phoEt_corrected = (*ggHi.phoEt)[i] / photonEnergyCorrections[icent][ieta]->Eval((*ggHi.phoEt)[i]);
                        //phoEt_corrected = (*ggHi.phoEt)[i] / photonEnergyCorrections[icent][ieta]->GetBinContent(photonEnergyCorrections[icent][ieta]->FindBin((*ggHi.phoEt)[i]));
                    phoEtCorrected.push_back(phoEt_corrected);
                    pho_sumIso.push_back(sumIso);
                    int icent_sumIso = 0;
                    for (; hiBin>=centBins_sumIso[1][icent_sumIso] && icent_sumIso<nCentBins_sumIso; ++icent_sumIso);
                    pho_sumIsoCorrected.push_back(sumIso - sumIsoCorrections[icent_sumIso]->GetBinContent(sumIsoCorrections[icent_sumIso]->FindBin(getAngleToEP(fabs((*ggHi.phoPhi)[i] - hiEvtPlanes[8])))));

                    // systematic variations from Ran
                    // MC   0 - 30%   Z mass: 9.094649e+01
                    // Data 0 - 30%   Z mass: 9.000079e+01
                    // MC   30 - 100% Z mass: 9.094943e+01
                    // Data 30 - 100% Z mass: 9.064840e+01
                    //phoEt_corrected = (hiBin < 60) ? phoEt_corrected * (90.94649 / 90.00079) : phoEt_corrected * (90.94943 / 90.64840);
                    
                    //Corrections/photonEnergyCorrections_pbpb_COMB_AllQCD_0726.root
                    //Centrality : 0 - 20
                    //Invariant Mass(MC RECO) / Invariant Mass(DATA RECO):    90.5593 / 89.9524
                    //Centrality : 20 - 60
                    //Invariant Mass(MC RECO) / Invariant Mass(DATA RECO):    90.5602 / 88.9576
                    //Centrality : 60 - 100
                    //Invariant Mass(MC RECO) / Invariant Mass(DATA RECO):    90.1456 / 89.8954
                    //Centrality : 100 - 200
                    //Invariant Mass(MC RECO) / Invariant Mass(DATA RECO):    90.1842 / 88.9759
                    if(hiBin>=0 && hiBin<20) phoEt_corrected = phoEt_corrected * 90.5593 / 89.9524 ; 
                    else if(hiBin>=20 && hiBin<60) phoEt_corrected = phoEt_corrected * 90.5602 / 88.9576; 
                    else if(hiBin>=60 && hiBin<100) phoEt_corrected = phoEt_corrected * 90.1456 / 89.8954; 
                    else phoEt_corrected = phoEt_corrected * 90.1842 / 88.9759; 
                    phoEtCorrected_sys.push_back(phoEt_corrected);
                    

                    // energy resolution systematic
                    float mean = photonEnergyCorrections[icent][ieta]->Eval((*ggHi.phoEt)[i]);
                    float rms = photonEnergyCorrections_res_rms[icent][ieta]->Eval((*ggHi.phoEt)[i]);
                    float sig = photonEnergyCorrections_res_sig[icent][ieta]->Eval((*ggHi.phoEt)[i]);
                    resUp = (*ggHi.phoEt)[i] /(mean+sig);
                    resDown = (*ggHi.phoEt)[i] /(mean-sig);
                    if ((*ggHi.phoEt)[i] > 10){
                        resCorr1 = (*ggHi.phoEt)[i] /(r3->Gaus(mean,rms));
                        resCorr2 = (*ggHi.phoEt)[i] /(r3->Gaus(mean,rms));
                        sigCorr1 = (*ggHi.phoEt)[i] /(r3->Gaus(mean,sig));
                        sigCorr2 = (*ggHi.phoEt)[i] /(r3->Gaus(mean,sig));
                    }
                   // if(jentry<50) cout << "pt = " << (*ggHi.phoEt)[i] << ", eta = " << (*ggHi.phoEta)[i] << "scale factor = " << mean << ", resolution = " << rms << ", resCorr = " << resCorr1 << ", "  << resCorr2  << endl; 
                    phoEtCorrected_resSys_rms.push_back(resCorr1);
                    phoEtCorrected_resSys_rms2.push_back(resCorr2);
                    phoEtCorrected_resSys_sig.push_back(sigCorr1);
                    phoEtCorrected_resSys_sig2.push_back(sigCorr2);
                    phoEtCorrected_resSys_up.push_back(resUp);
                    phoEtCorrected_resSys_down.push_back(resDown);


                } else { // pp
                    if ((*ggHi.phoEt)[i] > 10)
                        phoEt_corrected = (*ggHi.phoEt)[i] / photonEnergyCorrections_pp[ieta]->Eval((*ggHi.phoEt)[i]);

                    phoEtCorrected.push_back(phoEt_corrected);
                    pho_sumIso.push_back(sumIso);
                    pho_sumIsoCorrected.push_back(sumIso);
                    
                    //Invariant Mass(MC RECO) / Invariant Mass(DATA RECO):    90.702 / 92.1719 //pp_DSCB_AllQCD_0713 
                    //Invariant Mass(MC RECO) / Invariant Mass(DATA RECO):    90.729 / 92.2026 //pp_COMB_AllQCD_0729 
                    phoEt_corrected = phoEt_corrected * 90.729 / 92.2026;// after electron matching in the systematic
                    phoEtCorrected_sys.push_back(phoEt_corrected);
                    
                    // energy resolution systematic
                    float mean = photonEnergyCorrections_pp[ieta]->Eval((*ggHi.phoEt)[i]);
                    float rms = photonEnergyCorrections_res_rms_pp[ieta]->Eval((*ggHi.phoEt)[i]);
                    float sig = photonEnergyCorrections_res_sig_pp[ieta]->Eval((*ggHi.phoEt)[i]);
                    resUp = (*ggHi.phoEt)[i] /(mean+sig);
                    resDown = (*ggHi.phoEt)[i] /(mean-sig);
                    if ((*ggHi.phoEt)[i] > 10){
                        resCorr1 = (*ggHi.phoEt)[i] / (r3->Gaus(mean,rms));
                        resCorr2 = (*ggHi.phoEt)[i] / (r3->Gaus(mean,rms));
                        sigCorr1 = (*ggHi.phoEt)[i] / (r3->Gaus(mean,sig));
                        sigCorr2 = (*ggHi.phoEt)[i] / (r3->Gaus(mean,sig));
                    }
                   // if(jentry<50) cout << "pt = " << (*ggHi.phoEt)[i] << ", eta = " << (*ggHi.phoEta)[i] << "scale factor = " << mean << ", resolution = " << rms << ", resCorr = " << resCorr1 << ", "  << resCorr2  << endl; 
                    phoEtCorrected_resSys_rms.push_back(resCorr1);
                    phoEtCorrected_resSys_rms2.push_back(resCorr2);
                    phoEtCorrected_resSys_sig.push_back(sigCorr1);
                    phoEtCorrected_resSys_sig2.push_back(sigCorr2);
                    phoEtCorrected_resSys_up.push_back(resUp);
                    phoEtCorrected_resSys_down.push_back(resDown);
                
                }
        
                //////////////////// PreScale /////////////////////
                int prescl = 1;
                if(isMC){ 
                    prescl = 1; 
                } else if(isHighPhotonPD){// high pT 
                    if(isHI){
                        //if(HLT_HISinglePhoton40_Eta1p5_v1==1) prescl = HLT_HISinglePhoton40_Eta1p5_v1_Prescl * L1_SingleEG21_BptxAND_Prescl;
                        //else if(HLT_HISinglePhoton30_Eta1p5_v1==1) prescl = HLT_HISinglePhoton30_Eta1p5_v1_Prescl * L1_SingleEG7_BptxAND_Prescl;
                        if(HLT_HISinglePhoton40_Eta1p5_v1==1) prescl = 1; 
                        else prescl = 0;
                    } else{
                        //if(HLT_HISinglePhoton40_Eta1p5_v1==1) prescl = HLT_HISinglePhoton40_Eta1p5_v1_Prescl * L1_SingleEG20_BptxAND_Prescl;
                        //else if(HLT_HISinglePhoton30_Eta1p5_v1==1) prescl = HLT_HISinglePhoton30_Eta1p5_v1_Prescl * L1_SingleEG12_BptxAND_Prescl;
                        if(HLT_HISinglePhoton40_Eta1p5_v1==1) prescl = 1; 
                        else prescl = 0;
                    }
                } else if(isHI){
                    if(run >=262620 && run <=263035){ //HIHardProbes prescale 3 for [20-30]
                       // if(HLT_HISinglePhoton40_Eta1p5_v1==1) prescl = HLT_HISinglePhoton40_Eta1p5_v1_Prescl * L1_SingleEG21_BptxAND_Prescl;
                       // else if(HLT_HISinglePhoton30_Eta1p5_v1==1) prescl = HLT_HISinglePhoton30_Eta1p5_v1_Prescl * L1_SingleEG7_BptxAND_Prescl;
                       // if(HLT_HISinglePhoton30_Eta1p5_v1==1) prescl = HLT_HISinglePhoton30_Eta1p5_v1_Prescl * L1_SingleEG7_BptxAND_Prescl;
                        //else if(HLT_HISinglePhoton15_Eta1p5_v1==1) prescl = HLT_HISinglePhoton15_Eta1p5_v1_Prescl * L1_MinimumBiasHF1_AND_Prescl;
                        //else if(HLT_HISinglePhoton10_Eta1p5_v1==1) prescl = HLT_HISinglePhoton10_Eta1p5_v1_Prescl * L1_MinimumBiasHF1_AND_Prescl;
                        if(HLT_HISinglePhoton20_Eta1p5_v1==1 && L1_MinimumBiasHF1_AND_Prescl<1000 && L1_MinimumBiasHF1_AND_Prescl>0) prescl = HLT_HISinglePhoton20_Eta1p5_v1_Prescl * L1_MinimumBiasHF1_AND_Prescl;
                        else if(HLT_HISinglePhoton20_Eta1p5_v1==1 && L1_MinimumBiasHF1_AND_Prescl==-1) prescl = HLT_HISinglePhoton20_Eta1p5_v1_Prescl;
                        else prescl = 0; 
                    } else if(run >= 263233 && run <= 263614){ // HIHardProbesPhotons prescale 2 for [20-30]
                       // if(HLT_HISinglePhoton40_Eta1p5_v1==1) prescl = HLT_HISinglePhoton40_Eta1p5_v1_Prescl * L1_SingleEG21_BptxAND_Prescl;
                       // if(HLT_HISinglePhoton30_Eta1p5_v1==1) prescl = HLT_HISinglePhoton30_Eta1p5_v1_Prescl * L1_SingleEG7_BptxAND_Prescl;
                        if(HLT_HISinglePhoton20_Eta1p5_v2==1 && L1_MinimumBiasHF2_AND_Prescl<1000 && L1_MinimumBiasHF2_AND_Prescl>0) prescl = HLT_HISinglePhoton20_Eta1p5_v2_Prescl * L1_MinimumBiasHF2_AND_Prescl;
                        else if(HLT_HISinglePhoton20_Eta1p5_v2==1 && L1_MinimumBiasHF2_AND_Prescl==-1) prescl = HLT_HISinglePhoton20_Eta1p5_v2_Prescl;
                        else prescl = 0; 
                        //else if(HLT_HISinglePhoton15_Eta1p5_v2==1) prescl = HLT_HISinglePhoton15_Eta1p5_v2_Prescl * L1_MinimumBiasHF2_AND_Prescl;
                        //else if(HLT_HISinglePhoton10_Eta1p5_v2==1) prescl = HLT_HISinglePhoton10_Eta1p5_v2_Prescl * L1_MinimumBiasHF2_AND_Prescl;
                       // if(HLT_HISinglePhoton30_Eta1p5_v1==1) prescl = 1;
                       // else if(HLT_HISinglePhoton20_Eta1p5_v2==1 || HLT_HISinglePhoton30_Eta1p5_v1==0) prescl = HLT_HISinglePhoton20_Eta1p5_v2_Prescl;
                       // else prescl = 0; 
                    } else { // weird run 
                        prescl = 0; 
                    }
                } else{ //pp
                        //if(HLT_HISinglePhoton40_Eta1p5_v1==1) prescl = HLT_HISinglePhoton40_Eta1p5_v1_Prescl * L1_SingleEG20_BptxAND_Prescl;
                        //else if(HLT_HISinglePhoton30_Eta1p5_v1==1) prescl = HLT_HISinglePhoton30_Eta1p5_v1_Prescl * L1_SingleEG12_BptxAND_Prescl;
                        //else if(HLT_HISinglePhoton20_Eta1p5_v1==1) prescl = HLT_HISinglePhoton20_Eta1p5_v1_Prescl * L1_SingleEG5_BptxAND_Prescl;
                        if(HLT_HISinglePhoton20_Eta1p5_v1==1 && L1_SingleEG5_BptxAND_Prescl>0 && L1_SingleEG5_BptxAND_Prescl<1000) prescl = HLT_HISinglePhoton20_Eta1p5_v1_Prescl * L1_SingleEG5_BptxAND_Prescl;
                        else if(HLT_HISinglePhoton20_Eta1p5_v1==1 && L1_SingleEG5_BptxAND_Prescl==-1) prescl = HLT_HISinglePhoton20_Eta1p5_v1_Prescl;
                        else prescl = 0; 
                        //else if(HLT_HISinglePhoton15_Eta1p5_v1==1) prescl = HLT_HISinglePhoton15_Eta1p5_v1_Prescl * L1_SingleEG5_BptxAND_Prescl;
                        //else if(HLT_HISinglePhoton10_Eta1p5_v1==1) prescl = HLT_HISinglePhoton10_Eta1p5_v1_Prescl * L1_SingleEG5_BptxAND_Prescl;
                }
                
                phoPreScale.push_back(prescl);
                
                
                // fill photon gen variables
                if(isMC){
                    int matchedIndex = (*ggHi.pho_genMatchedIndex)[i];
                    pho_genPID.push_back((*ggHi.mcPID)[matchedIndex]);
                    if(matchedIndex!=-1){
                        pho_genPID.push_back((*ggHi.mcPID)[matchedIndex]);
                        pho_genStatus.push_back((*ggHi.mcStatus)[matchedIndex]);
                        pho_genPt.push_back((*ggHi.mcPt)[matchedIndex]);
                        pho_genEta.push_back((*ggHi.mcEta)[matchedIndex]);
                        pho_genPhi.push_back((*ggHi.mcPhi)[matchedIndex]);
                        pho_genE.push_back((*ggHi.mcE)[matchedIndex]);
                        pho_genEt.push_back((*ggHi.mcEt)[matchedIndex]);
                        pho_genMomPID.push_back((*ggHi.mcMomPID)[matchedIndex]);
                        pho_genCalIsoDR03.push_back((*ggHi.mcCalIsoDR03)[matchedIndex]);
                        pho_genCalIsoDR04.push_back((*ggHi.mcCalIsoDR04)[matchedIndex]);
                        pho_genTrkIsoDR03.push_back((*ggHi.mcTrkIsoDR03)[matchedIndex]);
                        pho_genTrkIsoDR04.push_back((*ggHi.mcTrkIsoDR04)[matchedIndex]);
                    } else{
                        pho_genPID.push_back(-100);
                        pho_genStatus.push_back(-100);
                        pho_genPt.push_back(-100);
                        pho_genEta.push_back(-100);
                        pho_genPhi.push_back(-100);
                        pho_genE.push_back(-100);
                        pho_genEt.push_back(-100);
                        pho_genMomPID.push_back(-100);
                        pho_genCalIsoDR03.push_back(-100);
                        pho_genCalIsoDR04.push_back(-100);
                        pho_genTrkIsoDR03.push_back(-100);
                        pho_genTrkIsoDR04.push_back(-100);
                    }
                }

                // fill noise variable
                int phois2015Noise = 0;
                if (((*ggHi.phoEt)[i] > 10) && 
                        ((*ggHi.phoE3x3)[i]/(*ggHi.phoE5x5)[i] > 2./3.-0.03 &&
                         (*ggHi.phoE3x3)[i]/(*ggHi.phoE5x5)[i] < 2./3.+0.03) &&
                        ((*ggHi.phoE1x5)[i]/(*ggHi.phoE5x5)[i] > 1./3.-0.03 &&
                         (*ggHi.phoE1x5)[i]/(*ggHi.phoE5x5)[i] < 1./3.+0.03) &&
                        ((*ggHi.phoE2x5)[i]/(*ggHi.phoE5x5)[i] > 2./3.-0.03 &&
                         (*ggHi.phoE2x5)[i]/(*ggHi.phoE5x5)[i] < 2./3.+0.03)) {
                    phois2015Noise = 1;
                }
                pho_is2015Noise.push_back(phois2015Noise);

                // electron rejection
                int phoisEle = 0;
                float eleEpTemp = 100.0;
                if ((*ggHi.phoEt)[i] > 10){
                    for (int ie=0; ie<ggHi.nEle; ++ie) {
                        if ((*ggHi.elePt)[ie] < 10)
                            continue;
                        if (abs((*ggHi.eleEta)[ie] - (*ggHi.phoEta)[i]) > 0.03) // deta
                            continue;
                        if (abs(getDPHI((*ggHi.elePhi)[ie], (*ggHi.phoPhi)[i])) > 0.03) // dphi
                            continue;
                        if (eleEpTemp < (*ggHi.eleEoverP)[ie])
                            continue;

                        phoisEle = 1;
                        break;
                    }
                }
                pho_isEle.push_back(phoisEle);

                bool failedEtCut = (ggHi.phoEt->at(i) < cutPhoEt);
                if (failedEtCut)
                    continue;
                bool failedEtaCut = (TMath::Abs(ggHi.phoEta->at(i)) > cutPhoEta);
                if (failedEtaCut)
                    continue;
                bool failedSpikeRejection;
                failedSpikeRejection = (ggHi.phoSigmaIEtaIEta->at(i) < 0.002 ||
                        ggHi.pho_swissCrx->at(i)     > 0.9   ||
                        TMath::Abs(ggHi.pho_seedTime->at(i)) > 3);
                if (failedSpikeRejection)
                    continue;

                if (ggHi.phoEt->at(i) > maxPhoEt) {
                    maxPhoEt = ggHi.phoEt->at(i);
                    phoIdx = i;
                }

            }

            if (phoIdx == -1)
                continue;

            entriesAnalyzed++;

            outputTreeHLT->Fill();
            outputTreeggHiNtuplizer->Fill();
            outputTreeHiEvt->Fill();
            outputTreeSkim->Fill();

        }
        inFile->Close();
    } // files loop

    std::cout << "Loop ENDED : ggHiNtuplizer/EventTree" << std::endl;
    std::cout << "entries            = " << totalEntries << std::endl;
    std::cout << "duplicateEntries   = " << duplicateEntries << std::endl;
    std::cout << "entriesPassedEventSelection   = " << entriesPassedEventSelection << std::endl;
    std::cout << "entriesAnalyzed               = " << entriesAnalyzed << std::endl;
    std::cout << "outputTreeHLT->GetEntries()   = " << outputTreeHLT->GetEntries() << std::endl;
    std::cout << "outputTreeggHiNtuplizer->GetEntries()   = " << outputTreeggHiNtuplizer->GetEntries() << std::endl;
    std::cout << "outputTreeSkim->GetEntries()  = " << outputTreeSkim->GetEntries() << std::endl;
    std::cout << "outputTreeHiEvt->GetEntries() = " << outputTreeHiEvt->GetEntries() << std::endl;

    output->cd();
    configTree->Write("", TObject::kOverwrite);

    output->Write("", TObject::kOverwrite);
    output->Close();

    std::cout << "photonRaaSkim() - END" << std::endl;

    return 0;
}

double getAngleToEP(double angle) {
    angle = (angle > TMath::Pi()) ? 2 * TMath::Pi() - angle : angle;
    return (angle > TMath::Pi()/2) ? TMath::Pi() - angle : angle;
}

int main(int argc, char** argv) {
    if (argc == 8)
        return photonRaaSkim(argv[1], argv[2], argv[3], atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), atoi(argv[7]));
    else if (argc == 7)
        return photonRaaSkim(argv[1], argv[2], argv[3], atoi(argv[4]), atoi(argv[5]), atoi(argv[6]));
    else if (argc == 6)
        return photonRaaSkim(argv[1], argv[2], argv[3], atoi(argv[4]), atoi(argv[5]));
    else if (argc == 5)
        return photonRaaSkim(argv[1], argv[2], argv[3], atoi(argv[4]));
    else if (argc == 4)
        return photonRaaSkim(argv[1], argv[2], argv[3]);
    else if (argc == 3)
        return photonRaaSkim(argv[1], argv[2]);
    else
        printf("Usage : \n"
                "./photonRaaSkim_workingOn.exe <configFile> <inputFile> <outputFile>\n");

    return 1;
}

