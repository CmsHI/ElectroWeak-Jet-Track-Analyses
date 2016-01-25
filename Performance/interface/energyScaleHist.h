/*
 * general purpose class for energy scale histograms
 */

#ifndef PERFORMANCE_INTERFACE_ENERGYSCALEHIST_H_
#define PERFORMANCE_INTERFACE_ENERGYSCALEHIST_H_

#include <TH1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TMath.h>

#include <string>

namespace ENERGYSCALE {  // energy scale

enum ENERGYSCALE_DEP {   // energy scale dependencies
    kETA,
    kGENPT,
    kRECOPT,
    kHIBIN,
    kN_ENERGYSCALE_DEP
};

std::string ENERGYSCALE_DEP_LABELS[kN_ENERGYSCALE_DEP] = {
        "ETA",
        "GENPT",
        "RECOPT",
        "HIBIN"
};

};

class energyScaleHist {
public :
    energyScaleHist(){

        for (int i=0; i<ENERGYSCALE::kN_ENERGYSCALE_DEP; ++i) {
            ranges[i][0] = 0;
            ranges[i][1] = -1;  // no upper bound
        }

        h2Dinitialized = false;
        hInitialized = false;
        h2DcorrInitialized = false;
    };
    ~energyScaleHist(){};

    void FillH2D(double energyScale, double x, float eta = -999, float genPt = -1, float recoPt = -1, int hiBin = -1)
    {
        // make sure to fill the histogram if no explicit kinematic range is specified.
        if (eta == -999)  eta = ranges[ENERGYSCALE::kETA][0];
        if (genPt == -1) genPt = ranges[ENERGYSCALE::kGENPT][0];
        if (recoPt == -1)  recoPt = ranges[ENERGYSCALE::kRECOPT][0];
        if (hiBin == -1)  hiBin = ranges[ENERGYSCALE::kHIBIN][0];

        if(ranges[ENERGYSCALE::kETA][0] <= TMath::Abs(eta) &&
           (ranges[ENERGYSCALE::kETA][1] == -1 || ranges[ENERGYSCALE::kETA][1] > TMath::Abs(eta))){
        if(ranges[ENERGYSCALE::kGENPT][0] <= genPt         &&
           (ranges[ENERGYSCALE::kGENPT][1] == -1  || ranges[ENERGYSCALE::kGENPT][1] > genPt)) {
        if(ranges[ENERGYSCALE::kRECOPT][0] <= recoPt       &&
           (ranges[ENERGYSCALE::kRECOPT][1] == -1 || ranges[ENERGYSCALE::kRECOPT][1] > recoPt)) {
        if(ranges[ENERGYSCALE::kHIBIN][0] <= hiBin         &&
           (ranges[ENERGYSCALE::kHIBIN][1] == -1  || ranges[ENERGYSCALE::kHIBIN][1] > hiBin)) {
                h2D->Fill(x, energyScale);
        }}}}
    }

    void FillH(double energyScale, float eta = -999, float genPt = -1, float recoPt = -1, int hiBin = -1)
    {
        // make sure to fill the histogram if no explicit kinematic range is specified.
        if (eta == -999)  eta = ranges[ENERGYSCALE::kETA][0];
        if (genPt == -1) genPt = ranges[ENERGYSCALE::kGENPT][0];
        if (recoPt == -1)  recoPt = ranges[ENERGYSCALE::kRECOPT][0];
        if (hiBin == -1)  hiBin = ranges[ENERGYSCALE::kHIBIN][0];

        if(ranges[ENERGYSCALE::kETA][0] <= TMath::Abs(eta) &&
           (ranges[ENERGYSCALE::kETA][1] == -1 || ranges[ENERGYSCALE::kETA][1] > TMath::Abs(eta))){
        if(ranges[ENERGYSCALE::kGENPT][0] <= genPt         &&
           (ranges[ENERGYSCALE::kGENPT][1] == -1  || ranges[ENERGYSCALE::kGENPT][1] > genPt)) {
        if(ranges[ENERGYSCALE::kRECOPT][0] <= recoPt       &&
           (ranges[ENERGYSCALE::kRECOPT][1] == -1 || ranges[ENERGYSCALE::kRECOPT][1] > recoPt)) {
        if(ranges[ENERGYSCALE::kHIBIN][0] <= hiBin         &&
           (ranges[ENERGYSCALE::kHIBIN][1] == -1  || ranges[ENERGYSCALE::kHIBIN][1] > hiBin)) {
                h->Fill(energyScale);
        }}}}
    }

    void FillH2Dcorr(float genPt, float recoPt, float eta = -999, int hiBin = -1)
    {
        // make sure to fill the histogram if no explicit kinematic range is specified.
        if (eta == -999)  eta = ranges[ENERGYSCALE::kETA][0];
        if (hiBin == -1)  hiBin = ranges[ENERGYSCALE::kHIBIN][0];

        if(ranges[ENERGYSCALE::kETA][0] <= TMath::Abs(eta) &&
           (ranges[ENERGYSCALE::kETA][1] == -1 || ranges[ENERGYSCALE::kETA][1] > TMath::Abs(eta))){
        if(ranges[ENERGYSCALE::kHIBIN][0] <= hiBin         &&
           (ranges[ENERGYSCALE::kHIBIN][1] == -1  || ranges[ENERGYSCALE::kHIBIN][1] > hiBin)) {
                h2Dcorr->Fill(genPt, recoPt);
        }}
    }

    /*
     * prepare the object title using the given ranges
     */
    void prepareTitle() {
        // prepare histogram title
        std::string etaStr  = "";       // whole detector
        std::string genPtStr  = "";     // whole pT range
        std::string recoPtStr = "";     // whole pT range
        std::string hiBinStr  = "";     // whole centrality range

        // special cases
        if (ranges[ENERGYSCALE::kETA][0] <= 0 && ranges[ENERGYSCALE::kETA][1] > 0)
            etaStr  = Form("|#eta|<%.2f", ranges[ENERGYSCALE::kETA][1]);
        else if (ranges[ENERGYSCALE::kETA][0] > 0 && ranges[ENERGYSCALE::kETA][1] > 0)
            etaStr  = Form("%.2f<|#eta|<%.2f", ranges[ENERGYSCALE::kETA][0], ranges[ENERGYSCALE::kETA][1]);

        if (ranges[ENERGYSCALE::kGENPT][0] > 0 && ranges[ENERGYSCALE::kGENPT][1] <= -1)
            genPtStr  = Form("p_{T}^{GEN}>%.0f", ranges[ENERGYSCALE::kGENPT][0]);
        else if (ranges[ENERGYSCALE::kGENPT][0] > 0 && ranges[ENERGYSCALE::kGENPT][1] > 0)
            genPtStr  = Form("%.0f<p_{T}^{GEN}<%.0f", ranges[ENERGYSCALE::kGENPT][0], ranges[ENERGYSCALE::kGENPT][1]);

        if (ranges[ENERGYSCALE::kRECOPT][0] > 0 && ranges[ENERGYSCALE::kRECOPT][1] <= -1)
            genPtStr  = Form("p_{T}^{RECO}>%.0f", ranges[ENERGYSCALE::kRECOPT][0]);
        else if (ranges[ENERGYSCALE::kRECOPT][0] > 0 && ranges[ENERGYSCALE::kRECOPT][1] > 0)
            recoPtStr = Form("%.0f<p_{T}^{RECO}<%.0f", ranges[ENERGYSCALE::kRECOPT][0], ranges[ENERGYSCALE::kRECOPT][1]);

        if (ranges[ENERGYSCALE::kHIBIN][0] <= 0 && ranges[ENERGYSCALE::kHIBIN][1] <= -1)
            hiBinStr = "";
        else if (ranges[ENERGYSCALE::kHIBIN][0] >= 0 && ranges[ENERGYSCALE::kHIBIN][1] > 0)
            hiBinStr  = Form("hiBin:%.0f-%.0f", ranges[ENERGYSCALE::kHIBIN][0], ranges[ENERGYSCALE::kHIBIN][1]);

        std::string tmpHistTitle = "";
        if (etaStr.size() > 0)  tmpHistTitle.append(Form("%s", etaStr.c_str()));
        if (genPtStr.size() > 0)  tmpHistTitle.append(Form(" %s", genPtStr.c_str()));
        if (recoPtStr.size() > 0) tmpHistTitle.append(Form(" %s", recoPtStr.c_str()));
        if (hiBinStr.size() > 0)  tmpHistTitle.append(Form(" %s", hiBinStr.c_str()));

        title = tmpHistTitle.c_str();
        if(h2Dinitialized) {
            h2D->SetTitle(title.c_str());
            titleX = h2D->GetXaxis()->GetTitle();
        }
        if (hInitialized) {
            h->SetTitle(title.c_str());
        }
        if(h2DcorrInitialized) {
            h2Dcorr->SetTitle(title.c_str());
        }
    }

    TH2D* h2D;
    TH1D* h1D[2];       // h1D[0] = energy scale histogram
                        // h1D[1] = energy resolution histogram

    TH1D* h;            // energy scale distribution
    TH2D* h2Dcorr;      // reco pt vs. gen pt correlation histogram.

    bool h2Dinitialized;
    bool hInitialized;
    bool h2DcorrInitialized;

    std::string name;   // this is basically histogram name excluding the "h1D"/"h2D" prefix
    std::string title;
    std::string titleX;

    // range of oberservables for which the histograms are made.
    // histograms are filled if range[i][0] <= observable < range[i][1]
    float ranges[ENERGYSCALE::kN_ENERGYSCALE_DEP][2];
};

#endif /* PERFORMANCE_INTERFACE_ENERGYSCALEHIST_H_ */
