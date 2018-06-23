/**
 * donnylib - A lightweight library for c++
 * 
 * datetime.hpp - Get datetime easily.
 * base : base.hpp, file.hpp
 * 
 * Author : Donny
 */

#pragma once

#include <iomanip>
#include <ctime>
#include <string>
#include <sstream>

#include "file.hpp"

namespace donny {
namespace datetime {

template<typename CharType>
using DateTimeString = std::basic_string<CharType>;

template<typename CharType>
constexpr const CharType* getWeekday(int wday)
{
    const CharType* weekdays[] = {
        AUTO_AW(CharType, "Sun"),
        AUTO_AW(CharType, "Mon"),
        AUTO_AW(CharType, "Tue"),
        AUTO_AW(CharType, "Wed"),
        AUTO_AW(CharType, "Thu"),
        AUTO_AW(CharType, "Fri"),
        AUTO_AW(CharType, "Sat"),
    };
    return weekdays[wday];
}

template<typename CharType>
constexpr const CharType* getMonth(int mon)
{
    const CharType* months[] = {
        AUTO_AW(CharType, "Jan"),
        AUTO_AW(CharType, "Feb"),
        AUTO_AW(CharType, "Mar"),
        AUTO_AW(CharType, "Apr"),
        AUTO_AW(CharType, "May"),
        AUTO_AW(CharType, "Jun"),
        AUTO_AW(CharType, "Jul"),
        AUTO_AW(CharType, "Aug"),
        AUTO_AW(CharType, "Sep"),
        AUTO_AW(CharType, "Oct"),
        AUTO_AW(CharType, "Nov"),
        AUTO_AW(CharType, "Dec"),
    };
    return months[mon];
}

template<typename CharType>
const DateTimeString<CharType>
    getUTCTime(tm *gmtm, const CharType*);

// dtformat example : %b %a %d %T %Y
template<>
inline const DateTimeString<char>
    getUTCTime<char>(tm *ptm, const char *dtformat)
{
    if (ptm == nullptr || dtformat == nullptr)
        return "";
        
    std::ostringstream buf;
    buf << std::put_time(ptm, dtformat);
    
    return buf.str();
}

// dtformat example : %b %a %d %T %Y
template<>
inline const DateTimeString<wchar_t>
    getUTCTime<wchar_t>(tm *ptm, const wchar_t *dtformat)
{
    if (ptm == nullptr || dtformat == nullptr)
        return L"";
        
    std::wostringstream buf;
    buf << std::put_time(ptm, dtformat);
    
    return buf.str();
}

}
}
