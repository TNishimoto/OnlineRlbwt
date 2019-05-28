
#include <stdint.h>

#include <time.h>
#include <iostream>
#include <iomanip>
#include <chrono>

#include "cmdline.h"
#include "OnlineRlbwt.hpp"
#include "DynRleForRlbwt.hpp"
#include "online_rlbwt.hpp"


using namespace itmmti;

template <typename T>
bool equalCheck(const std::vector<T> &vec1,const std::vector<T> &vec2)
{
    if (vec1.size() != vec2.size())
    {
        std::string s = std::string("String sizes are different!") + ", collect = " + std::to_string(vec1.size()) + ", test = " + std::to_string(vec2.size());
        throw std::logic_error(s);
    }
    for (uint64_t i = 0; i < vec1.size(); i++)
    {
        if (vec1[i] != vec2[i])
        {
            std::string msg = "collect_vec[" + std::to_string(i) + "] != test_vec[" + std::to_string(i) + "]";

            throw std::logic_error("Values are different! " + msg);
        }
    }
    return true;
}

int main(int argc, char *argv[])
{
  cmdline::parser parser;
  parser.add<std::string>("input",'i', "input file name", true);

  parser.parse_check(argc, argv);
  const std::string in = parser.get<std::string>("input");

	std::ifstream ifs(in);
  std::string s1;
  char c;
  while (ifs.peek() != std::ios::traits_type::eof()) {
    ifs.get(c);
    s1.push_back(c);
  }
  ifs.close();

  std::string s2 = load(in);

  if(s1 == s2){
    std::cout << "OK!" << std::endl;
  }else{
    std::cout << "ERROR!" << std::endl;
  std::cout << "s1: " << s1 << std::endl;
  std::cout << "s2: " << s2 << std::endl;
  }

  std::vector<char> cvec1;
  std::vector<uint64_t> nvec1;

  std::vector<char> cvec2;
  std::vector<uint64_t> nvec2;

  online_rlbwt(s1, cvec1, nvec1, 1);
  
  online_rlbwt_from_file<uint64_t>(in, cvec2, nvec2, 1);

    bool b1 = equalCheck<char>(cvec1, cvec2);
    bool b2 = equalCheck<uint64_t>(nvec1, nvec2);

    if (b1 && b2)
    {
        std::cout << "\033[32m"
                  << "\033[1m"
                  << "OK! "
                  << "\033[0m" << std::endl;
    }
    std::cout << cvec1.size() << "/" << cvec2.size() << std::endl;
  
}
