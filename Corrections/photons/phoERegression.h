#ifndef PHOEREGRESSION_H_
#define PHOEREGRESSION_H_

#include <TMath.h>

#include "TMVA/IMethod.h"
#include "TMVA/MethodBase.h"
#include "TMVA/MethodCuts.h"
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>

/*
 * NOTES BEFORE YOU USE (REQUIRES ACTION !) :
 * 1) These corrections are optimized for photon collection under ggHiNtuplizerGED/EventTree
 * No good performance is guaranteed if they are applied to photons in ggHiNtuplizer/EventTree
 *
 * 2) Not all the information needed is available in the default HiForest
 * You should add the following lines into foresting config to get all the information needed by regression
 *
 * process.ggHiNtuplizerGED.doEffectiveAreas = cms.bool(True)
 * process.ggHiNtuplizerGED.doPhoERegression = cms.bool(True)
 *
 * 3) All the xml files for 2018 PbPb and 2017 pp are available under /afs/cern.ch/work/k/katatar/public/photonEnergyRegression/
 *
 * How to use
 *
 phoERegression myPhoERegr;
 myPhoERegr.initiliazeReaderEB(<pathXMLfileforEB>); // /afs/cern.ch/work/k/katatar/public/photonEnergyRegression/tmvaTrainRegr_BDTG_pbpb18_EB.xml
 myPhoERegr.initiliazeReaderEE(<pathXMLfileforEE>); // /afs/cern.ch/work/k/katatar/public/photonEnergyRegression/tmvaTrainRegr_BDTG_pbpb18_EE.xml
 myPhoERegr.getCorrectedPt(phoVars, phoEt, phoEta, phoSCEta);
 *
 * "phoVars" is a vector of photon variables where the order must be as below.
 * See item 2) above to get these variables into HiForest.
 *
 * phoVars[0] --> phoSCRawE
 * phoVars[1] --> phoSCEta
 * phoVars[2] --> phoSCPhi
 * phoVars[3] --> phoSCEtaWidth
 * phoVars[4] --> phoSCPhiWidth
 * phoVars[5] --> phoE3x3_2012
 * phoVars[6] --> phoMaxEnergyXtal_2012
 * phoVars[7] --> phoE2nd_2012
 * phoVars[8] --> phoELeft_2012
 * phoVars[9] --> phoERight_2012
 * phoVars[10] --> phoETop_2012
 * phoVars[11] --> phoEBottom_2012
 * phoVars[12] --> phoSigmaIEtaIEta_2012
 * phoVars[13] --> phoSigmaIEtaIPhi_2012
 * phoVars[14] --> phoSigmaIPhiIPhi_2012
 * phoVars[15] --> rho
 * phoVars[16] --> phoESEn (effective for EE only)
 *
 */

class phoERegression {
public :
    phoERegression() {

        tmvaReaders.clear();
        tmvaReaders.assign(2, 0);

        tmvaMethods = {"BDTG", "BDTG"};

        // EB
        readerBranches[0] = {"phoSCRawE",
                "phoSCEta",
                "phoSCPhi",
                "phoSCEtaWidth",
                "phoSCPhiWidth",
                "phoE3x3_2012",
                "phoMaxEnergyXtal_2012",
                "phoE2nd_2012",
                "(phoELeft_2012-phoERight_2012)/(phoELeft_2012+phoERight_2012)",
                "(phoETop_2012-phoEBottom_2012)/(phoETop_2012+phoEBottom_2012)",
                "phoSigmaIEtaIEta_2012",
                "phoSigmaIEtaIPhi_2012",
                "phoSigmaIPhiIPhi_2012",
                "rho"};

        // EB
        readerBranches[1] = {"phoSCRawE",
                "phoSCEta",
                "phoSCPhi",
                "phoSCEtaWidth",
                "phoSCPhiWidth",
                "phoE3x3_2012",
                "phoMaxEnergyXtal_2012",
                "phoE2nd_2012",
                "(phoELeft_2012-phoERight_2012)/(phoELeft_2012+phoERight_2012)",
                "(phoETop_2012-phoEBottom_2012)/(phoETop_2012+phoEBottom_2012)",
                "phoSigmaIEtaIEta_2012",
                "phoSigmaIEtaIPhi_2012",
                "phoSigmaIPhiIPhi_2012",
                "rho",
                "phoESEn"};


        spectatorBranches = {
                "phoEt",
                "phoEta",
                "phoPhi",
                "phoSCEta",
                "phoSCPhi",
                "phoHoverE",
                "pho_ecalClusterIsoR4",
                "pho_hcalRechitIsoR4",
                "pho_trackIsoR4PtCut20",
                "pho_genMatchedIndex",
                "hiBin",
                "rho",
                "phoE"
        };

        nReaderBranches.clear();
        nReaderBranches.resize(2);
        nReaderBranches[0] = readerBranches[0].size();
        nReaderBranches[1] = readerBranches[1].size();

        phoE = 0;
        phoEt = 0;
        phoEtCorrected = 0;
        phoEta = 0;
        phoPhi = 0;
    };
  ~phoERegression(){};
  int initiliazeReader(std::string pathXml, int iEBEE);
  int initiliazeReaderEB(std::string pathXmlEB);
  int initiliazeReaderEE(std::string pathXmlEE);
  void addVariables(int iEBEE);
  void addVariablesEB();
  void addVariablesEE();
  void addSpectators(int iEBEE);
  std::vector<float> evalRegression(std::vector<float> & phoVars, int iEBEE);
  float getCorrectedEnergy(std::vector<float> & phoVars, int iEBEE);
  float getCorrectedPtEBEE(std::vector<float> & phoVars, float phoEt, float phoEta, int iEBEE);
  float getCorrectedPt(std::vector<float> & phoVars, float phoEt, float phoEta, float phoSCEta);

  // Declaration of leaf types

  std::vector<TMVA::Reader*> tmvaReaders;
  //tmvaReaders[iXML] = new TMVA::Reader("!Color");

  std::vector<std::string> tmvaMethods;

  std::vector<std::string> readerBranches[2];
  std::vector<std::string> spectatorBranches;

  float varsR[2][20];
  float varsSpec[2][20];

  std::vector<int> nReaderBranches;

  Int_t           nPho;
  std::vector<float>   *phoE;
  std::vector<float>   *phoEt;
  std::vector<float>   *phoEtCorrected;
  std::vector<float>   *phoEtCorrected_sys;
  std::vector<float>   *phoEta;
  std::vector<float>   *phoPhi;
};

int phoERegression::initiliazeReader(std::string pathXml, int iEBEE)
{
    tmvaReaders[iEBEE] = 0;
    tmvaReaders[iEBEE] = new TMVA::Reader("!Color");

    addVariables(iEBEE);
    addSpectators(iEBEE);

    tmvaReaders[iEBEE]->BookMVA(tmvaMethods[iEBEE].c_str(), pathXml.c_str());

    return 0;
}

int phoERegression::initiliazeReaderEB(std::string pathXmlEB)
{
    return initiliazeReader(pathXmlEB, 0);
}

int phoERegression::initiliazeReaderEE(std::string pathXmlEE)
{
    return initiliazeReader(pathXmlEE, 1);
}

void phoERegression::addVariables(int iEBEE)
{
    for (int i = 0; i < nReaderBranches[iEBEE]; ++i) {

        tmvaReaders[iEBEE]->AddVariable(readerBranches[iEBEE][i].c_str(), &(varsR[iEBEE][i]));
    }
}

void phoERegression::addVariablesEB()
{
    addVariables(0);
}

void phoERegression::addVariablesEE()
{
    addVariables(1);
}

void phoERegression::addSpectators(int iEBEE)
{
    int nSpecs = spectatorBranches.size();

    for (int i = 0; i < nSpecs; ++i) {
        tmvaReaders[iEBEE]->AddSpectator(spectatorBranches[i].c_str(), &(varsSpec[iEBEE][i]));
    }
}

std::vector<float> phoERegression::evalRegression(std::vector<float> & phoVars, int iEBEE)
{
    varsR[iEBEE][0] = phoVars[0];   // phoSCRawE
    varsR[iEBEE][1] = phoVars[1];   // phoSCEta
    varsR[iEBEE][2] = phoVars[2];   // phoSCPhi
    varsR[iEBEE][3] = phoVars[3];   // phoSCEtaWidth
    varsR[iEBEE][4] = phoVars[4];   // phoSCPhiWidth
    varsR[iEBEE][5] = phoVars[5];   // phoE3x3_2012
    varsR[iEBEE][6] = phoVars[6];   // phoMaxEnergyXtal_2012
    varsR[iEBEE][7] = phoVars[7];   // phoE2nd_2012

    // phoELeft_2012 and phoERight_2012
    if (phoVars[8] != 0 || phoVars[9] != 0) {
        varsR[iEBEE][8] = (phoVars[8] - phoVars[9]) / (phoVars[8] + phoVars[9]);
    }
    else {
        varsR[iEBEE][8] = 0;
    }

    // phoETop_2012 and phoEBottom_2012
    if (phoVars[10] != 0 || phoVars[11] != 0) {
        varsR[iEBEE][9] = (phoVars[10] - phoVars[11]) / (phoVars[10] + phoVars[11]);
    }
    else {
        varsR[iEBEE][9] = 0;
    }

    varsR[iEBEE][10] = phoVars[12];   // phoSigmaIEtaIEta_2012
    varsR[iEBEE][11] = phoVars[13];   // phoSigmaIEtaIPhi_2012
    varsR[iEBEE][12] = phoVars[14];   // phoSigmaIPhiIPhi_2012
    varsR[iEBEE][13] = phoVars[15];   // rho

    if (iEBEE == 1) {
        varsR[iEBEE][14] = phoVars[16];   // phoESEn
    }

    std::vector<float> targets_regr = tmvaReaders[iEBEE]->EvaluateRegression(tmvaMethods[iEBEE].c_str());
    return targets_regr;
}

float phoERegression::getCorrectedEnergy(std::vector<float> & phoVars, int iEBEE)
{
    std::vector<float> targets_regr = evalRegression(phoVars, iEBEE);
    return targets_regr[0];
}

float phoERegression::getCorrectedPtEBEE(std::vector<float> & phoVars, float phoEt, float phoEta, int iEBEE)
{
    // if the relevant xml file is not initialized somehow, then return the original value
    if (tmvaReaders[iEBEE] == 0) {
        return phoEt;
    }

    double energy = getCorrectedEnergy(phoVars, iEBEE);
    if (phoEt > 25) {
        return (energy / TMath::CosH(phoEta));
    }
    else {
        return phoEt;
    }
}

float phoERegression::getCorrectedPt(std::vector<float> & phoVars, float phoEt, float phoEta, float phoSCEta)
{
    if (std::fabs(phoSCEta) < 1.48) {
        return getCorrectedPtEBEE(phoVars, phoEt, phoEta, 0);
    }
    else if (std::fabs(phoSCEta) > 1.57 && std::fabs(phoSCEta) < 2.4) {
        return getCorrectedPtEBEE(phoVars, phoEt, phoEta, 1);
    }
    else {
        return phoEt;
    }
}

#endif /* PHOEREGRESSION_H_ */
