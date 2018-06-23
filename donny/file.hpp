/**
 * donnylib - A lightweight library for c++
 * 
 * file.hpp - basic_file
 * dependency : base.hpp
 * 
 * Author : Donny
 */

#pragma once

#include <cstdio>
#include <cstdarg>
#include <cwchar>
#include <string>

#include "base.hpp"

namespace donny {

namespace filesystem {

// Usage: basic_file::write(&UTF16LEHeader);
const char16_t UTF16BEHeader = (char16_t)0xFEFF; // =u'\xfe\xff'

// Usage: basic_file::write(&UTF16LEHeader);
const char16_t UTF16LEHeader = (char16_t)0xFFFE; // =u'\xff\xfe'

template<typename T>
struct EOFDeterminer
{
};
template<>
struct EOFDeterminer<char>
{
	typedef int ValType;
	constexpr static ValType Val() { return EOF; }
};
template<>
struct EOFDeterminer<wchar_t>
{
	typedef uint ValType;
	constexpr static ValType Val() { return WEOF; }
};
template<>
struct EOFDeterminer<char16_t>
{
	typedef ushort ValType;
	constexpr static ValType Val() { return 0xffffu; }
};
template<>
struct EOFDeterminer<char32_t>
{
	typedef uint ValType;
	constexpr static ValType Val() { return WEOF; }
};

template<typename CharType>
class basic_file
{
	typedef const char* const_str;
	typedef const wchar_t* const_wstr;

	const typename EOFDeterminer<CharType>::ValType
		EOFValue = EOFDeterminer<CharType>::Val();

public:

	using SizeType = long;
	using StringType = std::basic_string<CharType>;
	using BufStringType = donny::simple_string<CharType>;

	enum SeekOrigin { begin = SEEK_SET, current = SEEK_CUR, end = SEEK_END };

	inline basic_file()
	{
	}
	inline basic_file(const_str filename, const_str mode) : basic_file()
	{
		open(filename, mode);
	}
	inline ~basic_file()
	{
		close();
	}

	inline basic_file(const basic_file &that) : basic_file()
	{
		operator=(that);
	}
	inline basic_file & operator=(const basic_file &that)
	{
		close();
		_pFile = that._pFile;
		if (_pFile) ++_pFile->_refCount;
		return *this;
	}
	
	inline basic_file(FILE* cfile)
	{
		if (cfile == nullptr) return;
		_pFile = new FileStruct;
		_SetFile(cfile);
		_pFile->_refCount = 2; // Cause there already has another
							   // reference to this file
	}
	inline FILE* getFILE()
	{
		return _File();
	}

	inline SizeType file_size() const
	{
		if (!is_open()) return 0;
		SizeType curpos = ftell(_File());
		fseek(_File(), 0, end);
		SizeType size = ftell(_File());
		fseek(_File(), 0, begin);
		size -= ftell(_File);
		fseek(_File(), curpos, current);
		return size;
	}

	inline bool open(const_str filename, const_str mode)
	{
		close();
		
		_pFile = new FileStruct;

#ifdef __WINOS__
		FILE *f = nullptr;
		fopen_s(&f, filename, mode);
		_SetFile(f);
#else
		_SetFile(fopen(filename, mode));
#endif
		if (!_File()) return false;
		
		++_pFile->_refCount;

		return true;
	}
	inline bool close()
	{
		bool bSucceed = true;
		if (_pFile && --_pFile->_refCount <= 0)
		{
			if (_File() != nullptr)
			{
				bSucceed = (fclose(_File()) == 0) ? true : false;
				_SetFile(nullptr);
				delete _pFile;
			}
		}
		_pFile = nullptr;
		return bSucceed;
	}
	
	inline bool is_open() const
	{
		if (_File()) return true;
		else return false;
	}
	inline bool eof() const
	{
		if ((_File() != nullptr) && (feof(_File()) == 0)) return false;
		return true;
	}
	inline int error() const
	{
		return ferror(_File());
	}
	inline void clearerr()
	{
		::clearerr(_File());
	}

	inline SizeType tell() const
	{
		return ftell(_File());
	}
	inline bool seek(SizeType offset, SeekOrigin origin)
	{
		if (fseek(_File(), offset, origin)) return false;
		else return true;
	}
	inline void rewind()
	{
		return ::rewind(_File());
	}

	inline CharType getc()
	{
		CharType c;
		if (read(&c) != 1) c = EOFValue;
		return c;
	}
	inline CharType getc(CharType& c)
	{
		return c = getc();
	}
	inline CharType putc(CharType c)
	{
		return (write(&c) == 1) ? (c) : EOFValue;
	}
	inline StringType gets(SizeType n)
	{
		BufStringType buf(n);
		read(buf.data(), n); // fgets(buf, n, _File());
		return buf.data();
	}
	inline StringType gets(CharType endChar = '\0', bool bIncludeEndChar = true)
	{
		StringType buf;
		CharType c = EOFValue;
		while (((c = getc()) != EOFValue) && (c != endChar))
			buf += c;
		if ((bIncludeEndChar) && (c != EOFValue)) buf += c;
		return buf;
	}
	inline uint puts(const StringType src, bool bWithBlankChar = false)
	{
		SizeType n = src.length() + ((bWithBlankChar) ? 1 : 0);
		return write(src.c_str(), n);
	}
	inline uint puts(const StringType src, SizeType n)
	{
		if (n > (SizeType)src.length() + 1)
			n = (SizeType)src.length() + 1;
		return write(src.c_str(), n);
	}
	inline uint puts(const StringType src, SizeType offset, SizeType n)
	{
		if (offset > (SizeType)src.length() + 1)
			return 0;
		if (n + offset > (SizeType)src.length() + 1)
			n = (SizeType)src.length() + 1 - offset;
		return write(src.c_str() + offset, n);
	}

	template<typename T>
	inline uint read(T *dest, uint count = 1)
	{
		return read(dest, sizeof(T), count);
	}
	inline uint read(void *dest, uint elementSize, uint count)
	{
		return fread(dest, elementSize, count, _File());
	}

	template<typename T>
	inline uint write(const T *src, uint count = 1)
	{
		return write(src, sizeof(T), count);
	}
	inline uint write(const void *src, uint elementSize, uint count)
	{
		return fwrite(src, elementSize, count, _File());
	}

	inline int vscanf(const StringType format_, va_list args_);
	inline int scanf(const StringType format_, ...);

    inline int vprint(const StringType format_, va_list args_);
    inline int print(const StringType format_, ...);

	inline int flush()
	{
		return fflush(_File());
	}
	
#ifdef __WINOS__
	const CharType lineBreak[3] = { '\r', '\n', '\0' };
#elif __linux__
	const CharType lineBreak[2] = { '\n', '\0' };
#else // __MACH__
	const CharType lineBreak[2] = { '\r', '\0' };
#endif
	const int nLineBreak = length_of_array(lineBreak) - 1;

	uint newLine()
	{
		return write(lineBreak, nLineBreak);
	}
	bool isNewLine(bool bEat = true)
	{
		CharType nxt[nLineBreak];
		bool bNewLine = true;

		uint nFed = read(nxt, nLineBreak);
		if (nFed != nLineBreak)
			bNewLine = false;

		for (int ind = 0; bNewLine && ind < nLineBreak; ++ind)
			if (nxt[ind] != lineBreak[ind])
				bNewLine = false;
		
		if (!bNewLine || !bEat)
			seek(-nFed, current);

		return bNewLine;
	}

private:
	struct FileStruct
	{
		FILE *_file = nullptr;
		uint _refCount = 1;
	} *_pFile = nullptr;

	FILE * _File() const
	{
		if (_pFile == nullptr) return nullptr;
		return _pFile->_file;
	}
	void _SetFile(FILE *file_)
	{
		if (_pFile == nullptr || _pFile->_file) return;
		_pFile->_file = file_;
	}

};

typedef basic_file<char> file;
typedef basic_file<wchar_t> wfile;
typedef basic_file<char16_t> u16file;
typedef basic_file<char32_t> u32file;

static file din(stdin);
static file dout(stdout);
static file derr(stderr);

static file dnull("/dev/null", "wb");
static wfile dwnull("/dev/null", "wb");

template<>
inline int file::vscanf(const file::StringType format_, va_list args_)
{
	return vfscanf(_File(), format_.c_str(), args_);
}
template<>
inline int file::scanf(const file::StringType format_, ...)
{
	va_list args; va_start(args, format_);
	return TRAP_RET( vscanf(format_, args), va_end(args) );
}
template<>
inline int file::vprint(const file::StringType format_, va_list args_)
{
	// return vfprintf(_File(), format_.c_str(), args_);
	va_list args;
	va_copy(args,args_);
	int sz = vfprintf(dnull.getFILE(), format_.c_str(), args);
	va_end(args);
	char buf[sz+1];
	vsnprintf(buf, sz+1, format_.c_str(), args_);
	return write(buf, sz);
}
template<>
inline int file::print(const file::StringType format_, ...)
{
	va_list args; va_start(args, format_);
	return TRAP_RET( vprint(format_, args), va_end(args) );
}


template<>
inline int wfile::vscanf(const wfile::StringType format_, va_list args_)
{
	return vfwscanf(_File(), format_.c_str(), args_);
}
template<>
inline int wfile::scanf(const wfile::StringType format_, ...)
{
	va_list args; va_start(args, format_);
	return TRAP_RET( vscanf(format_, args), va_end(args) );
}
template<>
inline int wfile::vprint(const wfile::StringType format_, va_list args_)
{
	va_list args;
	va_copy(args,args_);
	int sz = vfwprintf(dwnull.getFILE(), format_.c_str(), args);
	va_end(args);
	wchar_t buf[sz+1];
	vswprintf(buf, sz+1, format_.c_str(), args_);
	return write(buf, sz);
	// return vfwprintf(_File(), format_.c_str(), args_);
}
template<>
inline int wfile::print(const wfile::StringType format_, ...)
{
	va_list args; va_start(args, format_);
	return TRAP_RET( vprint(format_, args), va_end(args) );
}

}
}
