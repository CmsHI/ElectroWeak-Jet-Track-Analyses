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
#include "GammaJetCuts.h"
#include "ZJetCuts.h"

#include "../../Utilities/systemUtil.h"

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
    kN_OBJECTS
  };

  const int SUMMARY_INFO_I[kN_OBJECTS] = {PHO::kN_I_CUTS,
					  ELE::kN_I_CUTS,
					  ZBO::kN_I_CUTS,
					  JET::kN_I_CUTS,
					  TRK::kN_I_CUTS,
					  GJT::kN_I_CUTS,
					  ZJT::kN_I_CUTS};
  const std::string *SUMMARY_INFO_I_LABELS[kN_OBJECTS] = {PHO::I_CUTS_LABELS,
							  ELE::I_CUTS_LABELS,
							  ZBO::I_CUTS_LABELS,
							  JET::I_CUTS_LABELS,
							  TRK::I_CUTS_LABELS,
							  GJT::I_CUTS_LABELS,
							  ZJT::I_CUTS_LABELS};

  const int SUMMARY_INFO_F[kN_OBJECTS] = {PHO::kN_F_CUTS,
					  ELE::kN_F_CUTS,
					  ZBO::kN_F_CUTS,
					  JET::kN_F_CUTS,
					  TRK::kN_F_CUTS,
					  GJT::kN_F_CUTS,
					  ZJT::kN_F_CUTS};
  const std::string *SUMMARY_INFO_F_LABELS[kN_OBJECTS] = {PHO::F_CUTS_LABELS,
							  ELE::F_CUTS_LABELS,
							  ZBO::F_CUTS_LABELS,
							  JET::F_CUTS_LABELS,
							  TRK::F_CUTS_LABELS,
							  GJT::F_CUTS_LABELS,
							  ZJT::F_CUTS_LABELS};

  const int SUMMARY_INFO_S[kN_OBJECTS] = {PHO::kN_S_CUTS,
					  ELE::kN_S_CUTS,
					  ZBO::kN_S_CUTS,
					  JET::kN_S_CUTS,
					  TRK::kN_S_CUTS,
					  GJT::kN_S_CUTS,
					  ZJT::kN_S_CUTS};
  const std::string *SUMMARY_INFO_S_LABELS[kN_OBJECTS] = {PHO::S_CUTS_LABELS,
							  ELE::S_CUTS_LABELS,
							  ZBO::S_CUTS_LABELS,
							  JET::S_CUTS_LABELS,
							  TRK::S_CUTS_LABELS,
							  GJT::S_CUTS_LABELS,
							  ZJT::S_CUTS_LABELS};

  std::string OBJECT_LABELS[kN_OBJECTS] = {"photon",
					   "electron",
					   "zboson",
					   "jet",
					   "track",
					   "gammajet",
					   "zjet"
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

struct CutConfiguration{
  CUTS::ProcessCuts proc[CUTS::kN_PROCESSES];
  bool isValid;
};

class CutConfigurationsParser{
public:
  static CutConfiguration Parse(std::string inFile);
  static std::vector<std::string> ParseList(std::string strList);
};


CutConfiguration CutConfigurationsParser::Parse(std::string inFile)
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
    if (line.find("#") != std::string::npos) continue; //allow # comments
    if (line.find("=") == std::string::npos) continue; //skip all lines without an =
    std::string value = line.substr(line.find("=") + 1);
    std::istringstream sin(value);
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
	  std::string label_I = Form(".%s",SUMMARY_INFO_I_LABELS[obj][j].c_str());    // prevent substring matching, e.g. : "et" and "trigger_gammaJet"
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
	  std::string label_F = Form(".%s",SUMMARY_INFO_F_LABELS[obj][j].c_str());    // prevent substring matching, e.g. : "et" and "trigger_gammaJet"
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
	  std::string label_S = Form(".%s",SUMMARY_INFO_S_LABELS[obj][j].c_str());    // prevent substring matching, e.g. : "et" and "trigger_gammaJet"
	  if(line.find(label_S) != std::string::npos) {
	    std::string in;
	    in = trim(value);   // stringstream ignores characters after a whitespace, use the original string to read the value.
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

std::vector<std::string> CutConfigurationsParser::ParseList(std::string strList)
{
    std::vector<std::string> list;

    if(strList.empty())
        return list;

    size_t posStart = strList.find("{");     // a valid list starts with '{' and ends with '}'
    if (posStart == std::string::npos) return list;

    size_t posEnd   = strList.find("}");     // a valid list starts with '{' and ends with '}'
    if (posEnd == std::string::npos) return list;

    // elements of the list are separated by ','
    size_t pos;
    bool listFinished = false;
    posStart++;     // exclude '{c'
    while (!listFinished) {

        pos = strList.find(",", posStart);
        if (pos == std::string::npos) {    // this must be the last element. nothing after '}' is accepted.
            pos = posEnd;
            listFinished = true;
        }

        std::string tmp = strList.substr(posStart, pos-posStart);  //  strList = ...,ABC123 ,... --> posStart = 0, pos = 8, tmp = "ABC123 "

        std::string element = trim(tmp);

        list.push_back(element.c_str());
        posStart = pos + 1;
    }

    return list;
}

#endif
