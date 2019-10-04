/*
 * diphotonTree.h
 *
 *      Author: Kaya Tatar
 *
 *       class to manage diphoton tree, tree of photon pairs that match to an electron
 */

#ifndef TREEHEADERS_DIPHOTONTREE_H_
#define TREEHEADERS_DIPHOTONTREE_H_

#include <TTree.h>
#include <TLorentzVector.h>

#include <vector>

#include "ggHiNtuplizerTree.h"
#include "../Utilities/physicsUtil.h"

class diphoton {
public :
    diphoton(){

        cutDeltaR = 0.15;

        phoE_1 = 0;
        phoEt_1 = 0;
        phoEta_1 = 0;
        phoPhi_1 = 0;
        phoSCE_1 = 0;
        phoSCRawE_1 = 0;
        phoESEn_1 = 0;
        phoSCEta_1 = 0;
        phoSCPhi_1 = 0;
        phoSCEtaWidth_1 = 0;
        phoSCPhiWidth_1 = 0;
        phoSCBrem_1 = 0;
        phohasPixelSeed_1 = 0;
        phoR9_1 = 0;
        phoHoverE_1 = 0;
        phoSigmaIEtaIEta_1 = 0;
        // phoE1x3_1 = 0;
        // phoE2x2_1 = 0;
        phoE3x3_1 = 0;
        // phoE2x5Max_1 = 0;
        phoE1x5_1 = 0;
        phoE2x5_1 = 0;
        phoE5x5_1 = 0;
        phoMaxEnergyXtal_1 = 0;
        phoSigmaEtaEta_1 = 0;
        phoR1x5_1 = 0;
        phoR2x5_1 = 0;
        phoESEffSigmaRR_1 = 0;
        phoSigmaIEtaIEta_2012_1 = 0;
        // phoSigmaIEtaIPhi_2012_1 = 0;
        // phoSigmaIPhiIPhi_2012_1 = 0;
        // phoE1x3_2012_1 = 0;
        // phoE2x2_2012_1 = 0;
        phoE3x3_2012_1 = 0;
        // phoE2x5Max_2012_1 = 0;
        // phoE5x5_2012_1 = 0;
        phoBC1E_1 = 0;
        phoBC1Eta_1 = 0;
        phoBC2E_1 = 0;
        phoBC2Eta_1 = 0;
        pho_ecalClusterIsoR2_1 = 0;
        pho_ecalClusterIsoR3_1 = 0;
        pho_ecalClusterIsoR4_1 = 0;
        pho_ecalClusterIsoR5_1 = 0;
        pho_hcalRechitIsoR1_1 = 0;
        pho_hcalRechitIsoR2_1 = 0;
        pho_hcalRechitIsoR3_1 = 0;
        pho_hcalRechitIsoR4_1 = 0;
        pho_hcalRechitIsoR5_1 = 0;
        pho_trackIsoR1PtCut20_1 = 0;
        pho_trackIsoR2PtCut20_1 = 0;
        pho_trackIsoR3PtCut20_1 = 0;
        pho_trackIsoR4PtCut20_1 = 0;
        pho_trackIsoR5PtCut20_1 = 0;
        pho_swissCrx_1 = 0;
        pho_seedTime_1 = 0;
        matched_eleCharge_1 = 0;
        matched_eleIndex_1 = 0;
        matched_elePt_1 = 0;
        matched_eleEta_1 = 0;
        matched_elePhi_1 = 0;
        matched_eleTrkPt_1 = 0;

        phoE_2 = 0;
        phoEt_2 = 0;
        phoEta_2 = 0;
        phoPhi_2 = 0;
        phoSCE_2 = 0;
        phoSCRawE_2 = 0;
        phoESEn_2 = 0;
        phoSCEta_2 = 0;
        phoSCPhi_2 = 0;
        phoSCEtaWidth_2 = 0;
        phoSCPhiWidth_2 = 0;
        phoSCBrem_2 = 0;
        phohasPixelSeed_2 = 0;
        phoR9_2 = 0;
        phoHoverE_2 = 0;
        phoSigmaIEtaIEta_2 = 0;
        // phoE1x3_2 = 0;
        // phoE2x2_2 = 0;
        phoE3x3_2 = 0;
        // phoE2x5Max_2 = 0;
        phoE1x5_2 = 0;
        phoE2x5_2 = 0;
        phoE5x5_2 = 0;
        phoMaxEnergyXtal_2 = 0;
        phoSigmaEtaEta_2 = 0;
        phoR1x5_2 = 0;
        phoR2x5_2 = 0;
        phoESEffSigmaRR_2 = 0;
        phoSigmaIEtaIEta_2012_2 = 0;
        // phoSigmaIEtaIPhi_2012_2 = 0;
        // phoSigmaIPhiIPhi_2012_2 = 0;
        // phoE1x3_2012_2 = 0;
        // phoE2x2_2012_2 = 0;
        phoE3x3_2012_2 = 0;
        // phoE2x5Max_2012_2 = 0;
        // phoE5x5_2012_2 = 0;
        phoBC1E_2 = 0;
        phoBC1Eta_2 = 0;
        phoBC2E_2 = 0;
        phoBC2Eta_2 = 0;
        pho_ecalClusterIsoR2_2 = 0;
        pho_ecalClusterIsoR3_2 = 0;
        pho_ecalClusterIsoR4_2 = 0;
        pho_ecalClusterIsoR5_2 = 0;
        pho_hcalRechitIsoR1_2 = 0;
        pho_hcalRechitIsoR2_2 = 0;
        pho_hcalRechitIsoR3_2 = 0;
        pho_hcalRechitIsoR4_2 = 0;
        pho_hcalRechitIsoR5_2 = 0;
        pho_trackIsoR1PtCut20_2 = 0;
        pho_trackIsoR2PtCut20_2 = 0;
        pho_trackIsoR3PtCut20_2 = 0;
        pho_trackIsoR4PtCut20_2 = 0;
        pho_trackIsoR5PtCut20_2 = 0;
        pho_swissCrx_2 = 0;
        pho_seedTime_2 = 0;
        matched_eleCharge_2 = 0;
        matched_eleIndex_2 = 0;
        matched_elePt_2 = 0;
        matched_eleEta_2 = 0;
        matched_elePhi_2 = 0;
        matched_eleTrkPt_2 = 0;

        diPhoM = 0;
        diPhoEn = 0;
        diPhoPt = 0;
        diPhoEta = 0;
        diPhoPhi = 0;
    };
    ~diphoton(){};
    void setupDiPhotonTree(TTree *t);
    void branchDiPhotonTree(TTree *t);
    void clearDiPhotonPairs();
    void pushbackDiPhotonPair(ggHiNtuplizer &tggHiNtuplizer, int i, int j, bool doSizeCheck = true);
    void makeDiPhotonPairs(ggHiNtuplizer &tggHiNtuplizer, bool doEleMatch = true, bool skipIfNoMatch = false, bool doSizeCheck = true);
    void makeDiPhotonPair(ggHiNtuplizer &tggHiNtuplizer, int i1, int i2, int matched_Index_1 = -1, int matched_Index_2 = -1, bool doSizeCheck = true);

    float cutDeltaR;
    // Declaration of leaf types
    Int_t           nPho;
    std::vector<float>   *phoE_1;
    std::vector<float>   *phoEt_1;
    std::vector<float>   *phoEta_1;
    std::vector<float>   *phoPhi_1;
    std::vector<float>   *phoSCE_1;
    std::vector<float>   *phoSCRawE_1;
    std::vector<float>   *phoESEn_1;
    std::vector<float>   *phoSCEta_1;
    std::vector<float>   *phoSCPhi_1;
    std::vector<float>   *phoSCEtaWidth_1;
    std::vector<float>   *phoSCPhiWidth_1;
    std::vector<float>   *phoSCBrem_1;
    std::vector<int>     *phohasPixelSeed_1;
    std::vector<float>   *phoR9_1;
    std::vector<float>   *phoHoverE_1;
    std::vector<float>   *phoSigmaIEtaIEta_1;
    // std::vector<float>   *phoE1x3_1;
    // std::vector<float>   *phoE2x2_1;
    std::vector<float>   *phoE3x3_1;
    // std::vector<float>   *phoE2x5Max_1;
    std::vector<float>   *phoE1x5_1;
    std::vector<float>   *phoE2x5_1;
    std::vector<float>   *phoE5x5_1;
    std::vector<float>   *phoMaxEnergyXtal_1;
    std::vector<float>   *phoSigmaEtaEta_1;
    std::vector<float>   *phoR1x5_1;
    std::vector<float>   *phoR2x5_1;
    std::vector<float>   *phoESEffSigmaRR_1;
    std::vector<float>   *phoSigmaIEtaIEta_2012_1;
    // std::vector<float>   *phoSigmaIEtaIPhi_2012_1;
    // std::vector<float>   *phoSigmaIPhiIPhi_2012_1;
    // std::vector<float>   *phoE1x3_2012_1;
    // std::vector<float>   *phoE2x2_2012_1;
    std::vector<float>   *phoE3x3_2012_1;
    // std::vector<float>   *phoE2x5Max_2012_1;
    // std::vector<float>   *phoE5x5_2012_1;
    std::vector<float>   *phoBC1E_1;
    std::vector<float>   *phoBC1Eta_1;
    std::vector<float>   *phoBC2E_1;
    std::vector<float>   *phoBC2Eta_1;
    std::vector<float>   *pho_ecalClusterIsoR2_1;
    std::vector<float>   *pho_ecalClusterIsoR3_1;
    std::vector<float>   *pho_ecalClusterIsoR4_1;
    std::vector<float>   *pho_ecalClusterIsoR5_1;
    std::vector<float>   *pho_hcalRechitIsoR1_1;
    std::vector<float>   *pho_hcalRechitIsoR2_1;
    std::vector<float>   *pho_hcalRechitIsoR3_1;
    std::vector<float>   *pho_hcalRechitIsoR4_1;
    std::vector<float>   *pho_hcalRechitIsoR5_1;
    std::vector<float>   *pho_trackIsoR1PtCut20_1;
    std::vector<float>   *pho_trackIsoR2PtCut20_1;
    std::vector<float>   *pho_trackIsoR3PtCut20_1;
    std::vector<float>   *pho_trackIsoR4PtCut20_1;
    std::vector<float>   *pho_trackIsoR5PtCut20_1;
    std::vector<float>   *pho_swissCrx_1;
    std::vector<float>   *pho_seedTime_1;
    std::vector<int>     *matched_eleCharge_1;
    std::vector<int>     *matched_eleIndex_1;
    std::vector<float>   *matched_elePt_1;
    std::vector<float>   *matched_eleEta_1;
    std::vector<float>   *matched_elePhi_1;
    std::vector<float>   *matched_eleTrkPt_1;

    std::vector<float>   *phoE_2;
    std::vector<float>   *phoEt_2;
    std::vector<float>   *phoEta_2;
    std::vector<float>   *phoPhi_2;
    std::vector<float>   *phoSCE_2;
    std::vector<float>   *phoSCRawE_2;
    std::vector<float>   *phoESEn_2;
    std::vector<float>   *phoSCEta_2;
    std::vector<float>   *phoSCPhi_2;
    std::vector<float>   *phoSCEtaWidth_2;
    std::vector<float>   *phoSCPhiWidth_2;
    std::vector<float>   *phoSCBrem_2;
    std::vector<int>     *phohasPixelSeed_2;
    std::vector<float>   *phoR9_2;
    std::vector<float>   *phoHoverE_2;
    std::vector<float>   *phoSigmaIEtaIEta_2;
    // std::vector<float>   *phoE1x3_2;
    // std::vector<float>   *phoE2x2_2;
    std::vector<float>   *phoE3x3_2;
    // std::vector<float>   *phoE2x5Max_2;
    std::vector<float>   *phoE1x5_2;
    std::vector<float>   *phoE2x5_2;
    std::vector<float>   *phoE5x5_2;
    std::vector<float>   *phoMaxEnergyXtal_2;
    std::vector<float>   *phoSigmaEtaEta_2;
    std::vector<float>   *phoR1x5_2;
    std::vector<float>   *phoR2x5_2;
    std::vector<float>   *phoESEffSigmaRR_2;
    std::vector<float>   *phoSigmaIEtaIEta_2012_2;
    // std::vector<float>   *phoSigmaIEtaIPhi_2012_2;
    // std::vector<float>   *phoSigmaIPhiIPhi_2012_2;
    // std::vector<float>   *phoE1x3_2012_2;
    // std::vector<float>   *phoE2x2_2012_2;
    std::vector<float>   *phoE3x3_2012_2;
    // std::vector<float>   *phoE2x5Max_2012_2;
    // std::vector<float>   *phoE5x5_2012_2;
    std::vector<float>   *phoBC1E_2;
    std::vector<float>   *phoBC1Eta_2;
    std::vector<float>   *phoBC2E_2;
    std::vector<float>   *phoBC2Eta_2;
    std::vector<float>   *pho_ecalClusterIsoR2_2;
    std::vector<float>   *pho_ecalClusterIsoR3_2;
    std::vector<float>   *pho_ecalClusterIsoR4_2;
    std::vector<float>   *pho_ecalClusterIsoR5_2;
    std::vector<float>   *pho_hcalRechitIsoR1_2;
    std::vector<float>   *pho_hcalRechitIsoR2_2;
    std::vector<float>   *pho_hcalRechitIsoR3_2;
    std::vector<float>   *pho_hcalRechitIsoR4_2;
    std::vector<float>   *pho_hcalRechitIsoR5_2;
    std::vector<float>   *pho_trackIsoR1PtCut20_2;
    std::vector<float>   *pho_trackIsoR2PtCut20_2;
    std::vector<float>   *pho_trackIsoR3PtCut20_2;
    std::vector<float>   *pho_trackIsoR4PtCut20_2;
    std::vector<float>   *pho_trackIsoR5PtCut20_2;
    std::vector<float>   *pho_swissCrx_2;
    std::vector<float>   *pho_seedTime_2;
    std::vector<int>     *matched_eleCharge_2;
    std::vector<int>     *matched_eleIndex_2;
    std::vector<float>   *matched_elePt_2;
    std::vector<float>   *matched_eleEta_2;
    std::vector<float>   *matched_elePhi_2;
    std::vector<float>   *matched_eleTrkPt_2;

    std::vector<float>   *diPhoM;
    std::vector<float>   *diPhoEn;
    std::vector<float>   *diPhoPt;
    std::vector<float>   *diPhoEta;
    std::vector<float>   *diPhoPhi;

    // List of branches
    TBranch        *b_nPho;
    TBranch        *b_phoE_1;
    TBranch        *b_phoEt_1;
    TBranch        *b_phoEta_1;
    TBranch        *b_phoPhi_1;
    TBranch        *b_phoSCE_1;
    TBranch        *b_phoSCRawE_1;
    TBranch        *b_phoESEn_1;
    TBranch        *b_phoSCEta_1;
    TBranch        *b_phoSCPhi_1;
    TBranch        *b_phoSCEtaWidth_1;
    TBranch        *b_phoSCPhiWidth_1;
    TBranch        *b_phoSCBrem_1;
    TBranch        *b_phohasPixelSeed_1;
    TBranch        *b_phoR9_1;
    TBranch        *b_phoHoverE_1;
    TBranch        *b_phoSigmaIEtaIEta_1;
    // TBranch        *b_phoE1x3_1;
    // TBranch        *b_phoE2x2_1;
    TBranch        *b_phoE3x3_1;
    // TBranch        *b_phoE2x5Max_1;
    TBranch        *b_phoE1x5_1;
    TBranch        *b_phoE2x5_1;
    TBranch        *b_phoE5x5_1;
    TBranch        *b_phoMaxEnergyXtal_1;
    TBranch        *b_phoSigmaEtaEta_1;
    TBranch        *b_phoR1x5_1;
    TBranch        *b_phoR2x5_1;
    TBranch        *b_phoESEffSigmaRR_1;
    TBranch        *b_phoSigmaIEtaIEta_2012_1;
    // TBranch        *b_phoSigmaIEtaIPhi_2012_1;
    // TBranch        *b_phoSigmaIPhiIPhi_2012_1;
    // TBranch        *b_phoE1x3_2012_1;
    // TBranch        *b_phoE2x2_2012_1;
    TBranch        *b_phoE3x3_2012_1;
    // TBranch        *b_phoE2x5Max_2012_1;
    // TBranch        *b_phoE5x5_2012_1;
    TBranch        *b_phoBC1E_1;
    TBranch        *b_phoBC1Eta_1;
    TBranch        *b_phoBC2E_1;
    TBranch        *b_phoBC2Eta_1;
    TBranch        *b_pho_ecalClusterIsoR2_1;
    TBranch        *b_pho_ecalClusterIsoR3_1;
    TBranch        *b_pho_ecalClusterIsoR4_1;
    TBranch        *b_pho_ecalClusterIsoR5_1;
    TBranch        *b_pho_hcalRechitIsoR1_1;
    TBranch        *b_pho_hcalRechitIsoR2_1;
    TBranch        *b_pho_hcalRechitIsoR3_1;
    TBranch        *b_pho_hcalRechitIsoR4_1;
    TBranch        *b_pho_hcalRechitIsoR5_1;
    TBranch        *b_pho_trackIsoR1PtCut20_1;
    TBranch        *b_pho_trackIsoR2PtCut20_1;
    TBranch        *b_pho_trackIsoR3PtCut20_1;
    TBranch        *b_pho_trackIsoR4PtCut20_1;
    TBranch        *b_pho_trackIsoR5PtCut20_1;
    TBranch        *b_pho_swissCrx_1;
    TBranch        *b_pho_seedTime_1;
    TBranch        *b_matched_eleCharge_1;
    TBranch        *b_matched_eleIndex_1;
    TBranch        *b_matched_elePt_1;
    TBranch        *b_matched_eleEta_1;
    TBranch        *b_matched_elePhi_1;
    TBranch        *b_matched_eleTrkPt_1;

    TBranch        *b_phoE_2;
    TBranch        *b_phoEt_2;
    TBranch        *b_phoEta_2;
    TBranch        *b_phoPhi_2;
    TBranch        *b_phoSCE_2;
    TBranch        *b_phoSCRawE_2;
    TBranch        *b_phoESEn_2;
    TBranch        *b_phoSCEta_2;
    TBranch        *b_phoSCPhi_2;
    TBranch        *b_phoSCEtaWidth_2;
    TBranch        *b_phoSCPhiWidth_2;
    TBranch        *b_phoSCBrem_2;
    TBranch        *b_phohasPixelSeed_2;
    TBranch        *b_phoR9_2;
    TBranch        *b_phoHoverE_2;
    TBranch        *b_phoSigmaIEtaIEta_2;
    // TBranch        *b_phoE1x3_2;
    // TBranch        *b_phoE2x2_2;
    TBranch        *b_phoE3x3_2;
    // TBranch        *b_phoE2x5Max_2;
    TBranch        *b_phoE1x5_2;
    TBranch        *b_phoE2x5_2;
    TBranch        *b_phoE5x5_2;
    TBranch        *b_phoMaxEnergyXtal_2;
    TBranch        *b_phoSigmaEtaEta_2;
    TBranch        *b_phoR1x5_2;
    TBranch        *b_phoR2x5_2;
    TBranch        *b_phoESEffSigmaRR_2;
    TBranch        *b_phoSigmaIEtaIEta_2012_2;
    // TBranch        *b_phoSigmaIEtaIPhi_2012_2;
    // TBranch        *b_phoSigmaIPhiIPhi_2012_2;
    // TBranch        *b_phoE1x3_2012_2;
    // TBranch        *b_phoE2x2_2012_2;
    TBranch        *b_phoE3x3_2012_2;
    // TBranch        *b_phoE2x5Max_2012_2;
    // TBranch        *b_phoE5x5_2012_2;
    TBranch        *b_phoBC1E_2;
    TBranch        *b_phoBC1Eta_2;
    TBranch        *b_phoBC2E_2;
    TBranch        *b_phoBC2Eta_2;
    TBranch        *b_pho_ecalClusterIsoR2_2;
    TBranch        *b_pho_ecalClusterIsoR3_2;
    TBranch        *b_pho_ecalClusterIsoR4_2;
    TBranch        *b_pho_ecalClusterIsoR5_2;
    TBranch        *b_pho_hcalRechitIsoR1_2;
    TBranch        *b_pho_hcalRechitIsoR2_2;
    TBranch        *b_pho_hcalRechitIsoR3_2;
    TBranch        *b_pho_hcalRechitIsoR4_2;
    TBranch        *b_pho_hcalRechitIsoR5_2;
    TBranch        *b_pho_trackIsoR1PtCut20_2;
    TBranch        *b_pho_trackIsoR2PtCut20_2;
    TBranch        *b_pho_trackIsoR3PtCut20_2;
    TBranch        *b_pho_trackIsoR4PtCut20_2;
    TBranch        *b_pho_trackIsoR5PtCut20_2;
    TBranch        *b_pho_swissCrx_2;
    TBranch        *b_pho_seedTime_2;
    TBranch        *b_matched_eleCharge_2;
    TBranch        *b_matched_eleIndex_2;
    TBranch        *b_matched_elePt_2;
    TBranch        *b_matched_eleEta_2;
    TBranch        *b_matched_elePhi_2;
    TBranch        *b_matched_eleTrkPt_2;

    TBranch        *b_diPhoM;
    TBranch        *b_diPhoEn;
    TBranch        *b_diPhoPt;
    TBranch        *b_diPhoEta;
    TBranch        *b_diPhoPhi;

    // list of objects to be used when creating a diPhoton Tree
    Int_t           nPho_out;
    std::vector<float>   phoE_1_out;
    std::vector<float>   phoEt_1_out;
    std::vector<float>   phoEta_1_out;
    std::vector<float>   phoPhi_1_out;
    std::vector<float>   phoSCE_1_out;
    std::vector<float>   phoSCRawE_1_out;
    std::vector<float>   phoESEn_1_out;
    std::vector<float>   phoSCEta_1_out;
    std::vector<float>   phoSCPhi_1_out;
    std::vector<float>   phoSCEtaWidth_1_out;
    std::vector<float>   phoSCPhiWidth_1_out;
    std::vector<float>   phoSCBrem_1_out;
    std::vector<int>     phohasPixelSeed_1_out;
    std::vector<float>   phoR9_1_out;
    std::vector<float>   phoHoverE_1_out;
    std::vector<float>   phoSigmaIEtaIEta_1_out;
    // std::vector<float>   phoE1x3_1_out;
    // std::vector<float>   phoE2x2_1_out;
    std::vector<float>   phoE3x3_1_out;
    // std::vector<float>   phoE2x5Max_1_out;
    std::vector<float>   phoE1x5_1_out;
    std::vector<float>   phoE2x5_1_out;
    std::vector<float>   phoE5x5_1_out;
    std::vector<float>   phoMaxEnergyXtal_1_out;
    std::vector<float>   phoSigmaEtaEta_1_out;
    std::vector<float>   phoR1x5_1_out;
    std::vector<float>   phoR2x5_1_out;
    std::vector<float>   phoESEffSigmaRR_1_out;
    std::vector<float>   phoSigmaIEtaIEta_2012_1_out;
    // std::vector<float>   phoSigmaIEtaIPhi_2012_1_out;
    // std::vector<float>   phoSigmaIPhiIPhi_2012_1_out;
    // std::vector<float>   phoE1x3_2012_1_out;
    // std::vector<float>   phoE2x2_2012_1_out;
    std::vector<float>   phoE3x3_2012_1_out;
    // std::vector<float>   phoE2x5Max_2012_1_out;
    // std::vector<float>   phoE5x5_2012_1_out;
    std::vector<float>   phoBC1E_1_out;
    std::vector<float>   phoBC1Eta_1_out;
    std::vector<float>   phoBC2E_1_out;
    std::vector<float>   phoBC2Eta_1_out;
    std::vector<float>   pho_ecalClusterIsoR2_1_out;
    std::vector<float>   pho_ecalClusterIsoR3_1_out;
    std::vector<float>   pho_ecalClusterIsoR4_1_out;
    std::vector<float>   pho_ecalClusterIsoR5_1_out;
    std::vector<float>   pho_hcalRechitIsoR1_1_out;
    std::vector<float>   pho_hcalRechitIsoR2_1_out;
    std::vector<float>   pho_hcalRechitIsoR3_1_out;
    std::vector<float>   pho_hcalRechitIsoR4_1_out;
    std::vector<float>   pho_hcalRechitIsoR5_1_out;
    std::vector<float>   pho_trackIsoR1PtCut20_1_out;
    std::vector<float>   pho_trackIsoR2PtCut20_1_out;
    std::vector<float>   pho_trackIsoR3PtCut20_1_out;
    std::vector<float>   pho_trackIsoR4PtCut20_1_out;
    std::vector<float>   pho_trackIsoR5PtCut20_1_out;
    std::vector<float>   pho_swissCrx_1_out;
    std::vector<float>   pho_seedTime_1_out;
    std::vector<int>     matched_eleCharge_1_out;
    std::vector<int>     matched_eleIndex_1_out;
    std::vector<float>   matched_elePt_1_out;
    std::vector<float>   matched_eleEta_1_out;
    std::vector<float>   matched_elePhi_1_out;
    std::vector<float>   matched_eleTrkPt_1_out;

    std::vector<float>   phoE_2_out;
    std::vector<float>   phoEt_2_out;
    std::vector<float>   phoEta_2_out;
    std::vector<float>   phoPhi_2_out;
    std::vector<float>   phoSCE_2_out;
    std::vector<float>   phoSCRawE_2_out;
    std::vector<float>   phoESEn_2_out;
    std::vector<float>   phoSCEta_2_out;
    std::vector<float>   phoSCPhi_2_out;
    std::vector<float>   phoSCEtaWidth_2_out;
    std::vector<float>   phoSCPhiWidth_2_out;
    std::vector<float>   phoSCBrem_2_out;
    std::vector<int>     phohasPixelSeed_2_out;
    std::vector<float>   phoR9_2_out;
    std::vector<float>   phoHoverE_2_out;
    std::vector<float>   phoSigmaIEtaIEta_2_out;
    // std::vector<float>   phoE1x3_2_out;
    // std::vector<float>   phoE2x2_2_out;
    std::vector<float>   phoE3x3_2_out;
    // std::vector<float>   phoE2x5Max_2_out;
    std::vector<float>   phoE1x5_2_out;
    std::vector<float>   phoE2x5_2_out;
    std::vector<float>   phoE5x5_2_out;
    std::vector<float>   phoMaxEnergyXtal_2_out;
    std::vector<float>   phoSigmaEtaEta_2_out;
    std::vector<float>   phoR1x5_2_out;
    std::vector<float>   phoR2x5_2_out;
    std::vector<float>   phoESEffSigmaRR_2_out;
    std::vector<float>   phoSigmaIEtaIEta_2012_2_out;
    // std::vector<float>   phoSigmaIEtaIPhi_2012_2_out;
    // std::vector<float>   phoSigmaIPhiIPhi_2012_2_out;
    // std::vector<float>   phoE1x3_2012_2_out;
    // std::vector<float>   phoE2x2_2012_2_out;
    std::vector<float>   phoE3x3_2012_2_out;
    // std::vector<float>   phoE2x5Max_2012_2_out;
    // std::vector<float>   phoE5x5_2012_2_out;
    std::vector<float>   phoBC1E_2_out;
    std::vector<float>   phoBC1Eta_2_out;
    std::vector<float>   phoBC2E_2_out;
    std::vector<float>   phoBC2Eta_2_out;
    std::vector<float>   pho_ecalClusterIsoR2_2_out;
    std::vector<float>   pho_ecalClusterIsoR3_2_out;
    std::vector<float>   pho_ecalClusterIsoR4_2_out;
    std::vector<float>   pho_ecalClusterIsoR5_2_out;
    std::vector<float>   pho_hcalRechitIsoR1_2_out;
    std::vector<float>   pho_hcalRechitIsoR2_2_out;
    std::vector<float>   pho_hcalRechitIsoR3_2_out;
    std::vector<float>   pho_hcalRechitIsoR4_2_out;
    std::vector<float>   pho_hcalRechitIsoR5_2_out;
    std::vector<float>   pho_trackIsoR1PtCut20_2_out;
    std::vector<float>   pho_trackIsoR2PtCut20_2_out;
    std::vector<float>   pho_trackIsoR3PtCut20_2_out;
    std::vector<float>   pho_trackIsoR4PtCut20_2_out;
    std::vector<float>   pho_trackIsoR5PtCut20_2_out;
    std::vector<float>   pho_swissCrx_2_out;
    std::vector<float>   pho_seedTime_2_out;
    std::vector<int>     matched_eleCharge_2_out;
    std::vector<int>     matched_eleIndex_2_out;
    std::vector<float>   matched_elePt_2_out;
    std::vector<float>   matched_eleEta_2_out;
    std::vector<float>   matched_elePhi_2_out;
    std::vector<float>   matched_eleTrkPt_2_out;

    std::vector<float>   diPhoM_out;
    std::vector<float>   diPhoEn_out;
    std::vector<float>   diPhoPt_out;
    std::vector<float>   diPhoEta_out;
    std::vector<float>   diPhoPhi_out;
};

void diphoton::setupDiPhotonTree(TTree *t)
{
    // Set branch addresses and branch pointers
    if (t->GetBranch("nPho"))  t->SetBranchAddress("nPho", &nPho, &b_nPho);

    if (t->GetBranch("phoE_1"))  t->SetBranchAddress("phoE_1", &phoE_1, &b_phoE_1);
    if (t->GetBranch("phoEt_1"))  t->SetBranchAddress("phoEt_1", &phoEt_1, &b_phoEt_1);
    if (t->GetBranch("phoEta_1"))  t->SetBranchAddress("phoEta_1", &phoEta_1, &b_phoEta_1);
    if (t->GetBranch("phoPhi_1"))  t->SetBranchAddress("phoPhi_1", &phoPhi_1, &b_phoPhi_1);
    if (t->GetBranch("phoSCE_1"))  t->SetBranchAddress("phoSCE_1", &phoSCE_1, &b_phoSCE_1);
    if (t->GetBranch("phoSCRawE_1"))  t->SetBranchAddress("phoSCRawE_1", &phoSCRawE_1, &b_phoSCRawE_1);
    if (t->GetBranch("phoESEn_1"))  t->SetBranchAddress("phoESEn_1", &phoESEn_1, &b_phoESEn_1);
    if (t->GetBranch("phoSCEta_1"))  t->SetBranchAddress("phoSCEta_1", &phoSCEta_1, &b_phoSCEta_1);
    if (t->GetBranch("phoSCPhi_1"))  t->SetBranchAddress("phoSCPhi_1", &phoSCPhi_1, &b_phoSCPhi_1);
    if (t->GetBranch("phoSCEtaWidth_1"))  t->SetBranchAddress("phoSCEtaWidth_1", &phoSCEtaWidth_1, &b_phoSCEtaWidth_1);
    if (t->GetBranch("phoSCPhiWidth_1"))  t->SetBranchAddress("phoSCPhiWidth_1", &phoSCPhiWidth_1, &b_phoSCPhiWidth_1);
    if (t->GetBranch("phoSCBrem_1"))  t->SetBranchAddress("phoSCBrem_1", &phoSCBrem_1, &b_phoSCBrem_1);
    if (t->GetBranch("phohasPixelSeed_1"))  t->SetBranchAddress("phohasPixelSeed_1", &phohasPixelSeed_1, &b_phohasPixelSeed_1);
    if (t->GetBranch("phoR9_1"))  t->SetBranchAddress("phoR9_1", &phoR9_1, &b_phoR9_1);
    if (t->GetBranch("phoHoverE_1"))  t->SetBranchAddress("phoHoverE_1", &phoHoverE_1, &b_phoHoverE_1);
    if (t->GetBranch("phoSigmaIEtaIEta_1"))  t->SetBranchAddress("phoSigmaIEtaIEta_1", &phoSigmaIEtaIEta_1, &b_phoSigmaIEtaIEta_1);
    // if (t->GetBranch("phoE1x3_1"))  t->SetBranchAddress("phoE1x3_1", &phoE1x3_1, &b_phoE1x3_1);
    // if (t->GetBranch("phoE2x2_1"))  t->SetBranchAddress("phoE2x2_1", &phoE2x2_1, &b_phoE2x2_1);
    if (t->GetBranch("phoE3x3_1"))  t->SetBranchAddress("phoE3x3_1", &phoE3x3_1, &b_phoE3x3_1);
    // if (t->GetBranch("phoE2x5Max_1"))  t->SetBranchAddress("phoE2x5Max_1", &phoE2x5Max_1, &b_phoE2x5Max_1);
    if (t->GetBranch("phoE1x5_1"))  t->SetBranchAddress("phoE1x5_1", &phoE1x5_1, &b_phoE1x5_1);
    if (t->GetBranch("phoE2x5_1"))  t->SetBranchAddress("phoE2x5_1", &phoE2x5_1, &b_phoE2x5_1);
    if (t->GetBranch("phoE5x5_1"))  t->SetBranchAddress("phoE5x5_1", &phoE5x5_1, &b_phoE5x5_1);
    if (t->GetBranch("phoMaxEnergyXtal_1"))  t->SetBranchAddress("phoMaxEnergyXtal_1", &phoMaxEnergyXtal_1, &b_phoMaxEnergyXtal_1);
    if (t->GetBranch("phoSigmaEtaEta_1"))  t->SetBranchAddress("phoSigmaEtaEta_1", &phoSigmaEtaEta_1, &b_phoSigmaEtaEta_1);
    if (t->GetBranch("phoR1x5_1"))  t->SetBranchAddress("phoR1x5_1", &phoR1x5_1, &b_phoR1x5_1);
    if (t->GetBranch("phoR2x5_1"))  t->SetBranchAddress("phoR2x5_1", &phoR2x5_1, &b_phoR2x5_1);
    if (t->GetBranch("phoESEffSigmaRR_1"))  t->SetBranchAddress("phoESEffSigmaRR_1", &phoESEffSigmaRR_1, &b_phoESEffSigmaRR_1);
    if (t->GetBranch("phoSigmaIEtaIEta_2012_1"))  t->SetBranchAddress("phoSigmaIEtaIEta_2012_1", &phoSigmaIEtaIEta_2012_1, &b_phoSigmaIEtaIEta_2012_1);
    // if (t->GetBranch("phoSigmaIEtaIPhi_2012_1"))  t->SetBranchAddress("phoSigmaIEtaIPhi_2012_1", &phoSigmaIEtaIPhi_2012_1, &b_phoSigmaIEtaIPhi_2012_1);
    // if (t->GetBranch("phoSigmaIPhiIPhi_2012_1"))  t->SetBranchAddress("phoSigmaIPhiIPhi_2012_1", &phoSigmaIPhiIPhi_2012_1, &b_phoSigmaIPhiIPhi_2012_1);
    // if (t->GetBranch("phoE1x3_2012_1"))  t->SetBranchAddress("phoE1x3_2012_1", &phoE1x3_2012_1, &b_phoE1x3_2012_1);
    // if (t->GetBranch("phoE2x2_2012_1"))  t->SetBranchAddress("phoE2x2_2012_1", &phoE2x2_2012_1, &b_phoE2x2_2012_1);
    if (t->GetBranch("phoE3x3_2012_1"))  t->SetBranchAddress("phoE3x3_2012_1", &phoE3x3_2012_1, &b_phoE3x3_2012_1);
    // if (t->GetBranch("phoE2x5Max_2012_1"))  t->SetBranchAddress("phoE2x5Max_2012_1", &phoE2x5Max_2012_1, &b_phoE2x5Max_2012_1);
    // if (t->GetBranch("phoE5x5_2012_1"))  t->SetBranchAddress("phoE5x5_2012_1", &phoE5x5_2012_1, &b_phoE5x5_2012_1);
    if (t->GetBranch("phoBC1E_1"))  t->SetBranchAddress("phoBC1E_1", &phoBC1E_1, &b_phoBC1E_1);
    if (t->GetBranch("phoBC1Eta_1"))  t->SetBranchAddress("phoBC1Eta_1", &phoBC1Eta_1, &b_phoBC1Eta_1);
    if (t->GetBranch("phoBC2E_1"))  t->SetBranchAddress("phoBC2E_1", &phoBC2E_1, &b_phoBC2E_1);
    if (t->GetBranch("phoBC2Eta_1"))  t->SetBranchAddress("phoBC2Eta_1", &phoBC2Eta_1, &b_phoBC2Eta_1);
    if (t->GetBranch("pho_ecalClusterIsoR2_1"))  t->SetBranchAddress("pho_ecalClusterIsoR2_1", &pho_ecalClusterIsoR2_1, &b_pho_ecalClusterIsoR2_1);
    if (t->GetBranch("pho_ecalClusterIsoR3_1"))  t->SetBranchAddress("pho_ecalClusterIsoR3_1", &pho_ecalClusterIsoR3_1, &b_pho_ecalClusterIsoR3_1);
    if (t->GetBranch("pho_ecalClusterIsoR4_1"))  t->SetBranchAddress("pho_ecalClusterIsoR4_1", &pho_ecalClusterIsoR4_1, &b_pho_ecalClusterIsoR4_1);
    if (t->GetBranch("pho_ecalClusterIsoR5_1"))  t->SetBranchAddress("pho_ecalClusterIsoR5_1", &pho_ecalClusterIsoR5_1, &b_pho_ecalClusterIsoR5_1);
    if (t->GetBranch("pho_hcalRechitIsoR1_1"))  t->SetBranchAddress("pho_hcalRechitIsoR1_1", &pho_hcalRechitIsoR1_1, &b_pho_hcalRechitIsoR1_1);
    if (t->GetBranch("pho_hcalRechitIsoR2_1"))  t->SetBranchAddress("pho_hcalRechitIsoR2_1", &pho_hcalRechitIsoR2_1, &b_pho_hcalRechitIsoR2_1);
    if (t->GetBranch("pho_hcalRechitIsoR3_1"))  t->SetBranchAddress("pho_hcalRechitIsoR3_1", &pho_hcalRechitIsoR3_1, &b_pho_hcalRechitIsoR3_1);
    if (t->GetBranch("pho_hcalRechitIsoR4_1"))  t->SetBranchAddress("pho_hcalRechitIsoR4_1", &pho_hcalRechitIsoR4_1, &b_pho_hcalRechitIsoR4_1);
    if (t->GetBranch("pho_hcalRechitIsoR5_1"))  t->SetBranchAddress("pho_hcalRechitIsoR5_1", &pho_hcalRechitIsoR5_1, &b_pho_hcalRechitIsoR5_1);
    if (t->GetBranch("pho_trackIsoR1PtCut20_1"))  t->SetBranchAddress("pho_trackIsoR1PtCut20_1", &pho_trackIsoR1PtCut20_1, &b_pho_trackIsoR1PtCut20_1);
    if (t->GetBranch("pho_trackIsoR2PtCut20_1"))  t->SetBranchAddress("pho_trackIsoR2PtCut20_1", &pho_trackIsoR2PtCut20_1, &b_pho_trackIsoR2PtCut20_1);
    if (t->GetBranch("pho_trackIsoR3PtCut20_1"))  t->SetBranchAddress("pho_trackIsoR3PtCut20_1", &pho_trackIsoR3PtCut20_1, &b_pho_trackIsoR3PtCut20_1);
    if (t->GetBranch("pho_trackIsoR4PtCut20_1"))  t->SetBranchAddress("pho_trackIsoR4PtCut20_1", &pho_trackIsoR4PtCut20_1, &b_pho_trackIsoR4PtCut20_1);
    if (t->GetBranch("pho_trackIsoR5PtCut20_1"))  t->SetBranchAddress("pho_trackIsoR5PtCut20_1", &pho_trackIsoR5PtCut20_1, &b_pho_trackIsoR5PtCut20_1);
    if (t->GetBranch("pho_swissCrx_1"))  t->SetBranchAddress("pho_swissCrx_1", &pho_swissCrx_1, &b_pho_swissCrx_1);
    if (t->GetBranch("pho_seedTime_1"))  t->SetBranchAddress("pho_seedTime_1", &pho_seedTime_1, &b_pho_seedTime_1);
    if (t->GetBranch("matched_eleCharge_1"))  t->SetBranchAddress("matched_eleCharge_1", &matched_eleCharge_1, &b_matched_eleCharge_1);
    if (t->GetBranch("matched_eleIndex_1"))  t->SetBranchAddress("matched_eleIndex_1", &matched_eleIndex_1, &b_matched_eleIndex_1);
    if (t->GetBranch("matched_elePt_1"))  t->SetBranchAddress("matched_elePt_1", &matched_elePt_1, &b_matched_elePt_1);
    if (t->GetBranch("matched_eleEta_1"))  t->SetBranchAddress("matched_eleEta_1", &matched_eleEta_1, &b_matched_eleEta_1);
    if (t->GetBranch("matched_elePhi_1"))  t->SetBranchAddress("matched_elePhi_1", &matched_elePhi_1, &b_matched_elePhi_1);
    if (t->GetBranch("matched_eleTrkPt_1"))  t->SetBranchAddress("matched_eleTrkPt_1", &matched_eleTrkPt_1, &b_matched_eleTrkPt_1);

    if (t->GetBranch("phoE_2"))  t->SetBranchAddress("phoE_2", &phoE_2, &b_phoE_2);
    if (t->GetBranch("phoEt_2"))  t->SetBranchAddress("phoEt_2", &phoEt_2, &b_phoEt_2);
    if (t->GetBranch("phoEta_2"))  t->SetBranchAddress("phoEta_2", &phoEta_2, &b_phoEta_2);
    if (t->GetBranch("phoPhi_2"))  t->SetBranchAddress("phoPhi_2", &phoPhi_2, &b_phoPhi_2);
    if (t->GetBranch("phoSCE_2"))  t->SetBranchAddress("phoSCE_2", &phoSCE_2, &b_phoSCE_2);
    if (t->GetBranch("phoSCRawE_2"))  t->SetBranchAddress("phoSCRawE_2", &phoSCRawE_2, &b_phoSCRawE_2);
    if (t->GetBranch("phoESEn_2"))  t->SetBranchAddress("phoESEn_2", &phoESEn_2, &b_phoESEn_2);
    if (t->GetBranch("phoSCEta_2"))  t->SetBranchAddress("phoSCEta_2", &phoSCEta_2, &b_phoSCEta_2);
    if (t->GetBranch("phoSCPhi_2"))  t->SetBranchAddress("phoSCPhi_2", &phoSCPhi_2, &b_phoSCPhi_2);
    if (t->GetBranch("phoSCEtaWidth_2"))  t->SetBranchAddress("phoSCEtaWidth_2", &phoSCEtaWidth_2, &b_phoSCEtaWidth_2);
    if (t->GetBranch("phoSCPhiWidth_2"))  t->SetBranchAddress("phoSCPhiWidth_2", &phoSCPhiWidth_2, &b_phoSCPhiWidth_2);
    if (t->GetBranch("phoSCBrem_2"))  t->SetBranchAddress("phoSCBrem_2", &phoSCBrem_2, &b_phoSCBrem_2);
    if (t->GetBranch("phohasPixelSeed_2"))  t->SetBranchAddress("phohasPixelSeed_2", &phohasPixelSeed_2, &b_phohasPixelSeed_2);
    if (t->GetBranch("phoR9_2"))  t->SetBranchAddress("phoR9_2", &phoR9_2, &b_phoR9_2);
    if (t->GetBranch("phoHoverE_2"))  t->SetBranchAddress("phoHoverE_2", &phoHoverE_2, &b_phoHoverE_2);
    if (t->GetBranch("phoSigmaIEtaIEta_2"))  t->SetBranchAddress("phoSigmaIEtaIEta_2", &phoSigmaIEtaIEta_2, &b_phoSigmaIEtaIEta_2);
    // if (t->GetBranch("phoE1x3_2"))  t->SetBranchAddress("phoE1x3_2", &phoE1x3_2, &b_phoE1x3_2);
    // if (t->GetBranch("phoE2x2_2"))  t->SetBranchAddress("phoE2x2_2", &phoE2x2_2, &b_phoE2x2_2);
    if (t->GetBranch("phoE3x3_2"))  t->SetBranchAddress("phoE3x3_2", &phoE3x3_2, &b_phoE3x3_2);
    // if (t->GetBranch("phoE2x5Max_2"))  t->SetBranchAddress("phoE2x5Max_2", &phoE2x5Max_2, &b_phoE2x5Max_2);
    if (t->GetBranch("phoE1x5_2"))  t->SetBranchAddress("phoE1x5_2", &phoE1x5_2, &b_phoE1x5_2);
    if (t->GetBranch("phoE2x5_2"))  t->SetBranchAddress("phoE2x5_2", &phoE2x5_2, &b_phoE2x5_2);
    if (t->GetBranch("phoE5x5_2"))  t->SetBranchAddress("phoE5x5_2", &phoE5x5_2, &b_phoE5x5_2);
    if (t->GetBranch("phoMaxEnergyXtal_2"))  t->SetBranchAddress("phoMaxEnergyXtal_2", &phoMaxEnergyXtal_2, &b_phoMaxEnergyXtal_2);
    if (t->GetBranch("phoSigmaEtaEta_2"))  t->SetBranchAddress("phoSigmaEtaEta_2", &phoSigmaEtaEta_2, &b_phoSigmaEtaEta_2);
    if (t->GetBranch("phoR1x5_2"))  t->SetBranchAddress("phoR1x5_2", &phoR1x5_2, &b_phoR1x5_2);
    if (t->GetBranch("phoR2x5_2"))  t->SetBranchAddress("phoR2x5_2", &phoR2x5_2, &b_phoR2x5_2);
    if (t->GetBranch("phoESEffSigmaRR_2"))  t->SetBranchAddress("phoESEffSigmaRR_2", &phoESEffSigmaRR_2, &b_phoESEffSigmaRR_2);
    if (t->GetBranch("phoSigmaIEtaIEta_2012_2"))  t->SetBranchAddress("phoSigmaIEtaIEta_2012_2", &phoSigmaIEtaIEta_2012_2, &b_phoSigmaIEtaIEta_2012_2);
    // if (t->GetBranch("phoSigmaIEtaIPhi_2012_2"))  t->SetBranchAddress("phoSigmaIEtaIPhi_2012_2", &phoSigmaIEtaIPhi_2012_2, &b_phoSigmaIEtaIPhi_2012_2);
    // if (t->GetBranch("phoSigmaIPhiIPhi_2012_2"))  t->SetBranchAddress("phoSigmaIPhiIPhi_2012_2", &phoSigmaIPhiIPhi_2012_2, &b_phoSigmaIPhiIPhi_2012_2);
    // if (t->GetBranch("phoE1x3_2012_2"))  t->SetBranchAddress("phoE1x3_2012_2", &phoE1x3_2012_2, &b_phoE1x3_2012_2);
    // if (t->GetBranch("phoE2x2_2012_2"))  t->SetBranchAddress("phoE2x2_2012_2", &phoE2x2_2012_2, &b_phoE2x2_2012_2);
    if (t->GetBranch("phoE3x3_2012_2"))  t->SetBranchAddress("phoE3x3_2012_2", &phoE3x3_2012_2, &b_phoE3x3_2012_2);
    // if (t->GetBranch("phoE2x5Max_2012_2"))  t->SetBranchAddress("phoE2x5Max_2012_2", &phoE2x5Max_2012_2, &b_phoE2x5Max_2012_2);
    // if (t->GetBranch("phoE5x5_2012_2"))  t->SetBranchAddress("phoE5x5_2012_2", &phoE5x5_2012_2, &b_phoE5x5_2012_2);
    if (t->GetBranch("phoBC1E_2"))  t->SetBranchAddress("phoBC1E_2", &phoBC1E_2, &b_phoBC1E_2);
    if (t->GetBranch("phoBC1Eta_2"))  t->SetBranchAddress("phoBC1Eta_2", &phoBC1Eta_2, &b_phoBC1Eta_2);
    if (t->GetBranch("phoBC2E_2"))  t->SetBranchAddress("phoBC2E_2", &phoBC2E_2, &b_phoBC2E_2);
    if (t->GetBranch("phoBC2Eta_2"))  t->SetBranchAddress("phoBC2Eta_2", &phoBC2Eta_2, &b_phoBC2Eta_2);
    if (t->GetBranch("pho_ecalClusterIsoR2_2"))  t->SetBranchAddress("pho_ecalClusterIsoR2_2", &pho_ecalClusterIsoR2_2, &b_pho_ecalClusterIsoR2_2);
    if (t->GetBranch("pho_ecalClusterIsoR3_2"))  t->SetBranchAddress("pho_ecalClusterIsoR3_2", &pho_ecalClusterIsoR3_2, &b_pho_ecalClusterIsoR3_2);
    if (t->GetBranch("pho_ecalClusterIsoR4_2"))  t->SetBranchAddress("pho_ecalClusterIsoR4_2", &pho_ecalClusterIsoR4_2, &b_pho_ecalClusterIsoR4_2);
    if (t->GetBranch("pho_ecalClusterIsoR5_2"))  t->SetBranchAddress("pho_ecalClusterIsoR5_2", &pho_ecalClusterIsoR5_2, &b_pho_ecalClusterIsoR5_2);
    if (t->GetBranch("pho_hcalRechitIsoR1_2"))  t->SetBranchAddress("pho_hcalRechitIsoR1_2", &pho_hcalRechitIsoR1_2, &b_pho_hcalRechitIsoR1_2);
    if (t->GetBranch("pho_hcalRechitIsoR2_2"))  t->SetBranchAddress("pho_hcalRechitIsoR2_2", &pho_hcalRechitIsoR2_2, &b_pho_hcalRechitIsoR2_2);
    if (t->GetBranch("pho_hcalRechitIsoR3_2"))  t->SetBranchAddress("pho_hcalRechitIsoR3_2", &pho_hcalRechitIsoR3_2, &b_pho_hcalRechitIsoR3_2);
    if (t->GetBranch("pho_hcalRechitIsoR4_2"))  t->SetBranchAddress("pho_hcalRechitIsoR4_2", &pho_hcalRechitIsoR4_2, &b_pho_hcalRechitIsoR4_2);
    if (t->GetBranch("pho_hcalRechitIsoR5_2"))  t->SetBranchAddress("pho_hcalRechitIsoR5_2", &pho_hcalRechitIsoR5_2, &b_pho_hcalRechitIsoR5_2);
    if (t->GetBranch("pho_trackIsoR1PtCut20_2"))  t->SetBranchAddress("pho_trackIsoR1PtCut20_2", &pho_trackIsoR1PtCut20_2, &b_pho_trackIsoR1PtCut20_2);
    if (t->GetBranch("pho_trackIsoR2PtCut20_2"))  t->SetBranchAddress("pho_trackIsoR2PtCut20_2", &pho_trackIsoR2PtCut20_2, &b_pho_trackIsoR2PtCut20_2);
    if (t->GetBranch("pho_trackIsoR3PtCut20_2"))  t->SetBranchAddress("pho_trackIsoR3PtCut20_2", &pho_trackIsoR3PtCut20_2, &b_pho_trackIsoR3PtCut20_2);
    if (t->GetBranch("pho_trackIsoR4PtCut20_2"))  t->SetBranchAddress("pho_trackIsoR4PtCut20_2", &pho_trackIsoR4PtCut20_2, &b_pho_trackIsoR4PtCut20_2);
    if (t->GetBranch("pho_trackIsoR5PtCut20_2"))  t->SetBranchAddress("pho_trackIsoR5PtCut20_2", &pho_trackIsoR5PtCut20_2, &b_pho_trackIsoR5PtCut20_2);
    if (t->GetBranch("pho_swissCrx_2"))  t->SetBranchAddress("pho_swissCrx_2", &pho_swissCrx_2, &b_pho_swissCrx_2);
    if (t->GetBranch("pho_seedTime_2"))  t->SetBranchAddress("pho_seedTime_2", &pho_seedTime_2, &b_pho_seedTime_2);
    if (t->GetBranch("matched_eleCharge_2"))  t->SetBranchAddress("matched_eleCharge_2", &matched_eleCharge_2, &b_matched_eleCharge_2);
    if (t->GetBranch("matched_eleIndex_2"))  t->SetBranchAddress("matched_eleIndex_2", &matched_eleIndex_2, &b_matched_eleIndex_2);
    if (t->GetBranch("matched_elePt_2"))  t->SetBranchAddress("matched_elePt_2", &matched_elePt_2, &b_matched_elePt_2);
    if (t->GetBranch("matched_eleEta_2"))  t->SetBranchAddress("matched_eleEta_2", &matched_eleEta_2, &b_matched_eleEta_2);
    if (t->GetBranch("matched_elePhi_2"))  t->SetBranchAddress("matched_elePhi_2", &matched_elePhi_2, &b_matched_elePhi_2);
    if (t->GetBranch("matched_eleTrkPt_2"))  t->SetBranchAddress("matched_eleTrkPt_2", &matched_eleTrkPt_2, &b_matched_eleTrkPt_2);

    if (t->GetBranch("diPhoM"))  t->SetBranchAddress("diPhoM", &diPhoM, &b_diPhoM);
    if (t->GetBranch("diPhoEn"))  t->SetBranchAddress("diPhoEn", &diPhoEn, &b_diPhoEn);
    if (t->GetBranch("diPhoPt"))  t->SetBranchAddress("diPhoPt", &diPhoPt, &b_diPhoPt);
    if (t->GetBranch("diPhoEta"))  t->SetBranchAddress("diPhoEta", &diPhoEta, &b_diPhoEta);
    if (t->GetBranch("diPhoPhi"))  t->SetBranchAddress("diPhoPhi", &diPhoPhi, &b_diPhoPhi);
}

void diphoton::branchDiPhotonTree(TTree *t)
{
    t->Branch("nPho", &nPho_out);

    t->Branch("phoE_1", &phoE_1_out);
    t->Branch("phoEt_1", &phoEt_1_out);
    t->Branch("phoEta_1", &phoEta_1_out);
    t->Branch("phoPhi_1", &phoPhi_1_out);
    t->Branch("phoSCE_1", &phoSCE_1_out);
    t->Branch("phoSCRawE_1", &phoSCRawE_1_out);
    t->Branch("phoESEn_1", &phoESEn_1_out);
    t->Branch("phoSCEta_1", &phoSCEta_1_out);
    t->Branch("phoSCPhi_1", &phoSCPhi_1_out);
    t->Branch("phoSCEtaWidth_1", &phoSCEtaWidth_1_out);
    t->Branch("phoSCPhiWidth_1", &phoSCPhiWidth_1_out);
    t->Branch("phoSCBrem_1", &phoSCBrem_1_out);
    t->Branch("phohasPixelSeed_1", &phohasPixelSeed_1_out);
    t->Branch("phoR9_1", &phoR9_1_out);
    t->Branch("phoHoverE_1", &phoHoverE_1_out);
    t->Branch("phoSigmaIEtaIEta_1", &phoSigmaIEtaIEta_1_out);
    // t->Branch("phoE1x3_1", &phoE1x3_1_out);
    // t->Branch("phoE2x2_1", &phoE2x2_1_out);
    t->Branch("phoE3x3_1", &phoE3x3_1_out);
    // t->Branch("phoE2x5Max_1", &phoE2x5Max_1_out);
    t->Branch("phoE1x5_1", &phoE1x5_1_out);
    t->Branch("phoE2x5_1", &phoE2x5_1_out);
    t->Branch("phoE5x5_1", &phoE5x5_1_out);
    t->Branch("phoMaxEnergyXtal_1", &phoMaxEnergyXtal_1_out);
    t->Branch("phoSigmaEtaEta_1", &phoSigmaEtaEta_1_out);
    t->Branch("phoR1x5_1", &phoR1x5_1_out);
    t->Branch("phoR2x5_1", &phoR2x5_1_out);
    t->Branch("phoESEffSigmaRR_1", &phoESEffSigmaRR_1_out);
    t->Branch("phoSigmaIEtaIEta_2012_1", &phoSigmaIEtaIEta_2012_1_out);
    // t->Branch("phoSigmaIEtaIPhi_2012_1", &phoSigmaIEtaIPhi_2012_1_out);
    // t->Branch("phoSigmaIPhiIPhi_2012_1", &phoSigmaIPhiIPhi_2012_1_out);
    // t->Branch("phoE1x3_2012_1", &phoE1x3_2012_1_out);
    // t->Branch("phoE2x2_2012_1", &phoE2x2_2012_1_out);
    t->Branch("phoE3x3_2012_1", &phoE3x3_2012_1_out);
    // t->Branch("phoE2x5Max_2012_1", &phoE2x5Max_2012_1_out);
    // t->Branch("phoE5x5_2012_1", &phoE5x5_2012_1_out);
    t->Branch("phoBC1E_1", &phoBC1E_1_out);
    t->Branch("phoBC1Eta_1", &phoBC1Eta_1_out);
    t->Branch("phoBC2E_1", &phoBC2E_1_out);
    t->Branch("phoBC2Eta_1", &phoBC2Eta_1_out);
    t->Branch("pho_ecalClusterIsoR2_1", &pho_ecalClusterIsoR2_1_out);
    t->Branch("pho_ecalClusterIsoR3_1", &pho_ecalClusterIsoR3_1_out);
    t->Branch("pho_ecalClusterIsoR4_1", &pho_ecalClusterIsoR4_1_out);
    t->Branch("pho_ecalClusterIsoR5_1", &pho_ecalClusterIsoR5_1_out);
    t->Branch("pho_hcalRechitIsoR1_1", &pho_hcalRechitIsoR1_1_out);
    t->Branch("pho_hcalRechitIsoR2_1", &pho_hcalRechitIsoR2_1_out);
    t->Branch("pho_hcalRechitIsoR3_1", &pho_hcalRechitIsoR3_1_out);
    t->Branch("pho_hcalRechitIsoR4_1", &pho_hcalRechitIsoR4_1_out);
    t->Branch("pho_hcalRechitIsoR5_1", &pho_hcalRechitIsoR5_1_out);
    t->Branch("pho_trackIsoR1PtCut20_1", &pho_trackIsoR1PtCut20_1_out);
    t->Branch("pho_trackIsoR2PtCut20_1", &pho_trackIsoR2PtCut20_1_out);
    t->Branch("pho_trackIsoR3PtCut20_1", &pho_trackIsoR3PtCut20_1_out);
    t->Branch("pho_trackIsoR4PtCut20_1", &pho_trackIsoR4PtCut20_1_out);
    t->Branch("pho_trackIsoR5PtCut20_1", &pho_trackIsoR5PtCut20_1_out);
    t->Branch("pho_swissCrx_1", &pho_swissCrx_1_out);
    t->Branch("pho_seedTime_1", &pho_seedTime_1_out);
    t->Branch("matched_eleCharge_1", &matched_eleCharge_1_out);
    t->Branch("matched_eleIndex_1", &matched_eleIndex_1_out);
    t->Branch("matched_elePt_1", &matched_elePt_1_out);
    t->Branch("matched_eleEta_1", &matched_eleEta_1_out);
    t->Branch("matched_eleTrkPt_1", &matched_eleTrkPt_1_out);

    t->Branch("phoE_2", &phoE_2_out);
    t->Branch("phoEt_2", &phoEt_2_out);
    t->Branch("phoEta_2", &phoEta_2_out);
    t->Branch("phoPhi_2", &phoPhi_2_out);
    t->Branch("phoSCE_2", &phoSCE_2_out);
    t->Branch("phoSCRawE_2", &phoSCRawE_2_out);
    t->Branch("phoESEn_2", &phoESEn_2_out);
    t->Branch("phoSCEta_2", &phoSCEta_2_out);
    t->Branch("phoSCPhi_2", &phoSCPhi_2_out);
    t->Branch("phoSCEtaWidth_2", &phoSCEtaWidth_2_out);
    t->Branch("phoSCPhiWidth_2", &phoSCPhiWidth_2_out);
    t->Branch("phoSCBrem_2", &phoSCBrem_2_out);
    t->Branch("phohasPixelSeed_2", &phohasPixelSeed_2_out);
    t->Branch("phoR9_2", &phoR9_2_out);
    t->Branch("phoHoverE_2", &phoHoverE_2_out);
    t->Branch("phoSigmaIEtaIEta_2", &phoSigmaIEtaIEta_2_out);
    // t->Branch("phoE1x3_2", &phoE1x3_2_out);
    // t->Branch("phoE2x2_2", &phoE2x2_2_out);
    t->Branch("phoE3x3_2", &phoE3x3_2_out);
    // t->Branch("phoE2x5Max_2", &phoE2x5Max_2_out);
    t->Branch("phoE1x5_2", &phoE1x5_2_out);
    t->Branch("phoE2x5_2", &phoE2x5_2_out);
    t->Branch("phoE5x5_2", &phoE5x5_2_out);
    t->Branch("phoMaxEnergyXtal_2", &phoMaxEnergyXtal_2_out);
    t->Branch("phoSigmaEtaEta_2", &phoSigmaEtaEta_2_out);
    t->Branch("phoR1x5_2", &phoR1x5_2_out);
    t->Branch("phoR2x5_2", &phoR2x5_2_out);
    t->Branch("phoESEffSigmaRR_2", &phoESEffSigmaRR_2_out);
    t->Branch("phoSigmaIEtaIEta_2012_2", &phoSigmaIEtaIEta_2012_2_out);
    // t->Branch("phoSigmaIEtaIPhi_2012_2", &phoSigmaIEtaIPhi_2012_2_out);
    // t->Branch("phoSigmaIPhiIPhi_2012_2", &phoSigmaIPhiIPhi_2012_2_out);
    // t->Branch("phoE1x3_2012_2", &phoE1x3_2012_2_out);
    // t->Branch("phoE2x2_2012_2", &phoE2x2_2012_2_out);
    t->Branch("phoE3x3_2012_2", &phoE3x3_2012_2_out);
    // t->Branch("phoE2x5Max_2012_2", &phoE2x5Max_2012_2_out);
    // t->Branch("phoE5x5_2012_2", &phoE5x5_2012_2_out);
    t->Branch("phoBC1E_2", &phoBC1E_2_out);
    t->Branch("phoBC1Eta_2", &phoBC1Eta_2_out);
    t->Branch("phoBC2E_2", &phoBC2E_2_out);
    t->Branch("phoBC2Eta_2", &phoBC2Eta_2_out);
    t->Branch("pho_ecalClusterIsoR2_2", &pho_ecalClusterIsoR2_2_out);
    t->Branch("pho_ecalClusterIsoR3_2", &pho_ecalClusterIsoR3_2_out);
    t->Branch("pho_ecalClusterIsoR4_2", &pho_ecalClusterIsoR4_2_out);
    t->Branch("pho_ecalClusterIsoR5_2", &pho_ecalClusterIsoR5_2_out);
    t->Branch("pho_hcalRechitIsoR1_2", &pho_hcalRechitIsoR1_2_out);
    t->Branch("pho_hcalRechitIsoR2_2", &pho_hcalRechitIsoR2_2_out);
    t->Branch("pho_hcalRechitIsoR3_2", &pho_hcalRechitIsoR3_2_out);
    t->Branch("pho_hcalRechitIsoR4_2", &pho_hcalRechitIsoR4_2_out);
    t->Branch("pho_hcalRechitIsoR5_2", &pho_hcalRechitIsoR5_2_out);
    t->Branch("pho_trackIsoR1PtCut20_2", &pho_trackIsoR1PtCut20_2_out);
    t->Branch("pho_trackIsoR2PtCut20_2", &pho_trackIsoR2PtCut20_2_out);
    t->Branch("pho_trackIsoR3PtCut20_2", &pho_trackIsoR3PtCut20_2_out);
    t->Branch("pho_trackIsoR4PtCut20_2", &pho_trackIsoR4PtCut20_2_out);
    t->Branch("pho_trackIsoR5PtCut20_2", &pho_trackIsoR5PtCut20_2_out);
    t->Branch("pho_swissCrx_2", &pho_swissCrx_2_out);
    t->Branch("pho_seedTime_2", &pho_seedTime_2_out);
    t->Branch("matched_eleCharge_2", &matched_eleCharge_2_out);
    t->Branch("matched_eleIndex_2", &matched_eleIndex_2_out);
    t->Branch("matched_elePt_2", &matched_elePt_2_out);
    t->Branch("matched_eleEta_2", &matched_eleEta_2_out);
    t->Branch("matched_eleTrkPt_2", &matched_eleTrkPt_2_out);

    t->Branch("diPhoM", &diPhoM_out);
    t->Branch("diPhoEn", &diPhoEn_out);
    t->Branch("diPhoPt", &diPhoPt_out);
    t->Branch("diPhoEta", &diPhoEta_out);
    t->Branch("diPhoPhi", &diPhoPhi_out);
}

void diphoton::clearDiPhotonPairs()
{
    phoE_1_out.clear();
    phoEt_1_out.clear();
    phoEta_1_out.clear();
    phoPhi_1_out.clear();
    phoSCE_1_out.clear();
    phoSCRawE_1_out.clear();
    phoESEn_1_out.clear();
    phoSCEta_1_out.clear();
    phoSCPhi_1_out.clear();
    phoSCEtaWidth_1_out.clear();
    phoSCPhiWidth_1_out.clear();
    phoSCBrem_1_out.clear();
    phohasPixelSeed_1_out.clear();
    phoR9_1_out.clear();
    phoHoverE_1_out.clear();
    phoSigmaIEtaIEta_1_out.clear();
    // phoE1x3_1_out.clear();
    // phoE2x2_1_out.clear();
    phoE3x3_1_out.clear();
    // phoE2x5Max_1_out.clear();
    phoE1x5_1_out.clear();
    phoE2x5_1_out.clear();
    phoE5x5_1_out.clear();
    phoMaxEnergyXtal_1_out.clear();
    phoSigmaEtaEta_1_out.clear();
    phoR1x5_1_out.clear();
    phoR2x5_1_out.clear();
    phoESEffSigmaRR_1_out.clear();
    phoSigmaIEtaIEta_2012_1_out.clear();
    // phoSigmaIEtaIPhi_2012_1_out.clear();
    // phoSigmaIPhiIPhi_2012_1_out.clear();
    // phoE1x3_2012_1_out.clear();
    // phoE2x2_2012_1_out.clear();
    phoE3x3_2012_1_out.clear();
    // phoE2x5Max_2012_1_out.clear();
    // phoE5x5_2012_1_out.clear();
    phoBC1E_1_out.clear();
    phoBC1Eta_1_out.clear();
    phoBC2E_1_out.clear();
    phoBC2Eta_1_out.clear();
    pho_ecalClusterIsoR2_1_out.clear();
    pho_ecalClusterIsoR3_1_out.clear();
    pho_ecalClusterIsoR4_1_out.clear();
    pho_ecalClusterIsoR5_1_out.clear();
    pho_hcalRechitIsoR1_1_out.clear();
    pho_hcalRechitIsoR2_1_out.clear();
    pho_hcalRechitIsoR3_1_out.clear();
    pho_hcalRechitIsoR4_1_out.clear();
    pho_hcalRechitIsoR5_1_out.clear();
    pho_trackIsoR1PtCut20_1_out.clear();
    pho_trackIsoR2PtCut20_1_out.clear();
    pho_trackIsoR3PtCut20_1_out.clear();
    pho_trackIsoR4PtCut20_1_out.clear();
    pho_trackIsoR5PtCut20_1_out.clear();
    pho_swissCrx_1_out.clear();
    pho_seedTime_1_out.clear();
    matched_eleCharge_1_out.clear();
    matched_eleIndex_1_out.clear();
    matched_elePt_1_out.clear();
    matched_eleEta_1_out.clear();
    matched_elePhi_1_out.clear();
    matched_eleTrkPt_1_out.clear();

    phoE_2_out.clear();
    phoEt_2_out.clear();
    phoEta_2_out.clear();
    phoPhi_2_out.clear();
    phoSCE_2_out.clear();
    phoSCRawE_2_out.clear();
    phoESEn_2_out.clear();
    phoSCEta_2_out.clear();
    phoSCPhi_2_out.clear();
    phoSCEtaWidth_2_out.clear();
    phoSCPhiWidth_2_out.clear();
    phoSCBrem_2_out.clear();
    phohasPixelSeed_2_out.clear();
    phoR9_2_out.clear();
    phoHoverE_2_out.clear();
    phoSigmaIEtaIEta_2_out.clear();
    // phoE1x3_2_out.clear();
    // phoE2x2_2_out.clear();
    phoE3x3_2_out.clear();
    // phoE2x5Max_2_out.clear();
    phoE1x5_2_out.clear();
    phoE2x5_2_out.clear();
    phoE5x5_2_out.clear();
    phoMaxEnergyXtal_2_out.clear();
    phoSigmaEtaEta_2_out.clear();
    phoR1x5_2_out.clear();
    phoR2x5_2_out.clear();
    phoESEffSigmaRR_2_out.clear();
    phoSigmaIEtaIEta_2012_2_out.clear();
    // phoSigmaIEtaIPhi_2012_2_out.clear();
    // phoSigmaIPhiIPhi_2012_2_out.clear();
    // phoE1x3_2012_2_out.clear();
    // phoE2x2_2012_2_out.clear();
    phoE3x3_2012_2_out.clear();
    // phoE2x5Max_2012_2_out.clear();
    // phoE5x5_2012_2_out.clear();
    phoBC1E_2_out.clear();
    phoBC1Eta_2_out.clear();
    phoBC2E_2_out.clear();
    phoBC2Eta_2_out.clear();
    pho_ecalClusterIsoR2_2_out.clear();
    pho_ecalClusterIsoR3_2_out.clear();
    pho_ecalClusterIsoR4_2_out.clear();
    pho_ecalClusterIsoR5_2_out.clear();
    pho_hcalRechitIsoR1_2_out.clear();
    pho_hcalRechitIsoR2_2_out.clear();
    pho_hcalRechitIsoR3_2_out.clear();
    pho_hcalRechitIsoR4_2_out.clear();
    pho_hcalRechitIsoR5_2_out.clear();
    pho_trackIsoR1PtCut20_2_out.clear();
    pho_trackIsoR2PtCut20_2_out.clear();
    pho_trackIsoR3PtCut20_2_out.clear();
    pho_trackIsoR4PtCut20_2_out.clear();
    pho_trackIsoR5PtCut20_2_out.clear();
    pho_swissCrx_2_out.clear();
    pho_seedTime_2_out.clear();
    matched_eleCharge_2_out.clear();
    matched_eleIndex_2_out.clear();
    matched_elePt_2_out.clear();
    matched_eleEta_2_out.clear();
    matched_elePhi_2_out.clear();
    matched_eleTrkPt_2_out.clear();

    diPhoM_out.clear();
    diPhoEn_out.clear();
    diPhoPt_out.clear();
    diPhoEta_out.clear();
    diPhoPhi_out.clear();
}

void diphoton::pushbackDiPhotonPair(ggHiNtuplizer &tggHiNtuplizer, int i, int j, bool doSizeCheck)
{
    if (doSizeCheck) {

        if(tggHiNtuplizer.phoE->size() == (unsigned)nPho_out)  phoE_1_out.push_back(tggHiNtuplizer.phoE->at(i));
        if(tggHiNtuplizer.phoEt->size() == (unsigned)nPho_out)  phoEt_1_out.push_back(tggHiNtuplizer.phoEt->at(i));
        if(tggHiNtuplizer.phoEta->size() == (unsigned)nPho_out)  phoEta_1_out.push_back(tggHiNtuplizer.phoEta->at(i));
        if(tggHiNtuplizer.phoPhi->size() == (unsigned)nPho_out)  phoPhi_1_out.push_back(tggHiNtuplizer.phoPhi->at(i));
        if(tggHiNtuplizer.phoSCE->size() == (unsigned)nPho_out)  phoSCE_1_out.push_back(tggHiNtuplizer.phoSCE->at(i));
        if(tggHiNtuplizer.phoSCRawE->size() == (unsigned)nPho_out)  phoSCRawE_1_out.push_back(tggHiNtuplizer.phoSCRawE->at(i));
        if(tggHiNtuplizer.phoESEn->size() == (unsigned)nPho_out)  phoESEn_1_out.push_back(tggHiNtuplizer.phoESEn->at(i));
        if(tggHiNtuplizer.phoSCEta->size() == (unsigned)nPho_out)  phoSCEta_1_out.push_back(tggHiNtuplizer.phoSCEta->at(i));
        if(tggHiNtuplizer.phoSCPhi->size() == (unsigned)nPho_out)  phoSCPhi_1_out.push_back(tggHiNtuplizer.phoSCPhi->at(i));
        if(tggHiNtuplizer.phoSCEtaWidth->size() == (unsigned)nPho_out)  phoSCEtaWidth_1_out.push_back(tggHiNtuplizer.phoSCEtaWidth->at(i));
        if(tggHiNtuplizer.phoSCPhiWidth->size() == (unsigned)nPho_out)  phoSCPhiWidth_1_out.push_back(tggHiNtuplizer.phoSCPhiWidth->at(i));
        if(tggHiNtuplizer.phoSCBrem->size() == (unsigned)nPho_out)  phoSCBrem_1_out.push_back(tggHiNtuplizer.phoSCBrem->at(i));
        if(tggHiNtuplizer.phoHasPixelSeed->size() == (unsigned)nPho_out)  phohasPixelSeed_1_out.push_back(tggHiNtuplizer.phoHasPixelSeed->at(i));
        if(tggHiNtuplizer.phoR9->size() == (unsigned)nPho_out)  phoR9_1_out.push_back(tggHiNtuplizer.phoR9->at(i));
        if(tggHiNtuplizer.phoHoverE->size() == (unsigned)nPho_out)  phoHoverE_1_out.push_back(tggHiNtuplizer.phoHoverE->at(i));
        if(tggHiNtuplizer.phoSigmaIEtaIEta->size() == (unsigned)nPho_out)  phoSigmaIEtaIEta_1_out.push_back(tggHiNtuplizer.phoSigmaIEtaIEta->at(i));
        // if(tggHiNtuplizer.phoE1x3->size() == (unsigned)nPho_out)  phoE1x3_1_out.push_back(tggHiNtuplizer.phoE1x3->at(i));
        // if(tggHiNtuplizer.phoE2x2->size() == (unsigned)nPho_out)  phoE2x2_1_out.push_back(tggHiNtuplizer.phoE2x2->at(i));
//                if(tggHiNtuplizer.phoE3x3->size() == (unsigned)nPho_out)  phoE3x3_1_out.push_back(tggHiNtuplizer.phoE3x3->at(i));
        // if(tggHiNtuplizer.phoE2x5Max->size() == (unsigned)nPho_out)  phoE2x5Max_1_out.push_back(tggHiNtuplizer.phoE2x5Max->at(i));
//                if(tggHiNtuplizer.phoE1x5->size() == (unsigned)nPho_out)  phoE1x5_1_out.push_back(tggHiNtuplizer.phoE1x5->at(i));
//                if(tggHiNtuplizer.phoE2x5->size() == (unsigned)nPho_out)  phoE2x5_1_out.push_back(tggHiNtuplizer.phoE2x5->at(i));
        if(tggHiNtuplizer.phoE5x5->size() == (unsigned)nPho_out)  phoE5x5_1_out.push_back(tggHiNtuplizer.phoE5x5->at(i));
//                if(tggHiNtuplizer.phoMaxEnergyXtal->size() == (unsigned)nPho_out)  phoMaxEnergyXtal_1_out.push_back(tggHiNtuplizer.phoMaxEnergyXtal->at(i));
//                if(tggHiNtuplizer.phoSigmaEtaEta->size() == (unsigned)nPho_out)  phoSigmaEtaEta_1_out.push_back(tggHiNtuplizer.phoSigmaEtaEta->at(i));
//                if(tggHiNtuplizer.phoR1x5->size() == (unsigned)nPho_out)  phoR1x5_1_out.push_back(tggHiNtuplizer.phoR1x5->at(i));
//                if(tggHiNtuplizer.phoR2x5->size() == (unsigned)nPho_out)  phoR2x5_1_out.push_back(tggHiNtuplizer.phoR2x5->at(i));
        if(tggHiNtuplizer.phoESEffSigmaRR->size() == (unsigned)nPho_out)  phoESEffSigmaRR_1_out.push_back(tggHiNtuplizer.phoESEffSigmaRR->at(i));
        if(tggHiNtuplizer.phoSigmaIEtaIEta_2012->size() == (unsigned)nPho_out)  phoSigmaIEtaIEta_2012_1_out.push_back(tggHiNtuplizer.phoSigmaIEtaIEta_2012->at(i));
        // if(tggHiNtuplizer.phoSigmaIEtaIPhi_2012->size() == (unsigned)nPho_out)  phoSigmaIEtaIPhi_2012_1_out.push_back(tggHiNtuplizer.phoSigmaIEtaIPhi_2012->at(i));
        // if(tggHiNtuplizer.phoSigmaIPhiIPhi_2012->size() == (unsigned)nPho_out)  phoSigmaIPhiIPhi_2012_1_out.push_back(tggHiNtuplizer.phoSigmaIPhiIPhi_2012->at(i));
        // if(tggHiNtuplizer.phoE1x3_2012->size() == (unsigned)nPho_out)  phoE1x3_2012_1_out.push_back(tggHiNtuplizer.phoE1x3_2012->at(i));
        // if(tggHiNtuplizer.phoE2x2_2012->size() == (unsigned)nPho_out)  phoE2x2_2012_1_out.push_back(tggHiNtuplizer.phoE2x2_2012->at(i));
//                if(tggHiNtuplizer.phoE3x3_2012->size() == (unsigned)nPho_out)  phoE3x3_2012_1_out.push_back(tggHiNtuplizer.phoE3x3_2012->at(i));
        // if(tggHiNtuplizer.phoE2x5Max_2012->size() == (unsigned)nPho_out)  phoE2x5Max_2012_1_out.push_back(tggHiNtuplizer.phoE2x5Max_2012->at(i));
        // if(tggHiNtuplizer.phoE5x5_2012->size() == (unsigned)nPho_out)  phoE5x5_2012_1_out.push_back(tggHiNtuplizer.phoE5x5_2012->at(i));
        if(tggHiNtuplizer.phoBC1E->size() == (unsigned)nPho_out)  phoBC1E_1_out.push_back(tggHiNtuplizer.phoBC1E->at(i));
        if(tggHiNtuplizer.phoBC1Eta->size() == (unsigned)nPho_out)  phoBC1Eta_1_out.push_back(tggHiNtuplizer.phoBC1Eta->at(i));
        if(tggHiNtuplizer.phoBC2E->size() == (unsigned)nPho_out)  phoBC2E_1_out.push_back(tggHiNtuplizer.phoBC2E->at(i));
        if(tggHiNtuplizer.phoBC2Eta->size() == (unsigned)nPho_out)  phoBC2Eta_1_out.push_back(tggHiNtuplizer.phoBC2Eta->at(i));
        if(tggHiNtuplizer.pho_ecalClusterIsoR2->size() == (unsigned)nPho_out)  pho_ecalClusterIsoR2_1_out.push_back(tggHiNtuplizer.pho_ecalClusterIsoR2->at(i));
        if(tggHiNtuplizer.pho_ecalClusterIsoR3->size() == (unsigned)nPho_out)  pho_ecalClusterIsoR3_1_out.push_back(tggHiNtuplizer.pho_ecalClusterIsoR3->at(i));
        if(tggHiNtuplizer.pho_ecalClusterIsoR4->size() == (unsigned)nPho_out)  pho_ecalClusterIsoR4_1_out.push_back(tggHiNtuplizer.pho_ecalClusterIsoR4->at(i));
        if(tggHiNtuplizer.pho_ecalClusterIsoR5->size() == (unsigned)nPho_out)  pho_ecalClusterIsoR5_1_out.push_back(tggHiNtuplizer.pho_ecalClusterIsoR5->at(i));
        if(tggHiNtuplizer.pho_hcalRechitIsoR1->size() == (unsigned)nPho_out)  pho_hcalRechitIsoR1_1_out.push_back(tggHiNtuplizer.pho_hcalRechitIsoR1->at(i));
        if(tggHiNtuplizer.pho_hcalRechitIsoR2->size() == (unsigned)nPho_out)  pho_hcalRechitIsoR2_1_out.push_back(tggHiNtuplizer.pho_hcalRechitIsoR2->at(i));
        if(tggHiNtuplizer.pho_hcalRechitIsoR3->size() == (unsigned)nPho_out)  pho_hcalRechitIsoR3_1_out.push_back(tggHiNtuplizer.pho_hcalRechitIsoR3->at(i));
        if(tggHiNtuplizer.pho_hcalRechitIsoR4->size() == (unsigned)nPho_out)  pho_hcalRechitIsoR4_1_out.push_back(tggHiNtuplizer.pho_hcalRechitIsoR4->at(i));
        if(tggHiNtuplizer.pho_hcalRechitIsoR5->size() == (unsigned)nPho_out)  pho_hcalRechitIsoR5_1_out.push_back(tggHiNtuplizer.pho_hcalRechitIsoR5->at(i));
        if(tggHiNtuplizer.pho_trackIsoR1PtCut20->size() == (unsigned)nPho_out)  pho_trackIsoR1PtCut20_1_out.push_back(tggHiNtuplizer.pho_trackIsoR1PtCut20->at(i));
        if(tggHiNtuplizer.pho_trackIsoR2PtCut20->size() == (unsigned)nPho_out)  pho_trackIsoR2PtCut20_1_out.push_back(tggHiNtuplizer.pho_trackIsoR2PtCut20->at(i));
        if(tggHiNtuplizer.pho_trackIsoR3PtCut20->size() == (unsigned)nPho_out)  pho_trackIsoR3PtCut20_1_out.push_back(tggHiNtuplizer.pho_trackIsoR3PtCut20->at(i));
        if(tggHiNtuplizer.pho_trackIsoR4PtCut20->size() == (unsigned)nPho_out)  pho_trackIsoR4PtCut20_1_out.push_back(tggHiNtuplizer.pho_trackIsoR4PtCut20->at(i));
        if(tggHiNtuplizer.pho_trackIsoR5PtCut20->size() == (unsigned)nPho_out)  pho_trackIsoR5PtCut20_1_out.push_back(tggHiNtuplizer.pho_trackIsoR5PtCut20->at(i));
        if(tggHiNtuplizer.pho_swissCrx->size() == (unsigned)nPho_out)  pho_swissCrx_1_out.push_back(tggHiNtuplizer.pho_swissCrx->at(i));
        if(tggHiNtuplizer.pho_seedTime->size() == (unsigned)nPho_out)  pho_seedTime_1_out.push_back(tggHiNtuplizer.pho_seedTime->at(i));

        if(tggHiNtuplizer.phoE->size() == (unsigned)nPho_out)  phoE_2_out.push_back(tggHiNtuplizer.phoE->at(j));
        if(tggHiNtuplizer.phoEt->size() == (unsigned)nPho_out)  phoEt_2_out.push_back(tggHiNtuplizer.phoEt->at(j));
        if(tggHiNtuplizer.phoEta->size() == (unsigned)nPho_out)  phoEta_2_out.push_back(tggHiNtuplizer.phoEta->at(j));
        if(tggHiNtuplizer.phoPhi->size() == (unsigned)nPho_out)  phoPhi_2_out.push_back(tggHiNtuplizer.phoPhi->at(j));
        if(tggHiNtuplizer.phoSCE->size() == (unsigned)nPho_out)  phoSCE_2_out.push_back(tggHiNtuplizer.phoSCE->at(j));
        if(tggHiNtuplizer.phoSCRawE->size() == (unsigned)nPho_out)  phoSCRawE_2_out.push_back(tggHiNtuplizer.phoSCRawE->at(j));
        if(tggHiNtuplizer.phoESEn->size() == (unsigned)nPho_out)  phoESEn_2_out.push_back(tggHiNtuplizer.phoESEn->at(j));
        if(tggHiNtuplizer.phoSCEta->size() == (unsigned)nPho_out)  phoSCEta_2_out.push_back(tggHiNtuplizer.phoSCEta->at(j));
        if(tggHiNtuplizer.phoSCPhi->size() == (unsigned)nPho_out)  phoSCPhi_2_out.push_back(tggHiNtuplizer.phoSCPhi->at(j));
        if(tggHiNtuplizer.phoSCEtaWidth->size() == (unsigned)nPho_out)  phoSCEtaWidth_2_out.push_back(tggHiNtuplizer.phoSCEtaWidth->at(j));
        if(tggHiNtuplizer.phoSCPhiWidth->size() == (unsigned)nPho_out)  phoSCPhiWidth_2_out.push_back(tggHiNtuplizer.phoSCPhiWidth->at(j));
        if(tggHiNtuplizer.phoSCBrem->size() == (unsigned)nPho_out)  phoSCBrem_2_out.push_back(tggHiNtuplizer.phoSCBrem->at(j));
        if(tggHiNtuplizer.phoHasPixelSeed->size() == (unsigned)nPho_out)  phohasPixelSeed_2_out.push_back(tggHiNtuplizer.phoHasPixelSeed->at(j));
        if(tggHiNtuplizer.phoR9->size() == (unsigned)nPho_out)  phoR9_2_out.push_back(tggHiNtuplizer.phoR9->at(j));
        if(tggHiNtuplizer.phoHoverE->size() == (unsigned)nPho_out)  phoHoverE_2_out.push_back(tggHiNtuplizer.phoHoverE->at(j));
        if(tggHiNtuplizer.phoSigmaIEtaIEta->size() == (unsigned)nPho_out)  phoSigmaIEtaIEta_2_out.push_back(tggHiNtuplizer.phoSigmaIEtaIEta->at(j));
        // if(tggHiNtuplizer.phoE1x3->size() == (unsigned)nPho_out)  phoE1x3_2_out.push_back(tggHiNtuplizer.phoE1x3->at(j));
        // if(tggHiNtuplizer.phoE2x2->size() == (unsigned)nPho_out)  phoE2x2_2_out.push_back(tggHiNtuplizer.phoE2x2->at(j));
//                if(tggHiNtuplizer.phoE3x3->size() == (unsigned)nPho_out)  phoE3x3_2_out.push_back(tggHiNtuplizer.phoE3x3->at(j));
        // if(tggHiNtuplizer.phoE2x5Max->size() == (unsigned)nPho_out)  phoE2x5Max_2_out.push_back(tggHiNtuplizer.phoE2x5Max->at(j));
//                if(tggHiNtuplizer.phoE1x5->size() == (unsigned)nPho_out)  phoE1x5_2_out.push_back(tggHiNtuplizer.phoE1x5->at(j));
//                if(tggHiNtuplizer.phoE2x5->size() == (unsigned)nPho_out)  phoE2x5_2_out.push_back(tggHiNtuplizer.phoE2x5->at(j));
        if(tggHiNtuplizer.phoE5x5->size() == (unsigned)nPho_out)  phoE5x5_2_out.push_back(tggHiNtuplizer.phoE5x5->at(j));
//                if(tggHiNtuplizer.phoMaxEnergyXtal->size() == (unsigned)nPho_out)  phoMaxEnergyXtal_2_out.push_back(tggHiNtuplizer.phoMaxEnergyXtal->at(j));
//                if(tggHiNtuplizer.phoSigmaEtaEta->size() == (unsigned)nPho_out)  phoSigmaEtaEta_2_out.push_back(tggHiNtuplizer.phoSigmaEtaEta->at(j));
//                if(tggHiNtuplizer.phoR1x5->size() == (unsigned)nPho_out)  phoR1x5_2_out.push_back(tggHiNtuplizer.phoR1x5->at(j));
//                if(tggHiNtuplizer.phoR2x5->size() == (unsigned)nPho_out)  phoR2x5_2_out.push_back(tggHiNtuplizer.phoR2x5->at(j));
        if(tggHiNtuplizer.phoESEffSigmaRR->size() == (unsigned)nPho_out)  phoESEffSigmaRR_2_out.push_back(tggHiNtuplizer.phoESEffSigmaRR->at(j));
        if(tggHiNtuplizer.phoSigmaIEtaIEta_2012->size() == (unsigned)nPho_out)  phoSigmaIEtaIEta_2012_2_out.push_back(tggHiNtuplizer.phoSigmaIEtaIEta_2012->at(j));
        // if(tggHiNtuplizer.phoSigmaIEtaIPhi_2012->size() == (unsigned)nPho_out)  phoSigmaIEtaIPhi_2012_2_out.push_back(tggHiNtuplizer.phoSigmaIEtaIPhi_2012->at(j));
        // if(tggHiNtuplizer.phoSigmaIPhiIPhi_2012->size() == (unsigned)nPho_out)  phoSigmaIPhiIPhi_2012_2_out.push_back(tggHiNtuplizer.phoSigmaIPhiIPhi_2012->at(j));
        // if(tggHiNtuplizer.phoE1x3_2012->size() == (unsigned)nPho_out)  phoE1x3_2012_2_out.push_back(tggHiNtuplizer.phoE1x3_2012->at(j));
        // if(tggHiNtuplizer.phoE2x2_2012->size() == (unsigned)nPho_out)  phoE2x2_2012_2_out.push_back(tggHiNtuplizer.phoE2x2_2012->at(j));
//                if(tggHiNtuplizer.phoE3x3_2012->size() == (unsigned)nPho_out)  phoE3x3_2012_2_out.push_back(tggHiNtuplizer.phoE3x3_2012->at(j));
        // if(tggHiNtuplizer.phoE2x5Max_2012->size() == (unsigned)nPho_out)  phoE2x5Max_2012_2_out.push_back(tggHiNtuplizer.phoE2x5Max_2012->at(j));
        // if(tggHiNtuplizer.phoE5x5_2012->size() == (unsigned)nPho_out)  phoE5x5_2012_2_out.push_back(tggHiNtuplizer.phoE5x5_2012->at(j));
        if(tggHiNtuplizer.phoBC1E->size() == (unsigned)nPho_out)  phoBC1E_2_out.push_back(tggHiNtuplizer.phoBC1E->at(j));
        if(tggHiNtuplizer.phoBC1Eta->size() == (unsigned)nPho_out)  phoBC1Eta_2_out.push_back(tggHiNtuplizer.phoBC1Eta->at(j));
        if(tggHiNtuplizer.phoBC2E->size() == (unsigned)nPho_out)  phoBC2E_2_out.push_back(tggHiNtuplizer.phoBC2E->at(j));
        if(tggHiNtuplizer.phoBC2Eta->size() == (unsigned)nPho_out)  phoBC2Eta_2_out.push_back(tggHiNtuplizer.phoBC2Eta->at(j));
        if(tggHiNtuplizer.pho_ecalClusterIsoR2->size() == (unsigned)nPho_out)  pho_ecalClusterIsoR2_2_out.push_back(tggHiNtuplizer.pho_ecalClusterIsoR2->at(j));
        if(tggHiNtuplizer.pho_ecalClusterIsoR3->size() == (unsigned)nPho_out)  pho_ecalClusterIsoR3_2_out.push_back(tggHiNtuplizer.pho_ecalClusterIsoR3->at(j));
        if(tggHiNtuplizer.pho_ecalClusterIsoR4->size() == (unsigned)nPho_out)  pho_ecalClusterIsoR4_2_out.push_back(tggHiNtuplizer.pho_ecalClusterIsoR4->at(j));
        if(tggHiNtuplizer.pho_ecalClusterIsoR5->size() == (unsigned)nPho_out)  pho_ecalClusterIsoR5_2_out.push_back(tggHiNtuplizer.pho_ecalClusterIsoR5->at(j));
        if(tggHiNtuplizer.pho_hcalRechitIsoR1->size() == (unsigned)nPho_out)  pho_hcalRechitIsoR1_2_out.push_back(tggHiNtuplizer.pho_hcalRechitIsoR1->at(j));
        if(tggHiNtuplizer.pho_hcalRechitIsoR2->size() == (unsigned)nPho_out)  pho_hcalRechitIsoR2_2_out.push_back(tggHiNtuplizer.pho_hcalRechitIsoR2->at(j));
        if(tggHiNtuplizer.pho_hcalRechitIsoR3->size() == (unsigned)nPho_out)  pho_hcalRechitIsoR3_2_out.push_back(tggHiNtuplizer.pho_hcalRechitIsoR3->at(j));
        if(tggHiNtuplizer.pho_hcalRechitIsoR4->size() == (unsigned)nPho_out)  pho_hcalRechitIsoR4_2_out.push_back(tggHiNtuplizer.pho_hcalRechitIsoR4->at(j));
        if(tggHiNtuplizer.pho_hcalRechitIsoR5->size() == (unsigned)nPho_out)  pho_hcalRechitIsoR5_2_out.push_back(tggHiNtuplizer.pho_hcalRechitIsoR5->at(j));
        if(tggHiNtuplizer.pho_trackIsoR1PtCut20->size() == (unsigned)nPho_out)  pho_trackIsoR1PtCut20_2_out.push_back(tggHiNtuplizer.pho_trackIsoR1PtCut20->at(j));
        if(tggHiNtuplizer.pho_trackIsoR2PtCut20->size() == (unsigned)nPho_out)  pho_trackIsoR2PtCut20_2_out.push_back(tggHiNtuplizer.pho_trackIsoR2PtCut20->at(j));
        if(tggHiNtuplizer.pho_trackIsoR3PtCut20->size() == (unsigned)nPho_out)  pho_trackIsoR3PtCut20_2_out.push_back(tggHiNtuplizer.pho_trackIsoR3PtCut20->at(j));
        if(tggHiNtuplizer.pho_trackIsoR4PtCut20->size() == (unsigned)nPho_out)  pho_trackIsoR4PtCut20_2_out.push_back(tggHiNtuplizer.pho_trackIsoR4PtCut20->at(j));
        if(tggHiNtuplizer.pho_trackIsoR5PtCut20->size() == (unsigned)nPho_out)  pho_trackIsoR5PtCut20_2_out.push_back(tggHiNtuplizer.pho_trackIsoR5PtCut20->at(j));
        if(tggHiNtuplizer.pho_swissCrx->size() == (unsigned)nPho_out)  pho_swissCrx_2_out.push_back(tggHiNtuplizer.pho_swissCrx->at(j));
        if(tggHiNtuplizer.pho_seedTime->size() == (unsigned)nPho_out)  pho_seedTime_2_out.push_back(tggHiNtuplizer.pho_seedTime->at(j));

        // diphoton
        if (tggHiNtuplizer.phoEt->size()  == (unsigned)nPho_out &&
            tggHiNtuplizer.phoEta->size() == (unsigned)nPho_out &&
            tggHiNtuplizer.phoPhi->size() == (unsigned)nPho_out)
        {
            TLorentzVector v1, v2, vSum;
            v1.SetPtEtaPhiE( tggHiNtuplizer.phoEt->at(i), tggHiNtuplizer.phoEta->at(i),
                    tggHiNtuplizer.phoPhi->at(i), tggHiNtuplizer.phoE->at(i));
            v2.SetPtEtaPhiE( tggHiNtuplizer.phoEt->at(j), tggHiNtuplizer.phoEta->at(j),
                    tggHiNtuplizer.phoPhi->at(j), tggHiNtuplizer.phoE->at(j));
            vSum = v1+v2;

            diPhoM_out.push_back(vSum.M());
            diPhoEn_out.push_back(vSum.Energy());
            diPhoPt_out.push_back(vSum.Pt());
            diPhoEta_out.push_back(vSum.Eta());
            diPhoPhi_out.push_back(vSum.Phi());
        }
    }
    else {

        phoE_1_out.push_back(tggHiNtuplizer.phoE->at(i));
        phoEt_1_out.push_back(tggHiNtuplizer.phoEt->at(i));
        phoEta_1_out.push_back(tggHiNtuplizer.phoEta->at(i));
        phoPhi_1_out.push_back(tggHiNtuplizer.phoPhi->at(i));
        phoSCE_1_out.push_back(tggHiNtuplizer.phoSCE->at(i));
        phoSCRawE_1_out.push_back(tggHiNtuplizer.phoSCRawE->at(i));
        phoESEn_1_out.push_back(tggHiNtuplizer.phoESEn->at(i));
        phoSCEta_1_out.push_back(tggHiNtuplizer.phoSCEta->at(i));
        phoSCPhi_1_out.push_back(tggHiNtuplizer.phoSCPhi->at(i));
        phoSCEtaWidth_1_out.push_back(tggHiNtuplizer.phoSCEtaWidth->at(i));
        phoSCPhiWidth_1_out.push_back(tggHiNtuplizer.phoSCPhiWidth->at(i));
        phoSCBrem_1_out.push_back(tggHiNtuplizer.phoSCBrem->at(i));
        phohasPixelSeed_1_out.push_back(tggHiNtuplizer.phoHasPixelSeed->at(i));
        phoR9_1_out.push_back(tggHiNtuplizer.phoR9->at(i));
        phoHoverE_1_out.push_back(tggHiNtuplizer.phoHoverE->at(i));
        phoSigmaIEtaIEta_1_out.push_back(tggHiNtuplizer.phoSigmaIEtaIEta->at(i));
        // phoE1x3_1_out.push_back(tggHiNtuplizer.phoE1x3->at(i));
        // phoE2x2_1_out.push_back(tggHiNtuplizer.phoE2x2->at(i));
//                phoE3x3_1_out.push_back(tggHiNtuplizer.phoE3x3->at(i));
        // phoE2x5Max_1_out.push_back(tggHiNtuplizer.phoE2x5Max->at(i));
//                phoE1x5_1_out.push_back(tggHiNtuplizer.phoE1x5->at(i));
//                phoE2x5_1_out.push_back(tggHiNtuplizer.phoE2x5->at(i));
        phoE5x5_1_out.push_back(tggHiNtuplizer.phoE5x5->at(i));
//                phoMaxEnergyXtal_1_out.push_back(tggHiNtuplizer.phoMaxEnergyXtal->at(i));
//                phoSigmaEtaEta_1_out.push_back(tggHiNtuplizer.phoSigmaEtaEta->at(i));
//                phoR1x5_1_out.push_back(tggHiNtuplizer.phoR1x5->at(i));
//                phoR2x5_1_out.push_back(tggHiNtuplizer.phoR2x5->at(i));
        phoESEffSigmaRR_1_out.push_back(tggHiNtuplizer.phoESEffSigmaRR->at(i));
        // phoSigmaIEtaIEta_2012_1_out.push_back(tggHiNtuplizer.phoSigmaIEtaIEta_2012->at(i));
        // phoSigmaIEtaIPhi_2012_1_out.push_back(tggHiNtuplizer.phoSigmaIEtaIPhi_2012->at(i));
        // phoSigmaIPhiIPhi_2012_1_out.push_back(tggHiNtuplizer.phoSigmaIPhiIPhi_2012->at(i));
        // phoE1x3_2012_1_out.push_back(tggHiNtuplizer.phoE1x3_2012->at(i));
        // phoE2x2_2012_1_out.push_back(tggHiNtuplizer.phoE2x2_2012->at(i));
//                phoE3x3_2012_1_out.push_back(tggHiNtuplizer.phoE3x3_2012->at(i));
        // phoE2x5Max_2012_1_out.push_back(tggHiNtuplizer.phoE2x5Max_2012->at(i));
        // phoE5x5_2012_1_out.push_back(tggHiNtuplizer.phoE5x5_2012->at(i));
        phoBC1E_1_out.push_back(tggHiNtuplizer.phoBC1E->at(i));
        phoBC1Eta_1_out.push_back(tggHiNtuplizer.phoBC1Eta->at(i));
        phoBC2E_1_out.push_back(tggHiNtuplizer.phoBC2E->at(i));
        phoBC2Eta_1_out.push_back(tggHiNtuplizer.phoBC2Eta->at(i));
        pho_ecalClusterIsoR2_1_out.push_back(tggHiNtuplizer.pho_ecalClusterIsoR2->at(i));
        pho_ecalClusterIsoR3_1_out.push_back(tggHiNtuplizer.pho_ecalClusterIsoR3->at(i));
        pho_ecalClusterIsoR4_1_out.push_back(tggHiNtuplizer.pho_ecalClusterIsoR4->at(i));
        pho_ecalClusterIsoR5_1_out.push_back(tggHiNtuplizer.pho_ecalClusterIsoR5->at(i));
        pho_hcalRechitIsoR1_1_out.push_back(tggHiNtuplizer.pho_hcalRechitIsoR1->at(i));
        pho_hcalRechitIsoR2_1_out.push_back(tggHiNtuplizer.pho_hcalRechitIsoR2->at(i));
        pho_hcalRechitIsoR3_1_out.push_back(tggHiNtuplizer.pho_hcalRechitIsoR3->at(i));
        pho_hcalRechitIsoR4_1_out.push_back(tggHiNtuplizer.pho_hcalRechitIsoR4->at(i));
        pho_hcalRechitIsoR5_1_out.push_back(tggHiNtuplizer.pho_hcalRechitIsoR5->at(i));
        pho_trackIsoR1PtCut20_1_out.push_back(tggHiNtuplizer.pho_trackIsoR1PtCut20->at(i));
        pho_trackIsoR2PtCut20_1_out.push_back(tggHiNtuplizer.pho_trackIsoR2PtCut20->at(i));
        pho_trackIsoR3PtCut20_1_out.push_back(tggHiNtuplizer.pho_trackIsoR3PtCut20->at(i));
        pho_trackIsoR4PtCut20_1_out.push_back(tggHiNtuplizer.pho_trackIsoR4PtCut20->at(i));
        pho_trackIsoR5PtCut20_1_out.push_back(tggHiNtuplizer.pho_trackIsoR5PtCut20->at(i));
        pho_swissCrx_1_out.push_back(tggHiNtuplizer.pho_swissCrx->at(i));
        pho_seedTime_1_out.push_back(tggHiNtuplizer.pho_seedTime->at(i));

        phoE_2_out.push_back(tggHiNtuplizer.phoE->at(j));
        phoEt_2_out.push_back(tggHiNtuplizer.phoEt->at(j));
        phoEta_2_out.push_back(tggHiNtuplizer.phoEta->at(j));
        phoPhi_2_out.push_back(tggHiNtuplizer.phoPhi->at(j));
        phoSCE_2_out.push_back(tggHiNtuplizer.phoSCE->at(j));
        phoSCRawE_2_out.push_back(tggHiNtuplizer.phoSCRawE->at(j));
        phoESEn_2_out.push_back(tggHiNtuplizer.phoESEn->at(j));
        phoSCEta_2_out.push_back(tggHiNtuplizer.phoSCEta->at(j));
        phoSCPhi_2_out.push_back(tggHiNtuplizer.phoSCPhi->at(j));
        phoSCEtaWidth_2_out.push_back(tggHiNtuplizer.phoSCEtaWidth->at(j));
        phoSCPhiWidth_2_out.push_back(tggHiNtuplizer.phoSCPhiWidth->at(j));
        phoSCBrem_2_out.push_back(tggHiNtuplizer.phoSCBrem->at(j));
        phohasPixelSeed_2_out.push_back(tggHiNtuplizer.phoHasPixelSeed->at(j));
        phoR9_2_out.push_back(tggHiNtuplizer.phoR9->at(j));
        phoHoverE_2_out.push_back(tggHiNtuplizer.phoHoverE->at(j));
        phoSigmaIEtaIEta_2_out.push_back(tggHiNtuplizer.phoSigmaIEtaIEta->at(j));
        // phoE1x3_2_out.push_back(tggHiNtuplizer.phoE1x3->at(j));
        // phoE2x2_2_out.push_back(tggHiNtuplizer.phoE2x2->at(j));
//                phoE3x3_2_out.push_back(tggHiNtuplizer.phoE3x3->at(j));
        // phoE2x5Max_2_out.push_back(tggHiNtuplizer.phoE2x5Max->at(j));
//                phoE1x5_2_out.push_back(tggHiNtuplizer.phoE1x5->at(j));
//                phoE2x5_2_out.push_back(tggHiNtuplizer.phoE2x5->at(j));
        phoE5x5_2_out.push_back(tggHiNtuplizer.phoE5x5->at(j));
//                phoMaxEnergyXtal_2_out.push_back(tggHiNtuplizer.phoMaxEnergyXtal->at(j));
//                phoSigmaEtaEta_2_out.push_back(tggHiNtuplizer.phoSigmaEtaEta->at(j));
//                phoR1x5_2_out.push_back(tggHiNtuplizer.phoR1x5->at(j));
//                phoR2x5_2_out.push_back(tggHiNtuplizer.phoR2x5->at(j));
        phoESEffSigmaRR_2_out.push_back(tggHiNtuplizer.phoESEffSigmaRR->at(j));
        phoSigmaIEtaIEta_2012_2_out.push_back(tggHiNtuplizer.phoSigmaIEtaIEta_2012->at(j));
        // phoSigmaIEtaIPhi_2012_2_out.push_back(tggHiNtuplizer.phoSigmaIEtaIPhi_2012->at(j));
        // phoSigmaIPhiIPhi_2012_2_out.push_back(tggHiNtuplizer.phoSigmaIPhiIPhi_2012->at(j));
        // phoE1x3_2012_2_out.push_back(tggHiNtuplizer.phoE1x3_2012->at(j));
        // phoE2x2_2012_2_out.push_back(tggHiNtuplizer.phoE2x2_2012->at(j));
//                phoE3x3_2012_2_out.push_back(tggHiNtuplizer.phoE3x3_2012->at(j));
        // phoE2x5Max_2012_2_out.push_back(tggHiNtuplizer.phoE2x5Max_2012->at(j));
        // phoE5x5_2012_2_out.push_back(tggHiNtuplizer.phoE5x5_2012->at(j));
        phoBC1E_2_out.push_back(tggHiNtuplizer.phoBC1E->at(j));
        phoBC1Eta_2_out.push_back(tggHiNtuplizer.phoBC1Eta->at(j));
        phoBC2E_2_out.push_back(tggHiNtuplizer.phoBC2E->at(j));
        phoBC2Eta_2_out.push_back(tggHiNtuplizer.phoBC2Eta->at(j));
        pho_ecalClusterIsoR2_2_out.push_back(tggHiNtuplizer.pho_ecalClusterIsoR2->at(j));
        pho_ecalClusterIsoR3_2_out.push_back(tggHiNtuplizer.pho_ecalClusterIsoR3->at(j));
        pho_ecalClusterIsoR4_2_out.push_back(tggHiNtuplizer.pho_ecalClusterIsoR4->at(j));
        pho_ecalClusterIsoR5_2_out.push_back(tggHiNtuplizer.pho_ecalClusterIsoR5->at(j));
        pho_hcalRechitIsoR1_2_out.push_back(tggHiNtuplizer.pho_hcalRechitIsoR1->at(j));
        pho_hcalRechitIsoR2_2_out.push_back(tggHiNtuplizer.pho_hcalRechitIsoR2->at(j));
        pho_hcalRechitIsoR3_2_out.push_back(tggHiNtuplizer.pho_hcalRechitIsoR3->at(j));
        pho_hcalRechitIsoR4_2_out.push_back(tggHiNtuplizer.pho_hcalRechitIsoR4->at(j));
        pho_hcalRechitIsoR5_2_out.push_back(tggHiNtuplizer.pho_hcalRechitIsoR5->at(j));
        pho_trackIsoR1PtCut20_2_out.push_back(tggHiNtuplizer.pho_trackIsoR1PtCut20->at(j));
        pho_trackIsoR2PtCut20_2_out.push_back(tggHiNtuplizer.pho_trackIsoR2PtCut20->at(j));
        pho_trackIsoR3PtCut20_2_out.push_back(tggHiNtuplizer.pho_trackIsoR3PtCut20->at(j));
        pho_trackIsoR4PtCut20_2_out.push_back(tggHiNtuplizer.pho_trackIsoR4PtCut20->at(j));
        pho_trackIsoR5PtCut20_2_out.push_back(tggHiNtuplizer.pho_trackIsoR5PtCut20->at(j));
        pho_swissCrx_2_out.push_back(tggHiNtuplizer.pho_swissCrx->at(j));
        pho_seedTime_2_out.push_back(tggHiNtuplizer.pho_seedTime->at(j));

        // diphoton
        TLorentzVector v1, v2, vSum;
        v1.SetPtEtaPhiE( tggHiNtuplizer.phoEt->at(i), tggHiNtuplizer.phoEta->at(i),
                tggHiNtuplizer.phoPhi->at(i), tggHiNtuplizer.phoE->at(i));
        v2.SetPtEtaPhiE( tggHiNtuplizer.phoEt->at(j), tggHiNtuplizer.phoEta->at(j),
                tggHiNtuplizer.phoPhi->at(j), tggHiNtuplizer.phoE->at(j));
        vSum = v1+v2;

        diPhoM_out.push_back(vSum.M());
        diPhoEn_out.push_back(vSum.Energy());
        diPhoPt_out.push_back(vSum.Pt());
        diPhoEta_out.push_back(vSum.Eta());
        diPhoPhi_out.push_back(vSum.Phi());
    }
}

void diphoton::makeDiPhotonPairs(ggHiNtuplizer &tggHiNtuplizer, bool doEleMatch, bool skipIfNoMatch, bool doSizeCheck)
{
    clearDiPhotonPairs();

    std::vector<int>   matched_eleCharge_tmp;
    std::vector<int>   matched_eleIndex_tmp;
    std::vector<float> matched_elePt_tmp;
    std::vector<float> matched_eleEta_tmp;
    std::vector<float> matched_elePhi_tmp;
    std::vector<float> matched_eleTrkPt_tmp;

    matched_eleCharge_tmp.clear();
    matched_eleIndex_tmp.clear();
    matched_elePt_tmp.clear();
    matched_eleEta_tmp.clear();
    matched_elePhi_tmp.clear();
    matched_eleTrkPt_tmp.clear();

    nPho_out = tggHiNtuplizer.nPho;
    if (doEleMatch) {
        // electron-photon matching
        // based on  https://github.com/CmsHI/HiForestAnalysis/blob/master/PhotonUtilities.C#L154-L186
        for(int i=0; i< nPho_out; ++i) {
            float currentMaxPt=-1;
            int matched_Index =-1;
            for (int j=0; j< tggHiNtuplizer.nEle; ++j) {
                if (tggHiNtuplizer.elePt->at(j) < currentMaxPt) continue;
                if (getDR(tggHiNtuplizer.eleEta->at(j), tggHiNtuplizer.elePhi->at(j), tggHiNtuplizer.phoEta->at(i), tggHiNtuplizer.phoPhi->at(i))>cutDeltaR) continue;

                matched_Index=j;
                currentMaxPt=tggHiNtuplizer.elePt->at(j);
            }
            matched_eleIndex_tmp.push_back(matched_Index);

            if (matched_Index > -1) {
                matched_eleCharge_tmp.push_back(tggHiNtuplizer.eleCharge->at(matched_Index));
                matched_elePt_tmp.push_back(tggHiNtuplizer.elePt->at(matched_Index));
                matched_eleEta_tmp.push_back(tggHiNtuplizer.eleEta->at(matched_Index));
                matched_elePhi_tmp.push_back(tggHiNtuplizer.elePhi->at(matched_Index));
                matched_eleTrkPt_tmp.push_back(tggHiNtuplizer.eleTrkPt->at(matched_Index));
            } else {
                matched_eleCharge_tmp.push_back(0);
                matched_elePt_tmp.push_back(-1);
                matched_eleEta_tmp.push_back(0);
                matched_elePhi_tmp.push_back(0);
                matched_eleTrkPt_tmp.push_back(-1);
            }
        }
    }
    else {
        for(int i=0; i< nPho_out; ++i) {
            matched_eleCharge_tmp.push_back(0);
            matched_elePt_tmp.push_back(-1);
            matched_eleEta_tmp.push_back(0);
            matched_elePhi_tmp.push_back(0);
            matched_eleTrkPt_tmp.push_back(-1);
        }
    }

    for(int i=0; i<nPho_out; ++i)
    {
        for(int j=i+1; j<nPho_out; ++j)
        {
            if (skipIfNoMatch) {
                if (matched_eleIndex_tmp.at(i) == -1 || matched_eleIndex_tmp.at(j) == -1)  continue;
            }
            pushbackDiPhotonPair(tggHiNtuplizer, i, j, doSizeCheck);

            matched_eleIndex_1_out.push_back(matched_eleIndex_tmp.at(i));
            matched_eleCharge_1_out.push_back(matched_eleCharge_tmp.at(i));
            matched_elePt_1_out.push_back(matched_elePt_tmp.at(i));
            matched_eleEta_1_out.push_back(matched_eleEta_tmp.at(i));
            matched_elePhi_1_out.push_back(matched_elePhi_tmp.at(i));
            matched_eleTrkPt_1_out.push_back(matched_eleTrkPt_tmp.at(i));

            matched_eleIndex_2_out.push_back(matched_eleIndex_tmp.at(j));
            matched_eleCharge_2_out.push_back(matched_eleCharge_tmp.at(j));
            matched_elePt_2_out.push_back(matched_elePt_tmp.at(j));
            matched_eleEta_2_out.push_back(matched_eleEta_tmp.at(j));
            matched_elePhi_2_out.push_back(matched_elePhi_tmp.at(j));
            matched_eleTrkPt_2_out.push_back(matched_eleTrkPt_tmp.at(j));
        }
    }
}

/*
 * make diphoton pairs using the given photon indices i,j
 * matched_Index_1, matched_Index_2 correspond to the index of the electron that matched to the photon
 */
void diphoton::makeDiPhotonPair(ggHiNtuplizer &tggHiNtuplizer, int i1, int i2, int matched_Index_1, int matched_Index_2, bool doSizeCheck)
{
    pushbackDiPhotonPair(tggHiNtuplizer, i1, i2, doSizeCheck);

    matched_eleIndex_1_out.push_back(matched_Index_1);
    if (matched_Index_1 > -1) {
        matched_eleCharge_1_out.push_back(tggHiNtuplizer.eleCharge->at(matched_Index_1));
        matched_elePt_1_out.push_back(tggHiNtuplizer.elePt->at(matched_Index_1));
        matched_eleEta_1_out.push_back(tggHiNtuplizer.eleEta->at(matched_Index_1));
        matched_elePhi_1_out.push_back(tggHiNtuplizer.elePhi->at(matched_Index_1));
        matched_eleTrkPt_1_out.push_back(tggHiNtuplizer.eleTrkPt->at(matched_Index_1));
    } else {
        matched_eleCharge_1_out.push_back(0);
        matched_elePt_1_out.push_back(-1);
        matched_eleEta_1_out.push_back(0);
        matched_elePhi_1_out.push_back(0);
        matched_eleTrkPt_1_out.push_back(-1);
    }

    matched_eleIndex_2_out.push_back(matched_Index_2);
    if (matched_Index_2 > -1) {
        matched_eleCharge_2_out.push_back(tggHiNtuplizer.eleCharge->at(matched_Index_2));
        matched_elePt_2_out.push_back(tggHiNtuplizer.elePt->at(matched_Index_2));
        matched_eleEta_2_out.push_back(tggHiNtuplizer.eleEta->at(matched_Index_2));
        matched_elePhi_2_out.push_back(tggHiNtuplizer.elePhi->at(matched_Index_2));
        matched_eleTrkPt_2_out.push_back(tggHiNtuplizer.eleTrkPt->at(matched_Index_2));
    } else {
        matched_eleCharge_2_out.push_back(0);
        matched_elePt_2_out.push_back(-1);
        matched_eleEta_2_out.push_back(0);
        matched_elePhi_2_out.push_back(0);
        matched_eleTrkPt_2_out.push_back(-1);
    }
}

#endif /* TREEHEADERS_DIPHOTONTREE_H_ */
