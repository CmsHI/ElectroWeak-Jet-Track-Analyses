#include "TFile.h"
#include "TH1.h"
#include "TGraphErrors.h"

#include "TStyle.h"
#include "TLine.h"
#include "TColor.h"

#include "../Utilities/interface/CutConfigurationParser.h"
#include "../Utilities/interface/InputConfigurationParser.h"

#include <string>
#include <vector>
#include <fstream>

#include "tiling.h"

static const float ncoll_w_npart[4] = {43.58, 118.8, 239.9, 363.4};

#define TRASH_TH1(hist)                                                     \
    for (int p=1; p<=hist->GetNbinsX(); ++p) {                              \
        hist->SetBinContent(p, -999);                                       \
        hist->SetBinError(p, 0);                                            \
    }

#define TH1_TO_TGRAPH(hist, graph)                                          \
    int npoints = hist->GetNbinsX();                                        \
    graph = new TGraphErrors(npoints);                                      \
    for (int p=0; p<npoints; ++p) {                                         \
        graph->SetPoint(p, ncoll_w_npart[p], hist->GetBinContent(p+1));     \
        graph->SetPointError(p, 0, hist->GetBinError(p+1));                 \
    }

std::string set_systematics_style(TGraph* gr, int style);
void set_histogram_style(TH1* h1, int style, std::vector<std::string>& option_strings);
void set_graph_style(TGraph* g1, int style, std::vector<std::string>& option_strings);

void draw_sys_unc(TGraph* gr, TH1* h1, TH1* h1_sys);
void draw_npart_sys_unc(TGraph* gr, TH1* h1, TH1* h1_sys, int x_width);

std::vector<std::string> legend_draw_options;

int gammaJetPlot(const std::string input_file, const std::string sys_file, const std::string hist_list, const std::string config_file) {
    gStyle->SetOptTitle(0);
    gStyle->SetOptStat(0);

    TFile* input = new TFile(input_file.c_str(), "read");
    TFile* sys = new TFile(sys_file.c_str(), "read");

    InputConfiguration config = InputConfigurationParser::Parse(config_file.c_str());

#define _GET_CONFIG_INT(value) int value = config.proc[INPUT::kPLOTTING].i[INPUT::k_mpp_##value];
#define _GET_CONFIG_FLOAT(value) float value = config.proc[INPUT::kPLOTTING].f[INPUT::k_mpp_##value];
#define _GET_CONFIG_STRING(value) std::string value = config.proc[INPUT::kPLOTTING].s[INPUT::k_mpp_##value];

#define _GET_CONFIG_INT_VECTOR(value, count)                                \
    std::vector<int> value = ConfigurationParser::ParseListInteger(config.proc[INPUT::kPLOTTING].str_i[INPUT::k_mpp_##value]);  \
    if ((int)value.size() < (int)count) {                                   \
        printf("error: values in list [" #value "]: %zu, expected: " #count "\n", value.size());    \
        return 1;                                                           \
    }                                                                       \

#define _GET_CONFIG_FLOAT_VECTOR(value, count)                              \
    std::vector<float> value = ConfigurationParser::ParseListFloat(config.proc[INPUT::kPLOTTING].str_f[INPUT::k_mpp_##value]);  \
    if ((int)value.size() < (int)count) {                                   \
        printf("error: values in list [" #value "]: %zu, expected: " #count "\n", value.size());    \
        return 1;                                                           \
    }                                                                       \

#define _GET_CONFIG_STRING_VECTOR(value, count)                             \
    std::vector<std::string> value = ConfigurationParser::ParseListLatex(config.proc[INPUT::kPLOTTING].s[INPUT::k_mpp_##value]);   \
    if ((int)value.size() < (int)count) {                                   \
        printf("error: values in list [" #value "]: %zu, expected: " #count "\n", value.size());    \
        return 1;                                                           \
    }                                                                       \

    _GET_CONFIG_INT(rows);
    _GET_CONFIG_INT(columns);

    _GET_CONFIG_FLOAT_VECTOR(margins, 4);
    _GET_CONFIG_FLOAT_VECTOR(title_offsets, 2);
    _GET_CONFIG_FLOAT_VECTOR(label_offsets, 2);
    _GET_CONFIG_FLOAT_VECTOR(latex_sizes, 4);
    _GET_CONFIG_FLOAT_VECTOR(title_sizes, 2);
    _GET_CONFIG_FLOAT_VECTOR(label_sizes, 2);
    _GET_CONFIG_FLOAT_VECTOR(tick_sizes, 2);
    _GET_CONFIG_INT_VECTOR(cover_options, 2);

    float cms_latex_size = latex_sizes[0];
    float legend_latex_size = latex_sizes[1];
    float info_latex_size = latex_sizes[2];
    float canvas_latex_size = latex_sizes[3];

    _GET_CONFIG_INT_VECTOR(set_log_scale, rows);
    _GET_CONFIG_FLOAT_VECTOR(y_min, rows);
    _GET_CONFIG_FLOAT_VECTOR(y_max, rows);

    _GET_CONFIG_INT_VECTOR(draw_sys, rows * columns);
    _GET_CONFIG_INT_VECTOR(clear_entries, rows * columns);

    _GET_CONFIG_INT_VECTOR(l_panel, 0);
    _GET_CONFIG_FLOAT_VECTOR(l_x1, l_panel.size());
    _GET_CONFIG_FLOAT_VECTOR(l_y1, l_panel.size());
    _GET_CONFIG_FLOAT_VECTOR(l_x2, l_panel.size());
    _GET_CONFIG_FLOAT_VECTOR(l_y2, l_panel.size());

    _GET_CONFIG_FLOAT_VECTOR(i_x, rows * columns);
    _GET_CONFIG_FLOAT_VECTOR(i_y, rows * columns);

    _GET_CONFIG_STRING_VECTOR(x_titles, 1);
    _GET_CONFIG_STRING_VECTOR(y_titles, rows);

    _GET_CONFIG_STRING(hist_type);
    _GET_CONFIG_STRING(canvas_title);

    std::vector<std::string> config_lines;
    std::ifstream file_stream(hist_list);
    if (!file_stream) { printf("bad input list\n"); return 1; }
    std::string line;
    while (std::getline(file_stream, line))
        config_lines.push_back(line);

    std::vector<std::string> histogram_names[rows][columns];
    std::vector<int> styles[rows][columns];

    std::vector<std::string> legend_labels[l_panel.size()];

    std::size_t i = 0;

    int r = 0;
    int c = 0;
    for (; i<config_lines.size(); ++i) {
        if (r >= rows) { printf("error: not enough panels\n"); return 1; }

        if (config_lines[i] == "+") {
            c = (c + 1) % columns;
            r += !c;
        } else if (config_lines[i] == "=") {
            ++i;
            break;
        } else {
            std::size_t pos = config_lines[i].find(" ");
            histogram_names[r][c].push_back(config_lines[i].substr(0, pos));
            styles[r][c].push_back(std::stol(config_lines[i].substr(pos + 1)));
        }
    }

    if (r != rows - 1 || c != columns - 1) {
        printf("warning: not enough groups of histogram names - %i > %i\n",
            rows * columns, r*columns + c+1);
        printf("some panels will be empty\n");
    }

    std::size_t n = 0;
    for (; i<config_lines.size(); ++i) {
        if (n >= l_panel.size()) {
            printf("error: not enough legends\n");
            return 1;
        }

        if (config_lines[i] == "+") { ++n; }
        else { legend_labels[n].push_back(config_lines[i]); }
    }

    if (n != l_panel.size() - 1) {
        printf("warning: not enough groups of legend labels - %zu > %zu\n",
            l_panel.size(), n);
        printf("some legends will be empty\n");
    }

    tiling* tiler = new tiling(columns, rows, 400, 400,
        margins[0], margins[1], margins[2], margins[3]);

    TCanvas* c1 = tiler->create_canvas("c1", "");
    for (int r=0; r<rows; ++r) {
        for (int c=0; c<columns; ++c) {
            c1->cd(r*columns + c+1);
            if (set_log_scale[r]) { gPad->SetLogy(); }

            TObject* generic[histogram_names[r][c].size()] = {0};
            TH1D* histograms[histogram_names[r][c].size()] = {0};
            TGraphErrors* graphs[histogram_names[r][c].size()] = {0};
            std::vector< std::vector< std::string > > option_strings(
                histogram_names[r][c].size(), std::vector< std::string >(0)
            );

            for (std::size_t l=0; l<histogram_names[r][c].size(); ++l) {
                if ((int)l == clear_entries[r*columns + c]) { gPad->Clear(); }

                TH1D* systematics = 0;
                systematics = (TH1D*)sys->Get((histogram_names[r][c][l] + "_diff_total").c_str());

                TGraph* gr = new TGraph();
                gr->SetFillStyle(1001);
                std::string sys_draw_options = set_systematics_style(gr, styles[r][c][l]);

                generic[l] = input->Get(histogram_names[r][c][l].c_str());

                if (hist_type.find("centBinAll") != std::string::npos) {
                    histograms[l] = (TH1D*)generic[l];
                    if (l < 2) {
                        TRASH_TH1(histograms[l]);
                    } else {
                        TH1_TO_TGRAPH(histograms[l], graphs[l]);
                        generic[l] = graphs[l];
                    }
                }

                if (generic[l]->InheritsFrom(TH1D::Class())) {
                    histograms[l] = (TH1D*)generic[l];
                    tiler->set_sizes(
                        histograms[l], 4,
                        title_sizes[0], title_sizes[1],
                        label_sizes[0], label_sizes[1],
                        tick_sizes[0], tick_sizes[1],
                        title_offsets[0], title_offsets[1],
                        label_offsets[0], label_offsets[1],
                        c, r
                    );

                    set_histogram_style(histograms[l], styles[r][c][l], option_strings[l]);
                    histograms[l]->GetXaxis()->SetTitle(x_titles[0].c_str());
                    histograms[l]->GetYaxis()->SetTitle(y_titles[r].c_str());

                    if (hist_type != "purity") {
                        histograms[l]->SetAxisRange(y_min[r], y_max[r], "Y");
                        histograms[l]->SetMinimum(y_min[r]);
                        histograms[l]->SetMaximum(y_max[r]);
                    }

                    /* tick mark settings */
                    if (hist_type == "xjg" || hist_type == "xjg_mean_rjg_centBinAll")
                        histograms[l]->SetNdivisions(504, "X");
                    else if (hist_type == "iaa")
                        histograms[l]->SetNdivisions(504, "Y");

                    /* draw systematic uncertainties */
                    if (systematics && (int)l < draw_sys[r*columns + c]) {
                        /* draw frame for systematics */
                        histograms[l]->Draw(sys_draw_options.c_str());
                        draw_sys_unc(gr, histograms[l], systematics);
                    }

                    /* draw histogram */
                    if (hist_type == "purity") {
                        histograms[l]->Draw(option_strings[l][0].c_str());
                    } else {
                        histograms[l] = (TH1D*)histograms[l]->DrawCopy(option_strings[l][0].c_str());
                        histograms[l]->ResetBit(kCanDelete);
                    }
                } else if (generic[l]->InheritsFrom(TGraphErrors::Class())) {
                    graphs[l] = (TGraphErrors*)generic[l];
                    tiler->set_sizes(
                        graphs[l], 4,
                        title_sizes[0], title_sizes[1],
                        label_sizes[0], label_sizes[1],
                        tick_sizes[0], tick_sizes[1],
                        title_offsets[0], title_offsets[1],
                        label_offsets[0], label_offsets[1],
                        c, r
                    );

                    set_graph_style(graphs[l], styles[r][c][l], option_strings[l]);
                    graphs[l]->GetXaxis()->SetTitle(x_titles[0].c_str());
                    graphs[l]->GetYaxis()->SetTitle(y_titles[r].c_str());

                    /* draw systematic uncertainties */
                    if (systematics && (int)l < draw_sys[r*columns + c]) {
                        if (hist_type.find("centBinAll") != std::string::npos) {
                            histograms[0]->Draw("same");
                            histograms[l]->SetMinimum(y_min[r]);
                            histograms[l]->SetMaximum(y_max[r]);
                            draw_npart_sys_unc(gr, histograms[l], systematics, 32);
                        }
                    }

                    /* draw graph */
                    graphs[l]->Draw(option_strings[l][0].c_str());
                } else {
                    printf("unknown object type!\n");
                    return 1;
                }
            }

            for (std::size_t s=0; s<l_panel.size(); ++s) {
                if (r*columns + c+1 == l_panel[s]) {
                    TLegend* l1 = tiler->create_legend_on_frame(
                        l_x1[s], l_y1[s], l_x2[s], l_y2[s],
                        4, legend_latex_size, c, r
                    );

                    for (std::size_t t=0; t<legend_labels[s].size(); ++t) {
                        if (legend_labels[s][t] == "@@") { continue; }

                        if (histograms[t]) {
                            l1->AddEntry(histograms[t], legend_labels[s][t].c_str(),
                                option_strings[t][1].c_str());
                        } else if (graphs[t]) {
                            l1->AddEntry(graphs[t], legend_labels[s][t].c_str(),
                                option_strings[t][1].c_str());
                        }
                    }

                    l1->Draw();
                }
            }

            if (!histograms[0]) { continue; }

            // Draw line at 1 for Jet IAA
            if (hist_type == "iaa") {
                TLine* line = new TLine();
                line->SetLineStyle(3);
                line->SetLineWidth(1);
                line->DrawLine(
                    histograms[0]->GetXaxis()->GetXmin(), 1,
                    histograms[0]->GetXaxis()->GetXmax(), 1
                );
            }

            // Draw line at y = 0 for dphi and xjg
            if (hist_type == "dphi" || hist_type == "xjg") {
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

            // draw latex info
            std::vector<std::string> plotInfo;

            if (hist_type != "purity") {
                if (histogram_names[r][c][0].find("centBinAll") == std::string::npos) {
                    int bins_cent[2][7] = {
                        {0, 0, 30, 0, 10, 30, 50},
                        {100, 30, 100, 10, 30, 50, 100}
                    };

                    std::size_t pos = histogram_names[r][c][0].find("hiBin") + 5;
                    int cent_index = std::strtol(histogram_names[r][c][0].c_str() + pos, nullptr, 10);

                    if (canvas_title == "xjg_cent")
                        plotInfo.push_back(Form("Cent. %d - %d%%", bins_cent[0][cent_index], bins_cent[1][cent_index]));
                }

                if (histogram_names[r][c][0].find("ptBinAll") == std::string::npos) {
                    int bins_pt[2][8] = {
                        {40, 60, 40, 50, 60, 80, 80, 100},
                        {9999, 9999, 50, 60, 80, 9999, 100, 9999}
                    };

                    std::size_t pos = histogram_names[r][c][0].find("ptBin") + 5;
                    int pt_index = std::strtol(histogram_names[r][c][0].c_str() + pos, nullptr, 10);

                    if (hist_type.find("ptBinAll") == std::string::npos &&
                        (r == 0 && hist_type.find("centBinAll") == std::string::npos) &&
                        canvas_title != "xjg_cent") {
                        if (bins_pt[1][pt_index] < 9999) {
                            plotInfo.push_back(Form("p_{T}^{#gamma} #in (%d,%d) GeV/c", bins_pt[0][pt_index], bins_pt[1][pt_index]));
                        } else {
                            plotInfo.push_back(Form("p_{T}^{#gamma} > %d GeV/c", bins_pt[0][pt_index]));
                        }
                    }

                    if (r == 0 && hist_type.find("centBinAll") != std::string::npos)
                        tiler->draw_latex_on_frame(0.96, 0.96, Form("p_{T}^{#gamma} > %d GeV/c", bins_pt[0][pt_index]), 4, info_latex_size, 33, c, r);

                    if (hist_type.find("xjg_mean_rjg") != std::string::npos) {
                        int cent_label_bins[2][4] = { {50, 30, 10, 0}, {100, 50, 30, 10} };
                        float cent_label_pos[4] = {0.03, 0.3, 0.55, 0.81};

                        if (r == 1) {
                            tiler->draw_latex_on_frame(0.06, 0.15, "Cent.", 4, info_latex_size, 11, c, r);
                            for (int p=0; p<4; ++p) {
                                std::string cent_label = Form("%i-%i%%", cent_label_bins[0][p], cent_label_bins[1][p]);
                                tiler->draw_latex_on_frame(cent_label_pos[p], 0.08, cent_label.c_str(), 4, info_latex_size, 11, c, r);
                            }
                        }
                    }
                }

                if (columns < 4) {
                    plotInfo.push_back("anti-k_{T} jet R = 0.3");
                    plotInfo.push_back("p_{T}^{jet} > 30 GeV/c, #left|#eta^{jet}#right| < 1.6");
                    plotInfo.push_back("#left|#eta^{#gamma}#right| < 1.44");

                    if (hist_type != "dphi" && hist_type != "iaa" && hist_type != "ptJet")
                        plotInfo.back() += ", #Delta#phi_{j#gamma} > #frac{7#pi}{8}";
                }
            } else {
                int cent_bins[2][4] = {{50, 30, 10, 0}, {100, 50, 30, 10}};
                float purity[4] = {0.82, 0.77, 0.73, 0.68};
                float chi2[4] = {95, 119, 107, 178};

                plotInfo.push_back(Form("Cent. %i - %i%%", cent_bins[0][c], cent_bins[1][c]));
                plotInfo.push_back(Form("Purity: %.2f", purity[c]));
                plotInfo.push_back(Form("#chi^{2}/ndf: %.0f/78", chi2[c]));
            }

            float line_pos = i_y[r*columns + c];
            int latex_align = (i_x[r*columns + c] > 0.8) ? 33 : 11;
            for (std::size_t q=0; q<plotInfo.size(); ++q) {
                tiler->draw_latex_on_frame(i_x[r*columns + c], line_pos, plotInfo[q].c_str(), 4, info_latex_size, latex_align, c, r);
                line_pos -= info_latex_size * 1.44;
            }

            gPad->RedrawAxis();
        }
    }

    // Draw energy, lumi info, jet cuts on top
    c1->cd();

    // Cover cut-off axis labels
    tiler->cover_axis_labels(
        cover_options[0], cover_options[1],
        label_sizes[0], label_sizes[1],
        label_offsets[0], label_offsets[1]
    );

    float canvas_margin_left = tiler->get_canvas_margin_left();
    float canvas_margin_right = tiler->get_canvas_margin_right();
    float canvas_margin_top = tiler->get_canvas_margin_top();

    // draw CMS label
    tiler->draw_latex_on_canvas(canvas_margin_left + 0.006, 1.0 - canvas_margin_top / 2, "CMS", 6, cms_latex_size, 12);

    // draw common information
    std::string lumiInfo = "#sqrt{s_{NN}} = 5.02 TeV, PbPb 404 #mub^{-1}";
    if (hist_type != "purity" && canvas_title.find("theory") == std::string::npos) { lumiInfo += ", pp 27.4 pb^{-1}"; }
    tiler->draw_latex_on_canvas(1 - canvas_margin_right - 0.006, 1.0 - canvas_margin_top / 2, lumiInfo.c_str(), 4, canvas_latex_size, 32);

    std::string commonInfo;
    if (hist_type == "purity") {
        commonInfo = "p_{T}^{#gamma} > 60 GeV/c, |#eta^{#gamma}| < 1.44";
    } else if (columns > 3) {
        commonInfo = "anti-k_{T} jet R = 0.3, p_{T}^{jet} > 30 GeV/c, |#eta^{jet}| < 1.6, |#eta^{#gamma}| < 1.44";
        if (canvas_title == "xjg_cent")
            commonInfo += ", p_{T}^{#gamma} > 60 GeV/c";
        if (hist_type.find("dphi") == std::string::npos && hist_type != "iaa" && hist_type != "ptJet")
            commonInfo += ", #Delta#phi_{j#gamma} > #frac{7#pi}{8}";
    }
    float middle_align = (canvas_margin_left + 1.0 - canvas_margin_right) / 2;
    if (canvas_title.find("theory") == std::string::npos) {
        if (canvas_title == "xjg_cent") { middle_align -= 0.116; }
        else { middle_align -= 0.125; }
    }
    tiler->draw_latex_on_canvas(middle_align, 1.0 - canvas_margin_top / 2, commonInfo.c_str(), 4, canvas_latex_size, 22);

    c1->SaveAs(canvas_title.append(".pdf").c_str());

    return 0;
}

#define PBPB_COLOUR (TColor::GetColor("#E74C3C"))
#define PP_COLOUR (TColor::GetColor("#2DCE58"))

std::string set_systematics_style(TGraph* gr, int style) {
    switch (style) {
        case 0:
            gr->SetFillStyle(1001);
            gr->SetFillColorAlpha(PBPB_COLOUR, 0.7);
            return "same e x0";
        case 1:
            gr->SetFillStyle(1001);
            gr->SetFillColorAlpha(PP_COLOUR, 0.7);
            return "same e x0";
        case 2:
            gr->SetFillStyle(1001);
            gr->SetFillColorAlpha(PBPB_COLOUR, 0.7);
            return "same e x0";
        case 3:
            gr->SetFillStyle(1001);
            gr->SetFillColorAlpha(PP_COLOUR, 0.7);
            return "same e x0";
        case 4:
            gr->SetFillStyle(1001);
            gr->SetFillColorAlpha(PBPB_COLOUR, 0.7);
            return "same e x0";
        case 5:
            gr->SetFillStyle(1001);
            gr->SetFillColorAlpha(PP_COLOUR, 0.7);
            return "same e x0";
        case 6:
            gr->SetFillStyle(1001);
            gr->SetFillColorAlpha(PBPB_COLOUR, 0.7);
            return "same e x0";
        case 7:
            gr->SetFillStyle(1001);
            gr->SetFillColorAlpha(PP_COLOUR, 0.7);
            return "same e x0";
        case 12:
            gr->SetFillStyle(1001);
            gr->SetFillColorAlpha(PBPB_COLOUR, 0.7);
            return "same e0 x0";
        case 13:
            gr->SetFillStyle(1001);
            gr->SetFillColorAlpha(PP_COLOUR, 0.7);
            return "same e0 x0";
        case 14:
            gr->SetFillStyle(1001);
            gr->SetFillColorAlpha(PBPB_COLOUR, 0.7);
            return "same e0 x0";
        case 15:
            gr->SetFillStyle(1001);
            gr->SetFillColorAlpha(PP_COLOUR, 0.7);
            return "same e0 x0";
        default:
            gr->SetFillStyle(1001);
            return "same e x0";
    }
}

void set_histogram_style(TH1* h1, int style, std::vector<std::string>& option_strings) {
    switch (style) {
        case 0:     /* PbPb data 0-30% legend */
            h1->SetLineColorAlpha(PBPB_COLOUR, 0.7);
            h1->SetLineWidth(0);
            h1->SetMarkerColor(1);
            h1->SetMarkerStyle(20);
            h1->SetMarkerSize(1.75);
            h1->SetFillStyle(1001);
            h1->SetFillColorAlpha(PBPB_COLOUR, 0.7);
            option_strings.push_back("same e x0");
            option_strings.push_back("pf");
            break;
        case 1:     /* pp data 0-30% legend */
            h1->SetLineColorAlpha(PP_COLOUR, 0.7);
            h1->SetLineWidth(0);
            h1->SetMarkerColor(1);
            h1->SetMarkerStyle(24);
            h1->SetMarkerSize(1.75);
            h1->SetFillStyle(1001);
            h1->SetFillColorAlpha(PP_COLOUR, 0.7);
            option_strings.push_back("same e x0");
            option_strings.push_back("pf");
            break;
        case 2:     /* PbPb data 30-100% legend */
            h1->SetLineColorAlpha(PBPB_COLOUR, 0.7);
            h1->SetLineWidth(0);
            h1->SetMarkerColor(1);
            h1->SetMarkerStyle(21);
            h1->SetMarkerSize(1.75);
            h1->SetFillStyle(1001);
            h1->SetFillColorAlpha(PBPB_COLOUR, 0.7);
            option_strings.push_back("same e x0");
            option_strings.push_back("pf");
            break;
        case 3:     /* pp data 30-100% legend */
            h1->SetLineColorAlpha(PP_COLOUR, 0.7);
            h1->SetLineWidth(0);
            h1->SetMarkerColor(1);
            h1->SetMarkerStyle(25);
            h1->SetMarkerSize(1.75);
            h1->SetFillStyle(1001);
            h1->SetFillColorAlpha(PP_COLOUR, 0.7);
            option_strings.push_back("same e x0");
            option_strings.push_back("pf");
            break;
        case 4:     /* PbPb data 0-30% */
            h1->SetLineColor(1);
            h1->SetMarkerColor(1);
            h1->SetMarkerStyle(20);
            h1->SetMarkerSize(1.75);
            option_strings.push_back("same e x0");
            option_strings.push_back("pf");
            break;
        case 5:     /* pp data 0-30% */
            h1->SetLineColor(1);
            h1->SetMarkerColor(1);
            h1->SetMarkerStyle(24);
            h1->SetMarkerSize(1.75);
            option_strings.push_back("same e x0");
            option_strings.push_back("pf");
            break;
        case 6:     /* PbPb data 30-100% */
            h1->SetLineColor(1);
            h1->SetMarkerColor(1);
            h1->SetMarkerStyle(21);
            h1->SetMarkerSize(1.75);
            option_strings.push_back("same e x0");
            option_strings.push_back("pf");
            break;
        case 7:     /* pp data 30-100% */
            h1->SetLineColor(1);
            h1->SetMarkerColor(1);
            h1->SetMarkerStyle(25);
            h1->SetMarkerSize(1.75);
            option_strings.push_back("same e x0");
            option_strings.push_back("pf");
            break;
        case 8:     /* PbPb MC 30-100% */
            h1->SetLineColor(1);
            h1->SetLineStyle(1);
            h1->SetLineWidth(1.2);
            h1->SetMarkerSize(0);
            option_strings.push_back("same hist e x0");
            option_strings.push_back("l");
            break;
        case 12:    /* PbPb data 0-30% dphi */
            h1->SetLineColor(1);
            h1->SetLineWidth(1.2);
            h1->SetMarkerColor(1);
            h1->SetMarkerStyle(20);
            h1->SetMarkerSize(1.75);
            option_strings.push_back("same e0 x0");
            option_strings.push_back("pf");
            break;
        case 13:    /* pp data 0-30% dphi */
            h1->SetLineColor(1);
            h1->SetLineWidth(1.2);
            h1->SetMarkerColor(1);
            h1->SetMarkerStyle(24);
            h1->SetMarkerSize(1.75);
            option_strings.push_back("same e0 x0");
            option_strings.push_back("pf");
            break;
        case 14:    /* PbPb data 30-100% dphi */
            h1->SetLineColor(1);
            h1->SetLineWidth(1.2);
            h1->SetMarkerColor(1);
            h1->SetMarkerStyle(21);
            h1->SetMarkerSize(1.75);
            option_strings.push_back("same e0 x0");
            option_strings.push_back("pf");
            break;
        case 15:    /* pp data 30-100% dphi */
            h1->SetLineColor(1);
            h1->SetLineWidth(1.2);
            h1->SetMarkerColor(1);
            h1->SetMarkerStyle(25);
            h1->SetMarkerSize(1.75);
            option_strings.push_back("same e0 x0");
            option_strings.push_back("pf");
            break;
        case 16:    /* PbPb data purity */
            h1->SetLineColor(1);
            h1->SetLineWidth(1.2);
            h1->SetMarkerColor(1);
            h1->SetMarkerStyle(21);
            h1->SetMarkerSize(1.25);
            option_strings.push_back("same e x0");
            option_strings.push_back("pl");
            break;
        case 17:    /* signal template */
            h1->SetLineColor(TColor::GetColor("#e74c3c"));
            h1->SetFillColor(TColor::GetColor("#e74c3c"));
            h1->SetFillStyle(3004);
            option_strings.push_back("same hist");
            option_strings.push_back("lf");
            break;
        case 18:    /* background template */
            h1->SetLineWidth(1);
            h1->SetLineColor(TColor::GetColor("#2ecc71"));
            h1->SetFillColor(TColor::GetColor("#2ecc71"));
            h1->SetFillStyle(3001);
            option_strings.push_back("same hist");
            option_strings.push_back("lf");
            break;
        default:
            option_strings.push_back("same e x0");
            option_strings.push_back("p");
            break;
    }
}

void set_graph_style(TGraph* g1, int style, std::vector<std::string>& option_strings) {
    switch (style) {
        case 0:     /* PbPb data 0-30% legend */
            g1->SetLineColorAlpha(PBPB_COLOUR, 0.7);
            g1->SetLineWidth(0);
            g1->SetMarkerColor(1);
            g1->SetMarkerStyle(20);
            g1->SetMarkerSize(1.75);
            g1->SetFillStyle(1001);
            g1->SetFillColorAlpha(PBPB_COLOUR, 0.7);
            option_strings.push_back("same p z");
            option_strings.push_back("pf");
            break;
        case 1:     /* pp data 0-30% legend */
            g1->SetLineColorAlpha(PP_COLOUR, 0.7);
            g1->SetLineWidth(0);
            g1->SetMarkerColor(1);
            g1->SetMarkerStyle(24);
            g1->SetMarkerSize(1.75);
            g1->SetFillStyle(1001);
            g1->SetFillColorAlpha(PP_COLOUR, 0.7);
            option_strings.push_back("same p z");
            option_strings.push_back("pf");
            break;
        case 2:     /* PbPb data 30-100% legend */
            g1->SetLineColorAlpha(PBPB_COLOUR, 0.7);
            g1->SetLineWidth(0);
            g1->SetMarkerColor(1);
            g1->SetMarkerStyle(21);
            g1->SetMarkerSize(1.75);
            g1->SetFillStyle(1001);
            g1->SetFillColorAlpha(PBPB_COLOUR, 0.7);
            option_strings.push_back("same p z");
            option_strings.push_back("pf");
            break;
        case 3:     /* pp data 30-100% legend */
            g1->SetLineColorAlpha(PP_COLOUR, 0.7);
            g1->SetLineWidth(0);
            g1->SetMarkerColor(1);
            g1->SetMarkerStyle(25);
            g1->SetMarkerSize(1.75);
            g1->SetFillStyle(1001);
            g1->SetFillColorAlpha(PP_COLOUR, 0.7);
            option_strings.push_back("same p z");
            option_strings.push_back("pf");
            break;
        case 4:     /* PbPb data 0-30% */
            g1->SetLineColor(1);
            g1->SetMarkerColor(1);
            g1->SetMarkerStyle(20);
            g1->SetMarkerSize(1.75);
            option_strings.push_back("same p z");
            option_strings.push_back("pf");
            break;
        case 5:     /* pp data 0-30% */
            g1->SetLineColor(1);
            g1->SetMarkerColor(1);
            g1->SetMarkerStyle(24);
            g1->SetMarkerSize(1.75);
            option_strings.push_back("same p z");
            option_strings.push_back("pf");
            break;
        case 6:     /* PbPb data 30-100% */
            g1->SetLineColor(1);
            g1->SetMarkerColor(1);
            g1->SetMarkerStyle(21);
            g1->SetMarkerSize(1.75);
            option_strings.push_back("same p z");
            option_strings.push_back("pf");
            break;
        case 7:     /* pp data 30-100% */
            g1->SetLineColor(1);
            g1->SetMarkerColor(1);
            g1->SetMarkerStyle(25);
            g1->SetMarkerSize(1.75);
            option_strings.push_back("same p z");
            option_strings.push_back("pf");
            break;
        case 8:
            option_strings.push_back("same l z");
            option_strings.push_back("l");
            break;
        case 9:     /* JEWEL */
            g1->SetLineColor(9);
            g1->SetLineStyle(1);
            g1->SetLineWidth(1.2);
            g1->SetMarkerSize(0);
            option_strings.push_back("same l z");
            option_strings.push_back("l");
            break;
        case 10:    /* LBT */
            g1->SetLineColor(kOrange-3);
            g1->SetLineStyle(1);
            g1->SetLineWidth(1.2);
            g1->SetMarkerSize(0);
            option_strings.push_back("same l z");
            option_strings.push_back("l");
            break;
        case 11:    /* Hybrid */
            g1->SetLineColorAlpha(kTeal+9, 0.7);
            g1->SetLineStyle(1);
            g1->SetMarkerSize(0);
            g1->SetFillColorAlpha(kTeal+9, 0.7);
            option_strings.push_back("same l e3");
            option_strings.push_back("f");
            break;
        default:
            option_strings.push_back("same l z");
            option_strings.push_back("l");
            break;
    }
}

void draw_sys_unc(TGraph* gr, TH1* h1, TH1* h1_sys) {
    for (int i=1; i<=h1->GetNbinsX(); ++i) {
        if (h1->GetBinError(i) == 0) continue;

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

void draw_npart_sys_unc(TGraph* gr, TH1* h1, TH1* h1_sys, int x_width) {
    for (int i=1; i<=h1->GetNbinsX(); ++i) {
        if (h1->GetBinError(i) == 0) continue;

        double x = ncoll_w_npart[i-1];
        double val = h1->GetBinContent(i);
        double error = TMath::Abs(h1_sys->GetBinContent(i));

        if (val < 0)
            continue;

        gr->SetPoint(0, x - (x_width/2), std::max(val - error, h1->GetMinimum()));
        gr->SetPoint(1, x + (x_width/2), std::max(val - error, h1->GetMinimum()));
        gr->SetPoint(2, x + (x_width/2), std::min(val + error, h1->GetMaximum()));
        gr->SetPoint(3, x - (x_width/2), std::min(val + error, h1->GetMaximum()));

        gr->DrawClone("f");
    }
}

int main(int argc, char* argv[]) {
    if (argc == 5) {
        return gammaJetPlot(argv[1], argv[2], argv[3], argv[4]);
    } else {
        printf("Usage: ./Plotting/gammaJetPlot.exe [input file] [systematics file] [histogram list] [config file]\n");
        return 1;
    }
}
