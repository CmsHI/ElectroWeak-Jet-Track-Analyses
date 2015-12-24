#include <TFile.h>
#include <TString.h>
#include <iostream>

#include "../TreeHeaders/CutConfigurationTree.h"
#include "../TreeHeaders/ggHiNtuplizerPhotonSkim.h"
#include "../Utilities/interface/CutConfigurationParser.h"

void photonCorrections(const TString configFile, const TString inputSkimFile, const TString outputSkimFile)
{
  CutConfiguration config = CutConfigurationParser::Parse(configFile.Data());
  TTree *configTree = setupConfigurationTreeForWriting(config);

  int montecarlo = config.proc[CUTS::kCORRECTION].obj[CUTS::kPHOTON].i[CUTS::PHO::k_MonteCarlo];

  TFile *inSkimFile = TFile::Open(inputSkimFile);
  TTree *inTree = (TTree*)inSkimFile->Get("photonSkimTree");
  PhotonSkim inPho(montecarlo);
  inPho.setupTreeForReading(inTree);

  TFile *outFile = TFile::Open(outputSkimFile,"RECREATE");
  PhotonSkim outPho(montecarlo);
  TTree *outTree = new TTree("photonSkimTree","photonSkimTree");
  outPho.setupTreeForWriting(outTree);

  Long64_t nentries = inTree->GetEntries();
  for(int ientries = 0; ientries < nentries; ++ientries)
  {
    inTree->GetEntry(ientries);

    inPho.resolvePointers();
    outPho.reset();

    //
    outPho.run = inPho.run;
    outPho.event = inPho.event;
    outPho.lumis = inPho.lumis;
    outPho.hiBin = inPho.hiBin;
    outPho.HBHENoiseFilterResult = inPho.HBHENoiseFilterResult;
    outPho.pcollisionEventSelection = inPho.pcollisionEventSelection;

    outPho.nPho = inPho.nPho;

    outPho.phoE = inPho.phoE;
    outPho.phoEt = inPho.phoEt;
    outPho.phoEta = inPho.phoEta;
    outPho.phoPhi = inPho.phoPhi;
    outPho.phoSCE = inPho.phoSCE;
    outPho.phoSCRawE = inPho.phoSCRawE;
    outPho.phoESEn = inPho.phoESEn;
    outPho.phoSCEta = inPho.phoSCEta;
    outPho.phoSCPhi = inPho.phoSCPhi;
    outPho.phoSCEtaWidth = inPho.phoSCEtaWidth;
    outPho.phoSCPhiWidth = inPho.phoSCPhiWidth;
    outPho.phoSCBrem = inPho.phoSCBrem;
    outPho.phohasPixelSeed = inPho.phohasPixelSeed;
    outPho.phoR9 = inPho.phoR9;
    outPho.phoHoverE = inPho.phoHoverE;
    outPho.phoSigmaIEtaIEta = inPho.phoSigmaIEtaIEta;
    outPho.phoE1x3 = inPho.phoE1x3;
    outPho.phoE2x2 = inPho.phoE2x2;
    //outPho.phoE3x3 = inPho.phoE3x3;
    outPho.phoE2x5Max = inPho.phoE2x5Max;
    //outPho.phoE1x5 = inPho.phoE1x5;
    //outPho.phoE2x5 = inPho.phoE2x5;
    outPho.phoE5x5 = inPho.phoE5x5;
    //outPho.phoMaxEnergyXtal = inPho.phoMaxEnergyXtal;
    //outPho.phoSigmaEtaEta = inPho.phoSigmaEtaEta;
    //outPho.phoR1x5 = inPho.phoR1x5;
    //outPho.phoR2x5 = inPho.phoR2x5;
    outPho.phoESEffSigmaRR = inPho.phoESEffSigmaRR;
    outPho.phoSigmaIEtaIEta_2012 = inPho.phoSigmaIEtaIEta_2012;
    outPho.phoSigmaIEtaIPhi_2012 = inPho.phoSigmaIEtaIPhi_2012;
    outPho.phoSigmaIPhiIPhi_2012 = inPho.phoSigmaIPhiIPhi_2012;
    outPho.phoE1x3_2012 = inPho.phoE1x3_2012;
    outPho.phoE2x2_2012 = inPho.phoE2x2_2012;
    //outPho.phoE3x3_2012 = inPho.phoE3x3_2012;
    outPho.phoE2x5Max_2012 = inPho.phoE2x5Max_2012;
    outPho.phoE5x5_2012 = inPho.phoE5x5_2012;
    outPho.phoBC1E = inPho.phoBC1E;
    outPho.phoBC1Eta = inPho.phoBC1Eta;
    outPho.phoBC2E = inPho.phoBC2E;
    outPho.phoBC2Eta = inPho.phoBC2Eta;
    outPho.pho_ecalClusterIsoR2 = inPho.pho_ecalClusterIsoR2;
    outPho.pho_ecalClusterIsoR3 = inPho.pho_ecalClusterIsoR3;
    outPho.pho_ecalClusterIsoR4 = inPho.pho_ecalClusterIsoR4;
    outPho.pho_ecalClusterIsoR5 = inPho.pho_ecalClusterIsoR5;
    outPho.pho_hcalRechitIsoR1 = inPho.pho_hcalRechitIsoR1;
    outPho.pho_hcalRechitIsoR2 = inPho.pho_hcalRechitIsoR2;
    outPho.pho_hcalRechitIsoR3 = inPho.pho_hcalRechitIsoR3;
    outPho.pho_hcalRechitIsoR4 = inPho.pho_hcalRechitIsoR4;
    outPho.pho_hcalRechitIsoR5 = inPho.pho_hcalRechitIsoR5;
    outPho.pho_trackIsoR1PtCut20 = inPho.pho_trackIsoR1PtCut20;
    outPho.pho_trackIsoR2PtCut20 = inPho.pho_trackIsoR2PtCut20;
    outPho.pho_trackIsoR3PtCut20 = inPho.pho_trackIsoR3PtCut20;
    outPho.pho_trackIsoR4PtCut20 = inPho.pho_trackIsoR4PtCut20;
    outPho.pho_trackIsoR5PtCut20 = inPho.pho_trackIsoR5PtCut20;
    outPho.pho_swissCrx = inPho.pho_swissCrx;
    outPho.pho_seedTime = inPho.pho_seedTime;

    outPho.pfcIso1 = inPho.pfcIso1;
    outPho.pfcIso2 = inPho.pfcIso2;
    outPho.pfcIso3 = inPho.pfcIso3;
    outPho.pfcIso4 = inPho.pfcIso4;
    outPho.pfcIso5 = inPho.pfcIso5;

    outPho.pfpIso1 = inPho.pfpIso1;
    outPho.pfpIso2 = inPho.pfpIso2;
    outPho.pfpIso3 = inPho.pfpIso3;
    outPho.pfpIso4 = inPho.pfpIso4;
    outPho.pfpIso5 = inPho.pfpIso5;

    outPho.pfnIso1 = inPho.pfnIso1;
    outPho.pfnIso2 = inPho.pfnIso2;
    outPho.pfnIso3 = inPho.pfnIso3;
    outPho.pfnIso4 = inPho.pfnIso4;
    outPho.pfnIso5 = inPho.pfnIso5;

    outPho.pfcVsIso1 = inPho.pfcVsIso1;
    outPho.pfcVsIso2 = inPho.pfcVsIso2;
    outPho.pfcVsIso3 = inPho.pfcVsIso3;
    outPho.pfcVsIso4 = inPho.pfcVsIso4;
    outPho.pfcVsIso5 = inPho.pfcVsIso5;
    outPho.pfcVsIso1th1 = inPho.pfcVsIso1th1;
    outPho.pfcVsIso2th1 = inPho.pfcVsIso2th1;
    outPho.pfcVsIso3th1 = inPho.pfcVsIso3th1;
    outPho.pfcVsIso4th1 = inPho.pfcVsIso4th1;
    outPho.pfcVsIso5th1 = inPho.pfcVsIso5th1;
    outPho.pfcVsIso1th2 = inPho.pfcVsIso1th2;
    outPho.pfcVsIso2th2 = inPho.pfcVsIso2th2;
    outPho.pfcVsIso3th2 = inPho.pfcVsIso3th2;
    outPho.pfcVsIso4th2 = inPho.pfcVsIso4th2;
    outPho.pfcVsIso5th2 = inPho.pfcVsIso5th2;

    outPho.pfnVsIso1 = inPho.pfnVsIso1;
    outPho.pfnVsIso2 = inPho.pfnVsIso2;
    outPho.pfnVsIso3 = inPho.pfnVsIso3;
    outPho.pfnVsIso4 = inPho.pfnVsIso4;
    outPho.pfnVsIso5 = inPho.pfnVsIso5;
    outPho.pfnVsIso1th1 = inPho.pfnVsIso1th1;
    outPho.pfnVsIso2th1 = inPho.pfnVsIso2th1;
    outPho.pfnVsIso3th1 = inPho.pfnVsIso3th1;
    outPho.pfnVsIso4th1 = inPho.pfnVsIso4th1;
    outPho.pfnVsIso5th1 = inPho.pfnVsIso5th1;
    outPho.pfnVsIso1th2 = inPho.pfnVsIso1th2;
    outPho.pfnVsIso2th2 = inPho.pfnVsIso2th2;
    outPho.pfnVsIso3th2 = inPho.pfnVsIso3th2;
    outPho.pfnVsIso4th2 = inPho.pfnVsIso4th2;
    outPho.pfnVsIso5th2 = inPho.pfnVsIso5th2;


    outPho.pfpVsIso1 = inPho.pfpVsIso1;
    outPho.pfpVsIso2 = inPho.pfpVsIso2;
    outPho.pfpVsIso3 = inPho.pfpVsIso3;
    outPho.pfpVsIso4 = inPho.pfpVsIso4;
    outPho.pfpVsIso5 = inPho.pfpVsIso5;
    outPho.pfpVsIso1th1 = inPho.pfpVsIso1th1;
    outPho.pfpVsIso2th1 = inPho.pfpVsIso2th1;
    outPho.pfpVsIso3th1 = inPho.pfpVsIso3th1;
    outPho.pfpVsIso4th1 = inPho.pfpVsIso4th1;
    outPho.pfpVsIso5th1 = inPho.pfpVsIso5th1;
    outPho.pfpVsIso1th2 = inPho.pfpVsIso1th2;
    outPho.pfpVsIso2th2 = inPho.pfpVsIso2th2;
    outPho.pfpVsIso3th2 = inPho.pfpVsIso3th2;
    outPho.pfpVsIso4th2 = inPho.pfpVsIso4th2;
    outPho.pfpVsIso5th2 = inPho.pfpVsIso5th2;

    outPho.towerIso1 = inPho.towerIso1;
    outPho.towerIso2 = inPho.towerIso2;
    outPho.towerIso3 = inPho.towerIso3;
    outPho.towerIso4 = inPho.towerIso4;
    outPho.towerIso5 = inPho.towerIso5;
    outPho.towerVsIso1 = inPho.towerVsIso1;
    outPho.towerVsIso2 = inPho.towerVsIso2;
    outPho.towerVsIso3 = inPho.towerVsIso3;
    outPho.towerVsIso4 = inPho.towerVsIso4;
    outPho.towerVsIso5 = inPho.towerVsIso5;
    outPho.towerVsSubIso1 = inPho.towerVsSubIso1;
    outPho.towerVsSubIso2 = inPho.towerVsSubIso2;
    outPho.towerVsSubIso3 = inPho.towerVsSubIso3;
    outPho.towerVsSubIso4 = inPho.towerVsSubIso4;
    outPho.towerVsSubIso5 = inPho.towerVsSubIso5;

    if(montecarlo){
      outPho.nMC = inPho.nMC;
      outPho.mcPID = inPho.mcPID;
      outPho.mcStatus = inPho.mcStatus;
      outPho.mcVtx_x = inPho.mcVtx_x;
      outPho.mcVtx_y = inPho.mcVtx_y;
      outPho.mcVtx_z = inPho.mcVtx_z;
      outPho.mcPt = inPho.mcPt;
      outPho.mcEta = inPho.mcEta;
      outPho.mcPhi = inPho.mcPhi;
      outPho.mcE = inPho.mcE;
      outPho.mcEt = inPho.mcEt;
      outPho.mcMass = inPho.mcMass;
      outPho.mcParentage = inPho.mcParentage;
      outPho.mcMomPID = inPho.mcMomPID;
      outPho.mcMomPt = inPho.mcMomPt;
      outPho.mcMomEta = inPho.mcMomEta;
      outPho.mcMomPhi = inPho.mcMomPhi;
      outPho.mcMomMass = inPho.mcMomMass;
      outPho.mcGMomPID = inPho.mcGMomPID;
      outPho.mcIndex = inPho.mcIndex;
      outPho.mcCalIsoDR03 = inPho.mcCalIsoDR03;
      outPho.mcCalIsoDR04 = inPho.mcCalIsoDR04;
      outPho.mcTrkIsoDR03 = inPho.mcTrkIsoDR03;
      outPho.mcTrkIsoDR04 = inPho.mcTrkIsoDR04;
      outPho.pho_genMatchedIndex = inPho.pho_genMatchedIndex;
    }


    outTree->Fill();
  }

  outFile->cd();
  configTree->Write();
  outTree->Write();
  outFile->Close();

}

int main(int argc, char **argv)
{
  if(argc == 4)
  {
    photonCorrections(argv[1], argv[2], argv[3]);
    return 0;
  } else {
    std::cout << "Usage : \n" <<
      "./ggHiNtuplizerSkim.exe <configFile> <inputHiForest> <outputSkimFile>"
	      << std::endl;
    return 1;
  }
}
