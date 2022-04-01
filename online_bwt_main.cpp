#include <cassert>
#include <chrono>
#include "stool/include/io.hpp"
#include "stool/include/cmdline.h"
#include "online_rlbwt.hpp"

using namespace std;
using namespace stool;
//using namespace stool::rlbwt;

int main(int argc, char *argv[])
{
    cmdline::parser p;
    p.add<string>("input_file", 'i', "input file name", true);
    p.add<string>("output_file", 'o', "output file name (the default output name is 'input_file.ext')", false, "");

    p.parse_check(argc, argv);
    string inputFile = p.get<string>("input_file");
    string outputFile = p.get<string>("output_file");

    uint64_t textSize = 0;

    if (outputFile.size() == 0)
    {
        outputFile = inputFile + ".bwt";
    }


    auto start = std::chrono::system_clock::now();

    std::pair<uint64_t, uint64_t> profile = itmmti::online_bwt_from_file(inputFile, outputFile, true);
    textSize = profile.first;
    uint64_t runCount = profile.second;

    auto end = std::chrono::system_clock::now();
    double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "\033[36m";
    std::cout << "=============RESULT===============" << std::endl;
    std::cout << "File : " << inputFile << std::endl;
    std::cout << "Output : " << outputFile << std::endl;

    std::cout << "The length of the input text : " << textSize << std::endl;
    double charperms = (double)textSize / elapsed;
    std::cout << "The number of runs : " << runCount << std::endl;
    std::cout << "Excecution time : " << elapsed << "ms";
    std::cout << "[" << charperms << "chars/ms]" << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "\033[39m" << std::endl;
}