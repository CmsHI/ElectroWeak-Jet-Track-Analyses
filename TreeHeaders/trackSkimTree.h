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
        p_pfEcal = 0;
        p_pfHcal = 0;
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
    Int_t nTrk;
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
    std::vector<float>   pfEcal;
    std::vector<float>   pfHcal;
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
    std::vector<float> *p_pfEcal;
    std::vector<float> *p_pfHcal;
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
    TBranch *b_pfEcal;
    TBranch *b_pfHcal;
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
    b_pfEcal = 0;
    b_pfHcal = 0;
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
    if (t->GetBranch("pfEcal"))  t->SetBranchAddress("pfEcal", &p_pfEcal, &b_pfEcal);
    if (t->GetBranch("pfHcal"))  t->SetBranchAddress("pfHcal", &p_pfHcal, &b_pfHcal);
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
    t->Branch("pfEcal", &pfEcal);
    t->Branch("pfHcal", &pfHcal);
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
    pfEcal.clear();
    pfHcal.clear();
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
