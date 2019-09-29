/*
 * macro to fill histograms of track spectra, to be used for residual track corrections
 */

#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TObjArray.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TMath.h>

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>

#include "../../CorrelationTuple/EventMatcher.h"
#include "../../TreeHeaders/CutConfigurationTree.h"
#include "../../TreeHeaders/hiEvtTree.h"
#include "../../TreeHeaders/skimAnalysisTree.h"
#include "../../TreeHeaders/trackTree.h"
#include "../../TreeHeaders/hiGenParticleTree.h"
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
#include "../../Utilities/vJetTrkUtil.h"
#include "../../Corrections/tracks/2017pp/trackingEfficiency2017pp.h"
#include "../../Corrections/tracks/2018PbPb/trackingEfficiency2018PbPb.h"

///// global variables
/// configuration variables

/// configuration variables - END

std::vector<std::string> argOptions;
///// global variables - END

void trkSpectra(std::string configFile, std::string inputFile, std::string outputFile = "trkSpectra.root");
void trkSpectraNoLoop(std::string configFile, std::string inputFile, std::string outputFile = "trkSpectra.root");

void trkSpectra(std::string configFile, std::string inputFile, std::string outputFile)
{
    std::cout<<"running trkSpectra()"<<std::endl;
    std::cout<<"configFile  = "<< configFile.c_str() <<std::endl;
    std::cout<<"inputFile   = "<< inputFile.c_str()  <<std::endl;
    std::cout<<"outputFile  = "<< outputFile.c_str() <<std::endl;

    std::string sampleType = (ArgumentParser::optionExists("--sampleType", argOptions)) ?
            ArgumentParser::ParseOptionInputSingle("--sampleType", argOptions).c_str() : "pbpb_2018_data";

    int applyTrkWeights = (ArgumentParser::optionExists("--applyTrkWeights", argOptions)) ?
            std::atoi(ArgumentParser::ParseOptionInputSingle("--applyTrkWeights", argOptions).c_str()) : 1;

    std::cout << "sampleType = " << sampleType << std::endl;
    std::cout << "applyTrkWeights = " << applyTrkWeights << std::endl;

    std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.c_str());
    std::cout<<"input ROOT files : num = "<<inputFiles.size()<< std::endl;
    std::cout<<"#####"<< std::endl;
    for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
        std::cout<<(*it).c_str()<< std::endl;
    }
    std::cout<<"##### END #####"<< std::endl;

    bool isMC = (toLowerCase(sampleType).find("_mc") != std::string::npos);
    bool isPbPb = (toLowerCase(sampleType).find("pbpb") != std::string::npos);
    bool isPP = (toLowerCase(sampleType).find("pp") != std::string::npos);

    bool isPbPb15 = isPbPb && (sampleType.find("2015") != std::string::npos);
    bool isPbPb18 = isPbPb && (sampleType.find("2018") != std::string::npos);
    bool isPP15 = isPP && (sampleType.find("2015") != std::string::npos);
    bool isPP17 = isPP && (sampleType.find("2017") != std::string::npos);

    std::cout << "isMC = " << isMC << std::endl;
    std::cout << "isPbPb15 = " << isPbPb15 << std::endl;
    std::cout << "isPbPb18 = " << isPbPb18 << std::endl;
    std::cout << "isPP15 = " << isPP15 << std::endl;
    std::cout << "isPP17 = " << isPP17 << std::endl;

    int collisionType = -1;
    if (isPbPb15) {
        collisionType = (isMC) ? COLL::TYPE::kHIMC : COLL::TYPE::kHI;
    }
    else if (isPbPb18) {
        collisionType = (isMC) ? COLL::TYPE::kHIMC2018 : COLL::TYPE::kHI2018;
    }
    else if (isPP15) {
        collisionType = (isMC) ? COLL::TYPE::kPPMC : COLL::TYPE::kPP;
    }
    else if (isPP17) {
        collisionType = (isMC) ? COLL::TYPE::kPPMC2017 : COLL::TYPE::kPP2017;
    }

    TrkEff2018PbPb trkEff2018 =  TrkEff2018PbPb("general", true, "Corrections/tracks/2018PbPb/");

    TrkEff2017pp trkEff2017 =  TrkEff2017pp(false, "Corrections/tracks/2017pp/");

    TFile* output = TFile::Open(outputFile.c_str(),"RECREATE");
    output->cd();

    // initialize objects
    TH1::SetDefaultSumw2();

    std::vector<TH1*> vec_h;

    std::vector<double> trkPts = getVecPt4TrkW();
    int nBinsPt = trkPts.size() - 1;

    std::vector<double> trkEtasMin = getVecEta4TrkW();
    int nBinsEta = trkEtasMin.size();
    double widthEta = (2.4*2) / nBinsEta;

    std::vector<int> hiBinsMin = getVecCent4TrkW();
    int nBinsCent = hiBinsMin.size();
    int widthCent = 200 / nBinsCent;

    TH1D* h_trkPhi[nBinsPt][nBinsEta][nBinsCent];

    enum RG {
        k_Gen,
        k_RecoEffCorr,
        k_RecoCorr,
        k_RecoUncorr,
        kN_RG,
    };

    std::string strRG[kN_RG] = {"gen", "trkEffCorr", "trkCorr", "trkUncorr"};

    std::vector<double> rgPts = getVecPt4TrkWCoarse();
    int nBinsPtRG = rgPts.size();

    std::vector<int> hiBinsMinRG = getVecCent4TrkWCoarse();
    int nBinsCentRG = hiBinsMinRG.size();

    TH2D* h2_phi_vs_eta[nBinsPtRG][nBinsCentRG][RG::kN_RG];

    int nBinsX_phi = 20;
    double xMax_phi = TMath::Pi()+1e-12;

    int nBinsX_eta = 24;
    double xMax_eta = 2.4;

    std::string text_trk = "trk";
    std::string text_trkPt = Form("p^{%s}_{T}", text_trk.c_str());
    std::string text_trkEta = Form("#eta^{%s}", text_trk.c_str());
    std::string text_trkPhi = Form("#phi^{%s}", text_trk.c_str());

    for (int i = 0; i < nBinsPt; ++i) {

        double trkPtMin = trkPts[i];
        double trkPtMax = trkPts[i+1];

        std::string text_range_trkPt = Form("%.2f < %s < %.2f", trkPtMin, text_trkPt.c_str(), trkPtMax);

        for (int j = 0; j < nBinsEta; ++j) {

            double trkEtaMin = trkEtasMin[j]-widthEta;
            double trkEtaMax = trkEtasMin[j];

            std::string text_range_trkEta = Form("%.1f < %s < %.1f", trkEtaMin, text_trkEta.c_str(), trkEtaMax);

            for (int k = 0; k < nBinsCent; ++k) {

                int hiBinMin = hiBinsMin[k]-widthCent;
                int hiBinMax = hiBinsMin[k];

                std::string text_range_hiBin = Form("hiBin:%d-%d", hiBinMin, hiBinMax);

                std::string title_h_suffix = Form("%s, %s, %s",
                                                            text_range_trkPt.c_str(),
                                                            text_range_trkEta.c_str(),
                                                            text_range_hiBin.c_str());

                std::string title_h_trkPhi = Form("%s;%s;", title_h_suffix.c_str(),
                                                            text_trkPhi.c_str());

                std::string name_h_suffix = Form("iPt_%d_iEta_%d_iCent_%d", i, j, k);
                std::string name_h_trkPhi = Form("h_trkPhi_%s", name_h_suffix.c_str());

                h_trkPhi[i][j][k] = 0;
                h_trkPhi[i][j][k] = new TH1D(name_h_trkPhi.c_str(), title_h_trkPhi.c_str(), nBinsX_phi, -1*xMax_phi, xMax_phi);

                h_trkPhi[i][j][k]->SetMarkerStyle(kFullCircle);

                vec_h.push_back(h_trkPhi[i][j][k]);
            }
        }
    }

    for (int i = 0; i < nBinsPtRG-1; ++i) {

        double trkPtMin = rgPts[i];
        double trkPtMax = rgPts[i+1];

        std::string text_range_trkPt = Form("%.2f < %s < %.2f", trkPtMin, text_trkPt.c_str(), trkPtMax);

        for (int k = 0; k < nBinsCentRG; ++k) {

            int hiBinMin = (k == 0) ? 0 : (hiBinsMinRG[k-1]);
            int hiBinMax = hiBinsMinRG[k];

            std::string text_range_hiBin = Form("hiBin:%d-%d", hiBinMin, hiBinMax);

            std::string title_h_suffix = Form("%s, %s", text_range_trkPt.c_str(),
                                                        text_range_hiBin.c_str());

            std::string title_h2_trkPhi_vs_trkEta = Form("%s;%s;%s", title_h_suffix.c_str(),
                                                        text_trkEta.c_str(),
                                                        text_trkPhi.c_str());

            std::string name_h_suffix = Form("iPt_%d_iCent_%d", i, k);

            for (int iRG = 0; iRG < RG::kN_RG; ++iRG) {

                std::string name_h2_phi_vs_eta = Form("h2_phi_vs_eta_%s_%s", strRG[iRG].c_str(), name_h_suffix.c_str());

                h2_phi_vs_eta[i][k][iRG] = 0;
                h2_phi_vs_eta[i][k][iRG] = new TH2D(name_h2_phi_vs_eta.c_str(), title_h2_trkPhi_vs_trkEta.c_str(),
                                                                                nBinsX_eta, -1*xMax_eta, xMax_eta,
                                                                                nBinsX_phi, -1*xMax_phi, xMax_phi);

                h2_phi_vs_eta[i][k][iRG]->SetMarkerStyle(kFullCircle);
                vec_h.push_back(h2_phi_vs_eta[i][k][iRG]);

            }
        }
    }

    TTree* treeHLT = 0;
    TTree* treeHiEvt = 0;
    TTree* treeSkim = 0;
    TTree* treeTrack = 0;
    TTree* treeHiGenParticle = 0;

    std::string treePathHLT = "hltanalysis/HltTree";
    std::string treePathHiEvt = "hiEvtAnalyzer/HiTree";
    std::string treePathSkimAna = "skimanalysis/HltTree";
    std::string treePathTrack = "ppTrack/trackTree";
    std::string treePathGen = "HiGenParticleAna/hi";

    std::vector<std::string> triggerBranches;
    //triggerBranches = {"HLT_HIL2Mu12_v1", "HLT_HIL3Mu12_v1"}; // "HLT_HIL1DoubleMu0_v1", "HLT_HIL1DoubleMu10_v1"
    int nTriggerBranches = triggerBranches.size();

    int nFiles = inputFiles.size();
    TFile* fileTmp = 0;

    std::cout << "initial reading to get the number of entries (if there is only one input file) and HiForest info" << std::endl;
    // read the first file only to get the HiForest info
    std::string inputPath = inputFiles.at(0).c_str();
    fileTmp = TFile::Open(inputPath.c_str(), "READ");
    fileTmp->cd();

    if (nFiles == 1) {
        // read one tree only to get the number of entries
        treeHiEvt = (TTree*)fileTmp->Get(treePathHiEvt.c_str());
        Long64_t entriesTmp = treeHiEvt->GetEntries();
        std::cout << "entries = " << entriesTmp << std::endl;
        treeHiEvt->Delete();
    }

    fileTmp->Close();
    // done with initial reading

    EventMatcher* em = new EventMatcher();
    Long64_t duplicateEntries = 0;

    Long64_t entries = 0;
    Long64_t entriesAnalyzed = 0;

    int nFilesSkipped = 0;
    std::cout<< "Loop : " << treePathHiEvt.c_str() <<std::endl;
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

        treeHLT = (TTree*)fileTmp->Get(treePathHLT.c_str());
        treeHLT->SetBranchStatus("*",0);     // disable all branches
        int triggerBits[nTriggerBranches];
        for (int iTrig = 0; iTrig < nTriggerBranches; ++iTrig) {
            if (treeHLT->GetBranch(triggerBranches[iTrig].c_str())) {
                treeHLT->SetBranchStatus(triggerBranches[iTrig].c_str(), 1);
                treeHLT->SetBranchAddress(triggerBranches[iTrig].c_str(), &(triggerBits[iTrig]));
            }
            else {
                triggerBits[iTrig] = 0;
            }
        }

        // specify explicitly which branches to use, do not use wildcard
        treeHiEvt = (TTree*)fileTmp->Get(treePathHiEvt.c_str());
        treeHiEvt->SetBranchStatus("*", 0);
        treeHiEvt->SetBranchStatus("hiBin", 1);
        treeHiEvt->SetBranchStatus("vz", 1);
        treeHiEvt->SetBranchStatus("run", 1);
        treeHiEvt->SetBranchStatus("lumi", 1);
        treeHiEvt->SetBranchStatus("evt", 1);

        treeSkim = (TTree*)fileTmp->Get(treePathSkimAna.c_str());
        treeSkim->SetBranchStatus("*", 0);

        treeTrack = (TTree*)fileTmp->Get(treePathTrack.c_str());
        treeTrack->SetBranchStatus("*", 0);     // disable all branches

        std::vector<std::string> trkBranches = {
                "nTrk",
                "trkPt",
                "trkEta",
                "trkPhi",
                "highPurity",
                "trkPtError",
                "trkDz1",
                "trkDzError1",
                "trkDxy1",
                "trkDxyError1",
                "trkNHit",
                "trkChi2",
                "trkNdof",
                "trkNlayer",
                "trkAlgo",
                "trkMVA",
                "pfHcal",
                "pfEcal",
        };

        int nTrkBranches = trkBranches.size();
        for (int iTrkBr = 0; iTrkBr < nTrkBranches; ++iTrkBr) {
            treeTrack->SetBranchStatus(trkBranches[iTrkBr].c_str(), 1);
        }

        if (isMC) {
            treeHiEvt->SetBranchStatus("weight", 1);
            treeHiEvt->SetBranchStatus("pthat",1);

            treeHiGenParticle = (TTree*)fileTmp->Get(treePathGen.c_str());
            treeHiGenParticle->SetBranchStatus("*", 0);     // disable all branches

            std::vector<std::string> genBranches = {
                    "mult",
                    "pt",
                    "eta",
                    "phi",
                    "chg",
            };

            int nGenBranches = genBranches.size();
            for (int iTrkBr = 0; iTrkBr < nGenBranches; ++iTrkBr) {
                treeHiGenParticle->SetBranchStatus(genBranches[iTrkBr].c_str(), 1);
            }
        }

        hiEvt hiEvt;
        hiEvt.setupTreeForReading(treeHiEvt);

        skimAnalysis skimAna;
        if (isPbPb15) skimAna.enableBranchesHI(treeSkim);
        else if (isPbPb18) skimAna.enableBranchesHI2018(treeSkim);
        else if (!isPbPb) skimAna.enableBranchesPP(treeSkim);
        skimAna.setupTreeForReading(treeSkim);
        skimAna.checkBranches(treeSkim);    // do the event selection if the branches exist.

        Tracks trks;
        trks.setupTreeForReading(treeTrack);

        hiGenParticle hiGen;
        if (isMC) {
            hiGen.setupTreeForReading(treeHiGenParticle);
        }

        Long64_t entriesTmp = treeHiEvt->GetEntries();
        entries += entriesTmp;
        std::cout << "entries in File = " << entriesTmp << std::endl;
        for (Long64_t j_entry = 0; j_entry < entriesTmp; ++j_entry)
        {
            if (j_entry % 2000 == 0)  {
              std::cout << "current entry = " <<j_entry<<" out of "<<entriesTmp<<" : "<<std::setprecision(2)<<(double)j_entry/entriesTmp*100<<" %"<<std::endl;
            }

            treeHiEvt->GetEntry(j_entry);
            // event selection
            if (!(std::fabs(hiEvt.vz) < 15))  continue;

            treeSkim->GetEntry(j_entry);
            if (isPbPb15 && !skimAna.passedEventSelectionHI())  continue;
            else if (isPbPb18 && !skimAna.passedEventSelectionHI2018())  continue;
            else if (!isPbPb && !skimAna.passedEventSelectionPP())  continue;

            treeHLT->GetEntry(j_entry);
            treeTrack->GetEntry(j_entry);
            if (isMC) {
                treeHiGenParticle->GetEntry(j_entry);
            }

            bool eventAdded = em->addEvent(hiEvt.run, hiEvt.lumi, hiEvt.evt, j_entry);
            if(!eventAdded) // this event is duplicate, skip this one.
            {
                duplicateEntries++;
                continue;
            }

            double w = 1;
            int hiBin = hiEvt.hiBin;
            if (isMC) {
                w = hiEvt.weight;
                double vertexWeight = 1;
                if (isPbPb18 && isMC)  {
                    vertexWeight = 1.19357*TMath::Exp(-0.5*TMath::Power((hiEvt.vz-(-22.3784))/39.7459, 2));
                }
                else if (isPbPb15 && isMC)  {
                    vertexWeight = 1.37487*TMath::Exp(-0.5*TMath::Power((hiEvt.vz-0.30709)/7.41379, 2));
                }
                else if (isPP17 && isMC)  {
                    vertexWeight = 1.0/(1.10749*TMath::Exp(-0.5*TMath::Power((hiEvt.vz-(-0.504278))/13.5601, 2)));
                }
                double centWeight = 1;
                if (isPbPb && isMC)  centWeight = findNcoll(hiBin);
                w *= vertexWeight * centWeight;
            }

            int iCent = getBinCent4TrkW(hiBin, hiBinsMin, nBinsCent);
            int iCentRG = getBinCent4TrkW(hiBin, hiBinsMinRG, nBinsCentRG);

            entriesAnalyzed++;

            for (int i = 0; i < trks.nTrk; ++i) {

                if (!passedTrkSelection(trks, i, collisionType))  continue;
                if (!(std::fabs(trks.trkEta[i]) < 2.4))  continue;

                double trkWeightTmp = 1;
                double trkWeightEffTmp = 1;
                if (applyTrkWeights > 0) {
                    if (isPP17) {
                        trkWeightTmp = trkEff2017.getCorrection(trks.trkPt[i], trks.trkEta[i]);
                    }
                    else if (isPbPb18) {
                        trkWeightTmp = trkEff2018.getCorrection(trks.trkPt[i], trks.trkEta[i], hiBin);
                        float effTmp = trkEff2018.getEfficiency(trks.trkPt[i], trks.trkEta[i], hiBin, true);
                        trkWeightEffTmp = (effTmp > 0.001) ? (1.0)/effTmp : 0;
                    }
                }

                double wTrk = w * trkWeightTmp;
                double wTrkEff = w * trkWeightEffTmp;

                int iTrkPt = getBinPt4TrkW(trks.trkPt[i], trkPts, nBinsPt);
                int iTrkEta = getBinEta4TrkW(trks.trkEta[i], trkEtasMin, nBinsEta);
                if (iTrkPt >= 0 && iTrkEta >= 0 && iCent >= 0) {
                    h_trkPhi[iTrkPt][iTrkEta][iCent]->Fill(trks.trkPhi[i], wTrk);
                }

                int iPtRG = getBinPt4TrkW(trks.trkPt[i], rgPts, nBinsPtRG);
                if (iPtRG >= 0 && iCentRG >= 0) {
                    h2_phi_vs_eta[iPtRG][iCentRG][RG::k_RecoEffCorr]->Fill(trks.trkEta[i], trks.trkPhi[i], wTrkEff);
                    h2_phi_vs_eta[iPtRG][iCentRG][RG::k_RecoCorr]->Fill(trks.trkEta[i], trks.trkPhi[i], wTrk);
                    h2_phi_vs_eta[iPtRG][iCentRG][RG::k_RecoUncorr]->Fill(trks.trkEta[i], trks.trkPhi[i], w);
                }
            }

            if (isMC) {
                for (int i = 0; i < hiGen.mult; ++i) {

                    if (!(std::fabs((*hiGen.eta)[i]) < 2.4))  continue;
                    if ( ((*hiGen.chg)[i] == 0) )  continue;

                    int iPtRG = getBinPt4TrkW((*hiGen.pt)[i], rgPts, nBinsPtRG);
                    if (iPtRG >= 0 && iCentRG >= 0) {
                        h2_phi_vs_eta[iPtRG][iCentRG][RG::k_Gen]->Fill((*hiGen.eta)[i], (*hiGen.phi)[i], w);
                    }
                }
            }
        }
        fileTmp->Close();
    }
    std::cout<<  "Loop ENDED : " << treePathHiEvt.c_str() <<std::endl;
    std::cout << "nFilesSkipped = " << nFilesSkipped << std::endl;
    std::cout << "entries            = " << entries << std::endl;
    std::cout << "duplicateEntries   = " << duplicateEntries << std::endl;
    std::cout << "entriesAnalyzed    = " << entriesAnalyzed << std::endl;

    output->cd();
    std::cout << "### post loop processing - START ###" << std::endl;

    std::cout << "### post loop processing - END ###" << std::endl;

    std::cout<<"Writing the output file."<<std::endl;
    output->Write("",TObject::kOverwrite);
    std::cout<<"Closing the output file."<<std::endl;
    output->Close();
    std::cout<<"running trkSpectra() - END"<<std::endl;
}

/*
 * run the macro without going through event loop, things done before and after the loop
 */
void trkSpectraNoLoop(std::string configFile, std::string inputFile, std::string outputFile)
{
    std::cout<<"running trkSpectra()"<<std::endl;
    std::cout<<"configFile  = "<< configFile.c_str() <<std::endl;
    std::cout<<"inputFile   = "<< inputFile.c_str()  <<std::endl;
    std::cout<<"outputFile  = "<< outputFile.c_str() <<std::endl;

    TFile* input = TFile::Open(inputFile.c_str(), "READ");

    TFile* output = TFile::Open(outputFile.c_str(),"RECREATE");
    output->cd();

    std::cout<<"Closing the input file."<<std::endl;
    input->Close();
    std::cout<<"Closing the output file."<<std::endl;
    output->Close();
    std::cout<<"running trkSpectra() - END"<<std::endl;
}

int main(int argc, char** argv)
{
    std::vector<std::string> argStr = ArgumentParser::ParseParameters(argc, argv);
    int nArgStr = argStr.size();

    argOptions = ArgumentParser::ParseOptions(argc, argv);
    bool noLoop = (findPositionInVector(argOptions, ARGUMENTPARSER::noLoop) >= 0);

    if (nArgStr == 4) {
        if (noLoop) trkSpectraNoLoop(argStr.at(1), argStr.at(2), argStr.at(3));
        else        trkSpectra(argStr.at(1), argStr.at(2), argStr.at(3));
        return 0;
    }
    else if (nArgStr == 3) {
        if (noLoop) trkSpectraNoLoop(argStr.at(1), argStr.at(2));
        else        trkSpectra(argStr.at(1), argStr.at(2));
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./Histogramming/vJetTrk/trkSpectra.exe <configFile> <inputFile> <outputFile>"
                << std::endl;
        return 1;
    }
}
