#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#ifndef CUTCONFIGURATION
#define CUTCONFIGURATION

struct PhotonCuts{
  double eta;
  double et;
  double sigmaIEtaIEta_2012;
};

struct ElectronCuts{
  double eta;
};

struct ZBosonCuts{
  double mass;
};

struct JetCuts{
  double pt;
};

struct TrackCuts{
  bool highPurity;
};

struct ProcessCuts{
  PhotonCuts photon;
  ElectronCuts electron;
  ZBosonCuts zboson;
  JetCuts jet;
  TrackCuts track;
};

struct CutConfiguration{
  ProcessCuts skim;
  ProcessCuts correction;
  ProcessCuts histogram;
  ProcessCuts plotting;
};


class CutConfigurationsParser{
public:
  static CutConfiguration Parse(std::string inFile);
};


CutConfiguration CutConfigurationsParser::Parse(std::string inFile)
{
  CutConfiguration config;

  std::ifstream fin(inFile);
  std::string line;
  while (getline(fin, line)) {
    std::istringstream sin(line.substr(line.find("=") + 1));
    if (line.find("#") != std::string::npos) continue; //allow # comments
    if (line.find("skim") != std::string::npos){
      if(line.find("photon") != std::string::npos){
	if(line.find("eta") != std::string::npos)
	  sin >> config.skim.photon.eta;
	else if(line.find("et") != std::string::npos)
	  sin >> config.skim.photon.et;
	else if(line.find("sigmaIEtaIEta_2012") != std::string::npos)
	  sin >> config.skim.photon.sigmaIEtaIEta_2012;
      } else if (line.find("electron") != std::string::npos){
	if(line.find("eta") != std::string::npos)
	  sin >> config.skim.electron.eta;
      } else if (line.find("zboson") != std::string::npos){
	if(line.find("mass") != std::string::npos)
	  sin >> config.skim.zboson.mass;
      } else if (line.find("jet") != std::string::npos){
	if(line.find("pt") != std::string::npos)
	  sin >> config.skim.jet.pt;
      } else if (line.find("track") != std::string::npos){
	if(line.find("highPurity") != std::string::npos)
	  sin >> config.skim.track.highPurity;
      }
    }
    else if (line.find("correction") != std::string::npos){
      if(line.find("photon") != std::string::npos){
	if(line.find("eta") != std::string::npos)
	  sin >> config.correction.photon.eta;
	else if(line.find("et") != std::string::npos)
	  sin >> config.correction.photon.et;
	else if(line.find("sigmaIEtaIEta_2012") != std::string::npos)
	  sin >> config.correction.photon.sigmaIEtaIEta_2012;
      } else if (line.find("electron") != std::string::npos){
	if(line.find("eta") != std::string::npos)
	  sin >> config.correction.electron.eta;
      } else if (line.find("zboson") != std::string::npos){
	if(line.find("mass") != std::string::npos)
	  sin >> config.correction.zboson.mass;
      } else if (line.find("jet") != std::string::npos){
	if(line.find("pt") != std::string::npos)
	  sin >> config.correction.jet.pt;
      } else if (line.find("track") != std::string::npos){
	if(line.find("highPurity") != std::string::npos)
	  sin >> config.correction.track.highPurity;
      }
    }
    else if (line.find("histogram") != std::string::npos){
      if(line.find("photon") != std::string::npos){
	if(line.find("eta") != std::string::npos)
	  sin >> config.histogram.photon.eta;
	else if(line.find("et") != std::string::npos)
	  sin >> config.histogram.photon.et;
	else if(line.find("sigmaIEtaIEta_2012") != std::string::npos)
	  sin >> config.histogram.photon.sigmaIEtaIEta_2012;
      } else if (line.find("electron") != std::string::npos){
	if(line.find("eta") != std::string::npos)
	  sin >> config.histogram.electron.eta;
      } else if (line.find("zboson") != std::string::npos){
	if(line.find("mass") != std::string::npos)
	  sin >> config.histogram.zboson.mass;
      } else if (line.find("jet") != std::string::npos){
	if(line.find("pt") != std::string::npos)
	  sin >> config.histogram.jet.pt;
      } else if (line.find("track") != std::string::npos){
	if(line.find("highPurity") != std::string::npos)
	  sin >> config.histogram.track.highPurity;
      }
    }
    else if (line.find("plotting") != std::string::npos){
      if(line.find("photon") != std::string::npos){
	if(line.find("eta") != std::string::npos)
	  sin >> config.plotting.photon.eta;
	else if(line.find("et") != std::string::npos)
	  sin >> config.plotting.photon.et;
	else if(line.find("sigmaIEtaIEta_2012") != std::string::npos)
	  sin >> config.plotting.photon.sigmaIEtaIEta_2012;
      } else if (line.find("electron") != std::string::npos){
	if(line.find("eta") != std::string::npos)
	  sin >> config.plotting.electron.eta;
      } else if (line.find("zboson") != std::string::npos){
	if(line.find("mass") != std::string::npos)
	  sin >> config.plotting.zboson.mass;
      } else if (line.find("jet") != std::string::npos){
	if(line.find("pt") != std::string::npos)
	  sin >> config.plotting.jet.pt;
      } else if (line.find("track") != std::string::npos){
	if(line.find("highPurity") != std::string::npos)
	  sin >> config.plotting.track.highPurity;
      }
    }
  }
  return config;
}

#endif
