/*
 * utilities related to TGraph objects.
 */

#include <TH1.h>
#include <TH1D.h>
#include <TF1.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include <TString.h>
#include <TMath.h>

#include <string>
#include <vector>

#ifndef TGRAPHUTIL_H_
#define TGRAPHUTIL_H_

int getMinimumBinIndex(TGraph* gr, double minval = -FLT_MAX);
int getMaximumBinIndex(TGraph* gr, double maxval = +FLT_MAX);
double getMinimum(TGraph* gr, double minval = -FLT_MAX);
double getMaximum(TGraph* gr, double maxval = +FLT_MAX);
void setTGraphBand(TGraph* gr, std::vector<double> x, std::vector<double> ymin, std::vector<double> ymax);
void setTGraphErrors(TGraphErrors* gr, std::vector<double> x, std::vector<double> y, std::vector<double> yerr);

/*
 * get the index of the bin with the minimum content greater than minval
 */
int getMinimumBinIndex(TGraph* gr, double minval)
{
    int fNpoints = gr->GetN();
    double* fY   = gr->GetY();
    int res = -1;
    double minTmp = +FLT_MAX;
    for (int i=0; i<fNpoints; ++i)
    {
        if (fY[i] > minval && fY[i] < minTmp) {
            minTmp = fY[i];
            res = i;
        }
    }

    return res;
}

/*
 * get the index of the bin with the maximum content smaller than maxval
 */
int getMaximumBinIndex(TGraph* gr, double maxval)
{
    int fNpoints = gr->GetN();
    double* fY   = gr->GetY();
    int res = -1;
    double maxTmp = -FLT_MAX;
    for (int i=0; i<fNpoints; ++i)
    {
        if (fY[i] < maxval && fY[i] > maxTmp) {
            maxTmp = fY[i];
            res = i;
        }
    }

    return res;
}

/*
 * get the minimum content greater than minval
 */
double getMinimum(TGraph* gr, double minval)
{
    int i = getMinimumBinIndex(gr, minval);
    return (i != -1) ? gr->GetY()[i] : -FLT_MAX;
}

/*
 * get the maximum content smaller than maxval
 */
double getMaximum(TGraph* gr, double maxval)
{
    int i = getMaximumBinIndex(gr, maxval);
    return (i != -1) ? gr->GetY()[i] : +FLT_MAX;
}

/*
 * set a TGraph object such that it contains a band (a curve with a thickness)
 * x    : x-axis values of the curve
 * ymin : y-axis values of the lower part of the band
 * ymax : y-axis values of the upper part of the band
 * x, ymin and ymax are assumed to have the same length
 *
 * the idea is to define the points of TGraph object such that
 * one traverses a closed loop if one starts out from a point and visits each consecutive point
 * until one comes back to the initial point.
 */
void setTGraphBand(TGraph* gr, std::vector<double> x, std::vector<double> ymin, std::vector<double> ymax)
{
    int n = x.size();
    if (n != (int)ymin.size())  return;

    /*
     * starting from index i = 0, the path goes through line defined by (x, ymax) points, then
     * starting from index i = n, the path goes through line defined by (x, ymin) points in the opposite direction
     * such that a closed loop is defined from i=0 to i=2n-1.
     */
    for (int i=0; i<n; i++) {
       gr->SetPoint(i, x.at(i), ymax.at(i));
       gr->SetPoint(n+i, x.at(n-i-1), ymin.at(n-i-1));
    }
}

void setTGraphErrors(TGraphErrors* gr, std::vector<double> x, std::vector<double> y, std::vector<double> yerr)
{
    int n = x.size();
    if (n != (int)y.size())  return;
    if (n != (int)yerr.size())  return;

    for (int i=0; i<n; i++) {
       gr->SetPoint(i, x.at(i), y.at(i));
       gr->SetPointError(i, 0, yerr.at(i));
    }
}

#endif /* TGRAPHUTIL_H_ */

