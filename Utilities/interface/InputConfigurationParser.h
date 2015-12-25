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

#include "../eventUtil.h"
#include "../systemUtil.h"

namespace INPUT {

struct ProcessInputs{
    std::vector<float> f;
    std::vector<int> i;
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
    kN_TYPES_I
};

const std::string TYPE_I_LABELS[kN_TYPES_I] = {
        "collisionType",
        "doHI",
        "doHIMC",
        "doPP",
        "doPPMC",
        "doPA",
        "doPAMC"
};

enum TYPE_S{
    k_TH1D_Bins_List,       // nBins, xLow, xUp for a TH1D histogram
    kN_TYPES_S
};

const std::string TYPE_S_LABELS[kN_TYPES_S] = {
        "TH1D_Bins_List"
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

struct InputConfiguration : public Configuration {
    INPUT::ProcessInputs proc[CUTS::kN_PROCESSES];

};

class InputConfigurationParser : public ConfigurationParser {

public:
    InputConfigurationParser(){};
    ~InputConfigurationParser(){};

    static bool isROOTfile(TString fileName) {
        return fileName.EndsWith(".root");
    }

    static bool isROOTfile(std::string fileName) {
        TString tstr = fileName.c_str();
        return isROOTfile(tstr);
    }

    static bool isFileList(TString fileName) {
        return (fileName.EndsWith(".txt") || fileName.EndsWith(".list"));
    }

    static bool isFileList(std::string fileName) {
        TString tstr = fileName.c_str();
        return isFileList(tstr);
    }

    static std::vector<std::string> ParseFiles(std::string fileName) {

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
                    // skip if the line if there is a comment sign anywhere
                    if (strLine.find("#") != std::string::npos) continue;
                    // if (!isROOTfile(strLine))                   continue;

                    // assume this line is not commented and contains a ROOT file
                    fileNames.push_back(strLine);
                }
            }
        }

        return fileNames;
    }

    static InputConfiguration Parse(std::string inFile) {

        InputConfiguration config;
        std::string endSignal = "#INPUT-END#";     // signals that input configuration parsing is to be terminated.
                                                  // another block of configuration parsing will start.

        for(int i = 0 ; i < INPUT::kN_PROCESSES; ++i){
            config.proc[i].i.resize(INPUT::kN_TYPES_I);
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
};

#endif
