/*
 * utilities related to system operations
 */

#ifndef SYSTEMUTIL_H_
#define SYSTEMUTIL_H_

#include <string>
#include <vector>
#include <set>
#include <iostream>
#include <fstream>      // ifstream, ofstream
#include <cstdlib>
#include <algorithm>    // std::transform
#include <regex>
#include <unistd.h>     // gethostname

bool        fileExists(std::string filename);
std::vector<std::string> getLines(std::string file);
int         replaceStringInFile(std::string file, std::string oldString, std::string newString);
std::string replaceAll(std::string str, std::string oldString, std::string newString);
std::string replaceAll(std::string str, std::string oldString, std::string newString, bool matchCase);
std::string replaceFirst(std::string str, std::string oldString, std::string newString);
std::string replaceFirst(std::string str, std::string oldString, std::string newString, bool matchCase);
std::string trim(std::string str);
std::string toLowerCase(std::string str);
std::string toUpperCase(std::string str);
std::string wildCard2Regex(std::string str);
std::vector<std::string> split(std::string str, std::string delimiter, bool includeEmptySubStr = true, bool emptyVecIfNoDelimiter = true);
std::vector<double> parseListNum(std::string strList, std::string delimiter = ",");
bool hasSubstr(const std::string & str, const std::string & substr);
bool startsWith(std::string str, std::string substr);
bool endsWith(std::string str, std::string substr);
bool matchesRegex(std::string str, std::string regexStr);
bool matchesWildCard(std::string str, std::string wcStr);
bool isInteger(std::string s);
int orderOfMag(double x);
double roundToPrecision(double x, int precision);
double roundToSignificantFigures(double x, int nFigures);
double roundToSignificantFigures(double x, int nFigures, bool ignoreBeforeDecimalPoint);
int  countOccurances(std::string str, std::string substr);
bool containsElement(std::vector<std::string> vec, std::string str);
int  findPositionInVector(std::vector<std::string> vSearch, std::string str);
template <typename T>
int  findInVector(const std::vector<T>  & vec, const T  & element);
std::vector<std::string> vectorUnique(std::vector<std::string> v);
std::vector<int> positionsInVector(std::vector<std::string> vSearch, std::vector<std::string> v);
std::string getEnvironmentVariable(std::string envName);
std::string getExpandedEnvironmentVariable(std::string s);
std::string getHostName();

/*
 * just check if the file exists. better use this short function to check existence of a file,
 * it makes sure that the "ifstream" object is alive only within the scope of the function.
 *
 * http://www.cplusplus.com/forum/general/1796/ :
 *
 * "return statement should cast the file object to a boolean which is true if the file exists.
 * The file is automatically closed at the end of the function scope."
 * */
bool fileExists(std::string filename)
{
  std::ifstream file(filename.c_str());
  return (bool)file; //root CINT compiler requires explicit cast
}

/*
 * read all the lines in a file and return them in a vector of strings
 */
std::vector<std::string> getLines(std::string file)
{
    std::vector<std::string> res;

    std::ifstream inFile(file.c_str());
    std::string strLine;
    if (inFile.is_open()) {
        while (getline(inFile,strLine)) {

            res.push_back(strLine);
        }
    }
    inFile.close();

    return res;
}

/*
 * replace each occurence of "oldString" in file "filePath" with "newString"
 */
int replaceStringInFile(std::string file, std::string oldString, std::string newString)
{
    std::ifstream inFile(file.c_str());      // file to be updated

    std::string strTmp = "TEMP";
    char* charTmp = new char[file.length() + strTmp.length()];
    sprintf(charTmp, "%s%s", file.c_str(), strTmp.c_str());

    std::string tmpFilePath = charTmp;
    delete[] charTmp;
    std::ofstream outFile(tmpFilePath.c_str());  // temporary output file

    std::string strLine;
    if (inFile.is_open() && outFile.is_open())
    {
        while( getline(inFile,strLine))
        {
            std::string newStrLine = replaceAll(strLine, oldString, newString);

            outFile << newStrLine << "\n";
        }
    }

    inFile.close();
    outFile.close();

    // overwrite the original file.
    return rename(tmpFilePath.c_str() , file.c_str());
}

/*
 * replace each occurence of "oldString" in string "str" with "newString"
 */
std::string replaceAll(std::string str, std::string oldString, std::string newString)
{
    if(str.empty())
        return str;

    size_t pos = 0;
    while ((pos = str.find(oldString, pos)) != std::string::npos) {
        str.replace(pos, oldString.length(), newString);
        pos += newString.length();
    }
    return str;
}

/*
 * replace each occurence of "oldString" in string "str" with "newString"
 * If matchCase = false, then replacement is not case sensitive.
 */
std::string replaceAll(std::string str, std::string oldString, std::string newString, bool matchCase)
{
    if (matchCase)  return replaceAll(str, oldString, newString);
    else {
        if(str.empty())
            return str;

        size_t pos = 0;
        std::string strLow = toLowerCase(str);
        std::string oldStringLow = toLowerCase(oldString);
        while ((pos = strLow.find(oldStringLow, pos)) != std::string::npos) {
            str.replace(pos, oldString.length(), newString);
            strLow.replace(pos, oldStringLow.length(), newString);
            pos += newString.length();
        }
        return str;
    }
}

/*
 * replace first occurence of "oldString" in string "str" with "newString"
 */
std::string replaceFirst(std::string str, std::string oldString, std::string newString)
{
    if(str.empty())
        return str;

    size_t pos = 0;
    if ((pos = str.find(oldString, pos)) != std::string::npos) {
        str.replace(pos, oldString.length(), newString);
    }
    return str;
}

/*
 * replace first occurence of "oldString" in string "str" with "newString"
 * If matchCase = false, then replacement is not case sensitive.
 */
std::string replaceFirst(std::string str, std::string oldString, std::string newString, bool matchCase)
{
    if (matchCase)  return replaceAll(str, oldString, newString);
    else {
        if(str.empty())
            return str;

        size_t pos = 0;
        std::string strLow = toLowerCase(str);
        std::string oldStringLow = toLowerCase(oldString);
        if((pos = strLow.find(oldStringLow, pos)) != std::string::npos) {
            str.replace(pos, oldString.length(), newString);
            strLow.replace(pos, oldStringLow.length(), newString);
        }
        return str;
    }
}

/*
 * remove white spaces at the beginning and at the end of "str"
 */
std::string trim(std::string str)
{
    if (str.size() > 0)
    {
        size_t first = str.find_first_not_of(' ');
        size_t last = str.find_last_not_of(' ');
        if (first == std::string::npos && last == std::string::npos) {  // str is full of white spaces, e.g. "   "
            return "";
        }
        return str.substr(first, (last-first+1));
    }
    else {  // str = ""
        return str;
    }
}

std::string toLowerCase(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

std::string toUpperCase(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

/*
 * convert a "wildcard" expression to "regex" expression
 */
std::string wildCard2Regex(std::string str)
{
    str = replaceAll(str, "\\", "\\\\");
    str = replaceAll(str, "^", "\\^");
    str = replaceAll(str, ".", "\\.");
    str = replaceAll(str, "$", "\\$");
    str = replaceAll(str, "|", "\\|");
    str = replaceAll(str, "(", "\\(");
    str = replaceAll(str, ")", "\\)");
    str = replaceAll(str, "[", "\\[");
    str = replaceAll(str, "]", "\\]");
    str = replaceAll(str, "*", "\\*");
    str = replaceAll(str, "+", "\\+");
    str = replaceAll(str, "?", "\\?");
    str = replaceAll(str, "/", "\\/");
    // Convert chars '*?' back to their regex equivalents
    str = replaceAll(str, "\\?", ".");
    str = replaceAll(str, "\\*", ".*");

    return str;
}

/*
 * split the string wrt. a delimiter and return a vector of substrings.
 * delimiter can be longer than a single character.
 * if includeEmptySubStr is true, then empty substrings are included.
 * if the delimiter is not found, then
 *      if "emptyVecIfNoDelimiter", return an empty vector.
 *      Else, return vector with "str" being only element
 */
std::vector<std::string> split(std::string str, std::string delimiter, bool includeEmptySubStr, bool emptyVecIfNoDelimiter)
{
    std::vector<std::string> v;

     size_t posStart = 0;
     size_t pos = str.find(delimiter);
     bool finished = (pos == std::string::npos);
     if (finished && !emptyVecIfNoDelimiter) {
         if (str.size() > 0 || includeEmptySubStr) {
             return {str};
         }
     }
     while (!finished) {

         pos = str.find(delimiter, posStart);
         std::string substr = str.substr(posStart, pos-posStart);
         if (substr.size() > 0 || includeEmptySubStr) {
             v.push_back(substr);
         }
         posStart = pos + delimiter.length();

         finished = (pos == std::string::npos);
     }

     return v;
}

std::vector<double> parseListNum(std::string strList, std::string delimiter)
{
    std::vector<std::string> vecStr = split(strList, delimiter, false, false);

    std::vector<double> v;
    for (std::vector<std::string>::const_iterator it = vecStr.begin(); it != vecStr.end(); ++it) {

        v.push_back(std::atof((*it).c_str()));
    }

    return v;
}


bool hasSubstr(const std::string & str, const std::string & substr)
{
  return (str.find(substr.c_str()) != std::string::npos);
}

/*
 * return true if string "str" ends with "substr"
 */
bool startsWith(std::string str, std::string substr)
{
    if (str.size() < substr.size()) return false;
    return str.compare(0, substr.size(), substr.c_str()) == 0;
}

/*
 * return true if string "str" ends with "substr"
 */
bool endsWith(std::string str, std::string substr)
{
    if (str.size() < substr.size()) return false;
    return str.compare(str.size() - substr.size(), substr.size(), substr.c_str()) == 0;
}

/*
 * return true if string "str" matches the regex "strRegex"
 */
bool matchesRegex(std::string str, std::string regexStr)
{
    std::regex regexTmp(regexStr.c_str());

    return regex_match(str, regexTmp);
}

/*
 * return true if string "str" matches the wildcard "wcStr"
 */
bool matchesWildCard(std::string str, std::string wcStr)
{
    return matchesRegex(str, wildCard2Regex(wcStr));
}

/*
 * count the number of times a substring appears in a string
 */
bool isInteger(std::string s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

/*
 * order of magnitude
 */
int orderOfMag(double x)
{
    return (int)(std::floor(std::log10(x)));
}

/*
 * round double variable to given precision after the decimal point
 */
double roundToPrecision(double x, int precision)
{
    return round(x * std::pow(10, precision)) / std::pow(10, precision);
}

/*
 * round double variable to given number of significant figures
 */
double roundToSignificantFigures(double x, int nFigures)
{
    if (nFigures <= 0) return x;
    if (x == 0) return 0;

    double sign = (x >= 0) ? 1 : -1;
    x = std::fabs(x);

    // helps to find the first non-zero leading digit
    int orderM = orderOfMag(x);
    double res = 0;

    while (nFigures > 0) {
        double tmpDouble = (x / std::pow(10, orderM));
        /*
         * 3.0 is stored as 2.9999... Casting it to integer will give 2.
         * Add small number to close the gap and so that casting gives the correct result.
         * Double-check addition of this small number does not break things for you.
         */
        if (tmpDouble > 0) tmpDouble += 0.000000001; // 0.3 is stored as 0.2999... Add the missing gap so that the casting gives correct result.

        int tmpDigit =((int)(tmpDouble) % 10);

        if (nFigures == 1) {
            // check the digit to the right of last significant figure
            double tmpDouble2 = (x / std::pow(10, orderM-1));
            if (tmpDouble2 > 0) tmpDouble2 += 0.000000001;

            if (((int)(tmpDouble2) % 10) >= 5)  tmpDigit++;
        }
        res += (double)(tmpDigit) * std::pow(10, orderM);

        orderM--;
        nFigures--;
    }

    return res*sign;
}

/*
 * round double variable to given number of significant figures
 * with the option of considering SF only after decimal point
 */
double roundToSignificantFigures(double x, int nFigures, bool ignoreBeforeDecimalPoint)
{
    if (!ignoreBeforeDecimalPoint) return roundToSignificantFigures(x, nFigures);
    else {
        double sign = (x >= 0) ? 1 : -1;
        x = std::fabs(x);

        int xBeforeDecimalPoint = (int)x;
        double xAfterDecimalPoint = x - xBeforeDecimalPoint;

        return ((double)xBeforeDecimalPoint + roundToSignificantFigures(xAfterDecimalPoint, nFigures))*sign;
    }
}

int countOccurances(std::string str, std::string substr)
{
    int n = 0;
    size_t pos = 0;
    while ((pos = str.find(substr, pos)) != std::string::npos) {
        ++n;
        pos += substr.length();
    }
    return n;
}

/*
 * return if "str" is in vector vSearch
 */
bool containsElement(std::vector<std::string> vec, std::string str)
{
    return (std::find(vec.begin(), vec.end(), str.c_str()) != vec.end());
}

/*
 * returns false unless the string is composed of digits only
 */
/*
 * return the index of a std::string in a std::vector
 * return -1 if not found.
 */
int findPositionInVector(std::vector<std::string> vSearch, std::string str)
{
    std::vector<std::string>::iterator it = std::find(vSearch.begin(), vSearch.end(), str.c_str());
    if (it != vSearch.end())  return int(it - vSearch.begin());
    else                return -1;
}

template <typename T>
int findInVector(const std::vector<T>  & vec, const T  & element)
{
    auto it = std::find(vec.begin(), vec.end(), element);

    if (it != vec.end())
    {
        return int(it - vec.begin());
    }
    else
    {
        return -1;
    }
}


/*
 * return a vector of unique elements
 */
std::vector<std::string> vectorUnique(std::vector<std::string> v)
{
    std::set<std::string> strSet;
    for (std::vector<std::string>::const_iterator it = v.begin(); it != v.end(); ++it) {
        strSet.insert((*it));
    }

    return std::vector<std::string>(strSet.begin(), strSet.end());
}

/*
 * search in each string from on vSearch and return its position in vSearch
 * returns a vector where vector[i] is the (first) position of v[i] in vSearch
 */
std::vector<int> positionsInVector(std::vector<std::string> vSearch, std::vector<std::string> v)
{
    std::vector<int> res;
    for (std::vector<std::string>::const_iterator it = v.begin(); it != v.end(); ++it) {
        int pos = findPositionInVector(vSearch, (*it));
        res.push_back(pos);
    }

    return res;
}

/*
 * returns the value of the given environment variable
 * returns an empty string if the environment variable is not defined.
 * https://stackoverflow.com/questions/7463947/make-and-stdlogic-error/7464044#7464044
 */
std::string getEnvironmentVariable(std::string envName)
{
    const char* envValueChar = std::getenv(envName.c_str());
    std::string res = envValueChar ? envValueChar : "";
    return res;
}

/*
 * expands the environment variables in a string
 * Ex. if s = "${HOME}/some/${MYDIR}" returns "/home/myname/some/my/dir"
 * Environment variables must be given in ${VARNAME} form
 * https://stackoverflow.com/questions/1902681/expand-file-names-that-have-environment-variables-in-their-path/20715800#20715800
 */
std::string getExpandedEnvironmentVariable(std::string s)
{
    if (s.find( "${" ) == std::string::npos) return s;

    std::string pre  = s.substr(0, s.find( "${" ));
    std::string post = s.substr(s.find( "${" ) + 2);

    if (post.find( "}" ) == std::string::npos) return s;

    std::string varName = post.substr( 0, post.find( "}" ));
    std::string value    = "";

    post = post.substr(post.find("}") + 1);

    value = getEnvironmentVariable(varName);

    return getExpandedEnvironmentVariable(pre + value + post);
}

std::string getHostName()
{
    char hostname[4096];

    if (gethostname(hostname, 4096) == 0) {
        return std::string(hostname);
    }
    else {
        return "";
    }
}

#endif
