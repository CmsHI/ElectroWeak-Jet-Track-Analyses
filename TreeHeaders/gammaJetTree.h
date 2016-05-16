#ifndef TREEHEADERS_GAMMAJETTREE_H_
#define TREEHEADERS_GAMMAJETTREE_H_

#include <TTree.h>
#include <TMath.h>

#include <vector>

#include "../Plotting/commonUtility.h"
#include "ggHiNtuplizerTree.h"
#include "JetTree.h"

class GammaJet {
public :
    GammaJet(){
        awayRange = TMath::Pi() * 7./8.;
        coneRange = 0.4;

        jetIdx = 0;
        xjg = 0;
        deta = 0;
        dphi = 0;
        dR = 0;
        insideJet = 0;
        nJetinAwayRange = 0;
        jetID = 0;
    }
    ~GammaJet(){};
    void resetAwayRange() { awayRange = TMath::Pi()  * 7./8.; }
    void resetConeRange() { coneRange = 0.4 ; }
    void setupGammaJetTree(TTree *t);
    void branchGammaJetTree(TTree *t);
    void makeGammaJetPairs(ggHiNtuplizer &tggHiNtuplizer, Jets &tJets, int phoIdx, int smearingBranchIndex=-1);
    void clearGammaJetPairs(int phoIdx);
    void makeGammaJetPairsMB(ggHiNtuplizer &tggHiNtuplizer, Jets &tJets, int phoIdx);

    float awayRange;
    float coneRange;

    // Declaration of leaf types
     Int_t           phoIdx;
     Int_t           jetIdx1;       // index of leading jet which passed jetID
     Int_t           jetIdx2;       // index of subleading jet which passed jetID
     std::vector<int>     *jetIdx;
     std::vector<float>   *xjg;
     std::vector<float>   *deta;
     std::vector<float>   *dphi;
     std::vector<float>   *dR;
     std::vector<int>     *insideJet;
     Int_t           nJetinAwayRange;
     std::vector<int>     *jetID;

     // List of branches
     TBranch        *b_phoIdx;   //!
     TBranch        *b_jetIdx1;   //!
     TBranch        *b_jetIdx2;   //!
     TBranch        *b_jetIdx;   //!
     TBranch        *b_xjg;   //!
     TBranch        *b_deta;   //!
     TBranch        *b_dphi;   //!
     TBranch        *b_dR;   //!
     TBranch        *b_insideJet;   //!
     TBranch        *b_nJetinAwayRange;   //!
     TBranch        *b_jetID;   //!

     // list of objects to be used when creating a gammaJet Tree
     Int_t           phoIdx_out;
     Int_t           jetIdx1_out;
     Int_t           jetIdx2_out;
     std::vector<int>     jetIdx_out;
     std::vector<float>   xjg_out;
     std::vector<float>   deta_out;
     std::vector<float>   dphi_out;
     std::vector<float>   dR_out;
     std::vector<int>     insideJet_out;
     Int_t           nJetinAwayRange_out;
     std::vector<int>     jetID_out;

};

void GammaJet::setupGammaJetTree(TTree *t)
{

    if (t->GetBranch("phoIdx"))  t->SetBranchAddress("phoIdx", &phoIdx, &b_phoIdx);
    if (t->GetBranch("jetIdx1"))  t->SetBranchAddress("jetIdx1", &jetIdx1, &b_jetIdx1);
    if (t->GetBranch("jetIdx2"))  t->SetBranchAddress("jetIdx1", &jetIdx2, &b_jetIdx2);
    if (t->GetBranch("jetIdx"))  t->SetBranchAddress("jetIdx", &jetIdx, &b_jetIdx);
    if (t->GetBranch("xjg"))  t->SetBranchAddress("xjg", &xjg, &b_xjg);
    if (t->GetBranch("deta"))  t->SetBranchAddress("deta", &deta, &b_deta);
    if (t->GetBranch("dphi"))  t->SetBranchAddress("dphi", &dphi, &b_dphi);
    if (t->GetBranch("dR"))  t->SetBranchAddress("dR", &dR, &b_dR);
    if (t->GetBranch("insideJet"))  t->SetBranchAddress("insideJet", &insideJet, &b_insideJet);
    if (t->GetBranch("nJetinAwayRange"))  t->SetBranchAddress("nJetinAwayRange", &nJetinAwayRange, &b_nJetinAwayRange);
    if (t->GetBranch("jetID"))  t->SetBranchAddress("jetID", &jetID, &b_jetID);
}

void GammaJet::branchGammaJetTree(TTree *t)
{
    t->Branch("phoIdx", &phoIdx_out);
    t->Branch("jetIdx1", &jetIdx1_out);
    t->Branch("jetIdx2", &jetIdx2_out);
    t->Branch("jetIdx", &jetIdx_out);
    t->Branch("xjg", &xjg_out);
    t->Branch("deta", &deta_out);
    t->Branch("dphi", &dphi_out);
    t->Branch("dR", &dR_out);
    t->Branch("insideJet", &insideJet_out);
    t->Branch("nJetinAwayRange", &nJetinAwayRange_out);
    t->Branch("jetID", &jetID_out);
}

void GammaJet::makeGammaJetPairs(ggHiNtuplizer &tggHiNtuplizer, Jets &tJets, int phoIdx, int smearingBranchIndex)
{
    jetIdx_out.clear();
    xjg_out.clear();
    deta_out.clear();
    dphi_out.clear();
    dR_out.clear();
    insideJet_out.clear();
    jetID_out.clear();

    phoIdx_out = phoIdx;
    nJetinAwayRange_out = 0;

    jetIdx1_out = -1;
    jetIdx2_out = -1;
    double jetIdx1_pt = -1;
    double jetIdx2_pt = -1;
    // all the jets must go into correlation,
    // no jet should be skipped.
    for (int i=0; i<tJets.nref; ++i)
    {
        float jtpt;
        float jtphi;
        switch(smearingBranchIndex){
        case 0:
            jtpt = tJets.jtpt[i];
            jtphi = tJets.jtphi[i];
            break;
        case 1:
            jtpt = tJets.jtpt_smeared_0_30[i];
            jtphi = tJets.jtphi_smeared_0_30[i];
            break;
        case 2:
            jtpt = tJets.jtpt_smeared_30_100[i];
            jtphi = tJets.jtphi_smeared_0_30[i];
            break;
        case 3:
            jtpt = tJets.jtpt_smeared_0_10[i];
            jtphi = tJets.jtphi_smeared_0_10[i];
            break;
        case 4:
            jtpt = tJets.jtpt_smeared_10_30[i];
            jtphi = tJets.jtphi_smeared_10_30[i];
            break;
        case 5:
            jtpt = tJets.jtpt_smeared_30_50[i];
            jtphi = tJets.jtphi_smeared_30_50[i];
            break;
        case 6:
            jtpt = tJets.jtpt_smeared_50_100[i];
            jtphi = tJets.jtphi_smeared_50_100[i];
            break;
        case 7:
            jtpt = tJets.jtpt_smeared_sys[i];
            jtphi = tJets.jtphi_smeared_sys[i];
            break;
        default:
            std::cout << "smearingBranchIndex set incorrectly in gammaJetTree" << std::endl;
            jtpt = tJets.jtpt[i];
            jtphi = tJets.jtphi[i];
            break;
        }
        
        // cuts on jets will be applied during plotting
        float tmp_deta = getDETA(tggHiNtuplizer.phoEta->at(phoIdx), tJets.jteta[i]);
        float tmp_dphi = getDPHI(tggHiNtuplizer.phoPhi->at(phoIdx), jtphi);
        if (TMath::Abs(tmp_dphi) > awayRange)
            nJetinAwayRange_out++;
        float tmp_dR   = getDR(tggHiNtuplizer.phoEta->at(phoIdx), tggHiNtuplizer.phoPhi->at(phoIdx), tJets.jteta[i], jtphi);

        int tmp_insideJet;
        if(tmp_dR < coneRange) tmp_insideJet = 1;
        else                             tmp_insideJet = 0;

        int tmp_jetID = 0;
        if (tJets.rawpt[i] > 0) {
            if (    tJets.neutralSum[i]/tJets.rawpt[i] < 0.9
                &&  tJets.chargedSum[i]/tJets.rawpt[i] > 0.01
                && (tJets.chargedN[i] + tJets.photonN[i] + tJets.neutralN[i] + tJets.eN[i] + tJets.muN[i]) > 0
                &&  tJets.chargedMax[i]/tJets.rawpt[i] < 0.99
                &&  tJets.photonSum[i]/tJets.rawpt[i]  < 0.99
                &&  tJets.eSum[i]/tJets.rawpt[i]       < 0.99) {
                    tmp_jetID = 1;
            }
        }

        if (jtpt > jetIdx1_pt && tmp_jetID == 1) {
            // current leading jet becomes subleading jet
            jetIdx2_pt = jetIdx1_pt;
            jetIdx2_out = jetIdx1_out;

            jetIdx1_pt = jtpt;
            jetIdx1_out = i;
        }
        else if (jtpt > jetIdx2_pt && tmp_jetID == 1) {
            jetIdx2_pt = jtpt;
            jetIdx2_out = i;
        }

        jetIdx_out.push_back(i);
        if (tggHiNtuplizer.phoEt->at(phoIdx) > 0) {
            xjg_out.push_back((float)jtpt/tggHiNtuplizer.phoEt->at(phoIdx));
        }
        else {
            xjg_out.push_back(-1);
        }
        deta_out.push_back(tmp_deta);
        dphi_out.push_back(tmp_dphi);
        dR_out.push_back(tmp_dR);
        insideJet_out.push_back(tmp_insideJet);
        jetID_out.push_back(tmp_jetID);
    }
}

/*
 * in MB event mixing step, one photon is paired to jets from multiple events.
 * So, In the same event, gammajet pairs will be made multiple times.
 * This function is used to make sure that vectors of "gammaJet" object is reset only once,
 * not at each pair making.
 */
void GammaJet::clearGammaJetPairs(int phoIdx)
{
    jetIdx_out.clear();
    xjg_out.clear();
    deta_out.clear();
    dphi_out.clear();
    dR_out.clear();
    insideJet_out.clear();
    jetID_out.clear();

    phoIdx_out = phoIdx;
    nJetinAwayRange_out = 0;
}

void GammaJet::makeGammaJetPairsMB(ggHiNtuplizer &tggHiNtuplizer, Jets &tJets, int phoIdx)
{
    jetIdx1_out = -1;
    jetIdx2_out = -1;
    double jetIdx1_pt = -1;
    double jetIdx2_pt = -1;
    // all the jets must go into correlation,
    // no jet should be skipped.
    for (int i=0; i<tJets.nref; ++i)
    {
        // cuts on jets will be applied during plotting
        float tmp_deta = getDETA(tggHiNtuplizer.phoEta->at(phoIdx), tJets.jteta[i]);
        float tmp_dphi = getDPHI(tggHiNtuplizer.phoPhi->at(phoIdx), tJets.jtphi[i]);
        if (TMath::Abs(tmp_dphi) > awayRange)
            nJetinAwayRange_out++;
        float tmp_dR   = getDR(tggHiNtuplizer.phoEta->at(phoIdx), tggHiNtuplizer.phoPhi->at(phoIdx), tJets.jteta[i], tJets.jtphi[i]);

        int tmp_insideJet;
        if(tmp_dR < coneRange) tmp_insideJet = 1;
        else                             tmp_insideJet = 0;

        int tmp_jetID = 0;
        if (tJets.rawpt[i] > 0) {
            if (    tJets.neutralSum[i]/tJets.rawpt[i] < 0.9
                &&  tJets.chargedSum[i]/tJets.rawpt[i] > 0.01
                && (tJets.chargedN[i] + tJets.photonN[i] + tJets.neutralN[i] + tJets.eN[i] + tJets.muN[i]) > 0
                &&  tJets.chargedMax[i]/tJets.rawpt[i] < 0.99
                &&  tJets.photonSum[i]/tJets.rawpt[i]  < 0.99
                &&  tJets.eSum[i]/tJets.rawpt[i]       < 0.99) {
                    tmp_jetID = 1;
            }
        }

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
        if (tggHiNtuplizer.phoEt->at(phoIdx) > 0) {
            xjg_out.push_back((float)tJets.jtpt[i]/tggHiNtuplizer.phoEt->at(phoIdx));
        }
        else {
            xjg_out.push_back(-1);
        }
        deta_out.push_back(tmp_deta);
        dphi_out.push_back(tmp_dphi);
        dR_out.push_back(tmp_dR);
        insideJet_out.push_back(tmp_insideJet);
        jetID_out.push_back(tmp_jetID);
    }
}

#endif /* TREEHEADERS_GAMMAJETTREE_H_ */
