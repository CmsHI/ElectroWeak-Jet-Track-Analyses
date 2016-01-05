/*
 * macro to draw spectra/distribution histograms.
 * saves histograms to a .root file.
 * if "outputFigureName" is specified and "drawSame" input is set, saves the canvas as picture.
 */

#include <TFile.h>
#include <TChain.h>
#include <TH1.h>
#include <TH1D.h>
#include <TCut.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TMath.h>

#include <string>
#include <vector>
#include <iostream>

#include "../TreeHeaders/CutConfigurationTree.h"
#include "../TreeHeaders/ggHiNtuplizerTree.h"
#include "../Utilities/interface/CutConfigurationParser.h"
#include "../Utilities/interface/InputConfigurationParser.h"
#include "../Utilities/styleUtil.h"

void drawSpectra(const TString configFile, const TString inputFile, const TString outputFile = "drawSpectra.root", const TString outputFigureName = "");

void drawSpectra(const TString configFile, const TString inputFile, const TString outputFile, const TString outputFigureName)
{
    std::cout<<"running drawSpectra()"<<std::endl;
    std::cout<<"configFile  = "<< configFile.Data() <<std::endl;
    std::cout<<"inputFile   = "<< inputFile.Data()  <<std::endl;
    std::cout<<"outputFile  = "<< outputFile.Data() <<std::endl;

    InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

    std::vector<std::string> formulas;
    std::string selectionBase;
    std::vector<std::string> selections;
    std::vector<std::string> weights;
    std::string title;
    std::string titleX;
    std::string titleY;
    std::vector<std::vector<float>> TH1D_Bins_List;      // nBins, xLow, xUp for the TH1D histogram
    std::vector<std::string> legendEntryLabels;
    std::string legendPosition;
    float legendOffsetX;
    float legendOffsetY;
    float leftMargin;
    float rightMargin;
    float bottomMargin;
    float topMargin;
    float titleOffsetX;
    float titleOffsetY;

    std::string treePath;
    std::vector<std::string> treeFriendsPath;

    int drawSame;
    int setLogx;
    int setLogy;
    if (configInput.isValid) {
        formulas = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treeFormula]);
        selectionBase = configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treeSelectionBase];
        selections = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treeSelection]);
        weights = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH1_weight]);
        title = ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH1_title]);
        titleX = ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH1_titleX]);
        titleY = ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH1_titleY]);
        TH1D_Bins_List = ConfigurationParser::ParseListTH1D_Bins(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH1D_Bins_List]);
        legendEntryLabels = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_legendEntryLabel]);
        legendPosition    = configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_legendPosition];
        legendOffsetX     = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_legendOffsetX];
        legendOffsetY     = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_legendOffsetY];
        leftMargin   = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_leftMargin];
        rightMargin  = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_rightMargin];
        bottomMargin = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_bottomMargin];
        topMargin    = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_topMargin];
        titleOffsetX = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_titleOffsetX];
        titleOffsetY = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_titleOffsetY];

        treePath  = configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treePath];
        treeFriendsPath = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treeFriends_List]);

        drawSame = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_drawSame];
        setLogx = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_setLogx];
        setLogy = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_setLogy];
    }
    else {
        formulas.push_back("Entry$");
        selectionBase = "";
        selections.push_back("1 == 1");
        weights.push_back("1");
        title = "";
        titleX = "";
        titleY = "";
        TH1D_Bins_List.resize(3);
        TH1D_Bins_List[0].push_back(100);    // nBins
        TH1D_Bins_List[1].push_back(0);      // xLow
        TH1D_Bins_List[2].push_back(100);    // xUp
        legendEntryLabels.push_back("");
        legendPosition = "";
        legendOffsetX = 0;
        legendOffsetY = 0;
        leftMargin = 0.1;
        rightMargin = 0.1;
        bottomMargin = 0.1;
        topMargin = 0.1;
        titleOffsetX = 1;
        titleOffsetY = 1;

        treePath = "";

        drawSame = 0;
        setLogx = 0;
        setLogy = 0;
    }
    // set default values
    if (weights.size() == 0)    weights.push_back("1");   // default weight = 1.
    if (leftMargin == 0) leftMargin = 0.1;  // default margin
    if (rightMargin == 0) rightMargin = 0.1;  // default margin
    if (bottomMargin == 0) bottomMargin = 0.1;  // default margin
    if (topMargin == 0) topMargin = 0.1;  // default margin
    if (titleOffsetX == 0) titleOffsetX = 1;    // default offset
    if (titleOffsetY == 0) titleOffsetY = 1;    // default offset

    int nFormulas = formulas.size();
    int nSelections = selections.size();
    int nWeights = weights.size();
    int nTH1D_Bins_List = TH1D_Bins_List[0].size();
    int nFriends = treeFriendsPath.size();
    int nLegendEntryLabels = legendEntryLabels.size();
    // verbose about input configuration
    std::cout<<"Input Configuration :"<<std::endl;
    std::cout << "nFormulas     = " << nFormulas << std::endl;
    for (int i=0; i<nFormulas; ++i) {
        std::cout << Form("formulas[%d]   = %s", i, formulas.at(i).c_str()) << std::endl;
    }
    std::cout << "selectionBase = " << selectionBase.c_str() << std::endl;
    std::cout << "nSelections   = " << nSelections << std::endl;
    for (int i=0; i<nSelections; ++i) {
            std::cout << Form("selections[%d] = %s", i, selections.at(i).c_str()) << std::endl;
    }
    std::cout << "nWeights   = " << nWeights << std::endl;
    for (int i=0; i<nWeights; ++i) {
            std::cout << Form("weights[%d] = %s", i, weights.at(i).c_str()) << std::endl;
    }
    std::cout << "nTH1D_Bins_List = " << nTH1D_Bins_List << std::endl;
    for (int i=0; i<nTH1D_Bins_List; ++i) {
            std::cout << Form("nBins[%d] = %.0f", i, TH1D_Bins_List[0].at(i)) << std::endl;
            std::cout << Form("xLow[%d]  = %f", i, TH1D_Bins_List[1].at(i)) << std::endl;
            std::cout << Form("xUp[%d]   = %f", i, TH1D_Bins_List[2].at(i)) << std::endl;
    }
    std::cout << "title  = " << title.c_str() << std::endl;
    std::cout << "titleX = " << titleX.c_str() << std::endl;
    std::cout << "titleY = " << titleY.c_str() << std::endl;
    std::cout << "nLegendEntryLabels   = " << nLegendEntryLabels << std::endl;
    for (int i = 0; i<nLegendEntryLabels; ++i) {
            std::cout << Form("legendEntryLabels[%d] = %s", i, legendEntryLabels.at(i).c_str()) << std::endl;
    }
    std::cout << "legendPosition   = " << legendPosition.c_str() << std::endl;
    if (legendPosition.size() == 0) std::cout<< "No position is provided, legend will not be drawn." <<std::endl;
    std::cout << "legendOffsetX    = " << legendOffsetX << std::endl;
    std::cout << "legendOffsetY    = " << legendOffsetY << std::endl;
    std::cout << "leftMargin   = " << leftMargin << std::endl;
    std::cout << "rightMargin  = " << rightMargin << std::endl;
    std::cout << "bottomMargin = " << bottomMargin << std::endl;
    std::cout << "topMargin    = " << topMargin << std::endl;
    std::cout << "titleOffsetX = " << titleOffsetX << std::endl;
    std::cout << "titleOffsetY = " << titleOffsetY << std::endl;

    std::cout << "treePath = " << treePath.c_str() << std::endl;
    std::cout << "nFriends = " << nFriends << std::endl;
    for (int i=0; i<nFriends; ++i) {
        std::cout << Form("treeFriends[%d] = %s", i, treeFriendsPath.at(i).c_str()) << std::endl;
    }

    std::cout << "drawSame = " << drawSame << std::endl;
    std::cout << "setLogx  = " << setLogx << std::endl;
    std::cout << "setLogy  = " << setLogy << std::endl;

    if (configCuts.isValid) {

    }
    else {

    }

    // verbose about cut configuration
    std::cout<<"Cut Configuration :"<<std::endl;

    std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.Data());

    std::cout<<"input ROOT files : num = "<<inputFiles.size()<< std::endl;
    std::cout<<"#####"<< std::endl;
    for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
        std::cout<<(*it).c_str()<< std::endl;
    }
    std::cout<<"##### END #####"<< std::endl;

    TChain* tree = new TChain(treePath.c_str());
    TChain* treeFriends[nFriends];
    for (int i=0; i<nFriends; ++i) {
        treeFriends[i] = new TChain(treeFriendsPath.at(i).c_str());
        tree->AddFriend(treeFriends[i], Form("t%d", i));
    }

    for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
       tree->Add((*it).c_str());
       for (int i=0; i<nFriends; ++i) {
           treeFriends[i]->Add((*it).c_str());
       }
    }

    TH1::SetDefaultSumw2();
    int nHistos = nFormulas;
    if (nSelections > nFormulas) nHistos = nSelections;
    std::cout << "nHistos = " << nHistos << std::endl;
    TH1D* h[nHistos];
    for (int i=0; i<nHistos; ++i) {
        int nBins  = (int)TH1D_Bins_List[0].at(0);
        float xLow = TH1D_Bins_List[1].at(0);
        float xUp  = TH1D_Bins_List[2].at(0);
        if (nHistos == nTH1D_Bins_List) {
            nBins = (int)TH1D_Bins_List[0].at(i);
            xLow  = TH1D_Bins_List[1].at(i);
            xUp   = TH1D_Bins_List[2].at(i);
        }
        h[i] = new TH1D(Form("h_%d", i),Form("%s;%s;%s", title.c_str(), titleX.c_str(), titleY.c_str()), nBins, xLow, xUp);
    }

    Long64_t entries = tree->GetEntries();
    Long64_t entriesSelected[nHistos];
    std::cout << "entries = " << entries << std::endl;
    std::cout << "TTree::Draw() : " << treePath.c_str() <<std::endl;
    for (int i=0; i<nHistos; ++i) {

        std::string formula = formulas.at(0).c_str();
        std::string selection = selections.at(0).c_str();
        std::string weight = weights.at(0).c_str();
        if (nHistos == nFormulas)  formula = formulas.at(i).c_str();
        if (nHistos == nSelections)  selection = selections.at(i).c_str();
        if (nHistos == nWeights)  weight = weights.at(i).c_str();

        std::cout << "drawing histogram i = " << i << ", ";

        TCut selectionFinal = selectionBase.c_str();
        selectionFinal = selectionFinal && selection.c_str();
        entriesSelected[i] = tree->GetEntries(selectionFinal.GetTitle());
        std::cout << "entriesSelected = " << entriesSelected[i] << std::endl;

        TCut weight_AND_selection = Form("(%s)*(%s)", weight.c_str(), selectionFinal.GetTitle());
        tree->Draw(Form("%s >> %s", formula.c_str(), h[i]->GetName()), weight_AND_selection.GetTitle(), "goff");
    }
    std::cout <<  "TTree::Draw() ENDED : " << treePath.c_str() <<std::endl;
    std::cout << "entries = " << entries << std::endl;

    TFile *output = TFile::Open(outputFile.Data(),"RECREATE");
    output->cd();

    TH1D* h_normEvents[nHistos];
    TH1D* h_normInt[nHistos];
    for (int i=0; i<nHistos; ++i) {
        h[i]->Write();

        h_normEvents[i] = (TH1D*)h[i]->Clone(Form("%s_normEvents", h[i]->GetName()));
        h_normEvents[i]->Scale(1./entriesSelected[i]);
        h_normEvents[i]->Write();

        h_normInt[i] = (TH1D*)h[i]->Clone(Form("%s_normInt", h[i]->GetName()));
        h_normInt[i]->Scale(1./h[i]->Integral());
        h_normInt[i]->Write();
    }

    // write canvases
    TCanvas* c;
    for (int i=0; i<nHistos; ++i) {
        c = new TCanvas(Form("cnv_%d",i),"",600,600);
        c->SetTitle(h[i]->GetTitle());
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        setCanvasFinal(c, setLogx, setLogy);
        c->cd();

        h[i]->SetTitleOffset(titleOffsetX,"X");
        h[i]->SetTitleOffset(titleOffsetY,"Y");
        h[i]->SetMarkerStyle(kFullCircle);
        h[i]->SetStats(false);
        h[i]->Draw("e");
        c->Write();
        c->Close();         // do not use Delete() for TCanvas.

        // normalized by number of events
        c = new TCanvas(Form("cnv_%d_normEvents",i),"",600,600);
        c->SetTitle(h_normEvents[i]->GetTitle());
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        setCanvasFinal(c, setLogx, setLogy);
        c->cd();

        h_normEvents[i]->SetTitleOffset(titleOffsetX,"X");
        h_normEvents[i]->SetTitleOffset(titleOffsetY,"Y");
        h_normEvents[i]->SetMarkerStyle(kFullCircle);
        h_normEvents[i]->SetStats(false);
        h_normEvents[i]->Draw("e");
        c->Write();
        c->Close();         // do not use Delete() for TCanvas.

        // normalized to 1.
        c = new TCanvas(Form("cnv_%d_normInt",i),"",600,600);
        c->SetTitle(h_normInt[i]->GetTitle());
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        setCanvasFinal(c, setLogx, setLogy);
        c->cd();

        h_normInt[i]->SetTitleOffset(titleOffsetX,"X");
        h_normInt[i]->SetTitleOffset(titleOffsetY,"Y");
        h_normInt[i]->SetMarkerStyle(kFullCircle);
        h_normInt[i]->SetStats(false);
        h_normInt[i]->Draw("e");
        c->Write();
        c->Close();         // do not use Delete() for TCanvas.
    }

    if (drawSame > 0) {
        c = new TCanvas("cnv_drawSpectra","",600,600);
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        setCanvasFinal(c, setLogx, setLogy);
        c->cd();

        // assume not more than 13 curves are drawn.
        int colors[13] = {kBlack, kBlue, kRed,   kOrange,  kViolet, kCyan, kSpring, kYellow,
                kAzure, kPink, kGreen, kMagenta, kTeal};
        TLegend* leg = new TLegend();

        // set maximum/minimum of y-axis
        double histMin = h_normInt[0]->GetMinimum();
        double histMax = h_normInt[0]->GetMaximum();
        for (int i = 1; i<nHistos; ++i) {
            if (h_normInt[i]->GetMinimum() < histMin)   histMin = h_normInt[i]->GetMinimum();
            if (h_normInt[i]->GetMaximum() > histMax)   histMax = h_normInt[i]->GetMaximum();
        }
        if (setLogy == 0) h_normInt[0]->SetMinimum(histMin-TMath::Abs(histMin)*0.1);
        h_normInt[0]->SetMaximum(histMax+TMath::Abs(histMax)*0.1*TMath::Power(10,setLogy));

        for (int i = 0; i<nHistos; ++i) {

            h_normInt[i]->SetTitleOffset(titleOffsetX,"X");
            h_normInt[i]->SetTitleOffset(titleOffsetY,"Y");
            h_normInt[i]->SetMarkerColor(colors[i]);
            h_normInt[i]->SetMarkerStyle(kFullCircle);
            h_normInt[i]->SetStats(false);
            h_normInt[i]->Draw("e same");

            std::string label = legendEntryLabels.at(0).c_str();
            if (nHistos == nLegendEntryLabels) label = legendEntryLabels.at(i).c_str();

            leg->AddEntry(h_normInt[i], label.c_str(),"lp");
        }

        double height = calcTLegendHeight(leg);
        double width = calcTLegendWidth(leg);
        if (legendPosition.size() > 0) {    // draw the legend if really a position is provided.
            setLegendPosition(leg, legendPosition, c, height, width, legendOffsetX, legendOffsetY);
            leg->Draw();
        }

        c->Write();
        // save canvas as picture if a figure name is provided.
        if (! outputFigureName.EqualTo("")) {

            c->SaveAs(Form("%s.C", outputFigureName.Data()));
            c->SaveAs(Form("%s.png", outputFigureName.Data()));
            c->SaveAs(Form("%s.pdf", outputFigureName.Data()));
        }
        c->Close();
    }

    output->Close();
}

int main(int argc, char** argv)
{
    if (argc == 5) {
        drawSpectra(argv[1], argv[2], argv[3], argv[4]);
        return 0;
    }
    else if (argc == 4) {
        drawSpectra(argv[1], argv[2], argv[3]);
        return 0;
    }
    else if (argc == 3) {
        drawSpectra(argv[1], argv[2]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./drawSpectra.exe <configFile> <inputFile> <outputFile> (<outputFigureName>)"
                << std::endl;
        return 1;
    }
}
