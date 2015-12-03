#ifndef CUTCONFIGURATION
#define CUTCONFIGURATION

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>

#include "PhotonCuts.h"
#include "ElectronCuts.h"
#include "ZBosonCuts.h"
#include "JetCuts.h"
#include "TrackCuts.h"

namespace CUTS {
  struct ObjectCuts{
    std::vector<float> f;
    std::vector<int> i;
    std::vector<std::string> s;

    std::vector<char*> c; // this is a c-string copy of s
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

  const int SUMMARY_INFO_S[kN_OBJECTS] = {PHO::kN_S_CUTS,
					  ELE::kN_S_CUTS,
					  ZBO::kN_S_CUTS,
					  JET::kN_S_CUTS,
					  TRK::kN_S_CUTS};
  const std::string *SUMMARY_INFO_S_LABELS[kN_OBJECTS] = {PHO::S_CUTS_LABELS,
							  ELE::S_CUTS_LABELS,
							  ZBO::S_CUTS_LABELS,
							  JET::S_CUTS_LABELS,
							  TRK::S_CUTS_LABELS};


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
      config.proc[i].obj[j].i.resize(SUMMARY_INFO_I[j]);
      config.proc[i].obj[j].f.resize(SUMMARY_INFO_F[j]);
      config.proc[i].obj[j].s.resize(SUMMARY_INFO_S[j]);
      config.proc[i].obj[j].c.resize(SUMMARY_INFO_S[j]);
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
	    int in;
	    sin >> in;
	    config.proc[proc].obj[obj].i[j] = in;
	    success = true;
	    break;
	  }
	}
	for(int j = 0; j < SUMMARY_INFO_F[obj]; ++j)
	{
	  if(line.find(SUMMARY_INFO_F_LABELS[obj][j]) != std::string::npos) {
	    float in;
	    sin >> in;
	    config.proc[proc].obj[obj].f[j] = in;
	    success = true;
	    break;
	  }
	}
	for(int j = 0; j < SUMMARY_INFO_S[obj]; ++j)
	{
	  if(line.find(SUMMARY_INFO_S_LABELS[obj][j]) != std::string::npos) {
	    std::string in;
	    sin >> in;
	    config.proc[proc].obj[obj].s[j] = in;
	    char * cstr = new char [in.length()+1];
	    std::strcpy (cstr, in.c_str());
	    config.proc[proc].obj[obj].c[j] = cstr;
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
