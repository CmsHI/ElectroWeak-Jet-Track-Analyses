#ifndef TREEHEADERS_JETSKIMTREE_H_
#define TREEHEADERS_JETSKIMTREE_H_

#include <TTree.h>
#include <TMath.h>
#include <TVector3.h>

#include <vector>

class jetSkim {
public :
    jetSkim(){
    }
    ~jetSkim(){};
    void setupTreeForReading(TTree *t);
    void setupTreeForWriting(TTree *t);
    void clearEvent();
    void clearMixEvent();

    // Declaration of leaf types
    // jets
    Int_t njet;
    std::vector<float>   jetptCorr;
    std::vector<float>   jetpt;
    std::vector<float>   jeteta;
    std::vector<float>   jetphi;
    std::vector<float>   rawpt;
    std::vector<float>   refpt;
    std::vector<float>   refeta;
    std::vector<float>   refphi;
    std::vector<int>     subid;
    std::vector<int>     refparton_flavor;

    Int_t ngen;
    std::vector<float>   genpt;
    std::vector<float>   geneta;
    std::vector<float>   genphi;
    std::vector<int>     gensubid;

    // objects from mixed event
    Int_t njet_mix;
    std::vector<float>   jetptCorr_mix;
    std::vector<float>   jetpt_mix;
    std::vector<float>   jeteta_mix;
    std::vector<float>   jetphi_mix;
    std::vector<float>   rawpt_mix;
    std::vector<float>   refpt_mix;
    std::vector<float>   refeta_mix;
    std::vector<float>   refphi_mix;
    std::vector<int>     subid_mix;
    std::vector<int>     evtjet_mix;

    Int_t ngen_mix;
    std::vector<float>   genpt_mix;
    std::vector<float>   geneta_mix;
    std::vector<float>   genphi_mix;
    std::vector<int>     gensubid_mix;
    std::vector<int>     evtgen_mix;

     // List of branches
    TBranch *b_njet;
    TBranch *b_jetptCorr;
    TBranch *b_jetpt;
    TBranch *b_jeteta;
    TBranch *b_jetphi;
    TBranch *b_rawpt;
    TBranch *b_refpt;
    TBranch *b_refeta;
    TBranch *b_refphi;
    TBranch *b_subid;
    TBranch *b_refparton_flavor;

    TBranch *b_ngen;
    TBranch *b_genpt;
    TBranch *b_geneta;
    TBranch *b_genphi;
    TBranch *b_gensubid;

    TBranch *b_njet_mix;
    TBranch *b_jetptCorr_mix;
    TBranch *b_jetpt_mix;
    TBranch *b_jeteta_mix;
    TBranch *b_jetphi_mix;
    TBranch *b_rawpt_mix;
    TBranch *b_refpt_mix;
    TBranch *b_refeta_mix;
    TBranch *b_refphi_mix;
    TBranch *b_subid_mix;
    TBranch *b_evtjet_mix;

    TBranch *b_ngen_mix;
    TBranch *b_genpt_mix;
    TBranch *b_geneta_mix;
    TBranch *b_genphi_mix;
    TBranch *b_gensubid_mix;
    TBranch *b_evtgen_mix;
};

void jetSkim::setupTreeForReading(TTree *t)
{
    b_njet = 0;
    b_jetptCorr = 0;
    b_jetpt = 0;
    b_jeteta = 0;
    b_jetphi = 0;
    b_rawpt = 0;
    b_refpt = 0;
    b_refeta = 0;
    b_refphi = 0;
    b_subid = 0;
    b_refparton_flavor = 0;

    b_ngen = 0;
    b_genpt = 0;
    b_geneta = 0;
    b_genphi = 0;
    b_gensubid = 0;

    b_njet_mix = 0;
    b_jetptCorr_mix = 0;
    b_jetpt_mix = 0;
    b_jeteta_mix = 0;
    b_jetphi_mix = 0;
    b_rawpt_mix = 0;
    b_refpt_mix = 0;
    b_refeta_mix = 0;
    b_refphi_mix = 0;
    b_subid_mix = 0;
    b_evtjet_mix = 0;

    b_ngen_mix = 0;
    b_genpt_mix = 0;
    b_geneta_mix = 0;
    b_genphi_mix = 0;
    b_gensubid_mix = 0;
    b_evtgen_mix = 0;

    if (t->GetBranch("njet"))  t->SetBranchAddress("njet", &njet, &b_njet);
    if (t->GetBranch("jetptCorr"))  t->SetBranchAddress("jetptCorr", &jetptCorr, &b_jetptCorr);
    if (t->GetBranch("jetpt"))  t->SetBranchAddress("jetpt", &jetpt, &b_jetpt);
    if (t->GetBranch("jeteta"))  t->SetBranchAddress("jeteta", &jeteta, &b_jeteta);
    if (t->GetBranch("jetphi"))  t->SetBranchAddress("jetphi", &jetphi, &b_jetphi);
    if (t->GetBranch("rawpt"))  t->SetBranchAddress("rawpt", &rawpt, &b_rawpt);
    if (t->GetBranch("refpt"))  t->SetBranchAddress("refpt", &refpt, &b_refpt);
    if (t->GetBranch("refeta"))  t->SetBranchAddress("refeta", &refeta, &b_refeta);
    if (t->GetBranch("refphi"))  t->SetBranchAddress("refphi", &refphi, &b_refphi);
    if (t->GetBranch("subid"))  t->SetBranchAddress("subid", &subid, &b_subid);
    if (t->GetBranch("refparton_flavor"))  t->SetBranchAddress("refparton_flavor", &refparton_flavor, &b_refparton_flavor);

    if (t->GetBranch("ngen"))  t->SetBranchAddress("ngen", &ngen, &b_ngen);
    if (t->GetBranch("genpt"))  t->SetBranchAddress("genpt", &genpt, &b_genpt);
    if (t->GetBranch("geneta"))  t->SetBranchAddress("geneta", &geneta, &b_geneta);
    if (t->GetBranch("genphi"))  t->SetBranchAddress("genphi", &genphi, &b_genphi);
    if (t->GetBranch("gensubid"))  t->SetBranchAddress("gensubid", &gensubid, &b_gensubid);

    if (t->GetBranch("njet_mix"))  t->SetBranchAddress("njet_mix", &njet_mix, &b_njet_mix);
    if (t->GetBranch("jetptCorr_mix"))  t->SetBranchAddress("jetptCorr_mix", &jetptCorr_mix, &b_jetptCorr_mix);
    if (t->GetBranch("jetpt_mix"))  t->SetBranchAddress("jetpt_mix", &jetpt_mix, &b_jetpt_mix);
    if (t->GetBranch("jeteta_mix"))  t->SetBranchAddress("jeteta_mix", &jeteta_mix, &b_jeteta_mix);
    if (t->GetBranch("jetphi_mix"))  t->SetBranchAddress("jetphi_mix", &jetphi_mix, &b_jetphi_mix);
    if (t->GetBranch("rawpt_mix"))  t->SetBranchAddress("rawpt_mix", &rawpt_mix, &b_rawpt_mix);
    if (t->GetBranch("refpt_mix"))  t->SetBranchAddress("refpt_mix", &refpt_mix, &b_refpt_mix);
    if (t->GetBranch("refeta_mix"))  t->SetBranchAddress("refeta_mix", &refeta_mix, &b_refeta_mix);
    if (t->GetBranch("refphi_mix"))  t->SetBranchAddress("refphi_mix", &refphi_mix, &b_refphi_mix);
    if (t->GetBranch("subid_mix"))  t->SetBranchAddress("subid_mix", &subid_mix, &b_subid_mix);
    if (t->GetBranch("evtjet_mix"))  t->SetBranchAddress("evtjet_mix", &evtjet_mix, &b_evtjet_mix);

    if (t->GetBranch("ngen_mix"))  t->SetBranchAddress("ngen_mix", &ngen_mix, &b_ngen_mix);
    if (t->GetBranch("genpt_mix"))  t->SetBranchAddress("genpt_mix", &genpt_mix, &b_genpt_mix);
    if (t->GetBranch("geneta_mix"))  t->SetBranchAddress("geneta_mix", &geneta_mix, &b_geneta_mix);
    if (t->GetBranch("genphi_mix"))  t->SetBranchAddress("genphi_mix", &genphi_mix, &b_genphi_mix);
    if (t->GetBranch("gensubid_mix"))  t->SetBranchAddress("gensubid_mix", &gensubid_mix, &b_gensubid_mix);
    if (t->GetBranch("evtgen_mix"))  t->SetBranchAddress("evtgen_mix", &evtgen_mix, &b_evtgen_mix);
}

void jetSkim::setupTreeForWriting(TTree *t)
{
    t->Branch("njet", &njet);
    t->Branch("jetptCorr", &jetptCorr);
    t->Branch("jetpt", &jetpt);
    t->Branch("jeteta", &jeteta);
    t->Branch("jetphi", &jetphi);
    t->Branch("rawpt", &rawpt);
    t->Branch("refpt", &refpt);
    t->Branch("refeta", &refeta);
    t->Branch("refphi", &refphi);
    t->Branch("subid", &subid);
    t->Branch("refparton_flavor", &refparton_flavor);

    t->Branch("ngen", &ngen);
    t->Branch("genpt", &genpt);
    t->Branch("geneta", &geneta);
    t->Branch("genphi", &genphi);
    t->Branch("gensubid", &gensubid);

    t->Branch("njet_mix", &njet_mix);
    t->Branch("jetptCorr_mix", &jetptCorr_mix);
    t->Branch("jetpt_mix", &jetpt_mix);
    t->Branch("jeteta_mix", &jeteta_mix);
    t->Branch("jetphi_mix", &jetphi_mix);
    t->Branch("rawpt_mix", &rawpt_mix);
    t->Branch("refpt_mix", &refpt_mix);
    t->Branch("refeta_mix", &refeta_mix);
    t->Branch("refphi_mix", &refphi_mix);
    t->Branch("subid_mix", &subid_mix);
    t->Branch("evtjet_mix", &evtjet_mix);

    t->Branch("ngen_mix", &ngen_mix);
    t->Branch("genpt_mix", &genpt_mix);
    t->Branch("geneta_mix", &geneta_mix);
    t->Branch("genphi_mix", &genphi_mix);
    t->Branch("gensubid_mix", &gensubid_mix);
    t->Branch("evtgen_mix", &evtgen_mix);
}

void jetSkim::clearEvent()
{
    njet = 0;
    ngen = 0;

    jetptCorr.clear();
    jetpt.clear();
    jeteta.clear();
    jetphi.clear();
    rawpt.clear();
    refpt.clear();
    refeta.clear();
    refphi.clear();
    subid.clear();
    refparton_flavor.clear();

    genpt.clear();
    geneta.clear();
    genphi.clear();
    gensubid.clear();

    clearMixEvent();
}

void jetSkim::clearMixEvent()
{
    njet_mix = 0;
    ngen_mix = 0;

    jetptCorr_mix.clear();
    jetpt_mix.clear();
    jeteta_mix.clear();
    jetphi_mix.clear();
    rawpt_mix.clear();
    refpt_mix.clear();
    refeta_mix.clear();
    refphi_mix.clear();
    subid_mix.clear();
    evtjet_mix.clear();

    genpt_mix.clear();
    geneta_mix.clear();
    genphi_mix.clear();
    gensubid_mix.clear();
    evtgen_mix.clear();
}

#endif /* TREEHEADERS_JETSKIMTREE_H_ */
