#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TCut.h>
#include <TH1D.h>

#include <iostream>

#include "../TreeHeaders/CutConfigurationTree.h"
#include "../Utilities/interface/CutConfigurationParser.h"

void dielectronHistogram(const char* configFile, const char* inputFile, const char* outputFile = "dielectronHistogram.root", const char* sampleName = "");

void dielectronHistogram(const char* configFile, const char* inputFile, const char* outputFile, const char* sampleName)
{
    std::cout<<"running dielectronHistogram()" <<std::endl;
    std::cout<<"configFile = "<< configFile <<std::endl;
    std::cout<<"inputFile  = "<< inputFile <<std::endl;
    std::cout<<"outputFile = "<< outputFile <<std::endl;

    TFile *input = new TFile(inputFile);
    TTree *tHLT = (TTree*)input->Get("HltTree");
    TTree *tHiEvt = (TTree*)input->Get("HiEvt");       // HiEvt tree will be placed in PP forest as well.
    TTree *t_diele = (TTree*)input->Get("dielectron");

    t_diele->AddFriend(tHLT,"tHLT");
    t_diele->AddFriend(tHiEvt,"HiEvt");

    TFile* output = new TFile(outputFile, "UPDATE");

    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile);
    TTree* configTree = setupConfigurationTreeForWriting(configCuts);

    // event cuts/weights
    int doEventWeight;
    std::string eventWeight;    // weight to be used for histogram entries
                                // current purpose of this variable is for weighting events from MC samples.
    std::string str_electronID;
    std::string str_trigger;
    float eleSigmaIEtaIEta_2012_EB;
    float eledEtaAtVtx_abs_EB;
    float eledPhiAtVtx_abs_EB;
    float eleHoverE_EB;
    float eleEoverPInv_EB;
    float eleD0_abs_EB;
    float eleDz_abs_EB;
    int   eleMissHits_EB;
    float eleSigmaIEtaIEta_2012_EE;
    float eledEtaAtVtx_abs_EE;
    float eledPhiAtVtx_abs_EE;
    float eleHoverE_EE;
    float eleEoverPInv_EE;
    float eleD0_abs_EE;
    float eleDz_abs_EE;
    int   eleMissHits_EE;
    if (configCuts.isValid) {
        doEventWeight = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].i[CUTS::EVT::k_doEventWeight];
        eventWeight = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_eventWeight].c_str();

        str_electronID = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].s[CUTS::ELE::k_electronID].c_str();
        str_trigger = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].s[CUTS::ELE::k_trigger].c_str();

        // Barrel
        eleSigmaIEtaIEta_2012_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleSigmaIEtaIEta_2012_EB];
        eledEtaAtVtx_abs_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eledEtaAtVtx_abs_EB];
        eledPhiAtVtx_abs_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eledPhiAtVtx_abs_EB];
        eleHoverE_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleHoverE_EB];
        eleEoverPInv_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleEoverPInv_EB];
        eleD0_abs_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleD0_abs_EB];
        eleDz_abs_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleDz_abs_EB];
        eleMissHits_EB = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].i[CUTS::ELE::k_eleMissHits_EB];

        // Endcap
        eleSigmaIEtaIEta_2012_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleSigmaIEtaIEta_2012_EE];
        eledEtaAtVtx_abs_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eledEtaAtVtx_abs_EE];
        eledPhiAtVtx_abs_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eledPhiAtVtx_abs_EE];
        eleHoverE_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleHoverE_EE];
        eleEoverPInv_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleEoverPInv_EE];
        eleD0_abs_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleD0_abs_EE];
        eleDz_abs_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleDz_abs_EE];
        eleMissHits_EE = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].i[CUTS::ELE::k_eleMissHits_EE];
    }
    else {  // default configuration for electron ID
        str_electronID = "veto";
        str_trigger = "HLT_HIDoublePhoton15_Eta2p5_Mass50_1000_R9SigmaHECut_v1";

        // Barrel
        eleSigmaIEtaIEta_2012_EB = 0.012;
        eledEtaAtVtx_abs_EB = 0.0126;
        eledPhiAtVtx_abs_EB = 0.107;
        eleHoverE_EB = 0.186;
        eleEoverPInv_EB = 0.239;
        eleD0_abs_EB = 0.0621;
        eleDz_abs_EB = 0.613;
        eleMissHits_EB = 2;

        // Endcap
        eleSigmaIEtaIEta_2012_EE = 0.0339;
        eledEtaAtVtx_abs_EE = 0.0109;
        eledPhiAtVtx_abs_EE = 0.219;
        eleHoverE_EE = 0.0962;
        eleEoverPInv_EE = 0.141;
        eleD0_abs_EE = 0.279;
        eleDz_abs_EE = 0.947;
        eleMissHits_EE = 3;
    }
    // default values
    if (eventWeight.size() == 0) eventWeight = "1";

    // verbose about cut configuration
    std::cout<<"Cut Configuration :"<<std::endl;
    std::cout<<"doEventWeight = "<<doEventWeight<<std::endl;
    if (doEventWeight > 0) {
        std::cout<<"eventWeight = "<<eventWeight.c_str()<<std::endl;
    }

    std::cout<<"electronID = "<<str_electronID.c_str()<<std::endl;
    std::cout<<"trigger    = "<<str_trigger.c_str()<<std::endl;

    std::cout<<"Barrel :"<<std::endl;
    std::cout<<"eleSigmaIEtaIEta_2012_EB = "<<eleSigmaIEtaIEta_2012_EB<<std::endl;
    std::cout<<"eledEtaAtVtx_abs_EB      = "<<eledEtaAtVtx_abs_EB<<std::endl;
    std::cout<<"eledPhiAtVtx_abs_EB      = "<<eledPhiAtVtx_abs_EB<<std::endl;
    std::cout<<"eleHoverE_EB             = "<<eleHoverE_EB<<std::endl;
    std::cout<<"eleEoverPInv_EB          = "<<eleEoverPInv_EB<<std::endl;
    std::cout<<"eleD0_abs_EB             = "<<eleD0_abs_EB<<std::endl;
    std::cout<<"eleDz_abs_EB             = "<<eleDz_abs_EB<<std::endl;
    std::cout<<"eleMissHits_EB           = "<<eleMissHits_EB<<std::endl;

    std::cout<<"Endcap :"<<std::endl;
    std::cout<<"eleSigmaIEtaIEta_2012_EE = "<<eleSigmaIEtaIEta_2012_EE<<std::endl;
    std::cout<<"eledEtaAtVtx_abs_EE      = "<<eledEtaAtVtx_abs_EE<<std::endl;
    std::cout<<"eledPhiAtVtx_abs_EE      = "<<eledPhiAtVtx_abs_EE<<std::endl;
    std::cout<<"eleHoverE_EE             = "<<eleHoverE_EE<<std::endl;
    std::cout<<"eleEoverPInv_EE          = "<<eleEoverPInv_EE<<std::endl;
    std::cout<<"eleD0_abs_EE             = "<<eleD0_abs_EE<<std::endl;
    std::cout<<"eleDz_abs_EE             = "<<eleDz_abs_EE<<std::endl;
    std::cout<<"eleMissHits_EE           = "<<eleMissHits_EE<<std::endl;

    // check the validity of the string for the trigger
    if (str_trigger.compare("") == 0) str_trigger = "1";    // trigger is turned off, if it is not valid.

    TCanvas* c = new TCanvas("cnv","",600,600);

    const int nBins_pt = 2;
    const int nBins_eta = 4;
    int bins_pt[nBins_pt] = {10, 20};
    float bins_eta_gt[nBins_eta] = {-1,    -1, 1.4791, 1.4791};    // All ECAL, Barrel, Endcap1, Endcap2
    float bins_eta_lt[nBins_eta] = {2.4, 1.44,    2.4, 2};

    std::string histNames_e1e2_M[nBins_pt][nBins_eta];

    int numEntries[nBins_pt][nBins_eta];
    int numEntriesMassWindow[nBins_pt][nBins_eta];      // number of entries within given mass window, e.g. 60-120
    TCut massWindow = "diEleM >= 60 && diEleM < 120";
    std::cout << "massWindow = " << massWindow.GetTitle() << std::endl;

    TH1::SetDefaultSumw2();
    TH1D* h1D_e1e2_M[nBins_pt][nBins_eta];
    TH1D* h1D_e1e2_M_sameCharge[nBins_pt][nBins_eta];       // histogram for same charge

    // x-axis range : 60-120
    TH1D* h1D_e1e2_M_final[nBins_pt][nBins_eta];
    TH1D* h1D_e1e2_M_sameCharge_final[nBins_pt][nBins_eta];     // histogram for same charge

    const int entriesPerGeV = 2;
    std::string titleY = Form("Entries / (%d GeV/c^{2})", entriesPerGeV);

    for (int i=0; i<nBins_pt; ++i){
        for(int j=0; j<nBins_eta; ++j){
            histNames_e1e2_M[i][j] = Form("e1e2_M_ptBin%d_etaBin%d", i, j);

            h1D_e1e2_M[i][j] = new TH1D(Form("h1D_%s", histNames_e1e2_M[i][j].c_str()),"",100,0,200);
        }
    }

    // final Z plots have x-axis range 60-120
    const float final_xMin = 60;
    const float final_xMax = 120;
    // diElectron mass
    bool doSameCharge = true;
    for(int i=0; i<nBins_pt; ++i){
        for(int j=0; j<nBins_eta; ++j){

            TCut selections[2];
            TCut selections_EB[2];
            TCut selections_EE[2];
            for (int iEle=0; iEle<2; ++iEle) {
                selections_EB[iEle] = "";
                selections_EE[iEle] = "";

                selections_EB[iEle] = selections_EB[iEle] && Form("eleSigmaIEtaIEta_2012_%d < %f" ,iEle+1 ,eleSigmaIEtaIEta_2012_EB);
                selections_EB[iEle] = selections_EB[iEle] && Form("abs(eledEtaAtVtx_%d) < %f" ,iEle+1 ,eledEtaAtVtx_abs_EB);
                selections_EB[iEle] = selections_EB[iEle] && Form("abs(eledPhiAtVtx_%d) < %f" ,iEle+1 ,eledPhiAtVtx_abs_EB);
                selections_EB[iEle] = selections_EB[iEle] && Form("eleHoverE_%d < %f" ,iEle+1 ,eleHoverE_EB);
                selections_EB[iEle] = selections_EB[iEle] && Form("eleEoverPInv_%d < %f" ,iEle+1 ,eleEoverPInv_EB);
                selections_EB[iEle] = selections_EB[iEle] && Form("abs(eleD0_%d) < %f" ,iEle+1 ,eleD0_abs_EB);
                selections_EB[iEle] = selections_EB[iEle] && Form("abs(eleDz_%d) < %f" ,iEle+1 ,eleDz_abs_EB);
                selections_EB[iEle] = selections_EB[iEle] && Form("eleMissHits_%d <= %d" ,iEle+1 ,eleMissHits_EB);

                selections_EE[iEle] = selections_EE[iEle] && Form("eleSigmaIEtaIEta_2012_%d < %f" ,iEle+1 ,eleSigmaIEtaIEta_2012_EE);
                selections_EE[iEle] = selections_EE[iEle] && Form("abs(eledEtaAtVtx_%d) < %f" ,iEle+1 ,eledEtaAtVtx_abs_EE);
                selections_EE[iEle] = selections_EE[iEle] && Form("abs(eledPhiAtVtx_%d) < %f" ,iEle+1 ,eledPhiAtVtx_abs_EE);
                selections_EE[iEle] = selections_EE[iEle] && Form("eleHoverE_%d < %f" ,iEle+1 ,eleHoverE_EE);
                selections_EE[iEle] = selections_EE[iEle] && Form("eleEoverPInv_%d < %f" ,iEle+1 ,eleEoverPInv_EE);
                selections_EE[iEle] = selections_EE[iEle] && Form("abs(eleD0_%d) < %f" ,iEle+1 ,eleD0_abs_EE);
                selections_EE[iEle] = selections_EE[iEle] && Form("abs(eleDz_%d) < %f" ,iEle+1 ,eleDz_abs_EE);
                selections_EE[iEle] = selections_EE[iEle] && Form("eleMissHits_%d <= %d" ,iEle+1 ,eleMissHits_EE);

                TCut selection_EB_eta = Form("abs(eleEta_%d) < %f" ,iEle+1 ,bins_eta_lt[1]);
                TCut selection_EE_eta = Form("abs(eleEta_%d) > %f && abs(eleEta_%d) < %f" ,iEle+1 ,bins_eta_gt[2] ,iEle+1 ,bins_eta_lt[2]);

                selections_EB[iEle] = selections_EB[iEle] && selection_EB_eta;
                selections_EE[iEle] = selections_EE[iEle] && selection_EE_eta;
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
            selection = selection && selection_event;
            selection = selection && Form("elePt_1 > %d && elePt_2 > %d", bins_pt[i], bins_pt[i]);
            selection = selection && selections[0] && selections[1];

            TCut selection_sameCh = selection && "eleCharge_1 == eleCharge_2";
            selection             = selection && "eleCharge_1 != eleCharge_2";
            TCut selection_massWindow = selection && massWindow;

            // verbose
            std::cout<< "[i][j] = " << i << " , " << j <<std::endl;
            std::cout<< "selection = " << selection.GetTitle() <<std::endl;
            std::cout<< "h1D_e1e2_M[i][j]->GetName() = " << h1D_e1e2_M[i][j]->GetName() <<std::endl;
            numEntries[i][j] = t_diele->GetEntries(selection.GetTitle());
            std::cout<< "numEntries[i][j] = " << numEntries[i][j] <<std::endl;
            numEntriesMassWindow[i][j] = t_diele->GetEntries(selection_massWindow.GetTitle());
            std::cout<< "numEntriesMassWindow[i][j] = " << numEntriesMassWindow[i][j] <<std::endl;

            if (doEventWeight > 0) {
                selection = Form("(%s)*(%s)", eventWeight.c_str(), selection.GetTitle());
                selection_sameCh = Form("(%s)*(%s)", eventWeight.c_str(), selection_sameCh.GetTitle());
            }

            std::string histoName  = Form("h_%s",histNames_e1e2_M[i][j].c_str());
            std::string histoTitle = Form("%s p^{e#pm}_{T} > %d GeV/c, %.2f< |#eta^{e#pm}| <%.1f ",sampleName , bins_pt[i], bins_eta_gt[j], bins_eta_lt[j]);
            // special cases
            if (bins_eta_gt[j] < 0)   {
                histoTitle = Form("%s p^{e#pm}_{T} > %d GeV/c, |#eta^{e#pm}| < %.2f ",sampleName , bins_pt[i], bins_eta_lt[j]);
            }
            h1D_e1e2_M[i][j]->SetTitle(Form("%s;M^{ee} (GeV/c^{2});%s",histoTitle.c_str(), titleY.c_str()));
            h1D_e1e2_M[i][j]->SetMarkerStyle(kFullCircle);
            h1D_e1e2_M[i][j]->SetMarkerColor(kBlack);
            h1D_e1e2_M_sameCharge[i][j] = (TH1D*)h1D_e1e2_M[i][j]->Clone(Form("%s_sameCharge", h1D_e1e2_M[i][j]->GetName()));
            h1D_e1e2_M_sameCharge[i][j]->SetMarkerStyle(kOpenCircle);

            c->SetName(Form("cnv_%s",histNames_e1e2_M[i][j].c_str()));
            c->cd();
            t_diele->Draw(Form("diEleM >> %s", h1D_e1e2_M[i][j]->GetName()), selection.GetTitle() ,"goff");
            h1D_e1e2_M[i][j]->Draw("e");
            h1D_e1e2_M[i][j]->Write("", TObject::kOverwrite);
            h1D_e1e2_M[i][j]->SetStats(false);     // remove stat box from the canvas, but keep in the histograms.
            if (doSameCharge) {
                t_diele->Draw(Form("diEleM >> %s", h1D_e1e2_M_sameCharge[i][j]->GetName()), selection_sameCh.GetTitle(), "goff");
                h1D_e1e2_M_sameCharge[i][j]->Draw("e same");
                h1D_e1e2_M_sameCharge[i][j]->Write("", TObject::kOverwrite);
                h1D_e1e2_M_sameCharge[i][j]->SetStats(false);
            }
            c->Write("", TObject::kOverwrite);
            c->Clear();

            c->SetName(Form("cnv_%s_final",histNames_e1e2_M[i][j].c_str()));
            c->cd();
            h1D_e1e2_M_final[i][j] = (TH1D*)h1D_e1e2_M[i][j]->Clone(Form("%s_final", h1D_e1e2_M[i][j]->GetName()));
            h1D_e1e2_M_final[i][j]->SetAxisRange(final_xMin,final_xMax);
            h1D_e1e2_M_final[i][j]->Draw("e");
            h1D_e1e2_M_final[i][j]->Write("", TObject::kOverwrite);
            h1D_e1e2_M_final[i][j]->SetStats(false);     // remove stat box from the canvas, but keep in the histograms.
            if (doSameCharge) {
                h1D_e1e2_M_sameCharge_final[i][j]= (TH1D*)h1D_e1e2_M_sameCharge[i][j]->Clone(Form("%s_final", h1D_e1e2_M_sameCharge[i][j]->GetName()));
                h1D_e1e2_M_sameCharge_final[i][j]->SetAxisRange(final_xMin,final_xMax);
                h1D_e1e2_M_sameCharge_final[i][j]->Draw("e same");
                h1D_e1e2_M_sameCharge_final[i][j]->Write("", TObject::kOverwrite);
                h1D_e1e2_M_sameCharge_final[i][j]->SetStats(false);
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
        dielectronHistogram(argv[1], argv[2], argv[3], argv[4]);
        return 0;
    }
    else if (argc == 4) {
        dielectronHistogram(argv[1], argv[2], argv[3]);
        return 0;
    }
    else if (argc == 3) {
        dielectronHistogram(argv[1], argv[2]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./dielectronHistogram.exe <configFile> <inputFile> <outputFile> (<sampleName>)"
                << std::endl;
        return 1;
    }
}
