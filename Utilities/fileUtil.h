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

/*
 * get list of all keys under a directory "dir" whose name contains "pattern"
 * pattern = "" means any pattern, hence getListOfSOMEKeys(dir, "") is the same as getListOfALLKeys(dir).
 */
TList* getListOfSOMEKeys(TDirectoryFile* dir, std::string pattern)
{
    TList* keysInDir = dir->GetListOfKeys();
    TIter* iter = new TIter(keysInDir);

    TDirectoryFile *subdir;
    TKey*  key;
    TList* keys=new TList();
    TList* newKeys=new TList();
    TString keyName;

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

    TDirectoryFile *subdir;
    TKey*  key;
    TList* keys=new TList();
    TList* newKeys=new TList();
    TString keyName;

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

    TDirectoryFile *subdir;
    TKey*  key;
    TList* keys=new TList();
    TList* newKeys=new TList();
    TString keyName;


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

    TDirectoryFile *subdir;
    TKey*  key;
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

    TDirectoryFile *subdir;
    TKey*  key;
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

    TDirectoryFile *subdir;
    TKey*  key;
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
    TKey*  key;
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
    TKey*  key;
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
    TKey*  key;
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
    TKey*  key;
    while ((key=(TKey*)iter->Next()))
    {
        canvases->Add((TCanvas*)key->ReadObj());
    }

    return canvases;
}

#endif /* FILEUTIL_H_ */
