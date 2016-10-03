/*
 * file with hard coded results for different Z+jet studies
 */

#ifndef DATA_ZBOSONS_ZJETDATA_H_
#define DATA_ZBOSONS_ZJETDATA_H_

#include <vector>
#include <string>

namespace HYBRID {

/*
 * hybrid model Z+jet predictions data points
 * naming convention :
 *      n_<observable>    : number of points in the observable
 *      x_<observable>    : x axis values of the observable, bin centers
 *      y_<observable>    : y axis values of the observable
 *      yerr_<observable> : absolute error on y-axis values
 *      ymin_*            : lower part of the band
 *      ymax_*            : upper part of the band
 *          The band includes all sort of uncertainties : theoretical, experimental and statistical.
 *     STRONG : strong coupling model
 *     RAD    : radiative energy loss model
 *     COLL   : collisional energy loss model
 *     PP     : vacuum (pp)
 */

// xjz points
const int n_xjz = 16;
const std::vector<double> x_xjz = {0.0625, 0.1875, 0.3125, 0.4375, 0.5625, 0.6875, 0.8125, 0.9375, 1.0625, 1.1875, 1.3125, 1.4375, 1.5625, 1.6875, 1.8125, 1.9375};

const std::vector<double> ymin_xjz_STRONG = {0.00128864, 0.0454047, 0.211556, 0.510503, 0.704073, 0.71926, 0.665012, 0.497719, 0.289259, 0.142004, 0.0613361, 0.0288084, 0.0154153, 0.00790779, 0.00439357, 0.00520737};
const std::vector<double> ymax_xjz_STRONG = {0.00204139, 0.0503173, 0.218705, 0.53655, 0.756522, 0.803848, 0.770538, 0.605536, 0.361668, 0.177063, 0.0793732, 0.0388172, 0.0203503, 0.0101173, 0.00582638, 0.00754024};

const std::vector<double> ymin_xjz_RAD = {0.00142662, 0.0447279, 0.213899, 0.549529, 0.786953, 0.804263, 0.725395, 0.532009, 0.296542, 0.135807, 0.0618211, 0.0291477, 0.0145584, 0.00801084, 0.00363744, 0.00551646};
const std::vector<double> ymax_xjz_RAD = {0.00237897, 0.0494925, 0.22798, 0.568981, 0.808747, 0.858247, 0.811916, 0.631005, 0.375536, 0.175543, 0.0779228, 0.0390154, 0.0191584, 0.0109995, 0.00605631, 0.00721872};

const std::vector<double> ymin_xjz_COLL = {0.00171394, 0.0436353, 0.213881, 0.553665, 0.815297, 0.846008, 0.762236, 0.542721, 0.294873, 0.136428, 0.0627293, 0.0294732, 0.0143535, 0.00822071, 0.00456571, 0.00516725};
const std::vector<double> ymax_xjz_COLL = {0.00243812, 0.0505108, 0.219787, 0.572492, 0.833098, 0.88704, 0.848816, 0.634904, 0.368147, 0.171379, 0.0766393, 0.0367642, 0.020213, 0.0110092, 0.00616015, 0.00707444};

const std::vector<double> y_xjz_PP = {0.00241773, 0.0612915, 0.227923, 0.502172, 0.716688, 0.828301, 0.951433, 0.894278, 0.638017, 0.334152, 0.150879, 0.065416, 0.0337776, 0.0183158, 0.0093329, 0.0122893};
const std::vector<double> yerr_xjz_PP = {0.000329856, 0.00166081, 0.00320269, 0.00475386, 0.00567918, 0.00610541, 0.00654349, 0.00634391, 0.00535842, 0.00387786, 0.00260577, 0.00171578, 0.00123292, 0.000907888, 0.000648081, 0.000743675};

// rjz points
const int n_rjz = 4;
const std::vector<double> x_rjz = {45, 55, 70, 100};

const std::vector<double> ymin_rjz_STRONG = {0.16154, 0.251802, 0.379953, 0.600896};
const std::vector<double> ymax_rjz_STRONG = {0.204494, 0.309096, 0.444437, 0.657};

const std::vector<double> ymin_rjz_RAD = {0.177303, 0.280107, 0.420941, 0.639642};
const std::vector<double> ymax_rjz_RAD = {0.21808, 0.331702, 0.473252, 0.681051};

const std::vector<double> ymin_rjz_COLL = {0.185707, 0.294468, 0.436337, 0.651479};
const std::vector<double> ymax_rjz_COLL = {0.223405, 0.343098, 0.483663, 0.688508};

const std::vector<double> y_rjz_PP = {0.350815, 0.467744, 0.58999, 0.769178};
const std::vector<double> yerr_rjz_PP = {0.00284269, 0.00428056, 0.00327163, 0.00388578};

// dphi points
const int n_dphi = 8;
const std::vector<double> x_dphi = {0.315, 0.945, 1.495, 1.965, 2.36, 2.675, 2.915, 3.0708};

const std::vector<double> ymin_dphi_STRONG = {0.00731118, 0.00930054, 0.0136894, 0.0301756, 0.103328, 0.339071, 0.921691, 2.00601};
const std::vector<double> ymax_dphi_STRONG = {0.0093717, 0.0123807, 0.0174326, 0.0376136, 0.121749, 0.387662, 1.04416, 2.23181};

const std::vector<double> ymin_dphi_RAD = {0.00739981, 0.00961568, 0.0139608, 0.0313482, 0.110334, 0.362603, 0.996463, 2.16914};
const std::vector<double> ymax_dphi_RAD = {0.00986992, 0.0126703, 0.0180214, 0.0391061, 0.127855, 0.407177, 1.09403, 2.33925};

const std::vector<double> ymin_dphi_COLL = {0.0077215, 0.00961275, 0.0139146, 0.0318785, 0.11313, 0.372858, 1.02271, 2.22413};
const std::vector<double> ymax_dphi_COLL = {0.00983782, 0.0124276, 0.0180857, 0.0395205, 0.128981, 0.413191, 1.10883, 2.37276};

const std::vector<double> y_dphi_PP = {0.0177265, 0.0236685, 0.0338809, 0.0665819, 0.181537, 0.533101, 1.33085, 2.67002};
const std::vector<double> yerr_dphi_PP = {0.000286749, 0.000331367, 0.000458977, 0.000643401, 0.00128754, 0.00224169, 0.00478292, 0.00742312};

// IAA
const int n_IAA = 5;

const std::vector<double> x_IAA_STRONG = {35.069, 45.1078, 54.9757, 67.307, 89.4147};
const std::vector<double> ymin_IAA_STRONG = {0.890972, 0.775532, 0.64381, 0.504133, 0.422482};
const std::vector<double> ymax_IAA_STRONG = {1.00827, 0.917928, 0.771226, 0.625474, 0.527708};

const std::vector<double> x_IAA_RAD = {35.0623, 45.0946, 54.9826, 67.3234, 89.4107};
const std::vector<double> ymin_IAA_RAD = {1.035, 0.897975, 0.706066, 0.527602, 0.417987};
const std::vector<double> ymax_IAA_RAD = {1.10573, 0.990985, 0.821189, 0.648459, 0.526771};

const std::vector<double> x_IAA_COLL = {35.0782, 45.0962, 54.9867, 67.313, 89.4289};
const std::vector<double> ymin_IAA_COLL = {1.07732, 0.949815, 0.740491, 0.548713, 0.416121};
const std::vector<double> ymax_IAA_COLL = {1.13572, 1.02667, 0.844317, 0.654911, 0.525503};

// IAA 2
const std::vector<double> x_IAA2_STRONG = {39.7211, 65.6362, 93.0506, 123.442};
const std::vector<double> ymin_IAA2_STRONG = {0.958907, 0.891856, 0.677577, 0.61297};
const std::vector<double> ymax_IAA2_STRONG = {1.00003, 0.9621, 0.777774, 0.730158};

const std::vector<double> x_IAA2_RAD = {39.7286, 65.613, 93.0124, 123.451};
const std::vector<double> ymin_IAA2_RAD = {1.00834, 0.965166, 0.713259, 0.645519};
const std::vector<double> ymax_IAA2_RAD = {1.04925, 1.02371, 0.803982, 0.738814};

const std::vector<double> x_IAA2_COLL = {39.7103, 65.6364, 92.9684, 123.436};
const std::vector<double> ymin_IAA2_COLL = {1.01515, 1.00435, 0.739984, 0.643649};
const std::vector<double> ymax_IAA2_COLL = {1.03824, 1.04631, 0.811429, 0.739145};

enum MODEL{
    kSTRONG,
    kRAD,
    kCOLL,
    kN_MODEL
};

// fill colors to be used in a plot
const int fillColors[kN_MODEL] = {
        kMagenta+2,
        kBlue+2,
        kOrange+2
};
const int fillColorPP = 34;

// legend entries to be used in a plot
const std::string legendEntries[kN_MODEL] = {
        "strong coupling",
        "radiative",
        "collisional"
};
const std::string legendEntryPP = "smeared pp";

const std::vector<double> ymin_xjz[kN_MODEL] = {
        ymin_xjz_STRONG,
        ymin_xjz_RAD,
        ymin_xjz_COLL
};

const std::vector<double> ymax_xjz[kN_MODEL] = {
        ymax_xjz_STRONG,
        ymax_xjz_RAD,
        ymax_xjz_COLL
};

const std::vector<double> ymin_dphi[kN_MODEL] = {
        ymin_dphi_STRONG,
        ymin_dphi_RAD,
        ymin_dphi_COLL
};

const std::vector<double> ymax_dphi[kN_MODEL] = {
        ymax_dphi_STRONG,
        ymax_dphi_RAD,
        ymax_dphi_COLL
};

const std::vector<double> ymin_rjz[kN_MODEL] = {
        ymin_rjz_STRONG,
        ymin_rjz_RAD,
        ymin_rjz_COLL
};

const std::vector<double> ymax_rjz[kN_MODEL] = {
        ymax_rjz_STRONG,
        ymax_rjz_RAD,
        ymax_rjz_COLL
};

const std::vector<double> x_IAA[kN_MODEL] = {
        x_IAA_STRONG,
        x_IAA_RAD,
        x_IAA_COLL
};

const std::vector<double> ymin_IAA[kN_MODEL] = {
        ymin_IAA_STRONG,
        ymin_IAA_RAD,
        ymin_IAA_COLL
};

const std::vector<double> ymax_IAA[kN_MODEL] = {
        ymax_IAA_STRONG,
        ymax_IAA_RAD,
        ymax_IAA_COLL
};

const std::vector<double> x_IAA2[kN_MODEL] = {
        x_IAA2_STRONG,
        x_IAA2_RAD,
        x_IAA2_COLL
};

const std::vector<double> ymin_IAA2[kN_MODEL] = {
        ymin_IAA2_STRONG,
        ymin_IAA2_RAD,
        ymin_IAA2_COLL
};

const std::vector<double> ymax_IAA2[kN_MODEL] = {
        ymax_IAA2_STRONG,
        ymax_IAA2_RAD,
        ymax_IAA2_COLL
};

};

namespace JEWEL {

/*
 * JEWEL Z+jet predictions data points
 * naming convention :
 *      n_<observable>    : number of points in the observable
 *      x_<observable>    : x axis values of the observable, bin centers
 *      y_<observable>    : y axis values of the observable
 *      yerr_<observable> : absolute error on y-axis values
 */

// xjz points
const int n_xjz = 16;
const std::vector<double> x_xjz = {0.1875, 0.3125, 0.4375, 0.5625, 0.6875, 0.8125, 0.9375, 1.0625, 1.1875, 1.3125, 1.4375, 1.5625, 1.6875};
const std::vector<double> x_xjz_PP = {0.0625, 0.1875, 0.3125, 0.4375, 0.5625, 0.6875, 0.8125, 0.9375, 1.0625, 1.1875, 1.3125, 1.4375, 1.5625, 1.6875, 1.8125, 1.9375};
const std::vector<double> x_xjz_low = {0.125, 0.250, 0.375, 0.500, 0.625, 0.750, 0.875, 1.0, 1.125, 1.250, 1.375, 1.500, 1.625};
const std::vector<double> x_xjz_up  = {0.250, 0.375, 0.500, 0.625, 0.750, 0.875, 1.0, 1.125, 1.250, 1.375, 1.500, 1.625, 1.750};

const std::vector<double> y_xjz =    {0.02291148,  0.2484096,  0.5738543,  0.7470694,  0.8140488,  0.6692568,  0.4340828,  0.1908817,  0.1066594,  0.03799797,  0.01936494,  0.004432283,  0.002174007};
const std::vector<double> yerr_xjz = {0.002084357, 0.02754472, 0.03648541, 0.04139489, 0.04977742, 0.03684065, 0.03549587, 0.01865503, 0.01797712, 0.008233660, 0.006611463, 0.001331190,  0.0007250199};

const std::vector<double> y_xjz_PP =    {0, 0.002847023,  0.05708221,  0.3091131,  0.6711203,  0.9013256,  1.139810,   1.100711,   0.5673296,  0.2940010,  0.08982424, 0.06205845, 0.01471836,  0.008852848, 0.004953986, 0.001412555};
const std::vector<double> yerr_xjz_PP = {0, 0.0009544396, 0.008018108, 0.04709293, 0.06275363, 0.06575365, 0.08107233, 0.06548313, 0.04075253, 0.04150607, 0.01684868, 0.01414269, 0.003900187, 0.004068555, 0.002208277, 0.0004786352};

// rjz points
const int n_rjz = 4;
const std::vector<double> x_rjz = {45, 55, 70, 100};
const std::vector<double> x_rjz_low = {40, 50, 60, 80};
const std::vector<double> x_rjz_up  = {60, 60, 80, 120};

const std::vector<double> y_rjz    = {0.1312656,  0.2526694,  0.2959370,  0.5298627};
const std::vector<double> yerr_rjz = {0.05117222, 0.04864656, 0.03172202, 0.01555707};

const std::vector<double> y_rjz_PP    = {0.3380081, 0.6071840,  0.5384109,  0.6991855};
const std::vector<double> yerr_rjz_PP = {0.1205536, 0.08370821, 0.03465326, 0.01848420};

// dphi points
const int n_dphi = 8;
const std::vector<double> x_dphi = {0.315, 0.945, 1.495, 1.965, 2.36, 2.675, 2.915, 3.0708};
const std::vector<double> x_dphi_low = {0.6283190, 1.256640, 1.727880, 2.199110, 2.513270, 2.827430, 2.984510};
const std::vector<double> x_dphi_up  = {1.256640,  1.727880, 2.199110, 2.513270, 2.827430, 2.984510, 3.141590};

const std::vector<double> y_dphi    = {0.001505730,  0.001981490,  0.01251127,  0.02756974,  0.08306831, 0.2259430,  0.7323275,  2.177156};
const std::vector<double> yerr_dphi = {0.0006533969, 0.0005124969, 0.004751477, 0.007617846, 0.01661666, 0.01672637, 0.04175201, 0.06261580};

const std::vector<double> y_dphi_PP    = {0.0005172377, 0.0002714026, 0.005032162, 0.01387987,  0.1030486,  0.3619837,  0.9465432,  2.921958};
const std::vector<double> yerr_dphi_PP = {0.0002737988, 0.0001355236, 0.002785972, 0.002916629, 0.01681101, 0.04335443, 0.06217190, 0.09742159};

// xjz_mean points
const int n_xjz_mean = 4;
const std::vector<double> x_xjz_mean = {45, 55, 70, 100};
const std::vector<double> x_xjz_mean_low = {40, 50, 60, 80};
const std::vector<double> x_xjz_mean_up  = {60, 60, 80, 120};

const std::vector<double> y_xjz_mean    = {0.9782095, 0.8053848,  0.7844115,  0.6702255};
const std::vector<double> yerr_xjz_mean = {0.1111712, 0.04514933, 0.02058386, 0.008383264};

const std::vector<double> y_xjz_mean_PP    = {0.9347126,  0.8099350,  0.8616110,  0.8125114};
const std::vector<double> yerr_xjz_mean_PP = {0.06997798, 0.07184345, 0.02075788, 0.009971107};

const std::string legendEntryPP = "smeared pp";

};

namespace VITEV {

/*
 * Ivan Vitev Z+jet predictions data points
 * naming convention :
 *      n_<observable>    : number of points in the observable
 *      x_<observable>    : x axis values of the observable, bin centers
 *      y_<observable>    : y axis values of the observable
 */

// xjz points
const int n_xjz = 39;
const std::vector<double> x_xjz = {0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1, 1.05, 1.1, 1.15, 1.2, 1.25, 1.3, 1.35, 1.4, 1.45, 1.5, 1.55, 1.6, 1.65, 1.7, 1.75, 1.8, 1.85, 1.9, 1.95, 2};

// 0-30 %
const std::vector<double> y_xjz_COLL =   {0.003767499, 0.020715959, 0.061261631, 0.13440306, 0.23350968, 0.35552325, 0.49775664, 0.65957264, 0.83420452, 0.83235345, 0.83828822, 0.84879899, 0.8588376, 0.85965292, 0.84431841, 0.8043959, 0.72759885, 0.61063726, 0.46964041, 0.33572475, 0.2319446, 0.15908775, 0.10975048, 0.076780311, 0.054498818, 0.039021169, 0.028352467, 0.020970583, 0.015614264, 0.011743023, 0.0089854737, 0.0069209085, 0.0053257263, 0.0040971224, 0.0031478445, 0.0024178486, 0.0018737271, 0.0014671336, 0.001147079};
const std::vector<double> y_xjz_NOCOLL = {0.0037958501, 0.019807912, 0.050756668, 0.099148737, 0.1694517, 0.27802328, 0.43708049, 0.64052515, 0.86698042, 0.8816398, 0.89815939, 0.91601401, 0.93325411, 0.94194038, 0.93419392, 0.89959692, 0.82231214, 0.69655078, 0.53918256, 0.38608678, 0.26627044, 0.18202115, 0.12510343, 0.087217453, 0.061704485, 0.044025948, 0.031884743, 0.023516369, 0.017457973, 0.013093009, 0.010000742, 0.0076964967, 0.0059196718, 0.0045510924, 0.0034922172, 0.0026775225, 0.0020722434, 0.0016227333, 0.001270907};

const std::vector<double> y_xjz_PP = {0.0040369166, 0.020680538, 0.05090977, 0.092425476, 0.14579128, 0.21335941, 0.29839051, 0.41153036, 0.56112517, 0.59948991, 0.64744763, 0.70926454, 0.78671365, 0.87154949, 0.95779418, 1.0282484, 1.0456372, 0.98009628, 0.82662044, 0.61632682, 0.4276026, 0.28979177, 0.19663476, 0.1353289, 0.094581254, 0.066670084, 0.047785852, 0.034879396, 0.025553236, 0.018871435, 0.01424658, 0.010932414, 0.0084477898, 0.0065410944, 0.0050271967, 0.0038180785, 0.0029075427, 0.0022483154, 0.0017669715};

// <xjz> points
// 0-30 %
const std::vector<double> xjz_mean_COLL = 0.71;
const std::vector<double> xjz_mean_NOCOLL = 0.73;

const std::vector<double> xjz_mean_PP = 0.8;

enum MODEL{
    kCOLL,      // collisional energy loss
    kNOCOLL,    // no collisional energy loss
    kN_MODEL
};

const std::vector<double> ymin_xjz[kN_MODEL] = {
        y_xjz_COLL,
        y_xjz_NOCOLL
};

// legend entries to be used in a plot
const std::string legendEntries[kN_MODEL] = {
        "with collisional E-loss",
        "without collisional E-loss"
};
const std::string legendEntryPP = "smeared pp";

};

#endif /* DATA_ZBOSONS_ZJETDATA_H_ */
