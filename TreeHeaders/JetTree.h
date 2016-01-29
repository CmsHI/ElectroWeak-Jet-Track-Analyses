#ifndef JetTree
#define JetTree

#include <TTree.h>
#include <TBranch.h>

#include <vector>

const unsigned int maxJets = 10000;     // set upper limit 10K to cover the jet trees from MB mixing
const unsigned int maxGenJets = 100;

class Jets {
public :
  Jets(){};
  ~Jets(){};
  void setupTreeForReading(TTree *t);
  void setupTreeForWriting(TTree *t);
  void setupTreeForWritingMB(TTree *t, bool doHiJetID, bool doMC);
  int  jetID(int i);

  // Declaration of leaf types
  ULong64_t       event;
  unsigned        run;
  unsigned        lumi;
  Int_t           hiBin;
  Int_t HBHENoiseFilterResult;
  Int_t pcollisionEventSelection;

  Float_t         b;
  Int_t           nref;
  Float_t         rawpt[maxJets];   //[nref]
  Float_t         jtpt[maxJets];   //[nref]
  Float_t         jteta[maxJets];   //[nref]
  Float_t         jty[maxJets];   //[nref]
  Float_t         jtphi[maxJets];   //[nref]
  Float_t         jtpu[maxJets];   //[nref]
  Float_t         jtm[maxJets];   //[nref]
  Float_t         discr_fr01[maxJets];   //[nref]
  Float_t         trackMax[maxJets];   //[nref]
  Float_t         trackSum[maxJets];   //[nref]
  Int_t           trackN[maxJets];   //[nref]
  Float_t         trackHardSum[maxJets];   //[nref]
  Int_t           trackHardN[maxJets];   //[nref]
  Float_t         chargedMax[maxJets];   //[nref]
  Float_t         chargedSum[maxJets];   //[nref]
  Int_t           chargedN[maxJets];   //[nref]
  Float_t         chargedHardSum[maxJets];   //[nref]
  Int_t           chargedHardN[maxJets];   //[nref]
  Float_t         photonMax[maxJets];   //[nref]
  Float_t         photonSum[maxJets];   //[nref]
  Int_t           photonN[maxJets];   //[nref]
  Float_t         photonHardSum[maxJets];   //[nref]
  Int_t           photonHardN[maxJets];   //[nref]
  Float_t         neutralMax[maxJets];   //[nref]
  Float_t         neutralSum[maxJets];   //[nref]
  Int_t           neutralN[maxJets];   //[nref]

  Float_t         hcalSum[maxJets];   //[nref]
  Float_t         ecalSum[maxJets];   //[nref]

  Float_t         eMax[maxJets];   //[nref]
  Float_t         eSum[maxJets];   //[nref]
  Int_t           eN[maxJets];   //[nref]
  Float_t         muMax[maxJets];   //[nref]
  Float_t         muSum[maxJets];   //[nref]
  Int_t           muN[maxJets];   //[nref]
  Float_t         matchedPt[maxJets];   //[nref]
  Float_t         matchedR[maxJets];   //[nref]
  Int_t           beamId1;
  Int_t           beamId2;
  Float_t         pthat;
  Float_t         refpt[maxJets];   //[nref]
  Float_t         refeta[maxJets];   //[nref]
  Float_t         refy[maxJets];   //[nref]
  Float_t         refphi[maxJets];   //[nref]
  Float_t         refdphijt[maxJets];   //[nref]
  Float_t         refdrjt[maxJets];   //[nref]
  Float_t         refparton_pt[maxJets];   //[nref]
  Int_t           refparton_flavor[maxJets];   //[nref]
  Int_t           refparton_flavorForB[maxJets];   //[nref]

  Float_t         genChargedSum[maxJets];   //[nref]
  Float_t         genHardSum[maxJets];   //[nref]

  Float_t         signalChargedSum[maxJets];   //[nref]
  Float_t         signalHardSum[maxJets];   //[nref]

  Int_t           subid[maxJets];   //[nref]

  Int_t           ngen;
  Int_t           genmatchindex[maxGenJets];   //[ngen]
  Float_t         genpt[maxGenJets];   //[ngen]
  Float_t         geneta[maxGenJets];   //[ngen]
  Float_t         geny[maxGenJets];   //[ngen]
  Float_t         genphi[maxGenJets];   //[ngen]
  Float_t         gendphijt[maxGenJets];   //[ngen]
  Float_t         gendrjt[maxGenJets];   //[ngen]
  Int_t           gensubid[maxGenJets];   //[ngen]

  Float_t         smpt[maxJets];   //[nref]
  Float_t         fr01Chg[maxJets];   //[nref]
  Float_t         fr01EM[maxJets];   //[nref]
  Float_t         fr01[maxJets];   //[nref]

  // List of branches
  TBranch        *b_event;   //!
  TBranch        *b_run;
  TBranch        *b_lumi;
  TBranch        *b_b;   //!
  TBranch        *b_nref;   //!
  TBranch        *b_rawpt;   //!
  TBranch        *b_jtpt;   //!
  TBranch        *b_jteta;   //!
  TBranch        *b_jty;   //!
  TBranch        *b_jtphi;   //!
  TBranch        *b_jtpu;   //!
  TBranch        *b_jtm;   //!
  TBranch        *b_discr_fr01;   //!
  TBranch        *b_trackMax;   //!
  TBranch        *b_trackSum;   //!
  TBranch        *b_trackN;   //!
  TBranch        *b_trackHardSum;   //!
  TBranch        *b_trackHardN;   //!
  TBranch        *b_chargedMax;   //!
  TBranch        *b_chargedSum;   //!
  TBranch        *b_chargedN;   //!
  TBranch        *b_chargedHardSum;   //!
  TBranch        *b_chargedHardN;   //!
  TBranch        *b_photonMax;   //!
  TBranch        *b_photonSum;   //!
  TBranch        *b_photonN;   //!
  TBranch        *b_photonHardSum;   //!
  TBranch        *b_photonHardN;   //!
  TBranch        *b_neutralMax;   //!
  TBranch        *b_neutralSum;   //!
  TBranch        *b_neutralN;   //!

  TBranch        *b_hcalSum;   //!
  TBranch        *b_ecalSum;   //!

  TBranch        *b_eMax;   //!
  TBranch        *b_eSum;   //!
  TBranch        *b_eN;   //!
  TBranch        *b_muMax;   //!
  TBranch        *b_muSum;   //!
  TBranch        *b_muN;   //!
  TBranch        *b_matchedPt;   //!
  TBranch        *b_matchedR;   //!
  TBranch        *b_beamId1;   //!
  TBranch        *b_beamId2;   //!
  TBranch        *b_pthat;   //!
  TBranch        *b_refpt;   //!
  TBranch        *b_refeta;   //!
  TBranch        *b_refy;   //!
  TBranch        *b_refphi;   //!
  TBranch        *b_refdphijt;   //!
  TBranch        *b_refdrjt;   //!
  TBranch        *b_refparton_pt;   //!
  TBranch        *b_refparton_flavor;   //!
  TBranch        *b_refparton_flavorForB;   //!

  TBranch        *b_genChargedSum;   //!
  TBranch        *b_genHardSum;   //!
  TBranch        *b_signalChargedSum;   //!
  TBranch        *b_signalHardSum;   //!
  TBranch        *b_subid;   //!

  TBranch        *b_ngen;   //!
  TBranch        *b_genmatchindex;   //!
  TBranch        *b_genpt;   //!
  TBranch        *b_geneta;   //!
  TBranch        *b_geny;   //!
  TBranch        *b_genphi;   //!
  TBranch        *b_gendphijt;   //!
  TBranch        *b_gendrjt;   //!
  TBranch        *b_gensubid;   //!
  TBranch        *b_smpt;   //!
  TBranch        *b_fr01Chg;   //!
  TBranch        *b_fr01EM;   //!
  TBranch        *b_fr01;   //!

};

void Jets::setupTreeForReading(TTree *t)
{
    // Set branch addresses and branch pointers
    if (t->GetBranch("b")) t->SetBranchAddress("b", &b, &b_b);
    if (t->GetBranch("nref")) t->SetBranchAddress("nref", &nref, &b_nref);
    if (t->GetBranch("rawpt")) t->SetBranchAddress("rawpt", rawpt, &b_rawpt);
    if (t->GetBranch("jtpt")) t->SetBranchAddress("jtpt", jtpt, &b_jtpt);
    if (t->GetBranch("jteta")) t->SetBranchAddress("jteta", jteta, &b_jteta);
    if (t->GetBranch("jty")) t->SetBranchAddress("jty", jty, &b_jty);
    if (t->GetBranch("jtphi")) t->SetBranchAddress("jtphi", jtphi, &b_jtphi);
    if (t->GetBranch("jtpu")) t->SetBranchAddress("jtpu", jtpu, &b_jtpu);
    if (t->GetBranch("jtm")) t->SetBranchAddress("jtm", jtm, &b_jtm);
    if (t->GetBranch("discr_fr01")) t->SetBranchAddress("discr_fr01", discr_fr01, &b_discr_fr01);
    if (t->GetBranch("trackMax")) t->SetBranchAddress("trackMax", trackMax, &b_trackMax);
    if (t->GetBranch("trackSum")) t->SetBranchAddress("trackSum", trackSum, &b_trackSum);
    if (t->GetBranch("trackN")) t->SetBranchAddress("trackN", trackN, &b_trackN);
    if (t->GetBranch("trackHardSum")) t->SetBranchAddress("trackHardSum", trackHardSum, &b_trackHardSum);
    if (t->GetBranch("trackHardN")) t->SetBranchAddress("trackHardN", trackHardN, &b_trackHardN);
    if (t->GetBranch("chargedMax")) t->SetBranchAddress("chargedMax", chargedMax, &b_chargedMax);
    if (t->GetBranch("chargedSum")) t->SetBranchAddress("chargedSum", chargedSum, &b_chargedSum);
    if (t->GetBranch("chargedN")) t->SetBranchAddress("chargedN", chargedN, &b_chargedN);
    if (t->GetBranch("chargedHardSum")) t->SetBranchAddress("chargedHardSum", chargedHardSum, &b_chargedHardSum);
    if (t->GetBranch("chargedHardN")) t->SetBranchAddress("chargedHardN", chargedHardN, &b_chargedHardN);
    if (t->GetBranch("photonMax")) t->SetBranchAddress("photonMax", photonMax, &b_photonMax);
    if (t->GetBranch("photonSum")) t->SetBranchAddress("photonSum", photonSum, &b_photonSum);
    if (t->GetBranch("photonN")) t->SetBranchAddress("photonN", photonN, &b_photonN);
    if (t->GetBranch("photonHardSum")) t->SetBranchAddress("photonHardSum", photonHardSum, &b_photonHardSum);
    if (t->GetBranch("photonHardN")) t->SetBranchAddress("photonHardN", photonHardN, &b_photonHardN);
    if (t->GetBranch("neutralMax")) t->SetBranchAddress("neutralMax", neutralMax, &b_neutralMax);
    if (t->GetBranch("neutralSum")) t->SetBranchAddress("neutralSum", neutralSum, &b_neutralSum);
    if (t->GetBranch("neutralN")) t->SetBranchAddress("neutralN", neutralN, &b_neutralN);

    if (t->GetBranch("hcalSum")) t->SetBranchAddress("hcalSum", hcalSum, &b_hcalSum);
    if (t->GetBranch("ecalSum")) t->SetBranchAddress("ecalSum", ecalSum, &b_ecalSum);

    if (t->GetBranch("eMax")) t->SetBranchAddress("eMax", eMax, &b_eMax);
    if (t->GetBranch("eSum")) t->SetBranchAddress("eSum", eSum, &b_eSum);
    if (t->GetBranch("eN")) t->SetBranchAddress("eN", eN, &b_eN);
    if (t->GetBranch("muMax")) t->SetBranchAddress("muMax", muMax, &b_muMax);
    if (t->GetBranch("muSum")) t->SetBranchAddress("muSum", muSum, &b_muSum);
    if (t->GetBranch("muN")) t->SetBranchAddress("muN", muN, &b_muN);
    if (t->GetBranch("matchedPt")) t->SetBranchAddress("matchedPt", matchedPt, &b_matchedPt);
    if (t->GetBranch("matchedR")) t->SetBranchAddress("matchedR", matchedR, &b_matchedR);
    if (t->GetBranch("beamId1")) t->SetBranchAddress("beamId1", &beamId1, &b_beamId1);
    if (t->GetBranch("beamId2")) t->SetBranchAddress("beamId2", &beamId2, &b_beamId2);
    if (t->GetBranch("pthat")) t->SetBranchAddress("pthat", &pthat, &b_pthat);
    if (t->GetBranch("refpt")) t->SetBranchAddress("refpt", refpt, &b_refpt);
    if (t->GetBranch("refeta")) t->SetBranchAddress("refeta", refeta, &b_refeta);
    if (t->GetBranch("refy")) t->SetBranchAddress("refy", refy, &b_refy);
    if (t->GetBranch("refphi")) t->SetBranchAddress("refphi", refphi, &b_refphi);
    if (t->GetBranch("refdphijt")) t->SetBranchAddress("refdphijt", refdphijt, &b_refdphijt);
    if (t->GetBranch("refdrjt")) t->SetBranchAddress("refdrjt", refdrjt, &b_refdrjt);
    if (t->GetBranch("refparton_pt")) t->SetBranchAddress("refparton_pt", refparton_pt, &b_refparton_pt);
    if (t->GetBranch("refparton_flavor")) t->SetBranchAddress("refparton_flavor", refparton_flavor, &b_refparton_flavor);
    if (t->GetBranch("refparton_flavorForB")) t->SetBranchAddress("refparton_flavorForB", refparton_flavorForB, &b_refparton_flavorForB);

    if (t->GetBranch("genChargedSum")) t->SetBranchAddress("genChargedSum", genChargedSum, &b_genChargedSum);
    if (t->GetBranch("genHardSum")) t->SetBranchAddress("genHardSum", genHardSum, &b_genHardSum);
    if (t->GetBranch("signalChargedSum")) t->SetBranchAddress("signalChargedSum", signalChargedSum, &b_signalChargedSum);
    if (t->GetBranch("signalHardSum")) t->SetBranchAddress("signalHardSum", signalHardSum, &b_signalHardSum);
    if (t->GetBranch("subid")) t->SetBranchAddress("subid", subid, &b_subid);

    if (t->GetBranch("ngen")) t->SetBranchAddress("ngen", &ngen, &b_ngen);
    if (t->GetBranch("genmatchindex")) t->SetBranchAddress("genmatchindex", genmatchindex, &b_genmatchindex);
    if (t->GetBranch("genpt")) t->SetBranchAddress("genpt", genpt, &b_genpt);
    if (t->GetBranch("geneta")) t->SetBranchAddress("geneta", geneta, &b_geneta);
    if (t->GetBranch("geny")) t->SetBranchAddress("geny", geny, &b_geny);
    if (t->GetBranch("genphi")) t->SetBranchAddress("genphi", genphi, &b_genphi);
    if (t->GetBranch("gendphijt")) t->SetBranchAddress("gendphijt", gendphijt, &b_gendphijt);
    if (t->GetBranch("gendrjt")) t->SetBranchAddress("gendrjt", gendrjt, &b_gendrjt);
    if (t->GetBranch("gensubid")) t->SetBranchAddress("gensubid", gensubid, &b_gensubid);
    if (t->GetBranch("smpt")) t->SetBranchAddress("smpt", smpt, &b_smpt);
    if (t->GetBranch("fr01Chg")) t->SetBranchAddress("fr01Chg", fr01Chg, &b_fr01Chg);
    if (t->GetBranch("fr01EM")) t->SetBranchAddress("fr01EM", fr01EM, &b_fr01EM);
    if (t->GetBranch("fr01")) t->SetBranchAddress("fr01", fr01, &b_fr01);
    //if (doCheck) {
    if (t->GetMaximum("nref")>maxJets){ std::cout <<"FATAL ERROR: Arrary size of nref too small!!!  "<<t->GetMaximum("nref")<<std::endl; exit(0);}
    if (t->GetMaximum("ngen")>maxGenJets){ std::cout <<"FATAL ERROR: Arrary size of ngen too small!!!  "<<t->GetMaximum("ngen")<<std::endl; exit(0);}
    //}
}

void Jets::setupTreeForWriting(TTree *t)
{
    t->Branch("event", &event);
    t->Branch("run", &run);
    t->Branch("lumi", &lumi);
    t->Branch("hiBin", &hiBin);
    t->Branch("HBHENoiseFilterResult",&HBHENoiseFilterResult);
    t->Branch("pcollisionEventSelection",&pcollisionEventSelection);

    t->Branch("b",&b,"b/F");

    t->Branch("nref",&nref,"nref/I");
    t->Branch("rawpt",rawpt,"rawpt[nref]/F");
    t->Branch("jtpt",jtpt,"jtpt[nref]/F");
    t->Branch("jteta",jteta,"jteta[nref]/F");
    t->Branch("jty",jty,"jty[nref]/F");
    t->Branch("jtphi",jtphi,"jtphi[nref]/F");
    t->Branch("jtpu",jtpu,"jtpu[nref]/F");
    t->Branch("jtm",jtm,"jtm[nref]/F");

    // jet ID information, jet composition
    //if(doHiJetID_)
    {
        t->Branch("discr_fr01", discr_fr01,"discr_fr01[nref]/F");

        t->Branch("trackMax", trackMax,"trackMax[nref]/F");
        t->Branch("trackSum", trackSum,"trackSum[nref]/F");
        t->Branch("trackN", trackN,"trackN[nref]/I");
        t->Branch("trackHardSum", trackHardSum,"trackHardSum[nref]/F");
        t->Branch("trackHardN", trackHardN,"trackHardN[nref]/I");

        t->Branch("chargedMax", chargedMax,"chargedMax[nref]/F");
        t->Branch("chargedSum", chargedSum,"chargedSum[nref]/F");
        t->Branch("chargedN", chargedN,"chargedN[nref]/I");
        t->Branch("chargedHardSum", chargedHardSum,"chargedHardSum[nref]/F");
        t->Branch("chargedHardN", chargedHardN,"chargedHardN[nref]/I");

        t->Branch("photonMax", photonMax,"photonMax[nref]/F");
        t->Branch("photonSum", photonSum,"photonSum[nref]/F");
        t->Branch("photonN", photonN,"photonN[nref]/I");
        t->Branch("photonHardSum", photonHardSum,"photonHardSum[nref]/F");
        t->Branch("photonHardN", photonHardN,"photonHardN[nref]/I");

        t->Branch("neutralMax", neutralMax,"neutralMax[nref]/F");
        t->Branch("neutralSum", neutralSum,"neutralSum[nref]/F");
        t->Branch("neutralN", neutralN,"neutralN[nref]/I");

        // t->Branch("hcalSum", hcalSum,"hcalSum[nref]/F");
        // t->Branch("ecalSum", ecalSum,"ecalSum[nref]/F");

        t->Branch("eMax", eMax,"eMax[nref]/F");
        t->Branch("eSum", eSum,"eSum[nref]/F");
        t->Branch("eN", eN,"eN[nref]/I");

        t->Branch("muMax", muMax,"muMax[nref]/F");
        t->Branch("muSum", muSum,"muSum[nref]/F");
        t->Branch("muN", muN,"muN[nref]/I");
    }

    //if(isMC_)
    {
        t->Branch("beamId1",&beamId1,"beamId1/I");
        t->Branch("beamId2",&beamId2,"beamId2/I");

        t->Branch("pthat",&pthat,"pthat/F");

        // Only matched gen jets
        t->Branch("refpt",refpt,"refpt[nref]/F");
        t->Branch("refeta",refeta,"refeta[nref]/F");
        t->Branch("refy",refy,"refy[nref]/F");
        t->Branch("refphi",refphi,"refphi[nref]/F");
        t->Branch("refdphijt",refdphijt,"refdphijt[nref]/F");
        t->Branch("refdrjt",refdrjt,"refdrjt[nref]/F");
        // matched parton
        t->Branch("refparton_pt",refparton_pt,"refparton_pt[nref]/F");
        t->Branch("refparton_flavor",refparton_flavor,"refparton_flavor[nref]/I");
        t->Branch("refparton_flavorForB",refparton_flavorForB,"refparton_flavorForB[nref]/I");

        t->Branch("genChargedSum", genChargedSum,"genChargedSum[nref]/F");
        t->Branch("genHardSum", genHardSum,"genHardSum[nref]/F");
        t->Branch("signalChargedSum", signalChargedSum,"signalChargedSum[nref]/F");
        t->Branch("signalHardSum", signalHardSum,"signalHardSum[nref]/F");

        //if(doSubEvent_)
        {
            t->Branch("subid",subid,"subid[nref]/I");
        }

        //if(fillGenJets_)
        {
            // For all gen jets, matched or unmatched
            t->Branch("ngen",&ngen,"ngen/I");
            t->Branch("genmatchindex",genmatchindex,"genmatchindex[ngen]/I");
            t->Branch("genpt",genpt,"genpt[ngen]/F");
            t->Branch("geneta",geneta,"geneta[ngen]/F");
            t->Branch("geny",geny,"geny[ngen]/F");
            t->Branch("genphi",genphi,"genphi[ngen]/F");
            t->Branch("gendphijt",gendphijt,"gendphijt[ngen]/F");
            t->Branch("gendrjt",gendrjt,"gendrjt[ngen]/F");

            //if(doSubEvent_)
            {
                t->Branch("gensubid",gensubid,"gensubid[ngen]/I");
            }
        }
    }
}

// make branches for trees whose single entry consists of multiple entries MB jet trees
void Jets::setupTreeForWritingMB(TTree *t, bool doHiJetID, bool doMC)
{
    t->Branch("b",&b,"b/F");

    t->Branch("nref",&nref,"nref/I");
    t->Branch("rawpt",rawpt,"rawpt[nref]/F");
    t->Branch("jtpt",jtpt,"jtpt[nref]/F");
    t->Branch("jteta",jteta,"jteta[nref]/F");
    t->Branch("jty",jty,"jty[nref]/F");
    t->Branch("jtphi",jtphi,"jtphi[nref]/F");
    t->Branch("jtpu",jtpu,"jtpu[nref]/F");
    t->Branch("jtm",jtm,"jtm[nref]/F");

    // jet ID information, jet composition
    if(doHiJetID)
    {
        t->Branch("discr_fr01", discr_fr01,"discr_fr01[nref]/F");

        t->Branch("trackMax", trackMax,"trackMax[nref]/F");
        t->Branch("trackSum", trackSum,"trackSum[nref]/F");
        t->Branch("trackN", trackN,"trackN[nref]/I");
        t->Branch("trackHardSum", trackHardSum,"trackHardSum[nref]/F");
        t->Branch("trackHardN", trackHardN,"trackHardN[nref]/I");

        t->Branch("chargedMax", chargedMax,"chargedMax[nref]/F");
        t->Branch("chargedSum", chargedSum,"chargedSum[nref]/F");
        t->Branch("chargedN", chargedN,"chargedN[nref]/I");
        t->Branch("chargedHardSum", chargedHardSum,"chargedHardSum[nref]/F");
        t->Branch("chargedHardN", chargedHardN,"chargedHardN[nref]/I");

        t->Branch("photonMax", photonMax,"photonMax[nref]/F");
        t->Branch("photonSum", photonSum,"photonSum[nref]/F");
        t->Branch("photonN", photonN,"photonN[nref]/I");
        t->Branch("photonHardSum", photonHardSum,"photonHardSum[nref]/F");
        t->Branch("photonHardN", photonHardN,"photonHardN[nref]/I");

        t->Branch("neutralMax", neutralMax,"neutralMax[nref]/F");
        t->Branch("neutralSum", neutralSum,"neutralSum[nref]/F");
        t->Branch("neutralN", neutralN,"neutralN[nref]/I");

        // t->Branch("hcalSum", hcalSum,"hcalSum[nref]/F");
        // t->Branch("ecalSum", ecalSum,"ecalSum[nref]/F");

        t->Branch("eMax", eMax,"eMax[nref]/F");
        t->Branch("eSum", eSum,"eSum[nref]/F");
        t->Branch("eN", eN,"eN[nref]/I");

        t->Branch("muMax", muMax,"muMax[nref]/F");
        t->Branch("muSum", muSum,"muSum[nref]/F");
        t->Branch("muN", muN,"muN[nref]/I");
    }

    if (doMC)
    {
        t->Branch("beamId1",&beamId1,"beamId1/I");
        t->Branch("beamId2",&beamId2,"beamId2/I");

        t->Branch("pthat",&pthat,"pthat/F");

        // Only matched gen jets
        t->Branch("refpt",refpt,"refpt[nref]/F");
        t->Branch("refeta",refeta,"refeta[nref]/F");
        t->Branch("refy",refy,"refy[nref]/F");
        t->Branch("refphi",refphi,"refphi[nref]/F");
        t->Branch("refdphijt",refdphijt,"refdphijt[nref]/F");
        t->Branch("refdrjt",refdrjt,"refdrjt[nref]/F");
        // matched parton
        t->Branch("refparton_pt",refparton_pt,"refparton_pt[nref]/F");
        t->Branch("refparton_flavor",refparton_flavor,"refparton_flavor[nref]/I");
        t->Branch("refparton_flavorForB",refparton_flavorForB,"refparton_flavorForB[nref]/I");

        t->Branch("genChargedSum", genChargedSum,"genChargedSum[nref]/F");
        t->Branch("genHardSum", genHardSum,"genHardSum[nref]/F");
        t->Branch("signalChargedSum", signalChargedSum,"signalChargedSum[nref]/F");
        t->Branch("signalHardSum", signalHardSum,"signalHardSum[nref]/F");

        //if(doSubEvent_)
        {
            t->Branch("subid",subid,"subid[nref]/I");
        }

        //if(fillGenJets_)
        {
            // For all gen jets, matched or unmatched
            t->Branch("ngen",&ngen,"ngen/I");
            t->Branch("genmatchindex",genmatchindex,"genmatchindex[ngen]/I");
            t->Branch("genpt",genpt,"genpt[ngen]/F");
            t->Branch("geneta",geneta,"geneta[ngen]/F");
            t->Branch("geny",geny,"geny[ngen]/F");
            t->Branch("genphi",genphi,"genphi[ngen]/F");
            t->Branch("gendphijt",gendphijt,"gendphijt[ngen]/F");
            t->Branch("gendrjt",gendrjt,"gendrjt[ngen]/F");

            //if(doSubEvent_)
            {
                t->Branch("gensubid",gensubid,"gensubid[ngen]/I");
            }
        }
    }
}

int Jets::jetID(int i)
{
    int result = 0;
    if (rawpt[i] > 0) {
        if (    neutralSum[i]/rawpt[i] < 0.9
                &&  chargedSum[i]/rawpt[i] > 0.01
                && (chargedN[i] + photonN[i] + neutralN[i] + eN[i] + muN[i]) > 0
                &&  chargedMax[i]/rawpt[i] < 0.99
                &&  photonSum[i]/rawpt[i]  < 0.99
                &&  eSum[i]/rawpt[i]       < 0.99) {
            result = 1;
        }
    }
    return result;
}

#endif
