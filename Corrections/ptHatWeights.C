#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TH1.h"
#include "TCanvas.h"

#include <vector>
#include <string>

#include "../Utilities/interface/InputConfigurationParser.h"

float calc_weight(TChain* tlowest, TChain* tmerged, float pthat_i, float pthat_f);

int ptHatWeights(std::string input, std::string lowest_pthat) {
    std::vector<std::string> input_list = InputConfigurationParser::ParseFiles(input);
    printf("number of input files: %zu\n", input_list.size());

    TChain* tmerged = new TChain("hiEvtAnalyzer/HiTree");
    for (std::vector<std::string>::iterator it = input_list.begin(); it != input_list.end(); ++it)
        tmerged->Add((*it).c_str());

    std::vector<std::string> lowest_pthat_list = InputConfigurationParser::ParseFiles(lowest_pthat);

    TChain* tlowest = new TChain("hiEvtAnalyzer/HiTree");
    for (std::vector<std::string>::iterator it = lowest_pthat_list.begin(); it != lowest_pthat_list.end(); ++it)
        tlowest->Add((*it).c_str());

    float pthat_division[6] = {15, 30, 50, 80, 120, 9999};
    float pthat_weights[5] = {0};
    for (int i=0; i<5; ++i) {
        pthat_weights[i] = calc_weight(tlowest, tmerged, pthat_division[i], pthat_division[i+1]);
        printf("pthat weight: %f < pthat < %f: %f\n", pthat_division[i], pthat_division[i+1], pthat_weights[i]);
    }

    return 0;
}

float calc_weight(TChain* tlowest, TChain* tmerged, float pthat_i, float pthat_f) {
    float pthat_lowest, pthat_merged;

    tlowest->SetBranchStatus("*", 0);
    tmerged->SetBranchStatus("*", 0);
    tlowest->SetBranchStatus("pthat", 1);
    tlowest->SetBranchAddress("pthat", &pthat_lowest);
    tmerged->SetBranchStatus("pthat", 1);
    tmerged->SetBranchAddress("pthat", &pthat_merged);

    int nlowest = tlowest->GetEntries(Form("pthat>=%f && pthat<%f", pthat_i, pthat_f));
    int nmerged = tmerged->GetEntries(Form("pthat>=%f && pthat<%f", pthat_i, pthat_f));
    float weight = ((float)nlowest)/((float)nmerged);

    return weight;
}

int main(int argc, char* argv[]) {
    if (argc == 3)
        return ptHatWeights(argv[1], argv[2]);
    else
        return 1;
}
