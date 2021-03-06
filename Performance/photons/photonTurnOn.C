/*
 * macro to draw photon trigger turn on curves.
 * saves histograms to a .root file.
 * if "outputFigureName" is specified, saves the canvas that includes all turn on curves as picture.
 */

#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLine.h>
#include <TGraphAsymmErrors.h>

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include "../../CorrelationTuple/EventMatcher.h"
#include "../../TreeHeaders/CutConfigurationTree.h"
#include "../../TreeHeaders/ggHiNtuplizerTree.h"
#include "../../Utilities/interface/CutConfigurationParser.h"
#include "../../Utilities/interface/InputConfigurationParser.h"
#include "../../Utilities/interface/HiForestInfoController.h"
#include "../../Utilities/fileUtil.h"
#include "../../Utilities/styleUtil.h"

void photonTurnOn(const TString configFile, const TString inputFile, const TString outputFile = "photonTurnOn.root", const TString outputFigureName = "");

void photonTurnOn(const TString configFile, const TString inputFile, const TString outputFile, const TString outputFigureName)
{
    std::cout<<"running photonTurnOn()"<<std::endl;
    std::cout<<"configFile  = "<< configFile.Data() <<std::endl;
    std::cout<<"inputFile   = "<< inputFile.Data()  <<std::endl;
    std::cout<<"outputFile  = "<< outputFile.Data() <<std::endl;

    bool doHoverE = false;
    std::cout << "doHoverE = " << doHoverE << std::endl;

    bool doEcalNoiseMask= false;
    std::cout << "doEcalNoiseMask = " << doEcalNoiseMask << std::endl;

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

    // input configuration

    // input for TH1
    // input for TTree
    std::string treePath = configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_treePath];

    int collisionType = configInput.proc[INPUT::kPERFORMANCE].i[INPUT::k_collisionType];

    // input for TH1
    // nBins, xLow, xUp for the TH1D histogram
    std::vector<float> TH1D_Bins = ConfigurationParser::ParseListFloat(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH1D_Bins_List]);

    std::string legendPosition = configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_legendPosition];

    int nTH1D_Bins = TH1D_Bins.size();
    int nBins = (int)TH1D_Bins.at(0);
    float xLow = TH1D_Bins.at(1);
    float xUp  = TH1D_Bins.at(2);
    // verbose about input configuration
    std::cout<<"Input Configuration :"<<std::endl;
    std::cout << "treePath = " << treePath.c_str() << std::endl;
    std::cout << "collisionType = " << collisionType << std::endl;
    const char* collisionName =  getCollisionTypeName((COLL::TYPE)collisionType).c_str();
    std::cout << "collision = " << collisionName << std::endl;
    std::cout << "nTH1D_Bins = " << nTH1D_Bins << std::endl;
    std::cout << "nBins = " << nBins << std::endl;
    std::cout << "xLow  = " << xLow << std::endl;
    std::cout << "xUp   = " << xUp << std::endl;
    std::cout << "legendPosition = " << legendPosition.c_str() << std::endl;

    //bool isMC = collisionIsMC((COLL::TYPE)collisionType);
    bool isHI = collisionIsHI((COLL::TYPE)collisionType);
    bool isPP = collisionIsPP((COLL::TYPE)collisionType);

    // cut configuration
    float cutPhoEta = configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].f[CUTS::PHO::k_eta];
    // triggers that go into numerator
    std::vector<std::string> triggerBranchesNum = ConfigurationParser::ParseList(
            configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_triggerNum_List]);
    // triggers that go into denominator
    std::vector<std::string> triggerBranchesDenom = ConfigurationParser::ParseList(
            configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_triggerDenom_List]);
    // These triggers will be "OR"ed.
    // This vector must be empty not to use any triggers in the denominator.

    // verbose about cut configuration
    std::cout<<"Cut Configuration :"<<std::endl;
    std::cout<<"cutPhoEta = "<<cutPhoEta<<std::endl;

    int nTriggersNum = triggerBranchesNum.size();
    int nTriggersDenom = triggerBranchesDenom.size();
    std::cout << "nTriggersNum = " << nTriggersNum << std::endl;
    for (int i=0; i<nTriggersNum; ++i) {
        std::cout << Form("triggerBranchesNum[%d] = ", i) << triggerBranchesNum.at(i).c_str() << std::endl;
    }
    std::cout << "nTriggersDenom = " << nTriggersDenom << std::endl;
    for (int i=0; i<nTriggersDenom; ++i) {
        std::cout << Form("triggerBranchesDenom[%d] = ", i) << triggerBranchesDenom.at(i).c_str() << std::endl;
    }

    std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.Data());

    std::cout<<"input ROOT files : num = "<<inputFiles.size()<< std::endl;
    std::cout<<"#####"<< std::endl;
    for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
        std::cout<<(*it).c_str()<< std::endl;
    }
    std::cout<<"##### END #####"<< std::endl;

    if (isHI && treePath != "ggHiNtuplizer/EventTree") {
        std::cout << "WARNING : Collision is HI. But the photon tree is not set to ggHiNtuplizer/EventTree" << std::endl;
    }
    else if (isPP && treePath != "ggHiNtuplizerGED/EventTree") {
        std::cout << "WARNING : Collision is PP. But the photon tree is not set to ggHiNtuplizerGED/EventTree" << std::endl;
    }

    TH1::SetDefaultSumw2();
    TH1D* h_pt = new TH1D("h_pt","Denominator;p_{T}^{#gamma} (GeV/c);", nBins, xLow, xUp);
    TH1D* h_pt_accepted[nTriggersNum];
    for (int i=0; i<nTriggersNum; ++i)
    {
        h_pt_accepted[i] = (TH1D*)h_pt->Clone(Form("%s_accepted_%d", h_pt->GetName(), i));
        h_pt_accepted[i]->SetTitle(triggerBranchesNum.at(i).c_str());
    }
    TH1D* h_pt_allpho = new TH1D("h_pt_allpho","all photon that pass at least one of the triggers ;p_{T}^{#gamma} (GeV/c);", nBins, xLow, xUp);

    TTree* treeHLT;
    TTree* treePhoton;
    TTree* treeHiForestInfo;

    int nFiles = inputFiles.size();
    TFile* fileTmp = 0;

    std::cout << "initial reading to get the number of entries (if there is only one input file) and HiForest info" << std::endl;
    // read the first file only to get the HiForest info
    std::string inputPath = inputFiles.at(0).c_str();
    fileTmp = new TFile(inputPath.c_str(), "READ");

    if (nFiles == 1) {
        // read one tree only to get the number of entries
        treePhoton = (TTree*)fileTmp->Get(treePath.c_str());
        Long64_t entriesTmp = treePhoton->GetEntries();
        std::cout << "entries = " << entriesTmp << std::endl;
    }

    treeHiForestInfo = (TTree*)fileTmp->Get("HiForest/HiForestInfo");
    HiForestInfoController hfic(treeHiForestInfo);
    std::cout<<"### HiForestInfo Tree ###"<< std::endl;
    hfic.printHiForestInfo();
    std::cout<<"###"<< std::endl;

    fileTmp->Close();
    // done with initial reading

    EventMatcher* em = new EventMatcher();
    Long64_t duplicateEntries = 0;

    Long64_t entries = 0;
    Long64_t entriesPassedDenom = 0;
    Long64_t entriesAnalyzed = 0;
    std::cout<< "Loop : " << treePath.c_str() <<std::endl;
    for (int iFile = 0; iFile < nFiles; ++iFile) {

        std::string inputPath = inputFiles.at(iFile).c_str();
        std::cout <<"iFile = " << iFile << " , " ;
        std::cout <<"reading input file : " << inputPath.c_str() << std::endl;
        fileTmp = new TFile(inputPath.c_str(), "READ");

        // check if the file is usable, if not skip the file.
        if (isGoodFile(fileTmp) != 0) {
            std::cout << "File is not good. skipping file." << std::endl;
            continue;
        }

        treeHLT = (TTree*)fileTmp->Get("hltanalysis/HltTree");
        treeHLT->SetBranchStatus("*",0);     // disable all branches
        Int_t triggersNum[nTriggersNum];
        for (int i=0; i < nTriggersNum; ++i) {

            std::string branch = triggerBranchesNum.at(i).c_str();
            if (treeHLT->GetBranch(branch.c_str())) {
                treeHLT->SetBranchStatus(branch.c_str(),1);
                treeHLT->SetBranchAddress(branch.c_str(),&(triggersNum[i]));
            }
            else {
                std::cout<<"set branch addresses for triggers that go into numerator"<<std::endl;
                std::cout<<"could not GetBranch() : "<<branch.c_str()<<std::endl;
                std::cout<<"default value set to  : "<<branch.c_str()<<" = 1"<<std::endl;
                triggersNum[i] = 1;
            }
        }

        Int_t triggersDenom[nTriggersDenom];
        for (int i=0; i < nTriggersDenom; ++i) {

            std::string branch = triggerBranchesDenom.at(i).c_str();
            if (treeHLT->GetBranch(branch.c_str())) {
                treeHLT->SetBranchStatus(branch.c_str(),1);
                treeHLT->SetBranchAddress(branch.c_str(),&(triggersDenom[i]));
            }
            else {
                std::cout<<"set branch addresses for triggers that go into denominator"<<std::endl;
                std::cout<<"could not GetBranch() : "<<branch.c_str()<<std::endl;
                std::cout<<"default value set to  : "<<branch.c_str()<<" = 1"<<std::endl;
                triggersDenom[i] = 1;
            }
        }

        treePhoton = (TTree*)fileTmp->Get(treePath.c_str());
        treePhoton->SetBranchStatus("*",0);     // disable all branches
        treePhoton->SetBranchStatus("run",1);    // enable event information
        treePhoton->SetBranchStatus("event",1);
        treePhoton->SetBranchStatus("lumis",1);

        treePhoton->SetBranchStatus("nPho",1);     // enable photon branches
        treePhoton->SetBranchStatus("pho*",1);     // enable photon branches

        ggHiNtuplizer ggHi;
        ggHi.setupTreeForReading(treePhoton);

        Long64_t entriesTmp = treePhoton->GetEntries();
        entries += entriesTmp;
        std::cout << "entries in File = " << entriesTmp << std::endl;

        for (Long64_t j_entry = 0; j_entry < entriesTmp; ++j_entry)
        {
            if (j_entry % 20000 == 0)  {
              std::cout << "current entry = " <<j_entry<<" out of "<<entriesTmp<<" : "<<std::setprecision(2)<<(double)j_entry/entriesTmp*100<<" %"<<std::endl;
            }

            treeHLT->GetEntry(j_entry);
            treePhoton->GetEntry(j_entry);

            bool eventAdded = em->addEvent(ggHi.run, ggHi.lumis, ggHi.event, j_entry);
            if(!eventAdded) // this event is duplicate, skip this one.
            {
                duplicateEntries++;
                continue;
            }

            bool passedDenom = false;
            if (nTriggersDenom == 0) passedDenom = true;
            else {
                // triggers in the denominator are "OR"ed.
                for (int i = 0; i<nTriggersDenom; ++i)
                {
                    if (triggersDenom[i] == 1) {
                        passedDenom = true;
                        break;
                    }
                }
            }
            if (!passedDenom) continue;
            entriesPassedDenom++;

            // is this event passing at least one of the triggers
            bool passedNumOR = false;
            for (int i = 0; i < nTriggersNum; ++i) {
                if (triggersNum[i] == 1) passedNumOR = true;
            }

            float maxPt = -1;
            for (int i=0; i<ggHi.nPho; ++i) {

                if (!(TMath::Abs(ggHi.phoEta->at(i)) < cutPhoEta)) continue;
                if (!(ggHi.phoSigmaIEtaIEta->at(i) > 0.002 && ggHi.pho_swissCrx->at(i) < 0.9 && TMath::Abs(ggHi.pho_seedTime->at(i)) < 3)) continue;
                if (doHoverE) {
                    if (!(ggHi.phoHoverE->at(i) < 0.1)) continue;
                }
                if (doEcalNoiseMask) {
                    if (((ggHi.phoE3x3->at(i))/(ggHi.phoE5x5->at(i)) > 2./3.-0.03 &&
                            (ggHi.phoE3x3->at(i))/(ggHi.phoE5x5->at(i)) < 2./3.+0.03) &&
                        ((ggHi.phoE1x5->at(i))/(ggHi.phoE5x5->at(i)) > 1./3.-0.03 &&
                            (ggHi.phoE1x5->at(i))/(ggHi.phoE5x5->at(i)) < 1./3.+0.03) &&
                        ((ggHi.phoE2x5->at(i))/(ggHi.phoE5x5->at(i)) > 2./3.-0.03 &&
                            (ggHi.phoE2x5->at(i))/(ggHi.phoE5x5->at(i)) < 2./3.+0.03)) continue;
                }
                if (passedNumOR) h_pt_allpho->Fill(ggHi.phoEt->at(i));

                if (ggHi.phoEt->at(i) > maxPt) {
                    maxPt = ggHi.phoEt->at(i);
                }
            }

            // leading photon goes into histograms
            if(maxPt == -1) continue;
            entriesAnalyzed++;

            h_pt->Fill(maxPt);

            for (int i=0; i<nTriggersNum; ++i) {
                if (triggersNum[i] == 1)  h_pt_accepted[i]->Fill(maxPt);
            }
        }

        fileTmp->Close();
    }
    std::cout<<  "Loop ENDED : " << treePath.c_str() <<std::endl;
    std::cout << "entries            = " << entries << std::endl;
    std::cout << "duplicateEntries   = " << duplicateEntries << std::endl;
    std::cout << "entriesPassedDenom = " << entriesPassedDenom << std::endl;
    std::cout << "entriesAnalyzed    = " << entriesAnalyzed << std::endl;

    TFile *output = TFile::Open(outputFile.Data(),"RECREATE");
    output->cd();
    h_pt->Write();
    h_pt_allpho->Write();

    TCanvas* c = new TCanvas("cnv_photonTurnOn", "", 800, 800);
    c->cd();

    // base histogram on which other histograms will be plotted.
    TH1D*  h_dummy =(TH1D*)h_pt->Clone("h_dummy");
    // polishing before drawing the turn on curves
    h_dummy->Reset();
    h_dummy->SetTitle("");
    h_dummy->GetYaxis()->SetTitle("Efficiency");
    h_dummy->SetMinimum(0);
    h_dummy->SetMaximum(1.4);
    h_dummy->SetStats(false);
    h_dummy->GetXaxis()->CenterTitle();
    h_dummy->GetYaxis()->CenterTitle();
    h_dummy->Draw();

    // draw line y = 1
    TLine *line = new TLine(h_dummy->GetXaxis()->GetXmin(), 1, h_dummy->GetXaxis()->GetXmax(),1);
    line->SetLineStyle(kDashed);
    line->Draw();

    TLegend* leg = new TLegend();
    leg->SetHeader("Triggers");

    // assume not more than 13 curves are drawn.
    int turnOnColors[13] = {kBlack, kBlue, kRed,   kOrange,  kViolet, kCyan, kSpring, kYellow,
                            kAzure, kPink, kGreen, kMagenta, kTeal};

    TGraphAsymmErrors* a[nTriggersNum];
    for (int i=0; i<nTriggersNum; ++i)
    {
        a[i] = new TGraphAsymmErrors();
        a[i]->SetName(Form("a_%s",h_pt_accepted[i]->GetName()));
        a[i]->SetTitle(h_pt_accepted[i]->GetTitle());
        a[i]->GetYaxis()->SetTitle("Efficiency");
        a[i]->BayesDivide(h_pt_accepted[i], h_pt);
        a[i]->SetMarkerStyle(kFullCircle);
        a[i]->SetMarkerColor(turnOnColors[i]);
        a[i]->SetLineColor(turnOnColors[i]);
        a[i]->Draw("p e");

        h_pt_accepted[i]->Write();
        a[i]->Write();

        leg->AddEntry(a[i], triggerBranchesNum.at(i).c_str(), "lp");
    }
    double height = calcTLegendHeight(leg);
    double width = calcTLegendWidth(leg);
    setLegendPosition(leg, legendPosition, c, height, width);
    leg->Draw();
    c->Write();

    // save canvas as picture if a figure name is provided.
    if (! outputFigureName.EqualTo("")) {

        c->SaveAs(Form("%s.C", outputFigureName.Data()));
        c->SaveAs(Form("%s.png", outputFigureName.Data()));
        c->SaveAs(Form("%s.pdf", outputFigureName.Data()));
    }
    c->Close();

    output->Close();
}

int main(int argc, char** argv)
{
    if (argc == 5) {
        photonTurnOn(argv[1], argv[2], argv[3], argv[4]);
        return 0;
    }
    else if (argc == 4) {
        photonTurnOn(argv[1], argv[2], argv[3]);
        return 0;
    }
    else if (argc == 3) {
        photonTurnOn(argv[1], argv[2]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./photonTurnOn.exe <configFile> <inputFile> <outputFile> (<outputFigureName>)"
                << std::endl;
        return 1;
    }
}
