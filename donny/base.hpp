#pragma once

namespace donny {
	
// Define __WINOS__
#if __WIN32 || __WIN64
#define __WINOS__ 1
#endif

// Get length of an array
template<typename T, size_t N>
constexpr size_t length_of_array(T (&arr)[N]) {
    return N;
}

#define RET_TRAP(_RET_STATEMENT_, _TRAP_STATEMENT_) \
	{ \
		auto _result = (_RET_STATEMENT_); \
		{ _TRAP_STATEMENT_; } \
		return _result; \
	}

#define TRAP_RET(_RET_STATEMENT_, _TRAP_STATEMENT_) \
	[&](){ \
		RET_TRAP(_RET_STATEMENT_, _TRAP_STATEMENT_) \
	}()


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
