#include "TROOT.h"
#include "TFile.h"
#include "TString.h"
#include "TF1.h"
#include "TKey.h"

#include "../TreeHeaders/CutConfigurationTree.h"
#include "../TreeHeaders/gammaJetTree.h"
#include "../Utilities/interface/CutConfigurationParser.h"
#include "../Utilities/interface/InputConfigurationParser.h"

static const long MAXTREESIZE = 2000000000000;

int gammaJetCorrections(const TString configFile, const TString inputFile, const TString outputFile) {
    InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

    TTree *configTree = setupConfigurationTreeForWriting(configCuts);

    TFile* inFile = TFile::Open(inputFile, "read");
    TTree* phoTree = (TTree*)inFile->Get("EventTree");
    ggHiNtuplizer pho_event;
    pho_event.setupTreeForReading(phoTree);

    std::vector<float> phoEtCorrected;
    phoTree->Branch("phoEtCorrected", &phoEtCorrected);

    TTree* eventTree = (TTree*)inFile->Get("HiEvt");

    std::vector<std::string> jetCollections = ConfigurationParser::ParseList(configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].s[CUTS::JET::k_jetCollection]);
    int nJetCollections = jetCollections.size();

    TTree* gjTree[nJetCollections];
    GammaJet gammajet_event[nJetCollections];
    std::vector<float> xjgCorrected[nJetCollections];
    for (int i=0; i<nJetCollections; ++i) {
        gjTree[i] = (TTree*)inFile->Get(Form("gamma_%s", jetCollections[i].c_str()));
        gammajet_event[i].setupGammaJetTree(gjTree[i]);
        gjTree[i]->Branch("xjgCorrected", &xjgCorrected[i]);
    }

    int hiBin;
    eventTree->SetBranchAddress("hiBin", &hiBin);

    TFile* outFile = TFile::Open(outputFile, "recreate");

    TTree* outPhoTree = phoTree->CloneTree(0);
    outPhoTree->SetMaxTreeSize(MAXTREESIZE);

    TTree* outGJTree[nJetCollections];
    for (int i=0; i<nJetCollections; ++i) {
        outGJTree[i] = gjTree[i]->CloneTree(0);
        outGJTree[i]->SetMaxTreeSize(MAXTREESIZE);
    }

    TTree* outEventTree = eventTree->CloneTree(-1, "fast");
    outEventTree->SetMaxTreeSize(MAXTREESIZE);

    std::vector<int> centBins[2];
    centBins[0] = ConfigurationParser::ParseListInteger(configCuts.proc[CUTS::kCORRECTION].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_gt]);
    centBins[1] = ConfigurationParser::ParseListInteger(configCuts.proc[CUTS::kCORRECTION].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_lt]);
    int nCentBins = centBins[0].size();

    std::vector<float> etaBins[2];
    etaBins[0] = ConfigurationParser::ParseListFloat(configCuts.proc[CUTS::kCORRECTION].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_eta_gt]);
    etaBins[1] = ConfigurationParser::ParseListFloat(configCuts.proc[CUTS::kCORRECTION].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_eta_lt]);
    int nEtaBins = etaBins[0].size();

    TFile* energyCorrectionFile = TFile::Open(configCuts.proc[CUTS::kCORRECTION].obj[CUTS::kPHOTON].s[CUTS::PHO::k_energy_correction_file].c_str());
    TH1D* photonEnergyCorrections[nCentBins][nEtaBins];
    for (int i=0; i<nCentBins; ++i)
        for (int j=0; j<nEtaBins; ++j)
            photonEnergyCorrections[i][j] = (TH1D*)energyCorrectionFile->Get(Form("photonEnergyCorr_cent%i_eta%i", i, j));

    uint64_t nentries = phoTree->GetEntries();
    for (uint64_t i=0; i<nentries; ++i) {
        if (i % 5000 == 0)
            printf("entry: %lu\n", i);

        phoTree->GetEntry(i);
        eventTree->GetEntry(i);

        phoEtCorrected.clear();

        for (int igj=0; igj<nJetCollections; ++igj) {
            gjTree[igj]->GetEntry(i);
            xjgCorrected[igj].clear();
        }

        int icent = 0;
        for (; hiBin>=centBins[1][icent] && icent<nCentBins; ++icent);

        for (int j=0; j<pho_event.nPho; ++j) {
            int ieta = 0;
            for (; fabs((*pho_event.phoEta)[j])>=etaBins[1][ieta] && ieta<nEtaBins; ++ieta);

            if (ieta == nEtaBins)
                phoEtCorrected.push_back(0);
            else
                phoEtCorrected.push_back((*pho_event.phoEt)[j] / photonEnergyCorrections[icent][ieta]->GetBinContent(photonEnergyCorrections[icent][ieta]->FindBin((*pho_event.phoEt)[j])));
        }

        for (int igj=0; igj<nJetCollections; ++igj) {
            int ieta = 0;
            for (; fabs((*pho_event.phoEta)[gammajet_event[igj].phoIdx])>=etaBins[1][ieta] && ieta<nEtaBins; ++ieta);

            for (std::size_t k=0; k<gammajet_event[igj].xjg->size(); ++k) {
                if (ieta == nEtaBins)
                    xjgCorrected[igj].push_back(0);
                else
                    xjgCorrected[igj].push_back((*gammajet_event[igj].xjg)[k] * photonEnergyCorrections[icent][ieta]->GetBinContent(photonEnergyCorrections[icent][ieta]->FindBin((*pho_event.phoEt)[gammajet_event[igj].phoIdx])));
            }
        }

        outPhoTree->Fill();
        for (int igj=0; igj<nJetCollections; ++igj)
            outGJTree[igj]->Fill();
    }

    outPhoTree->AutoSave();
    for (int i=0; i<nJetCollections; ++i)
        outGJTree[i]->AutoSave();

    outFile->cd();

    const char* photonTreeName = "EventTree";
    TString jetCollectionNames[nJetCollections];
    for (int i=0; i<nJetCollections; ++i)
        jetCollectionNames[i] = Form("gamma_%s", jetCollections[i].c_str());

    TKey* key;
    TIter nextkey(inFile->GetListOfKeys());
    while ((key = (TKey*)nextkey())) {
        if (key->GetName() == photonTreeName)
            continue;
        bool skipKey = false;
        for (int i=0; i<nJetCollections; ++i)
            if (key->GetName() == jetCollectionNames[i].Data())
                skipKey = true;
        if (skipKey)
            continue;

        printf("Cloning tree: %s\n", key->GetName());
        TClass* cl = gROOT->GetClass(key->GetClassName());
        if (!cl)
            continue;
        if (cl->InheritsFrom(TTree::Class())) {
            TTree* T = (TTree*)inFile->Get(key->GetName());
            TTree* outT = T->CloneTree(-1, "fast");
            outT->Write("", TObject::kOverwrite);
        }
    }

    configTree->Write("", TObject::kOverwrite);
    outPhoTree->Write("", TObject::kOverwrite);
    for (int i=0; i<nJetCollections; ++i)
        outGJTree[i]->Write("", TObject::kOverwrite);
    outEventTree->Write("", TObject::kOverwrite);
    outFile->Close();

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 4)
        return gammaJetCorrections(argv[1], argv[2], argv[3]);
    else
        printf("Usage: ./gammaJetCorrections.exe <configFile> <inputSkim> <outputFile>\n");

    return 1;
}
