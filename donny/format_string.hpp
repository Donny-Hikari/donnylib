/**
 * donnylib - A lightweight library for c++
 * 
 * format_string.hpp - a class to help to format string.
 * dependency: stringstream
 * 
 * Author : Donny
 */

#pragma once

#include <sstream>
#include <string>

namespace donny {


template<typename CharType>
class basic_format_string
{
public:
    template<typename T>
    basic_format_string& operator<<(const T& v)
    {
        ss << v;
        return *this;
    }

    std::basic_string<CharType> str() const
    {
        return ss.str();
    }

    CharType const* c_str() const
    {
        return ss.str().c_str();
    }

    operator std::basic_string<CharType>() const
    {
        return str();
    }

private:
    std::basic_stringstream<CharType> ss;

};

typedef basic_format_string<char> format_string;


} // donny
