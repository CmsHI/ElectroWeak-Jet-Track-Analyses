#ifndef TREEHEADERS_HIEVTTREE_H_
#define TREEHEADERS_HIEVTTREE_H_

#include <TTree.h>
#include <TBranch.h>

#include <vector>

class hiEvt {
public :
    hiEvt(){
        ttbar_w = 0;
        npus = 0;
        tnpus = 0;
    };
    ~hiEvt(){};
    void setupTreeForReading(TTree *t);
    void setupTreeForWriting(TTree *t);

    // Declaration of leaf types
    UInt_t          run;
    ULong64_t       evt;
    UInt_t          lumi;
    Float_t         vx;
    Float_t         vy;
    Float_t         vz;
    Float_t         Npart;
    Float_t         Ncoll;
    Float_t         Nhard;
    Float_t         phi0;
    Float_t         b;
    Int_t           Ncharged;
    Int_t           NchargedMR;
    Float_t         MeanPt;
    Float_t         MeanPtMR;
    Float_t         EtMR;
    Int_t           NchargedPtCut;
    Int_t           NchargedPtCutMR;
    Int_t           ProcessID;
    Float_t         pthat;
    Float_t         weight;
    Float_t         alphaQCD;
    Float_t         alphaQED;
    Float_t         qScale;
    Int_t           nMEPartons;
    Int_t           nMEPartonsFiltered;
    std::vector<float>   *ttbar_w;
    std::vector<int>     *npus;
    std::vector<float>   *tnpus;
    Int_t           hiBin;
    Float_t         hiHF;
    Float_t         hiHFplus;
    Float_t         hiHFminus;
    Float_t         hiHFplusEta4;
    Float_t         hiHFminusEta4;
    Float_t         hiZDC;
    Float_t         hiZDCplus;
    Float_t         hiZDCminus;
    Float_t         hiHFhit;
    Float_t         hiHFhitPlus;
    Float_t         hiHFhitMinus;
    Float_t         hiET;
    Float_t         hiEE;
    Float_t         hiEB;
    Float_t         hiEEplus;
    Float_t         hiEEminus;
    Int_t           hiNpix;
    Int_t           hiNpixelTracks;
    Int_t           hiNtracks;
    Int_t           hiNtracksPtCut;
    Int_t           hiNtracksEtaCut;
    Int_t           hiNtracksEtaPtCut;
    Int_t           hiNevtPlane;
    Float_t         hiEvtPlanes[29];   //[hiNevtPlane]

    // List of branches
    TBranch        *b_run;   //!
    TBranch        *b_evt;   //!
    TBranch        *b_lumi;   //!
    TBranch        *b_vx;   //!
    TBranch        *b_vy;   //!
    TBranch        *b_vz;   //!
    TBranch        *b_Npart;   //!
    TBranch        *b_Ncoll;   //!
    TBranch        *b_Nhard;   //!
    TBranch        *b_NPhi0;   //!
    TBranch        *b_b;   //!
    TBranch        *b_Ncharged;   //!
    TBranch        *b_NchargedMR;   //!
    TBranch        *b_MeanPt;   //!
    TBranch        *b_MeanPtMR;   //!
    TBranch        *b_EtMR;   //!
    TBranch        *b_NchargedPtCut;   //!
    TBranch        *b_NchargedPtCutMR;   //!
    TBranch        *b_ProcessID;   //!
    TBranch        *b_pthat;   //!
    TBranch        *b_weight;   //!
    TBranch        *b_alphaQCD;   //!
    TBranch        *b_alphaQED;   //!
    TBranch        *b_qScale;   //!
    TBranch        *b_nMEPartons;   //!
    TBranch        *b_nMEPartonsFiltered;   //!
    TBranch        *b_ttbar_w;   //!
    TBranch        *b_npus;   //!
    TBranch        *b_tnpus;   //!
    TBranch        *b_hiBin;   //!
    TBranch        *b_hiHF;   //!
    TBranch        *b_hiHFplus;   //!
    TBranch        *b_hiHFminus;   //!
    TBranch        *b_hiHFplusEta4;   //!
    TBranch        *b_hiHFminusEta4;   //!
    TBranch        *b_hiZDC;   //!
    TBranch        *b_hiZDCplus;   //!
    TBranch        *b_hiZDCminus;   //!
    TBranch        *b_hiHFhit;   //!
    TBranch        *b_hiHFhitPlus;   //!
    TBranch        *b_hiHFhitMinus;   //!
    TBranch        *b_hiET;   //!
    TBranch        *b_hiEE;   //!
    TBranch        *b_hiEB;   //!
    TBranch        *b_hiEEplus;   //!
    TBranch        *b_hiEEminus;   //!
    TBranch        *b_hiNpix;   //!
    TBranch        *b_hiNpixelTracks;   //!
    TBranch        *b_hiNtracks;   //!
    TBranch        *b_hiNtracksPtCut;   //!
    TBranch        *b_hiNtracksEtaCut;   //!
    TBranch        *b_hiNtracksEtaPtCut;   //!
    TBranch        *b_hiNevtPlane;   //!
    TBranch        *b_hiEvtPlanes;   //!

};

void hiEvt::setupTreeForReading(TTree *t)
{
    b_run = 0;
    b_evt = 0;
    b_lumi = 0;
    b_vx = 0;
    b_vy = 0;
    b_vz = 0;
    b_Npart = 0;
    b_Ncoll = 0;
    b_Nhard = 0;
    b_NPhi0 = 0;
    b_b = 0;
    b_Ncharged = 0;
    b_NchargedMR = 0;
    b_MeanPt = 0;
    b_MeanPtMR = 0;
    b_EtMR = 0;
    b_NchargedPtCut = 0;
    b_NchargedPtCutMR = 0;
    b_ProcessID = 0;
    b_pthat = 0;
    b_weight = 0;
    b_alphaQCD = 0;
    b_alphaQED = 0;
    b_qScale = 0;
    b_nMEPartons = 0;
    b_nMEPartonsFiltered = 0;
    b_ttbar_w = 0;
    b_npus = 0;
    b_tnpus = 0;
    b_hiBin = 0;
    b_hiHF = 0;
    b_hiHFplus = 0;
    b_hiHFminus = 0;
    b_hiHFplusEta4 = 0;
    b_hiHFminusEta4 = 0;
    b_hiZDC = 0;
    b_hiZDCplus = 0;
    b_hiZDCminus = 0;
    b_hiHFhit = 0;
    b_hiHFhitPlus = 0;
    b_hiHFhitMinus = 0;
    b_hiET = 0;
    b_hiEE = 0;
    b_hiEB = 0;
    b_hiEEplus = 0;
    b_hiEEminus = 0;
    b_hiNpix = 0;
    b_hiNpixelTracks = 0;
    b_hiNtracks = 0;
    b_hiNtracksPtCut = 0;
    b_hiNtracksEtaCut = 0;
    b_hiNtracksEtaPtCut = 0;
    b_hiNevtPlane = 0;
    b_hiEvtPlanes = 0;

    // Set branch addresses and branch pointers
    if (t->GetBranch("run"))  t->SetBranchAddress("run", &run, &b_run);
    if (t->GetBranch("evt"))  t->SetBranchAddress("evt", &evt, &b_evt);
    if (t->GetBranch("lumi"))  t->SetBranchAddress("lumi", &lumi, &b_lumi);
    if (t->GetBranch("vx"))  t->SetBranchAddress("vx", &vx, &b_vx);
    if (t->GetBranch("vy"))  t->SetBranchAddress("vy", &vy, &b_vy);
    if (t->GetBranch("vz"))  t->SetBranchAddress("vz", &vz, &b_vz);
    if (t->GetBranch("Npart"))  t->SetBranchAddress("Npart", &Npart, &b_Npart);
    if (t->GetBranch("Ncoll"))  t->SetBranchAddress("Ncoll", &Ncoll, &b_Ncoll);
    if (t->GetBranch("Nhard"))  t->SetBranchAddress("Nhard", &Nhard, &b_Nhard);
    if (t->GetBranch("phi0"))  t->SetBranchAddress("phi0", &phi0, &b_NPhi0);
    if (t->GetBranch("b"))  t->SetBranchAddress("b", &b, &b_b);
    if (t->GetBranch("Ncharged"))  t->SetBranchAddress("Ncharged", &Ncharged, &b_Ncharged);
    if (t->GetBranch("NchargedMR"))  t->SetBranchAddress("NchargedMR", &NchargedMR, &b_NchargedMR);
    if (t->GetBranch("MeanPt"))  t->SetBranchAddress("MeanPt", &MeanPt, &b_MeanPt);
    if (t->GetBranch("MeanPtMR"))  t->SetBranchAddress("MeanPtMR", &MeanPtMR, &b_MeanPtMR);
    if (t->GetBranch("EtMR"))  t->SetBranchAddress("EtMR", &EtMR, &b_EtMR);
    if (t->GetBranch("NchargedPtCut"))  t->SetBranchAddress("NchargedPtCut", &NchargedPtCut, &b_NchargedPtCut);
    if (t->GetBranch("NchargedPtCutMR"))  t->SetBranchAddress("NchargedPtCutMR", &NchargedPtCutMR, &b_NchargedPtCutMR);
    if (t->GetBranch("ProcessID"))  t->SetBranchAddress("ProcessID", &ProcessID, &b_ProcessID);
    if (t->GetBranch("pthat"))  t->SetBranchAddress("pthat", &pthat, &b_pthat);
    if (t->GetBranch("weight"))  t->SetBranchAddress("weight", &weight, &b_weight);
    if (t->GetBranch("alphaQCD"))  t->SetBranchAddress("alphaQCD", &alphaQCD, &b_alphaQCD);
    if (t->GetBranch("alphaQED"))  t->SetBranchAddress("alphaQED", &alphaQED, &b_alphaQED);
    if (t->GetBranch("qScale"))  t->SetBranchAddress("qScale", &qScale, &b_qScale);
    if (t->GetBranch("nMEPartons"))  t->SetBranchAddress("nMEPartons", &nMEPartons, &b_nMEPartons);
    if (t->GetBranch("nMEPartonsFiltered"))  t->SetBranchAddress("nMEPartonsFiltered", &nMEPartonsFiltered, &b_nMEPartonsFiltered);
    if (t->GetBranch("ttbar_w"))  t->SetBranchAddress("ttbar_w", &ttbar_w, &b_ttbar_w);
    if (t->GetBranch("npus"))  t->SetBranchAddress("npus", &npus, &b_npus);
    if (t->GetBranch("tnpus"))  t->SetBranchAddress("tnpus", &tnpus, &b_tnpus);
    if (t->GetBranch("hiBin"))  t->SetBranchAddress("hiBin", &hiBin, &b_hiBin);
    if (t->GetBranch("hiHF"))  t->SetBranchAddress("hiHF", &hiHF, &b_hiHF);
    if (t->GetBranch("hiHFplus"))  t->SetBranchAddress("hiHFplus", &hiHFplus, &b_hiHFplus);
    if (t->GetBranch("hiHFminus"))  t->SetBranchAddress("hiHFminus", &hiHFminus, &b_hiHFminus);
    if (t->GetBranch("hiHFplusEta4"))  t->SetBranchAddress("hiHFplusEta4", &hiHFplusEta4, &b_hiHFplusEta4);
    if (t->GetBranch("hiHFminusEta4"))  t->SetBranchAddress("hiHFminusEta4", &hiHFminusEta4, &b_hiHFminusEta4);
    if (t->GetBranch("hiZDC"))  t->SetBranchAddress("hiZDC", &hiZDC, &b_hiZDC);
    if (t->GetBranch("hiZDCplus"))  t->SetBranchAddress("hiZDCplus", &hiZDCplus, &b_hiZDCplus);
    if (t->GetBranch("hiZDCminus"))  t->SetBranchAddress("hiZDCminus", &hiZDCminus, &b_hiZDCminus);
    if (t->GetBranch("hiHFhit"))  t->SetBranchAddress("hiHFhit", &hiHFhit, &b_hiHFhit);
    if (t->GetBranch("hiHFhitPlus"))  t->SetBranchAddress("hiHFhitPlus", &hiHFhitPlus, &b_hiHFhitPlus);
    if (t->GetBranch("hiHFhitMinus"))  t->SetBranchAddress("hiHFhitMinus", &hiHFhitMinus, &b_hiHFhitMinus);
    if (t->GetBranch("hiET"))  t->SetBranchAddress("hiET", &hiET, &b_hiET);
    if (t->GetBranch("hiEE"))  t->SetBranchAddress("hiEE", &hiEE, &b_hiEE);
    if (t->GetBranch("hiEB"))  t->SetBranchAddress("hiEB", &hiEB, &b_hiEB);
    if (t->GetBranch("hiEEplus"))  t->SetBranchAddress("hiEEplus", &hiEEplus, &b_hiEEplus);
    if (t->GetBranch("hiEEminus"))  t->SetBranchAddress("hiEEminus", &hiEEminus, &b_hiEEminus);
    if (t->GetBranch("hiNpix"))  t->SetBranchAddress("hiNpix", &hiNpix, &b_hiNpix);
    if (t->GetBranch("hiNpixelTracks"))  t->SetBranchAddress("hiNpixelTracks", &hiNpixelTracks, &b_hiNpixelTracks);
    if (t->GetBranch("hiNtracks"))  t->SetBranchAddress("hiNtracks", &hiNtracks, &b_hiNtracks);
    if (t->GetBranch("hiNtracksPtCut"))  t->SetBranchAddress("hiNtracksPtCut", &hiNtracksPtCut, &b_hiNtracksPtCut);
    if (t->GetBranch("hiNtracksEtaCut"))  t->SetBranchAddress("hiNtracksEtaCut", &hiNtracksEtaCut, &b_hiNtracksEtaCut);
    if (t->GetBranch("hiNtracksEtaPtCut"))  t->SetBranchAddress("hiNtracksEtaPtCut", &hiNtracksEtaPtCut, &b_hiNtracksEtaPtCut);
    if (t->GetBranch("hiNevtPlane"))  t->SetBranchAddress("hiNevtPlane", &hiNevtPlane, &b_hiNevtPlane);
    if (t->GetBranch("hiEvtPlanes"))  t->SetBranchAddress("hiEvtPlanes", hiEvtPlanes, &b_hiEvtPlanes);
}

void hiEvt::setupTreeForWriting(TTree *t)
{
    // Run info
    t->Branch("run",&run,"run/i");
    t->Branch("evt",&evt,"evt/l");
    t->Branch("lumi",&lumi,"lumi/i");

    // Vertex
    t->Branch("vx",&vx,"vx/F");
    t->Branch("vy",&vy,"vy/F");
    t->Branch("vz",&vz,"vz/F");

    // doHiMC
    t->Branch("Npart",&Npart,"Npart/F");
    t->Branch("Ncoll",&Ncoll,"Ncoll/F");
    t->Branch("Nhard",&Nhard,"Nhard/F");
    t->Branch("phi0",&phi0,"phi0/F");
    t->Branch("b",&b,"b/F");
    t->Branch("Ncharged",&Ncharged,"Ncharged/I");
    t->Branch("NchargedMR",&NchargedMR,"NchargedMR/I");
    t->Branch("MeanPt",&MeanPt,"MeanPt/F");
    t->Branch("MeanPtMR",&MeanPtMR,"MeanPtMR/F");
    t->Branch("EtMR",&EtMR,"EtMR/F");
    t->Branch("NchargedPtCut",&NchargedPtCut,"NchargedPtCut/I");
    t->Branch("NchargedPtCutMR",&NchargedPtCutMR,"NchargedPtCutMR/I");

    // doMC
    t->Branch("ProcessID",&ProcessID,"ProcessID/I");
    t->Branch("pthat",&pthat,"pthat/F");
    t->Branch("weight",&weight,"weight/F");
    t->Branch("alphaQCD",&alphaQCD,"alphaQCD/F");
    t->Branch("alphaQED",&alphaQED,"alphaQED/F");
    t->Branch("qScale",&qScale,"qScale/F");
    t->Branch("nMEPartons",&nMEPartons,"nMEPartons/I");
    t->Branch("nMEPartonsFiltered",&nMEPartonsFiltered,"nMEPartonsFiltered/I");
    t->Branch("ttbar_w",&ttbar_w);
    t->Branch("npus",&npus);
    t->Branch("tnpus",&tnpus);

    // Centrality
    t->Branch("hiBin",&hiBin,"hiBin/I");
    t->Branch("hiHF",&hiHF,"hiHF/F");
    t->Branch("hiHFplus",&hiHFplus,"hiHFplus/F");
    t->Branch("hiHFminus",&hiHFminus,"hiHFminus/F");
    t->Branch("hiHFplusEta4",&hiHFplusEta4,"hiHFplusEta4/F");
    t->Branch("hiHFminusEta4",&hiHFminusEta4,"hiHFminusEta4/F");

    t->Branch("hiZDC",&hiZDC,"hiZDC/F");
    t->Branch("hiZDCplus",&hiZDCplus,"hiZDCplus/F");
    t->Branch("hiZDCminus",&hiZDCminus,"hiZDCminus/F");

    t->Branch("hiHFhit",&hiHFhit,"hiHFhit/F");
    t->Branch("hiHFhitPlus",&hiHFhitPlus,"hiHFhitPlus/F");
    t->Branch("hiHFhitMinus",&hiHFhitMinus,"hiHFhitMinus/F");

    t->Branch("hiET",&hiET,"hiET/F");
    t->Branch("hiEE",&hiEE,"hiEE/F");
    t->Branch("hiEB",&hiEB,"hiEB/F");
    t->Branch("hiEEplus",&hiEEplus,"hiEEplus/F");
    t->Branch("hiEEminus",&hiEEminus,"hiEEminus/F");
    t->Branch("hiNpix",&hiNpix,"hiNpix/I");
    t->Branch("hiNpixelTracks",&hiNpixelTracks,"hiNpixelTracks/I");
    t->Branch("hiNtracks",&hiNtracks,"hiNtracks/I");
    t->Branch("hiNtracksPtCut",&hiNtracksPtCut,"hiNtracksPtCut/I");
    t->Branch("hiNtracksEtaCut",&hiNtracksEtaCut,"hiNtracksEtaCut/I");
    t->Branch("hiNtracksEtaPtCut",&hiNtracksEtaPtCut,"hiNtracksEtaPtCut/I");
    // Event plane
    t->Branch("hiNevtPlane",&hiNevtPlane,"hiNevtPlane/I");
    t->Branch("hiEvtPlanes",hiEvtPlanes,"hiEvtPlanes[hiNevtPlane]/F");
}

#endif
