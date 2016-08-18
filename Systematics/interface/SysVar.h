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

        fit_diff_abs = sys.fit_diff_abs;
        fit_ratio_abs = sys.fit_ratio_abs;

        h1D_diff_abs_fit = sys.h1D_diff_abs_fit;
        h1D_ratio_abs_fit = sys.h1D_ratio_abs_fit;

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

        fit_diff_abs->Delete();
        fit_ratio_abs->Delete();

        h1D_diff_abs_fit->Reset("ICES");
        h1D_ratio_abs_fit->Reset("ICES");

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
    }

    void scale_sys(double scale_factor) {
        h1D_diff_abs->Scale(scale_factor);
        h1D_ratio_abs->Scale(scale_factor);
    }

    void fit_sys(std::string diff_fit_function, std::string ratio_fit_function) {
        std::string diff_fit_name = Form("fit_%s_diff_%s", hist_name.c_str(), sys_type.c_str());
        TF1* diff_fit = new TF1(diff_fit_name.c_str(), diff_fit_function.c_str());
        diff_fit->SetRange(h1D_diff_abs->GetBinLowEdge(1), h1D_diff_abs->GetBinLowEdge(h1D_diff_abs->GetNbinsX() + 1));

        h1D_diff_abs->Fit(diff_fit_name.c_str(), "Q0");
        fit_diff_abs = h1D_diff_abs->GetFunction(diff_fit_name.c_str());
        fit_diff_abs->Write("", TObject::kOverwrite);

        std::string ratio_fit_name = Form("fit_%s_ratio_%s", hist_name.c_str(), sys_type.c_str());
        TF1* ratio_fit = new TF1(ratio_fit_name.c_str(), ratio_fit_function.c_str());
        ratio_fit->SetRange(h1D_ratio_abs->GetBinLowEdge(1), h1D_ratio_abs->GetBinLowEdge(h1D_ratio_abs->GetNbinsX() + 1));

        h1D_ratio_abs->Fit(ratio_fit_name.c_str(), "Q0");
        fit_ratio_abs = h1D_ratio_abs->GetFunction(ratio_fit_name.c_str());
        fit_ratio_abs->Write("", TObject::kOverwrite);

        h1D_diff_abs_fit = (TH1D*)h1D_diff_abs->Clone(Form("h1D_%s_diff_abs_%s_fit", hist_name.c_str(), sys_type.c_str()));
        h1D_ratio_abs_fit = (TH1D*)h1D_ratio_abs->Clone(Form("h1D_%s_ratio_abs_%s_fit", hist_name.c_str(), sys_type.c_str()));

        TH1D_from_TF1(h1D_diff_abs_fit, fit_diff_abs);
        TH1D_from_TF1(h1D_ratio_abs_fit, fit_ratio_abs);

        calc_average();
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

    TF1* fit_diff_abs = 0;
    TF1* fit_ratio_abs = 0;

    TH1D* h1D_diff_abs_fit = 0;
    TH1D* h1D_ratio_abs_fit = 0;

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
        h1D_diff_fit = new TH1D();
        h1D_ratio_fit = new TH1D();

        up->h1D_diff_abs->Copy(*h1D_diff);
        up->h1D_ratio_abs->Copy(*h1D_ratio);
        up->h1D_diff_abs_fit->Copy(*h1D_diff_fit);
        up->h1D_ratio_abs_fit->Copy(*h1D_ratio_fit);

        h1D_diff->SetName(Form("h1D_%s_diff_%s_%s_total", hist_name.c_str(), up->sys_type.c_str(), down->sys_type.c_str()));
        h1D_ratio->SetName(Form("h1D_%s_ratio_%s_%s_total", hist_name.c_str(), up->sys_type.c_str(), down->sys_type.c_str()));
        h1D_diff_fit->SetName(Form("h1D_%s_diff_%s_%s_total_fit", hist_name.c_str(), up->sys_type.c_str(), down->sys_type.c_str()));
        h1D_ratio_fit->SetName(Form("h1D_%s_ratio_%s_%s_total_fit", hist_name.c_str(), up->sys_type.c_str(), down->sys_type.c_str()));

        TH1D_Max(h1D_diff, down->h1D_diff_abs);
        TH1D_Max(h1D_ratio, down->h1D_ratio_abs);
        TH1D_Max(h1D_diff_fit, down->h1D_diff_abs_fit);
        TH1D_Max(h1D_ratio_fit, down->h1D_ratio_abs_fit);

        calc_average();
    }

    TotalSysVar(const TotalSysVar& sys) {
        init = sys.init;

        hist_name = sys.hist_name;

        TotalSysVar_objects = sys.TotalSysVar_objects;
        SysVar_objects = sys.SysVar_objects;

        h1D_diff = sys.h1D_diff;
        h1D_ratio = sys.h1D_ratio;
        h1D_diff_fit = sys.h1D_diff_fit;
        h1D_ratio_fit = sys.h1D_ratio_fit;

        ave_sys = sys.ave_sys;
    }

    ~TotalSysVar() {};

    void clear() {
        init = false;

        TotalSysVar_objects.clear();
        SysVar_objects.clear();

        h1D_diff->Reset("ICES");
        h1D_ratio->Reset("ICES");
        h1D_diff_fit->Reset("ICES");
        h1D_ratio_fit->Reset("ICES");

        ave_sys = 0;
    }

    void add_SysVar(SysVar* sys) {
        if (!init) {
            init = true;

            hist_name = sys->hist_name;

            h1D_diff = new TH1D();
            h1D_ratio = new TH1D();
            h1D_diff_fit = new TH1D();
            h1D_ratio_fit = new TH1D();

            sys->h1D_diff_abs->Copy(*h1D_diff);
            sys->h1D_ratio_abs->Copy(*h1D_ratio);
            sys->h1D_diff_abs_fit->Copy(*h1D_diff_fit);
            sys->h1D_ratio_abs_fit->Copy(*h1D_ratio_fit);

            h1D_diff->SetName(Form("h1D_%s_diff_total", hist_name.c_str()));
            h1D_ratio->SetName(Form("h1D_%s_ratio_total", hist_name.c_str()));
            h1D_diff_fit->SetName(Form("h1D_%s_diff_total_fit", hist_name.c_str()));
            h1D_ratio_fit->SetName(Form("h1D_%s_ratio_total_fit", hist_name.c_str()));
        } else {
            TH1D_SqrtSumofSquares(h1D_diff, sys->h1D_diff_abs);
            TH1D_SqrtSumofSquares(h1D_ratio, sys->h1D_ratio_abs);
            TH1D_SqrtSumofSquares(h1D_diff_fit, sys->h1D_diff_abs_fit);
            TH1D_SqrtSumofSquares(h1D_ratio_fit, sys->h1D_ratio_abs_fit);
        }

        SysVar_objects.push_back(sys);
    }

    void add_SysVar(TotalSysVar* sys) {
        if (!init) {
            init = true;

            hist_name = sys->hist_name;

            h1D_diff = new TH1D();
            h1D_ratio = new TH1D();
            h1D_diff_fit = new TH1D();
            h1D_ratio_fit = new TH1D();

            sys->h1D_diff->Copy(*h1D_diff);
            sys->h1D_ratio->Copy(*h1D_ratio);
            sys->h1D_diff_fit->Copy(*h1D_diff_fit);
            sys->h1D_ratio_fit->Copy(*h1D_ratio_fit);

            h1D_diff->SetName(Form("h1D_%s_diff_total", hist_name.c_str()));
            h1D_ratio->SetName(Form("h1D_%s_ratio_total", hist_name.c_str()));
            h1D_diff_fit->SetName(Form("h1D_%s_diff_total_fit", hist_name.c_str()));
            h1D_ratio_fit->SetName(Form("h1D_%s_ratio_total_fit", hist_name.c_str()));
        } else {
            TH1D_SqrtSumofSquares(h1D_diff, sys->h1D_diff);
            TH1D_SqrtSumofSquares(h1D_ratio, sys->h1D_ratio);
            TH1D_SqrtSumofSquares(h1D_diff_fit, sys->h1D_diff_fit);
            TH1D_SqrtSumofSquares(h1D_ratio_fit, sys->h1D_ratio_fit);
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
    TH1D* h1D_diff_fit = 0;
    TH1D* h1D_ratio_fit = 0;

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
