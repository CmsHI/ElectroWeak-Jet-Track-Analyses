/*
 * utilities related to math.
 */

#include <TString.h>
#include <TMath.h>

#include <string>
#include <vector>
#include <iostream>

#include "physicsUtil.h"

#ifndef MATHUTIL_H_
#define MATHUTIL_H_

typedef double (*fncPointer)(double *, double *);

fncPointer getFncPointer(std::string fncName);
int getFncNpar(std::string fncName);
int getFncNpar(fncPointer fncPtr);
double fnc_DSCB(double* xx, double* params);
double fnc_energyCorr_eta(double* xx, double* params);
double fnc_energyCorr_et(double* xx, double* params);
double fnc_energyCorr_bremEta(double* xx, double* params);
double fnc_fourier_vn_2(double* xx, double* params);
double fnc_fourier_vn_3(double* xx, double* params);

namespace MATHUTIL {

enum FNCS {
    k_DSCB,
    k_energyCorr_eta,
    k_energyCorr_et,
    k_energyCorr_bremEta,
    k_fourier_vn_2,
    k_fourier_vn_3,
    kN_FNCS
};
const std::string FNCNAMES[kN_FNCS] = {
        "fnc_DSCB",
        "fnc_energyCorr_eta",
        "fnc_energyCorr_et",
        "fnc_energyCorr_bremEta",
        "fnc_fourier_vn_2",
        "fnc_fourier_vn_3"
    };

fncPointer FNCPOINTERS[kN_FNCS] = {
        fnc_DSCB,
        fnc_energyCorr_eta,
        fnc_energyCorr_et,
        fnc_energyCorr_bremEta,
        fnc_fourier_vn_2,
        fnc_fourier_vn_3,
    };

/*
 * number of parameters for a function
 */
int FNCNPARS[kN_FNCS] = {
        7,
        2,
        5,
        85,
        3,
        5
    };
}

fncPointer getFncPointer(std::string fncName)
{
    for (int i = 0; i < MATHUTIL::kN_FNCS; ++i) {
        if (fncName == MATHUTIL::FNCNAMES[i]){
            return MATHUTIL::FNCPOINTERS[i];
        }
    }

    return 0;
}

int getFncNpar(std::string fncName)
{
    return getFncNpar(getFncPointer(fncName));
}

/*
 * returns the number of parameters for the given function
 */
int getFncNpar(fncPointer fncPtr)
{
    for (int i = 0; i < MATHUTIL::kN_FNCS; ++i) {
        if (fncPtr == MATHUTIL::FNCPOINTERS[i]){
            return MATHUTIL::FNCNPARS[i];
        }
    }

    return 0;
}

/*
 * Double sided crystal ball function
 * The default crystal ball function has its tail to the left of Gaussian core.
 * For details of the crystal function see : https://en.wikipedia.org/wiki/Crystal_Ball_function
 */
double fnc_DSCB(double* xx, double* params)
{
    double x   = xx[0];
    // gaussian core
    double N     = params[0];
    double mean  = params[1];
    double sigma = params[2];
    // transition parameters
    double a1  = params[3];
    double n1  = params[4];
    double a2  = params[5];
    double n2  = params[6];

    double u   = (x-mean)/sigma;
    double A1  = TMath::Power(n1/TMath::Abs(a1),n1)*TMath::Exp(-a1*a1/2);
    double A2  = TMath::Power(n2/TMath::Abs(a2),n2)*TMath::Exp(-a2*a2/2);
    double B1  = n1/TMath::Abs(a1) - TMath::Abs(a1);
    double B2  = n2/TMath::Abs(a2) - TMath::Abs(a2);

    double res = N;
    if      (u < -a1) res *= A1*TMath::Power(B1-u,-n1);
    else if (u < a2)  res *= TMath::Exp(-u*u/2);
    else              res *= A2*TMath::Power(B2+u,-n2);
    return res;
}

/*
 * ECAL energy correction as fnc of eta
 * https://github.com/cms-sw/cmssw/blob/CMSSW_10_3_X/RecoEcal/EgammaCoreTools/plugins/EcalClusterEnergyCorrectionObjectSpecific.cc#L8
 * https://github.com/cms-sw/cmssw/blob/5d2ce01256b930ecac84b9df357f26ce907e8b9e/RecoEcal/EgammaCoreTools/plugins/EcalClusterEnergyCorrectionObjectSpecific.cc#L8
 *
 * Current values
 * params[0] = 40.2198
 * params[1] = -0.00000303103
 */
double fnc_energyCorr_eta(double* xx, double* params)
{
    double eta = xx[0];
    double ieta = TMath::Abs(eta)*(5/0.087);

    if (ieta < params[0]) {
        return 1;
    }
    else {
        return 1/(1.0+params[1]*(ieta-params[0])*(ieta-params[0]));
    }
}

/*
 * ECAL energy correction as fnc of eT for photons in EB
 * https://github.com/cms-sw/cmssw/blob/CMSSW_10_3_X/RecoEcal/EgammaCoreTools/plugins/EcalClusterEnergyCorrectionObjectSpecific.cc#L320
 * https://github.com/cms-sw/cmssw/blob/5d2ce01256b930ecac84b9df357f26ce907e8b9e/RecoEcal/EgammaCoreTools/plugins/EcalClusterEnergyCorrectionObjectSpecific.cc#L320
 *
 * Current values
 * params[0] = 1
 * params[1] = 1.00303
 * params[2] = 1.00061
 * params[3] = -0.00000520914
 * params[4] = 0.999906
 */
double fnc_energyCorr_et(double* xx, double* params)
{
    double eT = xx[0];

    if (             eT <   5) return         1;
    if (  5 <= eT && eT <  10) return         params[0] ;
    if ( 10 <= eT && eT <  20) return         params[1] ;
    if ( 20 <= eT && eT < 140) return         params[2] + params[3]*eT ;
    if (140 <= eT            ) return         params[4];

    return 1;
}

/*
 * ECAL energy correction as fnc of brems (SC phi width / SC eta width) and eta for photons in EB
 * https://github.com/cms-sw/cmssw/blob/CMSSW_10_3_X/RecoEcal/EgammaCoreTools/plugins/EcalClusterEnergyCorrectionObjectSpecific.cc#L29
 * https://github.com/cms-sw/cmssw/blob/5d2ce01256b930ecac84b9df357f26ce907e8b9e/RecoEcal/EgammaCoreTools/plugins/EcalClusterEnergyCorrectionObjectSpecific.cc#L29
 *
 * Current values
 *
xcorr[0] = 1.00911
par0[0] = 0.000768052
par1[0] = 2.04651
par2[0] = -0.00532509
par3[0] = 0.987668
par4[0] = 17.7301
xcorr[1] = 1.01103
par0[1] = 0.00331551
par1[1] = 1.42944
par2[1] = -0.00132638
par3[1] = 0.99663
par4[1] = 11.7288
xcorr[2] = 1.00755
par0[2] = 0.00299318
par1[2] = 1.58006
par2[2] = -0.00119715
par3[2] = 0.992485
par4[2] = 13.6014
xcorr[3] = 1.00755
par0[3] = 0.00167598
par1[3] = 1.32833
par2[3] = -0.000670924
par3[3] = 0.989889
par4[3] = 13.3696
xcorr[4] = 1.00755
par0[4] = 0.000790722
par1[4] = 1.35829
par2[4] = -0.0138033
par3[4] = 0.980184
par4[4] = 11.1678
xcorr[5] = 1.00755
par0[5] = 0.13577
par1[5] = 0.00373831
par2[5] = -0.0543079
par3[5] = 1.00842
par4[5] = 0.911564
xcorr[6] = 1.00755
par0[6] = 0.144223
par1[6] = 2.06662
par2[6] = -0.0563825
par3[6] = 0.990502
par4[6] = 2.29549
xcorr[7] = 0.973756
par0[7] = 0.187812
par1[7] = 2.79467
par2[7] = -0.0751247
par3[7] = 0.960841
par4[7] = 2.87279
xcorr[8] = 0.979872
par0[8] = 0.887431
par1[8] = 2.64958
par2[8] = -0.0225306
par3[8] = 0.960174
par4[8] = 2.70598
xcorr[9] = 0.981161
par0[9] = 0.294117
par1[9] = 3.44856
par2[9] = -0.117637
par3[9] = 0.953106
par4[9] = 2.83931
xcorr[10] = 0.981023
par0[10] = 0.276291
par1[10] = 3.08719
par2[10] = -0.110498
par3[10] = 0.949254
par4[10] = 3.07739
xcorr[11] = 0.981028
par0[11] = 0.219354
par1[11] = 2.12927
par2[11] = -0.0877415
par3[11] = 0.937027
par4[11] = 3.43749
xcorr[12] = 0.979634
par0[12] = 0.226169
par1[12] = 2.13503
par2[12] = -0.0904675
par3[12] = 0.969127
par4[12] = 2.17677
xcorr[13] = 0.979634
par0[13] = 0.0767101
par1[13] = 1.45445
par2[13] = -0.030684
par3[13] = 0.887661
par4[13] = 6.30074
 *
 */
double fnc_energyCorr_bremEta(double* xx, double* params)
{
    double brem = xx[0];
    double eta = params[84];

    float etaCrackMin = 1.44;
    float etaCrackMax = 1.56;

    const int nBinsEta         = 14;
    float  leftEta  [nBinsEta] = { 0.02, 0.25, 0.46, 0.81, 0.91, 1.01, 1.16,           etaCrackMax,  1.653,  1.8, 2.0, 2.2, 2.3, 2.4 };
    float  rightEta [nBinsEta] = { 0.25, 0.42, 0.77, 0.91, 1.01, 1.13, etaCrackMin,    1.653,        1.8  ,  2.0, 2.2, 2.3, 2.4, 2.5 };

    float xcorr[nBinsEta];

    float par0[nBinsEta];
    float par1[nBinsEta];
    float par2[nBinsEta];
    float par3[nBinsEta];
    float par4[nBinsEta];

    float sigmaPhiSigmaEtaMin = 0.8;
    float sigmaPhiSigmaEtaMax = 5.;

    float sigmaPhiSigmaEtaFit = -1;

    // extra protections
    // fix sigmaPhiSigmaEta boundaries
    if (brem < sigmaPhiSigmaEtaMin)  brem = sigmaPhiSigmaEtaMin;
    if (brem > sigmaPhiSigmaEtaMax  )  brem = sigmaPhiSigmaEtaMax;

    // eta = 0
    if (TMath::Abs(eta)  <  leftEta[0]            ) { eta = 0.02 ; }
    // outside acceptance
    if (TMath::Abs(eta)  >=  rightEta[nBinsEta-1] ) { eta = 2.49; } //if (DBG) std::cout << " WARNING [applyScCorrections]: TMath::Abs(eta)  >=  rightEta[nBinsEta-1] " << std::endl;}

    int tmpEta = -1;
    for (int iEta = 0; iEta < nBinsEta; ++iEta){
      if ( leftEta[iEta] <= TMath::Abs(eta) && TMath::Abs(eta) <rightEta[iEta] ){
        tmpEta = iEta;
      }
    }

    for (int iEta = 0; iEta < nBinsEta; ++iEta){
        xcorr[iEta] = params[iEta * 6];
        par0[iEta] = params[iEta * 6 + 1];
        par1[iEta] = params[iEta * 6 + 2];
        par2[iEta] = params[iEta * 6 + 3];
        par3[iEta] = params[iEta * 6 + 4];
        par4[iEta] = params[iEta * 6 + 5];
    }

      sigmaPhiSigmaEtaFit = 1.;

    // Interpolation
    float tmpInter = 1;
    // In eta cracks/gaps
    if (tmpEta == -1 ) { // need to interpolate
      for (int iEta = 0; iEta < nBinsEta-1; ++iEta){
        if (rightEta[iEta] <= TMath::Abs(eta) && TMath::Abs(eta) <leftEta[iEta+1] ){
      if (brem >= sigmaPhiSigmaEtaFit)  {
          /*
        if (algorithm==0){ //electron
          tmpInter = ( par0[iEta] + brem*par1[iEta] + brem*brem*par2[iEta] +
               par0[iEta+1] + brem*par1[iEta+1] + brem*brem*par2[iEta+1]) / 2. ;
        }
        */
        //if (algorithm==1){ //photon
          tmpInter = (par0[iEta]*(1.-exp(-(brem-par4[iEta])/par1[iEta]))*par2[iEta]*brem + par3[iEta]+
                par0[iEta+1]*(1.-exp(-(brem-par4[iEta+1])/par1[iEta+1]))*par2[iEta+1]*brem + par3[iEta+1] ) /2.;
        //}
      }
      else tmpInter = (xcorr[iEta] + xcorr[iEta+1])/2.;
        }
      }
      return tmpInter;
    }

    if (brem >= sigmaPhiSigmaEtaFit) {
      //if (algorithm==0) return par0[tmpEta] + brem*par1[tmpEta] + brem*brem*par2[tmpEta];
      //if (algorithm==1) return par0[tmpEta]*(1.-exp(-(brem-par4[tmpEta])/par1[tmpEta]))*par2[tmpEta]*brem + par3[tmpEta];
        return par0[tmpEta]*(1.-exp(-(brem-par4[tmpEta])/par1[tmpEta]))*par2[tmpEta]*brem + par3[tmpEta];
    }
    else return xcorr[tmpEta];

  return 1;
}


/*
 * Fourier expansion of the particle density, as function of phi up to 2nd order
 */
double fnc_fourier_vn_2(double* xx, double* params)
{
    double phi = xx[0];

    double N0 = params[0];
    double v2 = params[1];
    double phiEPv2 = params[2];

    double res = N0 * (1 + 2*v2*TMath::Cos( 2*getDPHI(phi, phiEPv2) ));
    return res;
}

/*
 * Fourier expansion of the particle density, as function of phi up to 3rd order
 */
double fnc_fourier_vn_3(double* xx, double* params)
{
    double phi = xx[0];

    double N0 = params[0];
    double v2 = params[1];
    double phiEPv2 = params[2];
    double v3 = params[3];
    double phiEPv3 = params[4];

    double res = N0 * (1 + 2*v2*TMath::Cos( 2*getDPHI(phi, phiEPv2) ) + 2*v3*TMath::Cos( 3*getDPHI(phi, phiEPv3) ));
    return res;
}

#endif /* MATHUTIL_H_ */

