#ifndef TREEHEADERS_ZJETTREE_H_
#define TREEHEADERS_ZJETTREE_H_

#include <TTree.h>
#include <TMath.h>

#include <vector>

#include "../Plotting/commonUtility.h"
#include "SetupJetTree.h"
#include "dielectronTree.h"

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
    }
    ~ZJet(){};
    void resetAwayRange() { awayRange = TMath::Pi() * 7./8.; }
    void resetConeRange() { coneRange = 0.4 ; }

    float awayRange;
    float coneRange;

    // Declaration of leaf types
     Int_t           zIdx;
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

     // List of branches
     TBranch        *b_zIdx;   //!
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

     // list of objects to be used when creating a zjet Tree
     Int_t           zIdx_out;
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

};

void setupZJetTree(TTree *t,ZJet &tzJet);
void branchZJetTree(TTree *t,ZJet &tzJet);
void makeZJetPairs(dielectron &tdielectron, Jets &tJets, ZJet &tzJet, int zIdx, bool use_diEle_out = true);
void clearZJetPairs(ZJet &tzJet, int zIdx);
void makeZJetPairsMB(dielectron &tdielectron, Jets &tJets, ZJet &tzJet, int zIdx, bool use_diEle_out = true);

void setupZJetTree(TTree *t,ZJet &tzJet)
{
    if (t->GetBranch("zIdx"))  t->SetBranchAddress("zIdx", &tzJet.zIdx, &tzJet.b_zIdx);
    if (t->GetBranch("jetIdx"))  t->SetBranchAddress("jetIdx", &tzJet.jetIdx, &tzJet.b_jetIdx);
    if (t->GetBranch("xjz"))  t->SetBranchAddress("xjz", &tzJet.xjz, &tzJet.b_xjz);
    if (t->GetBranch("deta"))  t->SetBranchAddress("deta", &tzJet.deta, &tzJet.b_deta);
    if (t->GetBranch("dphi"))  t->SetBranchAddress("dphi", &tzJet.dphi, &tzJet.b_dphi);
    if (t->GetBranch("dR"))  t->SetBranchAddress("dR", &tzJet.dR, &tzJet.b_dR);
    if (t->GetBranch("dR_ele_1"))  t->SetBranchAddress("dR_ele_1", &tzJet.dR_ele_1, &tzJet.b_dR_ele_1);
    if (t->GetBranch("dR_ele_2"))  t->SetBranchAddress("dR_ele_2", &tzJet.dR_ele_2, &tzJet.b_dR_ele_2);
    if (t->GetBranch("insideJet"))  t->SetBranchAddress("insideJet", &tzJet.insideJet, &tzJet.b_insideJet);
    if (t->GetBranch("insideJet_ele_1"))  t->SetBranchAddress("insideJet_ele_1", &tzJet.insideJet_ele_1, &tzJet.b_insideJet_ele_1);
    if (t->GetBranch("insideJet_ele_2"))  t->SetBranchAddress("insideJet_ele_2", &tzJet.insideJet_ele_2, &tzJet.b_insideJet_ele_2);
    if (t->GetBranch("nJetinAwayRange"))  t->SetBranchAddress("nJetinAwayRange", &tzJet.nJetinAwayRange, &tzJet.b_nJetinAwayRange);
}

void branchZJetTree(TTree *t,ZJet &tzJet)
{
    t->Branch("zIdx", &tzJet.zIdx_out);
    t->Branch("jetIdx", &tzJet.jetIdx_out);
    t->Branch("xjz", &tzJet.xjz_out);
    t->Branch("deta", &tzJet.deta_out);
    t->Branch("dphi", &tzJet.dphi_out);
    t->Branch("dR", &tzJet.dR_out);
    t->Branch("dR_ele_1", &tzJet.dR_ele_1_out);
    t->Branch("dR_ele_2", &tzJet.dR_ele_2_out);
    t->Branch("insideJet", &tzJet.insideJet_out);
    t->Branch("insideJet_ele_1", &tzJet.insideJet_ele_1_out);
    t->Branch("insideJet_ele_2", &tzJet.insideJet_ele_2_out);
    t->Branch("nJetinAwayRange", &tzJet.nJetinAwayRange_out);
}

void makeZJetPairs(dielectron &tdielectron, Jets &tJets, ZJet &tzJet, int zIdx, bool use_diEle_out)
{
    if (use_diEle_out) {
        tzJet.jetIdx_out.clear();
        tzJet.xjz_out.clear();
        tzJet.deta_out.clear();
        tzJet.dphi_out.clear();
        tzJet.dR_out.clear();
        tzJet.dR_ele_1_out.clear();
        tzJet.dR_ele_2_out.clear();
        tzJet.insideJet_out.clear();
        tzJet.insideJet_ele_1_out.clear();
        tzJet.insideJet_ele_2_out.clear();

        tzJet.zIdx_out = zIdx;
        tzJet.nJetinAwayRange_out = 0;

        // all the jets must go into correlation,
        // no jet should be skipped.
        for (int i=0; i<tJets.nref; ++i)
        {
            // cuts on jets will be applied during plotting
            float tmp_deta = getDETA(tdielectron.diEleEta_out.at(zIdx), tJets.jteta[i]);
            float tmp_dphi = getDPHI(tdielectron.diElePhi_out.at(zIdx), tJets.jtphi[i]);
            if (TMath::Abs(tmp_dphi) > tzJet.awayRange)
                tzJet.nJetinAwayRange_out++;
            float tmp_dR   = getDR(tdielectron.diEleEta_out.at(zIdx), tdielectron.diElePhi_out.at(zIdx), tJets.jteta[i], tJets.jtphi[i]);
            float tmp_dR_ele_1 = getDR(tdielectron.eleEta_1_out.at(zIdx), tdielectron.elePhi_1_out.at(zIdx), tJets.jteta[i], tJets.jtphi[i]);
            float tmp_dR_ele_2 = getDR(tdielectron.eleEta_2_out.at(zIdx), tdielectron.elePhi_2_out.at(zIdx), tJets.jteta[i], tJets.jtphi[i]);

            int tmp_insideJet;
            if(tmp_dR < tzJet.coneRange) tmp_insideJet = 1;
            else                         tmp_insideJet = 0;
            int tmp_insideJet_ele_1;
            if(tmp_dR_ele_1 < tzJet.coneRange) tmp_insideJet_ele_1 = 1;
            else                               tmp_insideJet_ele_1 = 0;
            int tmp_insideJet_ele_2;
            if(tmp_dR_ele_2 < tzJet.coneRange) tmp_insideJet_ele_2 = 1;
            else                               tmp_insideJet_ele_2 = 0;

            tzJet.jetIdx_out.push_back(i);
            if (tdielectron.diElePt_out.at(zIdx) > 0) {
                tzJet.xjz_out.push_back((float)tJets.jtpt[i]/tdielectron.diElePt_out.at(zIdx));
            }
            else {
                tzJet.xjz_out.push_back(-1);
            }
            tzJet.deta_out.push_back(tmp_deta);
            tzJet.dphi_out.push_back(tmp_dphi);
            tzJet.dR_out.push_back(tmp_dR);
            tzJet.dR_ele_1_out.push_back(tmp_dR_ele_1);
            tzJet.dR_ele_2_out.push_back(tmp_dR_ele_2);
            tzJet.insideJet_out.push_back(tmp_insideJet);
            tzJet.insideJet_ele_1_out.push_back(tmp_insideJet_ele_1);
            tzJet.insideJet_ele_2_out.push_back(tmp_insideJet_ele_2);
        }
    }
    else {
        tzJet.jetIdx_out.clear();
        tzJet.xjz_out.clear();
        tzJet.deta_out.clear();
        tzJet.dphi_out.clear();
        tzJet.dR_out.clear();
        tzJet.dR_ele_1_out.clear();
        tzJet.dR_ele_2_out.clear();
        tzJet.insideJet_out.clear();
        tzJet.insideJet_ele_1_out.clear();
        tzJet.insideJet_ele_2_out.clear();

        tzJet.zIdx_out = zIdx;
        tzJet.nJetinAwayRange_out = 0;

        // all the jets must go into correlation,
        // no jet should be skipped.
        for (int i=0; i<tJets.nref; ++i)
        {
            // cuts on jets will be applied during plotting
            float tmp_deta = getDETA(tdielectron.diEleEta->at(zIdx), tJets.jteta[i]);
            float tmp_dphi = getDPHI(tdielectron.diElePhi->at(zIdx), tJets.jtphi[i]);
            if (TMath::Abs(tmp_dphi) > tzJet.awayRange)
                tzJet.nJetinAwayRange_out++;
            float tmp_dR   = getDR(tdielectron.diEleEta->at(zIdx), tdielectron.diElePhi->at(zIdx), tJets.jteta[i], tJets.jtphi[i]);
            float tmp_dR_ele_1 = getDR(tdielectron.eleEta_1->at(zIdx), tdielectron.elePhi_1->at(zIdx), tJets.jteta[i], tJets.jtphi[i]);
            float tmp_dR_ele_2 = getDR(tdielectron.eleEta_2->at(zIdx), tdielectron.elePhi_2->at(zIdx), tJets.jteta[i], tJets.jtphi[i]);

            int tmp_insideJet;
            if(tmp_dR < tzJet.coneRange) tmp_insideJet = 1;
            else                         tmp_insideJet = 0;
            int tmp_insideJet_ele_1;
            if(tmp_dR_ele_1 < tzJet.coneRange) tmp_insideJet_ele_1 = 1;
            else                               tmp_insideJet_ele_1 = 0;
            int tmp_insideJet_ele_2;
            if(tmp_dR_ele_2 < tzJet.coneRange) tmp_insideJet_ele_2 = 1;
            else                               tmp_insideJet_ele_2 = 0;

            tzJet.jetIdx_out.push_back(i);
            if (tdielectron.diElePt->at(zIdx) > 0) {
                tzJet.xjz_out.push_back((float)tJets.jtpt[i]/tdielectron.diElePt->at(zIdx));
            }
            else {
                tzJet.xjz_out.push_back(-1);
            }
            tzJet.deta_out.push_back(tmp_deta);
            tzJet.dphi_out.push_back(tmp_dphi);
            tzJet.dR_out.push_back(tmp_dR);
            tzJet.dR_ele_1_out.push_back(tmp_dR_ele_1);
            tzJet.dR_ele_2_out.push_back(tmp_dR_ele_2);
            tzJet.insideJet_out.push_back(tmp_insideJet);
            tzJet.insideJet_ele_1_out.push_back(tmp_insideJet_ele_1);
            tzJet.insideJet_ele_2_out.push_back(tmp_insideJet_ele_2);
        }
    }
}

/*
 * in MB event mixing step, one Z boson is paired to jets from multiple events.
 * So, In the same event, zjet pairs will be made multiple times.
 * This function is used to make sure that vectors of "zJet" object is reset only once,
 * not at each pair making.
 */
void clearZJetPairs(ZJet &tzJet, int zIdx)
{
    tzJet.jetIdx_out.clear();
    tzJet.xjz_out.clear();
    tzJet.deta_out.clear();
    tzJet.dphi_out.clear();
    tzJet.dR_out.clear();
    tzJet.dR_ele_1_out.clear();
    tzJet.dR_ele_2_out.clear();
    tzJet.insideJet_out.clear();
    tzJet.insideJet_ele_1_out.clear();
    tzJet.insideJet_ele_2_out.clear();

    tzJet.zIdx_out = zIdx;
    tzJet.nJetinAwayRange_out = 0;
}

void makeZJetPairsMB(dielectron &tdielectron, Jets &tJets, ZJet &tzJet, int zIdx, bool use_diEle_out)
{
    if (use_diEle_out){
        // all the jets must go into correlation,
        // no jet should be skipped.
        for (int i=0; i<tJets.nref; ++i)
        {
            // cuts on jets will be applied during plotting
            float tmp_deta = getDETA(tdielectron.diEleEta_out.at(zIdx), tJets.jteta[i]);
            float tmp_dphi = getDPHI(tdielectron.diElePhi_out.at(zIdx), tJets.jtphi[i]);
            if (TMath::Abs(tmp_dphi) > tzJet.awayRange)
                tzJet.nJetinAwayRange_out++;
            float tmp_dR   = getDR(tdielectron.diEleEta_out.at(zIdx), tdielectron.diElePhi_out.at(zIdx), tJets.jteta[i], tJets.jtphi[i]);
            float tmp_dR_ele_1 = getDR(tdielectron.eleEta_1_out.at(zIdx), tdielectron.elePhi_1_out.at(zIdx), tJets.jteta[i], tJets.jtphi[i]);
            float tmp_dR_ele_2 = getDR(tdielectron.eleEta_2_out.at(zIdx), tdielectron.elePhi_2_out.at(zIdx), tJets.jteta[i], tJets.jtphi[i]);

            int tmp_insideJet;
            if(tmp_dR < tzJet.coneRange) tmp_insideJet = 1;
            else                         tmp_insideJet = 0;
            int tmp_insideJet_ele_1;
            if(tmp_dR_ele_1 < tzJet.coneRange) tmp_insideJet_ele_1 = 1;
            else                               tmp_insideJet_ele_1 = 0;
            int tmp_insideJet_ele_2;
            if(tmp_dR_ele_2 < tzJet.coneRange) tmp_insideJet_ele_2 = 1;
            else                               tmp_insideJet_ele_2 = 0;

            tzJet.jetIdx_out.push_back(i);
            if (tdielectron.diElePt_out.at(zIdx) > 0) {
                tzJet.xjz_out.push_back((float)tJets.jtpt[i]/tdielectron.diElePt_out.at(zIdx));
            }
            else {
                tzJet.xjz_out.push_back(-1);
            }
            tzJet.deta_out.push_back(tmp_deta);
            tzJet.dphi_out.push_back(tmp_dphi);
            tzJet.dR_out.push_back(tmp_dR);
            tzJet.dR_ele_1_out.push_back(tmp_dR_ele_1);
            tzJet.dR_ele_2_out.push_back(tmp_dR_ele_2);
            tzJet.insideJet_out.push_back(tmp_insideJet);
            tzJet.insideJet_ele_1_out.push_back(tmp_insideJet_ele_1);
            tzJet.insideJet_ele_2_out.push_back(tmp_insideJet_ele_2);
        }
    }
    else {
        // all the jets must go into correlation,
        // no jet should be skipped.
        for (int i=0; i<tJets.nref; ++i)
        {
            // cuts on jets will be applied during plotting
            float tmp_deta = getDETA(tdielectron.diEleEta->at(zIdx), tJets.jteta[i]);
            float tmp_dphi = getDPHI(tdielectron.diElePhi->at(zIdx), tJets.jtphi[i]);
            if (TMath::Abs(tmp_dphi) > tzJet.awayRange)
                tzJet.nJetinAwayRange_out++;
            float tmp_dR   = getDR(tdielectron.diEleEta->at(zIdx), tdielectron.diElePhi->at(zIdx), tJets.jteta[i], tJets.jtphi[i]);
            float tmp_dR_ele_1 = getDR(tdielectron.eleEta_1->at(zIdx), tdielectron.elePhi_1->at(zIdx), tJets.jteta[i], tJets.jtphi[i]);
            float tmp_dR_ele_2 = getDR(tdielectron.eleEta_2->at(zIdx), tdielectron.elePhi_2->at(zIdx), tJets.jteta[i], tJets.jtphi[i]);

            int tmp_insideJet;
            if(tmp_dR < tzJet.coneRange) tmp_insideJet = 1;
            else                         tmp_insideJet = 0;
            int tmp_insideJet_ele_1;
            if(tmp_dR_ele_1 < tzJet.coneRange) tmp_insideJet_ele_1 = 1;
            else                               tmp_insideJet_ele_1 = 0;
            int tmp_insideJet_ele_2;
            if(tmp_dR_ele_2 < tzJet.coneRange) tmp_insideJet_ele_2 = 1;
            else                               tmp_insideJet_ele_2 = 0;

            tzJet.jetIdx_out.push_back(i);
            if (tdielectron.diElePt->at(zIdx) > 0) {
                tzJet.xjz_out.push_back((float)tJets.jtpt[i]/tdielectron.diElePt->at(zIdx));
            }
            else {
                tzJet.xjz_out.push_back(-1);
            }
            tzJet.deta_out.push_back(tmp_deta);
            tzJet.dphi_out.push_back(tmp_dphi);
            tzJet.dR_out.push_back(tmp_dR);
            tzJet.dR_ele_1_out.push_back(tmp_dR_ele_1);
            tzJet.dR_ele_2_out.push_back(tmp_dR_ele_2);
            tzJet.insideJet_out.push_back(tmp_insideJet);
            tzJet.insideJet_ele_1_out.push_back(tmp_insideJet_ele_1);
            tzJet.insideJet_ele_2_out.push_back(tmp_insideJet_ele_2);
        }
    }
}

#endif /* TREEHEADERS_ZJETTREE_H_ */
