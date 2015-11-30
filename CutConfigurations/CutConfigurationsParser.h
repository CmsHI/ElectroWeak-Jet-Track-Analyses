#ifndef CUTCONFIGURATION
#define CUTCONFIGURATION

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "PhotonCuts.h"
#include "ElectronCuts.h"
#include "ZBosonCuts.h"
#include "JetCuts.h"
#include "TrackCuts.h"

namespace CUTS {
  struct ObjectCuts{
    float *f;
    int *i;
    bool *b;
  };

  enum OBJECT{
    kPHOTON,
    kELECTRON,
    kZBOSON,
    kJET,
    kTRACK,
    kN_OBJECTS
  };

  const int SUMMARY_INFO_I[kN_OBJECTS] = {PHO::kN_I_CUTS,
					  ELE::kN_I_CUTS,
					  ZBO::kN_I_CUTS,
					  JET::kN_I_CUTS,
					  TRK::kN_I_CUTS};
  const std::string *SUMMARY_INFO_I_LABELS[kN_OBJECTS] = {PHO::I_CUTS_LABELS,
							  ELE::I_CUTS_LABELS,
							  ZBO::I_CUTS_LABELS,
							  JET::I_CUTS_LABELS,
							  TRK::I_CUTS_LABELS};

  const int SUMMARY_INFO_F[kN_OBJECTS] = {PHO::kN_F_CUTS,
					  ELE::kN_F_CUTS,
					  ZBO::kN_F_CUTS,
					  JET::kN_F_CUTS,
					  TRK::kN_F_CUTS};
  const std::string *SUMMARY_INFO_F_LABELS[kN_OBJECTS] = {PHO::F_CUTS_LABELS,
							  ELE::F_CUTS_LABELS,
							  ZBO::F_CUTS_LABELS,
							  JET::F_CUTS_LABELS,
							  TRK::F_CUTS_LABELS};

  const int SUMMARY_INFO_B[kN_OBJECTS] = {PHO::kN_B_CUTS,
					  ELE::kN_B_CUTS,
					  ZBO::kN_B_CUTS,
					  JET::kN_B_CUTS,
					  TRK::kN_B_CUTS};
  const std::string *SUMMARY_INFO_B_LABELS[kN_OBJECTS] = {PHO::B_CUTS_LABELS,
							  ELE::B_CUTS_LABELS,
							  ZBO::B_CUTS_LABELS,
							  JET::B_CUTS_LABELS,
							  TRK::B_CUTS_LABELS};


  std::string OBJECT_LABELS[kN_OBJECTS] = {"photon",
					   "electron",
					   "zboson",
					   "jet",
					   "track"
  };

  struct ProcessCuts{
    ObjectCuts obj[kN_OBJECTS];
  };

  enum PROCESS{
    kSKIM,
    kCORRECTION,
    kHISTOGRAM,
    kPLOTTING,
    kN_PROCESSES // must come last in enum
  };

  std::string PROCESS_LABELS[kN_PROCESSES] = {"skim",
					      "correction",
					      "histogram",
					      "plotting"
  };
};

struct CutConfiguration{
  CUTS::ProcessCuts proc[CUTS::kN_PROCESSES];
};

class CutConfigurationsParser{
public:
  static CutConfiguration Parse(std::string inFile);
};


CutConfiguration CutConfigurationsParser::Parse(std::string inFile)
{
  using namespace CUTS;

  CutConfiguration config;
  for(int i = 0 ; i < kN_PROCESSES; ++i){
    for(int j = 0; j < kN_OBJECTS; ++j){
      config.proc[i].obj[j].i = (int*) calloc( SUMMARY_INFO_I[j], sizeof(int));
      config.proc[i].obj[j].f = (float*) calloc( SUMMARY_INFO_F[j], sizeof(float));
      config.proc[i].obj[j].b = (bool*) calloc( SUMMARY_INFO_B[j], sizeof(bool));
    }
  }

  std::ifstream fin(inFile);
  if ( (fin.rdstate() & std::ifstream::failbit ) != 0 ) {
    std::cout << "I/O Error opening configuration file." << std::endl;
    return config;
  }
  std::string line;
  unsigned int lineCounter = 0;
  while (getline(fin, line)) {
    lineCounter++;
    if (line.find("#") != std::string::npos) continue; //allow # comments
    if (line.find("=") == std::string::npos) continue; //skip all lines without an =
    std::istringstream sin(line.substr(line.find("=") + 1));
    bool success = false;
    PROCESS proc = kN_PROCESSES;
    for(int i = 0; i < kN_PROCESSES; ++i){
      if (line.find(PROCESS_LABELS[i]) != std::string::npos) {
	proc = (PROCESS)i;
	break;
      }
    }
    OBJECT obj = kN_OBJECTS;
    for(int i = 0; i < kN_OBJECTS; ++i){
      if(line.find(OBJECT_LABELS[i]) != std::string::npos)
      {
	obj = (OBJECT)i;

	for(int j = 0; j < SUMMARY_INFO_I[obj]; ++j)
	{
	  if(line.find(SUMMARY_INFO_I_LABELS[obj][j]) != std::string::npos) {
	    sin >> config.proc[proc].obj[obj].i[j];
	    success = true;
	    break;
	  }
	}
	for(int j = 0; j < SUMMARY_INFO_F[obj]; ++j)
	{
	  if(line.find(SUMMARY_INFO_F_LABELS[obj][j]) != std::string::npos) {
	    sin >> config.proc[proc].obj[obj].f[j];
	    success = true;
	    break;
	  }
	}
	for(int j = 0; j < SUMMARY_INFO_B[obj]; ++j)
	{
	  if(line.find(SUMMARY_INFO_B_LABELS[obj][j]) != std::string::npos) {
	    sin >> config.proc[proc].obj[obj].b[j];
	    success = true;
	    break;
	  }
	}
	break;
      }
    }
    if (!success) {
      std::cout << "Malformed line in configuration." << std::endl;
      std::cout << "Line #" << lineCounter << " : " << std::endl;
      std::cout << line << std::endl;
      return config;
    }
  }
  return config;
}

#endif
