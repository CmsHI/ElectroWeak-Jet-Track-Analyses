/*
 * class related to electron energy correction
 */

#ifndef ELECTRONCORRECTOR_H_
#define ELECTRONCORRECTOR_H_

#include <TMVA/Reader.h>
#include <TMath.h>

// for GBR
#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooConstVar.h>
#include <RooWorkspace.h>
//#include <../roofit/roofitcore/inc/RooRealVar.h>
//#include <../roofit/roofitcore/inc/RooDataSet.h>
//#include <../roofit/roofitcore/inc/RooConstVar.h>
//#include <../roofit/roofitcore/inc/RooWorkspace.h>
//#include "../../../GBRLikelihood/interface/RooDoubleCBFast.h"
//#include "../../../GBRLikelihood/interface/RooRealConstraint.h"
//#include "../../../GBRLikelihood/interface/RooGBRTargetFlex.h"
//#include "../../../GBRLikelihood/interface/RooGBRFunctionFlex.h"
//#include "../../../GBRLikelihood/interface/HybridGBRForestFlex.h"
//#include "../../../GBRLikelihood/interface/HybridGBRTreeD.h"

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
    void initRegressionGBR(std::string path = "weights/gbrmva_pp.root");
    float regressionTMVA(ggHiNtuplizer &tggHiNtuplizer, int i, int hiBin = 0);
    float regressionGBR(ggHiNtuplizer &tggHiNtuplizer, int i);
    void correctPtregressionTMVA(ggHiNtuplizer &tggHiNtuplizer, int i, int hiBin = 0);
    void correctPtsregressionTMVA(ggHiNtuplizer &tggHiNtuplizer, int hiBin = 0);
    void correctPtregressionGBR(ggHiNtuplizer &tggHiNtuplizer, int i);
    void correctPtsregressionGBR(ggHiNtuplizer &tggHiNtuplizer);
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
    float hiBin;

    //use GBR method for pp
    RooRealVar* invar[2][99]; // [EB or EE][varnum]
    RooAbsReal* mvaMean[2];
    RooAbsReal* mvaSigma[2];
    RooAbsReal* mvaAlphaL[2];
    RooAbsReal* mvaAlphaR[2];
    RooAbsReal* mvaPowerL[2];
    RooAbsReal* mvaPowerR[2];

private :
    float eleSCEta_EB;
};

/*
 * functions are derived from the following file :
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
      tmvaReader[iECAL]->AddVariable("hiBin",  &hiBin);

      //book method, read weight files
      if (iECAL == 0)
        tmvaReader[0]->BookMVA("BDTG", pathEB.c_str());
      else
        tmvaReader[1]->BookMVA("BDTG", pathEE.c_str());
    }
}

/*
 * functions are derived from the following file :
 * https://github.com/doanhien/ZJet/blob/2baa8c515188ac2ab7b36edb71200a46cebcf8f9/Electron_EnergyRegression/EnergyRegression_pp.h
 *
 * instructions on how to run electron regression for pp :
 * https://github.com/doanhien/ZJet/blob/08a3a662da42b15de8184864f28c71a3f132d9f8/Electron_EnergyRegression/README.md
 *
Since GBR method is used for energy regression in pp, we need to checkout this package first

git clone https://github.com/bendavid/GBRLikelihood.git HiggsAnalysis/GBRLikelihood

and then put the file of rootlogon.C in the directory you are running
 *
 */
void electronCorrector::initRegressionGBR(std::string path)
{
  TFile file(path.c_str());

  if (file.IsZombie()) {
      std::cout << "could not open file " << path.c_str() << std::endl;
      std::cout << "exiting" << std::endl;
      return;
  }

  for (int iBE = 0; iBE < 2; iBE++) { // barrel vs endcaps
    std::string det = (iBE == 0 ? "EB" : "EE");

    std::string wsname = Form("ws_mva_%s", det.c_str());

    RooWorkspace* ws = dynamic_cast<RooWorkspace*>(file.Get(wsname.c_str()));
    if (!ws) {
        std::cout << "could not open work space " << wsname.c_str() << std::endl;
        std::cout << "exiting" << std::endl;
        return;
    }

    invar[iBE][1] = ws->var("eleSCEta");
    invar[iBE][2] = ws->var("eleSCPhi");
    invar[iBE][3] = ws->var("eleSigmaIEtaIEta_2012");
    invar[iBE][4] = ws->var("eleSigmaIPhiIPhi");
    invar[iBE][5] = ws->var("eleSCEtaWidth");
    invar[iBE][6] = ws->var("eleSCPhiWidth");
    invar[iBE][7] = ws->var("eleHoverE");
    invar[iBE][8] = ws->var("eleR9");
    invar[iBE][9] = ws->var("eleE3x3");
    invar[iBE][10] = ws->var("eleE5x5");
    invar[iBE][11] = ws->var("eleR9Full5x5");
    invar[iBE][12] = ws->var("eleE3x3Full5x5");
    invar[iBE][13] = ws->var("eleE5x5Full5x5");
    invar[iBE][15] = ws->var("eleSeedEnToRawEn");
    invar[iBE][16] = ws->var("eleSeedEta");
    invar[iBE][17] = ws->var("eleSeedPhi");

    mvaMean[iBE] = ws->function("limMean");
    mvaSigma[iBE] = ws->function("limSigma");
    mvaAlphaL[iBE] = ws->function("limAlphaL");
    mvaAlphaR[iBE] = ws->function("limAlphaR");
    mvaPowerL[iBE] = ws->function("limPowerL");
    mvaPowerR[iBE] = ws->function("limPowerR");
  }
}

/*
 * to apply the correction multiply the return value with electron energy or electron pt
 * e.g. elePt *= regressionTMVA(i)
 *
 * functions are derived from the following file :
 * https://github.com/doanhien/ZJet/blob/882c2c8f513ffe7f0840abe93dec4c55327fda64/Electron_EnergyRegression/EnergyRegression_PbPb.h
 *
 * deprecated :
 * https://github.com/doanhien/ZJet/blob/51371bbffb6c6bcde2bb19120c479c9e861ddd95/Electron_EnergyRegression/PbPb/EnergyRegression.h
 */
float electronCorrector::regressionTMVA(ggHiNtuplizer &tggHiNtuplizer, int i, int hiBin)
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
    this->hiBin = (float)hiBin;

    // get regression
    return (tmvaReader[iBE]->EvaluateRegression("BDTG"))[0];
}

/*
 * to apply the correction multiply the return value with electron energy or electron pt
 * e.g. elePt *= regressionGBR(i)
 *
 * functions are derived from the following file :
 * https://github.com/doanhien/ZJet/blob/2baa8c515188ac2ab7b36edb71200a46cebcf8f9/Electron_EnergyRegression/EnergyRegression_pp.h
 *
 */
float electronCorrector::regressionGBR(ggHiNtuplizer &tggHiNtuplizer, int i) {

  float mean;

  int iBE = (TMath::Abs(tggHiNtuplizer.eleSCEta->at(i)) < eleSCEta_EB) ? 0 : 1;

  *invar[iBE][1] = tggHiNtuplizer.eleSCEta->at(i);
  *invar[iBE][2] = tggHiNtuplizer.eleSigmaIEtaIEta_2012->at(i);
  *invar[iBE][3] = tggHiNtuplizer.eleSCPhi->at(i);
  *invar[iBE][4] = tggHiNtuplizer.eleSigmaIPhiIPhi->at(i);
  *invar[iBE][5] = tggHiNtuplizer.eleSCEtaWidth->at(i);
  *invar[iBE][6] = tggHiNtuplizer.eleSCPhiWidth->at(i);
  *invar[iBE][7] = tggHiNtuplizer.eleHoverE->at(i);
  *invar[iBE][8] = tggHiNtuplizer.eleR9->at(i);
  *invar[iBE][9] = tggHiNtuplizer.eleE3x3->at(i);
  *invar[iBE][10] = tggHiNtuplizer.eleE5x5->at(i);
  *invar[iBE][11] = tggHiNtuplizer.eleR9Full5x5->at(i);
  *invar[iBE][12] = tggHiNtuplizer.eleE3x3Full5x5->at(i);
  *invar[iBE][13] = tggHiNtuplizer.eleE5x5Full5x5->at(i);
  *invar[iBE][15] = tggHiNtuplizer.eleSeedEn->at(i)/tggHiNtuplizer.eleSCRawEn->at(i);
  *invar[iBE][16] = tggHiNtuplizer.eleSeedEta->at(i);
  *invar[iBE][17] = tggHiNtuplizer.eleSeedPhi->at(i);

  mean = mvaMean[iBE]->getVal();

  return mean;
}

void electronCorrector::correctPtregressionTMVA(ggHiNtuplizer &tggHiNtuplizer, int i, int hiBin)
{
    tggHiNtuplizer.elePt->at(i) *= regressionTMVA(tggHiNtuplizer, i, hiBin);
}

/*
 * correct the pt of all electrons
 */
void electronCorrector::correctPtsregressionTMVA(ggHiNtuplizer &tggHiNtuplizer, int hiBin)
{
    for (int i = 0; i<tggHiNtuplizer.nEle; ++i) {
        correctPtregressionTMVA(tggHiNtuplizer, i, hiBin);
    }
}

void electronCorrector::correctPtregressionGBR(ggHiNtuplizer &tggHiNtuplizer, int i)
{
    tggHiNtuplizer.elePt->at(i) *= regressionGBR(tggHiNtuplizer, i);
}

/*
 * correct the pt of all electrons
 */
void electronCorrector::correctPtsregressionGBR(ggHiNtuplizer &tggHiNtuplizer)
{
    for (int i = 0; i<tggHiNtuplizer.nEle; ++i) {
        correctPtregressionGBR(tggHiNtuplizer, i);
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
