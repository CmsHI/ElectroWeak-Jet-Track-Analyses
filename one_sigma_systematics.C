#include "TFile.h"
#include "TH1F.h"
#include "TH2.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"

#include <fstream>
#include <vector>
#include <string>

#include "systematics.h"

int nbins = 8;
double binning[] = {0, 0.25, 0.5, 0.75, 1, 1.25, 1.5, 1.75, 2};

int one_sigma_systematics(const char* nominal_file, const char* filelist, const char* histlist, const char* label) {
    TH1::AddDirectory(kFALSE);
    TH1::SetDefaultSumw2(kTRUE);

    std::string line;

    std::vector<std::string> hist_list;
    std::ifstream hist_stream(histlist);
    if (!hist_stream) { printf("bad histogram list\n"); return 1; }
    while (std::getline(hist_stream, line))
        hist_list.push_back(line);

    std::size_t nhists = hist_list.size();
    if (!nhists) {printf("0 total hists!\n"); return 1;}

    std::vector<std::string> file_list;
    std::ifstream file_stream(filelist);
    if (!file_stream) { printf("bad file list\n"); return 1; }
    while (std::getline(file_stream, line))
        file_list.push_back(line);

    std::size_t nfiles = file_list.size();
    if (!nfiles) {printf("0 total files!\n"); return 1;}

    TFile* fnominal = new TFile(nominal_file, "read");
    TH1F* hnominals[nhists] = {0};
    TH1F* hfinal[nhists] = {0};
    for (std::size_t i=0; i<nhists; ++i) {
        hnominals[i] = (TH1F*)fnominal->Get(Form("%s", hist_list[i].c_str()));
        hfinal[i] = (TH1F*)hnominals[i]->Clone(hist_list[i].c_str());
    }

    TFile* fsys[nfiles] = {0};
    for (std::size_t i=0; i<nfiles; ++i)
        fsys[i] = new TFile(file_list[i].c_str(), "read");

    TFile* fout = new TFile(Form("gammaJetHistograms_%s-incremental.root", label), "recreate");

    sys_var_t* sys_vars[nhists][nfiles] = {0};
    TH2F* hvariations[nhists] = {0};
    for (std::size_t i=0; i<nhists; ++i) {
        int nbins = hnominals[i]->GetNbinsX();
        double binning[nbins + 1];
        for (int b=1; b<=nbins+1; ++b)
            binning[b-1] = hnominals[i]->GetBinLowEdge(b);

        hvariations[i] = new TH2F(Form("%s_variations", hist_list[i].c_str()), "", nbins, binning, 1000, -1, 1);
        for (std::size_t j=0; j<nfiles; ++j) {
            sys_vars[i][j] = new sys_var_t(hist_list[i].c_str(), Form("%zu", j), hnominals[i], (TH1F*)fsys[j]->Get(Form("%s", hist_list[i].c_str())));
            sys_vars[i][j]->calc_sys(nbins, binning);

            for (int k=1; k<=nbins; ++k)
                hvariations[i]->Fill(sys_vars[i][j]->get_diff()->GetBinCenter(k), sys_vars[i][j]->get_diff()->GetBinContent(k));
        }

        TH1D* hprojections[nbins] = {0};
        for (int l=1; l<=nbins; ++l) {
            hprojections[l-1] = hvariations[i]->ProjectionY(Form("_py_bin_%i", l), l, l);
            float mean = hprojections[l-1]->GetMean(1);
            float stddev = hprojections[l-1]->GetStdDev(1);
            hfinal[i]->SetBinContent(l, std::max(std::abs(mean + stddev), std::abs(mean - stddev)));
            hfinal[i]->SetBinError(l, 0);

            hprojections[l-1]->Write("", TObject::kOverwrite);
        }

        hvariations[i]->Write("", TObject::kOverwrite);
        hfinal[i]->Write("", TObject::kOverwrite);
    }

    for (std::size_t i=0; i<nfiles; ++i)
        fsys[i]->Close();

    fout->Write("", TObject::kOverwrite);
    fout->Close();

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 5)
        return one_sigma_systematics(argv[1], argv[2], argv[3], argv[4]);
    else
        return 1;
}
