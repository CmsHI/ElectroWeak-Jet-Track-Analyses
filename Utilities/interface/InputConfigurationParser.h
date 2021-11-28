#ifndef UTILITIES_INTERFACE_INPUTCONFIGURATIONPARSER_H_
#define UTILITIES_INTERFACE_INPUTCONFIGURATIONPARSER_H_

#include <TString.h>

#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <utility>      // std::pair

#include "Configuration.h"
#include "ConfigurationParser.h"
#include "CutConfigurationParser.h"
#include "GraphicsConfigurationParser.h"
#include "HiForestInfoController.h"

#include "../eventUtil.h"
#include "../systemUtil.h"

namespace INPUT {

struct ProcessInputs {
    std::vector<int> i;
    std::vector<float> f;
    std::vector<std::string> s;
    std::vector<std::string> str_i;     // string version of the integer input
    std::vector<std::string> str_f;     // string version of the float input

    std::vector<char*> c; // this is a c-string copy of s
};

enum TYPE_I {
    k_collisionType,
    k_doHI,
    k_doHIMC,
    k_doPP,
    k_doPPMC,
    k_doPA,
    k_doPAMC,
    k_isMC,
    k_mode,
    k_drawSame,
    k_drawNormalized,
    k_drawRatio,
    k_drawDiff,
    k_setLogx,
    k_setLogy,
    k_setLogz,
    k_windowWidth,
    k_windowHeight,
    k_textFont,
    k_textAbovePadFont,
    k_legendBorderSize,
    k_lineWidth,
    k_fitTH1,
    k_fitTH1_lowerPad,
    k_centerTitleX,
    k_centerTitleY,
    k_rows,
    k_columns,
    k_mpp_columns,
    k_mpp_rows,
    k_mpp_cover_options,
    k_mpp_l_panel,
    k_mpp_clear_entries,
    k_mpp_draw_sys,
    k_mpp_set_log_scale,
    kN_TYPES_I
};

const std::string TYPE_I_LABELS[kN_TYPES_I] = {
    "collisionType",
    "doHI",
    "doHIMC",
    "doPP",
    "doPPMC",
    "doPA",
    "doPAMC",
    "isMC",
    "mode",             // working behavior of the whole macro
    "drawSame",
    "drawNormalized",
    "drawRatio",        // ratio histograms will be drawn if drawRatio > 0
    "drawDiff",         // difference histograms will be drawn if drawDiff > 0
    "setLogx",
    "setLogy",
    "setLogz",
    "windowWidth",
    "windowHeight",
    "textFont",
    "textAbovePadFont",
    "legendBorderSize",
    "lineWidth",
    "fitTH1",
    "fitTH1_lowerPad",
    "centerTitleX",
    "centerTitleY",
    "rows",
    "columns",
    "mpp_columns",
    "mpp_rows",
    "mpp_cover_options",
    "mpp_l_panel",
    "mpp_clear_entries",
    "mpp_draw_sys",
    "mpp_set_log_scale"
};

enum TYPE_F {
    k_legendOffsetX,
    k_legendOffsetY,
    k_legendWidth,
    k_legendHeight,
    k_legendTextSize,
    k_frameWidth,
    k_frameHeight,
    k_leftMargin,
    k_rightMargin,
    k_bottomMargin,
    k_topMargin,
    k_xMargin,                  // horizontal margin between pads
    k_yMargin,                  // vertical margin between pads
    k_titleSizeX,
    k_titleSizeY,
    k_titleOffsetX,
    k_titleOffsetY,
    k_labelSizeX,
    k_labelSizeY,
    k_labelOffsetX,
    k_labelOffsetY,
    k_TH1_xMin,
    k_TH1_xMax,
    k_TH1_yMin,
    k_TH1_yMax,
    k_TH1_yMin_lowerPad,
    k_TH1_yMax_lowerPad,
    k_binsLogScaleX,
    k_binsLogScaleY,
    k_markerSize,
    k_textSize,
    k_textOffsetX,
    k_textOffsetY,
    k_textLineOffset,
    k_textAbovePadSize,
    k_textAbovePadOffsetX,
    k_textAbovePadOffsetY,
    k_ratioLineY,                // horizontal line at y = ratioLineY
    k_windowHeightFraction,      // height fraction of a pad relative to some other pad
    k_fillAlpha,
    k_mpp_margin,
    k_mpp_edge,
    k_mpp_x_axis_offset,
    k_mpp_y_axis_offset,
    k_mpp_margins,
    k_mpp_title_offsets,
    k_mpp_label_offsets,
    k_mpp_latex_sizes,
    k_mpp_title_sizes,
    k_mpp_label_sizes,
    k_mpp_tick_sizes,
    k_mpp_l_x1,
    k_mpp_l_y1,
    k_mpp_l_x2,
    k_mpp_l_y2,
    k_mpp_i_x,
    k_mpp_i_y,
    k_mpp_y_max,
    k_mpp_y_min,
    kN_TYPES_F
};

const std::string TYPE_F_LABELS[kN_TYPES_F] = {
    "legendOffsetX",
    "legendOffsetY",
    "legendWidth",
    "legendHeight",
    "legendTextSize",
    "frameWidth",
    "frameHeight",
    "leftMargin",
    "rightMargin",
    "bottomMargin",
    "topMargin",
    "xMargin",
    "yMargin",
    "titleSizeX",
    "titleSizeY",
    "titleOffsetX",
    "titleOffsetY",
    "labelSizeX",
    "labelSizeY",
    "labelOffsetX",
    "labelOffsetY",
    "TH1_xMin",
    "TH1_xMax",
    "TH1_yMin",
    "TH1_yMax",
    "TH1_yMin_lowerPad",
    "TH1_yMax_lowerPad",
    "binsLogScaleX",
    "binsLogScaleY",
    "markerSize",
    "textSize",
    "textOffsetX",
    "textOffsetY",
    "textLineOffset",
    "textAbovePadSize",
    "textAbovePadOffsetX",
    "textAbovePadOffsetY",
    "ratioLineY",
    "windowHeightFraction",
    "fillAlpha",
    "mpp_margin",
    "mpp_edge",
    "mpp_x_axis_offset",
    "mpp_y_axis_offset",
    "mpp_margins",
    "mpp_title_offsets",
    "mpp_label_offsets",
    "mpp_latex_sizes",
    "mpp_title_sizes",
    "mpp_label_sizes",
    "mpp_tick_sizes",
    "mpp_l_x1",
    "mpp_l_y1",
    "mpp_l_x2",
    "mpp_l_y2",
    "mpp_i_x",
    "mpp_i_y",
    "mpp_y_max",
    "mpp_y_min"
};

enum TYPE_S {
    k_treePath,
    k_treeFriendPath,
    k_treeFriendPathIndividual,  // ith element in that list is friend only to the ith tree in treePath list
    k_treeFormula,
    k_treeSelection,
    k_treeSelectionBase,        // selection that is applied/valid for all observables
    k_treeSelectionSplitter,    // Ex. treeSelectionSplitter = {hiBin<60, hiBin>=60} means that everything in treeSelection will be split into 0-30% and 30-100%.
    k_TH1_path,             // path to the TH1 object in a file
    k_TH1_title,
    k_TH1_titleX,
    k_TH1_titleY,
    k_TH1_weight,           // weight used when filling a histogram
    k_TH1_scale,
    k_TH1_rebin,
    k_TH1_norm,
    k_TH1_titleY_lowerPad,      // y-axis title for lower pad histograms
    k_drawOption,
    k_TH1D_Bins_List,       // nBins, xLow, xUp for a TH1D histogram
    k_TH2D_Bins_List,       // nBinsx, xLow, xUp, nBinsy, yLow, yUp for a TH2D histogram
    k_TH1error_path,        // path to the TH1 object whose content is bin errors
    k_TH1sysp_path,         // path to the TH1 object whose content is upwards (plus) systematics
    k_TH1sysm_path,         // path to the TH1 object whose content is downwards (minus) systematics
    k_TF1,                  // formula, xMin and xMax for a TF1 function
    k_fitOption,
    k_fitColor,
    k_TF1_lowerPad,                  // formula, xMin and xMax for a TF1 function
    k_fitOption_lowerPad,
    k_fitColor_lowerPad,
    k_legendPosition,       // one of NW, NE, SW, SE
    k_legendEntryLabel,
    k_color,
    k_fillColor,
    k_lineColor,
    k_markerStyle,
    k_lineStyle,
    k_fillStyle,
    k_text,                 // text content of Graphics objects, e.g. : TLatex
    k_textPosition,
    k_textAbovePad,
    k_textAbovePadAlign,
    k_TLine_horizontal,         // y-axis positions of the horizontal lines to be drawn
    k_LineStyle_horizontal,     // styles of the horizontal lines to be drawn
    k_TLine_vertical,           // x-axis positions of the vertical lines to be drawn
    k_LineStyle_vertical,       // styles of the vertical lines to be drawn
    k_TLine_horizontal_lowerPad,         // y-axis positions of the horizontal lines to be drawn in the lower pad
    k_LineStyle_horizontal_lowerPad,     // styles of the horizontal lines to be drawn in the lower pad
    k_TLine_vertical_lowerPad,           // x-axis positions of the vertical lines to be drawn in the lower pad
    k_LineStyle_vertical_lowerPad,       // styles of the vertical lines to be drawn in the lower pad
    k_mpp_x_titles,
    k_mpp_y_titles,
    k_mpp_hist_type,
    k_mpp_plot_type,
    k_mpp_canvas_title,
    k_mpp_custom_info,
    kN_TYPES_S
};

const std::string TYPE_S_LABELS[kN_TYPES_S] = {
    "treePath",
    "treeFriendPath",
    "treeFriendPathIndividual",
    "treeFormula",
    "treeSelection",
    "treeSelectionBase",
    "treeSelectionSplitter",
    "TH1_path",
    "TH1_title",
    "TH1_titleX",
    "TH1_titleY",
    "TH1_weight",
    "TH1_scale",
    "TH1_rebin",
    "TH1_norm",
    "TH1_titleY_lowerPad",
    "drawOption",
    "TH1D_Bins_List",
    "TH2D_Bins_List",
    "TH1error_path",
    "TH1sysp_path",
    "TH1sysm_path",
    "TF1",
    "fitOption",
    "fitColor",
    "TF1_lowerPad",
    "fitOption_lowerPad",
    "fitColor_lowerPad",
    "legendPosition",
    "legendEntryLabel",
    "color",
    "fillColor",
    "lineColor",
    "markerStyle",
    "lineStyle",
    "fillStyle",
    "text",
    "textPosition",
    "textAbovePad",
    "textAbovePadAlign",
    "TLine_horizontal",
    "lineStyle_horizontal",
    "TLine_vertical",
    "lineStyle_vertical",
    "TLine_horizontal_lowerPad",
    "lineStyle_horizontal_lowerPad",
    "TLine_vertical_lowerPad",
    "lineStyle_vertical_lowerPad",
    "mpp_x_titles",
    "mpp_y_titles",
    "mpp_hist_type",
    "mpp_plot_type",
    "mpp_canvas_title",
    "mpp_custom_info"
};

enum PROCESS {
    kSKIM,
    kCORRECTION,
    kHISTOGRAM,
    kPLOTTING,
    kPERFORMANCE,
    kN_PROCESSES // must come last in enum
};

std::string PROCESS_LABELS[kN_PROCESSES] = {
    "skim",
    "correction",
    "histogram",
    "plotting",
    "performance"
};

};

namespace INPUT_TH1 {

enum TYPE_NORM {
    k_noNorm,              // no normalization, just a place holder
    k_normInt,             // normalization by the total number of entries, histogram integral will be 1.
    k_normEvents,          // normalization by the number of events that pass full selection
    k_normEventsBaseSel,   // normalization by the number of events that pass base selection
    kN_TYPE_NORM           // must come last in enum
};

enum TYPE_DRAWSAME {
    k_noDrawSame,         // do not draw same, just a place holder
    k_drawSame,           // draw everything together
    k_drawSameAcrossSplits,     // draw histogram 1 from split bins 1,2,3, ... same, draw histogram 2 from split bins 1,2,3, ... same, ...
    k_drawSameInsideSplits,     // draw histogram 1,2,3, ... from split bin 1 same, draw histogram 1,2,3, ... from split bin 2 same, ...
    kN_TYPE_DRAWSAME           // must come last in enum
};

};

namespace INPUT_MODE {

enum TYPE_MODE {
    k_noMode,         // no mode, just a place holder
    k_comparison,
    kN_TYPE_MODE      // must come last in enum
};

};

namespace INPUT_DEFAULT {

const int drawNormalized = INPUT_TH1::k_noNorm;
const int windowWidth = 600;
const int windowHeight = 600;
const int textFont = 43;
const int textAbovePadFont = 43;
const int lineWidth = 1;
const int fitColor = EColor::kRed;
const int rows = 1;
const int columns = 1;

const float frameWidth = 0.8;
const float frameHeight = 0.8;
const float leftMargin = 0.1;
const float rightMargin = 0.1;
const float bottomMargin = 0.1;
const float topMargin = 0.1;
const float xMargin = 0.01;
const float yMargin = 0.01;
const float titleOffsetX = 1.25;
const float titleOffsetY = 1.75;
const float markerSize = 1;
const float textSize = 20;
const float textOffsetX = 0.04;
const float textOffsetY = 0.04;
const float textLineOffset = 0.05;
const float textAbovePadSize = 20;
const float textAbovePadOffsetX = 0;
const float textAbovePadOffsetY = 0;
const float xMin = 0.001;   // for logScale histograms
const float yMin = 0.001;   // for logScale histograms
const float windowHeightFraction = 0.25;

const std::string TH1_weight = "1";
const std::string fitOption = "R L M";
const std::string textPosition = "NE";
};

struct InputConfiguration : public Configuration {
    INPUT::ProcessInputs proc[CUTS::kN_PROCESSES];

};

class InputConfigurationParser : public ConfigurationParser {

  public:
    InputConfigurationParser() {};
    ~InputConfigurationParser() {};

    static bool isROOTfile(TString fileName);
    static bool isROOTfile(std::string fileName);
    static bool isListFile(TString fileName);
    static bool isListFile(std::string fileName);
    static bool isConfigurationFile(TString fileName);
    static bool isConfigurationFile(std::string fileName);
    static std::vector<std::string> replaceMntWithXrootd(std::vector<std::string> & fileNames);
    static std::vector<std::string> replaceXrootdWithMnt(std::vector<std::string> & fileNames);
    static std::vector<std::string> replaceMntT2USMIT(std::vector<std::string> & fileNames);
    static std::vector<std::string> ParseFiles(std::string fileName);
    static std::vector<std::string> ParseEvents(std::string fileName);
    static std::vector<std::string> ParseFileArgument(std::string fileArgument);
    static void copyConfiguration(InputConfiguration& config, InputConfiguration configCopy);
    static void replaceKeyWords(InputConfiguration& config, std::string dataFile);
    static std::vector<std::string> parseKeyWords(std::string dataFile);
    static InputConfiguration Parse(std::string inFile);
};

bool InputConfigurationParser::isROOTfile(TString fileName) {
    return fileName.EndsWith(".root");
}

bool InputConfigurationParser::isROOTfile(std::string fileName) {
    TString tstr = fileName.c_str();
    return isROOTfile(tstr);
}

bool InputConfigurationParser::isListFile(TString fileName) {
    return (fileName.EndsWith(".txt") || fileName.EndsWith(".list"));
}

bool InputConfigurationParser::isListFile(std::string fileName) {
    TString tstr = fileName.c_str();
    return isListFile(tstr);
}

bool InputConfigurationParser::isConfigurationFile(TString fileName) {
    return fileName.EndsWith(".conf");
}

bool InputConfigurationParser::isConfigurationFile(std::string fileName) {
    TString tstr = fileName.c_str();
    return isConfigurationFile(tstr);
}

std::vector<std::string> InputConfigurationParser::replaceMntWithXrootd(std::vector<std::string> & fileNames)
{
    std::vector<std::string> res;

    for (std::vector<std::string>::const_iterator it = fileNames.begin(); it != fileNames.end(); ++it) {
        std::string resTmp = replaceAll((*it), "/mnt/hadoop/cms/store", "root://xrootd.cmsaf.mit.edu//store");
        resTmp = replaceAll(resTmp, "/mnt/T2_US_MIT/hadoop/cms/store", "root://xrootd.cmsaf.mit.edu//store");
        res.push_back(resTmp);
    }

    return res;
}

std::vector<std::string> InputConfigurationParser::replaceXrootdWithMnt(std::vector<std::string> & fileNames)
{
    std::vector<std::string> res;

    for (std::vector<std::string>::const_iterator it = fileNames.begin(); it != fileNames.end(); ++it) {
        res.push_back(replaceAll((*it), "root://xrootd.cmsaf.mit.edu//store", "/mnt/hadoop/cms/store"));
    }

    return res;
}

/*
 * replace the "/mnt/T2_US_MIT/hadoop" path with "/mnt/hadoop"
 */
std::vector<std::string> InputConfigurationParser::replaceMntT2USMIT(std::vector<std::string> & fileNames)
{
    std::vector<std::string> res;

    for (std::vector<std::string>::const_iterator it = fileNames.begin(); it != fileNames.end(); ++it) {
        res.push_back(replaceAll((*it), "/mnt/T2_US_MIT/hadoop/cms/store", "/mnt/hadoop/cms/store"));
    }

    return res;
}

std::vector<std::string> InputConfigurationParser::ParseFiles(std::string fileName) {
    std::vector<std::string> fileNames;

    if (isROOTfile(fileName)) {
        fileNames.push_back(fileName);
    } else if (isListFile(fileName)) {

        // assumes there is exactly one file per line,
        // no empty line should be entered.
        // any line containing a "#" will be skipped.
        std::ifstream inFile(fileName.c_str());
        std::string strLine;
        if (inFile.is_open()) {
            while (getline(inFile,strLine)) {
                if (trim(strLine).find(CONFIGPARSER::comment.c_str()) == 0) continue;  //skip all lines starting with comment sign #

                size_t posLast = strLine.find(CONFIGPARSER::comment.c_str());    // allow inline comment signs with #
                std::string in = trim(strLine.substr(0, posLast));
                in = ConfigurationParser::substituteEnv(in);
                // assume this line contains a ROOT file
                fileNames.push_back(in);
            }
        }
    } else if (isConfigurationFile(fileName)) {
        // assumes there is exactly one file per line,
        // no empty line should be entered.
        // any line containing a "#" will be skipped.
        std::ifstream inFile(fileName.c_str());
        std::string strLine;
        if (inFile.is_open()) {
            bool fileListFound = false;
            std::string startSignal = "#FILELIST#";
            std::string endSignal = "#FILELIST-END#";
            while (getline(inFile,strLine)) {
                if (strLine.find(startSignal) != std::string::npos) fileListFound = true;
                else if (strLine.find(endSignal) != std::string::npos) break;

                if (!fileListFound) continue;
                if (trim(strLine).find(CONFIGPARSER::comment.c_str()) == 0) continue;  //skip all lines starting with comment sign #

                size_t posLast = strLine.find(CONFIGPARSER::comment.c_str());    // allow inline comment signs with #
                std::string in = trim(strLine.substr(0, posLast));
                in = ConfigurationParser::substituteEnv(in);
                // assume this line contains a ROOT file
                fileNames.push_back(in);
            }
        }
    }

    std::string hostName = getHostName();
    if (matchesWildCard(hostName, "submit*.mit.edu") ||
        matchesWildCard(hostName, "lxplus*.cern.ch")) {

        fileNames = InputConfigurationParser::replaceMntWithXrootd(fileNames);
    }
    else if (matchesWildCard(hostName, "*cmsaf.mit.edu")) {

        fileNames = InputConfigurationParser::replaceXrootdWithMnt(fileNames);
        fileNames = InputConfigurationParser::replaceMntT2USMIT(fileNames);
    }

    return fileNames;
}

std::vector<std::string> InputConfigurationParser::ParseEvents(std::string fileName) {
    std::vector<std::string> eventList;

    if (isListFile(fileName)) {

        // assumes there is exactly one event info per line,
        // no empty line should be entered.
        // any line containing a "#" will be skipped.
        std::ifstream inFile(fileName.c_str());
        std::string strLine;
        if (inFile.is_open()) {
            while (getline(inFile,strLine)) {
                if (trim(strLine).find(CONFIGPARSER::comment.c_str()) == 0) continue;  //skip all lines starting with comment sign #

                size_t posLast = strLine.find(CONFIGPARSER::comment.c_str());    // allow inline comment signs with #
                std::string in = trim(strLine.substr(0, posLast));
                // assume this line contains event info
                eventList.push_back(in);
            }
        }
    } else if (isConfigurationFile(fileName)) {
        // assumes there is exactly one event info per line,
        // no empty line should be entered.
        // any line containing a "#" will be skipped.
        std::ifstream inFile(fileName.c_str());
        std::string strLine;
        if (inFile.is_open()) {
            bool eventListFound = false;
            std::string startSignal = "#EVENTLIST#";
            std::string endSignal = "#EVENTLIST-END#";
            while (getline(inFile,strLine)) {
                if (strLine.find(startSignal) != std::string::npos) eventListFound = true;
                else if (strLine.find(endSignal) != std::string::npos) break;

                if (!eventListFound) continue;
                if (trim(strLine).find(CONFIGPARSER::comment.c_str()) == 0) continue;  //skip all lines starting with comment sign #

                size_t posLast = strLine.find(CONFIGPARSER::comment.c_str());    // allow inline comment signs with #
                std::string in = trim(strLine.substr(0, posLast));
                // assume this line contains event info
                eventList.push_back(in);
            }
        }
    }

    return eventList;
}

std::vector<std::string> InputConfigurationParser::ParseFileArgument(std::string fileArgument) {
    std::vector<std::string> fileArguments = ParseListWithoutBracket(fileArgument);

    return fileArguments;
}

/*
 * copy values from "configCopy" to "config".
 * if the original config has a value for a field, then do not copy.
 */
void InputConfigurationParser::copyConfiguration(InputConfiguration& config, InputConfiguration configCopy) {
    for (int i = 0 ; i < INPUT::kN_PROCESSES; ++i) {
        for (int j = 0 ; j < INPUT::kN_TYPES_I; ++j) {
            if (config.proc[i].i[j] == 0) {
                config.proc[i].i[j] = configCopy.proc[i].i[j];
                config.proc[i].str_i[j] = configCopy.proc[i].str_i[j];
            }
        }
        for (int j = 0 ; j < INPUT::kN_TYPES_F; ++j) {
            if (config.proc[i].f[j] == 0) {
                config.proc[i].f[j] = configCopy.proc[i].f[j];
                config.proc[i].str_f[j] = configCopy.proc[i].str_f[j];
            }
        }
        for (int j = 0 ; j < INPUT::kN_TYPES_S; ++j) {
            if (config.proc[i].s[j].size() == 0) {
                config.proc[i].s[j] = configCopy.proc[i].s[j];
                char * cstr = new char [config.proc[i].s[j].length()+1];
                std::strcpy(cstr, config.proc[i].s[j].c_str());
                config.proc[i].c[j] = cstr;
            }
        }
    }
}

void InputConfigurationParser::replaceKeyWords(InputConfiguration& config, std::string dataFile)
{
    std::vector<std::string> parsedKeyWords = InputConfigurationParser::parseKeyWords(dataFile);

    for (int iKey = 0; iKey < CONFIGPARSER::kN_KEYWORDS; ++iKey)
    {
        if (parsedKeyWords[iKey].size() == 0)  continue;

        for (int i = 0 ; i < INPUT::kN_PROCESSES; ++i) {
            for (int j = 0 ; j < INPUT::kN_TYPES_I; ++j) {
                config.proc[i].str_i[j] =
                        replaceAll(config.proc[i].str_i[j], CONFIGPARSER::KW_LABELS[iKey], parsedKeyWords[iKey]);
            }

            for (int j = 0 ; j < INPUT::kN_TYPES_F; ++j) {
                config.proc[i].str_f[j] =
                        replaceAll(config.proc[i].str_f[j], CONFIGPARSER::KW_LABELS[iKey], parsedKeyWords[iKey]);
            }

            for (int j = 0 ; j < INPUT::kN_TYPES_S; ++j) {
                config.proc[i].s[j] =
                        replaceAll(config.proc[i].s[j], CONFIGPARSER::KW_LABELS[iKey], parsedKeyWords[iKey]);
                char * cstr = new char [config.proc[i].s[j].length()+1];
                std::strcpy(cstr, config.proc[i].s[j].c_str());
                config.proc[i].c[j] = cstr;
            }
        }
    }
}

std::vector<std::string> InputConfigurationParser::parseKeyWords(std::string dataFile)
{
    std::vector<std::string> dataFiles = InputConfigurationParser::ParseFiles(dataFile);
    if (dataFiles.size() == 0) return {};

    // use the information from first file only
    std::vector<std::string> argsStr = {dataFiles[0]};
    std::vector<int> argsInt = {HiForestInfoController::getCollisionType(dataFiles[0])};

    return ConfigurationParser::ParseKeyWords(argsStr, argsInt);
}

InputConfiguration InputConfigurationParser::Parse(std::string inFile) {
    InputConfiguration config;
    std::string endSignal = "#INPUT-END#";     // signals that input configuration parsing is to be terminated.
    // another block of configuration parsing will start.

    for (int i = 0 ; i < INPUT::kN_PROCESSES; ++i) {
        config.proc[i].i.resize(INPUT::kN_TYPES_I);
        config.proc[i].f.resize(INPUT::kN_TYPES_F);
        config.proc[i].s.resize(INPUT::kN_TYPES_S);
        config.proc[i].str_i.resize(INPUT::kN_TYPES_I);
        config.proc[i].str_f.resize(INPUT::kN_TYPES_F);
        config.proc[i].c.resize(INPUT::kN_TYPES_S);
    }

    std::ifstream fin(inFile);
    if ((fin.rdstate() & std::ifstream::failbit) != 0) {
        std::cout << "I/O Error opening configuration file." << std::endl;
        config.isValid = false;
        return config;
    }

    std::map<std::string, std::string> mapVarString;    // map of variables of type string

    std::string line;
    unsigned int lineCounter = 0;
    while (getline(fin, line)) {
        lineCounter++;
        if (line.find(endSignal) != std::string::npos) break;
        bool isCommand = ConfigurationParser::isCommand(line);
        if (line.find("input") == std::string::npos && !isCommand) continue; //skip all lines without an "input"
        if (line.find("=") == std::string::npos) continue; //skip all lines without an =
        if (line.find(".") == std::string::npos) continue; //skip all lines without a dot
        if (ConfigurationParser::isComment(line)) continue;  //skip all lines starting with comment sign #
        size_t pos = line.find("=") + 1;
        size_t posLast = line.find(CONFIGPARSER::comment.c_str());    // allow inline comment signs with #
        std::string value = ConfigurationParser::ReadValue(fin, line.substr(pos, (posLast-pos)));
        value = ConfigurationParser::substituteVarString(value, mapVarString);
        value = ConfigurationParser::substituteEnv(value);
        value = getExpandedEnvironmentVariable(value);
        std::istringstream sin(value);
        line = line.substr(0, pos-1);        // "line" becomes the LHS of the "=" sign (excluing the "=" sign)
        if (isCommand) {
            if (ConfigurationParser::isVarDefinitionString(line)) {
                ConfigurationParser::insertVarString(ConfigurationParser::ParseVarDefinitionString(line, value), mapVarString);
            } else if (ConfigurationParser::isImportInputStatement(line)) {
                InputConfiguration importedConfig = InputConfigurationParser::Parse(value);
                InputConfigurationParser::copyConfiguration(config, importedConfig);
            }
            continue;
        }
        bool success = false;
        INPUT::PROCESS proc = INPUT::kN_PROCESSES;
        for (int i = 0; i < INPUT::kN_PROCESSES; ++i) {

            std::string label_proc = Form(".%s.",INPUT::PROCESS_LABELS[i].c_str());
            if (line.find(label_proc) != std::string::npos) {
                proc = (INPUT::PROCESS)i;
                break;
            }
        }

        for (int j = 0; j < INPUT::kN_TYPES_I; ++j) {
            std::string label = Form(".%s ",INPUT::TYPE_I_LABELS[j].c_str());    // prevent substring matching, e.g. : "doPP" and "doPPMC"
            if (line.find(label) != std::string::npos) {
                int in;
                sin >> in;
                config.proc[proc].i[j] = in;
                config.proc[proc].str_i[j] = trim(sin.str());
                success = true;
                break;
            }
        }

        for (int j = 0; j < INPUT::kN_TYPES_F; ++j) {
            std::string label = Form(".%s ",INPUT::TYPE_F_LABELS[j].c_str());    // prevent substring matching, e.g. : "doPP" and "doPPMC"
            if (line.find(label) != std::string::npos) {
                float in;
                sin >> in;
                config.proc[proc].f[j] = in;
                config.proc[proc].str_f[j] = trim(sin.str());
                success = true;
                break;
            }
        }

        for (int j = 0; j < INPUT::kN_TYPES_S; ++j) {
            std::string label = Form(".%s ",INPUT::TYPE_S_LABELS[j].c_str());    // prevent substring matching, e.g. : "doPP" and "doPPMC"
            if (line.find(label) != std::string::npos) {
                std::string in;
                in = trim(sin.str());   // stringstream ignores characters after a whitespace, use the original string to read the value.
                config.proc[proc].s[j] = in;
                char * cstr = new char [in.length()+1];
                std::strcpy(cstr, in.c_str());
                config.proc[proc].c[j] = cstr;
                success = true;
                break;
            }
        }

        if (!success) {
            std::cout << "Malformed line in configuration." << std::endl;
            std::cout << "Line #" << lineCounter << " : " << std::endl;
            std::cout << line << std::endl;
            config.isValid = false;
            return config;
        }
    }
    config.isValid = true;
    return config;
}

#endif
