#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <util.hpp>
#include <vector>
#include <transaction.hpp>

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
    Transaction coinbaseTransaction; 
    std::vector<unsigned char> expected {0x3b,0xa3,0xed,0xfd,0x7a,0x7b,0x12,0xb2,0x7a,0xc7,0x2c,0x3e,0x67,0x76,0x8f,0x61,0x7f,0xc8,0x1b,0xc3,0x88,0x8a,0x51,0x32,0x3a,0x9f,0xb8,0xaa,0x4b,0x1e,0x5e,0x4a};
    std::string coinbaseMessageStr = "The Times 03/Jan/2009 Chancellor on brink of second bailout for banks";
    std::vector<unsigned char> coinbaseMessage(coinbaseMessageStr.begin(), coinbaseMessageStr.end());
    

    std::vector<unsigned char> nBits {0xff,0xff,0x00,0x1d};

    // Construct the scriptSig
    std::vector<unsigned char> scriptSig = {0x04};
    scriptSig.insert(scriptSig.end(), nBits.begin(), nBits.end());
    int msgSize = coinbaseMessage.size();
    scriptSig.insert(scriptSig.end(), {0x01, 0x04, (unsigned char) msgSize});
    scriptSig.insert(scriptSig.end(), coinbaseMessage.begin(), coinbaseMessage.end());
    scriptSig.insert(scriptSig.begin(), scriptSig.size());

    // Construct the coinbase transaction
    coinbaseTransaction.version.assign({0x01, 0x00, 0x00, 0x00});
    coinbaseTransaction.numInputs.assign({0x01});
    std::fill(coinbaseTransaction.prevOutput.begin(), coinbaseTransaction.prevOutput.end(), 0);
    coinbaseTransaction.prevOutputIndex.assign({0xFF, 0xFF, 0xFF, 0xFF});
    coinbaseTransaction.scriptSig.insert(coinbaseTransaction.scriptSig.end(), scriptSig.begin(), scriptSig.end());
    coinbaseTransaction.sequence.assign({0xFF, 0xFF, 0xFF, 0xFF});
    coinbaseTransaction.numOutputs.assign({0x01});
    std::vector<unsigned char> value {0x00,0xf2,0x05,0x2a,0x01,0x00,0x00,0x00};
    coinbaseTransaction.outValue = value;
    coinbaseTransaction.scriptPubKey.assign({0xac}); // OP_CHECKSIG
    std::vector<unsigned char> pubkey {0x04,0x67,0x8a,0xfd,0xb0,0xfe,0x55,0x48,0x27,0x19,0x67,0xf1,0xa6,0x71,0x30,0xb7,0x10,0x5c,0xd6,0xa8,0x28,0xe0,0x39,0x09,0xa6,0x79,0x62,0xe0,0xea,0x1f,0x61,0xde,0xb6,0x49,0xf6,0xbc,0x3f,0x4c,0xef,0x38,0xc4,0xf3,0x55,0x04,0xe5,0x1e,0xc1,0x12,0xde,0x5c,0x38,0x4d,0xf7,0xba,0x0b,0x8d,0x57,0x8a,0x4c,0x70,0x2b,0x6b,0xf1,0x1d,0x5f};
    coinbaseTransaction.scriptPubKey.insert(coinbaseTransaction.scriptPubKey.begin(), pubkey.begin(), pubkey.end());
    coinbaseTransaction.scriptPubKey.insert(coinbaseTransaction.scriptPubKey.begin(), {0x43, 0x41}); // script size and pubkey push data
    coinbaseTransaction.lockTime.assign({0x00, 0x00, 0x00, 0x00}); 
    std::vector<unsigned char> merkleRoot = coinbaseTransaction.getTxid();
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), merkleRoot.begin(), merkleRoot.end());
}

BOOST_AUTO_TEST_CASE(test_calculateMerkleRoot2)
{
    Transaction coinbaseTransaction; 
    std::vector<unsigned char> expected {0x93,0x2a,0x78,0x7b,0x86,0x67,0x7e,0x46,0xdd,0x86,0x9d,0xf4,0xbb,0xed,0x5e,0xa5,0x2d,0x18,0xf4,0x88,0x85,0x35,0x9b,0x56,0x33,0xfd,0xba,0x9a,0x93,0xbd,0x77,0x9f};
    std::string coinbaseMessageStr = "Bloomberg Nov 14, 2018 Greenspan Says U.S. May Be Seeing First Signs of Inflation";
    std::vector<unsigned char> coinbaseMessage(coinbaseMessageStr.begin(), coinbaseMessageStr.end());
    

    std::vector<unsigned char> nBits {0xff,0xff,0x7f,0x20};

    // Construct the scriptSig
    std::vector<unsigned char> scriptSig = {0x04};
    scriptSig.insert(scriptSig.end(), nBits.begin(), nBits.end());
    int msgSize = coinbaseMessage.size();
    if (msgSize < 0x4b)
    {
        scriptSig.insert(scriptSig.end(), {0x01, 0x04, (unsigned char) msgSize});
    }
    else
    {
        scriptSig.insert(scriptSig.end(), {0x01, 0x04, 0x4c, (unsigned char) msgSize});
    }
    scriptSig.insert(scriptSig.end(), coinbaseMessage.begin(), coinbaseMessage.end());
    scriptSig.insert(scriptSig.begin(), scriptSig.size());

    // Construct the coinbase transaction
    coinbaseTransaction.version.assign({0x01, 0x00, 0x00, 0x00});
    coinbaseTransaction.numInputs.assign({0x01});
    std::fill(coinbaseTransaction.prevOutput.begin(), coinbaseTransaction.prevOutput.end(), 0);
    coinbaseTransaction.prevOutputIndex.assign({0xFF, 0xFF, 0xFF, 0xFF});
    coinbaseTransaction.scriptSig.insert(coinbaseTransaction.scriptSig.end(), scriptSig.begin(), scriptSig.end());
    coinbaseTransaction.sequence.assign({0xFF, 0xFF, 0xFF, 0xFF});
    coinbaseTransaction.numOutputs.assign({0x01});
    std::vector<unsigned char> value {0x00,0x20,0x67,0x6f,0xfe,0xbe,0x00,0x00};
    coinbaseTransaction.outValue = value;
    coinbaseTransaction.scriptPubKey.assign({0xac}); // OP_CHECKSIG
    std::vector<unsigned char> pubkey {0x04,0x83,0xa2,0xeb,0x1d,0x9a,0xe0,0x27,0xfe,0x47,0xc8,0xec,0x88,0xe6,0xd9,0xe6,0xa3,0x90,0x3f,0x5d,0x0e,0x56,0x02,0xfa,0x23,0x2a,0x0f,0xe2,0x21,0xda,0xe2,0x56,0xb1,0x7f,0x8c,0x62,0x07,0x22,0x9c,0x62,0x61,0xa6,0x65,0x9c,0x12,0xc5,0x92,0x65,0xa3,0x29,0x9c,0xbb,0x0e,0x45,0xd1,0x80,0xd1,0x5d,0x72,0x26,0xf2,0x93,0x23,0x89,0x4d};
    coinbaseTransaction.scriptPubKey.insert(coinbaseTransaction.scriptPubKey.begin(), pubkey.begin(), pubkey.end());
    coinbaseTransaction.scriptPubKey.insert(coinbaseTransaction.scriptPubKey.begin(), {0x43, 0x41}); // script size and pubkey push data
    coinbaseTransaction.lockTime.assign({0x00, 0x00, 0x00, 0x00}); 
    std::vector<unsigned char> merkleRoot = coinbaseTransaction.getTxid();
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), merkleRoot.begin(), merkleRoot.end());
}

BOOST_AUTO_TEST_CASE(test_intToHex)
{
    std::vector<unsigned char> expected {0x1d,0xac,0x2b,0x7c};
    std::vector<unsigned char> actual = intToHex(2083236893);
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), actual.begin(), actual.end());
}

BOOST_AUTO_TEST_CASE(test_calculateMerkleRoot)
{
    Transaction coinbaseTransaction; 
    std::vector<unsigned char> expected {0x3b,0xa3,0xed,0xfd,0x7a,0x7b,0x12,0xb2,0x7a,0xc7,0x2c,0x3e,0x67,0x76,0x8f,0x61,0x7f,0xc8,0x1b,0xc3,0x88,0x8a,0x51,0x32,0x3a,0x9f,0xb8,0xaa,0x4b,0x1e,0x5e,0x4a};
    std::string coinbaseMessageStr = "The Times 03/Jan/2009 Chancellor on brink of second bailout for banks";
    std::vector<unsigned char> coinbaseMessage(coinbaseMessageStr.begin(), coinbaseMessageStr.end());
    

    std::vector<unsigned char> nBits {0xff,0xff,0x00,0x1d};

    // Construct the scriptSig
    std::vector<unsigned char> scriptSig = {0x04};
    scriptSig.insert(scriptSig.end(), nBits.begin(), nBits.end());
    int msgSize = coinbaseMessage.size();
    scriptSig.insert(scriptSig.end(), {0x01, 0x04, (unsigned char) msgSize});
    scriptSig.insert(scriptSig.end(), coinbaseMessage.begin(), coinbaseMessage.end());
    scriptSig.insert(scriptSig.begin(), scriptSig.size());

    // Construct the coinbase transaction
    coinbaseTransaction.version.assign({0x01, 0x00, 0x00, 0x00});
    coinbaseTransaction.numInputs.assign({0x01});
    std::fill(coinbaseTransaction.prevOutput.begin(), coinbaseTransaction.prevOutput.end(), 0);
    coinbaseTransaction.prevOutputIndex.assign({0xFF, 0xFF, 0xFF, 0xFF});
    coinbaseTransaction.scriptSig.insert(coinbaseTransaction.scriptSig.end(), scriptSig.begin(), scriptSig.end());
    coinbaseTransaction.sequence.assign({0xFF, 0xFF, 0xFF, 0xFF});
    coinbaseTransaction.numOutputs.assign({0x01});
    std::vector<unsigned char> value {0x00,0xf2,0x05,0x2a,0x01,0x00,0x00,0x00};
    coinbaseTransaction.outValue = value;
    coinbaseTransaction.scriptPubKey.assign({0xac}); // OP_CHECKSIG
    std::vector<unsigned char> pubkey {0x04,0x67,0x8a,0xfd,0xb0,0xfe,0x55,0x48,0x27,0x19,0x67,0xf1,0xa6,0x71,0x30,0xb7,0x10,0x5c,0xd6,0xa8,0x28,0xe0,0x39,0x09,0xa6,0x79,0x62,0xe0,0xea,0x1f,0x61,0xde,0xb6,0x49,0xf6,0xbc,0x3f,0x4c,0xef,0x38,0xc4,0xf3,0x55,0x04,0xe5,0x1e,0xc1,0x12,0xde,0x5c,0x38,0x4d,0xf7,0xba,0x0b,0x8d,0x57,0x8a,0x4c,0x70,0x2b,0x6b,0xf1,0x1d,0x5f};
    coinbaseTransaction.scriptPubKey.insert(coinbaseTransaction.scriptPubKey.begin(), pubkey.begin(), pubkey.end());
    coinbaseTransaction.scriptPubKey.insert(coinbaseTransaction.scriptPubKey.begin(), {0x43, 0x41}); // script size and pubkey push data
    coinbaseTransaction.lockTime.assign({0x00, 0x00, 0x00, 0x00}); 
    std::vector<unsigned char> merkleRoot = coinbaseTransaction.getTxid();
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), merkleRoot.begin(), merkleRoot.end());
}

BOOST_AUTO_TEST_CASE(test_calculateMerkleRoot2)
{
    Transaction coinbaseTransaction; 
    std::vector<unsigned char> expected {0x93,0x2a,0x78,0x7b,0x86,0x67,0x7e,0x46,0xdd,0x86,0x9d,0xf4,0xbb,0xed,0x5e,0xa5,0x2d,0x18,0xf4,0x88,0x85,0x35,0x9b,0x56,0x33,0xfd,0xba,0x9a,0x93,0xbd,0x77,0x9f};
    std::string coinbaseMessageStr = "Bloomberg Nov 14, 2018 Greenspan Says U.S. May Be Seeing First Signs of Inflation";
    std::vector<unsigned char> coinbaseMessage(coinbaseMessageStr.begin(), coinbaseMessageStr.end());
    

    std::vector<unsigned char> nBits {0xff,0xff,0x7f,0x20};

    // Construct the scriptSig
    std::vector<unsigned char> scriptSig = {0x04};
    scriptSig.insert(scriptSig.end(), nBits.begin(), nBits.end());
    int msgSize = coinbaseMessage.size();
    if (msgSize < 0x4b)
    {
        scriptSig.insert(scriptSig.end(), {0x01, 0x04, (unsigned char) msgSize});
    }
    else
    {
        scriptSig.insert(scriptSig.end(), {0x01, 0x04, 0x4c, (unsigned char) msgSize});
    }
    scriptSig.insert(scriptSig.end(), coinbaseMessage.begin(), coinbaseMessage.end());
    scriptSig.insert(scriptSig.begin(), scriptSig.size());

    // Construct the coinbase transaction
    coinbaseTransaction.version.assign({0x01, 0x00, 0x00, 0x00});
    coinbaseTransaction.numInputs.assign({0x01});
    std::fill(coinbaseTransaction.prevOutput.begin(), coinbaseTransaction.prevOutput.end(), 0);
    coinbaseTransaction.prevOutputIndex.assign({0xFF, 0xFF, 0xFF, 0xFF});
    coinbaseTransaction.scriptSig.insert(coinbaseTransaction.scriptSig.end(), scriptSig.begin(), scriptSig.end());
    coinbaseTransaction.sequence.assign({0xFF, 0xFF, 0xFF, 0xFF});
    coinbaseTransaction.numOutputs.assign({0x01});
    std::vector<unsigned char> value {0x00,0x20,0x67,0x6f,0xfe,0xbe,0x00,0x00};
    coinbaseTransaction.outValue = value;
    coinbaseTransaction.scriptPubKey.assign({0xac}); // OP_CHECKSIG
    std::vector<unsigned char> pubkey {0x04,0x83,0xa2,0xeb,0x1d,0x9a,0xe0,0x27,0xfe,0x47,0xc8,0xec,0x88,0xe6,0xd9,0xe6,0xa3,0x90,0x3f,0x5d,0x0e,0x56,0x02,0xfa,0x23,0x2a,0x0f,0xe2,0x21,0xda,0xe2,0x56,0xb1,0x7f,0x8c,0x62,0x07,0x22,0x9c,0x62,0x61,0xa6,0x65,0x9c,0x12,0xc5,0x92,0x65,0xa3,0x29,0x9c,0xbb,0x0e,0x45,0xd1,0x80,0xd1,0x5d,0x72,0x26,0xf2,0x93,0x23,0x89,0x4d};
    coinbaseTransaction.scriptPubKey.insert(coinbaseTransaction.scriptPubKey.begin(), pubkey.begin(), pubkey.end());
    coinbaseTransaction.scriptPubKey.insert(coinbaseTransaction.scriptPubKey.begin(), {0x43, 0x41}); // script size and pubkey push data
    coinbaseTransaction.lockTime.assign({0x00, 0x00, 0x00, 0x00}); 
    std::vector<unsigned char> merkleRoot = coinbaseTransaction.getTxid();
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), merkleRoot.begin(), merkleRoot.end());
}

BOOST_AUTO_TEST_CASE(test_intToHex)
{
    std::vector<unsigned char> expected {0x1d,0xac,0x2b,0x7c};
    std::vector<unsigned char> actual = intToHex(2083236893);
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), actual.begin(), actual.end());
}

BOOST_AUTO_TEST_SUITE_END()

