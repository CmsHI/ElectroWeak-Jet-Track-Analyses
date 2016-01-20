#ifndef CUTCONFIGURATION
#define CUTCONFIGURATION

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>

#include "../../CutConfigurations/interface/PhotonCuts.h"
#include "../../CutConfigurations/interface/ElectronCuts.h"
#include "../../CutConfigurations/interface/ZBosonCuts.h"
#include "../../CutConfigurations/interface/JetCuts.h"
#include "../../CutConfigurations/interface/TrackCuts.h"
#include "../../CutConfigurations/interface/GammaJetCuts.h"
#include "../../CutConfigurations/interface/ZJetCuts.h"
#include "../../CutConfigurations/interface/EventCuts.h"

#include "Configuration.h"
#include "ConfigurationParser.h"

#include "../systemUtil.h"

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
    kGAMMAJET,
    kZJET,
    kEVENT,
    kN_OBJECTS
  };

  const int SUMMARY_INFO_I[kN_OBJECTS] = {PHO::kN_I_CUTS,
					  ELE::kN_I_CUTS,
					  ZBO::kN_I_CUTS,
					  JET::kN_I_CUTS,
					  TRK::kN_I_CUTS,
					  GJT::kN_I_CUTS,
					  ZJT::kN_I_CUTS,
                      EVT::kN_I_CUTS};
  const std::string *SUMMARY_INFO_I_LABELS[kN_OBJECTS] = {PHO::I_CUTS_LABELS,
							  ELE::I_CUTS_LABELS,
							  ZBO::I_CUTS_LABELS,
							  JET::I_CUTS_LABELS,
							  TRK::I_CUTS_LABELS,
							  GJT::I_CUTS_LABELS,
							  ZJT::I_CUTS_LABELS,
							  EVT::I_CUTS_LABELS};

  const int SUMMARY_INFO_F[kN_OBJECTS] = {PHO::kN_F_CUTS,
					  ELE::kN_F_CUTS,
					  ZBO::kN_F_CUTS,
					  JET::kN_F_CUTS,
					  TRK::kN_F_CUTS,
					  GJT::kN_F_CUTS,
					  ZJT::kN_F_CUTS,
					  EVT::kN_F_CUTS};
  const std::string *SUMMARY_INFO_F_LABELS[kN_OBJECTS] = {PHO::F_CUTS_LABELS,
							  ELE::F_CUTS_LABELS,
							  ZBO::F_CUTS_LABELS,
							  JET::F_CUTS_LABELS,
							  TRK::F_CUTS_LABELS,
							  GJT::F_CUTS_LABELS,
							  ZJT::F_CUTS_LABELS,
							  EVT::F_CUTS_LABELS};

  const int SUMMARY_INFO_S[kN_OBJECTS] = {PHO::kN_S_CUTS,
					  ELE::kN_S_CUTS,
					  ZBO::kN_S_CUTS,
					  JET::kN_S_CUTS,
					  TRK::kN_S_CUTS,
					  GJT::kN_S_CUTS,
					  ZJT::kN_S_CUTS,
					  EVT::kN_S_CUTS};
  const std::string *SUMMARY_INFO_S_LABELS[kN_OBJECTS] = {PHO::S_CUTS_LABELS,
							  ELE::S_CUTS_LABELS,
							  ZBO::S_CUTS_LABELS,
							  JET::S_CUTS_LABELS,
							  TRK::S_CUTS_LABELS,
							  GJT::S_CUTS_LABELS,
							  ZJT::S_CUTS_LABELS,
							  EVT::S_CUTS_LABELS};

  std::string OBJECT_LABELS[kN_OBJECTS] = {"photon",
					   "electron",
					   "zboson",
					   "jet",
					   "track",
					   "gammajet",
					   "zjet",
					   "event"
  };

  struct ProcessCuts{
    ObjectCuts obj[kN_OBJECTS];
  };

  enum PROCESS{
    kSKIM,
    kCORRECTION,
    kHISTOGRAM,
    kPLOTTING,
    kPERFORMANCE,
    kN_PROCESSES // must come last in enum
  };

  std::string PROCESS_LABELS[kN_PROCESSES] = {"skim",
					      "correction",
					      "histogram",
					      "plotting",
					      "performance"
  };
};

struct CutConfiguration : public Configuration{
  CUTS::ProcessCuts proc[CUTS::kN_PROCESSES];

};

class CutConfigurationParser : public ConfigurationParser{
public:
   CutConfigurationParser(){};
   ~CutConfigurationParser(){};

  static CutConfiguration Parse(std::string inFile);
};


CutConfiguration CutConfigurationParser::Parse(std::string inFile)
{
  using namespace CUTS;
  std::string endSignal = "#CUTS-END#";     // signals that cut configuration parsing is to be terminated.
                                            // another block of configuration parsing will start.

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
    config.isValid = false;
    return config;
  }
  std::string line;
  unsigned int lineCounter = 0;
  while (getline(fin, line)) {
    lineCounter++;
    if (line.find(endSignal) != std::string::npos) break;
    if (line.find("=") == std::string::npos) continue; //skip all lines without an =
    if (line.find(".") == std::string::npos) continue; //skip all lines without a dot
    if (trim(line).find_first_of(CONFIGPARSER::comment.c_str()) == 0) continue;  //skip all lines starting with comment sign #
    size_t pos = line.find_first_of("=") + 1;
    size_t posLast = line.find_first_of(CONFIGPARSER::comment.c_str());    // allow inline comment signs with #
    std::string value = ConfigurationParser::ReadValue(fin, line.substr(pos, (posLast-pos)));   // read value over multiple lines if necessary
    std::istringstream sin(value);
    line = line.substr(0, pos-1);        // "line" becomes the LHS of the "=" sign (excluing the "=" sign)
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
      std::string label = Form(".%s.",OBJECT_LABELS[i].c_str());    // prevent substring matching, e.g. : "jet" and "gammajet"
      if(line.find(label) != std::string::npos)
      {
	obj = (OBJECT)i;

	for(int j = 0; j < SUMMARY_INFO_I[obj]; ++j)
	{
	  std::string label_I = Form(".%s ",SUMMARY_INFO_I_LABELS[obj][j].c_str());    // prevent substring matching, e.g. : "et" and "trigger_gammaJet", "doPP" and "doPPMC"
	  if(line.find(label_I) != std::string::npos) {
	    int in;
	    sin >> in;
	    config.proc[proc].obj[obj].i[j] = in;
	    success = true;
	    break;
	  }
	}
	for(int j = 0; j < SUMMARY_INFO_F[obj]; ++j)
	{
	  std::string label_F = Form(".%s ",SUMMARY_INFO_F_LABELS[obj][j].c_str());    // prevent substring matching, e.g. : "et" and "trigger_gammaJet", "doPP" and "doPPMC"
	  if(line.find(label_F) != std::string::npos) {
	    float in;
	    sin >> in;
	    config.proc[proc].obj[obj].f[j] = in;
	    success = true;
	    break;
	  }
	}
	for(int j = 0; j < SUMMARY_INFO_S[obj]; ++j)
	{
	  std::string label_S = Form(".%s ",SUMMARY_INFO_S_LABELS[obj][j].c_str());    // prevent substring matching, e.g. : "et" and "trigger_gammaJet", "doPP" and "doPPMC"
	  if(line.find(label_S) != std::string::npos) {
	    std::string in;
	    in = trim(sin.str());   // stringstream ignores characters after a whitespace, use the original string to read the value.
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
      config.isValid = false;
      return config;
    }
  }
  config.isValid = true;
  return config;
}

#endif
