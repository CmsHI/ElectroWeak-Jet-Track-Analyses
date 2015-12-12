#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TCut.h>
#include <TH1D.h>

#include <iostream>

#include "../CutConfigurations/interface/CutConfigurationsParser.h"
#include "../TreeHeaders/CutConfigurationTree.h"

void dielectronHistogram(const char* configFile, const char* inputFile, const char* outputFile = "dielectronHistogram.root", const char* sampleName = "");

void dielectronHistogram(const char* configFile, const char* inputFile, const char* outputFile, const char* sampleName)
{
    std::cout<<"running dielectronHistogram()" <<std::endl;
    std::cout<<"configFile = "<< configFile <<std::endl;
    std::cout<<"inputFile  = "<< inputFile <<std::endl;
    std::cout<<"outputFile = "<< outputFile <<std::endl;

    TFile *input = new TFile(inputFile);
    TTree *tHLT = (TTree*)input->Get("HltTree");
    TTree *t_diele = (TTree*)input->Get("dielectron");

    t_diele->AddFriend(tHLT,"tHLT");

    TFile* output = new TFile(outputFile, "UPDATE");

    CutConfiguration config = CutConfigurationsParser::Parse(configFile);
    TTree* configTree = setupConfigurationTreeForWriting(config);
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
    if (config.isValid) {
        str_electronID = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].s[CUTS::ELE::k_electronID].c_str();
        str_trigger = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].s[CUTS::ELE::k_trigger].c_str();

        // Barrel
        eleSigmaIEtaIEta_2012_EB = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleSigmaIEtaIEta_2012_EB];
        eledEtaAtVtx_abs_EB = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eledEtaAtVtx_abs_EB];
        eledPhiAtVtx_abs_EB = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eledPhiAtVtx_abs_EB];
        eleHoverE_EB = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleHoverE_EB];
        eleEoverPInv_EB = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleEoverPInv_EB];
        eleD0_abs_EB = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleD0_abs_EB];
        eleDz_abs_EB = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleDz_abs_EB];
        eleMissHits_EB = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].i[CUTS::ELE::k_eleMissHits_EB];

        // Endcap
        eleSigmaIEtaIEta_2012_EE = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleSigmaIEtaIEta_2012_EE];
        eledEtaAtVtx_abs_EE = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eledEtaAtVtx_abs_EE];
        eledPhiAtVtx_abs_EE = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eledPhiAtVtx_abs_EE];
        eleHoverE_EE = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleHoverE_EE];
        eleEoverPInv_EE = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleEoverPInv_EE];
        eleD0_abs_EE = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleD0_abs_EE];
        eleDz_abs_EE = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].f[CUTS::ELE::k_eleDz_abs_EE];
        eleMissHits_EE = config.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].i[CUTS::ELE::k_eleMissHits_EE];
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

    // verbose about configuration
    std::cout<<"Configuration :"<<std::endl;
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
    float bins_eta_gt[nBins_eta] = {-1,      -1, 1.4791, 1.4791};    // All ECAL, Barrel, Endcap1, Endcap2
    float bins_eta_lt[nBins_eta] = {2.4, 1.4791,    2.4, 2};

    std::string histNames_e1e2_M[nBins_pt][nBins_eta];

    int numEntries[nBins_pt][nBins_eta];

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

            TCut selection_event = Form("%s == 1", str_trigger.c_str());
            TCut selection =  "";

//            selection = selection && "eleSigmaIEtaIEta_1>0.002       && eleSigmaIEtaIEta_2>0.002";  // spike rejection
//            selection = selection && "eleSigmaIPhiIPhi_1>0.002       && eleSigmaIPhiIPhi_2>0.002";  // spike rejection

            TCut selection_EB = "";
            TCut selection_EE = "";
            selection_EB = selection_EB && Form("eleSigmaIEtaIEta_2012_1 < %f && eleSigmaIEtaIEta_2012_2 < %f", eleSigmaIEtaIEta_2012_EB, eleSigmaIEtaIEta_2012_EB);
            selection_EB = selection_EB && Form("abs(eledEtaAtVtx_1) < %f && abs(eledEtaAtVtx_2) < %f", eledEtaAtVtx_abs_EB, eledEtaAtVtx_abs_EB);
            selection_EB = selection_EB && Form("abs(eledPhiAtVtx_1) < %f && abs(eledPhiAtVtx_2) < %f", eledPhiAtVtx_abs_EB, eledPhiAtVtx_abs_EB);
            selection_EB = selection_EB && Form("eleHoverE_1 < %f && eleHoverE_2 < %f", eleHoverE_EB, eleHoverE_EB);
            selection_EB = selection_EB && Form("eleEoverPInv_1 < %f && eleEoverPInv_2 < %f", eleEoverPInv_EB, eleEoverPInv_EB);
            selection_EB = selection_EB && Form("abs(eleD0_1) < %f && abs(eleD0_2) < %f", eleD0_abs_EB, eleD0_abs_EB);
            selection_EB = selection_EB && Form("abs(eleDz_1) < %f && abs(eleDz_2) < %f", eleDz_abs_EB, eleDz_abs_EB);
            selection_EB = selection_EB && Form("eleMissHits_1 <= %d && eleMissHits_2 <= %d", eleMissHits_EB, eleMissHits_EB);

            selection_EE = selection_EE && Form("eleSigmaIEtaIEta_2012_1 < %f && eleSigmaIEtaIEta_2012_2 < %f", eleSigmaIEtaIEta_2012_EE, eleSigmaIEtaIEta_2012_EE);
            selection_EE = selection_EE && Form("abs(eledEtaAtVtx_1) < %f && abs(eledEtaAtVtx_2) < %f", eledEtaAtVtx_abs_EE, eledEtaAtVtx_abs_EE);
            selection_EE = selection_EE && Form("abs(eledPhiAtVtx_1) < %f && abs(eledPhiAtVtx_2) < %f", eledPhiAtVtx_abs_EE, eledPhiAtVtx_abs_EE);
            selection_EE = selection_EE && Form("eleHoverE_1 < %f && eleHoverE_2 < %f", eleHoverE_EE, eleHoverE_EE);
            selection_EE = selection_EE && Form("eleEoverPInv_1 < %f && eleEoverPInv_2 < %f", eleEoverPInv_EE, eleEoverPInv_EE);
            selection_EE = selection_EE && Form("abs(eleD0_1) < %f && abs(eleD0_2) < %f", eleD0_abs_EE, eleD0_abs_EE);
            selection_EE = selection_EE && Form("abs(eleDz_1) < %f && abs(eleDz_2) < %f", eleDz_abs_EE, eleDz_abs_EE);
            selection_EE = selection_EE && Form("eleMissHits_1 <= %d && eleMissHits_2 <= %d", eleMissHits_EE, eleMissHits_EE);

            if (j == 0) {
                TCut selection_EB_eta = Form(" abs(eleEta_1) < %f && abs(eleEta_2) < %f", bins_eta_lt[1], bins_eta_lt[1]);
                selection_EB = selection_EB && selection_EB_eta;
                TCut selection_EE_eta = Form(" abs(eleEta_1) > %f && abs(eleEta_2) > %f", bins_eta_lt[1], bins_eta_lt[1]);
                selection_EE_eta = selection_EE_eta && Form(" abs(eleEta_1) < %f && abs(eleEta_2) < %f", bins_eta_lt[0], bins_eta_lt[0]);
                selection_EE = selection_EE && selection_EE_eta;
                TCut selection_EB_EE = selection_EB || selection_EE;
                selection = selection && selection_EB_EE;
            }
            if (j == 1) // Barrel,  |eta supercluster| <= 1.479
            {
                selection = selection && selection_EB;
                selection = selection && Form(" abs(eleEta_1) > %f && abs(eleEta_2) > %f", bins_eta_gt[j], bins_eta_gt[j]);
                selection = selection && Form(" abs(eleEta_1) < %f && abs(eleEta_2) < %f", bins_eta_lt[j], bins_eta_lt[j]);
            }
            if ( j>1 )  // Endcap,  1.479 < |eta supercluster| < 2.5
            {
                selection = selection && selection_EE;
                selection = selection && Form(" abs(eleEta_1) > %f && abs(eleEta_2) > %f", bins_eta_gt[j], bins_eta_gt[j]);
                selection = selection && Form(" abs(eleEta_1) < %f && abs(eleEta_2) < %f", bins_eta_lt[j], bins_eta_lt[j]);
            }
            selection = selection && Form("elePt_1 > %d && elePt_2 > %d", bins_pt[i], bins_pt[i]);
            selection = selection && selection_event;

            TCut selection_sameCh = selection && "eleCharge_1 == eleCharge_2";
            selection             = selection && "eleCharge_1 != eleCharge_2";

            // verbose
            std::cout<< "[i][j] = " << i << " , " << j <<std::endl;
            std::cout<< "selection = " << selection.GetTitle() <<std::endl;
            std::cout<< "h1D_e1e2_M[i][j]->GetName() = " << h1D_e1e2_M[i][j]->GetName() <<std::endl;
            numEntries[i][j] = t_diele->GetEntries(selection.GetTitle());
            std::cout<< "numEntries[i][j] = " << numEntries[i][j] <<std::endl;

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
