#ifndef TREEHEADERS_GGHIFLATTREE_H_
#define TREEHEADERS_GGHIFLATTREE_H_

#include <TTree.h>
#include <TBranch.h>

#include <vector>

#include "ggHiNtuplizerTree.h"

class ggHiFlat {
public :
  ggHiFlat() {

      angEP2pf = 0;
      angEP3pf = 0;

      partphi_binContents = 0;
      trkphi_binContents = 0;

      fMods_th1_yMin = 0;
      fMods_angEP2 = 0;
      fMods_angEP3 = 0;
      fMods_v2 = 0;
      fMods_v3 = 0;
      fMods_chi2 = 0;
      fMods_chi2prob = 0;

      pfpIso3subUEfMods = 0;
      pfnIso3subUEfMods = 0;
      pfcIso3subUEfMods = 0;
      pfcIso3pTgt2p0subUEfMods = 0;

      doEle = false;
      doPho = false;
      doMu = false;
      doMC = false;
      doMatchGen = false;
      doMatchPF = false;
  };
  ~ggHiFlat(){};
  void setupTreeForReading(TTree *t);
  void setupTreeForWriting(TTree* t);
  void clearEntry();
  void clearEntryEle();
  void clearEntryPho();
  void clearEntryMu();
  void clearEntryGen();
  void clearEntryMatchedGen();
  void clearEntryMatchedPF();
  void resetPhoPF();
  void copyEle(ggHiNtuplizer &tggHiNtuplizer, int i);
  void copyPho(ggHiNtuplizer &tggHiNtuplizer, int i);
  void copyMu(ggHiNtuplizer &tggHiNtuplizer, int i);
  void copyGen(ggHiNtuplizer &tggHiNtuplizer, int i);

  void clone(ggHiFlat &gg);

  bool passedHI18HEMfailurePho();

  double getValueByName(std::string varName);
  void copy2Vars(float *vals, std::vector<std::string>& varNames, int nVars, int offset);

  bool doEle;
  bool doPho;
  bool doMu;
  bool doMC;
  bool doMatchGen;
  bool doMatchPF;

  // Declaration of leaf types
  float weight;
  float weightCent;
  float weightKin;      // weight for kinematics : pt, eta
  float weightGenKin;   // weight for kinematics of matched gen-particle
  float weightPthat;
  float phi0;
  float pthat;
  int processID;
  int hiBin;
  float hiHF;
  float hiEP1HF;
  float hiEP2HF;   // 2nd order HF event plane https://github.com/CmsHI/cmssw/blob/48abfe59b0007693a8fa385abe45064a34abdcca/RecoHI/HiEvtPlaneAlgos/interface/HiEvtPlaneList.h#L13
  float hiEP3HF;   // 3rd order HF event plane
  float hiEP4HF;

  // event plane angle calculated using charged PF cands
  std::vector<float> *angEP2pf;
  std::vector<float> angEP2pf_out;
  std::vector<float> *angEP3pf;
  std::vector<float> angEP3pf_out;

  // event plane angle calculated using tracks
  float angEP2trk;
  float angEP3trk;

  int partphi_nBins;
  float partphi_minContent;
  std::vector<float> *partphi_binContents;
  std::vector<float> partphi_binContents_out;
  float fit_v2;
  float fit_v3;
  float fit_chi2;
  float fit_chi2prob;

  float fit_EPphi0_v2;
  float fit_EPphi0_chi2;
  float fit_EPphi0_chi2prob;

  int trkphi_nBins;
  float trkphi_minContent;
  std::vector<float> *trkphi_binContents;
  std::vector<float> trkphi_binContents_out;
  float fit_trkphi_v2;
  float fit_trkphi_v3;
  float fit_trkphi_chi2;
  float fit_trkphi_chi2prob;

  float rho;
  Float_t         angEvtPlane;
  Int_t           indexEvtPlane;
  Int_t           phi_nTot;
  Int_t           phi_minBinN;
  Float_t         phi_fit_chi2;
  Float_t         phi_fit_chi2prob;
  Float_t         phi_fit_v2;
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

  float pho_trkIso3pTgt2p0;
  float pho_trkIso3pTgt2p0subUE;
  float pho_trkIso3IDpTgt2p0;
  float pho_trkIso3IDpTgt2p0subUE;

  // different definitions for flow modulation, each definition is one element in vector
  std::vector<float> *fMods_th1_yMin;
  std::vector<float> *fMods_angEP2;
  std::vector<float> *fMods_angEP3;
  std::vector<float> *fMods_v2;
  std::vector<float> *fMods_v3;
  std::vector<float> *fMods_chi2;
  std::vector<float> *fMods_chi2prob;

  std::vector<float> fMods_th1_yMin_out;
  std::vector<float> fMods_angEP2_out;
  std::vector<float> fMods_angEP3_out;
  std::vector<float> fMods_v2_out;
  std::vector<float> fMods_v3_out;
  std::vector<float> fMods_chi2_out;
  std::vector<float> fMods_chi2prob_out;

  std::vector<float> *pfpIso3subUEfMods;
  std::vector<float> *pfnIso3subUEfMods;
  std::vector<float> *pfcIso3subUEfMods;
  std::vector<float> *pfcIso3pTgt2p0subUEfMods;

  std::vector<float> pfpIso3subUEfMods_out;
  std::vector<float> pfnIso3subUEfMods_out;
  std::vector<float> pfcIso3subUEfMods_out;
  std::vector<float> pfcIso3pTgt2p0subUEfMods_out;

  float pfpIso3subUEcalc;
  float pfnIso3subUEcalc;
  float pfcIso3pTgt2p0subUEcalc;
  float pfcIso3subUEcalc;
  float pfcIso3IDpTgt2p0subUEcalc;
  float pfcIso3IDsubUEcalc;
  // no footprint removal (for comparison to trkIso3)
  float pfpIso3noFPsubUEcalc;
  float pfnIso3noFPsubUEcalc;
  float pfcIso3pTgt2p0noFPsubUEcalc;
  float pfcIso3noFPsubUEcalc;
  float pfcIso3IDpTgt2p0noFPsubUEcalc;
  float pfcIso3IDnoFPsubUEcalc;

  // iso using PF electron and muon
  float pfeIso3pTgt2p0subUEcalc;
  float pfeIso3subUEcalc;
  float pfeIso3IDpTgt2p0subUEcalc;
  float pfeIso3IDsubUEcalc;
  float pfmIso3pTgt2p0subUEcalc;
  float pfmIso3subUEcalc;
  float pfmIso3IDpTgt2p0subUEcalc;
  float pfmIso3IDsubUEcalc;
  // noFP
  float pfeIso3pTgt2p0noFPsubUEcalc;
  float pfeIso3noFPsubUEcalc;
  float pfeIso3IDpTgt2p0noFPsubUEcalc;
  float pfeIso3IDnoFPsubUEcalc;
  float pfmIso3pTgt2p0noFPsubUEcalc;
  float pfmIso3noFPsubUEcalc;
  float pfmIso3IDpTgt2p0noFPsubUEcalc;
  float pfmIso3IDnoFPsubUEcalc;

  // using CS PF cands
  float pfCSpIso3subUEcalc;
  float pfCSnIso3subUEcalc;
  float pfCScIso3pTgt2p0subUEcalc;
  float pfCScIso3subUEcalc;

  // modulated via vn2 fit
  float pfpIso3subUEvn2;
  float pfnIso3subUEvn2;
  float pfcIso3pTgt2p0subUEvn2;
  float pfcIso3subUEvn2;

  // fit after reading event plane from "phi0"
  float pfpIso3subUEphi0vn2;
  float pfnIso3subUEphi0vn2;
  float pfcIso3pTgt2p0subUEphi0vn2;
  float pfcIso3subUEphi0vn2;

  // modulated via vn3 fit to trk phi
  float pfpIso3subUEtrkvn3;
  float pfnIso3subUEtrkvn3;
  float pfcIso3pTgt2p0subUEtrkvn3;
  float pfcIso3subUEtrkvn3;

  float trkIso3;
  float trkIso3subUE;
  float trkIso3ID;
  float trkIso3IDsubUE;
  float trkIso3IDmatchPF;

  float phoEAc;
  float phoEAp;
  float phoEAn;
  float phoEAhoe;
  float phoEAsieie;

  int nPhoPFp;
  int nPhoPFn;
  int nPhoPFc;
  int nPhoPFe;
  int nPhoPFm;
  int nPhoPFx;
  float ppfpPtSum;
  float ppfpEtaAve;
  float ppfpPhiAve;
  float ppfnPtSum;
  float ppfnEtaAve;
  float ppfnPhiAve;
  float ppfcPtSum;
  float ppfcEtaAve;
  float ppfcPhiAve;
  float ppfcTrkID;
  float ppfePtSum;
  float ppfeEtaAve;
  float ppfePhiAve;
  float ppfeTrkID;
  float ppfmPtSum;
  float ppfmEtaAve;
  float ppfmPhiAve;
  float ppfmTrkID;
  float ppfxPtSum;
  float ppfxEtaAve;
  float ppfxPhiAve;

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

  // info about matched gen particles
  int mgPDG;
  float mgPt;
  float mgEta;
  float mgPhi;
  int mgIdx;
  float mgPhoPt;
  float mgPhoEta;
  float mgPhoPhi;
  int mgPhoIdx;
  float mgElePt;
  float mgEleEta;
  float mgElePhi;
  int mgEleIdx;
  float mgMuoPt;
  float mgMuoEta;
  float mgMuoPhi;
  int mgMuoIdx;

  // info about matched PF cands
  int matchPFid;
  float matchPFpt;
  float matchPFeta;
  float matchPFphi;
  int matchPFidx;
  float matchPFphoPt;
  float matchPFphoEta;
  float matchPFphoPhi;
  int matchPFphoIdx;
  float matchPFelePt;
  float matchPFeleEta;
  float matchPFelePhi;
  int matchPFeleIdx;
  float matchPFmuoPt;
  float matchPFmuoEta;
  float matchPFmuoPhi;
  int matchPFmuoIdx;

  // List of branches
  TBranch        *b_weight;   //!
  TBranch        *b_weightCent;   //!
  TBranch        *b_weightKin;   //!
  TBranch        *b_weightGenKin;   //!
  TBranch        *b_weightPthat;   //!
  TBranch        *b_phi0;   //!
  TBranch        *b_pthat;   //!
  TBranch        *b_processID;   //!
  TBranch        *b_hiBin;   //!
  TBranch        *b_hiHF;   //!
  TBranch        *b_hiEP1HF;   //!
  TBranch        *b_hiEP2HF;   //!
  TBranch        *b_hiEP3HF;   //!
  TBranch        *b_hiEP4HF;   //!

  TBranch        *b_angEP2pf;   //!
  TBranch        *b_angEP3pf;   //!
  TBranch        *b_angEP2trk;   //!
  TBranch        *b_angEP3trk;   //!

  TBranch        *b_partphi_nBins;   //!
  TBranch        *b_partphi_minContent;   //!
  TBranch        *b_partphi_binContents;   //!
  TBranch        *b_fit_v2;   //!
  TBranch        *b_fit_v3;   //!
  TBranch        *b_fit_chi2;   //!
  TBranch        *b_fit_chi2prob;   //!

  TBranch        *b_fit_EPphi0_v2;   //!
  TBranch        *b_fit_EPphi0_chi2;   //!
  TBranch        *b_fit_EPphi0_chi2prob;   //!

  TBranch        *b_trkphi_nBins;   //!
  TBranch        *b_trkphi_minContent;   //!
  TBranch        *b_trkphi_binContents;   //!
  TBranch        *b_fit_trkphi_v2;   //!
  TBranch        *b_fit_trkphi_v3;   //!
  TBranch        *b_fit_trkphi_chi2;   //!
  TBranch        *b_fit_trkphi_chi2prob;   //!

  TBranch        *b_rho;   //!
  TBranch        *b_angEvtPlane;   //!
  TBranch        *b_indexEvtPlane;   //!
  TBranch        *b_phi_nTot;   //!
  TBranch        *b_phi_minBinN;   //!
  TBranch        *b_phi_fit_chi2;   //!
  TBranch        *b_phi_fit_chi2prob;   //!
  TBranch        *b_phi_fit_v2;   //!
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

  TBranch        *b_pho_trkIso3pTgt2p0;   //!
  TBranch        *b_pho_trkIso3pTgt2p0subUE;   //!
  TBranch        *b_pho_trkIso3IDpTgt2p0;   //!
  TBranch        *b_pho_trkIso3IDpTgt2p0subUE;   //!

  TBranch        *b_fMods_th1_yMin;   //!
  TBranch        *b_fMods_angEP2;   //!
  TBranch        *b_fMods_angEP3;   //!
  TBranch        *b_fMods_v2;   //!
  TBranch        *b_fMods_v3;   //!
  TBranch        *b_fMods_chi2;   //!
  TBranch        *b_fMods_chi2prob;   //!

  TBranch        *b_pfpIso3subUEfMods;   //!
  TBranch        *b_pfnIso3subUEfMods;   //!
  TBranch        *b_pfcIso3subUEfMods;   //!
  TBranch        *b_pfcIso3pTgt2p0subUEfMods;   //!

  TBranch        *b_pfpIso3subUEcalc;   //!
  TBranch        *b_pfnIso3subUEcalc;   //!
  TBranch        *b_pfcIso3pTgt2p0subUEcalc;   //!
  TBranch        *b_pfcIso3subUEcalc;   //!
  TBranch        *b_pfcIso3IDpTgt2p0subUEcalc;   //!
  TBranch        *b_pfcIso3IDsubUEcalc;   //!

  TBranch        *b_pfpIso3noFPsubUEcalc;   //!
  TBranch        *b_pfnIso3noFPsubUEcalc;   //!
  TBranch        *b_pfcIso3pTgt2p0noFPsubUEcalc;   //!
  TBranch        *b_pfcIso3noFPsubUEcalc;   //!
  TBranch        *b_pfcIso3IDpTgt2p0noFPsubUEcalc;   //!
  TBranch        *b_pfcIso3IDnoFPsubUEcalc;   //!

  TBranch        *b_pfeIso3pTgt2p0subUEcalc;   //!
  TBranch        *b_pfeIso3subUEcalc;   //!
  TBranch        *b_pfeIso3IDpTgt2p0subUEcalc;   //!
  TBranch        *b_pfeIso3IDsubUEcalc;   //!
  TBranch        *b_pfmIso3pTgt2p0subUEcalc;   //!
  TBranch        *b_pfmIso3subUEcalc;   //!
  TBranch        *b_pfmIso3IDpTgt2p0subUEcalc;   //!
  TBranch        *b_pfmIso3IDsubUEcalc;   //!

  TBranch        *b_pfeIso3pTgt2p0noFPsubUEcalc;   //!
  TBranch        *b_pfeIso3noFPsubUEcalc;   //!
  TBranch        *b_pfeIso3IDpTgt2p0noFPsubUEcalc;   //!
  TBranch        *b_pfeIso3IDnoFPsubUEcalc;   //!
  TBranch        *b_pfmIso3pTgt2p0noFPsubUEcalc;   //!
  TBranch        *b_pfmIso3noFPsubUEcalc;   //!
  TBranch        *b_pfmIso3IDpTgt2p0noFPsubUEcalc;   //!
  TBranch        *b_pfmIso3IDnoFPsubUEcalc;   //!

  TBranch        *b_pfCSpIso3subUEcalc;   //!
  TBranch        *b_pfCSnIso3subUEcalc;   //!
  TBranch        *b_pfCScIso3pTgt2p0subUEcalc;   //!
  TBranch        *b_pfCScIso3subUEcalc;   //!

  TBranch        *b_pfpIso3subUEvn2;   //!
  TBranch        *b_pfnIso3subUEvn2;   //!
  TBranch        *b_pfcIso3pTgt2p0subUEvn2;   //!
  TBranch        *b_pfcIso3subUEvn2;   //!

  TBranch        *b_pfpIso3subUEphi0vn2;   //!
  TBranch        *b_pfnIso3subUEphi0vn2;   //!
  TBranch        *b_pfcIso3pTgt2p0subUEphi0vn2;   //!
  TBranch        *b_pfcIso3subUEphi0vn2;   //!

  TBranch        *b_pfpIso3subUEtrkvn3;   //!
  TBranch        *b_pfnIso3subUEtrkvn3;   //!
  TBranch        *b_pfcIso3pTgt2p0subUEtrkvn3;   //!
  TBranch        *b_pfcIso3subUEtrkvn3;   //!

  TBranch        *b_trkIso3;   //!
  TBranch        *b_trkIso3subUE;   //!
  TBranch        *b_trkIso3ID;   //!
  TBranch        *b_trkIso3IDsubUE;   //!
  TBranch        *b_trkIso3IDmatchPF;   //!
  TBranch        *b_phoEAc;   //!
  TBranch        *b_phoEAp;   //!
  TBranch        *b_phoEAn;   //!
  TBranch        *b_phoEAhoe;   //!
  TBranch        *b_phoEAsieie;   //!

  TBranch        *b_nPhoPFp;   //!
  TBranch        *b_ppfpPtSum;   //!
  TBranch        *b_ppfpEtaAve;   //!
  TBranch        *b_ppfpPhiAve;   //!
  TBranch        *b_nPhoPFn;   //!
  TBranch        *b_ppfnPtSum;   //!
  TBranch        *b_ppfnEtaAve;   //!
  TBranch        *b_ppfnPhiAve;   //!
  TBranch        *b_nPhoPFc;   //!
  TBranch        *b_ppfcPtSum;   //!
  TBranch        *b_ppfcEtaAve;   //!
  TBranch        *b_ppfcPhiAve;   //!
  TBranch        *b_ppfcTrkID;   //!
  TBranch        *b_nPhoPFe;   //!
  TBranch        *b_ppfePtSum;   //!
  TBranch        *b_ppfeEtaAve;   //!
  TBranch        *b_ppfePhiAve;   //!
  TBranch        *b_ppfeTrkID;   //!
  TBranch        *b_nPhoPFm;   //!
  TBranch        *b_ppfmPtSum;   //!
  TBranch        *b_ppfmEtaAve;   //!
  TBranch        *b_ppfmPhiAve;   //!
  TBranch        *b_ppfmTrkID;   //!
  TBranch        *b_nPhoPFx;   //!
  TBranch        *b_ppfxPtSum;   //!
  TBranch        *b_ppfxEtaAve;   //!
  TBranch        *b_ppfxPhiAve;   //!

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

  TBranch        *b_mgPDG;   //!
  TBranch        *b_mgPt;   //!
  TBranch        *b_mgEta;   //!
  TBranch        *b_mgPhi;   //!
  TBranch        *b_mgIdx;   //!
  TBranch        *b_mgPhoPt;   //!
  TBranch        *b_mgPhoEta;   //!
  TBranch        *b_mgPhoPhi;   //!
  TBranch        *b_mgPhoIdx;   //!
  TBranch        *b_mgElePt;   //!
  TBranch        *b_mgEleEta;   //!
  TBranch        *b_mgElePhi;   //!
  TBranch        *b_mgEleIdx;   //!
  TBranch        *b_mgMuoPt;   //!
  TBranch        *b_mgMuoEta;   //!
  TBranch        *b_mgMuoPhi;   //!
  TBranch        *b_mgMuoIdx;   //!

  TBranch        *b_matchPFid;   //!
  TBranch        *b_matchPFpt;   //!
  TBranch        *b_matchPFeta;   //!
  TBranch        *b_matchPFphi;   //!
  TBranch        *b_matchPFidx;   //!
  TBranch        *b_matchPFphoPt;   //!
  TBranch        *b_matchPFphoEta;   //!
  TBranch        *b_matchPFphoPhi;   //!
  TBranch        *b_matchPFphoIdx;   //!
  TBranch        *b_matchPFelePt;   //!
  TBranch        *b_matchPFeleEta;   //!
  TBranch        *b_matchPFelePhi;   //!
  TBranch        *b_matchPFeleIdx;   //!
  TBranch        *b_matchPFmuoPt;   //!
  TBranch        *b_matchPFmuoEta;   //!
  TBranch        *b_matchPFmuoPhi;   //!
  TBranch        *b_matchPFmuoIdx;   //!
};

void ggHiFlat::setupTreeForReading(TTree *t)
{
    b_weight = 0;
    b_weightCent = 0;
    b_weightKin = 0;
    b_weightGenKin = 0;
    b_weightPthat = 0;
    b_phi0 = 0;
    b_pthat = 0;
    b_processID = 0;
    b_hiBin = 0;
    b_hiHF = 0;
    b_hiEP1HF = 0;
    b_hiEP2HF = 0;
    b_hiEP3HF = 0;
    b_hiEP4HF = 0;

    b_angEP2pf = 0;
    b_angEP3pf = 0;
    b_angEP2trk = 0;
    b_angEP3trk = 0;

    b_partphi_nBins = 0;
    b_partphi_minContent = 0;
    b_partphi_binContents = 0;
    b_fit_v2 = 0;
    b_fit_v3 = 0;
    b_fit_chi2 = 0;
    b_fit_chi2prob = 0;

    b_fit_EPphi0_v2 = 0;
    b_fit_EPphi0_chi2 = 0;
    b_fit_EPphi0_chi2prob = 0;

    b_trkphi_nBins = 0;
    b_trkphi_minContent = 0;
    b_trkphi_binContents = 0;
    b_fit_trkphi_v2 = 0;
    b_fit_trkphi_v3 = 0;
    b_fit_trkphi_chi2 = 0;
    b_fit_trkphi_chi2prob = 0;

    b_rho = 0;
    b_angEvtPlane = 0;
    b_indexEvtPlane = 0;
    b_phi_nTot = 0;
    b_phi_minBinN = 0;
    b_phi_fit_chi2 = 0;
    b_phi_fit_chi2prob = 0;
    b_phi_fit_v2 = 0;
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

    b_pho_trkIso3pTgt2p0 = 0;
    b_pho_trkIso3pTgt2p0subUE = 0;
    b_pho_trkIso3IDpTgt2p0 = 0;
    b_pho_trkIso3IDpTgt2p0subUE = 0;

    b_fMods_th1_yMin = 0;
    b_fMods_angEP2 = 0;
    b_fMods_angEP3 = 0;
    b_fMods_v2 = 0;
    b_fMods_v3 = 0;
    b_fMods_chi2 = 0;
    b_fMods_chi2prob = 0;

    b_pfpIso3subUEfMods = 0;
    b_pfnIso3subUEfMods = 0;
    b_pfcIso3subUEfMods = 0;
    b_pfcIso3pTgt2p0subUEfMods = 0;

    b_pfpIso3subUEcalc = 0;
    b_pfnIso3subUEcalc = 0;
    b_pfcIso3pTgt2p0subUEcalc = 0;
    b_pfcIso3subUEcalc = 0;
    b_pfcIso3IDpTgt2p0subUEcalc = 0;
    b_pfcIso3IDsubUEcalc = 0;

    b_pfpIso3noFPsubUEcalc = 0;
    b_pfnIso3noFPsubUEcalc = 0;
    b_pfcIso3pTgt2p0noFPsubUEcalc = 0;
    b_pfcIso3noFPsubUEcalc = 0;
    b_pfcIso3IDpTgt2p0noFPsubUEcalc = 0;
    b_pfcIso3IDnoFPsubUEcalc = 0;

    b_pfeIso3pTgt2p0subUEcalc = 0;
    b_pfeIso3subUEcalc = 0;
    b_pfeIso3IDpTgt2p0subUEcalc = 0;
    b_pfeIso3IDsubUEcalc = 0;
    b_pfmIso3pTgt2p0subUEcalc = 0;
    b_pfmIso3subUEcalc = 0;
    b_pfmIso3IDpTgt2p0subUEcalc = 0;
    b_pfmIso3IDsubUEcalc = 0;

    b_pfeIso3pTgt2p0noFPsubUEcalc = 0;
    b_pfeIso3noFPsubUEcalc = 0;
    b_pfeIso3IDpTgt2p0noFPsubUEcalc = 0;
    b_pfeIso3IDnoFPsubUEcalc = 0;
    b_pfmIso3pTgt2p0noFPsubUEcalc = 0;
    b_pfmIso3noFPsubUEcalc = 0;
    b_pfmIso3IDpTgt2p0noFPsubUEcalc = 0;
    b_pfmIso3IDnoFPsubUEcalc = 0;

    b_pfCSpIso3subUEcalc = 0;
    b_pfCSnIso3subUEcalc = 0;
    b_pfCScIso3pTgt2p0subUEcalc = 0;
    b_pfCScIso3subUEcalc = 0;

    b_pfpIso3subUEvn2 = 0;
    b_pfnIso3subUEvn2 = 0;
    b_pfcIso3pTgt2p0subUEvn2 = 0;
    b_pfcIso3subUEvn2 = 0;

    b_pfpIso3subUEphi0vn2 = 0;
    b_pfnIso3subUEphi0vn2 = 0;
    b_pfcIso3pTgt2p0subUEphi0vn2 = 0;
    b_pfcIso3subUEphi0vn2 = 0;

    b_pfpIso3subUEtrkvn3 = 0;
    b_pfnIso3subUEtrkvn3 = 0;
    b_pfcIso3pTgt2p0subUEtrkvn3 = 0;
    b_pfcIso3subUEtrkvn3 = 0;

    b_trkIso3 = 0;
    b_trkIso3subUE = 0;
    b_trkIso3ID = 0;
    b_trkIso3IDsubUE = 0;
    b_trkIso3IDmatchPF = 0;
    b_phoEAc = 0;
    b_phoEAp = 0;
    b_phoEAn = 0;
    b_phoEAhoe = 0;
    b_phoEAsieie = 0;

    b_nPhoPFp = 0;
    b_ppfpPtSum = 0;
    b_ppfpEtaAve = 0;
    b_ppfpPhiAve = 0;
    b_nPhoPFn = 0;
    b_ppfnPtSum = 0;
    b_ppfnEtaAve = 0;
    b_ppfnPhiAve = 0;
    b_nPhoPFc = 0;
    b_ppfcPtSum = 0;
    b_ppfcEtaAve = 0;
    b_ppfcPhiAve = 0;
    b_ppfcTrkID = 0;
    b_nPhoPFe = 0;
    b_ppfePtSum = 0;
    b_ppfeEtaAve = 0;
    b_ppfePhiAve = 0;
    b_ppfeTrkID = 0;
    b_nPhoPFm = 0;
    b_ppfmPtSum = 0;
    b_ppfmEtaAve = 0;
    b_ppfmPhiAve = 0;
    b_ppfmTrkID = 0;
    b_nPhoPFx = 0;
    b_ppfxPtSum = 0;
    b_ppfxEtaAve = 0;
    b_ppfxPhiAve = 0;

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

    b_mgPDG = 0;
    b_mgPt = 0;
    b_mgEta = 0;
    b_mgPhi = 0;
    b_mgIdx = 0;
    b_mgPhoPt = 0;
    b_mgPhoEta = 0;
    b_mgPhoPhi = 0;
    b_mgPhoIdx = 0;
    b_mgElePt = 0;
    b_mgEleEta = 0;
    b_mgElePhi = 0;
    b_mgEleIdx = 0;
    b_mgMuoPt = 0;
    b_mgMuoEta = 0;
    b_mgMuoPhi = 0;
    b_mgMuoIdx = 0;

    b_matchPFid = 0;
    b_matchPFpt = 0;
    b_matchPFeta = 0;
    b_matchPFphi = 0;
    b_matchPFidx = 0;
    b_matchPFphoPt = 0;
    b_matchPFphoEta = 0;
    b_matchPFphoPhi = 0;
    b_matchPFphoIdx = 0;
    b_matchPFelePt = 0;
    b_matchPFeleEta = 0;
    b_matchPFelePhi = 0;
    b_matchPFeleIdx = 0;
    b_matchPFmuoPt = 0;
    b_matchPFmuoEta = 0;
    b_matchPFmuoPhi = 0;
    b_matchPFmuoIdx = 0;

    // Set branch addresses and branch pointers
    if (t->GetBranch("weight")) t->SetBranchAddress("weight", &weight, &b_weight);
    if (t->GetBranch("weightCent")) t->SetBranchAddress("weightCent", &weightCent, &b_weightCent);
    if (t->GetBranch("weightKin")) t->SetBranchAddress("weightKin", &weightKin, &b_weightKin);
    if (t->GetBranch("weightGenKin")) t->SetBranchAddress("weightGenKin", &weightGenKin, &b_weightGenKin);
    if (t->GetBranch("weightPthat")) t->SetBranchAddress("weightPthat", &weightPthat, &b_weightPthat);
    if (t->GetBranch("phi0")) t->SetBranchAddress("phi0", &phi0, &b_phi0);
    if (t->GetBranch("pthat")) t->SetBranchAddress("pthat", &pthat, &b_pthat);
    if (t->GetBranch("processID")) t->SetBranchAddress("processID", &processID, &b_processID);
    if (t->GetBranch("hiBin")) t->SetBranchAddress("hiBin", &hiBin, &b_hiBin);
    if (t->GetBranch("hiHF")) t->SetBranchAddress("hiHF", &hiHF, &b_hiHF);
    if (t->GetBranch("hiEP1HF")) t->SetBranchAddress("hiEP1HF", &hiEP1HF, &b_hiEP1HF);
    if (t->GetBranch("hiEP2HF")) t->SetBranchAddress("hiEP2HF", &hiEP2HF, &b_hiEP2HF);
    if (t->GetBranch("hiEP3HF")) t->SetBranchAddress("hiEP3HF", &hiEP3HF, &b_hiEP3HF);
    if (t->GetBranch("hiEP4HF")) t->SetBranchAddress("hiEP4HF", &hiEP4HF, &b_hiEP4HF);

    if (t->GetBranch("angEP2pf")) t->SetBranchAddress("angEP2pf", &angEP2pf, &b_angEP2pf);
    if (t->GetBranch("angEP3pf")) t->SetBranchAddress("angEP3pf", &angEP3pf, &b_angEP3pf);
    if (t->GetBranch("angEP2trk")) t->SetBranchAddress("angEP2trk", &angEP2trk, &b_angEP2trk);
    if (t->GetBranch("angEP3trk")) t->SetBranchAddress("angEP3trk", &angEP3trk, &b_angEP3trk);

    if (false) {
        if (t->GetBranch("partphi_nBins")) t->SetBranchAddress("partphi_nBins", &partphi_nBins, &b_partphi_nBins);
        if (t->GetBranch("partphi_minContent")) t->SetBranchAddress("partphi_minContent", &partphi_minContent, &b_partphi_minContent);
        if (t->GetBranch("partphi_binContents")) t->SetBranchAddress("partphi_binContents", &partphi_binContents, &b_partphi_binContents);
        if (t->GetBranch("fit_v2")) t->SetBranchAddress("fit_v2", &fit_v2, &b_fit_v2);
        if (t->GetBranch("fit_v3")) t->SetBranchAddress("fit_v3", &fit_v3, &b_fit_v3);
        if (t->GetBranch("fit_chi2")) t->SetBranchAddress("fit_chi2", &fit_chi2, &b_fit_chi2);
        if (t->GetBranch("fit_chi2prob")) t->SetBranchAddress("fit_chi2prob", &fit_chi2prob, &b_fit_chi2prob);
    }

    if (t->GetBranch("fit_EPphi0_v2")) t->SetBranchAddress("fit_EPphi0_v2", &fit_EPphi0_v2, &b_fit_EPphi0_v2);
    if (t->GetBranch("fit_EPphi0_chi2")) t->SetBranchAddress("fit_EPphi0_chi2", &fit_EPphi0_chi2, &b_fit_EPphi0_chi2);
    if (t->GetBranch("fit_EPphi0_chi2prob")) t->SetBranchAddress("fit_EPphi0_chi2prob", &fit_EPphi0_chi2prob, &b_fit_EPphi0_chi2prob);

    if (t->GetBranch("trkphi_nBins")) t->SetBranchAddress("trkphi_nBins", &trkphi_nBins, &b_trkphi_nBins);
    if (t->GetBranch("trkphi_minContent")) t->SetBranchAddress("trkphi_minContent", &trkphi_minContent, &b_trkphi_minContent);
    if (t->GetBranch("trkphi_binContents")) t->SetBranchAddress("trkphi_binContents", &trkphi_binContents, &b_trkphi_binContents);
    if (t->GetBranch("fit_trkphi_v2")) t->SetBranchAddress("fit_trkphi_v2", &fit_trkphi_v2, &b_fit_trkphi_v2);
    if (t->GetBranch("fit_trkphi_v3")) t->SetBranchAddress("fit_trkphi_v3", &fit_trkphi_v3, &b_fit_trkphi_v3);
    if (t->GetBranch("fit_trkphi_chi2")) t->SetBranchAddress("fit_trkphi_chi2", &fit_trkphi_chi2, &b_fit_trkphi_chi2);
    if (t->GetBranch("fit_trkphi_chi2prob")) t->SetBranchAddress("fit_trkphi_chi2prob", &fit_trkphi_chi2prob, &b_fit_trkphi_chi2prob);

    if (t->GetBranch("fMods_th1_yMin")) t->SetBranchAddress("fMods_th1_yMin", &fMods_th1_yMin, &b_fMods_th1_yMin);
    if (t->GetBranch("fMods_angEP2")) t->SetBranchAddress("fMods_angEP2", &fMods_angEP2, &b_fMods_angEP2);
    if (t->GetBranch("fMods_angEP3")) t->SetBranchAddress("fMods_angEP3", &fMods_angEP3, &b_fMods_angEP3);
    if (t->GetBranch("fMods_v2")) t->SetBranchAddress("fMods_v2", &fMods_v2, &b_fMods_v2);
    if (t->GetBranch("fMods_v3")) t->SetBranchAddress("fMods_v3", &fMods_v3, &b_fMods_v3);
    if (t->GetBranch("fMods_chi2")) t->SetBranchAddress("fMods_chi2", &fMods_chi2, &b_fMods_chi2);
    if (t->GetBranch("fMods_chi2prob")) t->SetBranchAddress("fMods_chi2prob", &fMods_chi2prob, &b_fMods_chi2prob);

    if (t->GetBranch("rho")) t->SetBranchAddress("rho", &rho, &b_rho);
    if (t->GetBranch("angEvtPlane")) t->SetBranchAddress("angEvtPlane", &angEvtPlane, &b_angEvtPlane);
    if (t->GetBranch("indexEvtPlane")) t->SetBranchAddress("indexEvtPlane", &indexEvtPlane, &b_indexEvtPlane);
    if (t->GetBranch("phi_nTot")) t->SetBranchAddress("phi_nTot", &phi_nTot, &b_phi_nTot);
    if (t->GetBranch("phi_minBinN")) t->SetBranchAddress("phi_minBinN", &phi_minBinN, &b_phi_minBinN);
    if (t->GetBranch("phi_fit_chi2")) t->SetBranchAddress("phi_fit_chi2", &phi_fit_chi2, &b_phi_fit_chi2);
    if (t->GetBranch("phi_fit_chi2prob")) t->SetBranchAddress("phi_fit_chi2prob", &phi_fit_chi2prob, &b_phi_fit_chi2prob);
    if (t->GetBranch("phi_fit_v2")) t->SetBranchAddress("phi_fit_v2", &phi_fit_v2, &b_phi_fit_v2);
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

    if (t->GetBranch("pho_trkIso3pTgt2p0")) t->SetBranchAddress("pho_trkIso3pTgt2p0", &pho_trkIso3pTgt2p0, &b_pho_trkIso3pTgt2p0);
    if (t->GetBranch("pho_trkIso3pTgt2p0subUE")) t->SetBranchAddress("pho_trkIso3pTgt2p0subUE", &pho_trkIso3pTgt2p0subUE, &b_pho_trkIso3pTgt2p0subUE);
    if (t->GetBranch("pho_trkIso3IDpTgt2p0")) t->SetBranchAddress("pho_trkIso3IDpTgt2p0", &pho_trkIso3IDpTgt2p0, &b_pho_trkIso3IDpTgt2p0);
    if (t->GetBranch("pho_trkIso3IDpTgt2p0subUE")) t->SetBranchAddress("pho_trkIso3IDpTgt2p0subUE", &pho_trkIso3IDpTgt2p0subUE, &b_pho_trkIso3IDpTgt2p0subUE);

    if (t->GetBranch("pfpIso3subUEfMods")) t->SetBranchAddress("pfpIso3subUEfMods", &pfpIso3subUEfMods, &b_pfpIso3subUEfMods);
    if (t->GetBranch("pfnIso3subUEfMods")) t->SetBranchAddress("pfnIso3subUEfMods", &pfnIso3subUEfMods, &b_pfnIso3subUEfMods);
    if (t->GetBranch("pfcIso3subUEfMods")) t->SetBranchAddress("pfcIso3subUEfMods", &pfcIso3subUEfMods, &b_pfcIso3subUEfMods);
    if (t->GetBranch("pfcIso3pTgt2p0subUEfMods")) t->SetBranchAddress("pfcIso3pTgt2p0subUEfMods", &pfcIso3pTgt2p0subUEfMods, &b_pfcIso3pTgt2p0subUEfMods);

    if (t->GetBranch("pfpIso3subUEcalc")) t->SetBranchAddress("pfpIso3subUEcalc", &pfpIso3subUEcalc, &b_pfpIso3subUEcalc);
    if (t->GetBranch("pfnIso3subUEcalc")) t->SetBranchAddress("pfnIso3subUEcalc", &pfnIso3subUEcalc, &b_pfnIso3subUEcalc);
    if (t->GetBranch("pfcIso3pTgt2p0subUEcalc")) t->SetBranchAddress("pfcIso3pTgt2p0subUEcalc", &pfcIso3pTgt2p0subUEcalc, &b_pfcIso3pTgt2p0subUEcalc);
    if (t->GetBranch("pfcIso3subUEcalc")) t->SetBranchAddress("pfcIso3subUEcalc", &pfcIso3subUEcalc, &b_pfcIso3subUEcalc);
    if (t->GetBranch("pfcIso3IDpTgt2p0subUEcalc")) t->SetBranchAddress("pfcIso3IDpTgt2p0subUEcalc", &pfcIso3IDpTgt2p0subUEcalc, &b_pfcIso3IDpTgt2p0subUEcalc);
    if (t->GetBranch("pfcIso3IDsubUEcalc")) t->SetBranchAddress("pfcIso3IDsubUEcalc", &pfcIso3IDsubUEcalc, &b_pfcIso3IDsubUEcalc);

    if (t->GetBranch("pfpIso3noFPsubUEcalc")) t->SetBranchAddress("pfpIso3noFPsubUEcalc", &pfpIso3noFPsubUEcalc, &b_pfpIso3noFPsubUEcalc);
    if (t->GetBranch("pfnIso3noFPsubUEcalc")) t->SetBranchAddress("pfnIso3noFPsubUEcalc", &pfnIso3noFPsubUEcalc, &b_pfnIso3noFPsubUEcalc);
    if (t->GetBranch("pfcIso3pTgt2p0noFPsubUEcalc")) t->SetBranchAddress("pfcIso3pTgt2p0noFPsubUEcalc", &pfcIso3pTgt2p0noFPsubUEcalc, &b_pfcIso3pTgt2p0noFPsubUEcalc);
    if (t->GetBranch("pfcIso3noFPsubUEcalc")) t->SetBranchAddress("pfcIso3noFPsubUEcalc", &pfcIso3noFPsubUEcalc, &b_pfcIso3noFPsubUEcalc);
    if (t->GetBranch("pfcIso3IDpTgt2p0noFPsubUEcalc")) t->SetBranchAddress("pfcIso3IDpTgt2p0noFPsubUEcalc", &pfcIso3IDpTgt2p0noFPsubUEcalc, &b_pfcIso3IDpTgt2p0noFPsubUEcalc);
    if (t->GetBranch("pfcIso3IDnoFPsubUEcalc")) t->SetBranchAddress("pfcIso3IDnoFPsubUEcalc", &pfcIso3IDnoFPsubUEcalc, &b_pfcIso3IDnoFPsubUEcalc);

    if (t->GetBranch("pfeIso3pTgt2p0subUEcalc")) t->SetBranchAddress("pfeIso3pTgt2p0subUEcalc", &pfeIso3pTgt2p0subUEcalc, &b_pfeIso3pTgt2p0subUEcalc);
    if (t->GetBranch("pfeIso3subUEcalc")) t->SetBranchAddress("pfeIso3subUEcalc", &pfeIso3subUEcalc, &b_pfeIso3subUEcalc);
    if (t->GetBranch("pfeIso3IDpTgt2p0subUEcalc")) t->SetBranchAddress("pfeIso3IDpTgt2p0subUEcalc", &pfeIso3IDpTgt2p0subUEcalc, &b_pfeIso3IDpTgt2p0subUEcalc);
    if (t->GetBranch("pfeIso3IDsubUEcalc")) t->SetBranchAddress("pfeIso3IDsubUEcalc", &pfeIso3IDsubUEcalc, &b_pfeIso3IDsubUEcalc);
    if (t->GetBranch("pfmIso3pTgt2p0subUEcalc")) t->SetBranchAddress("pfmIso3pTgt2p0subUEcalc", &pfmIso3pTgt2p0subUEcalc, &b_pfmIso3pTgt2p0subUEcalc);
    if (t->GetBranch("pfmIso3subUEcalc")) t->SetBranchAddress("pfmIso3subUEcalc", &pfmIso3subUEcalc, &b_pfmIso3subUEcalc);
    if (t->GetBranch("pfmIso3IDpTgt2p0subUEcalc")) t->SetBranchAddress("pfmIso3IDpTgt2p0subUEcalc", &pfmIso3IDpTgt2p0subUEcalc, &b_pfmIso3IDpTgt2p0subUEcalc);
    if (t->GetBranch("pfmIso3IDsubUEcalc")) t->SetBranchAddress("pfmIso3IDsubUEcalc", &pfmIso3IDsubUEcalc, &b_pfmIso3IDsubUEcalc);

    if (t->GetBranch("pfeIso3pTgt2p0noFPsubUEcalc")) t->SetBranchAddress("pfeIso3pTgt2p0noFPsubUEcalc", &pfeIso3pTgt2p0noFPsubUEcalc, &b_pfeIso3pTgt2p0noFPsubUEcalc);
    if (t->GetBranch("pfeIso3noFPsubUEcalc")) t->SetBranchAddress("pfeIso3noFPsubUEcalc", &pfeIso3noFPsubUEcalc, &b_pfeIso3noFPsubUEcalc);
    if (t->GetBranch("pfeIso3IDpTgt2p0noFPsubUEcalc")) t->SetBranchAddress("pfeIso3IDpTgt2p0noFPsubUEcalc", &pfeIso3IDpTgt2p0noFPsubUEcalc, &b_pfeIso3IDpTgt2p0noFPsubUEcalc);
    if (t->GetBranch("pfeIso3IDnoFPsubUEcalc")) t->SetBranchAddress("pfeIso3IDnoFPsubUEcalc", &pfeIso3IDnoFPsubUEcalc, &b_pfeIso3IDnoFPsubUEcalc);
    if (t->GetBranch("pfmIso3pTgt2p0noFPsubUEcalc")) t->SetBranchAddress("pfmIso3pTgt2p0noFPsubUEcalc", &pfmIso3pTgt2p0noFPsubUEcalc, &b_pfmIso3pTgt2p0noFPsubUEcalc);
    if (t->GetBranch("pfmIso3noFPsubUEcalc")) t->SetBranchAddress("pfmIso3noFPsubUEcalc", &pfmIso3noFPsubUEcalc, &b_pfmIso3noFPsubUEcalc);
    if (t->GetBranch("pfmIso3IDpTgt2p0noFPsubUEcalc")) t->SetBranchAddress("pfmIso3IDpTgt2p0noFPsubUEcalc", &pfmIso3IDpTgt2p0noFPsubUEcalc, &b_pfmIso3IDpTgt2p0noFPsubUEcalc);
    if (t->GetBranch("pfmIso3IDnoFPsubUEcalc")) t->SetBranchAddress("pfmIso3IDnoFPsubUEcalc", &pfmIso3IDnoFPsubUEcalc, &b_pfmIso3IDnoFPsubUEcalc);

    if (t->GetBranch("pfCSpIso3subUEcalc")) t->SetBranchAddress("pfCSpIso3subUEcalc", &pfCSpIso3subUEcalc, &b_pfCSpIso3subUEcalc);
    if (t->GetBranch("pfCSnIso3subUEcalc")) t->SetBranchAddress("pfCSnIso3subUEcalc", &pfCSnIso3subUEcalc, &b_pfCSnIso3subUEcalc);
    if (t->GetBranch("pfCScIso3pTgt2p0subUEcalc")) t->SetBranchAddress("pfCScIso3pTgt2p0subUEcalc", &pfCScIso3pTgt2p0subUEcalc, &b_pfCScIso3pTgt2p0subUEcalc);
    if (t->GetBranch("pfCScIso3subUEcalc")) t->SetBranchAddress("pfCScIso3subUEcalc", &pfCScIso3subUEcalc, &b_pfCScIso3subUEcalc);

    if (t->GetBranch("pfpIso3subUEvn2")) t->SetBranchAddress("pfpIso3subUEvn2", &pfpIso3subUEvn2, &b_pfpIso3subUEvn2);
    if (t->GetBranch("pfnIso3subUEvn2")) t->SetBranchAddress("pfnIso3subUEvn2", &pfnIso3subUEvn2, &b_pfnIso3subUEvn2);
    if (t->GetBranch("pfcIso3pTgt2p0subUEvn2")) t->SetBranchAddress("pfcIso3pTgt2p0subUEvn2", &pfcIso3pTgt2p0subUEvn2, &b_pfcIso3pTgt2p0subUEvn2);
    if (t->GetBranch("pfcIso3subUEvn2")) t->SetBranchAddress("pfcIso3subUEvn2", &pfcIso3subUEvn2, &b_pfcIso3subUEvn2);

    if (t->GetBranch("pfpIso3subUEphi0vn2")) t->SetBranchAddress("pfpIso3subUEphi0vn2", &pfpIso3subUEphi0vn2, &b_pfpIso3subUEphi0vn2);
    if (t->GetBranch("pfnIso3subUEphi0vn2")) t->SetBranchAddress("pfnIso3subUEphi0vn2", &pfnIso3subUEphi0vn2, &b_pfnIso3subUEphi0vn2);
    if (t->GetBranch("pfcIso3pTgt2p0subUEphi0vn2")) t->SetBranchAddress("pfcIso3pTgt2p0subUEphi0vn2", &pfcIso3pTgt2p0subUEphi0vn2, &b_pfcIso3pTgt2p0subUEphi0vn2);
    if (t->GetBranch("pfcIso3subUEphi0vn2")) t->SetBranchAddress("pfcIso3subUEphi0vn2", &pfcIso3subUEphi0vn2, &b_pfcIso3subUEphi0vn2);

    if (t->GetBranch("pfpIso3subUEtrkvn3")) t->SetBranchAddress("pfpIso3subUEtrkvn3", &pfpIso3subUEtrkvn3, &b_pfpIso3subUEtrkvn3);
    if (t->GetBranch("pfnIso3subUEtrkvn3")) t->SetBranchAddress("pfnIso3subUEtrkvn3", &pfnIso3subUEtrkvn3, &b_pfnIso3subUEtrkvn3);
    if (t->GetBranch("pfcIso3pTgt2p0subUEtrkvn3")) t->SetBranchAddress("pfcIso3pTgt2p0subUEtrkvn3", &pfcIso3pTgt2p0subUEtrkvn3, &b_pfcIso3pTgt2p0subUEtrkvn3);
    if (t->GetBranch("pfcIso3subUEtrkvn3")) t->SetBranchAddress("pfcIso3subUEtrkvn3", &pfcIso3subUEtrkvn3, &b_pfcIso3subUEtrkvn3);

    if (t->GetBranch("trkIso3")) t->SetBranchAddress("trkIso3", &trkIso3, &b_trkIso3);
    if (t->GetBranch("trkIso3subUE")) t->SetBranchAddress("trkIso3subUE", &trkIso3subUE, &b_trkIso3subUE);
    if (t->GetBranch("trkIso3ID")) t->SetBranchAddress("trkIso3ID", &trkIso3ID, &b_trkIso3ID);
    if (t->GetBranch("trkIso3IDsubUE")) t->SetBranchAddress("trkIso3IDsubUE", &trkIso3IDsubUE, &b_trkIso3IDsubUE);
    if (t->GetBranch("trkIso3IDmatchPF")) t->SetBranchAddress("trkIso3IDmatchPF", &trkIso3IDmatchPF, &b_trkIso3IDmatchPF);
    if (t->GetBranch("phoEAc")) t->SetBranchAddress("phoEAc", &phoEAc, &b_phoEAc);
    if (t->GetBranch("phoEAp")) t->SetBranchAddress("phoEAp", &phoEAp, &b_phoEAp);
    if (t->GetBranch("phoEAn")) t->SetBranchAddress("phoEAn", &phoEAn, &b_phoEAn);
    if (t->GetBranch("phoEAhoe")) t->SetBranchAddress("phoEAhoe", &phoEAhoe, &b_phoEAhoe);
    if (t->GetBranch("phoEAsieie")) t->SetBranchAddress("phoEAsieie", &phoEAsieie, &b_phoEAsieie);

    if (t->GetBranch("nPhoPFp")) t->SetBranchAddress("nPhoPFp", &nPhoPFp, &b_nPhoPFp);
    if (t->GetBranch("ppfpPtSum")) t->SetBranchAddress("ppfpPtSum", &ppfpPtSum, &b_ppfpPtSum);
    if (t->GetBranch("ppfpEtaAve")) t->SetBranchAddress("ppfpEtaAve", &ppfpEtaAve, &b_ppfpEtaAve);
    if (t->GetBranch("ppfpPhiAve")) t->SetBranchAddress("ppfpPhiAve", &ppfpPhiAve, &b_ppfpPhiAve);
    if (t->GetBranch("nPhoPFn")) t->SetBranchAddress("nPhoPFn", &nPhoPFn, &b_nPhoPFn);
    if (t->GetBranch("ppfnPtSum")) t->SetBranchAddress("ppfnPtSum", &ppfnPtSum, &b_ppfnPtSum);
    if (t->GetBranch("ppfnEtaAve")) t->SetBranchAddress("ppfnEtaAve", &ppfnEtaAve, &b_ppfnEtaAve);
    if (t->GetBranch("ppfnPhiAve")) t->SetBranchAddress("ppfnPhiAve", &ppfnPhiAve, &b_ppfnPhiAve);
    if (t->GetBranch("nPhoPFc")) t->SetBranchAddress("nPhoPFc", &nPhoPFc, &b_nPhoPFc);
    if (t->GetBranch("ppfcPtSum")) t->SetBranchAddress("ppfcPtSum", &ppfcPtSum, &b_ppfcPtSum);
    if (t->GetBranch("ppfcEtaAve")) t->SetBranchAddress("ppfcEtaAve", &ppfcEtaAve, &b_ppfcEtaAve);
    if (t->GetBranch("ppfcPhiAve")) t->SetBranchAddress("ppfcPhiAve", &ppfcPhiAve, &b_ppfcPhiAve);
    if (t->GetBranch("ppfcTrkID")) t->SetBranchAddress("ppfcTrkID", &ppfcTrkID, &b_ppfcTrkID);
    if (t->GetBranch("nPhoPFe")) t->SetBranchAddress("nPhoPFe", &nPhoPFe, &b_nPhoPFe);
    if (t->GetBranch("ppfePtSum")) t->SetBranchAddress("ppfePtSum", &ppfePtSum, &b_ppfePtSum);
    if (t->GetBranch("ppfeEtaAve")) t->SetBranchAddress("ppfeEtaAve", &ppfeEtaAve, &b_ppfeEtaAve);
    if (t->GetBranch("ppfePhiAve")) t->SetBranchAddress("ppfePhiAve", &ppfePhiAve, &b_ppfePhiAve);
    if (t->GetBranch("ppfeTrkID")) t->SetBranchAddress("ppfeTrkID", &ppfeTrkID, &b_ppfeTrkID);
    if (t->GetBranch("nPhoPFm")) t->SetBranchAddress("nPhoPFm", &nPhoPFm, &b_nPhoPFm);
    if (t->GetBranch("ppfmPtSum")) t->SetBranchAddress("ppfmPtSum", &ppfmPtSum, &b_ppfmPtSum);
    if (t->GetBranch("ppfmEtaAve")) t->SetBranchAddress("ppfmEtaAve", &ppfmEtaAve, &b_ppfmEtaAve);
    if (t->GetBranch("ppfmPhiAve")) t->SetBranchAddress("ppfmPhiAve", &ppfmPhiAve, &b_ppfmPhiAve);
    if (t->GetBranch("ppfmTrkID")) t->SetBranchAddress("ppfmTrkID", &ppfmTrkID, &b_ppfmTrkID);
    if (t->GetBranch("nPhoPFx")) t->SetBranchAddress("nPhoPFx", &nPhoPFx, &b_nPhoPFx);
    if (t->GetBranch("ppfxPtSum")) t->SetBranchAddress("ppfxPtSum", &ppfxPtSum, &b_ppfxPtSum);
    if (t->GetBranch("ppfxEtaAve")) t->SetBranchAddress("ppfxEtaAve", &ppfxEtaAve, &b_ppfxEtaAve);
    if (t->GetBranch("ppfxPhiAve")) t->SetBranchAddress("ppfxPhiAve", &ppfxPhiAve, &b_ppfxPhiAve);

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

    if (t->GetBranch("mgPDG")) t->SetBranchAddress("mgPDG", &mgPDG, &b_mgPDG);
    if (t->GetBranch("mgPt")) t->SetBranchAddress("mgPt", &mgPt, &b_mgPt);
    if (t->GetBranch("mgEta")) t->SetBranchAddress("mgEta", &mgEta, &b_mgEta);
    if (t->GetBranch("mgPhi")) t->SetBranchAddress("mgPhi", &mgPhi, &b_mgPhi);
    if (t->GetBranch("mgIdx")) t->SetBranchAddress("mgIdx", &mgIdx, &b_mgIdx);
    if (t->GetBranch("mgPhoPt")) t->SetBranchAddress("mgPhoPt", &mgPhoPt, &b_mgPhoPt);
    if (t->GetBranch("mgPhoEta")) t->SetBranchAddress("mgPhoEta", &mgPhoEta, &b_mgPhoEta);
    if (t->GetBranch("mgPhoPhi")) t->SetBranchAddress("mgPhoPhi", &mgPhoPhi, &b_mgPhoPhi);
    if (t->GetBranch("mgPhoIdx")) t->SetBranchAddress("mgPhoIdx", &mgPhoIdx, &b_mgPhoIdx);
    if (t->GetBranch("mgElePt")) t->SetBranchAddress("mgElePt", &mgElePt, &b_mgElePt);
    if (t->GetBranch("mgEleEta")) t->SetBranchAddress("mgEleEta", &mgEleEta, &b_mgEleEta);
    if (t->GetBranch("mgElePhi")) t->SetBranchAddress("mgElePhi", &mgElePhi, &b_mgElePhi);
    if (t->GetBranch("mgEleIdx")) t->SetBranchAddress("mgEleIdx", &mgEleIdx, &b_mgEleIdx);
    if (t->GetBranch("mgMuoPt")) t->SetBranchAddress("mgMuoPt", &mgMuoPt, &b_mgMuoPt);
    if (t->GetBranch("mgMuoEta")) t->SetBranchAddress("mgMuoEta", &mgMuoEta, &b_mgMuoEta);
    if (t->GetBranch("mgMuoPhi")) t->SetBranchAddress("mgMuoPhi", &mgMuoPhi, &b_mgMuoPhi);
    if (t->GetBranch("mgMuoIdx")) t->SetBranchAddress("mgMuoIdx", &mgMuoIdx, &b_mgMuoIdx);

    if (t->GetBranch("matchPFid")) t->SetBranchAddress("matchPFid", &matchPFid, &b_matchPFid);
    if (t->GetBranch("matchPFpt")) t->SetBranchAddress("matchPFpt", &matchPFpt, &b_matchPFpt);
    if (t->GetBranch("matchPFeta")) t->SetBranchAddress("matchPFeta", &matchPFeta, &b_matchPFeta);
    if (t->GetBranch("matchPFphi")) t->SetBranchAddress("matchPFphi", &matchPFphi, &b_matchPFphi);
    if (t->GetBranch("matchPFidx")) t->SetBranchAddress("matchPFidx", &matchPFidx, &b_matchPFidx);
    if (t->GetBranch("matchPFphoPt")) t->SetBranchAddress("matchPFphoPt", &matchPFphoPt, &b_matchPFphoPt);
    if (t->GetBranch("matchPFphoEta")) t->SetBranchAddress("matchPFphoEta", &matchPFphoEta, &b_matchPFphoEta);
    if (t->GetBranch("matchPFphoPhi")) t->SetBranchAddress("matchPFphoPhi", &matchPFphoPhi, &b_matchPFphoPhi);
    if (t->GetBranch("matchPFphoIdx")) t->SetBranchAddress("matchPFphoIdx", &matchPFphoIdx, &b_matchPFphoIdx);
    if (t->GetBranch("matchPFelePt")) t->SetBranchAddress("matchPFelePt", &matchPFelePt, &b_matchPFelePt);
    if (t->GetBranch("matchPFeleEta")) t->SetBranchAddress("matchPFeleEta", &matchPFeleEta, &b_matchPFeleEta);
    if (t->GetBranch("matchPFelePhi")) t->SetBranchAddress("matchPFelePhi", &matchPFelePhi, &b_matchPFelePhi);
    if (t->GetBranch("matchPFeleIdx")) t->SetBranchAddress("matchPFeleIdx", &matchPFeleIdx, &b_matchPFeleIdx);
    if (t->GetBranch("matchPFmuoPt")) t->SetBranchAddress("matchPFmuoPt", &matchPFmuoPt, &b_matchPFmuoPt);
    if (t->GetBranch("matchPFmuoEta")) t->SetBranchAddress("matchPFmuoEta", &matchPFmuoEta, &b_matchPFmuoEta);
    if (t->GetBranch("matchPFmuoPhi")) t->SetBranchAddress("matchPFmuoPhi", &matchPFmuoPhi, &b_matchPFmuoPhi);
    if (t->GetBranch("matchPFmuoIdx")) t->SetBranchAddress("matchPFmuoIdx", &matchPFmuoIdx, &b_matchPFmuoIdx);
}

void ggHiFlat::setupTreeForWriting(TTree* t)
{
    t->Branch("weight", &weight);
    t->Branch("weightCent", &weightCent);
    t->Branch("weightKin", &weightKin);
    t->Branch("weightGenKin", &weightGenKin);
    t->Branch("weightPthat", &weightPthat);
    t->Branch("phi0", &phi0);
    t->Branch("pthat", &pthat);
    t->Branch("processID", &processID);
    t->Branch("hiBin", &hiBin);
    t->Branch("hiHF", &hiHF);
    t->Branch("hiEP1HF", &hiEP1HF);
    t->Branch("hiEP2HF", &hiEP2HF);
    t->Branch("hiEP3HF", &hiEP3HF);
    t->Branch("hiEP4HF", &hiEP4HF);

    t->Branch("angEP2pf", &angEP2pf_out);
    t->Branch("angEP3pf", &angEP3pf_out);
    t->Branch("angEP2trk", &angEP2trk);
    t->Branch("angEP3trk", &angEP3trk);

    if (false) {
        t->Branch("partphi_nBins", &partphi_nBins);
        t->Branch("partphi_minContent", &partphi_minContent);
        t->Branch("partphi_binContents", &partphi_binContents_out);
        t->Branch("fit_v2", &fit_v2);
        t->Branch("fit_v3", &fit_v3);
        t->Branch("fit_chi2", &fit_chi2);
        t->Branch("fit_chi2prob", &fit_chi2prob);
    }

    t->Branch("fit_EPphi0_v2", &fit_EPphi0_v2);
    t->Branch("fit_EPphi0_chi2", &fit_EPphi0_chi2);
    t->Branch("fit_EPphi0_chi2prob", &fit_EPphi0_chi2prob);

    t->Branch("trkphi_nBins", &trkphi_nBins);
    t->Branch("trkphi_minContent", &trkphi_minContent);
    t->Branch("trkphi_binContents", &trkphi_binContents_out);
    t->Branch("fit_trkphi_v2", &fit_trkphi_v2);
    t->Branch("fit_trkphi_v3", &fit_trkphi_v3);
    t->Branch("fit_trkphi_chi2", &fit_trkphi_chi2);
    t->Branch("fit_trkphi_chi2prob", &fit_trkphi_chi2prob);

    t->Branch("fMods_th1_yMin", &fMods_th1_yMin_out);
    t->Branch("fMods_angEP2", &fMods_angEP2_out);
    t->Branch("fMods_angEP3", &fMods_angEP3_out);
    t->Branch("fMods_v2", &fMods_v2_out);
    t->Branch("fMods_v3", &fMods_v3_out);
    t->Branch("fMods_chi2", &fMods_chi2_out);
    t->Branch("fMods_chi2prob", &fMods_chi2prob_out);

    t->Branch("rho", &rho);
    t->Branch("angEvtPlane", &angEvtPlane);
    t->Branch("indexEvtPlane", &indexEvtPlane);
    t->Branch("phi_nTot", &phi_nTot);
    t->Branch("phi_minBinN", &phi_minBinN);
    t->Branch("phi_fit_chi2", &phi_fit_chi2);
    t->Branch("phi_fit_chi2prob", &phi_fit_chi2prob);
    t->Branch("phi_fit_v2", &phi_fit_v2);
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

        t->Branch("pho_trkIso3pTgt2p0", &pho_trkIso3pTgt2p0);
        t->Branch("pho_trkIso3pTgt2p0subUE", &pho_trkIso3pTgt2p0subUE);
        t->Branch("pho_trkIso3IDpTgt2p0", &pho_trkIso3IDpTgt2p0);
        t->Branch("pho_trkIso3IDpTgt2p0subUE", &pho_trkIso3IDpTgt2p0subUE);

        t->Branch("pfpIso3subUEfMods", &pfpIso3subUEfMods_out);
        t->Branch("pfnIso3subUEfMods", &pfnIso3subUEfMods_out);
        t->Branch("pfcIso3subUEfMods", &pfcIso3subUEfMods_out);
        t->Branch("pfcIso3pTgt2p0subUEfMods", &pfcIso3pTgt2p0subUEfMods_out);

        t->Branch("pfpIso3subUEcalc", &pfpIso3subUEcalc);
        t->Branch("pfnIso3subUEcalc", &pfnIso3subUEcalc);
        t->Branch("pfcIso3pTgt2p0subUEcalc", &pfcIso3pTgt2p0subUEcalc);
        t->Branch("pfcIso3subUEcalc", &pfcIso3subUEcalc);
        t->Branch("pfcIso3IDpTgt2p0subUEcalc", &pfcIso3IDpTgt2p0subUEcalc);
        t->Branch("pfcIso3IDsubUEcalc", &pfcIso3IDsubUEcalc);

        t->Branch("pfpIso3noFPsubUEcalc", &pfpIso3noFPsubUEcalc);
        t->Branch("pfnIso3noFPsubUEcalc", &pfnIso3noFPsubUEcalc);
        t->Branch("pfcIso3pTgt2p0noFPsubUEcalc", &pfcIso3pTgt2p0noFPsubUEcalc);
        t->Branch("pfcIso3noFPsubUEcalc", &pfcIso3noFPsubUEcalc);
        t->Branch("pfcIso3IDpTgt2p0noFPsubUEcalc", &pfcIso3IDpTgt2p0noFPsubUEcalc);
        t->Branch("pfcIso3IDnoFPsubUEcalc", &pfcIso3IDnoFPsubUEcalc);

        t->Branch("pfeIso3pTgt2p0subUEcalc", &pfeIso3pTgt2p0subUEcalc);
        t->Branch("pfeIso3subUEcalc", &pfeIso3subUEcalc);
        t->Branch("pfeIso3IDpTgt2p0subUEcalc", &pfeIso3IDpTgt2p0subUEcalc);
        t->Branch("pfeIso3IDsubUEcalc", &pfeIso3IDsubUEcalc);
        t->Branch("pfmIso3pTgt2p0subUEcalc", &pfmIso3pTgt2p0subUEcalc);
        t->Branch("pfmIso3subUEcalc", &pfmIso3subUEcalc);
        t->Branch("pfmIso3IDpTgt2p0subUEcalc", &pfmIso3IDpTgt2p0subUEcalc);
        t->Branch("pfmIso3IDsubUEcalc", &pfmIso3IDsubUEcalc);

        t->Branch("pfeIso3pTgt2p0noFPsubUEcalc", &pfeIso3pTgt2p0noFPsubUEcalc);
        t->Branch("pfeIso3noFPsubUEcalc", &pfeIso3noFPsubUEcalc);
        t->Branch("pfeIso3IDpTgt2p0noFPsubUEcalc", &pfeIso3IDpTgt2p0noFPsubUEcalc);
        t->Branch("pfeIso3IDnoFPsubUEcalc", &pfeIso3IDnoFPsubUEcalc);
        t->Branch("pfmIso3pTgt2p0noFPsubUEcalc", &pfmIso3pTgt2p0noFPsubUEcalc);
        t->Branch("pfmIso3noFPsubUEcalc", &pfmIso3noFPsubUEcalc);
        t->Branch("pfmIso3IDpTgt2p0noFPsubUEcalc", &pfmIso3IDpTgt2p0noFPsubUEcalc);
        t->Branch("pfmIso3IDnoFPsubUEcalc", &pfmIso3IDnoFPsubUEcalc);

        t->Branch("pfCSpIso3subUEcalc", &pfCSpIso3subUEcalc);
        t->Branch("pfCSnIso3subUEcalc", &pfCSnIso3subUEcalc);
        t->Branch("pfCScIso3pTgt2p0subUEcalc", &pfCScIso3pTgt2p0subUEcalc);
        t->Branch("pfCScIso3subUEcalc", &pfCScIso3subUEcalc);

        t->Branch("pfpIso3subUEvn2", &pfpIso3subUEvn2);
        t->Branch("pfnIso3subUEvn2", &pfnIso3subUEvn2);
        t->Branch("pfcIso3pTgt2p0subUEvn2", &pfcIso3pTgt2p0subUEvn2);
        t->Branch("pfcIso3subUEvn2", &pfcIso3subUEvn2);

        t->Branch("pfpIso3subUEphi0vn2", &pfpIso3subUEphi0vn2);
        t->Branch("pfnIso3subUEphi0vn2", &pfnIso3subUEphi0vn2);
        t->Branch("pfcIso3pTgt2p0subUEphi0vn2", &pfcIso3pTgt2p0subUEphi0vn2);
        t->Branch("pfcIso3subUEphi0vn2", &pfcIso3subUEphi0vn2);

        t->Branch("pfpIso3subUEtrkvn3", &pfpIso3subUEtrkvn3);
        t->Branch("pfnIso3subUEtrkvn3", &pfnIso3subUEtrkvn3);
        t->Branch("pfcIso3pTgt2p0subUEtrkvn3", &pfcIso3pTgt2p0subUEtrkvn3);
        t->Branch("pfcIso3subUEtrkvn3", &pfcIso3subUEtrkvn3);

        t->Branch("trkIso3", &trkIso3);
        t->Branch("trkIso3subUE", &trkIso3subUE);
        t->Branch("trkIso3ID", &trkIso3ID);
        t->Branch("trkIso3IDsubUE", &trkIso3IDsubUE);
        t->Branch("trkIso3IDmatchPF", &trkIso3IDmatchPF);
        t->Branch("phoEAc", &phoEAc);
        t->Branch("phoEAp", &phoEAp);
        t->Branch("phoEAn", &phoEAn);
        t->Branch("phoEAhoe", &phoEAhoe);
        t->Branch("phoEAsieie", &phoEAsieie);

        t->Branch("nPhoPFp", &nPhoPFp);
        t->Branch("ppfpPtSum", &ppfpPtSum);
        t->Branch("ppfpEtaAve", &ppfpEtaAve);
        t->Branch("ppfpPhiAve", &ppfpPhiAve);
        t->Branch("nPhoPFn", &nPhoPFn);
        t->Branch("ppfnPtSum", &ppfnPtSum);
        t->Branch("ppfnEtaAve", &ppfnEtaAve);
        t->Branch("ppfnPhiAve", &ppfnPhiAve);
        t->Branch("nPhoPFc", &nPhoPFc);
        t->Branch("ppfcPtSum", &ppfcPtSum);
        t->Branch("ppfcEtaAve", &ppfcEtaAve);
        t->Branch("ppfcPhiAve", &ppfcPhiAve);
        t->Branch("ppfcTrkID", &ppfcTrkID);
        t->Branch("nPhoPFe", &nPhoPFe);
        t->Branch("ppfePtSum", &ppfePtSum);
        t->Branch("ppfeEtaAve", &ppfeEtaAve);
        t->Branch("ppfePhiAve", &ppfePhiAve);
        t->Branch("ppfeTrkID", &ppfeTrkID);
        t->Branch("nPhoPFm", &nPhoPFm);
        t->Branch("ppfmPtSum", &ppfmPtSum);
        t->Branch("ppfmEtaAve", &ppfmEtaAve);
        t->Branch("ppfmPhiAve", &ppfmPhiAve);
        t->Branch("ppfmTrkID", &ppfmTrkID);
        t->Branch("nPhoPFx", &nPhoPFx);
        t->Branch("ppfxPtSum", &ppfxPtSum);
        t->Branch("ppfxEtaAve", &ppfxEtaAve);
        t->Branch("ppfxPhiAve", &ppfxPhiAve);
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
    if (doMatchGen) {
        t->Branch("mgPDG", &mgPDG);
        t->Branch("mgPt", &mgPt);
        t->Branch("mgEta", &mgEta);
        t->Branch("mgPhi", &mgPhi);
        t->Branch("mgIdx", &mgIdx);
        t->Branch("mgPhoPt", &mgPhoPt);
        t->Branch("mgPhoEta", &mgPhoEta);
        t->Branch("mgPhoPhi", &mgPhoPhi);
        t->Branch("mgPhoIdx", &mgPhoIdx);
        t->Branch("mgElePt", &mgElePt);
        t->Branch("mgEleEta", &mgEleEta);
        t->Branch("mgElePhi", &mgElePhi);
        t->Branch("mgEleIdx", &mgEleIdx);
        t->Branch("mgMuoPt", &mgMuoPt);
        t->Branch("mgMuoEta", &mgMuoEta);
        t->Branch("mgMuoPhi", &mgMuoPhi);
        t->Branch("mgMuoIdx", &mgMuoIdx);
    }
    if (doMatchPF) {
        t->Branch("matchPFid", &matchPFid);
        t->Branch("matchPFpt", &matchPFpt);
        t->Branch("matchPFeta", &matchPFeta);
        t->Branch("matchPFphi", &matchPFphi);
        t->Branch("matchPFidx", &matchPFidx);
        t->Branch("matchPFphoPt", &matchPFphoPt);
        t->Branch("matchPFphoEta", &matchPFphoEta);
        t->Branch("matchPFphoPhi", &matchPFphoPhi);
        t->Branch("matchPFphoIdx", &matchPFphoIdx);
        t->Branch("matchPFelePt", &matchPFelePt);
        t->Branch("matchPFeleEta", &matchPFeleEta);
        t->Branch("matchPFelePhi", &matchPFelePhi);
        t->Branch("matchPFeleIdx", &matchPFeleIdx);
        t->Branch("matchPFmuoPt", &matchPFmuoPt);
        t->Branch("matchPFmuoEta", &matchPFmuoEta);
        t->Branch("matchPFmuoPhi", &matchPFmuoPhi);
        t->Branch("matchPFmuoIdx", &matchPFmuoIdx);
    }
}

void ggHiFlat::clearEntry()
{
    weight = -1;
    weightCent = -1;
    weightKin = -1;
    weightGenKin = -1;
    weightPthat = -1;
    phi0 = -987987;
    pthat = -1;
    processID = -999999;
    hiBin = -1;
    hiHF = -1;
    hiEP1HF = -987987;
    hiEP2HF = -987987;
    hiEP3HF = -987987;
    hiEP4HF = -987987;

    angEP2pf_out.clear();
    angEP3pf_out.clear();
    angEP2trk = -987987;
    angEP3trk = -987987;

    if (false) {
        partphi_nBins = 0;
        partphi_minContent = 0;
        partphi_binContents_out.clear();
        fit_v2 = 0;
        fit_v3 = 0;
        fit_chi2 = -987987;
        fit_chi2prob = -987987;
    }

    fit_EPphi0_v2 = 0;
    fit_EPphi0_chi2 = -987987;
    fit_EPphi0_chi2prob = -987987;

    trkphi_nBins = 0;
    trkphi_minContent = 0;
    trkphi_binContents_out.clear();
    fit_trkphi_v2 = 0;
    fit_trkphi_v3 = 0;
    fit_trkphi_chi2 = -987987;
    fit_trkphi_chi2prob = -987987;

    fMods_th1_yMin_out.clear();
    fMods_angEP2_out.clear();
    fMods_angEP3_out.clear();
    fMods_v2_out.clear();
    fMods_v3_out.clear();
    fMods_chi2_out.clear();
    fMods_chi2prob_out.clear();

    rho = -1;
    angEvtPlane = -999888;
    indexEvtPlane = -1;
    phi_nTot = 0;
    phi_minBinN = 999999;
    phi_fit_chi2 = -1;
    phi_fit_chi2prob = -1;
    phi_fit_v2 = -999777;
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

        pho_trkIso3pTgt2p0 = -987987;
        pho_trkIso3pTgt2p0subUE = -987987;
        pho_trkIso3IDpTgt2p0 = -987987;
        pho_trkIso3IDpTgt2p0subUE = -987987;

        pfpIso3subUEfMods_out.clear();
        pfnIso3subUEfMods_out.clear();
        pfcIso3subUEfMods_out.clear();
        pfcIso3pTgt2p0subUEfMods_out.clear();

        pfpIso3subUEcalc = -987987;
        pfnIso3subUEcalc = -987987;
        pfcIso3pTgt2p0subUEcalc = -987987;
        pfcIso3subUEcalc = -987987;
        pfcIso3IDpTgt2p0subUEcalc = -987987;
        pfcIso3IDsubUEcalc = -987987;

        pfpIso3noFPsubUEcalc = -987987;
        pfnIso3noFPsubUEcalc = -987987;
        pfcIso3pTgt2p0noFPsubUEcalc = -987987;
        pfcIso3noFPsubUEcalc = -987987;
        pfcIso3IDpTgt2p0noFPsubUEcalc = -987987;
        pfcIso3IDnoFPsubUEcalc = -987987;

        pfeIso3pTgt2p0subUEcalc = -987987;
        pfeIso3subUEcalc = -987987;
        pfeIso3IDpTgt2p0subUEcalc = -987987;
        pfeIso3IDsubUEcalc = -987987;
        pfmIso3pTgt2p0subUEcalc = -987987;
        pfmIso3subUEcalc = -987987;
        pfmIso3IDpTgt2p0subUEcalc = -987987;
        pfmIso3IDsubUEcalc = -987987;

        pfeIso3pTgt2p0noFPsubUEcalc = -987987;
        pfeIso3noFPsubUEcalc = -987987;
        pfeIso3IDpTgt2p0noFPsubUEcalc = -987987;
        pfeIso3IDnoFPsubUEcalc = -987987;
        pfmIso3pTgt2p0noFPsubUEcalc = -987987;
        pfmIso3noFPsubUEcalc = -987987;
        pfmIso3IDpTgt2p0noFPsubUEcalc = -987987;
        pfmIso3IDnoFPsubUEcalc = -987987;

        pfCSpIso3subUEcalc = -987987;
        pfCSnIso3subUEcalc = -987987;
        pfCScIso3pTgt2p0subUEcalc = -987987;
        pfCScIso3subUEcalc = -987987;

        pfpIso3subUEvn2 = -987987;
        pfnIso3subUEvn2 = -987987;
        pfcIso3pTgt2p0subUEvn2 = -987987;
        pfcIso3subUEvn2 = -987987;

        pfpIso3subUEphi0vn2 = -987987;
        pfnIso3subUEphi0vn2 = -987987;
        pfcIso3pTgt2p0subUEphi0vn2 = -987987;
        pfcIso3subUEphi0vn2 = -987987;

        pfpIso3subUEtrkvn3 = -987987;
        pfnIso3subUEtrkvn3 = -987987;
        pfcIso3pTgt2p0subUEtrkvn3 = -987987;
        pfcIso3subUEtrkvn3 = -987987;

        trkIso3 = -987987;
        trkIso3subUE = -987987;
        trkIso3ID = -987987;
        trkIso3IDsubUE = -987987;
        trkIso3IDmatchPF = -987987;
        phoEAc = 0;
        phoEAp = 0;
        phoEAn = 0;
        phoEAhoe = 0;
        phoEAsieie = 0;

        nPhoPFp = 0;
        ppfpPtSum = -1;
        ppfpEtaAve = -987987;
        ppfpPhiAve = -987987;
        nPhoPFn = 0;
        ppfnPtSum = -1;
        ppfnEtaAve = -987987;
        ppfnPhiAve = -987987;
        nPhoPFc = 0;
        ppfcPtSum = -1;
        ppfcEtaAve = -987987;
        ppfcPhiAve = -987987;
        ppfcTrkID = -1;
        nPhoPFe = 0;
        ppfePtSum = -1;
        ppfeEtaAve = -987987;
        ppfePhiAve = -987987;
        ppfeTrkID = -1;
        nPhoPFm = 0;
        ppfmPtSum = -1;
        ppfmEtaAve = -987987;
        ppfmPhiAve = -987987;
        ppfmTrkID = -1;
        nPhoPFx = 0;
        ppfxPtSum = -1;
        ppfxEtaAve = -987987;
        ppfxPhiAve = -987987;
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

void ggHiFlat::clearEntryMatchedGen()
{
    if (doMatchGen) {
        mgPDG = 0;
        mgPt = -1;
        mgEta = -987987;
        mgPhi = -987987;
        mgIdx = -1;
        mgPhoPt = -1;
        mgPhoEta = -987987;
        mgPhoPhi = -987987;
        mgPhoIdx = -1;
        mgElePt = -1;
        mgEleEta = -987987;
        mgElePhi = -987987;
        mgEleIdx = -1;
        mgMuoPt = -1;
        mgMuoEta = -987987;
        mgMuoPhi = -987987;
        mgMuoIdx = -1;
    }
}

void ggHiFlat::clearEntryMatchedPF()
{
    if (doMatchPF) {
        matchPFid = 0;
        matchPFpt = -1;
        matchPFeta = -987987;
        matchPFphi = -987987;
        matchPFidx = -1;
        matchPFphoPt = -1;
        matchPFphoEta = -987987;
        matchPFphoPhi = -987987;
        matchPFphoIdx = -1;
        matchPFelePt = -1;
        matchPFeleEta = -987987;
        matchPFelePhi = -987987;
        matchPFeleIdx = -1;
        matchPFmuoPt = -1;
        matchPFmuoEta = -987987;
        matchPFmuoPhi = -987987;
        matchPFmuoIdx = -1;
    }
}

void ggHiFlat::resetPhoPF()
{
    nPhoPFp = 0;
    ppfpPtSum = 0;
    ppfpEtaAve = 0;
    ppfpPhiAve = 0;
    nPhoPFn = 0;
    ppfnPtSum = 0;
    ppfnEtaAve = 0;
    ppfnPhiAve = 0;
    nPhoPFc = 0;
    ppfcPtSum = 0;
    ppfcEtaAve = 0;
    ppfcPhiAve = 0;
    ppfcTrkID = 0;
    nPhoPFe = 0;
    ppfePtSum = 0;
    ppfeEtaAve = 0;
    ppfePhiAve = 0;
    ppfeTrkID = 0;
    nPhoPFm = 0;
    ppfmPtSum = 0;
    ppfmEtaAve = 0;
    ppfmPhiAve = 0;
    ppfmTrkID = 0;
    nPhoPFx = 0;
    ppfxPtSum = 0;
    ppfxEtaAve = 0;
    ppfxPhiAve = 0;
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

    if (tggHiNtuplizer.b_pho_genMatchedIndex != 0) {
        pho_genMatchedIndex = (*tggHiNtuplizer.pho_genMatchedIndex)[i];
    }

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
    if (tggHiNtuplizer.b_pho_trkIso3pTgt2p0 != 0) {
        pho_trkIso3pTgt2p0 = (*tggHiNtuplizer.pho_trkIso3pTgt2p0)[i];
        pho_trkIso3pTgt2p0subUE = (*tggHiNtuplizer.pho_trkIso3pTgt2p0subUE)[i];
        pho_trkIso3IDpTgt2p0 = (*tggHiNtuplizer.pho_trkIso3IDpTgt2p0)[i];
        pho_trkIso3IDpTgt2p0subUE = (*tggHiNtuplizer.pho_trkIso3IDpTgt2p0subUE)[i];
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
    doMatchGen = gg.doMatchGen;
    doMatchPF = gg.doMatchPF;

    weight = gg.weight;
    weightCent = gg.weightCent;
    weightKin = gg.weightKin;
    weightGenKin = gg.weightGenKin;
    weightPthat = gg.weightPthat;
    phi0 = gg.phi0;
    pthat = gg.pthat;
    processID = gg.processID;
    hiBin = gg.hiBin;
    hiHF = gg.hiHF;
    hiEP1HF = gg.hiEP1HF;
    hiEP2HF = gg.hiEP2HF;
    hiEP3HF = gg.hiEP3HF;
    hiEP4HF = gg.hiEP4HF;

    angEP2pf_out.clear();
    for (int i = 0; i < (int)(*gg.angEP2pf).size(); ++i) {
        angEP2pf_out.push_back( (*gg.angEP2pf)[i] );
    }
    angEP3pf_out.clear();
    for (int i = 0; i < (int)(*gg.angEP3pf).size(); ++i) {
        angEP3pf_out.push_back( (*gg.angEP3pf)[i] );
    }

    angEP2trk = gg.angEP2trk;
    angEP3trk = gg.angEP3trk;

    if (false) {
        partphi_nBins = gg.partphi_nBins;
        partphi_minContent = gg.partphi_minContent;
        partphi_binContents_out.clear();
        for (int i = 0; i < partphi_nBins; ++i) {
            partphi_binContents_out.push_back( (*gg.partphi_binContents)[i] );
        }
        fit_v2 = gg.fit_v2;
        fit_v3 = gg.fit_v3;
        fit_chi2 = gg.fit_chi2;
        fit_chi2prob = gg.fit_chi2prob;
    }

    fit_EPphi0_v2 = gg.fit_EPphi0_v2;
    fit_EPphi0_chi2 = gg.fit_EPphi0_chi2;
    fit_EPphi0_chi2prob = gg.fit_EPphi0_chi2prob;

    trkphi_nBins = gg.trkphi_nBins;
    trkphi_minContent = gg.trkphi_minContent;
    trkphi_binContents_out.clear();
    for (int i = 0; i < trkphi_nBins; ++i) {
        trkphi_binContents_out.push_back( (*gg.trkphi_binContents)[i] );
    }
    fit_trkphi_v2 = gg.fit_trkphi_v2;
    fit_trkphi_v3 = gg.fit_trkphi_v3;
    fit_trkphi_chi2 = gg.fit_trkphi_chi2;
    fit_trkphi_chi2prob = gg.fit_trkphi_chi2prob;

    fMods_th1_yMin_out.clear();
    for (int i = 0; i < (int)((*gg.fMods_th1_yMin).size()); ++i) {
        fMods_th1_yMin_out.push_back( (*gg.fMods_th1_yMin)[i] );
    }
    fMods_angEP2_out.clear();
    for (int i = 0; i < (int)((*gg.fMods_angEP2).size()); ++i) {
        fMods_angEP2_out.push_back( (*gg.fMods_angEP2)[i] );
    }
    fMods_angEP3_out.clear();
    for (int i = 0; i < (int)((*gg.fMods_angEP3).size()); ++i) {
        fMods_angEP3_out.push_back( (*gg.fMods_angEP3)[i] );
    }
    fMods_v2_out.clear();
    for (int i = 0; i < (int)((*gg.fMods_v2).size()); ++i) {
        fMods_v2_out.push_back( (*gg.fMods_v2)[i] );
    }
    fMods_v3_out.clear();
    for (int i = 0; i < (int)((*gg.fMods_v3).size()); ++i) {
        fMods_v3_out.push_back( (*gg.fMods_v3)[i] );
    }
    fMods_chi2_out.clear();
    for (int i = 0; i < (int)((*gg.fMods_chi2).size()); ++i) {
        fMods_chi2_out.push_back( (*gg.fMods_chi2)[i] );
    }
    fMods_chi2prob_out.clear();
    for (int i = 0; i < (int)((*gg.fMods_chi2prob).size()); ++i) {
        fMods_chi2prob_out.push_back( (*gg.fMods_chi2prob)[i] );
    }

    rho = gg.rho;
    angEvtPlane = gg.angEvtPlane;
    indexEvtPlane = gg.indexEvtPlane;
    phi_nTot = gg.phi_nTot;
    phi_minBinN = gg.phi_minBinN;
    phi_fit_chi2 = gg.phi_fit_chi2;
    phi_fit_chi2prob = gg.phi_fit_chi2prob;
    phi_fit_v2 = gg.phi_fit_v2;
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

    pho_trkIso3pTgt2p0 = gg.pho_trkIso3pTgt2p0;
    pho_trkIso3pTgt2p0subUE = gg.pho_trkIso3pTgt2p0subUE;
    pho_trkIso3IDpTgt2p0 = gg.pho_trkIso3IDpTgt2p0;
    pho_trkIso3IDpTgt2p0subUE = gg.pho_trkIso3IDpTgt2p0subUE;

    pfpIso3subUEfMods_out.clear();
    for (int i = 0; i < (int)((*gg.pfpIso3subUEfMods).size()); ++i) {
        pfpIso3subUEfMods_out.push_back( (*gg.pfpIso3subUEfMods)[i] );
    }
    pfnIso3subUEfMods_out.clear();
    for (int i = 0; i < (int)((*gg.pfnIso3subUEfMods).size()); ++i) {
        pfnIso3subUEfMods_out.push_back( (*gg.pfnIso3subUEfMods)[i] );
    }
    pfcIso3subUEfMods_out.clear();
    for (int i = 0; i < (int)((*gg.pfcIso3subUEfMods).size()); ++i) {
        pfcIso3subUEfMods_out.push_back( (*gg.pfcIso3subUEfMods)[i] );
    }
    pfcIso3pTgt2p0subUEfMods_out.clear();
    for (int i = 0; i < (int)((*gg.pfcIso3pTgt2p0subUEfMods).size()); ++i) {
        pfcIso3pTgt2p0subUEfMods_out.push_back( (*gg.pfcIso3pTgt2p0subUEfMods)[i] );
    }

    pfpIso3subUEcalc = gg.pfpIso3subUEcalc;
    pfnIso3subUEcalc = gg.pfnIso3subUEcalc;
    pfcIso3pTgt2p0subUEcalc = gg.pfcIso3pTgt2p0subUEcalc;
    pfcIso3subUEcalc = gg.pfcIso3subUEcalc;
    pfcIso3IDpTgt2p0subUEcalc = gg.pfcIso3IDpTgt2p0subUEcalc;
    pfcIso3IDsubUEcalc = gg.pfcIso3IDsubUEcalc;

    pfpIso3noFPsubUEcalc = gg.pfpIso3noFPsubUEcalc;
    pfnIso3noFPsubUEcalc = gg.pfnIso3noFPsubUEcalc;
    pfcIso3pTgt2p0noFPsubUEcalc = gg.pfcIso3pTgt2p0noFPsubUEcalc;
    pfcIso3noFPsubUEcalc = gg.pfcIso3noFPsubUEcalc;
    pfcIso3IDpTgt2p0noFPsubUEcalc = gg.pfcIso3IDpTgt2p0noFPsubUEcalc;
    pfcIso3IDnoFPsubUEcalc = gg.pfcIso3IDnoFPsubUEcalc;

    pfeIso3pTgt2p0subUEcalc = gg.pfeIso3pTgt2p0subUEcalc;
    pfeIso3subUEcalc = gg.pfeIso3subUEcalc;
    pfeIso3IDpTgt2p0subUEcalc = gg.pfeIso3IDpTgt2p0subUEcalc;
    pfeIso3IDsubUEcalc = gg.pfeIso3IDsubUEcalc;
    pfmIso3pTgt2p0subUEcalc = gg.pfmIso3pTgt2p0subUEcalc;
    pfmIso3subUEcalc = gg.pfmIso3subUEcalc;
    pfmIso3IDpTgt2p0subUEcalc = gg.pfmIso3IDpTgt2p0subUEcalc;
    pfmIso3IDsubUEcalc = gg.pfmIso3IDsubUEcalc;

    pfeIso3pTgt2p0noFPsubUEcalc = gg.pfeIso3pTgt2p0noFPsubUEcalc;
    pfeIso3noFPsubUEcalc = gg.pfeIso3noFPsubUEcalc;
    pfeIso3IDpTgt2p0noFPsubUEcalc = gg.pfeIso3IDpTgt2p0noFPsubUEcalc;
    pfeIso3IDnoFPsubUEcalc = gg.pfeIso3IDnoFPsubUEcalc;
    pfmIso3pTgt2p0noFPsubUEcalc = gg.pfmIso3pTgt2p0noFPsubUEcalc;
    pfmIso3noFPsubUEcalc = gg.pfmIso3noFPsubUEcalc;
    pfmIso3IDpTgt2p0noFPsubUEcalc = gg.pfmIso3IDpTgt2p0noFPsubUEcalc;
    pfmIso3IDnoFPsubUEcalc = gg.pfmIso3IDnoFPsubUEcalc;

    pfCSpIso3subUEcalc = gg.pfCSpIso3subUEcalc;
    pfCSnIso3subUEcalc = gg.pfCSnIso3subUEcalc;
    pfCScIso3pTgt2p0subUEcalc = gg.pfCScIso3pTgt2p0subUEcalc;
    pfCScIso3subUEcalc = gg.pfCScIso3subUEcalc;

    pfpIso3subUEvn2 = gg.pfpIso3subUEvn2;
    pfnIso3subUEvn2 = gg.pfnIso3subUEvn2;
    pfcIso3pTgt2p0subUEvn2 = gg.pfcIso3pTgt2p0subUEvn2;
    pfcIso3subUEvn2 = gg.pfcIso3subUEvn2;

    pfpIso3subUEphi0vn2 = gg.pfpIso3subUEphi0vn2;
    pfnIso3subUEphi0vn2 = gg.pfnIso3subUEphi0vn2;
    pfcIso3pTgt2p0subUEphi0vn2 = gg.pfcIso3pTgt2p0subUEphi0vn2;
    pfcIso3subUEphi0vn2 = gg.pfcIso3subUEphi0vn2;

    pfpIso3subUEtrkvn3 = gg.pfpIso3subUEtrkvn3;
    pfnIso3subUEtrkvn3 = gg.pfnIso3subUEtrkvn3;
    pfcIso3pTgt2p0subUEtrkvn3 = gg.pfcIso3pTgt2p0subUEtrkvn3;
    pfcIso3subUEtrkvn3 = gg.pfcIso3subUEtrkvn3;

    trkIso3 = gg.trkIso3;
    trkIso3subUE = gg.trkIso3subUE;
    trkIso3ID = gg.trkIso3ID;
    trkIso3IDsubUE = gg.trkIso3IDsubUE;
    trkIso3IDmatchPF = gg.trkIso3IDmatchPF;

    phoEAc = gg.phoEAc;
    phoEAp = gg.phoEAp;
    phoEAn = gg.phoEAn;
    phoEAhoe = gg.phoEAhoe;
    phoEAsieie = gg.phoEAsieie;

    nPhoPFp = gg.nPhoPFp;
    ppfpPtSum = gg.ppfpPtSum;
    ppfpEtaAve = gg.ppfpEtaAve;
    ppfpPhiAve = gg.ppfpPhiAve;
    nPhoPFn = gg.nPhoPFn;
    ppfnPtSum = gg.ppfnPtSum;
    ppfnEtaAve = gg.ppfnEtaAve;
    ppfnPhiAve = gg.ppfnPhiAve;
    nPhoPFc = gg.nPhoPFc;
    ppfcPtSum = gg.ppfcPtSum;
    ppfcEtaAve = gg.ppfcEtaAve;
    ppfcPhiAve = gg.ppfcPhiAve;
    ppfcTrkID = gg.ppfcTrkID;
    nPhoPFe = gg.nPhoPFe;
    ppfePtSum = gg.ppfePtSum;
    ppfeEtaAve = gg.ppfeEtaAve;
    ppfePhiAve = gg.ppfePhiAve;
    ppfeTrkID = gg.ppfeTrkID;
    nPhoPFm = gg.nPhoPFm;
    ppfmPtSum = gg.ppfmPtSum;
    ppfmEtaAve = gg.ppfmEtaAve;
    ppfmPhiAve = gg.ppfmPhiAve;
    ppfmTrkID = gg.ppfmTrkID;
    nPhoPFx = gg.nPhoPFx;
    ppfxPtSum = gg.ppfxPtSum;
    ppfxEtaAve = gg.ppfxEtaAve;
    ppfxPhiAve = gg.ppfxPhiAve;

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

    mgPDG = gg.mgPDG;
    mgPt = gg.mgPt;
    mgEta = gg.mgEta;
    mgPhi = gg.mgPhi;
    mgIdx = gg.mgIdx;
    mgPhoPt = gg.mgPhoPt;
    mgPhoEta = gg.mgPhoEta;
    mgPhoPhi = gg.mgPhoPhi;
    mgPhoIdx = gg.mgPhoIdx;
    mgElePt = gg.mgElePt;
    mgEleEta = gg.mgEleEta;
    mgElePhi = gg.mgElePhi;
    mgEleIdx = gg.mgEleIdx;
    mgMuoPt = gg.mgMuoPt;
    mgMuoEta = gg.mgMuoEta;
    mgMuoPhi = gg.mgMuoPhi;
    mgMuoIdx = gg.mgMuoIdx;

    matchPFid = gg.matchPFid;
    matchPFpt = gg.matchPFpt;
    matchPFeta = gg.matchPFeta;
    matchPFphi = gg.matchPFphi;
    matchPFidx = gg.matchPFidx;
    matchPFphoPt = gg.matchPFphoPt;
    matchPFphoEta = gg.matchPFphoEta;
    matchPFphoPhi = gg.matchPFphoPhi;
    matchPFphoIdx = gg.matchPFphoIdx;
    matchPFelePt = gg.matchPFelePt;
    matchPFeleEta = gg.matchPFeleEta;
    matchPFelePhi = gg.matchPFelePhi;
    matchPFeleIdx = gg.matchPFeleIdx;
    matchPFmuoPt = gg.matchPFmuoPt;
    matchPFmuoEta = gg.matchPFmuoEta;
    matchPFmuoPhi = gg.matchPFmuoPhi;
    matchPFmuoIdx = gg.matchPFmuoIdx;
}

bool ggHiFlat::passedHI18HEMfailurePho()
{
    return !(phoSCEta < -1.39 && phoSCPhi < -0.9 && phoSCPhi > -1.6);
}


double ggHiFlat::getValueByName(std::string varName)
{
    if (varName == "phoE") {
        return (double)(phoE);
    }
    else if (varName == "phoEt") {
        return (double)(phoEt);
    }
    else if (varName == "phoEta") {
        return (double)(phoEta);
    }
    else if (varName == "phoPhi") {
        return (double)(phoPhi);
    }
    else if (varName == "phoSCE") {
        return (double)(phoSCE);
    }
    else if (varName == "phoSCRawE") {
        return (double)(phoSCRawE);
    }
    else if (varName == "phoSCEta") {
        return (double)(phoSCEta);
    }
    else if (varName == "phoSCPhi") {
        return (double)(phoSCPhi);
    }
    else if (varName == "phoSCEtaWidth") {
        return (double)(phoSCEtaWidth);
    }
    else if (varName == "phoSCPhiWidth") {
        return (double)(phoSCPhiWidth);
    }
    else if (varName == "phoE3x3_2012") {
        return (double)(phoE3x3_2012);
    }
    else if (varName == "phoMaxEnergyXtal_2012") {
        return (double)(phoMaxEnergyXtal_2012);
    }
    else if (varName == "phoE2nd_2012") {
        return (double)(phoE2nd_2012);
    }
    else if (varName == "phoE_LR" || varName == "(phoELeft_2012-phoERight_2012)/(phoELeft_2012+phoERight_2012)") {

        if (phoELeft_2012 != 0 || phoERight_2012 != 0) {
            return (double)((phoELeft_2012-phoERight_2012)/(phoELeft_2012+phoERight_2012));
        }
        else {
            return 0;
        }
    }
    else if (varName == "phoE_TB" || varName == "(phoETop_2012-phoEBottom_2012)/(phoETop_2012+phoEBottom_2012)") {

        if (phoETop_2012 != 0 || phoEBottom_2012 != 0) {
            return (double)((phoETop_2012-phoEBottom_2012)/(phoETop_2012+phoEBottom_2012));
        }
        else {
            return 0;
        }
    }
    else if (varName == "phoSigmaIEtaIEta_2012") {
        return (double)(phoSigmaIEtaIEta_2012);
    }
    else if (varName == "phoSigmaIEtaIPhi_2012") {
        return (double)(phoSigmaIEtaIPhi_2012);
    }
    else if (varName == "phoSigmaIPhiIPhi_2012") {
        return (double)(phoSigmaIPhiIPhi_2012);
    }
    else if (varName == "rho") {
        return (double)((rho));
    }
    else if (varName == "phoESEn") {
        return (double)(phoESEn);
    }
    else {
        return -998877;
    }
}

void ggHiFlat::copy2Vars(float *vals, std::vector<std::string>& varNames, int nVars, int offset)
{
    for (int j = 0; j < nVars; ++j) {
        vals[j+offset] = getValueByName(varNames[j]);
    }
}

#endif /* TREEHEADERS_GGHIFLATTREE_H_ */
