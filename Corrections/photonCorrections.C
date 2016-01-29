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

    if(!montecarlo){
      if(!inPho.HBHENoiseFilterResult) continue; // only apply HBHE filter to real data
    }
    if(!inPho.pcollisionEventSelection) continue;

    outPho.run = inPho.run;
    outPho.event = inPho.event;
    outPho.lumis = inPho.lumis;
    outPho.hiBin = inPho.hiBin;
    outPho.HBHENoiseFilterResult = inPho.HBHENoiseFilterResult;
    outPho.pcollisionEventSelection = inPho.pcollisionEventSelection;

    for(int i = 0; i < inPho.nPho; ++i)
    {
      // only consider photons which pass spike cuts
      if(inPho.pho_swissCrx[i] > 0.9 ) continue;
      if(TMath::Abs(inPho.pho_seedTime[i]) > 3.0) continue;

      //
      outPho.nPho++;

      outPho.phoE.push_back( inPho.phoE[i] );
      outPho.phoEt.push_back( inPho.phoEt[i] );
      outPho.phoEta.push_back( inPho.phoEta[i] );
      outPho.phoPhi.push_back( inPho.phoPhi[i] );
      outPho.phoSCE.push_back( inPho.phoSCE[i] );
      outPho.phoSCRawE.push_back( inPho.phoSCRawE[i] );
      outPho.phoESEn.push_back( inPho.phoESEn[i] );
      outPho.phoSCEta.push_back( inPho.phoSCEta[i] );
      outPho.phoSCPhi.push_back( inPho.phoSCPhi[i] );
      outPho.phoSCEtaWidth.push_back( inPho.phoSCEtaWidth[i] );
      outPho.phoSCPhiWidth.push_back( inPho.phoSCPhiWidth[i] );
      outPho.phoSCBrem.push_back( inPho.phoSCBrem[i] );
      outPho.phohasPixelSeed.push_back( inPho.phohasPixelSeed[i] );
      outPho.phoR9.push_back( inPho.phoR9[i] );
      outPho.phoHoverE.push_back( inPho.phoHoverE[i] );
      outPho.phoSigmaIEtaIEta.push_back( inPho.phoSigmaIEtaIEta[i] );
      //outPho.phoE1x3.push_back( inPho.phoE1x3[i] );
      //outPho.phoE2x2.push_back( inPho.phoE2x2[i] );
      //outPho.phoE3x3.push_back( inPho.phoE3x3[i] );
      //outPho.phoE2x5Max.push_back( inPho.phoE2x5Max[i] );
      //outPho.phoE1x5.push_back( inPho.phoE1x5[i] );
      //outPho.phoE2x5.push_back( inPho.phoE2x5[i] );
      //outPho.phoE5x5.push_back( inPho.phoE5x5[i] );
      //outPho.phoMaxEnergyXtal.push_back( inPho.phoMaxEnergyXtal[i] );
      //outPho.phoSigmaEtaEta.push_back( inPho.phoSigmaEtaEta[i] );
      //outPho.phoR1x5.push_back( inPho.phoR1x5[i] );
      //outPho.phoR2x5.push_back( inPho.phoR2x5[i] );
      //outPho.phoESEffSigmaRR.push_back( inPho.phoESEffSigmaRR[i] );
      outPho.phoSigmaIEtaIEta_2012.push_back( inPho.phoSigmaIEtaIEta_2012[i] );
      //outPho.phoSigmaIEtaIPhi_2012.push_back( inPho.phoSigmaIEtaIPhi_2012[i] );
      //outPho.phoSigmaIPhiIPhi_2012.push_back( inPho.phoSigmaIPhiIPhi_2012[i] );
      //outPho.phoE1x3_2012.push_back( inPho.phoE1x3_2012[i] );
      //outPho.phoE2x2_2012.push_back( inPho.phoE2x2_2012[i] );
      //outPho.phoE3x3_2012.push_back( inPho.phoE3x3_2012[i] );
      //outPho.phoE2x5Max_2012.push_back( inPho.phoE2x5Max_2012[i] );
      //outPho.phoE5x5_2012.push_back( inPho.phoE5x5_2012[i] );
      //outPho.phoBC1E.push_back( inPho.phoBC1E[i] );
      //outPho.phoBC1Eta.push_back( inPho.phoBC1Eta[i] );
      //outPho.phoBC2E.push_back( inPho.phoBC2E[i] );
      //outPho.phoBC2Eta.push_back( inPho.phoBC2Eta[i] );
      outPho.pho_ecalClusterIsoR2.push_back( inPho.pho_ecalClusterIsoR2[i] );
      outPho.pho_ecalClusterIsoR3.push_back( inPho.pho_ecalClusterIsoR3[i] );
      outPho.pho_ecalClusterIsoR4.push_back( inPho.pho_ecalClusterIsoR4[i] );
      outPho.pho_ecalClusterIsoR5.push_back( inPho.pho_ecalClusterIsoR5[i] );
      outPho.pho_hcalRechitIsoR1.push_back( inPho.pho_hcalRechitIsoR1[i] );
      outPho.pho_hcalRechitIsoR2.push_back( inPho.pho_hcalRechitIsoR2[i] );
      outPho.pho_hcalRechitIsoR3.push_back( inPho.pho_hcalRechitIsoR3[i] );
      outPho.pho_hcalRechitIsoR4.push_back( inPho.pho_hcalRechitIsoR4[i] );
      outPho.pho_hcalRechitIsoR5.push_back( inPho.pho_hcalRechitIsoR5[i] );
      outPho.pho_trackIsoR1PtCut20.push_back( inPho.pho_trackIsoR1PtCut20[i] );
      outPho.pho_trackIsoR2PtCut20.push_back( inPho.pho_trackIsoR2PtCut20[i] );
      outPho.pho_trackIsoR3PtCut20.push_back( inPho.pho_trackIsoR3PtCut20[i] );
      outPho.pho_trackIsoR4PtCut20.push_back( inPho.pho_trackIsoR4PtCut20[i] );
      outPho.pho_trackIsoR5PtCut20.push_back( inPho.pho_trackIsoR5PtCut20[i] );
      outPho.pho_swissCrx.push_back( inPho.pho_swissCrx[i] );
      outPho.pho_seedTime.push_back( inPho.pho_seedTime[i] );

      outPho.pfcIso1.push_back( inPho.pfcIso1[i] );
      outPho.pfcIso2.push_back( inPho.pfcIso2[i] );
      outPho.pfcIso3.push_back( inPho.pfcIso3[i] );
      outPho.pfcIso4.push_back( inPho.pfcIso4[i] );
      outPho.pfcIso5.push_back( inPho.pfcIso5[i] );

      outPho.pfpIso1.push_back( inPho.pfpIso1[i] );
      outPho.pfpIso2.push_back( inPho.pfpIso2[i] );
      outPho.pfpIso3.push_back( inPho.pfpIso3[i] );
      outPho.pfpIso4.push_back( inPho.pfpIso4[i] );
      outPho.pfpIso5.push_back( inPho.pfpIso5[i] );

      outPho.pfnIso1.push_back( inPho.pfnIso1[i] );
      outPho.pfnIso2.push_back( inPho.pfnIso2[i] );
      outPho.pfnIso3.push_back( inPho.pfnIso3[i] );
      outPho.pfnIso4.push_back( inPho.pfnIso4[i] );
      outPho.pfnIso5.push_back( inPho.pfnIso5[i] );

      outPho.pfcVsIso1.push_back( inPho.pfcVsIso1[i] );
      outPho.pfcVsIso2.push_back( inPho.pfcVsIso2[i] );
      outPho.pfcVsIso3.push_back( inPho.pfcVsIso3[i] );
      outPho.pfcVsIso4.push_back( inPho.pfcVsIso4[i] );
      outPho.pfcVsIso5.push_back( inPho.pfcVsIso5[i] );
      outPho.pfcVsIso1th1.push_back( inPho.pfcVsIso1th1[i] );
      outPho.pfcVsIso2th1.push_back( inPho.pfcVsIso2th1[i] );
      outPho.pfcVsIso3th1.push_back( inPho.pfcVsIso3th1[i] );
      outPho.pfcVsIso4th1.push_back( inPho.pfcVsIso4th1[i] );
      outPho.pfcVsIso5th1.push_back( inPho.pfcVsIso5th1[i] );
      outPho.pfcVsIso1th2.push_back( inPho.pfcVsIso1th2[i] );
      outPho.pfcVsIso2th2.push_back( inPho.pfcVsIso2th2[i] );
      outPho.pfcVsIso3th2.push_back( inPho.pfcVsIso3th2[i] );
      outPho.pfcVsIso4th2.push_back( inPho.pfcVsIso4th2[i] );
      outPho.pfcVsIso5th2.push_back( inPho.pfcVsIso5th2[i] );

      outPho.pfnVsIso1.push_back( inPho.pfnVsIso1[i] );
      outPho.pfnVsIso2.push_back( inPho.pfnVsIso2[i] );
      outPho.pfnVsIso3.push_back( inPho.pfnVsIso3[i] );
      outPho.pfnVsIso4.push_back( inPho.pfnVsIso4[i] );
      outPho.pfnVsIso5.push_back( inPho.pfnVsIso5[i] );
      outPho.pfnVsIso1th1.push_back( inPho.pfnVsIso1th1[i] );
      outPho.pfnVsIso2th1.push_back( inPho.pfnVsIso2th1[i] );
      outPho.pfnVsIso3th1.push_back( inPho.pfnVsIso3th1[i] );
      outPho.pfnVsIso4th1.push_back( inPho.pfnVsIso4th1[i] );
      outPho.pfnVsIso5th1.push_back( inPho.pfnVsIso5th1[i] );
      outPho.pfnVsIso1th2.push_back( inPho.pfnVsIso1th2[i] );
      outPho.pfnVsIso2th2.push_back( inPho.pfnVsIso2th2[i] );
      outPho.pfnVsIso3th2.push_back( inPho.pfnVsIso3th2[i] );
      outPho.pfnVsIso4th2.push_back( inPho.pfnVsIso4th2[i] );
      outPho.pfnVsIso5th2.push_back( inPho.pfnVsIso5th2[i] );


      outPho.pfpVsIso1.push_back( inPho.pfpVsIso1[i] );
      outPho.pfpVsIso2.push_back( inPho.pfpVsIso2[i] );
      outPho.pfpVsIso3.push_back( inPho.pfpVsIso3[i] );
      outPho.pfpVsIso4.push_back( inPho.pfpVsIso4[i] );
      outPho.pfpVsIso5.push_back( inPho.pfpVsIso5[i] );
      outPho.pfpVsIso1th1.push_back( inPho.pfpVsIso1th1[i] );
      outPho.pfpVsIso2th1.push_back( inPho.pfpVsIso2th1[i] );
      outPho.pfpVsIso3th1.push_back( inPho.pfpVsIso3th1[i] );
      outPho.pfpVsIso4th1.push_back( inPho.pfpVsIso4th1[i] );
      outPho.pfpVsIso5th1.push_back( inPho.pfpVsIso5th1[i] );
      outPho.pfpVsIso1th2.push_back( inPho.pfpVsIso1th2[i] );
      outPho.pfpVsIso2th2.push_back( inPho.pfpVsIso2th2[i] );
      outPho.pfpVsIso3th2.push_back( inPho.pfpVsIso3th2[i] );
      outPho.pfpVsIso4th2.push_back( inPho.pfpVsIso4th2[i] );
      outPho.pfpVsIso5th2.push_back( inPho.pfpVsIso5th2[i] );

      // outPho.towerIso1.push_back( inPho.towerIso1[i] );
      // outPho.towerIso2.push_back( inPho.towerIso2[i] );
      // outPho.towerIso3.push_back( inPho.towerIso3[i] );
      // outPho.towerIso4.push_back( inPho.towerIso4[i] );
      // outPho.towerIso5.push_back( inPho.towerIso5[i] );
      // outPho.towerVsIso1.push_back( inPho.towerVsIso1[i] );
      // outPho.towerVsIso2.push_back( inPho.towerVsIso2[i] );
      // outPho.towerVsIso3.push_back( inPho.towerVsIso3[i] );
      // outPho.towerVsIso4.push_back( inPho.towerVsIso4[i] );
      // outPho.towerVsIso5.push_back( inPho.towerVsIso5[i] );
      // outPho.towerVsSubIso1.push_back( inPho.towerVsSubIso1[i] );
      // outPho.towerVsSubIso2.push_back( inPho.towerVsSubIso2[i] );
      // outPho.towerVsSubIso3.push_back( inPho.towerVsSubIso3[i] );
      // outPho.towerVsSubIso4.push_back( inPho.towerVsSubIso4[i] );
      // outPho.towerVsSubIso5.push_back( inPho.towerVsSubIso5[i] );
    }

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
