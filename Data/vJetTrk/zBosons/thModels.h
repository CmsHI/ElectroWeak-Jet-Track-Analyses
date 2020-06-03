/*
 * file with hard coded results for different Z+hadron studies
 */

#ifndef DATA_VJETTRK_ZBOSONS_THMODELS_H_
#define DATA_VJETTRK_ZBOSONS_THMODELS_H_

#include <vector>
#include <string>
#include <TColor.h>


namespace HYBRID {

/*
 * Hybrid Z+hadron predictions data points
 * naming convention :
 *      n_<observable>    : number of points in the observable
 *      x_<observable>    : x axis values of the observable, bin centers
 *      y_<observable>    : y axis values of the observable
 */

// xi points
const int n_xi = 10;
const std::vector<double> x_xi = {0.25, 0.75, 1.25, 1.75, 2.25, 2.75, 3.25, 3.75, 4.25, 4.75};

//// cent030
const std::vector<double> y_min_xi_aa_pp_ratio_cent030_no_wake = {0.432889, 0.485254, 0.495774, 0.47226, 0.435216, 0.399367, 0.381226, 0.40599, 0.451366, 0.486786};
const std::vector<double> y_max_xi_aa_pp_ratio_cent030_no_wake = {0.507844, 0.527237, 0.545457, 0.510995, 0.475182, 0.440112, 0.413096, 0.434337, 0.477063, 0.506904};

const std::vector<double> y_min_xi_aa_pp_ratio_cent030_pos_wake = {0.432889, 0.485254, 0.495774, 0.47226, 0.452001, 0.603943, 1.15331, 1.73832, 2.24633, 2.71614};
const std::vector<double> y_max_xi_aa_pp_ratio_cent030_pos_wake = {0.507844, 0.527237, 0.545457, 0.510997, 0.491955, 0.63626, 1.18635, 1.80137, 2.34246, 2.85301};

const std::vector<double> y_min_xi_aa_pp_ratio_cent030_full_wake = {0.432889, 0.485254, 0.495774, 0.472257, 0.451788, 0.602257, 1.14484, 1.72315, 2.22523, 2.68894};
const std::vector<double> y_max_xi_aa_pp_ratio_cent030_full_wake = {0.507844, 0.527237, 0.545457, 0.510984, 0.491786, 0.634637, 1.1778, 1.78622, 2.32086, 2.82501};

// cent3050
const std::vector<double> y_min_xi_aa_pp_ratio_cent3050_no_wake = {0.758486, 0.77412, 0.786853, 0.734265, 0.694503, 0.642606, 0.619151, 0.617545, 0.62748, 0.619458};
const std::vector<double> y_max_xi_aa_pp_ratio_cent3050_no_wake = {0.83066, 0.837664, 0.810637, 0.762658, 0.715603, 0.671315, 0.63721, 0.64142, 0.644371, 0.634222};

const std::vector<double> y_min_xi_aa_pp_ratio_cent3050_pos_wake = {0.758486, 0.77412, 0.786853, 0.734265, 0.704895, 0.751214, 1.04658, 1.33934, 1.50334, 1.63926};
const std::vector<double> y_max_xi_aa_pp_ratio_cent3050_pos_wake = {0.83066, 0.837664, 0.810686, 0.762664, 0.723301, 0.776672, 1.08118, 1.36806, 1.54705, 1.69439};

const std::vector<double> y_min_xi_aa_pp_ratio_cent3050_full_wake = {0.758486, 0.77412, 0.786853, 0.734265, 0.704788, 0.750031, 1.04094, 1.32938, 1.48961, 1.62256};
const std::vector<double> y_max_xi_aa_pp_ratio_cent3050_full_wake = {0.83066, 0.837664, 0.810686, 0.762645, 0.723241, 0.775671, 1.07525, 1.35744, 1.53278, 1.67733};

// trk pT points
const int n_trkPt = 20;
const std::vector<double> x_trkPt = {1.05, 1.15, 1.3, 1.5, 1.7, 1.9, 2.1, 2.3, 2.8, 3.6, 4.4, 5.2, 6, 6.8, 8.4, 10.8, 13.2, 16.8, 21.6, 27};

//// cent030
const std::vector<double> y_min_trkPt_aa_pp_ratio_cent030_no_wake = {0.36701, 0.354228, 0.356566, 0.358742, 0.369664, 0.367781, 0.370008, 0.37464, 0.390088, 0.40832, 0.427545, 0.44653, 0.460527, 0.486871, 0.505991, 0.527594, 0.551633, 0.569145, 0.595373, 0.630736};
const std::vector<double> y_max_trkPt_aa_pp_ratio_cent030_no_wake = {0.40126, 0.386003, 0.390866, 0.393515, 0.408717, 0.408883, 0.410555, 0.419025, 0.42565, 0.449088, 0.465797, 0.480557, 0.501185, 0.529658, 0.546746, 0.57545, 0.594726, 0.60906, 0.641582, 0.677692};

const std::vector<double> y_min_trkPt_aa_pp_ratio_cent030_pos_wake = {2.88123, 2.56169, 2.2382, 1.78216, 1.48687, 1.18685, 0.985063, 0.822538, 0.623772, 0.444469, 0.427573, 0.44654, 0.460537, 0.486895, 0.505991, 0.527594, 0.551633, 0.569145, 0.595373, 0.630736};
const std::vector<double> y_max_trkPt_aa_pp_ratio_cent030_pos_wake = {3.08138, 2.72018, 2.36359, 1.89292, 1.55471, 1.23962, 1.01801, 0.852505, 0.651562, 0.483672, 0.465799, 0.480562, 0.501217, 0.529662, 0.546749, 0.57545, 0.594727, 0.60906, 0.641582, 0.677692};

const std::vector<double> y_min_trkPt_aa_pp_ratio_cent030_full_wake = {2.85116, 2.53563, 2.21619, 1.76621, 1.47454, 1.17869, 0.978973, 0.818094, 0.621467, 0.444059, 0.427554, 0.446539, 0.460522, 0.486894, 0.505991, 0.527594, 0.551633, 0.569145, 0.595373, 0.630736};
const std::vector<double> y_max_trkPt_aa_pp_ratio_cent030_full_wake = {3.05114, 2.69408, 2.34089, 1.87599, 1.54215, 1.23117, 1.0119, 0.848237, 0.649327, 0.483358, 0.465794, 0.48056, 0.50119, 0.529661, 0.546746, 0.57545, 0.594727, 0.60906, 0.641582, 0.677692};

//// cent3050
const std::vector<double> y_min_trkPt_aa_pp_ratio_cent3050_no_wake = {0.582511, 0.574843, 0.581398, 0.58706, 0.606449, 0.593785, 0.599674, 0.610775, 0.623795, 0.648487, 0.666548, 0.68586, 0.697687, 0.740443, 0.758784, 0.78182, 0.790877, 0.801103, 0.824756, 0.841196};
const std::vector<double> y_max_trkPt_aa_pp_ratio_cent3050_no_wake = {0.615986, 0.597823, 0.613689, 0.604582, 0.6394, 0.621998, 0.639959, 0.647091, 0.646982, 0.683875, 0.691907, 0.716228, 0.725121, 0.77168, 0.784323, 0.811032, 0.827759, 0.830388, 0.861129, 0.884908};

const std::vector<double> y_min_trkPt_aa_pp_ratio_cent3050_pos_wake = {1.97166, 1.7681, 1.6019, 1.32079, 1.17111, 0.985164, 0.890224, 0.828064, 0.731555, 0.664979, 0.666632, 0.685867, 0.697688, 0.740508, 0.758789, 0.78182, 0.790878, 0.801103, 0.824756, 0.841196};
const std::vector<double> y_max_trkPt_aa_pp_ratio_cent3050_pos_wake = {2.06919, 1.89215, 1.66108, 1.39958, 1.22753, 1.03323, 0.933431, 0.854634, 0.747006, 0.699034, 0.691923, 0.716233, 0.725125, 0.771686, 0.784336, 0.811032, 0.827859, 0.830388, 0.861129, 0.884908};

const std::vector<double> y_min_trkPt_aa_pp_ratio_cent3050_full_wake = {1.95097, 1.75024, 1.58763, 1.3104, 1.16336, 0.980326, 0.886379, 0.825476, 0.730048, 0.664811, 0.666604, 0.685864, 0.697615, 0.740508, 0.758783, 0.781819, 0.790878, 0.801103, 0.824756, 0.841196};
const std::vector<double> y_max_trkPt_aa_pp_ratio_cent3050_full_wake = {2.04711, 1.87365, 1.64617, 1.38804, 1.2197, 1.02756, 0.929824, 0.851823, 0.745804, 0.698858, 0.691911, 0.716228, 0.725079, 0.771686, 0.784336, 0.811031, 0.827859, 0.830388, 0.861129, 0.884908};

// dphi points
const int n_dphi = 20;
const std::vector<double> x_dphi = {0.0785398, 0.235619, 0.392699, 0.549779, 0.706858, 0.863938, 1.02102, 1.1781, 1.33518, 1.49226, 1.64934, 1.80642, 1.9635, 2.12058, 2.27765, 2.43473, 2.59181, 2.74889, 2.90597, 3.06305};

//// cent030
const std::vector<double> y_min_dphi_aa_pp_ratio_cent030_no_wake = {0.200728, 0.201245, 0.193496, 0.197357, 0.198233, 0.196249, 0.188832, 0.193483, 0.195911, 0.193576, 0.202215, 0.199478, 0.215053, 0.225865, 0.255091, 0.284052, 0.3232, 0.363229, 0.401568, 0.446337};
const std::vector<double> y_max_dphi_aa_pp_ratio_cent030_no_wake = {0.216835, 0.215122, 0.209806, 0.219825, 0.221132, 0.221843, 0.214681, 0.212213, 0.219659, 0.213509, 0.22272, 0.219031, 0.236747, 0.256119, 0.285537, 0.31752, 0.353737, 0.392948, 0.435006, 0.478811};

const std::vector<double> y_min_dphi_aa_pp_ratio_cent030_pos_wake = {1.56171, 1.56327, 1.52032, 1.59063, 1.60272, 1.63414, 1.63985, 1.75892, 1.88736, 1.95345, 2.11414, 2.15069, 2.19256, 2.11952, 2.03635, 1.82266, 1.63181, 1.39121, 1.1558, 0.997163};
const std::vector<double> y_max_dphi_aa_pp_ratio_cent030_pos_wake = {1.60738, 1.60723, 1.58711, 1.63214, 1.66824, 1.67642, 1.69678, 1.81254, 1.97215, 2.05652, 2.21919, 2.27544, 2.33104, 2.24413, 2.14316, 1.90528, 1.6922, 1.43793, 1.18176, 1.00486};

const std::vector<double> y_min_dphi_aa_pp_ratio_cent030_full_wake = {-0.876205, -0.734058, -0.408955, -0.0694816, 0.352216, 0.724818, 1.04562, 1.35188, 1.61044, 1.76678, 1.97012, 2.0469, 2.1164, 2.06342, 1.99649, 1.79469, 1.61211, 1.37829, 1.14731, 0.991494};
const std::vector<double> y_max_dphi_aa_pp_ratio_cent030_full_wake = {-0.667636, -0.538743, -0.269896, 0.0587939, 0.410313, 0.794861, 1.09344, 1.4113, 1.67617, 1.85675, 2.07246, 2.16794, 2.25001, 2.1853, 2.10133, 1.8766, 1.67237, 1.42448, 1.1734, 0.998964};

const std::vector<double> y_min_dphi_aa_pp_diff_cent030_no_wake = {-0.546513, -0.550652, -0.585154, -0.579854, -0.606703, -0.647788, -0.721192, -0.761017, -0.809351, -0.919395, -1.0032, -1.17291, -1.35646, -1.68433, -2.07211, -2.73861, -3.55925, -4.82445, -6.80715, -9.18051};
const std::vector<double> y_max_dphi_aa_pp_diff_cent030_no_wake = {-0.526206, -0.531114, -0.562113, -0.55396, -0.578636, -0.614871, -0.68248, -0.728483, -0.771281, -0.879205, -0.959818, -1.12317, -1.29601, -1.58974, -1.95655, -2.57238, -3.3588, -4.5544, -6.37986, -8.59366};

const std::vector<double> y_min_dphi_aa_pp_diff_cent030_pos_wake = {0.383968, 0.388287, 0.377598, 0.426677, 0.456002, 0.510937, 0.568761, 0.715811, 0.892868, 1.08674, 1.40052, 1.68567, 2.06141, 2.43596, 2.88241, 3.14676, 3.32258, 2.96382, 1.77216, -0.046883};
const std::vector<double> y_max_dphi_aa_pp_diff_cent030_pos_wake = {0.408419, 0.411254, 0.418118, 0.449149, 0.496895, 0.535045, 0.60602, 0.752144, 0.961141, 1.18202, 1.50652, 1.83516, 2.26103, 2.65903, 3.13142, 3.41273, 3.59841, 3.28627, 2.05291, 0.0803762};

const std::vector<double> y_min_dphi_aa_pp_diff_cent030_full_wake = {-1.27241, -1.18559, -1.01472, -0.768356, -0.488088, -0.221095, 0.0406252, 0.33168, 0.613989, 0.87382, 1.2193, 1.5335, 1.92965, 2.3138, 2.77151, 3.03975, 3.21895, 2.86594, 1.67558, -0.140877};
const std::vector<double> y_max_dphi_aa_pp_diff_cent030_full_wake = {-1.12951, -1.04962, -0.910041, -0.671768, -0.439968, -0.162494, 0.0814894, 0.381053, 0.668927, 0.958812, 1.32542, 1.68064, 2.12352, 2.5334, 3.0169, 3.30468, 3.49537, 3.18537, 1.95846, -0.0169276};

//// cent3050
const std::vector<double> y_min_dphi_aa_pp_ratio_cent3050_no_wake = {0.387713, 0.383107, 0.371492, 0.392603, 0.383527, 0.386027, 0.367337, 0.371082, 0.391683, 0.389157, 0.394791, 0.399695, 0.418237, 0.429785, 0.472389, 0.505065, 0.54997, 0.594608, 0.641519, 0.67893};
const std::vector<double> y_max_dphi_aa_pp_ratio_cent3050_no_wake = {0.404499, 0.417159, 0.407787, 0.413204, 0.415066, 0.405301, 0.402415, 0.399829, 0.41014, 0.417872, 0.4322, 0.423083, 0.44504, 0.46239, 0.496377, 0.529409, 0.56954, 0.622962, 0.658406, 0.697925};

const std::vector<double> y_min_dphi_aa_pp_ratio_cent3050_pos_wake = {1.3562, 1.36507, 1.34419, 1.40002, 1.39917, 1.37391, 1.36024, 1.41911, 1.49635, 1.49466, 1.57471, 1.5742, 1.55888, 1.49865, 1.43716, 1.33448, 1.23467, 1.1389, 1.03225, 0.966543};
const std::vector<double> y_max_dphi_aa_pp_ratio_cent3050_pos_wake = {1.42243, 1.42594, 1.38512, 1.44135, 1.43765, 1.44241, 1.42049, 1.46255, 1.54449, 1.57533, 1.62717, 1.62487, 1.641, 1.56303, 1.50515, 1.38667, 1.28883, 1.16718, 1.05536, 0.974688};

const std::vector<double> y_min_dphi_aa_pp_ratio_cent3050_full_wake = {0.325424, 0.401643, 0.479639, 0.628394, 0.799606, 0.937679, 1.06053, 1.19455, 1.3336, 1.37267, 1.48603, 1.50638, 1.5094, 1.46111, 1.41058, 1.31634, 1.22196, 1.13054, 1.02687, 0.962549};
const std::vector<double> y_max_dphi_aa_pp_ratio_cent3050_full_wake = {0.393202, 0.454798, 0.568776, 0.735066, 0.876705, 0.981418, 1.09887, 1.2305, 1.37436, 1.45373, 1.52934, 1.55268, 1.58742, 1.52427, 1.47614, 1.36701, 1.27545, 1.15826, 1.04962, 0.970944};

const std::vector<double> y_min_dphi_aa_pp_diff_cent3050_no_wake = {-0.418476, -0.425049, -0.455853, -0.438435, -0.466194, -0.49431, -0.562078, -0.592986, -0.61177, -0.695674, -0.760443, -0.878857, -1.00492, -1.23986, -1.46648, -1.89188, -2.36535, -3.07011, -4.07624, -5.32244};
const std::vector<double> y_max_dphi_aa_pp_diff_cent3050_no_wake = {-0.400329, -0.394753, -0.42165, -0.416797, -0.434845, -0.47016, -0.519612, -0.5553, -0.583131, -0.651219, -0.701681, -0.830097, -0.942892, -1.1494, -1.38009, -1.77453, -2.23815, -2.82986, -3.85837, -4.98183};

const std::vector<double> y_min_dphi_aa_pp_diff_cent3050_pos_wake = {0.243387, 0.251441, 0.24956, 0.288671, 0.301685, 0.301043, 0.320018, 0.395134, 0.499069, 0.563402, 0.721807, 0.84046, 0.965314, 1.08424, 1.21519, 1.27878, 1.2336, 1.05205, 0.366838, -0.554434};
const std::vector<double> y_max_dphi_aa_pp_diff_cent3050_pos_wake = {0.284318, 0.288841, 0.274453, 0.313925, 0.325769, 0.350284, 0.366094, 0.428506, 0.538884, 0.644324, 0.775716, 0.899971, 1.08987, 1.20443, 1.38486, 1.45877, 1.50239, 1.25521, 0.625589, -0.417357};

const std::vector<double> y_min_dphi_aa_pp_diff_cent3050_full_wake = {-0.458944, -0.410319, -0.375812, -0.267307, -0.151028, -0.0499858, 0.0538353, 0.183367, 0.335313, 0.424349, 0.610355, 0.741115, 0.879808, 1.00259, 1.14129, 1.20941, 1.16675, 0.988667, 0.305639, -0.620633};
const std::vector<double> y_max_dphi_aa_pp_diff_cent3050_full_wake = {-0.409746, -0.37082, -0.308165, -0.188799, -0.091851, -0.0146408, 0.0862713, 0.213749, 0.37069, 0.508309, 0.654861, 0.796117, 0.998875, 1.12157, 1.30538, 1.38462, 1.43282, 1.18825, 0.560782, -0.479114};

enum MODEL{
    k_xi_aa_pp_ratio_cent030_no_wake,
    k_xi_aa_pp_ratio_cent030_pos_wake,
    k_xi_aa_pp_ratio_cent030_full_wake,
    k_xi_aa_pp_ratio_cent3050_no_wake,
    k_xi_aa_pp_ratio_cent3050_pos_wake,
    k_xi_aa_pp_ratio_cent3050_full_wake,
    //
    k_trkPt_aa_pp_ratio_cent030_no_wake,
    k_trkPt_aa_pp_ratio_cent030_pos_wake,
    k_trkPt_aa_pp_ratio_cent030_full_wake,
    k_trkPt_aa_pp_ratio_cent3050_no_wake,
    k_trkPt_aa_pp_ratio_cent3050_pos_wake,
    k_trkPt_aa_pp_ratio_cent3050_full_wake,
    //
    k_dphi_aa_pp_ratio_cent030_no_wake,
    k_dphi_aa_pp_ratio_cent030_pos_wake,
    k_dphi_aa_pp_ratio_cent030_full_wake,
    k_dphi_aa_pp_diff_cent030_no_wake,
    k_dphi_aa_pp_diff_cent030_pos_wake,
    k_dphi_aa_pp_diff_cent030_full_wake,
    k_dphi_aa_pp_ratio_cent3050_no_wake,
    k_dphi_aa_pp_ratio_cent3050_pos_wake,
    k_dphi_aa_pp_ratio_cent3050_full_wake,
    k_dphi_aa_pp_diff_cent3050_no_wake,
    k_dphi_aa_pp_diff_cent3050_pos_wake,
    k_dphi_aa_pp_diff_cent3050_full_wake,
    kN_MODEL
};

const std::string modelTAG[kN_MODEL] = {
        "hybrid_wake_no_ratio_xivh_cent0_30",
        "hybrid_wake_pos_ratio_xivh_cent0_30",
        "hybrid_wake_full_ratio_xivh_cent0_30",
        "hybrid_wake_no_ratio_xivh_cent30_50",
        "hybrid_wake_pos_ratio_xivh_cent30_50",
        "hybrid_wake_full_ratio_xivh_cent30_50",
        //
        "hybrid_wake_no_ratio_trkPt_cent0_30",
        "hybrid_wake_pos_ratio_trkPt_cent0_30",
        "hybrid_wake_full_ratio_trkPt_cent0_30",
        "hybrid_wake_no_ratio_trkPt_cent30_50",
        "hybrid_wake_pos_ratio_trkPt_cent30_50",
        "hybrid_wake_full_ratio_trkPt_cent30_50",
        //
        "hybrid_wake_no_ratio_dphi_cent0_30",
        "hybrid_wake_pos_ratio_dphi_cent0_30",
        "hybrid_wake_full_ratio_dphi_cent0_30",
        "hybrid_wake_no_diff_dphi_cent0_30",
        "hybrid_wake_pos_diff_dphi_cent0_30",
        "hybrid_wake_full_diff_dphi_cent0_30",
        "hybrid_wake_no_ratio_dphi_cent30_50",
        "hybrid_wake_pos_ratio_dphi_cent30_50",
        "hybrid_wake_full_ratio_dphi_cent30_50",
        "hybrid_wake_no_diff_dphi_cent30_50",
        "hybrid_wake_pos_diff_dphi_cent30_50",
        "hybrid_wake_full_diff_dphi_cent30_50",
};

const std::vector<double> x_arr[kN_MODEL] = {
        x_xi,
        x_xi,
        x_xi,
        x_xi,
        x_xi,
        x_xi,
        //
        x_trkPt,
        x_trkPt,
        x_trkPt,
        x_trkPt,
        x_trkPt,
        x_trkPt,
        //
        x_dphi,
        x_dphi,
        x_dphi,
        x_dphi,
        x_dphi,
        x_dphi,
        x_dphi,
        x_dphi,
        x_dphi,
        x_dphi,
        x_dphi,
        x_dphi,
};

const std::vector<double> y_min_arr[kN_MODEL] = {
        y_min_xi_aa_pp_ratio_cent030_no_wake,
        y_min_xi_aa_pp_ratio_cent030_pos_wake,
        y_min_xi_aa_pp_ratio_cent030_full_wake,
        y_min_xi_aa_pp_ratio_cent3050_no_wake,
        y_min_xi_aa_pp_ratio_cent3050_pos_wake,
        y_min_xi_aa_pp_ratio_cent3050_full_wake,
        //
        y_min_trkPt_aa_pp_ratio_cent030_no_wake,
        y_min_trkPt_aa_pp_ratio_cent030_pos_wake,
        y_min_trkPt_aa_pp_ratio_cent030_full_wake,
        y_min_trkPt_aa_pp_ratio_cent3050_no_wake,
        y_min_trkPt_aa_pp_ratio_cent3050_pos_wake,
        y_min_trkPt_aa_pp_ratio_cent3050_full_wake,
        //
        y_min_dphi_aa_pp_ratio_cent030_no_wake,
        y_min_dphi_aa_pp_ratio_cent030_pos_wake,
        y_min_dphi_aa_pp_ratio_cent030_full_wake,
        y_min_dphi_aa_pp_diff_cent030_no_wake,
        y_min_dphi_aa_pp_diff_cent030_pos_wake,
        y_min_dphi_aa_pp_diff_cent030_full_wake,
        y_min_dphi_aa_pp_ratio_cent3050_no_wake,
        y_min_dphi_aa_pp_ratio_cent3050_pos_wake,
        y_min_dphi_aa_pp_ratio_cent3050_full_wake,
        y_min_dphi_aa_pp_diff_cent3050_no_wake,
        y_min_dphi_aa_pp_diff_cent3050_pos_wake,
        y_min_dphi_aa_pp_diff_cent3050_full_wake,
};

const std::vector<double> y_max_arr[kN_MODEL] = {
        y_max_xi_aa_pp_ratio_cent030_no_wake,
        y_max_xi_aa_pp_ratio_cent030_pos_wake,
        y_max_xi_aa_pp_ratio_cent030_full_wake,
        y_max_xi_aa_pp_ratio_cent3050_no_wake,
        y_max_xi_aa_pp_ratio_cent3050_pos_wake,
        y_max_xi_aa_pp_ratio_cent3050_full_wake,
        //
        y_max_trkPt_aa_pp_ratio_cent030_no_wake,
        y_max_trkPt_aa_pp_ratio_cent030_pos_wake,
        y_max_trkPt_aa_pp_ratio_cent030_full_wake,
        y_max_trkPt_aa_pp_ratio_cent3050_no_wake,
        y_max_trkPt_aa_pp_ratio_cent3050_pos_wake,
        y_max_trkPt_aa_pp_ratio_cent3050_full_wake,
        //
        y_max_dphi_aa_pp_ratio_cent030_no_wake,
        y_max_dphi_aa_pp_ratio_cent030_pos_wake,
        y_max_dphi_aa_pp_ratio_cent030_full_wake,
        y_max_dphi_aa_pp_diff_cent030_no_wake,
        y_max_dphi_aa_pp_diff_cent030_pos_wake,
        y_max_dphi_aa_pp_diff_cent030_full_wake,
        y_max_dphi_aa_pp_ratio_cent3050_no_wake,
        y_max_dphi_aa_pp_ratio_cent3050_pos_wake,
        y_max_dphi_aa_pp_ratio_cent3050_full_wake,
        y_max_dphi_aa_pp_diff_cent3050_no_wake,
        y_max_dphi_aa_pp_diff_cent3050_pos_wake,
        y_max_dphi_aa_pp_diff_cent3050_full_wake,
};

const int colors[kN_MODEL] = {
        kMagenta,
        TColor::GetColor("#C619D1"),   // TColor::GetColor("#D19019"),
        TColor::GetColor("#F00C46"),
        kMagenta,
        TColor::GetColor("#C619D1"),
        TColor::GetColor("#F00C46"),
        //
        kMagenta,
        TColor::GetColor("#C619D1"),   // TColor::GetColor("#D19019"),
        TColor::GetColor("#F00C46"),
        kMagenta,
        TColor::GetColor("#C619D1"),
        TColor::GetColor("#F00C46"),
        //
        kMagenta,
        TColor::GetColor("#C619D1"),   // TColor::GetColor("#D19019"),
        TColor::GetColor("#F00C46"),
        kMagenta,
        TColor::GetColor("#C619D1"),
        TColor::GetColor("#F00C46"),
        kMagenta,
        TColor::GetColor("#C619D1"),   // TColor::GetColor("#D19019"),
        TColor::GetColor("#F00C46"),
        kMagenta,
        TColor::GetColor("#C619D1"),
        TColor::GetColor("#F00C46"),
};

const double falpha = 0.7;

// legend entries to be used in a plot
const std::string legendHeader = "Hybrid";

const std::string legendEntry_wake_no = "w/o wake";
const std::string legendEntry_wake_pos = "w/ wake positive only";
const std::string legendEntry_wake_full = "w/ wake";

const std::string legendEntries[kN_MODEL] = {
        legendEntry_wake_no,
        legendEntry_wake_pos,
        legendEntry_wake_full,
        legendEntry_wake_no,
        legendEntry_wake_pos,
        legendEntry_wake_full,
        //
        legendEntry_wake_no,
        legendEntry_wake_pos,
        legendEntry_wake_full,
        legendEntry_wake_no,
        legendEntry_wake_pos,
        legendEntry_wake_full,
        //
        legendEntry_wake_no,
        legendEntry_wake_pos,
        legendEntry_wake_full,
        legendEntry_wake_no,
        legendEntry_wake_pos,
        legendEntry_wake_full,
        legendEntry_wake_no,
        legendEntry_wake_pos,
        legendEntry_wake_full,
        legendEntry_wake_no,
        legendEntry_wake_pos,
        legendEntry_wake_full,
};

}

namespace SCET_G {

/*
 * SCET_G Z+hadron predictions data points
 * naming convention :
 *      n_<observable>    : number of points in the observable
 *      x_<observable>    : x axis values of the observable, bin centers
 *      y_<observable>    : y axis values of the observable
 */

// xi points
const int n_xi = 25;
const std::vector<double> x_xi = {0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7};

// <public ref>
const std::vector<double> y_xi_cent030_g1p8 = {0.453448, 0.47412158, 0.49441026, 0.51431402, 0.53383288, 0.55296682, 0.57171586, 0.59007998, 0.60805919, 0.6256535, 0.64286289, 0.65968737, 0.67612694, 0.6921816, 0.70785135, 0.72313618, 0.7380361, 0.75255112, 0.76668123, 0.78042641, 0.7937867, 0.80676207, 0.81935253, 0.83155808, 0.84337872};
const std::vector<double> y_xi_cent030_g2p0 = {0.34720959, 0.36846595, 0.38943726, 0.41012351, 0.4305247, 0.45064084, 0.47047191, 0.49001793, 0.5092789, 0.5282548, 0.54694564, 0.56535143, 0.58347216, 0.60130783, 0.61885844, 0.636124, 0.6531045, 0.66979993, 0.68621032, 0.70233565, 0.71817591, 0.73373111, 0.74900127, 0.76398636, 0.77868639};
const std::vector<double> y_xi_cent030_g2p2 = {0.25668077, 0.27731857, 0.29777655, 0.3180547, 0.338153, 0.35807147, 0.37781012, 0.39736892, 0.41674788, 0.43594702, 0.45496632, 0.47380578, 0.49246542, 0.51094522, 0.52924518, 0.54736531, 0.5653056, 0.58306606, 0.60064669, 0.61804748, 0.63526844, 0.65230956, 0.66917085, 0.6858523, 0.70235392};

const std::vector<double> y_xi_cent3050_g1p8 = {0.69801884, 0.71307084, 0.72767654, 0.74183593, 0.75554902, 0.76881582, 0.78163631, 0.79401048, 0.80593836, 0.81741994, 0.82845522, 0.83904419, 0.84918685, 0.85888322, 0.86813329, 0.87693705, 0.88529451, 0.89320566, 0.90067052, 0.90768907, 0.91426131, 0.92038727, 0.92606691, 0.93130025, 0.93608728};
const std::vector<double> y_xi_cent3050_g2p0 = {0.60160706, 0.61989195, 0.63769762, 0.65502406, 0.67187129, 0.68823928, 0.70412804, 0.71953759, 0.7344679, 0.74891899, 0.76289085, 0.7763835, 0.7893969, 0.8019311, 0.81398605, 0.82556179, 0.83665829, 0.84727558, 0.85741364, 0.86707247, 0.87625208, 0.88495246, 0.89317362, 0.90091554, 0.90817825};
const std::vector<double> y_xi_cent3050_g2p2 = {0.50170929, 0.52259552, 0.5430112, 0.56295635, 0.58243094, 0.60143499, 0.6199685, 0.63803146, 0.65562388, 0.67274575, 0.68939708, 0.70557787, 0.7212881, 0.7365278, 0.75129696, 0.76559557, 0.77942363, 0.79278115, 0.80566813, 0.81808455, 0.83003044, 0.84150579, 0.85251058, 0.86304483, 0.87310854};

// trk pT points
const int n_trkPt = 14;
const std::vector<double> x_trkPt = {4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30};

// <public ref>
const std::vector<double> y_trkPt_cent030_g1p8 = {0.76234555, 0.7390664, 0.69948566, 0.6732037, 0.65488464, 0.63470386, 0.62202702, 0.61289969, 0.6045943, 0.59946497, 0.59603638, 0.58952403, 0.58370551, 0.5791508};
const std::vector<double> y_trkPt_cent030_g2p0 = {0.68171685, 0.65539889, 0.60992242, 0.58152061, 0.56195484, 0.54016013, 0.52686379, 0.51697947, 0.50773928, 0.50152588, 0.49674066, 0.48916887, 0.48246008, 0.47702};
const std::vector<double> y_trkPt_cent030_g2p2 = {0.5958773, 0.56885996, 0.52084102, 0.4920989, 0.47244331, 0.45037191, 0.43707746, 0.42686123, 0.41689263, 0.40963003, 0.40359988, 0.3952544, 0.38787324, 0.3816786};

const std::vector<double> y_trkPt_cent3050_g1p8 = {0.89579339, 0.88327955, 0.86286496, 0.84605567, 0.83453926, 0.82184033, 0.8127557, 0.80671778, 0.8012309, 0.79796445, 0.7966626, 0.79291585, 0.78942967, 0.78681904};
const std::vector<double> y_trkPt_cent3050_g2p0 = {0.85268643, 0.83566791, 0.80749376, 0.78596021, 0.7710255, 0.75469013, 0.74352648, 0.73596859, 0.7291701, 0.72523443, 0.72347262, 0.71866292, 0.71423696, 0.71090089};
const std::vector<double> y_trkPt_cent3050_g2p2 = {0.80139678, 0.77975695, 0.74362451, 0.71779574, 0.69979118, 0.68014209, 0.66727101, 0.6583506, 0.65035896, 0.64571383, 0.64322929, 0.63733332, 0.63197921, 0.62788731};

enum MODEL{
    k_xi_cent030_g1p8,       // jet-medium coupling strength g = 1.8
    k_xi_cent030_g2p0,       // jet-medium coupling strength g = 2.0
    k_xi_cent030_g2p2,       // jet-medium coupling strength g = 2.2
    k_xi_cent3050_g1p8,
    k_xi_cent3050_g2p0,
    k_xi_cent3050_g2p2,
    //
    k_trkPt_cent030_g1p8,
    k_trkPt_cent030_g2p0,
    k_trkPt_cent030_g2p2,
    k_trkPt_cent3050_g1p8,
    k_trkPt_cent3050_g2p0,
    k_trkPt_cent3050_g2p2,
    kN_MODEL
};

const std::string modelTAG[kN_MODEL] = {
        "scetg_g1p8_ratio_xivh_cent0_30",       // jet-medium coupling strength g = 1.8
        "scetg_g2p0_ratio_xivh_cent0_30",       // jet-medium coupling strength g = 2.0
        "scetg_g2p2_ratio_xivh_cent0_30",       // jet-medium coupling strength g = 2.2
        "scetg_g1p8_ratio_xivh_cent30_50",
        "scetg_g2p0_ratio_xivh_cent30_50",
        "scetg_g2p2_ratio_xivh_cent30_50",
        //
        "scetg_g1p8_ratio_trkPt_cent0_30",
        "scetg_g2p0_ratio_trkPt_cent0_30",
        "scetg_g2p2_ratio_trkPt_cent0_30",
        "scetg_g1p8_ratio_trkPt_cent30_50",
        "scetg_g2p0_ratio_trkPt_cent30_50",
        "scetg_g2p2_ratio_trkPt_cent30_50",
};

const std::vector<double> x_arr[kN_MODEL] = {
        x_xi,
        x_xi,
        x_xi,
        x_xi,
        x_xi,
        x_xi,
        //
        x_trkPt,
        x_trkPt,
        x_trkPt,
        x_trkPt,
        x_trkPt,
        x_trkPt,
};

const std::vector<double> y_arr[kN_MODEL] = {
        y_xi_cent030_g1p8,
        y_xi_cent030_g2p0,
        y_xi_cent030_g2p2,
        y_xi_cent3050_g1p8,
        y_xi_cent3050_g2p0,
        y_xi_cent3050_g2p2,
        //
        y_trkPt_cent030_g1p8,
        y_trkPt_cent030_g2p0,
        y_trkPt_cent030_g2p2,
        y_trkPt_cent3050_g1p8,
        y_trkPt_cent3050_g2p0,
        y_trkPt_cent3050_g2p2,
};

// fill colors to be used in a plot
int fillColors[kN_MODEL] = {
        TColor::GetColor("#0ECAE3"),
        TColor::GetColor("#138808"),
        TColor::GetColor("#E3790E"),
        TColor::GetColor("#0ECAE3"),
        TColor::GetColor("#138808"),
        TColor::GetColor("#E3790E"),
        //
        TColor::GetColor("#0ECAE3"),
        TColor::GetColor("#138808"),
        TColor::GetColor("#E3790E"),
        TColor::GetColor("#0ECAE3"),
        TColor::GetColor("#138808"),
        TColor::GetColor("#E3790E"),
};

const int lineStyles[kN_MODEL] = {
        11,
        12,
        13,
        11,
        12,
        13,
        //
        11,
        12,
        13,
        11,
        12,
        13,
};


const double falpha = 0.7;

// legend entries to be used in a plot
const std::string legendHeader = "SCET_{G}";

const std::string legendEntries[kN_MODEL] = {
        "g = 1.8",
        "g = 2.0",
        "g = 2.2",
        "g = 1.8",
        "g = 2.0",
        "g = 2.2",
        //
        "g = 1.8",
        "g = 2.0",
        "g = 2.2",
        "g = 1.8",
        "g = 2.0",
        "g = 2.2"
};

};

#endif /* DATA_VJETTRK_ZBOSONS_THMODELS_H_ */
