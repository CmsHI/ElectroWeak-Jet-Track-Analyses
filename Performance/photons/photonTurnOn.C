/*
 * macro to draw photon trigger turn on curves.
 * saves histograms to a .root file.
 * if "outputFigureName" is specified, saves the canvas that includes all turn on curves as picture.
 */

#include <TFile.h>
#include <TChain.h>
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

    InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

    std::vector<float> TH1D_Bins;      // nBins, xLow, xUp for the TH1D histogram
    std::string legendPosition;
    if (configInput.isValid) {
        TH1D_Bins = ConfigurationParser::ParseListFloat(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_TH1D_Bins_List]);

        legendPosition = configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_legendPosition];
    }
    else {
        TH1D_Bins.push_back(40);    // nBins
        TH1D_Bins.push_back(0);     // xLow
        TH1D_Bins.push_back(80);    // xUp

        legendPosition = "SE";
    }

    int nTH1D_Bins = TH1D_Bins.size();
    int nBins = (int)TH1D_Bins.at(0);
    float xLow = TH1D_Bins.at(1);
    float xUp  = TH1D_Bins.at(2);
    // verbose about input configuration
    std::cout<<"Input Configuration :"<<std::endl;
    std::cout << "nTH1D_Bins = " << nTH1D_Bins << std::endl;
    std::cout << "nBins = " << nBins << std::endl;
    std::cout << "xLow  = " << xLow << std::endl;
    std::cout << "xUp   = " << xUp << std::endl;
    std::cout << "legendPosition = " << legendPosition.c_str() << std::endl;

    float cutPhoEta;
    std::vector<std::string> triggerBranchesNum;        // triggers that go into numerator
    std::vector<std::string> triggerBranchesDenom;      // triggers that go into denominator
    // These triggers will be "OR"ed.
    // this vector must be empty not to use any triggers in the denominator
    if (configCuts.isValid) {
        cutPhoEta = configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].f[CUTS::PHO::k_eta];
        triggerBranchesNum = ConfigurationParser::ParseList(
                configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_triggerNum_List]);
        triggerBranchesDenom = ConfigurationParser::ParseList(
                configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_triggerDenom_List]);
    }
    else {
        cutPhoEta = 1.5;

        triggerBranchesNum.push_back("HLT_HISinglePhoton10_Eta1p5_v1");
        triggerBranchesNum.push_back("HLT_HISinglePhoton15_Eta1p5_v1");
        triggerBranchesNum.push_back("HLT_HISinglePhoton20_Eta1p5_v1");
        triggerBranchesNum.push_back("HLT_HISinglePhoton30_Eta1p5_v1");
        triggerBranchesNum.push_back("HLT_HISinglePhoton40_Eta1p5_v1");
        triggerBranchesNum.push_back("HLT_HISinglePhoton50_Eta1p5_v1");
        triggerBranchesNum.push_back("HLT_HISinglePhoton60_Eta1p5_v1");

        // default : no triggers in the denominator
    }

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

    TChain* treeHLT = new TChain("hltanalysis/HltTree");
    TChain* treeggHiNtuplizer = new TChain("ggHiNtuplizer/EventTree");

    for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
       treeHLT->Add((*it).c_str());
       treeggHiNtuplizer->Add((*it).c_str());
    }

    treeHLT->SetBranchStatus("*",0);     // disable all branches
    std::cout<<"set branch addresses for triggers that go into numerator"<<std::endl;
    Int_t triggersNum[nTriggersNum];
    for (int i=0; i < nTriggersNum; ++i) {
        std::string branch = triggerBranchesNum.at(i).c_str();
        std::cout << "branch : " << branch.c_str() <<std::endl;
        if (treeHLT->GetBranch(branch.c_str())) {
            treeHLT->SetBranchStatus(branch.c_str(),1);
            treeHLT->SetBranchAddress(branch.c_str(),&(triggersNum[i]));
        }
        else {
            std::cout<<"could not GetBranch() : "<<branch.c_str()<<std::endl;
            std::cout<<"default value set to  : "<<branch.c_str()<<" = 1"<<std::endl;
            triggersNum[i] = 1;
        }
    }

    std::cout<<"set branch addresses for triggers that go into denominator"<<std::endl;
    Int_t triggersDenom[nTriggersDenom];
    for (int i=0; i < nTriggersDenom; ++i) {
        std::string branch = triggerBranchesDenom.at(i).c_str();
        std::cout << "branch : " << branch.c_str() <<std::endl;
        if (treeHLT->GetBranch(branch.c_str())) {
            treeHLT->SetBranchStatus(branch.c_str(),1);
            treeHLT->SetBranchAddress(branch.c_str(),&(triggersDenom[i]));
        }
        else {
            std::cout<<"could not GetBranch() : "<<branch.c_str()<<std::endl;
            std::cout<<"default value set to  : "<<branch.c_str()<<" = 1"<<std::endl;
            triggersDenom[i] = 1;
        }
    }

    treeggHiNtuplizer->SetBranchStatus("*",0);     // disable all branches
    treeggHiNtuplizer->SetBranchStatus("run",1);    // enable event information
    treeggHiNtuplizer->SetBranchStatus("event",1);
    treeggHiNtuplizer->SetBranchStatus("lumis",1);

    treeggHiNtuplizer->SetBranchStatus("nPho",1);     // enable photon branches
    treeggHiNtuplizer->SetBranchStatus("pho*",1);     // enable photon branches
    ggHiNtuplizer ggHi;
    ggHi.setupTreeForReading(treeggHiNtuplizer);

    TH1::SetDefaultSumw2();
    TH1D* h_pt = new TH1D("h_pt","Denominator;photon p_{T};", nBins, xLow, xUp);
    TH1D*     h_pt_accepted[nTriggersNum];
    for (int i=0; i<nTriggersNum; ++i)
    {
        h_pt_accepted[i] = (TH1D*)h_pt->Clone(Form("%s_accepted_%d", h_pt->GetName(), i));
        h_pt_accepted[i]->SetTitle(triggerBranchesNum.at(i).c_str());
    }

    EventMatcher* em = new EventMatcher();
    Long64_t duplicateEntries = 0;

    Long64_t entries = treeggHiNtuplizer->GetEntries();
    Long64_t entriesAnalyzed = 0;
    Long64_t entriesPassedDenom = 0;
    std::cout << "entries = " << entries << std::endl;
    std::cout<< "Loop : ggHiNtuplizer/EventTree" <<std::endl;
    for (Long64_t j_entry = 0; j_entry < entries; ++j_entry)
    {
        if (j_entry % 2000 == 0)  {
          std::cout << "current entry = " <<j_entry<<" out of "<<entries<<" : "<<std::setprecision(2)<<(double)j_entry/entries*100<<" %"<<std::endl;
        }

        treeHLT->GetEntry(j_entry);
        treeggHiNtuplizer->GetEntry(j_entry);

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

        float maxPt = -1;
        for (int i=0; i<ggHi.nPho; ++i) {

            if (!(TMath::Abs(ggHi.phoEta->at(i)) < cutPhoEta)) continue;
            if (!(ggHi.phoSigmaIEtaIEta->at(i) > 0.002 && ggHi.pho_swissCrx->at(i) < 0.9 && TMath::Abs(ggHi.pho_seedTime->at(i)) < 3)) continue;
            if (doHoverE) {
                if (!(ggHi.phoHoverE->at(i) < 0.1)) continue;
            }

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
    std::cout<<  "Loop ENDED : ggHiNtuplizer/EventTree" <<std::endl;
    std::cout << "entries            = " << entries << std::endl;
    std::cout << "duplicateEntries   = " << duplicateEntries << std::endl;
    std::cout << "entriesPassedDenom = " << entriesPassedDenom << std::endl;
    std::cout << "entriesAnalyzed    = " << entriesAnalyzed << std::endl;

    TFile *output = TFile::Open(outputFile.Data(),"RECREATE");
    output->cd();
    h_pt->Write();

    TCanvas* c = new TCanvas();
    c->SetName("cnv_photonTurnOn");
    c->SetTitle("");
    c->cd();

    // base histogram on which other histograms will be plotted.
    TH1D*  h_dummy =(TH1D*)h_pt->Clone("h_dummy");
    // polishing before drawing the turn on curves
    h_dummy->Reset();
    h_dummy->SetTitle("");
    h_dummy->GetYaxis()->SetTitle("Efficiency");
    h_dummy->SetMinimum(0);
    h_dummy->SetMaximum(1.2);
    h_dummy->SetStats(false);
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
