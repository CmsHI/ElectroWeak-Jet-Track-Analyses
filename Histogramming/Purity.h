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
    Templates(TH1D* hdata, TH1D* hsig, TH1D* hbkg);
    Templates(TH1D* hdata, TTree* tsig, TCut csig, TTree* tbkg, TCut cbkg);

    ~Templates() {
        data_hist->Delete();

        sig_hist->Delete();
        bkg_hist->Delete();
    };

    int type() { return fit_type; };

    double evaluate(double* x, double* par);

  private:
    int fit_type;

    TH1D* data_hist;

    TH1D* sig_hist;
    TH1D* bkg_hist;

    TTree* data_tree;

    TTree* sig_tree;
    TTree* bkg_tree;
    TCut sig_cut;
    TCut bkg_cut;
    int sig_total;
    int bkg_total;
};

Templates::Templates(TH1D* hdata, TH1D* hsig, TH1D* hbkg) {
    fit_type = 0;

    data_hist = (TH1D*)hdata->Clone(Form("%s_template", hdata->GetName()));

    sig_hist = (TH1D*)hsig->Clone(Form("%s_template", hsig->GetName()));
    bkg_hist = (TH1D*)hbkg->Clone(Form("%s_template", hbkg->GetName()));

    sig_hist->Scale(1. / sig_hist->Integral(1, sig_hist->GetNbinsX()));
    bkg_hist->Scale(1. / bkg_hist->Integral(1, bkg_hist->GetNbinsX()));
}

Templates::Templates(TH1D* hdata, TTree* tsig, TCut csig, TTree* tbkg, TCut cbkg) {
    fit_type = 1;

    data_hist = (TH1D*)hdata->Clone(Form("%s_template", hdata->GetName()));

    data_tree = tbkg;

    sig_tree = tsig;
    bkg_tree = tbkg;
    sig_cut = csig;
    bkg_cut = cbkg;

    sig_total = tsig->GetEntries(
        Form("(%s) && (phoSigmaIEtaIEta_2012[phoIdx]>0.0 && phoSigmaIEtaIEta_2012[phoIdx]<0.025)",
            sig_cut.GetTitle())
    );
    bkg_total = tbkg->GetEntries(
        Form("(%s) && (phoSigmaIEtaIEta_2012[phoIdx]>0.0 && phoSigmaIEtaIEta_2012[phoIdx]<0.025)",
            bkg_cut.GetTitle())
    );
}

double Templates::evaluate(double* x, double* par) {
    double xx = x[0];

    switch (fit_type) {
        case 0: {
            int bkg_bin = bkg_hist->FindBin(xx);
            int sig_bin = sig_hist->FindBin(xx);
            return par[0] * (sig_hist->GetBinContent(sig_bin) * par[1] + bkg_hist->GetBinContent(bkg_bin) * (1 - par[1]));
            break; }
        case 1: {
            float half_bin_width = data_hist->GetBinWidth(data_hist->FindBin(xx));

            int bkg_entries = bkg_tree->GetEntries(
                Form("(%s) && (phoSigmaIEtaIEta_2012[phoIdx]>%f && phoSigmaIEtaIEta_2012[phoIdx]<%f)",
                    bkg_cut.GetTitle(), xx - half_bin_width, xx + half_bin_width)
            ) / bkg_total;
            int sig_entries = sig_tree->GetEntries(
                Form("(%s) && (phoSigmaIEtaIEta_2012[phoIdx]>%f && phoSigmaIEtaIEta_2012[phoIdx]<%f)",
                    sig_cut.GetTitle(), xx + par[2] - half_bin_width, xx + par[2] + half_bin_width)
            ) / sig_total;
            return par[0] * (sig_entries * par[1] + bkg_entries * (1 - par[1]));
            break; }
    }
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

    void fit(Templates* templates, TH1D* hdata, float range_low, float range_high, float sieie_cut);

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

    switch (templates->fit_type) {
        case 0:
            hsig_fit = (TH1D*)templates->sig_hist->Clone(Form("%s_fit", templates->sig_hist->GetName()));
            hbkg_fit = (TH1D*)templates->bkg_hist->Clone(Form("%s_fit", templates->bkg_hist->GetName()));
            break;
        case 1:
            hsig_fit = (TH1D*)templates->data_hist->Clone(Form("%s_fit_sig", hdata->GetName()));
            templates->sig_tree->Project(hsig_fit->GetName(), Form("phoSigmaIEtaIEta_2012[phoIdx] + %f", shift), templates->sig_cut, "");
            hbkg_fit = (TH1D*)templates->data_hist->Clone(Form("%s_fit_bkg", hdata->GetName()));
            templates->data_tree->Project(hbkg_fit->GetName(), Form("phoSigmaIEtaIEta_2012[phoIdx]"), templates->bkg_cut, "");
            break;
    }

    hsig_fit->Scale(nsig / hsig_fit->Integral(1, hsig_fit->GetNbinsX()));
    hbkg_fit->Scale((nentries - nsig) / hbkg_fit->Integral(1, hbkg_fit->GetNbinsX()));

    double signal = hsig_fit->Integral(1, hsig_fit->FindBin(sieie_cut), "width");
    double background = hbkg_fit->Integral(1, hbkg_fit->FindBin(sieie_cut), "width");
    purity = signal / (signal + background);
}

#endif
