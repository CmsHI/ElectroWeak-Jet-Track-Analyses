#include "TFile.h"
#include "TTree.h"
#include "TStyle.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TLegend.h"

#include "TString.h"

#include <vector>
#include "../Utilities/interface/InputConfigurationParser.h"
#include "../Utilities/mathUtil.h"
#include "../../yjUtility.h"

double myFunc(double *x, double *par){
    return par[0]+ par[1]/sqrt(x[0]) + par[2]/x[0];
}

void makeFinalCorrFile(bool isPP = false){

    // caption
    TString cap = "";
    cap = "_pbpb_COMB_AllQCD_0726";
    TString outfname = Form("photonEnergyCorrections%s.root",cap.Data());
    
    // kinematic ranges
    int centBins[2][7] = {{0,10,20,40,60,100,140},{10,20,40,60,100,140,200}};
    int nCentBins = 7;
    float etaBins[2][2] = {{0,1.44},{1.44,2.4}};
    int nEtaBins = 2; 

    if(isPP){ nCentBins = 1;}
    gStyle->SetOptStat(0);

    ////////////////////////////////////////////////////////
    // mean of reco/gen pt for each pt bin  
    TF1 *f_mean_gaus[nCentBins][nEtaBins];
    for (int i=0; i<nCentBins; ++i) {
        for (int j=0; j<nEtaBins; ++j) {
            f_mean_gaus[i][j] = new TF1(Form("f_mean_gaus_cent%d_eta%d",i,j), myFunc, 0, 300, 3);
            f_mean_gaus[i][j] -> SetParNames("C", "S", "N");
            if(centBins[0][i]==0 || centBins[0][i]==10){ 
                f_mean_gaus[i][j] -> SetParameters(9.850968e-01,5.382314e-01,-4.497160e-01);
            } else if(centBins[0][i]==20){
                f_mean_gaus[i][j] -> SetParameters(9.439817e-01,8.568852e-01,-1.819416e+00);
            } else if(centBins[0][i]==40){
                f_mean_gaus[i][j] -> SetParameters(9.533159e-01,7.007214e-01,-2.043700e+00);
            } else if(centBins[0][i]==60){
                f_mean_gaus[i][j] -> SetParameters(1.00,0.00,0.00);
            } else if(centBins[0][i]==100){
                f_mean_gaus[i][j] -> SetParameters(9.815839e-01,3.028975e-01,-1.571576e+00);
            } else if(centBins[0][i]==140){
                f_mean_gaus[i][j] -> SetParameters(9.864968e-01,2.202094e-01,-1.370800e+00);
            }
        }
    }
    
    TFile* inf = new TFile("/u/user/goyeonju/ElectroWeak-Jet-Track-Analyses/Corrections/photonEnergyCorrections_pbpb_COMB_AllQCD_0725.root");
    TF1 *f_rms[nCentBins][nEtaBins];
    TF1 *f_sig_gaus[nCentBins][nEtaBins];
    for (int i=0; i<nCentBins; ++i) {
        for (int j=0; j<nEtaBins; ++j) {
            f_rms[i][j] = (TF1*) inf->Get(Form("f_rms_cent%d_eta%d",i,j)); 
            f_sig_gaus[i][j] = (TF1*) inf->Get(Form("f_sig_gaus_cent%d_eta%d",i,j)); 
        }
    }

    /////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    // Save 
    TFile* energyCorrectionFile = TFile::Open(Form("%s",outfname.Data()), "recreate");
    energyCorrectionFile->cd();
    for (int i=0; i<nCentBins; ++i) {
        for (int j=0; j<nEtaBins; ++j) {
            f_rms[i][j]->Write();
            f_sig_gaus[i][j]->Write();
            f_mean_gaus[i][j]->Write();
        }
    }
    energyCorrectionFile->Close();
}
