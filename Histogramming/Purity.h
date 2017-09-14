#ifndef _PURITY_H
#define _PURITY_H

#include "TH1.h"
#include "TNtuple.h"
#include "TCut.h"
#include "TF1.h"

#include <string>

#include "../Utilities/interface/CutConfigurationParser.h"

class Purity;

class Templates {
    friend class Purity;

  public:
    Templates(TH1D* hdata, TH1D* hsig, TH1D* hbkg, int divisions);

    ~Templates() {
        data_hist->Delete();

        sig_hist->Delete();
        bkg_hist->Delete();
    };

    double evaluate(double* x, double* par);

  private:
    TH1D* data_hist;

    TH1D* sig_hist;
    TH1D* bkg_hist;

    int divisions;

    double getentries(double xx, TH1D* hist);
};

Templates::Templates(TH1D* hdata, TH1D* hsig, TH1D* hbkg, int divisions) {
    data_hist = (TH1D*)hdata->Clone(Form("%s_template", hdata->GetName()));

    sig_hist = (TH1D*)hsig->Clone(Form("%s_template", hsig->GetName()));
    bkg_hist = (TH1D*)hbkg->Clone(Form("%s_template", hbkg->GetName()));

    sig_hist->Scale(1. / sig_hist->Integral(1, sig_hist->GetNbinsX()));
    bkg_hist->Scale(1. / bkg_hist->Integral(1, bkg_hist->GetNbinsX()));

    this->divisions = divisions;
}

double Templates::evaluate(double* x, double* par) {
    double xx = x[0];

    float bkg_entries = getentries(xx, bkg_hist);
    float sig_entries = getentries(xx + par[2], sig_hist);
    return par[0] * (sig_entries * par[1] + bkg_entries * (1 - par[1]));
}

double Templates::getentries(double xx, TH1D* hist) {
    int bin = hist->FindBin(xx);

    int bin_low = bin - divisions / 2;
    int bin_high = bin + divisions - divisions / 2 - 1;

    if (bin_low != bin_high) {
        printf("xx: %f, bin_low: %i, bin_high: %i\n", xx, bin_low, bin_high);
    }

    return hist->Integral(bin_low, bin_high);
}

class Purity {
  public:
    double nentries;
    double nentrieserr;
    double ratio;
    double ratioerr;
    double shift;
    double shifterr;

    double chisq;
    int ndf;

    double nsig;
    double nsigerr;
    double purity;

    TH1D* hdata;
    TH1D* hsig_fit;
    TH1D* hbkg_fit;
    TH1D* htotal_fit;

    void fit(Templates* templates, TH1D* hdata, float range_low, float range_high, float sieie_cut);
    void write();

  private:
    void get_parameters(TF1* f);
    void construct(Templates* templates, float sieie_cut);
};

void Purity::fit(Templates* templates, TH1D* hdata, float range_low, float range_high, float sieie_cut) {
    TH1D* data_copy = (TH1D*)hdata->Clone(Form("%s_copy", hdata->GetName()));

    TF1* f = new TF1("f", templates, &Templates::evaluate, range_low, range_high, 3);
    f->SetParameters(data_copy->Integral(1, data_copy->GetNbinsX()), 0.72, 0.0);
    f->SetParLimits(1, 0, 1);
    f->FixParameter(2, 0.0);
    // f->SetParLimits(2, -0.0001, 0.0001);

    data_copy->Fit("f", "WL 0 Q", "", range_low, range_high);
    data_copy->Fit("f", "WL 0 Q", "", range_low, range_high);
    data_copy->Fit("f", "WL M 0 Q", "", range_low, range_high);

    get_parameters(f);
    construct(templates, sieie_cut);
}

void Purity::get_parameters(TF1* f) {
    nentries = f->GetParameter(0);
    ratio = f->GetParameter(1);
    shift = f->GetParameter(2);

    nentrieserr = f->GetParError(0);
    ratioerr = f->GetParError(1);
    shifterr = f->GetParError(2);

    nsig = nentries * ratio;
    nsigerr = nsig * sqrt(ratioerr / ratio * ratioerr / ratio + nentrieserr / nentries * nentrieserr / nentries);

    chisq = f->GetChisquare();
    ndf = f->GetNDF();
}

void Purity::construct(Templates* templates, float sieie_cut) {
    hdata = (TH1D*)templates->data_hist->Clone(Form("%s_fit", templates->data_hist->GetName()));

    hsig_fit = (TH1D*)templates->sig_hist->Clone(Form("%s_sig", hdata->GetName()));
    hbkg_fit = (TH1D*)templates->bkg_hist->Clone(Form("%s_bkg", hdata->GetName()));

    hsig_fit->Scale(nsig / hsig_fit->Integral(1, hsig_fit->GetNbinsX()));
    hbkg_fit->Scale((nentries - nsig) / hbkg_fit->Integral(1, hbkg_fit->GetNbinsX()));

    double signal = hsig_fit->Integral(1, hsig_fit->FindBin(sieie_cut));
    double background = hbkg_fit->Integral(1, hbkg_fit->FindBin(sieie_cut));
    purity = signal / (signal + background);

    htotal_fit = (TH1D*)hsig_fit->Clone(Form("%s_total", hdata->GetName()));
    htotal_fit->Add(hbkg_fit);
}

void Purity::write() {
    if (!hdata) { return; }

    hdata->Write("", TObject::kOverwrite);
    hsig_fit->Write("", TObject::kOverwrite);
    hbkg_fit->Write("", TObject::kOverwrite);
    htotal_fit->Write("", TObject::kOverwrite);
}

#endif
