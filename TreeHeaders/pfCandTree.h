#ifndef TREEHEADERS_PFCANDTREE_H_
#define TREEHEADERS_PFCANDTREE_H_

#include <TTree.h>
#include <TBranch.h>

#include <vector>

class pfCand {
public :
    pfCand(){

        pfKey = 0;
        pfId = 0;
        pfPt = 0;
        pfEnergy = 0;
        pfEta = 0;
        pfPhi = 0;
        pfM = 0;
        pfvx = 0;
        pfvy = 0;
        pfvz = 0;
        pfEcalE = 0;
        pfEcalEraw = 0;
        pfHcalE = 0;
        pfHcalEraw = 0;
        trkAlgo = 0;
        trkPtError = 0;
        trkNHit = 0;
        trkChi2 = 0;
        trkNdof = 0;
        trkNlayer = 0;
        highPurity = 0;
        trkMVA = 0;
        trkDz1 = 0;
        trkDzError1 = 0;
        trkDxy1 = 0;
        trkDxyError1 = 0;
    };
    ~pfCand(){};
    void setupTreeForReading(TTree *t);

    // Declaration of leaf types
    int           nPFpart;
    std::vector<unsigned long> *pfKey;
    std::vector<int>     *pfId;
    std::vector<float>   *pfPt;
    std::vector<float>   *pfEnergy;
    std::vector<float>   *pfEta;
    std::vector<float>   *pfPhi;
    std::vector<float>   *pfM;
    std::vector<float>   *pfvx;
    std::vector<float>   *pfvy;
    std::vector<float>   *pfvz;
    std::vector<float>   *pfEcalE;
    std::vector<float>   *pfEcalEraw;
    std::vector<float>   *pfHcalE;
    std::vector<float>   *pfHcalEraw;
    std::vector<unsigned char> *trkAlgo;
    std::vector<float>   *trkPtError;
    std::vector<unsigned char> *trkNHit;
    std::vector<float>   *trkChi2;
    std::vector<unsigned char> *trkNdof;
    std::vector<unsigned char> *trkNlayer;
    std::vector<bool> *highPurity;
    std::vector<float> *trkMVA;
    std::vector<float> *trkDz1;
    std::vector<float> *trkDzError1;
    std::vector<float> *trkDxy1;
    std::vector<float> *trkDxyError1;

    // List of branches
    TBranch        *b_nPFpart;   //!
    TBranch        *b_pfKey;   //!
    TBranch        *b_pfId;   //!
    TBranch        *b_pfPt;   //!
    TBranch        *b_pfEnergy;   //!
    TBranch        *b_pfEta;   //!
    TBranch        *b_pfPhi;   //!
    TBranch        *b_pfM;   //!
    TBranch        *b_pfvx;   //!
    TBranch        *b_pfvy;   //!
    TBranch        *b_pfvz;   //!
    TBranch        *b_pfEcalE;   //!
    TBranch        *b_pfEcalEraw;   //!
    TBranch        *b_pfHcalE;   //!
    TBranch        *b_pfHcalEraw;   //!
    TBranch        *b_trkAlgo;   //!
    TBranch        *b_trkPtError;   //!
    TBranch        *b_trkNHit;   //!
    TBranch        *b_trkChi2;   //!
    TBranch        *b_trkNdof;   //!
    TBranch        *b_trkNlayer;   //!
    TBranch        *b_highPurity;   //!
    TBranch        *b_trkMVA;   //!
    TBranch        *b_trkDz1;   //!
    TBranch        *b_trkDzError1;   //!
    TBranch        *b_trkDxy1;   //!
    TBranch        *b_trkDxyError1;   //!
};

void pfCand::setupTreeForReading(TTree *t)
{
    b_nPFpart = 0;
    b_pfKey = 0;
    b_pfId = 0;
    b_pfPt = 0;
    b_pfEnergy = 0;
    b_pfEta = 0;
    b_pfPhi = 0;
    b_pfM = 0;
    b_pfvx = 0;
    b_pfvy = 0;
    b_pfvz = 0;
    b_pfEcalE = 0;
    b_pfEcalEraw = 0;
    b_pfHcalE = 0;
    b_pfHcalEraw = 0;
    b_trkAlgo = 0;
    b_trkPtError = 0;
    b_trkNHit = 0;
    b_trkChi2 = 0;
    b_trkNdof = 0;
    b_trkNlayer = 0;
    b_highPurity = 0;
    b_trkMVA = 0;
    b_trkDz1 = 0;
    b_trkDzError1 = 0;
    b_trkDxy1 = 0;
    b_trkDxyError1 = 0;

    // Set branch addresses and branch pointers
    if (t->GetBranch("nPFpart"))  t->SetBranchAddress("nPFpart", &nPFpart, &b_nPFpart);
    if (t->GetBranch("pfKey"))  t->SetBranchAddress("pfKey", &pfKey, &b_pfKey);
    if (t->GetBranch("pfId"))  t->SetBranchAddress("pfId", &pfId, &b_pfId);
    if (t->GetBranch("pfPt"))  t->SetBranchAddress("pfPt", &pfPt, &b_pfPt);
    if (t->GetBranch("pfEnergy"))  t->SetBranchAddress("pfEnergy", &pfEnergy, &b_pfEnergy);
    if (t->GetBranch("pfEta"))  t->SetBranchAddress("pfEta", &pfEta, &b_pfEta);
    if (t->GetBranch("pfPhi"))  t->SetBranchAddress("pfPhi", &pfPhi, &b_pfPhi);
    if (t->GetBranch("pfM"))  t->SetBranchAddress("pfM", &pfM, &b_pfM);
    if (t->GetBranch("pfvx"))  t->SetBranchAddress("pfvx", &pfvx, &b_pfvx);
    if (t->GetBranch("pfvy"))  t->SetBranchAddress("pfvy", &pfvy, &b_pfvy);
    if (t->GetBranch("pfvz"))  t->SetBranchAddress("pfvz", &pfvz, &b_pfvz);
    if (t->GetBranch("pfEcalE"))  t->SetBranchAddress("pfEcalE", &pfEcalE, &b_pfEcalE);
    if (t->GetBranch("pfEcalEraw"))  t->SetBranchAddress("pfEcalEraw", &pfEcalEraw, &b_pfEcalEraw);
    if (t->GetBranch("pfHcalE"))  t->SetBranchAddress("pfHcalE", &pfHcalE, &b_pfHcalE);
    if (t->GetBranch("pfHcalEraw"))  t->SetBranchAddress("pfHcalEraw", &pfHcalEraw, &b_pfHcalEraw);
    if (t->GetBranch("trkAlgo"))  t->SetBranchAddress("trkAlgo", &trkAlgo, &b_trkAlgo);
    if (t->GetBranch("trkPtError"))  t->SetBranchAddress("trkPtError", &trkPtError, &b_trkPtError);
    if (t->GetBranch("trkNHit"))  t->SetBranchAddress("trkNHit", &trkNHit, &b_trkNHit);
    if (t->GetBranch("trkChi2"))  t->SetBranchAddress("trkChi2", &trkChi2, &b_trkChi2);
    if (t->GetBranch("trkNdof"))  t->SetBranchAddress("trkNdof", &trkNdof, &b_trkNdof);
    if (t->GetBranch("trkNlayer"))  t->SetBranchAddress("trkNlayer", &trkNlayer, &b_trkNlayer);
    if (t->GetBranch("highPurity"))  t->SetBranchAddress("highPurity", &highPurity, &b_highPurity);

    if (t->GetBranch("trkMVA"))  t->SetBranchAddress("trkMVA", &trkMVA, &b_trkMVA);

    if (t->GetBranch("trkDz1"))  t->SetBranchAddress("trkDz1", &trkDz1, &b_trkDz1);
    if (t->GetBranch("trkDzError1"))  t->SetBranchAddress("trkDzError1", &trkDzError1, &b_trkDzError1);
    if (t->GetBranch("trkDxy1"))  t->SetBranchAddress("trkDxy1", &trkDxy1, &b_trkDxy1);
    if (t->GetBranch("trkDxyError1"))  t->SetBranchAddress("trkDxyError1", &trkDxyError1, &b_trkDxyError1);
}

#endif
