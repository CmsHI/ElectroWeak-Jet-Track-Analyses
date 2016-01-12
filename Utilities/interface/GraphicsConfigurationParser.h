#ifndef UTILITIES_INTERFACE_GRAPHICSCONFIGURATIONPARSER_H_
#define UTILITIES_INTERFACE_GRAPHICSCONFIGURATIONPARSER_H_

#include <TAttMarker.h>
#include <TAttLine.h>
#include <Rtypes.h>

#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
#include <map>

#include "ConfigurationParser.h"

namespace GRAPHICS {

// assume not more than 13 colors are needed.
const int colors[13] = {kBlack, kBlue, kRed,   kOrange,  kViolet, kCyan, kSpring, kYellow,
        kAzure, kPink, kGreen, kMagenta, kTeal};
}

class GraphicsConfigurationParser : public ConfigurationParser {

public:
    GraphicsConfigurationParser(){};
    ~GraphicsConfigurationParser(){};

    static int ParseMarkerStyle(std::string markerStyle);
    static int ParseLineStyle(std::string lineStyle);
    static int ParseColor(std::string color);
};

int GraphicsConfigurationParser::ParseMarkerStyle(std::string markerStyle)
{
    std::map<std::string, int> map;
    map.insert(std::pair<std::string, int>("kDot", EMarkerStyle::kDot));
    map.insert(std::pair<std::string, int>("kPlus", EMarkerStyle::kPlus));
    map.insert(std::pair<std::string, int>("kStar", EMarkerStyle::kStar));
    map.insert(std::pair<std::string, int>("kCircle", EMarkerStyle::kCircle));
    map.insert(std::pair<std::string, int>("kMultiply", EMarkerStyle::kMultiply));
    map.insert(std::pair<std::string, int>("kFullDotSmall", EMarkerStyle::kFullDotSmall));
    map.insert(std::pair<std::string, int>("kFullDotMedium", EMarkerStyle::kFullDotMedium));
    map.insert(std::pair<std::string, int>("kFullDotLarge", EMarkerStyle::kFullDotLarge));
    map.insert(std::pair<std::string, int>("kFullCircle", EMarkerStyle::kFullCircle));
    map.insert(std::pair<std::string, int>("kFullSquare", EMarkerStyle::kFullSquare));
    map.insert(std::pair<std::string, int>("kFullTriangleUp", EMarkerStyle::kFullTriangleUp));
    map.insert(std::pair<std::string, int>("kFullTriangleDown", EMarkerStyle::kFullTriangleDown));
    map.insert(std::pair<std::string, int>("kOpenCircle", EMarkerStyle::kOpenCircle));
    map.insert(std::pair<std::string, int>("kOpenSquare", EMarkerStyle::kOpenSquare));
    map.insert(std::pair<std::string, int>("kOpenTriangleUp", EMarkerStyle::kOpenTriangleUp));
    map.insert(std::pair<std::string, int>("kOpenDiamond", EMarkerStyle::kOpenDiamond));
    map.insert(std::pair<std::string, int>("kOpenCross", EMarkerStyle::kOpenCross));
    map.insert(std::pair<std::string, int>("kFullStar", EMarkerStyle::kFullStar));
    map.insert(std::pair<std::string, int>("kOpenStar", EMarkerStyle::kOpenStar));
    map.insert(std::pair<std::string, int>("kOpenTriangleDown", EMarkerStyle::kOpenTriangleDown));
    map.insert(std::pair<std::string, int>("kFullDiamond", EMarkerStyle::kFullDiamond));
    map.insert(std::pair<std::string, int>("kFullCross", EMarkerStyle::kFullCross));

    int result = -1;

    // if the string input can be directly parsed into integer, then return that value.
    bool isInteger = true;
    try {
        result = std::stoi(markerStyle);
    }
    catch (const std::exception &ex) {
        isInteger = false;
    }

    if (!isInteger) {
        std::map<std::string, int>::const_iterator iterator = map.find(markerStyle.c_str());

        if(iterator != map.end()) {
            result = iterator->second;
        }
    }

    return result;
}

int GraphicsConfigurationParser::ParseLineStyle(std::string lineStyle)
{
    std::map<std::string, int> map;
    map.insert(std::pair<std::string, int>("kSolid", ELineStyle::kSolid));
    map.insert(std::pair<std::string, int>("kDashed", ELineStyle::kDashed));
    map.insert(std::pair<std::string, int>("kDotted", ELineStyle::kDotted));
    map.insert(std::pair<std::string, int>("kDashDotted", ELineStyle::kDashDotted));

    int result = -1;

    // if the string input can be directly parsed into integer, then return that value.
    bool isInteger = true;
    try {
        result = std::stoi(lineStyle);
    }
    catch (const std::exception &ex) {
        isInteger = false;
    }

    if (!isInteger) {
        std::map<std::string, int>::const_iterator iterator = map.find(lineStyle.c_str());

        if(iterator != map.end()) {
            result = iterator->second;
        }
    }

    return result;
}

int GraphicsConfigurationParser::ParseColor(std::string color)
{
    std::map<std::string, int> map;
    map.insert(std::pair<std::string, int>("kWhite", EColor::kWhite));
    map.insert(std::pair<std::string, int>("kBlack", EColor::kBlack));
    map.insert(std::pair<std::string, int>("kGray", EColor::kGray));
    map.insert(std::pair<std::string, int>("kRed", EColor::kRed));
    map.insert(std::pair<std::string, int>("kGreen", EColor::kGreen));
    map.insert(std::pair<std::string, int>("kBlue", EColor::kBlue));
    map.insert(std::pair<std::string, int>("kYellow", EColor::kYellow));
    map.insert(std::pair<std::string, int>("kMagenta", EColor::kMagenta));
    map.insert(std::pair<std::string, int>("kCyan", EColor::kCyan));
    map.insert(std::pair<std::string, int>("kOrange", EColor::kOrange));
    map.insert(std::pair<std::string, int>("kSpring", EColor::kSpring));
    map.insert(std::pair<std::string, int>("kTeal", EColor::kTeal));
    map.insert(std::pair<std::string, int>("kAzure", EColor::kAzure));
    map.insert(std::pair<std::string, int>("kViolet", EColor::kViolet));
    map.insert(std::pair<std::string, int>("kPink", EColor::kPink));

    int result = -1;

    // if the string input can be directly parsed into integer, then return that value.
    bool isInteger = true;
    try {
        result = std::stoi(color);
    }
    catch (const std::exception &ex) {
        isInteger = false;
    }

    if (!isInteger) {
        std::map<std::string, int>::const_iterator iterator = map.find(color.c_str());

        if(iterator != map.end()) {
            result = iterator->second;
        }
    }

    return result;
}
#endif
