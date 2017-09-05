#ifndef _SYSVAR_H
#define _SYSVAR_H

#include "TH1.h"
#include "TF1.h"
#include "TMath.h"

#include <string>
#include <map>

void TH1D_Abs(TH1D* h) {
    for (int i=1; i<=h->GetNbinsX(); ++i)
        h->SetBinContent(i, TMath::Abs(h->GetBinContent(i)));
}

void TH1D_AbsRatio(TH1D* h) {
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

void TH1D_Max(TH1D* h1, TH1D* h2) {
    for (int i=1; i<=h1->GetNbinsX(); ++i) {
        double sys1 = h1->GetBinContent(i);
        double err1 = h1->GetBinError(i);

        double sys2 = h2->GetBinContent(i);
        double err2 = h2->GetBinError(i);

        if (sys1 > sys2) {
            h1->SetBinContent(i, sys1);
            h1->SetBinError(i, err1);
        } else {
            h1->SetBinContent(i, sys2);
            h1->SetBinError(i, err2);
        }
    }
}

void TH1D_SqrtSumofSquares(TH1D* h1, TH1D* h2) {
    for (int i=1; i<=h1->GetNbinsX(); ++i) {
        double sys1 = h1->GetBinContent(i);
        double sys2 = h2->GetBinContent(i);
        double sys_total = TMath::Sqrt(sys1 * sys1 + sys2 * sys2);

        double err1 = h1->GetBinError(i);
        double err2 = h2->GetBinError(i);
        double err_total = TMath::Sqrt(err1 * err1 + err2 * err2);

        h1->SetBinContent(i, sys_total);
        h1->SetBinError(i, err_total);
    }
}

void TH1D_from_TF1(TH1D* h, TF1* f) {
    for (int i=1; i<=h->GetNbinsX(); ++i)
        h->SetBinContent(i, f->Eval(h->GetBinCenter(i)));
}

#define _N_REBIN 8
Double_t xjg_rebin[_N_REBIN + 1] = {0, 0.25, 0.5, 0.75, 1, 1.25, 1.5, 1.75, 2};

class TotalSysVar;

class SysVar {
friend class TotalSysVar;
public:
    SysVar(std::string hist_name, std::string sys_type) {
        this->hist_name = hist_name;
        this->sys_type = sys_type;
    }

    SysVar(const SysVar& sys) {
        hist_name = sys.hist_name;
        sys_type = sys.sys_type;

        h1D_nominal = sys.h1D_nominal;
        h1D_varied = sys.h1D_varied;
        h1D_diff = sys.h1D_diff;
        h1D_diff_abs = sys.h1D_diff_abs;
        h1D_ratio = sys.h1D_ratio;
        h1D_ratio_abs = sys.h1D_ratio_abs;

        ave_sys = sys.ave_sys;
    }

    ~SysVar() {};

    void clear() {
        h1D_nominal->Reset("ICES");
        h1D_varied->Reset("ICES");
        h1D_diff->Reset("ICES");
        h1D_diff_abs->Reset("ICES");
        h1D_ratio->Reset("ICES");
        h1D_ratio_abs->Reset("ICES");

        ave_sys = 0;
    }

    void init(TH1D* h1D_nominal, TH1D* h1D_varied) {
        this->h1D_nominal = (TH1D*)h1D_nominal->Clone(Form("h1D_%s_nominal_%s", hist_name.c_str(), sys_type.c_str()));
        this->h1D_varied = (TH1D*)h1D_varied->Clone(Form("h1D_%s_varied_%s", hist_name.c_str(), sys_type.c_str()));
    }

    void init_with_ratio(TH1D* h1D_nominal, TH1D* h1D_ratio) {
        this->h1D_nominal = (TH1D*)h1D_nominal->Clone(Form("h1D_%s_nominal_%s", hist_name.c_str(), sys_type.c_str()));
        this->h1D_varied = (TH1D*)h1D_nominal->Clone(Form("h1D_%s_varied_%s", hist_name.c_str(), sys_type.c_str()));
        this->h1D_varied->Multiply(h1D_ratio);
    }

    void calc_sys() {
        h1D_diff = (TH1D*)h1D_varied->Clone(Form("h1D_%s_diff_%s", hist_name.c_str(), sys_type.c_str()));
        h1D_diff->Add(h1D_nominal, -1);

        h1D_diff_abs = (TH1D*)h1D_diff->Clone(Form("h1D_%s_diff_abs_%s", hist_name.c_str(), sys_type.c_str()));
        TH1D_Abs(h1D_diff_abs);

        h1D_ratio = (TH1D*)h1D_varied->Clone(Form("h1D_%s_ratio_%s", hist_name.c_str(), sys_type.c_str()));
        h1D_ratio->Divide(h1D_nominal);

        h1D_ratio_abs = (TH1D*)h1D_ratio->Clone(Form("h1D_%s_ratio_abs_%s", hist_name.c_str(), sys_type.c_str()));
        TH1D_AbsRatio(h1D_ratio_abs);

        calc_average();
    }

    void rebin_and_calc_sys() {
        TH1D* h1D_nominal_rebin = (TH1D*)h1D_nominal->Rebin(_N_REBIN, Form("h1D_%s_nominal_%s_rebin", hist_name.c_str(), sys_type.c_str()), xjg_rebin);
        TH1D* h1D_varied_rebin = (TH1D*)h1D_varied->Rebin(_N_REBIN, Form("h1D_%s_varied_%s_rebin", hist_name.c_str(), sys_type.c_str()), xjg_rebin);

        TH1D* h1D_diff_rebin = (TH1D*)h1D_varied_rebin->Clone(Form("h1D_%s_diff_%s_rebin", hist_name.c_str(), sys_type.c_str()));
        h1D_diff_rebin->Add(h1D_nominal_rebin, -1);

        TH1D* h1D_diff_abs_rebin = (TH1D*)h1D_diff_rebin->Clone(Form("h1D_%s_diff_abs_%s_rebin", hist_name.c_str(), sys_type.c_str()));
        TH1D_Abs(h1D_diff_abs_rebin);

        TH1D* h1D_ratio_rebin = (TH1D*)h1D_varied_rebin->Clone(Form("h1D_%s_ratio_%s_rebin", hist_name.c_str(), sys_type.c_str()));
        h1D_ratio_rebin->Divide(h1D_nominal_rebin);

        TH1D* h1D_ratio_abs_rebin = (TH1D*)h1D_ratio_rebin->Clone(Form("h1D_%s_ratio_abs_%s_rebin", hist_name.c_str(), sys_type.c_str()));
        TH1D_AbsRatio(h1D_ratio_abs_rebin);

        h1D_diff = (TH1D*)h1D_varied->Clone(Form("h1D_%s_diff_%s", hist_name.c_str(), sys_type.c_str()));
        h1D_diff_abs = (TH1D*)h1D_diff->Clone(Form("h1D_%s_diff_abs_%s", hist_name.c_str(), sys_type.c_str()));
        h1D_ratio = (TH1D*)h1D_varied->Clone(Form("h1D_%s_ratio_%s", hist_name.c_str(), sys_type.c_str()));
        h1D_ratio_abs = (TH1D*)h1D_ratio->Clone(Form("h1D_%s_ratio_abs_%s", hist_name.c_str(), sys_type.c_str()));

        rebin_sys(h1D_diff, h1D_diff_rebin);
        rebin_sys(h1D_diff_abs, h1D_diff_abs_rebin);
        rebin_sys(h1D_ratio, h1D_ratio_rebin);
        rebin_sys(h1D_ratio_abs, h1D_ratio_abs_rebin);
    }

    // warning: no error checking done
    void rebin_sys(TH1D* h_orig, TH1D* h_rebinned) {
        int i_rebin = 1;
        for (int i=1; i<=h_orig->GetNbinsX(); ++i) {
            float rebin_factor = h_orig->GetBinWidth(i) / h_rebinned->GetBinWidth(i_rebin);
            h_orig->SetBinContent(i, rebin_factor * h_rebinned->GetBinContent(i_rebin));
            h_orig->SetBinError(i, rebin_factor * h_rebinned->GetBinContent(i_rebin));
            if (h_orig->GetBinLowEdge(i + 1) == h_rebinned->GetBinLowEdge(i_rebin + 1))
                ++i_rebin;
        }
    }

    void scale_sys(double scale_factor) {
        h1D_diff_abs->Scale(scale_factor);
        h1D_ratio_abs->Scale(scale_factor);
    }

    std::string get_hist_name() {return hist_name;}
    std::string get_sys_type() {return sys_type;}
    TH1D* get_ratio() {return h1D_ratio;}
    TH1D* get_diff() {return h1D_diff;}

private:
    std::string hist_name = "";
    std::string sys_type = "";

    TH1D* h1D_nominal = 0;
    TH1D* h1D_varied = 0;
    TH1D* h1D_diff = 0;
    TH1D* h1D_diff_abs = 0;
    TH1D* h1D_ratio = 0;
    TH1D* h1D_ratio_abs = 0;

    double ave_sys = 0;

    void calc_average() {
        int nonzero_bins = 0;
        for (int i=1; i<=h1D_ratio_abs->GetNbinsX(); ++i) {
            if (h1D_ratio_abs->GetBinContent(i) != 0) {
                ave_sys += h1D_ratio_abs->GetBinContent(i);
                ++nonzero_bins;
            }
        }
        if (nonzero_bins)
            ave_sys /= nonzero_bins;
    }
};

class TotalSysVar {
public:
    TotalSysVar() {};

    TotalSysVar(SysVar* up, SysVar* down) {
        init = true;

        hist_name = up->hist_name;
        sys_type = up->sys_type;

        SysVar_objects.push_back(up);
        SysVar_objects.push_back(down);

        h1D_diff = new TH1D();
        h1D_ratio = new TH1D();

        up->h1D_diff_abs->Copy(*h1D_diff);
        up->h1D_ratio_abs->Copy(*h1D_ratio);

        h1D_diff->SetName(Form("h1D_%s_diff_%s_%s_total", hist_name.c_str(), up->sys_type.c_str(), down->sys_type.c_str()));
        h1D_ratio->SetName(Form("h1D_%s_ratio_%s_%s_total", hist_name.c_str(), up->sys_type.c_str(), down->sys_type.c_str()));

        TH1D_Max(h1D_diff, down->h1D_diff_abs);
        TH1D_Max(h1D_ratio, down->h1D_ratio_abs);

        calc_average();
    }

    TotalSysVar(TotalSysVar* up, TotalSysVar* down) {
        init = true;

        hist_name = up->hist_name;
        sys_type = up->sys_type;

        // SysVar_objects.push_back(up);
        // SysVar_objects.push_back(down);

        h1D_diff = new TH1D();
        h1D_ratio = new TH1D();

        up->h1D_diff->Copy(*h1D_diff);
        up->h1D_ratio->Copy(*h1D_ratio);

        h1D_diff->SetName(Form("h1D_%s_diff_%s_%s_total", hist_name.c_str(), up->sys_type.c_str(), down->sys_type.c_str()));
        h1D_ratio->SetName(Form("h1D_%s_ratio_%s_%s_total", hist_name.c_str(), up->sys_type.c_str(), down->sys_type.c_str()));

        TH1D_Max(h1D_diff, down->h1D_diff);
        TH1D_Max(h1D_ratio, down->h1D_ratio);

        calc_average();
    }

    TotalSysVar(const TotalSysVar& sys) {
        init = sys.init;

        hist_name = sys.hist_name;

        TotalSysVar_objects = sys.TotalSysVar_objects;
        SysVar_objects = sys.SysVar_objects;

        h1D_diff = sys.h1D_diff;
        h1D_ratio = sys.h1D_ratio;

        ave_sys = sys.ave_sys;
    }

    ~TotalSysVar() {};

    void clear() {
        init = false;

        TotalSysVar_objects.clear();
        SysVar_objects.clear();

        h1D_diff->Reset("ICES");
        h1D_ratio->Reset("ICES");

        ave_sys = 0;
    }

    void add_SysVar(SysVar* sys) {
        if (!init) {
            init = true;

            hist_name = sys->hist_name;

            h1D_diff = new TH1D();
            h1D_ratio = new TH1D();

            sys->h1D_diff_abs->Copy(*h1D_diff);
            sys->h1D_ratio_abs->Copy(*h1D_ratio);

            h1D_diff->SetName(Form("h1D_%s_diff_total", hist_name.c_str()));
            h1D_ratio->SetName(Form("h1D_%s_ratio_total", hist_name.c_str()));
        } else {
            TH1D_SqrtSumofSquares(h1D_diff, sys->h1D_diff_abs);
            TH1D_SqrtSumofSquares(h1D_ratio, sys->h1D_ratio_abs);
        }

        SysVar_objects.push_back(sys);
    }

    void add_SysVar(TotalSysVar* sys) {
        if (!init) {
            init = true;

            hist_name = sys->hist_name;

            h1D_diff = new TH1D();
            h1D_ratio = new TH1D();

            sys->h1D_diff->Copy(*h1D_diff);
            sys->h1D_ratio->Copy(*h1D_ratio);

            h1D_diff->SetName(Form("h1D_%s_diff_total", hist_name.c_str()));
            h1D_ratio->SetName(Form("h1D_%s_ratio_total", hist_name.c_str()));
        } else {
            TH1D_SqrtSumofSquares(h1D_diff, sys->h1D_diff);
            TH1D_SqrtSumofSquares(h1D_ratio, sys->h1D_ratio);
        }

        TotalSysVar_objects.push_back(sys);
    }

    void print_latex(std::map<std::string, std::string> sys_names) {
        printf("\\begin{table}[hbtp]\n");
        printf("\\begin{center}\n");
        printf("\\begin{tabular}{|l|r|}\n");
        printf("\\hline\n");
        printf("Systematic Uncertainty   &        \\\n");
        printf("\\hline\n");
        for (size_t i=0; i<TotalSysVar_objects.size(); ++i)
            printf("%-24s & %5.1f%% \\\n", sys_names[TotalSysVar_objects[i]->sys_type].c_str(), TotalSysVar_objects[i]->ave_sys * 100);
        for (size_t i=0; i<SysVar_objects.size(); ++i)
            printf("%-24s & %5.1f%% \\\n", sys_names[SysVar_objects[i]->sys_type].c_str(), SysVar_objects[i]->ave_sys * 100);
        printf("\\hline\n");
        printf("\\end{tabular}\n");
        printf("\\caption{\\label{table:sys_unc_%s} Summary of the average systematic uncertainties for %s}\n", hist_name.c_str(), hist_name.c_str());
        printf("\\end{center}\n");
        printf("\\end{table}\n");
        printf("\n");
    }

    bool non_zero() {return init;}

private:
    bool init = false;

    std::string hist_name = "";
    std::string sys_type = "";

    std::vector<TotalSysVar*> TotalSysVar_objects;
    std::vector<SysVar*> SysVar_objects;

    TH1D* h1D_diff = 0;
    TH1D* h1D_ratio = 0;

    double ave_sys = 0;

    void calc_average() {
        int nonzero_bins = 0;
        for (int i=1; i<=h1D_ratio->GetNbinsX(); ++i) {
            if (h1D_ratio->GetBinContent(i) != 0) {
                ave_sys += h1D_ratio->GetBinContent(i);
                ++nonzero_bins;
            }
        }
        if (nonzero_bins)
            ave_sys /= nonzero_bins;
    }
};

#endif
