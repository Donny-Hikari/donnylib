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

    typedef std::size_t size_t;

    T* _p = nullptr;
    size_t _sz = 0;

public:
    typedef const_pointer const_iterator;
    typedef pointer iterator;

    template<size_t N>
    vector_view(T (&arr)[N])
    {
        _p = arr;
        _sz = N;
    }

    vector_view(T* pt, size_t sz)
    {
        _p = pt;
        _sz = sz;
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

    size_t size() const { return _sz; }

    pointer data() { return _p; }
    iterator begin() { return _p; }
    iterator end() { return _p + _sz; }
    reference operator[](size_t n) {
        if (n >= _sz)
            throw std::out_of_range("vector_view");
        return _p[n];
        // This will cause memory access violation
        // return n < _sz ? _p[n] : throw std::out_of_range("vector_view");
    }

    const_pointer data() const {
         printf("CALL ME\n");  return _p; }
    const_iterator begin() const {
         printf("CALL ME\n");  return _p; }
    const_iterator end() const {
         printf("CALL ME\n");  return _p + _sz; }
    const_reference operator[](size_t n) const {
        printf("CALL ME\n"); 
        if (n >= _sz)
            throw std::out_of_range("vector_view");
        return _p[n];
    }

};

}
