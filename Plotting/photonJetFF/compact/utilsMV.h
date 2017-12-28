#include "TFile.h"
#include "TF1.h"
#include "TRandom3.h"
#include "TH1.h"
#include "TLegend.h"
#include "TH2.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TLatex.h"
#include "Riostream.h"
#include "TMath.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TMath.h"
#include <vector>

double Median(const TH1D * h1) { 

   int n = h1->GetXaxis()->GetNbins();  
   std::vector<double>  x(n);
   h1->GetXaxis()->GetCenter( &x[0] );
   const double * y = h1->GetArray(); 
   // exclude underflow/overflows from bin content array y
   return TMath::Median(n, &x[0], &y[1]); 
}

Double_t interpolate(TGraph *gr, Double_t x) {
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

TH1F *GraphtoHisto(TGraph *gr)
{
  TH1F *h = new TH1F();

  Int_t N = gr->GetN();
  double x,y;
  double xmax,xmin;
  gr->GetPoint(0,xmin,y);
  gr->GetPoint(N-1,xmax,y);
  double dx = (xmax-xmin)/((double)N);

  
  h = new TH1F(gr->GetName(),gr->GetTitle(),N,xmin,xmax);
  h->SetXTitle(gr->GetXaxis()->GetTitle());
  h->SetYTitle(gr->GetYaxis()->GetTitle());
  for(int i =0; i<N; i++)
    {
      gr->GetPoint(i,x,y);
      int binx = (int)((x-xmin)/dx);
      h->SetBinContent(binx,y);
    }

  return h;
}

TGraphErrors *HistoToGraph(TH1 *h1 = 0x0, Double_t xmin = -1.) {

  TGraphErrors *gr1 = new TGraphErrors();
  for(int i=1; i<=h1->GetNbinsX(); i++) {

    if(xmin>0 && h1->GetXaxis()->GetBinCenter(i)<xmin) continue;

    gr1->SetPoint(gr1->GetN(), h1->GetXaxis()->GetBinCenter(i), h1->GetBinContent(i));
    gr1->SetPointError(gr1->GetN()-1, h1->GetXaxis()->GetBinWidth(i)*0.5,h1->GetBinError(i));

  }

  return gr1;

}

// double integrate_graph(TGraph *gr)
// {
//   double integ = 0.;
//   Int_t N = gr->GetN();
//   double x,y,xold,xnext,ynext;
//   double xmax,xmin;
//   gr->GetPoint(0,xmin,y);
//   gr->GetPoint(N-1,xmax,y);
//   xold = xmin;
//   double dx = (xmax-xmin)/((double)N);
//   for(int i =0; i<N; i++)
//     {
//       gr->GetPoint(i,x,y);
//       gr->GetPoint(i+1,xnext,ynext);
//       dx = (0.5*(xnext-x)+x)  - (0.5*(x-xold)+xold);
//       integ += dx*y;
//       xold=x;
//     }

//   return integ;
// }

Double_t integrate_graph(TGraph *gr) 
{
  //integrate graph dN/dx
  Double_t integr=0.; 
  Double_t xold, yold;
  gr->GetPoint(0,xold,yold);
  for (Int_t i = 1; i < gr->GetN(); i++) 
    {
      Double_t x, y;
      gr->GetPoint(i,x,y);
      integr += 0.5*(yold+y)*TMath::Abs((x-xold));
      xold = x;
      yold = y;
    }
  return integr;
} 

Double_t integrate_graph(TGraph *gr,float ymax) 
{
  //integrate graph dN/dx
  Double_t integr=0.; 
  Double_t xold, yold;
  gr->GetPoint(0,xold,yold);
  for (Int_t i = 1; i < gr->GetN(); i++) 
    {
      Double_t x, y;
      gr->GetPoint(i,x,y);
      if(y>ymax) continue;
      integr += 0.5*(yold+y)*TMath::Abs((x-xold));
      xold = x;
      yold = y;
    }
  return integr;
} 

Double_t integrate_graphX(TGraph *gr) 
{
  //integrate graph xdN/dx
  Double_t integr=0.; 
  Double_t xold, yold;
  gr->GetPoint(0,xold,yold);
  for (Int_t i = 1; i < gr->GetN(); i++) 
    {
      Double_t x, y;
      gr->GetPoint(i,x,y);
      integr += 0.5*(xold*yold+x*y)*TMath::Abs((x-xold));
      xold = x;
      yold = y;
    }
  return integr;
} 

Double_t meanX_graph(TGraph *gr, double p0=0.)
{
  Double_t integr=0.; 
  Double_t xold, yold;
  Double_t sumy = 0.;
  gr->GetPoint(0,xold,yold);
  for (Int_t i = 1; i < gr->GetN(); i++) 
    {
      Double_t x, y;
      gr->GetPoint(i,x,y);
      integr += x*y;
      sumy += y;
    }
  sumy+=p0;
  Double_t meanX = 1./sumy * integr;

  return meanX;
}

Double_t calc_Rn(TGraph *grOutgoingSpectrum,Float_t p0,Float_t Abs, Float_t pow_n)
{
  double Rn = (double)p0;
  double x,y,xold,yold;
  grOutgoingSpectrum->GetPoint(0,xold,yold);
  for(int i=1; i<grOutgoingSpectrum->GetN(); i++)
    {
      grOutgoingSpectrum->GetPoint(i,x,y);
      Rn+=TMath::Power(x,pow_n-1.)*y*TMath::Abs(xold-x);//0.5*(yold+)
      xold=x;
    }
  //Rn+=(double)Abs;
  return Rn;
}

TGraph *GetOutgoingSpectrum(TGraph *grPdE, Double_t EParton)
{
  TGraph *grOutgoingSpectrum = new TGraph();
  double x,y;
  for(int i=0; i<grPdE->GetN(); i++)
    {
      grPdE->GetPoint(i,x,y);
      if((EParton-x)/EParton>=0.) grOutgoingSpectrum->SetPoint(i,(EParton-x)/EParton,y*EParton);
    }
  return grOutgoingSpectrum;
}

Double_t calc_Rn_FromPdE1E(TH1F *hPdE1E=0,Double_t p0=0,Double_t Abs=0, Float_t pow_n=7., Bool_t width=kFALSE)
{
  double Rn = (double)p0;
  double x,y;
  for(int i=1; i<=hPdE1E->GetNbinsX(); i++)
    {
      x = (double)(hPdE1E->GetXaxis()->GetBinCenter(i));
      y = (double)(hPdE1E->GetBinContent(i));
      if(width)       Rn+=TMath::Power(x,pow_n-1.)*y*(double)(hPdE1E->GetXaxis()->GetBinWidth(i));
      else Rn+=TMath::Power(x,pow_n-1.)*y;
    }
  return Rn;
}


Float_t calc_R(TGraph *rad_gr = 0, Float_t p0 = 0., Float_t pow_n = 8) {
  // Calculate R = int dx (1-x)^n P(x)
  // should be representative of RAA

  Double_t mean_dE = 0;
  Double_t sum_w = 0;

  //Double_t dum, p0, p1;
  //edge_gr->GetPoint(0,dum,p0);
  //edge_gr->GetPoint(1,dum,p1);
  mean_dE += p0;
  sum_w = p0;

  cout << "R from dE=0 " << mean_dE << endl;
  Double_t old_x, x;
  Double_t old_p, p;
  rad_gr->GetPoint(0,old_x,old_p);
  for (Int_t i = 1; i < rad_gr->GetN(); i++) {
    rad_gr->GetPoint(i,x,p);
    Double_t pow_x = pow(1-x,pow_n);
    sum_w += old_p*(x-old_x);
    mean_dE += old_p*(x-old_x)*pow_x;
    if (old_x < 0.1 && x > 0.1)
      cout << "R up to dE/E = " << x << "  : " << mean_dE << endl;
    old_x = x;
    old_p = p;
  }
  cout << "sum_w " << sum_w << endl;
  return mean_dE;
}

Double_t calc_mean_de(TGraph *rad_gr = 0, Double_t p0 = 0.,Double_t Emax = 1e6) {
  Double_t mean_dE = 0.;
  Double_t sum_w = 0.;

//   Double_t dum, p0, p1;
//   edge_gr->GetPoint(0,dum,p0);
//   edge_gr->GetPoint(1,dum,p1);
//   mean_dE += p1;
//   sum_w = p0+p1;
  sum_w = p0;
  //cout << "Discrete part: mean_dE " << mean_dE << " p " << sum_w << endl;

  Double_t old_dE, dE;
  Double_t old_p, p;
  int j = 0;
  rad_gr->GetPoint(0,old_dE,old_p);
  //   Double_t Emax, dum;
  //   rad_gr->GetPoint(rad_gr->GetN(),Emax,dum);
  for (Int_t i = 0; i < rad_gr->GetN(); i++) {
    rad_gr->GetPoint(i,dE,p);
    //    if(dE>Emax && Emax!=1e6) continue;
    //    cout << "p: " << p << "\tsum_w: " << sum_w << endl;
    if(dE>=Emax && j==0)
      {
	p = 1.-sum_w;
	cout << "p dE>" << Emax << ": " << p << "\tsum_w: " << sum_w <<endl;
	j++;
      }
    if(dE>Emax && j>0) continue;
    sum_w += p*(dE-old_dE);
    mean_dE += p*(dE-old_dE)*dE;
    //cout << "ddE " << dE-old_dE << " dsumw " << old_p*(dE-old_dE) << " sum_w " << sum_w << endl;
    old_dE = dE;
    old_p = p;
  }
  //  cout << "mean_dE " << mean_dE << " sum_w " << sum_w << endl;
  return mean_dE/sum_w;
}

Double_t calc_mean_de(TH1F *hrad = 0, Double_t p0 = 0.,Double_t Emax = 1e6) {
  Double_t mean_dE = 0.;
  Double_t sum_w = 0.;

//   Double_t dum, p0, p1;
//   edge_gr->GetPoint(0,dum,p0);
//   edge_gr->GetPoint(1,dum,p1);
//   mean_dE += p1;
//   sum_w = p0+p1;
  sum_w = p0;
  //cout << "Discrete part: mean_dE " << mean_dE << " p " << sum_w << endl;

  int j = 0;
  Double_t old_dE, dE;
  Double_t old_p, p;
  old_dE = hrad->GetBinCenter(0);
  old_p=hrad->GetBinContent(0);
  //double Emax = hrad->GetBinCenter( hrad->GetNbinsX());
  for (Int_t i = 0; i < hrad->GetNbinsX(); i++) {
    dE = (double)hrad->GetBinCenter(i);
    //    if(dE>Emax && Emax!=1e6) continue;
    p=hrad->GetBinContent(i);
    if(dE>=Emax && j==0)
      {
	p = 1.-sum_w;
	cout << "p dE>" << Emax << ": " << p << "\tsum_w: " << sum_w << endl;
	j++;
      }
    if(dE>Emax && j>0) continue;
    //     sum_w += old_p*(dE-old_dE);
    //     mean_dE += old_p*(dE-old_dE)*dE;
    sum_w += p*(dE-old_dE);
    mean_dE += p*(dE-old_dE)*dE;
    //cout << "ddE " << dE-old_dE << " dsumw " << old_p*(dE-old_dE) << " sum_w " << sum_w << endl;
    old_dE = dE;
    old_p = p;
  }
  //cout << "mean_dE " << mean_dE << " sum_w " << sum_w << endl;
  return mean_dE/sum_w;
}

Double_t calc_mean_range(TH1F *hist = 0, int minbin = 0, int maxbin = 100)
{
  double mean = 0.;
  for(int i = minbin; i<maxbin; i++)
    {
      mean += hist->GetBinContent(i);
    }

  return mean/((double)(maxbin-minbin));
}

Double_t calc_mean_range(TGraph *gr = 0, int minbin = 0, int maxbin = 100)
{
  double mean = 0.;
  double RAA,pt;
  for(int i = minbin; i<maxbin; i++)
    {
      gr->GetPoint(i,pt,RAA);
      mean+=RAA;
    }

  return mean/((double)(maxbin-minbin));
}

Double_t calc_mean_range_width(TH1F *hist = 0, int minbin = 0, int maxbin = 100)
{
  double mean = 0.;
  double norm = 0.;
  for(int i = minbin; i<maxbin; i++)
    {
      mean += hist->GetBinContent(i)*hist->GetBinWidth(i);
      norm += hist->GetBinWidth(i);
    }

  return mean/norm;
}

Double_t calc_mean_range_width(TGraph *gr = 0, int minbin = 0, int maxbin = 100)
{
  double mean = 0.;
  double norm = 0.;
  double y,x;
  double yold, xold;
  gr->GetPoint(minbin,xold,yold);
  for(int i = minbin+1; i<maxbin; i++)
    {
      gr->GetPoint(i,x,y);
      mean+=(y-0.5*(y-yold))*(x-xold);
      norm += x-xold;
      xold=x;
      yold=y;
    }

  return mean/norm;
}

// Double_t calc_qhateff(TGraph *gr = 0x0, int minbin = 0, int maxbin = 100,Double_t &Leff, Bool_t fPartonStart = kFALSE, Int_t cas = 0, Float_t FormTime = 0.6)
// {
//   Double_t J03 = 0.;
//   Double_t J13 = 0.;
//   Double_t y,x;
//   Double_t yold, xold;
//   Double_t l = 0.;
//   Double_t kDl = 0.;
//   gr->GetPoint(minbin,xold,yold);
//   if(fPartonStart && cas==1) {
//     J03+= yold * FormTime;
//     J13+= yold * FormTime * FormTime;
//   }
//   for(int i = minbin+1; i<maxbin; i++)
//     {
//       gr->GetPoint(i,x,y);
//       kDl = x-xold;
//       l+=kDl;
//       if(fPartonStart) {
// 	J03 += 0.5*(y+yold)*(x-xold);
// 	J13 += 0.5*(y+yold)*(l+FormTime)*(x-xold);
//       } else {
// 	J03 += 0.5*(y+yold)*(x-xold);
// 	J13 += 0.5*(y+yold)*l*(x-xold);
//       }
//       //      cout << "l: " << l << "\tqhat: " << y << endl;

//       xold=x;
//       yold=y;
//    }
  
//   Double_t qhateff = J03*J03/(2.*J13);
//   Leff =  2*J13/J03;

//   return qhateff;
// }

void scale_graph(TGraphAsymmErrors *gr, Float_t scale) {
  for (Int_t i=0; i<gr->GetN(); i++) {
    Double_t x,y;
    gr->GetPoint(i,x,y);
    gr->SetPoint(i,x,scale*y);
    gr->SetPointError(i,gr->GetErrorXlow(i),gr->GetErrorXhigh(i),scale*gr->GetErrorYlow(i),scale*gr->GetErrorYhigh(i));
  }
}

void scale_graph(TGraphErrors *gr, Float_t scale) {
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

void scale_graph_x(TGraph *gr) {
  for (Int_t i=0; i<gr->GetN(); i++) {
    Double_t x,y;
    gr->GetPoint(i,x,y);
    gr->SetPoint(i,x,x*y);
  }
}

void add_graph(TGraphErrors *gr, Float_t a) {
  for (Int_t i=0; i<gr->GetN(); i++) {
    Double_t x,y;
    gr->GetPoint(i,x,y);
    gr->SetPoint(i,x,y+a);
    gr->SetPointError(i,gr->GetErrorX(i),gr->GetErrorY(i));// * (1.+a/y));
  }
}

void add_histo(TH1 *h1, Float_t a) {
  for (Int_t i=1; i<=h1->GetXaxis()->GetNbins(); i++) {
    double y = h1->GetBinContent(i);
    if(y>0.) {
      h1->SetBinContent(i,y+a);
      h1->SetBinError(i,h1->GetBinError(i));
    }
  }
}

void DivideGraphWithBinWidth(TGraphErrors *gr) {
  for (Int_t i=0; i<gr->GetN(); i++) {
    Double_t x,y,xerr,yerr;
    gr->GetPoint(i,x,y);
    gr->SetPoint(i,x,y/(2.*gr->GetErrorX(i)));
    gr->SetPointError(i,gr->GetErrorX(i),gr->GetErrorY(i)/(gr->GetErrorX(i)*2.));
  }
}

void MultiplyGraphWithBinWidth(TGraphErrors *gr) {
  for (Int_t i=0; i<gr->GetN(); i++) {
    Double_t x,y,xerr,yerr;
    gr->GetPoint(i,x,y);
    gr->SetPoint(i,x,y*2.*gr->GetErrorX(i));
    gr->SetPointError(i,gr->GetErrorX(i),gr->GetErrorY(i)*gr->GetErrorX(i)*2.);
  }
}

TGraph *add_graphs(TGraph *gr1, TGraph *gr2) {

  TGraph *newgraph = new TGraph();

  Double_t x1,y1,x2,y2;
  for(int j=0; j<gr1->GetN(); j++) {
    gr1->GetPoint(j,x1,y1);
    for(int i=0; i<gr2->GetN(); i++) {
      gr2->GetPoint(i,x2,y2);
      //      cout << "x1: " << x1 << "\ty1: " << y1 << endl;
      if(x1==x2) {
	//cout << "x: " << x1 << "\ty: " << y1 << endl;
	//cout << "x2: " << x2 << "\ty2: " << y2 << endl;
	newgraph->SetPoint(newgraph->GetN(),x1,y1+y2);
      }
    }
  }

  return newgraph;

}

TGraphErrors *add_graphs(TGraphErrors *gr1, TGraphErrors *gr2) {

  TGraphErrors *newgraph = new TGraphErrors();

  Double_t x1,y1,x2,y2;
  for(int j=0; j<gr1->GetN(); j++) {
    gr1->GetPoint(j,x1,y1);
    for(int i=0; i<gr2->GetN(); i++) {
      gr2->GetPoint(i,x2,y2);
      //      cout << "x1: " << x1 << "\ty1: " << y1 << endl;
      if(x1==x2) {
	//cout << "x: " << x1 << "\ty: " << y1 << endl;
	//cout << "x2: " << x2 << "\ty2: " << y2 << endl;
	newgraph->SetPoint(newgraph->GetN(),x1,y1+y2);
	double error2 = gr1->GetErrorY(j)*gr1->GetErrorY(j)+gr2->GetErrorY(i)*gr2->GetErrorY(i);
	if(error2>0.)
	  newgraph->SetPointError(newgraph->GetN()-1,gr1->GetErrorX(j),TMath::Sqrt(error2));
	else
	  newgraph->SetPointError(newgraph->GetN()-1,gr1->GetErrorX(j),0.);

      }
    }
  }

  return newgraph;

}

TGraph *divide_graphs_int(TGraph *num, TF1 *denom) {
  Double_t x,y1,y2;
  TGraph *gr = new TGraph();
  for (Int_t i = 0; i < num->GetN(); i++) {
    num->GetPoint(i,x,y1);
    y2 = denom->Eval(x);
    if(y2!=0.) gr->SetPoint(gr->GetN(),x,y1/y2);
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
      //cout << "x " << x << " x1 " << x1 << " x2 " << x2 << " y " << y << " y_int " << y_int << endl;
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
      cout << "i " << i << " x " << x << " y " << y << " x1 " << x1 << " x2 " << x2 << " y1 " << y1 << " y2 " << y2 << " y_int " << y_int << endl;
      Float_t y_int_err = sqrt(pow((x2-x)/(x2-x1)*y_err1,2)+pow((x-x1)/(x2-x1)*y_err2,2));
      cout << "y_int " << y_int << " y_int_err " << y_int_err << " y " << y << " y_err " << y_err << endl;
      gr->SetPoint(gr->GetN(),x,y/y_int);
      gr->SetPointError(gr->GetN()-1,0,sqrt(pow(y_err/y,2)+pow(y_int_err/y_int,2))*y/y_int);
    }
  }
  return gr;
}

TGraphErrors *divide_histowithTF1(TH1 *h1, TF1 *f1) {

  TGraphErrors *gr = new TGraphErrors();

  for(int i =1; i<h1->GetXaxis()->GetNbins(); i++) {
    Double_t binLo = h1->GetXaxis()->GetBinLowEdge(i);
    Double_t binUp = h1->GetXaxis()->GetBinUpEdge(i);
    Double_t binCenter = h1->GetXaxis()->GetBinCenter(i);
    Double_t binWidth = h1->GetXaxis()->GetBinWidth(i);
    Double_t binContent = h1->GetBinContent(i);

    Double_t yieldF = f1->Integral(binLo,binUp);

    if(yieldF!=0.) {
      gr->SetPoint(gr->GetN(),binCenter,binContent/yieldF*binWidth);
      Double_t error = 1./yieldF*h1->GetBinError(i);
      gr->SetPointError(gr->GetN()-1,binWidth/2.,error);
    }
  }

  return gr;

}

void subtract_constant_graph(TGraph *gr,Double_t constant) {
  for (Int_t i=0; i<gr->GetN(); i++) {
    Double_t x,y;
    gr->GetPoint(i,x,y);
    gr->SetPoint(i,x,y-constant);
  }
}  

void calc_meanX_hist(TH1F *hist,Double_t xmin = 0., Double_t xmax = 0.) {

}         

TGraphErrors* divide_histos(TH1 *h1 = 0x0, TH1* h2 = 0x0, Double_t xmax=-1., Bool_t bNoErrorh2 = kFALSE) {

  if(!h1) { Printf("h1 doesn't exist"); return 0; }
  if(!h2) { Printf("h2 doesn't exist"); return 0; }
  
  TGraphErrors *gr = new TGraphErrors();

  float binCent = 0.;
  int j = 0;
  float ratio = 0.;
  float error2 = 0.;
  float binWidth = 0.;
  for(int i=1; i<=h1->GetNbinsX(); i++) {
    binCent = h1->GetXaxis()->GetBinCenter(i);
    if(xmax>0. && binCent>xmax) continue;
    j = h2->FindBin(binCent);
    binWidth = h1->GetXaxis()->GetBinWidth(i);
    //    Printf("i,j: %d,%d",i,j);
    // cout << "i,j: " << i << "," << j << "\tbinWidth: " << binWidth << " - " << h2->GetXaxis()->GetBinWidth(j) << "\tbinCent: " << binCent << " - " << h2->GetXaxis()->GetBinCenter(j) << endl;
    if(h1->GetBinContent(i)<1e-12) continue;
    if(h2->GetBinContent(j)>0.) {
      //      cout << "h1->GetBinError(" << i << ") = " << h1->GetBinError(i) << "\t" << "h2->GetBinError(" << i << ") = " << h2->GetBinError(i) << endl;
      ratio = h1->GetBinContent(i)/h2->GetBinContent(j);
      //      cout << "yield: " << h1->GetBinContent(i) << " / " << h2->GetBinContent(j) << endl;

      //      error2 = (h1->GetBinContent(i)*h1->GetBinError(i))*(h1->GetBinContent(i)*h1->GetBinError(i)) + (-1.*h1->GetBinContent(i)/h2->GetBinContent(j)/h2->GetBinContent(j)*h2->GetBinError(j))*(-1.*h1->GetBinContent(i)/h2->GetBinContent(j)/h2->GetBinContent(j)*h2->GetBinError(j));

      Double_t A = 1./h2->GetBinContent(j)*h1->GetBinError(i);
      Double_t B = 0.;
      if(!bNoErrorh2) {
	if(h2->GetBinError(j)>0.) {
	  B = -1.*h1->GetBinContent(i)/(h2->GetBinContent(j)*h2->GetBinContent(j))*h2->GetBinError(j);
	  error2 = A*A + B*B;
	}
	else error2 = A*A;
      }
      else error2 = A*A;
      //Printf("binCent: %f ratio: %f",binCent,ratio);
      gr->SetPoint(gr->GetN(),binCent,ratio);
      gr->SetPointError(gr->GetN()-1,0.5*binWidth,TMath::Sqrt(error2));
    }
  }

  return gr;
}

TGraphErrors* divide_functions_binwidth(TF1 *f1, TF1 *f2, TH1 *h1) {

  //TH1 *h1 for bin intervals

  if(!f1) { Printf("f1 doesn't exist"); return 0; }
  if(!f2) { Printf("f2 doesn't exist"); return 0; }
  
  TGraphErrors *gr = new TGraphErrors();
  
  for(int i=1; i<=h1->GetNbinsX(); i++) {
    double binCent = h1->GetXaxis()->GetBinCenter(i);
    double binWidth = h1->GetXaxis()->GetBinWidth(i);
    double binMin = h1->GetXaxis()->GetBinLowEdge(i);
    double binMax = h1->GetXaxis()->GetBinUpEdge(i);

    double y1 = f1->Integral(binMin,binMax);
    double y2 = f2->Integral(binMin,binMax);
    
    double ratio = y1/y2;
    
    //Printf("binCent: %f ratio: %f",binCent,ratio);
    gr->SetPoint(gr->GetN(),binCent,ratio);
    gr->SetPointError(gr->GetN()-1,0.5*binWidth,0.);
  }

  return gr;
}

void createHistoFromFunction(TF1 *f1, TH1 *h1, double xmin = 0.1, bool useBinCenter = false) {

  //TH1 *h1 for bin intervals
  if(!f1) { Printf("f1 doesn't exist"); return; }

  for(int i=1; i<=h1->GetNbinsX(); i++) {
    double binCent = h1->GetXaxis()->GetBinCenter(i);
    if(binCent<xmin) continue;

    double y1 = 0.;
    if(useBinCenter) {
      y1 = f1->Eval(binCent);
    } else {
      double binWidth = h1->GetXaxis()->GetBinWidth(i);
      double binMin = h1->GetXaxis()->GetBinLowEdge(i);
      double binMax = h1->GetXaxis()->GetBinUpEdge(i);
      y1 = f1->Integral(binMin,binMax);
    }
    Printf("utilsMV::createHistoFromFunction binCent: %f y1: %f",binCent,y1);
    h1->SetBinContent(i,y1);
  }
  
}

TGraphErrors* divide_histosNoErrorh2(TH1 *h1, TH1* h2) {

  TGraphErrors *gr = new TGraphErrors();

  float binCent = 0.;
  int j = 0;
  float ratio = 0.;
  float error2 = 0.;
  float binWidth = 0.;
  for(int i=1; i<=h1->GetNbinsX(); i++) {
    binCent = h1->GetXaxis()->GetBinCenter(i);
    j = h2->FindBin(binCent);
    binWidth = h1->GetXaxis()->GetBinWidth(i);
    cout << "i,j: " << i << "," << j << "\tbinWidth: " << binWidth << " - " << h2->GetXaxis()->GetBinWidth(j) << "\tbinCent: " << binCent << " - " << h2->GetXaxis()->GetBinCenter(j) << endl;
    if(h2->GetBinContent(j)>0.) {
      //      cout << "h1->GetBinError(" << i << ") = " << h1->GetBinError(i) << "\t" << "h2->GetBinError(" << i << ") = " << h2->GetBinError(i) << endl;
      ratio = h1->GetBinContent(i)/h2->GetBinContent(j);
      //      cout << "yield: " << h1->GetBinContent(i) << " / " << h2->GetBinContent(j) << endl;

      //      error2 = (h1->GetBinContent(i)*h1->GetBinError(i))*(h1->GetBinContent(i)*h1->GetBinError(i)) + (-1.*h1->GetBinContent(i)/h2->GetBinContent(j)/h2->GetBinContent(j)*h2->GetBinError(j))*(-1.*h1->GetBinContent(i)/h2->GetBinContent(j)/h2->GetBinContent(j)*h2->GetBinError(j));

      Double_t A = 1./h2->GetBinContent(j)*h1->GetBinError(i);
      Double_t B = 1.;
      if(h2->GetBinError(j)>0.) {
	B = -1.*h1->GetBinContent(i)/(h2->GetBinContent(j)*h2->GetBinContent(j))*0.;//h2->GetBinError(j);
	error2 = A*A + B*B;
      }
      else error2 = 0.;

      gr->SetPoint(gr->GetN(),binCent,ratio);
      gr->SetPointError(gr->GetN()-1,0.5*binWidth,TMath::Sqrt(error2));
    }
  }

  return gr;
}    

TH1* subtract_histos(TH1 *h1, TH1 *h2) {

  TH1 *h3 = (TH3*)h1->Clone();
  h3->Reset();

  if(h1->GetNbinsX() != h2->GetNbinsX()) {
    printf("bins of histos is not the same (%d vs %d). Aborting.", h1->GetNbinsX(), h2->GetNbinsX());
    return 0;
  }

  for(int i=0; i<=h1->GetNbinsX(); i++) {

    h3->SetBinContent(i,h1->GetBinContent(i) - h2->GetBinContent(i));

  }

  return h3;
} 


TH2* subtract_histos(TH2 *h1, TH2 *h2) {

  TH2 *h3 = (TH2*)h1->Clone();
  h3->Reset();

  if(h1->GetNbinsX() != h2->GetNbinsX()) {
    printf("bins x-axis of histos is not the same (%d vs %d). Aborting.", h1->GetNbinsX(), h2->GetNbinsX());
    return 0;
  }

  if(h1->GetNbinsY() != h2->GetNbinsY()) {
    printf("bins y-axis of histos is not the same (%d vs %d). Aborting.", h1->GetNbinsY(), h2->GetNbinsY());
    return 0;
  }

  for(int i=0; i<=h1->GetNbinsX(); i++) {

    for(int j=0; j<=h1->GetNbinsY(); j++) {

      h3->SetBinContent(i,j,h1->GetBinContent(i,j) - h2->GetBinContent(i,j));

    }
    
  }

  return h3;
}         

       
               
TH1* add_histos(TH1 *h1, TH1 *h2) {

  TH1 *h3 = (TH1*)h1->Clone();
  h3->Reset();

  if(h1->GetNbinsX() != h2->GetNbinsX()) {
    printf("bins of histos is not the same (%d vs %d). Aborting.", h1->GetNbinsX(), h2->GetNbinsX());
    return 0;
  }

  for(int i=0; i<=h1->GetNbinsX(); i++) {

    h3->SetBinContent(i,h1->GetBinContent(i) + h2->GetBinContent(i));
    Double_t error2 = h1->GetBinError(i)*h1->GetBinError(i) + h2->GetBinError(i)*h2->GetBinError(i);
    h3->SetBinError(i,TMath::Sqrt(error2));

  }

  return h3;
} 


TH2* add_histos(TH2 *h1, TH2 *h2) {

  TH2 *h3 = (TH2*)(h1->Clone("h3clone"));
  h3->Reset();

  if(h1->GetNbinsX() != h2->GetNbinsX()) {
    printf("bins x-axis of histos is not the same (%d vs %d). Aborting.", h1->GetNbinsX(), h2->GetNbinsX());
    return 0;
  }

  if(h1->GetNbinsY() != h2->GetNbinsY()) {
    printf("bins y-axis of histos is not the same (%d vs %d). Aborting.", h1->GetNbinsY(), h2->GetNbinsY());
    return 0;
  }

  for(int i=0; i<=h1->GetNbinsX(); i++) {

    for(int j=0; j<=h1->GetNbinsY(); j++) {

      h3->SetBinContent(i,j,h1->GetBinContent(i,j) + h2->GetBinContent(i,j));

    }
    
  }

  return h3;
}   

//_______________________________________________________________________________________________________________________
Double_t GetFirstDerivative(TH1 *histo) {

  Double_t sumDiff = 0.;
  Double_t diff = 0.;

  for(int i =1; i<histo->GetNbinsX(); i++) {

    diff = TMath::Abs(histo->GetBinContent(i)-histo->GetBinContent(i+1))/(histo->GetBinCenter(i+1)-histo->GetBinCenter(i));
    sumDiff+=diff;

    cout << "i: " << i << "\tdiff: " << diff << endl;

  } 

  //  sumDiff = sumDiff/(double)i;
  cout << "sumDiff: " << sumDiff << endl;

  return sumDiff;
}

//_______________________________________________________________________________________________________________________
Double_t GetRatioYAvgY(TH1 *histo) {

  //Calculate avgY from linear fit to points
  TF1 *f1 = new TF1("linFit","pol0");
  f1->SetParameter(0,0.05);
  histo->Fit(f1,"0");
  Double_t avgY = f1->GetParameter(0);

  cout << "param lin fit: " << avgY << endl;
  if(avgY==0.) return 0.;

  Double_t ratio= 0.;
  Double_t sumRatio = 0.;
  int counter = 0;
  for(int i =1; i<histo->GetNbinsX(); i++) {

    double yield = histo->GetBinContent(i);
    cout << "i: " << i << "\tyield: " << yield << endl;
    if(yield>0.) {
      ratio=yield/avgY;   
      counter++;
    }

    cout << "i: " << i << "\tratio: " << ratio << endl;
    sumRatio+=ratio;
  }

  cout << "sumRatio: " << sumRatio << "\tcounter: " << counter << endl; 
  sumRatio = sumRatio/((double)counter);

  cout << "sumRatio: " << sumRatio << endl;

  return sumRatio;
}

//_______________________________________________________________________________________________________________________
Double_t GetMaxYTGraph(TGraph *gr) {

  Double_t maxY = 0.;
  Double_t x,y;
  for(int i=0; i<gr->GetN();i++) {

    gr->GetPoint(i,x,y);
    if(y>maxY) maxY=y;
  }

  return maxY;

}

//_______________________________________________________________________________________________________________________
Double_t GetMinYTGraph(TGraph *gr) {

  Double_t minY = GetMaxYTGraph(gr);
  Double_t xminY = 0.;
  Double_t x,y;
  for(int i=0; i<gr->GetN();i++) {

    gr->GetPoint(i,x,y);
    if(y<minY) {
      minY=y;
      xminY = x;
    }
  }

  return minY;

}

//_______________________________________________________________________________________________________________________
Double_t GetXMaxYTGraph(TGraph *gr) {

  Double_t maxY = 0.;
  Double_t xmaxY = 0.;
  Double_t x,y;
  for(int i=0; i<gr->GetN();i++) {

    gr->GetPoint(i,x,y);
    if(y>maxY) {
      maxY=y;
      xmaxY = x;
    }
  }

  return xmaxY;

}

//_______________________________________________________________________________________________________________________
Double_t GetXMinYTGraph(TGraph *gr, Double_t xmin) {

  Double_t minY = GetMaxYTGraph(gr);
  Double_t xminY = 0.;
  Double_t x,y;
  for(int i=0; i<gr->GetN();i++) {

    gr->GetPoint(i,x,y);
    if(x<=xmin) continue;
    if(y<minY) {
      minY=y;
      xminY = x;
    }
  }

  return xminY;

}


    
//_______________________________________________________________________________________________________________________
Double_t InterpolateFast(TGraph *gr, Double_t x) {

  Double_t ipmax = gr->GetN()-1.;
  Double_t x2,y2,xold,yold;

  Double_t xmin,ymin,xmax, ymax;
  gr->GetPoint(0,xmin,ymin);
  gr->GetPoint(gr->GetN()-1,xmax,ymax);
  if(x<xmin || x>xmax) return 0;

  Double_t ip = ipmax/2.;
  Double_t ipold = 0.;
  gr->GetPoint((int)(ip),x2,y2);

  Int_t i = 0;

  if(x2>x) {
    while(x2>x) {
      if(i==0) ipold=0.;
      ip -= (ip)/2.;
      gr->GetPoint((int)(ip),x2,y2);
      if(x2>x) ipold=ipold;
      else ipold = ip;
      i++;
      //      cout << "ipold: " << ipold << "\tip: " << ip << "\tx2: " << x2 << "\ty2: " << y2 << endl;
    }
  }
  else if(x2<x) {
    while(x2<x) {
      if(i==0) ipold=ipmax;
      ip += (ipold-ip)/2.;
      gr->GetPoint((int)(ip),x2,y2);
      if(x2>x) ipold = ip;
      else ipold = ipold;
      i++;
      //      cout << "ipold: " << ipold << "\tip: " << ip << "\tx2: " << x2 << "\ty2: " << y2 << endl;
    }
  }

  int ip2 = 0;
  if(x2>x) {
    ip2 = (int)(ip)-1;
    gr->GetPoint(ip2,x2,y2);
    while(x2>x) {
      ip2--;
      gr->GetPoint(ip2,x2,y2);
    }
    gr->GetPoint(ip2+1,xold,yold);
  }
  else {
    ip2 = (int)(ip)+1;
    gr->GetPoint(ip2,x2,y2);
    while(x2<x) {
      ip2++;
      gr->GetPoint(ip2,x2,y2);
    }
    gr->GetPoint(ip2-1,xold,yold);

  }
  //  cout << "For x=" << x << " interpolate between: " << xold << " and " << x2 << endl;
  return ((x-xold)*y2 + (x2-x)*yold) / (x2-xold);

}

//_______________________________________________________________________________________________________________________
void SetErrorXToZero(TGraphErrors *gr=0x0) {

  for(int i=0; i<gr->GetN(); i++) {
    gr->SetPointError(i,0.,gr->GetErrorY(i));
  }
}

//_______________________________________________________________________________________________________________________
void SetErrorXToZero(TGraphAsymmErrors *gr) {

  for(int i=0; i<gr->GetN(); i++) {
    //   gr->SetPointError(i,0.,gr->GetErrorY(i));
    gr->SetPointEXlow(i,0.);
    gr->SetPointEXhigh(i,0.);
  }
}

//_______________________________________________________________________________________________________________________
void SetErrorXToFrac(TGraphErrors *gr, Double_t frac) {

  for(int i=0; i<gr->GetN(); i++) {
    gr->SetPointError(i,frac*gr->GetErrorX(i),gr->GetErrorY(i));
  }
}

//_______________________________________________________________________________________________________________________
TGraphAsymmErrors *MoveBinPosX(TGraphErrors *gr, Double_t shift) {

  TGraphAsymmErrors *grNew = new TGraphAsymmErrors();
  for(int i=0; i<gr->GetN(); i++) {
    double x,y;
    double ex,ey;
    gr->GetPoint(i,x,y);
    ex = gr->GetErrorX(i);
    ey = gr->GetErrorY(i);

    grNew->SetPoint(i,x+shift,y);
    if(ex>0.) grNew->SetPointError(i,ex+shift,ex+shift,ey,ey);
    else  grNew->SetPointError(i,0.,0.,ey,ey);

  }

  return grNew;
}

//_______________________________________________________________________________________________________________________
TGraphAsymmErrors *MoveBinPosX(TGraphAsymmErrors *gr, Double_t shift =0) {

  TGraphAsymmErrors *grNew = new TGraphAsymmErrors();

  for(int i=0; i<gr->GetN(); i++) {
    double x,y;
    gr->GetPoint(i,x,y);
    double exlow = gr->GetErrorXlow(i);
    double exhigh = gr->GetErrorXhigh(i);
    double eylow = gr->GetErrorYlow(i);
    double eyhigh = gr->GetErrorYhigh(i);

    grNew->SetPoint(i,x+shift,y);
    //   grNew->SetPointError(i,exlow+shift,exlow-shift,eylow,eyhigh);
    grNew->SetPointError(i,exlow,exlow,eylow,eyhigh);

  }

  return grNew;
}


TGraphErrors *divide_graphs(TGraphErrors *gr1, TGraphErrors *gr2, Bool_t bWithoutErrorY) {

  TGraphErrors *newgraph = new TGraphErrors();

  Double_t x1,y1,x2,y2;
  for(int j=0; j<gr1->GetN(); j++) {
    gr1->GetPoint(j,x1,y1);
    for(int i=0; i<gr2->GetN(); i++) {
      gr2->GetPoint(i,x2,y2);
      if(x1==x2) {
	if(y2>0.){
	  newgraph->SetPoint(newgraph->GetN(),x1,y1/y2);
	  double A = (-1.*y1/y2/y2)*(-1.*y1/y2/y2)*gr2->GetErrorY(i)*gr2->GetErrorY(i);
	  if(bWithoutErrorY) A = 0.;
	  double B = 1./y2/y2*gr1->GetErrorY(j)*gr1->GetErrorY(j);
	  double error2 = A + B;
	  newgraph->SetPointError(newgraph->GetN()-1,gr1->GetErrorX(j),TMath::Sqrt(error2));
	}
      }
    }

  }

  return newgraph;
}



TGraphErrors *CalcRatioGraphs(TGraphErrors *gr1=0, TGraphErrors *gr2=0, Bool_t bWithoutErrorY=kFALSE, bool bPrint = false) {

  TGraphErrors *newgraph = new TGraphErrors();

  Double_t x1,y1,x2,y2;
  for(int j=0; j<gr1->GetN(); j++) {
    gr1->GetPoint(j,x1,y1);
    for(int i=0; i<gr2->GetN(); i++) {
      gr2->GetPoint(i,x2,y2);
      if(abs(x1-x2)<1e-6) {
        if(bPrint) Printf("CalcRatioGraphs x1: %f x2: %f",x1,x2);
	//double diff = x2-x1;
	//if(diff<5. && diff>0.) {
	if(y2>0.){
	  newgraph->SetPoint(newgraph->GetN(),x1,y1/y2);
	  double A = (-1.*y1/y2/y2)*gr2->GetErrorY(i);
	  if(bWithoutErrorY) A = 0.;
	  double B = 1./y2*gr1->GetErrorY(j);
          if(bPrint) {
            Printf("err_y2: %f A: %f",gr2->GetErrorY(i),A);
            Printf("err_y1: %f B: %f",gr1->GetErrorY(j),B);
          }
	  double error2 = A*A + B*B;
          if(bPrint) Printf("error2: %f  error: %f",error2,sqrt(error2));
	  newgraph->SetPointError(newgraph->GetN()-1,gr1->GetErrorX(j),TMath::Sqrt(error2));
	}
      }
    }

  }

  return newgraph;
}

TGraphAsymmErrors *CalcRatioGraphs(TGraphAsymmErrors *gr1=0, TGraphErrors *gr2=0, Bool_t bWithoutErrorY=kFALSE) {

  TGraphAsymmErrors *newgraph = new TGraphAsymmErrors();

  Double_t x1,y1,x2,y2;
  for(int j=0; j<gr1->GetN(); j++) {
    gr1->GetPoint(j,x1,y1);
    //    cout << "x1: " << x1 << endl;
    for(int i=0; i<gr2->GetN(); i++) {
      gr2->GetPoint(i,x2,y2);
      if(x1==x2) {
	//	cout << "x1: " << x1 << " x2: " << x2 << "  y1: " << y1 << " y2: " << y2 << "  y1/y2: " << y1/y2 << endl;
	if(y2>0.){
	  newgraph->SetPoint(newgraph->GetN(),x1,y1/y2);
	  double A = (-1.*y1/y2/y2)*(-1.*y1/y2/y2)*gr2->GetErrorYlow(i)*gr2->GetErrorYlow(i);
	  if(bWithoutErrorY) A = 0.;
	  double B = 1./y2/y2*gr1->GetErrorYlow(j)*gr1->GetErrorYlow(j);
	  double error2 = A + B;
	  newgraph->SetPointEYlow(newgraph->GetN()-1,TMath::Sqrt(error2));

	  A = (-1.*y1/y2/y2)*(-1.*y1/y2/y2)*gr2->GetErrorYhigh(i)*gr2->GetErrorYhigh(i);
	  if(bWithoutErrorY) A = 0.;
	  B = 1./y2/y2*gr1->GetErrorYhigh(j)*gr1->GetErrorYhigh(j);
	  error2 = A + B;
	  newgraph->SetPointEYhigh(newgraph->GetN()-1,TMath::Sqrt(error2));

	  newgraph->SetPointEXlow(newgraph->GetN()-1,gr1->GetErrorXlow(j));
	  newgraph->SetPointEXhigh(newgraph->GetN()-1,gr1->GetErrorXhigh(j));
	}
      }
    }

  }

  return newgraph;
}


TGraphAsymmErrors *CalcRatioGraphs(TGraphErrors *gr1=0, TGraphAsymmErrors *gr2=0, Bool_t bWithoutErrorY=kFALSE) {

  TGraphAsymmErrors *newgraph = new TGraphAsymmErrors();

  Double_t x1,y1,x2,y2;
  for(int j=0; j<gr1->GetN(); j++) {
    gr1->GetPoint(j,x1,y1);
    for(int i=0; i<gr2->GetN(); i++) {
      gr2->GetPoint(i,x2,y2);
      if(x1==x2) {
        if(y2>0.){
          newgraph->SetPoint(newgraph->GetN(),x1,y1/y2);
          double A = (-1.*y1/y2/y2)*(-1.*y1/y2/y2)*gr2->GetErrorYlow(i)*gr2->GetErrorYlow(i);
          if(bWithoutErrorY) A = 0.;
          double B = 1./y2/y2*gr1->GetErrorYlow(j)*gr1->GetErrorYlow(j);
          double error2 = A + B;
          newgraph->SetPointEYlow(newgraph->GetN()-1,TMath::Sqrt(error2));

          A = (-1.*y1/y2/y2)*(-1.*y1/y2/y2)*gr2->GetErrorYhigh(i)*gr2->GetErrorYhigh(i);
          if(bWithoutErrorY) A = 0.;
          B = 1./y2/y2*gr1->GetErrorYhigh(j)*gr1->GetErrorYhigh(j);
          error2 = A + B;
          newgraph->SetPointEYhigh(newgraph->GetN()-1,TMath::Sqrt(error2));

          newgraph->SetPointEXlow(newgraph->GetN()-1,gr1->GetErrorXlow(j));
          newgraph->SetPointEXhigh(newgraph->GetN()-1,gr1->GetErrorXhigh(j));
        }
      }
    }

  }

  return newgraph;
}


TGraphAsymmErrors *CalcRatioGraphs(TGraphAsymmErrors *gr1=0, TGraphAsymmErrors *gr2=0, Bool_t bWithoutErrorY=kFALSE) {

  TGraphAsymmErrors *newgraph = new TGraphAsymmErrors();

  if(!gr1) return 0x0;
  if(!gr2) return 0x0;

  Double_t x1,y1,x2,y2;
  for(int j=0; j<gr1->GetN(); j++) {
    gr1->GetPoint(j,x1,y1);
    for(int i=0; i<gr2->GetN(); i++) {
      gr2->GetPoint(i,x2,y2);
      if(x1==x2) {
	if(y2>0.){
	  newgraph->SetPoint(newgraph->GetN(),x1,y1/y2);
	  double A = (-1.*y1/y2/y2)*(-1.*y1/y2/y2)*gr2->GetErrorYlow(i)*gr2->GetErrorYlow(i);
	  if(bWithoutErrorY) A = 0.;
	  double B = 1./y2/y2*gr1->GetErrorYlow(j)*gr1->GetErrorYlow(j);
	  double error2 = A + B;
	  newgraph->SetPointEYlow(newgraph->GetN()-1,TMath::Sqrt(error2));

	  A = (-1.*y1/y2/y2)*(-1.*y1/y2/y2)*gr2->GetErrorYhigh(i)*gr2->GetErrorYhigh(i);
	  if(bWithoutErrorY) A = 0.;
	  B = 1./y2/y2*gr1->GetErrorYhigh(j)*gr1->GetErrorYhigh(j);
          error2 = A + B;
	  newgraph->SetPointEYhigh(newgraph->GetN()-1,TMath::Sqrt(error2));

	  newgraph->SetPointEXlow(newgraph->GetN()-1,gr1->GetErrorXlow(j));
	  newgraph->SetPointEXhigh(newgraph->GetN()-1,gr1->GetErrorXhigh(j));
	}
      }
    }

  }

  return newgraph;
}

TGraphAsymmErrors *CalcRatioGraphsOnlyLargest(TGraphAsymmErrors *gr1=0, TGraphAsymmErrors *gr2=0, Double_t xmax = -1.) {

  //Don't add errors but only take largest error up or down.

  TGraphAsymmErrors *newgraph = new TGraphAsymmErrors();

  if(!gr1) return 0x0;
  if(!gr2) return 0x0;

  Double_t x1,y1,x2,y2;
  for(int j=0; j<gr1->GetN(); j++) {
    gr1->GetPoint(j,x1,y1);
    for(int i=0; i<gr2->GetN(); i++) {
      gr2->GetPoint(i,x2,y2);
      if(x1==x2) {
	if(xmax>0 && x1>xmax) continue;

	if(y2>0.){
	  newgraph->SetPoint(newgraph->GetN(),x1,y1/y2);

	  Double_t eyLo1 = gr1->GetErrorYlow(j);
	  Double_t eyLo2 = gr2->GetErrorYlow(i);
	  Double_t eyUp1 = gr1->GetErrorYhigh(j);
	  Double_t eyUp2 = gr2->GetErrorYhigh(i);

	  if(eyLo1>eyLo2) eyLo2 = 0.;
	  else            eyLo1 = 0.;

	  if(eyUp1>eyUp2) eyUp2 = 0.;
	  else            eyUp1 = 0.;

	  double A = (-1.*y1/y2/y2)*(-1.*y1/y2/y2)*eyLo2*eyLo2;
	  double B = 1./y2/y2*eyLo1*eyLo1;
	  double error2 = A + B;
	  newgraph->SetPointEYlow(newgraph->GetN()-1,TMath::Sqrt(error2));

	  A = (-1.*y1/y2/y2)*(-1.*y1/y2/y2)*eyUp2*eyUp2;
	  B = 1./y2/y2*eyUp1*eyUp1;
	  error2 = A + B;
	  newgraph->SetPointEYhigh(newgraph->GetN()-1,TMath::Sqrt(error2));

	  newgraph->SetPointEXlow(newgraph->GetN()-1,gr1->GetErrorXlow(j));
	  newgraph->SetPointEXhigh(newgraph->GetN()-1,gr1->GetErrorXhigh(j));
	}
      }
    }

  }

  return newgraph;
}

TGraphErrors *CalcRatioGraphsOnlyLargest(TGraphErrors *gr1=0, TGraphErrors *gr2=0, Double_t xmax = -1.) {

  //Don't add errors but only take largest error up or down.

  TGraphErrors *newgraph = new TGraphErrors();

  if(!gr1) return 0x0;
  if(!gr2) return 0x0;

  Double_t x1,y1,x2,y2;
  for(int j=0; j<gr1->GetN(); j++) {
    gr1->GetPoint(j,x1,y1);
    if(xmax>0 && x1>xmax) continue;
    for(int i=0; i<gr2->GetN(); i++) {
      gr2->GetPoint(i,x2,y2);
      if(x1==x2) {
	if(y2>0.){
	  newgraph->SetPoint(newgraph->GetN(),x1,y1/y2);

	  Double_t ey1 = gr1->GetErrorY(j);
	  Double_t ey2 = gr2->GetErrorY(i);

	  if(ey1>ey2) ey2 = 0.;
	  else        ey1 = 0.;


	  double A = (-1.*y1/y2/y2)*(-1.*y1/y2/y2)*ey2*ey2;
	  double B = 1./y2/y2*ey1*ey1;
	  double error2 = A + B;
	  if(error2==0.) cout << "huh? error is 0 x1=" << x1<< endl;
	  // cout << "x1: " << x1 << "\terror: " << sqrt(error2) << endl;
	  newgraph->SetPointError(newgraph->GetN()-1,gr1->GetErrorX(j),TMath::Sqrt(error2));

	}
      }
      else {
	//	cout << "x1!=x2: " << x1 < "\t" << x2 << endl;
      }
    }

  }

  return newgraph;
}

TGraphAsymmErrors *CalcRatioGraphsCorrelated(TGraphAsymmErrors *gr1=0, TGraphAsymmErrors *gr2=0) {

  //Calc errors of ratio as grRatio_minError = gr1_minError/gr2_minError

  TGraphAsymmErrors *newgraph = new TGraphAsymmErrors();

  Double_t x1,y1,x2,y2;
  for(int j=0; j<gr1->GetN(); j++) {
    gr1->GetPoint(j,x1,y1);
    for(int i=0; i<gr2->GetN(); i++) {
      gr2->GetPoint(i,x2,y2);
      if(x1==x2) {
	if(y2>0.){
	  newgraph->SetPoint(newgraph->GetN(),x1,y1/y2);

	  double minError =0.;
	  double maxError = 0.;
	  
	  //cout << "y1: " << y1 << " lowErr: " << gr1->GetErrorYlow(j) << " highErr: " <<  gr1->GetErrorYhigh(j) << endl;
	  double minRatio = (y1 - gr1->GetErrorYlow(j)) / (y2 - gr2->GetErrorYlow(i));
	  minError += y1/y2 - minRatio;
	  if(minError<0.) {
	    maxError=maxError + -1.*minError;
	    minError-=minError;
	  }

	  double maxRatio = (y1 + gr1->GetErrorYhigh(j)) / (y2 + gr2->GetErrorYhigh(i));
	  maxError += maxRatio - y1/y2;
	  if(maxError<0.) {
	    minError=minError + -1.*maxError;
	    maxError-=maxError;
	  }
	  // cout << "ratio("<< x1 << "): " << y1/y2 << " - " << minError << " + " << maxError << endl;

	  newgraph->SetPointEYlow(newgraph->GetN()-1,minError);
	  newgraph->SetPointEYhigh(newgraph->GetN()-1,maxError);

	  newgraph->SetPointEXlow(newgraph->GetN()-1,gr1->GetErrorXlow(j));
	  newgraph->SetPointEXhigh(newgraph->GetN()-1,gr1->GetErrorXhigh(j));
	}
      }
    }

  }

  return newgraph;
}

TGraphErrors *CalcRatioGraphHisto(TGraphErrors *gr1=0, TH1 *h2=0, Bool_t bWithoutErrorY=kFALSE) {

  TGraphErrors *newgraph = new TGraphErrors();

  Double_t x1,y1,x2,y2;
  for(int j=0; j<gr1->GetN(); j++) {
    gr1->GetPoint(j,x1,y1);

    if(x1>h2->GetXaxis()->GetXmax()) continue;
    Int_t histoBin = h2->GetXaxis()->FindBin(x1);
    x2 = h2->GetXaxis()->GetBinCenter(histoBin);
    // cout << " x1 = " << x1 << "  x2 = " << x2 << "  histoBin = " << histoBin << endl;
    if(x1!=x2) {cout << "wrong bin x1 = " << x1 << "  x2 = " << x2 << "  histoBin = " << histoBin << endl; continue;}
    y2 = h2->GetBinContent(histoBin);
    Double_t errorY = h2->GetBinError(histoBin);
    Double_t errorX = h2->GetBinWidth(histoBin);

    if(y2>0.){
      newgraph->SetPoint(newgraph->GetN(),x1,y1/y2);

      double A = (-1.*y1/y2/y2)*(-1.*y1/y2/y2)*errorY*errorY;//*gr2->GetErrorY(i)*gr2->GetErrorY(i);
      if(bWithoutErrorY) A=0.;
      double B = 1./y2/y2*gr1->GetErrorY(j)*gr1->GetErrorY(j);
      double error2 = A + B;
      newgraph->SetPointError(newgraph->GetN()-1,gr1->GetErrorX(j),TMath::Sqrt(error2));

    }
  }
  
  return newgraph;
}

TGraphAsymmErrors *CalcRatioGraphHisto(TGraphAsymmErrors *gr1=0, TH1 *h2=0, Bool_t bWithoutErrorY=kFALSE) {

  TGraphAsymmErrors *newgraph = new TGraphAsymmErrors();

  Double_t x1,y1,x2,y2;
  for(int j=0; j<gr1->GetN(); j++) {
    gr1->GetPoint(j,x1,y1);

    if(x1>h2->GetXaxis()->GetXmax()) continue;
    Int_t histoBin = h2->GetXaxis()->FindBin(x1);
    x2 = h2->GetXaxis()->GetBinCenter(histoBin);
    if(x1!=x2) {cout << "wrong bin x1 = " << x1 << "  x2 = " << x2 << "  histoBin = " << histoBin << endl; continue;}
    y2 = h2->GetBinContent(histoBin);
    Double_t errorY = h2->GetBinError(histoBin);
    Double_t errorX = h2->GetBinWidth(histoBin);

    if(y2>0.){
      newgraph->SetPoint(newgraph->GetN(),x1,y1/y2);

      double A = (-1.*y1/y2/y2)*(-1.*y1/y2/y2)*errorY/2.*errorY/2.;//gr2->GetErrorYlow(i)*gr2->GetErrorYlow(i);
      if(bWithoutErrorY) A = 0.;
      double B = 1./y2/y2*gr1->GetErrorYlow(j)*gr1->GetErrorYlow(j);
      double error2 = A + B;
      newgraph->SetPointEYlow(newgraph->GetN()-1,TMath::Sqrt(error2));
      
      A = (-1.*y1/y2/y2)*(-1.*y1/y2/y2)*errorY/2.*errorY/2.;//*gr2->GetErrorYhigh(i)*gr2->GetErrorYhigh(i);
      if(bWithoutErrorY) A = 0.;
      B = 1./y2/y2*gr1->GetErrorYhigh(j)*gr1->GetErrorYhigh(j);
      error2 = A + B;
      newgraph->SetPointEYhigh(newgraph->GetN()-1,TMath::Sqrt(error2));
      
      newgraph->SetPointEXlow(newgraph->GetN()-1,gr1->GetErrorXlow(j));
      newgraph->SetPointEXhigh(newgraph->GetN()-1,gr1->GetErrorXhigh(j)); 
      
    }
  }
  
  return newgraph;
}

TGraphErrors *CalcRatioHistoGraph(TH1 *h2, TGraphErrors *gr1=0, Bool_t bWithoutErrorY=kFALSE) {

  TGraphErrors *newgraph = new TGraphErrors();

  Double_t x1,y1,x2,y2;
  for(int j=0; j<gr1->GetN(); j++) {
    gr1->GetPoint(j,x1,y1);

    if(x1>h2->GetXaxis()->GetXmax()) continue;
    Int_t histoBin = h2->GetXaxis()->FindBin(x1);
    x2 = h2->GetXaxis()->GetBinCenter(histoBin);
    // cout << " x1 = " << x1 << "  x2 = " << x2 << "  histoBin = " << histoBin << endl;
    if(x1!=x2) {cout << "wrong bin x1 = " << x1 << "  x2 = " << x2 << "  histoBin = " << histoBin << endl; continue;}
    y2 = h2->GetBinContent(histoBin);
    Double_t errorY = gr1->GetErrorY(j);
    Double_t errorX = h2->GetBinWidth(histoBin);

    if(y1>0.){
      newgraph->SetPoint(newgraph->GetN(),x1,y2/y1);

      double A = (-1.*y2/y1/y1)*(-1.*y2/y1/y1)*errorY*errorY;//*gr2->GetErrorY(i)*gr2->GetErrorY(i);
      if(bWithoutErrorY) A=0.;
      double B = 1./y1/y1*h2->GetBinError(histoBin)*h2->GetBinError(histoBin);
      double error2 = A + B;
      newgraph->SetPointError(newgraph->GetN()-1,gr1->GetErrorX(j),TMath::Sqrt(error2));
    }
  }
  
  return newgraph;
}

TGraphAsymmErrors *GetLargestErrors(TGraphAsymmErrors *gr1=0x0, TGraphAsymmErrors *gr2=0x0) {

  TGraphAsymmErrors *newgraph = new TGraphAsymmErrors();

  Double_t x1,y1,x2,y2;
  for(int i=0; i<gr1->GetN(); i++) {
    gr1->GetPoint(i,x1,y1);
    for(int j=0; j<gr2->GetN(); j++) {
      gr2->GetPoint(j,x2,y2);

      if(x1==x2 && y1==y2) {
	cout << "x: " << x1 << "\ty: " << y1 << endl;
	newgraph->SetPoint(newgraph->GetN(),x1,y1);

	double errLow1 = gr1->GetErrorYlow(i);
	double errHigh1 = gr1->GetErrorYhigh(i);

	double errLow2 = gr2->GetErrorYlow(j);
	double errHigh2 = gr2->GetErrorYhigh(j);

	newgraph->SetPointEYlow(newgraph->GetN()-1,TMath::Max(errLow1,errLow2));
	newgraph->SetPointEYhigh(newgraph->GetN()-1,TMath::Max(errHigh1,errHigh2));

	newgraph->SetPointEXlow(newgraph->GetN()-1,gr1->GetErrorXlow(i));
	newgraph->SetPointEXhigh(newgraph->GetN()-1,gr1->GetErrorXhigh(i));

      }
    }

  }


  return newgraph;
}

TGraphErrors *GetLargestRelativeErrorReturnGraph(TH1D *h1=0x0, TH1D *h2=0x0, TH1D *h3=0x0) {

  TGraphErrors *newgraph = new TGraphErrors();

  Double_t x1,y1,x2,y2,x3,y3;
  Double_t err1, err2, err3,relerr1, relerr2,relerr3;

  for(int i=1; i<=h1->GetNbinsX(); i++) {
    x1=h1->GetXaxis()->GetBinCenter(i);
    y1=h1->GetBinContent(i);
    err1=h1->GetBinError(i);
    if(y1>0)
      relerr1 = err1/y1;
    else
      continue;

    int j = h2->GetXaxis()->FindBin(x1);
    x2=h2->GetXaxis()->GetBinCenter(j);
    //    cout << "x1: " << x1 << "\tx2: " << x2 << endl;
    y2=h2->GetBinContent(j);
    err2=h2->GetBinError(j);
    if(y2>0)
      relerr2 = err2/y2;
    else
      continue;

    if(h3) {
      int k = h3->GetXaxis()->FindBin(x1);
      x3=h3->GetXaxis()->GetBinCenter(k);
      y3=h3->GetBinContent(k);
      err3=h3->GetBinError(k);
      if(y3>0)
	relerr3 = err3/y3;
      else
	relerr3 = 0.;  

      if(relerr3>relerr2) relerr2=relerr3;
      else if(relerr3>relerr1) relerr1=relerr3;
    }

    newgraph->SetPoint(newgraph->GetN(),x1,TMath::Max(relerr1,relerr2));
    newgraph->SetPointError(newgraph->GetN()-1,h1->GetXaxis()->GetBinWidth(i)*0.5,0.);
  }
  return newgraph;
}

TGraphErrors *GetSystematicGraphFromRelativeError(TGraphErrors *gr1, TGraphErrors *gr2) {

  TGraphErrors *newgraph = new TGraphErrors();

  Double_t x1,y1,x2,y2;
  for(int j=0; j<gr1->GetN(); j++) {
   gr1->GetPoint(j,x1,y1);
    for(int i=0; i<gr2->GetN(); i++) {
      gr2->GetPoint(i,x2,y2);
      if(x1==x2) {
	newgraph->SetPoint(newgraph->GetN(),x1,y1);
	newgraph->SetPointError(newgraph->GetN()-1,gr1->GetErrorX(j),y1*y2);
      }
    }
  }

  return newgraph;
}

TGraphAsymmErrors *AddErrGraphs(TGraphAsymmErrors *gr1=0x0, TGraphAsymmErrors *gr2=0x0, Bool_t bWithoutErrorY=kFALSE) {

  TGraphAsymmErrors *newgraph = new TGraphAsymmErrors();

  Double_t x1,y1,x2,y2;
  for(int j=0; j<gr1->GetN(); j++) {
    gr1->GetPoint(j,x1,y1);
    for(int i=0; i<gr2->GetN(); i++) {
      gr2->GetPoint(i,x2,y2);
      // cout << "x1: " << x1 << "\ty1: " << y1 << endl;
      // cout << "x2: " << x2 << "\ty2: " << y2 << endl;
      if(x1==x2 && y1==y2) {
	cout << "x: " << x1 << "\ty: " << y1 << endl;
	newgraph->SetPoint(newgraph->GetN(),x1,y1);
	double A = gr2->GetErrorYlow(i)*gr2->GetErrorYlow(i);
	if(bWithoutErrorY) A = 0.;
	double B = gr1->GetErrorYlow(j)*gr1->GetErrorYlow(j);
	double error2 = A + B;
	newgraph->SetPointEYlow(newgraph->GetN()-1,TMath::Sqrt(error2));

	A = gr2->GetErrorYhigh(i)*gr2->GetErrorYhigh(i);
	if(bWithoutErrorY) A = 0.;
        B = gr1->GetErrorYhigh(j)*gr1->GetErrorYhigh(j);
	error2 = A + B;
	newgraph->SetPointEYhigh(newgraph->GetN()-1,TMath::Sqrt(error2));

	newgraph->SetPointEXlow(newgraph->GetN()-1,gr1->GetErrorXlow(j));
	newgraph->SetPointEXhigh(newgraph->GetN()-1,gr1->GetErrorXhigh(j));
      }
    }
  }
  return newgraph;
}

void MultiplyHistGraph(TH1 *h2 = 0, TGraphErrors *gr1=0, Bool_t bWithoutErrorY=kFALSE) {

  Double_t x1,y1,x2,y2;
  for(int j=0; j<gr1->GetN(); j++) {
    gr1->GetPoint(j,x1,y1);

    if(x1>h2->GetXaxis()->GetXmax()) continue;
    Int_t histoBin = h2->GetXaxis()->FindBin(x1);
    x2 = h2->GetXaxis()->GetBinCenter(histoBin);
    // cout << " x1 = " << x1 << "  x2 = " << x2 << "  histoBin = " << histoBin << endl;
    //if(x1!=x2) {cout << "wrong bin x1 = " << x1 << "  x2 = " << x2 << "  histoBin = " << histoBin << endl; continue;}
    if(TMath::Abs(x1-x2)>1e-6) {cout << "wrong bin x1 = " << x1 << "  x2 = " << x2 << "  histoBin = " << histoBin << endl; continue;}
    y2 = h2->GetBinContent(histoBin);
    Double_t errorY = h2->GetBinError(histoBin);
    Double_t errorX = h2->GetBinWidth(histoBin);

    Printf("x1: %f y1: %f y2: %f",x1,y1,y2);
    double newContent = y1*y2;
    h2->SetBinContent(histoBin,newContent);
    double A = y1 * errorY;
    double B = y2 * gr1->GetErrorY(j);
    if(bWithoutErrorY) B = 0.;
    double error2 = A*A + B*B;
    if(error2>0.)
      h2->SetBinError(histoBin,sqrt(error2));
  }
}

void RemoveErrorsXaxis(TGraphErrors *gr) {

   for(int j=0; j<gr->GetN(); j++) {
     double ex = gr->GetErrorX(j);
     double ey = gr->GetErrorY(j);
     gr->SetPointError(j,0.,ey);
   }
  
}

// TGraphAsymmErrors *AddErrGraphs(TGraphAsymmErrors *gr1, TGraphAsymmErrors *gr2, Bool_t bWithoutErrorY) {

//   TGraphAsymmErrors *newgraph = new TGraphAsymmErrors();

//   Double_t x1,y1,x2,y2;
//   for(int j=0; j<gr1->GetN(); j++) {
//     gr1->GetPoint(j,x1,y1);
//     for(int i=0; i<gr2->GetN(); i++) {
//       gr2->GetPoint(i,x2,y2);
//       cout << "x1: " << x1 << "\ty1: " << y1 << endl;
//       cout << "x2: " << x2 << "\ty2: " << y2 << endl;
//       if(x1==x2 && y1==y2) {
// 	cout << "x: " << x1 << "\ty: " << y1 << endl;
// 	newgraph->SetPoint(newgraph->GetN(),x1,y1);
// 	double A = gr2->GetErrorYlow(i)*gr2->GetErrorYlow(i);
// 	if(bWithoutErrorY) A = 0.;
// 	double B = gr1->GetErrorYlow(j)*gr1->GetErrorYlow(j);
// 	double error2 = A + B;
// 	newgraph->SetPointEYlow(newgraph->GetN()-1,TMath::Sqrt(error2));

// 	double A = gr2->GetErrorYhigh(i)*gr2->GetErrorYhigh(i);
// 	if(bWithoutErrorY) A = 0.;
// 	double B = gr1->GetErrorYhigh(j)*gr1->GetErrorYhigh(j);
// 	double error2 = A + B;
// 	newgraph->SetPointEYhigh(newgraph->GetN()-1,TMath::Sqrt(error2));

// 	newgraph->SetPointEXlow(newgraph->GetN()-1,gr1->GetErrorXlow(j));
// 	newgraph->SetPointEXhigh(newgraph->GetN()-1,gr1->GetErrorXhigh(j));

//       }
//     }

//   }

//   return newgraph;
// }

//_______________________________________________________________________________________________________________________
TH1D *RandomizeDistribution(TH1D *hOrig, Bool_t bPois = kFALSE) {

  //
  // Create randomized distribution from original measured distribution
  // Poisson can only be used if original distribution contains counts
  //

  //use measured value as mean
  //use statistical error as sigma

  TH1D *hRandomized = (TH1D*)hOrig->Clone("hRandomized");
  hRandomized->Reset();

  TRandom3 *fRandom3 = new TRandom3(0);

  for(int i=1; i<=hOrig->GetNbinsX(); i++) {
    Double_t val = hOrig->GetBinContent(i);
    Double_t err = hOrig->GetBinError(i);
    Double_t ran = 0.;
    if(bPois)
      ran = fRandom3->PoissonD(val);
    else
      ran = fRandom3->Gaus(val,err);

    hRandomized->SetBinContent(i,ran);
    hRandomized->SetBinError(i,err);
  }

  return hRandomized;
}

//_______________________________________________________________________________________________________________________
TList *GetTListFromTrainOutput(TString str, Double_t R, Double_t ptTrack) {

  int filter = 272;
  Int_t iB = 2;
  TFile *fileRawJets = TFile::Open(str.Data());
  fileRawJets->ls();

 TString tmpName;
  if(R==0.2) {
    if(ptTrack==0.15)
      tmpName = Form("PWGJE_spec2_clustersAOD_ANTIKT02_B%d_Filter00%d_Cut00150_Skip00_clustersAOD_ANTIKT02_B0_Filter00%d_Cut00150_Skip00_0000000000_Class0%d",iB,filter,filter,0);
    if(ptTrack==1.)
      tmpName = Form("PWGJE_spec2_clustersAOD_ANTIKT02_B%d_Filter00%d_Cut01000_Skip00_clustersAOD_ANTIKT02_B0_Filter00%d_Cut01000_Skip00_0000000000_Class0%d",iB,filter,filter,0);
    if(ptTrack==2.)
      tmpName = Form("PWGJE_spec2_clustersAOD_ANTIKT02_B%d_Filter00%d_Cut02000_Skip00_clustersAOD_ANTIKT02_B0_Filter00%d_Cut02000_Skip00_0000000000_Class0%d",iB,filter,filter,0);
  }
  else if(R==0.3) {
    if(ptTrack==0.15)
      tmpName = Form("PWGJE_spec2_clustersAOD_ANTIKT03_B%d_Filter00%d_Cut00150_Skip00_clustersAOD_ANTIKT03_B0_Filter00%d_Cut00150_Skip00_0000000000_Class0%d",iB,filter,filter,0);
    if(ptTrack==1.)
      tmpName = Form("PWGJE_spec2_clustersAOD_ANTIKT03_B%d_Filter00%d_Cut01000_Skip00_clustersAOD_ANTIKT03_B0_Filter00%d_Cut01000_Skip00_0000000000_Class0%d",iB,filter,filter,0);
    if(ptTrack==2.)
      tmpName = Form("PWGJE_spec2_clustersAOD_ANTIKT03_B%d_Filter00%d_Cut02000_Skip00_clustersAOD_ANTIKT03_B0_Filter00%d_Cut02000_Skip00_0000000000_Class0%d",iB,filter,filter,0);
  }
  else if(R==0.4) {
    if(ptTrack==0.15)
      tmpName = Form("PWGJE_spec2_clustersAOD_ANTIKT04_B%d_Filter00%d_Cut00150_Skip00_clustersAOD_ANTIKT04_B0_Filter00%d_Cut00150_Skip00_0000000000_Class0%d",iB,filter,filter,0);
    if(ptTrack==1.)
      tmpName = Form("PWGJE_spec2_clustersAOD_ANTIKT04_B%d_Filter00%d_Cut01000_Skip00_clustersAOD_ANTIKT04_B0_Filter00%d_Cut01000_Skip00_0000000000_Class0%d",iB,filter,filter,0);
    if(ptTrack==2.)
      tmpName = Form("PWGJE_spec2_clustersAOD_ANTIKT04_B%d_Filter00%d_Cut02000_Skip00_clustersAOD_ANTIKT04_B0_Filter00%d_Cut02000_Skip00_0000000000_Class0%d",iB,filter,filter,0);
  }
  else {
    Printf("Radius not defined. Aborting...");
    return 0;
  }

  TDirectory *dir = (TDirectory*)fileRawJets->Get(tmpName.Data());
  if(!dir)Printf("!dir Line:%d %s not found",__LINE__,tmpName.Data());
  if(R==0.2) {
    if(ptTrack==0.15)
      tmpName = Form("pwgje_spec2_clustersAOD_ANTIKT02_B%d_Filter00%d_Cut00150_Skip00_clustersAOD_ANTIKT02_B0_Filter00%d_Cut00150_Skip00_0000000000_Class0%d",iB,filter,filter,0);
    if(ptTrack==1.)
      tmpName = Form("pwgje_spec2_clustersAOD_ANTIKT02_B%d_Filter00%d_Cut01000_Skip00_clustersAOD_ANTIKT02_B0_Filter00%d_Cut01000_Skip00_0000000000_Class0%d",iB,filter,filter,0);
    if(ptTrack==2.)
      tmpName = Form("pwgje_spec2_clustersAOD_ANTIKT02_B%d_Filter00%d_Cut02000_Skip00_clustersAOD_ANTIKT02_B0_Filter00%d_Cut02000_Skip00_0000000000_Class0%d",iB,filter,filter,0);
  }
  else if(R==0.3) {
    if(ptTrack==0.15)
      tmpName = Form("pwgje_spec2_clustersAOD_ANTIKT03_B%d_Filter00%d_Cut00150_Skip00_clustersAOD_ANTIKT03_B0_Filter00%d_Cut00150_Skip00_0000000000_Class0%d",iB,filter,filter,0);
    if(ptTrack==1.)
      tmpName = Form("pwgje_spec2_clustersAOD_ANTIKT03_B%d_Filter00%d_Cut01000_Skip00_clustersAOD_ANTIKT03_B0_Filter00%d_Cut01000_Skip00_0000000000_Class0%d",iB,filter,filter,0);
    if(ptTrack==2.)
      tmpName = Form("pwgje_spec2_clustersAOD_ANTIKT03_B%d_Filter00%d_Cut02000_Skip00_clustersAOD_ANTIKT03_B0_Filter00%d_Cut02000_Skip00_0000000000_Class0%d",iB,filter,filter,0);
  }
  else if(R==0.4) {
    if(ptTrack==0.15)
      tmpName = Form("pwgje_spec2_clustersAOD_ANTIKT04_B%d_Filter00%d_Cut00150_Skip00_clustersAOD_ANTIKT04_B0_Filter00%d_Cut00150_Skip00_0000000000_Class0%d",iB,filter,filter,0);
    if(ptTrack==1.)
      tmpName = Form("pwgje_spec2_clustersAOD_ANTIKT04_B%d_Filter00%d_Cut01000_Skip00_clustersAOD_ANTIKT04_B0_Filter00%d_Cut01000_Skip00_0000000000_Class0%d",iB,filter,filter,0);
    if(ptTrack==2.)
      tmpName = Form("pwgje_spec2_clustersAOD_ANTIKT04_B%d_Filter00%d_Cut02000_Skip00_clustersAOD_ANTIKT04_B0_Filter00%d_Cut02000_Skip00_0000000000_Class0%d",iB,filter,filter,0);
  }


  TList *list = (TList*)dir->Get(tmpName.Data());
  if(!list)Printf("Line:%d %s not found",__LINE__,tmpName.Data());

  return list;

}

//_______________________________________________________________________________________________________________________
TH1D *GetRawJetSpectrum(TString str, 
			Double_t areaMin, 
			Double_t centMin,
			Double_t centMax,
			Double_t ptTrack,
			Double_t R,
			Double_t ptMin,
			Double_t ptMax,
			Int_t trigHad) {


  TList *list = GetTListFromTrainOutput(str,R,ptTrack);

  THnSparse *fhnJetPtRec = (THnSparse*)list->FindObject("fhnJetPtRec");
  int centMinBin =fhnJetPtRec->GetAxis(2)->FindBin(centMin); 
  int centMaxBin =fhnJetPtRec->GetAxis(2)->FindBin(centMax)-1; 
  fhnJetPtRec->GetAxis(2)->SetRange(centMinBin,centMaxBin);     // Set Centrality bin
  // cout << "centMin: " <<  fhnJetPtRec->GetAxis(2)->GetBinLowEdge(centMinBin) << "  centMax: " <<   fhnJetPtRec->GetAxis(2)->GetBinUpEdge(centMaxBin) << endl;

  fhnJetPtRec->GetAxis(0)->SetRange(3,3);               // take all jets

  //cout << "nBinsArea: " <<   fhnJetPtRec->GetAxis(5)->GetNbins() << endl;
  int areaMinBin =fhnJetPtRec->GetAxis(5)->FindBin(areaMin);
  int areaMaxBin =fhnJetPtRec->GetAxis(5)->GetNbins();
  //cout << "areaMin: " << fhnJetPtRec->GetAxis(5)->GetBinLowEdge(areaMinBin) << endl;
  fhnJetPtRec->GetAxis(5)->SetRange(areaMinBin,areaMaxBin);     // Set Area min

  Int_t binLoPt = fhnJetPtRec->GetAxis(1)->FindBin(ptMin);
  Int_t binUpPt = fhnJetPtRec->GetAxis(1)->FindBin(ptMax)-1;

  fhnJetPtRec->GetAxis(1)->SetRange(binLoPt,binUpPt);   // Set pT range
  if(trigHad>0) {
    Double_t trigHadD = fhnJetPtRec->GetAxis(7)->GetBinLowEdge(trigHad);
    //    cout << "trigHadD: " << trigHadD << "  trigHad: " << trigHad << endl;
    fhnJetPtRec->GetAxis(7)->SetRange(trigHad,fhnJetPtRec->GetAxis(7)->GetNbins());
  }

  TH1D *fh1RawJetsCurrent = fhnJetPtRec->Projection(1,"E");
  fh1RawJetsCurrent->SetName("fh1RawJetsCurrent");
  fh1RawJetsCurrent->SetTitle("fh1RawJetsCurrent");

  return fh1RawJetsCurrent;

}

//_______________________________________________________________________________________________________________________
Double_t GetNEvents(TString str, Int_t cent) {
  //
  //Get NEvents in centrality bin
  //

  TFile *fileRawJets = TFile::Open(str.Data());
  fileRawJets->ls();
  TDirectory *dir = (TDirectory*)fileRawJets->Get("PWGJE_services");
  TList *list1 = (TList*)dir->Get("pwgje_services");

  TH2F* hTriggerCount =(TH2F*)list1->FindObject("fh2TriggerCount");
  //AliAnalysisTaskJetServices::kSelected=6
  Double_t fTriggerNorm = hTriggerCount->GetBinContent(hTriggerCount->FindBin(cent+1,6)); // scale with number of events which entered analysis

  return fTriggerNorm;

}

//_______________________________________________________________________________________________________________________
void FillHistoContent(TH1 *hTarget, TH1 *hOrig) {

  for(Int_t i = 1; i<=hOrig->GetNbinsX(); i++) {

    Int_t bin = hTarget->FindBin(hOrig->GetBinCenter(i));
    //Printf("x: %f to  %f - %f",hOrig->GetBinCenter(i),hTarget->GetXaxis()->GetBinLowEdge(bin),hTarget->GetXaxis()->GetBinUpEdge(bin));
    
    hTarget->SetBinContent(bin,hTarget->GetBinContent(bin)+hOrig->GetBinContent(i));
    Double_t error2 = hTarget->GetBinError(bin)*hTarget->GetBinError(bin) + hOrig->GetBinError(i)*hOrig->GetBinError(i);
    if(error2>0.)
      hTarget->SetBinError(bin,TMath::Sqrt(error2));
  }
}

// //_______________________________________________________________________________________________________________________
// void FillHistoContent(TH1D *hTarget, TH1D *hOrig) {

//   for(Int_t i = 1; i<hOrig->GetNbinsX(); i++) {

//     Int_t bin = hTarget->FindBin(hOrig->GetBinCenter(i));
    
//     hTarget->SetBinContent(bin,hTarget->GetBinContent(bin)+hOrig->GetBinContent(i));
//     Double_t error2 = hTarget->GetBinError(bin)*hTarget->GetBinError(bin) + hOrig->GetBinError(i)*hOrig->GetBinError(i);
//     if(error2>0.)
//       hTarget->SetBinError(bin,TMath::Sqrt(error2));
//   }
// }

TF1 *FitTruncatedGaus(TH1 *h1, double &sigma, double &sigma_error, double &mean, Double_t ns = 3.)
{
  h1->Fit("gaus");
  TF1 *f1 = h1->GetFunction("gaus");
  sigma = f1->GetParameter(2);
  mean = f1->GetParameter(1);
  h1->Fit("gaus","""","""",mean-ns*sigma,mean+ns*sigma);
  f1 =  h1->GetFunction("gaus");
  sigma = f1->GetParameter(2);
  sigma_error = f1->GetParError(2);

  return f1;
}




