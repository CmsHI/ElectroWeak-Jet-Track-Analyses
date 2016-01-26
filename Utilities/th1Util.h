/*
 * utilities related to TH1 objects.
 */

#include <TH1.h>
#include <TH1D.h>
#include <TGraphAsymmErrors.h>
#include <TString.h>

#include "interface/InputConfigurationParser.h"

#ifndef TH1UTIL_H_
#define TH1UTIL_H_

float resetTH1axisMin4LogScale(float axisMin, std::string axis);
std::string  summaryTH1(TH1* h);
TH1* Graph2Histogram(TGraphAsymmErrors* graph);
void setTH1_energyScale(TH1* h, float titleOffsetX = 1.25, float titleOffsetY = 1.75);
void setTH1_energyWidth(TH1* h, float titleOffsetX = 1.25, float titleOffsetY = 1.75);
void setTH1_efficiency (TH1* h, float titleOffsetX = 1.25, float titleOffsetY = 1.75);

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
TH1* Graph2Histogram(TGraphAsymmErrors* graph)
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

#endif /* TH1UTIL_H_ */
