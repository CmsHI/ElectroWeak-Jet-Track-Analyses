/*
 * macro to make technical plots for sumIso distribution
 * Usage : Plotting/photonJetFF/plotTechnicalSumIso.exe Data/photonJetFF/photonSpectra_Pythia8_AllQCDPhoton_Hydjet_MB.root Data/photonJetFF/photonSpectra_Pythia8_EmEnrDijet_Hydjet_MB.roo
 */
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

int plotTechnicalSumIso(TString inputFileSig, TString inputFileBkg);
void setTH1(TH1 *h);

enum SIGBKG{
    k_sig,
    k_bkg,
    kN_SIGBKG,
};

int plotTechnicalSumIso(TString inputFileSig, TString inputFileBkg)
{
    std::cout<<"running plotTechnicalSumIso()"   <<std::endl;

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
    double xMin = 0;
    double xMax = -1;
    double yMin = 0;
    double yMax = 0.14;

    // no horizontal error bars
    gStyle->SetErrorX(0);
    gStyle->SetHatchesLineWidth(2);
    gStyle->SetHatchesSpacing(0.6);

    TH1::SetDefaultSumw2();
    TH1D* h[kN_SIGBKG];
    for (int i = 0; i < kN_SIGBKG; ++i) h[i] = 0;

    TFile* inputSig = TFile::Open(inputFileSig.Data(), "read");
    TFile* inputBkg = TFile::Open(inputFileBkg.Data(), "read");

    h[k_sig] = (TH1D*)inputSig->Get("h_sumIso_hovere_etaBin0_ptBin0_hiBin0");
    h[k_bkg] = (TH1D*)inputBkg->Get("h_sumIso_hovere_etaBin0_ptBin0_hiBin0");
    const int nHist = kN_SIGBKG;

    // normalization
    for (int i = 0; i < nHist; ++i) {
        h[i]->Scale(1./h[i]->Integral());
    }

    std::string titleX = "#Sigma iso (GeV/c)";
    std::string titleY = "Unity Normalization";

    int fillStyles[nHist] = {3445, 3454};
    int fillColors[nHist] = {kRed, kGreen};
    float markerSize = 2.2;

    std::string legendLabels[nHist] = {"signal", "background"};

    for (int i = 0; i < nHist; ++i) {
        setTH1(h[i]);

        h[i]->SetAxisRange(xMin, xMax, "X");
        h[i]->SetAxisRange(yMin, yMax, "Y");

        h[i]->SetXTitle(titleX.c_str());
        h[i]->SetYTitle(titleY.c_str());

        h[i]->SetLineWidth(3);
        h[i]->SetFillStyle(fillStyles[i]);
        h[i]->SetFillColor(fillColors[i]);
        h[i]->SetLineColor(fillColors[i]);
        h[i]->SetMarkerSize(markerSize);
    }

    c = new TCanvas("cnv","",windowWidth,windowHeight);
    setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
    setCanvasFinal(c, logX, logY);
    c->cd();

    for (int i = 0; i < nHist; ++i) {

        if (i == 0)  h[i]->Draw("hist");
        else         h[i]->Draw("hist same");
    }
    c->Update();

    double legY2 = 0.90;
    double legY1 = legY2 - 0.05 - 0.04*nHist;
    leg = new TLegend(0.66, legY1, 0.92, legY2);
    leg->SetHeader("Pythia+Hydjet");
    for (int i = 0; i < nHist; ++i) {
        leg->AddEntry(h[i], legendLabels[i].c_str(), "lf");
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
    textLines = {"Cent. 0-100%"};
    nTextLines = textLines.size();
    for (int i = 0; i < nTextLines; ++i) {
        latex->DrawLatexNDC(leg->GetX1NDC()+0.01, legY1-0.04-0.055*i, textLines[i].c_str());
    }

    latex = new TLatex();
    latex->SetTextFont(43);
    latex->SetTextSize(20*grScale);
    textLines = {    "p_{T}^{#gamma} > 60 GeV/c",
                     "|#eta^{#gamma}| < 1.44"};
    nTextLines = textLines.size();
    for (int i = 0; i < nTextLines; ++i) {
        latex->DrawLatexNDC(0.20, legY1-0.04-0.055*i, textLines[i].c_str());
    }

    // CMS Simulation
    latex = new TLatex();
    latex->SetTextFont(61);
    latex->DrawLatexNDC(0.20, 0.87, "CMS");

    latex = new TLatex();
    latex->SetTextFont(52);
    latex->DrawLatexNDC(0.31, 0.87, "Simulation");

    // sqrt(s) and mc info
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
    latex->DrawLatexNDC(latex->GetX(), latex->GetY(), "PbPb MC");

    std::string outName = "plotTechnicalSumIso";
    c->SaveAs(Form("%s.png", outName.c_str()));
    c->SaveAs(Form("%s.pdf", outName.c_str()));

    std::cout<<"running plotTechnicalSumIso() - END"<<std::endl;
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 3)
        return plotTechnicalSumIso(argv[1], argv[2]);
    else {
        std::cout << "./plotTechnicalSumIso.exe [input file Signal] [input file Bkg]" << std::endl;
    }

    return 1;
}

void setTH1(TH1 *h) {

    h->GetXaxis()->CenterTitle();
    h->GetYaxis()->CenterTitle();

    double titleSize = 0.035*1.25*1.2;
    h->SetTitleSize(titleSize, "X");
    h->SetTitleSize(titleSize, "Y");

    h->SetTitleOffset(1.05, "X");
    h->SetTitleOffset(1.2,"Y");

    h->SetTitle("");
    h->SetStats(false);
}
