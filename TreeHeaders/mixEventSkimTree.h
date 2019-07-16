#ifndef TREEHEADERS_MIXEVENTSKIMTREE_H_
#define TREEHEADERS_MIXEVENTSKIMTREE_H_

#include <TTree.h>
#include <TMath.h>
#include <TVector3.h>

#include <vector>

class mixEventSkim {
public :
    mixEventSkim(){

        p_vz_mix = 0;
        p_hiBin_mix = 0;
        p_hiHF_mix = 0;
        p_hiHFhit_mix = 0;
        p_rho_mix = 0;
        p_hiEvtPlanes_mix = 0;
        p_run_mix = 0;
        p_evt_mix = 0;
        p_lumi_mix = 0;
        p_Npart_mix = 0;
        p_Ncoll_mix = 0;
        p_Nhard_mix = 0;
    }
    ~mixEventSkim(){};
    void setupTreeForReading(TTree *t);
    void setupTreeForWriting(TTree *t);
    void clearEvent();

    // Declaration of leaf types
    // objects from mixed event
    int nmix;
    std::vector<float>   vz_mix;
    std::vector<int>     hiBin_mix;
    std::vector<float>   hiHF_mix;
    std::vector<float>   hiHFhit_mix;
    std::vector<float>   hiEvtPlanes_mix;
    std::vector<float>   rho_mix;
    std::vector<UInt_t>     run_mix;
    std::vector<ULong64_t>  evt_mix;
    std::vector<UInt_t>     lumi_mix;
    std::vector<float>   Npart_mix;
    std::vector<float>   Ncoll_mix;
    std::vector<float>   Nhard_mix;

    // pointer for vectors (to be used when reading)
    std::vector<float>   *p_vz_mix;
    std::vector<int>     *p_hiBin_mix;
    std::vector<float>   *p_hiHF_mix;
    std::vector<float>   *p_hiHFhit_mix;
    std::vector<float>   *p_hiEvtPlanes_mix;
    std::vector<float>   *p_rho_mix;
    std::vector<UInt_t>     *p_run_mix;
    std::vector<ULong64_t>  *p_evt_mix;
    std::vector<UInt_t>     *p_lumi_mix;
    std::vector<float>   *p_Npart_mix;
    std::vector<float>   *p_Ncoll_mix;
    std::vector<float>   *p_Nhard_mix;

     // List of branches
    TBranch *b_nmix;
    TBranch *b_vz_mix;
    TBranch *b_hiBin_mix;
    TBranch *b_hiHF_mix;
    TBranch *b_hiHFhit_mix;
    TBranch *b_hiEvtPlanes_mix;
    TBranch *b_rho_mix;
    TBranch *b_run_mix;
    TBranch *b_evt_mix;
    TBranch *b_lumi_mix;
    TBranch *b_Npart_mix;
    TBranch *b_Ncoll_mix;
    TBranch *b_Nhard_mix;
};

void mixEventSkim::setupTreeForReading(TTree *t)
{
    b_nmix = 0;
    b_vz_mix = 0;
    b_hiBin_mix = 0;
    b_hiHF_mix = 0;
    b_hiHFhit_mix = 0;
    b_hiEvtPlanes_mix = 0;
    b_rho_mix = 0;
    b_run_mix = 0;
    b_evt_mix = 0;
    b_lumi_mix = 0;
    b_Npart_mix = 0;
    b_Ncoll_mix = 0;
    b_Nhard_mix = 0;

    if (t->GetBranch("nmix"))  t->SetBranchAddress("nmix", &nmix, &b_nmix);
    if (t->GetBranch("vz_mix"))  t->SetBranchAddress("vz_mix", &p_vz_mix, &b_vz_mix);
    if (t->GetBranch("hiBin_mix"))  t->SetBranchAddress("hiBin_mix", &p_hiBin_mix, &b_hiBin_mix);
    if (t->GetBranch("hiHF_mix"))  t->SetBranchAddress("hiHF_mix", &p_hiHF_mix, &b_hiHF_mix);
    if (t->GetBranch("hiHFhit_mix"))  t->SetBranchAddress("hiHFhit_mix", &p_hiHFhit_mix, &b_hiHFhit_mix);
    if (t->GetBranch("hiEvtPlanes_mix"))  t->SetBranchAddress("hiEvtPlanes_mix", &p_hiEvtPlanes_mix, &b_hiEvtPlanes_mix);
    if (t->GetBranch("rho_mix"))  t->SetBranchAddress("rho_mix", &p_rho_mix, &b_rho_mix);
    if (t->GetBranch("run_mix"))  t->SetBranchAddress("run_mix", &p_run_mix, &b_run_mix);
    if (t->GetBranch("evt_mix"))  t->SetBranchAddress("evt_mix", &p_evt_mix, &b_evt_mix);
    if (t->GetBranch("lumi_mix"))  t->SetBranchAddress("lumi_mix", &p_lumi_mix, &b_lumi_mix);
    if (t->GetBranch("Npart_mix"))  t->SetBranchAddress("Npart_mix", &p_Npart_mix, &b_Npart_mix);
    if (t->GetBranch("Ncoll_mix"))  t->SetBranchAddress("Ncoll_mix", &p_Ncoll_mix, &b_Ncoll_mix);
    if (t->GetBranch("Nhard_mix"))  t->SetBranchAddress("Nhard_mix", &p_Nhard_mix, &b_Nhard_mix);
}

void mixEventSkim::setupTreeForWriting(TTree *t)
{
    t->Branch("nmix", &nmix);
    t->Branch("vz_mix", &vz_mix);
    t->Branch("hiBin_mix", &hiBin_mix);
    t->Branch("hiHF_mix", &hiHF_mix);
    t->Branch("hiHFhit_mix", &hiHFhit_mix);
    t->Branch("hiEvtPlanes_mix", &hiEvtPlanes_mix);
    t->Branch("rho_mix", &rho_mix);
    t->Branch("run_mix", &run_mix);
    t->Branch("evt_mix", &evt_mix);
    t->Branch("lumi_mix", &lumi_mix);
    t->Branch("Npart_mix", &Npart_mix);
    t->Branch("Ncoll_mix", &Ncoll_mix);
    t->Branch("Nhard_mix", &Nhard_mix);
}

void mixEventSkim::clearEvent()
{
    nmix = 0;

    vz_mix.clear();
    hiBin_mix.clear();
    hiHF_mix.clear();
    hiHFhit_mix.clear();
    hiEvtPlanes_mix.clear();
    rho_mix.clear();
    run_mix.clear();
    evt_mix.clear();
    lumi_mix.clear();
    Npart_mix.clear();
    Ncoll_mix.clear();
    Nhard_mix.clear();
}

#endif /* TREEHEADERS_MIXEVENTSKIMTREE_H_ */
