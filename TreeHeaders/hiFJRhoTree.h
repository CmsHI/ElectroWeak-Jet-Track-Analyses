#ifndef TREEHEADERS_HIFJRHOTREE_H_
#define TREEHEADERS_HIFJRHOTREE_H_

#include <TTree.h>
#include <TBranch.h>

#include <vector>

class hiFJRho {
public :
    hiFJRho(){
        etaMin = 0;
        etaMax = 0;
        rho = 0;
        rhom = 0;
        rhoCorr = 0;
        rhomCorr = 0;
        rhoCorr1Bin = 0;
        rhomCorr1Bin = 0;
        rhoGrid = 0;
        meanRhoGrid = 0;
        etaMinRhoGrid = 0;
        etaMaxRhoGrid = 0;
        ptJets = 0;
        etaJets = 0;
        areaJets = 0;
    };
    ~hiFJRho(){};
    void setupTreeForReading(TTree *t);
    void setupTreeForWriting(TTree *t);
    double getRho(double eta);

    // Declaration of leaf types
    std::vector<double>  *etaMin;
    std::vector<double>  *etaMax;
    std::vector<double>  *rho;
    std::vector<double>  *rhom;
    std::vector<double>  *rhoCorr;
    std::vector<double>  *rhomCorr;
    std::vector<double>  *rhoCorr1Bin;
    std::vector<double>  *rhomCorr1Bin;
    std::vector<double>  *rhoGrid;
    std::vector<double>  *meanRhoGrid;
    std::vector<double>  *etaMinRhoGrid;
    std::vector<double>  *etaMaxRhoGrid;
    std::vector<double>  *ptJets;
    std::vector<double>  *etaJets;
    std::vector<double>  *areaJets;

    // List of branches
    TBranch        *b_etaMin;   //!
    TBranch        *b_etaMax;   //!
    TBranch        *b_rho;   //!
    TBranch        *b_rhom;   //!
    TBranch        *b_rhoCorr;   //!
    TBranch        *b_rhomCorr;   //!
    TBranch        *b_rhoCorr1Bin;   //!
    TBranch        *b_rhomCorr1Bin;   //!
    TBranch        *b_rhoGrid;   //!
    TBranch        *b_meanRhoGrid;   //!
    TBranch        *b_etaMinRhoGrid;   //!
    TBranch        *b_etaMaxRhoGrid;   //!
    TBranch        *b_ptJets;   //!
    TBranch        *b_etaJets;   //!
    TBranch        *b_areaJets;   //!
};

void hiFJRho::setupTreeForReading(TTree *t)
{
    b_etaMin = 0;
    b_etaMax = 0;
    b_rho = 0;
    b_rhom = 0;
    b_rhoCorr = 0;
    b_rhomCorr = 0;
    b_rhoCorr1Bin = 0;
    b_rhomCorr1Bin = 0;
    b_rhoGrid = 0;
    b_meanRhoGrid = 0;
    b_etaMinRhoGrid = 0;
    b_etaMaxRhoGrid = 0;
    b_ptJets = 0;
    b_etaJets = 0;
    b_areaJets = 0;

    // Set branch addresses and branch pointers
    if (t->GetBranch("etaMin"))  t->SetBranchAddress("etaMin", &etaMin, &b_etaMin);
    if (t->GetBranch("etaMax"))  t->SetBranchAddress("etaMax", &etaMax, &b_etaMax);
    if (t->GetBranch("rho"))  t->SetBranchAddress("rho", &rho, &b_rho);
    if (t->GetBranch("rhom"))  t->SetBranchAddress("rhom", &rhom, &b_rhom);
    if (t->GetBranch("rhoCorr"))  t->SetBranchAddress("rhoCorr", &rhoCorr, &b_rhoCorr);
    if (t->GetBranch("rhomCorr"))  t->SetBranchAddress("rhomCorr", &rhomCorr, &b_rhomCorr);
    if (t->GetBranch("rhoCorr1Bin"))  t->SetBranchAddress("rhoCorr1Bin", &rhoCorr1Bin, &b_rhoCorr1Bin);
    if (t->GetBranch("rhomCorr1Bin"))  t->SetBranchAddress("rhomCorr1Bin", &rhomCorr1Bin, &b_rhomCorr1Bin);
    if (t->GetBranch("rhoGrid"))  t->SetBranchAddress("rhoGrid", &rhoGrid, &b_rhoGrid);
    if (t->GetBranch("meanRhoGrid"))  t->SetBranchAddress("meanRhoGrid", &meanRhoGrid, &b_meanRhoGrid);
    if (t->GetBranch("etaMinRhoGrid"))  t->SetBranchAddress("etaMinRhoGrid", &etaMinRhoGrid, &b_etaMinRhoGrid);
    if (t->GetBranch("etaMaxRhoGrid"))  t->SetBranchAddress("etaMaxRhoGrid", &etaMaxRhoGrid, &b_etaMaxRhoGrid);
    if (t->GetBranch("ptJets"))  t->SetBranchAddress("ptJets", &ptJets, &b_ptJets);
    if (t->GetBranch("etaJets"))  t->SetBranchAddress("etaJets", &etaJets, &b_etaJets);
    if (t->GetBranch("areaJets"))  t->SetBranchAddress("areaJets", &areaJets, &b_areaJets);
}

void hiFJRho::setupTreeForWriting(TTree *t)
{
    t->Branch("etaMin", &etaMin, "etaMin/F");
    t->Branch("etaMax", &etaMax, "etaMax/F");
    t->Branch("rho", &rho, "rho/F");
    t->Branch("rhom", &rhom, "rhom/F");
    t->Branch("rhoCorr", &rhoCorr, "rhoCorr/F");
    t->Branch("rhomCorr", &rhomCorr, "rhomCorr/F");
    t->Branch("rhoCorr1Bin", &rhoCorr1Bin, "rhoCorr1Bin/F");
    t->Branch("rhomCorr1Bin", &rhomCorr1Bin, "rhomCorr1Bin/F");
    t->Branch("rhoGrid", &rhoGrid, "rhoGrid/F");
    t->Branch("meanRhoGrid", &meanRhoGrid, "meanRhoGrid/F");
    t->Branch("etaMinRhoGrid", &etaMinRhoGrid, "etaMinRhoGrid/F");
    t->Branch("etaMaxRhoGrid", &etaMaxRhoGrid, "etaMaxRhoGrid/F");
    t->Branch("ptJets", &ptJets, "ptJets/F");
    t->Branch("etaJets", &etaJets, "etaJets/F");
    t->Branch("areaJets", &areaJets, "areaJets/F");
}

double hiFJRho::getRho(double eta)
{
    int nEtas = (*etaMin).size();
    for (int i = 0; i < nEtas; ++i) {

        if ((*etaMin)[i] <= eta && eta < (*etaMax)[i]) {
            return (*rho)[i];
        }
    }

    return -1;
}

#endif
