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
#include "../TreeHeaders/hiEvtTree.h"
#include "../Utilities/interface/InputConfigurationParser.h"
#include "../../yjUtility.h"

#include <vector>

void calcMCweights(bool isHI=true, TString sample ="AllQCD") {
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(0);
 
    cout << "Is it heavy ion collisions? " << isHI << endl;
    // DATA tree 
    std::string input_data = "/u/user/goyeonju/ElectroWeak-Jet-Track-Analyses/ShellScripts/photonRaa/filelist/PbPb_Data_HiHardProbes_run262620to263035.list";
    if(!isHI && sample!="ZtoEE") input_data = "/u/user/goyeonju/ElectroWeak-Jet-Track-Analyses/ShellScripts/photonRaa/filelist/pp_Data_HighPtLowerPhotons.list"; 
    else if(isHI && sample!="ZtoEE") input_data = "/u/user/goyeonju/ElectroWeak-Jet-Track-Analyses/ShellScripts/photonRaa/filelist/PbPb_Data_HiHardProbes_run262620to263035.list";
    else if(isHI && sample=="ZtoEE") input_data = "/u/user/goyeonju/ElectroWeak-Jet-Track-Analyses/ShellScripts/photonRaa/filelist/PbPb_Data_dielectron.list";
    else if(!isHI && sample=="ZtoEE") input_data = "/u/user/goyeonju/ElectroWeak-Jet-Track-Analyses/ShellScripts/photonRaa/filelist/pp_Data_dielectron.list";
    else cout << " ERROR : wrong sample type! " << endl;
    std::vector<std::string> input_list_data = InputConfigurationParser::ParseFiles(input_data);
    printf("number of input files: %zu\n", input_list_data.size());

    TChain* t_event = new TChain("hiEvtAnalyzer/HiTree");
    TChain* t_skim= new TChain("skimanalysis/HltTree");
    for (std::vector<std::string>::iterator it = input_list_data.begin(); it != input_list_data.end(); ++it){
        t_event->Add((*it).c_str());
        t_skim->Add((*it).c_str());
    }
    t_event->AddFriend(t_skim);

    // MC tree 
    std::string input_mc = "";
    if(sample=="ZtoEE"){
        if(!isHI) input_mc = "/u/user/goyeonju/ElectroWeak-Jet-Track-Analyses/ShellScripts/photonRaa/filelist/pp_MC_Z30eeJet.list";
        else input_mc = "/u/user/goyeonju/ElectroWeak-Jet-Track-Analyses/ShellScripts/photonRaa/filelist/PbPb_MC_Z30eeJet.list";

    } else{
        if(isHI && sample=="AllQCD") input_mc = "/u/user/goyeonju/ElectroWeak-Jet-Track-Analyses/ShellScripts/photonRaa/filelist/PbPb_MC_Cymbal.list"; 
        else if(isHI && sample=="EmEnr") input_mc= "/u/user/goyeonju/ElectroWeak-Jet-Track-Analyses/ShellScripts/photonRaa/filelist/PbPb_EmEnrMC_Cymbal.list";
        else if(!isHI && sample=="AllQCD") input_mc= "/u/user/goyeonju/ElectroWeak-Jet-Track-Analyses/ShellScripts/photonRaa/filelist/pp_MC_v1.list"; 
        else if(!isHI && sample=="EmEnr") input_mc= "/u/user/goyeonju/ElectroWeak-Jet-Track-Analyses/ShellScripts/photonRaa/filelist/pp_EmEnrMC_v1.list"; 
        else cout << "[ERROR] : wrong MC input "<< endl;
    }

    std::vector<std::string> input_list_mc = InputConfigurationParser::ParseFiles(input_mc);
    printf("number of input files: %zu\n", input_list_mc.size());

    TChain* t_event_mc = new TChain("hiEvtAnalyzer/HiTree");
    TChain* t_skim_mc= new TChain("skimanalysis/HltTree");
    for (std::vector<std::string>::iterator it = input_list_mc.begin(); it != input_list_mc.end(); ++it){
        t_event_mc->Add((*it).c_str());
        t_skim_mc->Add((*it).c_str());
    }
    t_event_mc->AddFriend(t_skim_mc);


    TH1::SetDefaultSumw2();

    TString cap = "pp";
    if(isHI) cap = "pbpb";
    cap += Form("_%s",sample.Data());
    TCut skimCut = "pBeamScrapingFilter && pPAprimaryVertexFilter"; 
    if(isHI) skimCut = "pcollisionEventSelection";

    TString outfname = Form("%s_MC_weights.root",cap.Data());
    //if(isHI) outfname = "pbpb_MC_weights_Cymbal.root"; 
    TFile* weightsFile = TFile::Open(Form("%s",outfname.Data()), "RECREATE");
    
    //Define histograms
    TH1D* hvz_data = new TH1D("hvz_data", ";vz;", 75, -15, 15);
    TH1D* hvz_mc = new TH1D("hvz_mc", ";vz;", 75, -15, 15);
    TH1D* hvz = new TH1D("hvz", ";vz;Weight factor", 75, -15, 15);
    TH1D* hcent_data = new TH1D("hcent_data", ";Centrality bins;", 200, 0, 200);
    TH1D* hcent_mc = new TH1D("hcent_mc", ";Centrality bins;", 200, 0, 200);
    TH1D* hcent = new TH1D("hcent", ";Centrality bins;Weight factor", 200, 0, 200);
    TH1D* hweights = new TH1D("hweights", ";Centrality bins;Weight factor", 200, 0, 200);

    TF1* f_vertex;
    TF1* f_mc_vertex;
    TF1* f_cent_ratio;
   // TF1* f_cent;
   // TF1* f_mc_cent;
   
    t_event->Draw("vz>>hvz_data",skimCut);
    t_event_mc->Draw("vz>>hvz_mc");
    hvz_data->Scale(1./hvz_data->Integral());
    hvz_mc->Scale(1./hvz_mc->Integral());
    hvz_data->Fit("gaus");
    f_vertex = (TF1*)hvz_data->GetFunction("gaus");
    hvz_mc->Fit("gaus");
    f_mc_vertex = (TF1*)hvz_mc->GetFunction("gaus");
    
    for (int32_t ibin=1; ibin<hvz_data->GetNbinsX()+1; ++ibin) {
        double xbin_centre, fit_value, fit_value_mc;
        xbin_centre = hvz_data->GetXaxis()->GetBinCenter(ibin);
        fit_value = f_vertex->Eval(xbin_centre);
        fit_value_mc = f_mc_vertex->Eval(xbin_centre);
        hvz->SetBinContent(ibin,fit_value / fit_value_mc);
    }
    //hvz->Scale(1/hvz->GetMaximum());

    TH1D* hcent_tmp;
    if(isHI){ 
        t_event->Draw("hiBin>>hcent_data",skimCut);
        t_event_mc->Draw("hiBin>>hcent_mc");
        hcent_data->Scale(1./hcent_data->Integral());
        hcent_mc->Scale(1./hcent_mc->Integral());
        hcent_tmp = (TH1D*) hcent_data->Clone("hcent_tmp");
        hcent_tmp->Divide(hcent_mc);
        hcent_tmp->Fit("pol9","","",0,200);
        hcent_tmp->Fit("pol9","","",0,200);
        hcent_tmp->Fit("pol9","","",0,200);
        f_cent_ratio = (TF1*)hcent_tmp->GetFunction("pol9");
        
        for (int32_t ibin=1; ibin<hcent_data->GetNbinsX()+1; ++ibin) {
            double xbin_centre, fit_value, fit_value_mc;
            xbin_centre = hcent_data->GetXaxis()->GetBinCenter(ibin);
            fit_value = f_cent_ratio->Eval(xbin_centre);
            hcent->SetBinContent(ibin,fit_value);
        }
        //hcent->Scale(1./hcent->GetMaximum());

       // hcent_data->Fit("pol9");
       // f_cent = (TF1*)hcent_data->GetFunction("pol9");
       // hcent_mc->Fit("pol9", "", "", 0, 200);
       // f_mc_cent = (TF1*)hcent_mc->GetFunction("pol9");
        
       // for (int32_t ibin=1; ibin<hcent_data->GetNbinsX()+1; ++ibin) {
       //     double xbin_centre, fit_value, fit_value_mc;
       //     xbin_centre = hcent_data->GetXaxis()->GetBinCenter(ibin);
       //     fit_value = f_cent->Eval(xbin_centre);
       //     fit_value_mc = f_mc_cent->Eval(xbin_centre);
       //     hcent->SetBinContent(ibin,fit_value / fit_value_mc);
       // }
       // hcent->Scale(1/hcent->GetMaximum());
    }


    /////////////////////////////////////////////////
    // Draw
    TLegend* l1 = new TLegend(0.65,0.750,0.92,0.95);
    legStyle(l1);
    l1->AddEntry(hvz_data,"DATA","p");
    l1->AddEntry(f_vertex,"DATA fit","l");
    l1->AddEntry(hvz_mc,"MC","p");
    l1->AddEntry(f_mc_vertex,"MC fit", "l");

    TCanvas* cvz = new TCanvas("cvz","",400,600);
    ratioPanelCanvas(cvz);
    cvz->cd(1);
    hvz_data->SetLineColor(3);
    hvz_data->SetMarkerColor(kGreen+1);
    hvz_mc->SetLineColor(4);
    hvz_mc->SetMarkerColor(kBlue+1);
    f_vertex->SetLineColor(3);
    f_mc_vertex->SetLineColor(4);
    
    hvz_data->Draw("pe");
    hvz_mc->Draw("pe same");
    f_vertex->Draw("same");
    f_mc_vertex->Draw("same");
    l1->Draw("same");
    
    cvz->cd(2);
    hvz->Draw();
    cvz->SaveAs(Form("figures/MC_ReweightFactor_vz_%s.pdf",cap.Data()));

    TCanvas* ccent = new TCanvas("ccent","",400,600);
    ratioPanelCanvas(ccent);
    if(isHI){
        ccent->cd(1);
        hcent_data->SetLineColor(3);
        hcent_data->SetMarkerColor(kGreen+1);
        hcent_mc->SetLineColor(4);
        hcent_mc->SetMarkerColor(kBlue+1);
        f_cent_ratio->SetLineColor(3);
        //f_mc_cent->SetLineColor(4);
        
        hcent_data->Draw("pe");
        hcent_mc->Draw("pe same");
       // f_cent->Draw("same");
       // f_mc_cent->Draw("same");
        l1->Draw("same");
        ccent->cd(2);
        hcent->Draw();
        ccent->SaveAs(Form("figures/MC_ReweightFactor_cent_%s.pdf",cap.Data()));
    }

    //save
    if(isHI){ 
        hcent_mc->Write();
        hcent_data->Write();
        hcent->Write();
        f_cent_ratio->Write();
        hcent_tmp->Write();
        //f_mc_cent->Write();
        ccent->Write();
    }
    hvz_mc->Write();
    hvz_data->Write();
    hvz->Write();
    f_vertex->Write();
    f_mc_vertex->Write();
    cvz->Write();

    weightsFile->Close();
}
