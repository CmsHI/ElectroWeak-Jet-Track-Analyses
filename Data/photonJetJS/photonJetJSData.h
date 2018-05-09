/*
 * file with hard coded data tables for different photon+jet JS studies
 */

#ifndef DATA_PHOTONJETJS_PHOTONJETJSDATA_H_
#define DATA_PHOTONJETJS_PHOTONJETJSDATA_H_

#include <vector>
#include <string>
#include <TColor.h>

namespace SCET {

/*
 * SCET_G photon+jet JS prediction data tables
 * naming convention :
 *      n_<observable>    : number of points in the observable
 *      x_<observable>    : x axis values of the observable, bin centers
 *      y_<observable>    : y axis values of the observable
 */

const std::string modelName = "SCET";

// r points
const int n_rho = 6;
const std::vector<double> x_r = {0.025, 0.075, 0.125, 0.175, 0.225, 0.275};

// PbPb / pp ratio of rho(r) distributions
const std::vector<double> ymin_rho_ratio_pbpb_pp = {0.887049, 1.14779, 1.29244, 1.47301, 1.63391, 1.568};
const std::vector<double> ymax_rho_ratio_pbpb_pp = {0.88727, 1.16766, 1.33628, 1.57292, 1.85721, 2.15164};

enum DATATABLES{
    k_rho_ratio_pbpb_pp,
    kN_DATATABLES
};

const std::string dataNames[kN_DATATABLES] = {
        "rho_ratio_pbpb_pp"
};

const std::vector<double> xData[kN_DATATABLES] = {
        x_r
};

const std::vector<double> yminData[kN_DATATABLES] = {
        ymin_rho_ratio_pbpb_pp
};

const std::vector<double> ymaxData[kN_DATATABLES] = {
        ymax_rho_ratio_pbpb_pp
};

const std::string xTitles[kN_DATATABLES] = {
        "r"
};

const std::string yTitles[kN_DATATABLES] = {
        "\rho(r)_{PbPb} / \rho(r)_{pp}"
};

const std::string titles[kN_DATATABLES] = {
        "SCET_{G} Chien-Vitev"
};

// fill colors to be used in a plot
const int colors[kN_DATATABLES] = {
        TColor::GetColor("#139808"),
};

const int lineStyles[kN_DATATABLES] = {
        12
};

const double falphas[kN_DATATABLES] = {
        0.7
};

};

#endif /* DATA_PHOTONJETJS_PHOTONJETJSDATA_H_ */
