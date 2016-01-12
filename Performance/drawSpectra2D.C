/*
 * macro to draw 2D spectra/distribution histograms.
 * saves histograms to a .root file.
 * if "outputFigureName" is specified and "drawSame" input is set, saves the canvas as picture.
 */

#include <TFile.h>
#include <TChain.h>
#include <TH1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCut.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TMath.h>

#include <string>
#include <vector>
#include <iostream>

#include "../TreeHeaders/CutConfigurationTree.h"
#include "../Utilities/interface/CutConfigurationParser.h"
#include "../Utilities/interface/InputConfigurationParser.h"
#include "../Utilities/styleUtil.h"

void drawSpectra2D(const TString configFile, const TString inputFile, const TString outputFile = "drawSpectra2D.root", const TString outputFigureName = "");

void drawSpectra2D(const TString configFile, const TString inputFile, const TString outputFile, const TString outputFigureName)
{
    std::cout<<"running drawSpectra2D()"<<std::endl;
    std::cout<<"configFile  = "<< configFile.Data() <<std::endl;
    std::cout<<"inputFile   = "<< inputFile.Data()  <<std::endl;
    std::cout<<"outputFile  = "<< outputFile.Data() <<std::endl;

    InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

    /*
     * drawing behavior :
     *      1. If N = # formulas and N = # selections, then N histograms will be drawn,
     *      2. If 1 = # formulas and N = # selections, then N histograms will be drawn with the same formula.
     *      3. If N = # formulas and 1 = # selections, then N histograms will be drawn with the same selection.
     *      4. else, exit.
     */
     // input for TTree
    std::string treePath;
    std::vector<std::string> treeFriendsPath;
    std::vector<std::string> formulas;
    std::string selectionBase;
    std::vector<std::string> selections;
    std::vector<std::string> weights;
    
    // input for TH1
    std::string title;
    std::string titleX;
    std::string titleY;
    std::vector<std::vector<float>> TH2D_Bins_List;      // nBins, xLow, xUp for the TH1D histogram
    float titleOffsetX;
    float titleOffsetY;
    int drawNormalized;
    
    // input for TLegend
    std::vector<std::string> legendEntryLabels;
    std::string legendPosition;
    float legendOffsetX;
    float legendOffsetY;

    // input for TCanvas
    int windowWidth;
    int windowHeight;
    float leftMargin;
    float rightMargin;
    float bottomMargin;
    float topMargin;
    int setLogx;
    int setLogy;
    int setLogz;
    
    if (configInput.isValid) {
        treePath  = configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treePath];
        treeFriendsPath = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treeFriends_List]);
        formulas = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treeFormula]);
        selectionBase = configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treeSelectionBase];
        selections = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treeSelection]);
        weights = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH1_weight]);

        title = ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH1_title]);
        titleX = ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH1_titleX]);
        titleY = ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH1_titleY]);
        TH2D_Bins_List = ConfigurationParser::ParseListTH2D_Bins(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH2D_Bins_List]);
        titleOffsetX = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_titleOffsetX];
        titleOffsetY = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_titleOffsetY];
        drawNormalized = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_drawNormalized];

        legendEntryLabels = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_legendEntryLabel]);
        legendPosition    = configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_legendPosition];
        legendOffsetX     = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_legendOffsetX];
        legendOffsetY     = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_legendOffsetY];

        windowWidth = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_windowWidth];
        windowHeight = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_windowHeight];
        leftMargin   = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_leftMargin];
        rightMargin  = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_rightMargin];
        bottomMargin = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_bottomMargin];
        topMargin    = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_topMargin];
        setLogx = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_setLogx];
        setLogy = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_setLogy];
        setLogz = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_setLogz];
    }
    else {
        treePath = "";
        formulas.push_back("Entry$");
        selectionBase = "";
        selections.push_back("1 == 1");
        weights.push_back("1");
        
        title = "";
        titleX = "";
        titleY = "";
        TH2D_Bins_List.resize(6);
        TH2D_Bins_List[0].push_back(100);    // nBinsx
        TH2D_Bins_List[1].push_back(0);      // xLow
        TH2D_Bins_List[2].push_back(100);    // xUp
        TH2D_Bins_List[3].push_back(100);    // nBinsy
        TH2D_Bins_List[4].push_back(0);      // yLow
        TH2D_Bins_List[5].push_back(100);    // yUp
        titleOffsetX = 1;
        titleOffsetY = 1;
        drawNormalized = 0;
        
        legendEntryLabels.push_back("");
        legendPosition = "";
        legendOffsetX = 0;
        legendOffsetY = 0;

        windowWidth = 0;
        windowHeight = 0;
        leftMargin = 0.1;
        rightMargin = 0.1;
        bottomMargin = 0.1;
        topMargin = 0.1;
        setLogx = 0;
        setLogy = 0;
        setLogz = 0;
    }
    // set default values
    if (selections.size() == 0) selections.push_back("1");
    if (weights.size() == 0)    weights.push_back("1");   // default weight = 1.
    if (titleOffsetX == 0) titleOffsetX = INPUT_DEFAULT::titleOffsetX;
    if (titleOffsetY == 0) titleOffsetY = INPUT_DEFAULT::titleOffsetY;
    if (drawNormalized >= INPUT_TH1::kN_TYPE_NORM) drawNormalized = INPUT_DEFAULT::drawNormalized;
    if (windowWidth  == 0)  windowWidth = INPUT_DEFAULT::windowWidth;
    if (windowHeight == 0)  windowHeight = INPUT_DEFAULT::windowHeight;
    if (leftMargin == 0) leftMargin = INPUT_DEFAULT::leftMargin;
    if (rightMargin == 0) rightMargin = INPUT_DEFAULT::rightMargin;
    if (bottomMargin == 0) bottomMargin = INPUT_DEFAULT::bottomMargin;
    if (topMargin == 0) topMargin = INPUT_DEFAULT::topMargin;

    int nFormulas = formulas.size();
    int nSelections = selections.size();
    int nWeights = weights.size();
    int nTH2D_Bins_List = TH2D_Bins_List[0].size();
    int nFriends = treeFriendsPath.size();
    int nLegendEntryLabels = legendEntryLabels.size();
    // verbose about input configuration
    std::cout<<"Input Configuration :"<<std::endl;
    std::cout << "treePath = " << treePath.c_str() << std::endl;
    std::cout << "nFriends = " << nFriends << std::endl;
    for (int i=0; i<nFriends; ++i) {
        std::cout << Form("treeFriends[%d] = %s", i, treeFriendsPath.at(i).c_str()) << std::endl;
    }
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
    
    std::cout << "title  = " << title.c_str() << std::endl;
    std::cout << "titleX = " << titleX.c_str() << std::endl;
    std::cout << "titleY = " << titleY.c_str() << std::endl;
    std::cout << "nTH2D_Bins_List = " << nTH2D_Bins_List << std::endl;
    for (int i=0; i<nTH2D_Bins_List; ++i) {
            std::cout << Form("nBinsx[%d] = %.0f", i, TH2D_Bins_List[0].at(i)) << std::endl;
            std::cout << Form("xLow[%d]  = %f", i, TH2D_Bins_List[1].at(i)) << std::endl;
            std::cout << Form("xUp[%d]   = %f", i, TH2D_Bins_List[2].at(i)) << std::endl;
            std::cout << Form("nBinsy[%d] = %.0f", i, TH2D_Bins_List[3].at(i)) << std::endl;
            std::cout << Form("yLow[%d]  = %f", i, TH2D_Bins_List[4].at(i)) << std::endl;
            std::cout << Form("yUp[%d]   = %f", i, TH2D_Bins_List[5].at(i)) << std::endl;
    }
    std::cout << "titleOffsetX = " << titleOffsetX << std::endl;
    std::cout << "titleOffsetY = " << titleOffsetY << std::endl;
    std::cout << "drawNormalized = " << drawNormalized << std::endl;

    std::cout << "nLegendEntryLabels   = " << nLegendEntryLabels << std::endl;
    for (int i = 0; i<nLegendEntryLabels; ++i) {
            std::cout << Form("legendEntryLabels[%d] = %s", i, legendEntryLabels.at(i).c_str()) << std::endl;
    }
    std::cout << "legendPosition   = " << legendPosition.c_str() << std::endl;
    if (legendPosition.size() == 0) std::cout<< "No position is provided, legend will not be drawn." <<std::endl;
    std::cout << "legendOffsetX    = " << legendOffsetX << std::endl;
    std::cout << "legendOffsetY    = " << legendOffsetY << std::endl;

    std::cout << "windowWidth = " << windowWidth << std::endl;
    std::cout << "windowHeight = " << windowHeight << std::endl;
    std::cout << "leftMargin   = " << leftMargin << std::endl;
    std::cout << "rightMargin  = " << rightMargin << std::endl;
    std::cout << "bottomMargin = " << bottomMargin << std::endl;
    std::cout << "topMargin    = " << topMargin << std::endl;
    std::cout << "setLogx = " << setLogx << std::endl;
    std::cout << "setLogy = " << setLogy << std::endl;
    std::cout << "setLogz = " << setLogz << std::endl;
    
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
    if (nHistos == 1 && nSelections > nFormulas) nHistos = nSelections;
    else if (nHistos > 1 && nSelections > 1 && nHistos != nSelections) {
        std::cout << "mismatch of number of formulas and number of selections"<< std::endl;
        std::cout << "nHistos     = "<< nHistos << std::endl;
        std::cout << "nSelections = "<< nSelections << std::endl;
        std::cout << "exiting " << std::endl;
        return;
    }
    std::cout << "nHistos = " << nHistos << std::endl;
    TH2D* h[nHistos];
    for (int i=0; i<nHistos; ++i) {
        int nBinsx  = (int)TH2D_Bins_List[0].at(0);
        float xLow = TH2D_Bins_List[1].at(0);
        float xUp  = TH2D_Bins_List[2].at(0);
        int nBinsy  = (int)TH2D_Bins_List[3].at(0);
        float yLow = TH2D_Bins_List[4].at(0);
        float yUp  = TH2D_Bins_List[5].at(0);
        if (nHistos == nTH2D_Bins_List) {
            nBinsx = (int)TH2D_Bins_List[0].at(i);
            xLow  = TH2D_Bins_List[1].at(i);
            xUp   = TH2D_Bins_List[2].at(i);
            nBinsy = (int)TH2D_Bins_List[3].at(i);
            yLow  = TH2D_Bins_List[4].at(i);
            yUp   = TH2D_Bins_List[5].at(i);
        }
        h[i] = new TH2D(Form("h2D_%d", i),Form("%s;%s;%s", title.c_str(), titleX.c_str(), titleY.c_str()), nBinsx, xLow, xUp, nBinsy, yLow, yUp);
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

    TH2D* h_normInt[nHistos];
    TH2D* h_normEvents[nHistos];
    for (int i=0; i<nHistos; ++i) {
        h[i]->Write();

        h_normInt[i] = (TH2D*)h[i]->Clone(Form("%s_normInt", h[i]->GetName()));
        h_normInt[i]->Scale(1./h[i]->Integral());
        h_normInt[i]->Write();

        h_normEvents[i] = (TH2D*)h[i]->Clone(Form("%s_normEvents", h[i]->GetName()));
        h_normEvents[i]->Scale(1./entriesSelected[i]);
        h_normEvents[i]->Write();
    }
    // histograms are written. After this point changes to the histograms will not be reflected in the output ROOT file.

    // write canvases
    TCanvas* c;
    for (int i=0; i<nHistos; ++i) {
        c = new TCanvas(Form("cnv_%d",i),"",windowWidth,windowHeight);
        c->SetTitle(h[i]->GetTitle());
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        setCanvasFinal(c, setLogx, setLogy, setLogz);
        c->cd();

        h[i]->SetTitleOffset(titleOffsetX,"X");
        h[i]->SetTitleOffset(titleOffsetY,"Y");
        h[i]->SetStats(false);
        h[i]->Draw("colz");
        c->Write();
        c->Close();         // do not use Delete() for TCanvas.

        // normalized by number of events
        c = new TCanvas(Form("cnv_%d_normEvents",i),"",windowWidth,windowHeight);
        c->SetTitle(h_normEvents[i]->GetTitle());
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        setCanvasFinal(c, setLogx, setLogy, setLogz);
        c->cd();

        h_normEvents[i]->SetTitleOffset(titleOffsetX,"X");
        h_normEvents[i]->SetTitleOffset(titleOffsetY,"Y");
        h_normEvents[i]->SetStats(false);
        h_normEvents[i]->Draw("colz");
        c->Write();
        c->Close();         // do not use Delete() for TCanvas.

        // normalized to 1.
        c = new TCanvas(Form("cnv_%d_normInt",i),"",windowWidth,windowHeight);
        c->SetTitle(h_normInt[i]->GetTitle());
        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
        setCanvasFinal(c, setLogx, setLogy, setLogz);
        c->cd();

        h_normInt[i]->SetTitleOffset(titleOffsetX,"X");
        h_normInt[i]->SetTitleOffset(titleOffsetY,"Y");
        h_normInt[i]->SetStats(false);
        h_normInt[i]->Draw("colz");
        c->Write();
        c->Close();         // do not use Delete() for TCanvas.
    }
    // canvases are written.

    // save histograms as picture if a figure name is provided.
    // for now 2D canvases are not drawn on top, they are drawn separately.
    if (!outputFigureName.EqualTo("")) {
        TH1D* h_draw[nHistos];
        for (int i=0; i<nHistos; ++i) {
            if (drawNormalized == INPUT_TH1::k_normInt) {
                h_draw[i] = (TH1D*)h_normInt[i]->Clone(Form("h_%d_draw", i));
            }
            else if (drawNormalized == INPUT_TH1::k_normEvents) {
                h_draw[i] = (TH1D*)h_normEvents[i]->Clone(Form("h_%d_draw", i));
            }
            else {  // no normalization
                h_draw[i] = (TH1D*)h[i]->Clone(Form("h_%d_draw", i));
            }
        }

        for (int i = 0; i<nHistos; ++i) {

            c = new TCanvas(Form("cnv_drawSpectra2D_%d", i),"",windowWidth,windowHeight);
			setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
            setCanvasFinal(c, setLogx, setLogy, setLogz);
            c->cd();
            TLegend* leg = new TLegend();

            h_draw[i]->SetTitleOffset(titleOffsetX,"X");
            h_draw[i]->SetTitleOffset(titleOffsetY,"Y");
            h_draw[i]->SetStats(false);
            h_draw[i]->Draw("colz");

            std::string label = legendEntryLabels.at(0).c_str();
            if (nHistos == nLegendEntryLabels) label = legendEntryLabels.at(i).c_str();

            leg->AddEntry(h_draw[i], label.c_str(),"pf");
            double height = calcTLegendHeight(leg);
            double width = calcTLegendWidth(leg);
            if (legendPosition.size() > 0) {    // draw the legend if really a position is provided.
                setLegendPosition(leg, legendPosition, c, height, width, legendOffsetX, legendOffsetY);
                leg->Draw();
            }

            // saving histogram
            std::string tmpOutputFigureName = outputFigureName.Data();
            if (tmpOutputFigureName.find(".") != std::string::npos) {     // file extension is specified
                if (nHistos > 1) {
                    // modify outputFile name
                    // if i=1, then "output.ext" becomes "output_2.ext"
                    size_t pos = tmpOutputFigureName.find_last_of(".");
                    tmpOutputFigureName.replace(pos,1, Form("_%d.", i+1));
                }
                c->SaveAs(tmpOutputFigureName.c_str());
            }
            else {  // file extension is NOT specified
                if (nHistos > 1) {
                    // modify outputFile name
                    // if i=1, then "output" becomes "output_2"
                    tmpOutputFigureName = Form("%s_%d", tmpOutputFigureName.c_str(), i+1);
                }

                c->SaveAs(Form("%s.C", tmpOutputFigureName.c_str()));
                c->SaveAs(Form("%s.png", tmpOutputFigureName.c_str()));
                c->SaveAs(Form("%s.pdf", tmpOutputFigureName.c_str()));
            }

            leg->Delete();
            c->Close();
        }
    }


    output->Close();
}

int main(int argc, char** argv)
{
    if (argc == 5) {
        drawSpectra2D(argv[1], argv[2], argv[3], argv[4]);
        return 0;
    }
    else if (argc == 4) {
        drawSpectra2D(argv[1], argv[2], argv[3]);
        return 0;
    }
    else if (argc == 3) {
        drawSpectra2D(argv[1], argv[2]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./drawSpectra2D.exe <configFile> <inputFile> <outputFile> (<outputFigureName>)"
                << std::endl;
        return 1;
    }
}
