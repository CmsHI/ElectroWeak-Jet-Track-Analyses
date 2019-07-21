#ifndef TREEHEADERS_PFCANDTREE_H_
#define TREEHEADERS_PFCANDTREE_H_

#include <TTree.h>
#include <TBranch.h>

#include <vector>

class pfCand {
public :
    pfCand(){

        pfId = 0;
        pfPt = 0;
        pfEnergy = 0;
        pfEta = 0;
        pfPhi = 0;
        pfM = 0;
        pfEcalE = 0;
        pfEcalEraw = 0;
        pfHcalE = 0;
        pfHcalEraw = 0;
        trkAlgo = 0;
        trkPtError = 0;
        trkNHit = 0;
        trkChi2 = 0;
        trkNdof = 0;
    };
    ~pfCand(){};
    void setupTreeForReading(TTree *t);

    // Declaration of leaf types
    // Declaration of leaf types
    int           nPFpart;
    std::vector<int>     *pfId;
    std::vector<float>   *pfPt;
    std::vector<float>   *pfEnergy;
    std::vector<float>   *pfEta;
    std::vector<float>   *pfPhi;
    std::vector<float>   *pfM;
    std::vector<float>   *pfEcalE;
    std::vector<float>   *pfEcalEraw;
    std::vector<float>   *pfHcalE;
    std::vector<float>   *pfHcalEraw;
    std::vector<int>     *trkAlgo;
    std::vector<float>   *trkPtError;
    std::vector<float>   *trkNHit;
    std::vector<float>   *trkChi2;
    std::vector<float>   *trkNdof;

    // List of branches
    TBranch        *b_nPFpart;   //!
    TBranch        *b_pfId;   //!
    TBranch        *b_pfPt;   //!
    TBranch        *b_pfEnergy;   //!
    TBranch        *b_pfEta;   //!
    TBranch        *b_pfPhi;   //!
    TBranch        *b_pfM;   //!
    TBranch        *b_pfEcalE;   //!
    TBranch        *b_pfEcalEraw;   //!
    TBranch        *b_pfHcalE;   //!
    TBranch        *b_pfHcalEraw;   //!
    TBranch        *b_trkAlgo;   //!
    TBranch        *b_trkPtError;   //!
    TBranch        *b_trkNHit;   //!
    TBranch        *b_trkChi2;   //!
    TBranch        *b_trkNdof;   //!
};

void pfCand::setupTreeForReading(TTree *t)
{
    b_nPFpart = 0;
    b_pfId = 0;
    b_pfPt = 0;
    b_pfEnergy = 0;
    b_pfEta = 0;
    b_pfPhi = 0;
    b_pfM = 0;
    b_pfEcalE = 0;
    b_pfEcalEraw = 0;
    b_pfHcalE = 0;
    b_pfHcalEraw = 0;
    b_trkAlgo = 0;
    b_trkPtError = 0;
    b_trkNHit = 0;
    b_trkChi2 = 0;
    b_trkNdof = 0;

    // Set branch addresses and branch pointers
    if (t->GetBranch("nPFpart"))  t->SetBranchAddress("nPFpart", &nPFpart, &b_nPFpart);
    if (t->GetBranch("pfId"))  t->SetBranchAddress("pfId", &pfId, &b_pfId);
    if (t->GetBranch("pfPt"))  t->SetBranchAddress("pfPt", &pfPt, &b_pfPt);
    if (t->GetBranch("pfEnergy"))  t->SetBranchAddress("pfEnergy", &pfEnergy, &b_pfEnergy);
    if (t->GetBranch("pfEta"))  t->SetBranchAddress("pfEta", &pfEta, &b_pfEta);
    if (t->GetBranch("pfPhi"))  t->SetBranchAddress("pfPhi", &pfPhi, &b_pfPhi);
    if (t->GetBranch("pfM"))  t->SetBranchAddress("pfM", &pfM, &b_pfM);
    if (t->GetBranch("pfEcalE"))  t->SetBranchAddress("pfEcalE", &pfEcalE, &b_pfEcalE);
    if (t->GetBranch("pfEcalEraw"))  t->SetBranchAddress("pfEcalEraw", &pfEcalEraw, &b_pfEcalEraw);
    if (t->GetBranch("pfHcalE"))  t->SetBranchAddress("pfHcalE", &pfHcalE, &b_pfHcalE);
    if (t->GetBranch("pfHcalEraw"))  t->SetBranchAddress("pfHcalEraw", &pfHcalEraw, &b_pfHcalEraw);
    if (t->GetBranch("trkAlgo"))  t->SetBranchAddress("trkAlgo", &trkAlgo, &b_trkAlgo);
    if (t->GetBranch("trkPtError"))  t->SetBranchAddress("trkPtError", &trkPtError, &b_trkPtError);
    if (t->GetBranch("trkNHit"))  t->SetBranchAddress("trkNHit", &trkNHit, &b_trkNHit);
    if (t->GetBranch("trkChi2"))  t->SetBranchAddress("trkChi2", &trkChi2, &b_trkChi2);
    if (t->GetBranch("trkNdof"))  t->SetBranchAddress("trkNdof", &trkNdof, &b_trkNdof);
}

#endif
