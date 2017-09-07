#include "TFileMerger.h"
#include "TTree.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int lhadd(std::string input, std::string output) {
    std::cout<<"running lhadd()"<<std::endl;
    std::cout<<"input list  = "<< input.c_str() <<std::endl;
    std::cout<<"output      = "<< output.c_str() <<std::endl;

    TTree::SetMaxTreeSize(1LL * 1024 * 1024 * 1024 * 1024);

    std::ifstream file_stream(input);
    if (!file_stream) return 1;

    std::vector<std::string> file_list;
    std::string line;
    while (std::getline(file_stream, line))
        file_list.push_back(line);

    int nFiles = file_list.size();
    std::cout<<"input ROOT files : num = "<< nFiles << std::endl;
    std::cout<<"#####"<< std::endl;
    for (std::vector<std::string>::iterator it = file_list.begin() ; it != file_list.end(); ++it) {
        std::cout<<(*it).c_str()<< std::endl;
    }
    std::cout<<"##### END #####"<< std::endl;

    TFileMerger merger(kFALSE, kFALSE);
    merger.OutputFile(output.c_str());
    std::cout<< "adding files" <<std::endl;
    for (int i = 0; i < nFiles; ++i) {
        std::string filePath = file_list.at(i).c_str();
        std::cout <<"i = " << i << " , " ;
        std::cout <<"file : " << filePath.c_str() << std::endl;
        if (!merger.AddFile(file_list[i].c_str())) {
            std::cout << "failed adding file : " << filePath.c_str() << std::endl;
            return 1;
        }
    }
    std::cout<< "adding files - END" <<std::endl;

    std::cout<< "merging" <<std::endl;
    merger.Merge();
    std::cout<< "merging - END" <<std::endl;

    std::cout<< "running lhadd() - END" <<std::endl;
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 3) {
        return lhadd(argv[1], argv[2]);
    } else {
        printf("Usage: ./Utilities/lhadd.exe [input list] [output file]\n");
        return 1;
    }
}
