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
#include "../../Utilities/fileUtil.h"
#include "../../Utilities/interface/ArgumentParser.h"

std::vector<std::string> argOptions;

void convertImage(TCanvas* c, std::string fileName);
void plotTMVA(std::string inputFile, std::string dirPath, std::string outputDir)
{
    std::cout << "##### Parameters #####" << std::endl;
    std::cout << "running plotTMVA()" << std::endl;
    std::cout << "inputFile = " << inputFile.c_str() << std::endl;
    std::cout << "directory in file = " << dirPath.c_str() << std::endl;
    std::cout << "output directory for plots = " << outputDir.c_str() << std::endl;
    std::cout << "##### Parameters - END #####" << std::endl;

    std::string variablesStr = (ArgumentParser::ParseOptionInputSingle("--variables", argOptions).size() > 0) ?
            ArgumentParser::ParseOptionInputSingle("--variables", argOptions).c_str() : "";

    std::cout << "##### Optional Arguments #####" << std::endl;
    std::cout << "variables = " << variablesStr.c_str() << std::endl;
    std::cout << "##### Optional Arguments - END #####" << std::endl;

    std::vector<std::string> variables = split(variablesStr, ",", false, false);
    int nVariables = variables.size();

    std::cout << "nVariables = " << nVariables << std::endl;
    for (int i = 0; i < nVariables; ++i) {
        std::cout << Form("variables[%d] = %s", i, variables.at(i).c_str()) << std::endl;
    }

    bool isRegression = false;
    bool useTMVAStyle = false;

    // set style and remove existing canvas'
    TMVA::TMVAGlob::Initialize(useTMVAStyle);

    // checks if file with name "fin" is already open, and if not opens one
    TFile* file = 0;
    file = TMVA::TMVAGlob::OpenFile(inputFile.c_str());

    TDirectoryFile* dir = 0;
    dir = (TDirectoryFile*)file->GetDirectory(dirPath.c_str());

    TH1D* hSig = 0;
    TH1D* hBkg = 0;

    std::string suffixSig = "__Signal_Id";
    std::string suffixBkg = "__Background_Id";

    std::vector<std::string> histNames;
    if (nVariables > 0) {
        for (int i = 0; i < nVariables; ++i) {
            histNames.push_back(Form("%s__Signal_Id", variables[i].c_str()));
        }
    }
    else {
        std::cout << "No variable is specified in the option. All variables will be plotted." << std::endl;

        std::string regexStr = wildCard2Regex(Form("*%s", suffixSig.c_str()));
        TList* keysHisto = getListOfMatchedKeys(dir, regexStr, "TH1", true);

        TKey* key = 0;
        TIter* iter = new TIter(keysHisto);
        while ((key=(TKey*)iter->Next())) {
            histNames.push_back(key->GetName());
        }
    }

    int nHistNames = histNames.size();

    std::cout << "nHistNames = " << nHistNames << std::endl;
    for (int i = 0; i < nHistNames; ++i) {
        std::cout << Form("histNames[%d] = %s", i, histNames.at(i).c_str()) << std::endl;
    }

    TPad* cPad = 0;
    TCanvas* cnv = 0;
    TText* text = 0;
    for (int i = 0; i < nHistNames; ++i) {

        std::string histSigName = histNames[i];
        std::string histBkgName = replaceAll(histSigName, suffixSig, suffixBkg);

        std::string varNameTmp = replaceAll(histSigName, suffixSig, "");
        varNameTmp = replaceAll(varNameTmp, "h_", "");

        std::cout << "plotting variable : " << varNameTmp.c_str() << std::endl;

        hSig = 0;
        hBkg = 0;

        hSig = (TH1D*)dir->Get(histSigName.c_str());
        hBkg = (TH1D*)dir->Get(histBkgName.c_str());

        if (hSig == 0) {
            std::cout << "Signal histogram not found : " << histSigName.c_str() << std::endl;
            std::cout << "exiting" << std::endl;
            return;
        }
        if (hBkg == 0) {
            std::cout << "Background histogram not found : " << histBkgName.c_str() << std::endl;
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
        bool createNewFig = false;

        // create new canvas
        if (countPad % nPadsCnv==0) {
            ++countCanvas;
            cnv = new TCanvas(Form("canvas%d", countCanvas), "",
                    countCanvas*50+50, countCanvas*20, width, height);
            cnv->Divide(nPadsX,nPadsY);
            cnv->Draw();
        }

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

        text = new TText(0.98, 0.14, uoflow.c_str());
        text->SetNDC();
        text->SetTextSize(0.040);
        text->SetTextAngle(90);
        text->AppendPad();

        // save canvas to file
        std::string outputFileTmp = Form("%s/plot_%s", outputDir.c_str(), varNameTmp.c_str());
        if (countPad % nPadsCnv==0) {
            TMVA::TMVAGlob::plot_logo();
            convertImage(cnv, outputFileTmp.c_str());
            createNewFig = false;
        }
        else {
            createNewFig = true;
        }

        if (createNewFig) {
            TMVA::TMVAGlob::plot_logo();
            convertImage(cnv, outputFileTmp.c_str());
            createNewFig = false;
        }

        if (cnv != 0) {
            cnv->Close();
        }
    }

    std::cout << "running plotTMVA() - END" << std::endl;
}

int main(int argc, char** argv)
{
    std::vector<std::string> argStr = ArgumentParser::ParseParameters(argc, argv);
    int nArgStr = argStr.size();

    argOptions = ArgumentParser::ParseOptions(argc, argv);

    if (nArgStr == 4) {
        plotTMVA(argv[1], argv[2], argv[3]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./plotTMVA.exe <inputFile> <directory in file> <variable name> <output directory for plots>"
                << std::endl;
        std::cout << "Options are" << std::endl;
        std::cout << "variables=<comma separated list of variables to be plotted>" << std::endl;
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
