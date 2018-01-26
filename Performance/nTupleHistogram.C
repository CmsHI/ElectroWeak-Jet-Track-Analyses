/*
 * template macro to loop over a file with nTuples and fill histograms
 */

#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TMath.h>

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include "../Utilities/interface/InputConfigurationParser.h"
#include "../Utilities/interface/HiForestInfoController.h"
#include "../Utilities/fileUtil.h"

void setTH1D(TH1D* h);
void nTupleHistogram(std::string inputFile, std::string outputFile = "nTupleHistogram.root");

void nTupleHistogram(std::string inputFile, std::string outputFile)
{
    std::cout<<"running nTupleHistogram()"<<std::endl;
    std::cout<<"inputFile   = "<< inputFile.c_str()  <<std::endl;
    std::cout<<"outputFile  = "<< outputFile.c_str() <<std::endl;

    std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.c_str());
    std::cout<<"input ROOT files : num = "<<inputFiles.size()<< std::endl;
    std::cout<<"#####"<< std::endl;
    for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
        std::cout<<(*it).c_str()<< std::endl;
    }
    std::cout<<"##### END #####"<< std::endl;

    // TH1 objects
    TH1::SetDefaultSumw2();
    std::vector<TH1D*> vecH1D;

    TH1D* h_phoEt = new TH1D("h_phoEt", ";p_{T}^{#gamma} (GeV/c);", 60, 0, 150);
    TH1D* h_phoEta = new TH1D("h_phoEta", ";#eta^{#gamma};", 20, -1.44, 1.44);
    TH1D* h_phoPhi = new TH1D("h_phoPhi", ";#phi^{#gamma};", 20, -TMath::Pi(), TMath::Pi());
    vecH1D.push_back(h_phoEt);
    vecH1D.push_back(h_phoEta);
    vecH1D.push_back(h_phoPhi);

    TH1D* h_nJet = new TH1D("h_nJet", ";number of jets;", 20, 0, 20);
    TH1D* h_jetpt = new TH1D("h_jetpt", ";p_{T}^{jet} (GeV/c);", 60, 0, 150);
    TH1D* h_jeteta = new TH1D("h_jeteta", ";#eta^{jet};", 20, 1.6, 1.6);
    TH1D* h_jetphi = new TH1D("h_jetphi", ";#phi^{jet};", 20, -TMath::Pi(), TMath::Pi());
    vecH1D.push_back(h_nJet);
    vecH1D.push_back(h_jetpt);
    vecH1D.push_back(h_jeteta);
    vecH1D.push_back(h_jetphi);

    int nH1D = vecH1D.size();

    // TTree objects
    std::vector<std::string> treePaths = {"ggHiNtuplizerGED/EventTree", "ak3PFJetAnalyzer/t"};
    int nTrees = treePaths.size();
    std::vector<TTree*> trees(nTrees, 0);

    int nFiles = inputFiles.size();
    TFile* fileTmp = 0;

    std::cout << "initial reading to get the number of entries (if there is only one input file) and HiForest info" << std::endl;
    // read the first file only to get the HiForest info
    std::string inputPath = inputFiles.at(0).c_str();
    fileTmp = TFile::Open(inputPath.c_str(), "READ");
    fileTmp->cd();

    if (nFiles == 1) {
        // read one tree only to get the number of entries
        trees[0] = (TTree*)fileTmp->Get(treePaths[0].c_str());
        Long64_t entriesTmp = trees[0]->GetEntries();
        std::cout << "entries = " << entriesTmp << std::endl;
        trees[0]->Delete();
    }

    TTree* treeHiForestInfo = 0;
    treeHiForestInfo = (TTree*)fileTmp->Get("HiForest/HiForestInfo");
    HiForestInfoController hfic(treeHiForestInfo);
    std::cout<<"### HiForestInfo Tree ###"<< std::endl;
    hfic.printHiForestInfo();
    std::cout<<"###"<< std::endl;

    fileTmp->Close();
    // done with initial reading

    /*
    EventMatcher* em = new EventMatcher();
    Long64_t duplicateEntries = 0;
    */

    Long64_t entries = 0;
    Long64_t entriesAnalyzed = 0;

    std::cout<< "Loop STARTED" << std::endl;
    for (int iFile = 0; iFile < nFiles; ++iFile)  {

        std::string inputPath = inputFiles.at(iFile).c_str();
        std::cout <<"iFile = " << iFile << " , " ;
        std::cout <<"reading input file : " << inputPath.c_str() << std::endl;
        fileTmp = TFile::Open(inputPath.c_str(), "READ");

        // check if the file is usable, if not skip the file.
        if (isGoodFile(fileTmp) != 0) {
            std::cout << "File is not good. skipping file." << std::endl;
            continue;
        }

        for (int j = 0; j < nTrees; ++j) {
            trees[j] = (TTree*)fileTmp->Get(treePaths[j].c_str());
        }

        // branches for the 1st tree.
        int nPho;
        std::vector<float> *phoEt = 0;
        std::vector<float> *phoEta = 0;
        std::vector<float> *phoPhi = 0;
        // specify explicitly which branches to use, do not use wildcard
        trees[0]->SetBranchStatus("*", 0);     // disable all branches
        trees[0]->SetBranchStatus("nPho", 1);
        trees[0]->SetBranchStatus("phoEt", 1);
        trees[0]->SetBranchStatus("phoEta", 1);
        trees[0]->SetBranchStatus("phoPhi", 1);

        trees[0]->SetBranchAddress("nPho", &nPho);
        trees[0]->SetBranchAddress("phoEt", &phoEt);
        trees[0]->SetBranchAddress("phoEta", &phoEta);
        trees[0]->SetBranchAddress("phoPhi", &phoPhi);


        // branches for the 2nd tree.
        const int maxJets = 1000;
        int nref;
        float jtpt[maxJets];   //[nref]
        float jteta[maxJets];   //[nref]
        float jtphi[maxJets];   //[nref]
        // specify explicitly which branches to use, do not use wildcard
        trees[1]->SetBranchStatus("*", 0);     // disable all branches
        trees[1]->SetBranchStatus("nref", 1);
        trees[1]->SetBranchStatus("jtpt", 1);
        trees[1]->SetBranchStatus("jteta", 1);
        trees[1]->SetBranchStatus("jtphi", 1);

        trees[1]->SetBranchAddress("nref", &nref);
        trees[1]->SetBranchAddress("jtpt", jtpt);
        trees[1]->SetBranchAddress("jteta", jteta);
        trees[1]->SetBranchAddress("jtphi", jtphi);

        Long64_t entriesTmp = trees[0]->GetEntries();
        entries += entriesTmp;
        std::cout << "entries in File = " << entriesTmp << std::endl;
        for (Long64_t j_entry = 0; j_entry < entriesTmp; ++j_entry)
        {
            if (j_entry % 2000 == 0)  {
              std::cout << "current entry = " <<j_entry<<" out of "<<entriesTmp<<" : "<<std::setprecision(2)<<(double)j_entry/entriesTmp*100<<" %"<<std::endl;
            }

            for (int i = 0; i < nTrees; ++i) {
                trees[i]->GetEntry(j_entry);
            }

            /*
            bool eventAdded = em->addEvent(ggHi.run, ggHi.lumis, ggHi.event, j_entry);
            if(!eventAdded) // this event is duplicate, skip this one.
            {
                duplicateEntries++;
                continue;
            }
            */

            int indexMaxPt = -1;
            double maxPt = 0;
            for (int i = 0; i < nPho; ++i) {
                if (!(TMath::Abs((*phoEta)[i]) < 1.44)) continue;

                if ((*phoEt)[i] > maxPt) {
                    indexMaxPt = i;
                    maxPt = (*phoEt)[i];
                }
            }

            if (maxPt == 0) continue;
            entriesAnalyzed++;

            h_phoEt->Fill((*phoEt)[indexMaxPt]);
            h_phoEta->Fill((*phoEta)[indexMaxPt]);
            h_phoPhi->Fill((*phoPhi)[indexMaxPt]);

            int nJet = 0;
            for (int i = 0; i < nref; ++i) {

                if (!(TMath::Abs(jteta[i]) < 1.6)) continue;

                h_jetpt->Fill(jtpt[i]);

                if (!(jtpt[i] > 30)) continue;

                h_jeteta->Fill(jteta[i]);
                h_jetphi->Fill(jtphi[i]);

                nJet++;
            }

            h_nJet->Fill(nJet);
        }
        fileTmp->Close();
    }
    std::cout<<  "Loop ENDED" <<std::endl;
    std::cout << "entries            = " << entries << std::endl;
    //std::cout << "duplicateEntries   = " << duplicateEntries << std::endl;
    std::cout << "entriesAnalyzed    = " << entriesAnalyzed << std::endl;

    TFile* output = TFile::Open(outputFile.c_str(),"RECREATE");
    output->cd();

    for (int i = 0; i < nH1D; ++i) {
        setTH1D(vecH1D[i]);
        vecH1D[i]->Write("",TObject::kOverwrite);
    }

    std::cout<<"Closing the output file."<<std::endl;
    output->Close();
    std::cout<<"running nTupleHistogram() - END"<<std::endl;
}

int main(int argc, char** argv)
{
    if (argc == 3) {
        nTupleHistogram(argv[1], argv[2]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./nTupleHistogram.exe <inputFile> <outputFile>"
                << std::endl;
        return 1;
    }
}

void setTH1D(TH1D* h)
{
    h->SetMarkerStyle(kFullCircle);
    h->SetMarkerColor(kBlack);

    h->SetTitleOffset(1.25, "X");
    h->SetTitleOffset(1.5, "Y");
}
