#include <TFile.h>
#include <TString.h>
#include <iostream>
#include "../../TreeHeaders/ggHiNtuplizerTree.h"
#include "../../CutConfigurations/CutConfigurationsParser.h"
#include "../../TreeHeaders/CutConfigurationTree.h"

void ggHiNtuplizerSkim(const TString configFile/*, const TString inputHiForest, const TString outputSkimFile*/)
{
  CutConfiguration config = CutConfigurationsParser::Parse(configFile.Data());

  std::cout << config.proc[CUTS::kSKIM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_et] << std::endl;

  TTree *configTree = setupConfigurationTreeForWriting(config);

  TFile *outFile = TFile::Open("outfile.root","RECREATE");
  configTree->Write();
  outFile->Close();

}

int main(int argc, char **argv)
{
  if(argc == 2)
  {
    ggHiNtuplizerSkim(argv[1]/*, argv[2], argv[3]*/);
    return 0;
  } else {
    std::cout << "Usage : \n" <<
      "./ggHiNtuplizerSkim.exe <configFile> <inputHiForest> <outputSkimFile>"
	      << std::endl;
    return 1;
  }
}
