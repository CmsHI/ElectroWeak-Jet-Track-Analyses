/*
 * utilities related to TDirectoryFile objects.
 */

#include <TDirectoryFile.h>
#include <TList.h>
#include <TKey.h>
#include <TH1.h>
#include <TTree.h>
#include <TH1D.h>
#include <TCanvas.h>

#include <vector>
#include <string>

#ifndef FILEUTIL_H_
#define FILEUTIL_H_

TList*   getListOfSOMEKeys (TDirectoryFile* dir, std::string pattern);
TList*   getListOfSOMEKeys (TDirectoryFile* dir, std::string pattern, std::string type);
TList*   getListOfGIVENKeys(TDirectoryFile* dir, std::vector<std::string> keyNames);
TList*   getListOfALLKeys (TDirectoryFile* dir);
TList*   getListOfALLKeys (TDirectoryFile* dir, std::string type);
TList*   getListOfALLKeys (TDirectoryFile* dir, std::string type, bool inheritsFrom);
TList*   getListOfHistograms     (TDirectoryFile* dir, std::string pattern="");
TList*   getListOfGIVENHistograms(TDirectoryFile* dir, std::vector<std::string> histoNames);
TList*   getListOfALLHistograms(TDirectoryFile* dir);
TList*   getListOfALLCanvases(TDirectoryFile* dir);

void     saveAllHistogramsToPicture(TDirectoryFile* dir, std::string fileType="png", std::string directoryToBeSavedIn="", int styleIndex=0, int rebin=1);
void     saveAllGraphsToPicture(TDirectoryFile* dir, std::string fileType="png", std::string directoryToBeSavedIn="", int styleIndex=0);
void     saveAllCanvasesToPicture(TDirectoryFile* dir, std::string fileType="png", std::string directoryToBeSavedIn="");

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
    TList* keys=new TList();
    TList* newKeys=new TList();
    TString keyName = "";

    while ((key=(TKey*)iter->Next())) {

        keyName=key->GetName();
        if(keyName.Contains(pattern.c_str()))
        {
            keys->Add(key);
        }

        // traverse directories in a DFS manner (recursively)
        if(key->IsFolder())
        {
            subdir=(TDirectoryFile*)key->ReadObj();
            newKeys=getListOfSOMEKeys(subdir, pattern.c_str());
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
    TList* keys=new TList();
    TList* newKeys=new TList();
    TString keyName = "";

    while ((key=(TKey*)iter->Next())) {

        keyName=key->GetName();
        if(keyName.Contains(pattern.c_str()) && strcmp(key->GetClassName(), type.c_str()) == 0)
        {
            keys->Add(key);
        }

        // traverse directories in a DFS manner (recursively)
        if(key->IsFolder())
        {
            subdir=(TDirectoryFile*)key->ReadObj();
            newKeys=getListOfSOMEKeys(subdir, pattern, type);
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
    TList* keys=new TList();
    TList* newKeys=new TList();
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
        if(key->IsFolder())
        {
            subdir=(TDirectoryFile*)key->ReadObj();
            newKeys=getListOfGIVENKeys(subdir, keyNames);
            keys->AddAll(newKeys);
        }
    }

    return keys;
}

TList* getListOfALLKeys(TDirectoryFile* dir)
{
    TList* keysInDir = dir->GetListOfKeys();
    TIter* iter = new TIter(keysInDir);

    TDirectoryFile* subdir = 0;
    TKey*  key = 0;
    TList* keys=new TList();
    TList *newKeys=new TList();

    while ((key=(TKey*)iter->Next())) {

        keys->Add(key);

        // traverse directories in a DFS manner (recursively)
        if(key->IsFolder())
        {
            subdir=(TDirectoryFile*)key->ReadObj();
            newKeys=getListOfALLKeys(subdir);
            keys->AddAll(newKeys);
        }
    }
    return keys;
}

/*
 * get list of all keys under a directory "dir" for objects of a given "type"
 */
TList* getListOfALLKeys(TDirectoryFile* dir, std::string type)
{
    TList* keysInDir = dir->GetListOfKeys();
    TIter* iter = new TIter(keysInDir);

    TDirectoryFile* subdir = 0;
    TKey*  key = 0;
    TList* keysOfType=new TList();
    TList *newKeys=new TList();

    while ((key=(TKey*)iter->Next())) {

        //      http://www.cplusplus.com/reference/cstring/strcmp/
        if(strcmp(key->GetClassName(), type.c_str()) == 0)
        {
            keysOfType->Add(key);
        }

        // traverse directories in a DFS manner (recursively)
        if(key->IsFolder())
        {
            subdir=(TDirectoryFile*)key->ReadObj();
            newKeys=getListOfALLKeys(subdir, type);
            keysOfType->AddAll(newKeys);
        }
    }

    return keysOfType;
}

/*
 * get list of all keys under a directory "dir" for objects of a given "type"
 *
 * if inheritsFrom is true,  then get list of keys for objects that inherit from   "type"
 * if inheritsFrom is false, then get list of keys for objects that are exactly of "type"
 */
TList* getListOfALLKeys(TDirectoryFile* dir, std::string type, bool inheritsFrom)
{
    if(!inheritsFrom)
        return getListOfALLKeys(dir, type);

    TList* keysInDir = dir->GetListOfKeys();
    TIter* iter = new TIter(keysInDir);

    TDirectoryFile* subdir = 0;
    TKey*  key = 0;
    TList* keysOfType=new TList();
    TList *newKeys=new TList();

    while ((key=(TKey*)iter->Next())) {

        if(key->ReadObj()->InheritsFrom(type.c_str()))
        {
            keysOfType->Add(key);
        }

        // traverse directories in a DFS manner (recursively)
        if(key->IsFolder())
        {
            subdir=(TDirectoryFile*)key->ReadObj();
            newKeys=getListOfALLKeys(subdir, type);
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
 * save recursively all the TH1 histograms inside a TDirectoryFile "dir" to images
 */
void saveAllHistogramsToPicture(TDirectoryFile* dir, std::string fileType /* ="gif" */, std::string directoryToBeSavedIn /* ="" */, int styleIndex /* =0 */, int rebin /* =1 */)
{
    TList* keysHisto = getListOfALLKeys(dir, "TH1", true);  // all histograms that inherit from "TH1" will be saved to picture.

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
void saveAllGraphsToPicture(TDirectoryFile* dir, std::string fileType /* ="gif" */, std::string directoryToBeSavedIn /* ="" */, int styleIndex /* =0 */)
{
    TList* keysGraph = getListOfALLKeys(dir, "TGraph", true); // all graphs that inherit from "TGraph" will be saved to picture.

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

void saveAllCanvasesToPicture(TDirectoryFile* dir, std::string fileType /* ="gif" */, std::string directoryToBeSavedIn /* ="" */)
{
    TList* keysCanvas = getListOfALLKeys(dir, "TCanvas", true);  // all canvases that inherit from "TCanvas" will be saved to picture.

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
