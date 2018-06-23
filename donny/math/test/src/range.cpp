
#define BOOST_TEST_MODULE range

#include <boost/test/included/unit_test.hpp>

#include <donny/math/range.hpp>

using namespace donny::math;

BOOST_AUTO_TEST_CASE( test_constructor )
{
    typedef double ValueType;
    
    const ValueType left = -1.0, right = 2.3;
    const bool includedLeft = true, includedRight= false;

    range<ValueType> r(includedLeft, left, right, includedRight);
    
    BOOST_REQUIRE( r.left == left );
    BOOST_REQUIRE( r.right == right );
    BOOST_REQUIRE( r.includedLeft == includedLeft );
    BOOST_REQUIRE( r.includedRight == includedRight );
}

BOOST_AUTO_TEST_CASE( test_empty )
{
    range<double> r;

    // Empty
    r = range<double>(false, 0.0, 0.0, false);
    BOOST_CHECK( r.empty() );

    r = range<double>(false, -0.0, 0.0, false);
    BOOST_CHECK( r.empty() );

    r = range<double>(true, 0, 0, false);
    BOOST_CHECK( r.empty() );

    // Not empty
    r = range<double>(true, 0.0, 0.0, true);
    BOOST_CHECK( !r.empty() );

    r = range<double>(true, -0.0, 0.0, true);
    BOOST_CHECK( !r.empty() );

    r = range<double>(false, -100.0, 7.0, false);
    BOOST_CHECK( !r.empty() );

    r = range<double>(false, -0.000001, 0.0, false);
    BOOST_CHECK( !r.empty() );

    r = range<double>(true, -100.0, 70.0, false);
    BOOST_CHECK( !r.empty() );

    r = range<double>(false, -70.0, 100.0, true);
    BOOST_CHECK( !r.empty() );
}

BOOST_AUTO_TEST_CASE( test_defalut_constructor )
{
    range<double> r;
    BOOST_CHECK( r.empty() );
}
