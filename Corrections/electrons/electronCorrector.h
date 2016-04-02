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
    void initiliazeReader(std::string pathEB = "weights/BDTG_EB_PbPb_16V.weights.xml", std::string pathEE = "weights/BDTG_EE_PbPb_16V.weights.xml");
    float regressionTMVA(ggHiNtuplizer &tggHiNtuplizer, int i);
    void correctPt(ggHiNtuplizer &tggHiNtuplizer, int i);
    void correctPts(ggHiNtuplizer &tggHiNtuplizer);
    void applyEnergyScale(ggHiNtuplizer &tggHiNtuplizer, int i, double energyScale);
    void applyEnergyScale(ggHiNtuplizer &tggHiNtuplizer, double energyScale);

    TMVA::Reader* tmvaReader[2];

    float eleSCEta;
    float eleSCPhi;
    float eleSigmaIEtaIEta_2012;
    float eleSigmaIPhiIPhi;
    float eleSCEtaWidth;
    float eleSCPhiWidth;
    float eleHoverE;
    float eleR9;
    float eleE3x3;
    float eleE5x5;
    float eleR9Full5x5;
    float eleE3x3Full5x5;
    float eleE5x5Full5x5;
    float eleSeedEnToRawEn;
    float eleSeedEta;
    float eleSeedPhi;

private :
    float eleSCEta_EB;
};

/*
 * function are derived from the following file :
 * https://github.com/doanhien/ZJet/blob/882c2c8f513ffe7f0840abe93dec4c55327fda64/Electron_EnergyRegression/EnergyRegression_PbPb.h
 *
 * deprecated :
 * https://github.com/doanhien/ZJet/blob/51371bbffb6c6bcde2bb19120c479c9e861ddd95/Electron_EnergyRegression/PbPb/EnergyRegression.h
 */
void electronCorrector::initiliazeReader(std::string pathEB, std::string pathEE)
{
    for (int iECAL = 0; iECAL < 2; iECAL++) {

      tmvaReader[iECAL] = new TMVA::Reader("!Color:Silent");

      // add classification variables, variable name have to be the same in weight files
      tmvaReader[iECAL]->AddVariable("eleSCPhi", &eleSCPhi);
      tmvaReader[iECAL]->AddVariable("eleSCEta", &eleSCEta);
      tmvaReader[iECAL]->AddVariable("eleSigmaIEtaIEta_2012", &eleSigmaIEtaIEta_2012);
      tmvaReader[iECAL]->AddVariable("eleSigmaIPhiIPhi", &eleSigmaIPhiIPhi);
      tmvaReader[iECAL]->AddVariable("eleSCEtaWidth", &eleSCEtaWidth);
      tmvaReader[iECAL]->AddVariable("eleSCPhiWidth", &eleSCPhiWidth);
      tmvaReader[iECAL]->AddVariable("eleHoverE", &eleHoverE);
      tmvaReader[iECAL]->AddVariable("eleR9", &eleR9);
      tmvaReader[iECAL]->AddVariable("eleE3x3", &eleE3x3);
      tmvaReader[iECAL]->AddVariable("eleE5x5", &eleE5x5);
      tmvaReader[iECAL]->AddVariable("eleR9Full5x5", &eleR9Full5x5);
      tmvaReader[iECAL]->AddVariable("eleE3x3Full5x5", &eleE3x3Full5x5);
      tmvaReader[iECAL]->AddVariable("eleE5x5Full5x5", &eleE5x5Full5x5);
      tmvaReader[iECAL]->AddVariable("eleSeedEn/eleSCRawEn", &eleSeedEnToRawEn);
      tmvaReader[iECAL]->AddVariable("eleSeedEta",  &eleSeedEta);
      tmvaReader[iECAL]->AddVariable("eleSeedPhi",  &eleSeedPhi);

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
 * https://github.com/doanhien/ZJet/blob/882c2c8f513ffe7f0840abe93dec4c55327fda64/Electron_EnergyRegression/EnergyRegression_PbPb.h
 *
 * deprecated :
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
    eleR9 = tggHiNtuplizer.eleR9->at(i);
    eleE3x3 = tggHiNtuplizer.eleE3x3->at(i);
    eleE5x5 = tggHiNtuplizer.eleE5x5->at(i);
    eleR9Full5x5 = tggHiNtuplizer.eleR9Full5x5->at(i);
    eleE3x3Full5x5 = tggHiNtuplizer.eleE3x3Full5x5->at(i);
    eleE5x5Full5x5 = tggHiNtuplizer.eleE5x5Full5x5->at(i);
    eleSeedEnToRawEn = tggHiNtuplizer.eleSeedEn->at(i)/tggHiNtuplizer.eleSCRawEn->at(i);
    eleSeedEta = tggHiNtuplizer.eleSeedEta->at(i);
    eleSeedPhi = tggHiNtuplizer.eleSeedPhi->at(i);

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

void electronCorrector::applyEnergyScale(ggHiNtuplizer &tggHiNtuplizer, int i, double energyScale)
{
    tggHiNtuplizer.elePt->at(i) *= energyScale;
}

void electronCorrector::applyEnergyScale(ggHiNtuplizer &tggHiNtuplizer, double energyScale)
{
    for (int i = 0; i<tggHiNtuplizer.nEle; ++i) {
        applyEnergyScale(tggHiNtuplizer, i, energyScale);
    }
}

#endif
