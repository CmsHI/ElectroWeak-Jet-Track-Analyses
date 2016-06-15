#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TCut.h>
#include <TH1D.h>
#include <TMath.h>

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

#include "interface/correlationHist.h"
#include "../TreeHeaders/gammaJetTree.h"
#include "../TreeHeaders/CutConfigurationTree.h"
#include "../Utilities/interface/CutConfigurationParser.h"
#include "../Utilities/interface/InputConfigurationParser.h"

const std::vector<std::string> correlationHistNames   {"xjg", "dphi", "ptJet"};
const std::vector<std::string> correlationHistTitleX  {"p^{Jet}_{T}/p^{#gamma}_{T}", "#Delta#phi_{J#gamma}", "p^{Jet}_{T}"};
const std::vector<std::string> correlationHistTitleY_final_normalized{"#frac{1}{N_{#gamma}} #frac{dN_{J#gamma}}{dx_{J#gamma}}",
    "#frac{1}{N_{#gamma}} #frac{dN_{J#gamma}}{d#Delta#phi}",
    "#frac{1}{N_{#gamma}} #frac{dN_{J#gamma}}{dp^{Jet}_{T}}"};
const std::vector<int>         nBinsx{16, 20,          30};
const std::vector<double>      xlow  {0,  0,           0};
const std::vector<double>      xup   {2,  TMath::Pi(), 300};
const std::vector<double>      xlow_final{0,  0,           0};
const std::vector<double>      xup_final {2,  TMath::Pi(), 200};

void gammaJetHistogramArithmetic(const TString configFile, const TString inputFile, const TString outputFile)
{
  TH1::SetDefaultSumw2();

  std::cout<<"running gammaJetHistogramArithmetic()"<<std::endl;
  std::cout<<"configFile  = "<< configFile.Data() <<std::endl;
  std::cout<<"inputFile   = "<< inputFile.Data() <<std::endl;
  std::cout<<"outputFile  = "<< outputFile.Data() <<std::endl;

  InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
  CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());
  if(!configCuts.isValid){
    std::cout << "Invalid configfile, terminating." << std::endl;
    return;
  }

  // input configuration
  int collision;
  collision = configInput.proc[INPUT::kHISTOGRAM].i[INPUT::k_collisionType];
  // verbose about input configuration
  std::cout<<"Input Configuration :"<<std::endl;
  const char* collisionName =  getCollisionTypeName((COLL::TYPE)collision).c_str();
  std::cout << "collision = " << collisionName << std::endl;

  bool isMC = collisionIsMC((COLL::TYPE)collision);
  bool isHI = collisionIsHI((COLL::TYPE)collision);

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

  int nBins_pt = bins_pt[0].size();         // assume <myvector>[0] and <myvector>[1] have the same size.
  int nBins_hiBin = bins_hiBin[0].size();     // assume <myvector>[0] and <myvector>[1] have the same size.

  /// Purity Calculation Block ///
  // these lists of constants should really be in the conf. Cheat for time for now.
  const float tempPbPbPurity[56] = {0.676347, 0.686488, 0.756964, 0.675387, 0.703763, 0.752848, 0.787882, 0.712814, 0.707078, 0.784051, 0.675911, 0.72968, 0.76778, 0.844154, 0.663561, 0.678369, 0.746681, 0.651645, 0.693355, 0.742194, 0.776277, 0.680703, 0.679884, 0.757597, 0.662675, 0.689962, 0.752508, 0.769309, 0.703061, 0.693244, 0.780901, 0.639828, 0.713101, 0.760176, 0.841252, 0.73165, 0.727308, 0.788082, 0.711999, 0.738944, 0.771933, 0.855741, 0.720931, 0.713083, 0.77815, 0.682542, 0.708191, 0.759067, 0.849453, 0.744423, 0.735621, 0.803791, 0.691025, 0.763615, 0.778766, 0.886822};

  const float tempPbPbMCPurity[56] = {0.676347, 0.686488, 0.756964, 0.675387, 0.703763, 0.752848, 0.787882, 0.712814, 0.707078, 0.784051, 0.675911, 0.72968, 0.76778, 0.844154, 0.663561, 0.678369, 0.746681, 0.651645, 0.693355, 0.742194, 0.776277, 0.680703, 0.679884, 0.757597, 0.662675, 0.689962, 0.752508, 0.769309, 0.703061, 0.693244, 0.780901, 0.639828, 0.713101, 0.760176, 0.841252, 0.73165, 0.727308, 0.788082, 0.711999, 0.738944, 0.771933, 0.855741, 0.720931, 0.713083, 0.77815, 0.682542, 0.708191, 0.759067, 0.849453, 0.744423, 0.735621, 0.803791, 0.691025, 0.763615, 0.778766, 0.886822};

  const float tempPPPurity[56] = {0.676347, 0.686488, 0.756964, 0.675387, 0.703763, 0.752848, 0.787882, 0.712814, 0.707078, 0.784051, 0.675911, 0.72968, 0.76778, 0.844154, 0.663561, 0.678369, 0.746681, 0.651645, 0.693355, 0.742194, 0.776277, 0.680703, 0.679884, 0.757597, 0.662675, 0.689962, 0.752508, 0.769309, 0.703061, 0.693244, 0.780901, 0.639828, 0.713101, 0.760176, 0.841252, 0.73165, 0.727308, 0.788082, 0.711999, 0.738944, 0.771933, 0.855741, 0.720931, 0.713083, 0.77815, 0.682542, 0.708191, 0.759067, 0.849453, 0.744423, 0.735621, 0.803791, 0.691025, 0.763615, 0.778766, 0.886822};

  const float tempPPMCPurity[56] = {0.676347, 0.686488, 0.756964, 0.675387, 0.703763, 0.752848, 0.787882, 0.712814, 0.707078, 0.784051, 0.675911, 0.72968, 0.76778, 0.844154, 0.663561, 0.678369, 0.746681, 0.651645, 0.693355, 0.742194, 0.776277, 0.680703, 0.679884, 0.757597, 0.662675, 0.689962, 0.752508, 0.769309, 0.703061, 0.693244, 0.780901, 0.639828, 0.713101, 0.760176, 0.841252, 0.73165, 0.727308, 0.788082, 0.711999, 0.738944, 0.771933, 0.855741, 0.720931, 0.713083, 0.77815, 0.682542, 0.708191, 0.759067, 0.849453, 0.744423, 0.735621, 0.803791, 0.691025, 0.763615, 0.778766, 0.886822};
    
  double purity[nBins_pt][nBins_hiBin];   // fixed for the moment.
  for (int i = 0; i<nBins_pt; ++i){
    for (int j = 0; j<nBins_hiBin; ++j){
      if(isHI && !isMC){
	purity[i][j] = tempPbPbPurity[i*nBins_hiBin+j];
      } else if (isHI && isMC) {
	purity[i][j] = tempPbPbMCPurity[i*nBins_hiBin+j];
      } else if (!isHI && !isMC) {
	purity[i][j] = tempPPPurity[i*nBins_hiBin+j];
      } else {
	purity[i][j] = tempPPMCPurity[i*nBins_hiBin+j];
      }
    }
  }
  /// End Purity Block ///
  TFile* output = TFile::Open(outputFile, "RECREATE");
  
  TTree *configTree = setupConfigurationTreeForWriting(configCuts);

  TFile *input = TFile::Open(inputFile);

  // histograms will be put under a directory whose name is the type of the collision
  if(!output->GetKey(collisionName)) output->mkdir(collisionName, Form("input file is %s", inputFile.Data()));
  output->cd(collisionName);
  std::cout<<"histograms will be put under directory : " << collisionName << std::endl;


  int nCorrHist = correlationHistNames.size();
  correlationHist corrHists[nCorrHist][nBins_pt][nBins_hiBin];

  // prepare histogram names for xjg, abs(dphi) and jet pt
  // if the collision is not HI, then cannot split it into hiBins.
  //if (!isHI) nBins_hiBin = 1;
  for (int iHist=0; iHist<nCorrHist; ++iHist){
    for (int i=0; i<nBins_pt; ++i){
      for(int j=0; j<nBins_hiBin; ++j){
	corrHists[iHist][i][j].name = Form("%s_ptBin%d_hiBin%d", correlationHistNames[iHist].c_str(), i, j);

	for (int iCorr = 0; iCorr < CORR::kN_CORRFNC; ++iCorr) {
	  for (int jCorr = 0; jCorr < CORR::kN_CORRFNC; ++jCorr) {

	    std::string subHistName = Form("%s_ptBin%d_hiBin%d_%s_%s", correlationHistNames[iHist].c_str(), i, j,
					   CORR::CORR_PHO_LABELS[iCorr].c_str(), CORR::CORR_JET_LABELS[jCorr].c_str());
	    corrHists[iHist][i][j].h1D_name[iCorr][jCorr] = subHistName.c_str();
	    if(iCorr > 1 && jCorr > 1){
	      corrHists[iHist][i][j].h1D[iCorr][jCorr] = new TH1D(Form("h1D_%s", subHistName.c_str()),"",
								  nBinsx[iHist], xlow[iHist], xup[iHist]);
	    } else {
	      corrHists[iHist][i][j].h1D[iCorr][jCorr] = (TH1D*)input->Get(Form("%s/h1D_%s",collisionName,subHistName.c_str()));
	      corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr] = (TH1D*)input->Get(Form("%s/h1D_%s_final_norm",collisionName,subHistName.c_str()));
	      if(!corrHists[iHist][i][j].h1D[iCorr][jCorr]){
		std::cout << "Histogram not found: " << subHistName.c_str() << std::endl;
	      }
	    }

	    corrHists[iHist][i][j].h1D_titleX[iCorr][jCorr] = correlationHistTitleX[iHist].c_str();
	    corrHists[iHist][i][j].h1D_titleY_final_norm[iCorr][jCorr] = correlationHistTitleY_final_normalized[iHist].c_str();
	  }
	}
      }
    }
  }

  output->cd(collisionName);
  
  // histograms with pt bins on x-axis
  std::vector<std::string> correlationHistNames_ptBinAll = {"rjg", "xjg_mean"};       // histograms where x-axis is pt bins
  int nCorrHist_ptBinAll = correlationHistNames_ptBinAll.size();
  correlationHist corrHists_ptBinAll[nCorrHist_ptBinAll][nBins_hiBin];
  // prepare histogram names for rjg and <xjg>
  const int nBins_rjg = 4;
  const int nBins_xjg_mean= 4;
  double bins_rjg[nBins_rjg+1] =           {40, 50, 60, 80, 120};
  double bins_xjg_mean[nBins_xjg_mean+1] = {40, 50, 60, 80, 120};

  for (int j=0; j<nBins_hiBin; ++j){
    for (int iCorr = 0; iCorr < CORR::kN_CORRFNC; ++iCorr) {
      for (int jCorr = 0; jCorr < CORR::kN_CORRFNC; ++jCorr) {

	// rjg
	std::string subHistName;
	subHistName = Form("%s_ptBinAll_hiBin%d_%s_%s", correlationHistNames_ptBinAll[0].c_str(), j,
			   CORR::CORR_PHO_LABELS[iCorr].c_str(), CORR::CORR_JET_LABELS[jCorr].c_str());
	corrHists_ptBinAll[0][j].h1D_name[iCorr][jCorr] = subHistName.c_str();
	corrHists_ptBinAll[0][j].h1D[iCorr][jCorr] = new TH1D(Form("h1D_%s", subHistName.c_str()), "",nBins_rjg, bins_rjg);

	// x_jg_mean
	subHistName = Form("%s_ptBinAll_hiBin%d_%s_%s", correlationHistNames_ptBinAll[1].c_str(), j,
			   CORR::CORR_PHO_LABELS[iCorr].c_str(), CORR::CORR_JET_LABELS[jCorr].c_str());
	corrHists_ptBinAll[1][j].h1D_name[iCorr][jCorr] = subHistName.c_str();
	corrHists_ptBinAll[1][j].h1D[iCorr][jCorr] = new TH1D(Form("h1D_%s", subHistName.c_str()), "",nBins_xjg_mean, bins_xjg_mean);
      }
    }
  }

  // histograms with centrality on the x-axis
  std::vector<std::string> correlationHistNames_centBinAll = {"rjg", "xjg_mean"};
  int nCorrHist_centBinAll = correlationHistNames_centBinAll.size();
  correlationHist corrHists_centBinAll[nCorrHist_centBinAll][nBins_pt];
  // prepare histogram names for rjg and <xjg>
  const int nBins_rjg_cent = 4;
  const int nBins_xjg_mean_cent= 4;
  double bins_rjg_cent[nBins_rjg+1] =           {0, 20, 60, 100, 200};
  double bins_xjg_mean_cent[nBins_xjg_mean+1] = {0, 20, 60, 100, 200};

  for (int j=0; j<nBins_pt; ++j){
    for (int iCorr = 0; iCorr < CORR::kN_CORRFNC; ++iCorr) {
      for (int jCorr = 0; jCorr < CORR::kN_CORRFNC; ++jCorr) {

	// rjg
	std::string subHistName;
	subHistName = Form("%s_centBinAll_ptBin%d_%s_%s", correlationHistNames_centBinAll[0].c_str(), j,
			   CORR::CORR_PHO_LABELS[iCorr].c_str(), CORR::CORR_JET_LABELS[jCorr].c_str());
	corrHists_centBinAll[0][j].h1D_name[iCorr][jCorr] = subHistName.c_str();
	corrHists_centBinAll[0][j].h1D[iCorr][jCorr] = new TH1D(Form("h1D_%s", subHistName.c_str()), "",nBins_rjg_cent, bins_rjg_cent);

	// x_jg_mean
	subHistName = Form("%s_centBinAll_ptBin%d_%s_%s", correlationHistNames_centBinAll[1].c_str(), j,
			   CORR::CORR_PHO_LABELS[iCorr].c_str(), CORR::CORR_JET_LABELS[jCorr].c_str());
	corrHists_centBinAll[1][j].h1D_name[iCorr][jCorr] = subHistName.c_str();
	corrHists_centBinAll[1][j].h1D[iCorr][jCorr] = new TH1D(Form("h1D_%s", subHistName.c_str()), "",nBins_xjg_mean_cent, bins_xjg_mean_cent);
      }
    }
  }
  
  TCanvas* c = new TCanvas("cnv","",600,600);
  for(int iHist = 0; iHist < nCorrHist; iHist++){
    for (int i=0; i<nBins_pt; ++i){
      for(int j=0; j<nBins_hiBin; ++j){

	std::cout<<"making histograms for SIG regions"<<std::endl;
	// calculate SIGSIG histogram,
	// these histograms are ignored : SIGRAW, SIGBKG

	// no background for PP and PA
	if(!isHI){
	  // for non-HI, reset jet BKG histograms
	  // in that case BKG histograms are not used, but set to empty histograms.
	  // so it becomes SIG = RAW
	  std::cout<<"collision is "<< collisionName <<std::endl;
	  std::cout<<"contribution from BKG region is set to zero."<< std::endl;

	  std::string tmpHistName;

	  // reset RAWBKG
	  tmpHistName = Form("%s_final_norm", corrHists[iHist][i][j].h1D[CORR::kRAW][CORR::kBKG]->GetName());
	  corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kBKG] =
	    (TH1D*)corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kRAW]->Clone(tmpHistName.c_str());
	  corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kBKG]->Reset();

	  // reset BKGBKG
	  tmpHistName = Form("%s_final_norm", corrHists[iHist][i][j].h1D[CORR::kBKG][CORR::kBKG]->GetName());
	  corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kBKG] =
	    (TH1D*)corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kRAW]->Clone(tmpHistName.c_str());
	  corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kBKG]->Reset();
	}

	// first subtract jet BKG, then subtract photon BKG
	// subtract jet BKG
	// RAWSIG = RAWRAW - RAWBKG
	std::string tmpHistNameRAWSIG = Form("%s_final_norm", corrHists[iHist][i][j].h1D[CORR::kRAW][CORR::kSIG]->GetName());
	std::cout<<tmpHistNameRAWSIG.c_str()<<std::endl;
	corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kSIG] =
	  (TH1D*)corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kRAW]->Clone(tmpHistNameRAWSIG.c_str());
	// do arithmetic if histograms are not empty
	if (corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kBKG]->GetEntries() > 0) {
	  corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kSIG]->Add(corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kBKG],-1);
	}

	// BKGSIG = BKGRAW - BKGBKG
	std::string tmpHistNameBKGSIG = Form("%s_final_norm", corrHists[iHist][i][j].h1D[CORR::kBKG][CORR::kSIG]->GetName());
	std::cout<<tmpHistNameBKGSIG.c_str()<<std::endl;
	corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kSIG] =
	  (TH1D*)corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kRAW]->Clone(tmpHistNameBKGSIG.c_str());
	// do arithmetic if histograms are not empty
	if (corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kBKG]->GetEntries() > 0) {
	  corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kSIG]->Add(corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kBKG],-1);
	}

	// subtract photon BKG
	// purity*SIGSIG + (1-purity)*BKGSIG = RAWSIG
	// SIGSIG = 1/purity *  ( RAWSIG - (1-purity) * BKGSIG )
	std::string tmpHistNameSIGSIG = Form("%s_final_norm", corrHists[iHist][i][j].h1D[CORR::kSIG][CORR::kSIG]->GetName());
	std::cout<<tmpHistNameSIGSIG.c_str()<<std::endl;
	corrHists[iHist][i][j].h1D_final_norm[CORR::kSIG][CORR::kSIG] =
	  (TH1D*)corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kSIG]->Clone(tmpHistNameSIGSIG.c_str());
	// do arithmetic if histograms are not empty
	if (corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kSIG]->GetEntries() > 0) {
	  corrHists[iHist][i][j].h1D_final_norm[CORR::kSIG][CORR::kSIG]->Add(corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kSIG],-1*(1-purity[i][j]));
	  corrHists[iHist][i][j].h1D_final_norm[CORR::kSIG][CORR::kSIG]->Scale(1./purity[i][j]);
	}
	std::cout<< "purity[i][j] = " << purity[i][j] << std::endl;

	// FINAL_NORM  RAWSIG
	std::string tmpH1D_nameRAWSIG = corrHists[iHist][i][j].h1D_name[CORR::kRAW][CORR::kSIG].c_str();
	std::cout<<"drawing tmpH1D_nameRAWSIG = "<<tmpH1D_nameRAWSIG.c_str()<<std::endl;
	c->SetName(Form("cnv_%s_final_norm",tmpH1D_nameRAWSIG.c_str()));
	c->cd();
	corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kSIG]->Draw("e");
	corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kSIG]->Write("",TObject::kOverwrite);
	corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kSIG]->SetStats(false);  // remove stat box from the canvas, but keep in the histograms.
	c->Write("",TObject::kOverwrite);
	c->Clear();

	// FINAL_NORM  BKGSIG
	std::string tmpH1D_nameBKGSIG = corrHists[iHist][i][j].h1D_name[CORR::kBKG][CORR::kSIG].c_str();
	std::cout<<"drawing tmpH1D_nameBKGSIG = "<<tmpH1D_nameBKGSIG.c_str()<<std::endl;
	c->SetName(Form("cnv_%s_final_norm",tmpH1D_nameBKGSIG.c_str()));
	c->cd();
	corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kSIG]->Draw("e");
	corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kSIG]->Write("",TObject::kOverwrite);
	corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kSIG]->SetStats(false);  // remove stat box from the canvas, but keep in the histograms.
	c->Write("",TObject::kOverwrite);
	c->Clear();

	// FINAL_NORM  SIGSIG
	std::string tmpH1D_nameSIGSIG = corrHists[iHist][i][j].h1D_name[CORR::kSIG][CORR::kSIG].c_str();
	std::cout<<"drawing tmpH1D_nameSIGSIG = "<<tmpH1D_nameSIGSIG.c_str()<<std::endl;
	c->SetName(Form("cnv_%s_final_norm",tmpH1D_nameSIGSIG.c_str()));
	c->cd();
	corrHists[iHist][i][j].h1D_final_norm[CORR::kSIG][CORR::kSIG]->Draw("e");
	corrHists[iHist][i][j].h1D_final_norm[CORR::kSIG][CORR::kSIG]->Write("",TObject::kOverwrite);
	corrHists[iHist][i][j].h1D_final_norm[CORR::kSIG][CORR::kSIG]->SetStats(false);  // remove stat box from the canvas, but keep in the histograms.
	c->Write("",TObject::kOverwrite);
	c->Clear();

	std::cout<<Form("histogramming END : ptBin%d HiBin%d", i, j)<<std::endl;
	std::cout<<"##########"<<std::endl;
      }
    }
    std::cout<<"histogramming END : "<<correlationHistNames[iHist].c_str()<<std::endl;
    std::cout<<"####################"<<std::endl;
  }

// histograms with pt bins on x-axis
// corrHists_ptBinAll[0][]  = R_jg
// corrHists_ptBinAll[1][]  = <X_jg>
  std::cout<<"####################"<<std::endl;
  for(int j=0; j<nBins_hiBin; ++j){
    for(int iCorr=0; iCorr<CORR::kN_CORRFNC; ++iCorr) {
      for(int jCorr=0; jCorr<CORR::kN_CORRFNC; ++jCorr) {

	// ignore SIGRAW, SIGBKG histograms
	if ((iCorr == CORR::kSIG && (jCorr == CORR::kRAW || jCorr == CORR::kBKG))) continue;

	//if(j>0) continue;

	int offset = 2; // ptBin 40-50 starts from index 2.
	// rjg block
	for(int i=0; i<nBins_rjg; ++i){

	  double err;
	  double val = corrHists[0][i+offset][j].h1D_final_norm[iCorr][jCorr]->IntegralAndError(1,
												corrHists[0][i+offset][j].h1D_final_norm[iCorr][jCorr]->GetNbinsX(), err);

	  corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->SetBinContent(i+1, val);
	  corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->SetBinError(i+1, err);
	}

	std::string histoTitle = Form("%s , %d-%d %%",collisionName , bins_hiBin[0][j]/2, bins_hiBin[1][j]/2);

	corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->SetTitle(Form("%s;p^{#gamma}_{T} (GeV/c); R_{J#gamma}",histoTitle.c_str()));
	corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->SetMarkerStyle(kFullCircle);
	corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->SetMarkerColor(kBlack);

	std::cout<<"drawing : "<<corrHists_ptBinAll[0][j].h1D_name[iCorr][jCorr].c_str()<<std::endl;
	c->SetName(Form("cnv_%s",corrHists_ptBinAll[0][j].h1D_name[iCorr][jCorr].c_str()));
	c->cd();
	corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->Draw("e");
	corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->Write("",TObject::kOverwrite);
	corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->SetStats(false);     // remove stat box from the canvas, but keep in the histograms.
	c->Write("",TObject::kOverwrite);
	c->Clear();

	// xjg_mean block
	for(int i=0; i<nBins_xjg_mean; ++i){

	  double val = corrHists[0][i+offset][j].h1D_final_norm[iCorr][jCorr]->GetMean();
	  double err = corrHists[0][i+offset][j].h1D_final_norm[iCorr][jCorr]->GetMeanError();

	  corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->SetBinContent(i+1, val);
	  corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->SetBinError(i+1, err);
	}

	corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->SetTitle(Form("%s;p^{#gamma}_{T} (GeV/c); <x_{J#gamma}>",histoTitle.c_str()));
	corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->SetMarkerStyle(kFullCircle);
	corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->SetMarkerColor(kBlack);

	std::cout<<"drawing : "<<corrHists_ptBinAll[1][j].h1D_name[iCorr][jCorr].c_str()<<std::endl;
	c->SetName(Form("cnv_%s",corrHists_ptBinAll[1][j].h1D_name[iCorr][jCorr].c_str()));
	c->cd();
	corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->Draw("e");
	corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->Write("",TObject::kOverwrite);
	corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->SetStats(false);     // remove stat box from the canvas, but keep in the histograms.
	c->Write("",TObject::kOverwrite);
	c->Clear();
      }
    }
  }
  std::cout<<"####################"<<std::endl;

// histograms with centrality bins on x-axis
// corrHists_centBinAll[0][]  = R_jg
// corrHists_centBinAll[1][]  = <X_jg>
  std::cout<<"####################"<<std::endl;
  for(int j=0; j<nBins_pt; ++j){
    for(int iCorr=0; iCorr<CORR::kN_CORRFNC; ++iCorr) {
      for(int jCorr=0; jCorr<CORR::kN_CORRFNC; ++jCorr) {

	// ignore SIGRAW, SIGBKG histograms
	if ((iCorr == CORR::kSIG && (jCorr == CORR::kRAW || jCorr == CORR::kBKG))) continue;

	//if(j>0 && !isHI) continue;

	int offset = 6; // hiBin 0-10 starts from index 6.
	// rjg block
	for(int i=0; i<nBins_rjg_cent; ++i){

	  double err;
	  double val = corrHists[0][j][i+offset].h1D_final_norm[iCorr][jCorr]->IntegralAndError(1,
												corrHists[0][j][i+offset].h1D_final_norm[iCorr][jCorr]->GetNbinsX(), err);

	  corrHists_centBinAll[0][j].h1D[iCorr][jCorr]->SetBinContent(i+1, val);
	  corrHists_centBinAll[0][j].h1D[iCorr][jCorr]->SetBinError(i+1, err);
	}

	std::string histoTitle = Form("%s , %lf-%lf %%",collisionName , bins_pt[0][j], bins_pt[1][j]);

	corrHists_centBinAll[0][j].h1D[iCorr][jCorr]->SetTitle(Form("%s;Centrality Bin; R_{J#gamma}",histoTitle.c_str()));
	corrHists_centBinAll[0][j].h1D[iCorr][jCorr]->SetMarkerStyle(kFullCircle);
	corrHists_centBinAll[0][j].h1D[iCorr][jCorr]->SetMarkerColor(kBlack);

	std::cout<<"drawing : "<<corrHists_centBinAll[0][j].h1D_name[iCorr][jCorr].c_str()<<std::endl;
	c->SetName(Form("cnv_%s",corrHists_centBinAll[0][j].h1D_name[iCorr][jCorr].c_str()));
	c->cd();
	corrHists_centBinAll[0][j].h1D[iCorr][jCorr]->Draw("e");
	corrHists_centBinAll[0][j].h1D[iCorr][jCorr]->Write("",TObject::kOverwrite);
	corrHists_centBinAll[0][j].h1D[iCorr][jCorr]->SetStats(false);     // remove stat box from the canvas, but keep in the histograms.
	c->Write("",TObject::kOverwrite);
	c->Clear();

	// xjg_mean block
	for(int i=0; i<nBins_xjg_mean_cent; ++i){

	  double val = corrHists[0][j][i+offset].h1D_final_norm[iCorr][jCorr]->GetMean();
	  double err = corrHists[0][j][i+offset].h1D_final_norm[iCorr][jCorr]->GetMeanError();

	  corrHists_centBinAll[1][j].h1D[iCorr][jCorr]->SetBinContent(i+1, val);
	  corrHists_centBinAll[1][j].h1D[iCorr][jCorr]->SetBinError(i+1, err);
	}

	corrHists_centBinAll[1][j].h1D[iCorr][jCorr]->SetTitle(Form("%s;Centrality Bin; <x_{J#gamma}>",histoTitle.c_str()));
	corrHists_centBinAll[1][j].h1D[iCorr][jCorr]->SetMarkerStyle(kFullCircle);
	corrHists_centBinAll[1][j].h1D[iCorr][jCorr]->SetMarkerColor(kBlack);

	std::cout<<"drawing : "<<corrHists_centBinAll[1][j].h1D_name[iCorr][jCorr].c_str()<<std::endl;
	c->SetName(Form("cnv_%s",corrHists_centBinAll[1][j].h1D_name[iCorr][jCorr].c_str()));
	c->cd();
	corrHists_centBinAll[1][j].h1D[iCorr][jCorr]->Draw("e");
	corrHists_centBinAll[1][j].h1D[iCorr][jCorr]->Write("",TObject::kOverwrite);
	corrHists_centBinAll[1][j].h1D[iCorr][jCorr]->SetStats(false);     // remove stat box from the canvas, but keep in the histograms.
	c->Write("",TObject::kOverwrite);
	c->Clear();
      }
    }
  }
  std::cout<<"####################"<<std::endl;

  configTree->Write("",TObject::kOverwrite);

  output->Write("",TObject::kOverwrite);
  input->Close();
  output->Close();
}

int main(int argc, char** argv)
{
  if (argc == 4) {
    gammaJetHistogramArithmetic(argv[1], argv[2], argv[3]);
    return 0;
  }
  else {
    std::cout << "Usage : \n" <<
      "./gammaJetHistogramArithmetic.exe <configFile> <inputFile> <outputFile>"
	      << std::endl;
    return 1;
  }
}
