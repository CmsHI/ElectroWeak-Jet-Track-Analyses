/*
 * utilities related to TH1 objects.
 */

#include <TH1.h>
#include <TH1D.h>
#include <TString.h>

#include "interface/InputConfigurationParser.h"

#ifndef TH1UTIL_H_
#define TH1UTIL_H_

float resetTH1axisMin4LogScale(float axisMin, std::string axis);
std::string  summaryTH1(TH1* h);

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

#endif /* TH1UTIL_H_ */
