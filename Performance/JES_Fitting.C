#include <TFile.h>
#include <TH1D.h>
#include <TF1.h>
#include <TString.h>
#include "../Histogramming/interface/correlationHist.h"
#include <iostream>
#include "../TreeHeaders/CutConfigurationTree.h"
#include "../Utilities/interface/CutConfigurationParser.h"
#include "../Utilities/interface/InputConfigurationParser.h"

int JES_Fitting(TString configFile, TString inputName, TString outputName){

  InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
  CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

  if (!configInput.isValid) {
    std::cout << "Invalid input configuration" << std::endl;
    return 1;
  } else if (!configCuts.isValid) {
    std::cout << "Invalid cut configuration" << std::endl;
    return 1;
  }

  // input configuration
  const int collision = configInput.proc[INPUT::kHISTOGRAM].i[INPUT::k_collisionType];
  const char* collisionName = getCollisionTypeName((COLL::TYPE)collision).c_str();
  const bool isHI = collisionIsHI((COLL::TYPE)collision);

  // observable bins
  std::vector<float> bins_pt[2];          // array of vectors for eta bins, each array element is a vector.
  std::vector<int>   bins_hiBin[2];       // array of vectors for hiBin bins, each array element is a vector.

  bins_pt[0] = ConfigurationParser::ParseListFloat(
    configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_pt_gt]);
  bins_pt[1] = ConfigurationParser::ParseListFloat(
    configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_pt_lt]);
  bins_hiBin[0] = ConfigurationParser::ParseListInteger(
    configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_gt]);
  bins_hiBin[1] = ConfigurationParser::ParseListInteger(
    configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_lt]);
  const float energyScaleJet = configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].f[CUTS::JET::k_energyScale];

  const int nBins_pt = bins_pt[0].size();         // assume <myvector>[0] and <myvector>[1] have the same size.
  const int nBins_hiBin = bins_hiBin[0].size();     // assume <myvector>[0] and <myvector>[1] have the same size.
  const std::vector<std::string> correlationHistNames {
    "xjg", "dphi", "ptJet"
      };

  TFile *input = TFile::Open(inputName);
  TFile *output = TFile::Open(outputName, "RECREATE");
  TTree *configTree = setupConfigurationTreeForWriting(configCuts);
  // histograms will be put under a directory whose name is the type of the collision
  if (!output->GetKey(collisionName))
    output->mkdir(collisionName, Form("input file is %s", inputName.Data()));
  output->cd(collisionName);

  int nCorrHist = correlationHistNames.size();
  correlationHist corrHists[nCorrHist][nBins_pt][nBins_hiBin];
  TF1 *fitfunc_xjg[nCorrHist][nBins_pt][nBins_hiBin][CORR::kN_CORRFNC][CORR::kN_CORRFNC];

  // nPho Histograms
  TH1D* h_nPho[nBins_pt][nBins_hiBin][2];
  for (int i=0; i<nBins_pt; ++i) {
    for (int j=0; j<nBins_hiBin; ++j) {
      std::string histNamePhoRAW = Form("h_nPho_ptBin%d_hiBin%d_%s", i, j, CORR::CORR_PHO_LABELS[CORR::kRAW].c_str());
      h_nPho[i][j][CORR::kRAW] = (TH1D*)input->Get(Form("%s/%s", collisionName, histNamePhoRAW.c_str()));

      std::string histNamePhoBKG = Form("h_nPho_ptBin%d_hiBin%d_%s", i, j, CORR::CORR_PHO_LABELS[CORR::kBKG].c_str());
      h_nPho[i][j][CORR::kBKG] = (TH1D*)input->Get(Form("%s/%s", collisionName, histNamePhoBKG.c_str()));
      h_nPho[i][j][CORR::kRAW]->Write("",TObject::kOverwrite);
      h_nPho[i][j][CORR::kBKG]->Write("",TObject::kOverwrite);
    }
  }

  // prepare histogram names for xjg, abs(dphi) and jet pt
  for (int iHist=0; iHist<nCorrHist; ++iHist) {
    for (int i=0; i<nBins_pt; ++i) {
      for (int j=0; j<nBins_hiBin; ++j) {
        corrHists[iHist][i][j].name = Form("%s_ptBin%d_hiBin%d", correlationHistNames[iHist].c_str(), i, j);

        for (int iCorr = 0; iCorr < CORR::kN_CORRFNC-1; ++iCorr) {
          for (int jCorr = 0; jCorr < CORR::kN_CORRFNC-1; ++jCorr) {
            std::string subHistName = Form("%s_ptBin%d_hiBin%d_%s_%s", correlationHistNames[iHist].c_str(), i, j,
                                           CORR::CORR_PHO_LABELS[iCorr].c_str(), CORR::CORR_JET_LABELS[jCorr].c_str());
            corrHists[iHist][i][j].h1D_name[iCorr][jCorr] = subHistName.c_str();
            corrHists[iHist][i][j].h1D[iCorr][jCorr] = (TH1D*)input->Get(Form("%s/h1D_%s", collisionName, subHistName.c_str()));

            if (!corrHists[iHist][i][j].h1D[iCorr][jCorr])
              std::cout << "Histogram not found: " << subHistName.c_str() << std::endl;

	    if (jCorr == CORR::kBKG && !isHI) continue;
	    if( iHist == 0 ) { //xjg
	      TString funcName = Form("fitfunc_%s_ptBin%d_hiBin%d_%s_%s",
				      correlationHistNames[iHist].c_str(), i, j,
				      CORR::CORR_PHO_LABELS[iCorr].c_str(),
				      CORR::CORR_JET_LABELS[jCorr].c_str());
	      fitfunc_xjg[iHist][i][j][iCorr][jCorr] = new TF1(funcName,"gaus(0) + gaus(3)",0,2);
	      fitfunc_xjg[iHist][i][j][iCorr][jCorr]->SetParameter(0, 1.0);
	      fitfunc_xjg[iHist][i][j][iCorr][jCorr]->SetParameter(1, 1.0);
	      fitfunc_xjg[iHist][i][j][iCorr][jCorr]->SetParameter(2, 0.5);
	      fitfunc_xjg[iHist][i][j][iCorr][jCorr]->SetParameter(3, 1.0);
	      fitfunc_xjg[iHist][i][j][iCorr][jCorr]->SetParameter(4, 0.8);
	      fitfunc_xjg[iHist][i][j][iCorr][jCorr]->SetParameter(5, 0.5);
	      corrHists[iHist][i][j].h1D[iCorr][jCorr]->Fit(funcName,"M 0 Q");
	      corrHists[iHist][i][j].h1D[iCorr][jCorr]->Fit(funcName,"M 0 Q");

	      std::cout << subHistName << " : " << funcName << std::endl;
	    
	      fitfunc_xjg[iHist][i][j][iCorr][jCorr]->Write("", TObject::kOverwrite);
	      for(int bin = 1; bin <= corrHists[iHist][i][j].h1D[iCorr][jCorr]->GetNbinsX(); ++bin){
		double x = corrHists[iHist][i][j].h1D[iCorr][jCorr]->GetBinCenter(bin);
		corrHists[iHist][i][j].h1D[iCorr][jCorr]->SetBinContent(bin, fitfunc_xjg[iHist][i][j][iCorr][jCorr]->Eval(x/energyScaleJet));
	      }
	      corrHists[iHist][i][j].h1D[iCorr][jCorr]->Write("", TObject::kOverwrite);

	    } else if (iHist == 1){ //dphi
	      corrHists[iHist][i][j].h1D[iCorr][jCorr]->Write("", TObject::kOverwrite);
	    } else if (iHist == 2) { //jetpt
	      corrHists[iHist][i][j].h1D[iCorr][jCorr]->Write("", TObject::kOverwrite);
	    }
	  }
	}
      }
    }
  }
  configTree->Write("", TObject::kOverwrite);
  output->Write("", TObject::kOverwrite);
  return 0;
}

int main(int argc, char** argv) {
  if (argc == 4)
    return JES_Fitting(argv[1], argv[2], argv[3]);
  else
    printf("Usage : \n"
           "./JES_Fitting.exe <configFile> <inputFile> <outputFile>\n");

  return 1;
}
