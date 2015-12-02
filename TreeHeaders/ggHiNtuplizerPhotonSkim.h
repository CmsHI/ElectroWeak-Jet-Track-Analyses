#ifndef ggHiNtuplizerPhotonSkim
#define ggHiNtuplizerPhotonSkim

#include <TTree.h>
#include <vector>

class PhotonSkim{
public:
  PhotonSkim(){};
  ~PhotonSkim(){};
  UInt_t          run;
  ULong64_t       event;
  UInt_t          lumis;

  Int_t HBHENoiseFilterResult;
  Int_t pcollisionEventSelection;

  // reco::Photon
  Int_t          nPho;
  std::vector<float>  phoE;
  std::vector<float>  phoEt;
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
  std::vector<float>  phoE1x3;
  std::vector<float>  phoE2x2;
  std::vector<float>  phoE3x3;
  std::vector<float>  phoE2x5Max;
  std::vector<float>  phoE1x5;
  std::vector<float>  phoE2x5;
  std::vector<float>  phoE5x5;
  std::vector<float>  phoMaxEnergyXtal;
  std::vector<float>  phoSigmaEtaEta;
  std::vector<float>  phoR1x5;
  std::vector<float>  phoR2x5;
  std::vector<float>  phoESEffSigmaRR;
  std::vector<float>  phoSigmaIEtaIEta_2012;
  std::vector<float>  phoSigmaIEtaIPhi_2012;
  std::vector<float>  phoSigmaIPhiIPhi_2012;
  std::vector<float>  phoE1x3_2012;
  std::vector<float>  phoE2x2_2012;
  std::vector<float>  phoE3x3_2012;
  std::vector<float>  phoE2x5Max_2012;
  std::vector<float>  phoE5x5_2012;
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

  std::vector<float> pfsumIso1;
  std::vector<float> pfsumIso2;
  std::vector<float> pfsumIso3;
  std::vector<float> pfsumIso4;
  std::vector<float> pfsumIso5;

  std::vector<float> pfcVsIso1;
  std::vector<float> pfcVsIso2;
  std::vector<float> pfcVsIso3;
  std::vector<float> pfcVsIso4;
  std::vector<float> pfcVsIso5;
  std::vector<float> pfcVsIso1th1;
  std::vector<float> pfcVsIso2th1;
  std::vector<float> pfcVsIso3th1;
  std::vector<float> pfcVsIso4th1;
  std::vector<float> pfcVsIso5th1;
  std::vector<float> pfcVsIso1th2;
  std::vector<float> pfcVsIso2th2;
  std::vector<float> pfcVsIso3th2;
  std::vector<float> pfcVsIso4th2;
  std::vector<float> pfcVsIso5th2;

  std::vector<float> pfnVsIso1;
  std::vector<float> pfnVsIso2;
  std::vector<float> pfnVsIso3;
  std::vector<float> pfnVsIso4;
  std::vector<float> pfnVsIso5;
  std::vector<float> pfnVsIso1th1;
  std::vector<float> pfnVsIso2th1;
  std::vector<float> pfnVsIso3th1;
  std::vector<float> pfnVsIso4th1;
  std::vector<float> pfnVsIso5th1;
  std::vector<float> pfnVsIso1th2;
  std::vector<float> pfnVsIso2th2;
  std::vector<float> pfnVsIso3th2;
  std::vector<float> pfnVsIso4th2;
  std::vector<float> pfnVsIso5th2;


  std::vector<float> pfpVsIso1;
  std::vector<float> pfpVsIso2;
  std::vector<float> pfpVsIso3;
  std::vector<float> pfpVsIso4;
  std::vector<float> pfpVsIso5;
  std::vector<float> pfpVsIso1th1;
  std::vector<float> pfpVsIso2th1;
  std::vector<float> pfpVsIso3th1;
  std::vector<float> pfpVsIso4th1;
  std::vector<float> pfpVsIso5th1;
  std::vector<float> pfpVsIso1th2;
  std::vector<float> pfpVsIso2th2;
  std::vector<float> pfpVsIso3th2;
  std::vector<float> pfpVsIso4th2;
  std::vector<float> pfpVsIso5th2;


  std::vector<float> pfsumVsIso1;
  std::vector<float> pfsumVsIso2;
  std::vector<float> pfsumVsIso3;
  std::vector<float> pfsumVsIso4;
  std::vector<float> pfsumVsIso5;
  std::vector<float> pfsumVsIso1th1;
  std::vector<float> pfsumVsIso2th1;
  std::vector<float> pfsumVsIso3th1;
  std::vector<float> pfsumVsIso4th1;
  std::vector<float> pfsumVsIso5th1;
  std::vector<float> pfsumVsIso1th2;
  std::vector<float> pfsumVsIso2th2;
  std::vector<float> pfsumVsIso3th2;
  std::vector<float> pfsumVsIso4th2;
  std::vector<float> pfsumVsIso5th2;


  std::vector<float> pfVsSubIso1;
  std::vector<float> pfVsSubIso2;
  std::vector<float> pfVsSubIso3;
  std::vector<float> pfVsSubIso4;
  std::vector<float> pfVsSubIso5;

  std::vector<float> towerIso1;
  std::vector<float> towerIso2;
  std::vector<float> towerIso3;
  std::vector<float> towerIso4;
  std::vector<float> towerIso5;
  std::vector<float> towerVsIso1;
  std::vector<float> towerVsIso2;
  std::vector<float> towerVsIso3;
  std::vector<float> towerVsIso4;
  std::vector<float> towerVsIso5;
  std::vector<float> towerVsSubIso1;
  std::vector<float> towerVsSubIso2;
  std::vector<float> towerVsSubIso3;
  std::vector<float> towerVsSubIso4;
  std::vector<float> towerVsSubIso5;

  void reset(){

    run=0;
    event=0;
    lumis=0;
    HBHENoiseFilterResult=-1;
    pcollisionEventSelection=-1;

    nPho=-1;

    phoE.clear();
    phoEt.clear();
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
    phoE1x3.clear();
    phoE2x2.clear();
    phoE3x3.clear();
    phoE2x5Max.clear();
    phoE1x5.clear();
    phoE2x5.clear();
    phoE5x5.clear();
    phoMaxEnergyXtal.clear();
    phoSigmaEtaEta.clear();
    phoR1x5.clear();
    phoR2x5.clear();
    phoESEffSigmaRR.clear();
    phoSigmaIEtaIEta_2012.clear();
    phoSigmaIEtaIPhi_2012.clear();
    phoSigmaIPhiIPhi_2012.clear();
    phoE1x3_2012.clear();
    phoE2x2_2012.clear();
    phoE3x3_2012.clear();
    phoE2x5Max_2012.clear();
    phoE5x5_2012.clear();
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

    pfsumIso1.clear();
    pfsumIso2.clear();
    pfsumIso3.clear();
    pfsumIso4.clear();
    pfsumIso5.clear();

    pfcVsIso1.clear();
    pfcVsIso2.clear();
    pfcVsIso3.clear();
    pfcVsIso4.clear();
    pfcVsIso5.clear();
    pfcVsIso1th1.clear();
    pfcVsIso2th1.clear();
    pfcVsIso3th1.clear();
    pfcVsIso4th1.clear();
    pfcVsIso5th1.clear();
    pfcVsIso1th2.clear();
    pfcVsIso2th2.clear();
    pfcVsIso3th2.clear();
    pfcVsIso4th2.clear();
    pfcVsIso5th2.clear();

    pfnVsIso1.clear();
    pfnVsIso2.clear();
    pfnVsIso3.clear();
    pfnVsIso4.clear();
    pfnVsIso5.clear();
    pfnVsIso1th1.clear();
    pfnVsIso2th1.clear();
    pfnVsIso3th1.clear();
    pfnVsIso4th1.clear();
    pfnVsIso5th1.clear();
    pfnVsIso1th2.clear();
    pfnVsIso2th2.clear();
    pfnVsIso3th2.clear();
    pfnVsIso4th2.clear();
    pfnVsIso5th2.clear();

    pfpVsIso1.clear();
    pfpVsIso2.clear();
    pfpVsIso3.clear();
    pfpVsIso4.clear();
    pfpVsIso5.clear();
    pfpVsIso1th1.clear();
    pfpVsIso2th1.clear();
    pfpVsIso3th1.clear();
    pfpVsIso4th1.clear();
    pfpVsIso5th1.clear();
    pfpVsIso1th2.clear();
    pfpVsIso2th2.clear();
    pfpVsIso3th2.clear();
    pfpVsIso4th2.clear();
    pfpVsIso5th2.clear();

    pfsumVsIso1.clear();
    pfsumVsIso2.clear();
    pfsumVsIso3.clear();
    pfsumVsIso4.clear();
    pfsumVsIso5.clear();
    pfsumVsIso1th1.clear();
    pfsumVsIso2th1.clear();
    pfsumVsIso3th1.clear();
    pfsumVsIso4th1.clear();
    pfsumVsIso5th1.clear();
    pfsumVsIso1th2.clear();
    pfsumVsIso2th2.clear();
    pfsumVsIso3th2.clear();
    pfsumVsIso4th2.clear();
    pfsumVsIso5th2.clear();

    pfVsSubIso1.clear();
    pfVsSubIso2.clear();
    pfVsSubIso3.clear();
    pfVsSubIso4.clear();
    pfVsSubIso5.clear();

    towerIso1.clear();
    towerIso2.clear();
    towerIso3.clear();
    towerIso4.clear();
    towerIso5.clear();
    towerVsIso1.clear();
    towerVsIso2.clear();
    towerVsIso3.clear();
    towerVsIso4.clear();
    towerVsIso5.clear();
    towerVsSubIso1.clear();
    towerVsSubIso2.clear();
    towerVsSubIso3.clear();
    towerVsSubIso4.clear();
    towerVsSubIso5.clear();
  };

  void setupTreeBranchesForWriting(TTree * tree_)
  {
    tree_->Branch("run",    &run, "run/i");
    tree_->Branch("event",  &event, "event/l");
    tree_->Branch("lumis",  &lumis, "lumi/i");
    tree_->Branch("HBHENoiseFilterResult", &HBHENoiseFilterResult);
    tree_->Branch("pcollisionEventSelection", &pcollisionEventSelection);

    tree_->Branch("nPho",                  &nPho);
    tree_->Branch("phoE",                  &phoE);
    tree_->Branch("phoEt",                 &phoEt);
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
    tree_->Branch("phoE1x3",               &phoE1x3);
    tree_->Branch("phoE2x2",               &phoE2x2);
    tree_->Branch("phoE3x3",               &phoE3x3);
    tree_->Branch("phoE2x5Max",            &phoE2x5Max);
    tree_->Branch("phoE1x5",               &phoE1x5);
    tree_->Branch("phoE2x5",               &phoE2x5);
    tree_->Branch("phoE5x5",               &phoE5x5);
    tree_->Branch("phoMaxEnergyXtal",      &phoMaxEnergyXtal);
    tree_->Branch("phoSigmaEtaEta",        &phoSigmaEtaEta);
    tree_->Branch("phoR1x5",               &phoR1x5);
    tree_->Branch("phoR2x5",               &phoR2x5);
    tree_->Branch("phoESEffSigmaRR",       &phoESEffSigmaRR);
    tree_->Branch("phoSigmaIEtaIEta_2012", &phoSigmaIEtaIEta_2012);
    tree_->Branch("phoSigmaIEtaIPhi_2012", &phoSigmaIEtaIPhi_2012);
    tree_->Branch("phoSigmaIPhiIPhi_2012", &phoSigmaIPhiIPhi_2012);
    tree_->Branch("phoE1x3_2012",          &phoE1x3_2012);
    tree_->Branch("phoE2x2_2012",          &phoE2x2_2012);
    tree_->Branch("phoE3x3_2012",          &phoE3x3_2012);
    tree_->Branch("phoE2x5Max_2012",       &phoE2x5Max_2012);
    tree_->Branch("phoE5x5_2012",          &phoE5x5_2012);
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

      // tree_->Branch("pfsumIso1",&pfsumIso1);
      // tree_->Branch("pfsumIso2",&pfsumIso2);
      // tree_->Branch("pfsumIso3",&pfsumIso3);
      // tree_->Branch("pfsumIso4",&pfsumIso4);
      // tree_->Branch("pfsumIso5",&pfsumIso5);

      //if(doVsIso_)
      {
	tree_->Branch("pfcVsIso1",&pfcVsIso1);
	tree_->Branch("pfcVsIso2",&pfcVsIso2);
	tree_->Branch("pfcVsIso3",&pfcVsIso3);
	tree_->Branch("pfcVsIso4",&pfcVsIso4);
	tree_->Branch("pfcVsIso5",&pfcVsIso5);
	tree_->Branch("pfcVsIso1th1",&pfcVsIso1th1);
	tree_->Branch("pfcVsIso2th1",&pfcVsIso2th1);
	tree_->Branch("pfcVsIso3th1",&pfcVsIso3th1);
	tree_->Branch("pfcVsIso4th1",&pfcVsIso4th1);
	tree_->Branch("pfcVsIso5th1",&pfcVsIso5th1);
	tree_->Branch("pfcVsIso1th2",&pfcVsIso1th2);
	tree_->Branch("pfcVsIso2th2",&pfcVsIso2th2);
	tree_->Branch("pfcVsIso3th2",&pfcVsIso3th2);
	tree_->Branch("pfcVsIso4th2",&pfcVsIso4th2);
	tree_->Branch("pfcVsIso5th2",&pfcVsIso5th2);

	tree_->Branch("pfnVsIso1",&pfnVsIso1);
	tree_->Branch("pfnVsIso2",&pfnVsIso2);
	tree_->Branch("pfnVsIso3",&pfnVsIso3);
	tree_->Branch("pfnVsIso4",&pfnVsIso4);
	tree_->Branch("pfnVsIso5",&pfnVsIso5);
	tree_->Branch("pfnVsIso1th1",&pfnVsIso1th1);
	tree_->Branch("pfnVsIso2th1",&pfnVsIso2th1);
	tree_->Branch("pfnVsIso3th1",&pfnVsIso3th1);
	tree_->Branch("pfnVsIso4th1",&pfnVsIso4th1);
	tree_->Branch("pfnVsIso5th1",&pfnVsIso5th1);
	tree_->Branch("pfnVsIso1th2",&pfnVsIso1th2);
	tree_->Branch("pfnVsIso2th2",&pfnVsIso2th2);
	tree_->Branch("pfnVsIso3th2",&pfnVsIso3th2);
	tree_->Branch("pfnVsIso4th2",&pfnVsIso4th2);
	tree_->Branch("pfnVsIso5th2",&pfnVsIso5th2);

	tree_->Branch("pfpVsIso1",&pfpVsIso1);
	tree_->Branch("pfpVsIso2",&pfpVsIso2);
	tree_->Branch("pfpVsIso3",&pfpVsIso3);
	tree_->Branch("pfpVsIso4",&pfpVsIso4);
	tree_->Branch("pfpVsIso5",&pfpVsIso5);
	tree_->Branch("pfpVsIso1th1",&pfpVsIso1th1);
	tree_->Branch("pfpVsIso2th1",&pfpVsIso2th1);
	tree_->Branch("pfpVsIso3th1",&pfpVsIso3th1);
	tree_->Branch("pfpVsIso4th1",&pfpVsIso4th1);
	tree_->Branch("pfpVsIso5th1",&pfpVsIso5th1);
	tree_->Branch("pfpVsIso1th2",&pfpVsIso1th2);
	tree_->Branch("pfpVsIso2th2",&pfpVsIso2th2);
	tree_->Branch("pfpVsIso3th2",&pfpVsIso3th2);
	tree_->Branch("pfpVsIso4th2",&pfpVsIso4th2);
	tree_->Branch("pfpVsIso5th2",&pfpVsIso5th2);

	// tree_->Branch("pfsumVsIso1",&pfsumVsIso1);
	// tree_->Branch("pfsumVsIso2",&pfsumVsIso2);
	// tree_->Branch("pfsumVsIso3",&pfsumVsIso3);
	// tree_->Branch("pfsumVsIso4",&pfsumVsIso4);
	// tree_->Branch("pfsumVsIso5",&pfsumVsIso5);
	// tree_->Branch("pfsumVsIso1th1",&pfsumVsIso1th1);
	// tree_->Branch("pfsumVsIso2th1",&pfsumVsIso2th1);
	// tree_->Branch("pfsumVsIso3th1",&pfsumVsIso3th1);
	// tree_->Branch("pfsumVsIso4th1",&pfsumVsIso4th1);
	// tree_->Branch("pfsumVsIso5th1",&pfsumVsIso5th1);
	// tree_->Branch("pfsumVsIso1th2",&pfsumVsIso1th2);
	// tree_->Branch("pfsumVsIso2th2",&pfsumVsIso2th2);
	// tree_->Branch("pfsumVsIso3th2",&pfsumVsIso3th2);
	// tree_->Branch("pfsumVsIso4th2",&pfsumVsIso4th2);
	// tree_->Branch("pfsumVsIso5th2",&pfsumVsIso5th2);


	// tree_->Branch("pfVsSubIso1",&pfVsSubIso1);
	// tree_->Branch("pfVsSubIso2",&pfVsSubIso2);
	// tree_->Branch("pfVsSubIso3",&pfVsSubIso3);
	// tree_->Branch("pfVsSubIso4",&pfVsSubIso4);
	// tree_->Branch("pfVsSubIso5",&pfVsSubIso5);


	tree_->Branch("towerIso1",&towerIso1);
	tree_->Branch("towerIso2",&towerIso2);
	tree_->Branch("towerIso3",&towerIso3);
	tree_->Branch("towerIso4",&towerIso4);
	tree_->Branch("towerIso5",&towerIso5);
	tree_->Branch("towerVsIso1",&towerVsIso1);
	tree_->Branch("towerVsIso2",&towerVsIso2);
	tree_->Branch("towerVsIso3",&towerVsIso3);
	tree_->Branch("towerVsIso4",&towerVsIso4);
	tree_->Branch("towerVsIso5",&towerVsIso5);
	tree_->Branch("towerVsSubIso1",&towerVsSubIso1);
	tree_->Branch("towerVsSubIso2",&towerVsSubIso2);
	tree_->Branch("towerVsSubIso3",&towerVsSubIso3);
	tree_->Branch("towerVsSubIso4",&towerVsSubIso4);
	tree_->Branch("towerVsSubIso5",&towerVsSubIso5);
      }
    }
  }
};
#endif
