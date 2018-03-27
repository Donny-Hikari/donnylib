#pragma once

#include <fstream>
#include <iostream>
#include <string>

#include "file.hpp"

namespace donny {
namespace filesystem {

uintmax_t file_size(std::ifstream &fs) {
    uintmax_t size = 0;
    auto curpos = fs.tellg();
    fs.seekg(0, std::ios::beg);
    size = fs.tellg();
    fs.seekg(0, std::ios::end);
    size = (uintmax_t)fs.tellg() - size;
    fs.seekg(curpos, std::ios::beg);
    return size;
}

uintmax_t file_size(std::string filename) {
    std::ifstream ifs(filename);
    uintmax_t size = file_size(ifs);
    ifs.close();
    return size;
}

}
}
