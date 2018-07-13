// Created : 2018 July 07
// Modified : 2018 July 07
// Author : Yeonju Go
// This code produces MC weight factors according to centrality and vertex and in output root files 

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TCanvas.h"
#include "/u/user/goyeonju/PhotonAnalysis2016/yjUtility.h"
#include "../TreeHeaders/hiEvtTree.h"

#include <vector>

void calcMCweights(bool isHI=true) {

    TString configFile = "/u/user/goyeonju/PhotonAnalysis2016/ElectroWeak-Jet-Track-Analyses/CutConfigurations/photonRaa_mc.conf";
    const char* inputData = "./PbPb_Data_photonRaaSkim.root";
    const char* inputMC = "/d3/scratch/goyeonju/files/photons2016/GAMMAJETFILES/2018-06-10-nominal/PbPb_MC_photonRaaSkim_Cymbal_noFlt30.root";
    TString outfname = "PbPb_MC_weights_Cymbal_noExt_noFlt.root";
    bool isHI = true; 

    int type=1;//1:Hydjet Cymbal, 2:Hydjet Drum, 3:pp 4:ZtoEE Hydjet, 5:ZtoEE pp
    if(type==1){//1:Hydjet Cymbal
        configFile = "/u/user/goyeonju/PhotonAnalysis2016/ElectroWeak-Jet-Track-Analyses/CutConfigurations/photonRaa_mc.conf";
        inputData = "./PbPb_Data_photonRaaSkim.root";
        inputMC = "/d3/scratch/goyeonju/files/photons2016/GAMMAJETFILES/2018-06-10-nominal/PbPb_MC_photonRaaSkim_Cymbal_noFlt30.root";
        outfname = "PbPb_MC_weights_Cymbal_noExt_noFlt.root";
        isHI = true;
    } else if(type==2){//2:Hydjet Drum
        configFile = "/u/user/goyeonju/PhotonAnalysis2016/ElectroWeak-Jet-Track-Analyses/CutConfigurations/photonRaa_mc_Drum_v1_and_ext1.conf";
        inputData = "/u/user/goyeonju/scratch/files/photons2016/GAMMAJETFILES/2018-06-10-nominal/PbPb_Data_photonRaaSkim.root";
        inputMC = "/d3/scratch/goyeonju/files/photons2016/GAMMAJETFILES/2018-06-10-nominal/PbPb_MC_photonRaaSkim_Drum_v1_and_ext1.root";
        outfname = "PbPb_MC_weights_Drum_v1_and_ext1.root";
        isHI = true;
    } else if(type==3){//pp



    }
    //inputMC = "/d3/scratch/goyeonju/files/photons2016/GAMMAJETFILES/2018-06-10-nominal/PbPb_EmEnrMC_photonRaaSkim_Cymbal.root";
    //outfname = "PbPb_MC_weights_EmEnr_Cymbal.root";
    inputMC = "/d3/scratch/goyeonju/files/photons2016/GAMMAJETFILES/2018-06-10-nominal/pp_EmEnrMC_photonRaaSkim.root";
    outfname = "pp_MC_weights_EmEnr.root";


    TFile* f_data = TFile::Open(inputData);
    TFile* f_mc = TFile::Open(inputMC);

    TTree* t_event = (TTree*)f_data->Get("HiEvt");
    TTree* t_event_mc = (TTree*)f_mc->Get("HiEvt");

    TH1::SetDefaultSumw2();

    TFile* weightsFile = TFile::Open(Form("%s",outfname.Data()), "RECREATE");

    TH1D* hvz_data = new TH1D("hvz_data", "", 75, -15, 15);
    TH1D* hvz_mc = new TH1D("hvz_mc", "", 75, -15, 15);
    TH1D* hcent_data = new TH1D("hcent_data", "", 200, 0, 200);
    TH1D* hcent_mc = new TH1D("hcent_mc", "", 200, 0, 200);

    TF1* f_vertex;
    TF1* f_mc_vertex;
    TF1* f_cent;
    TF1* f_mc_cent;
    TH1D* hcent;
    TH1D* hvz;
   
    t_event->Draw("vz>>hvz_data");
    t_event_mc->Draw("vz>>hvz_mc");
    hvz_data->Fit("gaus");
    f_vertex = (TF1*)hvz_data->GetFunction("gaus");
    hvz_mc->Fit("gaus");
    f_mc_vertex = (TF1*)hvz_mc->GetFunction("gaus");
    
    hvz = (TH1D*) hvz_data->Clone("hvz");
    for (int32_t ibin=1; ibin<hvz_data->GetNbinsX()+1; ++ibin) {
        double xbin_centre, fit_value, fit_value_mc;
        xbin_centre = hvz_data->GetXaxis()->GetBinCenter(ibin);
        fit_value = f_vertex->Eval(xbin_centre);
        fit_value_mc = f_mc_vertex->Eval(xbin_centre);
        hvz->SetBinContent(ibin,fit_value / fit_value_mc);
    }

    if(isHI){ 
        t_event->Draw("hiBin>>hcent_data");
        t_event_mc->Draw("hiBin>>hcent_mc");
        
        hcent_data->Fit("pol9");
        f_cent = (TF1*)hcent_data->GetFunction("pol9");
        hcent_mc->Fit("pol9", "", "", 0, 200);
        f_mc_cent = (TF1*)hcent_mc->GetFunction("pol9");
        
        hcent = (TH1D*) hcent_data->Clone("hcent");
        for (int32_t ibin=1; ibin<hcent_data->GetNbinsX()+1; ++ibin) {
            double xbin_centre, fit_value, fit_value_mc;
            xbin_centre = hcent_data->GetXaxis()->GetBinCenter(ibin);
            fit_value = f_cent->Eval(xbin_centre);
            fit_value_mc = f_mc_cent->Eval(xbin_centre);
            hcent->SetBinContent(ibin,fit_value / fit_value_mc);
        }
    }

    // h_weights->Scale(1/h_weights->GetMaximum());

    //save
    if(isHI){ 
        hcent_mc->Write();
        hcent_data->Write();
        hcent->Write();
        f_cent->Write();
        f_mc_cent->Write();
    }
    hvz_mc->Write();
    hvz_data->Write();
    hvz->Write();
    f_vertex->Write();
    f_mc_vertex->Write();

    weightsFile->Close();
}
