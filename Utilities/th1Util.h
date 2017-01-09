/*
 * utilities related to TH1 objects.
 */

#include <TH1.h>
#include <TH1D.h>
#include <TF1.h>
#include <TGraph.h>
#include <TGraphAsymmErrors.h>
#include <TGraph.h>
#include <TBox.h>
#include <TString.h>
#include <TMath.h>

#include <string>
#include <vector>

#include "interface/InputConfigurationParser.h"

#ifndef TH1UTIL_H_
#define TH1UTIL_H_

float resetTH1axisMin4LogScale(float axisMin, std::string axis);
std::string  summaryTH1(TH1* h);
TH1* Graph2Histogram(TGraph* graph);
void setTH1_energyScale(TH1* h, float titleOffsetX = 1.25, float titleOffsetY = 1.75);
void setTH1_energyWidth(TH1* h, float titleOffsetX = 1.25, float titleOffsetY = 1.75);
void setTH1_efficiency (TH1* h, float titleOffsetX = 1.25, float titleOffsetY = 1.75);
double getMinimumTH1s(TH1D* h[], int nHistos, int start = 0);
double getMaximumTH1s(TH1D* h[], int nHistos, int start = 0);
void setConstantBinContent(TH1* h, double constantContent);
void setConstantBinContentError(TH1* h, double constantContent, double  constantError);
void setBinContents(TH1* h, std::vector<double> binContents);
void setBinContentsErrors(TH1* h, std::vector<double> binContents, std::vector<double> binErrors);
void scaleBinErrors(TH1* h, double scale);
void scaleBinContentErrors(TH1* h, double scaleContent, double scaleError);
std::vector<double> getTH1xBins(TH1* h);
std::vector<double> calcBinsLogScale(double min, double max, double nBins);
TH1* getResidualHistogram(TH1* h, TH1* hRef, bool normalize = false);
TH1* getPullHistogram(TH1* h, TH1* hRef);
TH1* getResidualHistogram(TH1* h, TF1* fRef, bool normalize = false);
TH1* getPullHistogram(TH1* h, TF1* fRef);
// systematic uncertainty
void fillTH1fromTF1(TH1* h, TF1* f);
void calcTH1Ratio4SysUnc(TH1* h, TH1* hNominal, float scaleFactor = 1);
void calcTF1Ratio4SysUnc(TH1* h, TF1* fNominal, TF1* fVaried, float scaleFactor = 1);
void calcTH1AbsMax4SysUnc(TH1* h, TH1* h1, TH1* h2);
void calcTH1Abs4SysUnc(TH1* h);
void setTH1Style4SysUnc(TH1* h);
void subtractIdentity4SysUnc(TH1* h);
void addSysUnc(TH1* hTot, TH1* h);
void setSysUncBox(TBox* box, TH1* h, TH1* hSys, int bin, double binWidth = -1, double binWidthScale = 1);
void drawSysUncBoxes(TBox* box, TH1* h, TH1* hSys, double binWidth = -1, double binWidthScale = 1);
void setSysUncBox(TGraph* gr, TH1* h, TH1* hSys, int bin, bool doRelUnc = false, double binWidth = -1, double binWidthScale = 1);
void drawSysUncBoxes(TGraph* gr, TH1* h, TH1* hSys, bool doRelUnc = false, double binWidth = -1, double binWidthScale = 1);

/*
 * reset the lower limit of an axis in case the plot will be drawn log scale and the relevant lower limit is non-positive.
 */
float resetTH1axisMin4LogScale(float axisMin, std::string axis)
{
    float result = axisMin;
    if (ToLower(axis.c_str()).EqualTo("x")) {
        if (result <= 0)   result = INPUT_DEFAULT::xMin;
    }
    else if (ToLower(axis.c_str()).EqualTo("y")) {
        if (result <= 0)   result = INPUT_DEFAULT::yMin;
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

void setTH1_energyScale(TH1* h, float titleOffsetX, float titleOffsetY) {

    h->SetYTitle("< Reco p_{T} / Gen p_{T} >");
    h->SetTitleOffset(titleOffsetX, "X");
    h->SetTitleOffset(titleOffsetY, "Y");
    h->SetAxisRange(0.8,1.5,"Y");
    h->SetStats(false);
    h->SetMarkerStyle(kFullCircle);
}

void setTH1_energyWidth(TH1* h, float titleOffsetX, float titleOffsetY) {

    h->SetYTitle("#sigma( Reco p_{T} / Gen p_{T} )");
    h->SetTitleOffset(titleOffsetX, "X");
    h->SetTitleOffset(titleOffsetY, "Y");
    h->SetAxisRange(0,0.5,"Y");
    h->SetStats(false);
    h->SetMarkerStyle(kFullCircle);
}

void setTH1_efficiency(TH1* h, float titleOffsetX, float titleOffsetY) {

    h->SetTitleOffset(titleOffsetX, "X");
    h->SetTitleOffset(titleOffsetY, "Y");
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
    std::vector<double> binContents (nBins, constantContent);
    setBinContents(h, binContents);
}

/*
 * set content of all bins to constantContent
 * set error of all bins to constantError
 */
void setConstantBinContentError(TH1* h, double constantContent, double  constantError)
{
    int nBins = h->GetNbinsX();
    std::vector<double> binContents (nBins, constantContent);
    std::vector<double> binErrors   (nBins, constantError);
    setBinContentsErrors(h, binContents, binErrors);
}

/*
 * function to set bin contents of TH1 histogram.
 * avoids looping over the bins in the main program.
 * helps to keep the code clean.
 */
void setBinContents(TH1* h, std::vector<double> binContents)
{
    int nBins = h->GetNbinsX();
    int nVec  = binContents.size();
    if (nBins != nVec)  return;

    for ( int i = 1; i <= nBins; i++)
    {
        h->SetBinContent(i, binContents.at(i-1));
    }
}

/*
 * function to set bin contents and errors of TH1 histogram.
 * avoids looping over the bins in the main program.
 * helps to keep the code clean.
 */
void setBinContentsErrors(TH1* h, std::vector<double> binContents, std::vector<double> binErrors)
{
    int nBins    = h->GetNbinsX();
    int nVec     = binContents.size();
    int nVecErr  = binErrors.size();
    if (nBins != nVec)     return;
    if (nBins != nVecErr)  return;

    for ( int i = 1; i <= nBins; i++)
    {
        h->SetBinContent(i, binContents.at(i-1));
        h->SetBinError(i, binErrors.at(i-1));
    }
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
 * returns a TH1* which is the residual of "h" wrt. "hRef"
 * calculation is based on what is done for RooHist, where the reference object is unbinned, (e.g. curve, graph)
 * https://root.cern.ch/doc/master/RooHist_8cxx_source.html#l00701
 */
TH1* getResidualHistogram(TH1* h, TH1* hRef, bool normalize)
{
    TH1* hRes = (TH1*)h->Clone(Form("%s_Residual", h->GetName()));

    int nBins = h->GetNbinsX();
    for (int i = 1; i <= nBins; ++i)
    {
        double y = h->GetBinContent(i);

        double x = h->GetXaxis()->GetBinCenter(i);
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

/*
 * returns a TH1* which is the residual of "h" wrt. "fRef"
 * calculation is based on what is done for RooHist, where the reference object is unbinned, (e.g. curve, graph)
 * https://root.cern.ch/doc/master/RooHist_8cxx_source.html#l00701
 */
TH1* getResidualHistogram(TH1* h, TF1* fRef, bool normalize)
{
    TH1* hRes = (TH1*)h->Clone(Form("%s_Residual", h->GetName()));

    int nBins = h->GetNbinsX();
    for (int i = 1; i <= nBins; ++i)
    {
        double y = h->GetBinContent(i);

        double x = h->GetXaxis()->GetBinCenter(i);
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

#endif /* TH1UTIL_H_ */

