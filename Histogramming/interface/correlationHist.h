#ifndef HISTOGRAMMING_INTERFACE_CORRELATIONHIST_H_
#define HISTOGRAMMING_INTERFACE_CORRELATIONHIST_H_

#include <TH1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCut.h>

#include <string>

namespace CORR {

enum CORRFNC {
    kRAW,
    kBKG,
    kSIG,
    kN_CORRFNC
};

std::string CORR_PHO_LABELS[kN_CORRFNC] = {
        "phoRAW",
        "phoBKG",
        "phoSIG"
};

std::string CORR_JET_LABELS[kN_CORRFNC] = {
        "jetRAW",
        "jetBKG",
        "jetSIG"
};

};

namespace ZJET{

enum CORR{
        kRJZ,
        kXJZ_mean,
        kRJZ_zNum,     // R_JZ calculated directly by the number of events
        kN_ZJETCORR
    };

std::string CORR_NAMES[kN_ZJETCORR] = {
        "rjz",
        "xjz_mean",
        "rjz_zNum"
};

};

class correlationHist {
public :
    correlationHist(){

        for (int i = 0; i < CORR::kN_CORRFNC; ++i) {
            for (int j = 0; j < CORR::kN_CORRFNC; ++j) {
                nEntries[i][j] = 0;
                nEntriesPho[i][j] = 0;
            }
        }
    };
    ~correlationHist(){};

    std::string name;   // name of the observable to be histogrammed.

    TH1D* h1D[CORR::kN_CORRFNC][CORR::kN_CORRFNC];
    TH1D* h1D_final[CORR::kN_CORRFNC][CORR::kN_CORRFNC];        // histograms in final axis ranges
    TH1D* h1D_final_norm[CORR::kN_CORRFNC][CORR::kN_CORRFNC];   // histograms in final axis ranges and normalization

    std::string h1D_name[CORR::kN_CORRFNC][CORR::kN_CORRFNC];         // histogram name excluding the "h1D" prefix
    std::string h1D_titleX[CORR::kN_CORRFNC][CORR::kN_CORRFNC];
    std::string h1D_titleY_final_norm[CORR::kN_CORRFNC][CORR::kN_CORRFNC];   // the important y-axis title is that of final, normalized histogram

    TH2D* h2D[CORR::kN_CORRFNC][CORR::kN_CORRFNC];

    TCut  selections[CORR::kN_CORRFNC][CORR::kN_CORRFNC];
    int   nEntries[CORR::kN_CORRFNC][CORR::kN_CORRFNC];
    int   nEntriesPho[CORR::kN_CORRFNC][CORR::kN_CORRFNC];
};

#endif
