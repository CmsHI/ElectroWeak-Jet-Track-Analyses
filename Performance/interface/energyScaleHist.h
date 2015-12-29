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

class energyScaleHist {
public :
    energyScaleHist(){
        range_eta[0] = 0;
        range_eta[1] = -1;  // no upper bound

        range_genPt[0] = 0;
        range_genPt[1] = -1;  // no upper bound

        range_recoPt[0] = 0;
        range_recoPt[1] = -1; // no upper bound

        range_hiBin[0] = 0;
        range_hiBin[1] = -1; // no upper bound
    };
    ~energyScaleHist(){};

    void Fillh2D(double energyScale, double x, float eta, float genPt, float recoPt, int hiBin)
    {
        if(range_eta[0] <= TMath::Abs(eta) && (range_eta[1] == -1    || range_eta[1] > TMath::Abs(eta)))
        if(range_genPt[0] <= genPt         && (range_genPt[1] == -1  || range_genPt[1] > genPt))
        if(range_recoPt[0] <= recoPt       && (range_recoPt[1] == -1 || range_recoPt[1] > recoPt))
        if(range_hiBin[0] <= hiBin         && (range_hiBin[1] == -1  || range_hiBin[1] > hiBin))
            h2D->Fill(x, energyScale);
    }

    TH2D* h2D;
    TH1D* h1D[2];       // h1D[0] = energy scale histogram
                        // h1D[1] = energy resolution histogram

    std::string name;   // this is basically histogram name excluding the "h1D"/"h2D" prefix
    std::string title;

    // range of oberservables for which the histograms are made.
    // histograms are filled if range[0] <= observable < range[1]
    float range_eta[2];
    float range_genPt[2];
    float range_recoPt[2];
    int   range_hiBin[2];
};

#endif /* PERFORMANCE_INTERFACE_ENERGYSCALEHIST_H_ */
