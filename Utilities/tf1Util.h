/*
 * utilities related to TF1 objects.
 */

#include <TF1.h>
#include <TGraphAsymmErrors.h>
#include <TGraphErrors.h>
#include <TBox.h>
#include <TString.h>
#include <TMath.h>

#ifndef TF1UTIL_H_
#define TF1UTIL_H_

std::string getTF1FormulaFitParams(TF1* fnc);

/*
 * for a TF1 function with a user defined expression, get the formula with "fitted" parameters
 */
std::string getTF1FormulaFitParams(TF1* fnc)
{
    std::string formula = fnc->GetExpFormula().Data();

    int nParams = fnc->GetNumberFreeParameters();
    for (int i = 0; i < nParams; ++i) {
        std::string strOld = Form("[p%d]", i);
        std::string strNew = Form("(%f)", fnc->GetParameter(i));
        formula = replaceAll(formula.c_str(), strOld.c_str(), strNew.c_str());
    }

    return formula;
}
#endif /* TF1UTIL_H_ */

