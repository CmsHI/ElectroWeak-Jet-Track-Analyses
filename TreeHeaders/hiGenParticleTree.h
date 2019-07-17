#ifndef TREEHEADERS_HIGENPARTICLETREE_H_
#define TREEHEADERS_HIGENPARTICLETREE_H_

#include <TTree.h>
#include <TBranch.h>

#include <vector>

class hiGenParticle {
public :
    hiGenParticle(){
        pt = 0;
        eta = 0;
        phi = 0;
        pdg = 0;
        chg = 0;
        matchingID = 0;
        nMothers = 0;
        motherIdx = 0;
        nDaughters = 0;
        daughterIdx = 0;
        sube = 0;
    };
    ~hiGenParticle(){};
    void setupTreeForReading(TTree *t);
    void setupTreeForWriting(TTree *t);

   // Declaration of leaf types
   Int_t           event;
   Float_t         b;
   Float_t         npart;
   Float_t         ncoll;
   Float_t         nhard;
   Float_t         phi0;
   Float_t         scale;
   Int_t           n[3];
   Float_t         ptav[3];
   Int_t           mult;
   std::vector<float>   *pt;
   std::vector<float>   *eta;
   std::vector<float>   *phi;
   std::vector<int>     *pdg;
   std::vector<int>     *chg;
   std::vector<int>     *matchingID;
   std::vector<int>     *nMothers;
   std::vector<std::vector<int> > *motherIdx;
   std::vector<int>     *nDaughters;
   std::vector<std::vector<int> > *daughterIdx;
   std::vector<int>     *sta;
   std::vector<int>     *sube;
   Float_t         vx;
   Float_t         vy;
   Float_t         vz;
   Float_t         vr;

   // List of branches
   TBranch        *b_event;   //!
   TBranch        *b_b;   //!
   TBranch        *b_npart;   //!
   TBranch        *b_ncoll;   //!
   TBranch        *b_nhard;   //!
   TBranch        *b_phi0;   //!
   TBranch        *b_scale;   //!
   TBranch        *b_n;   //!
   TBranch        *b_ptav;   //!
   TBranch        *b_mult;   //!
   TBranch        *b_pt;   //!
   TBranch        *b_eta;   //!
   TBranch        *b_phi;   //!
   TBranch        *b_pdg;   //!
   TBranch        *b_chg;   //!
   TBranch        *b_matchingID;   //!
   TBranch        *b_nMothers;   //!
   TBranch        *b_motherIdx;   //!
   TBranch        *b_nDaughters;   //!
   TBranch        *b_daughterIdx;   //!
   TBranch        *b_sta;   //!
   TBranch        *b_sube;   //!
   TBranch        *b_vx;   //!
   TBranch        *b_vy;   //!
   TBranch        *b_vz;   //!
   TBranch        *b_vr;   //!
};

void hiGenParticle::setupTreeForReading(TTree *t)
{
    b_event = 0;
    b_b = 0;
    b_npart = 0;
    b_ncoll = 0;
    b_nhard = 0;
    b_phi0 = 0;
    b_scale = 0;
    b_n = 0;
    b_ptav = 0;
    b_mult = 0;
    b_pt = 0;
    b_eta = 0;
    b_phi = 0;
    b_pdg = 0;
    b_chg = 0;
    b_matchingID = 0;
    b_nMothers = 0;
    b_motherIdx = 0;
    b_nDaughters = 0;
    b_daughterIdx = 0;
    b_sta = 0;
    b_sube = 0;
    b_vx = 0;
    b_vy = 0;
    b_vz = 0;
    b_vr = 0;

    // Set branch addresses and branch pointers
    if (t->GetBranch("event"))  t->SetBranchAddress("event", &event, &b_event);
    if (t->GetBranch("b"))  t->SetBranchAddress("b", &b, &b_b);
    if (t->GetBranch("npart"))  t->SetBranchAddress("npart", &npart, &b_npart);
    if (t->GetBranch("ncoll"))  t->SetBranchAddress("ncoll", &ncoll, &b_ncoll);
    if (t->GetBranch("nhard"))  t->SetBranchAddress("nhard", &nhard, &b_nhard);
    if (t->GetBranch("phi0"))  t->SetBranchAddress("phi0", &phi0, &b_phi0);
    if (t->GetBranch("scale"))  t->SetBranchAddress("scale", &scale, &b_scale);
    if (t->GetBranch("n"))  t->SetBranchAddress("n", n, &b_n);
    if (t->GetBranch("ptav"))  t->SetBranchAddress("ptav", ptav, &b_ptav);
    if (t->GetBranch("mult"))  t->SetBranchAddress("mult", &mult, &b_mult);
    if (t->GetBranch("pt"))  t->SetBranchAddress("pt", &pt, &b_pt);
    if (t->GetBranch("eta"))  t->SetBranchAddress("eta", &eta, &b_eta);
    if (t->GetBranch("phi"))  t->SetBranchAddress("phi", &phi, &b_phi);
    if (t->GetBranch("pdg"))  t->SetBranchAddress("pdg", &pdg, &b_pdg);
    if (t->GetBranch("chg"))  t->SetBranchAddress("chg", &chg, &b_chg);
    if (t->GetBranch("matchingID"))  t->SetBranchAddress("matchingID", &matchingID, &b_matchingID);
    if (t->GetBranch("nMothers"))  t->SetBranchAddress("nMothers", &nMothers, &b_nMothers);
    if (t->GetBranch("motherIdx"))  t->SetBranchAddress("motherIdx", &motherIdx, &b_motherIdx);
    if (t->GetBranch("nDaughters"))  t->SetBranchAddress("nDaughters", &nDaughters, &b_nDaughters);
    if (t->GetBranch("daughterIdx"))  t->SetBranchAddress("daughterIdx", &daughterIdx, &b_daughterIdx);
    if (t->GetBranch("sta"))  t->SetBranchAddress("sta", &sta, &b_sta);
    if (t->GetBranch("sube"))  t->SetBranchAddress("sube", &sube, &b_sube);
    if (t->GetBranch("vx"))  t->SetBranchAddress("vx", &vx, &b_vx);
    if (t->GetBranch("vy"))  t->SetBranchAddress("vy", &vy, &b_vy);
    if (t->GetBranch("vz"))  t->SetBranchAddress("vz", &vz, &b_vz);
    if (t->GetBranch("vr"))  t->SetBranchAddress("vr", &vr, &b_vr);
}

void hiGenParticle::setupTreeForWriting(TTree *t)
{
    t->Branch("event", &event, "event/I");
    t->Branch("b", &b, "b/F");
    t->Branch("npart", &npart, "npart/F");
    t->Branch("ncoll", &ncoll, "ncoll/F");
    t->Branch("nhard", &nhard, "nhard/F");
    t->Branch("phi0", &phi0, "phi0/F");
    t->Branch("scale", &scale, "scale/F");
    t->Branch("n", &n, "n[3]/I");
    t->Branch("ptav", &ptav, "ptav[3]/I");
    t->Branch("mult", &mult, "mult/I");
    t->Branch("pt", &pt);
    t->Branch("eta", &eta);
    t->Branch("phi", &phi);
    t->Branch("pdg", &pdg);
    t->Branch("chg", &chg);
    t->Branch("matchingID", &matchingID);
    t->Branch("nMothers", &nMothers);
    t->Branch("motherIdx", &motherIdx);
    t->Branch("nDaughters", &nDaughters);
    t->Branch("daughterIdx", &daughterIdx);
    t->Branch("sta", &sta);
    t->Branch("sube", &sube);
    t->Branch("vx", &vx, "vx/F");
    t->Branch("vy", &vy, "vy/F");
    t->Branch("vz", &vz, "vz/F");
    t->Branch("vr", &vr, "vr/F");
}

#endif
