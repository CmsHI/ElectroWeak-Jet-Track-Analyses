#ifndef TREEHEADERS_GAMMAJETTREE_H_
#define TREEHEADERS_GAMMAJETTREE_H_

#include <TTree.h>
#include <TMath.h>

#include <vector>

#include "../Plotting/commonUtility.h"
#include "SetupJetTree.h"
#include "ggHiNtuplizerTree.h"

class GammaJet {
public :
    GammaJet(){
        awayRange = PI * 7./8.;
        coneRange = 0.4;

        jetIdx = 0;
        xjg = 0;
        deta = 0;
        dphi = 0;
        dR = 0;
        insideJet = 0;
        nJetinAwayRange = 0;
    }
    ~GammaJet(){};
    void resetAwayRange() { awayRange = PI * 7./8.; }
    void resetConeRange() { coneRange = 0.4 ; }

    float awayRange;
    float coneRange;

    // Declaration of leaf types
     Int_t           phoIdx;
     std::vector<int>     *jetIdx;
     std::vector<float>   *xjg;
     std::vector<float>   *deta;
     std::vector<float>   *dphi;
     std::vector<float>   *dR;
     std::vector<int>     *insideJet;
     Int_t           nJetinAwayRange;

     // List of branches
     TBranch        *b_phoIdx;   //!
     TBranch        *b_jetIdx;   //!
     TBranch        *b_xjg;   //!
     TBranch        *b_deta;   //!
     TBranch        *b_dphi;   //!
     TBranch        *b_dR;   //!
     TBranch        *b_insideJet;   //!
     TBranch        *b_nJetin7over8;   //!

     // list of objects to be used when creating a gammaJet Tree
     Int_t           phoIdx_out;
     std::vector<int>     jetIdx_out;
     std::vector<float>   xjg_out;
     std::vector<float>   deta_out;
     std::vector<float>   dphi_out;
     std::vector<float>   dR_out;
     std::vector<int>     insideJet_out;
     Int_t           nJetinAwayRange_out;

};

void setupGammaJetTree(TTree *t,GammaJet &tgammaJet);
void branchGammaJetTree(TTree *t,GammaJet &tgammaJet);
void makeGammaJetPairs(ggHiNtuplizer &tggHiNtuplizer, Jets &tJets, GammaJet &tgammaJet, int phoIdx);
void clearGammaJetPairs(GammaJet &tgammaJet, int phoIdx);
void makeGammaJetPairsMB(ggHiNtuplizer &tggHiNtuplizer, Jets &tJets, GammaJet &tgammaJet, int phoIdx);

void setupGammaJetTree(TTree *t,GammaJet &tgammaJet)
{

    if (t->GetBranch("phoIdx"))  t->SetBranchAddress("phoIdx", &tgammaJet.phoIdx, &tgammaJet.b_phoIdx);
    if (t->GetBranch("jetIdx"))  t->SetBranchAddress("jetIdx", &tgammaJet.jetIdx, &tgammaJet.b_jetIdx);
    if (t->GetBranch("xjg"))  t->SetBranchAddress("xjg", &tgammaJet.xjg, &tgammaJet.b_xjg);
    if (t->GetBranch("deta"))  t->SetBranchAddress("deta", &tgammaJet.deta, &tgammaJet.b_deta);
    if (t->GetBranch("dphi"))  t->SetBranchAddress("dphi", &tgammaJet.dphi, &tgammaJet.b_dphi);
    if (t->GetBranch("dR"))  t->SetBranchAddress("dR", &tgammaJet.dR, &tgammaJet.b_dR);
    if (t->GetBranch("insideJet"))  t->SetBranchAddress("insideJet", &tgammaJet.insideJet, &tgammaJet.b_insideJet);
    if (t->GetBranch("nJetinAwayRange"))  t->SetBranchAddress("nJetinAwayRange", &tgammaJet.nJetinAwayRange, &tgammaJet.b_nJetin7over8);
}

void branchGammaJetTree(TTree *t,GammaJet &tgammaJet)
{
    t->Branch("phoIdx", &tgammaJet.phoIdx_out);
    t->Branch("jetIdx", &tgammaJet.jetIdx_out);
    t->Branch("xjg", &tgammaJet.xjg_out);
    t->Branch("deta", &tgammaJet.deta_out);
    t->Branch("dphi", &tgammaJet.dphi_out);
    t->Branch("dR", &tgammaJet.dR_out);
    t->Branch("insideJet", &tgammaJet.insideJet_out);
    t->Branch("nJetinAwayRange", &tgammaJet.nJetinAwayRange_out);
}

void makeGammaJetPairs(ggHiNtuplizer &tggHiNtuplizer, Jets &tJets, GammaJet &tgammaJet, int phoIdx)
{
    tgammaJet.jetIdx_out.clear();
    tgammaJet.xjg_out.clear();
    tgammaJet.deta_out.clear();
    tgammaJet.dphi_out.clear();
    tgammaJet.dR_out.clear();
    tgammaJet.insideJet_out.clear();

    tgammaJet.phoIdx_out = phoIdx;
    tgammaJet.nJetinAwayRange_out = 0;

    // all the jets must go into correlation,
    // no jet should be skipped.
    for (int i=0; i<tJets.nref; ++i)
    {
        // cuts on jets will be applied during plotting
        float tmp_deta = getDETA(tggHiNtuplizer.phoEta->at(phoIdx), tJets.jteta[i]);
        float tmp_dphi = getDPHI(tggHiNtuplizer.phoPhi->at(phoIdx), tJets.jtphi[i]);
        if (TMath::Abs(tmp_dphi) > tgammaJet.awayRange)
            tgammaJet.nJetinAwayRange_out++;
        float tmp_dR   = getDR(tggHiNtuplizer.phoEta->at(phoIdx), tggHiNtuplizer.phoPhi->at(phoIdx), tJets.jteta[i], tJets.jtphi[i]);

        int tmp_insideJet;
        if(tmp_dR < tgammaJet.coneRange) tmp_insideJet = 1;
        else                             tmp_insideJet = 0;

        tgammaJet.jetIdx_out.push_back(i);
        if (tggHiNtuplizer.phoEt->at(phoIdx) > 0) {
            tgammaJet.xjg_out.push_back((float)tJets.jtpt[i]/tggHiNtuplizer.phoEt->at(phoIdx));
        }
        else {
            tgammaJet.xjg_out.push_back(-1);
        }
        tgammaJet.deta_out.push_back(tmp_deta);
        tgammaJet.dphi_out.push_back(tmp_dphi);
        tgammaJet.dR_out.push_back(tmp_dR);
        tgammaJet.insideJet_out.push_back(tmp_insideJet);
    }
}

/*
 * in MB event mixing step, one photon is paired to jets from multiple events.
 * So, In the same event, gammajet pairs will be made multiple times.
 * This function is used to make sure that vectors of "gammaJet" object is reset only once,
 * not at each pair making.
 */
void clearGammaJetPairs(GammaJet &tgammaJet, int phoIdx)
{
    tgammaJet.jetIdx_out.clear();
    tgammaJet.xjg_out.clear();
    tgammaJet.deta_out.clear();
    tgammaJet.dphi_out.clear();
    tgammaJet.dR_out.clear();
    tgammaJet.insideJet_out.clear();

    tgammaJet.phoIdx_out = phoIdx;
    tgammaJet.nJetinAwayRange_out = 0;
}

void makeGammaJetPairsMB(ggHiNtuplizer &tggHiNtuplizer, Jets &tJets, GammaJet &tgammaJet, int phoIdx)
{
    // all the jets must go into correlation,
    // no jet should be skipped.
    for (int i=0; i<tJets.nref; ++i)
    {
        // cuts on jets will be applied during plotting
        float tmp_deta = getDETA(tggHiNtuplizer.phoEta->at(phoIdx), tJets.jteta[i]);
        float tmp_dphi = getDPHI(tggHiNtuplizer.phoPhi->at(phoIdx), tJets.jtphi[i]);
        if (TMath::Abs(tmp_dphi) > tgammaJet.awayRange)
            tgammaJet.nJetinAwayRange_out++;
        float tmp_dR   = getDR(tggHiNtuplizer.phoEta->at(phoIdx), tggHiNtuplizer.phoPhi->at(phoIdx), tJets.jteta[i], tJets.jtphi[i]);

        int tmp_insideJet;
        if(tmp_dR < tgammaJet.coneRange) tmp_insideJet = 1;
        else                             tmp_insideJet = 0;

        tgammaJet.jetIdx_out.push_back(i);
        if (tggHiNtuplizer.phoEt->at(phoIdx) > 0) {
            tgammaJet.xjg_out.push_back((float)tJets.jtpt[i]/tggHiNtuplizer.phoEt->at(phoIdx));
        }
        else {
            tgammaJet.xjg_out.push_back(-1);
        }
        tgammaJet.deta_out.push_back(tmp_deta);
        tgammaJet.dphi_out.push_back(tmp_dphi);
        tgammaJet.dR_out.push_back(tmp_dR);
        tgammaJet.insideJet_out.push_back(tmp_insideJet);
    }
}

#endif /* TREEHEADERS_GAMMAJETTREE_H_ */
