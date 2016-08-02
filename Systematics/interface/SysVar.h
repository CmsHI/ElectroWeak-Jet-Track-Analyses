#ifndef _SYSVAR_H
#define _SYSVAR_H

#include "TH1.h"
#include "TF1.h"
#include "TMath.h"

#include <string>

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
    }

    ~SysVar() {};

    void clear() {
        h1D_nominal->Delete();
        h1D_varied->Delete();
        h1D_diff->Delete();
        h1D_diff_abs->Delete();
        h1D_ratio->Delete();
        h1D_ratio_abs->Delete();

        fit_diff_abs->Delete();
        fit_ratio_abs->Delete();

        h1D_diff_abs_fit->Delete();
        h1D_ratio_abs_fit->Delete();
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

        h1D_diff_abs->Fit(diff_fit_name.c_str());
        fit_diff_abs = h1D_diff_abs->GetFunction(diff_fit_name.c_str());
        fit_diff_abs->Write("", TObject::kOverwrite);

        std::string ratio_fit_name = Form("fit_%s_ratio_%s", hist_name.c_str(), sys_type.c_str());
        TF1* ratio_fit = new TF1(ratio_fit_name.c_str(), ratio_fit_function.c_str());
        ratio_fit->SetRange(h1D_ratio_abs->GetBinLowEdge(1), h1D_ratio_abs->GetBinLowEdge(h1D_ratio_abs->GetNbinsX() + 1));

        h1D_ratio_abs->Fit(ratio_fit_name.c_str());
        fit_ratio_abs = h1D_ratio_abs->GetFunction(ratio_fit_name.c_str());
        fit_ratio_abs->Write("", TObject::kOverwrite);

        h1D_diff_abs_fit = (TH1D*)h1D_diff_abs->Clone(Form("h1D_%s_diff_abs_%s_fit", hist_name.c_str(), sys_type.c_str()));
        h1D_ratio_abs_fit = (TH1D*)h1D_ratio_abs->Clone(Form("h1D_%s_ratio_abs_%s_fit", hist_name.c_str(), sys_type.c_str()));

        TH1D_from_TF1(h1D_diff_abs_fit, fit_diff_abs);
        TH1D_from_TF1(h1D_ratio_abs_fit, fit_ratio_abs);
    }

    TH1D* get_ratio() {return h1D_ratio;}

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
};

class TotalSysVar {
public:
    TotalSysVar() {};

    TotalSysVar(SysVar* up, SysVar* down) {
        hist_name = up->hist_name;

        h1D_diff = new TH1D();
        h1D_ratio = new TH1D();
        h1D_diff_fit = new TH1D();
        h1D_ratio_fit = new TH1D();

        up->h1D_diff_abs->Copy(*h1D_diff);
        up->h1D_ratio_abs->Copy(*h1D_ratio);
        up->h1D_diff_abs_fit->Copy(*h1D_diff_fit);
        up->h1D_ratio_abs_fit->Copy(*h1D_ratio_fit);

        h1D_diff->SetName(Form("h1D_%s_diff_%s_down_total", hist_name.c_str(), up->sys_type.c_str()));
        h1D_ratio->SetName(Form("h1D_%s_ratio_%s_down_total", hist_name.c_str(), up->sys_type.c_str()));
        h1D_diff_fit->SetName(Form("h1D_%s_diff_%s_down_total_fit", hist_name.c_str(), up->sys_type.c_str()));
        h1D_ratio_fit->SetName(Form("h1D_%s_ratio_%s_down_total_fit", hist_name.c_str(), up->sys_type.c_str()));

        TH1D_Max(h1D_diff, down->h1D_diff_abs);
        TH1D_Max(h1D_ratio, down->h1D_ratio_abs);
        TH1D_Max(h1D_diff_fit, down->h1D_diff_abs_fit);
        TH1D_Max(h1D_ratio_fit, down->h1D_ratio_abs_fit);
    }

    TotalSysVar(const TotalSysVar& sys) {
        hist_name = sys.hist_name;

        h1D_diff = sys.h1D_diff;
        h1D_ratio = sys.h1D_ratio;
        h1D_diff_fit = sys.h1D_diff_fit;
        h1D_ratio_fit = sys.h1D_ratio_fit;
    }

    ~TotalSysVar() {};

    void clear() {
        h1D_diff->Delete();
        h1D_ratio->Delete();
        h1D_diff_fit->Delete();
        h1D_ratio_fit->Delete();
    }

    void add_SysVar(SysVar* sys) {
        if (hist_name == "") {
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
    }

    void add_SysVar(TotalSysVar* sys) {
        if (hist_name == "") {
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
    }

    bool non_zero() {return h1D_diff!=0 && h1D_ratio!=0;}

private:
    std::string hist_name = "";

    TH1D* h1D_diff = 0;
    TH1D* h1D_ratio = 0;
    TH1D* h1D_diff_fit = 0;
    TH1D* h1D_ratio_fit = 0;
};

#endif
