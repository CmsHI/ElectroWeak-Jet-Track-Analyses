/*
 * macro to draw eta, GEN Pt, RECO Pt and centrality dependent photon energy scale plots.
 * The macro can make 4 types of 1D energy scale  histograms
 *  1. x-axis is eta.
 *  2. x-axis is GEN Pt
 *  3. x-axis is RECO Pt.
 *  4. x-axis is hiBin (centrality)
 *  Each 1D energy scale histogram has a corresponding 1D energy width (resolution) histogram.
 *  The macro provides a RECO Pt vs. GEN Pt correlation histogram for each eta and centrality bin.
 * saves histograms to a .root file.
 */

#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TObjArray.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TLine.h>
#include <TLegend.h>
#include <TLatex.h>

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>

#include "../../CorrelationTuple/EventMatcher.h"
#include "../../TreeHeaders/CutConfigurationTree.h"
#include "../../TreeHeaders/ggHiNtuplizerTree.h"
#include "../../TreeHeaders/hiEvtTree.h"
#include "../../TreeHeaders/skimAnalysisTree.h"
#include "../../Utilities/interface/ArgumentParser.h"
#include "../../Utilities/interface/CutConfigurationParser.h"
#include "../../Utilities/interface/InputConfigurationParser.h"
#include "../../Utilities/interface/HiForestInfoController.h"
#include "../../Utilities/eventUtil.h"
#include "../../Utilities/styleUtil.h"
#include "../../Utilities/th1Util.h"
#include "../../Utilities/fileUtil.h"
#include "../interface/energyScaleHist.h"

///// global variables
/// configuration variables
// input configuration
// input for TTree
std::string treePath;
int collisionType;

// input for TH1
// nBins, xLow, xUp for the TH1D histogram
// this bin list will be used for histograms where x-axis is eta.
std::vector<std::vector<float>> TH1D_Bins_List;
// nBinsx, xLow, xUp, nBinsy, yLow, yUp for a TH2D histogram
// this bin list will be used for gen pt - reco pt correlation histogram.
std::vector<std::vector<float>> TH2D_Bins_List;

std::vector<float> titleOffsetsX;
std::vector<float> titleOffsetsY;

// min. y-axis value of energy Scale histogram default : 0.8
std::vector<float> yMin;
// max. y-axis value of energy Scale histogram default : 1.5
std::vector<float> yMax;

// input for TH1 - drawing styles
std::vector<float> markerSizes;
std::vector<std::string> markerStyles;
std::vector<std::string> colors;

// input for TLegend
std::string tmpLegend;
std::vector<std::pair<std::string, int>> legendEntries;
std::vector<std::string> legendEntryLabels;
std::vector<int> legendEntryPadIndices;
std::vector<std::string> legendPositions;
std::vector<float> legendOffsetsX;
std::vector<float> legendOffsetsY;
std::vector<int> legendBorderSizes;
std::vector<float> legendWidths;
std::vector<float> legendHeights;
std::vector<float> legendTextSizes;

// input for text objects
std::string tmpText;
std::vector<std::pair<std::string, int>> textEntries;
std::vector<std::string> textLines;
std::vector<int> textLinePadIndices;
std::vector<std::string> textPositions;
std::vector<int> textFonts;
std::vector<float> textSizes;
std::vector<float> textOffsetsX;
std::vector<float> textOffsetsY;

// input for TCanvas
int windowWidth;
int windowHeight;
float leftMargin;
float rightMargin;
float bottomMargin;
float topMargin;

std::string collisionName;
int nTH1D_Bins_List;
int nTH2D_Bins_List;

int nTitleOffsetX;
int nTitleOffsetY;

int nyMin;
int nyMax;

int nMarkerSizes;
int nMarkerStyles;
int nColors;

int nLegendEntryLabels;
int nLegendPositions;
int nLegendOffsetsX;
int nLegendOffsetsY;
int nLegendBorderSizes;
int nLegendWidths;
int nLegendHeights;
int nLegendTextSizes;

int nTextLines;
int nTextPositions;
int nTextFonts;
int nTextSizes;
int nTextOffsetsX;
int nTextOffsetsY;

// cut configuration
std::vector<float> bins_eta[2];         // array of vectors for eta bins, each array element is a vector.
std::vector<float> bins_genPt[2];       // array of vectors for genPt bins, each array element is a vector, should function also as
// list of pt cuts for GEN-level photons matched to RECO photons
std::vector<float> bins_recoPt[2];      // array of vectors for recoPt bins, each array element is a vector, should function also as
// list of pt cuts for RECO photons
std::vector<int>   bins_hiBin[2];       // array of vectors for hiBin bins, each array element is a vector.

// event cuts/weights
int doEventWeight;

// RECO photon cuts
float cut_phoHoverE;
float cut_pho_ecalClusterIsoR4;
float cut_pho_hcalRechitIsoR4;
float cut_pho_trackIsoR4PtCut20;
float cut_phoSigmaIEtaIEta_2012;
float cut_sumIso;

// GEN photon cuts
float cut_mcCalIsoDR04;
float cut_mcTrkIsoDR04;
float cut_mcSumIso;

int nBins_eta;
int nBins_genPt;
int nBins_recoPt;
int nBins_hiBin;
/// configuration variables - END
// object for set of all possible energy scale histograms
energyScaleHist hist[ENERGYSCALE::kN_DEPS][10][10][10][10];
// energyScaleHist hist[ENERGYSCALE::kN_ENERGYSCALE_DEP][nBins_eta][nBins_genPt][nBins_recoPt][nBins_hiBin];
///// global variables - END

int  readConfiguration(const TString configFile);
void printConfiguration();
int  preLoop(TFile* input = 0, bool makeNew = true);
int  postLoop();
void drawSame(TCanvas* c, int iObs, int iDep, int iEta, int iGenPt, int iRecoPt, int iHiBin);
void setTH1(TH1D* h, int iHist);
void setLegend(TPad* pad, TLegend* leg, int iLeg);
void setLatex(TPad* pad, TLatex* latex, int iLatex, std::vector<std::string> textLines, TLegend* leg);
void photonEnergyScale(const TString configFile, const TString inputFile, const TString outputFile = "photonEnergyScale.root");
void photonEnergyScaleNoLoop(const TString configFile, const TString inputFile, const TString outputFile = "photonEnergyScale.root");

void photonEnergyScale(const TString configFile, const TString inputFile, const TString outputFile)
{
    std::cout<<"running photonEnergyScale()"<<std::endl;
    std::cout<<"configFile  = "<< configFile.Data() <<std::endl;
    std::cout<<"inputFile   = "<< inputFile.Data()  <<std::endl;
    std::cout<<"outputFile  = "<< outputFile.Data() <<std::endl;

    if (readConfiguration(configFile) != 0)  return;
    printConfiguration();

    std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.Data());
    std::cout<<"input ROOT files : num = "<<inputFiles.size()<< std::endl;
    std::cout<<"#####"<< std::endl;
    for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
        std::cout<<(*it).c_str()<< std::endl;
    }
    std::cout<<"##### END #####"<< std::endl;

    TH1::SetDefaultSumw2();
    // initialize histograms
    if (preLoop() != 0) return;

    TTree* treeggHiNtuplizer = 0;
    TTree* treeHiEvt = 0;
    TTree* treeHiForestInfo = 0;
    TTree* treeSkim = 0;

    int nFiles = inputFiles.size();
    TFile* fileTmp = 0;

    std::cout << "initial reading to get the number of entries (if there is only one input file) and HiForest info" << std::endl;
    // read the first file only to get the HiForest info
    std::string inputPath = inputFiles.at(0).c_str();
    fileTmp = TFile::Open(inputPath.c_str(), "READ");
    fileTmp->cd();

    if (nFiles == 1) {
        // read one tree only to get the number of entries
        treeggHiNtuplizer = (TTree*)fileTmp->Get(treePath.c_str());
        Long64_t entriesTmp = treeggHiNtuplizer->GetEntries();
        std::cout << "entries = " << entriesTmp << std::endl;
        treeggHiNtuplizer->Delete();
    }

    treeHiForestInfo = (TTree*)fileTmp->Get("HiForest/HiForestInfo");
    HiForestInfoController hfic(treeHiForestInfo);
    std::cout<<"### HiForestInfo Tree ###"<< std::endl;
    hfic.printHiForestInfo();
    std::cout<<"###"<< std::endl;

    fileTmp->Close();
    // done with initial reading

    bool isMC = collisionIsMC((COLL::TYPE)collisionType);
    bool isHI = collisionIsHI((COLL::TYPE)collisionType);
    bool isPP = collisionIsPP((COLL::TYPE)collisionType);

    if (!isMC) {
        std::cout << "This macro runs on simulation samples only." << std::endl;
        std::cout << "Change the collisionType to a simulated collisions." << std::endl;
        std::cout << "exiting" << std::endl;
        return;
    }

    EventMatcher* em = new EventMatcher();
    Long64_t duplicateEntries = 0;

    Long64_t entries = 0;
    Long64_t entriesAnalyzed = 0;

    std::cout<< "Loop : " << treePath.c_str() <<std::endl;
    for (int iFile = 0; iFile < nFiles; ++iFile)  {

        std::string inputPath = inputFiles.at(iFile).c_str();
        std::cout <<"iFile = " << iFile << " , " ;
        std::cout <<"reading input file : " << inputPath.c_str() << std::endl;
        fileTmp = TFile::Open(inputPath.c_str(), "READ");

        // check if the file is usable, if not skip the file.
        if (isGoodFile(fileTmp) != 0) {
            std::cout << "File is not good. skipping file." << std::endl;
            continue;
        }

        treeggHiNtuplizer = (TTree*)fileTmp->Get(treePath.c_str());
        treeggHiNtuplizer->SetBranchStatus("*",0);     // disable all branches
        treeggHiNtuplizer->SetBranchStatus("run",1);    // enable event information
        treeggHiNtuplizer->SetBranchStatus("event",1);
        treeggHiNtuplizer->SetBranchStatus("lumis",1);

        treeggHiNtuplizer->SetBranchStatus("nPho",1);     // enable photon branches
        treeggHiNtuplizer->SetBranchStatus("pho*",1);     // enable photon branches
        treeggHiNtuplizer->SetBranchStatus("nMC*",1);     // enable GEN particle branches
        treeggHiNtuplizer->SetBranchStatus("mc*",1);      // enable GEN particle branches
        // check existence of genMatching branch
        if (!treeggHiNtuplizer->GetBranch("pho_genMatchedIndex")) {
            std::cout << "WARNING : Branch pho_genMatchedIndex does not exist." <<std::endl;
        }

        // specify explicitly which branches to use, do not use wildcard
        treeHiEvt = (TTree*)fileTmp->Get("hiEvtAnalyzer/HiTree");
        treeHiEvt->SetBranchStatus("*",0);     // disable all branches
        treeHiEvt->SetBranchStatus("vz",1);
        treeHiEvt->SetBranchStatus("hiBin",1);
        if (doEventWeight > 0) {
            treeHiEvt->SetBranchStatus("weight", 1);
        }

        treeSkim = (TTree*)fileTmp->Get("skimanalysis/HltTree");
        treeSkim->SetBranchStatus("*",0);     // disable all branches
        if (isHI) {
            treeSkim->SetBranchStatus("pcollisionEventSelection",1);
        }
        else if (isPP) {
            treeSkim->SetBranchStatus("pPAprimaryVertexFilter",1);
            treeSkim->SetBranchStatus("pBeamScrapingFilter",1);
        }

        ggHiNtuplizer ggHi;
        ggHi.setupTreeForReading(treeggHiNtuplizer);

        hiEvt hiEvt;
        hiEvt.setupTreeForReading(treeHiEvt);

        skimAnalysis skimAna;
        skimAna.setupTreeForReading(treeSkim);
        skimAna.checkBranches(treeSkim);    // do the event selection if the branches exist.

        Long64_t entriesTmp = treeggHiNtuplizer->GetEntries();
        entries += entriesTmp;
        std::cout << "entries in File = " << entriesTmp << std::endl;
        for (Long64_t j_entry = 0; j_entry < entriesTmp; ++j_entry)
        {
            if (j_entry % 2000 == 0)  {
              std::cout << "current entry = " <<j_entry<<" out of "<<entriesTmp<<" : "<<std::setprecision(2)<<(double)j_entry/entriesTmp*100<<" %"<<std::endl;
            }

            treeggHiNtuplizer->GetEntry(j_entry);
            treeHiEvt->GetEntry(j_entry);
            treeSkim->GetEntry(j_entry);

            bool eventAdded = em->addEvent(ggHi.run, ggHi.lumis, ggHi.event, j_entry);
            if(!eventAdded) // this event is duplicate, skip this one.
            {
                duplicateEntries++;
                continue;
            }

            entriesAnalyzed++;

            // event selection
            if (!(TMath::Abs(hiEvt.vz) < 15))  continue;
            if (isHI) {
                if (skimAna.has_pcollisionEventSelection && skimAna.pcollisionEventSelection < 1)  continue;
            }
            else if (isPP) {
                if ((skimAna.has_pPAprimaryVertexFilter && skimAna.pPAprimaryVertexFilter < 1) ||
                    (skimAna.has_pBeamScrapingFilter && skimAna.pBeamScrapingFilter < 1))  continue;
            }

            double w = 1;
            int hiBin = hiEvt.hiBin;
            if (doEventWeight > 0) {
                w = hiEvt.weight;
                double vertexWeight = 1;
                if (isHI && isMC)  vertexWeight = 1.37487*TMath::Exp(-0.5*TMath::Power((hiEvt.vz-0.30709)/7.41379, 2));  // 02.04.2016
                double centWeight = 1;
                if (isHI && isMC)  centWeight = findNcoll(hiBin);
                w *= vertexWeight * centWeight;
            }

            // energy scale
            for (int i=0; i<ggHi.nPho; ++i) {

                // selections on GEN particle
                int genMatchedIndex = (*ggHi.pho_genMatchedIndex)[i];
                if (genMatchedIndex < 0)   continue;    // is matched
                if ((*ggHi.mcPID)[genMatchedIndex] != 22)   continue;    // is matched to a photon

                double genPt = (*ggHi.mcPt)[genMatchedIndex];
                if (genPt <= 0)   continue;

                if (isHI) {
                    if (cut_mcCalIsoDR04 != 0) {
                        if (!((*ggHi.mcCalIsoDR04)[genMatchedIndex] < cut_mcCalIsoDR04))   continue;
                    }
                    if (cut_mcTrkIsoDR04 != 0) {
                        if (!((*ggHi.mcTrkIsoDR04)[genMatchedIndex] < cut_mcTrkIsoDR04))   continue;
                    }
                    if (cut_mcSumIso != 0) {
                        if (!(((*ggHi.mcCalIsoDR04)[genMatchedIndex] +
                               (*ggHi.mcTrkIsoDR04)[genMatchedIndex]) < cut_mcSumIso))   continue;
                    }
                }
                else {
                    if (cut_mcCalIsoDR04 != 0) {
                        if (!((*ggHi.mcCalIsoDR04)[genMatchedIndex] < cut_mcCalIsoDR04))   continue;
                    }
                    if (cut_mcTrkIsoDR04 != 0) {
                        if (!((*ggHi.mcTrkIsoDR04)[genMatchedIndex] < cut_mcTrkIsoDR04))   continue;
                    }
                    if (cut_mcSumIso != 0) {
                        if (!(((*ggHi.mcCalIsoDR04)[genMatchedIndex] +
                               (*ggHi.mcTrkIsoDR04)[genMatchedIndex]) < cut_mcSumIso))   continue;
                    }
                }

                // selections on RECO particle
                if (!((*ggHi.phoSigmaIEtaIEta_2012)[i] > 0.002 && (*ggHi.pho_swissCrx)[i] < 0.9 && TMath::Abs((*ggHi.pho_seedTime)[i]) < 3)) continue;

                if (cut_phoHoverE != 0) {
                    if (!((*ggHi.phoHoverE)[i] < cut_phoHoverE))   continue;
                }
                if (cut_pho_ecalClusterIsoR4 != 0) {
                    if (!((*ggHi.pho_ecalClusterIsoR4)[i] < cut_pho_ecalClusterIsoR4))   continue;
                }
                if (cut_pho_hcalRechitIsoR4 != 0) {
                    if (!((*ggHi.pho_hcalRechitIsoR4)[i] < cut_pho_hcalRechitIsoR4))   continue;
                }
                if (cut_pho_trackIsoR4PtCut20 != 0) {
                    if (!((*ggHi.pho_trackIsoR4PtCut20)[i] < cut_pho_trackIsoR4PtCut20))   continue;
                }
                if (cut_phoSigmaIEtaIEta_2012 != 0) {
                    if (!((*ggHi.phoSigmaIEtaIEta_2012)[i] < cut_phoSigmaIEtaIEta_2012))   continue;
                }
                if (cut_sumIso != 0) {
                    if (!(((*ggHi.pho_ecalClusterIsoR4)[i] +
                           (*ggHi.pho_hcalRechitIsoR4)[i]  +
                           (*ggHi.pho_trackIsoR4PtCut20)[i]) < cut_sumIso))   continue;
                }

                double eta = (*ggHi.phoEta)[i];
                double pt  = (*ggHi.phoEt)[i];
                double energyScale = pt/genPt;

                for (int iEta = 0;  iEta < nBins_eta; ++iEta) {
                for (int iGenPt = 0;  iGenPt < nBins_genPt; ++iGenPt) {
                for (int iRecoPt = 0; iRecoPt < nBins_recoPt; ++iRecoPt) {
                for (int iHiBin = 0;  iHiBin < nBins_hiBin; ++iHiBin) {

                    if (iEta > 0 && iGenPt > 0 && iRecoPt > 0 && iHiBin > 0)  continue;

                    hist[ENERGYSCALE::kETA][iEta][iGenPt][iRecoPt][iHiBin].FillH2D(energyScale, eta, w, eta, genPt, pt, hiBin);
                    hist[ENERGYSCALE::kETA][iEta][iGenPt][iRecoPt][iHiBin].FillH(energyScale, w, eta, genPt, pt, hiBin);

                    hist[ENERGYSCALE::kGENPT][iEta][iGenPt][iRecoPt][iHiBin].FillH2D(energyScale, genPt, w, eta, genPt, pt, hiBin);
                    hist[ENERGYSCALE::kGENPT][iEta][iGenPt][iRecoPt][iHiBin].FillH(energyScale, w, eta, genPt, pt, hiBin);

                    hist[ENERGYSCALE::kGENPT][iEta][iGenPt][iRecoPt][iHiBin].FillH2Dcorr(genPt, pt, w, eta, hiBin);

                    hist[ENERGYSCALE::kRECOPT][iEta][iGenPt][iRecoPt][iHiBin].FillH2D(energyScale, pt, w, eta, genPt, pt, hiBin);
                    hist[ENERGYSCALE::kRECOPT][iEta][iGenPt][iRecoPt][iHiBin].FillH(energyScale, w, eta, genPt, pt, hiBin);

                    hist[ENERGYSCALE::kHIBIN][iEta][iGenPt][iRecoPt][iHiBin].FillH2D(energyScale, hiBin, w, eta, genPt, pt, hiBin);
                    hist[ENERGYSCALE::kHIBIN][iEta][iGenPt][iRecoPt][iHiBin].FillH(energyScale, w, eta, genPt, pt, hiBin);
                }}}}

            }
        }
        fileTmp->Close();
    }
    std::cout<<  "Loop ENDED : " << treePath.c_str() <<std::endl;
    std::cout << "entries            = " << entries << std::endl;
    std::cout << "duplicateEntries   = " << duplicateEntries << std::endl;
    std::cout << "entriesAnalyzed    = " << entriesAnalyzed << std::endl;

    TFile* output = TFile::Open(outputFile.Data(),"RECREATE");
    output->cd();

    postLoop();

    std::cout<<"Closing the output file."<<std::endl;
    output->Close();
    std::cout<<"running photonEnergyScale() - END"<<std::endl;
}

/*
 * run the macro without going through event loop, things done before and after the loop
 */
void photonEnergyScaleNoLoop(const TString configFile, const TString inputFile, const TString outputFile)
{
    std::cout<<"running photonEnergyScale()"<<std::endl;
    std::cout<<"configFile  = "<< configFile.Data() <<std::endl;
    std::cout<<"inputFile   = "<< inputFile.Data()  <<std::endl;
    std::cout<<"outputFile  = "<< outputFile.Data() <<std::endl;

    if (readConfiguration(configFile) != 0)  return;
    printConfiguration();

    TFile* input = new TFile(inputFile.Data(), "READ");

    TH1::SetDefaultSumw2();
    if (preLoop(input, false) != 0) return;

    TFile* output = TFile::Open(outputFile.Data(),"RECREATE");
    output->cd();

    postLoop();

    std::cout<<"Closing the input file."<<std::endl;
    input->Close();
    std::cout<<"Closing the output file."<<std::endl;
    output->Close();
    std::cout<<"running photonEnergyScale() - END"<<std::endl;
}

int main(int argc, char** argv)
{
    std::vector<std::string> argStr = ArgumentParser::ParseParameters(argc, argv);
    int nArgStr = argStr.size();

    std::vector<std::string> argOptions = ArgumentParser::ParseOptions(argc, argv);
    bool noLoop = (findPositionInVector(argOptions, ARGUMENTPARSER::noLoop) >= 0);

    if (nArgStr == 4) {
        if (noLoop) photonEnergyScaleNoLoop(argStr.at(1), argStr.at(2), argStr.at(3));
        else        photonEnergyScale(argStr.at(1), argStr.at(2), argStr.at(3));
        return 0;
    }
    else if (nArgStr == 3) {
        if (noLoop) photonEnergyScaleNoLoop(argStr.at(1), argStr.at(2));
        else        photonEnergyScale(argStr.at(1), argStr.at(2));
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./photonEnergyScale.exe <configFile> <inputFile> <outputFile>"
                << std::endl;
        return 1;
    }
}

int readConfiguration(const TString configFile)
{
    InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

    if (!configInput.isValid) {
        std::cout << "Input configuration is invalid." << std::endl;
        std::cout << "exiting" << std::endl;
        return -1;
    }
    if (!configCuts.isValid) {
        std::cout << "Cut configuration is invalid." << std::endl;
        std::cout << "exiting" << std::endl;
        return -1;
    }

    // input configuration
    // input for TTree
    treePath  = configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treePath];
    collisionType = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_collisionType];

    // input for TH1
    // nBins, xLow, xUp for the TH1D histogram
    // this bin list will be used for histograms where x-axis is eta.
    TH1D_Bins_List = ConfigurationParser::ParseListTH1D_Bins(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH1D_Bins_List]);
    // nBinsx, xLow, xUp, nBinsy, yLow, yUp for a TH2D histogram
    // this bin list will be used for gen pt - reco pt correlation histogram.
    TH2D_Bins_List = ConfigurationParser::ParseListTH2D_Bins(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH2D_Bins_List]);

    titleOffsetsX = ConfigurationParser::ParseListOrFloat(configInput.proc[INPUT::kPERFORMANCE].str_f[INPUT::k_titleOffsetX]);
    titleOffsetsY = ConfigurationParser::ParseListOrFloat(configInput.proc[INPUT::kPERFORMANCE].str_f[INPUT::k_titleOffsetY]);

    // min. y-axis value of energy Scale histogram default : 0.8
    yMin = ConfigurationParser::ParseListOrFloat(configInput.proc[INPUT::kPERFORMANCE].str_f[INPUT::k_TH1_yMin]);
    // max. y-axis value of energy Scale histogram default : 1.5
    yMax = ConfigurationParser::ParseListOrFloat(configInput.proc[INPUT::kPERFORMANCE].str_f[INPUT::k_TH1_yMax]);

    // input for TH1
    markerSizes = ConfigurationParser::ParseListOrFloat(configInput.proc[INPUT::kPERFORMANCE].str_f[INPUT::k_markerSize]);
    markerStyles = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_markerStyle]);
    colors = ConfigurationParser::ParseList(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_color]);

    // input for TLegend
    tmpLegend = ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_legendEntryLabel]);
    legendEntries = ConfigurationParser::ParseListOfList(tmpLegend);
    legendEntryLabels = ConfigurationParser::getVecString(legendEntries);
    legendEntryPadIndices = ConfigurationParser::getVecIndex(legendEntries);
    legendPositions = ConfigurationParser::ParseListOrString(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_legendPosition]);
    legendOffsetsX = ConfigurationParser::ParseListOrFloat(configInput.proc[INPUT::kPERFORMANCE].str_f[INPUT::k_legendOffsetX]);
    legendOffsetsY = ConfigurationParser::ParseListOrFloat(configInput.proc[INPUT::kPERFORMANCE].str_f[INPUT::k_legendOffsetY]);
    legendBorderSizes = ConfigurationParser::ParseListOrInteger(configInput.proc[INPUT::kPERFORMANCE].str_i[INPUT::k_legendBorderSize]);
    legendWidths = ConfigurationParser::ParseListOrFloat(configInput.proc[INPUT::kPERFORMANCE].str_f[INPUT::k_legendWidth]);
    legendHeights = ConfigurationParser::ParseListOrFloat(configInput.proc[INPUT::kPERFORMANCE].str_f[INPUT::k_legendHeight]);
    legendTextSizes = ConfigurationParser::ParseListOrFloat(configInput.proc[INPUT::kPERFORMANCE].str_f[INPUT::k_legendTextSize]);

    // input for text objects
    std::string tmpText = ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_text]);
    textEntries = ConfigurationParser::ParseListOfList(tmpText);
    textLines = ConfigurationParser::getVecString(textEntries);
    textLinePadIndices = ConfigurationParser::getVecIndex(textEntries);
    textPositions = ConfigurationParser::ParseListOrString(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_textPosition]);
    textFonts = ConfigurationParser::ParseListOrInteger(configInput.proc[INPUT::kPERFORMANCE].str_i[INPUT::k_textFont]);
    textSizes = ConfigurationParser::ParseListOrFloat(configInput.proc[INPUT::kPERFORMANCE].str_f[INPUT::k_textSize]);
    textOffsetsX = ConfigurationParser::ParseListOrFloat(configInput.proc[INPUT::kPERFORMANCE].str_f[INPUT::k_textOffsetX]);
    textOffsetsY = ConfigurationParser::ParseListOrFloat(configInput.proc[INPUT::kPERFORMANCE].str_f[INPUT::k_textOffsetY]);

    // input for TCanvas
    windowWidth = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_windowWidth];
    windowHeight = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_windowHeight];
    leftMargin   = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_leftMargin];
    rightMargin  = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_rightMargin];
    bottomMargin = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_bottomMargin];
    topMargin    = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_topMargin];

    // set default values
    if (treePath.size() == 0) treePath = "ggHiNtuplizer/EventTree";

    if (TH1D_Bins_List[0].size() == 0) {
        TH1D_Bins_List[0].push_back(12);     // nBins
        TH1D_Bins_List[1].push_back(-2.4);   // xLow
        TH1D_Bins_List[2].push_back(2.4);    // xUp
    }
    if (TH2D_Bins_List[0].size() == 0) {
        TH2D_Bins_List[0].push_back(120);    // nBinsx
        TH2D_Bins_List[1].push_back(0);      // xLow
        TH2D_Bins_List[2].push_back(120);    // xUp
        TH2D_Bins_List[3].push_back(120);    // nBinsy
        TH2D_Bins_List[4].push_back(0);      // yLow
        TH2D_Bins_List[5].push_back(120);    // yUp
    }
    
    if (titleOffsetsX.size() == 0) titleOffsetsX = {1.25};
    if (titleOffsetsY.size() == 0) titleOffsetsY = {1.75};

    nTitleOffsetX = titleOffsetsX.size();
    nTitleOffsetY = titleOffsetsY.size();
    nMarkerSizes = markerSizes.size();
    nMarkerStyles = markerStyles.size();
    nColors = colors.size();

    // yMin, yMax
    if (yMin.size() == 0)  yMin = {0};
    nyMin = yMin.size();
    if (yMax.size() == 0)  yMax = {0};
    nyMax = yMax.size();
    if (nyMin != nyMax && nyMax > 1 && nyMin > 1) {
        std::cout << "mismatch between the lengths of yMin and yMax lists" << std::endl;
        std::cout << "exiting" << std::endl;
        return -1;
    }

    if (windowWidth  == 0)  windowWidth = INPUT_DEFAULT::windowWidth;
    if (windowHeight == 0)  windowHeight = INPUT_DEFAULT::windowHeight;
    if (leftMargin == 0) leftMargin = 0.15;
    if (rightMargin == 0) rightMargin = 0.05;
    if (bottomMargin == 0) bottomMargin = INPUT_DEFAULT::bottomMargin;
    if (topMargin == 0) topMargin = INPUT_DEFAULT::topMargin;

    collisionName =  getCollisionTypeName((COLL::TYPE)collisionType).c_str();
    nTH1D_Bins_List = TH1D_Bins_List[0].size();
    nTH2D_Bins_List = TH2D_Bins_List[0].size();

    nLegendEntryLabels = legendEntryLabels.size();
    nLegendPositions = legendPositions.size();
    nLegendOffsetsX = legendOffsetsX.size();
    nLegendOffsetsY = legendOffsetsY.size();
    nLegendBorderSizes = legendBorderSizes.size();
    nLegendWidths = legendWidths.size();
    nLegendHeights = legendHeights.size();
    nLegendTextSizes = legendTextSizes.size();

    nTextLines = textLines.size();
    nTextPositions = textPositions.size();
    nTextFonts = textFonts.size();
    nTextSizes = textSizes.size();
    nTextOffsetsX = textOffsetsX.size();
    nTextOffsetsY = textOffsetsY.size();

    // cut configuration
    bins_eta[0] = ConfigurationParser::ParseListFloat(
            configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_eta_gt]);
    bins_eta[1] = ConfigurationParser::ParseListFloat(
            configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_eta_lt]);
    bins_genPt[0] = ConfigurationParser::ParseListFloat(
            configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_mcPt_gt]);
    bins_genPt[1] = ConfigurationParser::ParseListFloat(
            configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_mcPt_lt]);
    bins_recoPt[0] = ConfigurationParser::ParseListFloat(
            configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_pt_gt]);
    bins_recoPt[1] = ConfigurationParser::ParseListFloat(
            configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_pt_lt]);
    bins_hiBin[0] = ConfigurationParser::ParseListInteger(
            configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_gt]);
    bins_hiBin[1] = ConfigurationParser::ParseListInteger(
            configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_lt]);

    // event cuts/weights
    doEventWeight = configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kEVENT].i[CUTS::EVT::k_doEventWeight];

    // RECO photon cuts
    cut_phoHoverE = configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].f[CUTS::PHO::k_phoHoverE];
    cut_pho_ecalClusterIsoR4 = configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pho_ecalClusterIsoR4];
    cut_pho_hcalRechitIsoR4 = configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pho_hcalRechitIsoR4];
    cut_pho_trackIsoR4PtCut20 = configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pho_trackIsoR4PtCut20];
    cut_phoSigmaIEtaIEta_2012 = configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].f[CUTS::PHO::k_phoSigmaIEtaIEta_2012];
    cut_sumIso = configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].f[CUTS::PHO::k_sumIso];

    // GEN photon cuts
    cut_mcCalIsoDR04 = configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].f[CUTS::PHO::k_mcCalIsoDR04];
    cut_mcTrkIsoDR04 = configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].f[CUTS::PHO::k_mcTrkIsoDR04];
    cut_mcSumIso = configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].f[CUTS::PHO::k_mcSumIso];

    // set default values
    if (bins_eta[0].size() == 0) {
        bins_eta[0].push_back(0);
        bins_eta[1].push_back(2.4);
    }
    if (bins_genPt[0].size() == 0) {
        bins_genPt[0].push_back(0);
        bins_genPt[1].push_back(-1);
    }
    if (bins_recoPt[0].size() == 0) {
        bins_recoPt[0].push_back(0);
        bins_recoPt[1].push_back(-1);
    }
    if (bins_hiBin[0].size() == 0) {
        bins_hiBin[0].push_back(0);
        bins_hiBin[1].push_back(-1);
    }

    if (bins_genPt[1].size() < bins_genPt[0].size()) {
        int diff = (int signed)(bins_genPt[0].size() - bins_genPt[1].size());
        for (int i=0; i < diff ; ++i)
            bins_genPt[1].push_back(-1);
    }
    if (bins_recoPt[1].size() < bins_recoPt[0].size()) {
        int diff = (int signed)(bins_recoPt[0].size() - bins_recoPt[1].size());
        for (int i=0; i < diff; ++i)
            bins_recoPt[1].push_back(-1);
    }

    nBins_eta = bins_eta[0].size();         // assume <myvector>[0] and <myvector>[1] have the same size.
    nBins_genPt = bins_genPt[0].size();     // assume <myvector>[0] and <myvector>[1] have the same size.
    nBins_recoPt = bins_recoPt[0].size();   // assume <myvector>[0] and <myvector>[1] have the same size.
    nBins_hiBin = bins_hiBin[0].size();     // assume <myvector>[0] and <myvector>[1] have the same size.

    return 0;
}

/*
 * print information read from input/cut configurations
 * assumes that readConfiguration() is run before
 */
void printConfiguration()
{
    // verbose about input configuration
        std::cout<<"Input Configuration :"<<std::endl;
        std::cout << "treePath = " << treePath.c_str() << std::endl;
        std::cout << "collision = " << collisionName.c_str() << std::endl;

        // verbose about cut configuration
        std::cout<<"Cut Configuration :"<<std::endl;
        std::cout << "nBins_eta = " << nBins_eta << std::endl;
        for (int i=0; i<nBins_eta; ++i) {
            std::cout << Form("bins_eta[%d] = [%f, %f)", i, bins_eta[0].at(i), bins_eta[1].at(i)) << std::endl;
        }
        std::cout << "nBins_genPt = " << nBins_genPt << std::endl;
        for (int i=0; i<nBins_genPt; ++i) {
            std::cout << Form("bins_genPt[%d] = [%f, %f)", i, bins_genPt[0].at(i), bins_genPt[1].at(i)) << std::endl;
        }
        std::cout << "nBins_recoPt = " << nBins_recoPt << std::endl;
        for (int i=0; i<nBins_recoPt; ++i) {
            std::cout << Form("bins_recoPt[%d] = [%f, %f)", i, bins_recoPt[0].at(i), bins_recoPt[1].at(i)) << std::endl;
        }
        std::cout << "nBins_hiBin = " << nBins_hiBin << std::endl;
        for (int i=0; i<nBins_hiBin; ++i) {
            std::cout << Form("bins_hiBin[%d] = [%d, %d)", i, bins_hiBin[0].at(i), bins_hiBin[1].at(i)) << std::endl;
        }

        std::cout<<"doEventWeight = "<< doEventWeight <<std::endl;

        std::cout<<"cut_phoHoverE             = "<< cut_phoHoverE <<std::endl;
        std::cout<<"cut_pho_ecalClusterIsoR4  = "<< cut_pho_ecalClusterIsoR4 <<std::endl;
        std::cout<<"cut_pho_hcalRechitIsoR4   = "<< cut_pho_hcalRechitIsoR4 <<std::endl;
        std::cout<<"cut_pho_trackIsoR4PtCut20 = "<< cut_pho_trackIsoR4PtCut20 <<std::endl;
        std::cout<<"cut_phoSigmaIEtaIEta_2012 = "<< cut_phoSigmaIEtaIEta_2012 <<std::endl;
        std::cout<<"cut_sumIso                = "<< cut_sumIso <<std::endl;

        std::cout<<"cut_mcCalIsoDR04 = "<< cut_mcCalIsoDR04 <<std::endl;
        std::cout<<"cut_mcTrkIsoDR04 = "<< cut_mcTrkIsoDR04 <<std::endl;
        std::cout<<"cut_mcSumIso     = "<< cut_mcSumIso <<std::endl;

        std::cout<<"Input Configuration (Cont'd) :"<<std::endl;

        std::cout << "nTH1D_Bins_List = " << nTH1D_Bins_List << std::endl;  // for this program nTH1D_Bins_List must be 1.
        for (int i=0; i<nTH1D_Bins_List; ++i) {
            std::cout << Form("TH1D_Bins_List[%d] = { ", i);
            std::cout << Form("%.0f, ", TH1D_Bins_List[0].at(i));
            std::cout << Form("%f, ", TH1D_Bins_List[1].at(i));
            std::cout << Form("%f }", TH1D_Bins_List[2].at(i)) << std::endl;;
        }
        std::cout << "nTH2D_Bins_List = " << nTH2D_Bins_List << std::endl;  // for this program nTH2D_Bins_List must be 1.
        for (int i=0; i<nTH2D_Bins_List; ++i) {
            std::cout << Form("TH2D_Bins_List[%d] = { ", i);
            std::cout << Form("%.0f, ", TH2D_Bins_List[0].at(i));
            std::cout << Form("%f, ", TH2D_Bins_List[1].at(i));
            std::cout << Form("%f }", TH2D_Bins_List[2].at(i));
            std::cout << " { ";
            std::cout << Form("%.0f, ", TH2D_Bins_List[3].at(i));
            std::cout << Form("%f, ", TH2D_Bins_List[4].at(i));
            std::cout << Form("%f }", TH2D_Bins_List[5].at(i)) << std::endl;;
        }
        std::cout << "nTitleOffsetX = " << nTitleOffsetX << std::endl;
        for (int i = 0; i<nTitleOffsetX; ++i) {
            std::cout << Form("titleOffsetsX[%d] = %f", i, titleOffsetsX.at(i)) << std::endl;
        }
        std::cout << "nTitleOffsetY = " << nTitleOffsetY << std::endl;
        for (int i = 0; i<nTitleOffsetY; ++i) {
            std::cout << Form("titleOffsetsY[%d] = %f", i, titleOffsetsY.at(i)) << std::endl;
        }
        std::cout << "nyMin = " << nyMin << std::endl;
        std::cout << "nyMax = " << nyMax << std::endl;
        for (int i = 0; i < nyMax; ++i) {   // assume nyMax >= nyMin
            float yMinTmp = yMin.at(0);
            if (nyMin > 1) yMinTmp = yMin.at(i);
            float yMaxTmp = yMax.at(0);
            if (nyMax > 1) yMaxTmp = yMax.at(i);
            std::cout << Form("(yMin, yMax)[%d] = (%f, %f)", i, yMinTmp, yMaxTmp) << std::endl;
        }
        std::cout << "nMarkerSizes  = " << nMarkerSizes << std::endl;
        for (int i = 0; i<nMarkerSizes; ++i) {
                std::cout << Form("markerSizes[%d] = %f", i, markerSizes.at(i)) << std::endl;
        }
        std::cout << "nMarkerStyles  = " << nMarkerStyles << std::endl;
        for (int i = 0; i<nMarkerStyles; ++i) {
                std::cout << Form("markerStyles[%d] = %s", i, markerStyles.at(i).c_str()) << std::endl;
        }
        std::cout << "nColors   = " << nColors << std::endl;
        for (int i = 0; i<nColors; ++i) {
                std::cout << Form("colors[%d] = %s", i, colors.at(i).c_str()) << std::endl;
        }

        std::cout << "nLegendEntryLabels   = " << nLegendEntryLabels << std::endl;
        for (int i = 0; i<nLegendEntryLabels; ++i) {
                std::cout << Form("legendEntryLabels[%d] = %s", i, legendEntryLabels.at(i).c_str()) << std::endl;
        }
        for (int i = 0; i<nLegendEntryLabels; ++i) {
                std::cout << Form("legendEntryPadIndices[%d] = %d", i, legendEntryPadIndices.at(i)) << std::endl;
        }
        std::cout << "nLegendPositions    = " << nLegendPositions << std::endl;
        if (nLegendPositions == 0) std::cout<< "No position is provided, legend will not be drawn." <<std::endl;
        for (int i = 0; i < nLegendPositions; ++i) {
            std::cout << Form("legendPositions[%d] = %s", i, legendPositions.at(i).c_str()) << std::endl;
        }
        std::cout << "nLegendOffsetsX = " << nLegendOffsetsX << std::endl;
        for (int i = 0; i < nLegendOffsetsX; ++i) {
            std::cout << Form("legendOffsetsX[%d] = %f", i, legendOffsetsX.at(i)) << std::endl;
        }
        std::cout << "nLegendOffsetsY = " << nLegendOffsetsY << std::endl;
        for (int i = 0; i < nLegendOffsetsY; ++i) {
            std::cout << Form("legendOffsetsY[%d] = %f", i, legendOffsetsY.at(i)) << std::endl;
        }
        std::cout << "nLegendBorderSizes = " << nLegendBorderSizes << std::endl;
        for (int i = 0; i < nLegendBorderSizes; ++i) {
            std::cout << Form("legendBorderSizes[%d] = %d", i, legendBorderSizes.at(i)) << std::endl;
        }
        std::cout << "nLegendWidths = " << nLegendWidths << std::endl;
        for (int i = 0; i < nLegendWidths; ++i) {
            std::cout << Form("legendWidths[%d] = %f", i, legendWidths.at(i)) << std::endl;
        }
        std::cout << "nLegendHeights = " << nLegendHeights << std::endl;
        for (int i = 0; i < nLegendHeights; ++i) {
            std::cout << Form("legendHeights[%d] = %f", i, legendHeights.at(i)) << std::endl;
        }
        std::cout << "nLegendTextSizes = " << nLegendTextSizes << std::endl;
        for (int i = 0; i < nLegendTextSizes; ++i) {
            std::cout << Form("legendTextSizes[%d] = %f", i, legendTextSizes.at(i)) << std::endl;
        }

        std::cout << "nTextLines   = " << nTextLines << std::endl;
        for (int i = 0; i<nTextLines; ++i) {
                std::cout << Form("textLines[%d] = %s", i, textLines.at(i).c_str()) << std::endl;
        }
        std::cout << "nTextPositions = " << nTextPositions << std::endl;
        for (int i = 0; i<nTextPositions; ++i) {
            std::cout << Form("textPositions[%d] = %s", i, textPositions.at(i).c_str()) << std::endl;
        }
        std::cout << "nTextFonts = " << nTextFonts << std::endl;
        for (int i = 0; i<nTextFonts; ++i) {
            std::cout << Form("textFonts[%d] = %d", i, textFonts.at(i)) << std::endl;
        }
        std::cout << "nTextSizes = " << nTextSizes << std::endl;
        for (int i = 0; i<nTextSizes; ++i) {
            std::cout << Form("textSizes[%d] = %f", i, textSizes.at(i)) << std::endl;
        }
        std::cout << "nTextOffsetsX = " << nTextOffsetsX << std::endl;
        for (int i = 0; i<nTextOffsetsX; ++i) {
            std::cout << Form("textOffsetsX[%d] = %f", i, textOffsetsX.at(i)) << std::endl;
        }
        std::cout << "nTextOffsetsY = " << nTextOffsetsY << std::endl;
        for (int i = 0; i<nTextOffsetsY; ++i) {
            std::cout << Form("textOffsetsY[%d] = %f", i, textOffsetsY.at(i)) << std::endl;
        }

        std::cout << "windowWidth = " << windowWidth << std::endl;
        std::cout << "windowHeight = " << windowHeight << std::endl;
        std::cout << "leftMargin   = " << leftMargin << std::endl;
        std::cout << "rightMargin  = " << rightMargin << std::endl;
        std::cout << "bottomMargin = " << bottomMargin << std::endl;
        std::cout << "topMargin    = " << topMargin << std::endl;
}

/*
 * initialize/read/modify the analysis objects before the loop.
 * Objects are eg. TH1, TGraph, ...
 */
int  preLoop(TFile* input, bool makeNew)
{
    if (!makeNew) {
        // check if the file is available
        if (input == 0 ) return -1;
        else if (!input->IsOpen()) return -1;
        input->cd();
    }

    for (int iDep = 0; iDep < ENERGYSCALE::kN_DEPS; ++iDep) {
        for (int iEta = 0; iEta < nBins_eta; ++iEta) {
            for (int iGenPt = 0; iGenPt < nBins_genPt; ++iGenPt) {
                for (int iRecoPt = 0; iRecoPt < nBins_recoPt; ++iRecoPt) {
                    for (int iHiBin = 0; iHiBin < nBins_hiBin; ++iHiBin) {

                        if (iEta > 0 && iGenPt > 0 && iRecoPt > 0 && iHiBin > 0)  continue;

                        // histogram ranges
                        hist[iDep][iEta][iGenPt][iRecoPt][iHiBin].ranges[ENERGYSCALE::kETA][0] = bins_eta[0].at(iEta);
                        hist[iDep][iEta][iGenPt][iRecoPt][iHiBin].ranges[ENERGYSCALE::kETA][1] = bins_eta[1].at(iEta);
                        hist[iDep][iEta][iGenPt][iRecoPt][iHiBin].ranges[ENERGYSCALE::kGENPT][0] = bins_genPt[0].at(iGenPt);
                        hist[iDep][iEta][iGenPt][iRecoPt][iHiBin].ranges[ENERGYSCALE::kGENPT][1] = bins_genPt[1].at(iGenPt);
                        hist[iDep][iEta][iGenPt][iRecoPt][iHiBin].ranges[ENERGYSCALE::kRECOPT][0] = bins_recoPt[0].at(iRecoPt);
                        hist[iDep][iEta][iGenPt][iRecoPt][iHiBin].ranges[ENERGYSCALE::kRECOPT][1] = bins_recoPt[1].at(iRecoPt);
                        hist[iDep][iEta][iGenPt][iRecoPt][iHiBin].ranges[ENERGYSCALE::kHIBIN][0] = bins_hiBin[0].at(iHiBin);
                        hist[iDep][iEta][iGenPt][iRecoPt][iHiBin].ranges[ENERGYSCALE::kHIBIN][1] = bins_hiBin[1].at(iHiBin);

                        // for histograms with a particular dependence,
                        // a single index is used in the multidimensional array of energyScaleHist objects is used.
                        // Example : for an energy scale histogram with eta dependence (eta is the x-axis), there will not be different histograms
                        // with different eta ranges.
                        // There will be objects like : hist[ENERGYSCALE::kETA][0][iGenPt][iRecoPt][iHiBin]
                        // but not like : hist[ENERGYSCALE::kETA][1][iGenPt][iRecoPt][iHiBin]
                        // in general there will be no object hist[someIndex][iEta][iGenPt][iRecoPt][iHiBin] such that iEta, iGenpT, iRecoPt, iHiBin > 0

                        int nBinsx2D = TH2D_Bins_List[0].at(0);    // nBinsx
                        float xLow2D = TH2D_Bins_List[1].at(0);    // xLow
                        float xUp2D  = TH2D_Bins_List[2].at(0);    // xUp
                        int nBinsy2D = TH2D_Bins_List[3].at(0);    // nBinsy
                        float yLow2D = TH2D_Bins_List[4].at(0);    // yLow
                        float yUp2D  = TH2D_Bins_List[5].at(0);    // yUp

                        float nBins = TH1D_Bins_List[0].at(iDep);   // nBins
                        float xLow  = TH1D_Bins_List[1].at(iDep);   // xLow
                        float xUp   = TH1D_Bins_List[2].at(iDep);   // xUp

                        std::string strDep = "";
                        std::string xTitle = "";
                        bool makeObject = false;
                        if (iEta == 0 && iDep == ENERGYSCALE::kETA) {
                            strDep = "depEta";
                            xTitle = "photon #eta";
                            makeObject = true;
                        }
                        else if (iGenPt == 0 && iDep == ENERGYSCALE::kGENPT && nBins_genPt > 1) {
                            strDep = "depGenPt";
                            xTitle = "Gen p_{T} (GeV/c)";
                            makeObject = true;
                        }
                        else if (iRecoPt == 0 && iDep == ENERGYSCALE::kRECOPT) {
                            strDep = "depRecoPt";
                            xTitle = "Reco p_{T} (GeV/c)";
                            makeObject = true;
                        }
                        else if (iHiBin == 0 && iDep == ENERGYSCALE::kHIBIN) {
                            strDep = "depHiBin";
                            xTitle = "Hibin";
                            makeObject = true;
                        }

                        if (makeObject) {
                            std::string tmpName = Form("%s_etaBin%d_genPtBin%d_recoPtBin%d_hiBin%d", strDep.c_str(), iEta, iGenPt, iRecoPt, iHiBin);
                            hist[iDep][iEta][iGenPt][iRecoPt][iHiBin].name = tmpName.c_str();

                            std::string tmpHistName = Form("h2D_%s", tmpName.c_str());
                            std::string tmpHistName1D = Form("h_%s", tmpName.c_str());

                            // disable the cuts/ranges for this dependence
                            // Ex. If the dependence is GenPt (GenPt is the x-axis),
                            // then there will not be GenPt cuts (eg. GenPt > 20) for this histogram.
                            // The x-axis bins will set the cuts.
                            hist[iDep][iEta][iGenPt][iRecoPt][iHiBin].ranges[iDep][0] = 0;
                            hist[iDep][iEta][iGenPt][iRecoPt][iHiBin].ranges[iDep][1] = -1;

                            if (makeNew) {
                                hist[iDep][iEta][iGenPt][iRecoPt][iHiBin].h2D =
                                        new TH2D(tmpHistName.c_str(), Form(";%s;Reco p_{T} / Gen p_{T}", xTitle.c_str()), nBins, xLow, xUp, 100, 0, 2);
                                hist[iDep][iEta][iGenPt][iRecoPt][iHiBin].h =
                                        new TH1D(tmpHistName1D.c_str(), ";Reco p_{T} / Gen p_{T};", 100, 0, 2);

                                hist[iDep][iEta][iGenPt][iRecoPt][iHiBin].h2Dinitialized = true;
                                hist[iDep][iEta][iGenPt][iRecoPt][iHiBin].hInitialized = true;
                            }
                            else {
                                hist[iDep][iEta][iGenPt][iRecoPt][iHiBin].h2D = (TH2D*)input->Get(tmpHistName.c_str());
                                hist[iDep][iEta][iGenPt][iRecoPt][iHiBin].h = (TH1D*)input->Get(tmpHistName1D.c_str());

                                hist[iDep][iEta][iGenPt][iRecoPt][iHiBin].h2Dinitialized =
                                        (!hist[iDep][iEta][iGenPt][iRecoPt][iHiBin].h2D->IsZombie());
                                hist[iDep][iEta][iGenPt][iRecoPt][iHiBin].hInitialized =
                                        (!hist[iDep][iEta][iGenPt][iRecoPt][iHiBin].h->IsZombie());
                            }

                            // special cases
                            if (iDep == ENERGYSCALE::kGENPT) {
                                std::string tmpHistNameCorr = Form("h2Dcorr_%s", tmpName.c_str());

                                if (makeNew) {
                                    hist[iDep][iEta][iGenPt][iRecoPt][iHiBin].h2Dcorr =
                                            new TH2D(tmpHistNameCorr.c_str(), ";Gen p_{T};Reco p_{T}", nBinsx2D, xLow2D, xUp2D, nBinsy2D, yLow2D, yUp2D);
                                    // h2Dcorr will be used only by hist[ENERGYSCALE::kGENPT] object.
                                    // By definition, hist[ENERGYSCALE::kEta] and hist[ENERGYSCALE::kHIBIN] objects would be redundant.

                                    hist[iDep][iEta][iGenPt][iRecoPt][iHiBin].h2DcorrInitialized = true;
                                }
                                else {
                                    hist[iDep][iEta][iGenPt][iRecoPt][iHiBin].h2Dcorr = (TH2D*)input->Get(tmpHistNameCorr.c_str());
                                    // h2Dcorr will be used only by hist[ENERGYSCALE::kGENPT] object.
                                    // By definition, hist[ENERGYSCALE::kEta] and hist[ENERGYSCALE::kHIBIN] objects would be redundant.

                                    hist[iDep][iEta][iGenPt][iRecoPt][iHiBin].h2DcorrInitialized =
                                            (!hist[iDep][iEta][iGenPt][iRecoPt][iHiBin].h2Dcorr->IsZombie());
                                }
                            }

                            // set histogram title
                            hist[iDep][iEta][iGenPt][iRecoPt][iHiBin].prepareTitle();
                        }

                        if (hist[iDep][iEta][iGenPt][iRecoPt][iHiBin].h2Dinitialized) {

                            if (nyMin == 1)  hist[iDep][iEta][iGenPt][iRecoPt][iHiBin].yMin[0] = yMin[0];
                            else if (nyMin == ENERGYSCALE::OBS::kN_OBS)  hist[iDep][iEta][iGenPt][iRecoPt][iHiBin].yMin = yMin;

                            if (nyMax == 1)  hist[iDep][iEta][iGenPt][iRecoPt][iHiBin].yMax[0] = yMax[0];
                            else if (nyMax == ENERGYSCALE::OBS::kN_OBS)  hist[iDep][iEta][iGenPt][iRecoPt][iHiBin].yMax = yMax;

                            float titleOffsetXTmp = titleOffsetsX.at(0);
                            float titleOffsetYTmp = titleOffsetsY.at(0);
                            if (nTitleOffsetX == ENERGYSCALE::kN_DEPS)  titleOffsetXTmp = titleOffsetsX.at(iDep);
                            if (nTitleOffsetY == ENERGYSCALE::kN_DEPS)  titleOffsetYTmp = titleOffsetsY.at(iDep);
                            hist[iDep][iEta][iGenPt][iRecoPt][iHiBin].titleOffsetX = titleOffsetXTmp;
                            hist[iDep][iEta][iGenPt][iRecoPt][iHiBin].titleOffsetY = titleOffsetYTmp;
                        }
                    }
                }
            }
        }
    }

    return 0;
}

int postLoop()
{
    // declaration of graphics objects.
    // the idea is to initialize each Graphics object right before its use, then to delete it right after they are saved/written.
    TCanvas* c = 0;

    for (int iDep = 0; iDep < ENERGYSCALE::kN_DEPS; ++iDep) {
        for (int iEta = 0; iEta < nBins_eta; ++iEta) {
            for (int iGenPt = 0; iGenPt < nBins_genPt; ++iGenPt) {
                for (int iRecoPt = 0; iRecoPt < nBins_recoPt; ++iRecoPt) {
                    for (int iHiBin = 0; iHiBin < nBins_hiBin; ++iHiBin) {

                        if (iDep == ENERGYSCALE::kETA && iEta != 0) continue;
                        if (iDep == ENERGYSCALE::kGENPT && iGenPt != 0) continue;
                        if (iDep == ENERGYSCALE::kRECOPT && iRecoPt != 0) continue;
                        if (iDep == ENERGYSCALE::kHIBIN && iHiBin != 0) continue;

                        if (iEta > 0 && iGenPt > 0 && iRecoPt > 0 && iHiBin > 0)  continue;
                        // for histograms with a particular dependence,
                        // a single index is used in the multidimensional array of energyScaleHist objects.
                        // Example : for an energy scale histogram with eta dependence (eta is the x-axis), there will not be different histograms
                        // with different eta ranges.
                        // There will be objects like : hist[ENERGYSCALE::kETA][0][iGenPt][iRecoPt][iHiBin]
                        // but not like : hist[ENERGYSCALE::kETA][1][iGenPt][iRecoPt][iHiBin]
                        // in general there will be no object hist[someIndex][iEta][iGenPt][iRecoPt][iHiBin] such that iEta, iGenpT, iRecoPt, iHiBin > 0

                        c = new TCanvas("cnvTmp", "", windowWidth, windowHeight);
                        setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);

                        hist[iDep][iEta][iGenPt][iRecoPt][iHiBin].postLoop();
                        hist[iDep][iEta][iGenPt][iRecoPt][iHiBin].writeObjects(c);

                        c->Close();         // do not use Delete() for TCanvas.
                    }
                }
            }
        }
    }

    /*
     * plot 1D energy scale/resolution plots on top split into
     *  1. eta bins
     *  2. gen pt bins
     *  3. hiBin bins
     */
    // iObs = 0 is energy scale
    // iObs = 1 is energy resolution
    for (int iObs = 0; iObs < ENERGYSCALE::OBS::kN_OBS; ++iObs) {
        for (int iDep = 0; iDep < ENERGYSCALE::kN_DEPS; ++iDep) {

            // plot from different eta bins
            for (int iGenPt = 0; iGenPt < nBins_genPt; ++iGenPt) {
                for (int iRecoPt = 0; iRecoPt < nBins_recoPt; ++iRecoPt) {
                    for (int iHiBin = 0; iHiBin < nBins_hiBin; ++iHiBin) {

                        if (!hist[iDep][0][iGenPt][iRecoPt][iHiBin].h2Dinitialized)  continue;
                        drawSame(c, iObs, iDep, -1, iGenPt, iRecoPt, iHiBin);
                    }
                }
            }

            // plot from different genGt bins
            for (int iEta = 0; iEta < nBins_eta; ++iEta) {
                for (int iRecoPt = 0; iRecoPt < nBins_recoPt; ++iRecoPt) {
                    for (int iHiBin = 0; iHiBin < nBins_hiBin; ++iHiBin) {

                        if (!hist[iDep][iEta][0][iRecoPt][iHiBin].h2Dinitialized)  continue;
                        drawSame(c, iObs, iDep, iEta, -1, iRecoPt, iHiBin);
                    }
                }
            }
        }
    }

    return 0;
}

void drawSame(TCanvas* c, int iObs, int iDep, int iEta, int iGenPt, int iRecoPt, int iHiBin)
{
    // if the dependency is GenPt (the x-axis is GenPt), then it must be iGenPt = 0
    if (iDep == ENERGYSCALE::kETA && iEta != 0) return;
    if (iDep == ENERGYSCALE::kGENPT && iGenPt != 0) return;
    if (iDep == ENERGYSCALE::kRECOPT && iRecoPt != 0) return;
    if (iDep == ENERGYSCALE::kHIBIN && iHiBin != 0) return;

    TH1D* hTmp = 0;

    // decide which bins will be plotted on top.
    std::string tmpName = "";
    std::string strBin = "";
    std::string strBin2 = "";
    int nBins = 0;
    if (iEta == -1) {
        tmpName = hist[iDep][0][iGenPt][iRecoPt][iHiBin].name.c_str();
        strBin = "etaBin";
        strBin2 = "etaBinAll";
        nBins = nBins_eta;
    }
    else if (iGenPt == -1) {
        tmpName = hist[iDep][iEta][0][iRecoPt][iHiBin].name.c_str();
        strBin = "genPtBin";
        strBin2 = "genPtBinAll";
        nBins = nBins_genPt;
    }
    else if (iRecoPt == -1) {
        tmpName = hist[iDep][iEta][iGenPt][0][iHiBin].name.c_str();
        strBin = "recoPtBin";
        strBin2 = "recoPtBinAll";
        nBins = nBins_recoPt;
    }
    else if (iHiBin == -1 && nBins_hiBin > 1) {
        tmpName = hist[iDep][iEta][iGenPt][iRecoPt][0].name.c_str();
        strBin = "hiBin";
        strBin2 = "hiBinAll";
        nBins = nBins_hiBin;
    }
    else return;

    c = new TCanvas("cnvTmp", "", windowWidth, windowHeight);

    // replace myBinX with name myBinAll
    size_t indexStart = tmpName.find(strBin.c_str());
    tmpName.replace(indexStart, strBin.size()+1, strBin2.c_str());
    c->SetName(Form("cnv_%s_%s", ENERGYSCALE::OBS_LABELS[iObs].c_str(), tmpName.c_str()));

    setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);

    TLegend* leg = new TLegend();
    // make legend transparent
    leg->SetFillColor(-1);
    leg->SetFillStyle(4000);

    std::vector<TH1D*> vecTmp;

    for (int iBin = 0; iBin < nBins; ++iBin) {

        if (iEta == -1) hTmp = (TH1D*)hist[iDep][iBin][iGenPt][iRecoPt][iHiBin].h1D[iObs]->Clone();
        else if (iGenPt == -1) hTmp = (TH1D*)hist[iDep][iEta][iBin][iRecoPt][iHiBin].h1D[iObs]->Clone();
        else if (iRecoPt == -1) hTmp = (TH1D*)hist[iDep][iEta][iGenPt][iBin][iHiBin].h1D[iObs]->Clone();
        else if (iHiBin == -1) hTmp = (TH1D*)hist[iDep][iEta][iGenPt][iRecoPt][iBin].h1D[iObs]->Clone();

        hTmp->SetTitle("");

        int iHist = iBin;
        if (iEta == -1) iHist = iBin;
        else if (iGenPt == -1) iHist = nBins_eta + iBin;
        else if (iRecoPt == -1) iHist = nBins_eta + nBins_genPt + iBin;
        else if (iHiBin == -1) iHist = nBins_eta +  nBins_genPt + nBins_recoPt + iBin;
        setTH1(hTmp, iHist);
        vecTmp.push_back(hTmp);

        std::string legendOption = "lpf";
        std::string legendText = "";
        if (iEta == -1) legendText = hist[iDep][iBin][iGenPt][iRecoPt][iHiBin].getRangeTextEta();
        else if (iGenPt == -1) legendText = hist[iDep][iEta][iBin][iRecoPt][iHiBin].getRangeTextGenPt();
        else if (iRecoPt == -1) legendText = hist[iDep][iEta][iGenPt][iBin][iHiBin].getRangeTextRecoPt();
        else if (iHiBin == -1) legendText = hist[iDep][iEta][iGenPt][iRecoPt][iBin].getRangeTextHiBin();

        leg->AddEntry(hTmp, legendText.c_str(), legendOption.c_str());
    }

    drawSameTH1D(c, vecTmp);

    TLine* line = new TLine();
    if (iObs == ENERGYSCALE::OBS::kESCALE) {
        // draw line y = 1
        float x1 = vecTmp[0]->GetXaxis()->GetXmin();
        float x2 = vecTmp[0]->GetXaxis()->GetXmax();
        line = new TLine(x1, 1, x2,1);
        line->SetLineStyle(kDashed);
        line->Draw();
    }

    setLegend(c, leg, iDep);
    int nLegEntriesTmp = leg->GetNRows();
    if (nLegEntriesTmp > 0)  leg->Draw();

    TLatex* latex = new TLatex();

    std::vector<std::string> textLinesTmp;

    bool writeTextEta = (iDep != ENERGYSCALE::DEPS::kETA);
    bool writeTextGenPt = (iDep != ENERGYSCALE::DEPS::kGENPT);
    bool writeTextRecoPt = (iDep != ENERGYSCALE::DEPS::kRECOPT);
    bool writeTextHiBin = (iDep != ENERGYSCALE::DEPS::kHIBIN);

    std::string textLineTmp;
    if (iEta == -1) {
        if (writeTextHiBin) {
            textLineTmp = hist[iDep][0][iGenPt][iRecoPt][iHiBin].getRangeTextHiBin().c_str();
            if (textLineTmp.size() > 0) textLinesTmp.push_back(textLineTmp.c_str());
        }

        if (writeTextGenPt) {
            textLineTmp = hist[iDep][0][iGenPt][iRecoPt][iHiBin].getRangeTextGenPt().c_str();
            if (textLineTmp.size() > 0) textLinesTmp.push_back(textLineTmp.c_str());
        }

        if (writeTextRecoPt) {
            textLineTmp = hist[iDep][0][iGenPt][iRecoPt][iHiBin].getRangeTextRecoPt().c_str();
            if (textLineTmp.size() > 0) textLinesTmp.push_back(textLineTmp.c_str());
        }
    }
    else if (iGenPt == -1) {
        if (writeTextHiBin) {
            textLineTmp = hist[iDep][iEta][0][iRecoPt][iHiBin].getRangeTextHiBin().c_str();
            if (textLineTmp.size() > 0) textLinesTmp.push_back(textLineTmp.c_str());
        }

        if (writeTextEta) {
            textLineTmp = hist[iDep][iEta][0][iRecoPt][iHiBin].getRangeTextEta().c_str();
            if (textLineTmp.size() > 0) textLinesTmp.push_back(textLineTmp.c_str());
        }

        if (writeTextRecoPt) {
            textLineTmp = hist[iDep][iEta][0][iRecoPt][iHiBin].getRangeTextRecoPt().c_str();
            if (textLineTmp.size() > 0) textLinesTmp.push_back(textLineTmp.c_str());
        }
    }
    else if (iRecoPt == -1) {
        if (writeTextHiBin) {
            textLineTmp = hist[iDep][iEta][iGenPt][0][iHiBin].getRangeTextHiBin().c_str();
            if (textLineTmp.size() > 0) textLinesTmp.push_back(textLineTmp.c_str());
        }

        if (writeTextEta) {
            textLineTmp = hist[iDep][iEta][iGenPt][0][iHiBin].getRangeTextEta().c_str();
            if (textLineTmp.size() > 0) textLinesTmp.push_back(textLineTmp.c_str());
        }

        if (writeTextGenPt) {
            textLineTmp = hist[iDep][iEta][iGenPt][0][iHiBin].getRangeTextGenPt().c_str();
            if (textLineTmp.size() > 0) textLinesTmp.push_back(textLineTmp.c_str());
        }
    }
    else if (iHiBin == -1) {
        if (writeTextEta) {
            textLineTmp = hist[iDep][iEta][iGenPt][iRecoPt][0].getRangeTextEta().c_str();
            if (textLineTmp.size() > 0) textLinesTmp.push_back(textLineTmp.c_str());
        }

        if (writeTextGenPt) {
            textLineTmp = hist[iDep][iEta][iGenPt][iRecoPt][0].getRangeTextGenPt().c_str();
            if (textLineTmp.size() > 0) textLinesTmp.push_back(textLineTmp.c_str());
        }

        if (writeTextRecoPt) {
            textLineTmp = hist[iDep][iEta][iGenPt][iRecoPt][0].getRangeTextRecoPt().c_str();
            if (textLineTmp.size() > 0) textLinesTmp.push_back(textLineTmp.c_str());
        }
    }

    setLatex(c, latex, iDep, textLinesTmp, leg);

    setCanvasFinal(c);
    c->Write("",TObject::kOverwrite);

    leg->Delete();
    line->Delete();
    latex->Delete();
    c->Close();         // do not use Delete() for TCanvas.
    if (hTmp != 0)  hTmp->Delete();
}

void setTH1(TH1D* h, int iHist)
{
    float titleOffsetX = titleOffsetsX.at(0);
    h->SetTitleOffset(titleOffsetX, "X");
    float titleOffsetY = titleOffsetsY.at(0);
    h->SetTitleOffset(titleOffsetY, "Y");

    int nBinsTot = nBins_eta + nBins_genPt + nBins_recoPt + nBins_hiBin;

    int markerStyle = GRAPHICS::markerStyle;
    if (nMarkerStyles == nBinsTot) markerStyle = GraphicsConfigurationParser::ParseMarkerStyle(markerStyles.at(iHist));
    h->SetMarkerStyle(markerStyle);

    int color = GRAPHICS::colors[iHist];
    if (nColors == nBinsTot) color = GraphicsConfigurationParser::ParseColor(colors.at(iHist));
    h->SetMarkerColor(color);
    h->SetLineColor(color);

    float markerSize = markerSizes.at(0);
    if (nMarkerSizes == nBinsTot) markerSize = markerSizes.at(iHist);
    h->SetMarkerSize(markerSize);
}

void setLegend(TPad* pad, TLegend* leg, int iLeg)
{
    float legendTextSize = 0;
    if (nLegendTextSizes == 1) legendTextSize = legendTextSizes.at(0);
    else if (nLegendTextSizes == ENERGYSCALE::kN_DEPS) legendTextSize = legendTextSizes.at(iLeg);
    if (legendTextSize != 0)  leg->SetTextSize(legendTextSize);

    int legendBorderSize = 0;
    if (nLegendBorderSizes == 1) legendBorderSize = legendBorderSizes.at(0);
    else if (nLegendBorderSizes == ENERGYSCALE::kN_DEPS) legendBorderSize = legendBorderSizes.at(iLeg);
    leg->SetBorderSize(legendBorderSize);

    float legendHeight = 0;
    if (nLegendHeights == 1) legendHeight = legendHeights.at(0);
    else if (nLegendHeights == ENERGYSCALE::kN_DEPS) legendHeight = legendHeights.at(iLeg);

    float legendWidth = 0;
    if (nLegendWidths == 1) legendWidth = legendWidths.at(0);
    else if (nLegendWidths == ENERGYSCALE::kN_DEPS) legendWidth = legendWidths.at(iLeg);

    double height = calcTLegendHeight(leg);
    double width = calcTLegendWidth(leg);
    if (legendHeight != 0)  height = legendHeight;
    if (legendWidth != 0)  width = legendWidth;

    std::string legendPosition = legendPositions.at(0).c_str();
    if (nLegendPositions == ENERGYSCALE::kN_DEPS)  legendPosition = legendPositions.at(iLeg).c_str();

    float legendOffsetX = 0;
    if (nLegendOffsetsX == 1) legendOffsetX = legendOffsetsX.at(0);
    else if (nLegendOffsetsX == ENERGYSCALE::kN_DEPS) legendOffsetX = legendOffsetsX.at(iLeg);

    float legendOffsetY = 0;
    if (nLegendOffsetsY == 1) legendOffsetY = legendOffsetsY.at(0);
    else if (nLegendOffsetsY == ENERGYSCALE::kN_DEPS) legendOffsetY = legendOffsetsY.at(iLeg);

    setLegendPosition(leg, legendPosition, pad, height, width, legendOffsetX, legendOffsetY);
}

void setLatex(TPad* pad, TLatex* latex, int iLatex, std::vector<std::string> textLines, TLegend* leg)
{
    std::string textPosition = "";
    if (nTextPositions == 1)  textPosition = textPositions.at(0);
    else if (nTextPositions == ENERGYSCALE::kN_DEPS)  textPosition = textPositions.at(iLatex);
    setTextAlignment(latex, textPosition);

    float textFont = textFonts.at(0);
    if (nTextFonts == ENERGYSCALE::kN_DEPS) textFont = textFonts.at(iLatex);
    latex->SetTextFont(textFont);

    float textSize = textSizes.at(0);
    if (nTextSizes == ENERGYSCALE::kN_DEPS) textSize = textSizes.at(iLatex);
    latex->SetTextSize(textSize);

    float textOffsetX = textOffsetsX.at(0);
    if (nTextOffsetsX == ENERGYSCALE::kN_DEPS) textOffsetX = textOffsetsX.at(iLatex);

    float textOffsetY = textOffsetsY.at(0);
    if (nTextOffsetsY == ENERGYSCALE::kN_DEPS) textOffsetY = textOffsetsY.at(iLatex);

    // update y offset if legend and latex will overlap
    if (leg != 0) {
        std::string legendPosition = legendPositions.at(0).c_str();
        if (nLegendPositions == ENERGYSCALE::kN_DEPS)  legendPosition = legendPositions.at(iLatex).c_str();

        if (textPosition.find("N") == 0 && textPosition == legendPosition) {
            textOffsetY += leg->GetY2NDC() - leg->GetY1NDC();
        }
    }

    int nTextLinesTmp = textLines.size();
    std::vector<std::pair<float,float>> textCoordinates = calcTextCoordinates(textLines, textPosition, pad, textOffsetX, textOffsetY);
    for (int i = 0; i<nTextLinesTmp; ++i){
        float x = textCoordinates.at(i).first;
        float y = textCoordinates.at(i).second;
        if (textLines.at(i) != CONFIGPARSER::nullInput.c_str())
            latex->DrawLatexNDC(x, y, textLines.at(i).c_str());
    }
}
