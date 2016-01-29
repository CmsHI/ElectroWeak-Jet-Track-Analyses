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
  float etaCutLow = config.proc[CUTS::kCORRECTION].obj[CUTS::kPHOTON].f[CUTS::PHO::k_EtaLowerBound];
  float etaCutHigh = config.proc[CUTS::kCORRECTION].obj[CUTS::kPHOTON].f[CUTS::PHO::k_EtaUpperBound];

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

    int maxIndex = -1;
    float maxEt = 0;
    for(int i = 0; i < inPho.nPho; ++i)
    {
      // only consider photons which pass spike cuts
      if(inPho.pho_swissCrx[i] > 0.9 ) continue;
      if(TMath::Abs(inPho.pho_seedTime[i]) > 3.0) continue;
      if(TMath::Abs(inPho.phoEta[i]) < etaCutLow) continue;
      if(TMath::Abs(inPho.phoEta[i]) > etaCutHigh) continue;
      //

      if(inPho.phoEt[i] > maxEt){
	maxIndex = i;
	maxEt = inPho.phoEt[i];
      }
    }

    if(maxIndex == -1) continue;
    
    outPho.nPho = 1;

    outPho.phoE.push_back( inPho.phoE[maxIndex] );
    outPho.phoEt.push_back( inPho.phoEt[maxIndex] );
    outPho.phoEta.push_back( inPho.phoEta[maxIndex] );
    outPho.phoPhi.push_back( inPho.phoPhi[maxIndex] );
    outPho.phoSCE.push_back( inPho.phoSCE[maxIndex] );
    outPho.phoSCRawE.push_back( inPho.phoSCRawE[maxIndex] );
    outPho.phoESEn.push_back( inPho.phoESEn[maxIndex] );
    outPho.phoSCEta.push_back( inPho.phoSCEta[maxIndex] );
    outPho.phoSCPhi.push_back( inPho.phoSCPhi[maxIndex] );
    outPho.phoSCEtaWidth.push_back( inPho.phoSCEtaWidth[maxIndex] );
    outPho.phoSCPhiWidth.push_back( inPho.phoSCPhiWidth[maxIndex] );
    outPho.phoSCBrem.push_back( inPho.phoSCBrem[maxIndex] );
    outPho.phohasPixelSeed.push_back( inPho.phohasPixelSeed[maxIndex] );
    outPho.phoR9.push_back( inPho.phoR9[maxIndex] );
    outPho.phoHoverE.push_back( inPho.phoHoverE[maxIndex] );
    outPho.phoSigmaIEtaIEta.push_back( inPho.phoSigmaIEtaIEta[maxIndex] );
    //outPho.phoE1x3.push_back( inPho.phoE1x3[maxIndex] );
    //outPho.phoE2x2.push_back( inPho.phoE2x2[maxIndex] );
    //outPho.phoE3x3.push_back( inPho.phoE3x3[maxIndex] );
    //outPho.phoE2x5Max.push_back( inPho.phoE2x5Max[maxIndex] );
    //outPho.phoE1x5.push_back( inPho.phoE1x5[maxIndex] );
    //outPho.phoE2x5.push_back( inPho.phoE2x5[maxIndex] );
    //outPho.phoE5x5.push_back( inPho.phoE5x5[maxIndex] );
    //outPho.phoMaxEnergyXtal.push_back( inPho.phoMaxEnergyXtal[maxIndex] );
    //outPho.phoSigmaEtaEta.push_back( inPho.phoSigmaEtaEta[maxIndex] );
    //outPho.phoR1x5.push_back( inPho.phoR1x5[maxIndex] );
    //outPho.phoR2x5.push_back( inPho.phoR2x5[maxIndex] );
    //outPho.phoESEffSigmaRR.push_back( inPho.phoESEffSigmaRR[maxIndex] );
    outPho.phoSigmaIEtaIEta_2012.push_back( inPho.phoSigmaIEtaIEta_2012[maxIndex] );
    //outPho.phoSigmaIEtaIPhi_2012.push_back( inPho.phoSigmaIEtaIPhi_2012[maxIndex] );
    //outPho.phoSigmaIPhiIPhi_2012.push_back( inPho.phoSigmaIPhiIPhi_2012[maxIndex] );
    //outPho.phoE1x3_2012.push_back( inPho.phoE1x3_2012[maxIndex] );
    //outPho.phoE2x2_2012.push_back( inPho.phoE2x2_2012[maxIndex] );
    //outPho.phoE3x3_2012.push_back( inPho.phoE3x3_2012[maxIndex] );
    //outPho.phoE2x5Max_2012.push_back( inPho.phoE2x5Max_2012[maxIndex] );
    //outPho.phoE5x5_2012.push_back( inPho.phoE5x5_2012[maxIndex] );
    //outPho.phoBC1E.push_back( inPho.phoBC1E[maxIndex] );
    //outPho.phoBC1Eta.push_back( inPho.phoBC1Eta[maxIndex] );
    //outPho.phoBC2E.push_back( inPho.phoBC2E[maxIndex] );
    //outPho.phoBC2Eta.push_back( inPho.phoBC2Eta[maxIndex] );
    outPho.pho_ecalClusterIsoR2.push_back( inPho.pho_ecalClusterIsoR2[maxIndex] );
    outPho.pho_ecalClusterIsoR3.push_back( inPho.pho_ecalClusterIsoR3[maxIndex] );
    outPho.pho_ecalClusterIsoR4.push_back( inPho.pho_ecalClusterIsoR4[maxIndex] );
    outPho.pho_ecalClusterIsoR5.push_back( inPho.pho_ecalClusterIsoR5[maxIndex] );
    outPho.pho_hcalRechitIsoR1.push_back( inPho.pho_hcalRechitIsoR1[maxIndex] );
    outPho.pho_hcalRechitIsoR2.push_back( inPho.pho_hcalRechitIsoR2[maxIndex] );
    outPho.pho_hcalRechitIsoR3.push_back( inPho.pho_hcalRechitIsoR3[maxIndex] );
    outPho.pho_hcalRechitIsoR4.push_back( inPho.pho_hcalRechitIsoR4[maxIndex] );
    outPho.pho_hcalRechitIsoR5.push_back( inPho.pho_hcalRechitIsoR5[maxIndex] );
    outPho.pho_trackIsoR1PtCut20.push_back( inPho.pho_trackIsoR1PtCut20[maxIndex] );
    outPho.pho_trackIsoR2PtCut20.push_back( inPho.pho_trackIsoR2PtCut20[maxIndex] );
    outPho.pho_trackIsoR3PtCut20.push_back( inPho.pho_trackIsoR3PtCut20[maxIndex] );
    outPho.pho_trackIsoR4PtCut20.push_back( inPho.pho_trackIsoR4PtCut20[maxIndex] );
    outPho.pho_trackIsoR5PtCut20.push_back( inPho.pho_trackIsoR5PtCut20[maxIndex] );
    outPho.pho_swissCrx.push_back( inPho.pho_swissCrx[maxIndex] );
    outPho.pho_seedTime.push_back( inPho.pho_seedTime[maxIndex] );

    outPho.pfcIso1.push_back( inPho.pfcIso1[maxIndex] );
    outPho.pfcIso2.push_back( inPho.pfcIso2[maxIndex] );
    outPho.pfcIso3.push_back( inPho.pfcIso3[maxIndex] );
    outPho.pfcIso4.push_back( inPho.pfcIso4[maxIndex] );
    outPho.pfcIso5.push_back( inPho.pfcIso5[maxIndex] );

    outPho.pfpIso1.push_back( inPho.pfpIso1[maxIndex] );
    outPho.pfpIso2.push_back( inPho.pfpIso2[maxIndex] );
    outPho.pfpIso3.push_back( inPho.pfpIso3[maxIndex] );
    outPho.pfpIso4.push_back( inPho.pfpIso4[maxIndex] );
    outPho.pfpIso5.push_back( inPho.pfpIso5[maxIndex] );

    outPho.pfnIso1.push_back( inPho.pfnIso1[maxIndex] );
    outPho.pfnIso2.push_back( inPho.pfnIso2[maxIndex] );
    outPho.pfnIso3.push_back( inPho.pfnIso3[maxIndex] );
    outPho.pfnIso4.push_back( inPho.pfnIso4[maxIndex] );
    outPho.pfnIso5.push_back( inPho.pfnIso5[maxIndex] );

    outPho.pfcVsIso1.push_back( inPho.pfcVsIso1[maxIndex] );
    outPho.pfcVsIso2.push_back( inPho.pfcVsIso2[maxIndex] );
    outPho.pfcVsIso3.push_back( inPho.pfcVsIso3[maxIndex] );
    outPho.pfcVsIso4.push_back( inPho.pfcVsIso4[maxIndex] );
    outPho.pfcVsIso5.push_back( inPho.pfcVsIso5[maxIndex] );
    outPho.pfcVsIso1th1.push_back( inPho.pfcVsIso1th1[maxIndex] );
    outPho.pfcVsIso2th1.push_back( inPho.pfcVsIso2th1[maxIndex] );
    outPho.pfcVsIso3th1.push_back( inPho.pfcVsIso3th1[maxIndex] );
    outPho.pfcVsIso4th1.push_back( inPho.pfcVsIso4th1[maxIndex] );
    outPho.pfcVsIso5th1.push_back( inPho.pfcVsIso5th1[maxIndex] );
    outPho.pfcVsIso1th2.push_back( inPho.pfcVsIso1th2[maxIndex] );
    outPho.pfcVsIso2th2.push_back( inPho.pfcVsIso2th2[maxIndex] );
    outPho.pfcVsIso3th2.push_back( inPho.pfcVsIso3th2[maxIndex] );
    outPho.pfcVsIso4th2.push_back( inPho.pfcVsIso4th2[maxIndex] );
    outPho.pfcVsIso5th2.push_back( inPho.pfcVsIso5th2[maxIndex] );

    outPho.pfnVsIso1.push_back( inPho.pfnVsIso1[maxIndex] );
    outPho.pfnVsIso2.push_back( inPho.pfnVsIso2[maxIndex] );
    outPho.pfnVsIso3.push_back( inPho.pfnVsIso3[maxIndex] );
    outPho.pfnVsIso4.push_back( inPho.pfnVsIso4[maxIndex] );
    outPho.pfnVsIso5.push_back( inPho.pfnVsIso5[maxIndex] );
    outPho.pfnVsIso1th1.push_back( inPho.pfnVsIso1th1[maxIndex] );
    outPho.pfnVsIso2th1.push_back( inPho.pfnVsIso2th1[maxIndex] );
    outPho.pfnVsIso3th1.push_back( inPho.pfnVsIso3th1[maxIndex] );
    outPho.pfnVsIso4th1.push_back( inPho.pfnVsIso4th1[maxIndex] );
    outPho.pfnVsIso5th1.push_back( inPho.pfnVsIso5th1[maxIndex] );
    outPho.pfnVsIso1th2.push_back( inPho.pfnVsIso1th2[maxIndex] );
    outPho.pfnVsIso2th2.push_back( inPho.pfnVsIso2th2[maxIndex] );
    outPho.pfnVsIso3th2.push_back( inPho.pfnVsIso3th2[maxIndex] );
    outPho.pfnVsIso4th2.push_back( inPho.pfnVsIso4th2[maxIndex] );
    outPho.pfnVsIso5th2.push_back( inPho.pfnVsIso5th2[maxIndex] );


    outPho.pfpVsIso1.push_back( inPho.pfpVsIso1[maxIndex] );
    outPho.pfpVsIso2.push_back( inPho.pfpVsIso2[maxIndex] );
    outPho.pfpVsIso3.push_back( inPho.pfpVsIso3[maxIndex] );
    outPho.pfpVsIso4.push_back( inPho.pfpVsIso4[maxIndex] );
    outPho.pfpVsIso5.push_back( inPho.pfpVsIso5[maxIndex] );
    outPho.pfpVsIso1th1.push_back( inPho.pfpVsIso1th1[maxIndex] );
    outPho.pfpVsIso2th1.push_back( inPho.pfpVsIso2th1[maxIndex] );
    outPho.pfpVsIso3th1.push_back( inPho.pfpVsIso3th1[maxIndex] );
    outPho.pfpVsIso4th1.push_back( inPho.pfpVsIso4th1[maxIndex] );
    outPho.pfpVsIso5th1.push_back( inPho.pfpVsIso5th1[maxIndex] );
    outPho.pfpVsIso1th2.push_back( inPho.pfpVsIso1th2[maxIndex] );
    outPho.pfpVsIso2th2.push_back( inPho.pfpVsIso2th2[maxIndex] );
    outPho.pfpVsIso3th2.push_back( inPho.pfpVsIso3th2[maxIndex] );
    outPho.pfpVsIso4th2.push_back( inPho.pfpVsIso4th2[maxIndex] );
    outPho.pfpVsIso5th2.push_back( inPho.pfpVsIso5th2[maxIndex] );

    // outPho.towerIso1.push_back( inPho.towerIso1[maxIndex] );
    // outPho.towerIso2.push_back( inPho.towerIso2[maxIndex] );
    // outPho.towerIso3.push_back( inPho.towerIso3[maxIndex] );
    // outPho.towerIso4.push_back( inPho.towerIso4[maxIndex] );
    // outPho.towerIso5.push_back( inPho.towerIso5[maxIndex] );
    // outPho.towerVsIso1.push_back( inPho.towerVsIso1[maxIndex] );
    // outPho.towerVsIso2.push_back( inPho.towerVsIso2[maxIndex] );
    // outPho.towerVsIso3.push_back( inPho.towerVsIso3[maxIndex] );
    // outPho.towerVsIso4.push_back( inPho.towerVsIso4[maxIndex] );
    // outPho.towerVsIso5.push_back( inPho.towerVsIso5[maxIndex] );
    // outPho.towerVsSubIso1.push_back( inPho.towerVsSubIso1[maxIndex] );
    // outPho.towerVsSubIso2.push_back( inPho.towerVsSubIso2[maxIndex] );
    // outPho.towerVsSubIso3.push_back( inPho.towerVsSubIso3[maxIndex] );
    // outPho.towerVsSubIso4.push_back( inPho.towerVsSubIso4[maxIndex] );
    // outPho.towerVsSubIso5.push_back( inPho.towerVsSubIso5[maxIndex] );

    if(montecarlo){
      int genIndex = inPho.pho_genMatchedIndex[maxIndex];
      outPho.pho_genMatchedIndex.push_back( 0 ); // 'new' genIndex will be 0 since we only save the matched
      if(genIndex >= 0){
	outPho.nMC = 1;
	outPho.mcPID.push_back(  inPho.mcPID[genIndex] );
	outPho.mcStatus.push_back(  inPho.mcStatus[genIndex] );
	outPho.mcVtx_x.push_back(  inPho.mcVtx_x[genIndex] );
	outPho.mcVtx_y.push_back(  inPho.mcVtx_y[genIndex] );
	outPho.mcVtx_z.push_back(  inPho.mcVtx_z[genIndex] );
	outPho.mcPt.push_back(  inPho.mcPt[genIndex] );
	outPho.mcEta.push_back(  inPho.mcEta[genIndex] );
	outPho.mcPhi.push_back(  inPho.mcPhi[genIndex] );
	outPho.mcE.push_back(  inPho.mcE[genIndex] );
	outPho.mcEt.push_back(  inPho.mcEt[genIndex] );
	outPho.mcMass.push_back(  inPho.mcMass[genIndex] );
	outPho.mcParentage.push_back(  inPho.mcParentage[genIndex] );
	outPho.mcMomPID.push_back(  inPho.mcMomPID[genIndex] );
	outPho.mcMomPt.push_back(  inPho.mcMomPt[genIndex] );
	outPho.mcMomEta.push_back(  inPho.mcMomEta[genIndex] );
	outPho.mcMomPhi.push_back(  inPho.mcMomPhi[genIndex] );
	outPho.mcMomMass.push_back(  inPho.mcMomMass[genIndex] );
	outPho.mcGMomPID.push_back(  inPho.mcGMomPID[genIndex] );
	outPho.mcIndex.push_back(  inPho.mcIndex[genIndex] );
	outPho.mcCalIsoDR03.push_back(  inPho.mcCalIsoDR03[genIndex] );
	outPho.mcCalIsoDR04.push_back(  inPho.mcCalIsoDR04[genIndex] );
	outPho.mcTrkIsoDR03.push_back(  inPho.mcTrkIsoDR03[genIndex] );
	outPho.mcTrkIsoDR04.push_back(  inPho.mcTrkIsoDR04[genIndex] );
      }
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
