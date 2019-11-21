#ifndef TREEHEADERS_TRACKSKIMTREE_H_
#define TREEHEADERS_TRACKSKIMTREE_H_

#include <TTree.h>
#include <TMath.h>
#include <TVector3.h>

#include <vector>

class trackSkim {
public :
    trackSkim(){
        p_trkPt = 0;
        p_trkEta = 0;
        p_trkPhi = 0;
        p_trkCharge = 0;
        p_trkPtError = 0;
        p_trkNHit = 0;
        p_trkNlayer = 0;
        p_highPurity = 0;
        p_trkChi2 = 0;
        p_trkNdof = 0;
        p_trkDxy1 = 0;
        p_trkDxyError1 = 0;
        p_trkDz1 = 0;
        p_trkDzError1 = 0;
        p_trkAlgo = 0;
        p_trkMVA = 0;
        p_pfType = 0;
        p_pfCandPt = 0;
        p_pfEcal = 0;
        p_pfHcal = 0;
        p_trkNVtx = 0;
        p_trkWeight = 0;

        p_pt = 0;
        p_eta = 0;
        p_phi = 0;
        p_pdg = 0;
        p_chg = 0;
        p_sube = 0;

        p_trkPt_mix = 0;
        p_trkEta_mix = 0;
        p_trkPhi_mix = 0;
        p_trkCharge_mix = 0;
        p_trkPtError_mix = 0;
        p_trkNHit_mix = 0;
        p_trkNlayer_mix = 0;
        p_highPurity_mix = 0;
        p_trkChi2_mix = 0;
        p_trkNdof_mix = 0;
        p_trkDxy1_mix = 0;
        p_trkDxyError1_mix = 0;
        p_trkDz1_mix = 0;
        p_trkDzError1_mix = 0;
        p_trkAlgo_mix = 0;
        p_trkMVA_mix = 0;
        p_pfType_mix = 0;
        p_pfCandPt_mix = 0;
        p_pfEcal_mix = 0;
        p_pfHcal_mix = 0;
        p_trkNVtx_mix = 0;
        p_trkWeight_mix = 0;
        p_evttrk_mix = 0;

        p_pt_mix = 0;
        p_eta_mix = 0;
        p_phi_mix = 0;
        p_pdg_mix = 0;
        p_chg_mix = 0;
        p_sube_mix = 0;
        p_evtgen_mix = 0;
    }
    ~trackSkim(){};
    void setupTreeForReading(TTree *t);
    void setupTreeForWriting(TTree *t);
    void clearEvent();
    void clearMixEvent();

    // Declaration of leaf types

    // to be used when writing
    // tracks
    int nTrk;
    int nVtx;
    int maxPtVtx;
    int maxMultVtx;

    std::vector<float>   trkPt;
    std::vector<float>   trkEta;
    std::vector<float>   trkPhi;
    std::vector<int>     trkCharge;
    std::vector<float>   trkPtError;
    std::vector<unsigned char> trkNHit;
    std::vector<unsigned char> trkNlayer;
    std::vector<bool>    highPurity;
    std::vector<float>   trkChi2;
    std::vector<unsigned char> trkNdof;
    std::vector<float>   trkDxy1;
    std::vector<float>   trkDxyError1;
    std::vector<float>   trkDz1;
    std::vector<float>   trkDzError1;
    std::vector<unsigned char> trkAlgo;
    std::vector<float>   trkMVA;
    std::vector<int>     pfType;
    std::vector<float>   pfCandPt;
    std::vector<float>   pfEcal;
    std::vector<float>   pfHcal;
    std::vector<unsigned char> trkNVtx;
    std::vector<float>   trkWeight;

    // gen particles
    Int_t mult;
    std::vector<float>   pt;
    std::vector<float>   eta;
    std::vector<float>   phi;
    std::vector<int>     pdg;
    std::vector<int>     chg;
    std::vector<int>     sube;

    // objects from mixed event
    Int_t nTrk_mix;
    std::vector<float>   trkPt_mix;
    std::vector<float>   trkEta_mix;
    std::vector<float>   trkPhi_mix;
    std::vector<int>     trkCharge_mix;
    std::vector<float>   trkPtError_mix;
    std::vector<unsigned char> trkNHit_mix;
    std::vector<unsigned char> trkNlayer_mix;
    std::vector<bool>    highPurity_mix;
    std::vector<float>   trkChi2_mix;
    std::vector<unsigned char> trkNdof_mix;
    std::vector<float>   trkDxy1_mix;
    std::vector<float>   trkDxyError1_mix;
    std::vector<float>   trkDz1_mix;
    std::vector<float>   trkDzError1_mix;
    std::vector<unsigned char> trkAlgo_mix;
    std::vector<float>   trkMVA_mix;
    std::vector<int>     pfType_mix;
    std::vector<float>   pfCandPt_mix;
    std::vector<float>   pfEcal_mix;
    std::vector<float>   pfHcal_mix;
    std::vector<unsigned char> trkNVtx_mix;
    std::vector<float>   trkWeight_mix;
    std::vector<int>     evttrk_mix;

    Int_t mult_mix;
    std::vector<float>   pt_mix;
    std::vector<float>   eta_mix;
    std::vector<float>   phi_mix;
    std::vector<int>     pdg_mix;
    std::vector<int>     chg_mix;
    std::vector<int>     sube_mix;
    std::vector<int>     evtgen_mix;

    // pointer for vectors (to be used when reading)
    std::vector<float> *p_trkPt;
    std::vector<float> *p_trkEta;
    std::vector<float> *p_trkPhi;
    std::vector<int> *p_trkCharge;
    std::vector<float> *p_trkPtError;
    std::vector<unsigned char> *p_trkNHit;
    std::vector<unsigned char> *p_trkNlayer;
    std::vector<bool>    *p_highPurity;
    std::vector<float> *p_trkChi2;
    std::vector<unsigned char> *p_trkNdof;
    std::vector<float> *p_trkDxy1;
    std::vector<float> *p_trkDxyError1;
    std::vector<float> *p_trkDz1;
    std::vector<float> *p_trkDzError1;
    std::vector<unsigned char> *p_trkAlgo;
    std::vector<float> *p_trkMVA;
    std::vector<int>   *p_pfType;
    std::vector<float> *p_pfCandPt;
    std::vector<float> *p_pfEcal;
    std::vector<float> *p_pfHcal;
    std::vector<unsigned char> *p_trkNVtx;
    std::vector<float> *p_trkWeight;

    // gen particles
    std::vector<float> *p_pt;
    std::vector<float> *p_eta;
    std::vector<float> *p_phi;
    std::vector<int> *p_pdg;
    std::vector<int> *p_chg;
    std::vector<int> *p_sube;

    // objects from mixed event
    std::vector<float> *p_trkPt_mix;
    std::vector<float> *p_trkEta_mix;
    std::vector<float> *p_trkPhi_mix;
    std::vector<int> *p_trkCharge_mix;
    std::vector<float> *p_trkPtError_mix;
    std::vector<unsigned char> *p_trkNHit_mix;
    std::vector<unsigned char> *p_trkNlayer_mix;
    std::vector<bool>    *p_highPurity_mix;
    std::vector<float> *p_trkChi2_mix;
    std::vector<unsigned char> *p_trkNdof_mix;
    std::vector<float> *p_trkDxy1_mix;
    std::vector<float> *p_trkDxyError1_mix;
    std::vector<float> *p_trkDz1_mix;
    std::vector<float> *p_trkDzError1_mix;
    std::vector<unsigned char> *p_trkAlgo_mix;
    std::vector<float> *p_trkMVA_mix;
    std::vector<int>   *p_pfType_mix;
    std::vector<float> *p_pfCandPt_mix;
    std::vector<float> *p_pfEcal_mix;
    std::vector<float> *p_pfHcal_mix;
    std::vector<unsigned char> *p_trkNVtx_mix;
    std::vector<float> *p_trkWeight_mix;
    std::vector<int> *p_evttrk_mix;

    std::vector<float> *p_pt_mix;
    std::vector<float> *p_eta_mix;
    std::vector<float> *p_phi_mix;
    std::vector<int> *p_pdg_mix;
    std::vector<int> *p_chg_mix;
    std::vector<int> *p_sube_mix;
    std::vector<int> *p_evtgen_mix;

     // List of branches
    TBranch *b_nTrk;
    TBranch *b_nVtx;
    TBranch *b_maxPtVtx;
    TBranch *b_maxMultVtx;

    TBranch *b_trkPt;
    TBranch *b_trkEta;
    TBranch *b_trkPhi;
    TBranch *b_trkCharge;
    TBranch *b_trkPtError;
    TBranch *b_trkNHit;
    TBranch *b_trkNlayer;
    TBranch *b_highPurity;
    TBranch *b_trkChi2;
    TBranch *b_trkNdof;
    TBranch *b_trkDxy1;
    TBranch *b_trkDxyError1;
    TBranch *b_trkDz1;
    TBranch *b_trkDzError1;
    TBranch *b_trkAlgo;
    TBranch *b_trkMVA;
    TBranch *b_pfType;
    TBranch *b_pfCandPt;
    TBranch *b_pfEcal;
    TBranch *b_pfHcal;
    TBranch *b_trkNVtx;
    TBranch *b_trkWeight;

    TBranch *b_mult;
    TBranch *b_pt;
    TBranch *b_eta;
    TBranch *b_phi;
    TBranch *b_pdg;
    TBranch *b_chg;
    TBranch *b_sube;

    TBranch *b_nTrk_mix;
    TBranch *b_trkPt_mix;
    TBranch *b_trkEta_mix;
    TBranch *b_trkPhi_mix;
    TBranch *b_trkCharge_mix;
    TBranch *b_trkPtError_mix;
    TBranch *b_trkNHit_mix;
    TBranch *b_trkNlayer_mix;
    TBranch *b_highPurity_mix;
    TBranch *b_trkChi2_mix;
    TBranch *b_trkNdof_mix;
    TBranch *b_trkDxy1_mix;
    TBranch *b_trkDxyError1_mix;
    TBranch *b_trkDz1_mix;
    TBranch *b_trkDzError1_mix;
    TBranch *b_trkAlgo_mix;
    TBranch *b_trkMVA_mix;
    TBranch *b_pfType_mix;
    TBranch *b_pfCandPt_mix;
    TBranch *b_pfEcal_mix;
    TBranch *b_pfHcal_mix;
    TBranch *b_trkNVtx_mix;
    TBranch *b_trkWeight_mix;
    TBranch *b_evttrk_mix;

    TBranch *b_mult_mix;
    TBranch *b_pt_mix;
    TBranch *b_eta_mix;
    TBranch *b_phi_mix;
    TBranch *b_pdg_mix;
    TBranch *b_chg_mix;
    TBranch *b_sube_mix;
    TBranch *b_evtgen_mix;
};

void trackSkim::setupTreeForReading(TTree *t)
{
    b_nTrk = 0;
    b_nVtx = 0;
    b_maxPtVtx = 0;
    b_maxMultVtx = 0;

    b_trkPt = 0;
    b_trkEta = 0;
    b_trkPhi = 0;
    b_trkCharge = 0;
    b_trkPtError = 0;
    b_trkNHit = 0;
    b_trkNlayer = 0;
    b_highPurity = 0;
    b_trkChi2 = 0;
    b_trkNdof = 0;
    b_trkDxy1 = 0;
    b_trkDxyError1 = 0;
    b_trkDz1 = 0;
    b_trkDzError1 = 0;
    b_trkAlgo = 0;
    b_trkMVA = 0;
    b_pfType = 0;
    b_pfCandPt = 0;
    b_pfEcal = 0;
    b_pfHcal = 0;
    b_trkNVtx = 0;
    b_trkWeight = 0;

    b_mult = 0;
    b_pt = 0;
    b_eta = 0;
    b_phi = 0;
    b_pdg = 0;
    b_chg = 0;
    b_sube = 0;

    b_nTrk_mix = 0;
    b_trkPt_mix = 0;
    b_trkEta_mix = 0;
    b_trkPhi_mix = 0;
    b_trkCharge_mix = 0;
    b_trkPtError_mix = 0;
    b_trkNHit_mix = 0;
    b_trkNlayer_mix = 0;
    b_highPurity_mix = 0;
    b_trkChi2_mix = 0;
    b_trkNdof_mix = 0;
    b_trkDxy1_mix = 0;
    b_trkDxyError1_mix = 0;
    b_trkDz1_mix = 0;
    b_trkDzError1_mix = 0;
    b_trkAlgo_mix = 0;
    b_trkMVA_mix = 0;
    b_pfType_mix = 0;
    b_pfCandPt_mix = 0;
    b_pfEcal_mix = 0;
    b_pfHcal_mix = 0;
    b_trkNVtx_mix = 0;
    b_trkWeight_mix = 0;
    b_evttrk_mix = 0;

    b_mult_mix = 0;
    b_pt_mix = 0;
    b_eta_mix = 0;
    b_phi_mix = 0;
    b_pdg_mix = 0;
    b_chg_mix = 0;
    b_sube_mix = 0;
    b_evtgen_mix = 0;

    if (t->GetBranch("nTrk"))  t->SetBranchAddress("nTrk", &nTrk, &b_nTrk);
    if (t->GetBranch("nVtx"))  t->SetBranchAddress("nVtx", &nVtx, &b_nVtx);
    if (t->GetBranch("maxPtVtx"))  t->SetBranchAddress("maxPtVtx", &maxPtVtx, &b_maxPtVtx);
    if (t->GetBranch("maxMultVtx"))  t->SetBranchAddress("maxMultVtx", &maxMultVtx, &b_maxMultVtx);

    if (t->GetBranch("trkPt"))  t->SetBranchAddress("trkPt", &p_trkPt, &b_trkPt);
    if (t->GetBranch("trkEta"))  t->SetBranchAddress("trkEta", &p_trkEta, &b_trkEta);
    if (t->GetBranch("trkPhi"))  t->SetBranchAddress("trkPhi", &p_trkPhi, &b_trkPhi);
    if (t->GetBranch("trkCharge"))  t->SetBranchAddress("trkCharge", &p_trkCharge, &b_trkCharge);
    if (t->GetBranch("trkPtError"))  t->SetBranchAddress("trkPtError", &p_trkPtError, &b_trkPtError);
    if (t->GetBranch("trkNHit"))  t->SetBranchAddress("trkNHit", &p_trkNHit, &b_trkNHit);
    if (t->GetBranch("trkNlayer"))  t->SetBranchAddress("trkNlayer", &p_trkNlayer, &b_trkNlayer);
    if (t->GetBranch("highPurity"))  t->SetBranchAddress("highPurity", &p_highPurity, &b_highPurity);
    if (t->GetBranch("trkChi2"))  t->SetBranchAddress("trkChi2", &p_trkChi2, &b_trkChi2);
    if (t->GetBranch("trkNdof"))  t->SetBranchAddress("trkNdof", &p_trkNdof, &b_trkNdof);
    if (t->GetBranch("trkDxy1"))  t->SetBranchAddress("trkDxy1", &p_trkDxy1, &b_trkDxy1);
    if (t->GetBranch("trkDxyError1"))  t->SetBranchAddress("trkDxyError1", &p_trkDxyError1, &b_trkDxyError1);
    if (t->GetBranch("trkDz1"))  t->SetBranchAddress("trkDz1", &p_trkDz1, &b_trkDz1);
    if (t->GetBranch("trkDzError1"))  t->SetBranchAddress("trkDzError1", &p_trkDzError1, &b_trkDzError1);
    if (t->GetBranch("trkAlgo"))  t->SetBranchAddress("trkAlgo", &p_trkAlgo, &b_trkAlgo);
    if (t->GetBranch("trkMVA"))  t->SetBranchAddress("trkMVA", &p_trkMVA, &b_trkMVA);
    if (t->GetBranch("pfType"))  t->SetBranchAddress("pfType", &p_pfType, &b_pfType);
    if (t->GetBranch("pfCandPt"))  t->SetBranchAddress("pfCandPt", &p_pfCandPt, &b_pfCandPt);
    if (t->GetBranch("pfEcal"))  t->SetBranchAddress("pfEcal", &p_pfEcal, &b_pfEcal);
    if (t->GetBranch("pfHcal"))  t->SetBranchAddress("pfHcal", &p_pfHcal, &b_pfHcal);
    if (t->GetBranch("trkNVtx"))  t->SetBranchAddress("trkNVtx", &p_trkNVtx, &b_trkNVtx);
    if (t->GetBranch("trkWeight"))  t->SetBranchAddress("trkWeight", &p_trkWeight, &b_trkWeight);

    if (t->GetBranch("mult"))  t->SetBranchAddress("mult", &mult, &b_mult);
    if (t->GetBranch("pt"))  t->SetBranchAddress("pt", &p_pt, &b_pt);
    if (t->GetBranch("eta"))  t->SetBranchAddress("eta", &p_eta, &b_eta);
    if (t->GetBranch("phi"))  t->SetBranchAddress("phi", &p_phi, &b_phi);
    if (t->GetBranch("pdg"))  t->SetBranchAddress("pdg", &p_pdg, &b_pdg);
    if (t->GetBranch("chg"))  t->SetBranchAddress("chg", &p_chg, &b_chg);
    if (t->GetBranch("sube"))  t->SetBranchAddress("sube", &p_sube, &b_sube);

    if (t->GetBranch("nTrk_mix"))  t->SetBranchAddress("nTrk_mix", &nTrk_mix, &b_nTrk_mix);
    if (t->GetBranch("trkPt_mix"))  t->SetBranchAddress("trkPt_mix", &p_trkPt_mix, &b_trkPt_mix);
    if (t->GetBranch("trkEta_mix"))  t->SetBranchAddress("trkEta_mix", &p_trkEta_mix, &b_trkEta_mix);
    if (t->GetBranch("trkPhi_mix"))  t->SetBranchAddress("trkPhi_mix", &p_trkPhi_mix, &b_trkPhi_mix);
    if (t->GetBranch("trkCharge_mix"))  t->SetBranchAddress("trkCharge_mix", &p_trkCharge_mix, &b_trkCharge_mix);
    if (t->GetBranch("trkPtError_mix"))  t->SetBranchAddress("trkPtError_mix", &p_trkPtError_mix, &b_trkPtError_mix);
    if (t->GetBranch("trkNHit_mix"))  t->SetBranchAddress("trkNHit_mix", &p_trkNHit_mix, &b_trkNHit_mix);
    if (t->GetBranch("trkNlayer_mix"))  t->SetBranchAddress("trkNlayer_mix", &p_trkNlayer_mix, &b_trkNlayer_mix);
    if (t->GetBranch("highPurity_mix"))  t->SetBranchAddress("highPurity_mix", &p_highPurity_mix, &b_highPurity_mix);
    if (t->GetBranch("trkChi2_mix"))  t->SetBranchAddress("trkChi2_mix", &p_trkChi2_mix, &b_trkChi2_mix);
    if (t->GetBranch("trkNdof_mix"))  t->SetBranchAddress("trkNdof_mix", &p_trkNdof_mix, &b_trkNdof_mix);
    if (t->GetBranch("trkDxy1_mix"))  t->SetBranchAddress("trkDxy1_mix", &p_trkDxy1_mix, &b_trkDxy1_mix);
    if (t->GetBranch("trkDxyError1_mix"))  t->SetBranchAddress("trkDxyError1_mix", &p_trkDxyError1_mix, &b_trkDxyError1_mix);
    if (t->GetBranch("trkDz1_mix"))  t->SetBranchAddress("trkDz1_mix", &p_trkDz1_mix, &b_trkDz1_mix);
    if (t->GetBranch("trkDzError1_mix"))  t->SetBranchAddress("trkDzError1_mix", &p_trkDzError1_mix, &b_trkDzError1_mix);
    if (t->GetBranch("trkAlgo_mix"))  t->SetBranchAddress("trkAlgo_mix", &p_trkAlgo_mix, &b_trkAlgo_mix);
    if (t->GetBranch("trkMVA_mix"))  t->SetBranchAddress("trkMVA_mix", &p_trkMVA_mix, &b_trkMVA_mix);
    if (t->GetBranch("pfType_mix"))  t->SetBranchAddress("pfType_mix", &p_pfType_mix, &b_pfType_mix);
    if (t->GetBranch("pfCandPt_mix"))  t->SetBranchAddress("pfCandPt_mix", &p_pfCandPt_mix, &b_pfCandPt_mix);
    if (t->GetBranch("pfEcal_mix"))  t->SetBranchAddress("pfEcal_mix", &p_pfEcal_mix, &b_pfEcal_mix);
    if (t->GetBranch("pfHcal_mix"))  t->SetBranchAddress("pfHcal_mix", &p_pfHcal_mix, &b_pfHcal_mix);
    if (t->GetBranch("trkNVtx_mix"))  t->SetBranchAddress("trkNVtx_mix", &p_trkNVtx_mix, &b_trkNVtx_mix);
    if (t->GetBranch("trkWeight_mix"))  t->SetBranchAddress("trkWeight_mix", &p_trkWeight_mix, &b_trkWeight_mix);
    if (t->GetBranch("evttrk_mix"))  t->SetBranchAddress("evttrk_mix", &p_evttrk_mix, &b_evttrk_mix);

    if (t->GetBranch("mult_mix"))  t->SetBranchAddress("mult_mix", &mult_mix, &b_mult_mix);
    if (t->GetBranch("pt_mix"))  t->SetBranchAddress("pt_mix", &p_pt_mix, &b_pt_mix);
    if (t->GetBranch("eta_mix"))  t->SetBranchAddress("eta_mix", &p_eta_mix, &b_eta_mix);
    if (t->GetBranch("phi_mix"))  t->SetBranchAddress("phi_mix", &p_phi_mix, &b_phi_mix);
    if (t->GetBranch("pdg_mix"))  t->SetBranchAddress("pdg_mix", &p_pdg_mix, &b_pdg_mix);
    if (t->GetBranch("chg_mix"))  t->SetBranchAddress("chg_mix", &p_chg_mix, &b_chg_mix);
    if (t->GetBranch("sube_mix"))  t->SetBranchAddress("sube_mix", &p_sube_mix, &b_sube_mix);
    if (t->GetBranch("evtgen_mix"))  t->SetBranchAddress("evtgen_mix", &p_evtgen_mix, &b_evtgen_mix);
}

void trackSkim::setupTreeForWriting(TTree *t)
{
    t->Branch("nTrk", &nTrk);
    t->Branch("nVtx", &nVtx);
    t->Branch("maxPtVtx", &maxPtVtx);
    t->Branch("maxMultVtx", &maxMultVtx);

    t->Branch("trkPt", &trkPt);
    t->Branch("trkEta", &trkEta);
    t->Branch("trkPhi", &trkPhi);
    t->Branch("trkCharge", &trkCharge);
    t->Branch("trkPtError", &trkPtError);
    t->Branch("trkNHit", &trkNHit);
    t->Branch("trkNlayer", &trkNlayer);
    t->Branch("highPurity", &highPurity);
    t->Branch("trkChi2", &trkChi2);
    t->Branch("trkNdof", &trkNdof);
    t->Branch("trkDxy1", &trkDxy1);
    t->Branch("trkDxyError1", &trkDxyError1);
    t->Branch("trkDz1", &trkDz1);
    t->Branch("trkDzError1", &trkDzError1);
    t->Branch("trkAlgo", &trkAlgo);
    t->Branch("trkMVA", &trkMVA);
    t->Branch("pfType", &pfType);
    t->Branch("pfCandPt", &pfCandPt);
    t->Branch("pfEcal", &pfEcal);
    t->Branch("pfHcal", &pfHcal);
    t->Branch("trkNVtx", &trkNVtx);
    t->Branch("trkWeight", &trkWeight);

    t->Branch("mult", &mult);
    t->Branch("pt", &pt);
    t->Branch("eta", &eta);
    t->Branch("phi", &phi);
    t->Branch("pdg", &pdg);
    t->Branch("chg", &chg);
    t->Branch("sube", &sube);

    t->Branch("nTrk_mix", &nTrk_mix);
    t->Branch("trkPt_mix", &trkPt_mix);
    t->Branch("trkEta_mix", &trkEta_mix);
    t->Branch("trkPhi_mix", &trkPhi_mix);
    t->Branch("trkCharge_mix", &trkCharge_mix);
    t->Branch("trkPtError_mix", &trkPtError_mix);
    t->Branch("trkNHit_mix", &trkNHit_mix);
    t->Branch("trkNlayer_mix", &trkNlayer_mix);
    t->Branch("highPurity_mix", &highPurity_mix);
    t->Branch("trkChi2_mix", &trkChi2_mix);
    t->Branch("trkNdof_mix", &trkNdof_mix);
    t->Branch("trkDxy1_mix", &trkDxy1_mix);
    t->Branch("trkDxyError1_mix", &trkDxyError1_mix);
    t->Branch("trkDz1_mix", &trkDz1_mix);
    t->Branch("trkDzError1_mix", &trkDzError1_mix);
    t->Branch("trkAlgo_mix", &trkAlgo_mix);
    t->Branch("trkMVA_mix", &trkMVA_mix);
    t->Branch("pfType_mix", &pfType_mix);
    t->Branch("pfCandPt_mix", &pfCandPt_mix);
    t->Branch("pfEcal_mix", &pfEcal_mix);
    t->Branch("pfHcal_mix", &pfHcal_mix);
    t->Branch("trkNVtx_mix", &trkNVtx_mix);
    t->Branch("trkWeight_mix", &trkWeight_mix);
    t->Branch("evttrk_mix", &evttrk_mix);

    t->Branch("mult_mix", &mult_mix);
    t->Branch("pt_mix", &pt_mix);
    t->Branch("eta_mix", &eta_mix);
    t->Branch("phi_mix", &phi_mix);
    t->Branch("pdg_mix", &pdg_mix);
    t->Branch("chg_mix", &chg_mix);
    t->Branch("sube_mix", &sube_mix);
    t->Branch("evtgen_mix", &evtgen_mix);
}

void trackSkim::clearEvent()
{
    nTrk = 0;
    mult = 0;
    nVtx = 0;
    maxPtVtx = -1;
    maxMultVtx = -1;

    trkPt.clear();
    trkEta.clear();
    trkPhi.clear();
    trkCharge.clear();
    trkPtError.clear();
    trkNHit.clear();
    trkNlayer.clear();
    highPurity.clear();
    trkChi2.clear();
    trkNdof.clear();
    trkDxy1.clear();
    trkDxyError1.clear();
    trkDz1.clear();
    trkDzError1.clear();
    trkAlgo.clear();
    trkMVA.clear();
    pfType.clear();
    pfCandPt.clear();
    pfEcal.clear();
    pfHcal.clear();
    trkNVtx.clear();
    trkWeight.clear();

    pt.clear();
    eta.clear();
    phi.clear();
    pdg.clear();
    chg.clear();
    sube.clear();

    clearMixEvent();
}

void trackSkim::clearMixEvent()
{
    nTrk_mix = 0;
    mult_mix = 0;

    trkPt_mix.clear();
    trkEta_mix.clear();
    trkPhi_mix.clear();
    trkCharge_mix.clear();
    trkPtError_mix.clear();
    trkNHit_mix.clear();
    trkNlayer_mix.clear();
    highPurity_mix.clear();
    trkChi2_mix.clear();
    trkNdof_mix.clear();
    trkDxy1_mix.clear();
    trkDxyError1_mix.clear();
    trkDz1_mix.clear();
    trkDzError1_mix.clear();
    trkAlgo_mix.clear();
    trkMVA_mix.clear();
    pfType_mix.clear();
    pfCandPt_mix.clear();
    pfEcal_mix.clear();
    pfHcal_mix.clear();
    trkNVtx_mix.clear();
    trkWeight_mix.clear();
    evttrk_mix.clear();

    pt_mix.clear();
    eta_mix.clear();
    phi_mix.clear();
    pdg_mix.clear();
    chg_mix.clear();
    sube_mix.clear();
    evtgen_mix.clear();
}

#endif /* TREEHEADERS_TRACKSKIMTREE_H_ */
