#ifndef BLOCK_HEADER_HPP
#define BLOCK_HEADER_HPP

#include <vector>
#define BLOCK_HEADER_SIZE_BYTES     80
#include <stdint.h>

class BlockHeader
{
public:
    std::vector <uint8_t> nVersion;
    std::vector <uint8_t> hashPrevBlock;
    std::vector <uint8_t> merkleRoot;
    std::vector <uint8_t> time;
    std::vector <uint8_t> nBits;
    std::vector <uint8_t> nonce;
    std::vector <uint8_t> target;
    bool isSerialized = false;
    std::vector <uint8_t> serializedHeader;
    std::vector <uint8_t> hash;

	BlockHeader(): nVersion(4), hashPrevBlock(32), merkleRoot(32), time(4), nBits(4), nonce(4), target(32), serializedHeader(BLOCK_HEADER_SIZE_BYTES), hash(32)
    {}

    void serialize();
    void hash256();
    void mine(uint8_t verbosity);

private:
    bool checkPoW();
    void incrementNonce();
    uint32_t nonceToInt();
    void intToNonce(uint32_t nonceInt);
};

#endif

