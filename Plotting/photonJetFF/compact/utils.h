/*
 *   Some plotting utilities for ROOT
 *   By Marco van Leeuwen
 *
 */

#include <TColor.h>
#include <TPad.h>

#include <iostream>

const Float_t kELMASS = 0.00051099907;
const Float_t kPMASS  = 0.93827231;
const Float_t kKMASS  = 0.49367;
const Float_t kPIMASS = 0.13957;

const Char_t *label_str[3]={"40 #font[12]{A}#upointGeV",
			    "80 #font[12]{A}#upointGeV",
			    "158 #font[12]{A}#upointGeV"};

void grey_palette()
{
  TColor* col=(TColor*) gROOT->GetListOfColors()->At(2);
  col->SetRGB(0.8,0.8,0.8);
  col=(TColor*) gROOT->GetListOfColors()->At(3);
  col->SetRGB(0.6,0.6,0.6);
  Int_t palette[20];
  for (Int_t i=0;i<20; i++) {
    col=(TColor*) gROOT->GetListOfColors()->At(50+i);
    col->SetRGB(1-0.05*i,1-0.05*i,1-0.05*i);
    palette[i]=50+i;
  }
  gStyle->SetPalette(15,palette);
}

void draw_numbers(TCanvas *c1,Int_t n_pad) {
  TLatex ltx;
  ltx.SetNDC();
  ltx.SetTextSize(0.07);
  for (Int_t i=0; i< n_pad; i++) {
    c1->cd(0);
    TVirtualPad::Pad()->cd(i+1);
    Char_t num[10];
    sprintf(num,"%c)",97+i);
    std::cout << TVirtualPad::Pad() << " " << num << std::endl;
    ltx.DrawLatex(0.22,0.8,num);
  }
}

Float_t get_txt_size(TVirtualPad *pad, const Float_t pix_size) {
  if (pad->UtoPixel(1) > pad->VtoPixel(0)) {
    // 'Horizontal pad'
    return pix_size/pad->VtoPixel(0);
  }
  else {
    // 'Vertical pad'
    return pix_size/pad->UtoPixel(1);
  }
}

void set_text_sizes(TVirtualPad *pad=0x0, TH1F* hist=0x0, Float_t textSizeFactor=2.) {
  const Float_t label_size=9*textSizeFactor;
  const Float_t title_size=11*textSizeFactor;
  hist->SetLabelSize(get_txt_size(pad,label_size));
  hist->SetLabelSize(get_txt_size(pad,label_size),"Y");
  hist->SetTitleSize(get_txt_size(pad,title_size));
  hist->SetTitleSize(get_txt_size(pad,title_size),"Y");

  std::cout << "get_txt_size(pad,label_size) " << get_txt_size(pad,label_size) << std::endl;
}


void eraselabel(TPad *p,Double_t h)
{
   p->cd();
   TPad* pe = new TPad("pe","pe",0,0,p->GetLeftMargin(),h);
   pe->Draw(); 
   pe->SetFillColor(p->GetFillColor());  
   pe->SetBorderMode(0);
}


TCanvas *make_canvas(const Char_t *name, const Char_t *title, Int_t n_x=1, Int_t n_y=1, Int_t share_axes=0, Int_t ww=0, Int_t wh=0){
  const Int_t width=350;
  const Int_t height=350;
  TCanvas *canvas;
  if (share_axes==0) {
    if (ww==0)
      ww=width*n_x;
    if (wh==0)
      wh=height*n_y;
    canvas=new TCanvas(name,title,ww,wh);
    canvas->Divide(n_x,n_y,0,0);
  }
  else {
    Float_t pix_width=(1-gStyle->GetPadLeftMargin()-gStyle->GetPadRightMargin())*width;
    Float_t pix_height=(1-gStyle->GetPadTopMargin()-gStyle->GetPadBottomMargin())*height;
    if (ww==0)
      ww=width+(n_x-1)*pix_width;
    if (wh==0)
      wh=height+(n_y-1)*pix_height;

    canvas=new TCanvas(name,title,ww,wh);
    Float_t tot_width;
    if (n_x>1) 
      tot_width=(n_x-2)+1./(1-gStyle->GetPadLeftMargin())
	+1./(1-gStyle->GetPadRightMargin());
    else
      tot_width=1./(1-gStyle->GetPadLeftMargin()-gStyle->GetPadRightMargin());
    Float_t tot_height;
    if (n_y>1) 
      tot_height=(n_y-2)+1./(1-gStyle->GetPadTopMargin())
	+1./(1-gStyle->GetPadBottomMargin());
    else
      tot_height=1./(1-gStyle->GetPadTopMargin()-gStyle->GetPadBottomMargin());

    //Int_t idx=n_x*n_y;
    
    for (Int_t j=0; j<n_y; j++) {
      for (Int_t i=0; i<n_x; i++) {
	//for (Int_t j=n_y-1; j>=0; j--) {
	//for (Int_t i=n_x-1; i>=0; i--) {
	Char_t tmp_str[256];
	Char_t p_title[256];
	Int_t idx=n_x*j+i+1;
	sprintf(tmp_str,"%s_%d",canvas->GetName(),idx);
	sprintf(p_title,"Pad %d",idx);
	Float_t x1=0,y1=0;
	Float_t x2=1,y2=1;
	if (n_x>1) {
	  if (i==0) 
	    x2=1./(1-gStyle->GetPadLeftMargin())/tot_width;
	  else {
	    x1=(1./(1-gStyle->GetPadLeftMargin())+i-1)/tot_width;
	    if (i<n_x-1)
	      x2=(1./(1-gStyle->GetPadLeftMargin())+i)/tot_width;
	  }
	}
	if (n_y>1) {
	  if (j==0) 
	    y1=1-1./(1-gStyle->GetPadTopMargin())/tot_height;
	  else {
	    y2=1-(1./(1-gStyle->GetPadTopMargin())+j-1)/tot_height;
	    if (j<n_y-1)
	      y1=1-(1./(1-gStyle->GetPadTopMargin())+j)/tot_height;
	  }
	}
	std::cout << "j: " << j << ", i: " << i << std::endl;
	std::cout << "x1 " << x1 << ", x2 " << x2 << std::endl;
	std::cout << "y1 " << y1 << ", y2 " << y2 << std::endl;
	TPad *pad=new TPad(tmp_str,title,x1,y1,x2,y2);
	//pad->SetFillColor(idx+1);
	if (i>0)
	  pad->SetLeftMargin(0.001);
	if (i<n_x-1)
	  pad->SetRightMargin(0.001);
	if (j>0)
	  pad->SetTopMargin(0.001);
	if (j<n_y-1)
	  pad->SetBottomMargin(0.001);


	pad->SetNumber(idx);
	//pad->SetNumber(n_x*j+i+1);
	pad->Draw();
	//idx--;
	//idx++;
	//pad->SetP
      }
    }
  }

  return canvas;
}


TCanvas *make_canvas2(const Char_t *name, const Char_t *title, Int_t n_x=1, Int_t n_y=1, Int_t share_axes=0, Int_t ww=0, Int_t wh=0){
  const Int_t width=350;
  const Int_t height=350;
  TCanvas *canvas;
  if (share_axes==0) {
    if (ww==0)
      ww=width*n_x;
    if (wh==0)
      wh=height*n_y;
    canvas=new TCanvas(name,title,ww,wh);
    canvas->Divide(n_x,n_y,0,0);
  }
  else {
    Float_t pix_width=(1-2.*gStyle->GetPadLeftMargin()-2.*gStyle->GetPadRightMargin())*width;
    Float_t pix_height=(1-gStyle->GetPadTopMargin()-gStyle->GetPadBottomMargin())*height;
    if (ww==0)
      ww=width+(n_x-1)*pix_width;
    if (wh==0)
      wh=height+(n_y-1)*pix_height;

    canvas=new TCanvas(name,title,ww,wh);
    Float_t tot_width;
    if (n_x>1) 
      tot_width=(n_x-2)+1./(1-2.*gStyle->GetPadLeftMargin())
	+1./(1-2.*gStyle->GetPadRightMargin());
    else
      tot_width=1./(1-2.*gStyle->GetPadLeftMargin()-2.*gStyle->GetPadRightMargin());
    Float_t tot_height;
    if (n_y>1) 
      tot_height=(n_y-2)+1./(1-gStyle->GetPadTopMargin())
	+1./(1-gStyle->GetPadBottomMargin());
    else
      tot_height=1./(1-gStyle->GetPadTopMargin()-gStyle->GetPadBottomMargin());

    //Int_t idx=n_x*n_y;
    
    for (Int_t j=0; j<n_y; j++) {
      for (Int_t i=0; i<n_x; i++) {
	//for (Int_t j=n_y-1; j>=0; j--) {
	//for (Int_t i=n_x-1; i>=0; i--) {
	Char_t tmp_str[256];
	Char_t p_title[256];
	Int_t idx=n_x*j+i+1;
	sprintf(tmp_str,"%s_%d",canvas->GetName(),idx);
	sprintf(p_title,"Pad %d",idx);
	Float_t x1=0,y1=0;
	Float_t x2=1,y2=1;
	if (n_x>1) {
	  if (i==0) 
	    x2=1./(1-gStyle->GetPadLeftMargin()-gStyle->GetPadRightMargin())/tot_width;
	  else {
	    x1=(1./(1-gStyle->GetPadLeftMargin()-gStyle->GetPadRightMargin())+i-1)/tot_width;
	    if (i<n_x-1)
	      x2=(1./(1-gStyle->GetPadLeftMargin()-gStyle->GetPadRightMargin())+i)/tot_width;

	    // x1=(1./(1-gStyle->GetPadLeftMargin())+i-1)/tot_width;
	    // if (i<n_x-1)
	    //   x2=(1./(1-gStyle->GetPadLeftMargin())+i)/tot_width;
	  }
	}
	if (n_y>1) {
	  if (j==0) 
	    y1=1-1./(1-gStyle->GetPadTopMargin())/tot_height;
	  else {
	    y2=1-(1./(1-gStyle->GetPadTopMargin())+j-1)/tot_height;
	    if (j<n_y-1)
	      y1=1-(1./(1-gStyle->GetPadTopMargin())+j)/tot_height;
	  }
	}
	//std::cout << "x1 " << x1 << ", x2 " << x2 << std::endl;
	TPad *pad=new TPad(tmp_str,title,x1,y1,x2,y2);
	//pad->SetFillColor(idx+1);
	// if (i>0)
	//   pad->SetLeftMargin(0.001);
	// if (i<n_x-1)
	//   pad->SetRightMargin(0.001);
	if (j>0)
	  pad->SetTopMargin(0.001);
	if (j<n_y-1)
	  pad->SetBottomMargin(0.001);

	pad->SetNumber(idx);
	//pad->SetNumber(n_x*j+i+1);
	pad->Draw();
	//idx--;
	//idx++;
	//pad->SetP
      }
    }
  }

  return canvas;
}

/*
TPolyLine3D *draw_func_3d(TF1 *f1) {
  Double_t xmin,xmax;
  Bool_t logx=TVirtualPad::Pad()->GetLogx();
  Bool_t logy=TVirtualPad::Pad()->GetLogy();
  Float_t vmin[3],vmax[3];
  TView *view=TVirtualPad::Pad()->GetView();
  if (view==0){
    std::cout << "ERROR in draw_func_3d : no view found" << std::endl;
    return 0;
  }
  view->GetRange(vmin,vmax);
  Int_t npmax=f1->GetNpx();
  Float_t *xx=new Float_t[npmax];
  Float_t *yy=new Float_t[npmax];
  Float_t *zz=new Float_t[npmax];
  f1->GetRange(xmin,xmax);
  if (logx) {
    if (xmin<=0)
      xmin=vmin[0];
    else
      xmin=log10(xmin);
    xmax=log10(xmax);
  }  
  Float_t step=(xmax-xmin)/(npmax-1);
  Int_t np=0;
  for (Int_t i=0; i < npmax; i++) {
    Float_t x=xmin+step*i;
    Float_t y;
    if (logx)
      y=f1->Eval(pow(10,x));
    else
      y=f1->Eval(x);
    if (logy) {
      if (y<=0)
	y=vmin[1];
      else
	y=log10(y);
    }
    if (y>=vmin[1] && y<=vmax[1] &&
	x>=vmin[0] && x<=vmax[0]) {
      xx[np]=x;
      yy[np]=y;
      zz[np]=vmin[2];
      np++;
    }
  }
  TPolyLine3D *poly=new TPolyLine3D(np,xx,yy,zz);
  poly->SetLineStyle(f1->GetLineStyle());
  poly->SetLineColor(f1->GetLineColor());
  poly->Draw();
  delete [] xx;
  delete [] yy;
  delete [] zz;
  return poly;
}

TH2F *rebin_2d(TH2F *hist,Int_t nx=2,Int_t ny=2,const Char_t *name) { 
  
  Float_t xmin=hist->GetXaxis()->GetXmin();
  Float_t xmax=hist->GetXaxis()->GetXmax();
  Float_t ymin=hist->GetYaxis()->GetXmin();
  Float_t ymax=hist->GetYaxis()->GetXmax();
  
  Int_t nb_x=hist->GetXaxis()->GetNbins();
  nb_x=nb_x%nx?nb_x/nx+1:nb_x/nx;
  xmax=xmin+nb_x*nx*hist->GetXaxis()->GetBinWidth(1);
  Int_t nb_y=hist->GetYaxis()->GetNbins();
  nb_y=nb_x%ny?nb_y/ny+1:nb_y/ny;
  ymax=ymin+nb_y*ny*hist->GetYaxis()->GetBinWidth(1);
  
  TH2F *n_hist=new TH2F(name,hist->GetTitle(),nb_x,xmin,xmax,nb_y,ymin,ymax);

  for (Int_t bx=1; bx<=nb_x; bx++)
    for (Int_t by=1; by<=nb_y; by++) 
      n_hist->Fill(hist->GetXaxis()->GetBinCenter(bx),hist->GetYaxis()->GetBinCenter(by),hist->GetCellContent(bx,by));
   
  return n_hist;
}

TPad *overlay_2d(const Char_t *name="ovl_p",Char_t *title="Overlay pad"){
  TPad *ovl_p=new TPad(name,title,0,0,1,1);
  ovl_p->SetFillStyle(4000);
  ovl_p->RangeAxis(0,0,2,2);
  Float_t dxr=2./(1-TVirtualPad::Pad()->GetLeftMargin()-TVirtualPad::Pad()->GetRightMargin());
  Float_t dyr=2./(1-TVirtualPad::Pad()->GetTopMargin()-TVirtualPad::Pad()->GetBottomMargin());
  ovl_p->Range(-TVirtualPad::Pad()->GetLeftMargin()*dxr,-TVirtualPad::Pad()->GetBottomMargin()*dyr,
	       2+TVirtualPad::Pad()->GetRightMargin()*dxr,2+TVirtualPad::Pad()->GetTopMargin()*dyr);
  ovl_p->RangeAxis(0,0,2,2);
  ovl_p->Draw();
  ovl_p->cd();
  return ovl_p;
}

TGraph *draw_graph_error(TGraphErrors *gr) {
  Int_t np=gr->GetN();
  TGraph *tmp_gr=new TGraph(2*np);
  for (Int_t i=0; i<np; i++) {
    Double_t x,y;
    gr->GetPoint(i,x,y);
    tmp_gr->SetPoint(i,x,y+gr->GetErrorY(i));
    tmp_gr->SetPoint(2*np-i-1,x,y-gr->GetErrorY(i));
  }
  tmp_gr->Draw("f");
  tmp_gr->Draw("l");
  gr->Draw("pX");
  return tmp_gr;
}

TGraph *draw_twograph_area(TGraph *gr_lo, TGraph *gr_hi) {
  Int_t np_lo=gr_lo->GetN();
  Int_t np_hi=gr_hi->GetN();
  TGraph *tmp_gr=new TGraph(np_lo+np_hi);
  for (Int_t i=0; i<np_lo; i++) {
    Double_t x,y;
    gr_lo->GetPoint(i,x,y);
    tmp_gr->SetPoint(i,x,y);
  }
  for (Int_t i=0; i<np_hi; i++) {
    gr_hi->GetPoint(i,x,y);
    tmp_gr->SetPoint(np_lo+np_hi-i-1,x,y);
  }
  tmp_gr->Draw("f");
  //tmp_gr->Draw("l");
  //gr->Draw("pX");
  return tmp_gr;
}

void energy_label(Int_t i, Float_t x=0,Float_t y=0, TLatex *ltx=0) {
  if (ltx==0) {
    ltx=new TLatex();
    ltx->SetTextSize(18);
    ltx->SetTextAlign(21);
    ltx->SetNDC();
  }
  if (x==0) 
    x=gPad->GetLeftMargin()+0.5*(1-gPad->GetRightMargin()-gPad->GetLeftMargin());
  if (y==0) 
    y=1-gPad->GetTopMargin()-0.10;

  ltx->DrawLatex(x,y,label_str[i]);
}

Float_t ndc_x(Float_t x) {
  return (1-TVirtualPad::Pad()->GetLeftMargin()-TVirtualPad::Pad()->GetRightMargin())*x+TVirtualPad::Pad()->GetLeftMargin();
}

Float_t ndc_y(Float_t y) {
  return (1-TVirtualPad::Pad()->GetTopMargin()-TVirtualPad::Pad()->GetBottomMargin())*y+TVirtualPad::Pad()->GetBottomMargin();
}

void draw_fill_graph(TGraph *gr, Char_t *opt="p") {
  TGraph *gr_fill=(TGraph*) gr->Clone();
  gr_fill->SetMarkerStyle(gr->GetMarkerStyle()-4);
  gr_fill->SetMarkerColor(10);
  Char_t optx[25];
  sprintf(optx,"%sx",opt);
  gr_fill->Draw(optx);
  gr->Draw(opt);
}

void draw_legend_b(Float_t x, Float_t y, TAttLine *att_l, TAttFill *att_f, const Char_t *label, Int_t i_ent, Int_t colored=1, Size_t txt_size=16) {
  //y-=0.07*i_ent;
  Float_t pix_y_siz=TVirtualPad::Pad()->AbsPixeltoY(1)-TVirtualPad::Pad()->AbsPixeltoY(2);
  y-=pix_y_siz*txt_size*1.1*i_ent/(TVirtualPad::Pad()->GetUymax()-TVirtualPad::Pad()->GetUymin());

  Float_t x_min_real=TVirtualPad::Pad()->GetUxmin()+(x+0.01)*(TVirtualPad::Pad()->GetUxmax()-TVirtualPad::Pad()->GetUxmin());
  Float_t x_max_real=TVirtualPad::Pad()->GetUxmin()+(x+0.08)*(TVirtualPad::Pad()->GetUxmax()-TVirtualPad::Pad()->GetUxmin());
  Float_t y_min_real=TVirtualPad::Pad()->GetUymin()+y*(TVirtualPad::Pad()->GetUymax()-TVirtualPad::Pad()->GetUymin())-0.4*pix_y_siz*txt_size;
  Float_t y_max_real=TVirtualPad::Pad()->GetUymin()+y*(TVirtualPad::Pad()->GetUymax()-TVirtualPad::Pad()->GetUymin())+0.4*pix_y_siz*txt_size;
  if (att_f) {
    TBox *box_f=new TBox(x_min_real,y_min_real,x_max_real,y_max_real);
    box_f->SetFillStyle(att_f->GetFillStyle());
    if (colored!=0)
      box_f->SetFillColor(att_f->GetFillColor());
    box_f->Draw();
  }
  if (att_l) {
    TBox *box_l=new TBox(x_min_real,y_min_real,x_max_real,y_max_real);
    box_l->SetLineStyle(att_l->GetLineStyle());
    box_l->SetLineWidth(att_l->GetLineWidth());
    if (colored!=0)
      box_l->SetLineColor(att_l->GetLineColor());
    box_l->SetFillStyle(0);
    box_l->Draw();
  }
  TLatex *ltx=new TLatex();
  ltx->SetTextFont(133);
  ltx->SetTextSize(txt_size);
  ltx->SetTextAlign(12);
  if (colored==2)
    ltx->SetTextColor(att_m->GetMarkerColor());
  ltx->SetNDC();
  ltx->DrawLatex(ndc_x(x+0.1),ndc_y(y),label);
}

void draw_legend_b2(Float_t x, Float_t y, TAttLine *att_l, TAttFill *att_f, const Char_t *label, Int_t i_ent, Int_t colored=1, Size_t txt_size=16) {
  //y-=0.07*i_ent;
  Float_t pix_y_siz=TVirtualPad::Pad()->AbsPixeltoY(1)-TVirtualPad::Pad()->AbsPixeltoY(2);
  y-=pix_y_siz*txt_size*1.1*i_ent/(TVirtualPad::Pad()->GetUymax()-TVirtualPad::Pad()->GetUymin());

  Float_t x_min_real=TVirtualPad::Pad()->GetUxmin()+(x+0.01)*(TVirtualPad::Pad()->GetUxmax()-TVirtualPad::Pad()->GetUxmin());
  Float_t x_max_real=TVirtualPad::Pad()->GetUxmin()+(x+0.08)*(TVirtualPad::Pad()->GetUxmax()-TVirtualPad::Pad()->GetUxmin());
  Float_t y_min_real=TVirtualPad::Pad()->GetUymin()+y*(TVirtualPad::Pad()->GetUymax()-TVirtualPad::Pad()->GetUymin())-0.4*pix_y_siz*txt_size;
  Float_t y_max_real=TVirtualPad::Pad()->GetUymin()+y*(TVirtualPad::Pad()->GetUymax()-TVirtualPad::Pad()->GetUymin())+0.4*pix_y_siz*txt_size;
  if (att_f) {
    TBox *box_f=new TBox(x_min_real,y_min_real,x_max_real,y_max_real);
    box_f->SetFillStyle(att_f->GetFillStyle());
    if (colored!=0)
      box_f->SetFillColor(att_f->GetFillColor());
    box_f->Draw();
  }
  if (att_l) {
    TLine *box_l=new TLine(x_min_real,y_min_real,x_max_real,y_min_real);
    box_l->SetLineStyle(att_l->GetLineStyle());
    box_l->SetLineWidth(att_l->GetLineWidth());
    if (colored!=0)
      box_l->SetLineColor(att_l->GetLineColor());
    box_l->Draw();
    box_l->DrawLine(x_min_real,y_max_real,x_max_real,y_max_real);
  }
  TLatex *ltx=new TLatex();
  ltx->SetTextFont(133);
  ltx->SetTextSize(txt_size);
  ltx->SetTextAlign(12);
  if (colored==2)
    ltx->SetTextColor(att_m->GetMarkerColor());
  ltx->SetNDC();
  ltx->DrawLatex(ndc_x(x+0.1),ndc_y(y),label);
}

void draw_legend_l(Float_t x, Float_t y, TAttLine *att_l, const Char_t *label, Int_t i_ent, Int_t colored = 1, Size_t txt_size=16, Int_t txt_font=133) {
  //y-=0.07*i_ent;
  Float_t pix_y_siz=TVirtualPad::Pad()->AbsPixeltoY(1)-TVirtualPad::Pad()->AbsPixeltoY(2);
  y-=pix_y_siz*txt_size*1.1*i_ent/(TVirtualPad::Pad()->GetUymax()-TVirtualPad::Pad()->GetUymin());

  Float_t x_min_real=TVirtualPad::Pad()->GetUxmin()+(x+0.01)*(TVirtualPad::Pad()->GetUxmax()-TVirtualPad::Pad()->GetUxmin());
  Float_t x_max_real=TVirtualPad::Pad()->GetUxmin()+(x+0.08)*(TVirtualPad::Pad()->GetUxmax()-TVirtualPad::Pad()->GetUxmin());
  Float_t y_real=TVirtualPad::Pad()->GetUymin()+y*(TVirtualPad::Pad()->GetUymax()-TVirtualPad::Pad()->GetUymin());
  Int_t l_style=att_l->GetLineStyle();
  if (TVirtualPad::Pad()->GetLogx()) {
    x_min_real=exp(log(10)*x_min_real);
    x_max_real=exp(log(10)*x_max_real);
  }
  if (TVirtualPad::Pad()->GetLogy())
    y_real=exp(log(10)*y_real);
  TLine *line=new TLine(x_min_real,y_real,x_max_real,y_real);
  Float_t l_siz=att_l->GetLineWidth();
  line->SetLineWidth(l_siz);
  line->SetLineStyle(l_style);
  line->SetLineColor(att_l->GetLineColor());
  line->Draw();
  TLatex *ltx=new TLatex();
  ltx->SetTextFont(txt_font);
  ltx->SetTextSize(txt_size);
  ltx->SetTextAlign(12);
  if (colored)
    ltx->SetTextColor(att_l->GetLineColor());
  ltx->SetNDC();
  ltx->DrawLatex(ndc_x(x+0.1),ndc_y(y),label);
}

void draw_legend_m(Float_t x, Float_t y, TAttMarker *att_m, const Char_t *label, Float_t i_ent, Int_t colored=1, Size_t txt_size=16, Int_t txt_font=133) {
  //y-=0.07*i_ent;
  Float_t pix_y_siz=TVirtualPad::Pad()->AbsPixeltoY(1)-TVirtualPad::Pad()->AbsPixeltoY(2);
  y-=pix_y_siz*txt_size*1.1*i_ent/(TVirtualPad::Pad()->GetUymax()-TVirtualPad::Pad()->GetUymin());

  Float_t x_real=TVirtualPad::Pad()->GetUxmin()+(x+0.035)*(TVirtualPad::Pad()->GetUxmax()-TVirtualPad::Pad()->GetUxmin());
  Float_t y_real=TVirtualPad::Pad()->GetUymin()+y*(TVirtualPad::Pad()->GetUymax()-TVirtualPad::Pad()->GetUymin());
  Int_t m_style=att_m->GetMarkerStyle();
  if (TVirtualPad::Pad()->GetLogy())
    y_real=exp(log(10)*y_real);
  if (TVirtualPad::Pad()->GetLogx())
    x_real=exp(log(10)*x_real);
  // std::cout << "uymin " << TVirtualPad::Pad()->GetUymin() << " max "
  //     << TVirtualPad::Pad()->GetUymax() << " y " << y << " y_real " << y_real << std::endl;
  TMarker *mrk=new TMarker(x_real,y_real,m_style);
  Float_t m_siz=att_m->GetMarkerSize();
  mrk->SetMarkerSize(m_siz);
  if (colored!=0)
    mrk->SetMarkerColor(att_m->GetMarkerColor());
  mrk->Draw();
  TLatex *ltx=new TLatex();
  ltx->SetTextFont(txt_font);
  ltx->SetTextSize(txt_size);
  ltx->SetTextAlign(12);
  if (colored==2)
    ltx->SetTextColor(att_m->GetMarkerColor());
  ltx->SetNDC();
  ltx->DrawLatex(ndc_x(x+0.08),ndc_y(y),label);
}

void scale_graph(TGraphErrors *gr,Float_t scale) {
  for (Int_t i=0; i<gr->GetN(); i++) {
    Double_t x,y;
    gr->GetPoint(i,x,y);
    gr->SetPoint(i,x,scale*y);
    gr->SetPointError(i,gr->GetErrorX(i),scale*gr->GetErrorY(i));
  }
}

void scale_graph(TGraph *gr,Float_t scale) {
  for (Int_t i=0; i<gr->GetN(); i++) {
    Double_t x,y;
    gr->GetPoint(i,x,y);
    gr->SetPoint(i,x,scale*y);
  }
}

void set_ratio(TGraphErrors *gr, Int_t pnt, Float_t x_val, Float_t num, Float_t den, Float_t num_err=0, Float_t den_err=0) {
  Float_t rat=num/den;
  gr->SetPoint(i,x_val,rat);
  if (rat!=0) {
    Float_t rat_err=rat*sqrt(num_err*num_err/num/num+den_err*den_err/den/den);
    gr->SetPointError(i,0,rat_err);
  }
}

void set_chi_levels(TH2 *h1, const Int_t n_cont, Double_t *c_levels, Double_t *x_min=0, Double_t *y_min=0) {

  Float_t min_chi=1e10;
  Int_t min_xbin=0, min_ybin=0;
  
  for (Int_t i=1; i <= h1->GetNbinsX(); i++) {
    for (Int_t j=1; j <= h1->GetNbinsY(); j++) {
      if (h1->GetCellContent(i,j) < min_chi) {
	min_xbin=i;
	min_ybin=j;
	min_chi=h1->GetCellContent(i,j);
      }
    }
  }
  
  std::cout << "min_xbin " << min_xbin << ", " << min_ybin << ", min_chi " << min_chi << std::endl;

  Double_t *c_levels_tmp=new Double_t[n_cont];
  for (Int_t i=0; i<n_cont;i++) {
    c_levels_tmp[i]=c_levels[i]+min_chi;
    std::cout << "c_levels[" <<i << "] " << c_levels[i] << ", tmp " << c_levels_tmp[i] << std::endl;
  }
  h1->SetContour(n_cont,c_levels_tmp);
  //delete c_levels_tmp;
  
  if (x_min) 
    *x_min=h1->GetXaxis()->GetBinCenter(min_xbin);
  if (y_min) 
    *y_min=h1->GetYaxis()->GetBinCenter(min_ybin);

}


void make_gr_file(const Char_t *name,TGraphErrors *gr,const Char_t mode='w') {
  //ios::open_mode m_flag=ios::out|ios::trunc;
  int m_flag=ios_base::out|ios_base::trunc;
  if (mode=='a')
    m_flag=ios_base::out|ios_base::app;
  //  m_flag=ios::out|ios::app;
  ofstream fout(name,m_flag);
  for (Int_t i=0; i< gr->GetN(); i++) {
    Double_t x,y;
    gr->GetPoint(i,x,y);
    fout << x << " " << y;
    fout << " " << gr->GetErrorY(i)<< std::endl;
  }
  fout.close();
}

// from Numerical recipes
// n is number of points, i.e. order of polynomial+1
void polint(const Double_t xa[], const Double_t ya[], const Int_t n, const Float_t x, Float_t &y, Float_t &dy) {
  Int_t i,m;
  Int_t ns=0;
  Float_t den,dif,dift,ho,hp,w;
  Float_t *c, *d;

  dif=fabs(x-xa[0]);
  c=new Float_t[n];
  d=new Float_t[n];
  for (i=0; i<n; i++) {
    if ( (dift=fabs(x-xa[i])) < dif) {
      ns=i;
      dif=dift;
    }
    // Initialise 'tableau'
    c[i]=ya[i];
    d[i]=ya[i];
  }
  //std::cout << "x_int " << x_int << ", ns " << ns << ", dif " << dif << std::endl;
  y=ya[ns--];
  //std::cout << "y " << y << std::endl;
  for (m=1; m < n; m++) {
    for (i=0; i < n-m; i++) {
      //std::cout << "i " << i << ", m " << m << std::endl;
      ho=xa[i]-x;
      hp=xa[i+m]-x;
      w=c[i+1]-d[i];
      if ( (den=ho-hp) == 0) 
	std::cout << "ERROR in polint" << std::endl;  // Error occurs if two xa are equal
      den=w/den;
      d[i]=hp*den;
      c[i]=ho*den;
    }
    y+=(dy=(2*ns < (n-m-1)?c[ns+1]:d[ns--]));
  }
  delete [] c;
  delete [] d;
} 
*/

/*
Float_t interpolate(TGraph *gr, Float_t x) {
  Int_t ip = 0;
  Double_t x2, y2;
  gr->GetPoint(ip,x2,y2);
  while (ip < gr->GetN() && x > x2) {
    ip++;
    gr->GetPoint(ip,x2,y2);
  }
  if (ip == 0 || ip == gr->GetN())
    return 0;
  Double_t x1, y1;
  gr->GetPoint(ip-1,x1,y1);
  return ((x-x1)*y2 + (x2-x)*y1) / (x2-x1);
}

Float_t integr_hist(TH1 *h1, Int_t min_bin, Int_t max_bin, Float_t &err) {
  Float_t integr=0;
  err=0;
  for (Int_t bin = min_bin; bin <= max_bin; bin++) {
    integr += h1->GetBinContent(bin);
    err += h1->GetBinError(bin)*h1->GetBinError(bin);
  }
  err=sqrt(err);
  return integr;
}

TGraphErrors *subtract_graph(TGraphErrors *gr, TF1* f1) {
  Int_t n=gr->GetN();
  TGraphErrors *diff_gr=new TGraphErrors(n);
  for (Int_t i=0; i<n; i++) {
    Double_t x,y;
    gr->GetPoint(i,x,y);
    Double_t y_fun=f1->Eval(x);
    if ( y_fun != 0) {
      diff_gr->SetPoint(i,x,y-y_fun);
      diff_gr->SetPointError(i,gr->GetErrorX(i),gr->GetErrorY(i));
    }
  }
  return diff_gr;
}

TGraphAsymmErrors *subtract_graph(TGraphAsymmErrors *gr, TF1* f1) {
  Int_t n=gr->GetN();
  TGraphAsymmErrors *diff_gr=new TGraphAsymmErrors(n);
  for (Int_t i=0; i<n; i++) {
    Double_t x,y;
    gr->GetPoint(i,x,y);
    Double_t y_fun=f1->Eval(x);
    if ( y_fun != 0) {
      diff_gr->SetPoint(i,x,y-y_fun);
      diff_gr->SetPointError(i,gr->GetErrorXlow(i),gr->GetErrorXhigh(i),gr->GetErrorYlow(i),gr->GetErrorYhigh(i));
    }
  }
  return diff_gr;
}

TGraphErrors *divide_graph(TGraphErrors *gr, TF1* f1) {
  Int_t n=gr->GetN();
  TGraphErrors *rat_gr=new TGraphErrors(n);
  for (Int_t i=0; i<n; i++) {
    Double_t x,y;
    gr->GetPoint(i,x,y);
    Double_t y_fun=f1->Eval(x);
    if ( y_fun != 0) {
      rat_gr->SetPoint(i,x,y/y_fun);
      rat_gr->SetPointError(i,gr->GetErrorX(i),gr->GetErrorY(i)/y_fun);
    }
  }
  return rat_gr;
}

TGraph *divide_graph(TGraph *gr, TF1* f1) {
  Int_t n=gr->GetN();
  TGraph *rat_gr=new TGraph(n);
  for (Int_t i=0; i<n; i++) {
    Double_t x,y;
    gr->GetPoint(i,x,y);
    Double_t y_fun=f1->Eval(x);
    if ( y_fun != 0) {
      rat_gr->SetPoint(i,x,y/y_fun);
    }
  }
  return rat_gr;
}

TGraphErrors *divide_graph(TGraphAsymmErrors *num, TGraphAsymmErrors *denom) {
  std::cout << "WARNING: not taking asymmetric errors into account" << std::endl;
  Int_t n=num->GetN();
  TGraphErrors *rat_gr=new TGraphErrors(n);
  for (Int_t i=0; i<n; i++) {
    Double_t x1,y1;
    num->GetPoint(i,x1,y1);
    Double_t x2,y2;
    denom->GetPoint(i,x2,y2);
    if (y2 != 0) {
      rat_gr->SetPoint(i,x1,y1/y2);
      Float_t num_err=num->GetErrorY(i);
      Float_t denom_err=denom->GetErrorY(i);
      if (y1 != 0) 
	rat_gr->SetPointError(i,0,sqrt(num_err*num_err/y1/y1+denom_err*denom_err/y2/y2)*y1/y2);
    }
  }
  return rat_gr;
}

TGraphErrors *divide_graph(TGraphErrors *num, TGraphErrors *denom, Int_t err_flag=1) {
  // err_flag==0 means no errors on denominator
  Int_t n=num->GetN();
  TGraphErrors *rat_gr=new TGraphErrors(n);
  for (Int_t i=0; i<n; i++) {
    Double_t x1,y1;
    num->GetPoint(i,x1,y1);
    Double_t x2,y2;
    denom->GetPoint(i,x2,y2);
    if (y2 != 0) {
      rat_gr->SetPoint(i,x1,y1/y2);
      Float_t num_err=num->GetErrorY(i);
      Float_t denom_err=denom->GetErrorY(i);
      if (y1 != 0) {
	if (err_flag)
	  rat_gr->SetPointError(i,0,sqrt(num_err*num_err/y1/y1+denom_err*denom_err/y2/y2)*y1/y2);
	else
	  rat_gr->SetPointError(i,0,num_err/y2);
      }
    }
  }
  rat_gr->SetMarkerStyle(num->GetMarkerStyle());  
  rat_gr->SetMarkerColor(num->GetMarkerColor());
  rat_gr->SetLineColor(num->GetLineColor());
  return rat_gr;
}

TGraphErrors *diff_gr(TGraphErrors *gr1, TGraph *gr2, Int_t err_flag=0, Float_t delta=0.001) {
  // err_flag==0 means no errors on second graph
  Int_t n=gr1->GetN();
  TGraphErrors *diff_gr=new TGraphErrors();
  Int_t i_point=0;
  Int_t i2=0;
  for (Int_t i1=0; i1<n; i1++) {
    Double_t x1,y1;
    gr1->GetPoint(i1,x1,y1);
    Double_t x2,y2;
    gr2->GetPoint(i2,x2,y2);
    while (i2 < gr2->GetN() && fabs(x2-x1) > delta*x1 && x2*(1+delta)<x1) {
      i2++;
      gr2->GetPoint(i2,x2,y2);
    }
    if (fabs(x2-x1) < delta*x1) {
      diff_gr->SetPoint(i_point,x1,y1-y2);
      Float_t gr1_err=gr1->GetErrorY(i1);
      Float_t gr2_err=gr2->GetErrorY(i2);
      if (err_flag)
	diff_gr->SetPointError(i_point,0,sqrt(gr1_err*gr1_err+gr2_err*gr2_err));
      else
	diff_gr->SetPointError(i_point,0,gr1_err);
      i_point++;
    }
  }
  return diff_gr;
}


TGraphErrors *merge_graphs(TGraphErrors *gr1, TGraphErrors *gr2) {
  TGraphErrors *gr_sum = new TGraphErrors();
  
  Int_t np=0;
  for (Int_t i=0; i < gr1->GetN(); i++) {
    Double_t x,y;
    gr1->GetPoint(i,x,y);
    gr_sum->SetPoint(np,x,y);
    gr_sum->SetPointError(np,gr1->GetErrorX(i),gr1->GetErrorY(i));
    np++;
  }

  for (Int_t i=0; i < gr2->GetN(); i++) {
    Double_t x,y;
    gr2->GetPoint(i,x,y);
    gr_sum->SetPoint(np,x,y);
    gr_sum->SetPointError(np,gr2->GetErrorX(i),gr2->GetErrorY(i));
    np++;
  }
 
  return gr_sum;
}

TGraph *add_graphs(TGraph *gr1, TGraph *gr2) {
  TGraph *gr_sum = new TGraph();
  
  Int_t np=0;
  for (Int_t i=0; i < gr1->GetN(); i++) {
    Double_t x,y;
    gr1->GetPoint(i,x,y);
    gr_sum->SetPoint(np,x,y);
    np++;
  }

  np = 0;
  Double_t x1, y1;
  gr_sum->GetPoint(np,x1,y1);
  for (Int_t i=0; i < gr2->GetN(); i++) {
    Double_t x2,y2;
    gr2->GetPoint(i,x2,y2);
    while ( np < gr_sum->GetN() && x1 < x2 && fabs(x1-x2)/x2 > 1e-3) {
      np++;
      gr_sum->GetPoint(np,x1,y1);
    }
    if (fabs(x1-x2)/x2 < 1e-3) {
      gr_sum->SetPoint(np,x1,y1+y2);
    }
  }
 
  return gr_sum;
}

TGraphErrors *add_graphs(TGraphErrors *gr1, TGraphErrors *gr2) {
  TGraphErrors *gr_sum = new TGraphErrors();
  
  Int_t np=0;
  for (Int_t i=0; i < gr1->GetN(); i++) {
    Double_t x,y;
    gr1->GetPoint(i,x,y);
    gr_sum->SetPoint(np,x,y);
    gr_sum->SetPointError(np,gr1->GetErrorX(i),gr1->GetErrorY(i));
    np++;
  }

  np = 0;
  Double_t x1, y1;
  gr_sum->GetPoint(np,x1,y1);
  for (Int_t i=0; i < gr2->GetN(); i++) {
    Double_t x2,y2;
    gr2->GetPoint(i,x2,y2);
    while ( np < gr_sum->GetN() && x1 < x2 && fabs(x1-x2)/x2 > 1e-3) {
      np++;
      gr_sum->GetPoint(np,x1,y1);
    }
    if (fabs(x1-x2)/x2 < 1e-3) {
      gr_sum->SetPoint(np,x1,y1+y2);
      gr_sum->SetPointError(np,gr_sum->GetErrorX(np),sqrt(gr2->GetErrorY(i)*gr2->GetErrorY(i)+gr_sum->GetErrorY(np)*gr_sum->GetErrorY(np)));
    }
  }
 
  return gr_sum;
}

TGraphAsymmErrors *add_graphs(TGraphAsymmErrors *gr1, TGraphAsymmErrors *gr2) {
  TGraphAsymmErrors *gr_sum = new TGraphAsymmErrors();
  
  Int_t np=0;
  for (Int_t i=0; i < gr1->GetN(); i++) {
    Double_t x,y;
    gr1->GetPoint(i,x,y);
    gr_sum->SetPoint(np,x,y);
    gr_sum->SetPointError(np,gr1->GetErrorXlow(i),gr1->GetErrorXhigh(i),gr1->GetErrorYlow(i),gr1->GetErrorYhigh(i));
    np++;
  }

  np = 0;
  Double_t x1, y1;
  gr_sum->GetPoint(np,x1,y1);
  for (Int_t i=0; i < gr2->GetN(); i++) {
    Double_t x2,y2;
    gr2->GetPoint(i,x2,y2);
    while ( np < gr_sum->GetN() && x1 < x2 && fabs(x1-x2)/x2 > 1e-3) {
      np++;
      gr_sum->GetPoint(np,x1,y1);
    }
    if (fabs(x1-x2)/x2 < 1e-3) {
      gr_sum->SetPoint(np,x1,y1+y2);
      gr_sum->SetPointError(np,gr_sum->GetErrorXlow(np),gr_sum->GetErrorXhigh(np),
			    sqrt(gr2->GetErrorYlow(i)*gr2->GetErrorYlow(i)+gr_sum->GetErrorYlow(np)*gr_sum->GetErrorYlow(np)),
			    sqrt(gr2->GetErrorYhigh(i)*gr2->GetErrorYhigh(i)+gr_sum->GetErrorYhigh(np)*gr_sum->GetErrorYhigh(np)));
    }
  }
 
  return gr_sum;
}


TGraphErrors *make_sys_gr(TH1*low_h, TH1*high_h) {
  // Make TGraphErrors out of two bounding histos
  // x-err is set to half the bin-width to enable grey-box-drawing

  TGraphErrors *sys_gr = new TGraphErrors();
  for (Int_t i_bin = 0; i_bin < low_h->GetNbinsX(); i_bin++) {
    Float_t low = low_h->GetBinContent(i_bin+1);
    Float_t high = high_h->GetBinContent(i_bin+1);
    sys_gr->SetPoint(i_bin, low_h->GetBinCenter(i_bin+1), 0.5*(low + high));
    sys_gr->SetPointError(i_bin, 0.5*low_h->GetBinWidth(i_bin+1), 0.5*fabs(high - low));
  }
  return sys_gr;
}

TGraphErrors *make_graph(TH1*h1) {
  // Make a TGraphErrors from a TH1
  TGraphErrors *gr = new TGraphErrors(h1->GetNbinsX());
  
  for (Int_t i=0; i < h1->GetNbinsX(); i++) {
    gr->SetPoint(i,h1->GetBinCenter(i+1),h1->GetBinContent(i+1));
    gr->SetPointError(i,0,h1->GetBinError(i+1));
  }
  return gr;
}

TGraph *divide_graphs_int(TGraph *num, TGraph *denom) {
  Int_t j = 0;
  Double_t x1,y1,x2,y2, y_int;
  denom->GetPoint(j,x1,y1);
  j++;
  denom->GetPoint(j,x2,y2);
  j++;
  TGraph *gr = new TGraph();
  for (Int_t i = 0; i < num->GetN(); i++) {
    Double_t x,y;
    num->GetPoint(i,x,y);
    while (x2 < x && j < denom->GetN()) {
      x1 = x2; y1 = y2;
      denom->GetPoint(j,x2,y2);
      j++;
    }
    if (x <= x1+(x2-x1)*1.05 && x > x1) {  // allow small tolerance for last point
      y_int = (x2-x)/(x2-x1)*y1 + (x-x1)/(x2-x1)*y2;
      //std::cout << "x " << x << " x1 " << x1 << " x2 " << x2 << " y " << y << " y_int " << y_int << std::endl;
      gr->SetPoint(gr->GetN(),x,y/y_int);
    }
  }
  return gr;
}

TGraphErrors *divide_graphs_int(TGraphErrors *num, TGraphErrors *denom) {
  Int_t j = 0;
  Double_t x1,y1,x2,y2, y_int;
  Float_t y_err1, y_err2;
  denom->GetPoint(j,x1,y1);
  y_err1 = denom->GetErrorY(j);
  j++;
  denom->GetPoint(j,x2,y2);
  y_err2 = denom->GetErrorY(j);
  j++;
  TGraphErrors *gr = new TGraphErrors();
  for (Int_t i = 0; i < num->GetN(); i++) {
    Double_t x,y;
    num->GetPoint(i,x,y);
    Float_t y_err = num->GetErrorY(i);
    while (x2 < x && j < denom->GetN()) {
      x1 = x2; y1 = y2; y_err1 = y_err2;
      denom->GetPoint(j,x2,y2);
      y_err2 = denom->GetErrorY(j);
      j++;
    }
    if (x <= x1+(x2-x1)*1.05 && x > x1) {  // allow small tolerance for last point   
      y_int = (x2-x)/(x2-x1)*y1 + (x-x1)/(x2-x1)*y2;
      std::cout << "i " << i << " x " << x << " y " << y << " x1 " << x1 << " x2 " << x2 << " y1 " << y1 << " y2 " << y2 << " y_int " << y_int << std::endl;
      Float_t y_int_err = sqrt(pow((x2-x)/(x2-x1)*y_err1,2)+pow((x-x1)/(x2-x1)*y_err2,2));
      std::cout << "y_int " << y_int << " y_int_err " << y_int_err << " y " << y << " y_err " << y_err << std::endl;
      gr->SetPoint(gr->GetN(),x,y/y_int);
      gr->SetPointError(gr->GetN()-1,0,sqrt(pow(y_err/y,2)+pow(y_int_err/y_int,2))*y/y_int);
    }
  }
  return gr;
}
*/
