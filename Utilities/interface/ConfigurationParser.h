#ifndef UTILITIES_INTERFACE_CONFIGURATIONPARSER_H_
#define UTILITIES_INTERFACE_CONFIGURATIONPARSER_H_

#include <string>
#include <vector>
#include <sstream>

#include "../systemUtil.h"

class ConfigurationParser {

public :
    static std::vector<std::string> ParseList(std::string strList);
    static std::vector<int> ParseListInteger(std::string strList);
    static std::vector<float> ParseListFloat(std::string strList);
    static std::vector<std::vector<float>> ParseListTH1D_Bins(std::string strList);
    static std::vector<std::vector<float>> ParseListTH2D_Bins(std::string strList);
    static std::string ParseLatex(std::string str);

};

std::vector<std::string> ConfigurationParser::ParseList(std::string strList)
{
    std::vector<std::string> list;

    if(strList.empty())
        return list;

    size_t posStart = strList.find("{");     // a valid list starts with '{' and ends with '}'
    if (posStart == std::string::npos) return list;

    size_t posEnd   = strList.find("}");     // a valid list starts with '{' and ends with '}'
    if (posEnd == std::string::npos) return list;

    // elements of the list are separated by ','
    size_t pos;
    bool listFinished = false;
    posStart++;     // exclude '{'
    while (!listFinished) {

        pos = strList.find(",", posStart);
        if (pos == std::string::npos) {    // this must be the last element. nothing after '}' is accepted.
            pos = posEnd;
            listFinished = true;
        }

        std::string tmp = strList.substr(posStart, pos-posStart);  //  strList = ...,ABC123 ,... --> posStart = 0, pos = 8, tmp = "ABC123 "

        std::string element = trim(tmp);

        if(element.size() >0 ) list.push_back(element.c_str());
        posStart = pos + 1;
    }

    return list;
}

std::vector<int> ConfigurationParser::ParseListInteger(std::string strList)
{
    std::vector<int> list;

    if(strList.empty())
        return list;

    size_t posStart = strList.find("{");     // a valid list starts with '{' and ends with '}'
    if (posStart == std::string::npos) return list;

    size_t posEnd   = strList.find("}");     // a valid list starts with '{' and ends with '}'
    if (posEnd == std::string::npos) return list;

    // elements of the list are separated by ','
    size_t pos;
    bool listFinished = false;
    posStart++;     // exclude '{'
    while (!listFinished) {

        pos = strList.find(",", posStart);
        if (pos == std::string::npos) {    // this must be the last element. nothing after '}' is accepted.
            pos = posEnd;
            listFinished = true;
        }

        std::string tmp = strList.substr(posStart, pos-posStart);  //  strList = ...,ABC123 ,... --> posStart = 0, pos = 8, tmp = "ABC123 "

        std::string element = trim(tmp);
        std::istringstream sin(element);

        int val;
        sin >> val;

        if(element.size() >0 ) list.push_back(val);
        posStart = pos + 1;
    }

    return list;
}

std::vector<float> ConfigurationParser::ParseListFloat(std::string strList)
{
    std::vector<float> list;

    if(strList.empty())
        return list;

    size_t posStart = strList.find("{");     // a valid list starts with '{' and ends with '}'
    if (posStart == std::string::npos) return list;

    size_t posEnd   = strList.find("}");     // a valid list starts with '{' and ends with '}'
    if (posEnd == std::string::npos) return list;

    // elements of the list are separated by ','
    size_t pos;
    bool listFinished = false;
    posStart++;     // exclude '{'
    while (!listFinished) {

        pos = strList.find(",", posStart);
        if (pos == std::string::npos) {    // this must be the last element. nothing after '}' is accepted.
            pos = posEnd;
            listFinished = true;
        }

        std::string tmp = strList.substr(posStart, pos-posStart);  //  strList = ...,ABC123 ,... --> posStart = 0, pos = 8, tmp = "ABC123 "

        std::string element = trim(tmp);
        std::istringstream sin(element);

        float val;
        sin >> val;

        if(element.size() > 0 ) list.push_back(val);
        posStart = pos + 1;
    }

    return list;
}

/*
 * list[0].at(i);   nBins for the ith TH1D histogram
 * list[1].at(i);   xLow  for the ith TH1D histogram
 * list[2].at(i);   xUp   for the ith TH1D histogram
 */
std::vector<std::vector<float>> ConfigurationParser::ParseListTH1D_Bins(std::string strList){

    std::vector<std::vector<float>> list(3);

    std::vector<float> listFlat = ParseListFloat(strList);
    if (listFlat.size() % 3 != 0)   return list;

    for (std::vector<float>::iterator it = listFlat.begin() ; it != listFlat.end(); it+=3) {
        list[0].push_back(*it);
        list[1].push_back(*(it+1));
        list[2].push_back(*(it+2));
    }

    return list;
}

/*
 * list[0].at(i);   nBinsx for the ith TH2D histogram
 * list[1].at(i);   xLow   for the ith TH2D histogram
 * list[2].at(i);   xUp    for the ith TH2D histogram
 * list[3].at(i);   nBinsy for the ith TH2D histogram
 * list[4].at(i);   yLow   for the ith TH2D histogram
 * list[5].at(i);   yUp    for the ith TH2D histogram
 */
std::vector<std::vector<float>> ConfigurationParser::ParseListTH2D_Bins(std::string strList){

    std::vector<std::vector<float>> list(6);

    std::vector<float> listFlat = ParseListFloat(strList);
    if (listFlat.size() % 6 != 0)   return list;

    for (std::vector<float>::iterator it = listFlat.begin() ; it != listFlat.end(); it+=6) {
        list[0].push_back(*it);
        list[1].push_back(*(it+1));
        list[2].push_back(*(it+2));
        list[3].push_back(*(it+3));
        list[4].push_back(*(it+4));
        list[5].push_back(*(it+5));
    }

    return list;
}

/*
 * parse the Latex syntax in the configuration file to the Latex syntax in ROOT
 */
std::string ConfigurationParser::ParseLatex(std::string str) {

    std::string strNew = replaceAll(str, "\\", "#");
    return strNew;
}

#endif
