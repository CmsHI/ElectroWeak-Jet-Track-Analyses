/*
 * class related to jet energy correction
 */

#ifndef JETCORRECTOR_H_
#define JETCORRECTOR_H_

#include <TF1.h>
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
        sigma_rel_Var = 1;
        CSN_HI.push_back(0.0363269);
        CSN_HI.push_back(1.3291);
        CSN_HI.push_back(6.49405);
        CSN_PP.push_back(0.056762);
        CSN_PP.push_back(0.808114);
        CSN_PP.push_back(0.000244992);

        sigmaPhi_rel_Var = 1;
        CSN_phi_HI.push_back(0.0216325);
        CSN_phi_HI.push_back(0.342842);
        CSN_phi_HI.push_back(0.0002195);
        CSN_phi_PP.push_back(0.024497);
        CSN_phi_PP.push_back(-0.170472);
        CSN_phi_PP.push_back(-0.000188492);

        smearingBranchIndex = 0;
    };
    ~jetCorrector(){};
    double getMatchingEfficiency(Jets &tJets, int i);
    double getMatchingEfficiencyCorrection(Jets &tJets, int i);
    double getResidualCorrection(Jets &tJets, int i);
    double getResidualCorrection(TF1* f1, Jets &tJets, int i);
    double getSmearingPt(Jets &tJets, int i);
    double getSmearingPhi(Jets &tJets, int i);
    double getResolutionHI(Jets &tJets, int i);
    void  correctPtResidual(Jets &tJets, int i);
    void  correctPtResidual(TF1* f1, Jets &tJets, int i);
    void  correctPtsResidual(Jets &tJets);
    void  correctPtsResidual(TF1* f1, Jets &tJets);
    void  applyPtSmearing(Jets &tJets, int i);
    void  applyPtsSmearing(Jets &tJets);
    void  applyPhiSmearing(Jets &tJets, int i);
    void  applyPhisSmearing(Jets &tJets);
    void  applyEnergyScale(Jets &tJets, int i, double energyScale);
    void  applyEnergyScale(Jets &tJets, double energyScale);
    void  applyPtSmearingRelative(Jets &tJets, int i, double resolutionRel);
    void  applyPtsSmearingRelative(Jets &tJets, double resolutionRel);
    void  applyPtResolution(Jets &tJets, int i, double resolutionRel);
    void  applyPtsResolution(Jets &tJets, double resolutionRel);
    void  applyPhiResolution(Jets &tJets, int i, double resolutionRel);
    void  applyPhisResolution(Jets &tJets, double resolutionRel);

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
    double sigma_rel_Var;   // variation of simg_rel for systematics studies
    double sigmaPhi_rel_Var;   // variation of simg_rel for systematics studies
    std::vector<double> CSN_HI;
    std::vector<double> CSN_PP;
    // pp smearing of phi
    std::vector<double> CSN_phi_HI;
    std::vector<double> CSN_phi_PP;

    int smearingBranchIndex;

private :
    float CorrectPhiRange(float phi){
        while (TMath::Abs(phi) > TMath::Pi())
        {
            if ( phi >    TMath::Pi() )  phi -= 2*TMath::Pi();
            if ( phi < -1*TMath::Pi() )  phi += 2*TMath::Pi();
        }
        return phi;
    }
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

double jetCorrector::getResidualCorrection(TF1* f1, Jets &tJets, int i)
{
    double result = 1./f1->Eval(tJets.jtpt[i]);
    return result;
}

double jetCorrector::getSmearingPt(Jets &tJets, int i)
{
    double sigma_rel = TMath::Sqrt(
                        (CSN_HI.at(0)*CSN_HI.at(0) - CSN_PP.at(0)*CSN_PP.at(0)) +
                        (CSN_HI.at(1)*CSN_HI.at(1) - CSN_PP.at(1)*CSN_PP.at(1))/tJets.jtpt[i] +
                        (CSN_HI.at(2)*CSN_HI.at(2) - CSN_PP.at(2)*CSN_PP.at(2))/(tJets.jtpt[i]*tJets.jtpt[i])
                                );

    return sigma_rel_Var*rand.Gaus(1, sigma_rel);
}

double jetCorrector::getSmearingPhi(Jets &tJets, int i)
{
    double sigma_rel = TMath::Sqrt(
                        (CSN_phi_HI.at(0)*CSN_phi_HI.at(0) - CSN_phi_PP.at(0)*CSN_phi_PP.at(0)) +
                        (CSN_phi_HI.at(1)*CSN_phi_HI.at(1) - CSN_phi_PP.at(1)*CSN_phi_PP.at(1))/tJets.jtpt[i] +
                        (CSN_phi_HI.at(2)*CSN_phi_HI.at(2) - CSN_phi_PP.at(2)*CSN_phi_PP.at(2))/(tJets.jtpt[i]*tJets.jtpt[i])
                                );

    return sigmaPhi_rel_Var*rand.Gaus(0, sigma_rel);
}

double jetCorrector::getResolutionHI(Jets &tJets, int i)
{
    double sigma = TMath::Sqrt(
            (CSN_HI.at(0)*CSN_HI.at(0)) +
            (CSN_HI.at(1)*CSN_HI.at(1))/tJets.jtpt[i] +
            (CSN_HI.at(2)*CSN_HI.at(2))/(tJets.jtpt[i]*tJets.jtpt[i])
                    );

    return sigma;
}

void jetCorrector::correctPtResidual(Jets &tJets, int i)
{
    tJets.jtpt[i] *= getResidualCorrection(tJets, i);
}

void jetCorrector::correctPtResidual(TF1* f1, Jets &tJets, int i)
{
    tJets.jtpt[i] *= getResidualCorrection(f1, tJets, i);
}

void jetCorrector::correctPtsResidual(Jets &tJets)
{
    for (int i = 0; i<tJets.nref; ++i) {
        correctPtResidual(tJets, i);
    }
}

void jetCorrector::correctPtsResidual(TF1* f1, Jets &tJets)
{
    for (int i = 0; i<tJets.nref; ++i) {
        correctPtResidual(f1, tJets, i);
    }
}

void jetCorrector::applyPtSmearing(Jets &tJets, int i)
{
    if (tJets.jtpt[i] > 5)
    {
        switch(smearingBranchIndex) {
        case 0:
            tJets.jtpt[i] *= getSmearingPt(tJets, i);
            break;
        case 1:
            tJets.jtpt_smeared_0_30[i] = tJets.jtpt[i] * getSmearingPt(tJets, i);
            break;
        case 2:
            tJets.jtpt_smeared_30_100[i] = tJets.jtpt[i] * getSmearingPt(tJets, i);
            break;
        case 3:
            tJets.jtpt_smeared_0_10[i] = tJets.jtpt[i] * getSmearingPt(tJets, i);
            break;
        case 4:
            tJets.jtpt_smeared_10_30[i] = tJets.jtpt[i] * getSmearingPt(tJets, i);
            break;
        case 5:
            tJets.jtpt_smeared_30_50[i] = tJets.jtpt[i] * getSmearingPt(tJets, i);
            break;
        case 6:
            tJets.jtpt_smeared_50_100[i] = tJets.jtpt[i] * getSmearingPt(tJets, i);
            break;
        case 7:
            tJets.jtpt_smeared_sys[i] = tJets.jtpt[i] * getSmearingPt(tJets, i);
            break;
        default:
            std::cout << "smearingBranchIndex out of bounds, no smearing applied" << std::endl;
            break;
        }
    }
}

void jetCorrector::applyPtsSmearing(Jets &tJets)
{
    for (int i = 0; i<tJets.nref; ++i) {
        applyPtSmearing(tJets, i);
    }
}

void jetCorrector::applyPhiSmearing(Jets &tJets, int i)
{
    if (tJets.jtpt[i] > 5) {
        switch(smearingBranchIndex) {
        case 0:
            tJets.jtphi[i] = CorrectPhiRange(tJets.jtphi[i] + getSmearingPhi(tJets, i));
            break;
        case 1:
            tJets.jtphi_smeared_0_30[i] = CorrectPhiRange(tJets.jtphi[i] + getSmearingPhi(tJets, i));
            break;
        case 2:
            tJets.jtphi_smeared_30_100[i] = CorrectPhiRange(tJets.jtphi[i] + getSmearingPhi(tJets, i));
            break;
        case 3:
            tJets.jtphi_smeared_0_10[i] = CorrectPhiRange(tJets.jtphi[i] + getSmearingPhi(tJets, i));
            break;
        case 4:
            tJets.jtphi_smeared_10_30[i] = CorrectPhiRange(tJets.jtphi[i] + getSmearingPhi(tJets, i));
            break;
        case 5:
            tJets.jtphi_smeared_30_50[i] = CorrectPhiRange(tJets.jtphi[i] + getSmearingPhi(tJets, i));
            break;
        case 6:
            tJets.jtphi_smeared_50_100[i] = CorrectPhiRange(tJets.jtphi[i] + getSmearingPhi(tJets, i));
            break;
        case 7:
            tJets.jtphi_smeared_sys[i] = CorrectPhiRange(tJets.jtphi[i] + getSmearingPhi(tJets, i));
            break;          
        default:
            std::cout << "smearingBranchIndex out of bounds, no smearing applied" << std::endl;
            break;
        }
    }
}

void jetCorrector::applyPhisSmearing(Jets &tJets)
{
    for (int i = 0; i<tJets.nref; ++i) {
        applyPhiSmearing(tJets, i);
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

void  jetCorrector::applyPtSmearingRelative(Jets &tJets, int i, double resolutionRel)
{
    double resCSN = getResolutionHI(tJets, i);
    tJets.jtpt[i] *= rand.Gaus(1, resCSN*resolutionRel);
}

void  jetCorrector::applyPtsSmearingRelative(Jets &tJets, double resolutionRel)
{
    for (int i = 0; i<tJets.nref; ++i) {
        applyPtSmearingRelative(tJets, i, resolutionRel);
    }
}

void  jetCorrector::applyPtResolution(Jets &tJets, int i, double resolutionRel)
{
    if (tJets.jtpt[i] > 5) {
        tJets.jtpt[i] *= rand.Gaus(1, resolutionRel);
    }
}

void  jetCorrector::applyPtsResolution(Jets &tJets, double resolutionRel)
{
    for (int i = 0; i<tJets.nref; ++i) {
        applyPtResolution(tJets, i, resolutionRel);
    }
}

void  jetCorrector::applyPhiResolution(Jets &tJets, int i, double resolutionRel)
{
    if (tJets.jtpt[i] > 5) {
        tJets.jtphi[i] = CorrectPhiRange(tJets.jtphi[i] + rand.Gaus(0, resolutionRel));
    }
}

void  jetCorrector::applyPhisResolution(Jets &tJets, double resolutionRel)
{
    for (int i = 0; i<tJets.nref; ++i) {
        applyPhiResolution(tJets, i, resolutionRel);
    }
}

#endif
