/*
 * utilities related to muon uncertainties.
 */
#ifndef LEPTONS_MUONS_SYSUTIL_H_
#define LEPTONS_MUONS_SYSUTIL_H_

#include <TMath.h>

#include <string>
#include <vector>
#include <iostream>

#include "2018PbPb/tnp_weight.h"

namespace MUONTNP {

enum UNC_LABELS {

   k_stat_id_p = 11,
   k_stat_id_m = 12,
   k_stat_trig_p = 21,
   k_stat_trig_m = 22,
   k_sys_id_p = 31,
   k_sys_id_m = 32,
   k_sys_trig_p = 41,
   k_sys_trig_m = 42,
};
}

class muonTnP {

public :
    muonTnP() {
    };
    ~muonTnP() {};

    static double get_unc(int unc_index, double pt, double eta, int cent);
    static double get_weight_nom(double pt, double eta, int cent);

    static double get_unc_stat_id_p(double pt, double eta, int cent);
    static double get_unc_stat_id_m(double pt, double eta, int cent);
    static double get_unc_stat_trig_p(double pt, double eta, int cent);
    static double get_unc_stat_trig_m(double pt, double eta, int cent);

    static double get_unc_sys_id_p(double pt, double eta, int cent);
    static double get_unc_sys_id_m(double pt, double eta, int cent);
    static double get_unc_sys_trig_p(double pt, double eta, int cent);
    static double get_unc_sys_trig_m(double pt, double eta, int cent);
};

double muonTnP::get_unc(int unc_index, double pt, double eta, int cent)
{
    if (unc_index > 0) {
        if (unc_index == MUONTNP::k_stat_id_p) {
            return get_unc_stat_id_p(pt, eta, cent);
        }
        else if (unc_index == MUONTNP::k_stat_id_m) {
            return get_unc_stat_id_m(pt, eta, cent);
        }
        else if (unc_index == MUONTNP::k_stat_trig_p) {
            return get_unc_stat_trig_p(pt, eta, cent);
        }
        else if (unc_index == MUONTNP::k_stat_trig_m) {
            return get_unc_stat_trig_m(pt, eta, cent);
        }
        else if (unc_index == MUONTNP::k_sys_id_p) {
            return get_unc_sys_id_p(pt, eta, cent);
        }
        else if (unc_index == MUONTNP::k_sys_id_m) {
            return get_unc_sys_id_m(pt, eta, cent);
        }
        else if (unc_index == MUONTNP::k_sys_trig_p) {
            return get_unc_sys_trig_p(pt, eta, cent);
        }
        else if (unc_index == MUONTNP::k_sys_trig_m) {
            return get_unc_sys_trig_m(pt, eta, cent);
        }
    }

    return 1;
}

double muonTnP::get_weight_nom(double pt, double eta, int cent)
{
    return (tnp_weight_muid_pbpb(eta, 0) * tnp_weight_trig_pbpb(pt, eta, cent, 0));
}

double muonTnP::get_unc_stat_id_p(double pt, double eta, int cent)
{
    return (tnp_weight_muid_pbpb(eta, 1) * tnp_weight_trig_pbpb(pt, eta, cent, 0) / get_weight_nom(pt, eta, cent));
}

double muonTnP::get_unc_stat_id_m(double pt, double eta, int cent)
{
    return (tnp_weight_muid_pbpb(eta, 2) * tnp_weight_trig_pbpb(pt, eta, cent, 0) / get_weight_nom(pt, eta, cent));
}

double muonTnP::get_unc_stat_trig_p(double pt, double eta, int cent)
{
    return (tnp_weight_muid_pbpb(eta, 0) * tnp_weight_trig_pbpb(pt, eta, cent, 1) / get_weight_nom(pt, eta, cent));
}

double muonTnP::get_unc_stat_trig_m(double pt, double eta, int cent)
{
    return (tnp_weight_muid_pbpb(eta, 0) * tnp_weight_trig_pbpb(pt, eta, cent, 2) / get_weight_nom(pt, eta, cent));
}

double muonTnP::get_unc_sys_id_p(double pt, double eta, int cent)
{
    return (tnp_weight_muid_pbpb(eta, -1) * tnp_weight_trig_pbpb(pt, eta, cent, 0) / get_weight_nom(pt, eta, cent));
}

double muonTnP::get_unc_sys_id_m(double pt, double eta, int cent)
{
    return (tnp_weight_muid_pbpb(eta, -2) * tnp_weight_trig_pbpb(pt, eta, cent, 0) / get_weight_nom(pt, eta, cent));
}

double muonTnP::get_unc_sys_trig_p(double pt, double eta, int cent)
{
    return (tnp_weight_muid_pbpb(eta, 0) * tnp_weight_trig_pbpb(pt, eta, cent, -1) / get_weight_nom(pt, eta, cent));
}

double muonTnP::get_unc_sys_trig_m(double pt, double eta, int cent)
{
    return (tnp_weight_muid_pbpb(eta, 0) * tnp_weight_trig_pbpb(pt, eta, cent, -2) / get_weight_nom(pt, eta, cent));
}

#endif /* LEPTONS_MUONS_SYSUTIL_H_ */

