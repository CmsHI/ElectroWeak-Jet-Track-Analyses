/*
 * file with hard coded data tables for different photon+jet FF studies
 */

#ifndef DATA_PHOTONJETFF_PHOTONJETFFDATA_H_
#define DATA_PHOTONJETFF_PHOTONJETFFDATA_H_

#include <vector>
#include <string>
#include <TColor.h>

namespace SCET {

/*
 * SCET_G. photon+jet FF prediction data tables
 * naming convention :
 *      n_<observable>    : number of points in the observable
 *      x_<observable>    : x axis values of the observable, bin centers
 *      y_<observable>    : y axis values of the observable
 */

// zh points, "The curves are versus zh which is to very good approximation your xi_jet"
const int n_zh = 98;
const std::vector<double> x_zh = {0.7, 0.740404, 0.780808, 0.821212, 0.861616, 0.90202, 0.942424, 0.982828, 1.02323, 1.06364, 1.10404, 1.14444, 1.18485, 1.22525, 1.26566, 1.30606, 1.34646, 1.38687, 1.42727, 1.46768, 1.50808, 1.54848, 1.58889, 1.62929, 1.6697, 1.7101, 1.75051, 1.79091, 1.83131, 1.87172, 1.91212, 1.95253, 1.99293, 2.03333, 2.07374, 2.11414, 2.15455, 2.19495, 2.23535, 2.27576, 2.31616, 2.35657, 2.39697, 2.43737, 2.47778, 2.51818, 2.55859, 2.59899, 2.63939, 2.6798, 2.7202, 2.76061, 2.80101, 2.84141, 2.88182, 2.92222, 2.96263, 3.00303, 3.04343, 3.08384, 3.12424, 3.16465, 3.20505, 3.24545, 3.28586, 3.32626, 3.36667, 3.40707, 3.44747, 3.48788, 3.52828, 3.56869, 3.60909, 3.64949, 3.6899, 3.7303, 3.77071, 3.81111, 3.85152, 3.89192, 3.93232, 3.97273, 4.01313, 4.05354, 4.09394, 4.13434, 4.17475, 4.21515, 4.25556, 4.29596, 4.33636, 4.37677, 4.41717, 4.45758, 4.49798, 4.53838, 4.57879, 4.61919};

// PbPb / pp ratio of zh distributions
const std::vector<double> ymin_zh_ratio_PbPb_pp_g2p0 = {0.845905, 0.848821, 0.851723, 0.854597, 0.857429, 0.860208, 0.862922, 0.865563, 0.868121, 0.870588, 0.872959, 0.875228, 0.877388, 0.879438, 0.881373, 0.883191, 0.884891, 0.886472, 0.887933, 0.889276, 0.8905, 0.891608, 0.892603, 0.893487, 0.894263, 0.894935, 0.895509, 0.895988, 0.896378, 0.896686, 0.896917, 0.897079, 0.897179, 0.897224, 0.897222, 0.897184, 0.897116, 0.89703, 0.896934, 0.89684, 0.896758, 0.896699, 0.896676, 0.896701, 0.896786, 0.896944, 0.897189, 0.897536, 0.897999, 0.898592, 0.899333, 0.900236, 0.901318, 0.902597, 0.90409, 0.905814, 0.90779, 0.910036, 0.91257, 0.915415, 0.918589, 0.922114, 0.926011, 0.930302, 0.935009, 0.940155, 0.945762, 0.951854, 0.958454, 0.965585, 0.973271, 0.981535, 0.990403, 0.999897, 1.01004, 1.02086, 1.03237, 1.04461, 1.05758, 1.07132, 1.08585, 1.10118, 1.11733, 1.13433, 1.15219, 1.17092, 1.19054, 1.21106, 1.2325, 1.25485, 1.27814, 1.30235, 1.32749, 1.35356, 1.38054, 1.40845, 1.43724, 1.46692};
const std::vector<double> ymax_zh_ratio_PbPb_pp_g2p0 = {0.895346, 0.898714, 0.902054, 0.905353, 0.9086, 0.911788, 0.914906, 0.917947, 0.920906, 0.923774, 0.926549, 0.929225, 0.931799, 0.934267, 0.936628, 0.938881, 0.941024, 0.943058, 0.944983, 0.946799, 0.948509, 0.950114, 0.951618, 0.953023, 0.954334, 0.955555, 0.95669, 0.957746, 0.958727, 0.95964, 0.960492, 0.96129, 0.962042, 0.962756, 0.963441, 0.964106, 0.964762, 0.965417, 0.966083, 0.966771, 0.967492, 0.96826, 0.969086, 0.969985, 0.970968, 0.972052, 0.973251, 0.97458, 0.976056, 0.977694, 0.979512, 0.981527, 0.983758, 0.986223, 0.988942, 0.991934, 0.99522, 0.99882, 1.00276, 1.00705, 1.01173, 1.01681, 1.02231, 1.02827, 1.0347, 1.04164, 1.04909, 1.0571, 1.06569, 1.07487, 1.08469, 1.09515, 1.1063, 1.11816, 1.13075, 1.1441, 1.15823, 1.17318, 1.18896, 1.2056, 1.22313, 1.24156, 1.26092, 1.28124, 1.30253, 1.3248, 1.34809, 1.3724, 1.39774, 1.42414, 1.45159, 1.48011, 1.5097, 1.54037, 1.5721, 1.6049, 1.63876, 1.67366};

enum MODEL{
    kG2P0,  // g=2.0 +/- 0.2
    kN_MODEL
};

const std::vector<double> ymin_zh[kN_MODEL] = {
        ymin_zh_ratio_PbPb_pp_g2p0
};

const std::vector<double> ymax_zh[kN_MODEL] = {
        ymax_zh_ratio_PbPb_pp_g2p0
};

// fill colors to be used in a plot
int fillColors[kN_MODEL] = {
        TColor::GetColor("#138808"),
};

const int lineStyles[kN_MODEL] = {
        12
};

const double falpha = 0.7;

const std::string legendEntries[kN_MODEL] = {
        "SCET_G."  // "g = 2.0 #pm 0.2"
};
const std::string modelTAG[kN_MODEL] = {
        "g2p0"
};

};

namespace COLBT {

/*
 * Couple LBT-hydro photon+jet FF prediction data tables
 * naming convention :
 *      n_<observable>    : number of points in the observable
 *      x_<observable>    : x axis values of the observable, bin centers
 *      y_<observable>    : y axis values of the observable
 */

const int n_xijet = 8;

// PbPb / pp ratio of xijet distributions
const std::vector<double> x_xijet_ratio_PbPb_pp_cent010 = {0.75, 1.25, 1.75, 2.25, 2.75, 3.25, 3.75, 4.25};
const std::vector<double> y_xijet_ratio_PbPb_pp_cent010 = {1.0376, 0.9242, 0.8798, 0.8929, 1.0036, 1.2092, 1.4642, 2.3966};

const std::vector<double> x_xijet_ratio_PbPb_pp_cent1030 = {0.75, 1.25, 1.75, 2.25, 2.75, 3.25, 3.75, 4.25};
const std::vector<double> y_xijet_ratio_PbPb_pp_cent1030 = {1.0316, 0.9419, 0.9118, 0.9555, 1.0186, 1.08, 1.1519, 1.4793};

const int n_xigamma = 8;

// PbPb / pp ratio of xigamma distributions
const std::vector<double> x_xigamma_ratio_PbPb_pp_cent010 = {0.75, 1.25, 1.75, 2.25, 2.75, 3.25, 3.75, 4.25};
const std::vector<double> y_xigamma_ratio_PbPb_pp_cent010 = {0.4912, 0.6387, 0.6579, 0.6522, 0.8253, 1.1071, 1.4865, 2.2029};

const std::vector<double> x_xigamma_ratio_PbPb_pp_cent1030 = {0.75, 1.25, 1.75, 2.25, 2.75, 3.25, 3.75, 4.25};
const std::vector<double> y_xigamma_ratio_PbPb_pp_cent1030 = {0.6131, 0.7321, 0.7181, 0.7401, 0.8398, 1.0148, 1.2143, 1.6115};

};

#endif /* DATA_PHOTONJETFF_PHOTONJETFFDATA_H_ */
