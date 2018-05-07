#include <TCanvas.h>
#include <TFile.h>
#include <TH3F.h>
#include <TH1F.h>
#include <TH1D.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include <TLegend.h>
#include <TLine.h>
#include <TPad.h>

#include "plotUtils.h"
#include "utilsMV.h"
#include "utils.h"

#include <string>

std::string getCentStr(int iCent);
TGraphErrors *MakeSystGraph(TH1* hC = 0, TH1 *hS = 0, double we = 1.);
void plotCompact(std::string inputFile = "Data/photonJetFF/data_60_30_gxi0_defnFF1-final-and-systematics.root", int ifig = 1, bool isJS = 0);

void plotCompact(std::string inputFile, int ifig, bool isJS)
{
  std::string tag = "#sqrt{s_{NN}} = 5.02 TeV";

  std::string xTitle = "#xi^{jet}";
  if(ifig == 2) xTitle = "#xi^{#gamma}_{T}";

  if (isJS) xTitle = "r";

  TFile* file = 0;
  file = new TFile(inputFile.c_str());
  if (!file || file->IsZombie()) {
      Printf("This root file doesn't exist. Aborting!");
      return;
  }

  const int nCent = 4; //centrality bins
  double centMin[nCent] = {0.,10.,30.,50.};
  double centMax[nCent] = {10.,30.,50.,100.};
   
  const int ns = 2; //systems: pp=0 PbPb=1
  std::vector<std::string> strLeg = {"pp (smeared)","PbPb"};
  if (isJS) {
      strLeg = {"pp","PbPb"};
  }

  /* Kaya's instructions:
    PbPb histograms are hff_final_pbpbdata_recoreco_X_Y where X_Y = 0_20 corresponds to Cent. 0-10% (Remaining centralities are clear from context.)
    Similarly smeared pp histograms are hff_final_ppdata_srecoreco_X_Y
    PbPb / pp ratio histograms are hff_final_ratio_X_Y.

    The systematics for each data are in the histograms with the suffix "_systematics". Systematics are in absolute values (difference from the nominal).
  */

  std::vector<double> scale = {0.,2.,4.,6.};  //scale factors for xi distributions
  std::vector<double> scale2 = {0.,1.,2.,3.}; //scale factors for PbPb/pp ratios (hidden)
  if (isJS) {
      scale = {1.,10.,100.,1000.};
  }

  std::string strObs = "hff";
  if (isJS) strObs = "hjs";
   
  //Get xi histograms
  TH1 *hObs[ns][nCent];
  TH1 *hObsSys[ns][nCent];
  TGraphErrors *grObsSys[ns][nCent];
  for(int is = 0; is<ns; ++is) {
    std::string strSysTmp = "";
    if(is==0) {
        //pp smeared
        strSysTmp = "ppdata_s";
        if (isJS) strSysTmp = "ppdata_corrjs";
    }
    else if(is==1) {
        //PbPb
        strSysTmp = "pbpbdata_";
        if (isJS) strSysTmp = "pbpbdata_corrjs";
    }
    for(int ic = 0; ic<nCent; ++ic) {
      std::string histName = "";
      std::string centStr = getCentStr(ic);

      hObs[is][ic] = 0;
      hObsSys[is][ic] = 0;

      if (is == 0 && isJS) {
          if (ic != 0) continue;
          centStr = getCentStr(3);
      }

      histName = Form("%s_final_%srecoreco_%s", strObs.c_str(), strSysTmp.c_str(), centStr.c_str());
      hObs[is][ic] = dynamic_cast<TH1*>(file->Get(histName.c_str()));

      histName = Form("%s_final_%srecoreco_%s_systematics", strObs.c_str(), strSysTmp.c_str(), centStr.c_str());
      hObsSys[is][ic] = dynamic_cast<TH1*>(file->Get(histName.c_str()));

      if(hObs[is][ic]) {
        grObsSys[is][ic] = MakeSystGraph(hObs[is][ic],hObsSys[is][ic],2.);
        bool multiply = isJS;
        add_histo(hObs[is][ic],scale[ic], multiply);
        add_graph(grObsSys[is][ic],scale[ic], multiply);
      } else
        Printf("Could not find hObs is =%d ic=%d",is,ic);
    }
  }

  //get ratio histograms
  TH1 *hRatio[nCent];
  TH1 *hRatioSys[nCent];
  TGraphErrors *grRatioSys[nCent];
  for(int ic = 0; ic<nCent; ++ic) {
    std::string histName = "";
    std::string centStr = getCentStr(ic);
    histName = Form("%s_final_ratio_%s", strObs.c_str(), centStr.c_str());
    hRatio[ic] = dynamic_cast<TH1*>(file->Get(histName.c_str()));

    histName = Form("%s_final_ratio_%s_systematics", strObs.c_str(), centStr.c_str());
    hRatioSys[ic] = dynamic_cast<TH1*>(file->Get(histName.c_str()));
    
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
  if (isJS) gPad->SetLogy(1);

  std::string yTitle = Form("1/N^{jet} dN^{trk}/d%s",xTitle.c_str());
  double xMin = 0.5;
  double xMax = 4.5;
  double yMin = -0.5;
  double yMax = 11.999;
  if (isJS) {
      xMin = 0;
      xMax = 0.3;
      yMin = 0.2;
      yMax = 200000-0.001;
      yTitle = "#rho(r)";
  }
  TH1F *fr1 = DrawFrame(xMin, xMax, yMin, yMax, xTitle.c_str(),yTitle.c_str(),false);
  fr1->GetXaxis()->SetNdivisions(509);
  fr1->GetXaxis()->SetLabelSize(get_txt_size(TVirtualPad::Pad(),18.));
  fr1->GetYaxis()->SetLabelSize(get_txt_size(TVirtualPad::Pad(),18.));
  fr1->GetXaxis()->SetTitleSize(get_txt_size(TVirtualPad::Pad(),24.));
  fr1->GetYaxis()->SetTitleSize(get_txt_size(TVirtualPad::Pad(),24.));
  fr1->GetXaxis()->SetTitleOffset(0.8);
  fr1->GetYaxis()->SetTitleOffset(1.02);
  if (isJS) {
      fr1->GetYaxis()->SetTitleOffset(1.16);
  }
  fr1->GetYaxis()->SetLabelOffset(0.021);
  fr1->GetXaxis()->CenterTitle(true);
  fr1->GetYaxis()->CenterTitle(true);

  for(int i = nCent-1; i>-1; --i) {
    for(int k = (ns-1); k>-1; --k) {
      if(!hObs[k][i]) continue;

      int colorCode = i+1;

      grObsSys[k][i]->SetFillColor(GetFillColor(colorCode));
      grObsSys[k][i]->SetLineColor(10);
      grObsSys[k][i]->SetMarkerColor(GetColor(colorCode));
      grObsSys[k][i]->SetMarkerStyle(GetMarker(k));
      grObsSys[k][i]->SetMarkerSize(1.3);

      if(k==0)
        grObsSys[k][i]->SetMarkerStyle(kOpenCross);
      else
        grObsSys[k][i]->SetMarkerStyle(kFullCross);

      if(k==0) {
        grObsSys[k][i]->SetFillStyle(0);
        grObsSys[k][i]->SetLineColor(GetColor(colorCode));
        grObsSys[k][i]->SetLineWidth(1);
        grObsSys[k][i]->SetFillColor(10);
      }
      if(k==1) grObsSys[k][i]->SetFillStyle(3002);//3002);

      grObsSys[k][i]->Draw("2");
    }
  }
  
  for(int i = nCent-1; i>-1; --i) {
    int colorCode = i+1;
    for(int k = (ns-1); k>-1; --k) {
      if(!hObs[k][i]) continue;
      
      hObs[k][i]->SetLineColor(GetColor(colorCode));
      hObs[k][i]->SetMarkerColor(GetColor(colorCode));

      if(k==0) {
          hObs[k][i]->SetMarkerStyle(kOpenCross);
      }
      else
          hObs[k][i]->SetMarkerStyle(kFullCross);

      hObs[k][i]->SetMarkerSize(1.3);

      hObs[k][i]->Draw("same EX0");
    }

    std::string strCent = Form("%.0f-%.0f",centMin[i],centMax[i]);
    strCent+="%";
    std::string scaleStr = Form("(+%.0f)",scale[i]);
    if (isJS) {
        double scaleLog = TMath::Log10(scale[i]);
        scaleStr = Form("(x10^{%.0f})",scaleLog);
    }
    double latex1X = 0;
    double latex1Y = 0;
    double latex2X = 0;
    double latex2Y = 0;
    if(i==0) {
        latex1X = 0.17;
        latex1Y = 0.10;
    }
    else if (i==1) {
        latex1X = 0.17;
        latex1Y = 0.28;
        latex2X = 0.17;
        latex2Y = 0.24;
    }
    else if (i==2) {
        latex1X = 0.17;
        latex1Y = 0.43;
        latex2X = 0.17;
        latex2Y = 0.39;
    }
    else if (i==3) {
        latex1X = 0.17;
        latex1Y = 0.58;
        latex2X = 0.17;
        latex2Y = 0.54;
    }
    if (isJS) {
        if(i==0) {
            latex1X = 0.18;
            latex1Y = 0.20;
        }
        else if (i==1) {
            latex1X = 0.18;
            latex1Y = 0.36;
            latex2X = 0.18;
            latex2Y = 0.31;
        }
        else if (i==2) {
            latex1X = 0.18;
            latex1Y = 0.52;
            latex2X = 0.18;
            latex2Y = 0.47;
        }
        else if (i==3) {
            latex1X = 0.18;
            latex1Y = 0.67;
            latex2X = 0.18;
            latex2Y = 0.62;
        }
    }

    DrawLatex(latex1X, latex1Y, strCent.c_str(),txtSize,GetColor(colorCode));
    if (i != 0) {
        DrawLatex(latex2X, latex2Y, scaleStr.c_str(),txtSize,GetColor(colorCode));
    }
  }

  TGraphErrors *grSystForLeg[2]; //clones for legend which we want in black
  grSystForLeg[0] = dynamic_cast<TGraphErrors*>(grObsSys[0][0]->Clone("grSystForLeg0"));
  grSystForLeg[1] = dynamic_cast<TGraphErrors*>(grObsSys[1][0]->Clone("grSystForLeg0"));
  for(int il = 0; il<2; ++il) {
    if(il==0) grSystForLeg[il]->SetLineColor(1);
    grSystForLeg[il]->SetMarkerColor(1);
    grSystForLeg[il]->SetFillColor(kGray+1);
  }
  
  TLegend *leg1 = 0;
  double leg1Xmin = 0.5;
  double leg1Xmax = 0.8;
  double leg1Ymin = 0.02;
  double leg1Ymax = 0.15;
  if (ifig == 2) {
      leg1Xmin = 0.63;
      leg1Xmax = 0.93;
      leg1Ymin = 0.02;
      leg1Ymax = 0.15;
  }
  if (isJS) {
      leg1Xmin = 0.2;
      leg1Xmax = 0.5;
      leg1Ymin = 0.02;
      leg1Ymax = 0.15;
  }
  leg1 = CreateLegend(leg1Xmin, leg1Xmax, leg1Ymin, leg1Ymax, "",txtSize);
  leg1->AddEntry(grSystForLeg[1],strLeg[1].c_str(),"pf");
  leg1->AddEntry(grSystForLeg[0],strLeg[0].c_str(),"pf");
  leg1->Draw();

  double cmsTextX = 0.83;
  double cmsTextY = 0.86;
  if (isJS) {
      cmsTextX = 0.20;
      cmsTextY = 0.86;
  }
  DrawLatex(cmsTextX, cmsTextY, "#bf{CMS}",get_txt_size(TVirtualPad::Pad(),20.));
  
  if (!isJS) {
      DrawLatex(0.18,0.86,Form("p_{T}^{#gamma} > 60 GeV/c, |#eta^{#gamma}| < 1.44, #Delta#phi_{j#gamma} > #frac{7#pi}{8}"),txtSize);
      DrawLatex(0.18,0.79,Form("anti-k_{T} jet R = 0.3, p_{T}^{jet} > 30 GeV/c, |#eta^{jet}| < %.1f",1.6),txtSize);
      DrawLatex(0.18,0.72,Form("p_{T}^{trk} > 1 GeV/c"),txtSize);
  }
  else {
      DrawLatex(0.35,0.86,Form("p_{T}^{#gamma} > 60 GeV/c, |#eta^{#gamma}| < 1.44, #Delta#phi_{j#gamma} > #frac{7#pi}{8}"),txtSize);
      DrawLatex(0.39,0.79,Form("anti-k_{T} jet R = 0.3, p_{T}^{jet} > 30 GeV/c"),txtSize);
      DrawLatex(0.53,0.72,Form("|#eta^{jet}| < %.1f, p_{T}^{trk} > 1 GeV/c", 1.6),txtSize);
  }


  TPad *pad=new TPad("padUp","padUp",0.,0.935,1.,1.);
  pad->Draw();
  pad->cd();
  DrawLatex(0.21,0.2,Form("%s, PbPb 404  #mub^{-1}, pp 27.4 pb^{-1}",tag.c_str()),get_txt_size(TVirtualPad::Pad(),18.));

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

  double yMinRatio = 0;
  double yMaxRatio = 5.2;
  if (isJS) {
      yMinRatio = 0;
      yMaxRatio = 5.2;
  }
  TH1F *fr2 = DrawFrame(xMin, xMax, yMinRatio, yMaxRatio, xTitle.c_str(), "PbPb / pp" , false);
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

  double xText2 = xMin - 0.2;
  if (isJS) xText2 = xMin - 0.01;
  text2.DrawLatex(xText2,0.-0.1,"0");
  text2.DrawLatex(xText2,1.-0.1,"1");
  text2.DrawLatex(xText2,2.-0.1,"2");
  text2.DrawLatex(xText2,3.-0.1,"3");
  text2.DrawLatex(xText2,4.-0.1,"4");
  text2.SetTextColor(1);

  for(int i = 0; i<nCent; ++i) {
    std::string strCent = Form("%.0f-%.0f",centMin[i],centMax[i]);
    strCent+="%";
    int colorCode = i+1;
    std::string strTmp = Form("%s (+%.0f)", strCent.c_str(), scale2[i]);
    if (i == 0) {
        strTmp = strCent.c_str();
        DrawLatex(0.18,0.36,strTmp.c_str(),txtSize,GetColor(colorCode));
    }
    else if (i == 1) {
        DrawLatex(0.18,0.52,strTmp.c_str(),txtSize,GetColor(colorCode));
    }
    else if (i == 2) {
        DrawLatex(0.18,0.70,strTmp.c_str(),txtSize,GetColor(colorCode));
    }
    else if (i == 3) {
        DrawLatex(0.18,0.87,strTmp.c_str(),txtSize,GetColor(colorCode));
    }
  }

  /*
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
  */

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

    if(i>1) {
        hRatio[i]->SetMarkerSize(1.7);
        grRatioSys[i]->SetMarkerSize(1.7);
    }
    else     {
        hRatio[i]->SetMarkerSize(1.2);
        grRatioSys[i]->SetMarkerSize(1.2);
    }
    
    hRatio[i]->Draw("same E X0");
  }

  /*
  TLegend *leg2 = 0;
  if(ifig==1) leg2 = CreateLegend(0.18,0.96,0.88,0.96,"",txtSize);
  if(ifig==2) leg2 = CreateLegend(0.18,0.96,0.88,0.96,"",txtSize);
  leg2->SetNColumns(4);
  for(int i = 0; i<nCent; ++i) {
    std::string strCent = Form("%.0f-%.0f",centMin[i],centMax[i]);
    strCent+="%";
    leg2->AddEntry(grRatioSys[i],strCent.c_str(),"pf");
  }
  leg2->Draw();
  */

  std::string obsCnv = (isJS) ? "JS" : "FF";
  c1->SaveAs(Form("photonjet%s_compact_fig%d.png", obsCnv.c_str(), ifig));
  c1->SaveAs(Form("photonjet%s_compact_fig%d.pdf", obsCnv.c_str(), ifig));
}

int main(int argc, char** argv)
{
    if (argc == 4) {
        plotCompact(argv[1], std::atoi(argv[2]), std::atoi(argv[3]));
        return 0;
    }
    else if (argc == 3) {
        plotCompact(argv[1], std::atoi(argv[2]));
        return 0;
    }
    else if (argc == 2) {
        plotCompact(argv[1]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./plotCompact.exe <inputFile> <figureIndex>"
                << std::endl;
        return 1;
    }
}

std::string getCentStr(int iCent)
{
    if(iCent == 0) {
        return "0_20";
    }
    else if(iCent == 1) {
        return "20_60";
    }
    else if(iCent == 2) {
        return "60_100";
    }
    else if(iCent == 3) {
        return "100_200";
    }
    else {
        return "";
    }
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
    double yerr = std::fabs(hS->GetBinContent(ic));

    gr->SetPoint(gr->GetN(),x,y);
    gr->SetPointError(gr->GetN()-1,xerr,yerr);
  }
  return gr;
}
