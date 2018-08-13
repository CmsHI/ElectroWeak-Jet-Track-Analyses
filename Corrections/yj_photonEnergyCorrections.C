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

//#include "../TreeHeaders/CutConfigurationTree.h"
//#include "../Utilities/interface/CutConfigurationParser.h"
#include "../Utilities/interface/InputConfigurationParser.h"
#include "../Utilities/mathUtil.h"
#include "../../yjUtility.h"

#define _SET_BRANCH_VEC(tree, type, branch)     \
    std::vector<type>* branch = 0;              \
tree->SetBranchStatus(#branch, 1);          \
tree->SetBranchAddress(#branch, &branch);   \

#define _SET_BRANCH_VAR(tree, type, branch)     \
    type branch;                                \
tree->SetBranchStatus(#branch, 1);          \
tree->SetBranchAddress(#branch, &branch);   \

float dR(float eta1, float eta2, float phi1, float phi2);
double myFunc(double *x, double *par){
    return par[0]+ par[1]/sqrt(x[0]) + par[2]/x[0];
}
TH1F* getPullHist(TH1F *h1, TF1 *f);
void yj_photonEnergyCorrections(bool isPP = true, TString fnc_name = "DSCB", bool doIsoCut=false, bool isPhotonSample=true){

    // caption
    TString collSt = "pbpb";
    if(isPP) collSt = "pp";
    TString GEDcap = "";
    if(isPP) GEDcap = "GED";
    TString cap = Form("%s_%s",collSt.Data(),fnc_name.Data());
    if(isPP) cap = Form("%s_GED_%s",collSt.Data(),fnc_name.Data());
    if(isPhotonSample) cap += "_AllQCD"; 
    else cap += "_ZtoEE"; 
    if (doIsoCut) cap += "_isoCut";
    cap += "_0729";
    //cap += "_0721_finerBinning";

    // Input Files
    std::string input = "/u/user/goyeonju/ElectroWeak-Jet-Track-Analyses/ShellScripts/photonRaa/filelist/PbPb_MC_ZtoEE.list";
    TString outfname = Form("photonEnergyCorrections_%s.root",cap.Data());
    if(!isPhotonSample && isPP) 
        input = "/u/user/goyeonju/ElectroWeak-Jet-Track-Analyses/ShellScripts/photonRaa/filelist/pp_MC_ZtoEE.list";
    else if(isPhotonSample && !isPP) 
        input = "/u/user/goyeonju/ElectroWeak-Jet-Track-Analyses/ShellScripts/photonRaa/filelist/PbPb_MC_Cymbal.list";
    else if(isPhotonSample && isPP) 
        input = "/u/user/goyeonju/ElectroWeak-Jet-Track-Analyses/ShellScripts/photonRaa/filelist/pp_MC_v1.list";
    else{
        input = "";
        cout << "no input file specified" << endl;
    }

    // Get tree 
    std::vector<std::string> input_list = InputConfigurationParser::ParseFiles(input);
    printf("number of input files: %zu\n", input_list.size());

    cout <<"photon tree = " <<  Form("ggHiNtuplizer%s/EventTree",GEDcap.Data()) << endl;
    TChain* t_photon = new TChain(Form("ggHiNtuplizer%s/EventTree",GEDcap.Data()));
    TChain* t_event= new TChain("hiEvtAnalyzer/HiTree");
    
    for (std::vector<std::string>::iterator it = input_list.begin(); it != input_list.end(); ++it){
        t_photon->Add((*it).c_str());
        t_event->Add((*it).c_str());
        cout << (*it).c_str() << endl;
    }

    t_photon->SetBranchStatus("*", 0);
    t_event->SetBranchStatus("*", 0);

    _SET_BRANCH_VEC(t_photon, float, mcEt);
    _SET_BRANCH_VEC(t_photon, float, mcEta);
    _SET_BRANCH_VEC(t_photon, float, mcPhi);
    _SET_BRANCH_VEC(t_photon, float, mcCalIsoDR04);
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
    _SET_BRANCH_VEC(t_photon, int, pho_genMatchedIndex);

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

    _SET_BRANCH_VAR(t_event, int, hiBin);

    // kinematic ranges
    int centBins[2][7] = {{0,10,20,40,60,100,140},{10,20,40,60,100,140,200}};
    int nCentBins = 7;
    //int centBins[2][5] = {{0,20,60,100,140},{20,60,100,140,200}};
    //int nCentBins = 5;
    float etaBins[2][2] = {{0,1.44},{1.44,2.4}};
    int nEtaBins = 2; 

    if(isPP){ nCentBins = 1;}
    gStyle->SetOptStat(0);

    cout << "sss"<< endl;
    double ptBins_normal[] = {20,22,24,26,28,30,35,40,50,60,70,80,90,100,110,120,140};
    const int nPtBins_normal = sizeof(ptBins_normal)/sizeof(double) -1;
    double ptBins_isoCut[] = {20,25,30,35,40,50,60,70,80,90,100,110,120,140};
    const int nPtBins_isoCut = sizeof(ptBins_isoCut)/sizeof(double) -1;
    int nPtBins = nPtBins_normal;
    if(doIsoCut) nPtBins = nPtBins_isoCut;
    double ptBins[nPtBins];
    for(int i=0; i<nPtBins+1; ++i){
        if(doIsoCut) ptBins[i] = ptBins_isoCut[i];
        else ptBins[i] = ptBins_normal[i];
        cout << "ipt = " << i << " : ptBin = " << ptBins[i] << endl;
    } 
    
    //double ptBins[] = {10,20,30,40,50,60,70,80,90,100,120,150,200};
    //double ptBins[] = {10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200};
    //const int nPtBins = sizeof(ptBins)/sizeof(double) -1;
    const int nPtBins_2d = 50; 
    int nbins = 100; //for ratio
    if(collSt=="pp") nbins = 500;
    TH2F* h_pt[nCentBins][nEtaBins];
    TH2F* h_ptratio[nCentBins][nEtaBins];
    TProfile* h_ptprof[nCentBins][nEtaBins];

    TH1F* h_ratio[nCentBins][nEtaBins][nPtBins];
    TH1F* h_ratio_pull[nCentBins][nEtaBins][nPtBins];    
    TH1F* h_mean_gaus[nCentBins][nEtaBins];
    TH1F* h_sig_gaus[nCentBins][nEtaBins];
    TH1F* h_mean[nCentBins][nEtaBins];
    TH1F* h_rms[nCentBins][nEtaBins];

    for (int i=0; i<nCentBins; ++i) {
        for (int j=0; j<nEtaBins; ++j) {
            TString centSt = Form("%i-%i%%", centBins[0][i]/2, centBins[1][i]/2); 
            if(isPP) centSt = "pp";
            h_pt[i][j] = new TH2F(Form("h_pt_%i_%i", i, j), Form("%s, %.2f<|#eta|<%.2f;reco p_{T}^{#gamma};gen p_{T}^{#gamma}", centSt.Data(), etaBins[0][j], etaBins[1][j]), nPtBins_2d, 0, 200, nPtBins_2d, 0, 200);
            h_ptratio[i][j] = new TH2F(Form("h_ptratio_%i_%i", i, j), Form("%s, %.2f<|#eta|<%.2f;reco p_{T}^{#gamma};#frac{reco p_{T}^{#gamma}}{gen p_{T}^{#gamma}}", centSt.Data(), etaBins[0][j], etaBins[1][j]), nPtBins_2d, 0, 200, 50, 0, 2);
            h_mean_gaus[i][j] = new TH1F(Form("h_mean_gaus_%i_%i", i, j), Form("%s, %.2f<|#eta|<%.2f;reco p_{T}^{#gamma};<#frac{reco p_{T}^{#gamma}}{gen p_{T}^{#gamma}}>", centSt.Data(), etaBins[0][j], etaBins[1][j]), nPtBins, ptBins);
            h_sig_gaus[i][j] = new TH1F(Form("h_sigma_gaus_%i_%i", i, j), Form("%s, %.2f<|#eta|<%.2f;reco p_{T}^{#gamma};#sigma(#frac{reco p_{T}^{#gamma}}{gen p_{T}^{#gamma}})", centSt.Data(), etaBins[0][j], etaBins[1][j]), nPtBins, ptBins);
            h_mean[i][j] = (TH1F*)  h_mean_gaus[i][j]->Clone(Form("h_mean_%i_%i", i, j));
            h_rms[i][j] = (TH1F*)  h_sig_gaus[i][j]->Clone(Form("h_rms_%i_%i", i, j));
            for (int k=0; k<nPtBins; ++k) {
                h_ratio[i][j][k] = new TH1F(Form("h_ratio_cent%i_eta%i_pt%i", i, j, k), ";#frac{reco p_{T}^{#gamma}}{gen p_{T}^{#gamma}};", nbins,0.0,2.0);
            }
        }
    }

    uint64_t nentries = t_photon->GetEntries();
    for (uint64_t i=0; i<nentries; ++i) {
        if (i % 100000 == 0)
            printf("entry: %lu / %lu \n", i,nentries);

        t_photon->GetEntry(i);
        t_event->GetEntry(i);

        for (uint32_t j=0; j<phoEt->size(); ++j) {
            if (phoEt->at(j) < 10)
                continue;
            if (fabs(phoEta->at(j)) > 2.4)
                continue;
            if (pho_genMatchedIndex->at(j) < 0)
                continue;
            if (pho_swissCrx->at(j) > 0.9)
                continue;
            if (fabs(pho_seedTime->at(j)) > 3)
                continue;
            if (doIsoCut && phoHoverE->at(j)>0.1) 
                continue;
            if (doIsoCut && phoSigmaIEtaIEta_2012->at(j)>0.011) 
                continue;
            if (doIsoCut && (pho_ecalClusterIsoR4->at(j)+pho_hcalRechitIsoR4->at(j)+pho_trackIsoR4PtCut20->at(j))>5) 
                continue;

            if(!isPhotonSample){ //Electron matching is only for ZtoEE sample
                int matchedEleIndex = -1;
                float matchedEoverP = 100;
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
                if (matchedEleIndex == -1)
                    continue;
            }

            int icent = 0;
            if(!isPP){
                for (; hiBin>=centBins[1][icent] && icent<nCentBins; ++icent);
            }
            int32_t mcindex = pho_genMatchedIndex->at(j);

            if (mcEt->at(mcindex) < 10)
                continue;
            if (mcCalIsoDR04->at(mcindex) > 5)
                continue;

            int ieta = 0;
            for (; fabs(phoEta->at(j))>=etaBins[1][ieta] && ieta<nEtaBins; ++ieta);

            int ipt = 0;
            for (; fabs(phoEt->at(j))>=ptBins[ipt+1] && ipt<nPtBins; ++ipt){
                if(ipt>nPtBins) break;    
            }

            // cout << " photon number : " << j << " / " << phoEt->size() << " ::: photon Et = " << phoEt->at(j) <<"::: ipt = " << ipt << ", nPtBins = "<< nPtBins <<  endl; 
            h_pt[icent][ieta]->Fill(phoEt->at(j), mcEt->at(mcindex));
            h_ptratio[icent][ieta]->Fill(phoEt->at(j), phoEt->at(j)/mcEt->at(mcindex));
            if(ipt<nPtBins) h_ratio[icent][ieta][ipt]->Fill(phoEt->at(j)/mcEt->at(mcindex));
        }
    }

    gStyle->SetOptFit(0);
    gStyle->SetOptStat(0);

    ///////////////////////////////////////
    // Gaus or DSCB fitting for reco/gen pt ratio  
    TCanvas* c[nCentBins][nEtaBins];
    TCanvas* ctemp = new TCanvas("ctemp","",300,300);
    TF1* ff[nCentBins][nEtaBins][nPtBins];
    for (int i=0; i<nCentBins; ++i) {
        for (int j=0; j<nEtaBins; ++j) {
            c[i][j] = new TCanvas(Form("can_cent%d_eta%d",i,j),"",250*4,250*3);
            c[i][j]->Divide(5,4);
            for (int k=0; k<nPtBins; ++k) {
                ctemp->cd();
                //h_ratio[i][j][k]->Scale(1./h_ratio[i][j][k]->GetMaximum());
                // gaussian or Double-sided crystal ball fitting 
                if(fnc_name=="gaus"){ 
                    ff[i][j][k] = cleverGaus(h_ratio[i][j][k], Form("f_%s",h_ratio[i][j][k]->GetName()));
                } else if(fnc_name=="DSCB"){
                    Int_t peakBin  = h_ratio[i][j][k]->GetMaximumBin();
                    Double_t peakContent  = h_ratio[i][j][k]->GetBinContent(peakBin);
                    Double_t peak =  h_ratio[i][j][k]->GetBinCenter(peakBin);
                    Double_t sigma = h_ratio[i][j][k]->GetRMS(); 
                    if(collSt =="pbpb"){
                        ff[i][j][k] = new TF1(Form("f1_%d_%d_%d",i,j,k), fnc_DSCB, peak-2.5*sigma, peak+2.5*sigma, 7);
                        if(i==0 && j==0 && (k==0)){
                            ff[i][j][k]->SetParameters(peakContent,peak,4.76101e-02,1.28763e+00,6.83786e+00,5.07594e-01,1.20400e+02);
                            ff[i][j][k]->SetParLimits(1,1.04,1.10);
                        }else if(i==0 && j==0 && k==2)
                            ff[i][j][k]->SetParameters(peakContent,1.03510e+00,-3.41453e-02,6.33186e-01,1.29222e+02,1.37638e+00,5.69004e+00);
                        else if(i==2 && j==0 && k==2)
                            ff[i][j][k]->SetParameters(peakContent,1.00215e+00,-1.43638e-02,9.16937e-01,1.18086e+01,8.16771e-01,4.69242e+00);
                        else if(i==4 && j==0 && (k==0))
                            ff[i][j][k]->SetParameters(peakContent,peak,1.57862e-02,4.09805e-01,4.31433e+00,1.43539e+00,3.52302e+00);
                        else if(i==4 && j==0 && (k==1))
                            ff[i][j][k]->SetParameters(peakContent,9.95122e-01,-1.28811e-02,2.33030e+00,2.73892e+00,7.29685e-01,4.33112e+00);
                        else
                            ff[i][j][k]->SetParameters(peakContent,peak,sigma,0.79,3.63,0.59,1.67);
                    } else{ // pp
                        ff[i][j][k] = new TF1(Form("f1_%d_%d_%d",i,j,k), fnc_DSCB, peak-2.5*sigma, peak+2.5*sigma, 7);
                        ff[i][j][k]->SetParameters(peakContent,peak,sigma/3.,6.33186e-01,1.29222e+02,1.37638e+00,5.69004e+00);
                        if(i==0 && j==0 && (k<=1))
                            ff[i][j][k]->SetParameters(peakContent,peak,sigma/3.,6.33186e-01,1.29222e+02,1.37638e+00,5.69004e+00);
                        else if(i==0 && j==0 && (k==2))
                            ff[i][j][k]->SetParameters(peakContent,peak,sigma/2.,6.33186e-01,1.29222e+02,1.37638e+00,5.69004e+00);
                        else if(i==0 && j==0 && (k==4 || k==5))
                            ff[i][j][k]->SetParameters(peakContent,peak,sigma/5.,9.16937e-01,1.18086e+01,8.16771e-01,4.69242e+00);
                        else if(i==0 && j==0 && (k==9 || k==10))
                            ff[i][j][k]->SetParameters(peakContent,peak,sigma/5.,2.16937e-01,1.18086e+01,2.16771e-01,4.69242e+00);
                    }
                    ff[i][j][k]->SetParLimits(0,peakContent*0.9,peakContent*1.1);
                    ff[i][j][k]->SetParLimits(2,0.005,0.06);
                    h_ratio[i][j][k]->Fit(ff[i][j][k],"LL M O Q R");
                    h_ratio[i][j][k]->Fit(ff[i][j][k],"LL M O Q R");
                    h_ratio[i][j][k]->Fit(ff[i][j][k],"LL M O Q R");
                } else if(fnc_name=="COMB"){
                    Int_t peakBin  = h_ratio[i][j][k]->GetMaximumBin();
                    Double_t peakContent  = h_ratio[i][j][k]->GetBinContent(peakBin);
                    Double_t peak =  h_ratio[i][j][k]->GetBinCenter(peakBin);
                    Double_t sigma = h_ratio[i][j][k]->GetRMS(); 
                    if(collSt =="pbpb"){
                        if(centBins[0][i]==100 || centBins[0][i]==140){
                            ff[i][j][k] = cleverGaus(h_ratio[i][j][k], Form("f_%s",h_ratio[i][j][k]->GetName()));
                        } else{
                            ff[i][j][k] = new TF1(Form("f1_%d_%d_%d",i,j,k), fnc_DSCB, peak-2.5*sigma, peak+2.5*sigma, 7);
                            ff[i][j][k]->SetParameters(peakContent,peak,sigma,0.79,3.63,0.59,1.67);
                        }
                    } else{ // pp
                        ff[i][j][k] = cleverGaus(h_ratio[i][j][k], Form("f_%s",h_ratio[i][j][k]->GetName()));
                    }
                }

                //gPad->SetLogy();
                double mean = ff[i][j][k]->GetParameter(1);
                double mean_err = ff[i][j][k]->GetParError(1);
                double resol = abs(ff[i][j][k]->GetParameter(2));
                double resol_err = abs(ff[i][j][k]->GetParError(2));
                h_mean_gaus[i][j]->SetBinContent(k+1, mean);
                h_mean_gaus[i][j]->SetBinError(k+1, mean_err);
                h_sig_gaus[i][j]->SetBinContent(k+1, resol);
                h_sig_gaus[i][j]->SetBinError(k+1, resol_err);

                // mean calculation
                mean = h_ratio[i][j][k]->GetMean();
                mean_err = h_ratio[i][j][k]->GetMeanError();
                resol = h_ratio[i][j][k]->GetRMS();
                resol_err = h_ratio[i][j][k]->GetRMSError();
                h_mean[i][j]->SetBinContent(k+1, mean);
                h_mean[i][j]->SetBinError(k+1, mean_err);
                h_rms[i][j]->SetBinContent(k+1, resol);
                h_rms[i][j]->SetBinError(k+1, resol_err);

                c[i][j]->cd(k+1);
                h_ratio[i][j][k]->GetXaxis()->SetRangeUser(0.7,1.3);
                h_ratio[i][j][k]->Draw();
                TString centSt = Form("%i-%i%%", centBins[0][i]/2, centBins[1][i]/2); 
                if(isPP) centSt = "pp";
                float xpos = 0.21;
                float dy = 0.09;
                drawText(Form("%s, %.2f<|#eta|<%.2f", centSt.Data(), etaBins[0][j], etaBins[1][j]),xpos,0.84);
                drawText(Form("%d<p_{T}^{reco #gamma}<%d GeV", (int)ptBins[k],(int)ptBins[k+1]),xpos,0.84-dy);
                drawText(Form("%s fitting", fnc_name.Data()),xpos,0.84-2*dy);

                ctemp->cd();
            }
            c[i][j]->SaveAs(Form("figures/photonEnergyCorrection_fitting_of_recoPt_over_genPt_inPtbins_%s_cent%d_eta%d.pdf",cap.Data(),i,j));
        }
    }

    ////////////////////////////////////////////////////////
    // pull distribution
    TCanvas* c_pull[nCentBins][nEtaBins];
    for (int i=0; i<nCentBins; ++i) {
        for (int j=0; j<nEtaBins; ++j) {
            c_pull[i][j] = new TCanvas(Form("can_pull_cent%d_eta%d",i,j),"",250*4,250*3);
            c_pull[i][j]->Divide(5,4);
            for (int k=0; k<nPtBins; ++k) {
                h_ratio_pull[i][j][k] = getPullHist(h_ratio[i][j][k], ff[i][j][k]);
                h_ratio_pull[i][j][k]->SetTitle(";#frac{reco p_{T}^{#gamma}}{gen p_{T}^{#gamma}};Pull");
                h_ratio_pull[i][j][k]->GetYaxis()->SetRangeUser(-5,5);
                h_ratio_pull[i][j][k]->GetXaxis()->SetRangeUser(0.7,1.3);
                c_pull[i][j]->cd(k+1);
                h_ratio_pull[i][j][k]->Draw("p");
                jumSun(0.7,0,1.3,0);
                TString centSt = Form("%i-%i%%", centBins[0][i]/2, centBins[1][i]/2);
                if(isPP) centSt = "pp";
                float xpos = 0.21;
                float dy = 0.09;
                drawText(Form("%s, %.2f<|#eta|<%.2f", centSt.Data(), etaBins[0][j], etaBins[1][j]),xpos,0.84);
                drawText(Form("%d<p_{T}^{reco #gamma}<%d GeV", (int)ptBins[k],(int)ptBins[k+1]),xpos,0.84-dy);
                drawText(Form("%s fitting", fnc_name.Data()),xpos,0.84-2*dy);
            }
            c_pull[i][j]->SaveAs(Form("figures/photonEnergyCorrection_PullDist_fitting_of_recoPt_over_genPt_nPtbins_%s_cent%d_eta%d.pdf",cap.Data(),i,j));
        }
    }

    ////////////////////////////////////////////////////////
    // mean of reco/gen pt for each pt bin  
    TF1 *f_mean[nCentBins][nEtaBins];
    TF1 *f_mean_gaus[nCentBins][nEtaBins];
    for (int i=0; i<nCentBins; ++i) {
        for (int j=0; j<nEtaBins; ++j) {
            f_mean_gaus[i][j] = new TF1(Form("f_mean_gaus_cent%d_eta%d",i,j), myFunc, 0, 300, 3);
            f_mean_gaus[i][j] -> SetParameters(0.03, 0.8, 0.01);
            f_mean_gaus[i][j] -> SetParNames("C", "S", "N");
            h_mean_gaus[i][j] -> Fit(f_mean_gaus[i][j], "RLL Q");
            h_mean_gaus[i][j] -> Fit(f_mean_gaus[i][j], "RLL Q");
            h_mean_gaus[i][j] -> Fit(f_mean_gaus[i][j], "RLL Q");

            f_mean[i][j] = new TF1(Form("f_mean_cent%d_eta%d",i,j), myFunc, 0, 300, 3);
            f_mean[i][j] -> SetParameters(0.03, 0.8, 0.01);
            f_mean[i][j] -> SetParNames("C", "S", "N");
            h_mean[i][j] -> Fit(f_mean[i][j], "RLL Q");
            h_mean[i][j] -> Fit(f_mean[i][j], "RLL Q");
            
            if(fnc_name=="COMB" && collSt =="pbpb"){
                if(centBins[0][i]==0 || centBins[0][i]==10 || centBins[0][i]==20 || centBins[0][i]==40){
                    f_mean_gaus[i][j] -> SetParameter(0,f_mean[i][j]->GetParameter(0));
                    f_mean_gaus[i][j] -> SetParameter(1,f_mean[i][j]->GetParameter(1));
                    f_mean_gaus[i][j] -> SetParameter(2,f_mean[i][j]->GetParameter(2));
                }
            } 
        }
    }
    

    ////////////////////////////////////////////////////////
    // resolution of reco/gen pt for each pt bin  
    TF1 *f_rms[nCentBins][nEtaBins];
    TF1 *f_sig_gaus[nCentBins][nEtaBins];
    for (int i=0; i<nCentBins; ++i) {
        for (int j=0; j<nEtaBins; ++j) {
            f_sig_gaus[i][j] = new TF1(Form("f_sig_gaus_cent%d_eta%d",i,j), myFunc, 0, 300, 3);
            f_sig_gaus[i][j] -> SetParameters(0.03, 0.8, 0.01);
            f_sig_gaus[i][j] -> SetParNames("C", "S", "N");
            h_sig_gaus[i][j] -> Fit(f_sig_gaus[i][j], "RLL Q");
            h_sig_gaus[i][j] -> Fit(f_sig_gaus[i][j], "RLL Q");
            h_sig_gaus[i][j] -> Fit(f_sig_gaus[i][j], "RLL Q");

            f_rms[i][j] = new TF1(Form("f_rms_cent%d_eta%d",i,j), myFunc, 0, 300, 3);
            f_rms[i][j] -> SetParameters(0.03, 0.8, 0.01);
            f_rms[i][j] -> SetParNames("C", "S", "N");
            h_rms[i][j] -> Fit(f_rms[i][j], "RLL Q");
            h_rms[i][j] -> Fit(f_rms[i][j], "RLL Q");
        }
    }

    //////////////////////////////////////////////////
    //Draw
    int j=0; // plot only barrel photons

    int nRow = 1;
    int nCol = 1;
    if(nCentBins<=2){ nCol = nCentBins; }
    else if(nCentBins>2 && nCentBins<=4){ nCol=2; nRow=2;} 
    else if(nCentBins>4 && nCentBins<=6){ nCol=3; nRow=2;} 
    else if(nCentBins>6 && nCentBins<=9){ nCol=4; nRow=2;} 
    else { nCol=4; nRow=3; }

    // 2D plot : Gen/Reco pT vs. Reco pT 
    TCanvas* c_2d = new TCanvas("c_ratio_vs_recopt", "", 500*nCol, 500 *nRow);
    c_2d->Divide(nCol,nRow);
    c_2d->SetRightMargin(0.10);
    for (int i=0; i<nCentBins; ++i) {
        c_2d->cd(i+1);
        h_ptratio[i][j]->Draw("colz");
    }
    c_2d->SaveAs(Form("figures/photonEnergyCorrection_2D_ratio_vs_recoPt_%s.pdf",cap.Data())); 
    // 2D plot : Gen vs. Reco pT 
    TCanvas* c_mat = new TCanvas("c_genpt_vs_recopt", "", 500*nCol, 500 *nRow);
    c_mat->Divide(nCol,nRow);
    c_mat->SetRightMargin(0.10);
    for (int i=0; i<nCentBins; ++i) {
        c_mat->cd(i+1);
        h_pt[i][j]->Draw("colz");
    }
    c_mat->SaveAs(Form("figures/photonEnergyCorrection_2D_genPt_vs_recoPt_%s.pdf",cap.Data())); 

    // mean(gen/reco pT) vs. Reco pT
    TCanvas* c_mean = new TCanvas("c_mean", "", 500*nCol, 500*nRow);
    c_mean->Divide(nCol,nRow);
    c_mean->SetRightMargin(0.10);
    for (int i=0; i<nCentBins; ++i){
        c_mean->cd(i+1);
        h_mean_gaus[i][j]->SetMarkerColor(2);
        h_mean_gaus[i][j]->SetMarkerStyle(25);
        h_mean_gaus[i][j]->GetYaxis()->SetRangeUser(0.8,1.1);
        h_mean_gaus[i][j]->Draw("p");
        // h_mean[i][j]->SetMarkerColor(8);
        // h_mean[i][j]->SetMarkerStyle(24);
        // h_mean[i][j]->Draw("p same");

        // f_mean[i][j]->SetLineColor(8);
        // f_mean[i][j]->DrawCopy("same");
        f_mean_gaus[i][j]->SetLineColor(2);
        f_mean_gaus[i][j]->DrawCopy("same");
        jumSun(ptBins[0],1,ptBins[nPtBins],1);
        float c = f_mean_gaus[i][j]->GetParameter(0);
        float n = f_mean_gaus[i][j]->GetParameter(1);
        float s = f_mean_gaus[i][j]->GetParameter(2);
        drawText(Form("%.2f + #frac{%.2f}{#sqrt{p_{T}}} + #frac{%.2f}{p_{T}}",c,n,s),0.4,0.3);
    }
    TLegend* l1 = new TLegend(0.5,0.7,0.9,0.9); 
    legStyle(l1);
    // l1->AddEntry(h_mean[0][0],"Mean","p");
    // l1->AddEntry(f_mean[0][0],"Fit Mean","l");
    l1->AddEntry(h_mean_gaus[0][0],Form("Mean(%s)",fnc_name.Data()),"p");
    l1->AddEntry(f_mean_gaus[0][0],Form("Fit Mean(%s)",fnc_name.Data()),"l");
    l1->Draw("same");

    TString st_cnsFun = "C + #frac{N}{#sqrt{p_{T}}} + #frac{S}{p_{T}}";
    c_mean->SaveAs(Form("figures/photonEnergyCorrection_1D_mean_vs_recoPt_%s.pdf",cap.Data())); 

    // resolution(gen/reco pT) vs. Reco pT
    TCanvas* c_res = new TCanvas("c_res", "", 500*nCol, 500*nRow);
    c_res->Divide(nCol,nRow);
    c_res->SetRightMargin(0.10);
    for (int i=0; i<nCentBins; ++i) {
        c_res->cd(i+1);
        h_sig_gaus[i][j]->SetMarkerColor(2);
        h_sig_gaus[i][j]->SetMarkerStyle(25);
        h_sig_gaus[i][j]->GetYaxis()->SetRangeUser(0.0,0.1);
        h_sig_gaus[i][j]->Draw("p");
        // h_rms[i][j]->SetMarkerColor(8);
        // h_rms[i][j]->SetMarkerStyle(24);
        // h_rms[i][j]->Draw("p same");

        // f_rms[i][j]->SetLineColor(8);
        // f_rms[i][j]->DrawCopy("same");
        f_sig_gaus[i][j]->SetLineColor(2);
        f_sig_gaus[i][j]->DrawCopy("same");

        float c = f_sig_gaus[i][j]->GetParameter(0);
        float n = f_sig_gaus[i][j]->GetParameter(1);
        float s = f_sig_gaus[i][j]->GetParameter(2);
        drawText(Form("%.2f + #frac{%.2f}{#sqrt{p_{T}}} + #frac{%.2f}{p_{T}}",c,n,s),0.4,0.55);
    }
    TLegend* l2 = new TLegend(0.5,0.7,0.9,0.9); 
    legStyle(l2);
    // l2->AddEntry(h_rms[0][0],"RMS","p");
    // l2->AddEntry(f_rms[0][0],"Fit RMS","l");
    l2->AddEntry(h_sig_gaus[0][0],Form("#sigma(%s)",fnc_name.Data()),"p");
    l2->AddEntry(f_sig_gaus[0][0],Form("Fit #sigma(%s)",fnc_name.Data()),"l");
    l2->Draw("same");
    c_res->SaveAs(Form("figures/photonEnergyCorrection_1D_resolution_vs_recoPt_%s.pdf",cap.Data())); 


#if 0
    ///////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////
    // Afer scale, obtain resolution functions

    TH1F* h_ratio_afterCorr[nCentBins][nEtaBins][nPtBins];
    TH1F* h_ratio_pull_afterCorr[nCentBins][nEtaBins][nPtBins];
    TH1F* h_mean_gaus_afterCorr[nCentBins][nEtaBins];
    TH1F* h_sig_gaus_afterCorr[nCentBins][nEtaBins];
    TH1F* h_mean_afterCorr[nCentBins][nEtaBins];
    TH1F* h_rms_afterCorr[nCentBins][nEtaBins];

    for (int i=0; i<nCentBins; ++i) {
        for (int j=0; j<nEtaBins; ++j) {
            TString centSt = Form("%i-%i%%", centBins[0][i]/2, centBins[1][i]/2);
            if(isPP) centSt = "pp";
            h_mean_gaus_afterCorr[i][j] = new TH1F(Form("h_mean_gaus_afterCorr_%i_%i", i, j), Form("%s, %.2f<|#eta|<%.2f;gen p_{T}^{#gamma};<#frac{reco p_{T}^{#gamma}}{gen p_{T}^{#gamma}}>", centSt.Data(), etaBins[0][j], etaBins[1][j]), nPtBins, ptBins);
            h_sig_gaus_afterCorr[i][j] = new TH1F(Form("h_sigma_gaus_afterCorr_%i_%i", i, j), Form("%s, %.2f<|#eta|<%.2f;gen p_{T}^{#gamma};#sigma(#frac{reco p_{T}^{#gamma}}{gen p_{T}^{#gamma}})", centSt.Data(), etaBins[0][j], etaBins[1][j]), nPtBins, ptBins);
            h_mean_afterCorr[i][j] = (TH1F*)  h_mean_gaus[i][j]->Clone(Form("h_mean_afterCorr_%i_%i", i, j));
            h_rms_afterCorr[i][j] = (TH1F*)  h_sig_gaus[i][j]->Clone(Form("h_rms_afterCorr_%i_%i", i, j));
            for (int k=0; k<nPtBins; ++k) {
                h_ratio_afterCorr[i][j][k] = new TH1F(Form("h_ratio_afterCorr_cent%i_eta%i_pt%i", i, j, k), ";#frac{reco p_{T}^{#gamma}}{gen p_{T}^{#gamma}};", nbins,0.0,2.0);
            }
        }
    }

    //uint64_t nentries = t_photon->GetEntries();
    for (uint64_t i=0; i<nentries; ++i) {
        if (i % 100000 == 0)
            printf("entry: %lu / %lu \n", i,nentries);

        t_photon->GetEntry(i);
        t_event->GetEntry(i);

        for (uint32_t j=0; j<phoEt->size(); ++j) {
            if (phoEt->at(j) < 10)
                continue;
            if (fabs(phoEta->at(j)) > 2.4)
                continue;
            if (pho_genMatchedIndex->at(j) < 0)
                continue;
            if (pho_swissCrx->at(j) > 0.9)
                continue;
            if (fabs(pho_seedTime->at(j)) > 3)
                continue;
            if (doIsoCut && phoHoverE->at(j)>0.1) 
                continue;
            if (doIsoCut && phoSigmaIEtaIEta_2012->at(j)>0.011) 
                continue;
            if (doIsoCut && (pho_ecalClusterIsoR4->at(j)+pho_hcalRechitIsoR4->at(j)+pho_trackIsoR4PtCut20->at(j))>5) 
                continue;

            if(!isPhotonSample){
                int matchedEleIndex = -1;
                float matchedEoverP = 100;
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
                if (matchedEleIndex == -1)
                    continue;
            }

            int icent = 0;
            if(!isPP){
                for (; hiBin>=centBins[1][icent] && icent<nCentBins; ++icent);
            }

            int32_t mcindex = pho_genMatchedIndex->at(j);

            if (mcEt->at(mcindex) < 10)
                continue;
            if (mcCalIsoDR04->at(mcindex) > 5)
                continue;

            int ieta = 0;
            for (; fabs(phoEta->at(j))>=etaBins[1][ieta] && ieta<nEtaBins; ++ieta);

            // according to the reco pt
            // int ipt = 0;
            // for (; fabs(phoEt->at(j))>=ptBins[ipt+1] && ipt<nPtBins; ++ipt){
            //     if(ipt>nPtBins) break;
            // }

            // according to the gen pt
            int ipt = 0;
            for (; fabs(mcEt->at(mcindex))>=ptBins[ipt+1] && ipt<nPtBins; ++ipt){
                if(ipt>nPtBins) break;
            }

            float corrFac =f_mean_gaus[icent][ieta]->Eval(phoEt->at(j));
            //cout << " photon number : " << j << " / " << phoEt->size() << " ::: photon Et = " << phoEt->at(j) <<", mcEt = " << mcEt->at(mcindex) << "::: ipt = " << ipt << ", nPtBins = "<< nPtBins << ", ieta = " << ieta << ", icent = " << icent<< ", correction Factor = " << corrFac << "ratio = " << (phoEt->at(j)/corrFac)/mcEt->at(mcindex) << endl;

            if(ipt<nPtBins) h_ratio_afterCorr[icent][ieta][ipt]->Fill((phoEt->at(j)/corrFac)/mcEt->at(mcindex));
        }
    }


    TCanvas* c_ss = new TCanvas("sss","",250,250);
    // h_ratio_afterCorr[0][0][0]->Draw();
    // c_ss->SaveAs("figures/test_firstbin.pdf");

    //////////////////////////////////////////////////////////////////
    // DONE filling h_ratio, now fit reco/gen ratio with gaus or DSCB 
    TCanvas* c_afterCorr[nCentBins][nEtaBins];
    TF1* ff_afterCorr[nCentBins][nEtaBins][nPtBins];
    for (int i=0; i<nCentBins; ++i) {
        for (int j=0; j<nEtaBins; ++j) {
            c_afterCorr[i][j] = new TCanvas(Form("can_afterCorr_cent%d_eta%d",i,j),"",250*4,250*3);
            c_afterCorr[i][j]->Divide(5,4);
            for (int k=0; k<nPtBins; ++k) {
                ctemp->cd();
                //h_ratio_afterCorr[i][j][k]->Scale(1./h_ratio_afterCorr[i][j][k]->GetMaximum());
                // gaussian or Double-sided crystal ball fitting
                if(fnc_name=="gaus"){
                    ff_afterCorr[i][j][k] = cleverGaus(h_ratio_afterCorr[i][j][k], Form("f1_%s_afterCorr",h_ratio_afterCorr[i][j][k]->GetName()));
                } else if(fnc_name=="DSCB"){
                    Int_t peakBin  = h_ratio_afterCorr[i][j][k]->GetMaximumBin();
                    Double_t peakContent  = h_ratio_afterCorr[i][j][k]->GetBinContent(peakBin);
                    Double_t peak =  h_ratio_afterCorr[i][j][k]->GetBinCenter(peakBin);
                    Double_t sigma = h_ratio_afterCorr[i][j][k]->GetRMS();
                    ff_afterCorr[i][j][k] = new TF1(Form("f1_%d_%d_%d_afterCorr",i,j,k), fnc_DSCB, peak-2.5*sigma, peak+2.5*sigma, 7);
                    if(collSt == "pbpb"){
                        if(i==0 && j==0 && k==0)
                            ff_afterCorr[i][j][k]->SetParameters(peakContent,peak,sigma,0.79,3.63,0.59,1.67);
                        else if(i==0 && j==0 && k==2)
                            ff_afterCorr[i][j][k]->SetParameters(peakContent,peak,sigma,1.37638e+00,5.69004e+00,6.33186e-01,1.29222e+02);
                        else if(i==1 && j==0 && k==6)
                            ff_afterCorr[i][j][k]->SetParameters(peakContent,peak,sigma,0.79,3.63,0.79,3.63);
                        else if(i==2 && j==0 && k==2)
                            ff_afterCorr[i][j][k]->SetParameters(peakContent,1.00215e+00,-1.43638e-02,9.16937e-01,1.18086e+01,8.16771e-01,4.69242e+00);
                        else if(i==3 && j==0 && (k==1 || k==4))
                            ff_afterCorr[i][j][k]->SetParameters(peakContent,peak,sigma,0.79,3.63,0.59,1.67);
                        else if(i==3 && j==0 && (k==2 || k==5))
                            ff_afterCorr[i][j][k]->SetParameters(peakContent,peak,sigma,0.79,3.63,0.59,1.67);
                        else if(i==3 && j==0 && (k==6 || k==9))
                            ff_afterCorr[i][j][k]->SetParameters(peakContent,peak,sigma,4.09805e-01,4.31433e+00,4.09805e-01,4.31433e+00);
                        else if(i==4 && j==0 && (k==0))
                            ff_afterCorr[i][j][k]->SetParameters(peakContent,peak,1.57862e-02,4.09805e-01,4.31433e+00,1.43539e+00,3.52302e+00);
                        else if(i==4 && j==0 && (k==1))
                            ff_afterCorr[i][j][k]->SetParameters(peakContent,9.95122e-01,-1.28811e-02,2.33030e+00,2.73892e+00,7.29685e-01,4.33112e+00);
                        else if(i==4 && j==0 && (k==3 || k==5))
                            ff_afterCorr[i][j][k]->SetParameters(peakContent,peak,sigma,4.09805e-01,4.31433e+00,4.09805e-01,4.31433e+00);
                        else
                            ff_afterCorr[i][j][k]->SetParameters(peakContent,peak,sigma,0.79,3.63,0.59,1.67);


                    } else{ // pp
                        ff_afterCorr[i][j][k]->SetParameters(peakContent,peak,sigma/3.,6.33186e-01,1.29222e+02,1.37638e+00,5.69004e+00);
                        if(i==0 && j==0 && (k<=1))
                            ff_afterCorr[i][j][k]->SetParameters(peakContent,peak,sigma/3.,6.33186e-01,1.29222e+02,1.37638e+00,5.69004e+00);
                        else if(i==0 && j==0 && (k==2))
                            ff_afterCorr[i][j][k]->SetParameters(peakContent,peak,sigma/5.,9.16937e-01,1.18086e+01,8.16771e-01,4.69242e+00);
                        else if(i==0 && j==0 && (k==4 || k==5 || k==6))
                            ff_afterCorr[i][j][k]->SetParameters(peakContent,peak,sigma/5.,9.16937e-01,1.18086e+01,8.16771e-01,4.69242e+00);
                        else if(i==0 && j==0 && (k==8))
                            ff_afterCorr[i][j][k]->SetParameters(peakContent,peak,sigma/3.,6.33186e-01,1.29222e+02,1.37638e+00,5.69004e+00);
                        else if(i==0 && j==0 && (k==9 || k==10))
                            ff_afterCorr[i][j][k]->SetParameters(peakContent,peak,sigma/5.,2.16937e-01,1.18086e+01,2.16771e-01,4.69242e+00);
                        else if(i==0 && j==0 && (k==11))
                            ff_afterCorr[i][j][k]->SetParameters(peakContent,peak,sigma/3.,6.33186e-01,1.29222e+02,1.37638e+00,5.69004e+00);
                    }
                    ff_afterCorr[i][j][k]->SetParLimits(0,peakContent*0.9,peakContent*1.1);
                    ff_afterCorr[i][j][k]->SetParLimits(2,0.005,0.06);
                    h_ratio_afterCorr[i][j][k]->Fit(ff_afterCorr[i][j][k],"LL M O Q R");
                    h_ratio_afterCorr[i][j][k]->Fit(ff_afterCorr[i][j][k],"LL M O Q R");
                    h_ratio_afterCorr[i][j][k]->Fit(ff_afterCorr[i][j][k],"LL M O Q R");
                   // h_ratio_afterCorr[i][j][k]->Fit(ff_afterCorr[i][j][k],"LL M O Q R");
                   // h_ratio_afterCorr[i][j][k]->Fit(ff_afterCorr[i][j][k],"LL M O Q R");

                } else if(fnc_name=="CB"){
                    Int_t peakBin  = h_ratio_afterCorr[i][j][k]->GetMaximumBin();
                    Double_t peakContent  = h_ratio_afterCorr[i][j][k]->GetBinContent(peakBin);
                    Double_t peak =  h_ratio_afterCorr[i][j][k]->GetBinCenter(peakBin);
                    Double_t sigma = h_ratio_afterCorr[i][j][k]->GetRMS();
                    ff_afterCorr[i][j][k] = new TF1(Form("f1_afterCorr_%d_%d_%d",i,j,k), Form("ROOT::Math::crystalball_pd(x, 3, 1.01, %f, %f)",sigma,peak), peak-2*sigma, peak+2*sigma);
                    h_ratio_afterCorr[i][j][k]->Fit(ff_afterCorr[i][j][k],"LL M O Q");
                    h_ratio_afterCorr[i][j][k]->Fit(ff_afterCorr[i][j][k],"LL M O Q");
                }

                //closure test
                if(i==0 && j==0 && k==0) {
                    c_ss->cd();
                    h_ratio_afterCorr[0][0][0]->Draw();
                    c_ss->SaveAs("figures/test_firstbin.pdf");
                }

                double mean = ff_afterCorr[i][j][k]->GetParameter(1);
                double mean_err = ff_afterCorr[i][j][k]->GetParError(1);
                double resol = abs(ff_afterCorr[i][j][k]->GetParameter(2));
                double resol_err = abs(ff_afterCorr[i][j][k]->GetParError(2));
                h_mean_gaus_afterCorr[i][j]->SetBinContent(k+1, mean);
                h_mean_gaus_afterCorr[i][j]->SetBinError(k+1, mean_err);
                h_sig_gaus_afterCorr[i][j]->SetBinContent(k+1, resol);
                h_sig_gaus_afterCorr[i][j]->SetBinError(k+1, resol_err);

                mean = h_ratio_afterCorr[i][j][k]->GetMean();
                mean_err = h_ratio_afterCorr[i][j][k]->GetMeanError();
                resol = h_ratio_afterCorr[i][j][k]->GetRMS();
                resol_err = h_ratio_afterCorr[i][j][k]->GetRMSError();
                h_mean_afterCorr[i][j]->SetBinContent(k+1, mean);
                h_mean_afterCorr[i][j]->SetBinError(k+1, mean_err);
                h_rms_afterCorr[i][j]->SetBinContent(k+1, resol);
                h_rms_afterCorr[i][j]->SetBinError(k+1, resol_err);

                c_afterCorr[i][j]->cd(k+1);
                h_ratio_afterCorr[i][j][k]->GetXaxis()->SetRangeUser(0.7,1.3);
                h_ratio_afterCorr[i][j][k]->Draw();
                //text in the ratio plots 
                TString centSt = Form("%i-%i%%", centBins[0][i]/2, centBins[1][i]/2);
                if(isPP) centSt = "pp";
                float xpos = 0.21;
                float dy = 0.09;
                drawText(Form("%s, %.2f<|#eta|<%.2f", centSt.Data(), etaBins[0][j], etaBins[1][j]),xpos,0.84);
                drawText(Form("%d<p_{T}^{gen #gamma}<%d GeV", (int)ptBins[k],(int)ptBins[k+1]),xpos,0.84-dy);
                drawText(Form("%s fitting", fnc_name.Data()),xpos,0.84-2*dy);
                drawText("After energy correction",xpos,0.84-3*dy);
                drawText(Form("%s",collSt.Data()),xpos,0.84-4*dy);
            }
            c_afterCorr[i][j]->SaveAs(Form("figures/photonEnergyCorrection_afterCorrection_fitting_of_recoPt_over_genPt_inPtbins_%s_cent%d_eta%d.pdf",cap.Data(),i,j));
        }
    }

    ////////////////////////////////////////////////////////
    // pull distribution
    TCanvas* c_pull_afterCorr[nCentBins][nEtaBins];
    for (int i=0; i<nCentBins; ++i) {
        for (int j=0; j<nEtaBins; ++j) {
            c_pull_afterCorr[i][j] = new TCanvas(Form("can_pull_afterCorr_cent%d_eta%d",i,j),"",250*4,250*3);
            c_pull_afterCorr[i][j]->Divide(5,4);
            for (int k=0; k<nPtBins; ++k) {
                h_ratio_pull_afterCorr[i][j][k] = getPullHist(h_ratio_afterCorr[i][j][k], ff_afterCorr[i][j][k]);
                h_ratio_pull_afterCorr[i][j][k]->SetTitle(";#frac{reco p_{T}^{#gamma}}{gen p_{T}^{#gamma}};Pull");
                h_ratio_pull_afterCorr[i][j][k]->GetYaxis()->SetRangeUser(-5,5);
                h_ratio_pull_afterCorr[i][j][k]->GetXaxis()->SetRangeUser(0.7,1.3);
                c_pull_afterCorr[i][j]->cd(k+1);
                h_ratio_pull_afterCorr[i][j][k]->Draw("p");
                jumSun(0.7,0,1.3,0);
                TString centSt = Form("%i-%i%%", centBins[0][i]/2, centBins[1][i]/2);
                if(isPP) centSt = "pp";
                float xpos = 0.21;
                float dy = 0.09;
                drawText(Form("%s, %.2f<|#eta|<%.2f", centSt.Data(), etaBins[0][j], etaBins[1][j]),xpos,0.84);
                drawText(Form("%d<p_{T}^{gen #gamma}<%d GeV", (int)ptBins[k],(int)ptBins[k+1]),xpos,0.84-dy);
                drawText(Form("%s fitting", fnc_name.Data()),xpos,0.84-2*dy);
                drawText("After energy correction",xpos,0.84-3*dy);
                drawText(Form("%s",collSt.Data()),xpos,0.84-4*dy);
            }
            c_pull_afterCorr[i][j]->SaveAs(Form("figures/photonEnergyCorrection_afterCorrection_PullDist_fitting_of_recoPt_over_genPt_inPtbins_%s_cent%d_eta%d.pdf",cap.Data(),i,j));
        }
    }

    ////////////////////////////////////////////////////////
    // mean of reco/gen pt as a function of gen pT  
    cout << "mean fitting after correction... " << endl;
    TF1 *f_mean_afterCorr[nCentBins][nEtaBins];
    TF1 *f_mean_gaus_afterCorr[nCentBins][nEtaBins];
    for (int i=0; i<nCentBins; ++i) {
        for (int j=0; j<nEtaBins; ++j) {
            f_mean_gaus_afterCorr[i][j] = new TF1(Form("f_mean_afterCorr_gaus_cent%d_eta%d",i,j), myFunc, 0, 300, 3);
            f_mean_gaus_afterCorr[i][j] -> SetParameters(0.03, 0.8, 0.01);
            f_mean_gaus_afterCorr[i][j] -> SetParNames("C", "S", "N");
            h_mean_gaus_afterCorr[i][j] -> Fit(f_mean_gaus_afterCorr[i][j], "RLL Q");
            h_mean_gaus_afterCorr[i][j] -> Fit(f_mean_gaus_afterCorr[i][j], "RLL Q");
            h_mean_gaus_afterCorr[i][j] -> Fit(f_mean_gaus_afterCorr[i][j], "RLL Q");

            f_mean_afterCorr[i][j] = new TF1(Form("f_mean_afterCorr_cent%d_eta%d",i,j), myFunc, 0, 300, 3);
            f_mean_afterCorr[i][j] -> SetParameters(0.03, 0.8, 0.01);
            f_mean_afterCorr[i][j] -> SetParNames("C", "S", "N");
            h_mean_afterCorr[i][j] -> Fit(f_mean_afterCorr[i][j], "RLL Q");
            h_mean_afterCorr[i][j] -> Fit(f_mean_afterCorr[i][j], "RLL Q");
        }
    }

    ////////////////////////////////////////////////////////
    // resoltuion of reco/gen pt as a function of gen pT  
    cout << "resolution fitting after correction... " << endl;
    TF1 *f_rms_afterCorr[nCentBins][nEtaBins];
    TF1 *f_sig_gaus_afterCorr[nCentBins][nEtaBins];
    for (int i=0; i<nCentBins; ++i) {
        for (int j=0; j<nEtaBins; ++j) {
            f_sig_gaus_afterCorr[i][j] = new TF1(Form("f_sig_gaus_afterCorr_cent%d_eta%d",i,j), myFunc, 0, 300, 3);
            f_sig_gaus_afterCorr[i][j] -> SetParameters(0.03, 0.8, 0.01);
            f_sig_gaus_afterCorr[i][j] -> SetParNames("C", "S", "N");
            h_sig_gaus_afterCorr[i][j] -> Fit(f_sig_gaus_afterCorr[i][j], "RLL Q");
            h_sig_gaus_afterCorr[i][j] -> Fit(f_sig_gaus_afterCorr[i][j], "RLL Q");
            h_sig_gaus_afterCorr[i][j] -> Fit(f_sig_gaus_afterCorr[i][j], "RLL Q");
            h_sig_gaus_afterCorr[i][j] -> Fit(f_sig_gaus_afterCorr[i][j], "RLL Q");
            f_rms_afterCorr[i][j] = new TF1(Form("f_rms_afterCorr_cent%d_eta%d",i,j), myFunc, 0, 300, 3);
            f_rms_afterCorr[i][j] -> SetParameters(0.03, 0.8, 0.01);
            f_rms_afterCorr[i][j] -> SetParNames("C", "S", "N");
            h_rms_afterCorr[i][j] -> Fit(f_rms_afterCorr[i][j], "RLL Q");
            h_rms_afterCorr[i][j] -> Fit(f_rms_afterCorr[i][j], "RLL Q");
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////
    //Draw
    TCanvas* c_mean_afterCorr = new TCanvas("c_mean_afterCorr", "", 500*nCol, 500*nRow);
    c_mean_afterCorr->Divide(nCol,nRow);
    c_mean_afterCorr->SetRightMargin(0.10);
    for (int i=0; i<nCentBins; ++i){
        c_mean_afterCorr->cd(i+1);
        h_mean_gaus_afterCorr[i][j]->SetMarkerColor(2);
        h_mean_gaus_afterCorr[i][j]->SetMarkerStyle(25);
        h_mean_gaus_afterCorr[i][j]->GetYaxis()->SetRangeUser(0.8,1.1);
        h_mean_gaus_afterCorr[i][j]->Draw("p");
        f_mean_gaus_afterCorr[i][j]->SetLineColor(2);
        f_mean_gaus_afterCorr[i][j]->DrawCopy("same");

        // h_mean_afterCorr[i][j]->SetMarkerColor(8);
        // h_mean_afterCorr[i][j]->SetMarkerStyle(24);
        // h_mean_afterCorr[i][j]->GetYaxis()->SetRangeUser(0.8,1.1);
        // h_mean_afterCorr[i][j]->Draw("p");
        // f_mean_afterCorr[i][j]->SetLineColor(8);
        // f_mean_afterCorr[i][j]->DrawCopy("same");
        jumSun(ptBins[0],1,ptBins[nPtBins],1);

        float xpos = 0.21;
        float ypos = 0.60;
        float dy = 0.06;
        drawText(Form("%s fitting", fnc_name.Data()),xpos,ypos-2*dy);
        drawText("After energy correction",xpos,ypos-3*dy);
        drawText(Form("%s",collSt.Data()),xpos,ypos-4*dy);

        float c = f_mean_gaus_afterCorr[i][j]->GetParameter(0);
        float n = f_mean_gaus_afterCorr[i][j]->GetParameter(1);
        float s = f_mean_gaus_afterCorr[i][j]->GetParameter(2);
        drawText(Form("%.2f + #frac{%.2f}{#sqrt{p_{T}}} + #frac{%.2f}{p_{T}}",c,n,s),xpos,ypos-5*dy);
    }
    TLegend* l1_afterCorr = new TLegend(0.5,0.7,0.9,0.9);
    legStyle(l1_afterCorr);
    // l1_afterCorr->AddEntry(h_mean_afterCorr[0][0],"Mean","p");
    // l1_afterCorr->AddEntry(f_mean_afterCorr[0][0],"Fit Mean","l");
    // l1_afterCorr->AddEntry(h_mean_afterCorr[0][0],Form("Mean%s",""),"p");
    // l1_afterCorr->AddEntry(f_mean_afterCorr[0][0],Form("Fit Mean%s",""),"l");
    l1_afterCorr->AddEntry(h_mean_gaus_afterCorr[0][0],Form("Mean(%s)",fnc_name.Data()),"p");
    l1_afterCorr->AddEntry(f_mean_gaus_afterCorr[0][0],Form("Fit Mean(%s)",fnc_name.Data()),"l");
    l1_afterCorr->Draw("same");
    c_mean_afterCorr->SaveAs(Form("figures/photonEnergyCorrection_afterCorrection_1D_mean_vs_genPt_%s.pdf",cap.Data()));

    TCanvas* c_res_afterCorr = new TCanvas("c_res_afterCorr", "", 500*nCol, 500*nRow);
    c_res_afterCorr->Divide(nCol,nRow);
    c_res_afterCorr->SetRightMargin(0.10);
    for (int i=0; i<nCentBins; ++i) {
        c_res_afterCorr->cd(i+1);
        h_sig_gaus_afterCorr[i][j]->SetMarkerColor(2);
        h_sig_gaus_afterCorr[i][j]->SetMarkerStyle(25);
        h_sig_gaus_afterCorr[i][j]->GetYaxis()->SetRangeUser(0.0,0.1);
        h_sig_gaus_afterCorr[i][j]->Draw("p");
        f_sig_gaus_afterCorr[i][j]->SetLineColor(2);
        f_sig_gaus_afterCorr[i][j]->DrawCopy("same");

        // h_rms_afterCorr[i][j]->SetMarkerColor(8);
        // h_rms_afterCorr[i][j]->SetMarkerStyle(24);
        // f_rms_afterCorr[i][j]->SetLineColor(8);
        // f_rms_afterCorr[i][j]->DrawCopy("same");

        float xpos = 0.21;
        float ypos = 0.84;
        float dy = 0.06;
        drawText(Form("%s fitting", fnc_name.Data()),xpos,ypos-2*dy);
        drawText("After energy correction",xpos,ypos-3*dy);
        drawText(Form("%s",collSt.Data()),xpos,ypos-4*dy);

        float c = f_sig_gaus_afterCorr[i][j]->GetParameter(0);
        float n = f_sig_gaus_afterCorr[i][j]->GetParameter(1);
        float s = f_sig_gaus_afterCorr[i][j]->GetParameter(2);
        drawText(Form("%.2f + #frac{%.2f}{#sqrt{p_{T}}} + #frac{%.2f}{p_{T}}",c,n,s),xpos,ypos-5*dy);
    }
    TLegend* l2_afterCorr = new TLegend(0.5,0.7,0.9,0.9);
    legStyle(l2_afterCorr);
    // l2_afterCorr->AddEntry(h_rms[0][0],"RMS","p");
    // l2_afterCorr->AddEntry(f_rms[0][0],"Fit RMS","l");
    l2_afterCorr->AddEntry(h_sig_gaus[0][0],Form("#sigma(%s)",fnc_name.Data()),"p");
    l2_afterCorr->AddEntry(f_sig_gaus[0][0],Form("Fit #sigma(%s)",fnc_name.Data()),"l");
    l2_afterCorr->Draw("same");
    c_res_afterCorr->SaveAs(Form("figures/photonEnergyCorrection_afterCorrection_1D_resolution_vs_genPt_%s.pdf",cap.Data()));

#endif






    /////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    // Save 
    TFile* energyCorrectionFile = TFile::Open(Form("%s",outfname.Data()), "recreate");
    c_2d->Write("", TObject::kOverwrite);
    c_mat->Write("", TObject::kOverwrite);
    c_mean->Write("", TObject::kOverwrite);
    c_res->Write("", TObject::kOverwrite);
   // c_mean_afterCorr->Write("", TObject::kOverwrite);
   // c_res_afterCorr->Write("", TObject::kOverwrite);
    for (int i=0; i<nCentBins; ++i) {
        for (int j=0; j<nEtaBins; ++j) {
            h_pt[i][j]->Write(Form("h_matrix_cent%i_eta%i", i, j), TObject::kOverwrite);
            h_ptratio[i][j]->Write(Form("h_ptratio_cent%i_eta%i", i, j), TObject::kOverwrite);

            f_rms[i][j]->Write();
            f_sig_gaus[i][j]->Write();
            f_mean[i][j]->Write();
            f_mean_gaus[i][j]->Write();
            h_mean_gaus[i][j]->Write(Form("photonEnergyCorr_mean_gaus_cent%i_eta%i", i, j), TObject::kOverwrite);
            h_sig_gaus[i][j]->Write(Form("photonEnergyCorr_sigma_gaus_cent%i_eta%i", i, j), TObject::kOverwrite);
            h_mean[i][j]->Write(Form("photonEnergyCorr_mean_cent%i_eta%i", i, j), TObject::kOverwrite);
            h_rms[i][j]->Write(Form("photonEnergyCorr_rms_cent%i_eta%i", i, j), TObject::kOverwrite);
            c[i][j]->Write(Form("ratio_cent%i_eta%i", i, j), TObject::kOverwrite);

           // f_rms_afterCorr[i][j]->Write();
           // f_sig_gaus_afterCorr[i][j]->Write();
           // f_mean_afterCorr[i][j]->Write();
           // f_mean_gaus_afterCorr[i][j]->Write();
           // h_mean_gaus_afterCorr[i][j]->Write(Form("photonEnergyCorr_mean_gaus_afterCorr_cent%i_eta%i", i, j), TObject::kOverwrite);
           // h_sig_gaus_afterCorr[i][j]->Write(Form("photonEnergyCorr_sigma_gaus_afterCorr_cent%i_eta%i", i, j), TObject::kOverwrite);
           // h_mean_afterCorr[i][j]->Write(Form("photonEnergyCorr_mean_afterCorr_cent%i_eta%i", i, j), TObject::kOverwrite);
           // h_rms_afterCorr[i][j]->Write(Form("photonEnergyCorr_rms_afterCorr_cent%i_eta%i", i, j), TObject::kOverwrite);
           // c_afterCorr[i][j]->Write(Form("ratio_afterCorr_cent%i_eta%i", i, j), TObject::kOverwrite);
            for (int k=0; k<nPtBins; ++k) {
                h_ratio[i][j][k]->Write();
                h_ratio_pull[i][j][k]->Write();
               // h_ratio_afterCorr[i][j][k]->Write();
               // h_ratio_pull_afterCorr[i][j][k]->Write();
            }
        }
    }
    energyCorrectionFile->Close();
}


#define _PI 3.1415926535897932384
float dR(float eta1, float eta2, float phi1, float phi2) {
    float deta = eta1 - eta2;
    float dphi = fabs(phi1 - phi2);
    dphi = (dphi > _PI) ? 2 * _PI - dphi : dphi;
    return sqrt(deta * deta + dphi * dphi);
}

TH1F* getPullHist(TH1F *h1, TF1 *f){
    Int_t nBins = h1->GetNbinsX();
    Double_t xMin = h1->GetBinLowEdge(1);
    Double_t xMax = h1->GetBinLowEdge(nBins)+h1->GetBinWidth(nBins);
    TH1F* hp = new TH1F(Form("%s_pull",h1->GetName()), ";#frac{reco p_{T}^{#gamma}}{gen p_{T}^{#gamma}};Pull", nBins,xMin,xMax);
    for(Int_t ix=1; ix<=nBins; ++ix){
        float data = h1->GetBinContent(ix);
        float data_err = h1->GetBinError(ix);
        float data_pos = h1->GetBinLowEdge(ix)+(h1->GetBinWidth(ix))/2.;
        float fit = f->Eval(data_pos);
        float pull = (data-fit)/data_err;
        if(data==0) pull = 0;
        hp->SetBinContent(ix+1,pull);
        hp->SetBinError(ix+1,0);
        // cout << "pull hist bin " << ix << " value = " << hp->GetBinContent(ix+1) << endl;
    }
    return hp;
}
