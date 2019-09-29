#ifndef TRKEFF2018PBPB
#define TRKEFF2018PBPB

#include "TFile.h"
#include "TH2D.h"
#include "TH3F.h"
#include "TMath.h"
#include <iostream>
#include <string>

class TrkEff2018PbPb{
public:

  TrkEff2018PbPb( std::string collectionName = "general", bool isQuiet_ = false ,std::string filePath = "");
  ~TrkEff2018PbPb();

  float getCorrection(float pt, float eta, int hiBin);
  float getEfficiency( float pt, float eta, int hiBin, bool passesCheck = false);
  float getFake( float pt, float eta, int hiBin, bool passesCheck = false);

private:

  inline bool checkBounds(float pt, float eta, int hiBin);

  std::string mode;
  bool isQuiet;

  TFile * trkEff;
  TFile * trkFake;
  TH3F * eff;
  TH3F * fake;

  TH2D * effPix[5];

};

inline bool TrkEff2018PbPb::checkBounds(float pt, float eta, int hiBin){
  if( TMath::Abs(eta) > 2.4 ){
    if( ! isQuiet) std::cout << "TrkEff2018PbPb: track outside |eta|<2.4, please apply this cut!  I am returning a correction factor of 0 for this track for now." << std::endl;
    return false;
  }
  
  if( hiBin <0 || hiBin > 199){
    if( ! isQuiet) std::cout << "TrkEff2018PbPb: hiBin is outside the range [0,199].  Please fix this issue!  I am returning a correction factor of 0 for this track for now." << std::endl;
    return false;
  }
  
  if( pt< 0 || pt > 500 ){
    if( ! isQuiet) std::cout << "TrkEff2018PbPb: pT is outside the range [0,500].  I am returning a correction factor of 0 for this track for now." << std::endl;
    return false;
  }

  return true;
}

float TrkEff2018PbPb::getCorrection(float pt, float eta, int hiBin){
  if( !checkBounds(pt, eta, hiBin) ) return 0;

  float efficiency = getEfficiency(pt, eta, hiBin, true);
  float fake = getFake(pt, eta, hiBin, true);

  //protect against dividing by 0
  if(efficiency > 0.001){
    return (1-fake)/efficiency;
  } else {
    if( ! isQuiet ) std::cout << "TrkEff2018PbPb: Warning! Tracking efficiency is very low for this track (close to dividing by 0).  Returning correction factor of 0 for this track for now." << std::endl;
    return 0;
  }
}

float TrkEff2018PbPb::getEfficiency( float pt, float eta, int hiBin, bool passesCheck){
  if( !passesCheck){
    if(  !checkBounds(pt, eta, hiBin) ) return 0;
  }

  if( mode.compare("general") == 0){
    return eff->GetBinContent( eff->FindBin(eta, pt, hiBin) );
  }

  if( mode.compare("pixel") == 0){
    //pixel tracks above 10 GeV not supported, assume they are the same as 10 GeV tracks
    if(pt > 10) pt = 9.99;

    if(hiBin >= 0 && hiBin<10) return effPix[0]->GetBinContent(effPix[0]->FindBin(eta,pt));
    if(hiBin >= 10 && hiBin<20) return effPix[1]->GetBinContent(effPix[1]->FindBin(eta,pt));
    if(hiBin >= 20 && hiBin<60) return effPix[2]->GetBinContent(effPix[2]->FindBin(eta,pt));
    if(hiBin >= 60 && hiBin<100) return effPix[3]->GetBinContent(effPix[3]->FindBin(eta,pt));
    if(hiBin >= 100 && hiBin<200) return effPix[4]->GetBinContent(effPix[4]->FindBin(eta,pt));

  }

  return 0;
}

float TrkEff2018PbPb::getFake( float pt, float eta, int hiBin, bool passesCheck){
  if( !passesCheck){
    if(  !checkBounds(pt, eta, hiBin) ) return 0;
  }

  if( mode.compare("general") == 0){
    return fake->GetBinContent( fake->FindBin(eta, pt, hiBin) );
  }
  
  if( mode.compare("pixel") == 0){
    if(pt > 10) pt = 9.99;
    
    return 0.0;
  }

  return 0;
}


TrkEff2018PbPb::TrkEff2018PbPb(std::string collectionName, bool isQuiet_, std::string filePath){
  isQuiet = isQuiet_;
  mode = collectionName;
  if( collectionName.compare("general") == 0 ){
    if(!isQuiet) std::cout << "TrkEff2018PbPb class opening in general tracks mode!" << std::endl;
    
    trkEff = TFile::Open( (filePath + "2018PbPb_Efficiency_GeneralTracks_highPt.root").c_str(),"open");
    
    if( !(trkEff->IsOpen() ) ){
      std::cout << "WARNING, COULD NOT FIND TRACK EFFICIENCY FILE FOR GENERAL TRACKS!" << std::endl;
    } else {
      eff = (TH3F*) trkEff->Get("Eff3D");
    }

    trkFake = TFile::Open( (filePath + "2018PbPb_Efficiency_GeneralTracks_MB.root").c_str(),"open");

    if( !(trkFake->IsOpen() ) ){
      std::cout << "WARNING, COULD NOT FIND TRACK FAKE FILE FOR GENERAL TRACKS!" << std::endl;
    } else {
      fake = (TH3F*) trkFake->Get("Fak3D");
    }


  } else if( collectionName.compare("pixel") == 0) {
    if(!isQuiet) std::cout << "TrkEff2018PbPb class opening in pixel tracks mode!" << std::endl;
    
    trkEff = TFile::Open( (filePath + "2018PbPb_Efficiency_PixelTracks.root").c_str(),"open");
    
    if( !(trkEff->IsOpen() ) ){
      std::cout << "WARNING, COULD NOT FIND TRACK EFFICIENCY FILE FOR PIXEL TRACKS!" << std::endl;
    } else {
      effPix[0] = (TH2D*) trkEff->Get("Eff_0_5");
      effPix[1] = (TH2D*) trkEff->Get("Eff_5_10");
      effPix[2] = (TH2D*) trkEff->Get("Eff_10_30");
      effPix[3] = (TH2D*) trkEff->Get("Eff_30_50");
      effPix[4] = (TH2D*) trkEff->Get("Eff_50_100");
    }
    

  } else {
    std::cout << "Error! incorrect collectionName parameter in TrkEff2018PbPb constructor.  Please try 'general' or 'pixel.'" << std::endl;
  } 
}

TrkEff2018PbPb::~TrkEff2018PbPb(){
  trkEff->Close();
  trkFake->Close();
}

#endif
