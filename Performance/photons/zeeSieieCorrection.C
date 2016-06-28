#include <TFile.h>
#include <TTree.h>
#include <vector>
#include <TH1D.h>
#include <TMath.h>
#include <TCanvas.h>
#include <iostream>
#include <TLorentzVector.h>

#include "../../TreeHeaders/ggHiNtuplizerTree.h"

void zeeSieieCorrection(const TString inputFile){
  TFile *infile = TFile::Open(inputFile);
  TTree *ggTree = (TTree*)infile->Get("ggHiNtuplizer/EventTree");

  ggHiNtuplizer ggHi;
  ggHi.setupTreeForReading(ggTree);

  TH1D *h_phoSieie, *h_eleSieie;
  h_phoSieie = new TH1D("phoSieie","", 100, 0, 0.03);
  h_eleSieie = new TH1D("eleSieie","", 100, 0, 0.03);

  Long64_t numMatched = 0;
  Long64_t goodPhotons = 0;
  Long64_t goodElectrons = 0;
  Long64_t tentries = ggTree->GetEntries();
  for(Long64_t jentry = 0; jentry < tentries; jentry++){
    ggTree->GetEntry(jentry);
    ///
    int phoIdx = -1;     // index of the leading photon
    double maxPhoEt = -1;
    for (int i=0; i<ggHi.nPho; ++i) {
      if(ggHi.phoEt->at(i) < 40) continue;
      if(TMath::Abs(ggHi.phoEta->at(i)) > 1.44) continue;
      bool failedSpikeRejection;
      failedSpikeRejection = (ggHi.phoSigmaIEtaIEta->at(i) < 0.002 ||
			      ggHi.pho_swissCrx->at(i)     > 0.9   ||
			      TMath::Abs(ggHi.pho_seedTime->at(i)) > 3);
      if (failedSpikeRejection)	continue;
      
      if (ggHi.phoEt->at(i) > maxPhoEt) {
	maxPhoEt = ggHi.phoEt->at(i);
	phoIdx = i;
      }
    }
    if (phoIdx == -1)
      continue;
    goodPhotons++;
    float phoSieie = ggHi.phoSigmaIEtaIEta_2012->at(phoIdx);
    float phoEta = ggHi.phoEta->at(phoIdx);
    float phoPhi = ggHi.phoPhi->at(phoIdx);
    ///
    float matchedPt = -1;
    int eleIdx = -1;
    for(int i=0; i<ggHi.nEle; ++i){
      if(TMath::Abs(ggHi.eleSCEta->at(i)) > 1.4442) continue;
      if(ggHi.elePt->at(i) < 30) continue;
      if(ggHi.eleEoverPInv->at(i) > 0.28051) continue;
      if(TMath::Abs(ggHi.eledEtaAtVtx->at(i)) > 0.01576) continue;
      if(TMath::Abs(ggHi.eledPhiAtVtx->at(i)) > 0.15724) continue;
      if(ggHi.eleHoverE->at(i) > 0.08849) continue;
      if(TMath::Abs(ggHi.eleD0->at(i)) > 0.05216) continue;
      if(TMath::Abs(ggHi.eleDz->at(i)) > 0.12997) continue;
      if(TMath::Abs(ggHi.eleMissHits->at(i)) > 1.00005) continue;

      float eleEta = ggHi.eleEta->at(i);
      float elePhi = ggHi.elePhi->at(i);
      float dEta = phoEta-eleEta;
      float dPhi = phoPhi-elePhi;
      if ( dPhi >  3.141592 ) dPhi = dPhi - 2* 3.141592;
      if ( dPhi < -3.141592 ) dPhi = dPhi + 2* 3.141592;

      float dR2 = (dEta*dEta)+(dPhi*dPhi);
      if(dR2 > 0.4*0.4) continue; //super hawt optimized out that sqrt

      if (ggHi.elePt->at(i) > matchedPt) {
	matchedPt = ggHi.elePt->at(i);
	eleIdx = i;
      } else {
	continue;
      }

      goodElectrons++;
      // require that the electron be in a mass pair that *might* be part of a Z
      bool mightBeZ = false;
      for(int j=0; j<ggHi.nEle; ++j){
	if(i==j) continue;

	if(TMath::Abs(ggHi.eleSCEta->at(j)) < 1.4442){
	  if(ggHi.eleEoverPInv->at(j) > 0.28051) continue;
	  if(TMath::Abs(ggHi.eledEtaAtVtx->at(j)) > 0.01576) continue;
	  if(TMath::Abs(ggHi.eledPhiAtVtx->at(j)) > 0.15724) continue;
	  if(ggHi.eleHoverE->at(j) > 0.08849) continue;
	  if(TMath::Abs(ggHi.eleD0->at(j)) > 0.05216) continue;
	  if(TMath::Abs(ggHi.eleDz->at(j)) > 0.12997) continue;
	  if(TMath::Abs(ggHi.eleMissHits->at(j)) > 1.00005) continue;
	} else if(TMath::Abs(ggHi.eleSCEta->at(j)) > 1.566 && TMath::Abs(ggHi.eleSCEta->at(j)) < 2.5){
	  if(ggHi.eleEoverPInv->at(j) > 0.18672) continue;
	  if(TMath::Abs(ggHi.eledEtaAtVtx->at(j)) > 0.01707) continue;
	  if(TMath::Abs(ggHi.eledPhiAtVtx->at(j)) > 0.35537) continue;
	  if(ggHi.eleHoverE->at(j) > 0.12275) continue;
	  if(TMath::Abs(ggHi.eleD0->at(j)) > 0.19092) continue;
	  if(TMath::Abs(ggHi.eleDz->at(j)) > 0.26407) continue;
	  if(TMath::Abs(ggHi.eleMissHits->at(j)) > 1.00005) continue;
	} else {
	  continue;
	}
	float elePt2 = ggHi.elePt->at(j);
	float eleEta2 = ggHi.eleEta->at(j);
	float elePhi2 = ggHi.elePhi->at(j);
	
	TLorentzVector v1, v2, vSum;
	v1.SetPtEtaPhiM( matchedPt, eleEta,
			 elePhi, 0.000511);
	v2.SetPtEtaPhiM( elePt2, eleEta2,
			 elePhi2, 0.000511);
	vSum = v1+v2;
	if(vSum.M() < 60 || vSum.M() > 120) continue;
	///
	mightBeZ = true;
	break;
      }
      if(mightBeZ == false) continue;
      
    }
    if(eleIdx == -1) continue;
    float eleSieie = ggHi.eleSigmaIEtaIEta_2012->at(eleIdx);
    numMatched++;

    h_phoSieie->Fill(phoSieie);
    h_eleSieie->Fill(eleSieie);
  }
  std::cout << "Good Photons: " << goodPhotons << std::endl;
  std::cout << "Good Electrons: " << goodElectrons << std::endl;
  std::cout << "Total Matched: " << numMatched << std::endl;
  // TCanvas *c1 = new TCanvas();
  // h_phoSieie->Draw();
  // TCanvas *c2 = new TCanvas();
  // h_eleSieie->Draw();  
}

int main(int argc, char **argv){
  if(argc == 2){
    zeeSieieCorrection(argv[1]);
  }
  return 0;
}
