#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <util.hpp>
#include <vector>

BOOST_AUTO_TEST_SUITE(test_util)

BOOST_AUTO_TEST_CASE(test_isSpace)
{
    bool answer = isSpace(' ');
    BOOST_CHECK_MESSAGE(answer, "Space not detected properly");
    answer = isSpace('\n');
    BOOST_CHECK_MESSAGE(answer, "newline character should be detected as space");
    answer = isSpace('\0');
    BOOST_CHECK_MESSAGE(!answer, "null character incorrectly identified as space");
    answer = isSpace('c');
    BOOST_CHECK_MESSAGE(!answer, "c character incorrectly identified as space");
}

BOOST_AUTO_TEST_CASE(test_nBitsToTarget)
{
    std::vector<unsigned char> expected {0,0,0,0,0,0,0,0,0x1b,0xc3,0x30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    std::vector<unsigned char> actual = nBitsToTarget(404472624);
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), actual.begin(), actual.end());
}

BOOST_AUTO_TEST_SUITE_END()
