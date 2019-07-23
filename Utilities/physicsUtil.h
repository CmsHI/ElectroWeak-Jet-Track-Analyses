/*
 * utilities related to physics.
 */

#include <TString.h>
#include <TMath.h>

#include <string>
#include <vector>
#include <iostream>

#ifndef PHYSICSUTIL_H_
#define PHYSICSUTIL_H_

const double mass_Z = 91.18;
const double mass_ele = 0.000511;

double getDETA(double eta1, double eta2);
double getDPHI(double phi1, double phi2);
double getAbsDPHI(double phi1, double phi2);
double getDR(double eta1, double phi1, double eta2, double phi2);
double getDR2(double eta1, double phi1, double eta2, double phi2);
double correctPhiRange(double phi);

double getDETA(double eta1, double eta2)
{
    return eta1 - eta2;
}

/*
 * TMath::Pi() is given here
 * https://root.cern.ch/doc/master/TMath_8h_source.html#l00044
 * inline Double_t Pi() { return 3.14159265358979323846; }
 */
double getDPHI(double phi1, double phi2)
{
    double dphi = phi1 - phi2;
    if (dphi > 3.14159265358979323846)
        dphi -= 2*3.14159265358979323846;
    if (dphi <= -1*3.14159265358979323846)
        dphi += 2*3.14159265358979323846;
    if (TMath::Abs(dphi) > 3.14159265358979323846) {
        std::cout << "Error in dphi calculation : |dphi| > PI" << std::endl;
        std::cout << "dphi is set to -999." << std::endl;
        return -999;
    }

    return dphi;
}

double getAbsDPHI(double phi1, double phi2)
{
    return TMath::Abs(getDPHI(phi1, phi2)) ;
}

double getDR(double eta1, double phi1, double eta2, double phi2)
{
    double deta = eta1-eta2;
    double dphi = getDPHI(phi1, phi2);
    return TMath::Sqrt(deta*deta + dphi*dphi);
}

double getDR2(double eta1, double phi1, double eta2, double phi2)
{
    double deta = eta1-eta2;
    double dphi = getDPHI(phi1, phi2);
    return (deta*deta + dphi*dphi);
}

double correctPhiRange(double phi){

    while (TMath::Abs(phi) > TMath::Pi())
    {
        if ( phi >    TMath::Pi() )  phi -= 2*TMath::Pi();
        if ( phi < -1*TMath::Pi() )  phi += 2*TMath::Pi();
    }
    return phi;
}

#endif /* PHYSICSUTIL_H_ */

