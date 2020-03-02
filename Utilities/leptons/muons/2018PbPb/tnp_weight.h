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
// - GlbTrk: (tnp_weight_glbPFtrk_pbpb)
//   * idx = 0: nominal
//   * idx = -1: syst variation,  +1 sigma
//   * idx = -2: syst variation,  -1 sigma
//   * idx = +1: stat variation,  +1 sigma
//   * idx = +2: stat variation,  -1 sigma

//   * idx = -3: syst variation, no PF uncertainty included  +1 sigma
//   * idx = -4: syst variation, no PF uncertainty included  -1 sigma
//   * idx = -5: only PF uncertainty included (1 +1 sigma)
//   * idx = -6: only PF uncertainty included (1 -1 sigma)


// - MuID: (tnp_weight_muid_pbpb)
//   * idx = 0: nominal
//   * idx = -1: syst variation,  +1 sigma
//   * idx = -2: syst variation,  -1 sigma
//   * idx = +1: stat variation,  +1 sigma
//   * idx = +2: stat variation,  -1 sigma
//
// - Trigger: (tnp_weight_trig_pbpb)
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
double tnp_weight_glbPFtrk_pbpb(double eta, double cent, int idx = 0);
double tnp_weight_muid_pbpb(double eta, int idx=0);
double tnp_weight_trig_pbpb(double pt, double eta, double cent, int idx=0);


///////////////////////////////////////////////////
//              G l b P F  T r k    P b P b           //
///////////////////////////////////////////////////

double tnp_uncertainty_pf_pbpb(double eta, double cent, int idx = 0); //not a standalone efficiency - only systematic uncertainty is provided, and added to the values of glbpftrk (as it started by taking PF out of glbtrk). No need to call, the syst uncertainty is included in the glbtrk by default

double tnp_weight_glbPFtrk_pbpb(double eta, double cent, int idx) //cent 0-100%
{
    double num = 1, den = 1;

    if (std::fabs(eta)>2.4) {
        //std::cout << "[WARNING] Muon pseudo-rapidity (" << eta << ") outside [-2.4, 2.4]" << std::endl;
        return 1.0;
    }
    if (cent < 0 || cent>100) {
        //std::cout << "[ERROR] Centrality (" << cent << ") outside [0%, 100%]" << std::endl;
        return 1.0;
    }

    double abseta = std::fabs(eta);

    if (cent >= 0 && cent < 40)
    {
        // MC
        if (abseta >= 0 && abseta <= 0.9) { den = 0.978912; }
        else if (abseta > 0.9 && abseta <= 1.2) { den = 0.980928; }
        else if (abseta > 1.2 && abseta <= 1.6) { den = 0.99245; }
        else if (abseta > 1.6 && abseta <= 2.1) { den = 0.986688; }
        else if (abseta > 2.1 && abseta <= 2.4) { den = 0.980687; }

        // data
        if (idx == 0|| idx == 200) { // nominal
            if (abseta >= 0 && abseta <= 0.9) { num = 0.971173; }
            else if (abseta > 0.9 && abseta <= 1.2) { num = 0.958148; }
            else if (abseta > 1.2 && abseta <= 1.6) { num = 0.987326; }
            else if (abseta > 1.6 && abseta <= 2.1) { num = 0.983108; }
            else if (abseta > 2.1 && abseta <= 2.4) { num = 0.965458; }
        }

        if (idx == 1) { // stat up
            if (abseta >= 0 && abseta <= 0.9) { num = 0.972533; }
            else if (abseta > 0.9 && abseta <= 1.2) { num = 0.963009; }
            else if (abseta > 1.2 && abseta <= 1.6) { num = 0.988907; }
            else if (abseta > 1.6 && abseta <= 2.1) { num = 0.984995; }
            else if (abseta > 2.1 && abseta <= 2.4) { num = 0.96963; }
        }
        if (idx == 2) { // stat down
            if (abseta >= 0 && abseta <= 0.9) { num = 0.969776; }
            else if (abseta > 0.9 && abseta <= 1.2) { num = 0.954932; }
            else if (abseta > 1.2 && abseta <= 1.6) { num = 0.985623; }
            else if (abseta > 1.6 && abseta <= 2.1) { num = 0.981102; }
            else if (abseta > 2.1 && abseta <= 2.4) { num = 0.96104; }
        }
        if (idx == -1|| idx == -3) { // syst up
        if (abseta >= 0 && abseta <= 0.9) { num = 0.971762; }
        else if (abseta > 0.9 && abseta <= 1.2) { num = 0.959959; }
        else if (abseta > 1.2 && abseta <= 1.6) { num = 0.988186; }
        else if (abseta > 1.6 && abseta <= 2.1) { num = 0.983305; }
        else if (abseta > 2.1 && abseta <= 2.4) { num = 0.966265; }
        }
        if (idx == -2 || idx == -4) { // syst down
        if (abseta >= 0 && abseta <= 0.9) { num = 0.970584; }
        else if (abseta > 0.9 && abseta <= 1.2) { num = 0.956336; }
        else if (abseta > 1.2 && abseta <= 1.6) { num = 0.986467; }
        else if (abseta > 1.6 && abseta <= 2.1) { num = 0.982912; }
        else if (abseta > 2.1 && abseta <= 2.4) { num = 0.964651; }
        }

    }
    else if (cent >= 40 && cent <= 100)
    {

        // MC
        if (abseta >= 0 && abseta <= 0.9) { den = 0.98744; }
        else if (abseta > 0.9 && abseta <= 1.2) { den = 0.99433; }
        else if (abseta > 1.2 && abseta <= 1.6) { den = 0.998913; }
        else if (abseta > 1.6 && abseta <= 2.1) { den = 0.996517; }
        else if (abseta > 2.1 && abseta <= 2.4) { den = 0.992023; }

        // data
        if (idx == 0 || idx == 200) { // nominal
            if (abseta >= 0 && abseta <= 0.9) { num = 0.985895; }
            else if (abseta > 0.9 && abseta <= 1.2) { num = 0.994328; }
            else if (abseta > 1.2 && abseta <= 1.6) { num = 0.997029; }
            else if (abseta > 1.6 && abseta <= 2.1) { num = 0.993172; }
            else if (abseta > 2.1 && abseta <= 2.4) { num = 0.990654; }
        }

        if (idx == 1) { // stat up
            if (abseta >= 0 && abseta <= 0.9) { num = 0.988874; }
            else if (abseta > 0.9 && abseta <= 1.2) { num = 0.997005; }
            else if (abseta > 1.2 && abseta <= 1.6) { num = 0.999124; }
            else if (abseta > 1.6 && abseta <= 2.1) { num = 0.996226; }
            else if (abseta > 2.1 && abseta <= 2.4) { num = 0.995055; }
        }
        if (idx == 2) { // stat down
            if (abseta >= 0 && abseta <= 0.9) { num = 0.982558; }
            else if (abseta > 0.9 && abseta <= 1.2) { num = 0.990409; }
            else if (abseta > 1.2 && abseta <= 1.6) { num = 0.993971; }
            else if (abseta > 1.6 && abseta <= 2.1) { num = 0.989008; }
            else if (abseta > 2.1 && abseta <= 2.4) { num = 0.984224; }
        }
        if (idx == -1 || idx == -3) { // syst up
        if (abseta >= 0 && abseta <= 0.9) { num = 0.986813; }
        else if (abseta > 0.9 && abseta <= 1.2) { num = 0.994583; }
        else if (abseta > 1.2 && abseta <= 1.6) { num = 0.997692; }
        else if (abseta > 1.6 && abseta <= 2.1) { num = 0.994297; }
        else if (abseta > 2.1 && abseta <= 2.4) { num = 0.991597; }
        }
        if (idx == -2 || idx == -4) { // syst down
        if (abseta >= 0 && abseta <= 0.9) { num = 0.984977; }
        else if (abseta > 0.9 && abseta <= 1.2) { num = 0.994074; }
        else if (abseta > 1.2 && abseta <= 1.6) { num = 0.996367; }
        else if (abseta > 1.6 && abseta <= 2.1) { num = 0.992047; }
        else if (abseta > 2.1 && abseta <= 2.4) { num = 0.989711; }
        }
    }

    if (idx == -1 || idx == -2) //add systematic from PF
    {
        double glbtrkNomRD = tnp_weight_glbPFtrk_pbpb(eta, cent, 200); //get the nominal RD value
        double pfSyst = (tnp_uncertainty_pf_pbpb(eta, cent, 200) - tnp_uncertainty_pf_pbpb(eta, cent, 300)) / tnp_uncertainty_pf_pbpb(eta, cent, 300); //relative pf syst uncertainty
        double glbtrkSyst = ((num - glbtrkNomRD) / glbtrkNomRD); //relative glbtrk syst uncertainty on RD
        double totalglbtrkSyst = TMath::Sqrt(pfSyst*pfSyst + glbtrkSyst * glbtrkSyst); //relative total uncertainty
        if (idx == -1) { num = glbtrkNomRD + totalglbtrkSyst * glbtrkNomRD; }//calculate back the actual value
        else { num = glbtrkNomRD - totalglbtrkSyst * glbtrkNomRD; }
        if (num > 1) num = 1; //with addition of PF, numerator could be higher than 1 - unphysical
    }

    if (idx == -5 || idx == -6) //return 1+- PF syst
    {
        double pfSyst = (tnp_uncertainty_pf_pbpb(eta, cent, 200) - tnp_uncertainty_pf_pbpb(eta, cent, 300)) / tnp_uncertainty_pf_pbpb(eta, cent, 300);
        if (idx == -5) { num = den + pfSyst * den; }
        else { num = den - pfSyst * den; }
    }


    if (idx == 200) den = 1.;
    if (idx == 300) num = den * den;


    return (num / den);

}

//              P F    P b P b           //

double tnp_uncertainty_pf_pbpb(double eta, double cent, int idx)
{
    double num = 1, den = 1;

    if (std::fabs(eta) > 2.4) {
        //std::cout << "[WARNING] Muon pseudo-rapidity (" << eta << ") outside [-2.4, 2.4]" << std::endl;
        return 1.0;
    }
    if (cent < 0 || cent>100) {
        //std::cout << "[ERROR] Centrality (" << cent << ") outside [0%, 100%]" << std::endl;
        return 1.0;
    }

    double abseta = std::fabs(eta);

    if (cent >= 0 && cent < 40)
    {
        // MC
        if (abseta >= 0 && abseta <= 0.9) { den = 0.990737; }
        else if (abseta > 0.9 && abseta <= 1.2) { den = 0.991441; }
        else if (abseta > 1.2 && abseta <= 1.6) { den = 0.98534; }
        else if (abseta > 1.6 && abseta <= 2.1) { den = 0.991639; }
        else if (abseta > 2.1 && abseta <= 2.4) { den = 0.978643; }


        if (idx == 0 || idx == 200) {
            if (abseta >= 0 && abseta <= 0.9) { num = 0.989266; }
            else if (abseta > 0.9 && abseta <= 1.2) { num = 0.984868; }
            else if (abseta > 1.2 && abseta <= 1.6) { num = 0.981871; }
            else if (abseta > 1.6 && abseta <= 2.1) { num = 0.984655; }
            else if (abseta > 2.1 && abseta <= 2.4) { num = 0.949866; }
        }

    }
    else if (cent >= 40 && cent <= 100)
    {
        // MC
        if (abseta >= 0 && abseta <= 0.9) { den = 0.997979; }
        else if (abseta > 0.9 && abseta <= 1.2) { den = 0.997599; }
        else if (abseta > 1.2 && abseta <= 1.6) { den = 0.997771; }
        else if (abseta > 1.6 && abseta <= 2.1) { den = 0.997692; }
        else if (abseta > 2.1 && abseta <= 2.4) { den = 0.994569; }

        if (idx == 0 || idx == 200) {
            if (abseta >= 0 && abseta <= 0.9) { num = 0.998014; }
            else if (abseta > 0.9 && abseta <= 1.2) { num = 0.992184; }
            else if (abseta > 1.2 && abseta <= 1.6) { num = 0.996689; }
            else if (abseta > 1.6 && abseta <= 2.1) { num = 0.991708; }
            else if (abseta > 2.1 && abseta <= 2.4) { num = 0.969514; }
        }
    }

    if (idx == 200) den = 1.;
    if (idx == 300) num = den * den;


    return (num / den);


}


///////////////////////////////////////////////////
//                 M u I D    P b P b            //
///////////////////////////////////////////////////
double tnp_weight_muid_pbpb(double eta, int idx)
{
   double syst = 0.6e-2;    //preliminary

   double num=1,den=1;

   if (std::fabs(eta) > 2.4) {
       //std::cout << "[WARNING] Muon pseudo-rapidity (" << eta << ") outside [-2.4, 2.4]" << std::endl;
       return 1.0;
   }

   // MC
   if (eta >= -2.4 && eta <= -2.1) { den = 0.994139; }
   else if (eta > -2.1 && eta <= -1.6) { den = 0.99449; }
   else if (eta > -1.6 && eta <= -1.2) { den = 0.983536; }
   else if (eta > -1.2 && eta <= -0.9) { den = 0.964562; }
   else if (eta > -0.9 && eta <= -0.6) { den = 0.973316; }
   else if (eta > -0.6 && eta <= -0.3) { den = 0.981446; }
   else if (eta > -0.3 && eta <= 0) { den = 0.968189; }
   else if (eta > 0 && eta <= 0.3) { den = 0.9617; }
   else if (eta > 0.3 && eta <= 0.6) { den = 0.979738; }
   else if (eta > 0.6 && eta <= 0.9) { den = 0.969536; }
   else if (eta > 0.9 && eta <= 1.2) { den = 0.960259; }
   else if (eta > 1.2 && eta <= 1.6) { den = 0.983279; }
   else if (eta > 1.6 && eta <= 2.1) { den = 0.99477; }
   else if (eta > 2.1 && eta <= 2.4) { den = 0.994065; }


   // data
   if (idx <= 0 || idx > 10) { // nominal
       if (eta >= -2.4 && eta <= -2.1) { num = 0.984278; }
       else if (eta > -2.1 && eta <= -1.6) { num = 0.994031; }
       else if (eta > -1.6 && eta <= -1.2) { num = 0.978562; }
       else if (eta > -1.2 && eta <= -0.9) { num = 0.954321; }
       else if (eta > -0.9 && eta <= -0.6) { num = 0.966508; }
       else if (eta > -0.6 && eta <= -0.3) { num = 0.98402; }
       else if (eta > -0.3 && eta <= 0) { num = 0.958369; }
       else if (eta > 0 && eta <= 0.3) { num = 0.959429; }
       else if (eta > 0.3 && eta <= 0.6) { num = 0.976528; }
       else if (eta > 0.6 && eta <= 0.9) { num = 0.967646; }
       else if (eta > 0.9 && eta <= 1.2) { num = 0.961046; }
       else if (eta > 1.2 && eta <= 1.6) { num = 0.980274; }
       else if (eta > 1.6 && eta <= 2.1) { num = 0.991677; }
       else if (eta > 2.1 && eta <= 2.4) { num = 0.993007; }
   }
   else if (idx == 1) { // stat up
       if (eta >= -2.4 && eta <= -2.1) { num = 0.987203; }
       else if (eta > -2.1 && eta <= -1.6) { num = 0.995641; }
       else if (eta > -1.6 && eta <= -1.2) { num = 0.981641; }
       else if (eta > -1.2 && eta <= -0.9) { num = 0.958889; }
       else if (eta > -0.9 && eta <= -0.6) { num = 0.970274; }
       else if (eta > -0.6 && eta <= -0.3) { num = 0.986882; }
       else if (eta > -0.3 && eta <= 0) { num = 0.962433; }
       else if (eta > 0 && eta <= 0.3) { num = 0.96344; }
       else if (eta > 0.3 && eta <= 0.6) { num = 0.979706; }
       else if (eta > 0.6 && eta <= 0.9) { num = 0.971414; }
       else if (eta > 0.9 && eta <= 1.2) { num = 0.965537; }
       else if (eta > 1.2 && eta <= 1.6) { num = 0.983167; }
       else if (eta > 1.6 && eta <= 2.1) { num = 0.99336; }
       else if (eta > 2.1 && eta <= 2.4) { num = 0.995579; }
   }
   else if (idx == 2) { // stat down
       if (eta >= -2.4 && eta <= -2.1) { num = 0.98094; }
       else if (eta > -2.1 && eta <= -1.6) { num = 0.992199; }
       else if (eta > -1.6 && eta <= -1.2) { num = 0.975247; }
       else if (eta > -1.2 && eta <= -0.9) { num = 0.949482; }
       else if (eta > -0.9 && eta <= -0.6) { num = 0.962497; }
       else if (eta > -0.6 && eta <= -0.3) { num = 0.980916; }
       else if (eta > -0.3 && eta <= 0) { num = 0.954075; }
       else if (eta > 0 && eta <= 0.3) { num = 0.955169; }
       else if (eta > 0.3 && eta <= 0.6) { num = 0.973111; }
       else if (eta > 0.6 && eta <= 0.9) { num = 0.963634; }
       else if (eta > 0.9 && eta <= 1.2) { num = 0.956295; }
       else if (eta > 1.2 && eta <= 1.6) { num = 0.977135; }
       else if (eta > 1.6 && eta <= 2.1) { num = 0.987932; }
       else if (eta > 2.1 && eta <= 2.4) { num = 0.989895; }
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
    double abseta = std::fabs(eta);
    if (pt < 15) {
        //std::cout << "[WARNING] Muon pT (" << pt << ") < 15 GeV/c" << std::endl;
        return 1.0;
    }
    if (abseta > 2.4) {
        //std::cout << "[WARNING] Muon pseudo-rapidity (" << eta << ") outside [-2.4, 2.4]" << std::endl;
        return 1.0;
    }
    if (cent < 0 || cent>100) {
        //std::cout << "[ERROR] Centrality (" << cent << ") outside [0%, 100%]" << std::endl;
        return 1.0;
    }

    double num = 1.0, den = 1.0;

    // MC
    if (cent >= 20 && cent <= 100) den = 0.9775;
    else if (abseta >= 0 && abseta < 1.2) {
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
    }
    else if (abseta >= 2.1 && abseta <= 2.4) {
        if (cent >= 0 && cent < 20) {
            if (pt >= 15 && pt < 20) den = 0.851172;
            else if (pt >= 20 && pt < 30) den = 0.883812;
            else if (pt >= 30 && pt < 50) den = 0.910599;
            else if (pt >= 50 && pt < 9999) den = 0.925773;
        }
    }

    // data
    if (idx <= 0 || idx > 10) { // nominal
        if (cent >= 20 && cent <= 100) { num = 0.969; }
        else if (abseta >= 0 && abseta < 1.2) {
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
        }
        else if (abseta >= 2.1 && abseta <= 2.4) {
            if (cent >= 0 && cent < 20) {
                if (pt >= 15 && pt < 20) num = 0.792647;
                else if (pt >= 20 && pt < 30) num = 0.813261;
                else if (pt >= 30 && pt < 50) num = 0.861218;
                else if (pt >= 50 && pt < 9999) num = 0.884202;
            }
        }
    }
    else if (idx == 1) { // stat up
        if (cent >= 20 && cent <= 100) { num = 0.971; }
        else if (abseta >= 0 && abseta < 1.2) {
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
        }
        else if (abseta >= 2.1 && abseta <= 2.4) {
            if (cent >= 0 && cent < 20) {
                if (pt >= 15 && pt < 20) num = 0.834505;
                else if (pt >= 20 && pt < 30) num = 0.834372;
                else if (pt >= 30 && pt < 50) num = 0.870948;
                else if (pt >= 50 && pt < 9999) num = 0.906348;
            }
        }
    }
    else if (idx == 2) { // stat down
        if (cent >= 20 && cent <= 100) { num = 0.967; }
        else if (abseta >= 0 && abseta < 1.2) {
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
        }
        else if (abseta >= 2.1 && abseta <= 2.4) {
            if (cent >= 0 && cent < 20) {
                if (pt >= 15 && pt < 20) num = 0.746978;
                else if (pt >= 20 && pt < 30) num = 0.790995;
                else if (pt >= 30 && pt < 50) num = 0.851077;
                else if (pt >= 50 && pt < 9999) num = 0.859061;
            }
        }
    }
    else if (idx == -1) { // TnP fit syst up
        if (cent >= 20 && cent <= 100) { num = 0.970; }
        else if (abseta >= 0 && abseta < 1.2) {
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
        }
        else if (abseta >= 2.1 && abseta <= 2.4) {
            if (cent >= 0 && cent < 20) {
                if (pt >= 15 && pt < 20) num = 0.814721;
                else if (pt >= 20 && pt < 30) num = 0.825021;
                else if (pt >= 30 && pt < 50) num = 0.865798;
                else if (pt >= 50 && pt < 9999) num = 0.886639;
            }
        }
    }
    else if (idx == -2) { // TnP fit syst down
        if (cent >= 20 && cent <= 100) { num = 0.968; }
        else if (abseta >= 0 && abseta < 1.2) {
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
        }
        else if (abseta >= 2.1 && abseta <= 2.4) {
            if (cent >= 0 && cent < 20) {
                if (pt >= 15 && pt < 20) num = 0.770573;
                else if (pt >= 20 && pt < 30) num = 0.8015;
                else if (pt >= 30 && pt < 50) num = 0.856638;
                else if (pt >= 50 && pt < 9999) num = 0.881766;
            }
        }
    }

    if (idx == 200) den = 1.0;
    if (idx == 300) num = den * den;

    return (num / den);
}

#endif //#ifndef muons_2018pbpb_tnp_weight_h
