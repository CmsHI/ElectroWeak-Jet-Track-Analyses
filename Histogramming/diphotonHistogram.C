#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TCut.h>
#include <TH1D.h>

#include <iostream>

#include "../TreeHeaders/CutConfigurationTree.h"
#include "../Utilities/interface/CutConfigurationParser.h"

void diphotonHistogram(const char* configFile, const char* inputFile, const char* outputFile = "diphotonHistogram.root", const char* sampleName = "");

void diphotonHistogram(const char* configFile, const char* inputFile, const char* outputFile, const char* sampleName)
{
    std::cout<<"running diphotonHistogram()" <<std::endl;
    std::cout<<"configFile = "<< configFile <<std::endl;
    std::cout<<"inputFile  = "<< inputFile <<std::endl;
    std::cout<<"outputFile = "<< outputFile <<std::endl;

    TFile* input = new TFile(inputFile);
    TTree* tHLT = (TTree*)input->Get("HltTree");
    TTree *tHiEvt = (TTree*)input->Get("HiEvt");       // HiEvt tree will be placed in PP forest as well.
    TTree* t_dipho = (TTree*)input->Get("diphoton");
    TTree* treeEvent;
    input->GetObject("EventTree",treeEvent);    // EventTree may not be in "diphotonSkim.root"

    t_dipho->AddFriend(tHLT,"tHLT");
    t_dipho->AddFriend(tHiEvt,"HiEvt");

    if(treeEvent)  t_dipho->AddFriend(treeEvent,"tEle");

    TFile* output = new TFile(outputFile, "UPDATE");

    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile);
    TTree* configTree = setupConfigurationTreeForWriting(configCuts);

    // event cuts/weights
    int doEventWeight;
    std::string eventWeight;    // weight to be used for histogram entries
                                // current purpose of this variable is for weighting events from MC samples.
    std::string str_trigger;

    int   cut_matched_eleIndex;
    float cut_phoSigmaIEtaIEta_spike;
//    float cut_phoSigmaIPhiIPhi_spike;     //  not available yet
    float cut_phoHoverE;
    float cut_pho_ecalClusterIsoR4;
    float cut_pho_hcalRechitIsoR4;
    float cut_pho_trackIsoR4PtCut20;
    float cut_phoSigmaIEtaIEta;
    float cut_sumIso;
    if (configCuts.isValid) {
        doEventWeight = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].i[CUTS::EVT::k_doEventWeight];
        eventWeight = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_eventWeight].c_str();


        str_trigger = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_trigger_diphoton].c_str();

        cut_matched_eleIndex = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].i[CUTS::PHO::k_matched_eleIndex];
        cut_phoSigmaIEtaIEta_spike = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_phoSigmaIEtaIEta_spike];
//        cut_phoSigmaIPhiIPhi_spike = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_phoSigmaIPhiIPhi_spike];
        cut_phoHoverE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_phoHoverE];
        cut_pho_ecalClusterIsoR4 = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pho_ecalClusterIsoR4];
        cut_pho_hcalRechitIsoR4 = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pho_hcalRechitIsoR4];
        cut_pho_trackIsoR4PtCut20 = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_pho_trackIsoR4PtCut20];
        cut_phoSigmaIEtaIEta = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_phoSigmaIEtaIEta];
        cut_sumIso = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].f[CUTS::PHO::k_sumIso];
    }
    else {  // default configuration for photons
        str_trigger = "HLT_HIDoublePhoton15_Eta2p5_Mass50_1000_R9SigmaHECut_v1";

        cut_matched_eleIndex = -1;
        cut_phoSigmaIEtaIEta_spike = 0.002;
//        cut_phoSigmaIPhiIPhi_spike = 0.002;
        cut_phoHoverE = 0.1;
        cut_pho_ecalClusterIsoR4 = 4.2;
        cut_pho_hcalRechitIsoR4 = 2.2;
        cut_pho_trackIsoR4PtCut20 = 2;
        cut_phoSigmaIEtaIEta = 0.01;
        cut_sumIso = 6;
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

    std::cout<<"cut_phoSigmaIEtaIEta_spike = "<< cut_phoSigmaIEtaIEta_spike <<std::endl;
//    std::cout<<"cut_phoSigmaIPhiIPhi_spike = "<< cut_phoSigmaIPhiIPhi_spike <<std::endl;
    std::cout<<"cut_phoHoverE              = "<< cut_phoHoverE <<std::endl;
    std::cout<<"cut_pho_ecalClusterIsoR4   = "<< cut_pho_ecalClusterIsoR4 <<std::endl;     // not used
    std::cout<<"cut_pho_hcalRechitIsoR4    = "<< cut_pho_hcalRechitIsoR4 <<std::endl;      // not used
    std::cout<<"cut_pho_trackIsoR4PtCut20  = "<< cut_pho_trackIsoR4PtCut20 <<std::endl;    // not used
    std::cout<<"cut_phoSigmaIEtaIEta       = "<< cut_phoSigmaIEtaIEta <<std::endl;
    std::cout<<"cut_sumIso                 = "<< cut_sumIso <<std::endl;

    // check the validity of the string for the trigger
    if (str_trigger.compare("") == 0) str_trigger = "1";    // trigger is turned off, if it is not valid.

    TCanvas* c = new TCanvas("cnv","",600,600);

    const int nBins_pt = 2;
    const int nBins_eta = 4;
    int bins_pt[nBins_pt] = {10, 20};
    float bins_eta_gt[nBins_eta] = {-1,    -1, 1.4791, 1.4791};    // All ECAL, Barrel, Endcap1, Endcap2
    float bins_eta_lt[nBins_eta] = {2.4, 1.44,    2.4, 2};

    std::string histNames_pho1pho2_M[nBins_pt][nBins_eta];

    int numEntries[nBins_pt][nBins_eta];
    int numEntriesMassWindow[nBins_pt][nBins_eta];      // number of entries within given mass window, e.g. 60-120
    TCut massWindow = "diPhoM >= 60 && diPhoM < 120";
    std::cout << "massWindow = " << massWindow.GetTitle() << std::endl;

    TH1::SetDefaultSumw2();
    TH1D* h1D_pho1pho2_M[nBins_pt][nBins_eta];
    TH1D* h1D_pho1pho2_M_sameCharge[nBins_pt][nBins_eta];       // histogram for same charge

    // x-axis range : 60-120
    TH1D* h1D_pho1pho2_M_final[nBins_pt][nBins_eta];
    TH1D* h1D_pho1pho2_M_sameCharge_final[nBins_pt][nBins_eta];     // histogram for same charge

    const int entriesPerGeV = 2;
    std::string titleY = Form("Entries / (%d GeV/c^{2})", entriesPerGeV);

    for (int i=0; i<nBins_pt; ++i){
        for(int j=0; j<nBins_eta; ++j){
            histNames_pho1pho2_M[i][j] = Form("pho1pho2_M_ptBin%d_etaBin%d", i, j);

            h1D_pho1pho2_M[i][j] = new TH1D(Form("h1D_%s", histNames_pho1pho2_M[i][j].c_str()),"",100,0,200);
        }
    }

    // final Z plots have x-axis range 60-120
    const float final_xMin = 60;
    const float final_xMax = 120;
    // diPhoton mass
    bool doSameCharge = true;
    for(int i=0; i<nBins_pt; ++i){
        for(int j=0; j<nBins_eta; ++j){

            TCut selections[2];
            TCut selections_EB[2];
            TCut selections_EE[2];
            for (int iPho=0; iPho<2; ++iPho) {
                selections_EB[iPho] = "";
                selections_EE[iPho] = "";

                selections_EB[iPho] = selections_EB[iPho] && Form("matched_eleIndex_%d < %d" ,iPho+1 ,cut_matched_eleIndex);
                // spike rejection
                selections_EB[iPho] = selections_EB[iPho] && Form("phoSigmaIEtaIEta_%d > %f" ,iPho+1 ,cut_phoSigmaIEtaIEta_spike);
                // isolation
                selections_EB[iPho] = selections_EB[iPho] && Form("(pho_ecalClusterIsoR4_%d + pho_hcalRechitIsoR4_%d + pho_trackIsoR4PtCut20_%d) < %f",
                                                                iPho+1, iPho+1, iPho+1 ,cut_sumIso);
                selections_EB[iPho] = selections_EB[iPho] && Form("phoHoverE_%d < %f" ,iPho+1 ,cut_phoHoverE);
                // purity
                selections_EB[iPho] = selections_EB[iPho] && Form("phoSigmaIEtaIEta_%d < %f" ,iPho+1 ,cut_phoSigmaIEtaIEta);

                selections_EE[iPho] = selections_EB[iPho];                  // no extra selection at the moment

                TCut selection_EB_eta = Form("abs(phoEta_%d) < %f" ,iPho+1 ,bins_eta_lt[1]);
                TCut selection_EE_eta = Form("abs(phoEta_%d) > %f && abs(phoEta_%d) < %f" ,iPho+1 ,bins_eta_gt[2] ,iPho+1 ,bins_eta_lt[2]);

                selections_EB[iPho] = selections_EB[iPho] && selection_EB_eta;
                selections_EE[iPho] = selections_EE[iPho] && selection_EE_eta;
            }
            if (j == 0) {
                selections[0] = selections_EB[0] || selections_EE[0];
                selections[1] = selections_EB[1] || selections_EE[1];
            }
            if (j == 1) // Barrel,  |eta supercluster| <= 1.479
            {
                selections[0] = selections_EB[0];
                selections[1] = selections_EB[1];
            }
            if ( j>1 )  // Endcap,  1.479 < |eta supercluster| < 2.4
            {
                selections[0] = selections_EE[0];
                selections[1] = selections_EE[1];
            }

            TCut selection_event = Form("%s == 1", str_trigger.c_str());
            TCut selection =  "";
            selection = selection && Form("phoEt_1 > %d && phoEt_2 > %d", bins_pt[i], bins_pt[i]);
            selection = selection && selection_event;

            TCut selection_sameCh = selection && "matched_eleCharge_1 == matched_eleCharge_2";
            selection             = selection && "matched_eleCharge_1 != matched_eleCharge_2";
            TCut selection_massWindow = selection && massWindow;

            // verbose
            std::cout<< "[i][j] = " << i << " , " << j <<std::endl;
            std::cout<< "selection = " << selection.GetTitle() <<std::endl;
            std::cout<< "h1D_pho1pho2_M[i][j]->GetName() = " << h1D_pho1pho2_M[i][j]->GetName() <<std::endl;
            numEntries[i][j] = t_dipho->GetEntries(selection.GetTitle());
            std::cout<< "numEntries[i][j] = " << numEntries[i][j] <<std::endl;
            numEntriesMassWindow[i][j] = t_dipho->GetEntries(selection_massWindow.GetTitle());
            std::cout<< "numEntriesMassWindow[i][j] = " << numEntriesMassWindow[i][j] <<std::endl;

            if (doEventWeight > 0) {
                selection = Form("(%s)*(%s)", eventWeight.c_str(), selection.GetTitle());
                selection_sameCh = Form("(%s)*(%s)", eventWeight.c_str(), selection_sameCh.GetTitle());
            }

            std::string histoName  = Form("h_%s",histNames_pho1pho2_M[i][j].c_str());
            std::string histoTitle = Form("%s p^{#gamma}_{T} > %d GeV/c, %.2f< |#eta^{#gamma}| <%.1f ",sampleName , bins_pt[i], bins_eta_gt[j], bins_eta_lt[j]);
            // special cases
            if (bins_eta_gt[j] < 0)   {
                histoTitle = Form("%s p^{#gamma}_{T} > %d GeV/c, |#eta^{#gamma}| < %.2f ",sampleName , bins_pt[i], bins_eta_lt[j]);
            }
            h1D_pho1pho2_M[i][j]->SetTitle(Form("%s;M^{#gamma#gamma} (GeV/c^{2});%s",histoTitle.c_str(), titleY.c_str()));
            h1D_pho1pho2_M[i][j]->SetMarkerStyle(kFullCircle);
            h1D_pho1pho2_M[i][j]->SetMarkerColor(kBlack);
            h1D_pho1pho2_M_sameCharge[i][j] = (TH1D*)h1D_pho1pho2_M[i][j]->Clone(Form("%s_sameCharge", h1D_pho1pho2_M[i][j]->GetName()));
            h1D_pho1pho2_M_sameCharge[i][j]->SetMarkerStyle(kOpenCircle);

            c->SetName(Form("cnv_%s",histNames_pho1pho2_M[i][j].c_str()));
            c->cd();
            t_dipho->Draw(Form("diPhoM >> %s", h1D_pho1pho2_M[i][j]->GetName()), selection.GetTitle() ,"goff");
            h1D_pho1pho2_M[i][j]->Draw("e");
            h1D_pho1pho2_M[i][j]->Write("", TObject::kOverwrite);
            h1D_pho1pho2_M[i][j]->SetStats(false);     // remove stat box from the canvas, but keep in the histograms.
            if (doSameCharge) {
                t_dipho->Draw(Form("diPhoM >> %s", h1D_pho1pho2_M_sameCharge[i][j]->GetName()), selection_sameCh.GetTitle(), "goff");
                h1D_pho1pho2_M_sameCharge[i][j]->Draw("e same");
                h1D_pho1pho2_M_sameCharge[i][j]->Write("", TObject::kOverwrite);
                h1D_pho1pho2_M_sameCharge[i][j]->SetStats(false);
            }
            c->Write("", TObject::kOverwrite);
            c->Clear();

            c->SetName(Form("cnv_%s_final",histNames_pho1pho2_M[i][j].c_str()));
            c->cd();
            h1D_pho1pho2_M_final[i][j] = (TH1D*)h1D_pho1pho2_M[i][j]->Clone(Form("%s_final", h1D_pho1pho2_M[i][j]->GetName()));
            h1D_pho1pho2_M_final[i][j]->SetAxisRange(final_xMin,final_xMax);
            h1D_pho1pho2_M_final[i][j]->Draw("e");
            h1D_pho1pho2_M_final[i][j]->Write("", TObject::kOverwrite);
            h1D_pho1pho2_M_final[i][j]->SetStats(false);     // remove stat box from the canvas, but keep in the histograms.
            if (doSameCharge) {
                h1D_pho1pho2_M_sameCharge_final[i][j]= (TH1D*)h1D_pho1pho2_M_sameCharge[i][j]->Clone(Form("%s_final", h1D_pho1pho2_M_sameCharge[i][j]->GetName()));
                h1D_pho1pho2_M_sameCharge_final[i][j]->SetAxisRange(final_xMin,final_xMax);
                h1D_pho1pho2_M_sameCharge_final[i][j]->Draw("e same");
                h1D_pho1pho2_M_sameCharge_final[i][j]->Write("", TObject::kOverwrite);
                h1D_pho1pho2_M_sameCharge_final[i][j]->SetStats(false);
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
        diphotonHistogram(argv[1], argv[2], argv[3], argv[4]);
        return 0;
    }
    else if (argc == 4) {
        diphotonHistogram(argv[1], argv[2], argv[3]);
        return 0;
    }
    else if (argc == 3) {
        diphotonHistogram(argv[1], argv[2]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./diphotonHistogram.exe <configFile> <inputFile> <outputFile> (<sampleName>)"
                << std::endl;
        return 1;
    }
}
