// To get (( Trigger Efficiency = events passing the trigger / events which have isolated photons over a given trigger threshold ))
// Date : 2018 July 17
// Creator : Yeonju Go 

#include "../Utilities/yjUtility.h"
//#include "../phoRaaCuts/phoRaaCuts_temp.h"
#include "../TreeHeaders/ggHiNtuplizerTree.h"
const float delta2 = 0.15*0.15;
const float delta = 0.15;
///////////// BINNING ///////////////////
const double ptBins[] = {40,50,60,80,100,130};
const double ptBins_draw[] = {40,50,60,80,100,130};
const double ptBins_draw_final[] = {30,40,50,60,80,100,130};
const double ptBins_mean_pbpb[] = {44.1827,54.2862,67.7425,88.1946,111.643};
const double ptBins_mean_pp[] = {44.5213,54.188,67.5644,88.1521,111.85};
const int nPtBin = sizeof(ptBins)/sizeof(double) -1;
const double ptBins_i[] = {40,40,50,60,80,100};
const double ptBins_f[] = {130,50,60,80,100,130};
const int nPtBinIF = sizeof(ptBins_i)/sizeof(double);
const int centBins[] = {0,20,60,100,200};
const int nCentBin = sizeof(centBins)/sizeof(int) -1;
const int centBins_i[] = {0,0,20,60,100};
const int centBins_f[] = {200,20,60,100,200};
const int nCentBinIF = sizeof(centBins_i)/sizeof(int);

void getTrigEff_fromInputHist(TString coll="pp", int eThr = 20)
{
    cout << " :::::: reco_efficiency_withForest.C :::::: " << endl;
    TH1::SetDefaultSumw2();
    gStyle->SetOptStat(0);
    //SetyjPadStyle();

    TFile* f1 = new TFile(Form("%s_trigger_efficiency.root",coll.Data()),"READ");
    
    ///////////////////////////////////////////////////////////////////////////
    // HISTOGRAM DEFINE
    Int_t nCENTBINS = nCentBinIF;
    if(coll=="pp") nCENTBINS = 1;
    //nCENTBINS=2; 
    TH1D* h1D_Num[nCENTBINS];//
    TH1D* h1D_Den[nCENTBINS];//
    TH1D* h1D_Eff[nCENTBINS];//
    
    TH1D* h1D_Num_wBaseTrig[nCENTBINS];//
    TH1D* h1D_Den_wBaseTrig[nCENTBINS];//
    TH1D* h1D_Eff_wBaseTrig[nCENTBINS];//
    double maxPhoPt = 130;
    double minPhoPt = 0;
    const int nPtBins = maxPhoPt-minPhoPt;

    for(Int_t i=0;i<nCENTBINS;++i){
        h1D_Num[i] = (TH1D*) f1->Get(Form("trigEff_num_cent%d",i));
        h1D_Den[i] = (TH1D*) f1->Get(Form("trigEff_den_cent%d",i));
        h1D_Num_wBaseTrig[i] = (TH1D*) f1->Get(Form("trigEff_wBaseTrig_num_cent%d",i));
        h1D_Den_wBaseTrig[i] = (TH1D*) f1->Get(Form("trigEff_wBaseTrig_den_cent%d",i));
    } 

    for(Int_t i=0;i<nCENTBINS;++i){ 
        h1D_Eff[i] = (TH1D*) h1D_Num[i]->Clone(Form("hEff_trig_HISinglePhoton%d_Eta1p5_v1_%d",eThr,i)); h1D_Eff[i]->Reset();
        h1D_Eff[i] -> Divide(h1D_Num[i],h1D_Den[i],1.,1.,"B");
        h1D_Eff_wBaseTrig[i] = (TH1D*) h1D_Num[i]->Clone(Form("hEff_trig_HISinglePhoton10_%d_Eta1p5_v1_%d",eThr,i)); h1D_Eff_wBaseTrig[i]->Reset();
        h1D_Eff_wBaseTrig[i] -> Divide(h1D_Num[i],h1D_Den[i],1.,1.,"B");
    }

    TFile* fout = new TFile(Form("%s_trigger_efficiency_fromInputHist.root",coll.Data()),"RECREATE");   
    fout->cd();
    for(Int_t i=0;i<nCENTBINS;++i){ 
      h1D_Num[i]->Write();
      h1D_Den[i]->Write();
      h1D_Eff[i]->Write();
    }
    fout->Close();
}
