/*
 * utilities related to bosonJet analysis.
 */

#include <TH1.h>
#include <TH1D.h>
#include <TString.h>
#include <TMath.h>

#ifndef BOSONJETUTIL_H_
#define BOSONJETUTIL_H_

std::vector<double> rebinXjBins(std::vector<double> binsTH1x);
std::vector<double> rebinXjBinsPP(std::vector<double> binsTH1x);
std::vector<double> rebinDphiBins(std::vector<double> binsTH1x);
std::string makeHistTitle(bool isHI, std::string collisionName, float ptBinLow, float ptBinUp, int hiBinLow, int hiBinUp);

/*
 * rebin the bins in xjz or xjg correlation
 * returns a vector that should be used in TH1::Rebin() function
 */
std::vector<double> rebinXjBins(std::vector<double> binsTH1x)
{
    binsTH1x.erase(binsTH1x.begin() + 13, binsTH1x.begin() + 13 + 3);  // merge last 4 bins : 13,14,15,16
    binsTH1x.erase(binsTH1x.begin() + 10, binsTH1x.begin() + 10 + 2);    // merge bins      :10,11,12
    binsTH1x.erase(binsTH1x.begin() + 8, binsTH1x.begin() + 8 + 1);    // merge bins        : 8,9
    binsTH1x.erase(binsTH1x.begin() + 6, binsTH1x.begin() + 6 + 1);    // merge bins        : 6,7
    binsTH1x.erase(binsTH1x.begin() + 4, binsTH1x.begin() + 4 + 1);    // merge bins        : 4,5
    binsTH1x.erase(binsTH1x.begin() + 2, binsTH1x.begin() + 2 + 1);    // merge bins        : 2,3

    return binsTH1x;
}

/*
 * rebin the bins in xjz or xjg correlation
 * returns a vector that should be used in TH1::Rebin() function
 */
std::vector<double> rebinXjBinsPP(std::vector<double> binsTH1x)
{
    binsTH1x.erase(binsTH1x.begin() + 13, binsTH1x.begin() + 13 + 3);  // merge last 4 bins : 13,14,15,16
    binsTH1x.erase(binsTH1x.begin() + 9, binsTH1x.begin() + 9 + 3);    // merge bins        : 9,10,11,12
    binsTH1x.erase(binsTH1x.begin() + 6, binsTH1x.begin() + 6 + 2);    // merge bins        : 6,7,8
    binsTH1x.erase(binsTH1x.begin() + 4, binsTH1x.begin() + 4 + 1);    // merge bins        : 4,5
    binsTH1x.erase(binsTH1x.begin() + 2, binsTH1x.begin() + 2 + 1);    // merge bins        : 2,3

    return binsTH1x;
}



/*
 * rebin the bins in dphi correlation
 * returns a vector that should be used in TH1::Rebin() function
 */
std::vector<double> rebinDphiBins(std::vector<double> binsTH1x)
{
    binsTH1x.erase(binsTH1x.begin() + 17, binsTH1x.begin() + 17 + 1);    // merge bins : 17,18
    binsTH1x.erase(binsTH1x.begin() + 15, binsTH1x.begin() + 15 + 1);    // merge bins : 15,16
    binsTH1x.erase(binsTH1x.begin() + 12, binsTH1x.begin() + 12 + 2);    // merge bins : 12,13,14
    binsTH1x.erase(binsTH1x.begin() + 9, binsTH1x.begin() + 9 + 2);      // merge bins : 9,10,11
    binsTH1x.erase(binsTH1x.begin() + 5, binsTH1x.begin() + 5 + 3);      // merge bins : 5,6,7,8
    binsTH1x.erase(binsTH1x.begin() + 1, binsTH1x.begin() + 1 + 3);      // merge bins : 1,2,3,4

    return binsTH1x;
}

std::string makeHistTitle(bool isHI, std::string collisionName, float ptBinLow, float ptBinUp, int hiBinLow, int hiBinUp)
{
    std::string histoTitle;
    if (isHI) {
        histoTitle = Form("%s , %.0f < p^{Z}_{T} < %.0f GeV/c, %d-%d %% ",collisionName.c_str() , ptBinLow, ptBinUp, hiBinLow/2, hiBinUp/2);

        // special cases
        if (ptBinLow <= 0 && ptBinUp < 0 && hiBinLow <= 0 && hiBinUp >= 200 )   {
            histoTitle = Form("%s",collisionName.c_str() );
        }
        else if (ptBinLow <= 0 && ptBinUp < 0)   {
            histoTitle = Form("%s , %d-%d %%",collisionName.c_str() , hiBinLow/2, hiBinUp/2);
        }
        else if (ptBinUp < 0 && hiBinLow <= 0 && hiBinUp >= 200)   {
            histoTitle = Form("%s ,  p^{Z}_{T} > %.0f GeV/c",collisionName.c_str() , ptBinLow);
        }
        else if (hiBinLow <= 0 && hiBinUp >= 200)   {
            histoTitle = Form("%s , %.0f < p^{Z}_{T} < %.0f GeV/c",collisionName.c_str() , ptBinLow, ptBinUp);
        }
        else if (ptBinUp < 0) {
            histoTitle = Form("%s , p^{Z}_{T} > %.0f GeV/c, %d-%d %% ",collisionName.c_str() , ptBinLow, hiBinLow/2, hiBinUp/2);
        }
    }
    else {
        histoTitle = Form("%s , %.0f < p^{Z}_{T} < %.0f GeV/c",collisionName.c_str() , ptBinLow, ptBinUp);

        // special cases
        if (ptBinLow <= 0 && ptBinUp < 0)   {
            histoTitle = Form("%s",collisionName.c_str() );
        }
        else if (ptBinUp < 0) {
            histoTitle = Form("%s , p^{Z}_{T} > %.0f GeV/c", collisionName.c_str() , ptBinLow);
        }
    }

    return histoTitle;
}

#endif /* BOSONJETUTIL_H_ */

