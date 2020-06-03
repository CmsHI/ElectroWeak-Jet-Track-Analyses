/*
 * macro to save theory calculations for Z+hadron studies
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

#include "thModels.h"
#include "../../../Utilities/tgraphUtil.h"
#include "../../../Utilities/th1Util.h"
#include "../../../Utilities/systemUtil.h"

void vTrk_saveModelDataPoints(const TString outputFile = "Data/vJetTrk/zBosons/model_vTrk_5TeV.root");

void vTrk_saveModelDataPoints(const TString outputFile)
{
   std::cout<<"running vTrk_saveModelDataPoints()"<<std::endl;

   TFile* output = new TFile(outputFile.Data(),"RECREATE");
   std::string dirName = "THEORY";
   dirName = "";
   std::string dirTitle = "Z+hadron at 5.02 TeV predictions";

   if (dirName.size() > 0) {
       output->mkdir(dirName.c_str(), dirTitle.c_str());
       output->cd(dirName.c_str());
   }

   std::cout << "output file : " << outputFile.Data() << std::endl;

   TGraph* gr = new TGraph();

   std::cout << "Hybrid calculations" << std::endl;

   int nObs_Hybrid = HYBRID::kN_MODEL;
   for (int i = 0; i < nObs_Hybrid; ++i) {

       std::string objectName = HYBRID::modelTAG[i];

       std::vector<double> x = HYBRID::x_arr[i];
       std::vector<double> y_min = HYBRID::y_min_arr[i];
       std::vector<double> y_max = HYBRID::y_max_arr[i];

       gr = new TGraph();
       setTGraphBand(gr, x, y_min, y_max);
       gr->SetLineWidth(3);
       gr->SetLineColor(HYBRID::colors[i]);
       gr->SetFillColor(HYBRID::colors[i]);
       gr->SetName(Form("gr_%s", objectName.c_str()));
       gr->SetTitle(Form("%s", HYBRID::legendEntries[i].c_str()));
       gr->Write("",TObject::kOverwrite);
   }

   std::cout << "SCET_G calculations" << std::endl;

   int nObs_SCET_G = SCET_G::kN_MODEL;
   for (int i = 0; i < nObs_SCET_G; ++i) {

       std::string objectName = SCET_G::modelTAG[i];

       std::vector<double> x = SCET_G::x_arr[i];
       std::vector<double> y = SCET_G::y_arr[i];

       int nPoints = x.size();
       double xArr[nPoints];
       std::copy(x.begin(), x.end(), xArr);
       double yArr[nPoints];
       std::copy(y.begin(), y.end(), yArr);
       gr = new TGraph(nPoints, xArr, yArr);
       gr->SetLineWidth(3);
       gr->SetLineColor(SCET_G::fillColors[i]);
       gr->SetFillColor(SCET_G::fillColors[i]);
       gr->SetName(Form("gr_%s", objectName.c_str()));
       gr->SetTitle(Form("%s", SCET_G::legendEntries[i].c_str()));
       gr->Write("",TObject::kOverwrite);
   }

   std::cout<<"Closing the output file"<<std::endl;
   output->Close();

   std::cout<<"running vTrk_saveModelDataPoints() - END"<<std::endl;
}

int main(int argc, char** argv)
{
    if (argc == 2) {
        vTrk_saveModelDataPoints(argv[1]);
        return 0;
    }
    else if (argc == 1) {
        vTrk_saveModelDataPoints();
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./vTrk_saveModelDataPoints.exe <outputFile>"
                << std::endl;
        return 1;
    }
    return 0;
}
