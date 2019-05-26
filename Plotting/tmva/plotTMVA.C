/*
 * macro to read TMVA output and plot distribution of input variables
 * derived from https://root.cern.ch/doc/v612/variables_8cxx_source.html
 */
#include "TMVA/tmvaglob.h"
#include "TMVA/Config.h"

#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>

#include "../../Utilities/systemUtil.h"
#include "../../Utilities/interface/ArgumentParser.h"

std::vector<std::string> argOptions;

void convertImage(TCanvas* c, std::string fileName);
void plotTMVA(std::string inputFile, std::string dirPath, std::string varName, std::string outputFile)
{
    std::cout << "##### Parameters #####" << std::endl;
    std::cout << "running plotTMVA()" << std::endl;
    std::cout << "inputFile = " << inputFile.c_str() << std::endl;
    std::cout << "directory in file = " << dirPath.c_str() << std::endl;
    std::cout << "variable name = " << varName.c_str() << std::endl;
    std::cout << "output picture file name = " << outputFile.c_str() << std::endl;
    std::cout << "##### Parameters - END #####" << std::endl;

    bool isRegression = false;
    bool useTMVAStyle = false;

    // set style and remove existing canvas'
    TMVA::TMVAGlob::Initialize(useTMVAStyle);

    // checks if file with name "fin" is already open, and if not opens one
    TFile* file = 0;
    file = TMVA::TMVAGlob::OpenFile(inputFile.c_str());

    TH1D* hSig = 0;
    TH1D* hBkg = 0;

    std::string histPathSig = Form("%s/%s__Signal_Id", dirPath.c_str(), varName.c_str());
    std::string histPathBkg = Form("%s/%s__Background_Id", dirPath.c_str(), varName.c_str());

    hSig = (TH1D*)file->Get(histPathSig.c_str());
    hBkg = (TH1D*)file->Get(histPathBkg.c_str());

    if (hSig == 0) {
        std::cout << "Signal histogram not found : " << histPathSig.c_str() << std::endl;
        std::cout << "exiting" << std::endl;
        return;
    }
    if (hBkg == 0) {
        std::cout << "Background histogram not found : " << histPathBkg.c_str() << std::endl;
        std::cout << "exiting" << std::endl;
        return;
    }

    int nPlots = 1;

    // define Canvas layout here!
    // default setting
    int nPadsX;
    int nPadsY;
    int width; // size of canvas
    int height;
    switch (nPlots) {
    case 1:
        nPadsX = 1; nPadsY = 1; width = 800; height = 0.90*width; break;
    case 2:
        nPadsX = 2; nPadsY = 1; width = 1600; height = 0.50*width; break;
    case 3:
        nPadsX = 3; nPadsY = 1; width = 1600; height = 0.4*width; break;
    case 4:
        nPadsX = 2; nPadsY = 2; width = 600; height = width; break;
    default:
        nPadsX = 3; nPadsY = 2; width = 1600; height = 0.55*width; break;
    }

    int nPadsCnv = nPadsX * nPadsY ;

    // counter plotTMVA
    int countCanvas = 0;
    int countPad    = 0;

    // loop over all objects in directory
    TCanvas* cnv = 0;
    bool createNewFig = false;

    // create new canvas
    if (countPad%nPadsCnv==0) {
        ++countCanvas;
        cnv = new TCanvas(Form("canvas%d", countCanvas), "",
                countCanvas*50+50, countCanvas*20, width, height);
        cnv->Divide(nPadsX,nPadsY);
        cnv->Draw();
    }

    TPad* cPad = 0;
    cPad = (TPad*)cnv->cd(countPad++%nPadsCnv+1);

    // this is set but not stored during plot creation in MVA_Factory
    TMVA::TMVAGlob::SetSignalAndBackgroundStyle(hSig, (isRegression ? 0 : hBkg));

    TMVA::TMVAGlob::SetFrameStyle(hSig, 1.2);

    // normalise both signal and background
    if (!isRegression) TMVA::TMVAGlob::NormalizeHists(hSig, hBkg);
    else {
        // change histogram title for target
        std::string nme = hSig->GetName();
        if (nme.find("_target") != std::string::npos) {
            std::string titleTmp = hSig->GetTitle();
            hSig->SetTitle(replaceAll(titleTmp, "Input variable", "Regression target").c_str());
        }
    }

    // finally plot and overlay
    double sc = 1.1;
    if (countPad == 1) sc = 1.3;
    hSig->SetMaximum(TMath::Max(hSig->GetMaximum(), hBkg->GetMaximum())*sc);
    hSig->Draw("hist");
    cPad->SetLeftMargin(0.17);

    hSig->GetYaxis()->SetTitleOffset(1.70);
    if (!isRegression) {
        hBkg->Draw("histsame");
        std::string titleTmpY = "(1/N) " + std::string(hSig->GetYaxis()->GetTitle());
        hSig->GetYaxis()->SetTitle(titleTmpY.c_str()); // histograms are normalised
    }

    // Draw legend
    if (countPad == 1 && !isRegression) {
        TLegend *legend= new TLegend(cPad->GetLeftMargin(),
                1-cPad->GetTopMargin()-.15,
                cPad->GetLeftMargin()+.4,
                1-cPad->GetTopMargin());
        legend->SetFillStyle(1);
        legend->AddEntry(hSig,"Signal","F");
        legend->AddEntry(hBkg,"Background","F");
        legend->SetBorderSize(1);
        legend->SetMargin(0.3);
        legend->Draw("same");
    }

    // redraw axes
    hSig->Draw("sameaxis");

    // text for overflows
    int    nbin = hSig->GetNbinsX();
    double dxu  = hSig->GetBinWidth(0);
    double dxo  = hSig->GetBinWidth(nbin+1);
    std::string uoflow = "";
    if (isRegression) {
        uoflow = Form("U/O-flow: %.1f%% / %.1f%%",
                hSig->GetBinContent(0)*dxu*100, hSig->GetBinContent(nbin+1)*dxo*100);
    }
    else {
        uoflow = Form("U/O-flow (S,B): (%.1f, %.1f)%% / (%.1f, %.1f)%%",
                hSig->GetBinContent(0)*dxu*100, hBkg->GetBinContent(0)*dxu*100,
                hSig->GetBinContent(nbin+1)*dxo*100, hBkg->GetBinContent(nbin+1)*dxo*100);
    }

    TText* text = new TText(0.98, 0.14, uoflow.c_str());
    text->SetNDC();
    text->SetTextSize(0.040);
    text->SetTextAngle(90);
    text->AppendPad();

    // save canvas to file
    if (countPad%nPadsCnv==0) {
        TMVA::TMVAGlob::plot_logo();
        /*
       TMVA::TMVAGlob::imgconv(canv, fname);
         */
        convertImage(cnv, outputFile.c_str());
        createNewFig = false;
    }
    else {
        createNewFig = true;
    }

    if (createNewFig) {
        TMVA::TMVAGlob::plot_logo();
        /*
       TMVA::TMVAGlob::imgconv(canv, fname);
         */
        convertImage(cnv, outputFile.c_str());
        createNewFig = false;
    }

    std::cout << "running plotTMVA() - END" << std::endl;
}

int main(int argc, char** argv)
{
    std::vector<std::string> argStr = ArgumentParser::ParseParameters(argc, argv);
    int nArgStr = argStr.size();

    argOptions = ArgumentParser::ParseOptions(argc, argv);

    if (nArgStr == 5) {
        plotTMVA(argv[1], argv[2], argv[3], argv[4]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./plotTMVA.exe <inputFile> <directory in file> <variable name> <output picture file name>"
                << std::endl;
        return 1;
    }
}

void convertImage(TCanvas* c, std::string fileName)
{
    // create directory if not existing
    TString f = fileName.c_str();
    std::string dir = f.Remove(f.Last('/'), f.Length() - f.Last('/')).Data();
    gSystem->mkdir(dir.c_str());

    std::string pngName = fileName + ".png";
    std::string gifName = fileName + ".gif";
    std::string epsName = fileName + ".eps";
    std::string pdfName = fileName + ".pdf";
    c->cd();

    // create eps (other option: c->Print(epsName))
    if (TMVA::gConfig().fVariablePlotting.fUsePaperStyle) {
        c->Print(epsName.c_str());
    }
    else {
        //c->Print(epsName.c_str());
        c->Print(pdfName.c_str());
        c->Print(pngName.c_str());
        //c->Print(gifName.c_str());
    }
}
