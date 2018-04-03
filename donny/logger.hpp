/**
 * donnylib - A lightweight library for c++
 * 
 * logger.hpp - A easy logger for c++
 * base : base.hpp, file.hpp, file_stream.hpp, datetime.hpp
 * 
 * Author : Donny
 */

#pragma once

#include <cstdio>
#include <cstdarg>
#include <ctime>
#include <string>

#include "file.hpp"
#include "file_stream.hpp"
#include "datetime.hpp"

namespace donny {

template<typename CharType = char>
class logger {

public:
    using logger_file = filesystem::basic_file<CharType>;
    using logger_stream = filesystem::file_stream<CharType>;
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

    logger(logger_file out_ = filesystem::dout)
        : _out(out_)
        , _stream(_out)
        , _bUseTimeStamp(true)
        , _dtFormat(AUTO_AW(CharType, "[%a %b %d %T %Y]"))
    {
        _prefixs[NONE] = AUTO_AW(CharType, "");
        _prefixs[INFO] = AUTO_AW(CharType, "[INFO] ");
        _prefixs[ERR] = AUTO_AW(CharType, "[ERR] ");
        _prefixs[DEB] = AUTO_AW(CharType, "[DEB] ");
        _prefixs[VERB] = AUTO_AW(CharType, "[VERB] ");
        _prefixs[LOG] = AUTO_AW(CharType, "[INFO] ");
    }

    close()
    {
        _out.close();
        _out = logger_file("/dev/null", "wb");
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

    inline void useTimeStamp(bool bUseTimeStamp_)
    {
        _bUseTimeStamp = bUseTimeStamp_;
    }
    inline bool isTimeStampOn() const
    {
        return _bUseTimeStamp;
    }
    inline int putTimeStamp()
    {
        return print(_getUTCTime());
    }
    inline StringType setTimeStampFormat(StringType newFormat)
    {
        StringType oldFormat = _dtFormat;
        _dtFormat = newFormat;
        return oldFormat;
    }
    inline StringType getTimeStampFormat()
    {
        return _dtFormat;
    }

    inline int i(const StringType format_, ...)
    {
        va_list args;
        va_start(args, format_);
        return TRAP_RET( _logTimeStamp()
                       + print(_prefixs[INFO])
                       + vprintln(format_, args)
                       , va_end(args) );
    }
    inline int e(const StringType format_, ...)
    {
        va_list args;
        va_start(args, format_);
        return TRAP_RET( _logTimeStamp()
                       + print(_prefixs[ERR])
                       + vprintln(format_, args)
                       , va_end(args) );
    }
    inline int d(const StringType format_, ...)
    {
        va_list args;
        va_start(args, format_);
        return TRAP_RET( _logTimeStamp()
                       + print(_prefixs[DEB])
                       + vprintln(format_, args)
                       , va_end(args) );
    }
    inline int v(const StringType format_, ...)
    {
        va_list args;
        va_start(args, format_);
        return TRAP_RET( _logTimeStamp()
                       + print(_prefixs[VERB])
                       + vprintln(format_, args)
                       , va_end(args) );
    }
    inline int log(const StringType format_, ...)
    {
        va_list args;
        va_start(args, format_);
        return TRAP_RET( _logTimeStamp()
                       + print(_prefixs[LOG])
                       + vprintln(format_, args)
                       , va_end(args) );
    }

    inline logger_stream& i()
    {
        _logTimeStamp();
        print(_prefixs[INFO]);
        return _stream;
    }
    inline logger_stream& e()
    {
        _logTimeStamp();
        print(_prefixs[ERR]);
        return _stream;
    }
    inline logger_stream& d()
    {
        _logTimeStamp();
        print(_prefixs[DEB]);
        return _stream;
    }
    inline logger_stream& v()
    {
        _logTimeStamp();
        print(_prefixs[VERB]);
        return _stream;
    }
    inline logger_stream& log()
    {
        _logTimeStamp();
        print(_prefixs[LOG]);
        return _stream;
    }

    template<typename AnyType>
    inline logger_stream& operator<<(AnyType any)
    {
        _logTimeStamp();
        // print(_prefixs[NONE]); // <-- NONE is ""
        return _stream << any;
    }
    
    inline logger_stream& operator<<
        (logger_stream& (*_pf)(logger_stream&))
    {
        _logTimeStamp();
        return _stream << _pf;
    }

private:
    logger_file _out;
    logger_stream _stream;

    StringType _prefixs[PREFIX_COUNT];

    StringType _dtFormat;
    bool _bUseTimeStamp;

    StringType _getUTCTime()
    {
        time_t now = time(nullptr);
        tm *gmtm = gmtime(&now);
        return datetime::getUTCTime(gmtm, _dtFormat.c_str());
    }

    int _logTimeStamp()
    {
        if (!_bUseTimeStamp) return 0;
        return putTimeStamp();
    }

};

static logger<char> logstdout(filesystem::dout);
static logger<char> logstderr(filesystem::derr);
static logger<char> logstdnull(filesystem::dnull);

}
