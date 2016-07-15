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
const int nSmearBins = 8; // should really come from config

double getAngleToEP(double angle);

int gammaJetCorrections(const TString configFile, const TString inputFile, const TString outputFile, const int nJobs = -1, const int job = -1) {
    InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

    int collision = configInput.proc[INPUT::kHISTOGRAM].i[INPUT::k_collisionType];
    bool isHI = collisionIsHI((COLL::TYPE)collision);

    TTree *configTree = setupConfigurationTreeForWriting(configCuts);

    TFile* inFile = TFile::Open(inputFile, "read");
    TTree* phoTree = (TTree*)inFile->Get("EventTree");
    ggHiNtuplizer pho_event;
    pho_event.setupTreeForReading(phoTree);

    std::vector<float> phoEtCorrected;
    phoTree->Branch("phoEtCorrected", &phoEtCorrected);
    std::vector<float> pho_sumIsoCorrected;
    phoTree->Branch("pho_sumIsoCorrected", &pho_sumIsoCorrected);

    std::vector<std::string> jetCollections = ConfigurationParser::ParseList(configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].s[CUTS::JET::k_jetCollection]);
    int nJetCollections = jetCollections.size();

    TTree* gjTree[nJetCollections][nSmearBins];
    GammaJet gammajet_event[nJetCollections][nSmearBins];
    TTree *gjTreeMB[nJetCollections];
    GammaJet gammajet_eventMB[nJetCollections];
    std::vector<float>* xjgCorrected[nJetCollections][nSmearBins] = {0};
    std::vector<float>* xjgCorrectedMB[nJetCollections] = {0};

    for (int i=0; i<nJetCollections; ++i) {
        gjTree[i][0] = (TTree*)inFile->Get(Form("gamma_%s", jetCollections[i].c_str()));
        gammajet_event[i][0].setupGammaJetTree(gjTree[i][0]);
        if (isHI) {
            gjTreeMB[i] = (TTree*)inFile->Get(Form("gamma_%sMB", jetCollections[i].c_str()));
            gammajet_eventMB[i].setupGammaJetTree(gjTreeMB[i]);
        } else {
            for (int j = 1; j < nSmearBins; j++) {
                gjTree[i][j] = (TTree*)inFile->Get(Form("gamma_%s_smearBin%i", jetCollections[i].c_str(), j-1));
                gammajet_event[i][j].setupGammaJetTree(gjTree[i][j]);
            }
        }
    }

    TTree* eventTree = (TTree*)inFile->Get("HiEvt");

    int hiBin;
    eventTree->SetBranchAddress("hiBin", &hiBin);
    float hiEvtPlanes[29];
    eventTree->SetBranchAddress("hiEvtPlanes", hiEvtPlanes);

    TFile* outFile = TFile::Open(outputFile, "recreate");

    TTree* outPhoTree = phoTree->CloneTree(0);
    outPhoTree->SetMaxTreeSize(MAXTREESIZE);

    TTree* outGJTree[nJetCollections][nSmearBins];
    TTree* outGJTreeMB[nJetCollections];
    for (int i=0; i<nJetCollections; ++i) {
        outGJTree[i][0] = gjTree[i][0]->CloneTree(0);
        outGJTree[i][0]->SetMaxTreeSize(MAXTREESIZE);
        outGJTree[i][0]->SetBranchAddress("xjgCorrected", &xjgCorrected[i][0]);
        if (isHI) {
            outGJTreeMB[i] = gjTreeMB[i]->CloneTree(0);
            outGJTreeMB[i]->SetMaxTreeSize(MAXTREESIZE);
            outGJTreeMB[i]->SetBranchAddress("xjgCorrected", &xjgCorrectedMB[i]);
        } else {
            for (int j = 1; j < nSmearBins; j++) {
                outGJTree[i][j] = gjTree[i][j]->CloneTree(0);
                outGJTree[i][j]->SetMaxTreeSize(MAXTREESIZE);
                outGJTree[i][j]->SetBranchAddress("xjgCorrected", &xjgCorrected[i][j]);
            }
        }
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

    TFile* sumIsoCorrectionFile = TFile::Open(configCuts.proc[CUTS::kCORRECTION].obj[CUTS::kPHOTON].s[CUTS::PHO::k_sumiso_correction_file].c_str());
    TH1D* sumIsoCorrections = (TH1D*)sumIsoCorrectionFile->Get("sumIsoCorrections");

    uint64_t nentries = phoTree->GetEntries();

    uint64_t first_entry = 0;
    uint64_t last_entry = nentries;

    if (nJobs > 0) {
        if (job >= nJobs || job < 0) {
            printf("invalid job number: %i\n", job);
            return 1;
        }

        first_entry = nentries/nJobs * job;
        last_entry = std::min(nentries/nJobs * (job+1), nentries);

        printf("job: %i, first entry = %lu, last entry < %lu\n", job, first_entry, last_entry);
    }

    for (uint64_t i=first_entry; i<last_entry; ++i) {
        if (i % 5000 == 0)
            printf("entry: %lu\n", i);

        eventTree->GetEntry(i);

        phoTree->GetEntry(i);
        phoEtCorrected.clear();
        pho_sumIsoCorrected.clear();

        for (int igj=0; igj<nJetCollections; ++igj) {
            gjTree[igj][0]->GetEntry(i);
            xjgCorrected[igj][0]->clear();
            if (isHI) {
                gjTreeMB[igj]->GetEntry(i);
                xjgCorrectedMB[igj]->clear();
            } else {
                for (int j = 1; j < nSmearBins; j++) {
                    gjTree[igj][j]->GetEntry(i);
                    xjgCorrected[igj][j]->clear();
                }
            }
        }

        int icent = 0;
        for (; hiBin>=centBins[1][icent] && icent<nCentBins; ++icent);

        for (int j=0; j<pho_event.nPho; ++j) {
            double sumIso = (*pho_event.pho_ecalClusterIsoR4)[j] + (*pho_event.pho_hcalRechitIsoR4)[j] + (*pho_event.pho_trackIsoR4PtCut20)[j];

            if (isHI) {
                int ieta = 0;
                for (; fabs((*pho_event.phoEta)[j])>=etaBins[1][ieta] && ieta<nEtaBins; ++ieta);

                if (ieta == nEtaBins)
                    phoEtCorrected.push_back(0);
                else
                    phoEtCorrected.push_back((*pho_event.phoEt)[j] / photonEnergyCorrections[icent][ieta]->GetBinContent(photonEnergyCorrections[icent][ieta]->FindBin((*pho_event.phoEt)[j])));

                pho_sumIsoCorrected.push_back(sumIso / sumIsoCorrections->GetBinContent(sumIsoCorrections->FindBin(getAngleToEP(fabs((*pho_event.phoPhi)[j] - hiEvtPlanes[8])))));
            } else {
                phoEtCorrected.push_back((*pho_event.phoEt)[j]);
                pho_sumIsoCorrected.push_back(sumIso);
            }
        }

        for (int igj=0; igj<nJetCollections; ++igj) {
            if (isHI) {
                int ieta = 0;
                for (; fabs((*pho_event.phoEta)[gammajet_eventMB[igj].phoIdx])>=etaBins[1][ieta] && ieta<nEtaBins; ++ieta);

                for (std::size_t k=0; k<gammajet_event[igj][0].xjg->size(); ++k) {
                    if (ieta == nEtaBins) {
                        xjgCorrected[igj][0]->push_back(0);
                    } else {
                        xjgCorrected[igj][0]->push_back((*gammajet_event[igj][0].xjg)[k] * photonEnergyCorrections[icent][ieta]->GetBinContent(photonEnergyCorrections[icent][ieta]->FindBin((*pho_event.phoEt)[gammajet_event[igj][0].phoIdx])));
                    }
                }

                for (std::size_t k=0; k< gammajet_eventMB[igj].xjg->size(); ++k) {
                    if (ieta == nEtaBins) {
                        xjgCorrectedMB[igj]->push_back(0);
                    } else {
                        xjgCorrectedMB[igj]->push_back((*gammajet_eventMB[igj].xjg)[k] * photonEnergyCorrections[icent][ieta]->GetBinContent(photonEnergyCorrections[icent][ieta]->FindBin((*pho_event.phoEt)[gammajet_eventMB[igj].phoIdx])));
                    }
                }
            } else {
                for (int j = 0; j < nSmearBins; j++) {
                    for (std::size_t k=0; k<gammajet_event[igj][j].xjg->size(); ++k) {
                        xjgCorrected[igj][j]->push_back((*gammajet_event[igj][j].xjg)[k]);
                    }
                }
            }
        }

        outPhoTree->Fill();
        for (int igj=0; igj<nJetCollections; ++igj) {
            outGJTree[igj][0]->Fill();
            if (isHI) {
                outGJTreeMB[igj]->Fill();
            } else {
                for (int j = 1; j < nSmearBins; j++) {
                    outGJTree[igj][j]->Fill();
                }
            }
        }
    }

    outPhoTree->AutoSave();
    for (int i=0; i<nJetCollections; ++i) {
        outGJTree[i][0]->AutoSave();
        if (isHI) {
            outGJTreeMB[i]->AutoSave();
        } else {
            for (int j = 1; j < nSmearBins; j++) {
                outGJTree[i][j]->AutoSave();
            }
        }
    }

    outFile->cd();

    const char* photonTreeName = "EventTree";
    TString jetCollectionNames[nJetCollections][nSmearBins];
    TString jetCollectionNamesMB[nJetCollections];
    for (int i=0; i<nJetCollections; ++i) {
        jetCollectionNames[i][0] = Form("gamma_%s", jetCollections[i].c_str());
        if (isHI) {
            jetCollectionNamesMB[i] = Form("gamma_%sMB", jetCollections[i].c_str());
        } else {
            for (int j = 1; j < nSmearBins; j++) {
                jetCollectionNames[i][j] = Form("gamma_%s_smearBin%i", jetCollections[i].c_str(),j-1);
            }
        }
    }

    TKey* key;
    TIter nextkey(inFile->GetListOfKeys());
    while ((key = (TKey*)nextkey())) {
        bool skipKey = false;
        if (strcmp(key->GetName(), photonTreeName) == 0)
            skipKey = true;
        for (int i=0; i<nJetCollections; ++i) {
            if (strcmp(key->GetName(), jetCollectionNames[i][0].Data()) == 0) {
                skipKey = true;
            }
            if (isHI) {
                if(strcmp(key->GetName(), jetCollectionNamesMB[i].Data()) == 0) {
                    skipKey = true;
                }
            } else {
                for (int j = 1; j < nSmearBins; j++) {
                    if (strcmp(key->GetName(), jetCollectionNames[i][j].Data()) == 0) {
                        skipKey = true;
                    }
                }
            }
        }
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
    for (int i=0; i<nJetCollections; ++i) {
        outGJTree[i][0]->Write("", TObject::kOverwrite);
        if (isHI) {
            outGJTreeMB[i]->Write("", TObject::kOverwrite);
        } else {
            for (int j = 1; j < nSmearBins; j++) {
                outGJTree[i][j]->Write("", TObject::kOverwrite);
            }
        }
    }
    outEventTree->Write("", TObject::kOverwrite);
    outFile->Close();

    return 0;
}

double getAngleToEP(double angle) {
    angle = (angle > TMath::Pi()) ? 2 * TMath::Pi() - angle : angle;
    return (angle > TMath::Pi()/2) ? TMath::Pi() - angle : angle;
}

int main(int argc, char* argv[]) {
    if (argc == 6)
        return gammaJetCorrections(argv[1], argv[2], argv[3], atoi(argv[4]), atoi(argv[5]));
    else if (argc == 4)
        return gammaJetCorrections(argv[1], argv[2], argv[3]);
    else
        printf("Usage: ./gammaJetCorrections.exe <configFile> <inputSkim> <outputFile>\n");

    return 1;
}
