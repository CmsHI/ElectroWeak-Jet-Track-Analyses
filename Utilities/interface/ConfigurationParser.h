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

        if(element.size() >0 ) list.push_back(val);
        posStart = pos + 1;
    }

    return list;
}

#endif
