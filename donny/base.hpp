/**
 * donnylib - A lightweight library for c++
 * 
 * base.hpp - basic marco definition, classes and functions declaration
 * 
 * Author : Donny
 */

#pragma once

namespace donny {
	
// Define __WINOS__
#if __WIN32 || __WIN64
#define __WINOS__ 1
#endif


// Get length of an array.
template<typename T, size_t N>
constexpr size_t length_of_array(T (&arr)[N]) {
    return N;
}


// do some jobs before return the value.
#define RET_TRAP(_RET_STATEMENT_, _TRAP_STATEMENT_) \
	{ \
		auto _result = (_RET_STATEMENT_); \
		{ _TRAP_STATEMENT_; } \
		return _result; \
	}

// do some jobs before return the value. notice that this is a lambda statement.
#define TRAP_RET(_RET_STATEMENT_, _TRAP_STATEMENT_) \
	[&](){ \
		RET_TRAP(_RET_STATEMENT_, _TRAP_STATEMENT_) \
	}()


// auto return char* or wchar_t*
class AUTOAW_HELPER
{
public:
	constexpr AUTOAW_HELPER(char *str, wchar_t *wstr)
		: _str(str), _wstr(wstr)
	{
	}

	constexpr operator char*() { return _str; }
	constexpr operator wchar_t*() { return _wstr; }

private:
	char* _str;
	wchar_t* _wstr;
	
};
#define AUTO_AW(_CharType_, _str_) \
	( (_CharType_*) AUTOAW_HELPER(_str_, L##_str_) )


// deduce the specifier for printf or scanf
template<typename T, typename CharType = char>
struct DeduceSpecifier {
	constexpr const CharType* operator()(T obj)
	{ return AUTO_AW(CharType, ""); }
};
template<typename CharType>
struct DeduceSpecifier<unsigned char, CharType> {
	constexpr const CharType* operator()(unsigned char)
	{ return AUTO_AW(CharType, "0x%X"); }
};
template<typename CharType>
struct DeduceSpecifier<short, CharType> {
	constexpr const CharType* operator()(short)
	{ return AUTO_AW(CharType, "%hd"); }
};
template<typename CharType>
struct DeduceSpecifier<unsigned short, CharType> {
	constexpr const CharType* operator()(unsigned short)
	{ return AUTO_AW(CharType, "%hu"); }
};
template<typename CharType>
struct DeduceSpecifier<int, CharType> {
	constexpr const CharType* operator()(int)
	{ return AUTO_AW(CharType, "%d"); }
};
template<typename CharType>
struct DeduceSpecifier<unsigned int, CharType> {
	constexpr const CharType* operator()(unsigned int)
	{ return AUTO_AW(CharType, "%u"); }
};
template<typename CharType>
struct DeduceSpecifier<long, CharType> {
	constexpr const CharType* operator()(long)
	{ return AUTO_AW(CharType, "%ld"); }
};
template<typename CharType>
struct DeduceSpecifier<unsigned long, CharType> {
	constexpr const CharType* operator()(unsigned long)
	{ return AUTO_AW(CharType, "%lu"); }
};
template<typename CharType>
struct DeduceSpecifier<long long, CharType> {
	constexpr const CharType* operator()(long long)
	{ return AUTO_AW(CharType, "%lld"); }
};
template<typename CharType>
struct DeduceSpecifier<unsigned long long, CharType> {
	constexpr const CharType* operator()(unsigned long long)
	{ return AUTO_AW(CharType, "%llu"); }
};
template<typename CharType>
struct DeduceSpecifier<double, CharType> {
	constexpr const CharType* operator()(double)
	{ return AUTO_AW(CharType, "%f"); }
};
template<typename CharType>
struct DeduceSpecifier<long double, CharType> {
	constexpr const CharType* operator()(long double)
	{ return AUTO_AW(CharType, "%Lf"); }
};
template<typename CharType>
struct DeduceSpecifier<bool, CharType> {
	constexpr const CharType* operator()(bool obj)
	{
		if (obj) return AUTO_AW(CharType, "True");
		else return AUTO_AW(CharType, "False");
	}
};
template<typename CharType>
struct DeduceSpecifier<char, CharType> {
	constexpr const CharType* operator()(char)
	{ return AUTO_AW(CharType, "%c"); }
};
template<typename CharType>
struct DeduceSpecifier<wchar_t, CharType> {
	constexpr const CharType* operator()(wchar_t)
	{ return AUTO_AW(CharType, "%lc"); }
};
template<typename CharType>
struct DeduceSpecifier<char*, CharType> {
	constexpr const CharType* operator()(char*)
	{ return AUTO_AW(CharType, "%s"); }
};
template<typename CharType>
struct DeduceSpecifier<wchar_t*, CharType> {
	constexpr const CharType* operator()(wchar_t*)
	{ return AUTO_AW(CharType, "%ls"); }
};

template<typename T, typename CharType = char>
constexpr const CharType* deduceSpecifier() {
	T obj;
	return DeduceSpecifier<T, CharType>()(obj);
}
template<typename T, typename CharType = char>
constexpr const CharType* deduceSpecifier(T obj) {
	return DeduceSpecifier<T, CharType>()(obj);
}


// A simple string
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

}
