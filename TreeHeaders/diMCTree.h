/*
 * diMCTree.h
 *
 *      Author: Kaya Tatar
 *
 *       class to manage diMC particle tree, tree of MC particle pairs
 */

#ifndef TREEHEADERS_DIMCTREE_H_
#define TREEHEADERS_DIMCTREE_H_

#include <TTree.h>
#include <TLorentzVector.h>

#include <vector>

#include "ggHiNtuplizerTree.h"

class diMC {
public :
    diMC(){

        mcPID_1 = 0;
        mcStatus_1 = 0;
        mcVtx_x_1 = 0;
        mcVtx_y_1 = 0;
        mcVtx_z_1 = 0;
        mcPt_1 = 0;
        mcEta_1 = 0;
        mcPhi_1 = 0;
        mcE_1 = 0;
        mcEt_1 = 0;
        mcMass_1 = 0;
        mcParentage_1 = 0;
        mcMomPID_1 = 0;
        mcMomPt_1 = 0;
        mcMomEta_1 = 0;
        mcMomPhi_1 = 0;
        mcMomMass_1 = 0;
        mcGMomPID_1 = 0;
        mcIndex_1 = 0;
        mcCalIsoDR03_1 = 0;
        mcCalIsoDR04_1 = 0;
        mcTrkIsoDR03_1 = 0;
        mcTrkIsoDR04_1 = 0;

        mcPID_2 = 0;
        mcStatus_2 = 0;
        mcVtx_x_2 = 0;
        mcVtx_y_2 = 0;
        mcVtx_z_2 = 0;
        mcPt_2 = 0;
        mcEta_2 = 0;
        mcPhi_2 = 0;
        mcE_2 = 0;
        mcEt_2 = 0;
        mcMass_2 = 0;
        mcParentage_2 = 0;
        mcMomPID_2 = 0;
        mcMomPt_2 = 0;
        mcMomEta_2 = 0;
        mcMomPhi_2 = 0;
        mcMomMass_2 = 0;
        mcGMomPID_2 = 0;
        mcIndex_2 = 0;
        mcCalIsoDR03_2 = 0;
        mcCalIsoDR04_2 = 0;
        mcTrkIsoDR03_2 = 0;
        mcTrkIsoDR04_2 = 0;

        diMCM = 0;
        diMCEn = 0;
        diMCPt = 0;
        diMCEta = 0;
        diMCPhi = 0;
        diMCY = 0;
    };
    ~diMC(){};
    void setupDiMCTree(TTree *t);
    void branchDiMCTree(TTree *t);
    void clearDiMCPairs();
    void pushbackDiMCPair(ggHiNtuplizer &tggHiNtuplizer, int i, int j, bool doSizeCheck = true);
    void makeDiMCPairs(ggHiNtuplizer &tggHiNtuplizer, bool doSizeCheck = true);

    // Declaration of leaf types
    Int_t           nMC;
    std::vector<int>     *mcPID_1;
    std::vector<int>     *mcStatus_1;
    std::vector<float>   *mcVtx_x_1;
    std::vector<float>   *mcVtx_y_1;
    std::vector<float>   *mcVtx_z_1;
    std::vector<float>   *mcPt_1;
    std::vector<float>   *mcEta_1;
    std::vector<float>   *mcPhi_1;
    std::vector<float>   *mcE_1;
    std::vector<float>   *mcEt_1;
    std::vector<float>   *mcMass_1;
    std::vector<int>     *mcParentage_1;
    std::vector<int>     *mcMomPID_1;
    std::vector<float>   *mcMomPt_1;
    std::vector<float>   *mcMomEta_1;
    std::vector<float>   *mcMomPhi_1;
    std::vector<float>   *mcMomMass_1;
    std::vector<int>     *mcGMomPID_1;
    std::vector<int>     *mcIndex_1;
    std::vector<float>   *mcCalIsoDR03_1;
    std::vector<float>   *mcCalIsoDR04_1;
    std::vector<float>   *mcTrkIsoDR03_1;
    std::vector<float>   *mcTrkIsoDR04_1;

    std::vector<int>     *mcPID_2;
    std::vector<int>     *mcStatus_2;
    std::vector<float>   *mcVtx_x_2;
    std::vector<float>   *mcVtx_y_2;
    std::vector<float>   *mcVtx_z_2;
    std::vector<float>   *mcPt_2;
    std::vector<float>   *mcEta_2;
    std::vector<float>   *mcPhi_2;
    std::vector<float>   *mcE_2;
    std::vector<float>   *mcEt_2;
    std::vector<float>   *mcMass_2;
    std::vector<int>     *mcParentage_2;
    std::vector<int>     *mcMomPID_2;
    std::vector<float>   *mcMomPt_2;
    std::vector<float>   *mcMomEta_2;
    std::vector<float>   *mcMomPhi_2;
    std::vector<float>   *mcMomMass_2;
    std::vector<int>     *mcGMomPID_2;
    std::vector<int>     *mcIndex_2;
    std::vector<float>   *mcCalIsoDR03_2;
    std::vector<float>   *mcCalIsoDR04_2;
    std::vector<float>   *mcTrkIsoDR03_2;
    std::vector<float>   *mcTrkIsoDR04_2;

    std::vector<float>   *diMCM;
    std::vector<float>   *diMCEn;
    std::vector<float>   *diMCPt;
    std::vector<float>   *diMCEta;
    std::vector<float>   *diMCPhi;
    std::vector<float>   *diMCY;

    // List of branches
    TBranch        *b_nMC;
    TBranch        *b_mcPID_1;
    TBranch        *b_mcStatus_1;
    TBranch        *b_mcVtx_x_1;
    TBranch        *b_mcVtx_y_1;
    TBranch        *b_mcVtx_z_1;
    TBranch        *b_mcPt_1;
    TBranch        *b_mcEta_1;
    TBranch        *b_mcPhi_1;
    TBranch        *b_mcE_1;
    TBranch        *b_mcEt_1;
    TBranch        *b_mcMass_1;
    TBranch        *b_mcParentage_1;
    TBranch        *b_mcMomPID_1;
    TBranch        *b_mcMomPt_1;
    TBranch        *b_mcMomEta_1;
    TBranch        *b_mcMomPhi_1;
    TBranch        *b_mcMomMass_1;
    TBranch        *b_mcGMomPID_1;
    TBranch        *b_mcIndex_1;
    TBranch        *b_mcCalIsoDR03_1;
    TBranch        *b_mcCalIsoDR04_1;
    TBranch        *b_mcTrkIsoDR03_1;
    TBranch        *b_mcTrkIsoDR04_1;

    TBranch        *b_mcPID_2;
    TBranch        *b_mcStatus_2;
    TBranch        *b_mcVtx_x_2;
    TBranch        *b_mcVtx_y_2;
    TBranch        *b_mcVtx_z_2;
    TBranch        *b_mcPt_2;
    TBranch        *b_mcEta_2;
    TBranch        *b_mcPhi_2;
    TBranch        *b_mcE_2;
    TBranch        *b_mcEt_2;
    TBranch        *b_mcMass_2;
    TBranch        *b_mcParentage_2;
    TBranch        *b_mcMomPID_2;
    TBranch        *b_mcMomPt_2;
    TBranch        *b_mcMomEta_2;
    TBranch        *b_mcMomPhi_2;
    TBranch        *b_mcMomMass_2;
    TBranch        *b_mcGMomPID_2;
    TBranch        *b_mcIndex_2;
    TBranch        *b_mcCalIsoDR03_2;
    TBranch        *b_mcCalIsoDR04_2;
    TBranch        *b_mcTrkIsoDR03_2;
    TBranch        *b_mcTrkIsoDR04_2;

    TBranch        *b_diMCM;
    TBranch        *b_diMCEn;
    TBranch        *b_diMCPt;
    TBranch        *b_diMCEta;
    TBranch        *b_diMCPhi;
    TBranch        *b_diMCY;

    // list of objects to be used when creating a diMC Tree
    Int_t           nMC_out;
    std::vector<int>     mcPID_1_out;
    std::vector<int>     mcStatus_1_out;
    std::vector<float>   mcVtx_x_1_out;
    std::vector<float>   mcVtx_y_1_out;
    std::vector<float>   mcVtx_z_1_out;
    std::vector<float>   mcPt_1_out;
    std::vector<float>   mcEta_1_out;
    std::vector<float>   mcPhi_1_out;
    std::vector<float>   mcE_1_out;
    std::vector<float>   mcEt_1_out;
    std::vector<float>   mcMass_1_out;
    std::vector<int>     mcParentage_1_out;
    std::vector<int>     mcMomPID_1_out;
    std::vector<float>   mcMomPt_1_out;
    std::vector<float>   mcMomEta_1_out;
    std::vector<float>   mcMomPhi_1_out;
    std::vector<float>   mcMomMass_1_out;
    std::vector<int>     mcGMomPID_1_out;
    std::vector<int>     mcIndex_1_out;
    std::vector<float>   mcCalIsoDR03_1_out;
    std::vector<float>   mcCalIsoDR04_1_out;
    std::vector<float>   mcTrkIsoDR03_1_out;
    std::vector<float>   mcTrkIsoDR04_1_out;

    std::vector<int>     mcPID_2_out;
    std::vector<int>     mcStatus_2_out;
    std::vector<float>   mcVtx_x_2_out;
    std::vector<float>   mcVtx_y_2_out;
    std::vector<float>   mcVtx_z_2_out;
    std::vector<float>   mcPt_2_out;
    std::vector<float>   mcEta_2_out;
    std::vector<float>   mcPhi_2_out;
    std::vector<float>   mcE_2_out;
    std::vector<float>   mcEt_2_out;
    std::vector<float>   mcMass_2_out;
    std::vector<int>     mcParentage_2_out;
    std::vector<int>     mcMomPID_2_out;
    std::vector<float>   mcMomPt_2_out;
    std::vector<float>   mcMomEta_2_out;
    std::vector<float>   mcMomPhi_2_out;
    std::vector<float>   mcMomMass_2_out;
    std::vector<int>     mcGMomPID_2_out;
    std::vector<int>     mcIndex_2_out;
    std::vector<float>   mcCalIsoDR03_2_out;
    std::vector<float>   mcCalIsoDR04_2_out;
    std::vector<float>   mcTrkIsoDR03_2_out;
    std::vector<float>   mcTrkIsoDR04_2_out;

    std::vector<float>   diMCM_out;
    std::vector<float>   diMCEn_out;
    std::vector<float>   diMCPt_out;
    std::vector<float>   diMCEta_out;
    std::vector<float>   diMCPhi_out;
    std::vector<float>   diMCY_out;
};

void diMC::setupDiMCTree(TTree *t)
{
    // Set branch addresses and branch pointers
    if (t->GetBranch("nMC"))  t->SetBranchAddress("nMC", &nMC, &b_nMC);

    if (t->GetBranch("mcPID_1"))  t->SetBranchAddress("mcPID_1", &mcPID_1, &b_mcPID_1);
    if (t->GetBranch("mcStatus_1"))  t->SetBranchAddress("mcStatus_1", &mcStatus_1, &b_mcStatus_1);
    if (t->GetBranch("mcVtx_x_1"))  t->SetBranchAddress("mcVtx_x_1", &mcVtx_x_1, &b_mcVtx_x_1);
    if (t->GetBranch("mcVtx_y_1"))  t->SetBranchAddress("mcVtx_y_1", &mcVtx_y_1, &b_mcVtx_y_1);
    if (t->GetBranch("mcVtx_z_1"))  t->SetBranchAddress("mcVtx_z_1", &mcVtx_z_1, &b_mcVtx_z_1);
    if (t->GetBranch("mcPt_1"))  t->SetBranchAddress("mcPt_1", &mcPt_1, &b_mcPt_1);
    if (t->GetBranch("mcEta_1"))  t->SetBranchAddress("mcEta_1", &mcEta_1, &b_mcEta_1);
    if (t->GetBranch("mcPhi_1"))  t->SetBranchAddress("mcPhi_1", &mcPhi_1, &b_mcPhi_1);
    if (t->GetBranch("mcE_1"))  t->SetBranchAddress("mcE_1", &mcE_1, &b_mcE_1);
    if (t->GetBranch("mcEt_1"))  t->SetBranchAddress("mcEt_1", &mcEt_1, &b_mcEt_1);
    if (t->GetBranch("mcMass_1"))  t->SetBranchAddress("mcMass_1", &mcMass_1, &b_mcMass_1);
    if (t->GetBranch("mcParentage_1"))  t->SetBranchAddress("mcParentage_1", &mcParentage_1, &b_mcParentage_1);
    if (t->GetBranch("mcMomPID_1"))  t->SetBranchAddress("mcMomPID_1", &mcMomPID_1, &b_mcMomPID_1);
    if (t->GetBranch("mcMomPt_1"))  t->SetBranchAddress("mcMomPt_1", &mcMomPt_1, &b_mcMomPt_1);
    if (t->GetBranch("mcMomEta_1"))  t->SetBranchAddress("mcMomEta_1", &mcMomEta_1, &b_mcMomEta_1);
    if (t->GetBranch("mcMomPhi_1"))  t->SetBranchAddress("mcMomPhi_1", &mcMomPhi_1, &b_mcMomPhi_1);
    if (t->GetBranch("mcMomMass_1"))  t->SetBranchAddress("mcMomMass_1", &mcMomMass_1, &b_mcMomMass_1);
    if (t->GetBranch("mcGMomPID_1"))  t->SetBranchAddress("mcGMomPID_1", &mcGMomPID_1, &b_mcGMomPID_1);
    if (t->GetBranch("mcIndex_1"))  t->SetBranchAddress("mcIndex_1", &mcIndex_1, &b_mcIndex_1);
    if (t->GetBranch("mcCalIsoDR03_1"))  t->SetBranchAddress("mcCalIsoDR03_1", &mcCalIsoDR03_1, &b_mcCalIsoDR03_1);
    if (t->GetBranch("mcCalIsoDR04_1"))  t->SetBranchAddress("mcCalIsoDR04_1", &mcCalIsoDR04_1, &b_mcCalIsoDR04_1);
    if (t->GetBranch("mcTrkIsoDR03_1"))  t->SetBranchAddress("mcTrkIsoDR03_1", &mcTrkIsoDR03_1, &b_mcTrkIsoDR03_1);
    if (t->GetBranch("mcTrkIsoDR04_1"))  t->SetBranchAddress("mcTrkIsoDR04_1", &mcTrkIsoDR04_1, &b_mcTrkIsoDR04_1);

    if (t->GetBranch("mcPID_2"))  t->SetBranchAddress("mcPID_2", &mcPID_2, &b_mcPID_2);
    if (t->GetBranch("mcStatus_2"))  t->SetBranchAddress("mcStatus_2", &mcStatus_2, &b_mcStatus_2);
    if (t->GetBranch("mcVtx_x_2"))  t->SetBranchAddress("mcVtx_x_2", &mcVtx_x_2, &b_mcVtx_x_2);
    if (t->GetBranch("mcVtx_y_2"))  t->SetBranchAddress("mcVtx_y_2", &mcVtx_y_2, &b_mcVtx_y_2);
    if (t->GetBranch("mcVtx_z_2"))  t->SetBranchAddress("mcVtx_z_2", &mcVtx_z_2, &b_mcVtx_z_2);
    if (t->GetBranch("mcPt_2"))  t->SetBranchAddress("mcPt_2", &mcPt_2, &b_mcPt_2);
    if (t->GetBranch("mcEta_2"))  t->SetBranchAddress("mcEta_2", &mcEta_2, &b_mcEta_2);
    if (t->GetBranch("mcPhi_2"))  t->SetBranchAddress("mcPhi_2", &mcPhi_2, &b_mcPhi_2);
    if (t->GetBranch("mcE_2"))  t->SetBranchAddress("mcE_2", &mcE_2, &b_mcE_2);
    if (t->GetBranch("mcEt_2"))  t->SetBranchAddress("mcEt_2", &mcEt_2, &b_mcEt_2);
    if (t->GetBranch("mcMass_2"))  t->SetBranchAddress("mcMass_2", &mcMass_2, &b_mcMass_2);
    if (t->GetBranch("mcParentage_2"))  t->SetBranchAddress("mcParentage_2", &mcParentage_2, &b_mcParentage_2);
    if (t->GetBranch("mcMomPID_2"))  t->SetBranchAddress("mcMomPID_2", &mcMomPID_2, &b_mcMomPID_2);
    if (t->GetBranch("mcMomPt_2"))  t->SetBranchAddress("mcMomPt_2", &mcMomPt_2, &b_mcMomPt_2);
    if (t->GetBranch("mcMomEta_2"))  t->SetBranchAddress("mcMomEta_2", &mcMomEta_2, &b_mcMomEta_2);
    if (t->GetBranch("mcMomPhi_2"))  t->SetBranchAddress("mcMomPhi_2", &mcMomPhi_2, &b_mcMomPhi_2);
    if (t->GetBranch("mcMomMass_2"))  t->SetBranchAddress("mcMomMass_2", &mcMomMass_2, &b_mcMomMass_2);
    if (t->GetBranch("mcGMomPID_2"))  t->SetBranchAddress("mcGMomPID_2", &mcGMomPID_2, &b_mcGMomPID_2);
    if (t->GetBranch("mcIndex_2"))  t->SetBranchAddress("mcIndex_2", &mcIndex_2, &b_mcIndex_2);
    if (t->GetBranch("mcCalIsoDR03_2"))  t->SetBranchAddress("mcCalIsoDR03_2", &mcCalIsoDR03_2, &b_mcCalIsoDR03_2);
    if (t->GetBranch("mcCalIsoDR04_2"))  t->SetBranchAddress("mcCalIsoDR04_2", &mcCalIsoDR04_2, &b_mcCalIsoDR04_2);
    if (t->GetBranch("mcTrkIsoDR03_2"))  t->SetBranchAddress("mcTrkIsoDR03_2", &mcTrkIsoDR03_2, &b_mcTrkIsoDR03_2);
    if (t->GetBranch("mcTrkIsoDR04_2"))  t->SetBranchAddress("mcTrkIsoDR04_2", &mcTrkIsoDR04_2, &b_mcTrkIsoDR04_2);

    if (t->GetBranch("diMCM"))  t->SetBranchAddress("diMCM", &diMCM, &b_diMCM);
    if (t->GetBranch("diMCEn"))  t->SetBranchAddress("diMCEn", &diMCEn, &b_diMCEn);
    if (t->GetBranch("diMCPt"))  t->SetBranchAddress("diMCPt", &diMCPt, &b_diMCPt);
    if (t->GetBranch("diMCEta"))  t->SetBranchAddress("diMCEta", &diMCEta, &b_diMCEta);
    if (t->GetBranch("diMCPhi"))  t->SetBranchAddress("diMCPhi", &diMCPhi, &b_diMCPhi);
    if (t->GetBranch("diMCY"))  t->SetBranchAddress("diMCY", &diMCY, &b_diMCY);
}

void diMC::branchDiMCTree(TTree *t)
{
    t->Branch("nMC", &nMC_out);

    t->Branch("mcPID_1", &mcPID_1_out);
    t->Branch("mcStatus_1", &mcStatus_1_out);
    t->Branch("mcVtx_x_1", &mcVtx_x_1_out);
    t->Branch("mcVtx_y_1", &mcVtx_y_1_out);
    t->Branch("mcVtx_z_1", &mcVtx_z_1_out);
    t->Branch("mcPt_1", &mcPt_1_out);
    t->Branch("mcEta_1", &mcEta_1_out);
    t->Branch("mcPhi_1", &mcPhi_1_out);
    t->Branch("mcE_1", &mcE_1_out);
    t->Branch("mcEt_1", &mcEt_1_out);
    t->Branch("mcMass_1", &mcMass_1_out);
    t->Branch("mcParentage_1", &mcParentage_1_out);
    t->Branch("mcMomPID_1", &mcMomPID_1_out);
    t->Branch("mcMomPt_1", &mcMomPt_1_out);
    t->Branch("mcMomEta_1", &mcMomEta_1_out);
    t->Branch("mcMomPhi_1", &mcMomPhi_1_out);
    t->Branch("mcMomMass_1", &mcMomMass_1_out);
    t->Branch("mcGMomPID_1", &mcGMomPID_1_out);
    t->Branch("mcIndex_1", &mcIndex_1_out);
    t->Branch("mcCalIsoDR03_1", &mcCalIsoDR03_1_out);
    t->Branch("mcCalIsoDR04_1", &mcCalIsoDR04_1_out);
    t->Branch("mcTrkIsoDR03_1", &mcTrkIsoDR03_1_out);
    t->Branch("mcTrkIsoDR04_1", &mcTrkIsoDR04_1_out);

    t->Branch("mcPID_2", &mcPID_2_out);
    t->Branch("mcStatus_2", &mcStatus_2_out);
    t->Branch("mcVtx_x_2", &mcVtx_x_2_out);
    t->Branch("mcVtx_y_2", &mcVtx_y_2_out);
    t->Branch("mcVtx_z_2", &mcVtx_z_2_out);
    t->Branch("mcPt_2", &mcPt_2_out);
    t->Branch("mcEta_2", &mcEta_2_out);
    t->Branch("mcPhi_2", &mcPhi_2_out);
    t->Branch("mcE_2", &mcE_2_out);
    t->Branch("mcEt_2", &mcEt_2_out);
    t->Branch("mcMass_2", &mcMass_2_out);
    t->Branch("mcParentage_2", &mcParentage_2_out);
    t->Branch("mcMomPID_2", &mcMomPID_2_out);
    t->Branch("mcMomPt_2", &mcMomPt_2_out);
    t->Branch("mcMomEta_2", &mcMomEta_2_out);
    t->Branch("mcMomPhi_2", &mcMomPhi_2_out);
    t->Branch("mcMomMass_2", &mcMomMass_2_out);
    t->Branch("mcGMomPID_2", &mcGMomPID_2_out);
    t->Branch("mcIndex_2", &mcIndex_2_out);
    t->Branch("mcCalIsoDR03_2", &mcCalIsoDR03_2_out);
    t->Branch("mcCalIsoDR04_2", &mcCalIsoDR04_2_out);
    t->Branch("mcTrkIsoDR03_2", &mcTrkIsoDR03_2_out);
    t->Branch("mcTrkIsoDR04_2", &mcTrkIsoDR04_2_out);

    t->Branch("diMCM", &diMCM_out);
    t->Branch("diMCEn", &diMCEn_out);
    t->Branch("diMCPt", &diMCPt_out);
    t->Branch("diMCEta", &diMCEta_out);
    t->Branch("diMCPhi", &diMCPhi_out);
    t->Branch("diMCY", &diMCY_out);
}

void diMC::clearDiMCPairs()
{
    mcPID_1_out.clear();
    mcStatus_1_out.clear();
    mcVtx_x_1_out.clear();
    mcVtx_y_1_out.clear();
    mcVtx_z_1_out.clear();
    mcPt_1_out.clear();
    mcEta_1_out.clear();
    mcPhi_1_out.clear();
    mcE_1_out.clear();
    mcEt_1_out.clear();
    mcMass_1_out.clear();
    mcParentage_1_out.clear();
    mcMomPID_1_out.clear();
    mcMomPt_1_out.clear();
    mcMomEta_1_out.clear();
    mcMomPhi_1_out.clear();
    mcMomMass_1_out.clear();
    mcGMomPID_1_out.clear();
    mcIndex_1_out.clear();
    mcCalIsoDR03_1_out.clear();
    mcCalIsoDR04_1_out.clear();
    mcTrkIsoDR03_1_out.clear();
    mcTrkIsoDR04_1_out.clear();

    mcPID_2_out.clear();
    mcStatus_2_out.clear();
    mcVtx_x_2_out.clear();
    mcVtx_y_2_out.clear();
    mcVtx_z_2_out.clear();
    mcPt_2_out.clear();
    mcEta_2_out.clear();
    mcPhi_2_out.clear();
    mcE_2_out.clear();
    mcEt_2_out.clear();
    mcMass_2_out.clear();
    mcParentage_2_out.clear();
    mcMomPID_2_out.clear();
    mcMomPt_2_out.clear();
    mcMomEta_2_out.clear();
    mcMomPhi_2_out.clear();
    mcMomMass_2_out.clear();
    mcGMomPID_2_out.clear();
    mcIndex_2_out.clear();
    mcCalIsoDR03_2_out.clear();
    mcCalIsoDR04_2_out.clear();
    mcTrkIsoDR03_2_out.clear();
    mcTrkIsoDR04_2_out.clear();

    diMCM_out.clear();
    diMCEn_out.clear();
    diMCPt_out.clear();
    diMCEta_out.clear();
    diMCPhi_out.clear();
    diMCY_out.clear();
}

void diMC::pushbackDiMCPair(ggHiNtuplizer &tggHiNtuplizer, int i, int j, bool doSizeCheck)
{
    if (doSizeCheck) {

        if(tggHiNtuplizer.mcPID->size() == (unsigned)nMC_out)  mcPID_1_out.push_back(tggHiNtuplizer.mcPID->at(i));
        if(tggHiNtuplizer.mcStatus->size() == (unsigned)nMC_out)  mcStatus_1_out.push_back(tggHiNtuplizer.mcStatus->at(i));
        if(tggHiNtuplizer.mcVtx_x->size() == (unsigned)nMC_out)  mcVtx_x_1_out.push_back(tggHiNtuplizer.mcVtx_x->at(i));
        if(tggHiNtuplizer.mcVtx_y->size() == (unsigned)nMC_out)  mcVtx_y_1_out.push_back(tggHiNtuplizer.mcVtx_y->at(i));
        if(tggHiNtuplizer.mcVtx_z->size() == (unsigned)nMC_out)  mcVtx_z_1_out.push_back(tggHiNtuplizer.mcVtx_z->at(i));
        if(tggHiNtuplizer.mcPt->size() == (unsigned)nMC_out)  mcPt_1_out.push_back(tggHiNtuplizer.mcPt->at(i));
        if(tggHiNtuplizer.mcEta->size() == (unsigned)nMC_out)  mcEta_1_out.push_back(tggHiNtuplizer.mcEta->at(i));
        if(tggHiNtuplizer.mcPhi->size() == (unsigned)nMC_out)  mcPhi_1_out.push_back(tggHiNtuplizer.mcPhi->at(i));
        if(tggHiNtuplizer.mcE->size() == (unsigned)nMC_out)  mcE_1_out.push_back(tggHiNtuplizer.mcE->at(i));
        if(tggHiNtuplizer.mcEt->size() == (unsigned)nMC_out)  mcEt_1_out.push_back(tggHiNtuplizer.mcEt->at(i));
        if(tggHiNtuplizer.mcMass->size() == (unsigned)nMC_out)  mcMass_1_out.push_back(tggHiNtuplizer.mcMass->at(i));
        if(tggHiNtuplizer.mcParentage->size() == (unsigned)nMC_out)  mcParentage_1_out.push_back(tggHiNtuplizer.mcParentage->at(i));
        if(tggHiNtuplizer.mcMomPID->size() == (unsigned)nMC_out)  mcMomPID_1_out.push_back(tggHiNtuplizer.mcMomPID->at(i));
        if(tggHiNtuplizer.mcMomPt->size() == (unsigned)nMC_out)  mcMomPt_1_out.push_back(tggHiNtuplizer.mcMomPt->at(i));
        if(tggHiNtuplizer.mcMomEta->size() == (unsigned)nMC_out)  mcMomEta_1_out.push_back(tggHiNtuplizer.mcMomEta->at(i));
        if(tggHiNtuplizer.mcMomPhi->size() == (unsigned)nMC_out)  mcMomPhi_1_out.push_back(tggHiNtuplizer.mcMomPhi->at(i));
        if(tggHiNtuplizer.mcMomMass->size() == (unsigned)nMC_out)  mcMomMass_1_out.push_back(tggHiNtuplizer.mcMomMass->at(i));
        if(tggHiNtuplizer.mcGMomPID->size() == (unsigned)nMC_out)  mcGMomPID_1_out.push_back(tggHiNtuplizer.mcGMomPID->at(i));
        if(tggHiNtuplizer.mcIndex->size() == (unsigned)nMC_out)  mcIndex_1_out.push_back(tggHiNtuplizer.mcIndex->at(i));
        if(tggHiNtuplizer.mcCalIsoDR03->size() == (unsigned)nMC_out)  mcCalIsoDR03_1_out.push_back(tggHiNtuplizer.mcCalIsoDR03->at(i));
        if(tggHiNtuplizer.mcCalIsoDR04->size() == (unsigned)nMC_out)  mcCalIsoDR04_1_out.push_back(tggHiNtuplizer.mcCalIsoDR04->at(i));
        if(tggHiNtuplizer.mcTrkIsoDR03->size() == (unsigned)nMC_out)  mcTrkIsoDR03_1_out.push_back(tggHiNtuplizer.mcTrkIsoDR03->at(i));
        if(tggHiNtuplizer.mcTrkIsoDR04->size() == (unsigned)nMC_out)  mcTrkIsoDR04_1_out.push_back(tggHiNtuplizer.mcTrkIsoDR04->at(i));

        if(tggHiNtuplizer.mcPID->size() == (unsigned)nMC_out)  mcPID_2_out.push_back(tggHiNtuplizer.mcPID->at(j));
        if(tggHiNtuplizer.mcStatus->size() == (unsigned)nMC_out)  mcStatus_2_out.push_back(tggHiNtuplizer.mcStatus->at(j));
        if(tggHiNtuplizer.mcVtx_x->size() == (unsigned)nMC_out)  mcVtx_x_2_out.push_back(tggHiNtuplizer.mcVtx_x->at(j));
        if(tggHiNtuplizer.mcVtx_y->size() == (unsigned)nMC_out)  mcVtx_y_2_out.push_back(tggHiNtuplizer.mcVtx_y->at(j));
        if(tggHiNtuplizer.mcVtx_z->size() == (unsigned)nMC_out)  mcVtx_z_2_out.push_back(tggHiNtuplizer.mcVtx_z->at(j));
        if(tggHiNtuplizer.mcPt->size() == (unsigned)nMC_out)  mcPt_2_out.push_back(tggHiNtuplizer.mcPt->at(j));
        if(tggHiNtuplizer.mcEta->size() == (unsigned)nMC_out)  mcEta_2_out.push_back(tggHiNtuplizer.mcEta->at(j));
        if(tggHiNtuplizer.mcPhi->size() == (unsigned)nMC_out)  mcPhi_2_out.push_back(tggHiNtuplizer.mcPhi->at(j));
        if(tggHiNtuplizer.mcE->size() == (unsigned)nMC_out)  mcE_2_out.push_back(tggHiNtuplizer.mcE->at(j));
        if(tggHiNtuplizer.mcEt->size() == (unsigned)nMC_out)  mcEt_2_out.push_back(tggHiNtuplizer.mcEt->at(j));
        if(tggHiNtuplizer.mcMass->size() == (unsigned)nMC_out)  mcMass_2_out.push_back(tggHiNtuplizer.mcMass->at(j));
        if(tggHiNtuplizer.mcParentage->size() == (unsigned)nMC_out)  mcParentage_2_out.push_back(tggHiNtuplizer.mcParentage->at(j));
        if(tggHiNtuplizer.mcMomPID->size() == (unsigned)nMC_out)  mcMomPID_2_out.push_back(tggHiNtuplizer.mcMomPID->at(j));
        if(tggHiNtuplizer.mcMomPt->size() == (unsigned)nMC_out)  mcMomPt_2_out.push_back(tggHiNtuplizer.mcMomPt->at(j));
        if(tggHiNtuplizer.mcMomEta->size() == (unsigned)nMC_out)  mcMomEta_2_out.push_back(tggHiNtuplizer.mcMomEta->at(j));
        if(tggHiNtuplizer.mcMomPhi->size() == (unsigned)nMC_out)  mcMomPhi_2_out.push_back(tggHiNtuplizer.mcMomPhi->at(j));
        if(tggHiNtuplizer.mcMomMass->size() == (unsigned)nMC_out)  mcMomMass_2_out.push_back(tggHiNtuplizer.mcMomMass->at(j));
        if(tggHiNtuplizer.mcGMomPID->size() == (unsigned)nMC_out)  mcGMomPID_2_out.push_back(tggHiNtuplizer.mcGMomPID->at(j));
        if(tggHiNtuplizer.mcIndex->size() == (unsigned)nMC_out)  mcIndex_2_out.push_back(tggHiNtuplizer.mcIndex->at(j));
        if(tggHiNtuplizer.mcCalIsoDR03->size() == (unsigned)nMC_out)  mcCalIsoDR03_2_out.push_back(tggHiNtuplizer.mcCalIsoDR03->at(j));
        if(tggHiNtuplizer.mcCalIsoDR04->size() == (unsigned)nMC_out)  mcCalIsoDR04_2_out.push_back(tggHiNtuplizer.mcCalIsoDR04->at(j));
        if(tggHiNtuplizer.mcTrkIsoDR03->size() == (unsigned)nMC_out)  mcTrkIsoDR03_2_out.push_back(tggHiNtuplizer.mcTrkIsoDR03->at(j));
        if(tggHiNtuplizer.mcTrkIsoDR04->size() == (unsigned)nMC_out)  mcTrkIsoDR04_2_out.push_back(tggHiNtuplizer.mcTrkIsoDR04->at(j));

        // diMC
        if (tggHiNtuplizer.mcPt->size()  == (unsigned)nMC_out &&
            tggHiNtuplizer.mcEta->size() == (unsigned)nMC_out &&
            tggHiNtuplizer.mcPhi->size() == (unsigned)nMC_out)
        {
            TLorentzVector v1, v2, vSum;
            v1.SetPtEtaPhiM( tggHiNtuplizer.mcPt->at(i), tggHiNtuplizer.mcEta->at(i),
                    tggHiNtuplizer.mcPhi->at(i), tggHiNtuplizer.mcMass->at(i));
            v2.SetPtEtaPhiM( tggHiNtuplizer.mcPt->at(j), tggHiNtuplizer.mcEta->at(j),
                    tggHiNtuplizer.mcPhi->at(j), tggHiNtuplizer.mcMass->at(j));
            vSum = v1+v2;

            diMCM_out.push_back(vSum.M());
            diMCEn_out.push_back(vSum.Energy());
            diMCPt_out.push_back(vSum.Pt());
            diMCEta_out.push_back(vSum.Eta());
            diMCPhi_out.push_back(vSum.Phi());
            diMCY_out.push_back(vSum.Rapidity());
        }
    }
    else {

        mcPID_1_out.push_back(tggHiNtuplizer.mcPID->at(i));
        mcStatus_1_out.push_back(tggHiNtuplizer.mcStatus->at(i));
        mcVtx_x_1_out.push_back(tggHiNtuplizer.mcVtx_x->at(i));
        mcVtx_y_1_out.push_back(tggHiNtuplizer.mcVtx_y->at(i));
        mcVtx_z_1_out.push_back(tggHiNtuplizer.mcVtx_z->at(i));
        mcPt_1_out.push_back(tggHiNtuplizer.mcPt->at(i));
        mcEta_1_out.push_back(tggHiNtuplizer.mcEta->at(i));
        mcPhi_1_out.push_back(tggHiNtuplizer.mcPhi->at(i));
        mcE_1_out.push_back(tggHiNtuplizer.mcE->at(i));
        mcEt_1_out.push_back(tggHiNtuplizer.mcEt->at(i));
        mcMass_1_out.push_back(tggHiNtuplizer.mcMass->at(i));
        mcParentage_1_out.push_back(tggHiNtuplizer.mcParentage->at(i));
        mcMomPID_1_out.push_back(tggHiNtuplizer.mcMomPID->at(i));
        mcMomPt_1_out.push_back(tggHiNtuplizer.mcMomPt->at(i));
        mcMomEta_1_out.push_back(tggHiNtuplizer.mcMomEta->at(i));
        mcMomPhi_1_out.push_back(tggHiNtuplizer.mcMomPhi->at(i));
        mcMomMass_1_out.push_back(tggHiNtuplizer.mcMomMass->at(i));
        mcGMomPID_1_out.push_back(tggHiNtuplizer.mcGMomPID->at(i));
        mcIndex_1_out.push_back(tggHiNtuplizer.mcIndex->at(i));
        mcCalIsoDR03_1_out.push_back(tggHiNtuplizer.mcCalIsoDR03->at(i));
        mcCalIsoDR04_1_out.push_back(tggHiNtuplizer.mcCalIsoDR04->at(i));
        mcTrkIsoDR03_1_out.push_back(tggHiNtuplizer.mcTrkIsoDR03->at(i));
        mcTrkIsoDR04_1_out.push_back(tggHiNtuplizer.mcTrkIsoDR04->at(i));

        mcPID_2_out.push_back(tggHiNtuplizer.mcPID->at(j));
        mcStatus_2_out.push_back(tggHiNtuplizer.mcStatus->at(j));
        mcVtx_x_2_out.push_back(tggHiNtuplizer.mcVtx_x->at(j));
        mcVtx_y_2_out.push_back(tggHiNtuplizer.mcVtx_y->at(j));
        mcVtx_z_2_out.push_back(tggHiNtuplizer.mcVtx_z->at(j));
        mcPt_2_out.push_back(tggHiNtuplizer.mcPt->at(j));
        mcEta_2_out.push_back(tggHiNtuplizer.mcEta->at(j));
        mcPhi_2_out.push_back(tggHiNtuplizer.mcPhi->at(j));
        mcE_2_out.push_back(tggHiNtuplizer.mcE->at(j));
        mcEt_2_out.push_back(tggHiNtuplizer.mcEt->at(j));
        mcMass_2_out.push_back(tggHiNtuplizer.mcMass->at(j));
        mcParentage_2_out.push_back(tggHiNtuplizer.mcParentage->at(j));
        mcMomPID_2_out.push_back(tggHiNtuplizer.mcMomPID->at(j));
        mcMomPt_2_out.push_back(tggHiNtuplizer.mcMomPt->at(j));
        mcMomEta_2_out.push_back(tggHiNtuplizer.mcMomEta->at(j));
        mcMomPhi_2_out.push_back(tggHiNtuplizer.mcMomPhi->at(j));
        mcMomMass_2_out.push_back(tggHiNtuplizer.mcMomMass->at(j));
        mcGMomPID_2_out.push_back(tggHiNtuplizer.mcGMomPID->at(j));
        mcIndex_2_out.push_back(tggHiNtuplizer.mcIndex->at(j));
        mcCalIsoDR03_2_out.push_back(tggHiNtuplizer.mcCalIsoDR03->at(j));
        mcCalIsoDR04_2_out.push_back(tggHiNtuplizer.mcCalIsoDR04->at(j));
        mcTrkIsoDR03_2_out.push_back(tggHiNtuplizer.mcTrkIsoDR03->at(j));
        mcTrkIsoDR04_2_out.push_back(tggHiNtuplizer.mcTrkIsoDR04->at(j));

        // diMC
        TLorentzVector v1, v2, vSum;
        v1.SetPtEtaPhiM( tggHiNtuplizer.mcPt->at(i), tggHiNtuplizer.mcEta->at(i),
                tggHiNtuplizer.mcPhi->at(i), tggHiNtuplizer.mcMass->at(i));
        v2.SetPtEtaPhiM( tggHiNtuplizer.mcPt->at(j), tggHiNtuplizer.mcEta->at(j),
                tggHiNtuplizer.mcPhi->at(j), tggHiNtuplizer.mcMass->at(j));
        vSum = v1+v2;

        diMCM_out.push_back(vSum.M());
        diMCEn_out.push_back(vSum.Energy());
        diMCPt_out.push_back(vSum.Pt());
        diMCEta_out.push_back(vSum.Eta());
        diMCPhi_out.push_back(vSum.Phi());
        diMCY_out.push_back(vSum.Rapidity());
    }
}

void diMC::makeDiMCPairs(ggHiNtuplizer &tggHiNtuplizer, bool doSizeCheck)
{
    clearDiMCPairs();

    nMC_out = tggHiNtuplizer.nMC;

    for(int i=0; i<nMC_out; ++i)
    {
        for(int j=i+1; j<nMC_out; ++j)
        {
            pushbackDiMCPair(tggHiNtuplizer, i, j, doSizeCheck);
        }
    }
}

#endif /* TREEHEADERS_DIMCTREE_H_ */
