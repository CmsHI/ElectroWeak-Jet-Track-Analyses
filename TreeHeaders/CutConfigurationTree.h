#include <TTree.h>
#include <TFile.h>
#include <TBranch.h>
#include "../CutConfigurations/CutConfigurationsParser.h"

TTree* setupConfigurationTreeForWriting(CutConfiguration config)
{
  TTree *t = new TTree("Configuration","Cut Configurations");

  t->Branch("skim.photon.eta",&config.skim.photon.eta);
  t->Branch("skim.photon.et",&config.skim.photon.et);
  t->Branch("skim.photon.sigmaIEtaIEta_2012",&config.skim.photon.sigmaIEtaIEta_2012);

  t->Branch("skim.electron.eta",&config.skim.electron.eta);

  t->Branch("skim.zboson.mass",&config.skim.zboson.mass);

  t->Branch("skim.jet.pt",&config.skim.jet.pt);

  t->Branch("skim.track.highPurity",&config.skim.track.highPurity);

  t->Branch("correction.photon.eta",&config.correction.photon.eta);
  t->Branch("correction.photon.et",&config.correction.photon.et);
  t->Branch("correction.photon.sigmaIEtaIEta_2012",&config.correction.photon.sigmaIEtaIEta_2012);

  t->Branch("correction.electron.eta",&config.correction.electron.eta);

  t->Branch("correction.zboson.mass",&config.correction.zboson.mass);

  t->Branch("correction.jet.pt",&config.correction.jet.pt);

  t->Branch("correction.track.highPurity",&config.correction.track.highPurity);

  t->Branch("histogram.photon.eta",&config.histogram.photon.eta);
  t->Branch("histogram.photon.et",&config.histogram.photon.et);
  t->Branch("histogram.photon.sigmaIEtaIEta_2012",&config.histogram.photon.sigmaIEtaIEta_2012);

  t->Branch("histogram.electron.eta",&config.histogram.electron.eta);

  t->Branch("histogram.zboson.mass",&config.histogram.zboson.mass);

  t->Branch("histogram.jet.pt",&config.histogram.jet.pt);

  t->Branch("histogram.track.highPurity",&config.histogram.track.highPurity);

  t->Branch("plotting.photon.eta",&config.plotting.photon.eta);
  t->Branch("plotting.photon.et",&config.plotting.photon.et);
  t->Branch("plotting.photon.sigmaIEtaIEta_2012",&config.plotting.photon.sigmaIEtaIEta_2012);

  t->Branch("plotting.electron.eta",&config.plotting.electron.eta);

  t->Branch("plotting.zboson.mass",&config.plotting.zboson.mass);

  t->Branch("plotting.jet.pt",&config.plotting.jet.pt);

  t->Branch("plotting.track.highPurity",&config.plotting.track.highPurity);

  t->Fill();
  return t;
}
