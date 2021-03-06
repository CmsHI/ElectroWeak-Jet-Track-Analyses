#ifndef TREEHEADERS_GGHIFLATTREE_H_
#define TREEHEADERS_GGHIFLATTREE_H_

#include <TTree.h>
#include <TBranch.h>

#include <vector>

#include "ggHiNtuplizerTree.h"

class ggHiFlat {
public :
  ggHiFlat() {
      doEle = false;
      doPho = false;
      doMu = false;
      doMC = false;
  };
  ~ggHiFlat(){};
  void setupTreeForReading(TTree *t);
  void setupTreeForWriting(TTree* t);
  void clearEntry();
  void clearEntryEle();
  void clearEntryPho();
  void clearEntryMu();
  void clearEntryGen();
  void copyEle(ggHiNtuplizer &tggHiNtuplizer, int i);
  void copyPho(ggHiNtuplizer &tggHiNtuplizer, int i);
  void copyMu(ggHiNtuplizer &tggHiNtuplizer, int i);
  void copyGen(ggHiNtuplizer &tggHiNtuplizer, int i);

  void clone(ggHiFlat &gg);

  bool doEle;
  bool doPho;
  bool doMu;
  bool doMC;

  // Declaration of leaf types
  float weight;
  float weightCent;
  float weightKin;      // weight for kinematics : pt, eta
  float weightPthat;
  float pthat;
  int hiBin;
  int hiHF;
  float rho;
  UInt_t          run;
  ULong64_t       event;
  UInt_t          lumis;
  Bool_t          isData;
  Int_t           nPUInfo;
  int nPU;
  int puBX;
  float puTrue;
  //int nMC;
  int mcPID;
  int mcStatus;
  float mcVtx_x;
  float mcVtx_y;
  float mcVtx_z;
  float mcPt;
  float mcEta;
  float mcPhi;
  float mcE;
  float mcEt;
  float mcMass;
  int mcParentage;
  int mcMomPID;
  float mcMomPt;
  float mcMomEta;
  float mcMomPhi;
  float mcMomMass;
  int mcGMomPID;
  int mcIndex;
  float mcCalIsoDR03;
  float mcCalIsoDR04;
  float mcTrkIsoDR03;
  float mcTrkIsoDR04;
  //int nEle;
  int eleCharge;
  int eleChargeConsistent;
  int eleSCPixCharge;
  int eleCtfCharge;
  float eleEn;
  float eleD0;
  float eleDz;
  float eleIP3D;
  float eleD0Err;
  float eleDzErr;
  float eleIP3DErr;
  float eleTrkPt;
  float eleTrkEta;
  float eleTrkPhi;
  int eleTrkCharge;
  float eleTrkChi2;
  float eleTrkNdof;
  float eleTrkNormalizedChi2;
  int eleTrkValidHits;
  int eleTrkLayers;
  float elePt;
  float eleEta;
  float elePhi;
  float eleSCEn;
  float eleESEn;
  float eleSCEta;
  float eleSCPhi;
  float eleSCRawEn;
  float eleSCEtaWidth;
  float eleSCPhiWidth;
  float eleHoverE;
  float eleHoverEBc;
  float eleEoverP;
  float eleEoverPInv;
  float eleBrem;
  float eledEtaAtVtx;
  float eledPhiAtVtx;
  float eledEtaSeedAtVtx;
  float eleSigmaIEtaIEta;
  float eleSigmaIEtaIEta_2012;
  float eleSigmaIPhiIPhi;
  int eleMissHits;
  float eleESEffSigmaRR;
  float elePFChIso;
  float elePFPhoIso;
  float elePFNeuIso;
  float elePFPUIso;
  float elePFChIso03;
  float elePFPhoIso03;
  float elePFNeuIso03;
  float elePFChIso04;
  float elePFPhoIso04;
  float elePFNeuIso04;
  float eleR9;
  float eleE3x3;
  float eleE5x5;
  float eleR9Full5x5;
  float eleE3x3Full5x5;
  float eleE5x5Full5x5;
  int NClusters;
  int NEcalClusters;
  float eleSeedEn;
  float eleSeedEta;
  float eleSeedPhi;
  float eleSeedCryEta;
  float eleSeedCryPhi;
  float eleSeedCryIeta;
  float eleSeedCryIphi;
  float eleBC1E;
  float eleBC1Eta;
  float eleBC2E;
  float eleBC2Eta;
  int eleIDVeto;
  int eleIDLoose;
  int eleIDMedium;
  int eleIDTight;
  int elepassConversionVeto;
  float elePFRelIsoWithEA;
  float eleEffAreaTimesRho;
  //int nPho;
  float phoE;
  float phoEt;
  float phoEta;
  float phoPhi;
  float phoSCE;
  float phoSCRawE;
  float phoESEn;
  float phoSCEta;
  float phoSCPhi;
  float phoSCEtaWidth;
  float phoSCPhiWidth;
  float phoSCBrem;

  float phoESEffSigmaRR;

  int   phoHasPixelSeed;
  int   phoHasConversionTracks;
  float phoHadTowerOverEm;
  float phoHoverE;
  int   phoHoverEValid;
  float phoSigmaIEtaIEta;
  float phoR9;
  float phoE1x5;
  float phoE2x5;
  float phoE3x3;
  float phoE5x5;
  float phoMaxEnergyXtal;
  float phoSigmaEtaEta;
  float phoSigmaIEtaIEta_2012;
  float phoR9_2012;
  float phoE1x5_2012;
  float phoE2x5_2012;
  float phoE3x3_2012;
  float phoE5x5_2012;
  float phoMaxEnergyXtal_2012;
  float phoSigmaEtaEta_2012;

  int pho_isEle;
  int pho_is2015Noise;

  float  phoHadTowerOverEm1;
  float  phoHadTowerOverEm2;
  float  phoHoverE1;
  float  phoHoverE2;

  float  phoSigmaIEtaIPhi;
  float  phoSigmaIPhiIPhi;
  float  phoR1x5;
  float  phoR2x5;
  float  phoE2nd;
  float  phoETop;
  float  phoEBottom;
  float  phoELeft;
  float  phoERight;
  float  phoE1x3;
  float  phoE2x2;
  float  phoE2x5Max;
  float  phoE2x5Top;
  float  phoE2x5Bottom;
  float  phoE2x5Left;
  float  phoE2x5Right;
  //float  phoSMMajor;   // TODO: enable when they become available in future releases
  //float  phoSMMinor;   // TODO: enable when they become available in future releases
  //float  phoSMAlpha;   // TODO: enable when they become available in future releases

  float  phoSigmaIEtaIPhi_2012;
  float  phoSigmaIPhiIPhi_2012;
  float  phoR1x5_2012;
  float  phoR2x5_2012;
  float  phoE2nd_2012;
  float  phoETop_2012;
  float  phoEBottom_2012;
  float  phoELeft_2012;
  float  phoERight_2012;
  float  phoE1x3_2012;
  float  phoE2x2_2012;
  float  phoE2x5Max_2012;
  float  phoE2x5Top_2012;
  float  phoE2x5Bottom_2012;
  float  phoE2x5Left_2012;
  float  phoE2x5Right_2012;
  //float  phoSMMajor_2012;   // TODO: enable when they become available in future releases
  //float  phoSMMinor_2012;   // TODO: enable when they become available in future releases
  //float  phoSMAlpha_2012;   // TODO: enable when they become available in future releases

  float phoBC1E;
  float phoBC1Eta;
  //float phoBC2E;
  //float phoBC2Eta;
  float pho_ecalClusterIsoR2;
  float pho_ecalClusterIsoR3;
  float pho_ecalClusterIsoR4;
  float pho_ecalClusterIsoR5;
  float pho_hcalRechitIsoR1;
  float pho_hcalRechitIsoR2;
  float pho_hcalRechitIsoR3;
  float pho_hcalRechitIsoR4;
  float pho_hcalRechitIsoR5;
  float pho_trackIsoR1PtCut20;
  float pho_trackIsoR2PtCut20;
  float pho_trackIsoR3PtCut20;
  float pho_trackIsoR4PtCut20;
  float pho_trackIsoR5PtCut20;
  float pho_swissCrx;
  float pho_seedTime;
  int pho_genMatchedIndex;
  float pfcIso1;
  float pfcIso2;
  float pfcIso3;
  float pfcIso4;
  float pfcIso5;
  float pfpIso1;
  float pfpIso2;
  float pfpIso3;
  float pfpIso4;
  float pfpIso5;
  float pfnIso1;
  float pfnIso2;
  float pfnIso3;
  float pfnIso4;
  float pfnIso5;
  float pfpIso1subSC;
  float pfpIso2subSC;
  float pfpIso3subSC;
  float pfpIso4subSC;
  float pfpIso5subSC;
  float pfcIso1subUE;
  float pfcIso2subUE;
  float pfcIso3subUE;
  float pfcIso4subUE;
  float pfcIso5subUE;
  float pfpIso1subUE;
  float pfpIso2subUE;
  float pfpIso3subUE;
  float pfpIso4subUE;
  float pfpIso5subUE;
  float pfnIso1subUE;
  float pfnIso2subUE;
  float pfnIso3subUE;
  float pfnIso4subUE;
  float pfnIso5subUE;
  float pfpIso1subSCsubUE;
  float pfpIso2subSCsubUE;
  float pfpIso3subSCsubUE;
  float pfpIso4subSCsubUE;
  float pfpIso5subSCsubUE;
  float pfcIso1pTgt1p0subUE;
  float pfcIso2pTgt1p0subUE;
  float pfcIso3pTgt1p0subUE;
  float pfcIso4pTgt1p0subUE;
  float pfcIso5pTgt1p0subUE;
  float pfcIso1pTgt2p0subUE;
  float pfcIso2pTgt2p0subUE;
  float pfcIso3pTgt2p0subUE;
  float pfcIso4pTgt2p0subUE;
  float pfcIso5pTgt2p0subUE;
  float pfcIso1pTgt3p0subUE;
  float pfcIso2pTgt3p0subUE;
  float pfcIso3pTgt3p0subUE;
  float pfcIso4pTgt3p0subUE;
  float pfcIso5pTgt3p0subUE;

  float pfcIso2subUEec;
  float pfcIso3subUEec;
  float pfcIso4subUEec;
  float pfpIso2subUEec;
  float pfpIso3subUEec;
  float pfpIso4subUEec;
  float pfnIso2subUEec;
  float pfnIso3subUEec;
  float pfnIso4subUEec;
  float pfcIso2pTgt2p0subUEec;
  float pfcIso3pTgt2p0subUEec;
  float pfcIso4pTgt2p0subUEec;

  float trkIso3;
  float trkIso3subUE;
  float trkIso3subUEec;
  float trkIso3ID;
  float trkIso3IDsubUE;
  float trkIso3IDsubUEec;

  float phoEAc;
  float phoEAp;
  float phoEAn;
  float phoEAhoe;
  float phoEAsieie;

  //int nMu;
  float muPt;
  float muEta;
  float muPhi;
  int muCharge;
  int muType;
  int muIsGood;
  int muIsGlobal;
  int muIsTracker;
  int muIsPF;
  int muIsSTA;
  float muD0;
  float muDz;
  float muChi2NDF;
  float muInnerD0;
  float muInnerDz;
  float muInnerD0Err;
  float muInnerDzErr;
  float muInnerPt;
  float muInnerPtErr;
  float muInnerEta;
  int muTrkLayers;
  int muPixelLayers;
  int muPixelHits;
  int muMuonHits;
  int muTrkQuality;
  int muStations;
  float muIsoTrk;
  float muPFChIso;
  float muPFPhoIso;
  float muPFNeuIso;
  float muPFPUIso;
  bool isGenMatched; // added by hand
  int genMatchedIdx; // added by hand

  // List of branches
  TBranch        *b_weight;   //!
  TBranch        *b_weightCent;   //!
  TBranch        *b_weightKin;   //!
  TBranch        *b_weightPthat;   //!
  TBranch        *b_pthat;   //!
  TBranch        *b_hiBin;   //!
  TBranch        *b_hiHF;   //!
  TBranch        *b_rho;   //!
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
  //TBranch        *b_phoBC2E;   //!
  //TBranch        *b_phoBC2Eta;   //!
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
  TBranch        *b_pho_swissCrx;   //!
  TBranch        *b_pho_seedTime;   //!
  TBranch        *b_pho_genMatchedIndex;
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
  TBranch        *b_pfcIso2subUEec;   //!
  TBranch        *b_pfcIso3subUEec;   //!
  TBranch        *b_pfcIso4subUEec;   //!
  TBranch        *b_pfpIso2subUEec;   //!
  TBranch        *b_pfpIso3subUEec;   //!
  TBranch        *b_pfpIso4subUEec;   //!
  TBranch        *b_pfnIso2subUEec;   //!
  TBranch        *b_pfnIso3subUEec;   //!
  TBranch        *b_pfnIso4subUEec;   //!
  TBranch        *b_pfcIso2pTgt2p0subUEec;   //!
  TBranch        *b_pfcIso3pTgt2p0subUEec;   //!
  TBranch        *b_pfcIso4pTgt2p0subUEec;   //!
  TBranch        *b_trkIso3;   //!
  TBranch        *b_trkIso3subUE;   //!
  TBranch        *b_trkIso3subUEec;   //!
  TBranch        *b_trkIso3ID;   //!
  TBranch        *b_trkIso3IDsubUE;   //!
  TBranch        *b_trkIso3IDsubUEec;   //!
  TBranch        *b_phoEAc;   //!
  TBranch        *b_phoEAp;   //!
  TBranch        *b_phoEAn;   //!
  TBranch        *b_phoEAhoe;   //!
  TBranch        *b_phoEAsieie;   //!

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

void ggHiFlat::setupTreeForReading(TTree *t)
{
    b_weight = 0;
    b_weightCent = 0;
    b_weightKin = 0;
    b_pthat = 0;
    b_hiBin = 0;
    b_hiHF = 0;
    b_rho = 0;
    b_run = 0;
    b_event = 0;
    b_lumis = 0;
    b_isData = 0;
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

    b_phoE3x3_2012 = 0;
    b_phoBC1E = 0;
    b_phoBC1Eta = 0;
    //b_phoBC2E = 0;
    //b_phoBC2Eta = 0;
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
    b_pho_swissCrx = 0;
    b_pho_seedTime = 0;
    b_pho_genMatchedIndex = 0;
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
    b_pfcIso2subUEec = 0;
    b_pfcIso3subUEec = 0;
    b_pfcIso4subUEec = 0;
    b_pfpIso2subUEec = 0;
    b_pfpIso3subUEec = 0;
    b_pfpIso4subUEec = 0;
    b_pfnIso2subUEec = 0;
    b_pfnIso3subUEec = 0;
    b_pfnIso4subUEec = 0;
    b_pfcIso2pTgt2p0subUEec = 0;
    b_pfcIso3pTgt2p0subUEec = 0;
    b_pfcIso4pTgt2p0subUEec = 0;
    b_trkIso3 = 0;
    b_trkIso3subUE = 0;
    b_trkIso3subUEec = 0;
    b_trkIso3ID = 0;
    b_trkIso3IDsubUE = 0;
    b_trkIso3IDsubUEec = 0;
    b_phoEAc = 0;
    b_phoEAp = 0;
    b_phoEAn = 0;
    b_phoEAhoe = 0;
    b_phoEAsieie = 0;

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
    if (t->GetBranch("weight")) t->SetBranchAddress("weight", &weight, &b_weight);
    if (t->GetBranch("weightCent")) t->SetBranchAddress("weightCent", &weightCent, &b_weightCent);
    if (t->GetBranch("weightKin")) t->SetBranchAddress("weightKin", &weightKin, &b_weightKin);
    if (t->GetBranch("weightPthat")) t->SetBranchAddress("weightPthat", &weightPthat, &b_weightPthat);
    if (t->GetBranch("pthat")) t->SetBranchAddress("pthat", &pthat, &b_pthat);
    if (t->GetBranch("hiBin")) t->SetBranchAddress("hiBin", &hiBin, &b_hiBin);
    if (t->GetBranch("hiHF")) t->SetBranchAddress("hiHF", &hiHF, &b_hiHF);
    if (t->GetBranch("rho")) t->SetBranchAddress("rho", &rho, &b_rho);
    if (t->GetBranch("run")) t->SetBranchAddress("run", &run, &b_run);
    if (t->GetBranch("event")) t->SetBranchAddress("event", &event, &b_event);
    if (t->GetBranch("lumis")) t->SetBranchAddress("lumis", &lumis, &b_lumis);
    if (t->GetBranch("isData")) t->SetBranchAddress("isData", &isData, &b_isData);
    if (t->GetBranch("nPUInfo")) t->SetBranchAddress("nPUInfo", &nPUInfo, &b_nPUInfo);
    if (t->GetBranch("nPU")) t->SetBranchAddress("nPU", &nPU, &b_nPU);
    if (t->GetBranch("puBX")) t->SetBranchAddress("puBX", &puBX, &b_puBX);
    if (t->GetBranch("puTrue")) t->SetBranchAddress("puTrue", &puTrue, &b_puTrue);
    //if (t->GetBranch("nMC")) t->SetBranchAddress("nMC", &nMC, &b_nMC);
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
    //if (t->GetBranch("nEle")) t->SetBranchAddress("nEle", &nEle, &b_nEle);
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
    //if (t->GetBranch("nPho")) t->SetBranchAddress("nPho", &nPho, &b_nPho);
    if (t->GetBranch("phoE")) t->SetBranchAddress("phoE", &phoE, &b_phoE);
    if (t->GetBranch("phoEt")) t->SetBranchAddress("phoEt", &phoEt, &b_phoEt);
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
    //if (t->GetBranch("phoBC2E")) t->SetBranchAddress("phoBC2E", &phoBC2E, &b_phoBC2E);
    //if (t->GetBranch("phoBC2Eta")) t->SetBranchAddress("phoBC2Eta", &phoBC2Eta, &b_phoBC2Eta);
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
    if (t->GetBranch("pho_swissCrx")) t->SetBranchAddress("pho_swissCrx", &pho_swissCrx, &b_pho_swissCrx);
    if (t->GetBranch("pho_seedTime")) t->SetBranchAddress("pho_seedTime", &pho_seedTime, &b_pho_seedTime);
    if (t->GetBranch("pho_genMatchedIndex")) t->SetBranchAddress("pho_genMatchedIndex", &pho_genMatchedIndex, &b_pho_genMatchedIndex);
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
    if (t->GetBranch("pfpIso1subSC")) t->SetBranchAddress("pfpIso1subSC", &pfpIso1subSC, &b_pfpIso1subSC);
    if (t->GetBranch("pfpIso2subSC")) t->SetBranchAddress("pfpIso2subSC", &pfpIso2subSC, &b_pfpIso2subSC);
    if (t->GetBranch("pfpIso3subSC")) t->SetBranchAddress("pfpIso3subSC", &pfpIso3subSC, &b_pfpIso3subSC);
    if (t->GetBranch("pfpIso4subSC")) t->SetBranchAddress("pfpIso4subSC", &pfpIso4subSC, &b_pfpIso4subSC);
    if (t->GetBranch("pfpIso5subSC")) t->SetBranchAddress("pfpIso5subSC", &pfpIso5subSC, &b_pfpIso5subSC);
    if (t->GetBranch("pfnIso1")) t->SetBranchAddress("pfnIso1", &pfnIso1, &b_pfnIso1);
    if (t->GetBranch("pfnIso2")) t->SetBranchAddress("pfnIso2", &pfnIso2, &b_pfnIso2);
    if (t->GetBranch("pfnIso3")) t->SetBranchAddress("pfnIso3", &pfnIso3, &b_pfnIso3);
    if (t->GetBranch("pfnIso4")) t->SetBranchAddress("pfnIso4", &pfnIso4, &b_pfnIso4);
    if (t->GetBranch("pfnIso5")) t->SetBranchAddress("pfnIso5", &pfnIso5, &b_pfnIso5);
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
    if (t->GetBranch("pfcIso2subUEec")) t->SetBranchAddress("pfcIso2subUEec", &pfcIso2subUEec, &b_pfcIso2subUEec);
    if (t->GetBranch("pfcIso3subUEec")) t->SetBranchAddress("pfcIso3subUEec", &pfcIso3subUEec, &b_pfcIso3subUEec);
    if (t->GetBranch("pfcIso4subUEec")) t->SetBranchAddress("pfcIso4subUEec", &pfcIso4subUEec, &b_pfcIso4subUEec);
    if (t->GetBranch("pfpIso2subUEec")) t->SetBranchAddress("pfpIso2subUEec", &pfpIso2subUEec, &b_pfpIso2subUEec);
    if (t->GetBranch("pfpIso3subUEec")) t->SetBranchAddress("pfpIso3subUEec", &pfpIso3subUEec, &b_pfpIso3subUEec);
    if (t->GetBranch("pfpIso4subUEec")) t->SetBranchAddress("pfpIso4subUEec", &pfpIso4subUEec, &b_pfpIso4subUEec);
    if (t->GetBranch("pfnIso2subUEec")) t->SetBranchAddress("pfnIso2subUEec", &pfnIso2subUEec, &b_pfnIso2subUEec);
    if (t->GetBranch("pfnIso3subUEec")) t->SetBranchAddress("pfnIso3subUEec", &pfnIso3subUEec, &b_pfnIso3subUEec);
    if (t->GetBranch("pfnIso4subUEec")) t->SetBranchAddress("pfnIso4subUEec", &pfnIso4subUEec, &b_pfnIso4subUEec);
    if (t->GetBranch("pfcIso2pTgt2p0subUEec")) t->SetBranchAddress("pfcIso2pTgt2p0subUEec", &pfcIso2pTgt2p0subUEec, &b_pfcIso2pTgt2p0subUEec);
    if (t->GetBranch("pfcIso3pTgt2p0subUEec")) t->SetBranchAddress("pfcIso3pTgt2p0subUEec", &pfcIso3pTgt2p0subUEec, &b_pfcIso3pTgt2p0subUEec);
    if (t->GetBranch("pfcIso4pTgt2p0subUEec")) t->SetBranchAddress("pfcIso4pTgt2p0subUEec", &pfcIso4pTgt2p0subUEec, &b_pfcIso4pTgt2p0subUEec);
    if (t->GetBranch("trkIso3")) t->SetBranchAddress("trkIso3", &trkIso3, &b_trkIso3);
    if (t->GetBranch("trkIso3subUE")) t->SetBranchAddress("trkIso3subUE", &trkIso3subUE, &b_trkIso3subUE);
    if (t->GetBranch("trkIso3subUEec")) t->SetBranchAddress("trkIso3subUEec", &trkIso3subUEec, &b_trkIso3subUEec);
    if (t->GetBranch("trkIso3ID")) t->SetBranchAddress("trkIso3ID", &trkIso3ID, &b_trkIso3ID);
    if (t->GetBranch("trkIso3IDsubUE")) t->SetBranchAddress("trkIso3IDsubUE", &trkIso3IDsubUE, &b_trkIso3IDsubUE);
    if (t->GetBranch("trkIso3IDsubUEec")) t->SetBranchAddress("trkIso3IDsubUEec", &trkIso3IDsubUEec, &b_trkIso3IDsubUEec);
    if (t->GetBranch("phoEAc")) t->SetBranchAddress("phoEAc", &phoEAc, &b_phoEAc);
    if (t->GetBranch("phoEAp")) t->SetBranchAddress("phoEAp", &phoEAp, &b_phoEAp);
    if (t->GetBranch("phoEAn")) t->SetBranchAddress("phoEAn", &phoEAn, &b_phoEAn);
    if (t->GetBranch("phoEAhoe")) t->SetBranchAddress("phoEAhoe", &phoEAhoe, &b_phoEAhoe);
    if (t->GetBranch("phoEAsieie")) t->SetBranchAddress("phoEAsieie", &phoEAsieie, &b_phoEAsieie);

    //if (t->GetBranch("nMu")) t->SetBranchAddress("nMu", &nMu, &b_nMu);
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

void ggHiFlat::setupTreeForWriting(TTree* t)
{
    t->Branch("weight", &weight);
    t->Branch("weightCent", &weightCent);
    t->Branch("weightKin", &weightKin);
    t->Branch("weightPthat", &weightPthat);
    t->Branch("pthat", &pthat);
    t->Branch("hiBin", &hiBin);
    t->Branch("hiHF", &hiHF);
    t->Branch("rho", &rho);
    t->Branch("run", &run);
    t->Branch("event", &event);
    t->Branch("lumis", &lumis);
    t->Branch("isData", &isData);
    t->Branch("nPUInfo", &nPUInfo);
    t->Branch("nPU", &nPU);
    t->Branch("puBX", &puBX);
    t->Branch("puTrue", &puTrue);
    if (doMC) {
        //t->Branch("nMC", &nMC);
        t->Branch("mcPID", &mcPID);
        t->Branch("mcStatus", &mcStatus);
        t->Branch("mcVtx_x", &mcVtx_x);
        t->Branch("mcVtx_y", &mcVtx_y);
        t->Branch("mcVtx_z", &mcVtx_z);
        t->Branch("mcPt", &mcPt);
        t->Branch("mcEta", &mcEta);
        t->Branch("mcPhi", &mcPhi);
        t->Branch("mcE", &mcE);
        t->Branch("mcEt", &mcEt);
        t->Branch("mcMass", &mcMass);
        t->Branch("mcParentage", &mcParentage);
        t->Branch("mcMomPID", &mcMomPID);
        t->Branch("mcMomPt", &mcMomPt);
        t->Branch("mcMomEta", &mcMomEta);
        t->Branch("mcMomPhi", &mcMomPhi);
        t->Branch("mcMomMass", &mcMomMass);
        t->Branch("mcGMomPID", &mcGMomPID);
        t->Branch("mcIndex", &mcIndex);
        t->Branch("mcCalIsoDR03", &mcCalIsoDR03);
        t->Branch("mcCalIsoDR04", &mcCalIsoDR04);
        t->Branch("mcTrkIsoDR03", &mcTrkIsoDR03);
        t->Branch("mcTrkIsoDR04", &mcTrkIsoDR04);
    }
    if (doEle) {
        //t->Branch("nEle", &nEle);
        t->Branch("eleCharge", &eleCharge);
        t->Branch("eleChargeConsistent", &eleChargeConsistent);
        t->Branch("eleSCPixCharge", &eleSCPixCharge);
        t->Branch("eleCtfCharge", &eleCtfCharge);
        t->Branch("eleEn", &eleEn);
        t->Branch("eleD0", &eleD0);
        t->Branch("eleDz", &eleDz);
        t->Branch("eleIP3D", &eleIP3D);
        t->Branch("eleD0Err", &eleD0Err);
        t->Branch("eleDzErr", &eleDzErr);
        t->Branch("eleIP3DErr", &eleIP3DErr);
        t->Branch("eleTrkPt", &eleTrkPt);
        t->Branch("eleTrkEta", &eleTrkEta);
        t->Branch("eleTrkPhi", &eleTrkPhi);
        t->Branch("eleTrkCharge", &eleTrkCharge);
        t->Branch("eleTrkChi2", &eleTrkChi2);
        t->Branch("eleTrkNdof", &eleTrkNdof);
        t->Branch("eleTrkNormalizedChi2", &eleTrkNormalizedChi2);
        t->Branch("eleTrkValidHits", &eleTrkValidHits);
        t->Branch("eleTrkLayers", &eleTrkLayers);
        t->Branch("elePt", &elePt);
        t->Branch("eleEta", &eleEta);
        t->Branch("elePhi", &elePhi);
        t->Branch("eleSCEn", &eleSCEn);
        t->Branch("eleESEn", &eleESEn);
        t->Branch("eleSCEta", &eleSCEta);
        t->Branch("eleSCPhi", &eleSCPhi);
        t->Branch("eleSCRawEn", &eleSCRawEn);
        t->Branch("eleSCEtaWidth", &eleSCEtaWidth);
        t->Branch("eleSCPhiWidth", &eleSCPhiWidth);
        t->Branch("eleHoverE", &eleHoverE);
        t->Branch("eleHoverEBc", &eleHoverEBc);
        t->Branch("eleEoverP", &eleEoverP);
        t->Branch("eleEoverPInv", &eleEoverPInv);
        t->Branch("eleBrem", &eleBrem);
        t->Branch("eledEtaAtVtx", &eledEtaAtVtx);
        t->Branch("eledPhiAtVtx", &eledPhiAtVtx);
        t->Branch("eledEtaSeedAtVtx", &eledEtaSeedAtVtx);
        t->Branch("eleSigmaIEtaIEta", &eleSigmaIEtaIEta);
        t->Branch("eleSigmaIEtaIEta_2012", &eleSigmaIEtaIEta_2012);
        t->Branch("eleSigmaIPhiIPhi", &eleSigmaIPhiIPhi);
        t->Branch("eleMissHits", &eleMissHits);
        t->Branch("eleESEffSigmaRR", &eleESEffSigmaRR);
        t->Branch("elePFChIso", &elePFChIso);
        t->Branch("elePFPhoIso", &elePFPhoIso);
        t->Branch("elePFNeuIso", &elePFNeuIso);
        t->Branch("elePFPUIso", &elePFPUIso);
        t->Branch("elePFChIso03", &elePFChIso03);
        t->Branch("elePFPhoIso03", &elePFPhoIso03);
        t->Branch("elePFNeuIso03", &elePFNeuIso03);
        t->Branch("elePFChIso04", &elePFChIso04);
        t->Branch("elePFPhoIso04", &elePFPhoIso04);
        t->Branch("elePFNeuIso04", &elePFNeuIso04);
        t->Branch("eleR9", &eleR9);
        t->Branch("eleE3x3", &eleE3x3);
        t->Branch("eleE5x5", &eleE5x5);
        t->Branch("eleR9Full5x5", &eleR9Full5x5);
        t->Branch("eleE3x3Full5x5", &eleE3x3Full5x5);
        t->Branch("eleE5x5Full5x5", &eleE5x5Full5x5);
        t->Branch("NClusters", &NClusters);
        t->Branch("NEcalClusters", &NEcalClusters);
        t->Branch("eleSeedEn", &eleSeedEn);
        t->Branch("eleSeedEta", &eleSeedEta);
        t->Branch("eleSeedPhi", &eleSeedPhi);
        t->Branch("eleSeedCryEta", &eleSeedCryEta);
        t->Branch("eleSeedCryPhi", &eleSeedCryPhi);
        t->Branch("eleSeedCryIeta", &eleSeedCryIeta);
        t->Branch("eleSeedCryIphi", &eleSeedCryIphi);
        t->Branch("eleBC1E", &eleBC1E);
        t->Branch("eleBC1Eta", &eleBC1Eta);
        t->Branch("eleBC2E", &eleBC2E);
        t->Branch("eleBC2Eta", &eleBC2Eta);
        t->Branch("eleIDVeto", &eleIDVeto);
        t->Branch("eleIDLoose", &eleIDLoose);
        t->Branch("eleIDMedium", &eleIDMedium);
        t->Branch("eleIDTight", &eleIDTight);
        t->Branch("elepassConversionVeto", &elepassConversionVeto);
        t->Branch("elePFRelIsoWithEA", &elePFRelIsoWithEA);
        t->Branch("eleEffAreaTimesRho", &eleEffAreaTimesRho);
    }
    if (doPho) {
        //t->Branch("nPho", &nPho);
        t->Branch("phoE", &phoE);
        t->Branch("phoEt", &phoEt);
        t->Branch("phoEta", &phoEta);
        t->Branch("phoPhi", &phoPhi);
        t->Branch("phoSCE", &phoSCE);
        t->Branch("phoSCRawE", &phoSCRawE);
        t->Branch("phoESEn", &phoESEn);
        t->Branch("phoSCEta", &phoSCEta);
        t->Branch("phoSCPhi", &phoSCPhi);
        t->Branch("phoSCEtaWidth", &phoSCEtaWidth);
        t->Branch("phoSCPhiWidth", &phoSCPhiWidth);
        t->Branch("phoSCBrem", &phoSCBrem);

        t->Branch("phoESEffSigmaRR", &phoESEffSigmaRR);

        t->Branch("phoHasPixelSeed", &phoHasPixelSeed);
        t->Branch("phoHasConversionTracks", &phoHasConversionTracks);
        t->Branch("phoHadTowerOverEm", &phoHadTowerOverEm);
        t->Branch("phoHoverE", &phoHoverE);
        t->Branch("phoHoverEValid", &phoHoverEValid);
        t->Branch("phoSigmaIEtaIEta", &phoSigmaIEtaIEta);
        t->Branch("phoR9", &phoR9);
        t->Branch("phoE1x5", &phoE1x5);
        t->Branch("phoE2x5", &phoE2x5);
        t->Branch("phoE3x3", &phoE3x3);
        t->Branch("phoE5x5", &phoE5x5);
        t->Branch("phoMaxEnergyXtal", &phoMaxEnergyXtal);
        t->Branch("phoSigmaEtaEta", &phoSigmaEtaEta);
        t->Branch("phoSigmaIEtaIEta_2012", &phoSigmaIEtaIEta_2012);
        t->Branch("phoR9_2012", &phoR9_2012);
        t->Branch("phoE1x5_2012", &phoE1x5_2012);
        t->Branch("phoE2x5_2012", &phoE2x5_2012);
        t->Branch("phoE3x3_2012", &phoE3x3_2012);
        t->Branch("phoE5x5_2012", &phoE5x5_2012);
        t->Branch("phoMaxEnergyXtal_2012", &phoMaxEnergyXtal_2012);
        t->Branch("phoSigmaEtaEta_2012", &phoSigmaEtaEta_2012);

        t->Branch("pho_isEle", &pho_isEle);
        t->Branch("pho_is2015Noise", &pho_is2015Noise);

        t->Branch("phoHadTowerOverEm1", &phoHadTowerOverEm1);
        t->Branch("phoHadTowerOverEm2", &phoHadTowerOverEm2);
        t->Branch("phoHoverE1", &phoHoverE1);
        t->Branch("phoHoverE2", &phoHoverE2);

        t->Branch("phoSigmaIEtaIPhi", &phoSigmaIEtaIPhi);
        t->Branch("phoSigmaIPhiIPhi", &phoSigmaIPhiIPhi);
        t->Branch("phoR1x5", &phoR1x5);
        t->Branch("phoR2x5", &phoR2x5);
        t->Branch("phoE2nd", &phoE2nd);
        t->Branch("phoETop", &phoETop);
        t->Branch("phoEBottom", &phoEBottom);
        t->Branch("phoELeft", &phoELeft);
        t->Branch("phoERight", &phoERight);
        t->Branch("phoE1x3", &phoE1x3);
        t->Branch("phoE2x2", &phoE2x2);
        t->Branch("phoE2x5Max", &phoE2x5Max);
        t->Branch("phoE2x5Top", &phoE2x5Top);
        t->Branch("phoE2x5Bottom", &phoE2x5Bottom);
        t->Branch("phoE2x5Left", &phoE2x5Left);
        t->Branch("phoE2x5Right", &phoE2x5Right);
        //t->Branch("phoSMMajor", &phoSMMajor);
        //t->Branch("phoSMMinor", &phoSMMinor);
        //t->Branch("phoSMAlpha", &phoSMAlpha);

        t->Branch("phoSigmaIEtaIPhi_2012", &phoSigmaIEtaIPhi_2012);
        t->Branch("phoSigmaIPhiIPhi_2012", &phoSigmaIPhiIPhi_2012);
        t->Branch("phoR1x5_2012", &phoR1x5_2012);
        t->Branch("phoR2x5_2012", &phoR2x5_2012);
        t->Branch("phoE2nd_2012", &phoE2nd_2012);
        t->Branch("phoETop_2012", &phoETop_2012);
        t->Branch("phoEBottom_2012", &phoEBottom_2012);
        t->Branch("phoELeft_2012", &phoELeft_2012);
        t->Branch("phoERight_2012", &phoERight_2012);
        t->Branch("phoE1x3_2012", &phoE1x3_2012);
        t->Branch("phoE2x2_2012", &phoE2x2_2012);
        t->Branch("phoE2x5Max_2012", &phoE2x5Max_2012);
        t->Branch("phoE2x5Top_2012", &phoE2x5Top_2012);
        t->Branch("phoE2x5Bottom_2012", &phoE2x5Bottom_2012);
        t->Branch("phoE2x5Left_2012", &phoE2x5Left_2012);
        t->Branch("phoE2x5Right_2012", &phoE2x5Right_2012);
        //t->Branch("phoSMMajor_2012", &phoSMMajor_2012);
        //t->Branch("phoSMMinor_2012", &phoSMMinor_2012);
        //t->Branch("phoSMAlpha_2012", &phoSMAlpha_2012);

        t->Branch("phoBC1E", &phoBC1E);
        t->Branch("phoBC1Eta", &phoBC1Eta);
        //t->Branch("phoBC2E", &phoBC2E);
        //t->Branch("phoBC2Eta", &phoBC2Eta);
        t->Branch("pho_ecalClusterIsoR2", &pho_ecalClusterIsoR2);
        t->Branch("pho_ecalClusterIsoR3", &pho_ecalClusterIsoR3);
        t->Branch("pho_ecalClusterIsoR4", &pho_ecalClusterIsoR4);
        t->Branch("pho_ecalClusterIsoR5", &pho_ecalClusterIsoR5);
        t->Branch("pho_hcalRechitIsoR1", &pho_hcalRechitIsoR1);
        t->Branch("pho_hcalRechitIsoR2", &pho_hcalRechitIsoR2);
        t->Branch("pho_hcalRechitIsoR3", &pho_hcalRechitIsoR3);
        t->Branch("pho_hcalRechitIsoR4", &pho_hcalRechitIsoR4);
        t->Branch("pho_hcalRechitIsoR5", &pho_hcalRechitIsoR5);
        t->Branch("pho_trackIsoR1PtCut20", &pho_trackIsoR1PtCut20);
        t->Branch("pho_trackIsoR2PtCut20", &pho_trackIsoR2PtCut20);
        t->Branch("pho_trackIsoR3PtCut20", &pho_trackIsoR3PtCut20);
        t->Branch("pho_trackIsoR4PtCut20", &pho_trackIsoR4PtCut20);
        t->Branch("pho_trackIsoR5PtCut20", &pho_trackIsoR5PtCut20);
        t->Branch("pho_swissCrx", &pho_swissCrx);
        t->Branch("pho_seedTime", &pho_seedTime);
        t->Branch("pho_genMatchedIndex", &pho_genMatchedIndex);
        t->Branch("pfcIso1", &pfcIso1);
        t->Branch("pfcIso2", &pfcIso2);
        t->Branch("pfcIso3", &pfcIso3);
        t->Branch("pfcIso4", &pfcIso4);
        t->Branch("pfcIso5", &pfcIso5);
        t->Branch("pfpIso1", &pfpIso1);
        t->Branch("pfpIso2", &pfpIso2);
        t->Branch("pfpIso3", &pfpIso3);
        t->Branch("pfpIso4", &pfpIso4);
        t->Branch("pfpIso5", &pfpIso5);
        t->Branch("pfnIso1", &pfnIso1);
        t->Branch("pfnIso2", &pfnIso2);
        t->Branch("pfnIso3", &pfnIso3);
        t->Branch("pfnIso4", &pfnIso4);
        t->Branch("pfnIso5", &pfnIso5);
        t->Branch("pfpIso1subSC", &pfpIso1subSC);
        t->Branch("pfpIso2subSC", &pfpIso2subSC);
        t->Branch("pfpIso3subSC", &pfpIso3subSC);
        t->Branch("pfpIso4subSC", &pfpIso4subSC);
        t->Branch("pfpIso5subSC", &pfpIso5subSC);
        t->Branch("pfcIso1subUE", &pfcIso1subUE);
        t->Branch("pfcIso2subUE", &pfcIso2subUE);
        t->Branch("pfcIso3subUE", &pfcIso3subUE);
        t->Branch("pfcIso4subUE", &pfcIso4subUE);
        t->Branch("pfcIso5subUE", &pfcIso5subUE);
        t->Branch("pfpIso1subUE", &pfpIso1subUE);
        t->Branch("pfpIso2subUE", &pfpIso2subUE);
        t->Branch("pfpIso3subUE", &pfpIso3subUE);
        t->Branch("pfpIso4subUE", &pfpIso4subUE);
        t->Branch("pfpIso5subUE", &pfpIso5subUE);
        t->Branch("pfnIso1subUE", &pfnIso1subUE);
        t->Branch("pfnIso2subUE", &pfnIso2subUE);
        t->Branch("pfnIso3subUE", &pfnIso3subUE);
        t->Branch("pfnIso4subUE", &pfnIso4subUE);
        t->Branch("pfnIso5subUE", &pfnIso5subUE);
        t->Branch("pfpIso1subSCsubUE", &pfpIso1subSCsubUE);
        t->Branch("pfpIso2subSCsubUE", &pfpIso2subSCsubUE);
        t->Branch("pfpIso3subSCsubUE", &pfpIso3subSCsubUE);
        t->Branch("pfpIso4subSCsubUE", &pfpIso4subSCsubUE);
        t->Branch("pfpIso5subSCsubUE", &pfpIso5subSCsubUE);
        t->Branch("pfcIso1pTgt1p0subUE", &pfcIso1pTgt1p0subUE);
        t->Branch("pfcIso2pTgt1p0subUE", &pfcIso2pTgt1p0subUE);
        t->Branch("pfcIso3pTgt1p0subUE", &pfcIso3pTgt1p0subUE);
        t->Branch("pfcIso4pTgt1p0subUE", &pfcIso4pTgt1p0subUE);
        t->Branch("pfcIso5pTgt1p0subUE", &pfcIso5pTgt1p0subUE);
        t->Branch("pfcIso1pTgt2p0subUE", &pfcIso1pTgt2p0subUE);
        t->Branch("pfcIso2pTgt2p0subUE", &pfcIso2pTgt2p0subUE);
        t->Branch("pfcIso3pTgt2p0subUE", &pfcIso3pTgt2p0subUE);
        t->Branch("pfcIso4pTgt2p0subUE", &pfcIso4pTgt2p0subUE);
        t->Branch("pfcIso5pTgt2p0subUE", &pfcIso5pTgt2p0subUE);
        t->Branch("pfcIso1pTgt3p0subUE", &pfcIso1pTgt3p0subUE);
        t->Branch("pfcIso2pTgt3p0subUE", &pfcIso2pTgt3p0subUE);
        t->Branch("pfcIso3pTgt3p0subUE", &pfcIso3pTgt3p0subUE);
        t->Branch("pfcIso4pTgt3p0subUE", &pfcIso4pTgt3p0subUE);
        t->Branch("pfcIso5pTgt3p0subUE", &pfcIso5pTgt3p0subUE);

        t->Branch("pfcIso2subUEec", &pfcIso2subUEec);
        t->Branch("pfcIso3subUEec", &pfcIso3subUEec);
        t->Branch("pfcIso4subUEec", &pfcIso4subUEec);
        t->Branch("pfpIso2subUEec", &pfpIso2subUEec);
        t->Branch("pfpIso3subUEec", &pfpIso3subUEec);
        t->Branch("pfpIso4subUEec", &pfpIso4subUEec);
        t->Branch("pfnIso2subUEec", &pfnIso2subUEec);
        t->Branch("pfnIso3subUEec", &pfnIso3subUEec);
        t->Branch("pfnIso4subUEec", &pfnIso4subUEec);
        t->Branch("pfcIso2pTgt2p0subUEec", &pfcIso2pTgt2p0subUEec);
        t->Branch("pfcIso3pTgt2p0subUEec", &pfcIso3pTgt2p0subUEec);
        t->Branch("pfcIso4pTgt2p0subUEec", &pfcIso4pTgt2p0subUEec);
        t->Branch("trkIso3", &trkIso3);
        t->Branch("trkIso3subUE", &trkIso3subUE);
        t->Branch("trkIso3subUEec", &trkIso3subUEec);
        t->Branch("trkIso3ID", &trkIso3ID);
        t->Branch("trkIso3IDsubUE", &trkIso3IDsubUE);
        t->Branch("trkIso3IDsubUEec", &trkIso3IDsubUEec);
        t->Branch("phoEAc", &phoEAc);
        t->Branch("phoEAp", &phoEAp);
        t->Branch("phoEAn", &phoEAn);
        t->Branch("phoEAhoe", &phoEAhoe);
        t->Branch("phoEAsieie", &phoEAsieie);
    }
    if (doMu) {
        //t->Branch("nMu", &nMu);
        t->Branch("muPt", &muPt);
        t->Branch("muEta", &muEta);
        t->Branch("muPhi", &muPhi);
        t->Branch("muCharge", &muCharge);
        t->Branch("muType", &muType);
        t->Branch("muIsGood", &muIsGood);
        t->Branch("muIsGlobal", &muIsGlobal);
        t->Branch("muIsTracker", &muIsTracker);
        t->Branch("muIsPF", &muIsPF);
        t->Branch("muIsSTA", &muIsSTA);
        t->Branch("muD0", &muD0);
        t->Branch("muDz", &muDz);
        t->Branch("muChi2NDF", &muChi2NDF);
        t->Branch("muInnerD0", &muInnerD0);
        t->Branch("muInnerDz", &muInnerDz);
        t->Branch("muInnerD0Err", &muInnerD0Err);
        t->Branch("muInnerDzErr", &muInnerDzErr);
        t->Branch("muInnerPt", &muInnerPt);
        t->Branch("muInnerPtErr", &muInnerPtErr);
        t->Branch("muInnerEta", &muInnerEta);
        t->Branch("muTrkLayers", &muTrkLayers);
        t->Branch("muPixelLayers", &muPixelLayers);
        t->Branch("muPixelHits", &muPixelHits);
        t->Branch("muMuonHits", &muMuonHits);
        t->Branch("muTrkQuality", &muTrkQuality);
        t->Branch("muStations", &muStations);
        t->Branch("muIsoTrk", &muIsoTrk);
        t->Branch("muPFChIso", &muPFChIso);
        t->Branch("muPFPhoIso", &muPFPhoIso);
        t->Branch("muPFNeuIso", &muPFNeuIso);
        t->Branch("muPFPUIso", &muPFPUIso);
    }
}

void ggHiFlat::clearEntry()
{
    weight = -1;
    weightCent = -1;
    weightKin = -1;
    weightPthat = -1;
    pthat = -1;
    hiBin = -1;
    hiHF = -1;
    rho = -1;
    run = 987987;
    event = 987987;
    lumis = 987987;
    isData = 0;
    nPUInfo = -987987;
    nPU = -987987;
    puBX = -987987;
    puTrue = -987987;
    clearEntryEle();
    clearEntryPho();
    clearEntryMu();
    clearEntryGen();
}

void ggHiFlat::clearEntryEle()
{
    if (doEle) {
        //nEle = 0;
        eleCharge = -987987;
        eleChargeConsistent = -987987;
        eleSCPixCharge = -987987;
        eleCtfCharge = -987987;
        eleEn = -987987;
        eleD0 = -987987;
        eleDz = -987987;
        eleIP3D = -987987;
        eleD0Err = -987987;
        eleDzErr = -987987;
        eleIP3DErr = -987987;
        eleTrkPt = -987987;
        eleTrkEta = -987987;
        eleTrkPhi = -987987;
        eleTrkCharge = -987987;
        eleTrkChi2 = -987987;
        eleTrkNdof = -987987;
        eleTrkNormalizedChi2 = -987987;
        eleTrkValidHits = -987987;
        eleTrkLayers = -987987;
        elePt = -987987;
        eleEta = -987987;
        elePhi = -987987;
        eleSCEn = -987987;
        eleESEn = -987987;
        eleSCEta = -987987;
        eleSCPhi = -987987;
        eleSCRawEn = -987987;
        eleSCEtaWidth = -987987;
        eleSCPhiWidth = -987987;
        eleHoverE = -987987;
        eleHoverEBc = -987987;
        eleEoverP = -987987;
        eleEoverPInv = -987987;
        eleBrem = -987987;
        eledEtaAtVtx = -987987;
        eledPhiAtVtx = -987987;
        eledEtaSeedAtVtx = -987987;
        eleSigmaIEtaIEta = -987987;
        eleSigmaIEtaIEta_2012 = -987987;
        eleSigmaIPhiIPhi = -987987;
        eleMissHits = -987987;
        eleESEffSigmaRR = -987987;
        elePFChIso = -987987;
        elePFPhoIso = -987987;
        elePFNeuIso = -987987;
        elePFPUIso = -987987;
        elePFChIso03 = -987987;
        elePFPhoIso03 = -987987;
        elePFNeuIso03 = -987987;
        elePFChIso04 = -987987;
        elePFPhoIso04 = -987987;
        elePFNeuIso04 = -987987;
        eleR9 = -987987;
        eleE3x3 = -987987;
        eleE5x5 = -987987;
        eleR9Full5x5 = -987987;
        eleE3x3Full5x5 = -987987;
        eleE5x5Full5x5 = -987987;
        NClusters = -987987;
        NEcalClusters = -987987;
        eleSeedEn = -987987;
        eleSeedEta = -987987;
        eleSeedPhi = -987987;
        eleSeedCryEta = -987987;
        eleSeedCryPhi = -987987;
        eleSeedCryIeta = -987987;
        eleSeedCryIphi = -987987;
        eleBC1E = -987987;
        eleBC1Eta = -987987;
        eleBC2E = -987987;
        eleBC2Eta = -987987;
        eleIDVeto = -987987;
        eleIDLoose = -987987;
        eleIDMedium = -987987;
        eleIDTight = -987987;
        elepassConversionVeto = -987987;
        elePFRelIsoWithEA = -987987;
        eleEffAreaTimesRho = -987987;
    }
}

void ggHiFlat::clearEntryPho()
{
    if (doPho) {
        //nPho = 0;
        phoE = -987987;
        phoEt = -987987;
        phoEta = -987987;
        phoPhi = -987987;
        phoSCE = -987987;
        phoSCRawE = -987987;
        phoESEn = -987987;
        phoSCEta = -987987;
        phoSCPhi = -987987;
        phoSCEtaWidth = -987987;
        phoSCPhiWidth = -987987;
        phoSCBrem = -987987;

        phoESEffSigmaRR = -987987;

        phoHasPixelSeed = -987987;
        phoHasConversionTracks = -987987;
        phoHadTowerOverEm = -987987;
        phoHoverE = -987987;
        phoHoverEValid = -987987;
        phoSigmaIEtaIEta = -987987;
        phoR9 = -987987;
        phoE1x5 = -987987;
        phoE2x5 = -987987;
        phoE3x3 = -987987;
        phoE5x5 = -987987;
        phoMaxEnergyXtal = -987987;
        phoSigmaEtaEta = -987987;
        phoSigmaIEtaIEta_2012 = -987987;
        phoR9_2012 = -987987;
        phoE1x5_2012 = -987987;
        phoE2x5_2012 = -987987;
        phoE3x3_2012 = -987987;
        phoE5x5_2012 = -987987;
        phoMaxEnergyXtal_2012 = -987987;
        phoSigmaEtaEta_2012 = -987987;

        pho_isEle = -987987;
        pho_is2015Noise = -987987;

        phoHadTowerOverEm1 = -987987;
        phoHadTowerOverEm2 = -987987;
        phoHoverE1 = -987987;
        phoHoverE2 = -987987;

        phoSigmaIEtaIPhi = -987987;
        phoSigmaIPhiIPhi = -987987;
        phoR1x5 = -987987;
        phoR2x5 = -987987;
        phoE2nd = -987987;
        phoETop = -987987;
        phoEBottom = -987987;
        phoELeft = -987987;
        phoERight = -987987;
        phoE1x3 = -987987;
        phoE2x2 = -987987;
        phoE2x5Max = -987987;
        phoE2x5Top = -987987;
        phoE2x5Bottom = -987987;
        phoE2x5Left = -987987;
        phoE2x5Right = -987987;
        //b_phoSMMajor = -987987;   // TODO: enable when they become available in future releases
        //b_phoSMMinor = -987987;   // TODO: enable when they become available in future releases
        //b_phoSMAlpha = -987987;   // TODO: enable when they become available in future releases

        phoSigmaIEtaIPhi_2012 = -987987;
        phoSigmaIPhiIPhi_2012 = -987987;
        phoR1x5_2012 = -987987;
        phoR2x5_2012 = -987987;
        phoE2nd_2012 = -987987;
        phoETop_2012 = -987987;
        phoEBottom_2012 = -987987;
        phoELeft_2012 = -987987;
        phoERight_2012 = -987987;
        phoE1x3_2012 = -987987;
        phoE2x2_2012 = -987987;
        phoE2x5Max_2012 = -987987;
        phoE2x5Top_2012 = -987987;
        phoE2x5Bottom_2012 = -987987;
        phoE2x5Left_2012 = -987987;
        phoE2x5Right_2012 = -987987;
        //b_phoSMMajor_2012 = -987987;   // TODO: enable when they become available in future releases
        //b_phoSMMinor_2012 = -987987;   // TODO: enable when they become available in future releases
        //b_phoSMAlpha_2012 = -987987;   // TODO: enable when they become available in future releases

        phoBC1E = -987987;
        phoBC1Eta = -987987;
        //phoBC2E = -987987;
        //phoBC2Eta = -987987;
        pho_ecalClusterIsoR2 = -987987;
        pho_ecalClusterIsoR3 = -987987;
        pho_ecalClusterIsoR4 = -987987;
        pho_ecalClusterIsoR5 = -987987;
        pho_hcalRechitIsoR1 = -987987;
        pho_hcalRechitIsoR2 = -987987;
        pho_hcalRechitIsoR3 = -987987;
        pho_hcalRechitIsoR4 = -987987;
        pho_hcalRechitIsoR5 = -987987;
        pho_trackIsoR1PtCut20 = -987987;
        pho_trackIsoR2PtCut20 = -987987;
        pho_trackIsoR3PtCut20 = -987987;
        pho_trackIsoR4PtCut20 = -987987;
        pho_trackIsoR5PtCut20 = -987987;
        pho_swissCrx = -987987;
        pho_seedTime = -987987;
        pho_genMatchedIndex = -987987;
        pfcIso1 = -987987;
        pfcIso2 = -987987;
        pfcIso3 = -987987;
        pfcIso4 = -987987;
        pfcIso5 = -987987;
        pfpIso1 = -987987;
        pfpIso2 = -987987;
        pfpIso3 = -987987;
        pfpIso4 = -987987;
        pfpIso5 = -987987;
        pfnIso1 = -987987;
        pfnIso2 = -987987;
        pfnIso3 = -987987;
        pfnIso4 = -987987;
        pfnIso5 = -987987;
        pfpIso1subSC = -987987;
        pfpIso2subSC = -987987;
        pfpIso3subSC = -987987;
        pfpIso4subSC = -987987;
        pfpIso5subSC = -987987;
        pfcIso1subUE = -987987;
        pfcIso2subUE = -987987;
        pfcIso3subUE = -987987;
        pfcIso4subUE = -987987;
        pfcIso5subUE = -987987;
        pfpIso1subUE = -987987;
        pfpIso2subUE = -987987;
        pfpIso3subUE = -987987;
        pfpIso4subUE = -987987;
        pfpIso5subUE = -987987;
        pfnIso1subUE = -987987;
        pfnIso2subUE = -987987;
        pfnIso3subUE = -987987;
        pfnIso4subUE = -987987;
        pfnIso5subUE = -987987;
        pfpIso1subSCsubUE = -987987;
        pfpIso2subSCsubUE = -987987;
        pfpIso3subSCsubUE = -987987;
        pfpIso4subSCsubUE = -987987;
        pfpIso5subSCsubUE = -987987;
        pfcIso1pTgt1p0subUE = -987987;
        pfcIso2pTgt1p0subUE = -987987;
        pfcIso3pTgt1p0subUE = -987987;
        pfcIso4pTgt1p0subUE = -987987;
        pfcIso5pTgt1p0subUE = -987987;
        pfcIso1pTgt2p0subUE = -987987;
        pfcIso2pTgt2p0subUE = -987987;
        pfcIso3pTgt2p0subUE = -987987;
        pfcIso4pTgt2p0subUE = -987987;
        pfcIso5pTgt2p0subUE = -987987;
        pfcIso1pTgt3p0subUE = -987987;
        pfcIso2pTgt3p0subUE = -987987;
        pfcIso3pTgt3p0subUE = -987987;
        pfcIso4pTgt3p0subUE = -987987;
        pfcIso5pTgt3p0subUE = -987987;
        pfcIso2subUEec = -987987;
        pfcIso3subUEec = -987987;
        pfcIso4subUEec = -987987;
        pfpIso2subUEec = -987987;
        pfpIso3subUEec = -987987;
        pfpIso4subUEec = -987987;
        pfnIso2subUEec = -987987;
        pfnIso3subUEec = -987987;
        pfnIso4subUEec = -987987;
        pfcIso2pTgt2p0subUEec = -987987;
        pfcIso3pTgt2p0subUEec = -987987;
        pfcIso4pTgt2p0subUEec = -987987;
        trkIso3 = -987987;
        trkIso3subUE = -987987;
        trkIso3subUEec = -987987;
        trkIso3ID = -987987;
        trkIso3IDsubUE = -987987;
        trkIso3IDsubUEec = -987987;
        phoEAc = 0;
        phoEAp = 0;
        phoEAn = 0;
        phoEAhoe = 0;
        phoEAsieie = 0;
    }
}

void ggHiFlat::clearEntryMu()
{
    if (doMu) {
        //nMu = 0;
        muPt = -987987;
        muEta = -987987;
        muPhi = -987987;
        muCharge = -987987;
        muType = -987987;
        muIsGood = -987987;
        muIsGlobal = -987987;
        muIsTracker = -987987;
        muIsPF = -987987;
        muIsSTA = -987987;
        muD0 = -987987;
        muDz = -987987;
        muChi2NDF = -987987;
        muInnerD0 = -987987;
        muInnerDz = -987987;
        muInnerD0Err = -987987;
        muInnerDzErr = -987987;
        muInnerPt = -987987;
        muInnerPtErr = -987987;
        muInnerEta = -987987;
        muTrkLayers = -987987;
        muPixelLayers = -987987;
        muPixelHits = -987987;
        muMuonHits = -987987;
        muTrkQuality = -987987;
        muStations = -987987;
        muIsoTrk = -987987;
        muPFChIso = -987987;
        muPFPhoIso = -987987;
        muPFNeuIso = -987987;
        muPFPUIso = -987987;
    }
}

void ggHiFlat::clearEntryGen()
{
    if (doMC) {
        //nMC = 0;
        mcPID = -987987;
        mcStatus = -987987;
        mcVtx_x = -987987;
        mcVtx_y = -987987;
        mcVtx_z = -987987;
        mcPt = -987987;
        mcEta = -987987;
        mcPhi = -987987;
        mcE = -987987;
        mcEt = -987987;
        mcMass = -987987;
        mcParentage = -987987;
        mcMomPID = -987987;
        mcMomPt = -987987;
        mcMomEta = -987987;
        mcMomPhi = -987987;
        mcMomMass = -987987;
        mcGMomPID = -987987;
        mcIndex = -987987;
        mcCalIsoDR03 = -987987;
        mcCalIsoDR04 = -987987;
        mcTrkIsoDR03 = -987987;
        mcTrkIsoDR04 = -987987;
    }
}

void ggHiFlat::copyEle(ggHiNtuplizer &tggHiNtuplizer, int i)
{
    //nEle = 1;

    eleCharge = (*tggHiNtuplizer.eleCharge)[i];
    eleChargeConsistent = (*tggHiNtuplizer.eleChargeConsistent)[i];
    eleSCPixCharge = (*tggHiNtuplizer.eleSCPixCharge)[i];
    eleCtfCharge = (*tggHiNtuplizer.eleCtfCharge)[i];
    eleEn = (*tggHiNtuplizer.eleEn)[i];
    eleD0 = (*tggHiNtuplizer.eleD0)[i];
    eleDz = (*tggHiNtuplizer.eleDz)[i];
    if (tggHiNtuplizer.b_eleIP3D != 0) {
        eleIP3D = (*tggHiNtuplizer.eleIP3D)[i];
        eleIP3DErr = (*tggHiNtuplizer.eleIP3DErr)[i];
    }
    eleD0Err = (*tggHiNtuplizer.eleD0Err)[i];
    eleDzErr = (*tggHiNtuplizer.eleDzErr)[i];
    eleTrkPt = (*tggHiNtuplizer.eleTrkPt)[i];
    eleTrkEta = (*tggHiNtuplizer.eleTrkEta)[i];
    eleTrkPhi = (*tggHiNtuplizer.eleTrkPhi)[i];
    eleTrkCharge = (*tggHiNtuplizer.eleTrkCharge)[i];
    eleTrkChi2 = (*tggHiNtuplizer.eleTrkChi2)[i];
    eleTrkNdof = (*tggHiNtuplizer.eleTrkNdof)[i];
    eleTrkNormalizedChi2 = (*tggHiNtuplizer.eleTrkNormalizedChi2)[i];
    eleTrkValidHits = (*tggHiNtuplizer.eleTrkValidHits)[i];
    eleTrkLayers = (*tggHiNtuplizer.eleTrkLayers)[i];
    elePt = (*tggHiNtuplizer.elePt)[i];
    eleEta = (*tggHiNtuplizer.eleEta)[i];
    elePhi = (*tggHiNtuplizer.elePhi)[i];
    eleSCEn = (*tggHiNtuplizer.eleSCEn)[i];
    eleESEn = (*tggHiNtuplizer.eleESEn)[i];
    eleSCEta = (*tggHiNtuplizer.eleSCEta)[i];
    eleSCPhi = (*tggHiNtuplizer.eleSCPhi)[i];
    eleSCRawEn = (*tggHiNtuplizer.eleSCRawEn)[i];
    eleSCEtaWidth = (*tggHiNtuplizer.eleSCEtaWidth)[i];
    eleSCPhiWidth = (*tggHiNtuplizer.eleSCPhiWidth)[i];
    eleHoverE = (*tggHiNtuplizer.eleHoverE)[i];
    eleHoverEBc = (*tggHiNtuplizer.eleHoverEBc)[i];
    eleEoverP = (*tggHiNtuplizer.eleEoverP)[i];
    eleEoverPInv = (*tggHiNtuplizer.eleEoverPInv)[i];
    eleBrem = (*tggHiNtuplizer.eleBrem)[i];
    eledEtaAtVtx = (*tggHiNtuplizer.eledEtaAtVtx)[i];
    eledPhiAtVtx = (*tggHiNtuplizer.eledPhiAtVtx)[i];
    if (tggHiNtuplizer.b_eledEtaSeedAtVtx != 0) {
        eledEtaSeedAtVtx = (*tggHiNtuplizer.eledEtaSeedAtVtx)[i];
    }
    eleSigmaIEtaIEta = (*tggHiNtuplizer.eleSigmaIEtaIEta)[i];
    eleSigmaIEtaIEta_2012 = (*tggHiNtuplizer.eleSigmaIEtaIEta_2012)[i];
    eleSigmaIPhiIPhi = (*tggHiNtuplizer.eleSigmaIPhiIPhi)[i];
    eleMissHits = (*tggHiNtuplizer.eleMissHits)[i];
    if (tggHiNtuplizer.b_eleESEffSigmaRR != 0) {
        eleESEffSigmaRR = (*tggHiNtuplizer.eleESEffSigmaRR)[i];
    }
    elePFChIso = (*tggHiNtuplizer.elePFChIso)[i];
    elePFPhoIso = (*tggHiNtuplizer.elePFPhoIso)[i];
    elePFNeuIso = (*tggHiNtuplizer.elePFNeuIso)[i];
    elePFPUIso = (*tggHiNtuplizer.elePFPUIso)[i];
    elePFChIso03 = (*tggHiNtuplizer.elePFChIso03)[i];
    elePFPhoIso03 = (*tggHiNtuplizer.elePFPhoIso03)[i];
    elePFNeuIso03 = (*tggHiNtuplizer.elePFNeuIso03)[i];
    elePFChIso04 = (*tggHiNtuplizer.elePFChIso04)[i];
    elePFPhoIso04 = (*tggHiNtuplizer.elePFPhoIso04)[i];
    elePFNeuIso04 = (*tggHiNtuplizer.elePFNeuIso04)[i];
    eleR9 = (*tggHiNtuplizer.eleR9)[i];
    eleE3x3 = (*tggHiNtuplizer.eleE3x3)[i];
    eleE5x5 = (*tggHiNtuplizer.eleE5x5)[i];
    eleR9Full5x5 = (*tggHiNtuplizer.eleR9Full5x5)[i];
    eleE3x3Full5x5 = (*tggHiNtuplizer.eleE3x3Full5x5)[i];
    eleE5x5Full5x5 = (*tggHiNtuplizer.eleE5x5Full5x5)[i];
    NClusters = (*tggHiNtuplizer.NClusters)[i];
    NEcalClusters = (*tggHiNtuplizer.NEcalClusters)[i];
    eleSeedEn = (*tggHiNtuplizer.eleSeedEn)[i];
    eleSeedEta = (*tggHiNtuplizer.eleSeedEta)[i];
    eleSeedPhi = (*tggHiNtuplizer.eleSeedPhi)[i];
    eleSeedCryEta = (*tggHiNtuplizer.eleSeedCryEta)[i];
    eleSeedCryPhi = (*tggHiNtuplizer.eleSeedCryPhi)[i];
    eleSeedCryIeta = (*tggHiNtuplizer.eleSeedCryIeta)[i];
    eleSeedCryIphi = (*tggHiNtuplizer.eleSeedCryIphi)[i];
    if (tggHiNtuplizer.b_eleBC1E != 0) {
        eleBC1E = (*tggHiNtuplizer.eleBC1E)[i];
        eleBC1Eta = (*tggHiNtuplizer.eleBC1Eta)[i];
    }
    if (tggHiNtuplizer.b_eleBC2E != 0) {
        eleBC2E = (*tggHiNtuplizer.eleBC2E)[i];
        eleBC2Eta = (*tggHiNtuplizer.eleBC2Eta)[i];
    }
    if (tggHiNtuplizer.b_eleEffAreaTimesRho != 0) {
        eleIDVeto = (*tggHiNtuplizer.eleIDVeto)[i];
        eleIDLoose = (*tggHiNtuplizer.eleIDLoose)[i];
        eleIDMedium = (*tggHiNtuplizer.eleIDMedium)[i];
        eleIDTight = (*tggHiNtuplizer.eleIDTight)[i];
        elepassConversionVeto = (*tggHiNtuplizer.elepassConversionVeto)[i];
        elePFRelIsoWithEA = (*tggHiNtuplizer.elePFRelIsoWithEA)[i];
        eleEffAreaTimesRho = (*tggHiNtuplizer.eleEffAreaTimesRho)[i];
    }
}

void ggHiFlat::copyPho(ggHiNtuplizer &tggHiNtuplizer, int i)
{
    //nPho = 1;

    phoE = (*tggHiNtuplizer.phoE)[i];
    phoEt = (*tggHiNtuplizer.phoEt)[i];
    phoEta = (*tggHiNtuplizer.phoEta)[i];
    phoPhi = (*tggHiNtuplizer.phoPhi)[i];
    phoSCE = (*tggHiNtuplizer.phoSCE)[i];
    phoSCRawE = (*tggHiNtuplizer.phoSCRawE)[i];
    phoESEn = (*tggHiNtuplizer.phoESEn)[i];
    phoSCEta = (*tggHiNtuplizer.phoSCEta)[i];
    phoSCPhi = (*tggHiNtuplizer.phoSCPhi)[i];
    phoSCEtaWidth = (*tggHiNtuplizer.phoSCEtaWidth)[i];
    phoSCPhiWidth = (*tggHiNtuplizer.phoSCPhiWidth)[i];
    phoSCBrem = (*tggHiNtuplizer.phoSCBrem)[i];

    if (tggHiNtuplizer.b_phoESEffSigmaRR != 0) {
        phoESEffSigmaRR = (*tggHiNtuplizer.phoESEffSigmaRR)[i];
    }

    if (tggHiNtuplizer.b_phoHasPixelSeed != 0) {
        phoHasPixelSeed = (*tggHiNtuplizer.phoHasPixelSeed)[i];
    }
    if (tggHiNtuplizer.b_phoHasConversionTracks != 0) {
        phoHasConversionTracks = (*tggHiNtuplizer.phoHasConversionTracks)[i];
    }
    phoHadTowerOverEm = (*tggHiNtuplizer.phoHadTowerOverEm)[i];
    phoHoverE = (*tggHiNtuplizer.phoHoverE)[i];
    if (tggHiNtuplizer.b_phoHoverEValid != 0) {
        phoHoverEValid = (*tggHiNtuplizer.phoHoverEValid)[i];
    }
    phoSigmaIEtaIEta = (*tggHiNtuplizer.phoSigmaIEtaIEta)[i];
    phoR9 = (*tggHiNtuplizer.phoR9)[i];
    phoE1x5 = (*tggHiNtuplizer.phoE1x5)[i];
    phoE2x5 = (*tggHiNtuplizer.phoE2x5)[i];
    phoE3x3 = (*tggHiNtuplizer.phoE3x3)[i];
    phoE5x5 = (*tggHiNtuplizer.phoE5x5)[i];
    phoMaxEnergyXtal = (*tggHiNtuplizer.phoMaxEnergyXtal)[i];
    phoSigmaEtaEta = (*tggHiNtuplizer.phoSigmaEtaEta)[i];
    phoSigmaIEtaIEta_2012 = (*tggHiNtuplizer.phoSigmaIEtaIEta_2012)[i];
    phoR9_2012 = (*tggHiNtuplizer.phoR9_2012)[i];
    phoE1x5_2012 = (*tggHiNtuplizer.phoE1x5_2012)[i];
    phoE2x5_2012 = (*tggHiNtuplizer.phoE2x5_2012)[i];
    phoE3x3_2012 = (*tggHiNtuplizer.phoE3x3_2012)[i];
    phoE5x5_2012 = (*tggHiNtuplizer.phoE5x5_2012)[i];
    phoMaxEnergyXtal_2012 = (*tggHiNtuplizer.phoMaxEnergyXtal_2012)[i];
    phoSigmaEtaEta_2012 = (*tggHiNtuplizer.phoSigmaEtaEta_2012)[i];

    //pho_isEle = (*tggHiNtuplizer.pho_isEle)[i];
    //pho_is2015Noise = (*tggHiNtuplizer.pho_is2015Noise)[i];

    if (tggHiNtuplizer.b_phoHadTowerOverEm1 != 0) {
        phoHadTowerOverEm1 = (*tggHiNtuplizer.phoHadTowerOverEm1)[i];
        phoHadTowerOverEm2 = (*tggHiNtuplizer.phoHadTowerOverEm2)[i];
        phoHoverE1 = (*tggHiNtuplizer.phoHoverE1)[i];
        phoHoverE2 = (*tggHiNtuplizer.phoHoverE2)[i];
    }

    if (tggHiNtuplizer.b_phoSigmaIEtaIPhi != 0) {

        phoSigmaIEtaIPhi = (*tggHiNtuplizer.phoSigmaIEtaIPhi)[i];
        phoSigmaIPhiIPhi = (*tggHiNtuplizer.phoSigmaIPhiIPhi)[i];
        phoR1x5 = (*tggHiNtuplizer.phoR1x5)[i];
        phoR2x5 = (*tggHiNtuplizer.phoR2x5)[i];
        phoE2nd = (*tggHiNtuplizer.phoE2nd)[i];
        phoETop = (*tggHiNtuplizer.phoETop)[i];
        phoEBottom = (*tggHiNtuplizer.phoEBottom)[i];
        phoELeft = (*tggHiNtuplizer.phoELeft)[i];
        phoERight = (*tggHiNtuplizer.phoERight)[i];
        phoE1x3 = (*tggHiNtuplizer.phoE1x3)[i];
        phoE2x2 = (*tggHiNtuplizer.phoE2x2)[i];
        phoE2x5Max = (*tggHiNtuplizer.phoE2x5Max)[i];
        phoE2x5Top = (*tggHiNtuplizer.phoE2x5Top)[i];
        phoE2x5Bottom = (*tggHiNtuplizer.phoE2x5Bottom)[i];
        phoE2x5Left = (*tggHiNtuplizer.phoE2x5Left)[i];
        phoE2x5Right = (*tggHiNtuplizer.phoE2x5Right)[i];
        //b_phoSMMajor = (*tggHiNtuplizer.b_phoSMMajor)[i];
        //b_phoSMMinor = (*tggHiNtuplizer.b_phoSMMinor)[i];
        //b_phoSMAlpha = (*tggHiNtuplizer.b_phoSMAlpha)[i];
    }

    if (tggHiNtuplizer.b_phoSigmaIEtaIPhi_2012 != 0) {

        phoSigmaIEtaIPhi_2012 = (*tggHiNtuplizer.phoSigmaIEtaIPhi_2012)[i];
        phoSigmaIPhiIPhi_2012 = (*tggHiNtuplizer.phoSigmaIPhiIPhi_2012)[i];
        phoR1x5_2012 = (*tggHiNtuplizer.phoR1x5_2012)[i];
        phoR2x5_2012 = (*tggHiNtuplizer.phoR2x5_2012)[i];
        phoE2nd_2012 = (*tggHiNtuplizer.phoE2nd_2012)[i];
        phoETop_2012 = (*tggHiNtuplizer.phoETop_2012)[i];
        phoEBottom_2012 = (*tggHiNtuplizer.phoEBottom_2012)[i];
        phoELeft_2012 = (*tggHiNtuplizer.phoELeft_2012)[i];
        phoERight_2012 = (*tggHiNtuplizer.phoERight_2012)[i];
        phoE1x3_2012 = (*tggHiNtuplizer.phoE1x3_2012)[i];
        phoE2x2_2012 = (*tggHiNtuplizer.phoE2x2_2012)[i];
        phoE2x5Max_2012 = (*tggHiNtuplizer.phoE2x5Max_2012)[i];
        phoE2x5Top_2012 = (*tggHiNtuplizer.phoE2x5Top_2012)[i];
        phoE2x5Bottom_2012 = (*tggHiNtuplizer.phoE2x5Bottom_2012)[i];
        phoE2x5Left_2012 = (*tggHiNtuplizer.phoE2x5Left_2012)[i];
        phoE2x5Right_2012 = (*tggHiNtuplizer.phoE2x5Right_2012)[i];
        //b_phoSMMajor_2012 = (*tggHiNtuplizer.b_phoSMMajor_2012)[i];
        //b_phoSMMinor_2012 = (*tggHiNtuplizer.b_phoSMMinor_2012)[i];
        //b_phoSMAlpha_2012 = (*tggHiNtuplizer.b_phoSMAlpha_2012)[i];
    }

    if (tggHiNtuplizer.b_phoBC1E != 0) {
        phoBC1E = (*tggHiNtuplizer.phoBC1E)[i];
        phoBC1Eta = (*tggHiNtuplizer.phoBC1Eta)[i];
    }
    /*
    if (tggHiNtuplizer.phoBC2E->size() > 0) {
        phoBC2E = (*tggHiNtuplizer.phoBC2E)[i];
        phoBC2Eta = (*tggHiNtuplizer.phoBC2Eta)[i];
    }
    */
    pho_ecalClusterIsoR2 = (*tggHiNtuplizer.pho_ecalClusterIsoR2)[i];
    pho_ecalClusterIsoR3 = (*tggHiNtuplizer.pho_ecalClusterIsoR3)[i];
    pho_ecalClusterIsoR4 = (*tggHiNtuplizer.pho_ecalClusterIsoR4)[i];
    pho_ecalClusterIsoR5 = (*tggHiNtuplizer.pho_ecalClusterIsoR5)[i];
    pho_hcalRechitIsoR1 = (*tggHiNtuplizer.pho_hcalRechitIsoR1)[i];
    pho_hcalRechitIsoR2 = (*tggHiNtuplizer.pho_hcalRechitIsoR2)[i];
    pho_hcalRechitIsoR3 = (*tggHiNtuplizer.pho_hcalRechitIsoR3)[i];
    pho_hcalRechitIsoR4 = (*tggHiNtuplizer.pho_hcalRechitIsoR4)[i];
    pho_hcalRechitIsoR5 = (*tggHiNtuplizer.pho_hcalRechitIsoR5)[i];
    pho_trackIsoR1PtCut20 = (*tggHiNtuplizer.pho_trackIsoR1PtCut20)[i];
    pho_trackIsoR2PtCut20 = (*tggHiNtuplizer.pho_trackIsoR2PtCut20)[i];
    pho_trackIsoR3PtCut20 = (*tggHiNtuplizer.pho_trackIsoR3PtCut20)[i];
    pho_trackIsoR4PtCut20 = (*tggHiNtuplizer.pho_trackIsoR4PtCut20)[i];
    pho_trackIsoR5PtCut20 = (*tggHiNtuplizer.pho_trackIsoR5PtCut20)[i];
    pho_swissCrx = (*tggHiNtuplizer.pho_swissCrx)[i];
    pho_seedTime = (*tggHiNtuplizer.pho_seedTime)[i];
    pho_genMatchedIndex = (*tggHiNtuplizer.pho_genMatchedIndex)[i];

    if (tggHiNtuplizer.b_pfcIso1 != 0) {
        pfcIso1 = (*tggHiNtuplizer.pfcIso1)[i];
        pfcIso2 = (*tggHiNtuplizer.pfcIso2)[i];
        pfcIso3 = (*tggHiNtuplizer.pfcIso3)[i];
        pfcIso4 = (*tggHiNtuplizer.pfcIso4)[i];
        pfcIso5 = (*tggHiNtuplizer.pfcIso5)[i];
        pfpIso1 = (*tggHiNtuplizer.pfpIso1)[i];
        pfpIso2 = (*tggHiNtuplizer.pfpIso2)[i];
        pfpIso3 = (*tggHiNtuplizer.pfpIso3)[i];
        pfpIso4 = (*tggHiNtuplizer.pfpIso4)[i];
        pfpIso5 = (*tggHiNtuplizer.pfpIso5)[i];
        pfnIso1 = (*tggHiNtuplizer.pfnIso1)[i];
        pfnIso2 = (*tggHiNtuplizer.pfnIso2)[i];
        pfnIso3 = (*tggHiNtuplizer.pfnIso3)[i];
        pfnIso4 = (*tggHiNtuplizer.pfnIso4)[i];
        pfnIso5 = (*tggHiNtuplizer.pfnIso5)[i];
    }
    if (tggHiNtuplizer.b_pfpIso1subSC != 0) {
        pfpIso1subSC = (*tggHiNtuplizer.pfpIso1subSC)[i];
        pfpIso2subSC = (*tggHiNtuplizer.pfpIso2subSC)[i];
        pfpIso3subSC = (*tggHiNtuplizer.pfpIso3subSC)[i];
        pfpIso4subSC = (*tggHiNtuplizer.pfpIso4subSC)[i];
        pfpIso5subSC = (*tggHiNtuplizer.pfpIso5subSC)[i];
    }
    if (tggHiNtuplizer.b_pfcIso1subUE != 0) {
        pfcIso1subUE = (*tggHiNtuplizer.pfcIso1subUE)[i];
        pfcIso2subUE = (*tggHiNtuplizer.pfcIso2subUE)[i];
        pfcIso3subUE = (*tggHiNtuplizer.pfcIso3subUE)[i];
        pfcIso4subUE = (*tggHiNtuplizer.pfcIso4subUE)[i];
        pfcIso5subUE = (*tggHiNtuplizer.pfcIso5subUE)[i];
        pfpIso1subUE = (*tggHiNtuplizer.pfpIso1subUE)[i];
        pfpIso2subUE = (*tggHiNtuplizer.pfpIso2subUE)[i];
        pfpIso3subUE = (*tggHiNtuplizer.pfpIso3subUE)[i];
        pfpIso4subUE = (*tggHiNtuplizer.pfpIso4subUE)[i];
        pfpIso5subUE = (*tggHiNtuplizer.pfpIso5subUE)[i];
        pfnIso1subUE = (*tggHiNtuplizer.pfnIso1subUE)[i];
        pfnIso2subUE = (*tggHiNtuplizer.pfnIso2subUE)[i];
        pfnIso3subUE = (*tggHiNtuplizer.pfnIso3subUE)[i];
        pfnIso4subUE = (*tggHiNtuplizer.pfnIso4subUE)[i];
        pfnIso5subUE = (*tggHiNtuplizer.pfnIso5subUE)[i];
    }
    if (tggHiNtuplizer.b_pfpIso1subSCsubUE != 0) {
        pfpIso1subSCsubUE = (*tggHiNtuplizer.pfpIso1subSCsubUE)[i];
        pfpIso2subSCsubUE = (*tggHiNtuplizer.pfpIso2subSCsubUE)[i];
        pfpIso3subSCsubUE = (*tggHiNtuplizer.pfpIso3subSCsubUE)[i];
        pfpIso4subSCsubUE = (*tggHiNtuplizer.pfpIso4subSCsubUE)[i];
        pfpIso5subSCsubUE = (*tggHiNtuplizer.pfpIso5subSCsubUE)[i];
    }
    if (tggHiNtuplizer.b_pfcIso1pTgt1p0subUE != 0) {
        pfcIso1pTgt1p0subUE = (*tggHiNtuplizer.pfcIso1pTgt1p0subUE)[i];
        pfcIso2pTgt1p0subUE = (*tggHiNtuplizer.pfcIso2pTgt1p0subUE)[i];
        pfcIso3pTgt1p0subUE = (*tggHiNtuplizer.pfcIso3pTgt1p0subUE)[i];
        pfcIso4pTgt1p0subUE = (*tggHiNtuplizer.pfcIso4pTgt1p0subUE)[i];
        pfcIso5pTgt1p0subUE = (*tggHiNtuplizer.pfcIso5pTgt1p0subUE)[i];
        pfcIso1pTgt2p0subUE = (*tggHiNtuplizer.pfcIso1pTgt2p0subUE)[i];
        pfcIso2pTgt2p0subUE = (*tggHiNtuplizer.pfcIso2pTgt2p0subUE)[i];
        pfcIso3pTgt2p0subUE = (*tggHiNtuplizer.pfcIso3pTgt2p0subUE)[i];
        pfcIso4pTgt2p0subUE = (*tggHiNtuplizer.pfcIso4pTgt2p0subUE)[i];
        pfcIso5pTgt2p0subUE = (*tggHiNtuplizer.pfcIso5pTgt2p0subUE)[i];
        pfcIso1pTgt3p0subUE = (*tggHiNtuplizer.pfcIso1pTgt3p0subUE)[i];
        pfcIso2pTgt3p0subUE = (*tggHiNtuplizer.pfcIso2pTgt3p0subUE)[i];
        pfcIso3pTgt3p0subUE = (*tggHiNtuplizer.pfcIso3pTgt3p0subUE)[i];
        pfcIso4pTgt3p0subUE = (*tggHiNtuplizer.pfcIso4pTgt3p0subUE)[i];
        pfcIso5pTgt3p0subUE = (*tggHiNtuplizer.pfcIso5pTgt3p0subUE)[i];
    }
    if (tggHiNtuplizer.b_pfcIso2subUEec != 0) {
        pfcIso2subUEec = (*tggHiNtuplizer.pfcIso2subUEec)[i];
        pfcIso3subUEec = (*tggHiNtuplizer.pfcIso3subUEec)[i];
        pfcIso4subUEec = (*tggHiNtuplizer.pfcIso4subUEec)[i];
        pfpIso2subUEec = (*tggHiNtuplizer.pfpIso2subUEec)[i];
        pfpIso3subUEec = (*tggHiNtuplizer.pfpIso3subUEec)[i];
        pfpIso4subUEec = (*tggHiNtuplizer.pfpIso4subUEec)[i];
        pfnIso2subUEec = (*tggHiNtuplizer.pfnIso2subUEec)[i];
        pfnIso3subUEec = (*tggHiNtuplizer.pfnIso3subUEec)[i];
        pfnIso4subUEec = (*tggHiNtuplizer.pfnIso4subUEec)[i];
        pfcIso2pTgt2p0subUEec = (*tggHiNtuplizer.pfcIso2pTgt2p0subUEec)[i];
        pfcIso3pTgt2p0subUEec = (*tggHiNtuplizer.pfcIso3pTgt2p0subUEec)[i];
        pfcIso4pTgt2p0subUEec = (*tggHiNtuplizer.pfcIso4pTgt2p0subUEec)[i];
    }
}

void ggHiFlat::copyMu(ggHiNtuplizer &tggHiNtuplizer, int i)
{
    //nMC = 1;

    muPt = (*tggHiNtuplizer.muPt)[i];
    muEta = (*tggHiNtuplizer.muEta)[i];
    muPhi = (*tggHiNtuplizer.muPhi)[i];
    muCharge = (*tggHiNtuplizer.muCharge)[i];
    muType = (*tggHiNtuplizer.muType)[i];
    muIsGood = (*tggHiNtuplizer.muIsGood)[i];
    muIsGlobal = (*tggHiNtuplizer.muIsGlobal)[i];
    muIsTracker = (*tggHiNtuplizer.muIsTracker)[i];
    muIsPF = (*tggHiNtuplizer.muIsPF)[i];
    muIsSTA = (*tggHiNtuplizer.muIsSTA)[i];
    muD0 = (*tggHiNtuplizer.muD0)[i];
    muDz = (*tggHiNtuplizer.muDz)[i];
    muChi2NDF = (*tggHiNtuplizer.muChi2NDF)[i];
    muInnerD0 = (*tggHiNtuplizer.muInnerD0)[i];
    muInnerDz = (*tggHiNtuplizer.muInnerDz)[i];
    muInnerD0Err = (*tggHiNtuplizer.muInnerD0Err)[i];
    muInnerDzErr = (*tggHiNtuplizer.muInnerDzErr)[i];
    muInnerPt = (*tggHiNtuplizer.muInnerPt)[i];
    muInnerPtErr = (*tggHiNtuplizer.muInnerPtErr)[i];
    muInnerEta = (*tggHiNtuplizer.muInnerEta)[i];
    muTrkLayers = (*tggHiNtuplizer.muTrkLayers)[i];
    muPixelLayers = (*tggHiNtuplizer.muPixelLayers)[i];
    muPixelHits = (*tggHiNtuplizer.muPixelHits)[i];
    muMuonHits = (*tggHiNtuplizer.muMuonHits)[i];
    muTrkQuality = (*tggHiNtuplizer.muTrkQuality)[i];
    muStations = (*tggHiNtuplizer.muStations)[i];
    muIsoTrk = (*tggHiNtuplizer.muIsoTrk)[i];
    muPFChIso = (*tggHiNtuplizer.muPFChIso)[i];
    muPFPhoIso = (*tggHiNtuplizer.muPFPhoIso)[i];
    muPFNeuIso = (*tggHiNtuplizer.muPFNeuIso)[i];
    muPFPUIso = (*tggHiNtuplizer.muPFPUIso)[i];
}

void ggHiFlat::copyGen(ggHiNtuplizer &tggHiNtuplizer, int i)
{
    //nMC = 1;

    mcPID = (*tggHiNtuplizer.mcPID)[i];
    mcStatus = (*tggHiNtuplizer.mcStatus)[i];
    mcVtx_x = (*tggHiNtuplizer.mcVtx_x)[i];
    mcVtx_y = (*tggHiNtuplizer.mcVtx_y)[i];
    mcVtx_z = (*tggHiNtuplizer.mcVtx_z)[i];
    mcPt = (*tggHiNtuplizer.mcPt)[i];
    mcEta = (*tggHiNtuplizer.mcEta)[i];
    mcPhi = (*tggHiNtuplizer.mcPhi)[i];
    mcE = (*tggHiNtuplizer.mcE)[i];
    mcEt = (*tggHiNtuplizer.mcEt)[i];
    mcMass = (*tggHiNtuplizer.mcMass)[i];
    mcParentage = (*tggHiNtuplizer.mcParentage)[i];
    mcMomPID = (*tggHiNtuplizer.mcMomPID)[i];
    mcMomPt = (*tggHiNtuplizer.mcMomPt)[i];
    mcMomEta = (*tggHiNtuplizer.mcMomEta)[i];
    mcMomPhi = (*tggHiNtuplizer.mcMomPhi)[i];
    mcMomMass = (*tggHiNtuplizer.mcMomMass)[i];
    mcGMomPID = (*tggHiNtuplizer.mcGMomPID)[i];
    mcIndex = (*tggHiNtuplizer.mcIndex)[i];
    mcCalIsoDR03 = (*tggHiNtuplizer.mcCalIsoDR03)[i];
    mcCalIsoDR04 = (*tggHiNtuplizer.mcCalIsoDR04)[i];
    mcTrkIsoDR03 = (*tggHiNtuplizer.mcTrkIsoDR03)[i];
    mcTrkIsoDR04 = (*tggHiNtuplizer.mcTrkIsoDR04)[i];
}

void ggHiFlat::clone(ggHiFlat &gg)
{
    doEle = gg.doEle;
    doPho = gg.doPho;
    doMu = gg.doMu;
    doMC = gg.doMC;

    weight = gg.weight;
    weightCent = gg.weightCent;
    weightKin = gg.weightKin;
    weightPthat = gg.weightPthat;
    pthat = gg.pthat;
    hiBin = gg.hiBin;
    hiHF = gg.hiHF;
    rho = gg.rho;
    run = gg.run;
    event = gg.event;
    lumis = gg.lumis;
    isData = gg.isData;
    nPUInfo = gg.nPUInfo;
    nPU = gg.nPU;
    puBX = gg.puBX;
    puTrue = gg.puTrue;
    //nMC = gg.nMC;
    mcPID = gg.mcPID;
    mcStatus = gg.mcStatus;
    mcVtx_x = gg.mcVtx_x;
    mcVtx_y = gg.mcVtx_y;
    mcVtx_z = gg.mcVtx_z;
    mcPt = gg.mcPt;
    mcEta = gg.mcEta;
    mcPhi = gg.mcPhi;
    mcE = gg.mcE;
    mcEt = gg.mcEt;
    mcMass = gg.mcMass;
    mcParentage = gg.mcParentage;
    mcMomPID = gg.mcMomPID;
    mcMomPt = gg.mcMomPt;
    mcMomEta = gg.mcMomEta;
    mcMomPhi = gg.mcMomPhi;
    mcMomMass = gg.mcMomMass;
    mcGMomPID = gg.mcGMomPID;
    mcIndex = gg.mcIndex;
    mcCalIsoDR03 = gg.mcCalIsoDR03;
    mcCalIsoDR04 = gg.mcCalIsoDR04;
    mcTrkIsoDR03 = gg.mcTrkIsoDR03;
    mcTrkIsoDR04 = gg.mcTrkIsoDR04;
    //nEle = gg.nEle;
    eleCharge = gg.eleCharge;
    eleChargeConsistent = gg.eleChargeConsistent;
    eleSCPixCharge = gg.eleSCPixCharge;
    eleCtfCharge = gg.eleCtfCharge;
    eleEn = gg.eleEn;
    eleD0 = gg.eleD0;
    eleDz = gg.eleDz;
    eleIP3D = gg.eleIP3D;
    eleD0Err = gg.eleD0Err;
    eleDzErr = gg.eleDzErr;
    eleIP3DErr = gg.eleIP3DErr;
    eleTrkPt = gg.eleTrkPt;
    eleTrkEta = gg.eleTrkEta;
    eleTrkPhi = gg.eleTrkPhi;
    eleTrkCharge = gg.eleTrkCharge;
    eleTrkChi2 = gg.eleTrkChi2;
    eleTrkNdof = gg.eleTrkNdof;
    eleTrkNormalizedChi2 = gg.eleTrkNormalizedChi2;
    eleTrkValidHits = gg.eleTrkValidHits;
    eleTrkLayers = gg.eleTrkLayers;
    elePt = gg.elePt;
    eleEta = gg.eleEta;
    elePhi = gg.elePhi;
    eleSCEn = gg.eleSCEn;
    eleESEn = gg.eleESEn;
    eleSCEta = gg.eleSCEta;
    eleSCPhi = gg.eleSCPhi;
    eleSCRawEn = gg.eleSCRawEn;
    eleSCEtaWidth = gg.eleSCEtaWidth;
    eleSCPhiWidth = gg.eleSCPhiWidth;
    eleHoverE = gg.eleHoverE;
    eleHoverEBc = gg.eleHoverEBc;
    eleEoverP = gg.eleEoverP;
    eleEoverPInv = gg.eleEoverPInv;
    eleBrem = gg.eleBrem;
    eledEtaAtVtx = gg.eledEtaAtVtx;
    eledPhiAtVtx = gg.eledPhiAtVtx;
    eledEtaSeedAtVtx = gg.eledEtaSeedAtVtx;
    eleSigmaIEtaIEta = gg.eleSigmaIEtaIEta;
    eleSigmaIEtaIEta_2012 = gg.eleSigmaIEtaIEta_2012;
    eleSigmaIPhiIPhi = gg.eleSigmaIPhiIPhi;
    eleMissHits = gg.eleMissHits;
    eleESEffSigmaRR = gg.eleESEffSigmaRR;
    elePFChIso = gg.elePFChIso;
    elePFPhoIso = gg.elePFPhoIso;
    elePFNeuIso = gg.elePFNeuIso;
    elePFPUIso = gg.elePFPUIso;
    elePFChIso03 = gg.elePFChIso03;
    elePFPhoIso03 = gg.elePFPhoIso03;
    elePFNeuIso03 = gg.elePFNeuIso03;
    elePFChIso04 = gg.elePFChIso04;
    elePFPhoIso04 = gg.elePFPhoIso04;
    elePFNeuIso04 = gg.elePFNeuIso04;
    eleR9 = gg.eleR9;
    eleE3x3 = gg.eleE3x3;
    eleE5x5 = gg.eleE5x5;
    eleR9Full5x5 = gg.eleR9Full5x5;
    eleE3x3Full5x5 = gg.eleE3x3Full5x5;
    eleE5x5Full5x5 = gg.eleE5x5Full5x5;
    NClusters = gg.NClusters;
    NEcalClusters = gg.NEcalClusters;
    eleSeedEn = gg.eleSeedEn;
    eleSeedEta = gg.eleSeedEta;
    eleSeedPhi = gg.eleSeedPhi;
    eleSeedCryEta = gg.eleSeedCryEta;
    eleSeedCryPhi = gg.eleSeedCryPhi;
    eleSeedCryIeta = gg.eleSeedCryIeta;
    eleSeedCryIphi = gg.eleSeedCryIphi;
    eleBC1E = gg.eleBC1E;
    eleBC1Eta = gg.eleBC1Eta;
    eleBC2E = gg.eleBC2E;
    eleBC2Eta = gg.eleBC2Eta;
    eleIDVeto = gg.eleIDVeto;
    eleIDLoose = gg.eleIDLoose;
    eleIDMedium = gg.eleIDMedium;
    eleIDTight = gg.eleIDTight;
    elepassConversionVeto = gg.elepassConversionVeto;
    elePFRelIsoWithEA = gg.elePFRelIsoWithEA;
    eleEffAreaTimesRho = gg.eleEffAreaTimesRho;
    //nPho = gg.nPho;
    phoE = gg.phoE;
    phoEt = gg.phoEt;
    phoEta = gg.phoEta;
    phoPhi = gg.phoPhi;
    phoSCE = gg.phoSCE;
    phoSCRawE = gg.phoSCRawE;
    phoESEn = gg.phoESEn;
    phoSCEta = gg.phoSCEta;
    phoSCPhi = gg.phoSCPhi;
    phoSCEtaWidth = gg.phoSCEtaWidth;
    phoSCPhiWidth = gg.phoSCPhiWidth;
    phoSCBrem = gg.phoSCBrem;

    phoESEffSigmaRR = gg.phoESEffSigmaRR;

    phoHasPixelSeed = gg.phoHasPixelSeed;
    phoHasConversionTracks = gg.phoHasConversionTracks;
    phoHadTowerOverEm = gg.phoHadTowerOverEm;
    phoHoverE = gg.phoHoverE;
    phoHoverEValid = gg.phoHoverEValid;
    phoSigmaIEtaIEta = gg.phoSigmaIEtaIEta;
    phoR9 = gg.phoR9;
    phoE1x5 = gg.phoE1x5;
    phoE2x5 = gg.phoE2x5;
    phoE3x3 = gg.phoE3x3;
    phoE5x5 = gg.phoE5x5;
    phoMaxEnergyXtal = gg.phoMaxEnergyXtal;
    phoSigmaEtaEta = gg.phoSigmaEtaEta;
    phoSigmaIEtaIEta_2012 = gg.phoSigmaIEtaIEta_2012;
    phoR9_2012 = gg.phoR9_2012;
    phoE1x5_2012 = gg.phoE1x5_2012;
    phoE2x5_2012 = gg.phoE2x5_2012;
    phoE3x3_2012 = gg.phoE3x3_2012;
    phoE5x5_2012 = gg.phoE5x5_2012;
    phoMaxEnergyXtal_2012 = gg.phoMaxEnergyXtal_2012;
    phoSigmaEtaEta_2012 = gg.phoSigmaEtaEta_2012;

    pho_isEle = gg.pho_isEle;
    pho_is2015Noise = gg.pho_is2015Noise;

    phoHadTowerOverEm1 = gg.phoHadTowerOverEm1;
    phoHadTowerOverEm2 = gg.phoHadTowerOverEm2;
    phoHoverE1 = gg.phoHoverE1;
    phoHoverE2 = gg.phoHoverE2;

    phoSigmaIEtaIPhi = gg.phoSigmaIEtaIPhi;
    phoSigmaIPhiIPhi = gg.phoSigmaIPhiIPhi;
    phoR1x5 = gg.phoR1x5;
    phoR2x5 = gg.phoR2x5;
    phoE2nd = gg.phoE2nd;
    phoETop = gg.phoETop;
    phoEBottom = gg.phoEBottom;
    phoELeft = gg.phoELeft;
    phoERight = gg.phoERight;
    phoE1x3 = gg.phoE1x3;
    phoE2x2 = gg.phoE2x2;
    phoE2x5Max = gg.phoE2x5Max;
    phoE2x5Top = gg.phoE2x5Top;
    phoE2x5Bottom = gg.phoE2x5Bottom;
    phoE2x5Left = gg.phoE2x5Left;
    phoE2x5Right = gg.phoE2x5Right;
    //phoSMMajor = gg.phoSMMajor;
    //phoSMMinor = gg.phoSMMinor;
    //phoSMAlpha = gg.phoSMAlpha;

    phoSigmaIEtaIPhi_2012 = gg.phoSigmaIEtaIPhi_2012;
    phoSigmaIPhiIPhi_2012 = gg.phoSigmaIPhiIPhi_2012;
    phoR1x5_2012 = gg.phoR1x5_2012;
    phoR2x5_2012 = gg.phoR2x5_2012;
    phoE2nd_2012 = gg.phoE2nd_2012;
    phoETop_2012 = gg.phoETop_2012;
    phoEBottom_2012 = gg.phoEBottom_2012;
    phoELeft_2012 = gg.phoELeft_2012;
    phoERight_2012 = gg.phoERight_2012;
    phoE1x3_2012 = gg.phoE1x3_2012;
    phoE2x2_2012 = gg.phoE2x2_2012;
    phoE2x5Max_2012 = gg.phoE2x5Max_2012;
    phoE2x5Top_2012 = gg.phoE2x5Top_2012;
    phoE2x5Bottom_2012 = gg.phoE2x5Bottom_2012;
    phoE2x5Left_2012 = gg.phoE2x5Left_2012;
    phoE2x5Right_2012 = gg.phoE2x5Right_2012;
    //phoSMMajor_2012 = gg.phoSMMajor_2012;
    //phoSMMinor_2012 = gg.phoSMMinor_2012;
    //phoSMAlpha_2012 = gg.phoSMAlpha_2012;

    phoBC1E = gg.phoBC1E;
    phoBC1Eta = gg.phoBC1Eta;
    //phoBC2E = gg.phoBC2E;
    //phoBC2Eta = gg.phoBC2Eta;
    pho_ecalClusterIsoR2 = gg.pho_ecalClusterIsoR2;
    pho_ecalClusterIsoR3 = gg.pho_ecalClusterIsoR3;
    pho_ecalClusterIsoR4 = gg.pho_ecalClusterIsoR4;
    pho_ecalClusterIsoR5 = gg.pho_ecalClusterIsoR5;
    pho_hcalRechitIsoR1 = gg.pho_hcalRechitIsoR1;
    pho_hcalRechitIsoR2 = gg.pho_hcalRechitIsoR2;
    pho_hcalRechitIsoR3 = gg.pho_hcalRechitIsoR3;
    pho_hcalRechitIsoR4 = gg.pho_hcalRechitIsoR4;
    pho_hcalRechitIsoR5 = gg.pho_hcalRechitIsoR5;
    pho_trackIsoR1PtCut20 = gg.pho_trackIsoR1PtCut20;
    pho_trackIsoR2PtCut20 = gg.pho_trackIsoR2PtCut20;
    pho_trackIsoR3PtCut20 = gg.pho_trackIsoR3PtCut20;
    pho_trackIsoR4PtCut20 = gg.pho_trackIsoR4PtCut20;
    pho_trackIsoR5PtCut20 = gg.pho_trackIsoR5PtCut20;
    pho_swissCrx = gg.pho_swissCrx;
    pho_seedTime = gg.pho_seedTime;
    pho_genMatchedIndex = gg.pho_genMatchedIndex;
    pfcIso1 = gg.pfcIso1;
    pfcIso2 = gg.pfcIso2;
    pfcIso3 = gg.pfcIso3;
    pfcIso4 = gg.pfcIso4;
    pfcIso5 = gg.pfcIso5;
    pfpIso1 = gg.pfpIso1;
    pfpIso2 = gg.pfpIso2;
    pfpIso3 = gg.pfpIso3;
    pfpIso4 = gg.pfpIso4;
    pfpIso5 = gg.pfpIso5;
    pfnIso1 = gg.pfnIso1;
    pfnIso2 = gg.pfnIso2;
    pfnIso3 = gg.pfnIso3;
    pfnIso4 = gg.pfnIso4;
    pfnIso5 = gg.pfnIso5;
    pfpIso1subSC = gg.pfpIso1subSC;
    pfpIso2subSC = gg.pfpIso2subSC;
    pfpIso3subSC = gg.pfpIso3subSC;
    pfpIso4subSC = gg.pfpIso4subSC;
    pfpIso5subSC = gg.pfpIso5subSC;
    pfcIso1subUE = gg.pfcIso1subUE;
    pfcIso2subUE = gg.pfcIso2subUE;
    pfcIso3subUE = gg.pfcIso3subUE;
    pfcIso4subUE = gg.pfcIso4subUE;
    pfcIso5subUE = gg.pfcIso5subUE;
    pfpIso1subUE = gg.pfpIso1subUE;
    pfpIso2subUE = gg.pfpIso2subUE;
    pfpIso3subUE = gg.pfpIso3subUE;
    pfpIso4subUE = gg.pfpIso4subUE;
    pfpIso5subUE = gg.pfpIso5subUE;
    pfnIso1subUE = gg.pfnIso1subUE;
    pfnIso2subUE = gg.pfnIso2subUE;
    pfnIso3subUE = gg.pfnIso3subUE;
    pfnIso4subUE = gg.pfnIso4subUE;
    pfnIso5subUE = gg.pfnIso5subUE;
    pfpIso1subSCsubUE = gg.pfpIso1subSCsubUE;
    pfpIso2subSCsubUE = gg.pfpIso2subSCsubUE;
    pfpIso3subSCsubUE = gg.pfpIso3subSCsubUE;
    pfpIso4subSCsubUE = gg.pfpIso4subSCsubUE;
    pfpIso5subSCsubUE = gg.pfpIso5subSCsubUE;
    pfcIso1pTgt1p0subUE = gg.pfcIso1pTgt1p0subUE;
    pfcIso2pTgt1p0subUE = gg.pfcIso2pTgt1p0subUE;
    pfcIso3pTgt1p0subUE = gg.pfcIso3pTgt1p0subUE;
    pfcIso4pTgt1p0subUE = gg.pfcIso4pTgt1p0subUE;
    pfcIso5pTgt1p0subUE = gg.pfcIso5pTgt1p0subUE;
    pfcIso1pTgt2p0subUE = gg.pfcIso1pTgt2p0subUE;
    pfcIso2pTgt2p0subUE = gg.pfcIso2pTgt2p0subUE;
    pfcIso3pTgt2p0subUE = gg.pfcIso3pTgt2p0subUE;
    pfcIso4pTgt2p0subUE = gg.pfcIso4pTgt2p0subUE;
    pfcIso5pTgt2p0subUE = gg.pfcIso5pTgt2p0subUE;
    pfcIso1pTgt3p0subUE = gg.pfcIso1pTgt3p0subUE;
    pfcIso2pTgt3p0subUE = gg.pfcIso2pTgt3p0subUE;
    pfcIso3pTgt3p0subUE = gg.pfcIso3pTgt3p0subUE;
    pfcIso4pTgt3p0subUE = gg.pfcIso4pTgt3p0subUE;
    pfcIso5pTgt3p0subUE = gg.pfcIso5pTgt3p0subUE;

    pfcIso2subUEec = gg.pfcIso2subUEec;
    pfcIso3subUEec = gg.pfcIso3subUEec;
    pfcIso4subUEec = gg.pfcIso4subUEec;
    pfpIso2subUEec = gg.pfpIso2subUEec;
    pfpIso3subUEec = gg.pfpIso3subUEec;
    pfpIso4subUEec = gg.pfpIso4subUEec;
    pfnIso2subUEec = gg.pfnIso2subUEec;
    pfnIso3subUEec = gg.pfnIso3subUEec;
    pfnIso4subUEec = gg.pfnIso4subUEec;
    pfcIso2pTgt2p0subUEec = gg.pfcIso2pTgt2p0subUEec;
    pfcIso3pTgt2p0subUEec = gg.pfcIso3pTgt2p0subUEec;
    pfcIso4pTgt2p0subUEec = gg.pfcIso4pTgt2p0subUEec;

    trkIso3 = gg.trkIso3;
    trkIso3subUE = gg.trkIso3subUE;
    trkIso3subUEec = gg.trkIso3subUEec;
    trkIso3ID = gg.trkIso3ID;
    trkIso3IDsubUE = gg.trkIso3IDsubUE;
    trkIso3IDsubUEec = gg.trkIso3IDsubUEec;

    phoEAc = gg.phoEAc;
    phoEAp = gg.phoEAp;
    phoEAn = gg.phoEAn;
    phoEAhoe = gg.phoEAhoe;
    phoEAsieie = gg.phoEAsieie;

    //nMu = gg.nMu;
    muPt = gg.muPt;
    muEta = gg.muEta;
    muPhi = gg.muPhi;
    muCharge = gg.muCharge;
    muType = gg.muType;
    muIsGood = gg.muIsGood;
    muIsGlobal = gg.muIsGlobal;
    muIsTracker = gg.muIsTracker;
    muIsPF = gg.muIsPF;
    muIsSTA = gg.muIsSTA;
    muD0 = gg.muD0;
    muDz = gg.muDz;
    muChi2NDF = gg.muChi2NDF;
    muInnerD0 = gg.muInnerD0;
    muInnerDz = gg.muInnerDz;
    muInnerD0Err = gg.muInnerD0Err;
    muInnerDzErr = gg.muInnerDzErr;
    muInnerPt = gg.muInnerPt;
    muInnerPtErr = gg.muInnerPtErr;
    muInnerEta = gg.muInnerEta;
    muTrkLayers = gg.muTrkLayers;
    muPixelLayers = gg.muPixelLayers;
    muPixelHits = gg.muPixelHits;
    muMuonHits = gg.muMuonHits;
    muTrkQuality = gg.muTrkQuality;
    muStations = gg.muStations;
    muIsoTrk = gg.muIsoTrk;
    muPFChIso = gg.muPFChIso;
    muPFPhoIso = gg.muPFPhoIso;
    muPFNeuIso = gg.muPFNeuIso;
    muPFPUIso = gg.muPFPUIso;
    isGenMatched = gg.isGenMatched;
    genMatchedIdx = gg.genMatchedIdx;
}

#endif /* TREEHEADERS_GGHIFLATTREE_H_ */
