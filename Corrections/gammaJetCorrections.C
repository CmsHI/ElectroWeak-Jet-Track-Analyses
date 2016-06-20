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

int photonCorrections(const TString configFile, const TString inputFile, const TString outputFile) {
    InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

    TTree *configTree = setupConfigurationTreeForWriting(configCuts);

    const char* photonTreeName = "EventTree";

    TFile* inFile = TFile::Open(inputFile, "read");
    TTree* inTree = (TTree*)inFile->Get("EventTree");
    ggHiNtuplizer inPho;
    inPho.setupTreeForReading(inTree);

    TTree* eventTree = (TTree*)inFile->Get("HiEvt");

    int hiBin;
    eventTree->SetBranchAddress("hiBin", &hiBin);

    TFile* outFile = TFile::Open(outputFile, "recreate");
    TTree* outTree = inTree->CloneTree(0);
    outTree->SetMaxTreeSize(MAXTREESIZE);

    TTree* outEventTree = eventTree->CloneTree(-1, "fast");
    outTree->SetMaxTreeSize(MAXTREESIZE);

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

    uint64_t nentries = inTree->GetEntries();
    for (uint64_t i=0; i<nentries; ++i) {
        if (i % 5000 == 0)
            printf("entry: %lu\n", i);

        inTree->GetEntry(i);
        eventTree->GetEntry(i);

        int icent = 0;
        for (; hiBin>=centBins[1][icent] && icent<nCentBins; ++icent);

        for (int j=0; j<inPho.nPho; ++j) {
            int ieta = 0;
            for (; fabs((*inPho.phoEta)[j])>=etaBins[1][ieta] && ieta<nEtaBins; ++ieta);
            if (ieta == nEtaBins)
                continue;

            (*inPho.phoEt)[j] /= photonEnergyCorrections[icent][ieta]->GetBinContent(photonEnergyCorrections[icent][ieta]->FindBin((*inPho.phoEt)[j]));
        }

        outTree->Fill();
    }
    outTree->AutoSave();

    outFile->cd();

    TKey* key;
    TIter nextkey(inFile->GetListOfKeys());
    while ((key = (TKey*)nextkey())) {
        if (key->GetName() == photonTreeName)
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
    outTree->Write("", TObject::kOverwrite);
    outEventTree->Write("", TObject::kOverwrite);
    outFile->Close();

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 4)
        return photonCorrections(argv[1], argv[2], argv[3]);

    return 1;
}
