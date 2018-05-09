/*
 * macro to save data tables from models for photon+jet JS at 5.02 TeV
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

#include "photonJetJSData.h"
#include "../../Utilities/tgraphUtil.h"
#include "../../Utilities/th1Util.h"
#include "../../Utilities/systemUtil.h"

void saveModelDataTables_photonJetJS(std::string outputFile = "Data/photonJetJS/model_photonJetJS_5TeV.root");

void saveModelDataTables_photonJetJS(std::string outputFile)
{
   std::cout<<"running saveModelDataTables_photonJetJS()"<<std::endl;

   TFile* output = TFile::Open(outputFile.c_str(),"RECREATE");
   //std::string dirName = "THEORY";
   //std::string dirTitle = "photon+jet JS at 5.02 TeV predictions";
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

   std::cout<<"Closing the output file"<<std::endl;
   output->Close();

   std::cout<<"running saveModelDataTables_photonJetJS() - END"<<std::endl;
}

int main(int argc, char** argv)
{
    if (argc == 2) {
        saveModelDataTables_photonJetJS(argv[1]);
        return 0;
    }
    else if (argc == 1) {
        saveModelDataTables_photonJetJS();
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./saveModelDataTables_photonJetJS.exe <outputFile>"
                << std::endl;
        return 1;
    }
    return 0;
}
