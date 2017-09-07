#ifndef _PURITY_H
#define _PURITY_H

#include "TH1.h"
#include "TNtuple.h"
#include "TCut.h"
#include "TF1.h"

#include <string>

#include "../Utilities/interface/CutConfigurationParser.h"

class Purity {
  public:
    double nsig;
    double nsigerr;
    double ratio;
    double ratioerr;
    double sigshift;
    double sigshifterr;
    double chisq;
    int ndf;

    double purity;

    TH1D* sig_fit;
    TH1D* bkg_fit;
    TH1D* data;
};

class Templates {
  public:
    Templates(TH1D* hdata, TH1D* hsig, TH1D* hbkg);
    Templates(TH1D* hdata, TTree* tsig, TCut csig, TTree* tbkg, TCut cbkg);

    ~Templates() {
        data_hist->Delete();

        sig_hist->Delete();
        bkg_hist->Delete();
    };

    double evaluate_hist(double* x, double* par);
    double evaluate(double* x, double* par);

  private:
    TH1D* data_hist;

    TH1D* sig_hist;
    TH1D* bkg_hist;

    TTree* sig_tree;
    TTree* bkg_tree;
    TCut sig_cut;
    TCut bkg_cut;
    int sig_total;
    int bkg_total;
};

Templates::Templates(TH1D* hdata, TH1D* hsig, TH1D* hbkg) {
    data_hist = (TH1D*)hdata->Clone("data");

    sig_hist = (TH1D*)hsig->Clone("signal_template");
    bkg_hist = (TH1D*)hbkg->Clone("background_template");

    sig_hist->Scale(1. / sig_hist->Integral(1, sig_hist->GetNbinsX()));
    bkg_hist->Scale(1. / bkg_hist->Integral(1, bkg_hist->GetNbinsX()));
}

Templates::Templates(TH1D* hdata, TTree* tsig, TCut csig, TTree* tbkg, TCut cbkg) {
    data_hist = (TH1D*)hdata->Clone("data");

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

double Templates::evaluate_hist(double* x, double* par) {
    double xx = x[0];
    int bkg_bin = bkg_hist->FindBin(xx);
    int sig_bin = sig_hist->FindBin(xx);
    return par[0] * (sig_hist->GetBinContent(sig_bin) * par[1] + bkg_hist->GetBinContent(bkg_bin) * (1 - par[1]));
}

double Templates::evaluate(double* x, double* par) {
    double xx = x[0];
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
}

Purity calc_purity(CutConfiguration config, TTree* data_tree, TTree* mc_tree,
                         TCut candidate_cut, TCut sideband_cut, TCut signal_cut) {
    float sig_shift = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_puritySignalShift];
    float bkg_shift = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_purityBackgroundShift];
    const int nbins = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].i[CUTS::PHO::k_puritySieieBins];

    const std::string weight_string = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_monteCarloWeightLabel];
    TCut weight_label = weight_string.c_str();

    TH1D* hdata = new TH1D("hdata", "", nbins, 0, 0.025);
    TH1D* hbkg = (TH1D*)hdata->Clone("hbkg");
    TH1D* hsig = (TH1D*)hdata->Clone("hsig");

    data_tree->Project(hdata->GetName(), "phoSigmaIEtaIEta_2012[phoIdx]", candidate_cut, "");
    data_tree->Project(hbkg->GetName(), Form("phoSigmaIEtaIEta_2012[phoIdx] + %f", bkg_shift), sideband_cut, "");
    mc_tree->Project(hsig->GetName(), Form("phoSigmaIEtaIEta_2012[phoIdx] + %f", sig_shift), weight_label * signal_cut, "");

    std::cout << "data count: " << hdata->GetEntries() << std::endl;
    std::cout << "bkg count: " << hbkg->GetEntries() << std::endl;
    std::cout << "sig count: " << hsig->GetEntries() << std::endl;

    float range_low = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_purityFitLow];
    float range_high = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_purityFitHigh];
    float sieie_cut = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_puritySieieThreshold];

    /* fit using binned templates */
    Templates* hist_templates = new Templates(hdata, hsig, hbkg);
    /* fit using exact ranges */
    // Templates* templates = new Templates(hdata, mc_tree, signal_cut, data_tree, sideband_cut);

    TH1D* data_copy = (TH1D*)hdata->Clone(Form("%s_copy", hdata->GetName()));

    TF1* f = new TF1("f", hist_templates, &Templates::evaluate_hist, range_low, range_high, 3);
    f->SetParameters(data_copy->Integral(1, nbins), 0.72, 0.0);
    f->SetParLimits(1, 0, 1);
    f->FixParameter(2, 0.0);

    data_copy->Fit("f", "WL 0 Q", "", range_low, range_high);
    data_copy->Fit("f", "WL 0 Q", "", range_low, range_high);
    data_copy->Fit("f", "WL M 0 Q", "", range_low, range_high);

    Purity res;

    double nevents = f->GetParameter(0);
    double ratio = f->GetParameter(1);
    double shift = f->GetParameter(2);

    double nevents_error = f->GetParError(0);
    double ratio_error = f->GetParError(1);
    double shift_error = f->GetParError(2);

    res.nsig = nevents * ratio;
    res.nsigerr = res.nsig * sqrt(ratio_error / ratio * ratio_error / ratio + nevents_error / nevents * nevents_error / nevents);
    res.ratio = ratio;
    res.ratioerr = ratio_error;
    res.sigshift = shift;
    res.sigshifterr = shift_error;
    res.chisq = f->GetChisquare();
    res.ndf = f->GetNDF();

    TH1F* hsig_pdf = (TH1F*)hsig->Clone(Form("%s_fit", hsig->GetName()));
    hsig_pdf->Scale(res.nsig / hsig_pdf->Integral(1, nbins));
    TH1F* hbkg_pdf = (TH1F*)hbkg->Clone(Form("%s_fit", hbkg->GetName()));
    hbkg_pdf->Scale((nevents - res.nsig) / hbkg_pdf->Integral(1, nbins));

    double signal = hsig_pdf->Integral(1, hsig_pdf->FindBin(sieie_cut), "width");
    double background = hbkg_pdf->Integral(1, hbkg_pdf->FindBin(sieie_cut), "width");
    res.purity = signal / (signal + background);

    res.sig_fit = (TH1D*)hsig_pdf->Clone(Form("%s_sig", hsig->GetName()));
    res.bkg_fit = (TH1D*)hbkg_pdf->Clone(Form("%s_bck", hbkg->GetName()));
    res.data = (TH1D*)hdata->Clone(Form("%s_cand", hdata->GetName()));

    std::cout << "purity: " << res.purity << std::endl;
    std::cout << "nsig: " << res.nsig << std::endl;
    std::cout << "chisq: " << res.chisq << std::endl;
    std::cout << "ndf: " << res.ndf << std::endl;

    delete hsig;
    delete hbkg;
    delete hdata;

    return res;
}

#endif
