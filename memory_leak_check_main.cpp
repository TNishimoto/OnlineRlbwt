
#include <stdint.h>

#include <time.h>
#include <iostream>
#include <iomanip>
#include <chrono>

#include "cmdline.h"
#include "OnlineRlbwt.hpp"
#include "DynRleForRlbwt.hpp"
#include "online_rlbwt.hpp"
#include <random>

using namespace itmmti;

std::string create_deterministic_string(uint64_t len, int64_t seed)
{
    std::string s;
	std::mt19937 mt(seed);
	std::uniform_int_distribution<> rand100(0, 2);

	for (size_t i = 0; i < len; i++)
	{
		char c = 96 + rand100(mt);
		s.push_back(c);
	}
	return s;
}

int main(int argc, char *argv[])
{
  //cmdline::parser parser;
  //parser.add<std::string>("input", 'i', "input file name", true);

  //parser.parse_check(argc, argv);
  //const std::string in = parser.get<std::string>("input");

  std::cout << "\033[47m"
            << "\033[31m"
            << "\033[1m"
            << "Test: backward_ifstream"
            << "\033[0m" << std::endl;

    for(int i=0;i<10;i++){
        std::cout << i << std::endl;
        std::string s1 = create_deterministic_string(100000, i);
        std::vector<char> cvec1;
        std::vector<uint64_t> nvec1;
        online_rlbwt(s1, cvec1, nvec1, 1);
    }
}
