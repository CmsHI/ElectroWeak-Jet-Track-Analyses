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
#include "../th1Util.h"
#include "../eventUtil.h"

namespace CONFIGPARSER{

const std::string comment = "#";
const std::string nullInput = "$NULL$";    // explicit specification of no input
const std::string noTrim = "$NOTRIM$";     // element will not be trimmed from the line.
const std::string newLine = "$NEWLINE$";   // the value continues over the next line. useful when entering a long list of values.
const std::string multiLineBegin = "$MLB$";   // sign the beginning of a multi line value
const std::string multiLineEnd = "$MLE$";     // sign the end of a multi line value
const std::string times = "$TIMES$";       // operator for multiple lists
const std::string plus = "$PLUS$";       // operator for multiple lists
const std::string importStatement = "import.";
const std::string importInputStatement = "import.input";
const std::string importCutStatement = "import.cut";
const std::string importConfigStatement = "import.config";
const std::string varDefinition = "var.";
const std::string varDefinitionString = "var.string";

enum KW {
    k_PARSESAMPLENAME,
    k_PARSEEVENTSEL,
    kN_KEYWORDS
};

const std::string KW_LABELS[kN_KEYWORDS] = {
        "$@PARSESAMPLENAME$",
        "$@PARSEEVENTSEL$"
};

enum ENV {
    k_EWJTAHOME,
    k_EWJTAOUT,
    kN_ENV
};

const std::string ENV_LABELS[kN_ENV] = {
        "$$EWJTAHOME$",
        "$$EWJTAOUT$"
};

const std::string separator = ",";
const std::string separator1 = ";";     // 1st alternative to separator
const std::string separator2 = ";;";    // 2nd alternative to separator
const std::string separator3 = ";;;";   // 3rd alternative, separator used for list of lists

struct RunLumiEvent {
  unsigned int run, lumi;
  unsigned long long event;
};

/*
 * object for reading TH1 axis information from configuration file
 */
struct TH1Axis {
    TH1Axis() : nBins(0), xLow(0), xUp(-1), bins({}), binsAreFromUser(false) {}
    void setBins() {
        bins = getTH1xBins(nBins, xLow, xUp);
    }

    int nBins;
    double xLow;
    double xUp;
    std::vector<double> bins;   // vector of size nBins+1
    bool binsAreFromUser;
};

/*
 * object for reading TH2D axis information from configuration file
 */
struct TH2DAxis {
    TH2DAxis() {}

    TH1Axis axisX;
    TH1Axis axisY;
};

/*
 * object for reading TH1 operation information from configuration file
 */
struct TH1Ops {
    TH1Ops() : histIndex(-1), bin(-1), x(-999), operand(-998877) {}
    bool histIndexValid() {
        return (histIndex != -1);
    }
    bool useBinNumber() {
        return (bin != -1);
    }
    std::string verbose() {
        std::string res;
        res.append(Form("{ histIndex = %d, ", histIndex));
        res.append(Form("bin = %d, ", bin));
        res.append(Form("x = %f, ", x));
        res.append(Form("operand = %f, ", operand));
        res.append(Form("operation = %s }", operation.c_str()));
        return res;
    }

    int histIndex;      // index of reference TH1D for operation
    int bin;            // bin wrt which the operation will be done
    double x;           // Operation will be done wrt. the bin corresponding to x value
    double operand;
    std::string operation;  // "scale", "add"
};

}

class ConfigurationParser {

public :
public:
    ConfigurationParser() {
    };
    ~ConfigurationParser() {};

    static bool isList(std::string str, std::string bracketLeft = "{", std::string bracketRight = "}");
    static bool isMultipleList(std::string str);
    static bool isComment(std::string line);
    static bool isCommand(std::string line);
    static bool isImportStatement(std::string line);
    static bool isImportInputStatement(std::string line);
    static bool isImportCutStatement(std::string line);
    static bool isImportConfigStatement(std::string line);
    static bool isVarDefinition(std::string line);
    static bool isVarDefinitionString(std::string line);
    static std::string getMultiListOperator(std::string strList);
    static std::vector<std::string> getVecString(std::vector<std::pair<std::string, int>> vecStringIndex, int increment = 1, int offset = 0);
    static std::vector<int> getVecInteger(std::vector<std::pair<int, int>> vecIntegerIndex);
    static std::vector<float> getVecFloat(std::vector<std::pair<float, int>> vecFloatIndex);
    static std::vector<int> getVecIndex(std::vector<std::pair<std::string, int>> vecStringIndex);
    static std::vector<int> getVecIndex(std::vector<std::pair<int, int>> vecIntegerIndex);
    static std::vector<int> getVecIndex(std::vector<std::pair<float, int>> vecFloatIndex);
    static std::string getEnvName(std::string strEnvLabel);
    static std::string varReference(std::string varName);
    static std::string trimComment(std::string line);
    static std::string ReadValue(std::ifstream& fin, std::string value);
    static std::string substituteVarString(std::string value, std::map<std::string, std::string> mapVarString);
    static std::string substituteEnv(std::string line);
    static std::pair<std::string, std::string> ParseVarDefinitionString(std::string command, std::string value);
    static void insertVarString(std::pair<std::string, std::string> pairKeyValue, std::map<std::string, std::string>& mapVarString);
    static std::vector<std::string> ParseList(std::string strList, std::string separator = "");
    static std::vector<std::string> ParseListOrString(std::string strListOrString);
    static std::vector<std::string> ParseMultipleLists(std::string strLists, std::string separator = "");
    static std::vector<std::string> ParseListWithoutBracket(std::string strList, std::string separator = "");
    static std::vector<int> ParseListWithoutBracketInteger(std::string strList, std::string separator = "");
    static std::vector<float> ParseListWithoutBracketFloat(std::string strList, std::string separator = "");
    static std::vector<int> ParseListInteger(std::string strList);
    static std::vector<int> ParseListOrInteger(std::string strListOrInteger);
    static std::vector<float> ParseListFloat(std::string strList);
    static std::vector<float> ParseListOrFloat(std::string strListOrFloat);
    static std::vector<std::pair<std::string, int>> ParseListOfList(std::string strListOfList);
    static std::vector<std::pair<std::string, int>> ParseListOfList(std::vector<std::string> listStrList);
    static std::vector<std::pair<int, int>> ParseListOfListInteger(std::string strListOfList);
    static std::vector<std::pair<int, int>> ParseListOfListInteger(std::vector<std::string> listStrList);
    static std::vector<std::pair<float, int>> ParseListOfListFloat(std::string strListOfList);
    static std::vector<std::pair<float, int>> ParseListOfListFloat(std::vector<std::string> listStrList);
    static CONFIGPARSER::RunLumiEvent ParseRunLumiEvent(std::string strRunLumiEvent);
    static unsigned int ParseRunNumber(std::string strRunLumiEvent);
    static unsigned int ParseLumiNumber(std::string strRunLumiEvent);
    static unsigned long long ParseEventNumber(std::string strRunLumiEvent);
    static std::string ParseSampleName(std::string fileName);
    static std::vector<std::string> ParseKeyWords(std::vector<std::string> argsStr, std::vector<int> argsInt);
    static std::string replaceKeyWords(std::string value, std::vector<std::string> parsedKW);
    static std::vector<std::vector<float>> ParseListMultiplet(std::string strList, int n);
    static std::vector<std::vector<float>> ParseListTriplet(std::string strList);
    static std::vector<std::vector<float>> ParseListTH1D_Bins(std::string strList);
    static std::vector<CONFIGPARSER::TH1Axis> ParseListTH1D_Axis(std::string strList);
    static std::vector<std::vector<float>> ParseListTH2D_Bins(std::string strList);
    static std::vector<CONFIGPARSER::TH2DAxis> ParseListTH2D_Axis(std::string strList);
    static std::string verboseTH1D_Axis(CONFIGPARSER::TH1Axis th1Axis);
    static std::string verboseTH2D_Axis(CONFIGPARSER::TH2DAxis th2DAxis);
    static std::vector<CONFIGPARSER::TH1Ops> ParseListTH1Ops(std::string strList);
    static std::string ParseLatex(std::string str);
    static std::vector<std::string> ParseListLatex(std::string strList, std::string separator = "");
    static std::vector<std::vector<std::string>> ParseListTF1(std::string strList);
    static std::vector<std::string> ParseListTF1Formula(std::string strList);
    static std::vector<std::vector<double>> ParseListTF1Range(std::string strList);

    std::string replaceKeyWords(std::string value);
    int openConfigFile(std::string fileName);
    std::string ReadConfigValue(std::string configName);
    int ReadConfigValueInteger(std::string configName);
    float ReadConfigValueFloat(std::string configName);
    std::string ReadConfigValue(std::ifstream& fin, std::string configName);
    std::string ReadConfigValue(std::string fileName, std::string configName);
    int ReadConfigValueInteger(std::string fileName, std::string configName);
    float ReadConfigValueFloat(std::string fileName, std::string configName);

    std::ifstream ifConfFile;

    std::map<std::string, std::string> mapVarStr;    // map of variables of type string

    std::vector<std::string> parsedKeyWords;
};

/*
 * a string is a list if it starts with bracketLeft (default "{") and ends with bracketRight (default "}")
 */
bool ConfigurationParser::isList(std::string str, std::string bracketLeft, std::string bracketRight)
{
    std::string tmp = trim(str);
    return (tmp.find(bracketLeft.c_str()) == 0 && tmp.rfind(bracketRight.c_str()) == tmp.size()-1);
}

/*
 * a string is a multiple list if it there are multiple lists separated by special delimiters
 */
bool ConfigurationParser::isMultipleList(std::string str)
{
    std::string tmp = trim(str);
    std::string operatorStr = getMultiListOperator(str);
    if (operatorStr.size() == 0)  return false;

    std::vector<std::string> tmpVec = split(str, operatorStr);
    if (tmpVec.size() <= 1) return false;
    else {
        for (std::vector<std::string>::iterator it = tmpVec.begin(); it != tmpVec.end(); ++it) {
            if (!isList((*it)))  return false;
        }
    }
    return true;
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

bool ConfigurationParser::isImportConfigStatement(std::string line)
{
    std::string tmp = trim(line);
    return (tmp.find(CONFIGPARSER::importConfigStatement.c_str()) == 0);
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

std::string ConfigurationParser::getMultiListOperator(std::string strList)
{
    std::string res;
    if (strList.find(CONFIGPARSER::times) != std::string::npos)
        res = CONFIGPARSER::times;
    else if (strList.find(CONFIGPARSER::plus) != std::string::npos)
        res = CONFIGPARSER::plus;

    return res;
}

std::vector<std::string> ConfigurationParser::getVecString(std::vector<std::pair<std::string, int>> vecStringIndex, int increment, int offset)
{
    std::vector<std::string> list;

    if (offset >= increment) {
        // wrong input
        return list;
    }

    int n = vecStringIndex.size();
    for (int i = 0; i < n; i+=increment) {
        list.push_back(vecStringIndex.at(i+offset).first);
    }

    return list;
}

std::vector<int> ConfigurationParser::getVecInteger(std::vector<std::pair<int, int>> vecIntegerIndex)
{
    std::vector<int> list;

    int n = vecIntegerIndex.size();
    for (int i = 0; i < n; ++i) {
        list.push_back(vecIntegerIndex.at(i).first);
    }

    return list;
}

std::vector<float> ConfigurationParser::getVecFloat(std::vector<std::pair<float, int>> vecFloatIndex)
{
    std::vector<float> list;

    int n = vecFloatIndex.size();
    for (int i = 0; i < n; ++i) {
        list.push_back(vecFloatIndex.at(i).first);
    }

    return list;
}

std::vector<int> ConfigurationParser::getVecIndex(std::vector<std::pair<std::string, int>> vecStringIndex)
{
    std::vector<int> list;

    int n = vecStringIndex.size();
    for (int i = 0; i < n; ++i) {
        list.push_back(vecStringIndex.at(i).second);
    }

    return list;
}

std::vector<int> ConfigurationParser::getVecIndex(std::vector<std::pair<int, int>> vecIntegerIndex)
{
    std::vector<int> list;

    int n = vecIntegerIndex.size();
    for (int i = 0; i < n; ++i) {
        list.push_back(vecIntegerIndex.at(i).second);
    }

    return list;
}

std::vector<int> ConfigurationParser::getVecIndex(std::vector<std::pair<float, int>> vecFloatIndex)
{
    std::vector<int> list;

    int n = vecFloatIndex.size();
    for (int i = 0; i < n; ++i) {
        list.push_back(vecFloatIndex.at(i).second);
    }

    return list;
}

/*
 * name of an environment variable does not contain any "$"
 */
std::string ConfigurationParser::getEnvName(std::string strEnvLabel)
{
    return replaceAll(strEnvLabel, "$", "");
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
 * read the value over multiple lines
 * 1. as long as the lines finish with "CONFIGPARSER::newLine" key.
 * OR
 * 2. if the value has started with "CONFIGPARSER::multiLineBegin", then read over multiple lines until "CONFIGPARSER::multiLineEnd" is found.
 */
std::string ConfigurationParser::ReadValue(std::ifstream& fin, std::string value)
{
    std::string result = trim(value);
    // ends with newLine
    bool endsWithNL = endsWith(result, CONFIGPARSER::newLine.c_str());

    // started with multiLineBegin
    bool startedWithMLB = startsWith(result, CONFIGPARSER::multiLineBegin.c_str());
    bool endsWithMLE = endsWith(result, CONFIGPARSER::multiLineEnd.c_str());

    if (endsWithNL || (startedWithMLB && !endsWithMLE)) {

        result = replaceAll(result, CONFIGPARSER::newLine.c_str(), "");
        result = replaceAll(result, CONFIGPARSER::multiLineBegin.c_str(), "");
        result = trim(result);  // need to trim the line again after removing newLine string
        std::string nextLine;
        while(getline(fin, nextLine)) {
            std::string trimmedLine = trimComment(nextLine);
            trimmedLine = trim(trimmedLine);
            if (trimmedLine.size() == 0) continue;  //skip all empty lines or the ones starting with comment sign

            endsWithNL = endsWith(trimmedLine, CONFIGPARSER::newLine.c_str());
            trimmedLine = replaceAll(trimmedLine, CONFIGPARSER::newLine.c_str(), "");
            trimmedLine = trim(trimmedLine);  // need to trim the line again after removing newLine string

            endsWithMLE = endsWith(trimmedLine, CONFIGPARSER::multiLineEnd.c_str());
            trimmedLine = replaceAll(trimmedLine, CONFIGPARSER::multiLineEnd.c_str(), "");
            trimmedLine = trim(trimmedLine);  // need to trim the line again after removing multiLineEnd string

            result.append(trimmedLine);
            if(!startedWithMLB && !endsWithNL) {
                break;
                /*
                 * input.someField = AAA    $NEWLINE$
                                     BBB    $NEWLINE$
                                     CCC
                    ==> someField = AAABBBCCC
                 */
            }
            else if (startedWithMLB && endsWithMLE) {
                break;
                /*
                 * input.someField = $MLB$ AAA
                                           BBB
                                           CCC $MLE$
                    ==> someField = AAABBBCCC
                 */
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

std::string ConfigurationParser::substituteEnv(std::string line)
{
    for (int iEnv = 0; iEnv < CONFIGPARSER::kN_ENV; ++iEnv)
    {
        std::string envName = getEnvName(CONFIGPARSER::ENV_LABELS[iEnv]);
        std::string envValue = getEnvironmentVariable(envName);
        line = replaceAll(line, CONFIGPARSER::ENV_LABELS[iEnv], envValue);
    }

    return line;
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

/*
 * insert "pairKeyValue" into "mapVarString".
 * If the key of "pairKeyValue" already exists in "mapVarString", then overwrite it in "mapVarString" with the value in "pairKeyValue"
 */
void ConfigurationParser::insertVarString(std::pair<std::string, std::string> pairKeyValue, std::map<std::string, std::string>& mapVarString)
{
    std::map<std::string, std::string>::iterator it = mapVarString.find(pairKeyValue.first.c_str());
    if (it != mapVarString.end()) {
        it->second = pairKeyValue.second;
    }
    else {
        mapVarString.insert(pairKeyValue);
    }
}

std::vector<std::string> ConfigurationParser::ParseList(std::string strList, std::string separator)
{
    std::vector<std::string> list;

    if(strList.empty())
        return list;

    if(isMultipleList(strList))  return ParseMultipleLists(strList, separator);

    size_t posStart = strList.find("{");     // a valid list starts with '{' and ends with '}'
    if (posStart == std::string::npos) return list;

    size_t posEnd   = strList.rfind("}");     // a valid list starts with '{' and ends with '}'
    if (posEnd == std::string::npos) return list;

    if (separator.size() == 0) {
        // by default, elements of the list are separated by ","
        separator = CONFIGPARSER::separator.c_str();
        // allow "," to be a list element
        // if one wants to use comma inside a list element, then one should use ";;" as element separator
        if (strList.find(CONFIGPARSER::separator2.c_str()) != std::string::npos)  separator = CONFIGPARSER::separator2.c_str();
        // if one wants to use ";;" inside a list element for lower-level separation, then one should use ";;;" as element separator
        if (strList.find(CONFIGPARSER::separator3.c_str()) != std::string::npos)  separator = CONFIGPARSER::separator3.c_str();
    }

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

        if(element.size() > 0 ) list.push_back(element.c_str());
        posStart = pos + separator.size();
    }

    return list;
}

/*
 * if the string is a list that starts with '{' and ends with '}', then parse it as list.
 * Otherwise, return a vector whose only element is the string itself.
 *
 * This function is to be used for configurations that can be a string or a list
 * Ex.
 * input.plotting.legendPosition = NE             # string
 * input.plotting.legendPosition = {NE, SW, NW}   # list
 */
std::vector<std::string> ConfigurationParser::ParseListOrString(std::string strListOrString)
{
    if (trim(strListOrString).size() == 0)
        return {};
    else if (!isList(strListOrString))
        return {strListOrString};
    else
        return ParseList(strListOrString);
}

/*
 * parse multiple lists separated by special operators
 * Ex.
 * strLists = {AA, BB} $TIMES$ {11, 22} $TIMES$ {CC, DD, EE}
 * returns
 * {AA11CC, BB11CC, AA22CC, BB22CC, AA11DD, BB11DD, AA22DD, BB22DD, AA11EE, BB11EE, AA22EE, BB22EE}
 * Ex.
 * strLists = {AA, BB} $PLUS$ {11, 22} $PLUS$ {CC, DD}
 * returns
 * {AA, 11, CC, BB, 11, CC, AA, 22, CC, BB, 22, CC, AA, 11, DD, BB, 11, DD, AA, 22, DD, BB, 22, DD}
 */
std::vector<std::string> ConfigurationParser::ParseMultipleLists(std::string strLists, std::string separator)
{
    std::string operatorStr = getMultiListOperator(strLists);
    std::vector<std::string> strListsVec = split(strLists, operatorStr);

    int nLists = strListsVec.size();
    std::vector<std::string> lists[nLists];

    for (int iList = 0; iList < nLists; ++iList) {
        lists[iList] = ParseList(strListsVec[iList], separator);
    }

    std::vector<std::string> list;
    if (operatorStr == CONFIGPARSER::times) {
        for (int iList = 0; iList < nLists; ++iList) {

            // take the list from previous iteration
            std::vector<std::string> listTmp = list;

            // the list from previous iteration is no more the one we want.
            list.clear();

            int n = lists[iList].size();
            for (int i = 0; i < n; ++i) {

                int nTmp = listTmp.size();
                std::vector<std::string> listTmp2 = listTmp;

                // append strings from the current list to strings from the previous iteration
                if (nTmp == 0) listTmp2.push_back(lists[iList][i]);
                else {
                    for (int j = 0; j < nTmp; ++j) {
                        listTmp2[j].append(lists[iList][i].c_str());
                    }
                }

                list.insert(list.end(), listTmp2.begin(), listTmp2.end());
            }
        }
    }
    else if (operatorStr == CONFIGPARSER::plus) {

        int nTot = 0;
        for (int iList = 0; iList < nLists; ++iList) {
            if (iList == 0)  nTot  = lists[iList].size();
            else             nTot *= lists[iList].size();
        }

        list.clear();
        list.resize(nTot*nLists);
        for (int i = 0; i < nTot; ++i) {

            for (int iList = nLists -1 ; iList >= 0; --iList) {

                int nTmp = nTot;
                for (int jList = iList+1 ; jList < nLists; ++jList) {
                    nTmp /= lists[jList].size();
                }
                int iTmp = i % nTmp;
                nTmp /= lists[iList].size();
                iTmp /= nTmp;

                list[i * nLists + iList] = lists[iList][iTmp];
            }
        }
    }

    return list;
}

/*
 * parse a list of strings where the list is not enclosed with brackets.
 */
std::vector<std::string> ConfigurationParser::ParseListWithoutBracket(std::string strList, std::string separator)
{
    std::vector<std::string> list;

    if(strList.empty())
        return list;

    size_t posStart = 0;
    size_t posEnd   = strList.size();

    if (separator.size() == 0) {
        // by default, elements of the list are separated by ","
        separator = CONFIGPARSER::separator.c_str();
        // allow "," to be a list element
        // if one wants to use comma inside a list element, then one should use ";;" as element separator
        if (strList.find(CONFIGPARSER::separator2.c_str()) != std::string::npos)  separator = CONFIGPARSER::separator2.c_str();
        // if one wants to use ";;" inside a list element for lower-level separation, then one should use ";;;" as element separator
        if (strList.find(CONFIGPARSER::separator3.c_str()) != std::string::npos)  separator = CONFIGPARSER::separator3.c_str();
    }

    size_t pos;
    bool listFinished = false;
    while (!listFinished) {

        pos = strList.find(separator, posStart);
        if (pos == std::string::npos) {    // this must be the last element.
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

        if(element.size() > 0 ) list.push_back(element.c_str());
        posStart = pos + separator.size();
    }

    return list;
}


/*
 * parse a list of integers where the list is not enclosed with brackets.
 */
std::vector<int> ConfigurationParser::ParseListWithoutBracketInteger(std::string strList, std::string separator)
{
    std::vector<int> list;
    std::vector<std::string> listStr = ParseListWithoutBracket(strList, separator);

    for (std::vector<std::string>::const_iterator it = listStr.begin(); it < listStr.end(); ++it) {

        std::string element = trim((*it).c_str());
        std::istringstream sin(element);

        int val;
        sin >> val;

        list.push_back(val);
    }

    return list;
}

/*
 * parse a list of floats where the list is not enclosed with brackets.
 */
std::vector<float> ConfigurationParser::ParseListWithoutBracketFloat(std::string strList, std::string separator)
{
    std::vector<float> list;
    std::vector<std::string> listStr = ParseListWithoutBracket(strList, separator);

    for (std::vector<std::string>::const_iterator it = listStr.begin(); it < listStr.end(); ++it) {

        std::string element = trim((*it).c_str());
        std::istringstream sin(element);

        float val;
        sin >> val;

        list.push_back(val);
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

        if(element.size() > 0 ) list.push_back(val);
        posStart = pos + 1;
    }

    return list;
}

std::vector<int> ConfigurationParser::ParseListOrInteger(std::string strListOrInteger)
{
    if (trim(strListOrInteger).size() == 0) {
        return {};
    }
    else if (!isList(strListOrInteger)) {
        std::istringstream sin(strListOrInteger);
        int val;
        sin >> val;
        return {val};
    }
    else
        return ParseListInteger(strListOrInteger);
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

std::vector<float> ConfigurationParser::ParseListOrFloat(std::string strListOrFloat)
{
    if (trim(strListOrFloat).size() == 0) {
        return {};
    }
    else if (!isList(strListOrFloat)) {
        std::istringstream sin(strListOrFloat);
        float val;
        sin >> val;
        return {val};
    }
    else
        return ParseListFloat(strListOrFloat);
}

std::vector<std::pair<std::string, int>> ConfigurationParser::ParseListOfList(std::string strListOfList)
{
    // strListOfList is a list of lists where lists are separated by separator3
    std::vector<std::string> strList = ParseList(strListOfList, CONFIGPARSER::separator3);

    return ParseListOfList(strList);
}

std::vector<std::pair<std::string, int>> ConfigurationParser::ParseListOfList(std::vector<std::string> listStrList)
{
    std::vector<std::pair<std::string, int>> listStringIndex;

    int nLists = listStrList.size();
    for (int i = 0; i < nLists; ++i) {

        std::vector<std::string> tmpList = ConfigurationParser::ParseListWithoutBracket(listStrList.at(i).c_str());
        int nElements = tmpList.size();

        for (int j = 0; j < nElements; ++j) {
            listStringIndex.push_back(std::pair<std::string, int>(tmpList.at(j).c_str(), i));
        }
    }

    return listStringIndex;
}

std::vector<std::pair<int, int>> ConfigurationParser::ParseListOfListInteger(std::string strListOfList)
{
    // strListOfList is a list of lists where lists are separated by separator3
    std::vector<std::string> strList = ParseList(strListOfList, CONFIGPARSER::separator3);

    return ParseListOfListInteger(strList);
}

std::vector<std::pair<int, int>> ConfigurationParser::ParseListOfListInteger(std::vector<std::string> listStrList)
{
    std::vector<std::pair<int, int>> listIntegerIndex;

    int nLists = listStrList.size();
    for (int i = 0; i < nLists; ++i) {

        std::vector<int> tmpList = ConfigurationParser::ParseListWithoutBracketInteger(listStrList.at(i).c_str());
        int nElements = tmpList.size();

        for (int j = 0; j < nElements; ++j) {
            listIntegerIndex.push_back(std::pair<int, int>(tmpList.at(j), i));
        }
    }

    return listIntegerIndex;
}

std::vector<std::pair<float, int>> ConfigurationParser::ParseListOfListFloat(std::string strListOfList)
{
    // strListOfList is a list of lists where lists are separated by separator3
    std::vector<std::string> strList = ParseList(strListOfList, CONFIGPARSER::separator3);

    return ParseListOfListFloat(strList);
}

std::vector<std::pair<float, int>> ConfigurationParser::ParseListOfListFloat(std::vector<std::string> listStrList)
{
    std::vector<std::pair<float, int>> listFloatIndex;

    int nLists = listStrList.size();
    for (int i = 0; i < nLists; ++i) {

        std::vector<float> tmpList = ConfigurationParser::ParseListWithoutBracketFloat(listStrList.at(i).c_str());
        int nElements = tmpList.size();

        for (int j = 0; j < nElements; ++j) {
            listFloatIndex.push_back(std::pair<float, int>(tmpList.at(j), i));
        }
    }

    return listFloatIndex;
}

/*
 * parse a string with run, lumi, and event info
 */
CONFIGPARSER::RunLumiEvent ConfigurationParser::ParseRunLumiEvent(std::string strRunLumiEvent)
{
    CONFIGPARSER::RunLumiEvent rle;
    rle.run = 0;
    rle.lumi = 0;
    rle.event = 0;

    if(strRunLumiEvent.empty())
        return rle;

    // by default run, lumi, and event are separated by " "
    std::string separator = " ";
    // if the string contains ":", then it becomes the default separator.
    if (strRunLumiEvent.find(":") != std::string::npos)  separator = ":";

    std::vector<std::string> vRunLumiEvent = split(strRunLumiEvent, separator);

    int vSize = vRunLumiEvent.size();
    unsigned int run, lumi;
    unsigned long long event;
    if (vSize == 3) {
        std::istringstream sin0(vRunLumiEvent.at(0));
        std::istringstream sin1(vRunLumiEvent.at(1));
        std::istringstream sin2(vRunLumiEvent.at(2));

        sin0 >> run;
        sin1 >> lumi;
        sin2 >> event;

        rle.run = run;
        rle.lumi = lumi;
        rle.event = event;
    }
    else if (vSize == 2) {
        std::istringstream sin0(vRunLumiEvent.at(0));
        std::istringstream sin1(vRunLumiEvent.at(1));

        // if there are 2 numbers, then assume those are run and event numbers
        sin0 >> run;
        sin1 >> event;

        rle.run = run;
        rle.event = event;
    }

    return rle;
}

unsigned int ConfigurationParser::ParseRunNumber(std::string strRunLumiEvent)
{
    CONFIGPARSER::RunLumiEvent rle = ParseRunLumiEvent(strRunLumiEvent);
    return rle.run;
}

unsigned int ConfigurationParser::ParseLumiNumber(std::string strRunLumiEvent)
{
    CONFIGPARSER::RunLumiEvent rle = ParseRunLumiEvent(strRunLumiEvent);
    return rle.lumi;
}

unsigned long long ConfigurationParser::ParseEventNumber(std::string strRunLumiEvent){
    CONFIGPARSER::RunLumiEvent rle = ParseRunLumiEvent(strRunLumiEvent);
    return rle.event;
}

/*
 * extract a reasonable sample name from the given file path
 */
std::string ConfigurationParser::ParseSampleName(std::string fileName)
{
    std::string res = fileName;

    bool isData = (toLowerCase(res).find("promptreco") != std::string::npos ||
                   toLowerCase(res).find("run2015") != std::string::npos ||
                   toLowerCase(res).find("run2016") != std::string::npos);
    bool hasPythia = (toLowerCase(res).find("pythia") != std::string::npos);
    bool hasHydjet = (toLowerCase(res).find("hydjet") != std::string::npos);
    bool hasEpos = (toLowerCase(res).find("epos") != std::string::npos);
    bool hasHijing = (toLowerCase(res).find("hijing") != std::string::npos);
    bool hasMadgraph = (toLowerCase(res).find("amcatnlo") != std::string::npos);

    std::string data_mc_Str = "";
    if (isData) {
        if (toLowerCase(fileName).find("pbpb") != std::string::npos || toLowerCase(fileName).find("hirun2015") != std::string::npos) {
            data_mc_Str = "PbPb Data";
        }
        else if (toLowerCase(fileName).find("ppb") != std::string::npos || toLowerCase(fileName).find("pbp") != std::string::npos
                                                                        || toLowerCase(fileName).find("parun2016") != std::string::npos) {
            data_mc_Str = "pPb Data";
            if (toLowerCase(fileName).find("parun2016b") != std::string::npos) {
                data_mc_Str = "pPb Data, 5 TeV";
            }
            else if (toLowerCase(fileName).find("parun2016c") != std::string::npos) {
                data_mc_Str = "pPb Data, 8 TeV";
            }
        }
        else if (toLowerCase(fileName).find("pp") != std::string::npos || toLowerCase(fileName).find("run2015e") != std::string::npos) {
            data_mc_Str = "pp Data";
        }
        else {
            data_mc_Str = "Data";
        }
    }
    else {
        if (hasMadgraph && hasHydjet)
            data_mc_Str = "Madgraph+H";
        else if (hasMadgraph && hasEpos)
            data_mc_Str = "Madgraph+Epos";
        else if (hasMadgraph && hasHijing)
            data_mc_Str = "Madgraph+Hijing";
        else if (hasPythia && hasHydjet)
            data_mc_Str = "P+H";
        else if (hasPythia && hasEpos)
            data_mc_Str = "P+Epos";
        else if (hasPythia && hasHijing)
            data_mc_Str = "P+Hijing";
        else if (hasHydjet)
            data_mc_Str = "Hydjet";
        else if (hasEpos)
            data_mc_Str = "Epos";
        else if (hasHijing)
            data_mc_Str = "Hijing";
        else if (hasMadgraph)
            data_mc_Str = "Madgraph";
        else if (hasPythia)
            data_mc_Str = "Pythia";
    }

    // remove frequently occuring unnecessary information
    res = replaceAll(res, "/mnt/hadoop/cms/store/user/", "", false);
    res = replaceAll(res, "xrootd.cmsaf.mit.edu", "", false);
    res = replaceAll(res, "/export/d00/scratch/", "", false);
    res = replaceAll(res, ".root", "", false);
    res = replaceAll(res, "PromptReco", "", false);
    res = replaceAll(res, "RECO", "", false);
    res = replaceAll(res, "Run2015", "", false);
    res = replaceAll(res, "Run2016", "", false);
    res = replaceAll(res, "FOREST", "", false);
    res = replaceAll(res, "mcRun2", "", false);
    res = replaceAll(res, "HeavyIon", "", false);
    res = replaceAll(res, "TuneCUETP8M1", "", false);
    res = replaceAll(res, "asymptotic", "", false);
    res = replaceAll(res, "ppAt5TeV", "", false);
    res = replaceAll(res, "amcatnlo", "", false);
    res = replaceAll(res, "official", "", false);

    // split by any of "/", "_", "-"
    res = replaceAll(res, "/", " ");
    res = replaceAll(res, "_", " ");
    res = replaceAll(res, "-", " ");

    std::string ptHatInfo = "";
    std::vector<std::string> strList = split(res, " ");
    int nStrList = strList.size();
    for (int i = nStrList - 1; i >= 0; --i) {

        if (!isData) {
            size_t pos = toLowerCase(strList.at(i)).find("pthat");
            if (pos != std::string::npos) {
                // look at the current string for any numbers
                std::string pthatXXX = strList.at(i).substr(pos+std::string("pthat").length(), 3);
                std::string pthatXX = strList.at(i).substr(pos+std::string("pthat").length(), 2);
                if (isInteger(pthatXXX))  ptHatInfo = pthatXXX;
                else if (isInteger(pthatXX))  ptHatInfo = pthatXX;

                if (ptHatInfo.size() == 0 && i+1 < nStrList) {
                    // look at the string after and see if it is a number
                    if (isInteger(strList.at(i+1)))  ptHatInfo = strList.at(i+1);
                }
            }
        }
        if (toLowerCase(strList.at(i)).find("winter") != std::string::npos ||
            toLowerCase(strList.at(i)).find("summer") != std::string::npos) {

            strList.erase(strList.begin() + i);
        }
    }

    std::sort(
        strList.begin(),
        strList.end(),
        [](const std::string& a, const std::string& b) {
            return a.size() > b.size();
    });

    res = "";
    if (strList.size() > 0) {
        // assuming the remaining longest string is the process (if MC) or PD (if data) name
        res = strList[0];
    }
    if (ptHatInfo.size() > 0) res.append(Form(" %s", ptHatInfo.c_str()));
    if (data_mc_Str.size() > 0)  res.append(Form(" %s", data_mc_Str.c_str()));

    return res;
}

std::vector<std::string> ConfigurationParser::ParseKeyWords(std::vector<std::string> argsStr, std::vector<int> argsInt)
{
    std::vector<std::string> res(CONFIGPARSER::kN_KEYWORDS);

    res[CONFIGPARSER::k_PARSESAMPLENAME] = ParseSampleName(argsStr[0]);
    res[CONFIGPARSER::k_PARSEEVENTSEL] = getEventSelection((COLL::TYPE)argsInt[0]);

    return res;
}

std::string ConfigurationParser::replaceKeyWords(std::string value, std::vector<std::string> parsedKW)
{
    int nKW = parsedKW.size();

    if (nKW == CONFIGPARSER::kN_KEYWORDS) {
        for (int i = 0; i < nKW; ++i)
        {
            if (parsedKW[i].size() == 0)  continue;

            value = replaceAll(value, CONFIGPARSER::KW_LABELS[i], parsedKW[i]);
        }
    }

    return value;
}

/*
 * parse list where each item is a sequence of "n" many numbers
 */
std::vector<std::vector<float>> ConfigurationParser::ParseListMultiplet(std::string strList, int n)
{
    std::vector<std::vector<float>> list(n);

    std::vector<float> listFlat = ParseListFloat(strList);
    if (listFlat.size() % n != 0)   return list;

    for (std::vector<float>::iterator it = listFlat.begin() ; it != listFlat.end(); it+=n) {
        for (int j = 0; j < n; ++j) {
            list[j].push_back(*(it+j));
        }
    }

    return list;
}

/*
 * parse list where each item is a sequence of three numbers
 */
std::vector<std::vector<float>> ConfigurationParser::ParseListTriplet(std::string strList)
{
    return ParseListMultiplet(strList, 3);
}

/*
 * list[0].at(i);   nBins for the ith TH1D histogram
 * list[1].at(i);   xLow  for the ith TH1D histogram
 * list[2].at(i);   xUp   for the ith TH1D histogram
 */
std::vector<std::vector<float>> ConfigurationParser::ParseListTH1D_Bins(std::string strList)
{
    return ParseListTriplet(strList);
}

std::vector<CONFIGPARSER::TH1Axis> ConfigurationParser::ParseListTH1D_Axis(std::string strList)
{
    std::vector<CONFIGPARSER::TH1Axis> list;

    // split TH1D axis information by ";;"
    std::vector<std::string> strListTH1D_Axis = ParseList(strList, CONFIGPARSER::separator2.c_str());

    for (std::vector<std::string>::iterator it = strListTH1D_Axis.begin() ; it != strListTH1D_Axis.end(); ++it) {

        std::string strTH1D_Axis = (*it);

        CONFIGPARSER::TH1Axis th1Axis;

        // a string is an array of TH1D bins if it starts with "[" and ends with "]"
        if (isList(strTH1D_Axis, "[", "]")) {
            strTH1D_Axis = replaceAll(strTH1D_Axis, "[", "{");
            strTH1D_Axis = replaceAll(strTH1D_Axis, "]", "}");
            // listBinEdges is an array of nBins+1 numbers where
            // 1. the first "nBins" of elements are bin low edges
            // 2. the last element is the last bin upper edges.
            std::vector<float> listFloat = ParseListFloat(strTH1D_Axis);
            std::vector<double> listBinEdges(listFloat.begin(), listFloat.end());

            th1Axis.nBins = listBinEdges.size()-1;
            th1Axis.xLow = listBinEdges.at(0);
            th1Axis.xUp = listBinEdges.at(listBinEdges.size()-1);
            th1Axis.bins = listBinEdges;
            th1Axis.binsAreFromUser = true;
        }
        else {
            std::vector<float> listTmp = ParseListWithoutBracketFloat(strTH1D_Axis);
            if (listTmp.size() == 3) {  // listTmp = { nBins, xLow, xUp }

                th1Axis.nBins = listTmp[0];
                th1Axis.xLow = listTmp[1];
                th1Axis.xUp = listTmp[2];
                th1Axis.binsAreFromUser = false;
                th1Axis.setBins();
            }
        }

        list.push_back(th1Axis);
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
std::vector<std::vector<float>> ConfigurationParser::ParseListTH2D_Bins(std::string strList)
{

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
 * assumes that different TH2DAxis info is split by ";;;"
 * assumes that xAxis and yAxis info is split by ";;"
 */
std::vector<CONFIGPARSER::TH2DAxis> ConfigurationParser::ParseListTH2D_Axis(std::string strList)
{
    std::vector<CONFIGPARSER::TH2DAxis> list;

    // split TH2D axis information by ";;;"
    std::vector<std::string> strListTH2D_Axis = ParseList(strList, CONFIGPARSER::separator3.c_str());

    for (std::vector<std::string>::iterator it = strListTH2D_Axis.begin() ; it != strListTH2D_Axis.end(); ++it) {

        std::string strTH2D_Axis = (*it);

        // change the string such that it is a list of 2 TH1Axis objects
        std::vector<CONFIGPARSER::TH1Axis> listTH1Axis = ParseListTH1D_Axis(Form("{ %s }", strTH2D_Axis.c_str()));

        CONFIGPARSER::TH2DAxis th2DAxis;
        if (listTH1Axis.size() == 2) {
            th2DAxis.axisX = listTH1Axis.at(0);
            th2DAxis.axisY = listTH1Axis.at(1);
        }

        list.push_back(th2DAxis);
    }

    return list;
}

std::string ConfigurationParser::verboseTH1D_Axis(CONFIGPARSER::TH1Axis th1Axis)
{
    std::string res;

    if (th1Axis.nBins > 0) {
        res.append(Form("{ %d, ", th1Axis.nBins));
        res.append(Form("%f, ", th1Axis.xLow));
        res.append(Form("%f }", th1Axis.xUp));

        if (th1Axis.binsAreFromUser) {

            res.append(", [ ");

            for (int iBin=0; iBin < th1Axis.nBins; ++iBin) {
                res.append(Form("%.2f, ", th1Axis.bins.at(iBin)));
            }
            res.append(Form("%.2f ]", th1Axis.bins.at(th1Axis.nBins)));
        }
    }

    return res;
}

std::string ConfigurationParser::verboseTH2D_Axis(CONFIGPARSER::TH2DAxis th2DAxis)
{
    std::string resX = verboseTH1D_Axis(th2DAxis.axisX);
    std::string resY = verboseTH1D_Axis(th2DAxis.axisY);

    std::string res = Form("xAxis : %s, yAxis : %s", resX.c_str(), resY.c_str());

    return res;
}

std::vector<CONFIGPARSER::TH1Ops> ConfigurationParser::ParseListTH1Ops(std::string strList)
{

    std::vector<CONFIGPARSER::TH1Ops> list;

    // split TH1Ops information by ";;"
    std::vector<std::string> strListTH1Ops = ParseList(strList, CONFIGPARSER::separator1.c_str());

    for (std::vector<std::string>::iterator it = strListTH1Ops.begin() ; it != strListTH1Ops.end(); ++it) {

        std::string strTH1Ops = (*it);

        CONFIGPARSER::TH1Ops th1Ops;

        // a string is a list containing histPath, bin/xUser, operand, operation if it starts with "[" and ends with "]"
        if (isList(strTH1Ops, "[", "]")) {
            strTH1Ops = replaceAll(strTH1Ops, "[", "{");
            strTH1Ops = replaceAll(strTH1Ops, "]", "}");

            std::vector<std::string> listParams = ParseList(strTH1Ops);
            // 1st element is the index of the reference TH1
            // 2nd element is the bin or the x-axis value of the reference point
            // 3rd element is the operand
            // 4th element (if exists) is the operation
            th1Ops.histIndex = std::atoi(listParams.at(0).c_str());

            std::string binOrxValStr = listParams.at(1);
            if (binOrxValStr.find("bin") != std::string::npos) {
                th1Ops.bin = std::atoi(binOrxValStr.substr(binOrxValStr.find("=")+1).c_str());
            }
            else if (binOrxValStr.find("x") != std::string::npos) {
                th1Ops.x = std::atof(binOrxValStr.substr(binOrxValStr.find("=")+1).c_str());
            }

            th1Ops.operand = std::atof(listParams.at(2).c_str());

            if ((int)(listParams.size()) == 4) {
                th1Ops.operation = listParams.at(3);
            }
            else {
                th1Ops.operation = "scale";
            }
        }
        else {
            th1Ops.operand = std::atof(strTH1Ops.c_str());
            th1Ops.operation = "scale";
        }

        list.push_back(th1Ops);
    }

    return list;

}

/*
 * parse the Latex syntax in the configuration file to the Latex syntax in ROOT
 */
std::string ConfigurationParser::ParseLatex(std::string str)
{
    std::string strNew = replaceAll(str, "\\", "#");
    return strNew;
}

/*
 * parse a list of Latex strings
 */
std::vector<std::string> ConfigurationParser::ParseListLatex(std::string strList, std::string separator)
{
    std::vector<std::string> list;

    if(strList.empty())
        return list;

    size_t posStart = 0;
    size_t posEnd   = strList.size();

    if (separator.size() == 0) {
        // by default, elements of the list are separated by ","
        separator = CONFIGPARSER::separator.c_str();
        // allow "," to be a list element
        // if one wants to use comma inside a list element, then one should use ";;" as element separator
        if (strList.find(CONFIGPARSER::separator2.c_str()) != std::string::npos)  separator = CONFIGPARSER::separator2.c_str();
        // if one wants to use ";;" inside a list element for lower-level separation, then one should use ";;;" as element separator
        if (strList.find(CONFIGPARSER::separator3.c_str()) != std::string::npos)  separator = CONFIGPARSER::separator3.c_str();
    }

    size_t pos;
    bool listFinished = false;
    while (!listFinished) {
        pos = strList.find(separator, posStart);
        if (pos == std::string::npos) {    // this must be the last element.
            pos = posEnd;
            listFinished = true;
        }

        std::string tmp = strList.substr(posStart, pos-posStart);  //  strList = ...,ABC123 ,... --> posStart = 0, pos = 8, tmp = "ABC123 "

        // make trimming optional, trimming can be skipped if "noTrim" key appears in the element
        bool doTrim = (tmp.find(CONFIGPARSER::noTrim) == std::string::npos);
        std::string element;
        if (doTrim) {
            element = trim(tmp);
        } else {  // do not trim, allow  leading and trailing white space characters to appear in the element.
            // remove the "do not trim" option from the element
            element = replaceAll(tmp, CONFIGPARSER::noTrim, "");
        }

        if (element.size() > 0)
            list.push_back(ConfigurationParser::ParseLatex(element.c_str()));
        posStart = pos + separator.size();
    }

    return list;
}

/*
 * list[0].at(i);   formula for the ith TF1 function
 * list[1].at(i);   xMin    for the ith TF1 function
 * list[2].at(i);   xMax    for the ith TF1 function
 */
std::vector<std::vector<std::string>> ConfigurationParser::ParseListTF1(std::string strList)
{
    std::vector<std::vector<std::string>> list(3);
    std::vector<std::string> listFlat = ParseList(strList);
    if (listFlat.size() % 3 != 0)   return list;

    for (std::vector<std::string>::iterator it = listFlat.begin() ; it != listFlat.end(); it+=3) {
        list[0].push_back(*it);
        list[1].push_back(*(it+1));
        list[2].push_back(*(it+2));
    }

    return list;
}

/*
 * list.at(i);   formula for the ith TF1 function
 */
std::vector<std::string> ConfigurationParser::ParseListTF1Formula(std::string strList)
{
    std::vector<std::string> list;
    std::vector<std::string> listFlat = ParseList(strList);
    if (listFlat.size() % 3 != 0)   return list;

    for (std::vector<std::string>::iterator it = listFlat.begin() ; it != listFlat.end(); it+=3) {
        list.push_back(*it);
    }

    return list;
}

/*
 * list[0].at(i);   xMin for the ith TF1 function
 * list[0].at(i);   xMax for the ith TF1 function
 */
std::vector<std::vector<double>> ConfigurationParser::ParseListTF1Range(std::string strList)
{
    std::vector<std::vector<double>> list(2);
    std::vector<std::string> listFlat = ParseList(strList);
    if (listFlat.size() % 3 != 0)   return list;

    for (std::vector<std::string>::iterator it = listFlat.begin() ; it != listFlat.end(); it+=3) {
        std::string str1 = *(it+1);
        std::string str2 = *(it+2);

        list[0].push_back(std::stod(str1));
        list[1].push_back(std::stod(str2));
    }

    return list;
}

std::string ConfigurationParser::replaceKeyWords(std::string value)
{
    return replaceKeyWords(value, parsedKeyWords);
}

int ConfigurationParser::openConfigFile(std::string fileName)
{
    if (ifConfFile.is_open()) {
        ifConfFile.close();
    }

    ifConfFile.open(fileName.c_str(), std::ifstream::in);

    if (ifConfFile.good()) {
        return 0;
    }
    else {
        return 1;
    }
}

std::string ConfigurationParser::ReadConfigValue(std::string configName)
{
    std::string res = ReadConfigValue(ifConfFile, configName);
    res = replaceKeyWords(res);
    return res;
}

int ConfigurationParser::ReadConfigValueInteger(std::string configName)
{
    return std::atoi(ReadConfigValue(configName).c_str());
}

float ConfigurationParser::ReadConfigValueFloat(std::string configName)
{
    return std::atof(ReadConfigValue(configName).c_str());
}

std::string ConfigurationParser::ReadConfigValue(std::ifstream& fin, std::string configName)
{
    std::string endSignal = "#CONFIG-END#";

    std::string res = "";

    if ((fin.rdstate() & std::ifstream::failbit) != 0) {
        std::cout << "I/O Error opening file." << std::endl;
        return res;
    }

    // fstream does not have to at the beginnig of file, can EOF, roll back to beginning
    fin.clear();
    fin.seekg(0, std::ifstream::beg);

    std::string configNameTmp = Form("%s ", configName.c_str());

    std::string line;
    while (getline(fin, line)) {

        line = trim(line);
        if (line.find(endSignal) != std::string::npos) break;
        if (ConfigurationParser::isComment(line)) continue;  //skip all lines starting with comment sign #
        if (line.find("=") == std::string::npos) continue; //skip all lines without an =
        //if (line.find(".") == std::string::npos) continue; //skip all lines without a dot
        size_t pos = line.find("=") + 1;
        size_t posLast = line.find(CONFIGPARSER::comment.c_str());    // allow inline comment signs with #
        std::string value = ConfigurationParser::ReadValue(fin, line.substr(pos, (posLast-pos)));   // read value over multiple lines if necessary
        value = ConfigurationParser::substituteVarString(value, mapVarStr);
        value = ConfigurationParser::substituteEnv(value);

        line = line.substr(0, pos-1);        // "line" becomes the LHS of the "=" sign (excluing the "=" sign)
        bool isCommand = ConfigurationParser::isCommand(line);
        bool configFound = (line.find(configNameTmp.c_str()) == 0);
        if (isCommand) {
            if (ConfigurationParser::isVarDefinitionString(line)) {
                insertVarString(ConfigurationParser::ParseVarDefinitionString(line, value), mapVarStr);
            } else if (ConfigurationParser::isImportConfigStatement(line)) {

                res = ReadConfigValue(value, configName);
            }
        }
        else if (configFound) {
            res = value;
        }
    }

    return res;
}

std::string ConfigurationParser::ReadConfigValue(std::string fileName, std::string configName)
{
    std::ifstream fin(fileName);
    return ReadConfigValue(fin, configName);
}

int ConfigurationParser::ReadConfigValueInteger(std::string fileName, std::string configName)
{
    return std::atoi(ReadConfigValue(fileName, configName).c_str());
}

float ConfigurationParser::ReadConfigValueFloat(std::string fileName, std::string configName)
{
    return std::atof(ReadConfigValue(fileName, configName).c_str());
}

#endif
