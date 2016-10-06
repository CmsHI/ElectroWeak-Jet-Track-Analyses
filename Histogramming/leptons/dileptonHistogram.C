/*
 * macro to draw and study dilepton spectra
 */
#include <TFile.h>
#include <TTree.h>
#include <TMath.h>
#include <TCut.h>
#include <TH1D.h>
#include <TCanvas.h>

#include <iostream>

#include "../../TreeHeaders/dielectronTree.h"
#include "../../TreeHeaders/dimuonTree.h"
#include "../../TreeHeaders/CutConfigurationTree.h"
#include "../../Utilities/interface/InputConfigurationParser.h"
#include "../../Utilities/interface/CutConfigurationParser.h"

namespace DILEPTON{
enum PAIRTYPE{
        kOppCh,
        kSameCh,
        kN_PAIRTYPE
    };
};

void dileptonHistogram(const TString configFile, const TString inputFile, const TString outputFile = "dileptonHistogram.root");

void dileptonHistogram(const TString configFile, const TString inputFile, const TString outputFile)
{
    std::cout<<"running dileptonHistogram()" <<std::endl;
    std::cout<<"configFile = "<< configFile.Data() <<std::endl;
    std::cout<<"inputFile  = "<< inputFile.Data() <<std::endl;
    std::cout<<"outputFile = "<< outputFile.Data() <<std::endl;

    InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

    if (!configInput.isValid) {
        std::cout << "Input configuration is invalid." << std::endl;
        std::cout << "exiting" << std::endl;
        return;
    }
    if (!configCuts.isValid) {
        std::cout << "Cut configuration is invalid." << std::endl;
        std::cout << "exiting" << std::endl;
        return;
    }

    // input configuration
    int collision = configInput.proc[INPUT::kHISTOGRAM].i[INPUT::k_collisionType];

    // verbose about input configuration
    std::cout<<"Input Configuration :"<<std::endl;
    const char* collisionName =  getCollisionTypeName((COLL::TYPE)collision).c_str();
    std::cout << "collision = " << collisionName << std::endl;

    bool isMC = collisionIsMC((COLL::TYPE)collision);
    bool isHI = collisionIsHI((COLL::TYPE)collision);

    TTree* configTree = setupConfigurationTreeForWriting(configCuts);

    // cut configuration
    // observable bins
    std::vector<int> bins_pt[2];          // array of vectors for electron pt bins, each array element is a vector.
    std::vector<int> bins_hiBin[2];     // array of vectors for hiBin bins, each array element is a vector.
    bins_pt[0] = ConfigurationParser::ParseListInteger(
            configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].s[CUTS::ELE::k_bins_pt_gt]);
    bins_pt[1] = ConfigurationParser::ParseListInteger(
            configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].s[CUTS::ELE::k_bins_pt_lt]);
    bins_hiBin[0] = ConfigurationParser::ParseListInteger(
            configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_gt]);
    bins_hiBin[1] = ConfigurationParser::ParseListInteger(
            configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_lt]);

    // event cuts/weights
    int doEventWeight = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].i[CUTS::EVT::k_doEventWeight];

    // Z boson cuts
    int doDiElectron = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kZBOSON].i[CUTS::ZBO::k_doDiElectron];
    int doDiMuon = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kZBOSON].i[CUTS::ZBO::k_doDiMuon];
    float massMin = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kZBOSON].f[CUTS::ZBO::k_massMin];
    float massMax = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kZBOSON].f[CUTS::ZBO::k_massMax];
    float zPt = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kZBOSON].f[CUTS::ZBO::k_pt];

    // electron cuts
    std::vector<std::string> triggersEle = ConfigurationParser::ParseList(configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].s[CUTS::ELE::k_trigger].c_str());

    // Barrel
    float eleSigmaIEtaIEta_2012_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleSigmaIEtaIEta_2012_EB];
    float eledEtaAtVtx_abs_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eledEtaAtVtx_abs_EB];
    float eledPhiAtVtx_abs_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eledPhiAtVtx_abs_EB];
    float eleHoverE_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleHoverE_EB];
    float eleEoverPInv_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleEoverPInv_EB];
    float eleD0_abs_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleD0_abs_EB];
    float eleDz_abs_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleDz_abs_EB];
    int eleMissHits_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].i[CUTS::ELE::k_eleMissHits_EB];

    // Endcap
    float eleSigmaIEtaIEta_2012_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleSigmaIEtaIEta_2012_EE];
    float eledEtaAtVtx_abs_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eledEtaAtVtx_abs_EE];
    float eledPhiAtVtx_abs_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eledPhiAtVtx_abs_EE];
    float eleHoverE_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleHoverE_EE];
    float eleEoverPInv_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleEoverPInv_EE];
    float eleD0_abs_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleD0_abs_EE];
    float eleDz_abs_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleDz_abs_EE];
    int eleMissHits_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].i[CUTS::ELE::k_eleMissHits_EE];

    // muon cuts
    std::vector<std::string> triggersMu = ConfigurationParser::ParseList(configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kMUON].s[CUTS::MUO::k_trigger]);

    float muChi2NDF = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kMUON].f[CUTS::MUO::k_muChi2NDF];
    float muInnerD0 = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kMUON].f[CUTS::MUO::k_muInnerD0];
    float muInnerDz = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kMUON].f[CUTS::MUO::k_muInnerDz];
    int muMuonHits = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kMUON].i[CUTS::MUO::k_muMuonHits];
    int muStations = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kMUON].i[CUTS::MUO::k_muStations];
    int muTrkLayers = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kMUON].i[CUTS::MUO::k_muTrkLayers];
    int muPixelHits = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kMUON].i[CUTS::MUO::k_muPixelHits];
    // default values

    int nBins_pt = bins_pt[0].size();         // assume <myvector>[0] and <myvector>[1] have the same size.
    int nBins_hiBin = bins_hiBin[0].size();   // assume <myvector>[0] and <myvector>[1] have the same size.
    int nTriggersEle = triggersEle.size();
    int nTriggersMu  = triggersMu.size();

    // verbose about cut configuration
    std::cout<<"Cut Configuration :"<<std::endl;
    std::cout << "nBins_pt = " << nBins_pt << std::endl;
    for (int i=0; i<nBins_pt; ++i) {
        std::cout << Form("bins_pt[%d] = [%d, %d)", i, bins_pt[0].at(i), bins_pt[1].at(i)) << std::endl;
    }
    std::cout << "nBins_hiBin = " << nBins_hiBin << std::endl;
    for (int i=0; i<nBins_hiBin; ++i) {
        std::cout << Form("bins_hiBin[%d] = [%d, %d)", i, bins_hiBin[0].at(i), bins_hiBin[1].at(i)) << std::endl;
    }

    std::cout<<"doEventWeight = "<<doEventWeight<<std::endl;

    std::cout<<"massMin = "<< massMin <<std::endl;
    std::cout<<"massMax = "<< massMax <<std::endl;
    std::cout<<"zPt = "    << zPt <<std::endl;

    std::cout<<"doDiElectron = "<<doDiElectron<<std::endl;
    if (doDiElectron > 0) {
        std::cout<<"nTriggersEle = "<< nTriggersEle <<std::endl;
        for (int i=0; i<nTriggersEle; ++i) {
            std::cout << Form("triggersEle[%d] = %s", i, triggersEle.at(i).c_str()) << std::endl;
        }

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
    }

    std::cout<<"doDiMuon = "<<doDiMuon<<std::endl;
    if (doDiMuon > 0) {
        std::cout<<"nTriggersMu = "<< nTriggersMu <<std::endl;
        for (int i=0; i<nTriggersMu; ++i) {
            std::cout << Form("triggersMu[%d] = %s", i, triggersMu.at(i).c_str()) << std::endl;
        }

        std::cout<<"muChi2NDF = "<<muChi2NDF<<std::endl;
        std::cout<<"muInnerD0 = "<<muInnerD0<<std::endl;
        std::cout<<"muInnerDz = "<<muInnerDz<<std::endl;
        std::cout<<"muMuonHits = "<<muMuonHits<<std::endl;
        std::cout<<"muStations = "<<muStations<<std::endl;
        std::cout<<"muTrkLayers = "<<muTrkLayers<<std::endl;
        std::cout<<"muPixelHits = "<<muPixelHits<<std::endl;
    }

    if (doDiElectron > 0 && doDiMuon > 0) {
         std::cout<<"WARNING : Both of doDiElectron and doDiMuon options are set."<<std::endl;
         std::cout<<"switch to default option : doDiElectron = 1, doDiMuon = 0"<<std::endl;
         doDiElectron = 1;
         doDiMuon = 0;
     }

    std::vector<std::string> triggers;
    std::string diLeptonM = "";         // name of the branch containing diLepton mass
    std::string diLeptonPt = "";
    std::string diLeptonEta = "";
    std::string diLeptonPhi = "";
    std::string diLeptonTreePath = "";
    std::string leptonSymbol = "";
    if (doDiElectron > 0) {
        triggers = triggersEle;
        diLeptonM = "diEleM";
        diLeptonPt = "diElePt";
        diLeptonEta = "diEleEta";
        diLeptonPhi = "diElePhi";
        diLeptonTreePath = "dielectron";
        leptonSymbol = "e";
    }
    else if (doDiMuon > 0) {
        triggers = triggersMu;
        diLeptonM = "diMuM";
        diLeptonPt = "diMuPt";
        diLeptonEta = "diMuEta";
        diLeptonPhi = "diMuPhi";
        diLeptonTreePath = "dimuon";
        leptonSymbol = "#mu";
    }
    std::string dileptonM_titleX = Form("M^{%s%s} (GeV/c^{2})", leptonSymbol.c_str(), leptonSymbol.c_str());
    std::string dileptonPt_titleX = Form("p^{%s%s}_{T} (GeV/c)", leptonSymbol.c_str(), leptonSymbol.c_str());
    std::string dileptonEta_titleX = Form("#eta^{%s%s}", leptonSymbol.c_str(), leptonSymbol.c_str());
    std::string dileptonPhi_titleX = Form("#phi^{%s%s}", leptonSymbol.c_str(), leptonSymbol.c_str());

    TCanvas* c = new TCanvas("cnv","",600,600);

    // eta bins are not part of cut configuration
    const int nBins_eta = 3;
    // https://twiki.cern.ch/twiki/bin/view/CMS/ElectronPbPb5TeV?rev=5#1_Selection_for_all_centrality_r
    // these are eleSCEta selection, not eleEta (08.03.2016 : switch from eleEta to eleSCEta for rapidity binnings)
    float EB_end = 1.4442;
    float EE_start = 1.566;
    if (!isHI)  {
        EB_end = 1.4791;
        EE_start = 1.4791;
    }
    float bins_eta_gt_Ele[nBins_eta] = {-1,  -1,     EE_start};    // All ECAL, Barrel, Endcap1, Endcap2
    float bins_eta_lt_Ele[nBins_eta] = {2.5, EB_end, 2.5};
    float bins_eta_gt_Mu[nBins_eta]  = {-1,  -1, 1.4791};    // All ECAL, Barrel, Endcap1, Endcap2
    float bins_eta_lt_Mu[nBins_eta]  = {2.4,  2,    2.4};

    float bins_eta_gt[nBins_eta];
    float bins_eta_lt[nBins_eta];
    if (doDiElectron > 0) {
        std::copy(bins_eta_gt_Ele, bins_eta_gt_Ele + nBins_eta, bins_eta_gt);
        std::copy(bins_eta_lt_Ele, bins_eta_lt_Ele + nBins_eta, bins_eta_lt);
    }
    else if (doDiMuon > 0) {
        std::copy(bins_eta_gt_Mu, bins_eta_gt_Mu + nBins_eta, bins_eta_gt);
        std::copy(bins_eta_lt_Mu, bins_eta_lt_Mu + nBins_eta, bins_eta_lt);
    }
    std::cout << "nBins_eta = " << nBins_eta << std::endl;
    for (int i=0; i<nBins_eta; ++i) {
        std::cout << Form("bins_eta[%d] = [%.2f, %.2f)", i, bins_eta_gt[i],  bins_eta_lt[i]) << std::endl;
    }

    std::vector<bool> doHistogram  {true, true, true, true, true, true};   // if want to turn off a histogram, set corresponding value to false
    std::vector<std::string> observableNames {"M", "Pt", "Eta", "Phi", "hiBin", "hiBin_nPart"};
    std::vector<bool> doMassSelection    {false, true,  true, true, true, true};
    std::vector<bool> doZptSelection     {true,  false, true, true, true, true};
    std::vector<std::string> histFormulas{diLeptonM.c_str(), diLeptonPt.c_str(), diLeptonEta.c_str(), diLeptonPhi.c_str(), "hiBin", "hiBin"};
    std::vector<std::string> histTitleX  {dileptonM_titleX.c_str(), dileptonPt_titleX.c_str(),  dileptonEta_titleX.c_str(), dileptonPhi_titleX.c_str(), "hiBin", "hiBin"};
    std::vector<std::string> histTitleY  {"Entries / (2 GeV/c^{2})",
                                                                 Form("#frac{1}{N} #frac{dN}{d%s}", dileptonPt_titleX.c_str()),
                                                                 Form("#frac{1}{N} #frac{dN}{d%s}", dileptonEta_titleX.c_str()),
                                                                 Form("#frac{1}{N} #frac{dN}{d%s}", dileptonPhi_titleX.c_str()),
                                                                     "Entries",
                                                                     "Entries"};
    std::vector<int>     nBinsx{100, 60,  20,    20,       20,     6};
    std::vector<double>  xlow  {0,   0,   -4, -TMath::Pi(), 0,    -1};
    std::vector<double>  xup   {200, 300,  4,  TMath::Pi(),  200, -1};
    std::vector<int>     nBinsx_final{30, 40,  20,    20,         20,  6};
    std::vector<double>  xlow_final  {60,  0,  -4, -TMath::Pi(),   0, -1};
    std::vector<double>  xup_final   {120, 200, 4,  TMath::Pi(), 200, -1};

    // special cases - nPart
    int nNPartBins = 6;
    float hiBinBins[nNPartBins+1] = {0,20,40,60,80,100,200};

    // READ TREES
    TFile *input = new TFile(inputFile);
    TTree *tHlt = (TTree*)input->Get("hltTree");
    TTree *tEvent = (TTree*)input->Get("EventTree");
    TTree *tHiEvt = (TTree*)input->Get("HiEvt");       // HiEvt tree will be placed in PP forest as well.
    TTree *tdiLepton = (TTree*)input->Get(diLeptonTreePath.c_str());
    // READ TREES - END

    // SET BRANCHES ADRESSESS
    // hltTree
    tHlt->SetBranchStatus("*", 0);
    int nTriggers = triggers.size();
    Int_t triggerBits[nTriggers];
    for (int i = 0; i < nTriggers; ++i) {
        tHlt->SetBranchStatus(triggers.at(i).c_str(), 1);
        tHlt->SetBranchAddress(triggers.at(i).c_str(), &triggerBits[i]);
    }

    // EventTree
    tEvent->SetBranchStatus("*", 0);
    tEvent->SetBranchStatus("run", 1);
    tEvent->SetBranchStatus("event", 1);
    tEvent->SetBranchStatus("lumis", 1);

    // HiEvt
    tHiEvt->SetBranchStatus("*", 0);

    float vz;
    Int_t hiBin;
    tHiEvt->SetBranchStatus("vz", 1);
    tHiEvt->SetBranchStatus("hiBin", 1);
    tHiEvt->SetBranchAddress("vz", &vz);
    tHiEvt->SetBranchAddress("hiBin", &hiBin);
    float weight_HiEvt;
    if (doEventWeight > 0) {
        tHiEvt->SetBranchStatus("weight", 1);
        tHiEvt->SetBranchAddress("weight", &weight_HiEvt);
    }
    else {
        weight_HiEvt = 1;
    }

    // gghi
    ggHiNtuplizer ggHi;
    ggHi.setupTreeForReading(tEvent);

    // dilepton tree
    dielectron diEle;
    if (doDiElectron > 0)  diEle.setupDiElectronTree(tdiLepton);     // diElectronTree is output
    dimuon diMu;
    if (doDiMuon > 0) diMu.setupDiMuonTree(tdiLepton);     // diMuonTree is output

    TFile* output = new TFile(outputFile, "UPDATE");

    TH1::SetDefaultSumw2();
    int nHistFormulas = histFormulas.size();
    TH1D* h1D[nHistFormulas][DILEPTON::kN_PAIRTYPE][nBins_eta][nBins_pt][nBins_hiBin];
    std::string histNames[nHistFormulas][DILEPTON::kN_PAIRTYPE][nBins_eta][nBins_pt][nBins_hiBin];

    // x-axis range : 60-120
    TH1D* h1D_final[nHistFormulas][DILEPTON::kN_PAIRTYPE][nBins_eta][nBins_pt][nBins_hiBin];

    for (int i=0; i<nHistFormulas; ++i){
        if (!doHistogram.at(i)) continue;
        for(int iCh=0; iCh<DILEPTON::kN_PAIRTYPE; ++iCh){
            for(int iEta=0; iEta<nBins_eta; ++iEta){
                for (int iPt=0; iPt<nBins_pt; ++iPt){
                    for(int iHiBin=0; iHiBin<nBins_hiBin; ++iHiBin){

                        std::string histName = Form("%s_etaBin%d_ptBin%d_hiBin%d",
                                observableNames.at(i).c_str(), iEta, iPt, iHiBin);
                        if (iCh == DILEPTON::kSameCh) {
                            histName = Form("%s_sameCh", histName.c_str());
                        }
                        histNames[i][iCh][iEta][iPt][iHiBin] = histName.c_str();

                        {   // special cases
                            // special cases - nPart
                            if (observableNames.at(i) == "hiBin_nPart") {
                                nBinsx.at(i) = nNPartBins;
                                xlow.at(i) = hiBinBins[0];
                                xup.at(i)  = hiBinBins[nNPartBins];
                                xlow_final.at(i) = hiBinBins[0];
                                xup_final.at(i)  = hiBinBins[nNPartBins];

                                h1D[i][iCh][iEta][iPt][iHiBin] = new TH1D(Form("h1D_%s", histName.c_str()),"",
                                        nNPartBins, hiBinBins);
                                h1D_final[i][iCh][iEta][iPt][iHiBin] = new TH1D(Form("h1D_%s_final", histName.c_str()),"",
                                        nNPartBins, hiBinBins);

                                continue;
                            }
                        }

                        // make Histogram Title
                        h1D[i][iCh][iEta][iPt][iHiBin] = new TH1D(Form("h1D_%s", histName.c_str()),"",
                                nBinsx.at(i), xlow.at(i), xup.at(i));
                        h1D_final[i][iCh][iEta][iPt][iHiBin] = new TH1D(Form("h1D_%s_final", histName.c_str()),"",
                                nBinsx_final.at(i), xlow_final.at(i), xup_final.at(i));

                        std::string leptonEta_str = Form("|#eta^{%s#pm}|", leptonSymbol.c_str());
                        std::string histoTitle_eta = Form("%s < %.2f", leptonEta_str.c_str() ,bins_eta_lt[iEta]);
                        if (bins_eta_gt[iEta] >= 0)  histoTitle_eta = Form("%.2f< %s <%.1f", bins_eta_gt[iEta], leptonEta_str.c_str(), bins_eta_lt[iEta]);

                        std::string leptonPt_str = Form("p^{%s#pm}_{T}", leptonSymbol.c_str());
                        std::string histoTitle_pt = Form("%s > %d", leptonPt_str.c_str(), bins_pt[0].at(iPt));
                        if (bins_pt[1].at(iPt) >= 0)  histoTitle_pt = Form("%d < %s < %d", bins_pt[0].at(iPt), leptonPt_str.c_str(), bins_pt[1].at(iPt));

                        std::string histoTitle_hiBin = Form("%d-%d %%", bins_hiBin[0].at(iHiBin)/2, bins_hiBin[1].at(iHiBin)/2);

                        std::string histoTitle = "";
                        if (histoTitle_eta.size() > 0)   histoTitle = histoTitle_eta.c_str();
                        if (histoTitle_pt.size() > 0)    histoTitle = Form("%s, %s", histoTitle.c_str(), histoTitle_pt.c_str());
                        if (histoTitle_hiBin.size() > 0) histoTitle = Form("%s, %s", histoTitle.c_str(), histoTitle_hiBin.c_str());
                        // make Histogram Title - END

                        h1D[i][iCh][iEta][iPt][iHiBin]->SetTitle(Form("%s;%s;%s", histoTitle.c_str(), histTitleX.at(i).c_str(), histTitleY.at(i).c_str()));
                        // TH1 style
                        h1D[i][iCh][iEta][iPt][iHiBin]->SetMarkerStyle(kFullCircle);
                        if (iCh == DILEPTON::kSameCh)  h1D[i][iCh][iEta][iPt][iHiBin]->SetMarkerStyle(kOpenCircle);
                        h1D[i][iCh][iEta][iPt][iHiBin]->SetMarkerColor(kBlack);
                        h1D[i][iCh][iEta][iPt][iHiBin]->SetStats(false);     // remove stat box from the canvas, but keep in the histograms.

                        h1D_final[i][iCh][iEta][iPt][iHiBin]->SetTitle(Form("%s;%s;%s", histoTitle.c_str(), histTitleX.at(i).c_str(), histTitleY.at(i).c_str()));
                        // TH1 style : _final
                        h1D_final[i][iCh][iEta][iPt][iHiBin]->SetMarkerStyle(kFullCircle);
                        if (iCh == DILEPTON::kSameCh)  h1D_final[i][iCh][iEta][iPt][iHiBin]->SetMarkerStyle(kOpenCircle);
                        h1D_final[i][iCh][iEta][iPt][iHiBin]->SetMarkerColor(kBlack);
                        h1D_final[i][iCh][iEta][iPt][iHiBin]->SetStats(false);     // remove stat box from the canvas, but keep in the histograms.
                    }
                }
            }
        }
    }

    bool doSameCharge = true;
    Long64_t entries = tdiLepton->GetEntries();
    Long64_t entriesPassedEventSelection = 0;
    Long64_t entriesPassedLeptonSelection[DILEPTON::kN_PAIRTYPE] = {0, 0};
    Long64_t entriesPassedZmassSelection[DILEPTON::kN_PAIRTYPE] = {0, 0};
    Long64_t entriesAnalyzed[DILEPTON::kN_PAIRTYPE] = {0, 0};
    std::cout<<"####################"<<std::endl;
    std::cout<<"tdiLepton->GetEntries() = "<<entries<<std::endl;
    if (nTriggers != 0 && !isMC) {
        std::string triggerStr = triggers.at(0).c_str();
        for (int i = 1; i < nTriggers; ++i) {
            triggerStr = Form ("%s || %s", triggerStr.c_str(), triggers.at(i).c_str());
        }
        std::cout<<"tHlt->GetEntries(triggerStr==1) = "<<tHlt->GetEntries(Form("( %s ) == 1", triggerStr.c_str()))<<std::endl;
    }
    else {
        std::cout<<"tHlt->GetEntries(triggerStr==1) is skipped because either no trigger is specified or the data is coming from MC."<<std::endl;
    }
    std::cout<<"####################"<<std::endl;

    std::cout<< "Loop : tdiLepton" <<std::endl;
    for (Long64_t j_entry=0; j_entry<entries; ++j_entry)
    {
        tHlt->GetEntry(j_entry);
        tEvent->GetEntry(j_entry);
        tHiEvt->GetEntry(j_entry);
        tdiLepton->GetEntry(j_entry);

        // event selection
        bool passedTrigger = false;      // triggers will be "OR"ed
        if (nTriggers == 0) passedTrigger = true;
        for (int i = 0; i < nTriggers; ++i)
        {
            if (triggerBits[i] > 0)  passedTrigger = true;
        }
        if (!passedTrigger)  continue;

        entriesPassedEventSelection++;

        std::vector<int> diLeptonIndices[DILEPTON::kN_PAIRTYPE];
        std::vector<double> diLeptonMs[DILEPTON::kN_PAIRTYPE];
        std::vector<double> diLeptonPts[DILEPTON::kN_PAIRTYPE];
        std::vector<double> diLeptonEtas[DILEPTON::kN_PAIRTYPE];
        std::vector<double> diLeptonPhis[DILEPTON::kN_PAIRTYPE];

        std::vector<double> leptonEtas_1[DILEPTON::kN_PAIRTYPE];
        std::vector<double> leptonEtas_2[DILEPTON::kN_PAIRTYPE];
        std::vector<double> leptonPts_1[DILEPTON::kN_PAIRTYPE];
        std::vector<double> leptonPts_2[DILEPTON::kN_PAIRTYPE];
        // dielectron block
        if (doDiElectron > 0)
        {
            int nDiEle = diEle.diEleM->size();
            for (int i = 0; i < nDiEle; ++i)
            {
                // selection of electron 1 based on ECAL regions
                if (TMath::Abs(diEle.eleSCEta_1->at(i)) < 1.4442)
                {
                    if (!(diEle.eleSigmaIEtaIEta_2012_1->at(i) < eleSigmaIEtaIEta_2012_EB)) continue;
                    if (!(TMath::Abs(diEle.eledEtaAtVtx_1->at(i)) < eledEtaAtVtx_abs_EB)) continue;
                    if (!(TMath::Abs(diEle.eledPhiAtVtx_1->at(i)) < eledPhiAtVtx_abs_EB)) continue;
                    if (!(diEle.eleHoverE_1->at(i) < eleHoverE_EB)) continue;
                    if (!(diEle.eleEoverPInv_1->at(i) < eleEoverPInv_EB)) continue;
                    if (!(TMath::Abs(diEle.eleD0_1->at(i)) < eleD0_abs_EB)) continue;
                    if (!(TMath::Abs(diEle.eleDz_1->at(i)) < eleDz_abs_EB)) continue;
                    if (!(diEle.eleMissHits_1->at(i) <= eleMissHits_EB)) continue;
                }
                else if (TMath::Abs(diEle.eleSCEta_1->at(i)) > 1.566 && TMath::Abs(diEle.eleSCEta_1->at(i)) < 2.5)
                {
                    if (!(diEle.eleSigmaIEtaIEta_2012_1->at(i) < eleSigmaIEtaIEta_2012_EE)) continue;
                    if (!(TMath::Abs(diEle.eledEtaAtVtx_1->at(i)) < eledEtaAtVtx_abs_EE)) continue;
                    if (!(TMath::Abs(diEle.eledPhiAtVtx_1->at(i)) < eledPhiAtVtx_abs_EE)) continue;
                    if (!(diEle.eleHoverE_1->at(i) < eleHoverE_EE)) continue;
                    if (!(diEle.eleEoverPInv_1->at(i) < eleEoverPInv_EE)) continue;
                    if (!(TMath::Abs(diEle.eleD0_1->at(i)) < eleD0_abs_EE)) continue;
                    if (!(TMath::Abs(diEle.eleDz_1->at(i)) < eleDz_abs_EE)) continue;
                    if (!(diEle.eleMissHits_1->at(i) <= eleMissHits_EE)) continue;
                }
                else  {
                    continue;
                }

                // selection of electron 2 based on ECAL regions
                if (TMath::Abs(diEle.eleSCEta_2->at(i)) < 1.4442)
                {
                    if (!(diEle.eleSigmaIEtaIEta_2012_2->at(i) < eleSigmaIEtaIEta_2012_EB)) continue;
                    if (!(TMath::Abs(diEle.eledEtaAtVtx_2->at(i)) < eledEtaAtVtx_abs_EB)) continue;
                    if (!(TMath::Abs(diEle.eledPhiAtVtx_2->at(i)) < eledPhiAtVtx_abs_EB)) continue;
                    if (!(diEle.eleHoverE_2->at(i) < eleHoverE_EB)) continue;
                    if (!(diEle.eleEoverPInv_2->at(i) < eleEoverPInv_EB)) continue;
                    if (!(TMath::Abs(diEle.eleD0_2->at(i)) < eleD0_abs_EB)) continue;
                    if (!(TMath::Abs(diEle.eleDz_2->at(i)) < eleDz_abs_EB)) continue;
                    if (!(diEle.eleMissHits_2->at(i) <= eleMissHits_EB)) continue;
                }
                else if (TMath::Abs(diEle.eleSCEta_2->at(i)) > 1.566 && TMath::Abs(diEle.eleSCEta_2->at(i)) < 2.5)
                {
                    if (!(diEle.eleSigmaIEtaIEta_2012_2->at(i) < eleSigmaIEtaIEta_2012_EE)) continue;
                    if (!(TMath::Abs(diEle.eledEtaAtVtx_2->at(i)) < eledEtaAtVtx_abs_EE)) continue;
                    if (!(TMath::Abs(diEle.eledPhiAtVtx_2->at(i)) < eledPhiAtVtx_abs_EE)) continue;
                    if (!(diEle.eleHoverE_2->at(i) < eleHoverE_EE)) continue;
                    if (!(diEle.eleEoverPInv_2->at(i) < eleEoverPInv_EE)) continue;
                    if (!(TMath::Abs(diEle.eleD0_2->at(i)) < eleD0_abs_EE)) continue;
                    if (!(TMath::Abs(diEle.eleDz_2->at(i)) < eleDz_abs_EE)) continue;
                    if (!(diEle.eleMissHits_2->at(i) <= eleMissHits_EE)) continue;
                }
                else  {
                    continue;
                }

                int iDiLep = -1;
                if (diEle.eleCharge_1->at(i) != diEle.eleCharge_2->at(i))  iDiLep = DILEPTON::kOppCh;
                else if (diEle.eleCharge_1->at(i) == diEle.eleCharge_2->at(i))  iDiLep = DILEPTON::kSameCh;

                diLeptonIndices[iDiLep].push_back(i);
                diLeptonMs[iDiLep].push_back(diEle.diEleM->at(i));
                diLeptonPts[iDiLep].push_back(diEle.diElePt->at(i));
                diLeptonEtas[iDiLep].push_back(diEle.diEleEta->at(i));
                diLeptonPhis[iDiLep].push_back(diEle.diElePhi->at(i));

                leptonEtas_1[iDiLep].push_back(diEle.eleSCEta_1->at(i));
                leptonEtas_2[iDiLep].push_back(diEle.eleSCEta_2->at(i));
                leptonPts_1[iDiLep].push_back(diEle.elePt_1->at(i));
                leptonPts_2[iDiLep].push_back(diEle.elePt_2->at(i));
            }
        }

        // dimuon block
        if (doDiMuon > 0)
        {
            int nDiMu = diMu.diMuM->size();
            for (int i = 0; i < nDiMu; ++i)
            {
                // selection of muon 1
                if (TMath::Abs(diMu.muEta_1->at(i)) < 2.4)
                {
                    if (!(diMu.muChi2NDF_1->at(i) < muChi2NDF)) continue;
                    if (!(TMath::Abs(diMu.muInnerD0_1->at(i)) < muInnerD0)) continue;
                    if (!(TMath::Abs(diMu.muInnerDz_1->at(i)) < muInnerDz)) continue;
                    if (!(diMu.muMuonHits_1->at(i) > muMuonHits)) continue;
                    if (!(diMu.muStations_1->at(i) > muStations)) continue;
                    if (!(diMu.muTrkLayers_1->at(i) > muTrkLayers)) continue;
                    if (!(diMu.muPixelHits_1->at(i) > muPixelHits)) continue;
                }
                else  {
                    continue;
                }

                // selection of muon 2
                if (TMath::Abs(diMu.muEta_2->at(i)) < 2.4)
                {
                    if (!(diMu.muChi2NDF_2->at(i) < muChi2NDF)) continue;
                    if (!(TMath::Abs(diMu.muInnerD0_2->at(i)) < muInnerD0)) continue;
                    if (!(TMath::Abs(diMu.muInnerDz_2->at(i)) < muInnerDz)) continue;
                    if (!(diMu.muMuonHits_2->at(i) > muMuonHits)) continue;
                    if (!(diMu.muStations_2->at(i) > muStations)) continue;
                    if (!(diMu.muTrkLayers_2->at(i) > muTrkLayers)) continue;
                    if (!(diMu.muPixelHits_2->at(i) > muPixelHits)) continue;
                }
                else  {
                    continue;
                }

                int iDiLep = -1;
                if (diMu.muCharge_1->at(i) != diMu.muCharge_2->at(i))  iDiLep = DILEPTON::kOppCh;
                else if (diMu.muCharge_1->at(i) == diMu.muCharge_2->at(i))  iDiLep = DILEPTON::kSameCh;

                diLeptonIndices[iDiLep].push_back(i);
                diLeptonMs[iDiLep].push_back(diMu.diMuM->at(i));
                diLeptonPts[iDiLep].push_back(diMu.diMuPt->at(i));
                diLeptonEtas[iDiLep].push_back(diMu.diMuEta->at(i));
                diLeptonPhis[iDiLep].push_back(diMu.diMuPhi->at(i));

                leptonEtas_1[iDiLep].push_back(diMu.muEta_1->at(i));
                leptonEtas_2[iDiLep].push_back(diMu.muEta_2->at(i));
                leptonPts_1[iDiLep].push_back(diMu.muPt_1->at(i));
                leptonPts_2[iDiLep].push_back(diMu.muPt_2->at(i));
            }
        }

        if (diLeptonIndices[DILEPTON::kOppCh].size() > 0)
            entriesPassedLeptonSelection[DILEPTON::kOppCh]++;
        if (diLeptonIndices[DILEPTON::kSameCh].size() > 0)
            entriesPassedLeptonSelection[DILEPTON::kSameCh]++;


        for (int iCh = 0; iCh < DILEPTON::kN_PAIRTYPE; ++iCh) {

            int nDiLep = diLeptonIndices[iCh].size();
            for (int iDiLep = 0; iDiLep < nDiLep; ++iDiLep) {
               if (diLeptonMs[iCh].at(iDiLep) > massMin && diLeptonMs[iCh].at(iDiLep) < massMax) {
                   entriesPassedZmassSelection[iCh]++;
                   break;
               }
            }
        }

        double eventWeightAll = 1;
        double vertexWeight = 1;
        double centWeight = 1;
        if (doEventWeight > 0) {
            vertexWeight = 1.37487*TMath::Exp(-0.5*TMath::Power((vz-0.30709)/7.41379, 2));  // 02.04.2016
            if (isHI)  centWeight = findNcoll(hiBin);
            eventWeightAll = weight_HiEvt * vertexWeight * centWeight;
        }

        for(int iHist = 0; iHist < nHistFormulas; ++iHist){
            if (!doHistogram.at(iHist)) continue;

            double w = 1;
            w = eventWeightAll;

            for(int iEta=0; iEta<nBins_eta; ++iEta){
                for(int iPt=0; iPt<nBins_pt; ++iPt){
                    for(int iHiBin=0; iHiBin<nBins_hiBin; ++iHiBin){

                        // selection : hiBin
                        if (isHI && !(hiBin >= bins_hiBin[0].at(iHiBin) && hiBin < bins_hiBin[1].at(iHiBin)))  continue;

                        for (int iCh = 0; iCh < DILEPTON::kN_PAIRTYPE; ++iCh) {

                            if (iCh == DILEPTON::kSameCh && !doSameCharge)  continue;

                            int nDiLep = diLeptonIndices[iCh].size();
                            bool pairPassedSelection = false;
                            bool isFilled = false;        // fill only one dilepton pair per event
                            for (int iDiLep = 0; iDiLep < nDiLep; ++iDiLep) {

                                if (isFilled)  continue;  // fill only one dilepton pair per event
                                // SELECTIONS
                                // selection : lepton eta
                                if (iEta == 1) // Barrel,  |eta supercluster| < 1.4442
                                {
                                    if (!(TMath::Abs(leptonEtas_1[iCh].at(iDiLep)) < 1.4442))  continue;
                                    if (!(TMath::Abs(leptonEtas_2[iCh].at(iDiLep)) < 1.4442))  continue;
                                }
                                if ( iEta>1 )  // Endcap,  1.566 < |eta supercluster| < 2.5
                                {
                                    if (!(TMath::Abs(leptonEtas_1[iCh].at(iDiLep)) > 1.566 && TMath::Abs(leptonEtas_1[iCh].at(iDiLep)) < 2.5))  continue;
                                    if (!(TMath::Abs(leptonEtas_2[iCh].at(iDiLep)) > 1.566 && TMath::Abs(leptonEtas_2[iCh].at(iDiLep)) < 2.5))  continue;
                                }

                                // selection : lepton pt
                                if (!(leptonPts_1[iCh].at(iDiLep) > bins_pt[0].at(iPt)))  continue;
                                if (!(leptonPts_2[iCh].at(iDiLep) > bins_pt[0].at(iPt)))  continue;
                                if (bins_pt[1].at(iPt) >= 0) {
                                    if (!(leptonPts_1[iCh].at(iDiLep) <= bins_pt[1].at(iPt)))  continue;
                                    if (!(leptonPts_2[iCh].at(iDiLep) <= bins_pt[1].at(iPt)))  continue;
                                }

                                // special selections : observable dependent selections
                                // selection : Z pt
                                if (doZptSelection.at(iHist))  {
                                    if (!(diLeptonPts[iCh].at(iDiLep) > zPt))  continue;
                                }
                                if (doMassSelection.at(iHist))  {
                                    if (!(diLeptonMs[iCh].at(iDiLep) > massMin && diLeptonMs[iCh].at(iDiLep) < massMax))  continue;
                                }
                                // SELECTIONS - END

                                // FILL HISTOGRAMS
                                std::string tmpObservable = observableNames.at(iHist).c_str();
                                if (tmpObservable == "M") {
                                    h1D      [iHist][iCh][iEta][iPt][iHiBin]->Fill(diLeptonMs[iCh].at(iDiLep), w);
                                    h1D_final[iHist][iCh][iEta][iPt][iHiBin]->Fill(diLeptonMs[iCh].at(iDiLep), w);
                                }
                                else if (tmpObservable == "Pt") {
                                    h1D      [iHist][iCh][iEta][iPt][iHiBin]->Fill(diLeptonPts[iCh].at(iDiLep), w);
                                    h1D_final[iHist][iCh][iEta][iPt][iHiBin]->Fill(diLeptonPts[iCh].at(iDiLep), w);
                                }
                                else if (tmpObservable == "Eta") {
                                    h1D      [iHist][iCh][iEta][iPt][iHiBin]->Fill(diLeptonEtas[iCh].at(iDiLep), w);
                                    h1D_final[iHist][iCh][iEta][iPt][iHiBin]->Fill(diLeptonEtas[iCh].at(iDiLep), w);
                                }
                                else if (tmpObservable == "Phi") {
                                    h1D      [iHist][iCh][iEta][iPt][iHiBin]->Fill(diLeptonPhis[iCh].at(iDiLep), w);
                                    h1D_final[iHist][iCh][iEta][iPt][iHiBin]->Fill(diLeptonPhis[iCh].at(iDiLep), w);
                                }

                                isFilled = true;    // fill only one dilepton pair per event
                                pairPassedSelection = true;
                            }
                            // filled once per event
                            if (pairPassedSelection) {
                                std::string tmpObservable = observableNames.at(iHist).c_str();
                                if (tmpObservable == "hiBin") {
                                    h1D      [iHist][iCh][iEta][iPt][iHiBin]->Fill(hiBin, w);
                                    h1D_final[iHist][iCh][iEta][iPt][iHiBin]->Fill(hiBin, w);
                                }
                            }
                        }
                    }
                }
            }

        }
    }

    std::cout<<  "Loop ENDED : tdiLepton" <<std::endl;
    std::cout << "entries            = " << entries << std::endl;
    std::cout << "entriesPassedEventSelection   = " << entriesPassedEventSelection << std::endl;
    for (int iCh = 0; iCh < DILEPTON::kN_PAIRTYPE; ++iCh)
    {
        if (iCh == DILEPTON::kOppCh)   std::cout << "##### opposite charge #####" << std::endl;
        if (iCh == DILEPTON::kSameCh)  std::cout << "##### same charge #####" << std::endl;

        std::cout << "entriesPassedLeptonSelection  = " << entriesPassedLeptonSelection[iCh] << std::endl;
        std::cout << "entriesPassedZmassSelection   = " << entriesPassedZmassSelection[iCh] << std::endl;
        std::cout << "entriesAnalyzed               = " << entriesAnalyzed[iCh] << std::endl;

        std::cout << "#####" << std::endl;
    }


    for(int iHist=0; iHist<nHistFormulas; ++iHist){
        if (!doHistogram.at(iHist)) continue;

        for(int iEta=0; iEta<nBins_eta; ++iEta){
            for(int iPt=0; iPt<nBins_pt; ++iPt){
                for(int iHiBin=0; iHiBin<nBins_hiBin; ++iHiBin){

                    int iCh1 = DILEPTON::kOppCh;
                    int iCh2 = DILEPTON::kSameCh;

                    c->SetName(Form("cnv_%s", histNames[iHist][iCh1][iEta][iPt][iHiBin].c_str()));
                    c->cd();
                    // scaling with bin width : for histograms with fixed bin width, this scaling is dummy.
                    double firstBinWidth = h1D[iHist][iCh1][iEta][iPt][iHiBin]->GetBinWidth(1);
                    h1D[iHist][iCh1][iEta][iPt][iHiBin]->Scale(firstBinWidth, "width");   // content of 1st bin will not change after scaling.
                    h1D[iHist][iCh1][iEta][iPt][iHiBin]->Draw("e");
                    h1D[iHist][iCh1][iEta][iPt][iHiBin]->Write("", TObject::kOverwrite);

                    if (doSameCharge) {
                        // scaling with bin width : for histograms with fixed bin width, this scaling is dummy.
                        double firstBinWidth_sameCharge = h1D[iHist][iCh2][iEta][iPt][iHiBin]->GetBinWidth(1);
                        h1D[iHist][iCh2][iEta][iPt][iHiBin]->Scale(firstBinWidth_sameCharge, "width");   // content of 1st bin will not change after scaling.
                        h1D[iHist][iCh2][iEta][iPt][iHiBin]->Draw("e same");
                        h1D[iHist][iCh2][iEta][iPt][iHiBin]->Write("", TObject::kOverwrite);
                    }

                    c->Write("", TObject::kOverwrite);
                    c->Clear();

                    c->SetName(Form("cnv_%s_final", histNames[iHist][iCh1][iEta][iPt][iHiBin].c_str()));
                    c->cd();
                    double firstBinWidth_final = h1D_final[iHist][iCh1][iEta][iPt][iHiBin]->GetBinWidth(1);
                    h1D_final[iHist][iCh1][iEta][iPt][iHiBin]->Scale(firstBinWidth_final, "width");   // content of 1st bin will not change after scaling.
                    h1D_final[iHist][iCh1][iEta][iPt][iHiBin]->Draw("e");
                    h1D_final[iHist][iCh1][iEta][iPt][iHiBin]->Write("", TObject::kOverwrite);
                    if (doSameCharge) {
                        double firstBinWidth_sameCharge_final = h1D_final[iHist][iCh2][iEta][iPt][iHiBin]->GetBinWidth(1);
                        h1D_final[iHist][iCh2][iEta][iPt][iHiBin]->Scale(firstBinWidth_sameCharge_final, "width");   // content of 1st bin will not change after scaling.
                        h1D_final[iHist][iCh2][iEta][iPt][iHiBin]->Draw("e same");
                        h1D_final[iHist][iCh2][iEta][iPt][iHiBin]->Write("", TObject::kOverwrite);
                    }
                    c->Write("", TObject::kOverwrite);
                    c->Clear();
                }
            }
        }
    }

    // special cases
    TH1D* h1D_nPart = 0;
    for (int i = 0; i < nHistFormulas; ++i) {
        if (!doHistogram.at(i)) continue;

        for(int iEta=0; iEta<nBins_eta; ++iEta){
            for(int iPt=0; iPt<nBins_pt; ++iPt){
                for(int iHiBin=0; iHiBin<nBins_hiBin; ++iHiBin){

                    // special cases - nPart
                    if (observableNames.at(i) == "hiBin_nPart") {

                        int iCh1 = DILEPTON::kOppCh;

                        std::string observableName = "nPart";
                        std::string histName = Form("%s_etaBin%d_ptBin%d_hiBin%d",
                                observableName.c_str(), iEta, iPt, iHiBin);

                        int len = nNPartBins + 1;
                        std::vector<float> nPartAves = findNpartAverages(std::vector<int>(hiBinBins, hiBinBins + len));
                        std::vector<float> nCollAves = findNcollAverages(std::vector<int>(hiBinBins, hiBinBins + len));

                        std::vector<float> nPartBins = NpartAveragestoLowEdges(nPartAves);
                        double nPartBinsArr[nNPartBins];
                        std::copy(nPartBins.begin(), nPartBins.end(), nPartBinsArr);

                        h1D_nPart = new TH1D(Form("h1D_%s", histName.c_str()),";N_{part};dN^{Z} / N_{coll}",
                                nNPartBins, nPartBinsArr);
                        h1D_nPart->SetTitle(h1D_final[i][iCh1][iEta][iPt][iHiBin]->GetTitle());
                        h1D_nPart->SetMarkerStyle(kFullCircle);
                        h1D_nPart->SetMarkerColor(kBlack);

                        // convert histogram with hiBin on x-axis to histogram with nPart on x-axis
                        // IMPORTANT : assumes that histogram with hiBin on x-axis is properly scaled by bin width
                        bool divideBinWidth = false;
                        for (int j = 1; j <= nNPartBins; ++j) {

                            double hiBinContent = h1D_final[i][iCh1][iEta][iPt][iHiBin]->GetBinContent(nNPartBins-j+1);
                            double hiBinError   = h1D_final[i][iCh1][iEta][iPt][iHiBin]->GetBinError(nNPartBins-j+1);

                            float binWidth = 1;
                            if (divideBinWidth)
                                binWidth = h1D_final[i][iCh1][iEta][iPt][iHiBin]->GetBinLowEdge(nNPartBins-j+2)-h1D_final[i][iCh1][iEta][iPt][iHiBin]->GetBinLowEdge(nNPartBins-j+1);

                            h1D_nPart->SetBinContent(j, hiBinContent/nCollAves.at(nNPartBins-j)/binWidth);
                            h1D_nPart->SetBinError(j, hiBinError/nCollAves.at(nNPartBins-j)/binWidth);
                        }

                        c->SetName(Form("cnv_%s", histName.c_str()));
                        c->cd();
                        h1D_nPart->Draw("e");
                        h1D_nPart->Write("", TObject::kOverwrite);
                        h1D_nPart->SetStats(false);     // remove stat box from the canvas, but keep in the histograms.
                        c->Write("", TObject::kOverwrite);
                        c->Clear();

                        continue;
                    }
                }
            }
        }
    }


    configTree->Write("", TObject::kOverwrite);

    output->Write("", TObject::kOverwrite);
    output->Close();
    input->Close();
}

int main(int argc, char** argv)
{
    if (argc == 4) {
        dileptonHistogram(argv[1], argv[2], argv[3]);
        return 0;
    }
    else if (argc == 3) {
        dileptonHistogram(argv[1], argv[2]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./dileptonHistogram.exe <configFile> <inputFile> <outputFile> (<sampleName>)"
                << std::endl;
        return 1;
    }
}
