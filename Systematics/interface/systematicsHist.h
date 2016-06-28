/*
 * general purpose class for systematic uncertainty calculation
 */

#ifndef SYSTEMATICS_INTERFACE_SYSTEMATICSHIST_H_
#define SYSTEMATICS_INTERFACE_SYSTEMATICSHIST_H_

#include <TH1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCut.h>

#include <string>

namespace SYS {

enum SYSPOLFNC {
    kPOL0,
    kPOL1,
    kPOL2,
    kN_SYSPOLFNC
};

std::string POLFNC_FORMULAS[kN_SYSPOLFNC] = {
        "pol0",
        "pol1",
        "pol2"
};

const int POLFNC_COLORS[kN_SYSPOLFNC] = {kRed, kBlue, kGreen};
const int POLFNC_MARKERSTYLES[kN_SYSPOLFNC] = {kOpenCircle, kOpenSquare, kOpenTriangleDown};

};

class systematicsHist {
public :
    systematicsHist(){

        name = "";
        observable = "";

        isValid = false;
        isSet = false;
        isFit_fnc_nominal = false;
        isFit_fnc_varied = false;
        for (int i = 0; i < SYS::kN_SYSPOLFNC; ++i) {
            fnc_pol[i] = 0;
            isFit_fnc_pol[i] = false;
            fnc_pol_formula[i] = "";
        }
    };
    ~systematicsHist(){};
    static std::string get_fnc_pol_Formula(TF1* fnc, int polIndex);

    std::string name;   // name of the systematics to be calculated.
    std::string observable;     // name of the observable

    TH1D* h1D_nominal;  // nominal observable
    TH1D* h1D_varied;   // observable after systematics variation
    TH1D* h1D_diff;     // diff = nominal - varied
    TH1D* h1D_ratio;    // ratio = varied / nominal
    TH1D* h1D_diff_rel;      // diff_rel  = abs(diff) / nominal
    TH1D* h1D_ratio_rel;     // ratio_rel = abs(ratio - 1)

    TH1D* h1D_diff_v1;     // initial version of h1D_diff  before scaling, abs(), rebinning etc. operations
    TH1D* h1D_ratio_v1;    // initial version of h1D_ratio before scaling, abs(), rebinning etc. operations

    TF1* fnc_nominal;           // function to fit nominal observable
    TF1* fnc_varied;            // function to fit varied observable
    TF1* fnc_pol[SYS::kN_SYSPOLFNC];    // polynomial functions to fit h1D_diff or h1D_ratio
    TF1* fnc_pol_rel[SYS::kN_SYSPOLFNC];    // case 1 : fnc_pol_rel = polynomial functions to fit h1D_diff_rel or h1D_ratio_rel
                                            // case 2 : fnc_pol_rel = abs(fnc_pol) / h1D_nominal or abs(fnc_pol - 1)

    TH1D* h1D_fnc_pol[SYS::kN_SYSPOLFNC];         // histograms made by bin-by-bin evaluation of fnc_pol
    TH1D* h1D_fnc_pol_rel[SYS::kN_SYSPOLFNC];     // histograms made by bin-by-bin evaluation of fnc_pol_rel

    bool isValid;
    bool isSet;
    bool isFit_fnc_nominal;
    bool isFit_fnc_varied;
    bool isFit_fnc_pol[SYS::kN_SYSPOLFNC];
    std::string fnc_pol_formula[SYS::kN_SYSPOLFNC];
};

std::string systematicsHist::get_fnc_pol_Formula(TF1* fnc, int polIndex)
{
    std::string formula = "";
    if (polIndex == 0) {
        formula = Form("(%f)", fnc->GetParameter(0));
    }
    else if (polIndex == 1) {
        formula = Form("(%f + %f*pow(x, 1))", fnc->GetParameter(0),
                                              fnc->GetParameter(1));
    }
    else if (polIndex == 2) {
        formula = Form("(%f + %f*pow(x, 1) + %f*pow(x, 2))", fnc->GetParameter(0),
                                                             fnc->GetParameter(1),
                                                             fnc->GetParameter(2));
    }

    return formula;
}

#endif
