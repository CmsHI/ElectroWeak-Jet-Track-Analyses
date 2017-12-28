#include "TCanvas.h"
#include "TFile.h"
#include "TH3F.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TGraphAsymmErrors.h"
#include "TLegend.h"

#include "plotUtils.C"
#include "utilsMV.C"
#include "utils.C"

TGraphErrors *MakeSystGraph(TH1* hC = 0, TH1 *hS = 0, double we = 1.);

void plotHIN16014(int ifig = 1) {
  
  TString tag = "#sqrt{s_{NN}} = 5.02 TeV";

  TString strFig1 = "data/photonJetFF_Data_xijet_paper.root";
  TString strFig2 = "data/photonJetFF_Data_xigamma_paper.root";

  TString xTitle = "#xi^{jet}";
  if(ifig==2) xTitle = "#xi^{#gamma}_{T}";

  TFile *f;
  if(ifig==1)      f = new TFile(strFig1.Data());
  else if(ifig==2) f = new TFile(strFig2.Data());
  else {Printf("This root file doesn't exist. Aborting!"); return;}

  const int nCent = 4; //centrality bins
  double centMin[nCent] = {0.,10.,30.,50.};
  double centMax[nCent] = {10.,30.,50.,100.};
   
  const int ns = 2; //systems: pp=0 PbPb=1
  TString strLeg[ns] = {"pp (smeared)","PbPb"};

  /* Kaya's instructions:
    PbPb histograms are hff_final_pbpbdata_recoreco_X_Y where X_Y = 0_20 corresponds to Cent. 0-10% (Remaining centralities are clear from context.)
    Similarly smeared pp histograms are hff_final_ppdata_srecoreco_X_Y
    PbPb / pp ratio histograms are hff_final_ratio_X_Y.

    The systematics for each data are in the histograms with the suffix "_systematics". Systematics are in absolute values (difference from the nominal).
  */

  double scale[nCent] = {0.,2.,4.,6.};  //scale factors for xi distributions
  double scale2[nCent] = {0.,1.,2.,3.}; //scale factors for PbPb/pp ratios (hidden)
   
  //Get xi histograms
  TH1 *hXi[ns][nCent];
  TH1 *hXiSys[ns][nCent];
  TGraphErrors *grXiSys[ns][nCent];
  for(int is = 0; is<ns; ++is) {
    TString strSysTmp;
    if(is==0) //pp smeared
      strSysTmp = "ppdata_s";
    else if(is==1) //PbPb
      strSysTmp = "pbpbdata_";
    for(int ic = 0; ic<nCent; ++ic) {
      TString histName;
      if(ic==0) histName = Form("hff_final_%srecoreco_0_20",strSysTmp.Data());
      else if(ic==1) histName = Form("hff_final_%srecoreco_20_60",strSysTmp.Data());
      else if(ic==2) histName = Form("hff_final_%srecoreco_60_100",strSysTmp.Data());
      else if(ic==3) histName = Form("hff_final_%srecoreco_100_200",strSysTmp.Data());
      hXi[is][ic] = dynamic_cast<TH1*>(f->Get(histName.Data()));

      if(ic==0) histName = Form("hff_final_%srecoreco_0_20_systematics",strSysTmp.Data());
      else if(ic==1) histName = Form("hff_final_%srecoreco_20_60_systematics",strSysTmp.Data());
      else if(ic==2) histName = Form("hff_final_%srecoreco_60_100_systematics",strSysTmp.Data());
      else if(ic==3) histName = Form("hff_final_%srecoreco_100_200_systematics",strSysTmp.Data());
      hXiSys[is][ic] = dynamic_cast<TH1*>(f->Get(histName.Data()));

      if(hXi[is][ic]) {
        grXiSys[is][ic] = MakeSystGraph(hXi[is][ic],hXiSys[is][ic],2.);
        add_histo(hXi[is][ic],scale[ic]);
        add_graph(grXiSys[is][ic],scale[ic]);
      } else
        Printf("Could not find hXi is =%d ic=%d",is,ic);
    }
  }

  //get ratio histograms
  TH1 *hRatio[nCent];
  TH1 *hRatioSys[nCent];
  TGraphErrors *grRatioSys[nCent];
  for(int ic = 0; ic<nCent; ++ic) {
    TString histName;
    if(ic==0) histName = Form("hff_final_ratio_0_20");
    else if(ic==1) histName = Form("hff_final_ratio_20_60");
    else if(ic==2) histName = Form("hff_final_ratio_60_100");
    else if(ic==3) histName = Form("hff_final_ratio_100_200");
    hRatio[ic] = dynamic_cast<TH1*>(f->Get(histName.Data()));

    if(ic==0) histName = Form("hff_final_ratio_0_20_systematics");
    else if(ic==1) histName = Form("hff_final_ratio_20_60_systematics");
    else if(ic==2) histName = Form("hff_final_ratio_60_100_systematics");
    else if(ic==3) histName = Form("hff_final_ratio_100_200_systematics");
    hRatioSys[ic] = dynamic_cast<TH1*>(f->Get(histName.Data()));
    
    grRatioSys[ic] = MakeSystGraph(hRatio[ic],hRatioSys[ic],2.);

    add_histo(hRatio[ic],scale2[ic]);
    add_graph(grRatioSys[ic],scale2[ic]);
  }

  TCanvas *c1 = new TCanvas("c1","c1: xi",480,750);

  TPad *pad1=new TPad("pad1","pad1",0.,0.45,1.,1.);
  pad1->Draw();
  pad1->cd();

  double txtSize = get_txt_size(TVirtualPad::Pad(),18.);

  gPad->SetLeftMargin(0.15);
  gPad->SetBottomMargin(0.);
  gPad->SetRightMargin(0.05);
  gPad->SetTopMargin(0.07);

  TH1F *fr1 = DrawFrame(0.5,4.5,-0.5,10.5,xTitle.Data(),Form("1/N^{jet} dN^{trk}/d%s",xTitle.Data()),0.1,false);
  fr1->GetXaxis()->SetNdivisions(509);
  fr1->GetXaxis()->SetLabelSize(get_txt_size(TVirtualPad::Pad(),18.));
  fr1->GetYaxis()->SetLabelSize(get_txt_size(TVirtualPad::Pad(),18.));
  fr1->GetXaxis()->SetTitleSize(get_txt_size(TVirtualPad::Pad(),24.));
  fr1->GetYaxis()->SetTitleSize(get_txt_size(TVirtualPad::Pad(),24.));
  fr1->GetXaxis()->SetTitleOffset(0.8);
  fr1->GetYaxis()->SetTitleOffset(1.02);
  fr1->GetYaxis()->SetLabelOffset(0.021);
  fr1->GetXaxis()->CenterTitle(true);
  fr1->GetYaxis()->CenterTitle(true);

  for(int i = nCent-1; i>-1; --i) {
    for(int k = (ns-1); k>-1; --k) {
      if(!hXi[k][i]) continue;

      int colorCode = i+1;

      grXiSys[k][i]->SetFillColor(GetFillColor(colorCode));
      grXiSys[k][i]->SetLineColor(10);
      grXiSys[k][i]->SetMarkerColor(GetColor(colorCode));
      grXiSys[k][i]->SetMarkerStyle(GetMarker(k));
      grXiSys[k][i]->SetMarkerSize(1.3);

      if(k==0)
        grXiSys[k][i]->SetMarkerStyle(kOpenCross);
      else
        grXiSys[k][i]->SetMarkerStyle(kFullCross);

      if(k==0) {
        grXiSys[k][i]->SetFillStyle(0);
        grXiSys[k][i]->SetLineColor(GetColor(colorCode));
        grXiSys[k][i]->SetLineWidth(1);
        grXiSys[k][i]->SetFillColor(10);
      }
      if(k==1) grXiSys[k][i]->SetFillStyle(3002);//3002);

      grXiSys[k][i]->Draw("2");
    }
  }
  
  for(int i = nCent-1; i>-1; --i) {
    int colorCode = i+1;
    for(int k = (ns-1); k>-1; --k) {
      if(!hXi[k][i]) continue;
      
      hXi[k][i]->SetLineColor(GetColor(colorCode));
      hXi[k][i]->SetMarkerColor(GetColor(colorCode));

      if(k==0) {
          hXi[k][i]->SetMarkerStyle(kOpenCross);
      }
      else
          hXi[k][i]->SetMarkerStyle(kFullCross);

      hXi[k][i]->SetMarkerSize(1.3);

      hXi[k][i]->Draw("same EX0");
    }

    TString strCent = Form("%.0f-%.0f",centMin[i],centMax[i]);
    strCent+="%";
    if(i==0) {
      DrawLatex(0.17,0.14,strCent.Data(),txtSize,GetColor(colorCode));
      DrawLatex(0.17,0.1,Form("(+%.0f)",scale[i]),txtSize,GetColor(colorCode));
    }
    if(i==1) {
      DrawLatex(0.17,0.31,strCent.Data(),txtSize,GetColor(colorCode));
      DrawLatex(0.17,0.27,Form("(+%.0f)",scale[i]),txtSize,GetColor(colorCode));
    }
    if(i==2) {
      DrawLatex(0.17,0.48,strCent.Data(),txtSize,GetColor(colorCode));
      DrawLatex(0.17,0.44,Form("(+%.0f)",scale[i]),txtSize,GetColor(colorCode));
    }
    if(i==3) {
      DrawLatex(0.17,0.65,strCent.Data(),txtSize,GetColor(colorCode));
      DrawLatex(0.17,0.61,Form("(+%.0f)",scale[i]),txtSize,GetColor(colorCode));
    }
  }

  TGraphErrors *grSystForLeg[2]; //clones for legend which we want in black
  grSystForLeg[0] = dynamic_cast<TGraphErrors*>(grXiSys[0][0]->Clone("grSystForLeg0"));
  grSystForLeg[1] = dynamic_cast<TGraphErrors*>(grXiSys[1][0]->Clone("grSystForLeg0"));
  for(int il = 0; il<2; ++il) {
    if(il==0) grSystForLeg[il]->SetLineColor(1);
    grSystForLeg[il]->SetMarkerColor(1);
    grSystForLeg[il]->SetFillColor(kGray+1);
  }
  
  TLegend *leg1;
  if(ifig==1) leg1 = CreateLegend(0.5,0.8,0.02,0.15,"",txtSize);
  if(ifig==2) leg1 = CreateLegend(0.63,0.93,0.02,0.15,"",txtSize);
  leg1->AddEntry(grSystForLeg[1],strLeg[1].Data(),"pf");
  leg1->AddEntry(grSystForLeg[0],strLeg[0].Data(),"pf");
  leg1->Draw();
  
  double xtext = 0.18;

  DrawLatex(0.83,0.86,"#bf{CMS}",get_txt_size(TVirtualPad::Pad(),20.));
  
  DrawLatex(xtext,0.86,Form("p_{T}^{#gamma} > 60 GeV/c, |#eta^{#gamma}| < 1.44, #Delta#phi_{j#gamma} > #frac{7#pi}{8}"),txtSize);
  DrawLatex(xtext,0.79,Form("anti-k_{T} jet R = 0.3, p_{T}^{jet} > 30 GeV/c, |#eta^{jet}| < %.1f",1.6),txtSize);
  DrawLatex(xtext,0.72,Form("p_{T}^{trk} > 1 GeV/c"),txtSize);

  TPad *pad=new TPad("padUp","padUp",0.,0.935,1.,1.);
  pad->Draw();
  pad->cd();
  DrawLatex(0.21,0.2,Form("%s, PbPb 404  #mub^{-1}, pp 27.4 pb^{-1}",tag.Data()),get_txt_size(TVirtualPad::Pad(),18.));

  //----------------------------------------------------------------------------------------
  //PbPb/pp ratios
  //----------------------------------------------------------------------------------------

  c1->cd();
  
  TPad *pad2=new TPad("pad2","pad2",0.,0.,1.,0.45);
  pad2->Draw();
  pad2->cd();

  gPad->SetLeftMargin(0.15);
  gPad->SetBottomMargin(0.17);
  gPad->SetRightMargin(0.05);
  gPad->SetTopMargin(0.);

  TH1F *fr2 = DrawFrame(0.5,4.5,0.,5.2,xTitle.Data(),"PbPb / pp",0.1,false);
  fr2->GetXaxis()->SetNdivisions(509);
  fr2->GetYaxis()->SetNdivisions(509);
  fr2->GetXaxis()->SetLabelSize(get_txt_size(TVirtualPad::Pad(),18.));
  fr2->GetYaxis()->SetLabelSize(0.0);//get_txt_size(TVirtualPad::Pad(),18.));
  fr2->GetXaxis()->SetTitleSize(get_txt_size(TVirtualPad::Pad(),24.));
  fr2->GetYaxis()->SetTitleSize(get_txt_size(TVirtualPad::Pad(),24.));
  fr2->GetXaxis()->SetTitleOffset(1.0);
  fr2->GetYaxis()->SetTitleOffset(0.88);
  fr2->GetXaxis()->CenterTitle(true);
  fr2->GetYaxis()->CenterTitle(true);

  fr2->GetYaxis()->SetLabelColor(10);

  txtSize = get_txt_size(TVirtualPad::Pad(),18.);

  TLatex text2; //drawer for y-axis of ratio lower panel
  text2.SetTextAlign(11);
  text2.SetTextSize(txtSize);
  text2.SetTextFont(42);
  text2.SetTextColor(1);

  text2.DrawLatex(0.5-0.2,0.-0.1,"0");
  text2.SetTextColor(GetColor(1));
  text2.DrawLatex(0.5-0.2,1.-0.1,"1");
  text2.DrawLatex(4.5+0.1,2.-0.1,"2");
  text2.SetTextColor(GetColor(2));
  text2.DrawLatex(0.5-0.2,2.-0.1,"1");
  text2.DrawLatex(4.5+0.1,3.-0.1,"2");
  text2.SetTextColor(GetColor(3));
  text2.DrawLatex(0.5-0.2,3.-0.1,"1");
  text2.DrawLatex(4.5+0.1,4.-0.1,"2");
  text2.SetTextColor(GetColor(4));
  text2.DrawLatex(0.5-0.2,4.-0.1,"1");
  text2.DrawLatex(4.5+0.1,5.-0.1,"2");
  text2.SetTextColor(1);
  //text2.DrawLatex(0.5-0.2,5.-0.1,"2");

  //the y-axis breakers
  TLine *ld1 = new TLine(0.5-0.1,1.4,0.5+0.08,1.5);
  ld1->SetLineWidth(1);
  ld1->SetLineColor(1);
  ld1->Draw();

  TLine *ld2 = new TLine(0.5-0.1,1.4+0.06,0.5+0.08,1.5+0.06);
  ld2->SetLineWidth(1);
  ld2->SetLineColor(1);
  ld2->Draw();

  TLine *ld3 = new TLine(0.5-0.1,2.4,0.5+0.08,2.5);
  ld3->SetLineWidth(1);
  ld3->SetLineColor(1);
  ld3->Draw();

  TLine *ld4 = new TLine(0.5-0.1,2.4+0.06,0.5+0.08,2.5+0.06);
  ld4->SetLineWidth(1);
  ld4->SetLineColor(1);
  ld4->Draw();

  TLine *ld5 = new TLine(0.5-0.1,3.4,0.5+0.08,3.5);
  ld5->SetLineWidth(1);
  ld5->SetLineColor(1);
  ld5->Draw();

  TLine *ld6 = new TLine(0.5-0.1,3.4+0.06,0.5+0.08,3.5+0.06);
  ld6->SetLineWidth(1);
  ld6->SetLineColor(1);
  ld6->Draw();

  for(int i = 0; i<nCent; ++i) {
    Printf("scale2[%d] = %f",i,scale2[i]);
    TLine *l11 = new TLine(fr2->GetXaxis()->GetXmin(),scale2[i]+1.,fr2->GetXaxis()->GetXmax(),scale2[i]+1.);
    l11->SetLineStyle(2);
    l11->SetLineColor(1);
    l11->Draw();
  }
  
  for(int i = nCent-1; i>-1; --i) {
    int colorCode = i+1;
    grRatioSys[i]->SetFillColor(GetFillColor(colorCode));
    grRatioSys[i]->SetFillStyle(3002);
    grRatioSys[i]->SetLineColor(10);
    grRatioSys[i]->SetMarkerColor(GetColor(colorCode));
    grRatioSys[i]->SetMarkerStyle(GetMarker(i));
    grRatioSys[i]->SetMarkerSize(1.);
    grRatioSys[i]->Draw("2");
  }

  for(int i = nCent-1; i>-1; --i) {
    int colorCode = i+1;
    hRatio[i]->SetLineColor(GetColor(colorCode));
    hRatio[i]->SetLineWidth(2);
    hRatio[i]->SetMarkerColor(GetColor(colorCode));
    hRatio[i]->SetMarkerStyle(GetMarker(i));

    if(i>1) hRatio[i]->SetMarkerSize(1.5);
    else     hRatio[i]->SetMarkerSize(1.2);
    
    hRatio[i]->Draw("same E X0");
  }

  TLegend *leg2;
  if(ifig==1) leg2 = CreateLegend(0.18,0.96,0.88,0.96,"",txtSize);
  if(ifig==2) leg2 = CreateLegend(0.18,0.96,0.88,0.96,"",txtSize);
  leg2->SetNColumns(4);
  for(int i = 0; i<nCent; ++i) {
    TString strCent = Form("%.0f-%.0f",centMin[i],centMax[i]);
    strCent+="%";
    leg2->AddEntry(grRatioSys[i],strCent.Data(),"pf");  
  }
  leg2->Draw();

  c1->SaveAs(Form("HIN16014Fig%d.png",ifig));
  c1->SaveAs(Form("HIN16014Fig%d.pdf",ifig));
}

TGraphErrors *MakeSystGraph(TH1* hC, TH1 *hS, double we) {

  int nbinsC = hC->GetNbinsX();
  int nbinsS = hS->GetNbinsX();

  if(nbinsC!=nbinsS) {
    Printf(">>MakeSystGraph: number of bins on x-axis not the same!!");
    return 0;
  }

  TGraphErrors *gr = new TGraphErrors();
  for(int ic = 1; ic<=nbinsC; ++ic) {
    double x = hC->GetXaxis()->GetBinCenter(ic);
    double y = hC->GetBinContent(ic);

    double xerr = hS->GetXaxis()->GetBinWidth(ic)/2./we;
    double yerr = abs(hS->GetBinContent(ic));

    gr->SetPoint(gr->GetN(),x,y);
    gr->SetPointError(gr->GetN()-1,xerr,yerr);
  }
  return gr;
}
