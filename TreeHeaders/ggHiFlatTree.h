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

  bool doEle;
  bool doPho;
  bool doMu;
  bool doMC;

  // Declaration of leaf types
  float weight;
  float weightCent;
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
  float eleD0Err;
  float eleDzErr;
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
  int phohasPixelSeed;
  float phoR9;
  float phoHoverE;
  float phoSigmaIEtaIEta;
  int pho_isEle;
  int pho_is2015Noise;
  //float phoE1x3;
  //float phoE2x2;
  //float phoE2x5Max;
  float phoE1x5;
  float phoE2x5;
  float phoE5x5;
  float phoE3x3;
  float phoMaxEnergyXtal;
  float phoSigmaEtaEta;
  float phoR1x5;
  float phoR2x5;
  float phoESEffSigmaRR;
  float phoSigmaIEtaIEta_2012;
  //float phoSigmaIEtaIPhi_2012;
  //float phoSigmaIPhiIPhi_2012;
  //float phoE1x3_2012;
  //float phoE2x2_2012;
  //float phoE2x5Max_2012;
  //float phoE5x5_2012;
  float phoE3x3_2012;
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
  float phoEAc;
  float phoEAp;
  float phoEAn;
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
  //int nMu;
  float muPt;
  float muEta;
  float muPhi;
  int muCharge;
  int muType;
  int muIsGood;
  float muD0;
  float muDz;
  float muChi2NDF;
  float muInnerD0;
  float muInnerDz;
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
  TBranch        *b_phohasPixelSeed;   //!
  TBranch        *b_phoR9;   //!
  TBranch        *b_phoHoverE;   //!
  TBranch        *b_phoSigmaIEtaIEta;   //!
  TBranch *b_pho_isEle;
  TBranch *b_pho_is2015Noise;
  //TBranch        *b_phoE1x3;   //!
  //TBranch        *b_phoE2x2;   //!
  //TBranch        *b_phoE2x5Max;   //!
  TBranch        *b_phoE1x5;
  TBranch        *b_phoE2x5;
  TBranch        *b_phoE3x3;
  TBranch        *b_phoE5x5;   //!
  TBranch *b_phoMaxEnergyXtal;
  TBranch        *b_phoSigmaEtaEta;
  TBranch        *b_phoR1x5;
  TBranch        *b_phoR2x5;
  TBranch        *b_phoESEffSigmaRR;   //!
  TBranch        *b_phoSigmaIEtaIEta_2012;   //!
  //TBranch        *b_phoSigmaIEtaIPhi_2012;   //!
  //TBranch        *b_phoSigmaIPhiIPhi_2012;   //!
  //TBranch        *b_phoE1x3_2012;   //!
  //TBranch        *b_phoE2x2_2012;   //!
  //TBranch        *b_phoE2x5Max_2012;   //!
  //TBranch        *b_phoE5x5_2012;   //!
  TBranch        *b_phoE3x3_2012;
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
  TBranch        *b_phoEAc;   //!
  TBranch        *b_phoEAp;   //!
  TBranch        *b_phoEAn;   //!
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
  TBranch        *b_nMu;   //!
  TBranch        *b_muPt;   //!
  TBranch        *b_muEta;   //!
  TBranch        *b_muPhi;   //!
  TBranch        *b_muCharge;   //!
  TBranch        *b_muType;   //!
  TBranch        *b_muIsGood;   //!
  TBranch        *b_muD0;   //!
  TBranch        *b_muDz;   //!
  TBranch        *b_muChi2NDF;   //!
  TBranch        *b_muInnerD0;   //!
  TBranch        *b_muInnerDz;   //!
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
    b_eleD0Err = 0;
    b_eleDzErr = 0;
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
    b_phohasPixelSeed = 0;
    b_phoR9 = 0;
    b_phoHoverE = 0;
    b_phoSigmaIEtaIEta = 0;
    TBranch *b_pho_isEle;
    TBranch *b_pho_is2015Noise;
    //b_phoE1x3 = 0;
    //b_phoE2x2 = 0;
    //b_phoE2x5Max = 0;
    b_phoE1x5 = 0;
    b_phoE2x5 = 0;
    b_phoE3x3 = 0;
    b_phoE5x5 = 0;
    TBranch *b_phoMaxEnergyXtal;
    b_phoSigmaEtaEta = 0;
    b_phoR1x5 = 0;
    b_phoR2x5 = 0;
    b_phoESEffSigmaRR = 0;
    b_phoSigmaIEtaIEta_2012 = 0;
    //b_phoSigmaIEtaIPhi_2012 = 0;
    //b_phoSigmaIPhiIPhi_2012 = 0;
    //b_phoE1x3_2012 = 0;
    //b_phoE2x2_2012 = 0;
    //b_phoE2x5Max_2012 = 0;
    //b_phoE5x5_2012 = 0;
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
    b_phoEAc = 0;
    b_phoEAp = 0;
    b_phoEAn = 0;
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
    b_nMu = 0;
    b_muPt = 0;
    b_muEta = 0;
    b_muPhi = 0;
    b_muCharge = 0;
    b_muType = 0;
    b_muIsGood = 0;
    b_muD0 = 0;
    b_muDz = 0;
    b_muChi2NDF = 0;
    b_muInnerD0 = 0;
    b_muInnerDz = 0;
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
    if (t->GetBranch("phohasPixelSeed")) t->SetBranchAddress("phohasPixelSeed", &phohasPixelSeed, &b_phohasPixelSeed);
    if (t->GetBranch("phoR9")) t->SetBranchAddress("phoR9", &phoR9, &b_phoR9);
    if (t->GetBranch("phoHoverE")) t->SetBranchAddress("phoHoverE", &phoHoverE, &b_phoHoverE);
    if (t->GetBranch("phoSigmaIEtaIEta")) t->SetBranchAddress("phoSigmaIEtaIEta", &phoSigmaIEtaIEta, &b_phoSigmaIEtaIEta);
    if (t->GetBranch("pho_isEle")) t->SetBranchAddress("pho_isEle", &pho_isEle, &b_pho_isEle);
    if (t->GetBranch("pho_is2015Noise")) t->SetBranchAddress("pho_is2015Noise", &pho_is2015Noise, &b_pho_is2015Noise);
    //if (t->GetBranch("phoE1x3")) t->SetBranchAddress("phoE1x3", &phoE1x3, &b_phoE1x3);
    //if (t->GetBranch("phoE2x2")) t->SetBranchAddress("phoE2x2", &phoE2x2, &b_phoE2x2);
    //if (t->GetBranch("phoE2x5Max")) t->SetBranchAddress("phoE2x5Max", &phoE2x5Max, &b_phoE2x5Max);
    if (t->GetBranch("phoE1x5")) t->SetBranchAddress("phoE1x5", &phoE1x5, &b_phoE1x5);
    if (t->GetBranch("phoE2x5")) t->SetBranchAddress("phoE2x5", &phoE2x5, &b_phoE2x5);
    if (t->GetBranch("phoE3x3")) t->SetBranchAddress("phoE3x3", &phoE3x3, &b_phoE3x3);
    if (t->GetBranch("phoE5x5")) t->SetBranchAddress("phoE5x5", &phoE5x5, &b_phoE5x5);
    if (t->GetBranch("phoMaxEnergyXtal")) t->SetBranchAddress("phoMaxEnergyXtal", &phoMaxEnergyXtal, &b_phoMaxEnergyXtal);
    if (t->GetBranch("phoSigmaEtaEta")) t->SetBranchAddress("phoSigmaEtaEta", &phoSigmaEtaEta, &b_phoSigmaEtaEta);
    if (t->GetBranch("phoR1x5")) t->SetBranchAddress("phoR1x5", &phoR1x5, &b_phoR1x5);
    if (t->GetBranch("phoR2x5")) t->SetBranchAddress("phoR2x5", &phoR2x5, &b_phoR2x5);
    if (t->GetBranch("phoESEffSigmaRR")) t->SetBranchAddress("phoESEffSigmaRR", &phoESEffSigmaRR, &b_phoESEffSigmaRR);
    if (t->GetBranch("phoSigmaIEtaIEta_2012")) t->SetBranchAddress("phoSigmaIEtaIEta_2012", &phoSigmaIEtaIEta_2012, &b_phoSigmaIEtaIEta_2012);
    //if (t->GetBranch("phoSigmaIEtaIPhi_2012")) t->SetBranchAddress("phoSigmaIEtaIPhi_2012", &phoSigmaIEtaIPhi_2012, &b_phoSigmaIEtaIPhi_2012);
    //if (t->GetBranch("phoSigmaIPhiIPhi_2012")) t->SetBranchAddress("phoSigmaIPhiIPhi_2012", &phoSigmaIPhiIPhi_2012, &b_phoSigmaIPhiIPhi_2012);
    //if (t->GetBranch("phoE1x3_2012")) t->SetBranchAddress("phoE1x3_2012", &phoE1x3_2012, &b_phoE1x3_2012);
    //if (t->GetBranch("phoE2x2_2012")) t->SetBranchAddress("phoE2x2_2012", &phoE2x2_2012, &b_phoE2x2_2012);
    //if (t->GetBranch("phoE2x5Max_2012")) t->SetBranchAddress("phoE2x5Max_2012", &phoE2x5Max_2012, &b_phoE2x5Max_2012);
    //if (t->GetBranch("phoE5x5_2012")) t->SetBranchAddress("phoE5x5_2012", &phoE5x5_2012, &b_phoE5x5_2012);
    if (t->GetBranch("phoE3x3_2012")) t->SetBranchAddress("phoE3x3_2012", &phoE3x3_2012, &b_phoE3x3_2012);
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
    if (t->GetBranch("pfnIso1")) t->SetBranchAddress("pfnIso1", &pfnIso1, &b_pfnIso1);
    if (t->GetBranch("pfnIso2")) t->SetBranchAddress("pfnIso2", &pfnIso2, &b_pfnIso2);
    if (t->GetBranch("pfnIso3")) t->SetBranchAddress("pfnIso3", &pfnIso3, &b_pfnIso3);
    if (t->GetBranch("pfnIso4")) t->SetBranchAddress("pfnIso4", &pfnIso4, &b_pfnIso4);
    if (t->GetBranch("pfnIso5")) t->SetBranchAddress("pfnIso5", &pfnIso5, &b_pfnIso5);
    if (t->GetBranch("phoEAc")) t->SetBranchAddress("phoEAc", &phoEAc, &b_phoEAc);
    if (t->GetBranch("phoEAp")) t->SetBranchAddress("phoEAp", &phoEAp, &b_phoEAp);
    if (t->GetBranch("phoEAn")) t->SetBranchAddress("phoEAn", &phoEAn, &b_phoEAn);
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
    //if (t->GetBranch("nMu")) t->SetBranchAddress("nMu", &nMu, &b_nMu);
    if (t->GetBranch("muPt")) t->SetBranchAddress("muPt", &muPt, &b_muPt);
    if (t->GetBranch("muEta")) t->SetBranchAddress("muEta", &muEta, &b_muEta);
    if (t->GetBranch("muPhi")) t->SetBranchAddress("muPhi", &muPhi, &b_muPhi);
    if (t->GetBranch("muCharge")) t->SetBranchAddress("muCharge", &muCharge, &b_muCharge);
    if (t->GetBranch("muType")) t->SetBranchAddress("muType", &muType, &b_muType);
    if (t->GetBranch("muIsGood")) t->SetBranchAddress("muIsGood", &muIsGood, &b_muIsGood);
    if (t->GetBranch("muD0")) t->SetBranchAddress("muD0", &muD0, &b_muD0);
    if (t->GetBranch("muDz")) t->SetBranchAddress("muDz", &muDz, &b_muDz);
    if (t->GetBranch("muChi2NDF")) t->SetBranchAddress("muChi2NDF", &muChi2NDF, &b_muChi2NDF);
    if (t->GetBranch("muInnerD0")) t->SetBranchAddress("muInnerD0", &muInnerD0, &b_muInnerD0);
    if (t->GetBranch("muInnerDz")) t->SetBranchAddress("muInnerDz", &muInnerDz, &b_muInnerDz);
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
        t->Branch("eleD0Err", &eleD0Err);
        t->Branch("eleDzErr", &eleDzErr);
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
        t->Branch("phohasPixelSeed", &phohasPixelSeed);
        t->Branch("phoR9", &phoR9);
        t->Branch("phoHoverE", &phoHoverE);
        t->Branch("phoSigmaIEtaIEta", &phoSigmaIEtaIEta);
        t->Branch("pho_isEle", &pho_isEle);
        t->Branch("pho_is2015Noise", &pho_is2015Noise);
        //t->Branch("phoE1x3", &phoE1x3);
        //t->Branch("phoE2x2", &phoE2x2);
        //t->Branch("phoE2x5Max", &phoE2x5Max);
        t->Branch("phoE1x5", &phoE1x5);
        t->Branch("phoE2x5", &phoE2x5);
        t->Branch("phoE3x3", &phoE3x3);
        t->Branch("phoE5x5", &phoE5x5);
        t->Branch("phoMaxEnergyXtal", &phoMaxEnergyXtal);
        t->Branch("phoSigmaEtaEta", &phoSigmaEtaEta);
        t->Branch("phoR1x5", &phoR1x5);
        t->Branch("phoR2x5", &phoR2x5);
        t->Branch("phoESEffSigmaRR", &phoESEffSigmaRR);
        t->Branch("phoSigmaIEtaIEta_2012", &phoSigmaIEtaIEta_2012);
        //t->Branch("phoSigmaIEtaIPhi_2012", &phoSigmaIEtaIPhi_2012);
        //t->Branch("phoSigmaIPhiIPhi_2012", &phoSigmaIPhiIPhi_2012);
        //t->Branch("phoE1x3_2012", &phoE1x3_2012);
        //t->Branch("phoE2x2_2012", &phoE2x2_2012);
        //t->Branch("phoE2x5Max_2012", &phoE2x5Max_2012);
        //t->Branch("phoE5x5_2012", &phoE5x5_2012);
        t->Branch("phoE3x3_2012", &phoE3x3_2012);
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
        t->Branch("phoEAc", &phoEAc);
        t->Branch("phoEAp", &phoEAp);
        t->Branch("phoEAn", &phoEAn);
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
    }
    if (doMu) {
        //t->Branch("nMu", &nMu);
        t->Branch("muPt", &muPt);
        t->Branch("muEta", &muEta);
        t->Branch("muPhi", &muPhi);
        t->Branch("muCharge", &muCharge);
        t->Branch("muType", &muType);
        t->Branch("muIsGood", &muIsGood);
        t->Branch("muD0", &muD0);
        t->Branch("muDz", &muDz);
        t->Branch("muChi2NDF", &muChi2NDF);
        t->Branch("muInnerD0", &muInnerD0);
        t->Branch("muInnerDz", &muInnerDz);
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
        eleD0Err = -987987;
        eleDzErr = -987987;
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
        phohasPixelSeed = -987987;
        phoR9 = -987987;
        phoHoverE = -987987;
        phoSigmaIEtaIEta = -987987;
        //phoE1x3 = -987987;
        //phoE2x2 = -987987;
        //phoE2x5Max = -987987;
        phoE3x3 = -987987;
        phoE1x5 = -987987;
        phoE2x5 = -987987;
        phoE5x5 = -987987;
        phoMaxEnergyXtal = -987987;
        phoSigmaEtaEta = -987987;
        phoR1x5 = -987987;
        phoR2x5 = -987987;
        phoESEffSigmaRR = -987987;
        phoSigmaIEtaIEta_2012 = -987987;
        //phoSigmaIEtaIPhi_2012 = -987987;
        //phoSigmaIPhiIPhi_2012 = -987987;
        //phoE1x3_2012 = -987987;
        //phoE2x2_2012 = -987987;
        //phoE2x5Max_2012 = -987987;
        //phoE5x5_2012 = -987987;
        phoE3x3_2012 = -987987;
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
        phoEAc = 0;
        phoEAp = 0;
        phoEAn = 0;
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
        muD0 = -987987;
        muDz = -987987;
        muChi2NDF = -987987;
        muInnerD0 = -987987;
        muInnerDz = -987987;
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
    //phohasPixelSeed = (*tggHiNtuplizer.phohasPixelSeed)[i];
    phoR9 = (*tggHiNtuplizer.phoR9)[i];
    phoHoverE = (*tggHiNtuplizer.phoHoverE)[i];
    //phoSigmaIEtaIEta = (*tggHiNtuplizer.phoSigmaIEtaIEta)[i];
    //phoE1x3 = (*tggHiNtuplizer.//phoE1x3)[i];
    //phoE2x2 = (*tggHiNtuplizer.//phoE2x2)[i];
    //phoE2x5Max = (*tggHiNtuplizer.//phoE2x5Max)[i];
    phoE3x3 = (*tggHiNtuplizer.phoE3x3)[i];
    phoE1x5 = (*tggHiNtuplizer.phoE1x5)[i];
    phoE2x5 = (*tggHiNtuplizer.phoE2x5)[i];
    phoE5x5 = (*tggHiNtuplizer.phoE5x5)[i];
    phoMaxEnergyXtal = (*tggHiNtuplizer.phoMaxEnergyXtal)[i];
    phoSigmaEtaEta = (*tggHiNtuplizer.phoSigmaEtaEta)[i];
    phoR1x5 = (*tggHiNtuplizer.phoR1x5)[i];
    phoR2x5 = (*tggHiNtuplizer.phoR2x5)[i];
    //phoESEffSigmaRR = (*tggHiNtuplizer.phoESEffSigmaRR)[i];
    phoSigmaIEtaIEta_2012 = (*tggHiNtuplizer.phoSigmaIEtaIEta_2012)[i];
    //phoSigmaIEtaIPhi_2012 = (*tggHiNtuplizer.//phoSigmaIEtaIPhi_2012)[i];
    //phoSigmaIPhiIPhi_2012 = (*tggHiNtuplizer.//phoSigmaIPhiIPhi_2012)[i];
    //phoE1x3_2012 = (*tggHiNtuplizer.//phoE1x3_2012)[i];
    //phoE2x2_2012 = (*tggHiNtuplizer.//phoE2x2_2012)[i];
    //phoE2x5Max_2012 = (*tggHiNtuplizer.//phoE2x5Max_2012)[i];
    //phoE5x5_2012 = (*tggHiNtuplizer.//phoE5x5_2012)[i];
    phoE3x3_2012 = (*tggHiNtuplizer.phoE3x3_2012)[i];
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
    muD0 = (*tggHiNtuplizer.muD0)[i];
    muDz = (*tggHiNtuplizer.muDz)[i];
    muChi2NDF = (*tggHiNtuplizer.muChi2NDF)[i];
    muInnerD0 = (*tggHiNtuplizer.muInnerD0)[i];
    muInnerDz = (*tggHiNtuplizer.muInnerDz)[i];
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

#endif /* TREEHEADERS_GGHIFLATTREE_H_ */
