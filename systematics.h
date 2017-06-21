#ifndef _SYSTEMATICS_H
#define _SYSTEMATICS_H

#include "TH1.h"
#include "TF1.h"
#include "TMath.h"

#include <string>

void th1_abs(TH1F* h) {
    for (int i=1; i<=h->GetNbinsX(); ++i)
        h->SetBinContent(i, TMath::Abs(h->GetBinContent(i)));
}

void th1_ratio_abs(TH1F* h) {
    for (int i=1; i<=h->GetNbinsX(); ++i) {
        if (h->GetBinContent(i) != 0) {
            h->SetBinContent(i, TMath::Abs(h->GetBinContent(i) - 1));
            h->SetBinError(i, h->GetBinError(i));
        } else {
            h->SetBinContent(i, 0);
            h->SetBinError(i, 0);
        }
    }
}

void th1_sqrt_sum_squares(TH1F* h1, TH1F* h2) {
    for (int i=1; i<=h1->GetNbinsX(); ++i) {
        double s1 = h1->GetBinContent(i);
        double s2 = h2->GetBinContent(i);
        double s_total = TMath::Sqrt(s1 * s1 + s2 * s2);

        double e1 = h1->GetBinError(i);
        double e2 = h2->GetBinError(i);
        double e_total = TMath::Sqrt(e1 * e1 + e2 * e2);

        h1->SetBinContent(i, s_total);
        h1->SetBinError(i, e_total);
    }
}

void th1_from_tf1(TH1F* h, TF1* f) {
    for (int i=1; i<=h->GetNbinsX(); ++i)
        if (h->GetBinContent(i) != 0)
            h->SetBinContent(i, f->Eval(h->GetBinCenter(i)));
}

void th1_max_of_2_th1(TH1F* h1, TH1F* h2, TH1F* h) {
    for (int i=1; i<=h1->GetNbinsX(); ++i) {
        if (h1->GetBinContent(i) > h2->GetBinContent(i)) {
            h->SetBinContent(i, h1->GetBinContent(i));
            h->SetBinError(i, h1->GetBinError(i));
        } else {
            h->SetBinContent(i, h2->GetBinContent(i));
            h->SetBinError(i, h2->GetBinError(i));
        }
    }
}

void th1_rebin(TH1F* h1, TH1F* hrebin) {
    int irebin = 1;
    for (int i=1; i<=h1->GetNbinsX(); ++i) {
        float rebin_factor = h1->GetBinWidth(i) / hrebin->GetBinWidth(i);
        h1->SetBinContent(i, rebin_factor * hrebin->GetBinContent(irebin));
        h1->SetBinError(i, rebin_factor * hrebin->GetBinContent(irebin));
        if (h1->GetBinLowEdge(i + 1) == hrebin->GetBinLowEdge(irebin + 1))
            ++irebin;
    }
}

class sys_var_t {
friend class total_sys_var_t;

private:
    std::string label = "";
    std::string type = "";

    std::string hist_name = "";

    TH1F* hnominal = 0;
    TH1F* hvariation = 0;

    TH1F* hdiff = 0;
    TH1F* hdiff_abs = 0;
    TH1F* hratio = 0;
    TH1F* hratio_abs = 0;

    TF1* fdiff = 0;
    TF1* fratio = 0;
    TH1F* hdiff_abs_fit = 0;
    TH1F* hratio_abs_fit = 0;

    TH1F* hnominal_rebin = 0;
    TH1F* hvariation_rebin = 0;

    TH1F* hdiff_rebin = 0;
    TH1F* hdiff_abs_rebin = 0;
    TH1F* hratio_rebin = 0;
    TH1F* hratio_abs_rebin = 0;

public:
    sys_var_t(const sys_var_t& sys_var);
    sys_var_t(std::string label, std::string type, TH1F* hnominal, TH1F* hvariation);
    sys_var_t(sys_var_t* sys_var1, sys_var_t* sys_var2);
    ~sys_var_t();

    void calc_sys();
    void calc_sys(int nbins, double* binning);
    void calc_sys(TH1F* hnominal_new, TH1F* hvariation_new);

    void scale_sys(double scale_factor);
    void fit_sys(std::string diff_fit_func, std::string ratio_fit_func);
    void print_latex();
    void write();

    TH1F* get_diff() { return hdiff; }
    TH1F* get_diff_abs() { return hdiff_abs; }
    TH1F* get_ratio() { return hratio; }
    TH1F* get_ratio_abs() { return hratio_abs; }

    TH1F* get_diff_abs_rebin() { return hdiff_abs_rebin; }
    TH1F* get_ratio_abs_rebin() { return hratio_abs_rebin; }
};

sys_var_t::sys_var_t(const sys_var_t& sys_var) {
    label = sys_var.label;
    type = sys_var.type;
}

sys_var_t::sys_var_t(std::string label, std::string type, TH1F* hnominal, TH1F* hvariation) {
    this->label = label;
    this->type = type;
    this->hist_name = label + "_" + type;

    this->hnominal = (TH1F*)hnominal->Clone(Form("%s_nominal", hist_name.c_str()));
    this->hvariation = (TH1F*)hvariation->Clone(Form("%s_variation", hist_name.c_str()));
}

sys_var_t::sys_var_t(sys_var_t* sys_var1, sys_var_t* sys_var2) {
    this->label = sys_var1->label;
    this->type = sys_var1->type + "_plus_" + sys_var2->type;
    this->hist_name = this->label + "_" + this->type;

    this->hdiff_abs = (TH1F*)sys_var1->hdiff_abs->Clone(Form("%s_diff_abs", this->hist_name.c_str()));
    th1_max_of_2_th1(sys_var1->hdiff_abs, sys_var2->hdiff_abs, this->hdiff_abs);
    this->hratio_abs = (TH1F*)sys_var1->hratio_abs->Clone(Form("%s_ratio_abs", this->hist_name.c_str()));
    th1_max_of_2_th1(sys_var1->hratio_abs, sys_var2->hratio_abs, this->hratio_abs);

    this->hnominal = (TH1F*)sys_var1->hnominal->Clone(Form("%s_nominal", this->hist_name.c_str()));
    this->hvariation = (TH1F*)this->hnominal->Clone(Form("%s_variation", this->hist_name.c_str()));
    this->hvariation->Add(this->hdiff_abs);
}

sys_var_t::~sys_var_t() {
    hnominal->Delete();
    hvariation->Delete();

    hdiff->Delete();
    hdiff_abs->Delete();
    hratio->Delete();
    hratio_abs->Delete();

    fdiff->Delete();
    fratio->Delete();
    hdiff_abs_fit->Delete();
    hratio_abs_fit->Delete();

    hnominal_rebin->Delete();
    hvariation_rebin->Delete();

    hdiff_rebin->Delete();
    hdiff_abs_rebin->Delete();
    hratio_rebin->Delete();
    hratio_abs_rebin->Delete();
}

void sys_var_t::calc_sys() {
    hdiff = (TH1F*)hvariation->Clone(Form("%s_diff", hist_name.c_str()));
    hdiff->Add(hnominal, -1);
    hdiff_abs = (TH1F*)hdiff->Clone(Form("%s_diff_abs", hist_name.c_str()));
    th1_abs(hdiff_abs);

    hratio = (TH1F*)hvariation->Clone(Form("%s_ratio", hist_name.c_str()));
    hratio->Divide(hvariation, hnominal);
    hratio_abs = (TH1F*)hratio->Clone(Form("%s_ratio_abs", hist_name.c_str()));
    th1_ratio_abs(hratio_abs);
}

void sys_var_t::calc_sys(int nbins, double* binning) {
    calc_sys();

    hnominal_rebin = (TH1F*)hnominal->Rebin(nbins, Form("%s_nominal_rebin", hist_name.c_str()), binning);
    hvariation_rebin = (TH1F*)hvariation->Rebin(nbins, Form("%s_variation_rebin", hist_name.c_str()), binning);

    hdiff_rebin = (TH1F*)hvariation_rebin->Clone(Form("%s_diff_rebin", hist_name.c_str()));
    hdiff_rebin->Add(hnominal_rebin, -1);
    hdiff_abs_rebin = (TH1F*)hdiff_rebin->Clone(Form("%s_diff_abs_rebin", hist_name.c_str()));
    th1_abs(hdiff_abs_rebin);

    hratio_rebin = (TH1F*)hvariation_rebin->Clone(Form("%s_ratio_rebin", hist_name.c_str()));
    hratio_rebin->Divide(hvariation_rebin, hnominal_rebin);
    hratio_abs_rebin = (TH1F*)hratio_rebin->Clone(Form("%s_ratio_abs_rebin", hist_name.c_str()));
    th1_ratio_abs(hratio_abs_rebin);

    th1_rebin(hdiff, hdiff_rebin);
    th1_rebin(hdiff_abs, hdiff_abs_rebin);
    th1_rebin(hratio, hratio_rebin);
    th1_rebin(hratio_abs, hratio_abs_rebin);
}

void sys_var_t::calc_sys(TH1F* hnominal_new, TH1F* hvariation_new) {
    hratio = (TH1F*)hvariation_new->Clone(Form("%s_ratio", hist_name.c_str()));
    hratio->Divide(hvariation_new, hnominal_new);

    hvariation->Delete();
    hvariation = (TH1F*)hnominal_new->Clone(Form("%s_variation", hist_name.c_str()));
    hvariation->Multiply(hratio);

    hdiff = (TH1F*)hvariation->Clone(Form("%s_diff", hist_name.c_str()));
    hdiff->Add(hnominal, -1);
    hdiff_abs = (TH1F*)hdiff->Clone(Form("%s_diff_abs", hist_name.c_str()));
    th1_abs(hdiff_abs);

    hratio_abs = (TH1F*)hratio->Clone(Form("%s_ratio_abs", hist_name.c_str()));
    th1_ratio_abs(hratio_abs);
}

void sys_var_t::scale_sys(double scale_factor) {
    hdiff_abs->Scale(scale_factor);
    hratio_abs->Scale(scale_factor);
}

void sys_var_t::fit_sys(std::string diff_fit_func, std::string ratio_fit_func) {
    double range_low = hnominal->GetBinLowEdge(hnominal->FindFirstBinAbove(0.1));
    double range_high = hnominal->GetBinLowEdge(hnominal->FindLastBinAbove(0.1) + 1);

    hdiff_abs_fit = (TH1F*)hdiff_abs->Clone(Form("%s_diff_abs_fit", hist_name.c_str()));
    TF1* diff_fit = new TF1(Form("%s_diff_fit_function", hist_name.c_str()), diff_fit_func.c_str());
    diff_fit->SetRange(range_low, range_high);

    hdiff_abs->Fit(Form("%s_diff_fit_function", hist_name.c_str()), "F Q", "", range_low, range_high);
    hdiff_abs->Fit(Form("%s_diff_fit_function", hist_name.c_str()), "F Q", "", range_low, range_high);
    hdiff_abs->Fit(Form("%s_diff_fit_function", hist_name.c_str()), "F M Q", "", range_low, range_high);
    fdiff = (TF1*)hdiff_abs->GetFunction(Form("%s_diff_fit_function", hist_name.c_str()))->Clone(Form("%s_diff_fit", hist_name.c_str()));
    th1_from_tf1(hdiff_abs_fit, fdiff);

    hratio_abs_fit = (TH1F*)hratio_abs->Clone(Form("%s_ratio_abs_fit", hist_name.c_str()));
    TF1* ratio_fit = new TF1(Form("%s_ratio_fit_function", hist_name.c_str()), ratio_fit_func.c_str());
    ratio_fit->SetRange(range_low, range_high);

    hratio_abs->Fit(Form("%s_ratio_fit_function", hist_name.c_str()), "F Q", "", range_low, range_high);
    hratio_abs->Fit(Form("%s_ratio_fit_function", hist_name.c_str()), "F Q", "", range_low, range_high);
    hratio_abs->Fit(Form("%s_ratio_fit_function", hist_name.c_str()), "F M Q", "", range_low, range_high);
    fratio = (TF1*)hratio_abs->GetFunction(Form("%s_ratio_fit_function", hist_name.c_str()))->Clone(Form("%s_ratio_fit", hist_name.c_str()));
    th1_from_tf1(hratio_abs_fit, fratio);
}

void sys_var_t::print_latex() {
    float average_sys = 0;
    int nonzero_bins = 0;
    for (int i=1; i<=hratio_abs->GetNbinsX(); ++i) {
        if (hratio_abs->GetBinContent(i) != 0) {
            average_sys += hratio_abs->GetBinContent(i);
            ++nonzero_bins;
        }
    }

    if (nonzero_bins) {
        average_sys /= nonzero_bins;
        printf(" & %5.1f%%", average_sys);
    }
}

void sys_var_t::write() {
    hnominal->Write("", TObject::kOverwrite);
    if (hvariation) hvariation->Write("", TObject::kOverwrite);

    if (hdiff) hdiff->Write("", TObject::kOverwrite);
    hdiff_abs->Write("", TObject::kOverwrite);
    if (hratio) hratio->Write("", TObject::kOverwrite);
    hratio_abs->Write("", TObject::kOverwrite);

    fdiff->Write("", TObject::kOverwrite);
    fratio->Write("", TObject::kOverwrite);
    hdiff_abs_fit->Write("", TObject::kOverwrite);
    hratio_abs_fit->Write("", TObject::kOverwrite);
}

class total_sys_var_t {
  private:
    std::string label = "";

    TH1F* hnominal = 0;
    TH1F* hsystematics = 0;

    void add_sqrt_sum_squares(TH1F* herr);

  public:
    total_sys_var_t(const total_sys_var_t& total_sys_var);
    total_sys_var_t(std::string label, TH1F* hnominal);
    ~total_sys_var_t();

    void add_sys_var(sys_var_t* sys_var, int option);
    void write();

    TH1F* get_total() { return hsystematics; }
};

total_sys_var_t::total_sys_var_t(const total_sys_var_t& total_sys_var) {
    label = total_sys_var.label;
}

total_sys_var_t::total_sys_var_t(std::string label, TH1F* hnominal) {
    this->label = label;
    this->hnominal = (TH1F*)hnominal->Clone(Form("%s_nominal", label.c_str()));
    this->hsystematics = (TH1F*)hnominal->Clone(Form("%s_systematics", label.c_str()));
    this->hsystematics->Reset("ICES");
}

total_sys_var_t::~total_sys_var_t() {};

void total_sys_var_t::add_sqrt_sum_squares(TH1F* herr) {
    th1_sqrt_sum_squares(hsystematics, herr);
}

void total_sys_var_t::add_sys_var(sys_var_t* sys_var, int option) {
    switch (option) {
        case 0:
            add_sqrt_sum_squares(sys_var->hdiff_abs);
            break;
        case 1: {
            TH1F* htmp = (TH1F*)sys_var->hratio_abs->Clone("htmp");
            htmp->Multiply(hnominal);
            add_sqrt_sum_squares(htmp);
            htmp->Delete();
            break; }
        case 2:
            if (!sys_var->hdiff_abs_fit) {printf("no fit found!\n"); return;}
            add_sqrt_sum_squares(sys_var->hdiff_abs_fit);
            break;
        case 3: {
            if (!sys_var->hratio_abs_fit) {printf("no fit found!\n"); return;}
            TH1F* htmp = (TH1F*)sys_var->hratio_abs_fit->Clone("htmp");
            htmp->Multiply(hnominal);
            add_sqrt_sum_squares(htmp);
            htmp->Delete();
            break; }
        case 4:
            add_sqrt_sum_squares(sys_var->hvariation);
            break;
        case 5: {
            TH1F* htmp = (TH1F*)sys_var->hvariation->Clone("htmp");
            htmp->Multiply(hnominal);
            add_sqrt_sum_squares(htmp);
            htmp->Delete();
            break; }
        case 6:
        default:
            break;
    }
}

void total_sys_var_t::write() {
    hnominal->Write("", TObject::kOverwrite);
    hsystematics->Write("", TObject::kOverwrite);
}

#endif
