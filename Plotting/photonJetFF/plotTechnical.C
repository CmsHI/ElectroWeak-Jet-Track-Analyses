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

int plotTechnical(TString inputFile);
void setTH1(TH1 *h);

enum TRACK_SIGBKG{
    k_rawTrk,
    k_ueTrk,
    k_sigTrk,
    kN_TRK_SIGBKG,
};

int plotTechnical(TString inputFile)
{
    std::cout<<"running plotTechnical()"   <<std::endl;

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
    double yMax = 18;

    // no horizontal error bars
    gStyle->SetErrorX(0);

    TH1::SetDefaultSumw2();
    TH1D* h[kN_TRK_SIGBKG];
    TH1D* hjetpt;
    for (int i = 0; i < kN_TRK_SIGBKG; ++i) h[i] = 0;

    TFile* input = TFile::Open(inputFile.Data(), "read");

    int hiBinMin = 0;
    int hiBinMax = 20;

    h[k_rawTrk] = (TH1D*)input->Get(Form("hgammaffxi_pbpbdata_recoreco_%d_%d", hiBinMin, hiBinMax));
    h[k_ueTrk] = (TH1D*)input->Get(Form("hgammaffxiuemix_pbpbdata_recoreco_%d_%d", hiBinMin, hiBinMax));
    h[k_sigTrk] = (TH1D*)input->Get(Form("hff_sub_pbpbdata_recoreco_%d_%d", hiBinMin, hiBinMax));
    const int nHist = kN_TRK_SIGBKG;
    hjetpt = (TH1D*)input->Get(Form("hjetpt_pbpbdata_recoreco_%d_%d", hiBinMin, hiBinMax));
    double intJets = hjetpt->Integral();

    // normalization
    for (int i = 0; i < nHist; ++i) {
        h[i]->Scale(1./intJets);
        h[i]->Scale(1, "width");
    }

    std::string titleX = "#xi^{jet}";
    std::string titleY = "#frac{1}{N^{jet}} #frac{dN^{trk}}{d#xi^{jet}}";

    int markerStyles[nHist] = {kFullSquare, kFullCross, kFullCircle};
    int markerColors[nHist] = {kBlack, kRed, kBlue};
    float markerSize = 2.2;

    std::string legendLabels[nHist] = {"Raw tracks", "UE tracks", "UE subtracted tracks"};

    for (int i = 0; i < nHist; ++i) {
        setTH1(h[i]);

        h[i]->SetAxisRange(xMin, xMax, "X");
        h[i]->SetAxisRange(yMin, yMax, "Y");

        h[i]->SetXTitle(titleX.c_str());
        h[i]->SetYTitle(titleY.c_str());

        h[i]->SetMarkerStyle(markerStyles[i]);
        h[i]->SetMarkerColor(markerColors[i]);
        h[i]->SetMarkerSize(markerSize);
    }

    c = new TCanvas("cnv_plotTechnical","",windowWidth,windowHeight);
    setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
    setCanvasFinal(c, logX, logY);
    c->cd();

    for (int i = 0; i < nHist; ++i) {

        if (i == 0)  h[i]->Draw("e");
        else         h[i]->Draw("e same");
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

    // text for kinematics
    latex = new TLatex();
    latex->SetTextFont(43);
    latex->SetTextSize(24*grScale);
    std::vector<std::string> textLines = {
                            "#gamma+jet events",
                            Form("Cent:%d-%d%%", hiBinMin/2, hiBinMax/2),
                            "p_{T}^{trk} > 1 GeV/c",
                            "anti-k_{T} jet R = 0.3",
                            "p_{T}^{jet} > 30 GeV/c",
                            "p_{T}^{#gamma} > 60 GeV/c",
                             "#Delta#phi_{j#gamma} > #frac{7#pi}{8}"};
    int nTextLines = textLines.size();
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
    latex->DrawLatexNDC(latex->GetX(), latex->GetY(), "PbPb 404 #mub^{-1}");

    std::string outName = "plotTechnical";
    c->SaveAs(Form("%s.png", outName.c_str()));
    c->SaveAs(Form("%s.pdf", outName.c_str()));

    std::cout<<"running plotTechnical() - END"<<std::endl;
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 2)
        return plotTechnical(argv[1]);
    else
        std::cout << "./plotPhotonJetFF [input file]" << std::endl;

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
