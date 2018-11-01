#include <block_header.hpp>
#include <openssl/sha.h>
#include <stdexcept>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>

void BlockHeader::serialize()
{
    std::copy(nVersion.begin(), nVersion.begin() + nVersion.size(), serializedHeader);
    std::copy(hashPrevBlock.begin(), hashPrevBlock.begin() + hashPrevBlock.size(), serializedHeader + nVersion.size());
    std::copy(merkleRoot.begin(), merkleRoot.begin() + merkleRoot.size(), serializedHeader + nVersion.size() + hashPrevBlock.size());
    std::copy(time.begin(), time.begin() + time.size(), serializedHeader + nVersion.size() + hashPrevBlock.size() + merkleRoot.size());
    std::copy(nBits.begin(), nBits.begin() + nBits.size(), serializedHeader + nVersion.size() + hashPrevBlock.size() + merkleRoot.size() + time.size());
    std::copy(nonce.begin(), nonce.begin() + nonce.size(), serializedHeader + nVersion.size() + hashPrevBlock.size() + merkleRoot.size() + time.size() + nBits.size());
    isSerialized = true;
}

void BlockHeader::hash256()
{
    if (!isSerialized)
    {
        throw std::logic_error("hash256() Error: The block header has not yet been serialized");
        return;
    }
    unsigned char tmp[32];
    SHA256(serializedHeader, BLOCK_HEADER_SIZE_BYTES, tmp);
    SHA256(tmp, 32, blockHash);
}

void BlockHeader::mine()
{
    hash256();
    while(!checkPoW())
    {
        incrementNonce();
        hash256();
    }
}

bool BlockHeader::checkPoW()
{
    for(int i=0; i < 32; i++)
    {
        if(blockHash[i] < target[i])
        {
            fprintf(stdout, "block header: ");
            for(int i=0; i < 80; i++)
            {
                fprintf(stdout, "%02x", serializedHeader[i]);
            }
            fprintf(stdout, "\n");

            fprintf(stdout, "nonce: 0x%02x%02x%02x%02x\n", nonce[0], nonce[1], nonce[2], nonce[3]);
            
            fprintf(stdout, "block hash: ");
            for(int i=0; i < 32; i++)
            {
                fprintf(stdout, "%02x", blockHash[i]);
            }
            fprintf(stdout, "\n");

            return true;
        }
        else if(blockHash[i] > target[i])
        {
            return false;
        }
    }
    return false;
}

void BlockHeader::incrementNonce()
{
    uint32_t nonceInt = nonceToInt();
    if (nonceInt == ULONG_MAX)
    {
        throw std::runtime_error("incrementNonce Error: We have exhausted all possible nonce values");
        return;
    }
    else
    {
        nonceInt++;
        intToNonce(nonceInt);
    }
}

uint32_t BlockHeader::nonceToInt()
{
    uint32_t nonceInt = (nonce[0] << 24) | (nonce[1] << 16) | (nonce[2] << 8) | nonce[3];
    return nonceInt;
}

void BlockHeader::intToNonce(uint32_t nonceInt)
{
    nonce[0] = nonceInt >> 24;
    nonce[1] = nonceInt >> 16;
    nonce[2] = nonceInt >> 8;
    nonce[3] = nonceInt;
    std::copy(nonce.begin(), nonce.begin() + nonce.size(), serializedHeader + nVersion.size() + hashPrevBlock.size() + merkleRoot.size() + time.size() + nBits.size());
}

