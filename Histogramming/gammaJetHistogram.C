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

#include "../CutConfigurations/interface/CutConfigurationsParser.h"
#include "../TreeHeaders/CutConfigurationTree.h"
#include "../Plotting/commonUtility.h"
#include "interface/correlationHist.h"
#include "../Utilities/eventUtil.h"
#include "../Utilities/interface/InputConfigurationParser.h"

const std::vector<std::string> correlationHistNames   {"xjg", "dphi", "ptJet"};
const std::vector<std::string> correlationHistFormulas{"xjg", "abs(dphi)", "jtpt"};
const std::vector<std::string> correlationHistTitleX  {"p^{Jet}_{T}/p^{#gamma}_{T}", "#Delta#phi_{J#gamma}", "p^{Jet}_{T}"};
const std::vector<std::string> correlationHistTitleY_final_normalized{"#frac{1}{N_{#gamma}}#frac{dN_{J#gamma}}{dx_{J#gamma}}",
                                                                    "#frac{1}{N_{J#gamma}}#frac{dN_{J#gamma}}{d#Delta#phi}",
                                                                    "#frac{1}{N_{#gamma}}#frac{dN_{J#gamma}}{dp^{Jet}_{T}}"};
const std::vector<int>         nBinsx{40, 20,       300};
const std::vector<double>      xlow  {0,  0,          0};
const std::vector<double>      xup   {5,  3.141592, 300};
const std::vector<double>      xlow_final{0,  0,          0};
const std::vector<double>      xup_final {2,  3.141592, 150};

void gammaJetHistogram(const TString configFile, const TString inputFile, const TString outputFile = "gammaJetHistogram.root");

void gammaJetHistogram(const TString configFile, const TString inputFile, const TString outputFile)
{
    std::cout<<"running gammaJetHistogram()"<<std::endl;
    std::cout<<"configFile  = "<< configFile.Data() <<std::endl;
    std::cout<<"inputFile   = "<< inputFile.Data() <<std::endl;
    std::cout<<"outputFile  = "<< outputFile.Data() <<std::endl;

    InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
    CutConfiguration configCuts = CutConfigurationsParser::Parse(configFile.Data());

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

    // photon cuts
    std::string str_trigger;
    float cut_phoHoverE;
    float cut_pho_ecalClusterIsoR4;
    float cut_pho_hcalRechitIsoR4;
    float cut_pho_trackIsoR4PtCut20;
    float cut_phoSigmaIEtaIEta;
    float cut_sumIso;
    float cut_sumPfIso;
    // jet cuts
    float cut_jetpt;
    float cut_jeteta;
    // gammaJet cuts
    float cut_awayRange;
    // process cuts
    int nEventsToMix;
    if (configCuts.isValid) {
        str_trigger = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_trigger_gammaJet].c_str();

        cut_phoHoverE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_phoHoverE];
        cut_pho_ecalClusterIsoR4 = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pho_ecalClusterIsoR4];
        cut_pho_hcalRechitIsoR4 = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pho_hcalRechitIsoR4];
        cut_pho_trackIsoR4PtCut20 = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pho_trackIsoR4PtCut20];
        cut_phoSigmaIEtaIEta = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_phoSigmaIEtaIEta];
        cut_sumIso = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_sumIso];
        cut_sumPfIso = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_sumPfIso];

        cut_jetpt  = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kJET].f[CUTS::JET::k_pt];
        cut_jeteta = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kJET].f[CUTS::JET::k_eta];
        cut_awayRange = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kGAMMAJET].f[CUTS::GJT::k_awayRange];

        nEventsToMix = configCuts.proc[CUTS::kSKIM].obj[CUTS::kGAMMAJET].i[CUTS::GJT::k_nEventsToMix];
    }
    else {  // default configuration for photons
        str_trigger = "HLT_HISinglePhoton20_Eta1p5_v1";

        cut_phoHoverE = 0.1;
        cut_pho_ecalClusterIsoR4 = 4.2;
        cut_pho_hcalRechitIsoR4 = 2.2;
        cut_pho_trackIsoR4PtCut20 = 2;
        cut_phoSigmaIEtaIEta = 0.01;
        cut_sumIso = 6;
        cut_sumPfIso = 6;

        cut_jetpt = 40;
        cut_jeteta = 1.6;
        cut_awayRange = 7./8.;

        nEventsToMix = 1;
    }
    // verbose about cut configuration
    std::cout<<"Cut Configuration :"<<std::endl;
    std::cout<<"trigger    = "<<str_trigger.c_str()<<std::endl;

    std::cout<<"cut_phoHoverE             = "<< cut_phoHoverE <<std::endl;
    std::cout<<"cut_pho_ecalClusterIsoR4  = "<< cut_pho_ecalClusterIsoR4 <<std::endl;
    std::cout<<"cut_pho_hcalRechitIsoR4   = "<< cut_pho_hcalRechitIsoR4 <<std::endl;
    std::cout<<"cut_pho_trackIsoR4PtCut20 = "<< cut_pho_trackIsoR4PtCut20 <<std::endl;
    std::cout<<"cut_phoSigmaIEtaIEta      = "<< cut_phoSigmaIEtaIEta <<std::endl;
    std::cout<<"cut_sumIso      = "<< cut_sumIso <<std::endl;
    std::cout<<"cut_sumPfIso    = "<< cut_sumPfIso <<std::endl;

    std::cout<<"cut_jetpt                 = "<< cut_jetpt <<std::endl;
    std::cout<<"cut_jeteta                = "<< cut_jeteta <<std::endl;
    std::cout<<"cut_awayRange             = "<< cut_awayRange << " * PI" <<std::endl;

    std::cout<<"nEventsToMix              = "<< nEventsToMix <<std::endl;

    //set real awayRange cut
    cut_awayRange = cut_awayRange * TMath::Pi();

    bool isMC = collisionIsMC((COLL::TYPE)collision);
    bool isHI = collisionIsHI((COLL::TYPE)collision);

    TFile *input = new TFile(inputFile, "READ");
    TTree *tHlt = (TTree*)input->Get("hltTree");
    TTree *tPho = (TTree*)input->Get("EventTree");    // photons
    TTree *tJet = (TTree*)input->Get("jets");
    TTree *tgj  = (TTree*)input->Get("gammaJet");
    TTree *tJetMB;
    TTree *tgjMB;
    TTree *tHiEvt;

    // check the existence of HI specific trees in "gammaJetSkim.root" file
    bool hasJetsMB = false;
    bool hasGammaJetMB = false;
    bool hasHiEvt = false;
    if (isHI) {
        input->GetObject("jetsMB",tJetMB);
        input->GetObject("gammaJetMB",tgjMB);
        input->GetObject("HiEvt",tHiEvt);

        if (tJetMB) hasJetsMB = true;
        if (tgjMB)  hasGammaJetMB = true;
        if (tHiEvt) hasHiEvt = true;
    }
    else {
        tJetMB = 0;
        tgjMB  = 0;
        tHiEvt = 0;
    }

    tgj->AddFriend(tHlt, "Hlt");
    tgj->AddFriend(tPho, "Pho");
    tgj->AddFriend(tJet, "Jet");
    if (hasHiEvt) {
        tgj->AddFriend(tHiEvt, "HiEvt");
    }

    // relation of trees from MB mixing block
    if (hasJetsMB && hasGammaJetMB) {
        tgjMB->AddFriend(tHlt, "Hlt");
        tgjMB->AddFriend(tPho, "Pho");
        tgjMB->AddFriend(tJetMB, "Jet");
        if (hasHiEvt) {
            tgjMB->AddFriend(tHiEvt, "HiEvt");
        }
    }

    TFile* output = new TFile(outputFile, "UPDATE");
    // histograms will be put under a directory whose name is the type of the collision
    if(!output->GetKey(collisionName)) output->mkdir(collisionName, Form("input file is %s", inputFile.Data()));
    output->cd(collisionName);
    std::cout<<"histograms will be put under directory : " << collisionName << std::endl;

    TTree *configTree = setupConfigurationTreeForWriting(configCuts);

    // pT bins
    const int nBins_pt = 7;
    int bins_pt_gt[nBins_pt] = {-1,     40,     60,     40, 50, 60, 80};
    int bins_pt_lt[nBins_pt] = {999999, 999999, 999999, 50, 60, 80, 999999};

    // centrality bins
    int nBins_hiBin  = 3;
    int bins_hiBin_gt[nBins_hiBin] = {0,    0,  60};
    int bins_hiBin_lt[nBins_hiBin] = {200, 60, 200};
    double purity[nBins_pt][nBins_hiBin];   // fixed for the moment.
    for (int i = 0; i<nBins_pt; ++i){
        for (int j = 0; j<nBins_hiBin; ++j){
            purity[i][j] = 0.75;
        }
    }

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

            for (int iCorr = 0; iCorr < CORR::kN_CORRFNC; ++iCorr) {
                for (int jCorr = 0; jCorr < CORR::kN_CORRFNC; ++jCorr) {

                  std::string subHistName = Form("%s_ptBin%d_HiBin%d_%s_%s", correlationHistNames.at(iHist).c_str(), i, j,
                                                 CORR::CORR_PHO_LABELS[iCorr].c_str(), CORR::CORR_JET_LABELS[jCorr].c_str());
                  corrHists[iHist][i][j].h1D_name[iCorr][jCorr] = subHistName.c_str();
                  corrHists[iHist][i][j].h1D[iCorr][jCorr] = new TH1D(Form("h1D_%s", subHistName.c_str()),"",
                                                                      nBinsx.at(iHist), xlow.at(iHist), xup.at(iHist));

                  corrHists[iHist][i][j].h1D_titleX[iCorr][jCorr] = correlationHistTitleX.at(iHist).c_str();
                  corrHists[iHist][i][j].h1D_titleY_final_norm[iCorr][jCorr] = correlationHistTitleY_final_normalized.at(iHist).c_str();
                }
            }
        }
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
                subHistName = Form("%s_ptBinAll_HiBin%d_%s_%s", correlationHistNames_ptBinAll.at(0).c_str(), j,
                                                               CORR::CORR_PHO_LABELS[iCorr].c_str(), CORR::CORR_JET_LABELS[jCorr].c_str());
                corrHists_ptBinAll[0][j].h1D_name[iCorr][jCorr] = subHistName.c_str();
                corrHists_ptBinAll[0][j].h1D[iCorr][jCorr] = new TH1D(Form("h1D_%s", subHistName.c_str()), "",nBins_rjg, bins_rjg);

                // x_jg_mean
                subHistName = Form("%s_ptBinAll_HiBin%d_%s_%s", correlationHistNames_ptBinAll.at(1).c_str(), j,
                                                                               CORR::CORR_PHO_LABELS[iCorr].c_str(), CORR::CORR_JET_LABELS[jCorr].c_str());
                corrHists_ptBinAll[1][j].h1D_name[iCorr][jCorr] = subHistName.c_str();
                corrHists_ptBinAll[1][j].h1D[iCorr][jCorr] = new TH1D(Form("h1D_%s", subHistName.c_str()), "",nBins_xjg_mean, bins_xjg_mean);
            }
        }
    }

    // selections for different signal regions that are applied to every histogram
    // selections for photon regions
    TCut selectionIso = "";
    if (isHI)
    {
        selectionIso = selectionIso && Form("(pho_ecalClusterIsoR4[phoIdx] + pho_hcalRechitIsoR4[phoIdx] + pho_trackIsoR4PtCut20[phoIdx]) < %f", cut_sumIso);
        selectionIso = selectionIso && Form("phoHoverE[phoIdx] < %f", cut_phoHoverE);
    }
    else {  // PP or PA
        // selectionIso = "pho_ecalClusterIsoR4[phoIdx] < 4.2 &&  pho_hcalRechitIsoR4[phoIdx] < 2.2 && pho_trackIsoR4PtCut20[phoIdx] < 2 ";  // 2011 style
        // use summed PF isolation
        selectionIso = selectionIso && Form("(pfcIso4[phoIdx] + pfnIso4[phoIdx]) < %f ", cut_sumPfIso);
        selectionIso = selectionIso && Form("phoHoverE[phoIdx] < %f", cut_phoHoverE);
    }
    if (isMC) {
        selectionIso = selectionIso && "1 == 1";    // gen particle specific isolation
    }
    TCut selectionPhoCORR[CORR::kN_CORRFNC];
    selectionPhoCORR[CORR::kRAW] = "phoSigmaIEtaIEta[phoIdx] < 0.01";
    selectionPhoCORR[CORR::kBKG] = "phoSigmaIEtaIEta[phoIdx] > 0.011 && phoSigmaIEtaIEta[phoIdx] < 0.017";
    // selection for jet regions
    // jet from bkg region are already separated from raw region.
    // no additional selection for jets. just use different trees.
    std::cout<<"####################"<<std::endl;
    std::cout<<"tgj->GetEntries() = "<<tgj->GetEntries()<<std::endl;
    if (str_trigger.compare("") != 0 && !isMC) {
        std::cout<<"tgj->GetEntries(trigger==1) = "<<tgj->GetEntries(Form("%s == 1",str_trigger.c_str()))<<std::endl;
    }
    else {
        std::cout<<"tgj->GetEntries(trigger==1) is skipped because either no trigger is specified or the data is coming from MC."<<std::endl;
    }
    std::cout<<"####################"<<std::endl;

    // use "EventList" approach to make the "draw()" shorter
    // store original eventlist
    const char* elist_tmp_name="elist_tmp";
    tgj->Draw(Form(">> %s", elist_tmp_name));
    TEventList* eventlist = ((TEventList*)gDirectory->Get(elist_tmp_name)->Clone("elist_Original"));
    gDirectory->Delete(elist_tmp_name);

    std::string eventlistNames[2]={"eventlist_PhoRAW", "eventlist_PhoBKG"};
    TEventList* elists[2];

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
                selection_event = selection_event && Form("hiBin >= %d && hiBin < %d", bins_hiBin_gt[j], bins_hiBin_lt[j]);
            }

            // photon cuts were applied in the analysis code
            // photon selection
            TCut selectionPho = Form("phoEt[phoIdx] >= %d && phoEt[phoIdx] < %d", bins_pt_gt[i], bins_pt_lt[i]);
            selectionPho = selectionPho && selectionIso;

            // jet selection
            TCut selectionJet = "";
            // special selection
            if (correlationHistNames.at(iHist).compare("dphi") != 0) {  // no awayRange cut for dphi histograms
                selectionJet = selectionJet && Form("abs(dphi) > %f ", cut_awayRange);
            }
            selectionJet = selectionJet && Form("insideJet == 0");
            selectionJet = selectionJet && Form("jtpt > %f", cut_jetpt);
            selectionJet = selectionJet && Form("abs(jteta) < %f", cut_jeteta);

            TCut selection_Barrel = "1";    // no extra selection at the moment
            TCut selection_Endcap = "1";    // no extra selection at the moment

            TCut selection = "";
            selection = selection && selection_event;
            selection = selection && selectionPho;
            selection = selection && selectionJet;

            for (int jCorr = 0; jCorr < CORR::kN_CORRFNC; ++jCorr) {
                    if (jCorr > CORR::kRAW && !isHI)  continue;      // no jet background for non-HI
                    corrHists[iHist][i][j].selections[CORR::kRAW][jCorr] = selection  && selectionPhoCORR[CORR::kRAW];
                    corrHists[iHist][i][j].selections[CORR::kBKG][jCorr] = selection  && selectionPhoCORR[CORR::kBKG];
            }

            std::cout<< "corrHists[iHist][i][j].h1D[CORR::kRAW][CORR::kRAW]->GetName() = " <<  corrHists[iHist][i][j].h1D[CORR::kRAW][CORR::kRAW]->GetName() <<std::endl;
            std::cout<< "corrHists[iHist][i][j].h1D[CORR::kBKG][CORR::kRAW]->GetName() = " <<  corrHists[iHist][i][j].h1D[CORR::kBKG][CORR::kRAW]->GetName() <<std::endl;
            std::cout<< "selections[CORR::kRAW][CORR::kRAW] = " << corrHists[iHist][i][j].selections[CORR::kRAW][CORR::kRAW].GetTitle() <<std::endl;
            std::cout<< "selections[CORR::kBKG][CORR::kRAW] = " << corrHists[iHist][i][j].selections[CORR::kBKG][CORR::kRAW].GetTitle() <<std::endl;

            // use "EventList" approach to make the "draw()" shorter
            tgj->Draw(Form(">> %s", eventlistNames[CORR::kRAW].c_str()), (selection_event && selectionPho && selectionPhoCORR[CORR::kRAW]).GetTitle());
            tgj->Draw(Form(">> %s", eventlistNames[CORR::kBKG].c_str()), (selection_event && selectionPho && selectionPhoCORR[CORR::kBKG]).GetTitle());
            elists[CORR::kRAW] = (TEventList*)gDirectory->Get(eventlistNames[CORR::kRAW].c_str());
            elists[CORR::kBKG] = (TEventList*)gDirectory->Get(eventlistNames[CORR::kBKG].c_str());

            // number of events with photons, not necessarily photon-jet events
            tgj->SetEventList(elists[CORR::kRAW]);
            corrHists[iHist][i][j].nEntriesPho[CORR::kRAW][CORR::kRAW] = tgj->GetEventList()->GetN();
            tgj->SetEventList(elists[CORR::kBKG]);
            corrHists[iHist][i][j].nEntriesPho[CORR::kBKG][CORR::kRAW] = tgj->GetEventList()->GetN();
            // nEntriesPho[][CORR::kRAW] = nEntriesPho[][CORR::kBKG] by definition
            // so no calculation for nEntriesPho[][CORR::kBKG]
            corrHists[iHist][i][j].nEntriesPho[CORR::kRAW][CORR::kBKG] = corrHists[iHist][i][j].nEntriesPho[CORR::kRAW][CORR::kRAW];
            corrHists[iHist][i][j].nEntriesPho[CORR::kBKG][CORR::kBKG] = corrHists[iHist][i][j].nEntriesPho[CORR::kBKG][CORR::kRAW];

            tgj->SetEventList(elists[CORR::kRAW]);
            corrHists[iHist][i][j].nEntries[CORR::kRAW][CORR::kRAW] = tgj->GetEntries(corrHists[iHist][i][j].selections[CORR::kRAW][CORR::kRAW].GetTitle());
            tgj->SetEventList(elists[CORR::kBKG]);
            corrHists[iHist][i][j].nEntries[CORR::kBKG][CORR::kRAW] = tgj->GetEntries(corrHists[iHist][i][j].selections[CORR::kBKG][CORR::kRAW].GetTitle());

            std::cout<< "nEntries[CORR::kRAW][CORR::kRAW] = " << corrHists[iHist][i][j].nEntries[CORR::kRAW][CORR::kRAW] <<std::endl;
            std::cout<< "nEntries[CORR::kBKG][CORR::kRAW] = " << corrHists[iHist][i][j].nEntries[CORR::kBKG][CORR::kRAW] <<std::endl;
            std::cout<< "nEntriesPho[CORR::kRAW][CORR::kRAW] = " << corrHists[iHist][i][j].nEntriesPho[CORR::kRAW][CORR::kRAW] <<std::endl;
            std::cout<< "nEntriesPho[CORR::kBKG][CORR::kRAW] = " << corrHists[iHist][i][j].nEntriesPho[CORR::kBKG][CORR::kRAW] <<std::endl;

            std::string histoTitle;
            if (isHI) {
              histoTitle = Form("%s , %d < p^{#gamma}_{T} < %d GeV/c, %d-%d %% ",collisionName , bins_pt_gt[i], bins_pt_lt[i], bins_hiBin_gt[j]/2, bins_hiBin_lt[j]/2);

              // special cases
              if (bins_pt_gt[i] < 0 && bins_pt_lt[i] > 777777 && bins_hiBin_gt[j] <= 0 && bins_hiBin_lt[j] >= 200 )   {
                  histoTitle = Form("%s",collisionName );
              }
              else if (bins_pt_gt[i] < 0 && bins_pt_lt[i] > 777777)   {
                  histoTitle = Form("%s , %d-%d %%",collisionName , bins_hiBin_gt[j]/2, bins_hiBin_lt[j]/2);
              }
              else if (bins_hiBin_gt[j] <= 0 && bins_hiBin_lt[j] >= 200)   {
                  histoTitle = Form("%s , %d < p^{#gamma}_{T} < %d GeV/c",collisionName , bins_pt_gt[i], bins_pt_lt[i]);
              }
              else if (bins_pt_lt[i] > 777777) {
                  histoTitle = Form("%s , p^{#gamma}_{T} > %d GeV/c, %d-%d %% ",collisionName , bins_pt_gt[i], bins_hiBin_gt[j]/2, bins_hiBin_lt[j]/2);
              }
            }
            else {
                histoTitle = Form("%s , %d < p^{#gamma}_{T} < %d GeV/c",collisionName , bins_pt_gt[i], bins_pt_lt[i]);

                // special cases
                if (bins_pt_gt[i] < 0 && bins_pt_lt[i] > 777777)   {
                    histoTitle = Form("%s",collisionName );
                }
                else if (bins_pt_lt[i] > 777777) {
                    histoTitle = Form("%s , p^{#gamma}_{T} > %d GeV/c", collisionName , bins_pt_gt[i]);
                }
            }

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
                    TCut selectionDraw = corrHists[iHist][i][j].selections[iCorr][jCorr].GetTitle();

                    std::string tmpHistName = corrHists[iHist][i][j].h1D[iCorr][jCorr]->GetName();
                    std::string tmpFormula = correlationHistFormulas.at(iHist).c_str();

                    c->SetName(Form("cnv_%s",tmpH1D_name.c_str()));
                    c->cd();
                    if (jCorr == CORR::kRAW){
                        tgj->SetEventList(elists[iCorr]);
                        tgj->Draw(Form("%s >> %s", tmpFormula.c_str(), tmpHistName.c_str()), selectionDraw.GetTitle(),"goff");
                    }
                    if (jCorr == CORR::kBKG && hasJetsMB && hasGammaJetMB) {
                        tgjMB->SetEventList(elists[iCorr]);
                        tgjMB->Draw(Form("%s >> %s", tmpFormula.c_str(), tmpHistName.c_str()), selectionDraw.GetTitle(),"goff");
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
                    if (jCorr == CORR::kBKG && hasJetsMB && hasGammaJetMB) {   // (hasJetsMB && hasGammaJetMB) ==> isHI
                        // normalize first by the number of mixed events
                        corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->Scale(1./nEventsToMix);
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
            corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kSIG]->Add(corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kBKG],-1);

            // BKGSIG = BKGRAW - BKGBKG
            std::string tmpHistNameBKGSIG = Form("%s_final_norm", corrHists[iHist][i][j].h1D[CORR::kBKG][CORR::kSIG]->GetName());
            std::cout<<tmpHistNameBKGSIG.c_str()<<std::endl;
            corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kSIG] =
                                 (TH1D*)corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kRAW]->Clone(tmpHistNameBKGSIG.c_str());
            corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kSIG]->Add(corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kBKG],-1);

            // subtract photon BKG
            // purity*SIGSIG + (1-purity)*BKGSIG = RAWSIG
            // SIGSIG = 1/purity *  ( RAWSIG - (1-purity) * BKGSIG )
            std::string tmpHistNameSIGSIG = Form("%s_final_norm", corrHists[iHist][i][j].h1D[CORR::kSIG][CORR::kSIG]->GetName());
            std::cout<<tmpHistNameSIGSIG.c_str()<<std::endl;
            corrHists[iHist][i][j].h1D_final_norm[CORR::kSIG][CORR::kSIG] =
                                 (TH1D*)corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kSIG]->Clone(tmpHistNameSIGSIG.c_str());
            corrHists[iHist][i][j].h1D_final_norm[CORR::kSIG][CORR::kSIG]->Add(corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kSIG],-1*(1-purity[i][j]));
            corrHists[iHist][i][j].h1D_final_norm[CORR::kSIG][CORR::kSIG]->Scale(1./purity[i][j]);
            if (isHI)  std::cout<< "purity[i][j] = " << purity[i][j] << std::endl;

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

            tgj->SetEventList(eventlist);      // restore the original event list after making the histograms
            if (hasJetsMB && hasGammaJetMB) {
                tgjMB->SetEventList(eventlist);
            }
            elists[CORR::kRAW]->Clear();
            elists[CORR::kBKG]->Clear();

            std::cout<<Form("histogramming END : ptBin%d HiBin%d", i, j)<<std::endl;
            std::cout<<"##########"<<std::endl;
        }
    }
    std::cout<<"histogramming END : "<<correlationHistNames.at(iHist).c_str()<<std::endl;
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

                if(j>0 && !isHI) continue;

                int offset = 3; // ptBin 40-50 starts from index 3.
                // rjg block
                for(int i=0; i<nBins_rjg; ++i){

                    double err;
                    double val = corrHists[0][i+offset][j].h1D_final_norm[iCorr][jCorr]->IntegralAndError(1,
                                                           corrHists[0][i+offset][j].h1D_final_norm[iCorr][jCorr]->GetNbinsX(), err);

                    corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->SetBinContent(i+1, val);
                    corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->SetBinError(i+1, err);
                }

                std::string histoTitle = Form("%s , %d-%d %%",collisionName , bins_hiBin_gt[j]/2, bins_hiBin_lt[j]/2);

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

    eventlist->Delete();
    elists[CORR::kRAW]->Delete();
    elists[CORR::kBKG]->Delete();

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
