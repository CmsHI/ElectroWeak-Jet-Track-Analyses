#include <TTree.h>
#include <TFile.h>
#include <TBranch.h>
#include "../CutConfigurations/CutConfigurationsParser.h"

TTree* setupConfigurationTreeForWriting(CutConfiguration config)
{
  TTree *t = new TTree("Configuration","Cut Configurations");

  for(int i = 0; i < CUTS::kN_PROCESSES; ++i)
  {
    for(int j = 0; j < CUTS::kN_OBJECTS; ++j)
    {
      for(int k = 0; k < CUTS::SUMMARY_INFO_I[j]; ++k){
	std::string branchname = (CUTS::PROCESS_LABELS[i] + "." +
				  CUTS::OBJECT_LABELS[j] + "." +
				  CUTS::SUMMARY_INFO_I_LABELS[j][k]);
	t->Branch(branchname.c_str(),&config.proc[i].obj[j].i[k],(branchname + "/I").c_str());
      }
      for(int k = 0; k < CUTS::SUMMARY_INFO_F[j]; ++k){
	std::string branchname = (CUTS::PROCESS_LABELS[i] + "." +
				  CUTS::OBJECT_LABELS[j] + "." +
				  CUTS::SUMMARY_INFO_F_LABELS[j][k]);
	t->Branch(branchname.c_str(),&config.proc[i].obj[j].f[k],(branchname + "/F").c_str());
      }
      for(int k = 0; k < CUTS::SUMMARY_INFO_B[j]; ++k){
	std::string branchname = (CUTS::PROCESS_LABELS[i] + "." +
				  CUTS::OBJECT_LABELS[j] + "." +
				  CUTS::SUMMARY_INFO_B_LABELS[j][k]);
	t->Branch(branchname.c_str(),&config.proc[i].obj[j].b[k],(branchname + "/B").c_str());
      }
    }
  }

  t->Fill();
  return t;
}
