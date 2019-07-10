/*
 * macro to evaluate output of TMVA classification and regression analysis
 */
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH1D.h"
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

#include "../../Utilities/fileUtil.h"
#include "../../Utilities/systemUtil.h"
#include "../../Utilities/interface/InputConfigurationParser.h"
#include "../../Utilities/interface/ArgumentParser.h"
#include "../../Utilities/interface/ConfigurationParser.h"

///// global variables
/// configuration variables
/*
* mode is a string of characters.
* Each character in "mode" is a flag.
* Characters are modes for SigEff, Regression
* If "0", then do not run the corresponding mode
*/
std::string mode;

// TTree
std::string treePath;

std::vector<std::string> variablesT;
std::vector<std::string> variablesR;
std::vector<std::string> spectatorsT;
std::vector<std::string> spectatorsR;
std::vector<std::string> weights;

/*
 * make histograms of expresions and calculate efficiency/rejection as a function of them
 */
std::vector<std::string> formulas;
std::vector<std::string> formulaSels;
std::vector<std::string> formulaWeights;
// nBins, xLow, xUp for the TH1D histogram;
std::vector<CONFIGPARSER::TH1Axis> formula_TH1D_Bins;

// selections for signal/background events
std::vector<std::string> sigSels;
std::vector<std::string> bkgSels;

std::vector<std::string> references;
std::vector<std::string> targets;

std::string preselection;

std::vector<float> sigEffs;

int nVariablesT;
int nVariablesR;
int nSpectatorsT;
int nSpectatorsR;
int nWeights;

int nFormulas;
int nFormulaSels;
int nFormulaWeights;
int nFormula_TH1D_Bins;

int nSigSels;
int nBkgSels;

int nReferences;
int nTargets;

int nSigEffs;

/// configuration variables - END

enum MODES {
    kSigEff,
    kRegression,
    kN_MODES
};
const std::string modesStr[kN_MODES] = {"SigEff", "Regression"};
std::vector<int> runMode;

enum MODES_EFFICIENCY {
    kNULL_EFFICIENCY,
    kPrintEff,
    kDrawEff,
    kN_MODES_EFFICIENCY
};
const std::string modesEfficiencyStr[kN_MODES_EFFICIENCY] = {"NULL_EFFICIENCY", "PrintEff", "DrawEff"};

enum MODES_REGRESSION {
    kNULL_REGRESSION,
    kRegrE,
    kN_MODES_REGRESSION
};
const std::string modesRegressionStr[kN_MODES_REGRESSION] = {"NULL_REGRESSION", "RegrE"};
///// global variables - END

std::vector<std::string> argOptions;

int readConfiguration(std::string configFile, std::string inputFile);
void printConfiguration();
std::vector<int> parseMode(std::string mode);
void setBranchesStatus(TTree* t, std::vector<std::string> branchList);
void tmvaEval(std::string configFile, std::string inputFile, std::string fileXML, std::string outputFile, std::string methodName);

void tmvaEval(std::string configFile, std::string inputFile, std::string fileXML, std::string outputFile, std::string methodName)
{
    std::cout << "##### Parameters #####" << std::endl;
    std::cout << "running tmvaEval()" << std::endl;
    std::cout << "configFile = " << configFile.c_str() << std::endl;
    std::cout << "inputFile = " << inputFile.c_str() << std::endl;
    std::cout << "fileXML = " << fileXML.c_str() << std::endl;
    std::cout << "outputFile = " << outputFile.c_str() << std::endl;
    std::cout << "methodName = " << methodName.c_str() << std::endl;
    std::cout << "##### Parameters - END #####" << std::endl;

    if (readConfiguration(configFile, inputFile) != 0)  return;
    printConfiguration();

    std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.c_str());
    std::cout << "input ROOT files : num = " << inputFiles.size() << std::endl;
    std::cout << "#####" << std::endl;
    for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
        std::cout << (*it).c_str() << std::endl;
    }
    std::cout << "##### END #####" << std::endl;

    TMVA::Reader *reader = new TMVA::Reader("!Color");

    float varsR[nVariablesR];
    for (int i = 0; i < nVariablesR; ++i) {
        reader->AddVariable(variablesR[i].c_str(), &(varsR[i]));
    }

    float specsR[nSpectatorsR];
    for (int i = 0; i < nSpectatorsR; ++i) {
        reader->AddSpectator(spectatorsR[i].c_str(), &(specsR[i]));
    }

    reader->BookMVA(methodName.c_str(), fileXML.c_str());
    TMVA::MethodCuts* mCuts = dynamic_cast<TMVA::MethodCuts*>(reader->FindCutsMVA(methodName.c_str()));

    void* doc = TMVA::gTools().xmlengine().ParseFile(fileXML.c_str(), TMVA::gTools().xmlenginebuffersize());
    void* rootnode = TMVA::gTools().xmlengine().DocGetRootElement(doc);
    void* varsNode = TMVA::gTools().GetChild(rootnode, "Variables");
    void* varNode = 0;

    std::vector<std::string> cutsStr(nSigEffs);
    for (int i = 0; i < nSigEffs; ++i) {
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
        if (nCutMins != nVariablesR) {
            std::cout << "ERROR : mismatch between number of cuts and variables" << std::endl;
            std::cout << "exiting" << std::endl;
            return;
        }

        std::string cutTmp = "";
        varNode = TMVA::gTools().GetChild(varsNode, "Variable");
        for (int i = 0; i < nVariablesR; ++i) {

            TString varLabel;
            TMVA::gTools().ReadAttr(varNode, "Label", varLabel);
            varNode = TMVA::gTools().GetNextChild(varNode);
            std::string cutVarTmp = Form("%f < %s && %s <= %f", cutMins[i], varLabel.Data(), varLabel.Data(), cutMaxs[i]);

            if (cutTmp.size() == 0) {
                cutTmp = cutVarTmp;
            }
            else {
                cutTmp += Form(" && %s", cutVarTmp.c_str());
            }
        }

        cutsStr[i] = cutTmp;
    }

    std::vector<std::vector<std::string>> weight_AND_Sel_num_sig(nFormulas);
    std::vector<std::string> weight_AND_Sel_denom_sig(nFormulas);

    std::vector<std::vector<std::string>> weight_AND_Sel_num_bkg(nFormulas);
    std::vector<std::string> weight_AND_Sel_denom_bkg(nFormulas);
    for (int i = 0; i < nFormulas; ++i) {

        std::string selTmp = "";
        if (nFormulaSels == 1) {
            selTmp = formulaSels[0];
        }
        else if (nFormulaSels == nFormulas) {
            selTmp = formulaSels[i];
        }

        std::string selSigTmp = "";
        if (nSigSels == 1) {
            selSigTmp = sigSels[0];
        }
        else if (nSigSels == nFormulas) {
            selSigTmp = sigSels[i];
        }

        if (selTmp.size() > 0 && selSigTmp.size() > 0) {
            selSigTmp = Form("%s && %s", selTmp.c_str(), selSigTmp.c_str());
        }
        else if (selTmp.size() > 0) {
            selSigTmp = selTmp;
        }

        std::string selBkgTmp = "";
        if (nBkgSels == 1) {
            selBkgTmp = bkgSels[0];
        }
        else if (nBkgSels == nFormulas) {
            selBkgTmp = bkgSels[i];
        }

        if (selTmp.size() > 0 && selBkgTmp.size() > 0) {
            selBkgTmp = Form("%s && %s", selTmp.c_str(), selBkgTmp.c_str());
        }
        else if (selTmp.size() > 0) {
            selBkgTmp = selTmp;
        }

        for (int j = 0; j < nSigEffs; ++j) {

            std::string sel_AND_cut_sig = "";
            if (cutsStr[j].size() == 0 && selSigTmp.size() == 0) {
                sel_AND_cut_sig = "1";
            }
            else if (selSigTmp.size() == 0) {
                sel_AND_cut_sig = cutsStr[j];
            }
            else {
                sel_AND_cut_sig = Form("%s && %s", cutsStr[j].c_str(), selSigTmp.c_str());
            }

            std::string sel_AND_cut_bkg = "";
            if (cutsStr[j].size() == 0 && selBkgTmp.size() == 0) {
                sel_AND_cut_bkg = "1";
            }
            else if (selBkgTmp.size() == 0) {
                sel_AND_cut_bkg = cutsStr[j];
            }
            else {
                sel_AND_cut_bkg = Form("%s && %s", cutsStr[j].c_str(), selBkgTmp.c_str());
            }

            std::string weight_AND_sel_num_sig = sel_AND_cut_sig;
            std::string weight_AND_sel_denom_sig = selSigTmp;
            std::string weight_AND_sel_num_bkg = sel_AND_cut_bkg;
            std::string weight_AND_sel_denom_bkg = selBkgTmp;
            if (nFormulaWeights == 1) {
                weight_AND_sel_num_sig = Form("(%s)*(%s)", formulaWeights[0].c_str(), sel_AND_cut_sig.c_str());
                weight_AND_sel_num_bkg = Form("(%s)*(%s)", formulaWeights[0].c_str(), sel_AND_cut_bkg.c_str());
            }
            else if (nFormulaWeights == nFormulas) {
                weight_AND_sel_num_sig = Form("(%s)*(%s)", formulaWeights[i].c_str(), sel_AND_cut_sig.c_str());
                weight_AND_sel_num_bkg = Form("(%s)*(%s)", formulaWeights[i].c_str(), sel_AND_cut_bkg.c_str());
            }

            weight_AND_Sel_num_sig[i].push_back(weight_AND_sel_num_sig);
            weight_AND_Sel_num_bkg[i].push_back(weight_AND_sel_num_bkg);
        }

        if (selSigTmp.size() == 0)  selSigTmp == "1";
        if (selBkgTmp.size() == 0)  selBkgTmp == "1";

        std::string tmp_weight_AND_sel_denom_sig = selSigTmp;
        std::string tmp_weight_AND_sel_denom_bkg = selBkgTmp;
        if (nFormulaWeights == 1) {
            tmp_weight_AND_sel_denom_sig = Form("(%s)*(%s)", formulaWeights[0].c_str(), selSigTmp.c_str());
            tmp_weight_AND_sel_denom_bkg = Form("(%s)*(%s)", formulaWeights[0].c_str(), selBkgTmp.c_str());
        }
        else if (nFormulaWeights == nFormulas) {
            tmp_weight_AND_sel_denom_sig = Form("(%s)*(%s)", formulaWeights[i].c_str(), selSigTmp.c_str());
            tmp_weight_AND_sel_denom_bkg = Form("(%s)*(%s)", formulaWeights[i].c_str(), selBkgTmp.c_str());
        }
        weight_AND_Sel_denom_sig[i] = tmp_weight_AND_sel_denom_sig;
        weight_AND_Sel_denom_bkg[i] = tmp_weight_AND_sel_denom_bkg;
    }

    for (int i = 0; i < nFormulas; ++i) {

        std::cout << "Weights and selections for Formula " << i << std::endl;
        std::cout << "Signal events : " << std::endl;
        std::cout << "  Denom : " << weight_AND_Sel_denom_sig[i] << std::endl;

        for (int j = 0; j < nSigEffs; ++j) {
            std::cout << "  Efficiency " << j << " : " << weight_AND_Sel_num_sig[i][j] << std::endl;
        }

        std::cout << "Background events : " << std::endl;
        std::cout << "  Denom : " << weight_AND_Sel_denom_bkg[i] << std::endl;

        for (int j = 0; j < nSigEffs; ++j) {
            std::cout << "  Efficiency " << j << " : " << weight_AND_Sel_num_bkg[i][j] << std::endl;
        }
    }

    TFile* output = TFile::Open(outputFile.c_str(),"RECREATE");
    output->cd();

    TH1::SetDefaultSumw2();

    // TH1 for signal efficiency
    std::vector<std::vector<TH1D*>> h_expr_num_seff(nFormulas);
    std::vector<TH1D*> h_expr_denom_seff(nFormulas);

    // TH1 for bkg rejection
    std::vector<std::vector<TH1D*>> h_expr_num_brej(nFormulas);
    std::vector<TH1D*> h_expr_denom_brej(nFormulas);

    if (runMode[kSigEff] == MODES_EFFICIENCY::kDrawEff) {
        for (int i = 0; i < nFormulas; ++i) {

            int nBins = formula_TH1D_Bins[i].nBins;
            std::vector<double> bins = formula_TH1D_Bins[i].bins;

            double arr[nBins+1];
            std::copy(bins.begin(), bins.end(), arr);

            h_expr_denom_seff[i] = new TH1D(Form("h_sig_denom_expr_%d", i), Form(";%s;Entries", formulas[i].c_str()), nBins, arr);
            h_expr_denom_seff[i]->SetTitle("Denominator");

            for (int j = 0; j < nSigEffs; ++j) {

                h_expr_num_seff[i].push_back(
                        new TH1D(Form("h_sig_num_expr_%d_effwp_%d", i, j), Form(";%s;Entries", formulas[i].c_str()), nBins, arr));
                h_expr_num_seff[i][j]->SetTitle(Form("Selected via %.1f %% efficient WP", (double)(sigEffs[j]*100)));
            }

            h_expr_denom_brej[i] = new TH1D(Form("h_bkg_denom_expr_%d", i), Form(";%s;Entries", formulas[i].c_str()), nBins, arr);
            h_expr_denom_brej[i]->SetTitle("Denominator");

            for (int j = 0; j < nSigEffs; ++j) {

                h_expr_num_brej[i].push_back(
                        new TH1D(Form("h_bkg_num_expr_%d_effwp_%d", i, j), Form(";%s;Entries", formulas[i].c_str()), nBins, arr));
                h_expr_num_brej[i][j]->SetTitle(Form("Rejected via %.1f %% efficient WP", (double)(sigEffs[j]*100)));
            }
        }
    }

    // plot variable minimum/maximum cut as function of signal efficiency
    std::vector<TH1D*> h_ref(nReferences, 0);
    std::vector<TH1D*> h_diff_ref_true(nReferences, 0);
    std::vector<TH1D*> h_ratio_ref_true(nReferences, 0);

    std::vector<TH1D*> h_target_true(nTargets, 0);
    std::vector<TH1D*> h_target_regr(nTargets, 0);
    std::vector<TH1D*> h_diff_target_regr_true(nTargets, 0);
    std::vector<TH1D*> h_ratio_target_regr_true(nTargets, 0);
    if (runMode[MODES::kRegression]) {
        int nBinsTmp = 100;
        for (int i = 0; i < nReferences; ++i) {
            h_ref[i] = new TH1D(Form("h_ref_%d", i),
                                        Form(";%s;", references[i].c_str()), nBinsTmp, 0, 200);

            h_diff_ref_true[i] = new TH1D(Form("h_diff_ref_true_%d", i),
                                        Form(";%s ref - true ;", references[i].c_str()), nBinsTmp, -20, 20);

            h_ratio_ref_true[i] = new TH1D(Form("h_ratio_ref_true_%d", i),
                                        Form(";%s ref / true;", references[i].c_str()), nBinsTmp, 0, 2);
        }

        for (int i = 0; i < nTargets; ++i) {
            h_target_true[i] = new TH1D(Form("h_target_true_%d", i),
                                        Form(";%s truth;", targets[i].c_str()), nBinsTmp, 0, 200);

            h_target_regr[i] = new TH1D(Form("h_target_regr_%d", i),
                                        Form(";%s regression;", targets[i].c_str()), nBinsTmp, 0, 200);

            h_diff_target_regr_true[i] = new TH1D(Form("h_diff_target_regr_true_%d", i),
                                        Form(";%s regression - true;", targets[i].c_str()), nBinsTmp, -20, 20);

            h_ratio_target_regr_true[i] = new TH1D(Form("h_ratio_target_regr_true_%d", i),
                                        Form(";%s regression / true;", targets[i].c_str()), nBinsTmp, 0, 2);
        }
    }

    int nFiles = inputFiles.size();
    TFile* fileTmp = 0;

    TTree* tree = 0;

    Long64_t entries = 0;
    Long64_t entriesAnalyzed = 0;

    double weightsAll = 0;
    double weightsPreSel = 0;
    std::vector<double> weightsPassed(nSigEffs, 0);

    int nFilesSkipped = 0;
    std::cout<< "Running over TTree : " << treePath.c_str() <<std::endl;
    for (int iFile = 0; iFile < nFiles; ++iFile)  {

        std::string inputPath = inputFiles.at(iFile).c_str();
        std::cout <<"iFile = " << iFile << " , " ;
        std::cout <<"reading input file : " << inputPath.c_str() << std::endl;
        fileTmp = TFile::Open(inputPath.c_str(), "READ");

        // check if the file is usable, if not skip the file.
        if (isGoodFile(fileTmp) != 0) {
            std::cout << "File is not good. skipping file." << std::endl;
            nFilesSkipped++;
            continue;
        }

        bool loopEvents = (runMode[MODES::kSigEff] == MODES_EFFICIENCY::kPrintEff ||
                runMode[MODES::kRegression] != MODES_REGRESSION::kNULL_REGRESSION);

        tree = (TTree*)fileTmp->Get(treePath.c_str());
        tree->SetBranchStatus("*", 0);
        setBranchesStatus(tree, variablesT);
        setBranchesStatus(tree, spectatorsT);
        setBranchesStatus(tree, weights);
        setBranchesStatus(tree, references);
        setBranchesStatus(tree, targets);

        Long64_t entriesTmp = tree->GetEntries();
        entries += entriesTmp;
        std::cout << "entries in File = " << entriesTmp << std::endl;

        if (loopEvents) {

            float varsT[nVariablesT];
            for (int i = 0; i < nVariablesT; ++i) {
                tree->SetBranchAddress(variablesT[i].c_str(), &(varsT[i]));
            }

            float referencesT[nReferences];
            for (int i = 0; i < nReferences; ++i) {
                tree->SetBranchAddress(references[i].c_str(), &(referencesT[i]));
            }

            float targetsT[nTargets];
            for (int i = 0; i < nTargets; ++i) {
                tree->SetBranchAddress(targets[i].c_str(), &(targetsT[i]));
            }

            float specsT[nSpectatorsT];
            int classID = -1;
            for (int i = 0; i < nSpectatorsT; ++i) {
                if (spectatorsT[i] == "classID") {
                    tree->SetBranchAddress(spectatorsT[i].c_str(), &(classID));
                }
                else {
                    tree->SetBranchAddress(spectatorsT[i].c_str(), &(specsT[i]));
                }
            }

            float weightsT[nWeights];
            for (int i = 0; i < nWeights; ++i) {
                tree->SetBranchAddress(weights[i].c_str(), &(weightsT[i]));
            }

            for (Long64_t j_entry = 0; j_entry < entriesTmp; ++j_entry)
            {
                if (j_entry % 2000 == 0)  {
                    std::cout << "current entry = " <<j_entry<<" out of "<<entriesTmp<<" : "<<std::setprecision(2)<<(double)j_entry/entriesTmp*100<<" %"<<std::endl;
                }

                double w = 1;
                for (int i = 0; i < nWeights; ++i) {
                    w *= weightsT[i];
                }
                weightsAll += w;

                tree->GetEntry(j_entry);

                bool isSig = (classID == 0);
                if (!isSig) continue;

                for (int i = 0; i < nVariablesR; ++i) {
                    varsR[i] = varsT[i];
                }

                weightsPreSel += w;

                if (runMode[MODES::kSigEff] == MODES_EFFICIENCY::kPrintEff) {

                    for (int i=0; i<nSigEffs; ++i) {
                        bool passedTmp = reader->EvaluateMVA(methodName, sigEffs[i]);
                        if (passedTmp) {
                            weightsPassed[i] += w;
                        }
                    }
                }

                if (runMode[MODES::kRegression]) {
                    for (int i = 0; i < nReferences; ++i) {
                        float target_true = targetsT[i];
                        float refTmp = referencesT[i];

                        h_ref[i]->Fill(refTmp, w);
                        h_diff_ref_true[i]->Fill(refTmp - target_true, w);
                        h_ratio_ref_true[i]->Fill(refTmp / target_true, w);
                    }

                    std::vector<float> targets_regr = reader->EvaluateRegression(methodName.c_str());
                    for (int i = 0; i < nTargets; ++i) {
                        float target_true = targetsT[i];
                        float target_regr = targets_regr[i];

                        h_target_true[i]->Fill(target_true, w);
                        h_target_regr[i]->Fill(target_regr, w);
                        h_diff_target_regr_true[i]->Fill(target_regr - target_true, w);
                        h_ratio_target_regr_true[i]->Fill(target_regr / target_true, w);
                    }
                }

                entriesAnalyzed++;
            }
        }

        output->cd();
        for (int i = 0; i < nFormulas; ++i) {

            tree->Draw(Form("%s >>+ %s", formulas[i].c_str(), h_expr_denom_seff[i]->GetName()), weight_AND_Sel_denom_sig[i].c_str(), "goff");
            tree->Draw(Form("%s >>+ %s", formulas[i].c_str(), h_expr_denom_brej[i]->GetName()), weight_AND_Sel_denom_bkg[i].c_str(), "goff");

            for (int j = 0; j < nSigEffs; ++j) {

                tree->Draw(Form("%s >>+ %s", formulas[i].c_str(), h_expr_num_seff[i][j]->GetName()), weight_AND_Sel_num_sig[i][j].c_str(), "goff");
                tree->Draw(Form("%s >>+ %s", formulas[i].c_str(), h_expr_num_brej[i][j]->GetName()), weight_AND_Sel_num_bkg[i][j].c_str(), "goff");
            }
        }

        fileTmp->Close();
    }
    std::cout<<  "Loop ENDED : " << treePath.c_str() <<std::endl;
    std::cout << "nFilesSkipped = " << nFilesSkipped << std::endl;
    std::cout << "entries            = " << entries << std::endl;
    std::cout << "entriesAnalyzed    = " << entriesAnalyzed << std::endl;
    std::cout << "weightsAll    = " << weightsAll << std::endl;
    std::cout << "weightsPreSel = " << weightsPreSel << std::endl;
    std::cout << "weights passed for signal efficiencies :" << std::endl;
    for (int i=0; i<nSigEffs; ++i) {
        std::cout << Form("sigEffs[%d] = %f", i, sigEffs[i]) << std::endl;
        std::cout << Form("weightsPassed[%d] = %f", i, weightsPassed[i]) << std::endl;
        std::cout << Form("weightsPassed[%d]/weightsPreSel = %f", i, (double)(weightsPassed[i] / weightsPreSel)) << std::endl;
    }

    output->cd();

    TH1D* hTmp = 0;
    for (int i = 0; i < nFormulas; ++i) {

        if (h_expr_denom_seff[i] == 0)  continue;

        h_expr_denom_seff[i]->Write("",TObject::kOverwrite);

        for (int j = 0; j < nSigEffs; ++j) {

            if (h_expr_num_seff[i][j] == 0)  continue;

            h_expr_num_seff[i][j]->Write("",TObject::kOverwrite);

            hTmp = 0;
            hTmp = (TH1D*)h_expr_num_seff[i][j]->Clone(Form("h_sigeff_expr_%d_effwp_%d", i, j));
            hTmp->Divide(h_expr_denom_seff[i]);

            hTmp->SetTitle("Signal Efficiency");

            hTmp->Write("",TObject::kOverwrite);
        }
    }
    for (int i = 0; i < nFormulas; ++i) {

        if (h_expr_denom_brej[i] == 0)  continue;

        h_expr_denom_brej[i]->Write("",TObject::kOverwrite);

        for (int j = 0; j < nSigEffs; ++j) {

            if (h_expr_num_brej[i][j] == 0)  continue;

            h_expr_num_brej[i][j]->Write("",TObject::kOverwrite);

            hTmp = 0;
            hTmp = (TH1D*)h_expr_denom_brej[i]->Clone(Form("h_bkgrej_expr_%d_effwp_%d", i, j));
            hTmp->Add(h_expr_num_brej[i][j], -1);
            hTmp->Divide(h_expr_denom_brej[i]);

            hTmp->SetTitle("Background Rejection");

            hTmp->Write("",TObject::kOverwrite);
        }
    }

    for (int i = 0; i < nReferences; ++i) {

        if (h_ref[i] != 0) {
            h_ref[i]->Write("",TObject::kOverwrite);
        }
        if (h_diff_ref_true[i] != 0) {
            h_diff_ref_true[i]->Write("",TObject::kOverwrite);
        }
        if (h_ratio_ref_true[i] != 0) {
            h_ratio_ref_true[i]->Write("",TObject::kOverwrite);
        }
    }

    for (int i = 0; i < nTargets; ++i) {

        if (h_target_true[i] != 0) {
            h_target_true[i]->Write("",TObject::kOverwrite);
        }
        if (h_target_regr[i] != 0) {
            h_target_regr[i]->Write("",TObject::kOverwrite);
        }
        if (h_diff_target_regr_true[i] != 0) {
            h_diff_target_regr_true[i]->Write("",TObject::kOverwrite);
        }
        if (h_ratio_target_regr_true[i] != 0) {
            h_ratio_target_regr_true[i]->Write("",TObject::kOverwrite);
        }
    }

    std::cout<<"Writing the output file."<<std::endl;
    output->Write("",TObject::kOverwrite);
    std::cout<<"Closing the output file."<<std::endl;
    output->Close();
    std::cout << "running tmvaEval() - END" << std::endl;
}

int main(int argc, char** argv)
{
    std::vector<std::string> argStr = ArgumentParser::ParseParameters(argc, argv);
    int nArgStr = argStr.size();

    argOptions = ArgumentParser::ParseOptions(argc, argv);

    if (nArgStr == 6) {
        tmvaEval(argv[1], argv[2], argv[3], argv[4], argv[5]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./tmvaEval.exe <configFile> <inputFile> <fileXML> <outputFile> <methodName>"
                << std::endl;
        return 1;
    }
}

int readConfiguration(std::string configFile, std::string inputFile)
{
    ConfigurationParser confParser;
    confParser.openConfigFile(configFile);

    confParser.parsedKeyWords = InputConfigurationParser::parseKeyWords(inputFile);

    mode = confParser.ReadConfigValue("mode");
    runMode = parseMode(mode);

    // TTree
    treePath= confParser.ReadConfigValue("treePath");

    variablesT = ConfigurationParser::ParseListOrString(confParser.ReadConfigValue("variablesTree"));
    variablesR = ConfigurationParser::ParseListOrString(confParser.ReadConfigValue("variablesReader"));
    spectatorsT = ConfigurationParser::ParseListOrString(confParser.ReadConfigValue("treeSpectatorBranches"));
    spectatorsR = ConfigurationParser::ParseListOrString(confParser.ReadConfigValue("readerSpectators"));
    weights = ConfigurationParser::ParseListOrString(confParser.ReadConfigValue("treeWeightBranches"));

    formulas = ConfigurationParser::ParseListOrString(confParser.ReadConfigValue("formulas"));
    formulaSels = ConfigurationParser::ParseListOrString(confParser.ReadConfigValue("formulaSelections"));
    formulaWeights = ConfigurationParser::ParseListOrString(confParser.ReadConfigValue("formulaWeights"));
    formula_TH1D_Bins = ConfigurationParser::ParseListTH1D_Axis(confParser.ReadConfigValue("formula_TH1D_Bins"));

    sigSels = ConfigurationParser::ParseListOrString(confParser.ReadConfigValue("sigSelections"));
    bkgSels = ConfigurationParser::ParseListOrString(confParser.ReadConfigValue("bkgSelections"));

    references = ConfigurationParser::ParseListOrString(confParser.ReadConfigValue("referenceVariables"));
    targets = ConfigurationParser::ParseListOrString(confParser.ReadConfigValue("targetVariables"));

    preselection = confParser.ReadConfigValue("preselection");

    sigEffs = ConfigurationParser::ParseListFloat(confParser.ReadConfigValue("signalEfficiencies"));

    if (treePath == "") {
        treePath = "ggHiNtuplizerGED/EventTree";
    }

    if (variablesT.size() == 0) {
        variablesT.push_back("*");
    }

    nVariablesT = variablesT.size();
    nVariablesR = variablesR.size();
    if (runMode[MODES::kSigEff] == MODES_EFFICIENCY::kPrintEff) {
        if (nVariablesT != nVariablesR) {
            std::cout << "ERROR : mismatch between number of variables for reader and  input tree" << std::endl;
            std::cout << "# of variables for input tree = " << variablesT.size() << std::endl;
            std::cout << "# of variables for reader = " << variablesR.size() << std::endl;
            std::cout << "exiting" << std::endl;
            return 1;
        }
    }

    nSpectatorsT = spectatorsT.size();
    nSpectatorsR = spectatorsR.size();
    nWeights = weights.size();

    nFormulas = formulas.size();
    nFormulaSels = formulaSels.size();
    nFormulaWeights = formulaWeights.size();
    nFormula_TH1D_Bins = formula_TH1D_Bins.size();

    nSigSels = sigSels.size();
    nBkgSels = bkgSels.size();

    nReferences = references.size();
    nTargets = targets.size();

    nSigEffs = sigEffs.size();

    return 0;
}

/*
 * print information read from configuration
 * assumes that readConfiguration() is run before
 */
void printConfiguration()
{
    std::cout<<"Configuration :"<<std::endl;

    std::cout << "mode = " << mode.c_str() << std::endl;
    for (int i = 0; i < (int)runMode.size(); ++i) {
        std::cout << "run " << modesStr[i].c_str() << " = " << runMode.at(i) << std::endl;
    }

    std::cout << "treePath = " << treePath.c_str() << std::endl;

    // branches activated in Tree
    std::cout << "nVariablesT = " << nVariablesT << std::endl;
    for (int i = 0; i < nVariablesT; ++i) {
        std::cout << Form("variablesT[%d] = %s", i, variablesT.at(i).c_str()) << std::endl;
    }

    // variables activated in Reader
    std::cout << "nVariablesR = " << nVariablesR << std::endl;
    for (int i = 0; i < nVariablesR; ++i) {
        std::cout << Form("variablesR[%d] = %s", i, variablesR.at(i).c_str()) << std::endl;
    }

    std::cout << "nSpectatorsT = " << nSpectatorsT << std::endl;
    for (int i = 0; i < nSpectatorsT; ++i) {
        std::cout << Form("spectatorsT[%d] = %s", i, spectatorsT.at(i).c_str()) << std::endl;
    }

    std::cout << "nSpectatorsR = " << nSpectatorsR << std::endl;
    for (int i = 0; i < nSpectatorsR; ++i) {
        std::cout << Form("spectatorsR[%d] = %s", i, spectatorsR.at(i).c_str()) << std::endl;
    }

    std::cout << "nWeights = " << nWeights << std::endl;
    for (int i = 0; i < nWeights; ++i) {
        std::cout << Form("weights[%d] = %s", i, weights.at(i).c_str()) << std::endl;
    }

    std::cout << "nFormulas = " << nFormulas << std::endl;
    for (int i = 0; i < nFormulas; ++i) {
        std::cout << Form("formulas[%d] = %s", i, formulas.at(i).c_str()) << std::endl;
    }

    std::cout << "nFormulaSels = " << nFormulaSels << std::endl;
    for (int i = 0; i < nFormulaSels; ++i) {
        std::cout << Form("formulaSels[%d] = %s", i, formulaSels.at(i).c_str()) << std::endl;
    }

    std::cout << "nFormulaWeights = " << nFormulaWeights << std::endl;
    for (int i = 0; i < nFormulaWeights; ++i) {
        std::cout << Form("formulaWeights[%d] = %s", i, formulaWeights.at(i).c_str()) << std::endl;
    }

    std::cout << "nFormula_TH1D_Bins = " << nFormula_TH1D_Bins << std::endl;
    for (int i=0; i<nFormula_TH1D_Bins; ++i) {
        std::string strTH1D_Axis = ConfigurationParser::verboseTH1D_Axis(formula_TH1D_Bins.at(i));
        std::cout << Form("formula_TH1D_Bins[%d] = %s", i, strTH1D_Axis.c_str()) << std::endl;
    }

    std::cout << "nSigSels = " << nSigSels << std::endl;
    for (int i = 0; i < nSigSels; ++i) {
        std::cout << Form("sigSels[%d] = %s", i, sigSels.at(i).c_str()) << std::endl;
    }

    std::cout << "nBkgSels = " << nBkgSels << std::endl;
    for (int i = 0; i < nBkgSels; ++i) {
        std::cout << Form("bkgSels[%d] = %s", i, bkgSels.at(i).c_str()) << std::endl;
    }

    std::cout << "nReferences = " << nReferences << std::endl;
    for (int i = 0; i < nReferences; ++i) {
        std::cout << Form("references[%d] = %s", i, references.at(i).c_str()) << std::endl;
    }

    std::cout << "nTargets = " << nTargets << std::endl;
    for (int i = 0; i < nTargets; ++i) {
        std::cout << Form("targets[%d] = %s", i, targets.at(i).c_str()) << std::endl;
    }

    std::cout << "preselection = " << preselection.c_str() << std::endl;

    std::cout << "nSigEffs = " << nSigEffs << std::endl;
    for (int i=0; i<nSigEffs; ++i) {
        std::cout << Form("sigEffs[%d] = %f", i, sigEffs[i]) << std::endl;
    }
}

std::vector<int> parseMode(std::string mode)
{
    std::vector<int> res(MODES::kN_MODES, 0);

    int len = mode.size();
    if (len != MODES::kN_MODES) return res;

    for (int i = 0; i < len; ++i) {

        std::istringstream sin(mode.substr(i, 1));
        int in;
        sin >> in;
        res.at(i) = in;
    }

    return res;
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
    for (std::vector<std::string>::iterator it = branchList.begin(); it != branchList.end(); ++it) {
        t->SetBranchStatus((*it).c_str(), 1);
    }
}
