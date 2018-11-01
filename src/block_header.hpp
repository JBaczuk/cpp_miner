#ifndef BLOCK_HEADER_HPP
#define BLOCK_HEADER_HPP

#include <vector>
#define BLOCK_HEADER_SIZE_BYTES     80
#include <stdint.h>

class BlockHeader
{
public:
    std::vector <unsigned char> nVersion;
    std::vector <unsigned char> hashPrevBlock;
    std::vector <unsigned char> merkleRoot;
    std::vector <unsigned char> time;
    std::vector <unsigned char> nBits;
    std::vector <unsigned char> nonce;
    std::vector <unsigned char> target;
    bool isSerialized = false;
    unsigned char serializedHeader[BLOCK_HEADER_SIZE_BYTES];
    unsigned char blockHash[32];


	BlockHeader(): nVersion(4), hashPrevBlock(32), merkleRoot(32), time(4), nBits(4), nonce(4), target(32)
    {}

    void serialize();
    void hash256();
    void mine();

private:
    bool checkPoW();
    void incrementNonce();
    uint32_t nonceToInt();
    void intToNonce(uint32_t nonceInt);
};

#endif

