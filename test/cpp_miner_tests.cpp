#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <testfuncs.hpp>

BOOST_AUTO_TEST_SUITE(test_testfuncs)

BOOST_AUTO_TEST_CASE(test_testfunc)
{
int answer = testfunc(1,1);
BOOST_CHECK_EQUAL(2,answer);
}

BOOST_AUTO_TEST_SUITE_END()
