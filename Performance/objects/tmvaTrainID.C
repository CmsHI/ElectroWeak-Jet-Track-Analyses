/*
 * code for training and testing TMVA classifiers
 * derived from tutorial code TMVAClassification.C
 */

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"

#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Tools.h"
#include "TMVA/TMVAGui.h"

#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <iomanip>
#include <algorithm>

#include "../../Utilities/interface/InputConfigurationParser.h"
#include "../../Utilities/interface/ArgumentParser.h"
#include "../../Utilities/interface/ConfigurationParser.h"
#include "../interface/tmvaAnalyzer.h"

///// global variables
/// configuration variables

// TTree
std::string treePathS;
std::string treePathB;

std::vector<std::string> treeBranchesS;
std::vector<std::string> treeBranchesB;
std::vector<std::string> treeBranchesSpec;

std::string tmvaFactoryOptions;
std::vector<std::pair<std::string, int>> tmvaMethodsList;
std::vector<std::string> tmvaMethodTypes;
std::vector<std::string> tmvaMethodTitles;
std::vector<std::string> tmvaMethodBaseOptions;
int nTmvaMethods;

std::string preselectionS;
std::string preselectionB;

float fracTrainEvtS;
float fracTrainEvtB;

std::vector<std::pair<std::string, int>> trainVarsList;

int nTreeBranchesS;
int nTreeBranchesB;
int nTreeBranchesSpec;

/// configuration variables - END
std::vector<TMVAANA::trainVar> trainVars;
int nTrainVars;
///// global variables - END

int readConfiguration(std::string configFile, std::string inputFile);
void printConfiguration();
void setBranchesStatus(TTree* t, std::vector<std::string> branchList);
std::vector<std::string> parseTmvaMethodTypes(std::vector<std::pair<std::string, int>> listOfList);
std::vector<std::string> parseTmvaMethodTitles(std::vector<std::pair<std::string, int>> listOfList);
std::vector<std::string> parseTmvaMethodBaseOptions(std::vector<std::pair<std::string, int>> listOfList);
int tmvaTrainID(std::string configFile, std::string signalFile, std::string backgroundFile, std::string outputFile = "tmvaTrainID.root", std::string jobLabel = "");

int tmvaTrainID(std::string configFile, std::string signalFile, std::string backgroundFile, std::string outputFile, std::string jobLabel)
{
    std::cout << "configFile = " << configFile.c_str() << std::endl;
    std::cout << "signalFile = " << signalFile.c_str() << std::endl;
    std::cout << "backgroundFile = " << backgroundFile.c_str() << std::endl;
    std::cout << "outputFile = " << outputFile.c_str() << std::endl;
    std::cout << "jobLabel = " << jobLabel.c_str() << std::endl;

    if (readConfiguration(configFile, signalFile) != 0)  return -1;
    printConfiguration();

    std::vector<std::string> signalFiles = InputConfigurationParser::ParseFiles(signalFile.c_str());
    std::cout << "signal ROOT files : num = " << signalFiles.size() << std::endl;
    std::cout << "#####" << std::endl;
    for (std::vector<std::string>::iterator it = signalFiles.begin() ; it != signalFiles.end(); ++it) {
        std::cout << (*it).c_str() << std::endl;
    }
    std::cout << "##### END #####" << std::endl;

    std::vector<std::string> backgroundFiles = InputConfigurationParser::ParseFiles(backgroundFile.c_str());
    std::cout << "background ROOT files : num = " << backgroundFiles.size() << std::endl;
    std::cout << "#####" << std::endl;
    for (std::vector<std::string>::iterator it = backgroundFiles.begin() ; it != backgroundFiles.end(); ++it) {
        std::cout << (*it).c_str() << std::endl;
    }
    std::cout << "##### END #####" << std::endl;

    TChain *treeSig = 0;
    TChain *treeBkg = 0;

    treeSig = new TChain(treePathS.c_str());
    treeBkg = new TChain(treePathB.c_str());

    for (std::vector<std::string>::iterator it = signalFiles.begin() ; it != signalFiles.end(); ++it) {
        treeSig->Add((*it).c_str());
    }
    for (std::vector<std::string>::iterator it = backgroundFiles.begin() ; it != backgroundFiles.end(); ++it) {
        treeBkg->Add((*it).c_str());
    }

    Long64_t entriesSig = treeSig->GetEntries();
    Long64_t entriesBkg = treeBkg->GetEntries();

    setBranchesStatus(treeSig, treeBranchesS);
    setBranchesStatus(treeBkg, treeBranchesB);

    TFile* output = 0;
    output = TFile::Open(outputFile.c_str(), "RECREATE" );

    // Create the factory object. Later you can choose the methods
    // whose performance you'd like to investigate. The factory is
    // the only TMVA object you have to interact with
    //
    // The first argument is the base of the name of all the
    // weightfiles in the directory weight/
    //
    // The second argument is the output file for the training results
    // All TMVA output can be suppressed by removing the "!" (not) in
    // front of the "Silent" argument in the option string

    TMVA::Factory* factory = 0;
    factory = new TMVA::Factory(jobLabel.c_str(), output, tmvaFactoryOptions.c_str());

    TMVA::DataLoader* dataloader = 0;
    dataloader = new TMVA::DataLoader("dataset");

    /*
    dataloader->SetSignalTree(treeSig);
    dataloader->SetSignalWeightExpression("weight");

    dataloader->SetBackgroundTree(treeBkg);
    dataloader->SetBackgroundWeightExpression("weight");
    */

    double weightSig = 1;
    double weightBkg = 1;
    dataloader->AddSignalTree(treeSig, weightSig);
    dataloader->AddBackgroundTree(treeBkg, weightBkg);

    /*
    dataloader->AddSpectator("phoEt");
    */
    for (int i = 0; i < nTreeBranchesSpec; ++i) {
        dataloader->AddSpectator(treeBranchesSpec[i].c_str());
    }

    /*
    TCut("phoEt > 40 && abs(phoSCEta) < 1.48 && phoHoverE < 0.1 && pho_genMatchedIndex > -1 && mcPID[pho_genMatchedIndex] == 22")
    */
    TCut preselS = TCut(preselectionS.c_str());
    TCut preselB = TCut(preselectionB.c_str());

    std::cout << "preselectionSig = " << preselS.GetTitle() << std::endl;
    std::cout << "preselectionBkg = " << preselB.GetTitle() << std::endl;

    Long64_t entriesPreSelSig = treeSig->GetEntries(preselS.GetTitle());
    Long64_t entriesPreSelBkg = treeBkg->GetEntries(preselB.GetTitle());

    std::cout << "entriesSig = " << entriesSig << std::endl;
    std::cout << "entriesBkg = " << entriesBkg << std::endl;
    std::cout << "entriesPreSelSig = " << entriesPreSelSig << std::endl;
    std::cout << "entriesPreSelBkg = " << entriesPreSelBkg << std::endl;

    int nTrainSig = entriesPreSelSig * fracTrainEvtS;
    int nTrainBkg = entriesPreSelBkg * fracTrainEvtB;
    std::string splitOption = Form("nTrain_Signal=%d:nTrain_Background=%d:SplitMode=Random:SplitSeed=12345:NormMode=NumEvents", nTrainSig, nTrainBkg);

    dataloader->PrepareTrainingAndTestTree(preselS, preselB, splitOption.c_str());

    /*
    trainVars.push_back(TMVAANA::trainVar("phoSigmaIEtaIEta_2012", "F", "FSmart", 0, 0.015));
    trainVars.push_back(TMVAANA::trainVar("sumIso := pho_ecalClusterIsoR4 + pho_hcalRechitIsoR4 + pho_trackIsoR4PtCut20", "F", "FSmart", -400, 50));
    */
    std::string methodOptionsTrainVar = "";
    for (int i = 0; i < nTrainVars; ++i) {
        TMVAANA::trainVar varTmp = trainVars.at(i);
        dataloader->AddVariable(varTmp.expression, varTmp.type.at(0));

        if (varTmp.varProp.size() > 0) {
            methodOptionsTrainVar += Form("VarProp[%d]=%s:", i, varTmp.varProp.c_str());
        }
        if (varTmp.cutRangeMin != 999999) {
            methodOptionsTrainVar += Form("CutRangeMin[%d]=%f:", i, varTmp.cutRangeMin);
        }
        if (varTmp.cutRangeMax != 999999) {
            methodOptionsTrainVar += Form("CutRangeMax[%d]=%f:", i, varTmp.cutRangeMax);
        }
    }
    methodOptionsTrainVar = methodOptionsTrainVar.substr(0, methodOptionsTrainVar.size()-1);    // trim last character, ":"

    for (int i = 0; i < nTmvaMethods; ++i) {

        int methodTypeCode = parseTmvaMethodType(tmvaMethodTypes[i].c_str());

        std::string methodOption = tmvaMethodBaseOptions[i];
        if (methodOptionsTrainVar.size() > 0 && (TMVA::Types::EMVA)methodTypeCode == TMVA::Types::EMVA::kCuts)
        {
            methodOption = Form("%s:%s", tmvaMethodBaseOptions[i].c_str(), methodOptionsTrainVar.c_str());
        }

        std::cout << "Method type : " << tmvaMethodTypes[i].c_str() << " , type code : " << methodTypeCode << std::endl;
        std::cout << "Method title : " << tmvaMethodTitles[i].c_str() << std::endl;
        std::cout << "Final method option : " << methodOption.c_str() << std::endl;

        factory->BookMethod(dataloader, (TMVA::Types::EMVA)methodTypeCode, tmvaMethodTitles[i].c_str(), methodOption.c_str());
    }

    // Train MVAs using the set of training events
    factory->TrainAllMethods();

    // Evaluate all MVAs using the set of test events
    factory->TestAllMethods();

    // Evaluate and compare performance of all configured MVAs
    factory->EvaluateAllMethods();

    output->Close();

    std::cout << "==> Wrote root file: " << output->GetName() << std::endl;
    std::cout << "==> TMVAClassification is done!" << std::endl;

    return 0;
}

int main(int argc, char** argv)
{
    if (argc == 6) {
        tmvaTrainID(argv[1], argv[2], argv[3], argv[4], argv[5]);
        return 0;
    }
    else if (argc == 5) {
        tmvaTrainID(argv[1], argv[2], argv[3], argv[4]);
        return 0;
    }
    else if (argc == 4) {
        tmvaTrainID(argv[1], argv[2], argv[3]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./tmvaTrainID.exe <arg1> <arg2> <arg3> <arg4> <arg5>"
                << std::endl;
        return 1;
    }
}

int readConfiguration(std::string configFile, std::string inputFile)
{
    ConfigurationParser confParser;
    confParser.openConfigFile(configFile);

    confParser.parsedKeyWords = InputConfigurationParser::parseKeyWords(inputFile);

    // TTree
    treePathS = confParser.ReadConfigValue("treePathSig");
    treePathB = confParser.ReadConfigValue("treePathBkg");

    treeBranchesS = ConfigurationParser::ParseListOrString(confParser.ReadConfigValue("treeSigBranches"));
    treeBranchesB = ConfigurationParser::ParseListOrString(confParser.ReadConfigValue("treeBkgBranches"));
    treeBranchesSpec = ConfigurationParser::ParseListOrString(confParser.ReadConfigValue("treeSpectatorBranches"));

    tmvaFactoryOptions = confParser.ReadConfigValue("tmvaFactoryOptions");
    tmvaMethodsList = ConfigurationParser::ParseListOfList(confParser.ReadConfigValue("tmvaMethods"));
    tmvaMethodTypes = parseTmvaMethodTypes(tmvaMethodsList);
    tmvaMethodTitles = parseTmvaMethodTitles(tmvaMethodsList);
    tmvaMethodBaseOptions = parseTmvaMethodBaseOptions(tmvaMethodsList);

    preselectionS = confParser.ReadConfigValue("preselectionSig");
    preselectionB = confParser.ReadConfigValue("preselectionBkg");

    fracTrainEvtS = confParser.ReadConfigValueFloat("fracTrainEvtSig");
    fracTrainEvtB = confParser.ReadConfigValueFloat("fracTrainEvtBkg");

    // list of training variables separated by ";;;"
    trainVarsList = ConfigurationParser::ParseListOfList(confParser.ReadConfigValue("trainVariables"));
    int indexTmp = trainVarsList.size() - 1;
    int nVarsTmp = trainVarsList.at(indexTmp).second + 1;
    int j = 0;
    for (int iVar = 0; iVar < nVarsTmp; ++iVar){

        TMVAANA::trainVar varTmp;
        varTmp.expression = trainVarsList.at(j).first;
        j++;

        if (j <= indexTmp && trainVarsList.at(j).second == iVar) {
            varTmp.type = trainVarsList.at(j).first;
            j++;
        }
        if (j <= indexTmp && trainVarsList.at(j).second == iVar) {
            varTmp.varProp = trainVarsList.at(j).first;
            j++;
        }
        if (j <= indexTmp && trainVarsList.at(j).second == iVar) {
            varTmp.cutRangeMin = std::atof(trainVarsList.at(j).first.c_str());
            j++;
        }
        if (j <= indexTmp && trainVarsList.at(j).second == iVar) {
            varTmp.cutRangeMax = std::atof(trainVarsList.at(j).first.c_str());
            j++;
        }

        trainVars.push_back(varTmp);
    }

    if (treePathS == "") {
        treePathS = "ggHiNtuplizerGED/EventTree";
    }
    if (treePathB == "") {
        treePathB = "ggHiNtuplizerGED/EventTree";
    }

    if (treeBranchesS.size() == 0) {
        treeBranchesS.push_back("*");
    }
    if (treeBranchesB.size() == 0) {
        treeBranchesB.push_back("*");
    }

    if (tmvaFactoryOptions == "") {
        /*
        tmvaFactoryOptions = "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification";
        */
        tmvaFactoryOptions = "!V:!Silent:Color=False:DrawProgressBar:AnalysisType=Classification";
    }
    if (tmvaMethodTitles.size() == 0) {
        tmvaMethodTypes.push_back("kCuts");
        tmvaMethodTitles.push_back("");

        // "!H:!V:FitMethod=GA:EffMethod=EffSEl:PopSize=800:Steps=60"
        tmvaMethodBaseOptions.push_back("!H:!V:FitMethod=GA:EffMethod=EffSEl");
    }

    if (fracTrainEvtS <= 0) {
        fracTrainEvtS = 0.5;
    }
    if (fracTrainEvtB <= 0) {
        fracTrainEvtB = 0.5;
    }

    nTmvaMethods = tmvaMethodTitles.size();

    nTreeBranchesS = treeBranchesS.size();
    nTreeBranchesB = treeBranchesB.size();
    nTreeBranchesSpec = treeBranchesSpec.size();

    nTrainVars = trainVars.size();

    return 0;
}

/*
 * print information read from configuration
 * assumes that readConfiguration() is run before
 */
void printConfiguration()
{
    std::cout<<"Configuration :"<<std::endl;

    std::cout << "treePathS = " << treePathS.c_str() << std::endl;
    std::cout << "treePathB = " << treePathB.c_str() << std::endl;

    // branches activated in signal and background trees
    std::cout << "nTreeBranchesS = " << nTreeBranchesS << std::endl;
    for (int i = 0; i < nTreeBranchesS; ++i) {
        std::cout << Form("treeBranchesS[%d] = %s", i, treeBranchesS.at(i).c_str()) << std::endl;
    }

    std::cout << "nTreeBranchesB = " << nTreeBranchesB << std::endl;
    for (int i = 0; i < nTreeBranchesB; ++i) {
        std::cout << Form("treeBranchesB[%d] = %s", i, treeBranchesB.at(i).c_str()) << std::endl;
    }

    std::cout << "nTreeBranchesSpec = " << nTreeBranchesSpec << std::endl;
    for (int i = 0; i < nTreeBranchesSpec; ++i) {
        std::cout << Form("treeBranchesSpec[%d] = %s", i, treeBranchesSpec.at(i).c_str()) << std::endl;
    }

    std::cout << "tmvaFactoryOptions = " << tmvaFactoryOptions.c_str() << std::endl;

    std::cout << "nTmvaMethods = " << nTmvaMethods << std::endl;
    for (int i = 0; i < nTmvaMethods; ++i){

        std::cout << Form("method[%d] : ", i) << std::endl;
        std::cout << Form(" type = %s , title = %s , base option = %s",
                            tmvaMethodTypes[i].c_str(),
                            tmvaMethodTitles[i].c_str(),
                            tmvaMethodBaseOptions[i].c_str()) << std::endl;
    }

    std::cout << "preselectionS = " << preselectionS.c_str() << std::endl;
    std::cout << "preselectionB = " << preselectionB.c_str() << std::endl;

    std::cout << "fracTrainEvtS = " << fracTrainEvtS << std::endl;
    std::cout << "fracTrainEvtB = " << fracTrainEvtB << std::endl;

    std::cout << "nTrainVars = " << nTrainVars << std::endl;
    for (int i = 0; i < nTrainVars; ++i){

        std::cout << Form("trainVar[%d] : ", i) << std::endl;
        std::cout << Form(" expression = %s", trainVars[i].expression.c_str()) << std::endl;
        std::cout << Form(" type = %s , varProp = %s , min = %f , max = %f",
                            trainVars[i].type.c_str(),
                            trainVars[i].varProp.c_str(),
                            trainVars[i].cutRangeMin,
                            trainVars[i].cutRangeMax) << std::endl;
    }
}

/*
t->SetBranchStatus("*", 0);
t->SetBranchStatus("nPho", 1);
t->SetBranchStatus("pho*", 1);
t->SetBranchStatus("nMC", 1);
t->SetBranchStatus("mc*", 1);
*/
void setBranchesStatus(TTree* t, std::vector<std::string> branchList)
{
    t->SetBranchStatus("*", 0);
    for (std::vector<std::string>::iterator it = branchList.begin(); it != branchList.end(); ++it) {
        t->SetBranchStatus((*it).c_str(), 1);
    }
}

std::vector<std::string> parseTmvaMethodTypes(std::vector<std::pair<std::string, int>> listOfList)
{

    std::vector<std::string> res;

    int n = listOfList.size();
    for (int i = 0; i < n; i+=3) {
        res.push_back(listOfList[i].first);
    }

    return res;
}

std::vector<std::string> parseTmvaMethodTitles(std::vector<std::pair<std::string, int>> listOfList)
{

    std::vector<std::string> res;

    int n = listOfList.size();
    for (int i = 0; i < n; i+=3) {
        res.push_back(listOfList[i+1].first);
    }

    return res;
}


std::vector<std::string> parseTmvaMethodBaseOptions(std::vector<std::pair<std::string, int>> listOfList)
{
    std::vector<std::string> res;

    int n = listOfList.size();
    for (int i = 0; i < n; i+=3) {
        res.push_back(listOfList[i+2].first);
    }

    return res;
}
