#ifndef UTILITIES_INTERFACE_INPUTCONFIGURATIONPARSER_H_
#define UTILITIES_INTERFACE_INPUTCONFIGURATIONPARSER_H_

#include <TString.h>

#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Configuration.h"
#include "ConfigurationParser.h"
#include "GraphicsConfigurationParser.h"

#include "../eventUtil.h"
#include "../systemUtil.h"

namespace INPUT {

struct ProcessInputs{
    std::vector<int> i;
    std::vector<float> f;
    std::vector<std::string> s;

    std::vector<char*> c; // this is a c-string copy of s
};

enum TYPE_I{
    k_CollisionType,
    k_doHI,
    k_doHIMC,
    k_doPP,
    k_doPPMC,
    k_doPA,
    k_doPAMC,
    k_drawSame,
    k_drawNormalized,
    k_setLogx,
    k_setLogy,
    k_setLogz,
    k_windowWidth,
    k_windowHeight,
    kN_TYPES_I
};

const std::string TYPE_I_LABELS[kN_TYPES_I] = {
        "collisionType",
        "doHI",
        "doHIMC",
        "doPP",
        "doPPMC",
        "doPA",
        "doPAMC",
        "drawSame",
        "drawNormalized",
        "setLogx",
        "setLogy",
        "setLogz",
        "windowWidth",
        "windowHeight"
};

enum TYPE_F{
    k_legendOffsetX,
    k_legendOffsetY,
    k_leftMargin,
    k_rightMargin,
    k_bottomMargin,
    k_topMargin,
    k_titleOffsetX,
    k_titleOffsetY,
    k_TH1_xMin,
    k_TH1_xMax,
    k_TH1_yMin,
    k_TH1_yMax,
    kN_TYPES_F
};

const std::string TYPE_F_LABELS[kN_TYPES_F] = {
        "legendOffsetX",
        "legendOffsetY",
        "leftMargin",
        "rightMargin",
        "bottomMargin",
        "topMargin",
        "titleOffsetX",
        "titleOffsetY",
        "TH1_xMin",
        "TH1_xMax",
        "TH1_yMin",
        "TH1_yMax"
};

enum TYPE_S{
    k_treeFormula,
    k_treeSelection,
    k_treeSelectionBase,    // selection that is applied/valid for all observables
    k_TH1_path,             // path to the TH1 object in a file
    k_TH1_title,
    k_TH1_titleX,
    k_TH1_titleY,
    k_TH1_weight,           // weight used when filling a histogram
    k_TH1_scale,
    k_TH1_rebin,
    k_TH1D_Bins_List,       // nBins, xLow, xUp for a TH1D histogram
    k_TH2D_Bins_List,       // nBinsx, xLow, xUp, nBinsy, yLow, yUp for a TH2D histogram
    k_treePath,
    k_treeFriends_List,
    k_legendPosition,       // one of NW, NE, SW, SE
    k_legendEntryLabel,
    k_color,
    k_markerStyle,
    k_lineStyle,
    kN_TYPES_S
};

const std::string TYPE_S_LABELS[kN_TYPES_S] = {
        "treeFormula",
        "treeSelection",
        "treeSelectionBase",
        "TH1_path",
        "TH1_title",
        "TH1_titleX",
        "TH1_titleY",
        "TH1_weight",
        "TH1_scale",
        "TH1_rebin",
        "TH1D_Bins_List",
        "TH2D_Bins_List",
        "treePath",
        "treeFriends_List",
        "legendPosition",
        "legendEntryLabel",
        "color",
        "markerStyle",
        "lineStyle"
};

enum PROCESS{
  kSKIM,
  kCORRECTION,
  kHISTOGRAM,
  kPLOTTING,
  kPERFORMANCE,
  kN_PROCESSES // must come last in enum
};

std::string PROCESS_LABELS[kN_PROCESSES] = {"skim",
                        "correction",
                        "histogram",
                        "plotting",
                        "performance"
};

};

namespace INPUT_TH1 {

enum TYPE_NORM{
  k_noNorm,         // no normalization, just a place holder
  k_normInt,        // normalization by the total number of entries, histogram integral will be 1.
  k_normEvents,     // normalization by the number of events
  kN_TYPE_NORM // must come last in enum
};

};

struct InputConfiguration : public Configuration {
    INPUT::ProcessInputs proc[CUTS::kN_PROCESSES];

};

class InputConfigurationParser : public ConfigurationParser {

public:
    InputConfigurationParser(){};
    ~InputConfigurationParser(){};

    static bool isROOTfile(TString fileName);
    static bool isROOTfile(std::string fileName);
    static bool isFileList(TString fileName);
    static bool isFileList(std::string fileName);
    static bool isConfigurationFile(TString fileName);
    static bool isConfigurationFile(std::string fileName);
    static std::vector<std::string> ParseFiles(std::string fileName);
    static InputConfiguration Parse(std::string inFile);
};

bool InputConfigurationParser::isROOTfile(TString fileName) {
    return fileName.EndsWith(".root");
}

bool InputConfigurationParser::isROOTfile(std::string fileName) {
    TString tstr = fileName.c_str();
    return isROOTfile(tstr);
}

bool InputConfigurationParser::isFileList(TString fileName) {
    return (fileName.EndsWith(".txt") || fileName.EndsWith(".list"));
}

bool InputConfigurationParser::isFileList(std::string fileName) {
    TString tstr = fileName.c_str();
    return isFileList(tstr);
}

bool InputConfigurationParser::isConfigurationFile(TString fileName) {
    return fileName.EndsWith(".conf");
}

bool InputConfigurationParser::isConfigurationFile(std::string fileName) {
    TString tstr = fileName.c_str();
    return isConfigurationFile(tstr);
}

std::vector<std::string> InputConfigurationParser::ParseFiles(std::string fileName) {

    std::vector<std::string> fileNames;

    if (isROOTfile(fileName)) {
        fileNames.push_back(fileName);
    }
    else if (isFileList(fileName)) {

        // assumes there is exactly one file per line,
        // no empty line should be entered.
        // any line containing a "#" will be skipped.
    std::ifstream inFile(fileName.c_str());
        std::string strLine;
        if (inFile.is_open())
        {
            while(getline(inFile,strLine))
            {
                if (trim(strLine).find_first_of("#") == 0) continue;  //skip all lines starting with comment sign #

                size_t posLast = strLine.find("#");    // allow inline comment signs with #
                std::string in = trim(strLine.substr(0, posLast));
                // assume this line contains a ROOT file
                fileNames.push_back(in);
            }
        }
    }
    else if (isConfigurationFile(fileName)) {

        // assumes there is exactly one file per line,
        // no empty line should be entered.
        // any line containing a "#" will be skipped.
        std::ifstream inFile(fileName.c_str());
        std::string strLine;
        if (inFile.is_open())
        {
            bool fileListFound = false;
            std::string startSignal = "#FILELIST#";
            std::string endSignal = "#FILELIST-END#";
            while(getline(inFile,strLine))
            {
                if (strLine.find(startSignal) != std::string::npos) fileListFound = true;
                else if (strLine.find(endSignal) != std::string::npos) break;

                if (!fileListFound) continue;
                if (trim(strLine).find_first_of("#") == 0) continue;  //skip all lines starting with comment sign #

                size_t posLast = strLine.find("#");    // allow inline comment signs with #
                std::string in = trim(strLine.substr(0, posLast));
                // assume this line contains a ROOT file
                fileNames.push_back(in);
            }
        }
    }

    return fileNames;
}

InputConfiguration InputConfigurationParser::Parse(std::string inFile) {

    InputConfiguration config;
    std::string endSignal = "#INPUT-END#";     // signals that input configuration parsing is to be terminated.
                                              // another block of configuration parsing will start.

    for(int i = 0 ; i < INPUT::kN_PROCESSES; ++i){
        config.proc[i].i.resize(INPUT::kN_TYPES_I);
        config.proc[i].f.resize(INPUT::kN_TYPES_F);
        config.proc[i].s.resize(INPUT::kN_TYPES_S);
        config.proc[i].c.resize(INPUT::kN_TYPES_S);
    }

    std::ifstream fin(inFile);
    if ( (fin.rdstate() & std::ifstream::failbit ) != 0 ) {
        std::cout << "I/O Error opening configuration file." << std::endl;
        config.isValid = false;
        return config;
    }

    std::string line;
    unsigned int lineCounter = 0;
    while (getline(fin, line)) {
        lineCounter++;
        if (line.find(endSignal) != std::string::npos) break;
        if (line.find("=") == std::string::npos) continue; //skip all lines without an =
        if (line.find("input.") == std::string::npos) continue; //skip all lines without an "input."
        if (line.find(".") == std::string::npos) continue; //skip all lines without a dot
        if (trim(line).find_first_of("#") == 0) continue;  //skip all lines starting with comment sign #
        size_t pos = line.find("=") + 1;
        size_t posLast = line.find("#");    // allow inline comment signs with #
        std::istringstream sin(line.substr(pos, (posLast-pos) ));
        bool success = false;
        INPUT::PROCESS proc = INPUT::kN_PROCESSES;
        for(int i = 0; i < INPUT::kN_PROCESSES; ++i){

            std::string label_proc = Form(".%s.",INPUT::PROCESS_LABELS[i].c_str());
            if (line.find(label_proc) != std::string::npos) {
                proc = (INPUT::PROCESS)i;
                break;
            }
        }

        for(int j = 0; j < INPUT::kN_TYPES_I; ++j){
            std::string label = Form(".%s ",INPUT::TYPE_I_LABELS[j].c_str());    // prevent substring matching, e.g. : "doPP" and "doPPMC"
            if (line.find(label) != std::string::npos) {
                int in;
                sin >> in;
                config.proc[proc].i[j] = in;
                success = true;
                break;
            }
        }

        for(int j = 0; j < INPUT::kN_TYPES_F; ++j){
            std::string label = Form(".%s ",INPUT::TYPE_F_LABELS[j].c_str());    // prevent substring matching, e.g. : "doPP" and "doPPMC"
            if (line.find(label) != std::string::npos) {
                float in;
                sin >> in;
                config.proc[proc].f[j] = in;
                success = true;
                break;
            }
        }

        for(int j = 0; j < INPUT::kN_TYPES_S; ++j){
            std::string label = Form(".%s ",INPUT::TYPE_S_LABELS[j].c_str());    // prevent substring matching, e.g. : "doPP" and "doPPMC"
            if (line.find(label) != std::string::npos) {
                std::string in;
                in = trim(sin.str());   // stringstream ignores characters after a whitespace, use the original string to read the value.
                config.proc[proc].s[j] = in;
                char * cstr = new char [in.length()+1];
                std::strcpy (cstr, in.c_str());
                config.proc[proc].c[j] = cstr;
                success = true;
                break;
            }
        }

        if (!success) {
            std::cout << "Malformed line in configuration." << std::endl;
            std::cout << "Line #" << lineCounter << " : " << std::endl;
            std::cout << line << std::endl;
            config.isValid = false;
            return config;
        }
    }
    config.isValid = true;
    return config;
}

#endif
