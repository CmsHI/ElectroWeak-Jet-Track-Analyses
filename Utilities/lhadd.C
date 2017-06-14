#include "TFileMerger.h"
#include "TTree.h"

#include <fstream>
#include <string>
#include <vector>

int lhadd(std::string input, std::string output) {
    TTree::SetMaxTreeSize(1LL * 1024 * 1024 * 1024 * 1024);

    std::ifstream file_stream(input);
    if (!file_stream) return 1;

    std::vector<std::string> file_list;
    std::string line;
    while (std::getline(file_stream, line))
        file_list.push_back(line);

    TFileMerger merger(kFALSE, kFALSE);
    merger.OutputFile(output.c_str());
    for (std::size_t i = 0; i < file_list.size(); ++i) {
        if (!merger.AddFile(file_list[i].c_str())) {
            return 1;
        }
    }

    merger.Merge();

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
