/*
 * macro to make performance plots for generic object reconstruction.
 * Plots for photons are eta, gen Pt, reco Pt, centrality, isolation, and shower shape dependent.
 * Plots for electrons are eta, gen Pt, reco Pt, centrality, and shower shape dependent.
 * The macro can make at most 6 types of plots
 *  1. x-axis is eta.
 *  2. x-axis is gen Pt.
 *  3. x-axis is reco Pt.
 *  4. x-axis is centrality (hiBin/2)
 *  5. x-axis is isolation (sumIso)
 *  6. x-axis is shower shape (sigmaIEtaIEta_2012)
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
#include <TMath.h>
#include <TLorentzVector.h>

#include "TMVA/IMethod.h"
#include "TMVA/MethodBase.h"
#include "TMVA/MethodCuts.h"
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>

#include "../../CorrelationTuple/EventMatcher.h"
#include "../../TreeHeaders/CutConfigurationTree.h"
#include "../../TreeHeaders/ggHiNtuplizerTree.h"
#include "../../TreeHeaders/hiEvtTree.h"
#include "../../TreeHeaders/hiGenParticleTree.h"
#include "../../TreeHeaders/skimAnalysisTree.h"
#include "../../Utilities/interface/ArgumentParser.h"
#include "../../Utilities/interface/ConfigurationParser.h"
#include "../../Utilities/interface/GraphicsConfigurationParser.h"
#include "../../Utilities/interface/InputConfigurationParser.h"
#include "../../Utilities/interface/HiForestInfoController.h"
#include "../../Utilities/eventUtil.h"
#include "../../Utilities/styleUtil.h"
#include "../../Utilities/th1Util.h"
#include "../../Utilities/fileUtil.h"
#include "../../Utilities/physicsUtil.h"
#include "../../Utilities/systemUtil.h"
#include "../interface/recoAnalyzer.h"

///// global variables
/// configuration variables
/*
* mode is a string of characters.
* Each character in "mode" is a flag.
* Characters are modes for EnergyScale, Correction, MatchEff, FakeRate, FakeRateComposition
* If "0", then do not run the corresponding mode
*/
std::string mode;

// input for TTree
std::string treePath;
int collisionType;

// input for TMVA regression
std::vector<std::string> tmvaXMLFiles;
std::vector<std::string> tmvaMethodNames;
std::vector<std::pair<std::string, int>> tmvaReaderVars;
std::vector<std::pair<std::string, int>> tmvaReaderSpectators;
std::vector<float> tmva_bins_eta[2];  // eta range for which a TMVA file will be used
std::vector<float> tmva_bins_pt[2];   // pT range for which a TMVA file will be used

// input for TH1
// nBins, xLow, xUp for the TH1D histogram
// this bin list will be used for histograms where x-axis is eta.
std::vector<CONFIGPARSER::TH1Axis> TH1D_Axis_List;
// nBinsx, xLow, xUp, nBinsy, yLow, yUp for a TH2D histogram
// this bin list will be used for gen pt - reco pt correlation histogram.
std::vector<CONFIGPARSER::TH2DAxis> TH2D_Axis_List;

std::string title;
float titleOffsetX;
float titleOffsetY;

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

int nTmvaXMLFiles;
int nTmvaMethods;
int nTmva_bins_eta;
int nTmva_bins_pt;

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

std::vector<float> bins_eta[2];         // array of vectors for eta bins, each array element is a vector.
// list of pt cuts for GEN-level object matched to RECO object
std::vector<float> bins_genPt[2];       // array of vectors for genPt bins, each array element is a vector, should function also as
// list of pt cuts for RECO object
std::vector<float> bins_recoPt[2];      // array of vectors for recoPt bins, each array element is a vector, should function also as
std::vector<int>   bins_cent[2];       // array of vectors for centrality bins, each array element is a vector.
// list of other cuts
std::vector<float>   bins_sumIso[2];
std::vector<float>   bins_sieie[2];
std::vector<float>   bins_r9[2];

// event cuts/weights
int doEventWeight;
std::vector<std::vector<float>> pthatWeights;

// RECO object cuts
float cut_hovere;
bool excludeHI18HEMfailure;

// GEN object cuts
float cut_mcCalIsoDR04;
float cut_mcTrkIsoDR04;
float cut_mcSumIso;

int nPthatWeights;

int nBins_eta;
int nBins_genPt;
int nBins_recoPt;
int nBins_cent;
int nBins_sumIso;
int nBins_sieie;
int nBins_r9;
/// configuration variables - END
enum RECOOBJS {
    kPhoton,
    kElectron,
    kN_RECOOBJS
};
const std::string recoObjsStr[kN_RECOOBJS] = {"photon", "electron"};
int recoObj;

enum MODES {
    kEnergyScale,
    kCorrection,
    kMatchEff,
    kFakeRate,
    kFakeComposition,
    kN_MODES
};
const std::string modesStr[kN_MODES] = {"EnergyScale", "Correction", "MatchEff", "FakeRate", "FakeComposition"};
std::vector<int> runMode;

enum MODES_ESCALE {
    kNULL_ESCALE,
    kRecoPtGenPt,
    kSCRawEGenE,
    kRecoPtGenPtmeson0,
    kSCRawEGenEmeson0,
    kRecoPtGenPtele,
    kSCRawEGenEele,
    kN_MODES_ESCALE
};
const std::string modesEScaleStr[kN_MODES_ESCALE] = {"NULL_ESCALE", "RecoPtGenPt", "SCRawEGenE",
                                                                    "RecoPtGenPtmeson0", "SCRawEGenEmeson0",
                                                                    "RecoPtGenPtele", "SCRawEGenEele"};

enum MODES_MATCHEFF {
    kNULL_MATCHEFF,
    kMatchDefault,
    kMatchMeson0,
    kMatchEle,
    kN_MODES_MATCHEFF
};
const std::string modesMatchEffStr[kN_MODES_MATCHEFF] = {"NULL_MATCHEFF", "MatchDefault", "MatchMeson0", "MatchEle"};

enum ANABINS {
    kEta,
    kGenPt,
    kRecoPt,
    kCent,
    kSumIso,
    kSieie,
    kR9,
    kN_ANABINS
};
int nRecoAna;
// object for set of all reco analysis
std::vector<recoAnalyzer> rAna[RECOANA::kN_DEPS];
// Each vector will have size nRecoAna
///// global variables - END

int readConfiguration(std::string configFile, std::string inputFile);
void printConfiguration();
int parseRecoObj(std::string recoObjStr);
std::vector<int> parseMode(std::string mode);
int getVecIndex(std::vector<int> binIndices);
std::vector<int> getBinIndices(int i);
int  preLoop(TFile* input = 0, bool makeNew = true);
int  postLoop();
void drawSame(TCanvas* c, int iObs, int iDep, std::vector<int> binIndices);
void setTH1(TH1D* h, int iHist);
void setTGraph(TGraph* g, int iGraph);
void setLegend(TPad* pad, TLegend* leg, int iLeg);
void setLatex(TPad* pad, TLatex* latex, int iLatex, std::vector<std::string> textLines, TLegend* leg);
bool isNeutralMeson(int pdg);
void copy2TmvaVars(ggHiNtuplizer& ggHi, int i, float *vals, std::vector<std::string>& tmvaVarNames, int nVars, int offset);
int findGenMatchedIndex(ggHiNtuplizer& ggHi, double recoEta, double recoPhi, double deltaR2, int genMatchedPID);
void objRecoAna(std::string configFile, std::string inputFile, std::string outputFile = "objRecoAna.root");
void objRecoAnaNoLoop(std::string configFile, std::string inputFile, std::string outputFile = "objRecoAna.root");

void objRecoAna(std::string configFile, std::string inputFile, std::string outputFile)
{
    std::cout<<"running objRecoAna()"<<std::endl;
    std::cout<<"configFile  = "<< configFile.c_str() <<std::endl;
    std::cout<<"inputFile   = "<< inputFile.c_str()  <<std::endl;
    std::cout<<"outputFile  = "<< outputFile.c_str() <<std::endl;

    if (readConfiguration(configFile, inputFile) != 0)  return;
    printConfiguration();

    std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.c_str());
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
    TTree* treeHiGenParticle = 0;
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
    bool isHI15 = collisionIsHI((COLL::TYPE)collisionType);
    bool isHI18 = collisionIsHI2018((COLL::TYPE)collisionType);
    bool isHI = (isHI15 || isHI18);
    bool isPP = collisionIsPP((COLL::TYPE)collisionType);

    if (!isMC) {
        std::cout << "This macro runs on simulation samples only." << std::endl;
        std::cout << "Change the collisionType to a simulated collisions." << std::endl;
        std::cout << "exiting" << std::endl;
        return;
    }

    bool doTMVA = (nTmvaXMLFiles != 0 && nTmvaMethods);

    std::vector<TMVA::Reader*> tmvaReaders(nTmvaXMLFiles, 0);
    int nTmvaReaderVars = tmvaReaderVars.size();
    int nTmvaReaderSpectators = tmvaReaderSpectators.size();
    float varsR[nTmvaReaderVars];
    float specsR[nTmvaReaderSpectators];
    std::vector<std::string> tmvaReaderVarsStr[nTmvaXMLFiles];
    std::vector<int> nReaderVarsInFile(nTmvaXMLFiles, 0);
    std::vector<std::string> tmvaReaderSpecsStr[nTmvaXMLFiles];
    if (doTMVA) {

        std::vector<std::string> tmvaReaderVarsStrAll = ConfigurationParser::getVecString(tmvaReaderVars);
        std::vector<int> tmvaReaderVarIndices = ConfigurationParser::getVecIndex(tmvaReaderVars);

        std::vector<std::string> tmvaReaderSpecsStrAll = ConfigurationParser::getVecString(tmvaReaderSpectators);
        std::vector<int> tmvaReaderSpecsIndices = ConfigurationParser::getVecIndex(tmvaReaderSpectators);

        for (int iXML = 0; iXML < nTmvaXMLFiles; ++iXML) {

            tmvaReaders[iXML] = new TMVA::Reader("!Color");

            int nVarsTmp = std::count(tmvaReaderVarIndices.begin(), tmvaReaderVarIndices.end(), iXML);
            nReaderVarsInFile[iXML] = nVarsTmp;

            int iStart = findInVector(tmvaReaderVarIndices, iXML);
            for (int i = iStart; i < iStart+nVarsTmp; ++i) {

                tmvaReaders[iXML]->AddVariable(tmvaReaderVarsStrAll[i].c_str(), &(varsR[i]));
            }
            tmvaReaderVarsStr[iXML].assign(tmvaReaderVarsStrAll.begin()+iStart, tmvaReaderVarsStrAll.begin()+iStart+nVarsTmp);

            int nSpecsTmp = std::count(tmvaReaderSpecsIndices.begin(), tmvaReaderSpecsIndices.end(), iXML);

            iStart = findInVector(tmvaReaderSpecsIndices, iXML);
            for (int i = iStart; i < iStart+nSpecsTmp; ++i) {
                tmvaReaders[iXML]->AddSpectator(tmvaReaderSpecsStrAll[i].c_str(), &(specsR[i]));
            }
            tmvaReaderSpecsStr[iXML].assign(tmvaReaderSpecsStrAll.begin()+iStart, tmvaReaderSpecsStrAll.begin()+iStart+nSpecsTmp);

            tmvaReaders[iXML]->BookMVA(tmvaMethodNames[iXML].c_str(), tmvaXMLFiles[iXML].c_str());
        }
    }

    EventMatcher* em = new EventMatcher();
    Long64_t duplicateEntries = 0;

    Long64_t entries = 0;
    Long64_t entriesAnalyzed = 0;

    int nFilesSkipped = 0;
    std::cout<< "Loop : " << treePath.c_str() <<std::endl;
    for (int iFile = 0; iFile < nFiles; ++iFile)  {

        std::string inputPath = inputFiles.at(iFile).c_str();
        std::cout <<"iFile = " << iFile << " , " ;
        std::cout <<"reading input file : " << inputPath.c_str() << std::endl;
        fileTmp = TFile::Open(inputPath.c_str(), "READ");

        // check if the file is usable, if not skip the file.
        if (isGoodFile(fileTmp) != 0) {
            std::cout << "File is not good. skipping file." << std::endl;
            nFilesSkipped++;
            continue;
        }

        treeggHiNtuplizer = (TTree*)fileTmp->Get(treePath.c_str());
        treeggHiNtuplizer->SetBranchStatus("*",0);     // disable all branches
        if (recoObj == RECOOBJS::kPhoton) {
            treeggHiNtuplizer->SetBranchStatus("nPho",1);     // enable photon branches
            treeggHiNtuplizer->SetBranchStatus("pho*",1);     // enable photon branches
        }
        else if (recoObj == RECOOBJS::kElectron) {
            treeggHiNtuplizer->SetBranchStatus("nEle",1);
            treeggHiNtuplizer->SetBranchStatus("ele*",1);
        }
        treeggHiNtuplizer->SetBranchStatus("nMC*",1);     // enable GEN particle branches
        treeggHiNtuplizer->SetBranchStatus("mc*",1);      // enable GEN particle branches
        treeggHiNtuplizer->SetBranchStatus("rho",1);
        // check existence of genMatching branch
        if (!treeggHiNtuplizer->GetBranch("pho_genMatchedIndex")) {
            std::cout << "WARNING : Branch pho_genMatchedIndex does not exist." <<std::endl;
        }

        // specify explicitly which branches to use, do not use wildcard
        treeHiEvt = (TTree*)fileTmp->Get("hiEvtAnalyzer/HiTree");
        treeHiEvt->SetBranchStatus("*",0);     // disable all branches
        treeHiEvt->SetBranchStatus("run",1);   // enable event information
        treeHiEvt->SetBranchStatus("evt",1);
        treeHiEvt->SetBranchStatus("lumi",1);
        treeHiEvt->SetBranchStatus("vz",1);
        treeHiEvt->SetBranchStatus("hiBin",1);
        if (doEventWeight > 0) {
            treeHiEvt->SetBranchStatus("weight", 1);
        }
        if (nPthatWeights > 0) {
            treeHiEvt->SetBranchStatus("pthat",1);
        }

        // specify explicitly which branches to use, do not use wildcard
        treeHiGenParticle = (TTree*)fileTmp->Get("HiGenParticleAna/hi");
        treeHiGenParticle->SetBranchStatus("*",0);     // disable all branches

        // read gen particle tree only if the relevant modes are run
        bool readHiGenParticle = (runMode[MODES::kFakeComposition]);
        if (readHiGenParticle) {
            treeHiGenParticle->SetBranchStatus("*",1);     // enable all branches
        }

        treeSkim = (TTree*)fileTmp->Get("skimanalysis/HltTree");
        treeSkim->SetBranchStatus("*",0);     // disable all branches

        ggHiNtuplizer ggHi;
        ggHi.setupTreeForReading(treeggHiNtuplizer);

        hiEvt hiEvt;
        hiEvt.setupTreeForReading(treeHiEvt);

        hiGenParticle hiGen;
        hiGen.setupTreeForReading(treeHiGenParticle);

        skimAnalysis skimAna;
        if (isHI15) skimAna.enableBranchesHI(treeSkim);
        else if (isHI18) skimAna.enableBranchesHI2018(treeSkim);
        else if (isPP) skimAna.enableBranchesPP(treeSkim);
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
            if (readHiGenParticle) {
                treeHiGenParticle->GetEntry(j_entry);
            }
            treeSkim->GetEntry(j_entry);

            bool eventAdded = em->addEvent(hiEvt.run, hiEvt.lumi, hiEvt.evt, j_entry);
            if(!eventAdded) // this event is duplicate, skip this one.
            {
                duplicateEntries++;
                continue;
            }

            // event selection
            if (!(TMath::Abs(hiEvt.vz) < 15))  continue;

            if (isHI15 && !skimAna.passedEventSelectionHI())  continue;
            else if (isHI18 && !skimAna.passedEventSelectionHI2018())  continue;
            else if (isPP && !skimAna.passedEventSelectionPP())  continue;

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

            if (nPthatWeights > 0) {
                double pthatWeight = 0;
                for (int i = 0; i < nPthatWeights; ++i) {
                    if (hiEvt.pthat >= pthatWeights[0][i] && hiEvt.pthat < pthatWeights[1][i]) {
                        pthatWeight = pthatWeights[2][i];
                        break;
                    }
                }
                w *= pthatWeight;
            }

            if (recoObj == RECOOBJS::kPhoton) {
                // energy scale
                if (runMode[MODES::kEnergyScale]) {
                    for (int i=0; i<ggHi.nPho; ++i) {

                        // selections on GEN particle
                        int genMatchedIndex = (*ggHi.pho_genMatchedIndex)[i];
                        if (genMatchedIndex < 0)   continue;    // is matched

                        int genMatchedPID = 22;
                        if (runMode[MODES::kEnergyScale] == kRecoPtGenPtele || runMode[MODES::kEnergyScale] == kSCRawEGenEele) {
                            genMatchedPID = 11;
                        }
                        if (TMath::Abs((*ggHi.mcPID)[genMatchedIndex]) != genMatchedPID)  continue;

                        double genPt = (*ggHi.mcPt)[genMatchedIndex];
                        double genE = (*ggHi.mcE)[genMatchedIndex];
                        if (runMode[MODES::kEnergyScale] == kRecoPtGenPtmeson0 || runMode[MODES::kEnergyScale] == kSCRawEGenEmeson0) {
                            if (!isNeutralMeson((*ggHi.mcMomPID)[genMatchedIndex]))  continue;

                            genPt = (*ggHi.mcMomPt)[genMatchedIndex];
                            TLorentzVector vec;
                            vec.SetPtEtaPhiM(genPt, (*ggHi.mcMomEta)[genMatchedIndex], (*ggHi.mcMomPhi)[genMatchedIndex], (*ggHi.mcMomMass)[genMatchedIndex]);
                            genE = vec.E();
                        }
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
                        if (!ggHi.passedPhoSpikeRejection(i)) continue;

                        if (cut_hovere != 0) {
                            if (!((*ggHi.phoHoverE)[i] < cut_hovere))   continue;
                        }

                        if (excludeHI18HEMfailure && !ggHi.passedHI18HEMfailurePho(i))  continue;

                        double eta = (*ggHi.phoEta)[i];
                        double pt  = (*ggHi.phoEt)[i];
                        double sumIso = ((*ggHi.pho_ecalClusterIsoR4)[i] +
                                (*ggHi.pho_hcalRechitIsoR4)[i]  +
                                (*ggHi.pho_trackIsoR4PtCut20)[i]);
                        double sieie = (*ggHi.phoSigmaIEtaIEta_2012)[i];
                        double r9 = (*ggHi.phoR9)[i];
                        double energyScale = -1;

                        if (doTMVA) {

                            int offset = 0;
                            for (int iXML = 0; iXML < nTmvaXMLFiles; ++iXML) {

                                double scEta = (*ggHi.phoSCEta)[i];
                                bool insideEtaRange = (
                                        (tmva_bins_eta[0][iXML] < tmva_bins_eta[1][iXML] && tmva_bins_eta[0][iXML] <= std::fabs(scEta) && std::fabs(scEta) < tmva_bins_eta[1][iXML]) ||
                                        (tmva_bins_eta[1][iXML] < 0 && tmva_bins_eta[0][iXML] <= std::fabs(scEta)));
                                bool insidePtRange = (
                                        (tmva_bins_pt[0][iXML] < tmva_bins_pt[1][iXML] && tmva_bins_pt[0][iXML] <= pt && pt < tmva_bins_pt[1][iXML]) ||
                                        (tmva_bins_pt[1][iXML] < 0 && tmva_bins_pt[0][iXML] <= pt));

                                if (insideEtaRange && insidePtRange) {
                                    copy2TmvaVars(ggHi, i, varsR, tmvaReaderVarsStr[iXML], nReaderVarsInFile[iXML], offset);
                                    std::vector<float> targets_regr = tmvaReaders[iXML]->EvaluateRegression(tmvaMethodNames[iXML].c_str());
                                    double energy = targets_regr[0];
                                    pt = energy / TMath::CosH(eta);
                                    break;
                                }

                                offset += nReaderVarsInFile[iXML];
                            }
                        }

                        if (runMode[MODES::kEnergyScale] == kRecoPtGenPt ||
                                runMode[MODES::kEnergyScale] == kRecoPtGenPtmeson0 ||
                                runMode[MODES::kEnergyScale] == kRecoPtGenPtele) {
                            energyScale = pt/genPt;
                        }
                        else if (runMode[MODES::kEnergyScale] == kSCRawEGenE ||
                                runMode[MODES::kEnergyScale] == kSCRawEGenEmeson0 ||
                                runMode[MODES::kEnergyScale] == kSCRawEGenEele) {
                            energyScale = (*ggHi.phoSCRawE)[i]/genE;
                        }

                        std::vector<double> vars = {eta, genPt, pt, (double)cent, sumIso, sieie, r9};
                        for (int iAna = 0;  iAna < nRecoAna; ++iAna) {

                            rAna[RECOANA::kETA][iAna].FillH2D(energyScale, eta, w, vars);
                            rAna[RECOANA::kGENPT][iAna].FillH2D(energyScale, genPt, w, vars);
                            rAna[RECOANA::kRECOPT][iAna].FillH2D(energyScale, pt, w, vars);
                            rAna[RECOANA::kCENT][iAna].FillH2D(energyScale, cent, w, vars);
                            rAna[RECOANA::kSUMISO][iAna].FillH2D(energyScale, sumIso, w, vars);
                            rAna[RECOANA::kSIEIE][iAna].FillH2D(energyScale, sieie, w, vars);

                            rAna[RECOANA::kGENPT][iAna].FillH2Dcc(genPt, pt, w, vars);
                        }
                    }
                }

                // correction
                if (runMode[MODES::kCorrection]) {
                    for (int i=0; i<ggHi.nPho; ++i) {

                        // selections on RECO particle
                        if (!ggHi.passedPhoSpikeRejection(i)) continue;

                        if (cut_hovere != 0) {
                            if (!((*ggHi.phoHoverE)[i] < cut_hovere))   continue;
                        }

                        if (excludeHI18HEMfailure && !ggHi.passedHI18HEMfailurePho(i))  continue;

                        double pt  = (*ggHi.phoEt)[i];
                        double eta = (*ggHi.phoEta)[i];
                        double sumIso = ((*ggHi.pho_ecalClusterIsoR4)[i] +
                                (*ggHi.pho_hcalRechitIsoR4)[i]  +
                                (*ggHi.pho_trackIsoR4PtCut20)[i]);
                        double sieie = (*ggHi.phoSigmaIEtaIEta_2012)[i];
                        double r9 = (*ggHi.phoR9)[i];
                        double phoE = (*ggHi.phoE)[i];
                        double phoSCE = (*ggHi.phoSCE)[i];
                        double phoSCRawE = (*ggHi.phoSCRawE)[i];

                        double corrE = phoE / phoSCE;
                        double corrSCE = phoSCE / phoSCRawE;

                        std::vector<double> vars = {eta, -1, pt, (double)cent, sumIso, sieie, r9};
                        for (int iAna = 0;  iAna < nRecoAna; ++iAna) {

                            rAna[RECOANA::kETA][iAna].FillH2DCorr(corrE, eta, RECOANA::k_corrE, w, vars);
                            //rAna[RECOANA::kGENPT][iAna].varsFillH2DCorr(corrE, genPt, RECOANA::k_corrE, w, vars);
                            rAna[RECOANA::kRECOPT][iAna].FillH2DCorr(corrE, pt, RECOANA::k_corrE, w, vars);
                            rAna[RECOANA::kCENT][iAna].FillH2DCorr(corrE, cent, RECOANA::k_corrE, w, vars);
                            rAna[RECOANA::kSUMISO][iAna].FillH2DCorr(corrE, sumIso, RECOANA::k_corrE, w, vars);
                            rAna[RECOANA::kSIEIE][iAna].FillH2DCorr(corrE, sieie, RECOANA::k_corrE, w, vars);

                            rAna[RECOANA::kETA][iAna].FillH2DCorr(corrSCE, eta, RECOANA::k_corrSCE, w, vars);
                            //rAna[RECOANA::kGENPT][iAna].varsFillH2DCorr(corrSCE, genPt, RECOANA::k_corrSCE, w, vars);
                            rAna[RECOANA::kRECOPT][iAna].FillH2DCorr(corrSCE, pt, RECOANA::k_corrSCE, w, vars);
                            rAna[RECOANA::kCENT][iAna].FillH2DCorr(corrSCE, cent, RECOANA::k_corrSCE, w, vars);
                            rAna[RECOANA::kSUMISO][iAna].FillH2DCorr(corrSCE, sumIso, RECOANA::k_corrSCE, w, vars);
                            rAna[RECOANA::kSIEIE][iAna].FillH2DCorr(corrSCE, sieie, RECOANA::k_corrSCE, w, vars);
                        }
                    }
                }

                // matching efficiency
                if (runMode[MODES::kMatchEff]) {
                    for (int i=0; i<ggHi.nMC; ++i) {

                        if ((*ggHi.mcStatus)[i] != 1)  continue;

                        int genMatchedPID = 22;
                        if (runMode[MODES::kMatchEff] == kMatchEle) {
                            genMatchedPID = 11;
                        }
                        if (TMath::Abs((*ggHi.mcPID)[i]) != genMatchedPID)  continue;

                        double genPt = (*ggHi.mcPt)[i];
                        double genEta = (*ggHi.mcEta)[i];
                        double genPhi = (*ggHi.mcPhi)[i];

                        if (runMode[MODES::kMatchEff] == kMatchMeson0)  {
                            if (!isNeutralMeson((*ggHi.mcMomPID)[i]))  continue;

                            genPt = (*ggHi.mcMomPt)[i];
                            genEta = (*ggHi.mcMomEta)[i];
                            genPhi = (*ggHi.mcMomPhi)[i];
                        }

                        if (isHI) {
                            if (cut_mcCalIsoDR04 != 0) {
                                if (!((*ggHi.mcCalIsoDR04)[i] < cut_mcCalIsoDR04))   continue;
                            }
                            if (cut_mcTrkIsoDR04 != 0) {
                                if (!((*ggHi.mcTrkIsoDR04)[i] < cut_mcTrkIsoDR04))   continue;
                            }
                            if (cut_mcSumIso != 0) {
                                if (!(((*ggHi.mcCalIsoDR04)[i] +
                                        (*ggHi.mcTrkIsoDR04)[i]) < cut_mcSumIso))   continue;
                            }
                        }
                        else {
                            if (cut_mcCalIsoDR04 != 0) {
                                if (!((*ggHi.mcCalIsoDR04)[i] < cut_mcCalIsoDR04))   continue;
                            }
                            if (cut_mcTrkIsoDR04 != 0) {
                                if (!((*ggHi.mcTrkIsoDR04)[i] < cut_mcTrkIsoDR04))   continue;
                            }
                            if (cut_mcSumIso != 0) {
                                if (!(((*ggHi.mcCalIsoDR04)[i] +
                                        (*ggHi.mcTrkIsoDR04)[i]) < cut_mcSumIso))   continue;
                            }
                        }

                        if (excludeHI18HEMfailure && !ggHi.passedHI18HEMfailureGen(i))  continue;

                        // look for matching RECO particle
                        double deltaR2 = 0.15*0.15;
                        int iReco = -1;
                        double recoPt = -1;
                        for (int j = 0; j < ggHi.nPho; ++j) {

                            if (!ggHi.passedPhoSpikeRejection(j)) continue;

                            if (cut_hovere != 0) {
                                if (!((*ggHi.phoHoverE)[j] < cut_hovere))   continue;
                            }

                            if (excludeHI18HEMfailure && !ggHi.passedHI18HEMfailurePho(i))  continue;

                            if (getDR2((*ggHi.phoEta)[j], (*ggHi.phoPhi)[j], genEta, genPhi) < deltaR2 && (*ggHi.phoEt)[j] > recoPt ) {
                                iReco = j;
                                recoPt = (*ggHi.phoEt)[j];
                            }
                        }
                        bool matched2RECO = (iReco > -1);

                        std::vector<double> varsDenom = {genEta, genPt, -1, (double)cent, -999, -1, -1};
                        for (int iAna = 0;  iAna < nRecoAna; ++iAna) {

                            rAna[RECOANA::kETA][iAna].FillHDenom(genEta, w, varsDenom);
                            rAna[RECOANA::kGENPT][iAna].FillHDenom(genPt, w, varsDenom);
                            //rAna[RECOANA::kRECOPT][iAna].FillHDenom(pt, w, varsDenom);
                            rAna[RECOANA::kCENT][iAna].FillHDenom(cent, w, varsDenom);
                            //rAna[RECOANA::kSUMISO][iAna].FillHDenom(sumIso, w, varsDenom);
                            //rAna[RECOANA::kSIEIE][iAna].FillHDenom(sieie, w, varsDenom);

                            if (matched2RECO) {
                                double pt  = (*ggHi.phoEt)[iReco];
                                double sumIso = ((*ggHi.pho_ecalClusterIsoR4)[iReco] +
                                        (*ggHi.pho_hcalRechitIsoR4)[iReco]  +
                                        (*ggHi.pho_trackIsoR4PtCut20)[iReco]);
                                double sieie = (*ggHi.phoSigmaIEtaIEta_2012)[iReco];
                                double r9 = (*ggHi.phoR9)[iReco];
                                std::vector<double> varsNum = {genEta, genPt, pt, (double)cent, sumIso, sieie, r9};

                                rAna[RECOANA::kETA][iAna].FillHNum(genEta, w, varsNum);
                                rAna[RECOANA::kGENPT][iAna].FillHNum(genPt, w, varsNum);
                                //rAna[RECOANA::kRECOPT][iAna].FillHNum(pt, w, varsNum);
                                rAna[RECOANA::kCENT][iAna].FillHNum(cent, w, varsNum);
                                //rAna[RECOANA::kSUMISO][iAna].FillHNum(sumIso, w, varsNum);
                                //rAna[RECOANA::kSIEIE][iAna].FillHNum(sieie, w, varsNum);
                            }

                        }
                    }
                }

                // fake rate
                if (runMode[MODES::kFakeRate]) {
                    for (int i=0; i<ggHi.nPho; ++i) {

                        // selections on RECO particle
                        if (!ggHi.passedPhoSpikeRejection(i)) continue;

                        if (cut_hovere != 0) {
                            if (!((*ggHi.phoHoverE)[i] < cut_hovere))   continue;
                        }

                        if (excludeHI18HEMfailure && !ggHi.passedHI18HEMfailurePho(i))  continue;

                        double pt  = (*ggHi.phoEt)[i];
                        double eta = (*ggHi.phoEta)[i];
                        double phi = (*ggHi.phoPhi)[i];
                        double sumIso = ((*ggHi.pho_ecalClusterIsoR4)[i] +
                                (*ggHi.pho_hcalRechitIsoR4)[i]  +
                                (*ggHi.pho_trackIsoR4PtCut20)[i]);
                        double sieie = (*ggHi.phoSigmaIEtaIEta_2012)[i];
                        double r9 = (*ggHi.phoR9)[i];

                        // selections on GEN particle
                        int genMatchedIndex = (*ggHi.pho_genMatchedIndex)[i];
                        bool isMatched = (genMatchedIndex >= 0);
                        bool isMatched2GenPhoton = (isMatched && (*ggHi.mcPID)[genMatchedIndex] == 22);

                        std::vector<double> varsFake = {eta, -1, pt, (double)cent, sumIso, sieie, r9};
                        for (int iAna = 0;  iAna < nRecoAna; ++iAna) {

                            rAna[RECOANA::kETA][iAna].FillHDenomFake(eta, w, varsFake);
                            //rAna[RECOANA::kGENPT][iAna].FillHDenomFake(genPt, w, varsFake);
                            rAna[RECOANA::kRECOPT][iAna].FillHDenomFake(pt, w, varsFake);
                            rAna[RECOANA::kCENT][iAna].FillHDenomFake(cent, w, varsFake);
                            rAna[RECOANA::kSUMISO][iAna].FillHDenomFake(sumIso, w, varsFake);
                            rAna[RECOANA::kSIEIE][iAna].FillHDenomFake(sieie, w, varsFake);

                            if (!isMatched2GenPhoton) {
                                rAna[RECOANA::kETA][iAna].FillHNumFake(eta, w, varsFake);
                                //rAna[RECOANA::kGENPT][iAna].FillHNumFake(genPt, w, varsFake);
                                rAna[RECOANA::kRECOPT][iAna].FillHNumFake(pt, w, varsFake);
                                rAna[RECOANA::kCENT][iAna].FillHNumFake(cent, w, varsFake);
                                rAna[RECOANA::kSUMISO][iAna].FillHNumFake(sumIso, w, varsFake);
                                rAna[RECOANA::kSIEIE][iAna].FillHNumFake(sieie, w, varsFake);
                            }
                        }

                        // fake composition
                        if (runMode[MODES::kFakeComposition]) {
                            int fakePDG = 0;
                            double fakeGenPt = -1;
                            if (!isMatched2GenPhoton) {

                                // identify GEN-level particle that matches the fake at RECO-level
                                double deltaR2 = 0.15*0.15;

                                bool useggHiMC = false;
                                if (useggHiMC) {
                                    int genMatchedIndexTmp = findGenMatchedIndex(ggHi, eta, phi, deltaR2, -777777);
                                    if (genMatchedIndexTmp >= 0) {
                                        fakeGenPt = (*ggHi.mcPt)[genMatchedIndexTmp];
                                        fakePDG = (*ggHi.mcPID)[genMatchedIndexTmp];
                                    }
                                }
                                else {
                                    for (int iMC = 0; iMC < hiGen.mult; ++iMC) {
                                        if (getDR2(eta, phi, (*hiGen.eta)[iMC], (*hiGen.phi)[iMC]) < deltaR2 &&
                                                (*hiGen.pt)[iMC] > fakeGenPt) {

                                            fakeGenPt = (*hiGen.pt)[iMC];
                                            fakePDG = (*hiGen.pdg)[iMC];
                                        }
                                    }
                                }
                                fakePDG = TMath::Abs(fakePDG);
                            }

                            std::vector<double> varsFakeParticle = {eta, -1, pt, (double)cent, sumIso, sieie, r9};
                            for (int iAna = 0;  iAna < nRecoAna; ++iAna) {

                                if (!isMatched2GenPhoton) {
                                    rAna[RECOANA::kETA][iAna].FillHFakeParticle(eta, fakePDG, w, varsFakeParticle);
                                    //rAna[RECOANA::kGENPT][iAna].FillHFakeParticle(genPt, fakePDG, w, varsFakeParticle);
                                    rAna[RECOANA::kRECOPT][iAna].FillHFakeParticle(pt, fakePDG, w, varsFakeParticle);
                                    rAna[RECOANA::kCENT][iAna].FillHFakeParticle(cent, fakePDG, w, varsFakeParticle);
                                    rAna[RECOANA::kSUMISO][iAna].FillHFakeParticle(sumIso, fakePDG, w, varsFakeParticle);
                                    rAna[RECOANA::kSIEIE][iAna].FillHFakeParticle(sieie, fakePDG, w, varsFakeParticle);

                                    rAna[RECOANA::kGENPT][iAna].FillHFakeParticleGenPt(fakeGenPt, fakePDG, w, varsFakeParticle);
                                }
                            }
                        }
                    }
                }
            }
            else if (recoObj == RECOOBJS::kElectron) {

                if (runMode[MODES::kEnergyScale]) {
                    for (int i=0; i<ggHi.nEle; ++i) {

                        if (!ggHi.passedEleSelection(i, collisionType, hiBin))  continue;

                        if (excludeHI18HEMfailure && !ggHi.passedHI18HEMfailureEle(i))  continue;

                        double pt  = (*ggHi.elePt)[i];
                        double eta = (*ggHi.eleEta)[i];
                        double phi = (*ggHi.elePhi)[i];

                        double deltaR2 = 0.15*0.15;
                        int genMatchedPID = 11;

                        int genMatchedIndex = findGenMatchedIndex(ggHi, eta, phi, deltaR2, genMatchedPID);
                        if (genMatchedIndex < 0)   continue;

                        double genPt = (*ggHi.mcPt)[genMatchedIndex];
                        double genE = (*ggHi.mcE)[genMatchedIndex];

                        double energyScale = -1;

                        if (runMode[MODES::kEnergyScale] == kRecoPtGenPt) {
                            energyScale = pt/genPt;
                        }
                        else if (runMode[MODES::kEnergyScale] == kSCRawEGenE) {
                            energyScale = (*ggHi.eleSCRawEn)[i]/genE;
                        }

                        double sumIso = -999;
                        double sieie = (*ggHi.eleSigmaIEtaIEta_2012)[i];
                        double r9 = (*ggHi.eleR9)[i];

                        std::vector<double> vars = {eta, genPt, pt, (double)cent, sumIso, sieie, r9};
                        for (int iAna = 0;  iAna < nRecoAna; ++iAna) {

                            rAna[RECOANA::kETA][iAna].FillH2D(energyScale, eta, w, vars);
                            rAna[RECOANA::kGENPT][iAna].FillH2D(energyScale, genPt, w, vars);
                            rAna[RECOANA::kRECOPT][iAna].FillH2D(energyScale, pt, w, vars);
                            rAna[RECOANA::kCENT][iAna].FillH2D(energyScale, cent, w, vars);
                            rAna[RECOANA::kSUMISO][iAna].FillH2D(energyScale, sumIso, w, vars);
                            rAna[RECOANA::kSIEIE][iAna].FillH2D(energyScale, sieie, w, vars);

                            rAna[RECOANA::kGENPT][iAna].FillH2Dcc(genPt, pt, w, vars);
                        }
                    }
                }

                if (runMode[MODES::kCorrection]) {
                    for (int i=0; i<ggHi.nEle; ++i) {

                        if (!ggHi.passedEleSelection(i, collisionType, hiBin))  continue;

                        if (excludeHI18HEMfailure && !ggHi.passedHI18HEMfailureEle(i))  continue;

                        double pt  = (*ggHi.elePt)[i];
                        double eta = (*ggHi.eleEta)[i];
                        double sumIso = -999;
                        double sieie = (*ggHi.eleSigmaIEtaIEta_2012)[i];
                        double r9 = (*ggHi.eleR9)[i];
                        double eleE = (*ggHi.eleEn)[i];
                        double eleSCE = (*ggHi.eleSCEn)[i];
                        double eleSCRawE = (*ggHi.eleSCRawEn)[i];

                        double corrE = eleE / eleSCE;
                        double corrSCE = eleSCE / eleSCRawE;

                        std::vector<double> vars = {eta, -1, pt, (double)cent, sumIso, sieie, r9};
                        for (int iAna = 0;  iAna < nRecoAna; ++iAna) {

                            rAna[RECOANA::kETA][iAna].FillH2DCorr(corrE, eta, RECOANA::k_corrE, w, vars);
                            //rAna[RECOANA::kGENPT][iAna].varsFillH2DCorr(corrE, genPt, RECOANA::k_corrE, w, vars);
                            rAna[RECOANA::kRECOPT][iAna].FillH2DCorr(corrE, pt, RECOANA::k_corrE, w, vars);
                            rAna[RECOANA::kCENT][iAna].FillH2DCorr(corrE, cent, RECOANA::k_corrE, w, vars);
                            rAna[RECOANA::kSUMISO][iAna].FillH2DCorr(corrE, sumIso, RECOANA::k_corrE, w, vars);
                            rAna[RECOANA::kSIEIE][iAna].FillH2DCorr(corrE, sieie, RECOANA::k_corrE, w, vars);

                            rAna[RECOANA::kETA][iAna].FillH2DCorr(corrSCE, eta, RECOANA::k_corrSCE, w, vars);
                            //rAna[RECOANA::kGENPT][iAna].varsFillH2DCorr(corrSCE, genPt, RECOANA::k_corrSCE, w, vars);
                            rAna[RECOANA::kRECOPT][iAna].FillH2DCorr(corrSCE, pt, RECOANA::k_corrSCE, w, vars);
                            rAna[RECOANA::kCENT][iAna].FillH2DCorr(corrSCE, cent, RECOANA::k_corrSCE, w, vars);
                            rAna[RECOANA::kSUMISO][iAna].FillH2DCorr(corrSCE, sumIso, RECOANA::k_corrSCE, w, vars);
                            rAna[RECOANA::kSIEIE][iAna].FillH2DCorr(corrSCE, sieie, RECOANA::k_corrSCE, w, vars);
                        }
                    }
                }

                if (runMode[MODES::kMatchEff]) {

                    for (int i=0; i<ggHi.nMC; ++i) {

                        if ((*ggHi.mcStatus)[i] != 1)  continue;

                        int genMatchedPID = 11;
                        if (TMath::Abs((*ggHi.mcPID)[i]) != genMatchedPID)  continue;

                        if (excludeHI18HEMfailure && !ggHi.passedHI18HEMfailureGen(i))  continue;

                        double genPt = (*ggHi.mcPt)[i];
                        double genEta = (*ggHi.mcEta)[i];
                        double genPhi = (*ggHi.mcPhi)[i];

                        // look for matching RECO particle
                        double deltaR2 = 0.15*0.15;
                        int iReco = -1;
                        double recoPt = -1;
                        for (int j = 0; j < ggHi.nEle; ++j) {

                            if (!ggHi.passedEleSelection(j, collisionType, hiBin))  continue;

                            if (excludeHI18HEMfailure && !ggHi.passedHI18HEMfailureEle(j))  continue;

                            if (getDR2((*ggHi.eleEta)[j], (*ggHi.elePhi)[j], genEta, genPhi) < deltaR2 && (*ggHi.elePt)[j] > recoPt ) {
                                iReco = j;
                                recoPt = (*ggHi.elePt)[j];
                            }
                        }
                        bool matched2RECO = (iReco > -1);

                        std::vector<double> varsDenom = {genEta, genPt, -1, (double)cent, -999, -1, -1};
                        for (int iAna = 0;  iAna < nRecoAna; ++iAna) {

                            rAna[RECOANA::kETA][iAna].FillHDenom(genEta, w, varsDenom);
                            rAna[RECOANA::kGENPT][iAna].FillHDenom(genPt, w, varsDenom);
                            //rAna[RECOANA::kRECOPT][iAna].FillHDenom(pt, w, varsDenom);
                            rAna[RECOANA::kCENT][iAna].FillHDenom(cent, w, varsDenom);
                            //rAna[RECOANA::kSUMISO][iAna].FillHDenom(sumIso, w, varsDenom);
                            //rAna[RECOANA::kSIEIE][iAna].FillHDenom(sieie, w, varsDenom);

                            if (matched2RECO) {
                                double pt  = (*ggHi.elePt)[iReco];
                                double sumIso = -999;
                                double sieie = (*ggHi.eleSigmaIEtaIEta_2012)[iReco];
                                double r9 = (*ggHi.eleR9)[iReco];
                                std::vector<double> varsNum = {genEta, genPt, pt, (double)cent, sumIso, sieie, r9};

                                rAna[RECOANA::kETA][iAna].FillHNum(genEta, w, varsNum);
                                rAna[RECOANA::kGENPT][iAna].FillHNum(genPt, w, varsNum);
                                //rAna[RECOANA::kRECOPT][iAna].FillHNum(pt, w, varsNum);
                                rAna[RECOANA::kCENT][iAna].FillHNum(cent, w, varsNum);
                                //rAna[RECOANA::kSUMISO][iAna].FillHNum(sumIso, w, varsNum);
                                //rAna[RECOANA::kSIEIE][iAna].FillHNum(sieie, w, varsNum);
                            }

                        }
                    }
                }

                if (runMode[MODES::kFakeRate]) {
                    for (int i=0; i<ggHi.nEle; ++i) {

                        // selections on RECO particle
                        if (!ggHi.passedEleSelection(i, collisionType, hiBin))  continue;

                        if (excludeHI18HEMfailure && !ggHi.passedHI18HEMfailureEle(i))  continue;

                        double pt  = (*ggHi.elePt)[i];
                        double eta = (*ggHi.eleEta)[i];
                        double phi = (*ggHi.elePhi)[i];
                        double sumIso = -999;
                        double sieie = (*ggHi.eleSigmaIEtaIEta_2012)[i];
                        double r9 = (*ggHi.eleR9)[i];

                        // selections on GEN particle
                        double deltaR2 = 0.15*0.15;

                        int genMatchedIndex = findGenMatchedIndex(ggHi, eta, phi, deltaR2, -777777);
                        if (genMatchedIndex < 0)   continue;    // is matched

                        bool isMatched = (genMatchedIndex >= 0);
                        bool isMatched2GenEle = (isMatched && TMath::Abs((*ggHi.mcPID)[genMatchedIndex]) == 11);

                        std::vector<double> varsFake = {eta, -1, pt, (double)cent, sumIso, sieie, r9};
                        for (int iAna = 0;  iAna < nRecoAna; ++iAna) {

                            rAna[RECOANA::kETA][iAna].FillHDenomFake(eta, w, varsFake);
                            //rAna[RECOANA::kGENPT][iAna].FillHDenomFake(genPt, w, varsFake);
                            rAna[RECOANA::kRECOPT][iAna].FillHDenomFake(pt, w, varsFake);
                            rAna[RECOANA::kCENT][iAna].FillHDenomFake(cent, w, varsFake);
                            rAna[RECOANA::kSUMISO][iAna].FillHDenomFake(sumIso, w, varsFake);
                            rAna[RECOANA::kSIEIE][iAna].FillHDenomFake(sieie, w, varsFake);

                            if (!isMatched2GenEle) {
                                rAna[RECOANA::kETA][iAna].FillHNumFake(eta, w, varsFake);
                                //rAna[RECOANA::kGENPT][iAna].FillHNumFake(genPt, w, varsFake);
                                rAna[RECOANA::kRECOPT][iAna].FillHNumFake(pt, w, varsFake);
                                rAna[RECOANA::kCENT][iAna].FillHNumFake(cent, w, varsFake);
                                rAna[RECOANA::kSUMISO][iAna].FillHNumFake(sumIso, w, varsFake);
                                rAna[RECOANA::kSIEIE][iAna].FillHNumFake(sieie, w, varsFake);
                            }
                        }

                        // fake composition
                        if (runMode[MODES::kFakeComposition]) {
                            int fakePDG = 0;
                            double fakeGenPt = -1;
                            if (!isMatched2GenEle) {

                                // identify GEN-level particle that matches the fake at RECO-level

                                bool useggHiMC = false;
                                if (useggHiMC) {
                                    int genMatchedIndexTmp = findGenMatchedIndex(ggHi, eta, phi, deltaR2, -777777);
                                    if (genMatchedIndexTmp >= 0) {
                                        fakeGenPt = (*ggHi.mcPt)[genMatchedIndexTmp];
                                        fakePDG = (*ggHi.mcPID)[genMatchedIndexTmp];
                                    }
                                }
                                else {
                                    for (int iMC = 0; iMC < hiGen.mult; ++iMC) {
                                        if (getDR2(eta, phi, (*hiGen.eta)[iMC], (*hiGen.phi)[iMC]) < deltaR2 &&
                                                (*hiGen.pt)[iMC] > fakeGenPt) {

                                            fakeGenPt = (*hiGen.pt)[iMC];
                                            fakePDG = (*hiGen.pdg)[iMC];
                                        }
                                    }
                                }
                                fakePDG = TMath::Abs(fakePDG);
                            }

                            std::vector<double> varsFakeParticle = {eta, -1, pt, (double)cent, sumIso, sieie, r9};
                            for (int iAna = 0;  iAna < nRecoAna; ++iAna) {

                                if (!isMatched2GenEle) {
                                    rAna[RECOANA::kETA][iAna].FillHFakeParticle(eta, fakePDG, w, varsFakeParticle);
                                    //rAna[RECOANA::kGENPT][iAna].FillHFakeParticle(genPt, fakePDG, w, varsFakeParticle);
                                    rAna[RECOANA::kRECOPT][iAna].FillHFakeParticle(pt, fakePDG, w, varsFakeParticle);
                                    rAna[RECOANA::kCENT][iAna].FillHFakeParticle(cent, fakePDG, w, varsFakeParticle);
                                    rAna[RECOANA::kSUMISO][iAna].FillHFakeParticle(sumIso, fakePDG, w, varsFakeParticle);
                                    rAna[RECOANA::kSIEIE][iAna].FillHFakeParticle(sieie, fakePDG, w, varsFakeParticle);

                                    rAna[RECOANA::kGENPT][iAna].FillHFakeParticleGenPt(fakeGenPt, fakePDG, w, varsFakeParticle);
                                }
                            }
                        }
                    }
                }
            }
        }
        fileTmp->Close();
    }
    std::cout<<  "Loop ENDED : " << treePath.c_str() <<std::endl;
    std::cout << "nFilesSkipped = " << nFilesSkipped << std::endl;
    std::cout << "entries            = " << entries << std::endl;
    std::cout << "duplicateEntries   = " << duplicateEntries << std::endl;
    std::cout << "entriesAnalyzed    = " << entriesAnalyzed << std::endl;

    TFile* output = TFile::Open(outputFile.c_str(),"RECREATE");
    output->cd();

    postLoop();

    std::cout<<"Closing the output file."<<std::endl;
    output->Close();
    std::cout<<"running objRecoAna() - END"<<std::endl;
}

/*
 * run the macro without going through event loop, things done before and after the loop
 */
void objRecoAnaNoLoop(std::string configFile, std::string inputFile, std::string outputFile)
{
    std::cout<<"running objRecoAna()"<<std::endl;
    std::cout<<"configFile  = "<< configFile.c_str() <<std::endl;
    std::cout<<"inputFile   = "<< inputFile.c_str()  <<std::endl;
    std::cout<<"outputFile  = "<< outputFile.c_str() <<std::endl;

    if (readConfiguration(configFile, inputFile) != 0)  return;
    printConfiguration();

    TFile* input = TFile::Open(inputFile.c_str(), "READ");

    if (preLoop(input, false) != 0) return;

    TFile* output = TFile::Open(outputFile.c_str(),"RECREATE");
    output->cd();

    postLoop();

    std::cout<<"Closing the input file."<<std::endl;
    input->Close();
    std::cout<<"Closing the output file."<<std::endl;
    output->Close();
    std::cout<<"running objRecoAna() - END"<<std::endl;
}

int main(int argc, char** argv)
{
    std::vector<std::string> argStr = ArgumentParser::ParseParameters(argc, argv);
    int nArgStr = argStr.size();

    std::vector<std::string> argOptions = ArgumentParser::ParseOptions(argc, argv);
    bool noLoop = (findPositionInVector(argOptions, ARGUMENTPARSER::noLoop) >= 0);

    if (nArgStr == 4) {
        if (noLoop) objRecoAnaNoLoop(argStr.at(1), argStr.at(2), argStr.at(3));
        else        objRecoAna(argStr.at(1), argStr.at(2), argStr.at(3));
        return 0;
    }
    else if (nArgStr == 3) {
        if (noLoop) objRecoAnaNoLoop(argStr.at(1), argStr.at(2));
        else        objRecoAna(argStr.at(1), argStr.at(2));
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./objRecoAna.exe <configFile> <inputFile> <outputFile>"
                << std::endl;
        return 1;
    }
}

int readConfiguration(std::string configFile, std::string inputFile)
{
    ConfigurationParser confParser;
    confParser.openConfigFile(configFile);

    confParser.parsedKeyWords = InputConfigurationParser::parseKeyWords(inputFile);

    recoObj = parseRecoObj(confParser.ReadConfigValue("recoObj"));
    mode = confParser.ReadConfigValue("mode");
    runMode = parseMode(mode);

    // input for TTree
    treePath = confParser.ReadConfigValue("treePath");
    collisionType = confParser.ReadConfigValueInteger("collisionType");

    // input for TMVA
    tmvaXMLFiles = ConfigurationParser::ParseListOrString(confParser.ReadConfigValue("tmvaXMLFiles"));
    tmvaMethodNames = ConfigurationParser::ParseListOrString(confParser.ReadConfigValue("tmvaMethodNames"));
    tmvaReaderVars = ConfigurationParser::ParseListOfList(confParser.ReadConfigValue("tmvaReaderVariables"));
    tmvaReaderSpectators = ConfigurationParser::ParseListOfList(confParser.ReadConfigValue("tmvaReaderSpectators"));

    tmva_bins_eta[0] = ConfigurationParser::ParseListFloat(confParser.ReadConfigValue("tmva_bins_eta_gt"));
    tmva_bins_eta[1] = ConfigurationParser::ParseListFloat(confParser.ReadConfigValue("tmva_bins_eta_lt"));

    tmva_bins_pt[0] = ConfigurationParser::ParseListFloat(confParser.ReadConfigValue("tmva_bins_pt_gt"));
    tmva_bins_pt[1] = ConfigurationParser::ParseListFloat(confParser.ReadConfigValue("tmva_bins_pt_lt"));

    // input for TH1
    // nBins, xLow, xUp for the TH1D histogram
    // this bin list will be used for histograms where x-axis is eta.
    TH1D_Axis_List = ConfigurationParser::ParseListTH1D_Axis(confParser.ReadConfigValue("TH1D_Bins_List"));
    // nBinsx, xLow, xUp, nBinsy, yLow, yUp for a TH2D histogram
    // this bin list will be used for gen pt - reco pt correlation histogram.
    TH2D_Axis_List = ConfigurationParser::ParseListTH2D_Axis(confParser.ReadConfigValue("TH2D_Bins_List"));

    title = confParser.ReadConfigValue("TH1_title");
    titleOffsetX = confParser.ReadConfigValueFloat("titleOffsetX");
    titleOffsetY = confParser.ReadConfigValueFloat("titleOffsetY");

    // min. y-axis value of energy Scale histogram default : 0.8
    yMin = ConfigurationParser::ParseListOrFloat(confParser.ReadConfigValue("TH1_yMin"));
    // max. y-axis value of energy Scale histogram default : 1.5
    yMax = ConfigurationParser::ParseListOrFloat(confParser.ReadConfigValue("TH1_yMax"));

    // input for TH1
    markerSizes = ConfigurationParser::ParseListOrFloat(confParser.ReadConfigValue("markerSize"));
    markerStyles = ConfigurationParser::ParseList(confParser.ReadConfigValue("markerStyle"));
    colors = ConfigurationParser::ParseList(confParser.ReadConfigValue("color"));

    // input for TLegend
    tmpLegend = ConfigurationParser::ParseLatex(confParser.ReadConfigValue("legendEntryLabel"));
    legendEntries = ConfigurationParser::ParseListOfList(tmpLegend);
    legendEntryLabels = ConfigurationParser::getVecString(legendEntries);
    legendEntryPadIndices = ConfigurationParser::getVecIndex(legendEntries);
    legendPositions = ConfigurationParser::ParseListOrString(confParser.ReadConfigValue("legendPosition"));
    legendOffsetsX = ConfigurationParser::ParseListOrFloat(confParser.ReadConfigValue("legendOffsetX"));
    legendOffsetsY = ConfigurationParser::ParseListOrFloat(confParser.ReadConfigValue("legendOffsetY"));
    legendBorderSizes = ConfigurationParser::ParseListOrInteger(confParser.ReadConfigValue("legendBorderSize"));
    legendWidths = ConfigurationParser::ParseListOrFloat(confParser.ReadConfigValue("legendWidth"));
    legendHeights = ConfigurationParser::ParseListOrFloat(confParser.ReadConfigValue("legendHeight"));
    legendTextSizes = ConfigurationParser::ParseListOrFloat(confParser.ReadConfigValue("legendTextSizes"));

    // input for text objects
    std::string tmpText = ConfigurationParser::ParseLatex(confParser.ReadConfigValue("text"));
    textEntries = ConfigurationParser::ParseListOfList(tmpText);
    textLines = ConfigurationParser::getVecString(textEntries);
    textLinePadIndices = ConfigurationParser::getVecIndex(textEntries);
    textPositions = ConfigurationParser::ParseListOrString(confParser.ReadConfigValue("textPosition"));
    textFonts = ConfigurationParser::ParseListOrInteger(confParser.ReadConfigValue("textFont"));
    textSizes = ConfigurationParser::ParseListOrFloat(confParser.ReadConfigValue("textSize"));
    textOffsetsX = ConfigurationParser::ParseListOrFloat(confParser.ReadConfigValue("textOffsetX"));
    textOffsetsY = ConfigurationParser::ParseListOrFloat(confParser.ReadConfigValue("textOffsetY"));

    // input for TCanvas
    windowWidth = confParser.ReadConfigValueInteger("windowWidth");
    windowHeight = confParser.ReadConfigValueInteger("windowHeight");
    leftMargin   = confParser.ReadConfigValueFloat("leftMargin");
    rightMargin  = confParser.ReadConfigValueFloat("rightMargin");
    bottomMargin = confParser.ReadConfigValueFloat("bottomMargin");
    topMargin    = confParser.ReadConfigValueFloat("topMargin");

    // set default values
    if (treePath.size() == 0) treePath = "ggHiNtuplizer/EventTree";

    if (TH1D_Axis_List.size() == 0) {
        CONFIGPARSER::TH1Axis th1Axis;
        th1Axis.nBins = 12;
        th1Axis.xLow = -2.4;
        th1Axis.xUp = 2.4;
        TH1D_Axis_List.push_back(th1Axis);
    }
    if (TH2D_Axis_List.size() == 0) {
        CONFIGPARSER::TH2DAxis th2DAxis;

        th2DAxis.axisX.nBins = 120;
        th2DAxis.axisX.xLow = 0;
        th2DAxis.axisX.xUp = 120;
        th2DAxis.axisY.nBins = 120;
        th2DAxis.axisY.xLow = 0;
        th2DAxis.axisY.xUp = 120;

        TH2D_Axis_List.push_back(th2DAxis);
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

    nTmvaXMLFiles = tmvaXMLFiles.size();
    nTmvaMethods = tmvaMethodNames.size();
    while ((int)tmva_bins_eta[0].size() < nTmvaXMLFiles) {
        tmva_bins_eta[0].push_back(0);
    }
    while ((int)tmva_bins_eta[1].size() < nTmvaXMLFiles) {
        tmva_bins_eta[1].push_back(-1);
    }
    while ((int)tmva_bins_pt[0].size() < nTmvaXMLFiles) {
        tmva_bins_pt[0].push_back(0);
    }
    while ((int)tmva_bins_pt[1].size() < nTmvaXMLFiles) {
        tmva_bins_pt[1].push_back(-1);
    }
    nTmva_bins_eta = tmva_bins_eta[0].size();
    nTmva_bins_pt = tmva_bins_pt[0].size();

    collisionName = getCollisionTypeName((COLL::TYPE)collisionType).c_str();
    nTH1D_Axis_List = TH1D_Axis_List.size();
    nTH2D_Axis_List = TH2D_Axis_List.size();

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

    bins_eta[0] = ConfigurationParser::ParseListFloat(confParser.ReadConfigValue("bins_eta_gt"));
    bins_eta[1] = ConfigurationParser::ParseListFloat(confParser.ReadConfigValue("bins_eta_lt"));

    bins_genPt[0] = ConfigurationParser::ParseListFloat(confParser.ReadConfigValue("bins_mcPt_gt"));
    bins_genPt[1] = ConfigurationParser::ParseListFloat(confParser.ReadConfigValue("bins_mcPt_lt"));

    bins_recoPt[0] = ConfigurationParser::ParseListFloat(confParser.ReadConfigValue("bins_pt_gt"));
    bins_recoPt[1] = ConfigurationParser::ParseListFloat(confParser.ReadConfigValue("bins_pt_lt"));

    bins_cent[0] = ConfigurationParser::ParseListInteger(confParser.ReadConfigValue("bins_cent_gt"));
    bins_cent[1] = ConfigurationParser::ParseListInteger(confParser.ReadConfigValue("bins_cent_lt"));

    bins_sumIso[0] = ConfigurationParser::ParseListFloat(confParser.ReadConfigValue("bins_sumIso_gt"));
    bins_sumIso[1] = ConfigurationParser::ParseListFloat(confParser.ReadConfigValue("bins_sumIso_lt"));

    bins_sieie[0] = ConfigurationParser::ParseListFloat(confParser.ReadConfigValue("bins_sieie_gt"));
    bins_sieie[1] = ConfigurationParser::ParseListFloat(confParser.ReadConfigValue("bins_sieie_lt"));

    bins_r9[0] = ConfigurationParser::ParseListFloat(confParser.ReadConfigValue("bins_r9_gt"));
    bins_r9[1] = ConfigurationParser::ParseListFloat(confParser.ReadConfigValue("bins_r9_lt"));

    // event cuts/weights
    doEventWeight = confParser.ReadConfigValueInteger("doEventWeight");
    pthatWeights = ConfigurationParser::ParseListTriplet(confParser.ReadConfigValue("pthatWeights"));

    nPthatWeights = pthatWeights[0].size();

    // RECO photon cuts
    cut_hovere = confParser.ReadConfigValueFloat("hovere");
    excludeHI18HEMfailure = (confParser.ReadConfigValueInteger("excludeHI18HEMfailure") > 0);

    // GEN photon cuts
    cut_mcCalIsoDR04 = confParser.ReadConfigValueFloat("mcCalIsoDR04");
    cut_mcTrkIsoDR04 = confParser.ReadConfigValueFloat("mcTrkIsoDR04");
    cut_mcSumIso = confParser.ReadConfigValueFloat("mcSumIso");

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
    if (bins_cent[0].size() == 0) {
        bins_cent[0].push_back(0);
        bins_cent[1].push_back(-1);
    }
    if (bins_sumIso[0].size() == 0) {
        bins_sumIso[0].push_back(0);
        bins_sumIso[1].push_back(-1);
    }
    if (bins_sieie[0].size() == 0) {
        bins_sieie[0].push_back(0);
        bins_sieie[1].push_back(-1);
    }
    if (bins_r9[0].size() == 0) {
        bins_r9[0].push_back(0);
        bins_r9[1].push_back(-1);
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
    nBins_cent = bins_cent[0].size();     // assume <myvector>[0] and <myvector>[1] have the same size.
    nBins_sumIso = bins_sumIso[0].size();     // assume <myvector>[0] and <myvector>[1] have the same size.
    nBins_sieie = bins_sieie[0].size();     // assume <myvector>[0] and <myvector>[1] have the same size.
    nBins_r9 = bins_r9[0].size();     // assume <myvector>[0] and <myvector>[1] have the same size.

    nRecoAna = nBins_eta * nBins_genPt * nBins_recoPt * nBins_cent * nBins_sumIso * nBins_sieie * nBins_r9;
    return 0;
}

/*
 * print information read from configuration
 * assumes that readConfiguration() is run before
 */
void printConfiguration()
{
    std::cout<<"Configuration :"<<std::endl;
    std::cout << "reco object = " << recoObjsStr[recoObj].c_str() << std::endl;
    if (recoObj < 0 || recoObj >= RECOOBJS::kN_RECOOBJS) {
        std::cout << "ERROR : no valid reco object given" << std::endl;
        std::cout << "recoObj (index for reco object) = " << recoObj << std::endl;
    }
    std::cout << "mode = " << mode.c_str() << std::endl;
    for (int i = 0; i < (int)runMode.size(); ++i) {
        std::cout << "run " << modesStr[i].c_str() << " = " << runMode.at(i) << std::endl;
    }

    std::cout << "treePath = " << treePath.c_str() << std::endl;
    std::cout << "collision = " << collisionName.c_str() << std::endl;

    std::cout << "nTmvaXMLFiles = " << nTmvaXMLFiles << std::endl;
    for (int i = 0; i < nTmvaXMLFiles; ++i) {
        std::cout << Form("tmvaXMLFiles[%d] = %s", i, tmvaXMLFiles.at(i).c_str()) << std::endl;
    }
    std::cout << "nTmvaMethod = " << nTmvaMethods << std::endl;
    for (int i = 0; i < nTmvaMethods; ++i) {
        std::cout << Form("tmvaMethodNames[%d] = %s", i, tmvaMethodNames.at(i).c_str()) << std::endl;
    }

    std::vector<std::string> tmvaReaderVarsStr = ConfigurationParser::getVecString(tmvaReaderVars);
    std::vector<int> tmvaReaderVarIndices = ConfigurationParser::getVecIndex(tmvaReaderVars);

    std::vector<std::string> tmvaReaderSpecsStr = ConfigurationParser::getVecString(tmvaReaderSpectators);
    std::vector<int> tmvaReaderSpecsIndices = ConfigurationParser::getVecIndex(tmvaReaderSpectators);
    for (int iXML = 0; iXML < nTmvaXMLFiles; ++iXML) {
        int nVarsTmp = std::count(tmvaReaderVarIndices.begin(), tmvaReaderVarIndices.end(), iXML);
        std::cout << "Number of reader variables for XML file " << iXML << " = " << nVarsTmp << std::endl;
        std::cout << "Reader variables for XML file " << iXML << " are :" << std::endl;

        int iStart = findInVector(tmvaReaderVarIndices, iXML);
        for (int i = iStart; i < iStart+nVarsTmp; ++i) {

            std::cout << Form("tmvaReaderVars[%d] = %s", i-iStart, tmvaReaderVarsStr.at(i).c_str()) << std::endl;
        }

        int nSpecsTmp = std::count(tmvaReaderSpecsIndices.begin(), tmvaReaderSpecsIndices.end(), iXML);
        std::cout << "Number of spectators for XML file " << iXML << " = " << nSpecsTmp << std::endl;
        std::cout << "Spectators for XML file " << iXML << " are :" << std::endl;

        iStart = findInVector(tmvaReaderSpecsIndices, iXML);
        for (int i = iStart; i < iStart+nSpecsTmp; ++i) {

            std::cout << Form("tmvaReaderSpectators[%d] = %s", i-iStart, tmvaReaderSpecsStr.at(i).c_str()) << std::endl;
        }
    }

    std::cout << "nTmva_bins_eta = " << nTmva_bins_eta << std::endl;
    for (int i=0; i<nTmva_bins_eta; ++i) {
        std::cout << Form("tmva_bins_eta[%d] = [%f, %f)", i, tmva_bins_eta[0].at(i), tmva_bins_eta[1].at(i)) << std::endl;
    }
    std::cout << "nTmva_bins_pt = " << nTmva_bins_pt << std::endl;
    for (int i=0; i<nTmva_bins_pt; ++i) {
        std::cout << Form("tmva_bins_pt[%d] = [%f, %f)", i, tmva_bins_pt[0].at(i), tmva_bins_pt[1].at(i)) << std::endl;
    }

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
    std::cout << "nPthatWeights = " << nPthatWeights << std::endl;
    for (int i = 0; i < nPthatWeights; ++i) {
        std::cout << Form("pthatWeights[%d] = { ", i);
        std::cout << Form("%.0f, ", pthatWeights[0].at(i));
        std::cout << Form("%f, ", pthatWeights[1].at(i));
        std::cout << Form("%f }", pthatWeights[2].at(i)) << std::endl;;
    }

    std::cout<<"cut_hovere = "<< cut_hovere <<std::endl;
    std::cout<<"excludeHI18HEMfailure = " << excludeHI18HEMfailure << std::endl;

    std::cout<<"cut_mcCalIsoDR04 = "<< cut_mcCalIsoDR04 <<std::endl;
    std::cout<<"cut_mcTrkIsoDR04 = "<< cut_mcTrkIsoDR04 <<std::endl;
    std::cout<<"cut_mcSumIso     = "<< cut_mcSumIso <<std::endl;

    std::cout<<"Input Configuration (Cont'd) :"<<std::endl;

    std::cout << "nTH1D_Axis_List = " << nTH1D_Axis_List << std::endl;
    for (int i=0; i<nTH1D_Axis_List; ++i) {
        std::string strTH1D_Axis = ConfigurationParser::verboseTH1D_Axis(TH1D_Axis_List.at(i));
        std::cout << Form("TH1D_Axis_List[%d] = %s", i, strTH1D_Axis.c_str()) << std::endl;
    }
    std::cout << "nTH2D_Axis_List = " << nTH2D_Axis_List << std::endl;
    for (int i=0; i<nTH2D_Axis_List; ++i) {
        std::string strTH2D_Axis = ConfigurationParser::verboseTH2D_Axis(TH2D_Axis_List.at(i));
        std::cout << Form("TH2D_Axis_List[%d] = %s", i, strTH2D_Axis.c_str()) << std::endl;
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

int parseRecoObj(std::string recoObjStr)
{
    recoObjStr = trim(recoObjStr);
    recoObjStr = toLowerCase(recoObjStr);

    if (recoObjStr == "pho" || recoObjStr == "photon" || recoObjStr == "0") {
        return RECOOBJS::kPhoton;
    }
    else if (recoObjStr == "ele" || recoObjStr == "electron" || recoObjStr == "1") {
        return RECOOBJS::kElectron;
    }
    else {
        return -1;
    }
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

    // special cases
    // If "fake composition" is to be run, then "fake rate" must be run as well.
    if (res[MODES::kFakeComposition])  res[MODES::kFakeRate] = true;

    return res;
}

/*
 * given the indices for analysis bins, return the vector index
 */
int getVecIndex(std::vector<int> binIndices)
{
    int n = binIndices.size();
    if (n != ANABINS::kN_ANABINS) return -1;

    int nTmp = nRecoAna / nBins_eta;
    int i = binIndices[ANABINS::kEta] * nTmp;

    nTmp /= nBins_genPt;
    i += binIndices[ANABINS::kGenPt] * nTmp;

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
    if (i > nRecoAna)  return {};

    std::vector<int> binIndices;
    binIndices.resize(ANABINS::kN_ANABINS);

    int iTmp = i;
    int nTmp = nRecoAna;

    // eta bin index
    nTmp /= nBins_eta;
    binIndices[ANABINS::kEta] = iTmp / nTmp;

    iTmp = i % nTmp;
    nTmp /= nBins_genPt;
    binIndices[ANABINS::kGenPt] = iTmp / nTmp;

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

    for (int iDep = 0; iDep < RECOANA::kN_DEPS; ++iDep) {

        rAna[iDep].clear();
        rAna[iDep].resize(nRecoAna);

        for (int iAna = 0; iAna < nRecoAna; ++iAna) {

        std::vector<int> binIndices = getBinIndices(iAna);

        int iEta = binIndices[ANABINS::kEta];
        int iGenPt = binIndices[ANABINS::kGenPt];
        int iRecoPt = binIndices[ANABINS::kRecoPt];
        int iCent = binIndices[ANABINS::kCent];
        int iSumIso = binIndices[ANABINS::kSumIso];
        int iSieie = binIndices[ANABINS::kSieie];
        int iR9 = binIndices[ANABINS::kR9];

        if (iEta > 0 && iGenPt > 0 && iRecoPt > 0 && iCent > 0 && iSumIso > 0 && iSieie > 0)  continue;

        // for histograms with a particular dependence,
        // a single index is used in the multidimensional array of recoAnalyzer objects.
        // Example : for a reco analysis object with eta dependence (eta is the x-axis), there will not be different objects
        // with different eta ranges.
        // For eta dependence, the objects will have iEta = 0, but not iEta > 0
        // In general there will be no object with all of iEta, iRecoPt, ..., > 0

        std::string strDep = "";
        std::string xTitle = "";
        bool makeObject = false;
        std::string recoObjStr = recoObjsStr[recoObj].c_str();

        if (iEta == 0 && iDep == RECOANA::kETA) {
            strDep = "depEta";
            xTitle = Form("%s #eta", recoObjStr.c_str());
            makeObject = !rAna[iDep][iAna].isValid();
        }
        else if (iGenPt == 0 && iDep == RECOANA::kGENPT) {
            strDep = "depGenPt";
            xTitle = "Gen p_{T} (GeV/c)";
            makeObject = !rAna[iDep][iAna].isValid();
        }
        else if (iRecoPt == 0 && iDep == RECOANA::kRECOPT) {
            strDep = "depRecoPt";
            xTitle = "Reco p_{T} (GeV/c)";
            makeObject = !rAna[iDep][iAna].isValid();
        }
        else if (iCent == 0 && iDep == RECOANA::kCENT) {
            strDep = "depCent";
            xTitle = "Centrality (%)";
            makeObject = !rAna[iDep][iAna].isValid();
        }
        else if (iSumIso == 0 && iDep == RECOANA::kSUMISO) {
            strDep = "depSumIso";
            xTitle = "sumIso";
            makeObject = !rAna[iDep][iAna].isValid();
        }
        else if (iSieie == 0 && iDep == RECOANA::kSIEIE) {
            strDep = "depSieie";
            xTitle = "#sigma_{#eta#eta}";
            makeObject = !rAna[iDep][iAna].isValid();
        }

        if (!makeObject)  continue;

        recoAnalyzer rAnaTmp;
        rAnaTmp.recoObj = -1;
        if (recoObj == RECOOBJS::kPhoton) {
            rAnaTmp.recoObj = RECOANA::OBJS::kPHOTON;
        }
        else if (recoObj == RECOOBJS::kElectron) {
            rAnaTmp.recoObj = RECOANA::OBJS::kELECTRON;
        }
        rAnaTmp.dep = iDep;

        // histogram ranges
        rAnaTmp.ranges[RECOANA::rETA][0] = bins_eta[0].at(iEta);
        rAnaTmp.ranges[RECOANA::rETA][1] = bins_eta[1].at(iEta);
        rAnaTmp.ranges[RECOANA::rGENPT][0] = bins_genPt[0].at(iGenPt);
        rAnaTmp.ranges[RECOANA::rGENPT][1] = bins_genPt[1].at(iGenPt);
        rAnaTmp.ranges[RECOANA::rRECOPT][0] = bins_recoPt[0].at(iRecoPt);
        rAnaTmp.ranges[RECOANA::rRECOPT][1] = bins_recoPt[1].at(iRecoPt);
        rAnaTmp.ranges[RECOANA::rCENT][0] = bins_cent[0].at(iCent);
        rAnaTmp.ranges[RECOANA::rCENT][1] = bins_cent[1].at(iCent);
        rAnaTmp.ranges[RECOANA::rSUMISO][0] = bins_sumIso[0].at(iSumIso);
        rAnaTmp.ranges[RECOANA::rSUMISO][1] = bins_sumIso[1].at(iSumIso);
        rAnaTmp.ranges[RECOANA::rSIEIE][0] = bins_sieie[0].at(iSieie);
        rAnaTmp.ranges[RECOANA::rSIEIE][1] = bins_sieie[1].at(iSieie);
        rAnaTmp.ranges[RECOANA::rR9][0] = bins_r9[0].at(iR9);
        rAnaTmp.ranges[RECOANA::rR9][1] = bins_r9[1].at(iR9);

        std::string tmpName = Form("%s_etaBin%d_genPtBin%d_recoPtBin%d_centBin%d", strDep.c_str(), iEta, iGenPt, iRecoPt, iCent);
        if (nBins_sumIso > 1) {
            tmpName.append(Form("_sumIsoBin%d", iSumIso));
        }
        if (nBins_sieie > 1) {
            tmpName.append(Form("_sieieBin%d", iSieie));
        }
        if (nBins_r9 > 1) {
            tmpName.append(Form("_r9Bin%d", iR9));
        }
        rAnaTmp.name = tmpName.c_str();
        rAnaTmp.title = title.c_str();
        rAnaTmp.titleX = xTitle.c_str();
        rAnaTmp.titleOffsetX = titleOffsetX;
        rAnaTmp.titleOffsetY = titleOffsetY;

        rAnaTmp.textLines = textLines;

        std::string nameH2D = Form("h2D_%s", tmpName.c_str());
        std::string nameH2DcorrE = rAnaTmp.getObjectName(recoAnalyzer::OBJ::k_corrE, recoAnalyzer::TOBJ::kTH2D);
        std::string nameH2DcorrSCE = rAnaTmp.getObjectName(recoAnalyzer::OBJ::k_corrSCE, recoAnalyzer::TOBJ::kTH2D);
        std::string nameEscale = Form("h_%s", tmpName.c_str());
        std::string nameMatchNum = rAnaTmp.getObjectName(recoAnalyzer::OBJ::kMatchNum);
        std::string nameMatchDenom = rAnaTmp.getObjectName(recoAnalyzer::OBJ::kMatchDenom);
        std::string nameFakeNum = rAnaTmp.getObjectName(recoAnalyzer::OBJ::kFakeNum);
        std::string nameFakeDenom = rAnaTmp.getObjectName(recoAnalyzer::OBJ::kFakeDenom);

        // disable the cuts/ranges for this dependence
        // Ex. If the dependence is GenPt (GenPt is the x-axis),
        // then there will not be GenPt cuts (eg. GenPt > 20) for this histogram.
        // The x-axis bins will set the cuts.
        if (iDep == RECOANA::kETA) {
            rAnaTmp.ranges[RECOANA::rETA][0] = 0;
            rAnaTmp.ranges[RECOANA::rETA][1] = -1;
        }
        else if (iDep == RECOANA::kGENPT) {
            rAnaTmp.ranges[RECOANA::rGENPT][0] = 0;
            rAnaTmp.ranges[RECOANA::rGENPT][1] = -1;
        }
        else if (iDep == RECOANA::kRECOPT) {
            rAnaTmp.ranges[RECOANA::rRECOPT][0] = 0;
            rAnaTmp.ranges[RECOANA::rRECOPT][1] = -1;
        }
        else if (iDep == RECOANA::kCENT) {
            rAnaTmp.ranges[RECOANA::rCENT][0] = 0;
            rAnaTmp.ranges[RECOANA::rCENT][1] = -1;
        }
        else if (iDep == RECOANA::kSUMISO) {
            rAnaTmp.ranges[RECOANA::rSUMISO][0] = 0;
            rAnaTmp.ranges[RECOANA::rSUMISO][1] = -1;
        }
        else if (iDep == RECOANA::kSIEIE) {
            rAnaTmp.ranges[RECOANA::rSIEIE][0] = 0;
            rAnaTmp.ranges[RECOANA::rSIEIE][1] = -1;
        }

        int iAxis = iDep;
        int nBins = TH1D_Axis_List[iAxis].nBins;  // nBins
        std::vector<double> bins = TH1D_Axis_List[iAxis].bins;

        // extract the x-axis bin information for energy scale distribution
        int iAxisEscale = RECOANA::kN_DEPS;
        CONFIGPARSER::TH1Axis axisEscale = TH1D_Axis_List[iAxisEscale];

        // extract the x-axis bin information for fake composition as function of genPt
        int iAxisFakeGenPt = RECOANA::kN_DEPS+1;
        CONFIGPARSER::TH1Axis axisFakeGenPt = TH1D_Axis_List[iAxisFakeGenPt];

        double arr[nBins+1];
        std::copy(bins.begin(), bins.end(), arr);

        // energy scale
        if (runMode[MODES::kEnergyScale]) {
            if (makeNew) {
                std::string yTitleEScale = "";
                if (runMode[MODES::kEnergyScale] == kRecoPtGenPt)  yTitleEScale = "Reco p_{T} / Gen p_{T}";
                else if (runMode[MODES::kEnergyScale] == kSCRawEGenE)  yTitleEScale = "SC Raw E / Gen E";
                else if (runMode[MODES::kEnergyScale] == kRecoPtGenPtmeson0)  yTitleEScale = Form("Reco %s p_{T} / Gen #h^{0} p_{T}", recoObjStr.c_str());
                else if (runMode[MODES::kEnergyScale] == kSCRawEGenEmeson0)  yTitleEScale = Form("%s SC Raw E / Gen #h^{0} E", recoObjStr.c_str());
                else if (runMode[MODES::kEnergyScale] == kRecoPtGenPtele)  yTitleEScale = Form("Reco %s p_{T} / Gen e^{#pm} p_{T}", recoObjStr.c_str());
                else if (runMode[MODES::kEnergyScale] == kSCRawEGenEele)  yTitleEScale = Form("%s SC Raw E / Gen e^{#pm} E", recoObjStr.c_str());

                rAnaTmp.h2D =
                        new TH2D(nameH2D.c_str(), Form(";%s;%s", xTitle.c_str(), yTitleEScale.c_str()), nBins, arr,
                                axisEscale.nBins, axisEscale.xLow,  axisEscale.xUp);
            }
            else {
                rAnaTmp.h2D = (TH2D*)input->Get(nameH2D.c_str());
            }

            // set y-axis ranges for energy scale and resolution histograms
            if (nyMin == 1)  rAnaTmp.yMin[0] = yMin[0];
            else if (nyMin == 2)  rAnaTmp.yMin = yMin;
            if (nyMax == 1)  rAnaTmp.yMax[0] = yMax[0];
            else if (nyMax == 2)  rAnaTmp.yMax = yMax;
        }

        // correction
        if (runMode[MODES::kCorrection]) {
        // correction histograms will not depend on GEN-level info.
        if (iDep != RECOANA::kGENPT && iGenPt == 0) {
            if (makeNew) {
                rAnaTmp.h2Dcorr[RECOANA::k_corrE] =
                        new TH2D(nameH2DcorrE.c_str(), Form(";%s;Reco E / SC E", xTitle.c_str()), nBins, arr,
                                100, 0, 2);
                rAnaTmp.h2Dcorr[RECOANA::k_corrSCE] =
                        new TH2D(nameH2DcorrSCE.c_str(), Form(";%s;SC E / Raw SC E", xTitle.c_str()), nBins, arr,
                                100, 0, 2);
            }
            else {
                rAnaTmp.h2Dcorr[RECOANA::k_corrE] = (TH2D*)input->Get(nameH2DcorrE.c_str());
                rAnaTmp.h2Dcorr[RECOANA::k_corrSCE] = (TH2D*)input->Get(nameH2DcorrSCE.c_str());
            }
        }
        }

        // matching efficiency
        if (runMode[MODES::kMatchEff]) {
            if (makeNew) {
                rAnaTmp.hMatchNum =
                        new TH1D(nameMatchNum.c_str(), Form(";%s;Entries", xTitle.c_str()), nBins, arr);
                rAnaTmp.hMatchDenom =
                        (TH1D*)rAnaTmp.hMatchNum->Clone(nameMatchDenom.c_str());
            }
            else {
                rAnaTmp.hMatchNum = (TH1D*)input->Get(nameMatchNum.c_str());
                rAnaTmp.hMatchDenom = (TH1D*)input->Get(nameMatchDenom.c_str());
            }
        }

        // fake rate
        if (runMode[MODES::kFakeRate]) {
            if (makeNew) {
                rAnaTmp.hFakeNum =
                        new TH1D(nameFakeNum.c_str(), Form(";%s;Entries", xTitle.c_str()), nBins, arr);
                rAnaTmp.hFakeDenom =
                        (TH1D*)rAnaTmp.hFakeNum->Clone(nameFakeDenom.c_str());
            }
            else {
                rAnaTmp.hFakeNum = (TH1D*)input->Get(nameFakeNum.c_str());
                rAnaTmp.hFakeDenom = (TH1D*)input->Get(nameFakeDenom.c_str());
            }
        }

        // fake composition
        if (runMode[MODES::kFakeComposition]) {
            int nFakeParticles = rAnaTmp.nFakeParticles;
            for (int iParticle = 0; iParticle < nFakeParticles; ++iParticle) {

                std::string pdgStr = rAnaTmp.getFakePDGstr(iParticle);
                std::string nameFakeParticle =
                        rAnaTmp.getObjectName(recoAnalyzer::OBJ::kFakeParticle);
                nameFakeParticle = replaceAll(nameFakeParticle, "PDG", Form("PDG%s", pdgStr.c_str()));

                std::string nameFakeOther =
                        rAnaTmp.getObjectName(recoAnalyzer::OBJ::kFakeOther);

                if (makeNew) {
                    rAnaTmp.hFakeParticle[iParticle] =
                            new TH1D(nameFakeParticle.c_str(), Form(";%s;Entries", xTitle.c_str()), nBins, arr);

                    if (iParticle == 0) {
                        rAnaTmp.hFakeOther =
                                (TH1D*)rAnaTmp.hFakeParticle[iParticle]->Clone(nameFakeOther.c_str());
                        rAnaTmp.hFakeOther->Reset();
                    }
                }
                else {
                    rAnaTmp.hFakeParticle[iParticle] =
                            (TH1D*)input->Get(nameFakeParticle.c_str());

                    if (iParticle == 0) {
                        rAnaTmp.hFakeOther =
                                (TH1D*)input->Get(nameFakeOther.c_str());
                    }
                }

                // special cases
                if (iDep == RECOANA::kGENPT) {
                    std::string nameFakeParticleGenPt =
                            rAnaTmp.getObjectName(recoAnalyzer::OBJ::kFakeParticleGenPt);
                    nameFakeParticleGenPt = replaceAll(nameFakeParticleGenPt, "PDG", Form("PDG%s", pdgStr.c_str()));

                    std::string nameFakeOtherGenPt =
                            rAnaTmp.getObjectName(recoAnalyzer::OBJ::kFakeOtherGenPt);

                    std::string nameFakeAllGenPt =
                            rAnaTmp.getObjectName(recoAnalyzer::OBJ::kFakeAllGenPt);

                    int nBinsFakeGenPt = axisFakeGenPt.nBins;  // nBins
                    std::vector<double> binsFakeGenPt = axisFakeGenPt.bins;

                    double arrFakeGenPt[nBinsFakeGenPt+1];
                    std::copy(binsFakeGenPt.begin(), binsFakeGenPt.end(), arrFakeGenPt);

                    if (makeNew) {
                        rAnaTmp.hFakeParticleGenPt[iParticle] =
                                new TH1D(nameFakeParticleGenPt.c_str(), Form(";%s;Entries", "Gen p_{T} (GeV/c)"),
                                        nBinsFakeGenPt, arrFakeGenPt);

                        if (iParticle == 0) {
                            rAnaTmp.hFakeOtherGenPt =
                                    (TH1D*)rAnaTmp.hFakeParticleGenPt[iParticle]->Clone(nameFakeOtherGenPt.c_str());
                            rAnaTmp.hFakeOtherGenPt->Reset();

                            rAnaTmp.hFakeAllGenPt =
                                    (TH1D*)rAnaTmp.hFakeParticleGenPt[iParticle]->Clone(
                                            nameFakeAllGenPt.c_str());
                            rAnaTmp.hFakeAllGenPt->Reset();
                        }
                    }
                    else {
                        rAnaTmp.hFakeParticleGenPt[iParticle] =
                                (TH1D*)input->Get(nameFakeParticleGenPt.c_str());

                        if (iParticle == 0) {
                            rAnaTmp.hFakeOtherGenPt =
                                    (TH1D*)input->Get(nameFakeOtherGenPt.c_str());

                            rAnaTmp.hFakeAllGenPt =
                                    (TH1D*)input->Get(nameFakeAllGenPt.c_str());
                        }
                    }
                }
            }

            // special cases
            if (iDep == RECOANA::kGENPT) {
                std::string tmpNameCorrCoeff = Form("h2Dcc_%s", tmpName.c_str());

                int nBinsx2D = TH2D_Axis_List[0].axisX.nBins;    // nBinsx
                int nBinsy2D = TH2D_Axis_List[0].axisY.nBins;    // nBinsy

                std::vector<double> binsx2D = TH2D_Axis_List[0].axisX.bins;
                std::vector<double> binsy2D = TH2D_Axis_List[0].axisY.bins;

                double arrX[nBinsx2D+1];
                std::copy(binsx2D.begin(), binsx2D.end(), arrX);
                double arrY[nBinsy2D+1];
                std::copy(binsy2D.begin(), binsy2D.end(), arrY);

                if (makeNew) {
                    rAnaTmp.h2Dcc =
                            new TH2D(tmpNameCorrCoeff.c_str(), ";Gen p_{T};Reco p_{T}", nBinsx2D, arrX, nBinsy2D, arrY);
                    // h2Dcc will be used only by rAna[RECOANA::kGENPT] object.
                    // By definition, rAna[RECOANA::kEta] and rAna[RECOANA::kCENT] objects would be redundant.
                }
                else {
                    rAnaTmp.h2Dcc = (TH2D*)input->Get(tmpNameCorrCoeff.c_str());
                    // h2Dcc will be used only by rAna[RECOANA::kGENPT] object.
                    // By definition, rAna[RECOANA::kEta] and rAna[RECOANA::kCENT] objects would be redundant.
                }
            }
        }

        rAnaTmp.updateTH1();
        // prepare title and textline using ranges
        rAnaTmp.prepareTitleRanges();
        rAnaTmp.prepareTextLinesRanges();

        rAna[iDep][iAna] = rAnaTmp;
        }
    }

    return 0;
}

int postLoop()
{
    // declaration of graphics objects.
    // the idea is to initialize each Graphics object right before its use, then to delete it right after they are saved/written.
    TCanvas* c = 0;

    for (int iDep = 0; iDep < RECOANA::kN_DEPS; ++iDep) {
        for (int iAna = 0; iAna < nRecoAna; ++iAna) {

            std::vector<int> binIndices = getBinIndices(iAna);

            int iEta = binIndices[ANABINS::kEta];
            int iGenPt = binIndices[ANABINS::kGenPt];
            int iRecoPt = binIndices[ANABINS::kRecoPt];
            int iCent = binIndices[ANABINS::kCent];
            int iSumIso = binIndices[ANABINS::kSumIso];
            int iSieie = binIndices[ANABINS::kSieie];

            if (iDep == RECOANA::kETA && iEta != 0) continue;
            if (iDep == RECOANA::kGENPT && iGenPt != 0) continue;
            if (iDep == RECOANA::kRECOPT && iRecoPt != 0) continue;
            if (iDep == RECOANA::kCENT && iCent != 0) continue;
            if (iDep == RECOANA::kSUMISO && iSumIso != 0) continue;
            if (iDep == RECOANA::kSIEIE && iSieie != 0) continue;

            if (iEta > 0 && iGenPt > 0 && iRecoPt > 0 && iCent > 0 && iSumIso > 0 && iSieie > 0) continue;

            c = new TCanvas("cnvTmp", "", windowWidth, windowHeight);
            setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);

            rAna[iDep][iAna].postLoop();
            rAna[iDep][iAna].writeObjects(c);

            c->Close();         // do not use Delete() for TCanvas.
        }
    }

    // iObs = 0 is energy scale
    // iObs = 1 is energy resolution
    // iObs = 2 is energy scale from histogram mean
    // iObs = 3 is energy resolution from histogram std dev
    // iObs = 4 is energy resolution from sigmaEff
    // iObs = 5 is energy resolution from sigmaHM
    // iObs = 6 is matching efficiency
    // iObs = 7 is fake rate
    for (int iObs = 0; iObs < RECOANA::OBS::kN_OBS; ++iObs) {
        for (int iDep = 0; iDep < RECOANA::kN_DEPS; ++iDep) {

            for (int iAna = 0; iAna < nRecoAna; ++iAna) {

                std::vector<int> binIndices = getBinIndices(iAna);

                int iEta = binIndices[ANABINS::kEta];
                int iGenPt = binIndices[ANABINS::kGenPt];
                int iRecoPt = binIndices[ANABINS::kRecoPt];
                int iCent = binIndices[ANABINS::kCent];
                int iSumIso = binIndices[ANABINS::kSumIso];
                int iSieie = binIndices[ANABINS::kSieie];
                int iR9 = binIndices[ANABINS::kR9];

                // plot from different eta bins
                if (iEta == 0 && rAna[iDep][iAna].name.size() > 0) {
                    drawSame(c, iObs, iDep, {-1, iGenPt, iRecoPt, iCent, iSumIso, iSieie, iR9});
                }

                // plot from different genPt bins
                if (iGenPt == 0 && rAna[iDep][iAna].name.size() > 0) {

                    // there is no genPt bin for fake rate
                    if (iObs != RECOANA::kFAKE) {
                        drawSame(c, iObs, iDep, {iEta, -1, iRecoPt, iCent, iSumIso, iSieie, iR9});
                    }
                }

                // plot from different recoPt bins
                if (iRecoPt == 0 && rAna[iDep][iAna].name.size() > 0) {
                    drawSame(c, iObs, iDep, {iEta, iGenPt, -1, iCent, iSumIso, iSieie, iR9});
                }

                // plot from different centrality bins
                if (iCent == 0 && rAna[iDep][iAna].name.size() > 0) {
                    drawSame(c, iObs, iDep, {iEta, iGenPt, iRecoPt, -1, iSumIso, iSieie, iR9});
                }

                // plot from different sumIso bins
                if (iSumIso == 0 && rAna[iDep][iAna].name.size() > 0) {
                    drawSame(c, iObs, iDep, {iEta, iGenPt, iRecoPt, iCent, -1, iSieie, iR9});
                }

                // plot from different shower shape bins
                if (iSieie == 0 && rAna[iDep][iAna].name.size() > 0) {
                    drawSame(c, iObs, iDep, {iEta, iGenPt, iRecoPt, iCent, iSumIso, -1, iR9});
                }

                // plot from different R9 bins
                if (iR9 == 0 && rAna[iDep][iAna].name.size() > 0) {
                    drawSame(c, iObs, iDep, {iEta, iGenPt, iRecoPt, iCent, iSumIso, iSieie, -1});
                }
            }
        }
    }

    return 0;
}

void drawSame(TCanvas* c, int iObs, int iDep, std::vector<int> binIndices)
{
    if ((iObs == RECOANA::kESCALE || iObs == RECOANA::kERES ||
         iObs == RECOANA::kESCALEARITH || iObs == RECOANA::kERESARITH ||
         iObs == RECOANA::kERESEFF || iObs == RECOANA::kERESHM) &&
            !runMode[MODES::kEnergyScale]) return;
    if (iObs == RECOANA::kEFF && !runMode[MODES::kMatchEff]) return;
    if (iObs == RECOANA::kFAKE && !runMode[MODES::kFakeRate]) return;

    int iEta = binIndices[ANABINS::kEta];
    int iGenPt = binIndices[ANABINS::kGenPt];
    int iRecoPt = binIndices[ANABINS::kRecoPt];
    int iCent = binIndices[ANABINS::kCent];
    int iSumIso = binIndices[ANABINS::kSumIso];
    int iSieie = binIndices[ANABINS::kSieie];
    int iR9 = binIndices[ANABINS::kR9];

    // if the dependency is GenPt (the x-axis is GenPt), then it must be iGenPt = 0
    if (iDep == RECOANA::kETA && iEta != 0) return;
    if (iDep == RECOANA::kGENPT && iGenPt != 0) return;
    if (iDep == RECOANA::kRECOPT && iRecoPt != 0) return;
    if (iDep == RECOANA::kCENT && iCent != 0) return;
    if (iDep == RECOANA::kSUMISO && iSumIso != 0) return;
    if (iDep == RECOANA::kSIEIE && iSieie != 0) return;

    TH1D* hTmp = 0;
    TGraphAsymmErrors* gTmp = 0;

    // decide which bins will be plotted on top.
    std::string tmpName = "";
    std::string strBin = "";
    std::string strBin2 = "";
    int nBins = 0;
    if (iEta == -1) {
        int iAna = getVecIndex({0, iGenPt, iRecoPt, iCent, iSumIso, iSieie, iR9});
        tmpName = rAna[iDep][iAna].name.c_str();
        strBin = "etaBin";
        strBin2 = "etaBinAll";
        nBins = nBins_eta;
    }
    else if (iGenPt == -1) {
        int iAna = getVecIndex({iEta, 0, iRecoPt, iCent, iSumIso, iSieie, iR9});
        tmpName = rAna[iDep][iAna].name.c_str();
        strBin = "genPtBin";
        strBin2 = "genPtBinAll";
        nBins = nBins_genPt;
    }
    else if (iRecoPt == -1) {
        int iAna = getVecIndex({iEta, iGenPt, 0, iCent, iSumIso, iSieie, iR9});
        tmpName = rAna[iDep][iAna].name.c_str();
        strBin = "recoPtBin";
        strBin2 = "recoPtBinAll";
        nBins = nBins_recoPt;
    }
    else if (iCent == -1 && nBins_cent > 1) {
        int iAna = getVecIndex({iEta, iGenPt, iRecoPt, 0, iSumIso, iSieie, iR9});
        tmpName = rAna[iDep][iAna].name.c_str();
        strBin = "centBin";
        strBin2 = "centBinAll";
        nBins = nBins_cent;
    }
    else if (iSumIso == -1 && nBins_sumIso > 1) {
        int iAna = getVecIndex({iEta, iGenPt, iRecoPt, iCent, 0, iSieie, iR9});
        tmpName = rAna[iDep][iAna].name.c_str();
        strBin = "sumIsoBin";
        strBin2 = "sumIsoBinAll";
        nBins = nBins_sumIso;
    }
    else if (iSieie == -1 && nBins_sieie > 1) {
        int iAna = getVecIndex({iEta, iGenPt, iRecoPt, iCent, iSumIso, 0, iR9});
        tmpName = rAna[iDep][iAna].name.c_str();
        strBin = "sieieBin";
        strBin2 = "sieieBinAll";
        nBins = nBins_sieie;
    }
    else if (iR9 == -1 && nBins_r9 > 1) {
        int iAna = getVecIndex({iEta, iGenPt, iRecoPt, iCent, iSumIso, iSieie, 0});
        tmpName = rAna[iDep][iAna].name.c_str();
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
    c->SetName(Form("cnv_%s_%s", RECOANA::OBS_LABELS[iObs].c_str(), tmpName.c_str()));

    setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);

    // prepare analysis indices
    std::vector<int> indicesAna(nBins);
    for (int iBin = 0; iBin < nBins; ++iBin) {

        int iAna = -1;
        if (iEta == -1) iAna = getVecIndex({iBin, iGenPt, iRecoPt, iCent, iSumIso, iSieie, iR9});
        else if (iGenPt == -1) iAna = getVecIndex({iEta, iBin, iRecoPt, iCent, iSumIso, iSieie, iR9});
        else if (iRecoPt == -1) iAna = getVecIndex({iEta, iGenPt, iBin, iCent, iSumIso, iSieie, iR9});
        else if (iCent == -1) iAna = getVecIndex({iEta, iGenPt, iRecoPt, iBin, iSumIso, iSieie, iR9});
        else if (iSumIso == -1) iAna = getVecIndex({iEta, iGenPt, iRecoPt, iCent, iBin, iSieie, iR9});
        else if (iSieie == -1) iAna = getVecIndex({iEta, iGenPt, iRecoPt, iCent, iSumIso, iBin, iR9});
        else if (iR9 == -1) iAna = getVecIndex({iEta, iGenPt, iRecoPt, iCent, iSumIso, iSieie, iBin});

        indicesAna[iBin] = iAna;
    }

    std::vector<TH1D*> vecH1D;
    std::vector<TGraph*> vecGraph;
    std::vector<TObject*> vecObj;
    for (int iBin = 0; iBin < nBins; ++iBin) {

        int iHist = iBin;
        if (iEta == -1) iHist = iBin;
        else if (iGenPt == -1) iHist = nBins_eta + iBin;
        else if (iRecoPt == -1) iHist = nBins_eta + nBins_genPt + iBin;
        else if (iCent == -1) iHist = nBins_eta +  nBins_genPt + nBins_recoPt + iBin;
        else if (iSumIso == -1) iHist = nBins_eta +  nBins_genPt + nBins_recoPt + nBins_cent + iBin;
        else if (iSieie == -1) iHist = nBins_eta +  nBins_genPt + nBins_recoPt + nBins_cent + nBins_sumIso + iBin;
        else if (iR9 == -1) iHist = nBins_eta +  nBins_genPt + nBins_recoPt + nBins_cent + nBins_sumIso + nBins_sieie + iBin;

        int iAnaTmp = indicesAna[iBin];

        if (iObs == RECOANA::kEFF) {
            gTmp = (TGraphAsymmErrors*)rAna[iDep][iAnaTmp].gMatchEff->Clone();

            if (iBin == 0) {
                // dummy histogram to be used as template for the graph
                hTmp = (TH1D*)rAna[iDep][iAnaTmp].h1D[RECOANA::kEFF]->Clone();

                hTmp->SetTitle("");
                hTmp->Reset();
                hTmp->SetMaximum(1.6);
                hTmp->Draw();
            }

            setTGraph(gTmp, iHist);
            vecGraph.push_back(gTmp);
            vecObj.push_back(gTmp);
        }
        else {
            hTmp = (TH1D*)rAna[iDep][iAnaTmp].h1D[iObs]->Clone();

            hTmp->SetTitle("");
            if (iObs == RECOANA::kFAKE)  hTmp->SetMaximum(1.6);

            setTH1(hTmp, iHist);
            vecH1D.push_back(hTmp);
            vecObj.push_back(hTmp);
        }
    }

    if (iObs == RECOANA::kEFF) {
        drawSameTGraph(c, vecGraph);
    }
    else {
        drawSameTH1D(c, vecH1D);
    }
    c->Update();

    TLine* line = new TLine();
    recoAnalyzer::setPad4Observable((TPad*)c, iObs, iDep);

    // vertical lines for pt ranges
    for (int iBin = 0; iBin < nBins; ++iBin) {

        int iAnaTmp = indicesAna[iBin];

        if (iObs == RECOANA::kEFF) {

            rAna[iDep][iAnaTmp].drawLine4PtRange(c, vecGraph[iBin]->GetMarkerColor());
        }
    }

    TLegend* leg = new TLegend();
    // make legend transparent
    leg->SetFillColor(-1);
    leg->SetFillStyle(4000);

    for (int iBin = 0; iBin < nBins; ++iBin) {

        int iAnaTmp = indicesAna[iBin];

        std::string legendOption = "lpf";
        if (iObs == RECOANA::kEFF)  legendOption = "lp";
        std::string legendText = "";
        if (iEta == -1) legendText = rAna[iDep][iAnaTmp].getRangeText(RECOANA::rETA);
        else if (iGenPt == -1) legendText = rAna[iDep][iAnaTmp].getRangeText(RECOANA::rGENPT);
        else if (iRecoPt == -1) legendText = rAna[iDep][iAnaTmp].getRangeText(RECOANA::rRECOPT);
        else if (iCent == -1) legendText = rAna[iDep][iAnaTmp].getRangeText(RECOANA::rCENT);
        else if (iSumIso == -1) legendText = rAna[iDep][iAnaTmp].getRangeText(RECOANA::rSUMISO);
        else if (iSieie == -1) legendText = rAna[iDep][iAnaTmp].getRangeText(RECOANA::rSIEIE);
        else if (iR9 == -1) legendText = rAna[iDep][iAnaTmp].getRangeText(RECOANA::rR9);

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
            RECOANA::rCENT,
            RECOANA::rGENPT,
            RECOANA::rRECOPT,
            RECOANA::rETA,
            RECOANA::rSUMISO,
            RECOANA::rSIEIE,
            RECOANA::rR9
    };
    int nTextRanges = textRanges.size();
    for (int i = 0; i < nTextRanges; ++i) {

        int iRange = textRanges[i];

        // if histograms are from different eta bins, then do not write eta range in the text lines
        if (iEta == -1 && iRange == RECOANA::rETA) continue;
        else if (iGenPt == -1 && iRange == RECOANA::rGENPT) continue;
        else if (iRecoPt == -1 && iRange == RECOANA::rRECOPT) continue;
        else if (iCent == -1 && iRange == RECOANA::rCENT) continue;

        // if the x-axis is eta, then do not write eta range in the text lines
        if (iDep == RECOANA::kETA && iRange == RECOANA::rETA) continue;
        else if (iDep == RECOANA::kGENPT && iRange == RECOANA::rGENPT) continue;
        else if (iDep == RECOANA::kRECOPT && iRange == RECOANA::rRECOPT) continue;
        else if (iDep == RECOANA::kCENT && iRange == RECOANA::rCENT) continue;
        else if (iDep == RECOANA::kSUMISO && iRange == RECOANA::rSUMISO) continue;
        else if (iDep == RECOANA::kSIEIE && iRange == RECOANA::rSIEIE) continue;

        // special cases
        // There are no genPt ranges for fake rate.
        if (iObs == RECOANA::kFAKE && iRange == RECOANA::rGENPT) continue;

        int iAna0 = indicesAna[0];
        std::string textLineTmp = rAna[iDep][iAna0].getRangeText(iRange);
        if (textLineTmp.size() > 0) textLinesTmp.push_back(textLineTmp);
    }

    setLatex(c, latex, iDep, textLinesTmp, leg);

    setCanvasFinal(c);
    c->Write("",TObject::kOverwrite);

    if (iObs == RECOANA::kFAKE) {

        // plot fake rate in log-scale as well
        int minTH1Dindex = getMinimumTH1Dindex(vecH1D, 0);
        int minBin = -1;
        if (minTH1Dindex > -1) minBin = getMinimumBin(vecH1D[minTH1Dindex], 0);
        double minContent = 0.001;
        if (minTH1Dindex > -1 && minBin > -1) minContent = vecH1D[minTH1Dindex]->GetBinContent(minBin);
        double logYmin = TMath::Floor(TMath::Log10(minContent));
        for (int i = 0; i < nBins; ++i) {
            vecH1D[i]->SetMinimum(TMath::Power(10, logYmin));
            vecH1D[i]->SetMaximum(40);
        }
        c->SetLogy(1);
        c->Update();
        canvasName = replaceAll(c->GetName(), "cnv_", "cnvLogy_");
        c->SetName(canvasName.c_str());
        c->Write("",TObject::kOverwrite);
    }

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

    int nBinsTot = nBins_eta + nBins_genPt + nBins_recoPt + nBins_cent + nBins_sumIso + nBins_sieie + nBins_r9;

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
    else if (nLegendTextSizes == RECOANA::kN_DEPS) legendTextSize = legendTextSizes.at(iLeg);
    if (legendTextSize != 0)  leg->SetTextSize(legendTextSize);

    int legendBorderSize = 0;
    if (nLegendBorderSizes == 1) legendBorderSize = legendBorderSizes.at(0);
    else if (nLegendBorderSizes == RECOANA::kN_DEPS) legendBorderSize = legendBorderSizes.at(iLeg);
    leg->SetBorderSize(legendBorderSize);

    float legendHeight = 0;
    if (nLegendHeights == 1) legendHeight = legendHeights.at(0);
    else if (nLegendHeights == RECOANA::kN_DEPS) legendHeight = legendHeights.at(iLeg);

    float legendWidth = 0;
    if (nLegendWidths == 1) legendWidth = legendWidths.at(0);
    else if (nLegendWidths == RECOANA::kN_DEPS) legendWidth = legendWidths.at(iLeg);

    double height = calcTLegendHeight(leg);
    double width = calcTLegendWidth(leg);
    if (legendHeight != 0)  height = legendHeight;
    if (legendWidth != 0)  width = legendWidth;

    std::string legendPosition = legendPositions.at(0).c_str();
    if (nLegendPositions == RECOANA::kN_DEPS)  legendPosition = legendPositions.at(iLeg).c_str();

    float legendOffsetX = 0;
    if (nLegendOffsetsX == 1) legendOffsetX = legendOffsetsX.at(0);
    else if (nLegendOffsetsX == RECOANA::kN_DEPS) legendOffsetX = legendOffsetsX.at(iLeg);

    float legendOffsetY = 0;
    if (nLegendOffsetsY == 1) legendOffsetY = legendOffsetsY.at(0);
    else if (nLegendOffsetsY == RECOANA::kN_DEPS) legendOffsetY = legendOffsetsY.at(iLeg);

    setLegendPosition(leg, legendPosition, pad, height, width, legendOffsetX, legendOffsetY, true);
}

void setLatex(TPad* pad, TLatex* latex, int iLatex, std::vector<std::string> textLines, TLegend* leg)
{
    std::string textPosition = "";
    if (nTextPositions == 1)  textPosition = textPositions.at(0);
    else if (nTextPositions == RECOANA::kN_DEPS)  textPosition = textPositions.at(iLatex);
    setTextAlignment(latex, textPosition);

    float textFont = textFonts.at(0);
    if (nTextFonts == RECOANA::kN_DEPS) textFont = textFonts.at(iLatex);
    latex->SetTextFont(textFont);

    float textSize = textSizes.at(0);
    if (nTextSizes == RECOANA::kN_DEPS) textSize = textSizes.at(iLatex);
    latex->SetTextSize(textSize);

    float textOffsetX = textOffsetsX.at(0);
    if (nTextOffsetsX == RECOANA::kN_DEPS) textOffsetX = textOffsetsX.at(iLatex);

    float textOffsetY = textOffsetsY.at(0);
    if (nTextOffsetsY == RECOANA::kN_DEPS) textOffsetY = textOffsetsY.at(iLatex);

    // update y offset if legend and latex will overlap
    if (leg != 0) {
        std::string legendPosition = legendPositions.at(0).c_str();
        if (nLegendPositions == RECOANA::kN_DEPS)  legendPosition = legendPositions.at(iLatex).c_str();

        if (textPosition.find("N") == 0 && textPosition == legendPosition) {
            textOffsetY += leg->GetY2NDC() - leg->GetY1NDC();
        }
    }

    drawTextLines(latex, pad, textLines, textPosition, textOffsetX, textOffsetY);
}

bool isNeutralMeson(int pdg)
{
    for (int i = 0; i < RECOANA::kN_NeutralMesons; ++i) {
        if (pdg == RECOANA::neutralMesons[i].PDG[0]) return true;
    }
    return false;
}

void copy2TmvaVars(ggHiNtuplizer& ggHi, int i, float *vals, std::vector<std::string>& tmvaVarNames, int nVars, int offset)
{
    for (int j = 0; j < nVars; ++j) {
        vals[j+offset] = ggHi.getValueByName(i, tmvaVarNames[j]);
    }
}

int findGenMatchedIndex(ggHiNtuplizer& ggHi, double recoEta, double recoPhi, double deltaR2, int genMatchedPID)
{
    double genPt = -1;
    int genMatchedIndex = -1;

    for (int iMC = 0; iMC < ggHi.nMC; ++iMC) {

        if (!(TMath::Abs((*ggHi.mcPID)[iMC]) == genMatchedPID || genMatchedPID == -777777))  continue;
        if ((*ggHi.mcStatus)[iMC] != 1)  continue;

        if (getDR2(recoEta, recoPhi, (*ggHi.mcEta)[iMC], (*ggHi.mcPhi)[iMC]) < deltaR2 &&
                (*ggHi.mcPt)[iMC] > genPt) {

            genMatchedIndex = iMC;
            genPt = (*ggHi.mcPt)[iMC];
        }
    }

    return genMatchedIndex;
}
