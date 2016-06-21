#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"

#include "../Utilities/interface/CutConfigurationParser.h"
#include "../Utilities/interface/InputConfigurationParser.h"
#include "../TreeHeaders/CutConfigurationTree.h"
#include "../TreeHeaders/hiEvtTree.h"

#include <vector>

void PbPbWeights(TH2D* h_cent_vertex, TH2D* h_mc_cent_vertex, TH2D* h_weights) {
    TH1D* h_cent_vertex_px = (TH1D*)h_cent_vertex->ProjectionX();
    h_cent_vertex_px->Fit("pol9");
    TF1* f_cent = (TF1*)h_cent_vertex_px->GetFunction("pol9");

    TH1D* h_cent_vertex_py = (TH1D*)h_cent_vertex->ProjectionY();
    h_cent_vertex_py->Fit("gaus");
    TF1* f_vertex = (TF1*)h_cent_vertex_py->GetFunction("gaus");

    TH1D* h_mc_cent_vertex_px = (TH1D*)h_mc_cent_vertex->ProjectionX();
    h_mc_cent_vertex_px->Fit("pol9", "", "", 0, 200);
    TF1* f_mc_cent = (TF1*)h_mc_cent_vertex_px->GetFunction("pol9");

    TH1D* h_mc_cent_vertex_py = (TH1D*)h_mc_cent_vertex->ProjectionY();
    h_mc_cent_vertex_py->Fit("gaus");
    TF1* f_mc_vertex = (TF1*)h_mc_cent_vertex_py->GetFunction("gaus");

    for (int32_t gbin=1; gbin!=h_weights->GetSize()-1; ++gbin) {
        int xbin, ybin, zbin;
        double xbin_centre, ybin_centre;

        h_weights->GetBinXYZ(gbin, xbin, ybin, zbin);
        xbin_centre = h_weights->GetXaxis()->GetBinCenter(xbin);
        ybin_centre = h_weights->GetYaxis()->GetBinCenter(ybin);

        double fit_value, fit_value_mc;
        fit_value = f_cent->Eval(xbin_centre) * f_vertex->Eval(ybin_centre);
        fit_value_mc = f_mc_cent->Eval(xbin_centre) * f_mc_vertex->Eval(ybin_centre);

        if (fit_value > 0 && fit_value_mc > 0)
            h_weights->SetBinContent(gbin, fit_value / fit_value_mc);
        else
            h_weights->SetBinContent(gbin, 0);
    }

    h_weights->Scale(1/h_weights->GetMaximum());
}

void ppWeights(TH1D* h_vertex, TH1D* h_mc_vertex, TH1D* h_weights) {
    h_vertex->Fit("gaus");
    TF1* f_vertex = (TF1*)h_vertex->GetFunction("gaus");

    h_mc_vertex->Fit("gaus");
    TF1* f_mc_vertex = (TF1*)h_mc_vertex->GetFunction("gaus");

    for (int32_t xbin=1; xbin!=h_weights->GetSize()-1; ++xbin) {
        double xbin_centre = h_weights->GetXaxis()->GetBinCenter(xbin);

        double fit_value, fit_value_mc;
        fit_value = f_vertex->Eval(xbin_centre);
        fit_value_mc = f_mc_vertex->Eval(xbin_centre);

        if (fit_value > 0 && fit_value_mc > 0)
            h_weights->SetBinContent(xbin, fit_value / fit_value_mc);
        else
            h_weights->SetBinContent(xbin, 0);
    }

    h_weights->Scale(1/h_weights->GetMaximum());
}

int calcWeights(const TString configFile, const char* inputData, const char* inputMC) {
    InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

    int collision = configInput.proc[INPUT::kHISTOGRAM].i[INPUT::k_collisionType];

    bool isHI = collisionIsHI((COLL::TYPE)collision);

    TFile* f_data = TFile::Open(inputData);
    TFile* f_mc = TFile::Open(inputMC);

    TTree* t_event = (TTree*)f_data->Get("HiEvt");
    TTree* t_event_mc = (TTree*)f_mc->Get("HiEvt");

    TH1::SetDefaultSumw2();

    TFile* weightsFile = TFile::Open(configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_weights_file].c_str(), "RECREATE");

    if (isHI) {
        TH2D* h_weights = new TH2D("h_weights", "", 200, 0, 200, 75, -15, 15);
        TH2D* h_cent_vertex = new TH2D("h_cent_vertex", "", 200, 0, 200, 75, -15, 15);
        TH2D* h_mc_cent_vertex = new TH2D("h_mc_cent_vertex", "", 200, 0, 200, 75, -15, 15);

        t_event->Draw("vz:hiBin>>h_cent_vertex");
        t_event_mc->Draw("vz:hiBin>>h_mc_cent_vertex");

        PbPbWeights(h_cent_vertex, h_mc_cent_vertex, h_weights);

        h_cent_vertex->Write("h_cent_vertex", TObject::kOverwrite);
        h_mc_cent_vertex->Write("h_mc_cent_vertex", TObject::kOverwrite);
        h_weights->Write("h_weights", TObject::kOverwrite);
    } else {
        TH1D* h_weights = new TH1D("h_weights", "", 75, -15, 15);
        TH1D* h_vertex = new TH1D("h_vertex", "", 75, -15, 15);
        TH1D* h_mc_vertex = new TH1D("h_mc_vertex", "", 75, -15, 15);

        t_event->Draw("vz>>h_vertex");
        t_event_mc->Draw("vz>>h_mc_vertex");

        ppWeights(h_vertex, h_mc_vertex, h_weights);

        h_vertex->Write("h_vertex", TObject::kOverwrite);
        h_mc_vertex->Write("h_mc_vertex", TObject::kOverwrite);
        h_weights->Write("h_weights", TObject::kOverwrite);
    }

    weightsFile->Close();

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 4)
        return calcWeights(argv[1], argv[2], argv[3]);

    return 1;
}
