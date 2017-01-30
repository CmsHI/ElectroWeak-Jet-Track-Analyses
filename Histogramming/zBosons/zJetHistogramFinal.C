/*
 * macro to produce last-step zJet results, such as IAA. to be used for results that requires both pp and PbPb collisions.
 */
#include <TFile.h>
#include <TDirectoryFile.h>
#include <TList.h>
#include <TKey.h>
#include <TCanvas.h>
#include <TCut.h>
#include <TH1D.h>
#include <TMath.h>

#include <vector>
#include <string>
#include <iostream>

#include "../../TreeHeaders/CutConfigurationTree.h"
#include "../../Plotting/commonUtility.h"
#include "../interface/correlationHist.h"
#include "../../Utilities/interface/CutConfigurationParser.h"
#include "../../Utilities/interface/InputConfigurationParser.h"
#include "../../Utilities/eventUtil.h"
#include "../../Utilities/fileUtil.h"
#include "../../Utilities/systemUtil.h"
#include "../../Utilities/th1Util.h"
#include "../../Utilities/bosonJetUtil.h"

void zJetHistogramFinal(const TString configFile, const TString inputFile, const TString outputFile = "zJetHistogramFinal.root");

void zJetHistogramFinal(const TString configFile, const TString inputFile, const TString outputFile)
{
    std::cout<<"running zJetHistogramFinal()"<<std::endl;
    std::cout<<"configFile = "<< configFile.Data() <<std::endl;
    std::cout<<"inputFile = "<< inputFile.Data() <<std::endl;
    std::cout<<"outputFile = "<< outputFile.Data() <<std::endl;

    InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

    if (!configInput.isValid) {
        std::cout << "Input configuration is invalid." << std::endl;
        std::cout << "exiting" << std::endl;
        return;
    }
    if (!configCuts.isValid) {
        std::cout << "Cut configuration is invalid." << std::endl;
        std::cout << "exiting" << std::endl;
        return;
    }

    // input configuration

    // verbose about input configuration
    std::cout<<"Input Configuration :"<<std::endl;

    // cut configuration
    // observable bins
    // for zJetHistogramFinal.C, the whole purpose of "pt" and "hiBin" bins is to get the number of bins used.
    std::vector<float> bins_pt[2];          // array of vectors for eta bins, each array element is a vector.
    std::vector<int>   bins_hiBin[2];       // array of vectors for hiBin bins, each array element is a vector.
    bins_pt[0] = ConfigurationParser::ParseListFloat(
            configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kZBOSON].s[CUTS::ZBO::k_bins_pt_gt]);
    bins_pt[1] = ConfigurationParser::ParseListFloat(
            configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kZBOSON].s[CUTS::ZBO::k_bins_pt_lt]);
    bins_hiBin[0] = ConfigurationParser::ParseListInteger(
            configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_gt]);
    bins_hiBin[1] = ConfigurationParser::ParseListInteger(
            configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_lt]);

    // default values

    int nBins_pt = bins_pt[0].size();         // assume <myvector>[0] and <myvector>[1] have the same size.
    int nBins_hiBin = bins_hiBin[0].size();     // assume <myvector>[0] and <myvector>[1] have the same size.
    // verbose about cut configuration
    std::cout<<"Cut Configuration :"<<std::endl;
    std::cout << "nBins_pt = " << nBins_pt << std::endl;
    for (int i=0; i<nBins_pt; ++i) {
        std::cout << Form("bins_pt[%d] = [%.1f, %.1f)", i, bins_pt[0].at(i), bins_pt[1].at(i)) << std::endl;
    }
    std::cout << "nBins_hiBin = " << nBins_hiBin << std::endl;
    for (int i=0; i<nBins_hiBin; ++i) {
        std::cout << Form("bins_hiBin[%d] = [%d, %d)", i, bins_hiBin[0].at(i), bins_hiBin[1].at(i)) << std::endl;
    }

    // inputFile should be a list of ROOT files, that is a ".txt" or ".list" file.
    // the assumed order in inputFile :
    // 1. HI DATA
    // 2. HI MC
    // 3. PP DATA
    // 4. PP MC
    std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.Data());

    int nInputFiles = inputFiles.size();
    std::vector<int> collisionTypes    {COLL::kHI,   COLL::kHIMC, COLL::kPP,   COLL::kPPMC};
    int nCollisions = collisionTypes.size();
    std::cout<<"input ROOT files : num = "<<nInputFiles<< std::endl;
    if (nInputFiles < 4)  {
        std::cout<<"need to provide 4 input files, provide non-existing files as DUMMY.root" << std::endl;
        std::cout<<"exiting" << std::endl;
        return;
    }
    else {
        std::cout<<"#####"<< std::endl;
        std::cout<<"HI DATA : "<<inputFiles.at(0).c_str()<< std::endl;
        std::cout<<"HI MC   : "<<inputFiles.at(1).c_str()<< std::endl;
        std::cout<<"PP DATA : "<<inputFiles.at(2).c_str()<< std::endl;
        std::cout<<"PP MC   : "<<inputFiles.at(3).c_str()<< std::endl;
        std::cout<<"##### END #####"<< std::endl;
    }

    TFile* input[nCollisions];
    bool   inputExists[nCollisions];
    TDirectoryFile* inputDir[nCollisions];

    for (int i = 0; i<nCollisions; ++i) {

        input[i] = new TFile(inputFiles.at(i).c_str(), "READ");
        inputExists[i] = (input[i] && input[i]->IsOpen());
        if (!inputExists[i])  continue;

        std::string collisionName = getCollisionTypeName((COLL::TYPE)collisionTypes.at(i));
        inputDir[i] = (TDirectoryFile*)input[i]->GetDirectory(collisionName.c_str());
    }

    TFile* output = new TFile(outputFile, "UPDATE");
    std::string outputDir = "";
    if(inputExists[COLL::kHI])  {
        outputDir = getCollisionTypeName((COLL::TYPE)collisionTypes.at(COLL::kHI));
    }
    else if(inputExists[COLL::kHIMC])  {
        outputDir = getCollisionTypeName((COLL::TYPE)collisionTypes.at(COLL::kHIMC));
    }
    output->cd(outputDir.c_str());
    std::cout<<"histograms will be put under directory : " << outputDir.c_str() << std::endl;

    TTree *configTree = setupConfigurationTreeForWriting(configCuts);

    std::vector<std::string> correlationHistNames {"xjz", "dphi", "dphi_rebin", "dphi_normJZ", "dphi_rebin_normJZ",
                                                          "ptJet", "zM",
                                                          "rjz", "xjz_mean", "rjz_zNum", "xjz_mean",
                                                          "zPt", "zEta", "zPhi", "jteta", "jtphi",
                                                          "nJet", "hiBin"};
    std::vector<std::string> versionSuffix {"final_norm", "final_norm", "final_norm", "final_norm", "final_norm",
                                                     "final_norm", "final_norm",
                                                     "", "", "", "",
                                                     "final_norm", "final_norm", "final_norm", "final_norm", "final_norm",
                                                     "final_norm", "final_norm"};
    std::vector<std::string> jetRegion    {"SIG", "SIG", "SIG", "SIG", "SIG",
                                                  "SIG", "RAW",
                                                  "", "SIG", "", "RAW",
                                                  "RAW", "RAW", "RAW", "SIG", "SIG",
                                                  "RAW", "RAW"};
    std::vector<std::string> jetRegionBKG {"BKG", "BKG", "BKG", "BKG", "BKG",
                                                  "BKG", "BKG", "BKG", "BKG",
                                                  "BKG", "BKG",
                                                  "BKG", "BKG", "BKG", "BKG", "BKG",
                                                  "BKG", "BKG"};

    TH1::SetDefaultSumw2();

    int nCorrHist = correlationHistNames.size();

    TH1D* h1D[nInputFiles][nCorrHist];
    bool  h1DisValid[nInputFiles][nCorrHist];

    TCanvas* c = new TCanvas("cnv","",600,600);
    TH1D* h1D_calc = 0;
    TH1D* h1D_calc2 = 0;

    for (int i=0; i<nCorrHist; ++i){
        for (int iPt=0; iPt < nBins_pt; ++iPt){
            for (int iHiBin=0; iHiBin < nBins_hiBin; ++iHiBin){

                std::string correlation = correlationHistNames.at(i).c_str();
                std::cout<<"##### "<< correlation.c_str() <<" #####"<<std::endl;

                std::string tmpName = Form("%s_ptBin%d_hiBin%d_jet%s_%s", correlationHistNames.at(i).c_str(), iPt, iHiBin,
                        jetRegion.at(i).c_str(), versionSuffix.at(i).c_str());
                // special cases
                if (correlation == "rjz" || correlation == "rjz_zNum" ) {
                    if (iPt > 0)  continue;
                    tmpName = Form("%s_ptBinAll_hiBin%d", correlation.c_str(), iHiBin);
                }
                else if (correlation == "xjz_mean") {
                    if (iPt > 0)  continue;
                    tmpName = Form("%s_ptBinAll_hiBin%d_jet%s", correlation.c_str(), iHiBin, jetRegion.at(i).c_str());
                }

                std::string tmpHistName = Form("h1D_%s", tmpName.c_str());

                // no centrality bin for PP or PPMC
                std::string tmpNamePP = replaceAll(tmpName, Form("_hiBin%d", iHiBin), "_hiBin0");
                std::string tmpHistNamePP = Form("h1D_%s", tmpNamePP.c_str());

                // read histograms
                std::cout<<"reading histogram : "<< tmpHistName.c_str() <<std::endl;
                for (int iColl = 0; iColl < nCollisions; ++iColl){
                    std::string tmpNameColl = tmpName.c_str();
                    std::string tmpHistNameColl = tmpHistName.c_str();
                    // no centrality bin for PP or PPMC
                    if (iColl == COLL::kPP || iColl == COLL::kPPMC) {
                        tmpNameColl = tmpNamePP.c_str();
                        tmpHistNameColl = tmpHistNamePP.c_str();

                        if (correlation == "xjz_mean" && iColl == COLL::kPPMC) {
                            tmpNameColl = replaceAll(tmpNameColl, "_jetSIG", "_jetRAW");
                            tmpHistNameColl = replaceAll(tmpHistNameColl, "_jetSIG", "_jetRAW");
                        }
                    }

                    h1D[iColl][i] = 0;
                    if (inputExists[iColl])
                        inputDir[iColl]->GetObject(tmpHistNameColl.c_str(), h1D[iColl][i]);

                    h1DisValid[iColl][i] = false;
                    if (h1D[iColl][i])  h1DisValid[iColl][i] = true;
                }

                std::string tmpNameCalc = "";
                std::string tmpHistNameCalc = "";
                // create I_AA histogram
                if (correlation == "ptJet") {

                    tmpNameCalc = replaceAll(tmpName, "ptJet", "iaa");
                    tmpHistNameCalc = Form("h1D_%s", tmpNameCalc.c_str());

                    if (h1DisValid[COLL::kHI][i] && (h1DisValid[COLL::kPP][i]))
                    {
                        h1D_calc = (TH1D*)h1D[COLL::kHI][i]->Clone(tmpHistNameCalc.c_str());
                        h1D_calc2 = (TH1D*)h1D[COLL::kPP][i]->Clone(Form("%s_2", tmpHistNameCalc.c_str()));
                    }
                    else if (h1DisValid[COLL::kHIMC][i] && h1DisValid[COLL::kPPMC][i])
                    {
                        h1D_calc = (TH1D*)h1D[COLL::kHIMC][i]->Clone(tmpHistNameCalc.c_str());
                        h1D_calc2 = (TH1D*)h1D[COLL::kPPMC][i]->Clone(Form("%s_2", tmpHistNameCalc.c_str()));
                    }
                    else if (h1DisValid[COLL::kHIMC][i] && h1DisValid[COLL::kPP][i])
                    {
                        h1D_calc = (TH1D*)h1D[COLL::kHIMC][i]->Clone(tmpHistNameCalc.c_str());
                        h1D_calc2 = (TH1D*)h1D[COLL::kPP][i]->Clone(Form("%s_2", tmpHistNameCalc.c_str()));
                    }

                    // rebin for I_AA
                    double binsRebin[8] = {0, 30, 40, 60, 80, 100, 140, 200};
                    h1D_calc = (TH1D*)h1D_calc->Rebin(8-1, h1D_calc->GetName(), binsRebin);
                    h1D_calc2 = (TH1D*)h1D_calc2->Rebin(8-1, h1D_calc2->GetName(), binsRebin);

                    // h1D_calc->Scale(1, "width");
                    // h1D_calc2->Scale(1, "width");

                    h1D_calc->Divide(h1D_calc2);

                    h1D_calc->SetYTitle("Jet I_{AA}");

                    c->SetName(Form("cnv_%s",tmpNameCalc.c_str()));
                    c->cd();
                    h1D_calc->Draw("e");
                    h1D_calc->Write("",TObject::kOverwrite);
                    c->Write("",TObject::kOverwrite);
                    c->Clear();

                    std::cout<<"wrote histogram : "<< h1D_calc->GetName() <<std::endl;
                }
                // create <xjz> ratio and rjz ratio histogram
                if (correlation == "xjz_mean" || correlation == "rjz") {

                    if (correlation == "xjz_mean")  tmpNameCalc = replaceAll(tmpName, "xjz_mean", "xjz_mean_ratio");
                    else if (correlation == "rjz")  tmpNameCalc = replaceAll(tmpName, "rjz", "rjz_ratio");
                    tmpHistNameCalc = Form("h1D_%s", tmpNameCalc.c_str());

                    if (h1DisValid[COLL::kHI][i] && (h1DisValid[COLL::kPP][i]))
                    {
                        h1D_calc = (TH1D*)h1D[COLL::kHI][i]->Clone(tmpHistNameCalc.c_str());
                        h1D_calc2 = (TH1D*)h1D[COLL::kPP][i]->Clone(Form("%s_2", tmpHistNameCalc.c_str()));
                    }
                    else if (h1DisValid[COLL::kHIMC][i] && h1DisValid[COLL::kPPMC][i])
                    {
                        h1D_calc = (TH1D*)h1D[COLL::kHIMC][i]->Clone(tmpHistNameCalc.c_str());
                        h1D_calc2 = (TH1D*)h1D[COLL::kPPMC][i]->Clone(Form("%s_2", tmpHistNameCalc.c_str()));
                    }
                    else if (h1DisValid[COLL::kHIMC][i] && h1DisValid[COLL::kPP][i])
                    {
                        h1D_calc = (TH1D*)h1D[COLL::kHIMC][i]->Clone(tmpHistNameCalc.c_str());
                        h1D_calc2 = (TH1D*)h1D[COLL::kPP][i]->Clone(Form("%s_2", tmpHistNameCalc.c_str()));
                    }

                    h1D_calc->Divide(h1D_calc2);

                    if (correlation == "xjz_mean")  h1D_calc->SetYTitle("#frac{<x_{jZ}> (PbPb)}{<x_{jZ}> (pp)}");
                    else if (correlation == "rjz")  h1D_calc->SetYTitle("#frac{R_{jZ} (PbPb)}{R_{jZ} (pp)}");

                    c->SetName(Form("cnv_%s",tmpNameCalc.c_str()));
                    c->cd();
                    h1D_calc->Draw("e");
                    h1D_calc->Write("",TObject::kOverwrite);
                    c->Write("",TObject::kOverwrite);
                    c->Clear();

                    std::cout<<"wrote histogram : "<< h1D_calc->GetName() <<std::endl;
                }

                std::cout<<"##########"<<std::endl;
            }
        }
    }

    configTree->Write("",TObject::kOverwrite);

    output->Write("",TObject::kOverwrite);
    for (int i = 0; i<nInputFiles; ++i) {
        if (inputExists[i])  {
            inputDir[i]->Close();
            input[i]->Close();
        }
    }
    output->Close();

    std::cout<<"zJetHistogramFinal() - END"<<std::endl;
}

int main(int argc, char** argv)
{
    if (argc == 4) {
        zJetHistogramFinal(argv[1], argv[2], argv[3]);
        return 0;
    }
    else if (argc == 3) {
        zJetHistogramFinal(argv[1], argv[2]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./zJetHistogramFinal.exe <configFile> <inputFile> <outputFile>"
                << std::endl;
        return 1;
    }
}
