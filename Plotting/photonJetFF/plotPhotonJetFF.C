#include "TFile.h"
#include "TH1.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TColor.h"
#include "TLine.h"
#include "TAxis.h"

#include <vector>
#include <string>
#include <fstream>

#include "../../Utilities/interface/CutConfigurationParser.h"
#include "../../Utilities/interface/InputConfigurationParser.h"
#include "../../Utilities/systemUtil.h"
#include "../../Data/photonJetFF/photonJetFFData.h"
#include "../../Utilities/tgraphUtil.h"

#include "../tiling.h"

#define PBPB_COLOUR     (TColor::GetColor("#ef5253"))
#define PP_COLOUR       (TColor::GetColor("#6699cc"))
#define RATIO_COLOUR    (TColor::GetColor("#a09f93"))

int min_hiBin[6] = {50, 30, 10, 0, 30, 0};
int max_hiBin[6] = {100, 50, 30, 10, 100, 30};

void draw_sys_unc(TGraph* gr, TH1* h1, TH1* h1_sys);
std::string set_systematics_style(TGraph* gr, int style);
void set_graph_style(TGraph* gr, int style, std::vector<std::string>& option_strings);
void set_histogram_style(TH1* h1, int style, std::vector<std::string>& option_strings);

int plotPhotonJetFF(const char* sys_file, const char* hist_list, const char* config_file) {
    gStyle->SetOptTitle(0);
    gStyle->SetOptStat(0);

    TFile* fsys = new TFile(sys_file, "read");

    InputConfiguration config = InputConfigurationParser::Parse(config_file);

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

    _GET_CONFIG_STRING_VECTOR(custom_info, 2);

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

    bool isJS = (hist_type.find("js") == 0);
    bool plotTheoryRatio = (hist_type.find("theory_ratio") != std::string::npos);
    bool plotTheoryPP = (hist_type.find("theory_pp") != std::string::npos);
    bool plotTheoryPbPb = (hist_type.find("theory_pbpb") != std::string::npos);
    bool plotTheory = (plotTheoryRatio || plotTheoryPP || plotTheoryPbPb);

    tiling* tiler = new tiling(columns, rows, 400, 400,
        margins[0], margins[1], margins[2], margins[3]);

    TCanvas* c1 = tiler->create_canvas("c1", "");
    TPad* emptyBox = 0;
    for (int r=0; r<rows; ++r) {
        for (int c=0; c<columns; ++c) {
            c1->cd(r*columns + c+1);
            if (set_log_scale[r]) { gPad->SetLogy(); }

            int nHistogram_names_Tmp = histogram_names[r][c].size();
            TObject* generic[histogram_names[r][c].size()];
            TH1D* histograms[histogram_names[r][c].size()];
            TGraph* graphs[histogram_names[r][c].size()];
            TGraphErrors* graphErrs[histogram_names[r][c].size()];
            for (int iName = 0; iName < nHistogram_names_Tmp; ++iName) {
                generic[iName] = 0;
                histograms[iName] = 0;
                graphs[iName] = 0;
                graphErrs[iName] = 0;
            }
            std::vector< std::vector< std::string > > option_strings(
                    nHistogram_names_Tmp,  std::vector< std::string >(0)
            );

            for (std::size_t l=0; l<(std::size_t)nHistogram_names_Tmp; ++l) {
                if ((int)l == clear_entries[r*columns + c]) { gPad->Clear(); }

                TH1D* systematics = 0;
                std::string systematicsName = (histogram_names[r][c][l] + "_systematics").c_str();
                if (histogram_names[r][c][l].find("ppdatareweight") != std::string::npos)
                    systematicsName = replaceAll(systematicsName, "ppdatareweight", "ppdata", false);
                else if (histogram_names[r][c][l].find("_reweight") != std::string::npos)
                    systematicsName = replaceAll(systematicsName, "_reweight", "", false);
                systematics = (TH1D*)fsys->Get(systematicsName.c_str());

                TGraph* gr = new TGraph();
                gr->SetFillStyle(1001);
                std::string sys_draw_options = set_systematics_style(gr, styles[r][c][l]);

                generic[l] = 0;
                generic[l] = fsys->Get((histogram_names[r][c][l]).c_str());
                if (generic[l] == 0)
                    generic[l] = fsys->Get((histogram_names[r][c][l] + "_nominal").c_str());

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

                    /* restrict x-axis range */
                    histograms[l]->SetAxisRange(0.5 + 0.001, 4.5 - 0.001, "X");   // FF
                    if (isJS)  histograms[l]->SetAxisRange(0 + 0.001, 0.3 - 0.001, "X");
                    histograms[l]->SetAxisRange(y_min[r], y_max[r], "Y");
                    histograms[l]->SetMinimum(y_min[r]);
                    histograms[l]->SetMaximum(y_max[r]);

                    /* tick mark settings */
                    histograms[l]->SetNdivisions(504, "X");
                    if (r == 0)
                        histograms[l]->SetNdivisions(505, "Y");

                    /* draw systematic uncertainties */
                    if (systematics && (int)l < draw_sys[r*columns + c]) {
                        /* draw frame for systematics */
                        histograms[l]->Draw(sys_draw_options.c_str());
                        draw_sys_unc(gr, histograms[l], systematics);
                    }

                    /* draw histogram */
                    histograms[l] = (TH1D*)histograms[l]->DrawCopy(option_strings[l][0].c_str());
                    histograms[l]->ResetBit(kCanDelete);
                }
                else if (generic[l]->InheritsFrom(TGraph::Class())) {
                    graphs[l] = (TGraph*)generic[l];

                    set_graph_style(graphs[l], styles[r][c][l], option_strings[l]);
                    graphs[l]->GetXaxis()->SetTitle(x_titles[0].c_str());
                    graphs[l]->GetYaxis()->SetTitle(y_titles[r].c_str());

                    /* restrict x-axis range */
                    graphs[l]->GetXaxis()->SetRange(0.5 + 0.001, 4.5 - 0.001);    // FF
                    if (isJS)  graphs[l]->GetXaxis()->SetRange(0 + 0.001, 0.3 - 0.001);
                    graphs[l]->GetYaxis()->SetRange(y_min[r], y_max[r]);
                    graphs[l]->SetMinimum(y_min[r]);
                    graphs[l]->SetMaximum(y_max[r]);

                    graphs[l]->DrawClone(option_strings[l][0].c_str());
                }
                else if (generic[l]->InheritsFrom(TGraphErrors::Class())) {
                    graphErrs[l] = (TGraphErrors*)generic[l];
                }
                else {
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
                        }
                        else if (graphs[t]) {
                            l1->AddEntry(graphs[t], legend_labels[s][t].c_str(),
                                    option_strings[t][1].c_str());
                            if (!plotTheoryPP && std::string(graphs[t]->GetName()).find("hybrid") != std::string::npos) {
                                l1->SetHeader("Hybrid");
                            }
                        }
                        else if (graphErrs[t]) {
                            l1->AddEntry(graphErrs[t], legend_labels[s][t].c_str(),
                                option_strings[t][1].c_str());
                            if (!plotTheoryPP && std::string(graphErrs[t]->GetName()).find("hybrid") != std::string::npos) {
                                l1->SetHeader("Hybrid");
                            }
                        }
                    }

                    l1->Draw();
                }
            }

            // draw CMS label
            if (c + r == 0) {
                tiler->draw_latex_on_frame(0.04, 0.96, "CMS", 6, cms_latex_size, 13, c, r);
                bool isPreliminary = true;  // FF
                bool isSupplementary = false; // FF
                if (isJS) {
                    isPreliminary = true;
                    isSupplementary = true;
                }
                if (isPreliminary)  tiler->draw_latex_on_frame(0.04, 0.96 - cms_latex_size * 0.89, "Preliminary", 5, cms_latex_size * 0.81, 13, c, r);
                if (!isPreliminary && isSupplementary)  tiler->draw_latex_on_frame(0.04, 0.96 - cms_latex_size * 0.89, "Supplementary", 5, cms_latex_size * 0.81, 13, c, r);
                if (isPreliminary && isSupplementary)  tiler->draw_latex_on_frame(0.04, 0.88 - cms_latex_size * 0.89, "Supplementary", 5, cms_latex_size * 0.81, 13, c, r);
            }

            if (r == 0) {
                std::vector<std::string> plotInfo;
                if (columns == 4)
                    plotInfo.push_back(Form("Cent. %d - %d%%", min_hiBin[c], max_hiBin[c]));
                else if (columns == 2 && plotTheory)
                    plotInfo.push_back(Form("Cent. %d - %d%%", min_hiBin[c+2], max_hiBin[c+2]));
                else
                    plotInfo.push_back(Form("Cent. %d - %d%%", min_hiBin[c+4], max_hiBin[c+4]));

                float line_pos = i_y[r*columns + c];
                int latex_align = (i_x[r*columns + c] > 0.8) ? 33 : 13;
                for (std::size_t q=0; q<plotInfo.size(); ++q) {
                    tiler->draw_latex_on_frame(i_x[r*columns + c], line_pos, plotInfo[q].c_str(), 6, info_latex_size, latex_align, c, r);
                    line_pos -= info_latex_size * 1.44;
                }
            }

            if (r == 1 || (plotTheoryRatio && r == 0)) {
                TLine* line = new TLine();
                line->SetLineStyle(2);
                line->SetLineWidth(1);
                if (isJS) {
                    line->DrawLine(0, 1, 0.3, 1);
                }
                else {
                    line->DrawLine(0.5, 1, 4.5, 1); // FF
                }
            }

            if (isJS && r == rows-1) {

                float tmp_margin_left = gPad->GetLeftMargin();
                float tmp_margin_right = gPad->GetRightMargin();
                float tmp_margin_bottom = gPad->GetBottomMargin();

                emptyBox = 0;
                if (c < columns - 1) {
                    emptyBox = new TPad("emptyBox", "", 1-0.1-tmp_margin_right,                tmp_margin_bottom-0.1,
                                                        std::min(1.0, 1.0 - tmp_margin_right), tmp_margin_bottom-0.02);
                    emptyBox->DrawClone();
                }
                if (c > 0) {
                    emptyBox = new TPad("emptyBox", "", std::max(0.0, tmp_margin_left - 0.1), tmp_margin_bottom-0.1,
                                                        tmp_margin_left+0.1, tmp_margin_bottom-0.02);
                    emptyBox->DrawClone();
                }
            }
        }
    }

    c1->cd();

    float canvas_margin_left = tiler->get_canvas_margin_left();
    float canvas_margin_right = tiler->get_canvas_margin_right();
    float canvas_margin_top = tiler->get_canvas_margin_top();

    if (columns == 4) {
        tiler->draw_latex_on_canvas(canvas_margin_left + 0.01, 1.0 - canvas_margin_top/2, "#sqrt{s_{NN}} = 5.02 TeV", 4, canvas_latex_size, 11);
        tiler->draw_latex_on_canvas(canvas_margin_left + 0.01, 1.0 - canvas_margin_top, "PbPb 404 #mub^{-1}, pp 27.4 pb^{-1}", 4, canvas_latex_size, 11);
        if (isJS) {
            tiler->draw_latex_on_canvas(0.99 - canvas_margin_right, 1.0 - canvas_margin_top/2, Form("p_{T}^{#gamma} > %i GeV/c, |#eta^{#gamma}| < 1.44, p_{T}^{trk} > 1 GeV/c", std::stoi(custom_info[1])), 4, canvas_latex_size, 31);
            tiler->draw_latex_on_canvas(0.99 - canvas_margin_right, 1.0 - canvas_margin_top, Form("anti-k_{T} jet R = 0.3, p_{T}^{jet} > %i GeV/c, #left|#eta^{jet}#right| < 1.6, #Delta#phi_{j#gamma} > #frac{7#pi}{8}", std::stoi(custom_info[0])), 4, canvas_latex_size, 31);
        }
        else {
            tiler->draw_latex_on_canvas(0.99 - canvas_margin_right, 1.0 - canvas_margin_top/2, Form("p_{T}^{trk} > 1 GeV/c, anti-k_{T} jet R = 0.3, p_{T}^{jet} > %i GeV/c, #left|#eta^{jet}#right| < 1.6", std::stoi(custom_info[0])), 4, canvas_latex_size, 31);
            tiler->draw_latex_on_canvas(0.99 - canvas_margin_right, 1.0 - canvas_margin_top, Form("p_{T}^{#gamma} > %i GeV/c, |#eta^{#gamma}| < 1.44, #Delta#phi_{j#gamma} > #frac{7#pi}{8}", std::stoi(custom_info[1])), 4, canvas_latex_size, 31);
        }
    }
    else {
        tiler->draw_latex_on_canvas(canvas_margin_left + 0.01, 1.0 - canvas_margin_top/3, "#sqrt{s_{NN}} = 5.02 TeV", 4, canvas_latex_size, 11);
        if (plotTheoryPP) {
            tiler->draw_latex_on_canvas(canvas_margin_left + 0.01, 1.0 - canvas_margin_top/3*2, "pp 27.4 pb^{-1}", 4, canvas_latex_size, 11);
        }
        else if (plotTheoryPbPb) {
            tiler->draw_latex_on_canvas(canvas_margin_left + 0.01, 1.0 - canvas_margin_top/3*2, "PbPb 404 #mub^{-1}", 4, canvas_latex_size, 11);
        }
        else {
            tiler->draw_latex_on_canvas(canvas_margin_left + 0.01, 1.0 - canvas_margin_top/3*2, "PbPb 404 #mub^{-1}", 4, canvas_latex_size, 11);
            tiler->draw_latex_on_canvas(canvas_margin_left + 0.01, 1.0 - canvas_margin_top, "pp 27.4 pb^{-1}", 4, canvas_latex_size, 11);
        }
        if (isJS) {
            tiler->draw_latex_on_canvas(0.99 - canvas_margin_right, 1.0 - canvas_margin_top/3, Form("p_{T}^{#gamma} > %i GeV/c, |#eta^{#gamma}| < 1.44", std::stoi(custom_info[1])), 4, canvas_latex_size, 31);
            tiler->draw_latex_on_canvas(0.99 - canvas_margin_right, 1.0 - canvas_margin_top/3*2, Form("p_{T}^{trk} > 1 GeV/c, anti-k_{T} jet R = 0.3"), 4, canvas_latex_size, 31);
            tiler->draw_latex_on_canvas(0.99 - canvas_margin_right, 1.0 - canvas_margin_top, Form("p_{T}^{jet} > %i GeV/c, #left|#eta^{jet}#right| < 1.6, #Delta#phi_{j#gamma} > #frac{7#pi}{8}", std::stoi(custom_info[0])), 4, canvas_latex_size, 31);
        }
        else {
            tiler->draw_latex_on_canvas(0.99 - canvas_margin_right, 1.0 - canvas_margin_top/3, Form("p_{T}^{trk} > 1 GeV/c, anti-k_{T} jet R = 0.3"), 4, canvas_latex_size, 31);
            tiler->draw_latex_on_canvas(0.99 - canvas_margin_right, 1.0 - canvas_margin_top/3*2, Form("p_{T}^{jet} > %i GeV/c, #left|#eta^{jet}#right| < 1.6", std::stoi(custom_info[0])), 4, canvas_latex_size, 31);
            tiler->draw_latex_on_canvas(0.99 - canvas_margin_right, 1.0 - canvas_margin_top, Form("p_{T}^{#gamma} > %i GeV/c, |#eta^{#gamma}| < 1.44, #Delta#phi_{j#gamma} > #frac{7#pi}{8}", std::stoi(custom_info[1])), 4, canvas_latex_size, 31);
        }
    }

    tiler->cover_axis_labels(
        cover_options[0], cover_options[1],
        label_sizes[0], label_sizes[1],
        label_offsets[0], label_offsets[1]
    );

    c1->SaveAs(Form("%s.pdf", canvas_title.c_str()));

    fsys->Close();

    return 0;
}

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
            gr->SetFillColorAlpha(RATIO_COLOUR, 0.7);
            return "same e x0";
        case 5:
            gr->SetFillStyle(1001);
            gr->SetFillColorAlpha(RATIO_COLOUR, 0.7);
            return "same e x0";
        default:
            gr->SetFillStyle(1001);
            gr->SetFillColor(1);
            return "same e x0";
    }
}

void set_graph_style(TGraph* gr, int style, std::vector<std::string>& option_strings) {

    if (10 <= style && style < 20) {
        gr->SetFillColorAlpha(SCET::colors[style - 10], SCET::falphas[style - 10]);
        gr->SetLineColorAlpha(SCET::colors[style - 10], SCET::falphas[style - 10]);
        gr->SetLineWidth(0);
        gr->SetFillStyle(1001);
        option_strings.push_back("same f");
        option_strings.push_back("f");
    }
    else if (20 <= style && style < 30) {
        gr->SetFillColorAlpha(COLBT::colors[style - 20], COLBT::falphas[style - 20]);
        gr->SetLineColorAlpha(COLBT::colors[style - 20], COLBT::falphas[style - 20]);
        gr->SetLineWidth(3);
        gr->SetFillStyle(1001);
        option_strings.push_back("same l");
        option_strings.push_back("l");
    }
    else if (30 <= style && style < 50) {
        gr->SetFillColorAlpha(HYBRID::colors[style - 30], HYBRID::falphas[style - 30]);
        gr->SetLineColorAlpha(HYBRID::colors[style - 30], HYBRID::falphas[style - 30]);
        gr->SetLineWidth(0);
        gr->SetFillStyle(1001);
        if ((style - 30) % 3 == 2) {
            gr->SetLineWidth(3);
            option_strings.push_back("same l");
            option_strings.push_back("l");
        }
        else {
            option_strings.push_back("same f");
            option_strings.push_back("f");
        }
    }
    else {
        gr->SetLineWidth(0);
        option_strings.push_back("f");
        option_strings.push_back("f");
    }
}

void set_histogram_style(TH1* h1, int style, std::vector<std::string>& option_strings) {
    switch (style) {
        case 0:     /* PbPb data (legend) */
            h1->SetLineColorAlpha(PBPB_COLOUR, 0);
            h1->SetLineWidth(0);
            h1->SetMarkerColor(1);
            h1->SetMarkerStyle(kFullCircle);
            h1->SetMarkerSize(1.75);
            h1->SetFillStyle(1001);
            h1->SetFillColorAlpha(PBPB_COLOUR, 0.7);
            option_strings.push_back("same e x0");
            option_strings.push_back("pf");
            break;
        case 1:     /* pp data (legend) */
            h1->SetLineColorAlpha(PP_COLOUR, 0);
            h1->SetLineWidth(0);
            h1->SetMarkerColor(1);
            h1->SetMarkerStyle(kOpenCircle);
            h1->SetMarkerSize(1.75);
            h1->SetFillStyle(1001);
            h1->SetFillColorAlpha(PP_COLOUR, 0.7);
            option_strings.push_back("same e x0");
            option_strings.push_back("pf");
            break;
        case 2:     /* PbPb data */
            h1->SetLineColor(1);
            h1->SetLineWidth(1.2);
            h1->SetMarkerColor(1);
            h1->SetMarkerStyle(kFullCircle);
            h1->SetMarkerSize(1.75);
            option_strings.push_back("same e x0");
            option_strings.push_back("pf");
            break;
        case 3:     /* pp data */
            h1->SetLineColor(1);
            h1->SetLineWidth(1.2);
            h1->SetMarkerColor(1);
            h1->SetMarkerStyle(kOpenCircle);
            h1->SetMarkerSize(1.75);
            option_strings.push_back("same e x0");
            option_strings.push_back("pf");
            break;
        case 4:     /* systematics (legend) */
            h1->SetLineColorAlpha(RATIO_COLOUR, 0);
            h1->SetLineWidth(0);
            h1->SetMarkerColor(1);
            h1->SetMarkerStyle(kFullSquare);
            h1->SetMarkerSize(1.5);
            h1->SetFillStyle(1001);
            h1->SetFillColorAlpha(RATIO_COLOUR, 0.7);
            option_strings.push_back("same e x0");
            option_strings.push_back("pf");
            break;
        case 5:     /* systematics */
            h1->SetLineColor(1);
            h1->SetLineWidth(1.2);
            h1->SetMarkerColor(1);
            h1->SetMarkerStyle(kFullSquare);
            h1->SetMarkerSize(1.5);
            option_strings.push_back("same e x0");
            option_strings.push_back("pf");
            break;
        default:
            option_strings.push_back("same e x0");
            option_strings.push_back("p");
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

        gr->SetPoint(0, x - (bin_width/2), val - error);
        gr->SetPoint(1, x + (bin_width/2), val - error);
        gr->SetPoint(2, x + (bin_width/2), val + error);
        gr->SetPoint(3, x - (bin_width/2), val + error);

        gr->DrawClone("f");
    }
}

int main(int argc, char* argv[]) {
    if (argc == 4)
        return plotPhotonJetFF(argv[1], argv[2], argv[3]);
    else
        printf("./plotPhotonJetFF [systematics file] [histogram list] [config file]\n");

    return 1;
}
