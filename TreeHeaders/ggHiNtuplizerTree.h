//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat Oct 24 17:09:15 2015 by ROOT version 5.32/00
// from TTree EventTree/Event data
// found on file: ../../HiForest2015/Pyquen_Unquenched_AllQCDPhoton30_PhotonFilter20GeV_eta24_TuneZ2_PbPb_5020GeV_0.root
//////////////////////////////////////////////////////////
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>

class ggHiNtuplizer {
public :
  ggHiNtuplizer(){
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
    eleBC1E = 0;
    eleBC1Eta = 0;
    eleBC2E = 0;
    eleBC2Eta = 0;
    phoE = 0;
    phoEt = 0;
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
    phohasPixelSeed = 0;
    phoR9 = 0;
    phoHoverE = 0;
    phoSigmaIEtaIEta = 0;
    phoE1x3 = 0;
    phoE2x2 = 0;
    phoE2x5Max = 0;
    phoE5x5 = 0;
    phoESEffSigmaRR = 0;
    phoSigmaIEtaIEta_2012 = 0;
    phoSigmaIEtaIPhi_2012 = 0;
    phoSigmaIPhiIPhi_2012 = 0;
    phoE1x3_2012 = 0;
    phoE2x2_2012 = 0;
    phoE2x5Max_2012 = 0;
    phoE5x5_2012 = 0;
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
    muPt = 0;
    muEta = 0;
    muPhi = 0;
    muCharge = 0;
    muType = 0;
    muIsGood = 0;
    muD0 = 0;
    muDz = 0;
    muChi2NDF = 0;
    muInnerD0 = 0;
    muInnerDz = 0;
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

  // Declaration of leaf types
  UInt_t           run;
  ULong64_t        event;
  UInt_t           lumis;
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
  Int_t           nEle;
  std::vector<int>     *eleCharge;
  std::vector<int>     *eleChargeConsistent;
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
  std::vector<float>   *eleBC1E;
  std::vector<float>   *eleBC1Eta;
  std::vector<float>   *eleBC2E;
  std::vector<float>   *eleBC2Eta;
  Int_t           nPho;
  std::vector<float>   *phoE;
  std::vector<float>   *phoEt;
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
  std::vector<int>     *phohasPixelSeed;
  std::vector<float>   *phoR9;
  std::vector<float>   *phoHoverE;
  std::vector<float>   *phoSigmaIEtaIEta;
  std::vector<float>   *phoE1x3;
  std::vector<float>   *phoE2x2;
  std::vector<float>   *phoE2x5Max;
  std::vector<float>   *phoE5x5;
  std::vector<float>   *phoESEffSigmaRR;
  std::vector<float>   *phoSigmaIEtaIEta_2012;
  std::vector<float>   *phoSigmaIEtaIPhi_2012;
  std::vector<float>   *phoSigmaIPhiIPhi_2012;
  std::vector<float>   *phoE1x3_2012;
  std::vector<float>   *phoE2x2_2012;
  std::vector<float>   *phoE2x5Max_2012;
  std::vector<float>   *phoE5x5_2012;
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
  Int_t           nMu;
  std::vector<float>   *muPt;
  std::vector<float>   *muEta;
  std::vector<float>   *muPhi;
  std::vector<int>     *muCharge;
  std::vector<int>     *muType;
  std::vector<int>     *muIsGood;
  std::vector<float>   *muD0;
  std::vector<float>   *muDz;
  std::vector<float>   *muChi2NDF;
  std::vector<float>   *muInnerD0;
  std::vector<float>   *muInnerDz;
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
  TBranch        *b_eleBC1E;   //!
  TBranch        *b_eleBC1Eta;   //!
  TBranch        *b_eleBC2E;   //!
  TBranch        *b_eleBC2Eta;   //!
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
  TBranch        *b_phoE1x3;   //!
  TBranch        *b_phoE2x2;   //!
  TBranch        *b_phoE2x5Max;   //!
  TBranch        *b_phoE5x5;   //!
  TBranch        *b_phoESEffSigmaRR;   //!
  TBranch        *b_phoSigmaIEtaIEta_2012;   //!
  TBranch        *b_phoSigmaIEtaIPhi_2012;   //!
  TBranch        *b_phoSigmaIPhiIPhi_2012;   //!
  TBranch        *b_phoE1x3_2012;   //!
  TBranch        *b_phoE2x2_2012;   //!
  TBranch        *b_phoE2x5Max_2012;   //!
  TBranch        *b_phoE5x5_2012;   //!
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


void setupPhotonTree(TTree *t,ggHiNtuplizer &tggHiNtuplizer)
{
    // Set branch addresses and branch pointers
    if (t->GetBranch("run")) t->SetBranchAddress("run", &tggHiNtuplizer.run, &tggHiNtuplizer.b_run);
    if (t->GetBranch("event")) t->SetBranchAddress("event", &tggHiNtuplizer.event, &tggHiNtuplizer.b_event);
    if (t->GetBranch("lumis")) t->SetBranchAddress("lumis", &tggHiNtuplizer.lumis, &tggHiNtuplizer.b_lumis);
    if (t->GetBranch("isData")) t->SetBranchAddress("isData", &tggHiNtuplizer.isData, &tggHiNtuplizer.b_isData);
    if (t->GetBranch("nPUInfo")) t->SetBranchAddress("nPUInfo", &tggHiNtuplizer.nPUInfo, &tggHiNtuplizer.b_nPUInfo);
    if (t->GetBranch("nPU")) t->SetBranchAddress("nPU", &tggHiNtuplizer.nPU, &tggHiNtuplizer.b_nPU);
    if (t->GetBranch("puBX")) t->SetBranchAddress("puBX", &tggHiNtuplizer.puBX, &tggHiNtuplizer.b_puBX);
    if (t->GetBranch("puTrue")) t->SetBranchAddress("puTrue", &tggHiNtuplizer.puTrue, &tggHiNtuplizer.b_puTrue);
    if (t->GetBranch("nMC")) t->SetBranchAddress("nMC", &tggHiNtuplizer.nMC, &tggHiNtuplizer.b_nMC);
    if (t->GetBranch("mcPID")) t->SetBranchAddress("mcPID", &tggHiNtuplizer.mcPID, &tggHiNtuplizer.b_mcPID);
    if (t->GetBranch("mcStatus")) t->SetBranchAddress("mcStatus", &tggHiNtuplizer.mcStatus, &tggHiNtuplizer.b_mcStatus);
    if (t->GetBranch("mcVtx_x")) t->SetBranchAddress("mcVtx_x", &tggHiNtuplizer.mcVtx_x, &tggHiNtuplizer.b_mcVtx_x);
    if (t->GetBranch("mcVtx_y")) t->SetBranchAddress("mcVtx_y", &tggHiNtuplizer.mcVtx_y, &tggHiNtuplizer.b_mcVtx_y);
    if (t->GetBranch("mcVtx_z")) t->SetBranchAddress("mcVtx_z", &tggHiNtuplizer.mcVtx_z, &tggHiNtuplizer.b_mcVtx_z);
    if (t->GetBranch("mcPt")) t->SetBranchAddress("mcPt", &tggHiNtuplizer.mcPt, &tggHiNtuplizer.b_mcPt);
    if (t->GetBranch("mcEta")) t->SetBranchAddress("mcEta", &tggHiNtuplizer.mcEta, &tggHiNtuplizer.b_mcEta);
    if (t->GetBranch("mcPhi")) t->SetBranchAddress("mcPhi", &tggHiNtuplizer.mcPhi, &tggHiNtuplizer.b_mcPhi);
    if (t->GetBranch("mcE")) t->SetBranchAddress("mcE", &tggHiNtuplizer.mcE, &tggHiNtuplizer.b_mcE);
    if (t->GetBranch("mcEt")) t->SetBranchAddress("mcEt", &tggHiNtuplizer.mcEt, &tggHiNtuplizer.b_mcEt);
    if (t->GetBranch("mcMass")) t->SetBranchAddress("mcMass", &tggHiNtuplizer.mcMass, &tggHiNtuplizer.b_mcMass);
    if (t->GetBranch("mcParentage")) t->SetBranchAddress("mcParentage", &tggHiNtuplizer.mcParentage, &tggHiNtuplizer.b_mcParentage);
    if (t->GetBranch("mcMomPID")) t->SetBranchAddress("mcMomPID", &tggHiNtuplizer.mcMomPID, &tggHiNtuplizer.b_mcMomPID);
    if (t->GetBranch("mcMomPt")) t->SetBranchAddress("mcMomPt", &tggHiNtuplizer.mcMomPt, &tggHiNtuplizer.b_mcMomPt);
    if (t->GetBranch("mcMomEta")) t->SetBranchAddress("mcMomEta", &tggHiNtuplizer.mcMomEta, &tggHiNtuplizer.b_mcMomEta);
    if (t->GetBranch("mcMomPhi")) t->SetBranchAddress("mcMomPhi", &tggHiNtuplizer.mcMomPhi, &tggHiNtuplizer.b_mcMomPhi);
    if (t->GetBranch("mcMomMass")) t->SetBranchAddress("mcMomMass", &tggHiNtuplizer.mcMomMass, &tggHiNtuplizer.b_mcMomMass);
    if (t->GetBranch("mcGMomPID")) t->SetBranchAddress("mcGMomPID", &tggHiNtuplizer.mcGMomPID, &tggHiNtuplizer.b_mcGMomPID);
    if (t->GetBranch("mcIndex")) t->SetBranchAddress("mcIndex", &tggHiNtuplizer.mcIndex, &tggHiNtuplizer.b_mcIndex);
    if (t->GetBranch("mcCalIsoDR03")) t->SetBranchAddress("mcCalIsoDR03", &tggHiNtuplizer.mcCalIsoDR03, &tggHiNtuplizer.b_mcCalIsoDR03);
    if (t->GetBranch("mcCalIsoDR04")) t->SetBranchAddress("mcCalIsoDR04", &tggHiNtuplizer.mcCalIsoDR04, &tggHiNtuplizer.b_mcCalIsoDR04);
    if (t->GetBranch("mcTrkIsoDR03")) t->SetBranchAddress("mcTrkIsoDR03", &tggHiNtuplizer.mcTrkIsoDR03, &tggHiNtuplizer.b_mcTrkIsoDR03);
    if (t->GetBranch("mcTrkIsoDR04")) t->SetBranchAddress("mcTrkIsoDR04", &tggHiNtuplizer.mcTrkIsoDR04, &tggHiNtuplizer.b_mcTrkIsoDR04);
    if (t->GetBranch("pho_genMatchedIndex")) t->SetBranchAddress("pho_genMatchedIndex", &tggHiNtuplizer.pho_genMatchedIndex, &tggHiNtuplizer.b_pho_genMatchedIndex);
    if (t->GetBranch("nEle")) t->SetBranchAddress("nEle", &tggHiNtuplizer.nEle, &tggHiNtuplizer.b_nEle);
    if (t->GetBranch("eleCharge")) t->SetBranchAddress("eleCharge", &tggHiNtuplizer.eleCharge, &tggHiNtuplizer.b_eleCharge);
    if (t->GetBranch("eleChargeConsistent")) t->SetBranchAddress("eleChargeConsistent", &tggHiNtuplizer.eleChargeConsistent, &tggHiNtuplizer.b_eleChargeConsistent);
    if (t->GetBranch("eleEn")) t->SetBranchAddress("eleEn", &tggHiNtuplizer.eleEn, &tggHiNtuplizer.b_eleEn);
    if (t->GetBranch("eleD0")) t->SetBranchAddress("eleD0", &tggHiNtuplizer.eleD0, &tggHiNtuplizer.b_eleD0);
    if (t->GetBranch("eleDz")) t->SetBranchAddress("eleDz", &tggHiNtuplizer.eleDz, &tggHiNtuplizer.b_eleDz);
    if (t->GetBranch("eleD0Err")) t->SetBranchAddress("eleD0Err", &tggHiNtuplizer.eleD0Err, &tggHiNtuplizer.b_eleD0Err);
    if (t->GetBranch("eleDzErr")) t->SetBranchAddress("eleDzErr", &tggHiNtuplizer.eleDzErr, &tggHiNtuplizer.b_eleDzErr);
    if (t->GetBranch("eleTrkPt")) t->SetBranchAddress("eleTrkPt", &tggHiNtuplizer.eleTrkPt, &tggHiNtuplizer.b_eleTrkPt);
    if (t->GetBranch("eleTrkEta")) t->SetBranchAddress("eleTrkEta", &tggHiNtuplizer.eleTrkEta, &tggHiNtuplizer.b_eleTrkEta);
    if (t->GetBranch("eleTrkPhi")) t->SetBranchAddress("eleTrkPhi", &tggHiNtuplizer.eleTrkPhi, &tggHiNtuplizer.b_eleTrkPhi);
    if (t->GetBranch("eleTrkCharge")) t->SetBranchAddress("eleTrkCharge", &tggHiNtuplizer.eleTrkCharge, &tggHiNtuplizer.b_eleTrkCharge);
    if (t->GetBranch("eleTrkCharge")) t->SetBranchAddress("eleTrkCharge", &tggHiNtuplizer.eleTrkChi2, &tggHiNtuplizer.b_eleTrkChi2);
    if (t->GetBranch("eleTrkNdof")) t->SetBranchAddress("eleTrkNdof", &tggHiNtuplizer.eleTrkNdof, &tggHiNtuplizer.b_eleTrkNdof);
    if (t->GetBranch("eleTrkNormalizedChi2")) t->SetBranchAddress("eleTrkNormalizedChi2", &tggHiNtuplizer.eleTrkNormalizedChi2, &tggHiNtuplizer.b_eleTrkNormalizedChi2);
    if (t->GetBranch("elePt")) t->SetBranchAddress("elePt", &tggHiNtuplizer.elePt, &tggHiNtuplizer.b_elePt);
    if (t->GetBranch("eleEta")) t->SetBranchAddress("eleEta", &tggHiNtuplizer.eleEta, &tggHiNtuplizer.b_eleEta);
    if (t->GetBranch("elePhi")) t->SetBranchAddress("elePhi", &tggHiNtuplizer.elePhi, &tggHiNtuplizer.b_elePhi);
    if (t->GetBranch("eleSCEn")) t->SetBranchAddress("eleSCEn", &tggHiNtuplizer.eleSCEn, &tggHiNtuplizer.b_eleSCEn);
    if (t->GetBranch("eleESEn")) t->SetBranchAddress("eleESEn", &tggHiNtuplizer.eleESEn, &tggHiNtuplizer.b_eleESEn);
    if (t->GetBranch("eleSCEta")) t->SetBranchAddress("eleSCEta", &tggHiNtuplizer.eleSCEta, &tggHiNtuplizer.b_eleSCEta);
    if (t->GetBranch("eleSCPhi")) t->SetBranchAddress("eleSCPhi", &tggHiNtuplizer.eleSCPhi, &tggHiNtuplizer.b_eleSCPhi);
    if (t->GetBranch("eleSCRawEn")) t->SetBranchAddress("eleSCRawEn", &tggHiNtuplizer.eleSCRawEn, &tggHiNtuplizer.b_eleSCRawEn);
    if (t->GetBranch("eleSCEtaWidth")) t->SetBranchAddress("eleSCEtaWidth", &tggHiNtuplizer.eleSCEtaWidth, &tggHiNtuplizer.b_eleSCEtaWidth);
    if (t->GetBranch("eleSCPhiWidth")) t->SetBranchAddress("eleSCPhiWidth", &tggHiNtuplizer.eleSCPhiWidth, &tggHiNtuplizer.b_eleSCPhiWidth);
    if (t->GetBranch("eleHoverE")) t->SetBranchAddress("eleHoverE", &tggHiNtuplizer.eleHoverE, &tggHiNtuplizer.b_eleHoverE);
    if (t->GetBranch("eleEoverP")) t->SetBranchAddress("eleEoverP", &tggHiNtuplizer.eleEoverP, &tggHiNtuplizer.b_eleEoverP);
    if (t->GetBranch("eleEoverPInv")) t->SetBranchAddress("eleEoverPInv", &tggHiNtuplizer.eleEoverPInv, &tggHiNtuplizer.b_eleEoverPInv);
    if (t->GetBranch("eleBrem")) t->SetBranchAddress("eleBrem", &tggHiNtuplizer.eleBrem, &tggHiNtuplizer.b_eleBrem);
    if (t->GetBranch("eledEtaAtVtx")) t->SetBranchAddress("eledEtaAtVtx", &tggHiNtuplizer.eledEtaAtVtx, &tggHiNtuplizer.b_eledEtaAtVtx);
    if (t->GetBranch("eledPhiAtVtx")) t->SetBranchAddress("eledPhiAtVtx", &tggHiNtuplizer.eledPhiAtVtx, &tggHiNtuplizer.b_eledPhiAtVtx);
    if (t->GetBranch("eleSigmaIEtaIEta")) t->SetBranchAddress("eleSigmaIEtaIEta", &tggHiNtuplizer.eleSigmaIEtaIEta, &tggHiNtuplizer.b_eleSigmaIEtaIEta);
    if (t->GetBranch("eleSigmaIEtaIEta_2012")) t->SetBranchAddress("eleSigmaIEtaIEta_2012", &tggHiNtuplizer.eleSigmaIEtaIEta_2012, &tggHiNtuplizer.b_eleSigmaIEtaIEta_2012);
    if (t->GetBranch("eleSigmaIPhiIPhi")) t->SetBranchAddress("eleSigmaIPhiIPhi", &tggHiNtuplizer.eleSigmaIPhiIPhi, &tggHiNtuplizer.b_eleSigmaIPhiIPhi);
    if (t->GetBranch("eleMissHits")) t->SetBranchAddress("eleMissHits", &tggHiNtuplizer.eleMissHits, &tggHiNtuplizer.b_eleMissHits);
    if (t->GetBranch("eleESEffSigmaRR")) t->SetBranchAddress("eleESEffSigmaRR", &tggHiNtuplizer.eleESEffSigmaRR, &tggHiNtuplizer.b_eleESEffSigmaRR);
    if (t->GetBranch("elePFChIso")) t->SetBranchAddress("elePFChIso", &tggHiNtuplizer.elePFChIso, &tggHiNtuplizer.b_elePFChIso);
    if (t->GetBranch("elePFPhoIso")) t->SetBranchAddress("elePFPhoIso", &tggHiNtuplizer.elePFPhoIso, &tggHiNtuplizer.b_elePFPhoIso);
    if (t->GetBranch("elePFNeuIso")) t->SetBranchAddress("elePFNeuIso", &tggHiNtuplizer.elePFNeuIso, &tggHiNtuplizer.b_elePFNeuIso);
    if (t->GetBranch("elePFPUIso")) t->SetBranchAddress("elePFPUIso", &tggHiNtuplizer.elePFPUIso, &tggHiNtuplizer.b_elePFPUIso);
    if (t->GetBranch("eleBC1E")) t->SetBranchAddress("eleBC1E", &tggHiNtuplizer.eleBC1E, &tggHiNtuplizer.b_eleBC1E);
    if (t->GetBranch("eleBC1Eta")) t->SetBranchAddress("eleBC1Eta", &tggHiNtuplizer.eleBC1Eta, &tggHiNtuplizer.b_eleBC1Eta);
    if (t->GetBranch("eleBC2E")) t->SetBranchAddress("eleBC2E", &tggHiNtuplizer.eleBC2E, &tggHiNtuplizer.b_eleBC2E);
    if (t->GetBranch("eleBC2Eta")) t->SetBranchAddress("eleBC2Eta", &tggHiNtuplizer.eleBC2Eta, &tggHiNtuplizer.b_eleBC2Eta);
    if (t->GetBranch("nPho")) t->SetBranchAddress("nPho", &tggHiNtuplizer.nPho, &tggHiNtuplizer.b_nPho);
    if (t->GetBranch("phoE")) t->SetBranchAddress("phoE", &tggHiNtuplizer.phoE, &tggHiNtuplizer.b_phoE);
    if (t->GetBranch("phoEt")) t->SetBranchAddress("phoEt", &tggHiNtuplizer.phoEt, &tggHiNtuplizer.b_phoEt);
    if (t->GetBranch("phoEta")) t->SetBranchAddress("phoEta", &tggHiNtuplizer.phoEta, &tggHiNtuplizer.b_phoEta);
    if (t->GetBranch("phoPhi")) t->SetBranchAddress("phoPhi", &tggHiNtuplizer.phoPhi, &tggHiNtuplizer.b_phoPhi);
    if (t->GetBranch("phoSCE")) t->SetBranchAddress("phoSCE", &tggHiNtuplizer.phoSCE, &tggHiNtuplizer.b_phoSCE);
    if (t->GetBranch("phoSCRawE")) t->SetBranchAddress("phoSCRawE", &tggHiNtuplizer.phoSCRawE, &tggHiNtuplizer.b_phoSCRawE);
    if (t->GetBranch("phoESEn")) t->SetBranchAddress("phoESEn", &tggHiNtuplizer.phoESEn, &tggHiNtuplizer.b_phoESEn);
    if (t->GetBranch("phoSCEta")) t->SetBranchAddress("phoSCEta", &tggHiNtuplizer.phoSCEta, &tggHiNtuplizer.b_phoSCEta);
    if (t->GetBranch("phoSCPhi")) t->SetBranchAddress("phoSCPhi", &tggHiNtuplizer.phoSCPhi, &tggHiNtuplizer.b_phoSCPhi);
    if (t->GetBranch("phoSCEtaWidth")) t->SetBranchAddress("phoSCEtaWidth", &tggHiNtuplizer.phoSCEtaWidth, &tggHiNtuplizer.b_phoSCEtaWidth);
    if (t->GetBranch("phoSCPhiWidth")) t->SetBranchAddress("phoSCPhiWidth", &tggHiNtuplizer.phoSCPhiWidth, &tggHiNtuplizer.b_phoSCPhiWidth);
    if (t->GetBranch("phoSCBrem")) t->SetBranchAddress("phoSCBrem", &tggHiNtuplizer.phoSCBrem, &tggHiNtuplizer.b_phoSCBrem);
    if (t->GetBranch("phohasPixelSeed")) t->SetBranchAddress("phohasPixelSeed", &tggHiNtuplizer.phohasPixelSeed, &tggHiNtuplizer.b_phohasPixelSeed);
    if (t->GetBranch("phoR9")) t->SetBranchAddress("phoR9", &tggHiNtuplizer.phoR9, &tggHiNtuplizer.b_phoR9);
    if (t->GetBranch("phoHoverE")) t->SetBranchAddress("phoHoverE", &tggHiNtuplizer.phoHoverE, &tggHiNtuplizer.b_phoHoverE);
    if (t->GetBranch("phoSigmaIEtaIEta")) t->SetBranchAddress("phoSigmaIEtaIEta", &tggHiNtuplizer.phoSigmaIEtaIEta, &tggHiNtuplizer.b_phoSigmaIEtaIEta);
    if (t->GetBranch("phoE1x3")) t->SetBranchAddress("phoE1x3", &tggHiNtuplizer.phoE1x3, &tggHiNtuplizer.b_phoE1x3);
    if (t->GetBranch("phoE2x2")) t->SetBranchAddress("phoE2x2", &tggHiNtuplizer.phoE2x2, &tggHiNtuplizer.b_phoE2x2);
    if (t->GetBranch("phoE2x5Max")) t->SetBranchAddress("phoE2x5Max", &tggHiNtuplizer.phoE2x5Max, &tggHiNtuplizer.b_phoE2x5Max);
    if (t->GetBranch("phoE5x5")) t->SetBranchAddress("phoE5x5", &tggHiNtuplizer.phoE5x5, &tggHiNtuplizer.b_phoE5x5);
    if (t->GetBranch("phoESEffSigmaRR")) t->SetBranchAddress("phoESEffSigmaRR", &tggHiNtuplizer.phoESEffSigmaRR, &tggHiNtuplizer.b_phoESEffSigmaRR);
    if (t->GetBranch("phoSigmaIEtaIEta_2012")) t->SetBranchAddress("phoSigmaIEtaIEta_2012", &tggHiNtuplizer.phoSigmaIEtaIEta_2012, &tggHiNtuplizer.b_phoSigmaIEtaIEta_2012);
    if (t->GetBranch("phoSigmaIEtaIPhi_2012")) t->SetBranchAddress("phoSigmaIEtaIPhi_2012", &tggHiNtuplizer.phoSigmaIEtaIPhi_2012, &tggHiNtuplizer.b_phoSigmaIEtaIPhi_2012);
    if (t->GetBranch("phoSigmaIPhiIPhi_2012")) t->SetBranchAddress("phoSigmaIPhiIPhi_2012", &tggHiNtuplizer.phoSigmaIPhiIPhi_2012, &tggHiNtuplizer.b_phoSigmaIPhiIPhi_2012);
    if (t->GetBranch("phoE1x3_2012")) t->SetBranchAddress("phoE1x3_2012", &tggHiNtuplizer.phoE1x3_2012, &tggHiNtuplizer.b_phoE1x3_2012);
    if (t->GetBranch("phoE2x2_2012")) t->SetBranchAddress("phoE2x2_2012", &tggHiNtuplizer.phoE2x2_2012, &tggHiNtuplizer.b_phoE2x2_2012);
    if (t->GetBranch("phoE2x5Max_2012")) t->SetBranchAddress("phoE2x5Max_2012", &tggHiNtuplizer.phoE2x5Max_2012, &tggHiNtuplizer.b_phoE2x5Max_2012);
    if (t->GetBranch("phoE5x5_2012")) t->SetBranchAddress("phoE5x5_2012", &tggHiNtuplizer.phoE5x5_2012, &tggHiNtuplizer.b_phoE5x5_2012);
    if (t->GetBranch("phoBC1E")) t->SetBranchAddress("phoBC1E", &tggHiNtuplizer.phoBC1E, &tggHiNtuplizer.b_phoBC1E);
    if (t->GetBranch("phoBC1Eta")) t->SetBranchAddress("phoBC1Eta", &tggHiNtuplizer.phoBC1Eta, &tggHiNtuplizer.b_phoBC1Eta);
    if (t->GetBranch("phoBC2E")) t->SetBranchAddress("phoBC2E", &tggHiNtuplizer.phoBC2E, &tggHiNtuplizer.b_phoBC2E);
    if (t->GetBranch("phoBC2Eta")) t->SetBranchAddress("phoBC2Eta", &tggHiNtuplizer.phoBC2Eta, &tggHiNtuplizer.b_phoBC2Eta);
    if (t->GetBranch("pho_ecalClusterIsoR2")) t->SetBranchAddress("pho_ecalClusterIsoR2", &tggHiNtuplizer.pho_ecalClusterIsoR2, &tggHiNtuplizer.b_pho_ecalClusterIsoR2);
    if (t->GetBranch("pho_ecalClusterIsoR3")) t->SetBranchAddress("pho_ecalClusterIsoR3", &tggHiNtuplizer.pho_ecalClusterIsoR3, &tggHiNtuplizer.b_pho_ecalClusterIsoR3);
    if (t->GetBranch("pho_ecalClusterIsoR4")) t->SetBranchAddress("pho_ecalClusterIsoR4", &tggHiNtuplizer.pho_ecalClusterIsoR4, &tggHiNtuplizer.b_pho_ecalClusterIsoR4);
    if (t->GetBranch("pho_ecalClusterIsoR5")) t->SetBranchAddress("pho_ecalClusterIsoR5", &tggHiNtuplizer.pho_ecalClusterIsoR5, &tggHiNtuplizer.b_pho_ecalClusterIsoR5);
    if (t->GetBranch("pho_hcalRechitIsoR1")) t->SetBranchAddress("pho_hcalRechitIsoR1", &tggHiNtuplizer.pho_hcalRechitIsoR1, &tggHiNtuplizer.b_pho_hcalRechitIsoR1);
    if (t->GetBranch("pho_hcalRechitIsoR2")) t->SetBranchAddress("pho_hcalRechitIsoR2", &tggHiNtuplizer.pho_hcalRechitIsoR2, &tggHiNtuplizer.b_pho_hcalRechitIsoR2);
    if (t->GetBranch("pho_hcalRechitIsoR3")) t->SetBranchAddress("pho_hcalRechitIsoR3", &tggHiNtuplizer.pho_hcalRechitIsoR3, &tggHiNtuplizer.b_pho_hcalRechitIsoR3);
    if (t->GetBranch("pho_hcalRechitIsoR4")) t->SetBranchAddress("pho_hcalRechitIsoR4", &tggHiNtuplizer.pho_hcalRechitIsoR4, &tggHiNtuplizer.b_pho_hcalRechitIsoR4);
    if (t->GetBranch("pho_hcalRechitIsoR5")) t->SetBranchAddress("pho_hcalRechitIsoR5", &tggHiNtuplizer.pho_hcalRechitIsoR5, &tggHiNtuplizer.b_pho_hcalRechitIsoR5);
    if (t->GetBranch("pho_trackIsoR1PtCut20")) t->SetBranchAddress("pho_trackIsoR1PtCut20", &tggHiNtuplizer.pho_trackIsoR1PtCut20, &tggHiNtuplizer.b_pho_trackIsoR1PtCut20);
    if (t->GetBranch("pho_trackIsoR2PtCut20")) t->SetBranchAddress("pho_trackIsoR2PtCut20", &tggHiNtuplizer.pho_trackIsoR2PtCut20, &tggHiNtuplizer.b_pho_trackIsoR2PtCut20);
    if (t->GetBranch("pho_trackIsoR3PtCut20")) t->SetBranchAddress("pho_trackIsoR3PtCut20", &tggHiNtuplizer.pho_trackIsoR3PtCut20, &tggHiNtuplizer.b_pho_trackIsoR3PtCut20);
    if (t->GetBranch("pho_trackIsoR4PtCut20")) t->SetBranchAddress("pho_trackIsoR4PtCut20", &tggHiNtuplizer.pho_trackIsoR4PtCut20, &tggHiNtuplizer.b_pho_trackIsoR4PtCut20);
    if (t->GetBranch("pho_trackIsoR5PtCut20")) t->SetBranchAddress("pho_trackIsoR5PtCut20", &tggHiNtuplizer.pho_trackIsoR5PtCut20, &tggHiNtuplizer.b_pho_trackIsoR5PtCut20);
    if (t->GetBranch("pho_swissCrx")) t->SetBranchAddress("pho_swissCrx", &tggHiNtuplizer.pho_swissCrx, &tggHiNtuplizer.b_pho_swissCrx);
    if (t->GetBranch("pho_seedTime")) t->SetBranchAddress("pho_seedTime", &tggHiNtuplizer.pho_seedTime, &tggHiNtuplizer.b_pho_seedTime);
    if (t->GetBranch("pfcIso1")) t->SetBranchAddress("pfcIso1", &tggHiNtuplizer.pfcIso1, &tggHiNtuplizer.b_pfcIso1);
    if (t->GetBranch("pfcIso2")) t->SetBranchAddress("pfcIso2", &tggHiNtuplizer.pfcIso2, &tggHiNtuplizer.b_pfcIso2);
    if (t->GetBranch("pfcIso3")) t->SetBranchAddress("pfcIso3", &tggHiNtuplizer.pfcIso3, &tggHiNtuplizer.b_pfcIso3);
    if (t->GetBranch("pfcIso4")) t->SetBranchAddress("pfcIso4", &tggHiNtuplizer.pfcIso4, &tggHiNtuplizer.b_pfcIso4);
    if (t->GetBranch("pfcIso5")) t->SetBranchAddress("pfcIso5", &tggHiNtuplizer.pfcIso5, &tggHiNtuplizer.b_pfcIso5);
    if (t->GetBranch("pfpIso1")) t->SetBranchAddress("pfpIso1", &tggHiNtuplizer.pfpIso1, &tggHiNtuplizer.b_pfpIso1);
    if (t->GetBranch("pfpIso2")) t->SetBranchAddress("pfpIso2", &tggHiNtuplizer.pfpIso2, &tggHiNtuplizer.b_pfpIso2);
    if (t->GetBranch("pfpIso3")) t->SetBranchAddress("pfpIso3", &tggHiNtuplizer.pfpIso3, &tggHiNtuplizer.b_pfpIso3);
    if (t->GetBranch("pfpIso4")) t->SetBranchAddress("pfpIso4", &tggHiNtuplizer.pfpIso4, &tggHiNtuplizer.b_pfpIso4);
    if (t->GetBranch("pfpIso5")) t->SetBranchAddress("pfpIso5", &tggHiNtuplizer.pfpIso5, &tggHiNtuplizer.b_pfpIso5);
    if (t->GetBranch("pfnIso1")) t->SetBranchAddress("pfnIso1", &tggHiNtuplizer.pfnIso1, &tggHiNtuplizer.b_pfnIso1);
    if (t->GetBranch("pfnIso2")) t->SetBranchAddress("pfnIso2", &tggHiNtuplizer.pfnIso2, &tggHiNtuplizer.b_pfnIso2);
    if (t->GetBranch("pfnIso3")) t->SetBranchAddress("pfnIso3", &tggHiNtuplizer.pfnIso3, &tggHiNtuplizer.b_pfnIso3);
    if (t->GetBranch("pfnIso4")) t->SetBranchAddress("pfnIso4", &tggHiNtuplizer.pfnIso4, &tggHiNtuplizer.b_pfnIso4);
    if (t->GetBranch("pfnIso5")) t->SetBranchAddress("pfnIso5", &tggHiNtuplizer.pfnIso5, &tggHiNtuplizer.b_pfnIso5);
    if (t->GetBranch("pfcVsIso1")) t->SetBranchAddress("pfcVsIso1", &tggHiNtuplizer.pfcVsIso1, &tggHiNtuplizer.b_pfcVsIso1);
    if (t->GetBranch("pfcVsIso2")) t->SetBranchAddress("pfcVsIso2", &tggHiNtuplizer.pfcVsIso2, &tggHiNtuplizer.b_pfcVsIso2);
    if (t->GetBranch("pfcVsIso3")) t->SetBranchAddress("pfcVsIso3", &tggHiNtuplizer.pfcVsIso3, &tggHiNtuplizer.b_pfcVsIso3);
    if (t->GetBranch("pfcVsIso4")) t->SetBranchAddress("pfcVsIso4", &tggHiNtuplizer.pfcVsIso4, &tggHiNtuplizer.b_pfcVsIso4);
    if (t->GetBranch("pfcVsIso5")) t->SetBranchAddress("pfcVsIso5", &tggHiNtuplizer.pfcVsIso5, &tggHiNtuplizer.b_pfcVsIso5);
    if (t->GetBranch("pfcVsIso1th1")) t->SetBranchAddress("pfcVsIso1th1", &tggHiNtuplizer.pfcVsIso1th1, &tggHiNtuplizer.b_pfcVsIso1th1);
    if (t->GetBranch("pfcVsIso2th1")) t->SetBranchAddress("pfcVsIso2th1", &tggHiNtuplizer.pfcVsIso2th1, &tggHiNtuplizer.b_pfcVsIso2th1);
    if (t->GetBranch("pfcVsIso3th1")) t->SetBranchAddress("pfcVsIso3th1", &tggHiNtuplizer.pfcVsIso3th1, &tggHiNtuplizer.b_pfcVsIso3th1);
    if (t->GetBranch("pfcVsIso4th1")) t->SetBranchAddress("pfcVsIso4th1", &tggHiNtuplizer.pfcVsIso4th1, &tggHiNtuplizer.b_pfcVsIso4th1);
    if (t->GetBranch("pfcVsIso5th1")) t->SetBranchAddress("pfcVsIso5th1", &tggHiNtuplizer.pfcVsIso5th1, &tggHiNtuplizer.b_pfcVsIso5th1);
    if (t->GetBranch("pfcVsIso1th2")) t->SetBranchAddress("pfcVsIso1th2", &tggHiNtuplizer.pfcVsIso1th2, &tggHiNtuplizer.b_pfcVsIso1th2);
    if (t->GetBranch("pfcVsIso2th2")) t->SetBranchAddress("pfcVsIso2th2", &tggHiNtuplizer.pfcVsIso2th2, &tggHiNtuplizer.b_pfcVsIso2th2);
    if (t->GetBranch("pfcVsIso3th2")) t->SetBranchAddress("pfcVsIso3th2", &tggHiNtuplizer.pfcVsIso3th2, &tggHiNtuplizer.b_pfcVsIso3th2);
    if (t->GetBranch("pfcVsIso4th2")) t->SetBranchAddress("pfcVsIso4th2", &tggHiNtuplizer.pfcVsIso4th2, &tggHiNtuplizer.b_pfcVsIso4th2);
    if (t->GetBranch("pfcVsIso5th2")) t->SetBranchAddress("pfcVsIso5th2", &tggHiNtuplizer.pfcVsIso5th2, &tggHiNtuplizer.b_pfcVsIso5th2);
    if (t->GetBranch("pfnVsIso1")) t->SetBranchAddress("pfnVsIso1", &tggHiNtuplizer.pfnVsIso1, &tggHiNtuplizer.b_pfnVsIso1);
    if (t->GetBranch("pfnVsIso2")) t->SetBranchAddress("pfnVsIso2", &tggHiNtuplizer.pfnVsIso2, &tggHiNtuplizer.b_pfnVsIso2);
    if (t->GetBranch("pfnVsIso3")) t->SetBranchAddress("pfnVsIso3", &tggHiNtuplizer.pfnVsIso3, &tggHiNtuplizer.b_pfnVsIso3);
    if (t->GetBranch("pfnVsIso4")) t->SetBranchAddress("pfnVsIso4", &tggHiNtuplizer.pfnVsIso4, &tggHiNtuplizer.b_pfnVsIso4);
    if (t->GetBranch("pfnVsIso5")) t->SetBranchAddress("pfnVsIso5", &tggHiNtuplizer.pfnVsIso5, &tggHiNtuplizer.b_pfnVsIso5);
    if (t->GetBranch("pfnVsIso1th1")) t->SetBranchAddress("pfnVsIso1th1", &tggHiNtuplizer.pfnVsIso1th1, &tggHiNtuplizer.b_pfnVsIso1th1);
    if (t->GetBranch("pfnVsIso2th1")) t->SetBranchAddress("pfnVsIso2th1", &tggHiNtuplizer.pfnVsIso2th1, &tggHiNtuplizer.b_pfnVsIso2th1);
    if (t->GetBranch("pfnVsIso3th1")) t->SetBranchAddress("pfnVsIso3th1", &tggHiNtuplizer.pfnVsIso3th1, &tggHiNtuplizer.b_pfnVsIso3th1);
    if (t->GetBranch("pfnVsIso4th1")) t->SetBranchAddress("pfnVsIso4th1", &tggHiNtuplizer.pfnVsIso4th1, &tggHiNtuplizer.b_pfnVsIso4th1);
    if (t->GetBranch("pfnVsIso5th1")) t->SetBranchAddress("pfnVsIso5th1", &tggHiNtuplizer.pfnVsIso5th1, &tggHiNtuplizer.b_pfnVsIso5th1);
    if (t->GetBranch("pfnVsIso1th2")) t->SetBranchAddress("pfnVsIso1th2", &tggHiNtuplizer.pfnVsIso1th2, &tggHiNtuplizer.b_pfnVsIso1th2);
    if (t->GetBranch("pfnVsIso2th2")) t->SetBranchAddress("pfnVsIso2th2", &tggHiNtuplizer.pfnVsIso2th2, &tggHiNtuplizer.b_pfnVsIso2th2);
    if (t->GetBranch("pfnVsIso3th2")) t->SetBranchAddress("pfnVsIso3th2", &tggHiNtuplizer.pfnVsIso3th2, &tggHiNtuplizer.b_pfnVsIso3th2);
    if (t->GetBranch("pfnVsIso4th2")) t->SetBranchAddress("pfnVsIso4th2", &tggHiNtuplizer.pfnVsIso4th2, &tggHiNtuplizer.b_pfnVsIso4th2);
    if (t->GetBranch("pfnVsIso5th2")) t->SetBranchAddress("pfnVsIso5th2", &tggHiNtuplizer.pfnVsIso5th2, &tggHiNtuplizer.b_pfnVsIso5th2);
    if (t->GetBranch("pfpVsIso1")) t->SetBranchAddress("pfpVsIso1", &tggHiNtuplizer.pfpVsIso1, &tggHiNtuplizer.b_pfpVsIso1);
    if (t->GetBranch("pfpVsIso2")) t->SetBranchAddress("pfpVsIso2", &tggHiNtuplizer.pfpVsIso2, &tggHiNtuplizer.b_pfpVsIso2);
    if (t->GetBranch("pfpVsIso3")) t->SetBranchAddress("pfpVsIso3", &tggHiNtuplizer.pfpVsIso3, &tggHiNtuplizer.b_pfpVsIso3);
    if (t->GetBranch("pfpVsIso4")) t->SetBranchAddress("pfpVsIso4", &tggHiNtuplizer.pfpVsIso4, &tggHiNtuplizer.b_pfpVsIso4);
    if (t->GetBranch("pfpVsIso5")) t->SetBranchAddress("pfpVsIso5", &tggHiNtuplizer.pfpVsIso5, &tggHiNtuplizer.b_pfpVsIso5);
    if (t->GetBranch("pfpVsIso1th1")) t->SetBranchAddress("pfpVsIso1th1", &tggHiNtuplizer.pfpVsIso1th1, &tggHiNtuplizer.b_pfpVsIso1th1);
    if (t->GetBranch("pfpVsIso2th1")) t->SetBranchAddress("pfpVsIso2th1", &tggHiNtuplizer.pfpVsIso2th1, &tggHiNtuplizer.b_pfpVsIso2th1);
    if (t->GetBranch("pfpVsIso3th1")) t->SetBranchAddress("pfpVsIso3th1", &tggHiNtuplizer.pfpVsIso3th1, &tggHiNtuplizer.b_pfpVsIso3th1);
    if (t->GetBranch("pfpVsIso4th1")) t->SetBranchAddress("pfpVsIso4th1", &tggHiNtuplizer.pfpVsIso4th1, &tggHiNtuplizer.b_pfpVsIso4th1);
    if (t->GetBranch("pfpVsIso5th1")) t->SetBranchAddress("pfpVsIso5th1", &tggHiNtuplizer.pfpVsIso5th1, &tggHiNtuplizer.b_pfpVsIso5th1);
    if (t->GetBranch("pfpVsIso1th2")) t->SetBranchAddress("pfpVsIso1th2", &tggHiNtuplizer.pfpVsIso1th2, &tggHiNtuplizer.b_pfpVsIso1th2);
    if (t->GetBranch("pfpVsIso2th2")) t->SetBranchAddress("pfpVsIso2th2", &tggHiNtuplizer.pfpVsIso2th2, &tggHiNtuplizer.b_pfpVsIso2th2);
    if (t->GetBranch("pfpVsIso3th2")) t->SetBranchAddress("pfpVsIso3th2", &tggHiNtuplizer.pfpVsIso3th2, &tggHiNtuplizer.b_pfpVsIso3th2);
    if (t->GetBranch("pfpVsIso4th2")) t->SetBranchAddress("pfpVsIso4th2", &tggHiNtuplizer.pfpVsIso4th2, &tggHiNtuplizer.b_pfpVsIso4th2);
    if (t->GetBranch("pfpVsIso5th2")) t->SetBranchAddress("pfpVsIso5th2", &tggHiNtuplizer.pfpVsIso5th2, &tggHiNtuplizer.b_pfpVsIso5th2);
    if (t->GetBranch("towerIso1")) t->SetBranchAddress("towerIso1", &tggHiNtuplizer.towerIso1, &tggHiNtuplizer.b_towerIso1);
    if (t->GetBranch("towerIso2")) t->SetBranchAddress("towerIso2", &tggHiNtuplizer.towerIso2, &tggHiNtuplizer.b_towerIso2);
    if (t->GetBranch("towerIso3")) t->SetBranchAddress("towerIso3", &tggHiNtuplizer.towerIso3, &tggHiNtuplizer.b_towerIso3);
    if (t->GetBranch("towerIso4")) t->SetBranchAddress("towerIso4", &tggHiNtuplizer.towerIso4, &tggHiNtuplizer.b_towerIso4);
    if (t->GetBranch("towerIso5")) t->SetBranchAddress("towerIso5", &tggHiNtuplizer.towerIso5, &tggHiNtuplizer.b_towerIso5);
    if (t->GetBranch("towerVsIso1")) t->SetBranchAddress("towerVsIso1", &tggHiNtuplizer.towerVsIso1, &tggHiNtuplizer.b_towerVsIso1);
    if (t->GetBranch("towerVsIso2")) t->SetBranchAddress("towerVsIso2", &tggHiNtuplizer.towerVsIso2, &tggHiNtuplizer.b_towerVsIso2);
    if (t->GetBranch("towerVsIso3")) t->SetBranchAddress("towerVsIso3", &tggHiNtuplizer.towerVsIso3, &tggHiNtuplizer.b_towerVsIso3);
    if (t->GetBranch("towerVsIso4")) t->SetBranchAddress("towerVsIso4", &tggHiNtuplizer.towerVsIso4, &tggHiNtuplizer.b_towerVsIso4);
    if (t->GetBranch("towerVsIso5")) t->SetBranchAddress("towerVsIso5", &tggHiNtuplizer.towerVsIso5, &tggHiNtuplizer.b_towerVsIso5);
    if (t->GetBranch("towerVsSubIso1")) t->SetBranchAddress("towerVsSubIso1", &tggHiNtuplizer.towerVsSubIso1, &tggHiNtuplizer.b_towerVsSubIso1);
    if (t->GetBranch("towerVsSubIso2")) t->SetBranchAddress("towerVsSubIso2", &tggHiNtuplizer.towerVsSubIso2, &tggHiNtuplizer.b_towerVsSubIso2);
    if (t->GetBranch("towerVsSubIso3")) t->SetBranchAddress("towerVsSubIso3", &tggHiNtuplizer.towerVsSubIso3, &tggHiNtuplizer.b_towerVsSubIso3);
    if (t->GetBranch("towerVsSubIso4")) t->SetBranchAddress("towerVsSubIso4", &tggHiNtuplizer.towerVsSubIso4, &tggHiNtuplizer.b_towerVsSubIso4);
    if (t->GetBranch("towerVsSubIso5")) t->SetBranchAddress("towerVsSubIso5", &tggHiNtuplizer.towerVsSubIso5, &tggHiNtuplizer.b_towerVsSubIso5);
    if (t->GetBranch("nMu")) t->SetBranchAddress("nMu", &tggHiNtuplizer.nMu, &tggHiNtuplizer.b_nMu);
    if (t->GetBranch("muPt")) t->SetBranchAddress("muPt", &tggHiNtuplizer.muPt, &tggHiNtuplizer.b_muPt);
    if (t->GetBranch("muEta")) t->SetBranchAddress("muEta", &tggHiNtuplizer.muEta, &tggHiNtuplizer.b_muEta);
    if (t->GetBranch("muPhi")) t->SetBranchAddress("muPhi", &tggHiNtuplizer.muPhi, &tggHiNtuplizer.b_muPhi);
    if (t->GetBranch("muCharge")) t->SetBranchAddress("muCharge", &tggHiNtuplizer.muCharge, &tggHiNtuplizer.b_muCharge);
    if (t->GetBranch("muType")) t->SetBranchAddress("muType", &tggHiNtuplizer.muType, &tggHiNtuplizer.b_muType);
    if (t->GetBranch("muIsGood")) t->SetBranchAddress("muIsGood", &tggHiNtuplizer.muIsGood, &tggHiNtuplizer.b_muIsGood);
    if (t->GetBranch("muD0")) t->SetBranchAddress("muD0", &tggHiNtuplizer.muD0, &tggHiNtuplizer.b_muD0);
    if (t->GetBranch("muDz")) t->SetBranchAddress("muDz", &tggHiNtuplizer.muDz, &tggHiNtuplizer.b_muDz);
    if (t->GetBranch("muChi2NDF")) t->SetBranchAddress("muChi2NDF", &tggHiNtuplizer.muChi2NDF, &tggHiNtuplizer.b_muChi2NDF);
    if (t->GetBranch("muInnerD0")) t->SetBranchAddress("muInnerD0", &tggHiNtuplizer.muInnerD0, &tggHiNtuplizer.b_muInnerD0);
    if (t->GetBranch("muInnerDz")) t->SetBranchAddress("muInnerDz", &tggHiNtuplizer.muInnerDz, &tggHiNtuplizer.b_muInnerDz);
    if (t->GetBranch("muTrkLayers")) t->SetBranchAddress("muTrkLayers", &tggHiNtuplizer.muTrkLayers, &tggHiNtuplizer.b_muTrkLayers);
    if (t->GetBranch("muPixelLayers")) t->SetBranchAddress("muPixelLayers", &tggHiNtuplizer.muPixelLayers, &tggHiNtuplizer.b_muPixelLayers);
    if (t->GetBranch("muPixelHits")) t->SetBranchAddress("muPixelHits", &tggHiNtuplizer.muPixelHits, &tggHiNtuplizer.b_muPixelHits);
    if (t->GetBranch("muMuonHits")) t->SetBranchAddress("muMuonHits", &tggHiNtuplizer.muMuonHits, &tggHiNtuplizer.b_muMuonHits);
    if (t->GetBranch("muTrkQuality")) t->SetBranchAddress("muTrkQuality", &tggHiNtuplizer.muTrkQuality, &tggHiNtuplizer.b_muTrkQuality);
    if (t->GetBranch("muStations")) t->SetBranchAddress("muStations", &tggHiNtuplizer.muStations, &tggHiNtuplizer.b_muStations);
    if (t->GetBranch("muIsoTrk")) t->SetBranchAddress("muIsoTrk", &tggHiNtuplizer.muIsoTrk, &tggHiNtuplizer.b_muIsoTrk);
    if (t->GetBranch("muPFChIso")) t->SetBranchAddress("muPFChIso", &tggHiNtuplizer.muPFChIso, &tggHiNtuplizer.b_muPFChIso);
    if (t->GetBranch("muPFPhoIso")) t->SetBranchAddress("muPFPhoIso", &tggHiNtuplizer.muPFPhoIso, &tggHiNtuplizer.b_muPFPhoIso);
    if (t->GetBranch("muPFNeuIso")) t->SetBranchAddress("muPFNeuIso", &tggHiNtuplizer.muPFNeuIso, &tggHiNtuplizer.b_muPFNeuIso);
    if (t->GetBranch("muPFPUIso")) t->SetBranchAddress("muPFPUIso", &tggHiNtuplizer.muPFPUIso, &tggHiNtuplizer.b_muPFPUIso);
}
