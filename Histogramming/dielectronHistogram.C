#include <TFile.h>
#include <TTree.h>
#include <TMath.h>
#include <TCut.h>
#include <TH1D.h>
#include <TCanvas.h>

#include <iostream>

#include "../TreeHeaders/CutConfigurationTree.h"
#include "../Utilities/interface/InputConfigurationParser.h"
#include "../Utilities/interface/CutConfigurationParser.h"

void dielectronHistogram(const TString configFile, const TString inputFile, const TString outputFile = "dielectronHistogram.root");

void dielectronHistogram(const TString configFile, const TString inputFile, const TString outputFile)
{
    std::cout<<"running dielectronHistogram()" <<std::endl;
    std::cout<<"configFile = "<< configFile.Data() <<std::endl;
    std::cout<<"inputFile  = "<< inputFile.Data() <<std::endl;
    std::cout<<"outputFile = "<< outputFile.Data() <<std::endl;

    TFile *input = new TFile(inputFile);
    TTree *tHLT = (TTree*)input->Get("HltTree");
    TTree *tHiEvt = (TTree*)input->Get("HiEvt");       // HiEvt tree will be placed in PP forest as well.
    TTree *t_diele = (TTree*)input->Get("dielectron");

    t_diele->AddFriend(tHLT,"tHLT");
    t_diele->AddFriend(tHiEvt,"HiEvt");

    TFile* output = new TFile(outputFile, "UPDATE");

    InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

    // input configuration
    int collision;
    if (configInput.isValid) {
        collision = configInput.proc[INPUT::kHISTOGRAM].i[INPUT::k_collisionType];
    }
    else {
        collision = COLL::kPP;
    }
    // verbose about input configuration
    std::cout<<"Input Configuration :"<<std::endl;
    const char* collisionName =  getCollisionTypeName((COLL::TYPE)collision).c_str();
    std::cout << "collision = " << collisionName << std::endl;

    bool isHI = collisionIsHI((COLL::TYPE)collision);

    TTree* configTree = setupConfigurationTreeForWriting(configCuts);

    // observable bins
    std::vector<int> bins_pt[2];          // array of vectors for electron pt bins, each array element is a vector.
    std::vector<int> bins_hiBin[2];     // array of vectors for hiBin bins, each array element is a vector.
    // event cuts/weights
    int doEventWeight;
    std::string eventWeight;    // weight to be used for histogram entries
                                // current purpose of this variable is for weighting events from MC samples.
    // Z boson cuts
    float massMin;
    float massMax;
    float zPt;
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
        bins_pt[0] = ConfigurationParser::ParseListInteger(
                configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].s[CUTS::ELE::k_bins_pt_gt]);
        bins_pt[1] = ConfigurationParser::ParseListInteger(
                configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kELECTRON].s[CUTS::ELE::k_bins_pt_lt]);
        bins_hiBin[0] = ConfigurationParser::ParseListInteger(
                configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_gt]);
        bins_hiBin[1] = ConfigurationParser::ParseListInteger(
                configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_lt]);

        doEventWeight = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].i[CUTS::EVT::k_doEventWeight];
        eventWeight = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_eventWeight].c_str();

        massMin = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kZBOSON].f[CUTS::ZBO::k_massMin];
        massMax = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kZBOSON].f[CUTS::ZBO::k_massMax];
        zPt = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kZBOSON].f[CUTS::ZBO::k_pt];

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
        massMin = 60;
        massMax = 120;
        zPt = 0;

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

    int nBins_pt = bins_pt[0].size();         // assume <myvector>[0] and <myvector>[1] have the same size.
    int nBins_hiBin = bins_hiBin[0].size();     // assume <myvector>[0] and <myvector>[1] have the same size.
    // verbose about cut configuration
    std::cout<<"Cut Configuration :"<<std::endl;
    std::cout << "nBins_pt = " << nBins_pt << std::endl;
    for (int i=0; i<nBins_pt; ++i) {
        std::cout << Form("bins_pt[%d] = [%d, %d)", i, bins_pt[0].at(i), bins_pt[1].at(i)) << std::endl;
    }
    std::cout << "nBins_hiBin = " << nBins_hiBin << std::endl;
    for (int i=0; i<nBins_hiBin; ++i) {
        std::cout << Form("bins_hiBin[%d] = [%d, %d)", i, bins_hiBin[0].at(i), bins_hiBin[1].at(i)) << std::endl;
    }

    std::cout<<"doEventWeight = "<<doEventWeight<<std::endl;
    if (doEventWeight > 0) {
        std::cout<<"eventWeight = "<<eventWeight.c_str()<<std::endl;
    }

    std::cout<<"massMin = "<< massMin <<std::endl;
    std::cout<<"massMax = "<< massMax <<std::endl;
    std::cout<<"zPt = "    << zPt <<std::endl;

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

    // eta bins are not part of cut configuration
    const int nBins_eta = 4;
    // https://twiki.cern.ch/twiki/bin/view/CMS/ElectronPbPb5TeV?rev=5#1_Selection_for_all_centrality_r
    // these are eleSCEta selection, not eleEta (08.03.2016 : switch from eleEta to eleSCEta for rapidity binnings)
    float EB_end = 1.4442;
    float EE_start = 1.566;
    if (!isHI)  {
        EB_end = 1.4791;
        EE_start = 1.4791;
    }
    float bins_eta_gt[nBins_eta] = {-1,  -1,     EE_start, EE_start};    // All ECAL, Barrel, Endcap1, Endcap2
    float bins_eta_lt[nBins_eta] = {2.5, EB_end, 2.5,   2};

    std::vector<std::string> observableNames {"M", "Pt", "Eta", "Phi", "hiBin"};
    std::vector<bool> doMassSelection    {false, true,  true, true, true};
    std::vector<bool> doZptSelection     {true,  false, true, true, true};
    std::vector<std::string> histFormulas{"diEleM", "diElePt", "diEleEta", "diElePhi", "hiBin"};
    std::vector<std::string> histTitleX  {"M^{ee} (GeV/c^{2})", "p^{ee}_{T}", "#eta^{ee}", "#phi^{ee}", "hiBin"};
    std::vector<std::string> histTitleY  {"Entries / (2 GeV/c^{2})",
                                                                     "#frac{1}{N} #frac{dN}{dp^{ee}_{T}}",
                                                                     "#frac{1}{N} #frac{dN}{d#eta^{ee}_{T}}",
                                                                     "#frac{1}{N} #frac{dN}{d#phi^{ee}_{T}}",
                                                                     "Entries"};
    std::vector<int>     nBinsx{100, 60,  20,    20,          20};
    std::vector<double>  xlow  {0,   0,   -4, -TMath::Pi(), 0};
    std::vector<double>  xup   {200, 300,  4,  TMath::Pi(),  200};
    std::vector<double>  xlow_final{60,  0,  -4, -TMath::Pi(), 0};
    std::vector<double>  xup_final {120, 200, 4,  TMath::Pi(), 200};

    TH1::SetDefaultSumw2();
    int nHistFormulas = histFormulas.size();
    TH1D* h1D[nHistFormulas][nBins_eta][nBins_pt][nBins_hiBin];
    TH1D* h1D_sameCharge[nHistFormulas][nBins_eta][nBins_pt][nBins_hiBin];       // histogram for same charge
    std::string histNames[nHistFormulas][nBins_eta][nBins_pt][nBins_hiBin];

    int numEntries[nHistFormulas][nBins_eta][nBins_pt][nBins_hiBin];
    int numEntriesMassWindow[nHistFormulas][nBins_eta][nBins_pt][nBins_hiBin];   // number of entries within given mass window, e.g. 60-120

    // x-axis range : 60-120
    TH1D* h1D_final[nHistFormulas][nBins_eta][nBins_pt][nBins_hiBin];
    TH1D* h1D_sameCharge_final[nHistFormulas][nBins_eta][nBins_pt][nBins_hiBin];  // histogram for same charge

    for (int i=0; i<nHistFormulas; ++i){
        for(int iEta=0; iEta<nBins_eta; ++iEta){
            for (int iPt=0; iPt<nBins_pt; ++iPt){
                for(int iHibin=0; iHibin<nBins_hiBin; ++iHibin){

                    std::string histName = Form("%s_etaBin%d_ptBin%d_hiBin%d",
                                                 observableNames.at(i).c_str(), iEta, iPt, iHibin);
                    histNames[i][iEta][iPt][iHibin] = histName.c_str();

                    //h1D[i][j] = new TH1D(Form("h1D_%s", histNames_M[i][j].c_str()),"",100,0,200);
                    h1D[i][iEta][iPt][iHibin] = new TH1D(Form("h1D_%s", histName.c_str()),"",
                            nBinsx.at(i), xlow.at(i), xup.at(i));
                }
            }
        }
    }

    bool doSameCharge = true;
    for(int i=0; i<nHistFormulas; ++i){
        for(int iEta=0; iEta<nBins_eta; ++iEta){
            for(int iPt=0; iPt<nBins_pt; ++iPt){
                for(int iHiBin=0; iHiBin<nBins_hiBin; ++iHiBin){

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

                        TCut selection_EB_eta = Form("abs(eleSCEta_%d) < %f" ,iEle+1 ,bins_eta_lt[1]);
                        TCut selection_EE_eta = Form("abs(eleSCEta_%d) > %f && abs(eleSCEta_%d) < %f" ,iEle+1 ,bins_eta_gt[2] ,iEle+1 ,bins_eta_lt[2]);

                        selections_EB[iEle] = selections_EB[iEle] && selection_EB_eta;
                        selections_EE[iEle] = selections_EE[iEle] && selection_EE_eta;
                    }

                    if (iEta == 0) {
                        selections[0] = selections_EB[0] || selections_EE[0];
                        selections[1] = selections_EB[1] || selections_EE[1];
                    }
                    if (iEta == 1) // Barrel,  |eta supercluster| < 1.4442
                    {
                        selections[0] = selections_EB[0];
                        selections[1] = selections_EB[1];
                    }
                    if ( iEta>1 )  // Endcap,  1.566 < |eta supercluster| < 2.5
                    {
                        selections[0] = selections_EE[0];
                        selections[1] = selections_EE[1];
                    }

                    TCut selectionElePt = Form("elePt_1 > %d && elePt_2 > %d", bins_pt[0].at(iPt), bins_pt[0].at(iPt));
                    if (bins_pt[1].at(iPt) >= 0)
                        selectionElePt = selectionElePt && Form("elePt_1 <= %d && elePt_2 <= %d", bins_pt[1].at(iPt), bins_pt[1].at(iPt));

                    // Z cuts were applied in the analysis code
                    // Z selection
                    TCut selectionZ = "1";
                    TCut selectionZpt = Form("diElePt >= %f", zPt);
                    TCut selectionZmass = Form("diEleM >= %f && diEleM < %f", massMin, massMax);
                    if (doZptSelection.at(i)) selectionZ = selectionZ && selectionZpt;
                    if (doMassSelection.at(i)) selectionZ = selectionZ && selectionZmass;

                    TCut selection_hiBin = Form("hiBin >= %d && hiBin < %d", bins_hiBin[0].at(iHiBin), bins_hiBin[1].at(iHiBin));
                    TCut selection_event = Form("%s == 1", str_trigger.c_str());

                    TCut selection =  "";
                    selection = selection && selection_event;
                    selection = selection && selection_hiBin;
                    selection = selection && selections[0] && selections[1];
                    selection = selection && selectionElePt;
                    selection = selection && selectionZ;

                    TCut selection_sameCh = selection && "eleCharge_1 == eleCharge_2";
                    selection             = selection && "eleCharge_1 != eleCharge_2";

                    // verbose
                    std::cout<< Form("[i][iEta][iPt][iHiBin] = %d, %d, %d, %d", i, iEta, iPt, iHiBin) << std::endl;
                    std::cout<< "selection = " << selection.GetTitle() <<std::endl;
                    std::cout<< "h1D[i][iEta][iPt][iHiBin]->GetName() = " << h1D[i][iEta][iPt][iHiBin]->GetName() << std::endl;
                    numEntries[i][iEta][iPt][iHiBin] = t_diele->GetEntries(selection.GetTitle());
                    std::cout<< "numEntries[i][iEta][iPt][iHiBin] = " << numEntries[i][iEta][iPt][iHiBin] << std::endl;
                    numEntriesMassWindow[i][iEta][iPt][iHiBin] = t_diele->GetEntries((selection && selectionZmass).GetTitle());
                    std::cout<< "numEntriesMassWindow[i][iEta][iPt][iHiBin] = " << numEntriesMassWindow[i][iEta][iPt][iHiBin] << std::endl;

                    if (doEventWeight > 0) {
                        selection = Form("(%s)*(%s)", eventWeight.c_str(), selection.GetTitle());
                        selection_sameCh = Form("(%s)*(%s)", eventWeight.c_str(), selection_sameCh.GetTitle());
                    }

                    std::string histoTitle_eta = Form("|#eta^{e#pm}| < %.2f", bins_eta_lt[iEta]);
                    if (bins_eta_gt[iEta] >= 0)  histoTitle_eta = Form("%.2f< |#eta^{e#pm}| <%.1f", bins_eta_gt[iEta], bins_eta_lt[iEta]);

                    std::string histoTitle_pt = Form("p^{e#pm}_{T} > %d", bins_pt[0].at(iPt));
                    if (bins_pt[1].at(iPt) >= 0)  histoTitle_pt = Form("%d < p^{e#pm}_{T} < %d", bins_pt[0].at(iPt), bins_pt[1].at(iPt));

                    std::string histoTitle_hiBin = Form("%d-%d %%", bins_hiBin[0].at(iHiBin)/2, bins_hiBin[1].at(iHiBin)/2);

                    std::string histoTitle = "";
                    if (histoTitle_eta.size() > 0)   histoTitle = histoTitle_eta.c_str();
                    if (histoTitle_pt.size() > 0)    histoTitle = Form("%s, %s", histoTitle.c_str(), histoTitle_pt.c_str());
                    if (histoTitle_hiBin.size() > 0) histoTitle = Form("%s, %s", histoTitle.c_str(), histoTitle_hiBin.c_str());

                    h1D[i][iEta][iPt][iHiBin]->SetTitle(Form("%s;%s;%s", histoTitle.c_str(), histTitleX.at(i).c_str(), histTitleY.at(i).c_str()));
                    h1D[i][iEta][iPt][iHiBin]->SetMarkerStyle(kFullCircle);
                    h1D[i][iEta][iPt][iHiBin]->SetMarkerColor(kBlack);
                    h1D_sameCharge[i][iEta][iPt][iHiBin] = (TH1D*)h1D[i][iEta][iPt][iHiBin]->Clone(Form("%s_sameCharge",
                            h1D[i][iEta][iPt][iHiBin]->GetName()));
                    h1D_sameCharge[i][iEta][iPt][iHiBin]->SetMarkerStyle(kOpenCircle);

                    c->SetName(Form("cnv_%s", histNames[i][iEta][iPt][iHiBin].c_str()));
                    c->cd();
                    t_diele->Draw(Form("%s >> %s", histFormulas.at(i).c_str() ,h1D[i][iEta][iPt][iHiBin]->GetName()), selection.GetTitle() ,"goff");
                    h1D[i][iEta][iPt][iHiBin]->Draw("e");
                    h1D[i][iEta][iPt][iHiBin]->Write("", TObject::kOverwrite);
                    h1D[i][iEta][iPt][iHiBin]->SetStats(false);     // remove stat box from the canvas, but keep in the histograms.
                    if (doSameCharge) {
                        t_diele->Draw(Form("%s >> %s", histFormulas.at(i).c_str() ,h1D_sameCharge[i][iEta][iPt][iHiBin]->GetName()), selection_sameCh.GetTitle(), "goff");
                        h1D_sameCharge[i][iEta][iPt][iHiBin]->Draw("e same");
                        h1D_sameCharge[i][iEta][iPt][iHiBin]->Write("", TObject::kOverwrite);
                        h1D_sameCharge[i][iEta][iPt][iHiBin]->SetStats(false);
                    }
                    c->Write("", TObject::kOverwrite);
                    c->Clear();

                    c->SetName(Form("cnv_%s_final", histNames[i][iEta][iPt][iHiBin].c_str()));
                    c->cd();
                    h1D_final[i][iEta][iPt][iHiBin] = (TH1D*)h1D[i][iEta][iPt][iHiBin]->Clone(Form("%s_final", h1D[i][iEta][iPt][iHiBin]->GetName()));
                    h1D_final[i][iEta][iPt][iHiBin]->SetAxisRange(xlow_final.at(i), xup_final.at(i));
                    h1D_final[i][iEta][iPt][iHiBin]->Draw("e");
                    h1D_final[i][iEta][iPt][iHiBin]->Write("", TObject::kOverwrite);
                    h1D_final[i][iEta][iPt][iHiBin]->SetStats(false);     // remove stat box from the canvas, but keep in the histograms.
                    if (doSameCharge) {
                        h1D_sameCharge_final[i][iEta][iPt][iHiBin] = (TH1D*)h1D_sameCharge[i][iEta][iPt][iHiBin]->Clone(Form("%s_final", h1D_sameCharge[i][iEta][iPt][iHiBin]->GetName()));
                        h1D_sameCharge_final[i][iEta][iPt][iHiBin]->SetAxisRange(xlow_final.at(i), xup_final.at(i));
                        h1D_sameCharge_final[i][iEta][iPt][iHiBin]->Draw("e same");
                        h1D_sameCharge_final[i][iEta][iPt][iHiBin]->Write("", TObject::kOverwrite);
                        h1D_sameCharge_final[i][iEta][iPt][iHiBin]->SetStats(false);
                    }
                    c->Write("", TObject::kOverwrite);
                    c->Clear();
                }
            }
        }
    }

    configTree->Write("", TObject::kOverwrite);

    output->Write("", TObject::kOverwrite);
    output->Close();
    input->Close();
}

int main(int argc, char** argv)
{
    if (argc == 4) {
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
