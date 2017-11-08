/*
 * macro to draw eta, reco Pt, centrality, isolation, and shower shape dependent photon trigger performance plots.
 * The macro can make 4 types of plots
 *  1. x-axis is eta.
 *  2. x-axis is reco Pt.
 *  3. x-axis is centrality (hiBin/2)
 *  4. x-axis is isolation (sumIso)
 *  5. x-axis is shower shape (sigmaIEtaIEta_2012)
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
#include "../../Utilities/physicsUtil.h"
#include "../interface/triggerAnalyzer.h"

///// global variables
/// configuration variables
// input configuration
/*
* mode is a string of bits.
* If bit j = 0, then do not run the corresponding mode
*/
std::string mode;

// input for TTree
std::string treePath;
int collisionType;

// input for TH1
// nBins, xLow, xUp for the TH1D histogram
// this bin list will be used for histograms where x-axis is eta.
std::vector<CONFIGPARSER::TH1Axis> TH1D_Axis_List;

std::string title;
float titleOffsetX;
float titleOffsetY;

std::vector<float> yMin;
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
int nTH1D_Axis_List;
int nTH2D_Axis_List;

int nTitles;
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

// triggers that go into numerator
std::vector<std::string> triggerBranchesNum;
// global denominator : list of triggers that define the subset of events to consider.
std::vector<std::string> triggerBranchesDenomGlobal;
// individual denominators
std::vector<std::string> triggerBranchesDenom;

// prescales to check in numerators and denominators
std::vector<std::string> prescaleBranchesNum;
// prescales to check only in denominators
std::vector<std::string> prescaleBranchesDenom;

std::vector<float> bins_eta[2];         // array of vectors for eta bins, each array element is a vector.
// list of pt cuts for RECO photons
std::vector<float> bins_recoPt[2];      // array of vectors for recoPt bins, each array element is a vector, should function also as
std::vector<int>   bins_cent[2];       // array of vectors for centrality bins, each array element is a vector.
// list of other cuts
std::vector<float>   bins_sumIso[2];
std::vector<float>   bins_sieie[2];
std::vector<float>   bins_r9[2];

// event cuts/weights
int doEventWeight;

// RECO photon cuts
float cut_phoHoverE;

int nTriggerBranchesNum;
int nTriggerBranchesDenomGlobal;
int nTriggerBranchesDenom;
int nPrescaleBranchesNum;
int nPrescaleBranchesDenom;

int nBins_eta;
int nBins_recoPt;
int nBins_cent;
int nBins_sumIso;
int nBins_sieie;
int nBins_r9;
/// configuration variables - END
enum MODES {
    kEff,
    kN_MODES
};
const std::string modesStr[kN_MODES] = {"Eff"};
std::vector<int> runMode;

enum ANABINS {
    kTrigger,
    kEta,
    kRecoPt,
    kCent,
    kSumIso,
    kSieie,
    kR9,
    kN_ANABINS
};
int nTriggerAna;
// object for set of all possible trigger analyzer objects
std::vector<triggerAnalyzer> tAna[TRIGGERANA::kN_DEPS];
// Each vector will have size nTriggerAna = nTriggerBranchesNum * nBins_eta * nBins_recoPt * nBins_cent * nBins_sumIso * nBins_sieie * nBins_r9

std::vector<int> indicesTriggerNum;
std::vector<int> indicesTriggerDenom;
// look up vector for which triggerAnalyzer object corresponds to which num/denom trigger
// triggerAnalyzer object with index j corresponds to num trigger with index indicesTriggerNum[j]

int nTriggers;

std::vector<int> indicesMapNum;
std::vector<int> indicesMapDenomGlobal;
std::vector<int> indicesMapDenom;
std::vector<std::string> triggerBranches;  // list of all trigger branches to be used. Elements are unique

std::vector<int> indicesMapPrescaleNum;
std::vector<int> indicesMapPrescaleDenom;
std::vector<std::string> prescaleBranches;  // list of all prescale branches to be used. Elements are unique
///// global variables - END

int  readConfiguration(const TString configFile);
void printConfiguration();
std::vector<int> parseMode(std::string mode);
int getVecIndex(std::vector<int> binIndices);
std::vector<int> getBinIndices(int i);
void indexTriggerBrances();
bool passedNum(int iTriggerNum, int triggerBits[]);
bool passedDenomGlobal(int triggerBits[]);
bool passedDenom(int iTriggerDenom, int triggerBits[]);
bool isPrescaledNum(int iTriggerNum, int triggerPrescales[]);
bool isPrescaledDenom(int iTriggerDenom, int triggerPrescales[]);
int  preLoop(TFile* input = 0, bool makeNew = true);
int  postLoop();
void drawSame(TCanvas* c, int iObs, int iDep, std::vector<int> binIndices);
void setTH1(TH1D* h, int iHist);
void setTGraph(TGraph* g, int iGraph);
void setLegend(TPad* pad, TLegend* leg, int iLeg);
void setLatex(TPad* pad, TLatex* latex, int iLatex, std::vector<std::string> textLines, TLegend* leg);
void photonTriggerAna(const TString configFile, const TString hltFile, const TString inputFile, const TString outputFile = "photonTriggerAna.root");
void photonTriggerAnaNoLoop(const TString configFile, const TString inputFile, const TString outputFile = "photonTriggerAna.root");

void photonTriggerAna(const TString configFile, const TString hltFile, const TString inputFile, const TString outputFile)
{
    std::cout<<"running photonTriggerAna()"<<std::endl;
    std::cout<<"configFile  = "<< configFile.Data() <<std::endl;
    std::cout<<"hltFile     = "<< hltFile.Data()  <<std::endl;
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

    // initialize objects
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

    std::cout<<"### HLT bit analysis file ###"<< std::endl;
    TFile* fileHlt = TFile::Open(hltFile.Data(), "READ");
    fileHlt->cd();

    TTree* treeHlt = 0;
    std::string treeHltPath = "hltbitanalysis/HltTree";
    treeHlt = (TTree*)fileHlt->Get(treeHltPath.c_str());
    treeHlt->SetBranchStatus("*",0);     // disable all branches

    // specify explicitly which branches to use
    treeHlt->SetBranchStatus("Event", 1);
    treeHlt->SetBranchStatus("LumiBlock", 1);
    treeHlt->SetBranchStatus("Run", 1);

    ULong64_t       hlt_event;
    Int_t           hlt_lumi;
    Int_t           hlt_run;
    treeHlt->SetBranchAddress("Event", &hlt_event);
    treeHlt->SetBranchAddress("LumiBlock", &hlt_lumi);
    treeHlt->SetBranchAddress("Run", &hlt_run);

    indexTriggerBrances();
    int nTriggerBranches =  triggerBranches.size();
    std::cout << "nTriggerBranches (trigger branches to read) = " << nTriggerBranches << std::endl;
    for (int i = 0; i < nTriggerBranches; ++i) {
        std::cout << Form("triggerBranches[%d] = %s", i, triggerBranches.at(i).c_str()) << std::endl;
    }

    Int_t triggerBits[nTriggerBranches];
    for (int i = 0; i < nTriggerBranches; ++i) {
        int branchSetFlag = triggerAnalyzer::setBranchAdressTrigger(treeHlt, triggerBranches[i], triggerBits[i]);
        if (branchSetFlag == -1) {
            std::cout << "set branch addresses for triggers" << std::endl;
            std::cout << "Following branch is not found : "  << triggerBranches[i].c_str() <<std::endl;
        }
    }

    int nPrescaleBranches =  prescaleBranches.size();
    std::cout << "nPrescaleBranches (prescale branches to read) = " << nPrescaleBranches << std::endl;
    for (int i = 0; i < nPrescaleBranches; ++i) {
        std::cout << Form("prescaleBranches[%d] = %s", i, prescaleBranches.at(i).c_str()) << std::endl;
        if (prescaleBranches.at(i).size() > 0 && prescaleBranches.at(i).find("Prescl") == std::string::npos) {
            std::cout << "Warning : Branch name does not contain 'Prescl'" << std::endl;
        }
    }

    Int_t triggerPrescales[nPrescaleBranches];
    for (int i = 0; i < nPrescaleBranches; ++i) {
        int branchSetFlag = triggerAnalyzer::setBranchAdressTrigger(treeHlt, prescaleBranches[i], triggerPrescales[i]);
        if (branchSetFlag == -1) {
            std::cout << "set branch addresses for prescales" << std::endl;
            std::cout << "Following branch is not found : "  << prescaleBranches[i].c_str() <<std::endl;
        }
    }

    EventMatcher* emHLT = new EventMatcher();

    Long64_t duplicateEntriesHlt = 0;

    Long64_t entriesHlt = 0;
    Long64_t entriesAnalyzedHlt = 0;

    std::cout<< "Loop HLT: " << treeHltPath.c_str() <<std::endl;
    entriesHlt = treeHlt->GetEntries();
    std::cout << "entries in HLT File = " << entriesHlt << std::endl;
    for (Long64_t j_entry = 0; j_entry < entriesHlt; ++j_entry)
    {
        if (j_entry % 2000 == 0)  {
            std::cout << "current entry = " <<j_entry<<" out of "<<entriesHlt<<" : "<<std::setprecision(2)<<(double)j_entry/entriesHlt*100<<" %"<<std::endl;
        }

        treeHlt->GetEntry(j_entry);

        bool eventAdded = emHLT->addEvent(hlt_run, hlt_lumi, hlt_event, j_entry);
        if(!eventAdded) // this event is duplicate, skip this one.
        {
            duplicateEntriesHlt++;
            continue;
        }

        entriesAnalyzedHlt++;
    }
    std::cout<<  "Loop HLT ENDED : " << treeHltPath.c_str() <<std::endl;
    std::cout << "entries HLT            = " << entriesHlt << std::endl;
    std::cout << "duplicateEntries HLT   = " << duplicateEntriesHlt << std::endl;
    std::cout << "entriesAnalyzed HLT    = " << entriesAnalyzedHlt << std::endl;
    std::cout<<"###"<< std::endl;

    bool isMC = collisionIsMC((COLL::TYPE)collisionType);
    bool isHI = collisionIsHI((COLL::TYPE)collisionType);
    bool isPP = collisionIsPP((COLL::TYPE)collisionType);

    EventMatcher* em = new EventMatcher();
    Long64_t duplicateEntries = 0;

    Long64_t entries = 0;
    Long64_t entriesNotFoundinHLT = 0;
    Long64_t entriesPassedDenomGlobal = 0;
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

            // find the event in Hlt file
            Long64_t entryHLT = emHLT->getEntry(ggHi.run, ggHi.lumis, ggHi.event);
            if (entryHLT < 0) {
                entriesNotFoundinHLT++;
                continue;
            }
            emHLT->removeEvent(ggHi.run, ggHi.lumis, ggHi.event);

            treeHlt->GetEntry(entryHLT);

            if (!passedDenomGlobal(triggerBits)) continue;
            entriesPassedDenomGlobal++;

            // event selection
            if (!(TMath::Abs(hiEvt.vz) < 15))  continue;
            if (isHI) {
                if (skimAna.has_pcollisionEventSelection && skimAna.pcollisionEventSelection < 1)  continue;
            }
            else if (isPP) {
                if ((skimAna.has_pPAprimaryVertexFilter && skimAna.pPAprimaryVertexFilter < 1) ||
                        (skimAna.has_pBeamScrapingFilter && skimAna.pBeamScrapingFilter < 1))  continue;
            }

            entriesAnalyzed++;

            double w = 1;
            int hiBin = hiEvt.hiBin;
            int cent = hiBin/2;
            if (doEventWeight > 0) {
                w = hiEvt.weight;
                double vertexWeight = 1;
                if (isHI && isMC)  vertexWeight = 1.37487*TMath::Exp(-0.5*TMath::Power((hiEvt.vz-0.30709)/7.41379, 2));  // 02.04.2016
                double centWeight = 1;
                if (isHI && isMC)  centWeight = findNcoll(hiBin);
                w *= vertexWeight * centWeight;
            }

            // efficiency
            if (runMode[MODES::kEff]) {

                for (int iAna = 0;  iAna < nTriggerAna; ++iAna) {

                    int iMax = -1;
                    double maxPt = 0;
                    for (int i=0; i<ggHi.nPho; ++i) {

                        if (!((*ggHi.phoSigmaIEtaIEta_2012)[i] > 0.002 && (*ggHi.pho_swissCrx)[i] < 0.9 && TMath::Abs((*ggHi.pho_seedTime)[i]) < 3)) continue;

                        if (cut_phoHoverE != 0) {
                            if (!((*ggHi.phoHoverE)[i] < cut_phoHoverE))   continue;
                        }

                        double pt = (*ggHi.phoEt)[i];
                        double eta = (*ggHi.phoEta)[i];
                        double sumIso = ((*ggHi.pho_ecalClusterIsoR4)[i] +
                                (*ggHi.pho_hcalRechitIsoR4)[i]  +
                                (*ggHi.pho_trackIsoR4PtCut20)[i]);
                        double sieie = (*ggHi.phoSigmaIEtaIEta_2012)[i];
                        double r9 = (*ggHi.phoR9)[i];
                        std::vector<double> vars = {eta, pt, (double)cent, sumIso, sieie, r9};

                        // triggerAnalyzer object with reco pt dependency is the correct one for this decision
                        if (!tAna[TRIGGERANA::kRECOPT][iAna].insideRange(vars)) continue;

                        if (pt > maxPt) {
                            iMax = i;
                            maxPt = pt;
                        }
                    }

                    // leading object goes into histograms
                    if(iMax == -1) continue;

                    if (isPrescaledDenom(indicesTriggerDenom[iAna], triggerPrescales))  continue;
                    if (isPrescaledNum(indicesTriggerNum[iAna], triggerPrescales))  continue;

                    if (passedDenom(indicesTriggerDenom[iAna], triggerBits)) {

                        double pt = (*ggHi.phoEt)[iMax];
                        double eta = (*ggHi.phoEta)[iMax];
                        double sumIso = ((*ggHi.pho_ecalClusterIsoR4)[iMax] +
                                (*ggHi.pho_hcalRechitIsoR4)[iMax]  +
                                (*ggHi.pho_trackIsoR4PtCut20)[iMax]);
                        double sieie = (*ggHi.phoSigmaIEtaIEta_2012)[iMax];
                        double r9 = (*ggHi.phoR9)[iMax];
                        std::vector<double> vars = {eta, pt, (double)cent, sumIso, sieie, r9};

                        tAna[TRIGGERANA::kETA][iAna].FillHDenom(eta, w, vars);
                        tAna[TRIGGERANA::kRECOPT][iAna].FillHDenom(pt, w, vars);
                        tAna[TRIGGERANA::kCENT][iAna].FillHDenom(cent, w, vars);
                        tAna[TRIGGERANA::kSUMISO][iAna].FillHDenom(sumIso, w, vars);
                        tAna[TRIGGERANA::kSIEIE][iAna].FillHDenom(sieie, w, vars);

                        if (passedNum(indicesTriggerNum[iAna], triggerBits)) {

                            tAna[TRIGGERANA::kETA][iAna].FillHNum(eta, w, vars);
                            tAna[TRIGGERANA::kRECOPT][iAna].FillHNum(pt, w, vars);
                            tAna[TRIGGERANA::kCENT][iAna].FillHNum(cent, w, vars);
                            tAna[TRIGGERANA::kSUMISO][iAna].FillHNum(sumIso, w, vars);
                            tAna[TRIGGERANA::kSIEIE][iAna].FillHNum(sieie, w, vars);
                        }
                    }

                }
            }

        }
        fileTmp->Close();
    }
    std::cout<<  "Loop ENDED : " << treePath.c_str() <<std::endl;
    std::cout << "entries            = " << entries << std::endl;
    std::cout << "duplicateEntries   = " << duplicateEntries << std::endl;
    std::cout << "entriesNotFoundinHLT = " << entriesNotFoundinHLT << std::endl;
    std::cout << "entriesPassedDenomGlobal = " << entriesPassedDenomGlobal << std::endl;
    std::cout << "entriesAnalyzed    = " << entriesAnalyzed << std::endl;

    TFile* output = TFile::Open(outputFile.Data(),"RECREATE");
    output->cd();

    postLoop();

    std::cout<<"Closing the HLT file."<<std::endl;
    fileHlt->Close();

    std::cout<<"Closing the output file."<<std::endl;
    output->Close();
    std::cout<<"running photonTriggerAna() - END"<<std::endl;
}

/*
 * run the macro without going through event loop, things done before and after the loop
 */
void photonTriggerAnaNoLoop(const TString configFile, const TString inputFile, const TString outputFile)
{
    std::cout<<"running photonTriggerAna()"<<std::endl;
    std::cout<<"configFile  = "<< configFile.Data() <<std::endl;
    std::cout<<"inputFile   = "<< inputFile.Data()  <<std::endl;
    std::cout<<"outputFile  = "<< outputFile.Data() <<std::endl;

    if (readConfiguration(configFile) != 0)  return;
    printConfiguration();

    TFile* input = new TFile(inputFile.Data(), "READ");

    if (preLoop(input, false) != 0) return;

    TFile* output = TFile::Open(outputFile.Data(),"RECREATE");
    output->cd();

    postLoop();

    std::cout<<"Closing the input file."<<std::endl;
    input->Close();
    std::cout<<"Closing the output file."<<std::endl;
    output->Close();
    std::cout<<"running photonTriggerAna() - END"<<std::endl;
}

int main(int argc, char** argv)
{
    std::vector<std::string> argStr = ArgumentParser::ParseParameters(argc, argv);
    int nArgStr = argStr.size();

    std::vector<std::string> argOptions = ArgumentParser::ParseOptions(argc, argv);
    bool noLoop = (findPositionInVector(argOptions, ARGUMENTPARSER::noLoop) >= 0);

    if (nArgStr == 5) {
        photonTriggerAna(argStr.at(1), argStr.at(2), argStr.at(3), argStr.at(4));
        return 0;
    }
    else if (nArgStr == 4) {
        if (noLoop) photonTriggerAnaNoLoop(argStr.at(1), argStr.at(2), argStr.at(3));
        else        photonTriggerAna(argStr.at(1), argStr.at(2), argStr.at(3));
        return 0;
    }
    else if (nArgStr == 3) {
        if (noLoop) photonTriggerAnaNoLoop(argStr.at(1), argStr.at(2));
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./photonTriggerAna.exe <configFile> <hltFile> <inputFile> <outputFile>"
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
    mode = configInput.proc[INPUT::kPERFORMANCE].str_i[INPUT::k_mode];
    runMode = parseMode(mode);

    // input for TTree
    treePath  = configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treePath];
    collisionType = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_collisionType];

    // input for TH1
    // nBins, xLow, xUp for the TH1D histogram
    // this bin list will be used for histograms where x-axis is eta.
    TH1D_Axis_List = ConfigurationParser::ParseListTH1D_Axis(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH1D_Bins_List]);

    title = ConfigurationParser::ParseLatex(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH1_title]);
    titleOffsetX = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_titleOffsetX];
    titleOffsetY = configInput.proc[INPUT::kPERFORMANCE].f[INPUT::k_titleOffsetY];

    yMin = ConfigurationParser::ParseListOrFloat(configInput.proc[INPUT::kPERFORMANCE].str_f[INPUT::k_TH1_yMin]);
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

    if (TH1D_Axis_List.size() == 0) {
        CONFIGPARSER::TH1Axis th1Axis;
        th1Axis.nBins = 12;
        th1Axis.xLow = -2.4;
        th1Axis.xUp = 2.4;
        TH1D_Axis_List.push_back(th1Axis);
    }

    if (titleOffsetX <= 0) titleOffsetX = INPUT_DEFAULT::titleOffsetX;
    if (titleOffsetY <= 0) titleOffsetY = INPUT_DEFAULT::titleOffsetY;

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
    nTH1D_Axis_List = TH1D_Axis_List.size();

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
    triggerBranchesNum = ConfigurationParser::ParseList(configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_triggerNum_List]);
    triggerBranchesDenomGlobal = ConfigurationParser::ParseList(configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_triggerDenomGlobal_List]);
    triggerBranchesDenom = ConfigurationParser::ParseList(configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_triggerDenom_List]);

    prescaleBranchesNum = ConfigurationParser::ParseList(configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_triggerNumPrescl_List]);
    prescaleBranchesDenom = ConfigurationParser::ParseList(configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_triggerDenomPrescl_List]);

    bins_eta[0] = ConfigurationParser::ParseListFloat(
            configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_eta_gt]);
    bins_eta[1] = ConfigurationParser::ParseListFloat(
            configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_eta_lt]);
    bins_recoPt[0] = ConfigurationParser::ParseListFloat(
            configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_pt_gt]);
    bins_recoPt[1] = ConfigurationParser::ParseListFloat(
            configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_pt_lt]);
    bins_cent[0] = ConfigurationParser::ParseListInteger(
            configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_cent_gt]);
    bins_cent[1] = ConfigurationParser::ParseListInteger(
            configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_cent_lt]);
    bins_sumIso[0] = ConfigurationParser::ParseListFloat(
            configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_sumIso_gt]);
    bins_sumIso[1] = ConfigurationParser::ParseListFloat(
            configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_sumIso_lt]);
    bins_sieie[0] = ConfigurationParser::ParseListFloat(
            configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_sieie_gt]);
    bins_sieie[1] = ConfigurationParser::ParseListFloat(
            configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_sieie_lt]);
    bins_r9[0] = ConfigurationParser::ParseListFloat(
            configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_r9_gt]);
    bins_r9[1] = ConfigurationParser::ParseListFloat(
            configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_r9_lt]);

    // event cuts/weights
    doEventWeight = configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kEVENT].i[CUTS::EVT::k_doEventWeight];

    // RECO photon cuts
    cut_phoHoverE = configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].f[CUTS::PHO::k_phoHoverE];

    // set default values
    if (bins_eta[0].size() == 0) {
        bins_eta[0].push_back(0);
        bins_eta[1].push_back(2.4);
    }
    if (bins_recoPt[0].size() == 0) {
        bins_recoPt[0].push_back(0);
        bins_recoPt[1].push_back(-1);
    }
    if (bins_cent[0].size() == 0) {
        bins_cent[0].push_back(0);
        bins_cent[1].push_back(-1);
    }
    if (bins_sumIso[0].size() == 0) {
        bins_sumIso[0].push_back(-999);
        bins_sumIso[1].push_back(-999);
    }
    if (bins_sieie[0].size() == 0) {
        bins_sieie[0].push_back(0);
        bins_sieie[1].push_back(-1);
    }
    if (bins_r9[0].size() == 0) {
        bins_r9[0].push_back(0);
        bins_r9[1].push_back(-1);
    }

    if (bins_recoPt[1].size() < bins_recoPt[0].size()) {
        int diff = (int signed)(bins_recoPt[0].size() - bins_recoPt[1].size());
        for (int i=0; i < diff; ++i)
            bins_recoPt[1].push_back(-1);
    }

    nTriggerBranchesNum = triggerBranchesNum.size();
    nTriggerBranchesDenomGlobal = triggerBranchesDenomGlobal.size();
    nTriggerBranchesDenom = triggerBranchesDenom.size();
    nPrescaleBranchesNum = prescaleBranchesNum.size();
    nPrescaleBranchesDenom = prescaleBranchesDenom.size();

    if (nTriggerBranchesNum > 1 && nTriggerBranchesDenom > 1 && nTriggerBranchesNum != nTriggerBranchesDenom) {
        std::cout << "mismatch in the number of numerators and denominators :" << std::endl;
        std::cout << "nTriggerBranchesNum = " << nTriggerBranchesNum << std::endl;
        std::cout << "nTriggerBranchesDenom = " << nTriggerBranchesDenom << std::endl;
        std::cout << "exiting" << std::endl;
        return -1;
    }

    nTriggers = nTriggerBranchesNum;
    if (nTriggerBranchesDenom > 1) nTriggers = nTriggerBranchesDenom;

    if (nPrescaleBranchesNum > 0 && nPrescaleBranchesNum != nTriggerBranchesNum)
    {
        std::cout << "mismatch in the number of numerators and prescales to be used for numerators :" << std::endl;
        std::cout << "nTriggerBranchesNum = " << nTriggerBranchesNum << " and nPrescaleBranchesNum = " << nPrescaleBranchesNum << std::endl;
        std::cout << "exiting" << std::endl;
        return -1;
    }
    if (nPrescaleBranchesDenom > 0 && nPrescaleBranchesDenom != nTriggerBranchesDenom)
    {
        std::cout << "mismatch in the number of denominators and prescales to be used for denominators :" << std::endl;
        std::cout << "nTriggerBranchesDenom = " << nTriggerBranchesDenom << " and nPrescaleBranchesDenom = " << nPrescaleBranchesDenom << std::endl;
        std::cout << "exiting" << std::endl;
        return -1;
    }

    nBins_eta = bins_eta[0].size();         // assume <myvector>[0] and <myvector>[1] have the same size.
    nBins_recoPt = bins_recoPt[0].size();   // assume <myvector>[0] and <myvector>[1] have the same size.
    nBins_cent = bins_cent[0].size();     // assume <myvector>[0] and <myvector>[1] have the same size.
    nBins_sumIso = bins_sumIso[0].size();     // assume <myvector>[0] and <myvector>[1] have the same size.
    nBins_sieie = bins_sieie[0].size();     // assume <myvector>[0] and <myvector>[1] have the same size.
    nBins_r9 = bins_r9[0].size();     // assume <myvector>[0] and <myvector>[1] have the same size.

    nTriggerAna = nTriggers * nBins_eta * nBins_recoPt * nBins_cent * nBins_sumIso * nBins_sieie * nBins_r9;

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
    std::cout << "mode = " << mode.c_str() << std::endl;
    for (int i = 0; i < (int)runMode.size(); ++i) {
        std::cout << "run " << modesStr[i].c_str() << " = " << runMode.at(i) << std::endl;
    }

    std::cout << "treePath = " << treePath.c_str() << std::endl;
    std::cout << "collision = " << collisionName.c_str() << std::endl;

    // verbose about cut configuration
    std::cout<<"Cut Configuration :"<<std::endl;
    std::cout << "nTriggerBranchesNum  = " << nTriggerBranchesNum << std::endl;
    for (int i = 0; i<nTriggerBranchesNum; ++i) {
        std::cout << Form("triggerBranchesNum[%d] = %s", i, triggerBranchesNum.at(i).c_str()) << std::endl;
    }
    std::cout << "nTriggerBranchesDenomGlobal  = " << nTriggerBranchesDenomGlobal << std::endl;
    for (int i = 0; i<nTriggerBranchesDenomGlobal; ++i) {
        std::cout << Form("triggerBranchesDenomGlobal[%d] = %s", i, triggerBranchesDenomGlobal.at(i).c_str()) << std::endl;
    }
    std::cout << "nTriggerBranchesDenom  = " << nTriggerBranchesDenom << std::endl;
    for (int i = 0; i<nTriggerBranchesDenom; ++i) {
        std::cout << Form("triggerBranchesDenom[%d] = %s", i, triggerBranchesDenom.at(i).c_str()) << std::endl;
    }

    std::cout << "nPrescaleBranchesNum = " << nPrescaleBranchesNum << std::endl;
    for (int i = 0; i < nPrescaleBranchesNum; ++i) {
        std::cout << Form("prescaleBranchesNum[%d] = ", i) << prescaleBranchesNum.at(i).c_str() << std::endl;
    }
    std::cout << "nPrescaleBranchesDenom = " << nPrescaleBranchesDenom << std::endl;
    for (int i = 0; i < nPrescaleBranchesDenom; ++i) {
        std::cout << Form("prescaleBranchesDenom[%d] = ", i) << prescaleBranchesDenom.at(i).c_str() << std::endl;
    }

    std::cout << "nBins_eta = " << nBins_eta << std::endl;
    for (int i=0; i<nBins_eta; ++i) {
        std::cout << Form("bins_eta[%d] = [%f, %f)", i, bins_eta[0].at(i), bins_eta[1].at(i)) << std::endl;
    }
    std::cout << "nBins_recoPt = " << nBins_recoPt << std::endl;
    for (int i=0; i<nBins_recoPt; ++i) {
        std::cout << Form("bins_recoPt[%d] = [%f, %f)", i, bins_recoPt[0].at(i), bins_recoPt[1].at(i)) << std::endl;
    }
    std::cout << "nBins_cent = " << nBins_cent << std::endl;
    for (int i=0; i<nBins_cent; ++i) {
        std::cout << Form("bins_cent[%d] = [%d, %d)", i, bins_cent[0].at(i), bins_cent[1].at(i)) << std::endl;
    }
    std::cout << "nBins_sumIso = " << nBins_sumIso << std::endl;
    for (int i=0; i<nBins_sumIso; ++i) {
        std::cout << Form("bins_sumIso[%d] = [%f, %f)", i, bins_sumIso[0].at(i), bins_sumIso[1].at(i)) << std::endl;
    }
    std::cout << "nBins_sieie = " << nBins_sieie << std::endl;
    for (int i=0; i<nBins_sieie; ++i) {
        std::cout << Form("bins_sieie[%d] = [%f, %f)", i, bins_sieie[0].at(i), bins_sieie[1].at(i)) << std::endl;
    }
    std::cout << "nBins_r9 = " << nBins_r9 << std::endl;
    for (int i=0; i<nBins_r9; ++i) {
        std::cout << Form("bins_r9[%d] = [%f, %f)", i, bins_r9[0].at(i), bins_r9[1].at(i)) << std::endl;
    }

    std::cout<<"doEventWeight = "<< doEventWeight <<std::endl;

    std::cout<<"cut_phoHoverE = "<< cut_phoHoverE <<std::endl;

    std::cout<<"Input Configuration (Cont'd) :"<<std::endl;

    std::cout << "nTH1D_Axis_List = " << nTH1D_Axis_List << std::endl;
    for (int i=0; i<nTH1D_Axis_List; ++i) {
        std::string strTH1D_Axis = ConfigurationParser::verboseTH1D_Axis(TH1D_Axis_List.at(i));
        std::cout << Form("TH1D_Axis_List[%d] = %s", i, strTH1D_Axis.c_str()) << std::endl;
    }

    std::cout << "title = " << title.c_str() << std::endl;
    std::cout << "titleOffsetX = " << titleOffsetX << std::endl;
    std::cout << "titleOffsetY = " << titleOffsetY << std::endl;
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

std::vector<int> parseMode(std::string mode)
{
    std::vector<int> res(MODES::kN_MODES, 0);

    int len = mode.size();
    if (len != MODES::kN_MODES) return res;

    for (int i = 0; i < len; ++i) {

        std::istringstream sin(mode.substr(i, 1));
        int in;
        sin >> in;
        res.at(i) = in;
    }

    return res;
}

/*
 * given the indices for analysis bins, return the vector index
 */
int getVecIndex(std::vector<int> binIndices)
{
    int n = binIndices.size();
    if (n != ANABINS::kN_ANABINS) return -1;

    int nTmp = nTriggerAna / nTriggers;
    int i = binIndices[ANABINS::kTrigger] * nTmp;

    nTmp /= nBins_eta;
    i += binIndices[ANABINS::kEta] * nTmp;

    nTmp /= nBins_recoPt;
    i += binIndices[ANABINS::kRecoPt] * nTmp;

    nTmp /= nBins_cent;
    i += binIndices[ANABINS::kCent] * nTmp;

    nTmp /= nBins_sumIso;
    i += binIndices[ANABINS::kSumIso] * nTmp;

    nTmp /= nBins_sieie;
    i += binIndices[ANABINS::kSieie] * nTmp;

    nTmp /= nBins_r9;
    i += binIndices[ANABINS::kR9] * nTmp;

    return i;
}

/*
 * given the vector index, return the indices for analysis bins
 */
std::vector<int> getBinIndices(int i)
{
    if (i > nTriggerAna)  return {};

    std::vector<int> binIndices;
    binIndices.resize(ANABINS::kN_ANABINS);

    int iTmp = i;
    int nTmp = nTriggerAna;

    // trigger branch index
    nTmp /= nTriggers;
    binIndices[ANABINS::kTrigger] = iTmp / nTmp;

    // eta bin index
    iTmp = i % nTmp;
    nTmp /= nBins_eta;
    binIndices[ANABINS::kEta] = iTmp / nTmp;

    iTmp = i % nTmp;
    nTmp /= nBins_recoPt;
    binIndices[ANABINS::kRecoPt] = iTmp / nTmp;

    iTmp = i % nTmp;
    nTmp /= nBins_cent;
    binIndices[ANABINS::kCent] = iTmp / nTmp;

    iTmp = i % nTmp;
    nTmp /= nBins_sumIso;
    binIndices[ANABINS::kSumIso] = iTmp / nTmp;

    iTmp = i % nTmp;
    nTmp /= nBins_sieie;
    binIndices[ANABINS::kSieie] = iTmp / nTmp;

    iTmp = i % nTmp;
    nTmp /= nBins_r9;
    binIndices[ANABINS::kR9] = iTmp / nTmp;

    return binIndices;
}

/*
 * collect all trigger branches in a single list of unique elements and map their indices to the ones in num / denom lists
 */
void indexTriggerBrances()
{
    triggerBranches.clear();
    triggerBranches.insert(triggerBranches.end(), triggerBranchesNum.begin(), triggerBranchesNum.end());
    triggerBranches.insert(triggerBranches.end(), triggerBranchesDenomGlobal.begin(), triggerBranchesDenomGlobal.end());
    triggerBranches.insert(triggerBranches.end(), triggerBranchesDenom.begin(), triggerBranchesDenom.end());

    triggerBranches = vectorUnique(triggerBranches);

    indicesMapNum.clear();
    indicesMapNum.resize((int)triggerBranchesNum.size());
    for (int i = 0; i < (int)triggerBranchesNum.size(); ++i) {
        indicesMapNum[i] = findPositionInVector(triggerBranches, triggerBranchesNum[i].c_str());
    }

    indicesMapDenomGlobal.clear();
    indicesMapDenomGlobal.resize((int)triggerBranchesDenomGlobal.size());
    for (int i = 0; i < (int)triggerBranchesDenomGlobal.size(); ++i) {
        indicesMapDenomGlobal[i] = findPositionInVector(triggerBranches, triggerBranchesDenomGlobal[i].c_str());
    }

    indicesMapDenom.clear();
    indicesMapDenom.resize((int)triggerBranchesDenom.size());
    for (int i = 0; i < (int)triggerBranchesDenom.size(); ++i) {
        indicesMapDenom[i] = findPositionInVector(triggerBranches, triggerBranchesDenom[i].c_str());
    }

    // do the same for prescale branches
    prescaleBranches.clear();
    prescaleBranches.insert(prescaleBranches.end(), prescaleBranchesNum.begin(), prescaleBranchesNum.end());
    prescaleBranches.insert(prescaleBranches.end(), prescaleBranchesDenom.begin(), prescaleBranchesDenom.end());

    prescaleBranches = vectorUnique(prescaleBranches);

    indicesMapPrescaleNum.clear();
    indicesMapPrescaleNum.resize((int)prescaleBranchesNum.size());
    for (int i = 0; i < (int)prescaleBranchesNum.size(); ++i) {
        indicesMapPrescaleNum[i] = findPositionInVector(prescaleBranches, prescaleBranchesNum[i].c_str());
    }

    indicesMapPrescaleDenom.clear();
    indicesMapPrescaleDenom.resize((int)prescaleBranchesDenom.size());
    for (int i = 0; i < (int)prescaleBranchesDenom.size(); ++i) {
        indicesMapPrescaleDenom[i] = findPositionInVector(prescaleBranches, prescaleBranchesDenom[i].c_str());
    }
}

bool passedNum(int iTriggerNum, int triggerBits[])
{
    if (iTriggerNum < 0) return true;

    int iTrig = indicesMapNum[iTriggerNum];
    return (triggerBits[iTrig] > 0);
}

bool passedDenomGlobal(int triggerBits[])
{
    if (nTriggerBranchesDenomGlobal == 0) return true;

    // these triggers are "OR"ed.
    for (int i = 0; i < nTriggerBranchesDenomGlobal; ++i) {
        int iTrig = indicesMapDenomGlobal[i];
        if (triggerBits[iTrig] > 0)  return true;
    }

    return false;
}

bool passedDenom(int iTriggerDenom, int triggerBits[])
{
    if (iTriggerDenom < 0) return true;

    int iTrig = indicesMapDenom[iTriggerDenom];
    return (triggerBits[iTrig] > 0);
}

bool isPrescaledNum(int iTriggerNum, int triggerPrescales[])
{
    if (nPrescaleBranchesNum == 0) return false;
    if (iTriggerNum < 0) return false;

    int iPrescale = indicesMapPrescaleNum[iTriggerNum];
    return (triggerPrescales[iPrescale] > 1);
}

bool isPrescaledDenom(int iTriggerDenom, int triggerPrescales[])
{
    if (nPrescaleBranchesDenom == 0) return false;
    if (iTriggerDenom < 0) return false;

    int iPrescale = indicesMapPrescaleDenom[iTriggerDenom];
    return (triggerPrescales[iPrescale] > 1);
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

    TH1::SetDefaultSumw2();

    for (int iDep = 0; iDep < TRIGGERANA::kN_DEPS; ++iDep) {

        tAna[iDep].clear();
        tAna[iDep].resize(nTriggerAna);

        for (int iAna = 0; iAna < nTriggerAna; ++iAna) {

        std::vector<int> binIndices = getBinIndices(iAna);

        int iTrigger = binIndices[ANABINS::kTrigger];

        int iEta = binIndices[ANABINS::kEta];
        int iRecoPt = binIndices[ANABINS::kRecoPt];
        int iCent = binIndices[ANABINS::kCent];
        int iSumIso = binIndices[ANABINS::kSumIso];
        int iSieie = binIndices[ANABINS::kSieie];
        int iR9 = binIndices[ANABINS::kR9];

        if (iEta > 0 && iRecoPt > 0 && iCent > 0 && iSumIso > 0 && iSieie > 0)  continue;

        std::string strDep = "";
        std::string xTitle = "";
        bool makeObject = false;
        if (iEta == 0 && iDep == TRIGGERANA::kETA) {
            strDep = "depEta";
            xTitle = "photon #eta";
            makeObject = true;
        }
        else if (iRecoPt == 0 && iDep == TRIGGERANA::kRECOPT) {
            strDep = "depRecoPt";
            xTitle = "Reco p_{T} (GeV/c)";
            makeObject = true;
        }
        else if (iCent == 0 && iDep == TRIGGERANA::kCENT) {
            strDep = "depCent";
            xTitle = "Centrality (%)";
            makeObject = true;
        }
        else if (iSumIso == 0 && iDep == TRIGGERANA::kSUMISO) {
            strDep = "depSumIso";
            xTitle = "sumIso";
            makeObject = true;
        }
        else if (iSieie == 0 && iDep == TRIGGERANA::kSIEIE) {
            strDep = "depSieie";
            xTitle = "#sigma_{#eta#eta}";
            makeObject = true;
        }

        if (!makeObject)  continue;

        triggerAnalyzer tAnaTmp;
        tAnaTmp.recoObj = TRIGGERANA::OBJS::kPHOTON;
        tAnaTmp.dep = iDep;

        int iTriggerNum = (nTriggerBranchesNum == nTriggers) ? iTrigger : 0;
        tAnaTmp.pathNum = triggerBranchesNum[iTriggerNum].c_str();

        if (nTriggerBranchesDenom > 0) {
            int iTriggerDenom = (nTriggerBranchesDenom == nTriggers) ? iTrigger : 0;
            tAnaTmp.pathDenom = triggerBranchesDenom[iTriggerDenom].c_str();
        }

        // histogram ranges
        tAnaTmp.ranges[TRIGGERANA::rETA][0] = bins_eta[0].at(iEta);
        tAnaTmp.ranges[TRIGGERANA::rETA][1] = bins_eta[1].at(iEta);
        tAnaTmp.ranges[TRIGGERANA::rRECOPT][0] = bins_recoPt[0].at(iRecoPt);
        tAnaTmp.ranges[TRIGGERANA::rRECOPT][1] = bins_recoPt[1].at(iRecoPt);
        tAnaTmp.ranges[TRIGGERANA::rCENT][0] = bins_cent[0].at(iCent);
        tAnaTmp.ranges[TRIGGERANA::rCENT][1] = bins_cent[1].at(iCent);
        tAnaTmp.ranges[TRIGGERANA::rSUMISO][0] = bins_sumIso[0].at(iSumIso);
        tAnaTmp.ranges[TRIGGERANA::rSUMISO][1] = bins_sumIso[1].at(iSumIso);
        tAnaTmp.ranges[TRIGGERANA::rSIEIE][0] = bins_sieie[0].at(iSieie);
        tAnaTmp.ranges[TRIGGERANA::rSIEIE][1] = bins_sieie[1].at(iSieie);
        tAnaTmp.ranges[TRIGGERANA::rR9][0] = bins_r9[0].at(iR9);
        tAnaTmp.ranges[TRIGGERANA::rR9][1] = bins_r9[1].at(iR9);

        // for histograms with a particular dependence,
        // a single index is used in the multidimensional array of triggerAnalyzer objects is used.
        // Example : for a trigger efficiency plot with eta dependence (eta is the x-axis), there will not be different histograms
        // with different eta ranges.
        // There will be objects like : tAna[TRIGGERANA::kETA][0][iRecoPt][iCent]
        // but not like : tAna[TRIGGERANA::kETA][1][iRecoPt][iCent]
        // in general there will be no object tAna[someIndex][iEta][iRecoPt][iCent] such that iEta, iRecoPt, iCent > 0

        std::string tmpName = Form("%s_etaBin%d_recoPtBin%d_centBin%d", strDep.c_str(), iEta, iRecoPt, iCent);
        if (nBins_sumIso > 1) {
            tmpName.append(Form("_sumIsoBin%d", iSumIso));
        }
        if (nBins_sieie > 1) {
            tmpName.append(Form("_sieieBin%d", iSieie));
        }
        if (nBins_r9 > 1) {
            tmpName.append(Form("_r9Bin%d", iR9));
        }
        tmpName.append(Form("_trig%d", iTrigger));

        tAnaTmp.name = tmpName.c_str();
        tAnaTmp.title = title.c_str();
        tAnaTmp.titleX = xTitle.c_str();
        tAnaTmp.titleOffsetX = titleOffsetX;
        tAnaTmp.titleOffsetY = titleOffsetY;

        tAnaTmp.textLines = textLines;

        std::string nameNum = tAnaTmp.getObjectName(triggerAnalyzer::OBJ::kNum);
        std::string nameDenom = tAnaTmp.getObjectName(triggerAnalyzer::OBJ::kDenom);

        // disable the cuts/ranges for this dependence
        // Ex. If the dependence is RecoPt (RecoPt is the x-axis),
        // then there will not be RecoPt cuts (eg. RecoPt > 20) for this histogram.
        // The x-axis bins will set the cuts.
        tAnaTmp.ranges[iDep][0] = 0;
        tAnaTmp.ranges[iDep][1] = -1;
        if (iDep == TRIGGERANA::kSUMISO) {
            tAnaTmp.ranges[iDep][0] = -999;
            tAnaTmp.ranges[iDep][1] = -999;
        }

        int iAxis = iDep;
        int nBins = TH1D_Axis_List[iAxis].nBins;  // nBins
        std::vector<double> bins = TH1D_Axis_List[iAxis].bins;

        double arr[nBins+1];
        std::copy(bins.begin(), bins.end(), arr);

        // efficiency
        if (runMode[MODES::kEff]) {
            if (makeNew) {
                tAnaTmp.hNum =
                        new TH1D(nameNum.c_str(), Form(";%s;Entries", xTitle.c_str()), nBins, arr);
                tAnaTmp.hDenom =
                        (TH1D*)tAnaTmp.hNum->Clone(nameDenom.c_str());
            }
            else {
                tAnaTmp.hNum = (TH1D*)input->Get(nameNum.c_str());
                tAnaTmp.hDenom = (TH1D*)input->Get(nameDenom.c_str());
            }
        }

        tAnaTmp.update();
        // prepare title and textline using ranges
        tAnaTmp.prepareTitleRanges();
        tAnaTmp.prepareTextLinesRanges();

        tAna[iDep][iAna] = tAnaTmp;
        }
    }

    indicesTriggerNum.clear();
    indicesTriggerNum.resize(nTriggerAna);

    indicesTriggerDenom.clear();
    indicesTriggerDenom.resize(nTriggerAna);
    for (int iAna = 0; iAna < nTriggerAna; ++iAna) {

        std::vector<int> binIndices = getBinIndices(iAna);

        if (nTriggerBranchesNum > 1)        indicesTriggerNum[iAna] = binIndices[ANABINS::kTrigger];
        else if (nTriggerBranchesNum == 1)  indicesTriggerNum[iAna] = 0;
        else                                indicesTriggerNum[iAna] = -1;

        if (nTriggerBranchesDenom > 1)        indicesTriggerDenom[iAna] = binIndices[ANABINS::kTrigger];
        else if (nTriggerBranchesDenom == 1)  indicesTriggerDenom[iAna] = 0;
        else                                  indicesTriggerDenom[iAna] = -1;
    }

    return 0;
}

int postLoop()
{
    // declaration of graphics objects.
    // the idea is to initialize each Graphics object right before its use, then to delete it right after they are saved/written.
    TCanvas* c = 0;

    for (int iDep = 0; iDep < TRIGGERANA::kN_DEPS; ++iDep) {
        for (int iAna = 0; iAna < nTriggerAna; ++iAna) {

            std::vector<int> binIndices = getBinIndices(iAna);

            //int iTrigger = binIndices[ANABINS::kTriggerNum];

            int iEta = binIndices[ANABINS::kEta];
            int iRecoPt = binIndices[ANABINS::kRecoPt];
            int iCent = binIndices[ANABINS::kCent];
            int iSumIso = binIndices[ANABINS::kSumIso];
            int iSieie = binIndices[ANABINS::kSieie];

            if (iDep == TRIGGERANA::kETA && iEta != 0) continue;
            if (iDep == TRIGGERANA::kRECOPT && iRecoPt != 0) continue;
            if (iDep == TRIGGERANA::kCENT && iCent != 0) continue;
            if (iDep == TRIGGERANA::kSUMISO && iSumIso != 0) continue;
            if (iDep == TRIGGERANA::kSIEIE && iSieie != 0) continue;

            if (iEta > 0 && iRecoPt > 0 && iCent > 0 && iSumIso > 0 && iSieie > 0) continue;

            // for histograms with a particular dependence,
            // a single index is used in the multidimensional array of triggerAnalyzer objects.
            // Example : for an efficiency plot with eta dependence (eta is the x-axis), there will not be different histograms
            // with different eta ranges.
            // There will be objects like : tAna[TRIGGERANA::kETA][0][iRecoPt][iCent]
            // but not like : tAna[TRIGGERANA::kETA][1][iRecoPt][iCent]
            // in general there will be no object tAna[someIndex][iEta][iRecoPt][iCent] such that iEta, iRecoPt, iCent > 0

            c = new TCanvas("cnvTmp", "", windowWidth, windowHeight);
            setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);

            tAna[iDep][iAna].postLoop();
            tAna[iDep][iAna].writeObjects(c);

            c->Close();         // do not use Delete() for TCanvas.
        }
    }


    /*
     * plot 1D trigger efficiency plots on top split into
     *  1. trigger path bins
     *  2. eta bins
     *  3. reco pt bins
     *  4. centrality bins
     *  5. isolation bins
     *  6. shower shape bins
     *  7. R9 bins
     */
    // iObs = 0 is trigger efficiency
    for (int iObs = 0; iObs < TRIGGERANA::OBS::kN_OBS; ++iObs) {
        for (int iDep = 0; iDep < TRIGGERANA::kN_DEPS; ++iDep) {

            for (int iAna = 0; iAna < nTriggerAna; ++iAna) {

                std::vector<int> binIndices = getBinIndices(iAna);

                int iTrigger = binIndices[ANABINS::kTrigger];

                int iEta = binIndices[ANABINS::kEta];
                int iRecoPt = binIndices[ANABINS::kRecoPt];
                int iCent = binIndices[ANABINS::kCent];
                int iSumIso = binIndices[ANABINS::kSumIso];
                int iSieie = binIndices[ANABINS::kSieie];
                int iR9 = binIndices[ANABINS::kR9];

                // plot from different triggers
                if (iTrigger == 0 && tAna[iDep][iAna].name.size() > 0) {
                    drawSame(c, iObs, iDep, {-1, iEta, iRecoPt, iCent, iSumIso, iSieie, iR9});
                }

                // plot from different eta bins
                if (iEta == 0 && tAna[iDep][iAna].name.size() > 0) {
                    drawSame(c, iObs, iDep, {iTrigger, -1, iRecoPt, iCent, iSumIso, iSieie, iR9});
                }

                // plot from different recoPt bins
                if (iRecoPt == 0 && tAna[iDep][iAna].name.size() > 0) {
                    drawSame(c, iObs, iDep, {iTrigger, iEta, -1, iCent, iSumIso, iSieie, iR9});
                }

                // plot from different centrality bins
                if (iCent == 0 && tAna[iDep][iAna].name.size() > 0) {
                    drawSame(c, iObs, iDep, {iTrigger, iEta, iRecoPt, -1, iSumIso, iSieie, iR9});
                }

                // plot from different sumIso bins
                if (iSumIso == 0 && tAna[iDep][iAna].name.size() > 0) {
                    drawSame(c, iObs, iDep, {iTrigger, iEta, iRecoPt, iCent, -1, iSieie, iR9});
                }

                // plot from different sumIso bins
                if (iSieie == 0 && tAna[iDep][iAna].name.size() > 0) {
                    drawSame(c, iObs, iDep, {iTrigger, iEta, iRecoPt, iCent, iSumIso, -1, iR9});
                }

                // plot from different R9 bins
                if (iR9 == 0 && tAna[iDep][iAna].name.size() > 0) {
                    drawSame(c, iObs, iDep, {iTrigger, iEta, iRecoPt, iCent, iSumIso, iSieie, -1});
                }
            }
        }
    }

    return 0;
}

void drawSame(TCanvas* c, int iObs, int iDep, std::vector<int> binIndices)
{
    if (iObs == TRIGGERANA::kEFF && !runMode[MODES::kEff]) return;

    int iTrigger = binIndices[ANABINS::kTrigger];

    int iEta = binIndices[ANABINS::kEta];
    int iRecoPt = binIndices[ANABINS::kRecoPt];
    int iCent = binIndices[ANABINS::kCent];
    int iSumIso = binIndices[ANABINS::kSumIso];
    int iSieie = binIndices[ANABINS::kSieie];
    int iR9 = binIndices[ANABINS::kR9];

    // if the dependency is RecoPt (the x-axis is RecoPt), then it must be iRecoPt = 0
    if (iDep == TRIGGERANA::kETA && iEta != 0) return;
    if (iDep == TRIGGERANA::kRECOPT && iRecoPt != 0) return;
    if (iDep == TRIGGERANA::kCENT && iCent != 0) return;
    if (iDep == TRIGGERANA::kSUMISO && iSumIso != 0) return;
    if (iDep == TRIGGERANA::kSIEIE && iSieie != 0) return;

    TH1D* hTmp = 0;
    TGraphAsymmErrors* gTmp = 0;

    // decide which bins will be plotted on top.
    std::string tmpName = "";
    std::string strBin = "";
    std::string strBin2 = "";
    int nBins = 0;
    if (iTrigger == -1) {
        int iAna = getVecIndex({0, iEta, iRecoPt, iCent, iSumIso, iSieie, iR9});
        tmpName = tAna[iDep][iAna].name.c_str();
        strBin = "trig";
        strBin2 = "trigAll";
        nBins = nTriggers;
    }
    else if (iEta == -1) {
        int iAna = getVecIndex({iTrigger, 0, iRecoPt, iCent, iSumIso, iSieie, iR9});
        tmpName = tAna[iDep][iAna].name.c_str();
        strBin = "etaBin";
        strBin2 = "etaBinAll";
        nBins = nBins_eta;
    }
    else if (iRecoPt == -1) {
        int iAna = getVecIndex({iTrigger, iEta, 0, iCent, iSumIso, iSieie, iR9});
        tmpName = tAna[iDep][iAna].name.c_str();
        strBin = "recoPtBin";
        strBin2 = "recoPtBinAll";
        nBins = nBins_recoPt;
    }
    else if (iCent == -1 && nBins_cent > 1) {
        int iAna = getVecIndex({iTrigger, iEta, iRecoPt, 0, iSumIso, iSieie, iR9});
        tmpName = tAna[iDep][iAna].name.c_str();
        strBin = "centBin";
        strBin2 = "centBinAll";
        nBins = nBins_cent;
    }
    else if (iSumIso == -1 && nBins_sumIso > 1) {
        int iAna = getVecIndex({iTrigger, iEta, iRecoPt, iCent, 0, iSieie, iR9});
        tmpName = tAna[iDep][iAna].name.c_str();
        strBin = "sumIsoBin";
        strBin2 = "sumIsoBinAll";
        nBins = nBins_sumIso;
    }
    else if (iSieie == -1 && nBins_sieie > 1) {
        int iAna = getVecIndex({iTrigger, iEta, iRecoPt, iCent, iSumIso, 0, iR9});
        tmpName = tAna[iDep][iAna].name.c_str();
        strBin = "sieieBin";
        strBin2 = "sieieBinAll";
        nBins = nBins_sieie;
    }
    else if (iR9 == -1 && nBins_r9 > 1) {
        int iAna = getVecIndex({iTrigger, iEta, iRecoPt, iCent, iSumIso, iSieie, 0});
        tmpName = tAna[iDep][iAna].name.c_str();
        strBin = "r9Bin";
        strBin2 = "r9BinAll";
        nBins = nBins_sieie;
    }
    else return;

    std::string canvasName = "cnvTmp";
    c = new TCanvas(canvasName.c_str(), "", windowWidth, windowHeight);

    // replace myBinX with name myBinAll
    size_t indexStart = tmpName.find(strBin.c_str());
    tmpName.replace(indexStart, strBin.size()+1, strBin2.c_str());
    c->SetName(Form("cnv_%s_%s", TRIGGERANA::OBS_LABELS[iObs].c_str(), tmpName.c_str()));

    setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);

    // prepare analysis indices
    std::vector<int> indicesAna(nBins);
    for (int iBin = 0; iBin < nBins; ++iBin) {

        int iAna = -1;
        if (iTrigger == -1) iAna = getVecIndex({iBin, iEta, iRecoPt, iCent, iSumIso, iSieie, iR9});
        else if (iEta == -1) iAna = getVecIndex({iTrigger, iBin, iRecoPt, iCent, iSumIso, iSieie, iR9});
        else if (iRecoPt == -1) iAna = getVecIndex({iTrigger, iEta, iBin, iCent, iSumIso, iSieie, iR9});
        else if (iCent == -1) iAna = getVecIndex({iTrigger, iEta, iRecoPt, iBin, iSumIso, iSieie, iR9});
        else if (iSumIso == -1) iAna = getVecIndex({iTrigger, iEta, iRecoPt, iCent, iBin, iSieie, iR9});
        else if (iSieie == -1) iAna = getVecIndex({iTrigger, iEta, iRecoPt, iCent, iSumIso, iBin, iR9});
        else if (iR9 == -1) iAna = getVecIndex({iTrigger, iEta, iRecoPt, iCent, iSumIso, iSieie, iBin});

        indicesAna[iBin] = iAna;
    }

    std::vector<TH1D*> vecH1D;
    std::vector<TGraph*> vecGraph;
    std::vector<TObject*> vecObj;
    for (int iBin = 0; iBin < nBins; ++iBin) {

        int iHist = iBin;
        if (iTrigger == -1) iHist = iBin;
        else if (iEta == -1) iHist = nTriggers + iBin;
        else if (iRecoPt == -1) iHist = nBins_eta + iBin;
        else if (iCent == -1) iHist = nBins_eta + nBins_recoPt + iBin;
        else if (iSumIso == -1) iHist = nBins_eta + nBins_recoPt + nBins_cent + iBin;
        else if (iSieie == -1) iHist = nBins_eta + nBins_recoPt + nBins_cent + nBins_sumIso + iBin;
        else if (iR9 == -1) iHist = nBins_eta + nBins_recoPt + nBins_cent + nBins_sumIso + nBins_sieie + iBin;

        int iAnaTmp = indicesAna[iBin];

        if (iObs == TRIGGERANA::kEFF) {
            gTmp = (TGraphAsymmErrors*)tAna[iDep][iAnaTmp].gEff->Clone();

            if (iBin == 0) {
                // dummy histogram to be used as template for the graph
                hTmp = (TH1D*)tAna[iDep][iAnaTmp].hEff->Clone();

                hTmp->SetTitle("");
                if (iTrigger != -1) {
                    hTmp->SetTitle(tAna[iDep][iAnaTmp].getPathNumText().c_str());
                }

                hTmp->Reset();
                hTmp->SetMaximum(1.6);
                hTmp->Draw();
            }

            setTGraph(gTmp, iHist);
            vecGraph.push_back(gTmp);
            vecObj.push_back(gTmp);
        }
        else {
            hTmp = (TH1D*)tAna[iDep][iAnaTmp].hNum->Clone();

            hTmp->SetTitle("");
            if (iTrigger != -1) {
                hTmp->SetTitle(tAna[iDep][iAnaTmp].getPathNumText().c_str());
            }

            setTH1(hTmp, iHist);
            vecH1D.push_back(hTmp);
            vecObj.push_back(hTmp);
        }
    }

    if (iObs == TRIGGERANA::kEFF) {
        drawSameTGraph(c, vecGraph);
    }
    else {
        drawSameTH1D(c, vecH1D);
    }
    c->Update();

    TLine* line = new TLine();
    triggerAnalyzer::setPad4Observable((TPad*)c, iObs, iDep);

    // vertical lines for pt thresholds
    for (int iBin = 0; iBin < nBins; ++iBin) {

        int iAnaTmp = indicesAna[iBin];

        if (iObs == TRIGGERANA::kEFF) {

            tAna[iDep][iAnaTmp].drawLine4PtThreshold(c, vecGraph[iBin]->GetMarkerColor());
        }
    }

    TLegend* leg = new TLegend();
    // make legend transparent
    leg->SetFillColor(-1);
    leg->SetFillStyle(4000);

    for (int iBin = 0; iBin < nBins; ++iBin) {

        int iAnaTmp = indicesAna[iBin];

        std::string legendOption = "lpf";
        if (iObs == TRIGGERANA::kEFF)  legendOption = "lp";
        std::string legendText = "";
        if (iTrigger == -1) legendText = tAna[iDep][iAnaTmp].getPathNumText();
        else if (iEta == -1) legendText = tAna[iDep][iAnaTmp].getRangeText(TRIGGERANA::rETA);
        else if (iRecoPt == -1) legendText = tAna[iDep][iAnaTmp].getRangeText(TRIGGERANA::rRECOPT);
        else if (iCent == -1) legendText = tAna[iDep][iAnaTmp].getRangeText(TRIGGERANA::rCENT);
        else if (iSumIso == -1) legendText = tAna[iDep][iAnaTmp].getRangeText(TRIGGERANA::rSUMISO);
        else if (iSieie == -1) legendText = tAna[iDep][iAnaTmp].getRangeText(TRIGGERANA::rSIEIE);
        else if (iR9 == -1) legendText = tAna[iDep][iAnaTmp].getRangeText(TRIGGERANA::rR9);

        if (iDep != TRIGGERANA::kRECOPT)
            legendText.append(Form(", p_{T} > %.0f", tAna[iDep][iAnaTmp].ptTreshold));

        leg->AddEntry(vecObj[iBin], legendText.c_str(), legendOption.c_str());
    }

    setLegend(c, leg, iDep);
    int nLegEntriesTmp = leg->GetNRows();
    if (nLegEntriesTmp > 0)  leg->Draw();

    TLatex* latex = new TLatex();

    std::vector<std::string> textLinesTmp;

    for (int i = 0; i < nTextLines; ++i) {
        textLinesTmp.push_back(textLines.at(i));
    }

    std::vector<int> textRanges = {
            TRIGGERANA::rCENT,
            TRIGGERANA::rRECOPT,
            TRIGGERANA::rETA,
            TRIGGERANA::rSUMISO,
            TRIGGERANA::rSIEIE,
            TRIGGERANA::rR9
    };
    int nTextRanges = textRanges.size();
    for (int i = 0; i < nTextRanges; ++i) {

        int iRange = textRanges[i];

        // if histograms are from different eta bins, then do not write eta range in the text lines
        if (iEta == -1 && iRange == TRIGGERANA::rETA) continue;
        else if (iRecoPt == -1 && iRange == TRIGGERANA::rRECOPT) continue;
        else if (iCent == -1 && iRange == TRIGGERANA::rCENT) continue;

        // if the x-axis is eta, then do not write eta range in the text lines
        if (iDep == TRIGGERANA::kETA && iRange == TRIGGERANA::rETA) continue;
        else if (iDep == TRIGGERANA::kRECOPT && iRange == TRIGGERANA::rRECOPT) continue;
        else if (iDep == TRIGGERANA::kCENT && iRange == TRIGGERANA::rCENT) continue;
        else if (iDep == TRIGGERANA::kSUMISO && iRange == TRIGGERANA::rSUMISO) continue;
        else if (iDep == TRIGGERANA::kSIEIE && iRange == TRIGGERANA::rSIEIE) continue;

        int iAna0 = indicesAna[0];
        std::string textLineTmp = tAna[iDep][iAna0].getRangeText(iRange);
        if (textLineTmp.size() > 0) textLinesTmp.push_back(textLineTmp);
    }

    setLatex(c, latex, iDep, textLinesTmp, leg);

    setCanvasFinal(c);
    c->Write("",TObject::kOverwrite);

    leg->Delete();
    line->Delete();
    latex->Delete();
    c->Close();         // do not use Delete() for TCanvas.
    if (hTmp != 0)  hTmp->Delete();
    if (gTmp != 0)  gTmp->Delete();
}

void setTH1(TH1D* h, int iHist)
{
    h->SetTitle(title.c_str());
    h->SetTitleOffset(titleOffsetX, "X");
    h->SetTitleOffset(titleOffsetY, "Y");

    int nBinsTot = nTriggers + nBins_eta + nBins_recoPt + nBins_cent + nBins_sumIso + nBins_sieie + nBins_r9;

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

void setTGraph(TGraph* g, int iGraph)
{
    TH1D* hTmp = 0;
    hTmp = new TH1D("hTmp_setTGraph", "", 1, 0, 1);
    setTH1(hTmp, iGraph);

    // copy properties from the dummy histogram
    g->GetXaxis()->SetTitleOffset(hTmp->GetTitleOffset("X"));
    g->GetYaxis()->SetTitleOffset(hTmp->GetTitleOffset("Y"));

    g->SetMarkerStyle(hTmp->GetMarkerStyle());
    g->SetMarkerColor(hTmp->GetMarkerColor());
    g->SetLineColor(hTmp->GetLineColor());
    g->SetMarkerSize(hTmp->GetMarkerSize());

    if (hTmp != 0) hTmp->Delete();
}

void setLegend(TPad* pad, TLegend* leg, int iLeg)
{
    float legendTextSize = 0;
    if (nLegendTextSizes == 1) legendTextSize = legendTextSizes.at(0);
    else if (nLegendTextSizes == TRIGGERANA::kN_DEPS) legendTextSize = legendTextSizes.at(iLeg);
    if (legendTextSize != 0)  leg->SetTextSize(legendTextSize);

    int legendBorderSize = 0;
    if (nLegendBorderSizes == 1) legendBorderSize = legendBorderSizes.at(0);
    else if (nLegendBorderSizes == TRIGGERANA::kN_DEPS) legendBorderSize = legendBorderSizes.at(iLeg);
    leg->SetBorderSize(legendBorderSize);

    float legendHeight = 0;
    if (nLegendHeights == 1) legendHeight = legendHeights.at(0);
    else if (nLegendHeights == TRIGGERANA::kN_DEPS) legendHeight = legendHeights.at(iLeg);

    float legendWidth = 0;
    if (nLegendWidths == 1) legendWidth = legendWidths.at(0);
    else if (nLegendWidths == TRIGGERANA::kN_DEPS) legendWidth = legendWidths.at(iLeg);

    double height = calcTLegendHeight(leg);
    double width = calcTLegendWidth(leg);
    if (legendHeight != 0)  height = legendHeight;
    if (legendWidth != 0)  width = legendWidth;

    std::string legendPosition = legendPositions.at(0).c_str();
    if (nLegendPositions == TRIGGERANA::kN_DEPS)  legendPosition = legendPositions.at(iLeg).c_str();

    float legendOffsetX = 0;
    if (nLegendOffsetsX == 1) legendOffsetX = legendOffsetsX.at(0);
    else if (nLegendOffsetsX == TRIGGERANA::kN_DEPS) legendOffsetX = legendOffsetsX.at(iLeg);

    float legendOffsetY = 0;
    if (nLegendOffsetsY == 1) legendOffsetY = legendOffsetsY.at(0);
    else if (nLegendOffsetsY == TRIGGERANA::kN_DEPS) legendOffsetY = legendOffsetsY.at(iLeg);

    setLegendPosition(leg, legendPosition, pad, height, width, legendOffsetX, legendOffsetY, true);
}

void setLatex(TPad* pad, TLatex* latex, int iLatex, std::vector<std::string> textLines, TLegend* leg)
{
    std::string textPosition = "";
    if (nTextPositions == 1)  textPosition = textPositions.at(0);
    else if (nTextPositions == TRIGGERANA::kN_DEPS)  textPosition = textPositions.at(iLatex);
    setTextAlignment(latex, textPosition);

    float textFont = textFonts.at(0);
    if (nTextFonts == TRIGGERANA::kN_DEPS) textFont = textFonts.at(iLatex);
    latex->SetTextFont(textFont);

    float textSize = textSizes.at(0);
    if (nTextSizes == TRIGGERANA::kN_DEPS) textSize = textSizes.at(iLatex);
    latex->SetTextSize(textSize);

    float textOffsetX = textOffsetsX.at(0);
    if (nTextOffsetsX == TRIGGERANA::kN_DEPS) textOffsetX = textOffsetsX.at(iLatex);

    float textOffsetY = textOffsetsY.at(0);
    if (nTextOffsetsY == TRIGGERANA::kN_DEPS) textOffsetY = textOffsetsY.at(iLatex);

    // update y offset if legend and latex will overlap
    if (leg != 0) {
        std::string legendPosition = legendPositions.at(0).c_str();
        if (nLegendPositions == TRIGGERANA::kN_DEPS)  legendPosition = legendPositions.at(iLatex).c_str();

        if (textPosition.find("N") == 0 && textPosition == legendPosition) {
            textOffsetY += leg->GetY2NDC() - leg->GetY1NDC();
        }
    }

    drawTextLines(latex, pad, textLines, textPosition, textOffsetX, textOffsetY);
}
