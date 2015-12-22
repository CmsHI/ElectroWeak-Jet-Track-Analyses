#include <TFile.h>
#include <TString.h>
#include <iostream>

#include "../../CutConfigurations/interface/CutConfigurationParser.h"
#include "../../TreeHeaders/CutConfigurationTree.h"
#include "../../TreeHeaders/SetupJetTree.h"
#include "../../TreeHeaders/ggHiNtuplizerTree.h"

void jetSkim(const TString configFile, const TString inputHiForest, const TString outputSkimFile)
{
  TFile *outFile = TFile::Open(outputSkimFile,"RECREATE");

  CutConfiguration config = CutConfigurationParser::Parse(configFile.Data());
  TTree *configTree = setupConfigurationTreeForWriting(config);

  std::string jetCollection = config.proc[CUTS::kSKIM].obj[CUTS::kJET].s[CUTS::JET::k_jetCollection];
  bool requirePhotonInEvent = (bool)config.proc[CUTS::kSKIM].obj[CUTS::kJET].i[CUTS::JET::k_requirePhotonInEvent];
  Float_t photonEtCut = config.proc[CUTS::kSKIM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_et];

  //std::cout << jetCollection << std::endl;

  TFile *inHiForest = TFile::Open(inputHiForest);
  TTree *inTree = (TTree*)inHiForest->Get((jetCollection+"/t").c_str());
  Jets inJets;
  inJets.setupTreeForReading(inTree);

  TTree *inPhoTree = (TTree*)inHiForest->Get("ggHiNtuplizer/EventTree");
  ggHiNtuplizer pho;
  if(requirePhotonInEvent)
    setupPhotonTree(inPhoTree, pho);

  TTree *skimTree = (TTree*)inHiForest->Get("skimanalysis/HltTree");
  Int_t HBHENoiseFilterResult;
  Int_t pcollisionEventSelection;
  skimTree->SetBranchAddress("HBHENoiseFilterResult",&HBHENoiseFilterResult);
  skimTree->SetBranchAddress("pcollisionEventSelection",&pcollisionEventSelection);

  // TTree *evtTree = (TTree*)inHiForest->Get("hiEvtAnalyzer/HiTree");
  // ULong64_t event;
  // unsigned run;
  // unsigned lumi;
  // int hiBin;
  // evtTree->SetBranchAddress("evt",&event);
  // evtTree->SetBranchAddress("run",&run);
  // evtTree->SetBranchAddress("lumi",&lumi);
  // evtTree->SetBranchAddress("hiBin",&hiBin);

  outFile->cd();
  Jets outJets;
  TTree *outTree = new TTree("jetSkimTree","jetSkimTree");
  outJets.setupTreeForWriting(outTree);

  Long64_t nentries = inTree->GetEntries();
  for(int ientries = 0; ientries < nentries; ++ientries)
  {
    if(requirePhotonInEvent){
      inPhoTree->GetEntry(ientries);
      if(pho.nPho < 1) continue;
      Float_t maxPho =-1;
      for(int i = 0; i < pho.nPho; ++i)
      {
	if(maxPho < pho.phoEt->at(i))
	  maxPho = pho.phoEt->at(i);
      }
      if(maxPho < photonEtCut) continue;
    }

    inTree->GetEntry(ientries);
    skimTree->GetEntry(ientries);
    // evtTree->GetEntry(ientries);

    // outJets.event = event;
    // outJets.run = run;
    // outJets.lumi = lumi;
    // outJets.hiBin = hiBin;
    {
      inPhoTree->GetEntry(ientries);
      outJets.event = pho.event;
      outJets.run = pho.run;
      outJets.lumi = pho.lumis;
    }
    outJets.HBHENoiseFilterResult = HBHENoiseFilterResult;
    outJets.pcollisionEventSelection = pcollisionEventSelection;

    outJets.b = inJets.b;
    outJets.nref = inJets.nref;
    outJets.beamId1 = inJets.beamId1;
    outJets.beamId2 = inJets.beamId2;
    outJets.pthat = inJets.pthat;
    for(int i = 0; i < inJets.nref; i++)
    {
      outJets.rawpt[i] = inJets.rawpt[i];
      outJets.jtpt[i] = inJets.jtpt[i];
      outJets.jteta[i] = inJets.jteta[i];
      outJets.jty[i] = inJets.jty[i];
      outJets.jtphi[i] = inJets.jtphi[i];
      outJets.jtpu[i] = inJets.jtpu[i];
      outJets.jtm[i] = inJets.jtm[i];
      outJets.discr_fr01[i] = inJets.discr_fr01[i];
      outJets.trackMax[i] = inJets.trackMax[i];
      outJets.trackSum[i] = inJets.trackSum[i];
      outJets.trackN[i] = inJets.trackN[i];
      outJets.trackHardSum[i] = inJets.trackHardSum[i];
      outJets.trackHardN[i] = inJets.trackHardN[i];
      outJets.chargedMax[i] = inJets.chargedMax[i];
      outJets.chargedSum[i] = inJets.chargedSum[i];
      outJets.chargedN[i] = inJets.chargedN[i];
      outJets.chargedHardSum[i] = inJets.chargedHardSum[i];
      outJets.chargedHardN[i] = inJets.chargedHardN[i];
      outJets.photonMax[i] = inJets.photonMax[i];
      outJets.photonSum[i] = inJets.photonSum[i];
      outJets.photonN[i] = inJets.photonN[i];
      outJets.photonHardSum[i] = inJets.photonHardSum[i];
      outJets.photonHardN[i] = inJets.photonHardN[i];
      outJets.neutralMax[i] = inJets.neutralMax[i];
      outJets.neutralSum[i] = inJets.neutralSum[i];
      outJets.neutralN[i] = inJets.neutralN[i];
      outJets.hcalSum[i] = inJets.hcalSum[i];
      outJets.ecalSum[i] = inJets.ecalSum[i];
      outJets.eMax[i] = inJets.eMax[i];
      outJets.eSum[i] = inJets.eSum[i];
      outJets.eN[i] = inJets.eN[i];
      outJets.muMax[i] = inJets.muMax[i];
      outJets.muSum[i] = inJets.muSum[i];
      outJets.muN[i] = inJets.muN[i];
      outJets.matchedPt[i] = inJets.matchedPt[i];
      outJets.matchedR[i] = inJets.matchedR[i];
      outJets.refpt[i] = inJets.refpt[i];
      outJets.refeta[i] = inJets.refeta[i];
      outJets.refy[i] = inJets.refy[i];
      outJets.refphi[i] = inJets.refphi[i];
      outJets.refdphijt[i] = inJets.refdphijt[i];
      outJets.refdrjt[i] = inJets.refdrjt[i];
      outJets.refparton_pt[i] = inJets.refparton_pt[i];
      outJets.refparton_flavor[i] = inJets.refparton_flavor[i];
      outJets.refparton_flavorForB[i] = inJets.refparton_flavorForB[i];
      outJets.genChargedSum[i] = inJets.genChargedSum[i];
      outJets.genHardSum[i] = inJets.genHardSum[i];
      outJets.signalChargedSum[i] = inJets.signalChargedSum[i];
      outJets.signalHardSum[i] = inJets.signalHardSum[i];
      outJets.subid[i] = inJets.subid[i];
      outJets.fr01Chg[i] = inJets.fr01Chg[i];
      outJets.fr01EM[i] = inJets.fr01EM[i];
      outJets.fr01[i] = inJets.fr01[i];
    }

    outJets.ngen = inJets.ngen;
    for(int i = 0; i < inJets.ngen; ++i)
    {
      outJets.genmatchindex[i] = inJets.genmatchindex[i];
      outJets.genpt[i] = inJets.genpt[i];
      outJets.geneta[i] = inJets.geneta[i];
      outJets.geny[i] = inJets.geny[i];
      outJets.genphi[i] = inJets.genphi[i];
      outJets.gendphijt[i] = inJets.gendphijt[i];
      outJets.gendrjt[i] = inJets.gendrjt[i];
      outJets.gensubid[i] = inJets.gensubid[i];
      outJets.smpt[i] = inJets.smpt[i];
    }

    outTree->Fill();
  }

  outFile->cd();
  configTree->Write();
  outTree->Write();
  outFile->Close();

}

int main(int argc, char **argv)
{
  if(argc == 4)
  {
    jetSkim(argv[1], argv[2], argv[3]);
    return 0;
  } else {
    std::cout << "Usage : \n" <<
      "./jetSkim.exe <configFile> <inputHiForest> <outputSkimFile>"
	      << std::endl;
    return 1;
  }
}
