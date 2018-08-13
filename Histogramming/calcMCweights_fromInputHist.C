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

void calcMCweights_fromInputHist(bool isHI=true) {
    cout << "Is it heavy ion collisions? " << isHI << endl;
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(0);
    TH1::SetDefaultSumw2();

    TString cap = "pp";
    if(isHI) cap = "pbpb";
    TCut skimCut = "pBeamScrapingFilter && pPAprimaryVertexFilter"; 
    if(isHI) skimCut = "pcollisionEventSelection";
    
    TString outfname = Form("%s_MC_weights_0717.root",cap.Data());
    if(isHI) outfname = "pbpb_MC_weights_Cymbal_0717.root"; 
    TFile* weightsFile = TFile::Open(Form("%s",outfname.Data()), "RECREATE");

    TString infname = "pp_MC_weights.root";
    if(isHI) infname = "pbpb_MC_weights_Cymbal.root";
    TFile* inf = new TFile(infname,"READ");

    //Define histograms
    TH1D* hvz_data = (TH1D*) inf->Get("hvz_data"); 
    TH1D* hvz_mc =  (TH1D*) inf->Get("hvz_mc"); 
    TH1D* hvz =  (TH1D*) inf->Get("hvz"); 
    TH1D* hcent_data =  (TH1D*) inf->Get("hcent_data"); 
    TH1D* hcent_mc =  (TH1D*) inf->Get("hcent_mc"); 
    TH1D* hcent =  (TH1D*) inf->Get("hcent"); 
    TH1D* hcent_smooth = (TH1D*) hcent->Clone("hcent_smooth"); 

    TF1* f_vertex;
    TF1* f_mc_vertex;
    TF1* f_cent;
    TF1* f_mc_cent;
    TF1* f_cent_ratio;
    //TF1* f_cent_ratio = new TF1("f_cent_ratio","pol9",0,160);;
    //f_cent_ratio->SetParameters(1.034289e+01,-4.391030e-01,1.582026e-02,-4.029602e-04,6.429773e-06,-6.470681e-08,4.132624e-10,-1.630315e-12,3.633860e-15,-3.508813e-18);
   
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
    hvz->Scale(1./hvz->GetMaximum());

    if(isHI){ 
       // hcent_data->Fit("pol9");
       // hcent_data->Fit("pol9");
       // hcent_data->Fit("pol9");
       // f_cent = (TF1*)hcent_data->GetFunction("pol9");
       // hcent_mc->Fit("pol9", "", "", 0, 200);
       // hcent_mc->Fit("pol9", "", "", 0, 200);
       // hcent_mc->Fit("pol9", "", "", 0, 200);
       // f_mc_cent = (TF1*)hcent_mc->GetFunction("pol9");
       // 
       // for (int32_t ibin=1; ibin<hcent_data->GetNbinsX()+1; ++ibin) {
       //     double xbin_centre, fit_value, fit_value_mc;
       //     xbin_centre = hcent_data->GetXaxis()->GetBinCenter(ibin);
       //     fit_value = f_cent->Eval(xbin_centre);
       //     fit_value_mc = f_mc_cent->Eval(xbin_centre);
       //     hcent->SetBinContent(ibin,fit_value / fit_value_mc);
       // }
        for (int32_t ibin=1; ibin<hcent_data->GetNbinsX()+1; ++ibin) {
            double mc, data, ratio;
            double mc_sig, data_sig, ratio_err;
            data = hcent_data->GetBinContent(ibin);
            mc = hcent_mc->GetBinContent(ibin);
            data_sig = hcent_data->GetBinError(ibin)/data;
            mc_sig = hcent_mc->GetBinError(ibin)/mc;
            if((mc==0 || data==0) && ibin!=1){
                ratio = hcent->GetBinContent(ibin-1);
                ratio_err = hcent->GetBinError(ibin-1);
            }else{
                ratio = data/mc; 
                ratio_err = ratio*TMath::Sqrt(data_sig*data_sig+mc_sig*mc_sig); 
            }
           // cout << ratio << endl;
            
            hcent->SetBinContent(ibin,ratio);
            hcent->SetBinError(ibin,ratio_err);
        }
        //hcent->Scale(1/hcent->GetMaximum());
        hcent->Fit("pol9","","",0,200);
        hcent->Fit("pol9","","",0,200);
        hcent->Fit("pol9","","",0,200);
        f_cent_ratio = (TF1*)hcent->GetFunction("pol9");
        for (int32_t ibin=1; ibin<hcent_data->GetNbinsX()+1; ++ibin) {
            double xbin_centre, fit_value, fit_value_mc;
            xbin_centre = hcent_data->GetXaxis()->GetBinCenter(ibin);
            fit_value = f_cent_ratio->Eval(xbin_centre);
            hcent_smooth->SetBinContent(ibin,fit_value);
        }
        hcent_smooth->Scale(1./hcent_smooth->GetMaximum());
    }


    /////////////////////////////////////////////////
    // Draw
    TLegend* l1 = new TLegend(0.75,0.65,0.92,0.92);
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
   // hvz_data->Scale(1./hvz_data->Integral());
   // hvz_mc->Scale(1./hvz_mc->Integral());
    if(isHI){
        hvz_mc->Draw("pe");
        hvz_data->Draw("pe same");
    } else{
        hvz_data->Draw("pe");
        hvz_mc->Draw("pe same");
    }
    f_vertex->Draw("same");
    f_mc_vertex->Draw("same");
    l1->Draw("same");

    cvz->cd(2);
    hvz->SetTitle(";vz;Weight factor");
    hvz->Draw();
    cvz->SaveAs(Form("figures/MC_ReweightFactor_vz_%s.pdf",cap.Data()));

    TLegend* l2 = new TLegend(0.7,0.750,0.92,0.92);
    legStyle(l2);
    l2->AddEntry(hvz_data,"DATA","p");
    l2->AddEntry(hvz_mc,"MC","p");
    TCanvas* ccent = new TCanvas("ccent","",400,600);
    ratioPanelCanvas(ccent);
    if(isHI){
        ccent->cd(1);
        hcent_data->SetLineColor(3);
        hcent_data->SetMarkerColor(kGreen+1);
        hcent_mc->SetLineColor(4);
        hcent_mc->SetMarkerColor(kBlue+1);
        hcent_data->GetFunction("pol9")->SetBit(TF1::kNotDraw);
        hcent_mc->GetFunction("pol9")->SetBit(TF1::kNotDraw);
       // hcent_data->Scale(1./hcent_data->Integral());
       // hcent_mc->Scale(1./hcent_mc->Integral());
        hcent_data->Draw("pe");
        hcent_mc->Draw("pe same");
        l2->Draw("same");
        ccent->cd(2);
       // hcent->Draw();
        hcent_smooth->SetLineColor(1);
        hcent_smooth->SetTitle(";Centrality bins;Weight factor");
        hcent_smooth->Draw();
        //f_cent_ratio->Draw("same");
        ccent->SaveAs(Form("figures/MC_ReweightFactor_cent_%s.pdf",cap.Data()));
    }

    hcent = (TH1D*) hcent_smooth->Clone("hcent");
    //save
    weightsFile->cd();
    if(isHI){ 
        hcent_mc->Write();
        hcent_data->Write();
        hcent->Write();
        //f_cent->Write();
        //f_mc_cent->Write();
        ccent->Write();
    }
    hvz_mc->Write();
    hvz_data->Write();
    hvz->Write();
    f_vertex->Write();
    f_mc_vertex->Write();
    cvz->Write();

    //weightsFile->Close();
}
