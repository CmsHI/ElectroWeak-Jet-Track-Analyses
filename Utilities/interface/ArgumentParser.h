/*
 * utilities for parsing command line arguments (not configurations)
 */
#ifndef UTILITIES_INTERFACE_ARGUMENTPARSER_H_
#define UTILITIES_INTERFACE_ARGUMENTPARSER_H_

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstddef>        // std::size_t
#include <map>
#include <utility>      // std::pair

#include "../systemUtil.h"

namespace ARGUMENTPARSER{

const std::string noLoop = "--noLoop";
const std::string wildCard = "--wc";
const std::string format = "--format";

}

class ArgumentParser {

public :
    static std::vector<std::string> ParseParameters(int argc, char** argv);
    static std::vector<std::string> ParseOptions(int argc, char** argv);
    static std::vector<std::string> ParseOptionInput(std::string optionType, std::string option);
    static std::vector<std::string> ParseOptionInput(std::string optionType, std::vector<std::string> options);
    static std::string ParseOptionInputSingle(std::string optionType, std::vector<std::string> options);
};

/*
 * Parameters are things like input file, output file, configuration file.
 */
std::vector<std::string> ArgumentParser::ParseParameters(int argc, char** argv)
{
    std::vector<std::string> list;

    for (int i = 0; i < argc; ++i) {

        std::string tmp = argv[i];
        // an option starts with "--" and is not a parameter
        if (tmp.find("--") != 0)  list.push_back(tmp);
    }

    return list;
}

/*
 * An option starts with "--".
 * eg. --noLoop, --plotOnly
 */
std::vector<std::string> ArgumentParser::ParseOptions(int argc, char** argv)
{
    std::vector<std::string> list;

    for (int i = 0; i < argc; ++i) {

        std::string tmp = argv[i];
        // an option starts with "--"
        if (tmp.find("--") == 0)  list.push_back(tmp);
    }

    return list;
}

/*
 * Some options come with an input, parse the input
 * Ex. optionType = "--wc"
 *     value = "--wc=AAA*BB*,123?ABC*"
 *     returns {AAA*BB*, 123?ABC*}
 * Ex. optionType = "--noLoop"
 *     value = "--noLoop"   // there is no input for this option
 *     returns {}           // return empty list
 */
std::vector<std::string> ArgumentParser::ParseOptionInput(std::string optionType, std::string option)
{
    if (option.find(Form("%s=", optionType.c_str())) != std::string::npos)
        option = replaceAll(option, Form("%s=", optionType.c_str()), "");
    else
        return {};

    if (optionType == ARGUMENTPARSER::wildCard) {
        if (option.find(",") != std::string::npos) return split(option, ",");
        else                                       return {option};
    }
    else {
        return {option};
    }
}

/*
 * Parse the input values for an option.
 * The input is parsed as a list of strings.
 */
std::vector<std::string> ArgumentParser::ParseOptionInput(std::string optionType, std::vector<std::string> options)
{
    for (std::vector<std::string>::const_iterator it = options.begin() ; it != options.end(); ++it){
        if ((*it).find(optionType.c_str()) != std::string::npos) {
            return ParseOptionInput(optionType, (*it));
        }
    }

    return {};
}

/*
 * Parse the input values for an option.
 * The input is parsed as a string.
 */
std::string ArgumentParser::ParseOptionInputSingle(std::string optionType, std::vector<std::string> options)
{
    std::vector<std::string> inputAsList =  ArgumentParser::ParseOptionInput(optionType, options);
    if (inputAsList.size()) return inputAsList.at(0);
    else                    return "";
}

#endif
