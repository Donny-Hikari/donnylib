
#define BOOST_TEST_MODULE vector_view

#include <boost/test/included/unit_test.hpp>

#include <donny/base.hpp>
#include <donny/vector_view.hpp>

// using namespace donny;
using donny::length_of_array;
using donny::vector_view;

// Test array constructor
BOOST_AUTO_TEST_CASE( test_array_constructor )
{
    int arr[] = { 1, 2, 3, 4, 5 };
    vector_view<int> vv(arr);

    BOOST_REQUIRE(vv.data() == arr);
    BOOST_REQUIRE(vv.size() == length_of_array(arr));
}

// Test pointer constructor
BOOST_AUTO_TEST_CASE( test_pointer_constructor )
{
    int arr[] = { 1, 2, 3, 4, 5 };
    int *parr = arr;
    vector_view<int> vv(parr, length_of_array(arr));

    BOOST_CHECK(vv.data() == parr);
    BOOST_CHECK(vv.size() == length_of_array(arr));
}

// Test iterator: begin(), end()
// Require: array constructor
BOOST_AUTO_TEST_CASE( test_iterator )
{
    int arr[] = { 1, 2, 3, 4, 5 };
    vector_view<int> vv(arr);

    BOOST_CHECK(vv.begin() == arr);
    BOOST_CHECK(vv.end() == arr + length_of_array(arr));

    int ind = 0;
    for (int e : vv) {
        BOOST_CHECK(e == arr[ind]);
        ++ind;
    }
}

// Test operator[]
// Require: array constructor
BOOST_AUTO_TEST_CASE( test_operator_subscript )
{
    int arr[] = { 1, 2, 3, 4, 5 };
    vector_view<int> vv(arr);

    for (int i = 0; i < length_of_array(arr); ++i) {
        BOOST_CHECK(vv[i] == arr[i]);
    }
}

// Test initializer_list constructor
// Require: iterator
BOOST_AUTO_TEST_CASE( test_initializer_list_constructor )
{
    int arr[] = { 1, 2, 3, 4, 5 };
    vector_view<const int> vv = { 1, 2, 3, 4, 5 };

    BOOST_CHECK(vv.size() == length_of_array(arr));
    
    auto it = vv.begin();
    for (int i = 0; i < length_of_array(arr); ++i, ++it) {
        BOOST_CHECK(*it == arr[i]);
    }
}

// Test copy constructor
// Require: array constructor
BOOST_AUTO_TEST_CASE( test_copy_constructor )
{
    int arr[] = { 1, 2, 3, 4, 5 };
    vector_view<int> vv1(arr);
    vector_view<int> vv2(vv1);

    BOOST_CHECK(vv1.size() == vv2.size());

    for (int ind = 0; ind < vv1.size(); ++ind) {
        BOOST_CHECK(vv1[ind] == vv2[ind]);
    }
}

// Test operator[] with assigning new value
// Require: array constructor, iterator, operator[]
BOOST_AUTO_TEST_CASE( test_operator_subscript_with_changing_value )
{
    int arr[] = { 1, 2, 3, 4, 5 };
    vector_view<int> vv = arr;

    for (int &e : vv) {
        e += 1;
    }

    for (int ind = 0; ind < vv.size(); ++ind) {
        BOOST_CHECK(vv[ind] == arr[ind]);
    }
}
