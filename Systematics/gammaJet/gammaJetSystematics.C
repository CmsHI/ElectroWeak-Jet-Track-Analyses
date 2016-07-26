#include <TFile.h>
#include <TDirectoryFile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TH1D.h>
#include <TF1.h>
#include <TLine.h>
#include <TLatex.h>

#include <vector>
#include <string>
#include <iostream>

#include "../interface/systematicsHist.h"
#include "../../TreeHeaders/CutConfigurationTree.h"
#include "../../Utilities/interface/CutConfigurationParser.h"
#include "../../Utilities/interface/InputConfigurationParser.h"
#include "../../Utilities/styleUtil.h"
#include "../../Utilities/th1Util.h"
#include "../../Utilities/tf1Util.h"
#include "../../Utilities/bosonJetUtil.h"

int gammaJetSystematics(const TString configFile, const TString inputFile, const TString outputFile, const TString outputFigurePrefix) {
    std::cout << "running gammaJetSystematics()" << std::endl;
    std::cout << "configFile = " << configFile.Data() << std::endl;
    std::cout << "inputFile = " << inputFile.Data() << std::endl;
    std::cout << "outputFile = " << outputFile.Data() << std::endl;
    std::cout << "outputFigurePrefix = " << outputFigurePrefix.Data() << std::endl;

    InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

    if (!configInput.isValid) {
        std::cout << "Invalid input configuration" << std::endl;
        return 1;
    }

    if (!configCuts.isValid) {
        std::cout << "Invalid cut configuration" << std::endl;
        return 1;
    }

    int collision = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_collisionType];

    // input for TH1
    float markerSize = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_markerSize];

    // input for TLegend
    float legendOffsetX  = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_legendOffsetX];
    float legendOffsetY  = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_legendOffsetY];
    int legendBorderSize = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_legendBorderSize];
    float legendWidth    = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_legendWidth];
    float legendHeight   = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_legendHeight];
    float legendTextSize = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_legendTextSize];

    // input for text objects
    int textFont = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_textFont];
    float textSize = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_textSize];
    float textOffsetX = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_textOffsetX];
    float textOffsetY = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_textOffsetY];

    std::vector<std::string> textsOverPad = ConfigurationParser::ParseListLatex(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_textAbovePad]);
    std::vector<std::string> textsOverPadAlignments = ConfigurationParser::ParseList(configInput.proc[INPUT::kPLOTTING].s[INPUT::k_textAbovePadAlign]);
    int textAbovePadFont = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_textAbovePadFont];
    float textAbovePadSize = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_textAbovePadSize];
    float textAbovePadOffsetX = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_textAbovePadOffsetX];
    float textAbovePadOffsetY = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_textAbovePadOffsetY];

    // input for TCanvas
    int windowWidth  = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_windowWidth];
    int windowHeight = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_windowHeight];
    float leftMargin   = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_leftMargin];
    float rightMargin  = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_rightMargin];
    float bottomMargin = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_bottomMargin];
    float topMargin    = configInput.proc[INPUT::kPLOTTING].f[INPUT::k_topMargin];
    int setLogx = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_setLogx];
    int setLogy = configInput.proc[INPUT::kPLOTTING].i[INPUT::k_setLogy];

    // set default values
    if (markerSize == 0) markerSize = INPUT_DEFAULT::markerSize;

    if (textFont == 0) textFont = INPUT_DEFAULT::textFont;
    if (textSize == 0) textSize = INPUT_DEFAULT::textSize;

    if (textAbovePadFont == 0) textAbovePadFont = INPUT_DEFAULT::textAbovePadFont;
    if (textAbovePadSize == 0) textAbovePadSize = INPUT_DEFAULT::textAbovePadSize;

    if (windowWidth == 0) windowWidth = INPUT_DEFAULT::windowWidth;
    if (windowHeight == 0) windowHeight = INPUT_DEFAULT::windowHeight;
    if (leftMargin == 0) leftMargin = INPUT_DEFAULT::leftMargin;
    if (rightMargin == 0) rightMargin = INPUT_DEFAULT::rightMargin;
    if (bottomMargin == 0) bottomMargin = INPUT_DEFAULT::bottomMargin;
    if (topMargin == 0) topMargin = INPUT_DEFAULT::topMargin;

    int nTextsOverPad = textsOverPad.size();
    int nTextsOverPadAlignments = textsOverPadAlignments.size();

    std::string collisionName = getCollisionTypeName((COLL::TYPE)collision).c_str();

    // verbose about input configuration
    std::cout << "Input Configuration :" << std::endl;
    std::cout << "collision = " << collisionName.c_str() << std::endl;
    std::cout << "markerSize = " << markerSize << std::endl;

    std::cout << "legendOffsetX    = " << legendOffsetX << std::endl;
    std::cout << "legendOffsetY    = " << legendOffsetY << std::endl;
    std::cout << "legendBorderSize = " << legendBorderSize << std::endl;
    std::cout << "legendWidth      = " << legendWidth << std::endl;
    std::cout << "legendHeight     = " << legendHeight << std::endl;
    std::cout << "legendTextSize   = " << legendTextSize << std::endl;

    std::cout << "textFont = " << textFont << std::endl;
    std::cout << "textSize = " << textSize << std::endl;
    std::cout << "textOffsetX  = " << textOffsetX << std::endl;
    std::cout << "textOffsetY  = " << textOffsetY << std::endl;

    std::cout << "nTextsOverPad = " << nTextsOverPad << std::endl;
    for (int i = 0; i<nTextsOverPad; ++i) {
        std::cout << Form("textsOverPad[%d] = %s", i, textsOverPad.at(i).c_str()) << std::endl;
    }
    if (nTextsOverPad > 0) {
        std::cout << "nTextsOverPadAlignments = " << nTextsOverPadAlignments << std::endl;
        for (int i = 0; i<nTextsOverPadAlignments; ++i) {
            std::cout << Form("textsOverPadAlignments[%d] = %s", i, textsOverPadAlignments.at(i).c_str()) << std::endl;
        }
        std::cout << "textAbovePadFont = " << textAbovePadFont << std::endl;
        std::cout << "textAbovePadSize = " << textAbovePadSize << std::endl;
        std::cout << "textAbovePadOffsetX = " << textAbovePadOffsetX << std::endl;
        std::cout << "textAbovePadOffsetY = " << textAbovePadOffsetY << std::endl;
    }

    std::cout << "windowWidth  = " << windowWidth << std::endl;
    std::cout << "windowHeight = " << windowHeight << std::endl;
    std::cout << "leftMargin   = " << leftMargin << std::endl;
    std::cout << "rightMargin  = " << rightMargin << std::endl;
    std::cout << "bottomMargin = " << bottomMargin << std::endl;
    std::cout << "topMargin    = " << topMargin << std::endl;
    std::cout << "setLogx  = " << setLogx << std::endl;
    std::cout << "setLogy  = " << setLogy << std::endl;

    // bool isMC = collisionIsMC((COLL::TYPE)collision);
    bool isHI = collisionIsHI((COLL::TYPE)collision);
    bool isPP = collisionIsPP((COLL::TYPE)collision);
    bool isMC = collisionIsMC((COLL::TYPE)collision);

    // cuts in this macro are only used for adding text to the plots
    std::vector<float> bins_pt[2];
    std::vector<int>   bins_hiBin[2];       // array of vectors for hiBin bins, each array element is a vector.
    // photon cuts
    // jet cuts
    std::string jetCollection;
    float jetpt;
    float jeteta;
    // gammaJet cuts
    float awayRange;    // awayRange = 78 means dphi > 7/8 PI

    bins_pt[0] = ConfigurationParser::ParseListFloat(configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_pt_gt]);
    bins_pt[1] = ConfigurationParser::ParseListFloat(configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_pt_lt]);
    bins_hiBin[0] = ConfigurationParser::ParseListInteger(configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_gt]);
    bins_hiBin[1] = ConfigurationParser::ParseListInteger(configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_lt]);

    jetCollection = configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kJET].s[CUTS::JET::k_jetCollection];
    jetpt  = configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kJET].f[CUTS::JET::k_pt];
    jeteta = configCuts.proc[CUTS::kPLOTTING].obj[CUTS::kJET].f[CUTS::JET::k_eta];

    awayRange = configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kGAMMAJET].f[CUTS::GJT::k_awayRange];

    int nBins_pt = bins_pt[0].size();
    int nBins_hiBin = bins_hiBin[0].size();     // assume <myvector>[0] and <myvector>[1] have the same size.
    // verbose about cut configuration
    std::cout << "Cut Configuration :" << std::endl;
    std::cout << "nBins_Pt = " << nBins_pt << std::endl;
    for (int i = 0; i<nBins_pt; ++i) {
        std::cout << Form("bins_phoPt[%d] = [%f, %f)", i, bins_pt[0].at(i), bins_pt[1].at(i)) << std::endl;
    }
    std::cout << "nBins_hiBin = " << nBins_hiBin << std::endl;
    for (int i=0; i<nBins_hiBin; ++i) {
        std::cout << Form("bins_hiBin[%d] = [%d, %d)", i, bins_hiBin[0].at(i), bins_hiBin[1].at(i)) << std::endl;
    }
    std::cout << "jetCollection = " << jetCollection.c_str() << std::endl;
    std::cout << "jetpt  = " << jetpt << std::endl;
    std::cout << "jeteta = " << jeteta << std::endl;
    std::cout << "awayRange = " << awayRange << std::endl;

    // inputFile should be a list of ROOT files, that is a ".txt" or ".list" file.
    // the assumed order in inputFile :
    // 1     : nominal results
    // i > 1 : results varied by systematics
    std::vector<std::string> inputFiles = InputConfigurationParser::ParseFiles(inputFile.Data());

    int nInputFiles = inputFiles.size();
    std::cout << "input ROOT files : num = " << nInputFiles << std::endl;
    if (nInputFiles < 2) {
        std::cout << "need to provide at least 2 input files" << std::endl;
        std::cout << "exiting" << std::endl;
        return 1;
    } else {
        std::cout << "#####" << std::endl;
        std::cout << "nominal result : " << inputFiles.at(0).c_str() << std::endl;
        for (int i = 1; i < nInputFiles; ++i) {
            std::cout << "sys. variation : " << inputFiles.at(i).c_str() << std::endl;
        }
        std::cout << "##### END #####" << std::endl;
    }

    TFile* input[nInputFiles];
    bool inputExists[nInputFiles];
    for (int i = 0; i<nInputFiles; ++i) {
        input[i] = new TFile(inputFiles.at(i).c_str(), "READ");
        inputExists[i] = (input[i] && input[i]->IsOpen());
        if (!inputExists[i])
            continue;
    }

    TFile* output = new TFile(outputFile, "RECREATE");
    std::string dirName = "SYS";
    if (!output->GetKey(dirName.c_str()))
        output->mkdir(dirName.c_str(), Form("file with nominal results is %s", inputFiles.at(0).c_str()));
    output->cd(dirName.c_str());
    // output->Delete(Form("%s/*;1", dirName.c_str()));      // delete all systematics histograms from the first cycle.
    // https://root.cern.ch/doc/master/classTDirectory.html#af206b823d1d9a7ecadd1fbcf41b01c25
    std::cout << "histograms will be put under directory : " << dirName.c_str() << std::endl;

    TTree* configTree = setupConfigurationTreeForWriting(configCuts);

    std::string type_suffix;
    if (isHI)         type_suffix = "PbPb_Data";
    if (isHI && isMC) type_suffix = "PbPb_MC";
    if (isPP)         type_suffix = "pp_Data";
    if (isPP && isMC) type_suffix = "pp_MC";

    std::vector<std::string> correlationHistNames {
        "xjg", "dphi", "ptJet", "rjg_ptBinAll", "rjg_centBinAll",
        "xjg_mean_ptBinAll", "xjg_mean_centBinAll", "dphi_width_ptBinAll", "dphi_width_centBinAll", "iaa"
    };

    std::vector<std::string> hist_suffix {
        type_suffix, type_suffix, type_suffix, type_suffix, type_suffix,
        type_suffix, type_suffix, type_suffix, type_suffix, "rebin"
    };

    std::vector<std::string> legendPositions {"NE", "NW", "NE", "NW", "NW", "NE", "NE", "NE", "NE", "NE"};
    std::vector<std::string> textPositions {"NE", "NW", "NE", "SE", "SE", "NE", "NE", "NE", "NE", "NE"};
    std::vector<bool> textWriteDphi {true, false, true, true, true, true, true, true, true, true};

    std::vector<std::string> functionFormulas {
        "([0]+[1]*x+[2]*x*x)*exp(-[3]*(x-[4]))", "([0]+[1]*x)*exp(-[2]*(TMath::Pi()-x))+[3]", "pol4", "pol4", "pol4",
        "pol4", "pol4", "pol4", "pol4", "pol4"
    };
    std::vector<std::string> fitOptions {
        "chi2 R M", "chi2 R M", "chi2 R M", "chi2 R M", "chi2 R M",
        "chi2 R M", "chi2 R M", "chi2 R M", "chi2 R M", "chi2 R M"
    };
    std::vector<std::string> functionFormulas_uncTot {
        "pol0", "pol0", "pol2", "pol2", "pol2",
        "pol2", "pol2", "pol2", "pol2", "pol2"
    };
    std::vector<int> rebins {
        1, 4, 3, 1, 1,
        1, 1, 1, 1, 1
    };

    int nCorrHist = correlationHistNames.size();

    std::vector<bool> do_SYS_UNC(nCorrHist, true);

    // declaration of graphics objects.
    // the idea is to initialize each Graphics object right before its use, then to delete it right after they are saved/written.
    TCanvas* c1;

    TH1::SetDefaultSumw2();
    systematicsHist sysHist[nInputFiles][nCorrHist];

    TF1* fnc_uncTot[nCorrHist];          // polynomial fit to final total uncertainty

    for (int i=0; i<nCorrHist; ++i) {
        if (!do_SYS_UNC.at(i)) continue;

        for (int iPt=0; iPt<nBins_pt; ++iPt) {
            int phoPt1 = (int)bins_pt[0].at(iPt);
            int phoPt2 = (int)bins_pt[1].at(iPt);

            for (int iHiBin=0; iHiBin<nBins_hiBin; ++iHiBin) {
                std::string correlation = correlationHistNames.at(i).c_str();
                std::cout << "##### " << correlation.c_str() << " #####" << std::endl;

                std::string tmpName = Form("%s_ptBin%d_hiBin%d_%s", correlationHistNames.at(i).c_str(), iPt, iHiBin, hist_suffix.at(i).c_str());
                // special cases
                if (correlation.find("ptBinAll") != std::string::npos) {
                    if (iPt > 0) break;
                    tmpName = Form("%s_hiBin%d_%s", correlationHistNames.at(i).c_str(), iHiBin, hist_suffix.at(i).c_str());
                } else if (correlation.find("centBinAll") != std::string::npos) {
                    if (iHiBin > 0) break;
                    tmpName = Form("%s_ptBin%d_%s", correlationHistNames.at(i).c_str(), iPt, hist_suffix.at(i).c_str());
                }

                std::string tmpHistName = Form("h1D_%s", tmpName.c_str());

                // read histograms
                std::cout << "reading histogram : " << tmpHistName.c_str() << std::endl;
                for (int iFile = 0; iFile < nInputFiles; ++iFile) {
                    if (inputExists[iFile]) {
                        sysHist[iFile][i].name = tmpName.c_str();
                        sysHist[iFile][i].observable = correlation.c_str();

                        input[iFile]->GetObject(tmpHistName.c_str(), sysHist[iFile][i].h1D_varied);
                    }

                    sysHist[iFile][i].isValid = false;
                    if (sysHist[iFile][i].h1D_varied) {
                        sysHist[iFile][i].isValid = true;

                        // variable size binning for SYS UNC calculation
                        std::vector<double> tmpBins = getTH1xBins(sysHist[iFile][i].h1D_varied);

                        if (correlation.compare("xjg") == 0) {
                            if (isPP) tmpBins = rebinXjBinsPP(tmpBins);
                            else tmpBins = rebinXjBins(tmpBins);
                        } else if (rebins.at(i) > 1) {
                            TH1D* htmp = (TH1D*)sysHist[iFile][i].h1D_varied->Rebin(rebins.at(i), Form("%s_%d_tmpRebin", tmpHistName.c_str(), iFile));
                            tmpBins = getTH1xBins(htmp);
                        }
                        int NtmpBins = tmpBins.size();
                        double binsRebin[NtmpBins];
                        std::copy(tmpBins.begin(), tmpBins.end(), binsRebin);

                        sysHist[iFile][i].h1D_varied = (TH1D*)sysHist[iFile][i].h1D_varied->Rebin(NtmpBins-1, Form("%s_%d", tmpHistName.c_str(), iFile), binsRebin);    //creates a new variable bin size histogram hnew
                        sysHist[iFile][i].h1D_varied->SetTitle("");
                        sysHist[iFile][i].h1D_varied->SetStats(false);
                    }
                }
                sysHist[0][i].h1D_nominal = (TH1D*)sysHist[0][i].h1D_varied->Clone(Form("%s_nominal", tmpHistName.c_str()));
                sysHist[0][i].h1D_nominal->SetStats(false);

                std::string tmpName_uncTot = Form("%s_uncTot", tmpName.c_str());
                std::string tmpHistName_uncTot = Form("h1D_%s", tmpName_uncTot.c_str());

                // sysHist[0][i] will contain the total SYS. UNC.
                // sysHist[0][i].h1D_diff  = sqrt( sum over j : (sysHist[j][i].h1D_diff)^2 )
                // sysHist[0][i].h1D_ratio = sqrt( sum over j : (sysHist[j][i].h1D_ratio)^2 )

                std::string tmpName_uncTot_diff = Form("%s_diff", tmpName_uncTot.c_str());
                std::string tmpHistName_uncTot_diff = Form("h1D_%s", tmpName_uncTot_diff.c_str());
                sysHist[0][i].h1D_diff = (TH1D*)sysHist[0][i].h1D_varied->Clone(tmpHistName_uncTot_diff.c_str());

                std::string tmpName_uncTot_ratio = Form("%s_ratio", tmpName_uncTot.c_str());
                std::string tmpHistName_uncTot_ratio = Form("h1D_%s", tmpName_uncTot_ratio.c_str());
                sysHist[0][i].h1D_ratio = (TH1D*)sysHist[0][i].h1D_varied->Clone(tmpHistName_uncTot_ratio.c_str());

                sysHist[0][i].h1D_diff->Reset("ICES");
                // set TH1 style
                std::string titleY_h1D_diff = "Difference = |Nominal - Varied|";
                std::string titleY_h1D_diff_rel = "Rel. Difference = |Nominal - Varied| / Nominal";
                sysHist[0][i].h1D_diff->SetYTitle(titleY_h1D_diff.c_str());
                setTH1Style4SysUnc(sysHist[0][i].h1D_diff);
                // set TH1 style - END
                std::string tmpName_uncTot_diff_rel = Form("%s_diff_rel", tmpName_uncTot.c_str());
                std::string tmpHistName_uncTot_diff_rel = Form("h1D_%s", tmpName_uncTot_diff_rel.c_str());
                sysHist[0][i].h1D_diff_rel = (TH1D*)sysHist[0][i].h1D_diff->Clone(tmpHistName_uncTot_diff_rel.c_str());
                sysHist[0][i].h1D_diff_rel->SetYTitle(titleY_h1D_diff_rel.c_str());
                sysHist[0][i].h1D_diff_rel->SetMinimum(0);
                sysHist[0][i].h1D_diff_rel->SetMaximum(2);

                sysHist[0][i].h1D_ratio->Reset("ICES");
                // set TH1 style
                std::string titleY_h1D_ratio = "Ratio = Varied / Nominal";
                std::string titleY_h1D_ratio_rel = "Rel. Ratio = (Varied - Nominal) / Nominal";
                sysHist[0][i].h1D_ratio->SetYTitle(titleY_h1D_ratio.c_str());
                setTH1Style4SysUnc(sysHist[0][i].h1D_ratio);
                // set TH1 style - END
                std::string tmpName_uncTot_ratio_rel = Form("%s_ratio_rel", tmpName_uncTot.c_str());
                std::string tmpHistName_uncTot_ratio_rel = Form("h1D_%s", tmpName_uncTot_ratio_rel.c_str());
                sysHist[0][i].h1D_ratio_rel = (TH1D*)sysHist[0][i].h1D_ratio->Clone(tmpHistName_uncTot_ratio_rel.c_str());
                sysHist[0][i].h1D_ratio_rel->SetYTitle(titleY_h1D_ratio_rel.c_str());
                sysHist[0][i].h1D_ratio_rel->SetMinimum(0);
                sysHist[0][i].h1D_ratio_rel->SetMaximum(2);

                for (int j = 0; j < SYS::kN_SYSPOLFNC; ++j) {
                    // histograms made by bin-by-bin evaluation of fnc_pol
                    std::string tmpName_fnc_pol = Form("fnc_%s_diff_pol_%d", tmpName_uncTot.c_str(), j);
                    std::string tmpHistName_fnc_pol = Form("h1D_%s", tmpName_fnc_pol.c_str());
                    sysHist[0][i].h1D_fnc_pol[j] = (TH1D*)sysHist[0][i].h1D_diff->Clone(tmpHistName_fnc_pol.c_str());
                    sysHist[0][i].h1D_fnc_pol[j]->SetMarkerColor(SYS::POLFNC_COLORS[j]);

                    std::string tmpName_fnc_pol_v1 = Form("fnc_%s_diff_pol_v1_%d", tmpName_uncTot.c_str(), j);
                    std::string tmpHistName_fnc_pol_v1 = Form("h1D_%s", tmpName_fnc_pol_v1.c_str());
                    sysHist[0][i].h1D_fnc_pol_v1[j] = (TH1D*)sysHist[0][i].h1D_fnc_pol[j]->Clone(tmpHistName_fnc_pol_v1.c_str());

                    std::string tmpName_fnc_pol_rel = Form("fnc_%s_diff_pol_%d_rel", tmpName_uncTot.c_str(), j);
                    std::string tmpHistName_fnc_pol_rel = Form("h1D_%s", tmpName_fnc_pol_rel.c_str());
                    sysHist[0][i].h1D_fnc_pol_rel[j] = (TH1D*)sysHist[0][i].h1D_diff->Clone(tmpHistName_fnc_pol_rel.c_str());
                    sysHist[0][i].h1D_fnc_pol_rel[j]->SetMarkerColor(SYS::POLFNC_COLORS[j]);
                    sysHist[0][i].h1D_fnc_pol_rel[j]->SetMarkerStyle(SYS::POLFNC_MARKERSTYLES[j]);

                    std::string tmpName_fnc_pol_rel_v1 = Form("fnc_%s_diff_pol_v1_%d_rel", tmpName_uncTot.c_str(), j);
                    std::string tmpHistName_fnc_pol_rel_v1 = Form("h1D_%s", tmpName_fnc_pol_rel_v1.c_str());
                    sysHist[0][i].h1D_fnc_pol_rel_v1[j] = (TH1D*)sysHist[0][i].h1D_fnc_pol_rel[j]->Clone(tmpHistName_fnc_pol_rel_v1.c_str());
                }
                // calculate the ratio of the results.
                // nominal histogram is always the denominator.

                // the total uncertainty function = sqrt (f1*f1 + f2*f2 + ...)
                // where f1 is the fit to the first SYS. UNC. source.
                // that function will be stored in the "sysHist[0][i]"
                for (int j = 0; j < SYS::kN_SYSPOLFNC; ++j) {
                    double xmin = sysHist[0][i].h1D_diff->GetXaxis()->GetBinLowEdge(sysHist[0][i].h1D_diff->GetXaxis()->GetFirst());
                    double xmax = sysHist[0][i].h1D_diff->GetXaxis()->GetBinLowEdge(sysHist[0][i].h1D_diff->GetXaxis()->GetLast()+1);
                    std::string fnc_pol_name = Form("fnc_%s_pol_%d", tmpName_uncTot.c_str(), j);

                    sysHist[0][i].fnc_pol_formula[j] = "pol0";      // dummy formula to avoid "Error in <TFormula::TFormula>: expression may not be 0 or have 0 length"
                    sysHist[0][i].fnc_pol[j] = new TF1(fnc_pol_name.c_str(), sysHist[0][i].fnc_pol_formula[j].c_str(), xmin, xmax);
                    sysHist[0][i].fnc_pol[j]->SetLineColor(SYS::POLFNC_COLORS[j]);

                    // this function is never fit. It is the sqrt of the sum of squares of functions which are fit.
                    sysHist[0][i].isFit_fnc_pol[j] = false;
                }

                // LOOP over SYSTEMATICS
                TH1D* h1D_temp = 0;
                for (int iFile = 1; iFile < nInputFiles; ++iFile) {
                    if (!sysHist[0][i].isValid) continue;
                    if (!sysHist[iFile][i].isValid) continue;

                    // binning of h1D_diff[0][i] was already set before this point.
                    std::vector<double> tmpBins = getTH1xBins(sysHist[0][i].h1D_diff);

                    int NtmpBins = tmpBins.size();
                    double binsRebin[NtmpBins];
                    std::copy(tmpBins.begin(), tmpBins.end(), binsRebin);

                    h1D_temp = (TH1D*)sysHist[iFile][i].h1D_varied->Clone(Form("%s_%d_temp", tmpHistName.c_str(), iFile));

                    std::string tmpName_nominal = Form("%s_nominal_%d", tmpName.c_str(), iFile);
                    std::string tmpHistName_nominal = Form("h1D_%s", tmpName_nominal.c_str());
                    sysHist[iFile][i].h1D_nominal = (TH1D*)sysHist[0][i].h1D_nominal->Clone(tmpHistName_nominal.c_str());
                    std::string tmpName_diff = Form("%s_diff_%d", tmpName.c_str(), iFile);
                    std::string tmpHistName_diff = Form("h1D_%s", tmpName_diff.c_str());
                    sysHist[iFile][i].h1D_diff = (TH1D*)sysHist[iFile][i].h1D_nominal->Clone(tmpHistName_diff.c_str());
                    sysHist[iFile][i].h1D_diff->Add(h1D_temp, -1);
                    // use the error bars of nominal histogram
                    for (int iBin = 0; iBin < sysHist[iFile][i].h1D_nominal->GetNbinsX(); ++iBin) {
                        sysHist[iFile][i].h1D_diff->SetBinContent(iBin, sysHist[iFile][i].h1D_nominal->GetBinError(iBin));
                    }

                    // set TH1 style
                    sysHist[iFile][i].h1D_diff->SetYTitle(titleY_h1D_diff.c_str());
                    setTH1Style4SysUnc(sysHist[iFile][i].h1D_diff);
                    // set TH1 style - END

                    // prepare names for relative difference
                    std::string tmpName_diff_rel = Form("%s_diff_rel_%d", tmpName.c_str(), iFile);
                    std::string tmpHistName_diff_rel = Form("h1D_%s", tmpName_diff_rel.c_str());

                    // initial version of h1D_diff  before scaling, abs(), rebinning etc. operations
                    std::string tmpHistName_diff_v1 = Form("%s_v1", sysHist[iFile][i].h1D_diff->GetName());
                    sysHist[iFile][i].h1D_diff_v1 = (TH1D*)sysHist[iFile][i].h1D_diff->Clone(tmpHistName_diff_v1.c_str());

                    std::string tmpName_ratio = Form("%s_ratio_%d", tmpName.c_str(), iFile);
                    std::string tmpHistName_ratio = Form("h1D_%s", tmpName_ratio.c_str());
                    sysHist[iFile][i].h1D_ratio = (TH1D*)h1D_temp->Clone(tmpHistName_ratio.c_str());
                    // calculate the ratios of the histograms.
                    calcTH1Ratio4SysUnc(sysHist[iFile][i].h1D_ratio, sysHist[0][i].h1D_ratio);
                    // set TH1 style
                    sysHist[iFile][i].h1D_ratio->SetYTitle(titleY_h1D_ratio.c_str());
                    setTH1Style4SysUnc(sysHist[iFile][i].h1D_ratio);
                    // set TH1 style - END

                    // prepare names for relative ratio
                    std::string tmpName_ratio_rel = Form("%s_ratio_rel_%d", tmpName.c_str(), iFile);
                    std::string tmpHistName_ratio_rel = Form("h1D_%s", tmpName_ratio_rel.c_str());

                    std::string tmpHistName_ratio_v1 = Form("%s_v1", sysHist[iFile][i].h1D_ratio->GetName());
                    sysHist[iFile][i].h1D_ratio_v1 = (TH1D*)sysHist[iFile][i].h1D_ratio->Clone(tmpHistName_ratio_v1.c_str());

                    // SYS UNC that is calculated from differences
                    // this will be the SYS UNC. calculation of the analysis
                    if (rebins.at(i) != 1) {    // do scaling only if the histogram is rebinned.
                        int nBins = sysHist[iFile][i].h1D_diff->GetNbinsX();
                        for (int iBin = 1; iBin <= nBins; ++iBin) {
                            double binContent = sysHist[iFile][i].h1D_diff->GetBinContent(iBin);
                            sysHist[iFile][i].h1D_diff->SetBinContent(iBin, binContent/rebins.at(i));
                        }
                    } else if (correlation.compare("xjg") == 0) {
                        sysHist[iFile][i].h1D_diff->Scale(1, "width");
                        sysHist[iFile][i].h1D_diff->Scale(sysHist[iFile][i].h1D_varied->GetBinWidth(1));
                    }

                    // SYS UNC that is calculated from ratios
                    // this will NOT be the SYS UNC. calculation of the analysis
                    if (rebins.at(i) != 1) {    // do scaling only if the histogram is rebinned.
                        int nBins = sysHist[iFile][i].h1D_ratio->GetNbinsX();
                        for (int iBin = 1; iBin <= nBins; ++iBin) {
                            double binContent = sysHist[iFile][i].h1D_ratio->GetBinContent(iBin);
                            sysHist[iFile][i].h1D_ratio->SetBinContent(iBin, binContent/rebins.at(i));
                        }
                    } else if (correlation.compare("xjg") == 0) {
                        sysHist[iFile][i].h1D_ratio->Scale(1, "width");
                        sysHist[iFile][i].h1D_ratio->Scale(sysHist[iFile][i].h1D_varied->GetBinWidth(1));   // assumes the original histogram has uniform binning.
                    }

                    // iFile = 6   : electron rejection
                    // hard coded scale for electron rejection
                    if (iFile == 6) {
                        sysHist[iFile][i].h1D_diff->Scale(0.36);
                        sysHist[iFile][i].h1D_ratio->Scale(0.36);
                    }

                    // take the maximum of uncertainties : SYS = max(down, up), iFile = 1,2
                    // iFile = 1,2 : jet energy scale down and up variation
                    // iFile = 3,4 : photon purity down and up variation
                    // iFile = 5   : jet energy resolution
                    // iFile = 6   : electron rejection
                    // iFile = 7   : noEngCorr
                    // iFile = 8   : photon isolation
                    if (iFile == 2 || iFile == 4) {
                        calcTH1AbsMax4SysUnc(sysHist[iFile][i].h1D_diff, sysHist[iFile-1][i].h1D_diff, sysHist[iFile][i].h1D_diff);

                        // from this moment on, iFile covers the SYS. UNC. coming from iFile-1
                        // do not use iFile-1 any more.
                        // Ex. iFile = 2, 4 cover SYS. UNC. coming from iFile = 1, 3
                    }

                    // h1D_unc_diff_Max must have been calculated in the block above.
                    sysHist[iFile][i].isSet = (iFile != 1 && iFile != 3);
                    if (!sysHist[iFile][i].isSet) continue;

                    calcTH1Abs4SysUnc(sysHist[iFile][i].h1D_diff);  // h = abs(h)
                    addSysUnc(sysHist[0][i].h1D_diff, sysHist[iFile][i].h1D_diff);

                    // calculate relative unc. histograms
                    sysHist[iFile][i].h1D_diff_rel = (TH1D*)sysHist[iFile][i].h1D_diff->Clone(tmpHistName_diff_rel.c_str());
                    sysHist[iFile][i].h1D_diff_rel->Divide(sysHist[iFile][i].h1D_nominal);
                    calcTH1Abs4SysUnc(sysHist[iFile][i].h1D_diff_rel);  // h = abs(h)
                    addSysUnc(sysHist[0][i].h1D_diff_rel, sysHist[iFile][i].h1D_diff_rel);
                    sysHist[iFile][i].h1D_diff_rel->SetYTitle(titleY_h1D_diff_rel.c_str());
                    sysHist[iFile][i].h1D_diff_rel->SetMinimum(0);
                    sysHist[iFile][i].h1D_diff_rel->SetMaximum(2);

                    sysHist[iFile][i].h1D_ratio_rel = (TH1D*)sysHist[iFile][i].h1D_ratio->Clone(tmpHistName_ratio_rel.c_str());
                    subtractIdentity4SysUnc(sysHist[iFile][i].h1D_ratio_rel);
                    calcTH1Abs4SysUnc(sysHist[iFile][i].h1D_ratio_rel);  // h = abs(h)
                    addSysUnc(sysHist[0][i].h1D_ratio_rel, sysHist[iFile][i].h1D_ratio_rel);
                    sysHist[iFile][i].h1D_ratio_rel->SetYTitle(titleY_h1D_ratio_rel.c_str());
                    sysHist[iFile][i].h1D_ratio_rel->SetMinimum(0);
                    sysHist[iFile][i].h1D_ratio_rel->SetMaximum(2);

                    // make a fit to h1D_diff
                    for (int j = 0; j < SYS::kN_SYSPOLFNC; ++j) {
                        double xmin = sysHist[iFile][i].h1D_diff->GetXaxis()->GetBinLowEdge(sysHist[iFile][i].h1D_diff->GetXaxis()->GetFirst());
                        double xmax = sysHist[iFile][i].h1D_diff->GetXaxis()->GetBinLowEdge(sysHist[iFile][i].h1D_diff->GetXaxis()->GetLast()+1);
                        std::string tmpName_fnc_pol = Form("fnc_%s_pol_%d", tmpName_diff.c_str(), j);

                        std::string tmpFormula = SYS::POLFNC_FORMULAS[j].c_str();
                        std::string tmpFitOption = fitOptions.at(0).c_str();
                        // special cases
                        if (j == 3) {   // user defined function
                            tmpFormula = functionFormulas.at(i).c_str();
                            // tmpFitOption = "LL R M";
                        }
                        sysHist[iFile][i].fnc_pol[j] = new TF1(tmpName_fnc_pol.c_str(), tmpFormula.c_str(), xmin, xmax);
                        sysHist[iFile][i].fnc_pol[j]->SetLineColor(SYS::POLFNC_COLORS[j]);

                        sysHist[iFile][i].h1D_diff->Fit(tmpName_fnc_pol.c_str(), tmpFitOption.c_str());
                        sysHist[iFile][i].isFit_fnc_pol[j] = true;

                        std::string tmpFormulaFitted = systematicsHist::get_fnc_pol_Formula(sysHist[iFile][i].fnc_pol[j], j);
                        // special cases
                        if (j == 3) {   // get fitted user defined function
                            tmpFormulaFitted = getTF1FormulaFitParams(sysHist[iFile][i].fnc_pol[j]);
                        }

                        // add this fit function to the total uncertainty function
                        std::string uncTot_diff_Formula = sysHist[0][i].fnc_pol_formula[j].c_str();
                        if (uncTot_diff_Formula.size() == 0) {
                            uncTot_diff_Formula = Form("%s*%s", tmpFormulaFitted.c_str(), tmpFormulaFitted.c_str());
                        } else {
                            uncTot_diff_Formula = Form("%s + %s*%s", uncTot_diff_Formula.c_str(), tmpFormulaFitted.c_str(), tmpFormulaFitted.c_str());
                        }
                        sysHist[0][i].fnc_pol_formula[j] = uncTot_diff_Formula.c_str();
                        sysHist[0][i].fnc_pol[j] = new TF1(sysHist[0][i].fnc_pol[j]->GetName(),
                                                           sysHist[0][i].fnc_pol_formula[j].c_str(),
                                                           sysHist[0][i].fnc_pol[j]->GetXmin(), sysHist[0][i].fnc_pol[j]->GetXmax());
                        sysHist[0][i].fnc_pol[j]->SetLineColor(SYS::POLFNC_COLORS[j]);
                        // debug with cout
                        std::cout << "uncTot_diff_Formula = " << sysHist[0][i].fnc_pol_formula[j].c_str() << std::endl;
                        std::cout << "sysHist[0][i].fnc_pol[j] name = " << sysHist[0][i].fnc_pol[j]->GetName() << std::endl;
                        std::cout << "xmin = " << sysHist[0][i].fnc_pol[j]->GetXmin() << std::endl;
                        std::cout << "xmax = " << sysHist[0][i].fnc_pol[j]->GetXmax() << std::endl;

                        // histograms made by bin-by-bin evaluation of fnc_pol
                        std::string tmpHistName_fnc_pol = Form("h1D_%s", tmpName_fnc_pol.c_str());
                        sysHist[iFile][i].h1D_fnc_pol[j] = (TH1D*)sysHist[iFile][i].h1D_diff->Clone(tmpHistName_fnc_pol.c_str());
                        fillTH1fromTF1(sysHist[iFile][i].h1D_fnc_pol[j], sysHist[iFile][i].fnc_pol[j]);
                        sysHist[iFile][i].h1D_fnc_pol[j]->SetMarkerColor(SYS::POLFNC_COLORS[j]);

                        std::string tmpName_fnc_pol_rel = Form("fnc_%s_pol_rel_%d", tmpName_diff.c_str(), j);
                        std::string tmpHistName_fnc_pol_rel = Form("h1D_%s", tmpName_fnc_pol_rel.c_str());
                        sysHist[iFile][i].h1D_fnc_pol_rel[j] = (TH1D*)sysHist[iFile][i].h1D_fnc_pol[j]->Clone(tmpHistName_fnc_pol_rel.c_str());
                        sysHist[iFile][i].h1D_fnc_pol_rel[j]->Divide(sysHist[iFile][i].h1D_nominal);
                        // TH1 style
                        sysHist[iFile][i].h1D_fnc_pol_rel[j]->SetMarkerColor(SYS::POLFNC_COLORS[j]);
                        sysHist[iFile][i].h1D_fnc_pol_rel[j]->SetMarkerStyle(SYS::POLFNC_MARKERSTYLES[j]);
                        sysHist[iFile][i].h1D_fnc_pol_rel[j]->GetFunction(tmpName_fnc_pol.c_str())->SetBit(TF1::kNotDraw);
                        // TH1 style - END

                        // add h1D_fnc_pol to the total uncertainty histogram
                        addSysUnc(sysHist[0][i].h1D_fnc_pol[j], sysHist[iFile][i].h1D_fnc_pol[j]);

                        // add h1D_fnc_pol_rel to the total uncertainty histogram
                        addSysUnc(sysHist[0][i].h1D_fnc_pol_rel[j], sysHist[iFile][i].h1D_fnc_pol_rel[j]);

                        // initial version of h1D_fnc_pol before any other setting
                        std::string tmpName_fnc_pol_v1 = Form("fnc_%s_pol_v1_%d", tmpName_diff.c_str(), j);
                        std::string tmpHistName_fnc_pol_v1 = Form("h1D_%s", tmpName_fnc_pol_v1.c_str());
                        sysHist[iFile][i].h1D_fnc_pol_v1[j] = (TH1D*)sysHist[iFile][i].h1D_fnc_pol[j]->Clone(tmpHistName_fnc_pol_v1.c_str());
                        sysHist[0][i].h1D_fnc_pol_v1[j] = (TH1D*)sysHist[0][i].h1D_fnc_pol[j]->Clone(sysHist[0][i].h1D_fnc_pol_v1[j]->GetName());

                        // initial version of h1D_fnc_pol_rel before any other setting
                        std::string tmpName_fnc_pol_rel_v1 = Form("fnc_%s_pol_rel_v1_%d", tmpName_diff.c_str(), j);
                        std::string tmpHistName_fnc_pol_rel_v1 = Form("h1D_%s", tmpName_fnc_pol_rel_v1.c_str());
                        sysHist[iFile][i].h1D_fnc_pol_rel_v1[j] = (TH1D*)sysHist[iFile][i].h1D_fnc_pol_rel[j]->Clone(tmpHistName_fnc_pol_rel_v1.c_str());
                        sysHist[0][i].h1D_fnc_pol_rel_v1[j] = (TH1D*)sysHist[0][i].h1D_fnc_pol_rel[j]->Clone(sysHist[0][i].h1D_fnc_pol_rel_v1[j]->GetName());
                    }

                    // all systematics are assumed to have been calculated after we have "iFile == nInputFiles-1"
                    if (iFile == nInputFiles-1) {
                        // finalize total uncertainty functions
                        for (int j = 0; j < SYS::kN_SYSPOLFNC; ++j) {
                            std::string uncTot_diff_Formula = sysHist[0][i].fnc_pol_formula[j].c_str();
                            sysHist[0][i].fnc_pol_formula[j] = Form("TMath::Sqrt(%s)", uncTot_diff_Formula.c_str());
                            sysHist[0][i].fnc_pol[j] = new TF1(sysHist[0][i].fnc_pol[j]->GetName(),
                                                               sysHist[0][i].fnc_pol_formula[j].c_str(),
                                                               sysHist[0][i].fnc_pol[j]->GetXmin(), sysHist[0][i].fnc_pol[j]->GetXmax());
                            sysHist[0][i].fnc_pol[j]->SetLineColor(SYS::POLFNC_COLORS[j]);
                        }
                    }

                    // plot systematics
                    for (int iCanvas = 0; iCanvas < 4; ++iCanvas) {
                        // this canvas will plot total uncertainty, skip until all the files are processed.
                        if (iCanvas > 1 && iFile < nInputFiles-1) continue;

                        std::string tmpCnvName = "";
                        if (iCanvas == 0) {         // draw difference
                            tmpCnvName = Form("cnv_%s", tmpName_diff.c_str());
                        } else if (iCanvas == 1) {       // draw relative difference
                            tmpCnvName = Form("cnv_%s", tmpName_diff_rel.c_str());
                        } else if (iCanvas == 2 && iFile == nInputFiles-1) {  // draw total uncertainty after all individual uncertainties are calculated.
                            tmpCnvName = Form("cnv_%s", tmpName_uncTot_diff.c_str());
                        } else if (iCanvas == 3 && iFile == nInputFiles-1) {  // draw total rel. uncertainty after all individual uncertainties are calculated.
                            tmpCnvName = Form("cnv_%s", tmpName_uncTot_diff_rel.c_str());
                        }
                        c1 = new TCanvas("cnv", "", windowWidth, windowHeight);
                        c1->SetName(tmpCnvName.c_str());
                        std::cout << "preparing canvas : " << c1->GetName() << std::endl;

                        setCanvasMargin(c1, leftMargin, rightMargin, bottomMargin, topMargin);
                        setCanvasFinal(c1, setLogx, setLogy);
                        c1->cd();

                        TLegend* leg = new TLegend();
                        double yHorizontal = -1;
                        if (iCanvas == 0) {         // draw difference
                            sysHist[iFile][i].h1D_nominal->SetMarkerColor(kRed);
                            sysHist[iFile][i].h1D_nominal->Draw("e same");
                            sysHist[iFile][i].h1D_varied->SetMarkerColor(kBlue);
                            sysHist[iFile][i].h1D_varied->Draw("e same");

                            sysHist[iFile][i].h1D_diff->Draw("e same");
                            sysHist[iFile][i].h1D_diff->Write("", TObject::kOverwrite);

                            leg = new TLegend(0.5, 0.6, 0.9, 0.85);
                            leg->AddEntry(sysHist[iFile][i].h1D_nominal, "nominal", "lpf");
                            leg->AddEntry(sysHist[iFile][i].h1D_varied, "varied", "lpf");
                            leg->AddEntry(sysHist[iFile][i].h1D_diff, "diff = |nominal - varied|", "lpf");

                            for (int j = 0; j < SYS::kN_SYSPOLFNC; ++j) {
                                if (sysHist[iFile][i].isFit_fnc_pol[j]) {
                                    sysHist[iFile][i].fnc_pol[j]->Draw("same");
                                    leg->AddEntry(sysHist[iFile][i].fnc_pol[j], sysHist[iFile][i].fnc_pol[j]->GetExpFormula().Data(), "l");
                                }
                            }

                            yHorizontal = 0;
                        } else if (iCanvas == 1) {         // draw relative difference
                            sysHist[iFile][i].h1D_diff_rel->Draw("e same");
                            sysHist[iFile][i].h1D_diff_rel->Write("", TObject::kOverwrite);

                            for (int j = 0; j < SYS::kN_SYSPOLFNC; ++j) {
                                if (sysHist[iFile][i].isFit_fnc_pol[j]) {
                                    sysHist[iFile][i].h1D_fnc_pol_rel[j]->Draw("e same");
                                }
                            }

                            yHorizontal = 0;
                        } else if (iCanvas == 2 && iFile == nInputFiles-1) {
                            // fit and draw total uncertainty after all individual uncertainties are calculated.
                            // initialize fit function
                            std::string tmpFncName_uncTotLinear = Form("fnc_%s_uncTot_%d", tmpName.c_str(), iFile);
                            int binFirst = sysHist[0][i].h1D_diff->GetXaxis()->GetFirst();
                            int binLast  = sysHist[0][i].h1D_diff->GetXaxis()->GetLast();
                            double TF1_xMin = sysHist[0][i].h1D_diff->GetXaxis()->GetBinLowEdge(binFirst);
                            double TF1_xMax = sysHist[0][i].h1D_diff->GetXaxis()->GetBinLowEdge(binLast+1);
                            if (correlation.compare("xjg") == 0) {
                                TF1_xMin = 0.2;
                                TF1_xMax = 1.2;
                            } else if (correlation.compare("dphi_rebin") == 0) {
                                TF1_xMin = 1.5;
                            } else if (correlation.compare("ptJet") == 0) {
                                TF1_xMin = jetpt;
                            }
                            fnc_uncTot[i] = new TF1(tmpFncName_uncTotLinear.c_str(), functionFormulas_uncTot.at(i).c_str(), TF1_xMin, TF1_xMax);

                            // fit the total uncertainty with a linear function
                            int fitColor = EColor::kRed;
                            fnc_uncTot[i]->SetLineColor(fitColor);
                            fnc_uncTot[i]->SetLineWidth(2);
                            fnc_uncTot[i]->SetLineStyle(kDashed);
                            if (functionFormulas_uncTot.at(i).compare("NULL") != 0)
                                sysHist[0][i].h1D_diff->Fit(fnc_uncTot[i]->GetName(), "chi2 R M");
                            fnc_uncTot[i]->Write(tmpFncName_uncTotLinear.c_str(), TObject::kOverwrite);

                            double tmpMax = sysHist[0][i].h1D_diff->GetMaximum();
                            sysHist[0][i].h1D_diff->SetMinimum(0);
                            sysHist[0][i].h1D_diff->SetMaximum(tmpMax + 2*TMath::Abs(tmpMax));

                            sysHist[0][i].h1D_diff->Draw("e same");
                            sysHist[0][i].h1D_diff->Write("", TObject::kOverwrite);

                            for (int j = 0; j < SYS::kN_SYSPOLFNC; ++j) {
                                // debug with cout
                                std::cout << "uncTot_diff_Formula = " << sysHist[0][i].fnc_pol_formula[j].c_str() << std::endl;
                                std::cout << "sysHist[0][i].fnc_pol[j] name = " << sysHist[0][i].fnc_pol[j]->GetName() << std::endl;
                                std::cout << "xmin = " << sysHist[0][i].fnc_pol[j]->GetXmin() << std::endl;
                                std::cout << "xmax = " << sysHist[0][i].fnc_pol[j]->GetXmax() << std::endl;

                                sysHist[0][i].fnc_pol[j]->Draw("same");
                            }

                            yHorizontal = 0;
                        } else if (iCanvas == 3 && iFile == nInputFiles-1) {
                            // fit and draw total uncertainty after all individual uncertainties are calculated.
                            // initialize fit function
                            std::string tmpFncName_uncTotLinear = Form("fnc_%s_uncTot_%d", tmpName.c_str(), iFile);
                            int binFirst = sysHist[0][i].h1D_diff_rel->GetXaxis()->GetFirst();
                            int binLast  = sysHist[0][i].h1D_diff_rel->GetXaxis()->GetLast();
                            double TF1_xMin = sysHist[0][i].h1D_diff_rel->GetXaxis()->GetBinLowEdge(binFirst);
                            double TF1_xMax = sysHist[0][i].h1D_diff_rel->GetXaxis()->GetBinLowEdge(binLast+1);
                            if (correlation.compare("xjg") == 0) {
                                TF1_xMin = 0.2;
                                TF1_xMax = 1.2;
                            } else if (correlation.compare("dphi_rebin") == 0) {
                                TF1_xMin = 1.5;
                            } else if (correlation.compare("ptJet") == 0) {
                                TF1_xMin = jetpt;
                            }
                            fnc_uncTot[i] = new TF1(tmpFncName_uncTotLinear.c_str(), functionFormulas_uncTot.at(i).c_str(), TF1_xMin, TF1_xMax);

                            // fit the total uncertainty with a linear function
                            int fitColor = EColor::kRed;
                            fnc_uncTot[i]->SetLineColor(fitColor);
                            fnc_uncTot[i]->SetLineWidth(2);
                            fnc_uncTot[i]->SetLineStyle(kDashed);
                            if (functionFormulas_uncTot.at(i).compare("NULL") != 0)
                                sysHist[0][i].h1D_diff_rel->Fit(fnc_uncTot[i]->GetName(), "chi2 R M");
                            fnc_uncTot[i]->Write(tmpFncName_uncTotLinear.c_str(), TObject::kOverwrite);

                            sysHist[0][i].h1D_diff_rel->Draw("e same");
                            sysHist[0][i].h1D_diff_rel->Write("", TObject::kOverwrite);

                            for (int j = 0; j < SYS::kN_SYSPOLFNC; ++j) {
                                sysHist[0][i].h1D_fnc_pol_rel[j]->Draw("e same");
                            }

                            yHorizontal = 0;
                        }

                        if (leg != 0) {
                            leg->SetBorderSize(0);
                            leg->Draw();
                        }

                        // draw a horizontal line
                        double xmin = -1;
                        double xmax = 1;
                        if (sysHist[0][i].isValid) {
                            xmin = sysHist[0][i].h1D_varied->GetXaxis()->GetBinLowEdge(sysHist[0][i].h1D_varied->GetXaxis()->GetFirst());
                            xmax = sysHist[0][i].h1D_varied->GetXaxis()->GetBinLowEdge(sysHist[0][i].h1D_varied->GetXaxis()->GetLast()+1);
                        }
                        TLine line_horizontal(xmin, yHorizontal, xmax, yHorizontal);
                        line_horizontal.SetLineStyle(kDashed);
                        line_horizontal.Draw();

                        int cent_low = (int)(bins_hiBin[0].at(iHiBin)/2);
                        int cent_up  = (int)(bins_hiBin[1].at(iHiBin)/2);
                        // put info onto the canvas

                        std::vector<std::string> textLines;
                        std::string collisionText;
                        if (isHI)
                            collisionText = Form("PbPb, %d-%d %%", cent_low, cent_up);
                        else
                            collisionText = "pp";
                        bool writeText = false;     // decide whether to write more info than collision type
                        textLines.push_back(collisionText.c_str());
                        if (writeText) {
                            if (phoPt2 > 0 && phoPt2 < 9999)
                                textLines.push_back(Form("%d < p_{T}^{#gamma} < %d GeV/c", phoPt1, phoPt2));
                            else
                                textLines.push_back(Form("p_{T}^{#gamma} > %d GeV/c", phoPt1));
                            textLines.push_back(Form("p_{T}^{Jet} > %d GeV/c, |#eta^{Jet}| < %.1f", (int)jetpt, jeteta));
                            if (jetCollection.size() > 0)
                                textLines.push_back(jetCollection.c_str());
                            if (textWriteDphi.at(i)) {
                                if (awayRange > 0) {
                                    // awayRange = 78 means dphi > 7/8 PI
                                    std::string tmp_dphi = Form("#Delta#phi_{J#gamma} > #frac{%d}{%d}#pi", int(awayRange/10), int(awayRange)%10);
                                    textLines.push_back(tmp_dphi.c_str());
                                }
                            }
                        }
                        int nTextLines = textLines.size();
                        std::string textPosition = textPositions.at(i).c_str();

                        TLatex* latex = 0;
                        if (nTextLines > 0) {
                            latex = new TLatex();
                            latex->SetTextFont(textFont);
                            latex->SetTextSize(textSize);
                            setTextAlignment(latex, textPosition.c_str());
                            std::vector< std::pair<float, float> > textCoordinates = calcTextCoordinates(textLines, textPosition.c_str(), c1, textOffsetX, textOffsetY);
                            for (int i = 0; i<nTextLines; ++i) {
                                float x = textCoordinates.at(i).first;
                                float y = textCoordinates.at(i).second;
                                latex->DrawLatexNDC(x, y, textLines.at(i).c_str());
                            }
                        }

                        // add Text above the pad
                        TLatex* latexOverPad = 0;
                        if (nTextsOverPad > 0) {
                            latexOverPad = new TLatex();
                            latexOverPad->SetTextFont(textAbovePadFont);
                            latexOverPad->SetTextSize(textAbovePadSize);
                            for (int i = 0; i < nTextsOverPad; ++i) {
                                int textOverPadAlignment = GRAPHICS::textAlign;
                                if (nTextsOverPadAlignments == 1)
                                    textOverPadAlignment = GraphicsConfigurationParser::ParseTextAlign(textsOverPadAlignments.at(0));
                                else if (nTextsOverPadAlignments == nTextsOverPad)
                                    textOverPadAlignment = GraphicsConfigurationParser::ParseTextAlign(textsOverPadAlignments.at(i));

                                latexOverPad->SetTextAlign(textOverPadAlignment);
                                setTextAbovePad(latexOverPad, c1, textAbovePadOffsetX, textAbovePadOffsetY);

                                latexOverPad->DrawLatexNDC(latexOverPad->GetX(), latexOverPad->GetY(), textsOverPad.at(i).c_str());
                            }
                        }

                        c1->Write("", TObject::kOverwrite);
                        std::string outputFigureName = c1->GetName();
                        if (!outputFigurePrefix.EqualTo("")) {
                            outputFigureName = Form("%s%s", outputFigurePrefix.Data(), c1->GetName());
                        }
                        c1->SaveAs(Form("%s.png", outputFigureName.c_str()));
                        c1->SaveAs(Form("%s.pdf", outputFigureName.c_str()));
                        // c1->SaveAs(Form("%s.C", outputFigureName.c_str()));

                        c1->Close();         // do not use Delete() for TCanvas.
                        std::cout << "##########" << std::endl;
                    }
                    // plot systematics - END

                    if (iFile == nInputFiles - 1) {
                        // print SYS. UNC. summary for this observable
                        std::cout << "### SYS. UNC. summary ###" << std::endl;
                        std::cout << "histogram name = " << tmpHistName.c_str() << std::endl;
                        for (int iFile2 = 1; iFile2 <= iFile; ++iFile2) {
                            if (!sysHist[iFile2][i].isSet) continue;

                            // use the sys. unc. estimated by a line fit (pol1)
                            int fitFncIndex = SYS::kPOL1;
                            if (correlation.compare("xjg") == 0)
                                fitFncIndex = 3;

                            int nBins = sysHist[iFile2][i].h1D_fnc_pol_rel[fitFncIndex]->GetNbinsX();
                            int binMin = sysHist[iFile2][i].h1D_fnc_pol_rel[fitFncIndex]->GetMinimumBin();
                            int binMax = sysHist[iFile2][i].h1D_fnc_pol_rel[fitFncIndex]->GetMaximumBin();

                            double uncMin = sysHist[iFile2][i].h1D_fnc_pol_rel[fitFncIndex]->GetBinContent(binMin);
                            double uncMax = sysHist[iFile2][i].h1D_fnc_pol_rel[fitFncIndex]->GetBinContent(binMax);
                            // assume the bin in the middle has the ave. sys. unc.
                            // this needs more thinking
                            double uncAve = sysHist[iFile2][i].h1D_fnc_pol_rel[fitFncIndex]->GetBinContent((nBins+1)/2);
                            std::cout << Form("# sys.unc. file %d : ", iFile2);
                            std::cout << "uncMin = " << uncMin << ", uncMax = " << uncMax << ", uncAve = " << uncAve << std::endl;
                        }
                        std::cout << "### SYS. UNC. summary - END ###" << std::endl;
                    }
                }
                // LOOP over SYSTEMATICS - END
            }
        }
    }

    std::cout << "##### END #####" << std::endl;

    configTree->Write("", TObject::kOverwrite);

    for (int i = 0; i<nInputFiles; ++i) {
        if (inputExists[i])
            input[i]->Close();
    }

    output->Write("", TObject::kOverwrite);
    output->Close();

    return 0;
}

int main(int argc, char** argv) {
    if (argc == 5)
        return gammaJetSystematics(argv[1], argv[2], argv[3], argv[4]);
    else
        printf("Usage : \n"
               "./gammaJetSystematics.exe <configFile> <inputFile> <outputFile> <outputPrefix>\n");

    return 1;
}
