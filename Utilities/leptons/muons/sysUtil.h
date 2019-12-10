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

   k_pbpb18_stat_id_p = 11,
   k_pbpb18_stat_id_m = 12,
   k_pbpb18_stat_trk_p = 21,
   k_pbpb18_stat_trk_m = 22,
   k_pbpb18_stat_trig_p = 31,
   k_pbpb18_stat_trig_m = 32,
   k_pbpb18_sys_id_p = 511,
   k_pbpb18_sys_id_m = 512,
   k_pbpb18_sys_trk_p = 521,
   k_pbpb18_sys_trk_m = 522,
   k_pbpb18_sys_trig_p = 531,
   k_pbpb18_sys_trig_m = 532,
};
}

class muonTnP {

public :
    muonTnP() {
    };
    ~muonTnP() {};

    static double get_unc(int unc_index, double pt, double eta, int cent);
    static double get_weight_nom_pbpb18(double pt, double eta, int cent);

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
        if (unc_index == MUONTNP::k_pbpb18_stat_id_p) {
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

