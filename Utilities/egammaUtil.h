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

double getTrkIso(Tracks& trks, ggHiNtuplizer& ggHi, int iEG, double r1=0.4, double r2=0.00, double threshold=0, double jWidth=0.0, bool applyTrkID = false, int colType = -1);
double getTrkIsoSubUE(Tracks& trks, ggHiNtuplizer& ggHi, int iEG, double r1=0.4, double r2=0.00, double threshold=0, double jWidth=0.0, bool applyTrkID = false, bool excludeCone = true, int colType = -1);
double getTrkIso(Tracks& trks, double egEta, double egPhi, double r1=0.4, double r2=0.00, double threshold=0, double jWidth=0.0, bool applyTrkID = false, int colType = -1);
double getTrkIsoSubUE(Tracks& trks, double egEta, double egPhi, double r1=0.4, double r2=0.00, double threshold=0, double jWidth=0.0, bool applyTrkID = false, bool excludeCone = true, int colType = -1);

// WARNING : footprints cannot be removed in these functions as they are is kept in AOD only, but not in forest
double getPFIso(pfCand& pfs, ggHiNtuplizer& ggHi, int iEG, int pfId, double r1=0.4, double r2=0.00, double threshold=0, double jWidth=0.0,
                bool removeFootPrint = true);
double getPFIsoSubUE(pfCand& pfs, ggHiNtuplizer& ggHi, int iEG, int pfId, double r1=0.4, double r2=0.00, double threshold=0, double jWidth=0.0,
                     bool removeFootPrint = true,
                     double vn_2 = 0, double angEPv2 = -999, double vn_3 = 0, double angEPv3 = -999);
double getPFIso(pfCand& pfs, double egEta, double egPhi, int pfId, double r1=0.4, double r2=0.00, double threshold=0, double jWidth=0.0,
                std::vector<basicPFCand> linkPFs = {});
double getPFIsoSubUE(pfCand& pfs, double egEta, double egPhi, int pfId, double r1=0.4, double r2=0.00, double threshold=0, double jWidth=0.0,
                     std::vector<basicPFCand> linkPFs = {},
                     double vn_2 = 0, double angEPv2 = -999, double vn_3 = 0, double angEPv3 = -999);

std::vector<basicPFCand> getLinkedPFCands(ggHiNtuplizer& ggHi, int iEG);
int isLinkedPFcand(std::vector<basicPFCand> linkPFs, const int id, const float pt, const float eta, const float phi);
int isLinkedPFcand(std::vector<basicPFCand> linkPFs, const unsigned long key);

bool isSamePFcand(const int id1, const float pt1, const float eta1, const float phi1, const int id2, const float pt2, const float eta2, const float phi2);
bool isSamePFcand(const unsigned long key1, const unsigned long key2);

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

double getPFIso(pfCand& pfs, ggHiNtuplizer& ggHi, int iEG, int pfId, double r1, double r2, double threshold, double jWidth, bool removeFootPrint)
{
    std::vector<basicPFCand> linkPFs = (removeFootPrint) ? getLinkedPFCands(ggHi, iEG) : std::vector<basicPFCand>();

    return getPFIso(pfs, (*ggHi.phoEta)[iEG], (*ggHi.phoPhi)[iEG], pfId, r1, r2, threshold, jWidth, linkPFs);
}

double getPFIsoSubUE(pfCand& pfs, ggHiNtuplizer& ggHi, int iEG, int pfId, double r1, double r2, double threshold, double jWidth, bool removeFootPrint,
                     double vn_2, double angEPv2, double vn_3, double angEPv3)
{
    std::vector<basicPFCand> linkPFs = (removeFootPrint) ? getLinkedPFCands(ggHi, iEG) : std::vector<basicPFCand>();

    return getPFIsoSubUE(pfs, (*ggHi.phoEta)[iEG], (*ggHi.phoPhi)[iEG], pfId, r1, r2, threshold, jWidth, linkPFs, vn_2, angEPv2, vn_3, angEPv3);
}

double getPFIso(pfCand& pfs, double egEta, double egPhi, int pfId, double r1, double r2, double threshold, double jWidth, std::vector<basicPFCand> linkPFs)
{
    double totalEt = 0;

    for (int i = 0; i < pfs.nPFpart; ++i) {

        double pfeta = (*pfs.pfEta)[i];
        double pfphi = (*pfs.pfPhi)[i];

        double dEta = std::abs(egEta - pfeta);
        double dPhi = getDPHI(pfphi, egPhi);
        double dR2 = dEta*dEta + dPhi*dPhi;
        double pfpt = (*pfs.pfPt)[i];

        // Jurassic Cone /////
        if (dR2 > r1 * r1) continue;
        if (dR2 < r2 * r2) continue;
        if (std::abs(dEta) < jWidth)  continue;
        if (pfpt < threshold) continue;

        if ((*pfs.pfId)[i] != pfId) continue;

        int iLinkPF = isLinkedPFcand(linkPFs, (*pfs.pfKey)[i]);
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

double getPFIsoSubUE(pfCand& pfs, double egEta, double egPhi, int pfId, double r1, double r2, double threshold, double jWidth, std::vector<basicPFCand> linkPFs,
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

        // Jurassic Cone /////
        if (dR2 <= r1 * r1) continue;  // exclude the isolation cone from strip area
        if (dR2 < r2 * r2) continue;
        if (pfpt < threshold) continue;

        int iLinkPF = isLinkedPFcand(linkPFs, (*pfs.pfKey)[i]);
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
    double coneEt = getPFIso(pfs, egEta, egPhi, pfId, r1, r2, threshold, jWidth, linkPFs);

    double termSub = 0;
    double flowSF = 1 + 2 * vn_2 * std::cos( 2 * getDPHI(egPhi, angEPv2) ) + 2 * vn_3 * std::cos( 2 * getDPHI(egPhi, angEPv3) );
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

bool isSamePFcand(const int id1, const float pt1, const float eta1, const float phi1, const int id2, const float pt2, const float eta2, const float phi2)
{
    if (id1 != id2) return false;
    if ( TMath::Abs(pt1-pt2) / pt1 > 0.0001 ) return false;
    if ( TMath::Abs((eta1-eta2)/eta1) > 0.0001 ) return false;
    if ( TMath::Abs(getDPHI(phi1, phi2)/phi1) > 0.0001 ) return false;

    return true;
}

bool isSamePFcand(const unsigned long key1, const unsigned long key2)
{
    return (key1 == key2);
}

#endif
