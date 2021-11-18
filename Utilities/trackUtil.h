/*
 * utilities for tracks/charged particles
 */
#ifndef TRACKUTIL_H_
#define TRACKUTIL_H_

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

#include "../TreeHeaders/trackTree.h"
#include "../TreeHeaders/trackSkimTree.h"
#include "eventUtil.h"

int getTrkMult(trackSkim& trks, double ptMin, double etaMax);
int getTrkMult(Tracks& trks, double ptMin, double etaMax);
bool passedTrkSelection(trackSkim& trks, int i, int collType);
bool passedTrkSelection(Tracks& trks, int i, int collType, bool ismAOD = false);

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

bool passedTrkSelection(Tracks& trks, int i, int collType, bool ismAOD)
{
    if (collisionIsHI2018((COLL::TYPE)collType)) {
        // cuts for 2018
        if (ismAOD) {
            if (!((*trks.highPurity_v)[i] == 1)) return false;
            if (!((*trks.trkPtError_v)[i] / (*trks.trkPt_v)[i] < 0.1 &&
                  std::fabs((*trks.trkDzFirstVtx)[i] / (*trks.trkDzErrFirstVtx)[i]) < 3 &&
                  std::fabs((*trks.trkDxyFirstVtx)[i] / (*trks.trkDxyErrFirstVtx)[i]) < 3))  return false;
            if (!((*trks.trkNHits)[i] >= 11))  return false;
            if (!((*trks.trkNormChi2)[i] / (float)((*trks.trkNLayers)[i]) < 0.18))  return false;

            if ((*trks.trkPt_v)[i] > 20) {
                float Et = ((*trks.pfHcal_v)[i] + (*trks.pfEcal_v)[i]) / TMath::CosH((*trks.trkEta_v)[i]);
                if ( !(Et > 0.5 * (*trks.trkPt_v)[i]) )  return false;
            }
        }
        else {
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

#endif
