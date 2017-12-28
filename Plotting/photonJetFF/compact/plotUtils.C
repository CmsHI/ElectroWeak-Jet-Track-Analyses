
static  int      myDarkRed     = TColor::GetColor(128,0,0);
static  int      myDarkGreen   = TColor::GetColor(0,128,0);
static  int      myDarkBlue    = TColor::GetColor(0,0,128);

TH1F *DrawFrame(Double_t xmin, Double_t xmax, Double_t ymin, Double_t ymax, TString xTitle = "", TString yTitle = "", double a = 0.1, bool setMargins = true) {

  if(setMargins) {
  gPad->SetLeftMargin(0.22);
  gPad->SetBottomMargin(0.15);
  gPad->SetRightMargin(0.1);//0.05);
  gPad->SetTopMargin(0.05);
  }

  TH1F *frame = gPad->DrawFrame(xmin,ymin,xmax,ymax);
  frame->SetXTitle(xTitle.Data());
  frame->SetYTitle(yTitle.Data());
  frame->GetXaxis()->SetLabelSize(0.05);
  frame->GetYaxis()->SetLabelSize(0.05);
  frame->GetXaxis()->SetTitleSize(0.06);
  frame->GetYaxis()->SetTitleSize(0.06);
  frame->GetXaxis()->SetTitleOffset(1.0);
  frame->GetYaxis()->SetTitleOffset(1.3);
  frame->GetXaxis()->CenterTitle(true);
  frame->GetYaxis()->CenterTitle(true);

  gPad->SetTicks(1,1);

  return frame;
}

TLegend *CreateLegend(Double_t xmin, Double_t xmax, Double_t ymin, Double_t ymax, TString title = "", Double_t textSize = 0.06) {

  TLegend *leg = new TLegend(xmin,ymin,xmax,ymax,title.Data());
  leg->SetFillColor(10);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetTextSize(textSize);
  leg->SetTextFont(42);

  return leg;
}

void DrawLatex(Double_t x, Double_t y, TString strText = "", Double_t textSize = 0.06, Int_t color = 1, int all = 11, bool bndc = true) {	
  TLatex text;
  if(bndc) text.SetNDC();
  text.SetTextAlign(all);
  text.SetTextSize(textSize);
  text.SetTextFont(42);
  text.SetTextColor(color);
  text.DrawLatex(x,y,strText.Data());

  //  return text;
}

Int_t GetColor(Int_t i) {
  const Int_t nc = 11;
  Int_t color[nc] = {1,kRed+1,4,kGreen+2,kOrange+7,kGray+2,myDarkRed,kAzure+10,kGreen+4,kYellow+2,kGreen};
  if(i<nc) return color[i];
  else     return i;
}

Int_t GetFillColor(Int_t i) {
  const Int_t nc = 11;
  Int_t color[nc] = {kGray,kRed-9,kAzure+1,kGreen-6,kOrange-9,kGray,kRed-6,kAzure+6,kGreen-5,kYellow-3,kGreen-9};
  if(i<nc) return color[i];
  else     return i;
}

Int_t GetMarker(Int_t i) {
  const Int_t nc = 8;
  Int_t markerStyle[nc] = {20,21,33,34,24,25,27,28};
  if(i<nc) return markerStyle[i];
  else     return 20+i;
}
