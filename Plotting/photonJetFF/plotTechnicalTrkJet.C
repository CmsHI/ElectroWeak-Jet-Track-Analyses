#include "TFile.h"
#include "TH1.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TColor.h"
#include "TLine.h"
#include "TMath.h"
#include "TF1.h"

#include <vector>
#include <string>
#include <iostream>

#include "../../Utilities/styleUtil.h"

int plotTechnicalTrkJet(TString inputFile);
void plotTechnicalTrk(TString inputFile);
void plotTechnicalJet(TString inputFile);
void setTH1(TH1 *h);

enum SIGBKG{
    k_raw,
    k_bkg,
    k_sig,
    kN_SIGBKG,
};

int plotTechnicalTrkJet(TString inputFile)
{
    std::cout<<"running plotTechnicalTrkJet()"   <<std::endl;
    plotTechnicalTrk(inputFile);
    plotTechnicalJet(inputFile);
    std::cout<<"running plotTechnicalTrkJet() - END"<<std::endl;

    return 0;
}

void plotTechnicalTrk(TString inputFile)
{
    std::cout<<"running plotTechnicalTrk()"   <<std::endl;

    // graphics objects
    TCanvas* c = 0;
    TLegend* leg = 0;
    TLatex* latex = 0;
    int logX = 0;
    int logY = 0;
    int windowWidth = 800;
    int windowHeight = 800;
    double leftMargin = 0.16;
    double rightMargin = 0.04;
    double bottomMargin = 0.14;
    double topMargin = 0.06;
    double grScale = (double)windowWidth/600;    // scale for graphics objects

    // axis objects
    double xMin = 0.5;
    double xMax = 4.0;
    double yMin = 0;
    double yMax = 20;

    // no horizontal error bars
    gStyle->SetErrorX(0);

    TH1::SetDefaultSumw2();
    TH1D* h[kN_SIGBKG];
    for (int i = 0; i < kN_SIGBKG; ++i) h[i] = 0;
    TH1D* hjetpt = 0;

    TFile* input = TFile::Open(inputFile.Data(), "read");

    int hiBinMin = 0;
    int hiBinMax = 20;

    h[k_raw] = (TH1D*)input->Get(Form("hgammaffxi_pbpbdata_recoreco_%d_%d", hiBinMin, hiBinMax));
    h[k_bkg] = (TH1D*)input->Get(Form("hgammaffxiuemix_pbpbdata_recoreco_%d_%d", hiBinMin, hiBinMax));
    h[k_sig] = (TH1D*)input->Get(Form("hff_sub_pbpbdata_recoreco_%d_%d", hiBinMin, hiBinMax));
    const int nHist = kN_SIGBKG;
    hjetpt = (TH1D*)input->Get(Form("hjetpt_pbpbdata_recoreco_%d_%d", hiBinMin, hiBinMax));
    double intJets = hjetpt->Integral();

    // normalization
    for (int i = 0; i < nHist; ++i) {
        h[i]->Scale(1./intJets);
        h[i]->Scale(1, "width");
    }
    TH1D* hSubCheck = (TH1D*)h[k_raw]->Clone("hSubCheck");
    hSubCheck->Add(h[k_bkg], -1);
    hSubCheck->Add(h[k_sig], -1);
    hSubCheck->Print("all");

    std::string titleX = "#xi^{jet}";
    std::string titleY = "#frac{1}{N^{jet}} #frac{dN^{trk}}{d#xi^{jet}}";

    int markerStyles[nHist] = {kFullSquare, kFullCross, kFullCircle};
    int markerColors[nHist] = {kBlack, kRed, kBlue};
    float markerSize = 2.4;

    std::string legendLabels[nHist] = {"raw tracks", "bkg tracks", "raw - bkg"};

    for (int i = 0; i < nHist; ++i) {
        setTH1(h[i]);

        h[i]->SetAxisRange(xMin, xMax, "X");
        h[i]->SetAxisRange(yMin, yMax, "Y");

        h[i]->SetXTitle(titleX.c_str());
        h[i]->SetYTitle(titleY.c_str());

        h[i]->SetMarkerStyle(markerStyles[i]);
        h[i]->SetMarkerColor(markerColors[i]);
        h[i]->SetMarkerSize(markerSize);

        h[i]->SetLineColor(markerColors[i]);
        h[i]->SetLineWidth(3);
    }

    c = new TCanvas("cnv_plotTechnicalTrk","",windowWidth,windowHeight);
    setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
    setCanvasFinal(c, logX, logY);
    c->cd();

    for (int i = 0; i < nHist; ++i) {

        if (i == 0)  h[i]->Draw("hist e");
        else         h[i]->Draw("hist e same");
    }

    double legY2 = 0.84;
    double legY1 = legY2 - 0.04*nHist;
    leg = new TLegend(0.18, legY1, 0.46, legY2);
    for (int i = 0; i < nHist; ++i) {
        leg->AddEntry(h[i], legendLabels[i].c_str(), "p");
    }
    leg->SetTextSize(0.04);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->Draw();
    c->Update();

    // info text
    std::vector<std::string> textLines;
    int nTextLines = 0;
    latex = new TLatex();
    latex->SetTextFont(43);
    latex->SetTextSize(24*grScale);
    textLines = { "#gamma+jet events",
                  Form("Cent. %d-%d%%", hiBinMin/2, hiBinMax/2)};
    nTextLines = textLines.size();
    for (int i = 0; i < nTextLines; ++i) {
        latex->DrawLatexNDC(0.70, 0.87-0.055*i, textLines[i].c_str());
    }

    latex = new TLatex();
    latex->SetTextFont(43);
    latex->SetTextSize(20*grScale);
    textLines = {    "p_{T}^{trk} > 1 GeV/c",
                     "anti-k_{T} jet R = 0.3",
                     "p_{T}^{jet} > 30 GeV/c",
                     "p_{T}^{#gamma} > 60 GeV/c",
                     "#Delta#phi_{j#gamma} > #frac{7#pi}{8}"};
    nTextLines = textLines.size();
    for (int i = 0; i < nTextLines; ++i) {
        latex->DrawLatexNDC(0.20, legY1-0.08-0.055*i, textLines[i].c_str());
    }

    // CMS Preliminary
    latex = new TLatex();
    latex->SetTextFont(61);
    latex->DrawLatexNDC(0.20, 0.87, "CMS");

    latex = new TLatex();
    latex->SetTextFont(52);
    latex->DrawLatexNDC(0.31, 0.87, "Preliminary");

    // sqrt(s) and data info
    latex = new TLatex();
    latex->SetTextFont(43);
    latex->SetTextSize(24*grScale);
    latex->SetTextAlign(11);
    setTextAbovePad(latex, c, 0.01, 0.02);
    latex->DrawLatexNDC(latex->GetX(), latex->GetY(), "#sqrt{s_{NN}} = 5.02 TeV");

    latex = new TLatex();
    latex->SetTextFont(43);
    latex->SetTextSize(24*grScale);
    latex->SetTextAlign(31);
    setTextAbovePad(latex, c, 0.01, 0.02);
    latex->DrawLatexNDC(latex->GetX(), latex->GetY(), "PbPb Data");

    std::string outName = "plotTechnicalTrk";
    c->SaveAs(Form("%s.png", outName.c_str()));
    c->SaveAs(Form("%s.pdf", outName.c_str()));
    c->Close();

    input->Close();

    std::cout<<"running plotTechnicalTrk() - END"<<std::endl;
}

void plotTechnicalJet(TString inputFile)
{
    std::cout<<"running plotTechnicalJet()"   <<std::endl;

    // graphics objects
    TCanvas* c = 0;
    TLegend* leg = 0;
    TLatex* latex = 0;
    int logX = 0;
    int logY = 0;
    int windowWidth = 800;
    int windowHeight = 800;
    double leftMargin = 0.16;
    double rightMargin = 0.04;
    double bottomMargin = 0.14;
    double topMargin = 0.06;
    double grScale = (double)windowWidth/600;    // scale for graphics objects

    // axis objects
    double xMin = 0.5;
    double xMax = 4.0;
    double yMin = 0;
    double yMax = 8;

    // no horizontal error bars
    gStyle->SetErrorX(0);

    TH1::SetDefaultSumw2();
    TH1D* h[kN_SIGBKG];
    for (int i = 0; i < kN_SIGBKG; ++i) h[i] = 0;
    TH1D* hjetpt = 0;
    TH1D* hjetptmix = 0;

    TFile* input = TFile::Open(inputFile.Data(), "read");

    int hiBinMin = 0;
    int hiBinMax = 20;

    h[k_raw] = (TH1D*)input->Get(Form("hff_sub_pbpbdata_recoreco_%d_%d", hiBinMin, hiBinMax));
    h[k_bkg] = (TH1D*)input->Get(Form("hff_jet_sub_pbpbdata_recoreco_%d_%d", hiBinMin, hiBinMax));
    h[k_sig] = (TH1D*)input->Get(Form("hff_signal_pbpbdata_recoreco_%d_%d", hiBinMin, hiBinMax));
    const int nHist = kN_SIGBKG;
    hjetpt = (TH1D*)input->Get(Form("hjetpt_pbpbdata_recoreco_%d_%d", hiBinMin, hiBinMax));
    hjetptmix = (TH1D*)input->Get(Form("hjetptjetmix_pbpbdata_recoreco_%d_%d", hiBinMin, hiBinMax));
    double intJets = (hjetpt->Integral() - hjetptmix->Integral());

    // normalization
    for (int i = 0; i < nHist; ++i) {

        if (i == k_raw || i == k_bkg) {
            h[i]->Scale(1./intJets);
        }
        h[i]->Scale(1, "width");
    }
    TH1D* hSubCheck = (TH1D*)h[k_raw]->Clone("hSubCheck");
    hSubCheck->Add(h[k_bkg], -1);
    hSubCheck->Add(h[k_sig], -1);
    hSubCheck->Print("all");

    std::string titleX = "#xi^{jet}";
    std::string titleY = "#frac{1}{N^{jet}} #frac{dN^{trk}}{d#xi^{jet}}";

    int markerStyles[nHist] = {kFullSquare, kFullCross, kFullCircle};
    int markerColors[nHist] = {kOrange+2, kViolet, 419};
    float markerSize = 2.4;

    std::string legendLabels[nHist] = {"raw jets", "bkg jets", "raw - bkg"};

    for (int i = 0; i < nHist; ++i) {
        setTH1(h[i]);

        h[i]->SetAxisRange(xMin, xMax, "X");
        h[i]->SetAxisRange(yMin, yMax, "Y");

        h[i]->SetXTitle(titleX.c_str());
        h[i]->SetYTitle(titleY.c_str());

        h[i]->SetMarkerStyle(markerStyles[i]);
        h[i]->SetMarkerColor(markerColors[i]);
        h[i]->SetMarkerSize(markerSize);

        h[i]->SetLineColor(markerColors[i]);
        h[i]->SetLineWidth(3);
    }

    c = new TCanvas("cnv_plotTechnicalJet","",windowWidth,windowHeight);
    setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
    setCanvasFinal(c, logX, logY);
    c->cd();

    for (int i = 0; i < nHist; ++i) {

        if (i == 0)  h[i]->Draw("hist e");
        else         h[i]->Draw("hist e same");
    }

    double legY2 = 0.84;
    double legY1 = legY2 - 0.04*nHist;
    leg = new TLegend(0.18, legY1, 0.46, legY2);
    for (int i = 0; i < nHist; ++i) {
        leg->AddEntry(h[i], legendLabels[i].c_str(), "p");
    }
    leg->SetTextSize(0.04);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->Draw();
    c->Update();

    // info text
    std::vector<std::string> textLines;
    int nTextLines = 0;
    latex = new TLatex();
    latex->SetTextFont(43);
    latex->SetTextSize(24*grScale);
    textLines = { "#gamma+jet events",
                  Form("Cent. %d-%d%%", hiBinMin/2, hiBinMax/2)};
    nTextLines = textLines.size();
    for (int i = 0; i < nTextLines; ++i) {
        latex->DrawLatexNDC(0.70, 0.87-0.055*i, textLines[i].c_str());
    }

    latex = new TLatex();
    latex->SetTextFont(43);
    latex->SetTextSize(20*grScale);
    textLines = {    "p_{T}^{trk} > 1 GeV/c",
                     "anti-k_{T} jet R = 0.3",
                     "p_{T}^{jet} > 30 GeV/c",
                     "p_{T}^{#gamma} > 60 GeV/c",
                     "#Delta#phi_{j#gamma} > #frac{7#pi}{8}"};
    nTextLines = textLines.size();
    for (int i = 0; i < nTextLines; ++i) {
        latex->DrawLatexNDC(0.20, legY1-0.08-0.055*i, textLines[i].c_str());
    }

    // CMS Preliminary
    latex = new TLatex();
    latex->SetTextFont(61);
    latex->DrawLatexNDC(0.20, 0.87, "CMS");

    latex = new TLatex();
    latex->SetTextFont(52);
    latex->DrawLatexNDC(0.31, 0.87, "Preliminary");

    // sqrt(s) and lumi info
    latex = new TLatex();
    latex->SetTextFont(43);
    latex->SetTextSize(24*grScale);
    latex->SetTextAlign(11);
    setTextAbovePad(latex, c, 0.01, 0.02);
    latex->DrawLatexNDC(latex->GetX(), latex->GetY(), "#sqrt{s_{NN}} = 5.02 TeV");

    latex = new TLatex();
    latex->SetTextFont(43);
    latex->SetTextSize(24*grScale);
    latex->SetTextAlign(31);
    setTextAbovePad(latex, c, 0.01, 0.02);
    latex->DrawLatexNDC(latex->GetX(), latex->GetY(), "PbPb Data");

    std::string outName = "plotTechnicalJet";
    c->SaveAs(Form("%s.png", outName.c_str()));
    c->SaveAs(Form("%s.pdf", outName.c_str()));
    c->Close();

    input->Close();

    std::cout<<"running plotTechnicalJet() - END"<<std::endl;
}

int main(int argc, char* argv[]) {
    if (argc == 2)
        return plotTechnicalTrkJet(argv[1]);
    else
        std::cout << "./plotTechnicalTrkJet.exe [input file]" << std::endl;

    return 1;
}

void setTH1(TH1 *h) {

    h->GetXaxis()->CenterTitle();
    h->GetYaxis()->CenterTitle();

    double titleSize = 0.035*1.25*1.2;
    h->SetTitleSize(titleSize, "X");
    h->SetTitleSize(titleSize, "Y");

    h->SetTitleOffset(1.25, "X");
    h->SetTitleOffset(1.2,"Y");

    h->SetTitle("");
    h->SetStats(false);
}
