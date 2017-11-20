/*
 * utilities related to TH1 objects.
 */
#ifndef TH1UTIL_H_
#define TH1UTIL_H_

#include <TH1.h>
#include <TH1D.h>
#include <TF1.h>
#include <TGraph.h>
#include <TGraphAsymmErrors.h>
#include <TGraph.h>
#include <TBox.h>
#include <TString.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TPad.h>

#include <string>
#include <vector>

float resetTH1axisMin4LogScale(float axisMin, std::string axis);
std::string  summaryTH1(TH1* h);
TH1* Graph2Histogram(TGraph* graph);
void fillTH1fromTGraph(TH1* h, TGraph* graph);
void setTH1_energyScale(TH1* h, float titleOffsetX = 1.25, float titleOffsetY = 1.75);
void setTH1_energyWidth(TH1* h, float titleOffsetX = 1.25, float titleOffsetY = 1.75);
void setTH1_efficiency (TH1* h, float titleOffsetX = 1.25, float titleOffsetY = 1.75);
double getMinimumTH1s(TH1D* h[], int nHistos, int start = 0);
double getMaximumTH1s(TH1D* h[], int nHistos, int start = 0);
void setConstantBinContent(TH1* h, double constantContent);
void setConstantBinError(TH1* h, double constantError);
void setConstantBinContentError(TH1* h, double constantContent, double  constantError);
void setBinContents(TH1* h, std::vector<double> binContents);
void setBinErrors(TH1* h, std::vector<double> binErrors);
void setBinContentsErrors(TH1* h, std::vector<double> binContents, std::vector<double> binErrors);
void scaleBinErrors(TH1* h, double scale);
void scaleBinContentErrors(TH1* h, double scaleContent, double scaleError);
std::vector<double> getBinContents(TH1* h);
std::vector<double> getBinErrors(TH1* h);
std::vector<double> getTH1xBins(int nBins, double xLow, double xUp);
std::vector<double> getTH1xBins(TH1* h);
int getMinimumBin(TH1D* h, double minval = -FLT_MAX, int binFirst = 1, int binLast = -1);
int getMaximumBin(TH1D* h, double maxval = +FLT_MAX, int binFirst = 1, int binLast = -1);
double getMinimum(TH1D* h, double minval = -FLT_MAX, int binFirst = 1, int binLast = -1);
double getMaximum(TH1D* h, double maxval = +FLT_MAX, int binFirst = 1, int binLast = -1);
int getMinimumTH1Dindex(std::vector<TH1D*> vecH, double minval = -FLT_MAX, int binFirst = 1, int binLast = -1);
int getMaximumTH1Dindex(std::vector<TH1D*> vecH, double maxval = +FLT_MAX, int binFirst = 1, int binLast = -1);
double getMinimumTH1DContent(std::vector<TH1D*> vecH, double minval = -FLT_MAX, int binFirst = 1, int binLast = -1);
double getMaximumTH1DContent(std::vector<TH1D*> vecH, double maxval = +FLT_MAX, int binFirst = 1, int binLast = -1);
std::vector<double> calcBinsLogScale(double min, double max, double nBins);
std::vector<int> getBinRange4IntegralFraction(TH1D* h, int binStart, double fraction, std::string direction = "LR");
int getLeftBin4IntegralFraction(TH1D* h, int binStart, double fraction);
int getRightBin4IntegralFraction(TH1D* h, int binStart, double fraction);
std::vector<int> getLeftRightBins4IntegralFraction(TH1D* h, int binStart, double fraction);
TH1* getResidualHistogram(TH1* h, TH1* hRef, bool normalize = false);
TH1* getResidualHistogram(TH1* h, TH1* hRef, double xMin, double xMax, bool normalize = false);
TH1* getPullHistogram(TH1* h, TH1* hRef);
TH1* getPullHistogram(TH1* h, TH1* hRef, double xMin, double xMax);
TH1* getResidualHistogram(TH1* h, TF1* fRef, bool normalize = false);
TH1* getResidualHistogram(TH1* h, TF1* fRef, double xMin, double xMax, bool normalize = false);
TH1* getPullHistogram(TH1* h, TF1* fRef);
TH1* getPullHistogram(TH1* h, TF1* fRef, double xMin, double xMax);
// systematic uncertainty
void fillTH1fromTF1(TH1* h, TF1* f);
void calcTH1Ratio4SysUnc(TH1* h, TH1* hNominal, float scaleFactor = 1);
void calcTF1Ratio4SysUnc(TH1* h, TF1* fNominal, TF1* fVaried, float scaleFactor = 1);
void calcTH1AbsMax4SysUnc(TH1* h, TH1* h1, TH1* h2);
void calcTH1Abs4SysUnc(TH1* h);
void setTH1Style4SysUnc(TH1* h);
void subtractIdentity4SysUnc(TH1* h);
void addSysUnc(TH1* hTot, TH1* h);
void sysDiff2sysRel(TH1D* hNom, TH1D* hSys);
void sysRel2sysDiff(TH1D* hNom, TH1D* hSys);
void setSysUncBox(TBox* box, TH1* h, TH1* hSys, int bin, double binWidth = -1, double binWidthScale = 1);
void drawSysUncBoxes(TBox* box, TH1* h, TH1* hSys, double binWidth = -1, double binWidthScale = 1);
void setSysUncBox(TGraph* gr, TH1* h, TH1* hSys, int bin, bool doRelUnc = false, double binWidth = -1, double binWidthScale = 1);
void setSysUncBox(TGraph* gr, TH1* h, TH1* hSys, bool isSysMinus, int bin, bool doRelUnc = false, double binWidth = -1, double binWidthScale = 1);
void setSysUncBoxDown(TGraph* gr, TH1* h, TH1* hSys, int bin, bool doRelUnc = false, double binWidth = -1, double binWidthScale = 1);
void setSysUncBoxUp(TGraph* gr, TH1* h, TH1* hSys, int bin, bool doRelUnc = false, double binWidth = -1, double binWidthScale = 1);
void drawSysUncBoxes(TGraph* gr, TH1* h, TH1* hSys, bool doRelUnc = false, double binWidth = -1, double binWidthScale = 1);
void drawSysUncBoxes(TGraph* gr, TH1* h, TH1* hSysDown, TH1* hSysUp, bool doRelUnc = false, double binWidth = -1, double binWidthScale = 1);
// plotting
void drawSameTH1D(TCanvas* c, std::vector<TH1D*> vecTH1D);
void drawSameTH1D(TPad* pad, std::vector<TH1D*> vecTH1D);
void drawSameTGraph(TCanvas* c, std::vector<TGraph*> vecTGraph);
void drawSameTGraph(TPad* pad, std::vector<TGraph*> vecTGraph);

/*
 * reset the lower limit of an axis in case the plot will be drawn log scale and the relevant lower limit is non-positive.
 */
float resetTH1axisMin4LogScale(float axisMin, std::string axis)
{
    float result = axisMin;
    if (ToLower(axis.c_str()).EqualTo("x")) {
        if (result <= 0)   result = 0.001;
    }
    else if (ToLower(axis.c_str()).EqualTo("y")) {
        if (result <= 0)   result = 0.001;
    }
    return result;
}

/*
 * print info about histogram
 */
std::string summaryTH1(TH1* h)
{
    int binFirst = h->GetXaxis()->GetFirst();
    int binLast  = h->GetXaxis()->GetLast();
    std::string result;

    result.append(Form("h->GetEntries() = %f \n", h->GetEntries()));
    result.append(Form("h->GetNbinsX() = %d \n", h->GetNbinsX()));
    result.append(Form("h->GetXaxis()->GetXmin() = %f \n", h->GetXaxis()->GetXmin()));
    result.append(Form("h->GetXaxis()->GetXmax() = %f \n", h->GetXaxis()->GetXmax()));
    result.append(Form("BinLowEdge(h->GetXaxis()->GetFirst()) = %f \n", h->GetXaxis()->GetBinLowEdge(binFirst)));
    result.append(Form("BinLowEdge(h->GetXaxis()->GetLast()) = %f \n", h->GetXaxis()->GetBinLowEdge(binLast)));
    result.append(Form("h->GetBinContent(0) = %f \n", h->GetBinContent(0)));
    result.append(Form("h->GetBinContent(NbinsX+1) = %f \n", h->GetBinContent(h->GetNbinsX()+1)));
    result.append(Form("h->Integral() = %f \n", h->Integral()));
    result.append(Form("h->Integral(\"width\") = %f \n", h->Integral("width")));
    result.append(Form("h->Integral(firstBin, lastBin) = %f \n", h->Integral(binFirst, binLast)));
    result.append(Form("h->GetSumOfWeights() = %f \n", h->GetSumOfWeights()));
    result.append(Form("h->GetMean() = %f , h->GetMeanError() = %f \n", h->GetMean(), h->GetMeanError()));
    result.append(Form("h->GetStdDev() = %f , h->GetStdDevError() = %f \n", h->GetStdDev(), h->GetStdDevError()));
    result.append(Form("h->GetMinimum() = %f , h->GetMaximum() = %f ", h->GetMinimum(), h->GetMaximum()));
    // do not put a new line to the end of the last line

    return result;
}

/*
 * convert a TGraphAsymmErrors to 1D TH1 by setting the data points bin by bin.
 * NOTE : cannot set the asymmetric error bars for TH1. So cannot carry over the asymmetric errors
 * in "graph" to the histogram that is returned.
 * The error in the histogram will be
 * TMath::Sqrt(0.5*(elow*elow + ehigh*ehigh))  which is returned by graph->GetErrorY(i)
 * https://root.cern.ch/root/html/src/TGraphAsymmErrors.cxx.html#rBLO5D
 */
TH1* Graph2Histogram(TGraph* graph)
{
    int fNpoints = graph->GetN();
    double* fX   = graph->GetX();
    double* fY   = graph->GetY();

    if (fNpoints == 0) {
        return NULL;
    }

    // prepare x-bins for the histograms
    // xbins  : array of low-edges for each bin
    //          This is an array of size nbins+1
    double xbins[fNpoints+1];
    for (int i=0; i<fNpoints; ++i)  {
        xbins[i]=fX[i]-graph->GetErrorXlow(i);
    }
    xbins[fNpoints]=fX[fNpoints-1]+graph->GetErrorXhigh(fNpoints-1);

    TH1* h = new TH1D(graph->GetName(),graph->GetTitle(), fNpoints, xbins);

    for (int i=0; i<fNpoints; ++i)  {
        h->SetBinContent(i+1, fY[i]);
        h->SetBinError(i+1, graph->GetErrorY(i));
    }

    return h;
}

void fillTH1fromTGraph(TH1* h, TGraph* graph)
{
    h->Reset();

    int fNpoints = graph->GetN();
    double* fX   = graph->GetX();
    double* fY   = graph->GetY();

    for (int i=0; i<fNpoints; ++i)  {

        double x = fX[i];
        int iBin = h->FindBin(x);

        h->SetBinContent(iBin, fY[i]);
        h->SetBinError(iBin, graph->GetErrorY(i));
    }
}

void setTH1_energyScale(TH1* h, float titleOffsetX, float titleOffsetY) {

    h->SetYTitle("< Reco p_{T} / Gen p_{T} >");
    h->SetTitleOffset(titleOffsetX, "X");
    h->SetTitleOffset(titleOffsetY, "Y");
    h->GetXaxis()->CenterTitle();
    h->GetYaxis()->CenterTitle();
    h->SetAxisRange(0.8,1.5,"Y");
    h->SetStats(false);
    h->SetMarkerStyle(kFullCircle);
}

void setTH1_energyWidth(TH1* h, float titleOffsetX, float titleOffsetY) {

    h->SetYTitle("#sigma( Reco p_{T} / Gen p_{T} )");
    h->SetTitleOffset(titleOffsetX, "X");
    h->SetTitleOffset(titleOffsetY, "Y");
    h->GetXaxis()->CenterTitle();
    h->GetYaxis()->CenterTitle();
    h->SetAxisRange(0,0.5,"Y");
    h->SetStats(false);
    h->SetMarkerStyle(kFullCircle);
}

void setTH1_efficiency(TH1* h, float titleOffsetX, float titleOffsetY) {

    h->SetTitleOffset(titleOffsetX, "X");
    h->SetTitleOffset(titleOffsetY, "Y");
    h->GetXaxis()->CenterTitle();
    h->GetYaxis()->CenterTitle();
    h->SetStats(false);
    h->SetMarkerStyle(kFullCircle);
}

/*
 * get minimum of an array of TH1D
 */
double getMinimumTH1s(TH1D* h[], int nHistos, int start) {

    double result = h[start]->GetMinimum();
    for (int i = start+1; i < start+nHistos; ++i) {
        if (h[i]->GetMinimum() < result)  result = h[i]->GetMinimum();
    }

    return result;
}

/*
 * get maximum of an array of TH1D
 */
double getMaximumTH1s(TH1D* h[], int nHistos, int start) {

    double result = h[start]->GetMaximum();
    for (int i = start+1; i < start+nHistos; ++i) {
        if (h[i]->GetMaximum() > result)  result = h[i]->GetMaximum();
    }

    return result;
}

/*
 * set content of all bins to the given value
 */
void setConstantBinContent(TH1* h, double constantContent)
{
    int nBins = h->GetNbinsX();
    std::vector<double> binContents (nBins+2, constantContent);
    setBinContents(h, binContents);
}

/*
 * set error of all bins to the given value
 */
void setConstantBinError(TH1* h, double constantError)
{
    int nBins = h->GetNbinsX();
    std::vector<double> binErrors (nBins+2, constantError);
    setBinErrors(h, binErrors);
}

void setConstantBinContentError(TH1* h, double constantContent, double  constantError)
{
    setConstantBinContent(h, constantContent);
    setConstantBinError(h, constantError);
}

/*
 * function to set bin contents of TH1 histogram.
 * includes underflow and overflow bins, size of binContents must be nBins + 2.
 * avoids looping over the bins in the main program.
 * helps to keep the code clean.
 */
void setBinContents(TH1* h, std::vector<double> binContents)
{
    int nBins = h->GetNbinsX();
    int nVec  = binContents.size();
    if (nBins+2 != nVec)  return;

    for ( int i = 0; i <= nBins+1; ++i)
    {
        h->SetBinContent(i, binContents.at(i));
    }
}

/*
 * function to set bin errors of TH1 histogram.
 * includes underflow and overflow bins, size of binErrors must be nBins + 2.
 * avoids looping over the bins in the main program.
 * helps to keep the code clean.
 */
void setBinErrors(TH1* h, std::vector<double> binErrors)
{
    int nBins = h->GetNbinsX();
    int nVec  = binErrors.size();
    if (nBins+2 != nVec)  return;

    for ( int i = 0; i <= nBins+1; ++i)
    {
        h->SetBinError(i, binErrors.at(i));
    }
}

void setBinContentsErrors(TH1* h, std::vector<double> binContents, std::vector<double> binErrors)
{
    setBinContents(h, binContents);
    setBinErrors(h, binErrors);
}

void scaleBinErrors(TH1* h, double scale)
{
    int nBins = h->GetNbinsX();
    for ( int i = 0; i <= nBins+1; i++)
    {
        h->SetBinError(i, h->GetBinError(i)*scale);
    }
}

void scaleBinContentErrors(TH1* h, double scaleContent, double scaleError)
{
    int nBins = h->GetNbinsX();
    for ( int i = 0; i <= nBins+1; i++)
    {
        h->SetBinContent(i, h->GetBinContent(i)*scaleContent);
        h->SetBinError(i,   h->GetBinError(i)*scaleError);
    }
}

/*
 * returns the bin contents including underflow and overflow bins
 * size of the vector is nBins+2.
 */
std::vector<double> getBinContents(TH1* h)
{
    std::vector<double> res;
    int nBins = h->GetNbinsX();
    for ( int i = 0; i <= nBins+1; ++i)
    {
        res.push_back(h->GetBinContent(i));
    }

    return res;
}

/*
 * returns the bin errors including underflow and overflow bins
 * size of the vector is nBins+2.
 */
std::vector<double> getBinErrors(TH1* h)
{
    std::vector<double> res;
    int nBins = h->GetNbinsX();
    for ( int i = 0; i <= nBins+1; ++i)
    {
        res.push_back(h->GetBinError(i));
    }

    return res;
}

/*
 * returns the bins along x-axis as a std::vector. creates a temporary TH1 object for this.
 * size of the vector is nBins+1.
 * ith element is the lower edge of bin i.
 * last element is the upper edge of the last bin.
 */
std::vector<double> getTH1xBins(int nBins, double xLow, double xUp)
{
    TH1D hTmp("hTmp_getTH1xBins", "", nBins, xLow, xUp);

    std::vector<double> res = getTH1xBins(&hTmp);
    return res;
}

/*
 * returns the bins along x-axis of a "TH1" object as a std::vector.
 * size of the vector is nBins+1.
 * ith element is the lower edge of bin i.
 * last element is the upper edge of the last bin.
 */
std::vector<double> getTH1xBins(TH1* h) {

    std::vector<double> bins;
    int nBins = h->GetNbinsX();
    for ( int i = 1; i <= nBins; i++)
    {
        bins.push_back(h->GetXaxis()->GetBinLowEdge(i));
        if (i == nBins) bins.push_back(h->GetXaxis()->GetBinUpEdge(i));
    }

    return bins;
}

/*
 * get the bin with the minimum content greater than minval
 * Do the search for bin range [binFirst, binLast]
 * A similar function is TH1::GetMinimum(Double_t minval = -FLT_MAX), but it is useless if TH1::SetMinimum() was used before.
 */
int getMinimumBin(TH1D* h, double minval, int binFirst, int binLast)
{
    if (binLast == -1)  binLast = h->GetNbinsX();
    int res = -1;
    double minTmp = +FLT_MAX;
    for ( int i = binFirst; i <= binLast; i++)
    {
        double content = h->GetBinContent(i);
        if (content > minval && content < minTmp) {
            minTmp = content;
            res = i;
        }
    }

    return res;
}

/*
 * get the bin with the maximum content smaller than maxval
 * Do the search for bin range [binFirst, binLast]
 * A similar function is TH1::GetMaximum(Double_t maxval = FLT_MAX), but it is useless if TH1::SetMaximum() was used before.
 */
int getMaximumBin(TH1D* h, double maxval, int binFirst, int binLast)
{
    if (binLast == -1)  binLast = h->GetNbinsX();
    int res = -1;
    double maxTmp = -FLT_MAX;
    for ( int i = binFirst; i <= binLast; i++)
    {
        double content = h->GetBinContent(i);
        if (content < maxval && content > maxTmp) {
            maxTmp = content;
            res = i;
        }
    }

    return res;
}

double getMinimum(TH1D* h, double minval, int binFirst, int binLast)
{
    int i = getMinimumBin(h, minval, binFirst, binLast);
    return (i != -1) ? h->GetBinContent(i) : -FLT_MAX;
}

double getMaximum(TH1D* h, double maxval, int binFirst, int binLast)
{
    int i = getMaximumBin(h, maxval, binFirst, binLast);
    return (i != -1) ? h->GetBinContent(i) : +FLT_MAX;
}

/*
 * find the histogram with the minimum content greater than minval
 */
int getMinimumTH1Dindex(std::vector<TH1D*> vecH, double minval, int binFirst, int binLast)
{
    int nHist = vecH.size();
    int res = -1;
    double minTmp = +FLT_MAX;
    for (int i = 0; i < nHist; ++i) {

        double content = getMinimum(vecH[i], minval, binFirst, binLast);
        if (content < minTmp) {
            minTmp = content;
            res = i;
        }
    }

    return res;
}

/*
 * find the histogram with the maximum content smaller than maxval
 */
int getMaximumTH1Dindex(std::vector<TH1D*> vecH, double maxval, int binFirst, int binLast)
{
    int nHist = vecH.size();
    int res = -1;
    double maxTmp = -FLT_MAX;
    for (int i = 0; i < nHist; ++i) {

        double content = getMaximum(vecH[i], maxval, binFirst, binLast);
        if (content > maxTmp) {
            maxTmp = content;
            res = i;
        }
    }

    return res;
}

/*
 * from the list of histograms, get the overall minimum content greater than minval
 */
double getMinimumTH1DContent(std::vector<TH1D*> vecH, double minval, int binFirst, int binLast)
{
    int i = getMinimumTH1Dindex(vecH, minval, binFirst, binLast);
    return getMinimum(vecH[i], minval, binFirst, binLast);
}

/*
 * from the list of histograms, get the overall maximum content smaller than maxval
 */
double getMaximumTH1DContent(std::vector<TH1D*> vecH, double maxval, int binFirst, int binLast)
{
    int i = getMaximumTH1Dindex(vecH, maxval, binFirst, binLast);
    return getMaximum(vecH[i], maxval, binFirst, binLast);
}

/*
 * calculate the set of bins where the bins are spaced uniformly in log scale.
 * size of the vector is nBins+1.
 * ith element is the lower edge of bin i.
 * last element is the upper edge of the last bin.
 * Ex.
 *      calcBinsLogScale(0.001, 100, 5) will return {0.001, 0.01, 0.1, 1, 10, 100}
 *      calcBinsLogScale(4, 256, 6) will return {4, 8, 16, 32, 64, 128, 256}
 */
std::vector<double> calcBinsLogScale(double min, double max, double nBins)
{
    std::vector<double> bins;

    double binWidth = TMath::Log10(max/min) / nBins;

    bins.push_back(min);
    for (int i = 1; i < nBins; ++i) {
        bins.push_back(min * TMath::Power(10, i*binWidth));
    }
    bins.push_back(max);

    return bins;
}

/*
 * function for extracting the bin range which covers a given fraction of the total integral.
 * start from binStart and return the shortest range [bin1, bin2] which contains "fraction" of the integral.
 * "direction" can take 3 values :
 * "L" : look at the bins to the left of binStart, by definition bin2 = binStart
 * "R" : look at the bins to the right of binStart, by definition bin1 = binStart
 * "LR" : look at the bins to the left and right of binStart, in that case binStart sits at the center
 */
std::vector<int> getBinRange4IntegralFraction(TH1D* h, int binStart, double fraction, std::string direction)
{
    double intTot = h->Integral();
    double intFrac = intTot*fraction;

    int n = h->GetNbinsX();
    if (fraction >= 1) {
        if (direction == "L")  return {1, binStart};
        else if (direction == "L")  return {binStart, n};
        else if (direction == "LR") return {1, n};
        else                        return {-1, -1};
    }

    if (direction == "L") {
        int bin = binStart;

        while (h->Integral(bin, binStart) < intFrac && bin >= 0) {
            --bin;

            if (h->Integral(bin, binStart) >= intFrac) {
                return {bin, binStart};
            }
        }

        return {bin, binStart};
    }
    else if (direction == "R") {
        int bin = binStart;

        while (h->Integral(binStart, bin) < intFrac && bin <= n+1) {
            ++bin;

            if (h->Integral(binStart, bin) >= intFrac) {
                return {binStart, bin};
            }
        }

        return {binStart, bin};
    }
    else if (direction == "LR") {
        int bin1 = binStart;
        int bin2 = binStart;

        while (h->Integral(bin1, bin2) < intFrac) {

            double f1 = h->Integral(bin1-1, bin2);
            double f2 = h->Integral(bin1, bin2+1);

            // choose the bin range that is just above the fraction
            if (f1 >= intFrac && f2 >= intFrac) {
                if (f1 < f2) return {bin1-1, bin2};
                else         return {bin1, bin2+1};
            }
            else if (f1 >= intFrac) {
                return {bin1-1, bin2};
            }
            else if (f2 >= intFrac) {
                return {bin1, bin2+1};
            }
            else {
                --bin1;
                ++bin2;
            }
        }

        return {bin1, bin2};
    }
    else {
        return {-1, -1};
    }
}

/*
 * start from binStart, go left and return the closest bin such that range [bin, binStart] contains "fraction" of the integral.
 */
int getLeftBin4IntegralFraction(TH1D* h, int binStart, double fraction)
{
    return getBinRange4IntegralFraction(h, binStart, fraction, "L").at(0);
}

/*
 * start from binStart, go right and return the closest bin such that range [binStart, bin] contains "fraction" of the integral.
 */
int getRightBin4IntegralFraction(TH1D* h, int binStart, double fraction)
{
    return getBinRange4IntegralFraction(h, binStart, fraction, "R").at(1);
}

/*
 * start from binStart, go left/right and return the shortest range [bin1, bin2] which contains "fraction" of the integral.
 * binStart sits at the center
 */
std::vector<int> getLeftRightBins4IntegralFraction(TH1D* h, int binStart, double fraction)
{
    return getBinRange4IntegralFraction(h, binStart, fraction, "LR");
}

/*
 * returns a TH1* which is the residual of "h" wrt. "hRef"
 * calculation is based on what is done for RooHist, where the reference object is unbinned, (e.g. curve, graph)
 * https://root.cern.ch/doc/master/RooHist_8cxx_source.html#l00701
 */
TH1* getResidualHistogram(TH1* h, TH1* hRef, bool normalize)
{
    double xMin = h->GetXaxis()->GetBinCenter(1);
    double xMax = h->GetXaxis()->GetBinCenter(h->GetNbinsX());

    return getResidualHistogram(h, hRef, xMin, xMax, normalize);
}

/*
 * returns a TH1* which is the residual of "h" wrt. "hRef" inside range [xMin, xMax]
 * calculation is based on what is done for RooHist, where the reference object is unbinned, (e.g. curve, graph)
 * https://root.cern.ch/doc/master/RooHist_8cxx_source.html#l00701
 */
TH1* getResidualHistogram(TH1* h, TH1* hRef, double xMin, double xMax, bool normalize)
{
    TH1* hRes = (TH1*)h->Clone(Form("%s_Residual", h->GetName()));
    hRes->Reset();

    int nBins = h->GetNbinsX();
    for (int i = 1; i <= nBins; ++i)
    {
        double y = h->GetBinContent(i);

        double x = h->GetXaxis()->GetBinCenter(i);
        if (x < xMin || x > xMax)  continue;

        double iRef = hRef->FindBin(x);
        double yRef = hRef->GetBinContent(iRef);

        double dy = y-yRef;
        double dyErr = h->GetBinError(i);

        if (normalize) {
            double norm = dyErr;
            if (norm == 0) {
                dy = 0;
                dyErr = 0;
            }
            else {
                dy /= norm;
                dyErr /= norm;
            }
        }

        hRes->SetBinContent(i, dy);
        hRes->SetBinError(i, dyErr);
    }

    return hRes;
}

TH1* getPullHistogram(TH1* h, TH1* hRef)
{
    return getResidualHistogram(h, hRef, true);
}

TH1* getPullHistogram(TH1* h, TH1* hRef, double xMin, double xMax)
{
    return getResidualHistogram(h, hRef, xMin, xMax, true);
}

/*
 * returns a TH1* which is the residual of "h" wrt. "fRef"
 * calculation is based on what is done for RooHist, where the reference object is unbinned, (e.g. curve, graph)
 * https://root.cern.ch/doc/master/RooHist_8cxx_source.html#l00701
 */
TH1* getResidualHistogram(TH1* h, TF1* fRef, bool normalize)
{
    double xMin = h->GetXaxis()->GetBinCenter(1);
    double xMax = h->GetXaxis()->GetBinCenter(h->GetNbinsX());

    return getResidualHistogram(h, fRef, xMin, xMax, normalize);
}

/*
 * returns a TH1* which is the residual of "h" wrt. "fRef" inside range [xMin, xMax]
 * calculation is based on what is done for RooHist, where the reference object is unbinned, (e.g. curve, graph)
 * https://root.cern.ch/doc/master/RooHist_8cxx_source.html#l00701
 */
TH1* getResidualHistogram(TH1* h, TF1* fRef, double xMin, double xMax, bool normalize)
{
    TH1* hRes = (TH1*)h->Clone(Form("%s_Residual", h->GetName()));
    hRes->Reset();

    int nBins = h->GetNbinsX();
    for (int i = 1; i <= nBins; ++i)
    {
        double y = h->GetBinContent(i);

        double x = h->GetXaxis()->GetBinCenter(i);
        if (x < xMin || x > xMax)  continue;

        double yRef = fRef->Eval(x);

        double dy = y-yRef;
        double dyErr = h->GetBinError(i);

        if (normalize) {
            double norm = dyErr;
            if (norm == 0) {
                dy = 0;
                dyErr = 0;
            }
            else {
                dy /= norm;
                dyErr /= norm;
            }
        }

        hRes->SetBinContent(i, dy);
        hRes->SetBinError(i, dyErr);
    }

    return hRes;
}

TH1* getPullHistogram(TH1* h, TF1* fRef)
{
    return getResidualHistogram(h, fRef, true);
}

TH1* getPullHistogram(TH1* h, TF1* fRef, double xMin, double xMax)
{
    return getResidualHistogram(h, fRef, xMin, xMax, true);
}

void fillTH1fromTF1(TH1* h, TF1* f)
{
    int nBins = h->GetNbinsX();
    for ( int i = 1; i <= nBins; i++)
    {
        double x = h->GetBinCenter(i);
        double y = f->Eval(x);

        h->SetBinContent(i, y);
        h->SetBinError(i, 0.0001);
    }
}

/*
 * it is assumed that "h" contains the content of hVaried, in particular "h" is clone of hVaried
 * h = hRatio = (( (hVaried - hNominal) * scaleFactor ) - hNominal ) / hNominal
 * if scaleFactor = 1, then : h = hRatio = hVaried / hNominal
 */
void calcTH1Ratio4SysUnc(TH1* h, TH1* hNominal, float scaleFactor)
{
    if (scaleFactor != 1) {
        h->Add(hNominal,-1);
        h->Scale(scaleFactor);
        h->Add(hNominal);
    }
    h->Divide(hNominal);
}

/*
 * it is assumed that "h" is a dummy clone of the histogram for which the uncertainty is calculated.
 */
void calcTF1Ratio4SysUnc(TH1* h, TF1* fNominal, TF1* fVaried, float scaleFactor)
{
  int nBins = h->GetNbinsX();
  for ( int i = 1; i <= nBins; i++)
  {
      double x = h->GetBinCenter(i);
      double yNom = fNominal->Eval(x);
      double yVar = fVaried->Eval(x);

      if (scaleFactor != 1)  h->SetBinContent(i, ((yVar - yNom)*scaleFactor + yNom) / yNom);
      else  h->SetBinContent(i, yVar/yNom);
      h->SetBinError(i, 0.0001);
  }
}

/*
 * h = max(h1, h2) : maximum of absolute value is calculated bin by bin
 */
void calcTH1AbsMax4SysUnc(TH1* h, TH1* h1, TH1* h2)
{
    int nBins = h1->GetNbinsX();
    for (int i = 1; i <= nBins; ++i) {

        double val1 = h1->GetBinContent(i);
        double val2 = h2->GetBinContent(i);
        double err1 = h1->GetBinError(i);
        double err2 = h2->GetBinError(i);

        double val  = (TMath::Abs(val1) > TMath::Abs(val2)) ? val1 : val2;
        h->SetBinContent(i, val);

        if (val == val1)  h->SetBinError(i, err1);
        else              h->SetBinError(i, err2);
    }
}

/*
 * "h" becomes "abs(h)" : replace the bin contents of a "h" with the absolute values
 */
void calcTH1Abs4SysUnc(TH1* h)
{
    int nBins = h->GetNbinsX();
    for ( int i = 1; i <= nBins; i++)
    {
        double x = TMath::Abs(h->GetBinContent(i));
        h->SetBinContent(i, x);
    }
}

void setTH1Style4SysUnc(TH1* h)
{
    h->GetXaxis()->CenterTitle();
    h->GetYaxis()->CenterTitle();

    double titleSize = h->GetTitleSize("X");
    h->SetTitleSize(titleSize*1.2,"X");
    titleSize = h->GetTitleSize("Y");
    h->SetTitleSize(titleSize*1.2,"Y");

    h->SetTitleOffset(1.2,"X");
    h->SetTitleOffset(1.5,"Y");
    h->SetMarkerStyle(kFullSquare);
    h->SetMarkerSize(2);
}

/*
 * "h" is a clone of hRatio.
 * absolute value of the entries of "h" will be uncertainty.
 */
void subtractIdentity4SysUnc(TH1* h)
{
    int nBins = h -> GetNbinsX();
    for(int i = 1; i <= nBins; i++)
    {
        float val   = TMath::Abs(h->GetBinContent(i)) - 1;
        float error = h->GetBinError(i) - 1;
        if( val == -1 )
        {
            val = 0;
            error = 0;
        }

        h->SetBinContent(i, val);
        h->SetBinError(i,error);
    }
}

/*
 * "hTot" is the histogram that contains total uncertainty
 * add the individual uncertainty contribution from "h"
 */
void addSysUnc(TH1* hTot, TH1* h)
{
    int nBins = hTot->GetNbinsX();
    for (int i = 1; i <= nBins; ++i)
    {
        double unc1 = hTot->GetBinContent(i);
        double unc2 = h->GetBinContent(i);
        double uncTot = TMath::Sqrt(unc1*unc1 + unc2*unc2);

        double err1 = hTot->GetBinError(i);
        double err2 = h->GetBinError(i);
        double errTot = TMath::Sqrt(err1*err1 + err2*err2);

        hTot->SetBinContent(i, uncTot);
        hTot->SetBinError(i, errTot);
    }
}

/*
 * "hSys" is the histogram that contains the systematics variation in difference.
 * convert it to histogram that contains the relative systematics variation.
 */
void sysDiff2sysRel(TH1D* hNom, TH1D* hSys)
{
    int nBins = hSys->GetNbinsX();
    for (int iBin = 1; iBin <= nBins; ++iBin) {
        double valDiff = hSys->GetBinContent(iBin);
        double x = hSys->GetBinCenter(iBin);

        int binNom = hNom->FindBin(x);
        double valNom = hNom->GetBinContent(binNom);

        double val = TMath::Abs(valDiff / valNom);
        hSys->SetBinContent(iBin, val);
    }
}

/*
 * "hSys" is the histogram that contains the relative systematics variation.
 * convert it to histogram that contains the systematics variation in difference.
 */
void sysRel2sysDiff(TH1D* hNom, TH1D* hSys)
{
    int nBins = hSys->GetNbinsX();
    for (int iBin = 1; iBin <= nBins; ++iBin) {
        double valRel = hSys->GetBinContent(iBin);
        double x = hSys->GetBinCenter(iBin);

        int binNom = hNom->FindBin(x);
        double valNom = hNom->GetBinContent(binNom);

        double val = TMath::Abs(valRel * valNom);
        hSys->SetBinContent(iBin, val);
    }
}

void setSysUncBox(TBox* box, TH1* h, TH1* hSys, int bin, double binWidth, double binWidthScale)
{
   double val = h->GetBinContent(bin);
   double x   = h->GetBinCenter(bin);
   int binSys = hSys->FindBin(x);

   // double error = TMath::Abs(val * hSys->GetBinContent(binSys));    // if the uncertainty is calculated using ratios
   double error = TMath::Abs(hSys->GetBinContent(binSys));             // if the uncertainty is calculated using differences
   std::string hSysName = hSys->GetName();

   if (binWidth < 0) {
     binWidth = h->GetBinLowEdge(bin+1) - h->GetBinLowEdge(bin);
   }

   double errorLow = val - error;
   double errorUp = val + error;
   if (errorLow < h->GetMinimum())  errorLow = h->GetMinimum();
   if (errorUp  > h->GetMaximum())  errorUp = h->GetMaximum();

   box->SetX1(x - (binWidth/2)*binWidthScale);
   box->SetX2(x + (binWidth/2)*binWidthScale);
   box->SetY1(errorLow);
   box->SetY2(errorUp);
}

void drawSysUncBoxes(TBox* box, TH1* h, TH1* hSys, double binWidth, double binWidthScale)
{
    int nBins = h->GetNbinsX();
    for (int i = 1; i <= nBins; ++i) {
        if (h->GetBinError(i) == 0) continue;
        if (h->GetBinContent(i) < h->GetMinimum()) continue;
        if (h->GetBinContent(i) > h->GetMaximum()) continue;

        setSysUncBox(box, h, hSys, i, binWidth, binWidthScale);
        box->DrawClone();
    }
}

void setSysUncBox(TGraph* gr, TH1* h, TH1* hSys, int bin, bool doRelUnc, double binWidth, double binWidthScale)
{
   double val = h->GetBinContent(bin);
   double x   = h->GetBinCenter(bin);
   int binSys = hSys->FindBin(x);

   double error = TMath::Abs(hSys->GetBinContent(binSys));             // if the uncertainty is calculated using differences
   if (doRelUnc) error = TMath::Abs(val * hSys->GetBinContent(binSys));    // if the uncertainty is calculated using ratios

   std::string hSysName = hSys->GetName();

   if (binWidth < 0) {
     binWidth = h->GetBinLowEdge(bin+1) - h->GetBinLowEdge(bin);
   }

   double errorLow = val - error;
   double errorUp = val + error;
   if (errorLow < h->GetMinimum())  errorLow = h->GetMinimum();
   if (errorUp  > h->GetMaximum())  errorUp = h->GetMaximum();

   gr->SetPoint(0, x - (binWidth/2)*binWidthScale, errorLow);
   gr->SetPoint(1, x + (binWidth/2)*binWidthScale, errorLow);
   gr->SetPoint(2, x + (binWidth/2)*binWidthScale, errorUp);
   gr->SetPoint(3, x - (binWidth/2)*binWidthScale, errorUp);
}

void setSysUncBox(TGraph* gr, TH1* h, TH1* hSys, bool isSysMinus, int bin, bool doRelUnc, double binWidth, double binWidthScale)
{
    double val = h->GetBinContent(bin);
    double x   = h->GetBinCenter(bin);
    int binSys = hSys->FindBin(x);

    double error = TMath::Abs(hSys->GetBinContent(binSys));             // if the uncertainty is calculated using differences
    if (doRelUnc) error = TMath::Abs(val * hSys->GetBinContent(binSys));    // if the uncertainty is calculated using ratios

    std::string hSysName = hSys->GetName();

    if (binWidth < 0) {
      binWidth = h->GetBinLowEdge(bin+1) - h->GetBinLowEdge(bin);
    }

    if (isSysMinus) {
        double valSys = val - error;
        if (valSys < h->GetMinimum())  valSys = h->GetMinimum();

        gr->SetPoint(0, x - (binWidth/2)*binWidthScale, valSys);
        gr->SetPoint(1, x + (binWidth/2)*binWidthScale, valSys);
    }
    else {
        double valSys = val + error;
        if (valSys > h->GetMaximum()) valSys = h->GetMaximum();

        gr->SetPoint(2, x + (binWidth/2)*binWidthScale, valSys);
        gr->SetPoint(3, x - (binWidth/2)*binWidthScale, valSys);
    }
}

void setSysUncBoxUp(TGraph* gr, TH1* h, TH1* hSys, int bin, bool doRelUnc, double binWidth, double binWidthScale)
{
    setSysUncBox(gr, h, hSys, false, bin, doRelUnc, binWidth, binWidthScale);
}

void setSysUncBoxDown(TGraph* gr, TH1* h, TH1* hSys, int bin, bool doRelUnc, double binWidth, double binWidthScale)
{
    setSysUncBox(gr, h, hSys, true, bin, doRelUnc, binWidth, binWidthScale);
}

/*
 * draws SysUnc boxes using TGraph objects instead of TBox. TBox objects with transparent fill do not
 * show up in ".png" files. Hence, use this version of the function to produce transparent boxes in ".png" files
 * if doRelUnc == true, then draw SysUnc. boxes using relative values, otherwise draw it using absolute values
 */
void drawSysUncBoxes(TGraph* gr, TH1* h, TH1* hSys, bool doRelUnc, double binWidth, double binWidthScale)
{
    int nBins = h->GetNbinsX();
    for (int i = 1; i <= nBins; ++i) {
        if (h->GetBinError(i) == 0) continue;
        if (h->GetBinContent(i) < h->GetMinimum()) continue;
        if (h->GetBinContent(i) > h->GetMaximum()) continue;

        setSysUncBox(gr, h, hSys, i, doRelUnc, binWidth, binWidthScale);
        gr->DrawClone("f");
    }
}

void drawSysUncBoxes(TGraph* gr, TH1* h, TH1* hSysDown, TH1* hSysUp, bool doRelUnc, double binWidth, double binWidthScale)
{
    int nBins = h->GetNbinsX();
    for (int i = 1; i <= nBins; ++i) {
        if (h->GetBinError(i) == 0) continue;
        if (h->GetBinContent(i) < h->GetMinimum()) continue;
        if (h->GetBinContent(i) > h->GetMaximum()) continue;

        setSysUncBox(gr, h, hSysDown, true, i, doRelUnc, binWidth, binWidthScale);
        setSysUncBox(gr, h, hSysUp, false, i, doRelUnc, binWidth, binWidthScale);
        gr->DrawClone("f");
    }
}

void drawSameTH1D(TCanvas* c, std::vector<TH1D*> vecTH1D)
{
    drawSameTH1D((TPad*)c, vecTH1D);
}

/*
 * draw a list of histograms onto the same pad
 */
void drawSameTH1D(TPad* pad, std::vector<TH1D*> vecTH1D)
{
    pad->cd();
    int n = vecTH1D.size();
    for (int i = 0; i < n; ++i) {
        if (vecTH1D[i]->GetEntries() == 0) continue;

        if (i == 0)  vecTH1D[i]->Draw();
        else         vecTH1D[i]->Draw("same");
    }
}

void drawSameTGraph(TCanvas* c, std::vector<TGraph*> vecTGraph)
{
    drawSameTGraph((TPad*)c, vecTGraph);
}

/*
 * draw a list of graphs onto the same pad
 */
void drawSameTGraph(TPad* pad, std::vector<TGraph*> vecTGraph)
{
    pad->cd();
    int n = vecTGraph.size();
    for (int i = 0; i < n; ++i) {
        if (i == 0)  vecTGraph[i]->Draw("p e");
        else         vecTGraph[i]->Draw("p e");
    }
}

#endif /* TH1UTIL_H_ */

