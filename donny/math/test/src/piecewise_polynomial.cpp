
#define BOOST_TEST_MODULE piecewise_polynomial

#include <boost/test/included/unit_test.hpp>

#include <limits>
#include <donny/logger.hpp>
#include <donny/math/piecewise_polynomial.hpp>

using namespace donny;
using namespace donny::math;

BOOST_AUTO_TEST_CASE( test_str )
{
    typedef piecewise_polynomial::ValueType ValueType;
    typedef piecewise_polynomial::RangeType RangeType;

    const ValueType VALUE_MAX = std::numeric_limits<ValueType>::max();
    const ValueType VALUE_MIN = -std::numeric_limits<ValueType>::max();

    piecewise_polynomial pp;
    RangeType r;

    r = RangeType(true, VALUE_MIN, -100.0, false);
    pp.set(polynomial().add(3, 0).add(4.2, 2), r);

    r = RangeType(true, -100.0, 99.98, false);
    pp.set(polynomial().add(4.2, 2).add(352.45, 10).add(-12.0, 10), r);

    r = RangeType(true, 99.98, VALUE_MAX, true);
    pp.set(polynomial(), r);

    logstdout << endl << pp.str() << endl;
}

BOOST_AUTO_TEST_CASE( test_eval )
{
    typedef piecewise_polynomial::ValueType ValueType;
    typedef piecewise_polynomial::RangeType RangeType;

    const ValueType VALUE_MAX = std::numeric_limits<ValueType>::max();
    const ValueType VALUE_MIN = -std::numeric_limits<ValueType>::max();

    piecewise_polynomial pp;
    RangeType r;

    r = RangeType(true, VALUE_MIN, -10.0, true);
    pp.set(polynomial().add(3, 0).add(4.2, 2), r);

    r = RangeType(false, -10.0, 99.98, false);
    pp.set(polynomial().add(4.2, 2).add(352.45, 10).add(-12.0, 10), r);

    r = RangeType(true, 99.98, VALUE_MAX, true);
    pp.set(polynomial(), r);

    logstdout << "pp.eval(-10): " << pp.eval(-10) << endl;
}
