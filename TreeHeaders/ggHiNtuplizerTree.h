#ifndef TREEHEADERS_GGHINTUPLIZERTREE_H_
#define TREEHEADERS_GGHINTUPLIZERTREE_H_

#include <TTree.h>
#include <TBranch.h>

#include <vector>

#include "../Utilities/eventUtil.h"
#include "hiFJRhoTree.h"

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
    pho_genMatchedIndex = 0;
    eleCharge = 0;
    eleChargeConsistent = 0;
    eleSCPixCharge = 0;
    eleCtfCharge = 0;
    eleEn = 0;
    eleD0 = 0;
    eleDz = 0;
    eleIP3D = 0;
    eleD0Err = 0;
    eleDzErr = 0;
    eleIP3DErr = 0;
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
    eledEtaSeedAtVtx = 0;
    eleSigmaIEtaIEta = 0;
    eleSigmaIEtaIEta_2012 = 0;
    eleSigmaIPhiIPhi = 0;
    eleConvVeto = 0;
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
    elePFRelIsoWithEA = 0;
    eleEffAreaTimesRho = 0;
    phoE = 0;
    phoEt = 0;
    phoEtCorrected = 0;
    phoEtCorrected_sys = 0;
    phoEta = 0;
    phoPhi = 0;
    phoSCE = 0;
    phoSCRawE = 0;
    phoESEn = 0;
    phoSCEta = 0;
    phoSCPhi = 0;
    phoSCEtaWidth = 0;
    phoSCPhiWidth = 0;
    phoSCBrem = 0;

    phoESEffSigmaRR = 0;

    phoHasPixelSeed = 0;
    phoHasConversionTracks = 0;
    phoHadTowerOverEm = 0;
    phoHoverE = 0;
    phoHoverEValid = 0;
    phoSigmaIEtaIEta = 0;
    phoR9 = 0;
    phoE1x5 = 0;
    phoE2x5 = 0;
    phoE3x3 = 0;
    phoE5x5 = 0;
    phoMaxEnergyXtal = 0;
    phoSigmaEtaEta = 0;
    phoSigmaIEtaIEta_2012 = 0;
    phoR9_2012 = 0;
    phoE1x5_2012 = 0;
    phoE2x5_2012 = 0;
    phoE3x3_2012 = 0;
    phoE5x5_2012 = 0;
    phoMaxEnergyXtal_2012 = 0;
    phoSigmaEtaEta_2012 = 0;

    phoHadTowerOverEm1 = 0;
    phoHadTowerOverEm2 = 0;
    phoHoverE1 = 0;
    phoHoverE2 = 0;

    phoSigmaIEtaIPhi = 0;
    phoSigmaIPhiIPhi = 0;
    phoR1x5 = 0;
    phoR2x5 = 0;
    phoE2nd = 0;
    phoETop = 0;
    phoEBottom = 0;
    phoELeft = 0;
    phoERight = 0;
    phoE1x3 = 0;
    phoE2x2 = 0;
    phoE2x5Max = 0;
    phoE2x5Top = 0;
    phoE2x5Bottom = 0;
    phoE2x5Left = 0;
    phoE2x5Right = 0;
    //phoSMMajor = 0;   // TODO: enable when they become available in future releases
    //phoSMMinor = 0;   // TODO: enable when they become available in future releases
    //phoSMAlpha = 0;   // TODO: enable when they become available in future releases

    phoSigmaIEtaIPhi_2012 = 0;
    phoSigmaIPhiIPhi_2012 = 0;
    phoR1x5_2012 = 0;
    phoR2x5_2012 = 0;
    phoE2nd_2012 = 0;
    phoETop_2012 = 0;
    phoEBottom_2012 = 0;
    phoELeft_2012 = 0;
    phoERight_2012 = 0;
    phoE1x3_2012 = 0;
    phoE2x2_2012 = 0;
    phoE2x5Max_2012 = 0;
    phoE2x5Top_2012 = 0;
    phoE2x5Bottom_2012 = 0;
    phoE2x5Left_2012 = 0;
    phoE2x5Right_2012 = 0;
    //phoSMMajor_2012 = 0;   // TODO: enable when they become available in future releases
    //phoSMMinor_2012 = 0;   // TODO: enable when they become available in future releases
    //phoSMAlpha_2012 = 0;   // TODO: enable when they become available in future releases

    phoBC1E = 0;
    phoBC1Eta = 0;
    phoBC2E = 0;
    phoBC2Eta = 0;
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
    pfpIso1subSC = 0;
    pfpIso2subSC = 0;
    pfpIso3subSC = 0;
    pfpIso4subSC = 0;
    pfpIso5subSC = 0;
    pfcIso1subUE = 0;
    pfcIso2subUE = 0;
    pfcIso3subUE = 0;
    pfcIso4subUE = 0;
    pfcIso5subUE = 0;
    pfpIso1subUE = 0;
    pfpIso2subUE = 0;
    pfpIso3subUE = 0;
    pfpIso4subUE = 0;
    pfpIso5subUE = 0;
    pfnIso1subUE = 0;
    pfnIso2subUE = 0;
    pfnIso3subUE = 0;
    pfnIso4subUE = 0;
    pfnIso5subUE = 0;
    pfpIso1subSCsubUE = 0;
    pfpIso2subSCsubUE = 0;
    pfpIso3subSCsubUE = 0;
    pfpIso4subSCsubUE = 0;
    pfpIso5subSCsubUE = 0;
    pfcIso1pTgt1p0subUE = 0;
    pfcIso2pTgt1p0subUE = 0;
    pfcIso3pTgt1p0subUE = 0;
    pfcIso4pTgt1p0subUE = 0;
    pfcIso5pTgt1p0subUE = 0;
    pfcIso1pTgt2p0subUE = 0;
    pfcIso2pTgt2p0subUE = 0;
    pfcIso3pTgt2p0subUE = 0;
    pfcIso4pTgt2p0subUE = 0;
    pfcIso5pTgt2p0subUE = 0;
    pfcIso1pTgt3p0subUE = 0;
    pfcIso2pTgt3p0subUE = 0;
    pfcIso3pTgt3p0subUE = 0;
    pfcIso4pTgt3p0subUE = 0;
    pfcIso5pTgt3p0subUE = 0;
    muPt = 0;
    muEta = 0;
    muPhi = 0;
    muCharge = 0;
    muType = 0;
    muIsGood = 0;
    muIsGlobal = 0;
    muIsTracker = 0;
    muIsPF = 0;
    muIsSTA = 0;
    muD0 = 0;
    muDz = 0;
    muChi2NDF = 0;
    muInnerD0 = 0;
    muInnerDz = 0;
    muInnerD0Err = 0;
    muInnerDzErr = 0;
    muInnerPt = 0;
    muInnerPtErr = 0;
    muInnerEta = 0;
    muTrkLayers = 0;
    muPixelLayers = 0;
    muPixelHits = 0;
    muMuonHits = 0;
    muTrkQuality = 0;
    muStations = 0;
    muIsoTrk = 0;
    muPFChIso = 0;
    muPFPhoIso = 0;
    muPFNeuIso = 0;
    muPFPUIso = 0;

  };
  ~ggHiNtuplizer(){};
  void setupTreeForReading(TTree *t);
  bool is2015EcalNoise(int iPho);
  bool passedPhoSpikeRejection(int i);
  bool passedHI18HEMfailurePho(int i);
  bool passedHI18HEMfailureEle(int i);
  bool passedHI18HEMfailureMu(int i);
  bool passedHI18HEMfailureGen(int i);
  bool passedEleSelection(int i, int collType, int hiBin, double eleRho = -1, int WPindex = 0);
  bool passedMuSelection(int i, int collType);
  double getElePtCorrFactor(int i, int collType, int hiBin);
  double getElePtSmearFactor(int i, int collType, int hiBin);
  double getHiFJRho4Ele(int i, hiFJRho& hifjrho);
  double getElePFIsoSubRho(int i, double eleRho);
  double getValueByName(int i, std::string varName);
  void copy2Vars(int i, float *vals, std::vector<std::string>& varNames, int nVars, int offset);

  // Declaration of leaf types
  UInt_t          run;
  ULong64_t       event;
  UInt_t          lumis;
  Bool_t          isData;
  Float_t         rho;
  Float_t         angEvtPlane;
  Int_t           indexEvtPlane;
  Int_t           phi_nTot;
  Int_t           phi_minBinN;
  Float_t         phi_fit_chi2;
  Float_t         phi_fit_chi2prob;
  Float_t         phi_fit_v2;
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
  Int_t           nEle;
  std::vector<int>     *eleCharge;
  std::vector<int>     *eleChargeConsistent;
  std::vector<int>     *eleSCPixCharge;
  std::vector<int>     *eleCtfCharge;
  std::vector<float>   *eleEn;
  std::vector<float>   *eleD0;
  std::vector<float>   *eleDz;
  std::vector<float>   *eleIP3D;
  std::vector<float>   *eleD0Err;
  std::vector<float>   *eleDzErr;
  std::vector<float>   *eleIP3DErr;
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
  std::vector<float>   *eledEtaSeedAtVtx;
  std::vector<float>   *eleSigmaIEtaIEta;
  std::vector<float>   *eleSigmaIEtaIEta_2012;
  std::vector<float>   *eleSigmaIPhiIPhi;
  std::vector<int>     *eleConvVeto;
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
  std::vector<float>   *elePFRelIsoWithEA;
  std::vector<float>   *eleEffAreaTimesRho;
  Int_t           nPho;
  std::vector<float>   *phoE;
  std::vector<float>   *phoEt;
  std::vector<float>   *phoEtCorrected;
  std::vector<float>   *phoEtCorrected_sys;
  std::vector<float>   *phoEta;
  std::vector<float>   *phoPhi;
  std::vector<float>   *phoSCE;
  std::vector<float>   *phoSCRawE;
  std::vector<float>   *phoESEn;
  std::vector<float>   *phoSCEta;
  std::vector<float>   *phoSCPhi;
  std::vector<float>   *phoSCEtaWidth;
  std::vector<float>   *phoSCPhiWidth;
  std::vector<float>   *phoSCBrem;

  std::vector<float>   *phoESEffSigmaRR;

  std::vector<int>     *phoHasPixelSeed;
  std::vector<int>     *phoHasConversionTracks;
  std::vector<float>   *phoHadTowerOverEm;
  std::vector<float>   *phoHoverE;
  std::vector<int>     *phoHoverEValid;
  std::vector<float>   *phoSigmaIEtaIEta;
  std::vector<float>   *phoR9;
  std::vector<float>   *phoE1x5;
  std::vector<float>   *phoE2x5;
  std::vector<float>   *phoE3x3;
  std::vector<float>   *phoE5x5;
  std::vector<float>   *phoMaxEnergyXtal;
  std::vector<float>   *phoSigmaEtaEta;
  std::vector<float>   *phoSigmaIEtaIEta_2012;
  std::vector<float>   *phoR9_2012;
  std::vector<float>   *phoE1x5_2012;
  std::vector<float>   *phoE2x5_2012;
  std::vector<float>   *phoE3x3_2012;
  std::vector<float>   *phoE5x5_2012;
  std::vector<float>   *phoMaxEnergyXtal_2012;
  std::vector<float>   *phoSigmaEtaEta_2012;

  Int_t pho_isEle;
  Int_t pho_is2015Noise;

  std::vector<float>  *phoHadTowerOverEm1;
  std::vector<float>  *phoHadTowerOverEm2;
  std::vector<float>  *phoHoverE1;
  std::vector<float>  *phoHoverE2;

  std::vector<float>  *phoSigmaIEtaIPhi;
  std::vector<float>  *phoSigmaIPhiIPhi;
  std::vector<float>  *phoR1x5;
  std::vector<float>  *phoR2x5;
  std::vector<float>  *phoE2nd;
  std::vector<float>  *phoETop;
  std::vector<float>  *phoEBottom;
  std::vector<float>  *phoELeft;
  std::vector<float>  *phoERight;
  std::vector<float>  *phoE1x3;
  std::vector<float>  *phoE2x2;
  std::vector<float>  *phoE2x5Max;
  std::vector<float>  *phoE2x5Top;
  std::vector<float>  *phoE2x5Bottom;
  std::vector<float>  *phoE2x5Left;
  std::vector<float>  *phoE2x5Right;
  //std::vector<float>  *phoSMMajor;   // TODO: enable when they become available in future releases
  //std::vector<float>  *phoSMMinor;   // TODO: enable when they become available in future releases
  //std::vector<float>  *phoSMAlpha;   // TODO: enable when they become available in future releases

  std::vector<float>  *phoSigmaIEtaIPhi_2012;
  std::vector<float>  *phoSigmaIPhiIPhi_2012;
  std::vector<float>  *phoR1x5_2012;
  std::vector<float>  *phoR2x5_2012;
  std::vector<float>  *phoE2nd_2012;
  std::vector<float>  *phoETop_2012;
  std::vector<float>  *phoEBottom_2012;
  std::vector<float>  *phoELeft_2012;
  std::vector<float>  *phoERight_2012;
  std::vector<float>  *phoE1x3_2012;
  std::vector<float>  *phoE2x2_2012;
  std::vector<float>  *phoE2x5Max_2012;
  std::vector<float>  *phoE2x5Top_2012;
  std::vector<float>  *phoE2x5Bottom_2012;
  std::vector<float>  *phoE2x5Left_2012;
  std::vector<float>  *phoE2x5Right_2012;
  //std::vector<float>  *phoSMMajor_2012;   // TODO: enable when they become available in future releases
  //std::vector<float>  *phoSMMinor_2012;   // TODO: enable when they become available in future releases
//std::vector<float>  *phoSMAlpha_2012;   // TODO: enable when they become available in future releases

  std::vector<float>   *phoBC1E;
  std::vector<float>   *phoBC1Eta;
  std::vector<float>   *phoBC2E;
  std::vector<float>   *phoBC2Eta;
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
  std::vector<float>   *pfpIso1subSC;
  std::vector<float>   *pfpIso2subSC;
  std::vector<float>   *pfpIso3subSC;
  std::vector<float>   *pfpIso4subSC;
  std::vector<float>   *pfpIso5subSC;
  std::vector<float>   *pfcIso1subUE;
  std::vector<float>   *pfcIso2subUE;
  std::vector<float>   *pfcIso3subUE;
  std::vector<float>   *pfcIso4subUE;
  std::vector<float>   *pfcIso5subUE;
  std::vector<float>   *pfpIso1subUE;
  std::vector<float>   *pfpIso2subUE;
  std::vector<float>   *pfpIso3subUE;
  std::vector<float>   *pfpIso4subUE;
  std::vector<float>   *pfpIso5subUE;
  std::vector<float>   *pfnIso1subUE;
  std::vector<float>   *pfnIso2subUE;
  std::vector<float>   *pfnIso3subUE;
  std::vector<float>   *pfnIso4subUE;
  std::vector<float>   *pfnIso5subUE;
  std::vector<float>   *pfpIso1subSCsubUE;
  std::vector<float>   *pfpIso2subSCsubUE;
  std::vector<float>   *pfpIso3subSCsubUE;
  std::vector<float>   *pfpIso4subSCsubUE;
  std::vector<float>   *pfpIso5subSCsubUE;
  std::vector<float>   *pfcIso1pTgt1p0subUE;
  std::vector<float>   *pfcIso2pTgt1p0subUE;
  std::vector<float>   *pfcIso3pTgt1p0subUE;
  std::vector<float>   *pfcIso4pTgt1p0subUE;
  std::vector<float>   *pfcIso5pTgt1p0subUE;
  std::vector<float>   *pfcIso1pTgt2p0subUE;
  std::vector<float>   *pfcIso2pTgt2p0subUE;
  std::vector<float>   *pfcIso3pTgt2p0subUE;
  std::vector<float>   *pfcIso4pTgt2p0subUE;
  std::vector<float>   *pfcIso5pTgt2p0subUE;
  std::vector<float>   *pfcIso1pTgt3p0subUE;
  std::vector<float>   *pfcIso2pTgt3p0subUE;
  std::vector<float>   *pfcIso3pTgt3p0subUE;
  std::vector<float>   *pfcIso4pTgt3p0subUE;
  std::vector<float>   *pfcIso5pTgt3p0subUE;
  Int_t           nMu;
  std::vector<float>   *muPt;
  std::vector<float>   *muEta;
  std::vector<float>   *muPhi;
  std::vector<int>     *muCharge;
  std::vector<int>     *muType;
  std::vector<int>     *muIsGood;
  std::vector<int>     *muIsGlobal;
  std::vector<int>     *muIsTracker;
  std::vector<int>     *muIsPF;
  std::vector<int>     *muIsSTA;
  std::vector<float>   *muD0;
  std::vector<float>   *muDz;
  std::vector<float>   *muChi2NDF;
  std::vector<float>   *muInnerD0;
  std::vector<float>   *muInnerDz;
  std::vector<float>   *muInnerD0Err;
  std::vector<float>   *muInnerDzErr;
  std::vector<float>   *muInnerPt;
  std::vector<float>   *muInnerPtErr;
  std::vector<float>   *muInnerEta;
  std::vector<int>     *muTrkLayers;
  std::vector<int>     *muPixelLayers;
  std::vector<int>     *muPixelHits;
  std::vector<int>     *muMuonHits;
  std::vector<int>     *muTrkQuality;
  std::vector<int>     *muStations;
  std::vector<float>   *muIsoTrk;
  std::vector<float>   *muPFChIso;
  std::vector<float>   *muPFPhoIso;
  std::vector<float>   *muPFNeuIso;
  std::vector<float>   *muPFPUIso;
  std::vector<bool>    *isGenMatched; // added by hand
  std::vector<int>     *genMatchedIdx; // added by hand

  // List of branches
  TBranch        *b_run;   //!
  TBranch        *b_event;   //!
  TBranch        *b_lumis;   //!
  TBranch        *b_isData;   //!
  TBranch        *b_rho;   //!
  TBranch        *b_angEvtPlane;   //!
  TBranch        *b_indexEvtPlane;   //!
  TBranch        *b_phi_nTot;   //!
  TBranch        *b_phi_minBinN;   //!
  TBranch        *b_phi_fit_chi2;   //!
  TBranch        *b_phi_fit_chi2prob;   //!
  TBranch        *b_phi_fit_v2;   //!
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
  TBranch        *b_nEle;   //!
  TBranch        *b_eleCharge;   //!
  TBranch        *b_eleChargeConsistent;   //!
  TBranch        *b_eleSCPixCharge;   //!
  TBranch        *b_eleCtfCharge;   //!
  TBranch        *b_eleEn;   //!
  TBranch        *b_eleD0;   //!
  TBranch        *b_eleDz;   //!
  TBranch        *b_eleIP3D;   //!
  TBranch        *b_eleD0Err;   //!
  TBranch        *b_eleDzErr;   //!
  TBranch        *b_eleIP3DErr;   //!
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
  TBranch        *b_eledEtaSeedAtVtx;   //!
  TBranch        *b_eleSigmaIEtaIEta;   //!
  TBranch        *b_eleSigmaIEtaIEta_2012;   //!
  TBranch        *b_eleSigmaIPhiIPhi;   //!
  TBranch        *b_eleConvVeto;   //!
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
  TBranch        *b_elePFRelIsoWithEA;   //!
  TBranch        *b_eleEffAreaTimesRho;   //!
  TBranch        *b_nPho;   //!
  TBranch        *b_phoE;   //!
  TBranch        *b_phoEt;   //!
  TBranch        *b_phoEtCorrected;   //!
  TBranch        *b_phoEtCorrected_sys;   //!
  TBranch        *b_phoEta;   //!
  TBranch        *b_phoPhi;   //!
  TBranch        *b_phoSCE;   //!
  TBranch        *b_phoSCRawE;   //!
  TBranch        *b_phoESEn;   //!
  TBranch        *b_phoSCEta;   //!
  TBranch        *b_phoSCPhi;   //!
  TBranch        *b_phoSCEtaWidth;   //!
  TBranch        *b_phoSCPhiWidth;   //!
  TBranch        *b_phoSCBrem;   //!

  TBranch        *b_phoESEffSigmaRR;

  TBranch        *b_phoHasPixelSeed;   //!
  TBranch        *b_phoHasConversionTracks;
  TBranch        *b_phoHadTowerOverEm;
  TBranch        *b_phoHoverE;
  TBranch        *b_phoHoverEValid;
  TBranch        *b_phoSigmaIEtaIEta;
  TBranch        *b_phoR9;
  TBranch        *b_phoE1x5;
  TBranch        *b_phoE2x5;
  TBranch        *b_phoE3x3;
  TBranch        *b_phoE5x5;
  TBranch        *b_phoMaxEnergyXtal;
  TBranch        *b_phoSigmaEtaEta;
  TBranch        *b_phoSigmaIEtaIEta_2012;
  TBranch        *b_phoR9_2012;
  TBranch        *b_phoE1x5_2012;
  TBranch        *b_phoE2x5_2012;
  TBranch        *b_phoE3x3_2012;
  TBranch        *b_phoE5x5_2012;
  TBranch        *b_phoMaxEnergyXtal_2012;
  TBranch        *b_phoSigmaEtaEta_2012;

  TBranch        *b_pho_isEle;
  TBranch        *b_pho_is2015Noise;

  TBranch        *b_phoHadTowerOverEm1;
  TBranch        *b_phoHadTowerOverEm2;
  TBranch        *b_phoHoverE1;
  TBranch        *b_phoHoverE2;

  TBranch        *b_phoSigmaIEtaIPhi;
  TBranch        *b_phoSigmaIPhiIPhi;
  TBranch        *b_phoR1x5;
  TBranch        *b_phoR2x5;
  TBranch        *b_phoE2nd;
  TBranch        *b_phoETop;
  TBranch        *b_phoEBottom;
  TBranch        *b_phoELeft;
  TBranch        *b_phoERight;
  TBranch        *b_phoE1x3;
  TBranch        *b_phoE2x2;
  TBranch        *b_phoE2x5Max;
  TBranch        *b_phoE2x5Top;
  TBranch        *b_phoE2x5Bottom;
  TBranch        *b_phoE2x5Left;
  TBranch        *b_phoE2x5Right;
  //TBranch        *b_phoSMMajor;   // TODO: enable when they become available in future releases
  //TBranch        *b_phoSMMinor;   // TODO: enable when they become available in future releases
  //TBranch        *b_phoSMAlpha;   // TODO: enable when they become available in future releases

  TBranch        *b_phoSigmaIEtaIPhi_2012;
  TBranch        *b_phoSigmaIPhiIPhi_2012;
  TBranch        *b_phoR1x5_2012;
  TBranch        *b_phoR2x5_2012;
  TBranch        *b_phoE2nd_2012;
  TBranch        *b_phoETop_2012;
  TBranch        *b_phoEBottom_2012;
  TBranch        *b_phoELeft_2012;
  TBranch        *b_phoERight_2012;
  TBranch        *b_phoE1x3_2012;
  TBranch        *b_phoE2x2_2012;
  TBranch        *b_phoE2x5Max_2012;
  TBranch        *b_phoE2x5Top_2012;
  TBranch        *b_phoE2x5Bottom_2012;
  TBranch        *b_phoE2x5Left_2012;
  TBranch        *b_phoE2x5Right_2012;
  //TBranch        *b_phoSMMajor_2012;   // TODO: enable when they become available in future releases
  //TBranch        *b_phoSMMinor_2012;   // TODO: enable when they become available in future releases
  //TBranch        *b_phoSMAlpha_2012;   // TODO: enable when they become available in future releases

  TBranch        *b_phoBC1E;   //!
  TBranch        *b_phoBC1Eta;   //!
  TBranch        *b_phoBC2E;   //!
  TBranch        *b_phoBC2Eta;   //!
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
  TBranch        *b_pho_sumIsoCorrected;
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
  TBranch        *b_pfpIso1subSC;   //!
  TBranch        *b_pfpIso2subSC;   //!
  TBranch        *b_pfpIso3subSC;   //!
  TBranch        *b_pfpIso4subSC;   //!
  TBranch        *b_pfpIso5subSC;   //!
  TBranch        *b_pfcIso1subUE;   //!
  TBranch        *b_pfcIso2subUE;   //!
  TBranch        *b_pfcIso3subUE;   //!
  TBranch        *b_pfcIso4subUE;   //!
  TBranch        *b_pfcIso5subUE;   //!
  TBranch        *b_pfpIso1subUE;   //!
  TBranch        *b_pfpIso2subUE;   //!
  TBranch        *b_pfpIso3subUE;   //!
  TBranch        *b_pfpIso4subUE;   //!
  TBranch        *b_pfpIso5subUE;   //!
  TBranch        *b_pfnIso1subUE;   //!
  TBranch        *b_pfnIso2subUE;   //!
  TBranch        *b_pfnIso3subUE;   //!
  TBranch        *b_pfnIso4subUE;   //!
  TBranch        *b_pfnIso5subUE;   //!
  TBranch        *b_pfpIso1subSCsubUE;   //!
  TBranch        *b_pfpIso2subSCsubUE;   //!
  TBranch        *b_pfpIso3subSCsubUE;   //!
  TBranch        *b_pfpIso4subSCsubUE;   //!
  TBranch        *b_pfpIso5subSCsubUE;   //!
  TBranch        *b_pfcIso1pTgt1p0subUE;   //!
  TBranch        *b_pfcIso2pTgt1p0subUE;   //!
  TBranch        *b_pfcIso3pTgt1p0subUE;   //!
  TBranch        *b_pfcIso4pTgt1p0subUE;   //!
  TBranch        *b_pfcIso5pTgt1p0subUE;   //!
  TBranch        *b_pfcIso1pTgt2p0subUE;   //!
  TBranch        *b_pfcIso2pTgt2p0subUE;   //!
  TBranch        *b_pfcIso3pTgt2p0subUE;   //!
  TBranch        *b_pfcIso4pTgt2p0subUE;   //!
  TBranch        *b_pfcIso5pTgt2p0subUE;   //!
  TBranch        *b_pfcIso1pTgt3p0subUE;   //!
  TBranch        *b_pfcIso2pTgt3p0subUE;   //!
  TBranch        *b_pfcIso3pTgt3p0subUE;   //!
  TBranch        *b_pfcIso4pTgt3p0subUE;   //!
  TBranch        *b_pfcIso5pTgt3p0subUE;   //!
  TBranch        *b_nMu;   //!
  TBranch        *b_muPt;   //!
  TBranch        *b_muEta;   //!
  TBranch        *b_muPhi;   //!
  TBranch        *b_muCharge;   //!
  TBranch        *b_muType;   //!
  TBranch        *b_muIsGood;   //!
  TBranch        *b_muIsGlobal;   //!
  TBranch        *b_muIsTracker;   //!
  TBranch        *b_muIsPF;   //!
  TBranch        *b_muIsSTA;   //!
  TBranch        *b_muD0;   //!
  TBranch        *b_muDz;   //!
  TBranch        *b_muChi2NDF;   //!
  TBranch        *b_muInnerD0;   //!
  TBranch        *b_muInnerDz;   //!
  TBranch        *b_muInnerD0Err;   //!
  TBranch        *b_muInnerDzErr;   //!
  TBranch        *b_muInnerPt;   //!
  TBranch        *b_muInnerPtErr;   //!
  TBranch        *b_muInnerEta;   //!
  TBranch        *b_muTrkLayers;   //!
  TBranch        *b_muPixelLayers;   //!
  TBranch        *b_muPixelHits;   //!
  TBranch        *b_muMuonHits;   //!
  TBranch        *b_muTrkQuality;   //!
  TBranch        *b_muStations;   //!
  TBranch        *b_muIsoTrk;   //!
  TBranch        *b_muPFChIso;   //!
  TBranch        *b_muPFPhoIso;   //!
  TBranch        *b_muPFNeuIso;   //!
  TBranch        *b_muPFPUIso;   //!
};

void ggHiNtuplizer::setupTreeForReading(TTree *t)
{
    b_run = 0;
    b_event = 0;
    b_lumis = 0;
    b_isData = 0;
    b_rho = 0;
    b_angEvtPlane = 0;
    b_indexEvtPlane = 0;
    b_phi_nTot = 0;
    b_phi_minBinN = 0;
    b_phi_fit_chi2 = 0;
    b_phi_fit_chi2prob = 0;
    b_phi_fit_v2 = 0;
    b_nPUInfo = 0;
    b_nPU = 0;
    b_puBX = 0;
    b_puTrue = 0;
    b_nMC = 0;
    b_mcPID = 0;
    b_mcStatus = 0;
    b_mcVtx_x = 0;
    b_mcVtx_y = 0;
    b_mcVtx_z = 0;
    b_mcPt = 0;
    b_mcEta = 0;
    b_mcPhi = 0;
    b_mcE = 0;
    b_mcEt = 0;
    b_mcMass = 0;
    b_mcParentage = 0;
    b_mcMomPID = 0;
    b_mcMomPt = 0;
    b_mcMomEta = 0;
    b_mcMomPhi = 0;
    b_mcMomMass = 0;
    b_mcGMomPID = 0;
    b_mcIndex = 0;
    b_mcCalIsoDR03 = 0;
    b_mcCalIsoDR04 = 0;
    b_mcTrkIsoDR03 = 0;
    b_mcTrkIsoDR04 = 0;
    b_pho_genMatchedIndex = 0;
    b_nEle = 0;
    b_eleCharge = 0;
    b_eleChargeConsistent = 0;
    b_eleSCPixCharge = 0;
    b_eleCtfCharge = 0;
    b_eleEn = 0;
    b_eleD0 = 0;
    b_eleDz = 0;
    b_eleIP3D = 0;
    b_eleD0Err = 0;
    b_eleDzErr = 0;
    b_eleIP3DErr = 0;
    b_eleTrkPt = 0;
    b_eleTrkEta = 0;
    b_eleTrkPhi = 0;
    b_eleTrkCharge = 0;
    b_eleTrkChi2 = 0;
    b_eleTrkNdof = 0;
    b_eleTrkNormalizedChi2 = 0;
    b_eleTrkValidHits = 0;
    b_eleTrkLayers = 0;
    b_elePt = 0;
    b_eleEta = 0;
    b_elePhi = 0;
    b_eleSCEn = 0;
    b_eleESEn = 0;
    b_eleSCEta = 0;
    b_eleSCPhi = 0;
    b_eleSCRawEn = 0;
    b_eleSCEtaWidth = 0;
    b_eleSCPhiWidth = 0;
    b_eleHoverE = 0;
    b_eleHoverEBc = 0;
    b_eleEoverP = 0;
    b_eleEoverPInv = 0;
    b_eleBrem = 0;
    b_eledEtaAtVtx = 0;
    b_eledPhiAtVtx = 0;
    b_eledEtaSeedAtVtx = 0;
    b_eleSigmaIEtaIEta = 0;
    b_eleSigmaIEtaIEta_2012 = 0;
    b_eleSigmaIPhiIPhi = 0;
    b_eleConvVeto = 0;
    b_eleMissHits = 0;
    b_eleESEffSigmaRR = 0;
    b_elePFChIso = 0;
    b_elePFPhoIso = 0;
    b_elePFNeuIso = 0;
    b_elePFPUIso = 0;
    b_elePFChIso03 = 0;
    b_elePFPhoIso03 = 0;
    b_elePFNeuIso03 = 0;
    b_elePFChIso04 = 0;
    b_elePFPhoIso04 = 0;
    b_elePFNeuIso04 = 0;
    b_eleR9 = 0;
    b_eleE3x3 = 0;
    b_eleE5x5 = 0;
    b_eleR9Full5x5 = 0;
    b_eleE3x3Full5x5 = 0;
    b_eleE5x5Full5x5 = 0;
    b_NClusters = 0;
    b_NEcalClusters = 0;
    b_eleSeedEn = 0;
    b_eleSeedEta = 0;
    b_eleSeedPhi = 0;
    b_eleSeedCryEta = 0;
    b_eleSeedCryPhi = 0;
    b_eleSeedCryIeta = 0;
    b_eleSeedCryIphi = 0;
    b_eleBC1E = 0;
    b_eleBC1Eta = 0;
    b_eleBC2E = 0;
    b_eleBC2Eta = 0;
    b_eleIDVeto = 0;
    b_eleIDLoose = 0;
    b_eleIDMedium = 0;
    b_eleIDTight = 0;
    b_elepassConversionVeto = 0;
    b_elePFRelIsoWithEA = 0;
    b_eleEffAreaTimesRho = 0;
    b_nPho = 0;
    b_phoE = 0;
    b_phoEt = 0;
    b_phoEtCorrected = 0;
    b_phoEtCorrected_sys = 0;
    b_phoEta = 0;
    b_phoPhi = 0;
    b_phoSCE = 0;
    b_phoSCRawE = 0;
    b_phoESEn = 0;
    b_phoSCEta = 0;
    b_phoSCPhi = 0;
    b_phoSCEtaWidth = 0;
    b_phoSCPhiWidth = 0;
    b_phoSCBrem = 0;

    b_phoESEffSigmaRR = 0;

    b_phoHasPixelSeed = 0;
    b_phoHasConversionTracks = 0;
    b_phoHadTowerOverEm = 0;
    b_phoHoverE = 0;
    b_phoHoverEValid = 0;
    b_phoSigmaIEtaIEta = 0;
    b_phoR9 = 0;
    b_phoE1x5 = 0;
    b_phoE2x5 = 0;
    b_phoE3x3 = 0;
    b_phoE5x5 = 0;
    b_phoMaxEnergyXtal = 0;
    b_phoSigmaEtaEta = 0;
    b_phoSigmaIEtaIEta_2012 = 0;
    b_phoR9_2012 = 0;
    b_phoE1x5_2012 = 0;
    b_phoE2x5_2012 = 0;
    b_phoE3x3_2012 = 0;
    b_phoE5x5_2012 = 0;
    b_phoMaxEnergyXtal_2012 = 0;
    b_phoSigmaEtaEta_2012 = 0;

    b_pho_isEle = 0;
    b_pho_is2015Noise = 0;

    b_phoHadTowerOverEm1 = 0;
    b_phoHadTowerOverEm2 = 0;
    b_phoHoverE1 = 0;
    b_phoHoverE2 = 0;

    b_phoSigmaIEtaIPhi = 0;
    b_phoSigmaIPhiIPhi = 0;
    b_phoR1x5 = 0;
    b_phoR2x5 = 0;
    b_phoE2nd = 0;
    b_phoETop = 0;
    b_phoEBottom = 0;
    b_phoELeft = 0;
    b_phoERight = 0;
    b_phoE1x3 = 0;
    b_phoE2x2 = 0;
    b_phoE2x5Max = 0;
    b_phoE2x5Top = 0;
    b_phoE2x5Bottom = 0;
    b_phoE2x5Left = 0;
    b_phoE2x5Right = 0;
    //b_phoSMMajor = 0;   // TODO: enable when they become available in future releases
    //b_phoSMMinor = 0;   // TODO: enable when they become available in future releases
    //b_phoSMAlpha = 0;   // TODO: enable when they become available in future releases

    b_phoSigmaIEtaIPhi_2012 = 0;
    b_phoSigmaIPhiIPhi_2012 = 0;
    b_phoR1x5_2012 = 0;
    b_phoR2x5_2012 = 0;
    b_phoE2nd_2012 = 0;
    b_phoETop_2012 = 0;
    b_phoEBottom_2012 = 0;
    b_phoELeft_2012 = 0;
    b_phoERight_2012 = 0;
    b_phoE1x3_2012 = 0;
    b_phoE2x2_2012 = 0;
    b_phoE2x5Max_2012 = 0;
    b_phoE2x5Top_2012 = 0;
    b_phoE2x5Bottom_2012 = 0;
    b_phoE2x5Left_2012 = 0;
    b_phoE2x5Right_2012 = 0;
    //b_phoSMMajor_2012 = 0;   // TODO: enable when they become available in future releases
    //b_phoSMMinor_2012 = 0;   // TODO: enable when they become available in future releases
    //b_phoSMAlpha_2012 = 0;   // TODO: enable when they become available in future releases

    b_phoBC1E = 0;
    b_phoBC1Eta = 0;
    b_phoBC2E = 0;
    b_phoBC2Eta = 0;
    b_pho_ecalClusterIsoR2 = 0;
    b_pho_ecalClusterIsoR3 = 0;
    b_pho_ecalClusterIsoR4 = 0;
    b_pho_ecalClusterIsoR5 = 0;
    b_pho_hcalRechitIsoR1 = 0;
    b_pho_hcalRechitIsoR2 = 0;
    b_pho_hcalRechitIsoR3 = 0;
    b_pho_hcalRechitIsoR4 = 0;
    b_pho_hcalRechitIsoR5 = 0;
    b_pho_trackIsoR1PtCut20 = 0;
    b_pho_trackIsoR2PtCut20 = 0;
    b_pho_trackIsoR3PtCut20 = 0;
    b_pho_trackIsoR4PtCut20 = 0;
    b_pho_trackIsoR5PtCut20 = 0;
    b_pho_sumIsoCorrected = 0;
    b_pho_swissCrx = 0;
    b_pho_seedTime = 0;
    b_pfcIso1 = 0;
    b_pfcIso2 = 0;
    b_pfcIso3 = 0;
    b_pfcIso4 = 0;
    b_pfcIso5 = 0;
    b_pfpIso1 = 0;
    b_pfpIso2 = 0;
    b_pfpIso3 = 0;
    b_pfpIso4 = 0;
    b_pfpIso5 = 0;
    b_pfnIso1 = 0;
    b_pfnIso2 = 0;
    b_pfnIso3 = 0;
    b_pfnIso4 = 0;
    b_pfnIso5 = 0;
    b_pfpIso1subSC = 0;
    b_pfpIso2subSC = 0;
    b_pfpIso3subSC = 0;
    b_pfpIso4subSC = 0;
    b_pfpIso5subSC = 0;
    b_pfcIso1subUE = 0;
    b_pfcIso2subUE = 0;
    b_pfcIso3subUE = 0;
    b_pfcIso4subUE = 0;
    b_pfcIso5subUE = 0;
    b_pfpIso1subUE = 0;
    b_pfpIso2subUE = 0;
    b_pfpIso3subUE = 0;
    b_pfpIso4subUE = 0;
    b_pfpIso5subUE = 0;
    b_pfnIso1subUE = 0;
    b_pfnIso2subUE = 0;
    b_pfnIso3subUE = 0;
    b_pfnIso4subUE = 0;
    b_pfnIso5subUE = 0;
    b_pfpIso1subSCsubUE = 0;
    b_pfpIso2subSCsubUE = 0;
    b_pfpIso3subSCsubUE = 0;
    b_pfpIso4subSCsubUE = 0;
    b_pfpIso5subSCsubUE = 0;
    b_pfcIso1pTgt1p0subUE = 0;
    b_pfcIso2pTgt1p0subUE = 0;
    b_pfcIso3pTgt1p0subUE = 0;
    b_pfcIso4pTgt1p0subUE = 0;
    b_pfcIso5pTgt1p0subUE = 0;
    b_pfcIso1pTgt2p0subUE = 0;
    b_pfcIso2pTgt2p0subUE = 0;
    b_pfcIso3pTgt2p0subUE = 0;
    b_pfcIso4pTgt2p0subUE = 0;
    b_pfcIso5pTgt2p0subUE = 0;
    b_pfcIso1pTgt3p0subUE = 0;
    b_pfcIso2pTgt3p0subUE = 0;
    b_pfcIso3pTgt3p0subUE = 0;
    b_pfcIso4pTgt3p0subUE = 0;
    b_pfcIso5pTgt3p0subUE = 0;
    b_nMu = 0;
    b_muPt = 0;
    b_muEta = 0;
    b_muPhi = 0;
    b_muCharge = 0;
    b_muType = 0;
    b_muIsGood = 0;
    b_muIsGlobal = 0;
    b_muIsTracker = 0;
    b_muIsPF = 0;
    b_muIsSTA = 0;
    b_muD0 = 0;
    b_muDz = 0;
    b_muChi2NDF = 0;
    b_muInnerD0 = 0;
    b_muInnerDz = 0;
    b_muInnerD0Err = 0;
    b_muInnerDzErr = 0;
    b_muInnerPt = 0;
    b_muInnerPtErr = 0;
    b_muInnerEta = 0;
    b_muTrkLayers = 0;
    b_muPixelLayers = 0;
    b_muPixelHits = 0;
    b_muMuonHits = 0;
    b_muTrkQuality = 0;
    b_muStations = 0;
    b_muIsoTrk = 0;
    b_muPFChIso = 0;
    b_muPFPhoIso = 0;
    b_muPFNeuIso = 0;
    b_muPFPUIso = 0;

    // Set branch addresses and branch pointers
    if (t->GetBranch("run")) t->SetBranchAddress("run", &run, &b_run);
    if (t->GetBranch("event")) t->SetBranchAddress("event", &event, &b_event);
    if (t->GetBranch("lumis")) t->SetBranchAddress("lumis", &lumis, &b_lumis);
    if (t->GetBranch("isData")) t->SetBranchAddress("isData", &isData, &b_isData);
    if (t->GetBranch("rho")) t->SetBranchAddress("rho", &rho, &b_rho);
    if (t->GetBranch("angEvtPlane")) t->SetBranchAddress("angEvtPlane", &angEvtPlane, &b_angEvtPlane);
    if (t->GetBranch("indexEvtPlane")) t->SetBranchAddress("indexEvtPlane", &indexEvtPlane, &b_indexEvtPlane);
    if (t->GetBranch("phi_nTot")) t->SetBranchAddress("phi_nTot", &phi_nTot, &b_phi_nTot);
    if (t->GetBranch("phi_minBinN")) t->SetBranchAddress("phi_minBinN", &phi_minBinN, &b_phi_minBinN);
    if (t->GetBranch("phi_fit_chi2")) t->SetBranchAddress("phi_fit_chi2", &phi_fit_chi2, &b_phi_fit_chi2);
    if (t->GetBranch("phi_fit_chi2prob")) t->SetBranchAddress("phi_fit_chi2prob", &phi_fit_chi2prob, &b_phi_fit_chi2prob);
    if (t->GetBranch("phi_fit_v2")) t->SetBranchAddress("phi_fit_v2", &phi_fit_v2, &b_phi_fit_v2);
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
    if (t->GetBranch("nEle")) t->SetBranchAddress("nEle", &nEle, &b_nEle);
    if (t->GetBranch("eleCharge")) t->SetBranchAddress("eleCharge", &eleCharge, &b_eleCharge);
    if (t->GetBranch("eleChargeConsistent")) t->SetBranchAddress("eleChargeConsistent", &eleChargeConsistent, &b_eleChargeConsistent);
    if (t->GetBranch("eleSCPixCharge")) t->SetBranchAddress("eleSCPixCharge", &eleSCPixCharge, &b_eleSCPixCharge);
    if (t->GetBranch("eleCtfCharge")) t->SetBranchAddress("eleCtfCharge", &eleCtfCharge, &b_eleCtfCharge);
    if (t->GetBranch("eleEn")) t->SetBranchAddress("eleEn", &eleEn, &b_eleEn);
    if (t->GetBranch("eleD0")) t->SetBranchAddress("eleD0", &eleD0, &b_eleD0);
    if (t->GetBranch("eleDz")) t->SetBranchAddress("eleDz", &eleDz, &b_eleDz);
    if (t->GetBranch("eleIP3D")) t->SetBranchAddress("eleIP3D", &eleIP3D, &b_eleIP3D);
    if (t->GetBranch("eleD0Err")) t->SetBranchAddress("eleD0Err", &eleD0Err, &b_eleD0Err);
    if (t->GetBranch("eleDzErr")) t->SetBranchAddress("eleDzErr", &eleDzErr, &b_eleDzErr);
    if (t->GetBranch("eleIP3DErr")) t->SetBranchAddress("eleIP3DErr", &eleIP3DErr, &b_eleIP3DErr);
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
    if (t->GetBranch("eledEtaSeedAtVtx")) t->SetBranchAddress("eledEtaSeedAtVtx", &eledEtaSeedAtVtx, &b_eledEtaSeedAtVtx);
    if (t->GetBranch("eleSigmaIEtaIEta")) t->SetBranchAddress("eleSigmaIEtaIEta", &eleSigmaIEtaIEta, &b_eleSigmaIEtaIEta);
    if (t->GetBranch("eleSigmaIEtaIEta_2012")) t->SetBranchAddress("eleSigmaIEtaIEta_2012", &eleSigmaIEtaIEta_2012, &b_eleSigmaIEtaIEta_2012);
    if (t->GetBranch("eleSigmaIPhiIPhi")) t->SetBranchAddress("eleSigmaIPhiIPhi", &eleSigmaIPhiIPhi, &b_eleSigmaIPhiIPhi);
    if (t->GetBranch("eleConvVeto")) t->SetBranchAddress("eleConvVeto", &eleConvVeto, &b_eleConvVeto);
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
    if (t->GetBranch("elePFRelIsoWithEA")) t->SetBranchAddress("elePFRelIsoWithEA", &elePFRelIsoWithEA, &b_elePFRelIsoWithEA);
    if (t->GetBranch("eleEffAreaTimesRho")) t->SetBranchAddress("eleEffAreaTimesRho", &eleEffAreaTimesRho, &b_eleEffAreaTimesRho);
    if (t->GetBranch("nPho")) t->SetBranchAddress("nPho", &nPho, &b_nPho);
    if (t->GetBranch("phoE")) t->SetBranchAddress("phoE", &phoE, &b_phoE);
    if (t->GetBranch("phoEt")) t->SetBranchAddress("phoEt", &phoEt, &b_phoEt);
    if (t->GetBranch("phoEtCorrected")) t->SetBranchAddress("phoEtCorrected", &phoEtCorrected, &b_phoEtCorrected);
    if (t->GetBranch("phoEtCorrected_sys")) t->SetBranchAddress("phoEtCorrected_sys", &phoEtCorrected_sys, &b_phoEtCorrected_sys);
    if (t->GetBranch("phoEta")) t->SetBranchAddress("phoEta", &phoEta, &b_phoEta);
    if (t->GetBranch("phoPhi")) t->SetBranchAddress("phoPhi", &phoPhi, &b_phoPhi);
    if (t->GetBranch("phoSCE")) t->SetBranchAddress("phoSCE", &phoSCE, &b_phoSCE);
    if (t->GetBranch("phoSCRawE")) t->SetBranchAddress("phoSCRawE", &phoSCRawE, &b_phoSCRawE);
    if (t->GetBranch("phoESEn")) t->SetBranchAddress("phoESEn", &phoESEn, &b_phoESEn);
    if (t->GetBranch("phoSCEta")) t->SetBranchAddress("phoSCEta", &phoSCEta, &b_phoSCEta);
    if (t->GetBranch("phoSCPhi")) t->SetBranchAddress("phoSCPhi", &phoSCPhi, &b_phoSCPhi);
    if (t->GetBranch("phoSCEtaWidth")) t->SetBranchAddress("phoSCEtaWidth", &phoSCEtaWidth, &b_phoSCEtaWidth);
    if (t->GetBranch("phoSCPhiWidth")) t->SetBranchAddress("phoSCPhiWidth", &phoSCPhiWidth, &b_phoSCPhiWidth);
    if (t->GetBranch("phoSCBrem")) t->SetBranchAddress("phoSCBrem", &phoSCBrem, &b_phoSCBrem);

    if (t->GetBranch("phoESEffSigmaRR")) t->SetBranchAddress("phoESEffSigmaRR", &phoESEffSigmaRR, &b_phoESEffSigmaRR);

    if (t->GetBranch("phoHasPixelSeed")) t->SetBranchAddress("phoHasPixelSeed", &phoHasPixelSeed, &b_phoHasPixelSeed);
    if (t->GetBranch("phoHasConversionTracks")) t->SetBranchAddress("phoHasConversionTracks", &phoHasConversionTracks, &b_phoHasConversionTracks);
    if (t->GetBranch("phoHadTowerOverEm")) t->SetBranchAddress("phoHadTowerOverEm", &phoHadTowerOverEm, &b_phoHadTowerOverEm);
    if (t->GetBranch("phoHoverE")) t->SetBranchAddress("phoHoverE", &phoHoverE, &b_phoHoverE);
    if (t->GetBranch("phoHoverEValid")) t->SetBranchAddress("phoHoverEValid", &phoHoverEValid, &b_phoHoverEValid);
    if (t->GetBranch("phoSigmaIEtaIEta")) t->SetBranchAddress("phoSigmaIEtaIEta", &phoSigmaIEtaIEta, &b_phoSigmaIEtaIEta);
    if (t->GetBranch("phoR9")) t->SetBranchAddress("phoR9", &phoR9, &b_phoR9);
    if (t->GetBranch("phoE1x5")) t->SetBranchAddress("phoE1x5", &phoE1x5, &b_phoE1x5);
    if (t->GetBranch("phoE2x5")) t->SetBranchAddress("phoE2x5", &phoE2x5, &b_phoE2x5);
    if (t->GetBranch("phoE3x3")) t->SetBranchAddress("phoE3x3", &phoE3x3, &b_phoE3x3);
    if (t->GetBranch("phoE5x5")) t->SetBranchAddress("phoE5x5", &phoE5x5, &b_phoE5x5);
    if (t->GetBranch("phoMaxEnergyXtal")) t->SetBranchAddress("phoMaxEnergyXtal", &phoMaxEnergyXtal, &b_phoMaxEnergyXtal);
    if (t->GetBranch("phoSigmaEtaEta")) t->SetBranchAddress("phoSigmaEtaEta", &phoSigmaEtaEta, &b_phoSigmaEtaEta);
    if (t->GetBranch("phoSigmaIEtaIEta_2012")) t->SetBranchAddress("phoSigmaIEtaIEta_2012", &phoSigmaIEtaIEta_2012, &b_phoSigmaIEtaIEta_2012);
    if (t->GetBranch("phoR9_2012")) t->SetBranchAddress("phoR9_2012", &phoR9_2012, &b_phoR9_2012);
    if (t->GetBranch("phoE1x5_2012")) t->SetBranchAddress("phoE1x5_2012", &phoE1x5_2012, &b_phoE1x5_2012);
    if (t->GetBranch("phoE2x5_2012")) t->SetBranchAddress("phoE2x5_2012", &phoE2x5_2012, &b_phoE2x5_2012);
    if (t->GetBranch("phoE3x3_2012")) t->SetBranchAddress("phoE3x3_2012", &phoE3x3_2012, &b_phoE3x3_2012);
    if (t->GetBranch("phoE5x5_2012")) t->SetBranchAddress("phoE5x5_2012", &phoE5x5_2012, &b_phoE5x5_2012);
    if (t->GetBranch("phoMaxEnergyXtal_2012")) t->SetBranchAddress("phoMaxEnergyXtal_2012", &phoMaxEnergyXtal_2012, &b_phoMaxEnergyXtal_2012);
    if (t->GetBranch("phoSigmaEtaEta_2012")) t->SetBranchAddress("phoSigmaEtaEta_2012", &phoSigmaEtaEta_2012, &b_phoSigmaEtaEta_2012);

    if (t->GetBranch("pho_isEle")) t->SetBranchAddress("pho_isEle", &pho_isEle, &b_pho_isEle);
    if (t->GetBranch("pho_is2015Noise")) t->SetBranchAddress("pho_is2015Noise", &pho_is2015Noise, &b_pho_is2015Noise);

    if (t->GetBranch("phoHadTowerOverEm1")) t->SetBranchAddress("phoHadTowerOverEm1", &phoHadTowerOverEm1, &b_phoHadTowerOverEm1);
    if (t->GetBranch("phoHadTowerOverEm2")) t->SetBranchAddress("phoHadTowerOverEm2", &phoHadTowerOverEm2, &b_phoHadTowerOverEm2);
    if (t->GetBranch("phoHoverE1")) t->SetBranchAddress("phoHoverE1", &phoHoverE1, &b_phoHoverE1);
    if (t->GetBranch("phoHoverE2")) t->SetBranchAddress("phoHoverE2", &phoHoverE2, &b_phoHoverE2);

    if (t->GetBranch("phoSigmaIEtaIPhi")) t->SetBranchAddress("phoSigmaIEtaIPhi", &phoSigmaIEtaIPhi, &b_phoSigmaIEtaIPhi);
    if (t->GetBranch("phoSigmaIPhiIPhi")) t->SetBranchAddress("phoSigmaIPhiIPhi", &phoSigmaIPhiIPhi, &b_phoSigmaIPhiIPhi);
    if (t->GetBranch("phoR1x5")) t->SetBranchAddress("phoR1x5", &phoR1x5, &b_phoR1x5);
    if (t->GetBranch("phoR2x5")) t->SetBranchAddress("phoR2x5", &phoR2x5, &b_phoR2x5);
    if (t->GetBranch("phoE2nd")) t->SetBranchAddress("phoE2nd", &phoE2nd, &b_phoE2nd);
    if (t->GetBranch("phoETop")) t->SetBranchAddress("phoETop", &phoETop, &b_phoETop);
    if (t->GetBranch("phoEBottom")) t->SetBranchAddress("phoEBottom", &phoEBottom, &b_phoEBottom);
    if (t->GetBranch("phoELeft")) t->SetBranchAddress("phoELeft", &phoELeft, &b_phoELeft);
    if (t->GetBranch("phoERight")) t->SetBranchAddress("phoERight", &phoERight, &b_phoERight);
    if (t->GetBranch("phoE1x3")) t->SetBranchAddress("phoE1x3", &phoE1x3, &b_phoE1x3);
    if (t->GetBranch("phoE2x2")) t->SetBranchAddress("phoE2x2", &phoE2x2, &b_phoE2x2);
    if (t->GetBranch("phoE2x5Max")) t->SetBranchAddress("phoE2x5Max", &phoE2x5Max, &b_phoE2x5Max);
    if (t->GetBranch("phoE2x5Top")) t->SetBranchAddress("phoE2x5Top", &phoE2x5Top, &b_phoE2x5Top);
    if (t->GetBranch("phoE2x5Bottom")) t->SetBranchAddress("phoE2x5Bottom", &phoE2x5Bottom, &b_phoE2x5Bottom);
    if (t->GetBranch("phoE2x5Left")) t->SetBranchAddress("phoE2x5Left", &phoE2x5Left, &b_phoE2x5Left);
    if (t->GetBranch("phoE2x5Right")) t->SetBranchAddress("phoE2x5Right", &phoE2x5Right, &b_phoE2x5Right);
    //if (t->GetBranch("phoSMMajor")) t->SetBranchAddress("phoSMMajor", &phoSMMajor, &b_phoSMMajor);
    //if (t->GetBranch("phoSMMinor")) t->SetBranchAddress("phoSMMinor", &phoSMMinor, &b_phoSMMinor);
    //if (t->GetBranch("phoSMAlpha")) t->SetBranchAddress("phoSMAlpha", &phoSMAlpha, &b_phoSMAlpha);

    if (t->GetBranch("phoSigmaIEtaIPhi_2012")) t->SetBranchAddress("phoSigmaIEtaIPhi_2012", &phoSigmaIEtaIPhi_2012, &b_phoSigmaIEtaIPhi_2012);
    if (t->GetBranch("phoSigmaIPhiIPhi_2012")) t->SetBranchAddress("phoSigmaIPhiIPhi_2012", &phoSigmaIPhiIPhi_2012, &b_phoSigmaIPhiIPhi_2012);
    if (t->GetBranch("phoR1x5_2012")) t->SetBranchAddress("phoR1x5_2012", &phoR1x5_2012, &b_phoR1x5_2012);
    if (t->GetBranch("phoR2x5_2012")) t->SetBranchAddress("phoR2x5_2012", &phoR2x5_2012, &b_phoR2x5_2012);
    if (t->GetBranch("phoE2nd_2012")) t->SetBranchAddress("phoE2nd_2012", &phoE2nd_2012, &b_phoE2nd_2012);
    if (t->GetBranch("phoETop_2012")) t->SetBranchAddress("phoETop_2012", &phoETop_2012, &b_phoETop_2012);
    if (t->GetBranch("phoEBottom_2012")) t->SetBranchAddress("phoEBottom_2012", &phoEBottom_2012, &b_phoEBottom_2012);
    if (t->GetBranch("phoELeft_2012")) t->SetBranchAddress("phoELeft_2012", &phoELeft_2012, &b_phoELeft_2012);
    if (t->GetBranch("phoERight_2012")) t->SetBranchAddress("phoERight_2012", &phoERight_2012, &b_phoERight_2012);
    if (t->GetBranch("phoE1x3_2012")) t->SetBranchAddress("phoE1x3_2012", &phoE1x3_2012, &b_phoE1x3_2012);
    if (t->GetBranch("phoE2x2_2012")) t->SetBranchAddress("phoE2x2_2012", &phoE2x2_2012, &b_phoE2x2_2012);
    if (t->GetBranch("phoE2x5Max_2012")) t->SetBranchAddress("phoE2x5Max_2012", &phoE2x5Max_2012, &b_phoE2x5Max_2012);
    if (t->GetBranch("phoE2x5Top_2012")) t->SetBranchAddress("phoE2x5Top_2012", &phoE2x5Top_2012, &b_phoE2x5Top_2012);
    if (t->GetBranch("phoE2x5Bottom_2012")) t->SetBranchAddress("phoE2x5Bottom_2012", &phoE2x5Bottom_2012, &b_phoE2x5Bottom_2012);
    if (t->GetBranch("phoE2x5Left_2012")) t->SetBranchAddress("phoE2x5Left_2012", &phoE2x5Left_2012, &b_phoE2x5Left_2012);
    if (t->GetBranch("phoE2x5Right_2012")) t->SetBranchAddress("phoE2x5Right_2012", &phoE2x5Right_2012, &b_phoE2x5Right_2012);
    //if (t->GetBranch("phoSMMajor_2012")) t->SetBranchAddress("phoSMMajor_2012", &phoSMMajor_2012, &b_phoSMMajor_2012);
    //if (t->GetBranch("phoSMMinor_2012")) t->SetBranchAddress("phoSMMinor_2012", &phoSMMinor_2012, &b_phoSMMinor_2012);
    //if (t->GetBranch("phoSMAlpha_2012")) t->SetBranchAddress("phoSMAlpha_2012", &phoSMAlpha_2012, &b_phoSMAlpha_2012);

    if (t->GetBranch("phoBC1E")) t->SetBranchAddress("phoBC1E", &phoBC1E, &b_phoBC1E);
    if (t->GetBranch("phoBC1Eta")) t->SetBranchAddress("phoBC1Eta", &phoBC1Eta, &b_phoBC1Eta);
    if (t->GetBranch("phoBC2E")) t->SetBranchAddress("phoBC2E", &phoBC2E, &b_phoBC2E);
    if (t->GetBranch("phoBC2Eta")) t->SetBranchAddress("phoBC2Eta", &phoBC2Eta, &b_phoBC2Eta);
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
    if (t->GetBranch("pfpIso1subSC")) t->SetBranchAddress("pfpIso1subSC", &pfpIso1subSC, &b_pfpIso1subSC);
    if (t->GetBranch("pfpIso2subSC")) t->SetBranchAddress("pfpIso2subSC", &pfpIso2subSC, &b_pfpIso2subSC);
    if (t->GetBranch("pfpIso3subSC")) t->SetBranchAddress("pfpIso3subSC", &pfpIso3subSC, &b_pfpIso3subSC);
    if (t->GetBranch("pfpIso4subSC")) t->SetBranchAddress("pfpIso4subSC", &pfpIso4subSC, &b_pfpIso4subSC);
    if (t->GetBranch("pfpIso5subSC")) t->SetBranchAddress("pfpIso5subSC", &pfpIso5subSC, &b_pfpIso5subSC);
    if (t->GetBranch("pfcIso1subUE")) t->SetBranchAddress("pfcIso1subUE", &pfcIso1subUE, &b_pfcIso1subUE);
    if (t->GetBranch("pfcIso2subUE")) t->SetBranchAddress("pfcIso2subUE", &pfcIso2subUE, &b_pfcIso2subUE);
    if (t->GetBranch("pfcIso3subUE")) t->SetBranchAddress("pfcIso3subUE", &pfcIso3subUE, &b_pfcIso3subUE);
    if (t->GetBranch("pfcIso4subUE")) t->SetBranchAddress("pfcIso4subUE", &pfcIso4subUE, &b_pfcIso4subUE);
    if (t->GetBranch("pfcIso5subUE")) t->SetBranchAddress("pfcIso5subUE", &pfcIso5subUE, &b_pfcIso5subUE);
    if (t->GetBranch("pfpIso1subUE")) t->SetBranchAddress("pfpIso1subUE", &pfpIso1subUE, &b_pfpIso1subUE);
    if (t->GetBranch("pfpIso2subUE")) t->SetBranchAddress("pfpIso2subUE", &pfpIso2subUE, &b_pfpIso2subUE);
    if (t->GetBranch("pfpIso3subUE")) t->SetBranchAddress("pfpIso3subUE", &pfpIso3subUE, &b_pfpIso3subUE);
    if (t->GetBranch("pfpIso4subUE")) t->SetBranchAddress("pfpIso4subUE", &pfpIso4subUE, &b_pfpIso4subUE);
    if (t->GetBranch("pfpIso5subUE")) t->SetBranchAddress("pfpIso5subUE", &pfpIso5subUE, &b_pfpIso5subUE);
    if (t->GetBranch("pfnIso1subUE")) t->SetBranchAddress("pfnIso1subUE", &pfnIso1subUE, &b_pfnIso1subUE);
    if (t->GetBranch("pfnIso2subUE")) t->SetBranchAddress("pfnIso2subUE", &pfnIso2subUE, &b_pfnIso2subUE);
    if (t->GetBranch("pfnIso3subUE")) t->SetBranchAddress("pfnIso3subUE", &pfnIso3subUE, &b_pfnIso3subUE);
    if (t->GetBranch("pfnIso4subUE")) t->SetBranchAddress("pfnIso4subUE", &pfnIso4subUE, &b_pfnIso4subUE);
    if (t->GetBranch("pfnIso5subUE")) t->SetBranchAddress("pfnIso5subUE", &pfnIso5subUE, &b_pfnIso5subUE);
    if (t->GetBranch("pfpIso1subSCsubUE")) t->SetBranchAddress("pfpIso1subSCsubUE", &pfpIso1subSCsubUE, &b_pfpIso1subSCsubUE);
    if (t->GetBranch("pfpIso2subSCsubUE")) t->SetBranchAddress("pfpIso2subSCsubUE", &pfpIso2subSCsubUE, &b_pfpIso2subSCsubUE);
    if (t->GetBranch("pfpIso3subSCsubUE")) t->SetBranchAddress("pfpIso3subSCsubUE", &pfpIso3subSCsubUE, &b_pfpIso3subSCsubUE);
    if (t->GetBranch("pfpIso4subSCsubUE")) t->SetBranchAddress("pfpIso4subSCsubUE", &pfpIso4subSCsubUE, &b_pfpIso4subSCsubUE);
    if (t->GetBranch("pfpIso5subSCsubUE")) t->SetBranchAddress("pfpIso5subSCsubUE", &pfpIso5subSCsubUE, &b_pfpIso5subSCsubUE);
    if (t->GetBranch("pfcIso1pTgt1p0subUE")) t->SetBranchAddress("pfcIso1pTgt1p0subUE", &pfcIso1pTgt1p0subUE, &b_pfcIso1pTgt1p0subUE);
    if (t->GetBranch("pfcIso2pTgt1p0subUE")) t->SetBranchAddress("pfcIso2pTgt1p0subUE", &pfcIso2pTgt1p0subUE, &b_pfcIso2pTgt1p0subUE);
    if (t->GetBranch("pfcIso3pTgt1p0subUE")) t->SetBranchAddress("pfcIso3pTgt1p0subUE", &pfcIso3pTgt1p0subUE, &b_pfcIso3pTgt1p0subUE);
    if (t->GetBranch("pfcIso4pTgt1p0subUE")) t->SetBranchAddress("pfcIso4pTgt1p0subUE", &pfcIso4pTgt1p0subUE, &b_pfcIso4pTgt1p0subUE);
    if (t->GetBranch("pfcIso5pTgt1p0subUE")) t->SetBranchAddress("pfcIso5pTgt1p0subUE", &pfcIso5pTgt1p0subUE, &b_pfcIso5pTgt1p0subUE);
    if (t->GetBranch("pfcIso1pTgt2p0subUE")) t->SetBranchAddress("pfcIso1pTgt2p0subUE", &pfcIso1pTgt2p0subUE, &b_pfcIso1pTgt2p0subUE);
    if (t->GetBranch("pfcIso2pTgt2p0subUE")) t->SetBranchAddress("pfcIso2pTgt2p0subUE", &pfcIso2pTgt2p0subUE, &b_pfcIso2pTgt2p0subUE);
    if (t->GetBranch("pfcIso3pTgt2p0subUE")) t->SetBranchAddress("pfcIso3pTgt2p0subUE", &pfcIso3pTgt2p0subUE, &b_pfcIso3pTgt2p0subUE);
    if (t->GetBranch("pfcIso4pTgt2p0subUE")) t->SetBranchAddress("pfcIso4pTgt2p0subUE", &pfcIso4pTgt2p0subUE, &b_pfcIso4pTgt2p0subUE);
    if (t->GetBranch("pfcIso5pTgt2p0subUE")) t->SetBranchAddress("pfcIso5pTgt2p0subUE", &pfcIso5pTgt2p0subUE, &b_pfcIso5pTgt2p0subUE);
    if (t->GetBranch("pfcIso1pTgt3p0subUE")) t->SetBranchAddress("pfcIso1pTgt3p0subUE", &pfcIso1pTgt3p0subUE, &b_pfcIso1pTgt3p0subUE);
    if (t->GetBranch("pfcIso2pTgt3p0subUE")) t->SetBranchAddress("pfcIso2pTgt3p0subUE", &pfcIso2pTgt3p0subUE, &b_pfcIso2pTgt3p0subUE);
    if (t->GetBranch("pfcIso3pTgt3p0subUE")) t->SetBranchAddress("pfcIso3pTgt3p0subUE", &pfcIso3pTgt3p0subUE, &b_pfcIso3pTgt3p0subUE);
    if (t->GetBranch("pfcIso4pTgt3p0subUE")) t->SetBranchAddress("pfcIso4pTgt3p0subUE", &pfcIso4pTgt3p0subUE, &b_pfcIso4pTgt3p0subUE);
    if (t->GetBranch("pfcIso5pTgt3p0subUE")) t->SetBranchAddress("pfcIso5pTgt3p0subUE", &pfcIso5pTgt3p0subUE, &b_pfcIso5pTgt3p0subUE);
    if (t->GetBranch("nMu")) t->SetBranchAddress("nMu", &nMu, &b_nMu);
    if (t->GetBranch("muPt")) t->SetBranchAddress("muPt", &muPt, &b_muPt);
    if (t->GetBranch("muEta")) t->SetBranchAddress("muEta", &muEta, &b_muEta);
    if (t->GetBranch("muPhi")) t->SetBranchAddress("muPhi", &muPhi, &b_muPhi);
    if (t->GetBranch("muCharge")) t->SetBranchAddress("muCharge", &muCharge, &b_muCharge);
    if (t->GetBranch("muType")) t->SetBranchAddress("muType", &muType, &b_muType);
    if (t->GetBranch("muIsGood")) t->SetBranchAddress("muIsGood", &muIsGood, &b_muIsGood);
    if (t->GetBranch("muIsGlobal")) t->SetBranchAddress("muIsGlobal", &muIsGlobal, &b_muIsGlobal);
    if (t->GetBranch("muIsTracker")) t->SetBranchAddress("muIsTracker", &muIsTracker, &b_muIsTracker);
    if (t->GetBranch("muIsPF")) t->SetBranchAddress("muIsPF", &muIsPF, &b_muIsPF);
    if (t->GetBranch("muIsSTA")) t->SetBranchAddress("muIsSTA", &muIsSTA, &b_muIsSTA);
    if (t->GetBranch("muD0")) t->SetBranchAddress("muD0", &muD0, &b_muD0);
    if (t->GetBranch("muDz")) t->SetBranchAddress("muDz", &muDz, &b_muDz);
    if (t->GetBranch("muChi2NDF")) t->SetBranchAddress("muChi2NDF", &muChi2NDF, &b_muChi2NDF);
    if (t->GetBranch("muInnerD0")) t->SetBranchAddress("muInnerD0", &muInnerD0, &b_muInnerD0);
    if (t->GetBranch("muInnerDz")) t->SetBranchAddress("muInnerDz", &muInnerDz, &b_muInnerDz);
    if (t->GetBranch("muInnerD0Err")) t->SetBranchAddress("muInnerD0Err", &muInnerD0Err, &b_muInnerD0Err);
    if (t->GetBranch("muInnerDzErr")) t->SetBranchAddress("muInnerDzErr", &muInnerDzErr, &b_muInnerDzErr);
    if (t->GetBranch("muInnerPt")) t->SetBranchAddress("muInnerPt", &muInnerPt, &b_muInnerPt);
    if (t->GetBranch("muInnerPtErr")) t->SetBranchAddress("muInnerPtErr", &muInnerPtErr, &b_muInnerPtErr);
    if (t->GetBranch("muInnerEta")) t->SetBranchAddress("muInnerEta", &muInnerEta, &b_muInnerEta);
    if (t->GetBranch("muTrkLayers")) t->SetBranchAddress("muTrkLayers", &muTrkLayers, &b_muTrkLayers);
    if (t->GetBranch("muPixelLayers")) t->SetBranchAddress("muPixelLayers", &muPixelLayers, &b_muPixelLayers);
    if (t->GetBranch("muPixelHits")) t->SetBranchAddress("muPixelHits", &muPixelHits, &b_muPixelHits);
    if (t->GetBranch("muMuonHits")) t->SetBranchAddress("muMuonHits", &muMuonHits, &b_muMuonHits);
    if (t->GetBranch("muTrkQuality")) t->SetBranchAddress("muTrkQuality", &muTrkQuality, &b_muTrkQuality);
    if (t->GetBranch("muStations")) t->SetBranchAddress("muStations", &muStations, &b_muStations);
    if (t->GetBranch("muIsoTrk")) t->SetBranchAddress("muIsoTrk", &muIsoTrk, &b_muIsoTrk);
    if (t->GetBranch("muPFChIso")) t->SetBranchAddress("muPFChIso", &muPFChIso, &b_muPFChIso);
    if (t->GetBranch("muPFPhoIso")) t->SetBranchAddress("muPFPhoIso", &muPFPhoIso, &b_muPFPhoIso);
    if (t->GetBranch("muPFNeuIso")) t->SetBranchAddress("muPFNeuIso", &muPFNeuIso, &b_muPFNeuIso);
    if (t->GetBranch("muPFPUIso")) t->SetBranchAddress("muPFPUIso", &muPFPUIso, &b_muPFPUIso);
}

/*
 * check if the photon is ECAL noise in 2015 PbPb data
 */
bool ggHiNtuplizer::is2015EcalNoise(int iPho)
{
    bool failedNoiseCut =  ((*phoE3x3)[iPho]/(*phoE5x5)[iPho] > 2./3.-0.03 &&
            (*phoE3x3)[iPho]/(*phoE5x5)[iPho] < 2./3.+0.03) &&
           ((*phoE1x5)[iPho]/(*phoE5x5)[iPho] > 1./3.-0.03 &&
            (*phoE1x5)[iPho]/(*phoE5x5)[iPho] < 1./3.+0.03) &&
           ((*phoE2x5)[iPho]/(*phoE5x5)[iPho] > 2./3.-0.03 &&
            (*phoE2x5)[iPho]/(*phoE5x5)[iPho] < 2./3.+0.03);
    return failedNoiseCut;
}

bool ggHiNtuplizer::passedPhoSpikeRejection(int i)
{
    return ( (*phoSigmaIEtaIEta_2012)[i] > 0.002 && (*pho_swissCrx)[i] < 0.9 && std::fabs((*pho_seedTime)[i]) < 3 );
}

bool ggHiNtuplizer::passedHI18HEMfailurePho(int i)
{
    return !((*phoSCEta)[i] < -1.39 && (*phoSCPhi)[i] < -0.9 && (*phoSCPhi)[i] > -1.6);
}

bool ggHiNtuplizer::passedHI18HEMfailureEle(int i)
{
    return !((*eleSCEta)[i] < -1.39 && (*eleSCPhi)[i] < -0.9 && (*eleSCPhi)[i] > -1.6);
}

bool ggHiNtuplizer::passedHI18HEMfailureMu(int i)
{
    return !((*muEta)[i] < -1.39 && (*muPhi)[i] < -0.9 && (*muPhi)[i] > -1.6);
}

bool ggHiNtuplizer::passedHI18HEMfailureGen(int i)
{
    return !((*mcEta)[i] < -1.39 && (*mcPhi)[i] < -0.9 && (*mcPhi)[i] > -1.6);
}

bool ggHiNtuplizer::passedEleSelection(int i, int collType, int hiBin, double eleRho, int WPindex)
{
    /*
     * options for WPindex
     *
     * 0 : default
     * 1 : veto
     * 2 : loose
     * 3 : medium
     * 4 : tight
     * 5 : very tight
     */

    if (collisionIsHI2018((COLL::TYPE)collType)) {
        // default WP for 2018 pbpb is veto.
        if (WPindex == 0) WPindex = 1;

        //if (!((*eleConvVeto)[i] > 0) ) return false;

        if (! (getElePFIsoSubRho(i, eleRho) < 0) )  return false;

        //if ( !((*eleMissHits)[i] == 0) )  return false;

        if (WPindex == 1) {
            // preliminary electron ID (July 2019) : veto WP
            if (hiBin >= 0 && hiBin < 60) {
                // selection on RECO electron based on ECAL regions
                if (std::fabs((*eleSCEta)[i]) < 1.4442)
                {
                    if (!((*eleSigmaIEtaIEta_2012)[i] < 0.0147)) return false;
                    if (!(std::fabs((*eledEtaSeedAtVtx)[i]) < 0.0041)) return false;
                    if (!(std::fabs((*eledPhiAtVtx)[i]) < 0.0853)) return false;
                    if (!((*eleHoverEBc)[i] < 0.2733)) return false;
                    if (!(std::fabs((*eleEoverPInv)[i]) < 0.0367)) return false;
                    //if (!(std::fabs((*eleD0)[i]) < 0.01)) return false;
                    //if (!(std::fabs((*eleDz)[i]) < 0.04)) return false;
                    if (!((*eleMissHits)[i] <= 3)) return false;
                    if (!((*eleIP3D)[i] < 0.03)) return false;
                }
                else if (std::fabs((*eleSCEta)[i]) > 1.566 && std::fabs((*eleSCEta)[i]) < 2.5)
                {
                    if (!((*eleSigmaIEtaIEta_2012)[i] < 0.048)) return false;
                    if (!(std::fabs((*eledEtaSeedAtVtx)[i]) < 0.0097)) return false;
                    if (!(std::fabs((*eledPhiAtVtx)[i]) < 0.2348)) return false;
                    if (!((*eleHoverEBc)[i] < 0.1898)) return false;
                    if (!(std::fabs((*eleEoverPInv)[i]) < 0.0300)) return false;
                    //if (!(std::fabs((*eleD0)[i]) < 0.02)) return false;
                    //if (!(std::fabs((*eleDz)[i]) < 0.04)) return false;
                    if (!((*eleMissHits)[i] <= 3)) return false;
                    if (!((*eleIP3D)[i] < 0.03)) return false;
                }
            }
            else if (hiBin >= 60 && hiBin < 200) {
                if (std::fabs((*eleSCEta)[i]) < 1.4442)
                {
                    if (!((*eleSigmaIEtaIEta_2012)[i] < 0.0113)) return false;
                    if (!(std::fabs((*eledEtaSeedAtVtx)[i]) < 0.0037)) return false;
                    if (!(std::fabs((*eledPhiAtVtx)[i]) < 0.1280)) return false;
                    if (!((*eleHoverEBc)[i] < 0.1814)) return false;
                    if (!(std::fabs((*eleEoverPInv)[i]) < 0.1065)) return false;
                    //if (!(std::fabs((*eleD0)[i]) < 0.01)) return false;
                    //if (!(std::fabs((*eleDz)[i]) < 0.04)) return false;
                    if (!((*eleMissHits)[i] <= 3)) return false;
                    if (!((*eleIP3D)[i] < 0.03)) return false;
                }
                else if (std::fabs((*eleSCEta)[i]) > 1.566 && std::fabs((*eleSCEta)[i]) < 2.5)
                {
                    if (!((*eleSigmaIEtaIEta_2012)[i] < 0.0376)) return false;
                    if (!(std::fabs((*eledEtaSeedAtVtx)[i]) < 0.0074)) return false;
                    if (!(std::fabs((*eledPhiAtVtx)[i]) < 0.2085)) return false;
                    if (!((*eleHoverEBc)[i] < 0.1138)) return false;
                    if (!(std::fabs((*eleEoverPInv)[i]) < 0.0237)) return false;
                    //if (!(std::fabs((*eleD0)[i]) < 0.02)) return false;
                    //if (!(std::fabs((*eleDz)[i]) < 0.04)) return false;
                    if (!((*eleMissHits)[i] <= 3)) return false;
                    if (!((*eleIP3D)[i] < 0.03)) return false;
                }
            }
        }
        else if (WPindex == 2) {
            // preliminary electron ID (July 2019) : loose WP
            if (hiBin >= 0 && hiBin < 60) {
                // selection on RECO electron based on ECAL regions
                if (std::fabs((*eleSCEta)[i]) < 1.4442)
                {
                    if (!((*eleSigmaIEtaIEta_2012)[i] < 0.0135)) return false;
                    if (!(std::fabs((*eledEtaSeedAtVtx)[i]) < 0.0038)) return false;
                    if (!(std::fabs((*eledPhiAtVtx)[i]) < 0.0376)) return false;
                    if (!((*eleHoverEBc)[i] < 0.1616)) return false;
                    if (!(std::fabs((*eleEoverPInv)[i]) < 0.0177)) return false;
                    //if (!(std::fabs((*eleD0)[i]) < 0.01)) return false;
                    //if (!(std::fabs((*eleDz)[i]) < 0.04)) return false;
                    if (!((*eleMissHits)[i] <= 1)) return false;
                    if (!((*eleIP3D)[i] < 0.03)) return false;
                }
                else if (std::fabs((*eleSCEta)[i]) > 1.566 && std::fabs((*eleSCEta)[i]) < 2.5)
                {
                    if (!((*eleSigmaIEtaIEta_2012)[i] < 0.0466)) return false;
                    if (!(std::fabs((*eledEtaSeedAtVtx)[i]) < 0.0063)) return false;
                    if (!(std::fabs((*eledPhiAtVtx)[i]) < 0.1186)) return false;
                    if (!((*eleHoverEBc)[i] < 0.1317)) return false;
                    if (!(std::fabs((*eleEoverPInv)[i]) < 0.0201)) return false;
                    //if (!(std::fabs((*eleD0)[i]) < 0.02)) return false;
                    //if (!(std::fabs((*eleDz)[i]) < 0.04)) return false;
                    if (!((*eleMissHits)[i] <= 1)) return false;
                    if (!((*eleIP3D)[i] < 0.03)) return false;
                }
            }
            else if (hiBin >= 60 && hiBin < 200) {
                if (std::fabs((*eleSCEta)[i]) < 1.4442)
                {
                    if (!((*eleSigmaIEtaIEta_2012)[i] < 0.0107)) return false;
                    if (!(std::fabs((*eledEtaSeedAtVtx)[i]) < 0.0035)) return false;
                    if (!(std::fabs((*eledPhiAtVtx)[i]) < 0.0327)) return false;
                    if (!((*eleHoverEBc)[i] < 0.1268)) return false;
                    if (!(std::fabs((*eleEoverPInv)[i]) < 0.0774)) return false;
                    //if (!(std::fabs((*eleD0)[i]) < 0.01)) return false;
                    //if (!(std::fabs((*eleDz)[i]) < 0.04)) return false;
                    if (!((*eleMissHits)[i] <= 1)) return false;
                    if (!((*eleIP3D)[i] < 0.03)) return false;
                }
                else if (std::fabs((*eleSCEta)[i]) > 1.566 && std::fabs((*eleSCEta)[i]) < 2.5)
                {
                    if (!((*eleSigmaIEtaIEta_2012)[i] < 0.0339)) return false;
                    if (!(std::fabs((*eledEtaSeedAtVtx)[i]) < 0.0067)) return false;
                    if (!(std::fabs((*eledPhiAtVtx)[i]) < 0.0838)) return false;
                    if (!((*eleHoverEBc)[i] < 0.0977)) return false;
                    if (!(std::fabs((*eleEoverPInv)[i]) < 0.0193)) return false;
                    //if (!(std::fabs((*eleD0)[i]) < 0.02)) return false;
                    //if (!(std::fabs((*eleDz)[i]) < 0.04)) return false;
                    if (!((*eleMissHits)[i] <= 1)) return false;
                    if (!((*eleIP3D)[i] < 0.03)) return false;
                }
            }
        }
        else if (WPindex == 3) {
            // preliminary electron ID (July 2019) : medium WP
            if (hiBin >= 0 && hiBin < 60) {
                // selection on RECO electron based on ECAL regions
                if (std::fabs((*eleSCEta)[i]) < 1.4442)
                {
                    if (!((*eleSigmaIEtaIEta_2012)[i] < 0.0116)) return false;
                    if (!(std::fabs((*eledEtaSeedAtVtx)[i]) < 0.0037)) return false;
                    if (!(std::fabs((*eledPhiAtVtx)[i]) < 0.0224)) return false;
                    if (!((*eleHoverEBc)[i] < 0.1589)) return false;
                    if (!(std::fabs((*eleEoverPInv)[i]) < 0.0173)) return false;
                    //if (!(std::fabs((*eleD0)[i]) < 0.01)) return false;
                    //if (!(std::fabs((*eleDz)[i]) < 0.04)) return false;
                    if (!((*eleMissHits)[i] <= 1)) return false;
                    if (!((*eleIP3D)[i] < 0.03)) return false;
                }
                else if (std::fabs((*eleSCEta)[i]) > 1.566 && std::fabs((*eleSCEta)[i]) < 2.5)
                {
                    if (!((*eleSigmaIEtaIEta_2012)[i] < 0.0418)) return false;
                    if (!(std::fabs((*eledEtaSeedAtVtx)[i]) < 0.0062)) return false;
                    if (!(std::fabs((*eledPhiAtVtx)[i]) < 0.0373)) return false;
                    if (!((*eleHoverEBc)[i] < 0.1092)) return false;
                    if (!(std::fabs((*eleEoverPInv)[i]) < 0.0133)) return false;
                    //if (!(std::fabs((*eleD0)[i]) < 0.02)) return false;
                    //if (!(std::fabs((*eleDz)[i]) < 0.04)) return false;
                    if (!((*eleMissHits)[i] <= 1)) return false;
                    if (!((*eleIP3D)[i] < 0.03)) return false;
                }
            }
            else if (hiBin >= 60 && hiBin < 200) {
                if (std::fabs((*eleSCEta)[i]) < 1.4442)
                {
                    if (!((*eleSigmaIEtaIEta_2012)[i] < 0.0101)) return false;
                    if (!(std::fabs((*eledEtaSeedAtVtx)[i]) < 0.0033)) return false;
                    if (!(std::fabs((*eledPhiAtVtx)[i]) < 0.0210)) return false;
                    if (!((*eleHoverEBc)[i] < 0.0311)) return false;
                    if (!(std::fabs((*eleEoverPInv)[i]) < 0.0701)) return false;
                    //if (!(std::fabs((*eleD0)[i]) < 0.01)) return false;
                    //if (!(std::fabs((*eleDz)[i]) < 0.04)) return false;
                    if (!((*eleMissHits)[i] <= 1)) return false;
                    if (!((*eleIP3D)[i] < 0.03)) return false;
                }
                else if (std::fabs((*eleSCEta)[i]) > 1.566 && std::fabs((*eleSCEta)[i]) < 2.5)
                {
                    if (!((*eleSigmaIEtaIEta_2012)[i] < 0.0316)) return false;
                    if (!(std::fabs((*eledEtaSeedAtVtx)[i]) < 0.0051)) return false;
                    if (!(std::fabs((*eledPhiAtVtx)[i]) < 0.0384)) return false;
                    if (!((*eleHoverEBc)[i] < 0.0810)) return false;
                    if (!(std::fabs((*eleEoverPInv)[i]) < 0.0192)) return false;
                    //if (!(std::fabs((*eleD0)[i]) < 0.02)) return false;
                    //if (!(std::fabs((*eleDz)[i]) < 0.04)) return false;
                    if (!((*eleMissHits)[i] <= 1)) return false;
                    if (!((*eleIP3D)[i] < 0.03)) return false;
                }
            }
        }
        else if (WPindex == 4) {
            // preliminary electron ID (July 2019) : tight WP
            if (hiBin >= 0 && hiBin < 60) {
                // selection on RECO electron based on ECAL regions
                if (std::fabs((*eleSCEta)[i]) < 1.4442)
                {
                    if (!((*eleSigmaIEtaIEta_2012)[i] < 0.0101)) return false;
                    if (!(std::fabs((*eledEtaSeedAtVtx)[i]) < 0.0029)) return false;
                    if (!(std::fabs((*eledPhiAtVtx)[i]) < 0.0206)) return false;
                    if (!((*eleHoverEBc)[i] < 0.1459)) return false;
                    if (!(std::fabs((*eleEoverPInv)[i]) < 0.0105)) return false;
                    //if (!(std::fabs((*eleD0)[i]) < 0.01)) return false;
                    //if (!(std::fabs((*eleDz)[i]) < 0.04)) return false;
                    if (!((*eleMissHits)[i] <= 1)) return false;
                    if (!((*eleIP3D)[i] < 0.03)) return false;
                }
                else if (std::fabs((*eleSCEta)[i]) > 1.566 && std::fabs((*eleSCEta)[i]) < 2.5)
                {
                    if (!((*eleSigmaIEtaIEta_2012)[i] < 0.0358)) return false;
                    if (!(std::fabs((*eledEtaSeedAtVtx)[i]) < 0.0051)) return false;
                    if (!(std::fabs((*eledPhiAtVtx)[i]) < 0.0266)) return false;
                    if (!((*eleHoverEBc)[i] < 0.0925)) return false;
                    if (!(std::fabs((*eleEoverPInv)[i]) < 0.0065)) return false;
                    //if (!(std::fabs((*eleD0)[i]) < 0.02)) return false;
                    //if (!(std::fabs((*eleDz)[i]) < 0.04)) return false;
                    if (!((*eleMissHits)[i] <= 1)) return false;
                    if (!((*eleIP3D)[i] < 0.03)) return false;
                }
            }
            else if (hiBin >= 60 && hiBin < 200) {
                if (std::fabs((*eleSCEta)[i]) < 1.4442)
                {
                    if (!((*eleSigmaIEtaIEta_2012)[i] < 0.0099)) return false;
                    if (!(std::fabs((*eledEtaSeedAtVtx)[i]) < 0.0026)) return false;
                    if (!(std::fabs((*eledPhiAtVtx)[i]) < 0.0170)) return false;
                    if (!((*eleHoverEBc)[i] < 0.0067)) return false;
                    if (!(std::fabs((*eleEoverPInv)[i]) < 0.0077)) return false;
                    //if (!(std::fabs((*eleD0)[i]) < 0.01)) return false;
                    //if (!(std::fabs((*eleDz)[i]) < 0.04)) return false;
                    if (!((*eleMissHits)[i] <= 1)) return false;
                    if (!((*eleIP3D)[i] < 0.03)) return false;
                }
                else if (std::fabs((*eleSCEta)[i]) > 1.566 && std::fabs((*eleSCEta)[i]) < 2.5)
                {
                    if (!((*eleSigmaIEtaIEta_2012)[i] < 0.0288)) return false;
                    if (!(std::fabs((*eledEtaSeedAtVtx)[i]) < 0.0044)) return false;
                    if (!(std::fabs((*eledPhiAtVtx)[i]) < 0.0266)) return false;
                    if (!((*eleHoverEBc)[i] < 0.0655)) return false;
                    if (!(std::fabs((*eleEoverPInv)[i]) < 0.0123)) return false;
                    //if (!(std::fabs((*eleD0)[i]) < 0.02)) return false;
                    //if (!(std::fabs((*eleDz)[i]) < 0.04)) return false;
                    if (!((*eleMissHits)[i] <= 1)) return false;
                    if (!((*eleIP3D)[i] < 0.03)) return false;
                }
            }
        }
        else {
            return false;
        }
    }
    else if (collisionIsHI((COLL::TYPE)collType)) {     // pbpb 2015

        // default WP for 2015 pbpb is veto.
        if (WPindex == 0) WPindex = 1;

        // selection on RECO electron based on ECAL regions
        if (WPindex == 1) {
            if (std::fabs((*eleSCEta)[i]) < 1.4442)
            {
                if (!((*eleSigmaIEtaIEta_2012)[i] < 0.01107)) return false;
                if (!(std::fabs((*eledEtaAtVtx)[i]) < 0.01576)) return false;
                if (!(std::fabs((*eledPhiAtVtx)[i]) < 0.15724)) return false;
                if (!((*eleHoverE)[i] < 0.08849)) return false;
                if (!(std::fabs((*eleEoverPInv)[i]) < 0.28051)) return false;
                if (!(std::fabs((*eleD0)[i]) < 0.05216)) return false;
                if (!(std::fabs((*eleDz)[i]) < 0.12997)) return false;
                if (!((*eleMissHits)[i] <= 1)) return false;
            }
            else if (std::fabs((*eleSCEta)[i]) > 1.566 && std::fabs((*eleSCEta)[i]) < 2.5)
            {
                if (!((*eleSigmaIEtaIEta_2012)[i] < 0.03488)) return false;
                if (!(std::fabs((*eledEtaAtVtx)[i]) < 0.01707)) return false;
                if (!(std::fabs((*eledPhiAtVtx)[i]) < 0.35537)) return false;
                if (!((*eleHoverE)[i] < 0.12275)) return false;
                if (!(std::fabs((*eleEoverPInv)[i]) < 0.18672)) return false;
                if (!(std::fabs((*eleD0)[i]) < 0.19092)) return false;
                if (!(std::fabs((*eleDz)[i]) < 0.26407)) return false;
                if (!((*eleMissHits)[i] <= 1)) return false;
            }
        }
        else if (WPindex == 3) {

            if (std::fabs((*eleSCEta)[i]) < 1.4442)
            {
                if (!((*eleSigmaIEtaIEta_2012)[i] < 0.01058)) return false;
                if (!(std::fabs((*eledEtaAtVtx)[i]) < 0.01494)) return false;
                if (!(std::fabs((*eledPhiAtVtx)[i]) < 0.03396)) return false;
                if (!((*eleHoverE)[i] < 0.03329)) return false;
                if (!(std::fabs((*eleEoverPInv)[i]) < 0.01482)) return false;
                if (!(std::fabs((*eleD0)[i]) < 0.01602)) return false;
                if (!(std::fabs((*eleDz)[i]) < 0.06939)) return false;
                if (!((*eleMissHits)[i] <= 1)) return false;
            }
            else if (std::fabs((*eleSCEta)[i]) > 1.566 && std::fabs((*eleSCEta)[i]) < 2.5)
            {
                if (!((*eleSigmaIEtaIEta_2012)[i] < 0.02995)) return false;
                if (!(std::fabs((*eledEtaAtVtx)[i]) < 0.00860)) return false;
                if (!(std::fabs((*eledPhiAtVtx)[i]) < 0.12246)) return false;
                if (!((*eleHoverE)[i] < 0.09265)) return false;
                if (!(std::fabs((*eleEoverPInv)[i]) < 0.12623)) return false;
                if (!(std::fabs((*eleD0)[i]) < 0.08725)) return false;
                if (!(std::fabs((*eleDz)[i]) < 0.20320)) return false;
                if (!((*eleMissHits)[i] <= 1)) return false;
            }
        }
        else {
            return false;
        }
    }
    else if (collisionIsPP2017((COLL::TYPE)collType)) {

        // default WP for 2017 pp is veto.
        if (WPindex == 0) WPindex = 1;

        //if (!((*eleConvVeto)[i] > 0) ) return false;

        if (WPindex == 1) {
            if (std::fabs((*eleSCEta)[i]) < 1.4442)
            {
                if (!((*eleSigmaIEtaIEta_2012)[i] < 0.01029)) return false;
                if (!(std::fabs((*eledEtaSeedAtVtx)[i]) < 0.00475)) return false;
                if (!(std::fabs((*eledPhiAtVtx)[i]) < 0.06250)) return false;
                if (!((*eleHoverE)[i] < 0.06071)) return false;
                if (!(std::fabs((*eleEoverPInv)[i]) < 0.13274)) return false;
                //if (!(std::fabs((*eleD0)[i]) < 0.0564)) return false;
                //if (!(std::fabs((*eleDz)[i]) < 0.472)) return false;
                if (!((*eleMissHits)[i] <= 1)) return false;
                if (!((*eleIP3D)[i] < 0.03)) return false;

                //if (!((*elePFRelIsoWithEA)[i] < 0.198+0.506/(*elePt)[i])) return false;
            }
            else if (std::fabs((*eleSCEta)[i]) > 1.566 && std::fabs((*eleSCEta)[i]) < 2.5)
            {
                if (!((*eleSigmaIEtaIEta_2012)[i] < 0.03055)) return false;
                if (!(std::fabs((*eledEtaSeedAtVtx)[i]) < 0.00662)) return false;
                if (!(std::fabs((*eledPhiAtVtx)[i]) < 0.08807)) return false;
                if (!((*eleHoverE)[i] < 0.04518)) return false;
                if (!(std::fabs((*eleEoverPInv)[i]) < 0.90658)) return false;
                //if (!(std::fabs((*eleD0)[i]) < 0.0564)) return false;
                //if (!(std::fabs((*eleDz)[i]) < 0.472)) return false;
                if (!((*eleMissHits)[i] <= 1)) return false;
                if (!((*eleIP3D)[i] < 0.03)) return false;

                //if (!((*elePFRelIsoWithEA)[i] < 0.203+0.963/(*elePt)[i])) return false;
            }
        }
        else if (WPindex == 2) {
            if (std::fabs((*eleSCEta)[i]) < 1.4442)
            {
                if (!((*eleSigmaIEtaIEta_2012)[i] < 0.01016)) return false;
                if (!(std::fabs((*eledEtaSeedAtVtx)[i]) < 0.00316)) return false;
                if (!(std::fabs((*eledPhiAtVtx)[i]) < 0.03937)) return false;
                if (!((*eleHoverE)[i] < 0.02711)) return false;
                if (!(std::fabs((*eleEoverPInv)[i]) < 0.05304)) return false;
                //if (!(std::fabs((*eleD0)[i]) < 0.0564)) return false;
                //if (!(std::fabs((*eleDz)[i]) < 0.472)) return false;
                if (!((*eleMissHits)[i] <= 1)) return false;
                if (!((*eleIP3D)[i] < 0.03)) return false;
            }
            else if (std::fabs((*eleSCEta)[i]) > 1.566 && std::fabs((*eleSCEta)[i]) < 2.5)
            {
                if (!((*eleSigmaIEtaIEta_2012)[i] < 0.02946)) return false;
                if (!(std::fabs((*eledEtaSeedAtVtx)[i]) < 0.00565)) return false;
                if (!(std::fabs((*eledPhiAtVtx)[i]) < 0.03816)) return false;
                if (!((*eleHoverE)[i] < 0.03750)) return false;
                if (!(std::fabs((*eleEoverPInv)[i]) < 0.02356)) return false;
                //if (!(std::fabs((*eleD0)[i]) < 0.0564)) return false;
                //if (!(std::fabs((*eleDz)[i]) < 0.472)) return false;
                if (!((*eleMissHits)[i] <= 1)) return false;
                if (!((*eleIP3D)[i] < 0.03)) return false;
            }
        }
        else if (WPindex == 3) {
            if (std::fabs((*eleSCEta)[i]) < 1.4442)
            {
                if (!((*eleSigmaIEtaIEta_2012)[i] < 0.00971)) return false;
                if (!(std::fabs((*eledEtaSeedAtVtx)[i]) < 0.00240)) return false;
                if (!(std::fabs((*eledPhiAtVtx)[i]) < 0.02921)) return false;
                if (!((*eleHoverE)[i] < 0.02456)) return false;
                if (!(std::fabs((*eleEoverPInv)[i]) < 0.04474)) return false;
                //if (!(std::fabs((*eleD0)[i]) < 0.0564)) return false;
                //if (!(std::fabs((*eleDz)[i]) < 0.472)) return false;
                if (!((*eleMissHits)[i] <= 1)) return false;
                if (!((*eleIP3D)[i] < 0.03)) return false;
            }
            else if (std::fabs((*eleSCEta)[i]) > 1.566 && std::fabs((*eleSCEta)[i]) < 2.5)
            {
                if (!((*eleSigmaIEtaIEta_2012)[i] < 0.02941)) return false;
                if (!(std::fabs((*eledEtaSeedAtVtx)[i]) < 0.00559)) return false;
                if (!(std::fabs((*eledPhiAtVtx)[i]) < 0.02826)) return false;
                if (!((*eleHoverE)[i] < 0.01133)) return false;
                if (!(std::fabs((*eleEoverPInv)[i]) < 0.02343)) return false;
                //if (!(std::fabs((*eleD0)[i]) < 0.0564)) return false;
                //if (!(std::fabs((*eleDz)[i]) < 0.472)) return false;
                if (!((*eleMissHits)[i] <= 1)) return false;
                if (!((*eleIP3D)[i] < 0.03)) return false;
            }
        }
        else {
            return false;
        }
    }
    else if (collisionIsPP((COLL::TYPE)collType)) {

        // default WP for 2015 pp is medium.
        if (WPindex == 0) WPindex = 3;

        if (WPindex == 1) {
            if (std::fabs((*eleSCEta)[i]) < 1.4442)
            {
                if (!((*eleSigmaIEtaIEta_2012)[i] < 0.0114)) return false;
                if (!(std::fabs((*eledEtaAtVtx)[i]) < 0.0152)) return false;
                if (!(std::fabs((*eledPhiAtVtx)[i]) < 0.216)) return false;
                if (!((*eleHoverE)[i] < 0.181)) return false;
                if (!(std::fabs((*eleEoverPInv)[i]) < 0.207)) return false;
                if (!(std::fabs((*eleD0)[i]) < 0.0564)) return false;
                if (!(std::fabs((*eleDz)[i]) < 0.472)) return false;
                if (!((*eleMissHits)[i] <= 2)) return false;
            }
            else if (std::fabs((*eleSCEta)[i]) > 1.566 && std::fabs((*eleSCEta)[i]) < 2.5)
            {
                if (!((*eleSigmaIEtaIEta_2012)[i] < 0.0352)) return false;
                if (!(std::fabs((*eledEtaAtVtx)[i]) < 0.0113)) return false;
                if (!(std::fabs((*eledPhiAtVtx)[i]) < 0.237)) return false;
                if (!((*eleHoverE)[i] < 0.116)) return false;
                if (!(std::fabs((*eleEoverPInv)[i]) < 0.174)) return false;
                if (!(std::fabs((*eleD0)[i]) < 0.222)) return false;
                if (!(std::fabs((*eleDz)[i]) < 0.921)) return false;
                if (!((*eleMissHits)[i] <= 3)) return false;
            }
        }
        else if (WPindex == 3) {
            if (std::fabs((*eleSCEta)[i]) < 1.4442)
            {
                if (!((*eleSigmaIEtaIEta_2012)[i] < 0.0101)) return false;
                if (!(std::fabs((*eledEtaAtVtx)[i]) < 0.0103)) return false;
                if (!(std::fabs((*eledPhiAtVtx)[i]) < 0.0336)) return false;
                if (!((*eleHoverE)[i] < 0.0876)) return false;
                if (!(std::fabs((*eleEoverPInv)[i]) < 0.0174)) return false;
                if (!(std::fabs((*eleD0)[i]) < 0.0118)) return false;
                if (!(std::fabs((*eleDz)[i]) < 0.373)) return false;
                if (!((*eleMissHits)[i] <= 2)) return false;
            }
            else if (std::fabs((*eleSCEta)[i]) > 1.566 && std::fabs((*eleSCEta)[i]) < 2.5)
            {
                if (!((*eleSigmaIEtaIEta_2012)[i] < 0.0283)) return false;
                if (!(std::fabs((*eledEtaAtVtx)[i]) < 0.00733)) return false;
                if (!(std::fabs((*eledPhiAtVtx)[i]) < 0.114)) return false;
                if (!((*eleHoverE)[i] < 0.0678)) return false;
                if (!(std::fabs((*eleEoverPInv)[i]) < 0.0898)) return false;
                if (!(std::fabs((*eleD0)[i]) < 0.0739)) return false;
                if (!(std::fabs((*eleDz)[i]) < 0.602)) return false;
                if (!((*eleMissHits)[i] <= 1)) return false;
            }
        }
        else {
            return false;
        }
    }

    return true;
}

bool ggHiNtuplizer::passedMuSelection(int i, int collType)
{
    if (collisionIsHI((COLL::TYPE)collType)) {
        // tight muon ID from 2015 PbPb
        if (std::fabs((*muEta)[i]) < 2.4)
        {
            if (!((*muChi2NDF)[i] < 10)) return false;
            if (!(std::fabs((*muInnerD0)[i]) < 0.2)) return false;
            if (!(std::fabs((*muInnerDz)[i]) < 0.5)) return false;
            if (!((*muMuonHits)[i] > 0)) return false;
            if (!((*muStations)[i] > 1)) return false;
            if (!((*muTrkLayers)[i] > 5)) return false;
            if (!((*muPixelHits)[i] > 0)) return false;
        }
        else {
            return false;
        }
    }
    else if (collisionIsHI2018((COLL::TYPE)collType)) {
        // tight muon ID from 2018 PbPb
        if (std::fabs((*muEta)[i]) < 2.4)
        {
            if (!((*muIsGlobal)[i] > 0)) return false;
            if (!((*muIsPF)[i] > 0)) return false;
            if (!((*muChi2NDF)[i] >= 0 && (*muChi2NDF)[i] < 10)) return false;
//            if (!(std::fabs((*muInnerD0)[i]) < 0.2)) return false;
//            if (!(std::fabs((*muInnerDz)[i]) < 0.5)) return false;
            if (!(std::fabs((*muD0)[i]) < 0.2)) return false;
            if (!(std::fabs((*muDz)[i]) < 0.5)) return false;
            if (!((*muMuonHits)[i] > 0)) return false;
            if (!((*muStations)[i] > 1)) return false;
            if (!((*muTrkLayers)[i] > 5)) return false;
            if (!((*muPixelHits)[i] > 0)) return false;
        }
        else {
            return false;
        }
    }
    else {
        if (std::fabs((*muEta)[i]) < 2.4)
        {
            if (!((*muChi2NDF)[i] < 10)) return false;
            if (!(std::fabs((*muInnerD0)[i]) < 0.2)) return false;
            if (!(std::fabs((*muInnerDz)[i]) < 0.5)) return false;
            if (!((*muMuonHits)[i] > 0)) return false;
            if (!((*muStations)[i] > 1)) return false;
            if (!((*muTrkLayers)[i] > 5)) return false;
            if (!((*muPixelHits)[i] > 0)) return false;
        }
        else {
            return false;
        }
    }

    return true;
}

double ggHiNtuplizer::getElePtCorrFactor(int i, int collType, int hiBin)
{
    if (collisionIsHI2018((COLL::TYPE)collType)) {

        if (collisionIsDATA((COLL::TYPE)collType)) {

            if ((*elePt)[i] < 20) return 1;

            if (std::fabs((*eleSCEta)[i]) < 1.4442) {

                if (hiBin < 20) {
                    return 0.98904;//return 0.989;
                }
                else if (hiBin < 60) {
                    return 1.00560;//return 1.006;
                }
                else {
                    return 1.01586;//return 1.016;
                }
            }
            else if (std::fabs((*eleSCEta)[i]) > 1.566 && std::fabs((*eleSCEta)[i]) < 2.5) {

                if (hiBin < 20) {
                    return 0.96650;//return 0.967;
                }
                else if (hiBin < 60) {
                    return 1.01827;//return 1.018;
                }
                else {
                    return 1.05412;//return 1.054;
                }
            }

        }
        else if (collisionIsMC((COLL::TYPE)collType)) {

            if (std::fabs((*eleSCEta)[i]) < 1.4442) {

                if (hiBin < 20) {
                    //return 0.974;
                    return 0.971;
                }
                else if (hiBin < 60) {
                    //return 0.992;
                    return 0.990;
                }
                else {
                    //return 1.005;
                    return 1.003;
                }
            }
            else if (std::fabs((*eleSCEta)[i]) > 1.566 && std::fabs((*eleSCEta)[i]) < 2.5) {

                if (hiBin < 20) {
                    //return 0.913;
                    return 0.910;
                }
                else if (hiBin < 60) {
                    //return 0.952;
                    return 0.951;
                }
                else {
                    //return 0.992;
                    return 0.988;
                }
            }

        }
    }

    return 1;
}

double ggHiNtuplizer::getElePtSmearFactor(int i, int collType, int hiBin)
{
    if (collisionIsHI2018((COLL::TYPE)collType)) {

        if (collisionIsMC((COLL::TYPE)collType)) {

            if (std::fabs((*eleSCEta)[i]) < 1.4442) {

                if (hiBin < 20) {
                    return 1.19113 / 91.1876;
                }
                else if (hiBin < 60) {
                    return 1.28262 / 91.1876;
                }
                else {
                    return 2.20549 / 91.1876;
                }
            }
            else if (std::fabs((*eleSCEta)[i]) > 1.566 && std::fabs((*eleSCEta)[i]) < 2.5) {

                if (hiBin < 20) {
                    return 3.13988 / 91.1876;
                }
                else if (hiBin < 60) {
                    return 3.15340 / 91.1876;
                }
                else {
                    return 3.17194 / 91.1876;
                }
            }

        }
    }

    return 0;
}

double ggHiNtuplizer::getHiFJRho4Ele(int i, hiFJRho& hifjrho)
{
    return hifjrho.getRho((*eleEta)[i]);
}

double ggHiNtuplizer::getElePFIsoSubRho(int i, double eleRho)
{
    if (eleRho < 0) {
        return -999;
    }

    double a1 = 0.000906;
    double b1 = 0.241511;
    double c1 = 2.328351;
    double d1 = 49.606071;
    double e1 = -416.456563;
    double rho0 = 120;

    // 901.16345
    double f1 = (a1*rho0*rho0 + b1*rho0 + c1) - (d1*(std::log(rho0)*std::log(rho0))+e1*std::log(rho0));

    double sumPFIso = (*elePFChIso03)[i] + (*elePFPhoIso03)[i] + (*elePFNeuIso03)[i];
    double rhoUE = (eleRho <= rho0) ? (a1*eleRho*eleRho + b1*eleRho + c1) : (d1*(std::log(eleRho)*std::log(eleRho))+e1*std::log(eleRho) + f1);

    return (sumPFIso-rhoUE)/(*elePt)[i];
}

double ggHiNtuplizer::getValueByName(int i, std::string varName)
{
    if (varName == "phoE") {
        return (double)((*phoE)[i]);
    }
    else if (varName == "phoEt") {
        return (double)((*phoEt)[i]);
    }
    else if (varName == "phoEta") {
        return (double)((*phoEta)[i]);
    }
    else if (varName == "phoPhi") {
        return (double)((*phoPhi)[i]);
    }
    else if (varName == "phoSCE") {
        return (double)((*phoSCE)[i]);
    }
    else if (varName == "phoSCRawE") {
        return (double)((*phoSCRawE)[i]);
    }
    else if (varName == "phoSCEta") {
        return (double)((*phoSCEta)[i]);
    }
    else if (varName == "phoSCPhi") {
        return (double)((*phoSCPhi)[i]);
    }
    else if (varName == "phoSCEtaWidth") {
        return (double)((*phoSCEtaWidth)[i]);
    }
    else if (varName == "phoSCPhiWidth") {
        return (double)((*phoSCPhiWidth)[i]);
    }
    else if (varName == "phoE3x3_2012") {
        return (double)((*phoE3x3_2012)[i]);
    }
    else if (varName == "phoMaxEnergyXtal_2012") {
        return (double)((*phoMaxEnergyXtal_2012)[i]);
    }
    else if (varName == "phoE2nd_2012") {
        return (double)((*phoE2nd_2012)[i]);
    }
    else if (varName == "phoE_LR" || varName == "(phoELeft_2012-phoERight_2012)/(phoELeft_2012+phoERight_2012)") {

        if ((*phoELeft_2012)[i] != 0 || (*phoERight_2012)[i] != 0) {
            return (double)(((*phoELeft_2012)[i]-(*phoERight_2012)[i])/((*phoELeft_2012)[i]+(*phoERight_2012)[i]));
        }
        else {
            return 0;
        }
    }
    else if (varName == "phoE_TB" || varName == "(phoETop_2012-phoEBottom_2012)/(phoETop_2012+phoEBottom_2012)") {

        if ((*phoETop_2012)[i] != 0 || (*phoEBottom_2012)[i] != 0) {
            return (double)(((*phoETop_2012)[i]-(*phoEBottom_2012)[i])/((*phoETop_2012)[i]+(*phoEBottom_2012)[i]));
        }
        else {
            return 0;
        }
    }
    else if (varName == "phoSigmaIEtaIEta_2012") {
        return (double)((*phoSigmaIEtaIEta_2012)[i]);
    }
    else if (varName == "phoSigmaIEtaIPhi_2012") {
        return (double)((*phoSigmaIEtaIPhi_2012)[i]);
    }
    else if (varName == "phoSigmaIPhiIPhi_2012") {
        return (double)((*phoSigmaIPhiIPhi_2012)[i]);
    }
    else if (varName == "rho") {
        return (double)((rho));
    }
    else if (varName == "phoESEn") {
        return (double)((*phoESEn)[i]);
    }
    else {
        return -998877;
    }
}

void ggHiNtuplizer::copy2Vars(int i, float *vals, std::vector<std::string>& varNames, int nVars, int offset)
{
    for (int j = 0; j < nVars; ++j) {
        vals[j+offset] = getValueByName(i, varNames[j]);
    }
}

#endif /* TREEHEADERS_GGHINTUPLIZERTREE_H_ */
