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

BOOST_AUTO_TEST_CASE(test_calculateMerkleRoot)
{
    std::vector<unsigned char> expected {0x3b,0xa3,0xed,0xfd,0x7a,0x7b,0x12,0xb2,0x7a,0xc7,0x2c,0x3e,0x67,0x76,0x8f,0x61,0x7f,0xc8,0x1b,0xc3,0x88,0x8a,0x51,0x32,0x3a,0x9f,0xb8,0xaa,0x4b,0x1e,0x5e,0x4a};
    std::string coinbaseMessage = "The Times 03/Jan/2009 Chancellor on brink of second bailout for banks";
    std::vector<unsigned char> vCoinbaseMessage(coinbaseMessage.begin(), coinbaseMessage.end());
}

BOOST_AUTO_TEST_CASE(test_intToHex)
{
    std::vector<unsigned char> expected {0x1d,0xac,0x2b,0x7c};
    std::vector<unsigned char> actual = intToHex(2083236893);
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), actual.begin(), actual.end());
}

BOOST_AUTO_TEST_SUITE_END()

