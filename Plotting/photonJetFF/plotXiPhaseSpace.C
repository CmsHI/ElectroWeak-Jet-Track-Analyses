/*
 * plots about xi distribution phase space
 */
#include "TCanvas.h"
#include "TLine.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TAxis.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TF1.h"
#include "TMath.h"
#include "TMathText.h"
#include "TStyle.h"

#include <vector>
#include <string>
#include <iostream>

void plotXiPhaseSpace();
double getXNDC(TPad* pad, double x);
double getYNDC(TPad* pad, double y);
void copyAxisTitle(TAxis* axis1, TAxis* axis2);

void jetpt_vs_xi();
void jetpt_vs_xi(int trkPt, double deltaR);
void jetpt_vs_xi_plusminus(int trkPt, double deta);
void trackpt_vs_xi();
void trackpt_vs_xi_plusminus(int jetPt, double deta);

void phopt_vs_xi();
void phopt_vs_xi(int trkPt);
void trackpt_vs_xigamma();
void trackpt_vs_xigamma(int trkPt);

const std::string xijet = "#xi^{jet}";
const std::string xigamma = "#xi_{T}^{#gamma}";
const std::string ptjet = "p_{T}^{jet} (GeV/c)";
const std::string pttrk = "p_{T}^{trk} (GeV/c)";
const std::string ptg = "p_{T}^{#gamma} (GeV/c)";

const std::string ptrkvec = "#bf{p}^{trk}";
const std::string pjetvec = "#bf{p}^{jet}";
const std::string eqnXijet = "#xi^{jet} = ln #frac{#left|#bf{p}^{jet}#right|^{2}}{#bf{p}^{trk} #bf{p}^{jet}}";
const std::string eqnXigamma = "#xi_{T}^{#gamma} = ln #frac{-#left|#bf{p}_{T}^{#gamma}#right|^{2}}{#bf{p}_{T}^{trk} #bf{p}_{T}^{#gamma}}";

/*
const std::string ptrkvec = "\\mathbf{p}^\\mathrm{trk}";
const std::string pjetvec = "\\mathbf{p}^\\mathrm{jet}";
const std::string eqnXijet = "\\mathrm{\\xi}^\\mathrm{jet} = \\ln \\frac{|\\mathbf{p}^\\mathrm{jet}|^{2}}{\\mathbf{p}^\\mathrm{trk} \\cdot \\mathbf{p}^\\mathrm{jet}}";
*/

void plotXiPhaseSpace()
{
    std::cout<<"running plotXiPhaseSpace()"   <<std::endl;
    jetpt_vs_xi();
    trackpt_vs_xi();
    jetpt_vs_xi(1, 0.3);
    jetpt_vs_xi_plusminus(1, 0.3);
    trackpt_vs_xi_plusminus(30, 0.3);
    //
    phopt_vs_xi();
    trackpt_vs_xigamma();
    phopt_vs_xi(1);
    trackpt_vs_xigamma(60);

    std::cout<<"running plotXiPhaseSpace() - END"<<std::endl;
}

void jetpt_vs_xi()
{
    std::cout<<"running jetpt_vs_xi()"   <<std::endl;

    // graphics objects
    TCanvas* c = 0;
    //TLine* line = 0;
    TLegend* leg = 0;
    TLatex* latex = 0;
    //TMathText* mathText = 0;
    int logX = 0;
    int logY = 0;
    int windowSize = 800;
    double grScale = (double)windowSize/600;    // scale for graphics objects

    // axis objects
    double xMin = 0.5;
    double xMax = 4.5;
    double yMin = 0;
    double yMax = 120;

    std::string titleX = xijet.c_str();
    std::string titleY = ptjet.c_str();

    std::string TF1_formula = "[0]*exp(x)";

    double deltaR = 0;
    std::vector<double> minTrkPts = {1, 2, 5};
    std::vector<TF1*> f1s;
    std::vector<int> f1_colors = {kRed, kBlue, kGreen+3};

    TF1* f1 = 0;
    int n = minTrkPts.size();
    for (int i = 0; i < n; ++i) {
        f1 = new TF1(Form("f1_%d", i), TF1_formula.c_str(), xMin, xMax);

        f1->SetParameter(0, minTrkPts[i]);

        // plotting settings
        f1->SetLineColor(f1_colors[i]);
        f1->SetLineWidth(f1->GetLineWidth()*2);

        f1->GetXaxis()->SetTitle(titleX.c_str());
        f1->GetYaxis()->SetTitle(titleY.c_str());
        f1->SetTitle("");

        f1->GetXaxis()->CenterTitle();
        f1->GetYaxis()->CenterTitle();

        f1->GetXaxis()->SetTitleSize(f1->GetXaxis()->GetTitleSize()*grScale);
        f1->GetYaxis()->SetTitleSize(f1->GetYaxis()->GetTitleSize()*grScale);

        f1->GetXaxis()->SetTitleOffset(1.75/grScale);
        f1->GetYaxis()->SetTitleOffset(1.75/grScale);

        f1s.push_back(f1);
    }

    c = new TCanvas("", "", windowSize, windowSize);
    c->SetTickx(1);
    c->SetTicky(1);
    c->SetMargin(0.15, 0.05, 0.15, 0.05);
    c->SetGrid();
    c->SetLogx(logX);
    c->SetLogy(logY);
    TH1F* hFrame = c->DrawFrame(xMin, yMin, xMax, yMax);
    copyAxisTitle(hFrame->GetXaxis(), f1s[0]->GetXaxis());
    copyAxisTitle(hFrame->GetYaxis(), f1s[0]->GetYaxis());

    for (int i = 0; i < n; ++i) {
        f1s[i]->Draw("same");
    }
    c->Update();

    double legY2 = 0.90;
    double legY1 = legY2 - 0.07*n;
    leg = new TLegend(0.20, legY1, 0.46, legY2);
    for (int i = 0; i < n; ++i) {
        leg->AddEntry(f1s[i], Form("p_{T}^{trk} = %d GeV/c", (int)minTrkPts[i]), "l");
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
    if (deltaR != 0) latex->DrawLatexNDC(leg->GetX1NDC()+0.01, legY1-0.05, Form("#DeltaR (jet, trk) = %.1f", deltaR));
    else             latex->DrawLatexNDC(leg->GetX1NDC()+0.01, legY1-0.05, Form("#DeltaR (jet, trk) = 0"));

    // text for xi equation
    latex = new TLatex();
    latex->SetTextFont(43);
    latex->SetTextSize(24*grScale);
    latex->DrawLatexNDC(leg->GetX1NDC(), 0.46, eqnXijet.c_str());

    c->Update();
    /*
    double cnvYMin = c->GetUymin();
    double cnvYMax = c->GetUymax();
    if (logY == 1) {
        cnvYMin = TMath::Power(TMath::Power(10, logY), c->GetUymin());
        cnvYMax = TMath::Power(TMath::Power(10, logY), c->GetUymax());
    }
    */

    /*
    line = new TLine(xMin, 30, xMax, 30);
    line->SetLineStyle(kDashed);
    line->SetLineWidth(line->GetLineWidth()*3);
    line->Draw();
    */

    std::string outName = "jetpt_vs_xi";
    c->SaveAs(Form("%s.png", outName.c_str()));
    c->SaveAs(Form("%s.pdf", outName.c_str()));

    std::cout<<"running jetpt_vs_xi() - END"<<std::endl;
}

void jetpt_vs_xi(int trkPt, double deltaR)
{
    std::cout<<"running jetpt_vs_xi()"   <<std::endl;

    // graphics objects
    TCanvas* c = 0;
    //TLine* line = 0;
    TLegend* leg = 0;
    TLatex* latex = 0;
    int logX = 0;
    int logY = 0;
    int windowSize = 800;
    double grScale = (double)windowSize/600;    // scale for graphics objects

    // axis objects
    double xMin = 0.5;
    double xMax = 4.5;
    double yMin = 0;
    double yMax = 120;

    std::string titleX = xijet.c_str();
    std::string titleY = ptjet.c_str();

    std::string TF1_formula = "[0]*(cosh([1]+[2])/cosh([1]))*cos([2])*exp(x)";

    std::vector<double> jetEtas = {0, 0.5, 1, 1.6};
    std::vector<TF1*> f1s;
    std::vector<int> f1_colors = {kRed, kBlue, kGreen+3, kOrange+1};

    TF1* f1 = 0;
    int n = jetEtas.size();
    for (int i = 0; i < n; ++i) {
        f1 = new TF1(Form("f1_%d", i), TF1_formula.c_str(), xMin, xMax);

        f1->SetParameter(0, trkPt);
        f1->SetParameter(1, jetEtas[i]);
        f1->SetParameter(2, deltaR);

        // plotting settings
        f1->SetLineColor(f1_colors[i]);
        f1->SetLineWidth(f1->GetLineWidth()*2);

        f1->GetXaxis()->SetTitle(titleX.c_str());
        f1->GetYaxis()->SetTitle(titleY.c_str());
        f1->SetTitle("");

        f1->GetXaxis()->CenterTitle();
        f1->GetYaxis()->CenterTitle();

        f1->GetXaxis()->SetTitleSize(f1->GetXaxis()->GetTitleSize()*grScale);
        f1->GetYaxis()->SetTitleSize(f1->GetYaxis()->GetTitleSize()*grScale);

        f1->GetXaxis()->SetTitleOffset(1.75/grScale);
        f1->GetYaxis()->SetTitleOffset(1.75/grScale);

        f1s.push_back(f1);
    }

    c = new TCanvas("", "", windowSize, windowSize);
    c->SetTickx(1);
    c->SetTicky(1);
    c->SetWindowSize(windowSize, windowSize);
    c->SetMargin(0.15, 0.05, 0.15, 0.05);
    c->SetGrid();
    c->SetLogx(logX);
    c->SetLogy(logY);
    TH1F* hFrame = c->DrawFrame(xMin, yMin, xMax, yMax);
    copyAxisTitle(hFrame->GetXaxis(), f1s[0]->GetXaxis());
    copyAxisTitle(hFrame->GetYaxis(), f1s[0]->GetYaxis());

    for (int i = 0; i < n; ++i) {
        f1s[i]->Draw("same");
    }
    c->Update();

    double legY2 = 0.90;
    double legY1 = legY2 - 0.07*n;

    leg = new TLegend(0.20, legY1, 0.46, legY2);
    for (int i = 0; i < n; ++i) {
        if (jetEtas[i] == 0) leg->AddEntry(f1s[i], Form("|#eta^{jet}| = 0"), "l");
        else                 leg->AddEntry(f1s[i], Form("|#eta^{jet}| = %.1f", jetEtas[i]), "l");
    }
    leg->SetTextSize(0.04);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->DrawClone();
    c->Update();

    // text for kinematics
    latex = new TLatex();
    latex->SetTextFont(43);
    latex->SetTextSize(24*grScale);
    std::vector<std::string> textLines = {Form("p_{T}^{trk} = %d GeV/c", trkPt),
                                          Form("#DeltaR (jet, trk) = %.1f", deltaR)};
    int nTextLines = textLines.size();
    for (int i = 0; i < nTextLines; ++i) {
        latex->DrawLatexNDC(0.20, legY1-0.05-0.06*i, textLines.at(i).c_str());
    }

    // text for xi equation
    latex = new TLatex();
    latex->SetTextFont(43);
    latex->SetTextSize(24*grScale);
    latex->DrawLatexNDC(0.50, 0.86, eqnXijet.c_str());

    c->Update();
    /*
    double cnvYMin = c->GetUymin();
    double cnvYMax = c->GetUymax();
    if (logY == 1) {
        cnvYMin = TMath::Power(TMath::Power(10, logY), c->GetUymin());
        cnvYMax = TMath::Power(TMath::Power(10, logY), c->GetUymax());
    }
    */

    /*
    line = new TLine(xMin, 30, xMax, 30);
    line->SetLineStyle(kDashed);
    line->SetLineWidth(line->GetLineWidth()*3);
    line->Draw();
    */

    std::string outName = Form("jetpt_vs_xi_trkPt%d_dR0p%d", trkPt, int(deltaR*10));
    c->SaveAs(Form("%s.png", outName.c_str()));
    c->SaveAs(Form("%s.pdf", outName.c_str()));

    std::cout<<"running jetpt_vs_xi() - END"<<std::endl;
}

void jetpt_vs_xi_plusminus(int trkPt, double deta)
{
    std::cout<<"running jetpt_vs_xi_plusminus()"   <<std::endl;

    gStyle->SetLineStyleString(11,"15 4 4 4");
    gStyle->SetLineStyleString(12,"5 5");
    gStyle->SetLineStyleString(13,"16 4");

    gStyle->SetLineStyleString(14,"20 8");

    // graphics objects
    TCanvas* c = 0;
    //TLine* line = 0;
    TLegend* leg = 0;
    TLatex* latex = 0;
    int logX = 0;
    int logY = 0;
    int windowSize = 800;
    double grScale = (double)windowSize/600;    // scale for graphics objects

    // axis objects
    double xMin = 0.5;
    double xMax = 4.5;
    double yMin = 0;
    double yMax = 120;

    std::string titleX = xijet.c_str();
    std::string titleY = ptjet.c_str();

    std::string thetaJet_formula = "2*atan(exp(-[1]))";
    std::string thetaTrk_formula = "2*atan(exp(-([1]+[2]*[3])))";
    std::string TF1_formula = Form("[0]*(cosh([1]+[2]*[3])/cosh([1]))*cos(%s - %s)*exp(x)", thetaJet_formula.c_str()
                                                                                          , thetaTrk_formula.c_str());

    const int nPM = 2;
    const int detaPM[nPM] = { // detaAbs = |eta trk| - |eta jet|
        +1,  // detaAbs > 0
        -1, // detaAbs < 0
    };
    const std::string legendHeader = "#left|#eta^{trk}#right| - #left|#eta^{jet}#right| =";
    const std::string legendHeaders[nPM] = {Form("%.1f", deta), Form("-%.1f", deta)};

    std::vector<double> jetEtas = {0.1, 0.5, 1, 1.6};
    std::vector<TF1*> f1s[nPM];
    std::vector<int> f1_colors = {kRed, kBlue, kGreen+3, kOrange+1};
    int f1_lineStyles[nPM] = {kSolid, 14};

    TF1* f1 = 0;
    int n = jetEtas.size();
    for (int i = 0; i < nPM; ++i) {
        for (int j = 0; j < n; ++j) {
            f1 = new TF1(Form("f1_%d_%d", i, j), TF1_formula.c_str(), xMin, xMax);

            f1->SetParameter(0, trkPt);
            f1->SetParameter(1, jetEtas[j]);
            f1->SetParameter(2, deta);
            f1->SetParameter(3, detaPM[i]);

            // plotting settings
            f1->SetLineColor(f1_colors[j]);
            f1->SetLineWidth(f1->GetLineWidth()*2);
            f1->SetLineStyle(f1_lineStyles[i]);

            f1->GetXaxis()->SetTitle(titleX.c_str());
            f1->GetYaxis()->SetTitle(titleY.c_str());
            f1->SetTitle("");

            f1->GetXaxis()->CenterTitle();
            f1->GetYaxis()->CenterTitle();

            f1->GetXaxis()->SetTitleSize(f1->GetXaxis()->GetTitleSize()*grScale);
            f1->GetYaxis()->SetTitleSize(f1->GetYaxis()->GetTitleSize()*grScale);

            f1->GetXaxis()->SetTitleOffset(1.75/grScale);
            f1->GetYaxis()->SetTitleOffset(1.75/grScale);

            f1s[i].push_back(f1);
        }
    }

    c = new TCanvas("", "", windowSize, windowSize);
    c->SetTickx(1);
    c->SetTicky(1);
    c->SetWindowSize(windowSize, windowSize);
    c->SetMargin(0.15, 0.05, 0.15, 0.05);
    c->SetGrid();
    c->SetLogx(logX);
    c->SetLogy(logY);
    TH1F* hFrame = c->DrawFrame(xMin, yMin, xMax, yMax);
    copyAxisTitle(hFrame->GetXaxis(), f1s[0][0]->GetXaxis());
    copyAxisTitle(hFrame->GetYaxis(), f1s[0][0]->GetYaxis());

    for (int i = 0; i < nPM; ++i) {
        for (int j = 0; j < n; ++j) {
            f1s[i][j]->Draw("same");
        }
    }
    c->Update();


    double legY2 = 0.92;
    double legY1 = legY2 - 0.06*n;
    double legX1 = 0.42;
    // text above the legend
    latex = new TLatex();
    latex->SetTextFont(43);
    latex->SetTextSize(24*grScale);
    latex->DrawLatexNDC(legX1-0.23, legY2-0.03, legendHeader.c_str());
    double leg1Width = 0.26;
    double leg2Offset = 0.08;
    for (int i = 0; i < nPM; ++i) {
        leg = new TLegend(legX1 + i*leg2Offset, legY1, legX1 + leg1Width + i*leg2Offset, legY2);
        leg->SetHeader(legendHeaders[i].c_str());
        for (int j = 0; j < n; ++j) {

            std::string legendText = "";
            if (i == 1) {
                if (jetEtas[j] == 0) legendText = Form("|#eta^{jet}| = 0");
                else                 legendText = Form("|#eta^{jet}| = %.1f", jetEtas[j]);
            }

            leg->AddEntry(f1s[i][j], legendText.c_str(), "l");
        }
        leg->SetTextSize(0.04);
        leg->SetFillStyle(0);
        leg->SetBorderSize(0);
        leg->DrawClone();
        c->Update();
    }

    // text for kinematics
    latex = new TLatex();
    latex->SetTextFont(43);
    latex->SetTextSize(24*grScale);
    std::vector<std::string> textLines = {Form("p_{T}^{trk} = %d GeV/c", trkPt),
                                          Form("#Delta#phi (jet, trk) = 0")};
    int nTextLines = textLines.size();
    for (int i = 0; i < nTextLines; ++i) {
        latex->DrawLatexNDC(0.20, legY1-0.05-0.06*i, textLines.at(i).c_str());
    }

    // text for xi equation
    latex = new TLatex();
    latex->SetTextFont(43);
    latex->SetTextSize(24*grScale);
    latex->DrawLatexNDC(0.20, 0.46, eqnXijet.c_str());

    c->Update();
    /*
    double cnvYMin = c->GetUymin();
    double cnvYMax = c->GetUymax();
    if (logY == 1) {
        cnvYMin = TMath::Power(TMath::Power(10, logY), c->GetUymin());
        cnvYMax = TMath::Power(TMath::Power(10, logY), c->GetUymax());
    }
    */

    /*
    line = new TLine(xMin, 30, xMax, 30);
    line->SetLineStyle(kDashed);
    line->SetLineWidth(line->GetLineWidth()*3);
    line->Draw();
    */

    std::string outName = Form("jetpt_vs_xi_plusminus_trkPt%d_deta0p%d", trkPt, int(deta*10));
    c->SaveAs(Form("%s.png", outName.c_str()));
    c->SaveAs(Form("%s.pdf", outName.c_str()));

    std::cout<<"running jetpt_vs_xi_plusminus() - END"<<std::endl;
}

void trackpt_vs_xi()
{
    std::cout<<"running trackpt_vs_xi()"   <<std::endl;

    // graphics objects
    TCanvas* c = 0;
    //TLine* line = 0;
    TLegend* leg = 0;
    TLatex* latex = 0;
    int logX = 0;
    int logY = 1;
    int windowSize = 800;
    double grScale = (double)windowSize/600;    // scale for graphics objects

    // axis objects
    double xMin = 0.5;
    double xMax = 4.5;
    double yMin = 0.8;
    double yMax = 55;

    std::string titleX = xijet.c_str();
    std::string titleY = pttrk.c_str();

    std::string TF1_formula = "[0]/exp(x)";

    double deltaR = 0;
    std::vector<double> minJetPts = {30, 60, 90};
    std::vector<TF1*> f1s;
    std::vector<int> f1_colors = {kViolet, kOrange+1, kRed+1};

    TF1* f1 = 0;
    int n = minJetPts.size();
    for (int i = 0; i < n; ++i) {
        f1 = new TF1(Form("f1_%d", i), TF1_formula.c_str(), xMin, xMax);

        f1->SetParameter(0, minJetPts[i]);

        // plotting settings
        f1->SetLineColor(f1_colors[i]);
        f1->SetLineWidth(f1->GetLineWidth()*2);

        f1->GetXaxis()->SetTitle(titleX.c_str());
        f1->GetYaxis()->SetTitle(titleY.c_str());
        f1->SetTitle("");

        f1->GetXaxis()->CenterTitle();
        f1->GetYaxis()->CenterTitle();

        f1->GetXaxis()->SetTitleSize(f1->GetXaxis()->GetTitleSize()*grScale);
        f1->GetYaxis()->SetTitleSize(f1->GetYaxis()->GetTitleSize()*grScale);

        f1->GetXaxis()->SetTitleOffset(1.75/grScale);
        f1->GetYaxis()->SetTitleOffset(1.75/grScale);

        f1s.push_back(f1);
    }

    c = new TCanvas("", "", windowSize, windowSize);
    c->SetTickx(1);
    c->SetTicky(1);
    c->SetWindowSize(windowSize, windowSize);
    c->SetMargin(0.15, 0.05, 0.15, 0.05);
    c->SetGrid();
    c->SetLogx(logX);
    c->SetLogy(logY);
    TH1F* hFrame = c->DrawFrame(xMin, yMin, xMax, yMax);
    copyAxisTitle(hFrame->GetXaxis(), f1s[0]->GetXaxis());
    copyAxisTitle(hFrame->GetYaxis(), f1s[0]->GetYaxis());

    for (int i = 0; i < n; ++i) {
        f1s[i]->Draw("same");
    }
    c->Update();

    double legY2 = 0.90;
    double legY1 = legY2 - 0.07*n;
    double legX1 = 0.60;
    leg = new TLegend(legX1, legY1, 0.90, legY2);
    for (int i = 0; i < n; ++i) {
        leg->AddEntry(f1s[i], Form("p_{T}^{jet} = %d GeV/c", (int)minJetPts[i]), "l");
    }
    leg->SetTextSize(0.04);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->DrawClone();
    c->Update();

    // text for kinematics
    latex = new TLatex();
    latex->SetTextFont(43);
    latex->SetTextSize(24*grScale);
    if (deltaR != 0) latex->DrawLatexNDC(legX1, legY1-0.05, Form("#DeltaR (jet, trk) = %.1f", deltaR));
    else             latex->DrawLatexNDC(legX1, legY1-0.05, Form("#DeltaR (jet, trk) = 0"));

    // text for xi equation
    latex = new TLatex();
    latex->SetTextFont(43);
    latex->SetTextSize(24*grScale);
    latex->DrawLatexNDC(0.18, 0.25, eqnXijet.c_str());

    c->Update();
    /*
    double cnvYMin = c->GetUymin();
    double cnvYMax = c->GetUymax();
    if (logY == 1) {
        cnvYMin = TMath::Power(TMath::Power(10, logY), c->GetUymin());
        cnvYMax = TMath::Power(TMath::Power(10, logY), c->GetUymax());
    }
    */

    /*
    line = new TLine(xMin, 1, xMax, 1);
    line->SetLineStyle(kDashed);
    line->SetLineWidth(line->GetLineWidth()*3);
    line->Draw();
    */

    std::string outName = "trackpt_vs_xi";
    c->SaveAs(Form("%s.png", outName.c_str()));
    c->SaveAs(Form("%s.pdf", outName.c_str()));

    std::cout<<"running trackpt_vs_xi() - END"<<std::endl;
}

void trackpt_vs_xi_plusminus(int jetPt, double deta)
{
    std::cout<<"running trackpt_vs_xi_plusminus()"   <<std::endl;

    gStyle->SetLineStyleString(11,"15 4 4 4");
    gStyle->SetLineStyleString(12,"5 5");
    gStyle->SetLineStyleString(13,"16 4");

    gStyle->SetLineStyleString(14,"20 8");

    // graphics objects
    TCanvas* c = 0;
    //TLine* line = 0;
    TLegend* leg = 0;
    TLatex* latex = 0;
    int logX = 0;
    int logY = 1;
    int windowSize = 800;
    double grScale = (double)windowSize/600;    // scale for graphics objects

    // axis objects
    double xMin = 0.5;
    double xMax = 4.5;
    double yMin = 0.8;
    double yMax = 55;

    std::string titleX = xijet.c_str();
    std::string titleY = pttrk.c_str();

    std::string thetaJet_formula = "2*atan(exp(-[1]))";
    std::string thetaTrk_formula = "2*atan(exp(-([1]+[2]*[3])))";
    std::string TF1_formula = Form("[0]*(cosh([1])/cosh([1]+[2]*[3]))/cos(%s - %s)/exp(x)", thetaJet_formula.c_str()
                                                                                          , thetaTrk_formula.c_str());

    const int nPM = 2;
    const int detaPM[nPM] = { // detaAbs = |eta trk| - |eta jet|
        +1,  // detaAbs > 0
        -1, // detaAbs < 0
    };
    const std::string legendHeader = "#left|#eta^{trk}#right| - #left|#eta^{jet}#right| =";
    const std::string legendHeaders[nPM] = {Form("%.1f", deta), Form("-%.1f", deta)};

    std::vector<double> jetEtas = {0.1, 0.5, 1, 1.6};
    std::vector<TF1*> f1s[nPM];
    std::vector<int> f1_colors = {kRed, kBlue, kGreen+3, kOrange+1};
    int f1_lineStyles[nPM] = {kSolid, 14};

    TF1* f1 = 0;
    int n = jetEtas.size();
    for (int i = 0; i < nPM; ++i) {
        for (int j = 0; j < n; ++j) {
            f1 = new TF1(Form("f1_%d_%d", i, j), TF1_formula.c_str(), xMin, xMax);

            f1->SetParameter(0, jetPt);
            f1->SetParameter(1, jetEtas[j]);
            f1->SetParameter(2, deta);
            f1->SetParameter(3, detaPM[i]);

            // plotting settings
            f1->SetLineColor(f1_colors[j]);
            f1->SetLineWidth(f1->GetLineWidth()*2);
            f1->SetLineStyle(f1_lineStyles[i]);

            f1->GetXaxis()->SetTitle(titleX.c_str());
            f1->GetYaxis()->SetTitle(titleY.c_str());
            f1->SetTitle("");

            f1->GetXaxis()->CenterTitle();
            f1->GetYaxis()->CenterTitle();

            f1->GetXaxis()->SetTitleSize(f1->GetXaxis()->GetTitleSize()*grScale);
            f1->GetYaxis()->SetTitleSize(f1->GetYaxis()->GetTitleSize()*grScale);

            f1->GetXaxis()->SetTitleOffset(1.75/grScale);
            f1->GetYaxis()->SetTitleOffset(1.75/grScale);

            f1s[i].push_back(f1);
        }
    }

    c = new TCanvas("", "", windowSize, windowSize);
    c->SetTickx(1);
    c->SetTicky(1);
    c->SetWindowSize(windowSize, windowSize);
    c->SetMargin(0.15, 0.05, 0.15, 0.05);
    c->SetGrid();
    c->SetLogx(logX);
    c->SetLogy(logY);
    TH1F* hFrame = c->DrawFrame(xMin, yMin, xMax, yMax);
    copyAxisTitle(hFrame->GetXaxis(), f1s[0][0]->GetXaxis());
    copyAxisTitle(hFrame->GetYaxis(), f1s[0][0]->GetYaxis());

    for (int i = 0; i < nPM; ++i) {
        for (int j = 0; j < n; ++j) {
            f1s[i][j]->Draw("same");
        }
    }
    c->Update();

    double legY2 = 0.92;
    double legY1 = legY2 - 0.06*n;
    double legX1 = 0.60;
    // text above the legend
    latex = new TLatex();
    latex->SetTextFont(43);
    latex->SetTextSize(24*grScale);
    latex->DrawLatexNDC(legX1-0.23, legY2-0.03, legendHeader.c_str());
    double leg1Width = 0.26;
    double leg2Offset = 0.08;
    for (int i = 0; i < nPM; ++i) {
        leg = new TLegend(legX1 + i*leg2Offset, legY1, legX1 + leg1Width + i*leg2Offset, legY2);
        leg->SetHeader(legendHeaders[i].c_str());
        for (int j = 0; j < n; ++j) {

            std::string legendText = "";
            if (i == 1) {
                if (jetEtas[j] == 0) legendText = Form("|#eta^{jet}| = 0");
                else                 legendText = Form("|#eta^{jet}| = %.1f", jetEtas[j]);
            }

            leg->AddEntry(f1s[i][j], legendText.c_str(), "l");
        }
        leg->SetTextSize(0.04);
        leg->SetFillStyle(0);
        leg->SetBorderSize(0);
        leg->DrawClone();
        c->Update();
    }

    // text for kinematics
    latex = new TLatex();
    latex->SetTextFont(43);
    latex->SetTextSize(24*grScale);
    std::vector<std::string> textLines = {Form("p_{T}^{jet} = %d GeV/c", jetPt),
                                          Form("#Delta#phi (jet, trk) = 0")};
    int nTextLines = textLines.size();
    for (int i = 0; i < nTextLines; ++i) {
        latex->DrawLatexNDC(legX1, legY1-0.05-0.06*i, textLines.at(i).c_str());
    }

    // text for xi equation
    latex = new TLatex();
    latex->SetTextFont(43);
    latex->SetTextSize(24*grScale);
    latex->DrawLatexNDC(0.18, 0.25, eqnXijet.c_str());

    c->Update();
    /*
    double cnvYMin = c->GetUymin();
    double cnvYMax = c->GetUymax();
    if (logY == 1) {
        cnvYMin = TMath::Power(TMath::Power(10, logY), c->GetUymin());
        cnvYMax = TMath::Power(TMath::Power(10, logY), c->GetUymax());
    }
    */

    /*
    line = new TLine(xMin, 1, xMax, 1);
    line->SetLineStyle(kDashed);
    line->SetLineWidth(line->GetLineWidth()*3);
    line->Draw();
    */

    std::string outName = Form("trackpt_vs_xi_plusminus_jetPt%d_deta0p%d", jetPt, int(deta*10));
    c->SaveAs(Form("%s.png", outName.c_str()));
    c->SaveAs(Form("%s.pdf", outName.c_str()));

    std::cout<<"running trackpt_vs_xi_plusminus() - END"<<std::endl;
}

void phopt_vs_xi()
{
    std::cout<<"running phopt_vs_xi()"   <<std::endl;

    // graphics objects
    TCanvas* c = 0;
    //TLine* line = 0;
    TLegend* leg = 0;
    TLatex* latex = 0;
    //TMathText* mathText = 0;
    int logX = 0;
    int logY = 0;
    int windowSize = 800;
    double grScale = (double)windowSize/600;    // scale for graphics objects

    // axis objects
    double xMin = 0.5;
    double xMax = 4.5;
    double yMin = 0;
    double yMax = 120;

    std::string titleX = xigamma.c_str();
    std::string titleY = ptg.c_str();

    std::string TF1_formula = "[0]*exp(x)";

    double dphi = 8;    // dphi as factor of pi/8
    std::vector<double> minTrkPts = {1, 2, 5};
    std::vector<TF1*> f1s;
    std::vector<int> f1_colors = {kRed, kBlue, kGreen+3};

    TF1* f1 = 0;
    int n = minTrkPts.size();
    for (int i = 0; i < n; ++i) {
        f1 = new TF1(Form("f1_%d", i), TF1_formula.c_str(), xMin, xMax);

        f1->SetParameter(0, minTrkPts[i]);

        // plotting settings
        f1->SetLineColor(f1_colors[i]);
        f1->SetLineWidth(f1->GetLineWidth()*2);

        f1->GetXaxis()->SetTitle(titleX.c_str());
        f1->GetYaxis()->SetTitle(titleY.c_str());
        f1->SetTitle("");

        f1->GetXaxis()->CenterTitle();
        f1->GetYaxis()->CenterTitle();

        f1->GetXaxis()->SetTitleSize(f1->GetXaxis()->GetTitleSize()*grScale);
        f1->GetYaxis()->SetTitleSize(f1->GetYaxis()->GetTitleSize()*grScale);

        f1->GetXaxis()->SetTitleOffset(1.75/grScale);
        f1->GetYaxis()->SetTitleOffset(1.75/grScale);

        f1s.push_back(f1);
    }

    c = new TCanvas("", "", windowSize, windowSize);
    c->SetTickx(1);
    c->SetTicky(1);
    c->SetWindowSize(windowSize, windowSize);
    c->SetMargin(0.15, 0.05, 0.15, 0.05);
    c->SetGrid();
    c->SetLogx(logX);
    c->SetLogy(logY);
    TH1F* hFrame = c->DrawFrame(xMin, yMin, xMax, yMax);
    copyAxisTitle(hFrame->GetXaxis(), f1s[0]->GetXaxis());
    copyAxisTitle(hFrame->GetYaxis(), f1s[0]->GetYaxis());

    for (int i = 0; i < n; ++i) {
        f1s[i]->Draw("same");
    }
    c->Update();

    double legY2 = 0.90;
    double legY1 = legY2 - 0.07*n;
    leg = new TLegend(0.20, legY1, 0.46, legY2);
    for (int i = 0; i < n; ++i) {
        leg->AddEntry(f1s[i], Form("p_{T}^{trk} = %d GeV/c", (int)minTrkPts[i]), "l");
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
    if (dphi != 8) latex->DrawLatexNDC(leg->GetX1NDC()+0.01, legY1-0.05, Form("#Delta#phi (#gamma, trk) = #frac{%.0f}{8} #pi", dphi));
    else           latex->DrawLatexNDC(leg->GetX1NDC()+0.01, legY1-0.05, Form("#Delta#phi (#gamma, trk) = #pi"));

    // text for xi equation
    latex = new TLatex();
    latex->SetTextFont(43);
    latex->SetTextSize(24*grScale);
    latex->DrawLatexNDC(leg->GetX1NDC(), 0.47, eqnXigamma.c_str());

    c->Update();
    /*
    double cnvYMin = c->GetUymin();
    double cnvYMax = c->GetUymax();
    if (logY == 1) {
        cnvYMin = TMath::Power(TMath::Power(10, logY), c->GetUymin());
        cnvYMax = TMath::Power(TMath::Power(10, logY), c->GetUymax());
    }
    */

    /*
    line = new TLine(xMin, 60, xMax, 60);
    line->SetLineStyle(kDashed);
    line->SetLineWidth(line->GetLineWidth()*3);
    line->Draw();
    */

    std::string outName = "phopt_vs_xi";
    c->SaveAs(Form("%s.png", outName.c_str()));
    c->SaveAs(Form("%s.pdf", outName.c_str()));

    std::cout<<"running phopt_vs_xi() - END"<<std::endl;
}

void phopt_vs_xi(int trkPt)
{
    std::cout<<"running phopt_vs_xi()"   <<std::endl;

    // graphics objects
    TCanvas* c = 0;
    //TLine* line = 0;
    TLegend* leg = 0;
    TLatex* latex = 0;
    int logX = 0;
    int logY = 0;
    int windowSize = 800;
    double grScale = (double)windowSize/600;    // scale for graphics objects

    // axis objects
    double xMin = 0.5;
    double xMax = 4.5;
    double yMin = 0;
    double yMax = 120;

    std::string titleX = xigamma.c_str();
    std::string titleY = ptg.c_str();

    std::string TF1_formula = "-[0]*cos([1])*exp(x)";

    // dphi as factor of pi/8
    std::vector<double> dphis = {8, 7};
    std::vector<TF1*> f1s;
    std::vector<int> f1_colors = {kRed, kBlue};
    std::vector<int> f1_lineStyles = {kSolid, 14};

    TF1* f1 = 0;
    int n = dphis.size();
    for (int i = 0; i < n; ++i) {
        f1 = new TF1(Form("f1_%d", i), TF1_formula.c_str(), xMin, xMax);

        f1->SetParameter(0, trkPt);
        f1->SetParameter(1, dphis[i]*TMath::Pi()/8);

        // plotting settings
        f1->SetLineColor(f1_colors[i]);
        f1->SetLineWidth(f1->GetLineWidth()*2);
        f1->SetLineStyle(f1_lineStyles[i]);

        f1->GetXaxis()->SetTitle(titleX.c_str());
        f1->GetYaxis()->SetTitle(titleY.c_str());
        f1->SetTitle("");

        f1->GetXaxis()->CenterTitle();
        f1->GetYaxis()->CenterTitle();

        f1->GetXaxis()->SetTitleSize(f1->GetXaxis()->GetTitleSize()*grScale);
        f1->GetYaxis()->SetTitleSize(f1->GetYaxis()->GetTitleSize()*grScale);

        f1->GetXaxis()->SetTitleOffset(1.75/grScale);
        f1->GetYaxis()->SetTitleOffset(1.75/grScale);

        f1s.push_back(f1);
    }

    c = new TCanvas("", "", windowSize, windowSize);
    c->SetTickx(1);
    c->SetTicky(1);
    c->SetWindowSize(windowSize, windowSize);
    c->SetMargin(0.15, 0.05, 0.15, 0.05);
    c->SetGrid();
    c->SetLogx(logX);
    c->SetLogy(logY);
    TH1F* hFrame = c->DrawFrame(xMin, yMin, xMax, yMax);
    copyAxisTitle(hFrame->GetXaxis(), f1s[0]->GetXaxis());
    copyAxisTitle(hFrame->GetYaxis(), f1s[0]->GetYaxis());

    for (int i = 0; i < n; ++i) {
        f1s[i]->Draw("same");
    }
    c->Update();

    double legY2 = 0.90;
    double legY1 = legY2 - 0.07*n;
    double legX1 = 0.20;
    leg = new TLegend(legX1, legY1, legX1+0.26, legY2);
    for (int i = 0; i < n; ++i) {
        if (dphis[i] != 8) leg->AddEntry(f1s[i], Form("#Delta#phi (#gamma, trk) = #frac{%.0f}{8} #pi", dphis[i]));
        else               leg->AddEntry(f1s[i], Form("#Delta#phi (#gamma, trk) = #pi"), "l");
    }
    leg->SetTextSize(0.04);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->DrawClone();
    c->Update();

    // text for kinematics
    latex = new TLatex();
    latex->SetTextFont(43);
    latex->SetTextSize(24*grScale);
    std::vector<std::string> textLines = {Form("p_{T}^{trk} = %d GeV/c", trkPt)};
    int nTextLines = textLines.size();
    for (int i = 0; i < nTextLines; ++i) {
        latex->DrawLatexNDC(legX1+0.01, legY1-0.05-0.06*i, textLines.at(i).c_str());
    }

    // text for xi equation
    latex = new TLatex();
    latex->SetTextFont(43);
    latex->SetTextSize(24*grScale);
    latex->DrawLatexNDC(legX1, 0.47, eqnXigamma.c_str());

    c->Update();
    /*
    double cnvYMin = c->GetUymin();
    double cnvYMax = c->GetUymax();
    if (logY == 1) {
        cnvYMin = TMath::Power(TMath::Power(10, logY), c->GetUymin());
        cnvYMax = TMath::Power(TMath::Power(10, logY), c->GetUymax());
    }
    */

    /*
    line = new TLine(xMin, 60, xMax, 60);
    line->SetLineStyle(kDashed);
    line->SetLineWidth(line->GetLineWidth()*3);
    line->Draw();
    */

    std::string outName = Form("phopt_vs_xi_trkPt%d", trkPt);
    c->SaveAs(Form("%s.png", outName.c_str()));
    c->SaveAs(Form("%s.pdf", outName.c_str()));

    std::cout<<"running phopt_vs_xi() - END"<<std::endl;
}

void trackpt_vs_xigamma()
{
    std::cout<<"running trackpt_vs_xigamma()"   <<std::endl;

    // graphics objects
    TCanvas* c = 0;
    //TLine* line = 0;
    TLegend* leg = 0;
    TLatex* latex = 0;
    int logX = 0;
    int logY = 1;
    int windowSize = 800;
    double grScale = (double)windowSize/600;    // scale for graphics objects

    // axis objects
    double xMin = 0.5;
    double xMax = 4.5;
    double yMin = 0.8;
    double yMax = 55;

    std::string titleX = xigamma.c_str();
    std::string titleY = pttrk.c_str();

    std::string TF1_formula = "-[0]/(cos([1])*exp(x))";

    double dphi = 8;    // dphi as factor of pi/8
    std::vector<double> minPhoPts = {60, 90, 120};
    std::vector<TF1*> f1s;
    std::vector<int> f1_colors = {kViolet, kOrange+1, kRed+1};

    TF1* f1 = 0;
    int n = minPhoPts.size();
    for (int i = 0; i < n; ++i) {
        f1 = new TF1(Form("f1_%d", i), TF1_formula.c_str(), xMin, xMax);

        f1->SetParameter(0, minPhoPts[i]);
        f1->SetParameter(1, dphi*TMath::Pi()/8);

        // plotting settings
        f1->SetLineColor(f1_colors[i]);
        f1->SetLineWidth(f1->GetLineWidth()*2);

        f1->GetXaxis()->SetTitle(titleX.c_str());
        f1->GetYaxis()->SetTitle(titleY.c_str());
        f1->SetTitle("");

        f1->GetXaxis()->CenterTitle();
        f1->GetYaxis()->CenterTitle();

        f1->GetXaxis()->SetTitleSize(f1->GetXaxis()->GetTitleSize()*grScale);
        f1->GetYaxis()->SetTitleSize(f1->GetYaxis()->GetTitleSize()*grScale);

        f1->GetXaxis()->SetTitleOffset(1.75/grScale);
        f1->GetYaxis()->SetTitleOffset(1.75/grScale);

        f1s.push_back(f1);
    }

    c = new TCanvas("", "", windowSize, windowSize);
    c->SetTickx(1);
    c->SetTicky(1);
    c->SetWindowSize(windowSize, windowSize);
    c->SetMargin(0.15, 0.05, 0.15, 0.05);
    c->SetGrid();
    c->SetLogx(logX);
    c->SetLogy(logY);
    TH1F* hFrame = c->DrawFrame(xMin, yMin, xMax, yMax);
    copyAxisTitle(hFrame->GetXaxis(), f1s[0]->GetXaxis());
    copyAxisTitle(hFrame->GetYaxis(), f1s[0]->GetYaxis());

    for (int i = 0; i < n; ++i) {
        f1s[i]->Draw("same");
    }
    c->Update();

    double legY2 = 0.90;
    double legY1 = legY2 - 0.07*n;
    double legX1 = 0.60;
    leg = new TLegend(legX1, legY1, 0.90, legY2);
    for (int i = 0; i < n; ++i) {
        leg->AddEntry(f1s[i], Form("p_{T}^{#gamma} = %d GeV/c", (int)minPhoPts[i]), "l");
    }
    leg->SetTextSize(0.04);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->DrawClone();
    c->Update();

    // text for kinematics
    latex = new TLatex();
    latex->SetTextFont(43);
    latex->SetTextSize(24*grScale);
    if (dphi != 8) latex->DrawLatexNDC(legX1+0.01, legY1-0.05, Form("#Delta#phi (#gamma, trk) = #frac{%.0f}{8} #pi", dphi));
    else           latex->DrawLatexNDC(legX1+0.01, legY1-0.05, Form("#Delta#phi (#gamma, trk) = #pi"));

    // text for xi equation
    latex = new TLatex();
    latex->SetTextFont(43);
    latex->SetTextSize(24*grScale);
    latex->DrawLatexNDC(0.18, 0.245, eqnXigamma.c_str());

    c->Update();
    /*
    double cnvYMin = c->GetUymin();
    double cnvYMax = c->GetUymax();
    if (logY == 1) {
        cnvYMin = TMath::Power(TMath::Power(10, logY), c->GetUymin());
        cnvYMax = TMath::Power(TMath::Power(10, logY), c->GetUymax());
    }
    */

    /*
    line = new TLine(xMin, 1, xMax, 1);
    line->SetLineStyle(kDashed);
    line->SetLineWidth(line->GetLineWidth()*3);
    line->Draw();
    */

    std::string outName = "trackpt_vs_xigamma";
    c->SaveAs(Form("%s.png", outName.c_str()));
    c->SaveAs(Form("%s.pdf", outName.c_str()));

    std::cout<<"running trackpt_vs_xigamma() - END"<<std::endl;
}

void trackpt_vs_xigamma(int phoPt)
{
    std::cout<<"running trackpt_vs_xigamma()"   <<std::endl;

    gStyle->SetLineStyleString(11,"15 4 4 4");
    gStyle->SetLineStyleString(12,"5 5");
    gStyle->SetLineStyleString(13,"16 4");

    gStyle->SetLineStyleString(14,"20 8");

    // graphics objects
    TCanvas* c = 0;
    //TLine* line = 0;
    TLegend* leg = 0;
    TLatex* latex = 0;
    int logX = 0;
    int logY = 1;
    int windowSize = 800;
    double grScale = (double)windowSize/600;    // scale for graphics objects

    // axis objects
    double xMin = 0.5;
    double xMax = 4.5;
    double yMin = 0.8;
    double yMax = 55;

    std::string titleX = xigamma.c_str();
    std::string titleY = pttrk.c_str();

    std::string TF1_formula = "-[0]/(cos([1])*exp(x))";

    // dphi as factor of pi/8
    std::vector<double> dphis = {8, 7};
    std::vector<TF1*> f1s;
    std::vector<int> f1_colors = {kRed, kBlue};
    std::vector<int> f1_lineStyles = {kSolid, 14};

    TF1* f1 = 0;
     int n = dphis.size();
     for (int i = 0; i < n; ++i) {
         f1 = new TF1(Form("f1_%d", i), TF1_formula.c_str(), xMin, xMax);

         f1->SetParameter(0, phoPt);
         f1->SetParameter(1, dphis[i]*TMath::Pi()/8);

         // plotting settings
         f1->SetLineColor(f1_colors[i]);
         f1->SetLineWidth(f1->GetLineWidth()*2);
         f1->SetLineStyle(f1_lineStyles[i]);

         f1->GetXaxis()->SetTitle(titleX.c_str());
         f1->GetYaxis()->SetTitle(titleY.c_str());
         f1->SetTitle("");

         f1->GetXaxis()->CenterTitle();
         f1->GetYaxis()->CenterTitle();

         f1->GetXaxis()->SetTitleSize(f1->GetXaxis()->GetTitleSize()*grScale);
         f1->GetYaxis()->SetTitleSize(f1->GetYaxis()->GetTitleSize()*grScale);

         f1->GetXaxis()->SetTitleOffset(1.75/grScale);
         f1->GetYaxis()->SetTitleOffset(1.75/grScale);

         f1s.push_back(f1);
     }

    c = new TCanvas("", "", windowSize, windowSize);
    c->SetTickx(1);
    c->SetTicky(1);
    c->SetWindowSize(windowSize, windowSize);
    c->SetMargin(0.15, 0.05, 0.15, 0.05);
    c->SetGrid();
    c->SetLogx(logX);
    c->SetLogy(logY);
    TH1F* hFrame = c->DrawFrame(xMin, yMin, xMax, yMax);
    copyAxisTitle(hFrame->GetXaxis(), f1s[0]->GetXaxis());
    copyAxisTitle(hFrame->GetYaxis(), f1s[0]->GetYaxis());

    for (int i = 0; i < n; ++i) {
        f1s[i]->Draw("same");
    }
    c->Update();

    double legY2 = 0.90;
    double legY1 = legY2 - 0.07*n;
    double legX1 = 0.60;
    leg = new TLegend(legX1, legY1, 0.90, legY2);
    for (int i = 0; i < n; ++i) {
        if (dphis[i] != 8) leg->AddEntry(f1s[i], Form("#Delta#phi (#gamma, trk) = #frac{%.0f}{8} #pi", dphis[i]));
        else               leg->AddEntry(f1s[i], Form("#Delta#phi (#gamma, trk) = #pi"), "l");
    }
    leg->SetTextSize(0.04);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->DrawClone();
    c->Update();

    // text for kinematics
    latex = new TLatex();
    latex->SetTextFont(43);
    latex->SetTextSize(24*grScale);
    std::vector<std::string> textLines = {Form("p_{T}^{#gamma} = %d GeV/c", phoPt)};
    int nTextLines = textLines.size();
    for (int i = 0; i < nTextLines; ++i) {
        latex->DrawLatexNDC(legX1+0.01, legY1-0.05-0.06*i, textLines.at(i).c_str());
    }

    // text for xi equation
    latex = new TLatex();
    latex->SetTextFont(43);
    latex->SetTextSize(24*grScale);
    latex->DrawLatexNDC(0.18, 0.245, eqnXigamma.c_str());

    c->Update();
    /*
    double cnvYMin = c->GetUymin();
    double cnvYMax = c->GetUymax();
    if (logY == 1) {
        cnvYMin = TMath::Power(TMath::Power(10, logY), c->GetUymin());
        cnvYMax = TMath::Power(TMath::Power(10, logY), c->GetUymax());
    }
    */

    /*
    line = new TLine(xMin, 1, xMax, 1);
    line->SetLineStyle(kDashed);
    line->SetLineWidth(line->GetLineWidth()*3);
    line->Draw();
    */

    std::string outName = Form("trackpt_vs_xigamma_phoPt%d", phoPt);
    c->SaveAs(Form("%s.png", outName.c_str()));
    c->SaveAs(Form("%s.pdf", outName.c_str()));

    std::cout<<"running trackpt_vs_xigamma() - END"<<std::endl;
}

int main()
{
    plotXiPhaseSpace();
    return 0;
}

double getXNDC(TPad* pad, double x) {
  pad->Update();//this is necessary!

  if (pad->GetLogx() == 1) {
      x = TMath::Log10(x);
  }
  return (x - pad->GetX1()) / (pad->GetX2()-pad->GetX1());
}

double getYNDC(TPad* pad, double y) {
    pad->Update();//this is necessary!

    if (pad->GetLogy() == 1) {
        y = TMath::Log10(y);
    }
    return (y - pad->GetY1()) / (pad->GetY2()-pad->GetY1());
}

void copyAxisTitle(TAxis* axis1, TAxis* axis2)
{
    axis1->SetTitle(axis2->GetTitle());
    axis1->SetTitleColor(axis2->GetTitleColor());
    axis1->SetTitleFont(axis2->GetTitleFont());
    axis1->SetTitleOffset(axis2->GetTitleOffset());
    axis1->SetTitleSize(axis2->GetTitleSize());
    axis1->CenterTitle(axis2->GetCenterTitle());
    axis1->RotateTitle(axis2->GetRotateTitle());
}
