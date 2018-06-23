/**
 * donnylib - A lightweight library for c++
 * 
 * range.hpp - a class to describe range limit.
 * dependency: format_string
 * 
 * Author : Donny
 */

#pragma once

#include <vector>
#include <donny/format_string.hpp>

namespace donny {
namespace math {


template<typename ValueType>
struct range
{
    ValueType left = (ValueType)0;
    ValueType right = (ValueType)0;
    bool includedLeft = false;
    bool includedRight = false;

    range() {}
    range(bool _includedLeft, ValueType _left, ValueType _right, bool _includedRight)
    {
        left = _left;
        right = _right;
        includedLeft = _includedLeft;
        includedRight = _includedRight;
    }

    bool empty() const
    {
        return (left > right) || (left == right && !(includedLeft && includedRight));
    }

    /**
     * @return: -1: left to the range;
     *           0: included in the range;
     *           1: right to the range
     */
    int compare(ValueType v) const
    {
        if ((includedLeft && (v < left)) ||
            (!includedLeft && (v <= left)))
            return -1;
        if ((includedRight && (v > right)) ||
            (!includedRight && (v >= right)))
            return 1;
        return 0;
    }

    bool included(ValueType v) const
    {
        return compare(v) == 0;
    }

    ValueType length() const
    {
        return right - left;
    }

    std::string str() const
    {
        return format_string() << 
            (includedLeft ? "[" : "(") << left << "," << right << (includedRight ? "]" : ")");
    }

    template<typename _ValueType>
    friend
    bool operator<(
        const range<_ValueType>& a,
        const range<_ValueType>& b
    );

    template<typename _ValueType>
    friend
    bool operator==(
        const range<_ValueType>& a,
        const range<_ValueType>& b
    );

    template<typename _ValueType>
    friend
    bool operator!=(
        const range<_ValueType>& a,
        const range<_ValueType>& b
    );

    static std::vector<range> cut(const range& a,
                                  const range& b,
                                  bool removeBlankRange = true);

    enum relationship: int
    {
        A_RIGHT_INTERSECTED_B = -2, // b lies left of a
        B_LEFT_INTERSECTED_A = -2, // b lies left of a;
        A_LEFT_INTERSECTED_B = -1, // a lies left of b
        B_RIGHT_INTERSECTED_A = -1, // a lies left of b
        SEPARATED = 0,
        A_INCLUDED_B = 1, // a included b
        B_INCLUDED_A = 2, // b included a
        EQUAL = 3,
    };

    /**
     * @return: -2: intersected, b lies left of a;
     *          -1: intersected, a lies left of b;
     *           0: separated;
     *           1: a included b;
     *           2: b included a;
     *           3: a equals b;
     */
    static int relation(const range& a, const range& b);

    static bool is_neighbor(const range& a, const range& b);

private:
    static void push_back_not_empty(std::vector<range>& storage, range r);

};

template<typename ValueType>
void range<ValueType>::push_back_not_empty(std::vector<range<ValueType>>& storage, range<ValueType> r)
{
    if (!r.empty())
        storage.push_back(r);
}

template<typename ValueType>
inline std::vector<range<ValueType>> range<ValueType>::cut(const range<ValueType>& a,
                                                           const range<ValueType>& b,
                                                           bool removeBlankRange)
{
    std::vector<range<ValueType>> result;
    int relationship = relation(a, b);

    if (relationship > 0) // (a included b) || (b included a)
    {
        const range<ValueType>& outer = (relationship == A_INCLUDED_B) ? a : b;
        const range<ValueType>& inner = (relationship == A_INCLUDED_B) ? b : a;
        
        if (!removeBlankRange)
            push_back_not_empty(result, range<ValueType>(outer.includedLeft, outer.left, inner.left, !inner.includedLeft));
        push_back_not_empty(result, inner);
        if (!removeBlankRange)
            push_back_not_empty(result, range<ValueType>(!inner.includedRight, inner.right, outer.right, outer.includedRight));
    }
    else if (relationship < 0) // intersect, (a lies left of b) || (b lies left of a)
    {
        const range<ValueType>& left = (relationship == A_LEFT_INTERSECTED_B) ? a : b;
        const range<ValueType>& right = (relationship == A_LEFT_INTERSECTED_B) ? b : a;

        if (!removeBlankRange)
            push_back_not_empty(result, range<ValueType>(left.includedLeft, left.left, right.left, !right.includedLeft));
        push_back_not_empty(result, range<ValueType>(right.left, right.includedLeft, left.right, left.includedRight));
        if (!removeBlankRange)
            push_back_not_empty(result, range<ValueType>(!left.includedRight, left.right, right.right, right.includedRight));
    }
    else // separated
    {
        if (!removeBlankRange) {
            push_back_not_empty(result, a);
            push_back_not_empty(result, b);
        }
    }

    return result;
}

template<typename ValueType>
inline int range<ValueType>::relation(const range<ValueType>& a, const range<ValueType>& b)
{
    const bool aibl = a.included(b.left) || (a.left == b.left && !b.includedLeft),
               aibr = a.included(b.right) || (a.right == b.right && !b.includedRight),
               bial = b.included(a.left) || (b.left == a.left && !a.includedLeft),
               biar = b.included(a.right) || (b.right == a.right && !a.includedRight);
    if (aibl && aibr && bial && biar)
        return 3;
    if (aibl && aibr) // a included b
        return 1;
    if (bial && biar) // b included a
        return 2;
    if (aibl && biar) // intersect, a lies left of b
        return -1;
    if (bial && aibr) // intersect, b lies left of a
        return -2;
    return 0; // separated
}

template<typename ValueType>
inline bool range<ValueType>::is_neighbor(const range<ValueType>& a, const range<ValueType>& b)
{
    if (a.empty() || b.empty()) return false;
    if ((a.right == b.left) && (a.includedRight != b.includedLeft))
        return true;
    if ((a.left == b.right) && (a.includedLeft != b.includedRight))
        return true;
    return false;
}

template<typename ValueType>
bool operator<(
    const range<ValueType>& a,
    const range<ValueType>& b
)
{
    if (a.empty() || b.empty())
        throw std::logic_error("can't compare with empty range(s)");
    return (a.left < b.left) || (a.left == b.left && a.includedLeft && !b.includedLeft);
}

template<typename ValueType>
bool operator==(
    const range<ValueType>& a,
    const range<ValueType>& b
)
{
    return range<ValueType>::relation(a, b) == range<ValueType>::EQUAL;
}

template<typename ValueType>
bool operator!=(
    const range<ValueType>& a,
    const range<ValueType>& b
)
{
    return !(a == b);
}

    
} // math
} // donny
