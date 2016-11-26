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
#include <algorithm>    // std::transform

bool        fileExists(std::string filename);
int         replaceStringInFile(std::string file, std::string oldString, std::string newString);
std::string replaceAll(std::string str, std::string oldString, std::string newString);
std::string trim(std::string str);
std::string toLowerCase(std::string str);
std::vector<std::string> split(std::string str, std::string delimiter);
bool endsWith(std::string str, std::string suffix);
int  findPositionInVector(std::vector<std::string> vSearch, std::string str);
std::vector<int> positionsInVector(std::vector<std::string> vSearch, std::vector<std::string> v);
std::vector<std::string> vectorUnique(std::vector<std::string> v);

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

/*
 * split the string wrt. a delimiter and return a vector of substrings.
 * delimiter can be longer than a single character.
 * empty substrings are included.
 * if the delimiter is not found, return an empty vector.
 */
std::vector<std::string> split(std::string str, std::string delimiter)
{
    std::vector<std::string> v;

     size_t posStart = 0;
     size_t pos = str.find(delimiter);
     bool finished = (pos == std::string::npos);
     while (!finished) {

         pos = str.find(delimiter, posStart);
         std::string substr = str.substr(posStart, pos-posStart);
         v.push_back(substr);
         posStart = pos + delimiter.length();

         finished = (pos == std::string::npos);
     }

     return v;
}

/*
 * return true if string "str" ends with "suffix"
 */
bool endsWith(std::string str, std::string suffix)
{
    if (str.size() < suffix.size()) return false;
    return str.compare(str.size() - suffix.size(), suffix.size(), suffix.c_str()) == 0;
}

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

#endif
