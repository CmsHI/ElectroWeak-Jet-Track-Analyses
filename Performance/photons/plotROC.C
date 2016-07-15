#include<stdio.h>
#include "TFile.h"
#include "TTree.h"
#include "TApplication.h"
#include "TPad.h"

void plotROC(const char in_filepath[] = "/net/hisrv0001/home/juliusbl/ElectroWeak-Jet-Track-Analyses/TMVA.root") {
    TFile* infile = TFile::Open(in_filepath);
    TTree* TestTree = (TTree*) infile->Get("TestTree");

    TFile* outfile = TFile::Open("ROCTree.root", "RECREATE");
    TTree* ROC = new TTree("ROC", "ROCTree");

    int allphos = TestTree->GetEntries();
    printf("\nallphos: %d\n", allphos);

    int nloops = allphos;
    printf("nloops: %d\n", nloops);

    int classID = 0; TestTree->SetBranchAddress("classID", &classID);
    float LD = 0; TestTree->SetBranchAddress("LD", &LD);
    // float prob_LD = 0; TestTree->SetBranchAddress("prob_LD", &prob_LD);

    float LD_cut_OUT;
    float Backg_Rej_OUT;
    float Sign_Eff_OUT;

    ROC->Branch("LD_cut", &LD_cut_OUT, "LD_cut/F");
    ROC->Branch("Backg_Rej", &Backg_Rej_OUT, "Backg_Rej/F");
    ROC->Branch("Sign_Eff", &Sign_Eff_OUT, "Sign_Eff/F");

    float maxLD = 0;
    float minLD = 0;
    int n_real = 0;
    for (int pho_i = 0; pho_i < nloops; ++pho_i) {
        TestTree->GetEntry(pho_i);

        if (LD > maxLD) {
            maxLD = LD;
        }
        else if (LD < minLD) {
            minLD = LD;
        }
        else if (classID == 0) {
            ++n_real;
        }
    }
    int n_fake = allphos - n_real;

    float iterMax = 500.0;
    float delta = (maxLD - minLD) / iterMax;
    float cutVal = minLD;

    for (float iter = 0.0; iter < iterMax; ++iter) {
        float true_positives = 0;
        float false_positives = 0;
        for (int pho_i=0; pho_i < nloops; ++pho_i) {
            TestTree->GetEntry(pho_i);
            if ((classID == 0) && (LD > cutVal)) {
                ++true_positives;
            }
            else if ((classID == 1) && (LD > cutVal))
                ++false_positives;
        }
        float tpr = true_positives / n_real;
        float fpr = false_positives / n_fake;

        LD_cut_OUT = cutVal;
        Backg_Rej_OUT = tpr;
        Sign_Eff_OUT = 1 - fpr;

        ROC->Fill();

        cutVal = cutVal + delta;
    }

    printf("Loop done.\n" );
    outfile->Write();

    ROC->Draw("Backg_Rej:Sign_Eff");
    gPad->WaitPrimitive();

    outfile->Close();
    printf("\nDone! \n");
}


int main(int argc, char *argv[]) {

    TApplication theAPP("App", &argc, argv);
    plotROC();
    theAPP.Terminate();

    return 0;
}
