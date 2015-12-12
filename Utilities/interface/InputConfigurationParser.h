#ifndef UTILITIES_INTERFACE_INPUTCONFIGURATIONPARSER_H_
#define UTILITIES_INTERFACE_INPUTCONFIGURATIONPARSER_H_

#include <TString.h>

#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>

#include "../eventUtil.h"

namespace INPUT {

enum TYPE_I{
    k_CollisionType,
    kN_TYPES_I
};

const std::string TYPE_I_LABELS[kN_TYPES_I] = {
        "collisionType"
};

};

struct InputConfiguration{
    std::vector<float> f;
    std::vector<int> i;
    std::vector<std::string> s;

    std::vector<char*> c; // this is a c-string copy of s
    bool isValid;
};

class InputConfigurationParser {
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

        config.i.resize(INPUT::kN_TYPES_I);

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
            if (line.find("#") != std::string::npos) continue; //allow # comments
            if (line.find("=") == std::string::npos) continue; //skip all lines without an =
            if (line.find("input.") == std::string::npos) continue; //skip all lines without an "input."
            std::istringstream sin(line.substr(line.find("=") + 1));
            bool success = false;
            for(int j = 0; j < INPUT::kN_TYPES_I; ++j){
                std::string label = Form(".%s",INPUT::TYPE_I_LABELS[j].c_str());    // prevent substring matching
                if (line.find(label) != std::string::npos) {
                    int in;
                    sin >> in;
                    config.i[j] = in;
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
