/*
 * file with hard coded results for different Z+hadron studies
 */

#ifndef DATA_VJETTRK_ZBOSONS_THMODELS_H_
#define DATA_VJETTRK_ZBOSONS_THMODELS_H_

#include <vector>
#include <string>
#include <TColor.h>

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
