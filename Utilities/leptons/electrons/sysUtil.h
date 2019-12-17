/*
 * utilities related to electron uncertainties.
 */
#ifndef LEPTONS_ELECTRONS_SYSUTIL_H_
#define LEPTONS_ELECTRONS_SYSUTIL_H_

#include <TMath.h>

#include <string>
#include <vector>
#include <iostream>

namespace ELESYS {

enum UNC_LABELS {

   k_unc_pp17_tot_id = 720,
   k_unc_pp17_les_p = 731,
   k_unc_pp17_les_m = 732,
   k_unc_pbpb18_tot_reco = 810,
   k_unc_pbpb18_stat_reco = 811,
   k_unc_pbpb18_sys_reco = 812,
   k_unc_pbpb18_tot_id = 820,
   k_unc_pbpb18_stat_id = 821,
   k_unc_pbpb18_sys_id = 822,
};
}

class eleSys {

public :
    eleSys() {
    };
    ~eleSys() {};

    static double get_unc(int unc_index, double pt, double eta, int cent);

    static double get_unc_pp17_les_p(double pt, double eta);
    static double get_unc_pp17_les_m(double pt, double eta);
    static double get_unc_pp17_scale_pt(double pt, double eta);

    static double get_unc_pp17_tot_id(double pt, double eta);
    static double get_unc_pp17_stat_id(double pt, double eta);
    static double get_unc_pp17_sys_id(double pt, double eta);

    static double get_unc_pp17_tot_id_loose(double pt, double eta);
    static double get_unc_pp17_stat_id_loose(double pt, double eta);
    static double get_unc_pp17_sys_id_loose(double pt, double eta);

    static double get_unc_pbpb18_tot_reco(double pt, double eta, int cent);
    static double get_unc_pbpb18_stat_reco(double pt, double eta, int cent);
    static double get_unc_pbpb18_sys_reco(double pt, double eta, int cent);

    static double get_unc_pbpb18_tot_id(double pt, double eta, int cent);
    static double get_unc_pbpb18_stat_id(double pt, double eta, int cent);
    static double get_unc_pbpb18_sys_id(double pt, double eta, int cent);

    static double get_unc_pbpb18_tot_id_loose(double pt, double eta, int cent);
    static double get_unc_pbpb18_stat_id_loose(double pt, double eta, int cent);
    static double get_unc_pbpb18_sys_id_loose(double pt, double eta, int cent);
};

double eleSys::get_unc(int unc_index, double pt, double eta, int cent)
{
    if (unc_index > 0) {
        if (unc_index == ELESYS::k_unc_pp17_tot_id) {
            return get_unc_pp17_tot_id(pt, eta);
        }
        else if (unc_index == ELESYS::k_unc_pp17_les_p) {
            return get_unc_pp17_les_p(pt, eta);
        }
        else if (unc_index == ELESYS::k_unc_pp17_les_m) {
            return get_unc_pp17_les_m(pt, eta);
        }
        else if (unc_index == ELESYS::k_unc_pbpb18_tot_reco) {
            return get_unc_pbpb18_tot_reco(pt, eta, cent);
        }
        else if (unc_index == ELESYS::k_unc_pbpb18_stat_reco) {
            return get_unc_pbpb18_stat_reco(pt, eta, cent);
        }
        else if (unc_index == ELESYS::k_unc_pbpb18_sys_reco) {
            return get_unc_pbpb18_sys_reco(pt, eta, cent);
        }
        else if (unc_index == ELESYS::k_unc_pbpb18_tot_id) {
            return get_unc_pbpb18_tot_id(pt, eta, cent);
        }
        else if (unc_index == ELESYS::k_unc_pbpb18_stat_id) {
            return get_unc_pbpb18_stat_id(pt, eta, cent);
        }
        else if (unc_index == ELESYS::k_unc_pbpb18_sys_id) {
            return get_unc_pbpb18_sys_id(pt, eta, cent);
        }
    }

    return 1;
}

double eleSys::get_unc_pp17_les_p(double pt, double eta)
{
    return get_unc_pp17_scale_pt(pt, eta);
}

double eleSys::get_unc_pp17_les_m(double pt, double eta)
{
    return 1.0/(get_unc_pp17_scale_pt(pt, eta));
}

double eleSys::get_unc_pp17_scale_pt(double pt, double eta)
{
    if (std::fabs(eta) < 1.0)  return 1.0025960735;
    else if (std::fabs(eta) < 1.4)  return 1.0032187978;
    else if (std::fabs(eta) < 2.0)  return 1.0040023043;
    else if (std::fabs(eta) < 2.5)  return 1.0042536181;

    if (pt < 0) return 1;

    return 1;
}

double eleSys::get_unc_pp17_tot_id(double pt, double eta)
{
    return get_unc_pp17_tot_id_loose(pt, eta);
}

double eleSys::get_unc_pp17_stat_id(double pt, double eta)
{
    return get_unc_pp17_stat_id_loose(pt, eta);
}

double eleSys::get_unc_pp17_sys_id(double pt, double eta)
{
    return get_unc_pp17_sys_id_loose(pt, eta);
}

double eleSys::get_unc_pp17_tot_id_loose(double pt, double eta)
{
    double unc_stat = get_unc_pp17_stat_id_loose(pt, eta) - 1;
    double unc_sys = get_unc_pp17_sys_id_loose(pt, eta) - 1;

    return 1 + std::sqrt(unc_stat*unc_stat + unc_sys*unc_sys);
}

double eleSys::get_unc_pp17_stat_id_loose(double pt, double eta)
{
    double x = 0;
    if (pt < 0) {
        x = 0;
    }

    if (std::fabs(eta) < 1.4442) {

        if (pt > 60) {
            x = 1.0;
        }
        else if (pt > 40) {
            x = 0.5;
        }
    }
    else if (std::fabs(eta) > 1.566) {

        if (pt > 60) {
            x = 3.0;
        }
        else if (pt > 20) {
            x = 1.5;
        }
    }

    return 1 + (x/100);
}

double eleSys::get_unc_pp17_sys_id_loose(double pt, double eta)
{
    double x = 0;
    if (pt < 0) {
        x = 0;
    }
    if (eta < -999) {
        x = 0;
    }

    return 1 + (x/100);
}

double eleSys::get_unc_pbpb18_tot_reco(double pt, double eta, int cent)
{
    double unc_stat = get_unc_pbpb18_stat_reco(pt, eta, cent) - 1;
    double unc_sys = get_unc_pbpb18_sys_reco(pt, eta, cent) - 1;

    return 1 + std::sqrt(unc_stat*unc_stat + unc_sys*unc_sys);
}

double eleSys::get_unc_pbpb18_stat_reco(double pt, double eta, int cent)
{
    double x = 0;

    if (cent < 30) {
        if (std::fabs(eta) < 1.4442) {

            if (pt >= 20 && pt < 30) {
                x = 1.79619;
            }
            else if (pt >= 30 && pt < 40) {
                x = 0.602463;
            }
            else if (pt >= 40 && pt < 50) {
                x = 0.331475;
            }
            else if (pt >= 50) {
                x = 0.84636;
            }
        }
        else if (std::fabs(eta) > 1.566) {

            if (pt >= 20 && pt < 40) {
                x = 1.76571;
            }
            else if (pt >= 40 && pt < 50) {
                x = 0.739106;
            }
            else if (pt >= 50) {
                x = 1.73988;
            }
        }
    }
    else if (cent < 100) {
        if (std::fabs(eta) < 1.4442) {

            if (pt >= 20 && pt < 30) {
                x = 1.96384;
            }
            else if (pt >= 30 && pt < 40) {
                x = 0.80662;
            }
            else if (pt >= 40 && pt < 50) {
                x = 0.140941;
            }
            else if (pt >= 50) {
                x = 1.64099;
            }

        }
        else if (std::fabs(eta) > 1.566) {

            if (pt >= 20 && pt < 40) {
                x = 1.10104;
            }
            else if (pt >= 40) {
                x = 1.19394;
            }
        }
    }

    return 1 + (x/100);
}

double eleSys::get_unc_pbpb18_sys_reco(double pt, double eta, int cent)
{
    double x = 0;

    if (cent < 30) {
        if (std::fabs(eta) < 1.4442) {

            if (pt >= 20 && pt < 30) {
                x = 4.02559;
            }
            else if (pt >= 30 && pt < 40) {
                x = 0.610049;
            }
            else if (pt >= 40 && pt < 50) {
                x = 0.881408;
            }
            else if (pt >= 50) {
                x = 1.79758;
            }
        }
        else if (std::fabs(eta) > 1.566) {

            if (pt >= 20 && pt < 40) {
                x = 2.26438;
            }
            else if (pt >= 40 && pt < 50) {
                x = 1.36678;
            }
            else if (pt >= 50) {
                x = 5.2795;
            }
        }
    }
    else if (cent < 100) {
        if (std::fabs(eta) < 1.4442) {

            if (pt >= 20 && pt < 30) {
                x = 2.67146;
            }
            else if (pt >= 30 && pt < 40) {
                x = 0.656081;
            }
            else if (pt >= 40 && pt < 50) {
                x = 0.405282;
            }
            else if (pt >= 50) {
                x = 0.532908;
            }
        }
        else if (std::fabs(eta) > 1.566) {

            if (pt >= 20 && pt < 40) {
                x = 1.20122;
            }
            else if (pt >= 40) {
                x = 1.38008;
            }
        }
    }

    return 1 + (x/100);
}


double eleSys::get_unc_pbpb18_tot_id(double pt, double eta, int cent)
{
    return get_unc_pbpb18_tot_id_loose(pt, eta, cent);
}

double eleSys::get_unc_pbpb18_stat_id(double pt, double eta, int cent)
{
    return get_unc_pbpb18_stat_id_loose(pt, eta, cent);
}

double eleSys::get_unc_pbpb18_sys_id(double pt, double eta, int cent)
{
    return get_unc_pbpb18_sys_id_loose(pt, eta, cent);
}

double eleSys::get_unc_pbpb18_tot_id_loose(double pt, double eta, int cent)
{
    double unc_stat = get_unc_pbpb18_stat_id_loose(pt, eta, cent) - 1;
    double unc_sys = get_unc_pbpb18_sys_id_loose(pt, eta, cent) - 1;

    return 1 + std::sqrt(unc_stat*unc_stat + unc_sys*unc_sys);
}

double eleSys::get_unc_pbpb18_stat_id_loose(double pt, double eta, int cent)
{
    double x = 0;

    if (cent < 30) {
        if (std::fabs(eta) < 1.4442) {

            if (pt >= 20 && pt < 30) {
                x = 1.9745;
            }
            else if (pt >= 30 && pt < 40) {
                x = 1.60662;
            }
            else if (pt >= 40 && pt < 50) {
                x = 0.934056;
            }
            else if (pt >= 50) {
                x = 1.66259;
            }
        }
        else if (std::fabs(eta) > 1.566) {

            if (pt >= 20 && pt < 40) {
                x = 5.38379;
            }
            else if (pt >= 40) {
                x = 2.50464;
            }
        }
    }
    else if (cent < 100) {
        if (std::fabs(eta) < 1.4442) {

            if (pt >= 20 && pt < 40) {
                x = 2.38699;
            }
            else if (pt >= 40) {
                x = 1.87081;
            }

        }
        else if (std::fabs(eta) > 1.566) {

            if (pt >= 20 && pt < 40) {
                x = 3.91349;
            }
            else if (pt >= 40) {
                x = 2.66237;
            }
        }
    }

    return 1 + (x/100);
}

double eleSys::get_unc_pbpb18_sys_id_loose(double pt, double eta, int cent)
{
    double x = 0;

    if (cent < 30) {
        if (std::fabs(eta) < 1.4442) {

            if (pt >= 20 && pt < 30) {
                x = 4.51672;
            }
            else if (pt >= 30 && pt < 40) {
                x = 3.00496;
            }
            else if (pt >= 40 && pt < 50) {
                x = 1.79509;
            }
            else if (pt >= 50) {
                x = 1.31062;
            }
        }
        else if (std::fabs(eta) > 1.566) {

            if (pt >= 20 && pt < 40) {
                x = 4.22958;
            }
            else if (pt >= 40) {
                x = 2.94438;
            }
        }
    }
    else if (cent < 100) {
        if (std::fabs(eta) < 1.4442) {

            if (pt >= 20 && pt < 30) {
                x = 2.21367;
            }
            else if (pt >= 40) {
                x = 2.06116;
            }
        }
        else if (std::fabs(eta) > 1.566) {

            if (pt >= 20 && pt < 40) {
                x = 7.52736;
            }
            else if (pt >= 40) {
                x = 2.01388;
            }
        }
    }

    return 1 + (x/100);
}

#endif /* LEPTONS_ELECTRONS_SYSUTIL_H_ */

