#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TH1D.h>

#include "../Utilities/interface/CutConfigurationParser.h"
#include "../Utilities/interface/InputConfigurationParser.h"

#include "../Histogramming/PhotonPurity.h"
#include "../Plotting/commonUtility.h"

void printPurity(const TString configFile, const TString inputFile, const TString inputMC, const bool savePlots = false){
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

  //bool isMC = collisionIsMC((COLL::TYPE)collision);
  bool isHI = collisionIsHI((COLL::TYPE)collision);
  bool isMC = collisionIsMC((COLL::TYPE)collision);

  std::string collisionTypeLabel = "PbPb";
  if(isHI && isMC) {
    collisionTypeLabel = "PbPb_MC";
  } else if (!isHI && !isMC) {
    collisionTypeLabel = "PP";
  } else if (!isHI && isMC) {
    collisionTypeLabel = "PP_MC";
  }

  // observable bins
  std::vector<float> bins_pt[2];          // array of vectors for eta bins, each array element is a vector.
  std::vector<int>   bins_hiBin[2];       // array of vectors for hiBin bins, each array element is a vector.
  // photon cuts
  std::string trigger;
  float cut_phoHoverE;
  float cut_sumIso;
  std::string jetCollection;
  bool useCorrectedSumIso;

  // process cuts
  bins_pt[0] = ConfigurationParser::ParseListFloat(
    configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_pt_gt]);
  bins_pt[1] = ConfigurationParser::ParseListFloat(
    configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_pt_lt]);
  bins_hiBin[0] = ConfigurationParser::ParseListInteger(
    configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_gt]);
  bins_hiBin[1] = ConfigurationParser::ParseListInteger(
    configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_lt]);

  trigger = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_trigger_gammaJet].c_str();
  // trigger is named differently in MC, hardcode for now :(
  std::string triggerMC_forPurity = "(HLT_HISinglePhoton40_Eta1p5_v2)";
  std::string triggerMC_forPurity_pp = "(HLT_HISinglePhoton40_Eta1p5ForPPRef_v1)";

  cut_phoHoverE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_phoHoverE];
  cut_sumIso = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_sumIso];
  jetCollection = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kJET].s[CUTS::JET::k_jetCollection];
  useCorrectedSumIso = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].i[CUTS::PHO::k_useCorrectedSumIso];

  int nBins_pt = bins_pt[0].size();         // assume <myvector>[0] and <myvector>[1] have the same size.
  int nBins_hiBin = bins_hiBin[0].size();     // assume <myvector>[0] and <myvector>[1] have the same size.

  TFile *input = TFile::Open(inputFile);
  TTree *tHlt = (TTree*)input->Get("hltTree");
  TTree *tPho = (TTree*)input->Get("EventTree");    // photons
  TTree *tgj  = (TTree*)input->Get(Form("gamma_%s", jetCollection.c_str()));
  TTree *tHiEvt = (TTree*)input->Get("HiEvt");       // HiEvt tree will be placed in PP forest as well.

  /// Purity Calculation Block ///
  // mc only needed for purity calc.
  TFile *inputMCFile = TFile::Open(inputMC);
  TTree *tmcHlt = (TTree*)inputMCFile->Get("hltTree");
  TTree *tmcPho = (TTree*)inputMCFile->Get("EventTree");    // photons
  TTree *tmcgj  = (TTree*)inputMCFile->Get(Form("gamma_%s", jetCollection.c_str()));
  TTree *tmcHiEvt = (TTree*)inputMCFile->Get("HiEvt");       // HiEvt tree will be placed in PP forest as well.

  // need to addfriend for purity calculation
  tgj->AddFriend(tHlt, "Hlt");
  tgj->AddFriend(tPho, "Pho");
  tgj->AddFriend(tHiEvt, "HiEvt");

  tmcgj->AddFriend(tmcHlt, "Hlt");
  tmcgj->AddFriend(tmcPho, "Pho");
  tmcgj->AddFriend(tmcHiEvt, "HiEvt");

  // should be migrated to config files ASAP
  // noise cut moved to skim
  const TCut noiseCut = "!((phoE3x3[phoIdx]/phoE5x5[phoIdx] > 2./3.-0.03 && phoE3x3[phoIdx]/phoE5x5[phoIdx] < 2./3.+0.03) && (phoE1x5[phoIdx]/phoE5x5[phoIdx] > 1./3.-0.03 && phoE1x5[phoIdx]/phoE5x5[phoIdx] < 1./3.+0.03) && (phoE2x5[phoIdx]/phoE5x5[phoIdx] > 2./3.-0.03 && phoE2x5[phoIdx]/phoE5x5[phoIdx] < 2./3.+0.03))";
  TCut sidebandIsolation;
  if(useCorrectedSumIso){
    sidebandIsolation = "(pho_sumIsoCorrected>10) && (pho_sumIsoCorrected<20)";
  } else {
    sidebandIsolation = "((pho_ecalClusterIsoR4[phoIdx] + pho_hcalRechitIsoR4[phoIdx] + pho_trackIsoR4PtCut20[phoIdx])>10) && ((pho_ecalClusterIsoR4[phoIdx] + pho_hcalRechitIsoR4[phoIdx] + pho_trackIsoR4PtCut20[phoIdx])<20)";
  }
  const TCut mcIsolation = "(pho_genMatchedIndex[phoIdx]!= -1) && mcCalIsoDR04[pho_genMatchedIndex[phoIdx]]<5 && abs(mcPID[pho_genMatchedIndex[phoIdx]])<=22";
  const TCut etaCut = "abs(phoEta[phoIdx]) < 1.44";


  double purity[nBins_pt][nBins_hiBin];   // fixed for the moment.
  for (int i = 0; i<nBins_pt; ++i){
    for (int j = 0; j<nBins_hiBin; ++j){
      if(!isHI && j > 0) {
	purity[i][j] = purity[i][j-1];
	continue;
      }
      TCut selection_event = Form("%s == 1", trigger.c_str());
      TCut selection_event_mc_forPurity =  Form("%s == 1", triggerMC_forPurity.c_str());
      TCut selection_event_mc_forPurity_pp = Form("%s == 1", triggerMC_forPurity_pp.c_str());
      if (isHI) {
	selection_event = selection_event && Form("hiBin >= %d && hiBin < %d", bins_hiBin[0][j], bins_hiBin[1][j]);
	selection_event_mc_forPurity = selection_event_mc_forPurity && Form("hiBin >= %d && hiBin < %d", bins_hiBin[0][j], bins_hiBin[1][j]);
      }

      TCut selectionPho;
      if (bins_pt[1][i] >= 0){
	selectionPho = Form("phoEtCorrected[phoIdx] >= %f && phoEtCorrected[phoIdx] < %f", bins_pt[0][i], bins_pt[1][i]);
      } else {
	selectionPho = Form("phoEtCorrected[phoIdx] >= %f", bins_pt[0][i]);
      }
      selectionPho = selectionPho && etaCut;

      TCut selectionIso = "";
      selectionIso = selectionIso && Form("pho_sumIsoCorrected < %f", cut_sumIso);
      selectionIso = selectionIso && Form("phoHoverE[phoIdx] < %f", cut_phoHoverE);

      TCut dataCandidateCut = selectionPho && selection_event && etaCut && noiseCut;
      TCut sidebandCut = dataCandidateCut && sidebandIsolation && noiseCut;
      TCut mcSignalCut;
      if(isHI){
	mcSignalCut = selectionPho && selection_event_mc_forPurity && etaCut && mcIsolation;
      } else {
	mcSignalCut = selectionPho && selection_event_mc_forPurity_pp && etaCut && mcIsolation;
      }
      dataCandidateCut = dataCandidateCut && selectionIso;

      PhotonPurity fitr = getPurity(configCuts, tgj, tmcgj,
				    dataCandidateCut, sidebandCut,
				    mcSignalCut);
      purity[i][j] = fitr.purity;

      std::cout << "Purity for ptBin"<< i << " hiBin"<< j << ": " << purity[i][j] << std::endl;
      std::cout << "nSig for ptBin"<< i << " hiBin"<< j << ": " << fitr.nSig << std::endl;
      std::cout << "chisq for ptBin"<< i << " hiBin"<< j << ": " << fitr.chisq << std::endl;

      if(savePlots){
        TCanvas *c1 = new TCanvas();
        TH1F *hSigPdf = fitr.sigPdf;
	TH1F *hBckPdf = fitr.bckPdf;
	TH1D *hData1  = fitr.data;
	hSigPdf->Add(hBckPdf);

        handsomeTH1(hSigPdf);
	mcStyle(hSigPdf);
	sbStyle(hBckPdf);
	cleverRange(hSigPdf,1.5);
	hSigPdf->SetAxisRange(0.001,0.024,"X");
	hSigPdf->SetNdivisions(505);
	hSigPdf->GetYaxis()->SetTitleOffset(1.75);
	hSigPdf->SetYTitle("Entries");
	hSigPdf->SetXTitle("#sigma_{#eta #eta}");
        hSigPdf->SetStats(false);
        hSigPdf->GetXaxis()->CenterTitle();
        hSigPdf->GetYaxis()->CenterTitle();

        hData1->SetMarkerStyle(kFullCircle);
        hData1->SetLineColor(kBlack);
        hData1->SetMarkerColor(kBlack);

	hSigPdf->DrawCopy("hist");
	//drawSys(hSigPdf, err, kRed, -1, 0.001);
	hBckPdf->DrawCopy("same hist");
	hData1->DrawCopy("same e");

        TLegend *t3=new TLegend(0.5, 0.68, 0.92, 0.92);
	t3->AddEntry(hData1,Form("%s #sqrt{s}_{_{NN}}=5.02 TeV",collisionTypeLabel.c_str()),"pl");
	t3->AddEntry(hSigPdf,"Signal","lf");
	t3->AddEntry(hBckPdf,"Background","lf");
	t3->SetFillColor(0);
	t3->SetBorderSize(0);
	t3->SetFillStyle(0);
	t3->SetTextFont(43);
	t3->SetTextSize(20);
        t3->Draw();

        drawText(Form("%.0f GeV < p_{T}^{#gamma} < %.0f GeV",
		      bins_pt[0][i], bins_pt[1][i]),
		 0.5, 0.60,1,20);
	drawText(Form("%.0f - %.0f%c",
	 	      bins_hiBin[0][j]/2., bins_hiBin[1][j]/2.,'%'),
	 	 0.5, 0.52,1,20);
	drawText(Form("Purity (#sigma_{#eta#eta} < 0.01) : %.2f", (Float_t)fitr.purity),
		 0.5, 0.46,1,20);
	drawText(Form("#chi^{2}/ndf : %.2f", (Float_t)fitr.chisq),
		 0.5, 0.38,1,20);

        c1->SaveAs(Form("Performance/photons/purityPlots/%s_purity_ptBin%i_hiBin%i.pdf",collisionTypeLabel.c_str(), i, j));
        delete c1;
      }
    }
  }

  input->Close();
  inputMCFile->Close();
  /// End Purity Block ///

  for (int i = 0; i<nBins_pt; ++i){
    for (int j = 0; j<nBins_hiBin; ++j){
      std::cout << purity[i][j] << ", ";
    }
  }
  std::cout << std::endl;
}

int main(int argc, char** argv)
{
  if (argc == 4) {
    printPurity(argv[1], argv[2], argv[3]);
    return 0;
  } else if (argc ==5) {
    printPurity(argv[1], argv[2], argv[3], atoi(argv[4]));
  }
  else {
    std::cout << "Usage : \n" <<
      "./printPurity.exe <configFile> <inputFile> <inputMCFile>"
	      << std::endl;
    return 1;
  }
}
