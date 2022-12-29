/*
 * utilities for electron/photon objects
 */
#ifndef EGAMMAUTIL_H_
#define EGAMMAUTIL_H_

#include <TFile.h>
#include <TTree.h>
#include <TObjArray.h>
#include <TMath.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TRandom3.h>

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>

#include "../TreeHeaders/ggHiNtuplizerTree.h"
#include "../TreeHeaders/trackTree.h"
#include "../TreeHeaders/pfCandTree.h"
#include "eventUtil.h"
#include "trackUtil.h"
#include "physicsUtil.h"

/*
 * simple object to contain info about a PF cand
 */
struct basicPFCand {
    basicPFCand() : key(0), id(-1), pt(-1), eta(-999999), phi(-999999) {}

    unsigned long key; // key for this PF cand, ref https://github.com/cms-sw/cmssw/blob/master/DataFormats/Common/interface/Ptr.h#L163
    int id; // ParticleType of PFCandidate
    float pt;
    float eta;
    float phi;
};

namespace egUtil {

  enum defn_phi_flow_mod {
      k_PF_ch=0,        // multiplicity of PF charged hadrons
      k_PF_ch_trkID=1,  // multiplicity of PF charged hadrons passing trkID
      k_PF_ch_trkID_wE=2,  // energy weighted multiplicity of PF charged hadrons passing trkID
      k_PF_all_wE=3,    // energy weighted multiplicity of all PF cands
      k_PF_all_trkID_wE=4,  // energy weighted multiplicity of all PF cands with ch PF passing trkID
      k_EPHF_PF_ch=5,   // multiplicity of PF charged hadrons, fit using event plane angles from HF
      N_defn_phi_flow_mod,
  };

  // if true, then particles are required to be charged
  const bool phi_flow_mod_ch_flag[N_defn_phi_flow_mod] = {
      true,
      true,
      true,
      false,
      false,
      true,
  };

  // if true, then charged particles are required to pass trkID
  const bool phi_flow_mod_trkID_flag[N_defn_phi_flow_mod] = {
      false,
      true,
      true,
      false,
      true,
      false,
  };

  // if true, then particles are weighted by their energy
  const bool phi_flow_mod_wE[N_defn_phi_flow_mod] = {
      false,
      false,
      true,
      true,
      true,
      false,
  };

  enum footprintMode {
      noRemoval=0,  // do not remove PF footprint
      matchKey=1,     // remove PF footprint by matching keys of candidates
      matchKin=2,     // remove PF footprint by matching kinematics of candidates
      matchEtaPhi=3,     // remove PF footprint by matching direction of candidates
  };

  const float limit_match_kin = 0.00001;
  const float limit_match_eta_phi = 0.001;
}

int getIndexGenMatched2Pho(ggHiNtuplizer& ggHi, int iPho, int pdg = 0, float mindR = 0, float maxdR = 999999, float minpt = -1, float maxpt = 999999, float maxdR_maxPt = 0.15);
int getIndexGenMatched(ggHiNtuplizer& ggHi, double reta, double rphi, int pdg = 0, float mindR = 0, float maxdR = 999999, float minpt = -1, float maxpt = 999999, float maxdR_maxPt = 0.15);

int getIndexPFMatched2Pho(pfCand& pfs, ggHiNtuplizer& ggHi, int iPho, int pfId = 0, float mindR = 0, float maxdR = 999999, float minpt = -1, float maxpt = 999999, float maxdR_maxPt = 0.15);
int getIndexPFMatched(pfCand& pfs, double reta, double rphi, int pfId = 0, float mindR = 0, float maxdR = 999999, float minpt = -1, float maxpt = 999999, float maxdR_maxPt = 0.15);

double getTrkIso(Tracks& trks, ggHiNtuplizer& ggHi, int iEG, double r1=0.4, double r2=0.00, double threshold=0, double jWidth=0.0, bool applyTrkID = false, int colType = -1);
double getTrkIsoSubUE(Tracks& trks, ggHiNtuplizer& ggHi, int iEG, double r1=0.4, double r2=0.00, double threshold=0, double jWidth=0.0, bool applyTrkID = false, bool excludeCone = true, int colType = -1);
double getTrkIso(Tracks& trks, double egEta, double egPhi, double r1=0.4, double r2=0.00, double threshold=0, double jWidth=0.0, bool applyTrkID = false, int colType = -1);
double getTrkIsoSubUE(Tracks& trks, double egEta, double egPhi, double r1=0.4, double r2=0.00, double threshold=0, double jWidth=0.0, bool applyTrkID = false, bool excludeCone = true, int colType = -1);

double getPFIso(pfCand& pfs, ggHiNtuplizer& ggHi, int iEG, int pfId, double r1=0.4, double r2=0.00, double threshold=0, double jWidth=0.0, int trkOpt = -1,
                int footprintOpt = egUtil::matchKey, const TVector3& vtxPos = TVector3(0,0,-999));
double getPFIsoSubUE(pfCand& pfs, ggHiNtuplizer& ggHi, int iEG, int pfId, double r1=0.4, double r2=0.00, double threshold=0, double jWidth=0.0, int trkOpt = -1,
                     int footprintOpt = egUtil::matchKey, const TVector3& vtxPos = TVector3(0,0,-999),
                     double vn_2 = 0, double angEPv2 = -999, double vn_3 = 0, double angEPv3 = -999);
double getPFIso(pfCand& pfs, double egEta, double egPhi, int pfId, double r1=0.4, double r2=0.00, double threshold=0, double jWidth=0.0, int trkOpt = -1,
                std::vector<basicPFCand> linkPFs = {}, int footprintOpt = egUtil::matchKey, const TVector3& vtxPos = TVector3(0,0,-999));
double getPFIsoSubUE(pfCand& pfs, double egEta, double egPhi, int pfId, double r1=0.4, double r2=0.00, double threshold=0, double jWidth=0.0, int trkOpt = -1,
                     std::vector<basicPFCand> linkPFs = {}, int footprintOpt = egUtil::matchKey, const TVector3& vtxPos = TVector3(0,0,-999),
                     double vn_2 = 0, double angEPv2 = -999, double vn_3 = 0, double angEPv3 = -999);

std::vector<basicPFCand> getLinkedPFCands(ggHiNtuplizer& ggHi, int iEG);
int isLinkedPFcand(std::vector<basicPFCand> linkPFs, const int id, const float eta, const float phi);
int isLinkedPFcand(std::vector<basicPFCand> linkPFs, const int id, const float pt, const float eta, const float phi);
int isLinkedPFcand(std::vector<basicPFCand> linkPFs, const unsigned long key);

bool isSamePFcand(const int id1, const float eta1, const float phi1, const int id2, const float eta2, const float phi2);
bool isSamePFcand(const int id1, const float pt1, const float eta1, const float phi1, const int id2, const float pt2, const float eta2, const float phi2);
bool isSamePFcand(const unsigned long key1, const unsigned long key2);
int getIndexSamePFcand(pfCand& pfs, const int id, const float pt, const float eta, const float phi);

int getIndexGenMatched2Pho(ggHiNtuplizer& ggHi, int iPho, int pdg, float mindR, float maxdR, float minpt, float maxpt, float maxdR_maxPt)
{
    return getIndexGenMatched(ggHi, (*ggHi.phoEta)[iPho], (*ggHi.phoPhi)[iPho], pdg, mindR, maxdR, minpt, maxpt, maxdR_maxPt);
}

int getIndexGenMatched(ggHiNtuplizer& ggHi, double reta, double rphi, int pdg, float mindR, float maxdR, float minpt, float maxpt, float maxdR_maxPt)
{
    const float mindR2 = (mindR < 0) ? 0 : mindR*mindR;
    const float maxdR2 = maxdR*maxdR;
    const float maxdR2_maxPt = maxdR_maxPt*maxdR_maxPt;
    float dR2ref = 999999;
    float ptref = -1;
    int res = -1;

    for (int i = 0; i < ggHi.nMC; ++i) {

        if ( (*ggHi.mcStatus)[i] != 1 ) continue;
        if ( pdg != 0 && std::abs((*ggHi.mcPID)[i]) != pdg ) continue;
        if ( (*ggHi.mcPt)[i] < minpt ) continue;
        if ( (*ggHi.mcPt)[i] > maxpt ) continue;

        double dR2 = getDR2(reta, rphi, (*ggHi.mcEta)[i], (*ggHi.mcPhi)[i]);

        if (dR2 < mindR2) continue;
        if (dR2 > maxdR2) continue;

        if (dR2 < maxdR2_maxPt && (*ggHi.mcPt)[i] > ptref) {
            res = i;
            dR2ref = dR2;
            ptref = (*ggHi.mcPt)[i];
        }
        else if (dR2 >= maxdR2_maxPt && dR2 < dR2ref) {
            res = i;
            dR2ref = dR2;
        }
    }

    return res;
}

int getIndexPFMatched2Pho(pfCand& pfs, ggHiNtuplizer& ggHi, int iPho, int pfId, float mindR, float maxdR, float minpt, float maxpt, float maxdR_maxPt)
{
    return getIndexPFMatched(pfs, (*ggHi.phoEta)[iPho], (*ggHi.phoPhi)[iPho], pfId, mindR, maxdR, minpt, maxpt, maxdR_maxPt);
}

int getIndexPFMatched(pfCand& pfs, double reta, double rphi, int pfId, float mindR, float maxdR, float minpt, float maxpt, float maxdR_maxPt)
{
    const float mindR2 = (mindR < 0) ? 0 : mindR*mindR;
    const float maxdR2 = maxdR*maxdR;
    const float maxdR2_maxPt = maxdR_maxPt*maxdR_maxPt;
    float dR2ref = 999999;
    float ptref = -1;
    int res = -1;

    for (int i = 0; i < pfs.nPFpart; ++i) {

        if ( pfId != 0 && (*pfs.pfId)[i] != pfId ) continue;
        if ( (*pfs.pfPt)[i] < minpt ) continue;
        if ( (*pfs.pfPt)[i] > maxpt ) continue;

        double dR2 = getDR2(reta, rphi, (*pfs.pfEta)[i], (*pfs.pfPhi)[i]);

        if (dR2 < mindR2) continue;
        if (dR2 > maxdR2) continue;

        if (dR2 < maxdR2_maxPt && (*pfs.pfPt)[i] > ptref) {
            res = i;
            dR2ref = dR2;
            ptref = (*pfs.pfPt)[i];
        }
        else if (dR2 >= maxdR2_maxPt && dR2 < dR2ref) {
            res = i;
            dR2ref = dR2;
        }
    }

    return res;
}

double getTrkIso(Tracks& trks, ggHiNtuplizer& ggHi, int iEG, double r1, double r2, double threshold, double jWidth, bool applyTrkID, int colType)
{
    return getTrkIso(trks, (*ggHi.phoSCEta)[iEG], (*ggHi.phoSCPhi)[iEG], r1, r2, threshold, jWidth, applyTrkID, colType);
}

double getTrkIsoSubUE(Tracks& trks, ggHiNtuplizer& ggHi, int iEG, double r1, double r2, double threshold, double jWidth, bool applyTrkID, bool excludeCone, int colType)
{
    return getTrkIsoSubUE(trks, (*ggHi.phoSCEta)[iEG], (*ggHi.phoSCPhi)[iEG], r1, r2, threshold, jWidth, applyTrkID, excludeCone, colType);
}

double getTrkIso(Tracks& trks, double egEta, double egPhi, double r1, double r2, double threshold, double jWidth, bool applyTrkID, int colType)
{
    double totalEt = 0;

    for (int i = 0; i < trks.nTrk; ++i) {

        double teta = trks.trkEta[i];
        double tphi = trks.trkPhi[i];

        double dEta = std::abs(egEta - teta);
        double dPhi = getDPHI(tphi, egPhi);
        double dR2 = dEta*dEta + dPhi*dPhi;
        double tpt = trks.trkPt[i];

        // Jurassic Cone /////
        if (dR2 > r1 * r1) continue;
        if (dR2 < r2 * r2) continue;
        if (std::abs(dEta) < jWidth)  continue;
        if (tpt < threshold) continue;

        if (!(trks.highPurity[i] == 1)) continue;
        if (applyTrkID) {
            if (!passedTrkSelection(trks, i, colType))  continue;
        }

        totalEt += tpt;
    }

    return totalEt;
}

double getTrkIsoSubUE(Tracks& trks, double egEta, double egPhi, double r1, double r2, double threshold, double jWidth, bool applyTrkID, bool excludeCone, int colType)
{
    double totalEt = 0;

    for (int i = 0; i < trks.nTrk; ++i) {

        double teta = trks.trkEta[i];
        double tphi = trks.trkPhi[i];

        double dEta = std::abs(egEta - teta);
        if (dEta > r1) continue;
        if (dEta < jWidth)  continue;

        if (!(trks.highPurity[i] == 1)) continue;
        if (applyTrkID) {
            if (!passedTrkSelection(trks, i, colType))  continue;
        }

        double dPhi = getDPHI(tphi, egPhi);
        double dR2 = dEta*dEta + dPhi*dPhi;
        double tpt = trks.trkPt[i];

        // Jurassic Cone /////
        if (dR2 < r2 * r2) continue;
        if (tpt < threshold) continue;
        totalEt += tpt;
    }

    double areaStrip = 4*M_PI*(r1-jWidth);   // strip area over which UE is estimated
    double areaCone = M_PI*r1*r1;       // area inside which isolation is to be applied
    if (jWidth > 0) {
        // calculate overlap area between disk with radius r2 and rectangle of width jwidth
        double angTmp = std::acos(jWidth / r1);
        double lenTmp = std::sqrt(r1*r1 - jWidth*jWidth) * 2;
        double areaTwoTriangles = lenTmp * jWidth;
        double areaTwoArcs = (M_PI - 2*angTmp) * r1 * r1;
        areaCone -= (areaTwoTriangles + areaTwoArcs);
    }

    double areaInnerCone = 0;
    if (r2 > jWidth) {
        areaInnerCone = M_PI*r2*r2;
        if (jWidth > 0) {
            double angTmp = std::acos(jWidth / r2);
            double lenTmp = std::sqrt(r2*r2 - jWidth*jWidth) * 2;
            double areaTwoTriangles = lenTmp * jWidth;
            double areaTwoArcs = (M_PI - 2*angTmp) * r2 * r2;
            areaInnerCone -= (areaTwoTriangles + areaTwoArcs);
        }
    }
    areaStrip -= areaInnerCone;
    areaCone -= areaInnerCone;

    double coneEt = getTrkIso(trks, egEta, egPhi, r1, r2, threshold, jWidth, applyTrkID, colType);
    double ueEt = totalEt;
    double ueArea = areaStrip;
    if (excludeCone) { // exclude the isolation cone from strip area
        // Note the result for excludeCone=True is a scaled version ofthe result for excludeCone=False
        // In particular : f[excludeCone=True] = f[excludeCone=False] * 4 / (4 - R)
        ueEt = totalEt - coneEt;
        ueArea = areaStrip - areaCone;
    }

    return coneEt - ueEt * (areaCone / ueArea);
}

double getPFIso(pfCand& pfs, ggHiNtuplizer& ggHi, int iEG, int pfId, double r1, double r2, double threshold, double jWidth, int trkOpt, int footprintOpt, const TVector3& vtxPos)
{
    std::vector<basicPFCand> linkPFs = (footprintOpt != egUtil::noRemoval) ? getLinkedPFCands(ggHi, iEG) : std::vector<basicPFCand>();

    return getPFIso(pfs, (*ggHi.phoEta)[iEG], (*ggHi.phoPhi)[iEG], pfId, r1, r2, threshold, jWidth, trkOpt, linkPFs, footprintOpt, vtxPos);
}

double getPFIsoSubUE(pfCand& pfs, ggHiNtuplizer& ggHi, int iEG, int pfId, double r1, double r2, double threshold, double jWidth, int trkOpt,
                     int footprintOpt, const TVector3& vtxPos,
                     double vn_2, double angEPv2, double vn_3, double angEPv3)
{
    std::vector<basicPFCand> linkPFs = (footprintOpt != egUtil::noRemoval) ? getLinkedPFCands(ggHi, iEG) : std::vector<basicPFCand>();

    return getPFIsoSubUE(pfs, (*ggHi.phoEta)[iEG], (*ggHi.phoPhi)[iEG], pfId, r1, r2, threshold, jWidth, trkOpt, linkPFs, footprintOpt, vtxPos, vn_2, angEPv2, vn_3, angEPv3);
}

double getPFIso(pfCand& pfs, double egEta, double egPhi, int pfId, double r1, double r2, double threshold, double jWidth, int trkOpt,
                std::vector<basicPFCand> linkPFs, int footprintOpt, const TVector3& vtxPos)
{
    double totalEt = 0;

    for (int i = 0; i < pfs.nPFpart; ++i) {

        double pfeta = (*pfs.pfEta)[i];
        double pfphi = (*pfs.pfPhi)[i];

        double dEta = std::abs(egEta - pfeta);
        double dPhi = getDPHI(pfphi, egPhi);
        double dR2 = dEta*dEta + dPhi*dPhi;
        double pfpt = (*pfs.pfPt)[i];
        bool pfHasTrkInfo = (pfs.b_trkPt != 0);
        if (1 <= pfId && pfId <=3 && ((trkOpt / 100) % 10) == 1 && pfHasTrkInfo) {
            pfpt = (*pfs.trkPt)[i];
        }

        // Jurassic Cone /////
        if (dR2 > r1 * r1) continue;
        if (dR2 < r2 * r2) continue;
        if (std::abs(dEta) < jWidth)  continue;
        if (pfpt < threshold) continue;

        if ((*pfs.pfId)[i] != pfId) continue;

        if (1 <= pfId && pfId <=3) {
            int trkIDOpt = trkOpt % 10;
            if (trkIDOpt >= 0 && pfHasTrkInfo) {
                bool useTrkPt4ID = ((trkOpt / 10) % 10) == 1;
                if( !passedTrkSelection(pfs, i, trkIDOpt, useTrkPt4ID) ) continue;
            }
            else if (vtxPos.Z() > -987 && pfs.b_pfvz != 0) {
                float dz = std::abs((*pfs.pfvz)[i] - vtxPos.Z());
                if (dz > 0.2) continue;

                TVector3 pVec;
                pVec.SetPtEtaPhi(pfpt, pfeta, pfphi);

                double dxy = ( (vtxPos.X() - (*pfs.pfvx)[i])*pVec.Py() + ((*pfs.pfvy)[i] - vtxPos.Y())*pVec.Px() ) / pfpt;
                if (std::abs(dxy) > 0.1) continue;
            }
        }

        int iLinkPF = -1;
        if (footprintOpt == egUtil::matchKey) {
            iLinkPF = isLinkedPFcand(linkPFs, (*pfs.pfKey)[i]);
        }
        else if (footprintOpt == egUtil::matchKin) {
            iLinkPF = isLinkedPFcand(linkPFs, pfId, pfpt, pfeta, pfphi);
        }
        else if (footprintOpt == egUtil::matchEtaPhi) {
            iLinkPF = isLinkedPFcand(linkPFs, pfId, pfeta, pfphi);
        }

        if (iLinkPF >= 0) {
            linkPFs.erase(linkPFs.begin() + iLinkPF);
            continue;
        }

        totalEt += pfpt;
    }
    bool debug = false;
    if (debug) {
        int nLinkPFsRemain = linkPFs.size();
        if (nLinkPFsRemain > 0) {
            std::cout << "There may be not matched PF cands for egEta, egPhi = " << egEta << " , " << egPhi << std::endl;
            for (int iLPF = 0; iLPF < nLinkPFsRemain; ++iLPF) {
                if (linkPFs[iLPF].id != pfId) continue;
                std::cout << "PF id, pt, eta, phi = " << linkPFs[iLPF].id << " , "<< linkPFs[iLPF].pt << " , "<< linkPFs[iLPF].eta << " , "<< linkPFs[iLPF].phi << std::endl;
            }
        }
    }

    return totalEt;
}

double getPFIsoSubUE(pfCand& pfs, double egEta, double egPhi, int pfId, double r1, double r2, double threshold, double jWidth, int trkOpt,
                     std::vector<basicPFCand> linkPFs, int footprintOpt, const TVector3& vtxPos,
                     double vn_2, double angEPv2, double vn_3, double angEPv3)
{
    double totalEt = 0;

    for (int i = 0; i < pfs.nPFpart; ++i) {

        double pfeta = (*pfs.pfEta)[i];
        double pfphi = (*pfs.pfPhi)[i];

        double dEta = std::abs(egEta - pfeta);
        if (dEta > r1) continue;
        if (dEta < jWidth)  continue;

        if ((*pfs.pfId)[i] != pfId) continue;

        double dPhi = getDPHI(pfphi, egPhi);
        double dR2 = dEta*dEta + dPhi*dPhi;
        double pfpt = (*pfs.pfPt)[i];
        bool pfHasTrkInfo = (pfs.b_trkPt != 0);
        if (1 <= pfId && pfId <=3 && ((trkOpt / 100) % 10) == 1 && pfHasTrkInfo) {
            pfpt = (*pfs.trkPt)[i];
        }

        // Jurassic Cone /////
        if (dR2 <= r1 * r1) continue;  // exclude the isolation cone from strip area
        if (dR2 < r2 * r2) continue;
        if (pfpt < threshold) continue;

        if (1 <= pfId && pfId <=3) {
            int trkIDOpt = trkOpt % 10;
            if (trkIDOpt >= 0 && pfHasTrkInfo) {
                bool useTrkPt4ID = ((trkOpt / 10) % 10) == 1;
                if( !passedTrkSelection(pfs, i, trkIDOpt, useTrkPt4ID) ) continue;
            }
            else if (vtxPos.Z() > -987 && pfs.b_pfvz != 0) {
                float dz = std::abs((*pfs.pfvz)[i] - vtxPos.Z());
                if (dz > 0.2) continue;

                TVector3 pVec;
                pVec.SetPtEtaPhi(pfpt, pfeta, pfphi);

                double dxy = ( (vtxPos.X() - (*pfs.pfvx)[i])*pVec.Py() + ((*pfs.pfvy)[i] - vtxPos.Y())*pVec.Px() ) / pfpt;
                if (std::abs(dxy) > 0.1) continue;
            }
        }

        int iLinkPF = -1;
        if (footprintOpt == egUtil::matchKey) {
            iLinkPF = isLinkedPFcand(linkPFs, (*pfs.pfKey)[i]);
        }
        else if (footprintOpt == egUtil::matchKin) {
            iLinkPF = isLinkedPFcand(linkPFs, pfId, pfpt, pfeta, pfphi);
        }
        else if (footprintOpt == egUtil::matchEtaPhi) {
            iLinkPF = isLinkedPFcand(linkPFs, pfId, pfeta, pfphi);
        }

        if (iLinkPF >= 0) {
            linkPFs.erase(linkPFs.begin() + iLinkPF);
            continue;
        }

        totalEt += pfpt;
    }
    bool debug = false;
    if (debug) {
        int nLinkPFsRemain = linkPFs.size();
        if (nLinkPFsRemain > 0) {
            std::cout << "There may be not matched PF cands for egEta, egPhi = " << egEta << " , " << egPhi << std::endl;
            for (int iLPF = 0; iLPF < nLinkPFsRemain; ++iLPF) {
                if (linkPFs[iLPF].id != pfId) continue;
                std::cout << "PF id, pt, eta, phi = " << linkPFs[iLPF].id << " , "<< linkPFs[iLPF].pt << " , "<< linkPFs[iLPF].eta << " , "<< linkPFs[iLPF].phi << std::endl;
            }
        }
    }

    double areaStrip = 4*M_PI*(r1-jWidth);   // strip area over which UE is estimated
    double areaCone = M_PI*r1*r1;       // area inside which isolation is to be applied
    if (jWidth > 0) {
        // calculate overlap area between disk with radius r2 and rectangle of width jwidth
        double angTmp = std::acos(jWidth / r1);
        double lenTmp = std::sqrt(r1*r1 - jWidth*jWidth) * 2;
        double areaTwoTriangles = lenTmp * jWidth;
        double areaTwoArcs = (M_PI - 2*angTmp) * r1 * r1;
        areaCone -= (areaTwoTriangles + areaTwoArcs);
    }

    double areaInnerCone = 0;
    if (r2 > jWidth) {
        areaInnerCone = M_PI*r2*r2;
        if (jWidth > 0) {
            double angTmp = std::acos(jWidth / r2);
            double lenTmp = std::sqrt(r2*r2 - jWidth*jWidth) * 2;
            double areaTwoTriangles = lenTmp * jWidth;
            double areaTwoArcs = (M_PI - 2*angTmp) * r2 * r2;
            areaInnerCone -= (areaTwoTriangles + areaTwoArcs);
        }
    }
    areaStrip -= areaInnerCone;
    areaCone -= areaInnerCone;

    double ueEt = totalEt;
    double coneEt = getPFIso(pfs, egEta, egPhi, pfId, r1, r2, threshold, jWidth, trkOpt, linkPFs, footprintOpt, vtxPos);

    double termSub = 0;
    double flowSF = 1 + 2 * vn_2 * std::cos( 2 * getDPHI(egPhi, angEPv2) ) + 2 * vn_3 * std::cos( 3 * getDPHI(egPhi, angEPv3) );
    double ueArea = areaStrip - areaCone;

    termSub = ueEt * (areaCone / ueArea) * flowSF;

    return coneEt - termSub;
}

std::vector<basicPFCand> getLinkedPFCands(ggHiNtuplizer& ggHi, int iEG)
{
    std::vector<basicPFCand> res;

    if (ggHi.b_nPhoPF != 0) {
        for (int i = 0; i < ggHi.nPhoPF; ++i) {

            if ((*ggHi.ppfPhoIdx)[i] != iEG) continue;

            basicPFCand candTmp;
            candTmp.key = (*ggHi.ppfKey)[i];
            candTmp.id = (*ggHi.ppfId)[i];
            candTmp.pt = (*ggHi.ppfPt)[i];
            candTmp.eta = (*ggHi.ppfEta)[i];
            candTmp.phi = (*ggHi.ppfPhi)[i];

            res.push_back(candTmp);
        }
    }

    return res;
}

int isLinkedPFcand(std::vector<basicPFCand> linkPFs, const int id, const float eta, const float phi)
{
    for (std::vector<basicPFCand>::const_iterator itPF = linkPFs.begin(); itPF != linkPFs.end(); ++itPF) {
        if ( isSamePFcand((*itPF).id, (*itPF).eta, (*itPF).phi, id, eta, phi) ) return (int)(itPF-linkPFs.begin());
    }

    return -1;
}

int isLinkedPFcand(std::vector<basicPFCand> linkPFs, const int id, const float pt, const float eta, const float phi)
{
    for (std::vector<basicPFCand>::const_iterator itPF = linkPFs.begin(); itPF != linkPFs.end(); ++itPF) {
        if ( isSamePFcand((*itPF).id, (*itPF).pt, (*itPF).eta, (*itPF).phi, id, pt, eta, phi) ) return (int)(itPF-linkPFs.begin());
    }

    return -1;
}

int isLinkedPFcand(std::vector<basicPFCand> linkPFs, const unsigned long key)
{
    for (std::vector<basicPFCand>::const_iterator itPF = linkPFs.begin(); itPF != linkPFs.end(); ++itPF) {
        if ( isSamePFcand((*itPF).key, key) ) return (int)(itPF-linkPFs.begin());
    }

    return -1;
}

bool isSamePFcand(const int id1, const float eta1, const float phi1, const int id2, const float eta2, const float phi2)
{
    if (id1 != id2) return false;
    if ( std::abs((eta1-eta2)/eta1) > egUtil::limit_match_eta_phi ) return false;
    if ( std::abs(getDPHI(phi1, phi2)/phi1) > egUtil::limit_match_eta_phi ) return false;

    return true;
}

bool isSamePFcand(const int id1, const float pt1, const float eta1, const float phi1, const int id2, const float pt2, const float eta2, const float phi2)
{
    if (id1 != id2) return false;
    if ( std::abs(pt1-pt2) / pt1 > egUtil::limit_match_kin ) return false;
    if ( std::abs((eta1-eta2)/eta1) > egUtil::limit_match_kin ) return false;
    if ( std::abs(getDPHI(phi1, phi2)/phi1) > egUtil::limit_match_kin ) return false;

    return true;
}

bool isSamePFcand(const unsigned long key1, const unsigned long key2)
{
    return (key1 == key2);
}

int getIndexSamePFcand(pfCand& pfs, const int id, const float pt, const float eta, const float phi)
{
    for (int i = 0; i < pfs.nPFpart; ++i) {
        if ( isSamePFcand(id, pt, eta, phi, (*pfs.pfId)[i], (*pfs.pfPt)[i], (*pfs.pfEta)[i], (*pfs.pfPhi)[i]) ) {
            return i;
        }
    }

    return -1;
}

#endif
