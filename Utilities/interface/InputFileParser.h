#ifndef UTILITIES_INTERFACE_INPUTFILEPARSER_H_
#define UTILITIES_INTERFACE_INPUTFILEPARSER_H_

#include <TString.h>

#include <vector>
#include <string>
#include <iostream>
#include <fstream>      // ifstream, ofstream

class InputFileParser {
public:
    InputFileParser(){};
    ~InputFileParser(){};

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

    static std::vector<std::string> parse(std::string fileName) {

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
};

#endif /* UTILITIES_INTERFACE_INPUTFILEPARSER_H_ */
