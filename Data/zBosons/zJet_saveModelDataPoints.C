/*
 * macro to save data points from models for Z+jet at 5.02 TeV
 */
#include <TFile.h>
#include <TDirectoryFile.h>
#include <TH1.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include <TString.h>

#include <string>
#include <iostream>
#include <vector>

#include "zJetData.h"
#include "../../Utilities/tgraphUtil.h"
#include "../../Utilities/th1Util.h"
#include "../../Utilities/systemUtil.h"

void zJet_saveModelDataPoints(const TString outputFile = "Data/zBosons/model_zJet_5TeV.root");

void zJet_saveModelDataPoints(const TString outputFile)
{
   std::string dirName = "";
   std::string dirTitle = "";
   TFile* output = new TFile(outputFile.Data(),"RECREATE");

   std::cout << "output file : " << outputFile.Data() << std::endl;

   // TH1D* h1D = 0;
   TGraph* gr = new TGraph();
   TGraphErrors* grErr = new TGraphErrors();

   // HYBRID model predictions
   std::cout << "HYBRID model predictions" << std::endl;
   dirName = "HYBRID";
   dirTitle = Form("Z+jet at 5.02 TeV predictions from %s", dirName.c_str());
   output->mkdir(dirName.c_str(), dirTitle.c_str());
   output->cd(dirName.c_str());

   std::vector<std::string> observables_HYBRID {"xjz", "dphi", "rjz", "IAA", "IAA_ptBin5", "IAA_ptBin6"};
   int nObservables_HYBRID = observables_HYBRID.size();
   for (int i = 0; i < nObservables_HYBRID; ++i) {

       std::string observable = observables_HYBRID.at(i);
       std::string tmpObjectName = Form("%s_ptBin0_hiBin1", observable.c_str());
       if (observable == "IAA_ptBin5" || observable == "IAA_ptBin6")
           tmpObjectName = replaceAll(tmpObjectName, "_ptBin0", "");

       std::string objectName = "";

       std::vector<double> x[HYBRID::kN_MODEL];
       std::vector<double> ymin[HYBRID::kN_MODEL];
       std::vector<double> ymax[HYBRID::kN_MODEL];
       std::vector<double> xPP;
       std::vector<double> yPP;
       std::vector<double> yerrPP;

       for (int iModel = 0; iModel < HYBRID::kN_MODEL; ++iModel) {
           if (observable == "xjz") {
               x[iModel] = HYBRID::x_xjz;
               ymin[iModel] = HYBRID::ymin_xjz[iModel];
               ymax[iModel] = HYBRID::ymax_xjz[iModel];
               xPP =  HYBRID::x_xjz;
               yPP = HYBRID::y_xjz_PP;
               yerrPP = HYBRID::yerr_xjz_PP;
           }
           else if (observable == "dphi") {
               x[iModel] = HYBRID::x_dphi;
               ymin[iModel] = HYBRID::ymin_dphi[iModel];
               ymax[iModel] = HYBRID::ymax_dphi[iModel];
               xPP =  HYBRID::x_dphi;
               yPP = HYBRID::y_dphi_PP;
               yerrPP = HYBRID::yerr_dphi_PP;
           }
           else if (observable == "rjz") {
               x[iModel] = HYBRID::x_rjz;
               ymin[iModel] = HYBRID::ymin_rjz[iModel];
               ymax[iModel] = HYBRID::ymax_rjz[iModel];
               xPP =  HYBRID::x_rjz;
               yPP = HYBRID::y_rjz_PP;
               yerrPP = HYBRID::yerr_rjz_PP;
           }
           else if (observable == "IAA") {
               x[iModel] = HYBRID::x_IAA[iModel];
               ymin[iModel] = HYBRID::ymin_IAA[iModel];
               ymax[iModel] = HYBRID::ymax_IAA[iModel];
           }
           else if (observable == "IAA_ptBin5") {
               x[iModel] = HYBRID::x_IAA_ptBin5[iModel];
               ymin[iModel] = HYBRID::ymin_IAA_ptBin5[iModel];
               ymax[iModel] = HYBRID::ymax_IAA_ptBin5[iModel];
           }
           else if (observable == "IAA_ptBin6") {
               x[iModel] = HYBRID::x_IAA_ptBin6[iModel];
               ymin[iModel] = HYBRID::ymin_IAA_ptBin6[iModel];
               ymax[iModel] = HYBRID::ymax_IAA_ptBin6[iModel];
           }

           if (x[iModel].size() > 0) {
               objectName = Form("%s_%s", tmpObjectName.c_str(), HYBRID::modelTAG[iModel].c_str());
               gr = new TGraph();
               setTGraphBand(gr, x[iModel], ymin[iModel], ymax[iModel]);
               gr->SetName(Form("gr_%s", objectName.c_str()));
               gr->SetTitle(Form("%s - PbPb", HYBRID::legendEntries[iModel].c_str()));
               gr->Write("",TObject::kOverwrite);
           }
       }

       if (xPP.size() > 0) {
           objectName = Form("%s_PP", tmpObjectName.c_str());
           grErr = new TGraphErrors();
           setTGraphErrors(grErr, xPP, yPP, yerrPP);
           grErr->SetName(Form("gr_%s", objectName.c_str()));
           grErr->SetTitle(HYBRID::legendEntryPP.c_str());
           grErr->Write("",TObject::kOverwrite);
       }
   }

   // JEWEL predictions
   std::cout << "JEWEL predictions" << std::endl;
   dirName = "JEWEL";
   dirTitle = Form("Z+jet at 5.02 TeV predictions from %s", dirName.c_str());
   output->mkdir(dirName.c_str(), dirTitle.c_str());
   output->cd(dirName.c_str());

   std::vector<std::string> observables_JEWEL {"xjz", "dphi", "rjz", "xjz_mean"};
   int nObservables_JEWEL = observables_JEWEL.size();
   for (int i = 0; i < nObservables_JEWEL; ++i) {

       std::string observable = observables_JEWEL.at(i);
       std::string tmpObjectName = Form("%s_ptBin0_hiBin1", observable.c_str());

       std::string objectName = "";

       std::vector<double> x;
       std::vector<double> y;
       std::vector<double> yerr;
       std::vector<double> xPP;
       std::vector<double> yPP;
       std::vector<double> yerrPP;

       if (observable == "xjz") {
           x = JEWEL::x_xjz;
           y = JEWEL::y_xjz;
           yerr = JEWEL::yerr_xjz;
           xPP =  JEWEL::x_xjz_PP;
           yPP = JEWEL::y_xjz_PP;
           yerrPP = JEWEL::yerr_xjz_PP;
       }
       else if (observable == "dphi") {
           x = JEWEL::x_dphi;
           y = JEWEL::y_dphi;
           yerr = JEWEL::yerr_dphi;
           xPP =  JEWEL::x_dphi;
           yPP = JEWEL::y_dphi_PP;
           yerrPP = JEWEL::yerr_dphi_PP;
       }
       else if (observable == "rjz") {
           x = JEWEL::x_rjz;
           y = JEWEL::y_rjz;
           yerr = JEWEL::yerr_rjz;
           xPP =  JEWEL::x_rjz;
           yPP = JEWEL::y_rjz_PP;
           yerrPP = JEWEL::yerr_rjz_PP;
       }
       else if (observable == "xjz_mean") {
           x = JEWEL::x_xjz_mean;
           y = JEWEL::y_xjz_mean;
           yerr = JEWEL::yerr_xjz_mean;
           xPP =  JEWEL::x_xjz_mean;
           yPP = JEWEL::y_xjz_mean_PP;
           yerrPP = JEWEL::yerr_xjz_mean_PP;
       }

       if (x.size() > 0) {
           objectName = tmpObjectName.c_str();
           grErr = new TGraphErrors();
           setTGraphErrors(grErr, x, y, yerr);
           grErr->SetName(Form("gr_%s", objectName.c_str()));
           grErr->SetTitle("JEWEL - PbPb");
           grErr->Write("",TObject::kOverwrite);
       }

       if (xPP.size() > 0) {
           objectName = Form("%s_PP", tmpObjectName.c_str());
           grErr = new TGraphErrors();
           setTGraphErrors(grErr, xPP, yPP, yerrPP);
           grErr->SetName(Form("gr_%s", objectName.c_str()));
           grErr->SetTitle(JEWEL::legendEntryPP.c_str());
           grErr->Write("",TObject::kOverwrite);
       }
   }

   // Ivan Vitev predictions
   std::cout << "Ivan Vitev predictions" << std::endl;
   dirName = "VITEV";
   dirTitle = Form("Z+jet at 5.02 TeV predictions from %s", dirName.c_str());
   output->mkdir(dirName.c_str(), dirTitle.c_str());
   output->cd(dirName.c_str());

   std::vector<std::string> observables_VITEV {"xjz"};
   int nObservables_VITEV = observables_VITEV.size();
   for (int i = 0; i < nObservables_VITEV; ++i) {

       std::string observable = observables_VITEV.at(i);
       std::string tmpObjectName = Form("%s_ptBin0_hiBin1", observable.c_str());

       std::string objectName = "";

       std::vector<double> x[VITEV::kN_MODEL];
       std::vector<double> y[VITEV::kN_MODEL];
       std::vector<double> xPP;
       std::vector<double> yPP;

       for (int iModel = 0; iModel < VITEV::kN_MODEL; ++iModel) {
           if (observable == "xjz") {
               x[iModel] = VITEV::x_xjz;
               y[iModel] = VITEV::y_xjz[iModel];
               xPP =  VITEV::x_xjz;
               yPP = VITEV::y_xjz_PP;
           }

           if (x[iModel].size() > 0) {

               objectName = Form("%s_%s", tmpObjectName.c_str(), VITEV::modelTAG[iModel].c_str());

               int nPoints = x[iModel].size();
               double xArr[nPoints];
               std::copy(x[iModel].begin(), x[iModel].end(), xArr);
               double yArr[nPoints];
               std::copy(y[iModel].begin(), y[iModel].end(), yArr);
               gr = new TGraph(nPoints, xArr, yArr);
               gr->SetName(Form("gr_%s", objectName.c_str()));
               gr->SetTitle(Form("%s - PbPb", VITEV::legendEntries[iModel].c_str()));
               gr->Write("",TObject::kOverwrite);
           }
       }

       if (xPP.size() > 0) {
           objectName = Form("%s_PP", tmpObjectName.c_str());

           int nPoints = xPP.size();
           double xArr[nPoints];
           std::copy(xPP.begin(), xPP.end(), xArr);
           double yArr[nPoints];
           std::copy(yPP.begin(), yPP.end(), yArr);
           gr = new TGraph(nPoints, xArr, yArr);
           gr->SetName(Form("gr_%s", objectName.c_str()));
           gr->SetTitle(VITEV::legendEntryPP.c_str());
           gr->Write("",TObject::kOverwrite);
       }
   }

   output->Close();
   std::cout<< "output file has been closed" <<std::endl;
}

int main(int argc, char** argv)
{
    if (argc == 2) {
        zJet_saveModelDataPoints(argv[1]);
        return 0;
    }
    else if (argc == 1) {
        zJet_saveModelDataPoints();
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./zJet_saveModelDataPoints.exe <outputFile>"
                << std::endl;
        return 1;
    }
    return 0;
}
