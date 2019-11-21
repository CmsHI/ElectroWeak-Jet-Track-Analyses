/*
 * utilities for v+jet+track analysis
 */
#ifndef VJETRKUTIL_H_
#define VJETRKUTIL_H_

#include <TFile.h>
#include <TTree.h>
#include <TObjArray.h>
#include <TMath.h>
#include <TLorentzVector.h>
#include <TRandom3.h>

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>

#include "../TreeHeaders/ggHiNtuplizerTree.h"
#include "../TreeHeaders/trackTree.h"
#include "../TreeHeaders/pfCandTree.h"
#include "../TreeHeaders/trackSkimTree.h"
#include "leptons/muons/sysUtil.h"
#include "leptons/electrons/sysUtil.h"
#include "physicsUtil.h"
#include "systemUtil.h"

namespace VJT {

enum EWOBJS {
    kPhoton,
    kZBoson,
    kN_EWOBJS
};
const std::string ewObjsStr[kN_EWOBJS] = {"photon", "zBoson"};
int ewObj;

// variables for mix event files
const int nCentBins = 800;  // event activity can be centrality, multiplicity, transverse energy, energy density, etc.
const int nVzBins = 15;
const int nEventPlaneBins = 1;

const double etaMin_pf_HF = 3;
const double etaMax_pf_HF = 5;

const double PF_HF_totE_max = 150000;

const double PF_HF_E_eta4to5_max = 85000;
const double PF_HF_E_eta3to4_max = 60000;

const int hiNpix_max = 100000;

const double etaMin_pf_HE = 2.5;
const double etaMax_pf_HE = 2.9;

const double PF_HE_totE_max = 12000;

const int nTrk_max = 8000;

enum MIXMETHODS {
    k_match_hiBin,
    k_match_Npart,
    k_match_hiHF,
    k_match_rho,
    k_match_PF_HF_totE,
    k_match_PF_HF_E_eta4to5,
    k_match_PF_HF_E_eta3to4,
    k_match_hiNpix,
    k_match_PF_HE_totE,
    k_match_nTrk,
    k_match_nVtx,
    kN_MITMETHODS
};
const std::string mixMerhodsStr[kN_MITMETHODS] = {"match_hiBin", "match_Npart", "match_hiHF", "match_rho",
                                                  "match_PF_HF_totE", "match_PF_HF_E_eta4to5", "match_PF_HF_E_eta3to4",
                                                  "match_hiNpix", "match_PF_HE_totE", "match_nTrk",
                                                  "match_nVtx"};
int mixMethod;

enum RECOGEN {
    kReco,
    kGen,
    kN_RECOGEN
};

enum RBS {
    kRaw,
    kBkg,
    kSig,
    kN_RBS
};

}

void setBranchStatusTreeHiEvt(TTree* t, bool isMC = false);
void setBranchStatusTreeJet(TTree* t, bool isMC = false);
void setBranchStatusTreeTrack(TTree* t, bool enableVtx = false);
void setBranchStatusTreePFCand(TTree* t);
void setBranchStatusTreeSkimAna(TTree* t);
void setBranchStatusTreeHiGenParticle(TTree* t);
int getIndex4CentBin(int hiBin);
int getHiBin(int hiBin);
int getVzBin(float vz);
int getEventPlaneBin(double eventPlaneAngle);
int getNpartBin(float Npart);
int getNTrkBin(int nTrk, int nTrkMax = 14280);
int getHiHFBin(float hiHF, float hiHFMax = 6000);
int getHiHFhitBin(float hiHFhit, float hiHFhitMax = 180000);
int getHiNpixBin(int hiNpix, int hiNpixMax = 100000);
int getRhoBin(double rho, double rhoMax = 250);
int getPFEnergyBin(double PF_E, double PF_E_max);
int getEventEnergyBin(double evtEnergy, double evtEnergyMax);
int getNVtxBin(int nVtx);
double getRhoDiff(double rho);
int parseEWObj(std::string ewObjStr);
int parseMixMethod(std::string mixMethodStr);
bool isMCsample(std::string sampleType);
bool isPbPbsample(std::string sampleType);
bool isPPsample(std::string sampleType);
bool isRecoObj(std::string objRG);
bool isGenObj(std::string objRG);
bool isRawObj(std::string objRBS);
bool isBkgObj(std::string objRBS);
bool isSigObj(std::string objRBS);
bool passedMuSelection(ggHiNtuplizer& ggHi, int i);
bool passedTrkSelection(trackSkim& trks, int i, int collType);
bool passedTrkSelection(Tracks& trks, int i, int collType);
int getNTrkPerp(Tracks& trks, double vPhi);
int getTrkMult(trackSkim& trks, double ptMin, double etaMax);
int getTrkMult(Tracks& trks, double ptMin, double etaMax);
bool passedPerpTrkSelection(Tracks& trks, int i, int collType, double vPhi);
int getTrkMultPerp(Tracks& trks, int collType, double vPhi);
float getPFtotE(pfCand& pf, int pfId = 0, float etaMin = 3, float etaMax = 5);
double getVRecoEffCorrection(double vPt, double vY, TH2D* h2_eff);
// trk corrections
std::vector<double> getVecPt4TrkW();
std::vector<double> getVecPt4TrkWCoarse();
std::vector<double> getVecPt4TrkWCoarse2();
std::vector<double> getVecEta4TrkW(int nBins = 12);
std::vector<double> getVecAbsEta4TrkWCoarse();
std::vector<int> getVecCent4TrkW(int nBins = 20);
std::vector<int> getVecCent4TrkWCoarse();
std::vector<int> getVecCent4TrkWCoarse2();
int getBinPt4TrkW(double pt, std::vector<double>& trkPts, int nBins);
int getBinEta4TrkW(double eta, std::vector<double>& trkEtas, int nBins);
int getBinCent4TrkW(int hiBin, std::vector<int>& hiBins, int nBins);
int getBinCent4TrkWCoarse(int hiBin, std::vector<int>& hiBins, int nBins);
double getTrkPhiEffCorrection(double trkPhi, TH1D* h_effcorr);
double getTrkEtaPhiEffCorrection(double trkEta, double trkPhi, TH2D* h_effcorr);
// trigger
std::vector<std::string> getTreeNamesHLTObj(int collType);
std::vector<std::string> getTreeNamesHLTObjpp13TeV();
// leptons
std::vector<int> indicesNearPhotons(ggHiNtuplizer& ggHi, int iEle, double dRmax);
// lepton systematics
double getSFweight(bool isMu, double pt, double eta, int cent);
int parseLepSysIndex(std::string sys_label);
double getLepSysVar(bool isMu, int sys_index, double pt, double eta, int cent);
// user interaction
std::vector<double> parseRanges(std::string rangesStr, int indexMinMax);
std::vector<double> parseRangesMin(std::string rangesStr);
std::vector<double> parseRangesMax(std::string rangesStr);
// histogram util
double parseVPtMin(std::string histPath);
double parseVPtMax(std::string histPath);
double parseTrkPtMin(std::string histPath);
double parseTrkPtMax(std::string histPath);
int parseCentMin(std::string histPath);
int parseCentMax(std::string histPath);
double parseVTrkDPhiMin(std::string text);
std::string parseVTrkObs(std::string label);
bool isDphi(std::string label);
bool isXivh(std::string label);
bool isTrkPt(std::string label);
bool isRatioDphi(std::string label);
bool isRatioXivh(std::string label);
bool isRatioTrkPt(std::string label);
bool isDiffDphi(std::string label);
bool isRatioVTrk(std::string label);

void setBranchStatusTreeHiEvt(TTree* t, bool isMC)
{
    // specify explicitly which branches to use, do not use wildcard
    t->SetBranchStatus("*",0);     // disable all branches
    t->SetBranchStatus("run",1);   // enable event information
    t->SetBranchStatus("evt",1);
    t->SetBranchStatus("lumi",1);
    t->SetBranchStatus("vz",1);
    t->SetBranchStatus("hiBin",1);
    t->SetBranchStatus("hiEvtPlanes",1);
    t->SetBranchStatus("hiHF",1);
    t->SetBranchStatus("hiHFhit",1);
    t->SetBranchStatus("hiNpix*",1);
    t->SetBranchStatus("npus",1);
    t->SetBranchStatus("tnpus",1);
    if (isMC) {
        t->SetBranchStatus("Npart",1);
        t->SetBranchStatus("Ncoll",1);
        t->SetBranchStatus("Nhard",1);
        t->SetBranchStatus("phi0",1);
    }
}

void setBranchStatusTreeJet(TTree* t, bool isMC)
{
    t->SetBranchStatus("*",0);     // disable all branches
    t->SetBranchStatus("nref",1);
    t->SetBranchStatus("jtpt",1);
    t->SetBranchStatus("jteta",1);
    t->SetBranchStatus("jtphi",1);
    t->SetBranchStatus("rawpt",1);

    t->SetBranchStatus("chargedMax",1);
    t->SetBranchStatus("chargedSum",1);
    t->SetBranchStatus("chargedN",1);
    t->SetBranchStatus("chargedHardSum",1);
    t->SetBranchStatus("chargedHardN",1);

    if (isMC) {
        t->SetBranchStatus("subid",1);
        t->SetBranchStatus("refpt",1);
        t->SetBranchStatus("refeta",1);
        t->SetBranchStatus("refphi",1);
        t->SetBranchStatus("refparton_flavor",1);
        t->SetBranchStatus("ngen",1);
        t->SetBranchStatus("genpt",1);
        t->SetBranchStatus("geneta",1);
        t->SetBranchStatus("genphi",1);
        t->SetBranchStatus("gensubid",1);
    }
}

void setBranchStatusTreeTrack(TTree* t, bool enableVtx)
{
    // specify explicitly which branches to use, do not use wildcard
    t->SetBranchStatus("*",0);     // disable all branches
    t->SetBranchStatus("nTrk",1);
    t->SetBranchStatus("trkPt",1);
    t->SetBranchStatus("trkEta",1);
    t->SetBranchStatus("trkPhi",1);
    t->SetBranchStatus("trkCharge",1);
    t->SetBranchStatus("trkPtError",1);
    t->SetBranchStatus("trkDz1",1);
    t->SetBranchStatus("trkDzError1",1);
    t->SetBranchStatus("trkDxy1",1);
    t->SetBranchStatus("trkDxyError1",1);
    t->SetBranchStatus("trkChi2",1);
    t->SetBranchStatus("trkNdof",1);
    t->SetBranchStatus("trkNlayer",1);
    t->SetBranchStatus("trkNHit",1);
    t->SetBranchStatus("trkAlgo",1);
    t->SetBranchStatus("trkMVA",1);
    t->SetBranchStatus("highPurity",1);
    //t->SetBranchStatus("tight",1);
    //t->SetBranchStatus("loose",1);
    t->SetBranchStatus("pfType",1);
    t->SetBranchStatus("pfCandPt",1);
    t->SetBranchStatus("pfEcal",1);
    t->SetBranchStatus("pfHcal",1);

    if (enableVtx) {
        // vertex related
        t->SetBranchStatus("nVtx",1);
        t->SetBranchStatus("maxPtVtx",1);
        t->SetBranchStatus("maxMultVtx",1);
        t->SetBranchStatus("trkNVtx",1);
        t->SetBranchStatus("nTrkTimesnVtx",1);
        t->SetBranchStatus("trkAssocVtx",1);
    }
}

void setBranchStatusTreePFCand(TTree* t)
{
    t->SetBranchStatus("*",0);     // disable all branches
    t->SetBranchStatus("nPFpart",1);
    t->SetBranchStatus("pfId",1);
    t->SetBranchStatus("pfEnergy",1);
    t->SetBranchStatus("pfEta",1);
    //t->SetBranchStatus("pfEcalE",1);
    //t->SetBranchStatus("pfHcalE",1);
}

void setBranchStatusTreeSkimAna(TTree* t)
{
    t->SetBranchStatus("*",1);
}

void setBranchStatusTreeHiGenParticle(TTree* t)
{
    t->SetBranchStatus("*",0);     // disable all branches
    t->SetBranchStatus("mult",1);
    t->SetBranchStatus("pt",1);
    t->SetBranchStatus("eta",1);
    t->SetBranchStatus("phi",1);
    t->SetBranchStatus("pdg",1);
    t->SetBranchStatus("chg",1);
    t->SetBranchStatus("sube",1);
}

int getIndex4CentBin(int hiBin)
{
    if (hiBin < 20) {
        return 0;
    }
    else if (hiBin < 60) {
        return 1;
    }
    else if (hiBin < 100) {
        return 2;
    }
    else {
        return 3;
    }
}

int getHiBin(int hiBin)
{
    int deltaHiBin = 204 / VJT::nCentBins;
    return (int)(hiBin / deltaHiBin);
}

int getVzBin(float vz)
{
    if (!(std::fabs(vz) < 15)) return -1;

    double deltavz = 30 / VJT::nVzBins;
    for (int i = 0; i < VJT::nVzBins; ++i) {
        if ( (-15.0 + i*deltavz) < vz && vz <= (-15.0 + (i+1)*deltavz) ) {
            return i;
        }
    }
    return -1;
}

int getEventPlaneBin(double eventPlaneAngle)
{
    if (!(std::fabs(eventPlaneAngle) < TMath::PiOver2())) return -1;

    double deltaAngle = TMath::Pi() / VJT::nEventPlaneBins;
    for (int i = 0; i < VJT::nEventPlaneBins; ++i){
        if ( (-1.0*TMath::PiOver2() + i*deltaAngle) < eventPlaneAngle &&
                                    eventPlaneAngle <= (-1.0*TMath::PiOver2()+ (i+1)*deltaAngle) ) {
            return i;
        }
    }

    return -1;
}

int getNpartBin(float Npart)
{
    int deltaNpart = (int)(408 / VJT::nCentBins);
    return (int)(Npart / deltaNpart);
}

int getNTrkBin(int nTrk, int nTrkMax)
{
    int deltaNtrk = (int)(nTrkMax / VJT::nCentBins);
    if (nTrk < 0) nTrk = 0;
    int binTmp = (int)(nTrk / deltaNtrk);
    if (binTmp < VJT::nCentBins) {
        return binTmp;
    }
    else {
        return (VJT::nCentBins)-1;
    }
}

int getHiHFBin(float hiHF, float hiHFMax)
{
    return getEventEnergyBin(hiHF, hiHFMax);
}

int getHiHFhitBin(float hiHFhit, float hiHFhitMax)
{
    return getEventEnergyBin(hiHFhit, hiHFhitMax);
}

int getHiNpixBin(int hiNpix, int hiNpixMax)
{
    int deltaHiNpix = (int)(hiNpixMax / VJT::nCentBins);
    if (hiNpix < 0) hiNpix = 0;
    int binTmp = (int)(hiNpix / deltaHiNpix);
    if (binTmp < VJT::nCentBins) {
        return binTmp;
    }
    else {
        return (VJT::nCentBins)-1;
    }
}

int getRhoBin(double rho, double rhoMax)
{
    return getEventEnergyBin(rho, rhoMax);
}

int getPFEnergyBin(double PF_E, double PF_E_max)
{
    return getEventEnergyBin(PF_E, PF_E_max);
}

int getEventEnergyBin(double evtEnergy, double evtEnergyMax)
{
    if (evtEnergy <= 0) return 0;

    double deltaEnergy = (double)(evtEnergyMax / VJT::nCentBins);
    int binTmp = (int)(evtEnergy / deltaEnergy);
    if (binTmp < VJT::nCentBins) {
        return binTmp;
    }
    else {
        return (VJT::nCentBins)-1;
    }
}

int getNVtxBin(int nVtx)
{
    if (nVtx <= 0) return -1;

    if (nVtx < VJT::nCentBins) {
        return nVtx - 1;
    }
    else {
        return (VJT::nCentBins)-1;
    }
}

double getRhoDiff(double rho)
{
    double rho1 = 0.696165;
    double rho2 = 212.53;

    // (1.91953-1.05409) = 0.86544
    double a = 0.86544 / (rho2 - rho1);

    return (a * (rho - rho1) + 1.05409);
}

int parseEWObj(std::string ewObjStr)
{
    ewObjStr = trim(ewObjStr);
    ewObjStr = toLowerCase(ewObjStr);

    if (ewObjStr == "pho" || ewObjStr == "photon" || ewObjStr == "0") {
        return VJT::EWOBJS::kPhoton;
    }
    else if (ewObjStr == "zboson" || ewObjStr == "1") {
        return VJT::EWOBJS::kZBoson;
    }
    else {
        return -1;
    }
}

int parseMixMethod(std::string mixMethodStr)
{
    mixMethodStr = trim(mixMethodStr);
    mixMethodStr = toLowerCase(mixMethodStr);

    if (mixMethodStr == "match_hibin" || mixMethodStr == "0") {
        return VJT::MIXMETHODS::k_match_hiBin;
    }
    else if (mixMethodStr == "match_npart" || mixMethodStr == "1") {
        return VJT::MIXMETHODS::k_match_Npart;
    }
    else if (mixMethodStr == "match_hihf" || mixMethodStr == "2") {
        return VJT::MIXMETHODS::k_match_hiHF;
    }
    else if (mixMethodStr == "match_rho" || mixMethodStr == "3") {
        return VJT::MIXMETHODS::k_match_rho;
    }
    else if (mixMethodStr == "match_pf_hf_tote" || mixMethodStr == "4") {
        return VJT::MIXMETHODS::k_match_PF_HF_totE;
    }
    else if (mixMethodStr == "match_pf_hf_e_eta4to5" || mixMethodStr == "5") {
        return VJT::MIXMETHODS::k_match_PF_HF_E_eta4to5;
    }
    else if (mixMethodStr == "match_pf_hf_e_eta3to4" || mixMethodStr == "6") {
        return VJT::MIXMETHODS::k_match_PF_HF_E_eta3to4;
    }
    else if (mixMethodStr == "match_hinpix" || mixMethodStr == "7") {
        return VJT::MIXMETHODS::k_match_hiNpix;
    }
    else if (mixMethodStr == "match_pf_he_tote" || mixMethodStr == "8") {
        return VJT::MIXMETHODS::k_match_PF_HE_totE;
    }
    else if (mixMethodStr == "match_ntrk" || mixMethodStr == "9") {
        return VJT::MIXMETHODS::k_match_nTrk;
    }
    else if (mixMethodStr == "match_nvtx" || mixMethodStr == "10") {
        return VJT::MIXMETHODS::k_match_nVtx;
    }
    else {
        return -1;
    }
}

bool isMCsample(std::string sampleType)
{
    return (toLowerCase(sampleType).find("_mc") != std::string::npos);
}

bool isPbPbsample(std::string sampleType)
{
    return (toLowerCase(sampleType).find("pbpb") != std::string::npos);
}

bool isPPsample(std::string sampleType)
{
    return (toLowerCase(sampleType).find("pp") != std::string::npos);
}

/*
 * RG = reco/gen
 */
bool isRecoObj(std::string objRG)
{
    return (toLowerCase(objRG) == "r" || toLowerCase(objRG) == "reco");
}

/*
 * RG = reco/gen
 */
bool isGenObj(std::string objRG)
{
    return (toLowerCase(objRG) == "g" || toLowerCase(objRG) == "gen");
}

/*
 * RBS = raw/background/signal
 */
bool isRawObj(std::string objRBS)
{
    return (toLowerCase(objRBS) == "r" || toLowerCase(objRBS) == "raw");
}

/*
 * RBS = raw/background/signal
 */
bool isBkgObj(std::string objRBS)
{
    return (toLowerCase(objRBS) == "b" || toLowerCase(objRBS) == "bkg");
}

/*
 * RBS = raw/background/signal
 */
bool isSigObj(std::string objRBS)
{
    return (toLowerCase(objRBS) == "s" || toLowerCase(objRBS) == "sig");
}

bool passedMuSelection(ggHiNtuplizer& ggHi, int i)
{
    // tight muon ID from 2015 PbPb
    if (std::fabs((*ggHi.muEta)[i]) < 2.4)
    {
        if (!((*ggHi.muChi2NDF)[i] < 10)) return false;
        if (!(std::fabs((*ggHi.muInnerD0)[i]) < 0.2)) return false;
        if (!(std::fabs((*ggHi.muInnerDz)[i]) < 0.5)) return false;
        if (!((*ggHi.muMuonHits)[i] > 0)) return false;
        if (!((*ggHi.muStations)[i] > 1)) return false;
        if (!((*ggHi.muTrkLayers)[i] > 5)) return false;
        if (!((*ggHi.muPixelHits)[i] > 0)) return false;
    }
    else {
        return false;
    }

    return true;
}

bool passedTrkSelection(trackSkim& trks, int i, int collType)
{
    if (collisionIsHI2018((COLL::TYPE)collType)) {
        // cuts for 2018
        if (((*trks.p_trkAlgo)[i] == 6 && (*trks.p_trkMVA)[i] < 0.98)) return false;

        if (!((*trks.p_highPurity)[i] == 1)) return false;
        if (!((*trks.p_trkPtError)[i] / (*trks.p_trkPt)[i] < 0.1 &&
              std::fabs((*trks.p_trkDz1)[i] / (*trks.p_trkDzError1)[i]) < 3 &&
              std::fabs((*trks.p_trkDxy1)[i] / (*trks.p_trkDxyError1)[i]) < 3))  return false;
        if (!((*trks.p_trkNHit)[i] >= 11))  return false;
        if (!((*trks.p_trkChi2)[i] / (float)(*trks.p_trkNdof)[i] / (float)(*trks.p_trkNlayer)[i] < 0.18))  return false;

        if ((*trks.p_trkPt)[i] > 20) {
            float Et = ((*trks.p_pfHcal)[i] + (*trks.p_pfEcal)[i]) / TMath::CosH((*trks.p_trkEta)[i]);
            if ( !(Et > 0.5 * (*trks.p_trkPt)[i]) )  return false;
        }
    }
    else if (collisionIsHI((COLL::TYPE)collType)) {
        // cuts for 2015
        if (!((*trks.p_trkPt)[i] <= 300 && std::fabs((*trks.p_trkEta)[i]) < 2.4)) return false;
        if (!((*trks.p_highPurity)[i] == 1)) return false;
        if (!((*trks.p_trkPtError)[i] / (*trks.p_trkPt)[i] < 0.1 &&
              std::fabs((*trks.p_trkDz1)[i] / (*trks.p_trkDzError1)[i]) < 3 &&
              std::fabs((*trks.p_trkDxy1)[i] / (*trks.p_trkDxyError1)[i]) < 3))  return false;
        if (!((*trks.p_trkChi2)[i] / (float)(*trks.p_trkNdof)[i] / (float)(*trks.p_trkNlayer)[i] < 0.15))  return false;
        if (!((*trks.p_trkNHit)[i] >= 11))  return false;

        if ((*trks.p_trkPt)[i] > 20) {
            float Et = ((*trks.p_pfHcal)[i] + (*trks.p_pfEcal)[i]) / TMath::CosH((*trks.p_trkEta)[i]);
            if ( !(Et > 0.5 * (*trks.p_trkPt)[i]) )  return false;
        }
    }
    else if (collisionIsPP2017((COLL::TYPE)collType)) {
        // cuts for pp 2017
        if (!((*trks.p_highPurity)[i] == 1)) return false;
        if (!((*trks.p_trkPtError)[i] / (*trks.p_trkPt)[i] < 0.1 &&
              std::fabs((*trks.p_trkDz1)[i] / (*trks.p_trkDzError1)[i]) < 3 &&
              std::fabs((*trks.p_trkDxy1)[i] / (*trks.p_trkDxyError1)[i]) < 3))  return false;
    }
    else if (collisionIsPP((COLL::TYPE)collType)) {
        // cuts for pp 2015
        if (!((*trks.p_trkPt)[i] <= 300 && std::fabs((*trks.p_trkEta)[i]) < 2.4)) return false;
        if (!((*trks.p_highPurity)[i] == 1)) return false;
        if (!((*trks.p_trkPtError)[i] / (*trks.p_trkPt)[i] < 0.1 &&
              std::fabs((*trks.p_trkDz1)[i] / (*trks.p_trkDzError1)[i]) < 3 &&
              std::fabs((*trks.p_trkDxy1)[i] / (*trks.p_trkDxyError1)[i]) < 3))  return false;

        if ((*trks.p_trkPt)[i] > 20) {
            float Et = ((*trks.p_pfHcal)[i] + (*trks.p_pfEcal)[i]) / TMath::CosH((*trks.p_trkEta)[i]);
            if ( !(Et > 0.5 * (*trks.p_trkPt)[i]) )  return false;
        }
    }
    else {
        return false;
    }

    return true;
}

bool passedTrkSelection(Tracks& trks, int i, int collType)
{
    if (collisionIsHI2018((COLL::TYPE)collType)) {
        // cuts for 2018
        if ((trks.trkAlgo[i] == 6 && trks.trkMVA[i] < 0.98)) return false;

        if (!(trks.highPurity[i] == 1)) return false;
        if (!(trks.trkPtError[i] / trks.trkPt[i] < 0.1 &&
              std::fabs(trks.trkDz1[i] / trks.trkDzError1[i]) < 3 &&
              std::fabs(trks.trkDxy1[i] / trks.trkDxyError1[i]) < 3))  return false;
        if (!(trks.trkNHit[i] >= 11))  return false;
        if (!(trks.trkChi2[i] / (float)trks.trkNdof[i] / (float)trks.trkNlayer[i] < 0.18))  return false;

        if (trks.trkPt[i] > 20) {
            float Et = (trks.pfHcal[i] + trks.pfEcal[i]) / TMath::CosH(trks.trkEta[i]);
            if ( !(Et > 0.5 * trks.trkPt[i]) )  return false;
        }
    }
    else if (collisionIsHI((COLL::TYPE)collType)) {
        // cuts for 2015
        if (!(trks.trkPt[i] <= 300 && std::fabs(trks.trkEta[i]) < 2.4)) return false;
        if (!(trks.highPurity[i] == 1)) return false;
        if (!(trks.trkPtError[i] / trks.trkPt[i] < 0.1 &&
              std::fabs(trks.trkDz1[i] / trks.trkDzError1[i]) < 3 &&
              std::fabs(trks.trkDxy1[i] / trks.trkDxyError1[i]) < 3))  return false;
        if (!(trks.trkChi2[i] / (float)trks.trkNdof[i] / (float)trks.trkNlayer[i] < 0.15))  return false;
        if (!(trks.trkNHit[i] >= 11))  return false;

        if (trks.trkPt[i] > 20) {
            float Et = (trks.pfHcal[i] + trks.pfEcal[i]) / TMath::CosH(trks.trkEta[i]);
            if ( !(Et > 0.5 * trks.trkPt[i]) )  return false;
        }
    }
    else if (collisionIsPP2017((COLL::TYPE)collType)) {
        // cuts for pp 2017
        if (!(trks.highPurity[i] == 1)) return false;
        if (!(trks.trkPtError[i] / trks.trkPt[i] < 0.1 &&
              std::fabs(trks.trkDz1[i] / trks.trkDzError1[i]) < 3 &&
              std::fabs(trks.trkDxy1[i] / trks.trkDxyError1[i]) < 3))  return false;
    }
    else if (collisionIsPP((COLL::TYPE)collType)) {
        // cuts for pp 2015
        if (!(trks.trkPt[i] <= 300 && std::fabs(trks.trkEta[i]) < 2.4)) return false;
        if (!(trks.highPurity[i] == 1)) return false;
        if (!(trks.trkPtError[i] / trks.trkPt[i] < 0.1 &&
              std::fabs(trks.trkDz1[i] / trks.trkDzError1[i]) < 3 &&
              std::fabs(trks.trkDxy1[i] / trks.trkDxyError1[i]) < 3))  return false;

        if (trks.trkPt[i] > 20) {
            float Et = (trks.pfHcal[i] + trks.pfEcal[i]) / TMath::CosH(trks.trkEta[i]);
            if ( !(Et > 0.5 * trks.trkPt[i]) )  return false;
        }
    }
    else {
        return false;
    }

    return true;
}

int getNTrkPerp(Tracks& trks, double vPhi)
{
    int res = 0;
    for (int i = 0; i < trks.nTrk; ++i) {

        double dphi_trkV_abs = std::fabs(getDPHI(vPhi, trks.trkPhi[i]));

        if ( !(TMath::PiOver4() < dphi_trkV_abs && dphi_trkV_abs < TMath::PiOver4()*3.0) ) continue;

        res++;
    }

    return res;
}

int getTrkMult(trackSkim& trks, double ptMin, double etaMax)
{
    int res = 0;
    for (int i = 0; i < trks.nTrk; ++i) {

        if ( !((*trks.p_trkPt)[i] > ptMin) )  continue;
        if ( !(std::fabs((*trks.p_trkEta)[i]) < etaMax) )  continue;

        res++;
    }

    return res;
}

int getTrkMult(Tracks& trks, double ptMin, double etaMax)
{
    int res = 0;
    for (int i = 0; i < trks.nTrk; ++i) {

        if ( !(trks.trkPt[i] > ptMin) )  continue;
        if ( !(std::fabs(trks.trkEta[i]) < etaMax) )  continue;

        res++;
    }

    return res;
}

bool passedPerpTrkSelection(Tracks& trks, int i, int collType, double vPhi)
{
    if (!passedTrkSelection(trks, i, collType))  return false;
    if (!(trks.trkPt[i] > 0.5))  return false;
    if (!(std::fabs(trks.trkEta[i]) < 2.4))  return false;

    double dphi_trkV_abs = std::fabs(getDPHI(vPhi, trks.trkPhi[i]));

    if ( !(TMath::PiOver4() < dphi_trkV_abs && dphi_trkV_abs < TMath::PiOver4()*3.0) ) return false;

    return true;
}

int getTrkMultPerp(Tracks& trks, int collType, double vPhi)
{
    int res = 0;
    for (int i = 0; i < trks.nTrk; ++i) {

        if (!passedPerpTrkSelection(trks, i, collType, vPhi))  continue;

        res++;
    }

    return res;
}

float getPFtotE(pfCand& pf, int pfId, float etaMin, float etaMax)
{
    float res = 0;

    for (int i = 0; i < pf.nPFpart; ++i) {

        if ( !(etaMin <= std::fabs((*pf.pfEta)[i])) )  continue;
        if ( !(etaMax > std::fabs((*pf.pfEta)[i])) )  continue;

        if ((*pf.pfId)[i] == pfId) {
            res += (*pf.pfEnergy)[i];
        }
    }

    return res;
}

double getVRecoEffCorrection(double vPt, double vY, TH2D* h2_eff)
{
    int nBinsX = h2_eff->GetXaxis()->GetNbins();
    int nBinsY = h2_eff->GetYaxis()->GetNbins();

    int binX = h2_eff->GetXaxis()->FindBin(vY);
    int binY = h2_eff->GetYaxis()->FindBin(vPt);

    if (binX == 0 || binX > nBinsX || binY == 0) {
        return 0;
    }

    if (binY > nBinsY) {
        binY = nBinsY;
    }

    double effTmp = h2_eff->GetBinContent(binX, binY);
    if (effTmp < 0.001) {
        binX = h2_eff->GetXaxis()->FindBin(-1*vY);
        effTmp = h2_eff->GetBinContent(binX, binY);
        if (effTmp < 0.001) {
            std::cout << "WARNING : very low or no efficiency for vPt = " << vPt << " and vY = " << vY << std::endl;
            std::cout << "Returning a correction of 0." << std::endl;
            return 0;
        }
    }
    if (effTmp > 1.0) {
        effTmp = 1.0;
    }

    return 1./(effTmp);
}

std::vector<double> getVecPt4TrkW()
{
    std::vector<double> trkPts = { 0.20, 0.250, 0.30, 0.350, 0.40, 0.450, 0.50, 0.550, 0.60, 0.650, 0.70, 0.750, 0.80, 0.850, 0.90, 0.950, 1.0, 1.05, 1.10, 1.15, 1.20, 1.30, 1.40, 1.50, 1.60, 1.70, 1.80, 1.90, 2.0, 2.50, 3.0, 4.0, 5.0, 7.50, 10.0, 12.0, 15.0, 20.0, 25.0, 30.0, 45.0, 60.0, 90.0, 120.0, 180.0, 300.0 };
    return trkPts;
}

std::vector<double> getVecPt4TrkWCoarse()
{
    //std::vector<double> trkPts = { 0.50, 0.60, 0.70, 0.80, 0.90, 1.0, 1.20, 1.50, 2.0, 3.0, 4.0, 5.0, 8.0, 10.0, 12.0, 20.0, 60.0, 120.0, 300.0 };
    //std::vector<double> trkPts = { 0.50, 0.60, 0.70, 0.80, 0.90, 1.0, 1.20, 1.40, 1.60, 1.80, 2.0, 2.5, 3.0, 4.0, 5.0, 8.0, 10.0, 12.0, 20.0, 60.0, 120.0, 300.0 };
    std::vector<double> trkPts = { 0.50, 0.60, 0.70, 0.80, 0.90, 1.0, 1.10, 1.20, 1.30, 1.40, 1.60, 1.80, 2.0, 2.5, 3.0, 4.0, 5.0, 8.0, 10.0, 12.0, 20.0, 60.0, 120.0, 300.0 };
    return trkPts;
}

std::vector<double> getVecPt4TrkWCoarse2()
{
    //std::vector<double> trkPts = { 0.50, 0.60, 0.70, 0.80, 0.90, 1.0, 1.20, 1.50, 2.0, 3.0, 4.0, 5.0, 8.0, 10.0, 12.0, 20.0, 60.0, 120.0, 300.0 };
    //std::vector<double> trkPts = { 0.50, 0.60, 0.70, 0.80, 0.90, 1.0, 1.20, 1.40, 1.60, 1.80, 2.0, 2.5, 3.0, 4.0, 5.0, 8.0, 10.0, 12.0, 20.0, 60.0, 120.0, 300.0 };
    //std::vector<double> trkPts = { 0.50, 0.60, 0.70, 0.80, 0.90, 1.0, 1.10, 1.20, 1.30, 1.40, 1.60, 1.80, 2.0, 2.5, 3.0, 4.0, 5.0, 8.0, 10.0, 12.0, 20.0, 60.0, 120.0, 300.0 };
    std::vector<double> trkPts = { 0.50, 0.80, 1.0, 1.30, 2.0, 2.5, 3.0, 4.0, 5.0, 8.0, 12.0, 20.0, 60.0, 120.0, 300.0 };
    return trkPts;
}

std::vector<double> getVecEta4TrkW(int nBins)
{
    double widthEta = (2.4*2) / nBins;
    std::vector<double> trkEtasMin;
    for (int i = 1; i <= nBins; ++i) {

        double tmp = -2.4 + widthEta*i;
        trkEtasMin.push_back(tmp);
    }

    return trkEtasMin;
}

std::vector<double> getVecAbsEta4TrkWCoarse()
{
    std::vector<double> trkAbsEtas = { 0.8, 1.6, 2.4 };
    return trkAbsEtas;
}

std::vector<int> getVecCent4TrkW(int nBins)
{
    int widthCent = 200 / nBins;
    std::vector<int> hiBinsMin;
    for (int i = 1; i <= nBins; ++i) {

        int tmp = widthCent*i;
        hiBinsMin.push_back(tmp);
    }

    return hiBinsMin;
}

std::vector<int> getVecCent4TrkWCoarse()
{
    //std::vector<int> hiBinsMin = {20, 60, 100, 180};
    //std::vector<int> hiBinsMin = {10, 20, 40, 60, 80, 100, 140, 180};
    std::vector<int> hiBinsMin = {10, 20, 30, 40, 50, 60, 70, 80, 100, 140, 180};

    return hiBinsMin;
}

std::vector<int> getVecCent4TrkWCoarse2()
{
    std::vector<int> hiBinsMin = {20, 60, 100, 180};

    return hiBinsMin;
}

int getBinPt4TrkW(double pt, std::vector<double>& trkPts, int nBins)
{
    for (int i = 0; i < nBins; ++i) {
        if (trkPts[i] < pt && pt <= trkPts[i+1]) {
            return i;
        }
    }

    return -1;
}

int getBinEta4TrkW(double eta, std::vector<double>& trkEtas, int nBins)
{
    for (int i = 0; i < nBins; ++i) {
        if (eta < trkEtas[i]) {
            return i;
        }
    }

    return -1;
}

int getBinCent4TrkW(int hiBin, std::vector<int>& hiBins, int nBins)
{
    for (int i = 0; i < nBins; ++i) {
        if (hiBin < hiBins[i]) {
            return i;
        }
    }

    return -1;
}

double getTrkPhiEffCorrection(double trkPhi, TH1D* h_effcorr)
{
    int iBin = h_effcorr->FindBin(trkPhi);
    if (iBin >= 1 && iBin <= h_effcorr->GetNbinsX()) {

        double effcorr = h_effcorr->GetBinContent(iBin);
        if (effcorr > 0.01) {
            return effcorr;
        }
    }

    return 1;
}

double getTrkEtaPhiEffCorrection(double trkEta, double trkPhi, TH2D* h_effcorr)
{
    int iBin = h_effcorr->FindBin(trkEta, trkPhi);
    if (iBin >= 1 && iBin <= (h_effcorr->GetNbinsX()*h_effcorr->GetNbinsY())) {

        double effcorr = h_effcorr->GetBinContent(iBin);
        if (effcorr > 0.01) {
            return effcorr;
        }
    }

    return 1;
}

std::vector<std::string> getTreeNamesHLTObj(int collType)
{
    std::vector<std::string> res;

    if (collisionIsHI2018((COLL::TYPE)collType)) {
        res = {
                //"HLT_HIL2Mu12_v",
                "HLT_HIL3Mu12_v",
                "HLT_HIDoubleEle10Gsf_v",
                "HLT_HIEle20Gsf_v",
        };
    }
    else if (collisionIsPP2017((COLL::TYPE)collType)) {
        res = {
                "HLT_HIL2Mu12_v",
                "HLT_HIL3Mu12_v",
                "HLT_HIDoublePhoton15_Eta3p1ForPPRef_Mass50to1000_v",
                "HLT_HIEle15_WPLoose_Gsf_v",
                "HLT_HIEle20_WPLoose_Gsf_v",
                "HLT_HIEle30_WPLoose_Gsf_v",
        };
    }

    return res;
}

std::vector<std::string> getTreeNamesHLTObjpp13TeV()
{
    std::vector<std::string> res;

    res = {
            "HLT_IsoMu20_v",
            "HLT_IsoMu24_v",
            "HLT_IsoMu24_eta2p1_v",
            "HLT_IsoMu27_v",
            "HLT_IsoMu30_v",
            "HLT_L1SingleMu18_v",
            "HLT_L1SingleMu25_v",
            "HLT_L2Mu10_v",
            "HLT_L2Mu50_v",
            "HLT_DoubleL2Mu50_v",
            "HLT_Mu20_v",
            "HLT_Mu27_v",
            "HLT_Mu50_v",
            "HLT_Mu55_v",
            "HLT_DoubleMu20_7_Mass0to30_Photon23_v",
            "HLT_Mu8_v",
            "HLT_Mu17_v",
            "HLT_Mu19_v",
            "HLT_Mu17_Photon30_IsoCaloId_v",
            "HLT_Mu18_Mu9_v",
            "HLT_Mu18_Mu9_DZ_v",
            "HLT_Mu20_Mu10_v",
            "HLT_Mu20_Mu10_DZ_v",
            "HLT_Mu23_Mu12_v",
            "HLT_Mu23_Mu12_DZ_v",
    };

    return res;
}

/*
 * find photons near an electron, but not the electron itself
 */
std::vector<int> indicesNearPhotons(ggHiNtuplizer& ggHi, int iEle, double dRmax)
{
    std::vector<int> res;
    double dR2min = 0.005*0.005;
    double dR2max = dRmax * dRmax;
    for (int i = 0; i < ggHi.nPho; ++i) {

        bool isEle = false;
        for (int iEle2 = 0; iEle2 < ggHi.nEle; ++iEle2) {
            if ( (getDR2((*ggHi.phoSCEta)[i], (*ggHi.phoSCPhi)[i], (*ggHi.eleSCEta)[iEle2], (*ggHi.eleSCEta)[iEle2]) < dR2min) ) isEle = true;
        }
        if (isEle) continue;

        if ( (getDR2((*ggHi.phoSCEta)[i], (*ggHi.phoSCPhi)[i], (*ggHi.eleSCEta)[iEle], (*ggHi.eleSCEta)[iEle]) > dR2max) ) continue;

        if ( !(((*ggHi.phoSCE)[i] / (TMath::CosH((*ggHi.phoSCEta)[i]))) > 10) ) continue;
        if ( !((*ggHi.phoHoverE)[i] < 0.13) ) continue;
        if ( !((*ggHi.phoSigmaIEtaIEta_2012)[i] < 0.012) ) continue;

        res.push_back(i);
    }

    return res;
}

double getSFweight(bool isMu, double pt, double eta, int cent)
{
    if (isMu) {
        return muonTnP::get_weight_nom(pt, eta, cent);
    }
    else {
        return eleSys::get_unc(-1, pt, eta, cent);
    }
}

int parseLepSysIndex(std::string sys_label)
{
    sys_label = toLowerCase(sys_label);

    if (sys_label.find("zmm") != std::string::npos) {

        if (sys_label.find("stat_lep_id_p") != std::string::npos) {
            return MUONTNP::k_stat_id_p;
        }
        else if (sys_label.find("stat_lep_id_m") != std::string::npos) {
            return MUONTNP::k_stat_id_m;
        }
        else if (sys_label.find("stat_lep_trk_p") != std::string::npos) {
            return MUONTNP::k_stat_trk_p;
        }
        else if (sys_label.find("stat_lep_trk_m") != std::string::npos) {
            return MUONTNP::k_stat_trk_m;
        }
        else if (sys_label.find("stat_lep_trig_p") != std::string::npos) {
            return MUONTNP::k_stat_trig_p;
        }
        else if (sys_label.find("stat_lep_trig_m") != std::string::npos) {
            return MUONTNP::k_stat_trig_m;
        }
        else if (sys_label.find("sys_lep_id_p") != std::string::npos) {
            return MUONTNP::k_sys_id_p;
        }
        else if (sys_label.find("sys_lep_id_m") != std::string::npos) {
            return MUONTNP::k_sys_id_m;
        }
        else if (sys_label.find("sys_lep_trk_p") != std::string::npos) {
            return MUONTNP::k_sys_trk_p;
        }
        else if (sys_label.find("sys_lep_trk_m") != std::string::npos) {
            return MUONTNP::k_sys_trk_m;
        }
        else if (sys_label.find("sys_lep_trig_p") != std::string::npos) {
            return MUONTNP::k_sys_trig_p;
        }
        else if (sys_label.find("sys_lep_trig_m") != std::string::npos) {
            return MUONTNP::k_sys_trig_m;
        }
    }
    else if (sys_label.find("zee") != std::string::npos) {

        if (sys_label.find("pp17_tot_id") != std::string::npos) {
            return ELESYS::k_unc_pp17_tot_id;
        }
        else if (sys_label.find("pbpb18_tot_reco") != std::string::npos) {
            return ELESYS::k_unc_pbpb18_tot_reco;
        }
        else if (sys_label.find("pbpb18_stat_reco") != std::string::npos) {
            return ELESYS::k_unc_pbpb18_stat_reco;
        }
        else if (sys_label.find("pbpb18_sys_reco") != std::string::npos) {
            return ELESYS::k_unc_pbpb18_sys_reco;
        }
        else if (sys_label.find("pbpb18_tot_id") != std::string::npos) {
            return ELESYS::k_unc_pbpb18_tot_id;
        }
        else if (sys_label.find("pbpb18_stat_id") != std::string::npos) {
            return ELESYS::k_unc_pbpb18_stat_id;
        }
        else if (sys_label.find("pbpb18_sys_id") != std::string::npos) {
            return ELESYS::k_unc_pbpb18_sys_id;
        }
    }

    return -1;
}

double getLepSysVar(bool isMu, int sys_index, double pt, double eta, int cent)
{
    if (isMu) {
        return muonTnP::get_unc(sys_index, pt, eta, cent);
    }
    else {
        return eleSys::get_unc(sys_index, pt, eta, cent);
    }
}

std::vector<double> parseRanges(std::string rangesStr, int indexMinMax)
{
    std::vector<std::string> vecStr = split(rangesStr, ",", false, false);

    std::vector<double> res;
    for (std::vector<std::string>::const_iterator it = vecStr.begin(); it != vecStr.end(); ++it) {
        std::vector<std::string> vecStrTmp = split((*it), ":");

        res.push_back(std::atof(vecStrTmp[indexMinMax].c_str()));
    }

    return res;
}

std::vector<double> parseRangesMin(std::string rangesStr)
{
    return parseRanges(rangesStr, 0);
}

std::vector<double> parseRangesMax(std::string rangesStr)
{
    return parseRanges(rangesStr, 1);
}

/*
 * parse the min vPt used when filling the given TH1
 * Example : returns 20 if given "h_dphi_vPt20_40_trkPt1_2_cent0_30"
 */
double parseVPtMin(std::string histPath)
{
    // histPath = "h_dphi_vPt20_40_trkPt1_2_cent0_30"
    size_t pos = histPath.find("vPt");
    std::string strTmp = histPath.substr(pos + std::string("vPt").size());
    // strTmp = "20_40_trkPt1_2_cent0_30"

    pos = strTmp.find("_");
    strTmp = strTmp.substr(0,pos);
    // strTmp = "20"

    return std::atof(strTmp.c_str());
}

/*
 * parse the max vPt used when filling the given TH1
 * Example : returns 40 if given "h_dphi_vPt20_40_trkPt1_2_cent0_30"
 */
double parseVPtMax(std::string histPath)
{
    // histPath = "h_dphi_vPt20_40_trkPt1_2_cent0_30"
    size_t pos = histPath.find("vPt");
    std::string strTmp = histPath.substr(pos + std::string("vPt").size());
    // strTmp = "20_40_trkPt1_2_cent0_30"

    pos = strTmp.find("_");
    strTmp = strTmp.substr(pos+1);
    // strTmp = "40_trkPt1_2_cent0_30"

    pos = strTmp.find("_");
    strTmp = strTmp.substr(0,pos);
    // strTmp = "40"

    return std::atof(strTmp.c_str());
}

double parseTrkPtMin(std::string histPath)
{
    size_t pos = histPath.find("trkPt");
    std::string strTmp = histPath.substr(pos + std::string("trkPt").size());

    pos = strTmp.find("_");
    strTmp = strTmp.substr(0,pos);
    strTmp = replaceFirst(strTmp, "p", ".");

    return std::atof(strTmp.c_str());
}

double parseTrkPtMax(std::string histPath)
{
    size_t pos = histPath.find("trkPt");
    std::string strTmp = histPath.substr(pos + std::string("trkPt").size());

    pos = strTmp.find("_");
    strTmp = strTmp.substr(pos+1);

    pos = strTmp.find("_");
    strTmp = strTmp.substr(0,pos);
    strTmp = replaceFirst(strTmp, "p", ".");

    return std::atof(strTmp.c_str());
}

int parseCentMin(std::string histPath)
{
    // histPath = "h_dphi_vPt20_40_trkPt1_2_cent0_30"
    size_t pos = histPath.find("cent");
    std::string strTmp = histPath.substr(pos + std::string("cent").size());
    // strTmp = "0_30"

    pos = strTmp.find("_");
    strTmp = strTmp.substr(0,pos);
    // strTmp = "0"

    return std::atoi(strTmp.c_str());
}

int parseCentMax(std::string histPath)
{
    // histPath = "h_dphi_vPt20_40_trkPt1_2_cent0_30"
    size_t pos = histPath.find("cent");
    std::string strTmp = histPath.substr(pos + std::string("cent").size());
    // strTmp = "0_30"

    pos = strTmp.find("_");
    strTmp = strTmp.substr(pos+1);
    // strTmp = "30"

    pos = strTmp.find("_");
    strTmp = strTmp.substr(0,pos);
    // strTmp = "30"

    return std::atof(strTmp.c_str());
}

double parseVTrkDPhiMin(std::string text)
{
    // histPath = "h_xivh_vPt20_40_trkPt1_2_cent0_30_vTrkDphi0p875_xyz"
    size_t pos = text.find("vTrkDphi");
    if (pos == std::string::npos) {
        return -1;
    }
    std::string strTmp = text.substr(pos + std::string("vTrkDphi").size());
    // strTmp = "0p875_xyz"

    pos = strTmp.find("_");
    strTmp = strTmp.substr(0,pos);
    // strTmp = "0p875"

    strTmp = replaceAll(strTmp, "p", ".");
    // strTmp = "0.875"

    return std::atof(strTmp.c_str());
}

std::string parseVTrkObs(std::string label)
{
    if (label.find("dphi") == 0 || label.find("h_dphi") == 0) {
        return "dphi";
    }
    else if (label.find("ratio_dphi") == 0 || label.find("h_ratio_dphi") == 0) {
        return "ratio_dphi";
    }
    else if (label.find("xivh") == 0 || label.find("h_xivh") == 0) {
        return "xivh";
    }
    else if (label.find("ratio_xivh") == 0 || label.find("h_ratio_xivh") == 0) {
        return "ratio_xivh";
    }
    else if (label.find("trkPt") == 0 || label.find("h_trkPt") == 0) {
        return "trkpt";
    }
    else if (label.find("ratio_trkPt") == 0 || label.find("h_ratio_trkPt") == 0) {
        return "ratio_trkpt";
    }

    return "";
}

bool isDphi(std::string label)
{
    return (parseVTrkObs(label) == "dphi");
}

bool isXivh(std::string label)
{
    return (parseVTrkObs(label) == "xivh");
}

bool isTrkPt(std::string label)
{
    return (parseVTrkObs(label) == "trkpt");
}

bool isRatioDphi(std::string label)
{
    return (parseVTrkObs(label) == "ratio_dphi");
}

bool isRatioXivh(std::string label)
{
    return (parseVTrkObs(label) == "ratio_xivh");
}

bool isRatioTrkPt(std::string label)
{
    return (parseVTrkObs(label) == "ratio_trkpt");
}

bool isDiffDphi(std::string label)
{
    return (parseVTrkObs(label) == "diff_dphi");
}

bool isRatioVTrk(std::string label)
{
    return (isRatioDphi(label) || isRatioXivh(label) || isRatioTrkPt(label));
}

#endif
