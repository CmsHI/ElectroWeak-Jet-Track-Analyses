#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TGraphErrors.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TArrow.h"
#include "TLine.h"

#include "../Utilities/interface/CutConfigurationParser.h"
#include "../Utilities/interface/InputConfigurationParser.h"

#include <string>

#define HIST_MINUS_N_PTS_TO_NPART_GRAPH(h_name, g_name, n)                      \
    int npoints = h_name->GetNbinsX() - n;                                      \
    g_name = new TGraphErrors(npoints);                                         \
    for (int p=0; p<npoints; ++p) {                                             \
        g_name->SetPoint(p, ncoll_w_npart[p+n], h_name->GetBinContent(p+1+n));  \
        g_name->SetPointError(p, 0, h_name->GetBinError(p+1+n));                \
    }                                                                           \

#define HIST_MINUS_N_PTS_TO_GRAPH(h_name, g_name, n)                                    \
    int npoints = h_name->GetNbinsX() - n;                                              \
    g_name = new TGraphErrors(npoints);                                                 \
    for (int p=0; p<npoints; ++p) {                                                     \
        g_name->SetPoint(p, h_name->GetBinCenter(p+1+n), h_name->GetBinContent(p+1+n)); \
        g_name->SetPointError(p, 0, h_name->GetBinError(p+1+n));                        \
    }                                                                                   \

typedef struct box_t {
    float x1, y1, x2, y2;
} box_t;

#define _PBPB_DATA 0
#define _PBPB_MC 1
#define _PP_DATA 2
#define _PP_MC 3
#define _JEWEL 4
#define _JEWEL_REF 5
#define _LBT 6
#define _LBT_REF 7
#define _HYBRID 8
#define _HYBRID_REF 9
#define _IVITEV 10
#define _IVITEV_REF 11
#define _HYBRIDRAD 12
#define _HYBRIDRAD_REF 13
#define _HYBRIDCOLL 14
#define _HYBRIDCOLL_REF 15
#define _NPLOTS 16
#define _WCEL 17

const float ncoll_w_npart[4] = {43.58, 118.8, 239.9, 363.4};

static const int hist_width = 250;
static const int hist_height = 250;

static int rows;
static int columns;

static float row_scale_factor;
static float column_scale_factor;

static int axis_font_size;
static int axis_label_font_size;
static int latex_font_size;
static int line_width;
static float latex_spacing;
static float axis_label_cover_size;

void set_global_style();
void divide_canvas(TCanvas* c1, int rows, int columns, float margin, float edge);
void draw_sys_unc(TGraph* gr, TH1* h1, TH1* h1_sys, int first_bin = 1);
void draw_graph_sys_unc(TGraph* gr, TH1* h1, TH1* h1_sys, int first_bin = 1);
void set_legend_style(TLegend* l1);
void set_hist_style(TH1D* h1, int k);
void set_graph_style(TGraphErrors* g1, int k);
void set_axis_style(TH1D* h1, int i, int j, float x_axis_offset, float y_axis_offset);
void set_graph_axis_style(TGraphErrors* g1, int i, int j, float x_axis_offset, float y_axis_offset);
void adjust_coordinates(box_t& box, float margin, float edge, int i, int j);
void cover_axis(std::string hist_type, float margin, float edge);
void trash_histogram(TH1D* h1);

int multiPanelPlotter(const TString inputFile, const TString configFile) {
    gStyle->SetOptTitle(0);
    gStyle->SetOptStat(0);

    std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.Data());

    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

    std::vector<int> bins_pt[2];
    bins_pt[0] = ConfigurationParser::ParseListInteger(configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_pt_gt]);
    bins_pt[1] = ConfigurationParser::ParseListInteger(configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_pt_lt]);
    std::vector<int> pt_bin_numbers;
    pt_bin_numbers = ConfigurationParser::ParseListInteger(configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_pt_bin_numbers]);

    std::vector<int> bins_cent[2];
    bins_cent[0] = ConfigurationParser::ParseListInteger(configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_gt]);
    bins_cent[1] = ConfigurationParser::ParseListInteger(configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_lt]);
    std::vector<int> cent_bin_numbers;
    cent_bin_numbers = ConfigurationParser::ParseListInteger(configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_cent_bin_numbers]);

    InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());

    float margin = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_mpp_margin];
    float edge = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_mpp_edge];

    std::string hist_type = configInput.proc[INPUT::kPLOTTING].s[INPUT::k_mpp_hist_type];
    std::string plot_type = configInput.proc[INPUT::kPLOTTING].s[INPUT::k_mpp_plot_type];

    int set_log_scale = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_mpp_set_log_scale];

    float y_max = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_mpp_y_max];
    float y_min = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_mpp_y_min];

    int l_panel = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_mpp_l_panel];
    float l_x1 = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_mpp_l_x1];
    float l_y1 = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_mpp_l_y1];
    float l_x2 = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_mpp_l_x2];
    float l_y2 = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_mpp_l_y2];

    std::vector<float> i_x = ConfigurationParser::ParseListFloat(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_mpp_i_x]);
    std::vector<float> i_y = ConfigurationParser::ParseListFloat(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_mpp_i_y]);

    float x_axis_offset = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_mpp_x_axis_offset];
    float y_axis_offset = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_mpp_y_axis_offset];

    std::string canvas_title = configInput.proc[INPUT::kPLOTTING].s[INPUT::k_mpp_canvas_title].c_str();

    TFile* hist_files[_NPLOTS];
    bool hist_file_valid[_NPLOTS] = {false};
    TFile* sys_files[_NPLOTS];
    bool sys_file_valid[_NPLOTS] = {false};

    // read files
    for (int i=0; i<_NPLOTS; ++i) {
        hist_files[i] = new TFile(inputFiles.at(i).c_str(), "READ");
        hist_file_valid[i] = (hist_files[i] && hist_files[i]->IsOpen());
        if (!hist_file_valid[i])
            continue;

        sys_files[i] = new TFile(inputFiles.at(i+_NPLOTS).c_str(), "READ");
        sys_file_valid[i] = (sys_files[i] && sys_files[i]->IsOpen());
    }

    if (hist_type == "iaa") {
        hist_file_valid[_PBPB_MC] = false;
        hist_file_valid[_PP_DATA] = false;
        hist_file_valid[_PP_MC] = false;
        hist_file_valid[_JEWEL_REF] = false;
        hist_file_valid[_LBT_REF] = false;
        hist_file_valid[_HYBRID_REF] = false;
        hist_file_valid[_IVITEV_REF] = false;
        hist_file_valid[_HYBRIDRAD_REF] = false;
        hist_file_valid[_HYBRIDCOLL_REF] = false;
    }

    std::string suffix[_NPLOTS] = {
        "PbPb_Data", "PbPb_MC", "pp_Data", "pp_MC",
        "JEWEL", "JEWEL_ppref", "LBT", "LBT_ppref", "Hybrid", "Hybrid_ppref", "pQCD", "pQCD_ppref",
        "HybridRad", "HybridRad_ppref", "HybridColl", "HybridColl_ppref"
    };
    std::string draw_options[_NPLOTS] = {
        "same e x0", "same hist e x0", "same e x0", "same hist e x0",
        "same l hist x0", "same l hist x0", "same l x0", "same l x0", "same l hist x0", "same l hist x0", "same l x0", "same l x0",
        "same l hist x0", "same l hist x0", "same l hist x0", "same l hist x0"
    };
    std::string sys_draw_options[_NPLOTS] = {
        "same e x0", "same hist e x0", "same e x0", "same hist e x0",
        "", "", "", "", "", "", "", "", "", "", "", ""
    };
    std::string graph_draw_options[_NPLOTS] = {
        "same p z", "same l z", "same p z", " same l z",
        "same l z", "same l z", "same l z", "same l z", "same l e3", "same l z", "same l z", "same l z",
        "same l e3", "same l z", "same l e3", "same l z"
    };
    std::string legend_labels[_NPLOTS] = {
        "PbPb", "PYTHIA + HYDJET", "pp (smeared)", "PYTHIA",
        "JEWEL + PYTHIA", "pp (JEWEL + PYTHIA)", "LBT 2017", "pp (LBT 2017)",
        "Hybrid Model", "pp (Hybrid Model)", "pQCD jet E-loss", "pp (pQCD jet E-loss)",
        "Hybrid dE/dx #alpha T^{3}", "pp (Hybrid dE/dx #alpha T^{3})", "Hybrid dE/dx #alpha T^{2}", "pp (Hybrid dE/dx #alpha T^{2})"
    };
    std::string legend_options[_NPLOTS] = {"pf", "l", "pf", "l", "l", "l", "l", "l", "f", "l", "l", "l", "f", "l", "f", "l"};

    int draw_order[_NPLOTS] = {_PP_MC, _PBPB_DATA, _PP_DATA, _PBPB_MC, _HYBRID_REF, _HYBRID, _JEWEL_REF, _JEWEL, _LBT_REF, _LBT, _HYBRIDRAD_REF, _HYBRIDRAD, _HYBRIDCOLL_REF, _HYBRIDCOLL, _IVITEV_REF, _IVITEV};
    for (int i=0; i<_NPLOTS; ++i) {
        int j = draw_order[i];
        if (hist_file_valid[j]) {
            draw_options[j].erase(draw_options[j].find("same "), 5);
            break;
        }
    }

    bool cent_based_plots = (plot_type == "cent");

    rows = cent_based_plots ? pt_bin_numbers.size() : cent_bin_numbers.size();
    columns = cent_based_plots ? cent_bin_numbers.size() : pt_bin_numbers.size();
    row_scale_factor = (rows > 1) ? 1.0/(1.0-margin) + 1.0/(1.0-edge) + rows - 2 : 1.0/(1.0-margin-edge);
    column_scale_factor = (columns > 1) ? 1.0/(1.0-margin) + 1.0/(1.0-edge) + columns - 2 : 1.0/(1.0-margin-edge);
    set_global_style();

    float pad_width = hist_width * column_scale_factor;
    float pad_height = hist_height * row_scale_factor;

    TCanvas* c1 = new TCanvas(Form("canvas_%s", canvas_title.c_str()), "", pad_width, pad_height);
    divide_canvas(c1, rows, columns, margin, edge);

    TH1D* h1[rows][columns][_NPLOTS];
    TH1D* h1_sys[rows][columns][_NPLOTS];
    TGraphErrors* g1[rows][columns][_NPLOTS];
    for (int iRows = 0; iRows < rows; ++iRows) {
        for (int iCols = 0; iCols < columns; ++iCols) {
            for (int iPlot = 0; iPlot < _NPLOTS; ++iPlot) {
                h1[iRows][iCols][iPlot] = 0;
                h1_sys[iRows][iCols][iPlot] = 0;
                g1[iRows][iCols][iPlot] = 0;
            }
        }
    }

    for (int i=0; i<rows; ++i) {
        for (int j=0; j<columns; ++j) {
            c1->cd(i*columns+j+1);
            if (set_log_scale)
                gPad->SetLogy();

            // Draw histograms
            for (int l=0; l<_NPLOTS; ++l) {
                int k = draw_order[l];
                if (!hist_file_valid[k])
                    continue;

                std::string hist_name;
                if (hist_type == "xjg" || (hist_type == "dphi" && k > _PP_MC)) {
                    if (cent_based_plots)
                        hist_name = Form("h1D_%s_ptBin%d_hiBin%d_%s", hist_type.c_str(), pt_bin_numbers[i], cent_bin_numbers[j], suffix[k].c_str());
                    else
                        hist_name = Form("h1D_%s_ptBin%d_hiBin%d_%s", hist_type.c_str(), pt_bin_numbers[j], cent_bin_numbers[i], suffix[k].c_str());
                } else if (hist_type == "dphi" || hist_type == "ptJet") {
                    if (cent_based_plots)
                        hist_name = Form("h1D_%s_ptBin%d_hiBin%d_%s_rebin", hist_type.c_str(), pt_bin_numbers[i], cent_bin_numbers[j], suffix[k].c_str());
                    else
                        hist_name = Form("h1D_%s_ptBin%d_hiBin%d_%s_rebin", hist_type.c_str(), pt_bin_numbers[j], cent_bin_numbers[i], suffix[k].c_str());
                } else if (hist_type == "iaa") {
                    if (cent_based_plots)
                        hist_name = Form("h1D_%s_ptBin%d_hiBin%d_rebin", hist_type.c_str(), pt_bin_numbers[i], cent_bin_numbers[j]);
                    else
                        hist_name = Form("h1D_%s_ptBin%d_hiBin%d_rebin", hist_type.c_str(), pt_bin_numbers[j], cent_bin_numbers[i]);
                } else if (hist_type == "rjg_ptBinAll" || hist_type == "xjg_mean_ptBinAll" || hist_type == "dphi_width_ptBinAll") {
                    if (cent_based_plots)
                        hist_name = Form("h1D_%s_hiBin%d_%s", hist_type.c_str(), cent_bin_numbers[j], suffix[k].c_str());
                    else
                        printf("Error: trying to plot %s as a function of pT!\n", hist_type.c_str());
                } else if (hist_type == "rjg_centBinAll" || hist_type == "xjg_mean_centBinAll" || hist_type == "dphi_width_centBinAll") {
                    if (cent_based_plots)
                        printf("Error: trying to plot %s as a function of centrality!\n", hist_type.c_str());
                    else
                        hist_name = Form("h1D_%s_ptBin%d_%s", hist_type.c_str(), pt_bin_numbers[j], suffix[k].c_str());
                } else {
                    printf("Unknown plot type: %s\n", hist_type.c_str());
                }

                if (k != _HYBRID && k != _HYBRIDRAD && k != _HYBRIDCOLL && ((k != _JEWEL && k != _JEWEL_REF) || hist_type.find("centBinAll") == std::string::npos)) {
                    h1[i][j][k] = (TH1D*)hist_files[k]->Get(hist_name.c_str());
                    if (!h1[i][j][k])
                        continue;

                    if (hist_type == "xjg") {
                        h1[i][j][k]->SetXTitle("x_{j#gamma} = p^{jet}_{T}/p^{#gamma}_{T}");
                        h1[i][j][k]->SetYTitle("#frac{1}{N_{#gamma}} #frac{dN_{j#gamma}}{dx_{j#gamma}}");
                    }
                    if (hist_type == "dphi_width_centBinAll" || hist_type == "dphi_width_ptBinAll")
                        h1[i][j][k]->SetYTitle("#sigma (#Delta#phi_{j#gamma})");
                    if (hist_type == "iaa") {
                        h1[i][j][k]->SetXTitle("p^{jet}_{T} (GeV/c)");
                        h1[i][j][k]->SetYTitle("jet I_{AA}");
                    }
                    if (hist_type == "ptJet") {
                        h1[i][j][k]->SetXTitle("p^{jet}_{T} (GeV/c)");
                        h1[i][j][k]->SetYTitle("#frac{1}{N_{#gamma}} #frac{dN_{j#gamma}}{dp^{jet}_{T}}");
                    }
                    if (hist_type == "dphi") {
                        h1[i][j][k]->SetXTitle("#Delta#phi_{j#gamma}");
                        h1[i][j][k]->SetYTitle("#frac{1}{N_{j#gamma}} #frac{dN_{j#gamma}}{d#Delta#phi_{j#gamma}}");
                    }
                    if (hist_type.find("xjg_mean") != std::string::npos)
                        h1[i][j][k]->SetYTitle("<x_{j#gamma}>");
                    if (hist_type == "rjg_ptBinAll" || hist_type == "rjg_centBinAll")
                        h1[i][j][k]->SetYTitle("R_{j#gamma}");
                    if (hist_type.find("centBinAll") != std::string::npos)
                        h1[i][j][k]->SetXTitle("N_{coll}-weighted <N_{part}>");

                    set_hist_style(h1[i][j][k], k);
                    set_axis_style(h1[i][j][k], i, j, x_axis_offset, y_axis_offset);

                    h1[i][j][k]->SetAxisRange(y_min, y_max, "Y");
                    h1[i][j][k]->SetMaximum(y_max);
                    h1[i][j][k]->SetMinimum(y_min);

                    if (hist_type == "dphi" && set_log_scale)
                        h1[i][j][k]->SetAxisRange(2, 3.14, "X");

                    if (hist_type == "xjg")
                        h1[i][j][k]->SetNdivisions(504);

                    // normalize dphi correctly
                    if ((k == _LBT || k == _LBT_REF || k == _HYBRID || k == _HYBRIDRAD || k == _HYBRIDCOLL) && hist_type == "dphi")
                        h1[i][j][k]->Scale(1./h1[i][j][k]->Integral(), "width");

                    // Workaround for not being able to draw a line through histogram contents and error bars at the same time
                    // LBT has no error bars!
                    if (k == _JEWEL || k == _JEWEL_REF || ((k == _HYBRID_REF || k == _HYBRIDRAD_REF || k == _HYBRIDCOLL_REF) && hist_type.find("centBinAll") == std::string::npos))
                        h1[i][j][k]->Draw("same e x0");

                    if ((k != _JEWEL && k != _JEWEL_REF && hist_type.find("centBinAll") != std::string::npos) ||
                        ((k == _LBT || k == _LBT_REF || k == _IVITEV) && (hist_type == "iaa" || hist_type == "ptJet"))) {
                        if (k == _HYBRID_REF || k == _HYBRIDRAD_REF || k == _HYBRIDCOLL_REF) {HIST_MINUS_N_PTS_TO_NPART_GRAPH(h1[i][j][k], g1[i][j][k], 1);}
                        else if (k == _LBT || k == _LBT_REF || k == _IVITEV) {HIST_MINUS_N_PTS_TO_GRAPH(h1[i][j][k], g1[i][j][k], 3);}
                        else {HIST_MINUS_N_PTS_TO_NPART_GRAPH(h1[i][j][k], g1[i][j][k], 0);}
                        set_graph_style(g1[i][j][k], k);

                        TH1D* h_tmp = (TH1D*)h1[i][j][k]->Clone();
                        trash_histogram(h_tmp);

                        h_tmp->Draw(draw_options[k].c_str());
                        g1[i][j][k]->Draw(graph_draw_options[k].c_str());

                        if (k == _IVITEV) {
                            TH1D* h_wcel = (TH1D*)hist_files[k]->Get(Form("%s_wcel", hist_name.c_str()));
                            TGraphErrors* g_wcel = 0;
                            HIST_MINUS_N_PTS_TO_GRAPH(h_wcel, g_wcel, 3);
                            set_graph_style(g_wcel, _WCEL);
                            g_wcel->Draw(graph_draw_options[k].c_str());
                        }
                    } else {
                        h1[i][j][k]->Draw(draw_options[k].c_str());
                    }
                } else {
                    g1[i][j][k] = (TGraphErrors*)hist_files[k]->Get(hist_name.c_str());
                    if (!g1[i][j][k])
                        continue;

                    set_graph_style(g1[i][j][k], k);
                    if ((k == _HYBRID || k == _HYBRID_REF || k == _HYBRIDRAD || k == _HYBRIDRAD_REF || k == _HYBRIDCOLL || k == _HYBRIDCOLL_REF) && hist_type == "dphi")
                        g1[i][j][k]->SetLineWidth(line_width);

                    g1[i][j][k]->Draw(graph_draw_options[k].c_str());
                }

                if (sys_file_valid[k]) {
                    h1_sys[i][j][k] = (TH1D*)sys_files[k]->Get(Form("%s_diff_total", hist_name.c_str()));

                    TGraph* sys_gr = new TGraph();
                    sys_gr->SetFillStyle(1001);
                    if (k == _PBPB_DATA)
                        sys_gr->SetFillColorAlpha(46, 0.7);
                    else if (k == _PP_DATA)
                        sys_gr->SetFillColorAlpha(30, 0.7);

                    if (hist_type.find("centBinAll") == std::string::npos) {
                        if (hist_type == "dphi" && set_log_scale)
                            draw_sys_unc(sys_gr, h1[i][j][k], h1_sys[i][j][k], 4);
                        else
                            draw_sys_unc(sys_gr, h1[i][j][k], h1_sys[i][j][k]);
                        h1[i][j][k]->Draw(sys_draw_options[k].c_str());
                    } else {
                        draw_graph_sys_unc(sys_gr, h1[i][j][k], h1_sys[i][j][k]);
                        g1[i][j][k]->Draw(graph_draw_options[k].c_str());
                    }

                    h1[i][j][k]->SetFillColor(sys_gr->GetFillColor());
                    h1[i][j][k]->SetFillStyle(1001);
                }
            }

            // Draw legend
            if (i * columns + j == l_panel - 1) {
                box_t l_box = (box_t) {l_x1, l_y1, l_x2, l_y2};
                adjust_coordinates(l_box, margin, edge, i, j);
                TLegend* l1 = new TLegend(l_box.x1, l_box.y1, l_box.x2, l_box.y2);
                set_legend_style(l1);
                if (columns == 5) {
                    if (configFile.Contains("theory_pp") || (configFile.Contains("theory_PbPb") && hist_type == "iaa"))
                        l1->SetTextSize(latex_font_size - 2);
                    else
                        l1->SetTextSize(latex_font_size - 1);
                }

                if (hist_type != "iaa" || hist_file_valid[_JEWEL]) {
                    for (int p=0; p<_NPLOTS; ++p) {
                        int k = draw_order[p];
                        if (hist_file_valid[k]) {
                            if (k == _HYBRID || k == _HYBRIDRAD || k == _HYBRIDCOLL || ((k == _JEWEL || k == _JEWEL_REF) && hist_type.find("centBinAll") != std::string::npos)) {
                                if (g1[i][j][k])
                                    l1->AddEntry(g1[i][j][k], legend_labels[k].c_str(), legend_options[k].c_str());
                            } else {
                                if (h1[i][j][k])
                                    l1->AddEntry(h1[i][j][k], legend_labels[k].c_str(), legend_options[k].c_str());
                            }
                        }
                    }
                }

                l1->Draw();
            }

            // ---- Photon/Jet Cuts ----
            std::vector<std::string> plotInfo;

            int cent_index = cent_based_plots ? j : i;
            int pt_index = cent_based_plots ? i : j;

            if (hist_type.find("centBinAll") == std::string::npos)
                plotInfo.push_back(Form("%d - %d%%", bins_cent[0][cent_bin_numbers[cent_index]]/2, bins_cent[1][cent_bin_numbers[cent_index]]/2));
            if (hist_type.find("ptBinAll") == std::string::npos) {
                if (bins_pt[1][pt_bin_numbers[pt_index]] < 9999)
                    plotInfo.push_back(Form("%d < p_{T}^{#gamma} < %d GeV/c", bins_pt[0][pt_bin_numbers[pt_index]], bins_pt[1][pt_bin_numbers[pt_index]]));
                else
                    plotInfo.push_back(Form("p_{T}^{#gamma} > %d GeV/c", bins_pt[0][pt_bin_numbers[pt_index]]));
            }

            if (i + j == 0) {
                TLatex* latexCMS = new TLatex();
                latexCMS->SetTextFont(63);
                latexCMS->SetTextSize(16);
                box_t cms_box = (box_t) {0.04, 0.9, 1, 1};
                if (hist_type == "xjg_mean_ptBinAll")
                    cms_box.x1 = 0.2;
                adjust_coordinates(cms_box, margin, edge, i, j);
                latexCMS->DrawLatexNDC(cms_box.x1, cms_box.y1, "CMS");

                TLatex* latexPrelim = new TLatex();
                latexPrelim->SetTextFont(53);
                latexPrelim->SetTextSize(13);
                box_t prelim_box = (box_t) {0.04, 0.84, 1, 1};
                if (hist_type == "xjg_mean_ptBinAll")
                    prelim_box.x1 = 0.2;
                adjust_coordinates(prelim_box, margin, edge, i, j);
                latexPrelim->DrawLatexNDC(prelim_box.x1, prelim_box.y1, "Preliminary");

                if (columns < 4) {
                    plotInfo.push_back("anti-k_{T} jet R = 0.3");
                    plotInfo.push_back("p_{T}^{jet} > 30 GeV/c");
                    plotInfo.push_back("#left|#eta^{jet}#right| < 1.6");

                    if (columns == 1 && hist_type.find("dphi") == std::string::npos && hist_type != "iaa" && hist_type != "ptJet")
                        plotInfo.push_back("#Delta#phi_{j#gamma} > #frac{7#pi}{8}");
                }
                if (configFile.Contains("theory_PbPb") && canvas_title == "xjg_mean_ptBinAll") {
                    plotInfo.push_back("p_{T}^{jet} > 30 GeV/c");
                    plotInfo.push_back("anti-k_{T} jet R = 0.3");
                    plotInfo.erase(plotInfo.begin() + 1);
                    plotInfo.erase(plotInfo.begin() + 1);
                }
            }

            TLatex* latexInfo = new TLatex();
            latexInfo->SetTextFont(43);
            latexInfo->SetTextSize(latex_font_size);
            if (columns == 5) latexInfo->SetTextSize(latex_font_size - 1);

            // (should move to config file)
            if (i_x[i * columns + j] > 0.8)
                latexInfo->SetTextAlign(31);
            else
                latexInfo->SetTextAlign(11);

            for (std::size_t l=0; l<plotInfo.size(); ++l) {
                float line_pos = i_y[i * columns + j] - l * latex_spacing;
                box_t info_box = (box_t) {0, 0, i_x[i * columns + j], line_pos};
                adjust_coordinates(info_box, margin, edge, i, j);
                latexInfo->DrawLatexNDC(info_box.x2, info_box.y2, plotInfo[l].c_str());
            }

            // Draw line at 1 for Jet IAA
            if (hist_type == "iaa" && hist_file_valid[_PBPB_DATA]) {
                TLine* line = new TLine();
                line->SetLineStyle(3);
                line->SetLineWidth(1);
                line->DrawLine(h1[i][j][_PBPB_DATA]->GetXaxis()->GetXmin(), 1, h1[i][j][_PBPB_DATA]->GetXaxis()->GetXmax(), 1);
            }

            // Draw line at y = 0 for dphi and xjg
            if ((hist_type == "dphi" || hist_type == "xjg") && hist_file_valid[_PBPB_DATA]) {
                TLine* line = new TLine();
                line->SetLineStyle(kDashed);
                line->SetLineWidth(1);
                gPad->Update();
                double xMin = gPad->GetUxmin();
                double xMax = gPad->GetUxmax();
                double yMin = gPad->GetUymin();
                double yMax = gPad->GetUymax();
                if (yMin < 0 && 0 < yMax)
                    line->DrawLine(xMin, 0, xMax, 0);
            }
        }
    }

    // Draw energy, lumi info, jet cuts on top
    c1->cd();

    float canvas_left_margin = (columns > 1) ? margin / (1-margin) / column_scale_factor : margin;
    float canvas_right_margin = (columns > 1) ? edge / (1-edge) / column_scale_factor : edge;
    float canvas_top_edge = (rows > 1) ? 1.02 - edge / (1-edge) / row_scale_factor : 1.02 - edge;

    TLatex* energyLatex = new TLatex();
    energyLatex->SetTextFont(43);
    energyLatex->SetTextSize(latex_font_size);
    energyLatex->SetTextAlign(11);
    energyLatex->DrawLatexNDC(canvas_left_margin+0.01, canvas_top_edge, "#sqrt{s_{NN}} = 5.02 TeV");

    TLatex* lumiLatex = new TLatex();
    lumiLatex->SetTextFont(43);
    lumiLatex->SetTextSize(latex_font_size);
    lumiLatex->SetTextAlign(31);
    lumiLatex->DrawLatexNDC(1-canvas_right_margin-0.01, canvas_top_edge, "PbPb 404 #mub^{-1}, pp 27.4 pb^{-1}");

    std::string commonInfo;
    if (columns > 3) {
        commonInfo = "anti-k_{T} jet R = 0.3, p_{T}^{jet} > 30 GeV/c, #left|#eta^{jet}#right| < 1.6";
        if (hist_type.find("dphi") == std::string::npos && hist_type != "iaa" && hist_type != "ptJet")
            commonInfo += ", #Delta#phi_{j#gamma} > #frac{7#pi}{8}";
    } else if (columns > 1) {
        if (hist_type.find("dphi") == std::string::npos && hist_type != "iaa" && hist_type != "ptJet")
            commonInfo = "#Delta#phi_{j#gamma} > #frac{7#pi}{8}";
    }

    TLatex* infoLatex = new TLatex();
    infoLatex->SetTextFont(43);
    infoLatex->SetTextSize(latex_font_size);
    infoLatex->SetTextAlign(21);
    infoLatex->DrawLatexNDC((canvas_left_margin+1-canvas_right_margin)/2, canvas_top_edge, commonInfo.c_str());

    // Cover cut-off axis labels
    cover_axis(hist_type, margin, edge);

    c1->SaveAs(canvas_title.append(".pdf").c_str());

    return 0;
}

void set_global_style() {
    switch (columns) {
        case 1:
            axis_font_size = 13;
            axis_label_font_size = 13;
            latex_font_size = 12;
            line_width = 3;
            latex_spacing = 0.0675;
            axis_label_cover_size = 0.02;
            break;
        case 2:
            axis_font_size = 14;
            axis_label_font_size = 14;
            latex_font_size = 13;
            line_width = 2;
            latex_spacing = 0.07;
            axis_label_cover_size = 0.016;
            break;
        case 3:
            axis_font_size = 15;
            axis_label_font_size = 16;
            latex_font_size = 14;
            line_width = 2;
            latex_spacing = 0.0775;
            axis_label_cover_size = 0.014;
            break;
        case 4:
            axis_font_size = 18;
            axis_label_font_size = 20;
            latex_font_size = 16;
            line_width = 1;
            latex_spacing = 0.081;
            axis_label_cover_size = 0.0125;
            break;
        case 5:
            axis_font_size = 22;
            axis_label_font_size = 25;
            latex_font_size = 20;
            line_width = 1;
            latex_spacing = 0.085;
            axis_label_cover_size = 0.012;
            break;
        default:
            axis_font_size = 13;
            axis_label_font_size = 13;
            latex_font_size = 12;
            line_width = 3;
            latex_spacing = 0.07;
            axis_label_cover_size = 0.02;
            break;
    }
}

void divide_canvas(TCanvas* c1, int rows, int columns, float margin, float edge) {
    c1->Clear();

    TPad* pads[rows][columns];

    float pad_width = 1.0 / column_scale_factor;
    float pad_height = 1.0 / row_scale_factor;

    float x_min[columns], x_max[columns];
    x_min[0] = 0;
    x_max[0] = pad_width/(1.0-margin);
    for (int i=1; i<columns; ++i) {
        x_min[i] = x_max[i-1];
        x_max[i] = x_max[i-1] + pad_width;
    }
    x_max[columns-1] = 1;

    float y_min[rows], y_max[rows];
    y_min[0] = 1.0-pad_height/(1.0-edge);
    y_max[0] = 1;
    for (int i=1; i<rows; ++i) {
        y_min[i] = y_min[i-1] - pad_height;
        y_max[i] = y_min[i-1];
    }
    y_min[rows-1] = 0;

    for (int i=0; i<rows; i++) {
        for (int j=0; j<columns; j++) {
            c1->cd();
            pads[i][j] = new TPad(Form("pad_%d_%d", i, j), Form("pad_%d_%d", i, j), x_min[j], y_min[i], x_max[j], y_max[i]);

            if (i == 0) pads[i][j]->SetTopMargin(edge);
            else pads[i][j]->SetTopMargin(0);
            if (i == rows - 1) pads[i][j]->SetBottomMargin(margin);
            else pads[i][j]->SetBottomMargin(0);
            if (j == 0) pads[i][j]->SetLeftMargin(margin);
            else pads[i][j]->SetLeftMargin(0);
            if (j == columns - 1) pads[i][j]->SetRightMargin(edge);
            else pads[i][j]->SetRightMargin(0);

            pads[i][j]->Draw();
            pads[i][j]->cd();
            pads[i][j]->SetNumber(i*columns+j+1);

            pads[i][j]->SetTickx();
            pads[i][j]->SetTicky();
        }
    }
}

void draw_sys_unc(TGraph* gr, TH1* h1, TH1* h1_sys, int first_bin) {
    int nBins = h1->GetNbinsX();
    for (int i=first_bin; i<=nBins; ++i) {
        if (h1->GetBinError(i) == 0) continue;
        if (h1->GetBinContent(i) < h1->GetMinimum()) continue;
        if (h1->GetBinContent(i) > h1->GetMaximum()) continue;

        double x = h1->GetBinCenter(i);
        int sys_bin = h1_sys->FindBin(x);
        double bin_width = h1->GetBinLowEdge(i+1) - h1->GetBinLowEdge(i);

        double val = h1->GetBinContent(i);
        double error = TMath::Abs(h1_sys->GetBinContent(sys_bin));

        gr->SetPoint(0, x - (bin_width/2), std::max(val - error, h1->GetMinimum()));
        gr->SetPoint(1, x + (bin_width/2), std::max(val - error, h1->GetMinimum()));
        gr->SetPoint(2, x + (bin_width/2), std::min(val + error, h1->GetMaximum()));
        gr->SetPoint(3, x - (bin_width/2), std::min(val + error, h1->GetMaximum()));

        gr->DrawClone("f");
    }
}

void draw_graph_sys_unc(TGraph* gr, TH1* h1, TH1* h1_sys, int first_bin) {
    int nBins = h1->GetNbinsX();
    for (int i=first_bin; i<=nBins; ++i) {
        if (h1->GetBinError(i) == 0) continue;
        if (h1->GetBinContent(i) < h1->GetMinimum()) continue;
        if (h1->GetBinContent(i) > h1->GetMaximum()) continue;

        double x = ncoll_w_npart[i-first_bin];
        int sys_bin = h1_sys->FindBin(x);

        double val = h1->GetBinContent(i);
        double error = TMath::Abs(h1_sys->GetBinContent(sys_bin));

        gr->SetPoint(0, x - 16, std::max(val - error, h1->GetMinimum()));
        gr->SetPoint(1, x + 16, std::max(val - error, h1->GetMinimum()));
        gr->SetPoint(2, x + 16, std::min(val + error, h1->GetMaximum()));
        gr->SetPoint(3, x - 16, std::min(val + error, h1->GetMaximum()));

        gr->DrawClone("f");
    }
}

void set_legend_style(TLegend* l1) {
    l1->SetTextFont(43);
    l1->SetTextSize(latex_font_size);
    l1->SetBorderSize(0);
    l1->SetFillStyle(0);
}

void set_hist_style(TH1D* h1, int k) {
    switch (k) {
        case _PBPB_DATA:
            h1->SetLineColor(kBlack);
            h1->SetMarkerSize(0.64);
            h1->SetMarkerStyle(kFullCircle);
            h1->SetMarkerColor(kBlack);
            break;
        case _PBPB_MC:
            h1->SetLineColor(1);
            h1->SetLineStyle(1);
            h1->SetLineWidth(line_width);
            h1->SetMarkerSize(0);
            break;
        case _PP_DATA:
            h1->SetLineColor(kBlack);
            h1->SetMarkerSize(0.64);
            h1->SetMarkerStyle(kOpenCircle);
            h1->SetMarkerColor(kBlack);
            break;
        case _PP_MC:
            break;
        case _JEWEL: case _JEWEL_REF:
            h1->SetLineColor(9);
            h1->SetLineStyle(1);
            h1->SetLineWidth(line_width);
            h1->SetMarkerColor(9);
            h1->SetMarkerStyle(20);
            h1->SetMarkerSize(0);
            break;
        case _LBT: case _LBT_REF:
            h1->SetLineColor(kOrange-3);
            h1->SetLineStyle(1);
            h1->SetLineWidth(line_width);
            h1->SetMarkerSize(0);
            break;
        case _HYBRID:
            break;
        case _HYBRID_REF:
            h1->SetLineColor(kTeal+9);
            h1->SetLineStyle(1);
            h1->SetLineWidth(line_width);
            h1->SetMarkerColor(kTeal+9);
            h1->SetMarkerStyle(20);
            h1->SetMarkerSize(0);
            break;
        case _IVITEV:
            h1->SetLineColor(kMagenta+1);
            h1->SetLineStyle(1);
            h1->SetLineWidth(line_width);
            h1->SetMarkerSize(0);
            break;
        case _IVITEV_REF:
            break;
        case _HYBRIDRAD:
            break;
        case _HYBRIDRAD_REF:
            h1->SetLineColor(39);
            h1->SetLineStyle(1);
            h1->SetLineWidth(line_width);
            h1->SetMarkerColor(39);
            h1->SetMarkerStyle(20);
            h1->SetMarkerSize(0);
            break;
        case _HYBRIDCOLL:
            break;
        case _HYBRIDCOLL_REF:
            h1->SetLineColor(kPink+1);
            h1->SetLineStyle(1);
            h1->SetLineWidth(line_width);
            h1->SetMarkerColor(kPink+1);
            h1->SetMarkerStyle(20);
            h1->SetMarkerSize(0);
            break;
        default:
            break;
    }
}

void set_graph_style(TGraphErrors* g1, int k) {
    switch (k) {
        case _PBPB_DATA:
            g1->SetLineColor(kBlack);
            g1->SetMarkerSize(0.64);
            g1->SetMarkerStyle(kFullCircle);
            g1->SetMarkerColor(kBlack);
            break;
        case _PBPB_MC:
            g1->SetLineColor(1);
            g1->SetLineStyle(1);
            g1->SetLineWidth(line_width);
            g1->SetMarkerSize(0);
            break;
        case _PP_DATA:
            g1->SetLineColor(kBlack);
            g1->SetMarkerSize(0.64);
            g1->SetMarkerStyle(kOpenCircle);
            g1->SetMarkerColor(kBlack);
            break;
        case _PP_MC:
            break;
        case _JEWEL: case _JEWEL_REF:
            g1->SetLineColor(9);
            g1->SetLineStyle(1);
            g1->SetLineWidth(line_width);
            g1->SetMarkerSize(0);
            break;
        case _LBT: case _LBT_REF:
            g1->SetLineColor(kOrange-3);
            g1->SetLineStyle(1);
            g1->SetLineWidth(line_width);
            g1->SetMarkerSize(0);
            break;
        case _HYBRID:
            g1->SetLineColorAlpha(kTeal+9, 0.7);
            g1->SetLineStyle(1);
            g1->SetLineWidth(0);
            g1->SetFillColor(kTeal+9);
            g1->SetFillColorAlpha(kTeal+9, 0.7);
            break;
        case _HYBRID_REF:
            g1->SetLineColorAlpha(kTeal+9, 0.7);
            g1->SetLineStyle(1);
            g1->SetLineWidth(line_width);
            break;
        case _IVITEV: case _IVITEV_REF:
            g1->SetLineColorAlpha(kMagenta+1, 0.7);
            g1->SetLineStyle(1);
            g1->SetLineWidth(line_width);
            g1->SetMarkerSize(0);
            break;
        case _HYBRIDRAD:
            g1->SetLineColorAlpha(39, 0.7);
            g1->SetLineStyle(1);
            g1->SetLineWidth(0);
            g1->SetFillColor(39);
            g1->SetFillColorAlpha(39, 0.7);
            break;
        case _HYBRIDRAD_REF:
            g1->SetLineColorAlpha(39, 0.7);
            g1->SetLineStyle(1);
            g1->SetLineWidth(line_width);
            break;
        case _HYBRIDCOLL:
            g1->SetLineColorAlpha(kPink+1, 0.7);
            g1->SetLineStyle(1);
            g1->SetLineWidth(0);
            g1->SetFillColor(kPink+1);
            g1->SetFillColorAlpha(kPink+1, 0.7);
            break;
        case _HYBRIDCOLL_REF:
            g1->SetLineColorAlpha(kPink+1, 0.7);
            g1->SetLineStyle(1);
            g1->SetLineWidth(line_width);
            break;
        case _WCEL:
            g1->SetLineColorAlpha(kMagenta+1, 0.7);
            g1->SetLineStyle(2);
            g1->SetLineWidth(line_width);
            g1->SetMarkerSize(0);
            break;
        default:
            break;
    }
}

void set_axis_style(TH1D* h1, int i, int j, float x_axis_offset, float y_axis_offset) {
    TAxis* x_axis = h1->GetXaxis();
    TAxis* y_axis = h1->GetYaxis();

    x_axis->SetLabelFont(43);
    x_axis->SetLabelSize(axis_font_size);
    y_axis->SetLabelFont(43);
    y_axis->SetLabelSize(axis_font_size);

    x_axis->SetLabelOffset(0.012);
    y_axis->SetLabelOffset(0.012);

    x_axis->SetTitleFont(43);
    x_axis->SetTitleSize(axis_label_font_size);
    y_axis->SetTitleFont(43);
    y_axis->SetTitleSize(axis_label_font_size);

    if (i == rows - 1) {
        x_axis->SetTitleOffset(x_axis_offset);
        x_axis->CenterTitle();
    } else {
        x_axis->SetLabelOffset(999);
        x_axis->SetTitleOffset(999);
        x_axis->SetTitle("");
    }

    if (j == 0) {
        y_axis->SetTitleOffset(y_axis_offset);
        y_axis->CenterTitle();
    } else {
        y_axis->SetLabelOffset(999);
        y_axis->SetTitleOffset(999);
        y_axis->SetTitle("");
    }
}

void set_graph_axis_style(TGraphErrors* g1, int i, int j, float x_axis_offset, float y_axis_offset) {
    TAxis* x_axis = g1->GetXaxis();
    TAxis* y_axis = g1->GetYaxis();

    x_axis->SetLabelFont(43);
    x_axis->SetLabelSize(axis_font_size);
    y_axis->SetLabelFont(43);
    y_axis->SetLabelSize(axis_font_size);

    x_axis->SetTitleFont(43);
    x_axis->SetTitleSize(axis_label_font_size);
    y_axis->SetTitleFont(43);
    y_axis->SetTitleSize(axis_label_font_size);

    if (i == rows - 1) {
        x_axis->SetTitleOffset(x_axis_offset);
        x_axis->CenterTitle();
    } else {
        x_axis->SetTitleOffset(999);
        x_axis->SetTitle("");
    }

    if (j == 0) {
        y_axis->SetTitleOffset(y_axis_offset);
        y_axis->CenterTitle();
    } else {
        y_axis->SetTitleOffset(999);
        y_axis->SetTitle("");
    }
}

void adjust_coordinates(box_t& box, float margin, float edge, int i, int j) {
    if (columns == 1) {
        box.x1 = box.x1 * (1-margin-edge) + margin;
        box.x2 = box.x2 * (1-margin-edge) + margin;
    } else if (j == 0) {
        box.x1 = box.x1 * (1-margin) + margin;
        box.x2 = box.x2 * (1-margin) + margin;
    } else if (j == columns - 1) {
        box.x1 = box.x1 * (1-edge);
        box.x2 = box.x2 * (1-edge);
    }

    if (rows == 1) {
        box.y1 = box.y1 * (1-margin-edge) + margin;
        box.y2 = box.y2 * (1-margin-edge) + margin;
    } else if (i == 0) {
        box.y1 = box.y1 * (1-edge);
        box.y2 = box.y2 * (1-edge);
    } else if (i == rows - 1) {
        box.y1 = box.y1 * (1-margin) + margin;
        box.y2 = box.y2 * (1-margin) + margin;
    }
}

void cover_axis(std::string hist_type, float margin, float edge) {
    TPad* x_covers[columns - 1];
    TPad* y_covers[rows - 1];

    float pad_width = 1.0 / column_scale_factor;
    float pad_height = 1.0 / row_scale_factor;

    float x_min[columns];
    x_min[0] = (columns > 1) ? pad_width*margin/(1.0-margin) : margin;
    for (int i=1; i<columns; ++i)
        x_min[i] = x_min[i-1] + pad_width;

    float y_min[rows];
    y_min[0] = (rows > 1) ? 1.0-pad_height/(1.0-edge) : margin;
    for (int i=1; i<rows; ++i)
        y_min[i] = y_min[i-1] - pad_height;

    float axis_label_cover_size_tall = (hist_type == "ptJet") ? 0.02 : axis_label_cover_size;
    float axis_label_cover_size_upper = (hist_type == "xjg" || hist_type == "dphi") ? 0 : axis_label_cover_size_tall;
    for (int p=0; p<rows-1; ++p) {
        y_covers[p] = new TPad(Form("y_cover_%d", p), Form("y_cover_%d", p), x_min[0]-0.05, y_min[p]-axis_label_cover_size_tall, x_min[0]-0.0018, y_min[p]+axis_label_cover_size_upper);
        y_covers[p]->Draw();
    }

    float axis_label_cover_size_wide = (hist_type.find("BinAll") != std::string::npos) ? 0.025 : axis_label_cover_size;
    for (int p=1; p<columns; ++p) {
        x_covers[p] = new TPad(Form("x_cover_%d", p), Form("x_cover_%d", p), x_min[p]-axis_label_cover_size_wide, y_min[rows-1]-0.05, x_min[p]+axis_label_cover_size_wide, y_min[rows-1]-0.005);
        x_covers[p]->Draw();
    }
}

void trash_histogram(TH1D* h1) {
    for (int i=1; i<=h1->GetNbinsX(); ++i)
        h1->SetBinContent(i, -100);
}

int main(int argc, char* argv[]) {
    if (argc > 2) {
        for (int p=2; p<argc; ++p) {
            multiPanelPlotter(argv[1], argv[p]);
        }
    } else {
        printf("Usage: ./multiPanelPlotter.exe <fileList> <configFiles>\n");
        printf("./Plotting/multiPanelPlotter.exe Configurations/gammaJet/data/gammaJetPlot.list Configurations/gammaJet/data/*.conf\n");
        printf("./Plotting/multiPanelPlotter.exe Configurations/gammaJet/theory_PbPb/gammaJetPlot.list Configurations/gammaJet/theory_PbPb/*.conf\n");
        printf("./Plotting/multiPanelPlotter.exe Configurations/gammaJet/theory_pp/gammaJetPlot.list Configurations/gammaJet/theory_pp/*.conf\n");
    }

    return 0;
}
