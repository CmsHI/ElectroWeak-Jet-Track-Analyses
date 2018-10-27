/*
 * macro to plot together objects from different files
 * It is assumed that objects to be plotted together have the same path in all files.
 * Canvases are written to "outputFile" and saved as picture in "outputDir"
 */
#include <TKey.h>
#include <TDirectoryFile.h>
#include <TFile.h>
#include <TH1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TSystem.h>
#include <TCanvas.h>
#include <TLegend.h>

#include <vector>
#include <string>
#include <iostream>

#include "../Utilities/interface/ArgumentParser.h"
#include "../Utilities/styleUtil.h"
#include "../Utilities/fileUtil.h"
#include "../Utilities/systemUtil.h"

std::vector<std::string> argOptions;

void plotSameAll(std::string inputFiles, std::string outputFile = "plotSameAll.root", std::string outputDir = "");

void plotSameAll(std::string inputFiles, std::string outputFile, std::string outputDir)
{
    std::cout << "running plotSameAll()" << std::endl;
    std::cout << "inputPaths = " << inputFiles.c_str() << std::endl;
    std::cout << "outputFile = " << outputFile.c_str() << std::endl;
    std::cout << "outputDir = " << outputDir.c_str() << std::endl;
    std::cout << "Parsing list of input files" << std::endl;
    std::vector<std::string> inputFilesStr = split(inputFiles, ",");
    int nInputFiles = inputFilesStr.size();
    std::cout << "nInputFiles = " << nInputFiles << std::endl;
    for (int i = 0; i < nInputFiles; ++i) {
        std::cout << Form("inputFiles[%d] = %s", i, inputFilesStr.at(i).c_str()) << std::endl;
    }

    std::vector<TFile*> inputs(nInputFiles, 0);
    for (int i = 0; i < nInputFiles; ++i) {
        inputs[i] = new TFile(inputFilesStr.at(i).c_str(), "READ");
    }

    TFile* output = new TFile(outputFile.c_str(),"UPDATE");
    output->cd();

    if(outputDir == "")   { // no input directory was given.
        outputDir = replaceAll(outputFile, ".root", "");
    }
    gSystem->mkdir(outputDir.c_str(), true);


    // labels for each file, will be used as legend entries
    std::string labelsStr = ArgumentParser::ParseOptionInputSingle("--labels", argOptions);
    std::vector<std::string> labels = split(labelsStr, ";;;", false);
    int nLabels = labels.size();
    std::cout << "nLabels = " << nLabels << std::endl;
    for (int i = 0; i < nLabels; ++i) {
        std::cout << Form("labels[%d] = %s", i, labels.at(i).c_str()) << std::endl;
    }
    if (nLabels != nInputFiles) {
        labels.clear();
        for (int i = 0; i < nInputFiles; ++i) {
            labels.push_back(Form("file %d", i+1));
        }
    }

    std::string graphicsFormat = (ArgumentParser::ParseOptionInputSingle(ARGUMENTPARSER::format, argOptions).size() > 0) ?
            ArgumentParser::ParseOptionInputSingle(ARGUMENTPARSER::format, argOptions) : "png";

    std::cout << "graphicsFormat = " << graphicsFormat.c_str() << std::endl;

    // list of paths for TH1 objects to be plotted, wildcards are also accepted
    std::string th1sStr = ArgumentParser::ParseOptionInputSingle("--th1s", argOptions);
    std::vector<std::string> th1Paths = split(th1sStr, ",", false);
    int nTH1 = th1Paths.size();

    std::cout << "nTH1 = " << nTH1 << std::endl;
    for (int i = 0; i < nTH1; ++i) {
        std::cout << Form("th1s[%d] = %s", i, th1Paths.at(i).c_str()) << std::endl;
    }

    std::cout << "checking paths that match the given wildcards if any" << std::endl;
    std::vector<std::string> th1PathsTmp = th1Paths;
    th1Paths.clear();
    for (int i = 0; i < nTH1; ++i){

        std::string regexStr = wildCard2Regex(th1PathsTmp[i]);

        std::vector<std::string> tmpMatchedPaths = getListOfMatchedPaths(inputs[0], regexStr, "TH1", true);

        if (tmpMatchedPaths.size() > 0) {
            // replace the current path with the list of matched paths
            th1Paths.insert(th1Paths.end(), tmpMatchedPaths.begin(), tmpMatchedPaths.end());
        }
        else {
            // put back the current path
            th1Paths.push_back(th1PathsTmp[i]);
        }
    }
    nTH1 = th1Paths.size();
    std::cout << "final nTH1 = " << nTH1 << std::endl;
    for (int i = 0; i < nTH1; ++i) {
        std::cout << Form("th1s[%d] = %s", i, th1Paths.at(i).c_str()) << std::endl;
    }

    std::vector<bool> doPlotSame(nTH1, true);

    std::vector<TH1D*> vecTH1[nInputFiles];
    for (int i = 0; i < nInputFiles; ++i) {
        vecTH1[i].clear();
        vecTH1[i].resize(nTH1);
        for (int j = 0; j < nTH1; ++j) {
            vecTH1[i][j] = 0;
            vecTH1[i][j] = (TH1D*)inputs[i]->Get(th1Paths[j].c_str());

            if (vecTH1[i][j] == 0) {
                doPlotSame[j] = false;
                std::cout << "Warning : TH1 with path " << th1Paths[j].c_str() << " is not found in file " << inputFilesStr.at(i).c_str() << std::endl;
                std::cout << "plotSame will not be run for this TH1 " << std::endl;
            }
        }
    }

    TCanvas* c = 0;
    TLegend* leg = 0;

    std::vector<int> markerColors = {kBlack, kBlue, kRed, kOrange,  kViolet, kCyan, kSpring, kYellow,
            kAzure, kPink, kGreen, kMagenta, kTeal};
    int nMarkerColors = markerColors.size();

    for (int j = 0; j < nTH1; ++j) {

        if (!doPlotSame[j])  continue;

        std::string tmpPath = th1Paths[j];
        tmpPath = replaceAll(tmpPath, " ", "_");
        tmpPath = replaceAll(tmpPath, "/", "_");
        std::string canvasName = Form("cnv_%s", tmpPath.c_str());

        int windowWidth = 800;
        int windowHeight = 800;

        double leftMargin = 0.1;
        double rightMargin = 0.1;
        double bottomMargin = 0.1;
        double topMargin = 0.1;

        c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);
        c->cd();
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);

        leg = new TLegend();
        // make legend transparent
        leg->SetFillColor(-1);
        leg->SetFillStyle(4000);
        leg->SetBorderSize(0);

        std::string legendOption = "lpf";

        for (int i = 0; i < nInputFiles; ++i) {

            vecTH1[i][j]->SetStats(false);

            vecTH1[i][j]->SetMarkerStyle(kFullCircle);
            vecTH1[i][j]->SetMarkerColor(markerColors[i%nMarkerColors]);

            std::string drawOption = (i == 0) ? "e" : "e same";
            vecTH1[i][j]->Draw(drawOption.c_str());

            leg->AddEntry(vecTH1[i][j], labels[i].c_str(), legendOption.c_str());
        }

        double height = calcTLegendHeight(leg);
        double width = calcTLegendWidth(leg);
        std::string legendPosition = "NW";
        double legendOffsetX = 0.04;
        double legendOffsetY = 0.04;
        setLegendPosition(leg, legendPosition, (TPad*)c, height, width, legendOffsetX, legendOffsetY, true);
        int nLegEntriesTmp = leg->GetNRows();
        if (nLegEntriesTmp > 0)  leg->Draw();

        setCanvasFinal(c);
        c->Write("",TObject::kOverwrite);
        c->SaveAs(Form("%s/%s.%s", outputDir.c_str(), canvasName.c_str(), graphicsFormat.c_str()));
        c->Close();         // do not use Delete() for TCanvas.
    }

    std::cout<< "end : saving graphics" <<std::endl;

    std::cout<<"Closing the input files."<<std::endl;
    for (int i = 0; i < nInputFiles; ++i) {
        inputs[i]->Close();
    }
    std::cout<<"Closing the output file."<<std::endl;
    output->Close();
    std::cout<<"running plotSameAll() - END"<<std::endl;
}

int main(int argc, char** argv)
{
    std::vector<std::string> argStr = ArgumentParser::ParseParameters(argc, argv);
    int nArgStr = argStr.size();

    argOptions = ArgumentParser::ParseOptions(argc, argv);

    if (nArgStr == 4)    plotSameAll(argStr.at(1), argStr.at(2), argStr.at(3));
    else if (nArgStr == 3)    plotSameAll(argStr.at(1), argStr.at(2));
    else if (nArgStr == 2)    plotSameAll(argStr.at(1));
    else {
        std::cout << "Usage : \n" <<
                "./plotSameAll.exe <inputFiles> <outputFile> <outputDir> [options]"
                << std::endl;
        std::cout << "<inputFiles> is a comma separated list of files" << std::endl;
        std::cout << "Options are" << std::endl;
        std::cout << "--th1s=<comma separated list of TH1 paths, wildcards can be used>" << std::endl;
        std::cout << "--labels=<labels to be used in the legend, separated by ;;;>" << std::endl;
        std::cout << "--format=<graphicsFormat>" << std::endl;
        return 1;
    }
    return 0;
}
