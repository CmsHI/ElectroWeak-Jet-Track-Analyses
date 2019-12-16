/*
 * https://twiki.cern.ch/twiki/bin/viewauth/CMS/MuonReferenceEffs2017
 * https://twiki.cern.ch/twiki/pub/CMS/MuonReferenceEffs2017/RunF_data_ID.root
 * https://twiki.cern.ch/twiki/pub/CMS/MuonReferenceEffs2017/RunF_data_ISO.root
 */
#ifndef muons_2017pp_weight_h
#define muons_2017pp_weight_h

#include "TMath.h"
#include <iostream>

double weight_sf_id_pp2017(double pt, double eta, int type = 0);
double weight_sf_val_id_pp2017(double pt, double eta);
double weight_sf_unc_id_pp2017(double pt, double eta);

double weight_sf_id_pp2017(double pt, double eta, int type)
{
    double x = 1;

    if (type == 0) {

        if (pt >= 60 && pt < 120) {

            if (std::fabs(eta) < 0.9) { x = 0.961493; }
            else if (std::fabs(eta) < 1.2) { x = 0.957492; }
            else if (std::fabs(eta) < 2.1) { x = 0.978918; }
            else if (std::fabs(eta) < 2.4) { x = 0.936803; }

        }
        else if (pt >= 50) {

            if (std::fabs(eta) < 0.9) { x = 0.955283; }
            else if (std::fabs(eta) < 1.2) { x = 0.958409; }
            else if (std::fabs(eta) < 2.1) { x = 0.975811; }
            else if (std::fabs(eta) < 2.4) { x = 0.934953; }

        }
        else if (pt >= 40) {

            if (std::fabs(eta) < 0.9) { x = 0.957119; }
            else if (std::fabs(eta) < 1.2) { x = 0.957934; }
            else if (std::fabs(eta) < 2.1) { x = 0.974349; }
            else if (std::fabs(eta) < 2.4) { x = 0.942218; }

        }
        else if (pt >= 30) {

            if (std::fabs(eta) < 0.9) { x = 0.955489; }
            else if (std::fabs(eta) < 1.2) { x = 0.955264; }
            else if (std::fabs(eta) < 2.1) { x = 0.973445; }
            else if (std::fabs(eta) < 2.4) { x = 0.9403; }

        }
        else if (pt >= 25) {

            if (std::fabs(eta) < 0.9) { x = 0.9549; }
            else if (std::fabs(eta) < 1.2) { x = 0.952441; }
            else if (std::fabs(eta) < 2.1) { x = 0.968172; }
            else if (std::fabs(eta) < 2.4) { x = 0.942218; }

        }
        else if (pt >= 20) {

            if (std::fabs(eta) < 0.9) { x = 0.957958; }
            else if (std::fabs(eta) < 1.2) { x = 0.958386; }
            else if (std::fabs(eta) < 2.1) { x = 0.969121; }
            else if (std::fabs(eta) < 2.4) { x = 0.944519; }

        }
    }
    else if (type == 1) {

        if (pt >= 60 && pt < 120) {

            if (std::fabs(eta) < 0.9) { x = 1.00183161; }
            else if (std::fabs(eta) < 1.2) { x = 1.00388916; }
            else if (std::fabs(eta) < 2.1) { x = 1.00288075; }
            else if (std::fabs(eta) < 2.4) { x = 1.0107482; }

        }
        else if (pt >= 50) {

            if (std::fabs(eta) < 0.9) { x = 1.000827368; }
            else if (std::fabs(eta) < 1.2) { x = 1.00169185; }
            else if (std::fabs(eta) < 2.1) { x = 1.00115402; }
            else if (std::fabs(eta) < 2.4) { x = 1.00422523; }

        }
        else if (pt >= 40) {

            if (std::fabs(eta) < 0.9) { x = 1.000248809; }
            else if (std::fabs(eta) < 1.2) { x = 1.000357591; }
            else if (std::fabs(eta) < 2.1) { x = 1.000664353; }
            else if (std::fabs(eta) < 2.4) { x = 1.000911656; }

        }
        else if (pt >= 30) {

            if (std::fabs(eta) < 0.9) { x = 1.000423584; }
            else if (std::fabs(eta) < 1.2) { x = 1.000728648; }
            else if (std::fabs(eta) < 2.1) { x = 1.000428213; }
            else if (std::fabs(eta) < 2.4) { x = 1.00111307; }

        }
        else if (pt >= 25) {

            if (std::fabs(eta) < 0.9) { x = 1.00197158; }
            else if (std::fabs(eta) < 1.2) { x = 1.00245284; }
            else if (std::fabs(eta) < 2.1) { x = 1.000428213; }
            else if (std::fabs(eta) < 2.4) { x = 1.00388791; }

        }
        else if (pt >= 20) {

            if (std::fabs(eta) < 0.9) { x = 1.00450795; }
            else if (std::fabs(eta) < 1.2) { x = 1.00579795; }
            else if (std::fabs(eta) < 2.1) { x = 1.0; }
            else if (std::fabs(eta) < 2.4) { x = 1.00399166; }
        }
    }

    return x;
}

double weight_sf_val_id_pp2017(double pt, double eta)
{
    return weight_sf_id_pp2017(pt, eta, 0);
}

double weight_sf_unc_id_pp2017(double pt, double eta)
{
    return weight_sf_id_pp2017(pt, eta, 1);
}

#endif //#ifndef muons_2017pp_weight_h
