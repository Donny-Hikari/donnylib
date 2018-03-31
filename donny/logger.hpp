#pragma once

#include <cstdio>
#include <cstdarg>
#include <string>

#include "filesystem.hpp"

namespace donny {

template<typename T>
inline const char* deduceSpecifier(T obj)
{ return ""; }
template<>
constexpr const char*
    deduceSpecifier<int>(int)
{ return "%d"; }
template<>
constexpr const char*
    deduceSpecifier<unsigned int>(unsigned int)
{ return "%ud"; }
template<>
constexpr const char*
    deduceSpecifier<long>(long)
{ return "%ld"; }
template<>
constexpr const char*
    deduceSpecifier<unsigned long>(unsigned long)
{ return "%uld"; }
template<>
constexpr const char*
    deduceSpecifier<long long>(long long)
{ return "%lld"; }
template<>
constexpr const char*
    deduceSpecifier<unsigned long long>(unsigned long long)
{ return "%ulld"; }
template<>
constexpr const char*
    deduceSpecifier<double>(double)
{ return "%f"; }
template<>
constexpr const char*
    deduceSpecifier<long double>(long double)
{ return "%Lf"; }
template<>
inline const char*
    deduceSpecifier<bool>(bool obj)
{ return obj ? "True" : "False"; }

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

    inline file_stream& operator<<(StringType str)
    {
        _file.print(str);
        return *this;
    }

    inline file_stream& operator<<(RawString str)
    {
        _file.print(str);
        return *this;
    }

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
        _file.print(deduceSpecifier(n), n);
        return *this;
    }

};

template<typename CharType>
inline file_stream<CharType>&
    endl(file_stream<CharType> &ls)
{ return ls.newLine(); }

template<typename CharType = char>
class logger {

public:
    using logger_file = filesystem::basic_file<CharType>;
    using logger_stream = file_stream<CharType>;
    using SizeType = typename logger_file::SizeType;
    using StringType = typename logger_file::StringType;
    using RawString = CharType*;

    enum PrefixType {
        NONE = 0,
        INFO, // info
        ERR, // error
        DEB, // debug
        VERB, // verbose
        LOG, // log
        PREFIX_COUNT
    };

    logger(logger_file out_ = filesystem::scrout)
        : _out(out_)
        , _stream(_out)
    {
        _prefixs[NONE] = "";
        _prefixs[INFO] = "[INFO] ";
        _prefixs[ERR] = "[ERR] ";
        _prefixs[DEB] = "[DEB] ";
        _prefixs[VERB] = "[VERB] ";
        _prefixs[LOG] = "[INFO] ";
    }

    inline int vprint(const StringType format_, va_list args_)
    {
        return _out.vprint(format_, args_);
    }
    inline int vprintln(const StringType format_, va_list args_)
    {
        return vprint(format_, args_) + _out.newLine();
    }
    inline int print(const StringType format_, ...)
    {
        va_list args; va_start(args, format_);
        return TRAP_RET( vprint(format_, args), va_end(args) );
    }
    inline int println(const StringType format_, ...)
    {
        va_list args; va_start(args, format_);
        return TRAP_RET( vprintln(format_, args), va_end(args) );
    }

	inline CharType putc(CharType c)
	{
        return _out.putc(c);
	}
	inline uint puts(const StringType src, bool bWithBlankChar = false)
	{
        return _out.puts(src, bWithBlankChar);
	}
	inline uint puts(const StringType src, SizeType n)
	{
        return _out.puts(src, n);
	}
	inline uint puts(const StringType src, SizeType offset, SizeType n)
	{
        return _out.puts(src, offset, n);
	}
	template<typename T>
	inline uint write(const T *src, uint count = 1)
	{
        return _out.write(src, count);
	}
	inline uint write(const void *src, uint elementSize, uint count)
	{
        return _out.write(src, elementSize, count);
	}
    
	inline int flush()
	{
        return _out.flush();
	}

    /**
     *  @param tp : NONE < tp < PREFIX_COUNT
     */
    inline StringType setPrefix(PrefixType tp, StringType newPrefix)
    {
        StringType oldPrefix = _prefixs[tp];
        if (tp > NONE && tp < PREFIX_COUNT)
            _prefixs[tp] = newPrefix;
        return oldPrefix;
    }

    inline const StringType getPrefix(PrefixType tp) const
    {
        return _prefixs[tp];
    }

    inline int i(const StringType format_, ...)
    {
        va_list args;
        va_start(args, format_);
        return TRAP_RET( print(_prefixs[INFO]) + vprintln(format_, args), va_end(args) );
    }
    inline int e(const StringType format_, ...)
    {
        va_list args;
        va_start(args, format_);
        return TRAP_RET( print(_prefixs[ERR]) + vprintln(format_, args), va_end(args) );
    }
    inline int d(const StringType format_, ...)
    {
        va_list args;
        va_start(args, format_);
        return TRAP_RET( print(_prefixs[DEB]) + vprintln(format_, args), va_end(args) );
    }
    inline int v(const StringType format_, ...)
    {
        va_list args;
        va_start(args, format_);
        return TRAP_RET( print(_prefixs[VERB]) + vprintln(format_, args), va_end(args) );
    }
    inline int log(const StringType format_, ...)
    {
        va_list args;
        va_start(args, format_);
        return TRAP_RET( print(_prefixs[LOG]) + vprintln(format_, args), va_end(args) );
    }

    inline logger_stream& i()
    {
        print(_prefixs[INFO]);
        return _stream;
    }
    inline logger_stream& e()
    {
        print(_prefixs[ERR]);
        return _stream;
    }
    inline logger_stream& d()
    {
        print(_prefixs[DEB]);
        return _stream;
    }
    inline logger_stream& v()
    {
        print(_prefixs[VERB]);
        return _stream;
    }
    inline logger_stream& log()
    {
        print(_prefixs[LOG]);
        return _stream;
    }

    template<typename AnyType>
    inline logger_stream& operator<<(AnyType any)
    {
        // print(_prefixs[NONE]); // <-- NONE is ""
        return _stream;
    }

private:
    logger_file _out;
    logger_stream _stream;

    StringType _prefixs[PREFIX_COUNT];

};

}
