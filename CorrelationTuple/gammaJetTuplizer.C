#include <TFile.h>
#include <TString.h>
#include <iostream>

#include "../TreeHeaders/CutConfigurationTree.h"
#include "../Utilities/interface/CutConfigurationParser.h"
//#include "../TreeHeaders/ggHiNtuplizerPhotonSkim.h"
//#include "../TreeHeaders/SetupJetTree.h"

void photonCorrections(const TString configFile, const TString inputPhoSkimFile,
		       const TString inputJetSkimFile, const TString outputSkimFile)
{
  CutConfiguration config = CutConfigurationParser::Parse(configFile.Data());
  TTree *configTree = setupConfigurationTreeForWriting(config);

  TFile *inPhoSkimFile = TFile::Open(inputPhoSkimFile);
  TTree *inPhoTree = (TTree*)inPhoSkimFile->Get("photonSkimTree");
  //PhotonSkim inPho;
  //inPho.setupTreeForReading(inPhoTree);
  inPhoTree->SetBranchStatus("*",1);

  TFile *inJetSkimFile = TFile::Open(inputJetSkimFile);
  TTree *inJetTree = (TTree*)inJetSkimFile->Get("jetSkimTree");
  //Jets inJets;
  //inJets.setupTreeForReading(inJetTree);
  inJetTree->SetBranchStatus("*",1);

  TFile *outFile = TFile::Open(outputSkimFile,"RECREATE");
  TTree *outPhoTree = inPhoTree->CloneTree();
  TTree *outJetTree = inJetTree->CloneTree();

  outFile->cd();
  configTree->Write();
  outPhoTree->Write();
  outJetTree->Write();
  outFile->Close();

}

int main(int argc, char **argv)
{
  if(argc == 5)
  {
    photonCorrections(argv[1], argv[2], argv[3], argv[4]);
    return 0;
  } else {
    std::cout << "Usage : \n" <<
      "./gammaJetTuplizer.exe <configFile> <inputPhoSkimFile> <inputJetSkimFile> <outputSkimFile>"
	      << std::endl;
    return 1;
  }
}
