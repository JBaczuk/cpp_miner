#ifndef BLOCK_HEADER_HPP
#define BLOCK_HEADER_HPP

#include <vector>
#define BLOCK_HEADER_SIZE_BYTES     80

class BlockHeader
{
public:
    std::vector <unsigned char> nVersion;
    std::vector <unsigned char> hashPrevBlock;
    std::vector <unsigned char> merkleRoot;
    std::vector <unsigned char> time;
    std::vector <unsigned char> nBits;
    std::vector <unsigned char> nonce;


	BlockHeader(): nVersion(4), hashPrevBlock(32), merkleRoot(32), time(4), nBits(4), nonce(4)
    {}

    void serialize(unsigned char header[BLOCK_HEADER_SIZE_BYTES]);
};

#endif

