#ifndef TREEHEADERS_TRACKTREE_H_
#define TREEHEADERS_TRACKTREE_H_

#include <TTree.h>
#include <TBranch.h>

#include <vector>

const unsigned int maxNVtx = 10;
const unsigned int maxNVtxSim = 10;
const unsigned int maxNTrk = 50000;
const unsigned int maxNTrkTimesnVtx = 50000;

class Tracks {
public :
    Tracks(bool isMiniAOD = false)
    {
        ismAOD = isMiniAOD;

        xVtx_v = 0;
        yVtx_v = 0;
        zVtx_v = 0;
        xErrVtx = 0;
        yErrVtx = 0;
        zErrVtx = 0;
        chi2Vtx = 0;
        ndofVtx = 0;
        isFakeVtx = 0;
        nTracksVtx = 0;
        ptSumVtx = 0;

        trkPt_v = 0;
        trkPtError_v = 0;
        trkEta_v = 0;
        trkPhi_v = 0;
        trkCharge_v = 0;
        trkPDGId = 0;
        trkNHits = 0;
        trkNPixHits = 0;
        trkNLayers = 0;
        trkNormChi2 = 0;
        highPurity_v = 0;
        pfEnergy = 0;
        pfEcal_v = 0;
        pfHcal_v = 0;
        trkAssociatedVtxIndx = 0;
        trkAssociatedVtxQuality = 0;
        trkDzAssociatedVtx = 0;
        trkDzErrAssociatedVtx = 0;
        trkDxyAssociatedVtx = 0;
        trkDxyErrAssociatedVtx = 0;
        trkFirstVtxQuality = 0;
        trkDzFirstVtx = 0;
        trkDzErrFirstVtx = 0;
        trkDxyFirstVtx = 0;
        trkDxyErrFirstVtx = 0;
    };
    ~Tracks(){};
    void setupTreeForReading(TTree *t);
    void setupTreeForWriting(TTree *t);

   bool ismAOD;

   // Declaration of leaf types
   Int_t           nEv;
   Int_t           nLumi;
   Int_t           nBX;
   Int_t           nRun;
   Int_t           N;
   Int_t           nVtx;
   Int_t           nTrk;
   Int_t           maxPtVtx;
   Int_t           maxMultVtx;
   Int_t           nTrkVtx[maxNVtx];   //[nVtx]
   Float_t         normChi2Vtx[maxNVtx];   //[nVtx]
   Float_t         sumPtVtx[maxNVtx];   //[nVtx]
   Float_t         xVtx[maxNVtx];   //[nVtx]
   Float_t         yVtx[maxNVtx];   //[nVtx]
   Float_t         zVtx[maxNVtx];   //[nVtx]
   Float_t         xVtxErr[maxNVtx];   //[nVtx]
   Float_t         yVtxErr[maxNVtx];   //[nVtx]
   Float_t         zVtxErr[maxNVtx];   //[nVtx]
   Float_t         vtxDist2D[maxNVtx];   //[nVtx]
   Float_t         vtxDist2DErr[maxNVtx];   //[nVtx]
   Float_t         vtxDist2DSig[maxNVtx];   //[nVtx]
   Float_t         vtxDist3D[maxNVtx];   //[nVtx]
   Float_t         vtxDist3DErr[maxNVtx];   //[nVtx]
   Float_t         vtxDist3DSig[maxNVtx];   //[nVtx]

   Int_t           nVtxSim;
   Float_t         xVtxSim[maxNVtxSim];   //[nVtxSim]
   Float_t         yVtxSim[maxNVtxSim];   //[nVtxSim]
   Float_t         zVtxSim[maxNVtxSim];   //[nVtxSim]

   Float_t         trkPt[maxNTrk];   //[nTrk]
   Float_t         trkPtError[maxNTrk];   //[nTrk]
   UChar_t         trkNHit[maxNTrk];   //[nTrk]
   UChar_t         trkNlayer[maxNTrk];   //[nTrk]
   Float_t         trkEta[maxNTrk];   //[nTrk]
   Float_t         trkPhi[maxNTrk];   //[nTrk]
   Int_t           trkCharge[maxNTrk];   //[nTrk]
   UChar_t         trkNVtx[maxNTrk];   //[nTrk]
   UInt_t          trkVtxIndex[maxNTrk];
   Int_t           nTrkTimesnVtx;
   Bool_t          trkAssocVtx[maxNTrkTimesnVtx];   //[nTrkTimesnVtx]
   Float_t         trkDxyOverDxyError[maxNTrkTimesnVtx];   //[nTrkTimesnVtx]
   Float_t         trkDzOverDzError[maxNTrkTimesnVtx];   //[nTrkTimesnVtx]
   Bool_t          highPurity[maxNTrk];   //[nTrk]
   Bool_t          tight[maxNTrk];   //[nTrk]
   Bool_t          loose[maxNTrk];   //[nTrk]
   Float_t         trkChi2[maxNTrk];   //[nTrk]
   UChar_t         trkNdof[maxNTrk];   //[nTrk]
   Float_t         trkDxy1[maxNTrk];   //[nTrk]
   Float_t         trkDxyError1[maxNTrk];   //[nTrk]
   Float_t         trkDz1[maxNTrk];   //[nTrk]
   Float_t         trkDzError1[maxNTrk];   //[nTrk]
   Bool_t          trkFake[maxNTrk];   //[nTrk]
   UChar_t         trkAlgo[maxNTrk];   //[nTrk]
   UChar_t         trkOriginalAlgo[maxNTrk];   //[nTrk]
   Float_t         trkMVA[maxNTrk];   //[nTrk]
   Bool_t          trkMVALoose[maxNTrk];
   Bool_t          trkMVATight[maxNTrk];   //[nTrk]
   Float_t         dedx[maxNTrk];
   Int_t           pfType[maxNTrk];   //[nTrk]
   Float_t         pfCandPt[maxNTrk];   //[nTrk]
   Float_t         pfEcal[maxNTrk];   //[nTrk]
   Float_t         pfHcal[maxNTrk];   //[nTrk]

   std::vector<float>   *xVtx_v;
   std::vector<float>   *yVtx_v;
   std::vector<float>   *zVtx_v;
   std::vector<float>   *xErrVtx;
   std::vector<float>   *yErrVtx;
   std::vector<float>   *zErrVtx;
   std::vector<float>   *chi2Vtx;
   std::vector<float>   *ndofVtx;
   std::vector<bool>    *isFakeVtx;
   std::vector<int>     *nTracksVtx;
   std::vector<float>   *ptSumVtx;

   std::vector<float>   *trkPt_v;
   std::vector<float>   *trkPtError_v;
   std::vector<float>   *trkEta_v;
   std::vector<float>   *trkPhi_v;
   std::vector<char>    *trkCharge_v;
   std::vector<int>     *trkPDGId;
   std::vector<char>    *trkNHits;
   std::vector<char>    *trkNPixHits;
   std::vector<char>    *trkNLayers;
   std::vector<float>   *trkNormChi2;
   std::vector<bool>    *highPurity_v;
   std::vector<float>   *pfEnergy;
   std::vector<float>   *pfEcal_v;
   std::vector<float>   *pfHcal_v;
   std::vector<int>     *trkAssociatedVtxIndx;
   std::vector<int>     *trkAssociatedVtxQuality;
   std::vector<float>   *trkDzAssociatedVtx;
   std::vector<float>   *trkDzErrAssociatedVtx;
   std::vector<float>   *trkDxyAssociatedVtx;
   std::vector<float>   *trkDxyErrAssociatedVtx;
   std::vector<int>     *trkFirstVtxQuality;
   std::vector<float>   *trkDzFirstVtx;
   std::vector<float>   *trkDzErrFirstVtx;
   std::vector<float>   *trkDxyFirstVtx;
   std::vector<float>   *trkDxyErrFirstVtx;

   // List of branches
   TBranch        *b_nEv;   //!
   TBranch        *b_nLumi;   //!
   TBranch        *b_nBX;   //!
   TBranch        *b_nRun;   //!
   TBranch        *b_N;   //!
   TBranch        *b_nVtx;   //!
   TBranch        *b_nTrk;   //!
   TBranch        *b_maxPtVtx;   //!
   TBranch        *b_maxMultVtx;   //!
   TBranch        *b_nTrkVtx;   //!
   TBranch        *b_normChi2Vtx;   //!
   TBranch        *b_sumPtVtx;   //!
   TBranch        *b_xVtx;   //!
   TBranch        *b_yVtx;   //!
   TBranch        *b_zVtx;   //!
   TBranch        *b_xVtxErr;   //!
   TBranch        *b_yVtxErr;   //!
   TBranch        *b_zVtxErr;   //!
   TBranch        *b_vtxDist2D;   //!
   TBranch        *b_vtxDist2DErr;   //!
   TBranch        *b_vtxDist2DSig;   //!
   TBranch        *b_vtxDist3D;   //!
   TBranch        *b_vtxDist3DErr;   //!
   TBranch        *b_vtxDist3DSig;   //!

   TBranch        *b_nVtxSim;   //!
   TBranch        *b_xVtxSim;   //!
   TBranch        *b_yVtxSim;   //!
   TBranch        *b_zVtxSim;   //!

   TBranch        *b_chi2Vtx;   //!
   TBranch        *b_ndofVtx;   //!
   TBranch        *b_isFakeVtx;   //!
   TBranch        *b_nTracksVtx;   //!
   TBranch        *b_ptSumVtx;   //!

   TBranch        *b_trkPt;   //!
   TBranch        *b_trkPtError;   //!
   TBranch        *b_trkNHit;   //!
   TBranch        *b_trkNlayer;   //!
   TBranch        *b_trkEta;   //!
   TBranch        *b_trkPhi;   //!
   TBranch        *b_trkCharge;   //!
   TBranch        *b_trkNVtx;   //!
   TBranch        *b_trkVtxIndex;
   TBranch        *b_nTrkTimesnVtx;   //!
   TBranch        *b_trkAssocVtx;   //!
   TBranch        *b_trkDxyOverDxyError;   //!
   TBranch        *b_trkDzOverDzError;   //!
   TBranch        *b_highPurity;   //!
   TBranch        *b_tight;   //!
   TBranch        *b_loose;   //!
   TBranch        *b_trkChi2;   //!
   TBranch        *b_trkNdof;   //!
   TBranch        *b_trkDxy1;   //!
   TBranch        *b_trkDxyError1;   //!
   TBranch        *b_trkDz1;   //!
   TBranch        *b_trkDzError1;   //!
   TBranch        *b_trkFake;   //!
   TBranch        *b_trkAlgo;   //!
   TBranch        *b_trkOriginalAlgo;   //!
   TBranch        *b_trkMVA;   //!
   TBranch        *b_trkMVALoose;
   TBranch        *b_trkMVATight;   //!
   TBranch        *b_dedx;
   TBranch        *b_pfType;   //!
   TBranch        *b_pfCandPt;   //!
   TBranch        *b_pfEnergy;   //!
   TBranch        *b_pfEcal;   //!
   TBranch        *b_pfHcal;   //!

   TBranch        *b_trkPDGId;   //!
   TBranch        *b_trkNHits;   //!
   TBranch        *b_trkNPixHits;   //!
   TBranch        *b_trkNLayers;   //!
   TBranch        *b_trkNormChi2;   //!
   TBranch        *b_trkAssociatedVtxIndx;   //!
   TBranch        *b_trkAssociatedVtxQuality;   //!
   TBranch        *b_trkDzAssociatedVtx;   //!
   TBranch        *b_trkDzErrAssociatedVtx;   //!
   TBranch        *b_trkDxyAssociatedVtx;   //!
   TBranch        *b_trkDxyErrAssociatedVtx;   //!
   TBranch        *b_trkFirstVtxQuality;   //!
   TBranch        *b_trkDzFirstVtx;   //!
   TBranch        *b_trkDzErrFirstVtx;   //!
   TBranch        *b_trkDxyFirstVtx;   //!
   TBranch        *b_trkDxyErrFirstVtx;   //!
};

void Tracks::setupTreeForReading(TTree *t)
{
    b_nEv = 0;
    b_nLumi = 0;
    b_nBX = 0;
    b_nRun = 0;
    b_N = 0;
    b_nVtx = 0;
    b_nTrk = 0;
    b_maxPtVtx = 0;
    b_maxMultVtx = 0;
    b_nTrkVtx = 0;
    b_normChi2Vtx = 0;
    b_sumPtVtx = 0;
    b_xVtx = 0;
    b_yVtx = 0;
    b_zVtx = 0;
    b_xVtxErr = 0;
    b_yVtxErr = 0;
    b_zVtxErr = 0;
    b_vtxDist2D = 0;
    b_vtxDist2DErr = 0;
    b_vtxDist2DSig = 0;
    b_vtxDist3D = 0;
    b_vtxDist3DErr = 0;
    b_vtxDist3DSig = 0;

    b_nVtxSim = 0;
    b_xVtxSim = 0;
    b_yVtxSim = 0;
    b_zVtxSim = 0;

    b_chi2Vtx = 0;
    b_ndofVtx = 0;
    b_isFakeVtx = 0;
    b_nTracksVtx = 0;
    b_ptSumVtx = 0;

    b_trkPt = 0;
    b_trkPtError = 0;
    b_trkNHit = 0;
    b_trkNlayer = 0;
    b_trkEta = 0;
    b_trkPhi = 0;
    b_trkCharge = 0;
    b_trkNVtx = 0;
    b_trkVtxIndex = 0;
    b_nTrkTimesnVtx = 0;
    b_trkAssocVtx = 0;
    b_trkDxyOverDxyError = 0;
    b_trkDzOverDzError = 0;
    b_highPurity = 0;
    b_tight = 0;
    b_loose = 0;
    b_trkChi2 = 0;
    b_trkNdof = 0;
    b_trkDxy1 = 0;
    b_trkDxyError1 = 0;
    b_trkDz1 = 0;
    b_trkDzError1 = 0;
    b_trkFake = 0;
    b_trkAlgo = 0;
    b_trkOriginalAlgo = 0;
    b_trkMVA = 0;
    b_trkMVALoose = 0;
    b_trkMVATight = 0;
    b_dedx = 0;
    b_pfType = 0;
    b_pfCandPt = 0;
    b_pfEnergy = 0;
    b_pfEcal = 0;
    b_pfHcal = 0;

    b_trkPDGId = 0;
    b_trkNHits = 0;
    b_trkNPixHits = 0;
    b_trkNLayers = 0;
    b_trkNormChi2 = 0;
    b_trkAssociatedVtxIndx = 0;
    b_trkAssociatedVtxQuality = 0;
    b_trkDzAssociatedVtx = 0;
    b_trkDzErrAssociatedVtx = 0;
    b_trkDxyAssociatedVtx = 0;
    b_trkDxyErrAssociatedVtx = 0;
    b_trkFirstVtxQuality = 0;
    b_trkDzFirstVtx = 0;
    b_trkDzErrFirstVtx = 0;
    b_trkDxyFirstVtx = 0;
    b_trkDxyErrFirstVtx = 0;

    // Set branch addresses and branch pointers
    if (t->GetBranch("nEv"))  t->SetBranchAddress("nEv", &nEv, &b_nEv);
    if (t->GetBranch("nLumi"))  t->SetBranchAddress("nLumi", &nLumi, &b_nLumi);
    if (t->GetBranch("nBX"))  t->SetBranchAddress("nBX", &nBX, &b_nBX);
    if (t->GetBranch("nRun"))  t->SetBranchAddress("nRun", &nRun, &b_nRun);
    if (t->GetBranch("N"))  t->SetBranchAddress("N", &N, &b_N);
    if (t->GetBranch("nVtx"))  t->SetBranchAddress("nVtx", &nVtx, &b_nVtx);
    if (t->GetBranch("nTrk"))  t->SetBranchAddress("nTrk", &nTrk, &b_nTrk);
    if (t->GetBranch("maxPtVtx"))  t->SetBranchAddress("maxPtVtx", &maxPtVtx, &b_maxPtVtx);
    if (t->GetBranch("maxMultVtx"))  t->SetBranchAddress("maxMultVtx", &maxMultVtx, &b_maxMultVtx);

    if (t->GetBranch("vtxDist2D"))  t->SetBranchAddress("vtxDist2D", &vtxDist2D, &b_vtxDist2D);
    if (t->GetBranch("vtxDist2DErr"))  t->SetBranchAddress("vtxDist2DErr", &vtxDist2DErr, &b_vtxDist2DErr);
    if (t->GetBranch("vtxDist2DSig"))  t->SetBranchAddress("vtxDist2DSig", &vtxDist2DSig, &b_vtxDist2DSig);
    if (t->GetBranch("vtxDist3D"))  t->SetBranchAddress("vtxDist3D", &vtxDist3D, &b_vtxDist3D);
    if (t->GetBranch("vtxDist3DErr"))  t->SetBranchAddress("vtxDist3DErr", &vtxDist3DErr, &b_vtxDist3DErr);
    if (t->GetBranch("vtxDist3DSig"))  t->SetBranchAddress("vtxDist3DSig", &vtxDist3DSig, &b_vtxDist3DSig);

    if (t->GetBranch("nVtxSim"))  t->SetBranchAddress("nVtxSim", &nVtxSim, &b_nVtxSim);
    if (t->GetBranch("xVtxSim"))  t->SetBranchAddress("xVtxSim", &xVtxSim, &b_xVtxSim);
    if (t->GetBranch("yVtxSim"))  t->SetBranchAddress("yVtxSim", &yVtxSim, &b_yVtxSim);
    if (t->GetBranch("zVtxSim"))  t->SetBranchAddress("zVtxSim", &zVtxSim, &b_zVtxSim);

    if (ismAOD) {
        if (t->GetBranch("xVtx"))  t->SetBranchAddress("xVtx", &xVtx_v, &b_xVtx);
        if (t->GetBranch("yVtx"))  t->SetBranchAddress("yVtx", &yVtx_v, &b_yVtx);
        if (t->GetBranch("zVtx"))  t->SetBranchAddress("zVtx", &zVtx_v, &b_zVtx);
        if (t->GetBranch("xErrVtx"))  t->SetBranchAddress("xErrVtx", &xErrVtx, &b_xVtxErr);
        if (t->GetBranch("yErrVtx"))  t->SetBranchAddress("yErrVtx", &yErrVtx, &b_yVtxErr);
        if (t->GetBranch("zErrVtx"))  t->SetBranchAddress("zErrVtx", &zErrVtx, &b_zVtxErr);
        if (t->GetBranch("chi2Vtx"))  t->SetBranchAddress("chi2Vtx", &chi2Vtx, &b_chi2Vtx);
        if (t->GetBranch("ndofVtx"))  t->SetBranchAddress("ndofVtx", &ndofVtx, &b_ndofVtx);
        if (t->GetBranch("isFakeVtx"))  t->SetBranchAddress("isFakeVtx", &isFakeVtx, &b_isFakeVtx);
        if (t->GetBranch("nTracksVtx"))  t->SetBranchAddress("nTracksVtx", &nTracksVtx, &b_nTracksVtx);
        if (t->GetBranch("ptSumVtx"))  t->SetBranchAddress("ptSumVtx", &ptSumVtx, &b_ptSumVtx);

        if (t->GetBranch("trkPt"))  t->SetBranchAddress("trkPt", &trkPt_v, &b_trkPt);
        if (t->GetBranch("trkPtError"))  t->SetBranchAddress("trkPtError", &trkPtError_v, &b_trkPtError);
        if (t->GetBranch("trkEta"))  t->SetBranchAddress("trkEta", &trkEta_v, &b_trkEta);
        if (t->GetBranch("trkPhi"))  t->SetBranchAddress("trkPhi", &trkPhi_v, &b_trkPhi);
        if (t->GetBranch("trkCharge"))  t->SetBranchAddress("trkCharge", &trkCharge_v, &b_trkCharge);
        if (t->GetBranch("trkPDGId"))  t->SetBranchAddress("trkPDGId", &trkPDGId, &b_trkPDGId);
        if (t->GetBranch("trkNHits"))  t->SetBranchAddress("trkNHits", &trkNHits, &b_trkNHits);
        if (t->GetBranch("trkNPixHits"))  t->SetBranchAddress("trkNPixHits", &trkNPixHits, &b_trkNPixHits);
        if (t->GetBranch("trkNLayers"))  t->SetBranchAddress("trkNLayers", &trkNLayers, &b_trkNLayers);
        if (t->GetBranch("trkNormChi2"))  t->SetBranchAddress("trkNormChi2", &trkNormChi2, &b_trkNormChi2);
        if (t->GetBranch("highPurity"))  t->SetBranchAddress("highPurity", &highPurity_v, &b_highPurity);
        if (t->GetBranch("pfEnergy"))  t->SetBranchAddress("pfEnergy", &pfEnergy, &b_pfEnergy);
        if (t->GetBranch("pfEcal"))  t->SetBranchAddress("pfEcal", &pfEcal_v, &b_pfEcal);
        if (t->GetBranch("pfHcal"))  t->SetBranchAddress("pfHcal", &pfHcal_v, &b_pfHcal);
        if (t->GetBranch("trkAssociatedVtxIndx"))  t->SetBranchAddress("trkAssociatedVtxIndx", &trkAssociatedVtxIndx, &b_trkAssociatedVtxIndx);
        if (t->GetBranch("trkAssociatedVtxQuality"))  t->SetBranchAddress("trkAssociatedVtxQuality", &trkAssociatedVtxQuality, &b_trkAssociatedVtxQuality);
        if (t->GetBranch("trkDzAssociatedVtx"))  t->SetBranchAddress("trkDzAssociatedVtx", &trkDzAssociatedVtx, &b_trkDzAssociatedVtx);
        if (t->GetBranch("trkDzErrAssociatedVtx"))  t->SetBranchAddress("trkDzErrAssociatedVtx", &trkDzErrAssociatedVtx, &b_trkDzErrAssociatedVtx);
        if (t->GetBranch("trkDxyAssociatedVtx"))  t->SetBranchAddress("trkDxyAssociatedVtx", &trkDxyAssociatedVtx, &b_trkDxyAssociatedVtx);
        if (t->GetBranch("trkDxyErrAssociatedVtx"))  t->SetBranchAddress("trkDxyErrAssociatedVtx", &trkDxyErrAssociatedVtx, &b_trkDxyErrAssociatedVtx);
        if (t->GetBranch("trkFirstVtxQuality"))  t->SetBranchAddress("trkFirstVtxQuality", &trkFirstVtxQuality, &b_trkFirstVtxQuality);
        if (t->GetBranch("trkDzFirstVtx"))  t->SetBranchAddress("trkDzFirstVtx", &trkDzFirstVtx, &b_trkDzFirstVtx);
        if (t->GetBranch("trkDzErrFirstVtx"))  t->SetBranchAddress("trkDzErrFirstVtx", &trkDzErrFirstVtx, &b_trkDzErrFirstVtx);
        if (t->GetBranch("trkDxyFirstVtx"))  t->SetBranchAddress("trkDxyFirstVtx", &trkDxyFirstVtx, &b_trkDxyFirstVtx);
        if (t->GetBranch("trkDxyErrFirstVtx"))  t->SetBranchAddress("trkDxyErrFirstVtx", &trkDxyErrFirstVtx, &b_trkDxyErrFirstVtx);
    }
    else {
        if (t->GetBranch("nTrkVtx"))  t->SetBranchAddress("nTrkVtx", &nTrkVtx, &b_nTrkVtx);
        if (t->GetBranch("normChi2Vtx"))  t->SetBranchAddress("normChi2Vtx", &normChi2Vtx, &b_normChi2Vtx);
        if (t->GetBranch("sumPtVtx"))  t->SetBranchAddress("sumPtVtx", &sumPtVtx, &b_sumPtVtx);
        if (t->GetBranch("xVtx"))  t->SetBranchAddress("xVtx", &xVtx, &b_xVtx);
        if (t->GetBranch("yVtx"))  t->SetBranchAddress("yVtx", &yVtx, &b_yVtx);
        if (t->GetBranch("zVtx"))  t->SetBranchAddress("zVtx", &zVtx, &b_zVtx);
        if (t->GetBranch("xVtxErr"))  t->SetBranchAddress("xVtxErr", &xVtxErr, &b_xVtxErr);
        if (t->GetBranch("yVtxErr"))  t->SetBranchAddress("yVtxErr", &yVtxErr, &b_yVtxErr);
        if (t->GetBranch("zVtxErr"))  t->SetBranchAddress("zVtxErr", &zVtxErr, &b_zVtxErr);

        if (t->GetBranch("trkPt"))  t->SetBranchAddress("trkPt", &trkPt, &b_trkPt);
        if (t->GetBranch("trkPtError"))  t->SetBranchAddress("trkPtError", &trkPtError, &b_trkPtError);
        if (t->GetBranch("trkNHit"))  t->SetBranchAddress("trkNHit", &trkNHit, &b_trkNHit);
        if (t->GetBranch("trkNlayer"))  t->SetBranchAddress("trkNlayer", &trkNlayer, &b_trkNlayer);
        if (t->GetBranch("trkEta"))  t->SetBranchAddress("trkEta", &trkEta, &b_trkEta);
        if (t->GetBranch("trkPhi"))  t->SetBranchAddress("trkPhi", &trkPhi, &b_trkPhi);
        if (t->GetBranch("trkCharge"))  t->SetBranchAddress("trkCharge", &trkCharge, &b_trkCharge);
        if (t->GetBranch("trkNVtx"))  t->SetBranchAddress("trkNVtx", &trkNVtx, &b_trkNVtx);
        if (t->GetBranch("trkVtxIndex"))  t->SetBranchAddress("trkVtxIndex", &trkVtxIndex, &b_trkVtxIndex);
        if (t->GetBranch("nTrkTimesnVtx"))  t->SetBranchAddress("nTrkTimesnVtx", &nTrkTimesnVtx, &b_nTrkTimesnVtx);
        if (t->GetBranch("trkAssocVtx"))  t->SetBranchAddress("trkAssocVtx", &trkAssocVtx, &b_trkAssocVtx);
        if (t->GetBranch("trkDxyOverDxyError"))  t->SetBranchAddress("trkDxyOverDxyError", &trkDxyOverDxyError, &b_trkDxyOverDxyError);
        if (t->GetBranch("trkDzOverDzError"))  t->SetBranchAddress("trkDzOverDzError", &trkDzOverDzError, &b_trkDzOverDzError);
        if (t->GetBranch("highPurity"))  t->SetBranchAddress("highPurity", &highPurity, &b_highPurity);
        if (t->GetBranch("tight"))  t->SetBranchAddress("tight", &tight, &b_tight);
        if (t->GetBranch("loose"))  t->SetBranchAddress("loose", &loose, &b_loose);
        if (t->GetBranch("trkChi2"))  t->SetBranchAddress("trkChi2", &trkChi2, &b_trkChi2);
        if (t->GetBranch("trkNdof"))  t->SetBranchAddress("trkNdof", &trkNdof, &b_trkNdof);
        if (t->GetBranch("trkDxy1"))  t->SetBranchAddress("trkDxy1", &trkDxy1, &b_trkDxy1);
        if (t->GetBranch("trkDxyError1"))  t->SetBranchAddress("trkDxyError1", &trkDxyError1, &b_trkDxyError1);
        if (t->GetBranch("trkDz1"))  t->SetBranchAddress("trkDz1", &trkDz1, &b_trkDz1);
        if (t->GetBranch("trkDzError1"))  t->SetBranchAddress("trkDzError1", &trkDzError1, &b_trkDzError1);
        if (t->GetBranch("trkFake"))  t->SetBranchAddress("trkFake", &trkFake, &b_trkFake);
        if (t->GetBranch("trkAlgo"))  t->SetBranchAddress("trkAlgo", &trkAlgo, &b_trkAlgo);
        if (t->GetBranch("trkOriginalAlgo"))  t->SetBranchAddress("trkOriginalAlgo", &trkOriginalAlgo, &b_trkOriginalAlgo);
        if (t->GetBranch("trkMVA"))  t->SetBranchAddress("trkMVA", &trkMVA, &b_trkMVA);
        if (t->GetBranch("trkMVALoose"))  t->SetBranchAddress("trkMVALoose", &trkMVALoose, &b_trkMVALoose);
        if (t->GetBranch("trkMVATight"))  t->SetBranchAddress("trkMVATight", &trkMVATight, &b_trkMVATight);
        if (t->GetBranch("dedx"))  t->SetBranchAddress("dedx", &dedx, &b_dedx);
        if (t->GetBranch("pfType"))  t->SetBranchAddress("pfType", &pfType, &b_pfType);
        if (t->GetBranch("pfCandPt"))  t->SetBranchAddress("pfCandPt", &pfCandPt, &b_pfCandPt);
        if (t->GetBranch("pfEcal"))  t->SetBranchAddress("pfEcal", &pfEcal, &b_pfEcal);
        if (t->GetBranch("pfHcal"))  t->SetBranchAddress("pfHcal", &pfHcal, &b_pfHcal);
    }
}

void Tracks::setupTreeForWriting(TTree *t)
{
    // event
    t->Branch("nEv",&nEv,"nEv/I");
    t->Branch("nLumi",&nLumi,"nLumi/I");
    t->Branch("nBX",&nBX,"nBX/I");
    t->Branch("nRun",&nRun,"nRun/I");
    t->Branch("N",&N,"N/I");

    // vertex
    t->Branch("nVtx",&nVtx,"nVtx/I");
    t->Branch("nTrk",&nTrk,"nTrk/I");
    t->Branch("maxPtVtx",&maxPtVtx,"maxPtVtx/I");
    t->Branch("maxMultVtx",&maxMultVtx,"maxMultVtx/I");
    //  t->Branch("maxVtxHard",&maxVtxHard,"maxVtxHard/I");

    t->Branch("nTrkVtx",nTrkVtx,"nTrkVtx[nVtx]/I");
    t->Branch("normChi2Vtx",normChi2Vtx,"normChi2Vtx[nVtx]/F");
    t->Branch("sumPtVtx",sumPtVtx,"sumPtVtx[nVtx]/F");
    //  t->Branch("nTrkVtxHard",nTrkVtxHard,"nTrkVtxHard[nVtx]/I");

    t->Branch("xVtx",xVtx,"xVtx[nVtx]/F");
    t->Branch("yVtx",yVtx,"yVtx[nVtx]/F");
    t->Branch("zVtx",zVtx,"zVtx[nVtx]/F");
    t->Branch("xVtxErr",xVtxErr,"xVtxErr[nVtx]/F");
    t->Branch("yVtxErr",yVtxErr,"yVtxErr[nVtx]/F");
    t->Branch("zVtxErr",zVtxErr,"zVtxErr[nVtx]/F");

    t->Branch("vtxDist2D",vtxDist2D,"vtxDist2D[nVtx]/F");
    t->Branch("vtxDist2DErr",vtxDist2DErr,"vtxDist2DErr[nVtx]/F");
    t->Branch("vtxDist2DSig",vtxDist2DSig,"vtxDist2DSig[nVtx]/F");
    t->Branch("vtxDist3D",vtxDist3D,"vtxDist3D[nVtx]/F");
    t->Branch("vtxDist3DErr",vtxDist3DErr,"vtxDist3DErr[nVtx]/F");
    t->Branch("vtxDist3DSig",vtxDist3DSig,"vtxDist3DSig[nVtx]/F");

    t->Branch("nVtxSim",&nVtxSim,"nVtxSim/I");
    t->Branch("xVtxSim",xVtxSim,"xVtx[nVtxSim]/F");
    t->Branch("yVtxSim",yVtxSim,"yVtx[nVtxSim]/F");
    t->Branch("zVtxSim",zVtxSim,"zVtx[nVtxSim]/F");

    // Tracks
    t->Branch("trkPt",trkPt,"trkPt[nTrk]/F");
    t->Branch("trkPtError",trkPtError,"trkPtError[nTrk]/F");
    t->Branch("trkNHit",trkNHit,"trkNHit[nTrk]/b");
    t->Branch("trkNlayer",trkNlayer,"trkNlayer[nTrk]/b");
    t->Branch("trkEta",trkEta,"trkEta[nTrk]/F");
    t->Branch("trkPhi",trkPhi,"trkPhi[nTrk]/F");
    t->Branch("trkCharge",trkCharge,"trkCharge[nTrk]/I");
    // if(doTrackVtxWImpPar_)
    {
        t->Branch("trkNVtx",trkNVtx,"trkNVtx[nTrk]/b");
        t->Branch("nTrkTimesnVtx",&nTrkTimesnVtx,"nTrkTimesnVtx/I");
        t->Branch("trkAssocVtx",trkAssocVtx,"trkAssocVtx[nTrkTimesnVtx]/O");
        t->Branch("trkDxyOverDxyError",trkDxyOverDxyError,"trkDxyOverDxyError[nTrkTimesnVtx]/F");
        t->Branch("trkDzOverDzError",trkDzOverDzError,"trkDzOverDzError[nTrkTimesnVtx]/F");
    }
    // else
    {
        t->Branch("trkVtxIndex",trkVtxIndex,"trkVtxIndex[nTrk]/I");
    }

    // if (doDeDx_)
    {
        t->Branch("dedx",dedx,"dedx[nTrk]/F");
    }

    //  t->Branch("trkQual",trkQual,"trkQual[nTrk]/I");

//    for(unsigned int i  = 0; i < qualityStrings_.size(); ++i)
//    {
//        t->Branch(qualityStrings_[i].data(),&trkQual[i],(qualityStrings_[i]+"[nTrk]/O").data());
//    }
    t->Branch("highPurity",highPurity,"highPurity[nTrk]/O");
    t->Branch("tight",tight,"tight[nTrk]/O");
    t->Branch("loose",loose,"loose[nTrk]/O");

    t->Branch("trkChi2",trkChi2,"trkChi2[nTrk]/F");
    t->Branch("trkNdof",trkNdof,"trkNdof[nTrk]/b");
    t->Branch("trkDxy1",trkDxy1,"trkDxy1[nTrk]/F");
    t->Branch("trkDxyError1",trkDxyError1,"trkDxyError1[nTrk]/F");
    t->Branch("trkDz1",trkDz1,"trkDz1[nTrk]/F");
    t->Branch("trkDzError1",trkDzError1,"trkDzError1[nTrk]/F");
    //t->Branch("trkDzError2",trkDzError2,"trkDzError2[nTrk]/F");
    //t->Branch("trkDxy2",trkDxy2,"trkDxy2[nTrk]/F");
    //t->Branch("trkDz2",trkDz2,"trkDz2[nTrk]/F");
    //t->Branch("trkDxyError2",trkDxyError2,"trkDxyError2[nTrk]/F");
    t->Branch("trkFake",trkFake,"trkFake[nTrk]/O");
    t->Branch("trkAlgo",trkAlgo,"trkAlgo[nTrk]/b");
    t->Branch("trkOriginalAlgo",trkOriginalAlgo,"trkOriginalAlgo[nTrk]/b");
    // if(doMVA_)
    {
        t->Branch("trkMVA",trkMVA,"trkMVA[nTrk]/F");
        // if(mvaSrcLabel_.label() == "generalTracks")
        {
            t->Branch("trkMVALoose",trkMVALoose,"trkMVALoose[nTrk]/O");
            t->Branch("trkMVATight",trkMVATight,"trkMVATight[nTrk]/O");
        }
//        // if(mvaSrcLabel_.label() == "hiGeneralTracks")
//        {
//            t->Branch("trkMVATight",trkMVATight,"trkMVATight[nTrk]/O");
//        }
    }

    // if (doPFMatching_)
    {
        t->Branch("pfType",pfType,"pfType[nTrk]/I");
        t->Branch("pfCandPt",pfCandPt,"pfCandPt[nTrk]/F");
        t->Branch("pfEcal",pfEcal,"pfEcal[nTrk]/F");
        t->Branch("pfHcal",pfHcal,"pfHcal[nTrk]/F");
    }

/*
    // if (doDebug_)
    {
        t->Branch("trkNlayer3D",trkNlayer3D,"trkNlayer3D[nTrk]/I");
        t->Branch("trkDxyBS",trkDxyBS,"trkDxyBS[nTrk]/F");
        t->Branch("trkDxyErrorBS",trkDxyErrorBS,"trkDxyErrorBS[nTrk]/F");
        t->Branch("trkDxy",trkDxy,"trkDxy[nTrk]/F");
        t->Branch("trkDz",trkDz,"trkDz[nTrk]/F");
        t->Branch("trkDxyError",trkDxyError,"trkDxyError[nTrk]/F");
        t->Branch("trkDzError",trkDzError,"trkDzError[nTrk]/F");
        t->Branch("trkChi2hit1D",trkChi2hit1D,"trkChi2hit1D[nTrk]/F");
        t->Branch("trkVx",trkVx,"trkVx[nTrk]/F");
        t->Branch("trkVy",trkVy,"trkVy[nTrk]/F");
        t->Branch("trkVz",trkVz,"trkVz[nTrk]/F");
    }

    // Track Extra
    // if (doTrackExtra_)
    {
        t->Branch("trkExpHit1Eta",trkExpHit1Eta,"trkExpHit1Eta[nTrk]/F");
        t->Branch("trkExpHit2Eta",trkExpHit2Eta,"trkExpHit2Eta[nTrk]/F");
        t->Branch("trkExpHit3Eta",trkExpHit3Eta,"trkExpHit3Eta[nTrk]/F");
    }

    // Sim Tracks
    // if (doSimTrack_)
    {
        t->Branch("trkStatus",trkStatus,"trkStatus[nTrk]/F");
        t->Branch("trkPId",trkPId,"trkPId[nTrk]/F");
        t->Branch("trkMPId",trkMPId,"trkMPId[nTrk]/F");
        t->Branch("trkGMPId",trkGMPId,"trkGMPId[nTrk]/F");
        t->Branch("matchedGenID",matchedGenID,"matchedGenID[nTrk][5]/I");

        // if(fillSimTrack_)
        {

            t->Branch("nParticle",&nParticle,"nParticle/I");
            t->Branch("pStatus",pStatus,"pStatus[nParticle]/I");
            t->Branch("pPId",pPId,"pPId[nParticle]/I");
            t->Branch("pEta",pEta,"pEta[nParticle]/F");
            t->Branch("pPhi",pPhi,"pPhi[nParticle]/F");
            t->Branch("pPt",pPt,"pPt[nParticle]/F");
            t->Branch("pAcc",pAcc,"pAcc[nParticle]/F");
            t->Branch("pAccPair",pAccPair,"pAccPair[nParticle]/F");
            t->Branch("pNRec",pNRec,"pNRec[nParticle]/I");
            t->Branch("pNHit",pNHit,"pNHit[nParticle]/I");
            t->Branch("mtrkPt",mtrkPt,"mtrkPt[nParticle]/F");
            t->Branch("mtrkPtError",mtrkPtError,"mtrkPtError[nParticle]/F");
            t->Branch("mtrkNHit",mtrkNHit,"mtrkNHit[nParticle]/I");
            t->Branch("mtrkNlayer",mtrkNlayer,"mtrkNlayer[nParticle]/I");
            t->Branch("mtrkNlayer3D",mtrkNlayer3D,"mtrkNlayer3D[nParticle]/I");
            // t->Branch("mtrkQual",mtrkQual,"mtrkQual[nParticle]/I");
//            for(unsigned int i  = 0; i < qualityStrings_.size(); ++i)
//            {
//                t->Branch(("m"+qualityStrings_[i]).data(),&mtrkQual[i],("m"+qualityStrings_[i]+"[nParticle]/O").data());
//            }
            t->Branch("mtrkChi2",mtrkChi2,"mtrkChi2[nParticle]/F");
            t->Branch("mtrkNdof",mtrkNdof,"mtrkNdof[nParticle]/I");
            t->Branch("mtrkDz1",mtrkDz1,"mtrkDz1[nParticle]/F");
            t->Branch("mtrkDzError1",mtrkDzError1,"mtrkDzError1[nParticle]/F");
            t->Branch("mtrkDxy1",mtrkDxy1,"mtrkDxy1[nParticle]/F");
            t->Branch("mtrkDxyError1",mtrkDxyError1,"mtrkDxyError1[nParticle]/F");
            //t->Branch("mtrkDz2",mtrkDz2,"mtrkDz2[nParticle]/F");
            //t->Branch("mtrkDzError2",mtrkDzError2,"mtrkDzError2[nParticle]/F");
            //t->Branch("mtrkDxy2",mtrkDxy2,"mtrkDxy2[nParticle]/F");
            //t->Branch("mtrkDxyError2",mtrkDxyError2,"mtrkDxyError2[nParticle]/F");
            t->Branch("mtrkAlgo",mtrkAlgo,"mtrkAlgo[nParticle]/I");
            t->Branch("mtrkOriginalAlgo",mtrkOriginalAlgo,"mtrkOriginalAlgo[nParticle]/I");
            // if(doTrackVtxWImpPar_)
            {
                t->Branch("nParticleTimesnVtx",&nParticleTimesnVtx,"nParticleTimesnVtx/I");
                t->Branch("mtrkDzOverDzError",mtrkDzOverDzError,"mtrkDzOverDzError[nParticleTimesnVtx]/F");
                t->Branch("mtrkDxyOverDxyError",mtrkDxyOverDxyError,"mtrkDxyOverDxyError[nParticleTimesnVtx]/F");
            }
            // if(doMVA_)
            {
                t->Branch("mtrkMVA",mtrkMVA,"mtrkMVA[nParticle]/F");

                // if(mvaSrcLabel_.label() == "generalTracks")
                {
                    t->Branch("mtrkMVALoose",mtrkMVALoose,"mtrkMVALoose[nTrk]/O");
                    t->Branch("mtrkMVATight",mtrkMVATight,"mtrkMVATight[nTrk]/O");
                }
                // if(mvaSrcLabel_.label() == "hiGeneralTracks")
                {
                    t->Branch("mtrkMVATight",mtrkMVATight,"mtrkMVATight[nTrk]/O");
                }
            }
            // if (doPFMatching_)
            {
                t->Branch("mtrkPfType",mtrkPfType,"mtrkPfType[nParticle]/I");
                t->Branch("mtrkPfCandPt",mtrkPfCandPt,"mtrkPfCandPt[nParticle]/F");
                t->Branch("mtrkPfEcal",mtrkPfEcal,"mtrkPfEcal[nParticle]/F");
                t->Branch("mtrkPfHcal",mtrkPfHcal,"mtrkPfHcal[nParticle]/F");
            }
        }
    }
*/

}

#endif
