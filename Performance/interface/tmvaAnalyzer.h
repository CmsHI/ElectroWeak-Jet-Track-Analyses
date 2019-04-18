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

int parseTmvaMethodType(std::string methodType);

int parseTmvaMethodType(std::string methodType)
{
    if (methodType == "kVariable") {
        return TMVA::Types::EMVA::kVariable;
    }
    else if (methodType == "kCuts") {
        return TMVA::Types::EMVA::kCuts;
    }
    else if (methodType == "kLikelihood") {
        return TMVA::Types::EMVA::kLikelihood;
    }
    else if (methodType == "kPDERS") {
        return TMVA::Types::EMVA::kPDERS;
    }
    else if (methodType == "kHMatrix") {
        return TMVA::Types::EMVA::kHMatrix;
    }
    else if (methodType == "kFisher") {
        return TMVA::Types::EMVA::kFisher;
    }
    else if (methodType == "kKNN") {
        return TMVA::Types::EMVA::kKNN;
    }
    else if (methodType == "kCFMlpANN") {
        return TMVA::Types::EMVA::kCFMlpANN;
    }
    else if (methodType == "kTMlpANN") {
        return TMVA::Types::EMVA::kTMlpANN;
    }
    else if (methodType == "kBDT") {
        return TMVA::Types::EMVA::kBDT;
    }
    else if (methodType == "kDT") {
        return TMVA::Types::EMVA::kDT;
    }
    else if (methodType == "kRuleFit") {
        return TMVA::Types::EMVA::kRuleFit;
    }
    else if (methodType == "kSVM") {
        return TMVA::Types::EMVA::kSVM;
    }
    else if (methodType == "kMLP") {
        return TMVA::Types::EMVA::kMLP;
    }
    else if (methodType == "kBayesClassifier") {
        return TMVA::Types::EMVA::kBayesClassifier;
    }
    else if (methodType == "kFDA") {
        return TMVA::Types::EMVA::kFDA;
    }
    else if (methodType == "kBoost") {
        return TMVA::Types::EMVA::kBoost;
    }
    else if (methodType == "kPDEFoam") {
        return TMVA::Types::EMVA::kPDEFoam;
    }
    else if (methodType == "kLD") {
        return TMVA::Types::EMVA::kLD;
    }
    else if (methodType == "kPlugins") {
        return TMVA::Types::EMVA::kPlugins;
    }
    else if (methodType == "kCategory") {
        return TMVA::Types::EMVA::kCategory;
    }
    else if (methodType == "kMaxMethod") {
        return TMVA::Types::EMVA::kMaxMethod;
    }
    else if (methodType == "kPyRandomForest") {
        return TMVA::Types::EMVA::kPyRandomForest;
    }
    else if (methodType == "kPyAdaBoost") {
        return TMVA::Types::EMVA::kPyAdaBoost;
    }
    else if (methodType == "kPyGTB") {
        return TMVA::Types::EMVA::kPyGTB;
    }
    else if (methodType == "kC50") {
        return TMVA::Types::EMVA::kC50;
    }
    else if (methodType == "kRSNNS") {
        return TMVA::Types::EMVA::kRSNNS;
    }
    else if (methodType == "kRSVM") {
        return TMVA::Types::EMVA::kRSVM;
    }
    else if (methodType == "kRXGB") {
        return TMVA::Types::EMVA::kRXGB;
    }
    else {
        return -1;
    }
}

#endif
