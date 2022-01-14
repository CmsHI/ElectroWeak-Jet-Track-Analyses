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
#include "../TreeHeaders/pfCandTree.h"
#include "eventUtil.h"

int getTrkMult(trackSkim& trks, double ptMin, double etaMax);
int getTrkMult(Tracks& trks, double ptMin, double etaMax);
bool passedTrkSelection(trackSkim& trks, int i, int collType);
bool passedTrkSelection(Tracks& trks, int i, int collType, bool ismAOD = false);
bool passedTrkSelection(pfCand& pfs, int i, int collType, bool useTrkPt = false);
bool passedTrkSelPbPb18(unsigned char trkAlgo, float trkMVA, bool highPurity, float trkPtError, float trkPt,
                        float trkDz1, float trkDzError1, float trkDxy1, float trkDxyError1,
                        unsigned char trkNHit, float trkChi2, unsigned char trkNdof, unsigned char trkNlayer,
                        float pfHcal, float pfEcal, float trkEta);

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

bool passedTrkSelection(pfCand& pfs, int i, int collType, bool useTrkPt)
{
    float tmpPt = useTrkPt ? (*pfs.trkPt)[i] : (*pfs.pfPt)[i];

    if (collisionIsHI2018((COLL::TYPE)collType)) {
        // cuts for 2018
        if (((*pfs.trkAlgo)[i] == 6 && (*pfs.trkMVA)[i] < 0.98)) return false;

        if (!((*pfs.highPurity)[i] == 1)) return false;
        if (!((*pfs.trkPtError)[i] / tmpPt < 0.1 &&
              std::fabs((*pfs.trkDz1)[i] / (*pfs.trkDzError1)[i]) < 3 &&
              std::fabs((*pfs.trkDxy1)[i] / (*pfs.trkDxyError1)[i]) < 3))  return false;
        if (!((*pfs.trkNHit)[i] >= 11))  return false;
        if (!((*pfs.trkChi2)[i] / (float)(*pfs.trkNdof)[i] / (float)(*pfs.trkNlayer)[i] < 0.18))  return false;

        if (tmpPt > 20) {
            float Et = ((*pfs.pfHcalE)[i] + (*pfs.pfEcalE)[i]) / TMath::CosH((*pfs.pfEta)[i]);
            if ( !(Et > 0.5 * tmpPt) )  return false;
        }
    }
    else if (collisionIsHI((COLL::TYPE)collType)) {
        // cuts for 2015
        if (!(tmpPt <= 300 && std::fabs((*pfs.pfEta)[i]) < 2.4)) return false;
        if (!((*pfs.highPurity)[i] == 1)) return false;
        if (!((*pfs.trkPtError)[i] / tmpPt < 0.1 &&
              std::fabs((*pfs.trkDz1)[i] / (*pfs.trkDzError1)[i]) < 3 &&
              std::fabs((*pfs.trkDxy1)[i] / (*pfs.trkDxyError1)[i]) < 3))  return false;
        if (!((*pfs.trkChi2)[i] / (float)(*pfs.trkNdof)[i] / (float)(*pfs.trkNlayer)[i] < 0.15))  return false;
        if (!((*pfs.trkNHit)[i] >= 11))  return false;

        if (tmpPt > 20) {
            float Et = ((*pfs.pfHcalE)[i] + (*pfs.pfEcalE)[i]) / TMath::CosH((*pfs.pfEta)[i]);
            if ( !(Et > 0.5 * tmpPt) )  return false;
        }
    }
    else if (collisionIsPP2017((COLL::TYPE)collType)) {
        // cuts for pp 2017
        if (!((*pfs.highPurity)[i] == 1)) return false;
        if (!((*pfs.trkPtError)[i] / tmpPt < 0.1 &&
              std::fabs((*pfs.trkDz1)[i] / (*pfs.trkDzError1)[i]) < 3 &&
              std::fabs((*pfs.trkDxy1)[i] / (*pfs.trkDxyError1)[i]) < 3))  return false;
    }
    else if (collisionIsPP((COLL::TYPE)collType)) {
        // cuts for pp 2015
        if (!(tmpPt <= 300 && std::fabs((*pfs.pfEta)[i]) < 2.4)) return false;
        if (!((*pfs.highPurity)[i] == 1)) return false;
        if (!((*pfs.trkPtError)[i] / tmpPt < 0.1 &&
              std::fabs((*pfs.trkDz1)[i] / (*pfs.trkDzError1)[i]) < 3 &&
              std::fabs((*pfs.trkDxy1)[i] / (*pfs.trkDxyError1)[i]) < 3))  return false;

        if (tmpPt > 20) {
            float Et = ((*pfs.pfHcalE)[i] + (*pfs.pfEcalE)[i]) / TMath::CosH((*pfs.pfEta)[i]);
            if ( !(Et > 0.5 * tmpPt) )  return false;
        }
    }
    else {
        return false;
    }

    return true;
}

/*
 * passedTrkSelPbPb18(trkAlgo, trkMVA, highPurity, trkPtError, trkPt,
 *                    trkDz1, trkDzError1, trkDxy1, trkDxyError1,
 *                    trkNHit, trkChi2, trkNdof, trkNlayer,
 *                    pfHcal, pfEcal, trkEta)
 */
bool passedTrkSelPbPb18(unsigned char trkAlgo, float trkMVA, bool highPurity, float trkPtError, float trkPt,
                        float trkDz1, float trkDzError1, float trkDxy1, float trkDxyError1,
                        unsigned char trkNHit, float trkChi2, unsigned char trkNdof, unsigned char trkNlayer,
                        float pfHcal, float pfEcal, float trkEta)
{
    if ((trkAlgo == 6 && trkMVA < 0.98)) return false;

    if (!(highPurity == true)) return false;
    if (!(trkPtError / trkPt < 0.1 &&
          std::fabs(trkDz1 / trkDzError1) < 3 &&
          std::fabs(trkDxy1 / trkDxyError1) < 3))  return false;
    if (!(trkNHit >= 11))  return false;
    if (!(trkChi2 / (float)trkNdof / (float)trkNlayer < 0.18))  return false;

    if (trkPt > 20) {
        float Et = (pfHcal + pfEcal) / TMath::CosH(trkEta);
        if ( !(Et > 0.5 * trkPt) )  return false;
    }

    return true;
}

#endif
