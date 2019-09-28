/*
 * https://github.com/CMS-HIN-dilepton/MuonAnalysis-TagAndProbe/blob/103X_HI/macros/tnp_weight.h
 */
#ifndef muons_2018pbpb_tnp_weight_h
#define muons_2018pbpb_tnp_weight_h

#include "TMath.h"
#include <iostream>

// IN THIS FILE YOU WILL FIND:
// ++++++++++++++
//
// - MuID: (tnp_weight_muid_pbpb)   Preliminary
//   * idx = 0: nominal
//   * idx = -1: syst variation,  +1 sigma
//   * idx = -2: syst variation,  -1 sigma
//   * idx = +1: stat variation,  +1 sigma
//   * idx = +2: stat variation,  -1 sigma
//
// - Trigger: (tnp_weight_trg_pbpb)  Preliminary
//   * idx = 0:  nominal
//   * idx = -1: TnP syst variation,  +1 sigma
//   * idx = -2: TnP syst variation,  -1 sigma
//   * idx = +1: stat variation,  +1 sigma
//   * idx = +2: stat variation,  -1 sigma

// For all:
//   * idx = +200: tnp efficiency for data
//   * idx = +300: tnp efficiency for MC

// THE INDIVIDUAL SFs
// ++++++++++++++++++
double tnp_weight_muid_pbpb(double eta, int idx=0);
double tnp_weight_trig_pbpb(double pt, double eta, double cent, int idx=0);



///////////////////////////////////////////////////
//                 M u I D    P b P b                //
///////////////////////////////////////////////////
double tnp_weight_muid_pbpb(double eta, int idx)
{
   double x = eta;
   double syst = 0.6e-2;  //preliminary

   double num=1,den=1;
   

   // MC
   if (x > -2.4&&x <= -2.1) den = 0.994717;
   if (x > -2.1&&x <= -1.6) den = 0.993653;
   if (x > -1.6&&x <= -1.2) den = 0.982687;
   if (x > -1.2&&x <= -0.9) den = 0.962992;
   if (x > -0.9&&x <= -0.6) den = 0.970103;
   if (x > -0.6&&x <= -0.3) den = 0.982073;
   if (x > -0.3&&x <= 0) den = 0.968219;
   if (x > 0 && x <= 0.3) den = 0.961703;
   if (x > 0.3&&x <= 0.6) den = 0.978947;
   if (x > 0.6&&x <= 0.9) den = 0.970324;
   if (x > 0.9&&x <= 1.2) den = 0.956433;
   if (x > 1.2&&x <= 1.6) den = 0.982577;
   if (x > 1.6&&x <= 2.1) den = 0.995146;
   if (x > 2.1&&x <= 2.4) den = 0.994106;


   // data
   if (idx <= 0 || idx > 10) { // nominal
	   if (x > -2.4&&x <= -2.1) num = 0.983734;
	   if (x > -2.1&&x <= -1.6) num = 0.993892;
	   if (x > -1.6&&x <= -1.2) num = 0.979222;
	   if (x > -1.2&&x <= -0.9) num = 0.955529;
	   if (x > -0.9&&x <= -0.6) num = 0.966578;
	   if (x > -0.6&&x <= -0.3) num = 0.983958;
	   if (x > -0.3&&x <= 0) num = 0.959491;
	   if (x > 0 && x <= 0.3) num = 0.959744;
	   if (x > 0.3&&x <= 0.6) num = 0.976402;
	   if (x > 0.6&&x <= 0.9) num = 0.96839;
	   if (x > 0.9&&x <= 1.2) num = 0.961177;
	   if (x > 1.2&&x <= 1.6) num = 0.98078;
	   if (x > 1.6&&x <= 2.1) num = 0.991852;
	   if (x > 2.1&&x <= 2.4) num = 0.993082;
   }
   else if (idx == 1) { // stat up
	   if (x > -2.4&&x <= -2.1) num = 0.98710825;
	   if (x > -2.1&&x <= -1.6) num = 0.99575701;
	   if (x > -1.6&&x <= -1.2) num = 0.98250043;
	   if (x > -1.2&&x <= -0.9) num = 0.96029298;
	   if (x > -0.9&&x <= -0.6) num = 0.97057735;
	   if (x > -0.6&&x <= -0.3) num = 0.98705592;
	   if (x > -0.3&&x <= 0) num = 0.96370511;
	   if (x > 0 && x <= 0.3) num = 0.96396652;
	   if (x > 0.3&&x <= 0.6) num = 0.97980996;
	   if (x > 0.6&&x <= 0.9) num = 0.97234624;
	   if (x > 0.9&&x <= 1.2) num = 0.9659164;
	   if (x > 1.2&&x <= 1.6) num = 0.98388187;
	   if (x > 1.6&&x <= 2.1) num = 0.99371692;
	   if (x > 2.1&&x <= 2.4) num = 0.9961665;
   }
   else if (idx == 2) { // stat down
	   if (x > -2.4&&x <= -2.1) num = 0.98075691;
	   if (x > -2.1&&x <= -1.6) num = 0.99224693;
	   if (x > -1.6&&x <= -1.2) num = 0.97618383;
	   if (x > -1.2&&x <= -0.9) num = 0.95101985;
	   if (x > -0.9&&x <= -0.6) num = 0.96279449;
	   if (x > -0.6&&x <= -0.3) num = 0.98108946;
	   if (x > -0.3&&x <= 0) num = 0.95550071;
	   if (x > 0 && x <= 0.3) num = 0.95575316;
	   if (x > 0.3&&x <= 0.6) num = 0.97322907;
	   if (x > 0.6&&x <= 0.9) num = 0.96468663;
	   if (x > 0.9&&x <= 1.2) num = 0.9567025;
	   if (x > 1.2&&x <= 1.6) num = 0.9779218;
	   if (x > 1.6&&x <= 2.1) num = 0.99018962;
	   if (x > 2.1&&x <= 2.4) num = 0.9905424;
   }

   if (idx == 200) den = 1.;
   if (idx == 300) num = den * den;


   double syst_factor = 1.;
   if (idx == -1) syst_factor = 1 + syst;
   if (idx == -2) syst_factor = 1 - syst;
   return (num / den)*syst_factor;
}


///////////////////////////////////////////////////
//               T R G      P b P b              //
///////////////////////////////////////////////////
double tnp_weight_trig_pbpb(double pt, double eta, double cent, int idx)
{
  // Check input variables
  double abseta = fabs(eta);
  if (pt<15) { std::cout << "[WARNING] Muon pT (" << pt <<") < 15 GeV/c" << std::endl; return 1.0; }
  if (abseta>2.4) { std::cout << "[WARNING] Muon pseudo-rapidity (" << eta << ") outside [-2.4, 2.4]" << std::endl; return 1.0; }
  if (cent<0 || cent>100) { std::cout << "[ERROR] Centrality (" << cent << ") outside [0%, 100%]" << std::endl; return 1.0; }

  double num=1.0, den=1.0;

  // MC
  if (abseta >= 0 && abseta < 1.2) { 
    if (cent >= 0 && cent < 10) { 
      if (pt >= 15 && pt < 20) den = 0.90236;
      else if (pt >= 20 && pt < 30) den = 0.926668;
      else if (pt >= 30 && pt < 50) den = 0.950628;
      else if (pt >= 50 && pt < 80) den = 0.955633;
      else if (pt >= 80 && pt < 9999) den = 0.941845;
    }
    else if (cent >= 10 && cent < 20) { 
      if (pt >= 15 && pt < 20) den = 0.945948;
      else if (pt >= 20 && pt < 30) den = 0.957968;
      else if (pt >= 30 && pt < 50) den = 0.966709;
      else if (pt >= 50 && pt < 80) den = 0.96932;
      else if (pt >= 80 && pt < 9999) den = 0.962404;
    }
    else if (cent >= 20 && cent < 100) { 
      if (pt >= 15 && pt < 20) den = 0.972135;
      else if (pt >= 20 && pt < 30) den = 0.976769;
      else if (pt >= 30 && pt < 50) den = 0.977621;
      else if (pt >= 50 && pt < 80) den = 0.976922;
      else if (pt >= 80 && pt < 9999) den = 0.97363;
    }
  }
  else if (abseta >= 1.2 && abseta < 2.1) { 
    if (cent >= 0 && cent < 10) { 
      if (pt >= 15 && pt < 20) den = 0.886045;
      else if (pt >= 20 && pt < 30) den = 0.905927;
      else if (pt >= 30 && pt < 50) den = 0.927901;
      else if (pt >= 50 && pt < 80) den = 0.93394;
      else if (pt >= 80 && pt < 9999) den = 0.941171;
    }
    else if (cent >= 10 && cent < 20) { 
      if (pt >= 15 && pt < 20) den = 0.930556;
      else if (pt >= 20 && pt < 30) den = 0.939308;
      else if (pt >= 30 && pt < 50) den = 0.95477;
      else if (pt >= 50 && pt < 80) den = 0.961815;
      else if (pt >= 80 && pt < 9999) den = 0.961547;
    }
    else if (cent >= 20 && cent < 100) { 
      if (pt >= 15 && pt < 20) den = 0.96814;
      else if (pt >= 20 && pt < 30) den = 0.972713;
      else if (pt >= 30 && pt < 50) den = 0.980361;
      else if (pt >= 50 && pt < 80) den = 0.981367;
      else if (pt >= 80 && pt < 9999) den = 0.980518;
    }
  }
  else if (abseta >= 2.1 && abseta < 2.4) { 
    if (cent >= 0 && cent < 20) { 
      if (pt >= 15 && pt < 20) den = 0.851172;
      else if (pt >= 20 && pt < 30) den = 0.883812;
      else if (pt >= 30 && pt < 50) den = 0.910599;
      else if (pt >= 50 && pt < 9999) den = 0.925773;
    }
    else if (cent >= 20 && cent < 100) { 
      if (pt >= 15 && pt < 20) den = 0.957833;
      else if (pt >= 20 && pt < 30) den = 0.969499;
      else if (pt >= 30 && pt < 50) den = 0.975326;
      else if (pt >= 50 && pt < 9999) den = 0.976695;
    }
  }

  // data
  if (idx <= 0 || idx > 10) { // nominal
    if (abseta >= 0 && abseta < 1.2) { 
      if (cent >= 0 && cent < 10) { 
        if (pt >= 15 && pt < 20) num = 0.825623;
        else if (pt >= 20 && pt < 30) num = 0.88834;
        else if (pt >= 30 && pt < 50) num = 0.924312;
        else if (pt >= 50 && pt < 80) num = 0.92636;
        else if (pt >= 80 && pt < 9999) num = 0.957096;
      }
      else if (cent >= 10 && cent < 20) { 
        if (pt >= 15 && pt < 20) num = 0.902638;
        else if (pt >= 20 && pt < 30) num = 0.935704;
        else if (pt >= 30 && pt < 50) num = 0.949354;
        else if (pt >= 50 && pt < 80) num = 0.949748;
        else if (pt >= 80 && pt < 9999) num = 0.930156;
      }
      else if (cent >= 20 && cent < 100) { 
        if (pt >= 15 && pt < 20) num = 0.974178;
        else if (pt >= 20 && pt < 30) num = 0.971988;
        else if (pt >= 30 && pt < 50) num = 0.971353;
        else if (pt >= 50 && pt < 80) num = 0.970512;
        else if (pt >= 80 && pt < 9999) num = 0.936028;
      }
    }
    else if (abseta >= 1.2 && abseta < 2.1) { 
      if (cent >= 0 && cent < 10) { 
        if (pt >= 15 && pt < 20) num = 0.837076;
        else if (pt >= 20 && pt < 30) num = 0.85444;
        else if (pt >= 30 && pt < 50) num = 0.886979;
        else if (pt >= 50 && pt < 80) num = 0.906104;
        else if (pt >= 80 && pt < 9999) num = 0.924293;
      }
      else if (cent >= 10 && cent < 20) { 
        if (pt >= 15 && pt < 20) num = 0.887064;
        else if (pt >= 20 && pt < 30) num = 0.914084;
        else if (pt >= 30 && pt < 50) num = 0.924853;
        else if (pt >= 50 && pt < 80) num = 0.948331;
        else if (pt >= 80 && pt < 9999) num = 0.864777;
      }
      else if (cent >= 20 && cent < 100) { 
        if (pt >= 15 && pt < 20) num = 0.965473;
        else if (pt >= 20 && pt < 30) num = 0.966372;
        else if (pt >= 30 && pt < 50) num = 0.967069;
        else if (pt >= 50 && pt < 80) num = 0.968079;
        else if (pt >= 80 && pt < 9999) num = 0.983006;
      }
    }
    else if (abseta >= 2.1 && abseta < 2.4) { 
      if (cent >= 0 && cent < 20) { 
        if (pt >= 15 && pt < 20) num = 0.792647;
        else if (pt >= 20 && pt < 30) num = 0.813261;
        else if (pt >= 30 && pt < 50) num = 0.861218;
        else if (pt >= 50 && pt < 9999) num = 0.884202;
      }
      else if (cent >= 20 && cent < 100) { 
        if (pt >= 15 && pt < 20) num = 0.93984;
        else if (pt >= 20 && pt < 30) num = 0.968807;
        else if (pt >= 30 && pt < 50) num = 0.964683;
        else if (pt >= 50 && pt < 9999) num = 0.961035;
      }
    }
  }
  else if (idx == 1) { // stat up
    if (abseta >= 0 && abseta < 1.2) { 
      if (cent >= 0 && cent < 10) { 
        if (pt >= 15 && pt < 20) num = 0.849434;
        else if (pt >= 20 && pt < 30) num = 0.898723;
        else if (pt >= 30 && pt < 50) num = 0.927712;
        else if (pt >= 50 && pt < 80) num = 0.935152;
        else if (pt >= 80 && pt < 9999) num = 0.973337;
      }
      else if (cent >= 10 && cent < 20) { 
        if (pt >= 15 && pt < 20) num = 0.929686;
        else if (pt >= 20 && pt < 30) num = 0.944892;
        else if (pt >= 30 && pt < 50) num = 0.952679;
        else if (pt >= 50 && pt < 80) num = 0.957877;
        else if (pt >= 80 && pt < 9999) num = 0.95577;
      }
      else if (cent >= 20 && cent < 100) { 
        if (pt >= 15 && pt < 20) num = 0.985178;
        else if (pt >= 20 && pt < 30) num = 0.97714;
        else if (pt >= 30 && pt < 50) num = 0.975451;
        else if (pt >= 50 && pt < 80) num = 0.975891;
        else if (pt >= 80 && pt < 9999) num = 0.958392;
      }
    }
    else if (abseta >= 1.2 && abseta < 2.1) { 
      if (cent >= 0 && cent < 10) { 
        if (pt >= 15 && pt < 20) num = 0.86489;
        else if (pt >= 20 && pt < 30) num = 0.866847;
        else if (pt >= 30 && pt < 50) num = 0.892546;
        else if (pt >= 50 && pt < 80) num = 0.919425;
        else if (pt >= 80 && pt < 9999) num = 0.956828;
      }
      else if (cent >= 10 && cent < 20) { 
        if (pt >= 15 && pt < 20) num = 0.91195;
        else if (pt >= 20 && pt < 30) num = 0.926608;
        else if (pt >= 30 && pt < 50) num = 0.930635;
        else if (pt >= 50 && pt < 80) num = 0.959537;
        else if (pt >= 80 && pt < 9999) num = 0.914375;
      }
      else if (cent >= 20 && cent < 100) { 
        if (pt >= 15 && pt < 20) num = 0.979286;
        else if (pt >= 20 && pt < 30) num = 0.973193;
        else if (pt >= 30 && pt < 50) num = 0.970141;
        else if (pt >= 50 && pt < 80) num = 0.975837;
        else if (pt >= 80 && pt < 9999) num = 0.994835;
      }
    }
    else if (abseta >= 2.1 && abseta < 2.4) { 
      if (cent >= 0 && cent < 20) { 
        if (pt >= 15 && pt < 20) num = 0.834505;
        else if (pt >= 20 && pt < 30) num = 0.834372;
        else if (pt >= 30 && pt < 50) num = 0.870948;
        else if (pt >= 50 && pt < 9999) num = 0.906348;
      }
      else if (cent >= 20 && cent < 100) { 
        if (pt >= 15 && pt < 20) num = 0.965181;
        else if (pt >= 20 && pt < 30) num = 0.979263;
        else if (pt >= 30 && pt < 50) num = 0.971248;
        else if (pt >= 50 && pt < 9999) num = 0.978381;
      }
    }
  }
  else if (idx == 2) { // stat down
    if (abseta >= 0 && abseta < 1.2) { 
      if (cent >= 0 && cent < 10) { 
        if (pt >= 15 && pt < 20) num = 0.799965;
        else if (pt >= 20 && pt < 30) num = 0.877428;
        else if (pt >= 30 && pt < 50) num = 0.920806;
        else if (pt >= 50 && pt < 80) num = 0.916977;
        else if (pt >= 80 && pt < 9999) num = 0.935705;
      }
      else if (cent >= 10 && cent < 20) { 
        if (pt >= 15 && pt < 20) num = 0.871347;
        else if (pt >= 20 && pt < 30) num = 0.925738;
        else if (pt >= 30 && pt < 50) num = 0.945883;
        else if (pt >= 50 && pt < 80) num = 0.940674;
        else if (pt >= 80 && pt < 9999) num = 0.897599;
      }
      else if (cent >= 20 && cent < 100) { 
        if (pt >= 15 && pt < 20) num = 0.959218;
        else if (pt >= 20 && pt < 30) num = 0.966266;
        else if (pt >= 30 && pt < 50) num = 0.968389;
        else if (pt >= 50 && pt < 80) num = 0.964405;
        else if (pt >= 80 && pt < 9999) num = 0.906852;
      }
    }
    else if (abseta >= 1.2 && abseta < 2.1) { 
      if (cent >= 0 && cent < 10) { 
        if (pt >= 15 && pt < 20) num = 0.807131;
        else if (pt >= 20 && pt < 30) num = 0.8416;
        else if (pt >= 30 && pt < 50) num = 0.881247;
        else if (pt >= 50 && pt < 80) num = 0.89155;
        else if (pt >= 80 && pt < 9999) num = 0.880776;
      }
      else if (cent >= 10 && cent < 20) { 
        if (pt >= 15 && pt < 20) num = 0.857638;
        else if (pt >= 20 && pt < 30) num = 0.900577;
        else if (pt >= 30 && pt < 50) num = 0.918849;
        else if (pt >= 50 && pt < 80) num = 0.935638;
        else if (pt >= 80 && pt < 9999) num = 0.802326;
      }
      else if (cent >= 20 && cent < 100) { 
        if (pt >= 15 && pt < 20) num = 0.948239;
        else if (pt >= 20 && pt < 30) num = 0.958668;
        else if (pt >= 30 && pt < 50) num = 0.963797;
        else if (pt >= 50 && pt < 80) num = 0.959117;
        else if (pt >= 80 && pt < 9999) num = 0.960314;
      }
    }
    else if (abseta >= 2.1 && abseta < 2.4) { 
      if (cent >= 0 && cent < 20) { 
        if (pt >= 15 && pt < 20) num = 0.746978;
        else if (pt >= 20 && pt < 30) num = 0.790995;
        else if (pt >= 30 && pt < 50) num = 0.851077;
        else if (pt >= 50 && pt < 9999) num = 0.859061;
      }
      else if (cent >= 20 && cent < 100) { 
        if (pt >= 15 && pt < 20) num = 0.905329;
        else if (pt >= 20 && pt < 30) num = 0.955546;
        else if (pt >= 30 && pt < 50) num = 0.957204;
        else if (pt >= 50 && pt < 9999) num = 0.937631;
      }
    }
  }
  else if (idx == -1) { // TnP fit syst up
    if (abseta >= 0 && abseta < 1.2) { 
      if (cent >= 0 && cent < 10) { 
        if (pt >= 15 && pt < 20) num = 0.841672;
        else if (pt >= 20 && pt < 30) num = 0.892162;
        else if (pt >= 30 && pt < 50) num = 0.927346;
        else if (pt >= 50 && pt < 80) num = 0.932938;
        else if (pt >= 80 && pt < 9999) num = 0.957671;
      }
      else if (cent >= 10 && cent < 20) { 
        if (pt >= 15 && pt < 20) num = 0.914009;
        else if (pt >= 20 && pt < 30) num = 0.940075;
        else if (pt >= 30 && pt < 50) num = 0.950756;
        else if (pt >= 50 && pt < 80) num = 0.950313;
        else if (pt >= 80 && pt < 9999) num = 0.93906;
      }
      else if (cent >= 20 && cent < 100) { 
        if (pt >= 15 && pt < 20) num = 0.976316;
        else if (pt >= 20 && pt < 30) num = 0.979949;
        else if (pt >= 30 && pt < 50) num = 0.974861;
        else if (pt >= 50 && pt < 80) num = 0.974511;
        else if (pt >= 80 && pt < 9999) num = 0.940442;
      }
    }
    else if (abseta >= 1.2 && abseta < 2.1) { 
      if (cent >= 0 && cent < 10) { 
        if (pt >= 15 && pt < 20) num = 0.864864;
        else if (pt >= 20 && pt < 30) num = 0.863452;
        else if (pt >= 30 && pt < 50) num = 0.89454;
        else if (pt >= 50 && pt < 80) num = 0.911352;
        else if (pt >= 80 && pt < 9999) num = 0.936901;
      }
      else if (cent >= 10 && cent < 20) { 
        if (pt >= 15 && pt < 20) num = 0.898048;
        else if (pt >= 20 && pt < 30) num = 0.9333;
        else if (pt >= 30 && pt < 50) num = 0.926577;
        else if (pt >= 50 && pt < 80) num = 0.951679;
        else if (pt >= 80 && pt < 9999) num = 0.885396;
      }
      else if (cent >= 20 && cent < 100) { 
        if (pt >= 15 && pt < 20) num = 0.970971;
        else if (pt >= 20 && pt < 30) num = 0.968504;
        else if (pt >= 30 && pt < 50) num = 0.967916;
        else if (pt >= 50 && pt < 80) num = 0.969153;
        else if (pt >= 80 && pt < 9999) num = 0.98622;
      }
    }
    else if (abseta >= 2.1 && abseta < 2.4) { 
      if (cent >= 0 && cent < 20) { 
        if (pt >= 15 && pt < 20) num = 0.814721;
        else if (pt >= 20 && pt < 30) num = 0.825021;
        else if (pt >= 30 && pt < 50) num = 0.865798;
        else if (pt >= 50 && pt < 9999) num = 0.886639;
      }
      else if (cent >= 20 && cent < 100) { 
        if (pt >= 15 && pt < 20) num = 0.958064;
        else if (pt >= 20 && pt < 30) num = 0.971345;
        else if (pt >= 30 && pt < 50) num = 0.966718;
        else if (pt >= 50 && pt < 9999) num = 0.966004;
      }
    }
  }
  else if (idx == -2) { // TnP fit syst down
    if (abseta >= 0 && abseta < 1.2) { 
      if (cent >= 0 && cent < 10) { 
        if (pt >= 15 && pt < 20) num = 0.809573;
        else if (pt >= 20 && pt < 30) num = 0.884517;
        else if (pt >= 30 && pt < 50) num = 0.921277;
        else if (pt >= 50 && pt < 80) num = 0.919781;
        else if (pt >= 80 && pt < 9999) num = 0.956521;
      }
      else if (cent >= 10 && cent < 20) { 
        if (pt >= 15 && pt < 20) num = 0.891268;
        else if (pt >= 20 && pt < 30) num = 0.931334;
        else if (pt >= 30 && pt < 50) num = 0.947952;
        else if (pt >= 50 && pt < 80) num = 0.949184;
        else if (pt >= 80 && pt < 9999) num = 0.921253;
      }
      else if (cent >= 20 && cent < 100) { 
        if (pt >= 15 && pt < 20) num = 0.972041;
        else if (pt >= 20 && pt < 30) num = 0.964027;
        else if (pt >= 30 && pt < 50) num = 0.967845;
        else if (pt >= 50 && pt < 80) num = 0.966513;
        else if (pt >= 80 && pt < 9999) num = 0.931614;
      }
    }
    else if (abseta >= 1.2 && abseta < 2.1) { 
      if (cent >= 0 && cent < 10) { 
        if (pt >= 15 && pt < 20) num = 0.809289;
        else if (pt >= 20 && pt < 30) num = 0.845428;
        else if (pt >= 30 && pt < 50) num = 0.879417;
        else if (pt >= 50 && pt < 80) num = 0.900856;
        else if (pt >= 80 && pt < 9999) num = 0.911685;
      }
      else if (cent >= 10 && cent < 20) { 
        if (pt >= 15 && pt < 20) num = 0.87608;
        else if (pt >= 20 && pt < 30) num = 0.894868;
        else if (pt >= 30 && pt < 50) num = 0.923129;
        else if (pt >= 50 && pt < 80) num = 0.944984;
        else if (pt >= 80 && pt < 9999) num = 0.844158;
      }
      else if (cent >= 20 && cent < 100) { 
        if (pt >= 15 && pt < 20) num = 0.959975;
        else if (pt >= 20 && pt < 30) num = 0.964239;
        else if (pt >= 30 && pt < 50) num = 0.966221;
        else if (pt >= 50 && pt < 80) num = 0.967005;
        else if (pt >= 80 && pt < 9999) num = 0.979791;
      }
    }
    else if (abseta >= 2.1 && abseta < 2.4) { 
      if (cent >= 0 && cent < 20) { 
        if (pt >= 15 && pt < 20) num = 0.770573;
        else if (pt >= 20 && pt < 30) num = 0.8015;
        else if (pt >= 30 && pt < 50) num = 0.856638;
        else if (pt >= 50 && pt < 9999) num = 0.881766;
      }
      else if (cent >= 20 && cent < 100) { 
        if (pt >= 15 && pt < 20) num = 0.921617;
        else if (pt >= 20 && pt < 30) num = 0.966269;
        else if (pt >= 30 && pt < 50) num = 0.962647;
        else if (pt >= 50 && pt < 9999) num = 0.956067;
      }
    }
  }

  if (idx == 200) den = 1.0;
  if (idx == 300) num = den * den;

  return (num/den);
}

#endif //#ifndef muons_2018pbpb_tnp_weight_h
