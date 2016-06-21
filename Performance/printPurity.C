#include <TFile.h>
#include <TTree.h>

#include "../Utilities/interface/CutConfigurationParser.h"
#include "../Utilities/interface/InputConfigurationParser.h"

#include "../Histogramming/PhotonPurity.h"

void printPurity(const TString configFile, const TString inputFile, const TString inputMC){
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

  // observable bins
  std::vector<float> bins_pt[2];          // array of vectors for eta bins, each array element is a vector.
  std::vector<int>   bins_hiBin[2];       // array of vectors for hiBin bins, each array element is a vector.
  // photon cuts
  std::string trigger;
  float cut_phoHoverE;
  float cut_sumIso;
  std::string jetCollection;

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
  const TCut noiseCut = "!((phoE3x3[phoIdx]/phoE5x5[phoIdx] > 2/3-0.03 && phoE3x3[phoIdx]/phoE5x5[phoIdx] < 2/3+0.03) && (phoE1x5[phoIdx]/phoE5x5[phoIdx] > 1/3-0.03 && phoE1x5[phoIdx]/phoE5x5[phoIdx] < 1/3+0.03) && (phoE2x5[phoIdx]/phoE5x5[phoIdx] > 2/3-0.03 && phoE2x5[phoIdx]/phoE5x5[phoIdx] < 2/3+0.03))";
  const TCut sidebandIsolation = "((pho_ecalClusterIsoR4[phoIdx] + pho_hcalRechitIsoR4[phoIdx] + pho_trackIsoR4PtCut20[phoIdx])>10) && ((pho_ecalClusterIsoR4[phoIdx] + pho_hcalRechitIsoR4[phoIdx] + pho_trackIsoR4PtCut20[phoIdx])<20)";
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
	selectionPho = Form("phoEt[phoIdx] >= %f && phoEt[phoIdx] < %f", bins_pt[0][i], bins_pt[1][i]);
      } else {
	selectionPho = Form("phoEt[phoIdx] >= %f", bins_pt[0][i]);
      }
      selectionPho = selectionPho && etaCut;

      TCut selectionIso = "";
      selectionIso = selectionIso && Form("(pho_ecalClusterIsoR4[phoIdx] + pho_hcalRechitIsoR4[phoIdx] + pho_trackIsoR4PtCut20[phoIdx]) < %f", cut_sumIso);
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
  }
  else {
    std::cout << "Usage : \n" <<
      "./printPurity.exe <configFile> <inputFile> <inputMCFile>"
	      << std::endl;
    return 1;
  }
}
