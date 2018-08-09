/*
 * utilities related to math.
 */

#include <TString.h>
#include <TMath.h>

#include <string>
#include <vector>
#include <iostream>

#ifndef MATHUTIL_H_
#define MATHUTIL_H_

typedef double (*fncPointer)(double *, double *);

fncPointer getFncPointer(std::string fncName);
int getFncNpar(std::string fncName);
int getFncNpar(fncPointer fncPtr);
double fnc_DSCB(double* xx, double* params);
double fnc_energyCorr_eta(double* xx, double* params);
double fnc_energyCorr_et(double* xx, double* params);

namespace MATHUTIL {

enum FNCS {
    k_DSCB,
    k_energyCorr_eta,
    k_energyCorr_et,
    kN_FNCS
};
const std::string FNCNAMES[kN_FNCS] = {
        "fnc_DSCB",
        "fnc_energyCorr_eta",
        "fnc_energyCorr_et"
    };

fncPointer FNCPOINTERS[kN_FNCS] = {
        fnc_DSCB,
        fnc_energyCorr_eta,
        fnc_energyCorr_et
    };

/*
 * number of parameters for a function
 */
int FNCNPARS[kN_FNCS] = {
        7,
        2,
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

#endif /* MATHUTIL_H_ */

