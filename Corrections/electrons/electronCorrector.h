/*
 * class related to electron energy correction
 */

#ifndef ELECTRONCORRECTOR_H_
#define ELECTRONCORRECTOR_H_

#include <TMVA/Reader.h>
#include <TMath.h>

#include <string>
#include <vector>

#include "../../TreeHeaders/ggHiNtuplizerTree.h"

class electronCorrector {
public :
    electronCorrector(){
        eleSCEta_EB = 1.4442;
    };
    ~electronCorrector(){};
    void initiliazeReader(std::string pathEB = "weights/BDTG_EB_PbPb.weights.xml", std::string pathEE = "weights/BDTG_EE_PbPb.weights.xml");
    float regressionTMVA(ggHiNtuplizer &tggHiNtuplizer, int i);
    void correctPt(ggHiNtuplizer &tggHiNtuplizer, int i);
    void correctPts(ggHiNtuplizer &tggHiNtuplizer);

    TMVA::Reader* tmvaReader[2];

    float eleSCEta;
    float eleSCPhi;
    float eleSigmaIEtaIEta_2012;
    float eleSigmaIPhiIPhi;
    float eleSCEtaWidth;
    float eleSCPhiWidth;
    float eleHoverE;

private :
    float eleSCEta_EB;
};

/*
 * function are derived from the following file :
 * https://github.com/doanhien/ZJet/blob/51371bbffb6c6bcde2bb19120c479c9e861ddd95/Electron_EnergyRegression/PbPb/EnergyRegression.h
 */
void electronCorrector::initiliazeReader(std::string pathEB, std::string pathEE)
{
    for (int iECAL = 0; iECAL < 2; iECAL++) {

      tmvaReader[iECAL] = new TMVA::Reader("!Color:Silent");

      // add classification variables, variable name have to be the same in weight files
      tmvaReader[iECAL]->AddVariable("EleSCPhi", &eleSCPhi);
      tmvaReader[iECAL]->AddVariable("EleSCEta", &eleSCEta);
      tmvaReader[iECAL]->AddVariable("EleSigmaIEtaIEta", &eleSigmaIEtaIEta_2012);
      tmvaReader[iECAL]->AddVariable("EleSigmaIPhiIPhi", &eleSigmaIPhiIPhi);
      tmvaReader[iECAL]->AddVariable("EleSCEtaWidth", &eleSCEtaWidth);
      tmvaReader[iECAL]->AddVariable("EleSCPhiWidth", &eleSCPhiWidth);
      tmvaReader[iECAL]->AddVariable("EleHoverE", &eleHoverE);

      //book method, read weight files
      if (iECAL == 0)
        tmvaReader[0]->BookMVA("BDTG", pathEB.c_str());
      else
        tmvaReader[1]->BookMVA("BDTG", pathEE.c_str());
    }
}

/*
 * to apply the correction multiply the return value with electron energy or electron pt
 * e.g. elePt *= ElectronRegressionTMVA(i)
 *
 * function are derived from the following file :
 * https://github.com/doanhien/ZJet/blob/51371bbffb6c6bcde2bb19120c479c9e861ddd95/Electron_EnergyRegression/PbPb/EnergyRegression.h
 */
float electronCorrector::regressionTMVA(ggHiNtuplizer &tggHiNtuplizer, int i)
{
    int iBE = (TMath::Abs(tggHiNtuplizer.eleSCEta->at(i)) < eleSCEta_EB) ? 0 : 1;

    // set variables
    eleSCPhi = tggHiNtuplizer.eleSCPhi->at(i);
    eleSCEta = tggHiNtuplizer.eleSCEta->at(i);
    eleSigmaIEtaIEta_2012 = tggHiNtuplizer.eleSigmaIEtaIEta_2012->at(i);
    eleSigmaIPhiIPhi = tggHiNtuplizer.eleSigmaIPhiIPhi->at(i);
    eleSCEtaWidth = tggHiNtuplizer.eleSCEtaWidth->at(i);
    eleSCPhiWidth = tggHiNtuplizer.eleSCPhiWidth->at(i);
    eleHoverE = tggHiNtuplizer.eleHoverE->at(i);

    // get regression
    return (tmvaReader[iBE]->EvaluateRegression("BDTG"))[0];
}

void electronCorrector::correctPt(ggHiNtuplizer &tggHiNtuplizer, int i)
{
    tggHiNtuplizer.elePt->at(i) *= regressionTMVA(tggHiNtuplizer, i);
}

/*
 * correct the pt of all electrons
 */
void electronCorrector::correctPts(ggHiNtuplizer &tggHiNtuplizer)
{
    for (int i = 0; i<tggHiNtuplizer.nEle; ++i) {
        tggHiNtuplizer.elePt->at(i) *= regressionTMVA(tggHiNtuplizer, i);
    }
}

#endif
