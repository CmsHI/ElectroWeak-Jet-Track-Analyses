#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TRandom3.h>

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

#include "../../CorrelationTuple/EventMatcher.h"
#include "../../TreeHeaders/trackJetTree.h"
#include "../../TreeHeaders/CutConfigurationTree.h"
#include "../../Plotting/commonUtility.h"
#include "../../Utilities/interface/CutConfigurationParser.h"
#include "../../Utilities/interface/InputConfigurationParser.h"
#include "../../Utilities/interface/HiForestInfoController.h"

const long MAXTREESIZE = 200000000000; // set maximum tree size from 10 GB to 100 GB, so that the code does not switch to a new file after 10 GB

void trackJetSkim(const TString configFile, const TString inputFile, const TString outputFile = "trackJetSkim.root");

void trackJetSkim(const TString configFile, const TString inputFile, const TString outputFile)
{
       std::cout<<"running trackJetSkim()"<<std::endl;
       std::cout<<"configFile  = "<< configFile.Data() <<std::endl;
       std::cout<<"inputFile   = "<< inputFile.Data() <<std::endl;
       std::cout<<"outputFile  = "<< outputFile.Data() <<std::endl;

       InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
       CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

       // input configuration
       int collisionType;
       if (configInput.isValid) {
           collisionType = configInput.proc[INPUT::kSKIM].i[INPUT::k_collisionType];
       }
       else {
           collisionType = COLL::kPP;
       }
       // verbose about input configuration
       std::cout<<"Input Configuration :"<<std::endl;
       std::cout << "collisionType = " << collisionType << std::endl;
       const char* collisionName =  getCollisionTypeName((COLL::TYPE)collisionType).c_str();
       std::cout << "collision = " << collisionName << std::endl;

       // cut configuration
       float cut_vz;
       int cut_pcollisionEventSelection;
       int cut_pPAprimaryVertexFilter;
       int cut_pBeamScrapingFilter;
       // track cuts
       float trkPt;
       float trkEta;
       float trkMVA;
       float trkPtErrorOverPt;
       float trkDz1OverError1;
       float trkDxy1OverError1;
       int   trkNHit;
       int   highPurity;        // quality cut
       int   loose;             // quality cut
       int   tight;             // quality cut
       // jet cuts
       std::vector<std::string> jetCollections;
       float jetpt;
       float jeteta;
       int jetType;
       // track-jet cuts
       float dR;

       if (configCuts.isValid) {
           cut_vz = configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].f[CUTS::EVT::k_vz];
           cut_pcollisionEventSelection = configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].i[CUTS::EVT::k_pcollisionEventSelection];
           cut_pPAprimaryVertexFilter = configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].i[CUTS::EVT::k_pPAprimaryVertexFilter];
           cut_pBeamScrapingFilter = configCuts.proc[CUTS::kSKIM].obj[CUTS::kEVENT].i[CUTS::EVT::k_pBeamScrapingFilter];

           trkPt = configCuts.proc[CUTS::kSKIM].obj[CUTS::kTRACK].f[CUTS::TRK::k_trkPt];
           trkEta = configCuts.proc[CUTS::kSKIM].obj[CUTS::kTRACK].f[CUTS::TRK::k_trkEta];
           trkMVA = configCuts.proc[CUTS::kSKIM].obj[CUTS::kTRACK].f[CUTS::TRK::k_trkMVA];
           trkPtErrorOverPt = configCuts.proc[CUTS::kSKIM].obj[CUTS::kTRACK].f[CUTS::TRK::k_trkPtErrorOverPt];
           trkDz1OverError1 = configCuts.proc[CUTS::kSKIM].obj[CUTS::kTRACK].f[CUTS::TRK::k_trkDz1OverError1];
           trkDxy1OverError1 = configCuts.proc[CUTS::kSKIM].obj[CUTS::kTRACK].f[CUTS::TRK::k_trkDxy1OverError1];
           trkNHit = configCuts.proc[CUTS::kSKIM].obj[CUTS::kTRACK].i[CUTS::TRK::k_trkNHit];
           highPurity = configCuts.proc[CUTS::kSKIM].obj[CUTS::kTRACK].i[CUTS::TRK::k_highPurity];
           loose = configCuts.proc[CUTS::kSKIM].obj[CUTS::kTRACK].i[CUTS::TRK::k_loose];
           tight = configCuts.proc[CUTS::kSKIM].obj[CUTS::kTRACK].i[CUTS::TRK::k_tight];

           jetCollections = ConfigurationParser::ParseList(configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].s[CUTS::JET::k_jetCollection]);
           jetpt = configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].f[CUTS::JET::k_pt];
           jeteta = configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].f[CUTS::JET::k_eta];
           jetType = configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].i[CUTS::JET::k_jetType];

           dR = configCuts.proc[CUTS::kSKIM].obj[CUTS::kTRACKJET].f[CUTS::TJT::k_dR];
       }
       else {
           cut_vz = 15;
           cut_pcollisionEventSelection = 1;
           cut_pPAprimaryVertexFilter = 1;
           cut_pBeamScrapingFilter = 1;

           trkPt = 4;
           trkEta = 3;
           trkMVA = 0.5;
           trkPtErrorOverPt = 0.3;
           trkDz1OverError1 = 3;
           trkDxy1OverError1 = 3;
           trkNHit = 8;
           highPurity = 0;
           loose = 0;
           tight = 0;

           jetpt = 30;
           jeteta = 1.6;
           jetType = 0;

           dR = 0.3;
       }
       int nJetCollections = jetCollections.size();

       bool isMC = collisionIsMC((COLL::TYPE)collisionType);
       bool isHI = collisionIsHI((COLL::TYPE)collisionType);

       // verbose about cut configuration
       std::cout<<"Cut Configuration :"<<std::endl;
       std::cout<<"cut_vz = "<< cut_vz <<std::endl;
       if (isHI) {
           std::cout<<"cut_pcollisionEventSelection = "<< cut_pcollisionEventSelection <<std::endl;
       }
       else {   // PP
           std::cout<<"cut_pPAprimaryVertexFilter = "<< cut_pPAprimaryVertexFilter <<std::endl;
           std::cout<<"cut_pBeamScrapingFilter = "<< cut_pBeamScrapingFilter <<std::endl;
       }

       std::cout<<"trackPt  = "<<trkPt<<std::endl;
       std::cout<<"trkEta   = "<<trkEta<<std::endl;
       std::cout<<"trkMVA   = "<<trkMVA<<std::endl;
       std::cout<<"trkPtErrorOverPt = "<<trkPtErrorOverPt<<std::endl;
       std::cout<<"trkDz1OverError1 = "<<trkDz1OverError1<<std::endl;
       std::cout<<"trkDxy1OverError1 = "<<trkDxy1OverError1<<std::endl;
       std::cout<<"trkNHit = "<<trkNHit<<std::endl;
       std::cout<<"highPurity = "<<highPurity<<std::endl;
       std::cout<<"loose      = "<<loose<<std::endl;
       std::cout<<"tight      = "<<tight<<std::endl;

       std::cout<<"nJetCollections = "<< nJetCollections <<std::endl;
       for (int i=0; i<nJetCollections; ++i) {
           std::cout << Form("jetCollections[%d] = %s", i, jetCollections.at(i).c_str()) << std::endl;
       }
       std::cout<<"jetpt  = "<<jetpt<<std::endl;
       std::cout<<"jeteta = "<<jeteta<<std::endl;
       std::cout<<"jetIndex = "<<jetType<<std::endl;
       if (!(jetType > -1 && jetType < CUTS::JET::kN_JETINDEX)) {
           std::cout<<"no proper jetType is specified."<< std::endl;
           std::cout<<"exiting"<< std::endl;
           return;
       }

       std::cout<<"dR = "<<dR<<std::endl;

       std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.Data());

       std::cout<<"input ROOT files : num = "<<inputFiles.size()<< std::endl;
       std::cout<<"#####"<< std::endl;
       for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
           std::cout<<(*it).c_str()<< std::endl;
       }
       std::cout<<"##### END #####"<< std::endl;

       TChain* treeHLT   = new TChain("hltanalysis/HltTree");
       TChain* treeTrack  = new TChain("anaTrack/trackTree");
       TChain* treeEvent = new TChain("ggHiNtuplizer/EventTree");
       TChain* treeJet[nJetCollections];
       for (int i=0; i<nJetCollections; ++i) {
           treeJet[i] = new TChain(Form("%s/t", jetCollections.at(i).c_str()));
       }
       TChain* treeHiEvt = new TChain("hiEvtAnalyzer/HiTree");
       TChain* treeSkim  = new TChain("skimanalysis/HltTree");
       TChain* treeHiForestInfo = new TChain("HiForest/HiForestInfo");

       for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
          treeHLT->Add((*it).c_str());
          treeTrack->Add((*it).c_str());
          treeEvent->Add((*it).c_str());
          for (int i=0; i<nJetCollections; ++i) {
              treeJet[i]->Add((*it).c_str());
          }
          treeHiEvt->Add((*it).c_str());
          treeSkim->Add((*it).c_str());
          treeHiForestInfo->Add((*it).c_str());
       }

       HiForestInfoController hfic(treeHiForestInfo);
       std::cout<<"### HiForestInfo Tree ###"<< std::endl;
       hfic.printHiForestInfo();
       std::cout<<"###"<< std::endl;

       treeHLT->SetBranchStatus("*",0);     // disable all branches
       treeHLT->SetBranchStatus("HLT_HI*SinglePhoton*Eta*v1*",1);     // enable photon branches
       treeHLT->SetBranchStatus("HLT_HI*DoublePhoton*Eta*v1*",1);     // enable photon branches

       treeTrack->SetBranchStatus("*", 0);
       treeTrack->SetBranchStatus("nTrk*", 1);
       treeTrack->SetBranchStatus("trk*", 1);
       treeTrack->SetBranchStatus("highPurity", 1);
       treeTrack->SetBranchStatus("tight", 1);
       treeTrack->SetBranchStatus("loose", 1);

       treeEvent->SetBranchStatus("*",0);
       treeEvent->SetBranchStatus("run",1);
       treeEvent->SetBranchStatus("event",1);
       treeEvent->SetBranchStatus("lumis",1);

       for (int i=0; i<nJetCollections; ++i) {
           treeJet[i]->SetBranchStatus("*",0);        // disable all branches
           treeJet[i]->SetBranchStatus("nref",1);     // enable jet branches
           treeJet[i]->SetBranchStatus("rawpt",1);    // enable jet branches
           treeJet[i]->SetBranchStatus("jtpt",1);     // enable jet branches
           treeJet[i]->SetBranchStatus("jteta",1);     // enable jet branches
           treeJet[i]->SetBranchStatus("jtphi",1);     // enable jet branches
           treeJet[i]->SetBranchStatus("track*",1);
           treeJet[i]->SetBranchStatus("charged*",1);
           treeJet[i]->SetBranchStatus("photon*",1);
           treeJet[i]->SetBranchStatus("neutral*",1);
           treeJet[i]->SetBranchStatus("eMax*",1);
           treeJet[i]->SetBranchStatus("eSum*",1);
           treeJet[i]->SetBranchStatus("eN*",1);
           treeJet[i]->SetBranchStatus("muMax*",1);
           treeJet[i]->SetBranchStatus("muSum*",1);
           treeJet[i]->SetBranchStatus("muN*",1);
           if (isMC) {
               treeJet[i]->SetBranchStatus("matchedPt",1);
               treeJet[i]->SetBranchStatus("matchedR",1);
               treeJet[i]->SetBranchStatus("beamId1",1);
               treeJet[i]->SetBranchStatus("beamId2",1);
               treeJet[i]->SetBranchStatus("pthat",1);
               treeJet[i]->SetBranchStatus("ngen",1);
               treeJet[i]->SetBranchStatus("gen*",1);
               treeJet[i]->SetBranchStatus("signalChargedSum",1);
               treeJet[i]->SetBranchStatus("signalHardSum",1);
               treeJet[i]->SetBranchStatus("subid",1);
               treeJet[i]->SetBranchStatus("smpt",1);
               treeJet[i]->SetBranchStatus("fr01Chg",1);
               treeJet[i]->SetBranchStatus("fr01EM",1);
               treeJet[i]->SetBranchStatus("fr01",1);
           }
       }

       // specify explicitly which branches to store, do not use wildcard
       treeHiEvt->SetBranchStatus("*",0);
       treeHiEvt->SetBranchStatus("run",1);
       treeHiEvt->SetBranchStatus("evt",1);
       treeHiEvt->SetBranchStatus("lumi",1);
       treeHiEvt->SetBranchStatus("vz",1);
       treeHiEvt->SetBranchStatus("hiBin",1);
       treeHiEvt->SetBranchStatus("hiHF",1);
       treeHiEvt->SetBranchStatus("hiHFplus",1);
       treeHiEvt->SetBranchStatus("hiHFminus",1);
       treeHiEvt->SetBranchStatus("hiHFplusEta4",1);
       treeHiEvt->SetBranchStatus("hiHFminusEta4",1);
       treeHiEvt->SetBranchStatus("hiNevtPlane",1);
       if (isMC) {
           treeHiEvt->SetBranchStatus("Npart",1);
           treeHiEvt->SetBranchStatus("Ncoll",1);
           treeHiEvt->SetBranchStatus("Nhard",1);
           treeHiEvt->SetBranchStatus("ProcessID",1);
           treeHiEvt->SetBranchStatus("pthat",1);
           treeHiEvt->SetBranchStatus("weight",1);
           treeHiEvt->SetBranchStatus("alphaQCD",1);
           treeHiEvt->SetBranchStatus("alphaQED",1);
           treeHiEvt->SetBranchStatus("qScale",1);
       }

       float vz;
       Int_t hiBin;

       treeHiEvt->SetBranchAddress("vz",&vz);
       treeHiEvt->SetBranchAddress("hiBin",&hiBin);

       // specify explicitly which branches to store, do not use wildcard
       treeSkim->SetBranchStatus("*",0);

       Int_t pcollisionEventSelection;  // this filter is used for HI.
       if (isHI) {
           treeSkim->SetBranchStatus("pcollisionEventSelection",1);
           if (treeSkim->GetBranch("pcollisionEventSelection")) {
               treeSkim->SetBranchAddress("pcollisionEventSelection",&pcollisionEventSelection);
           }
           else {   // overwrite to default
               pcollisionEventSelection = 1;
               std::cout<<"could not get branch : pcollisionEventSelection"<<std::endl;
               std::cout<<"set to default value : pcollisionEventSelection = "<<pcollisionEventSelection<<std::endl;
           }
       }
       else {
           pcollisionEventSelection = 0;    // default value if the collision is not HI, will not be used anyway.
       }
       Int_t pPAprimaryVertexFilter;    // this filter is used for PP.
       if (!isHI) {
           treeSkim->SetBranchStatus("pPAprimaryVertexFilter",1);
           if (treeSkim->GetBranch("pPAprimaryVertexFilter")) {
               treeSkim->SetBranchAddress("pPAprimaryVertexFilter",&pPAprimaryVertexFilter);
           }
           else {   // overwrite to default
               pPAprimaryVertexFilter = 1;
               std::cout<<"could not get branch : pPAprimaryVertexFilter"<<std::endl;
               std::cout<<"set to default value : pPAprimaryVertexFilter = "<<pPAprimaryVertexFilter<<std::endl;
           }
       }
       else {
           pPAprimaryVertexFilter = 0;      // default value if the collision is not PP, will not be used anyway.
       }
       Int_t pBeamScrapingFilter;   // this filter is used for PP.
       if (!isHI) {
           treeSkim->SetBranchStatus("pBeamScrapingFilter",1);
           if (treeSkim->GetBranch("pBeamScrapingFilter")) {
               treeSkim->SetBranchAddress("pBeamScrapingFilter",&pBeamScrapingFilter);
           }
           else {   // overwrite to default
               pBeamScrapingFilter = 1;
               std::cout<<"could not get branch : pBeamScrapingFilter"<<std::endl;
               std::cout<<"set to default value : pBeamScrapingFilter = "<<pBeamScrapingFilter<<std::endl;
           }
       }
       else {
           pBeamScrapingFilter = 0;     // default value if the collision is not PP, will not be used anyway.
       }

       // event information
       UInt_t run, lumis;
       ULong64_t event;
       treeEvent->SetBranchAddress("run", &run);
       treeEvent->SetBranchAddress("event", &event);
       treeEvent->SetBranchAddress("lumis", &lumis);

       // objects for track-jet correlations
       Tracks tracks;
       tracks.setupTreeForReading(treeTrack);    // treeggHiNtuplizer is input
       std::vector<Jets> jets(nJetCollections);
       for (int i=0; i<nJetCollections; ++i) {
           jets.at(i).setupTreeForReading(treeJet[i]);   // treeJet is input
       }

       TFile* output = new TFile(outputFile,"RECREATE");
       TTree* configTree = setupConfigurationTreeForWriting(configCuts);

       // output tree variables
       TTree* outputTreeHLT    = treeHLT->CloneTree(0);
       outputTreeHLT->SetName("hltTree");
       outputTreeHLT->SetTitle("subbranches of hltanalysis/HltTree");
       TTree* outputTreeTrack = treeTrack->CloneTree(0);
       TTree* outputTreeJet[nJetCollections];
       for (int i=0; i<nJetCollections; ++i) {
           outputTreeJet[i] = treeJet[i]->CloneTree(0);

           // pick a unique, but also not complicated name for jet Trees
           // jet collection names which are complicated will be put into tree title
           std::string treeJetName = jetCollections.at(i).c_str();
           std::string treeJetTitle = jetCollections.at(i).c_str();
           std::string currentTitle = outputTreeJet[i]->GetTitle();
           // do not lose the current title
           if (currentTitle.size() > 0) treeJetTitle = Form("%s - %s", treeJetTitle.c_str(), currentTitle.c_str());

           outputTreeJet[i]->SetName(treeJetName.c_str());
           outputTreeJet[i]->SetTitle(treeJetTitle.c_str());
       }
       TTree* outputTreeHiEvt = treeHiEvt->CloneTree(0);
       outputTreeHiEvt->SetName("HiEvt");
       outputTreeHiEvt->SetTitle("subbranches of hiEvtAnalyzer/HiTree");
       TTree* outputTreeSkim   = treeSkim->CloneTree(0);
       outputTreeSkim->SetName("skim");
       outputTreeSkim->SetTitle("subbranches of skimanalysis/HltTree");
       TTree* outputTreeHiForestInfo = treeHiForestInfo->CloneTree(0);
       outputTreeHiForestInfo->SetName("HiForestInfo");
       outputTreeHiForestInfo->SetTitle("first entry of HiForest/HiForestInfo");

       outputTreeHLT->SetMaxTreeSize(MAXTREESIZE);
       outputTreeTrack->SetMaxTreeSize(MAXTREESIZE);
       outputTreeHiEvt->SetMaxTreeSize(MAXTREESIZE);
       for (int i=0; i<nJetCollections; ++i) {
           outputTreeJet[i]->SetMaxTreeSize(MAXTREESIZE);
       }
       outputTreeSkim->SetMaxTreeSize(MAXTREESIZE);
       outputTreeHiForestInfo->SetMaxTreeSize(MAXTREESIZE);

       // record HiForestInfo
       treeHiForestInfo->GetEntry(0);
       outputTreeHiForestInfo->Fill();

       TTree* trackJetTree[nJetCollections];
       for (int i=0; i<nJetCollections; ++i) {

           // pick a unique, but also not complicated name for gammaJet Trees
           // jet collection names which are complicated will be put into tree title
           std::string treetrackJetName = Form("track_%s", jetCollections.at(i).c_str());
           std::string treetrackJetTitle = Form("%s : track-jet correlations", jetCollections.at(i).c_str());

           trackJetTree[i] = new TTree(treetrackJetName.c_str(),treetrackJetTitle.c_str());
           trackJetTree[i]->SetMaxTreeSize(MAXTREESIZE);
       }
       std::vector<TrackJet> trackjet(nJetCollections);
       for (int i=0; i<nJetCollections; ++i) {
           trackjet.at(i).resetConeRange();
           trackjet.at(i).branchTrackJetTree(trackJetTree[i]);

           trackjet.at(i).coneRange = dR;
       }

       EventMatcher* em = new EventMatcher();
       Long64_t duplicateEntries = 0;

       Long64_t entries = treeEvent->GetEntries();
       Long64_t entriesPassedEventSelection = 0;
       Long64_t entriesAnalyzed = 0;
       std::cout << "entries = " << entries << std::endl;
       std::cout<< "Loop : ggHiNtuplizer/EventTree" <<std::endl;
       for (Long64_t j_entry=0; j_entry < entries; ++j_entry)
       {
           if (j_entry % 2000 == 0)  {
             std::cout << "current entry = " <<j_entry<<" out of "<<entries<<" : "<<std::setprecision(2)<<(double)j_entry/entries*100<<" %"<<std::endl;
           }

           treeHLT->GetEntry(j_entry);
           treeTrack->GetEntry(j_entry);
           treeEvent->GetEntry(j_entry);
           for (int i=0; i<nJetCollections; ++i) {
               treeJet[i]->GetEntry(j_entry);
           }
           treeSkim->GetEntry(j_entry);
           treeHiEvt->GetEntry(j_entry);

           bool eventAdded = em->addEvent(run,lumis,event,j_entry);
           if(!eventAdded) // this event is duplicate, skip this one.
           {
               duplicateEntries++;
               continue;
           }

           // event selection
           if (!(TMath::Abs(vz) < cut_vz))  continue;
           if (isHI) {
               if ((pcollisionEventSelection < cut_pcollisionEventSelection))  continue;
           }
           else {
               if (pPAprimaryVertexFilter < cut_pPAprimaryVertexFilter || pBeamScrapingFilter < cut_pBeamScrapingFilter)  continue;
           }
           entriesPassedEventSelection++;

           // track-jet block
           // jets
           std::vector<int> jetIndices(nJetCollections, -1);    // index of the finally selected jet
           for(int iAlgo = 0; iAlgo < nJetCollections; ++iAlgo) {

               // clear the object that is filled by the previous event
               trackjet.at(iAlgo).clearTrackJetPairs();

               float maxPt = -1;
               int maxPtIndex = -1;
               float max2Pt = -1;
               int max2PtIndex = -1;
               for(int i = 0; i < jets.at(iAlgo).nref; ++i) {

                   // jet cuts
                   if (!(jets.at(iAlgo).jtpt[i] > jetpt))               continue;
                   if (!(TMath::Abs(jets.at(iAlgo).jteta[i]) < jeteta || jeteta == 0)) continue;

                   if (jets.at(iAlgo).jtpt[i] > maxPt) {
                       // current leading becomes subleading
                       max2PtIndex = maxPtIndex;
                       max2Pt      = maxPt;

                       maxPtIndex = i;
                       maxPt      = jets.at(iAlgo).jtpt[i];
                   }
                   else if (jets.at(iAlgo).jtpt[i] > max2Pt) {
                       max2PtIndex = i;
                       max2Pt      = jets.at(iAlgo).jtpt[i];
                   }
               }
               if (jetType == CUTS::JET::k_leading)    jetIndices.at(iAlgo) = maxPtIndex;
               if (jetType == CUTS::JET::k_subleading) jetIndices.at(iAlgo) = max2PtIndex;
           }
           for(int iAlgo = 0; iAlgo < nJetCollections; ++iAlgo) {
               if (jetIndices.at(iAlgo) > -1) {
                   entriesAnalyzed++;
                   break;
               }
           }
           // tracks
           for (int i = 0; i < tracks.nTrk; ++i) {

               // track cuts
               if (!(tracks.trkPt[i] > trkPt))  continue;
               if (!(TMath::Abs(tracks.trkEta[i]) < jeteta || jeteta == 0)) continue;
               if ( tracks.trkMVA[i] < trkMVA && tracks.trkMVA[i] != -99)   continue;
               if ( tracks.trkPtError[i]/tracks.trkPt[i] > trkPtErrorOverPt) continue;
               if ( TMath::Abs(tracks.trkDz1[i])/tracks.trkDzError1[i] > trkDz1OverError1) continue;
               if ( TMath::Abs(tracks.trkDxy1[i])/tracks.trkDxyError1[i] > trkDxy1OverError1) continue;
               if ( tracks.trkNHit[i] < trkNHit) continue;
               if ( tracks.highPurity[i] < highPurity )  continue;
               if ( tracks.loose[i] < loose )  continue;
               if ( tracks.tight[i] < tight )  continue;

               // track-jet correlation
               for (int iAlgo = 0; iAlgo < nJetCollections; ++iAlgo) {

                   if (jetIndices.at(iAlgo) > -1)
                   {
                       trackjet.at(iAlgo).makeTrackJetPair(tracks, i, jets.at(iAlgo), jetIndices.at(iAlgo));
                   }
               }
           }

           outputTreeHLT->Fill();
           outputTreeTrack->Fill();
           for (int i = 0; i < nJetCollections; ++i) {
               outputTreeJet[i]->Fill();
           }
           outputTreeHiEvt->Fill();
           outputTreeSkim->Fill();

           for (int i = 0; i < nJetCollections; ++i) {
               trackJetTree[i]->Fill();
           }
       }
       std::cout<<  "Loop ENDED : ggHiNtuplizer/EventTree" <<std::endl;
       std::cout << "entries            = " << entries << std::endl;
       std::cout << "duplicateEntries   = " << duplicateEntries << std::endl;
       std::cout << "entriesPassedEventSelection   = " << entriesPassedEventSelection << std::endl;
       std::cout << "entriesAnalyzed               = " << entriesAnalyzed << std::endl;
       std::cout << "outputTreeHLT->GetEntries()   = " << outputTreeHLT->GetEntries() << std::endl;
       std::cout << "outputTreeTrack->GetEntries() = " << outputTreeTrack->GetEntries() << std::endl;
       for (int i = 0; i < nJetCollections; ++i) {
           std::cout << Form("outputTreeJet[%d]->GetEntries() = ", i) << outputTreeJet[i]->GetEntries() << std::endl;
       }
       std::cout << "outputTreeSkim->GetEntries()  = " << outputTreeSkim->GetEntries() << std::endl;
       std::cout << "outputTreeHiEvt->GetEntries() = " << outputTreeHiEvt->GetEntries() << std::endl;
    
       for (int i = 0; i < nJetCollections; ++i) {
           std::cout << Form("trackJetTree[%d]->GetEntries() = ", i) << trackJetTree[i]->GetEntries() << std::endl;
       }
  
       configTree->Write("",TObject::kOverwrite);

       output->Write("",TObject::kOverwrite);
       output->Close();

       std::cout<<"trackJetSkim() - END"<<std::endl;
}

int main(int argc, char** argv)
{
    if (argc == 4) {
        trackJetSkim(argv[1], argv[2], argv[3]);
        return 0;
    }
    else if (argc == 3) {
        trackJetSkim(argv[1], argv[2]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./trackJetSkim.exe <configFile> <inputFile> <outputFile>"
                << std::endl;
        return 1;
    }
}
