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
// - GlbTrk: (tnp_weight_glbtrk_pbpb)   Preliminary
//   * idx = 0: nominal
//   * idx = -1: syst variation,  +1 sigma
//   * idx = -2: syst variation,  -1 sigma
//   * idx = +1: stat variation,  +1 sigma
//   * idx = +2: stat variation,  -1 sigma
//



// - MuID: (tnp_weight_muid_pbpb)   Preliminary
//   * idx = 0: nominal
//   * idx = -1: syst variation,  +1 sigma
//   * idx = -2: syst variation,  -1 sigma
//   * idx = +1: stat variation,  +1 sigma
//   * idx = +2: stat variation,  -1 sigma
//
// - Trigger: (tnp_weight_trig_pbpb)  Preliminary
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
double tnp_weight_glbtrk_pbpb(double eta, double cent, int idx = 0);
double tnp_weight_muid_pbpb(double eta, int idx=0);
double tnp_weight_trig_pbpb(double pt, double eta, double cent, int idx=0);

///////////////////////////////////////////////////
//              G l b T r k    P b P b           //
///////////////////////////////////////////////////

double tnp_weight_glbtrk_pbpb(double eta, double cent, int idx) //cent 0-100%
{
    double num = 1, den = 1;

    if (std::fabs(eta)>2.4) { std::cout << "[WARNING] Muon pseudo-rapidity (" << eta << ") outside [-2.4, 2.4]" << std::endl; return 1.0; }
    if (cent < 0 || cent>100) { std::cout << "[ERROR] Centrality (" << cent << ") outside [0%, 100%]" << std::endl; return 1.0; }

    if (cent >= 0 && cent < 40)
    {
        // MC
        if (eta >= -2.4 && eta <= -2.1) { den = 0.955981; }
        else if (eta > -2.1 && eta <= -1.6) { den = 0.976366; }
        else if (eta > -1.6 && eta <= -1.2) { den = 0.977068; }
        else if (eta > -1.2 && eta <= -0.9) { den = 0.973544; }
        else if (eta > -0.9 && eta <= 0) { den = 0.970051; }
        else if (eta > 0 && eta <= 0.9) { den = 0.969666; }
        else if (eta > 0.9 && eta <= 1.2) { den = 0.972616; }
        else if (eta > 1.2 && eta <= 1.6) { den = 0.978239; }
        else if (eta > 1.6 && eta <= 2.1) { den = 0.980999; }
        else if (eta > 2.1 && eta <= 2.4) { den = 0.962922; }

        // data
        if (idx <= 0 || idx > 10) { // nominal
            if (eta >= -2.4 && eta <= -2.1) { num = 0.895748; }
            else if (eta > -2.1 && eta <= -1.6) { num = 0.961812; }
            else if (eta > -1.6 && eta <= -1.2) { num = 0.963615; }
            else if (eta > -1.2 && eta <= -0.9) { num = 0.953689; }
            else if (eta > -0.9 && eta <= 0) { num = 0.960387; }
            else if (eta > 0 && eta <= 0.9) { num = 0.959435; }
            else if (eta > 0.9 && eta <= 1.2) { num = 0.935934; }
            else if (eta > 1.2 && eta <= 1.6) { num = 0.968385; }
            else if (eta > 1.6 && eta <= 2.1) { num = 0.97469; }
            else if (eta > 2.1 && eta <= 2.4) { num = 0.940124; }
        }

        if (idx == 1) { // stat up
            if (eta >= -2.4 && eta <= -2.1) { num = 0.912779; }
            else if (eta > -2.1 && eta <= -1.6) { num = 0.96884; }
            else if (eta > -1.6 && eta <= -1.2) { num = 0.968562; }
            else if (eta > -1.2 && eta <= -0.9) { num = 0.960157; }
            else if (eta > -0.9 && eta <= 0) { num = 0.963199; }
            else if (eta > 0 && eta <= 0.9) { num = 0.962357; }
            else if (eta > 0.9 && eta <= 1.2) { num = 0.943263; }
            else if (eta > 1.2 && eta <= 1.6) { num = 0.97301; }
            else if (eta > 1.6 && eta <= 2.1) { num = 0.981141; }
            else if (eta > 2.1 && eta <= 2.4) { num = 0.959268; }
        }

        if (idx == 2) { // stat down
            if (eta >= -2.4 && eta <= -2.1) { num = 0.878718; }
            else if (eta > -2.1 && eta <= -1.6) { num = 0.954599; }
            else if (eta > -1.6 && eta <= -1.2) { num = 0.958397; }
            else if (eta > -1.2 && eta <= -0.9) { num = 0.946923; }
            else if (eta > -0.9 && eta <= 0) { num = 0.957485; }
            else if (eta > 0 && eta <= 0.9) { num = 0.956421; }
            else if (eta > 0.9 && eta <= 1.2) { num = 0.928318; }
            else if (eta > 1.2 && eta <= 1.6) { num = 0.963989; }
            else if (eta > 1.6 && eta <= 2.1) { num = 0.968032; }
            else if (eta > 2.1 && eta <= 2.4) { num = 0.921065; }
        }
        if (idx == -1) { // syst up
            if (eta >= -2.4 && eta <= -2.1) { num = 0.912388; }
            else if (eta > -2.1 && eta <= -1.6) { num = 0.965296; }
            else if (eta > -1.6 && eta <= -1.2) { num = 0.965531; }
            else if (eta > -1.2 && eta <= -0.9) { num = 0.955926; }
            else if (eta > -0.9 && eta <= 0) { num = 0.961333; }
            else if (eta > 0 && eta <= 0.9) { num = 0.960196; }
            else if (eta > 0.9 && eta <= 1.2) { num = 0.937628; }
            else if (eta > 1.2 && eta <= 1.6) { num = 0.971776; }
            else if (eta > 1.6 && eta <= 2.1) { num = 0.97886; }
            else if (eta > 2.1 && eta <= 2.4) { num = 0.947469; }
        }
        if (idx == -2) { // syst down
            if (eta >= -2.4 && eta <= -2.1) { num = 0.879108; }
            else if (eta > -2.1 && eta <= -1.6) { num = 0.958329; }
            else if (eta > -1.6 && eta <= -1.2) { num = 0.961699; }
            else if (eta > -1.2 && eta <= -0.9) { num = 0.951452; }
            else if (eta > -0.9 && eta <= 0) { num = 0.959441; }
            else if (eta > 0 && eta <= 0.9) { num = 0.958675; }
            else if (eta > 0.9 && eta <= 1.2) { num = 0.934239; }
            else if (eta > 1.2 && eta <= 1.6) { num = 0.964993; }
            else if (eta > 1.6 && eta <= 2.1) { num = 0.97052; }
            else if (eta > 2.1 && eta <= 2.4) { num = 0.932779; }
        }
    }
    else if (cent >= 40 && cent <= 100)
    {
        // MC
        if (eta >= -2.4 && eta <= -2.1) { den = 0.985973; }
        else if (eta > -2.1 && eta <= -1.6) { den = 0.99412; }
        else if (eta > -1.6 && eta <= -1.2) { den = 0.996646; }
        else if (eta > -1.2 && eta <= -0.9) { den = 0.991832; }
        else if (eta > -0.9 && eta <= 0) { den = 0.985575; }
        else if (eta > 0 && eta <= 0.9) { den = 0.985295; }
        else if (eta > 0.9 && eta <= 1.2) { den = 0.992634; }
        else if (eta > 1.2 && eta <= 1.6) { den = 0.996896; }
        else if (eta > 1.6 && eta <= 2.1) { den = 0.994506; }
        else if (eta > 2.1 && eta <= 2.4) { den = 0.987764; }

        // data
        if (idx == 0) { // nominal
            if (eta >= -2.4 && eta <= -2.1) { num = 1; }
            else if (eta > -2.1 && eta <= -1.6) { num = 0.986484; }
            else if (eta > -1.6 && eta <= -1.2) { num = 0.998253; }
            else if (eta > -1.2 && eta <= -0.9) { num = 0.987887; }
            else if (eta > -0.9 && eta <= 0) { num = 0.987426; }
            else if (eta > 0 && eta <= 0.9) { num = 0.981425; }
            else if (eta > 0.9 && eta <= 1.2) { num = 0.984663; }
            else if (eta > 1.2 && eta <= 1.6) { num = 0.992795; }
            else if (eta > 1.6 && eta <= 2.1) { num = 0.983557; }
            else if (eta > 2.1 && eta <= 2.4) { num = 0.954671; }
        }

        if (idx == 1) { // stat up
            if (eta >= -2.4 && eta <= -2.1) { num = 1; }
            else if (eta > -2.1 && eta <= -1.6) { num = 0.99644; }
            else if (eta > -1.6 && eta <= -1.2) { num = 1; }
            else if (eta > -1.2 && eta <= -0.9) { num = 0.994144; }
            else if (eta > -0.9 && eta <= 0) { num = 0.992029; }
            else if (eta > 0 && eta <= 0.9) { num = 0.987024; }
            else if (eta > 0.9 && eta <= 1.2) { num = 0.992709; }
            else if (eta > 1.2 && eta <= 1.6) { num = 0.997686; }
            else if (eta > 1.6 && eta <= 2.1) { num = 0.991048; }
            else if (eta > 2.1 && eta <= 2.4) { num = 0.979823; }
        }
        if (idx == 2) { // stat down
            if (eta >= -2.4 && eta <= -2.1) { num = 0.979675; }
            else if (eta > -2.1 && eta <= -1.6) { num = 0.974748; }
            else if (eta > -1.6 && eta <= -1.2) { num = 0.993399; }
            else if (eta > -1.2 && eta <= -0.9) { num = 0.978866; }
            else if (eta > -0.9 && eta <= 0) { num = 0.98235; }
            else if (eta > 0 && eta <= 0.9) { num = 0.974871; }
            else if (eta > 0.9 && eta <= 1.2) { num = 0.974046; }
            else if (eta > 1.2 && eta <= 1.6) { num = 0.985567; }
            else if (eta > 1.6 && eta <= 2.1) { num = 0.974474; }
            else if (eta > 2.1 && eta <= 2.4) { num = 0.92457; }
        }
        if (idx == -1) { // syst up
            if (eta >= -2.4 && eta <= -2.1) { num = 1.0107; }
            else if (eta > -2.1 && eta <= -1.6) { num = 0.992138; }
            else if (eta > -1.6 && eta <= -1.2) { num = 0.999945; }
            else if (eta > -1.2 && eta <= -0.9) { num = 0.988701; }
            else if (eta > -0.9 && eta <= 0) { num = 0.988734; }
            else if (eta > 0 && eta <= 0.9) { num = 0.985979; }
            else if (eta > 0.9 && eta <= 1.2) { num = 0.987238; }
            else if (eta > 1.2 && eta <= 1.6) { num = 0.994278; }
            else if (eta > 1.6 && eta <= 2.1) { num = 0.98701; }
            else if (eta > 2.1 && eta <= 2.4) { num = 0.960013; }
        }
        if (idx == -2) { // syst down
            if (eta >= -2.4 && eta <= -2.1) { num = 0.989305; }
            else if (eta > -2.1 && eta <= -1.6) { num = 0.980829; }
            else if (eta > -1.6 && eta <= -1.2) { num = 0.99656; }
            else if (eta > -1.2 && eta <= -0.9) { num = 0.987074; }
            else if (eta > -0.9 && eta <= 0) { num = 0.986119; }
            else if (eta > 0 && eta <= 0.9) { num = 0.976872; }
            else if (eta > 0.9 && eta <= 1.2) { num = 0.982088; }
            else if (eta > 1.2 && eta <= 1.6) { num = 0.991312; }
            else if (eta > 1.6 && eta <= 2.1) { num = 0.980103; }
            else if (eta > 2.1 && eta <= 2.4) { num = 0.949328; }
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

    if (std::fabs(eta) > 2.4) { std::cout << "[WARNING] Muon pseudo-rapidity (" << eta << ") outside [-2.4, 2.4]" << std::endl; return 1.0; }

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
    if (pt < 15) { std::cout << "[WARNING] Muon pT (" << pt << ") < 15 GeV/c" << std::endl; return 1.0; }
    if (abseta > 2.4) { std::cout << "[WARNING] Muon pseudo-rapidity (" << eta << ") outside [-2.4, 2.4]" << std::endl; return 1.0; }
    if (cent < 0 || cent>100) { std::cout << "[ERROR] Centrality (" << cent << ") outside [0%, 100%]" << std::endl; return 1.0; }

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
        //else if (cent >= 20 && cent < 100) {
        //  if (pt >= 15 && pt < 20) den = 0.972135;
        //  else if (pt >= 20 && pt < 30) den = 0.976769;
        //  else if (pt >= 30 && pt < 50) den = 0.977621;
        //  else if (pt >= 50 && pt < 80) den = 0.976922;
        //  else if (pt >= 80 && pt < 9999) den = 0.97363;
        //}
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
        //else if (cent >= 20 && cent < 100) {
        //  if (pt >= 15 && pt < 20) den = 0.96814;
        //  else if (pt >= 20 && pt < 30) den = 0.972713;
        //  else if (pt >= 30 && pt < 50) den = 0.980361;
        //  else if (pt >= 50 && pt < 80) den = 0.981367;
        //  else if (pt >= 80 && pt < 9999) den = 0.980518;
        //}
    }
    else if (abseta >= 2.1 && abseta < 2.4) {
        if (cent >= 0 && cent < 20) {
            if (pt >= 15 && pt < 20) den = 0.851172;
            else if (pt >= 20 && pt < 30) den = 0.883812;
            else if (pt >= 30 && pt < 50) den = 0.910599;
            else if (pt >= 50 && pt < 9999) den = 0.925773;
        }
        //else if (cent >= 20 && cent < 100) {
        //  if (pt >= 15 && pt < 20) den = 0.957833;
        //  else if (pt >= 20 && pt < 30) den = 0.969499;
        //  else if (pt >= 30 && pt < 50) den = 0.975326;
        //  else if (pt >= 50 && pt < 9999) den = 0.976695;
        //}
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
            //else if (cent >= 20 && cent < 100) {
            //  if (pt >= 15 && pt < 20) num = 0.974178;
            //  else if (pt >= 20 && pt < 30) num = 0.971988;
            //  else if (pt >= 30 && pt < 50) num = 0.971353;
            //  else if (pt >= 50 && pt < 80) num = 0.970512;
            //  else if (pt >= 80 && pt < 9999) num = 0.936028;
            //}
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
            //else if (cent >= 20 && cent < 100) {
            //  if (pt >= 15 && pt < 20) num = 0.965473;
            //  else if (pt >= 20 && pt < 30) num = 0.966372;
            //  else if (pt >= 30 && pt < 50) num = 0.967069;
            //  else if (pt >= 50 && pt < 80) num = 0.968079;
            //  else if (pt >= 80 && pt < 9999) num = 0.983006;
            //}
        }
        else if (abseta >= 2.1 && abseta < 2.4) {
            if (cent >= 0 && cent < 20) {
                if (pt >= 15 && pt < 20) num = 0.792647;
                else if (pt >= 20 && pt < 30) num = 0.813261;
                else if (pt >= 30 && pt < 50) num = 0.861218;
                else if (pt >= 50 && pt < 9999) num = 0.884202;
            }
            //else if (cent >= 20 && cent < 100) {
            //  if (pt >= 15 && pt < 20) num = 0.93984;
            //  else if (pt >= 20 && pt < 30) num = 0.968807;
            //  else if (pt >= 30 && pt < 50) num = 0.964683;
            //  else if (pt >= 50 && pt < 9999) num = 0.961035;
            //}
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
            //else if (cent >= 20 && cent < 100) {
            //  if (pt >= 15 && pt < 20) num = 0.985178;
            //  else if (pt >= 20 && pt < 30) num = 0.97714;
            //  else if (pt >= 30 && pt < 50) num = 0.975451;
            //  else if (pt >= 50 && pt < 80) num = 0.975891;
            //  else if (pt >= 80 && pt < 9999) num = 0.958392;
            //}
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
            //else if (cent >= 20 && cent < 100) {
            //  if (pt >= 15 && pt < 20) num = 0.979286;
            //  else if (pt >= 20 && pt < 30) num = 0.973193;
            //  else if (pt >= 30 && pt < 50) num = 0.970141;
            //  else if (pt >= 50 && pt < 80) num = 0.975837;
            //  else if (pt >= 80 && pt < 9999) num = 0.994835;
            //}
        }
        else if (abseta >= 2.1 && abseta < 2.4) {
            if (cent >= 0 && cent < 20) {
                if (pt >= 15 && pt < 20) num = 0.834505;
                else if (pt >= 20 && pt < 30) num = 0.834372;
                else if (pt >= 30 && pt < 50) num = 0.870948;
                else if (pt >= 50 && pt < 9999) num = 0.906348;
            }
            //else if (cent >= 20 && cent < 100) {
            //  if (pt >= 15 && pt < 20) num = 0.965181;
            //  else if (pt >= 20 && pt < 30) num = 0.979263;
            //  else if (pt >= 30 && pt < 50) num = 0.971248;
            //  else if (pt >= 50 && pt < 9999) num = 0.978381;
            //}
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
            //else if (cent >= 20 && cent < 100) {
            //  if (pt >= 15 && pt < 20) num = 0.959218;
            //  else if (pt >= 20 && pt < 30) num = 0.966266;
            //  else if (pt >= 30 && pt < 50) num = 0.968389;
            //  else if (pt >= 50 && pt < 80) num = 0.964405;
            //  else if (pt >= 80 && pt < 9999) num = 0.906852;
            //}
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
            //else if (cent >= 20 && cent < 100) {
            //  if (pt >= 15 && pt < 20) num = 0.948239;
            //  else if (pt >= 20 && pt < 30) num = 0.958668;
            //  else if (pt >= 30 && pt < 50) num = 0.963797;
            //  else if (pt >= 50 && pt < 80) num = 0.959117;
            //  else if (pt >= 80 && pt < 9999) num = 0.960314;
            //}
        }
        else if (abseta >= 2.1 && abseta < 2.4) {
            if (cent >= 0 && cent < 20) {
                if (pt >= 15 && pt < 20) num = 0.746978;
                else if (pt >= 20 && pt < 30) num = 0.790995;
                else if (pt >= 30 && pt < 50) num = 0.851077;
                else if (pt >= 50 && pt < 9999) num = 0.859061;
            }
            //else if (cent >= 20 && cent < 100) {
            //  if (pt >= 15 && pt < 20) num = 0.905329;
            //  else if (pt >= 20 && pt < 30) num = 0.955546;
            //  else if (pt >= 30 && pt < 50) num = 0.957204;
            //  else if (pt >= 50 && pt < 9999) num = 0.937631;
            //}
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
            //else if (cent >= 20 && cent < 100) {
            //  if (pt >= 15 && pt < 20) num = 0.976316;
            //  else if (pt >= 20 && pt < 30) num = 0.979949;
            //  else if (pt >= 30 && pt < 50) num = 0.974861;
            //  else if (pt >= 50 && pt < 80) num = 0.974511;
            //  else if (pt >= 80 && pt < 9999) num = 0.940442;
            //}
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
            //else if (cent >= 20 && cent < 100) {
            //  if (pt >= 15 && pt < 20) num = 0.970971;
            //  else if (pt >= 20 && pt < 30) num = 0.968504;
            //  else if (pt >= 30 && pt < 50) num = 0.967916;
            //  else if (pt >= 50 && pt < 80) num = 0.969153;
            //  else if (pt >= 80 && pt < 9999) num = 0.98622;
            //}
        }
        else if (abseta >= 2.1 && abseta < 2.4) {
            if (cent >= 0 && cent < 20) {
                if (pt >= 15 && pt < 20) num = 0.814721;
                else if (pt >= 20 && pt < 30) num = 0.825021;
                else if (pt >= 30 && pt < 50) num = 0.865798;
                else if (pt >= 50 && pt < 9999) num = 0.886639;
            }
            //else if (cent >= 20 && cent < 100) {
            //  if (pt >= 15 && pt < 20) num = 0.958064;
            //  else if (pt >= 20 && pt < 30) num = 0.971345;
            //  else if (pt >= 30 && pt < 50) num = 0.966718;
            //  else if (pt >= 50 && pt < 9999) num = 0.966004;
            //}
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
            //else if (cent >= 20 && cent < 100) {
            //  if (pt >= 15 && pt < 20) num = 0.972041;
            //  else if (pt >= 20 && pt < 30) num = 0.964027;
            //  else if (pt >= 30 && pt < 50) num = 0.967845;
            //  else if (pt >= 50 && pt < 80) num = 0.966513;
            //  else if (pt >= 80 && pt < 9999) num = 0.931614;
            //}
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
            //else if (cent >= 20 && cent < 100) {
            //  if (pt >= 15 && pt < 20) num = 0.959975;
            //  else if (pt >= 20 && pt < 30) num = 0.964239;
            //  else if (pt >= 30 && pt < 50) num = 0.966221;
            //  else if (pt >= 50 && pt < 80) num = 0.967005;
            //  else if (pt >= 80 && pt < 9999) num = 0.979791;
            //}
        }
        else if (abseta >= 2.1 && abseta < 2.4) {
            if (cent >= 0 && cent < 20) {
                if (pt >= 15 && pt < 20) num = 0.770573;
                else if (pt >= 20 && pt < 30) num = 0.8015;
                else if (pt >= 30 && pt < 50) num = 0.856638;
                else if (pt >= 50 && pt < 9999) num = 0.881766;
            }
            //else if (cent >= 20 && cent < 100) {
            //  if (pt >= 15 && pt < 20) num = 0.921617;
            //  else if (pt >= 20 && pt < 30) num = 0.966269;
            //  else if (pt >= 30 && pt < 50) num = 0.962647;
            //  else if (pt >= 50 && pt < 9999) num = 0.956067;
            //}
        }
    }

    if (idx == 200) den = 1.0;
    if (idx == 300) num = den * den;

    return (num / den);
}

#endif //#ifndef muons_2018pbpb_tnp_weight_h
