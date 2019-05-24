/*
 * macro to read information from XML file produced by TMVA classification
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

void tmvaReadXML(std::string fileXML, std::string methodName, std::string variablesStr, std::string outputFile);

void tmvaReadXML(std::string fileXML, std::string methodName, std::string variablesStr, std::string outputFile)
{
    std::cout << "##### Parameters #####" << std::endl;
    std::cout << "running tmvaReadXML()" << std::endl;
    std::cout << "fileXML = " << fileXML.c_str() << std::endl;
    std::cout << "methodName = " << methodName.c_str() << std::endl;
    std::cout << "variables = " << variablesStr.c_str() << std::endl;
    std::cout << "outputFile = " << outputFile.c_str() << std::endl;
    std::cout << "##### Parameters - END #####" << std::endl;

    std::string spectatorsStr = (ArgumentParser::ParseOptionInputSingle("--spectators", argOptions).size() > 0) ?
            ArgumentParser::ParseOptionInputSingle("--spectators", argOptions).c_str() : "";
    std::string sigEffStr = (ArgumentParser::ParseOptionInputSingle("--signalEffs", argOptions).size() > 0) ?
            ArgumentParser::ParseOptionInputSingle("--signalEffs", argOptions).c_str() : "";

    std::cout << "##### Optional Arguments #####" << std::endl;
    std::cout << "spectators = " << spectatorsStr.c_str() << std::endl;
    std::cout << "signal efficiencies = " << sigEffStr.c_str() << std::endl;
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

    std::vector<std::string> sigEffStrVec = split(sigEffStr, ",", false, false);
    std::vector<float> sigEffs;
    for (std::vector<std::string>::iterator it = sigEffStrVec.begin() ; it != sigEffStrVec.end(); ++it) {
        sigEffs.push_back(std::atof((*it).c_str()));
    }
    int nSigEffs = sigEffs.size();
    std::cout << "nSigEffs = " << nSigEffs << std::endl;
    for (int i = 0; i < nSigEffs; ++i) {
        std::cout << Form("sigEffs[%d] = %.3f", i, sigEffs.at(i)) << std::endl;
    }

    TFile* output = TFile::Open(outputFile.c_str(),"RECREATE");
    output->cd();

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

    for (int i = 0; i < nSigEffs; ++i) {
        std::cout << Form("signal efficiency = %.3f", sigEffs.at(i)) << std::endl;

        std::vector<double> cutMins;
        std::vector<double> cutMaxs;
        mCuts->GetCuts(sigEffs.at(i), cutMins, cutMaxs);

        int nCutMins = cutMins.size();
        int nCutMaxs = cutMaxs.size();

        if (nCutMins != nCutMaxs) {
            std::cout << "ERROR : mismatch between number of minimum and maximum cuts" << std::endl;
            std::cout << "nCutMins = " << nCutMins << std::endl;
            std::cout << "nCutMaxs = " << nCutMaxs << std::endl;
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
    }

    output->cd();

    // plot variable minimum/maximum cut as function of signal efficiency
    std::vector<TH1D*> h_var_cutMin_vs_sigEff(nVariables, 0);
    std::vector<TH1D*> h_var_cutMax_vs_sigEff(nVariables, 0);
    int nBinsSigEff = 100;
    for (int i = 0; i < nVariables; ++i) {
        h_var_cutMin_vs_sigEff[i] = new TH1D(Form("h_var%d_cutMin_vs_sigEff", i), Form(";signal efficiency;minimum of %s", variables[i].c_str()), nBinsSigEff, 0, 1);
        h_var_cutMax_vs_sigEff[i] = new TH1D(Form("h_var%d_cutMax_vs_sigEff", i), Form(";signal efficiency;maximum of %s", variables[i].c_str()), nBinsSigEff, 0, 1);
    }

    for (int i = 0; i < nBinsSigEff; ++i) {
        double sigEffTmp = (double)(i) / 100;

        std::vector<double> cutMins;
        std::vector<double> cutMaxs;
        mCuts->GetCuts(sigEffTmp, cutMins, cutMaxs);

        for (int iVar = 0; iVar < nVariables; ++iVar) {
            h_var_cutMin_vs_sigEff[iVar]->SetBinContent(i+1, cutMins[iVar]);
            h_var_cutMin_vs_sigEff[iVar]->SetBinError(i+1, 0.00001);

            h_var_cutMax_vs_sigEff[iVar]->SetBinContent(i+1, cutMaxs[iVar]);
            h_var_cutMax_vs_sigEff[iVar]->SetBinError(i+1, 0.00001);
        }
    }

    // create copies of histograms named using name of the variable, not its index
    TH1D* hTmp = 0;
    for (int i = 0; i < nVariables; ++i) {
        std::string tmpName = "";

        tmpName = replaceAll(h_var_cutMin_vs_sigEff[i]->GetName(), Form("_var%d", i), Form("_%s", variables[i].c_str()));
        hTmp = (TH1D*)h_var_cutMin_vs_sigEff[i]->Clone(tmpName.c_str());
        hTmp->Write("",TObject::kOverwrite);

        tmpName = replaceAll(h_var_cutMax_vs_sigEff[i]->GetName(), Form("_var%d", i), Form("_%s", variables[i].c_str()));
        hTmp = (TH1D*)h_var_cutMax_vs_sigEff[i]->Clone(tmpName.c_str());
        hTmp->Write("",TObject::kOverwrite);
    }

    std::cout<<"Writing the output file."<<std::endl;
    output->Write("",TObject::kOverwrite);
    std::cout<<"Closing the output file."<<std::endl;
    output->Close();

    std::cout << "running tmvaReadXML() - END" << std::endl;
}

int main(int argc, char** argv)
{
    std::vector<std::string> argStr = ArgumentParser::ParseParameters(argc, argv);
    int nArgStr = argStr.size();

    argOptions = ArgumentParser::ParseOptions(argc, argv);

    if (nArgStr == 5) {
        tmvaReadXML(argv[1], argv[2], argv[3], argv[4]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./tmvaReadXML.exe <fileXML> <method name> <variables> <output file> <signal efficiencies>"
                << std::endl;
        std::cout << "Options are" << std::endl;
        std::cout << "spectators=<comma separated list of spectators>" << std::endl;
        std::cout << "signalEffs=<signal efficiencies for which cuts are to be printed>" << std::endl;
        return 1;
    }
}
