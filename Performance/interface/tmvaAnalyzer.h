/*
 * general purpose class for TMVA analysis
 */

#ifndef PERFORMANCE_INTERFACE_TMVAANALYZER_H_
#define PERFORMANCE_INTERFACE_TMVAANALYZER_H_

#include <string>
#include <vector>
#include <map>

#include "../../Utilities/systemUtil.h"

namespace TMVAANA {

struct trainVar
{
    trainVar() {
        this->expression = "";
        this->type = "";
        this->varProp = "";
        this->cutRangeMin = 999999;
        this->cutRangeMax = 999999;
    }
    trainVar(std::string expression, std::string type, std::string varProp, double cutRangeMin, double cutRangeMax) {
        this->expression = expression;
        this->type = type;
        this->varProp = varProp;
        this->cutRangeMin = cutRangeMin;
        this->cutRangeMax = cutRangeMax;
    }

    std::string expression;
    std::string type;       // char = string with only one character
    std::string varProp;
    double cutRangeMin;
    double cutRangeMax;
};

};

class tmvaAnalyzer {
public :
    tmvaAnalyzer(){

    };
    ~tmvaAnalyzer(){};

};

#endif
