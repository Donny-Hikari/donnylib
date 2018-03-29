#pragma once

#include <initializer_list>
#include <stdexcept>

namespace donny {

template<typename T>
class vector_view {

    typedef T& reference;
    typedef const T& const_reference;
    typedef T* pointer;
    typedef const T* const_pointer;

    T* _p = nullptr;
    std::size_t _sz = 0;

public:
    typedef const_pointer const_iterator;
    typedef pointer iterator;

    template<std::size_t N>
    vector_view(T (&arr)[N])
    {
        _p = arr;
        _sz = N;
    }

    vector_view(std::initializer_list<T> arr)
    {
        _p = arr.begin();
        _sz = arr.size();
    }

    vector_view(const vector_view &that)
    {
        _p = that._p;
        _sz = that._sz;
    }

    std::size_t size() const { return _sz; }

    pointer data() { return _p; }
    iterator begin() { return _p; }
    iterator end() { return _p + _sz; }
    reference operator[](std::size_t n) {
        return n < _sz ? _p[n] : throw std::out_of_range("vector_view");
    }

    const_pointer data() const { return _p; }
    const_iterator begin() const { return _p; }
    const_iterator end() const { return _p + _sz; }
    const_reference operator[](std::size_t n) const {
        return n < _sz ? _p[n] : throw std::out_of_range("vector_view");
    }

};

}
