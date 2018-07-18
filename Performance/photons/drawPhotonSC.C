/*
 * Macro to visualize the Super Cluster for a photon using position and energy of ECAL crystals
 * Branches for ECAL rechits belonging to a photon SC must be in the forest
 *   See https://github.com/CmsHI/cmssw/pull/153
 */
#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TObjArray.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TLine.h>
#include <TLatex.h>
#include <TStyle.h>

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>

#include "../../Utilities/interface/HiForestInfoController.h"
#include "../../Utilities/styleUtil.h"
#include "../../Utilities/fileUtil.h"
#include "../../Utilities/systemUtil.h"
#include "../../TreeHeaders/ggHiNtuplizerTree.h"

void drawPhotonSC(std::string inputFile, Long64_t entry = 0, int phoIdx = 0, std::string treePath = "ggHiNtuplizer/EventTree", int nBinsX = 11, int nBinsY = 21, std::string outputFile = "drawPhotonSC.root");
void drawBasicClusterBorders(TH2D* h2D, int** indicesBC, int nBinsX, int nBinsY);
void setBinsToPrecision(TH2D* h2D, int precision);
void setTH2D(TH2D* h);
void setLatex(TLatex* latex, std::string position);

void drawPhotonSC(std::string inputFile, Long64_t entry, int phoIdx, std::string treePath, int nBinsX, int nBinsY, std::string outputFile)
{
    std::cout << "running drawPhotonSC()" << std::endl;
    std::cout << "inputFile   = " << inputFile.c_str() << std::endl;
    std::cout << "outputFile  = " << outputFile.c_str() << std::endl;

    TH1::SetDefaultSumw2();

    TH2D* h2D = new TH2D(Form("h2D_photonSC_entry%lld_phoIdx%d", entry, phoIdx), ";ieta;iphi", nBinsX, 0, nBinsX, nBinsY, 0, nBinsY);

    int **bcIndices;
    bcIndices = new int *[nBinsX];
    for(int ix = 0; ix < nBinsX; ix++) {
        bcIndices[ix] = new int[nBinsY];
        for (int iy = 0; iy < nBinsY; ++iy) {
            bcIndices[ix][iy] = -1;
        }
    }

    std::cout <<"reading input file." << std::endl;
    TFile* input = TFile::Open(inputFile.c_str(), "READ");
    input->cd();

    // check if the file is usable, if not skip the file.
    if (isGoodFile(input) != 0) {
        std::cout << "File is not good." << std::endl;
        std::cout << "exiting." << std::endl;
        return;
    }

    TTree* treeHiForestInfo = (TTree*)input->Get("HiForest/HiForestInfo");
    HiForestInfoController hfic(treeHiForestInfo);
    std::cout<<"### HiForestInfo Tree ###"<< std::endl;
    hfic.printHiForestInfo();
    std::cout<<"###"<< std::endl;

    TTree* treeggHiNtuplizer = (TTree*)input->Get(treePath.c_str());
    Long64_t entries = treeggHiNtuplizer->GetEntries();
    std::cout << "entries = " << entries << std::endl;

    treeggHiNtuplizer = (TTree*)input->Get(treePath.c_str());
    treeggHiNtuplizer->SetBranchStatus("*",0);     // disable all branches

    treeggHiNtuplizer->SetBranchStatus("nPho",1);    // enable photon branches
    treeggHiNtuplizer->SetBranchStatus("pho*",1);    // enable photon branches
    treeggHiNtuplizer->SetBranchStatus("nRH",1);     // enable rechit branches
    treeggHiNtuplizer->SetBranchStatus("rh*",1);     // enable rechit branches

    ggHiNtuplizer ggHi;
    ggHi.setupTreeForReading(treeggHiNtuplizer);

    // rechit info
    int nRH = 0;
    std::vector<uint32_t> *rhRawId = 0;
    std::vector<int> *rhieta = 0;
    std::vector<int> *rhiphi = 0;
    std::vector<int> *rhix = 0;
    std::vector<int> *rhiy = 0;
    std::vector<float> *rhE = 0;
    std::vector<float> *rhEt = 0;
    std::vector<float> *rhEta = 0;
    std::vector<float> *rhPhi = 0;
    std::vector<float> *rhChi2 = 0;
    std::vector<float> *rhEerror = 0;
    std::vector<uint32_t> *rhFlags = 0;
    std::vector<int> *rhPhoIdx = 0;   // index of the photon this rechit belongs
    std::vector<int> *rhBCIdx = 0;    // index of this rechit's BC in the SC

    treeggHiNtuplizer->SetBranchAddress("nRH", &nRH);
    treeggHiNtuplizer->SetBranchAddress("rhRawId", &rhRawId);
    treeggHiNtuplizer->SetBranchAddress("rhieta", &rhieta);
    treeggHiNtuplizer->SetBranchAddress("rhiphi", &rhiphi);
    treeggHiNtuplizer->SetBranchAddress("rhix", &rhix);
    treeggHiNtuplizer->SetBranchAddress("rhiy", &rhiy);
    treeggHiNtuplizer->SetBranchAddress("rhE", &rhE);
    treeggHiNtuplizer->SetBranchAddress("rhEt", &rhEt);
    treeggHiNtuplizer->SetBranchAddress("rhEta", &rhEta);
    treeggHiNtuplizer->SetBranchAddress("rhPhi", &rhPhi);
    treeggHiNtuplizer->SetBranchAddress("rhChi2", &rhChi2);
    treeggHiNtuplizer->SetBranchAddress("rhEerror", &rhEerror);
    treeggHiNtuplizer->SetBranchAddress("rhFlags", &rhFlags);
    treeggHiNtuplizer->SetBranchAddress("rhPhoIdx", &rhPhoIdx);
    treeggHiNtuplizer->SetBranchAddress("rhBCIdx", &rhBCIdx);

    std::cout<<  "drawing : " << treePath.c_str() <<std::endl;
    std::cout << "entry  = " << entry << std::endl;
    std::cout << "phoIdx = " << phoIdx << std::endl;

    treeggHiNtuplizer->GetEntry(entry);
    int seedHit = -1;
    double maxHitEnergy = -1;
    std::cout<<  "nRH = " << nRH <<std::endl;
    // find the seed hit
    for (int iHit = 0; iHit < nRH; ++iHit) {

        if ((*rhPhoIdx)[iHit] != phoIdx) continue;

        if ((*rhE)[iHit] > maxHitEnergy) {
            maxHitEnergy = (*rhE)[iHit];
            seedHit = iHit;
        }
    }

    double sumRhE = 0;
    for (int iHit = 0; iHit < nRH; ++iHit) {

        if ((*rhPhoIdx)[iHit] != phoIdx) continue;

        sumRhE += (*rhE)[iHit];

        int centerBinX = (nBinsX % 2 == 0) ? nBinsX/2 : nBinsX/2 + 1;
        int centerBinY = (nBinsY % 2 == 0) ? nBinsY/2 : nBinsY/2 + 1;

        // put the first hit at the center of the histogram
        int iBinX = centerBinX + (*rhieta)[iHit] - (*rhieta)[seedHit];
        int iBinY = centerBinY + (*rhiphi)[iHit] - (*rhiphi)[seedHit];

        if (iBinX < 1 || iBinX > nBinsX || iBinY < 1 || iBinY > nBinsY) {
            std::cout << "Warning : Calculated bins are out of TH2D bin range." << std::endl;
            std::cout << "iBinX = " << iBinX << std::endl;
            std::cout << "iBinY = " << iBinY << std::endl;
            std::cout << "not showing this RecHit." << std::endl;
            continue;
        }

        h2D->SetBinContent(iBinX, iBinY, (*rhE)[iHit]);
        h2D->SetBinError(iBinX, iBinY, 1);

        bcIndices[iBinX-1][iBinY-1] = (*rhBCIdx)[iHit];
    }
    // check rechit energies vs. SC raw energy
    std::cout << "SC raw energy - sum(rechit E) = " << (*ggHi.phoSCRawE)[phoIdx] - sumRhE << std::endl;

    std::cout << "Closing the input file."<<std::endl;
    input->Close();

    TFile* output = TFile::Open(outputFile.c_str(),"update");
    output->cd();

    setTH2D(h2D);
    h2D->Write("",TObject::kOverwrite);

    TH2D* h2Dtext = (TH2D*)h2D->Clone(Form("%s_text", h2D->GetName()));
    setBinsToPrecision(h2Dtext, 2);

    std::cout<<"writing canvases."<<std::endl;
    TCanvas* c = 0;
    float leftMargin = 0.1;
    float rightMargin = 0.1;
    float bottomMargin = 0.1;
    float topMargin = 0.1;
    TLatex* latex = 0;

    std::vector<std::string> textLines = {
            Form("p^{#gamma}_{T} = %.1f", (*ggHi.phoEt)[phoIdx]),
            Form("SC E = %.1f", (*ggHi.phoE)[phoIdx]),
            Form("SC Raw E = %.2f", (*ggHi.phoSCRawE)[phoIdx]),
            Form("#eta^{SC} = %.2f", (*ggHi.phoSCEta)[phoIdx]),
            Form("#phi^{SC} = %.2f", (*ggHi.phoSCPhi)[phoIdx])
    };

    c = new TCanvas(Form("cnv_photonSC_entry%lld_phoIdx%d", entry, phoIdx), "", 800, 800);
    setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
    h2D->Draw("colz");
    drawBasicClusterBorders(h2D, bcIndices, nBinsX, nBinsY);
    latex = new TLatex();
    setLatex(latex, "NE");
    drawTextLines(latex, c, textLines, "NE", 0.04, 0.08);
    setCanvasFinal(c);
    c->Write("",TObject::kOverwrite);
    c->Close();

    c = new TCanvas(Form("cnv_photonSC_entry%lld_phoIdx%d_textE", entry, phoIdx), "", 800, 800);
    setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
    h2D->Draw("colz");
    h2Dtext->SetMarkerSize(1.25);
    h2Dtext->Draw("text same");
    drawBasicClusterBorders(h2D, bcIndices, nBinsX, nBinsY);
    latex = new TLatex();
    setLatex(latex, "NE");
    drawTextLines(latex, c, textLines, "NE", 0.04, 0.08);
    setCanvasFinal(c);
    c->Write("",TObject::kOverwrite);
    c->Close();

    c = new TCanvas(Form("cnv_photonSC_entry%lld_phoIdx%d_lego2z", entry, phoIdx), "", 800, 800);
    setCanvasMargin(c, leftMargin, rightMargin, bottomMargin, topMargin);
    h2D->Draw("lego2z");
    latex = new TLatex();
    setLatex(latex, "NW");
    drawTextLines(latex, c, textLines, "NW", 0.04, 0.04);
    setCanvasFinal(c);
    c->Write("",TObject::kOverwrite);
    c->Close();

    std::cout<<"Closing the output file."<<std::endl;
    output->Close();

    std::cout<<"running drawPhotonSC() - END"<<std::endl;
}

/*
 * draw lines between to show the borders of basic clusters.
 * The seed BC is bordered by dashed lines
 */
void drawBasicClusterBorders(TH2D* h2D, int** indicesBC, int nBinsX, int nBinsY)
{
    for (int ix = 0; ix < nBinsX - 1; ++ix) {
        for (int iy = 0; iy < nBinsY - 1; ++iy) {

            float lineWidthFactor = 4;
            int lineStyle = kSolid;
            // draw lines along y-direction
            if (indicesBC[ix][iy] != indicesBC[ix+1][iy]) {
                double x = h2D->GetXaxis()->GetBinLowEdge(ix+2);
                double y1 = h2D->GetYaxis()->GetBinLowEdge(iy+1);
                double y2 = h2D->GetYaxis()->GetBinLowEdge(iy+2);
                TLine line(x, y1, x, y2);
                line.SetLineColor(kRed);
                if (indicesBC[ix][iy] == 0 || indicesBC[ix+1][iy] == 0) {
                    lineWidthFactor = 6;
                    lineStyle = kDashed;
                }
                line.SetLineWidth(line.GetLineWidth() * lineWidthFactor);
                line.SetLineStyle(lineStyle);
                line.DrawClone();
            }
            // draw lines along x-direction
            if (indicesBC[ix][iy] != indicesBC[ix][iy+1]) {
                double x1 = h2D->GetXaxis()->GetBinLowEdge(ix+1);
                double x2 = h2D->GetXaxis()->GetBinLowEdge(ix+2);
                double y = h2D->GetYaxis()->GetBinLowEdge(iy+2);
                TLine line(x1, y, x2, y);
                line.SetLineColor(kRed);
                if (indicesBC[ix][iy] == 0 || indicesBC[ix][iy+1] == 0) {
                    lineWidthFactor = 6;
                    lineStyle = kDashed;
                }
                line.SetLineWidth(line.GetLineWidth() * lineWidthFactor);
                line.SetLineStyle(lineStyle);
                line.DrawClone();
            }
        }
    }
}

void setBinsToPrecision(TH2D* h2D, int precision)
{
    int nBinsX = h2D->GetNbinsX();
    int nBinsY = h2D->GetNbinsY();
    for (int iX = 1; iX <= nBinsX; ++iX) {
        for (int iY = 1; iY <= nBinsY; ++iY) {
            double binContent = h2D->GetBinContent(iX, iY);
            double binError = h2D->GetBinError(iX, iY);

            h2D->SetBinContent(iX, iY, roundToPrecision(binContent, precision));
            h2D->SetBinError(iX, iY, roundToPrecision(binError, precision));
        }
    }
}

void setTH2D(TH2D* h)
{
    h->SetTitleOffset(1.25, "X");
    h->SetTitleOffset(1.25,  "Y");

    h->SetTitle("");
    h->SetStats(false);

    h->GetXaxis()->CenterTitle();
    h->GetYaxis()->CenterTitle();
}

void setLatex(TLatex* latex, std::string position)
{
    setTextAlignment(latex, position.c_str());
    latex->SetTextFont(43);
    latex->SetTextSize(32);
}

int main(int argc, char** argv)
{
    if (argc == 8) {
        drawPhotonSC(argv[1], std::atoll(argv[2]), std::atoi(argv[3]), argv[4], std::atoi(argv[5]), std::atoi(argv[6]), argv[7]);
        return 0;
    }
    else if (argc == 7) {
        drawPhotonSC(argv[1], std::atoll(argv[2]), std::atoi(argv[3]), argv[4], std::atoi(argv[5]), std::atoi(argv[6]));
        return 0;
    }
    else if (argc == 6) {
        drawPhotonSC(argv[1], std::atoll(argv[2]), std::atoi(argv[3]), argv[4], std::atoi(argv[5]));
        return 0;
    }
    else if (argc == 5) {
        drawPhotonSC(argv[1], std::atoll(argv[2]), std::atoi(argv[3]), argv[4]);
        return 0;
    }
    else if (argc == 4) {
        drawPhotonSC(argv[1], std::atoll(argv[2]), std::atoi(argv[3]));
        return 0;
    }
    else if (argc == 3) {
        drawPhotonSC(argv[1], std::atoll(argv[2]));
        return 0;
    }
    else if (argc == 2) {
        drawPhotonSC(argv[1]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./drawPhotonSC.exe <inputFile> <entry> <phoIdx> <treePath> <nBinsX> <nBinsY> <outputFile>"
                << std::endl;
        return 1;
    }
}
