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

#ifndef STYLEUTIL_H_
#define STYLEUTIL_H_

void setCanvasFinal(TCanvas* c, int logx = 0, int logy = 0, int logz = 0);
void setCanvasMargin(TCanvas* c, float leftMargin = 0.1, float rightMargin = 0.1, float bottomMargin = 0.1, float topMargin = 0.1);
void setCanvasSizeMargin(TCanvas* c, double normWidth = 1, double normHeight = 1, float leftMargin = 0.1, float rightMargin = 0.1, float bottomMargin = 0.1, float topMargin = 0.1);
void divideCanvas(TCanvas* c, int rows = 1, int columns = 1, float leftMargin = 0.1, float rightMargin = 0.1, float bottomMargin = 0.1, float topMargin = 0.1, float xMargin = 0.01, float yMargin = 0.01, float yMinOffSet = 0);
void divideCanvas(TCanvas* c, TPad* pads[], int rows = 1, int columns = 1, float leftMargin = 0.1, float rightMargin = 0.1, float bottomMargin = 0.1, float topMargin = 0.1, float xMargin = 0.01, float yMargin = 0.01, float yMinOffSet = 0);
void setPadFinal(TPad* pad, int logx = 0, int logy = 0, int logz = 0);
void setTH1Final (TH1* h);
void setTH1Ratio (TH1* h, TH1* hBase, double factor);
void setLegendFinal(TLegend* legend);
void setLegendPosition(TLegend* legend, std::string position, TCanvas* c);
void setLegendPosition(TLegend* legend, std::string position, TCanvas* c, double height, double width, double offsetX = 0, double offsetY = 0);
void setLegendPosition(TLegend* legend, std::string position, TPad* pad, double height, double width, double offsetX = 0, double offsetY = 0);
void setTextAlignment(TLatex* latex, std::string position);
void setTextAbovePad(TLatex* latex, TPad* pad, double offsetX = 0, double offsetY = 0);
std::vector<std::pair<float, float>> calcTextCoordinates(std::vector<std::string> lines, std::string position, TCanvas* c, double offsetX = 0, double offsetY = 0, float lineOffset = 0.05);
void drawTextLines(TLatex* latex, TPad* pad, std::vector<std::string> lines, std::string position, double offsetX = 0, double offsetY = 0);
double calcNormCanvasWidth(int columns = 1, float leftMargin = 0.1, float rightMargin = 0.1, float xMargin = 0.01);
double calcNormCanvasHeight(int rows = 1, float bottomMargin = 0.1, float topMargin = 0.1, float yMargin = 0.01);
double calcTextWidth(std::vector<std::string> lines, TCanvas* c);
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
    setPadFinal(c, logx, logy, logz);
}

/*
 * modifies canvas margins such that size of the original figure does not change.
 */
void setCanvasMargin(TCanvas* c, float leftMargin, float rightMargin, float bottomMargin, float topMargin)
{
    float defaultMargin = 0.1;
    double normWidth  = (1 - defaultMargin*2 + leftMargin + rightMargin);
    double normHeight = (1 - defaultMargin*2 + bottomMargin + topMargin);

    setCanvasSizeMargin(c, normWidth, normHeight, leftMargin, rightMargin, bottomMargin, topMargin);
}

/*
 * modifies canvas size and margins such that size of the original figure inside for a 1x1 Canvas does not change.
 */
void setCanvasSizeMargin(TCanvas* c, double normWidth, double normHeight, float leftMargin, float rightMargin, float bottomMargin, float topMargin)
{
    // https://root.cern.ch/doc/master/TCanvas_8h_source.html#l00058
    UInt_t width = c->GetWindowWidth();     // fWindowWidth  : Width of window (including borders, etc.)
    UInt_t height = c->GetWindowHeight();   // fWindowHeight : Height of window (including menubar, borders, etc.)

    // assume the function is called in batch mode, so use SetCanvasSize() instead of SetWindowSize()
    c->SetCanvasSize(width*normWidth, height*normHeight);
    c->SetLeftMargin(leftMargin);
    c->SetRightMargin(rightMargin);
    c->SetBottomMargin(bottomMargin);
    c->SetTopMargin(topMargin);
}

void divideCanvas(TCanvas* c, int rows, int columns, float leftMargin, float rightMargin, float bottomMargin, float topMargin, float xMargin, float yMargin, float yMinOffset)
{
    TPad* pads[rows*columns];
    divideCanvas(c, pads, rows, columns, leftMargin, rightMargin, bottomMargin, topMargin, xMargin, yMargin, yMinOffset);
}

void divideCanvas(TCanvas* c, TPad* pads[], int rows, int columns, float leftMargin, float rightMargin, float bottomMargin, float topMargin, float xMargin, float yMargin, float yMinOffset)
{
    c->Clear();

    double normPadWidth = calcNormCanvasWidth(1, leftMargin, rightMargin, xMargin);
    double normPadHeight = calcNormCanvasHeight(1, bottomMargin, topMargin, yMargin);

    float x_min[columns], x_max[columns];
    x_min[0] = 0;
    x_max[0] = normPadWidth + leftMargin - xMargin/2;   // left margin is inside the width of leftmost panel
    for (int i = 1; i < columns; ++i) {
        x_min[i] = x_max[i-1];
        x_max[i] = x_max[i-1] + normPadWidth;
    }
    x_max[columns-1] += rightMargin - xMargin/2;

    float y_min[rows], y_max[rows];
    y_min[rows-1] = yMinOffset;
    y_max[rows-1] = normPadHeight + bottomMargin - yMargin/2;  // bottom margin is inside the height of bottom panel
    for (int i = rows - 2; i >= 0; --i) {
        y_min[i] = y_max[i+1]+yMinOffset;
        y_max[i] = y_min[i] + normPadHeight;
    }
    y_max[0] += topMargin - yMargin/2;

    double normCanvasWidth = x_max[columns-1];
    double normCanvasHeight = y_max[0];
    // normalize the coordinates such that x_max[columns-1] = 1 and y_max[0] = 1
    for (int i = 0; i < columns; ++i) {
        x_min[i] /= normCanvasWidth;
        x_max[i] /= normCanvasWidth;
    }
    for (int i = 0; i < rows; ++i) {
        y_min[i] /= normCanvasHeight;
        y_max[i] /= normCanvasHeight;
    }

    for (int i=0; i<rows; i++) {
        for (int j=0; j<columns; j++) {
            c->cd();
            int ij = i*columns+j;
            pads[ij] = new TPad(Form("pad_%d_%d", i, j), Form("pad_%d_%d", i, j), x_min[j], y_min[i], x_max[j], y_max[i]);

            if (i == 0) pads[ij]->SetTopMargin(topMargin);
            else pads[ij]->SetTopMargin(yMargin/2);
            if (i == rows - 1) pads[ij]->SetBottomMargin(bottomMargin);
            else pads[ij]->SetBottomMargin(yMargin/2);
            if (j == 0) pads[ij]->SetLeftMargin(leftMargin);
            else pads[ij]->SetLeftMargin(xMargin/2);
            if (j == columns - 1) pads[ij]->SetRightMargin(rightMargin);
            else pads[ij]->SetRightMargin(xMargin/2);

            pads[ij]->Draw();
            pads[ij]->cd();
            pads[ij]->SetNumber(ij+1);

            setPadFinal(pads[ij]);
        }
    }
}

void setPadFinal(TPad* pad, int logx, int logy, int logz)
{
    pad->SetBorderMode(0);
    pad->SetBorderSize(0);
    pad->SetFrameBorderMode(0);
    pad->SetFrameLineColor(0);

    // put ticks to upper and right part of the axis.
    pad->SetTickx(1);
    pad->SetTicky(1);

    pad->SetLogx(logx);
    pad->SetLogy(logy);
    pad->SetLogz(logz);
}

void setTH1Final(TH1* h)
{
    h->SetTitleOffset(1.25, "X");
    h->SetTitleOffset(2,  "Y");

    h->SetTitle("");
    h->SetStats(false);

    h->GetXaxis()->CenterTitle();
    h->GetYaxis()->CenterTitle();
}

/*
 * set the axis properties of a "ratio histogram" using the axis properties of the original histogram.
 * this function is useful for canvases where the ratio histogram is drawn in a small pad below the original histogram.
 * If the axis properties of the ratio is not set properly, then they will appear significantly small in the final canvas because the pad
 * for ratio histogram is small.
 *
 * axis properties of the ratio histogram will be scaled with "factor"
 */
void setTH1Ratio(TH1* h, TH1* hBase, double factor)
{
    // default titles for the ratio histogram.
    h->SetTitle("");
    h->SetXTitle("");      // no x-axis title for ratio histograms
    //h->SetYTitle("Ratio");      // no x-axis title for ratio histograms

    h->SetStats(false);

    h->SetLabelSize(hBase->GetLabelSize("X")*factor, "X");   //in pixels
    h->SetLabelSize(hBase->GetLabelSize("Y")*factor, "Y");   //in pixels

    h->SetTitleSize(hBase->GetTitleSize("X")*factor, "X");   //in pixels
    h->SetTitleSize(hBase->GetTitleSize("Y")*factor, "Y");   //in pixels

    h->SetTitleOffset(hBase->GetTitleOffset("X")/factor, "X");
    h->SetTitleOffset(hBase->GetTitleOffset("Y")/factor, "Y");
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
    setLegendPosition(legend, position, (TPad*)c, height, width, offsetX, offsetY);
}

/*
 * offsetX and offsetY are the distances of the legend from the corresponding corner.
 * Ex. If position = NE, then the legend will be put such that the upper-right corner of the legend
 *     has distance of offsetX and offsetY to the upper-right corner of the canvas
 */
void setLegendPosition(TLegend* legend, std::string position, TPad* pad, double height, double width, double offsetX, double offsetY)
{
    if (width>0.50) legend->SetMargin(0.075);     // if the legend is wide, then keep the length of the line not wide.
    else if (width>0.25) legend->SetMargin(0.15);     // if the legend is wide, then keep the length of the line not wide.
    // TLegend::SetMargin() helps to set the length of the line in the legend entry.
    if (position == "NW") { // upper-left corner
        legend->SetX1(pad->GetLeftMargin() + offsetX);
        legend->SetY1(1 - pad->GetTopMargin() - height - offsetY);
        legend->SetX2(pad->GetLeftMargin() + width + offsetX);
        legend->SetY2(1 - pad->GetTopMargin() - offsetY);
    }
    else if (position == "NE") { // upper-right corner
        legend->SetX1(1 - pad->GetRightMargin() - width - offsetX);
        legend->SetY1(1 - pad->GetTopMargin() - height - offsetY);
        legend->SetX2(1 - pad->GetRightMargin() - offsetX);
        legend->SetY2(1 - pad->GetTopMargin() - offsetY);
    }
    else if (position == "SW") { // lower-left corner
        legend->SetX1(pad->GetLeftMargin() + offsetX);
        legend->SetY1(pad->GetBottomMargin() + offsetY);
        legend->SetX2(pad->GetLeftMargin() + width + offsetX);
        legend->SetY2(pad->GetBottomMargin() + height + offsetY);
    }
    else if (position == "SE") { // lower-right corner
        legend->SetX1(1 - pad->GetRightMargin() - width - offsetX);
        legend->SetY1(pad->GetBottomMargin() + offsetY);
        legend->SetX2(1 - pad->GetRightMargin() - offsetX);
        legend->SetY2(pad->GetBottomMargin() + height + offsetY);
    }
}

void setTextAlignment(TLatex* latex, std::string position)
{
    int verticalAlign = latex->GetTextAlign() % 10;
    if (position == "NW" || position == "SW") { // left corner
        latex->SetTextAlign(10 + verticalAlign);    // horizontal alignment is left
    }
    else if (position == "NE" || position == "SE") { // right corner
        latex->SetTextAlign(30 + verticalAlign);    // horizontal alignment is right
    }
}

/*
 * set properties of a TLatex object that should lie above a TPad
 * such objects are thing
 */
void setTextAbovePad(TLatex* latex, TPad* pad, double offsetX, double offsetY)
{
    // assume the text objects is to be left aligned.
    float x = pad->GetLeftMargin() + offsetX;
    float y = 1 - pad->GetTopMargin() + offsetY;
    if (latex->GetTextAlign() / 10 == 3) {  // text object is set to be right aligned.
        x = 1 - pad->GetRightMargin() - offsetX;
    }

    latex->SetX(x);
    latex->SetY(y);
}

std::vector<std::pair<float, float>> calcTextCoordinates(std::vector<std::string> lines, std::string position, TCanvas* c, double offsetX, double offsetY, float lineOffset)
{
    float x = 0.1;
    float y = 0.1;
    if (position == "NW") { // upper-left corner
        x = c->GetLeftMargin() + offsetX;
        y = 1 - c->GetTopMargin() - offsetY;
    }
    else if (position == "NE") { // upper-right corner
        x = 1 - c->GetRightMargin() - offsetX;
        y = 1 - c->GetTopMargin() - offsetY;
    }
    else if (position == "SW") { // lower-left corner
        x = c->GetLeftMargin() + offsetX;
        y = c->GetBottomMargin() + offsetY;
    }
    else if (position == "SE") { // lower-right corner
        x = 1 - c->GetRightMargin() - offsetX;
        y = c->GetBottomMargin() + offsetY;
    }

    std::vector<std::pair<float, float>> coordinatesNDC;
    for (std::vector<std::string>::const_iterator it = lines.begin(); it != lines.end(); ++it){

        coordinatesNDC.push_back(std::make_pair(x, y));
        y -= lineOffset;
    }

    return coordinatesNDC;
}

void drawTextLines(TLatex* latex, TPad* pad, std::vector<std::string> lines, std::string position, double offsetX, double offsetY)
{
    std::vector<std::pair<float,float>> textCoordinates = calcTextCoordinates(lines, position, (TCanvas*)pad, offsetX, offsetY);
    int nLines = lines.size();

    for (int i = 0; i<nLines; ++i){
        float x = textCoordinates.at(i).first;
        float y = textCoordinates.at(i).second;
        if (lines.at(i) != CONFIGPARSER::nullInput.c_str())
            latex->DrawLatexNDC(x, y, lines.at(i).c_str());
    }
}

/*
 * calculate the width of a TCanvas in a normalization scheme where the width is 1 for
 * a canvas with a columns = 1, bottomMargin + topMargin = 2*defaultMargin and xMargin = 0
 */
double calcNormCanvasWidth(int columns, float leftMargin, float rightMargin, float xMargin)
{
    double defaultMargin = 0.1;
    double padWidth = (1 - defaultMargin*2 + xMargin);

    float x_max[columns];
    x_max[0] = padWidth + leftMargin - xMargin/2;   // left margin is inside the width of leftmost panel
    for (int i = 1; i < columns; ++i) {
        x_max[i] = x_max[i-1] + padWidth;
    }
    x_max[columns-1] += rightMargin - xMargin/2;

    return x_max[columns-1];
}

/*
 * calculate the height of a TCanvas in a normalization scheme where the height is 1 for
 * a canvas with a rows = 1, bottomMargin + topMargin = 2*defaultMargin and yMargin = 0
 */
double calcNormCanvasHeight(int rows, float bottomMargin, float topMargin, float yMargin)
{
    double defaultMargin = 0.1;
    double padHeight = (1 - defaultMargin*2 + yMargin);

    float y_min[rows], y_max[rows];
    y_min[rows-1] = 0;
    y_max[rows-1] = padHeight + bottomMargin - yMargin/2;  // bottom margin is inside the height of bottom panel
    for (int i = rows - 2; i >= 0; --i) {
        y_min[i] = y_max[i+1];
        y_max[i] = y_min[i] + padHeight;
    }
    y_max[0] += topMargin - yMargin/2;

    return y_max[0];
}

double calcTextWidth(std::vector<std::string> lines, TCanvas* c)
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
        if (legend->GetHeader() != NULL && label == legend->GetHeader())  continue;    // in this case, assume that the entry is actually the header

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
  if (str != "")
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

#endif /* STYLEUTIL_H_ */
