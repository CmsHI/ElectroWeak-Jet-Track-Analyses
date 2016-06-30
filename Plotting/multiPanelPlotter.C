#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLegend.h"
#include "TPaveText.h"

#include "../Utilities/interface/CutConfigurationParser.h"
#include "../Utilities/interface/InputConfigurationParser.h"

typedef struct box_t {
    float x1, y1, x2, y2;
} box_t;

void divide_canvas(TCanvas* c1, int rows, int columns, float margin, float edge);
void set_hist_style(TH1D* h1, int h, TString configFile);
void set_axis_style(TH1D* h1, int i, int j, int rows);
void set_legend_style(TLegend* l1);
void set_title_style(TPaveText* t1);
void adjust_coordinates(box_t& box, float margin, float edge, int i, int j, int rows, int columns);

int multiPanelPlotter(const TString configFile) {
    gStyle->SetOptTitle(0);
    gStyle->SetOptStat(0);

    InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());

    std::string file_name = configInput.proc[INPUT::kPLOTTING].s[INPUT::k_mpp_file_name].c_str();

    int columns = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_mpp_columns];
    int rows = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_mpp_rows];
    int layers = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_mpp_layers];

    int hist_width = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_mpp_hist_width];
    int hist_height = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_mpp_hist_height];

    float margin = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_mpp_margin];
    float edge = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_mpp_edge];

    std::vector<std::string> hist_names = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_mpp_hist_names]);
    std::vector<std::string> draw_options = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_mpp_draw_options]);
    std::vector<int> is_tf1 = ConfigurationParser::ParseListInteger(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_mpp_is_tf1]);

    std::vector<std::string> legend_labels = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_mpp_legend_labels]);
    std::vector<std::string> legend_options = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_mpp_legend_options]);

    std::vector<std::string> pad_titles = ConfigurationParser::ParseListLatex(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_mpp_pad_titles]);

    std::vector<float> y_max = ConfigurationParser::ParseListFloat(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_mpp_y_max]);
    std::vector<float> y_min = ConfigurationParser::ParseListFloat(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_mpp_y_min]);

    std::vector<float> l_x1 = ConfigurationParser::ParseListFloat(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_mpp_l_x1]);
    std::vector<float> l_y1 = ConfigurationParser::ParseListFloat(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_mpp_l_y1]);
    std::vector<float> l_x2 = ConfigurationParser::ParseListFloat(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_mpp_l_x2]);
    std::vector<float> l_y2 = ConfigurationParser::ParseListFloat(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_mpp_l_y2]);

    std::string canvas_title = configInput.proc[INPUT::kPLOTTING].s[INPUT::k_mpp_canvas_title].c_str();

    TFile* histogramFile = new TFile(file_name.c_str(), "read");

    float pad_width = (columns > 1) ? hist_width*(1.0/(1.0-margin) + 1.0/(1.0-edge) + columns - 2) : hist_width/(1.0-margin-edge);
    float pad_height = (rows > 1) ? hist_height*(1.0/(1.0-margin) + 1.0/(1.0-edge) + rows - 2) : hist_height/(1.0-margin-edge);

    TCanvas* c1 = new TCanvas("c1", "", (int)pad_width, (int)pad_height);
    divide_canvas(c1, rows, columns, margin, edge);

    TH1D* h1[rows][columns][layers];
    TLegend* l1[rows][columns];
    TPaveText* t1[rows][columns];

    int h = 0;
    for (int i=0; i<rows; ++i) {
        for (int j=0; j<columns; ++j) {
            c1->cd(i*columns+j+1);

            box_t l_box = (box_t) {
                l_x1[i*columns+j],
                l_y1[i*columns+j],
                l_x2[i*columns+j],
                l_y2[i*columns+j]
            };
            adjust_coordinates(l_box, margin, edge, i, j, rows, columns);
            l1[i][j] = new TLegend(l_box.x1, l_box.y1, l_box.x2, l_box.y2);
            set_legend_style(l1[i][j]);

            for (int k=0; k<layers; ++k, ++h) {
                h1[i][j][k] = (TH1D*)histogramFile->Get(hist_names[h].c_str());
                if (!is_tf1.size() || (is_tf1.size() && !is_tf1[h])) {
                    set_hist_style(h1[i][j][k], h, configFile);
                    set_axis_style(h1[i][j][k], i, j, rows);
                }

                h1[i][j][k]->SetTitle("");
                if (y_max.size())
                    h1[i][j][k]->SetAxisRange(y_min[h], y_max[h], "Y");
                h1[i][j][k]->Draw(draw_options[h].c_str());

                if (legend_labels.size())
                    l1[i][j]->AddEntry(h1[i][j][k], legend_labels[h].c_str(), legend_options[h].c_str());
            }

            l1[i][j]->Draw();

            box_t t_box = (box_t) {0, 0.9, 1, 1};
            adjust_coordinates(t_box, margin, edge, i, j, rows, columns);
            t1[i][j] = new TPaveText(t_box.x1, t_box.y1, t_box.x2, t_box.y2, "NDC");
            set_title_style(t1[i][j]);

            t1[i][j]->AddText(pad_titles[i*columns+j].c_str());
            t1[i][j]->Draw();
        }
    }

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
        }
    }
}

void set_hist_style(TH1D* h1, int h, TString configFile) {
    InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());

    std::vector<int> marker_style = ConfigurationParser::ParseListInteger(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_mpp_marker_style]);
    std::vector<int> marker_color = ConfigurationParser::ParseListInteger(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_mpp_marker_color]);
    std::vector<float> marker_size = ConfigurationParser::ParseListFloat(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_mpp_marker_size]);
    std::vector<int> line_style = ConfigurationParser::ParseListInteger(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_mpp_line_style]);
    std::vector<int> line_color = ConfigurationParser::ParseListInteger(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_mpp_line_color]);

    h1->SetMarkerStyle(marker_style[h]);
    h1->SetMarkerColor(marker_color[h]);
    h1->SetMarkerSize(marker_size[h]);
    h1->SetLineStyle(line_style[h]);
    h1->SetLineColor(line_color[h]);
}

void set_axis_style(TH1D* h1, int i, int j, int rows) {
    TAxis* x_axis = h1->GetXaxis();
    TAxis* y_axis = h1->GetYaxis();

    x_axis->SetLabelFont(43);
    x_axis->SetLabelSize(13);
    y_axis->SetLabelFont(43);
    y_axis->SetLabelSize(13);

    x_axis->SetTitleFont(43);
    x_axis->SetTitleSize(13);
    y_axis->SetTitleFont(43);
    y_axis->SetTitleSize(13);

    if (i == rows - 1) {
        x_axis->SetTitleOffset(2.4);
        x_axis->CenterTitle();
    } else {
        x_axis->SetLabelOffset(999);
        x_axis->SetTitle("");
    }

    if (j == 0) {
        y_axis->SetTitleOffset(4);
        y_axis->CenterTitle();
    } else {
        y_axis->SetLabelOffset(999);
        y_axis->SetTitle("");
    }
}

void set_legend_style(TLegend* l1) {
    l1->SetTextFont(43);
    l1->SetTextSize(12);
    l1->SetBorderSize(0);
    l1->SetFillStyle(0);
}

void set_title_style(TPaveText* t1) {
    t1->SetBorderSize(0);
    t1->SetFillColor(0);
    t1->SetFillStyle(0);
    t1->SetTextFont(43);
    t1->SetTextSize(13);
}

void adjust_coordinates(box_t& box, float margin, float edge, int i, int j, int rows, int columns) {
    if (j == 0) {
        box.x1 = box.x1 * (1-margin) + margin;
        box.x2 = box.x2 * (1-margin) + margin;
    } else if (j == columns - 1) {
        box.x1 = box.x1 * (1-edge);
        box.x2 = box.x2 * (1-edge);
    }

    if (i == 0) {
        box.y1 = box.y1 * (1-edge);
        box.y2 = box.y2 * (1-edge);
    } else if (i == rows - 1) {
        box.y1 = box.y1 * (1-margin) + margin;
        box.y2 = box.y2 * (1-margin) + margin;
    }
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        for (int p=1; p<argc; ++p) {
            multiPanelPlotter(argv[p]);
        }
    } else {
        printf("Usage: ./multiPanelPlotter.exe <configFiles>\n");
    }

    return 0;
}
