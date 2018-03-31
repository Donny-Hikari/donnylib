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

// Deduce the specifier for printf or scanf
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
