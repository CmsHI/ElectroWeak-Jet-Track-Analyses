/*
 * macro to save data tables from models for photon+jet FF at 5.02 TeV
 */
#include <TFile.h>
#include <TDirectoryFile.h>
#include <TH1.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>

#include <string>
#include <iostream>
#include <vector>

#include "photonJetFFData.h"
#include "../../Utilities/tgraphUtil.h"
#include "../../Utilities/th1Util.h"
#include "../../Utilities/systemUtil.h"

void saveModelDataTables_photonJetFF(std::string outputFile = "Data/photonJetFF/model_photonJetFF_5TeV.root");

void saveModelDataTables_photonJetFF(std::string outputFile)
{
   std::cout<<"running saveModelDataTables_photonJetFF()"<<std::endl;

   TFile* output = TFile::Open(outputFile.c_str(),"RECREATE");
   //std::string dirName = "THEORY";
   //std::string dirTitle = "photon+jet FF at 5.02 TeV predictions";
   //output->mkdir(dirName.c_str(), dirTitle.c_str());
   //output->cd(dirName.c_str());

   std::cout << "output file : " << outputFile.c_str() << std::endl;

   TGraph* gr = new TGraph();

   std::cout << "SCET predictions" << std::endl;
   for (int i = 0; i < SCET::DATATABLES::kN_DATATABLES; ++i) {

       std::string modelName = SCET::modelName;
       std::string dataName = SCET::dataNames[i];

       std::vector<double> x = SCET::xData[i];
       std::vector<double> ymin = SCET::yminData[i];
       std::vector<double> ymax = SCET::ymaxData[i];

       int fillColor = SCET::colors[i];
       double falpha = SCET::falphas[i];
       std::string xTitle = SCET::xTitles[i].c_str();
       std::string yTitle = SCET::yTitles[i].c_str();
       std::string title = SCET::titles[i].c_str();

       std::string objectName = Form("gr_%s_%s", modelName.c_str(), dataName.c_str());
       gr = new TGraph();

       setTGraphBand(gr, x, ymin, ymax);
       gr->SetName(objectName.c_str());
       gr->GetXaxis()->SetTitle(xTitle.c_str());
       gr->GetYaxis()->SetTitle(yTitle.c_str());
       gr->SetTitle(title.c_str());
       gr->SetFillColorAlpha(fillColor, falpha);
       gr->SetFillStyle(1001);
       gr->Write("",TObject::kOverwrite);
   }

   std::cout << "COLBT predictions" << std::endl;
   for (int i = 0; i < COLBT::DATATABLES::kN_DATATABLES; ++i) {

       std::string modelName = COLBT::modelName;
       std::string dataName = COLBT::dataNames[i];

       std::vector<double> x = COLBT::xData[i];
       std::vector<double> y = COLBT::yData[i];

       int fillColor = COLBT::colors[i];
       int lineStyle = COLBT::lineStyles[i];
       double falpha = COLBT::falphas[i];
       std::string xTitle = COLBT::xTitles[i].c_str();
       std::string yTitle = COLBT::yTitles[i].c_str();
       std::string title = COLBT::titles[i].c_str();

       std::string objectName = Form("gr_%s_%s", modelName.c_str(), dataName.c_str());
       gr = new TGraphErrors();

       setTGraphErrors((TGraphErrors*)gr, x, y, std::vector<double> ((int)x.size(), 0));
       gr->SetName(objectName.c_str());
       gr->GetXaxis()->SetTitle(xTitle.c_str());
       gr->GetYaxis()->SetTitle(yTitle.c_str());
       gr->SetTitle(title.c_str());
       gr->SetFillColorAlpha(fillColor, falpha);
       gr->SetLineStyle(lineStyle);
       gr->SetFillStyle(1001);
       gr->Write("",TObject::kOverwrite);
   }

   std::cout << "HYBRID predictions" << std::endl;
   for (int i = 0; i < HYBRID::DATATABLES::kN_DATATABLES; ++i) {

       std::string modelName = HYBRID::modelName;
       std::string dataName = HYBRID::dataNames[i];

       std::vector<double> x = HYBRID::xData[i];
       std::vector<double> ymin = HYBRID::yminData[i];
       std::vector<double> ymax = HYBRID::ymaxData[i];
       std::vector<double> y = HYBRID::yData[i];
       std::vector<double> yerr = HYBRID::yerrData[i];

       int fillColor = HYBRID::colors[i];
       double falpha = HYBRID::falphas[i];
       std::string xTitle = HYBRID::xTitles[i].c_str();
       std::string yTitle = HYBRID::yTitles[i].c_str();
       std::string title = HYBRID::titles[i].c_str();

       std::string objectName = Form("gr_%s_%s", modelName.c_str(), dataName.c_str());

       if ((int)ymin.size() > 0) {
           gr = new TGraph();
           setTGraphBand(gr, x, ymin, ymax);
       }
       else {
           gr = new TGraphErrors();
           setTGraphErrors((TGraphErrors*)gr, x, y, yerr);
       }
       gr->SetName(objectName.c_str());
       gr->GetXaxis()->SetTitle(xTitle.c_str());
       gr->GetYaxis()->SetTitle(yTitle.c_str());
       gr->SetTitle(title.c_str());
       gr->SetFillColorAlpha(fillColor, falpha);
       gr->SetFillStyle(1001);
       gr->Write("",TObject::kOverwrite);
   }

   std::cout<<"Closing the output file"<<std::endl;
   output->Close();

   std::cout<<"running saveModelDataTables_photonJetFF() - END"<<std::endl;
}

int main(int argc, char** argv)
{
    if (argc == 2) {
        saveModelDataTables_photonJetFF(argv[1]);
        return 0;
    }
    else if (argc == 1) {
        saveModelDataTables_photonJetFF();
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./saveModelDataTables_photonJetFF.exe <outputFile>"
                << std::endl;
        return 1;
    }
    return 0;
}
