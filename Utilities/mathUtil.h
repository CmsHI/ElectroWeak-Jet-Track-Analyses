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
double fnc_DSCB(double* xx, double* params);

namespace MATHUTIL {

enum FNCS {
    kDSCB,
    kN_FNCS
};
const std::string FNCNAMES[kN_FNCS] = {
        "fnc_DSCB"
    };

fncPointer FNCPOINTERS[kN_FNCS] = {
        fnc_DSCB
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

#endif /* MATHUTIL_H_ */

