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

#endif /* TH1UTIL_H_ */
