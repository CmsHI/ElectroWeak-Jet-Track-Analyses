#ifndef ggHiNtuplizerPhotonSkim
#define ggHiNtuplizerPhotonSkim

#include <TTree.h>
#include <vector>

class PhotonSkim{
public:
  PhotonSkim(bool mc){montecarlo = mc;};
  ~PhotonSkim(){};
  bool montecarlo;

  UInt_t          run;
  ULong64_t       event;
  UInt_t          lumis;

  Int_t hiBin;
  Int_t HBHENoiseFilterResult;
  Int_t pcollisionEventSelection;

  // mc stuff
  Int_t           nMC;
  Float_t         ptHat;
  std::vector<int>     mcPID;
  std::vector<int>     mcStatus;
  std::vector<float>   mcVtx_x;
  std::vector<float>   mcVtx_y;
  std::vector<float>   mcVtx_z;
  std::vector<float>   mcPt;
  std::vector<float>   mcEta;
  std::vector<float>   mcPhi;
  std::vector<float>   mcE;
  std::vector<float>   mcEt;
  std::vector<float>   mcMass;
  std::vector<int>     mcParentage;
  std::vector<int>     mcMomPID;
  std::vector<float>   mcMomPt;
  std::vector<float>   mcMomEta;
  std::vector<float>   mcMomPhi;
  std::vector<float>   mcMomMass;
  std::vector<int>     mcGMomPID;
  std::vector<int>     mcIndex;
  std::vector<float>   mcCalIsoDR03;
  std::vector<float>   mcCalIsoDR04;
  std::vector<float>   mcTrkIsoDR03;
  std::vector<float>   mcTrkIsoDR04;
  std::vector<int>     pho_genMatchedIndex;

  // reco::Photon
  Int_t          nPho;
  std::vector<float>  phoE;
  std::vector<float>  phoEt;
  std::vector<float>  phoEtCorrected; //only present after correction step.
  std::vector<float>  phoEta;
  std::vector<float>  phoPhi;
  std::vector<float>  phoSCE;
  std::vector<float>  phoSCRawE;
  std::vector<float>  phoESEn;
  std::vector<float>  phoSCEta;
  std::vector<float>  phoSCPhi;
  std::vector<float>  phoSCEtaWidth;
  std::vector<float>  phoSCPhiWidth;
  std::vector<float>  phoSCBrem;
  std::vector<int>    phohasPixelSeed;
// std::vector<int>    phoEleVeto;         // TODO: not available in reco::
  std::vector<float>  phoR9;
  std::vector<float>  phoHoverE;
  std::vector<float>  phoSigmaIEtaIEta;
// std::vector<float>  phoSigmaIEtaIPhi;   // TODO: not available in reco::
// std::vector<float>  phoSigmaIPhiIPhi;   // TODO: not available in reco::
  //std::vector<float>  phoE1x3;
  //std::vector<float>  phoE2x2;
  std::vector<float>  phoE3x3;
  //std::vector<float>  phoE2x5Max;
  std::vector<float>  phoE1x5;
  std::vector<float>  phoE2x5;
  std::vector<float>  phoE5x5;
  std::vector<float>  phoMaxEnergyXtal;
  std::vector<float>  phoSigmaEtaEta;
  std::vector<float>  phoR1x5;
  std::vector<float>  phoR2x5;
  std::vector<float>  phoESEffSigmaRR;
  std::vector<float>  phoSigmaIEtaIEta_2012;
  //std::vector<float>  phoSigmaIEtaIPhi_2012;
  //std::vector<float>  phoSigmaIPhiIPhi_2012;
  //std::vector<float>  phoE1x3_2012;
  //std::vector<float>  phoE2x2_2012;
  std::vector<float>  phoE3x3_2012;
  //std::vector<float>  phoE2x5Max_2012;
  //std::vector<float>  phoE5x5_2012;
  std::vector<float>  phoBC1E;
  std::vector<float>  phoBC1Eta;
  std::vector<float>  phoBC2E;
  std::vector<float>  phoBC2Eta;
  std::vector<float>  pho_ecalClusterIsoR2;
  std::vector<float>  pho_ecalClusterIsoR3;
  std::vector<float>  pho_ecalClusterIsoR4;
  std::vector<float>  pho_ecalClusterIsoR5;
  std::vector<float>  pho_hcalRechitIsoR1;
  std::vector<float>  pho_hcalRechitIsoR2;
  std::vector<float>  pho_hcalRechitIsoR3;
  std::vector<float>  pho_hcalRechitIsoR4;
  std::vector<float>  pho_hcalRechitIsoR5;
  std::vector<float>  pho_trackIsoR1PtCut20;
  std::vector<float>  pho_trackIsoR2PtCut20;
  std::vector<float>  pho_trackIsoR3PtCut20;
  std::vector<float>  pho_trackIsoR4PtCut20;
  std::vector<float>  pho_trackIsoR5PtCut20;
  std::vector<float>  pho_swissCrx;
  std::vector<float>  pho_seedTime;

  //photon pf isolation stuff
  std::vector<float> pfcIso1;
  std::vector<float> pfcIso2;
  std::vector<float> pfcIso3;
  std::vector<float> pfcIso4;
  std::vector<float> pfcIso5;

  std::vector<float> pfpIso1;
  std::vector<float> pfpIso2;
  std::vector<float> pfpIso3;
  std::vector<float> pfpIso4;
  std::vector<float> pfpIso5;

  std::vector<float> pfnIso1;
  std::vector<float> pfnIso2;
  std::vector<float> pfnIso3;
  std::vector<float> pfnIso4;
  std::vector<float> pfnIso5;

  std::vector<int>     *p_mcPID =0;
  std::vector<int>     *p_mcStatus =0;
  std::vector<float>   *p_mcVtx_x =0;
  std::vector<float>   *p_mcVtx_y =0;
  std::vector<float>   *p_mcVtx_z =0;
  std::vector<float>   *p_mcPt =0;
  std::vector<float>   *p_mcEta =0;
  std::vector<float>   *p_mcPhi =0;
  std::vector<float>   *p_mcE =0;
  std::vector<float>   *p_mcEt =0;
  std::vector<float>   *p_mcMass =0;
  std::vector<int>     *p_mcParentage =0;
  std::vector<int>     *p_mcMomPID =0;
  std::vector<float>   *p_mcMomPt =0;
  std::vector<float>   *p_mcMomEta =0;
  std::vector<float>   *p_mcMomPhi =0;
  std::vector<float>   *p_mcMomMass =0;
  std::vector<int>     *p_mcGMomPID =0;
  std::vector<int>     *p_mcIndex =0;
  std::vector<float>   *p_mcCalIsoDR03 =0;
  std::vector<float>   *p_mcCalIsoDR04 =0;
  std::vector<float>   *p_mcTrkIsoDR03 =0;
  std::vector<float>   *p_mcTrkIsoDR04 =0;
  std::vector<int>     *p_pho_genMatchedIndex =0;

  std::vector<float> *p_phoE = 0;
  std::vector<float> *p_phoEt = 0;
  std::vector<float> *p_phoEtCorrected = 0;
  std::vector<float> *p_phoEta = 0;
  std::vector<float> *p_phoPhi = 0;
  std::vector<float> *p_phoSCE = 0;
  std::vector<float> *p_phoSCRawE = 0;
  std::vector<float> *p_phoESEn = 0;
  std::vector<float> *p_phoSCEta = 0;
  std::vector<float> *p_phoSCPhi = 0;
  std::vector<float> *p_phoSCEtaWidth = 0;
  std::vector<float> *p_phoSCPhiWidth = 0;
  std::vector<float> *p_phoSCBrem = 0;
  std::vector<int> *p_phohasPixelSeed = 0;
  std::vector<float> *p_phoR9 = 0;
  std::vector<float> *p_phoHoverE = 0;
  std::vector<float> *p_phoSigmaIEtaIEta = 0;
  //std::vector<float> *p_phoE1x3 = 0;
  //std::vector<float> *p_phoE2x2 = 0;
  std::vector<float> *p_phoE3x3 = 0;
  //std::vector<float> *p_phoE2x5Max = 0;
  std::vector<float> *p_phoE1x5 = 0;
  std::vector<float> *p_phoE2x5 = 0;
  std::vector<float> *p_phoE5x5 = 0;
  std::vector<float> *p_phoMaxEnergyXtal = 0;
  std::vector<float> *p_phoSigmaEtaEta = 0;
  std::vector<float> *p_phoR1x5 = 0;
  std::vector<float> *p_phoR2x5 = 0;
  std::vector<float> *p_phoESEffSigmaRR = 0;
  std::vector<float> *p_phoSigmaIEtaIEta_2012 = 0;
  //std::vector<float> *p_phoSigmaIEtaIPhi_2012 = 0;
  //std::vector<float> *p_phoSigmaIPhiIPhi_2012 = 0;
  //std::vector<float> *p_phoE1x3_2012 = 0;
  //std::vector<float> *p_phoE2x2_2012 = 0;
  std::vector<float> *p_phoE3x3_2012 = 0;
  //std::vector<float> *p_phoE2x5Max_2012 = 0;
  //std::vector<float> *p_phoE5x5_2012 = 0;
  std::vector<float> *p_phoBC1E = 0;
  std::vector<float> *p_phoBC1Eta = 0;
  std::vector<float> *p_phoBC2E = 0;
  std::vector<float> *p_phoBC2Eta = 0;
  std::vector<float> *p_pho_ecalClusterIsoR2 = 0;
  std::vector<float> *p_pho_ecalClusterIsoR3 = 0;
  std::vector<float> *p_pho_ecalClusterIsoR4 = 0;
  std::vector<float> *p_pho_ecalClusterIsoR5 = 0;
  std::vector<float> *p_pho_hcalRechitIsoR1 = 0;
  std::vector<float> *p_pho_hcalRechitIsoR2 = 0;
  std::vector<float> *p_pho_hcalRechitIsoR3 = 0;
  std::vector<float> *p_pho_hcalRechitIsoR4 = 0;
  std::vector<float> *p_pho_hcalRechitIsoR5 = 0;
  std::vector<float> *p_pho_trackIsoR1PtCut20 = 0;
  std::vector<float> *p_pho_trackIsoR2PtCut20 = 0;
  std::vector<float> *p_pho_trackIsoR3PtCut20 = 0;
  std::vector<float> *p_pho_trackIsoR4PtCut20 = 0;
  std::vector<float> *p_pho_trackIsoR5PtCut20 = 0;
  std::vector<float> *p_pho_swissCrx = 0;
  std::vector<float> *p_pho_seedTime = 0;
  std::vector<float> *p_pfcIso1 = 0;
  std::vector<float> *p_pfcIso2 = 0;
  std::vector<float> *p_pfcIso3 = 0;
  std::vector<float> *p_pfcIso4 = 0;
  std::vector<float> *p_pfcIso5 = 0;
  std::vector<float> *p_pfpIso1 = 0;
  std::vector<float> *p_pfpIso2 = 0;
  std::vector<float> *p_pfpIso3 = 0;
  std::vector<float> *p_pfpIso4 = 0;
  std::vector<float> *p_pfpIso5 = 0;
  std::vector<float> *p_pfnIso1 = 0;
  std::vector<float> *p_pfnIso2 = 0;
  std::vector<float> *p_pfnIso3 = 0;
  std::vector<float> *p_pfnIso4 = 0;
  std::vector<float> *p_pfnIso5 = 0;

  void reset(){

    run=0;
    event=0;
    lumis=0;
    hiBin = -1;
    HBHENoiseFilterResult=-1;
    pcollisionEventSelection=-1;

    nMC = 0;
    ptHat = -1;
    mcPID.clear();
    mcStatus.clear();
    mcVtx_x.clear();
    mcVtx_y.clear();
    mcVtx_z.clear();
    mcPt.clear();
    mcEta.clear();
    mcPhi.clear();
    mcE.clear();
    mcEt.clear();
    mcMass.clear();
    mcParentage.clear();
    mcMomPID.clear();
    mcMomPt.clear();
    mcMomEta.clear();
    mcMomPhi.clear();
    mcMomMass.clear();
    mcGMomPID.clear();
    mcIndex.clear();
    mcCalIsoDR03.clear();
    mcCalIsoDR04.clear();
    mcTrkIsoDR03.clear();
    mcTrkIsoDR04.clear();
    pho_genMatchedIndex.clear();

    nPho=0;

    phoE.clear();
    phoEt.clear();
    phoEtCorrected.clear();
    phoEta.clear();
    phoPhi.clear();
    phoSCE.clear();
    phoSCRawE.clear();
    phoESEn.clear();
    phoSCEta.clear();
    phoSCPhi.clear();
    phoSCEtaWidth.clear();
    phoSCPhiWidth.clear();
    phoSCBrem.clear();
    phohasPixelSeed.clear();
    phoR9.clear();
    phoHoverE.clear();
    phoSigmaIEtaIEta.clear();
    //phoE1x3.clear();
    //phoE2x2.clear();
    phoE3x3.clear();
    //phoE2x5Max.clear();
    phoE1x5.clear();
    phoE2x5.clear();
    phoE5x5.clear();
    phoMaxEnergyXtal.clear();
    phoSigmaEtaEta.clear();
    phoR1x5.clear();
    phoR2x5.clear();
    phoESEffSigmaRR.clear();
    phoSigmaIEtaIEta_2012.clear();
    //phoSigmaIEtaIPhi_2012.clear();
    //phoSigmaIPhiIPhi_2012.clear();
    //phoE1x3_2012.clear();
    //phoE2x2_2012.clear();
    phoE3x3_2012.clear();
    //phoE2x5Max_2012.clear();
    //phoE5x5_2012.clear();
    phoBC1E.clear();
    phoBC1Eta.clear();
    phoBC2E.clear();
    phoBC2Eta.clear();
    pho_ecalClusterIsoR2.clear();
    pho_ecalClusterIsoR3.clear();
    pho_ecalClusterIsoR4.clear();
    pho_ecalClusterIsoR5.clear();
    pho_hcalRechitIsoR1.clear();
    pho_hcalRechitIsoR2.clear();
    pho_hcalRechitIsoR3.clear();
    pho_hcalRechitIsoR4.clear();
    pho_hcalRechitIsoR5.clear();
    pho_trackIsoR1PtCut20.clear();
    pho_trackIsoR2PtCut20.clear();
    pho_trackIsoR3PtCut20.clear();
    pho_trackIsoR4PtCut20.clear();
    pho_trackIsoR5PtCut20.clear();
    pho_swissCrx.clear();
    pho_seedTime.clear();

    pfcIso1.clear();
    pfcIso2.clear();
    pfcIso3.clear();
    pfcIso4.clear();
    pfcIso5.clear();

    pfpIso1.clear();
    pfpIso2.clear();
    pfpIso3.clear();
    pfpIso4.clear();
    pfpIso5.clear();

    pfnIso1.clear();
    pfnIso2.clear();
    pfnIso3.clear();
    pfnIso4.clear();
    pfnIso5.clear();
  };

  void setupTreeForWriting(TTree * tree_)
  {
    tree_->Branch("run",    &run, "run/i");
    tree_->Branch("event",  &event, "event/l");
    tree_->Branch("lumis",  &lumis, "lumi/i");
    tree_->Branch("hiBin",  &hiBin, "hiBin/I");
    tree_->Branch("HBHENoiseFilterResult", &HBHENoiseFilterResult);
    tree_->Branch("pcollisionEventSelection", &pcollisionEventSelection);

    if(montecarlo){
      tree_->Branch("nMC",&nMC);
      tree_->Branch("ptHat",&ptHat);
      tree_->Branch("mcPID",&mcPID);
      tree_->Branch("mcStatus",&mcStatus);
      tree_->Branch("mcVtx_x",&mcVtx_x);
      tree_->Branch("mcVtx_y",&mcVtx_y);
      tree_->Branch("mcVtx_z",&mcVtx_z);
      tree_->Branch("mcPt",&mcPt);
      tree_->Branch("mcEta",&mcEta);
      tree_->Branch("mcPhi",&mcPhi);
      tree_->Branch("mcE",&mcE);
      tree_->Branch("mcEt",&mcEt);
      tree_->Branch("mcMass",&mcMass);
      tree_->Branch("mcParentage",&mcParentage);
      tree_->Branch("mcMomPID",&mcMomPID);
      tree_->Branch("mcMomPt",&mcMomPt);
      tree_->Branch("mcMomEta",&mcMomEta);
      tree_->Branch("mcMomPhi",&mcMomPhi);
      tree_->Branch("mcMomMass",&mcMomMass);
      tree_->Branch("mcGMomPID",&mcGMomPID);
      tree_->Branch("mcIndex",&mcIndex);
      tree_->Branch("mcCalIsoDR03",&mcCalIsoDR03);
      tree_->Branch("mcCalIsoDR04",&mcCalIsoDR04);
      tree_->Branch("mcTrkIsoDR03",&mcTrkIsoDR03);
      tree_->Branch("mcTrkIsoDR04",&mcTrkIsoDR04);
      tree_->Branch("pho_genMatchedIndex",&pho_genMatchedIndex);
    }

    tree_->Branch("nPho",                  &nPho);
    tree_->Branch("phoE",                  &phoE);
    tree_->Branch("phoEt",                 &phoEt);
    tree_->Branch("phoEtCorrected",        &phoEtCorrected);
    tree_->Branch("phoEta",                &phoEta);
    tree_->Branch("phoPhi",                &phoPhi);
    tree_->Branch("phoSCE",                &phoSCE);
    tree_->Branch("phoSCRawE",             &phoSCRawE);
    tree_->Branch("phoESEn",               &phoESEn);
    tree_->Branch("phoSCEta",              &phoSCEta);
    tree_->Branch("phoSCPhi",              &phoSCPhi);
    tree_->Branch("phoSCEtaWidth",         &phoSCEtaWidth);
    tree_->Branch("phoSCPhiWidth",         &phoSCPhiWidth);
    tree_->Branch("phoSCBrem",             &phoSCBrem);
    tree_->Branch("phohasPixelSeed",       &phohasPixelSeed);
// tree_->Branch("phoEleVeto",            &phoEleVeto);        // TODO: not available in reco::
    tree_->Branch("phoR9",                 &phoR9);
    tree_->Branch("phoHoverE",             &phoHoverE);
    tree_->Branch("phoSigmaIEtaIEta",      &phoSigmaIEtaIEta);
// tree_->Branch("phoSigmaIEtaIPhi",      &phoSigmaIEtaIPhi);  // TODO: not available in reco::
// tree_->Branch("phoSigmaIPhiIPhi",      &phoSigmaIPhiIPhi);  // TODO: not available in reco::
    //tree_->Branch("phoE1x3",               &phoE1x3);
    //tree_->Branch("phoE2x2",               &phoE2x2);
    tree_->Branch("phoE3x3",               &phoE3x3);
    //tree_->Branch("phoE2x5Max",            &phoE2x5Max);
    tree_->Branch("phoE1x5",               &phoE1x5);
    tree_->Branch("phoE2x5",               &phoE2x5);
    tree_->Branch("phoE5x5",               &phoE5x5);
    tree_->Branch("phoMaxEnergyXtal",      &phoMaxEnergyXtal);
    tree_->Branch("phoSigmaEtaEta",        &phoSigmaEtaEta);
    tree_->Branch("phoR1x5",               &phoR1x5);
    tree_->Branch("phoR2x5",               &phoR2x5);
    tree_->Branch("phoESEffSigmaRR",       &phoESEffSigmaRR);
    tree_->Branch("phoSigmaIEtaIEta_2012", &phoSigmaIEtaIEta_2012);
    //tree_->Branch("phoSigmaIEtaIPhi_2012", &phoSigmaIEtaIPhi_2012);
    //tree_->Branch("phoSigmaIPhiIPhi_2012", &phoSigmaIPhiIPhi_2012);
    //tree_->Branch("phoE1x3_2012",          &phoE1x3_2012);
    //tree_->Branch("phoE2x2_2012",          &phoE2x2_2012);
    tree_->Branch("phoE3x3_2012",          &phoE3x3_2012);
    //tree_->Branch("phoE2x5Max_2012",       &phoE2x5Max_2012);
    //tree_->Branch("phoE5x5_2012",          &phoE5x5_2012);
    tree_->Branch("phoBC1E",               &phoBC1E);
    tree_->Branch("phoBC1Eta",             &phoBC1Eta);
    tree_->Branch("phoBC2E",               &phoBC2E);
    tree_->Branch("phoBC2Eta",             &phoBC2Eta);
    tree_->Branch("pho_ecalClusterIsoR2", &pho_ecalClusterIsoR2);
    tree_->Branch("pho_ecalClusterIsoR3", &pho_ecalClusterIsoR3);
    tree_->Branch("pho_ecalClusterIsoR4", &pho_ecalClusterIsoR4);
    tree_->Branch("pho_ecalClusterIsoR5", &pho_ecalClusterIsoR5);
    tree_->Branch("pho_hcalRechitIsoR1", &pho_hcalRechitIsoR1);
    tree_->Branch("pho_hcalRechitIsoR2", &pho_hcalRechitIsoR2);
    tree_->Branch("pho_hcalRechitIsoR3", &pho_hcalRechitIsoR3);
    tree_->Branch("pho_hcalRechitIsoR4", &pho_hcalRechitIsoR4);
    tree_->Branch("pho_hcalRechitIsoR5", &pho_hcalRechitIsoR5);
    tree_->Branch("pho_trackIsoR1PtCut20", &pho_trackIsoR1PtCut20);
    tree_->Branch("pho_trackIsoR2PtCut20", &pho_trackIsoR2PtCut20);
    tree_->Branch("pho_trackIsoR3PtCut20", &pho_trackIsoR3PtCut20);
    tree_->Branch("pho_trackIsoR4PtCut20", &pho_trackIsoR4PtCut20);
    tree_->Branch("pho_trackIsoR5PtCut20", &pho_trackIsoR5PtCut20);
    tree_->Branch("pho_swissCrx", &pho_swissCrx);
    tree_->Branch("pho_seedTime", &pho_seedTime);

    //if(doPfIso_)
    {
      tree_->Branch("pfcIso1",&pfcIso1);
      tree_->Branch("pfcIso2",&pfcIso2);
      tree_->Branch("pfcIso3",&pfcIso3);
      tree_->Branch("pfcIso4",&pfcIso4);
      tree_->Branch("pfcIso5",&pfcIso5);

      tree_->Branch("pfpIso1",&pfpIso1);
      tree_->Branch("pfpIso2",&pfpIso2);
      tree_->Branch("pfpIso3",&pfpIso3);
      tree_->Branch("pfpIso4",&pfpIso4);
      tree_->Branch("pfpIso5",&pfpIso5);

      tree_->Branch("pfnIso1",&pfnIso1);
      tree_->Branch("pfnIso2",&pfnIso2);
      tree_->Branch("pfnIso3",&pfnIso3);
      tree_->Branch("pfnIso4",&pfnIso4);
      tree_->Branch("pfnIso5",&pfnIso5);
    }
  }

  void setupTreeForReading(TTree * tree_)
  {
    tree_->SetBranchAddress("run",    &run);
    tree_->SetBranchAddress("event",  &event);
    tree_->SetBranchAddress("lumis",  &lumis);
    tree_->SetBranchAddress("hiBin",  &hiBin);
    tree_->SetBranchAddress("HBHENoiseFilterResult", &HBHENoiseFilterResult);
    tree_->SetBranchAddress("pcollisionEventSelection", &pcollisionEventSelection);

    if(montecarlo){
      tree_->SetBranchAddress("nMC",&nMC);
      tree_->SetBranchAddress("ptHat",&ptHat);
      tree_->SetBranchAddress("mcPID",&p_mcPID);
      tree_->SetBranchAddress("mcStatus",&p_mcStatus);
      tree_->SetBranchAddress("mcVtx_x",&p_mcVtx_x);
      tree_->SetBranchAddress("mcVtx_y",&p_mcVtx_y);
      tree_->SetBranchAddress("mcVtx_z",&p_mcVtx_z);
      tree_->SetBranchAddress("mcPt",&p_mcPt);
      tree_->SetBranchAddress("mcEta",&p_mcEta);
      tree_->SetBranchAddress("mcPhi",&p_mcPhi);
      tree_->SetBranchAddress("mcE",&p_mcE);
      tree_->SetBranchAddress("mcEt",&p_mcEt);
      tree_->SetBranchAddress("mcMass",&p_mcMass);
      tree_->SetBranchAddress("mcParentage",&p_mcParentage);
      tree_->SetBranchAddress("mcMomPID",&p_mcMomPID);
      tree_->SetBranchAddress("mcMomPt",&p_mcMomPt);
      tree_->SetBranchAddress("mcMomEta",&p_mcMomEta);
      tree_->SetBranchAddress("mcMomPhi",&p_mcMomPhi);
      tree_->SetBranchAddress("mcMomMass",&p_mcMomMass);
      tree_->SetBranchAddress("mcGMomPID",&p_mcGMomPID);
      tree_->SetBranchAddress("mcIndex",&p_mcIndex);
      tree_->SetBranchAddress("mcCalIsoDR03",&p_mcCalIsoDR03);
      tree_->SetBranchAddress("mcCalIsoDR04",&p_mcCalIsoDR04);
      tree_->SetBranchAddress("mcTrkIsoDR03",&p_mcTrkIsoDR03);
      tree_->SetBranchAddress("mcTrkIsoDR04",&p_mcTrkIsoDR04);
      tree_->SetBranchAddress("pho_genMatchedIndex",&p_pho_genMatchedIndex);
    }

    tree_->SetBranchAddress("nPho",                  &nPho);
    tree_->SetBranchAddress("phoE",                  &p_phoE);
    tree_->SetBranchAddress("phoEt",                 &p_phoEt);
    tree_->SetBranchAddress("phoEtCorrected",        &p_phoEtCorrected);
    tree_->SetBranchAddress("phoEta",                &p_phoEta);
    tree_->SetBranchAddress("phoPhi",                &p_phoPhi);
    tree_->SetBranchAddress("phoSCE",                &p_phoSCE);
    tree_->SetBranchAddress("phoSCRawE",             &p_phoSCRawE);
    tree_->SetBranchAddress("phoESEn",               &p_phoESEn);
    tree_->SetBranchAddress("phoSCEta",              &p_phoSCEta);
    tree_->SetBranchAddress("phoSCPhi",              &p_phoSCPhi);
    tree_->SetBranchAddress("phoSCEtaWidth",         &p_phoSCEtaWidth);
    tree_->SetBranchAddress("phoSCPhiWidth",         &p_phoSCPhiWidth);
    tree_->SetBranchAddress("phoSCBrem",             &p_phoSCBrem);
    tree_->SetBranchAddress("phohasPixelSeed",       &p_phohasPixelSeed);
// tree_->SetBranchAddress("phoEleVeto",            &p_phoEleVeto);        // TODO: not available in reco::
    tree_->SetBranchAddress("phoR9",                 &p_phoR9);
    tree_->SetBranchAddress("phoHoverE",             &p_phoHoverE);
    tree_->SetBranchAddress("phoSigmaIEtaIEta",      &p_phoSigmaIEtaIEta);
// tree_->SetBranchAddress("phoSigmaIEtaIPhi",      &p_phoSigmaIEtaIPhi);  // TODO: not available in reco::
// tree_->SetBranchAddress("phoSigmaIPhiIPhi",      &p_phoSigmaIPhiIPhi);  // TODO: not available in reco::
    //tree_->SetBranchAddress("phoE1x3",               &p_phoE1x3);
    //tree_->SetBranchAddress("phoE2x2",               &p_phoE2x2);
    tree_->SetBranchAddress("phoE3x3",               &p_phoE3x3);
    //tree_->SetBranchAddress("phoE2x5Max",            &p_phoE2x5Max);
    tree_->SetBranchAddress("phoE1x5",               &p_phoE1x5);
    tree_->SetBranchAddress("phoE2x5",               &p_phoE2x5);
    tree_->SetBranchAddress("phoE5x5",               &p_phoE5x5);
    tree_->SetBranchAddress("phoMaxEnergyXtal",      &p_phoMaxEnergyXtal);
    tree_->SetBranchAddress("phoSigmaEtaEta",        &p_phoSigmaEtaEta);
    tree_->SetBranchAddress("phoR1x5",               &p_phoR1x5);
    tree_->SetBranchAddress("phoR2x5",               &p_phoR2x5);
    tree_->SetBranchAddress("phoESEffSigmaRR",       &p_phoESEffSigmaRR);
    tree_->SetBranchAddress("phoSigmaIEtaIEta_2012", &p_phoSigmaIEtaIEta_2012);
    // tree_->SetBranchAddress("phoSigmaIEtaIPhi_2012", &p_phoSigmaIEtaIPhi_2012);
    // tree_->SetBranchAddress("phoSigmaIPhiIPhi_2012", &p_phoSigmaIPhiIPhi_2012);
    // tree_->SetBranchAddress("phoE1x3_2012",          &p_phoE1x3_2012);
    // tree_->SetBranchAddress("phoE2x2_2012",          &p_phoE2x2_2012);
    tree_->SetBranchAddress("phoE3x3_2012",          &p_phoE3x3_2012);
    // tree_->SetBranchAddress("phoE2x5Max_2012",       &p_phoE2x5Max_2012);
    // tree_->SetBranchAddress("phoE5x5_2012",          &p_phoE5x5_2012);
    tree_->SetBranchAddress("phoBC1E",               &p_phoBC1E);
    tree_->SetBranchAddress("phoBC1Eta",             &p_phoBC1Eta);
    tree_->SetBranchAddress("phoBC2E",               &p_phoBC2E);
    tree_->SetBranchAddress("phoBC2Eta",             &p_phoBC2Eta);
    tree_->SetBranchAddress("pho_ecalClusterIsoR2", &p_pho_ecalClusterIsoR2);
    tree_->SetBranchAddress("pho_ecalClusterIsoR3", &p_pho_ecalClusterIsoR3);
    tree_->SetBranchAddress("pho_ecalClusterIsoR4", &p_pho_ecalClusterIsoR4);
    tree_->SetBranchAddress("pho_ecalClusterIsoR5", &p_pho_ecalClusterIsoR5);
    tree_->SetBranchAddress("pho_hcalRechitIsoR1", &p_pho_hcalRechitIsoR1);
    tree_->SetBranchAddress("pho_hcalRechitIsoR2", &p_pho_hcalRechitIsoR2);
    tree_->SetBranchAddress("pho_hcalRechitIsoR3", &p_pho_hcalRechitIsoR3);
    tree_->SetBranchAddress("pho_hcalRechitIsoR4", &p_pho_hcalRechitIsoR4);
    tree_->SetBranchAddress("pho_hcalRechitIsoR5", &p_pho_hcalRechitIsoR5);
    tree_->SetBranchAddress("pho_trackIsoR1PtCut20", &p_pho_trackIsoR1PtCut20);
    tree_->SetBranchAddress("pho_trackIsoR2PtCut20", &p_pho_trackIsoR2PtCut20);
    tree_->SetBranchAddress("pho_trackIsoR3PtCut20", &p_pho_trackIsoR3PtCut20);
    tree_->SetBranchAddress("pho_trackIsoR4PtCut20", &p_pho_trackIsoR4PtCut20);
    tree_->SetBranchAddress("pho_trackIsoR5PtCut20", &p_pho_trackIsoR5PtCut20);
    tree_->SetBranchAddress("pho_swissCrx", &p_pho_swissCrx);
    tree_->SetBranchAddress("pho_seedTime", &p_pho_seedTime);

    //if(doPfIso_)
    {
      tree_->SetBranchAddress("pfcIso1",&p_pfcIso1);
      tree_->SetBranchAddress("pfcIso2",&p_pfcIso2);
      tree_->SetBranchAddress("pfcIso3",&p_pfcIso3);
      tree_->SetBranchAddress("pfcIso4",&p_pfcIso4);
      tree_->SetBranchAddress("pfcIso5",&p_pfcIso5);

      tree_->SetBranchAddress("pfpIso1",&p_pfpIso1);
      tree_->SetBranchAddress("pfpIso2",&p_pfpIso2);
      tree_->SetBranchAddress("pfpIso3",&p_pfpIso3);
      tree_->SetBranchAddress("pfpIso4",&p_pfpIso4);
      tree_->SetBranchAddress("pfpIso5",&p_pfpIso5);

      tree_->SetBranchAddress("pfnIso1",&p_pfnIso1);
      tree_->SetBranchAddress("pfnIso2",&p_pfnIso2);
      tree_->SetBranchAddress("pfnIso3",&p_pfnIso3);
      tree_->SetBranchAddress("pfnIso4",&p_pfnIso4);
      tree_->SetBranchAddress("pfnIso5",&p_pfnIso5);
    }
  }

  void resolvePointers()
  {
    if(montecarlo){
      mcPID = *p_mcPID;
      mcStatus = *p_mcStatus;
      mcVtx_x = *p_mcVtx_x;
      mcVtx_y = *p_mcVtx_y;
      mcVtx_z = *p_mcVtx_z;
      mcPt = *p_mcPt;
      mcEta = *p_mcEta;
      mcPhi = *p_mcPhi;
      mcE = *p_mcE;
      mcEt = *p_mcEt;
      mcMass = *p_mcMass;
      mcParentage = *p_mcParentage;
      mcMomPID = *p_mcMomPID;
      mcMomPt = *p_mcMomPt;
      mcMomEta = *p_mcMomEta;
      mcMomPhi = *p_mcMomPhi;
      mcMomMass = *p_mcMomMass;
      mcGMomPID = *p_mcGMomPID;
      mcIndex = *p_mcIndex;
      mcCalIsoDR03 = *p_mcCalIsoDR03;
      mcCalIsoDR04 = *p_mcCalIsoDR04;
      mcTrkIsoDR03 = *p_mcTrkIsoDR03;
      mcTrkIsoDR04 = *p_mcTrkIsoDR04;
      pho_genMatchedIndex = *p_pho_genMatchedIndex;
    }

    phoE = *p_phoE;
    phoEt = *p_phoEt;
    phoEtCorrected = *p_phoEtCorrected;
    phoEta = *p_phoEta;
    phoPhi = *p_phoPhi;
    phoSCE = *p_phoSCE;
    phoSCRawE = *p_phoSCRawE;
    phoESEn = *p_phoESEn;
    phoSCEta = *p_phoSCEta;
    phoSCPhi = *p_phoSCPhi;
    phoSCEtaWidth = *p_phoSCEtaWidth;
    phoSCPhiWidth = *p_phoSCPhiWidth;
    phoSCBrem = *p_phoSCBrem;
    phohasPixelSeed = *p_phohasPixelSeed;
    phoR9 = *p_phoR9;
    phoHoverE = *p_phoHoverE;
    phoSigmaIEtaIEta = *p_phoSigmaIEtaIEta;
    //phoE1x3 = *p_phoE1x3;
    //phoE2x2 = *p_phoE2x2;
    phoE3x3 = *p_phoE3x3;
    //phoE2x5Max = *p_phoE2x5Max;
    phoE1x5 = *p_phoE1x5;
    phoE2x5 = *p_phoE2x5;
    phoE5x5 = *p_phoE5x5;
    phoMaxEnergyXtal = *p_phoMaxEnergyXtal;
    phoSigmaEtaEta = *p_phoSigmaEtaEta;
    phoR1x5 = *p_phoR1x5;
    phoR2x5 = *p_phoR2x5;
    phoESEffSigmaRR = *p_phoESEffSigmaRR;
    phoSigmaIEtaIEta_2012 = *p_phoSigmaIEtaIEta_2012;
    // phoSigmaIEtaIPhi_2012 = *p_phoSigmaIEtaIPhi_2012;
    // phoSigmaIPhiIPhi_2012 = *p_phoSigmaIPhiIPhi_2012;
    // phoE1x3_2012 = *p_phoE1x3_2012;
    // phoE2x2_2012 = *p_phoE2x2_2012;
    phoE3x3_2012 = *p_phoE3x3_2012;
    // phoE2x5Max_2012 = *p_phoE2x5Max_2012;
    // phoE5x5_2012 = *p_phoE5x5_2012;
    phoBC1E = *p_phoBC1E;
    phoBC1Eta = *p_phoBC1Eta;
    phoBC2E = *p_phoBC2E;
    phoBC2Eta = *p_phoBC2Eta;
    pho_ecalClusterIsoR2 = *p_pho_ecalClusterIsoR2;
    pho_ecalClusterIsoR3 = *p_pho_ecalClusterIsoR3;
    pho_ecalClusterIsoR4 = *p_pho_ecalClusterIsoR4;
    pho_ecalClusterIsoR5 = *p_pho_ecalClusterIsoR5;
    pho_hcalRechitIsoR1 = *p_pho_hcalRechitIsoR1;
    pho_hcalRechitIsoR2 = *p_pho_hcalRechitIsoR2;
    pho_hcalRechitIsoR3 = *p_pho_hcalRechitIsoR3;
    pho_hcalRechitIsoR4 = *p_pho_hcalRechitIsoR4;
    pho_hcalRechitIsoR5 = *p_pho_hcalRechitIsoR5;
    pho_trackIsoR1PtCut20 = *p_pho_trackIsoR1PtCut20;
    pho_trackIsoR2PtCut20 = *p_pho_trackIsoR2PtCut20;
    pho_trackIsoR3PtCut20 = *p_pho_trackIsoR3PtCut20;
    pho_trackIsoR4PtCut20 = *p_pho_trackIsoR4PtCut20;
    pho_trackIsoR5PtCut20 = *p_pho_trackIsoR5PtCut20;
    pho_swissCrx = *p_pho_swissCrx;
    pho_seedTime = *p_pho_seedTime;
    pfcIso1 = *p_pfcIso1;
    pfcIso2 = *p_pfcIso2;
    pfcIso3 = *p_pfcIso3;
    pfcIso4 = *p_pfcIso4;
    pfcIso5 = *p_pfcIso5;
    pfpIso1 = *p_pfpIso1;
    pfpIso2 = *p_pfpIso2;
    pfpIso3 = *p_pfpIso3;
    pfpIso4 = *p_pfpIso4;
    pfpIso5 = *p_pfpIso5;
    pfnIso1 = *p_pfnIso1;
    pfnIso2 = *p_pfnIso2;
    pfnIso3 = *p_pfnIso3;
    pfnIso4 = *p_pfnIso4;
    pfnIso5 = *p_pfnIso5;
  }
};
#endif
