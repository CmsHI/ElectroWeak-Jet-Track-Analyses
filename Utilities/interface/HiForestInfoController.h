/*
 *  class to read and evaluate HiForest production info, works on tree path HiForest/HiForestInfo
 */

#ifndef HIFORESTINFOCONTROLLER_H_
#define HIFORESTINFOCONTROLLER_H_

#include <TTree.h>
#include <TFile.h>
#include <TDirectoryFile.h>

#include <string>
#include <iostream>

#include "../eventUtil.h"

const int CHARARRAYSIZE = 50;

class HiForestInfoController {
public:
    HiForestInfoController(){
        tree = 0;
        isHiForest = false;
    };
    HiForestInfoController(TTree* tree){
        isHiForest = false;

        this->tree = tree;
        if (this->tree->GetEntries() > 0) isHiForest = true;
    };
    ~HiForestInfoController(){};

    std::string readBranch(TTree* tree, std::string branchName);
    void readVersion(TTree* tree);
    void readGlobalTag(TTree* tree);
    void readInputLines(TTree* tree);
    void printHiForestInfo();
    static int getCollisionType(std::string filePath);
    static int getCollisionType(TFile* file);
    static int getCollisionType(TTree* treeSkim);

    bool isHiForest;
    TTree* tree;

    std::string globalTag;
    std::string hiForestVersion;
    std::string inputLines;

private:
};

std::string HiForestInfoController::readBranch(TTree* tree, std::string branchName)
{
    if (tree == 0)  {
        return "NULLTREE";
    }
    isHiForest = true;

    char* charArr = new char[CHARARRAYSIZE];   // branches are of type char array
    // https://github.com/CmsHI/cmssw/blob/4c644639d7a75dbf15cbcdd6bce6313fa446836c/HeavyIonsAnalysis/JetAnalysis/src/HiForestInfo.cc#L125-L127

    // https://root.cern.ch/doc/master/TTree_8h_source.html#l00230
    int tmpSetBranchAddressStatus = tree->SetBranchAddress(branchName.c_str(), charArr);
    if (tmpSetBranchAddressStatus != TTree::kMatch)  return "";

    std::string value = "";
    Long64_t entries = tree->GetEntries();
    for (Long64_t j_entry=0; j_entry<entries; ++j_entry) {

        tree->GetEntry(j_entry);

        std::string tmp(charArr);

        if (j_entry > 0) {
            if (tmp != value) {
                std::cout << "WARNING : value of the branch is not the same across HiForestInfo tree"<< std::endl;
                std::cout << "branch : " << branchName.c_str()<< std::endl;
                std::cout << "tree has " << entries << " entries" << std::endl;
                std::cout << "value for entry "<< j_entry-1 << " = " << value.c_str() << std::endl;
                std::cout << "value for entry "<< j_entry   << " = " << tmp.c_str() << std::endl;
            }
        }

        value = tmp.c_str();
    }
    memset(charArr, 0, CHARARRAYSIZE * (sizeof charArr[0]));
    delete[] charArr;

    return value;
}

void HiForestInfoController::readVersion(TTree* tree) {
    hiForestVersion = readBranch(tree, "HiForestVersion");
}

void HiForestInfoController::readGlobalTag(TTree* tree) {
    globalTag = readBranch(tree, "GlobalTag");
}

void HiForestInfoController::readInputLines(TTree* tree) {
    inputLines = readBranch(tree, "InputLines");
}

void HiForestInfoController::printHiForestInfo() {
    if (isHiForest) {
        readVersion(tree);
        readGlobalTag(tree);
        readInputLines(tree);

        std::cout<<"entries = " << tree->GetEntries() << std::endl;
        std::cout<<"HiForestVersion = " << hiForestVersion << std::endl;
        std::cout<<"GlobalTag       = " << globalTag << std::endl;
        std::cout<<"InputLines      = " << inputLines << std::endl;
    }
    else {
        std::cout<<"input ROOT file does not have HiForestInfo tree" << std::endl;
    }
}

int HiForestInfoController::getCollisionType(std::string filePath)
{
    TFile* file = TFile::Open(filePath.c_str(), "READ");
    if (file == 0 || file->IsZombie())  return -3;

    int collisionType = getCollisionType(file);

    file->Close();
    return collisionType;
}

int HiForestInfoController::getCollisionType(TFile* file)
{
    TTree* treeSkim = (TTree*)file->Get("skimanalysis/HltTree");
    if (treeSkim == 0 || treeSkim->IsZombie()) return -2;

    return getCollisionType(treeSkim);
}

int HiForestInfoController::getCollisionType(TTree* treeSkim)
{
    if (treeSkim->GetBranch("pcollisionEventSelection"))
        return COLL::kHI;
    else if (treeSkim->GetBranch("pPAprimaryVertexFilter") && treeSkim->GetBranch("pBeamScrapingFilter"))
        return COLL::kPP;
    else
        return -1;
}

#endif /* HIFORESTINFOCONTROLLER_H_ */
