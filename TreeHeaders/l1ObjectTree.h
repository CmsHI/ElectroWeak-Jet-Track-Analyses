#ifndef TREEHEADERS_L1OBJECTTREE_H_
#define TREEHEADERS_L1OBJECTTREE_H_

#include <TTree.h>
#include <TBranch.h>

#include "vector"

class l1Object {
public :
    l1Object(){

        egEt = 0;
        egEta = 0;
        egPhi = 0;
        egIEt = 0;
        egIEta = 0;
        egIPhi = 0;
        egIso = 0;
        egBx = 0;
        egTowerIPhi = 0;
        egTowerIEta = 0;
        egRawEt = 0;
        egIsoEt = 0;
        egFootprintEt = 0;
        egNTT = 0;
        egShape = 0;
        egTowerHoE = 0;
        egHwQual = 0;

        tauEt = 0;
        tauEta = 0;
        tauPhi = 0;
        tauIEt = 0;
        tauIEta = 0;
        tauIPhi = 0;
        tauIso = 0;
        tauBx = 0;
        tauTowerIPhi = 0;
        tauTowerIEta = 0;
        tauRawEt = 0;
        tauIsoEt = 0;
        tauNTT = 0;
        tauHasEM = 0;
        tauIsMerged = 0;
        tauHwQual = 0;

        jetEt = 0;
        jetEta = 0;
        jetPhi = 0;
        jetIEt = 0;
        jetIEta = 0;
        jetIPhi = 0;
        jetBx = 0;
        jetTowerIPhi = 0;
        jetTowerIEta = 0;
        jetRawEt = 0;
        jetSeedEt = 0;
        jetPUEt = 0;
        jetPUDonutEt0 = 0;
        jetPUDonutEt1 = 0;
        jetPUDonutEt2 = 0;
        jetPUDonutEt3 = 0;

        muonEt = 0;
        muonEta = 0;
        muonPhi = 0;
        muonEtaAtVtx = 0;
        muonPhiAtVtx = 0;
        muonIEt = 0;
        muonIEta = 0;
        muonIPhi = 0;
        muonIEtaAtVtx = 0;
        muonIPhiAtVtx = 0;
        muonIDEta = 0;
        muonIDPhi = 0;
        muonChg = 0;
        muonIso = 0;
        muonQual = 0;
        muonTfMuonIdx = 0;
        muonBx = 0;

        sumType = 0;
        sumEt = 0;
        sumPhi = 0;
        sumIEt = 0;
        sumIPhi = 0;
        sumBx = 0;
    };
    ~l1Object(){};
    void reset(){ l1Object(); };
    void setupTreeForReading(TTree *t);
    void setupTreeForWriting(TTree *t);

    // Declaration of leaf types
    UShort_t        nEGs;
    std::vector<float>   *egEt;
    std::vector<float>   *egEta;
    std::vector<float>   *egPhi;
    std::vector<short>   *egIEt;
    std::vector<short>   *egIEta;
    std::vector<short>   *egIPhi;
    std::vector<short>   *egIso;
    std::vector<short>   *egBx;
    std::vector<short>   *egTowerIPhi;
    std::vector<short>   *egTowerIEta;
    std::vector<short>   *egRawEt;
    std::vector<short>   *egIsoEt;
    std::vector<short>   *egFootprintEt;
    std::vector<short>   *egNTT;
    std::vector<short>   *egShape;
    std::vector<short>   *egTowerHoE;
    std::vector<short>   *egHwQual;
    UShort_t        nTaus;
    std::vector<float>   *tauEt;
    std::vector<float>   *tauEta;
    std::vector<float>   *tauPhi;
    std::vector<short>   *tauIEt;
    std::vector<short>   *tauIEta;
    std::vector<short>   *tauIPhi;
    std::vector<short>   *tauIso;
    std::vector<short>   *tauBx;
    std::vector<short>   *tauTowerIPhi;
    std::vector<short>   *tauTowerIEta;
    std::vector<short>   *tauRawEt;
    std::vector<short>   *tauIsoEt;
    std::vector<short>   *tauNTT;
    std::vector<short>   *tauHasEM;
    std::vector<short>   *tauIsMerged;
    std::vector<short>   *tauHwQual;
    UShort_t        nJets;
    std::vector<float>   *jetEt;
    std::vector<float>   *jetEta;
    std::vector<float>   *jetPhi;
    std::vector<short>   *jetIEt;
    std::vector<short>   *jetIEta;
    std::vector<short>   *jetIPhi;
    std::vector<short>   *jetBx;
    std::vector<short>   *jetTowerIPhi;
    std::vector<short>   *jetTowerIEta;
    std::vector<short>   *jetRawEt;
    std::vector<short>   *jetSeedEt;
    std::vector<short>   *jetPUEt;
    std::vector<short>   *jetPUDonutEt0;
    std::vector<short>   *jetPUDonutEt1;
    std::vector<short>   *jetPUDonutEt2;
    std::vector<short>   *jetPUDonutEt3;
    UShort_t        nMuons;
    std::vector<float>   *muonEt;
    std::vector<float>   *muonEta;
    std::vector<float>   *muonPhi;
    std::vector<float>   *muonEtaAtVtx;
    std::vector<float>   *muonPhiAtVtx;
    std::vector<short>   *muonIEt;
    std::vector<short>   *muonIEta;
    std::vector<short>   *muonIPhi;
    std::vector<short>   *muonIEtaAtVtx;
    std::vector<short>   *muonIPhiAtVtx;
    std::vector<short>   *muonIDEta;
    std::vector<short>   *muonIDPhi;
    std::vector<short>   *muonChg;
    std::vector<unsigned short> *muonIso;
    std::vector<unsigned short> *muonQual;
    std::vector<unsigned short> *muonTfMuonIdx;
    std::vector<short>   *muonBx;
    UShort_t        nSums;
    std::vector<short>   *sumType;
    std::vector<float>   *sumEt;
    std::vector<float>   *sumPhi;
    std::vector<short>   *sumIEt;
    std::vector<short>   *sumIPhi;
    std::vector<float>   *sumBx;

    // List of branches
    TBranch        *b_nEGs;   //!
    TBranch        *b_egEt;   //!
    TBranch        *b_egEta;   //!
    TBranch        *b_egPhi;   //!
    TBranch        *b_egIEt;   //!
    TBranch        *b_egIEta;   //!
    TBranch        *b_egIPhi;   //!
    TBranch        *b_egIso;   //!
    TBranch        *b_egBx;   //!
    TBranch        *b_egTowerIPhi;   //!
    TBranch        *b_egTowerIEta;   //!
    TBranch        *b_egRawEt;   //!
    TBranch        *b_egIsoEt;   //!
    TBranch        *b_egFootprintEt;   //!
    TBranch        *b_egNTT;   //!
    TBranch        *b_egShape;   //!
    TBranch        *b_egTowerHoE;   //!
    TBranch        *b_egHwQual;   //!
    TBranch        *b_nTaus;   //!
    TBranch        *b_tauEt;   //!
    TBranch        *b_tauEta;   //!
    TBranch        *b_tauPhi;   //!
    TBranch        *b_tauIEt;   //!
    TBranch        *b_tauIEta;   //!
    TBranch        *b_tauIPhi;   //!
    TBranch        *b_tauIso;   //!
    TBranch        *b_tauBx;   //!
    TBranch        *b_tauTowerIPhi;   //!
    TBranch        *b_tauTowerIEta;   //!
    TBranch        *b_tauRawEt;   //!
    TBranch        *b_tauIsoEt;   //!
    TBranch        *b_tauNTT;   //!
    TBranch        *b_tauHasEM;   //!
    TBranch        *b_tauIsMerged;   //!
    TBranch        *b_tauHwQual;   //!
    TBranch        *b_nJets;   //!
    TBranch        *b_jetEt;   //!
    TBranch        *b_jetEta;   //!
    TBranch        *b_jetPhi;   //!
    TBranch        *b_jetIEt;   //!
    TBranch        *b_jetIEta;   //!
    TBranch        *b_jetIPhi;   //!
    TBranch        *b_jetBx;   //!
    TBranch        *b_jetTowerIPhi;   //!
    TBranch        *b_jetTowerIEta;   //!
    TBranch        *b_jetRawEt;   //!
    TBranch        *b_jetSeedEt;   //!
    TBranch        *b_jetPUEt;   //!
    TBranch        *b_jetPUDonutEt0;   //!
    TBranch        *b_jetPUDonutEt1;   //!
    TBranch        *b_jetPUDonutEt2;   //!
    TBranch        *b_jetPUDonutEt3;   //!
    TBranch        *b_nMuons;   //!
    TBranch        *b_muonEt;   //!
    TBranch        *b_muonEta;   //!
    TBranch        *b_muonPhi;   //!
    TBranch        *b_muonEtaAtVtx;   //!
    TBranch        *b_muonPhiAtVtx;   //!
    TBranch        *b_muonIEt;   //!
    TBranch        *b_muonIEta;   //!
    TBranch        *b_muonIPhi;   //!
    TBranch        *b_muonIEtaAtVtx;   //!
    TBranch        *b_muonIPhiAtVtx;   //!
    TBranch        *b_muonIDEta;   //!
    TBranch        *b_muonIDPhi;   //!
    TBranch        *b_muonChg;   //!
    TBranch        *b_muonIso;   //!
    TBranch        *b_muonQual;   //!
    TBranch        *b_muonTfMuonIdx;   //!
    TBranch        *b_muonBx;   //!
    TBranch        *b_nSums;   //!
    TBranch        *b_sumType;   //!
    TBranch        *b_sumEt;   //!
    TBranch        *b_sumPhi;   //!
    TBranch        *b_sumIEt;   //!
    TBranch        *b_sumIPhi;   //!
    TBranch        *b_sumBx;   //!
};

void l1Object::setupTreeForReading(TTree *t)
{

    if (t->GetBranch("nEGs"))  t->SetBranchAddress("nEGs",&nEGs,&b_nEGs);
    if (t->GetBranch("egEt"))  t->SetBranchAddress("egEt",&egEt,&b_egEt);
    if (t->GetBranch("egEta"))  t->SetBranchAddress("egEta",&egEta,&b_egEta);
    if (t->GetBranch("egPhi"))  t->SetBranchAddress("egPhi",&egPhi,&b_egPhi);
    if (t->GetBranch("egIEt"))  t->SetBranchAddress("egIEt",&egIEt,&b_egIEt);
    if (t->GetBranch("egIEta"))  t->SetBranchAddress("egIEta",&egIEta,&b_egIEta);
    if (t->GetBranch("egIPhi"))  t->SetBranchAddress("egIPhi",&egIPhi,&b_egIPhi);
    if (t->GetBranch("egIso"))  t->SetBranchAddress("egIso",&egIso,&b_egIso);
    if (t->GetBranch("egBx"))  t->SetBranchAddress("egBx",&egBx,&b_egBx);
    if (t->GetBranch("egTowerIPhi"))  t->SetBranchAddress("egTowerIPhi",&egTowerIPhi,&b_egTowerIPhi);
    if (t->GetBranch("egTowerIEta"))  t->SetBranchAddress("egTowerIEta",&egTowerIEta,&b_egTowerIEta);
    if (t->GetBranch("egRawEt"))  t->SetBranchAddress("egRawEt",&egRawEt,&b_egRawEt);
    if (t->GetBranch("egIsoEt"))  t->SetBranchAddress("egIsoEt",&egIsoEt,&b_egIsoEt);
    if (t->GetBranch("egFootprintEt"))  t->SetBranchAddress("egFootprintEt",&egFootprintEt,&b_egFootprintEt);
    if (t->GetBranch("egNTT"))  t->SetBranchAddress("egNTT",&egNTT,&b_egNTT);
    if (t->GetBranch("egShape"))  t->SetBranchAddress("egShape",&egShape,&b_egShape);
    if (t->GetBranch("egTowerHoE"))  t->SetBranchAddress("egTowerHoE",&egTowerHoE,&b_egTowerHoE);
    if (t->GetBranch("egHwQual"))  t->SetBranchAddress("egHwQual",&egHwQual,&b_egHwQual);
    if (t->GetBranch("nTaus"))  t->SetBranchAddress("nTaus",&nTaus,&b_nTaus);
    if (t->GetBranch("tauEt"))  t->SetBranchAddress("tauEt",&tauEt,&b_tauEt);
    if (t->GetBranch("tauEta"))  t->SetBranchAddress("tauEta",&tauEta,&b_tauEta);
    if (t->GetBranch("tauPhi"))  t->SetBranchAddress("tauPhi",&tauPhi,&b_tauPhi);
    if (t->GetBranch("tauIEt"))  t->SetBranchAddress("tauIEt",&tauIEt,&b_tauIEt);
    if (t->GetBranch("tauIEta"))  t->SetBranchAddress("tauIEta",&tauIEta,&b_tauIEta);
    if (t->GetBranch("tauIPhi"))  t->SetBranchAddress("tauIPhi",&tauIPhi,&b_tauIPhi);
    if (t->GetBranch("tauIso"))  t->SetBranchAddress("tauIso",&tauIso,&b_tauIso);
    if (t->GetBranch("tauBx"))  t->SetBranchAddress("tauBx",&tauBx,&b_tauBx);
    if (t->GetBranch("tauTowerIPhi"))  t->SetBranchAddress("tauTowerIPhi",&tauTowerIPhi,&b_tauTowerIPhi);
    if (t->GetBranch("tauTowerIEta"))  t->SetBranchAddress("tauTowerIEta",&tauTowerIEta,&b_tauTowerIEta);
    if (t->GetBranch("tauRawEt"))  t->SetBranchAddress("tauRawEt",&tauRawEt,&b_tauRawEt);
    if (t->GetBranch("tauIsoEt"))  t->SetBranchAddress("tauIsoEt",&tauIsoEt,&b_tauIsoEt);
    if (t->GetBranch("tauNTT"))  t->SetBranchAddress("tauNTT",&tauNTT,&b_tauNTT);
    if (t->GetBranch("tauHasEM"))  t->SetBranchAddress("tauHasEM",&tauHasEM,&b_tauHasEM);
    if (t->GetBranch("tauIsMerged"))  t->SetBranchAddress("tauIsMerged",&tauIsMerged,&b_tauIsMerged);
    if (t->GetBranch("tauHwQual"))  t->SetBranchAddress("tauHwQual",&tauHwQual,&b_tauHwQual);
    if (t->GetBranch("nJets"))  t->SetBranchAddress("nJets",&nJets,&b_nJets);
    if (t->GetBranch("jetEt"))  t->SetBranchAddress("jetEt",&jetEt,&b_jetEt);
    if (t->GetBranch("jetEta"))  t->SetBranchAddress("jetEta",&jetEta,&b_jetEta);
    if (t->GetBranch("jetPhi"))  t->SetBranchAddress("jetPhi",&jetPhi,&b_jetPhi);
    if (t->GetBranch("jetIEt"))  t->SetBranchAddress("jetIEt",&jetIEt,&b_jetIEt);
    if (t->GetBranch("jetIEta"))  t->SetBranchAddress("jetIEta",&jetIEta,&b_jetIEta);
    if (t->GetBranch("jetIPhi"))  t->SetBranchAddress("jetIPhi",&jetIPhi,&b_jetIPhi);
    if (t->GetBranch("jetBx"))  t->SetBranchAddress("jetBx",&jetBx,&b_jetBx);
    if (t->GetBranch("jetTowerIPhi"))  t->SetBranchAddress("jetTowerIPhi",&jetTowerIPhi,&b_jetTowerIPhi);
    if (t->GetBranch("jetTowerIEta"))  t->SetBranchAddress("jetTowerIEta",&jetTowerIEta,&b_jetTowerIEta);
    if (t->GetBranch("jetRawEt"))  t->SetBranchAddress("jetRawEt",&jetRawEt,&b_jetRawEt);
    if (t->GetBranch("jetSeedEt"))  t->SetBranchAddress("jetSeedEt",&jetSeedEt,&b_jetSeedEt);
    if (t->GetBranch("jetPUEt"))  t->SetBranchAddress("jetPUEt",&jetPUEt,&b_jetPUEt);
    if (t->GetBranch("jetPUDonutEt0"))  t->SetBranchAddress("jetPUDonutEt0",&jetPUDonutEt0,&b_jetPUDonutEt0);
    if (t->GetBranch("jetPUDonutEt1"))  t->SetBranchAddress("jetPUDonutEt1",&jetPUDonutEt1,&b_jetPUDonutEt1);
    if (t->GetBranch("jetPUDonutEt2"))  t->SetBranchAddress("jetPUDonutEt2",&jetPUDonutEt2,&b_jetPUDonutEt2);
    if (t->GetBranch("jetPUDonutEt3"))  t->SetBranchAddress("jetPUDonutEt3",&jetPUDonutEt3,&b_jetPUDonutEt3);
    if (t->GetBranch("nMuons"))  t->SetBranchAddress("nMuons",&nMuons,&b_nMuons);
    if (t->GetBranch("muonEt"))  t->SetBranchAddress("muonEt",&muonEt,&b_muonEt);
    if (t->GetBranch("muonEta"))  t->SetBranchAddress("muonEta",&muonEta,&b_muonEta);
    if (t->GetBranch("muonPhi"))  t->SetBranchAddress("muonPhi",&muonPhi,&b_muonPhi);
    if (t->GetBranch("muonEtaAtVtx"))  t->SetBranchAddress("muonEtaAtVtx",&muonEtaAtVtx,&b_muonEtaAtVtx);
    if (t->GetBranch("muonPhiAtVtx"))  t->SetBranchAddress("muonPhiAtVtx",&muonPhiAtVtx,&b_muonPhiAtVtx);
    if (t->GetBranch("muonIEt"))  t->SetBranchAddress("muonIEt",&muonIEt,&b_muonIEt);
    if (t->GetBranch("muonIEta"))  t->SetBranchAddress("muonIEta",&muonIEta,&b_muonIEta);
    if (t->GetBranch("muonIPhi"))  t->SetBranchAddress("muonIPhi",&muonIPhi,&b_muonIPhi);
    if (t->GetBranch("muonIEtaAtVtx"))  t->SetBranchAddress("muonIEtaAtVtx",&muonIEtaAtVtx,&b_muonIEtaAtVtx);
    if (t->GetBranch("muonIPhiAtVtx"))  t->SetBranchAddress("muonIPhiAtVtx",&muonIPhiAtVtx,&b_muonIPhiAtVtx);
    if (t->GetBranch("muonIDEta"))  t->SetBranchAddress("muonIDEta",&muonIDEta,&b_muonIDEta);
    if (t->GetBranch("muonIDPhi"))  t->SetBranchAddress("muonIDPhi",&muonIDPhi,&b_muonIDPhi);
    if (t->GetBranch("muonChg"))  t->SetBranchAddress("muonChg",&muonChg,&b_muonChg);
    if (t->GetBranch("muonIso"))  t->SetBranchAddress("muonIso",&muonIso,&b_muonIso);
    if (t->GetBranch("muonQual"))  t->SetBranchAddress("muonQual",&muonQual,&b_muonQual);
    if (t->GetBranch("muonTfMuonIdx"))  t->SetBranchAddress("muonTfMuonIdx",&muonTfMuonIdx,&b_muonTfMuonIdx);
    if (t->GetBranch("muonBx"))  t->SetBranchAddress("muonBx",&muonBx,&b_muonBx);
    if (t->GetBranch("nSums"))  t->SetBranchAddress("nSums",&nSums,&b_nSums);
    if (t->GetBranch("sumType"))  t->SetBranchAddress("sumType",&sumType,&b_sumType);
    if (t->GetBranch("sumEt"))  t->SetBranchAddress("sumEt",&sumEt,&b_sumEt);
    if (t->GetBranch("sumPhi"))  t->SetBranchAddress("sumPhi",&sumPhi,&b_sumPhi);
    if (t->GetBranch("sumIEt"))  t->SetBranchAddress("sumIEt",&sumIEt,&b_sumIEt);
    if (t->GetBranch("sumIPhi"))  t->SetBranchAddress("sumIPhi",&sumIPhi,&b_sumIPhi);
    if (t->GetBranch("sumBx"))  t->SetBranchAddress("sumBx",&sumBx,&b_sumBx);
}

void l1Object::setupTreeForWriting(TTree *t) {

    t->Branch("nEGs", nEGs);
    t->Branch("egEt", egEt);
    t->Branch("egEta", egEta);
    t->Branch("egPhi", egPhi);
    t->Branch("egIEt", egIEt);
    t->Branch("egIEta", egIEta);
    t->Branch("egIPhi", egIPhi);
    t->Branch("egIso", egIso);
    t->Branch("egBx", egBx);
    t->Branch("egTowerIPhi", egTowerIPhi);
    t->Branch("egTowerIEta", egTowerIEta);
    t->Branch("egRawEt", egRawEt);
    t->Branch("egIsoEt", egIsoEt);
    t->Branch("egFootprintEt", egFootprintEt);
    t->Branch("egNTT", egNTT);
    t->Branch("egShape", egShape);
    t->Branch("egTowerHoE", egTowerHoE);
    t->Branch("egHwQual", egHwQual);
    t->Branch("nTaus", nTaus);
    t->Branch("tauEt", tauEt);
    t->Branch("tauEta", tauEta);
    t->Branch("tauPhi", tauPhi);
    t->Branch("tauIEt", tauIEt);
    t->Branch("tauIEta", tauIEta);
    t->Branch("tauIPhi", tauIPhi);
    t->Branch("tauIso", tauIso);
    t->Branch("tauBx", tauBx);
    t->Branch("tauTowerIPhi", tauTowerIPhi);
    t->Branch("tauTowerIEta", tauTowerIEta);
    t->Branch("tauRawEt", tauRawEt);
    t->Branch("tauIsoEt", tauIsoEt);
    t->Branch("tauNTT", tauNTT);
    t->Branch("tauHasEM", tauHasEM);
    t->Branch("tauIsMerged", tauIsMerged);
    t->Branch("tauHwQual", tauHwQual);
    t->Branch("nJets", nJets);
    t->Branch("jetEt", jetEt);
    t->Branch("jetEta", jetEta);
    t->Branch("jetPhi", jetPhi);
    t->Branch("jetIEt", jetIEt);
    t->Branch("jetIEta", jetIEta);
    t->Branch("jetIPhi", jetIPhi);
    t->Branch("jetBx", jetBx);
    t->Branch("jetTowerIPhi", jetTowerIPhi);
    t->Branch("jetTowerIEta", jetTowerIEta);
    t->Branch("jetRawEt", jetRawEt);
    t->Branch("jetSeedEt", jetSeedEt);
    t->Branch("jetPUEt", jetPUEt);
    t->Branch("jetPUDonutEt0", jetPUDonutEt0);
    t->Branch("jetPUDonutEt1", jetPUDonutEt1);
    t->Branch("jetPUDonutEt2", jetPUDonutEt2);
    t->Branch("jetPUDonutEt3", jetPUDonutEt3);
    t->Branch("nMuons", nMuons);
    t->Branch("muonEt", muonEt);
    t->Branch("muonEta", muonEta);
    t->Branch("muonPhi", muonPhi);
    t->Branch("muonEtaAtVtx", muonEtaAtVtx);
    t->Branch("muonPhiAtVtx", muonPhiAtVtx);
    t->Branch("muonIEt", muonIEt);
    t->Branch("muonIEta", muonIEta);
    t->Branch("muonIPhi", muonIPhi);
    t->Branch("muonIEtaAtVtx", muonIEtaAtVtx);
    t->Branch("muonIPhiAtVtx", muonIPhiAtVtx);
    t->Branch("muonIDEta", muonIDEta);
    t->Branch("muonIDPhi", muonIDPhi);
    t->Branch("muonChg", muonChg);
    t->Branch("muonIso", muonIso);
    t->Branch("muonQual", muonQual);
    t->Branch("muonTfMuonIdx", muonTfMuonIdx);
    t->Branch("muonBx", muonBx);
    t->Branch("nSums", nSums);
    t->Branch("sumType", sumType);
    t->Branch("sumEt", sumEt);
    t->Branch("sumPhi", sumPhi);
    t->Branch("sumIEt", sumIEt);
    t->Branch("sumIPhi", sumIPhi);
    t->Branch("sumBx", sumBx);
}

#endif
