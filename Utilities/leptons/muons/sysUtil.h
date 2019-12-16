/*
 * utilities related to muon uncertainties.
 */
#ifndef LEPTONS_MUONS_SYSUTIL_H_
#define LEPTONS_MUONS_SYSUTIL_H_

#include <TMath.h>

#include <string>
#include <vector>
#include <iostream>

#include "2017pp/weight.h"
#include "2018PbPb/tnp_weight.h"

namespace MUONTNP {

enum UNC_LABELS {

   k_pp17_stat_id_p = 1711,
   k_pp17_stat_id_m = 1712,

   k_pbpb18_stat_id_p = 1811,
   k_pbpb18_stat_id_m = 1812,
   k_pbpb18_stat_trk_p = 1821,
   k_pbpb18_stat_trk_m = 1822,
   k_pbpb18_stat_trig_p = 1831,
   k_pbpb18_stat_trig_m = 1832,
   k_pbpb18_sys_id_p = 18511,
   k_pbpb18_sys_id_m = 18512,
   k_pbpb18_sys_trk_p = 18521,
   k_pbpb18_sys_trk_m = 18522,
   k_pbpb18_sys_trig_p = 18531,
   k_pbpb18_sys_trig_m = 18532,
};
}

class muonTnP {

public :
    muonTnP() {
    };
    ~muonTnP() {};

    static double get_unc(int unc_index, double pt, double eta, int cent);
    static double get_weight_nom_pbpb18(double pt, double eta, int cent);

    static double get_unc_pp17_stat_id_p(double pt, double eta);
    static double get_unc_pp17_stat_id_m(double pt, double eta);

    static double get_unc_pbpb18_stat_id_p(double pt, double eta, int cent);
    static double get_unc_pbpb18_stat_id_m(double pt, double eta, int cent);
    static double get_unc_pbpb18_stat_glbtrk_p(double pt, double eta, int cent);
    static double get_unc_pbpb18_stat_glbtrk_m(double pt, double eta, int cent);
    static double get_unc_pbpb18_stat_trig_p(double pt, double eta, int cent);
    static double get_unc_pbpb18_stat_trig_m(double pt, double eta, int cent);

    static double get_unc_pbpb18_sys_id_p(double pt, double eta, int cent);
    static double get_unc_pbpb18_sys_id_m(double pt, double eta, int cent);
    static double get_unc_pbpb18_sys_glbtrk_p(double pt, double eta, int cent);
    static double get_unc_pbpb18_sys_glbtrk_m(double pt, double eta, int cent);
    static double get_unc_pbpb18_sys_trig_p(double pt, double eta, int cent);
    static double get_unc_pbpb18_sys_trig_m(double pt, double eta, int cent);
};

double muonTnP::get_unc(int unc_index, double pt, double eta, int cent)
{
    if (unc_index > 0) {
        if (unc_index == MUONTNP::k_pp17_stat_id_p) {
            return get_unc_pp17_stat_id_p(pt, eta);
        }
        else if (unc_index == MUONTNP::k_pp17_stat_id_m) {
            return get_unc_pp17_stat_id_m(pt, eta);
        }
        else if (unc_index == MUONTNP::k_pbpb18_stat_id_p) {
            return get_unc_pbpb18_stat_id_p(pt, eta, cent);
        }
        else if (unc_index == MUONTNP::k_pbpb18_stat_id_m) {
            return get_unc_pbpb18_stat_id_m(pt, eta, cent);
        }
        else if (unc_index == MUONTNP::k_pbpb18_stat_trk_p) {
            return get_unc_pbpb18_stat_glbtrk_p(pt, eta, cent);
        }
        else if (unc_index == MUONTNP::k_pbpb18_stat_trk_m) {
            return get_unc_pbpb18_stat_glbtrk_m(pt, eta, cent);
        }
        else if (unc_index == MUONTNP::k_pbpb18_stat_trig_p) {
            return get_unc_pbpb18_stat_trig_p(pt, eta, cent);
        }
        else if (unc_index == MUONTNP::k_pbpb18_stat_trig_m) {
            return get_unc_pbpb18_stat_trig_m(pt, eta, cent);
        }
        else if (unc_index == MUONTNP::k_pbpb18_sys_id_p) {
            return get_unc_pbpb18_sys_id_p(pt, eta, cent);
        }
        else if (unc_index == MUONTNP::k_pbpb18_sys_id_m) {
            return get_unc_pbpb18_sys_id_m(pt, eta, cent);
        }
        else if (unc_index == MUONTNP::k_pbpb18_sys_trk_p) {
            return get_unc_pbpb18_sys_glbtrk_p(pt, eta, cent);
        }
        else if (unc_index == MUONTNP::k_pbpb18_sys_trk_m) {
            return get_unc_pbpb18_sys_glbtrk_m(pt, eta, cent);
        }
        else if (unc_index == MUONTNP::k_pbpb18_sys_trig_p) {
            return get_unc_pbpb18_sys_trig_p(pt, eta, cent);
        }
        else if (unc_index == MUONTNP::k_pbpb18_sys_trig_m) {
            return get_unc_pbpb18_sys_trig_m(pt, eta, cent);
        }
    }

    return 1;
}

double muonTnP::get_unc_pp17_stat_id_p(double pt, double eta)
{
    return weight_sf_unc_id_pp2017(pt, eta);
}

double muonTnP::get_unc_pp17_stat_id_m(double pt, double eta)
{
    return 1./(weight_sf_unc_id_pp2017(pt, eta));
}

double muonTnP::get_weight_nom_pbpb18(double pt, double eta, int cent)
{
    return (tnp_weight_glbtrk_pbpb(eta, cent, 0) * tnp_weight_muid_pbpb(eta, 0) * tnp_weight_trig_pbpb(pt, eta, cent, 0));
}

double muonTnP::get_unc_pbpb18_stat_id_p(double pt, double eta, int cent)
{
    return (tnp_weight_glbtrk_pbpb(eta, cent, 0) * tnp_weight_muid_pbpb(eta, 1) * tnp_weight_trig_pbpb(pt, eta, cent, 0) / get_weight_nom_pbpb18(pt, eta, cent));
}

double muonTnP::get_unc_pbpb18_stat_id_m(double pt, double eta, int cent)
{
    return (tnp_weight_glbtrk_pbpb(eta, cent, 0) * tnp_weight_muid_pbpb(eta, 2) * tnp_weight_trig_pbpb(pt, eta, cent, 0) / get_weight_nom_pbpb18(pt, eta, cent));
}

double muonTnP::get_unc_pbpb18_stat_glbtrk_p(double pt, double eta, int cent)
{
    return (tnp_weight_glbtrk_pbpb(eta, cent, 1) * tnp_weight_muid_pbpb(eta, 0) * tnp_weight_trig_pbpb(pt, eta, cent, 0) / get_weight_nom_pbpb18(pt, eta, cent));
}

double muonTnP::get_unc_pbpb18_stat_glbtrk_m(double pt, double eta, int cent)
{
    return (tnp_weight_glbtrk_pbpb(eta, cent, 2) * tnp_weight_muid_pbpb(eta, 0) * tnp_weight_trig_pbpb(pt, eta, cent, 0) / get_weight_nom_pbpb18(pt, eta, cent));
}

double muonTnP::get_unc_pbpb18_stat_trig_p(double pt, double eta, int cent)
{
    return (tnp_weight_glbtrk_pbpb(eta, cent, 0) * tnp_weight_muid_pbpb(eta, 0) * tnp_weight_trig_pbpb(pt, eta, cent, 1) / get_weight_nom_pbpb18(pt, eta, cent));
}

double muonTnP::get_unc_pbpb18_stat_trig_m(double pt, double eta, int cent)
{
    return (tnp_weight_glbtrk_pbpb(eta, cent, 0) * tnp_weight_muid_pbpb(eta, 0) * tnp_weight_trig_pbpb(pt, eta, cent, 2) / get_weight_nom_pbpb18(pt, eta, cent));
}

double muonTnP::get_unc_pbpb18_sys_id_p(double pt, double eta, int cent)
{
    return (tnp_weight_glbtrk_pbpb(eta, cent, 0) * tnp_weight_muid_pbpb(eta, -1) * tnp_weight_trig_pbpb(pt, eta, cent, 0) / get_weight_nom_pbpb18(pt, eta, cent));
}

double muonTnP::get_unc_pbpb18_sys_id_m(double pt, double eta, int cent)
{
    return (tnp_weight_glbtrk_pbpb(eta, cent, 0) * tnp_weight_muid_pbpb(eta, -2) * tnp_weight_trig_pbpb(pt, eta, cent, 0) / get_weight_nom_pbpb18(pt, eta, cent));
}

double muonTnP::get_unc_pbpb18_sys_glbtrk_p(double pt, double eta, int cent)
{
    return (tnp_weight_glbtrk_pbpb(eta, cent, -1) * tnp_weight_muid_pbpb(eta, 0) * tnp_weight_trig_pbpb(pt, eta, cent, 0) / get_weight_nom_pbpb18(pt, eta, cent));
}

double muonTnP::get_unc_pbpb18_sys_glbtrk_m(double pt, double eta, int cent)
{
    return (tnp_weight_glbtrk_pbpb(eta, cent, -2) * tnp_weight_muid_pbpb(eta, 0) * tnp_weight_trig_pbpb(pt, eta, cent, 0) / get_weight_nom_pbpb18(pt, eta, cent));
}

double muonTnP::get_unc_pbpb18_sys_trig_p(double pt, double eta, int cent)
{
    return (tnp_weight_glbtrk_pbpb(eta, cent, 0) * tnp_weight_muid_pbpb(eta, 0) * tnp_weight_trig_pbpb(pt, eta, cent, -1) / get_weight_nom_pbpb18(pt, eta, cent));
}

double muonTnP::get_unc_pbpb18_sys_trig_m(double pt, double eta, int cent)
{
    return (tnp_weight_glbtrk_pbpb(eta, cent, 0) * tnp_weight_muid_pbpb(eta, 0) * tnp_weight_trig_pbpb(pt, eta, cent, -2) / get_weight_nom_pbpb18(pt, eta, cent));
}

#endif /* LEPTONS_MUONS_SYSUTIL_H_ */

