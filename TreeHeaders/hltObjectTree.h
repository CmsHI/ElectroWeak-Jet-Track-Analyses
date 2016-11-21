#ifndef TREEHEADERS_HLTOBJECTTREE_H_
#define TREEHEADERS_HLTOBJECTTREE_H_

#include <TTree.h>
#include <TBranch.h>

#include "vector"

class hltObject {
public :
    hltObject(){
        TriggerObjID = 0;
        pt = 0;
        eta = 0;
        phi = 0;
        mass = 0;
    };
    ~hltObject(){};
    void setupTreeForReading(TTree *t);
    void setupTreeForWriting(TTree *t);

   // Declaration of leaf types
   std::vector<double>  *TriggerObjID;
   std::vector<double>  *pt;
   std::vector<double>  *eta;
   std::vector<double>  *phi;
   std::vector<double>  *mass;

   // List of branches
   TBranch        *b_TriggerObjID;   //!
   TBranch        *b_pt;   //!
   TBranch        *b_eta;   //!
   TBranch        *b_phi;   //!
   TBranch        *b_mass;   //!
};

void hltObject::setupTreeForReading(TTree *t)
{
    // Set branch addresses and branch pointers
    if (t->GetBranch("TriggerObjID"))  t->SetBranchAddress("TriggerObjID", &TriggerObjID, &b_TriggerObjID);
    if (t->GetBranch("pt"))  t->SetBranchAddress("pt", &pt, &b_pt);
    if (t->GetBranch("eta"))  t->SetBranchAddress("eta", &eta, &b_eta);
    if (t->GetBranch("phi"))  t->SetBranchAddress("phi", &phi, &b_phi);
    if (t->GetBranch("mass"))  t->SetBranchAddress("mass", &mass, &b_mass);
}

void hltObject::setupTreeForWriting(TTree *t)
{
    t->Branch("TriggerObjID",&TriggerObjID);
    t->Branch("pt",&pt);
    t->Branch("eta",&eta);
    t->Branch("phi",&phi);
    t->Branch("mass",&mass);
}

#endif
