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

const std::string modelName = "SCET";

// zh points, "The curves are versus zh which is to very good approximation your xi_jet"
const int n_zh = 98;
const std::vector<double> x_zh = {0.7, 0.740404, 0.780808, 0.821212, 0.861616, 0.90202, 0.942424, 0.982828, 1.02323, 1.06364, 1.10404, 1.14444, 1.18485, 1.22525, 1.26566, 1.30606, 1.34646, 1.38687, 1.42727, 1.46768, 1.50808, 1.54848, 1.58889, 1.62929, 1.6697, 1.7101, 1.75051, 1.79091, 1.83131, 1.87172, 1.91212, 1.95253, 1.99293, 2.03333, 2.07374, 2.11414, 2.15455, 2.19495, 2.23535, 2.27576, 2.31616, 2.35657, 2.39697, 2.43737, 2.47778, 2.51818, 2.55859, 2.59899, 2.63939, 2.6798, 2.7202, 2.76061, 2.80101, 2.84141, 2.88182, 2.92222, 2.96263, 3.00303, 3.04343, 3.08384, 3.12424, 3.16465, 3.20505, 3.24545, 3.28586, 3.32626, 3.36667, 3.40707, 3.44747, 3.48788, 3.52828, 3.56869, 3.60909, 3.64949, 3.6899, 3.7303, 3.77071, 3.81111, 3.85152, 3.89192, 3.93232, 3.97273, 4.01313, 4.05354, 4.09394, 4.13434, 4.17475, 4.21515, 4.25556, 4.29596, 4.33636, 4.37677, 4.41717, 4.45758, 4.49798, 4.53838, 4.57879, 4.61919};

// PbPb / pp ratio of zh distributions
const std::vector<double> ymin_zh_ratio_pbpb_pp_g2p0 = {0.845905, 0.848821, 0.851723, 0.854597, 0.857429, 0.860208, 0.862922, 0.865563, 0.868121, 0.870588, 0.872959, 0.875228, 0.877388, 0.879438, 0.881373, 0.883191, 0.884891, 0.886472, 0.887933, 0.889276, 0.8905, 0.891608, 0.892603, 0.893487, 0.894263, 0.894935, 0.895509, 0.895988, 0.896378, 0.896686, 0.896917, 0.897079, 0.897179, 0.897224, 0.897222, 0.897184, 0.897116, 0.89703, 0.896934, 0.89684, 0.896758, 0.896699, 0.896676, 0.896701, 0.896786, 0.896944, 0.897189, 0.897536, 0.897999, 0.898592, 0.899333, 0.900236, 0.901318, 0.902597, 0.90409, 0.905814, 0.90779, 0.910036, 0.91257, 0.915415, 0.918589, 0.922114, 0.926011, 0.930302, 0.935009, 0.940155, 0.945762, 0.951854, 0.958454, 0.965585, 0.973271, 0.981535, 0.990403, 0.999897, 1.01004, 1.02086, 1.03237, 1.04461, 1.05758, 1.07132, 1.08585, 1.10118, 1.11733, 1.13433, 1.15219, 1.17092, 1.19054, 1.21106, 1.2325, 1.25485, 1.27814, 1.30235, 1.32749, 1.35356, 1.38054, 1.40845, 1.43724, 1.46692};
const std::vector<double> ymax_zh_ratio_pbpb_pp_g2p0 = {0.895346, 0.898714, 0.902054, 0.905353, 0.9086, 0.911788, 0.914906, 0.917947, 0.920906, 0.923774, 0.926549, 0.929225, 0.931799, 0.934267, 0.936628, 0.938881, 0.941024, 0.943058, 0.944983, 0.946799, 0.948509, 0.950114, 0.951618, 0.953023, 0.954334, 0.955555, 0.95669, 0.957746, 0.958727, 0.95964, 0.960492, 0.96129, 0.962042, 0.962756, 0.963441, 0.964106, 0.964762, 0.965417, 0.966083, 0.966771, 0.967492, 0.96826, 0.969086, 0.969985, 0.970968, 0.972052, 0.973251, 0.97458, 0.976056, 0.977694, 0.979512, 0.981527, 0.983758, 0.986223, 0.988942, 0.991934, 0.99522, 0.99882, 1.00276, 1.00705, 1.01173, 1.01681, 1.02231, 1.02827, 1.0347, 1.04164, 1.04909, 1.0571, 1.06569, 1.07487, 1.08469, 1.09515, 1.1063, 1.11816, 1.13075, 1.1441, 1.15823, 1.17318, 1.18896, 1.2056, 1.22313, 1.24156, 1.26092, 1.28124, 1.30253, 1.3248, 1.34809, 1.3724, 1.39774, 1.42414, 1.45159, 1.48011, 1.5097, 1.54037, 1.5721, 1.6049, 1.63876, 1.67366};

enum DATATABLES{
    k_zh_ratio_pbpb_pp_g2p0,  // g=2.0 +/- 0.2
    kN_DATATABLES
};

const std::string dataNames[kN_DATATABLES] = {
        "zh_ratio_pbpb_pp_g2p0"
};

const std::vector<double> xData[kN_DATATABLES] = {
        x_zh
};

const std::vector<double> yminData[kN_DATATABLES] = {
        ymin_zh_ratio_pbpb_pp_g2p0
};

const std::vector<double> ymaxData[kN_DATATABLES] = {
        ymax_zh_ratio_pbpb_pp_g2p0
};

const std::string xTitles[kN_DATATABLES] = {
        "zh"
};

const std::string yTitles[kN_DATATABLES] = {
        "PbPb / pp"
};

const std::string titles[kN_DATATABLES] = {
        "SCET_{G}"  // "g = 2.0 #pm 0.2"
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

namespace COLBT {

/*
 * Couple LBT-hydro photon+jet FF prediction data tables
 * naming convention :
 *      n_<observable>    : number of points in the observable
 *      x_<observable>    : x axis values of the observable, bin centers
 *      y_<observable>    : y axis values of the observable
 */

const std::string modelName = "CoLBT";

const int n_xijet = 8;

// xijet distributions
const std::vector<double> x_xijet_pp = {0.75, 1.25, 1.75, 2.25, 2.75, 3.25, 3.75, 4.25};
const std::vector<double> y_xijet_pp = {0.3652, 0.9028, 1.4704, 2.0299, 2.304, 2.1461, 1.5155, 0.6117};

const std::vector<double> x_xijet_pbpb_cent010 = {0.75, 1.25, 1.75, 2.25, 2.75, 3.25, 3.75, 4.25};
const std::vector<double> y_xijet_pbpb_cent010 = {0.3710931049, 0.8654814314, 1.2892395996, 1.8841397385, 2.4116838017, 2.514590212, 1.8644089835, 1.0202310513};

const std::vector<double> x_xijet_pbpb_cent1030 = {0.75, 1.25, 1.75, 2.25, 2.75, 3.25, 3.75, 4.25};
const std::vector<double> y_xijet_pbpb_cent1030 = {0.3596858395, 0.8909740806, 1.3419588522, 1.9115780402, 2.3754135282, 2.3013550272, 1.7296642085, 0.9200285501};

// PbPb / pp ratio of xijet distributions
const std::vector<double> x_xijet_ratio_pbpb_pp_cent010 = {0.75, 1.25, 1.75, 2.25, 2.75, 3.25, 3.75, 4.25};
const std::vector<double> y_xijet_ratio_pbpb_pp_cent010 = {1.0376, 0.9242, 0.8798, 0.8929, 1.0036, 1.2092, 1.4642, 2.3966};

const std::vector<double> x_xijet_ratio_pbpb_pp_cent1030 = {0.75, 1.25, 1.75, 2.25, 2.75, 3.25, 3.75, 4.25};
const std::vector<double> y_xijet_ratio_pbpb_pp_cent1030 = {1.0316, 0.9419, 0.9118, 0.9555, 1.0186, 1.08, 1.1519, 1.4793};

const int n_xigamma = 8;

// xigamma distributions
const std::vector<double> x_xigamma_pp = {0.75, 1.25, 1.75, 2.25, 2.75, 3.25, 3.75, 4.25};
const std::vector<double> y_xigamma_pp = {0.2299, 0.596, 1.1496, 1.6699, 2.1609, 2.2637, 1.9984, 1.156};

const std::vector<double> x_xigamma_pbpb_cent010 = {0.75, 1.25, 1.75, 2.25, 2.75, 3.25, 3.75, 4.25};
const std::vector<double> y_xigamma_pbpb_cent010 = {0.1105796894, 0.3773785425, 0.7195914223, 1.0972855858, 1.5507180121, 1.9483615093, 2.2350400486, 2.0758411067};

const std::vector<double> x_xigamma_pbpb_cent1030 = {0.75, 1.25, 1.75, 2.25, 2.75, 3.25, 3.75, 4.25};
const std::vector<double> y_xigamma_pbpb_cent1030 = {0.133059313, 0.4360962846, 0.8358689396, 1.2285471967, 1.7364654846, 2.0810782201, 2.134946902, 1.7832719146};

// PbPb / pp ratio of xigamma distributions
const std::vector<double> x_xigamma_ratio_pbpb_pp_cent010 = {0.75, 1.25, 1.75, 2.25, 2.75, 3.25, 3.75, 4.25};
const std::vector<double> y_xigamma_ratio_pbpb_pp_cent010 = {0.4912, 0.6387, 0.6579, 0.6522, 0.8253, 1.1071, 1.4865, 2.2029};

const std::vector<double> x_xigamma_ratio_pbpb_pp_cent1030 = {0.75, 1.25, 1.75, 2.25, 2.75, 3.25, 3.75, 4.25};
const std::vector<double> y_xigamma_ratio_pbpb_pp_cent1030 = {0.6131, 0.7321, 0.7181, 0.7401, 0.8398, 1.0148, 1.2143, 1.6115};

enum DATATABLES{
    k_xijet_pp,
    k_xijet_pbpb_cent010,
    k_xijet_ratio_pbpb_pp_cent010,
    k_xijet_pbpb_cent1030,
    k_xijet_ratio_pbpb_pp_cent1030,
    k_xigamma_pp,
    k_xigamma_pbpb_cent010,
    k_xigamma_ratio_pbpb_pp_cent010,
    k_xigamma_pbpb_cent1030,
    k_xigamma_ratio_pbpb_pp_cent1030,
    kN_DATATABLES
};

const std::string dataNames[kN_DATATABLES] = {
        "xijet_pp",
        "xijet_pbpb_cent010",
        "xijet_ratio_pbpb_pp_cent010",
        "xijet_pbpb_cent1030",
        "xijet_ratio_pbpb_pp_cent1030",
        "xigamma_pp",
        "xigamma_pbpb_cent010",
        "xigamma_ratio_pbpb_pp_cent010",
        "xigamma_pbpb_cent1030",
        "xigamma_ratio_pbpb_pp_cent1030"
};

const std::vector<double> xData[kN_DATATABLES] = {
        x_xijet_pp,
        x_xijet_pbpb_cent010,
        x_xijet_ratio_pbpb_pp_cent010,
        x_xijet_pbpb_cent1030,
        x_xijet_ratio_pbpb_pp_cent1030,
        x_xigamma_pp,
        x_xigamma_pbpb_cent010,
        x_xigamma_ratio_pbpb_pp_cent010,
        x_xigamma_pbpb_cent1030,
        x_xigamma_ratio_pbpb_pp_cent1030
};

const std::vector<double> yData[kN_DATATABLES] = {
        y_xijet_pp,
        y_xijet_pbpb_cent010,
        y_xijet_ratio_pbpb_pp_cent010,
        y_xijet_pbpb_cent1030,
        y_xijet_ratio_pbpb_pp_cent1030,
        y_xigamma_pp,
        y_xigamma_pbpb_cent010,
        y_xigamma_ratio_pbpb_pp_cent010,
        y_xigamma_pbpb_cent1030,
        y_xigamma_ratio_pbpb_pp_cent1030
};

const std::string xTitles[kN_DATATABLES] = {
        "#xi^{jet}",
        "#xi^{jet}",
        "#xi^{jet}",
        "#xi^{jet}",
        "#xi^{jet}",
        "#xi^{#gamma}_{T}",
        "#xi^{#gamma}_{T}",
        "#xi^{#gamma}_{T}",
        "#xi^{#gamma}_{T}",
        "#xi^{#gamma}_{T}"
};

const std::string yTitles[kN_DATATABLES] = {
        "#frac{1}{N^{jet}} #frac{dN^{trk}}{d#xi^{jet}}",
        "#frac{1}{N^{jet}} #frac{dN^{trk}}{d#xi^{jet}}",
        "PbPb / pp",
        "#frac{1}{N^{jet}} #frac{dN^{trk}}{d#xi^{jet}}",
        "PbPb / pp",
        "#frac{1}{N^{jet}} #frac{dN^{trk}}{d#xi^{#gamma}_{T}}",
        "#frac{1}{N^{jet}} #frac{dN^{trk}}{d#xi^{#gamma}_{T}}",
        "PbPb / pp",
        "#frac{1}{N^{jet}} #frac{dN^{trk}}{d#xi^{#gamma}_{T}}",
        "PbPb / pp"
};

const std::string titles[kN_DATATABLES] = {
        "CoLBT-hydro pp",
        "CoLBT-hydro 0-10%",
        "CoLBT-hydro 0-10%",
        "CoLBT-hydro 10-30%",
        "CoLBT-hydro 10-30%",
        "CoLBT-hydro pp",
        "CoLBT-hydro 0-10%",
        "CoLBT-hydro 0-10%",
        "CoLBT-hydro 10-30%",
        "CoLBT-hydro 10-30%"
};

// fill colors to be used in a plot
const int colors[kN_DATATABLES] = {
        kBlue,
        kBlue,
        kBlue,
        kBlue,
        kBlue,
        kBlue,
        kBlue,
        kBlue,
        kBlue,
        kBlue
};

const int lineStyles[kN_DATATABLES] = {
        kDashed,
        kDashed,
        kBlue,
        kDashed,
        kBlue,
        kDashed,
        kDashed,
        kBlue,
        kDashed,
        kBlue
};

const double falphas[kN_DATATABLES] = {
        0.7,
        0.7,
        0.7,
        0.7,
        0.7,
        0.7,
        0.7,
        0.7,
        0.7,
        0.7
};

};

namespace HYBRID {

/*
 * Hybrid Model photon+jet FF prediction data tables
 * naming convention :
 *      n_<observable>    : number of points in the observable
 *      x_<observable>    : x axis values of the observable, bin centers
 *      y_<observable>    : y axis values of the observable
 *      yerr_<observable> : absolute error on y-axis values
 *      ymin_<observable> : lower part of the band
 *      ymax_<observable> : upper part of the band
 *          The band includes all sort of uncertainties : theoretical, experimental and statistical.
 *
 *      "back" means medium response contribution is included, while "noback" means it is not.
 */

const std::string modelName = "hybrid";

const int n_xijet = 20;

const std::vector<double> x_xijet_back_cent010 = {0.1275, 0.3775, 0.6275, 0.8775, 1.1275, 1.3775, 1.6275, 1.8775, 2.1275, 2.3775, 2.6275, 2.8775, 3.1275, 3.3775, 3.6275, 3.8775, 4.1275, 4.3775, 4.6275, 4.8775};
const std::vector<double> ymin_xijet_back_ratio_pbpb_pp_cent010 = {1.06162, 1.08988, 1.11578, 1.09236, 1.0229, 0.980937, 0.94261, 0.899682, 0.865457, 0.853641, 0.834432, 0.851882, 0.896879, 0.970647, 0.992001, 0.986937, 0.980073, 1.01144, 1.01587, 0.96315};
const std::vector<double> ymax_xijet_back_ratio_pbpb_pp_cent010 = {1.26339, 1.18986, 1.16695, 1.13012, 1.05148, 1.00139, 0.966934, 0.933911, 0.885026, 0.869082, 0.863481, 0.867031, 0.925491, 0.989294, 1.03206, 1.00999, 1.01725, 1.04443, 1.0735, 1.05582};
const std::vector<double> ymin_xijet_back_pbpb_cent010 = {0.0633979, 0.164918, 0.309284, 0.511786, 0.732664, 0.973023, 1.21255, 1.43001, 1.6123, 1.75822, 1.84057, 1.93499, 1.96756, 1.9432, 1.66915, 1.25142, 0.823188, 0.474948, 0.232291, 0.101915};
const std::vector<double> ymax_xijet_back_pbpb_cent010 = {0.0721686, 0.174094, 0.324997, 0.525956, 0.751866, 0.994257, 1.25089, 1.47814, 1.64642, 1.78074, 1.89636, 1.96636, 2.01322, 1.98851, 1.7232, 1.27359, 0.847832, 0.492071, 0.243579, 0.111028};
const std::vector<double> y_xijet_back_pp_cent010 = {0.0574145, 0.153448, 0.274224, 0.46585, 0.712836, 0.991465, 1.29623, 1.57229, 1.87001, 2.05393, 2.20047, 2.28263, 2.17382, 2.00731, 1.66891, 1.26268, 0.839779, 0.466291, 0.229291, 0.104403};
const std::vector<double> yerr_xijet_back_pp_cent010 = {0.00205314, 0.00335651, 0.00448704, 0.00584831, 0.00723439, 0.00853191, 0.00975548, 0.0107442, 0.0117173, 0.01228, 0.0127106, 0.0129457, 0.0126334, 0.0121399, 0.0110694, 0.00962839, 0.00785217, 0.00585108, 0.00410299, 0.00276863};

const std::vector<double> x_xijet_back_cent1030 = {0.1275, 0.3775, 0.6275, 0.8775, 1.1275, 1.3775, 1.6275, 1.8775, 2.1275, 2.3775, 2.6275, 2.8775, 3.1275, 3.3775, 3.6275, 3.8775, 4.1275, 4.3775, 4.6275, 4.8775};
const std::vector<double> ymin_xijet_back_ratio_pbpb_pp_cent1030 = {1.0192, 1.10565, 1.07753, 1.07158, 1.01129, 0.987919, 0.953502, 0.933529, 0.880806, 0.863486, 0.866952, 0.865443, 0.900005, 0.954036, 0.990873, 0.97571, 0.979212, 0.986018, 1.02657, 0.975988};
const std::vector<double> ymax_xijet_back_ratio_pbpb_pp_cent1030 = {1.21763, 1.17589, 1.15066, 1.13137, 1.04024, 1.0235, 0.980982, 0.951415, 0.914997, 0.902898, 0.887666, 0.889113, 0.918816, 0.984067, 1.0102, 1.01836, 1.00565, 1.03134, 1.07793, 1.10569};
const std::vector<double> ymin_xijet_back_pbpb_cent1030 = {0.0612988, 0.165206, 0.307184, 0.508006, 0.71947, 0.984038, 1.23763, 1.47958, 1.6351, 1.78663, 1.90497, 1.95073, 1.9878, 1.92717, 1.66659, 1.23736, 0.823669, 0.468954, 0.23602, 0.103601};
const std::vector<double> ymax_xijet_back_pbpb_cent1030 = {0.0714103, 0.172349, 0.324606, 0.525396, 0.735384, 1.0124, 1.26349, 1.50233, 1.69699, 1.85006, 1.95355, 2.00431, 2.00926, 1.96919, 1.6886, 1.2837, 0.842419, 0.485631, 0.243325, 0.112012};
const std::vector<double> y_xijet_back_pp_cent1030 = {0.0590409, 0.144918, 0.286528, 0.465416, 0.707093, 0.990975, 1.2914, 1.58844, 1.85902, 2.05437, 2.19833, 2.26429, 2.18789, 2.0043, 1.69777, 1.25324, 0.845762, 0.473944, 0.229546, 0.102641};
const std::vector<double> yerr_xijet_back_pp_cent1030 = {0.00208351, 0.00326423, 0.00458989, 0.00584977, 0.00721036, 0.00853591, 0.00974426, 0.010807, 0.0116912, 0.0122902, 0.0127135, 0.0129028, 0.0126833, 0.0121395, 0.0111727, 0.00959921, 0.00788574, 0.00590313, 0.00410822, 0.00274713};

const std::vector<double> x_xijet_noback_cent010 = {0.1275, 0.3775, 0.6275, 0.8775, 1.1275, 1.3775, 1.6275, 1.8775, 2.1275, 2.3775, 2.6275, 2.8775, 3.1275, 3.3775, 3.6275, 3.8775, 4.1275, 4.3775, 4.6275, 4.8775};
const std::vector<double> ymin_xijet_noback_ratio_pbpb_pp_cent010 = {1.20734, 1.21062, 1.18181, 1.14208, 1.07451, 1.01725, 0.969792, 0.921457, 0.887938, 0.871712, 0.840247, 0.805935, 0.800188, 0.791806, 0.765525, 0.719655, 0.711934, 0.748924, 0.743819, 0.766343};
const std::vector<double> ymax_xijet_noback_ratio_pbpb_pp_cent010 = {1.37357, 1.29575, 1.23239, 1.19196, 1.11083, 1.05296, 1.01027, 0.951159, 0.908107, 0.886732, 0.855208, 0.832866, 0.823591, 0.811481, 0.782082, 0.752001, 0.747369, 0.777626, 0.793076, 0.847956};
const std::vector<double> ymin_xijet_noback_pbpb_cent010 = {0.0721551, 0.179377, 0.337779, 0.536991, 0.766864, 1.01412, 1.26642, 1.46839, 1.64487, 1.78961, 1.84452, 1.82868, 1.76416, 1.59337, 1.27278, 0.913037, 0.60172, 0.35131, 0.172761, 0.0787095};
const std::vector<double> ymax_xijet_noback_pbpb_cent010 = {0.076529, 0.191759, 0.348276, 0.558437, 0.782646, 1.03651, 1.28802, 1.50943, 1.68871, 1.82587, 1.88681, 1.88808, 1.79885, 1.62374, 1.30229, 0.941489, 0.621305, 0.366582, 0.179847, 0.0868258};
const std::vector<double> y_xijet_noback_pp_cent010 = {0.0565229, 0.149701, 0.283714, 0.474396, 0.704153, 0.995638, 1.28661, 1.58997, 1.85477, 2.04296, 2.21751, 2.26605, 2.18526, 2.01107, 1.6591, 1.25567, 0.838092, 0.463473, 0.232323, 0.102636};
const std::vector<double> yerr_xijet_noback_pp_cent010 = {0.00203562, 0.00331282, 0.00456063, 0.00589733, 0.00718486, 0.0085435, 0.00971199, 0.0107964, 0.0116608, 0.0122381, 0.0127502, 0.012889, 0.0126572, 0.0121422, 0.0110286, 0.00959452, 0.00783846, 0.00582904, 0.00412697, 0.00274305};

const std::vector<double> x_xijet_noback_cent1030 = {0.1275, 0.3775, 0.6275, 0.8775, 1.1275, 1.3775, 1.6275, 1.8775, 2.1275, 2.3775, 2.6275, 2.8775, 3.1275, 3.3775, 3.6275, 3.8775, 4.1275, 4.3775, 4.6275, 4.8775};
const std::vector<double> ymin_xijet_noback_ratio_pbpb_pp_cent1030 = {1.23663, 1.1961, 1.14515, 1.13472, 1.06063, 1.01762, 0.987095, 0.940399, 0.904504, 0.884424, 0.86058, 0.836885, 0.815211, 0.809196, 0.783079, 0.750221, 0.733215, 0.738581, 0.757021, 0.76616};
const std::vector<double> ymax_xijet_noback_ratio_pbpb_pp_cent1030 = {1.36795, 1.26808, 1.18621, 1.18756, 1.09053, 1.04164, 1.00522, 0.96959, 0.932321, 0.918651, 0.88918, 0.851989, 0.840622, 0.843762, 0.806957, 0.765773, 0.753131, 0.784937, 0.802937, 0.827042};
const std::vector<double> ymin_xijet_noback_pbpb_cent1030 = {0.0741574, 0.178978, 0.327003, 0.529899, 0.759538, 1.01471, 1.277, 1.49598, 1.68364, 1.82819, 1.90001, 1.89318, 1.78375, 1.62381, 1.32118, 0.948986, 0.614226, 0.349786, 0.173076, 0.0793648};
const std::vector<double> ymax_xijet_noback_pbpb_cent1030 = {0.0785621, 0.185547, 0.338025, 0.549233, 0.772822, 1.03726, 1.29584, 1.53296, 1.73337, 1.87853, 1.95241, 1.93497, 1.84358, 1.67116, 1.34662, 0.971283, 0.631128, 0.364137, 0.181487, 0.0857679};
const std::vector<double> y_xijet_noback_pp_cent1030 = {0.0600062, 0.148363, 0.281816, 0.464846, 0.709425, 0.9944, 1.30075, 1.57103, 1.85637, 2.05343, 2.19519, 2.27473, 2.20128, 1.99313, 1.66776, 1.27651, 0.838765, 0.472265, 0.232607, 0.100843};
const std::vector<double> yerr_xijet_noback_pp_cent1030 = {0.00209935, 0.00330103, 0.00454957, 0.00584308, 0.00721839, 0.00854609, 0.00977425, 0.0107419, 0.0116767, 0.0122808, 0.0126976, 0.0129256, 0.0127153, 0.0120992, 0.0110676, 0.00968276, 0.00784888, 0.00588952, 0.00413331, 0.00272151};

const int n_xigamma = 21;

const std::vector<double> x_xigamma_back_cent010 = {-0.1225, 0.1275, 0.3775, 0.6275, 0.8775, 1.1275, 1.3775, 1.6275, 1.8775, 2.1275, 2.3775, 2.6275, 2.8775, 3.1275, 3.3775, 3.6275, 3.8775, 4.1275, 4.3775, 4.6275, 4.8775};
const std::vector<double> ymin_xigamma_back_ratio_pbpb_pp_cent010 = {0.673659, 0.683894, 0.742717, 0.818388, 0.886424, 0.891427, 0.885037, 0.878223, 0.873402, 0.857526, 0.827266, 0.827339, 0.805151, 0.816, 0.827226, 0.91589, 1.03421, 1.22239, 1.41654, 1.57968, 1.76847};
const std::vector<double> ymax_xigamma_back_ratio_pbpb_pp_cent010 = {0.873439, 0.776548, 0.817682, 0.87961, 0.923941, 0.93606, 0.915492, 0.918062, 0.912597, 0.884161, 0.865214, 0.848659, 0.827546, 0.832074, 0.850992, 0.934507, 1.05857, 1.27884, 1.5379, 1.79038, 2.05059};
const std::vector<double> ymin_xigamma_back_pbpb_cent010 = {0.00643008, 0.0257817, 0.0734223, 0.153278, 0.283854, 0.460869, 0.661202, 0.887588, 1.11748, 1.34187, 1.51431, 1.67053, 1.77886, 1.83567, 1.85696, 1.88026, 1.86969, 1.72362, 1.00226, 0.485818, 0.221346};
const std::vector<double> ymax_xigamma_back_pbpb_cent010 = {0.00794535, 0.0287792, 0.0800094, 0.163657, 0.294198, 0.480212, 0.681018, 0.923227, 1.16306, 1.38132, 1.57866, 1.70941, 1.82298, 1.86804, 1.90612, 1.91274, 1.90414, 1.7939, 1.08331, 0.545506, 0.253463};
const std::vector<double> y_xigamma_back_pp_cent010 = {0.00932435, 0.0372974, 0.0985298, 0.186854, 0.319304, 0.515629, 0.745728, 1.00784, 1.277, 1.56392, 1.82992, 2.01523, 2.20818, 2.24952, 2.24196, 2.04871, 1.7982, 1.40387, 0.70263, 0.304473, 0.122244};
const std::vector<double> yerr_xigamma_back_pp_cent010 = {0.000827402, 0.0016548, 0.00268962, 0.0037039, 0.00484183, 0.00615285, 0.00739942, 0.00860207, 0.00968283, 0.0107156, 0.0115911, 0.0121638, 0.0127328, 0.0128515, 0.0128298, 0.0122645, 0.0114902, 0.0101524, 0.00718242, 0.00472805, 0.00299586};

const std::vector<double> x_xigamma_back_cent1030 = {-0.1225, 0.1275, 0.3775, 0.6275, 0.8775, 1.1275, 1.3775, 1.6275, 1.8775, 2.1275, 2.3775, 2.6275, 2.8775, 3.1275, 3.3775, 3.6275, 3.8775, 4.1275, 4.3775, 4.6275, 4.8775};
const std::vector<double> ymin_xigamma_back_ratio_pbpb_pp_cent1030 = {0.723931, 0.733597, 0.73198, 0.879048, 0.905012, 0.915778, 0.883072, 0.898924, 0.896048, 0.889728, 0.858096, 0.858111, 0.830968, 0.837492, 0.85248, 0.924141, 1.03485, 1.17369, 1.2859, 1.37092, 1.45253};
const std::vector<double> ymax_xigamma_back_ratio_pbpb_pp_cent1030 = {0.979116, 0.887627, 0.860539, 0.952921, 0.984506, 0.949887, 0.951017, 0.940422, 0.943405, 0.9166, 0.881069, 0.887388, 0.863751, 0.864481, 0.872942, 0.942611, 1.05825, 1.2418, 1.39799, 1.55995, 1.79764};
const std::vector<double> ymin_xigamma_back_pbpb_cent1030 = {0.00691376, 0.0278177, 0.0725209, 0.165617, 0.290441, 0.474116, 0.662485, 0.909243, 1.14765, 1.39574, 1.57334, 1.73314, 1.83439, 1.88408, 1.91018, 1.89517, 1.87335, 1.65591, 0.913044, 0.422524, 0.181452};
const std::vector<double> ymax_xigamma_back_pbpb_cent1030 = {0.00885146, 0.0329754, 0.0840271, 0.17702, 0.313244, 0.488494, 0.709964, 0.943305, 1.20515, 1.43339, 1.60753, 1.78922, 1.90418, 1.94118, 1.947, 1.9282, 1.90114, 1.74515, 0.981535, 0.472123, 0.219758};
const std::vector<double> y_xigamma_back_pp_cent1030 = {0.00919067, 0.0377185, 0.0981564, 0.186828, 0.319394, 0.516663, 0.746576, 1.00487, 1.28044, 1.56624, 1.83019, 2.01702, 2.20495, 2.24899, 2.24216, 2.04945, 1.80659, 1.40728, 0.706138, 0.306013, 0.123596};
const std::vector<double> yerr_xigamma_back_pp_cent1030 = {0.000822038, 0.00166531, 0.00268644, 0.00370629, 0.00484598, 0.00616343, 0.00740893, 0.00859555, 0.00970284, 0.0107312, 0.0116002, 0.0121779, 0.0127326, 0.0128592, 0.0128396, 0.0122754, 0.0115252, 0.010172, 0.00720548, 0.00474338, 0.00301454};

const std::vector<double> x_xigamma_noback_cent010 = {-0.1225, 0.1275, 0.3775, 0.6275, 0.8775, 1.1275, 1.3775, 1.6275, 1.8775, 2.1275, 2.3775, 2.6275, 2.8775, 3.1275, 3.3775, 3.6275, 3.8775, 4.1275, 4.3775, 4.6275, 4.8775};
const std::vector<double> ymin_xigamma_noback_ratio_pbpb_pp_cent010 = {1.01233, 0.871825, 0.874125, 0.917256, 0.956263, 0.958277, 0.933759, 0.942572, 0.91978, 0.898719, 0.866243, 0.857354, 0.831524, 0.828203, 0.803715, 0.823217, 0.811258, 0.823502, 0.880623, 0.944743, 1.02553};
const std::vector<double> ymax_xigamma_noback_ratio_pbpb_pp_cent010 = {1.32794, 1.00083, 0.934116, 0.965747, 1.00182, 0.99028, 0.965174, 0.964213, 0.946213, 0.918039, 0.896606, 0.878991, 0.851464, 0.840892, 0.825789, 0.838489, 0.833451, 0.848497, 0.93098, 0.983291, 1.10247};
const std::vector<double> ymin_xigamma_noback_pbpb_cent010 = {0.00972535, 0.0331995, 0.0866033, 0.17233, 0.307051, 0.494391, 0.699277, 0.949873, 1.17992, 1.40824, 1.58841, 1.73246, 1.83572, 1.86231, 1.80403, 1.68618, 1.46587, 1.16123, 0.621978, 0.288424, 0.127257};
const std::vector<double> ymax_xigamma_noback_pbpb_cent010 = {0.0120639, 0.0371684, 0.091387, 0.179329, 0.319252, 0.507691, 0.717431, 0.968983, 1.20748, 1.43378, 1.63558, 1.76798, 1.87412, 1.88429, 1.84817, 1.71477, 1.50277, 1.19171, 0.655787, 0.298182, 0.135154};
const std::vector<double> y_xigamma_noback_pp_cent010 = {0.00938382, 0.0378285, 0.0980902, 0.186723, 0.319563, 0.512664, 0.745574, 1.0037, 1.27957, 1.56409, 1.82581, 2.01232, 2.20534, 2.24332, 2.24156, 2.0478, 1.80375, 1.40669, 0.703127, 0.303581, 0.122943};
const std::vector<double> yerr_xigamma_noback_pp_cent010 = {0.00082942, 0.00166531, 0.00268162, 0.00369985, 0.0048402, 0.00613058, 0.00739316, 0.00857802, 0.00968539, 0.0107082, 0.0115695, 0.012146, 0.0127152, 0.0128242, 0.0128192, 0.0122526, 0.0114993, 0.0101551, 0.00717962, 0.00471761, 0.00300218};

const std::vector<double> x_xigamma_noback_cent1030 = {-0.1225, 0.1275, 0.3775, 0.6275, 0.8775, 1.1275, 1.3775, 1.6275, 1.8775, 2.1275, 2.3775, 2.6275, 2.8775, 3.1275, 3.3775, 3.6275, 3.8775, 4.1275, 4.3775, 4.6275, 4.8775};
const std::vector<double> ymin_xigamma_noback_ratio_pbpb_pp_cent1030 = {1.00621, 0.856508, 0.824712, 0.952827, 0.964489, 0.948795, 0.929947, 0.93754, 0.932929, 0.922375, 0.884262, 0.883035, 0.850237, 0.846428, 0.826965, 0.835876, 0.837851, 0.845035, 0.86351, 0.913438, 0.910796};
const std::vector<double> ymax_xigamma_noback_ratio_pbpb_pp_cent1030 = {1.22405, 0.983816, 0.926992, 1.00996, 1.02674, 0.999765, 0.985628, 0.968795, 0.968839, 0.941599, 0.903781, 0.909175, 0.881837, 0.867914, 0.841416, 0.856617, 0.856091, 0.865769, 0.895976, 0.956322, 1.04946};
const std::vector<double> ymin_xigamma_noback_pbpb_cent1030 = {0.00959412, 0.0325402, 0.0814381, 0.178455, 0.308633, 0.490313, 0.695838, 0.946663, 1.19444, 1.44666, 1.62078, 1.78495, 1.87955, 1.90394, 1.85615, 1.71346, 1.51493, 1.18932, 0.610169, 0.280251, 0.11393};
const std::vector<double> ymax_xigamma_noback_pbpb_cent1030 = {0.011093, 0.036534, 0.0904273, 0.187734, 0.326946, 0.513378, 0.735735, 0.973054, 1.23889, 1.46975, 1.65056, 1.83167, 1.93808, 1.94789, 1.88465, 1.75634, 1.54044, 1.21336, 0.628661, 0.290002, 0.128445};
const std::vector<double> y_xigamma_noback_pp_cent1030 = {0.00925433, 0.037458, 0.0979784, 0.186629, 0.319495, 0.51648, 0.747397, 1.00512, 1.28209, 1.5631, 1.82655, 2.01664, 2.2065, 2.24498, 2.24469, 2.05152, 1.79901, 1.40695, 0.703109, 0.304805, 0.124272};
const std::vector<double> yerr_xigamma_noback_pp_cent1030 = {0.000824441, 0.00165867, 0.00268258, 0.00370234, 0.00484417, 0.00615905, 0.00740906, 0.00859205, 0.00970391, 0.0107147, 0.0115825, 0.0121703, 0.0127303, 0.0128408, 0.01284, 0.0122751, 0.0114949, 0.0101655, 0.00718619, 0.0047315, 0.00302117};

enum DATATABLES{
    k_xijet_back_ratio_pbpb_pp_cent010,
    k_xijet_back_pbpb_cent010,
    k_xijet_back_pp_cent010,
    k_xijet_back_ratio_pbpb_pp_cent1030,
    k_xijet_back_pbpb_cent1030,
    k_xijet_back_pp_cent1030,
    k_xijet_noback_ratio_pbpb_pp_cent010,
    k_xijet_noback_pbpb_cent010,
    k_xijet_noback_pp_cent010,
    k_xijet_noback_ratio_pbpb_pp_cent1030,
    k_xijet_noback_pbpb_cent1030,
    k_xijet_noback_pp_cent1030,
    k_xigamma_back_ratio_pbpb_pp_cent010,
    k_xigamma_back_pbpb_cent010,
    k_xigamma_back_pp_cent010,
    k_xigamma_back_ratio_pbpb_pp_cent1030,
    k_xigamma_back_pbpb_cent1030,
    k_xigamma_back_pp_cent1030,
    k_xigamma_noback_ratio_pbpb_pp_cent010,
    k_xigamma_noback_pbpb_cent010,
    k_xigamma_noback_pp_cent010,
    k_xigamma_noback_ratio_pbpb_pp_cent1030,
    k_xigamma_noback_pbpb_cent1030,
    k_xigamma_noback_pp_cent1030,
    kN_DATATABLES
};

const std::string dataNames[kN_DATATABLES] = {
        "xijet_back_ratio_pbpb_pp_cent010",
        "xijet_back_pbpb_cent010",
        "xijet_back_pp_cent010",
        "xijet_back_ratio_pbpb_pp_cent1030",
        "xijet_back_pbpb_cent1030",
        "xijet_back_pp_cent1030",
        "xijet_noback_ratio_pbpb_pp_cent010",
        "xijet_noback_pbpb_cent010",
        "xijet_noback_pp_cent010",
        "xijet_noback_ratio_pbpb_pp_cent1030",
        "xijet_noback_pbpb_cent1030",
        "xijet_noback_pp_cent1030",
        "xigamma_back_ratio_pbpb_pp_cent010",
        "xigamma_back_pbpb_cent010",
        "xigamma_back_pp_cent010",
        "xigamma_back_ratio_pbpb_pp_cent1030",
        "xigamma_back_pbpb_cent1030",
        "xigamma_back_pp_cent1030",
        "xigamma_noback_ratio_pbpb_pp_cent010",
        "xigamma_noback_pbpb_cent010",
        "xigamma_noback_pp_cent010",
        "xigamma_noback_ratio_pbpb_pp_cent1030",
        "xigamma_noback_pbpb_cent1030",
        "xigamma_noback_pp_cent1030"
};

const std::vector<double> xData[kN_DATATABLES] = {
        x_xijet_back_cent010,
        x_xijet_back_cent010,
        x_xijet_back_cent010,
        x_xijet_back_cent1030,
        x_xijet_back_cent1030,
        x_xijet_back_cent1030,
        x_xijet_noback_cent010,
        x_xijet_noback_cent010,
        x_xijet_noback_cent010,
        x_xijet_noback_cent1030,
        x_xijet_noback_cent1030,
        x_xijet_noback_cent1030,
        x_xigamma_back_cent010,
        x_xigamma_back_cent010,
        x_xigamma_back_cent010,
        x_xigamma_back_cent1030,
        x_xigamma_back_cent1030,
        x_xigamma_back_cent1030,
        x_xigamma_noback_cent010,
        x_xigamma_noback_cent010,
        x_xigamma_noback_cent010,
        x_xigamma_noback_cent1030,
        x_xigamma_noback_cent1030,
        x_xigamma_noback_cent1030
};

const std::vector<double> yminData[kN_DATATABLES] = {
        ymin_xijet_back_ratio_pbpb_pp_cent010,
        ymin_xijet_back_pbpb_cent010,
        {},
        ymin_xijet_back_ratio_pbpb_pp_cent1030,
        ymin_xijet_back_pbpb_cent1030,
        {},
        ymin_xijet_noback_ratio_pbpb_pp_cent010,
        ymin_xijet_noback_pbpb_cent010,
        {},
        ymin_xijet_noback_ratio_pbpb_pp_cent1030,
        ymin_xijet_noback_pbpb_cent1030,
        {},
        ymin_xigamma_back_ratio_pbpb_pp_cent010,
        ymin_xigamma_back_pbpb_cent010,
        {},
        ymin_xigamma_back_ratio_pbpb_pp_cent1030,
        ymin_xigamma_back_pbpb_cent1030,
        {},
        ymin_xigamma_noback_ratio_pbpb_pp_cent010,
        ymin_xigamma_noback_pbpb_cent010,
        {},
        ymin_xigamma_noback_ratio_pbpb_pp_cent1030,
        ymin_xigamma_noback_pbpb_cent1030,
        {},
};

const std::vector<double> ymaxData[kN_DATATABLES] = {
        ymax_xijet_back_ratio_pbpb_pp_cent010,
        ymax_xijet_back_pbpb_cent010,
        {},
        ymax_xijet_back_ratio_pbpb_pp_cent1030,
        ymax_xijet_back_pbpb_cent1030,
        {},
        ymax_xijet_noback_ratio_pbpb_pp_cent010,
        ymax_xijet_noback_pbpb_cent010,
        {},
        ymax_xijet_noback_ratio_pbpb_pp_cent1030,
        ymax_xijet_noback_pbpb_cent1030,
        {},
        ymax_xigamma_back_ratio_pbpb_pp_cent010,
        ymax_xigamma_back_pbpb_cent010,
        {},
        ymax_xigamma_back_ratio_pbpb_pp_cent1030,
        ymax_xigamma_back_pbpb_cent1030,
        {},
        ymax_xigamma_noback_ratio_pbpb_pp_cent010,
        ymax_xigamma_noback_pbpb_cent010,
        {},
        ymax_xigamma_noback_ratio_pbpb_pp_cent1030,
        ymax_xigamma_noback_pbpb_cent1030,
        {},
};

const std::vector<double> yData[kN_DATATABLES] = {
        {},
        {},
        y_xijet_back_pp_cent010,
        {},
        {},
        y_xijet_back_pp_cent1030,
        {},
        {},
        y_xijet_noback_pp_cent010,
        {},
        {},
        y_xijet_noback_pp_cent1030,
        {},
        {},
        y_xigamma_back_pp_cent010,
        {},
        {},
        y_xigamma_back_pp_cent1030,
        {},
        {},
        y_xigamma_noback_pp_cent010,
        {},
        {},
        y_xigamma_noback_pp_cent1030,
};

const std::vector<double> yerrData[kN_DATATABLES] = {
        {},
        {},
        yerr_xijet_back_pp_cent010,
        {},
        {},
        yerr_xijet_back_pp_cent1030,
        {},
        {},
        yerr_xijet_noback_pp_cent010,
        {},
        {},
        yerr_xijet_noback_pp_cent1030,
        {},
        {},
        yerr_xigamma_back_pp_cent010,
        {},
        {},
        yerr_xigamma_back_pp_cent1030,
        {},
        {},
        yerr_xigamma_noback_pp_cent010,
        {},
        {},
        yerr_xigamma_noback_pp_cent1030,
};

const std::string xTitles[kN_DATATABLES] = {
        "#xi^{jet}",
        "#xi^{jet}",
        "#xi^{jet}",
        "#xi^{jet}",
        "#xi^{jet}",
        "#xi^{jet}",
        "#xi^{jet}",
        "#xi^{jet}",
        "#xi^{jet}",
        "#xi^{jet}",
        "#xi^{jet}",
        "#xi^{jet}",
        "#xi^{#gamma}_{T}",
        "#xi^{#gamma}_{T}",
        "#xi^{#gamma}_{T}",
        "#xi^{#gamma}_{T}",
        "#xi^{#gamma}_{T}",
        "#xi^{#gamma}_{T}",
        "#xi^{#gamma}_{T}",
        "#xi^{#gamma}_{T}",
        "#xi^{#gamma}_{T}",
        "#xi^{#gamma}_{T}",
        "#xi^{#gamma}_{T}",
        "#xi^{#gamma}_{T}"
};

const std::string yTitles[kN_DATATABLES] = {
        "PbPb / pp",
        "FF",
        "FF",
        "PbPb / pp",
        "FF",
        "FF",
        "PbPb / pp",
        "FF",
        "FF",
        "PbPb / pp",
        "FF",
        "FF",
        "PbPb / pp",
        "FF",
        "FF",
        "PbPb / pp",
        "FF",
        "FF",
        "PbPb / pp",
        "FF",
        "FF",
        "PbPb / pp",
        "FF",
        "FF"
};

const std::string titles[kN_DATATABLES] = {
        "Hybrid, back reaction, 0-10%",
        "Hybrid, back reaction, PbPb 0-10%",
        "Hybrid, back reaction, pp reference for 0-10%",
        "Hybrid, back reaction, 10-30%",
        "Hybrid, back reaction, PbPb 10-30%",
        "Hybrid, back reaction, pp reference for 10-30%",
        "Hybrid, no back reaction, 0-10%",
        "Hybrid, no back reaction, PbPb 0-10%",
        "Hybrid, no back reaction, pp reference for 0-10%",
        "Hybrid, no back reaction, 10-30%",
        "Hybrid, no back reaction, PbPb 10-30%",
        "Hybrid, no back reaction, pp reference for 10-30%",
        "Hybrid, back reaction, 0-10%",
        "Hybrid, back reaction, PbPb 0-10%",
        "Hybrid, back reaction, pp reference for 0-10%",
        "Hybrid, back reaction, 10-30%",
        "Hybrid, back reaction, PbPb 10-30%",
        "Hybrid, back reaction, pp reference for 10-30%",
        "Hybrid, no back reaction, 0-10%",
        "Hybrid, no back reaction, PbPb 0-10%",
        "Hybrid, no back reaction, pp reference for 0-10%",
        "Hybrid, no back reaction, 10-30%",
        "Hybrid, no back reaction, PbPb 10-30%",
        "Hybrid, no back reaction, pp reference for 10-30%"
};

const int colors[kN_DATATABLES] = {
        TColor::GetColor("#D19019"),
        TColor::GetColor("#32C1C1"),
        TColor::GetColor("#D19019"),
        TColor::GetColor("#D19019"),
        TColor::GetColor("#32C1C1"),
        TColor::GetColor("#D19019"),
        kMagenta,
        TColor::GetColor("#873DD1"),
        kMagenta,
        kMagenta,
        TColor::GetColor("#873DD1"),
        kMagenta,
        TColor::GetColor("#D19019"),
        TColor::GetColor("#32C1C1"),
        TColor::GetColor("#D19019"),
        TColor::GetColor("#D19019"),
        TColor::GetColor("#32C1C1"),
        TColor::GetColor("#D19019"),
        kMagenta,
        TColor::GetColor("#873DD1"),
        kMagenta,
        kMagenta,
        TColor::GetColor("#873DD1"),
        kMagenta
};

const double falphas[kN_DATATABLES] = {
        0.8,
        0.8,
        0.8,
        0.8,
        0.8,
        0.8,
        0.7,
        0.85,
        0.7,
        0.7,
        0.85,
        0.7,
        0.8,
        0.8,
        0.8,
        0.8,
        0.8,
        0.8,
        0.7,
        0.85,
        0.7,
        0.7,
        0.85,
        0.7
};

};

#endif /* DATA_PHOTONJETFF_PHOTONJETFFDATA_H_ */
