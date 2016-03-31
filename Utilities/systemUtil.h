/*
 * utilities related to system operations
 */

#ifndef SYSTEMUTIL_H_
#define SYSTEMUTIL_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>      // ifstream, ofstream
#include <algorithm>    // std::transform

bool        fileExists(std::string filename);
int         replaceStringInFile(std::string file, std::string oldString, std::string newString);
std::string replaceAll(std::string str, std::string oldString, std::string newString);
std::string trim(std::string str);
std::string toLowerCase(std::string str);
bool endsWith(std::string str, std::string suffix);

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
    const char* tmpFilePath = Form("%sTEMP", file.c_str());
    std::ofstream outFile(tmpFilePath);  // temporary output file

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
    return rename(tmpFilePath , file.c_str());
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
 * return true if string "str" ends with "suffix"
 */
bool endsWith(std::string str, std::string suffix)
{
    if (str.size() < suffix.size()) return false;
    return str.compare(str.size() - suffix.size(), suffix.size(), suffix.c_str()) == 0;
}

#endif
