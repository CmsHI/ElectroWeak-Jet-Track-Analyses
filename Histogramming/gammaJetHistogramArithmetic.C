#include <TFile.h>
#include <TTree.h>
#include <TCut.h>
#include <TH1D.h>
#include <TMath.h>
#include "TF1.h"

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

#include "interface/correlationHist.h"
#include "../TreeHeaders/gammaJetTree.h"
#include "../TreeHeaders/CutConfigurationTree.h"
#include "../Utilities/interface/CutConfigurationParser.h"
#include "../Utilities/interface/InputConfigurationParser.h"

const std::vector<std::string> correlationHistNames {
  "xjg", "dphi", "ptJet"
    };
const std::vector<std::string> correlationHistTitleX {
  "p^{Jet}_{T}/p^{#gamma}_{T}", "#Delta#phi_{J#gamma}", "p^{Jet}_{T}"
    };
const std::vector<std::string> correlationHistTitleY_final_normalized {
  "#frac{1}{N_{#gamma}} #frac{dN_{J#gamma}}{dx_{J#gamma}}",
    "#frac{1}{N_{#gamma}} #frac{dN_{J#gamma}}{d#Delta#phi}",
    "#frac{1}{N_{#gamma}} #frac{dN_{J#gamma}}{dp^{Jet}_{T}}"
    };
const std::vector<int>         nBinsx {16, 20,          30};
const std::vector<double>      xlow   {0,  0,           0};
const std::vector<double>      xup    {2,  TMath::Pi(), 300};
// const std::vector<double>      xlow_final {0,  0,           0};
// const std::vector<double>      xup_final  {2,  TMath::Pi(), 200};

void gammaJetHistogramArithmetic(const TString configFile, const TString inputFile, const TString outputFile)
{
  TH1::SetDefaultSumw2();

  std::cout << "running gammaJetHistogramArithmetic()" << std::endl;
  std::cout << "configFile  = " << configFile.Data() << std::endl;
  std::cout << "inputFile   = " << inputFile.Data() << std::endl;
  std::cout << "outputFile  = " << outputFile.Data() << std::endl;

  InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
  CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());
  if (!configCuts.isValid) {
    std::cout << "Invalid configfile, terminating." << std::endl;
    return;
  }

  // input configuration
  int collision;
  collision = configInput.proc[INPUT::kHISTOGRAM].i[INPUT::k_collisionType];
  // verbose about input configuration
  std::cout << "Input Configuration :" << std::endl;
  const char* collisionName = getCollisionTypeName((COLL::TYPE)collision).c_str();
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

  // dphi width/pedestal plots as a function of photon pt, for different centrality bins
  std::vector<int> pt_bin_numbers;
  pt_bin_numbers = ConfigurationParser::ParseListInteger(configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_pt_bin_numbers]);
  const int n_pt_bins = pt_bin_numbers.size();

  // dphi width/pedestal plots as a function of centrality, for different photon pt bins
  std::vector<int> cent_bin_numbers;
  cent_bin_numbers = ConfigurationParser::ParseListInteger(configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_cent_bin_numbers]);
  int n_cent_bins = cent_bin_numbers.size();

  int nEventsToMix;
  int nSmear;
  nEventsToMix = configCuts.proc[CUTS::kSKIM].obj[CUTS::kGAMMAJET].i[CUTS::GJT::k_nEventsToMix];
  nSmear = configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].i[CUTS::JET::k_nSmear];

  int nBins_pt = bins_pt[0].size();         // assume <myvector>[0] and <myvector>[1] have the same size.
  int nBins_hiBin = bins_hiBin[0].size();     // assume <myvector>[0] and <myvector>[1] have the same size.

  /// Purity Calculation Block ///
  // these lists of constants should really be in the conf. Cheat for time for now.
  const float tempPbPbPurity[56] = {0.706449, 0.695512, 0.753847, 0.672904, 0.70562, 0.745369, 0.779227, 0.720351, 0.70361, 0.783455, 0.672856, 0.723058, 0.763915, 0.845693, 0.696766, 0.685863, 0.74166, 0.641307, 0.692725, 0.731084, 0.76233, 0.704944, 0.690111, 0.750074, 0.654865, 0.696883, 0.748097, 0.760527, 0.706296, 0.686822, 0.77576, 0.638959, 0.703232, 0.751671, 0.838696, 0.739794, 0.720459, 0.792379, 0.688511, 0.738537, 0.771016, 0.860922, 0.72964, 0.701675, 0.786383, 0.663564, 0.705977, 0.761013, 0.862756, 0.752235, 0.734993, 0.799376, 0.67615, 0.763525, 0.77734, 0.871363};

  const float tempPbPbMCPurity[56] = {0.984186, 0.93407, 0.986594, 0.830477, 0.94706, 0.97694, 0.981124, 0.973568, 0.910024, 0.978645, 0.825902, 0.918562, 0.967179, 0.977758, 0.969806, 0.899218, 0.978373, 0.811125, 0.914797, 0.968709, 0.970082, 0.969542, 0.911165, 0.970947, 0.831977, 0.893451, 0.936001, 0.96505, 0.961371, 0.864521, 0.973363, 0.795989, 0.856696, 0.968921, 0.973975, 0.967064, 0.907272, 0.967539, 0.804415, 0.925754, 0.940879, 0.970023, 0.964022, 0.889302, 0.959816, 0.831406, 0.882411, 0.930203, 0.962523, 0.95241, 0.872021, 0.964027, 0.756482, 0.890066, 0.930738, 0.969223};

  const float tempPPPurity[56] = {0.824017, 0.824017, 0.824017, 0.824017, 0.824017, 0.824017, 0.824017, 0.845796, 0.845796, 0.845796, 0.845796, 0.845796, 0.845796, 0.845796, 0.820373, 0.820373, 0.820373, 0.820373, 0.820373, 0.820373, 0.820373, 0.831518, 0.831518, 0.831518, 0.831518, 0.831518, 0.831518, 0.831518, 0.846746, 0.846746, 0.846746, 0.846746, 0.846746, 0.846746, 0.846746, 0.856995, 0.856995, 0.856995, 0.856995, 0.856995, 0.856995, 0.856995, 0.858343, 0.858343, 0.858343, 0.858343, 0.858343, 0.858343, 0.858343, 0.858506, 0.858506, 0.858506, 0.858506, 0.858506, 0.858506, 0.858506};

  const float tempPPMCPurity[56] = {0.983406, 0.983406, 0.983406, 0.983406, 0.983406, 0.983406, 0.983406, 0.982107, 0.982107, 0.982107, 0.982107, 0.982107, 0.982107, 0.982107, 0.973667, 0.973667, 0.973667, 0.973667, 0.973667, 0.973667, 0.973667, 0.985271, 0.985271, 0.985271, 0.985271, 0.985271, 0.985271, 0.985271, 0.974137, 0.974137, 0.974137, 0.974137, 0.974137, 0.974137, 0.974137, 0.985353, 0.985353, 0.985353, 0.985353, 0.985353, 0.985353, 0.985353, 0.977951, 0.977951, 0.977951, 0.977951, 0.977951, 0.977951, 0.977951, 0.988275, 0.988275, 0.988275, 0.988275, 0.988275, 0.988275, 0.988275};

  double purity[nBins_pt][nBins_hiBin];   // fixed for the moment.
  for (int i = 0; i<nBins_pt; ++i) {
    for (int j = 0; j<nBins_hiBin; ++j) {
      if (isHI && !isMC) {
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
  if (!output->GetKey(collisionName)) output->mkdir(collisionName, Form("input file is %s", inputFile.Data()));
  output->cd(collisionName);
  std::cout << "histograms will be put under directory : " << collisionName << std::endl;

  int nCorrHist = correlationHistNames.size();
  correlationHist corrHists[nCorrHist][nBins_pt][nBins_hiBin];

  // nPho Histograms
  TH1D* h_nPho[nBins_pt][nBins_hiBin][2];
  for (int i=0; i<nBins_pt; ++i) {
    for (int j=0; j<nBins_hiBin; ++j) {
      std::string histNamePhoRAW = Form("h_nPho_ptBin%d_hiBin%d_%s", i, j, CORR::CORR_PHO_LABELS[CORR::kRAW].c_str());
      h_nPho[i][j][CORR::kRAW] = (TH1D*)input->Get(Form("%s/%s", collisionName, histNamePhoRAW.c_str()));

      std::string histNamePhoBKG = Form("h_nPho_ptBin%d_hiBin%d_%s", i, j, CORR::CORR_PHO_LABELS[CORR::kBKG].c_str());
      h_nPho[i][j][CORR::kBKG] = (TH1D*)input->Get(Form("%s/%s", collisionName, histNamePhoBKG.c_str()));
    }
  }


  // prepare histogram names for xjg, abs(dphi) and jet pt
  // if the collision is not HI, then cannot split it into hiBins.
  //if (!isHI) nBins_hiBin = 1;
  for (int iHist=0; iHist<nCorrHist; ++iHist) {
    for (int i=0; i<nBins_pt; ++i) {
      for (int j=0; j<nBins_hiBin; ++j) {
        corrHists[iHist][i][j].name = Form("%s_ptBin%d_hiBin%d", correlationHistNames[iHist].c_str(), i, j);

        for (int iCorr = 0; iCorr < CORR::kN_CORRFNC; ++iCorr) {
          for (int jCorr = 0; jCorr < CORR::kN_CORRFNC; ++jCorr) {
            std::string subHistName = Form("%s_ptBin%d_hiBin%d_%s_%s", correlationHistNames[iHist].c_str(), i, j,
                                           CORR::CORR_PHO_LABELS[iCorr].c_str(), CORR::CORR_JET_LABELS[jCorr].c_str());
            corrHists[iHist][i][j].h1D_name[iCorr][jCorr] = subHistName.c_str();
            if (iCorr > 1 || jCorr > 1) {
              corrHists[iHist][i][j].h1D[iCorr][jCorr] = new TH1D(Form("h1D_%s", subHistName.c_str()), "",
                                                                  nBinsx[iHist], xlow[iHist], xup[iHist]);
              continue;
            }
            // } else {
            corrHists[iHist][i][j].h1D[iCorr][jCorr] = (TH1D*)input->Get(Form("%s/h1D_%s", collisionName, subHistName.c_str()));

            if (!corrHists[iHist][i][j].h1D[iCorr][jCorr]) {
              std::cout << "Histogram not found: " << subHistName.c_str() << std::endl;
            }
            //}

            corrHists[iHist][i][j].h1D_titleX[iCorr][jCorr] = correlationHistTitleX[iHist].c_str();
            corrHists[iHist][i][j].h1D_titleY_final_norm[iCorr][jCorr] = correlationHistTitleY_final_normalized[iHist].c_str();

            if (jCorr == CORR::kBKG && !isHI) continue;
            corrHists[iHist][i][j].h1D[iCorr][jCorr]->Scale(1, "width"); // distribution histogram : scale by bin width

            // FINAL
            std::string tmpHistName = corrHists[iHist][i][j].h1D[iCorr][jCorr]->GetName();
            corrHists[iHist][i][j].h1D_final[iCorr][jCorr] =
              (TH1D*)corrHists[iHist][i][j].h1D[iCorr][jCorr]->Clone(Form("%s_final", tmpHistName.c_str()));
            // double tmpXlow = xlow_final[iHist];
            // double tmpXup  = xup_final[iHist];
            // corrHists[iHist][i][j].h1D_final[iCorr][jCorr]->SetAxisRange(tmpXlow, tmpXup);
            corrHists[iHist][i][j].h1D_final[iCorr][jCorr]->Write("", TObject::kOverwrite);

            // FINAL_NORM
            corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr] =
              (TH1D*)corrHists[iHist][i][j].h1D_final[iCorr][jCorr]->Clone(Form("%s_final_norm", tmpHistName.c_str()));
            if (jCorr == CORR::kBKG && isHI) {   // (hasJetsMB && hasGammaJetMB) ==> isHI
              // normalize first by the number of mixed events
              corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->Scale(1./nEventsToMix);
            }
            if (nSmear > 0 && nSmear != 1) {
              //first correct actual bin value
              corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->Scale(1./nSmear);
              //then increase statistical bin error by 10 to account for 100 "fake" smearing
              for (int ibin = 1;
                   ibin <= corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->GetNbinsX();
                   ibin++)
              {
                corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->SetBinError
                  (ibin,
                   TMath::Sqrt(nSmear)*
                   corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->GetBinError(ibin));
              }
            }
            // normalization is done with photon events, not necessarily by photon-jet events
            // so the integral of the normalized histogram is R_jg.
            double tmpNEntriesPho = h_nPho[i][j][iCorr]->GetBinContent(1);
            corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->Scale(1./tmpNEntriesPho);
            std::string tmpTitleY_final_norm = corrHists[iHist][i][j].h1D_titleY_final_norm[iCorr][jCorr].c_str();
            corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->GetYaxis()->SetTitle(tmpTitleY_final_norm.c_str());
            corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->Write("", TObject::kOverwrite);
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
  double bins_rjg[n_pt_bins+1];
  double bins_xjg_mean[n_pt_bins+1];
  for(int i = 0; i < n_pt_bins; ++i){
    if(i != 0){
      if(bins_pt[0][pt_bin_numbers[i]] != bins_pt[1][pt_bin_numbers[i-1]]){
        std::cout << "ERROR: pt bins are not arranged correctly." << std::endl;
      }
    }
    bins_rjg[i] = bins_pt[0][pt_bin_numbers[i]];
    bins_xjg_mean[i] = bins_pt[0][pt_bin_numbers[i]];
  }
  bins_rjg[n_pt_bins] = bins_pt[0][pt_bin_numbers[n_pt_bins-1]]+20;
  bins_xjg_mean[n_pt_bins] = bins_pt[0][pt_bin_numbers[n_pt_bins-1]]+20;

  for (int j=0; j<nBins_hiBin; ++j) {
    for (int iCorr = 0; iCorr < CORR::kN_CORRFNC; ++iCorr) {
      for (int jCorr = 0; jCorr < CORR::kN_CORRFNC; ++jCorr) {
        // rjg
        std::string subHistName;
        subHistName = Form("%s_ptBinAll_hiBin%d_%s_%s", correlationHistNames_ptBinAll[0].c_str(), j,
                           CORR::CORR_PHO_LABELS[iCorr].c_str(), CORR::CORR_JET_LABELS[jCorr].c_str());
        corrHists_ptBinAll[0][j].h1D_name[iCorr][jCorr] = subHistName.c_str();
        corrHists_ptBinAll[0][j].h1D[iCorr][jCorr] = new TH1D(Form("h1D_%s", subHistName.c_str()), "", n_pt_bins, bins_rjg);

        // x_jg_mean
        subHistName = Form("%s_ptBinAll_hiBin%d_%s_%s", correlationHistNames_ptBinAll[1].c_str(), j,
                           CORR::CORR_PHO_LABELS[iCorr].c_str(), CORR::CORR_JET_LABELS[jCorr].c_str());
        corrHists_ptBinAll[1][j].h1D_name[iCorr][jCorr] = subHistName.c_str();
        corrHists_ptBinAll[1][j].h1D[iCorr][jCorr] = new TH1D(Form("h1D_%s", subHistName.c_str()), "", n_pt_bins, bins_xjg_mean);
      }
    }
  }

  // histograms with centrality on the x-axis
  std::vector<std::string> correlationHistNames_centBinAll = {"rjg", "xjg_mean"};
  int nCorrHist_centBinAll = correlationHistNames_centBinAll.size();
  correlationHist corrHists_centBinAll[nCorrHist_centBinAll][nBins_pt];
  // prepare histogram names for rjg and <xjg>
  double bins_rjg_cent[n_cent_bins+1];
  double bins_xjg_mean_cent[n_cent_bins+1];
  for(int i = 0; i < n_cent_bins; ++i){
    if(i != 0){
      if(bins_hiBin[0][cent_bin_numbers[i]] != bins_hiBin[1][cent_bin_numbers[i-1]]){
        std::cout << "ERROR: cent bins are not arranged correctly." << std::endl;
      }
    }
    bins_rjg_cent[i] = bins_hiBin[0][cent_bin_numbers[i]];
    bins_xjg_mean_cent[i] = bins_hiBin[0][cent_bin_numbers[i]];
  }
  bins_rjg_cent[n_cent_bins] = bins_hiBin[1][cent_bin_numbers[n_cent_bins-1]];
  bins_xjg_mean_cent[n_cent_bins] = bins_hiBin[1][cent_bin_numbers[n_cent_bins-1]];

  for (int j=0; j<nBins_pt; ++j) {
    for (int iCorr = 0; iCorr < CORR::kN_CORRFNC; ++iCorr) {
      for (int jCorr = 0; jCorr < CORR::kN_CORRFNC; ++jCorr) {
        // rjg
        std::string subHistName;
        subHistName = Form("%s_centBinAll_ptBin%d_%s_%s", correlationHistNames_centBinAll[0].c_str(), j,
                           CORR::CORR_PHO_LABELS[iCorr].c_str(), CORR::CORR_JET_LABELS[jCorr].c_str());
        corrHists_centBinAll[0][j].h1D_name[iCorr][jCorr] = subHistName.c_str();
        corrHists_centBinAll[0][j].h1D[iCorr][jCorr] = new TH1D(Form("h1D_%s", subHistName.c_str()), "", n_cent_bins, bins_rjg_cent);

        // x_jg_mean
        subHistName = Form("%s_centBinAll_ptBin%d_%s_%s", correlationHistNames_centBinAll[1].c_str(), j,
                           CORR::CORR_PHO_LABELS[iCorr].c_str(), CORR::CORR_JET_LABELS[jCorr].c_str());
        corrHists_centBinAll[1][j].h1D_name[iCorr][jCorr] = subHistName.c_str();
        corrHists_centBinAll[1][j].h1D[iCorr][jCorr] = new TH1D(Form("h1D_%s", subHistName.c_str()), "", n_cent_bins, bins_xjg_mean_cent);
      }
    }
  }

  for (int iHist = 0; iHist < nCorrHist; iHist++) {
    for (int i=0; i<nBins_pt; ++i) {
      for (int j=0; j<nBins_hiBin; ++j) {
        // histograms for RAW and BKG regions
        for (int iCorr = 0; iCorr < CORR::kN_CORRFNC -1; ++iCorr) {
          for (int jCorr = 0; jCorr < CORR::kN_CORRFNC -1; ++jCorr) {
            if (jCorr == CORR::kBKG && !isHI) continue;      // no jet background for non-HI

            corrHists[iHist][i][j].h1D[iCorr][jCorr]->Write("", TObject::kOverwrite);
            corrHists[iHist][i][j].h1D_final[iCorr][jCorr]->Write("", TObject::kOverwrite);
            corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->Write("", TObject::kOverwrite);
          }
        }

        //std::cout << "making histograms for SIG regions" << std::endl;
        // calculate SIGSIG histogram,
        // these histograms are ignored : SIGRAW, SIGBKG

        // no background for PP and PA
        if (!isHI) {
          // for non-HI, reset jet BKG histograms
          // in that case BKG histograms are not used, but set to empty histograms.
          // so it becomes SIG = RAW
          std::cout << "collision is " << collisionName << std::endl;
          std::cout << "contribution from BKG region is set to zero." << std::endl;

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
        //std::cout << tmpHistNameRAWSIG.c_str() << std::endl;
        corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kSIG] =
          (TH1D*)corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kRAW]->Clone(tmpHistNameRAWSIG.c_str());
        // do arithmetic if histograms are not empty
        if (corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kBKG]->GetEntries() > 0) {
          corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kSIG]->Add(corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kBKG], -1);
        }

        // BKGSIG = BKGRAW - BKGBKG
        std::string tmpHistNameBKGSIG = Form("%s_final_norm", corrHists[iHist][i][j].h1D[CORR::kBKG][CORR::kSIG]->GetName());
        //std::cout << tmpHistNameBKGSIG.c_str() << std::endl;
        corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kSIG] =
          (TH1D*)corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kRAW]->Clone(tmpHistNameBKGSIG.c_str());
        // do arithmetic if histograms are not empty
        if (corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kBKG]->GetEntries() > 0) {
          corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kSIG]->Add(corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kBKG], -1);
        }

        // subtract photon BKG
        // purity*SIGSIG + (1-purity)*BKGSIG = RAWSIG
        // SIGSIG = 1/purity *  ( RAWSIG - (1-purity) * BKGSIG )
        std::string tmpHistNameSIGSIG = Form("%s_final_norm", corrHists[iHist][i][j].h1D[CORR::kSIG][CORR::kSIG]->GetName());
        //std::cout << tmpHistNameSIGSIG.c_str() << std::endl;
        corrHists[iHist][i][j].h1D_final_norm[CORR::kSIG][CORR::kSIG] =
          (TH1D*)corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kSIG]->Clone(tmpHistNameSIGSIG.c_str());
        // do arithmetic if histograms are not empty
        if (corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kSIG]->GetEntries() > 0) {
          corrHists[iHist][i][j].h1D_final_norm[CORR::kSIG][CORR::kSIG]->Add(corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kSIG], -1*(1-purity[i][j]));
          corrHists[iHist][i][j].h1D_final_norm[CORR::kSIG][CORR::kSIG]->Scale(1./purity[i][j]);
        }
        //std::cout << "purity[i][j] = " << purity[i][j] << std::endl;

        // correct dphi normalization
        if (iHist == 1) {
          corrHists[iHist][i][j].h1D_final_norm[CORR::kSIG][CORR::kSIG]->Scale(1./corrHists[iHist][i][j].h1D_final_norm[CORR::kSIG][CORR::kSIG]->Integral());
        }

        // FINAL_NORM  RAWSIG
        std::string tmpH1D_nameRAWSIG = corrHists[iHist][i][j].h1D_name[CORR::kRAW][CORR::kSIG].c_str();
        //std::cout << "drawing tmpH1D_nameRAWSIG = " << tmpH1D_nameRAWSIG.c_str() << std::endl;
        corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kSIG]->Write("", TObject::kOverwrite);

        // FINAL_NORM  BKGSIG
        std::string tmpH1D_nameBKGSIG = corrHists[iHist][i][j].h1D_name[CORR::kBKG][CORR::kSIG].c_str();
        //std::cout << "drawing tmpH1D_nameBKGSIG = " << tmpH1D_nameBKGSIG.c_str() << std::endl;
        corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kSIG]->Write("", TObject::kOverwrite);

        // FINAL_NORM  SIGSIG
        std::string tmpH1D_nameSIGSIG = corrHists[iHist][i][j].h1D_name[CORR::kSIG][CORR::kSIG].c_str();
        //std::cout << "drawing tmpH1D_nameSIGSIG = " << tmpH1D_nameSIGSIG.c_str() << std::endl;
        corrHists[iHist][i][j].h1D_final_norm[CORR::kSIG][CORR::kSIG]->Write("", TObject::kOverwrite);

        std::cout << Form("histogramming END : ptBin%d HiBin%d", i, j) << std::endl;
        //std::cout << "##########" << std::endl;
      }
    }
    std::cout << "histogramming END : " << correlationHistNames[iHist].c_str() << std::endl;
    std::cout << "####################" << std::endl;
  }

  // fit dphi
  TF1* dphi_fit_func = new TF1("dphi_fit_func", "[0]+[1]*exp((x-3.14159265358979323)/[2])", 0, TMath::Pi());
  dphi_fit_func->SetParameters(0.01, 0.5, 0.3);

  TF1* fit_dphi[nBins_pt][nBins_hiBin];
  for (int i=0; i<nBins_pt; ++i) {
    for (int j=0; j<nBins_hiBin; ++j) {
      corrHists[1][i][j].h1D_final_norm[CORR::kSIG][CORR::kSIG]->Fit("dphi_fit_func", "QREM0", "", TMath::Pi()*2/3, TMath::Pi());
      fit_dphi[i][j] = corrHists[1][i][j].h1D_final_norm[CORR::kSIG][CORR::kSIG]->GetFunction("dphi_fit_func");
      fit_dphi[i][j]->Write(Form("fit_dphi_ptBin%i_hiBin%i", i, j), TObject::kOverwrite);
    }
  }

  float dphi_fit_pt_bins[n_pt_bins + 2];
  dphi_fit_pt_bins[0] = 0;
  dphi_fit_pt_bins[1] = bins_pt[0][pt_bin_numbers[0]];
  for (int i=0; i<n_pt_bins; ++i)
    dphi_fit_pt_bins[i+2] = bins_pt[1][pt_bin_numbers[i]];
  if (dphi_fit_pt_bins[n_pt_bins+1] > 200)
    dphi_fit_pt_bins[n_pt_bins+1] = 200;

  TH1D* h_dphi_width_pt[nBins_hiBin];
  TH1D* h_dphi_pedestal_pt[nBins_hiBin];
  for (int i=0; i<nBins_hiBin; ++i) {
    h_dphi_width_pt[i] = new TH1D(Form("h1D_dphi_width_ptBinAll_hiBin%i", i), "", n_pt_bins+1, dphi_fit_pt_bins);
    h_dphi_pedestal_pt[i] = new TH1D(Form("h1D_dphi_pedestal_ptBinAll_hiBin%i", i), "", n_pt_bins+1, dphi_fit_pt_bins);

    for (int j=0; j<n_pt_bins; ++j) {
      h_dphi_width_pt[i]->SetTitle(";p^{#gamma}_{T} (GeV/c);#Delta#phi width");
      h_dphi_width_pt[i]->SetBinContent(j+2, fit_dphi[pt_bin_numbers[j]][i]->GetParameter(2));
      h_dphi_width_pt[i]->SetBinError(j+2, fit_dphi[pt_bin_numbers[j]][i]->GetParError(2));
      h_dphi_pedestal_pt[i]->SetTitle(";p^{#gamma}_{T} (GeV/c);#Delta#phi pedestal");
      h_dphi_pedestal_pt[i]->SetBinContent(j+2, fit_dphi[pt_bin_numbers[j]][i]->GetParameter(0));
      h_dphi_pedestal_pt[i]->SetBinError(j+2, fit_dphi[pt_bin_numbers[j]][i]->GetParError(0));
    }
  }

  float dphi_fit_cent_bins[n_cent_bins + 1];
  dphi_fit_cent_bins[0] = bins_hiBin[0][cent_bin_numbers[0]];
  for (int i=0; i<n_cent_bins; ++i)
    dphi_fit_cent_bins[i+1] = bins_hiBin[1][cent_bin_numbers[i]];

  TH1D* h_dphi_width_cent[nBins_pt];
  TH1D* h_dphi_pedestal_cent[nBins_pt];
  for (int i=0; i<nBins_pt; ++i) {
    h_dphi_width_cent[i] = new TH1D(Form("h1D_dphi_width_centBinAll_ptBin%i", i), "", n_cent_bins, dphi_fit_cent_bins);
    h_dphi_pedestal_cent[i] = new TH1D(Form("h1D_dphi_pedestal_centBinAll_ptBin%i", i), "", n_cent_bins, dphi_fit_cent_bins);

    for (int j=0; j<n_cent_bins; ++j) {
      h_dphi_width_cent[i]->SetTitle(";Centrality;#Delta#phi width");
      h_dphi_width_cent[i]->SetBinContent(j+1, fit_dphi[i][cent_bin_numbers[j]]->GetParameter(2));
      h_dphi_width_cent[i]->SetBinError(j+1, fit_dphi[i][cent_bin_numbers[j]]->GetParError(2));
      h_dphi_pedestal_cent[i]->SetTitle(";Centrality;#Delta#phi pedestal");
      h_dphi_pedestal_cent[i]->SetBinContent(j+1, fit_dphi[i][cent_bin_numbers[j]]->GetParameter(0));
      h_dphi_pedestal_cent[i]->SetBinError(j+1, fit_dphi[i][cent_bin_numbers[j]]->GetParError(0));
    }
  }

// histograms with pt bins on x-axis
// corrHists_ptBinAll[0][]  = R_jg
// corrHists_ptBinAll[1][]  = <X_jg>
  std::cout << "####################" << std::endl;
  for (int j=0; j<nBins_hiBin; ++j) {
    for (int iCorr=0; iCorr<CORR::kN_CORRFNC; ++iCorr) {
      for (int jCorr=0; jCorr<CORR::kN_CORRFNC; ++jCorr) {
        // ignore SIGRAW, SIGBKG histograms
        if ((iCorr == CORR::kSIG && (jCorr == CORR::kRAW || jCorr == CORR::kBKG))) continue;

        // if (j>0) continue;

        //int offset = 2; // ptBin 40-50 starts from index 2.
        // rjg block
        for (int i=0; i<n_pt_bins; ++i) {

          double err;
          double val = corrHists[0][pt_bin_numbers[i]][j].h1D_final_norm[iCorr][jCorr]->IntegralAndError(1,
                                                                                                         corrHists[0][pt_bin_numbers[i]][j].h1D_final_norm[iCorr][jCorr]->GetNbinsX(), err, "width");

          corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->SetBinContent(i+1, val);
          corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->SetBinError(i+1, err);
        }

        std::string histoTitle = Form("%s , %d-%d %%", collisionName , bins_hiBin[0][j]/2, bins_hiBin[1][j]/2);

        corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->SetTitle(Form("%s;p^{#gamma}_{T} (GeV/c); R_{J#gamma}", histoTitle.c_str()));
        corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->SetMarkerStyle(kFullCircle);
        corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->SetMarkerColor(kBlack);

        //std::cout << "drawing : " << corrHists_ptBinAll[0][j].h1D_name[iCorr][jCorr].c_str() << std::endl;
        corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->Write("", TObject::kOverwrite);

        // xjg_mean block
        for (int i=0; i<n_pt_bins; ++i) {
          double val = corrHists[0][pt_bin_numbers[i]][j].h1D_final_norm[iCorr][jCorr]->GetMean();
          double err = corrHists[0][pt_bin_numbers[i]][j].h1D_final_norm[iCorr][jCorr]->GetMeanError();

          corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->SetBinContent(i+1, val);
          corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->SetBinError(i+1, err);
        }

        corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->SetTitle(Form("%s;p^{#gamma}_{T} (GeV/c); <x_{J#gamma}>", histoTitle.c_str()));
        corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->SetMarkerStyle(kFullCircle);
        corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->SetMarkerColor(kBlack);

        //std::cout << "drawing : " << corrHists_ptBinAll[1][j].h1D_name[iCorr][jCorr].c_str() << std::endl;
        corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->Write("", TObject::kOverwrite);
      }
    }
  }
  std::cout << "####################" << std::endl;

// histograms with centrality bins on x-axis
// corrHists_centBinAll[0][]  = R_jg
// corrHists_centBinAll[1][]  = <X_jg>
  std::cout << "####################" << std::endl;
  for (int j=0; j<nBins_pt; ++j) {
    for (int iCorr=0; iCorr<CORR::kN_CORRFNC; ++iCorr) {
      for (int jCorr=0; jCorr<CORR::kN_CORRFNC; ++jCorr) {
        // ignore SIGRAW, SIGBKG histograms
        if ((iCorr == CORR::kSIG && (jCorr == CORR::kRAW || jCorr == CORR::kBKG))) continue;

        // if (j>0 && !isHI) continue;

        //int offset = 3; // hiBin 0-10 starts from index 3.
        // rjg block
        for (int i=0; i<n_cent_bins; ++i) {
          double err;
          double val = corrHists[0][j][cent_bin_numbers[i]].h1D_final_norm[iCorr][jCorr]->IntegralAndError(1,
                                                                                                           corrHists[0][j][cent_bin_numbers[i]].h1D_final_norm[iCorr][jCorr]->GetNbinsX(), err, "width");

          corrHists_centBinAll[0][j].h1D[iCorr][jCorr]->SetBinContent(i+1, val);
          corrHists_centBinAll[0][j].h1D[iCorr][jCorr]->SetBinError(i+1, err);

          // std::cout << "ENTER" << std::endl;
          // std::cout << "histname: " << corrHists[0][j][cent_bin_numbers[i]].h1D_final_norm[iCorr][jCorr]->GetName() << std::endl;
          // std::cout << "outhistname: " << corrHists_centBinAll[0][j].h1D[iCorr][jCorr]->GetName() << std::endl;
          // std::cout << "ptBin: " << j << " hiBin: " << cent_bin_numbers[i] << std::endl;
          // std::cout << "Rjg: " << val << std::endl;
        }

        std::string histoTitle = Form("%s , %.0f < p^{#gamma}_{T} < %.0f GeV/c", collisionName , bins_pt[0][j], bins_pt[1][j]);

        corrHists_centBinAll[0][j].h1D[iCorr][jCorr]->SetTitle(Form("%s;Centrality Bin; R_{J#gamma}", histoTitle.c_str()));
        corrHists_centBinAll[0][j].h1D[iCorr][jCorr]->SetMarkerStyle(kFullCircle);
        corrHists_centBinAll[0][j].h1D[iCorr][jCorr]->SetMarkerColor(kBlack);

        //std::cout << "drawing : " << corrHists_centBinAll[0][j].h1D_name[iCorr][jCorr].c_str() << std::endl;
        corrHists_centBinAll[0][j].h1D[iCorr][jCorr]->Write("", TObject::kOverwrite);

        // xjg_mean block
        for (int i=0; i<n_cent_bins; ++i) {
          double val = corrHists[0][j][cent_bin_numbers[i]].h1D_final_norm[iCorr][jCorr]->GetMean();
          double err = corrHists[0][j][cent_bin_numbers[i]].h1D_final_norm[iCorr][jCorr]->GetMeanError();

          corrHists_centBinAll[1][j].h1D[iCorr][jCorr]->SetBinContent(i+1, val);
          corrHists_centBinAll[1][j].h1D[iCorr][jCorr]->SetBinError(i+1, err);
        }

        corrHists_centBinAll[1][j].h1D[iCorr][jCorr]->SetTitle(Form("%s;Centrality Bin; <x_{J#gamma}>", histoTitle.c_str()));
        corrHists_centBinAll[1][j].h1D[iCorr][jCorr]->SetMarkerStyle(kFullCircle);
        corrHists_centBinAll[1][j].h1D[iCorr][jCorr]->SetMarkerColor(kBlack);

        //std::cout << "drawing : " << corrHists_centBinAll[1][j].h1D_name[iCorr][jCorr].c_str() << std::endl;
        corrHists_centBinAll[1][j].h1D[iCorr][jCorr]->Write("", TObject::kOverwrite);
      }
    }
  }
  std::cout << "####################" << std::endl;

  configTree->Write("", TObject::kOverwrite);

  output->Write("", TObject::kOverwrite);
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
