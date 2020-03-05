/*
 *  general purpose utilities used in event analysis
 *
 */

#ifndef EVENTUTILS_H_
#define EVENTUTILS_H_

#include <string>
#include <algorithm>    // std::sort
#include <vector>

namespace COLL {

    enum TYPE{
        kHI,
        kHIMC,
        kPP,
        kPPMC,
        kPA,
        kPAMC,
        kHI2018,
        kHIMC2018,
        kPP2017,
        kPPMC2017,
        kN_COLL
    };

    std::string LABELS[kN_COLL] = {
            "HI",
            "HIMC",
            "PP",
            "PPMC",
            "PA",
            "PAMC",
            "HI2018",
            "HIMC2018",
            "PP2017",
            "PPMC2017"
    };

    std::string EVENTSEL[kN_COLL] = {
            "pcollisionEventSelection > 0",
            "pcollisionEventSelection > 0",
            "pPAprimaryVertexFilter > 0 && pBeamScrapingFilter > 0",
            "pPAprimaryVertexFilter > 0 && pBeamScrapingFilter > 0",
            "pPAprimaryVertexFilter > 0 && pBeamScrapingFilter > 0 && pVertexFilterCutGplus > 0",
            "pPAprimaryVertexFilter > 0 && pBeamScrapingFilter > 0 && pVertexFilterCutGplus > 0",
            "pprimaryVertexFilter > 0 && pclusterCompatibilityFilter > 0 && phfCoincFilter2Th4 > 0",
            "pprimaryVertexFilter > 0 && pclusterCompatibilityFilter > 0 && phfCoincFilter2Th4 > 0",
            "pPAprimaryVertexFilter > 0 && pBeamScrapingFilter > 0",
            "pPAprimaryVertexFilter > 0 && pBeamScrapingFilter > 0"
    };
};

std::string getCollisionTypeName (COLL::TYPE collision);
bool collisionIsMC(COLL::TYPE collision);
bool collisionIsDATA(COLL::TYPE collision);
bool collisionIsHI(COLL::TYPE collision);
bool collisionIsHI2018(COLL::TYPE collision);
bool collisionIsPP(COLL::TYPE collision);
bool collisionIsPP2017(COLL::TYPE collision);
bool collisionIsPA(COLL::TYPE collision);
std::string getEventSelection(COLL::TYPE collision);
float findNcoll(int hiBin);
float findNcollAverage(int hiBinLow, int hiBinHigh);
std::vector<float> findNcollAverages(std::vector<int> hiBins);
float findNpart(int hiBin);
float findNpartAverage(int hiBinLow, int hiBinHigh);
std::vector<float> findNpartAverages(std::vector<int> hiBins);
std::vector<float> NpartAveragestoLowEdges(std::vector<float> NpartAverages);
std::vector<int> hiBin2centrality(std::vector<int> hiBins);
std::vector<int> centrality2hiBin(std::vector<int> centralities);

std::string getCollisionTypeName (COLL::TYPE collision) {
  if (collision >=0 && collision < COLL::kN_COLL) return COLL::LABELS[collision];
  return "NULL";
}

bool collisionIsMC(COLL::TYPE collision) {

    return (collision == COLL::kHIMC || collision == COLL::kPPMC || collision == COLL::kPAMC || collision == COLL::kHIMC2018 || collision == COLL::kPPMC2017);
}

bool collisionIsDATA(COLL::TYPE collision) {

    return (collision == COLL::kHI || collision == COLL::kPP || collision == COLL::kPA || collision == COLL::kHI2018 || collision == COLL::kPP2017);
}

bool collisionIsHI(COLL::TYPE collision) {

    return (collision == COLL::kHI || collision == COLL::kHIMC);
}

bool collisionIsHI2018(COLL::TYPE collision) {

    return (collision == COLL::kHI2018 || collision == COLL::kHIMC2018);
}

bool collisionIsPP(COLL::TYPE collision) {

    return (collision == COLL::kPP || collision == COLL::kPPMC);
}

bool collisionIsPP2017(COLL::TYPE collision) {

    return (collision == COLL::kPP2017 || collision == COLL::kPPMC2017);
}

bool collisionIsPA(COLL::TYPE collision) {

    return (collision == COLL::kPA || collision == COLL::kPAMC);
}

std::string getEventSelection(COLL::TYPE collision)
{
    if (collision >= 0 && collision < COLL::kN_COLL)
        return COLL::EVENTSEL[collision];
    else
        return "";
}

/*
 * https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideHeavyIonCentrality?rev=100#Ncoll_Npart_5_TeV
 * https://twiki.cern.ch/twiki/pub/CMS/HI2015DailyMeetings/Ncoll_Npart_04Dec2015.pdf
 * 03.03.2016
 */
float findNcoll(int hiBin) {
    const int nbins = 200;
    const float Ncoll[nbins] = {1976.95, 1944.02, 1927.29, 1891.9, 1845.3, 1807.2, 1760.45, 1729.18, 1674.8, 1630.3, 1590.52, 1561.72, 1516.1, 1486.5, 1444.68, 1410.88, 1376.4, 1347.32, 1309.71, 1279.98, 1255.31, 1219.89, 1195.13, 1165.96, 1138.92, 1113.37, 1082.26, 1062.42, 1030.6, 1009.96, 980.229, 955.443, 936.501, 915.97, 892.063, 871.289, 847.364, 825.127, 806.584, 789.163, 765.42, 751.187, 733.001, 708.31, 690.972, 677.711, 660.682, 640.431, 623.839, 607.456, 593.307, 576.364, 560.967, 548.909, 530.475, 519.575, 505.105, 490.027, 478.133, 462.372, 451.115, 442.642, 425.76, 416.364, 405.154, 392.688, 380.565, 371.167, 360.28, 348.239, 340.587, 328.746, 320.268, 311.752, 300.742, 292.172, 281.361, 274.249, 267.025, 258.625, 249.931, 240.497, 235.423, 228.63, 219.854, 214.004, 205.425, 199.114, 193.618, 185.644, 180.923, 174.289, 169.641, 161.016, 157.398, 152.151, 147.425, 140.933, 135.924, 132.365, 127.017, 122.127, 117.817, 113.076, 109.055, 105.16, 101.323, 98.098, 95.0548, 90.729, 87.6495, 84.0899, 80.2237, 77.2201, 74.8848, 71.3554, 68.7745, 65.9911, 63.4136, 61.3859, 58.1903, 56.4155, 53.8486, 52.0196, 49.2921, 47.0735, 45.4345, 43.8434, 41.7181, 39.8988, 38.2262, 36.4435, 34.8984, 33.4664, 31.8056, 30.351, 29.2074, 27.6924, 26.7754, 25.4965, 24.2802, 22.9651, 22.0059, 21.0915, 19.9129, 19.1041, 18.1487, 17.3218, 16.5957, 15.5323, 14.8035, 14.2514, 13.3782, 12.8667, 12.2891, 11.61, 11.0026, 10.3747, 9.90294, 9.42648, 8.85324, 8.50121, 7.89834, 7.65197, 7.22768, 6.7755, 6.34855, 5.98336, 5.76555, 5.38056, 5.11024, 4.7748, 4.59117, 4.23247, 4.00814, 3.79607, 3.68702, 3.3767, 3.16309, 2.98282, 2.8095, 2.65875, 2.50561, 2.32516, 2.16357, 2.03235, 1.84061, 1.72628, 1.62305, 1.48916, 1.38784, 1.28366, 1.24693, 1.18552, 1.16085, 1.12596, 1.09298, 1.07402, 1.06105, 1.02954};
    return Ncoll[hiBin];
}

/*
 * https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideHeavyIonCentrality?rev=100#Ncoll_Npart_5_TeV
 * 03.03.2016
 */
float findNcollAverage(int hiBinLow, int hiBinHigh) {
    float w=0;
    const int nbins = 200;
    const float Ncoll[nbins] = {1976.95, 1944.02, 1927.29, 1891.9, 1845.3, 1807.2, 1760.45, 1729.18, 1674.8, 1630.3, 1590.52, 1561.72, 1516.1, 1486.5, 1444.68, 1410.88, 1376.4, 1347.32, 1309.71, 1279.98, 1255.31, 1219.89, 1195.13, 1165.96, 1138.92, 1113.37, 1082.26, 1062.42, 1030.6, 1009.96, 980.229, 955.443, 936.501, 915.97, 892.063, 871.289, 847.364, 825.127, 806.584, 789.163, 765.42, 751.187, 733.001, 708.31, 690.972, 677.711, 660.682, 640.431, 623.839, 607.456, 593.307, 576.364, 560.967, 548.909, 530.475, 519.575, 505.105, 490.027, 478.133, 462.372, 451.115, 442.642, 425.76, 416.364, 405.154, 392.688, 380.565, 371.167, 360.28, 348.239, 340.587, 328.746, 320.268, 311.752, 300.742, 292.172, 281.361, 274.249, 267.025, 258.625, 249.931, 240.497, 235.423, 228.63, 219.854, 214.004, 205.425, 199.114, 193.618, 185.644, 180.923, 174.289, 169.641, 161.016, 157.398, 152.151, 147.425, 140.933, 135.924, 132.365, 127.017, 122.127, 117.817, 113.076, 109.055, 105.16, 101.323, 98.098, 95.0548, 90.729, 87.6495, 84.0899, 80.2237, 77.2201, 74.8848, 71.3554, 68.7745, 65.9911, 63.4136, 61.3859, 58.1903, 56.4155, 53.8486, 52.0196, 49.2921, 47.0735, 45.4345, 43.8434, 41.7181, 39.8988, 38.2262, 36.4435, 34.8984, 33.4664, 31.8056, 30.351, 29.2074, 27.6924, 26.7754, 25.4965, 24.2802, 22.9651, 22.0059, 21.0915, 19.9129, 19.1041, 18.1487, 17.3218, 16.5957, 15.5323, 14.8035, 14.2514, 13.3782, 12.8667, 12.2891, 11.61, 11.0026, 10.3747, 9.90294, 9.42648, 8.85324, 8.50121, 7.89834, 7.65197, 7.22768, 6.7755, 6.34855, 5.98336, 5.76555, 5.38056, 5.11024, 4.7748, 4.59117, 4.23247, 4.00814, 3.79607, 3.68702, 3.3767, 3.16309, 2.98282, 2.8095, 2.65875, 2.50561, 2.32516, 2.16357, 2.03235, 1.84061, 1.72628, 1.62305, 1.48916, 1.38784, 1.28366, 1.24693, 1.18552, 1.16085, 1.12596, 1.09298, 1.07402, 1.06105, 1.02954};
    for(int i=hiBinLow; i<hiBinHigh; i++)  w+=Ncoll[i]/(hiBinHigh-hiBinLow);
    return w;
}

/*
 * takes, a list of centrality bins as input and outputs a list of average Ncoll values.
 * number of elements in the output is one less than the number of elements in the input
 */
std::vector<float> findNcollAverages(std::vector<int> hiBins) {

    std::vector<float> NcollAverages;

    int nHibins = hiBins.size();
    for (int i = 0; i < nHibins-1; ++i)
    {
        NcollAverages.push_back(findNcollAverage(hiBins.at(i), hiBins.at(i+1)));
    }

    return NcollAverages;
}

/*
 * https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideHeavyIonCentrality?rev=100#Ncoll_Npart_5_TeV
 * https://twiki.cern.ch/twiki/pub/CMS/HI2015DailyMeetings/Ncoll_Npart_04Dec2015.pdf
 * 03.03.2016
 * Npart[200] is a duplicate of Npart[199], for convenience
 */
float findNpart(int hiBin) {
   const int nbins = 200;
   const float Npart[nbins+1] = {401.99, 398.783, 396.936, 392.71, 387.901, 383.593, 377.914, 374.546, 367.507, 361.252, 356.05, 352.43, 345.701, 341.584, 335.148, 330.581, 325.135, 320.777, 315.074, 310.679, 306.687, 301.189, 296.769, 291.795, 287.516, 283.163, 277.818, 274.293, 269.29, 265.911, 260.574, 256.586, 252.732, 249.194, 245.011, 241.292, 236.715, 232.55, 229.322, 225.328, 221.263, 218.604, 214.728, 210.554, 206.878, 203.924, 200.84, 196.572, 193.288, 189.969, 186.894, 183.232, 180.24, 177.36, 174.008, 171.222, 168.296, 165.319, 162.013, 158.495, 156.05, 154.218, 150.559, 148.455, 145.471, 142.496, 139.715, 137.395, 134.469, 131.926, 129.817, 127.045, 124.467, 122.427, 119.698, 117.607, 114.543, 112.662, 110.696, 108.294, 105.777, 103.544, 101.736, 99.943, 97.4951, 95.4291, 93.2148, 91.2133, 89.5108, 87.2103, 85.7498, 83.5134, 81.9687, 79.7456, 78.1684, 76.4873, 74.7635, 72.761, 71.0948, 69.6102, 67.7806, 66.2215, 64.5813, 63.0269, 61.4325, 59.8065, 58.2423, 57.2432, 55.8296, 54.2171, 52.8809, 51.3254, 49.9902, 48.6927, 47.5565, 46.136, 44.8382, 43.6345, 42.3964, 41.4211, 39.9681, 39.178, 37.9341, 36.9268, 35.5626, 34.5382, 33.6912, 32.8156, 31.6695, 30.6552, 29.7015, 28.8655, 27.9609, 27.0857, 26.105, 25.3163, 24.4872, 23.6394, 23.0484, 22.2774, 21.4877, 20.5556, 19.9736, 19.3296, 18.5628, 17.916, 17.2928, 16.6546, 16.1131, 15.4013, 14.8264, 14.3973, 13.7262, 13.2853, 12.8253, 12.2874, 11.7558, 11.2723, 10.8829, 10.4652, 9.96477, 9.6368, 9.09316, 8.84175, 8.48084, 8.05694, 7.64559, 7.29709, 7.07981, 6.70294, 6.45736, 6.10284, 5.91788, 5.5441, 5.33311, 5.06641, 4.96415, 4.6286, 4.38214, 4.2076, 4.01099, 3.81054, 3.63854, 3.43403, 3.23244, 3.08666, 2.86953, 2.74334, 2.62787, 2.48354, 2.38115, 2.26822, 2.23137, 2.1665, 2.14264, 2.10636, 2.07358, 2.05422, 2.04126, 2.00954, 2.00954};
   return Npart[hiBin];
}

/*
 * https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideHeavyIonCentrality?rev=100#Ncoll_Npart_5_TeV
 * 03.03.2016
 */
float findNpartAverage(int hiBinLow, int hiBinHigh) {
   float w=0;
   const int nbins = 200;
   const float Npart[nbins] = {401.99, 398.783, 396.936, 392.71, 387.901, 383.593, 377.914, 374.546, 367.507, 361.252, 356.05, 352.43, 345.701, 341.584, 335.148, 330.581, 325.135, 320.777, 315.074, 310.679, 306.687, 301.189, 296.769, 291.795, 287.516, 283.163, 277.818, 274.293, 269.29, 265.911, 260.574, 256.586, 252.732, 249.194, 245.011, 241.292, 236.715, 232.55, 229.322, 225.328, 221.263, 218.604, 214.728, 210.554, 206.878, 203.924, 200.84, 196.572, 193.288, 189.969, 186.894, 183.232, 180.24, 177.36, 174.008, 171.222, 168.296, 165.319, 162.013, 158.495, 156.05, 154.218, 150.559, 148.455, 145.471, 142.496, 139.715, 137.395, 134.469, 131.926, 129.817, 127.045, 124.467, 122.427, 119.698, 117.607, 114.543, 112.662, 110.696, 108.294, 105.777, 103.544, 101.736, 99.943, 97.4951, 95.4291, 93.2148, 91.2133, 89.5108, 87.2103, 85.7498, 83.5134, 81.9687, 79.7456, 78.1684, 76.4873, 74.7635, 72.761, 71.0948, 69.6102, 67.7806, 66.2215, 64.5813, 63.0269, 61.4325, 59.8065, 58.2423, 57.2432, 55.8296, 54.2171, 52.8809, 51.3254, 49.9902, 48.6927, 47.5565, 46.136, 44.8382, 43.6345, 42.3964, 41.4211, 39.9681, 39.178, 37.9341, 36.9268, 35.5626, 34.5382, 33.6912, 32.8156, 31.6695, 30.6552, 29.7015, 28.8655, 27.9609, 27.0857, 26.105, 25.3163, 24.4872, 23.6394, 23.0484, 22.2774, 21.4877, 20.5556, 19.9736, 19.3296, 18.5628, 17.916, 17.2928, 16.6546, 16.1131, 15.4013, 14.8264, 14.3973, 13.7262, 13.2853, 12.8253, 12.2874, 11.7558, 11.2723, 10.8829, 10.4652, 9.96477, 9.6368, 9.09316, 8.84175, 8.48084, 8.05694, 7.64559, 7.29709, 7.07981, 6.70294, 6.45736, 6.10284, 5.91788, 5.5441, 5.33311, 5.06641, 4.96415, 4.6286, 4.38214, 4.2076, 4.01099, 3.81054, 3.63854, 3.43403, 3.23244, 3.08666, 2.86953, 2.74334, 2.62787, 2.48354, 2.38115, 2.26822, 2.23137, 2.1665, 2.14264, 2.10636, 2.07358, 2.05422, 2.04126, 2.00954};
   for(int i=hiBinLow; i<hiBinHigh; i++)  w+=Npart[i]/(hiBinHigh-hiBinLow);
   return w;
}

/*
 * takes, a list of centrality bins as input and outputs a list of average Npart values.
 * number of elements in the output is one less than the number of elements in the input
 */
std::vector<float> findNpartAverages(std::vector<int> hiBins) {

    std::vector<float> NpartAverages;

    int nHibins = hiBins.size();
    for (int i = 0; i < nHibins-1; ++i)
    {
        NpartAverages.push_back(findNpartAverage(hiBins.at(i), hiBins.at(i+1)));
    }

    return NpartAverages;
}

/*
 *  using NpartAverages create a vector that contains the proper low edges
 *  if the given Npart values are to be used on histogram x-axis
 *  number of elements in the output is one more (upper edge of the last bin) than the number of elements in the input
 */
std::vector<float> NpartAveragestoLowEdges(std::vector<float> NpartAverages) {

    std::vector<float> NpartLowEdges;
    NpartLowEdges.push_back(0);     // low edge of the first bin

    // final vector should be in increasing order
    std::sort(NpartAverages.begin(), NpartAverages.end());

    int n = NpartAverages.size();
    for (int i = 0; i < n-1; ++i)
    {
        double edge = (NpartAverages.at(i) + NpartAverages.at(i+1)) / 2;
        NpartLowEdges.push_back(edge);
    }
    NpartLowEdges.push_back(401.99);     // upper edge of the last bin

    return NpartLowEdges;
}

std::vector<int> hiBin2centrality(std::vector<int> hiBins)
{
    std::vector<int> res;
    for (std::vector<int>::iterator it = hiBins.begin(); it != hiBins.end(); it++) {
        int tmp = (*it);
        if ((*it) >= 0) tmp = (*it)/2;
        res.push_back(tmp);
    }

    return res;
}

std::vector<int> centrality2hiBin(std::vector<int> centralities)
{
    std::vector<int> res;
    for (std::vector<int>::iterator it = centralities.begin(); it != centralities.end(); it++) {
        int tmp = (*it);
        if ((*it) >= 0) tmp = (*it)*2;
        res.push_back(tmp);
    }

    return res;
}


/*
 * Chris' centrality table for Hydjet Drum sample
 *
 * Created with file '/mnt/hadoop/cms/store/user/cmcginn/Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018/cmcginn-HydjetMB_Drum5Ev8_CMSSW1030pre5_GT103X_upgrade2018_realistic_HI_v4_FOREST_20181007/181007_035311/merged/HiForestAOD_cmcginn-HydjetMB_Drum5Ev8_CMSSW1030pre5_GT103X_upgrade2018_realistic_HI_v4_FOREST_20181007_181007_035311_104_OutOf104_MERGED.root'
 * nBins=200, meaning percentiles of 0.50%
 *
 * Returns lower bound value of hibin at the granularity of nBins=200
 */
int getHiBinFromHiHF_HydjetDrum2018_pre5_GT103X_upgrade2018_realistic_HI_v4(const double hiHF)
{
  int binPos = -1;
  const int nbins = 200;
  const double bins[nbins+1] = {0, 11.402981, 11.863598, 12.203489, 12.504443, 12.78535, 13.069993, 13.353013, 13.639939, 13.920027, 14.222142, 14.541481, 14.857064, 15.181408, 15.520039, 15.869592, 16.241499, 16.617125, 17.017603, 17.440634, 17.884993, 18.329363, 18.804626, 19.312521, 19.820789, 20.364552, 20.935425, 21.541672, 22.184324, 22.888805, 23.635494, 24.441858, 25.306753, 26.174648, 27.142305, 28.195105, 29.308109, 30.483839, 31.833176, 33.257278, 34.802288, 36.39703, 38.065117, 39.911739, 41.818432, 43.841061, 45.909393, 48.025803, 50.288593, 52.587036, 55.007565, 57.524788, 60.070835, 62.743, 65.3703, 68.087112, 70.770195, 73.596375, 76.489021, 79.738335, 83.002754, 86.286354, 89.826096, 93.403397, 96.914337, 100.721191, 104.641968, 108.79834, 113.275139, 117.779503, 122.366592, 127.122688, 132.072113, 137.15509, 142.474838, 147.9561, 153.627625, 159.401855, 165.39827, 171.550644, 178.150681, 184.871353, 191.657211, 198.864273, 206.231171, 213.450623, 221.082794, 229.284409, 237.481613, 245.987503, 254.983063, 263.957977, 272.816193, 282.393768, 292.223328, 302.25885, 312.624634, 322.997864, 333.637054, 344.636383, 355.699371, 367.0914, 378.843933, 391.428192, 404.00824, 416.037689, 429.006409, 442.128845, 455.632599, 469.897888, 484.46463, 498.705872, 513.616028, 528.75, 544.495972, 560.059998, 575.826721, 592.561096, 608.960999, 626.161682, 644.245972, 662.186096, 680.731262, 699.170776, 719.053223, 739.042542, 759.209351, 779.132568, 798.878845, 820.443176, 841.818115, 863.771362, 886.225037, 908.910706, 932.184021, 955.070679, 979.368042, 1004.284302, 1029.14502, 1054.491943, 1081.030762, 1106.862671, 1133.913574, 1161.992065, 1189.975098, 1218.963379, 1247.056885, 1275.42041, 1305.49585, 1335.017334, 1365.326538, 1397.496948, 1430.483765, 1463.624146, 1498.024414, 1531.614868, 1567.229614, 1602.755615, 1638.500366, 1673.411743, 1710.786621, 1749.142944, 1789.001953, 1827.888794, 1867.984985, 1909.746826, 1950.378906, 1993.248657, 2035.986328, 2079.056152, 2125.136475, 2172.588379, 2220.071045, 2267.9104, 2316.889404, 2367.734131, 2418.838379, 2470.061035, 2523.489502, 2577.966553, 2634.092285, 2690.586426, 2751.527832, 2812.916748, 2875.904053, 2939.97583, 3004.644531, 3070.96167, 3138.467041, 3210.053223, 3284.937744, 3356.917969, 3433.714111, 3512.362793, 3596.166748, 3686.692139, 3782.96167, 3891.727783, 4021.669434, 4208.821777, 50000};

  for(int i = 0; i < nbins; ++i){
    if(hiHF >= bins[i] && hiHF < bins[i+1]){
      binPos = i;
      break;
    }
  }

  binPos = nbins - 1 - binPos;

  return (int)(200*((double)binPos)/((double)nbins));
}

#endif /* EVENTUTILS_H_ */
