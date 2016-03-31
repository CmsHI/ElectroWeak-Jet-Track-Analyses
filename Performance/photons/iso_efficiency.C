#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TGraphAsymmErrors.h>
#include <TMath.h>
#include <TCanvas.h>

#include "../../TreeHeaders/ggHiNtuplizerPhotonSkim.h"
//#include "../Utilities/interface/CutConfigurationParser.h"

void iso_efficiency()
{
  TFile *inFile = TFile::Open("~/Downloads/PbPb_AllQCDPhoton_eta3.phoCorrectedMerged.root");
  TTree *inTree = (TTree*)inFile->Get("photonSkimTree");

  PhotonSkim inPho(true); //montecarlo = true
  inPho.setupTreeForReading(inTree);

  const int maxSumIso = 200;
  const int minSumIso = -50;
  const int nBins = maxSumIso - minSumIso;

  TH1D *eff_vs_sumIso_num = new TH1D("eff_vs_sumIso_num",";sumIso;count",nBins,minSumIso,maxSumIso);
  TH1D *sideband_vs_sumIso_num = new TH1D("sideband_vs_sumIso_num",";sumIso;sideband count",nBins,minSumIso,maxSumIso);
  TH1D *eff_vs_sumIso_denom = new TH1D("eff_vs_sumIso_denom",";sumIso;total events",nBins,minSumIso,maxSumIso);
  
  Long64_t nentries = inTree->GetEntries();
  for(int ientries = 0; ientries < nentries; ++ientries)
  {
    inTree->GetEntry(ientries);
    inPho.resolvePointers();

    if (inPho.phoEt[0] < 50 ) continue; // only high pt photons
    if (inPho.nMC == 0) continue; // nothing is gen matched
    if (inPho.mcPID[0] != 22) continue; // ignore electrons for now
    if (inPho.phoHoverE[0] > 0.1) continue;
    if (TMath::Abs(inPho.phoEta[0]) < 1.44) continue;
    bool signal = (TMath::Abs(inPho.mcMomPID[0])<=22 || inPho.mcMomPID[0] == -999) && inPho.mcCalIsoDR04[0]<5;
    bool bkg = (TMath::Abs(inPho.mcMomPID[0])>22 && inPho.mcMomPID[0] != -999);
    
    float sumIso = inPho.pho_ecalClusterIsoR4[0] + inPho.pho_hcalRechitIsoR4[0] + inPho.pho_trackIsoR4PtCut20[0];

    eff_vs_sumIso_denom->Fill(sumIso);
    if(signal){
      eff_vs_sumIso_num->Fill(sumIso);
    } else if (bkg) {
      sideband_vs_sumIso_num->Fill(sumIso);
    }
  }

  TGraphAsymmErrors *eff = new TGraphAsymmErrors();
  eff->BayesDivide(eff_vs_sumIso_num,eff_vs_sumIso_denom);
  eff->SetName("eff");
  TGraphAsymmErrors *sideband = new TGraphAsymmErrors();
  sideband->BayesDivide(sideband_vs_sumIso_num,eff_vs_sumIso_denom);
  sideband->SetName("sideband");

  TH1D *hEmpty = new TH1D("hEmpty",Form(";sumIso (GeV);Efficiency"),nBins,minSumIso,maxSumIso);
  hEmpty->SetMaximum(1.2);
  hEmpty->SetMinimum(0);
  
  TCanvas *c0 = new TCanvas();
  c0->SetLogy();
  eff_vs_sumIso_num->Draw("p");
  TCanvas *c01 = new TCanvas();
  sideband_vs_sumIso_num->Draw("p");
  c01->SetLogy();
  TCanvas *c02 = new TCanvas();
  eff_vs_sumIso_denom->Draw("p");
  c02->SetLogy();
  TCanvas *c1 = new TCanvas();
  hEmpty->Draw();
  eff->Draw("p e");
  TCanvas *c2 = new TCanvas();
  hEmpty->Draw();
  sideband->Draw("p e");

  c0->SaveAs("figures/signal_endcap_num.png");
  c01->SaveAs("figures/sideband_endcap_num.png");
  c02->SaveAs("figures/total_events_endcap.png");
  c1->SaveAs("figures/signal_endcap_eff.png");
  c2->SaveAs("figures/sideband_endcap_eff.png");
}

int main()
{
  iso_efficiency();
  return 0;
}
