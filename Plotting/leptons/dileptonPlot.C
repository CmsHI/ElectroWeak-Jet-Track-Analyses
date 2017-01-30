/*
 * macro to plot dilepton spectra from a histogram file
 */
#include <TFile.h>
#include <TDirectoryFile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TH1D.h>
#include <TMath.h>
#include <TLatex.h>

#include <vector>
#include <string>
#include <iostream>

#include "../../TreeHeaders/CutConfigurationTree.h"
#include "../../Utilities/interface/CutConfigurationParser.h"
#include "../../Utilities/interface/InputConfigurationParser.h"
#include "../../Utilities/eventUtil.h"
#include "../../Utilities/styleUtil.h"
#include "../../Utilities/systemUtil.h"

void dileptonPlot(const TString configFile, const TString inputFile, const TString outputFile = "dileptonPlot.root", const TString outputFigurePrefix = "");
void setTH1_diLepton(TH1* h, bool sameCharge = false);
void setTH1StyleSample(TH1 *h, COLL::TYPE collisionType);

void dileptonPlot(const TString configFile, const TString inputFile, const TString outputFile, const TString outputFigurePrefix)
{
    std::cout<<"running dileptonPlot()"<<std::endl;
    std::cout<<"configFile = "<< configFile.Data() <<std::endl;
    std::cout<<"inputFile = "<< inputFile.Data() <<std::endl;
    std::cout<<"outputFile = "<< outputFile.Data() <<std::endl;
    std::cout<<"outputFigurePrefix = "<< outputFigurePrefix.Data() <<std::endl;

    InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

    if (!configInput.isValid) {
        std::cout << "Input configuration is invalid." << std::endl;
        std::cout << "exiting" << std::endl;
        return;
    }
    if (!configCuts.isValid) {
        std::cout << "Cut configuration is invalid." << std::endl;
        std::cout << "exiting" << std::endl;
        return;
    }

    // input for TH1
    // paths of TH1D histograms
    std::vector<std::string> TH1_paths = ConfigurationParser::ParseList(configInput.proc[INPUT::kHISTOGRAM].s[INPUT::k_TH1_path]);
    float markerSize = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_markerSize];

    // input for TLegend
    float legendOffsetX  = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_legendOffsetX];
    float legendOffsetY  = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_legendOffsetY];
    int legendBorderSize = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_legendBorderSize];
    float legendWidth    = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_legendWidth];
    float legendHeight   = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_legendHeight];
    float legendTextSize = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_legendTextSize];

    // input for text objects
    int textFont = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_textFont];
    float textSize = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_textSize];
    float textOffsetX = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_textOffsetX];
    float textOffsetY = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_textOffsetY];

    std::string tmpTextOverPad = ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_textAbovePad]);
    std::vector<std::string> textsOverPad = ConfigurationParser::ParseList(tmpTextOverPad);
    std::vector<std::string> textsOverPadAlignments = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_textAbovePadAlign]);
    int textAbovePadFont = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_textAbovePadFont];
    float textAbovePadSize = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_textAbovePadSize];
    float textAbovePadOffsetX = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_textAbovePadOffsetX];
    float textAbovePadOffsetY = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_textAbovePadOffsetY];

    // input for TCanvas
    int windowWidth    = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_windowWidth];
    int windowHeight   = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_windowHeight];
    float leftMargin   = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_leftMargin];
    float rightMargin  = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_rightMargin];
    float bottomMargin = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_bottomMargin];
    float topMargin    = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_topMargin];
    int setLogx = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_setLogx];
    int setLogy = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_setLogy];

    // set default values
    if (markerSize == 0)  markerSize = INPUT_DEFAULT::markerSize;

    if (textFont == 0)  textFont = INPUT_DEFAULT::textFont;
    if (textSize == 0)  textSize = INPUT_DEFAULT::textSize;

    if (textAbovePadFont == 0)  textAbovePadFont = INPUT_DEFAULT::textAbovePadFont;
    if (textAbovePadSize == 0)  textAbovePadSize = INPUT_DEFAULT::textAbovePadSize;

    if (windowWidth  == 0)  windowWidth = INPUT_DEFAULT::windowWidth;
    if (windowHeight == 0)  windowHeight = INPUT_DEFAULT::windowHeight;
    if (leftMargin == 0) leftMargin = INPUT_DEFAULT::leftMargin;
    if (rightMargin == 0) rightMargin = INPUT_DEFAULT::rightMargin;
    if (bottomMargin == 0) bottomMargin = INPUT_DEFAULT::bottomMargin;
    if (topMargin == 0) topMargin = INPUT_DEFAULT::topMargin;

    int nTH1_Paths = TH1_paths.size();

    int nTextsOverPad = textsOverPad.size();
    int nTextsOverPadAlignments = textsOverPadAlignments.size();

    // verbose about input configuration
    std::cout<<"Input Configuration :"<<std::endl;
    std::cout << "nTH1_Paths = " << nTH1_Paths << std::endl;
    for (int i = 0; i<nTH1_Paths; ++i) {
            std::cout << Form("TH1_Path[%d] = %s", i, TH1_paths.at(i).c_str()) << std::endl;
    }
    std::cout << "markerSize = " << markerSize << std::endl;

    std::cout << "legendOffsetX    = " << legendOffsetX << std::endl;
    std::cout << "legendOffsetY    = " << legendOffsetY << std::endl;
    std::cout << "legendBorderSize = " << legendBorderSize << std::endl;
    std::cout << "legendWidth      = " << legendWidth << std::endl;
    std::cout << "legendHeight     = " << legendHeight << std::endl;
    std::cout << "legendTextSize   = " << legendTextSize << std::endl;

    std::cout << "textFont = " << textFont << std::endl;
    std::cout << "textSize = " << textSize << std::endl;
    std::cout << "textOffsetX  = " << textOffsetX << std::endl;
    std::cout << "textOffsetY  = " << textOffsetY << std::endl;

    std::cout << "nTextsOverPad = " << nTextsOverPad << std::endl;
    for (int i = 0; i<nTextsOverPad; ++i) {
            std::cout << Form("textsOverPad[%d] = %s", i, textsOverPad.at(i).c_str()) << std::endl;
    }
    if (nTextsOverPad > 0) {
        std::cout << "nTextsOverPadAlignments = " << nTextsOverPadAlignments << std::endl;
        for (int i = 0; i<nTextsOverPadAlignments; ++i) {
                std::cout << Form("textsOverPadAlignments[%d] = %s", i, textsOverPadAlignments.at(i).c_str()) << std::endl;
        }
        std::cout << "textAbovePadFont = " << textAbovePadFont << std::endl;
        std::cout << "textAbovePadSize = " << textAbovePadSize << std::endl;
        std::cout << "textAbovePadOffsetX  = " << textAbovePadOffsetX << std::endl;
        std::cout << "textAbovePadOffsetY  = " << textAbovePadOffsetY << std::endl;
    }

    std::cout << "windowWidth = " << windowWidth << std::endl;
    std::cout << "windowHeight = " << windowHeight << std::endl;
    std::cout << "leftMargin   = " << leftMargin << std::endl;
    std::cout << "rightMargin  = " << rightMargin << std::endl;
    std::cout << "bottomMargin = " << bottomMargin << std::endl;
    std::cout << "topMargin    = " << topMargin << std::endl;
    std::cout << "setLogx  = " << setLogx << std::endl;
    std::cout << "setLogy  = " << setLogy << std::endl;

    // cuts in this macro are only used for adding text to the plots
    // observable bins
    std::vector<int> bins_pt[2];        // array of vectors for lepton pt bins, each array element is a vector.
    std::vector<int> bins_hiBin[2];     // array of vectors for hiBin bins, each array element is a vector.
    bins_pt[0] = ConfigurationParser::ParseListInteger(
            configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kELECTRON].s[CUTS::ELE::k_bins_pt_gt]);
    bins_pt[1] = ConfigurationParser::ParseListInteger(
            configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kELECTRON].s[CUTS::ELE::k_bins_pt_lt]);
    bins_hiBin[0] = ConfigurationParser::ParseListInteger(
            configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_gt]);
    bins_hiBin[1] = ConfigurationParser::ParseListInteger(
            configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_lt]);

    // Z Boson cuts
    int doDiElectron = configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kZBOSON].i[CUTS::ZBO::k_doDiElectron];
    int doDiMuon = configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kZBOSON].i[CUTS::ZBO::k_doDiMuon];
    float massMin = configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kZBOSON].f[CUTS::ZBO::k_massMin];
    float massMax = configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kZBOSON].f[CUTS::ZBO::k_massMax];
    float zPt = configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kZBOSON].f[CUTS::ZBO::k_pt];

    // electron cuts
    float elePt = configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kELECTRON].f[CUTS::ELE::k_elePt];
    // muon cuts
    float muPt = configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kMUON].f[CUTS::MUO::k_muPt];

    int nBins_pt = bins_pt[0].size();         // assume <myvector>[0] and <myvector>[1] have the same size.
    int nBins_hiBin = bins_hiBin[0].size();     // assume <myvector>[0] and <myvector>[1] have the same size.
    // verbose about cut configuration
    std::cout<<"Cut Configuration :"<<std::endl;
    std::cout << "nBins_pt = " << nBins_pt << std::endl;
    for (int i=0; i<nBins_pt; ++i) {
        std::cout << Form("bins_pt[%d] = [%d, %d)", i, bins_pt[0].at(i), bins_pt[1].at(i)) << std::endl;
    }
    std::cout << "nBins_hiBin = " << nBins_hiBin << std::endl;
    for (int i=0; i<nBins_hiBin; ++i) {
        std::cout << Form("bins_hiBin[%d] = [%d, %d)", i, bins_hiBin[0].at(i), bins_hiBin[1].at(i)) << std::endl;
    }

    std::cout<<"doDiElectron = "<<doDiElectron<<std::endl;
    std::cout<<"doDiMuon     = "<<doDiMuon<<std::endl;
    std::cout << "massMin = " << massMin << std::endl;
    std::cout << "massMax = " << massMax << std::endl;
    std::cout << "zPt = " << zPt << std::endl;
    std::cout << "elePt = " << elePt << std::endl;
    std::cout << "muPt = " << muPt << std::endl;

    // inputFile should be a list of ROOT files, that is a ".txt" or ".list" file.
    // the assumed order in inputFile :
    // 1. HI DATA
    // 2. HI MC
    // 3. PP DATA
    // 4. PP MC
    std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.Data());

    int nInputFiles = inputFiles.size();
    std::cout<<"input ROOT files : num = "<<nInputFiles<< std::endl;
    if (nInputFiles != 4 )  {
        std::cout<<"need to provide 4 input files, provide non-existing files as DUMMY.root" << std::endl;
        std::cout<<"exiting" << std::endl;
        return;
    }
    else {
        std::cout<<"#####"<< std::endl;
        std::cout<<"HI DATA : "<<inputFiles.at(0).c_str()<< std::endl;
        std::cout<<"HI MC   : "<<inputFiles.at(1).c_str()<< std::endl;
        std::cout<<"PP DATA : "<<inputFiles.at(2).c_str()<< std::endl;
        std::cout<<"PP MC   : "<<inputFiles.at(3).c_str()<< std::endl;
        std::cout<<"##### END #####"<< std::endl;
    }

    TFile* input[nInputFiles];
    bool   inputExists[nInputFiles];
    TDirectoryFile* inputDir[nInputFiles];
    std::vector<int> collisionTypes {COLL::kHI, COLL::kHIMC, COLL::kPP, COLL::kPPMC};
    for (int i = 0; i<nInputFiles; ++i) {

        inputExists[i] = fileExists(inputFiles.at(i).c_str());
        if (!inputExists[i])  continue;

        input[i] = new TFile(inputFiles.at(i).c_str(), "READ");

//        std::string collisionName = getCollisionTypeName((COLL::TYPE)collisionTypes.at(i));
        inputDir[i] = (TDirectoryFile*)input[i]->Clone();
    }

    TFile* output = new TFile(outputFile, "RECREATE");
    TTree *configTree = setupConfigurationTreeForWriting(configCuts);

    std::string lepton = "";
    std::string leptonSymbol = "";
    std::string diLeptonM = "";
    std::string diLeptonPt = "";
    // float leptonPt = -1;
    // float leptonEta = -1;
    if (doDiElectron > 0) {
        lepton = "e";
        leptonSymbol = "e";
        diLeptonM = "diEleM";
        diLeptonPt = "diElePt";
        // leptonPt = elePt;
        // leptonEta = 2.4;
    }
    else if (doDiMuon > 0) {
        lepton = "m";
        leptonSymbol = "#mu";
        diLeptonM = "diMuM";
        diLeptonPt = "diMuPt";
        // leptonPt = muPt;
        // leptonEta = 2.4;
    }
    else {
        std::cout<<"None of doDiElectron and doDiMuon options are set."<<std::endl;
        std::cout<<"exiting" << std::endl;
        return;
    }

    // const variables
    // eta bins are not part of cut configuration
    const int nBins_eta = 4;
    float bins_eta_gt[nBins_eta] = {-1,    -1, 1.4791, 1.4791};    // All ECAL, Barrel, Endcap1, Endcap2
    float bins_eta_lt[nBins_eta] = {2.4, 1.44,    2.4, 2};
    if (doDiElectron > 0)
    {
        bins_eta_lt[0] = 2.5;
        bins_eta_lt[2] = 2.5;
    }

    std::vector<std::string> observables {"M", "Pt", "Eta", "Phi", "hiBin"};
    /*
    std::vector<std::string> correlationHistTitleX  {"p^{Jet}_{T}/p^{Z}_{T}", "#Delta#phi_{JZ}", "p^{Jet}_{T}"};
    std::vector<std::string> correlationHistTitleY_final_normalized{"#frac{1}{N_{Z}}#frac{dN_{JZ}}{dx_{JZ}}",
                                                                    "#frac{1}{N_{JZ}}#frac{dN_{JZ}}{d#Delta#phi}",
                                                                    "#frac{1}{N_{Z}}#frac{dN_{JZ}}{dp^{Jet}_{T}}"};
    */

    std::vector<std::string> versionSuffix {"final", "final", "final", "final", "final"};
    std::vector<std::string> legendPositions {"NW", "NE", "NW", "NW", "NE"};
    std::vector<std::string> textPositions   {"NW", "NE", "NE", "NE", "NE"};
    std::vector<bool> plotSameCharge {true, true, true, true, true};
    std::vector<bool> plotHI   {true, true, true, true, true};
    std::vector<bool> plotHIMC {true, true, true, true, true};
    std::vector<bool> plotPP   {true, true, true, true, true};
    std::vector<bool> plotPPMC {true, true, true, true, true};

    int nObservables = observables.size();

    // declaration of graphics objects.
    // the idea is to initialize each Graphics object right before its use, then to delete it right after they are saved/written.
    TCanvas* c = 0;
    TLegend* leg = 0;

    TH1::SetDefaultSumw2();
    TH1D* h1D[nInputFiles];
    bool  h1DisValid[nInputFiles];

    TH1D* h1DsameCharge[nInputFiles];
    bool  h1DsameChargeisValid[nInputFiles];
    for (int i=0; i<nObservables; ++i){
        for (int iEta=0; iEta < 2; ++iEta){
        for (int iPt=0; iPt < nBins_pt; ++iPt){
            for (int iHiBin=0; iHiBin < nBins_hiBin; ++iHiBin){

            std::string observable = observables.at(i).c_str();
            std::cout<<"##### "<< observable.c_str() <<" #####"<<std::endl;

            std::string tmpName = Form("%s_etaBin%d_ptBin%d_hiBin%d_%s", observables.at(i).c_str(), iEta, iPt, iHiBin, versionSuffix.at(i).c_str());
            std::string tmpHistName = Form("h1D_%s", tmpName.c_str());
            std::string tmpNameSameCharge = Form("%s_etaBin%d_ptBin%d_hiBin%d_sameCh_%s",
                                                                 observables.at(i).c_str(), iEta, iPt, iHiBin, versionSuffix.at(i).c_str());
            std::string tmpHistNameSameCharge = Form("h1D_%s", tmpNameSameCharge.c_str());

            std::vector<std::string> textLines;
            std::vector<std::string> textLinesChannel;
            if (doDiElectron > 0) textLinesChannel.push_back("Z #rightarrow ee");
            else if (doDiMuon > 0) textLinesChannel.push_back("Z #rightarrow #mu#mu");
            std::string line_pt = Form("p^{%s#pm}_{T} > %d GeV/c", leptonSymbol.c_str(), bins_pt[0].at(iPt));
            if (bins_pt[1].at(iPt) >= 0)  line_pt = Form("%d < p^{%s#pm}_{T} < %d", bins_pt[0].at(iPt), leptonSymbol.c_str(), bins_pt[1].at(iPt));

            std::string line_eta = Form("|#eta^{%s#pm}| < %.1f", leptonSymbol.c_str(), bins_eta_lt[iEta]);
            if (bins_eta_gt[iEta] >= 0)  line_eta = Form("%.1f< |#eta^{%s#pm}| <%.1f", bins_eta_gt[iEta], leptonSymbol.c_str(), bins_eta_lt[iEta]);

            std::string line_pt_eta = Form("%s, %s", line_pt.c_str(), line_eta.c_str());

            std::string line_hiBin = "";
            if (!(bins_hiBin[0].at(iHiBin) == 0 && bins_hiBin[1].at(iHiBin) == 200))
                line_hiBin = Form("Cent. %d-%d %%", bins_hiBin[0].at(iHiBin)/2, bins_hiBin[1].at(iHiBin)/2);

            if (observable == "M") {
                if (line_pt.size() > 0)  textLines.push_back(line_pt);
                if (line_eta.size() > 0)  textLines.push_back(line_eta);
            }
            else {
                if (line_pt_eta.size() > 0)  textLines.push_back(line_pt_eta);
                textLines.push_back(Form("%d < M^{%s%s} < %d GeV/c^{2}", (int)massMin, leptonSymbol.c_str(), leptonSymbol.c_str(), (int)massMax));
            }
            if (line_hiBin.size() > 0)   textLinesChannel.push_back(line_hiBin);
            if (zPt > 0)  textLinesChannel.push_back(Form("p_{T}^{Z} > %.0f GeV/c", zPt));

            int nTextLines = textLines.size();
            int nTextLinesChannel = textLinesChannel.size();

            // read histograms
            std::cout<<"reading histogram : "<< tmpHistName.c_str() <<std::endl;
            for (int iColl = 0; iColl < nInputFiles; ++iColl){

                h1DisValid[iColl] = false;

                h1D[iColl] = 0;
                if (inputExists[iColl]) {
                    h1DisValid[iColl] = input[iColl]->GetListOfKeys()->Contains(tmpHistName.c_str());
                    input[iColl]->GetObject(tmpHistName.c_str(), h1D[iColl]);
                }

                // same charge block
                h1DsameCharge[iColl] = 0;
                if (inputExists[iColl] && plotSameCharge.at(i))
                    input[iColl]->GetObject(tmpHistNameSameCharge.c_str(), h1DsameCharge[iColl]);

                h1DsameChargeisValid[iColl] = false;
                if (h1DsameCharge[iColl] && plotSameCharge.at(i))  h1DsameChargeisValid[iColl] = true;
            }

            // special cases
            // add pair count into TLegend
            bool writePairCount = (observable == "M");
            int count_OS = -1;
            int count_SS = -1;
            bool massPlotSetAxis = writePairCount;
            double massMin_count = 70;
            double massMax_count = 110;
            int binLow_count = -1;
            int binUp_count = -1;
            if (writePairCount) {
                if (h1DisValid[COLL::kHI] && plotHI.at(i)){
                    binLow_count = h1D[COLL::kHI]->FindBin(massMin_count);
                    binUp_count = h1D[COLL::kHI]->FindBin(massMax_count) - 1;

                    count_OS = (int)h1D[COLL::kHI]->Integral(binLow_count, binUp_count);
                    if (h1DsameChargeisValid[COLL::kHI] && plotSameCharge.at(i)) {
                        count_SS = (int)h1DsameCharge[COLL::kHI]->Integral(binLow_count, binUp_count);
                    }
                }
                else if (h1DisValid[COLL::kPP] && plotPP.at(i)) {
                    binLow_count = h1D[COLL::kPP]->FindBin(massMin_count);
                    binUp_count = h1D[COLL::kPP]->FindBin(massMax_count) - 1;

                    count_OS = (int)h1D[COLL::kPP]->Integral(binLow_count, binUp_count);
                    if (h1DsameChargeisValid[COLL::kPP] && plotSameCharge.at(i)) {
                        count_SS = (int)h1DsameCharge[COLL::kPP]->Integral(binLow_count, binUp_count);
                    }
                }
                if (count_OS > -1) {
                    textLinesChannel.push_back(Form("OS pairs : %d", count_OS));
                    if (count_SS > -1)  textLinesChannel.push_back(Form("LS pairs : %d", count_SS));
                }
            }

            // set canvas
            c = new TCanvas("cnv","",windowWidth,windowHeight);
            std::string tmpCnvName = Form("cnv_%s", tmpName.c_str());
            c->SetName(tmpCnvName.c_str());
            std::cout<<"preparing canvas  : "<< c->GetName() <<std::endl;

            setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
            setCanvasFinal(c, setLogx, setLogy);
            c->cd();

            leg = new TLegend();
            // make legend transparent
            leg->SetFillColor(-1);
            leg->SetFillStyle(4000);

            if (h1DisValid[COLL::kHI] && plotHI.at(i)) {
                setTH1StyleSample(h1D[COLL::kHI], COLL::kHI);
                setTH1_diLepton(h1D[COLL::kHI]);

                h1D[COLL::kHI]->SetMarkerSize(markerSize);

                std::string strLeg = "Opposite sign";
                // special cases
                if (writePairCount && count_OS > -1)  strLeg = Form ("Opposite sign (%d counts)", count_OS);
                leg->AddEntry(h1D[COLL::kHI], strLeg.c_str(), "lpf");

                // special cases
                if (massPlotSetAxis) h1D[COLL::kHI]->GetXaxis()->SetRange(binLow_count, binUp_count);

                if (h1DsameChargeisValid[COLL::kHI] && plotSameCharge.at(i)) {
                    setTH1StyleSample(h1DsameCharge[COLL::kHI], COLL::kHI);
                    setTH1_diLepton(h1DsameCharge[COLL::kHI], true);

                    h1DsameCharge[COLL::kHI]->SetMarkerSize(markerSize);

                    std::string strLegSameCharge = "Same sign";
                    // special cases
                    if (writePairCount && count_SS > -1)  strLegSameCharge = Form ("Same sign (%d counts)", count_SS);
                    leg->AddEntry(h1DsameCharge[COLL::kHI], strLegSameCharge.c_str(), "lpf");
                }
            }
            if (h1DisValid[COLL::kPP] && plotPP.at(i)) {
                setTH1StyleSample(h1D[COLL::kPP], COLL::kPP);
                setTH1_diLepton(h1D[COLL::kPP]);

                h1D[COLL::kPP]->SetMarkerSize(markerSize);

                std::string strLeg = "Opposite sign";
                // special cases
                if (writePairCount && count_OS > -1)  strLeg = Form ("Opposite sign (%d counts)", count_OS);
                leg->AddEntry(h1D[COLL::kPP], strLeg.c_str(), "lpf");

                // special cases
                if (massPlotSetAxis) h1D[COLL::kPP]->GetXaxis()->SetRange(binLow_count, binUp_count);

                if (h1DsameChargeisValid[COLL::kPP] && plotSameCharge.at(i)) {
                    setTH1StyleSample(h1DsameCharge[COLL::kPP], COLL::kPP);
                    setTH1_diLepton(h1DsameCharge[COLL::kPP], true);

                    h1DsameCharge[COLL::kPP]->SetMarkerSize(markerSize);

                    std::string strLegSameCharge = "Same sign";
                    // special cases
                    if (writePairCount && count_SS > -1)  strLegSameCharge = Form ("Same sign (%d counts)", count_SS);
                    leg->AddEntry(h1DsameCharge[COLL::kPP], strLegSameCharge.c_str(), "lpf");
                }
            }
            if (h1DisValid[COLL::kHIMC] && plotHIMC.at(i)) {
                // scale MC histogram
                h1D[COLL::kHIMC]->Scale(h1D[COLL::kHI]->Integral() / h1D[COLL::kHIMC]->Integral());

                setTH1StyleSample(h1D[COLL::kHIMC], COLL::kHIMC);
                setTH1_diLepton(h1D[COLL::kHIMC]);

                leg->AddEntry(h1D[COLL::kHIMC], "PYTHIA+HYDJET", "lf");

                // special cases
                if (massPlotSetAxis) h1D[COLL::kHIMC]->GetXaxis()->SetRange(binLow_count, binUp_count);
            }
            if (h1DisValid[COLL::kPPMC] && plotPPMC.at(i)) {
                // scale MC histogram
                h1D[COLL::kPPMC]->Scale(h1D[COLL::kPP]->Integral() / h1D[COLL::kPPMC]->Integral());

                setTH1StyleSample(h1D[COLL::kPPMC], COLL::kPPMC);
                setTH1_diLepton(h1D[COLL::kPPMC]);

                leg->AddEntry(h1D[COLL::kPPMC], "PYTHIA", "lf");

                // special cases
                if (massPlotSetAxis) h1D[COLL::kPPMC]->GetXaxis()->SetRange(binLow_count, binUp_count);
            }
            // set maximum/minimum of y-axis
            double histMin = -1;
            double histMax = -1;
            for (int iColl = 0; iColl < nInputFiles; ++iColl) {

                if (!h1DisValid[iColl]) continue;

                if (h1D[iColl]->GetMinimum() < histMin)   histMin = h1D[iColl]->GetMinimum();
                if (h1D[iColl]->GetMaximum() > histMax)   histMax = h1D[iColl]->GetMaximum();
            }
            int setLogy = 0;
            //if (setLogy == 0) histMin = histMin-TMath::Abs(histMin)*0.1;
            if (setLogy == 0) histMin = 0;
            histMax = histMax+TMath::Abs(histMax)*0.2*TMath::Power(10,setLogy);
            if (observable == "M") histMax *= 1.15;
            if (observable == "Eta" || observable == "Phi" ) histMax *= 1.4;

            if (h1DisValid[COLL::kHIMC] && plotHIMC.at(i)) {
                h1D[COLL::kHIMC]->SetMinimum(histMin);
                h1D[COLL::kHIMC]->SetMaximum(histMax);

                h1D[COLL::kHIMC]->Draw("hist");  // first draw "hist" option
                h1D[COLL::kHIMC]->Write();
            }
            if (h1DisValid[COLL::kPPMC] && plotPPMC.at(i)) {
                h1D[COLL::kPPMC]->SetMinimum(histMin);
                h1D[COLL::kPPMC]->SetMaximum(histMax);

                h1D[COLL::kPPMC]->Draw("hist");  // first draw "hist" option
                h1D[COLL::kPPMC]->Write();
            }
            if (h1DisValid[COLL::kHI] && plotHI.at(i)) {
                h1D[COLL::kHI]->SetMinimum(histMin);
                h1D[COLL::kHI]->SetMaximum(histMax);

                h1D[COLL::kHI]->Draw("e same");
                h1D[COLL::kHI]->Write();

                if (h1DsameChargeisValid[COLL::kHI] && plotSameCharge.at(i)) {
                    h1DsameCharge[COLL::kHI]->SetMinimum(histMin);
                    h1DsameCharge[COLL::kHI]->SetMaximum(histMax);

                    h1DsameCharge[COLL::kHI]->Draw("e same");
                    h1DsameCharge[COLL::kHI]->Write();
                }
            }
            if (h1DisValid[COLL::kPP] && plotPP.at(i)) {
                h1D[COLL::kPP]->SetMinimum(histMin);
                h1D[COLL::kPP]->SetMaximum(histMax);

                h1D[COLL::kPP]->Draw("e same");
                h1D[COLL::kPP]->Write();

                if (h1DsameChargeisValid[COLL::kPP] && plotSameCharge.at(i)) {
                    h1DsameCharge[COLL::kPP]->SetMinimum(histMin);
                    h1DsameCharge[COLL::kPP]->SetMaximum(histMax);

                    h1DsameCharge[COLL::kPP]->Draw("e same");
                    h1DsameCharge[COLL::kPP]->Write();
                }
            }

            std::string textPosition = textPositions.at(i).c_str();

            TLatex* latex = 0;
            if (nTextLines > 0) {
                latex = new TLatex();
                latex->SetTextFont(textFont);
                latex->SetTextSize(textSize);
                setTextAlignment(latex, textPosition.c_str());
                float tmpTextOffsetY = textOffsetY;
                if (textPosition != legendPositions.at(i)) {
                      if (textPosition.find("N") != std::string::npos)  tmpTextOffsetY = 0.08;
                }
                std::vector<std::pair<float,float>> textCoordinates = calcTextCoordinates(textLines, textPosition.c_str(), c, textOffsetX, tmpTextOffsetY);
                for (int i = 0; i<nTextLines; ++i){
                    float x = textCoordinates.at(i).first;
                    float y = textCoordinates.at(i).second;
                    latex->DrawLatexNDC(x, y, textLines.at(i).c_str());
                }
            }

            // add Text above the pad
            TLatex* latexOverPad = 0;
            if (nTextsOverPad > 0) {
                latexOverPad = new TLatex();
                latexOverPad->SetTextFont(textAbovePadFont);
                latexOverPad->SetTextSize(textAbovePadSize);
                for (int i = 0; i < nTextsOverPad; ++i) {
                    int textOverPadAlignment = GRAPHICS::textAlign;
                    if (nTextsOverPadAlignments == 1) textOverPadAlignment = GraphicsConfigurationParser::ParseTextAlign(textsOverPadAlignments.at(0));
                    else if (nTextsOverPadAlignments == nTextsOverPad) textOverPadAlignment = GraphicsConfigurationParser::ParseTextAlign(textsOverPadAlignments.at(i));

                    latexOverPad->SetTextAlign(textOverPadAlignment);
                    setTextAbovePad(latexOverPad, c, textAbovePadOffsetX, textAbovePadOffsetY);

                    latexOverPad->DrawLatexNDC(latexOverPad->GetX(), latexOverPad->GetY(), textsOverPad.at(i).c_str());
                }
            }

            ////////// latexCMS //////////
            bool drawLatexCMS = true;
            drawLatexCMS = (observable == "M");
            float posCMSX = 1 - c->GetRightMargin();
            float posCMSY = 1 - c->GetTopMargin();
            if (drawLatexCMS) {      // if theory models are plotted, then there is no place to add big CMS preliminary Latex.
                TLatex* latexCMS = new TLatex();
                TString cmsText     = "CMS";
                float cmsTextFont   = 61;  // default is helvetic-bold

                bool writeExtraText = true;
                TString extraText   = "Preliminary";
                extraText = "";
                float extraTextFont = 52;  // default is helvetica-italics

                // text sizes and text offsets with respect to the top frame
                // in unit of the top margin size
                float cmsTextSize      = 0.75;
                // ratio of "CMS" and extra text size
                float extraOverCmsTextSize  = 0.76;

                float relExtraDY = 1.2;

                float scaleLatexCMS = 1.2;
                latexCMS->SetTextFont(cmsTextFont);
                latexCMS->SetTextSize(cmsTextSize*c->GetTopMargin()*scaleLatexCMS);
                setTextAlignment(latexCMS, "NW");
                float posX_ = 1 - c->GetRightMargin() - 0.26;
                float posY_ = 1 - c->GetTopMargin() - 0.08;
                posCMSX = posX_;
                posCMSY = posY_;
                latexCMS->DrawLatexNDC(posX_, posY_, cmsText);
                if( writeExtraText )
                {
                    latexCMS->SetTextFont(extraTextFont);
                    latexCMS->SetTextSize(extraOverCmsTextSize*cmsTextSize*c->GetTopMargin()*scaleLatexCMS);
                    latexCMS->DrawLatexNDC(posX_, posY_- relExtraDY*cmsTextSize*c->GetTopMargin(), extraText);
                }
            }
            ////////// latexCMS - END //////////
            TLatex* latexChannel = 0;
            if (nTextLinesChannel > 0) {
                latexChannel = new TLatex();
                latexChannel->SetTextFont(textFont);
                latexChannel->SetTextSize(textSize);
                // setTextAlignment(latexChannel, textPosition.c_str());
                for (int i = 0; i<nTextLinesChannel; ++i){
                    float x = posCMSX;
                    float y = posCMSY - 0.06 - i*0.05;
                    latexChannel->DrawLatexNDC(x, y , textLinesChannel.at(i).c_str());
                }
            }

            if (legendTextSize != 0)  leg->SetTextSize(legendTextSize);
            leg->SetBorderSize(legendBorderSize);
            double height = calcTLegendHeight(leg);
            double width = calcTLegendWidth(leg);
            if (legendHeight != 0)  height = legendHeight;
            if (legendWidth != 0)  width = legendWidth;
            std::string legendPosition = legendPositions.at(i).c_str();
            setLegendPosition(leg, legendPosition.c_str(), c, height, width, legendOffsetX, legendOffsetY);
            leg->Draw();

            c->Write("",TObject::kOverwrite);
            std::string outputFigureName = c->GetName();
            if (!outputFigurePrefix.EqualTo("")) {
                outputFigureName = Form("%s%s", outputFigurePrefix.Data(), c->GetName());
            }
            c->SaveAs(Form("%s.png", outputFigureName.c_str()));
            c->SaveAs(Form("%s.pdf", outputFigureName.c_str()));
            c->SaveAs(Form("%s.C",   outputFigureName.c_str()));

            leg->Delete();
            c->Close();         // do not use Delete() for TCanvas.
            std::cout<<"##########"<<std::endl;
        }
    }
        }
    }

    configTree->Write("",TObject::kOverwrite);

    output->Write("",TObject::kOverwrite);
    for (int i = 0; i<nInputFiles; ++i) {
        if (inputExists[i])  {
            inputDir[i]->Close();
            input[i]->Close();
        }
    }
    output->Close();

    std::cout<<"dileptonPlot() - END"<<std::endl;
}

int main(int argc, char** argv)
{
    if (argc == 5) {
        dileptonPlot(argv[1], argv[2], argv[3], argv[4]);
        return 0;
    }
    else if (argc == 4) {
        dileptonPlot(argv[1], argv[2], argv[3]);
        return 0;
    }
    else if (argc == 3) {
        dileptonPlot(argv[1], argv[2]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./dileptonPlot.exe <configFile> <inputFile> <outputFile>"
                << std::endl;
        return 1;
    }
}

void setTH1_diLepton(TH1* h, bool sameCharge) {

    setTH1Final(h);

    h->SetTitleSize(1.2*h->GetTitleSize("X"), "X");
    h->SetTitleSize(1.2*h->GetTitleSize("Y"), "Y");

    h->SetTitleOffset(1.25, "X");
    h->SetTitleOffset(1.7, "Y");

    if (sameCharge) {
        h->SetMarkerStyle(kOpenSquare);
    }
}

void setTH1StyleSample(TH1 *h, COLL::TYPE collisionType) {

    if (collisionType == COLL::kHI)
    {
        h->SetMarkerStyle(kFullCircle);
        h->SetMarkerColor(kBlack);
    }
    else if (collisionType == COLL::kHIMC)
    {
        h->SetLineWidth(2);
        h->SetLineColor(kOrange+7);     // kOrange=800
        h->SetFillColor(kOrange-2);
        h->SetFillStyle(1001);
    }
    else if (collisionType == COLL::kPP) {
        h->SetMarkerStyle(kFullCircle);
        h->SetMarkerColor(kBlack);
    }
    else if (collisionType == COLL::kPPMC) {
        h->SetLineWidth(2);
        h->SetLineColor(kOrange+7);     // kOrange=800
        h->SetFillColor(kOrange-2);
        h->SetFillStyle(1001);
    }
}
