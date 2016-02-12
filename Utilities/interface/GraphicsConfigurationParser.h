#ifndef UTILITIES_INTERFACE_GRAPHICSCONFIGURATIONPARSER_H_
#define UTILITIES_INTERFACE_GRAPHICSCONFIGURATIONPARSER_H_

#include <TAttMarker.h>
#include <TAttLine.h>
#include <TAttFill.h>
#include <TAttText.h>
#include <Rtypes.h>

#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
#include <map>
#include <utility>      // std::pair

#include "ConfigurationParser.h"

namespace GRAPHICS {

// assume not more than 13 colors are needed.
const int colors[13] = {kBlack, kBlue, kRed,   kOrange,  kViolet, kCyan, kSpring, kYellow,
        kAzure, kPink, kGreen, kMagenta, kTeal};

// default values
const int markerStyle = EMarkerStyle::kFullCircle;
const int lineStyle = ELineStyle::kSolid;
const int fillStyle = EFillStyle::kFSolid;
const int textAlign = 11;       // horizontal : left align, vertical : bottom align
const int lineStyle_horizontal = ELineStyle::kDashed;
const int lineStyle_vertical = ELineStyle::kDashed;
};

class GraphicsConfigurationParser : public ConfigurationParser {

public:
    GraphicsConfigurationParser(){};
    ~GraphicsConfigurationParser(){};

    static int ParseFromMap(std::map<std::string, int> map, std::string str);

    static int ParseMarkerStyle(std::string markerStyle);
    static int ParseLineStyle(std::string lineStyle);
    static int ParseFillStyle(std::string fillStyle);
    static int ParseColor(std::string color);
    static int ParseTextAlign(std::string textAlign);
};

/*
 * with this function, the user can specify the relevant property both using the integer and string value.
 * Example :
 * ParseFromMap(mapMarkerStyle, "kFullSquare")  returns 21
 * ParseFromMap(mapMarkerStyle, "21")           returns 21
 *
 * parsing of the strings is not in "ignore case" mode
 * Example :
 * ParseFromMap(mapMarkerStyle, "kfullSquAre")  returns -1
 */
int GraphicsConfigurationParser::ParseFromMap(std::map<std::string, int> map, std::string str)
{
    int result = -1;

    // if the string input can be directly parsed into integer, then return that value.
    bool isInteger = true;
    try {
        result = std::stoi(str);
    }
    catch (const std::exception &ex) {
        isInteger = false;
    }

    if (!isInteger) {
        std::map<std::string, int>::const_iterator iterator = map.find(str.c_str());

        if(iterator != map.end()) {
            result = iterator->second;
        }
    }

    return result;
}

int GraphicsConfigurationParser::ParseMarkerStyle(std::string markerStyle)
{
    // https://root.cern.ch/doc/master/TAttMarker_8h.html#a8e27ac630ae56999f0e087dcfbca0619
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

    return ParseFromMap(map, markerStyle);
}

int GraphicsConfigurationParser::ParseLineStyle(std::string lineStyle)
{
    // https://root.cern.ch/doc/master/TAttLine_8h.html#a7092c0c4616367016b70d54e5c680a69
    std::map<std::string, int> map;
    map.insert(std::pair<std::string, int>("kSolid", ELineStyle::kSolid));
    map.insert(std::pair<std::string, int>("kDashed", ELineStyle::kDashed));
    map.insert(std::pair<std::string, int>("kDotted", ELineStyle::kDotted));
    map.insert(std::pair<std::string, int>("kDashDotted", ELineStyle::kDashDotted));

    return ParseFromMap(map, lineStyle);
}

int GraphicsConfigurationParser::ParseFillStyle(std::string fillStyle)
{
    // https://root.cern.ch/doc/master/TAttFill_8h.html#ac40bc38f3700fabd4156d6e66312162f
    std::map<std::string, int> map;
    map.insert(std::pair<std::string, int>("kFDotted1", EFillStyle::kFDotted1));
    map.insert(std::pair<std::string, int>("kFDotted2", EFillStyle::kFDotted2));
    map.insert(std::pair<std::string, int>("kFDotted3", EFillStyle::kFDotted3));
    map.insert(std::pair<std::string, int>("kFHatched1", EFillStyle::kFHatched1));
    map.insert(std::pair<std::string, int>("kHatched2", EFillStyle::kHatched2));
    map.insert(std::pair<std::string, int>("kFHatched3", EFillStyle::kFHatched3));
    map.insert(std::pair<std::string, int>("kFHatched4", EFillStyle::kFHatched4));
    map.insert(std::pair<std::string, int>("kFWicker", EFillStyle::kFWicker));
    map.insert(std::pair<std::string, int>("kFScales", EFillStyle::kFScales));
    map.insert(std::pair<std::string, int>("kFBricks", EFillStyle::kFBricks));
    map.insert(std::pair<std::string, int>("kFSnowflakes", EFillStyle::kFSnowflakes));
    map.insert(std::pair<std::string, int>("kFCircles", EFillStyle::kFCircles));
    map.insert(std::pair<std::string, int>("kFTiles", EFillStyle::kFTiles));
    map.insert(std::pair<std::string, int>("kFMondrian", EFillStyle::kFMondrian));
    map.insert(std::pair<std::string, int>("kFDiamonds", EFillStyle::kFDiamonds));
    map.insert(std::pair<std::string, int>("kFWaves1", EFillStyle::kFWaves1));
    map.insert(std::pair<std::string, int>("kFDashed1", EFillStyle::kFDashed1));
    map.insert(std::pair<std::string, int>("kFDashed2", EFillStyle::kFDashed2));
    map.insert(std::pair<std::string, int>("kFAlhambra", EFillStyle::kFAlhambra));
    map.insert(std::pair<std::string, int>("kFWaves2", EFillStyle::kFWaves2));
    map.insert(std::pair<std::string, int>("kFStars1", EFillStyle::kFStars1));
    map.insert(std::pair<std::string, int>("kFStars2", EFillStyle::kFStars2));
    map.insert(std::pair<std::string, int>("kFPyramids", EFillStyle::kFPyramids));
    map.insert(std::pair<std::string, int>("kFFrieze", EFillStyle::kFFrieze));
    map.insert(std::pair<std::string, int>("kFMetopes", EFillStyle::kFMetopes));
    map.insert(std::pair<std::string, int>("kFEmpty", EFillStyle::kFEmpty));
    map.insert(std::pair<std::string, int>("kFSolid", EFillStyle::kFSolid));

    return ParseFromMap(map, fillStyle);
}

int GraphicsConfigurationParser::ParseColor(std::string color)
{
    // https://root.cern.ch/doc/master/Rtypes_8h.html#ac31db05c6cb5891c704eae374f6926a8
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

    return ParseFromMap(map, color);
}

int GraphicsConfigurationParser::ParseTextAlign(std::string textAlign)
{
    // https://root.cern.ch/doc/master/TAttText_8h.html#aa442da9b78fc8d86e5445d27833587db
    std::map<std::string, int> map;
    map.insert(std::pair<std::string, int>("kHAlignLeft", ETextAlign::kHAlignLeft));
    map.insert(std::pair<std::string, int>("kHAlignCenter", ETextAlign::kHAlignCenter));
    map.insert(std::pair<std::string, int>("kHAlignRight", ETextAlign::kHAlignRight));
    map.insert(std::pair<std::string, int>("kVAlignBottom", ETextAlign::kVAlignBottom));
    map.insert(std::pair<std::string, int>("kVAlignCenter", ETextAlign::kVAlignCenter));
    map.insert(std::pair<std::string, int>("kVAlignTop", ETextAlign::kVAlignTop));

    return ParseFromMap(map, textAlign);
}

#endif
