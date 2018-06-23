/**
 * donnylib - A lightweight library for c++
 * 
 * polynomial.hpp - a non-negative integer exponent polynomial class.
 * dependency: format_string
 * 
 * Author : Donny
 */

#pragma once

#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <donny/format_string.hpp>

namespace donny {
namespace math {


template<typename ValueType>
class basic_polynomial
{
public:
    basic_polynomial() {}
    explicit basic_polynomial(ValueType coefficient, int exponent = 0)
    {
        *this = basic_polynomial().add(coefficient, exponent);
    }

    basic_polynomial add(ValueType coefficient, int exponent) const
    {
        basic_polynomial p = *this;
        if (p.coefficients.size() <= exponent)
        {
            p.coefficients.resize(exponent+1);
        }
        p.coefficients[exponent] += coefficient;
        return p;
    }

    ValueType get(int exponent) const
    {
        if (exponent >= coefficients.size()) return (ValueType)0;
        return coefficients[exponent];
    }

    ValueType eval(ValueType x) const
    {
        ValueType powx = 1;
        ValueType result = 0;
        for (int ind = 0; ind < coefficients.size(); ++ind)
        {
            result += powx * coefficients[ind];
            powx *= x;
        }
        return result;
    }

    basic_polynomial subsitute(const basic_polynomial& p) const
    {
        basic_polynomial powp(1);
        basic_polynomial result;
        for (int ind = 0; ind < coefficients.size(); ++ind)
        {
            result = result + powp * coefficients[ind];
            powp = powp * p;
        }
        return result;
    }

    std::string str(std::string variable = "x") const
    {
        // TODO: consider percise problem

        format_string fs;
        bool bFirst = true;

        for (int ind = 0; ind < coefficients.size(); ++ind)
        {
            if (coefficients[ind] == (ValueType)0) continue;
            
            if (bFirst) bFirst = false;
            else if (coefficients[ind] > (ValueType)0) fs << '+';

            if (ind == 0)
                fs << coefficients[ind];
            else
            {
                if (coefficients[ind] == -1)
                    fs << '-';
                else if (coefficients[ind] != 1)
                    fs << coefficients[ind];
                fs << variable;
                if (ind > 1)
                    fs << "^" << ind;
            }
        }

        std::string s = fs.str();
        return s.empty() ? "0" : s;
    }

    void shrink()
    {
        int last = coefficients.size()-1;
        for (; last >= 0; --last)
            if (coefficients[last] != (ValueType)0) break;
        coefficients.erase(coefficients.begin()+last+1, coefficients.end());
    }

    basic_polynomial operator-()
    {
        basic_polynomial p;
        p.coefficients.resize(coefficients.size());
        for (int ind = 0; ind < coefficients.size(); ++ind)
        {
            p.coefficients[ind] = -coefficients[ind];
        }
        return p;
    }

    basic_polynomial operator=(const basic_polynomial& o)
    {
        coefficients = o.coefficients;
        return *this;
    }

    static basic_polynomial parse(const std::string sPolynomial, const char variable)
    {
        return parse(sPolynomial, variable, 0, sPolynomial.size());
    }

    template<typename _ValueType>
    friend
    basic_polynomial<_ValueType> operator+(
        const basic_polynomial<_ValueType>& a,
        const basic_polynomial<_ValueType>& b
    );

    template<typename _ValueType>
    friend
    basic_polynomial<_ValueType> operator-(
        const basic_polynomial<_ValueType>& a,
        const basic_polynomial<_ValueType>& b
    );

    template<typename _ValueType>
    friend
    basic_polynomial<_ValueType> operator*(
        const basic_polynomial<_ValueType>& a,
        const basic_polynomial<_ValueType>& b
    );

    template<typename _ValueType>
    friend
    basic_polynomial<_ValueType> operator*(
        const basic_polynomial<_ValueType>& a,
        const _ValueType b
    );

    template<typename _ValueType>
    friend
    basic_polynomial<_ValueType> operator/(
        const basic_polynomial<_ValueType>& m,
        const _ValueType d
    );

    template<typename _ValueType>
    friend
    basic_polynomial<_ValueType> operator^(
        const basic_polynomial<_ValueType>& p,
        const int e
    );

    template<typename _ValueType>
    friend
    bool operator==(
        const basic_polynomial<_ValueType>& a,
        const basic_polynomial<_ValueType>& b
    );

    template<typename _ValueType>
    friend
    bool operator!=(
        const basic_polynomial<_ValueType>& a,
        const basic_polynomial<_ValueType>& b
    );

private:
    std::vector<ValueType> coefficients;

    static basic_polynomial parse(
        const std::string sPolynomial,
        const char variable,
        const int pBegin,
        const int pEnd
    );
};

template<typename ValueType>
basic_polynomial<ValueType> operator+(
    const basic_polynomial<ValueType>& a,
    const basic_polynomial<ValueType>& b
)
{
    const basic_polynomial<ValueType>& shorter = (a.coefficients.size() <= b.coefficients.size()) ? a : b;
    const basic_polynomial<ValueType>& longer = (a.coefficients.size() > b.coefficients.size()) ? a : b;
    basic_polynomial<ValueType> p = longer;
    for (int ind = 0; ind < shorter.coefficients.size(); ++ind)
        p = p.add(shorter.coefficients[ind], ind);
    return p;
}

template<typename ValueType>
basic_polynomial<ValueType> operator-(
    const basic_polynomial<ValueType>& a,
    const basic_polynomial<ValueType>& b
)
{
    const basic_polynomial<ValueType>& shorter = (a.coefficients.size() <= b.coefficients.size()) ? a : b;
    const basic_polynomial<ValueType>& longer = (a.coefficients.size() > b.coefficients.size()) ? a : b;
    basic_polynomial<ValueType> p = longer;
    for (int ind = 0; ind < shorter.coefficients.size(); ++ind)
        p = p.add(-shorter.coefficients[ind], ind);
    return p;
}

template<typename ValueType>
basic_polynomial<ValueType> operator*(
    const basic_polynomial<ValueType>& a,
    const basic_polynomial<ValueType>& b
)
{
    basic_polynomial<ValueType> p;
    for (int ia = 0; ia < a.coefficients.size(); ++ia)
    {
        for (int ib = 0; ib < b.coefficients.size(); ++ib)
            p = p.add(a.coefficients[ia] * b.coefficients[ib], ia + ib);
    }
    return p;
}

template<typename ValueType>
basic_polynomial<ValueType> operator*(
    const basic_polynomial<ValueType>& a,
    const ValueType b
)
{
    basic_polynomial<ValueType> p = a;
    for (int ind = 0; ind < p.coefficients.size(); ++ind)
        p.coefficients[ind] = p.coefficients[ind] * b;
    return p;
}

template<typename ValueType>
basic_polynomial<ValueType> operator/(
    const basic_polynomial<ValueType>& m,
    const ValueType d
)
{
    basic_polynomial<ValueType> p = m;
    for (int ind = 0; ind < p.coefficients.size(); ++ind)
        p.coefficients[ind] = p.coefficients[ind] / d;
    return p;
}

template<typename ValueType>
basic_polynomial<ValueType> operator^(
    const basic_polynomial<ValueType>& _p,
    const int _e
)
{
    if (_e < 0)
        throw std::invalid_argument("exponent can only be non-negative");

    if (_e == 0)
        return basic_polynomial<ValueType>(1);

    basic_polynomial<ValueType> p = _p;
    int e = _e - 1;
    while (e >= 1)
    {
        if (e % 2 == 0)
        {
            e /= 2;
            p = p * p;
        }
        else
        {
            e -= 1;
            p = p * _p;
        }
    }

    return p;
}

template<typename ValueType>
bool operator==(
    const basic_polynomial<ValueType>& a,
    const basic_polynomial<ValueType>& b
)
{
    const basic_polynomial<ValueType>& shorter =
        (a.coefficients.size() < b.coefficients.size()) ? a : b;
    const basic_polynomial<ValueType>& longer =
        (a.coefficients.size() < b.coefficients.size()) ? b : a;

    for (int ind = 0; ind < shorter.coefficients.size(); ++ind)
    {
        if (shorter.coefficients[ind] != longer.coefficients[ind])
            return false;
    }

    for (int ind = shorter.coefficients.size(); ind < longer.coefficients.size(); ++ind)
    {
        if (longer.coefficients[ind] != 0)
            return false;
    }

    return true;
}

template<typename ValueType>
bool operator!=(
    const basic_polynomial<ValueType>& a,
    const basic_polynomial<ValueType>& b
)
{
    return !(a == b);
}

template<typename ValueType>
basic_polynomial<ValueType> basic_polynomial<ValueType>::parse(
    const std::string sPolynomial,
    const char variable,
    const int pBegin,
    const int pEnd
)
{
#define THROW_INVALID_SYMBOL(msg, pos) \
    throw std::invalid_argument(format_string() << \
        msg << ": \'" << sPolynomial[pos] << '\'' << \
        " at position " << (pos) << " .")

#define GET_NEXT_OP \
    [&]() { \
        while (ind+1 < pEnd && sPolynomial[ind+1] == ' ') ++ind; \
        if (ind+1 >= pEnd) return '\0'; \
        switch(sPolynomial[ind+1]) \
        { \
        case '+': \
        case '-': \
        case '*': \
        case '/': \
        case '^': \
            if (ind+2 >= pEnd) \
                THROW_INVALID_SYMBOL("expect an operand", ind+1); \
            ++ind; \
            return sPolynomial[ind]; \
        case '(': \
            return '*'; \
        default: \
            THROW_INVALID_SYMBOL("expect an operator", ind+1); \
        } \
    }

    typedef basic_polynomial<ValueType> _polynomial;
    struct item
    {
        _polynomial p;
        char op; // operation
        int pos_start;
    };

    if (!std::isalpha(variable))
        throw std::invalid_argument("variable should be a letter");
    if (sPolynomial.empty())
        throw std::invalid_argument("parsing empty string");

    std::vector<item> poly_items;

    // Convert string to struct item objects
    int sign = 0; // 0: not specify; 1: positive; -1: negative;
    int pos_start = 0;
    int parentheses_cnt = 0;
    item next_item;
    for (int ind = pBegin; ind < pEnd; ++ind)
    {
        switch (sPolynomial[ind])
        {
        case '+':
            if (sign == 0) sign = 1;
            else
                THROW_INVALID_SYMBOL("unexpected symbol", ind);
            break; // do nothing
        case '-':
            if (sign == 0) sign = -1;
            else
                THROW_INVALID_SYMBOL("unexpected symbol", ind);
            break;
        case ' ':
            break; // do nothing
        case '(':
            pos_start = ind; parentheses_cnt = 1; ++ind;
            while (ind < pEnd &&
                   parentheses_cnt != 0)
            {
                if (sPolynomial[ind] == '(') ++parentheses_cnt;
                else if (sPolynomial[ind] == ')') --parentheses_cnt;
                ++ind;
            }
            --ind;
            if (ind >= pEnd)
                THROW_INVALID_SYMBOL("parentheses not closed", pos_start);
            if (ind-pos_start-1 <= 0)
                THROW_INVALID_SYMBOL("empty parentheses not allowed", pos_start);
            next_item.pos_start = pos_start;
            next_item.p = parse(sPolynomial, variable, pos_start+1, ind);
            if (sign == -1) next_item.p = -next_item.p;
            next_item.op = GET_NEXT_OP();
            poly_items.push_back(next_item);
            sign = 0;
            break;
        case '*':
        case '/':
        case ')':
        case '^':
            THROW_INVALID_SYMBOL("unexpected symbol", ind);
            break;
        default:
            if (sPolynomial[ind] == variable)
            {
                next_item.pos_start = ind;
                next_item.p = _polynomial().add((sign == -1) ? -1 : 1, 1);
                next_item.op = GET_NEXT_OP();
                poly_items.push_back(next_item);
                sign = 0;
            }
            else if (std::isdigit(sPolynomial[ind]) || sPolynomial[ind] == '.')
            {
                int pPoint = -1;
                double val = 0;
                pos_start = ind;
                while (ind < pEnd &&
                       (std::isdigit(sPolynomial[ind]) || sPolynomial[ind] == '.'))
                {
                    if (sPolynomial[ind] == '.')
                    {
                        if (pPoint == -1) pPoint = ind;
                        else
                            THROW_INVALID_SYMBOL("unexpected symbol", ind);
                    }
                    else
                    {
                        val = val * 10 + (sPolynomial[ind] - '0');
                    }
                    ++ind;
                }
                --ind;
                if (pPoint != -1 && ind - pos_start + 1 <= 1)
                    THROW_INVALID_SYMBOL("unexpected symbol", pos_start);
                if (pPoint != -1)
                    val = val * std::pow(0.1, ind - pPoint);
                next_item.pos_start = pos_start;
                next_item.p = _polynomial((sign == -1) ? -val : val);
                while (ind+1 < pEnd && sPolynomial[ind+1] == ' ') ++ind;
                if (ind+1 < pEnd && sPolynomial[ind+1] == variable)
                    next_item.op = '*';
                else
                    next_item.op = GET_NEXT_OP();
                poly_items.push_back(next_item);
                sign = 0;
            }
            else
                THROW_INVALID_SYMBOL("unrecognized symbol", ind);
        } // switch
    } // for

    if (poly_items.rbegin()->op != '\0')
        throw std::logic_error("unknown error (0x01)");

    // Calculate power
    for (int ind = 0; ind < poly_items.size(); )
    {
        if (poly_items[ind].op == '^')
        {
            auto& C2s = poly_items[ind+1].p.coefficients;
            if (C2s.size() > 1)
                THROW_INVALID_SYMBOL(
                    "polynomial can't have variables in the exponent",
                    poly_items[ind+1].pos_start
                );
            if (C2s.size() == 1)
                poly_items[ind].p = poly_items[ind].p ^ C2s[0];
            else // C2s.size() == 0
                poly_items[ind].p = _polynomial(1);
            
            poly_items[ind].p.shrink();
            poly_items[ind].op = poly_items[ind+1].op;
            poly_items.erase(poly_items.begin()+ind+1);
        }
        else ++ind;
    }

    // Calculate multiply and divide
    for (int ind = 0; ind < poly_items.size(); )
    {
        if (poly_items[ind].op == '*')
        {
            poly_items[ind].p = poly_items[ind].p * poly_items[ind+1].p;

            poly_items[ind].p.shrink();
            poly_items[ind].op = poly_items[ind+1].op;
            poly_items.erase(poly_items.begin()+ind+1);
        }
        else if (poly_items[ind].op == '/')
        {
            auto& C2s = poly_items[ind+1].p.coefficients;
            if (C2s.size() > 1)
                THROW_INVALID_SYMBOL(
                    "polynomial with variables in denominator is not supported",
                    poly_items[ind+1].pos_start
                );
            if (C2s.size() == 1)
                poly_items[ind].p = poly_items[ind].p / C2s[0];
            else // C2s.size() == 0
                poly_items[ind].p = poly_items[ind].p / (ValueType)0;

            poly_items[ind].p.shrink();
            poly_items[ind].op = poly_items[ind+1].op;
            poly_items.erase(poly_items.begin()+ind+1);
        }
        else ++ind;
    }

    // Calculate plus and sub
    for (int ind = 0; ind < poly_items.size(); )
    {
        switch (poly_items[ind].op)
        {
        case '+':
            poly_items[ind].p = poly_items[ind].p + poly_items[ind+1].p;

            poly_items[ind].p.shrink();
            poly_items[ind].op = poly_items[ind+1].op;
            poly_items.erase(poly_items.begin()+ind+1);
            break;
        case '-':
            poly_items[ind].p = poly_items[ind].p - poly_items[ind+1].p;

            poly_items[ind].p.shrink();
            poly_items[ind].op = poly_items[ind+1].op;
            poly_items.erase(poly_items.begin()+ind+1);
            break;
        default:
            ++ind;
        }
    }

    if (poly_items.size() != 1)
        throw std::logic_error("unknown error (0x02)");

    poly_items[0].p.shrink();
    return poly_items[0].p;

#undef THROW_INVALID_SYMBOL
#undef GET_NEXT_OP
}

typedef basic_polynomial<double> polynomial;


} // math
} // donny
