/*
 * utilities related to TCanvas, TPad objects.
 */

#include <TCanvas.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TList.h>
#include <TIterator.h>
#include <TH1.h>
#include <TH1D.h>
#include <TAttFill.h>

#include <string>
#include <vector>
#include <utility>      // std::pair

#ifndef CANVASUTIL_H_
#define CANVASUTIL_H_

void setCanvasFinal(TCanvas* c, int logx = 0, int logy = 0, int logz = 0);
void setCanvasMargin(TCanvas* c, float leftMargin = 0.1, float rightMargin = 0.1, float bottomMargin = 0.1, float topMargin = 0.1);
void setTH1Final   (TH1* c);
void setLegendFinal(TLegend* legend);
void setLegendPosition(TLegend* legend, std::string position, TCanvas* c);
void setLegendPosition(TLegend* legend, std::string position, TCanvas* c, double height, double width, double offsetX = 0, double offsetY = 0);
std::vector<std::pair<float, float>> calcTextCoordinates(std::vector<std::string> lines, std::string position, TCanvas* c, double offsetX = 0, double offsetY = 0, float lineOffset = 0.05);
double calcTextWidth(std::vector<std::string> lines, TCanvas* c, float textSize = 18);
double calcTLegendHeight(TLegend* legend, double offset = 0.0375, double ratio = 0.0375);
double calcTLegendWidth (TLegend* legend, double offset = 0.06,   double ratio = 25./3000, double threshold = 0.2);

void setCanvasTLatex(TCanvas* c, float px, float py, std::vector<std::string> lines, float pyOffset = 0.05);

void setCanvas_InvMass(TCanvas* c, float px = 0.65, float py = 0.85,
                       const char* collision = "CMS pp", const char* energy    = "#sqrt{s} = 5.02 TeV",
                       const char* ptCut     = "p^{e}_{T} > 20 GeV/c", const char* etaCut = "|#eta^{e}| <1 .44",
                       const char* extra = "");

void setCanvas_InvMass_pp(TCanvas* c, float px = 0.65, float py = 0.85,
                          const char* ptCut = "p^{e}_{T} > 20 GeV/c", const char* etaCut = "|#eta^{e}| < 1.44",
                          const char* extra = "");

void setCanvas_InvMass_PbPb(TCanvas* c, float px = 0.65, float py = 0.85,
                          const char* ptCut = "p^{e}_{T} > 20 GeV/c", const char* etaCut = "|#eta^{e}| < 1.44",
                          const char* extra = "");

void setCanvasFinal(TCanvas* c, int logx, int logy, int logz)
{
    c->SetBorderMode(0);
    c->SetBorderSize(0);
    c->SetFrameBorderMode(0);
    c->SetFrameLineColor(0);

    // put ticks to upper and right part of the axis.
    c->SetTickx(1);
    c->SetTicky(1);

    c->SetLogx(logx);
    c->SetLogy(logy);
    c->SetLogz(logz);
}

/*
 * modifies canvas margins such that size of the original figure does not change.
 */
void setCanvasMargin(TCanvas* c, float leftMargin, float rightMargin, float bottomMargin, float topMargin)
{
    float defaultMargin = 0.1;
    UInt_t width = c->GetWindowWidth();
    UInt_t height = c->GetWindowHeight();

    // assume the function is called in batch mode, so use SetCanvasSize() instead of SetWindowSize()
    c->SetCanvasSize(width* (1 - defaultMargin*2 + leftMargin + rightMargin),
                     height*(1 - defaultMargin*2 + bottomMargin + topMargin));
    c->SetLeftMargin(leftMargin);
    c->SetRightMargin(rightMargin);
    c->SetBottomMargin(bottomMargin);
    c->SetTopMargin(topMargin);
}

void setTH1Final(TH1* h)
{
    h->SetTitleOffset(1.25, "X");
    h->SetTitleOffset(1.5,  "Y");

    h->SetTitle("");
    h->SetStats(false);
}

void setLegendFinal(TLegend* legend)
{
    legend->SetBorderSize(0);
    legend->SetTextFont(43);
    legend->SetTextSize(20*0.8);
    legend->SetLineColor(1);
    legend->SetLineStyle(kSolid); // kSolid = 1
    legend->SetLineWidth(1);
    legend->SetFillColor(kWhite);   // kWhite = 0
    legend->SetFillStyle(0);  // kFEmpty = 0
}

void setLegendPosition(TLegend* legend, std::string position, TCanvas* c)
{
    setLegendPosition(legend, position, c, legend->GetX2NDC() - legend->GetX1NDC(), legend->GetY2NDC() - legend->GetY1NDC());
}

/*
 * offsetX and offsetY are the distances of the legend from the corresponding corner.
 * Ex. If position = NE, then the legend will be put such that the upper-right corner of the legend
 *     has distance of offsetX and offsetY to the upper-right corner of the canvas
 */
void setLegendPosition(TLegend* legend, std::string position, TCanvas* c, double height, double width, double offsetX, double offsetY)
{
    if (width>0.50) legend->SetMargin(0.075);     // if the legend is wide, then keep the length of the line not wide.
    else if (width>0.25) legend->SetMargin(0.15);     // if the legend is wide, then keep the length of the line not wide.
    // TLegend::SetMargin() helps to set the length of the line in the legend entry.
    if (position.compare("NW") == 0) { // upper-left corner
        legend->SetX1(c->GetLeftMargin() + offsetX);
        legend->SetY1(1 - c->GetTopMargin() - height - offsetY);
        legend->SetX2(c->GetLeftMargin() + width + offsetX);
        legend->SetY2(1 - c->GetTopMargin() - offsetY);
    }
    else if (position.compare("NE") == 0) { // upper-right corner
        legend->SetX1(1 - c->GetRightMargin() - width - offsetX);
        legend->SetY1(1 - c->GetTopMargin() - height - offsetY);
        legend->SetX2(1 - c->GetRightMargin() - offsetX);
        legend->SetY2(1 - c->GetTopMargin() - offsetY);
    }
    else if (position.compare("SW") == 0) { // lower-left corner
        legend->SetX1(c->GetLeftMargin() + offsetX);
        legend->SetY1(c->GetBottomMargin() + offsetY);
        legend->SetX2(c->GetLeftMargin() + width + offsetX);
        legend->SetY2(c->GetBottomMargin() + height + offsetY);
    }
    else if (position.compare("SE") == 0) { // lower-right corner
        legend->SetX1(1 - c->GetRightMargin() - width - offsetX);
        legend->SetY1(c->GetBottomMargin() + offsetY);
        legend->SetX2(1 - c->GetRightMargin() - offsetX);
        legend->SetY2(c->GetBottomMargin() + height + offsetY);
    }
}

std::vector<std::pair<float, float>> calcTextCoordinates(std::vector<std::string> lines, std::string position, TCanvas* c, double offsetX, double offsetY, float lineOffset)
{

    float x = 0.1;
    float y = 0.1;
    if (position.compare("NW") == 0) { // upper-left corner
        x = c->GetLeftMargin() + offsetX;
        y = 1 - c->GetTopMargin() - offsetY;
    }
    else if (position.compare("SW") == 0) { // lower-left corner
        x = c->GetLeftMargin() + offsetX;
        y = c->GetBottomMargin() + offsetY;
    }

    std::vector<std::pair<float, float>> coordinatesNDC;
    for (std::vector<std::string>::const_iterator it = lines.begin(); it != lines.end(); ++it){

        coordinatesNDC.push_back(std::make_pair(x, y));
        y -= lineOffset;
    }

    return coordinatesNDC;
}

double calcTextWidth(std::vector<std::string> lines, TCanvas* c, float textSize)
{
    c->cd();

    double w = 0;
    for (std::vector<std::string>::const_iterator it = lines.begin(); it != lines.end(); ++it){
        TLatex latex(0.1, 0.1, (*it).c_str());  // dummy TLatex object to calculate text width
        double tmp = latex.GetXsize();
        if (tmp > w)    w = tmp;
    }

    return w;
}

/*
 *  calculate a proper height for the TLegend object using the number of rows (excluding the header)
 */
double calcTLegendHeight(TLegend* legend, double offset, double ratio) {

    int nEntries = legend->GetNRows();
    if (legend->GetHeader() != NULL) nEntries--;
    return ratio*nEntries + offset;
}

/*
 *  calculate a proper width for the TLegend object that can cover the entry with longest label (excluding the header)
 */
double calcTLegendWidth(TLegend* legend, double offset, double ratio, double threshold) {

    TIter iter(legend->GetListOfPrimitives());
    TLegendEntry* entry;
    double w = 0;
    while (( entry = (TLegendEntry*)iter.Next() )) {
        std::string label = entry->GetLabel();
        if (legend->GetHeader() != NULL && label.compare(legend->GetHeader()) == 0)  continue;    // in this case, assume that the entry is actually the header

        double c = 1;
        if (label.length() > 50) c = 1.5;
        else if (label.length() > 30) c = 1.6;
        else if (label.length() > 15) c = 1.4 + (label.length()-15)* 0.2 / 15 ;

        double tmp = c*ratio*label.length() + offset;
        if (tmp < threshold) tmp = threshold;
        if (tmp > w) w = tmp;
    }

    return w;
}

void setCanvasTLatex(TCanvas* c, float px, float py, std::vector<std::string> lines, float pyOffset)
{
    c->cd();

    float pyNew = py;
    TLatex* latex;
    for (std::vector<std::string>::const_iterator it = lines.begin(); it != lines.end(); ++it){

        latex = new TLatex(px, pyNew, (*it).c_str());
        latex->SetTextFont(43);
        latex->SetTextSize(20*0.8);
        latex->SetNDC();
        latex->Draw();
        pyNew = pyNew - pyOffset;
    }
}

void setCanvas_InvMass(TCanvas* c, float px, float py, const char* collision, const char* energy,
                       const char* ptCut, const char* etaCut, const char* extra)
{
  c->cd();

  float pyNew = py;
  TLatex *latexCMS = new TLatex(px,pyNew, collision);
  latexCMS->SetTextFont(43);
  latexCMS->SetTextSize(20);
  latexCMS->SetNDC();
  latexCMS->Draw();

  pyNew = pyNew-0.05;
  TLatex *latexEnergy = new TLatex(px, pyNew, energy);
  latexEnergy->SetTextFont(43);
  latexEnergy->SetTextSize(20);
  latexEnergy->SetNDC();
  latexEnergy->Draw();

  pyNew = pyNew-0.05;
  TLatex *latexPt = new TLatex(px, pyNew, ptCut);
  latexPt->SetTextFont(43);
  latexPt->SetTextSize(20);
  latexPt->SetNDC();
  latexPt->Draw();

  pyNew = pyNew-0.06;
  TLatex *latexEta = new TLatex(px, pyNew, etaCut);
  latexEta->SetTextFont(43);
  latexEta->SetTextSize(20);
  latexEta->SetNDC();
  latexEta->Draw();

  std::string str = extra;
  if (str.compare("") != 0)
  {
      pyNew = pyNew-0.05;
      TLatex *latexExtra = new TLatex(px, pyNew, extra);
      latexExtra->SetTextFont(43);
      latexExtra->SetTextSize(20);
      latexExtra->SetNDC();
      latexExtra->Draw();
  }
}

void setCanvas_InvMass_pp(TCanvas* c, float px, float py, const char* ptCut, const char* etaCut, const char* extra)
{
    setCanvas_InvMass(c, px, py, "CMS preliminary pp", "#sqrt{s} = 5.02 TeV", ptCut, etaCut, extra);
}

void setCanvas_InvMass_PbPb(TCanvas* c, float px, float py, const char* ptCut, const char* etaCut, const char* extra)
{
    setCanvas_InvMass(c, px, py, "CMS preliminary PbPb", "#sqrt{s_{NN}} = 5.02 TeV", ptCut, etaCut, extra);
}

void easyLeg( TLegend *legend=0 , const char* head="")
{
  legend->SetBorderSize(0);
  legend->SetHeader(head);
  legend->SetTextFont(43);
  legend->SetTextSize(20);
  legend->SetLineColor(1);
  legend->SetLineStyle(kSolid); // kSolid = 1
  legend->SetLineWidth(1);
  legend->SetFillColor(kWhite);   // kWhite = 0
  legend->SetFillStyle(0);  // kFEmpty = 0
}

/*
 * h1 is assumed to be the histogram for opposite charge.
 * h2 is assumed to be the histogram for same charge.
 */
void setCanvas_InvMass_Histo2Legend(TCanvas* c, TH1* h1, TH1* h2)
{
    c->cd();

    TLegend *legend = new TLegend(0.1,0.75,0.4,0.9,"","brNDC");
    easyLeg(legend,"");

    legend->AddEntry(h1, "opposite charge", "p");
    legend->AddEntry(h2, "same charge", "p");

    legend->Draw();
}

#endif /* CANVASUTIL_H_ */
