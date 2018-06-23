
#define BOOST_TEST_MODULE polynomial

#include <boost/test/included/unit_test.hpp>

#include <donny/logger.hpp>
#include <donny/math/polynomial.hpp>

using namespace donny;
using namespace donny::math;

BOOST_AUTO_TEST_CASE( test_eval )
{
    polynomial p = polynomial().add(1, 0).add(1, 1) * polynomial().add(1, 1);

    logstdout << "p.str(): " << p.str() << endl;
    logstdout << "p.eval(1): " << p.eval(1) << endl;
    logstdout << "p.eval(2): " << p.eval(2) << endl;
    logstdout << "p.eval(10): " << p.eval(10) << endl;
}

BOOST_AUTO_TEST_CASE( test_parse )
{
    std::string sPolynomial = "-x + 1.34x^2 + 1/50 x^3 + 344(x+2)^2 + 2*-1";
    std::string sResult = "1374+1375x+345.34x^2+0.02x^3";
    polynomial p = polynomial::parse(sPolynomial, 'x');
    logstdout << "Parse result of " << sPolynomial << endl << p.str() << endl;
    BOOST_CHECK(sResult == p.str());

    sPolynomial = "-1/10(x(2-x)^2)^3";
    sResult = "-6.4x^3+19.2x^4-24x^5+16x^6-6x^7+1.2x^8-0.1x^9";
    p = polynomial::parse(sPolynomial, 'x');
    logstdout << "Parse result of " << sPolynomial << endl << p.str() << endl;
    BOOST_CHECK(sResult == p.str());
}

BOOST_AUTO_TEST_CASE( test_subsitute )
{
    polynomial p = polynomial::parse("0.5x + x^2", 'x');
    logstdout << "Polynomial: " << p.str() << endl;
    polynomial s = polynomial::parse("t+1", 't');
    logstdout << "Subsitution: " << s.str("t") << endl;
    polynomial res = p.subsitute(s);
    logstdout << "Subsitute with \"t+1\": " << res.str("t") << endl;

}
