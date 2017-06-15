/*
 * utilities related to TDirectoryFile objects.
 */

#include <TFile.h>
#include <TDirectoryFile.h>
#include <TList.h>
#include <TKey.h>
#include <TH1.h>
#include <TTree.h>
#include <TH1D.h>
#include <TGraph.h>
#include <TCanvas.h>

#include <vector>
#include <string>
#include <iostream>

#include "systemUtil.h"

#ifndef FILEUTIL_H_
#define FILEUTIL_H_

int isGoodFile(TFile* file, bool verbose = false);

TList*   getListOfSOMEKeys (TDirectoryFile* dir, std::string pattern);
TList*   getListOfSOMEKeys (TDirectoryFile* dir, std::string pattern, std::string type);
TList*   getListOfGIVENKeys(TDirectoryFile* dir, std::vector<std::string> keyNames);
TList*   getListOfALLKeys (TDirectoryFile* dir);
TList*   getListOfALLKeys (TDirectoryFile* dir, std::string type);
TList*   getListOfALLKeys (TDirectoryFile* dir, std::string type, bool inheritsFrom);
TList*   getListOfMatchedKeys (TDirectoryFile* dir, std::string regex, std::string type, bool inheritsFrom);
TList*   getListOfHistograms     (TDirectoryFile* dir, std::string pattern="");
TList*   getListOfGIVENHistograms(TDirectoryFile* dir, std::vector<std::string> histoNames);
TList*   getListOfALLHistograms(TDirectoryFile* dir);
TList*   getListOfALLCanvases(TDirectoryFile* dir);

std::string getKeyPath(TKey* key);

void     saveAllHistogramsToPicture(TDirectoryFile* dir, std::string fileType="png", std::string directoryToBeSavedIn="", int styleIndex=0, int rebin=1);
void     saveHistogramsToPicture(TDirectoryFile* dir, std::string regex="", std::string fileType="png", std::string directoryToBeSavedIn="", int styleIndex=0, int rebin=1);
void     saveAllGraphsToPicture(TDirectoryFile* dir, std::string fileType="png", std::string directoryToBeSavedIn="", int styleIndex=0);
void     saveGraphsToPicture(TDirectoryFile* dir, std::string regex="", std::string fileType="png", std::string directoryToBeSavedIn="", int styleIndex=0);
void     saveAllCanvasesToPicture(TDirectoryFile* dir, std::string fileType="png", std::string directoryToBeSavedIn="");
void     saveCanvasesToPicture(TDirectoryFile* dir, std::string regex="", std::string fileType="png", std::string directoryToBeSavedIn="");

/*
 * return 0 , if the file is good to read.
 * otherwise, return a nonzero number.
 */
int isGoodFile(TFile* file, bool verbose)
{
    int res = 0;
    if (file->IsZombie()) {
        res = 1;
        if (verbose) std::cout << "File is zombie." << std::endl;
    }
    if (file->TestBits(TFile::kRecovered)) {
        res = TFile::kRecovered;    // TFile::kRecovered == 1024
        if (verbose) std::cout << "File has kRecovered flag." << std::endl;
    }

    return res;
}

/*
 * get list of all keys under a directory "dir" whose name contains "pattern"
 * pattern = "" means any pattern, hence getListOfSOMEKeys(dir, "") is the same as getListOfALLKeys(dir).
 */
TList* getListOfSOMEKeys(TDirectoryFile* dir, std::string pattern)
{
    TList* keysInDir = dir->GetListOfKeys();
    TIter* iter = new TIter(keysInDir);

    TDirectoryFile* subdir = 0;
    TKey*  key = 0;
    TList* keys = new TList();
    TList* newKeys = 0;
    TString keyName = "";

    while ((key=(TKey*)iter->Next())) {

        keyName=key->GetName();
        if(keyName.Contains(pattern.c_str()))
        {
            keys->Add(key);
        }

        // traverse directories in a DFS manner (recursively)
        if(key->IsFolder() && std::string(key->GetClassName()) == "TDirectoryFile")
        {
            subdir = (TDirectoryFile*)key->ReadObj();
            newKeys = getListOfSOMEKeys(subdir, pattern.c_str());
            keys->AddAll(newKeys);
        }
    }
    return keys;
}

/*
 * get list of all keys under a directory "dir" for objects of a given "type" and whose name contains "pattern"
 * type    = "" means any type   , hence getListOfSOMEKeys(dir, pattern, "") is the same as getListOfSOMEKeys(dir, pattern).
 * pattern = "" means any pattern, hence getListOfSOMEKeys(dir, "", "") is the same as getListOfALLKeys(dir).
 */
TList* getListOfSOMEKeys(TDirectoryFile* dir, std::string pattern, std::string type /* ="" */ )
{
    TList* keysInDir = dir->GetListOfKeys();
    TIter* iter = new TIter(keysInDir);

    TDirectoryFile* subdir = 0;
    TKey*  key = 0;
    TList* keys = new TList();
    TList* newKeys = 0;
    TString keyName = "";

    while ((key=(TKey*)iter->Next())) {

        keyName=key->GetName();
        if(keyName.Contains(pattern.c_str()) && std::string(key->GetClassName()) == type.c_str())
        {
            keys->Add(key);
        }

        // traverse directories in a DFS manner (recursively)
        if(key->IsFolder() && std::string(key->GetClassName()) == "TDirectoryFile")
        {
            subdir = (TDirectoryFile*)key->ReadObj();
            newKeys = getListOfSOMEKeys(subdir, pattern, type);
            keys->AddAll(newKeys);
        }
    }
    return keys;
}

/*
 * get list of all keys under a directory "dir" where key names are listed in "keyNames"
 */
TList* getListOfGIVENKeys(TDirectoryFile* dir, std::vector<std::string> keyNames)
{
    TList* keysInDir = dir->GetListOfKeys();
    TIter* iter = new TIter(keysInDir);

    TDirectoryFile* subdir = 0;
    TKey*  key = 0;
    TList* keys = new TList();
    TList* newKeys = 0;
    TString keyName = "";

    while ((key=(TKey*)iter->Next())) {

        keyName=key->GetName();
        for (std::vector<std::string>::iterator itergivenNames = keyNames.begin() ; itergivenNames != keyNames.end(); ++itergivenNames){
            std::string givenName = (std::string)(*itergivenNames);
            if(keyName.EqualTo(givenName.data()))
            {
                keys->Add(key);
            }
        }

        // traverse directories in a DFS manner (recursively)
        if(key->IsFolder() && std::string(key->GetClassName()) == "TDirectoryFile")
        {
            subdir = (TDirectoryFile*)key->ReadObj();
            newKeys = getListOfGIVENKeys(subdir, keyNames);
            keys->AddAll(newKeys);
        }
    }

    return keys;
}

TList* getListOfALLKeys(TDirectoryFile* dir)
{
    return getListOfALLKeys(dir, "");
}

/*
 * get list of all keys under a directory "dir" for objects of a given "type"
 */
TList* getListOfALLKeys(TDirectoryFile* dir, std::string type)
{
    return getListOfALLKeys(dir, type, false);
}

/*
 * get list of all keys under a directory "dir" for objects of a given "type"
 *
 * if inheritsFrom is true,  then get list of keys for objects that inherit from   "type"
 * if inheritsFrom is false, then get list of keys for objects that are exactly of "type"
 */
TList* getListOfALLKeys(TDirectoryFile* dir, std::string type, bool inheritsFrom)
{
    return getListOfMatchedKeys(dir, "", type, inheritsFrom);
}

TList* getListOfMatchedKeys(TDirectoryFile* dir, std::string regex, std::string type, bool inheritsFrom)
{
    TList* keysInDir = dir->GetListOfKeys();
    TIter* iter = new TIter(keysInDir);

    TDirectoryFile* subdir = 0;
    TKey*  key = 0;
    TList* keysOfType = new TList();
    TList* newKeys = 0;

    bool checkType = (type.size() > 0);

    while ((key=(TKey*)iter->Next())) {

        std::string keyPath = getKeyPath(key);

        bool matched = true;
        if (regex.size() > 0)  matched = matchesRegex(keyPath, regex);

        if (matched) {
            if (checkType) {
                if(!inheritsFrom && strcmp(key->GetClassName(), type.c_str()) == 0)  {
                    keysOfType->Add(key);
                }
                else if(inheritsFrom && key->ReadObj()->InheritsFrom(type.c_str()))  {
                    keysOfType->Add(key);
                }
            }
            else {
                keysOfType->Add(key);
            }
        }

        // traverse directories in a DFS manner (recursively)
        if(key->IsFolder() && std::string(key->GetClassName()) == "TDirectoryFile")
        {
            subdir = (TDirectoryFile*)key->ReadObj();
            newKeys = getListOfMatchedKeys(subdir, regex, type, inheritsFrom);
            keysOfType->AddAll(newKeys);
        }
    }

    return keysOfType;
}

/*
 * get list of histograms under a directory "dir" for objects of a given "type"
 * the name of the histograms should contain the pattern
 */
TList* getListOfHistograms(TDirectoryFile* dir, std::string pattern /* ="" */ )
{
    TList* histos=new TList();
    TList* keysHisto = getListOfSOMEKeys(dir, pattern, "TH1D");

    TIter* iter = new TIter(keysHisto);
    TKey*  key = 0;
    while ((key=(TKey*)iter->Next()))
    {
        histos->Add((TH1D*)key->ReadObj());
    }

    return histos;
}

/*
 * get list of histograms under a directory "dir" where histogram names are listed in "histoNames"
 */
TList* getListOfGIVENHistograms(TDirectoryFile* dir, std::vector<std::string> histoNames)
{
    TList* histos=new TList();
    TList* keysHisto = getListOfGIVENKeys(dir, histoNames);

    TIter* iter = new TIter(keysHisto);
    TKey*  key = 0;
    while ((key=(TKey*)iter->Next()))
    {
        histos->Add((TH1D*)key->ReadObj());
    }

    return histos;
}

/*
 * get list of all histograms under a directory "dir" for objects of a given "type"
 */
TList* getListOfALLHistograms(TDirectoryFile* dir)
{
    TList* histos=new TList();
    TList* keysHisto = getListOfALLKeys(dir, "TH1D");

    TIter* iter = new TIter(keysHisto);
    TKey*  key = 0;
    while ((key=(TKey*)iter->Next()))
    {
        histos->Add((TH1D*)key->ReadObj());
    }

    return histos;
}

/*
 * get list of all canvases under a directory "dir" for objects of a given "type"
 */
TList* getListOfALLCanvases(TDirectoryFile* dir)
{
    TList* canvases=new TList();
    TList* keysHisto = getListOfALLKeys(dir, "TCanvas");

    TIter* iter = new TIter(keysHisto);
    TKey*  key = 0;
    while ((key=(TKey*)iter->Next()))
    {
        canvases->Add((TCanvas*)key->ReadObj());
    }

    return canvases;
}

/*
 * return the path of the Key inside file.
 */
std::string getKeyPath(TKey* key)
{
    TDirectory* dir = key->GetMotherDir();
    std::string dirPath = dir->GetPath();

    size_t indexStart = dirPath.find(":/")+2;
    if (indexStart == std::string::npos)  indexStart = 0;
    dirPath = dirPath.substr(indexStart, dirPath.size());

    if (dirPath.size() > 0) {
        return Form("%s/%s", dirPath.c_str(), key->GetName());
    }
    else
        return key->GetName();
}

/*
 * save recursively all the TH1 histograms inside a TDirectoryFile "dir" to images
 */
void saveAllHistogramsToPicture(TDirectoryFile* dir, std::string fileType, std::string directoryToBeSavedIn, int styleIndex, int rebin)
{
    saveHistogramsToPicture(dir, "", fileType, directoryToBeSavedIn, styleIndex, rebin);
}

void saveHistogramsToPicture(TDirectoryFile* dir, std::string regex, std::string fileType, std::string directoryToBeSavedIn, int styleIndex, int rebin)
{
    TList* keysHisto = getListOfMatchedKeys(dir, regex, "TH1", true);  // all histograms that inherit from "TH1" will be saved to picture.

    TH1*  h = 0;
    TKey*  key = 0;
    TIter* iter = new TIter(keysHisto);
    TCanvas* c1=new TCanvas();
    while ((key=(TKey*)iter->Next()))
    {
        h = (TH1*)key->ReadObj();

        if(rebin!=1)
        {
            h->Rebin(rebin);
        }

        if(styleIndex==1)
        {
            h->Draw("E");
        }
        else
        {
            h->Draw();
            if(h->InheritsFrom("TH2"))
            {
                h->SetStats(false);
                h->Draw("COLZ");    // default plot style for TH2 histograms
            }
        }

        if(directoryToBeSavedIn == "")   // save in the current directory if no directory is specified
        {
            c1->SaveAs(Form("%s.%s" ,h->GetName(), fileType.c_str()));  // name of the file is the name of the histogram
        }
        else
        {
            c1->SaveAs(Form("%s/%s.%s", directoryToBeSavedIn.c_str() ,h->GetName(), fileType.c_str()));
        }
    }
    c1->Close();
}

/*
 * save recursively all the graphs inside a TDirectoryFile "dir" to images
 */
void saveAllGraphsToPicture(TDirectoryFile* dir, std::string fileType, std::string directoryToBeSavedIn, int styleIndex)
{
    saveGraphsToPicture(dir, "", fileType, directoryToBeSavedIn, styleIndex);
}

void saveGraphsToPicture(TDirectoryFile* dir, std::string regex, std::string fileType, std::string directoryToBeSavedIn, int styleIndex)
{
    TList* keysGraph = getListOfMatchedKeys(dir, regex, "TGraph", true); // all graphs that inherit from "TGraph" will be saved to picture.

    TGraph* graph = 0;
    TKey*  key = 0;
    TIter* iter = new TIter(keysGraph);
    TCanvas* c1=new TCanvas();
    while ((key=(TKey*)iter->Next()))
    {
        graph = (TGraph*)key->ReadObj();

        if(styleIndex==1)
        {
            graph->Draw();
        }
        else
        {
            graph->Draw("a p");
        }

        if(directoryToBeSavedIn == "")   // save in the current directory if no directory is specified
        {
            c1->SaveAs(Form("%s.%s" ,graph->GetName(), fileType.c_str()));  // name of the file is the name of the histogram
        }
        else
        {
            c1->SaveAs(Form("%s/%s.%s", directoryToBeSavedIn.c_str() ,graph->GetName(), fileType.c_str()));
        }
    }
    c1->Close();
}

void saveAllCanvasesToPicture(TDirectoryFile* dir, std::string fileType, std::string directoryToBeSavedIn)
{
    saveCanvasesToPicture(dir, "", fileType, directoryToBeSavedIn);
}

void saveCanvasesToPicture(TDirectoryFile* dir, std::string regex, std::string fileType, std::string directoryToBeSavedIn)
{
    TList* keysCanvas = getListOfMatchedKeys(dir, regex,"TCanvas", true);  // all canvases that inherit from "TCanvas" will be saved to picture.

    TCanvas* c = new TCanvas();
    TKey*  key = 0;
    TIter* iter = new TIter(keysCanvas);
    while ((key=(TKey*)iter->Next()))
    {
        c = (TCanvas*)key->ReadObj();
        c->Draw();

        if(directoryToBeSavedIn == "")   // save in the current directory if no directory is specified
        {
            c->SaveAs(Form("%s.%s" ,c->GetName(), fileType.c_str()));   // name of the file is the name of the histogram
        }
        else
        {
            c->SaveAs(Form("%s/%s.%s", directoryToBeSavedIn.c_str() ,c->GetName(), fileType.c_str()));
        }
        c->Close();  // close the canvas after each iteration,
                     // otherwise each iteration will have open a new window and
                     // they will not be closed until the code terminates.
    }
    c->Close();
}

#endif /* FILEUTIL_H_ */
