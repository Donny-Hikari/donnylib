#pragma once

#include <cstdio>
#include <string>

namespace donny {
	
#if __WIN32 || __WIN64
#define __WINOS__ 1
#endif

template<typename CharType>
class simple_string
{
	typedef CharType* pointer;
	typedef const CharType* const_pointer;
	typedef CharType& reference;
	typedef const CharType& const_reference;

	typedef long SizeType;

	pointer _data = nullptr;
	SizeType _size = 0;

public:
	simple_string()
	{
	}
	explicit simple_string(int n)
		: _data(new CharType[n]{0})
		, _size(n)
	{
	}
	~simple_string()
	{
		delete[] _data;
	}

	SizeType size() const
	{
		return _size;
	}

	pointer data()
	{
		return _data;
	}
	const_pointer data() const
	{
		return _data;
	}

	operator pointer()
	{
		return _data;
	}
	operator const_pointer() const
	{
		return _data;
	}

};

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

	using SizeType = long;
	using StringType = std::basic_string<CharType>;
	using BufStringType = donny::simple_string<CharType>;

	const typename EOFDeterminer<CharType>::ValType
		EOFValue = EOFDeterminer<CharType>::Val();

public:

#ifdef __WINOS__
	const CharType lineBreak[2] = { '\r', '\n' };
#elif __linux__
	const CharType lineBreak[1] = { '\n' };
#else // __MACH__
	const CharType lineBreak[1] = { '\r' };
#endif

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
	inline const FILE* getFILE()
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
	// template<typename T>
	// //Won't change the size of Dest.
	// inline uint read(DPointer<T>& Dest, uint Count = 1) const
	// {
	// 	return read(Dest, sizeof(T), Count);
	// }
	// template<typename T>
	// //Will change the size of Dest to Count.
	// inline uint read(DPointer<T>* pDest, uint Count = 1) const
	// {
	// 	if (!pDest) return 0;
	// 	pDest->New(Count);
	// 	return read(*pDest, Count);
	// }
	inline uint read(void *dest, uint elementSize, uint count)
	{
		return fread(dest, elementSize, count, _File());
	}
	template<typename T>
	inline uint write(const T *src, uint count = 1)
	{
		return write(src, sizeof(T), count);
	}
	// template<typename T>
	// inline uint write(const DPointer<T>& Src, uint Count = 1)
	// {
	// 	return write(Src, sizeof(T), Count);
	// }
	inline uint write(const void *src, uint elementSize, uint count)
	{
		return fwrite(src, elementSize, count, _File());
	}
	inline int flush()
	{
		return fflush(_File());
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

private:
	struct FileStruct
	{
		FILE *_file = nullptr;
		uint _refCount = 1;
	} *_pFile = nullptr;

	FILE * _File()
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

static file scrin(stdin);
static file scrout(stdout);

/*
template<typename CharType>
class basic_memfile
{
	typedef long SizeType;
	using StringType = std::basic_string<CharType>;

	CharType sBinaryWriteMode[] = { 'w', 'b' };
	CharType sBinaryReadMode[] = { 'r', 'b' };

public:
	enum SeekOrigin { begin = SEEK_SET, current = SEEK_CUR, end = SEEK_END };

	inline basic_memfile()
	{
	}
	inline basic_memfile(StringType filename, StringType mode) : basic_memfile()
	{
		open(filename, mode);
	}
	inline basic_memfile(const basic_memfile &file_) : basic_memfile()
	{
		operator=(file_);
	}
	inline ~basic_memfile()
	{
		close();
	}
	inline basic_memfile& operator=(const basic_memfile &file_)
	{
		if (!close()) return *this;
		_FileName = file_._FileName;
		_pFile = file_._pFile;
		_CurPos = file_._CurPos;
		_bFixSize = file_._bFixSize;
		_bWrite = file_._bWrite;
		if (_bWrite) ++_nWrite();
		return *this;
	}

	inline SizeType file_size() const
	{
		return _pData().Size();
	}
	inline bool isSaveToDisk() const
	{
		return _bSaveToDisk();
	}
	inline bool setSaveToDisk(bool bSaveToDisk)
	{
		_bSaveToDisk() = bSaveToDisk;
		return true;
	}
	inline bool saveToDisk()
	{
		basic_file<CharType> diskFile(_FileName, sBinaryWriteMode);
		if (!diskFile.is_open()) return false;
		diskFile.write(_pData(), file_size());
		diskFile.close();
		return true;
	}

	//Either read or write or append.
	//Mode[0] = 'r' | 'w' | 'a'
	inline bool create(const basic_memfile& file_, StringType mode)
	{
		if (!close()) return false;
		_pFile = file_._pFile;
		if (mode[0] == 'r')
		{
			_CurPos = 0;
			_bWrite = false;
		}
		else if (mode[0] == 'w')
		{
			++_nWrite();
			_CurPos = 0;
			_bWrite = true;
		}
		else if (mode[0] == 'a')
		{
			++_nWrite();
			_CurPos = file_.file_size();
			_bWrite = true;
		}
		_FileName = file_._FileName;
		_bFixSize = File_._bFixSize;
		return true;
	}
	//Either append or write.
	inline bool create(StringType filename, bool bAppend, SizeType fileSize, bool bFixSize)
	{
		if (!close()) return false;
		if (bAppend)
		{
			basic_file diskFile(filename, sBinaryReadMode);
			if (!diskFile.is_open()) return false;
			_CurPos = diskFile.file_size();
			if (fileSize < _CurPos) fileSize = _CurPos;
			_pData().New(fileSize);
			diskFile.read(_pData(), _CurPos);
			diskFile.close();
		}
		else
		{
			_pData().New(fileSize);
			_CurPos = 0;
		}
		_FileName = filename;
		_nWrite() = 1;
		_bFixSize = bFixSize;
		_bWrite = true;
		return true;
	}

	inline bool open(StringType filename, StringType mode)
	{
		if (!close()) return false;
		if ((mode[0] == 'r') || (mode[0] == 'a'))
		{
			basic_file diskFile(filename, L"rb");
			if (!diskFile.is_open()) return false;
			_pData().New(diskFile.file_size());
			diskFile.read(_pData(), _pData().Size());
			diskFile.close();
		}
		if (mode[0] == 'r')
		{
			_nWrite() = 0;
			_CurPos = 0;
			_bFixSize = true;
			_bWrite = false;
		}
		else if ((mode[0] == 'a') || (mode[0] == 'w'))
		{
			_nWrite() = 1;
			_CurPos = file_size();//When Mode[0]=='w', _CurPos=file_size()=0.
			_bFixSize = false;
			_bWrite = true;
		}
		else return false;
		_FileName = filename;
		return true;
	}
	inline bool close()
	{
		if ((_bSaveToDisk()) && (_bWrite))//_nWrite()>=1
		{
			if (_nWrite() > 1) --_nWrite();
			else saveToDisk();//_nWrite()==1
		}
		if (_RefCount() == 1) delete _pFile;
		_FileName = L"";
		_pFile = new DPMemFile;
		_CurPos = 0;
		_bFixSize = true;
		_bWrite = false;
		return true;
	}

	inline wchar getwc(wchar& CharType) const
	{
		if (read(&CharType) != 1) CharType = WEOF;
		return CharType;
	}
	inline char getc(char& CharType) const
	{
		if (read(&CharType) != 1) CharType = EOF;
		return CharType;
	}
	inline wchar getwc() const
	{
		wchar CharType;
		return getwc(CharType);
	}
	inline char getc() const
	{
		char CharType;
		return getc(CharType);
	}
	inline wchar putwc(wchar CharType)
	{
		return (write(&CharType) == 1) ? (CharType) : WEOF;
	}
	inline char putc(char CharType)
	{
		return (write(&CharType) == 1) ? (CharType) : EOF;
	}
	template<typename IniType>
	inline wString getws(IniType n) const
	{
		typedef typename DIniSize<IniType>::Type QualifiedIniType;
		wString Buf(n);
		read(Buf.StringType(), n);//fgetws(Buf, n, _File());
		return Buf;
	}
	template<typename IniType>
	inline StringType gets(IniType n) const
	{
		typedef typename DIniSize<IniType>::Type QualifiedIniType;
		StringType Buf(n);
		read(Buf.StringType(), n);//fgets(Buf, n, _File());
		return Buf;
	}
	inline wString getws(wchar EndChar = '\0', bool bIncludeEndChar = true) const
	{
		wString Buf;
		wchar CharType = WEOF;
		getwc(CharType);
		while ((CharType != WEOF) && (CharType != EndChar))
		{
			Buf += CharType;
			getwc(CharType);
		}
		if ((bIncludeEndChar) && (CharType != WEOF)) Buf += CharType;
		return Buf;
	}
	inline StringType gets(char EndChar = '\0', bool bIncludeEndChar = true) const
	{
		StringType Buf;
		char CharType = EOF;
		getc(CharType);
		while ((CharType != EOF) && (CharType != EndChar))
		{
			Buf += CharType;
			getc(CharType);
		}
		if ((bIncludeEndChar) && (CharType != EOF)) Buf += CharType;
		return Buf;
	}
	inline uint putws(const wString Src, bool bWithBlankChar = false)
	{
		DSize n = Src.length() + ((bWithBlankChar) ? 1 : 0);
		return write(Src.StringType(), n);
	}
	inline uint puts(const StringType Src, bool bWithBlankChar = false)
	{
		DSize n = Src.length() + ((bWithBlankChar) ? 1 : 0);
		return write(Src.StringType(), n);
	}
	template<typename IniType>
	inline uint putws(const wString Src, IniType n)
	{
		typedef typename DIniSize<IniType>::Type QualifiedIniType;
		if ((DSize)n > Src.length() + 1) n = (QualifiedIniType)(Src.length() + 1);
		return write(Src.StringType(), n);
	}
	template<typename IniType>
	inline uint puts(const StringType Src, IniType n)
	{
		typedef typename DIniSize<IniType>::Type QualifiedIniType;
		if ((DSize)n > Src.length() + 1) n = (QualifiedIniType)(Src.length() + 1);
		return write(Src.StringType(), n);
	}
	template<typename IniType>
	inline uint putws(const wString Src, IniType Offset, IniType n)
	{
		typedef typename DIniSize<IniType>::Type QualifiedIniType;
		if ((DSize)Offset > Src.length() + 1) Offset = (IniType)(Src.length() + 1);
		if ((DSize)(n + Offset) > Src.length() + 1) n = (IniType)(Src.length() + 1 - Offset);
		return write(Src.StringType() + Offset, n);
	}
	template<typename IniType>
	inline uint puts(const StringType Src, IniType Offset, IniType n)
	{
		typedef typename DIniSize<IniType>::Type QualifiedIniType;
		if ((DSize)Offset > Src.length() + 1) Offset = (IniType)(Src.length() + 1);
		if ((DSize)(n + Offset) > Src.length() + 1) n = (IniType)(Src.length() + 1 - Offset);
		return write(Src.StringType() + Offset, n);
	}

	template<typename T>
	inline uint read(T* Dest, uint Count = 1) const
	{
		return read(Dest, sizeof(T), Count);
	}
	template<typename T>
	//Won't change the size of Dest.
	inline uint read(DPointer<T>& Dest, uint Count = 1) const
	{
		return read(Dest, sizeof(T), Count);
	}
	template<typename T>
	//Will change the size of Dest to Count.
	inline uint read(DPointer<T>* pDest, uint Count = 1) const
	{
		if (!pDest) return 0;
		pDest->New(Count);
		return read(*pDest, Count);
	}
	inline uint read(void* pDest, uint ElementSize, uint Count) const
	{
		DSize i = 0;
		for (; i < ElementSize*Count; ++i)
			((DByte*)pDest)[i] = _pData()[_CurPos + i];
		_CurPos += ElementSize*Count;
		return i;
	}
	template<typename T>
	inline uint write(const T* Src, uint Count = 1)
	{
		return write(Src, sizeof(T), Count);
	}
	template<typename T>
	inline uint write(const DPointer<T>& Src, uint Count = 1)
	{
		return write(Src, sizeof(T), Count);
	}
	inline uint write(const void* Src, uint ElementSize, uint Count)
	{
		if (!_bWrite) return 0;
		DSize FileSize = file_size();
		DSize CountofByte = ElementSize*Count;
		if (FileSize <= _CurPos + CountofByte)
		{
			if (_bFixSize) CountofByte = FileSize - _CurPos;
			else
			{
				DPointer<DByte> Buffer(GetLargerValue(FileSize * 2, _CurPos + CountofByte));
				for (DSize i = 0; i < FileSize; ++i)
					Buffer[i] = _pData()[i];
				_pData() = Buffer;
			}
		}
		DSize i = 0;
		for (; i < CountofByte; ++i)
			_pData()[_CurPos + i] = ((DByte*)Src)[i];
		_CurPos += CountofByte;
		return i;
	}
	inline int flush()
	{
		return 0;
	}

	inline bool is_open() const
	{
#if DONNYLIBVERSION >= 2
		if (!_FileName.isEmpty()) return true;
#else
		if (!_FileName.IsEmpty()) return true;
#endif
		else return false;
	}
	inline bool eof() const
	{
		if (_CurPos < file_size()) return false;
		return true;
	}
	inline int error() const
	{
		return 0;
	}
	inline void clearerr()
	{
	}

	inline bool seek(DSize Offset, SeekOrigin Origin)
	{
		DSize FileSize = file_size();
		switch (Origin)
		{
		case begin:
			_CurPos = Offset;
			if (_CurPos <= FileSize) return true;
			_CurPos = FileSize;
			break;
		case current:
			_CurPos += Offset;
			if (_CurPos <= FileSize) return true;
			_CurPos = FileSize;
			break;
		case end:
			if (Offset <= FileSize)
			{
				_CurPos = FileSize - Offset;
				return true;
			}
			_CurPos = 0;
			break;
		}
		return false;
	}
	inline DSize tell() const
	{
		return _CurPos;
	}
	inline void rewind()
	{
		_CurPos = 0;
	}

private:
	wString _FileName;//Full Name
	struct DPMemFile
	{
		DPointer<DByte> _pData;
		DSize _nWrite = 0;
		bool _bSaveToDisk = true;
	}*_pFile = new DPMemFile;
	//Using _pData().RefCount();
	mutable DSize _CurPos = 0;
	bool _bFixSize = true;
	bool _bWrite = false;

	inline DPointer<DByte>& _pData()
	{
		return _pFile->_pData;
	}
	inline const DPointer<DByte>& _pData() const
	{
		return _pFile->_pData;
	}
	inline DSize& _nWrite()
	{
		return _pFile->_nWrite;
	}
	inline const DSize& _nWrite() const
	{
		return _pFile->_nWrite;
	}
	inline bool& _bSaveToDisk()
	{
		return _pFile->_bSaveToDisk;
	}
	inline const bool& _bSaveToDisk() const
	{
		return _pFile->_bSaveToDisk;
	}
	inline DSize _RefCount() const
	{
		return _pData().RefCount();
	}
};
*/

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
