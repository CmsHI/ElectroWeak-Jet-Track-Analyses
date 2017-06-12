#ifndef TREEHEADERS_ZJETTREE_H_
#define TREEHEADERS_ZJETTREE_H_

#include <TTree.h>
#include <TMath.h>

#include <vector>

#include "../Utilities/physicsUtil.h"
#include "dielectronTree.h"
#include "dimuonTree.h"
#include "JetTree.h"

class ZJet {
public :
    ZJet(){
        awayRange = TMath::Pi() * 7./8.;
        coneRange = 0.4;

        jetIdx = 0;
        xjz = 0;
        deta = 0;
        dphi = 0;
        dR = 0;
        dR_ele_1 = 0;
        dR_ele_2 = 0;
        insideJet = 0;
        insideJet_ele_1 = 0;
        insideJet_ele_2 = 0;
        nJetinAwayRange = 0;
        jetID = 0;
    }
    ~ZJet(){};
    void resetAwayRange() { awayRange = TMath::Pi() * 7./8.; }
    void resetConeRange() { coneRange = 0.4 ; }
    void setupZJetTree(TTree *t);
    void branchZJetTree(TTree *t);
    void makeZeeJetPairs(dielectron &tdielectron, Jets &tJets, int zIdx, bool use_diEle_out = true);
    void makeZmmJetPairs(dimuon     &tdimuon    , Jets &tJets, int zIdx, bool use_diMu_out = true);
    void clearZJetPairs(int zIdx);
    void makeZeeJetPairsMB(dielectron &tdielectron, Jets &tJets, int zIdx, bool use_diEle_out = true);
    void makeZmmJetPairsMB(dimuon     &tdimuon    , Jets &tJets, int zIdx, bool use_diMu_out = true);

    float awayRange;
    float coneRange;

    // Declaration of leaf types
     Int_t           zIdx;
     Int_t           jetIdx1;       // index of leading jet which passed jetID
     Int_t           jetIdx2;       // index of subleading jet which passed jetID
     std::vector<int>     *jetIdx;
     std::vector<float>   *xjz;
     std::vector<float>   *deta;
     std::vector<float>   *dphi;
     std::vector<float>   *dR;
     std::vector<float>   *dR_ele_1;
     std::vector<float>   *dR_ele_2;
     std::vector<int>     *insideJet;
     std::vector<int>     *insideJet_ele_1;
     std::vector<int>     *insideJet_ele_2;
     Int_t           nJetinAwayRange;
     std::vector<int>     *jetID;

     // List of branches
     TBranch        *b_zIdx;   //!
     TBranch        *b_jetIdx1;   //!
     TBranch        *b_jetIdx2;   //!
     TBranch        *b_jetIdx;   //!
     TBranch        *b_xjz;   //!
     TBranch        *b_deta;   //!
     TBranch        *b_dphi;   //!
     TBranch        *b_dR;   //!
     TBranch        *b_dR_ele_1;   //!
     TBranch        *b_dR_ele_2;   //!
     TBranch        *b_insideJet;   //!
     TBranch        *b_insideJet_ele_1;   //!
     TBranch        *b_insideJet_ele_2;   //!
     TBranch        *b_nJetinAwayRange;   //!
     TBranch        *b_jetID;   //!

     // list of objects to be used when creating a zjet Tree
     Int_t           zIdx_out;
     Int_t           jetIdx1_out;
     Int_t           jetIdx2_out;
     std::vector<int>     jetIdx_out;
     std::vector<float>   xjz_out;
     std::vector<float>   deta_out;
     std::vector<float>   dphi_out;
     std::vector<float>   dR_out;
     std::vector<float>   dR_ele_1_out;
     std::vector<float>   dR_ele_2_out;
     std::vector<int>     insideJet_out;
     std::vector<int>     insideJet_ele_1_out;
     std::vector<int>     insideJet_ele_2_out;
     Int_t           nJetinAwayRange_out;
     std::vector<int>     jetID_out;

};

void ZJet::setupZJetTree(TTree *t)
{
    if (t->GetBranch("zIdx"))  t->SetBranchAddress("zIdx", &zIdx, &b_zIdx);
    if (t->GetBranch("jetIdx1"))  t->SetBranchAddress("jetIdx1", &jetIdx1, &b_jetIdx1);
    if (t->GetBranch("jetIdx2"))  t->SetBranchAddress("jetIdx1", &jetIdx2, &b_jetIdx2);
    if (t->GetBranch("jetIdx"))  t->SetBranchAddress("jetIdx", &jetIdx, &b_jetIdx);
    if (t->GetBranch("xjz"))  t->SetBranchAddress("xjz", &xjz, &b_xjz);
    if (t->GetBranch("deta"))  t->SetBranchAddress("deta", &deta, &b_deta);
    if (t->GetBranch("dphi"))  t->SetBranchAddress("dphi", &dphi, &b_dphi);
    if (t->GetBranch("dR"))  t->SetBranchAddress("dR", &dR, &b_dR);
    if (t->GetBranch("dR_ele_1"))  t->SetBranchAddress("dR_ele_1", &dR_ele_1, &b_dR_ele_1);
    if (t->GetBranch("dR_ele_2"))  t->SetBranchAddress("dR_ele_2", &dR_ele_2, &b_dR_ele_2);
    if (t->GetBranch("insideJet"))  t->SetBranchAddress("insideJet", &insideJet, &b_insideJet);
    if (t->GetBranch("insideJet_ele_1"))  t->SetBranchAddress("insideJet_ele_1", &insideJet_ele_1, &b_insideJet_ele_1);
    if (t->GetBranch("insideJet_ele_2"))  t->SetBranchAddress("insideJet_ele_2", &insideJet_ele_2, &b_insideJet_ele_2);
    if (t->GetBranch("nJetinAwayRange"))  t->SetBranchAddress("nJetinAwayRange", &nJetinAwayRange, &b_nJetinAwayRange);
    if (t->GetBranch("jetID"))  t->SetBranchAddress("jetID", &jetID, &b_jetID);
}

void ZJet::branchZJetTree(TTree *t)
{
    t->Branch("zIdx", &zIdx_out);
    t->Branch("jetIdx1", &jetIdx1_out);
    t->Branch("jetIdx2", &jetIdx2_out);
    t->Branch("jetIdx", &jetIdx_out);
    t->Branch("xjz", &xjz_out);
    t->Branch("deta", &deta_out);
    t->Branch("dphi", &dphi_out);
    t->Branch("dR", &dR_out);
    t->Branch("dR_ele_1", &dR_ele_1_out);
    t->Branch("dR_ele_2", &dR_ele_2_out);
    t->Branch("insideJet", &insideJet_out);
    t->Branch("insideJet_ele_1", &insideJet_ele_1_out);
    t->Branch("insideJet_ele_2", &insideJet_ele_2_out);
    t->Branch("nJetinAwayRange", &nJetinAwayRange_out);
    t->Branch("jetID", &jetID_out);
}

void ZJet::makeZeeJetPairs(dielectron &tdielectron, Jets &tJets, int zIdx, bool use_diEle_out)
{
    if (use_diEle_out) {
        clearZJetPairs(zIdx);

        jetIdx1_out = -1;
        jetIdx2_out = -1;
        double jetIdx1_pt = -1;
        double jetIdx2_pt = -1;
        // all the jets must go into correlation,
        // no jet should be skipped.
        for (int i=0; i<tJets.nref; ++i)
        {
            // cuts on jets will be applied during plotting
            float tmp_deta = getDETA(tdielectron.diEleEta_out.at(zIdx), tJets.jteta[i]);
            float tmp_dphi = getDPHI(tdielectron.diElePhi_out.at(zIdx), tJets.jtphi[i]);
            if (TMath::Abs(tmp_dphi) > awayRange)
                nJetinAwayRange_out++;
            float tmp_dR   = getDR(tdielectron.diEleEta_out.at(zIdx), tdielectron.diElePhi_out.at(zIdx), tJets.jteta[i], tJets.jtphi[i]);
            float tmp_dR_ele_1 = getDR(tdielectron.eleEta_1_out.at(zIdx), tdielectron.elePhi_1_out.at(zIdx), tJets.jteta[i], tJets.jtphi[i]);
            float tmp_dR_ele_2 = getDR(tdielectron.eleEta_2_out.at(zIdx), tdielectron.elePhi_2_out.at(zIdx), tJets.jteta[i], tJets.jtphi[i]);

            int tmp_insideJet;
            if(tmp_dR < coneRange) tmp_insideJet = 1;
            else                         tmp_insideJet = 0;
            int tmp_insideJet_ele_1;
            if(tmp_dR_ele_1 < coneRange) tmp_insideJet_ele_1 = 1;
            else                               tmp_insideJet_ele_1 = 0;
            int tmp_insideJet_ele_2;
            if(tmp_dR_ele_2 < coneRange) tmp_insideJet_ele_2 = 1;
            else                               tmp_insideJet_ele_2 = 0;

            int tmp_jetID = tJets.jetID(i);

            if (tJets.jtpt[i] > jetIdx1_pt && tmp_jetID == 1) {
                // current leading jet becomes subleading jet
                jetIdx2_pt = jetIdx1_pt;
                jetIdx2_out = jetIdx1_out;

                jetIdx1_pt = tJets.jtpt[i];
                jetIdx1_out = i;
            }
            else if (tJets.jtpt[i] > jetIdx2_pt && tmp_jetID == 1) {
                jetIdx2_pt = tJets.jtpt[i];
                jetIdx2_out = i;
            }

            jetIdx_out.push_back(i);
            if (tdielectron.diElePt_out.at(zIdx) > 0) {
                xjz_out.push_back((float)tJets.jtpt[i]/tdielectron.diElePt_out.at(zIdx));
            }
            else {
                xjz_out.push_back(-1);
            }
            deta_out.push_back(tmp_deta);
            dphi_out.push_back(tmp_dphi);
            dR_out.push_back(tmp_dR);
            dR_ele_1_out.push_back(tmp_dR_ele_1);
            dR_ele_2_out.push_back(tmp_dR_ele_2);
            insideJet_out.push_back(tmp_insideJet);
            insideJet_ele_1_out.push_back(tmp_insideJet_ele_1);
            insideJet_ele_2_out.push_back(tmp_insideJet_ele_2);
            jetID_out.push_back(tmp_jetID);
        }
    }
    else {
        clearZJetPairs(zIdx);

        jetIdx1_out = -1;
        jetIdx2_out = -1;
        double jetIdx1_pt = -1;
        double jetIdx2_pt = -1;
        // all the jets must go into correlation,
        // no jet should be skipped.
        for (int i=0; i<tJets.nref; ++i)
        {
            // cuts on jets will be applied during plotting
            float tmp_deta = getDETA(tdielectron.diEleEta->at(zIdx), tJets.jteta[i]);
            float tmp_dphi = getDPHI(tdielectron.diElePhi->at(zIdx), tJets.jtphi[i]);
            if (TMath::Abs(tmp_dphi) > awayRange)
                nJetinAwayRange_out++;
            float tmp_dR   = getDR(tdielectron.diEleEta->at(zIdx), tdielectron.diElePhi->at(zIdx), tJets.jteta[i], tJets.jtphi[i]);
            float tmp_dR_ele_1 = getDR(tdielectron.eleEta_1->at(zIdx), tdielectron.elePhi_1->at(zIdx), tJets.jteta[i], tJets.jtphi[i]);
            float tmp_dR_ele_2 = getDR(tdielectron.eleEta_2->at(zIdx), tdielectron.elePhi_2->at(zIdx), tJets.jteta[i], tJets.jtphi[i]);

            int tmp_insideJet;
            if(tmp_dR < coneRange) tmp_insideJet = 1;
            else                         tmp_insideJet = 0;
            int tmp_insideJet_ele_1;
            if(tmp_dR_ele_1 < coneRange) tmp_insideJet_ele_1 = 1;
            else                               tmp_insideJet_ele_1 = 0;
            int tmp_insideJet_ele_2;
            if(tmp_dR_ele_2 < coneRange) tmp_insideJet_ele_2 = 1;
            else                               tmp_insideJet_ele_2 = 0;

            int tmp_jetID = tJets.jetID(i);

            if (tJets.jtpt[i] > jetIdx1_pt && tmp_jetID == 1) {
                // current leading jet becomes subleading jet
                jetIdx2_pt = jetIdx1_pt;
                jetIdx2_out = jetIdx1_out;

                jetIdx1_pt = tJets.jtpt[i];
                jetIdx1_out = i;
            }
            else if (tJets.jtpt[i] > jetIdx2_pt && tmp_jetID == 1) {
                jetIdx2_pt = tJets.jtpt[i];
                jetIdx2_out = i;
            }

            jetIdx_out.push_back(i);
            if (tdielectron.diElePt->at(zIdx) > 0) {
                xjz_out.push_back((float)tJets.jtpt[i]/tdielectron.diElePt->at(zIdx));
            }
            else {
                xjz_out.push_back(-1);
            }
            deta_out.push_back(tmp_deta);
            dphi_out.push_back(tmp_dphi);
            dR_out.push_back(tmp_dR);
            dR_ele_1_out.push_back(tmp_dR_ele_1);
            dR_ele_2_out.push_back(tmp_dR_ele_2);
            insideJet_out.push_back(tmp_insideJet);
            insideJet_ele_1_out.push_back(tmp_insideJet_ele_1);
            insideJet_ele_2_out.push_back(tmp_insideJet_ele_2);
            jetID_out.push_back(tmp_jetID);
        }
    }
}

void ZJet::makeZmmJetPairs(dimuon &tdimuon, Jets &tJets, int zIdx, bool use_diMu_out)
{
    if (use_diMu_out) {
        clearZJetPairs(zIdx);

        jetIdx1_out = -1;
        jetIdx2_out = -1;
        double jetIdx1_pt = -1;
        double jetIdx2_pt = -1;
        // all the jets must go into correlation,
        // no jet should be skipped.
        for (int i=0; i<tJets.nref; ++i)
        {
            // cuts on jets will be applied during plotting
            float tmp_deta = getDETA(tdimuon.diMuEta_out.at(zIdx), tJets.jteta[i]);
            float tmp_dphi = getDPHI(tdimuon.diMuPhi_out.at(zIdx), tJets.jtphi[i]);
            if (TMath::Abs(tmp_dphi) > awayRange)
                nJetinAwayRange_out++;
            float tmp_dR   = getDR(tdimuon.diMuEta_out.at(zIdx), tdimuon.diMuPhi_out.at(zIdx), tJets.jteta[i], tJets.jtphi[i]);
            float tmp_dR_ele_1 = getDR(tdimuon.muEta_1_out.at(zIdx), tdimuon.muPhi_1_out.at(zIdx), tJets.jteta[i], tJets.jtphi[i]);
            float tmp_dR_ele_2 = getDR(tdimuon.muEta_2_out.at(zIdx), tdimuon.muPhi_2_out.at(zIdx), tJets.jteta[i], tJets.jtphi[i]);

            int tmp_insideJet;
            if(tmp_dR < coneRange) tmp_insideJet = 1;
            else                         tmp_insideJet = 0;
            int tmp_insideJet_ele_1;
            if(tmp_dR_ele_1 < coneRange) tmp_insideJet_ele_1 = 1;
            else                               tmp_insideJet_ele_1 = 0;
            int tmp_insideJet_ele_2;
            if(tmp_dR_ele_2 < coneRange) tmp_insideJet_ele_2 = 1;
            else                               tmp_insideJet_ele_2 = 0;

            int tmp_jetID = tJets.jetID(i);

            if (tJets.jtpt[i] > jetIdx1_pt && tmp_jetID == 1) {
                // current leading jet becomes subleading jet
                jetIdx2_pt = jetIdx1_pt;
                jetIdx2_out = jetIdx1_out;

                jetIdx1_pt = tJets.jtpt[i];
                jetIdx1_out = i;
            }
            else if (tJets.jtpt[i] > jetIdx2_pt && tmp_jetID == 1) {
                jetIdx2_pt = tJets.jtpt[i];
                jetIdx2_out = i;
            }

            jetIdx_out.push_back(i);
            if (tdimuon.diMuPt_out.at(zIdx) > 0) {
                xjz_out.push_back((float)tJets.jtpt[i]/tdimuon.diMuPt_out.at(zIdx));
            }
            else {
                xjz_out.push_back(-1);
            }
            deta_out.push_back(tmp_deta);
            dphi_out.push_back(tmp_dphi);
            dR_out.push_back(tmp_dR);
            dR_ele_1_out.push_back(tmp_dR_ele_1);
            dR_ele_2_out.push_back(tmp_dR_ele_2);
            insideJet_out.push_back(tmp_insideJet);
            insideJet_ele_1_out.push_back(tmp_insideJet_ele_1);
            insideJet_ele_2_out.push_back(tmp_insideJet_ele_2);
            jetID_out.push_back(tmp_jetID);
        }
    }
    else {
        clearZJetPairs(zIdx);

        jetIdx1_out = -1;
        jetIdx2_out = -1;
        double jetIdx1_pt = -1;
        double jetIdx2_pt = -1;
        // all the jets must go into correlation,
        // no jet should be skipped.
        for (int i=0; i<tJets.nref; ++i)
        {
            // cuts on jets will be applied during plotting
            float tmp_deta = getDETA(tdimuon.diMuEta->at(zIdx), tJets.jteta[i]);
            float tmp_dphi = getDPHI(tdimuon.diMuPhi->at(zIdx), tJets.jtphi[i]);
            if (TMath::Abs(tmp_dphi) > awayRange)
                nJetinAwayRange_out++;
            float tmp_dR   = getDR(tdimuon.diMuEta->at(zIdx), tdimuon.diMuPhi->at(zIdx), tJets.jteta[i], tJets.jtphi[i]);
            float tmp_dR_ele_1 = getDR(tdimuon.muEta_1->at(zIdx), tdimuon.muPhi_1->at(zIdx), tJets.jteta[i], tJets.jtphi[i]);
            float tmp_dR_ele_2 = getDR(tdimuon.muEta_2->at(zIdx), tdimuon.muPhi_2->at(zIdx), tJets.jteta[i], tJets.jtphi[i]);

            int tmp_insideJet;
            if(tmp_dR < coneRange) tmp_insideJet = 1;
            else                         tmp_insideJet = 0;
            int tmp_insideJet_ele_1;
            if(tmp_dR_ele_1 < coneRange) tmp_insideJet_ele_1 = 1;
            else                               tmp_insideJet_ele_1 = 0;
            int tmp_insideJet_ele_2;
            if(tmp_dR_ele_2 < coneRange) tmp_insideJet_ele_2 = 1;
            else                               tmp_insideJet_ele_2 = 0;

            int tmp_jetID = tJets.jetID(i);

            if (tJets.jtpt[i] > jetIdx1_pt && tmp_jetID == 1) {
                // current leading jet becomes subleading jet
                jetIdx2_pt = jetIdx1_pt;
                jetIdx2_out = jetIdx1_out;

                jetIdx1_pt = tJets.jtpt[i];
                jetIdx1_out = i;
            }
            else if (tJets.jtpt[i] > jetIdx2_pt && tmp_jetID == 1) {
                jetIdx2_pt = tJets.jtpt[i];
                jetIdx2_out = i;
            }

            jetIdx_out.push_back(i);
            if (tdimuon.diMuPt->at(zIdx) > 0) {
                xjz_out.push_back((float)tJets.jtpt[i]/tdimuon.diMuPt->at(zIdx));
            }
            else {
                xjz_out.push_back(-1);
            }
            deta_out.push_back(tmp_deta);
            dphi_out.push_back(tmp_dphi);
            dR_out.push_back(tmp_dR);
            dR_ele_1_out.push_back(tmp_dR_ele_1);
            dR_ele_2_out.push_back(tmp_dR_ele_2);
            insideJet_out.push_back(tmp_insideJet);
            insideJet_ele_1_out.push_back(tmp_insideJet_ele_1);
            insideJet_ele_2_out.push_back(tmp_insideJet_ele_2);
            jetID_out.push_back(tmp_jetID);
        }
    }
}

/*
 * in MB event mixing step, one Z boson is paired to jets from multiple events.
 * So, In the same event, zjet pairs will be made multiple times.
 * This function is used to make sure that vectors of "zJet" object is reset only once,
 * not at each pair making.
 */
void ZJet::clearZJetPairs(int zIdx)
{
    jetIdx_out.clear();
    xjz_out.clear();
    deta_out.clear();
    dphi_out.clear();
    dR_out.clear();
    dR_ele_1_out.clear();
    dR_ele_2_out.clear();
    insideJet_out.clear();
    insideJet_ele_1_out.clear();
    insideJet_ele_2_out.clear();
    jetID_out.clear();

    zIdx_out = zIdx;
    nJetinAwayRange_out = 0;
}

void ZJet::makeZeeJetPairsMB(dielectron &tdielectron, Jets &tJets, int zIdx, bool use_diEle_out)
{
    if (use_diEle_out){
        jetIdx1_out = -1;
        jetIdx2_out = -1;
        double jetIdx1_pt = -1;
        double jetIdx2_pt = -1;
        // all the jets must go into correlation,
        // no jet should be skipped.
        for (int i=0; i<tJets.nref; ++i)
        {
            // cuts on jets will be applied during plotting
            float tmp_deta = getDETA(tdielectron.diEleEta_out.at(zIdx), tJets.jteta[i]);
            float tmp_dphi = getDPHI(tdielectron.diElePhi_out.at(zIdx), tJets.jtphi[i]);
            if (TMath::Abs(tmp_dphi) > awayRange)
                nJetinAwayRange_out++;
            float tmp_dR   = getDR(tdielectron.diEleEta_out.at(zIdx), tdielectron.diElePhi_out.at(zIdx), tJets.jteta[i], tJets.jtphi[i]);
            float tmp_dR_ele_1 = getDR(tdielectron.eleEta_1_out.at(zIdx), tdielectron.elePhi_1_out.at(zIdx), tJets.jteta[i], tJets.jtphi[i]);
            float tmp_dR_ele_2 = getDR(tdielectron.eleEta_2_out.at(zIdx), tdielectron.elePhi_2_out.at(zIdx), tJets.jteta[i], tJets.jtphi[i]);

            int tmp_insideJet;
            if(tmp_dR < coneRange) tmp_insideJet = 1;
            else                         tmp_insideJet = 0;
            int tmp_insideJet_ele_1;
            if(tmp_dR_ele_1 < coneRange) tmp_insideJet_ele_1 = 1;
            else                               tmp_insideJet_ele_1 = 0;
            int tmp_insideJet_ele_2;
            if(tmp_dR_ele_2 < coneRange) tmp_insideJet_ele_2 = 1;
            else                               tmp_insideJet_ele_2 = 0;

            int tmp_jetID = tJets.jetID(i);

            if (tJets.jtpt[i] > jetIdx1_pt && tmp_jetID == 1) {
                // current leading jet becomes subleading jet
                jetIdx2_pt = jetIdx1_pt;
                jetIdx2_out = jetIdx1_out;

                jetIdx1_pt = tJets.jtpt[i];
                jetIdx1_out = i;
            }
            else if (tJets.jtpt[i] > jetIdx2_pt && tmp_jetID == 1) {
                jetIdx2_pt = tJets.jtpt[i];
                jetIdx2_out = i;
            }

            jetIdx_out.push_back(i);
            if (tdielectron.diElePt_out.at(zIdx) > 0) {
                xjz_out.push_back((float)tJets.jtpt[i]/tdielectron.diElePt_out.at(zIdx));
            }
            else {
                xjz_out.push_back(-1);
            }
            deta_out.push_back(tmp_deta);
            dphi_out.push_back(tmp_dphi);
            dR_out.push_back(tmp_dR);
            dR_ele_1_out.push_back(tmp_dR_ele_1);
            dR_ele_2_out.push_back(tmp_dR_ele_2);
            insideJet_out.push_back(tmp_insideJet);
            insideJet_ele_1_out.push_back(tmp_insideJet_ele_1);
            insideJet_ele_2_out.push_back(tmp_insideJet_ele_2);
            jetID_out.push_back(tmp_jetID);
        }
    }
    else {
        jetIdx1_out = -1;
        jetIdx2_out = -1;
        double jetIdx1_pt = -1;
        double jetIdx2_pt = -1;
        // all the jets must go into correlation,
        // no jet should be skipped.
        for (int i=0; i<tJets.nref; ++i)
        {
            // cuts on jets will be applied during plotting
            float tmp_deta = getDETA(tdielectron.diEleEta->at(zIdx), tJets.jteta[i]);
            float tmp_dphi = getDPHI(tdielectron.diElePhi->at(zIdx), tJets.jtphi[i]);
            if (TMath::Abs(tmp_dphi) > awayRange)
                nJetinAwayRange_out++;
            float tmp_dR   = getDR(tdielectron.diEleEta->at(zIdx), tdielectron.diElePhi->at(zIdx), tJets.jteta[i], tJets.jtphi[i]);
            float tmp_dR_ele_1 = getDR(tdielectron.eleEta_1->at(zIdx), tdielectron.elePhi_1->at(zIdx), tJets.jteta[i], tJets.jtphi[i]);
            float tmp_dR_ele_2 = getDR(tdielectron.eleEta_2->at(zIdx), tdielectron.elePhi_2->at(zIdx), tJets.jteta[i], tJets.jtphi[i]);

            int tmp_insideJet;
            if(tmp_dR < coneRange) tmp_insideJet = 1;
            else                         tmp_insideJet = 0;
            int tmp_insideJet_ele_1;
            if(tmp_dR_ele_1 < coneRange) tmp_insideJet_ele_1 = 1;
            else                               tmp_insideJet_ele_1 = 0;
            int tmp_insideJet_ele_2;
            if(tmp_dR_ele_2 < coneRange) tmp_insideJet_ele_2 = 1;
            else                               tmp_insideJet_ele_2 = 0;

            int tmp_jetID = tJets.jetID(i);

            if (tJets.jtpt[i] > jetIdx1_pt && tmp_jetID == 1) {
                // current leading jet becomes subleading jet
                jetIdx2_pt = jetIdx1_pt;
                jetIdx2_out = jetIdx1_out;

                jetIdx1_pt = tJets.jtpt[i];
                jetIdx1_out = i;
            }
            else if (tJets.jtpt[i] > jetIdx2_pt && tmp_jetID == 1) {
                jetIdx2_pt = tJets.jtpt[i];
                jetIdx2_out = i;
            }

            jetIdx_out.push_back(i);
            if (tdielectron.diElePt->at(zIdx) > 0) {
                xjz_out.push_back((float)tJets.jtpt[i]/tdielectron.diElePt->at(zIdx));
            }
            else {
                xjz_out.push_back(-1);
            }
            deta_out.push_back(tmp_deta);
            dphi_out.push_back(tmp_dphi);
            dR_out.push_back(tmp_dR);
            dR_ele_1_out.push_back(tmp_dR_ele_1);
            dR_ele_2_out.push_back(tmp_dR_ele_2);
            insideJet_out.push_back(tmp_insideJet);
            insideJet_ele_1_out.push_back(tmp_insideJet_ele_1);
            insideJet_ele_2_out.push_back(tmp_insideJet_ele_2);
            jetID_out.push_back(tmp_jetID);
        }
    }
}

void ZJet::makeZmmJetPairsMB(dimuon &tdimuon, Jets &tJets, int zIdx, bool use_diMu_out)
{
    if (use_diMu_out){
        jetIdx1_out = -1;
        jetIdx2_out = -1;
        double jetIdx1_pt = -1;
        double jetIdx2_pt = -1;
        // all the jets must go into correlation,
        // no jet should be skipped.
        for (int i=0; i<tJets.nref; ++i)
        {
            // cuts on jets will be applied during plotting
            float tmp_deta = getDETA(tdimuon.diMuEta_out.at(zIdx), tJets.jteta[i]);
            float tmp_dphi = getDPHI(tdimuon.diMuPhi_out.at(zIdx), tJets.jtphi[i]);
            if (TMath::Abs(tmp_dphi) > awayRange)
                nJetinAwayRange_out++;
            float tmp_dR   = getDR(tdimuon.diMuEta_out.at(zIdx), tdimuon.diMuPhi_out.at(zIdx), tJets.jteta[i], tJets.jtphi[i]);
            float tmp_dR_ele_1 = getDR(tdimuon.muEta_1_out.at(zIdx), tdimuon.muPhi_1_out.at(zIdx), tJets.jteta[i], tJets.jtphi[i]);
            float tmp_dR_ele_2 = getDR(tdimuon.muEta_2_out.at(zIdx), tdimuon.muPhi_2_out.at(zIdx), tJets.jteta[i], tJets.jtphi[i]);

            int tmp_insideJet;
            if(tmp_dR < coneRange) tmp_insideJet = 1;
            else                         tmp_insideJet = 0;
            int tmp_insideJet_ele_1;
            if(tmp_dR_ele_1 < coneRange) tmp_insideJet_ele_1 = 1;
            else                               tmp_insideJet_ele_1 = 0;
            int tmp_insideJet_ele_2;
            if(tmp_dR_ele_2 < coneRange) tmp_insideJet_ele_2 = 1;
            else                               tmp_insideJet_ele_2 = 0;

            int tmp_jetID = tJets.jetID(i);

            if (tJets.jtpt[i] > jetIdx1_pt && tmp_jetID == 1) {
                // current leading jet becomes subleading jet
                jetIdx2_pt = jetIdx1_pt;
                jetIdx2_out = jetIdx1_out;

                jetIdx1_pt = tJets.jtpt[i];
                jetIdx1_out = i;
            }
            else if (tJets.jtpt[i] > jetIdx2_pt && tmp_jetID == 1) {
                jetIdx2_pt = tJets.jtpt[i];
                jetIdx2_out = i;
            }

            jetIdx_out.push_back(i);
            if (tdimuon.diMuPt_out.at(zIdx) > 0) {
                xjz_out.push_back((float)tJets.jtpt[i]/tdimuon.diMuPt_out.at(zIdx));
            }
            else {
                xjz_out.push_back(-1);
            }
            deta_out.push_back(tmp_deta);
            dphi_out.push_back(tmp_dphi);
            dR_out.push_back(tmp_dR);
            dR_ele_1_out.push_back(tmp_dR_ele_1);
            dR_ele_2_out.push_back(tmp_dR_ele_2);
            insideJet_out.push_back(tmp_insideJet);
            insideJet_ele_1_out.push_back(tmp_insideJet_ele_1);
            insideJet_ele_2_out.push_back(tmp_insideJet_ele_2);
            jetID_out.push_back(tmp_jetID);
        }
    }
    else {
        jetIdx1_out = -1;
        jetIdx2_out = -1;
        double jetIdx1_pt = -1;
        double jetIdx2_pt = -1;
        // all the jets must go into correlation,
        // no jet should be skipped.
        for (int i=0; i<tJets.nref; ++i)
        {
            // cuts on jets will be applied during plotting
            float tmp_deta = getDETA(tdimuon.diMuEta->at(zIdx), tJets.jteta[i]);
            float tmp_dphi = getDPHI(tdimuon.diMuPhi->at(zIdx), tJets.jtphi[i]);
            if (TMath::Abs(tmp_dphi) > awayRange)
                nJetinAwayRange_out++;
            float tmp_dR   = getDR(tdimuon.diMuEta->at(zIdx), tdimuon.diMuPhi->at(zIdx), tJets.jteta[i], tJets.jtphi[i]);
            float tmp_dR_ele_1 = getDR(tdimuon.muEta_1->at(zIdx), tdimuon.muPhi_1->at(zIdx), tJets.jteta[i], tJets.jtphi[i]);
            float tmp_dR_ele_2 = getDR(tdimuon.muEta_2->at(zIdx), tdimuon.muPhi_2->at(zIdx), tJets.jteta[i], tJets.jtphi[i]);

            int tmp_insideJet;
            if(tmp_dR < coneRange) tmp_insideJet = 1;
            else                         tmp_insideJet = 0;
            int tmp_insideJet_ele_1;
            if(tmp_dR_ele_1 < coneRange) tmp_insideJet_ele_1 = 1;
            else                               tmp_insideJet_ele_1 = 0;
            int tmp_insideJet_ele_2;
            if(tmp_dR_ele_2 < coneRange) tmp_insideJet_ele_2 = 1;
            else                               tmp_insideJet_ele_2 = 0;

            int tmp_jetID = tJets.jetID(i);

            if (tJets.jtpt[i] > jetIdx1_pt && tmp_jetID == 1) {
                // current leading jet becomes subleading jet
                jetIdx2_pt = jetIdx1_pt;
                jetIdx2_out = jetIdx1_out;

                jetIdx1_pt = tJets.jtpt[i];
                jetIdx1_out = i;
            }
            else if (tJets.jtpt[i] > jetIdx2_pt && tmp_jetID == 1) {
                jetIdx2_pt = tJets.jtpt[i];
                jetIdx2_out = i;
            }

            jetIdx_out.push_back(i);
            if (tdimuon.diMuPt->at(zIdx) > 0) {
                xjz_out.push_back((float)tJets.jtpt[i]/tdimuon.diMuPt->at(zIdx));
            }
            else {
                xjz_out.push_back(-1);
            }
            deta_out.push_back(tmp_deta);
            dphi_out.push_back(tmp_dphi);
            dR_out.push_back(tmp_dR);
            dR_ele_1_out.push_back(tmp_dR_ele_1);
            dR_ele_2_out.push_back(tmp_dR_ele_2);
            insideJet_out.push_back(tmp_insideJet);
            insideJet_ele_1_out.push_back(tmp_insideJet_ele_1);
            insideJet_ele_2_out.push_back(tmp_insideJet_ele_2);
            jetID_out.push_back(tmp_jetID);
        }
    }
}

#endif /* TREEHEADERS_ZJETTREE_H_ */
