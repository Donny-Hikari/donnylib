/**
 * donnylib - A lightweight library for c++
 * 
 * datetime.hpp - Get datetime easily.
 * base : base.hpp, file.hpp
 * 
 * Author : Donny
 */

#pragma once

#include <cstdio>
#include <cwchar>
#include <ctime>
#include <string>

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

template<>
const DateTimeString<char>
    getUTCTime<char>(tm *gmtm, const char *dtformat)
{
    if (gmtm == nullptr || dtformat == nullptr)
        return "";

    int sz = fprintf(
        filesystem::dnull.getFILE(), dtformat,
        getWeekday<char>(gmtm->tm_wday),
        getMonth<char>(gmtm->tm_mon), gmtm->tm_mday,
        gmtm->tm_hour, gmtm->tm_min, gmtm->tm_sec,
        gmtm->tm_year + 1900
    );
    char buf[sz+1];

    snprintf(
        buf, sz+1, dtformat,
        getWeekday<char>(gmtm->tm_wday),
        getMonth<char>(gmtm->tm_mon), gmtm->tm_mday,
        gmtm->tm_hour, gmtm->tm_min, gmtm->tm_sec,
        gmtm->tm_year + 1900
    );
    
    return buf;
}

template<>
const DateTimeString<wchar_t>
    getUTCTime<wchar_t>(tm *gmtm, const wchar_t *dtformat)
{
    if (gmtm == nullptr || dtformat == nullptr)
        return L"";

    int sz = fwprintf(
        filesystem::wdnull.getFILE(), dtformat,
        getWeekday<wchar_t>(gmtm->tm_wday),
        getMonth<wchar_t>(gmtm->tm_mon), gmtm->tm_mday,
        gmtm->tm_hour, gmtm->tm_min, gmtm->tm_sec,
        gmtm->tm_year + 1900
    );
    wchar_t buf[sz+1];

    swprintf(
        buf, sz+1, dtformat,
        getWeekday<wchar_t>(gmtm->tm_wday),
        getMonth<wchar_t>(gmtm->tm_mon), gmtm->tm_mday,
        gmtm->tm_hour, gmtm->tm_min, gmtm->tm_sec,
        gmtm->tm_year + 1900
    );

    return buf;
}

}
}
