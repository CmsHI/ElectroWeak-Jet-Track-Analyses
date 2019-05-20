/*
 * macro to parse information from XML file produced by TMVA classification
 * -- prints cut value for the given signal efficiency
 */
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"

#include "TMVA/IMethod.h"
#include "TMVA/MethodBase.h"
#include "TMVA/MethodCuts.h"
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"

#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <iomanip>
#include <algorithm>

#include "../Utilities/systemUtil.h"
#include "../Utilities/interface/ArgumentParser.h"
#include "interface/tmvaAnalyzer.h"

std::vector<std::string> argOptions;

void tmvaParseXML(std::string fileXML, std::string methodName, std::string variablesStr, double sigEff);

void tmvaParseXML(std::string fileXML, std::string methodName, std::string variablesStr, double sigEff)
{
    std::cout << "##### Parameters #####" << std::endl;
    std::cout << "running tmvaParseXML()" << std::endl;
    std::cout << "fileXML = " << fileXML.c_str() << std::endl;
    std::cout << "methodName = " << methodName.c_str() << std::endl;
    std::cout << "variables = " << variablesStr.c_str() << std::endl;
    std::cout << "signal efficiency = " << sigEff << std::endl;
    std::cout << "##### Parameters - END #####" << std::endl;

    std::string spectatorsStr = (ArgumentParser::ParseOptionInputSingle("--spectators", argOptions).size() > 0) ?
            ArgumentParser::ParseOptionInputSingle("--spectators", argOptions).c_str() : "";

    std::cout << "##### Optional Arguments #####" << std::endl;
    std::cout << "spectators = " << spectatorsStr.c_str() << std::endl;
    std::cout << "##### Optional Arguments - END #####" << std::endl;

    std::vector<std::string> variables = split(variablesStr, ",", false, false);
    int nVariables = variables.size();

    std::cout << "nVariables = " << nVariables << std::endl;
    for (int i = 0; i < nVariables; ++i) {
        std::cout << Form("variables[%d] = %s", i, variables.at(i).c_str()) << std::endl;
    }

    std::vector<std::string> spectators = split(spectatorsStr, ",", false, false);
    int nSpectators = spectators.size();

    std::cout << "nSpectators = " << nSpectators << std::endl;
    for (int i = 0; i < nSpectators; ++i) {
        std::cout << Form("spectators[%d] = %s", i, spectators.at(i).c_str()) << std::endl;
    }

    TMVA::Reader *reader = new TMVA::Reader("!Color");

    float varF[nVariables];
    for (int i = 0; i < nVariables; ++i) {
        reader->AddVariable(variables[i].c_str(), &(varF[i]));
    }

    float specF[nSpectators];
    for (int i = 0; i < nSpectators; ++i) {
        reader->AddSpectator(spectators[i].c_str(), &(specF[i]));
    }

    reader->BookMVA(methodName.c_str(), fileXML.c_str());
    TMVA::MethodCuts* mCuts = dynamic_cast<TMVA::MethodCuts*>(reader->FindCutsMVA(methodName.c_str()));

    std::vector<double> cutMins;
    std::vector<double> cutMaxs;
    mCuts->GetCuts(sigEff, cutMins, cutMaxs);

    int nCutMins = cutMins.size();
    int nCutMaxs = cutMaxs.size();

    std::cout << "nCutMins = " << nCutMins << std::endl;
    std::cout << "nCutMaxs = " << nCutMaxs << std::endl;
    if (nCutMins != nCutMaxs) {
        std::cout << "ERROR : mismatch between number of minimum and maximum cuts" << std::endl;
        std::cout << "exiting" << std::endl;
        return;
    }
    if (nCutMins != nVariables) {
        std::cout << "ERROR : mismatch between number of cuts and variables" << std::endl;
        std::cout << "exiting" << std::endl;
        return;
    }

    for (int i = 0; i < nVariables; ++i) {
        std::cout << Form("%s :", variables.at(i).c_str()) << std::endl;
        std::cout << Form("min : %f , max : %f", cutMins[i], cutMaxs[i]) << std::endl;
    }

    std::cout << "running tmvaParseXML() - END" << std::endl;
}

int main(int argc, char** argv)
{
    std::vector<std::string> argStr = ArgumentParser::ParseParameters(argc, argv);
    int nArgStr = argStr.size();

    argOptions = ArgumentParser::ParseOptions(argc, argv);

    if (nArgStr == 5) {
        tmvaParseXML(argv[1], argv[2], argv[3], std::atof(argv[4]));
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./tmvaParseXML.exe <fileXML> <variables>"
                << std::endl;
        std::cout << "Options are" << std::endl;
        std::cout << "spectators=<comma separated list of spectators" << std::endl;
        return 1;
    }
}
