
#pragma once

#include <string>
#include <stdexcept>
#include <algorithm>
#include <donny/format_string.hpp>
#include "polynomial.hpp"
#include "range.hpp"

namespace donny {
namespace math {


template<typename _ValueType>
class basic_piecewise_polynomial
{
public:
    typedef _ValueType ValueType;
    typedef range<ValueType> RangeType;

    basic_piecewise_polynomial& set(basic_polynomial<ValueType> p, RangeType r, bool bOverride=false)
    {
        if (r.empty()) return *this;

        for (polynomial_fragment frag : fragments)
        {
            if (RangeType::relation(frag.r, r) != 0) // intersected or included
            {
                if (!bOverride)
                    throw std::domain_error(format_string()
                        << "range " << r.str() << " conflict with original range " << frag.r.str());
                else
                {
                    if (frag.r < r)
                    {
                        frag.r.right = r.left;
                        frag.r.includedRight = !r.includedLeft;
                    }
                    else
                    {
                        frag.r.left = r.right;
                        frag.r.includedLeft = !r.includedLeft;
                    }
                }
            }
        }
        fragments.push_back(polynomial_fragment(p, r));
        
        return *this;
    }
    
    ValueType eval(ValueType x) const
    {
        for (const polynomial_fragment& frag : fragments)
        {
            if (frag.r.included(x))
            {
                return frag.p.eval(x);
            }
        }
        throw std::domain_error(format_string() << "value " << x << " is not in the range.");
    }

    std::string str(std::string variable = "x") const
    {
        std::stringstream ss;
        for (int ind = 0; ind < fragments.size(); ++ind)
        {
            ss << fragments[ind].r.str() << " : " << fragments[ind].p.str(variable);
            if (ind != fragments.size()) ss << std::endl;
        }
        return ss.str();
    }

    void shrink()
    {
        std::sort(fragments.begin(), fragments.end(), polynomial_fragment::cmp_range);

        for (int ind = 1; ind < fragments.size(); )
        {
            if (!polynomial_fragment::can_merge(fragments[ind-1], fragments[ind]))
                ++ind;
            else
            {
                fragments[ind-1].r.right = fragments[ind].r.right;
                fragments[ind-1].r.includedRight = fragments[ind].r.includedRight;
                fragments.erase(fragments.begin() + ind);
            }
        }
    }

    basic_piecewise_polynomial& operator=(const basic_piecewise_polynomial& o)
    {
        fragments = o.fragments;
        return *this;
    }

    template<typename __ValueType>
    friend
    basic_piecewise_polynomial<__ValueType> operator+(
        const basic_piecewise_polynomial<__ValueType>& a,
        const basic_piecewise_polynomial<__ValueType>& b
    );

    template<typename __ValueType>
    friend
    basic_piecewise_polynomial<__ValueType> operator*(
        const basic_piecewise_polynomial<__ValueType>& a,
        const basic_piecewise_polynomial<__ValueType>& b
    );

    template<typename __ValueType>
    friend
    bool operator==(
        const basic_piecewise_polynomial<__ValueType>& a,
        const basic_piecewise_polynomial<__ValueType>& b
    );

private:
    struct polynomial_fragment
    {
        basic_polynomial<ValueType> p;
        RangeType r;

        polynomial_fragment() {}
        polynomial_fragment(basic_polynomial<ValueType> _p, RangeType _r)
            : p(_p)
            , r(_r)
        {
        }

        static bool cmp_range(const polynomial_fragment& a, const polynomial_fragment& b)
        {
            return a.r < b.r;
        }

        static bool can_merge(const polynomial_fragment& a, const polynomial_fragment& b)
        {
            if ((a.p == b.p) && RangeType::is_neighbor(a.r, b.r))
                return true;
            return false;
        }
    };

    std::vector<polynomial_fragment> fragments;

};

typedef double ValueType;

template<typename ValueType>
basic_piecewise_polynomial<ValueType> operator+(
    const basic_piecewise_polynomial<ValueType>& a,
    const basic_piecewise_polynomial<ValueType>& b
)
{
    typedef typename basic_piecewise_polynomial<ValueType>::RangeType RangeType;

    basic_piecewise_polynomial<ValueType> pp;
    for (auto& afrag : a.fragments)
    {
        for (auto& bfrag : b.fragments)
        {
            auto cutedRanges = RangeType::cut(afrag.r, bfrag.r, true);
            for (RangeType r : cutedRanges)
                pp.set(afrag.p + bfrag.p, r);
        }
    }
    return pp;
}

template<typename ValueType>
basic_piecewise_polynomial<ValueType> operator*(
    const basic_piecewise_polynomial<ValueType>& a,
    const basic_piecewise_polynomial<ValueType>& b
)
{
    typedef typename basic_piecewise_polynomial<ValueType>::RangeType RangeType;

    basic_piecewise_polynomial<ValueType> pp;
    for (auto& afrag : a.fragments)
    {
        for (auto& bfrag : b.fragments)
        {
            auto cutedRanges = RangeType::cut(afrag.r, bfrag.r, true);
            for (RangeType r : cutedRanges)
                pp.set(afrag.p * bfrag.p, r);
        }
    }
    return pp;
}

template<typename ValueType>
bool operator==(
    const basic_piecewise_polynomial<ValueType>& _a,
    const basic_piecewise_polynomial<ValueType>& _b
)
{
    typedef typename basic_piecewise_polynomial<ValueType>::RangeType RangeType;

    basic_piecewise_polynomial<ValueType> a = _a; a.shrink();
    basic_piecewise_polynomial<ValueType> b = _b; b.shrink();
    if (a.fragments.size() != b.fragments.size()) return false;

    for (int ind = 0; ind < a.fragments.size(); ++ind)
    {
        if ((a.fragments[ind].p != b.fragments[ind].p) ||
            (a.fragments[ind].r != b.fragments[ind].r))
            return false;
    }

    return true;
}

typedef basic_piecewise_polynomial<double> piecewise_polynomial;


}
}
