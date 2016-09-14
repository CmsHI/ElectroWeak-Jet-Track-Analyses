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

#define H1D_TO_GRAPH(g_name)                                                        \
    int npoints = h1[i][j][k]->GetNbinsX();                                         \
    g_name = new TGraphErrors(npoints);                                             \
    for (int p=0; p<npoints; ++p) {                                                 \
        g_name->SetPoint(p, ncoll_w_npart[p], h1[i][j][k]->GetBinContent(p+1));     \
        g_name->SetPointError(p, 0, h1[i][j][k]->GetBinError(p+1));                 \
    }                                                                               \

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
#define _NPLOTS 10

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
void draw_sys_unc(TBox* box, TH1* h1, TH1* h1_sys, int first_bin = 1);
void draw_graph_sys_unc(TBox* box, TH1* h1, TH1* h1_sys, int first_bin = 1);
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

    std::vector<int> set_log_scale = ConfigurationParser::ParseListInteger(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_mpp_set_log_scale]);

    std::vector<float> y_max = ConfigurationParser::ParseListFloat(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_mpp_y_max]);
    std::vector<float> y_min = ConfigurationParser::ParseListFloat(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_mpp_y_min]);

    std::vector<float> l_x1 = ConfigurationParser::ParseListFloat(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_mpp_l_x1]);
    std::vector<float> l_y1 = ConfigurationParser::ParseListFloat(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_mpp_l_y1]);
    std::vector<float> l_x2 = ConfigurationParser::ParseListFloat(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_mpp_l_x2]);
    std::vector<float> l_y2 = ConfigurationParser::ParseListFloat(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_mpp_l_y2]);

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
    }

    std::string suffix[_NPLOTS] = {
        "PbPb_Data", "PbPb_MC", "pp_Data", "pp_MC",
        "JEWEL", "JEWEL_ppref", "LBT", "LBT_ppref", "Hybrid", "Hybrid_ppref"
    };
    std::string draw_options[_NPLOTS] = {
        "same e x0", "same hist e x0", "same e x0", "same hist e x0",
        "same l hist x0", "same l hist x0", "same l x0", "same l x0", "same l hist x0", "same l hist x0"
    };
    std::string sys_draw_options[_NPLOTS] = {
        "same e x0", "same hist e x0", "same e x0", "same hist e x0",
        "", "", "", "", "", ""
    };
    std::string graph_draw_options[_NPLOTS] = {
        "same p z", "same l", "same p z", " same l",
        "same l z", "same l z", "", "", "same l e3", ""
    };
    std::string legend_labels[_NPLOTS] = {
        "PbPb", "PYTHIA + HYDJET", "pp (smeared)", "PYTHIA",
        "JEWEL + PYTHIA", "pp (JEWEL + PYTHIA)", "LBT (CCNU-LBNL)", "pp (LBT (CCNU-LBNL))", "Hybrid Model", "pp (Hybrid Model)"
    };
    std::string legend_options[_NPLOTS] = {"pf", "l", "pf", "l", "l", "l", "l", "l", "f", "l"};

    int draw_order[_NPLOTS] = {_PP_MC, _PP_DATA, _PBPB_DATA, _PBPB_MC, _JEWEL_REF, _JEWEL, _LBT_REF, _LBT, _HYBRID, _HYBRID_REF};
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

    if (!set_log_scale.size())
        set_log_scale = std::vector<int>(rows, 0);

    TCanvas* c1 = new TCanvas(Form("canvas_%s", canvas_title.c_str()), "", pad_width, pad_height);
    divide_canvas(c1, rows, columns, margin, edge);

    TH1D* h1[rows][columns][_NPLOTS] = {0};
    TH1D* h1_sys[rows][columns][_NPLOTS] = {0};
    TGraphErrors* g1[rows][columns][_NPLOTS] = {0};

    for (int i=0; i<rows; ++i) {
        for (int j=0; j<columns; ++j) {
            c1->cd(i*columns+j+1);
            if (set_log_scale[i])
                gPad->SetLogy();

            // Draw histograms
            for (int l=0; l<_NPLOTS; ++l) {
                int k = draw_order[l];
                if (!hist_file_valid[k])
                    continue;

                std::string hist_name;
                if (hist_type == "xjg" || hist_type == "dphi") {
                    if (cent_based_plots)
                        hist_name = Form("h1D_%s_ptBin%d_hiBin%d_%s", hist_type.c_str(), pt_bin_numbers[i], cent_bin_numbers[j], suffix[k].c_str());
                    else
                        hist_name = Form("h1D_%s_ptBin%d_hiBin%d_%s", hist_type.c_str(), pt_bin_numbers[j], cent_bin_numbers[i], suffix[k].c_str());
                } else if (hist_type == "ptJet") {
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

                if (k != _HYBRID && ((k != _JEWEL && k != _JEWEL_REF) || hist_type.find("centBinAll") == std::string::npos)) {
                    h1[i][j][k] = (TH1D*)hist_files[k]->Get(hist_name.c_str());
                    if (!h1[i][j][k])
                        continue;

                    if (hist_type == "dphi_width_centBinAll" || hist_type == "dphi_width_ptBinAll")
                        h1[i][j][k]->SetYTitle("#sigma (#Delta#phi_{J#gamma})");
                    if (hist_type == "iaa")
                        h1[i][j][k]->SetYTitle("Jet I_{AA}");
                    if (hist_type == "ptJet" || hist_type == "iaa")
                        h1[i][j][k]->SetXTitle("p^{Jet}_{T} (GeV/c)");
                    if (hist_type == "dphi")
                        h1[i][j][k]->SetYTitle("#frac{1}{N_{J#gamma}} #frac{dN_{J#gamma}}{d#Delta#phi_{J#gamma}}");
                    if (hist_type.find("xjg_mean") != std::string::npos)
                        h1[i][j][k]->SetYTitle("<x_{J#gamma}>");
                    if (hist_type.find("centBinAll") != std::string::npos)
                        h1[i][j][k]->SetXTitle("N_{coll} weighted <N_{part}>");

                    set_hist_style(h1[i][j][k], k);
                    set_axis_style(h1[i][j][k], i, j, x_axis_offset, y_axis_offset);

                    h1[i][j][k]->SetAxisRange(y_min[i], y_max[i], "Y");
                    h1[i][j][k]->SetMaximum(y_max[i]);
                    h1[i][j][k]->SetMinimum(y_min[i]);

                    if (hist_type == "dphi" && set_log_scale[i])
                        h1[i][j][k]->SetAxisRange(2, 3.14, "X");

                    if (hist_type == "xjg")
                        h1[i][j][k]->SetNdivisions(504);

                    if ((k == _JEWEL || k == _JEWEL_REF || k == _HYBRID_REF) && hist_type == "dphi")
                        h1[i][j][k]->Scale(1/h1[i][j][k]->Integral());

                    // Workaround for not being able to draw a line through histogram contents and error bars at the same time
                    if (k == _JEWEL || k == _JEWEL_REF || k == _HYBRID || k == _HYBRID_REF)
                        h1[i][j][k]->Draw("same e x0");

                    if (k < _JEWEL && hist_type.find("centBinAll") != std::string::npos) {
                        H1D_TO_GRAPH(g1[i][j][k]);
                        set_graph_style(g1[i][j][k], k);

                        TH1D* h_tmp = (TH1D*)h1[i][j][k]->Clone();
                        trash_histogram(h_tmp);

                        h_tmp->Draw(draw_options[k].c_str());
                        g1[i][j][k]->Draw(graph_draw_options[k].c_str());
                    } else {
                        h1[i][j][k]->Draw(draw_options[k].c_str());
                    }
                } else {
                    g1[i][j][k] = (TGraphErrors*)hist_files[k]->Get(hist_name.c_str());
                    if (!g1[i][j][k])
                        continue;

                    set_graph_style(g1[i][j][k], k);
                    if ((k == _HYBRID || k == _HYBRID_REF) && hist_name.find("dphi") != std::string::npos)
                        g1[i][j][k]->SetLineWidth(line_width);

                    g1[i][j][k]->Draw(graph_draw_options[k].c_str());
                }

                if (sys_file_valid[k]) {
                    h1_sys[i][j][k] = (TH1D*)sys_files[k]->Get(Form("%s_diff_total", hist_name.c_str()));
                    if (hist_type == "dphi")
                        h1_sys[i][j][k] = (TH1D*)sys_files[k]->Get(Form("%s_diff_total_fit", hist_name.c_str()));

                    TBox* sys_box = new TBox();
                    sys_box->SetFillStyle(1001);
                    if (k == _PBPB_DATA)
                        sys_box->SetFillColorAlpha(46, 0.7);
                    else if (k == _PP_DATA)
                        sys_box->SetFillColorAlpha(30, 0.7);

                    if (hist_type.find("centBinAll") == std::string::npos) {
                        if (hist_type == "dphi" && set_log_scale[i])
                            draw_sys_unc(sys_box, h1[i][j][k], h1_sys[i][j][k], 13);
                        else
                            draw_sys_unc(sys_box, h1[i][j][k], h1_sys[i][j][k]);
                        h1[i][j][k]->Draw(sys_draw_options[k].c_str());
                    } else {
                        draw_graph_sys_unc(sys_box, h1[i][j][k], h1_sys[i][j][k]);
                        g1[i][j][k]->Draw(graph_draw_options[k].c_str());
                    }

                    h1[i][j][k]->SetFillColor(sys_box->GetFillColor());
                    h1[i][j][k]->SetFillStyle(1001);
                }
            }

            // Draw legend
            if (i + j == 0) {
                box_t l_box = (box_t) {l_x1[i], l_y1[i], l_x2[i], l_y2[i]};
                adjust_coordinates(l_box, margin, edge, i, j);
                TLegend* l1 = new TLegend(l_box.x1, l_box.y1, l_box.x2, l_box.y2);
                set_legend_style(l1);

                if (hist_type != "iaa" || hist_file_valid[_JEWEL]) {
                    for (int k=0; k<_NPLOTS; ++k) {
                        if (hist_file_valid[k]) {
                            if (k == _HYBRID || ((k == _JEWEL || k == _JEWEL_REF) && hist_type.find("centBinAll") != std::string::npos)) {
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
            if (bins_pt[1][pt_bin_numbers[pt_index]] < 9999)
                plotInfo.push_back(Form("%d < p_{T}^{#gamma} < %d GeV/c", bins_pt[0][pt_bin_numbers[pt_index]], bins_pt[1][pt_bin_numbers[pt_index]]));
            else
                plotInfo.push_back(Form("p_{T}^{#gamma} > %d GeV/c", bins_pt[0][pt_bin_numbers[pt_index]]));

            if (i + j == 0) {
                TLatex* latexCMS = new TLatex();
                latexCMS->SetTextFont(63);
                latexCMS->SetTextSize(16);
                box_t cms_box = (box_t) {0.04, 0.9, 1, 1};
                adjust_coordinates(cms_box, margin, edge, i, j);
                latexCMS->DrawLatexNDC(cms_box.x1, cms_box.y1, "CMS");

                TLatex* latexPrelim = new TLatex();
                latexPrelim->SetTextFont(53);
                latexPrelim->SetTextSize(13);
                box_t prelim_box = (box_t) {0.04, 0.84, 1, 1};
                adjust_coordinates(prelim_box, margin, edge, i, j);
                latexPrelim->DrawLatexNDC(prelim_box.x1, prelim_box.y1, "Preliminary");

                if (columns < 4) {
                    plotInfo.push_back("anti-k_{T} Jet R = 0.3");
                    plotInfo.push_back("p_{T}^{Jet} > 30 GeV/c");
                    plotInfo.push_back("#left|#eta^{Jet}#right| < 1.6");

                    if (columns == 1 && hist_type.find("dphi") == std::string::npos && hist_type != "iaa" && hist_type != "ptJet")
                        plotInfo.push_back("#Delta#phi_{J#gamma} > #frac{7#pi}{8}");
                }
            }

            TLatex* latexInfo = new TLatex();
            latexInfo->SetTextFont(43);
            latexInfo->SetTextSize(latex_font_size);
            if (i_x[i] > 0.8)
                latexInfo->SetTextAlign(31);
            else
                latexInfo->SetTextAlign(11);

            if (i_x[i] < 0.05 && i_y[i] > 0.45 && i + j > 0) {i_y[i] = 0.9;}
            else if (i_x[i] > 0.95 && i_y[i] > 0.45 && i + j > 0) {i_y[i] = 0.9;}
            for (std::size_t l=0; l<plotInfo.size(); ++l) {
                float line_pos = i_y[i] - l * latex_spacing;
                box_t info_box = (box_t) {0, 0, i_x[i], line_pos};
                adjust_coordinates(info_box, margin, edge, i, j);
                latexInfo->DrawLatexNDC(info_box.x2, info_box.y2, plotInfo[l].c_str());
            }

            // Draw arrow for xjg mean
            const std::vector<int> show_xjg_arrow = {};
            const std::vector<int> xjg_arrow_colour = {46, 30};
            // Don't draw arrows if comparing against theory
            if (hist_type == "xjg" && hist_file_valid[_PBPB_DATA] && hist_file_valid[_PP_DATA]) {
                for (std::size_t n=0; n<show_xjg_arrow.size(); ++n) {
                    if (hist_file_valid[show_xjg_arrow[n]]) {
                        TArrow* arrow = new TArrow();
                        arrow->SetArrowSize(0.01);
                        arrow->SetLineColor(xjg_arrow_colour[n]);
                        arrow->SetFillColor(xjg_arrow_colour[n]);

                        double arrow_x = h1[i][j][show_xjg_arrow[n]]->GetMean();
                        double arrow_y = h1[i][j][show_xjg_arrow[n]]->GetMaximum()*0.12;
                        arrow->DrawArrow(arrow_x, arrow_y, arrow_x, 0);
                    }
                }
            }

            // Draw line at 1 for Jet IAA
            if (hist_type == "iaa" && hist_file_valid[_PBPB_DATA]) {
                TLine* line = new TLine();
                line->SetLineStyle(3);
                line->SetLineWidth(1);
                line->DrawLine(h1[i][j][_PBPB_DATA]->GetXaxis()->GetXmin(), 1, h1[i][j][_PBPB_DATA]->GetXaxis()->GetXmax(), 1);
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
    lumiLatex->DrawLatexNDC(1-canvas_right_margin-0.01, canvas_top_edge, "PbPb 404 #mub^{-1}, pp 27.9 pb^{-1}");

    std::string commonInfo;
    if (columns > 3) {
        commonInfo = "anti-k_{T} Jet R = 0.3, p_{T}^{Jet} > 30 GeV/c, #left|#eta^{Jet}#right| < 1.6";
        if (hist_type.find("dphi") == std::string::npos && hist_type != "iaa" && hist_type != "ptJet")
            commonInfo += ", #Delta#phi_{J#gamma} > #frac{7#pi}{8}";
    } else if (columns > 1) {
        if (hist_type.find("dphi") == std::string::npos && hist_type != "iaa" && hist_type != "ptJet")
            commonInfo = "#Delta#phi_{J#gamma} > #frac{7#pi}{8}";
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
            axis_font_size = 24;
            axis_label_font_size = 27;
            latex_font_size = 20;
            line_width = 1;
            latex_spacing = 0.085;
            axis_label_cover_size = 0.0125;
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

void draw_sys_unc(TBox* box, TH1* h1, TH1* h1_sys, int first_bin) {
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

        box->SetX1(x - (bin_width/2));
        box->SetX2(x + (bin_width/2));
        box->SetY1(std::max(val - error, h1->GetMinimum()));
        box->SetY2(std::min(val + error, h1->GetMaximum()));

        box->DrawClone();
    }
}

void draw_graph_sys_unc(TBox* box, TH1* h1, TH1* h1_sys, int first_bin) {
    int nBins = h1->GetNbinsX();
    for (int i=first_bin; i<=nBins; ++i) {
        if (h1->GetBinError(i) == 0) continue;
        if (h1->GetBinContent(i) < h1->GetMinimum()) continue;
        if (h1->GetBinContent(i) > h1->GetMaximum()) continue;

        double x = ncoll_w_npart[i-first_bin];
        int sys_bin = h1_sys->FindBin(x);

        double val = h1->GetBinContent(i);
        double error = TMath::Abs(h1_sys->GetBinContent(sys_bin));

        box->SetX1(x - 10);
        box->SetX2(x + 10);
        box->SetY1(std::max(val - error, h1->GetMinimum()));
        box->SetY2(std::min(val + error, h1->GetMaximum()));

        box->DrawClone();
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
        case _JEWEL:
            h1->SetLineColor(9);
            h1->SetLineStyle(1);
            h1->SetLineWidth(line_width);
            h1->SetMarkerColor(9);
            h1->SetMarkerStyle(20);
            h1->SetMarkerSize(0);
            break;
        case _JEWEL_REF:
            h1->SetLineColor(9);
            h1->SetLineStyle(1);
            h1->SetLineWidth(line_width);
            h1->SetMarkerColor(9);
            h1->SetMarkerStyle(20);
            h1->SetMarkerSize(0);
            break;
        case _LBT:
            h1->SetLineColor(kOrange-3);
            h1->SetLineStyle(1);
            h1->SetLineWidth(line_width);
            h1->SetMarkerSize(0);
            break;
        case _LBT_REF:
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
        default:
            break;
    }
}

void set_graph_style(TGraphErrors* g1, int k) {
    if (k == _PBPB_DATA) {
        g1->SetLineColor(kBlack);
        g1->SetMarkerSize(0.64);
        g1->SetMarkerStyle(kFullCircle);
        g1->SetMarkerColor(kBlack);
    } else if (k == _PBPB_MC) {
        g1->SetLineColor(1);
        g1->SetLineStyle(1);
        g1->SetLineWidth(line_width);
        g1->SetMarkerSize(0);
    } else if (k == _PP_DATA) {
        g1->SetLineColor(kBlack);
        g1->SetMarkerSize(0.64);
        g1->SetMarkerStyle(kOpenCircle);
        g1->SetMarkerColor(kBlack);
    } else if (k == _JEWEL) {
        g1->SetLineColor(9);
        g1->SetLineStyle(1);
        g1->SetLineWidth(line_width);
        g1->SetMarkerSize(0);
    } else if (k == _JEWEL_REF) {
        g1->SetLineColor(9);
        g1->SetLineStyle(1);
        g1->SetLineWidth(line_width);
        g1->SetMarkerSize(0);
    } else if (k == _HYBRID) {
        g1->SetLineColorAlpha(kTeal+9, 0.7);
        g1->SetLineStyle(1);
        g1->SetLineWidth(0);
        g1->SetFillColor(kTeal+9);
        g1->SetFillColorAlpha(kTeal+9, 0.7);
    }
}

void set_axis_style(TH1D* h1, int i, int j, float x_axis_offset, float y_axis_offset) {
    TAxis* x_axis = h1->GetXaxis();
    TAxis* y_axis = h1->GetYaxis();

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
    float axis_label_cover_size_upper = (hist_type == "xjg") ? 0 : axis_label_cover_size_tall;
    for (int p=0; p<rows-1; ++p) {
        y_covers[p] = new TPad(Form("y_cover_%d", p), Form("y_cover_%d", p), x_min[0]-0.05, y_min[p]-axis_label_cover_size_tall, x_min[0]-0.0018, y_min[p]+axis_label_cover_size_upper);
        y_covers[p]->Draw();
    }

    float axis_label_cover_size_wide = (hist_type.find("BinAll") != std::string::npos) ? 0.025 : axis_label_cover_size;
    for (int p=1; p<columns; ++p) {
        x_covers[p] = new TPad(Form("x_cover_%d", p), Form("x_cover_%d", p), x_min[p]-axis_label_cover_size_wide, y_min[rows-1]-0.05, x_min[p]+axis_label_cover_size_wide, y_min[rows-1]-0.0024);
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
    }

    return 0;
}
