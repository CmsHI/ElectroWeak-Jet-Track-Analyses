#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TCut.h>
#include <TH1D.h>

#include <iostream>

#include "../TreeHeaders/CutConfigurationTree.h"
#include "../Utilities/interface/CutConfigurationParser.h"

void dimuonHistogram(const char* configFile, const char* inputFile, const char* outputFile = "dimuonHistogram.root", const char* sampleName = "");

void dimuonHistogram(const char* configFile, const char* inputFile, const char* outputFile, const char* sampleName)
{
    std::cout<<"running dimuonHistogram()" <<std::endl;
    std::cout<<"configFile = "<< configFile <<std::endl;
    std::cout<<"inputFile  = "<< inputFile <<std::endl;
    std::cout<<"outputFile = "<< outputFile <<std::endl;

    TFile *input = new TFile(inputFile);
    TTree *tHLT = (TTree*)input->Get("HltTree");
    TTree *tHiEvt = (TTree*)input->Get("HiEvt");       // HiEvt tree will be placed in PP forest as well.
    TTree *t_dimu = (TTree*)input->Get("dimuon");

    t_dimu->AddFriend(tHLT,"tHLT");
    t_dimu->AddFriend(tHiEvt,"HiEvt");

    TFile* output = new TFile(outputFile, "UPDATE");

    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile);
    TTree* configTree = setupConfigurationTreeForWriting(configCuts);

    // event cuts/weights
    int doEventWeight;
    std::string eventWeight;    // weight to be used for histogram entries
                                // current purpose of this variable is for weighting events from MC samples.
    std::string str_trigger;
    float muPt;
    float muChi2NDF;
    float muInnerD0;
    float muInnerDz;
    int muMuonHits;
    int muStations;
    int muTrkLayers;
    int muPixelHits;

    if (configCuts.isValid) {
        doEventWeight = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].i[CUTS::EVT::k_doEventWeight];
        eventWeight = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_eventWeight].c_str();

        str_trigger = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kMUON].s[CUTS::MUO::k_trigger].c_str();

        muPt = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kMUON].f[CUTS::MUO::k_muPt];
        muChi2NDF = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kMUON].f[CUTS::MUO::k_muChi2NDF];
        muInnerD0 = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kMUON].f[CUTS::MUO::k_muInnerD0];
        muInnerDz = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kMUON].f[CUTS::MUO::k_muInnerDz];
        muMuonHits = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kMUON].i[CUTS::MUO::k_muMuonHits];
        muStations = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kMUON].i[CUTS::MUO::k_muStations];
        muTrkLayers = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kMUON].i[CUTS::MUO::k_muTrkLayers];
        muPixelHits = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kMUON].i[CUTS::MUO::k_muPixelHits];
    }
    else {
        str_trigger = "HLT_HIL1DoubleMu10_v1";

        muPt = 0;
        muChi2NDF = 10;
        muInnerD0 = 0.2;
        muInnerDz = 0.5;

        muMuonHits = 0;
        muStations = 1;
        muTrkLayers = 5;
        muPixelHits = 0;
    }
    // default values
    if (eventWeight.size() == 0) eventWeight = "1";

    // verbose about cut configuration
    std::cout<<"Cut Configuration :"<<std::endl;
    std::cout<<"doEventWeight = "<<doEventWeight<<std::endl;
    if (doEventWeight > 0) {
        std::cout<<"eventWeight = "<<eventWeight.c_str()<<std::endl;
    }

    std::cout<<"trigger    = "<<str_trigger.c_str()<<std::endl;

    std::cout<<"muPt = "<<muPt<<std::endl;
    std::cout<<"muChi2NDF = "<<muChi2NDF<<std::endl;
    std::cout<<"muInnerD0 = "<<muInnerD0<<std::endl;
    std::cout<<"muInnerDz = "<<muInnerDz<<std::endl;
    std::cout<<"muMuonHits = "<<muMuonHits<<std::endl;
    std::cout<<"muStations = "<<muStations<<std::endl;
    std::cout<<"muTrkLayers = "<<muTrkLayers<<std::endl;
    std::cout<<"muPixelHits = "<<muPixelHits<<std::endl;

    // check the validity of the string for the trigger
    if (str_trigger.compare("") == 0) str_trigger = "1";    // trigger is turned off, if it is not valid.

    TCanvas* c = new TCanvas("cnv","",600,600);

    const int nBins_pt = 2;
    const int nBins_eta = 4;
    int bins_pt[nBins_pt] = {10, 20};
    float bins_eta_gt[nBins_eta] = {-1,      -1, 1.4791, 1.4791};    // All ECAL, Barrel, Endcap1, Endcap2
    float bins_eta_lt[nBins_eta] = {2.4,      2,    2.4, 2};

    std::string histNames_m1m2_M[nBins_pt][nBins_eta];

    int numEntries[nBins_pt][nBins_eta];
    int numEntriesMassWindow[nBins_pt][nBins_eta];      // number of entries within given mass window, e.g. 60-120
    TCut massWindow = "diMuM >= 60 && diMuM < 120";
    std::cout << "massWindow = " << massWindow.GetTitle() << std::endl;

    TH1::SetDefaultSumw2();
    TH1D* h1D_m1m2_M[nBins_pt][nBins_eta];
    TH1D* h1D_m1m2_M_sameCharge[nBins_pt][nBins_eta];       // histogram for same charge

    // x-axis range : 60-120
    TH1D* h1D_m1m2_M_final[nBins_pt][nBins_eta];
    TH1D* h1D_m1m2_M_sameCharge_final[nBins_pt][nBins_eta];     // histogram for same charge

    const int entriesPerGeV = 2;
    std::string titleY = Form("Entries / (%d GeV/c^{2})", entriesPerGeV);

    for (int i=0; i<nBins_pt; ++i){
        for(int j=0; j<nBins_eta; ++j){
            histNames_m1m2_M[i][j] = Form("m1m2_M_ptBin%d_etaBin%d", i, j);

            h1D_m1m2_M[i][j] = new TH1D(Form("h1D_%s", histNames_m1m2_M[i][j].c_str()),"",100,0,200);
        }
    }

    // final Z plots have x-axis range 60-120
    const float final_xMin = 60;
    const float final_xMax = 120;
    // diMuon mass
    bool doSameCharge = true;
    for(int i=0; i<nBins_pt; ++i){
        for(int j=0; j<nBins_eta; ++j){

            TCut selections[2];
            for (int iMu=0; iMu<2; ++iMu) {
                selections[iMu] = "";

                selections[iMu] = selections[iMu] && Form("muChi2NDF_%d < %f", iMu+1, muChi2NDF);
                selections[iMu] = selections[iMu] && Form("abs(muInnerD0_%d) < %f", iMu+1, muInnerD0);
                selections[iMu] = selections[iMu] && Form("abs(muInnerDz_%d) < %f", iMu+1, muInnerDz);
                selections[iMu] = selections[iMu] && Form("muMuonHits_%d > %d", iMu+1, muMuonHits);
                selections[iMu] = selections[iMu] && Form("muStations_%d > %d", iMu+1, muStations);
                selections[iMu] = selections[iMu] && Form("muTrkLayers_%d > %d", iMu+1, muTrkLayers);
                selections[iMu] = selections[iMu] && Form("muPixelHits_%d > %d", iMu+1, muPixelHits);
            }

            if (j == 0) {
                selections[0] = selections[0] && Form("abs(muEta_1) < %f", bins_eta_lt[j]);
                selections[1] = selections[1] && Form("abs(muEta_2) < %f", bins_eta_lt[j]);
            }
            else // Barrel,  |eta supercluster| <= 1.479  OR  Endcap,  1.479 < |eta supercluster| < 2.5
            {
                selections[0] = selections[0] && Form("abs(muEta_1) > %f", bins_eta_gt[j]);
                selections[0] = selections[0] && Form("abs(muEta_1) < %f", bins_eta_lt[j]);

                selections[1] = selections[1] && Form("abs(muEta_2) > %f", bins_eta_gt[j]);
                selections[1] = selections[1] && Form("abs(muEta_2) < %f", bins_eta_lt[j]);
            }

            TCut selection_event = Form("%s == 1", str_trigger.c_str());
            TCut selection =  "";
            selection = selection && selection_event;
            selection = selection && Form("muPt_1 > %d && muPt_2 > %d", bins_pt[i], bins_pt[i]);
            selection = selection && selections[0] && selections[1];

            TCut selection_sameCh = selection && "muCharge_1 == muCharge_2";
            selection             = selection && "muCharge_1 != muCharge_2";
            TCut selection_massWindow = selection && massWindow;

            // verbose
            std::cout<< "[i][j] = " << i << " , " << j <<std::endl;
            std::cout<< "selection = " << selection.GetTitle() <<std::endl;
            std::cout<< "h1D_m1m2_M[i][j]->GetName() = " << h1D_m1m2_M[i][j]->GetName() <<std::endl;
            numEntries[i][j] = t_dimu->GetEntries(selection.GetTitle());
            std::cout<< "numEntries[i][j] = " << numEntries[i][j] <<std::endl;
            numEntriesMassWindow[i][j] = t_dimu->GetEntries(selection_massWindow.GetTitle());
            std::cout<< "numEntriesMassWindow[i][j] = " << numEntriesMassWindow[i][j] <<std::endl;

            if (doEventWeight > 0) {
                selection = Form("(%s)*(%s)", eventWeight.c_str(), selection.GetTitle());
                selection_sameCh = Form("(%s)*(%s)", eventWeight.c_str(), selection_sameCh.GetTitle());
            }

            std::string histoName  = Form("h_%s",histNames_m1m2_M[i][j].c_str());
            std::string histoTitle = Form("%s p^{#mu#pm}_{T} > %d GeV/c, %.2f< |#eta^{#mu#pm}| <%.1f ",sampleName , bins_pt[i], bins_eta_gt[j], bins_eta_lt[j]);
            // special cases
            if (bins_eta_gt[j] < 0)   {
                histoTitle = Form("%s p^{#mu#pm}_{T} > %d GeV/c, |#eta^{#mu#pm}| < %.2f ",sampleName , bins_pt[i], bins_eta_lt[j]);
            }
            h1D_m1m2_M[i][j]->SetTitle(Form("%s;M^{#mu#mu} (GeV/c^{2});%s",histoTitle.c_str(), titleY.c_str()));
            h1D_m1m2_M[i][j]->SetMarkerStyle(kFullCircle);
            h1D_m1m2_M[i][j]->SetMarkerColor(kBlack);
            h1D_m1m2_M_sameCharge[i][j] = (TH1D*)h1D_m1m2_M[i][j]->Clone(Form("%s_sameCharge", h1D_m1m2_M[i][j]->GetName()));
            h1D_m1m2_M_sameCharge[i][j]->SetMarkerStyle(kOpenCircle);

            c->SetName(Form("cnv_%s",histNames_m1m2_M[i][j].c_str()));
            c->cd();
            t_dimu->Draw(Form("diMuM >> %s", h1D_m1m2_M[i][j]->GetName()), selection.GetTitle() ,"goff");
            h1D_m1m2_M[i][j]->Draw("e");
            h1D_m1m2_M[i][j]->Write("", TObject::kOverwrite);
            h1D_m1m2_M[i][j]->SetStats(false);     // remove stat box from the canvas, but keep in the histograms.
            if (doSameCharge) {
                t_dimu->Draw(Form("diMuM >> %s", h1D_m1m2_M_sameCharge[i][j]->GetName()), selection_sameCh.GetTitle(), "goff");
                h1D_m1m2_M_sameCharge[i][j]->Draw("e same");
                h1D_m1m2_M_sameCharge[i][j]->Write("", TObject::kOverwrite);
                h1D_m1m2_M_sameCharge[i][j]->SetStats(false);
            }
            c->Write("", TObject::kOverwrite);
            c->Clear();

            c->SetName(Form("cnv_%s_final",histNames_m1m2_M[i][j].c_str()));
            c->cd();
            h1D_m1m2_M_final[i][j] = (TH1D*)h1D_m1m2_M[i][j]->Clone(Form("%s_final", h1D_m1m2_M[i][j]->GetName()));
            h1D_m1m2_M_final[i][j]->SetAxisRange(final_xMin,final_xMax);
            h1D_m1m2_M_final[i][j]->Draw("e");
            h1D_m1m2_M_final[i][j]->Write("", TObject::kOverwrite);
            h1D_m1m2_M_final[i][j]->SetStats(false);     // remove stat box from the canvas, but keep in the histograms.
            if (doSameCharge) {
                h1D_m1m2_M_sameCharge_final[i][j]= (TH1D*)h1D_m1m2_M_sameCharge[i][j]->Clone(Form("%s_final", h1D_m1m2_M_sameCharge[i][j]->GetName()));
                h1D_m1m2_M_sameCharge_final[i][j]->SetAxisRange(final_xMin,final_xMax);
                h1D_m1m2_M_sameCharge_final[i][j]->Draw("e same");
                h1D_m1m2_M_sameCharge_final[i][j]->Write("", TObject::kOverwrite);
                h1D_m1m2_M_sameCharge_final[i][j]->SetStats(false);
            }
            c->Write("", TObject::kOverwrite);
            c->Clear();
        }
    }

    configTree->Write("", TObject::kOverwrite);

    output->Write("", TObject::kOverwrite);
    output->Close();
    input->Close();
}

int main(int argc, char** argv)
{
    if (argc == 5) {
        dimuonHistogram(argv[1], argv[2], argv[3], argv[4]);
        return 0;
    }
    else if (argc == 4) {
        dimuonHistogram(argv[1], argv[2], argv[3]);
        return 0;
    }
    else if (argc == 3) {
        dimuonHistogram(argv[1], argv[2]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./dimuonHistogram.exe <configFile> <inputFile> <outputFile> (<sampleName>)"
                << std::endl;
        return 1;
    }
}
