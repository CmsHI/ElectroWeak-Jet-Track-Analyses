// To get (( Trigger Efficiency = events passing the trigger / events which have isolated photons over a given trigger threshold ))
// Date : 2018 July 17
// Creator : Yeonju Go 

#include "../../yjUtility.h"
//#include "../phoRaaCuts/phoRaaCuts_temp.h"
#include "../TreeHeaders/ggHiNtuplizerTree.h"
const float delta2 = 0.15*0.15;
const float delta = 0.15;
///////////// BINNING ///////////////////
const double ptBins[] = {0,10,20,30,40,50,60,80,100,130,200};
const double ptBins_draw[] = {0,10,20,30,40,50,60,80,100,130,200};
const double ptBins_draw_final[] = {30,40,50,60,80,100,130};
const double ptBins_mean_pbpb[] = {44.1827,54.2862,67.7425,88.1946,111.643};
const double ptBins_mean_pp[] = {44.5213,54.188,67.5644,88.1521,111.85};
const int nPtBin = sizeof(ptBins)/sizeof(double) -1;
const double ptBins_i[] = {40,40,50,60,80,100};
const double ptBins_f[] = {130,50,60,80,100,130};
const int nPtBinIF = sizeof(ptBins_i)/sizeof(double);
const int centBins[] = {0,20,60,100,200};
const int nCentBin = sizeof(centBins)/sizeof(int) -1;
const int centBins_i[] = {0,0,20,60,100};
const int centBins_f[] = {200,20,60,100,200};
const int nCentBinIF = sizeof(centBins_i)/sizeof(int);

void getTrigEff_prescaleCorrected(TString coll="pp", TString sample = "MB", bool doIDcut=false, bool doAnalysisBin=false)
{
    cout << " :::::: reco_efficiency_withForest.C :::::: " << endl;
    TH1::SetDefaultSumw2();
    gStyle->SetOptStat(0);
    //SetyjPadStyle();


    ///////////////////////////////////////////////////////////////////////////
    // TCAHIN, GET FILE Using Forest 
    TString evtT = "ggHiNtuplizer/EventTree";
    if(sample!="photon_skim" && coll=="pp") evtT = "ggHiNtuplizerGED/EventTree";
    TString hltT = "hltanalysis/HltTree";
    TString hiT = "hiEvtAnalyzer/HiTree";
    if(sample=="photon_skim"){
        evtT = "EventTree";
        hltT = "hltTree";
        hiT = "HiEvt";
    }

    TChain * t = new TChain(Form("%s",evtT.Data()));
    TChain * t_hlt = new TChain(Form("%s",hltT.Data()));
    TChain * t_hi = new TChain(Form("%s",hiT.Data()));

    if(sample == "MB"){
        // TFile f = TFile::Open("root://xrootd.cmsaf.mit.edu://store/user/tatar/HIMinimumBias2/HIRun2015-PromptReco-v1-Run263233-263284-FOREST/0.root")
        //https://twiki.cern.ch/twiki/bin/viewauth/CMS/HiForest2015#MinimumBias_Forests
        //pbpb MB: /mnt/hadoop/cms/store/user/rbi/merged/HIMinimumBias2-HIRun2015-PromptReco-v1_forest_csjet_v1/*.root
        //pbpb MB: /mnt/hadoop/cms/store/user/tatar/HIMinimumBias2/HIRun2015-PromptReco-v1-Run263233-263284-FOREST/*.root
        //pp MB PD1: /mnt/hadoop/cms/store/user/dgulhan/MinimumBias1/HiForest_MinimumBias1_Run2015E_PromptReco_pp_5020GeV/151201_113508/0000/  
        const char* inFile = "root://xrootd.cmsaf.mit.edu://store/user/tatar/HIMinimumBias2/HIRun2015-PromptReco-v1-Run263233-263284-FOREST/0.root";
        if(coll=="pp") 
            inFile = "root://xrootd.cmsaf.mit.edu://store/user/dgulhan/MinimumBias1/HiForest_MinimumBias1_Run2015E_PromptReco_pp_5020GeV/151201_113508/0000/HiForest_0.root";
        t->Add(inFile);
        t_hlt->Add(inFile);
        t_hi->Add(inFile);

    } else if(sample == "photon_skim"){

        ///////////////////////////////////////////////////////////////////////////
        // TCAHIN, GET FILE Using Skim file 
        const char* inFile = "/u/user/goyeonju/scratch/files/photons2016/GAMMAJETFILES/2018-07-17-nominal/PbPb_Data_photonRaaSkim.root";
        if(coll=="pp") 
            inFile = "/u/user/goyeonju/scratch/files/photons2016/GAMMAJETFILES/2018-07-17-nominal/pp_Data_photonRaaSkim_test.root";
        t->Add(inFile);
        t_hlt->Add(inFile);
        t_hi->Add(inFile);

    } else if(sample == "photon_forest"){
        int nFiles(0);
        char line[1024];
        TString inFileName = "/u/user/goyeonju/ElectroWeak-Jet-Track-Analyses/ShellScripts/photonRaa/filelist/PbPb_Data_HiHardProbes_run262620to263035.list";
        if(coll=="pp") inFileName = "/u/user/goyeonju/ElectroWeak-Jet-Track-Analyses/ShellScripts/photonRaa/filelist/pp_Data_HighPtLowerPhotons.list"; 
        ifstream in(inFileName.Data());
        while (in.getline(line,1024,'\n'))
        {
            //cout << line << endl;
            const char* fname = Form("%s",line);
            //const char* fname = Form("%s/%s/%s",dir,runN[ir].data(),line);
            t->Add(fname);
            t_hi->Add(fname);
            t_hlt->Add(fname);
            nFiles++;
            cout << "Added " << fname << endl;
        } 

        if(coll=="pbpb"){
            TString inFileName2 = "/u/user/goyeonju/ElectroWeak-Jet-Track-Analyses/ShellScripts/photonRaa/filelist/PbPb_Data_HiHardProbesPhotons_run262233to263400_notFinished.list";
            ifstream in2(inFileName2.Data());
            while (in2.getline(line,1024,'\n'))
            {
                //cout << line << endl;
                const char* fname = Form("%s",line);
                //const char* fname = Form("%s/%s/%s",dir,runN[ir].data(),line);
                t->Add(fname);
                t_hi->Add(fname);
                t_hlt->Add(fname);
                nFiles++;
                cout << "Added " << fname << endl;
            } 
            TString inFileName3 = "/u/user/goyeonju/ElectroWeak-Jet-Track-Analyses/ShellScripts/photonRaa/filelist/PbPb_Data_HiHardProbesPhotons_run263406to263614.list";
            ifstream in3(inFileName3.Data());
            while (in3.getline(line,1024,'\n'))
            {
                //cout << line << endl;
                const char* fname = Form("%s",line);
                //const char* fname = Form("%s/%s/%s",dir,runN[ir].data(),line);
                t->Add(fname);
                t_hi->Add(fname);
                t_hlt->Add(fname);
                nFiles++;
                cout << "Added " << fname << endl;
            } 

        }
        cout << "total number of files = " << nFiles << endl;

    } else {
        cout << "ERROR : There is no such sample" << endl;

    }

    t->AddFriend(t_hlt);
    t->AddFriend(t_hi);


    ///////////////////////////////////////////////////////////////////////////
    // SetBranchAddress
    Int_t           nPho=0;
    std::vector<float>   *phoE=0;
    std::vector<float>   *phoEt=0;
    // std::vector<float>   *phoEtCorrected=0;
    // std::vector<float>   *phoEtCorrected_sys=0;
    std::vector<float>   *phoEta=0;
    std::vector<float>   *phoPhi=0;
    std::vector<float>   *phoR9=0;
    std::vector<float>   *phoHoverE=0;
    std::vector<float>   *phoSigmaIEtaIEta=0;
    Int_t           pho_isEle=0;
    Int_t           pho_is2015Noise=0;
    std::vector<float>   *phoSigmaIEtaIEta_2012=0;
    // std::vector<float>   *pho_sumIsoCorrected=0;
    std::vector<float>   *pho_swissCrx=0;
    std::vector<float>   *pho_seedTime=0;
    std::vector<float> *phoE3x3 = 0;
    std::vector<float> *phoE1x5 = 0;
    std::vector<float> *phoE2x5 = 0;
    std::vector<float> *phoE5x5 = 0;
    if (t->GetBranch("nPho")) t->SetBranchAddress("nPho", &nPho);
    if (t->GetBranch("phoE")) t->SetBranchAddress("phoE", &phoE);
    if (t->GetBranch("phoEt")) t->SetBranchAddress("phoEt", &phoEt);
    // if (t->GetBranch("phoEtCorrected")) t->SetBranchAddress("phoEtCorrected", &phoEtCorrected);
    // if (t->GetBranch("phoEtCorrected_sys")) t->SetBranchAddress("phoEtCorrected_sys", &phoEtCorrected_sys);
    if (t->GetBranch("phoEta")) t->SetBranchAddress("phoEta", &phoEta);
    if (t->GetBranch("phoPhi")) t->SetBranchAddress("phoPhi", &phoPhi);
    if (t->GetBranch("phoR9")) t->SetBranchAddress("phoR9", &phoR9);
    if (t->GetBranch("phoHoverE")) t->SetBranchAddress("phoHoverE", &phoHoverE);
    if (t->GetBranch("phoSigmaIEtaIEta")) t->SetBranchAddress("phoSigmaIEtaIEta", &phoSigmaIEtaIEta);
    // if (t->GetBranch("pho_isEle")) t->SetBranchAddress("pho_isEle", &pho_isEle);
    // if (t->GetBranch("pho_is2015Noise")) t->SetBranchAddress("pho_is2015Noise", &pho_is2015Noise);
    if (t->GetBranch("phoSigmaIEtaIEta_2012")) t->SetBranchAddress("phoSigmaIEtaIEta_2012", &phoSigmaIEtaIEta_2012);
    if (t->GetBranch("pho_swissCrx")) t->SetBranchAddress("pho_swissCrx", &pho_swissCrx);
    if (t->GetBranch("pho_seedTime")) t->SetBranchAddress("pho_seedTime", &pho_seedTime);
    if (t->GetBranch("phoE3x3")) t->SetBranchAddress("phoE3x3", &phoE3x3);
    if (t->GetBranch("phoE1x5")) t->SetBranchAddress("phoE1x5", &phoE1x5);
    if (t->GetBranch("phoE2x5")) t->SetBranchAddress("phoE2x5", &phoE2x5);
    if (t->GetBranch("phoE5x5")) t->SetBranchAddress("phoE5x5", &phoE5x5);

    int hiBin;
    t->SetBranchAddress("hiBin",&hiBin);

    Int_t HLT_HISinglePhoton40_Eta1p5_v1=0;
    Int_t HLT_HISinglePhoton30_Eta1p5_v1=0;
    Int_t HLT_HISinglePhoton20_Eta1p5_v1=0;
    Int_t HLT_HISinglePhoton15_Eta1p5_v1=0;
    Int_t HLT_HISinglePhoton10_Eta1p5_v1=0;
    Int_t HLT_HISinglePhoton20_Eta1p5_v2=0;
    Int_t HLT_HISinglePhoton15_Eta1p5_v2=0;
    Int_t HLT_HISinglePhoton10_Eta1p5_v2=0;
    if (t_hlt->GetBranch("HLT_HISinglePhoton40_Eta1p5_v1")) t_hlt->SetBranchAddress("HLT_HISinglePhoton40_Eta1p5_v1", &HLT_HISinglePhoton40_Eta1p5_v1);
    if (t_hlt->GetBranch("HLT_HISinglePhoton30_Eta1p5_v1")) t_hlt->SetBranchAddress("HLT_HISinglePhoton30_Eta1p5_v1", &HLT_HISinglePhoton30_Eta1p5_v1);
    if (t_hlt->GetBranch("HLT_HISinglePhoton20_Eta1p5_v1")) t_hlt->SetBranchAddress("HLT_HISinglePhoton20_Eta1p5_v1", &HLT_HISinglePhoton20_Eta1p5_v1);
    if (t_hlt->GetBranch("HLT_HISinglePhoton15_Eta1p5_v1")) t_hlt->SetBranchAddress("HLT_HISinglePhoton15_Eta1p5_v1", &HLT_HISinglePhoton15_Eta1p5_v1);
    if (t_hlt->GetBranch("HLT_HISinglePhoton10_Eta1p5_v1")) t_hlt->SetBranchAddress("HLT_HISinglePhoton10_Eta1p5_v1", &HLT_HISinglePhoton10_Eta1p5_v1);
    if (t_hlt->GetBranch("HLT_HISinglePhoton20_Eta1p5_v2")) t_hlt->SetBranchAddress("HLT_HISinglePhoton20_Eta1p5_v2", &HLT_HISinglePhoton20_Eta1p5_v2);
    if (t_hlt->GetBranch("HLT_HISinglePhoton15_Eta1p5_v2")) t_hlt->SetBranchAddress("HLT_HISinglePhoton15_Eta1p5_v2", &HLT_HISinglePhoton15_Eta1p5_v2);
    if (t_hlt->GetBranch("HLT_HISinglePhoton10_Eta1p5_v2")) t_hlt->SetBranchAddress("HLT_HISinglePhoton10_Eta1p5_v2", &HLT_HISinglePhoton10_Eta1p5_v2);


    ///////////////////////////////////////////////////////////////////////////
    // HISTOGRAM DEFINE
    Int_t nCENTBINS = nCentBinIF;
    if(coll=="pp") nCENTBINS = 1;
    Int_t nTRIG = 4;//15,20
    double trigThr[] = {15,20,30,40};
    TH1D* h1D_Num[nTRIG][nCENTBINS];//
    TH1D* h1D_Den[nTRIG][nCENTBINS];//
    TH1D* h1D_Eff[nTRIG][nCENTBINS];//

    TH1D* h1D_Num_wBaseTrig[nTRIG][nCENTBINS];//
    TH1D* h1D_Den_wBaseTrig[nTRIG][nCENTBINS];//
    TH1D* h1D_Eff_wBaseTrig[nTRIG][nCENTBINS];//
    double maxPhoPt = 130;
    double minPhoPt = 0;
    const int nPtBins = maxPhoPt-minPhoPt;

    for(Int_t j=0;j<nTRIG;++j){
    for(Int_t i=0;i<nCENTBINS;++i){
        if(doAnalysisBin)
            h1D_Num[j][i] = new TH1D(Form("num_HISinglePhoton%d_Eta1p5_v1_cent%d",(int)trigThr[j],i), ";p_{T}^{#gamma} (GeV);dN/dp_{T}", nPtBin,ptBins);
        else 
            h1D_Num[j][i] = new TH1D(Form("num_HISinglePhoton%d_Eta1p5_v1_cent%d",(int)trigThr[j],i), ";p_{T}^{#gamma} (GeV);dN/dp_{T}", nPtBins,minPhoPt,maxPhoPt);
        h1D_Den[j][i] = (TH1D*) h1D_Num[j][i]->Clone(Form("den_HISinglePhoton%d_Eta1p5_v1_cent%d",(int)trigThr[j],i));
        h1D_Eff[j][i] = (TH1D*) h1D_Num[j][i]->Clone(Form("eff_HISinglePhoton%d_Eta1p5_v1_cent%d",(int)trigThr[j],i));
        if(doAnalysisBin)
        h1D_Num_wBaseTrig[j][i] = new TH1D(Form("wBaseTrig_num_HISinglePhoton10_%d_Eta1p5_v1_cent%d",(int)trigThr[j],i), ";p_{T}^{#gamma} (GeV);dN/dp_{T}", nPtBin,ptBins);
        else 
        h1D_Num_wBaseTrig[j][i] = new TH1D(Form("wBaseTrig_num_HISinglePhoton10_%d_Eta1p5_v1_cent%d",(int)trigThr[j],i), ";p_{T}^{#gamma} (GeV);dN/dp_{T}", nPtBins,minPhoPt,maxPhoPt);
        h1D_Den_wBaseTrig[j][i] = (TH1D*) h1D_Num_wBaseTrig[j][i]->Clone(Form("wBaseTrig_den_HISinglePhoton10_%d_Eta1p5_v1_cent%d",(int)trigThr[j],i));
        h1D_Eff_wBaseTrig[j][i] = (TH1D*) h1D_Num_wBaseTrig[j][i]->Clone(Form("wBaseTrig_eff_HISinglePhoton10_%d_Eta1p5_v1_cent%d",(int)trigThr[j],i));
    } 
    }

    ///////////////////////////////////////////////////////////////////////////
    // EVENT LOOP 
    cout << "event loop has been started" << endl;
    Long64_t nentries = t->GetEntries();
    cout << "total entries = " << nentries << endl;
    for(int ientries = 0; ientries < nentries; ++ientries){
    //for(int ientries = 0; ientries < 1000; ++ientries){
        t->GetEntry(ientries);
        t_hlt->GetEntry(ientries);
        t_hi->GetEntry(ientries);
    //cout << "entry : " << ientries << endl;
        if (ientries % 20000 == 0)
            printf("current entry = %d out of %lld : %.3f %%\n", ientries, nentries, (double)ientries / nentries * 100);

        int icent = 1;
        if(coll!="pp"){
            for (; hiBin>=centBins_f[icent] && icent<nCENTBINS; ++icent);
        } else if(coll=="pp"){
            icent = 0;
        }
        //cout << "icent = " << icent << endl;
        //cout << "nPho = " << nPho << endl;

        //photon loop
        double maxPt= 0;
        for(int ipho = 0; ipho<nPho;++ipho){

            if(abs(phoEta->at(ipho)) >= 1.44) continue;
            if(doIDcut && (phoHoverE->at(ipho) >= 0.1 || phoSigmaIEtaIEta_2012->at(ipho) >= 0.010)) continue;

            bool failedSpikeRejection = 0;
            failedSpikeRejection = (phoSigmaIEtaIEta_2012->at(ipho) < 0.002 || pho_swissCrx->at(ipho) > 0.9 || TMath::Abs(pho_seedTime->at(ipho)) > 3);
            if (failedSpikeRejection) continue;

            pho_is2015Noise = 0;
            if (((*phoE3x3)[ipho] / (*phoE5x5)[ipho] > 2. / 3. - 0.03 &&
                     (*phoE3x3)[ipho] / (*phoE5x5)[ipho] < 2. / 3. + 0.03) &&
                    ((*phoE1x5)[ipho] / (*phoE5x5)[ipho] > 1. / 3. - 0.03 &&
                     (*phoE1x5)[ipho] / (*phoE5x5)[ipho] < 1. / 3. + 0.03) &&
                    ((*phoE2x5)[ipho] / (*phoE5x5)[ipho] > 2. / 3. - 0.03 &&
                     (*phoE2x5)[ipho] / (*phoE5x5)[ipho] < 2. / 3. + 0.03)) {
                pho_is2015Noise = 1;
                continue;
            }

            if(phoEt->at(ipho)>maxPt) maxPt = phoEt->at(ipho);
        }// photon loop
       
        for(Int_t j=0;j<nTRIG;++j){
            bool currTrig = 0;
            if(trigThr[j]==15){ 
                if(coll=="pp") currTrig = HLT_HISinglePhoton15_Eta1p5_v1;
                else currTrig = HLT_HISinglePhoton15_Eta1p5_v2 || HLT_HISinglePhoton15_Eta1p5_v1;
            } else if(trigThr[j]==20){ 
                if(coll=="pp") currTrig = HLT_HISinglePhoton20_Eta1p5_v1;
                else currTrig = HLT_HISinglePhoton20_Eta1p5_v2 || HLT_HISinglePhoton20_Eta1p5_v1;
            } else if(trigThr[j]==30){ 
                currTrig = HLT_HISinglePhoton30_Eta1p5_v1;
            } else if(trigThr[j]==40){ 
                currTrig = HLT_HISinglePhoton40_Eta1p5_v1;
            } else { 
                if(ientries<2) cout << "[ERROR] : There's no the trigger info" << endl;
            }
            bool baseTrig = 0;
            if(coll=="pp") baseTrig = HLT_HISinglePhoton10_Eta1p5_v1;
            else baseTrig = HLT_HISinglePhoton10_Eta1p5_v1 || HLT_HISinglePhoton10_Eta1p5_v2;

            h1D_Den[j][0]->Fill(maxPt); if(coll=="pbpb") h1D_Den[j][icent]->Fill(maxPt);
            if(currTrig==1){ h1D_Num[j][0]->Fill(maxPt); if(coll=="pbpb") h1D_Num[j][icent]->Fill(maxPt); }
            if(baseTrig==1){ h1D_Den_wBaseTrig[j][0]->Fill(maxPt); if(coll=="pbpb")  h1D_Den_wBaseTrig[j][icent]->Fill(maxPt); }
            if(baseTrig==1 && currTrig==1){ h1D_Num_wBaseTrig[j][0]->Fill(maxPt); if(coll=="pbpb")  h1D_Num_wBaseTrig[j][icent]->Fill(maxPt); }
        }
       // bool currTrig = 0;
       // if(eThr==15) currTrig = HLT_HISinglePhoton15_Eta1p5_v1;
       // else if(eThr==20) currTrig = HLT_HISinglePhoton20_Eta1p5_v1;
       // else if(eThr==30) currTrig = HLT_HISinglePhoton30_Eta1p5_v1;
       // else if(eThr==40) currTrig = HLT_HISinglePhoton40_Eta1p5_v1;
       // else{ if(ientries<2) cout << "[ERROR] : There's no " << eThr << " trigger info" << endl;}

       // h1D_Den[0]->Fill(maxPt); if(coll=="pbpb") h1D_Den[icent]->Fill(maxPt);
       // if(currTrig==1){ h1D_Num[0]->Fill(maxPt); if(coll=="pbpb")  h1D_Num[icent]->Fill(maxPt); }
       // if(HLT_HISinglePhoton10_Eta1p5_v1==1){ h1D_Den_wBaseTrig[0]->Fill(maxPt); if(coll=="pbpb")  h1D_Den_wBaseTrig[icent]->Fill(maxPt); }
       // if(HLT_HISinglePhoton10_Eta1p5_v1==1 && currTrig==1){ h1D_Num_wBaseTrig[0]->Fill(maxPt); if(coll=="pbpb")  h1D_Num_wBaseTrig[icent]->Fill(maxPt); }

    }//event loop

    for(Int_t j=0;j<nTRIG;++j){
        for(Int_t i=0;i<nCENTBINS;++i){ 
            h1D_Eff[j][i] -> Divide(h1D_Num[j][i],h1D_Den[j][i],1.,1.,"B");
            h1D_Eff_wBaseTrig[j][i] -> Divide(h1D_Num_wBaseTrig[j][i],h1D_Den_wBaseTrig[j][i],1.,1.,"B");
        }
    }

    TFile* fout = new TFile(Form("output/%s_trigger_efficiency_%s_doIDcut%d.root",coll.Data(),sample.Data(),(int)doIDcut),"RECREATE");   
    fout->cd();
    for(Int_t j=0;j<nTRIG;++j){
    for(Int_t i=0;i<nCENTBINS;++i){ 
        h1D_Num[j][i]->Write();
        h1D_Den[j][i]->Write();
        h1D_Eff[j][i]->Write();
        h1D_Num_wBaseTrig[j][i]->Write();
        h1D_Den_wBaseTrig[j][i]->Write();
        h1D_Eff_wBaseTrig[j][i]->Write();
    }
    }
    fout->Close();
}
