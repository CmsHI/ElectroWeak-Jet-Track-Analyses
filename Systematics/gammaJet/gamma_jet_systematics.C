#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"

#include <fstream>
#include <vector>
#include <string>
#include <map>

#include "../interface/systematics.h"

std::string sys_types[7] = {
    "pes", "purity_up", "purity_down", "ele_rej", "iso", "jer", "jes"
};

std::string fit_funcs[7] = {
    "pol2", "pol2", "pol2", "pol2", "pol2", "pol2", "pol2"
};

int options[7] = {
    0, 6, 0, 0, 0, 4, 4
};

int special[7] = {
    0, 0, 3, 4, 2, 5, 5
};

std::string sys_labels[7] = {
    "Photon Energy Scale", "Photon Purity", "Photon Purity", "Electron Contamination", "Photon Isolation", "Jet Energy Resolution", "Jet Energy Scale"
};

bool replace(std::string& str, const std::string& from, const std::string& to);

int get_index(std::vector<std::string> hist_list, std::string hist_name);

int gamma_jet_systematics(const char* nominal_file, const char* filelist, const char* histlist, const char* output) {
    TH1::AddDirectory(kFALSE);
    TH1::SetDefaultSumw2(kTRUE);

    std::string line;

    std::vector<std::string> hist_list;
    std::ifstream hist_stream(histlist);
    if (!hist_stream) return 1;
    while (std::getline(hist_stream, line))
        hist_list.push_back(line);

    std::size_t nhists = hist_list.size();
    if (!nhists) {printf("0 total hists!\n"); return 1;}

    std::vector<std::string> file_list;
    std::ifstream file_stream(filelist);
    if (!file_stream) return 1;
    while (std::getline(file_stream, line))
        file_list.push_back(line);

    std::size_t nfiles = file_list.size();
    if (!nfiles) {printf("0 total files!\n"); return 1;}

    std::map<std::string, std::string> iso_map;
    for (std::size_t i=0; i<nhists; ++i) {
        std::string mc_hist_name = hist_list[i];
        replace(mc_hist_name, "Data", "MC");
        iso_map[hist_list[i]] = mc_hist_name;
    }

    TFile* fnominal = new TFile(nominal_file, "read");
    TH1F* hnominals[nhists] = {0};
    for (std::size_t i=0; i<nhists; ++i)
        hnominals[i] = (TH1F*)fnominal->Get(hist_list[i].c_str());

    TFile* fsys[nfiles] = {0};
    for (std::size_t i=0; i<nfiles; ++i)
        fsys[i] = new TFile(file_list[i].c_str(), "read");

    TFile* fout = new TFile(Form("%s.root", output), "recreate");

    total_sys_var_t* total_sys_vars[nhists] = {0};
    sys_var_t* sys_vars[nhists][nfiles] = {0};
    for (std::size_t i=0; i<nhists; ++i) {
        total_sys_vars[i] = new total_sys_var_t(hist_list[i], hnominals[i]);

        for (std::size_t j=0; j<nfiles; ++j) {
            sys_vars[i][j] = new sys_var_t(hist_list[i], sys_types[j], hnominals[i], (TH1F*)fsys[j]->Get(hist_list[i].c_str()));

            switch (special[j]) {
                case 0:
                    sys_vars[i][j]->calc_sys();
                    break;
                case 1: {
                    double binning[] = {};
                    sys_vars[i][j]->calc_sys(0, binning);
                    break; }
                case 2: {
                    TH1F* hnominal_iso = (TH1F*)fnominal->Get(iso_map[hist_list[i]].c_str());
                    TH1F* hvariation_iso = (TH1F*)fsys[j]->Get(iso_map[hist_list[i]].c_str());
                    sys_vars[i][j]->calc_sys(hnominal_iso, hvariation_iso);
                    break; }
                case 3: {
                    sys_var_t* tmp_sys_var = sys_vars[i][j];
                    tmp_sys_var->calc_sys();
                    sys_vars[i][j] = new sys_var_t(sys_vars[i][j-1], tmp_sys_var);
                    break; }
                case 4:
                    sys_vars[i][j]->calc_sys();
                    sys_vars[i][j]->scale_sys(0.55);
                default:
                    break;
            }

            // sys_vars[i][j]->fit_sys(fit_funcs[j].c_str(), "pol2");
            sys_vars[i][j]->write();

            total_sys_vars[i]->add_sys_var(sys_vars[i][j], options[j]);
        }

        total_sys_vars[i]->write();
    }

    std::vector<std::string> hist_set[2];
    hist_set[0] = {"h1D_xjg_mean_centBinAll_ptBin1_PbPb_Data", "h1D_xjg_mean_ptBinAll_hiBin1_PbPb_Data", "h1D_xjg_mean_ptBinAll_hiBin2_PbPb_Data", "h1D_dphi_width_centBinAll_ptBin1_PbPb_Data"};
    hist_set[1] = {"h1D_rjg_centBinAll_ptBin1_PbPb_Data", "h1D_rjg_ptBinAll_hiBin1_PbPb_Data", "h1D_rjg_ptBinAll_hiBin2_PbPb_Data"};

    for (int r=0; r<1; ++r) {
        printf("\\begin{table}[hbtp]\n");
        printf("\\begin{center}\n");
        printf("\\begin{tabular}{r c c c c}\n");
        printf("\\hline\n");
        printf("Systematic Uncertainty   & \\shortstack{\\avexjg \\\\ $\\ptg > 60$~GeV} & \\shortstack{\\avexjg \\\\ 0-30\\%%} & \\shortstack{\\avexjg \\\\ 30-100\\%%} & \\shortstack{\\sjg \\\\ $\\ptg > 60$~GeV} \\\\\n");
        printf("\\hline\n");
        printf("\\hline\n");
        for (int s=0; s<7; ++s) {
            printf("%-24s", sys_labels[s].c_str());
            for (std::size_t t=0; t<hist_set[r].size(); ++t)
                sys_vars[get_index(hist_list, hist_set[r][t])][s]->print_latex(options[s]);
            printf(" \\\\\n");
        }
        printf("\\hline\n");
        printf("\\end{tabular}\n");

        printf("\n");
    }

    for (std::size_t i=0; i<nfiles; ++i)
        fsys[i]->Close();

    fout->Write("", TObject::kOverwrite);
    fout->Close();

    return 0;
}

int get_index(std::vector<std::string> hist_list, std::string hist_name) {
    std::size_t index = find(hist_list.begin(), hist_list.end(), hist_name) - hist_list.begin();
    if (index >= hist_list.size())
        return -1;
    else
        return index;
}

bool replace(std::string& str, const std::string& from, const std::string& to) {
    std::size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);

    return true;
}

int main(int argc, char* argv[]) {
    if (argc == 5)
       return gamma_jet_systematics(argv[1], argv[2], argv[3], argv[4]);
    else
        return 1;
}
