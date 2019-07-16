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
#include "../TreeHeaders/trackSkimTree.h"
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
const int nEventPlaneBins = 8;

enum MIXMETHODS {
    k_match_hiBin,
    k_match_Npart,
    k_match_hiHF,
    k_match_hiHFhit,
    k_match_rho,
    k_match_nTrk,
    k_match_multperp,      // multiplicity perpendicular to tag
    k_match_energyperp,    // energy perpendicular to tag
    kN_MITMETHODS
};
const std::string mixMerhodsStr[kN_MITMETHODS] = {"match_hiBin", "match_Npart", "match_hiHF", "match_hiHFhit", "match_rho",
                                                  "match_nTrk", "match_multperp", "match_energyperp"};
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

void setTreeHiEvt(TTree* t, bool isMC = false);
void setTreeJet(TTree* t, bool isMC = false);
void setTreeTrack(TTree* t, bool enableVtx = false);
void setTreeSkimAna(TTree* t);
void setTreeHiGenParticle(TTree* t);
int getHiBin(int hiBin);
int getVzBin(float vz);
int getEventPlaneBin(double eventPlaneAngle);
int getNpartBin(float Npart);
int getNTrkBin(int nTrk, int nTrkMax = 14280);
int getHiHFBin(float hiHF, float hiHFMax = 6000);
int getHiHFhitBin(float hiHFhit, float hiHFhitMax = 180000);
int getRhoBin(double rho, double rhoMax = 250);
int getEventEnergyBin(double evtEnergy, double evtEnergyMax);
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
bool passedPerpTrkSelection(Tracks& trks, int i, int collType, double vPhi);
int getTrkMultPerp(Tracks& trks, int collType, double vPhi);

void setTreeHiEvt(TTree* t, bool isMC)
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
    t->SetBranchStatus("npus",1);
    t->SetBranchStatus("tnpus",1);
    if (isMC) {
        t->SetBranchStatus("Npart",1);
        t->SetBranchStatus("Ncoll",1);
        t->SetBranchStatus("Nhard",1);
    }
}

void setTreeJet(TTree* t, bool isMC)
{
    t->SetBranchStatus("*",0);     // disable all branches
    t->SetBranchStatus("nref",1);
    t->SetBranchStatus("jtpt",1);
    t->SetBranchStatus("jteta",1);
    t->SetBranchStatus("jtphi",1);
    t->SetBranchStatus("rawpt",1);
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

void setTreeTrack(TTree* t, bool enableVtx)
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
    t->SetBranchStatus("pfHcal",1);
    t->SetBranchStatus("pfEcal",1);

    if (enableVtx) {
        // vertex related
        t->SetBranchStatus("nVtx",1);
        t->SetBranchStatus("trkNVtx",1);
        t->SetBranchStatus("nTrkTimesnVtx",1);
        t->SetBranchStatus("trkAssocVtx",1);
    }
}

void setTreeSkimAna(TTree* t)
{
    t->SetBranchStatus("*",1);
}

void setTreeHiGenParticle(TTree* t)
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

int getRhoBin(double rho, double rhoMax)
{
    return getEventEnergyBin(rho, rhoMax);
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
    else if (mixMethodStr == "match_hihfhit" || mixMethodStr == "3") {
        return VJT::MIXMETHODS::k_match_hiHFhit;
    }
    else if (mixMethodStr == "match_rho" || mixMethodStr == "4") {
        return VJT::MIXMETHODS::k_match_rho;
    }
    else if (mixMethodStr == "match_ntrk" || mixMethodStr == "5") {
        return VJT::MIXMETHODS::k_match_nTrk;
    }
    else if (mixMethodStr == "match_multperp" || mixMethodStr == "6") {
        return VJT::MIXMETHODS::k_match_multperp;
    }
    else if (mixMethodStr == "match_energyperp" || mixMethodStr == "7") {
        return VJT::MIXMETHODS::k_match_energyperp;
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

        float Et = ((*trks.p_pfHcal)[i] + (*trks.p_pfEcal)[i]) / TMath::CosH((*trks.p_trkEta)[i]);
        if ((*trks.p_trkPt)[i] > 20 && !(Et > 0.5 * (*trks.p_trkPt)[i]))  return false;
    }
    else {
        // cuts for 2015
        if (!((*trks.p_trkPt)[i] <= 300 && std::fabs((*trks.p_trkEta)[i]) < 2.4)) return false;
        if (!((*trks.p_highPurity)[i] == 1)) return false;
        if (!((*trks.p_trkPtError)[i] / (*trks.p_trkPt)[i] < 0.1 &&
              std::fabs((*trks.p_trkDz1)[i] / (*trks.p_trkDzError1)[i]) < 3 &&
              std::fabs((*trks.p_trkDxy1)[i] / (*trks.p_trkDxyError1)[i]) < 3))  return false;

        float Et = ((*trks.p_pfHcal)[i] + (*trks.p_pfEcal)[i]) / TMath::CosH((*trks.p_trkEta)[i]);
        if ((*trks.p_trkPt)[i] > 20 && !(Et > 0.5 * (*trks.p_trkPt)[i]))  return false;

        return true;
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

        float Et = (trks.pfHcal[i] + trks.pfEcal[i]) / TMath::CosH(trks.trkEta[i]);
        if (trks.trkPt[i] > 20 && !(Et > 0.5 * trks.trkPt[i]))  return false;
    }
    else {
        // cuts for 2015
        if (!(trks.trkPt[i] <= 300 && std::fabs(trks.trkEta[i]) < 2.4)) return false;
        if (!(trks.highPurity[i] == 1)) return false;
        if (!(trks.trkPtError[i] / trks.trkPt[i] < 0.1 &&
              std::fabs(trks.trkDz1[i] / trks.trkDzError1[i]) < 3 &&
              std::fabs(trks.trkDxy1[i] / trks.trkDxyError1[i]) < 3))  return false;

        float Et = (trks.pfHcal[i] + trks.pfEcal[i]) / TMath::CosH(trks.trkEta[i]);
        if (trks.trkPt[i] > 20 && !(Et > 0.5 * trks.trkPt[i]))  return false;

        return true;
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

bool passedPerpTrkSelection(Tracks& trks, int i, int collType, double vPhi)
{
    if (!passedTrkSelection(trks, i, collType))  return false;
    if (!(trks.trkPt[i] > 0.5))  return false;
    if (!(std::fabs(trks.trkEta[i]) < 2.4))  return false;;

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


#endif
