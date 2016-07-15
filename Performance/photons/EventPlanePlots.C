#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TString.h>
#include <TH1D.h>
#include <TMath.h>
#include <TProfile.h>

void EventPlanePlots(TString infilename){
  TFile *inFile = TFile::Open(infilename);
  TTree *EventTree = (TTree*)inFile->Get("EventTree");
  TTree *hiTree = (TTree*)inFile->Get("HiEvt");
  TTree *gjTree = (TTree*)inFile->Get("gamma_akPu3PFJetAnalyzer");

  EventTree->AddFriend(hiTree, "evt");
  EventTree->AddFriend(gjTree, "gj");

  TH1D *eventPlaneBias = new TH1D("eventPlaneBias",";#Delta #phi_{EP,#gamma}",100, 0, TMath::Pi()/2.0);
  
  EventTree->Draw("acos(cos(phoPhi[phoIdx] - hiEvtPlanes[8]))*(acos(cos(phoPhi[phoIdx] - hiEvtPlanes[8]))<TMath::Pi()/2.) + (TMath::Pi() - acos(cos(phoPhi[phoIdx] - hiEvtPlanes[8])))*(acos(cos(phoPhi[phoIdx] - hiEvtPlanes[8]))>TMath::Pi()/2.)>>eventPlaneBias","phoEtCorrected[phoIdx]>60&& !((phoE3x3[phoIdx]/phoE5x5[phoIdx] > 2/3-0.03 && phoE3x3[phoIdx]/phoE5x5[phoIdx] < 2/3+0.03) && (phoE1x5[phoIdx]/phoE5x5[phoIdx] > 1/3-0.03 && phoE1x5[phoIdx]/phoE5x5[phoIdx] < 1/3+0.03) && (phoE2x5[phoIdx]/phoE5x5[phoIdx] > 2/3-0.03 && phoE2x5[phoIdx]/phoE5x5[phoIdx] < 2/3+0.03)) && phoHoverE[phoIdx]<0.1 && (pho_ecalClusterIsoR4[phoIdx] + pho_hcalRechitIsoR4[phoIdx] + pho_trackIsoR4PtCut20[phoIdx])<1.0");

  TCanvas *c1 = new TCanvas();
  eventPlaneBias->SetMinimum(0);
  eventPlaneBias->SetStats(0);
  eventPlaneBias->Draw("hist");
  c1->SaveAs("eventPlaneBias.pdf");

  TProfile *meanIso = new TProfile("meanIso", ";#Delta #phi_{EP,#gamma};<SumIso>;", 100, 0, TMath::Pi()/2.0);

  EventTree->Draw("(pho_ecalClusterIsoR4[phoIdx] + pho_hcalRechitIsoR4[phoIdx] + pho_trackIsoR4PtCut20[phoIdx]):acos(cos(phoPhi[phoIdx] - hiEvtPlanes[8]))*(acos(cos(phoPhi[phoIdx] - hiEvtPlanes[8]))<TMath::Pi()/2.) + (TMath::Pi() - acos(cos(phoPhi[phoIdx] - hiEvtPlanes[8])))*(acos(cos(phoPhi[phoIdx] - hiEvtPlanes[8]))>TMath::Pi()/2.)>>meanIso","!((phoE3x3[phoIdx]/phoE5x5[phoIdx] > 2/3-0.03 && phoE3x3[phoIdx]/phoE5x5[phoIdx] < 2/3+0.03) && (phoE1x5[phoIdx]/phoE5x5[phoIdx] > 1/3-0.03 && phoE1x5[phoIdx]/phoE5x5[phoIdx] < 1/3+0.03) && (phoE2x5[phoIdx]/phoE5x5[phoIdx] > 2/3-0.03 && phoE2x5[phoIdx]/phoE5x5[phoIdx] < 2/3+0.03))","prof");

  meanIso->SetMinimum(25);
  meanIso->SetMaximum(75);
  meanIso->SetMarkerStyle(20);
  meanIso->SetMarkerColor(kBlack);
  meanIso->SetLineColor(kBlack);
  meanIso->Draw("e");
  c1->SaveAs("meanIso.pdf");
}

int main(int argc, char **argv){
  if(argc == 2){
    EventPlanePlots(argv[1]);
    return 0;
  } else {
    return 1;
  }
}
