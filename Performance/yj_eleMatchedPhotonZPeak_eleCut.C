#include "TTree.h"

#include "TH1.h"
#include "TH2.h"
#include "TString.h"
#include "TLorentzVector.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TLegend.h"

#include "RooFit.h"
#include "RooRealVar.h"
#include "RooBreitWigner.h"
#include "RooCBShape.h"
#include "RooVoigtian.h"
#include "RooFFTConvPdf.h"
#include "RooPlot.h"
#include "RooDataHist.h"

#include <vector>

#include "../CorrelationTuple/EventMatcher.h"
#include "../TreeHeaders/CutConfigurationTree.h"
#include "../Utilities/interface/CutConfigurationParser.h"
#include "../Utilities/interface/InputConfigurationParser.h"
#include "../Utilities/interface/HiForestInfoController.h"
#include "../../phoRaaCuts/yjUtility.h"

#define _SET_BRANCH_VEC(tree, type, branch)     \
    std::vector<type>* branch = 0;              \
    tree->SetBranchStatus(#branch, 1);          \
    tree->SetBranchAddress(#branch, &branch);   \

#define _SET_BRANCH_VAR(tree, type, branch)     \
    type branch;                                \
    tree->SetBranchStatus(#branch, 1);          \
    tree->SetBranchAddress(#branch, &branch);   \

float dR(float eta1, float eta2, float phi1, float phi2);

void fill_hists_YJCorr(TTree* t_photon, TTree* t_event, TH1D* h_mass, TH2D* h_weights,
                std::vector<int> centBins[2], std::vector<float> etaBins[2], TH1D** h_mass_cent,
                int apply_corrections, TF1** f_ecorr_ptr);
void fill_hists(TTree* t_photon, TTree* t_event, TH1D* h_mass, TH2D* h_weights,
                std::vector<int> centBins[2], std::vector<float> etaBins[2], TH1D** h_mass_cent,
                int apply_corrections, TH1D** h_ecorr_ptr);
void fill_hists_gen(TTree* t_photon, TTree* t_event, TH1D* h_mass, TH2D* h_weights,
                    std::vector<int> centBins[2], TH1D** h_mass_cent);

void yj_eleMatchedPhotonZPeak_eleCut(
                          bool isPP=false,
                          bool isYJCorr=true,
                          TString configFile = "./../CutConfigurations/photonRaa_eleMatchedPhotonZPeak.conf", 
                          const int apply_corrections = 1,
                          const char* fn_data = "/pnfs/knu.ac.kr/data/cms/store/user/ygo/photonOfficialMC2016/Z30eeJet/pbpb_data_azsigmon-HIRun2015E-PromptReco-AOD-DielectronSkim-ElePt8-v3_forest_csjet_v1_3.root",
                          const char* fn_mc = "/pnfs/knu.ac.kr/data/cms/store/user/ygo/photonOfficialMC2016/Z30eeJet/pbpb_mc_Pythia8_Z30eeJet_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13_ext1-FOREST.root",
                          const char* fn_weights = "") {
                 
    TString corrFileName = "/u/user/goyeonju/PhotonAnalysis2017/ElectroWeak-Jet-Track-Analyses/Corrections/photonEnergyCorrections.root";  
    TString cap = "pbpb";

    if(!isPP && isYJCorr){ // isYJCorr means using correction factor from fit function produced by yeonju
        //corrFileName = "/u/user/goyeonju/PhotonAnalysis2017/ElectroWeak-Jet-Track-Analyses/Corrections/yj_photonEnergyCorrections_usingPhotonSample_pbpb_DSCB.root";  
        //corrFileName = "/u/user/goyeonju/PhotonAnalysis2017/ElectroWeak-Jet-Track-Analyses/Corrections/photonEnergyCorrections_resolution_lowpt.root";  
        
        corrFileName = "/u/user/goyeonju/PhotonAnalysis2017/ElectroWeak-Jet-Track-Analyses/Corrections/photonEnergyCorrections_pbpb_DSCB_0712.root";  
        cap = "pbpb_DSCB_0712";
       // corrFileName = "/u/user/goyeonju/PhotonAnalysis2017/ElectroWeak-Jet-Track-Analyses/Corrections/photonEnergyCorrections_pbpb_gaus_0712.root";  
       // cap = "pbpb_gaus_0712";
    }

    if(isPP){
        configFile = "./../CutConfigurations/photonRaa_eleMatchedPhotonZPeak_pp.conf", 
        fn_data = "/pnfs/knu.ac.kr/data/cms/store/user/ygo/photonOfficialMC2016/Z30eeJet/pp_data_HighPtPhoton30AndZ_HIRun2015E-PromptReco-AOD-dielectron-skim-FOREST.root";
        fn_mc= "/pnfs/knu.ac.kr/data/cms/store/user/ygo/photonOfficialMC2016/Z30eeJet/pp_mc_Pythia8_Z30eeJet_HINppWinter16DR-75X_mcRun2_asymptotic_ppAt5TeV_v3_ext1-v1-FOREST.root";
        fn_weights= "";
        corrFileName = "/u/user/goyeonju/PhotonAnalysis2017/ElectroWeak-Jet-Track-Analyses/Corrections/photonEnergyCorrections_pp.root";  
        cap = "pp";
        if(isYJCorr){
            //corrFileName = "/u/user/goyeonju/PhotonAnalysis2017/ElectroWeak-Jet-Track-Analyses/Corrections/yj_photonEnergyCorrections_usingPhotonSample_pp_DSCB.root";
            //corrFileName = "/u/user/goyeonju/PhotonAnalysis2017/ElectroWeak-Jet-Track-Analyses/Corrections/photonEnergyCorrections_resolution_lowpt_pp.root";  
            
            corrFileName = "/u/user/goyeonju/PhotonAnalysis2017/ElectroWeak-Jet-Track-Analyses/Corrections/photonEnergyCorrections_pp_DSCB_0712.root";  
            cap = "pp_DSCB_0712";
           // corrFileName = "/u/user/goyeonju/PhotonAnalysis2017/ElectroWeak-Jet-Track-Analyses/Corrections/photonEnergyCorrections_pp_gaus_0712.root";  
           // cap = "pp_gaus_0712";
        }
    }


            
           // // pp gaus 
           // corrFileName = "/u/user/goyeonju/PhotonAnalysis2017/ElectroWeak-Jet-Track-Analyses/Corrections/yj_photonEnergyCorrections_lowpt_pp_gaus_0629.root";  
           // cap = "yj_photonEnergyCorrections_lowpt_pp_gaus_0629";

           // // pp DSCB
           // corrFileName = "/u/user/goyeonju/PhotonAnalysis2017/ElectroWeak-Jet-Track-Analyses/Corrections/yj_photonEnergyCorrections_lowpt_pp_DSCB_setSigParLimit.root";  
           // cap = "yj_photonEnergyCorrections_lowpt_pp_DSCB_setSigParLimit";



           // // PbPb gaus 
           // corrFileName = "/u/user/goyeonju/PhotonAnalysis2017/ElectroWeak-Jet-Track-Analyses/Corrections/yj_photonEnergyCorrections_lowpt_pbpb_gaus_0629.root";  
           // cap = "yj_photonEnergyCorrections_lowpt_pbpb_gaus_0629";
           // 
           // // PbPb DSCB
           // corrFileName = "/u/user/goyeonju/PhotonAnalysis2017/ElectroWeak-Jet-Track-Analyses/Corrections/yj_photonEnergyCorrections_lowpt_pbpb_DSCB_setSigParLimit.root";  
           // cap = "yj_photonEnergyCorrections_lowpt_pbpb_DSCB_setSigParLimit";




    TFile* f_data = new TFile(fn_data, "read");
    TTree* t_photon = (TTree*)f_data->Get("ggHiNtuplizer/EventTree");
    TTree* t_event = (TTree*)f_data->Get("hiEvtAnalyzer/HiTree");

    InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

   // TFile* f_weights = new TFile(fn_weights, "read");
   // TH2D* h_weights = 0;
   // if (f_weights && f_weights->IsOpen())
   //     (TH2D*)f_weights->Get("h_weights");
    cout << "sss" << endl;
    std::vector<int> centBins[2];
    centBins[0] = ConfigurationParser::ParseListInteger(configCuts.proc[CUTS::kCORRECTION].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_gt]);
    centBins[1] = ConfigurationParser::ParseListInteger(configCuts.proc[CUTS::kCORRECTION].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_lt]);
    int nCentBins = centBins[0].size();
    cout << "nCentBins = " << nCentBins << endl;
    std::vector<float> etaBins[2];
    etaBins[0] = ConfigurationParser::ParseListFloat(configCuts.proc[CUTS::kCORRECTION].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_eta_gt]);
    etaBins[1] = ConfigurationParser::ParseListFloat(configCuts.proc[CUTS::kCORRECTION].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_eta_lt]);
    int nEtaBins = etaBins[0].size();
    
    if(isPP){
        nCentBins = 1; 
        cout << "PP collisions # of centrality bin = " << nCentBins << endl;
    }
    cap += Form("_nCentBins%d",nCentBins);
    //if(isYJCorr && !isPP) nCentBins = 3; 

    TFile* energyCorrectionFile = TFile::Open(Form("%s",corrFileName.Data()), "read");
    TH1D* h_ecorr[nCentBins][nEtaBins];
    TF1* f_ecorr[nCentBins][nEtaBins];
    for (int i=0; i<nCentBins; ++i){
        for (int j=0; j<nEtaBins; ++j){
            if(isPP) h_ecorr[i][j] = (TH1D*)energyCorrectionFile->Get(Form("photonEnergyCorr_eta%i", j));
            else h_ecorr[i][j] = (TH1D*)energyCorrectionFile->Get(Form("photonEnergyCorr_cent%i_eta%i", i, j));
    }}
            
    if(isYJCorr){
        for (int i=0; i<nCentBins; ++i){
            for (int j=0; j<nEtaBins; ++j){
                if(isPP) f_ecorr[i][j] = (TF1*)energyCorrectionFile->Get(Form("f_mean_gaus_cent0_eta%i", j));
                else f_ecorr[i][j] = (TF1*)energyCorrectionFile->Get(Form("f_mean_gaus_cent%i_eta%i",i, j));
            }
        }
    }

    gStyle->SetOptStat(0);

    TH1::SetDefaultSumw2();

    TH1D* h_mass = new TH1D("h_mass", "", 30, 60, 120);

    TH1D* h_mass_cent[nCentBins];
    for (int i=0; i<nCentBins; ++i)
        h_mass_cent[i] = new TH1D(Form("h_mass_cent_%i", i), Form("%i-%i%% Centrality;M_{Inv};", centBins[0][i]/2, centBins[1][i]/2), 30, 60, 120);

    if(isYJCorr) fill_hists_YJCorr(t_photon, t_event, h_mass, 0, centBins, etaBins, h_mass_cent, apply_corrections, (TF1**)f_ecorr);
    else        fill_hists(t_photon, t_event, h_mass, 0, centBins, etaBins, h_mass_cent, apply_corrections, (TH1D**)h_ecorr);

    TFile* f_mc = new TFile(fn_mc, "read");
    t_photon = (TTree*)f_mc->Get("ggHiNtuplizer/EventTree");
    t_event = (TTree*)f_mc->Get("hiEvtAnalyzer/HiTree");

    TH1D* h_mass_mc = new TH1D("h_mass_mc", "", 30, 60, 120);

    TH1D* h_mass_cent_mc[nCentBins];
    for (int i=0; i<nCentBins; ++i)
        h_mass_cent_mc[i] = new TH1D(Form("h_mass_cent_mc_%i", i), Form("%i-%i%% Centrality;M_{Inv};", centBins[0][i]/2, centBins[1][i]/2), 30, 60, 120);

    if(isYJCorr) fill_hists_YJCorr(t_photon, t_event, h_mass_mc, 0, centBins, etaBins, h_mass_cent_mc, apply_corrections, (TF1**)f_ecorr);
    else         fill_hists(t_photon, t_event, h_mass_mc, 0, centBins, etaBins, h_mass_cent_mc, apply_corrections, (TH1D**)h_ecorr);
    //fill_hists(t_photon, t_event, h_mass_mc, h_weights, centBins, etaBins, h_mass_cent_mc, apply_corrections, (TH1D**)h_ecorr);

    TH1D* h_mass_gen = new TH1D("h_mass_gen", "", 30, 60, 120);

    TH1D* h_mass_cent_gen[nCentBins];
    for (int i=0; i<nCentBins; ++i)
        h_mass_cent_gen[i] = new TH1D(Form("h_mass_cent_gen_%i", i), Form("%i-%i%% Centrality;M_{Inv};", centBins[0][i]/2, centBins[1][i]/2), 30, 60, 120);

    fill_hists_gen(t_photon, t_event, h_mass_gen, 0, centBins, h_mass_cent_gen);
    //fill_hists_gen(t_photon, t_event, h_mass_gen, h_weights, centBins, h_mass_cent_gen);

    RooRealVar x("x", "x", 60, 120);
    cout << "qqqqqqqqqqqsssssssaaaaaaaaaaaaaaaasss" << endl;

    RooRealVar m0("m0", "m0", 90, 60, 120);
    RooRealVar width("width", "width", 10, 0, 40);
    RooBreitWigner bw("bw", "bw", x, m0, width);

    RooRealVar mean("mean", "mean", 0);
    RooRealVar sigma("sigma", "sigma", 10, 0, 40);
    RooRealVar alpha("alpha", "alpha", 10, 0, 40);
    RooRealVar n("n", "n", 10, 0, 40);
    RooCBShape cb("cb", "cb", x, mean, sigma, alpha, n);

    RooFFTConvPdf pdf("pdf", "pdf", x, bw, cb);

    RooRealVar vmean("vmean", "vmean", 90, 60, 120);
    RooRealVar vwidth("vwidth", "vwidth", 10, 0, 40);
    RooRealVar vsigma("vsigma", "vsigma", 10, 0, 40);
    RooVoigtian v("v", "v", x, vmean, vwidth, vsigma);

    double hist_centBins[nCentBins + 1];
    for (int i=0; i<nCentBins; ++i)
        hist_centBins[i] = centBins[0][i];
    hist_centBins[nCentBins] = centBins[1][nCentBins - 1];

    TH1D* h_z_mass = new TH1D("h_z_mass", "Z Mass;Centrality;M_{Inv}", nCentBins, hist_centBins);
    TH1D* h_z_mass_mc = new TH1D("h_z_mass_mc", "Z Mass;Centrality;M_{Inv}", nCentBins, hist_centBins);
    TH1D* h_z_mass_gen = new TH1D("h_z_mass_gen", "Z Mass;Centrality;M_{Inv}", nCentBins, hist_centBins);

    for (int i=0; i<nCentBins; ++i) {
        cout << "Centrality : "<< centBins[0][i] << " - " << centBins[1][i] << endl;
        RooDataHist data_cent("data_cent", "dataset", x, h_mass_cent[i]);
        bw.fitTo(data_cent);
        h_z_mass->SetBinContent(i+1, m0.getVal());
        h_z_mass->SetBinError(i+1, m0.getError());
        cout << "Invariant Mass(DATA RECO): \t" << h_z_mass->GetBinContent(i+1) << endl;

        RooDataHist data_cent_mc("data_cent_mc", "dataset", x, h_mass_cent_mc[i]);
        bw.fitTo(data_cent_mc);
        h_z_mass_mc->SetBinContent(i+1, m0.getVal());
        h_z_mass_mc->SetBinError(i+1, m0.getError());
        cout << "Invariant Mass(MC RECO): \t" << h_z_mass_mc->GetBinContent(i+1) << endl;

        RooDataHist data_cent_gen("data_cent_gen", "dataset", x, h_mass_cent_gen[i]);
        bw.fitTo(data_cent_gen);
        h_z_mass_gen->SetBinContent(i+1, m0.getVal());
        h_z_mass_gen->SetBinError(i+1, m0.getError());
        cout << "Invariant Mass(MC GEN): \t" << h_z_mass_gen->GetBinContent(i+1) << endl;

    }

    TCanvas* c1 = new TCanvas("c1", "", 400, 400);

    h_z_mass_gen->SetMarkerStyle(21);
    h_z_mass_gen->SetMarkerSize(0.8);
    h_z_mass_gen->SetMarkerColor(4);
    h_z_mass_gen->SetLineColor(4);
    h_z_mass_gen->SetAxisRange(80, 110, "Y");
    
    h_z_mass_mc->SetMarkerStyle(20);
    h_z_mass_mc->SetMarkerSize(0.8);
    h_z_mass_mc->SetMarkerColor(2);
    h_z_mass_mc->SetLineColor(2);
    h_z_mass_mc->SetAxisRange(80, 110, "Y");

    h_z_mass->SetMarkerStyle(22);
    h_z_mass->SetMarkerSize(0.8);
    h_z_mass->SetMarkerColor(1);
    h_z_mass->SetLineColor(1);
    h_z_mass->SetAxisRange(80, 110, "Y");

    h_z_mass_gen->Draw();
    h_z_mass_mc->Draw("same");
    h_z_mass->Draw("same");

    TLegend* l1 = new TLegend(0.6, 0.64, 0.8, 0.80);
    l1->SetBorderSize(0);
    l1->SetTextSize(0.04);
    l1->AddEntry(h_z_mass, "Data");
    l1->AddEntry(h_z_mass_mc, "MC Reco");
    l1->AddEntry(h_z_mass_gen, "MC Gen");
    l1->Draw();

    drawText(Form("%s",cap.Data()),0.2,0.8);
    int nrows = (nCentBins - 1) / 5 + 1;
    TCanvas* c2 = new TCanvas("c2", "", 300*nCentBins, 300);
    //TCanvas* c2 = new TCanvas("c2", "", 1500, 300 * nrows);
    c2->Divide(nCentBins,1);
    //c2->Divide(nCentBins, nrows);
    for (int i=0; i<nCentBins; ++i) {
        c2->cd(i+1);
        if(isPP) h_mass_cent_gen[i]->SetTitle("pp (|#eta|<1.44) ;M_{inv};");
        h_mass_cent_gen[i]->Scale(1/h_mass_cent_gen[i]->Integral());
        h_mass_cent_gen[i]->SetMarkerSize(0);
        h_mass_cent_gen[i]->SetLineColor(4);
        h_mass_cent_gen[i]->Draw("hist e");
        h_mass_cent_mc[i]->Scale(1/h_mass_cent_mc[i]->Integral());
        h_mass_cent_mc[i]->SetMarkerSize(0);
        h_mass_cent_mc[i]->SetLineColor(2);
        h_mass_cent_mc[i]->Draw("same hist e");
        h_mass_cent[i]->Scale(1/h_mass_cent[i]->Integral());
        h_mass_cent[i]->SetMarkerStyle(20);
        h_mass_cent[i]->SetMarkerSize(0.7);
        h_mass_cent[i]->SetMarkerColor(1);
        h_mass_cent[i]->SetLineColor(1);
        h_mass_cent[i]->Draw("same p e");
        drawText(Form("%s",cap.Data()),0.2,0.8);
        if(i==0) l1->Draw("same");
    }
    c1->SaveAs(Form("figures/eleMatchedPhotonZPeak_Zmass_summary_noWeight_%s.pdf",cap.Data()));
    c2->SaveAs(Form("figures/eleMatchedPhotonZPeak_Zmass_distributions_noWeight_%s.pdf",cap.Data()));
    TFile* f_out = new TFile("eleMatchedPhotonZPeak_noWeight_pp.root", "recreate");
    h_z_mass->Write("h_z_mass", TObject::kOverwrite);
    h_z_mass_mc->Write("h_z_mass_mc", TObject::kOverwrite);
    h_z_mass_gen->Write("h_z_mass_gen", TObject::kOverwrite);
    for (int i=0; i<nCentBins; ++i) {
        h_mass_cent_gen[i]->Write("", TObject::kOverwrite);
        h_mass_cent_mc[i]->Write("", TObject::kOverwrite);
        h_mass_cent[i]->Write("", TObject::kOverwrite);
    }
    c1->Write("", TObject::kOverwrite);
    c2->Write("", TObject::kOverwrite);
    f_out->Close();

    printf("\n%s\n", "root -l eleMatchedPhotonZPeak.root");

    //return 0;
}

void fill_hists_YJCorr(TTree* t_photon, TTree* t_event, TH1D* h_mass, TH2D* h_weights,
                std::vector<int> centBins[2], std::vector<float> etaBins[2], TH1D** h_mass_cent,
                int apply_corrections, TF1** f_ecorr_ptr) {
    int nCentBins = centBins[1].size();
    int nEtaBins = etaBins[1].size();

    TF1* f_ecorr[5][nEtaBins];
    memcpy(f_ecorr, f_ecorr_ptr, 5 * nEtaBins * sizeof(TF1*));
    //TF1* f_ecorr[nCentBins][nEtaBins];
    //memcpy(f_ecorr, f_ecorr_ptr, nCentBins * nEtaBins * sizeof(TF1*));

    t_photon->SetBranchStatus("*", 0);
    _SET_BRANCH_VAR(t_photon, int, nPho)
    _SET_BRANCH_VEC(t_photon, float, phoEt);
    _SET_BRANCH_VEC(t_photon, float, phoEta);
    _SET_BRANCH_VEC(t_photon, float, phoPhi);
    _SET_BRANCH_VEC(t_photon, float, phoHoverE);
    _SET_BRANCH_VEC(t_photon, float, pho_ecalClusterIsoR4);
    _SET_BRANCH_VEC(t_photon, float, pho_hcalRechitIsoR4);
    _SET_BRANCH_VEC(t_photon, float, pho_trackIsoR4PtCut20);
    _SET_BRANCH_VEC(t_photon, float, phoSigmaIEtaIEta_2012);
   // _SET_BRANCH_VEC(t_photon, int, pho_genMatchedIndex);
    _SET_BRANCH_VEC(t_photon, float, pho_swissCrx);
    _SET_BRANCH_VEC(t_photon, float, pho_seedTime);

   // _SET_BRANCH_VAR(t_photon, int, nMC);
   // _SET_BRANCH_VEC(t_photon, int, mcPID);
   // _SET_BRANCH_VEC(t_photon, int, mcStatus);
   // _SET_BRANCH_VEC(t_photon, float, mcPt);
   // _SET_BRANCH_VEC(t_photon, float, mcEta);
   // _SET_BRANCH_VEC(t_photon, float, mcPhi);

    _SET_BRANCH_VAR(t_photon, int, nEle);
    _SET_BRANCH_VEC(t_photon, float, elePt);
    _SET_BRANCH_VEC(t_photon, float, eleEta);
    _SET_BRANCH_VEC(t_photon, float, elePhi);
    _SET_BRANCH_VEC(t_photon, float, eleEoverP);
    _SET_BRANCH_VEC(t_photon, float, eleSigmaIEtaIEta_2012);
    _SET_BRANCH_VEC(t_photon, float, eledEtaAtVtx);
    _SET_BRANCH_VEC(t_photon, float, eledPhiAtVtx);
    _SET_BRANCH_VEC(t_photon, float, eleHoverE);
    _SET_BRANCH_VEC(t_photon, float, eleEoverPInv);
    _SET_BRANCH_VEC(t_photon, float, eleD0);
    _SET_BRANCH_VEC(t_photon, float, eleDz);
    _SET_BRANCH_VEC(t_photon, float, eleMissHits);

    t_event->SetBranchStatus("*", 0);
    _SET_BRANCH_VAR(t_event, int, hiBin);
    _SET_BRANCH_VAR(t_event, float, vz);

    int32_t nentries = t_photon->GetEntries();
    for (int32_t i=0; i<nentries; ++i) {
        if (i % 5000 == 0)
            printf("entry: %i\n", i);

        t_photon->GetEntry(i);
        t_event->GetEntry(i);

        for (int32_t j=0; j<nPho; ++j) {
            if (phoEt->at(j) < 20)
                break;
            if (fabs(phoEta->at(j)) > 1.44)
                continue;
            if (phoSigmaIEtaIEta_2012->at(j) > 0.01)
                continue;
            if (phoHoverE->at(j) > 0.1)
                continue;
            if (pho_swissCrx->at(j) > 0.9)
                continue;
            if (fabs(pho_seedTime->at(j)) > 3)
                continue;

/////////////////////////////
   int matchedEleIndex = -1;
            float matchedEoverP = 100;
            //for (uint32_t k=0; k<nEle; ++k) {
            for (uint32_t k=0; k<elePt->size(); ++k) {
                if (elePt->at(k) < 10)
                    continue;
                if (eleSigmaIEtaIEta_2012->at(k) > 0.01107)
                    continue;
                if (eleEoverPInv->at(k) > 0.28051)
                    continue;
                if (fabs(eledEtaAtVtx->at(k)) > 0.01576)
                    continue;
                if (fabs(eledPhiAtVtx->at(k)) > 0.15724)
                    continue;
                if (eleHoverE->at(k) > 0.08849)
                    continue;
                if (fabs(eleD0->at(k)) > 0.05216)
                    continue;
                if (fabs(eleDz->at(k)) > 0.12997)
                    continue;
                if (eleMissHits->at(k) > 1)
                    continue;
                if (dR(phoEta->at(j), eleEta->at(k), phoPhi->at(j), elePhi->at(k)) > 0.04242)
                    continue;
                if (eleEoverP->at(k) > matchedEoverP)
                    continue;
                matchedEleIndex = k;
            }
            //cout << "mactchedIndex = " <<matchedEleIndex << endl;
            if (matchedEleIndex == -1)
                continue;

           // int32_t mcindex=0;
           // if(isMC){ 
           //     mcindex = pho_genMatchedIndex->at(j);
           //     if(mcindex
           //     cout << "mc index 1 = " << mcPID->at(mcindex)<< endl;
           //     //if(abs(mcPID->at(mcindex))!=11) continue;
           // }
            for (int32_t k=j+1; k<nPho; ++k) {
                if (phoEt->at(k) < 20)
                    break;
                if (fabs(phoEta->at(k)) > 1.44)
                    continue;
                if (phoSigmaIEtaIEta_2012->at(k) > 0.01)
                    continue;
                if (phoHoverE->at(k) > 0.1)
                    continue;
                if (pho_swissCrx->at(k) > 0.9)
                    continue;
                if (fabs(pho_seedTime->at(k)) > 3)
                    continue;
                /////////////////////////////
                int matchedEleIndex2 = -1;
                float matchedEoverP2 = 100;
                //for (uint32_t e=0; e<nEle; ++e) {
                for (uint32_t e=0; e<elePt->size(); ++e) {
                    if (elePt->at(e) < 10)
                        continue;
                if (eleSigmaIEtaIEta_2012->at(e) > 0.01107)
                    continue;
                if (eleEoverPInv->at(e) > 0.28051)
                    continue;
                if (fabs(eledEtaAtVtx->at(e)) > 0.01576)
                    continue;
                if (fabs(eledPhiAtVtx->at(e)) > 0.15724)
                    continue;
                if (eleHoverE->at(e) > 0.08849)
                    continue;
                if (fabs(eleD0->at(e)) > 0.05216)
                    continue;
                if (fabs(eleDz->at(e)) > 0.12997)
                    continue;
                if (eleMissHits->at(e) > 1)
                    continue;
                if (dR(phoEta->at(k), eleEta->at(e), phoPhi->at(k), elePhi->at(e)) > 0.04242)
                    continue;
                if (eleEoverP->at(e) > matchedEoverP2)
                    continue;
                matchedEleIndex2 = e;
            }
            //cout << "mactchedIndex2 = " <<matchedEleIndex2 << endl;
            if (matchedEleIndex2 == -1)
                continue;
               // int32_t mcindex2=0;
               // if(isMC){ 
               //     mcindex2 = pho_genMatchedIndex->at(k);
               //     cout << "mc index 2 = " << mcindex2 << endl;
               //     //if(abs(mcPID->at(mcindex2))!=11) continue;
               // }

                int icent = 0;
                for (; hiBin>=centBins[1][icent] && icent<nCentBins; ++icent);

                int ieta1 = 0;
                for (; fabs(phoEta->at(j))>=etaBins[1][ieta1] && ieta1<nEtaBins; ++ieta1);
                int ieta2 = 0;
                for (; fabs(phoEta->at(k))>=etaBins[1][ieta2] && ieta2<nEtaBins; ++ieta2);

                float phoEt1 = phoEt->at(j);
                float phoEt2 = phoEt->at(k);
                if (apply_corrections) {
                    phoEt1 /= f_ecorr[icent][ieta1]->Eval(phoEt1);
                    phoEt2 /= f_ecorr[icent][ieta2]->Eval(phoEt2);
                }

                TLorentzVector pho1, pho2;
                pho1.SetPtEtaPhiM(phoEt1, phoEta->at(j), phoPhi->at(j), 0);
                pho2.SetPtEtaPhiM(phoEt2, phoEta->at(k), phoPhi->at(k), 0);
                TLorentzVector z = pho1 + pho2;

                double weight;
                if (h_weights)
                    weight = h_weights->GetBinContent(h_weights->FindBin(hiBin, vz));
                else
                    weight = 1;

                if (z.M() != 0) {
                    h_mass->Fill(z.M(), weight);
                    h_mass_cent[icent]->Fill(z.M(), weight);
                }
            }
        }
    }
}

void fill_hists(TTree* t_photon, TTree* t_event, TH1D* h_mass, TH2D* h_weights,
                std::vector<int> centBins[2], std::vector<float> etaBins[2], TH1D** h_mass_cent,
                int apply_corrections, TH1D** h_ecorr_ptr) {
    int nCentBins = centBins[1].size();
    int nEtaBins = etaBins[1].size();

    TH1D* h_ecorr[nCentBins][nEtaBins];
    memcpy(h_ecorr, h_ecorr_ptr, nCentBins * nEtaBins * sizeof(TH1D*));

    t_photon->SetBranchStatus("*", 0);
    _SET_BRANCH_VAR(t_photon, int, nPho)
    _SET_BRANCH_VEC(t_photon, float, phoEt);
    _SET_BRANCH_VEC(t_photon, float, phoEta);
    _SET_BRANCH_VEC(t_photon, float, phoPhi);
    _SET_BRANCH_VEC(t_photon, float, phoHoverE);
    _SET_BRANCH_VEC(t_photon, float, pho_ecalClusterIsoR4);
    _SET_BRANCH_VEC(t_photon, float, pho_hcalRechitIsoR4);
    _SET_BRANCH_VEC(t_photon, float, pho_trackIsoR4PtCut20);
    _SET_BRANCH_VEC(t_photon, float, phoSigmaIEtaIEta_2012);
    _SET_BRANCH_VEC(t_photon, float, pho_swissCrx);
    _SET_BRANCH_VEC(t_photon, float, pho_seedTime);

    t_event->SetBranchStatus("*", 0);
    _SET_BRANCH_VAR(t_event, int, hiBin);
    _SET_BRANCH_VAR(t_event, float, vz);

    int32_t nentries = t_photon->GetEntries();
    for (int32_t i=0; i<nentries; ++i) {
        if (i % 5000 == 0)
            printf("entry: %i\n", i);

        t_photon->GetEntry(i);
        t_event->GetEntry(i);

        for (int32_t j=0; j<nPho; ++j) {
            if (phoEt->at(j) < 20)
                break;
            if (fabs(phoEta->at(j)) > 1.44)
                continue;
            if (phoSigmaIEtaIEta_2012->at(j) > 0.01)
                continue;
            if (phoHoverE->at(j) > 0.1)
                continue;
            if (pho_swissCrx->at(j) > 0.9)
                continue;
            if (fabs(pho_seedTime->at(j)) > 3)
                continue;

            for (int32_t k=j+1; k<nPho; ++k) {
                if (phoEt->at(k) < 20)
                    break;
                if (fabs(phoEta->at(k)) > 1.44)
                    continue;
                if (phoSigmaIEtaIEta_2012->at(k) > 0.01)
                    continue;
                if (phoHoverE->at(k) > 0.1)
                    continue;
                if (pho_swissCrx->at(k) > 0.9)
                    continue;
                if (fabs(pho_seedTime->at(k)) > 3)
                    continue;

                int icent = 0;
                for (; hiBin>=centBins[1][icent] && icent<nCentBins; ++icent);

                int ieta1 = 0;
                for (; fabs(phoEta->at(j))>=etaBins[1][ieta1] && ieta1<nEtaBins; ++ieta1);
                int ieta2 = 0;
                for (; fabs(phoEta->at(k))>=etaBins[1][ieta2] && ieta2<nEtaBins; ++ieta2);

                float phoEt1 = phoEt->at(j);
                float phoEt2 = phoEt->at(k);
                if (apply_corrections) {
                    phoEt1 /= h_ecorr[icent][ieta1]->GetBinContent(h_ecorr[icent][ieta1]->FindBin(phoEt1));
                    phoEt2 /= h_ecorr[icent][ieta2]->GetBinContent(h_ecorr[icent][ieta2]->FindBin(phoEt2));
                }

                TLorentzVector pho1, pho2;
                pho1.SetPtEtaPhiM(phoEt1, phoEta->at(j), phoPhi->at(j), 0);
                pho2.SetPtEtaPhiM(phoEt2, phoEta->at(k), phoPhi->at(k), 0);
                TLorentzVector z = pho1 + pho2;

                double weight;
                if (h_weights)
                    weight = h_weights->GetBinContent(h_weights->FindBin(hiBin, vz));
                else
                    weight = 1;

                if (z.M() != 0) {
                    h_mass->Fill(z.M(), weight);
                    h_mass_cent[icent]->Fill(z.M(), weight);
                }
            }
        }
    }
}

void fill_hists_gen(TTree* t_photon, TTree* t_event, TH1D* h_mass, TH2D* h_weights,
                    std::vector<int> centBins[2], TH1D** h_mass_cent) {
    t_photon->SetBranchStatus("*", 0);
    _SET_BRANCH_VAR(t_photon, int, nMC);
    _SET_BRANCH_VEC(t_photon, int, mcPID);
    _SET_BRANCH_VEC(t_photon, int, mcStatus);
    _SET_BRANCH_VEC(t_photon, float, mcPt);
    _SET_BRANCH_VEC(t_photon, float, mcEta);
    _SET_BRANCH_VEC(t_photon, float, mcPhi);

    t_event->SetBranchStatus("*", 0);
    _SET_BRANCH_VAR(t_event, int, hiBin);
    _SET_BRANCH_VAR(t_event, float, vz);

    const double electronMass = 0.000510998;

    int32_t nentries = t_photon->GetEntries();
    for (int32_t i=0; i<nentries; ++i) {
        if (i % 5000 == 0)
            printf("entry: %i\n", i);

        t_photon->GetEntry(i);
        t_event->GetEntry(i);

        for (int32_t j=0; j<nMC; ++j) {
            if (abs(mcPID->at(j)) != 11)
                continue;
            if (mcStatus->at(j) != 1)
                continue;
            if (mcPt->at(j) < 20)
                continue;
            if (fabs(mcEta->at(j)) > 1.44)
                continue;

            for (int32_t k=j+1; k<nMC; ++k) {
                if (abs(mcPID->at(k)) != 11)
                    continue;
                if (mcStatus->at(k) != 1)
                    continue;
                if (mcPt->at(k) < 20)
                    continue;
                if (fabs(mcEta->at(k)) > 1.44)
                    continue;

                if (mcPID->at(j) == mcPID->at(k))
                    continue;

                int icent = 0;
                int nCentBins = centBins[1].size();
                for (; hiBin>=centBins[1][icent] && icent<nCentBins; ++icent);

                TLorentzVector ele1, ele2;
                ele1.SetPtEtaPhiM(mcPt->at(j), mcEta->at(j), mcPhi->at(j), electronMass);
                ele2.SetPtEtaPhiM(mcPt->at(k), mcEta->at(k), mcPhi->at(k), electronMass);
                TLorentzVector z = ele1 + ele2;

                double weight;
                if (h_weights)
                    weight = h_weights->GetBinContent(h_weights->FindBin(hiBin, vz));
                else
                    weight = 1;

                if (z.M() != 0) {
                    h_mass->Fill(z.M(), weight);
                    h_mass_cent[icent]->Fill(z.M(), weight);
                }
            }
        }
    }

//int main(int argc, char* argv[]) {
//    if (argc == 1)
//        return eleMatchedPhotonZPeak_yj();
//    else if (argc == 3)
//        return eleMatchedPhotonZPeak_yj(argv[1], atoi(argv[2]));
//    else if (argc == 6)
//        return eleMatchedPhotonZPeak_yj(argv[1], atoi(argv[2]), argv[3], argv[4], argv[5]);
//
//    return 1;
//}
}
#define _PI 3.1415926535897932384
float dR(float eta1, float eta2, float phi1, float phi2) {
    float deta = eta1 - eta2;
    float dphi = fabs(phi1 - phi2);
    dphi = (dphi > _PI) ? 2 * _PI - dphi : dphi;
    return sqrt(deta * deta + dphi * dphi);
}
