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
        kN_COLL
    };

    std::string LABELS[kN_COLL] = {
            "HI",
            "HIMC",
            "PP",
            "PPMC",
            "PA",
            "PAMC"
    };

    std::string EVENTSEL[kN_COLL] = {
            "pcollisionEventSelection > 0",
            "pcollisionEventSelection > 0",
            "pPAprimaryVertexFilter > 0 && pBeamScrapingFilter > 0",
            "pPAprimaryVertexFilter > 0 && pBeamScrapingFilter > 0",
            "pPAprimaryVertexFilter > 0 && pBeamScrapingFilter > 0 && pVertexFilterCutGplus > 0",
            "pPAprimaryVertexFilter > 0 && pBeamScrapingFilter > 0 && pVertexFilterCutGplus > 0"
    };
};

std::string getCollisionTypeName (COLL::TYPE collision);
bool collisionIsMC(COLL::TYPE collision);
bool collisionIsDATA(COLL::TYPE collision);
bool collisionIsHI(COLL::TYPE collision);
bool collisionIsPP(COLL::TYPE collision);
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

    return (collision == COLL::kHIMC || collision == COLL::kPPMC || collision == COLL::kPAMC);
}

bool collisionIsDATA(COLL::TYPE collision) {

    return (collision == COLL::kHI || collision == COLL::kPP || collision == COLL::kPA);
}

bool collisionIsHI(COLL::TYPE collision) {

    return (collision == COLL::kHI || collision == COLL::kHIMC);
}

bool collisionIsPP(COLL::TYPE collision) {

    return (collision == COLL::kPP || collision == COLL::kPPMC);
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

#endif /* EVENTUTILS_H_ */
