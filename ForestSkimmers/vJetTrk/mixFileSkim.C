/*
 * macro to skim HiForest containing mixed events, create trees with pre-processed info
 * Output is the "mixFile" input for mixFileSkim.C
 */

#include <TFile.h>
#include <TTree.h>
#include <TObjArray.h>
#include <TMath.h>
#include <TLorentzVector.h>
#include <TRandom3.h>

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>

#include "../../TreeHeaders/CutConfigurationTree.h"
#include "../../TreeHeaders/ggHiNtuplizerTree.h"
#include "../../TreeHeaders/hiEvtTree.h"
#include "../../TreeHeaders/JetTree.h"
#include "../../TreeHeaders/trackTree.h"
#include "../../TreeHeaders/pfCandTree.h"
#include "../../TreeHeaders/hiGenParticleTree.h"
#include "../../TreeHeaders/skimAnalysisTree.h"
#include "../../TreeHeaders/eventSkimTree.h"
#include "../../Utilities/interface/ArgumentParser.h"
#include "../../Utilities/interface/ConfigurationParser.h"
#include "../../Utilities/interface/GraphicsConfigurationParser.h"
#include "../../Utilities/interface/InputConfigurationParser.h"
#include "../../Utilities/interface/HiForestInfoController.h"
#include "../../Utilities/eventUtil.h"
#include "../../Utilities/fileUtil.h"
#include "../../Utilities/physicsUtil.h"
#include "../../Utilities/vJetTrkUtil.h"

///// global variables
/// configuration variables

// input for TTree
std::string sampleType;

std::vector<std::string> jetCollections;

int nJetCollections;

/// configuration variables - END

std::vector<std::string> argOptions;
///// global variables - END

const long MAXTREESIZE = 100000000000; // set maximum tree size from 10 GB to 100 GB, so that the code does not switch to a new file after 10 GB

int readConfiguration(std::string configFile, std::string inputFile);
void printConfiguration();
void mixFileSkim(std::string configFile, std::string inputFile, std::string outputFile = "mixFileSkim.root");

void mixFileSkim(std::string configFile, std::string inputFile, std::string outputFile)
{
    std::cout<<"running mixFileSkim()"<<std::endl;
    std::cout<<"configFile  = "<< configFile.c_str() <<std::endl;
    std::cout<<"inputFile   = "<< inputFile.c_str()  <<std::endl;
    std::cout<<"outputFile  = "<< outputFile.c_str() <<std::endl;

    if (readConfiguration(configFile, inputFile) != 0)  return;
    printConfiguration();

    bool isMC = isMCsample(sampleType);
    bool isPbPb = isPbPbsample(sampleType);
    bool isPP = isPPsample(sampleType);

    bool isPbPb15 = isPbPb && (sampleType.find("2015") != std::string::npos);
    bool isPbPb18 = isPbPb && (sampleType.find("2018") != std::string::npos);
    bool isPP17data = !isPbPb && !isMC && (sampleType.find("2017") != std::string::npos);

    int collisionType = -1;
    if (isPbPb15) {
        collisionType = COLL::TYPE::kHI;
    }
    else if (isPbPb18) {
        collisionType = COLL::TYPE::kHI2018;
    }
    else if (isPP) {
        collisionType = COLL::TYPE::kPP;
    }

    std::cout << "isMC = " << isMC << std::endl;
    std::cout << "isPbPb15 = " << isPbPb15 << std::endl;
    std::cout << "isPbPb18 = " << isPbPb18 << std::endl;
    std::cout << "isPP17data = " << isPP17data << std::endl;
    std::cout << "collisionType = " << collisionType << std::endl;

    double jetPtMin = (ArgumentParser::optionExists("--jetPtMin", argOptions)) ?
            std::atof(ArgumentParser::ParseOptionInputSingle("--jetPtMin", argOptions).c_str()) : 10;
    double jetEtaMax = (ArgumentParser::optionExists("--jetEtaMax", argOptions)) ?
                std::atof(ArgumentParser::ParseOptionInputSingle("--jetEtaMax", argOptions).c_str()) : 2;

    double trkPtMin = (ArgumentParser::optionExists("--trkPtMin", argOptions)) ?
                std::atof(ArgumentParser::ParseOptionInputSingle("--trkPtMin", argOptions).c_str()) : 0.7;

    double trkEtaMax = (ArgumentParser::optionExists("--trkEtaMax", argOptions)) ?
                    std::atof(ArgumentParser::ParseOptionInputSingle("--trkEtaMax", argOptions).c_str()) : 2.4;

    std::cout << "jetPtMin = " << jetPtMin << std::endl;
    std::cout << "jetEtaMax = " << jetEtaMax << std::endl;

    std::cout << "trkPtMin = " << trkPtMin << std::endl;
    std::cout << "trkEtaMax = " << trkEtaMax << std::endl;

    std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.c_str());
    std::cout<<"input ROOT files : num = "<<inputFiles.size()<< std::endl;
    std::cout<<"#####"<< std::endl;
    for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
        std::cout<<(*it).c_str()<< std::endl;
    }
    std::cout<<"##### END #####"<< std::endl;

    // input trees
    TTree* treeHLT = 0;
    TTree* treeggHiNtuplizer = 0;
    TTree* treeHiEvt = 0;
    std::vector<TTree*> treesJet(nJetCollections, 0);
    TTree* treeTrack = 0;
    TTree* treePFCand = 0;
    TTree* treeHiGenParticle = 0;
    TTree* treeHiForestInfo = 0;
    TTree* treeSkim = 0;

    std::string treePathHLT = "hltanalysis/HltTree";
    std::string treePathHiEvt = "hiEvtAnalyzer/HiTree";
    std::string treePathTrack = "ppTrack/trackTree";
    std::string treePathggHi = "ggHiNtuplizerGED/EventTree";
    if (isPbPb15) {
        treePathTrack = "anaTrack/trackTree";
    }
    std::string treePathPFCand = "pfcandAnalyzer/pfTree";
    std::string treePathSkimAna = "skimanalysis/HltTree";
    std::string treePathGen = "HiGenParticleAna/hi";

    int nFiles = inputFiles.size();
    TFile* fileTmp = 0;

    std::cout << "initial reading to get the number of entries (if there is only one input file) and HiForest info" << std::endl;
    // read the first file only to get the HiForest info
    std::string inputPath = inputFiles.at(0).c_str();
    fileTmp = TFile::Open(inputPath.c_str(), "READ");
    fileTmp->cd();

    if (nFiles == 1) {
        // read one tree only to get the number of entries
        treeggHiNtuplizer = (TTree*)fileTmp->Get(treePathggHi.c_str());
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

    TFile* output = TFile::Open(outputFile.c_str(), "RECREATE");

    // output trees
    TTree* outTreeHLT = 0;
    TTree* outTreeggHiNtuplizer = 0;
    TTree* outTreeHiEvt = 0;
    TTree* outTreeSkimAna = 0;
    TTree* eventSkimTree = 0;
    std::vector<TTree*> outTreesJet(nJetCollections, 0);
    TTree* outTreeTrack = 0;
    TTree* outTreeHiGenParticle = 0;

    eventSkimTree= new TTree("eventSkim", "additional event info that is not in hiEvt");
    eventSkimTree->SetMaxTreeSize(MAXTREESIZE);
    eventSkim evtskim;
    evtskim.setupTreeForWriting(eventSkimTree);

    for (int i=0; i<nJetCollections; ++i) {

        // pick a unique, but also not complicated name for zJet Trees
        // jet collection names which are complicated will be put into tree title
        std::string treeJetSkimTitle = Form("skim of %s", jetCollections.at(i).c_str());

        output->cd();
        output->mkdir(jetCollections.at(i).c_str());
        output->cd(jetCollections.at(i).c_str());
        outTreesJet[i] = new TTree("t", treeJetSkimTitle.c_str());
        outTreesJet[i]->SetMaxTreeSize(MAXTREESIZE);
    }
    std::vector<Jets> jetsOut(nJetCollections);
    for (int i=0; i<nJetCollections; ++i) {
        jetsOut[i].setupTreeForWriting(outTreesJet[i]);
    }

    output->cd();
    if (!isPbPb15) {
        output->mkdir("ppTrack");
        output->cd("ppTrack");
    }
    else {
        output->mkdir("anaTrack");
        output->cd("anaTrack");
    }
    outTreeTrack = new TTree("trackTree", "skim of tracks");
    outTreeTrack->SetMaxTreeSize(MAXTREESIZE);
    Tracks trksOut;
    trksOut.setupTreeForWriting(outTreeTrack);

    hiGenParticle hiGenOut;
    if (isMC) {
        output->cd();
        output->mkdir("HiGenParticleAna");
        output->cd("HiGenParticleAna");
        outTreeHiGenParticle = new TTree("hi", "skim of gen particles");
        outTreeHiGenParticle->SetMaxTreeSize(MAXTREESIZE);
        hiGenOut.setupTreeForWriting(outTreeHiGenParticle);
    }

    Long64_t entries = 0;
    Long64_t entriesAnalyzed = 0;

    int nFilesSkipped = 0;
    std::cout<< "Loop : " << treePathggHi.c_str() <<std::endl;
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

        // trees for which all info in a branch will be written
        treeHLT = (TTree*)fileTmp->Get(treePathHLT.c_str());
        if (treeHLT == 0) {
            treeHLT = (TTree*)fileTmp->Get("hltanalysisReco/HltTree");
        }
        treeHLT->SetBranchStatus("*",1);

        treeggHiNtuplizer = (TTree*)fileTmp->Get(treePathggHi.c_str());
        treeggHiNtuplizer->SetBranchStatus("*",1);

        treeHiEvt = (TTree*)fileTmp->Get(treePathHiEvt.c_str());
        treeHiEvt->SetBranchStatus("*",1);

        treeSkim = (TTree*)fileTmp->Get(treePathSkimAna.c_str());
        treeSkim->SetBranchStatus("*",1);

        for (int i = 0; i < nJetCollections; ++i) {

            treesJet[i] = (TTree*)fileTmp->Get(Form("%s/t", jetCollections[i].c_str()));
            setBranchStatusTreeJet(treesJet[i], isMC);
        }

        treeTrack = (TTree*)fileTmp->Get(treePathTrack.c_str());
        setBranchStatusTreeTrack(treeTrack, true);

        treePFCand = (TTree*)fileTmp->Get(treePathPFCand.c_str());
        setBranchStatusTreePFCand(treePFCand);

        if (isMC) {
            treeHiGenParticle = (TTree*)fileTmp->Get(treePathGen.c_str());
            setBranchStatusTreeHiGenParticle(treeHiGenParticle);
        }

        ggHiNtuplizer ggHi;
        ggHi.setupTreeForReading(treeggHiNtuplizer);

        hiEvt hiEvt;
        hiEvt.setupTreeForReading(treeHiEvt);

        skimAnalysis skimAna;
        if (isPbPb15) skimAna.enableBranchesHI(treeSkim);
        else if (isPbPb18) skimAna.enableBranchesHI2018(treeSkim);
        else if (!isPbPb) skimAna.enableBranchesPP(treeSkim);
        skimAna.setupTreeForReading(treeSkim);
        skimAna.checkBranches(treeSkim);    // do the event selection if the branches exist.

        std::vector<Jets> jets(nJetCollections);
        for (int i=0; i<nJetCollections; ++i) {
            jets.at(i).setupTreeForReading(treesJet[i]);
        }

        Tracks trks;
        trks.setupTreeForReading(treeTrack);

        pfCand pf;
        pf.setupTreeForReading(treePFCand);

        hiGenParticle hiGen;
        if (isMC) {
            hiGen.setupTreeForReading(treeHiGenParticle);
        }

        output->cd();

        if (iFile == 0) {
            output->cd();
            output->mkdir("hltanalysis");
            output->cd("hltanalysis");
            outTreeHLT = treeHLT->CloneTree(0);

            output->cd();
            output->mkdir("ggHiNtuplizerGED");
            output->cd("ggHiNtuplizerGED");
            outTreeggHiNtuplizer = treeggHiNtuplizer->CloneTree(0);

            output->cd();
            output->mkdir("hiEvtAnalyzer");
            output->cd("hiEvtAnalyzer");
            outTreeHiEvt = treeHiEvt->CloneTree(0);

            output->cd();
            output->mkdir("skimanalysis");
            output->cd("skimanalysis");
            outTreeSkimAna = treeSkim->CloneTree(0);

            outTreeHLT->SetMaxTreeSize(MAXTREESIZE);
            outTreeggHiNtuplizer->SetMaxTreeSize(MAXTREESIZE);
            outTreeHiEvt->SetMaxTreeSize(MAXTREESIZE);
            outTreeSkimAna->SetMaxTreeSize(MAXTREESIZE);

            //outTreeHLT->SetName("hltTree");
            //outTreeHLT->SetTitle("subbranches of hltanalysis/HltTree");
        }
        else {
            treeHLT->CopyAddresses(outTreeHLT);
            treeggHiNtuplizer->CopyAddresses(outTreeggHiNtuplizer);
            treeHiEvt->CopyAddresses(outTreeHiEvt);
            treeSkim->CopyAddresses(outTreeSkimAna);
        }

        Long64_t entriesTmp = treeggHiNtuplizer->GetEntries();
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

            entriesAnalyzed++;

            treeHLT->GetEntry(j_entry);
            treeggHiNtuplizer->GetEntry(j_entry);
            if (isMC) {
                treeHiGenParticle->GetEntry(j_entry);
            }

            for (int i = 0; i < nJetCollections; ++i) {
                treesJet[i]->GetEntry(j_entry);
            }
            treeTrack->GetEntry(j_entry);
            treePFCand->GetEntry(j_entry);

            for (int iJ = 0; iJ < nJetCollections; ++iJ) {

                int iOut = 0;
                for (int i = 0; i < jets[iJ].nref; ++i) {

                    if (jets[iJ].jtpt[i] < jetPtMin) continue;
                    if (std::fabs(jets[iJ].jteta[i]) > jetEtaMax) continue;

                    jetsOut[iJ].jtpt[iOut] = jets[iJ].jtpt[i];
                    jetsOut[iJ].jteta[iOut] = jets[iJ].jteta[i];
                    jetsOut[iJ].jtphi[iOut] = jets[iJ].jtphi[i];
                    jetsOut[iJ].rawpt[iOut] = jets[iJ].rawpt[i];
                    if (isMC) {
                        jetsOut[iJ].refpt[iOut] = jets[iJ].refpt[i];
                        jetsOut[iJ].refeta[iOut] = jets[iJ].refeta[i];
                        jetsOut[iJ].refphi[iOut] = jets[iJ].refphi[i];
                        jetsOut[iJ].refparton_flavor[iOut] = jets[iJ].refparton_flavor[i];
                        jetsOut[iJ].subid[iOut] = jets[iJ].subid[i];
                    }

                    iOut++;
                }
                jetsOut[iJ].nref = iOut;

                iOut = 0;
                if (isMC) {
                    for (int i = 0; i < jets[iJ].ngen; ++i) {

                        jetsOut[iJ].genpt[iOut] = jets[iJ].genpt[i];
                        jetsOut[iJ].geneta[iOut] = jets[iJ].geneta[i];
                        jetsOut[iJ].genphi[iOut] = jets[iJ].genphi[i];
                        jetsOut[iJ].gensubid[iOut] = jets[iJ].gensubid[i];

                        iOut++;
                    }
                }
                jetsOut[iJ].ngen = iOut;
            }

            int iOut = 0;
            for (int i = 0; i < trks.nTrk; ++i) {

                //if (!passedTrkSelection(trks, i, collisionType))  continue;
                if (!(trks.trkPt[i] > trkPtMin))  continue;
                if (!(std::fabs(trks.trkEta[i]) < trkEtaMax))  continue;

                trksOut.trkPt[iOut] = trks.trkPt[i];
                trksOut.trkEta[iOut] = trks.trkEta[i];
                trksOut.trkPhi[iOut] = trks.trkPhi[i];
                trksOut.trkCharge[iOut] = trks.trkCharge[i];
                trksOut.trkPtError[iOut] = trks.trkPtError[i];
                trksOut.trkNHit[iOut] = trks.trkNHit[i];
                trksOut.trkNlayer[iOut] = trks.trkNlayer[i];
                trksOut.highPurity[iOut] = trks.highPurity[i];
                trksOut.trkChi2[iOut] = trks.trkChi2[i];
                trksOut.trkNdof[iOut] = trks.trkNdof[i];
                trksOut.trkDxy1[iOut] = trks.trkDxy1[i];
                trksOut.trkDxyError1[iOut] = trks.trkDxyError1[i];
                trksOut.trkDz1[iOut] = trks.trkDz1[i];
                trksOut.trkDzError1[iOut] = trks.trkDzError1[i];
                trksOut.trkAlgo[iOut] = trks.trkAlgo[i];
                if (trks.b_trkMVA != 0) {
                    trksOut.trkMVA[iOut] = trks.trkMVA[i];
                }
                else {
                    trksOut.trkMVA[iOut] = -987987;
                }
                if (trks.b_pfType != 0) {
                    trksOut.pfType[iOut] = trks.pfType[i];
                    trksOut.pfCandPt[iOut] = trks.pfCandPt[i];
                    trksOut.pfEcal[iOut] = trks.pfEcal[i];
                    trksOut.pfHcal[iOut] = trks.pfHcal[i];
                }
                else {
                    trksOut.pfType[iOut] = -776655;
                    trksOut.pfCandPt[iOut] = -776655;
                    trksOut.pfEcal[iOut] = -776655;
                    trksOut.pfHcal[iOut] = -776655;
                }
                trksOut.trkNVtx[iOut] = trks.trkNVtx[i];
                iOut++;
            }
            trksOut.nVtx = trks.nVtx;
            trksOut.maxPtVtx = trks.maxPtVtx;
            trksOut.maxMultVtx = trks.maxMultVtx;
            trksOut.nTrk = iOut;

            evtskim.clearEvent();
            evtskim.rho = ggHi.rho;
            evtskim.pf_h_HF_E_eta3to4 = getPFtotE(pf, 6, 3.0, 4.0);
            evtskim.pf_eg_HF_E_eta3to4 = getPFtotE(pf, 7, 3.0, 4.0);
            evtskim.pf_h_HF_E_eta4to5 = getPFtotE(pf, 6, 4.0, 5.0);
            evtskim.pf_eg_HF_E_eta4to5 = getPFtotE(pf, 7, 4.0, 5.0);
            evtskim.pf_h_HF_totE = (evtskim.pf_h_HF_E_eta3to4 + evtskim.pf_h_HF_E_eta4to5);
            evtskim.pf_eg_HF_totE = (evtskim.pf_eg_HF_E_eta3to4 + evtskim.pf_eg_HF_E_eta4to5);
            evtskim.pf_h_HE_totE = getPFtotE(pf, 1, VJT::etaMin_pf_HE, VJT::etaMax_pf_HE);
            evtskim.pf_e_HE_totE = getPFtotE(pf, 2, VJT::etaMin_pf_HE, VJT::etaMax_pf_HE);
            evtskim.pf_mu_HE_totE = getPFtotE(pf, 3, VJT::etaMin_pf_HE, VJT::etaMax_pf_HE);
            evtskim.pf_gamma_HE_totE = getPFtotE(pf, 4, VJT::etaMin_pf_HE, VJT::etaMax_pf_HE);
            evtskim.pf_h0_HE_totE = getPFtotE(pf, 5, VJT::etaMin_pf_HE, VJT::etaMax_pf_HE);

            hiGenOut.clearEvent();
            if (isMC) {
                for (int i = 0; i < hiGen.mult; ++i) {

                    if (!((*hiGen.pt)[i] > trkPtMin))  continue;
                    if (!(std::fabs((*hiGen.eta)[i]) < trkEtaMax))  continue;

                    hiGenOut.out_pt.push_back((*hiGen.pt)[i]);
                    hiGenOut.out_eta.push_back((*hiGen.eta)[i]);
                    hiGenOut.out_phi.push_back((*hiGen.phi)[i]);
                    hiGenOut.out_pdg.push_back((*hiGen.pdg)[i]);
                    hiGenOut.out_chg.push_back((*hiGen.chg)[i]);
                    hiGenOut.out_sube.push_back((*hiGen.sube)[i]);
                    hiGenOut.mult++;
                }
            }

            outTreeHLT->Fill();
            outTreeggHiNtuplizer->Fill();
            outTreeHiEvt->Fill();
            outTreeSkimAna->Fill();
            eventSkimTree->Fill();
            for (int i=0; i<nJetCollections; ++i) {
                outTreesJet[i]->Fill();
            }
            outTreeTrack->Fill();
            if (isMC) {
                outTreeHiGenParticle->Fill();
            }
        }
        fileTmp->Close();
    }

    std::cout<<  "Loop ENDED : " << treePathggHi.c_str() <<std::endl;
    std::cout << "nFilesSkipped = " << nFilesSkipped << std::endl;
    std::cout << "entries             = " << entries << std::endl;
    std::cout << "entriesAnalyzed     = " << entriesAnalyzed << std::endl;

    std::cout << "outTreeHLT->GetEntries()           = " << outTreeHLT->GetEntries() << std::endl;
    std::cout << "outTreeggHiNtuplizer->GetEntries() = " << outTreeggHiNtuplizer->GetEntries() << std::endl;
    std::cout << "outTreeHiEvt->GetEntries()         = " << outTreeHiEvt->GetEntries() << std::endl;
    std::cout << "outTreeSkimAna->GetEntries()       = " << outTreeSkimAna->GetEntries() << std::endl;
    std::cout << "eventSkimTree->GetEntries()        = " << eventSkimTree->GetEntries() << std::endl;
    for (int i=0; i<nJetCollections; ++i) {
        std::cout << Form("outTreesJet[%d]->GetEntries()      = ", i) << outTreesJet[i]->GetEntries() << std::endl;
    }
    std::cout << "outTreeTrack->GetEntries()     = " << outTreeTrack->GetEntries() << std::endl;
    if (isMC) {
        std::cout << "outTreeHiGenParticle->GetEntries()  = " << outTreeHiGenParticle->GetEntries() << std::endl;
    }

    output->cd();
    std::cout<<"Writing the output file."<<std::endl;
    output->Write("",TObject::kOverwrite);
    std::cout<<"Closing the output file."<<std::endl;
    output->Close();

    std::cout<<"running mixFileSkim() - END"<<std::endl;
}

int main(int argc, char** argv)
{
    std::vector<std::string> argStr = ArgumentParser::ParseParameters(argc, argv);
    int nArgStr = argStr.size();

    argOptions = ArgumentParser::ParseOptions(argc, argv);

    if (nArgStr == 4) {
        mixFileSkim(argStr.at(1), argStr.at(2), argStr.at(3));
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./mixFileSkim.exe <configFile> <inputFile> <outputFile>"
                << std::endl;
        return 1;
    }
}

int readConfiguration(std::string configFile, std::string inputFile)
{
    ConfigurationParser confParser;
    confParser.openConfigFile(configFile);

    confParser.parsedKeyWords = InputConfigurationParser::parseKeyWords(inputFile);

    sampleType = confParser.ReadConfigValue("sampleType");

    jetCollections = ConfigurationParser::ParseList(confParser.ReadConfigValue("jetCollections"));

    nJetCollections = jetCollections.size();

    return 0;
}

/*
 * print information read from configuration
 * assumes that readConfiguration() is run before
 */
void printConfiguration()
{
    std::cout<<"Configuration :"<<std::endl;

    std::cout << "sampleType = " << sampleType.c_str() << std::endl;

    std::cout << "nJetCollections  = " << nJetCollections << std::endl;
    for (int i = 0; i < nJetCollections; ++i) {
        std::cout << Form("jetCollections[%d] = %s", i, jetCollections.at(i).c_str()) << std::endl;
    }
}
