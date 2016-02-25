/*
 *  class to read and evaluate HiForest production info, works on tree path HiForest/HiForestInfo
 */

#ifndef HIFORESTINFOCONTROLLER_H_
#define HIFORESTINFOCONTROLLER_H_

#include <TTree.h>

#include <string>

const int CHARARRAYSIZE = 5000;

class HiForestInfoController {
public:
    HiForestInfoController(){
        isHiForest = false;
    };
    HiForestInfoController(TTree* tree){
        isHiForest = false;

        this->tree = tree;
        if (this->tree->GetEntries() > 0) isHiForest = true;
    };
    ~HiForestInfoController(){};

    std::string readBranch(TTree* tree, std::string branchName)
    {
        if (!tree)  {
            return "NULLTREE";
        }
        isHiForest = true;

        char* charArr = new char[CHARARRAYSIZE];   // branches are of type char array
        // https://github.com/CmsHI/cmssw/blob/4c644639d7a75dbf15cbcdd6bce6313fa446836c/HeavyIonsAnalysis/JetAnalysis/src/HiForestInfo.cc#L125-L127

        tree->SetBranchAddress(branchName.c_str(), charArr);
        std::string value = "";
        Long64_t entries = tree->GetEntries();
        for (Long64_t j_entry=0; j_entry<entries; ++j_entry) {

            tree->GetEntry(j_entry);

            std::string tmp(charArr);

            if (j_entry > 0) {
                if (tmp.compare(value.c_str()) != 0) {
                    std::cout << "WARNING : value of the branch is not the same across HiForestInfo tree"<< std::endl;
                    std::cout << "branch : " << branchName.c_str()<< std::endl;
                    std::cout << "tree has " << entries << " entries" << std::endl;
                    std::cout << "value for entry "<< j_entry-1 << " = " << value.c_str() << std::endl;
                    std::cout << "value for entry "<< j_entry   << " = " << tmp.c_str() << std::endl;
                }
            }

            value = tmp.c_str();
        }
        delete[] charArr;

        return value;
    }

    void readVersion(TTree* tree) {
        hiForestVersion = readBranch(tree, "HiForestVersion");
    }

    void readGlobalTag(TTree* tree) {
        globalTag = readBranch(tree, "GlobalTag");
    }

    void readInputLines(TTree* tree) {
        inputLines = readBranch(tree, "InputLines");
    }

    void printHiForestInfo() {
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

    bool isHiForest;
    TTree* tree;

    std::string globalTag;
    std::string hiForestVersion;
    std::string inputLines;

private:
};

#endif /* HIFORESTINFOCONTROLLER_H_ */
