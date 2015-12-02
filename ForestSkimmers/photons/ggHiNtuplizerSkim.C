#include <TFile.h>
#include <TString.h>
#include <iostream>
#include "../../TreeHeaders/ggHiNtuplizerTree.h"
#include "../../CutConfigurations/CutConfigurationsParser.h"
#include "../../TreeHeaders/CutConfigurationTree.h"
#include "../../TreeHeaders/ggHiNtuplizerPhotonSkim.h"

void ggHiNtuplizerSkim(const TString configFile, const TString inputHiForest, const TString outputSkimFile)
{
  CutConfiguration config = CutConfigurationsParser::Parse(configFile.Data());
  TTree *configTree = setupConfigurationTreeForWriting(config);

  //std::cout << config.proc[CUTS::kSKIM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_et] << std::endl;

  TFile *inHiForest = TFile::Open(inputHiForest);
  TTree *inTree = (TTree*)inHiForest->Get("ggHiNtuplizer/EventTree");
  ggHiNtuplizer pho;
  setupPhotonTree(inTree, pho);

  TTree *skimTree = (TTree*)inHiForest->Get("skimanalysis/HltTree");
  Int_t HBHENoiseFilterResult;
  Int_t pcollisionEventSelection;
  skimTree->SetBranchAddress("HBHENoiseFilterResult",&HBHENoiseFilterResult);
  skimTree->SetBranchAddress("pcollisionEventSelection",&pcollisionEventSelection);

  PhotonSkim phoSkim;
  TTree *outTree = new TTree("photonSkimTree","photonSkimTree");
  phoSkim.setupTreeBranchesForWriting(outTree);

  Long64_t nentries = inTree->GetEntries();
  for(int ientries = 0; ientries < nentries; ++ientries)
  {
    inTree->GetEntry(ientries);
    skimTree->GetEntry(ientries);

    phoSkim.reset();

    //
    phoSkim.run = pho.run;
    phoSkim.event = pho.event;
    phoSkim.lumis = pho.lumis;
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
    phoSkim.phohasPixelSeed = * pho.phohasPixelSeed;
    phoSkim.phoR9 = * pho.phoR9;
    phoSkim.phoHoverE = * pho.phoHoverE;
    phoSkim.phoSigmaIEtaIEta = * pho.phoSigmaIEtaIEta;
    phoSkim.phoE1x3 = * pho.phoE1x3;
    phoSkim.phoE2x2 = * pho.phoE2x2;
    //phoSkim.phoE3x3 = * pho.phoE3x3;
    phoSkim.phoE2x5Max = * pho.phoE2x5Max;
    //phoSkim.phoE1x5 = * pho.phoE1x5;
    //phoSkim.phoE2x5 = * pho.phoE2x5;
    phoSkim.phoE5x5 = * pho.phoE5x5;
    //phoSkim.phoMaxEnergyXtal = * pho.phoMaxEnergyXtal;
    //phoSkim.phoSigmaEtaEta = * pho.phoSigmaEtaEta;
    //phoSkim.phoR1x5 = * pho.phoR1x5;
    //phoSkim.phoR2x5 = * pho.phoR2x5;
    phoSkim.phoESEffSigmaRR = * pho.phoESEffSigmaRR;
    phoSkim.phoSigmaIEtaIEta_2012 = * pho.phoSigmaIEtaIEta_2012;
    phoSkim.phoSigmaIEtaIPhi_2012 = * pho.phoSigmaIEtaIPhi_2012;
    phoSkim.phoSigmaIPhiIPhi_2012 = * pho.phoSigmaIPhiIPhi_2012;
    phoSkim.phoE1x3_2012 = * pho.phoE1x3_2012;
    phoSkim.phoE2x2_2012 = * pho.phoE2x2_2012;
    //phoSkim.phoE3x3_2012 = * pho.phoE3x3_2012;
    phoSkim.phoE2x5Max_2012 = * pho.phoE2x5Max_2012;
    phoSkim.phoE5x5_2012 = * pho.phoE5x5_2012;
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

    //phoSkim.pfsumIso1 = * pho.pfsumIso1;
    //phoSkim.pfsumIso2 = * pho.pfsumIso2;
    //phoSkim.pfsumIso3 = * pho.pfsumIso3;
    //phoSkim.pfsumIso4 = * pho.pfsumIso4;
    //phoSkim.pfsumIso5 = * pho.pfsumIso5;

    phoSkim.pfcVsIso1 = * pho.pfcVsIso1;
    phoSkim.pfcVsIso2 = * pho.pfcVsIso2;
    phoSkim.pfcVsIso3 = * pho.pfcVsIso3;
    phoSkim.pfcVsIso4 = * pho.pfcVsIso4;
    phoSkim.pfcVsIso5 = * pho.pfcVsIso5;
    phoSkim.pfcVsIso1th1 = * pho.pfcVsIso1th1;
    phoSkim.pfcVsIso2th1 = * pho.pfcVsIso2th1;
    phoSkim.pfcVsIso3th1 = * pho.pfcVsIso3th1;
    phoSkim.pfcVsIso4th1 = * pho.pfcVsIso4th1;
    phoSkim.pfcVsIso5th1 = * pho.pfcVsIso5th1;
    phoSkim.pfcVsIso1th2 = * pho.pfcVsIso1th2;
    phoSkim.pfcVsIso2th2 = * pho.pfcVsIso2th2;
    phoSkim.pfcVsIso3th2 = * pho.pfcVsIso3th2;
    phoSkim.pfcVsIso4th2 = * pho.pfcVsIso4th2;
    phoSkim.pfcVsIso5th2 = * pho.pfcVsIso5th2;

    phoSkim.pfnVsIso1 = * pho.pfnVsIso1;
    phoSkim.pfnVsIso2 = * pho.pfnVsIso2;
    phoSkim.pfnVsIso3 = * pho.pfnVsIso3;
    phoSkim.pfnVsIso4 = * pho.pfnVsIso4;
    phoSkim.pfnVsIso5 = * pho.pfnVsIso5;
    phoSkim.pfnVsIso1th1 = * pho.pfnVsIso1th1;
    phoSkim.pfnVsIso2th1 = * pho.pfnVsIso2th1;
    phoSkim.pfnVsIso3th1 = * pho.pfnVsIso3th1;
    phoSkim.pfnVsIso4th1 = * pho.pfnVsIso4th1;
    phoSkim.pfnVsIso5th1 = * pho.pfnVsIso5th1;
    phoSkim.pfnVsIso1th2 = * pho.pfnVsIso1th2;
    phoSkim.pfnVsIso2th2 = * pho.pfnVsIso2th2;
    phoSkim.pfnVsIso3th2 = * pho.pfnVsIso3th2;
    phoSkim.pfnVsIso4th2 = * pho.pfnVsIso4th2;
    phoSkim.pfnVsIso5th2 = * pho.pfnVsIso5th2;


    phoSkim.pfpVsIso1 = * pho.pfpVsIso1;
    phoSkim.pfpVsIso2 = * pho.pfpVsIso2;
    phoSkim.pfpVsIso3 = * pho.pfpVsIso3;
    phoSkim.pfpVsIso4 = * pho.pfpVsIso4;
    phoSkim.pfpVsIso5 = * pho.pfpVsIso5;
    phoSkim.pfpVsIso1th1 = * pho.pfpVsIso1th1;
    phoSkim.pfpVsIso2th1 = * pho.pfpVsIso2th1;
    phoSkim.pfpVsIso3th1 = * pho.pfpVsIso3th1;
    phoSkim.pfpVsIso4th1 = * pho.pfpVsIso4th1;
    phoSkim.pfpVsIso5th1 = * pho.pfpVsIso5th1;
    phoSkim.pfpVsIso1th2 = * pho.pfpVsIso1th2;
    phoSkim.pfpVsIso2th2 = * pho.pfpVsIso2th2;
    phoSkim.pfpVsIso3th2 = * pho.pfpVsIso3th2;
    phoSkim.pfpVsIso4th2 = * pho.pfpVsIso4th2;
    phoSkim.pfpVsIso5th2 = * pho.pfpVsIso5th2;

    //phoSkim.pfsumVsIso1 = * pho.pfsumVsIso1;
    //phoSkim.pfsumVsIso2 = * pho.pfsumVsIso2;
    //phoSkim.pfsumVsIso3 = * pho.pfsumVsIso3;
    //phoSkim.pfsumVsIso4 = * pho.pfsumVsIso4;
    //phoSkim.pfsumVsIso5 = * pho.pfsumVsIso5;
    //phoSkim.pfsumVsIso1th1 = * pho.pfsumVsIso1th1;
    // phoSkim.pfsumVsIso2th1 = * pho.pfsumVsIso2th1;
    // phoSkim.pfsumVsIso3th1 = * pho.pfsumVsIso3th1;
    // phoSkim.pfsumVsIso4th1 = * pho.pfsumVsIso4th1;
    // phoSkim.pfsumVsIso5th1 = * pho.pfsumVsIso5th1;
    // phoSkim.pfsumVsIso1th2 = * pho.pfsumVsIso1th2;
    // phoSkim.pfsumVsIso2th2 = * pho.pfsumVsIso2th2;
    // phoSkim.pfsumVsIso3th2 = * pho.pfsumVsIso3th2;
    // phoSkim.pfsumVsIso4th2 = * pho.pfsumVsIso4th2;
    // phoSkim.pfsumVsIso5th2 = * pho.pfsumVsIso5th2;


    // phoSkim.pfVsSubIso1 = * pho.pfVsSubIso1;
    // phoSkim.pfVsSubIso2 = * pho.pfVsSubIso2;
    // phoSkim.pfVsSubIso3 = * pho.pfVsSubIso3;
    // phoSkim.pfVsSubIso4 = * pho.pfVsSubIso4;
    // phoSkim.pfVsSubIso5 = * pho.pfVsSubIso5;

    phoSkim.towerIso1 = * pho.towerIso1;
    phoSkim.towerIso2 = * pho.towerIso2;
    phoSkim.towerIso3 = * pho.towerIso3;
    phoSkim.towerIso4 = * pho.towerIso4;
    phoSkim.towerIso5 = * pho.towerIso5;
    phoSkim.towerVsIso1 = * pho.towerVsIso1;
    phoSkim.towerVsIso2 = * pho.towerVsIso2;
    phoSkim.towerVsIso3 = * pho.towerVsIso3;
    phoSkim.towerVsIso4 = * pho.towerVsIso4;
    phoSkim.towerVsIso5 = * pho.towerVsIso5;
    phoSkim.towerVsSubIso1 = * pho.towerVsSubIso1;
    phoSkim.towerVsSubIso2 = * pho.towerVsSubIso2;
    phoSkim.towerVsSubIso3 = * pho.towerVsSubIso3;
    phoSkim.towerVsSubIso4 = * pho.towerVsSubIso4;
    phoSkim.towerVsSubIso5 = * pho.towerVsSubIso5;

    outTree->Fill();
  }

  TFile *outFile = TFile::Open(outputSkimFile,"RECREATE");
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
