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
#include <iostream>

//#include "cmdline.h"
//#include "OnlineRlbwt.hpp"
//#include "DynRleForRlbwt.hpp"

//using namespace itmmti;

namespace itmmti
{
    /*
class backward_ifstream{
    private:

    std::ifstream ifs;
    uint64_t pos;
    std::vector<char> buffer;
    uint64_t _size;
    int64_t local_pos;
    static const uint64_t BufferSize = 8092;

    public:
    backward_ifstream(std::string filepath);
    uint64_t size();
    uint64_t rest_size();
    
    void get(char &c);
    void close();
    bool is_end();
};
*/

std::string online_bwt(std::string &text);

template <typename INDEX>
bool online_rlbwt_from_file(std::string filepath, std::vector<char> &outputCharVec, std::vector<INDEX> &outputNumVec, int outputType);



template <typename INDEX>
bool online_rlbwt(std::string &text, std::vector<char> &outputCharVec, std::vector<INDEX> &outputNumVec, int outputType);





//bool online_rlbwt_from_file(std::string filepath, std::vector<char> &outputCharVec, std::vector<uint64_t> &outputNumVec, int outputType);
std::string load(std::string filepath);

} // namespace itmmti