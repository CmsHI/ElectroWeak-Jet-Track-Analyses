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

double getTrkIso(Tracks& trks, ggHiNtuplizer& ggHi, int iEG, double egPhi, double r1=0.4, double r2=0.00, double threshold=0, double jWidth=0.0, bool applyTrkID = false, int colType = -1);
double getTrkIsoSubUE(Tracks& trks, ggHiNtuplizer& ggHi, int iEG, double r1=0.4, double r2=0.00, double threshold=0, double jWidth=0.0, bool applyTrkID = false, int colType = -1);
double getTrkIso(Tracks& trks, double egEta, double egPhi, double r1=0.4, double r2=0.00, double threshold=0, double jWidth=0.0, bool applyTrkID = false, int colType = -1);
double getTrkIsoSubUE(Tracks& trks, double egEta, double egPhi, double r1=0.4, double r2=0.00, double threshold=0, double jWidth=0.0, bool applyTrkID = false, int colType = -1);

// WARNING : footprints cannot be removed in these functions as they are is kept in AOD only, but not in forest
double getPFIso(pfCand& pfs, ggHiNtuplizer& ggHi, int iEG, int pfId, double r1=0.4, double r2=0.00, double threshold=0, double jWidth=0.0);
double getPFIsoSubUE(pfCand& pfs, ggHiNtuplizer& ggHi, int iEG, int pfId, double r1=0.4, double r2=0.00, double threshold=0, double jWidth=0.0);
double getPFIso(pfCand& pfs, double egEta, double egPhi, int pfId, double r1=0.4, double r2=0.00, double threshold=0, double jWidth=0.0);
double getPFIsoSubUE(pfCand& pfs, double egEta, double egPhi, int pfId, double r1=0.4, double r2=0.00, double threshold=0, double jWidth=0.0);

double getTrkIso(Tracks& trks, ggHiNtuplizer& ggHi, int iEG, double r1, double r2, double threshold, double jWidth, bool applyTrkID, int colType)
{
    return getTrkIso(trks, (*ggHi.phoSCEta)[iEG], (*ggHi.phoSCPhi)[iEG], r1, r2, threshold, jWidth, applyTrkID, colType);
}

double getTrkIsoSubUE(Tracks& trks, ggHiNtuplizer& ggHi, int iEG, double r1, double r2, double threshold, double jWidth, bool applyTrkID, int colType)
{
    return getTrkIsoSubUE(trks, (*ggHi.phoSCEta)[iEG], (*ggHi.phoSCPhi)[iEG], r1, r2, threshold, jWidth, applyTrkID, colType);
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

double getTrkIsoSubUE(Tracks& trks, double egEta, double egPhi, double r1, double r2, double threshold, double jWidth, bool applyTrkID, int colType)
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
    // exclude the isolation cone from strip area
    ueEt = totalEt - coneEt;
    ueArea = areaStrip - areaCone;

    return coneEt - ueEt * (areaCone / ueArea);
}

double getPFIso(pfCand& pfs, ggHiNtuplizer& ggHi, int iEG, int pfId, double r1, double r2, double threshold, double jWidth)
{
    return getPFIso(pfs, (*ggHi.phoEta)[iEG], (*ggHi.phoPhi)[iEG], pfId, r1, r2, threshold, jWidth);
}

double getPFIsoSubUE(pfCand& pfs, ggHiNtuplizer& ggHi, int iEG, int pfId, double r1, double r2, double threshold, double jWidth)
{
    return getPFIsoSubUE(pfs, (*ggHi.phoEta)[iEG], (*ggHi.phoPhi)[iEG], pfId, r1, r2, threshold, jWidth);
}

double getPFIso(pfCand& pfs, double egEta, double egPhi, int pfId, double r1, double r2, double threshold, double jWidth)
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

        totalEt += pfpt;
    }

    return totalEt;
}

double getPFIsoSubUE(pfCand& pfs, double egEta, double egPhi, int pfId, double r1, double r2, double threshold, double jWidth)
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
        if (dR2 < r2 * r2) continue;
        if (pfpt < threshold) continue;
        totalEt += pfpt;
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

    double coneEt = getPFIso(pfs, egEta, egPhi, pfId, r1, r2, threshold, jWidth);
    double ueEt = totalEt;
    double ueArea = areaStrip;
    // exclude the isolation cone from strip area
    ueEt = totalEt - coneEt;
    ueArea = areaStrip - areaCone;

    return coneEt - ueEt * (areaCone / ueArea);
}

#endif
