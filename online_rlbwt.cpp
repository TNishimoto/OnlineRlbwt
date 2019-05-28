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
#include <exception>

//#include "cmdline.h"
#include "OnlineRlbwt.hpp"
#include "DynRleForRlbwt.hpp"

using namespace itmmti;

namespace itmmti
{

class backward_ifstream
{

    std::ifstream ifs;
    int64_t pos = 0;
    std::vector<char> buffer;
    int64_t _size;
    int64_t local_pos;
    static const int64_t BufferSize = 8092;


public:
    backward_ifstream(std::string filepath) : ifs(filepath)
    {
        ifs.seekg(0, std::ios::end);
        uint64_t textSize = (uint64_t)ifs.tellg() / sizeof(char);
        ifs.seekg(0, std::ios::end);
        _size = textSize;

        this->buffer.resize(backward_ifstream::BufferSize, 0);
        //std::cout << this->buffer.size() << "/" <<  backward_ifstream::BufferSize<< std::endl;
        this->buffering();
        //std::cout << this->buffer.size() << "/" <<  backward_ifstream::BufferSize<< std::endl;
        //std::cout << this->is_end() << std::endl;
        //for(int64_t i=local_pos;i>=0;i--){
            //std::cout << std::to_string(this->buffer[i]) << "," ;
            //std::cout << (this->buffer[i] == 0 ? '0' : this->buffer[i]);
        //}
        //std::cout << std::endl;

    }
    uint64_t size()
    {
        return this->_size;
    }
    uint64_t rest_size(){
        return this->_size - this->pos;
    }
    bool buffering()
    {
        //this->buffer.clear();
        if(pos != this->_size){
            uint64_t _rest_size = this->rest_size();
            if( _rest_size < backward_ifstream::BufferSize){
                ifs.seekg(this->_size-(pos + _rest_size), std::ios::beg);
                ifs.read((char *)&(this->buffer)[0],  _rest_size * sizeof(char));
                pos +=  _rest_size;
                this->local_pos =  _rest_size-1;

            } else{
                ifs.seekg(this->_size-(pos + backward_ifstream::BufferSize), std::ios::beg);
                ifs.read((char *)&(this->buffer)[0], backward_ifstream::BufferSize * sizeof(char));
                pos += backward_ifstream::BufferSize;
                this->local_pos = backward_ifstream::BufferSize-1;
            }
            return true;
        }else{
            return false;
        }
    }

    void get(char &c)
    {
        //std::cout << pos << "/" << this->local_pos << "/" << this->_size << std::endl;
        if(this->local_pos == -1){
            bool b = this->buffering();
            if(!b){
                throw std::logic_error("error!");
            }
        }
        c = this->buffer[this->local_pos--];
    }
    void close()
    {
        ifs.close();
    }
    bool is_end(){
        return pos == this->_size && this->local_pos == -1;
    }
};


template <typename INDEX>
bool online_rlbwt(std::string &text, std::vector<char> &outputCharVec, std::vector<INDEX> &outputNumVec, int outputType)
{
    for (auto c : text)
    {
        if (c == 0)
        {
            throw std::logic_error("The input text for the online_rlbwt function cannot contain the character (char)0.");
        }
    }

    std::reverse(text.begin(), text.end());
    const bool verbose = false;

    auto t1 = std::chrono::high_resolution_clock::now();
    std::cout << "Building RLBWT ..." << std::endl;

    size_t j = 0;
    const size_t step = 1000000; // print status every step characters
    size_t last_step = 0;

    using BTreeNodeT = BTreeNode<32>;
    using BtmNodeMT = BtmNodeM_StepCode<BTreeNodeT, 32>;
    using BtmMInfoT = BtmMInfo_BlockVec<BtmNodeMT, 512>;
    using BtmNodeST = BtmNodeS<BTreeNodeT, uint32_t, 8>;
    using BtmSInfoT = BtmSInfo_BlockVec<BtmNodeST, 1024>;
    using DynRleT = DynRleForRlbwt<WBitsBlockVec<1024>, Samples_Null, BtmMInfoT, BtmSInfoT>;
    OnlineRlbwt<DynRleT> rlbwt(1);

    char c; // Assume that the input character fits in char.
    unsigned char uc;

    uint64_t x = 0;

    while (x < text.size())
    {
        c = text[x++];
        uc = static_cast<unsigned char>(c);
        if (verbose)
        {
            if (j > last_step + (step - 1))
            {
                last_step = j;
                std::cout << " " << j << " characters processed ..." << std::endl;
            }
        }

        rlbwt.extend(uint8_t(uc));
        ++j;
    }

    auto t2 = std::chrono::high_resolution_clock::now();
    double sec = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
    std::cout << "RLBWT construction done. " << sec << " sec" << std::endl;

    rlbwt.printStatistics(std::cout, false);

    rlbwt.invert(outputCharVec, outputNumVec, outputType);
    std::reverse(text.begin(), text.end());

    return true;
}
template bool online_rlbwt<uint32_t>(std::string &text, std::vector<char> &outputCharVec, std::vector<uint32_t> &outputNumVec, int outputType);
template bool online_rlbwt<uint64_t>(std::string &text, std::vector<char> &outputCharVec, std::vector<uint64_t> &outputNumVec, int outputType);
template bool online_rlbwt<int32_t>(std::string &text, std::vector<char> &outputCharVec, std::vector<int32_t> &outputNumVec, int outputType);
template bool online_rlbwt<int64_t>(std::string &text, std::vector<char> &outputCharVec, std::vector<int64_t> &outputNumVec, int outputType);

std::string online_bwt(std::string &text){
    std::vector<char> cVec;
    std::vector<uint64_t> nVec;
    online_rlbwt(text, cVec, nVec, 0);
    std::string s;
    for(uint64_t i=0;i<cVec.size();i++){
        for(uint64_t x=0;x<nVec[i];x++){
            s.push_back(cVec[i]);
        }
    }
    return s;
}
/*
template <typename INDEX>
bool online_rlbwt1(std::string &text, std::vector<char> &outputCharVec, std::vector<INDEX> &outputNumVec, int outputType){

}
*/

template <typename INDEX>
bool online_rlbwt_from_file(std::string filepath, std::vector<char> &outputCharVec, std::vector<INDEX> &outputNumVec, int outputType)
//bool online_rlbwt_from_file(std::string filepath, std::vector<char> &outputCharVec, std::vector<uint64_t> &outputNumVec, int outputType)
{
    /*
    cmdline::parser parser;
    parser.add<std::string>("input", 'i', "input file name", true);
    parser.add<std::string>("output", 'o', "output file name", false);
    parser.add<bool>("check", 0, "check correctness", false, 0);
    parser.add<bool>("verbose", 'v', "verbose", false, 0);
    parser.add("help", 0, "print help");

    parser.parse_check(argc, argv);
    */
    //const std::string in = filepath;
    //const std::string in = parser.get<std::string>("input");
    //const std::string out = parser.get<std::string>("output");
    //const bool check = parser.get<bool>("check");
    //const bool verbose = parser.get<bool>("verbose");
    const bool verbose = false;

    auto t1 = std::chrono::high_resolution_clock::now();
    std::cout << "Building RLBWT ..." << std::endl;

    //std::ifstream ifs(in);
    backward_ifstream bifs(filepath);

    size_t j = 0;
    const size_t step = 1000000; // print status every step characters
    size_t last_step = 0;

    using BTreeNodeT = BTreeNode<32>;
    using BtmNodeMT = BtmNodeM_StepCode<BTreeNodeT, 32>;
    using BtmMInfoT = BtmMInfo_BlockVec<BtmNodeMT, 512>;
    using BtmNodeST = BtmNodeS<BTreeNodeT, uint32_t, 8>;
    using BtmSInfoT = BtmSInfo_BlockVec<BtmNodeST, 1024>;
    using DynRleT = DynRleForRlbwt<WBitsBlockVec<1024>, Samples_Null, BtmMInfoT, BtmSInfoT>;
    OnlineRlbwt<DynRleT> rlbwt(1);

    char c; // Assume that the input character fits in char.
    unsigned char uc;

    while (!bifs.is_end() )
    {
        bifs.get(c);
        uc = static_cast<unsigned char>(c);
        if (verbose)
        {
            if (j > last_step + (step - 1))
            {
                last_step = j;
                std::cout << " " << j << " characters processed ..." << std::endl;
            }
        }

        rlbwt.extend(uint8_t(uc));
        ++j;
    }

    bifs.close();
    {
        auto t2 = std::chrono::high_resolution_clock::now();
        double sec = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
        std::cout << "RLBWT construction done. " << sec << " sec" << std::endl;
    }

    rlbwt.printStatistics(std::cout, false);
    //t1 = std::chrono::high_resolution_clock::now();
    //std::cout << "Decompressing RLBWT ..." << std::endl;
    //std::ofstream ofs(out, std::ios::out);
    rlbwt.invert(outputCharVec, outputNumVec, outputType);

    //auto t2 = std::chrono::high_resolution_clock::now();
    //double sec = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
    //std::cout << "RLBWT decompression done. " << sec << " sec" << std::endl;
    /*
    if (!(out.empty()))
    {
        
    }

    if (check)
    { // check correctness
        t1 = std::chrono::high_resolution_clock::now();
        std::cout << "Checking RLBWT inversion ..." << std::endl;
        std::ifstream ifssss(in);
        if (rlbwt.checkDecompress(ifssss))
        {
            auto t2 = std::chrono::high_resolution_clock::now();
            double sec = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
            std::cout << "RLBWT decompressed correctly. " << sec << " sec" << std::endl;
        }
        else
        {
            std::cout << "RLBWT inversion failed." << std::endl;
        }
    }
    */
    return true;
}
template bool online_rlbwt_from_file<uint32_t>(std::string filepath, std::vector<char> &outputCharVec, std::vector<uint32_t> &outputNumVec, int outputType);
template bool online_rlbwt_from_file<uint64_t>(std::string filepath, std::vector<char> &outputCharVec, std::vector<uint64_t> &outputNumVec, int outputType);
template bool online_rlbwt_from_file<int32_t>(std::string filepath, std::vector<char> &outputCharVec, std::vector<int32_t> &outputNumVec, int outputType);
template bool online_rlbwt_from_file<int64_t>(std::string filepath, std::vector<char> &outputCharVec, std::vector<int64_t> &outputNumVec, int outputType);


std::string load(std::string filepath){
    char c;
  std::string s2;
  backward_ifstream bifs(filepath);
  while(!bifs.is_end()){
    bifs.get(c);
    s2.push_back(c);
  }
  bifs.close();
  std::reverse(s2.begin(), s2.end());
  return s2;
}
} // namespace itmmti