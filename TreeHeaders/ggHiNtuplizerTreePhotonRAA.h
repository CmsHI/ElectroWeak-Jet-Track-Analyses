#ifndef TREEHEADERS_GGHINTUPLIZERTREE_PHOTONRAA_H_
#define TREEHEADERS_GGHINTUPLIZERTREE_PHOTONRAA_H_

#include <TTree.h>
#include <TBranch.h>

#include <vector>

class ggHiNtuplizer {
public :
  ggHiNtuplizer() {
    nPU = 0;
    puBX = 0;
    puTrue = 0;
    mcPID = 0;
    mcStatus = 0;
    mcVtx_x = 0;
    mcVtx_y = 0;
    mcVtx_z = 0;
    mcPt = 0;
    mcEta = 0;
    mcPhi = 0;
    mcE = 0;
    mcEt = 0;
    mcMass = 0;
    mcParentage = 0;
    mcMomPID = 0;
    mcMomPt = 0;
    mcMomEta = 0;
    mcMomPhi = 0;
    mcMomMass = 0;
    mcGMomPID = 0;
    mcIndex = 0;
    mcCalIsoDR03 = 0;
    mcCalIsoDR04 = 0;
    mcTrkIsoDR03 = 0;
    mcTrkIsoDR04 = 0;
    //pho gen matched information
    pho_genMatchedIndex = 0;
    pho_genPID = 0;
    pho_genStatus = 0;
    pho_genPt = 0;
    pho_genEta = 0;
    pho_genPhi = 0;
    pho_genE = 0;
    pho_genEt = 0;
    pho_genMomPID = 0;
    pho_genCalIsoDR03 = 0;
    pho_genCalIsoDR04 = 0;
    pho_genTrkIsoDR03 = 0;
    pho_genTrkIsoDR04 = 0;
    
    eleCharge = 0;
    eleChargeConsistent = 0;
    eleSCPixCharge = 0;
    eleCtfCharge = 0;
    eleEn = 0;
    eleD0 = 0;
    eleDz = 0;
    eleD0Err = 0;
    eleDzErr = 0;
    eleTrkPt = 0;
    eleTrkEta = 0;
    eleTrkPhi = 0;
    eleTrkCharge = 0;
    eleTrkChi2 = 0;
    eleTrkNdof = 0;
    eleTrkNormalizedChi2 = 0;
    eleTrkValidHits = 0;
    eleTrkLayers = 0;
    elePt = 0;
    eleEta = 0;
    elePhi = 0;
    eleSCEn = 0;
    eleESEn = 0;
    eleSCEta = 0;
    eleSCPhi = 0;
    eleSCRawEn = 0;
    eleSCEtaWidth = 0;
    eleSCPhiWidth = 0;
    eleHoverE = 0;
    eleHoverEBc = 0;
    eleEoverP = 0;
    eleEoverPInv = 0;
    eleBrem = 0;
    eledEtaAtVtx = 0;
    eledPhiAtVtx = 0;
    eleSigmaIEtaIEta = 0;
    eleSigmaIEtaIEta_2012 = 0;
    eleSigmaIPhiIPhi = 0;
    eleMissHits = 0;
    eleESEffSigmaRR = 0;
    elePFChIso = 0;
    elePFPhoIso = 0;
    elePFNeuIso = 0;
    elePFPUIso = 0;
    elePFChIso03 = 0;
    elePFPhoIso03 = 0;
    elePFNeuIso03 = 0;
    elePFChIso04 = 0;
    elePFPhoIso04 = 0;
    elePFNeuIso04 = 0;
    eleR9 = 0;
    eleE3x3 = 0;
    eleE5x5 = 0;
    eleR9Full5x5 = 0;
    eleE3x3Full5x5 = 0;
    eleE5x5Full5x5 = 0;
    NClusters = 0;
    NEcalClusters = 0;
    eleSeedEn = 0;
    eleSeedEta = 0;
    eleSeedPhi = 0;
    eleSeedCryEta = 0;
    eleSeedCryPhi = 0;
    eleSeedCryIeta = 0;
    eleSeedCryIphi = 0;
    eleBC1E = 0;
    eleBC1Eta = 0;
    eleBC2E = 0;
    eleBC2Eta = 0;
    eleIDVeto = 0;
    eleIDLoose = 0;
    eleIDMedium = 0;
    eleIDTight = 0;
    elepassConversionVeto = 0;
    eleEffAreaTimesRho = 0;
    phoE = 0;
    phoEt = 0;
    phoPreScale = 0;
    phoEtCorrected = 0;
    phoEtCorrected_sys = 0;
    phoEtCorrected_resSys_sig = 0;
    phoEtCorrected_resSys_sig2 = 0;
    phoEtCorrected_resSys_rms = 0;
    phoEtCorrected_resSys_rms2 = 0;
    phoEtCorrected_resSys_up = 0;
    phoEtCorrected_resSys_down = 0;
    phoEta = 0;
    phoPhi = 0;
  phoSCE = 0;   //!
  phoSCRawE = 0;   //!
  phoSCEta = 0;   //!
  phoSCPhi = 0;   //!
  phoSCEtaWidth = 0;   //!
  phoSCPhiWidth = 0;   //!
  phoSCBrem = 0;   //!
  phoSCnHits = 0;   //!
  phoSCflags = 0;   //!
  phoSCinClean = 0;   //!
  phoSCinUnClean = 0;   //!
  phoSCnBC = 0;   //!
  phoESEn = 0;   //!
  phoPSCE = 0;   //!
  phoPSCRawE = 0;   //!
  phoPSCEta = 0;   //!
  phoPSCPhi = 0;   //!
  phoPSCEtaWidth = 0;   //!
  phoPSCPhiWidth = 0;   //!
  phoPSCBrem = 0;   //!
  phoPSCnHits = 0;   //!
  phoPSCflags = 0;   //!
  phoPSCinClean = 0;   //!
  phoPSCinUnClean = 0;   //!
  phoPSCnBC = 0;   //!
  phoPESEn = 0;   //!
  phoIsPFPhoton = 0;   //!
  phoIsStandardPhoton = 0;   //!
  phoHasPixelSeed = 0;   //!
  phoHasConversionTracks = 0;
  phoR9 = 0;   //!
  phoHoverE = 0;   //!
  phoHadTowerOverEm = 0;   //!
  phoSigmaIEtaIEta = 0;   //!
  pho_isEle = 0;   //!
  pho_is2015Noise = 0;   //!e
  phoE1x5 = 0;   //!
  phoE2x5 = 0;   //!
  phoE3x3 = 0;   //!
  phoE5x5 = 0;   //!
  phoMaxEnergyXtal = 0;   //!
  phoSigmaEtaEta = 0;   //!
  phoR1x5 = 0;   //!
  phoR2x5 = 0;   //!
  phoESEffSigmaRR = 0;   //!
  phoE1x5_2012 = 0;   //!
  phoE2x5_2012 = 0;   //!
  phoE3x3_2012 = 0;   //!
  phoE5x5_2012 = 0;   //!
  phoR1x5_2012 = 0;   //!
  phoR2x5_2012 = 0;   //!
  phoMaxEnergyXtal_2012 = 0;   //!
  phoSigmaIEtaIEta_2012 = 0;   //!
  phoE3x3_2012 = 0;   //!
  phoBC1E = 0;   //!
  phoBC1Ecorr = 0;   //!
  phoBC1Eta = 0;   //!
  phoBC1Phi = 0;   //!
  phoBC1size = 0;   //!
  phoBC1flags = 0;   //!
  phoBC1inClean = 0;   //!
  phoBC1inUnClean = 0;   //!
  phoBC1rawID = 0;   //!
  phoBC2E = 0;   //!
  phoBC2Eta = 0;   //!
  phoBC2Phi = 0;   //!
  phoE1x3 = 0;   //!
  phoE2x2 = 0;   //!
  phoE2x5Max = 0;   //!
  phoSigmaIEtaIPhi_2012 = 0;   //!
  phoSigmaIPhiIPhi_2012 = 0;   //!
  phoE1x3_2012 = 0;   //!
  phoE2x2_2012 = 0;   //!
  phoE2x5Max_2012 = 0;   //!
  phoE5x5_2012 = 0;   //!
    pho_ecalClusterIsoR2 = 0;
    pho_ecalClusterIsoR3 = 0;
    pho_ecalClusterIsoR4 = 0;
    pho_ecalClusterIsoR5 = 0;
    pho_hcalRechitIsoR1 = 0;
    pho_hcalRechitIsoR2 = 0;
    pho_hcalRechitIsoR3 = 0;
    pho_hcalRechitIsoR4 = 0;
    pho_hcalRechitIsoR5 = 0;
    pho_trackIsoR1PtCut20 = 0;
    pho_trackIsoR2PtCut20 = 0;
    pho_trackIsoR3PtCut20 = 0;
    pho_trackIsoR4PtCut20 = 0;
    pho_trackIsoR5PtCut20 = 0;
    pho_sumIsoCorrected = 0;
    pho_sumIso = 0;
    pho_swissCrx = 0;
    pho_seedTime = 0;
    pfcIso1 = 0;
    pfcIso2 = 0;
    pfcIso3 = 0;
    pfcIso4 = 0;
    pfcIso5 = 0;
    pfpIso1 = 0;
    pfpIso2 = 0;
    pfpIso3 = 0;
    pfpIso4 = 0;
    pfpIso5 = 0;
    pfnIso1 = 0;
    pfnIso2 = 0;
    pfnIso3 = 0;
    pfnIso4 = 0;
    pfnIso5 = 0;
    pfcVsIso1 = 0;
    pfcVsIso2 = 0;
    pfcVsIso3 = 0;
    pfcVsIso4 = 0;
    pfcVsIso5 = 0;
    pfcVsIso1th1 = 0;
    pfcVsIso2th1 = 0;
    pfcVsIso3th1 = 0;
    pfcVsIso4th1 = 0;
    pfcVsIso5th1 = 0;
    pfcVsIso1th2 = 0;
    pfcVsIso2th2 = 0;
    pfcVsIso3th2 = 0;
    pfcVsIso4th2 = 0;
    pfcVsIso5th2 = 0;
    pfnVsIso1 = 0;
    pfnVsIso2 = 0;
    pfnVsIso3 = 0;
    pfnVsIso4 = 0;
    pfnVsIso5 = 0;
    pfnVsIso1th1 = 0;
    pfnVsIso2th1 = 0;
    pfnVsIso3th1 = 0;
    pfnVsIso4th1 = 0;
    pfnVsIso5th1 = 0;
    pfnVsIso1th2 = 0;
    pfnVsIso2th2 = 0;
    pfnVsIso3th2 = 0;
    pfnVsIso4th2 = 0;
    pfnVsIso5th2 = 0;
    pfpVsIso1 = 0;
    pfpVsIso2 = 0;
    pfpVsIso3 = 0;
    pfpVsIso4 = 0;
    pfpVsIso5 = 0;
    pfpVsIso1th1 = 0;
    pfpVsIso2th1 = 0;
    pfpVsIso3th1 = 0;
    pfpVsIso4th1 = 0;
    pfpVsIso5th1 = 0;
    pfpVsIso1th2 = 0;
    pfpVsIso2th2 = 0;
    pfpVsIso3th2 = 0;
    pfpVsIso4th2 = 0;
    pfpVsIso5th2 = 0;
    towerIso1 = 0;
    towerIso2 = 0;
    towerIso3 = 0;
    towerIso4 = 0;
    towerIso5 = 0;
    towerVsIso1 = 0;
    towerVsIso2 = 0;
    towerVsIso3 = 0;
    towerVsIso4 = 0;
    towerVsIso5 = 0;
    towerVsSubIso1 = 0;
    towerVsSubIso2 = 0;
    towerVsSubIso3 = 0;
    towerVsSubIso4 = 0;
    towerVsSubIso5 = 0;

  };
  ~ggHiNtuplizer(){};
  void setupTreeForReading(TTree *t);

  // Declaration of leaf types
  UInt_t          run;
  ULong64_t       event;
  UInt_t          lumis;
  Bool_t          isData;
  Int_t           nPUInfo;
  std::vector<int>     *nPU;
  std::vector<int>     *puBX;
  std::vector<float>   *puTrue;
  Int_t           nMC;
  std::vector<int>     *mcPID;
  std::vector<int>     *mcStatus;
  std::vector<float>   *mcVtx_x;
  std::vector<float>   *mcVtx_y;
  std::vector<float>   *mcVtx_z;
  std::vector<float>   *mcPt;
  std::vector<float>   *mcEta;
  std::vector<float>   *mcPhi;
  std::vector<float>   *mcE;
  std::vector<float>   *mcEt;
  std::vector<float>   *mcMass;
  std::vector<int>     *mcParentage;
  std::vector<int>     *mcMomPID;
  std::vector<float>   *mcMomPt;
  std::vector<float>   *mcMomEta;
  std::vector<float>   *mcMomPhi;
  std::vector<float>   *mcMomMass;
  std::vector<int>     *mcGMomPID;
  std::vector<int>     *mcIndex;
  std::vector<float>   *mcCalIsoDR03;
  std::vector<float>   *mcCalIsoDR04;
  std::vector<float>   *mcTrkIsoDR03;
  std::vector<float>   *mcTrkIsoDR04;
  std::vector<int>     *pho_genMatchedIndex;
  std::vector<int>     *pho_genPID;
  std::vector<int>     *pho_genStatus;
  std::vector<float>   *pho_genPt;
  std::vector<float>   *pho_genEta;
  std::vector<float>   *pho_genPhi;
  std::vector<float>   *pho_genE;
  std::vector<float>   *pho_genEt;
  std::vector<int>     *pho_genMomPID;
  std::vector<float>   *pho_genCalIsoDR03;
  std::vector<float>   *pho_genCalIsoDR04;
  std::vector<float>   *pho_genTrkIsoDR03;
  std::vector<float>   *pho_genTrkIsoDR04;
  Int_t           nEle;
  std::vector<int>     *eleCharge;
  std::vector<int>     *eleChargeConsistent;
  std::vector<int>     *eleSCPixCharge;
  std::vector<int>     *eleCtfCharge;
  std::vector<float>   *eleEn;
  std::vector<float>   *eleD0;
  std::vector<float>   *eleDz;
  std::vector<float>   *eleD0Err;
  std::vector<float>   *eleDzErr;
  std::vector<float>   *eleTrkPt;
  std::vector<float>   *eleTrkEta;
  std::vector<float>   *eleTrkPhi;
  std::vector<int>     *eleTrkCharge;
  std::vector<float>   *eleTrkChi2;
  std::vector<float>   *eleTrkNdof;
  std::vector<float>   *eleTrkNormalizedChi2;
  std::vector<int>     *eleTrkValidHits;
  std::vector<int>     *eleTrkLayers;
  std::vector<float>   *elePt;
  std::vector<float>   *eleEta;
  std::vector<float>   *elePhi;
  std::vector<float>   *eleSCEn;
  std::vector<float>   *eleESEn;
  std::vector<float>   *eleSCEta;
  std::vector<float>   *eleSCPhi;
  std::vector<float>   *eleSCRawEn;
  std::vector<float>   *eleSCEtaWidth;
  std::vector<float>   *eleSCPhiWidth;
  std::vector<float>   *eleHoverE;
  std::vector<float>   *eleHoverEBc;
  std::vector<float>   *eleEoverP;
  std::vector<float>   *eleEoverPInv;
  std::vector<float>   *eleBrem;
  std::vector<float>   *eledEtaAtVtx;
  std::vector<float>   *eledPhiAtVtx;
  std::vector<float>   *eleSigmaIEtaIEta;
  std::vector<float>   *eleSigmaIEtaIEta_2012;
  std::vector<float>   *eleSigmaIPhiIPhi;
  std::vector<int>     *eleMissHits;
  std::vector<float>   *eleESEffSigmaRR;
  std::vector<float>   *elePFChIso;
  std::vector<float>   *elePFPhoIso;
  std::vector<float>   *elePFNeuIso;
  std::vector<float>   *elePFPUIso;
  std::vector<float>   *elePFChIso03;
  std::vector<float>   *elePFPhoIso03;
  std::vector<float>   *elePFNeuIso03;
  std::vector<float>   *elePFChIso04;
  std::vector<float>   *elePFPhoIso04;
  std::vector<float>   *elePFNeuIso04;
  std::vector<float>   *eleR9;
  std::vector<float>   *eleE3x3;
  std::vector<float>   *eleE5x5;
  std::vector<float>   *eleR9Full5x5;
  std::vector<float>   *eleE3x3Full5x5;
  std::vector<float>   *eleE5x5Full5x5;
  std::vector<int>     *NClusters;
  std::vector<int>     *NEcalClusters;
  std::vector<float>   *eleSeedEn;
  std::vector<float>   *eleSeedEta;
  std::vector<float>   *eleSeedPhi;
  std::vector<float>   *eleSeedCryEta;
  std::vector<float>   *eleSeedCryPhi;
  std::vector<float>   *eleSeedCryIeta;
  std::vector<float>   *eleSeedCryIphi;
  std::vector<float>   *eleBC1E;
  std::vector<float>   *eleBC1Eta;
  std::vector<float>   *eleBC2E;
  std::vector<float>   *eleBC2Eta;
  std::vector<int>     *eleIDVeto;
  std::vector<int>     *eleIDLoose;
  std::vector<int>     *eleIDMedium;
  std::vector<int>     *eleIDTight;
  std::vector<int>     *elepassConversionVeto;
  std::vector<float>   *eleEffAreaTimesRho;
  Int_t           nPho;
  std::vector<float>   *phoE;
  std::vector<float>   *phoEt;
  std::vector<int>   *phoPreScale;
  std::vector<float>   *phoEtCorrected;
  std::vector<float>   *phoEtCorrected_sys;
  std::vector<float>   *phoEtCorrected_resSys_sig;
  std::vector<float>   *phoEtCorrected_resSys_sig2;
  std::vector<float>   *phoEtCorrected_resSys_rms;
  std::vector<float>   *phoEtCorrected_resSys_rms2;
  std::vector<float>   *phoEtCorrected_resSys_up;
  std::vector<float>   *phoEtCorrected_resSys_down;
  std::vector<float>   *phoEta;
  std::vector<float>   *phoPhi;
  std::vector<float>   *phoSCE;   //!
  std::vector<float>   *phoSCRawE;   //!
  std::vector<float>   *phoSCEta;   //!
  std::vector<float>   *phoSCPhi;   //!
  std::vector<float>   *phoSCEtaWidth;   //!
  std::vector<float>   *phoSCPhiWidth;   //!
  std::vector<float>   *phoSCBrem;   //!
  std::vector<int>   *phoSCnHits;   //!
  std::vector<unsigned int>   *phoSCflags;   //!
  std::vector<int>   *phoSCinClean;   //!
  std::vector<int>   *phoSCinUnClean;   //!
  std::vector<int>   *phoSCnBC;   //!
  std::vector<float>   *phoESEn;   //!
  std::vector<float>   *phoPSCE;   //!
  std::vector<float>   *phoPSCRawE;   //!
  std::vector<float>   *phoPSCEta;   //!
  std::vector<float>   *phoPSCPhi;   //!
  std::vector<float>   *phoPSCEtaWidth;   //!
  std::vector<float>   *phoPSCPhiWidth;   //!
  std::vector<float>   *phoPSCBrem;   //!
  std::vector<int>   *phoPSCnHits;   //!
  std::vector<unsigned int>   *phoPSCflags;   //!
  std::vector<int>   *phoPSCinClean;   //!
  std::vector<int>   *phoPSCinUnClean;   //!
  std::vector<int>   *phoPSCnBC;   //!
  std::vector<float>   *phoPESEn;   //!
  std::vector<int>   *phoIsPFPhoton;   //!
  std::vector<int>   *phoIsStandardPhoton;   //!
  std::vector<int>   *phoHasPixelSeed;   //!
  std::vector<int>   *phoHasConversionTracks;
  std::vector<float>   *phoR9;   //!
  std::vector<float>   *phoHoverE;   //!
  std::vector<float>   *phoHadTowerOverEm;   //!
  std::vector<float>   *phoSigmaIEtaIEta;   //!
  std::vector<int>   *pho_isEle;   //!
  std::vector<int>   *pho_is2015Noise;   //!
  std::vector<float>   *phoE1x5;   //!
  std::vector<float>   *phoE2x5;   //!
  std::vector<float>   *phoE3x3;   //!
  std::vector<float>   *phoE5x5;   //!
  std::vector<float>   *phoMaxEnergyXtal;   //!
  std::vector<float>   *phoSigmaEtaEta;   //!
  std::vector<float>   *phoR1x5;   //!
  std::vector<float>   *phoR2x5;   //!
  std::vector<float>   *phoESEffSigmaRR;   //!
  std::vector<float>   *phoE1x5_2012;   //!
  std::vector<float>   *phoE2x5_2012;   //!
  std::vector<float>   *phoE3x3_2012;   //!
  std::vector<float>   *phoE5x5_2012;   //!
  std::vector<float>   *phoR1x5_2012;   //!
  std::vector<float>   *phoR2x5_2012;   //!
  std::vector<float>   *phoMaxEnergyXtal_2012;   //!
  std::vector<float>   *phoSigmaIEtaIEta_2012;   //!
  std::vector<float>   *phoBC1E;   //!
  std::vector<float>   *phoBC1Ecorr;   //!
  std::vector<float>   *phoBC1Eta;   //!
  std::vector<float>   *phoBC1Phi;   //!
  std::vector<int>   *phoBC1size;   //!
  std::vector<unsigned int>   *phoBC1flags;   //!
  std::vector<int>   *phoBC1inClean;   //!
  std::vector<int>   *phoBC1inUnClean;   //!
  std::vector<unsigned int>   *phoBC1rawID;   //!
  std::vector<float>   *phoBC2E;   //!
  std::vector<float>   *phoBC2Eta;   //!
  std::vector<float>   *phoBC2Phi;   //!
  std::vector<float>   *phoE1x3;   //!
  std::vector<float>   *phoE2x2;   //!
  std::vector<float>   *phoE2x5Max;   //!
  std::vector<float>   *phoE1x3_2012;   //!
  std::vector<float>   *phoE2x2_2012;   //!
  std::vector<float>   *phoE2x5Max_2012;   //!
  std::vector<float>   *phoSigmaIEtaIPhi_2012;   //!
  std::vector<float>   *phoSigmaIPhiIPhi_2012;   //!
  std::vector<float>   *pho_ecalClusterIsoR2;
  std::vector<float>   *pho_ecalClusterIsoR3;
  std::vector<float>   *pho_ecalClusterIsoR4;
  std::vector<float>   *pho_ecalClusterIsoR5;
  std::vector<float>   *pho_hcalRechitIsoR1;
  std::vector<float>   *pho_hcalRechitIsoR2;
  std::vector<float>   *pho_hcalRechitIsoR3;
  std::vector<float>   *pho_hcalRechitIsoR4;
  std::vector<float>   *pho_hcalRechitIsoR5;
  std::vector<float>   *pho_trackIsoR1PtCut20;
  std::vector<float>   *pho_trackIsoR2PtCut20;
  std::vector<float>   *pho_trackIsoR3PtCut20;
  std::vector<float>   *pho_trackIsoR4PtCut20;
  std::vector<float>   *pho_trackIsoR5PtCut20;
  std::vector<float>   *pho_sumIsoCorrected;
  std::vector<float>   *pho_sumIso;
  std::vector<float>   *pho_swissCrx;
  std::vector<float>   *pho_seedTime;
  std::vector<float>   *pfcIso1;
  std::vector<float>   *pfcIso2;
  std::vector<float>   *pfcIso3;
  std::vector<float>   *pfcIso4;
  std::vector<float>   *pfcIso5;
  std::vector<float>   *pfpIso1;
  std::vector<float>   *pfpIso2;
  std::vector<float>   *pfpIso3;
  std::vector<float>   *pfpIso4;
  std::vector<float>   *pfpIso5;
  std::vector<float>   *pfnIso1;
  std::vector<float>   *pfnIso2;
  std::vector<float>   *pfnIso3;
  std::vector<float>   *pfnIso4;
  std::vector<float>   *pfnIso5;
  std::vector<float>   *pfcVsIso1;
  std::vector<float>   *pfcVsIso2;
  std::vector<float>   *pfcVsIso3;
  std::vector<float>   *pfcVsIso4;
  std::vector<float>   *pfcVsIso5;
  std::vector<float>   *pfcVsIso1th1;
  std::vector<float>   *pfcVsIso2th1;
  std::vector<float>   *pfcVsIso3th1;
  std::vector<float>   *pfcVsIso4th1;
  std::vector<float>   *pfcVsIso5th1;
  std::vector<float>   *pfcVsIso1th2;
  std::vector<float>   *pfcVsIso2th2;
  std::vector<float>   *pfcVsIso3th2;
  std::vector<float>   *pfcVsIso4th2;
  std::vector<float>   *pfcVsIso5th2;
  std::vector<float>   *pfnVsIso1;
  std::vector<float>   *pfnVsIso2;
  std::vector<float>   *pfnVsIso3;
  std::vector<float>   *pfnVsIso4;
  std::vector<float>   *pfnVsIso5;
  std::vector<float>   *pfnVsIso1th1;
  std::vector<float>   *pfnVsIso2th1;
  std::vector<float>   *pfnVsIso3th1;
  std::vector<float>   *pfnVsIso4th1;
  std::vector<float>   *pfnVsIso5th1;
  std::vector<float>   *pfnVsIso1th2;
  std::vector<float>   *pfnVsIso2th2;
  std::vector<float>   *pfnVsIso3th2;
  std::vector<float>   *pfnVsIso4th2;
  std::vector<float>   *pfnVsIso5th2;
  std::vector<float>   *pfpVsIso1;
  std::vector<float>   *pfpVsIso2;
  std::vector<float>   *pfpVsIso3;
  std::vector<float>   *pfpVsIso4;
  std::vector<float>   *pfpVsIso5;
  std::vector<float>   *pfpVsIso1th1;
  std::vector<float>   *pfpVsIso2th1;
  std::vector<float>   *pfpVsIso3th1;
  std::vector<float>   *pfpVsIso4th1;
  std::vector<float>   *pfpVsIso5th1;
  std::vector<float>   *pfpVsIso1th2;
  std::vector<float>   *pfpVsIso2th2;
  std::vector<float>   *pfpVsIso3th2;
  std::vector<float>   *pfpVsIso4th2;
  std::vector<float>   *pfpVsIso5th2;
  std::vector<float>   *towerIso1;
  std::vector<float>   *towerIso2;
  std::vector<float>   *towerIso3;
  std::vector<float>   *towerIso4;
  std::vector<float>   *towerIso5;
  std::vector<float>   *towerVsIso1;
  std::vector<float>   *towerVsIso2;
  std::vector<float>   *towerVsIso3;
  std::vector<float>   *towerVsIso4;
  std::vector<float>   *towerVsIso5;
  std::vector<float>   *towerVsSubIso1;
  std::vector<float>   *towerVsSubIso2;
  std::vector<float>   *towerVsSubIso3;
  std::vector<float>   *towerVsSubIso4;
  std::vector<float>   *towerVsSubIso5;

  // List of branches
  TBranch        *b_run;   //!
  TBranch        *b_event;   //!
  TBranch        *b_lumis;   //!
  TBranch        *b_isData;   //!
  TBranch        *b_nPUInfo;   //!
  TBranch        *b_nPU;   //!
  TBranch        *b_puBX;   //!
  TBranch        *b_puTrue;   //!
  TBranch        *b_nMC;   //!
  TBranch        *b_mcPID;   //!
  TBranch        *b_mcStatus;   //!
  TBranch        *b_mcVtx_x;   //!
  TBranch        *b_mcVtx_y;   //!
  TBranch        *b_mcVtx_z;   //!
  TBranch        *b_mcPt;   //!
  TBranch        *b_mcEta;   //!
  TBranch        *b_mcPhi;   //!
  TBranch        *b_mcE;   //!
  TBranch        *b_mcEt;   //!
  TBranch        *b_mcMass;   //!
  TBranch        *b_mcParentage;   //!
  TBranch        *b_mcMomPID;   //!
  TBranch        *b_mcMomPt;   //!
  TBranch        *b_mcMomEta;   //!
  TBranch        *b_mcMomPhi;   //!
  TBranch        *b_mcMomMass;   //!
  TBranch        *b_mcGMomPID;   //!
  TBranch        *b_mcIndex;   //!
  TBranch        *b_mcCalIsoDR03;   //!
  TBranch        *b_mcCalIsoDR04;   //!
  TBranch        *b_mcTrkIsoDR03;   //!
  TBranch        *b_mcTrkIsoDR04;   //!
  TBranch        *b_pho_genMatchedIndex;
  TBranch        *b_pho_genPID;
  TBranch        *b_pho_genStatus;
  TBranch        *b_pho_genPt;
  TBranch        *b_pho_genEta;
  TBranch        *b_pho_genPhi;
  TBranch        *b_pho_genE;
  TBranch        *b_pho_genEt;
  TBranch        *b_pho_genMomPID;
  TBranch        *b_pho_genCalIsoDR03;
  TBranch        *b_pho_genCalIsoDR04;
  TBranch        *b_pho_genTrkIsoDR03;
  TBranch        *b_pho_genTrkIsoDR04;
  TBranch        *b_nEle;   //!
  TBranch        *b_eleCharge;   //!
  TBranch        *b_eleChargeConsistent;   //!
  TBranch        *b_eleSCPixCharge;   //!
  TBranch        *b_eleCtfCharge;   //!
  TBranch        *b_eleEn;   //!
  TBranch        *b_eleD0;   //!
  TBranch        *b_eleDz;   //!
  TBranch        *b_eleD0Err;   //!
  TBranch        *b_eleDzErr;   //!
  TBranch        *b_eleTrkPt;   //!
  TBranch        *b_eleTrkEta;   //!
  TBranch        *b_eleTrkPhi;   //!
  TBranch        *b_eleTrkCharge;   //!
  TBranch        *b_eleTrkChi2;   //!
  TBranch        *b_eleTrkNdof;   //!
  TBranch        *b_eleTrkNormalizedChi2;   //!
  TBranch        *b_eleTrkValidHits;   //!
  TBranch        *b_eleTrkLayers;   //!
  TBranch        *b_elePt;   //!
  TBranch        *b_eleEta;   //!
  TBranch        *b_elePhi;   //!
  TBranch        *b_eleSCEn;   //!
  TBranch        *b_eleESEn;   //!
  TBranch        *b_eleSCEta;   //!
  TBranch        *b_eleSCPhi;   //!
  TBranch        *b_eleSCRawEn;   //!
  TBranch        *b_eleSCEtaWidth;   //!
  TBranch        *b_eleSCPhiWidth;   //!
  TBranch        *b_eleHoverE;   //!
  TBranch        *b_eleHoverEBc;   //!
  TBranch        *b_eleEoverP;   //!
  TBranch        *b_eleEoverPInv;   //!
  TBranch        *b_eleBrem;   //!
  TBranch        *b_eledEtaAtVtx;   //!
  TBranch        *b_eledPhiAtVtx;   //!
  TBranch        *b_eleSigmaIEtaIEta;   //!
  TBranch        *b_eleSigmaIEtaIEta_2012;   //!
  TBranch        *b_eleSigmaIPhiIPhi;   //!
  TBranch        *b_eleMissHits;   //!
  TBranch        *b_eleESEffSigmaRR;   //!
  TBranch        *b_elePFChIso;   //!
  TBranch        *b_elePFPhoIso;   //!
  TBranch        *b_elePFNeuIso;   //!
  TBranch        *b_elePFPUIso;   //!
  TBranch        *b_elePFChIso03;   //!
  TBranch        *b_elePFPhoIso03;   //!
  TBranch        *b_elePFNeuIso03;   //!
  TBranch        *b_elePFChIso04;   //!
  TBranch        *b_elePFPhoIso04;   //!
  TBranch        *b_elePFNeuIso04;   //!
  TBranch        *b_eleR9;   //!
  TBranch        *b_eleE3x3;   //!
  TBranch        *b_eleE5x5;   //!
  TBranch        *b_eleR9Full5x5;   //!
  TBranch        *b_eleE3x3Full5x5;   //!
  TBranch        *b_eleE5x5Full5x5;   //!
  TBranch        *b_NClusters;   //!
  TBranch        *b_NEcalClusters;   //!
  TBranch        *b_eleSeedEn;   //!
  TBranch        *b_eleSeedEta;   //!
  TBranch        *b_eleSeedPhi;   //!
  TBranch        *b_eleSeedCryEta;   //!
  TBranch        *b_eleSeedCryPhi;   //!
  TBranch        *b_eleSeedCryIeta;   //!
  TBranch        *b_eleSeedCryIphi;   //!
  TBranch        *b_eleBC1E;   //!
  TBranch        *b_eleBC1Eta;   //!
  TBranch        *b_eleBC2E;   //!
  TBranch        *b_eleBC2Eta;   //!
  TBranch        *b_eleIDVeto;   //!
  TBranch        *b_eleIDLoose;   //!
  TBranch        *b_eleIDMedium;   //!
  TBranch        *b_eleIDTight;   //!
  TBranch        *b_elepassConversionVeto;   //!
  TBranch        *b_eleEffAreaTimesRho;   //!
  TBranch        *b_nPho;   //!
  TBranch        *b_phoE;   //!
  TBranch        *b_phoEt;   //!
  TBranch        *b_phoPreScale; //!
  TBranch        *b_phoEtCorrected;   //!
  TBranch        *b_phoEtCorrected_sys;   //!
  TBranch        *b_phoEtCorrected_resSys_sig;   //!
  TBranch        *b_phoEtCorrected_resSys_sig2;   //!
  TBranch        *b_phoEtCorrected_resSys_rms;   //!
  TBranch        *b_phoEtCorrected_resSys_rms2;   //!
  TBranch        *b_phoEtCorrected_resSys_up;   //!
  TBranch        *b_phoEtCorrected_resSys_down;   //!
  TBranch        *b_phoEta;   //!
  TBranch        *b_phoPhi;   //!
    TBranch        *b_phoSCE;   //!
    TBranch        *b_phoSCRawE;   //!
    TBranch        *b_phoSCEta;   //!
    TBranch        *b_phoSCPhi;   //!
    TBranch        *b_phoSCEtaWidth;   //!
    TBranch        *b_phoSCPhiWidth;   //!
    TBranch        *b_phoSCBrem;   //!
    TBranch        *b_phoSCnHits;   //!
    TBranch        *b_phoSCflags;   //!
    TBranch        *b_phoSCinClean;   //!
    TBranch        *b_phoSCinUnClean;   //!
    TBranch        *b_phoSCnBC;   //!
    TBranch        *b_phoESEn;   //!
    TBranch        *b_phoPSCE;   //!
    TBranch        *b_phoPSCRawE;   //!
    TBranch        *b_phoPSCEta;   //!
    TBranch        *b_phoPSCPhi;   //!
    TBranch        *b_phoPSCEtaWidth;   //!
    TBranch        *b_phoPSCPhiWidth;   //!
    TBranch        *b_phoPSCBrem;   //!
    TBranch        *b_phoPSCnHits;   //!
    TBranch        *b_phoPSCflags;   //!
    TBranch        *b_phoPSCinClean;   //!
    TBranch        *b_phoPSCinUnClean;   //!
    TBranch        *b_phoPSCnBC;   //!
    TBranch        *b_phoPESEn;   //!
    TBranch        *b_phoIsPFPhoton;   //!
    TBranch        *b_phoIsStandardPhoton;   //!
    TBranch        *b_phoHasPixelSeed;   //!
    TBranch        *b_phoHasConversionTracks;   //!
    TBranch        *b_phoR9;   //!
    TBranch        *b_phoHoverE;   //!
    TBranch        *b_phoHadTowerOverEm;   //!
    TBranch        *b_phoSigmaIEtaIEta;   //!
    TBranch        *b_pho_isEle;   //!
    TBranch        *b_pho_is2015Noise;   //!e
    TBranch        *b_phoE1x5;   //!
    TBranch        *b_phoE2x5;   //!
    TBranch        *b_phoE3x3;   //!
    TBranch        *b_phoE5x5;   //!
    TBranch        *b_phoMaxEnergyXtal;   //!
    TBranch        *b_phoSigmaEtaEta;   //!
    TBranch        *b_phoR1x5;   //!
    TBranch        *b_phoR2x5;   //!
    TBranch        *b_phoESEffSigmaRR;   //!
    TBranch        *b_phoE1x5_2012;   //!
    TBranch        *b_phoE2x5_2012;   //!
    TBranch        *b_phoE3x3_2012;   //!
    TBranch        *b_phoE5x5_2012;   //!
    TBranch        *b_phoR1x5_2012;   //!
    TBranch        *b_phoR2x5_2012;   //!
    TBranch        *b_phoMaxEnergyXtal_2012;   //!
    TBranch        *b_phoSigmaIEtaIEta_2012;   //!
    TBranch        *b_phoBC1E;   //!
    TBranch        *b_phoBC1Ecorr;   //!
    TBranch        *b_phoBC1Eta;   //!
    TBranch        *b_phoBC1Phi;   //!
    TBranch        *b_phoBC1size;   //!
    TBranch        *b_phoBC1flags;   //!
    TBranch        *b_phoBC1inClean;   //!
    TBranch        *b_phoBC1inUnClean;   //!
    TBranch        *b_phoBC1rawID;   //!
    TBranch        *b_phoBC2E;   //!
    TBranch        *b_phoBC2Eta;   //!
    TBranch        *b_phoBC2Phi;   //!
    TBranch        *b_phoE1x3;   //!
    TBranch        *b_phoE2x2;   //!
    TBranch        *b_phoE2x5Max;   //!
    TBranch        *b_phoSigmaIEtaIPhi_2012;   //!
    TBranch        *b_phoSigmaIPhiIPhi_2012;   //!
    TBranch        *b_phoE1x3_2012;   //!
    TBranch        *b_phoE2x2_2012;   //!
    TBranch        *b_phoE2x5Max_2012;   //!
  TBranch        *b_pho_ecalClusterIsoR2;   //!
  TBranch        *b_pho_ecalClusterIsoR3;   //!
  TBranch        *b_pho_ecalClusterIsoR4;   //!
  TBranch        *b_pho_ecalClusterIsoR5;   //!
  TBranch        *b_pho_hcalRechitIsoR1;   //!
  TBranch        *b_pho_hcalRechitIsoR2;   //!
  TBranch        *b_pho_hcalRechitIsoR3;   //!
  TBranch        *b_pho_hcalRechitIsoR4;   //!
  TBranch        *b_pho_hcalRechitIsoR5;   //!
  TBranch        *b_pho_trackIsoR1PtCut20;   //!
  TBranch        *b_pho_trackIsoR2PtCut20;   //!
  TBranch        *b_pho_trackIsoR3PtCut20;   //!
  TBranch        *b_pho_trackIsoR4PtCut20;   //!
  TBranch        *b_pho_trackIsoR5PtCut20;   //!
  TBranch        *b_pho_sumIsoCorrected;   //!
  TBranch        *b_pho_sumIso;   //!
  TBranch        *b_pho_swissCrx;   //!
  TBranch        *b_pho_seedTime;   //!
  TBranch        *b_pfcIso1;   //!
  TBranch        *b_pfcIso2;   //!
  TBranch        *b_pfcIso3;   //!
  TBranch        *b_pfcIso4;   //!
  TBranch        *b_pfcIso5;   //!
  TBranch        *b_pfpIso1;   //!
  TBranch        *b_pfpIso2;   //!
  TBranch        *b_pfpIso3;   //!
  TBranch        *b_pfpIso4;   //!
  TBranch        *b_pfpIso5;   //!
  TBranch        *b_pfnIso1;   //!
  TBranch        *b_pfnIso2;   //!
  TBranch        *b_pfnIso3;   //!
  TBranch        *b_pfnIso4;   //!
  TBranch        *b_pfnIso5;   //!
  TBranch        *b_pfcVsIso1;   //!
  TBranch        *b_pfcVsIso2;   //!
  TBranch        *b_pfcVsIso3;   //!
  TBranch        *b_pfcVsIso4;   //!
  TBranch        *b_pfcVsIso5;   //!
  TBranch        *b_pfcVsIso1th1;   //!
  TBranch        *b_pfcVsIso2th1;   //!
  TBranch        *b_pfcVsIso3th1;   //!
  TBranch        *b_pfcVsIso4th1;   //!
  TBranch        *b_pfcVsIso5th1;   //!
  TBranch        *b_pfcVsIso1th2;   //!
  TBranch        *b_pfcVsIso2th2;   //!
  TBranch        *b_pfcVsIso3th2;   //!
  TBranch        *b_pfcVsIso4th2;   //!
  TBranch        *b_pfcVsIso5th2;   //!
  TBranch        *b_pfnVsIso1;   //!
  TBranch        *b_pfnVsIso2;   //!
  TBranch        *b_pfnVsIso3;   //!
  TBranch        *b_pfnVsIso4;   //!
  TBranch        *b_pfnVsIso5;   //!
  TBranch        *b_pfnVsIso1th1;   //!
  TBranch        *b_pfnVsIso2th1;   //!
  TBranch        *b_pfnVsIso3th1;   //!
  TBranch        *b_pfnVsIso4th1;   //!
  TBranch        *b_pfnVsIso5th1;   //!
  TBranch        *b_pfnVsIso1th2;   //!
  TBranch        *b_pfnVsIso2th2;   //!
  TBranch        *b_pfnVsIso3th2;   //!
  TBranch        *b_pfnVsIso4th2;   //!
  TBranch        *b_pfnVsIso5th2;   //!
  TBranch        *b_pfpVsIso1;   //!
  TBranch        *b_pfpVsIso2;   //!
  TBranch        *b_pfpVsIso3;   //!
  TBranch        *b_pfpVsIso4;   //!
  TBranch        *b_pfpVsIso5;   //!
  TBranch        *b_pfpVsIso1th1;   //!
  TBranch        *b_pfpVsIso2th1;   //!
  TBranch        *b_pfpVsIso3th1;   //!
  TBranch        *b_pfpVsIso4th1;   //!
  TBranch        *b_pfpVsIso5th1;   //!
  TBranch        *b_pfpVsIso1th2;   //!
  TBranch        *b_pfpVsIso2th2;   //!
  TBranch        *b_pfpVsIso3th2;   //!
  TBranch        *b_pfpVsIso4th2;   //!
  TBranch        *b_pfpVsIso5th2;   //!
  TBranch        *b_towerIso1;   //!
  TBranch        *b_towerIso2;   //!
  TBranch        *b_towerIso3;   //!
  TBranch        *b_towerIso4;   //!
  TBranch        *b_towerIso5;   //!
  TBranch        *b_towerVsIso1;   //!
  TBranch        *b_towerVsIso2;   //!
  TBranch        *b_towerVsIso3;   //!
  TBranch        *b_towerVsIso4;   //!
  TBranch        *b_towerVsIso5;   //!
  TBranch        *b_towerVsSubIso1;   //!
  TBranch        *b_towerVsSubIso2;   //!
  TBranch        *b_towerVsSubIso3;   //!
  TBranch        *b_towerVsSubIso4;   //!
  TBranch        *b_towerVsSubIso5;   //!
};

void ggHiNtuplizer::setupTreeForReading(TTree *t)
{
    // Set branch addresses and branch pointers
    if (t->GetBranch("run")) t->SetBranchAddress("run", &run, &b_run);
    if (t->GetBranch("event")) t->SetBranchAddress("event", &event, &b_event);
    if (t->GetBranch("lumis")) t->SetBranchAddress("lumis", &lumis, &b_lumis);
    if (t->GetBranch("isData")) t->SetBranchAddress("isData", &isData, &b_isData);
    if (t->GetBranch("nPUInfo")) t->SetBranchAddress("nPUInfo", &nPUInfo, &b_nPUInfo);
    if (t->GetBranch("nPU")) t->SetBranchAddress("nPU", &nPU, &b_nPU);
    if (t->GetBranch("puBX")) t->SetBranchAddress("puBX", &puBX, &b_puBX);
    if (t->GetBranch("puTrue")) t->SetBranchAddress("puTrue", &puTrue, &b_puTrue);
    if (t->GetBranch("nMC")) t->SetBranchAddress("nMC", &nMC, &b_nMC);
    if (t->GetBranch("mcPID")) t->SetBranchAddress("mcPID", &mcPID, &b_mcPID);
    if (t->GetBranch("mcStatus")) t->SetBranchAddress("mcStatus", &mcStatus, &b_mcStatus);
    if (t->GetBranch("mcVtx_x")) t->SetBranchAddress("mcVtx_x", &mcVtx_x, &b_mcVtx_x);
    if (t->GetBranch("mcVtx_y")) t->SetBranchAddress("mcVtx_y", &mcVtx_y, &b_mcVtx_y);
    if (t->GetBranch("mcVtx_z")) t->SetBranchAddress("mcVtx_z", &mcVtx_z, &b_mcVtx_z);
    if (t->GetBranch("mcPt")) t->SetBranchAddress("mcPt", &mcPt, &b_mcPt);
    if (t->GetBranch("mcEta")) t->SetBranchAddress("mcEta", &mcEta, &b_mcEta);
    if (t->GetBranch("mcPhi")) t->SetBranchAddress("mcPhi", &mcPhi, &b_mcPhi);
    if (t->GetBranch("mcE")) t->SetBranchAddress("mcE", &mcE, &b_mcE);
    if (t->GetBranch("mcEt")) t->SetBranchAddress("mcEt", &mcEt, &b_mcEt);
    if (t->GetBranch("mcMass")) t->SetBranchAddress("mcMass", &mcMass, &b_mcMass);
    if (t->GetBranch("mcParentage")) t->SetBranchAddress("mcParentage", &mcParentage, &b_mcParentage);
    if (t->GetBranch("mcMomPID")) t->SetBranchAddress("mcMomPID", &mcMomPID, &b_mcMomPID);
    if (t->GetBranch("mcMomPt")) t->SetBranchAddress("mcMomPt", &mcMomPt, &b_mcMomPt);
    if (t->GetBranch("mcMomEta")) t->SetBranchAddress("mcMomEta", &mcMomEta, &b_mcMomEta);
    if (t->GetBranch("mcMomPhi")) t->SetBranchAddress("mcMomPhi", &mcMomPhi, &b_mcMomPhi);
    if (t->GetBranch("mcMomMass")) t->SetBranchAddress("mcMomMass", &mcMomMass, &b_mcMomMass);
    if (t->GetBranch("mcGMomPID")) t->SetBranchAddress("mcGMomPID", &mcGMomPID, &b_mcGMomPID);
    if (t->GetBranch("mcIndex")) t->SetBranchAddress("mcIndex", &mcIndex, &b_mcIndex);
    if (t->GetBranch("mcCalIsoDR03")) t->SetBranchAddress("mcCalIsoDR03", &mcCalIsoDR03, &b_mcCalIsoDR03);
    if (t->GetBranch("mcCalIsoDR04")) t->SetBranchAddress("mcCalIsoDR04", &mcCalIsoDR04, &b_mcCalIsoDR04);
    if (t->GetBranch("mcTrkIsoDR03")) t->SetBranchAddress("mcTrkIsoDR03", &mcTrkIsoDR03, &b_mcTrkIsoDR03);
    if (t->GetBranch("mcTrkIsoDR04")) t->SetBranchAddress("mcTrkIsoDR04", &mcTrkIsoDR04, &b_mcTrkIsoDR04);
    if (t->GetBranch("pho_genMatchedIndex")) t->SetBranchAddress("pho_genMatchedIndex", &pho_genMatchedIndex, &b_pho_genMatchedIndex);
    if (t->GetBranch("pho_genPID")) t->SetBranchAddress("pho_genPID", &pho_genPID, &b_pho_genPID);
    if (t->GetBranch("pho_genStatus")) t->SetBranchAddress("pho_genStatus", &pho_genStatus, &b_pho_genStatus);
    if (t->GetBranch("pho_genPt")) t->SetBranchAddress("pho_genPt", &pho_genPt, &b_pho_genPt);
    if (t->GetBranch("pho_genEta")) t->SetBranchAddress("pho_genEta", &pho_genEta, &b_pho_genEta);
    if (t->GetBranch("pho_genPhi")) t->SetBranchAddress("pho_genPhi", &pho_genPhi, &b_pho_genPhi);
    if (t->GetBranch("pho_genE")) t->SetBranchAddress("pho_genE", &pho_genE, &b_pho_genE);
    if (t->GetBranch("pho_genEt")) t->SetBranchAddress("pho_genEt", &pho_genEt, &b_pho_genEt);
    if (t->GetBranch("pho_genMomPID")) t->SetBranchAddress("pho_genMomPID", &pho_genMomPID, &b_pho_genMomPID);
    if (t->GetBranch("pho_genCalIsoDR03")) t->SetBranchAddress("pho_genCalIsoDR03", &pho_genCalIsoDR03, &b_pho_genCalIsoDR03);
    if (t->GetBranch("pho_genCalIsoDR04")) t->SetBranchAddress("pho_genCalIsoDR04", &pho_genCalIsoDR04, &b_pho_genCalIsoDR04);
    if (t->GetBranch("pho_genTrkIsoDR03")) t->SetBranchAddress("pho_genTrkIsoDR03", &pho_genTrkIsoDR03, &b_pho_genTrkIsoDR03);
    if (t->GetBranch("pho_genTrkIsoDR04")) t->SetBranchAddress("pho_genTrkIsoDR04", &pho_genTrkIsoDR04, &b_pho_genTrkIsoDR04);
    if (t->GetBranch("nEle")) t->SetBranchAddress("nEle", &nEle, &b_nEle);
    if (t->GetBranch("eleCharge")) t->SetBranchAddress("eleCharge", &eleCharge, &b_eleCharge);
    if (t->GetBranch("eleChargeConsistent")) t->SetBranchAddress("eleChargeConsistent", &eleChargeConsistent, &b_eleChargeConsistent);
    if (t->GetBranch("eleSCPixCharge")) t->SetBranchAddress("eleSCPixCharge", &eleSCPixCharge, &b_eleSCPixCharge);
    if (t->GetBranch("eleCtfCharge")) t->SetBranchAddress("eleCtfCharge", &eleCtfCharge, &b_eleCtfCharge);
    if (t->GetBranch("eleEn")) t->SetBranchAddress("eleEn", &eleEn, &b_eleEn);
    if (t->GetBranch("eleD0")) t->SetBranchAddress("eleD0", &eleD0, &b_eleD0);
    if (t->GetBranch("eleDz")) t->SetBranchAddress("eleDz", &eleDz, &b_eleDz);
    if (t->GetBranch("eleD0Err")) t->SetBranchAddress("eleD0Err", &eleD0Err, &b_eleD0Err);
    if (t->GetBranch("eleDzErr")) t->SetBranchAddress("eleDzErr", &eleDzErr, &b_eleDzErr);
    if (t->GetBranch("eleTrkPt")) t->SetBranchAddress("eleTrkPt", &eleTrkPt, &b_eleTrkPt);
    if (t->GetBranch("eleTrkEta")) t->SetBranchAddress("eleTrkEta", &eleTrkEta, &b_eleTrkEta);
    if (t->GetBranch("eleTrkPhi")) t->SetBranchAddress("eleTrkPhi", &eleTrkPhi, &b_eleTrkPhi);
    if (t->GetBranch("eleTrkCharge")) t->SetBranchAddress("eleTrkCharge", &eleTrkCharge, &b_eleTrkCharge);
    if (t->GetBranch("eleTrkChi2")) t->SetBranchAddress("eleTrkChi2", &eleTrkChi2, &b_eleTrkChi2);
    if (t->GetBranch("eleTrkNdof")) t->SetBranchAddress("eleTrkNdof", &eleTrkNdof, &b_eleTrkNdof);
    if (t->GetBranch("eleTrkNormalizedChi2")) t->SetBranchAddress("eleTrkNormalizedChi2", &eleTrkNormalizedChi2, &b_eleTrkNormalizedChi2);
    if (t->GetBranch("eleTrkValidHits")) t->SetBranchAddress("eleTrkValidHits", &eleTrkValidHits, &b_eleTrkValidHits);
    if (t->GetBranch("eleTrkLayers")) t->SetBranchAddress("eleTrkLayers", &eleTrkLayers, &b_eleTrkLayers);
    if (t->GetBranch("elePt")) t->SetBranchAddress("elePt", &elePt, &b_elePt);
    if (t->GetBranch("eleEta")) t->SetBranchAddress("eleEta", &eleEta, &b_eleEta);
    if (t->GetBranch("elePhi")) t->SetBranchAddress("elePhi", &elePhi, &b_elePhi);
    if (t->GetBranch("eleSCEn")) t->SetBranchAddress("eleSCEn", &eleSCEn, &b_eleSCEn);
    if (t->GetBranch("eleESEn")) t->SetBranchAddress("eleESEn", &eleESEn, &b_eleESEn);
    if (t->GetBranch("eleSCEta")) t->SetBranchAddress("eleSCEta", &eleSCEta, &b_eleSCEta);
    if (t->GetBranch("eleSCPhi")) t->SetBranchAddress("eleSCPhi", &eleSCPhi, &b_eleSCPhi);
    if (t->GetBranch("eleSCRawEn")) t->SetBranchAddress("eleSCRawEn", &eleSCRawEn, &b_eleSCRawEn);
    if (t->GetBranch("eleSCEtaWidth")) t->SetBranchAddress("eleSCEtaWidth", &eleSCEtaWidth, &b_eleSCEtaWidth);
    if (t->GetBranch("eleSCPhiWidth")) t->SetBranchAddress("eleSCPhiWidth", &eleSCPhiWidth, &b_eleSCPhiWidth);
    if (t->GetBranch("eleHoverE")) t->SetBranchAddress("eleHoverE", &eleHoverE, &b_eleHoverE);
    if (t->GetBranch("eleHoverEBc")) t->SetBranchAddress("eleHoverEBc", &eleHoverEBc, &b_eleHoverEBc);
    if (t->GetBranch("eleEoverP")) t->SetBranchAddress("eleEoverP", &eleEoverP, &b_eleEoverP);
    if (t->GetBranch("eleEoverPInv")) t->SetBranchAddress("eleEoverPInv", &eleEoverPInv, &b_eleEoverPInv);
    if (t->GetBranch("eleBrem")) t->SetBranchAddress("eleBrem", &eleBrem, &b_eleBrem);
    if (t->GetBranch("eledEtaAtVtx")) t->SetBranchAddress("eledEtaAtVtx", &eledEtaAtVtx, &b_eledEtaAtVtx);
    if (t->GetBranch("eledPhiAtVtx")) t->SetBranchAddress("eledPhiAtVtx", &eledPhiAtVtx, &b_eledPhiAtVtx);
    if (t->GetBranch("eleSigmaIEtaIEta")) t->SetBranchAddress("eleSigmaIEtaIEta", &eleSigmaIEtaIEta, &b_eleSigmaIEtaIEta);
    if (t->GetBranch("eleSigmaIEtaIEta_2012")) t->SetBranchAddress("eleSigmaIEtaIEta_2012", &eleSigmaIEtaIEta_2012, &b_eleSigmaIEtaIEta_2012);
    if (t->GetBranch("eleSigmaIPhiIPhi")) t->SetBranchAddress("eleSigmaIPhiIPhi", &eleSigmaIPhiIPhi, &b_eleSigmaIPhiIPhi);
    if (t->GetBranch("eleMissHits")) t->SetBranchAddress("eleMissHits", &eleMissHits, &b_eleMissHits);
    if (t->GetBranch("eleESEffSigmaRR")) t->SetBranchAddress("eleESEffSigmaRR", &eleESEffSigmaRR, &b_eleESEffSigmaRR);
    if (t->GetBranch("elePFChIso")) t->SetBranchAddress("elePFChIso", &elePFChIso, &b_elePFChIso);
    if (t->GetBranch("elePFPhoIso")) t->SetBranchAddress("elePFPhoIso", &elePFPhoIso, &b_elePFPhoIso);
    if (t->GetBranch("elePFNeuIso")) t->SetBranchAddress("elePFNeuIso", &elePFNeuIso, &b_elePFNeuIso);
    if (t->GetBranch("elePFPUIso")) t->SetBranchAddress("elePFPUIso", &elePFPUIso, &b_elePFPUIso);
    if (t->GetBranch("elePFChIso03")) t->SetBranchAddress("elePFChIso03", &elePFChIso03, &b_elePFChIso03);
    if (t->GetBranch("elePFPhoIso03")) t->SetBranchAddress("elePFPhoIso03", &elePFPhoIso03, &b_elePFPhoIso03);
    if (t->GetBranch("elePFNeuIso03")) t->SetBranchAddress("elePFNeuIso03", &elePFNeuIso03, &b_elePFNeuIso03);
    if (t->GetBranch("elePFChIso04")) t->SetBranchAddress("elePFChIso04", &elePFChIso04, &b_elePFChIso04);
    if (t->GetBranch("elePFPhoIso04")) t->SetBranchAddress("elePFPhoIso04", &elePFPhoIso04, &b_elePFPhoIso04);
    if (t->GetBranch("elePFNeuIso04")) t->SetBranchAddress("elePFNeuIso04", &elePFNeuIso04, &b_elePFNeuIso04);
    if (t->GetBranch("eleR9")) t->SetBranchAddress("eleR9", &eleR9, &b_eleR9);
    if (t->GetBranch("eleE3x3")) t->SetBranchAddress("eleE3x3", &eleE3x3, &b_eleE3x3);
    if (t->GetBranch("eleE5x5")) t->SetBranchAddress("eleE5x5", &eleE5x5, &b_eleE5x5);
    if (t->GetBranch("eleR9Full5x5")) t->SetBranchAddress("eleR9Full5x5", &eleR9Full5x5, &b_eleR9Full5x5);
    if (t->GetBranch("eleE3x3Full5x5")) t->SetBranchAddress("eleE3x3Full5x5", &eleE3x3Full5x5, &b_eleE3x3Full5x5);
    if (t->GetBranch("eleE5x5Full5x5")) t->SetBranchAddress("eleE5x5Full5x5", &eleE5x5Full5x5, &b_eleE5x5Full5x5);
    if (t->GetBranch("NClusters")) t->SetBranchAddress("NClusters", &NClusters, &b_NClusters);
    if (t->GetBranch("NEcalClusters")) t->SetBranchAddress("NEcalClusters", &NEcalClusters, &b_NEcalClusters);
    if (t->GetBranch("eleSeedEn")) t->SetBranchAddress("eleSeedEn", &eleSeedEn, &b_eleSeedEn);
    if (t->GetBranch("eleSeedEta")) t->SetBranchAddress("eleSeedEta", &eleSeedEta, &b_eleSeedEta);
    if (t->GetBranch("eleSeedPhi")) t->SetBranchAddress("eleSeedPhi", &eleSeedPhi, &b_eleSeedPhi);
    if (t->GetBranch("eleSeedCryEta")) t->SetBranchAddress("eleSeedCryEta", &eleSeedCryEta, &b_eleSeedCryEta);
    if (t->GetBranch("eleSeedCryPhi")) t->SetBranchAddress("eleSeedCryPhi", &eleSeedCryPhi, &b_eleSeedCryPhi);
    if (t->GetBranch("eleSeedCryIeta")) t->SetBranchAddress("eleSeedCryIeta", &eleSeedCryIeta, &b_eleSeedCryIeta);
    if (t->GetBranch("eleSeedCryIphi")) t->SetBranchAddress("eleSeedCryIphi", &eleSeedCryIphi, &b_eleSeedCryIphi);
    if (t->GetBranch("eleBC1E")) t->SetBranchAddress("eleBC1E", &eleBC1E, &b_eleBC1E);
    if (t->GetBranch("eleBC1Eta")) t->SetBranchAddress("eleBC1Eta", &eleBC1Eta, &b_eleBC1Eta);
    if (t->GetBranch("eleBC2E")) t->SetBranchAddress("eleBC2E", &eleBC2E, &b_eleBC2E);
    if (t->GetBranch("eleBC2Eta")) t->SetBranchAddress("eleBC2Eta", &eleBC2Eta, &b_eleBC2Eta);
    if (t->GetBranch("eleIDVeto")) t->SetBranchAddress("eleIDVeto", &eleIDVeto, &b_eleIDVeto);
    if (t->GetBranch("eleIDLoose")) t->SetBranchAddress("eleIDLoose", &eleIDLoose, &b_eleIDLoose);
    if (t->GetBranch("eleIDMedium")) t->SetBranchAddress("eleIDMedium", &eleIDMedium, &b_eleIDMedium);
    if (t->GetBranch("eleIDTight")) t->SetBranchAddress("eleIDTight", &eleIDTight, &b_eleIDTight);
    if (t->GetBranch("elepassConversionVeto")) t->SetBranchAddress("elepassConversionVeto", &elepassConversionVeto, &b_elepassConversionVeto);
    if (t->GetBranch("eleEffAreaTimesRho")) t->SetBranchAddress("eleEffAreaTimesRho", &eleEffAreaTimesRho, &b_eleEffAreaTimesRho);
    if (t->GetBranch("nPho")) t->SetBranchAddress("nPho", &nPho, &b_nPho);
    if (t->GetBranch("phoE")) t->SetBranchAddress("phoE", &phoE, &b_phoE);
    if (t->GetBranch("phoEt")) t->SetBranchAddress("phoEt", &phoEt, &b_phoEt);
    if (t->GetBranch("phoPreScale")) t->SetBranchAddress("phoPreScale", &phoPreScale, &b_phoPreScale);
    if (t->GetBranch("phoEtCorrected")) t->SetBranchAddress("phoEtCorrected", &phoEtCorrected, &b_phoEtCorrected);
    if (t->GetBranch("phoEtCorrected_sys")) t->SetBranchAddress("phoEtCorrected_sys", &phoEtCorrected_sys, &b_phoEtCorrected_sys);
    if (t->GetBranch("phoEtCorrected_resSys_sig")) t->SetBranchAddress("phoEtCorrected_resSys_sig", &phoEtCorrected_resSys_sig, &b_phoEtCorrected_resSys_sig);
    if (t->GetBranch("phoEtCorrected_resSys_sig2")) t->SetBranchAddress("phoEtCorrected_resSys_sig2", &phoEtCorrected_resSys_sig2, &b_phoEtCorrected_resSys_sig2);
    if (t->GetBranch("phoEtCorrected_resSys_rms")) t->SetBranchAddress("phoEtCorrected_resSys_rms", &phoEtCorrected_resSys_rms, &b_phoEtCorrected_resSys_rms);
    if (t->GetBranch("phoEtCorrected_resSys_rms2")) t->SetBranchAddress("phoEtCorrected_resSys_rms2", &phoEtCorrected_resSys_rms2, &b_phoEtCorrected_resSys_rms2);
    if (t->GetBranch("phoEtCorrected_resSys_up")) t->SetBranchAddress("phoEtCorrected_resSys_up", &phoEtCorrected_resSys_up, &b_phoEtCorrected_resSys_up);
    if (t->GetBranch("phoEtCorrected_resSys_down")) t->SetBranchAddress("phoEtCorrected_resSys_down", &phoEtCorrected_resSys_down, &b_phoEtCorrected_resSys_down);
    if (t->GetBranch("phoEta")) t->SetBranchAddress("phoEta", &phoEta, &b_phoEta);
    if (t->GetBranch("phoPhi")) t->SetBranchAddress("phoPhi", &phoPhi, &b_phoPhi);
    if (t->GetBranch("phoSCE")) t->SetBranchAddress("phoSCE", &phoSCE, &b_phoSCE);
    if (t->GetBranch("phoSCRawE")) t->SetBranchAddress("phoSCRawE", &phoSCRawE, &b_phoSCRawE);
    if (t->GetBranch("phoSCEta")) t->SetBranchAddress("phoSCEta", &phoSCEta, &b_phoSCEta);
    if (t->GetBranch("phoSCPhi")) t->SetBranchAddress("phoSCPhi", &phoSCPhi, &b_phoSCPhi);
    if (t->GetBranch("phoSCEtaWidth")) t->SetBranchAddress("phoSCEtaWidth", &phoSCEtaWidth, &b_phoSCEtaWidth);
    if (t->GetBranch("phoSCPhiWidth")) t->SetBranchAddress("phoSCPhiWidth", &phoSCPhiWidth, &b_phoSCPhiWidth);
    if (t->GetBranch("phoSCBrem")) t->SetBranchAddress("phoSCBrem", &phoSCBrem, &b_phoSCBrem);
    if (t->GetBranch("phoSCnHits")) t->SetBranchAddress("phoSCnHits", &phoSCnHits, &b_phoSCnHits);
    if (t->GetBranch("phoSCflags")) t->SetBranchAddress("phoSCflags", &phoSCflags, &b_phoSCflags);
    if (t->GetBranch("phoSCinClean")) t->SetBranchAddress("phoSCinClean", &phoSCinClean, &b_phoSCinClean);
    if (t->GetBranch("phoSCinUnClean")) t->SetBranchAddress("phoSCinUnClean", &phoSCinUnClean, &b_phoSCinUnClean);
    if (t->GetBranch("phoSCnBC")) t->SetBranchAddress("phoSCnBC", &phoSCnBC, &b_phoSCnBC);
    if (t->GetBranch("phoESEn")) t->SetBranchAddress("phoESEn", &phoESEn, &b_phoESEn);
    if (t->GetBranch("phoPSCE")) t->SetBranchAddress("phoPSCE", &phoPSCE, &b_phoPSCE);
    if (t->GetBranch("phoPSCRawE")) t->SetBranchAddress("phoPSCRawE", &phoPSCRawE, &b_phoPSCRawE);
    if (t->GetBranch("phoPSCEta")) t->SetBranchAddress("phoPSCEta", &phoPSCEta, &b_phoPSCEta);
    if (t->GetBranch("phoPSCPhi")) t->SetBranchAddress("phoPSCPhi", &phoPSCPhi, &b_phoPSCPhi);
    if (t->GetBranch("phoPSCEtaWidth")) t->SetBranchAddress("phoPSCEtaWidth", &phoPSCEtaWidth, &b_phoPSCEtaWidth);
    if (t->GetBranch("phoPSCPhiWidth")) t->SetBranchAddress("phoPSCPhiWidth", &phoPSCPhiWidth, &b_phoPSCPhiWidth);
    if (t->GetBranch("phoPSCBrem")) t->SetBranchAddress("phoPSCBrem", &phoPSCBrem, &b_phoPSCBrem);
    if (t->GetBranch("phoPSCnHits")) t->SetBranchAddress("phoPSCnHits", &phoPSCnHits, &b_phoPSCnHits);
    if (t->GetBranch("phoPSCflags")) t->SetBranchAddress("phoPSCflags", &phoPSCflags, &b_phoPSCflags);
    if (t->GetBranch("phoPSCinClean")) t->SetBranchAddress("phoPSCinClean", &phoPSCinClean, &b_phoPSCinClean);
    if (t->GetBranch("phoPSCinUnClean")) t->SetBranchAddress("phoPSCinUnClean", &phoPSCinUnClean, &b_phoPSCinUnClean);
    if (t->GetBranch("phoPSCnBC")) t->SetBranchAddress("phoPSCnBC", &phoPSCnBC, &b_phoPSCnBC);
    if (t->GetBranch("phoPESEn")) t->SetBranchAddress("phoPESEn", &phoPESEn, &b_phoPESEn);
    if (t->GetBranch("phoIsPFPhoton")) t->SetBranchAddress("phoIsPFPhoton", &phoIsPFPhoton, &b_phoIsPFPhoton);
    if (t->GetBranch("phoIsStandardPhoton")) t->SetBranchAddress("phoIsStandardPhoton", &phoIsStandardPhoton, &b_phoIsStandardPhoton);
    if (t->GetBranch("phoHasPixelSeed")) t->SetBranchAddress("phoHasPixelSeed", &phoHasPixelSeed, &b_phoHasPixelSeed);
    if (t->GetBranch("phoHasConversionTracks")) t->SetBranchAddress("phoHasConversionTracks", &phoHasConversionTracks, &b_phoHasConversionTracks);
    if (t->GetBranch("phoR9")) t->SetBranchAddress("phoR9", &phoR9, &b_phoR9);
    if (t->GetBranch("phoHoverE")) t->SetBranchAddress("phoHoverE", &phoHoverE, &b_phoHoverE);
    if (t->GetBranch("phoHadTowerOverEm")) t->SetBranchAddress("phoHadTowerOverEm", &phoHadTowerOverEm, &b_phoHadTowerOverEm);
    if (t->GetBranch("phoSigmaIEtaIEta")) t->SetBranchAddress("phoSigmaIEtaIEta", &phoSigmaIEtaIEta, &b_phoSigmaIEtaIEta);
    if (t->GetBranch("pho_isEle")) t->SetBranchAddress("pho_isEle", &pho_isEle, &b_pho_isEle);
    if (t->GetBranch("pho_is2015Noise")) t->SetBranchAddress("pho_is2015Noise", &pho_is2015Noise, &b_pho_is2015Noise);
    
    if (t->GetBranch("phoE1x3")) t->SetBranchAddress("phoE1x3", &phoE1x3, &b_phoE1x3);
    if (t->GetBranch("phoE2x2")) t->SetBranchAddress("phoE2x2", &phoE2x2, &b_phoE2x2);
    if (t->GetBranch("phoE2x5Max")) t->SetBranchAddress("phoE2x5Max", &phoE2x5Max, &b_phoE2x5Max);
    //
    if (t->GetBranch("phoE1x5")) t->SetBranchAddress("phoE1x5", &phoE1x5, &b_phoE1x5);
    if (t->GetBranch("phoE2x5")) t->SetBranchAddress("phoE2x5", &phoE2x5, &b_phoE2x5);
    if (t->GetBranch("phoE3x3")) t->SetBranchAddress("phoE3x3", &phoE3x3, &b_phoE3x3);
    if (t->GetBranch("phoE5x5")) t->SetBranchAddress("phoE5x5", &phoE5x5, &b_phoE5x5);
    if (t->GetBranch("phoMaxEnergyXtal")) t->SetBranchAddress("phoMaxEnergyXtal", &phoMaxEnergyXtal, &b_phoMaxEnergyXtal);
    if (t->GetBranch("phoSigmaEtaEta")) t->SetBranchAddress("phoSigmaEtaEta", &phoSigmaEtaEta, &b_phoSigmaEtaEta);
    if (t->GetBranch("phoR1x5")) t->SetBranchAddress("phoR1x5", &phoR1x5, &b_phoR1x5);
    if (t->GetBranch("phoR2x5")) t->SetBranchAddress("phoR2x5", &phoR2x5, &b_phoR2x5);
    if (t->GetBranch("phoESEffSigmaRR")) t->SetBranchAddress("phoESEffSigmaRR", &phoESEffSigmaRR, &b_phoESEffSigmaRR);
    if (t->GetBranch("phoE1x5_2012")) t->SetBranchAddress("phoE1x5_2012", &phoE1x5_2012, &b_phoE1x5_2012);
    if (t->GetBranch("phoE2x5_2012")) t->SetBranchAddress("phoE2x5_2012", &phoE2x5_2012, &b_phoE2x5_2012);
    if (t->GetBranch("phoE3x3_2012")) t->SetBranchAddress("phoE3x3_2012", &phoE3x3_2012, &b_phoE3x3_2012);
    if (t->GetBranch("phoE5x5_2012")) t->SetBranchAddress("phoE5x5_2012", &phoE5x5_2012, &b_phoE5x5_2012);
    if (t->GetBranch("phoR1x5_2012")) t->SetBranchAddress("phoR1x5_2012", &phoR1x5_2012, &b_phoR1x5_2012);
    if (t->GetBranch("phoR2x5_2012")) t->SetBranchAddress("phoR2x5_2012", &phoR2x5_2012, &b_phoR2x5_2012);
    if (t->GetBranch("phoMaxEnergyXtal_2012")) t->SetBranchAddress("phoMaxEnergyXtal_2012", &phoMaxEnergyXtal_2012, &b_phoMaxEnergyXtal_2012);
    if (t->GetBranch("phoSigmaIEtaIEta_2012")) t->SetBranchAddress("phoSigmaIEtaIEta_2012", &phoSigmaIEtaIEta_2012, &b_phoSigmaIEtaIEta_2012);
    
    if (t->GetBranch("phoSigmaIEtaIPhi_2012")) t->SetBranchAddress("phoSigmaIEtaIPhi_2012", &phoSigmaIEtaIPhi_2012, &b_phoSigmaIEtaIPhi_2012);
    if (t->GetBranch("phoSigmaIPhiIPhi_2012")) t->SetBranchAddress("phoSigmaIPhiIPhi_2012", &phoSigmaIPhiIPhi_2012, &b_phoSigmaIPhiIPhi_2012);
    if (t->GetBranch("phoE1x3_2012")) t->SetBranchAddress("phoE1x3_2012", &phoE1x3_2012, &b_phoE1x3_2012);
    if (t->GetBranch("phoE2x2_2012")) t->SetBranchAddress("phoE2x2_2012", &phoE2x2_2012, &b_phoE2x2_2012);
    if (t->GetBranch("phoE2x5Max_2012")) t->SetBranchAddress("phoE2x5Max_2012", &phoE2x5Max_2012, &b_phoE2x5Max_2012);
    if (t->GetBranch("phoE5x5_2012")) t->SetBranchAddress("phoE5x5_2012", &phoE5x5_2012, &b_phoE5x5_2012);
    //
    if (t->GetBranch("phoE3x3_2012")) t->SetBranchAddress("phoE3x3_2012", &phoE3x3_2012, &b_phoE3x3_2012);
    if (t->GetBranch("phoBC1E")) t->SetBranchAddress("phoBC1E", &phoBC1E, &b_phoBC1E);
    if (t->GetBranch("phoBC1Ecorr")) t->SetBranchAddress("phoBC1Ecorr", &phoBC1Ecorr, &b_phoBC1Ecorr);
    if (t->GetBranch("phoBC1Eta")) t->SetBranchAddress("phoBC1Eta", &phoBC1Eta, &b_phoBC1Eta);
    if (t->GetBranch("phoBC1Phi")) t->SetBranchAddress("phoBC1Phi", &phoBC1Phi, &b_phoBC1Phi);
    if (t->GetBranch("phoBC1size")) t->SetBranchAddress("phoBC1size", &phoBC1size, &b_phoBC1size);
    if (t->GetBranch("phoBC1flags")) t->SetBranchAddress("phoBC1flags", &phoBC1flags, &b_phoBC1flags);
    if (t->GetBranch("phoBC1inClean")) t->SetBranchAddress("phoBC1inClean", &phoBC1inClean, &b_phoBC1inClean);
    if (t->GetBranch("phoBC1inUnClean")) t->SetBranchAddress("phoBC1inUnClean", &phoBC1inUnClean, &b_phoBC1inUnClean);
    if (t->GetBranch("phoBC1rawID")) t->SetBranchAddress("phoBC1rawID", &phoBC1rawID, &b_phoBC1rawID);
    if (t->GetBranch("phoBC2E")) t->SetBranchAddress("phoBC2E", &phoBC2E, &b_phoBC2E);
    if (t->GetBranch("phoBC2Eta")) t->SetBranchAddress("phoBC2Eta", &phoBC2Eta, &b_phoBC2Eta);
    if (t->GetBranch("phoBC2Phi")) t->SetBranchAddress("phoBC2Phi", &phoBC2Phi, &b_phoBC2Phi);
    if (t->GetBranch("pho_ecalClusterIsoR2")) t->SetBranchAddress("pho_ecalClusterIsoR2", &pho_ecalClusterIsoR2, &b_pho_ecalClusterIsoR2);
    if (t->GetBranch("pho_ecalClusterIsoR3")) t->SetBranchAddress("pho_ecalClusterIsoR3", &pho_ecalClusterIsoR3, &b_pho_ecalClusterIsoR3);
    if (t->GetBranch("pho_ecalClusterIsoR4")) t->SetBranchAddress("pho_ecalClusterIsoR4", &pho_ecalClusterIsoR4, &b_pho_ecalClusterIsoR4);
    if (t->GetBranch("pho_ecalClusterIsoR5")) t->SetBranchAddress("pho_ecalClusterIsoR5", &pho_ecalClusterIsoR5, &b_pho_ecalClusterIsoR5);
    if (t->GetBranch("pho_hcalRechitIsoR1")) t->SetBranchAddress("pho_hcalRechitIsoR1", &pho_hcalRechitIsoR1, &b_pho_hcalRechitIsoR1);
    if (t->GetBranch("pho_hcalRechitIsoR2")) t->SetBranchAddress("pho_hcalRechitIsoR2", &pho_hcalRechitIsoR2, &b_pho_hcalRechitIsoR2);
    if (t->GetBranch("pho_hcalRechitIsoR3")) t->SetBranchAddress("pho_hcalRechitIsoR3", &pho_hcalRechitIsoR3, &b_pho_hcalRechitIsoR3);
    if (t->GetBranch("pho_hcalRechitIsoR4")) t->SetBranchAddress("pho_hcalRechitIsoR4", &pho_hcalRechitIsoR4, &b_pho_hcalRechitIsoR4);
    if (t->GetBranch("pho_hcalRechitIsoR5")) t->SetBranchAddress("pho_hcalRechitIsoR5", &pho_hcalRechitIsoR5, &b_pho_hcalRechitIsoR5);
    if (t->GetBranch("pho_trackIsoR1PtCut20")) t->SetBranchAddress("pho_trackIsoR1PtCut20", &pho_trackIsoR1PtCut20, &b_pho_trackIsoR1PtCut20);
    if (t->GetBranch("pho_trackIsoR2PtCut20")) t->SetBranchAddress("pho_trackIsoR2PtCut20", &pho_trackIsoR2PtCut20, &b_pho_trackIsoR2PtCut20);
    if (t->GetBranch("pho_trackIsoR3PtCut20")) t->SetBranchAddress("pho_trackIsoR3PtCut20", &pho_trackIsoR3PtCut20, &b_pho_trackIsoR3PtCut20);
    if (t->GetBranch("pho_trackIsoR4PtCut20")) t->SetBranchAddress("pho_trackIsoR4PtCut20", &pho_trackIsoR4PtCut20, &b_pho_trackIsoR4PtCut20);
    if (t->GetBranch("pho_trackIsoR5PtCut20")) t->SetBranchAddress("pho_trackIsoR5PtCut20", &pho_trackIsoR5PtCut20, &b_pho_trackIsoR5PtCut20);
    if (t->GetBranch("pho_sumIsoCorrected")) t->SetBranchAddress("pho_sumIsoCorrected", &pho_sumIsoCorrected, &b_pho_sumIsoCorrected);
    if (t->GetBranch("pho_sumIso")) t->SetBranchAddress("pho_sumIso", &pho_sumIso, &b_pho_sumIso);
    if (t->GetBranch("pho_swissCrx")) t->SetBranchAddress("pho_swissCrx", &pho_swissCrx, &b_pho_swissCrx);
    if (t->GetBranch("pho_seedTime")) t->SetBranchAddress("pho_seedTime", &pho_seedTime, &b_pho_seedTime);
    if (t->GetBranch("pfcIso1")) t->SetBranchAddress("pfcIso1", &pfcIso1, &b_pfcIso1);
    if (t->GetBranch("pfcIso2")) t->SetBranchAddress("pfcIso2", &pfcIso2, &b_pfcIso2);
    if (t->GetBranch("pfcIso3")) t->SetBranchAddress("pfcIso3", &pfcIso3, &b_pfcIso3);
    if (t->GetBranch("pfcIso4")) t->SetBranchAddress("pfcIso4", &pfcIso4, &b_pfcIso4);
    if (t->GetBranch("pfcIso5")) t->SetBranchAddress("pfcIso5", &pfcIso5, &b_pfcIso5);
    if (t->GetBranch("pfpIso1")) t->SetBranchAddress("pfpIso1", &pfpIso1, &b_pfpIso1);
    if (t->GetBranch("pfpIso2")) t->SetBranchAddress("pfpIso2", &pfpIso2, &b_pfpIso2);
    if (t->GetBranch("pfpIso3")) t->SetBranchAddress("pfpIso3", &pfpIso3, &b_pfpIso3);
    if (t->GetBranch("pfpIso4")) t->SetBranchAddress("pfpIso4", &pfpIso4, &b_pfpIso4);
    if (t->GetBranch("pfpIso5")) t->SetBranchAddress("pfpIso5", &pfpIso5, &b_pfpIso5);
    if (t->GetBranch("pfnIso1")) t->SetBranchAddress("pfnIso1", &pfnIso1, &b_pfnIso1);
    if (t->GetBranch("pfnIso2")) t->SetBranchAddress("pfnIso2", &pfnIso2, &b_pfnIso2);
    if (t->GetBranch("pfnIso3")) t->SetBranchAddress("pfnIso3", &pfnIso3, &b_pfnIso3);
    if (t->GetBranch("pfnIso4")) t->SetBranchAddress("pfnIso4", &pfnIso4, &b_pfnIso4);
    if (t->GetBranch("pfnIso5")) t->SetBranchAddress("pfnIso5", &pfnIso5, &b_pfnIso5);
    if (t->GetBranch("pfcVsIso1")) t->SetBranchAddress("pfcVsIso1", &pfcVsIso1, &b_pfcVsIso1);
    if (t->GetBranch("pfcVsIso2")) t->SetBranchAddress("pfcVsIso2", &pfcVsIso2, &b_pfcVsIso2);
    if (t->GetBranch("pfcVsIso3")) t->SetBranchAddress("pfcVsIso3", &pfcVsIso3, &b_pfcVsIso3);
    if (t->GetBranch("pfcVsIso4")) t->SetBranchAddress("pfcVsIso4", &pfcVsIso4, &b_pfcVsIso4);
    if (t->GetBranch("pfcVsIso5")) t->SetBranchAddress("pfcVsIso5", &pfcVsIso5, &b_pfcVsIso5);
    if (t->GetBranch("pfcVsIso1th1")) t->SetBranchAddress("pfcVsIso1th1", &pfcVsIso1th1, &b_pfcVsIso1th1);
    if (t->GetBranch("pfcVsIso2th1")) t->SetBranchAddress("pfcVsIso2th1", &pfcVsIso2th1, &b_pfcVsIso2th1);
    if (t->GetBranch("pfcVsIso3th1")) t->SetBranchAddress("pfcVsIso3th1", &pfcVsIso3th1, &b_pfcVsIso3th1);
    if (t->GetBranch("pfcVsIso4th1")) t->SetBranchAddress("pfcVsIso4th1", &pfcVsIso4th1, &b_pfcVsIso4th1);
    if (t->GetBranch("pfcVsIso5th1")) t->SetBranchAddress("pfcVsIso5th1", &pfcVsIso5th1, &b_pfcVsIso5th1);
    if (t->GetBranch("pfcVsIso1th2")) t->SetBranchAddress("pfcVsIso1th2", &pfcVsIso1th2, &b_pfcVsIso1th2);
    if (t->GetBranch("pfcVsIso2th2")) t->SetBranchAddress("pfcVsIso2th2", &pfcVsIso2th2, &b_pfcVsIso2th2);
    if (t->GetBranch("pfcVsIso3th2")) t->SetBranchAddress("pfcVsIso3th2", &pfcVsIso3th2, &b_pfcVsIso3th2);
    if (t->GetBranch("pfcVsIso4th2")) t->SetBranchAddress("pfcVsIso4th2", &pfcVsIso4th2, &b_pfcVsIso4th2);
    if (t->GetBranch("pfcVsIso5th2")) t->SetBranchAddress("pfcVsIso5th2", &pfcVsIso5th2, &b_pfcVsIso5th2);
    if (t->GetBranch("pfnVsIso1")) t->SetBranchAddress("pfnVsIso1", &pfnVsIso1, &b_pfnVsIso1);
    if (t->GetBranch("pfnVsIso2")) t->SetBranchAddress("pfnVsIso2", &pfnVsIso2, &b_pfnVsIso2);
    if (t->GetBranch("pfnVsIso3")) t->SetBranchAddress("pfnVsIso3", &pfnVsIso3, &b_pfnVsIso3);
    if (t->GetBranch("pfnVsIso4")) t->SetBranchAddress("pfnVsIso4", &pfnVsIso4, &b_pfnVsIso4);
    if (t->GetBranch("pfnVsIso5")) t->SetBranchAddress("pfnVsIso5", &pfnVsIso5, &b_pfnVsIso5);
    if (t->GetBranch("pfnVsIso1th1")) t->SetBranchAddress("pfnVsIso1th1", &pfnVsIso1th1, &b_pfnVsIso1th1);
    if (t->GetBranch("pfnVsIso2th1")) t->SetBranchAddress("pfnVsIso2th1", &pfnVsIso2th1, &b_pfnVsIso2th1);
    if (t->GetBranch("pfnVsIso3th1")) t->SetBranchAddress("pfnVsIso3th1", &pfnVsIso3th1, &b_pfnVsIso3th1);
    if (t->GetBranch("pfnVsIso4th1")) t->SetBranchAddress("pfnVsIso4th1", &pfnVsIso4th1, &b_pfnVsIso4th1);
    if (t->GetBranch("pfnVsIso5th1")) t->SetBranchAddress("pfnVsIso5th1", &pfnVsIso5th1, &b_pfnVsIso5th1);
    if (t->GetBranch("pfnVsIso1th2")) t->SetBranchAddress("pfnVsIso1th2", &pfnVsIso1th2, &b_pfnVsIso1th2);
    if (t->GetBranch("pfnVsIso2th2")) t->SetBranchAddress("pfnVsIso2th2", &pfnVsIso2th2, &b_pfnVsIso2th2);
    if (t->GetBranch("pfnVsIso3th2")) t->SetBranchAddress("pfnVsIso3th2", &pfnVsIso3th2, &b_pfnVsIso3th2);
    if (t->GetBranch("pfnVsIso4th2")) t->SetBranchAddress("pfnVsIso4th2", &pfnVsIso4th2, &b_pfnVsIso4th2);
    if (t->GetBranch("pfnVsIso5th2")) t->SetBranchAddress("pfnVsIso5th2", &pfnVsIso5th2, &b_pfnVsIso5th2);
    if (t->GetBranch("pfpVsIso1")) t->SetBranchAddress("pfpVsIso1", &pfpVsIso1, &b_pfpVsIso1);
    if (t->GetBranch("pfpVsIso2")) t->SetBranchAddress("pfpVsIso2", &pfpVsIso2, &b_pfpVsIso2);
    if (t->GetBranch("pfpVsIso3")) t->SetBranchAddress("pfpVsIso3", &pfpVsIso3, &b_pfpVsIso3);
    if (t->GetBranch("pfpVsIso4")) t->SetBranchAddress("pfpVsIso4", &pfpVsIso4, &b_pfpVsIso4);
    if (t->GetBranch("pfpVsIso5")) t->SetBranchAddress("pfpVsIso5", &pfpVsIso5, &b_pfpVsIso5);
    if (t->GetBranch("pfpVsIso1th1")) t->SetBranchAddress("pfpVsIso1th1", &pfpVsIso1th1, &b_pfpVsIso1th1);
    if (t->GetBranch("pfpVsIso2th1")) t->SetBranchAddress("pfpVsIso2th1", &pfpVsIso2th1, &b_pfpVsIso2th1);
    if (t->GetBranch("pfpVsIso3th1")) t->SetBranchAddress("pfpVsIso3th1", &pfpVsIso3th1, &b_pfpVsIso3th1);
    if (t->GetBranch("pfpVsIso4th1")) t->SetBranchAddress("pfpVsIso4th1", &pfpVsIso4th1, &b_pfpVsIso4th1);
    if (t->GetBranch("pfpVsIso5th1")) t->SetBranchAddress("pfpVsIso5th1", &pfpVsIso5th1, &b_pfpVsIso5th1);
    if (t->GetBranch("pfpVsIso1th2")) t->SetBranchAddress("pfpVsIso1th2", &pfpVsIso1th2, &b_pfpVsIso1th2);
    if (t->GetBranch("pfpVsIso2th2")) t->SetBranchAddress("pfpVsIso2th2", &pfpVsIso2th2, &b_pfpVsIso2th2);
    if (t->GetBranch("pfpVsIso3th2")) t->SetBranchAddress("pfpVsIso3th2", &pfpVsIso3th2, &b_pfpVsIso3th2);
    if (t->GetBranch("pfpVsIso4th2")) t->SetBranchAddress("pfpVsIso4th2", &pfpVsIso4th2, &b_pfpVsIso4th2);
    if (t->GetBranch("pfpVsIso5th2")) t->SetBranchAddress("pfpVsIso5th2", &pfpVsIso5th2, &b_pfpVsIso5th2);
    if (t->GetBranch("towerIso1")) t->SetBranchAddress("towerIso1", &towerIso1, &b_towerIso1);
    if (t->GetBranch("towerIso2")) t->SetBranchAddress("towerIso2", &towerIso2, &b_towerIso2);
    if (t->GetBranch("towerIso3")) t->SetBranchAddress("towerIso3", &towerIso3, &b_towerIso3);
    if (t->GetBranch("towerIso4")) t->SetBranchAddress("towerIso4", &towerIso4, &b_towerIso4);
    if (t->GetBranch("towerIso5")) t->SetBranchAddress("towerIso5", &towerIso5, &b_towerIso5);
    if (t->GetBranch("towerVsIso1")) t->SetBranchAddress("towerVsIso1", &towerVsIso1, &b_towerVsIso1);
    if (t->GetBranch("towerVsIso2")) t->SetBranchAddress("towerVsIso2", &towerVsIso2, &b_towerVsIso2);
    if (t->GetBranch("towerVsIso3")) t->SetBranchAddress("towerVsIso3", &towerVsIso3, &b_towerVsIso3);
    if (t->GetBranch("towerVsIso4")) t->SetBranchAddress("towerVsIso4", &towerVsIso4, &b_towerVsIso4);
    if (t->GetBranch("towerVsIso5")) t->SetBranchAddress("towerVsIso5", &towerVsIso5, &b_towerVsIso5);
    if (t->GetBranch("towerVsSubIso1")) t->SetBranchAddress("towerVsSubIso1", &towerVsSubIso1, &b_towerVsSubIso1);
    if (t->GetBranch("towerVsSubIso2")) t->SetBranchAddress("towerVsSubIso2", &towerVsSubIso2, &b_towerVsSubIso2);
    if (t->GetBranch("towerVsSubIso3")) t->SetBranchAddress("towerVsSubIso3", &towerVsSubIso3, &b_towerVsSubIso3);
    if (t->GetBranch("towerVsSubIso4")) t->SetBranchAddress("towerVsSubIso4", &towerVsSubIso4, &b_towerVsSubIso4);
    if (t->GetBranch("towerVsSubIso5")) t->SetBranchAddress("towerVsSubIso5", &towerVsSubIso5, &b_towerVsSubIso5);
}

#endif /* TREEHEADERS_GGHINTUPLIZERTREE_PHOTONRAA_H_ */
