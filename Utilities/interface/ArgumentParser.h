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

}

class ArgumentParser {

public :
    static std::vector<std::string> ParseParameters(int argc, char** argv);
    static std::vector<std::string> ParseOptions(int argc, char** argv);
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

#endif
