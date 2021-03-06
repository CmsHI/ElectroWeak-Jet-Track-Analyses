#include <TFile.h>
#include <TString.h>
#include <iostream>

#include "../../TreeHeaders/ggHiNtuplizerTree.h"
#include "../../TreeHeaders/CutConfigurationTree.h"
#include "../../TreeHeaders/ggHiNtuplizerPhotonSkim.h"
#include "../../Utilities/interface/CutConfigurationParser.h"

void ggHiNtuplizerSkim(const TString configFile, const TString inputHiForest, const TString outputSkimFile)
{
  TFile *outFile = TFile::Open(outputSkimFile,"RECREATE");

  CutConfiguration config = CutConfigurationParser::Parse(configFile.Data());
  TTree *configTree = setupConfigurationTreeForWriting(config);

  Float_t photonEtCut = config.proc[CUTS::kSKIM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_et];
  int montecarlo = config.proc[CUTS::kSKIM].obj[CUTS::kPHOTON].i[CUTS::PHO::k_MonteCarlo];

  TFile *inHiForest = TFile::Open(inputHiForest);
  TTree *inTree = (TTree*)inHiForest->Get("ggHiNtuplizer/EventTree");
  ggHiNtuplizer pho;
  pho.setupTreeForReading(inTree);

  TTree *skimTree = (TTree*)inHiForest->Get("skimanalysis/HltTree");
  Int_t HBHENoiseFilterResult;
  Int_t pcollisionEventSelection;
  skimTree->SetBranchAddress("HBHENoiseFilterResult",&HBHENoiseFilterResult);
  skimTree->SetBranchAddress("pcollisionEventSelection",&pcollisionEventSelection);

  TTree *eventTree = (TTree*)inHiForest->Get("hiEvtAnalyzer/HiTree");
  ULong64_t event;
  unsigned run;
  unsigned lumi;
  int hiBin;
  float ptHat;
  eventTree->SetBranchAddress("evt",&event);
  eventTree->SetBranchAddress("run",&run);
  eventTree->SetBranchAddress("lumi",&lumi);
  eventTree->SetBranchAddress("hiBin",&hiBin);
  if(montecarlo){
    eventTree->SetBranchAddress("pthat",&ptHat);
  }
  outFile->cd();
  PhotonSkim phoSkim(montecarlo);
  TTree *outTree = new TTree("photonSkimTree","photonSkimTree");
  phoSkim.setupTreeForWriting(outTree);

  Long64_t nentries = inTree->GetEntries();
  for(int ientries = 0; ientries < nentries; ++ientries)
  {
    inTree->GetEntry(ientries);
    skimTree->GetEntry(ientries);

    if(pho.nPho < 1) continue;
    Float_t maxPho =-1;
    for(int i = 0; i < pho.nPho; ++i)
    {
      if(maxPho < pho.phoEt->at(i))
	maxPho = pho.phoEt->at(i);
    }
    if(maxPho < photonEtCut) continue;

    phoSkim.reset();

    eventTree->GetEntry(ientries);
    phoSkim.run = run;
    phoSkim.lumis = lumi;
    phoSkim.event = event;
    phoSkim.hiBin = hiBin;

    phoSkim.HBHENoiseFilterResult = HBHENoiseFilterResult;
    phoSkim.pcollisionEventSelection = pcollisionEventSelection;

    phoSkim.nPho = pho.nPho;

    phoSkim.phoE = * pho.phoE;
    phoSkim.phoEt = * pho.phoEt;
    phoSkim.phoEta = * pho.phoEta;
    phoSkim.phoPhi = * pho.phoPhi;
    phoSkim.phoSCE = * pho.phoSCE;
    phoSkim.phoSCRawE = * pho.phoSCRawE;
    phoSkim.phoESEn = * pho.phoESEn;
    phoSkim.phoSCEta = * pho.phoSCEta;
    phoSkim.phoSCPhi = * pho.phoSCPhi;
    phoSkim.phoSCEtaWidth = * pho.phoSCEtaWidth;
    phoSkim.phoSCPhiWidth = * pho.phoSCPhiWidth;
    phoSkim.phoSCBrem = * pho.phoSCBrem;
    phoSkim.phohasPixelSeed = * pho.phoHasPixelSeed;
    phoSkim.phoR9 = * pho.phoR9;
    phoSkim.phoHoverE = * pho.phoHoverE;
    phoSkim.phoSigmaIEtaIEta = * pho.phoSigmaIEtaIEta;
    //phoSkim.phoE1x3 = * pho.phoE1x3;
    //phoSkim.phoE2x2 = * pho.phoE2x2;
    phoSkim.phoE3x3 = * pho.phoE3x3;
    //phoSkim.phoE2x5Max = * pho.phoE2x5Max;
    phoSkim.phoE1x5 = * pho.phoE1x5;
    phoSkim.phoE2x5 = * pho.phoE2x5;
    phoSkim.phoE5x5 = * pho.phoE5x5;
    phoSkim.phoMaxEnergyXtal = * pho.phoMaxEnergyXtal;
    phoSkim.phoSigmaEtaEta = * pho.phoSigmaEtaEta;
    phoSkim.phoR1x5 = * pho.phoR1x5;
    phoSkim.phoR2x5 = * pho.phoR2x5;
    phoSkim.phoESEffSigmaRR = * pho.phoESEffSigmaRR;
    phoSkim.phoSigmaIEtaIEta_2012 = * pho.phoSigmaIEtaIEta_2012;
    //phoSkim.phoSigmaIEtaIPhi_2012 = * pho.phoSigmaIEtaIPhi_2012;
    //phoSkim.phoSigmaIPhiIPhi_2012 = * pho.phoSigmaIPhiIPhi_2012;
    //phoSkim.phoE1x3_2012 = * pho.phoE1x3_2012;
    //phoSkim.phoE2x2_2012 = * pho.phoE2x2_2012;
    //phoSkim.phoE3x3_2012 = * pho.phoE3x3_2012;
    //phoSkim.phoE2x5Max_2012 = * pho.phoE2x5Max_2012;
    phoSkim.phoE3x3_2012 = * pho.phoE3x3_2012;
    phoSkim.phoBC1E = * pho.phoBC1E;
    phoSkim.phoBC1Eta = * pho.phoBC1Eta;
    phoSkim.phoBC2E = * pho.phoBC2E;
    phoSkim.phoBC2Eta = * pho.phoBC2Eta;
    phoSkim.pho_ecalClusterIsoR2 = * pho.pho_ecalClusterIsoR2;
    phoSkim.pho_ecalClusterIsoR3 = * pho.pho_ecalClusterIsoR3;
    phoSkim.pho_ecalClusterIsoR4 = * pho.pho_ecalClusterIsoR4;
    phoSkim.pho_ecalClusterIsoR5 = * pho.pho_ecalClusterIsoR5;
    phoSkim.pho_hcalRechitIsoR1 = * pho.pho_hcalRechitIsoR1;
    phoSkim.pho_hcalRechitIsoR2 = * pho.pho_hcalRechitIsoR2;
    phoSkim.pho_hcalRechitIsoR3 = * pho.pho_hcalRechitIsoR3;
    phoSkim.pho_hcalRechitIsoR4 = * pho.pho_hcalRechitIsoR4;
    phoSkim.pho_hcalRechitIsoR5 = * pho.pho_hcalRechitIsoR5;
    phoSkim.pho_trackIsoR1PtCut20 = * pho.pho_trackIsoR1PtCut20;
    phoSkim.pho_trackIsoR2PtCut20 = * pho.pho_trackIsoR2PtCut20;
    phoSkim.pho_trackIsoR3PtCut20 = * pho.pho_trackIsoR3PtCut20;
    phoSkim.pho_trackIsoR4PtCut20 = * pho.pho_trackIsoR4PtCut20;
    phoSkim.pho_trackIsoR5PtCut20 = * pho.pho_trackIsoR5PtCut20;
    phoSkim.pho_swissCrx = * pho.pho_swissCrx;
    phoSkim.pho_seedTime = * pho.pho_seedTime;

    phoSkim.pfcIso1 = * pho.pfcIso1;
    phoSkim.pfcIso2 = * pho.pfcIso2;
    phoSkim.pfcIso3 = * pho.pfcIso3;
    phoSkim.pfcIso4 = * pho.pfcIso4;
    phoSkim.pfcIso5 = * pho.pfcIso5;

    phoSkim.pfpIso1 = * pho.pfpIso1;
    phoSkim.pfpIso2 = * pho.pfpIso2;
    phoSkim.pfpIso3 = * pho.pfpIso3;
    phoSkim.pfpIso4 = * pho.pfpIso4;
    phoSkim.pfpIso5 = * pho.pfpIso5;

    phoSkim.pfnIso1 = * pho.pfnIso1;
    phoSkim.pfnIso2 = * pho.pfnIso2;
    phoSkim.pfnIso3 = * pho.pfnIso3;
    phoSkim.pfnIso4 = * pho.pfnIso4;
    phoSkim.pfnIso5 = * pho.pfnIso5;

    if(montecarlo){
      phoSkim.nMC = pho.nMC;
      phoSkim.ptHat = ptHat;
      phoSkim.mcPID = * pho.mcPID;
      phoSkim.mcStatus = * pho.mcStatus;
      phoSkim.mcVtx_x = * pho.mcVtx_x;
      phoSkim.mcVtx_y = * pho.mcVtx_y;
      phoSkim.mcVtx_z = * pho.mcVtx_z;
      phoSkim.mcPt = * pho.mcPt;
      phoSkim.mcEta = * pho.mcEta;
      phoSkim.mcPhi = * pho.mcPhi;
      phoSkim.mcE = * pho.mcE;
      phoSkim.mcEt = * pho.mcEt;
      phoSkim.mcMass = * pho.mcMass;
      phoSkim.mcParentage = * pho.mcParentage;
      phoSkim.mcMomPID = * pho.mcMomPID;
      phoSkim.mcMomPt = * pho.mcMomPt;
      phoSkim.mcMomEta = * pho.mcMomEta;
      phoSkim.mcMomPhi = * pho.mcMomPhi;
      phoSkim.mcMomMass = * pho.mcMomMass;
      phoSkim.mcGMomPID = * pho.mcGMomPID;
      phoSkim.mcIndex = * pho.mcIndex;
      phoSkim.mcCalIsoDR03 = * pho.mcCalIsoDR03;
      phoSkim.mcCalIsoDR04 = * pho.mcCalIsoDR04;
      phoSkim.mcTrkIsoDR03 = * pho.mcTrkIsoDR03;
      phoSkim.mcTrkIsoDR04 = * pho.mcTrkIsoDR04;
      phoSkim.pho_genMatchedIndex = * pho.pho_genMatchedIndex;
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
    ggHiNtuplizerSkim(argv[1], argv[2], argv[3]);
    return 0;
  } else {
    std::cout << "Usage : \n" <<
      "./ggHiNtuplizerSkim.exe <configFile> <inputHiForest> <outputSkimFile>"
	      << std::endl;
    return 1;
  }
}
