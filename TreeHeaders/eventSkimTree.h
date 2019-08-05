/*
 * reads/writes additional event info that is not in hiEvtAnalyzer
 */
#ifndef TREEHEADERS_EVENTSKIMTREE_H_
#define TREEHEADERS_EVENTSKIMTREE_H_

#include <TTree.h>
#include <TMath.h>
#include <TVector3.h>

#include <vector>

class eventSkim {
public :
    eventSkim(){

    }
    ~eventSkim(){};
    void setupTreeForReading(TTree *t);
    void setupTreeForWriting(TTree *t);
    void clearEvent();

    float rho;
    float pf_h_HF_totE;
    float pf_eg_HF_totE;
    float pf_h_HE_totE;
    float pf_e_HE_totE;
    float pf_mu_HE_totE;
    float pf_gamma_HE_totE;
    float pf_h0_HE_totE;

     // List of branches
    TBranch *b_rho;
    TBranch *b_pf_h_HF_totE;
    TBranch *b_pf_eg_HF_totE;
    TBranch *b_pf_h_HE_totE;
    TBranch *b_pf_e_HE_totE;
    TBranch *b_pf_mu_HE_totE;
    TBranch *b_pf_gamma_HE_totE;
    TBranch *b_pf_h0_HE_totE;
};

void eventSkim::setupTreeForReading(TTree *t)
{
    b_rho = 0;
    b_pf_h_HF_totE = 0;
    b_pf_eg_HF_totE = 0;
    b_pf_h_HE_totE = 0;
    b_pf_e_HE_totE = 0;
    b_pf_mu_HE_totE = 0;
    b_pf_gamma_HE_totE = 0;
    b_pf_h0_HE_totE = 0;

    if (t->GetBranch("rho"))  t->SetBranchAddress("rho", &rho, &b_rho);
    if (t->GetBranch("pf_h_HF_totE"))  t->SetBranchAddress("pf_h_HF_totE", &pf_h_HF_totE, &b_pf_h_HF_totE);
    if (t->GetBranch("pf_eg_HF_totE"))  t->SetBranchAddress("pf_eg_HF_totE", &pf_eg_HF_totE, &b_pf_eg_HF_totE);
    if (t->GetBranch("pf_h_HE_totE"))  t->SetBranchAddress("pf_h_HE_totE", &pf_h_HE_totE, &b_pf_h_HE_totE);
    if (t->GetBranch("pf_e_HE_totE"))  t->SetBranchAddress("pf_e_HE_totE", &pf_e_HE_totE, &b_pf_e_HE_totE);
    if (t->GetBranch("pf_mu_HE_totE"))  t->SetBranchAddress("pf_mu_HE_totE", &pf_mu_HE_totE, &b_pf_mu_HE_totE);
    if (t->GetBranch("pf_gamma_HE_totE"))  t->SetBranchAddress("pf_gamma_HE_totE", &pf_gamma_HE_totE, &b_pf_gamma_HE_totE);
    if (t->GetBranch("pf_h0_HE_totE"))  t->SetBranchAddress("pf_h0_HE_totE", &pf_h0_HE_totE, &b_pf_h0_HE_totE);
}

void eventSkim::setupTreeForWriting(TTree *t)
{
    t->Branch("rho", &rho);
    t->Branch("pf_h_HF_totE", &pf_h_HF_totE);
    t->Branch("pf_eg_HF_totE", &pf_eg_HF_totE);
    t->Branch("pf_h_HE_totE", &pf_h_HE_totE);
    t->Branch("pf_e_HE_totE", &pf_e_HE_totE);
    t->Branch("pf_mu_HE_totE", &pf_mu_HE_totE);
    t->Branch("pf_gamma_HE_totE", &pf_gamma_HE_totE);
    t->Branch("pf_h0_HE_totE", &pf_h0_HE_totE);
}

void eventSkim::clearEvent()
{
    rho = 0;
    pf_h_HF_totE = 0;
    pf_eg_HF_totE = 0;
    pf_h_HE_totE = 0;
    pf_e_HE_totE = 0;
    pf_mu_HE_totE = 0;
    pf_gamma_HE_totE = 0;
    pf_h0_HE_totE = 0;
}

#endif /* TREEHEADERS_EVENTSKIMTREE_H_ */
