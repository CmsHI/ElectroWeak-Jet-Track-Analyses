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
#include "../interface/tmvaAnalyzer.h"

int tmvaTrainID(std::string configFile, std::string signalFile, std::string backgroundFile, std::string outputFile = "tmvaTrainID.root", std::string methodLabel = "");

int tmvaTrainID(std::string configFile, std::string signalFile, std::string backgroundFile, std::string outputFile, std::string methodLabel)
{
    std::cout << "configFile = " << configFile.c_str() << std::endl;
    std::cout << "signalFile = " << signalFile.c_str() << std::endl;
    std::cout << "backgroundFile = " << backgroundFile.c_str() << std::endl;
    std::cout << "outputFile = " << outputFile.c_str() << std::endl;
    std::cout << "methodLabel = " << methodLabel.c_str() << std::endl;

    bool isInputFlatTree = false;
    std::string treePathSig = "treeSig";
    std::string treePathBkg = "treeBkg";
    if (!isInputFlatTree) {
        treePathSig = "ggHiNtuplizerGED/EventTree";
        treePathBkg = "ggHiNtuplizerGED/EventTree";
    }

    std::cout << "treePathSig = " << treePathSig.c_str() << std::endl;
    std::cout << "treePathBkg = " << treePathBkg.c_str() << std::endl;

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

    // Register the training and test trees
//    TFile* inputSig = 0;
//    inputSig = TFile::Open(signalFile.c_str(), "READ");

//    TFile* inputBkg = 0;
//    inputBkg = TFile::Open(backgroundFile.c_str(), "READ");

    TChain *treeSig = 0;
    TChain *treeBkg = 0;

    treeSig = new TChain(treePathSig.c_str());
    treeBkg = new TChain(treePathBkg.c_str());

    for (std::vector<std::string>::iterator it = signalFiles.begin() ; it != signalFiles.end(); ++it) {
        treeSig->Add((*it).c_str());
    }
    for (std::vector<std::string>::iterator it = backgroundFiles.begin() ; it != backgroundFiles.end(); ++it) {
        treeBkg->Add((*it).c_str());
    }

    Long64_t entriesSig = treeSig->GetEntries();
    Long64_t entriesBkg = treeBkg->GetEntries();

    treeSig->SetBranchStatus("*", 0);
    treeSig->SetBranchStatus("nPho", 1);
    treeSig->SetBranchStatus("pho*", 1);
    treeSig->SetBranchStatus("nMC", 1);
    treeSig->SetBranchStatus("mc*", 1);

    treeBkg->SetBranchStatus("*", 0);
    treeBkg->SetBranchStatus("nPho", 1);
    treeBkg->SetBranchStatus("pho*", 1);
    treeBkg->SetBranchStatus("nMC", 1);
    treeBkg->SetBranchStatus("mc*", 1);

    // Create a ROOT output file where TMVA will store ntuples, histograms, etc.
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

    //std::string factoryOptions = "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification";
    //std::string factoryOptions = "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;G,D:AnalysisType=Classification";
    std::string factoryOptions = "!V:!Silent:Color=False:DrawProgressBar:AnalysisType=Classification";

    TMVA::Factory* factory = 0;
    factory = new TMVA::Factory(Form("tmvaTrainIDFactoryJob_method%s", methodLabel.c_str()), output, factoryOptions.c_str());

    TMVA::DataLoader* dataloader = 0;
    dataloader = new TMVA::DataLoader("dataset");

    if (isInputFlatTree) {
        dataloader->SetSignalTree(treeSig);
        dataloader->SetSignalWeightExpression("weight");

        dataloader->SetBackgroundTree(treeBkg);
        dataloader->SetBackgroundWeightExpression("weight");
    }
    else {
        double weightSig = 1;
        double weightBkg = 1;
        dataloader->AddSignalTree(treeSig, weightSig);
        dataloader->AddBackgroundTree(treeBkg, weightBkg);
    }

    int phoEtMin = 40;
    // flatTree
    TCut preSelection = Form("phoEt > %d && abs(phoSCEta) < 1.48 && phoHoverE < 0.1", phoEtMin);
    //TCut preSelection = Form("phoEt > %d && abs(phoSCEta) < 1.48", phoEtMin);
    TCut cut_genMatched = "mcPID == 22";
    if (!isInputFlatTree) {
        cut_genMatched = "pho_genMatchedIndex > -1 && mcPID[pho_genMatchedIndex] == 22";
    }

    std::vector<TMVAANA::trainVar> trainVars;
    //trainVars.push_back(TMVAANA::trainVar("phoHoverE", "F", "FSmart", 0, 0.5));
    trainVars.push_back(TMVAANA::trainVar("phoSigmaIEtaIEta_2012", "F", "FSmart", 0, 0.14));
    trainVars.push_back(TMVAANA::trainVar("sumIso := pho_ecalClusterIsoR4 + pho_hcalRechitIsoR4 + pho_trackIsoR4PtCut20", "F", "FSmart", -1000, 50));
    //trainVars.push_back(TMVAANA::trainVar("ecalIso := pho_ecalClusterIsoR4", "F", "FMin", -100, 50));
    //trainVars.push_back(TMVAANA::trainVar("hcalIso := pho_hcalRechitIsoR4", "F", "FMin", -100, 50));
    //trainVars.push_back(TMVAANA::trainVar("trkIso := pho_trackIsoR4PtCut20", "F", "FMin", -100, 50));

    int nTrainVars = trainVars.size();

    dataloader->AddSpectator("phoEt");
    dataloader->AddSpectator("phoEta");
    dataloader->AddSpectator("phoPhi");
    dataloader->AddSpectator("phoHoverE");
    dataloader->AddSpectator("pho_ecalClusterIsoR4");
    dataloader->AddSpectator("pho_hcalRechitIsoR4");
    dataloader->AddSpectator("pho_trackIsoR4PtCut20");
    dataloader->AddSpectator("pho_genMatchedIndex");

    std::string genSigStr = "mcCalIsoDR04 < 5 && (abs(mcMomPID) <= 22 || mcMomPID == -999)";
    if (!isInputFlatTree) {
        genSigStr = "mcCalIsoDR04[pho_genMatchedIndex] < 5 && (abs(mcMomPID[pho_genMatchedIndex]) <= 22 || mcMomPID[pho_genMatchedIndex] == -999)";
    }
    std::string genBkgStr = Form("!(%s)", genSigStr.c_str());
    //    std::string genBkgStr = "abs(mcMomPID[pho_genMatchedIndex]) == 111";

    TCut preSelectionSig = preSelection && cut_genMatched && TCut(genSigStr.c_str());
    TCut preSelectionBkg = preSelection && cut_genMatched && TCut(genBkgStr.c_str());

    std::cout << "preSelectionSig = " << preSelectionSig.GetTitle() << std::endl;
    std::cout << "preSelectionBkg = " << preSelectionBkg.GetTitle() << std::endl;

    Long64_t entriesPreSelSig = treeSig->GetEntries(preSelectionSig.GetTitle());
    Long64_t entriesPreSelBkg = treeBkg->GetEntries(preSelectionBkg.GetTitle());

    std::cout << "entriesSig = " << entriesSig << std::endl;
    std::cout << "entriesBkg = " << entriesBkg << std::endl;
    std::cout << "entriesPreSelSig = " << entriesPreSelSig << std::endl;
    std::cout << "entriesPreSelBkg = " << entriesPreSelBkg << std::endl;

    double splitFactor = 3;
    int nPreSelectSig = entriesPreSelSig;
    int nTrainSig = nPreSelectSig / splitFactor;
    //int nTestSig = nPreSelectSig * (1 - 1/splitFactor);
    int nPreSelectBkg = entriesPreSelBkg;
    int nTrainBkg = nPreSelectBkg / splitFactor;
    //int nTestBkg = nPreSelectBkg * (1 - 1/splitFactor);
    std::string options1 = Form("nTrain_Signal=%d:nTrain_Background=%d:SplitMode=Random:SplitSeed=12345:NormMode=NumEvents", nTrainSig, nTrainBkg);

    dataloader->PrepareTrainingAndTestTree(preSelectionSig, preSelectionBkg, options1.c_str());

    std::string methodOptionsCommon = "";
    methodOptionsCommon += "!H:!V:FitMethod=GA:EffMethod=EffSEl";
    //methodOptionsCommon += "!H:!V:FitMethod=GA:EffMethod=EffSEl:PopSize=1600:Steps=8";
    //methodOptionsCommon += "!H:!V:FitMethod=GA:EffMethod=EffSEl:PopSize=800:Steps=60";

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

    std::string methodOptions = methodOptionsCommon;
    if (methodOptionsTrainVar.size() > 0)
    {
        methodOptions = Form("%s:%s", methodOptionsCommon.c_str(), methodOptionsTrainVar.c_str());
    }

    std::cout << "methodOptions = " << methodOptions.c_str() << std::endl;

    factory->BookMethod(dataloader, TMVA::Types::kCuts, "Cuts_tmvaTrainID", methodOptions.c_str());

    // Now you can tell the factory to train, test, and evaluate the MVAs
    //
    // Train MVAs using the set of training events
    factory->TrainAllMethods();

    // Evaluate all MVAs using the set of test events
    factory->TestAllMethods();

    // Evaluate and compare performance of all configured MVAs
    factory->EvaluateAllMethods();

    // --------------------------------------------------------------

//    inputSig->Close();
//    inputBkg->Close();

    // Save the output
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
