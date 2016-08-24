#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TGraphErrors.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "TLatex.h"
#include "TArrow.h"
#include "TLine.h"

#include "../Utilities/interface/CutConfigurationParser.h"
#include "../Utilities/interface/InputConfigurationParser.h"

#include <string>

typedef struct box_t {
    float x1, y1, x2, y2;
} box_t;

void divide_canvas(TCanvas* c1, int rows, int columns, float margin, float edge);
void draw_sys_uncertainties(TBox* box, TH1* h1, TH1* h1_sys);
void set_legend_style(TLegend* l1, int font_size);
void set_hist_style(TH1D* h1, int k, int columns);
void set_graph_style(TGraphErrors* g1, int k, int columns);
void set_axis_style(TH1D* h1, int i, int j, int rows, int axis_font_size, int label_font_size);
void adjust_coordinates(box_t& box, float margin, float edge, int i, int j, int rows, int columns);

#define _NPLOTS 8
#define _PBPB_DATA 0
#define _PBPB_MC 1
#define _PP_DATA 2
#define _PP_MC 3
#define _JEWEL 4
#define _JEWEL_REF 5
#define _LBT 6
#define _LBT_REF 7

int multiPanelPlotter(const TString inputFile, const TString configFile) {
    gStyle->SetOptTitle(0);
    gStyle->SetOptStat(0);

    std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.Data());

    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

    std::vector<int> bins_pt[2];
    std::vector<int> bins_cent[2];

    bins_pt[0] = ConfigurationParser::ParseListInteger(configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_pt_gt]);
    bins_pt[1] = ConfigurationParser::ParseListInteger(configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_pt_lt]);
    std::vector<int> pt_bin_numbers;
    pt_bin_numbers = ConfigurationParser::ParseListInteger(configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_pt_bin_numbers]);

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

    std::string canvas_title = configInput.proc[INPUT::kPLOTTING].s[INPUT::k_mpp_canvas_title].c_str();

    std::string suffix[_NPLOTS] = {"PbPb_Data", "PbPb_MC", "pp_Data", "pp_MC", "JEWEL", "JEWEL_ppref", "LBT", "LBT_ppref"};
    std::string draw_options[_NPLOTS] = {"same e x0", "same hist x0", "e x0", "hist x0", "same hist e x0", "same hist e x0", "same hist x0", "same hist x0"};
    std::string sys_draw_options[_NPLOTS] = {"same e x0", "same hist x0", "same e x0", "hist x0", "", "", "", ""};
    std::string graph_draw_options[_NPLOTS] = {"", "", "", "", "same p[]", "same p[]", "", ""};
    std::string legend_labels[_NPLOTS] = {"PbPb", "Pythia + Hydjet", "pp (smeared)", "Pythia", "JEWEL + PYTHIA", "pp (JEWEL + PYTHIA)", "LBT", "pp (LBT)"};
    std::string legend_options[_NPLOTS] = {"pf", "l", "pf", "l", "l", "l", "l", "l"};

    int draw_order[_NPLOTS] = {_PP_MC, _PP_DATA, _PBPB_DATA, _PBPB_MC, _JEWEL_REF, _JEWEL, _LBT_REF, _LBT};

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
    } else if (hist_type == "ptJet") {
        hist_file_valid[_PBPB_MC] = false;
        hist_file_valid[_PP_MC] = false;
    }

    const int hist_width = 250;
    const int hist_height = 250;

    const int latex_font_sizes[6] = {0, 12, 13, 14, 16, 18};
    const int axis_font_sizes[6] = {0, 13, 13, 13, 14, 14};
    const int label_font_sizes[6] = {0, 13, 14, 15, 16, 18};
    const float latex_spacing[6] = {0, 0.07, 0.0725, 0.0775, 0.08, 0.084};

    bool cent_based_plots = (plot_type == "cent");
    int columns = cent_based_plots ? cent_bin_numbers.size() : pt_bin_numbers.size();
    int rows = cent_based_plots ? pt_bin_numbers.size() : cent_bin_numbers.size();

    float pad_width = (columns > 1) ? hist_width*(1.0/(1.0-margin) + 1.0/(1.0-edge) + columns - 2) : hist_width/(1.0-margin-edge);
    float pad_height = (rows > 1) ? hist_height*(1.0/(1.0-margin) + 1.0/(1.0-edge) + rows - 2) : hist_height/(1.0-margin-edge);

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

                std::string hist_handle;
                if (hist_type == "xjg" || hist_type == "dphi") {
                    if (cent_based_plots)
                        hist_handle = Form("%s_ptBin%d_hiBin%d_%s", hist_type.c_str(), pt_bin_numbers[i], cent_bin_numbers[j], suffix[k].c_str());
                    else
                        hist_handle = Form("%s_ptBin%d_hiBin%d_%s", hist_type.c_str(), pt_bin_numbers[j], cent_bin_numbers[i], suffix[k].c_str());
                } else if (hist_type == "ptJet") {
                    if (cent_based_plots)
                        hist_handle = Form("%s_ptBin%d_hiBin%d_%s_rebin", hist_type.c_str(), pt_bin_numbers[i], cent_bin_numbers[j], suffix[k].c_str());
                    else
                        hist_handle = Form("%s_ptBin%d_hiBin%d_%s_rebin", hist_type.c_str(), pt_bin_numbers[j], cent_bin_numbers[i], suffix[k].c_str());
                } else if (hist_type == "iaa") {
                    if (cent_based_plots)
                        hist_handle = Form("%s_ptBin%d_hiBin%d_rebin", hist_type.c_str(), pt_bin_numbers[i], cent_bin_numbers[j]);
                    else
                        hist_handle = Form("%s_ptBin%d_hiBin%d_rebin", hist_type.c_str(), pt_bin_numbers[j], cent_bin_numbers[i]);
                } else if (hist_type == "rjg_ptBinAll" || hist_type == "xjg_mean_ptBinAll" || hist_type == "dphi_width_ptBinAll") {
                    if (cent_based_plots)
                        hist_handle = Form("%s_hiBin%d_%s", hist_type.c_str(), cent_bin_numbers[j], suffix[k].c_str());
                    else
                        printf("Error: trying to plot %s as a function of pT!\n", hist_type.c_str());
                } else if (hist_type == "rjg_centBinAll" || hist_type == "xjg_mean_centBinAll" || hist_type == "dphi_width_centBinAll") {
                    if (cent_based_plots)
                        printf("Error: trying to plot %s as a function of centrality!\n", hist_type.c_str());
                    else
                        hist_handle = Form("%s_ptBin%d_%s", hist_type.c_str(), pt_bin_numbers[j], suffix[k].c_str());
                } else {
                    printf("Unknown plot type: %s\n", hist_type.c_str());
                }

                if ((k != _JEWEL && k != _JEWEL_REF) || hist_type.find("centBinAll") == std::string::npos) {
                    h1[i][j][k] = (TH1D*)hist_files[k]->Get(Form("h1D_%s", hist_handle.c_str()));
                    if (!h1[i][j][k])
                        continue;

                    h1[i][j][k]->SetAxisRange(y_min[i], y_max[i], "Y");
                    h1[i][j][k]->SetMaximum(y_max[i]);
                    h1[i][j][k]->SetMinimum(y_min[i]);

                    if (hist_type == "dphi_width_centBinAll" || hist_type == "dphi_width_ptBinAll")
                        h1[i][j][k]->SetYTitle("#sigma (#Delta#phi_{J#gamma})");
                    if (hist_type == "iaa") {
                        h1[i][j][k]->SetXTitle("p^{Jet}_{T} (GeV/c)");
                        h1[i][j][k]->SetYTitle("Jet I_{AA}");
                    }
                    if (hist_type == "dphi")
                        h1[i][j][k]->SetYTitle("#frac{1}{N_{J#gamma}} #frac{dN_{J#gamma}}{d#Delta#phi_{J#gamma}}");
                    if (hist_type.find("xjg_mean") != std::string::npos)
                        h1[i][j][k]->SetYTitle("<x_{J#gamma}>");

                    set_hist_style(h1[i][j][k], k, columns);
                    set_axis_style(h1[i][j][k], i, j, rows, axis_font_sizes[columns], label_font_sizes[columns]);

                    if ((k == _JEWEL || k == _JEWEL_REF) && hist_type == "dphi")
                        h1[i][j][k]->Scale(1/h1[i][j][k]->Integral());

                    h1[i][j][k]->Draw(draw_options[k].c_str());
                } else {
                    g1[i][j][k] = (TGraphErrors*)hist_files[k]->Get(Form("h1D_%s", hist_handle.c_str()));
                    if (!g1[i][j][k])
                        continue;

                    set_graph_style(g1[i][j][k], k, columns);

                    g1[i][j][k]->Draw(graph_draw_options[k].c_str());
                }

                if (sys_file_valid[k]) {
                    h1_sys[i][j][k] = (TH1D*)sys_files[k]->Get(Form("h1D_%s_diff_total", hist_handle.c_str()));
                    if (hist_type == "dphi")
                        h1_sys[i][j][k] = (TH1D*)sys_files[k]->Get(Form("h1D_%s_diff_total_fit", hist_handle.c_str()));

                    TBox* sys_box = new TBox();
                    sys_box->SetFillColorAlpha(46, 0.7);
                    sys_box->SetFillStyle(1001);
                    if (!k) sys_box->SetFillColorAlpha(46, 0.7);
                    else sys_box->SetFillColorAlpha(30, 0.7);

                    draw_sys_uncertainties(sys_box, h1[i][j][k], h1_sys[i][j][k]);
                    h1[i][j][k]->Draw(sys_draw_options[k].c_str());

                    h1[i][j][k]->SetFillColor(sys_box->GetFillColor());
                    h1[i][j][k]->SetFillStyle(1001);
                    h1[i][j][k]->SetLineColor(h1[i][j][k]->GetFillColor());
                }
            }

            // Draw legend
            if (i + j == 0) {
                box_t l_box = (box_t) {l_x1[i], l_y1[i], l_x2[i], l_y2[i]};
                adjust_coordinates(l_box, margin, edge, i, j, rows, columns);
                TLegend* l1 = new TLegend(l_box.x1, l_box.y1, l_box.x2, l_box.y2);
                set_legend_style(l1, latex_font_sizes[columns]);

                if (hist_type != "iaa" || hist_file_valid[_JEWEL]) {
                    for (int k=0; k<_NPLOTS; ++k) {
                        if (hist_file_valid[k]) {
                            if ((k == _JEWEL || k == _JEWEL_REF) && hist_type.find("centBinAll") != std::string::npos) {
                                if (g1[i][j][k]) {
                                    l1->AddEntry(g1[i][j][k], Form("%s", legend_labels[k].c_str()), legend_options[k].c_str());
                                }
                            } else {
                                l1->AddEntry(h1[i][j][k], Form("%s", legend_labels[k].c_str()), legend_options[k].c_str());
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
                adjust_coordinates(cms_box, margin, edge, i, j, rows, columns);
                latexCMS->DrawLatexNDC(cms_box.x1, cms_box.y1, "CMS");

                TLatex* latexPrelim = new TLatex();
                latexPrelim->SetTextFont(53);
                latexPrelim->SetTextSize(13);
                box_t prelim_box = (box_t) {0.04, 0.84, 1, 1};
                adjust_coordinates(prelim_box, margin, edge, i, j, rows, columns);
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
            latexInfo->SetTextSize(latex_font_sizes[columns]);
            if (i_x[i] > 0.6)
                latexInfo->SetTextAlign(31);
            else
                latexInfo->SetTextAlign(11);

            if (i_x[i] < 0.05 && i_y[i] > 0.45 && i + j > 0) {i_y[i] = 0.9;}
            else if (i_x[i] > 0.95 && i_y[i] > 0.45 && i + j > 0) {i_y[i] = 0.9;}
            for (std::size_t l=0; l<plotInfo.size(); ++l) {
                float line_pos = i_y[i] - l * latex_spacing[columns];
                box_t info_box = (box_t) {0, 0, i_x[i], line_pos};
                adjust_coordinates(info_box, margin, edge, i, j, rows, columns);
                latexInfo->DrawLatexNDC(info_box.x2, info_box.y2, plotInfo[l].c_str());
            }

            // Draw arrow for xjg mean
            if (hist_type == "xjg") {
                TArrow* arrow = new TArrow();
                arrow->SetArrowSize(0.01);
                arrow->SetLineColor(46);
                arrow->SetFillColor(46);

                TArrow* arrow_pp = new TArrow();
                arrow_pp->SetArrowSize(0.01);
                arrow_pp->SetLineColor(30);
                arrow_pp->SetFillColor(30);

                double arrow_x = h1[i][j][0]->GetMean();
                double arrow_x_pp = h1[i][j][2]->GetMean();
                double arrow_y = h1[i][j][0]->GetMaximum()*0.12;

                arrow->DrawArrow(arrow_x, arrow_y, arrow_x, 0);
                arrow_pp->DrawArrow(arrow_x_pp, arrow_y, arrow_x_pp, 0);
            }

            // Draw line at 1 for Jet IAA
            if (hist_type == "iaa") {
                TLine* line = new TLine();
                line->SetLineStyle(3);
                line->SetLineWidth(1);
                line->DrawLine(h1[i][j][0]->GetXaxis()->GetXmin(), 1, h1[i][j][0]->GetXaxis()->GetXmax(), 1);
            }
        }
    }

    // Draw energy, lumi info, jet cuts on top
    c1->cd();

    float canvas_left_margin = (columns > 1) ? margin / (1-margin) / (1.0/(1.0-margin) + 1.0/(1.0-edge) + columns - 2) : margin;
    float canvas_right_margin = (columns > 1) ? edge / (1-edge) / (1.0/(1.0-margin) + 1.0/(1.0-edge) + columns - 2) : edge;
    float canvas_top_edge = (rows > 1) ? 1.02 - edge / (1-edge) / (1.0/(1.0-margin) + 1.0/(1.0-edge) + rows - 2) : 1.02 - edge;

    TLatex* energyLatex = new TLatex();
    energyLatex->SetTextFont(43);
    energyLatex->SetTextSize(latex_font_sizes[columns]);
    energyLatex->SetTextAlign(11);
    energyLatex->DrawLatexNDC(canvas_left_margin+0.01, canvas_top_edge, "#sqrt{s_{NN}} = 5.02 TeV");

    TLatex* lumiLatex = new TLatex();
    lumiLatex->SetTextFont(43);
    lumiLatex->SetTextSize(latex_font_sizes[columns]);
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
    infoLatex->SetTextSize(latex_font_sizes[columns]);
    infoLatex->SetTextAlign(21);
    infoLatex->DrawLatexNDC((canvas_left_margin+1-canvas_right_margin)/2, canvas_top_edge, commonInfo.c_str());

    c1->SaveAs(Form("%s.pdf", canvas_title.c_str()));

    return 0;
}

void divide_canvas(TCanvas* c1, int rows, int columns, float margin, float edge) {
    c1->Clear();

    TPad* pads[rows][columns];

    float pad_width = (columns > 1) ? 1.0 / (1.0/(1.0-margin) + 1.0/(1.0-edge) + columns - 2) : 1.0-margin-edge;
    float pad_height = (rows > 1) ? 1.0 / (1.0/(1.0-margin) + 1.0/(1.0-edge) + rows - 2) : 1.0-margin-edge;

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

void draw_sys_uncertainties(TBox* box, TH1* h1, TH1* h1_sys) {
    int nBins = h1->GetNbinsX();
    for (int i=1; i<=nBins; ++i) {
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

void set_legend_style(TLegend* l1, int font_size) {
    l1->SetTextFont(43);
    l1->SetTextSize(font_size);
    l1->SetBorderSize(0);
    l1->SetFillStyle(0);
}

void set_hist_style(TH1D* h1, int k, int columns) {
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
            h1->SetLineWidth(columns > 3 ? 1 : 3);
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
            h1->SetLineWidth(columns > 3 ? 1 : 2);
            h1->SetMarkerSize(0);
            break;
        case _JEWEL_REF:
            h1->SetLineColor(6);
            h1->SetLineStyle(2);
            h1->SetLineWidth(columns > 3 ? 1 : 2);
            h1->SetMarkerSize(0);
            break;
        case _LBT:
            h1->SetLineColor(7);
            h1->SetLineStyle(1);
            h1->SetLineWidth(columns > 3 ? 1 : 2);
            h1->SetMarkerSize(0);
            break;
        case _LBT_REF:
            h1->SetLineColor(5);
            h1->SetLineStyle(5);
            h1->SetLineWidth(columns > 3 ? 1 : 2);
            h1->SetMarkerSize(0);
            break;
        default:
            break;
    }
}

void set_graph_style(TGraphErrors* g1, int k, int columns) {
    if (k == _JEWEL) {
        g1->SetLineColor(9);
        g1->SetLineStyle(1);
        g1->SetLineWidth(columns > 3 ? 1 : 2);
        g1->SetMarkerSize(0);
    } else if (k == _JEWEL_REF) {
        g1->SetLineColor(6);
        g1->SetLineStyle(1);
        g1->SetLineWidth(columns > 3 ? 1 : 2);
        g1->SetMarkerSize(0);
    }
}

void set_axis_style(TH1D* h1, int i, int j, int rows, int axis_font_size, int label_font_size) {
    TAxis* x_axis = h1->GetXaxis();
    TAxis* y_axis = h1->GetYaxis();

    x_axis->SetLabelFont(43);
    x_axis->SetLabelSize(axis_font_size);
    y_axis->SetLabelFont(43);
    y_axis->SetLabelSize(axis_font_size);

    x_axis->SetTitleFont(43);
    x_axis->SetTitleSize(label_font_size);
    y_axis->SetTitleFont(43);
    y_axis->SetTitleSize(label_font_size);

    if (i == rows - 1) {
        x_axis->SetTitleOffset(2.4);
        x_axis->CenterTitle();
    } else {
        x_axis->SetLabelOffset(999);
        x_axis->SetTitle("");
    }

    if (j == 0) {
        y_axis->SetTitleOffset(3.2);
        y_axis->CenterTitle();
    } else {
        y_axis->SetLabelOffset(999);
        y_axis->SetTitle("");
    }

    if (rows == 1) {
        x_axis->SetTitleOffset(1.5);
        y_axis->SetTitleOffset(1.6);
    }
}

void adjust_coordinates(box_t& box, float margin, float edge, int i, int j, int rows, int columns) {
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

int main(int argc, char* argv[]) {
    if (argc > 2) {
        for (int p=2; p<argc; ++p) {
            multiPanelPlotter(argv[1], argv[p]);
        }
    } else {
        printf("Usage: ./multiPanelPlotter.exe <fileList> <configFiles>\n");
        printf("./Plotting/multiPanelPlotter.exe Configurations/gammaJet/gammaJetPlot.list CutConfigurations/mpp_configs/*.conf\n");
    }

    return 0;
}
