/*
 * class to organize the analysis of the 1D energy scale distribution
 */
#ifndef PERFORMANCE_INTERFACE_ESCALEANALYZER_H_
#define PERFORMANCE_INTERFACE_ESCALEANALYZER_H_

#include <TH1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TF1.h>
#include <TFormula.h>
#include <TMath.h>

#include <string>
#include <vector>
#include <map>

#include "../../Utilities/th1Util.h"

namespace ESANA {

enum FNCS {
    kGAUS_FitSlicesY,   // initial fit from TH2::FitSlicesY
    kGAUS_95,   // Gaus fit seeded by FitSlicesY, uses bin range that covers 95% of the integral
    kGAUS_98,   // Gaus fit seeded by FitSlicesY, uses bin range that covers 98% of the integral
    kCBALL_95,  // crystal ball fit where the Gaussian part is seeded by FitSlicesY, uses bin range that covers 95% of the integral
    kCBALL_99,  // crystal ball fit where the Gaussian part is seeded by FitSlicesY, uses bin range that covers 99% of the integral
    kDSCB_95,   // double sided crystal ball fit where the Gaussian part is seeded by FitSlicesY, uses bin range that covers 95% of the integral
    kDSCB_99,   // double sided crystal ball fit where the Gaussian part is seeded by FitSlicesY, uses bin range that covers 99% of the integral
    kDSCB_92,   // double sided crystal ball fit where the Gaussian part is seeded by FitSlicesY, uses bin range that covers 92% of the integral
    kN_FNCS
};

// labels to identify the function form
const std::string labelGaus = "gaus";
const std::string labelCB = "crystalball";
const std::string labelDSCB = "DSCB";

const std::string FNC_LABELS[kN_FNCS] = {
        labelGaus,
        labelGaus,
        labelGaus,
        labelCB,
        labelCB,
        labelDSCB,
        labelDSCB,
        labelDSCB
};

const std::string FNC_TITLES[kN_FNCS] = {
        "Gaussian",
        "Gaussian",
        "Gaussian",
        "Crystal Ball",
        "Crystal Ball",
        "DSCB",
        "DSCB",
        "DSCB"
};

const std::string FNC_FORMULAS[kN_FNCS] = {
        "gaus",
        "gaus",
        "gaus",
        "crystalball",
        "crystalball",
        "0",
        "0",
        "0"
};

const double intFractions[kN_FNCS] = {1, 0.95, 0.98, 0.95, 0.99, 0.95, 0.99, 0.92};
const int fncColors[kN_FNCS] = {kGreen+2, kRed, kBlue, kOrange-1, kOrange+2, kBlue+1, kRed+1, kBlue+1};
const std::string fitOption = "Q M R N";

};

class eScaleAnalyzer {
public :
    eScaleAnalyzer(){
        h = 0;
        f1 = 0;
        hPull = 0;
        isValid_h = false;
        isValid_f1 = false;
        isValid_hPull = false;

        fitOption = "Q M R N";
        fncLabel = "";

        hMean = -1;
        hMeanErr = -1;
        hStdDev = -1;
        hStdDevErr = -1;
        sigmaEff = -1;
        sigmaEffErr = -1;
        sigmaHM = -1;
        sigmaHMErr = -1;

        f1Mean = -1;
        f1MeanErr = -1;
        f1Sigma = -1;
        f1SigmaErr = -1;
        f1Chi2 = -1;
        f1Ndf = -1;
    };
    ~eScaleAnalyzer(){};

    bool fit();
    bool makePull();
    void setPullTH1D();
    void calcSigmaEff();
    void calcSigmaHM();
    void update();
    std::vector<std::string> getTextLines4HistResult();
    std::vector<std::string> getTextLines4FitResult();

    TH1D* h;       // energy scale distribution
    TF1*  f1;      // fit function
    TH1D* hPull;   // pull distribution for the fit

    std::string fitOption;
    std::string fncLabel;       // label to identify the function form

    bool isValid_h;
    bool isValid_f1;
    bool isValid_hPull;

    double hMean;    // histogram (arithmetic) mean of h, used as cross-check for the mean extracted from fit
    double hMeanErr;
    double hStdDev;   // histogram (arithmetic) std dev of h, used as cross-check for the width extracted from fit
    double hStdDevErr;
    double sigmaEff;  // half-width of the narrowest interval containing 68.27% of the distribution
    double sigmaEffErr;
    double sigmaHM;   // full-width-at-half-maximum (FWHM) of the distribution divided by 2.355
    double sigmaHMErr;

    // Function parameters for a "gaus" function
    double f1Mean;
    double f1MeanErr;
    double f1Sigma;
    double f1SigmaErr;
    double f1Chi2;
    int    f1Ndf;
};

bool eScaleAnalyzer::fit()
{
    if (isValid_h && isValid_f1) {
        h->Fit(f1, fitOption.c_str());
        return true;
    }
    return false;
}

bool eScaleAnalyzer::makePull()
{
    if (isValid_h && isValid_f1) {
        hPull = (TH1D*)getPullHistogram(h, f1);
        isValid_hPull = true;

        setPullTH1D();
        return true;
    }
    return false;
}

void eScaleAnalyzer::setPullTH1D()
{
    if (isValid_hPull) {
        hPull->SetYTitle("Pull");
        hPull->SetMarkerStyle(kFullCircle);
        if (isValid_f1) {
            hPull->SetMarkerColor(f1->GetLineColor());
        }

        // set maximum and minimum of the pull distributions symmetric about y = 0
        double extremum = std::max(TMath::Abs(hPull->GetMinimum()), TMath::Abs(hPull->GetMaximum()));
        extremum *= 1.3;
        hPull->SetMaximum(extremum);
        hPull->SetMinimum(-1*extremum);
    }
}

void eScaleAnalyzer::calcSigmaEff()
{
    if (!isValid_h)  return;
    if (h->GetEntries() == 0) return;

    int binMax = h->GetMaximumBin();

    double fraction = 0.6827;

    std::vector<int> binRange = getLeftRightBins4IntegralFraction(h, binMax, fraction);
    double fracFromBinEdges = h->Integral(binRange[0], binRange[1]) / h->Integral();

    double x1 = h->GetBinLowEdge(binRange[0]);
    double x2 = h->GetBinLowEdge(binRange[1]+1);
    double width = x2-x1;

    // by definition fracFromBinEdges >= fraction
    // scale the width to remove any residual deviation from 0.6827 due to binned data.
    width *= fraction / fracFromBinEdges;

    sigmaEff = width / 2;
    sigmaEffErr = hStdDevErr;       // for the moment, same error as in histogram stdDev
}

void eScaleAnalyzer::calcSigmaHM()
{
    if (!isValid_h)  return;
    if (h->GetEntries() == 0) return;

    int binMax = h->GetMaximumBin();
    double maxContent = h->GetBinContent(binMax);

    double halfMax = maxContent/2;

    int binLeft = getMaximumBin(h, halfMax, 1, binMax);
    int binRight = getMaximumBin(h, halfMax, binMax);

    double x1 = h->GetBinLowEdge(binLeft);
    double x2 = h->GetBinLowEdge(binRight+1);
    double width = x2-x1;

    // For a Gaussian distribution
    // full-width-at-half-maximum = 2.355 * sigma
    sigmaHM = width / 2.355;
    sigmaHMErr = hStdDevErr;       // for the moment, same error as in histogram stdDev
}

void eScaleAnalyzer::update()
{
    isValid_h = (h != 0 && !h->IsZombie());
    isValid_f1 = (f1 != 0 && !f1->IsZombie());
    isValid_hPull = (hPull != 0 && !hPull->IsZombie());

    if (isValid_h) {
        hMean = h->GetMean();
        hMeanErr = h->GetMeanError();

        hStdDev = h->GetStdDev();
        hStdDevErr = h->GetStdDevError();
    }

    calcSigmaEff();
    calcSigmaHM();

    if (isValid_f1) {
        if (fncLabel == ESANA::labelGaus.c_str()) {

            f1Mean = f1->GetParameter(1);
            f1MeanErr = f1->GetParError(1);
            f1Sigma = f1->GetParameter(2);
            f1SigmaErr = f1->GetParError(2);
            f1Chi2 = f1->GetChisquare();
            f1Ndf = f1->GetNDF();
        }
        else if (fncLabel == ESANA::labelCB.c_str()) {

            f1Mean = f1->GetParameter(1);
            f1MeanErr = f1->GetParError(1);
            f1Sigma = f1->GetParameter(2);
            f1SigmaErr = f1->GetParError(2);
            f1Chi2 = f1->GetChisquare();
            f1Ndf = f1->GetNDF();
        }
        else if (fncLabel == ESANA::labelDSCB.c_str()) {

            f1Mean = f1->GetParameter(1);
            f1MeanErr = f1->GetParError(1);
            f1Sigma = TMath::Abs(f1->GetParameter(2));
            f1SigmaErr = f1->GetParError(2);
            f1Chi2 = f1->GetChisquare();
            f1Ndf = f1->GetNDF();
        }
    }
}

/*
 * prepare text lines for mean values and resolutions from histogram mean and stdDev
 */
std::vector<std::string> eScaleAnalyzer::getTextLines4HistResult()
{
    std::vector<std::string> res;

    res.push_back(Form("#mu (Arith) = %.2f#pm%.3f", hMean, hMeanErr));
    res.push_back(Form("#sigma (Arith) = %.2f#pm%.3f", hStdDev, hStdDevErr));

    return res;
}

/*
 * prepare text lines for results from fit, e.g. mean values and resolutions
 */
std::vector<std::string> eScaleAnalyzer::getTextLines4FitResult()
{
    std::vector<std::string> res;

    if (isValid_f1) {
        if (fncLabel == ESANA::labelGaus.c_str()) {

            res.push_back(Form("#mu = %.2f#pm%.3f", f1Mean, f1MeanErr));
            res.push_back(Form("#sigma = %.2f#pm%.3f", f1Sigma, f1SigmaErr));
            res.push_back(Form("#chi^{2} = %.4f", f1Chi2));
        }
        else if (fncLabel == ESANA::labelCB.c_str()) {

            res.push_back(Form("#mu = %.2f#pm%.3f", f1Mean, f1MeanErr));
            res.push_back(Form("#sigma = %.2f#pm%.3f", f1Sigma, f1SigmaErr));
            res.push_back(Form("#alpha = %.2f#pm%.3f", f1->GetParameter(3), f1->GetParError(3)));
            res.push_back(Form("n = %.2f#pm%.3f", f1->GetParameter(4), f1->GetParError(4)));
            res.push_back(Form("#chi^{2} = %.4f", f1Chi2));
        }
        else if (fncLabel == ESANA::labelDSCB.c_str()) {

            res.push_back(Form("#mu = %.2f#pm%.3f", f1Mean, f1MeanErr));
            res.push_back(Form("#sigma = %.2f#pm%.3f", f1Sigma, f1SigmaErr));
            if (f1->GetParameter(2) >= 0) {
                res.push_back(Form("#alpha_{1} = %.2f#pm%.3f", f1->GetParameter(3), f1->GetParError(3)));
                res.push_back(Form("n_{1} = %.2f#pm%.3f", f1->GetParameter(4), f1->GetParError(4)));
                res.push_back(Form("#alpha_{2} = %.2f#pm%.3f", f1->GetParameter(5), f1->GetParError(5)));
                res.push_back(Form("n_{2} = %.2f#pm%.3f", f1->GetParameter(6), f1->GetParError(6)));
            }
            else {
                res.push_back(Form("#alpha_{1} = %.2f#pm%.3f", f1->GetParameter(5), f1->GetParError(5)));
                res.push_back(Form("n_{1} = %.2f#pm%.3f", f1->GetParameter(6), f1->GetParError(6)));
                res.push_back(Form("#alpha_{2} = %.2f#pm%.3f", f1->GetParameter(3), f1->GetParError(3)));
                res.push_back(Form("n_{2} = %.2f#pm%.3f", f1->GetParameter(4), f1->GetParError(4)));
            }
            res.push_back(Form("#chi^{2} = %.4f", f1Chi2));
        }
    }

    return res;
}

#endif
