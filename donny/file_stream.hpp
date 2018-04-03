/**
 * donnylib - A lightweight library for c++
 * 
 * file_stream.hpp - stream of basic_file
 * base : base.hpp, file.hpp
 * 
 * Author : Donny
 */

#pragma once

#include <cstdio>
#include <cstdarg>
#include <cwchar>
#include <string>

#include "file.hpp"

namespace donny {
namespace filesystem {

template<typename CharType>
class file_stream {

public:
    using FileType = filesystem::basic_file<CharType>;
    using SizeType = typename FileType::SizeType;
    using StringType = typename FileType::StringType;
    using RawString = CharType*;

    file_stream(FileType& f)
        : _file(f)
    {
    }

    inline file_stream& newLine() {
        _file.newLine();
        return *this;
    }

    inline file_stream& operator<<(const StringType str)
    {
        _file.print(str);
        return *this;
    }

    inline file_stream& operator<<(const void* str)
    {
        _file.print((const RawString)str);
        return *this;
    }

    inline file_stream& operator<<(unsigned char n)
    { return logNumber(n); }
    inline file_stream& operator<<(short n)
    { return logNumber(n); }
    inline file_stream& operator<<(unsigned short n)
    { return logNumber(n); }
    inline file_stream& operator<<(int n)
    { return logNumber(n); }
    inline file_stream& operator<<(unsigned int n)
    { return logNumber(n); }
    inline file_stream& operator<<(long n)
    { return logNumber(n); }
    inline file_stream& operator<<(unsigned long n)
    { return logNumber(n); }
    inline file_stream& operator<<(long long n)
    { return logNumber(n); }
    inline file_stream& operator<<(unsigned long long n)
    { return logNumber(n); }
    inline file_stream& operator<<(double n)
    { return logNumber(n); }
    inline file_stream& operator<<(long double n)
    { return logNumber(n); }
    inline file_stream& operator<<(bool n)
    { return logNumber(n); }

    inline file_stream& operator<<(CharType c)
    {
		_file.putc(c);
		return *this;
	}

    inline file_stream& operator<<
        (file_stream& (*_pf)(file_stream&))
    {
        return _pf(*this);
    }

private:
    FileType& _file;

    template<typename T>
    inline file_stream& logNumber(T &n)
    {
        _file.print(deduceSpecifier<T, CharType>(n), n);
        return *this;
    }

};

template<typename CharType>
inline file_stream<CharType>&
    endl(file_stream<CharType> &ls)
{ return ls.newLine(); }

} // namespace filesystem

using filesystem::endl;

} // namespace donny