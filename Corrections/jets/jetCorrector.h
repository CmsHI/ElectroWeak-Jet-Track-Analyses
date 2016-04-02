/*
 * class related to jet energy correction
 */

#ifndef JETCORRECTOR_H_
#define JETCORRECTOR_H_

#include <TMath.h>
#include <TRandom3.h>

#include <string>
#include <vector>

#include "../../TreeHeaders/JetTree.h"

class jetCorrector {
public :
    jetCorrector(){
        // from AN-13-257, eqn.13
        eps0 = 0.9930;
        a0 = -0.63;
        a1 = 0.0546;
        // from https://github.com/CmsHI/gammaJetAnalysis/blob/a7df8217baa7238af6591e95d22d1a8c9041056d/photonSkimProducer/forest2yskim_jetSkim_forestV3.C#L602
        p0 = 1.04503;
        p1 = -1.6122;
        p2 = 9.27212;
        // from https://github.com/CmsHI/gammaJetAnalysis/blob/ee5ef885636a47a6a3d7f52dbdb9f20dc5f9208c/CutAndBinCollection2012.h
        CSN_HI.push_back(0.0363269);
        CSN_HI.push_back(1.3291);
        CSN_HI.push_back(6.49405);
        CSN_PP.push_back(0.056762);
        CSN_PP.push_back(0.808114);
        CSN_PP.push_back(0.000244992);

        CSN_phi_HI.push_back(0.0216325);
        CSN_phi_HI.push_back(0.342842);
        CSN_phi_HI.push_back(0.0002195);
        CSN_phi_PP.push_back(0.024497);
        CSN_phi_PP.push_back(-0.170472);
        CSN_phi_PP.push_back(-0.000188492);
    };
    ~jetCorrector(){};
    double getMatchingEfficiency(Jets &tJets, int i);
    double getMatchingEfficiencyCorrection(Jets &tJets, int i);
    double getResidualCorrection(Jets &tJets, int i);
    double getSmearingCorrection(Jets &tJets, int i);
    double getSmearingCorrectionPhi(Jets &tJets, int i);
    void  correctPtResidual(Jets &tJets, int i);
    void  correctPtsResidual(Jets &tJets);
    void  correctPtSmearing(Jets &tJets, int i);
    void  correctPtsSmearing(Jets &tJets);
    void  correctPhiSmearing(Jets &tJets, int i);
    void  correctPhisSmearing(Jets &tJets);
    void  applyEnergyScale(Jets &tJets, int i, double energyScale);
    void  applyEnergyScale(Jets &tJets, double energyScale);

    TRandom3 rand;  // used for smearing
    // matching efficiency function : eps = 1/2 * eps0 * (1 + erf(a0 + a1 * pt))
    double eps0;
    double a0;
    double a1;
    // residual correction function : corr = p0 + p1/sqrt(pt) + p2/pt
    double p0;
    double p1;
    double p2;
    // pp smearing : sigma_rel = sqrt( (C_HI^2 - C_PP^2) + (S_HI^2 - S_PP^2) / pt + (N_HI^2 - N_PP^2)/pt^2 )
    std::vector<double> CSN_HI;
    std::vector<double> CSN_PP;
    // pp smearing of phi
    std::vector<double> CSN_phi_HI;
    std::vector<double> CSN_phi_PP;

private :

};

double jetCorrector::getMatchingEfficiency(Jets &tJets, int i)
{
    double result = 0.5 * eps0 * (1 + TMath::Erf(a0 + a1 * tJets.jtpt[i]));
    return result;
}

double jetCorrector::getMatchingEfficiencyCorrection(Jets &tJets, int i)
{
    return 1./TMath::Sqrt(getMatchingEfficiency(tJets, i));
}

double jetCorrector::getResidualCorrection(Jets &tJets, int i)
{
    double result = 1./(p0 + p1/TMath::Sqrt(tJets.jtpt[i]) + p2/tJets.jtpt[i]);
    return result;
}

double jetCorrector::getSmearingCorrection(Jets &tJets, int i)
{
    double sigma_rel = TMath::Sqrt(
                        (CSN_HI.at(0)*CSN_HI.at(0) - CSN_PP.at(0)*CSN_PP.at(0)) +
                        (CSN_HI.at(1)*CSN_HI.at(1) - CSN_PP.at(1)*CSN_PP.at(1))/tJets.jtpt[i] +
                        (CSN_HI.at(2)*CSN_HI.at(2) - CSN_PP.at(2)*CSN_PP.at(2))/(tJets.jtpt[i]*tJets.jtpt[i])
                                );

    return rand.Gaus(1, sigma_rel);
}

double jetCorrector::getSmearingCorrectionPhi(Jets &tJets, int i)
{
    double sigma_rel = TMath::Sqrt(
                        (CSN_phi_HI.at(0)*CSN_phi_HI.at(0) - CSN_phi_PP.at(0)*CSN_phi_PP.at(0)) +
                        (CSN_phi_HI.at(1)*CSN_phi_HI.at(1) - CSN_phi_PP.at(1)*CSN_phi_PP.at(1))/tJets.jtpt[i] +
                        (CSN_phi_HI.at(2)*CSN_phi_HI.at(2) - CSN_phi_PP.at(2)*CSN_phi_PP.at(2))/(tJets.jtpt[i]*tJets.jtpt[i])
                                );

    return rand.Gaus(0, sigma_rel);
}

void jetCorrector::correctPtResidual(Jets &tJets, int i)
{
    tJets.jtpt[i] *= getResidualCorrection(tJets, i);
}

void jetCorrector::correctPtsResidual(Jets &tJets)
{
    for (int i = 0; i<tJets.nref; ++i) {
        correctPtResidual(tJets, i);
    }
}

void jetCorrector::correctPtSmearing(Jets &tJets, int i)
{
    tJets.jtpt[i] *= getSmearingCorrection(tJets, i);
}

void jetCorrector::correctPtsSmearing(Jets &tJets)
{
    for (int i = 0; i<tJets.nref; ++i) {
        correctPtSmearing(tJets, i);
    }
}

void jetCorrector::correctPhiSmearing(Jets &tJets, int i)
{
    tJets.jtphi[i] += getSmearingCorrectionPhi(tJets, i);

    while (TMath::Abs(tJets.jtphi[i]) > TMath::Pi())
    {
        if ( tJets.jtphi[i] >    TMath::Pi() )  tJets.jtphi[i] -= 2*TMath::Pi();
        if ( tJets.jtphi[i] < -1*TMath::Pi() )  tJets.jtphi[i] += 2*TMath::Pi();
    }
}

void jetCorrector::correctPhisSmearing(Jets &tJets)
{
    for (int i = 0; i<tJets.nref; ++i) {
        correctPhiSmearing(tJets, i);
    }
}

void  jetCorrector::applyEnergyScale(Jets &tJets, int i, double energyScale)
{
    tJets.jtpt[i] *= energyScale;
}

void  jetCorrector::applyEnergyScale(Jets &tJets, double energyScale)
{
    for (int i = 0; i<tJets.nref; ++i) {
        applyEnergyScale(tJets, i, energyScale);
    }
}

#endif
