/*!
 * Copyright (c) 2017 Tomohiro I
 *
 * This program is released under the MIT License.
 * http://opensource.org/licenses/mit-license.php
 */
/*!
 * @file OnlineRlbwt.cpp
 * @brief Online RLBWT construction.
 * @author Tomohiro I
 * @date 2018-01-27
 */
#include <stdint.h>

#include <time.h>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>

//#include "cmdline.h"
//#include "OnlineRlbwt.hpp"
//#include "DynRleForRlbwt.hpp"

//using namespace itmmti;

namespace itmmti
{
bool online_rlbwt(std::string filepath, std::vector<char> &outputCharVec, std::vector<uint64_t> &outputNumVec, int outputType);
bool online_rlbwt2(std::string &text, std::vector<char> &outputCharVec, std::vector<uint64_t> &outputNumVec, int outputType);
} // namespace itmmti