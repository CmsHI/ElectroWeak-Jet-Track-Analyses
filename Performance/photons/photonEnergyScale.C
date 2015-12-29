/*
 * macro to draw eta dependent photon energy scale plots.
 * saves histograms to a .root file.
 * if "outputFigureName" is specified, saves the canvas as picture.
 */

#include <TFile.h>
#include <TChain.h>
#include <TH1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TObjArray.h>
#include <TCanvas.h>
#include <TLine.h>

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include "../../CorrelationTuple/EventMatcher.h"
#include "../../TreeHeaders/CutConfigurationTree.h"
#include "../../TreeHeaders/ggHiNtuplizerTree.h"
#include "../../Utilities/interface/CutConfigurationParser.h"
#include "../../Utilities/interface/InputConfigurationParser.h"
#include "../../Utilities/styleUtil.h"
#include "../interface/energyScaleHist.h"


void photonEnergyScale(const TString configFile, const TString inputFile, const TString outputFile = "photonEnergyScale.root", const TString outputFigureName = "");
void setCanvas_energyScale(TCanvas* c);
void setTH1_energyScale(TH1* h);
void setTH1_energyWidth(TH1* h);

void photonEnergyScale(const TString configFile, const TString inputFile, const TString outputFile, const TString outputFigureName)
{
    std::cout<<"running photonEnergyScale()"<<std::endl;
    std::cout<<"configFile  = "<< configFile.Data() <<std::endl;
    std::cout<<"inputFile   = "<< inputFile.Data()  <<std::endl;
    std::cout<<"outputFile  = "<< outputFile.Data() <<std::endl;

    bool doHoverE = false;
    std::cout << "doHoverE = " << doHoverE << std::endl;

    InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

    // input configuration
    std::vector<int> bins_hiBin[2];         // array of vectors for hiBin bins, each array element is a vector.
    int collision;
    std::string legendPosition;
    if (configInput.isValid) {
        collision = configInput.proc[INPUT::kSKIM].i[INPUT::k_CollisionType];
        bins_hiBin[0] = ConfigurationParser::ParseListInteger(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_hiBin_List_gt]);
        bins_hiBin[1] = ConfigurationParser::ParseListInteger(configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_hiBin_List_lt]);

        legendPosition = configInput.proc[INPUT::kPERFORMANCE].s[INPUT::k_legendPosition];
    }
    else {
        collision = COLL::kPP;

        bins_hiBin[0].push_back(0);
        bins_hiBin[1].push_back(200);

        legendPosition = "NW";
    }
    const char* collisionName =  getCollisionTypeName((COLL::TYPE)collision).c_str();
    int nBins_hiBin = bins_hiBin[0].size();     // assume <myvector>[0] and <myvector>[1] have the same size.
    // verbose about input configuration
    std::cout<<"Input Configuration :"<<std::endl;
    std::cout << "collision = " << collisionName << std::endl;
    std::cout << "nBins_hiBin = " << nBins_hiBin << std::endl;
    for (int i=0; i<nBins_hiBin; ++i) {
        std::cout << Form("bins_hiBin[%d] = [%d, %d)", i, bins_hiBin[0].at(i), bins_hiBin[1].at(i)) << std::endl;
    }
    std::cout << "legendPosition = " << legendPosition.c_str() << std::endl;

    std::vector<float> bins_eta[2];         // array of vectors for eta bins, each array element is a vector.
    std::vector<float> cuts_genPt;        // list of pt cuts for GEN-level photons matched to RECO photons
    std::vector<float> cuts_recoPt;       // list of pt cuts for RECO photons
    if (configCuts.isValid) {
        bins_eta[0] = ConfigurationParser::ParseListFloat(
                configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_eta_gt]);
        bins_eta[1] = ConfigurationParser::ParseListFloat(
                configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_eta_lt]);

        cuts_genPt = ConfigurationParser::ParseListFloat(
                configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_mcPt_List]);
        cuts_recoPt = ConfigurationParser::ParseListFloat(
                configCuts.proc[CUTS::kPERFORMANCE].obj[CUTS::kPHOTON].s[CUTS::PHO::k_pt_List]);
    }
    else {
        bins_eta[0].push_back(0);
        bins_eta[1].push_back(2.4);

        cuts_genPt.push_back(15);
        cuts_recoPt.push_back(0);
    }
    int nBins_eta = bins_eta[0].size();     // assume <myvector>[0] and <myvector>[1] have the same size.
    int nCuts_genPt = cuts_genPt.size();
    int nCuts_recoPt = cuts_recoPt.size();
    // verbose about cut configuration
    std::cout<<"Cut Configuration :"<<std::endl;
    std::cout << "nBins_eta = " << nBins_eta << std::endl;
    for (int i=0; i<nBins_eta; ++i) {
        std::cout << Form("bins_eta[%d] = [%f, %f)", i, bins_eta[0].at(i), bins_eta[1].at(i)) << std::endl;
    }
    std::cout << "nCuts_genPt = " << nCuts_genPt << std::endl;
    for (int i=0; i<nCuts_genPt; ++i) {
        std::cout << Form("cuts_genPt[%d] = ", i) << cuts_genPt.at(i) << std::endl;
    }
    std::cout << "nCuts_recoPt = " << nCuts_recoPt << std::endl;
    for (int i=0; i<nCuts_recoPt; ++i) {
        std::cout << Form("cuts_recoPt[%d] = ", i) << cuts_recoPt.at(i) << std::endl;
    }

    std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.Data());

    std::cout<<"input ROOT files : num = "<<inputFiles.size()<< std::endl;
    std::cout<<"#####"<< std::endl;
    for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
        std::cout<<(*it).c_str()<< std::endl;
    }
    std::cout<<"##### END #####"<< std::endl;

    bool isHI = collisionIsHI((COLL::TYPE)collision);

    TChain* treeHLT = new TChain("hltanalysis/HltTree");
    TChain* treeggHiNtuplizer = new TChain("ggHiNtuplizer/EventTree");
    TChain* treeHiEvt;
    bool hasHiEvt = false;
    if (isHI) {
        treeHiEvt = new TChain("hiEvtAnalyzer/HiTree");
        hasHiEvt  = true;
    }
    else {
        treeHiEvt = 0;
        hasHiEvt  = false;
    }

    for (std::vector<std::string>::iterator it = inputFiles.begin() ; it != inputFiles.end(); ++it) {
       treeHLT->Add((*it).c_str());
       treeggHiNtuplizer->Add((*it).c_str());
       if(hasHiEvt) treeHiEvt->Add((*it).c_str());
    }

    treeHLT->SetBranchStatus("*",0);     // disable all branches
    treeHLT->SetBranchStatus("HLT_HI*SinglePhoton*Eta*v1*",1);     // enable photon branches
    treeHLT->SetBranchStatus("HLT_HI*DoublePhoton*Eta*v1*",1);     // enable photon branches

    treeggHiNtuplizer->SetBranchStatus("*",0);     // disable all branches
    treeggHiNtuplizer->SetBranchStatus("run",1);    // enable event information
    treeggHiNtuplizer->SetBranchStatus("event",1);
    treeggHiNtuplizer->SetBranchStatus("lumis",1);

    treeggHiNtuplizer->SetBranchStatus("nPho",1);     // enable photon branches
    treeggHiNtuplizer->SetBranchStatus("pho*",1);     // enable photon branches
    treeggHiNtuplizer->SetBranchStatus("mc*",1);      // enable GEN particle branches
    // check existence of genMatching branch
    if (!treeggHiNtuplizer->GetBranch("pho_genMatchedIndex")) {
        std::cout << "WARNING : Branch pho_genMatchedIndex does not exist." <<std::endl;
    }

    // specify explicitly which branches to use, do not use wildcard
    Int_t hiBin;
    if (hasHiEvt) {
        treeHiEvt->SetBranchStatus("*",0);
        treeHiEvt->SetBranchStatus("hiBin",1);

        treeHiEvt->SetBranchAddress("hiBin",&hiBin);
    }
    else {   // overwrite to default
        hiBin = 0;
    }

    ggHiNtuplizer ggHi;
    ggHi.setupTreeForReading(treeggHiNtuplizer);

    TH1::SetDefaultSumw2();
    energyScaleHist hist_depEta[nBins_hiBin][nCuts_genPt][nCuts_recoPt];   // energyScaleHist where x-axis is eta
    // eta dependence of energy scale : x-axis is eta
    for (int iHibin = 0; iHibin < nBins_hiBin; ++iHibin) {
        for (int iGenPt = 0; iGenPt < nCuts_genPt; ++iGenPt) {
            for (int iRecoPt = 0; iRecoPt < nCuts_recoPt; ++iRecoPt) {

                // prepare histogram name
                std::string tmpName = Form("depEta_hiBin%d_genPtBin%d_recoPtBin%d", iHibin, iGenPt, iRecoPt);
                hist_depEta[iHibin][iGenPt][iRecoPt].name = tmpName.c_str();

                float xUp  = bins_eta[1].at(0);
                std::string tmpHistName = Form("h2D_%s", tmpName.c_str());
                hist_depEta[iHibin][iGenPt][iRecoPt].h2D =
                        new TH2D(tmpHistName.c_str(), ";photon #eta;Reco p_{T} / Gen p_{T}", 12, -xUp, xUp, 100, 0, 2);

                // histogram ranges
                hist_depEta[iHibin][iGenPt][iRecoPt].range_genPt[0] = cuts_genPt.at(iGenPt);
                hist_depEta[iHibin][iGenPt][iRecoPt].range_recoPt[0] = cuts_recoPt.at(iRecoPt);
                hist_depEta[iHibin][iGenPt][iRecoPt].range_hiBin[0] = bins_hiBin[0].at(iHibin);
                hist_depEta[iHibin][iGenPt][iRecoPt].range_hiBin[1] = bins_hiBin[1].at(iHibin);

                // prepare histogram title
                std::string hiBinStr  = Form("hiBin:%d-%d%%", bins_hiBin[0].at(iHibin)/2, bins_hiBin[1].at(iHibin)/2);
                std::string genPtStr  = Form("p_{T}^{#gamma}(GEN)>%.0f",  cuts_genPt.at(iGenPt));
                std::string recoPtStr = Form("p_{T}^{#gamma}(RECO)>%.0f", cuts_recoPt.at(iRecoPt));

                // special cases
                if (bins_hiBin[0].at(iHibin) <= 0 && bins_hiBin[1].at(iHibin) >=200) hiBinStr = "";     // whole centrality range
                if (cuts_genPt.at(iGenPt)    <= 0) genPtStr  = "";       // whole pT range
                if (cuts_recoPt.at(iRecoPt)  <= 0) recoPtStr = "";       // whole pT range

                std::string tmpHistTitle = "";
                if (hiBinStr.size() > 0)  tmpHistTitle = Form("%s", hiBinStr.c_str());
                if (genPtStr.size() > 0)  tmpHistTitle = Form("%s %s", tmpHistTitle.c_str(), genPtStr.c_str());
                if (recoPtStr.size() > 0) tmpHistTitle = Form("%s %s", tmpHistTitle.c_str(), recoPtStr.c_str());

                hist_depEta[iHibin][iGenPt][iRecoPt].title = tmpHistTitle.c_str();
                hist_depEta[iHibin][iGenPt][iRecoPt].h2D->SetTitle(tmpHistTitle.c_str());
            }
        }
    }

    EventMatcher* em = new EventMatcher();
    Long64_t duplicateEntries = 0;

    Long64_t entries = treeggHiNtuplizer->GetEntries();
    Long64_t entriesAnalyzed = 0;
    std::cout << "entries = " << entries << std::endl;
    std::cout<< "Loop : ggHiNtuplizer/EventTree" <<std::endl;
    for (Long64_t j_entry = 0; j_entry < entries; ++j_entry)
    {
        if (j_entry % 2000 == 0)  {
          std::cout << "current entry = " <<j_entry<<" out of "<<entries<<" : "<<std::setprecision(2)<<(double)j_entry/entries*100<<" %"<<std::endl;
        }

        treeHLT->GetEntry(j_entry);
        treeggHiNtuplizer->GetEntry(j_entry);
        if (hasHiEvt) treeHiEvt->GetEntry(j_entry);

        bool eventAdded = em->addEvent(ggHi.run, ggHi.lumis, ggHi.event, j_entry);
        if(!eventAdded) // this event is duplicate, skip this one.
        {
            duplicateEntries++;
            continue;
        }

        entriesAnalyzed++;

        // energy scale
        for (int i=0; i<ggHi.nPho; ++i) {
            // selections on GEN particle
            if (ggHi.pho_genMatchedIndex->at(i) < 0)   continue;    // is matched
            if (ggHi.mcPID->at(ggHi.pho_genMatchedIndex->at(i)) != 22)   continue;    // is matched to a photon

            // selections on RECO particle
            if (!(ggHi.phoSigmaIEtaIEta->at(i) > 0.002 && ggHi.pho_swissCrx->at(i) < 0.9 && TMath::Abs(ggHi.pho_seedTime->at(i)) < 3)) continue;
            if (doHoverE) {
                if (!(ggHi.phoHoverE->at(i) < 0.1)) continue;
            }

            double mcPt = ggHi.mcPt->at(ggHi.pho_genMatchedIndex->at(i));
            if (mcPt <= 0)   continue;

            double eta = ggHi.phoEta->at(i);
            double pt  = ggHi.phoEt->at(i);
            double energyScale = pt/mcPt;

            // eta dependence of energy scale : x-axis is eta
            for (int iHibin = 0;  iHibin < nBins_hiBin; ++iHibin)
            for (int iGenPt = 0;  iGenPt < nCuts_genPt; ++iGenPt)
            for (int iRecoPt = 0; iRecoPt < nCuts_recoPt; ++iRecoPt)
                 hist_depEta[iHibin][iGenPt][iRecoPt].Fillh2D(energyScale, eta, -1, mcPt, pt, hiBin);

        }
    }
    std::cout<<  "Loop ENDED : ggHiNtuplizer/EventTree" <<std::endl;
    std::cout << "entries            = " << entries << std::endl;
    std::cout << "duplicateEntries   = " << duplicateEntries << std::endl;
    std::cout << "entriesAnalyzed    = " << entriesAnalyzed << std::endl;

    TFile *output = TFile::Open(outputFile.Data(),"RECREATE");
    output->cd();

    // declaration of graphics objects.
    // the idea is to initialize each Graphics object right before its use, then to delete it right after they are saved/written.
    TCanvas* c;

    for (int iHibin = 0; iHibin < nBins_hiBin; ++iHibin) {
        for (int iGenPt = 0; iGenPt < nCuts_genPt; ++iGenPt) {
            for (int iRecoPt = 0; iRecoPt < nCuts_recoPt; ++iRecoPt) {

                hist_depEta[iHibin][iGenPt][iRecoPt].h2D->Write();
                TObjArray aSlices;
                hist_depEta[iHibin][iGenPt][iRecoPt].h2D->FitSlicesY(0,0,-1,0,"Q LL m", &aSlices);

                std::string canvasName;
                std::string name = hist_depEta[iHibin][iGenPt][iRecoPt].name.c_str();
                std::string title = hist_depEta[iHibin][iGenPt][iRecoPt].title.c_str();
                // energy scale
                canvasName = Form("cnv_eScale_%s", name.c_str());
                c = new TCanvas(canvasName.c_str(), "", 600, 600);
                c->cd();
                setCanvas_energyScale(c);

                hist_depEta[iHibin][iGenPt][iRecoPt].h1D[0] = (TH1D*)aSlices.At(1)->Clone(Form("h1D_eScale_%s", name.c_str()));
                hist_depEta[iHibin][iGenPt][iRecoPt].h1D[0]->SetTitle(title.c_str());
                setTH1_energyScale(hist_depEta[iHibin][iGenPt][iRecoPt].h1D[0]);
                hist_depEta[iHibin][iGenPt][iRecoPt].h1D[0]->Draw("e");
                hist_depEta[iHibin][iGenPt][iRecoPt].h1D[0]->Write();

                // draw line y = 1
                float x1 = hist_depEta[iHibin][iGenPt][iRecoPt].h1D[0]->GetXaxis()->GetXmin();
                float x2 = hist_depEta[iHibin][iGenPt][iRecoPt].h1D[0]->GetXaxis()->GetXmax();
                TLine line(x1, 1, x2,1);
                line.SetLineStyle(kDashed);
                line.Draw();
                setCanvasFinal(c);
                c->Write();
                c->Close();         // do not use Delete() for TCanvas.

                // width of energy scale
                canvasName = Form("cnv_eRes_%s", name.c_str());
                c = new TCanvas(canvasName.c_str(), "", 600, 600);
                c->cd();
                setCanvas_energyScale(c);
                hist_depEta[iHibin][iGenPt][iRecoPt].h1D[1] = (TH1D*)aSlices.At(2)->Clone(Form("h1D_eRes_%s", name.c_str()));
                hist_depEta[iHibin][iGenPt][iRecoPt].h1D[1]->SetTitle(title.c_str());
                setTH1_energyWidth(hist_depEta[iHibin][iGenPt][iRecoPt].h1D[1]);
                hist_depEta[iHibin][iGenPt][iRecoPt].h1D[1]->Draw("e");
                hist_depEta[iHibin][iGenPt][iRecoPt].h1D[1]->Write();
                setCanvasFinal(c);
                c->Write();
                c->Close();         // do not use Delete() for TCanvas.
            }
        }
    }

    // assume not more than 13 curves are drawn.
    TLegend* legend;
    TLine* line;
    int colors[13] = {kBlack, kBlue, kRed,   kOrange,  kViolet, kCyan, kSpring, kYellow,
                      kAzure, kPink, kGreen, kMagenta, kTeal};
    // draw onto same canvas : x-Axis is eta.
    for (int iGenPt = 0; iGenPt < nCuts_genPt; ++iGenPt) {
        for (int iRecoPt = 0; iRecoPt < nCuts_recoPt; ++iRecoPt) {

            std::string genPtStr = Form("p_{T}^{#gamma}(GEN)>%.0f", cuts_genPt.at(iGenPt));
            std::string recoPtStr = Form("p_{T}^{#gamma}(RECO)>%.0f", cuts_recoPt.at(iRecoPt));

            // special cases
            if (cuts_genPt.at(iGenPt)   <= 0) genPtStr  = "";       // whole pT range
            if (cuts_recoPt.at(iRecoPt) <= 0) recoPtStr = "";       // whole pT range

            std::string tmpHistTitle = "";
            if (genPtStr.size() > 0)  tmpHistTitle = Form("%s", genPtStr.c_str());
            if (recoPtStr.size() > 0) tmpHistTitle = Form("%s, %s", tmpHistTitle.c_str(), recoPtStr.c_str());

            std::string name = Form ("eScale_depEta_hiBinAll_genPtBin%d_recoPtBin%d", iGenPt, iRecoPt);
            std::string canvasName;
            // energy scale
            canvasName = Form("cnv_%s", name.c_str());
            c = new TCanvas(canvasName.c_str(), "", 600, 600);
            c->cd();
            setCanvas_energyScale(c);

            // histograms for different centralities
            legend = new TLegend();
            legend->SetHeader(tmpHistTitle.c_str());
            for (int i = 0; i < nBins_hiBin; ++i) {

                setTH1_energyScale(hist_depEta[i][iGenPt][iRecoPt].h1D[0]);
                hist_depEta[i][iGenPt][iRecoPt].h1D[0]->SetMarkerColor(colors[i]);
                hist_depEta[i][iGenPt][iRecoPt].h1D[0]->Draw("e same");

                std::string hiBinStr = Form("hiBin:%d-%d%%", bins_hiBin[0].at(i)/2, bins_hiBin[1].at(i)/2);
                legend->AddEntry(hist_depEta[i][iGenPt][iRecoPt].h1D[0], hiBinStr.c_str(), "lp");
            }
            double height = calcTLegendHeight(legend, 0.0475);
            double width = calcTLegendWidth(legend);
            setLegendPosition(legend, legendPosition, c, height, width);
            legend->Draw();

            // draw line y = 1
            if (nBins_hiBin > 0) {
                float x1 = hist_depEta[0][iGenPt][iRecoPt].h1D[0]->GetXaxis()->GetXmin();
                float x2 = hist_depEta[0][iGenPt][iRecoPt].h1D[0]->GetXaxis()->GetXmax();
                line = new TLine(x1, 1, x2, 1);
                line->SetLineStyle(kDashed);
                line->Draw();
            }

            setCanvasFinal(c);
            c->Write();

            // save canvas as picture if a figure name is provided.
            if (!outputFigureName.EqualTo("")) {
                c->SaveAs(Form("%s_%s.C", outputFigureName.Data(), name.c_str()));
                c->SaveAs(Form("%s_%s.png", outputFigureName.Data(), name.c_str()));
                c->SaveAs(Form("%s_%s.pdf", outputFigureName.Data(), name.c_str()));
            }

            legend->Delete();
            c->Close();         // do not use Delete() for TCanvas.
        }
    }

    output->Close();
}

int main(int argc, char** argv)
{
    if (argc == 5) {
        photonEnergyScale(argv[1], argv[2], argv[3], argv[4]);
        return 0;
    }
    else if (argc == 4) {
        photonEnergyScale(argv[1], argv[2], argv[3]);
        return 0;
    }
    else if (argc == 3) {
        photonEnergyScale(argv[1], argv[2]);
        return 0;
    }
    else {
        std::cout << "Usage : \n" <<
                "./photonEnergyScale.exe <configFile> <inputFile> <outputFile> (<outputFigureName>)"
                << std::endl;
        return 1;
    }
}

void setCanvas_energyScale(TCanvas* c) {

    c->Size(600*(1+0.2),600);
    c->SetLeftMargin(0.15);
    c->SetRightMargin(0.05);
}

void setTH1_energyScale(TH1* h) {

    h->SetXTitle("photon #eta");
    h->SetYTitle("Reco p_{T} / Gen p_{T}");
    h->SetTitleOffset(1.5, "Y");
    h->SetTitleOffset(1.1, "X");
    h->SetAxisRange(0.8,1.5,"Y");
    h->SetStats(false);
    h->SetMarkerStyle(kFullCircle);
}

void setTH1_energyWidth(TH1* h) {

    h->SetXTitle("photon #eta");
    h->SetYTitle("#sigma( Reco p_{T} / Gen p_{T} )");
    h->SetTitleOffset(1.5, "Y");
    h->SetTitleOffset(1.1, "X");
    h->SetAxisRange(0,0.5,"Y");
    h->SetStats(false);
    h->SetMarkerStyle(kFullCircle);
}
