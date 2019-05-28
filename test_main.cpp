
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

template <typename INDEX>
std::vector<INDEX> naive_sa(std::string &text)
{
  std::vector<INDEX> sa;
  INDEX n = text.size();
  sa.resize(n);
  for (INDEX i = 0; i < n; i++)
  {
    sa[i] = i;
  }
  std::sort(sa.begin(), sa.end(),
            [&](const INDEX &x, const INDEX &y) {
              INDEX size = x < y ? text.size() - y : text.size() - x;
              for (INDEX i = 0; i < size; i++)
              {
                if ((uint8_t)text[x + i] != (uint8_t)text[y + i])
                {
                  return (uint8_t)text[x + i] < (uint8_t)text[y + i];
                }
              }
              return x <= y ? false : true;
            });
  return sa;
}
std::string naive_bwt(std::string &text)
{
  std::vector<uint64_t> sa = naive_sa<uint64_t>(text);

  std::string output;
  output.resize(text.size());
  for (uint64_t i = 0; i < text.size(); i++)
  {
    if (sa[i] == 0)
    {
      output[i] = text[text.size() - 1];
    }
    else
    {
      output[i] = text[sa[i] - 1];
    }
  }
  return output;
}

template <typename T>
bool equalCheck(const std::vector<T> &vec1, const std::vector<T> &vec2)
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
  parser.add<std::string>("input", 'i', "input file name", true);

  parser.parse_check(argc, argv);
  const std::string in = parser.get<std::string>("input");

  std::cout << "\033[47m"
            << "\033[31m"
            << "\033[1m"
            << "Test: backward_ifstream"
            << "\033[0m" << std::endl;

  std::ifstream ifs(in);
  std::string s1;
  char c;
  while (ifs.peek() != std::ios::traits_type::eof())
  {
    ifs.get(c);
    s1.push_back(c);
  }
  ifs.close();
  std::string s2 = load(in);

  if (s1 == s2)
  {
    std::cout << "\033[32m"
              << "\033[1m"
              << "OK! "
              << "\033[0m" << std::endl;
  }
  else
  {
    std::cout << "ERROR!" << std::endl;
    throw - 1;
  }

  std::cout << "\033[47m"
            << "\033[31m"
            << "\033[1m"
            << "Test: bwt"
            << "\033[0m" << std::endl;
  std::string bwt2 = online_bwt(s1);

  std::string textx = s1;
  textx.push_back(0);
  std::string bwt1 = naive_bwt(textx);

  if (bwt1 == bwt2)
  {

    std::cout << "\033[32m"
              << "\033[1m"
              << "OK! "
              << "\033[0m" << std::endl;
  }
  else
  {

    std::cout << "ERROR!" << std::endl;
    for (auto c : bwt1)
    {
      std::cout << (c == 0 ? '#' : c);
    }
    std::cout << std::endl;

    for (auto c : bwt2)
    {
      std::cout << (c == 0 ? '#' : c);
    }
    std::cout << std::endl;

    throw - 1;
  }

  std::cout << "\033[47m"
            << "\033[31m"
            << "\033[1m"
            << "Test: RLBWT"
            << "\033[0m" << std::endl;

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
  else
  {
    std::cout << "ERROR!" << std::endl;
    throw - 1;
  }

  std::cout << cvec1.size() << "/" << cvec2.size() << std::endl;
}
