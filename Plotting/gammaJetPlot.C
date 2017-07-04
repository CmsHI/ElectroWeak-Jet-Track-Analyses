#include "TFile.h"
#include "TH1.h"
#include "TGraphErrors.h"

#include "TStyle.h"
#include "TLine.h"

#include "../Utilities/interface/CutConfigurationParser.h"
#include "../Utilities/interface/InputConfigurationParser.h"

#include <string>
#include <vector>
#include <fstream>

#include "tiling.h"

const float ncoll_weighted_npart[4] = {43.58, 118.8, 239.9, 363.4};

const float cms_latex_size = 0.07;
const float canvas_latex_size = 0.08;
const float latex_size = 0.064;

std::string set_systematics_style(TGraph* gr, int style);
void set_histogram_style(TH1* h1, int style, std::vector<std::string>& option_strings);
void set_graph_style(TGraph* g1, int style, std::vector<std::string>& option_strings);

void draw_sys_unc(TGraph* gr, TH1* h1, TH1* h1_sys, int first_bin = 1);

std::vector<std::string> legend_draw_options;

int gammaJetPlot(const std::string hist_list, const std::string input_file, const std::string sys_file, const std::string config_file) {
    gStyle->SetOptTitle(0);
    gStyle->SetOptStat(0);

    TFile* input = new TFile(input_file.c_str(), "read");
    TFile* sys = new TFile(sys_file.c_str(), "read");

    InputConfiguration config = InputConfigurationParser::Parse(config_file.c_str());

#define _GET_CONFIG_INT(value) int value = config.proc[INPUT::kPLOTTING].i[INPUT::k_mpp_##value];
#define _GET_CONFIG_FLOAT(value) float value = config.proc[INPUT::kPLOTTING].f[INPUT::k_mpp_##value];
#define _GET_CONFIG_STRING(value) std::string value = config.proc[INPUT::kPLOTTING].s[INPUT::k_mpp_##value];

#define _GET_CONFIG_INT_VECTOR(value) std::vector<int> value = ConfigurationParser::ParseListInteger(config.proc[INPUT::kPLOTTING].str_i[INPUT::k_mpp_##value]);
#define _GET_CONFIG_FLOAT_VECTOR(value) std::vector<float> value = ConfigurationParser::ParseListFloat(config.proc[INPUT::kPLOTTING].str_f[INPUT::k_mpp_##value]);

    _GET_CONFIG_INT(rows);
    _GET_CONFIG_INT(columns);

    _GET_CONFIG_FLOAT_VECTOR(margins);
    _GET_CONFIG_FLOAT_VECTOR(title_offsets);
    _GET_CONFIG_FLOAT_VECTOR(label_offsets);
    _GET_CONFIG_FLOAT_VECTOR(title_sizes);
    _GET_CONFIG_FLOAT_VECTOR(label_sizes);
    _GET_CONFIG_FLOAT_VECTOR(tick_sizes);
    _GET_CONFIG_INT_VECTOR(cover_options);

    _GET_CONFIG_INT_VECTOR(set_log_scale);
    _GET_CONFIG_FLOAT_VECTOR(y_min);
    _GET_CONFIG_FLOAT_VECTOR(y_max);

    _GET_CONFIG_INT_VECTOR(draw_sys);
    _GET_CONFIG_INT_VECTOR(clear_entries);

    _GET_CONFIG_INT_VECTOR(l_panel);
    _GET_CONFIG_FLOAT_VECTOR(l_x1);
    _GET_CONFIG_FLOAT_VECTOR(l_y1);
    _GET_CONFIG_FLOAT_VECTOR(l_x2);
    _GET_CONFIG_FLOAT_VECTOR(l_y2);

    _GET_CONFIG_FLOAT_VECTOR(i_x);
    _GET_CONFIG_FLOAT_VECTOR(i_y);

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
                histogram_names[r][c].size(),  std::vector< std::string >(0)
            );

            for (std::size_t l=0; l<histogram_names[r][c].size(); ++l) {
                if ((int)l == clear_entries[r*columns + c]) { gPad->Clear(); }

                TH1D* systematics = 0;
                systematics = (TH1D*)sys->Get((histogram_names[r][c][l] + "_diff_total").c_str());

                TGraph* gr = new TGraph();
                gr->SetFillStyle(1001);
                std::string sys_draw_options = set_systematics_style(gr, styles[r][c][l]);

                generic[l] = input->Get(histogram_names[r][c][l].c_str());

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

                    histograms[l]->SetAxisRange(y_min[r], y_max[r], "Y");
                    histograms[l]->SetMinimum(y_min[r]);
                    histograms[l]->SetMaximum(y_max[r]);

                    /* special case for all xjg plots */
                    if (hist_type == "xjg")
                        histograms[l]->SetNdivisions(504);

                    /* draw systematic uncertainties */
                    if (systematics && (int)l < draw_sys[r*columns + c]) {
                        /* draw frame for systematics */
                        histograms[l]->Draw(sys_draw_options.c_str());
                        draw_sys_unc(gr, histograms[l], systematics);
                    }

                    /* draw histogram */
                    histograms[l]->Draw(option_strings[l][0].c_str());
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

                    /* draw graph */
                    graphs[l]->Draw(option_strings[l][0].c_str());
                } else {
                    printf("unknown object type!\n");
                    return 1;
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

            for (std::size_t s=0; s<l_panel.size(); ++s) {
                if (r*columns + c+1 == l_panel[s]) {
                    TLegend* l1 = tiler->create_legend_on_frame(
                        l_x1[s], l_y1[s], l_x2[s], l_y2[s],
                        4, 0.06, c, r
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

            // draw CMS label
            if (c + r == 0 || columns < 4) {
                float CMS_label_x = 0.04;
                if (hist_type == "xjg_mean_ptBinAll" || hist_type == "rjg_ptBinAll")
                    CMS_label_x = 0.82;
                tiler->create_latex_on_frame(
                    CMS_label_x, 0.9, "CMS", 6,
                    cms_latex_size, 11, c, r
                );
            }

            // draw latex info
            std::vector<std::string> plotInfo;

            if (histogram_names[r][c][0].find("centBinAll") == std::string::npos) {
                int bins_cent[2][7] = {
                    {0, 0, 30, 0, 10, 30, 60},
                    {100, 30, 100, 10, 30, 60, 100}
                };

                std::size_t pos = histogram_names[r][c][0].find("hiBin") + 5;
                int cent_index = std::strtol(histogram_names[r][c][0].c_str() + pos, nullptr, 10);

                if (canvas_title == "xjg_cent")
                    plotInfo.push_back(Form("Cent. %d - %d%%", bins_cent[0][cent_index]/2, bins_cent[1][cent_index]/2));

                if (hist_type.find("xjg_mean_rjg") != std::string::npos) {
                    int cent_label_bins[2][2] = { {50, 0}, {100, 10} };
                    for (int p=0; p<2; ++p) {
                        std::string cent_label = Form("%i-%i%%", cent_label_bins[0][p], cent_label_bins[1][p]);

                        if (r == 0) {
                            tiler->create_latex_on_frame(
                                0.06 + p * 0.78, 0.32 + c * 0.23 - (!c) * p * 0.15, "Cent.", 4,
                                latex_size, 11, c, r
                            );
                            tiler->create_latex_on_frame(
                                0.04 + p * 0.78, 0.27 + c * 0.23 - (!c) * p * 0.15, cent_label.c_str(), 4,
                                latex_size, 11, c, r
                            );
                        } else if (r == 1) {
                            tiler->create_latex_on_frame(
                                0.06 + p * 0.78, 0.35 + c * 0.13 - p * 0.13, "Cent.", 4,
                                latex_size, 11, c, r
                            );
                            tiler->create_latex_on_frame(
                                0.04 + p * 0.78, 0.3 + c * 0.13 - p * 0.13, cent_label.c_str(), 4,
                                latex_size, 11, c, r
                            );
                        }
                    }
                }
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
            }

            if (columns < 4) {
                plotInfo.push_back("anti-k_{T} jet R = 0.3");
                if (hist_type.find("BinAll") != std::string::npos) {
                    plotInfo.push_back("p_{T}^{jet} > 30 GeV/c, #left|#eta^{jet}#right| < 1.6");
                } else {
                    plotInfo.push_back("p_{T}^{jet} > 30 GeV/c");
                    plotInfo.push_back("#left|#eta^{jet}#right| < 1.6");
                }

                if (hist_type.find("dphi") == std::string::npos && hist_type != "iaa" && hist_type != "ptJet")
                    plotInfo.push_back("#Delta#phi_{j#gamma} > #frac{7#pi}{8}");
            }

            float line_pos = i_y[r*columns + c];
            int latex_align = (i_x[r*columns + c] > 0.8) ? 31 : 11;
            for (std::size_t q=0; q<plotInfo.size(); ++q) {
                tiler->create_latex_on_frame(i_x[r*columns + c], line_pos, plotInfo[q].c_str(), 4, latex_size, latex_align, c, r);
                line_pos -= latex_size;
            }

            /* draw pt label for xjg_cent */
            if (canvas_title == "xjg_cent") {
                tiler->create_latex_on_frame(
                    0.43, 0.82, "p_{T}^{#gamma} > 60 GeV/c", 4,
                    latex_size, 11, c, r
                );
            }
        }
    }

    // Draw energy, lumi info, jet cuts on top
    c1->cd();

    float canvas_margin_left = tiler->get_canvas_margin_left();
    float canvas_margin_right = tiler->get_canvas_margin_right();
    float canvas_margin_top = tiler->get_canvas_margin_top();

    tiler->create_latex_on_canvas(canvas_margin_left + 0.01, 1.0 - canvas_margin_top + tiler->normalize_size(0.06, -1, -1), "#sqrt{s_{NN}} = 5.02 TeV", 4, canvas_latex_size, 11);
    tiler->create_latex_on_canvas(1 - canvas_margin_right - 0.01, 1.0 - canvas_margin_top + tiler->normalize_size(0.06, -1, -1), "PbPb 404 #mub^{-1}, pp 27.4 pb^{-1}", 4, canvas_latex_size, 31);

    std::string commonInfo;
    if (columns > 3) {
        commonInfo = "anti-k_{T} jet R = 0.3, p_{T}^{jet} > 30 GeV/c, #left|#eta^{jet}#right| < 1.6";
        if (hist_type.find("dphi") == std::string::npos && hist_type != "iaa" && hist_type != "ptJet")
            commonInfo += ", #Delta#phi_{j#gamma} > #frac{7#pi}{8}";
    }

    tiler->create_latex_on_canvas((canvas_margin_left + 1.0 - canvas_margin_right) / 2, 1.0 - canvas_margin_top + tiler->normalize_size(0.06, -1, -1), commonInfo.c_str(), 4, canvas_latex_size, 21);

    // Cover cut-off axis labels
    tiler->cover_axis(cover_options[0], cover_options[1]);

    c1->SaveAs(canvas_title.append(".pdf").c_str());

    return 0;
}

std::string set_systematics_style(TGraph* gr, int style) {
    switch (style) {
        /* PbPb data 0-30% */
        case 0:
            gr->SetFillStyle(1001);
            gr->SetFillColorAlpha(46, 0.7);
            return "same e x0";
        /* pp data 0-30% */
        case 1:
            gr->SetFillStyle(1001);
            gr->SetFillColorAlpha(30, 0.7);
            return "same e x0";
        /* PbPb data 30-100% */
        case 2:
            gr->SetFillStyle(1001);
            gr->SetFillColorAlpha(46, 0.7);
            return "same e x0";
        /* pp data 30-100% */
        case 3:
            gr->SetFillStyle(1001);
            gr->SetFillColorAlpha(30, 0.7);
            return "same e x0";
        /* PbPb data 0-30% */
        case 4:
            gr->SetFillStyle(1001);
            gr->SetFillColorAlpha(46, 0.7);
            return "same e x0";
        /* pp data 0-30% */
        case 5:
            gr->SetFillStyle(1001);
            gr->SetFillColorAlpha(30, 0.7);
            return "same e x0";
        /* PbPb data 30-100% */
        case 6:
            gr->SetFillStyle(1001);
            gr->SetFillColorAlpha(46, 0.7);
            return "same e x0";
        /* pp data 30-100% */
        case 7:
            gr->SetFillStyle(1001);
            gr->SetFillColorAlpha(30, 0.7);
            return "same e x0";
        default:
            gr->SetFillStyle(1001);
            return "same e x0";
    }
}

void set_histogram_style(TH1* h1, int style, std::vector<std::string>& option_strings) {
    switch (style) {
        /* PbPb data 0-30% legend */
        case 0:
            h1->SetLineWidth(0);
            h1->SetLineColorAlpha(46, 0.7);
            h1->SetMarkerColor(1);
            h1->SetMarkerStyle(20);
            h1->SetMarkerSize(1.2);
            h1->SetFillStyle(1001);
            h1->SetFillColorAlpha(46, 0.7);
            option_strings.push_back("same e x0");
            option_strings.push_back("pf");
            break;
        /* pp data 0-30% legend */
        case 1:
            h1->SetLineWidth(0);
            h1->SetLineColorAlpha(30, 0.7);
            h1->SetMarkerColor(1);
            h1->SetMarkerStyle(24);
            h1->SetMarkerSize(1.2);
            h1->SetFillStyle(1001);
            h1->SetFillColorAlpha(30, 0.7);
            option_strings.push_back("same e x0");
            option_strings.push_back("pf");
            break;
        /* PbPb data 30-100% legend */
        case 2:
            h1->SetLineWidth(0);
            h1->SetLineColorAlpha(46, 0.7);
            h1->SetMarkerColor(1);
            h1->SetMarkerStyle(21);
            h1->SetMarkerSize(1.2);
            h1->SetFillStyle(1001);
            h1->SetFillColorAlpha(46, 0.7);
            option_strings.push_back("same e x0");
            option_strings.push_back("pf");
            break;
        /* pp data 30-100% legend */
        case 3:
            h1->SetLineWidth(0);
            h1->SetLineColorAlpha(30, 0.7);
            h1->SetMarkerColor(1);
            h1->SetMarkerStyle(25);
            h1->SetMarkerSize(1.2);
            h1->SetFillStyle(1001);
            h1->SetFillColorAlpha(30, 0.7);
            option_strings.push_back("same e x0");
            option_strings.push_back("pf");
            break;
        /* PbPb data 0-30% */
        case 4:
            h1->SetLineColor(1);
            h1->SetMarkerColor(1);
            h1->SetMarkerStyle(20);
            h1->SetMarkerSize(1.2);
            option_strings.push_back("same e x0");
            option_strings.push_back("pf");
            break;
        /* pp data 0-30% */
        case 5:
            h1->SetLineColor(1);
            h1->SetMarkerColor(1);
            h1->SetMarkerStyle(24);
            h1->SetMarkerSize(1.2);
            option_strings.push_back("same e x0");
            option_strings.push_back("pf");
            break;
        /* PbPb data 30-100% */
        case 6:
            h1->SetLineColor(1);
            h1->SetMarkerColor(1);
            h1->SetMarkerStyle(21);
            h1->SetMarkerSize(1.2);
            option_strings.push_back("same e x0");
            option_strings.push_back("pf");
            break;
        /* pp data 30-100% */
        case 7:
            h1->SetLineColor(1);
            h1->SetMarkerColor(1);
            h1->SetMarkerStyle(25);
            h1->SetMarkerSize(1.2);
            option_strings.push_back("same e x0");
            option_strings.push_back("pf");
            break;
        /* pp data 30-100% */
        case 8:
            h1->SetLineColor(1);
            h1->SetLineStyle(1);
            h1->SetLineWidth(1.2);
            h1->SetMarkerSize(0);
            option_strings.push_back("same hist e x0");
            option_strings.push_back("l");
            break;
        default:
            option_strings.push_back("same e x0");
            option_strings.push_back("p");
            break;
    }
}

void set_graph_style(TGraph* g1, int style, std::vector<std::string>& option_strings) {
    switch (style) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
            break;
        case 9:
            g1->SetLineColor(9);
            g1->SetLineStyle(1);
            g1->SetLineWidth(1.2);
            g1->SetMarkerSize(0);
            option_strings.push_back("same l z");
            option_strings.push_back("l");
            break;
        case 10:
            g1->SetLineColor(kOrange-3);
            g1->SetLineStyle(1);
            g1->SetLineWidth(1.2);
            g1->SetMarkerSize(0);
            option_strings.push_back("same l z");
            option_strings.push_back("l");
            break;
        case 11:
            g1->SetLineColorAlpha(kTeal+9, 0.7);
            g1->SetLineStyle(1);
            g1->SetLineWidth(0);
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

void draw_sys_unc(TGraph* gr, TH1* h1, TH1* h1_sys, int first_bin) {
    int nBins = h1->GetNbinsX();
    for (int i=first_bin; i<=nBins; ++i) {
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

int main(int argc, char* argv[]) {
    if (argc > 4) {
        for (int p=4; p<argc; ++p) {
            gammaJetPlot(argv[1], argv[2], argv[3], argv[p]);
        }
    } else {
        printf("Usage: ./Plotting/gammaJetPlot.exe [histogram list] [input file] [systematics file] [config file]\n");
    }

    return 0;
}
