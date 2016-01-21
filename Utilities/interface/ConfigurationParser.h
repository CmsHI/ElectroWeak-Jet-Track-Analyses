#ifndef UTILITIES_INTERFACE_CONFIGURATIONPARSER_H_
#define UTILITIES_INTERFACE_CONFIGURATIONPARSER_H_

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstddef>        // std::size_t
#include <map>
#include <utility>      // std::pair

#include "../systemUtil.h"

namespace CONFIGPARSER{

const std::string comment = "#";
const std::string noTrim = "$NOTRIM$";     // element will not be trimmed from the line.
const std::string newLine = "$NEWLINE$";   // the value continues over the next line. useful when entering a long list of values.
const std::string importStatement = "import.";
const std::string importInputStatement = "import.input";
const std::string importCutStatement = "import.cut";
const std::string varDefinition = "var.";
const std::string varDefinitionString = "var.string";
}

class ConfigurationParser {

public :
    static bool isList(std::string str);
    static bool isComment(std::string line);
    static bool isCommand(std::string line);
    static bool isImportStatement(std::string line);
    static bool isImportInputStatement(std::string line);
    static bool isImportCutStatement(std::string line);
    static bool isVarDefinition(std::string line);
    static bool isVarDefinitionString(std::string line);
    static std::string varReference(std::string varName);
    static std::string trimComment(std::string line);
    static std::string ReadValue(std::ifstream& fin, std::string value);
    static std::string substituteVarString(std::string value, std::map<std::string, std::string> mapVarString);
    static std::pair<std::string, std::string> ParseVarDefinitionString(std::string command, std::string value);
    static std::vector<std::string> ParseList(std::string strList);
    static std::vector<int> ParseListInteger(std::string strList);
    static std::vector<float> ParseListFloat(std::string strList);
    static std::vector<std::vector<float>> ParseListTH1D_Bins(std::string strList);
    static std::vector<std::vector<float>> ParseListTH2D_Bins(std::string strList);
    static std::string ParseLatex(std::string str);

};

/*
 * a string is a list if it starts with "{" and ends with "}"
 */
bool ConfigurationParser::isList(std::string str)
{
    std::string tmp = trim(str);
    return (tmp.find("{") == 0 && tmp.rfind("}") == tmp.size()-1);
}

bool ConfigurationParser::isComment(std::string line)
{
    return (trim(line).find(CONFIGPARSER::comment.c_str()) == 0);
}

bool ConfigurationParser::isCommand(std::string line)
{
    return (isImportStatement(line) || isVarDefinition(line));
}

bool ConfigurationParser::isImportStatement(std::string line)
{
    std::string tmp = trim(line);
    return (tmp.find(CONFIGPARSER::importStatement.c_str()) == 0);
}

bool ConfigurationParser::isImportInputStatement(std::string line)
{
    std::string tmp = trim(line);
    return (tmp.find(CONFIGPARSER::importInputStatement.c_str()) == 0);
}

bool ConfigurationParser::isImportCutStatement(std::string line)
{
    std::string tmp = trim(line);
    return (tmp.find(CONFIGPARSER::importCutStatement.c_str()) == 0);
}

bool ConfigurationParser::isVarDefinition(std::string line)
{
    std::string tmp = trim(line);
    return (tmp.find(CONFIGPARSER::varDefinition.c_str()) == 0);
}

bool ConfigurationParser::isVarDefinitionString(std::string line)
{
    std::string tmp = trim(line);
    return (tmp.find(CONFIGPARSER::varDefinitionString.c_str()) == 0);
}

/*
 * constructs the syntax for referencing a variable
 */
std::string ConfigurationParser::varReference(std::string varName)
{
    return "$"+varName+"$";
}

std::string ConfigurationParser::trimComment(std::string line)
{
    size_t pos = line.find(CONFIGPARSER::comment.c_str());
    return line.substr(0,pos);
}

/*
 * read the value over multiple lines as long as the lines finish with "CONFIGPARSER::newLine" key.
 */
std::string ConfigurationParser::ReadValue(std::ifstream& fin, std::string value)
{
    std::string result = trim(value);
    if (endsWith(result, CONFIGPARSER::newLine.c_str())) {
        result = replaceAll(result, CONFIGPARSER::newLine.c_str(), "");
        std::string nextLine;
        while(getline(fin, nextLine)) {
            std::string trimmedLine = trimComment(nextLine);
            trimmedLine = trim(trimmedLine);
            if (trimmedLine.size() == 0) continue;  //skip all empty lines or the ones starting with comment sign
            std::string tmpValue = replaceAll(trimmedLine, CONFIGPARSER::newLine.c_str(), "");
            result.append(tmpValue);
            if(! endsWith(trimmedLine, CONFIGPARSER::newLine.c_str())) {
                break;
            }
        }
    }
    return result;
}

/*
 * substitute the values of all current string variables in "value"
 */
std::string ConfigurationParser::substituteVarString(std::string value, std::map<std::string, std::string> mapVarString)
{
    std::map<std::string, std::string>::const_iterator iterator;
    for (iterator = mapVarString.begin(); iterator != mapVarString.end(); ++iterator) {
        std::string varName = iterator->first.c_str();
        std::string varRef = varReference(varName);
        value = replaceAll(value, varRef, iterator->second.c_str());
    }

    return value;
}

/*
 * parse the name and value of a string variable.
 */
std::pair<std::string, std::string> ConfigurationParser::ParseVarDefinitionString(std::string command, std::string value)
{
    std::string varName = replaceAll(command,CONFIGPARSER::varDefinitionString,"");
    varName = trim(varName);
    value = trim(value);
    return (std::pair<std::string, std::string>(varName, value));
}

std::vector<std::string> ConfigurationParser::ParseList(std::string strList)
{
    std::vector<std::string> list;

    if(strList.empty())
        return list;

    size_t posStart = strList.find("{");     // a valid list starts with '{' and ends with '}'
    if (posStart == std::string::npos) return list;

    size_t posEnd   = strList.rfind("}");     // a valid list starts with '{' and ends with '}'
    if (posEnd == std::string::npos) return list;

    // by default, elements of the list are separated by ","
    std::string separator = ",";
    // allow "," to be a list element
    // if one wants to use comma inside a list element, then one should use ";;" as element separator
    if (strList.find(";;") != std::string::npos)  separator = ";;";

    size_t pos;
    bool listFinished = false;
    posStart++;     // exclude '{'
    while (!listFinished) {

        pos = strList.find(separator, posStart);
        if (pos == std::string::npos) {    // this must be the last element. nothing after '}' is accepted.
            pos = posEnd;
            listFinished = true;
        }

        std::string tmp = strList.substr(posStart, pos-posStart);  //  strList = ...,ABC123 ,... --> posStart = 0, pos = 8, tmp = "ABC123 "

        // make trimming optional, trimming can be skipped if "noTrim" key appears in the element
        bool doTrim = (tmp.find(CONFIGPARSER::noTrim) == std::string::npos);
        std::string element;
        if (doTrim) {
            element = trim(tmp);
        }
        else {  // do not trim, allow  leading and trailing white space characters to appear in the element.
            // remove the "do not trim" option from the element
            element = replaceAll(tmp, CONFIGPARSER::noTrim, "");
        }

        if(element.size() >0 ) list.push_back(element.c_str());
        posStart = pos + separator.size();
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

    size_t posEnd   = strList.rfind("}");     // a valid list starts with '{' and ends with '}'
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

    size_t posEnd   = strList.rfind("}");     // a valid list starts with '{' and ends with '}'
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
