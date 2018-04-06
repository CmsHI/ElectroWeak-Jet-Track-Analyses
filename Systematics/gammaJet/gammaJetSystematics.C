#include "TFile.h"
#include "TH1.h"

#include "TCanvas.h"
#include "TLegend.h"

#include <vector>
#include <string>
#include <map>

#include "../interface/SysVar.h"

#include "../../Utilities/interface/CutConfigurationParser.h"
#include "../../Utilities/interface/InputConfigurationParser.h"

int gammaJetSystematics(const TString configFile, const TString inputList, const TString outputFile, bool rebin_xjg_jers = 1) {
    InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

    if (!configInput.isValid) {
        std::cout << "Invalid input configuration" << std::endl;
        return 1;
    } else if (!configCuts.isValid) {
        std::cout << "Invalid cut configuration" << std::endl;
        return 1;
    }

    TH1::SetDefaultSumw2();

    std::vector<float> pt_bins[2];
    pt_bins[0] = ConfigurationParser::ParseListFloat(configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_pt_gt]);
    pt_bins[1] = ConfigurationParser::ParseListFloat(configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_pt_lt]);
    int n_pt_bins = pt_bins[0].size();

    std::vector<int> cent_bins[2];
    cent_bins[0] = ConfigurationParser::ParseListInteger(configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_gt]);
    cent_bins[1] = ConfigurationParser::ParseListInteger(configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_lt]);
    int n_cent_bins = cent_bins[0].size();

#define _JES_UP      1
#define _JES_DOWN    2
#define _JES_UP2     3
#define _JES_DOWN2   4
#define _JES_GLUON   5
#define _JES_QUARK   6
#define _PURITY_UP   7
#define _PURITY_DOWN 8
#define _PHOTON_ISO  13

    std::vector<std::string> sys_types {
        "nominal",
        "JES_up", "JES_down", "JES_up2", "JES_down2", "JES_gluon", "JES_quark", "purity_up", "purity_down",
        "JER", "ele_rejection", "photon_energy", "photon_efficiency",
        "photon_iso"                                                // photon isolation _must_ be the last in this list
    };
    int n_sys_types = sys_types.size();

    std::vector<std::string> input_list = InputConfigurationParser::ParseFiles(inputList.Data());
    int n_input_files = input_list.size();
    if (n_input_files != n_sys_types) {
        printf("%i input files for %i systematic variations\n", n_input_files, n_sys_types);
        printf("please add dummy files to input file list: %s\n", inputList.Data());
        return 1;
    }

    std::map<std::string, std::string> sys_names;
    sys_names["JES_up"] = "Jet Energy Scale";
    sys_names["JES_down"] = "Jet Energy Scale";
    sys_names["JES_up2"] = "Jet Energy Scale";
    sys_names["JES_down2"] = "Jet Energy Scale";
    sys_names["JES_gluon"] = "Jet Energy Scale (Gluon)";
    sys_names["JES_quark"] = "Jet Energy Scale (Quark)";
    sys_names["purity_up"] = "Photon Purity";
    sys_names["purity_down"] = "Photon Purity";
    sys_names["JER"] = "Jet Energy Resolution";
    sys_names["ele_rejection"] = "Electron Contamination";
    sys_names["photon_energy"] = "Photon Energy Scale";
    sys_names["photon_efficiency"] = "Photon Efficiency";
    sys_names["photon_iso"] = "Photon Isolation";

    TFile* input_files[n_input_files] = {0};
    std::vector<bool> valid_input(n_input_files, false);
    for (int i=0; i<n_input_files; ++i) {
        input_files[i] = new TFile(input_list.at(i).c_str(), "read");
        valid_input[i] = (input_files[i] && input_files[i]->IsOpen());
    }

    if (!valid_input[0]) {
        printf("file with nominal histograms broken!\n");
        return 1;
    }

    std::vector<std::string> data_types {"PbPb_Data", "PbPb_MC", "pp_Data", "pp_MC"};
    int n_data_types = data_types.size();

    std::vector<std::string> hist_types {
        "xjg", "dphi", "ptJet", "rjg_ptBinAll", "rjg_centBinAll",
        "xjg_mean_ptBinAll", "xjg_mean_centBinAll", "dphi_width_ptBinAll", "dphi_width_centBinAll", "iaa"
    };
    int n_hist_types = hist_types.size();

    TFile* output = new TFile(outputFile, "recreate");

    typedef std::vector<SysVar*>    vvector;
    typedef std::vector<vvector>    vvvector;
    typedef std::vector<vvvector>   vvvvector;
    typedef std::vector<vvvvector>  vvvvvector;
    typedef std::vector<vvvvvector> vvvvvvector;

    vvvvvvector systematics(n_hist_types, vvvvvector(n_pt_bins, vvvvector(n_cent_bins, vvvector(n_data_types, vvector()))));

    for (int i=0; i<n_hist_types; ++i) {
        for (int j=0; j<n_pt_bins; ++j) {
            if (hist_types[i].find("ptBinAll") != std::string::npos)
                if (j) break;

            for (int k=0; k<n_cent_bins; ++k) {
                if (hist_types[i].find("centBinAll") != std::string::npos)
                    if (k) break;

                std::string hist_name;
                if (hist_types[i] == "xjg" || hist_types[i] == "dphi" || hist_types[i] == "ptJet" || hist_types[i] == "iaa")
                    hist_name = Form("%s_ptBin%d_hiBin%d", hist_types[i].c_str(), j, k);
                else if (hist_types[i].find("ptBinAll") != std::string::npos)
                    hist_name = Form("%s_hiBin%d", hist_types[i].c_str(), k);
                else if (hist_types[i].find("centBinAll") != std::string::npos)
                    hist_name = Form("%s_ptBin%d", hist_types[i].c_str(), j);
                else
                    printf("invalid histogram type: %s\n", hist_types[i].c_str());

                for (int l=0; l<n_data_types; ++l) {
                    if (hist_types[i] == "iaa")
                        if (l) break;

                    std::string hist_full_name;
                    if (hist_types[i] == "iaa") {
                        hist_full_name = Form("%s_rebin", hist_name.c_str());
                    } else if (hist_types[i] == "ptJet" && (l == 0 || l == 2)) {
                        hist_full_name = Form("%s_%s_rebin", hist_name.c_str(), data_types[l].c_str());
                    } else if (hist_types[i] == "dphi") {
                        hist_full_name = Form("%s_%s_rebin", hist_name.c_str(), data_types[l].c_str());
                    } else {
                        hist_full_name = Form("%s_%s", hist_name.c_str(), data_types[l].c_str());
                    }

                    printf("nominal: %s\n", hist_full_name.c_str());
                    TH1D* h1D_nominal = (TH1D*)input_files[0]->Get(Form("h1D_%s", hist_full_name.c_str()));

                    printf("calculating systematics...\n");
                    for (int m=1; m<n_sys_types; ++m) {
                        if (!valid_input[m])
                            continue;

                        if (sys_types[m] == "photon_energy" && data_types[l].find("pp") != std::string::npos)
                            continue;

                        TH1D* h1D_varied = (TH1D*)input_files[m]->Get(Form("h1D_%s", hist_full_name.c_str()));

                        SysVar* sys_hists = new SysVar(hist_full_name, sys_types[m]);
                        sys_hists->init(h1D_nominal, h1D_varied);

                        if (rebin_xjg_jers && hist_types[i] == "xjg" && (sys_types[m] == "JER" || sys_types[m] == "JES_up" || sys_types[m] == "JES_down" || sys_types[m] == "JES_up2" || sys_types[m] == "JES_down2" || sys_types[m] == "JES_gluon" || sys_types[m] == "JES_quark"))
                            sys_hists->rebin_and_calc_sys();
                        else
                            sys_hists->calc_sys();

                        if (sys_types[m] == "ele_rejection")
                            sys_hists->scale_sys(0.55);

                        systematics[i][j][k][l].push_back(sys_hists);
                    }
                }

                // photon isolation
                if (valid_input.back()) {
                    if (hist_types[i] != "iaa") {
                        TH1D* h1D_nominal_PbPb = 0;
                        TH1D* h1D_nominal_pp = 0;

                        if (hist_types[i] == "dphi") {
                            h1D_nominal_PbPb = (TH1D*)input_files[0]->Get(Form("h1D_%s_PbPb_Data_rebin", hist_name.c_str()));
                            h1D_nominal_pp = (TH1D*)input_files[0]->Get(Form("h1D_%s_pp_Data_rebin", hist_name.c_str()));
                        } else {
                            h1D_nominal_PbPb = (TH1D*)input_files[0]->Get(Form("h1D_%s_PbPb_Data", hist_name.c_str()));
                            h1D_nominal_pp = (TH1D*)input_files[0]->Get(Form("h1D_%s_pp_Data", hist_name.c_str()));
                        }

                        systematics[i][j][k][0].back()->init_with_ratio(h1D_nominal_PbPb, systematics[i][j][k][1].back()->get_ratio());
                        systematics[i][j][k][0].back()->calc_sys();

                        systematics[i][j][k][2].back()->init_with_ratio(h1D_nominal_pp, systematics[i][j][k][3].back()->get_ratio());
                        systematics[i][j][k][2].back()->calc_sys();
                    } else {
                        // no jet iaa in MC yet
                        systematics[i][j][k][0].pop_back();
                    }
                }
            }
        }
    }

    std::vector<TotalSysVar*> total_sys;

    printf("calculating total systematics...\n");
    for (std::size_t i=0; i<systematics.size(); ++i) {
        printf("for %s\n", hist_types[i].c_str());
        for (std::size_t j=0; j<systematics[i].size(); ++j) {
            for (std::size_t k=0; k<systematics[i][j].size(); ++k) {
                for (std::size_t l=0; l<systematics[i][j][k].size(); ++l) {
                    if (!systematics[i][j][k][l].size())
                        continue;

                    std::size_t m = 0;
                    TotalSysVar* total_sys_hists = new TotalSysVar();

                    TotalSysVar* JES_up_hists = new TotalSysVar();
                    JES_up_hists->add_SysVar(systematics[i][j][k][l][m]);
                    JES_up_hists->add_SysVar(systematics[i][j][k][l][m+2]);
                    JES_up_hists->add_SysVar(systematics[i][j][k][l][m+4]);

                    TotalSysVar* JES_down_hists = new TotalSysVar();
                    JES_down_hists->add_SysVar(systematics[i][j][k][l][m+1]);
                    JES_down_hists->add_SysVar(systematics[i][j][k][l][m+3]);
                    JES_down_hists->add_SysVar(systematics[i][j][k][l][m+5]);

                    TotalSysVar* JES_sys_hists = new TotalSysVar(JES_up_hists, JES_down_hists);
                    total_sys_hists->add_SysVar(JES_sys_hists);
                    m = m + 6;

                    if (valid_input[_PURITY_UP] && valid_input[_PURITY_DOWN]) {
                        TotalSysVar* purity_sys_hists = new TotalSysVar(systematics[i][j][k][l][m], systematics[i][j][k][l][m+1]);
                        total_sys_hists->add_SysVar(purity_sys_hists);
                        ++m; ++m;
                    } else if (valid_input[_PURITY_UP] ^ valid_input[_PURITY_DOWN]) {
                        total_sys_hists->add_SysVar(systematics[i][j][k][l][m]);
                        ++m;
                    }

                    for (; m<systematics[i][j][k][l].size(); ++m)
                        total_sys_hists->add_SysVar(systematics[i][j][k][l][m]);

                    if (total_sys_hists->non_zero()) {
                        total_sys.push_back(total_sys_hists);
                        total_sys_hists->print_latex(sys_names);
                    }
                }
            }
        }
    }

    printf("writing objects...\n");
    output->Write("", TObject::kOverwrite);
    output->Close();

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 4)
        return gammaJetSystematics(argv[1], argv[2], argv[3]);
    else if (argc == 5)
        return gammaJetSystematics(argv[1], argv[2], argv[3], atoi(argv[4]));
    else
        printf("Usage : \n"
               "./gammaJetSystematics.exe <configFile> <inputList> <outputFile>\n");

    return 1;
}
