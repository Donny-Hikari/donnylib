/**
 * donnylib - A lightweight library for c++
 * 
 * filesystem.hpp - classes and functions for filesystem
 * dependency : base.hpp, file.hpp
 * 
 * Author : Donny
 */

#pragma once

#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>

#include "file.hpp"
#include "file_stream.hpp"

namespace donny {
namespace filesystem {

// file_size

uintmax_t file_size(std::ifstream &fs) {
    if (!fs.is_open()) return 0;

    uintmax_t size = 0;
    uintmax_t curpos = fs.tellg(); // store current position

    // get beginning position
    fs.seekg(0, std::ios::beg);
    size = fs.tellg();
    
    // get end position
    fs.seekg(0, std::ios::end);

    // filesize = endpos - begpos
    size = (uintmax_t)fs.tellg() - size;

    // seek back to original position
    fs.seekg(curpos, std::ios::beg);

    return size;
}

uintmax_t file_size(std::string filename) {
    std::ifstream ifs(filename);
    uintmax_t size = file_size(ifs);
    ifs.close();
    return size;
}

uintmax_t file_size(FILE* f) {
    if (f == nullptr) return 0;

    uintmax_t size = 0;
    uintmax_t curpos = ftell(f); // store current position
    
    // get end position
    fseek(f, 0, SEEK_END);
    size = ftell(f);

    // get beginning position
    fseek(f, 0, SEEK_SET);

    // filesize = endpos - begpos
    size -= ftell(f);

    // seek back to original position
    fseek(f, curpos, SEEK_SET);
    
    return size;
}

// readString and writeString

template<typename CharType>
inline std::basic_string<CharType> readString(basic_file<CharType> f)
{
	using StringType = std::basic_string<CharType>;

	if (!f.is_open()) return StringType();

	int len = 0;
	f.read(&len);
	return f.gets(len);
}
template<typename CharType>
inline uint writeString(basic_file<CharType> f, std::basic_string<CharType> str)
{
	int len = str.length();
	f.write(&len);
	return f.puts(str, len);
}

}
}
