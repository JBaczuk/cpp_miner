#include <block_header.hpp>
#include <openssl/sha.h>
#include <stdexcept>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <algorithm>

void BlockHeader::serialize()
{
    serializedHeader.assign(nVersion.begin(), nVersion.end());
    serializedHeader.insert(serializedHeader.end(), hashPrevBlock.begin(), hashPrevBlock.end());
    serializedHeader.insert(serializedHeader.end(), merkleRoot.begin(), merkleRoot.end());
    serializedHeader.insert(serializedHeader.end(), time.begin(), time.end());
    serializedHeader.insert(serializedHeader.end(), nBits.begin(), nBits.end());
    serializedHeader.insert(serializedHeader.end(), nonce.begin(), nonce.end());
    isSerialized = true;

    fprintf(stdout, "header: ");
    for (int i=0; i < BLOCK_HEADER_SIZE_BYTES; i++)
    {
        fprintf(stdout, "%02x", serializedHeader[i]);
    }
    fprintf(stdout, "\n");

    /*
     * Expected block for cpp_miner genesis block
        raw block:
        0100000000000000000000000000000000000000000000000000000000000000000000003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4a29ab5f49ffff001d1dac2b7c0101000000010000000000000000000000000000000000000000000000000000000000000000ffffffff4d04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73ffffffff0100f2052a01000000434104678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5fac00000000
        
        block header:
        0100000000000000000000000000000000000000000000000000000000000000000000003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4a29ab5f49ffff001d1dac2b7c
        
        transaction:
       01000000010000000000000000000000000000000000000000000000000000000000000000ffffffff4d04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73ffffffff0100f2052a01000000434104678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5fac00000000
        */
	
}

void BlockHeader::hash256()
{
    if (!isSerialized)
    {
        throw std::logic_error("hash256() Error: The block header has not yet been serialized");
        return;
    }
    uint8_t tmp[32];
    uint8_t blockHash[32];
    SHA256(&serializedHeader[0], BLOCK_HEADER_SIZE_BYTES, tmp);
    SHA256(tmp, 32, blockHash);

    hash.assign(blockHash, blockHash + 32); 
    std::reverse(hash.begin(), hash.end());

    fprintf(stdout, "block hash: ");
    for(int i=0; i < 32; i++)
    {
        fprintf(stdout, "%02x", hash[i]);
    }
    fprintf(stdout, "\n");


}

void BlockHeader::mine()
{
    serialize();
    hash256();
    //while(!checkPoW())
    {
        //incrementNonce();
        //hash256();

	fprintf(stdout, "nonce: %02x%0x%0x%0x\n", nonce[0], nonce[1], nonce[2], nonce[3]); 
    }
}

bool BlockHeader::checkPoW()
{
    for(int i=0; i < 32; i++)
    {
        if(hash[i] < target[i])
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
                fprintf(stdout, "%02x", hash[i]);
            }
            fprintf(stdout, "\n");

            return true;
        }
        else if(hash[i] > target[i])
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
	serialize();
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
    serializedHeader.insert(serializedHeader.end(), nonce.begin(), nonce.end());
}

