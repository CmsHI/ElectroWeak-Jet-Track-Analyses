/*
 * code to make a flat TTree skim of electron/photon, and event information
 */

#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TObjArray.h>
#include <TMath.h>
#include <TLorentzVector.h>
#include <TVector3.h>

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>

#include "../../CorrelationTuple/EventMatcher.h"
#include "../../TreeHeaders/ggHiNtuplizerTree.h"
#include "../../TreeHeaders/hiEvtTree.h"
#include "../../TreeHeaders/skimAnalysisTree.h"
#include "../../TreeHeaders/hiFJRhoTree.h"
#include "../../TreeHeaders/ggHiFlatTree.h"
#include "../../Utilities/interface/ArgumentParser.h"
#include "../../Utilities/interface/ConfigurationParser.h"
#include "../../Utilities/interface/InputConfigurationParser.h"
#include "../../Utilities/interface/HiForestInfoController.h"
#include "../../Utilities/eventUtil.h"
#include "../../Utilities/fileUtil.h"
#include "../../Utilities/egammaUtil.h"
#include "../../Utilities/mathUtil.h"
#include "../../Utilities/physicsUtil.h"

///// global variables
/// configuration variables

// TTree
std::string inputTreePath;
std::string outputTreePath;

int collisionType;

std::string collisionName;

// event cuts/weights
bool doWeightCent;
std::vector<std::vector<float>> pthatWeights;

std::string fileKinWeight;
std::string histKinWeight;

std::string fileGenKinWeight;
std::string histGenKinWeight;

bool calcPFIso; // WARNING : footprints cannot be removed in these functions as they are is kept in AOD only, but not in forest

bool calcIsoFlow;

bool calcRhoEtaAve;

// effective areas
std::vector<std::vector<float>> effAreaC;   // PF charged iso
std::vector<std::vector<float>> effAreaP;   // PF photon iso
std::vector<std::vector<float>> effAreaN;   // PF neutral iso
std::vector<std::vector<float>> effAreaHoE;   // for H/E
std::vector<std::vector<float>> effAreaSieie;   // for phoSigmaIEtaIEta_2012

// object cuts
float ptMin;
float ptMax;
float etaMin;
float etaMax;

int nPthatWeights;

int nEffAreaC;
int nEffAreaP;
int nEffAreaN;
int nEffAreaHoE;
int nEffAreaSieie;

/// configuration variables - END
enum INFILE_TYPES {
    kHiForest,
    kFlatTree,
    kN_INFILE_TYPES
};
const std::string inFileTypesStr[kN_INFILE_TYPES] = {"HiForest", "flattree"};
int inFileType;

enum RECOOBJS {
    kPhoton,
    kElectron,
    kMuon,
    kN_RECOOBJS
};
const std::string recoObjsStr[kN_RECOOBJS] = {"photon", "electron", "muon"};
int recoObj;

std::vector<std::string> argOptions;
///// global variables - END

int readConfiguration(std::string configFile, std::string inputFile);
void printConfiguration();
int parseInFileType(std::string inFileTypeStr);
int parseRecoObj(std::string recoObjStr);
double getEffArea(double eta, std::vector<float> &minEtas, std::vector<float> &maxEtas, std::vector<float> &effAreas, int nEffAreas);
void flatTreeSkim(std::string configFile, std::string inputFile, std::string outputFile = "flatTreeSkim.root");

void flatTreeSkim(std::string configFile, std::string inputFile, std::string outputFile)
{
    std::cout<<"running flatTreeSkim()"<<std::endl;
    std::cout<<"configFile  = "<< configFile.c_str() <<std::endl;
    std::cout<<"inputFile   = "<< inputFile.c_str()  <<std::endl;
    std::cout<<"outputFile  = "<< outputFile.c_str() <<std::endl;

    if (readConfiguration(configFile, inputFile) != 0)  return;
    printConfiguration();

    std::string skimMode = (ArgumentParser::optionExists("--mode", argOptions)) ?
            ArgumentParser::ParseOptionInputSingle("--mode", argOptions).c_str() : "";
    bool isPhoZee = (toLowerCase(skimMode) == "phozee");

    int nEvts = (ArgumentParser::optionExists("--nEvts", argOptions)) ?
                std::atoi(ArgumentParser::ParseOptionInputSingle("--nEvts", argOptions).c_str()) : -1;

    float phiFitEtaMin = (ArgumentParser::optionExists("--phiFitEtaMin", argOptions)) ?
                std::atof(ArgumentParser::ParseOptionInputSingle("--phiFitEtaMin", argOptions).c_str()) : 0;
    float phiFitEtaMax = (ArgumentParser::optionExists("--phiFitEtaMax", argOptions)) ?
                std::atof(ArgumentParser::ParseOptionInputSingle("--phiFitEtaMax", argOptions).c_str()) : -999999;

    int iEvtPlanePF = (ArgumentParser::optionExists("--iEvtPlanePF", argOptions)) ?
                    std::atoi(ArgumentParser::ParseOptionInputSingle("--iEvtPlanePF", argOptions).c_str()) : -1;
    /*
     * If >=0, then use event planes calculated by one of the following methods
     * https://github.com/CmsHI/cmssw/blob/358561bafa7aee76567aba78f80112562d840228/HeavyIonsAnalysis/PhotonAnalysis/interface/ggHiNtuplizer.h#L143-L149
     */

    int calcCSPFisoTmp = (ArgumentParser::optionExists("--calcCSPFiso", argOptions)) ?
                    std::atoi(ArgumentParser::ParseOptionInputSingle("--calcCSPFiso", argOptions).c_str()) : 0;
    bool calcCSPFiso = (calcCSPFisoTmp > 0);

    std::cout << "skimMode = " << skimMode << std::endl;
    std::cout << "nEvts = " << nEvts << std::endl;
    std::cout << "phiFitEtaMin = " << phiFitEtaMin << std::endl;
    std::cout << "phiFitEtaMax = " << phiFitEtaMax << std::endl;
    if (phiFitEtaMax < phiFitEtaMin) {
        std::cout << "Overwriting phiFitEta Min/Max to default values " << std::endl;
        phiFitEtaMin = -1.0;
        phiFitEtaMax = 1.0;
    }
    std::cout << "iEvtPlanePF = " << iEvtPlanePF << std::endl;
    std::cout << "calcCSPFiso = " << calcCSPFiso << std::endl;

    std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.c_str());
    std::cout<<"input ROOT files : num = "<<inputFiles.size()<< std::endl;
    std::cout<<"#####"<< std::endl;
    for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
        std::cout<<(*it).c_str()<< std::endl;
    }
    std::cout<<"##### END #####"<< std::endl;

    bool isMC = collisionIsMC((COLL::TYPE)collisionType);
    bool isHI15 = collisionIsHI((COLL::TYPE)collisionType);
    bool isHI18 = collisionIsHI2018((COLL::TYPE)collisionType);
    bool isHI = (isHI15 || isHI18);
    bool isPP = collisionIsPP((COLL::TYPE)collisionType);

    TFile* output = TFile::Open(outputFile.c_str(), "UPDATE");
    output->cd();

    TTree* outputTree = new TTree(outputTreePath.c_str(), Form("skim of %s", inputTreePath.c_str()));

    ggHiFlat ggHiOut;
    ggHiOut.doEle = (recoObj == RECOOBJS::kElectron);
    ggHiOut.doPho = (recoObj == RECOOBJS::kPhoton);
    ggHiOut.doMu = (recoObj == RECOOBJS::kMuon);
    ggHiOut.doMC = (recoObj == RECOOBJS::kPhoton && isMC);
    ggHiOut.setupTreeForWriting(outputTree);

    int nFiles = inputFiles.size();
    TFile* fileTmp = 0;
    std::cout << "initial reading to get the number of entries (if there is only one input file) and HiForest info" << std::endl;
    // read the first file only to get the HiForest info
    std::string inputPath = inputFiles.at(0).c_str();
    fileTmp = TFile::Open(inputPath.c_str(), "READ");
    fileTmp->cd();

    TTree* treeIn = 0;
    TTree* treeHiEvt = 0;
    TTree* treeHiForestInfo = 0;
    TTree* treeSkim = 0;
    TTree* treeHiFJRho = 0;
    TTree* treeTrack = 0;
    TTree* treePFCand = 0;
    TTree* treeCSPFCand = 0;

    if (nFiles == 1) {
        // read one tree only to get the number of entries
        treeIn = (TTree*)fileTmp->Get(inputTreePath.c_str());
        Long64_t entriesTmp = treeIn->GetEntries();
        std::cout << "entries = " << entriesTmp << std::endl;
        treeIn->Delete();
    }

    if (inFileType == INFILE_TYPES::kHiForest) {
        treeHiForestInfo = (TTree*)fileTmp->Get("HiForest/HiForestInfo");
        HiForestInfoController hfic(treeHiForestInfo);
        std::cout<<"### HiForestInfo Tree ###"<< std::endl;
        hfic.printHiForestInfo();
        std::cout<<"###"<< std::endl;
    }

    fileTmp->Close();
    // done with initial reading

    // kinematics weights
    fileTmp = 0;
    TH2D* h2D_weightKin = 0;
    std::string fileKinWeightPrsd = (fileKinWeight.size() > 0) ? (InputConfigurationParser::ParseFiles(fileKinWeight.c_str()))[0] : "";
    if (fileKinWeightPrsd.size() > 0 && histKinWeight.size() > 0) {
        fileTmp = TFile::Open(fileKinWeightPrsd.c_str(), "READ");
        h2D_weightKin = (TH2D*)fileTmp->Get(histKinWeight.c_str());
    }

    TH2D* h2D_weightGenKin = 0;
    std::string fileGenKinWeightPrsd = (fileGenKinWeight.size() > 0) ? (InputConfigurationParser::ParseFiles(fileGenKinWeight.c_str()))[0] : "";
    if (fileGenKinWeightPrsd.size() > 0 && histGenKinWeight.size() > 0) {
        fileTmp = TFile::Open(fileGenKinWeightPrsd.c_str(), "READ");
        h2D_weightGenKin = (TH2D*)fileTmp->Get(histGenKinWeight.c_str());
    }

    //double xRange_phi = -1;
    const int nDefnFlowMod = egUtil::defn_phi_flow_mod::N_defn_phi_flow_mod;
    std::vector<TH1D*> h1D_phi(nDefnFlowMod, 0);
    std::vector<TF1*> f1_phi_vn3(nDefnFlowMod, 0);
    std::vector<TF1*> f1_phi_vn2(nDefnFlowMod, 0);
    if (calcIsoFlow) {
        for (int j = 0; j < nDefnFlowMod; ++j) {
            h1D_phi[j] = new TH1D(Form("h1D_phi_%d", j), "Particle #phi distribution;#phi;Counts", 20, -1*TMath::Pi(), TMath::Pi());
            //xRange_phi = h1D_phi->GetXaxis()->GetXmax() - h1D_phi->GetXaxis()->GetXmin();

            f1_phi_vn3[j] = new TF1(Form("f1_phi_vn3_%d", j), fnc_fourier_vn_3, -1*TMath::Pi(), TMath::Pi(), getFncNpar(fnc_fourier_vn_3));
            f1_phi_vn2[j] = new TF1(Form("f1_phi_vn2_%d", j), fnc_fourier_vn_2, -1*TMath::Pi(), TMath::Pi(), getFncNpar(fnc_fourier_vn_2));
        }
    }

    EventMatcher* em = new EventMatcher();
    Long64_t duplicateEntries = 0;

    Long64_t entries = 0;
    Long64_t entriesAnalyzed = 0;
    Long64_t objectsSkimmed = 0;

    bool didNEvts = false;
    int nFilesSkipped = 0;
    std::cout<< "Loop : " << inputTreePath.c_str() <<std::endl;
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

        treeIn = (TTree*)fileTmp->Get(inputTreePath.c_str());
        treeIn->SetBranchStatus("*",0);     // disable all branches
        treeIn->SetBranchStatus("run",1);   // enable event information
        treeIn->SetBranchStatus("event",1);
        treeIn->SetBranchStatus("lumis",1);
        treeIn->SetBranchStatus("angEP*pf",1);
        if (recoObj == RECOOBJS::kPhoton) {
            treeIn->SetBranchStatus("nPho",1);     // enable photon branches
            treeIn->SetBranchStatus("pho*",1);     // enable photon branches
            treeIn->SetBranchStatus("pf*",1);     // enable photon branches
            treeIn->SetBranchStatus("rho",1);
            treeIn->SetBranchStatus("*EvtPlane",1);
            treeIn->SetBranchStatus("phi_*",1);
            treeIn->SetBranchStatus("nPhoPF",1);    // associated PF cands
            treeIn->SetBranchStatus("ppf*",1);
        }
        else if (recoObj == RECOOBJS::kElectron) {
            treeIn->SetBranchStatus("nEle",1);
            treeIn->SetBranchStatus("ele*",1);
            treeIn->SetBranchStatus("NClusters",1);
            treeIn->SetBranchStatus("NEcalClusters",1);
            treeIn->SetBranchStatus("rho",1);
        }
        else if (recoObj == RECOOBJS::kMuon) {
            treeIn->SetBranchStatus("nMu",1);
            treeIn->SetBranchStatus("mu*",1);
        }

        if (isMC) {
            treeIn->SetBranchStatus("nMC",1);     // enable GEN particle branches
            treeIn->SetBranchStatus("mc*",1);      // enable GEN particle branches
            // check existence of genMatching branch
            if (!treeIn->GetBranch("pho_genMatchedIndex")) {
                std::cout << "WARNING : Branch pho_genMatchedIndex does not exist." <<std::endl;
            }
        }

        if (inFileType == INFILE_TYPES::kFlatTree) {
            treeIn->SetBranchStatus("*",1);
        }

        // input type is forest
        ggHiNtuplizer ggHi;
        hiEvt hiEvt;
        skimAnalysis skimAna;
        hiFJRho hiFJRho;
        Tracks trks;
        pfCand pfs;
        pfCand cspfs;

        bool doPFCand = (calcPFIso || calcIsoFlow);
        bool doCSPFCand = calcCSPFiso;

        // input type is flatTree
        ggHiFlat ggFlat;

        if (inFileType == INFILE_TYPES::kHiForest) {

            // specify explicitly which branches to use, do not use wildcard
            treeHiEvt = (TTree*)fileTmp->Get("hiEvtAnalyzer/HiTree");
            treeHiEvt->SetBranchStatus("*",0);     // disable all branches
            treeHiEvt->SetBranchStatus("run",1);   // enable event information
            treeHiEvt->SetBranchStatus("evt",1);
            treeHiEvt->SetBranchStatus("lumi",1);
            treeHiEvt->SetBranchStatus("vz",1);
            treeHiEvt->SetBranchStatus("hiBin",1);
            treeHiEvt->SetBranchStatus("hiHF",1);
            treeHiEvt->SetBranchStatus("hiEvtPlanes",1);
            if (isMC) {
                treeHiEvt->SetBranchStatus("weight", 1);
                treeHiEvt->SetBranchStatus("phi0",1);
                treeHiEvt->SetBranchStatus("pthat",1);
                treeHiEvt->SetBranchStatus("ProcessID",1);
            }

            treeSkim = (TTree*)fileTmp->Get("skimanalysis/HltTree");
            treeSkim->SetBranchStatus("*",0);     // disable all branches

            treeHiFJRho = 0;
            if (calcRhoEtaAve) {
                treeHiFJRho = (TTree*)fileTmp->Get("hiFJRhoAnalyzer/t");
                if (treeHiFJRho != 0) {
                    treeHiFJRho->SetBranchStatus("*", 0);     // disable all branches
                    treeHiFJRho->SetBranchStatus("etaMin", 1);
                    treeHiFJRho->SetBranchStatus("etaMax", 1);
                    treeHiFJRho->SetBranchStatus("rho", 1);
                }
            }

            treeTrack = 0;
            treeTrack = (TTree*)fileTmp->Get("ppTrack/trackTree");
            if (treeTrack != 0) {
                treeTrack->SetBranchStatus("*", 0);     // disable all branches

                treeTrack->SetBranchStatus("nTrk",1);
                treeTrack->SetBranchStatus("trkPt",1);
                treeTrack->SetBranchStatus("trkEta",1);
                treeTrack->SetBranchStatus("trkPhi",1);
                treeTrack->SetBranchStatus("trkCharge",1);
                treeTrack->SetBranchStatus("highPurity",1);
                bool anaTrkID = true;
                if (anaTrkID) {
                    treeTrack->SetBranchStatus("trkPtError",1);
                    treeTrack->SetBranchStatus("trkDz1",1);
                    treeTrack->SetBranchStatus("trkDzError1",1);
                    treeTrack->SetBranchStatus("trkDxy1",1);
                    treeTrack->SetBranchStatus("trkDxyError1",1);
                    treeTrack->SetBranchStatus("trkNHit",1);
                    treeTrack->SetBranchStatus("trkChi2",1);
                    treeTrack->SetBranchStatus("trkNdof",1);
                    treeTrack->SetBranchStatus("trkNlayer",1);
                    treeTrack->SetBranchStatus("trkAlgo",1);
                    treeTrack->SetBranchStatus("trkMVA",1);
                }
                treeTrack->SetBranchStatus("pfType",1);
                treeTrack->SetBranchStatus("pfHcal",1);
                treeTrack->SetBranchStatus("pfEcal",1);

                if (doPFCand) {
                    treeTrack->SetBranchStatus("xVtx",1);
                    treeTrack->SetBranchStatus("yVtx",1);
                    treeTrack->SetBranchStatus("zVtx",1);
                }
            }

            if (doPFCand) {
                treePFCand = 0;
                treePFCand = (TTree*)fileTmp->Get("pfcandAnalyzer/pfTree");
                if (treePFCand != 0) {
                    treePFCand->SetBranchStatus("*", 0);     // disable all branches

                    treePFCand->SetBranchStatus("nPFpart",1);
                    treePFCand->SetBranchStatus("pfKey",1);
                    treePFCand->SetBranchStatus("pfId",1);
                    treePFCand->SetBranchStatus("pfPt",1);
                    treePFCand->SetBranchStatus("pfEta",1);
                    treePFCand->SetBranchStatus("pfPhi",1);
                    treePFCand->SetBranchStatus("pfv*",1);
                    treePFCand->SetBranchStatus("trk*",1);
                    treePFCand->SetBranchStatus("highPurity",1);
                    treePFCand->SetBranchStatus("pf*calE",1);
                }
            }

            if (doCSPFCand) {
                treeCSPFCand = 0;
                treeCSPFCand = (TTree*)fileTmp->Get("pfcandAnalyzerCS/pfTree");
                if (treeCSPFCand != 0) {
                    treeCSPFCand->SetBranchStatus("*", 0);     // disable all branches

                    treeCSPFCand->SetBranchStatus("nPFpart",1);
                    treeCSPFCand->SetBranchStatus("pfId",1);
                    treeCSPFCand->SetBranchStatus("pfPt",1);
                    treeCSPFCand->SetBranchStatus("pfEta",1);
                    treeCSPFCand->SetBranchStatus("pfPhi",1);
                }
            }

            ggHi.setupTreeForReading(treeIn);

            hiEvt.setupTreeForReading(treeHiEvt);

            if (isHI15) skimAna.enableBranchesHI(treeSkim);
            else if (isHI18) skimAna.enableBranchesHI2018(treeSkim);
            else if (isPP) skimAna.enableBranchesPP(treeSkim);
            skimAna.setupTreeForReading(treeSkim);
            skimAna.checkBranches(treeSkim);    // do the event selection if the branches exist.

            if (treeHiFJRho != 0) {
                hiFJRho.setupTreeForReading(treeHiFJRho);
            }

            trks.setupTreeForReading(treeTrack);

            if (doPFCand) {
                pfs.setupTreeForReading(treePFCand);
            }
            if (doCSPFCand) {
                cspfs.setupTreeForReading(treeCSPFCand);
            }
        }
        else if (inFileType == INFILE_TYPES::kFlatTree) {

            ggFlat.setupTreeForReading(treeIn);
        }

        Long64_t entriesProcessed = entries;
        Long64_t entriesTmp = treeIn->GetEntries();
        entries += entriesTmp;
        std::cout << "entries in File = " << entriesTmp << std::endl;
        for (Long64_t j_entry = 0; j_entry < entriesTmp; ++j_entry)
        {
            if (nEvts >= 0 && nEvts < j_entry + entriesProcessed - 1) {
                didNEvts = true;
                break;
            }
            if (j_entry % 2000 == 0)  {
                std::cout << "current entry = " <<j_entry<<" out of "<<entriesTmp<<" : "<<std::setprecision(5)<<(double)j_entry/entriesTmp*100<<" %"<<std::endl;
            }

            treeIn->GetEntry(j_entry);

            if (inFileType == INFILE_TYPES::kHiForest) {
                treeHiEvt->GetEntry(j_entry);
                treeSkim->GetEntry(j_entry);
                if (treeHiFJRho != 0) {
                    treeHiFJRho->GetEntry(j_entry);
                }
                treeTrack->GetEntry(j_entry);
                if (doPFCand) {
                    treePFCand->GetEntry(j_entry);
                }
                if (doCSPFCand) {
                    treeCSPFCand->GetEntry(j_entry);
                }

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
            }

            entriesAnalyzed++;

            ggHiOut.clearEntry();

            if (inFileType == INFILE_TYPES::kHiForest) {
                ggHiOut.weight = hiEvt.weight;
                ggHiOut.hiBin = hiEvt.hiBin;
                ggHiOut.hiHF = hiEvt.hiHF;
                ggHiOut.hiEP1HF = hiEvt.hiEvtPlanes[(int)(HiEP::HF1)];
                ggHiOut.hiEP2HF = hiEvt.hiEvtPlanes[(int)(HiEP::HF2)];
                ggHiOut.hiEP3HF = hiEvt.hiEvtPlanes[(int)(HiEP::HF3)];
                ggHiOut.hiEP4HF = hiEvt.hiEvtPlanes[(int)(HiEP::HF4)];
                if (isMC) {
                    ggHiOut.phi0 = hiEvt.phi0;
                    ggHiOut.pthat = hiEvt.pthat;
                    ggHiOut.processID = hiEvt.ProcessID;
                }

                ggHiOut.angEP2pf_out.clear();
                for (int i = 0; i < (int)(*ggHi.angEP2pf).size(); ++i) {
                    ggHiOut.angEP2pf_out.push_back( (*ggHi.angEP2pf)[i] );
                }
                ggHiOut.angEP3pf_out.clear();
                for (int i = 0; i < (int)(*ggHi.angEP3pf).size(); ++i) {
                    ggHiOut.angEP3pf_out.push_back( (*ggHi.angEP3pf)[i] );
                }

                double rho = -1;
                // calc eta-ave rho
                if (calcRhoEtaAve && treeHiFJRho != 0) {
                    rho = 0;
                    double totEta = 0;
                    int nEtaBins = hiFJRho.rho->size();
                    for (int i = 0; i < nEtaBins; ++i) {
                        double dEtaTmp = TMath::Abs((*hiFJRho.etaMax)[i] - (*hiFJRho.etaMin)[i]);
                        totEta += dEtaTmp;
                        rho += (*hiFJRho.rho)[i] * dEtaTmp;
                    }
                    rho = rho / totEta;
                }
                else if (ggHi.b_rho != 0) {
                    rho = ggHi.rho;
                }
                ggHiOut.rho = rho;

                ggHiOut.angEvtPlane = ggHi.angEvtPlane;
                ggHiOut.indexEvtPlane = ggHi.indexEvtPlane;
                ggHiOut.phi_nTot = ggHi.phi_nTot;
                ggHiOut.phi_minBinN = ggHi.phi_minBinN;
                ggHiOut.phi_fit_chi2 = ggHi.phi_fit_chi2;
                ggHiOut.phi_fit_chi2prob = ggHi.phi_fit_chi2prob;
                ggHiOut.phi_fit_v2 = ggHi.phi_fit_v2;

                ggHiOut.run = ggHi.run;
                ggHiOut.event = ggHi.event;
                ggHiOut.lumis = ggHi.lumis;

                ggHiOut.weightPthat = 1;
            }
            else if (inFileType == INFILE_TYPES::kFlatTree) {
                ggHiOut.clone(ggFlat);
            }

            ggHiOut.weightCent = (doWeightCent && isHI && isMC) ? findNcoll(ggHiOut.hiBin) : 1;
            ggHiOut.weightKin = 1;

            /*
                if (isHI && isMC)  vertexWeight = 1.37487*TMath::Exp(-0.5*TMath::Power((hiEvt.vz-0.30709)/7.41379, 2));  // 02.04.2016
             */

            if (nPthatWeights > 0) {
                for (int i = 0; i < nPthatWeights; ++i) {
                    if (ggHiOut.pthat >= pthatWeights[0][i] && ggHiOut.pthat < pthatWeights[1][i]) {
                        ggHiOut.weightPthat = pthatWeights[2][i];
                        break;
                    }
                }
            }

            const double angEP2 = ((iEvtPlanePF >= 0) ? ggHiOut.angEP2pf_out[iEvtPlanePF] : ggHiOut.hiEP2HF);
            const double angEP3 = ((iEvtPlanePF >= 0) ? ggHiOut.angEP3pf_out[iEvtPlanePF] : ggHiOut.hiEP3HF);
            if (calcIsoFlow) {
                for (int j = 0; j < nDefnFlowMod; ++j) {
                    h1D_phi[j]->Reset();
                }
                const float calcIso_pfPtMin = 0.3;
                const float calcIso_pfPtMax = 3.0;
                const float calcIso_pfEtaMin = phiFitEtaMin;
                const float calcIso_pfEtaMax = phiFitEtaMax;

                //
                /* Ref
                 * https://github.com/cms-sw/cmssw/blob/357be33285cdf679b02ac087b4644f0ce8a269fb/RecoHI/HiJetAlgos/plugins/HiFJRhoFlowModulationProducer.cc#L152-L153
                 */
                std::vector<double> ep2Cos = std::vector<double>(nDefnFlowMod, 0);
                std::vector<double> ep2Sin = std::vector<double>(nDefnFlowMod, 0);
                std::vector<double> ep3Cos = std::vector<double>(nDefnFlowMod, 0);
                std::vector<double> ep3Sin = std::vector<double>(nDefnFlowMod, 0);

                const int iLast4angEPcalc = egUtil::defn_phi_flow_mod::k_PF_all_trkID_wE; // last flow modulation definition for which event plane is calculated using PF cands
                // event plane angle calculation
                for (int i=0; i<pfs.nPFpart; ++i) {

                    float tId = (*pfs.pfId)[i];
                    if ((!((*pfs.pfPt)[i] > calcIso_pfPtMin)) && tId == 1) continue;    // apply pT threshold for ch PF cands only
                    if (!((*pfs.pfPt)[i] < calcIso_pfPtMax)) continue;
                    if (!(calcIso_pfEtaMin < (*pfs.pfEta)[i] && (*pfs.pfEta)[i] < calcIso_pfEtaMax) ) continue;
                    // checked in forest via Scan("Sum$(pfId == 1 && pfPt > 0.3 && pfPt < 3 && abs(pfEta) < 1):hiBin")

                    for (int j = 0; j <= iLast4angEPcalc; ++j) {

                        if (egUtil::phi_flow_mod_ch_flag[j] && (*pfs.pfId)[i] != 1 ) {
                            continue;
                        }
                        if (egUtil::phi_flow_mod_trkID_flag[j] && (*pfs.pfId)[i] == 1 &&
                            !passedTrkSelection(pfs, i, collisionType)) {
                            continue;
                        }

                        float tWeight = (egUtil::phi_flow_mod_wE[j]) ? (*pfs.pfPt)[i] : 1;
                        float tphi = (*pfs.pfPhi)[i];

                        h1D_phi[j]->Fill(tphi, tWeight);

                        ep2Cos[j] += (std::cos(2 * tphi)*tWeight);
                        ep2Sin[j] += (std::sin(2 * tphi)*tWeight);

                        ep3Cos[j] += (std::cos(3 * tphi)*tWeight);
                        ep3Sin[j] += (std::sin(3 * tphi)*tWeight);
                    }
                }

                ggHiOut.fMods_th1_yMin_out = std::vector<float>(nDefnFlowMod, 0);
                ggHiOut.fMods_angEP2_out = std::vector<float>(nDefnFlowMod, 0);
                ggHiOut.fMods_angEP3_out = std::vector<float>(nDefnFlowMod, 0);
                ggHiOut.fMods_v2_out = std::vector<float>(nDefnFlowMod, 0);
                ggHiOut.fMods_v3_out = std::vector<float>(nDefnFlowMod, 0);
                ggHiOut.fMods_chi2_out = std::vector<float>(nDefnFlowMod, 0);
                ggHiOut.fMods_chi2prob_out = std::vector<float>(nDefnFlowMod, 0);

                h1D_phi[egUtil::k_EPHF_PF_ch] = h1D_phi[egUtil::k_PF_ch];
                // fit PF candidate phi
                int nBinsX = -1;
                double initN0 = -1;
                for (int j = 0; j < nDefnFlowMod; ++j) {

                    ggHiOut.fMods_angEP2_out[j] = (j != egUtil::k_EPHF_PF_ch) ? (std::atan2(ep2Sin[j], ep2Cos[j]) / 2.) : ggHiOut.hiEP2HF;
                    ggHiOut.fMods_angEP3_out[j] = (j != egUtil::k_EPHF_PF_ch) ? (std::atan2(ep3Sin[j], ep3Cos[j]) / 3.) : ggHiOut.hiEP3HF;

                    nBinsX = h1D_phi[j]->GetNbinsX();
                    ggHiOut.fMods_th1_yMin_out[j] = 99999999999;
                    for (int iBin = 1; iBin <= nBinsX; ++iBin) {

                        double binContent = h1D_phi[j]->GetBinContent(iBin);
                        if (binContent < ggHiOut.fMods_th1_yMin_out[j]) {
                            ggHiOut.fMods_th1_yMin_out[j] = binContent;
                        }
                    }

                    //// vn_3
                    // set initial parameters assuming there is no flow, i.e. v2 = v3 = 0
                    initN0 = h1D_phi[j]->Integral() / nBinsX;
                    f1_phi_vn3[j]->SetParameter(0, initN0); // N0
                    f1_phi_vn3[j]->SetParameter(1, 0); // v2
                    f1_phi_vn3[j]->FixParameter(2, ggHiOut.fMods_angEP2_out[j]); // set to 2nd event plane angle
                    f1_phi_vn3[j]->SetParameter(3, 0); // v3
                    f1_phi_vn3[j]->FixParameter(4, ggHiOut.fMods_angEP3_out[j]); // set to 3rd event plane angle
                    h1D_phi[j]->Fit(f1_phi_vn3[j], "Q M R N");

                    ggHiOut.fMods_chi2_out[j] = f1_phi_vn3[j]->GetChisquare();
                    ggHiOut.fMods_chi2prob_out[j] = f1_phi_vn3[j]->GetProb();

                    ggHiOut.fMods_v2_out[j] = f1_phi_vn3[j]->GetParameter(1);
                    ggHiOut.fMods_v3_out[j] = f1_phi_vn3[j]->GetParameter(3);
                    if (ggHiOut.fMods_v2_out[j] < 0 || ggHiOut.fMods_v2_out[j] > 0.5) {
                        // https://arxiv.org/abs/1702.00630
                        //std::cout << "WARNING : Unexpected value extracted : fit_v2 = " << ggHiOut.fit_v2 << std::endl;
                    }
                }

                // fit using phi0 as EP angle
                if (isMC) {
                    int iEPgen = egUtil::defn_phi_flow_mod::k_PF_ch; // an arbitrary index that is free to use
                    nBinsX = h1D_phi[iEPgen]->GetNbinsX();
                    initN0 = h1D_phi[iEPgen]->Integral() / nBinsX;
                    f1_phi_vn2[iEPgen]->SetParameter(0, initN0); // N0
                    f1_phi_vn2[iEPgen]->SetParameter(1, 0); // v2
                    f1_phi_vn2[iEPgen]->FixParameter(2, ggHiOut.phi0); // set to 2nd event plane angle
                    h1D_phi[iEPgen]->Fit(f1_phi_vn2[iEPgen], "Q M R N");

                    ggHiOut.fit_EPphi0_chi2 = f1_phi_vn2[iEPgen]->GetChisquare();
                    ggHiOut.fit_EPphi0_chi2prob = f1_phi_vn2[iEPgen]->GetProb();

                    ggHiOut.fit_EPphi0_v2 = f1_phi_vn2[iEPgen]->GetParameter(1);
                }

                // fit track phi
                float ep2TrkCos = 0;
                float ep2TrkSin = 0;
                float ep3TrkCos = 0;
                float ep3TrkSin = 0;

                ggHiOut.fit_trkphi_v2 = 0;
                ggHiOut.fit_trkphi_v3 = 0;
                int iPhiTrk = 0; // an arbitrary index to use, since fitting PF cands is completed at this point
                h1D_phi[iPhiTrk]->Reset();
                for (int i=0; i<trks.nTrk; ++i) {

                    if (! (trks.trkPt[i] > calcIso_pfPtMin) ) continue;
                    if (! (trks.trkPt[i] < calcIso_pfPtMax) ) continue;
                    if (! (calcIso_pfEtaMin < trks.trkEta[i] && trks.trkEta[i] < calcIso_pfEtaMax) ) continue;
                    if (!passedTrkSelection(trks, i, collisionType))  continue;

                    double tphi = trks.trkPhi[i];

                    h1D_phi[iPhiTrk]->Fill(tphi);

                    ep2TrkCos += std::cos(2 * tphi);
                    ep2TrkSin += std::sin(2 * tphi);

                    ep3TrkCos += std::cos(3 * tphi);
                    ep3TrkSin += std::sin(3 * tphi);
                }

                ggHiOut.angEP2trk = std::atan2(ep2TrkSin, ep2TrkCos) / 2.;
                ggHiOut.angEP3trk = std::atan2(ep3TrkSin, ep3TrkCos) / 3.;

                //std::cout << "h1D_phi entries = " << h1D_phi->GetEntries() << std::endl;
                nBinsX = h1D_phi[iPhiTrk]->GetNbinsX();
                ggHiOut.trkphi_nBins = nBinsX;
                ggHiOut.trkphi_minContent = 99999999999;
                for (int iBin = 1; iBin <= nBinsX; ++iBin) {

                    double binContent = h1D_phi[iPhiTrk]->GetBinContent(iBin);
                    ggHiOut.trkphi_binContents_out.push_back(binContent);
                    if (binContent < ggHiOut.trkphi_minContent) {
                        ggHiOut.trkphi_minContent = binContent;
                    }
                }

                //// vn_3
                // set initial parameters assuming there is no flow, i.e. v2 = v3 = 0
                initN0 = h1D_phi[iPhiTrk]->Integral() / nBinsX;
                f1_phi_vn3[iPhiTrk]->SetParameter(0, initN0); // N0
                f1_phi_vn3[iPhiTrk]->SetParameter(1, 0); // v2
                f1_phi_vn3[iPhiTrk]->FixParameter(2, angEP2); // set to 2nd event plane angle
                f1_phi_vn3[iPhiTrk]->SetParameter(3, 0); // v3
                f1_phi_vn3[iPhiTrk]->FixParameter(4, angEP3); // set to 3rd event plane angle
                h1D_phi[iPhiTrk]->Fit(f1_phi_vn3[iPhiTrk], "Q M R N");

                ggHiOut.fit_trkphi_chi2 = f1_phi_vn3[iPhiTrk]->GetChisquare();
                ggHiOut.fit_trkphi_chi2prob = f1_phi_vn3[iPhiTrk]->GetProb();

                ggHiOut.fit_trkphi_v2 = f1_phi_vn3[iPhiTrk]->GetParameter(1);
                ggHiOut.fit_trkphi_v3 = f1_phi_vn3[iPhiTrk]->GetParameter(3);
            }

            if (recoObj == RECOOBJS::kPhoton) {

                if (inFileType == INFILE_TYPES::kHiForest) {
                    for (int i=0; i<ggHi.nPho; ++i) {

                        ggHiOut.clearEntryPho();
                        if (ggHiOut.doMC) {
                            ggHiOut.clearEntryGen();
                        }

                        if (!ggHi.passedPhoSpikeRejection(i)) continue;

                        if (!((*ggHi.phoEt)[i] >= ptMin))  continue;
                        if (ptMax > 0 && !((*ggHi.phoEt)[i] < ptMax))  continue;

                        if (!(TMath::Abs((*ggHi.phoEta)[i]) >= etaMin))  continue;
                        if (etaMax > 0 && !(TMath::Abs((*ggHi.phoEta)[i]) < etaMax))  continue;

                        ggHiOut.copyPho(ggHi, i);

                        if (isMC) {
                            int genMatchedIndex = (*ggHi.pho_genMatchedIndex)[i];
                            if (genMatchedIndex >= 0) {
                                ggHiOut.copyGen(ggHi, genMatchedIndex);
                            }
                        }

                        const double tR2 = 0.0;
                        const double tJW = 0.0;
                        const bool tEC = true;
                        const bool tDoTrkID = true;

                        ggHiOut.trkIso3 = getTrkIso(trks, ggHi, i, 0.3, tR2, 2.0, tJW, !(tDoTrkID), collisionType);
                        ggHiOut.trkIso3subUE = getTrkIsoSubUE(trks, ggHi, i, 0.3, tR2, 2.0, tJW, !(tDoTrkID), tEC, collisionType);
                        ggHiOut.trkIso3ID = getTrkIso(trks, ggHi, i, 0.3, tR2, 2.0, tJW, tDoTrkID, collisionType);
                        ggHiOut.trkIso3IDsubUE = getTrkIsoSubUE(trks, ggHi, i, 0.3, tR2, 2.0, tJW, tDoTrkID, tEC, collisionType);
                        ggHiOut.trkIso3IDmatchPF = getTrkIsoSubUE(trks, (*ggHi.phoEta)[i], (*ggHi.phoPhi)[i], 0.3, tR2, 2.0, tJW, tDoTrkID, tEC, collisionType);

                        TVector3 pVtx;
                        pVtx.SetZ(-999);
                        if (doPFCand) {
                            pVtx.SetX(trks.xVtx[0]);
                            pVtx.SetY(trks.yVtx[0]);
                            pVtx.SetZ(trks.zVtx[0]);
                        }
                        //const int modeFP = egUtil::footprintMode::matchKey;
                        const int modeFP = egUtil::footprintMode::matchKin;
                        int trkIDOpt = -1;
                        if (calcPFIso) {
                            ggHiOut.pfpIso3subUEcalc = getPFIsoSubUE(pfs, ggHi, i, 4, 0.3, tR2, 0.0, tJW, -1, modeFP, pVtx);
                            ggHiOut.pfnIso3subUEcalc = getPFIsoSubUE(pfs, ggHi, i, 5, 0.3, tR2, 0.0, tJW, -1, modeFP, pVtx);
                            ggHiOut.pfcIso3pTgt2p0subUEcalc = getPFIsoSubUE(pfs, ggHi, i, 1, 0.3, tR2, 2.0, tJW, -1, modeFP, pVtx);
                            ggHiOut.pfcIso3subUEcalc = getPFIsoSubUE(pfs, ggHi, i, 1, 0.3, tR2, 0.0, tJW, -1, modeFP, pVtx);
                            ggHiOut.pfcIso3IDpTgt2p0subUEcalc = getPFIsoSubUE(pfs, ggHi, i, 1, 0.3, tR2, 2.0, tJW, collisionType, modeFP, pVtx);
                            ggHiOut.pfcIso3IDsubUEcalc = getPFIsoSubUE(pfs, ggHi, i, 1, 0.3, tR2, 0.0, tJW, collisionType, modeFP, pVtx);

                            const int modeNoFP = egUtil::footprintMode::noRemoval;
                            ggHiOut.pfcIso3pTgt2p0subUEcalc = getPFIsoSubUE(pfs, ggHi, i, 1, 0.3, tR2, 2.0, tJW, -1, modeNoFP, pVtx);
                            ggHiOut.pfcIso3IDpTgt2p0subUEcalcNoFP = getPFIsoSubUE(pfs, ggHi, i, 1, 0.3, tR2, 2.0, tJW, collisionType, modeNoFP, pVtx);
                            ggHiOut.pfcIso3IDsubUEcalcNoFP = getPFIsoSubUE(pfs, ggHi, i, 1, 0.3, tR2, 0.0, tJW, collisionType, modeNoFP, pVtx);
                            ggHiOut.pfcIso3IDmatchTrk = getPFIsoSubUE(pfs, ggHi, i, 1, 0.3, tR2, 2.0, tJW, collisionType, modeNoFP, pVtx) +
                                                        getPFIsoSubUE(pfs, ggHi, i, 2, 0.3, tR2, 2.0, tJW, collisionType, modeNoFP, pVtx) +
                                                        getPFIsoSubUE(pfs, ggHi, i, 3, 0.3, tR2, 2.0, tJW, collisionType, modeNoFP, pVtx);
                        }
                        if (calcCSPFiso) {
                            TVector3 pVtxCS(0, 0, -999);
                            const int modeCSFP = egUtil::footprintMode::matchEtaPhi;
                            ggHiOut.pfCSpIso3subUEcalc = getPFIsoSubUE(cspfs, ggHi, i, 4, 0.3, tR2, 0.0, tJW, -1, modeCSFP, pVtxCS);
                            ggHiOut.pfCSnIso3subUEcalc = getPFIsoSubUE(cspfs, ggHi, i, 5, 0.3, tR2, 0.0, tJW, -1, modeCSFP, pVtxCS);
                            ggHiOut.pfCScIso3pTgt2p0subUEcalc = getPFIsoSubUE(cspfs, ggHi, i, 1, 0.3, tR2, 2.0, tJW, -1, modeCSFP, pVtxCS);
                            ggHiOut.pfCScIso3subUEcalc = getPFIsoSubUE(cspfs, ggHi, i, 1, 0.3, tR2, 0.0, tJW, -1, modeCSFP, pVtxCS);
                        }
                        if (calcIsoFlow) {
                            float angEPv2 = ggHiOut.angEvtPlane;
                            double only_vn_2 = ggHiOut.phi_fit_v2;

                            ggHiOut.pfpIso3subUEvn2 = getPFIsoSubUE(pfs, ggHi, i, 4, 0.3, tR2, 0.0, tJW, -1, modeFP, pVtx, only_vn_2, angEPv2);
                            ggHiOut.pfnIso3subUEvn2 = getPFIsoSubUE(pfs, ggHi, i, 5, 0.3, tR2, 0.0, tJW, -1, modeFP, pVtx, only_vn_2, angEPv2);
                            ggHiOut.pfcIso3pTgt2p0subUEvn2 = getPFIsoSubUE(pfs, ggHi, i, 1, 0.3, tR2, 2.0, tJW, trkIDOpt, modeFP, pVtx, only_vn_2, angEPv2);
                            ggHiOut.pfcIso3subUEvn2 = getPFIsoSubUE(pfs, ggHi, i, 1, 0.3, tR2, 0.0, tJW, trkIDOpt, modeFP, pVtx, only_vn_2, angEPv2);

                            float vn_2 = ggHiOut.fit_v2;
                            float vn_3 = 0; // ggHiOut.fit_v3;
                            float angEPv3 = angEP3;

                            ggHiOut.pfpIso3subUEfMods_out = std::vector<float>(nDefnFlowMod, 0);
                            ggHiOut.pfnIso3subUEfMods_out = std::vector<float>(nDefnFlowMod, 0);
                            ggHiOut.pfcIso3subUEfMods_out = std::vector<float>(nDefnFlowMod, 0);
                            ggHiOut.pfcIso3pTgt2p0subUEfMods_out = std::vector<float>(nDefnFlowMod, 0);

                            for (int jFMod = 0; jFMod < nDefnFlowMod; ++jFMod) {

                                vn_2 = ggHiOut.fMods_v2_out[jFMod];
                                angEPv2 = ggHiOut.fMods_angEP2_out[jFMod];
                                vn_3 = ggHiOut.fMods_v3_out[jFMod];
                                angEPv3 = ggHiOut.fMods_angEP3_out[jFMod];

                                trkIDOpt = (egUtil::phi_flow_mod_trkID_flag[jFMod]) ? collisionType : -1;

                                ggHiOut.pfpIso3subUEfMods_out[jFMod] = getPFIsoSubUE(pfs, ggHi, i, 4, 0.3, tR2, 0.0, tJW, -1, modeFP, pVtx, vn_2, angEPv2, vn_3, angEPv3);
                                ggHiOut.pfnIso3subUEfMods_out[jFMod] = getPFIsoSubUE(pfs, ggHi, i, 5, 0.3, tR2, 0.0, tJW, -1, modeFP, pVtx, vn_2, angEPv2, vn_3, angEPv3);
                                ggHiOut.pfcIso3subUEfMods_out[jFMod] = getPFIsoSubUE(pfs, ggHi, i, 1, 0.3, tR2, 0.0, tJW, trkIDOpt, modeFP, pVtx, vn_2, angEPv2, vn_3, angEPv3);
                                ggHiOut.pfcIso3pTgt2p0subUEfMods_out[jFMod] = getPFIsoSubUE(pfs, ggHi, i, 1, 0.3, tR2, 2.0, tJW, trkIDOpt, modeFP, pVtx, vn_2, angEPv2, vn_3, angEPv3);
                            }

                            trkIDOpt = -1;
                            if (isMC) {
                                ggHiOut.pfpIso3subUEphi0vn2 = getPFIsoSubUE(pfs, ggHi, i, 4, 0.3, tR2, 0.0, tJW, -1, modeFP, pVtx, ggHiOut.fit_EPphi0_v2, ggHiOut.phi0);
                                ggHiOut.pfnIso3subUEphi0vn2 = getPFIsoSubUE(pfs, ggHi, i, 5, 0.3, tR2, 0.0, tJW, -1, modeFP, pVtx, ggHiOut.fit_EPphi0_v2, ggHiOut.phi0);
                                ggHiOut.pfcIso3pTgt2p0subUEphi0vn2 = getPFIsoSubUE(pfs, ggHi, i, 1, 0.3, tR2, 2.0, tJW, trkIDOpt, modeFP, pVtx, ggHiOut.fit_EPphi0_v2, ggHiOut.phi0);
                                ggHiOut.pfcIso3subUEphi0vn2 = getPFIsoSubUE(pfs, ggHi, i, 1, 0.3, tR2, 0.0, tJW, trkIDOpt, modeFP, pVtx, ggHiOut.fit_EPphi0_v2, ggHiOut.phi0);
                            }

                            vn_2 = ggHiOut.fit_trkphi_v2;
                            vn_3 = ggHiOut.fit_trkphi_v3;
                            angEPv2 = (iEvtPlanePF >= 0) ? ggHiOut.angEP2trk : ggHiOut.hiEP2HF;
                            angEPv3 = (iEvtPlanePF >= 0) ? ggHiOut.angEP3trk : ggHiOut.hiEP3HF;
                            ggHiOut.pfpIso3subUEtrkvn3 = getPFIsoSubUE(pfs, ggHi, i, 4, 0.3, tR2, 0.0, tJW, -1, modeFP, pVtx, vn_2, angEPv2, vn_3, angEPv3);
                            ggHiOut.pfnIso3subUEtrkvn3 = getPFIsoSubUE(pfs, ggHi, i, 5, 0.3, tR2, 0.0, tJW, -1, modeFP, pVtx, vn_2, angEPv2, vn_3, angEPv3);
                            ggHiOut.pfcIso3pTgt2p0subUEtrkvn3 = getPFIsoSubUE(pfs, ggHi, i, 1, 0.3, tR2, 2.0, tJW, trkIDOpt, modeFP, pVtx, vn_2, angEPv2, vn_3, angEPv3);
                            ggHiOut.pfcIso3subUEtrkvn3 = getPFIsoSubUE(pfs, ggHi, i, 1, 0.3, tR2, 0.0, tJW, trkIDOpt, modeFP, pVtx, vn_2, angEPv2, vn_3, angEPv3);
                        }

                        ggHiOut.phoEAc = getEffArea((*ggHi.phoSCEta)[i], effAreaC[0], effAreaC[1], effAreaC[2], nEffAreaC);
                        ggHiOut.phoEAp = getEffArea((*ggHi.phoSCEta)[i], effAreaP[0], effAreaP[1], effAreaP[2], nEffAreaP);
                        ggHiOut.phoEAn = getEffArea((*ggHi.phoSCEta)[i], effAreaN[0], effAreaN[1], effAreaN[2], nEffAreaN);
                        ggHiOut.phoEAhoe = getEffArea((*ggHi.phoSCEta)[i], effAreaHoE[0], effAreaHoE[1], effAreaHoE[2], nEffAreaHoE);
                        ggHiOut.phoEAsieie = getEffArea((*ggHi.phoSCEta)[i], effAreaSieie[0], effAreaSieie[1], effAreaSieie[2], nEffAreaSieie);

                        const std::vector<basicPFCand> phoLinkedPFs = getLinkedPFCands(ggHi, i);
                        if (phoLinkedPFs.size() > 0) {
                            ggHiOut.resetPhoPF();

                            float wpfp = 0;
                            float wpfn = 0;
                            float wpfc = 0;
                            float wpfx = 0;
                            for (std::vector<basicPFCand>::const_iterator itPF = phoLinkedPFs.begin(); itPF != phoLinkedPFs.end(); ++itPF) {
                                float wTmp = (*itPF).pt;
                                if ((*itPF).id == 4) {
                                    ggHiOut.ppfpPtSum += (*itPF).pt;
                                    ggHiOut.ppfpEtaAve += ((*itPF).eta * wTmp);
                                    ggHiOut.ppfpPhiAve += ((*itPF).phi * wTmp);
                                    wpfp += wTmp;
                                    ggHiOut.nPhoPFp++;
                                }
                                else if ((*itPF).id == 5) {
                                    ggHiOut.ppfnPtSum += (*itPF).pt;
                                    ggHiOut.ppfnEtaAve += ((*itPF).eta * wTmp);
                                    ggHiOut.ppfnPhiAve += ((*itPF).phi * wTmp);
                                    wpfn += wTmp;
                                    ggHiOut.nPhoPFn++;
                                }
                                else if ((*itPF).id == 1) {
                                    ggHiOut.ppfcPtSum += (*itPF).pt;
                                    ggHiOut.ppfcEtaAve += ((*itPF).eta * wTmp);
                                    ggHiOut.ppfcPhiAve += ((*itPF).phi * wTmp);
                                    wpfc += wTmp;
                                    ggHiOut.nPhoPFc++;
                                }
                                else {
                                    ggHiOut.ppfxPtSum += (*itPF).pt;
                                    ggHiOut.ppfxEtaAve += ((*itPF).eta * wTmp);
                                    ggHiOut.ppfxPhiAve += ((*itPF).phi * wTmp);
                                    wpfx += wTmp;
                                    ggHiOut.nPhoPFx++;
                                }
                            }

                            ggHiOut.ppfpEtaAve /= wpfp;
                            ggHiOut.ppfpPhiAve /= wpfp;

                            ggHiOut.ppfnEtaAve /= wpfn;
                            ggHiOut.ppfnPhiAve /= wpfn;

                            ggHiOut.ppfcEtaAve /= wpfc;
                            ggHiOut.ppfcPhiAve /= wpfc;

                            ggHiOut.ppfxEtaAve /= wpfx;
                            ggHiOut.ppfxPhiAve /= wpfx;
                        }

                        if (h2D_weightKin != 0) {
                            int binTmp = h2D_weightKin->FindBin((*ggHi.phoEt)[i], (*ggHi.phoEta)[i]);
                            ggHiOut.weightKin = h2D_weightKin->GetBinContent(binTmp);
                        }

                        if (h2D_weightGenKin != 0 && (ggHiOut.mcPID == 22 || (isPhoZee && TMath::Abs(ggHiOut.mcPID) == 11))) {
                            int binTmp = h2D_weightGenKin->FindBin(ggHiOut.mcPt, ggHiOut.mcEta);
                            ggHiOut.weightGenKin = h2D_weightGenKin->GetBinContent(binTmp);
                        }

                        outputTree->Fill();
                        objectsSkimmed++;
                    }
                }
                else if (inFileType == INFILE_TYPES::kFlatTree) {

                    if (!(ggHiOut.phoEt >= ptMin))  continue;
                    if (ptMax > 0 && !(ggHiOut.phoEt < ptMax))  continue;

                    if (!(TMath::Abs(ggHiOut.phoEta) >= etaMin))  continue;
                    if (etaMax > 0 && !(TMath::Abs(ggHiOut.phoEta) < etaMax))  continue;

                    if (h2D_weightKin != 0) {
                        int binTmp = h2D_weightKin->FindBin(ggHiOut.phoEt, ggHiOut.phoEta);
                        ggHiOut.weightKin = h2D_weightKin->GetBinContent(binTmp);
                    }

                    if (h2D_weightGenKin != 0 && (ggHiOut.mcPID == 22 || (isPhoZee && TMath::Abs(ggHiOut.mcPID) == 11))) {
                        int binTmp = h2D_weightGenKin->FindBin(ggHiOut.mcPt, ggHiOut.mcEta);
                        ggHiOut.weightGenKin = h2D_weightGenKin->GetBinContent(binTmp);
                    }

                    outputTree->Fill();
                    objectsSkimmed++;
                }
            }
            else if (recoObj == RECOOBJS::kElectron) {
                for (int i=0; i<ggHi.nEle; ++i) {

                    ggHiOut.clearEntryEle();
                    if (ggHiOut.doMC) {
                        ggHiOut.clearEntryGen();
                    }

                    if (!((*ggHi.elePt)[i] >= ptMin))  continue;
                    if (ptMax > 0 && !((*ggHi.elePt)[i] < ptMax))  continue;

                    if (!(TMath::Abs((*ggHi.eleEta)[i]) >= etaMin))  continue;
                    if (etaMax > 0 && !(TMath::Abs((*ggHi.eleEta)[i]) < etaMax))  continue;

                    ggHiOut.copyEle(ggHi, i);

                    outputTree->Fill();
                    objectsSkimmed++;
                }
            }
            else if (recoObj == RECOOBJS::kMuon) {
                for (int i=0; i<ggHi.nMu; ++i) {

                    ggHiOut.clearEntryMu();
                    if (ggHiOut.doMC) {
                        ggHiOut.clearEntryGen();
                    }

                    if (!((*ggHi.muPt)[i] >= ptMin))  continue;
                    if (ptMax > 0 && !((*ggHi.muPt)[i] < ptMax))  continue;

                    if (!(TMath::Abs((*ggHi.muEta)[i]) >= etaMin))  continue;
                    if (etaMax > 0 && !(TMath::Abs((*ggHi.muEta)[i]) < etaMax))  continue;

                    ggHiOut.copyMu(ggHi, i);

                    outputTree->Fill();
                    objectsSkimmed++;
                }
            }
        }
        fileTmp->Close();
        if (didNEvts) {
            break;
        }
    }
    std::cout<<  "Loop ENDED : " << inputTreePath.c_str() <<std::endl;
    std::cout << "nFilesSkipped = " << nFilesSkipped << std::endl;
    std::cout << "entries            = " << entries << std::endl;
    std::cout << "duplicateEntries   = " << duplicateEntries << std::endl;
    std::cout << "entriesAnalyzed    = " << entriesAnalyzed << std::endl;
    std::cout << "objectsSkimmed    = " << objectsSkimmed << std::endl;

    // overwrite existing trees
    output->cd();
    outputTree->Write("", TObject::kOverwrite);

    std::cout<<"Writing the output file."<<std::endl;
    output->Write("",TObject::kOverwrite);
    std::cout<<"Closing the output file."<<std::endl;
    output->Close();
    std::cout<<"running flatTreeSkim() - END"<<std::endl;
}

int main(int argc, char** argv)
{
    std::vector<std::string> argStr = ArgumentParser::ParseParameters(argc, argv);
    int nArgStr = argStr.size();

    argOptions = ArgumentParser::ParseOptions(argc, argv);

    if (nArgStr == 4) {
        flatTreeSkim(argStr.at(1), argStr.at(2), argStr.at(3));
        return 0;
    }
    else if (nArgStr == 3) {
        flatTreeSkim(argStr.at(1), argStr.at(2));
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./flatTreeSkim.exe <configFile> <inputFile> <outputFile>"
                << std::endl;
        return 1;
    }
}

int readConfiguration(std::string configFile, std::string inputFile)
{
    ConfigurationParser confParser;
    confParser.openConfigFile(configFile);

    confParser.parsedKeyWords = InputConfigurationParser::parseKeyWords(inputFile);

    inFileType = parseInFileType(confParser.ReadConfigValue("inFileType"));

    recoObj = parseRecoObj(confParser.ReadConfigValue("recoObj"));

    // TTree
    inputTreePath = confParser.ReadConfigValue("inputTreePath");
    outputTreePath = confParser.ReadConfigValue("outputTreePath");

    collisionType = confParser.ReadConfigValueInteger("collisionType");

    // event cuts/weights
    doWeightCent = (confParser.ReadConfigValueInteger("doWeightCent") > 0);
    pthatWeights = ConfigurationParser::ParseListTriplet(confParser.ReadConfigValue("pthatWeights"));

    fileKinWeight = confParser.ReadConfigValue("fileKinWeight");
    histKinWeight = confParser.ReadConfigValue("histKinWeight");

    fileGenKinWeight = confParser.ReadConfigValue("fileGenKinWeight");
    histGenKinWeight = confParser.ReadConfigValue("histGenKinWeight");

    calcPFIso = (confParser.ReadConfigValueInteger("calcPFIso") > 0);

    calcIsoFlow = (confParser.ReadConfigValueInteger("calcIsoFlow") > 0);
    if (calcIsoFlow && inFileType != INFILE_TYPES::kHiForest) {
        std::cout << "WARNING : calcIsoFlow is set to 1. But, isolation can be calculated only if input is HiForest. Disabling calcIsoFlow" << std::endl;
        calcIsoFlow = false;
    }

    calcRhoEtaAve = (confParser.ReadConfigValueInteger("calcRhoEtaAve") > 0);

    effAreaC = ConfigurationParser::ParseListTriplet(confParser.ReadConfigValue("effAreaC"));
    effAreaP = ConfigurationParser::ParseListTriplet(confParser.ReadConfigValue("effAreaP"));
    effAreaN = ConfigurationParser::ParseListTriplet(confParser.ReadConfigValue("effAreaN"));
    effAreaHoE = ConfigurationParser::ParseListTriplet(confParser.ReadConfigValue("effAreaHoE"));
    effAreaSieie = ConfigurationParser::ParseListTriplet(confParser.ReadConfigValue("effAreaSieie"));

    // object cuts
    ptMin = confParser.ReadConfigValueFloat("ptMin");
    ptMax = confParser.ReadConfigValueFloat("ptMax");
    etaMin = confParser.ReadConfigValueFloat("etaMin");
    etaMax = confParser.ReadConfigValueFloat("etaMax");

    nPthatWeights = pthatWeights[0].size();

    nEffAreaC = effAreaC[0].size();
    nEffAreaP = effAreaP[0].size();
    nEffAreaN = effAreaN[0].size();
    nEffAreaHoE = effAreaHoE[0].size();
    nEffAreaSieie = effAreaSieie[0].size();

    // set default values
    if (inputTreePath.size() == 0) inputTreePath = "ggHiNtuplizer/EventTree";
    if (outputTreePath.size() == 0) outputTreePath = "treeSignal";

    if (ptMax == 0)  ptMax = -1;
    if (etaMax == 0)  etaMax = -1;

    collisionName = getCollisionTypeName((COLL::TYPE)collisionType).c_str();

    return 0;
}

/*
 * print information read from input/cut configurations
 * assumes that readConfiguration() is run before
 */
void printConfiguration()
{
    std::cout<<"Configuration :"<<std::endl;
    std::cout << "input file type = " << inFileTypesStr[inFileType].c_str() << std::endl;
    if (inFileType < 0 || inFileType >= INFILE_TYPES::kN_INFILE_TYPES) {
        std::cout << "ERROR : no valid input file type given" << std::endl;
        std::cout << "inFileType (index for input file type) = " << inFileType << std::endl;
    }
    std::cout << "reco object = " << recoObjsStr[recoObj].c_str() << std::endl;
    if (recoObj < 0 || recoObj >= RECOOBJS::kN_RECOOBJS) {
        std::cout << "ERROR : no valid reco object given" << std::endl;
        std::cout << "recoObj (index for reco object) = " << recoObj << std::endl;
    }

    std::cout << "inputTreePath = " << inputTreePath.c_str() << std::endl;
    std::cout << "outputTreePath = " << outputTreePath.c_str() << std::endl;

    std::cout << "collision = " << collisionName.c_str() << std::endl;

    std::cout << "doWeightCent = " << doWeightCent << std::endl;

    std::cout << "nPthatWeights = " << nPthatWeights << std::endl;
    for (int i = 0; i < nPthatWeights; ++i) {
        std::cout << Form("pthatWeights[%d] = { ", i);
        std::cout << Form("%.0f, ", pthatWeights[0].at(i));
        std::cout << Form("%f, ", pthatWeights[1].at(i));
        std::cout << Form("%f }", pthatWeights[2].at(i)) << std::endl;;
    }

    std::cout << "fileKinWeight = " << fileKinWeight.c_str() << std::endl;
    std::cout << "histKinWeight = " << histKinWeight.c_str() << std::endl;

    std::cout << "fileGenKinWeight = " << fileGenKinWeight.c_str() << std::endl;
    std::cout << "histGenKinWeight = " << histGenKinWeight.c_str() << std::endl;

    std::cout << "calcPFIso = " << calcPFIso << std::endl;

    std::cout << "calcIsoFlow = " << calcIsoFlow << std::endl;

    std::cout << "calcRhoEtaAve = " << calcRhoEtaAve << std::endl;

    std::cout << "nEffAreaC = " << nEffAreaC << std::endl;
    for (int i = 0; i < nEffAreaC; ++i) {
        std::cout << Form("effAreaC[%d] = { ", i);
        std::cout << Form("%f, ", effAreaC[0].at(i));
        std::cout << Form("%f, ", effAreaC[1].at(i));
        std::cout << Form("%f }", effAreaC[2].at(i)) << std::endl;;
    }

    std::cout << "nEffAreaP = " << nEffAreaP << std::endl;
    for (int i = 0; i < nEffAreaP; ++i) {
        std::cout << Form("effAreaP[%d] = { ", i);
        std::cout << Form("%f, ", effAreaP[0].at(i));
        std::cout << Form("%f, ", effAreaP[1].at(i));
        std::cout << Form("%f }", effAreaP[2].at(i)) << std::endl;;
    }

    std::cout << "nEffAreaN = " << nEffAreaN << std::endl;
    for (int i = 0; i < nEffAreaN; ++i) {
        std::cout << Form("effAreaN[%d] = { ", i);
        std::cout << Form("%f, ", effAreaN[0].at(i));
        std::cout << Form("%f, ", effAreaN[1].at(i));
        std::cout << Form("%f }", effAreaN[2].at(i)) << std::endl;;
    }

    std::cout << "nEffAreaHoE = " << nEffAreaHoE << std::endl;
    for (int i = 0; i < nEffAreaHoE; ++i) {
        std::cout << Form("effAreaHoE[%d] = { ", i);
        std::cout << Form("%f, ", effAreaHoE[0].at(i));
        std::cout << Form("%f, ", effAreaHoE[1].at(i));
        std::cout << Form("%f }", effAreaHoE[2].at(i)) << std::endl;;
    }

    std::cout << "nEffAreaSieie = " << nEffAreaSieie << std::endl;
    for (int i = 0; i < nEffAreaSieie; ++i) {
        std::cout << Form("effAreaSieie[%d] = { ", i);
        std::cout << Form("%f, ", effAreaSieie[0].at(i));
        std::cout << Form("%f, ", effAreaSieie[1].at(i));
        std::cout << Form("%f }", effAreaSieie[2].at(i)) << std::endl;;
    }

    std::cout << "ptMin = " << ptMin << std::endl;
    std::cout << "ptMax = " << ptMax << std::endl;
    std::cout << "etaMin = " << etaMin << std::endl;
    std::cout << "etaMax = " << etaMax << std::endl;
}

int parseInFileType(std::string inFileTypeStr)
{
    inFileTypeStr = trim(inFileTypeStr);
    inFileTypeStr = toLowerCase(inFileTypeStr);

    if (inFileTypeStr.find("forest") != std::string::npos || inFileTypeStr == "0") {
        return INFILE_TYPES::kHiForest;
    }
    else if (inFileTypeStr.find("flat") != std::string::npos || inFileTypeStr == "1") {
        return INFILE_TYPES::kFlatTree;
    }
    else {
        return -1;
    }
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
    else if (recoObjStr == "mu" || recoObjStr == "muon" || recoObjStr == "2") {
        return RECOOBJS::kMuon;
    }
    else {
        return -1;
    }
}

double getEffArea(double eta, std::vector<float> &minEtas, std::vector<float> &maxEtas, std::vector<float> &effAreas, int nEffAreas)
{
    for (int i = 0; i < nEffAreas; ++i) {
        if (TMath::Abs(eta) >= minEtas[i] && TMath::Abs(eta) < maxEtas[i]) {
            return effAreas[i];
        }
    }

    return 0;
}

