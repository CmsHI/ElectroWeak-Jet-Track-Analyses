/*
 * class to manage dimuon tree, tree of muon pairs
 */

#ifndef TREEHEADERS_DIMUONTREE_H_
#define TREEHEADERS_DIMUONTREE_H_

#include <TTree.h>
#include <TLorentzVector.h>

#include <vector>

#include "ggHiNtuplizerTree.h"

class dimuon {
public :
    dimuon(){
        mass = 0.105658;

        muPt_1 = 0;
        muEta_1 = 0;
        muPhi_1 = 0;
        muCharge_1 = 0;
        muType_1 = 0;
        muIsGood_1 = 0;
        muD0_1 = 0;
        muDz_1 = 0;
        muChi2NDF_1 = 0;
        muInnerD0_1 = 0;
        muInnerDz_1 = 0;
        muTrkLayers_1 = 0;
        muPixelLayers_1 = 0;
        muPixelHits_1 = 0;
        muMuonHits_1 = 0;
        muTrkQuality_1 = 0;
        muStations_1 = 0;
        muIsoTrk_1 = 0;
        muPFChIso_1 = 0;
        muPFPhoIso_1 = 0;
        muPFNeuIso_1 = 0;
        muPFPUIso_1 = 0;

        muPt_2 = 0;
        muEta_2 = 0;
        muPhi_2 = 0;
        muCharge_2 = 0;
        muType_2 = 0;
        muIsGood_2 = 0;
        muD0_2 = 0;
        muDz_2 = 0;
        muChi2NDF_2 = 0;
        muInnerD0_2 = 0;
        muInnerDz_2 = 0;
        muTrkLayers_2 = 0;
        muPixelLayers_2 = 0;
        muPixelHits_2 = 0;
        muMuonHits_2 = 0;
        muTrkQuality_2 = 0;
        muStations_2 = 0;
        muIsoTrk_2 = 0;
        muPFChIso_2 = 0;
        muPFPhoIso_2 = 0;
        muPFNeuIso_2 = 0;
        muPFPUIso_2 = 0;

        diMuM = 0;
        diMuEn = 0;
        diMuPt = 0;
        diMuEta = 0;
        diMuPhi = 0;
    };
    ~dimuon(){};
    void setupDiMuonTree(TTree *t);
    void branchDiMuonTree(TTree *t);
    void makeDiMuonPairs(ggHiNtuplizer &tggHiNtuplizer, bool doSizeCheck = true);

    double mass;
    // Declaration of leaf types
    Int_t           nMu;
    std::vector<float>   *muPt_1;
    std::vector<float>   *muEta_1;
    std::vector<float>   *muPhi_1;
    std::vector<int>     *muCharge_1;
    std::vector<int>     *muType_1;
    std::vector<int>     *muIsGood_1;
    std::vector<float>   *muD0_1;
    std::vector<float>   *muDz_1;
    std::vector<float>   *muChi2NDF_1;
    std::vector<float>   *muInnerD0_1;
    std::vector<float>   *muInnerDz_1;
    std::vector<int>     *muTrkLayers_1;
    std::vector<int>     *muPixelLayers_1;
    std::vector<int>     *muPixelHits_1;
    std::vector<int>     *muMuonHits_1;
    std::vector<int>     *muTrkQuality_1;
    std::vector<int>     *muStations_1;
    std::vector<float>   *muIsoTrk_1;
    std::vector<float>   *muPFChIso_1;
    std::vector<float>   *muPFPhoIso_1;
    std::vector<float>   *muPFNeuIso_1;
    std::vector<float>   *muPFPUIso_1;

    std::vector<float>   *muPt_2;
    std::vector<float>   *muEta_2;
    std::vector<float>   *muPhi_2;
    std::vector<int>     *muCharge_2;
    std::vector<int>     *muType_2;
    std::vector<int>     *muIsGood_2;
    std::vector<float>   *muD0_2;
    std::vector<float>   *muDz_2;
    std::vector<float>   *muChi2NDF_2;
    std::vector<float>   *muInnerD0_2;
    std::vector<float>   *muInnerDz_2;
    std::vector<int>     *muTrkLayers_2;
    std::vector<int>     *muPixelLayers_2;
    std::vector<int>     *muPixelHits_2;
    std::vector<int>     *muMuonHits_2;
    std::vector<int>     *muTrkQuality_2;
    std::vector<int>     *muStations_2;
    std::vector<float>   *muIsoTrk_2;
    std::vector<float>   *muPFChIso_2;
    std::vector<float>   *muPFPhoIso_2;
    std::vector<float>   *muPFNeuIso_2;
    std::vector<float>   *muPFPUIso_2;

    std::vector<float>   *diMuM;
    std::vector<float>   *diMuEn;
    std::vector<float>   *diMuPt;
    std::vector<float>   *diMuEta;
    std::vector<float>   *diMuPhi;

    // List of branches
    TBranch        *b_nMu;
    TBranch        *b_muPt_1;
    TBranch        *b_muEta_1;
    TBranch        *b_muPhi_1;
    TBranch        *b_muCharge_1;
    TBranch        *b_muType_1;
    TBranch        *b_muIsGood_1;
    TBranch        *b_muD0_1;
    TBranch        *b_muDz_1;
    TBranch        *b_muChi2NDF_1;
    TBranch        *b_muInnerD0_1;
    TBranch        *b_muInnerDz_1;
    TBranch        *b_muTrkLayers_1;
    TBranch        *b_muPixelLayers_1;
    TBranch        *b_muPixelHits_1;
    TBranch        *b_muMuonHits_1;
    TBranch        *b_muTrkQuality_1;
    TBranch        *b_muStations_1;
    TBranch        *b_muIsoTrk_1;
    TBranch        *b_muPFChIso_1;
    TBranch        *b_muPFPhoIso_1;
    TBranch        *b_muPFNeuIso_1;
    TBranch        *b_muPFPUIso_1;

    TBranch        *b_muPt_2;
    TBranch        *b_muEta_2;
    TBranch        *b_muPhi_2;
    TBranch        *b_muCharge_2;
    TBranch        *b_muType_2;
    TBranch        *b_muIsGood_2;
    TBranch        *b_muD0_2;
    TBranch        *b_muDz_2;
    TBranch        *b_muChi2NDF_2;
    TBranch        *b_muInnerD0_2;
    TBranch        *b_muInnerDz_2;
    TBranch        *b_muTrkLayers_2;
    TBranch        *b_muPixelLayers_2;
    TBranch        *b_muPixelHits_2;
    TBranch        *b_muMuonHits_2;
    TBranch        *b_muTrkQuality_2;
    TBranch        *b_muStations_2;
    TBranch        *b_muIsoTrk_2;
    TBranch        *b_muPFChIso_2;
    TBranch        *b_muPFPhoIso_2;
    TBranch        *b_muPFNeuIso_2;
    TBranch        *b_muPFPUIso_2;

    TBranch        *b_diMuM;
    TBranch        *b_diMuEn;
    TBranch        *b_diMuPt;
    TBranch        *b_diMuEta;
    TBranch        *b_diMuPhi;

    // list of objects to be used when creating a diMuon Tree
    Int_t           nMu_out;
    std::vector<float>   muPt_1_out;
    std::vector<float>   muEta_1_out;
    std::vector<float>   muPhi_1_out;
    std::vector<int>     muCharge_1_out;
    std::vector<int>     muType_1_out;
    std::vector<int>     muIsGood_1_out;
    std::vector<float>   muD0_1_out;
    std::vector<float>   muDz_1_out;
    std::vector<float>   muChi2NDF_1_out;
    std::vector<float>   muInnerD0_1_out;
    std::vector<float>   muInnerDz_1_out;
    std::vector<int>     muTrkLayers_1_out;
    std::vector<int>     muPixelLayers_1_out;
    std::vector<int>     muPixelHits_1_out;
    std::vector<int>     muMuonHits_1_out;
    std::vector<int>     muTrkQuality_1_out;
    std::vector<int>     muStations_1_out;
    std::vector<float>   muIsoTrk_1_out;
    std::vector<float>   muPFChIso_1_out;
    std::vector<float>   muPFPhoIso_1_out;
    std::vector<float>   muPFNeuIso_1_out;
    std::vector<float>   muPFPUIso_1_out;

    std::vector<float>   muPt_2_out;
    std::vector<float>   muEta_2_out;
    std::vector<float>   muPhi_2_out;
    std::vector<int>     muCharge_2_out;
    std::vector<int>     muType_2_out;
    std::vector<int>     muIsGood_2_out;
    std::vector<float>   muD0_2_out;
    std::vector<float>   muDz_2_out;
    std::vector<float>   muChi2NDF_2_out;
    std::vector<float>   muInnerD0_2_out;
    std::vector<float>   muInnerDz_2_out;
    std::vector<int>     muTrkLayers_2_out;
    std::vector<int>     muPixelLayers_2_out;
    std::vector<int>     muPixelHits_2_out;
    std::vector<int>     muMuonHits_2_out;
    std::vector<int>     muTrkQuality_2_out;
    std::vector<int>     muStations_2_out;
    std::vector<float>   muIsoTrk_2_out;
    std::vector<float>   muPFChIso_2_out;
    std::vector<float>   muPFPhoIso_2_out;
    std::vector<float>   muPFNeuIso_2_out;
    std::vector<float>   muPFPUIso_2_out;

    std::vector<float>   diMuM_out;
    std::vector<float>   diMuEn_out;
    std::vector<float>   diMuPt_out;
    std::vector<float>   diMuEta_out;
    std::vector<float>   diMuPhi_out;
};

void dimuon::setupDiMuonTree(TTree *t)
{
    // Set branch addresses and branch pointers
    if (t->GetBranch("nMu"))  t->SetBranchAddress("nMu", &nMu, &b_nMu);

    if (t->GetBranch("muPt_1"))  t->SetBranchAddress("muPt_1", &muPt_1, &b_muPt_1);
    if (t->GetBranch("muEta_1"))  t->SetBranchAddress("muEta_1", &muEta_1, &b_muEta_1);
    if (t->GetBranch("muPhi_1"))  t->SetBranchAddress("muPhi_1", &muPhi_1, &b_muPhi_1);
    if (t->GetBranch("muCharge_1"))  t->SetBranchAddress("muCharge_1", &muCharge_1, &b_muCharge_1);
    if (t->GetBranch("muType_1"))  t->SetBranchAddress("muType_1", &muType_1, &b_muType_1);
    if (t->GetBranch("muIsGood_1"))  t->SetBranchAddress("muIsGood_1", &muIsGood_1, &b_muIsGood_1);
    if (t->GetBranch("muD0_1"))  t->SetBranchAddress("muD0_1", &muD0_1, &b_muD0_1);
    if (t->GetBranch("muDz_1"))  t->SetBranchAddress("muDz_1", &muDz_1, &b_muDz_1);
    if (t->GetBranch("muChi2NDF_1"))  t->SetBranchAddress("muChi2NDF_1", &muChi2NDF_1, &b_muChi2NDF_1);
    if (t->GetBranch("muInnerD0_1"))  t->SetBranchAddress("muInnerD0_1", &muInnerD0_1, &b_muInnerD0_1);
    if (t->GetBranch("muInnerDz_1"))  t->SetBranchAddress("muInnerDz_1", &muInnerDz_1, &b_muInnerDz_1);
    if (t->GetBranch("muTrkLayers_1"))  t->SetBranchAddress("muTrkLayers_1", &muTrkLayers_1, &b_muTrkLayers_1);
    if (t->GetBranch("muPixelLayers_1"))  t->SetBranchAddress("muPixelLayers_1", &muPixelLayers_1, &b_muPixelLayers_1);
    if (t->GetBranch("muPixelHits_1"))  t->SetBranchAddress("muPixelHits_1", &muPixelHits_1, &b_muPixelHits_1);
    if (t->GetBranch("muMuonHits_1"))  t->SetBranchAddress("muMuonHits_1", &muMuonHits_1, &b_muMuonHits_1);
    if (t->GetBranch("muTrkQuality_1"))  t->SetBranchAddress("muTrkQuality_1", &muTrkQuality_1, &b_muTrkQuality_1);
    if (t->GetBranch("muStations_1"))  t->SetBranchAddress("muStations_1", &muStations_1, &b_muStations_1);
    if (t->GetBranch("muIsoTrk_1"))  t->SetBranchAddress("muIsoTrk_1", &muIsoTrk_1, &b_muIsoTrk_1);
    if (t->GetBranch("muPFChIso_1"))  t->SetBranchAddress("muPFChIso_1", &muPFChIso_1, &b_muPFChIso_1);
    if (t->GetBranch("muPFPhoIso_1"))  t->SetBranchAddress("muPFPhoIso_1", &muPFPhoIso_1, &b_muPFPhoIso_1);
    if (t->GetBranch("muPFNeuIso_1"))  t->SetBranchAddress("muPFNeuIso_1", &muPFNeuIso_1, &b_muPFNeuIso_1);
    if (t->GetBranch("muPFPUIso_1"))  t->SetBranchAddress("muPFPUIso_1", &muPFPUIso_1, &b_muPFPUIso_1);

    if (t->GetBranch("muPt_2"))  t->SetBranchAddress("muPt_2", &muPt_2, &b_muPt_2);
    if (t->GetBranch("muEta_2"))  t->SetBranchAddress("muEta_2", &muEta_2, &b_muEta_2);
    if (t->GetBranch("muPhi_2"))  t->SetBranchAddress("muPhi_2", &muPhi_2, &b_muPhi_2);
    if (t->GetBranch("muCharge_2"))  t->SetBranchAddress("muCharge_2", &muCharge_2, &b_muCharge_2);
    if (t->GetBranch("muType_2"))  t->SetBranchAddress("muType_2", &muType_2, &b_muType_2);
    if (t->GetBranch("muIsGood_2"))  t->SetBranchAddress("muIsGood_2", &muIsGood_2, &b_muIsGood_2);
    if (t->GetBranch("muD0_2"))  t->SetBranchAddress("muD0_2", &muD0_2, &b_muD0_2);
    if (t->GetBranch("muDz_2"))  t->SetBranchAddress("muDz_2", &muDz_2, &b_muDz_2);
    if (t->GetBranch("muChi2NDF_2"))  t->SetBranchAddress("muChi2NDF_2", &muChi2NDF_2, &b_muChi2NDF_2);
    if (t->GetBranch("muInnerD0_2"))  t->SetBranchAddress("muInnerD0_2", &muInnerD0_2, &b_muInnerD0_2);
    if (t->GetBranch("muInnerDz_2"))  t->SetBranchAddress("muInnerDz_2", &muInnerDz_2, &b_muInnerDz_2);
    if (t->GetBranch("muTrkLayers_2"))  t->SetBranchAddress("muTrkLayers_2", &muTrkLayers_2, &b_muTrkLayers_2);
    if (t->GetBranch("muPixelLayers_2"))  t->SetBranchAddress("muPixelLayers_2", &muPixelLayers_2, &b_muPixelLayers_2);
    if (t->GetBranch("muPixelHits_2"))  t->SetBranchAddress("muPixelHits_2", &muPixelHits_2, &b_muPixelHits_2);
    if (t->GetBranch("muMuonHits_2"))  t->SetBranchAddress("muMuonHits_2", &muMuonHits_2, &b_muMuonHits_2);
    if (t->GetBranch("muTrkQuality_2"))  t->SetBranchAddress("muTrkQuality_2", &muTrkQuality_2, &b_muTrkQuality_2);
    if (t->GetBranch("muStations_2"))  t->SetBranchAddress("muStations_2", &muStations_2, &b_muStations_2);
    if (t->GetBranch("muIsoTrk_2"))  t->SetBranchAddress("muIsoTrk_2", &muIsoTrk_2, &b_muIsoTrk_2);
    if (t->GetBranch("muPFChIso_2"))  t->SetBranchAddress("muPFChIso_2", &muPFChIso_2, &b_muPFChIso_2);
    if (t->GetBranch("muPFPhoIso_2"))  t->SetBranchAddress("muPFPhoIso_2", &muPFPhoIso_2, &b_muPFPhoIso_2);
    if (t->GetBranch("muPFNeuIso_2"))  t->SetBranchAddress("muPFNeuIso_2", &muPFNeuIso_2, &b_muPFNeuIso_2);
    if (t->GetBranch("muPFPUIso_2"))  t->SetBranchAddress("muPFPUIso_2", &muPFPUIso_2, &b_muPFPUIso_2);

    if (t->GetBranch("diMuM"))  t->SetBranchAddress("diMuM", &diMuM, &b_diMuM);
    if (t->GetBranch("diMuEn"))  t->SetBranchAddress("diMuEn", &diMuEn, &b_diMuEn);
    if (t->GetBranch("diMuPt"))  t->SetBranchAddress("diMuPt", &diMuPt, &b_diMuPt);
    if (t->GetBranch("diMuEta"))  t->SetBranchAddress("diMuEta", &diMuEta, &b_diMuEta);
    if (t->GetBranch("diMuPhi"))  t->SetBranchAddress("diMuPhi", &diMuPhi, &b_diMuPhi);
}

void dimuon::branchDiMuonTree(TTree* t)
{
    t->Branch("nMu", &nMu_out);

    t->Branch("muPt_1", &muPt_1_out);
    t->Branch("muEta_1", &muEta_1_out);
    t->Branch("muPhi_1", &muPhi_1_out);
    t->Branch("muCharge_1", &muCharge_1_out);
    t->Branch("muType_1", &muType_1_out);
    t->Branch("muIsGood_1", &muIsGood_1_out);
    t->Branch("muD0_1", &muD0_1_out);
    t->Branch("muDz_1", &muDz_1_out);
    t->Branch("muChi2NDF_1", &muChi2NDF_1_out);
    t->Branch("muInnerD0_1", &muInnerD0_1_out);
    t->Branch("muInnerDz_1", &muInnerDz_1_out);
    t->Branch("muTrkLayers_1", &muTrkLayers_1_out);
    t->Branch("muPixelLayers_1", &muPixelLayers_1_out);
    t->Branch("muPixelHits_1", &muPixelHits_1_out);
    t->Branch("muMuonHits_1", &muMuonHits_1_out);
    t->Branch("muTrkQuality_1", &muTrkQuality_1_out);
    t->Branch("muStations_1", &muStations_1_out);
    t->Branch("muIsoTrk_1", &muIsoTrk_1_out);
    t->Branch("muPFChIso_1", &muPFChIso_1_out);
    t->Branch("muPFPhoIso_1", &muPFPhoIso_1_out);
    t->Branch("muPFNeuIso_1", &muPFNeuIso_1_out);
    t->Branch("muPFPUIso_1", &muPFPUIso_1_out);

    t->Branch("muPt_2", &muPt_2_out);
    t->Branch("muEta_2", &muEta_2_out);
    t->Branch("muPhi_2", &muPhi_2_out);
    t->Branch("muCharge_2", &muCharge_2_out);
    t->Branch("muType_2", &muType_2_out);
    t->Branch("muIsGood_2", &muIsGood_2_out);
    t->Branch("muD0_2", &muD0_2_out);
    t->Branch("muDz_2", &muDz_2_out);
    t->Branch("muChi2NDF_2", &muChi2NDF_2_out);
    t->Branch("muInnerD0_2", &muInnerD0_2_out);
    t->Branch("muInnerDz_2", &muInnerDz_2_out);
    t->Branch("muTrkLayers_2", &muTrkLayers_2_out);
    t->Branch("muPixelLayers_2", &muPixelLayers_2_out);
    t->Branch("muPixelHits_2", &muPixelHits_2_out);
    t->Branch("muMuonHits_2", &muMuonHits_2_out);
    t->Branch("muTrkQuality_2", &muTrkQuality_2_out);
    t->Branch("muStations_2", &muStations_2_out);
    t->Branch("muIsoTrk_2", &muIsoTrk_2_out);
    t->Branch("muPFChIso_2", &muPFChIso_2_out);
    t->Branch("muPFPhoIso_2", &muPFPhoIso_2_out);
    t->Branch("muPFNeuIso_2", &muPFNeuIso_2_out);
    t->Branch("muPFPUIso_2", &muPFPUIso_2_out);

    t->Branch("diMuM", &diMuM_out);
    t->Branch("diMuEn", &diMuEn_out);
    t->Branch("diMuPt", &diMuPt_out);
    t->Branch("diMuEta", &diMuEta_out);
    t->Branch("diMuPhi", &diMuPhi_out);
}

void dimuon::makeDiMuonPairs(ggHiNtuplizer &tggHiNtuplizer, bool doSizeCheck)
{
    muPt_1_out.clear();
    muEta_1_out.clear();
    muPhi_1_out.clear();
    muCharge_1_out.clear();
    muType_1_out.clear();
    muIsGood_1_out.clear();
    muD0_1_out.clear();
    muDz_1_out.clear();
    muChi2NDF_1_out.clear();
    muInnerD0_1_out.clear();
    muInnerDz_1_out.clear();
    muTrkLayers_1_out.clear();
    muPixelLayers_1_out.clear();
    muPixelHits_1_out.clear();
    muMuonHits_1_out.clear();
    muTrkQuality_1_out.clear();
    muStations_1_out.clear();
    muIsoTrk_1_out.clear();
    muPFChIso_1_out.clear();
    muPFPhoIso_1_out.clear();
    muPFNeuIso_1_out.clear();
    muPFPUIso_1_out.clear();

    muPt_2_out.clear();
    muEta_2_out.clear();
    muPhi_2_out.clear();
    muCharge_2_out.clear();
    muType_2_out.clear();
    muIsGood_2_out.clear();
    muD0_2_out.clear();
    muDz_2_out.clear();
    muChi2NDF_2_out.clear();
    muInnerD0_2_out.clear();
    muInnerDz_2_out.clear();
    muTrkLayers_2_out.clear();
    muPixelLayers_2_out.clear();
    muPixelHits_2_out.clear();
    muMuonHits_2_out.clear();
    muTrkQuality_2_out.clear();
    muStations_2_out.clear();
    muIsoTrk_2_out.clear();
    muPFChIso_2_out.clear();
    muPFPhoIso_2_out.clear();
    muPFNeuIso_2_out.clear();
    muPFPUIso_2_out.clear();

    diMuM_out.clear();
    diMuEn_out.clear();
    diMuPt_out.clear();
    diMuEta_out.clear();
    diMuPhi_out.clear();

    nMu_out = tggHiNtuplizer.nMu;
    for(int i=0; i<nMu_out; ++i)
    {
        for(int j=i+1; j<nMu_out; ++j)
        {
            if (doSizeCheck) {

                if(tggHiNtuplizer.muPt->size() == (unsigned)nMu_out)  muPt_1_out.push_back(tggHiNtuplizer.muPt->at(i));
                if(tggHiNtuplizer.muEta->size() == (unsigned)nMu_out)  muEta_1_out.push_back(tggHiNtuplizer.muEta->at(i));
                if(tggHiNtuplizer.muPhi->size() == (unsigned)nMu_out)  muPhi_1_out.push_back(tggHiNtuplizer.muPhi->at(i));
                if(tggHiNtuplizer.muCharge->size() == (unsigned)nMu_out)  muCharge_1_out.push_back(tggHiNtuplizer.muCharge->at(i));
                if(tggHiNtuplizer.muType->size() == (unsigned)nMu_out)  muType_1_out.push_back(tggHiNtuplizer.muType->at(i));
                if(tggHiNtuplizer.muIsGood->size() == (unsigned)nMu_out)  muIsGood_1_out.push_back(tggHiNtuplizer.muIsGood->at(i));
                if(tggHiNtuplizer.muD0->size() == (unsigned)nMu_out)  muD0_1_out.push_back(tggHiNtuplizer.muD0->at(i));
                if(tggHiNtuplizer.muDz->size() == (unsigned)nMu_out)  muDz_1_out.push_back(tggHiNtuplizer.muDz->at(i));
                if(tggHiNtuplizer.muChi2NDF->size() == (unsigned)nMu_out)  muChi2NDF_1_out.push_back(tggHiNtuplizer.muChi2NDF->at(i));
                if(tggHiNtuplizer.muInnerD0->size() == (unsigned)nMu_out)  muInnerD0_1_out.push_back(tggHiNtuplizer.muInnerD0->at(i));
                if(tggHiNtuplizer.muInnerDz->size() == (unsigned)nMu_out)  muInnerDz_1_out.push_back(tggHiNtuplizer.muInnerDz->at(i));
                if(tggHiNtuplizer.muTrkLayers->size() == (unsigned)nMu_out)  muTrkLayers_1_out.push_back(tggHiNtuplizer.muTrkLayers->at(i));
                if(tggHiNtuplizer.muPixelLayers->size() == (unsigned)nMu_out)  muPixelLayers_1_out.push_back(tggHiNtuplizer.muPixelLayers->at(i));
                if(tggHiNtuplizer.muPixelHits->size() == (unsigned)nMu_out)  muPixelHits_1_out.push_back(tggHiNtuplizer.muPixelHits->at(i));
                if(tggHiNtuplizer.muMuonHits->size() == (unsigned)nMu_out)  muMuonHits_1_out.push_back(tggHiNtuplizer.muMuonHits->at(i));
                if(tggHiNtuplizer.muTrkQuality->size() == (unsigned)nMu_out)  muTrkQuality_1_out.push_back(tggHiNtuplizer.muTrkQuality->at(i));
                if(tggHiNtuplizer.muStations->size() == (unsigned)nMu_out)  muStations_1_out.push_back(tggHiNtuplizer.muStations->at(i));
                if(tggHiNtuplizer.muIsoTrk->size() == (unsigned)nMu_out)  muIsoTrk_1_out.push_back(tggHiNtuplizer.muIsoTrk->at(i));
                if(tggHiNtuplizer.muPFChIso->size() == (unsigned)nMu_out)  muPFChIso_1_out.push_back(tggHiNtuplizer.muPFChIso->at(i));
                if(tggHiNtuplizer.muPFPhoIso->size() == (unsigned)nMu_out)  muPFPhoIso_1_out.push_back(tggHiNtuplizer.muPFPhoIso->at(i));
                if(tggHiNtuplizer.muPFNeuIso->size() == (unsigned)nMu_out)  muPFNeuIso_1_out.push_back(tggHiNtuplizer.muPFNeuIso->at(i));
                if(tggHiNtuplizer.muPFPUIso->size() == (unsigned)nMu_out)  muPFPUIso_1_out.push_back(tggHiNtuplizer.muPFPUIso->at(i));

                if(tggHiNtuplizer.muPt->size() == (unsigned)nMu_out)  muPt_2_out.push_back(tggHiNtuplizer.muPt->at(j));
                if(tggHiNtuplizer.muEta->size() == (unsigned)nMu_out)  muEta_2_out.push_back(tggHiNtuplizer.muEta->at(j));
                if(tggHiNtuplizer.muPhi->size() == (unsigned)nMu_out)  muPhi_2_out.push_back(tggHiNtuplizer.muPhi->at(j));
                if(tggHiNtuplizer.muCharge->size() == (unsigned)nMu_out)  muCharge_2_out.push_back(tggHiNtuplizer.muCharge->at(j));
                if(tggHiNtuplizer.muType->size() == (unsigned)nMu_out)  muType_2_out.push_back(tggHiNtuplizer.muType->at(j));
                if(tggHiNtuplizer.muIsGood->size() == (unsigned)nMu_out)  muIsGood_2_out.push_back(tggHiNtuplizer.muIsGood->at(j));
                if(tggHiNtuplizer.muD0->size() == (unsigned)nMu_out)  muD0_2_out.push_back(tggHiNtuplizer.muD0->at(j));
                if(tggHiNtuplizer.muDz->size() == (unsigned)nMu_out)  muDz_2_out.push_back(tggHiNtuplizer.muDz->at(j));
                if(tggHiNtuplizer.muChi2NDF->size() == (unsigned)nMu_out)  muChi2NDF_2_out.push_back(tggHiNtuplizer.muChi2NDF->at(j));
                if(tggHiNtuplizer.muInnerD0->size() == (unsigned)nMu_out)  muInnerD0_2_out.push_back(tggHiNtuplizer.muInnerD0->at(j));
                if(tggHiNtuplizer.muInnerDz->size() == (unsigned)nMu_out)  muInnerDz_2_out.push_back(tggHiNtuplizer.muInnerDz->at(j));
                if(tggHiNtuplizer.muTrkLayers->size() == (unsigned)nMu_out)  muTrkLayers_2_out.push_back(tggHiNtuplizer.muTrkLayers->at(j));
                if(tggHiNtuplizer.muPixelLayers->size() == (unsigned)nMu_out)  muPixelLayers_2_out.push_back(tggHiNtuplizer.muPixelLayers->at(j));
                if(tggHiNtuplizer.muPixelHits->size() == (unsigned)nMu_out)  muPixelHits_2_out.push_back(tggHiNtuplizer.muPixelHits->at(j));
                if(tggHiNtuplizer.muMuonHits->size() == (unsigned)nMu_out)  muMuonHits_2_out.push_back(tggHiNtuplizer.muMuonHits->at(j));
                if(tggHiNtuplizer.muTrkQuality->size() == (unsigned)nMu_out)  muTrkQuality_2_out.push_back(tggHiNtuplizer.muTrkQuality->at(j));
                if(tggHiNtuplizer.muStations->size() == (unsigned)nMu_out)  muStations_2_out.push_back(tggHiNtuplizer.muStations->at(j));
                if(tggHiNtuplizer.muIsoTrk->size() == (unsigned)nMu_out)  muIsoTrk_2_out.push_back(tggHiNtuplizer.muIsoTrk->at(j));
                if(tggHiNtuplizer.muPFChIso->size() == (unsigned)nMu_out)  muPFChIso_2_out.push_back(tggHiNtuplizer.muPFChIso->at(j));
                if(tggHiNtuplizer.muPFPhoIso->size() == (unsigned)nMu_out)  muPFPhoIso_2_out.push_back(tggHiNtuplizer.muPFPhoIso->at(j));
                if(tggHiNtuplizer.muPFNeuIso->size() == (unsigned)nMu_out)  muPFNeuIso_2_out.push_back(tggHiNtuplizer.muPFNeuIso->at(j));
                if(tggHiNtuplizer.muPFPUIso->size() == (unsigned)nMu_out)  muPFPUIso_2_out.push_back(tggHiNtuplizer.muPFPUIso->at(j));

                // dimuon
                if (tggHiNtuplizer.muPt->size()  == (unsigned)nMu_out &&
                    tggHiNtuplizer.muEta->size() == (unsigned)nMu_out &&
                    tggHiNtuplizer.muPhi->size() == (unsigned)nMu_out)
                {
                    TLorentzVector v1, v2, vSum;
                    v1.SetPtEtaPhiM( tggHiNtuplizer.muPt->at(i), tggHiNtuplizer.muEta->at(i),
                            tggHiNtuplizer.muPhi->at(i), mass);
                    v2.SetPtEtaPhiM( tggHiNtuplizer.muPt->at(j), tggHiNtuplizer.muEta->at(j),
                            tggHiNtuplizer.muPhi->at(j), mass);
                    vSum = v1+v2;

                    diMuM_out.push_back(vSum.M());
                    diMuEn_out.push_back(vSum.Energy());
                    diMuPt_out.push_back(vSum.Pt());
                    diMuEta_out.push_back(vSum.Eta());
                    diMuPhi_out.push_back(vSum.Phi());
                }
            }
            else {
                muPt_1_out.push_back(tggHiNtuplizer.muPt->at(i));
                muEta_1_out.push_back(tggHiNtuplizer.muEta->at(i));
                muPhi_1_out.push_back(tggHiNtuplizer.muPhi->at(i));
                muCharge_1_out.push_back(tggHiNtuplizer.muCharge->at(i));
                muType_1_out.push_back(tggHiNtuplizer.muType->at(i));
                muIsGood_1_out.push_back(tggHiNtuplizer.muIsGood->at(i));
                muD0_1_out.push_back(tggHiNtuplizer.muD0->at(i));
                muDz_1_out.push_back(tggHiNtuplizer.muDz->at(i));
                muChi2NDF_1_out.push_back(tggHiNtuplizer.muChi2NDF->at(i));
                muInnerD0_1_out.push_back(tggHiNtuplizer.muInnerD0->at(i));
                muInnerDz_1_out.push_back(tggHiNtuplizer.muInnerDz->at(i));
                muTrkLayers_1_out.push_back(tggHiNtuplizer.muTrkLayers->at(i));
                muPixelLayers_1_out.push_back(tggHiNtuplizer.muPixelLayers->at(i));
                muPixelHits_1_out.push_back(tggHiNtuplizer.muPixelHits->at(i));
                muMuonHits_1_out.push_back(tggHiNtuplizer.muMuonHits->at(i));
                muTrkQuality_1_out.push_back(tggHiNtuplizer.muTrkQuality->at(i));
                muStations_1_out.push_back(tggHiNtuplizer.muStations->at(i));
                muIsoTrk_1_out.push_back(tggHiNtuplizer.muIsoTrk->at(i));
                muPFChIso_1_out.push_back(tggHiNtuplizer.muPFChIso->at(i));
                muPFPhoIso_1_out.push_back(tggHiNtuplizer.muPFPhoIso->at(i));
                muPFNeuIso_1_out.push_back(tggHiNtuplizer.muPFNeuIso->at(i));
                muPFPUIso_1_out.push_back(tggHiNtuplizer.muPFPUIso->at(i));

                muPt_2_out.push_back(tggHiNtuplizer.muPt->at(j));
                muEta_2_out.push_back(tggHiNtuplizer.muEta->at(j));
                muPhi_2_out.push_back(tggHiNtuplizer.muPhi->at(j));
                muCharge_2_out.push_back(tggHiNtuplizer.muCharge->at(j));
                muType_2_out.push_back(tggHiNtuplizer.muType->at(j));
                muIsGood_2_out.push_back(tggHiNtuplizer.muIsGood->at(j));
                muD0_2_out.push_back(tggHiNtuplizer.muD0->at(j));
                muDz_2_out.push_back(tggHiNtuplizer.muDz->at(j));
                muChi2NDF_2_out.push_back(tggHiNtuplizer.muChi2NDF->at(j));
                muInnerD0_2_out.push_back(tggHiNtuplizer.muInnerD0->at(j));
                muInnerDz_2_out.push_back(tggHiNtuplizer.muInnerDz->at(j));
                muTrkLayers_2_out.push_back(tggHiNtuplizer.muTrkLayers->at(j));
                muPixelLayers_2_out.push_back(tggHiNtuplizer.muPixelLayers->at(j));
                muPixelHits_2_out.push_back(tggHiNtuplizer.muPixelHits->at(j));
                muMuonHits_2_out.push_back(tggHiNtuplizer.muMuonHits->at(j));
                muTrkQuality_2_out.push_back(tggHiNtuplizer.muTrkQuality->at(j));
                muStations_2_out.push_back(tggHiNtuplizer.muStations->at(j));
                muIsoTrk_2_out.push_back(tggHiNtuplizer.muIsoTrk->at(j));
                muPFChIso_2_out.push_back(tggHiNtuplizer.muPFChIso->at(j));
                muPFPhoIso_2_out.push_back(tggHiNtuplizer.muPFPhoIso->at(j));
                muPFNeuIso_2_out.push_back(tggHiNtuplizer.muPFNeuIso->at(j));
                muPFPUIso_2_out.push_back(tggHiNtuplizer.muPFPUIso->at(j));

                TLorentzVector v1, v2, vSum;
                v1.SetPtEtaPhiM( tggHiNtuplizer.muPt->at(i), tggHiNtuplizer.muEta->at(i),
                        tggHiNtuplizer.muPhi->at(i), mass);
                v2.SetPtEtaPhiM( tggHiNtuplizer.muPt->at(j), tggHiNtuplizer.muEta->at(j),
                        tggHiNtuplizer.muPhi->at(j), mass);
                vSum = v1+v2;

                diMuM_out.push_back(vSum.M());
                diMuEn_out.push_back(vSum.Energy());
                diMuPt_out.push_back(vSum.Pt());
                diMuEta_out.push_back(vSum.Eta());
                diMuPhi_out.push_back(vSum.Phi());
            }
        }
    }
}

#endif /* TREEHEADERS_DIMUONTREE_H_ */
