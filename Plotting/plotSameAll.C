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
#include <TLine.h>

#include <vector>
#include <string>
#include <iostream>

#include "../Utilities/interface/ConfigurationParser.h"
#include "../Utilities/interface/GraphicsConfigurationParser.h"
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

    std::string title = ArgumentParser::ParseOptionInputSingle("--title", argOptions).c_str();
    if (title.size() > 0) {
        std::cout << "title set to " << title.c_str() << std::endl;
    }

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

    // marker colors
    std::string colorsStr = ArgumentParser::ParseOptionInputSingle("--colors", argOptions);
    std::vector<std::string> colors = split(colorsStr, ",", false, false);
    int nColors = colors.size();
    std::cout << "nColors = " << nColors << std::endl;
    for (int i = 0; i < nColors; ++i) {
        std::cout << Form("colors[%d] = %s", i, colors.at(i).c_str()) << std::endl;
    }
    if (nColors > 1 && nColors != nInputFiles) {
        std::cout << "Warning : Number of colors does not match number of input files. Default colors will be used" << std::endl;
    }

    // marker styles
    std::string markerStylesStr = ArgumentParser::ParseOptionInputSingle("--mstyles", argOptions);
    std::vector<std::string> markerStyles = split(markerStylesStr, ",", false, false);
    int nMarkerStyles = markerStyles.size();
    std::cout << "nMarkerStyles = " << nMarkerStyles << std::endl;
    for (int i = 0; i < nMarkerStyles; ++i) {
        std::cout << Form("markerStyles[%d] = %s", i, markerStyles.at(i).c_str()) << std::endl;
    }
    if (nMarkerStyles > 1 && nMarkerStyles != nInputFiles) {
        std::cout << "Warning : Number of marker styles does not match number of input files. Default marker style will be used" << std::endl;
    }

    // marker sizes
    std::string markerSizesStr = ArgumentParser::ParseOptionInputSingle("--msizes", argOptions);
    std::vector<std::string> markerSizes = split(markerSizesStr, ",", false, false);
    int nMarkerSizes = markerSizes.size();
    std::cout << "nMarkerSizes = " << nMarkerSizes << std::endl;
    for (int i = 0; i < nMarkerSizes; ++i) {
        std::cout << Form("markerSizes[%d] = %s", i, markerSizes.at(i).c_str()) << std::endl;
    }
    if (nMarkerSizes > 1 && nMarkerSizes != nInputFiles) {
        std::cout << "Warning : Number of marker sizes does not match number of input files. Default marker size will be used" << std::endl;
    }

    // x-axis ranges
    double xmin = (ArgumentParser::ParseOptionInputSingle("--xmin", argOptions).size() > 0) ?
            std::atof(ArgumentParser::ParseOptionInputSingle("--xmin", argOptions).c_str()) : -999999;
    double xmax = (ArgumentParser::ParseOptionInputSingle("--xmax", argOptions).size() > 0) ?
            std::atof(ArgumentParser::ParseOptionInputSingle("--xmax", argOptions).c_str()) : -999999;

    if (xmin != -999999) {
        std::cout << "xmin set to " << xmin << std::endl;
    }
    if (xmax != -999999) {
        std::cout << "xmax set to " << xmax << std::endl;
    }

    // y-axis ranges
    double ymin = (ArgumentParser::ParseOptionInputSingle("--ymin", argOptions).size() > 0) ?
            std::atof(ArgumentParser::ParseOptionInputSingle("--ymin", argOptions).c_str()) : -999999;
    double ymax = (ArgumentParser::ParseOptionInputSingle("--ymax", argOptions).size() > 0) ?
            std::atof(ArgumentParser::ParseOptionInputSingle("--ymax", argOptions).c_str()) : -999999;

    if (ymin != -999999) {
        std::cout << "ymin set to " << ymin << std::endl;
    }
    if (ymax != -999999) {
        std::cout << "ymax set to " << ymax << std::endl;
    }

    int binToMatch = (ArgumentParser::ParseOptionInputSingle("--binToMatch", argOptions).size() > 0) ?
            std::atof(ArgumentParser::ParseOptionInputSingle("--binToMatch", argOptions).c_str()) : -1;

    if (binToMatch > 0) {
        std::cout << "binToMatch = " << binToMatch << std::endl;
    }

    // horizontal/vertical lines
    std::string linesHorizontalArg = ArgumentParser::ParseOptionInputSingle("--hlines", argOptions);
    std::vector<float> linesHorizontal = ConfigurationParser::ParseListWithoutBracketFloat(linesHorizontalArg, ",");
    int nLinesHorizontal = linesHorizontal.size();
    std::cout << "nLinesHorizontal = " << nLinesHorizontal << std::endl;
    for (int i = 0; i < nLinesHorizontal; ++i) {
        std::cout << Form("linesHorizontal[%d] = %f", i, linesHorizontal.at(i)) << std::endl;
    }

    std::string lineStylesHorizontalArg = ArgumentParser::ParseOptionInputSingle("--hlinestyles", argOptions);
    std::vector<std::string> lineStylesHorizontal = split(lineStylesHorizontalArg, ",", false, false);
    int nLineStylesHorizontal = lineStylesHorizontal.size();
    std::cout << "nLineStylesHorizontal = " << nLineStylesHorizontal << std::endl;
    for (int i = 0; i < nLineStylesHorizontal; ++i) {
        std::cout << Form("lineStylesHorizontal[%d] = %s", i, lineStylesHorizontal.at(i).c_str()) << std::endl;
    }
    if (nLinesHorizontal > 1 && nLineStylesHorizontal > 1 && nLinesHorizontal != nLineStylesHorizontal) {
        std::cout << "Warning : Number of horizontal lines styles does not match number of lines. Default line style will be used" << std::endl;
        lineStylesHorizontal = {"kDashed"};
        nLineStylesHorizontal = lineStylesHorizontal.size();
    }

    std::string linesVerticalArg = ArgumentParser::ParseOptionInputSingle("--vlines", argOptions);
    std::vector<float> linesVertical = ConfigurationParser::ParseListWithoutBracketFloat(linesVerticalArg, ",");
    int nLinesVertical = linesVertical.size();
    std::cout << "nLinesVertical = " << nLinesVertical << std::endl;
    for (int i = 0; i < nLinesVertical; ++i) {
        std::cout << Form("linesVertical[%d] = %f", i, linesVertical.at(i)) << std::endl;
    }

    std::string lineStylesVerticalArg = ArgumentParser::ParseOptionInputSingle("--vlinestyles", argOptions);
    std::vector<std::string> lineStylesVertical = split(lineStylesVerticalArg, ",", false, false);
    int nLineStylesVertical = lineStylesVertical.size();
    std::cout << "nLineStylesVertical = " << nLineStylesVertical << std::endl;
    for (int i = 0; i < nLineStylesVertical; ++i) {
        std::cout << Form("lineStylesVertical[%d] = %s", i, lineStylesVertical.at(i).c_str()) << std::endl;
    }
    if (nLinesVertical > 1 && nLineStylesVertical > 1 && nLinesVertical != nLineStylesVertical) {
        std::cout << "Warning : Number of vertical lines styles does not match number of lines. Default line style will be used" << std::endl;
        lineStylesVertical = {"kDashed"};
        nLineStylesVertical = lineStylesVertical.size();
    }

    std::string graphicsFormat = (ArgumentParser::ParseOptionInputSingle(ARGUMENTPARSER::format, argOptions).size() > 0) ?
            ArgumentParser::ParseOptionInputSingle(ARGUMENTPARSER::format, argOptions) : "png";

    std::cout << "graphicsFormat = " << graphicsFormat.c_str() << std::endl;

    // window width
    int ww = (ArgumentParser::ParseOptionInputSingle("--ww", argOptions).size() > 0) ?
            std::atoi(ArgumentParser::ParseOptionInputSingle("--ww", argOptions).c_str()) : 600;
    // window height
    int wh = (ArgumentParser::ParseOptionInputSingle("--wh", argOptions).size() > 0) ?
            std::atoi(ArgumentParser::ParseOptionInputSingle("--wh", argOptions).c_str()) : 600;

    // margins
    double lmargin = (ArgumentParser::ParseOptionInputSingle("--lmargin", argOptions).size() > 0) ?
            std::atof(ArgumentParser::ParseOptionInputSingle("--lmargin", argOptions).c_str()) : 0.1;
    double rmargin = (ArgumentParser::ParseOptionInputSingle("--rmargin", argOptions).size() > 0) ?
            std::atof(ArgumentParser::ParseOptionInputSingle("--rmargin", argOptions).c_str()) : 0.1;
    double bmargin = (ArgumentParser::ParseOptionInputSingle("--bmargin", argOptions).size() > 0) ?
            std::atof(ArgumentParser::ParseOptionInputSingle("--bmargin", argOptions).c_str()) : 0.1;
    double tmargin = (ArgumentParser::ParseOptionInputSingle("--tmargin", argOptions).size() > 0) ?
            std::atof(ArgumentParser::ParseOptionInputSingle("--tmargin", argOptions).c_str()) : 0.1;

    std::cout << "ww = " << ww << std::endl;
    std::cout << "wh = " << wh << std::endl;

    std::cout << "lmargin = " << lmargin << std::endl;
    std::cout << "rmargin = " << rmargin << std::endl;
    std::cout << "bmargin = " << bmargin << std::endl;
    std::cout << "tmargin = " << tmargin << std::endl;

    int logx = (ArgumentParser::ParseOptionInputSingle("--logx", argOptions).size() > 0) ?
            std::atoi(ArgumentParser::ParseOptionInputSingle("--logx", argOptions).c_str()) : 0;
    int logy = (ArgumentParser::ParseOptionInputSingle("--logy", argOptions).size() > 0) ?
            std::atoi(ArgumentParser::ParseOptionInputSingle("--logy", argOptions).c_str()) : 0;
    int logz = (ArgumentParser::ParseOptionInputSingle("--logz", argOptions).size() > 0) ?
            std::atoi(ArgumentParser::ParseOptionInputSingle("--logz", argOptions).c_str()) : 0;

    if (logx != 0) {
        std::cout << "logx set to " << logx << std::endl;
    }
    if (logy != 0) {
        std::cout << "logy set to " << logy << std::endl;
    }
    if (logz != 0) {
        std::cout << "logz set to " << logz << std::endl;
    }

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
    TLine* line = 0;

    for (int j = 0; j < nTH1; ++j) {

        if (!doPlotSame[j])  continue;

        std::string tmpPath = th1Paths[j];
        tmpPath = replaceAll(tmpPath, " ", "_");
        tmpPath = replaceAll(tmpPath, "/", "_");
        std::string canvasName = Form("cnv_%s", tmpPath.c_str());

        c = new TCanvas(canvasName.c_str(), "", ww, wh);
        c->cd();
        setCanvasMargin(c, lmargin, rmargin, bmargin, tmargin);

        leg = new TLegend();
        // make legend transparent
        leg->SetFillColor(-1);
        leg->SetFillStyle(4000);
        leg->SetBorderSize(0);

        std::string legendOption = "lpf";

        if (binToMatch > 0) {
            for (int i = 0; i < nInputFiles; ++i) {
                vecTH1[i][j]->Scale(vecTH1[0][j]->GetBinContent(binToMatch) / vecTH1[i][j]->GetBinContent(binToMatch));
            }
        }

        // estimate a common min/max for TH1 y-axis
        // if "ymin" / "ymax" were set already, then estimated values will be overwritten
        std::vector<TH1*> tmpVecTH1;
        for (int i = 0; i < nInputFiles; ++i) {
            tmpVecTH1.push_back(vecTH1[i][j]);
        }
        double th1Min = getMinimumTH1s(tmpVecTH1, nInputFiles);
        double th1Max = getMaximumTH1s(tmpVecTH1, nInputFiles);
        bool isGlobalMin = true;
        bool isGlobalMax = true;
        for (int i = 0; i < nInputFiles; ++i) {
            if (vecTH1[i][j]->GetMinimum() != th1Min) isGlobalMin = false;
            if (vecTH1[i][j]->GetMaximum() != th1Max) isGlobalMax = false;
        }
        // adjust the estimated min/max unless they were shared by all TH1
        if (!isGlobalMin) {
            th1Min -= th1Min-TMath::Abs(th1Min)*0.1;
        }
        if (!isGlobalMax) {
            th1Max += TMath::Abs(th1Max)*0.1*TMath::Power(10,logy);
        }

        double xMinFinal = 0;
        double xMaxFinal = -1;
        for (int i = 0; i < nInputFiles; ++i) {

            vecTH1[i][j]->SetStats(false);

            if (title.size() > 0) {
                if (title == "NULL") {
                    vecTH1[i][j]->SetTitle("");
                }
                else {
                    vecTH1[i][j]->SetTitle(title.c_str());
                }
            }

            int color = GRAPHICS::colors[i%13];
            if (nColors == 1) color = GraphicsConfigurationParser::ParseColor(colors.at(0));
            else if (nColors == nInputFiles) color = GraphicsConfigurationParser::ParseColor(colors.at(i));
            vecTH1[i][j]->SetMarkerColor(color);

            int markerStyle = kFullCircle;
            if (nMarkerStyles == 1) markerStyle = GraphicsConfigurationParser::ParseMarkerStyle(markerStyles.at(0));
            else if (nMarkerStyles == nInputFiles) markerStyle = GraphicsConfigurationParser::ParseMarkerStyle(markerStyles.at(i));
            vecTH1[i][j]->SetMarkerStyle(markerStyle);

            double markerSize = vecTH1[i][j]->GetMarkerSize();
            if (nMarkerSizes == 1) markerSize = std::atof(markerSizes.at(0).c_str());
            else if (nMarkerSizes == nInputFiles) markerSize = std::atof(markerSizes.at(i).c_str());
            vecTH1[i][j]->SetMarkerSize(markerSize);

            int nBinsX = vecTH1[i][j]->GetNbinsX();
            xMinFinal = vecTH1[i][j]->GetXaxis()->GetXmin();
            xMaxFinal = vecTH1[i][j]->GetXaxis()->GetXmax();
            if (xmin != -999999) {
                if (xmin < vecTH1[i][j]->GetBinLowEdge(nBinsX+1)) {
                    xMinFinal = xmin;
                }
                else {
                    std::cout << "Warning : xmin is not smaller than the upper limit of x-axis." << std::endl;
                    std::cout << "xmin is not set for " << vecTH1[i][j]->GetName() << std::endl;
                }
            }
            if (xmax != -999999) {
                if (xmax >= vecTH1[i][j]->GetBinLowEdge(1)) {
                    xMaxFinal = xmax;
                }
                else {
                    std::cout << "Warning : xmax is smaller than the lower limit of x-axis." << std::endl;
                    std::cout << "xmax is not set for " << vecTH1[i][j]->GetName() << std::endl;
                }
            }
            vecTH1[i][j]->GetXaxis()->SetRangeUser(xMinFinal, xMaxFinal);

            vecTH1[i][j]->SetMinimum(th1Min);
            if (ymin != -999999) {
                vecTH1[i][j]->SetMinimum(ymin);
            }
            vecTH1[i][j]->SetMaximum(th1Max);
            if (ymax != -999999) {
                vecTH1[i][j]->SetMaximum(ymax);
            }

            std::string drawOption = (i == 0) ? "e" : "e same";
            vecTH1[i][j]->Draw(drawOption.c_str());

            leg->AddEntry(vecTH1[i][j], labels[i].c_str(), legendOption.c_str());
        }

        for (int iLine = 0; iLine < nLinesHorizontal; ++iLine) {

            // draw horizontal line
            gPad->Update();
            int lineStyleHorizontal = kDashed;
            if (nLineStylesHorizontal == 1)
                lineStyleHorizontal = GraphicsConfigurationParser::ParseLineStyle(lineStylesHorizontal.at(0));
            else if (nLineStylesHorizontal == nLinesHorizontal)
                lineStyleHorizontal = GraphicsConfigurationParser::ParseLineStyle(lineStylesHorizontal.at(iLine));

            line = 0;
            line = new TLine(xMinFinal, linesHorizontal.at(iLine), xMaxFinal, linesHorizontal.at(iLine));
            line->SetLineStyle(lineStyleHorizontal);   // https://root.cern.ch/doc/master/TAttLine_8h.html#a7092c0c4616367016b70d54e5c680a69
            line->Draw();
        }

        for (int iLine = 0; iLine < nLinesVertical; ++iLine) {

            // draw vertical line
            gPad->Update();
            double ymin = gPad->GetUymin();
            double ymax = gPad->GetUymax();
            if (gPad->GetLogy() > 0) {
                ymin = TMath::Power(TMath::Power(10, gPad->GetLogy()), gPad->GetUymin());
                ymax = TMath::Power(TMath::Power(10, gPad->GetLogy()), gPad->GetUymax());
            }

            int lineStyleVertical = kDashed;
            if (nLineStylesVertical == 1)
                lineStyleVertical = GraphicsConfigurationParser::ParseLineStyle(lineStylesVertical.at(0));
            else if (nLineStylesVertical == nLinesVertical)
                lineStyleVertical = GraphicsConfigurationParser::ParseLineStyle(lineStylesVertical.at(iLine));

            line = 0;
            line = new TLine(linesVertical.at(iLine), ymin, linesVertical.at(iLine), ymax);
            line->SetLineStyle(lineStyleVertical);   // https://root.cern.ch/doc/master/TAttLine_8h.html#a7092c0c4616367016b70d54e5c680a69
            line->Draw();
        }

        double height = calcTLegendHeight(leg);
        double width = calcTLegendWidth(leg);
        std::string legendPosition = "NW";
        double legendOffsetX = 0.04;
        double legendOffsetY = 0.04;
        setLegendPosition(leg, legendPosition, (TPad*)c, height, width, legendOffsetX, legendOffsetY, false);
        int nLegEntriesTmp = leg->GetNRows();
        if (nLegEntriesTmp > 0)  leg->Draw();

        setCanvasFinal(c, logx, logy, logz);
        c->Update();
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
        std::cout << "--colors=<comma separated list of colors>" << std::endl;
        std::cout << "--mstyles=<comma separated list of marker styles>" << std::endl;
        std::cout << "--msizes=<comma separated list of marker sizes>" << std::endl;
        std::cout << "--xmin=<minimum value for x-axis>" << std::endl;
        std::cout << "--xmax=<maximum value for x-axis>" << std::endl;
        std::cout << "--ymin=<minimum value for y-axis>" << std::endl;
        std::cout << "--ymax=<maximum value for y-axis>" << std::endl;
        std::cout << "--format=<graphicsFormat>" << std::endl;
        std::cout << "--ww=<window width>" << std::endl;
        std::cout << "--wh=<window height>" << std::endl;
        std::cout << "--lmargin=<left margin>, Similarly rmargin, bmargin, tmargin for right, bottom, and top margins" << std::endl;
        std::cout << "--logx=<log scale of x-axis>, Similarly for logy and logz" << std::endl;
        return 1;
    }
    return 0;
}
