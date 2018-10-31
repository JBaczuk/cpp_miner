#include <block_header.hpp>
#include <openssl/sha.h>
#include <stdexcept>
#include <stdio.h>

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
            fprintf(stdout, "nonce: 0x%02x%02x%02x%02x\n", nonce[0], nonce[1], nonce[2], nonce[3]);
            
            fprintf(stdout, "block hash: ");
            for(int i=0; i < 32; i++)
            {
                fprintf(stdout, "%02x", blockHash[i]);
            }
            fprintf(stdout, "\n");

            return true;
        }
        else
        {
            //fprintf(stdout, "block hash: ");
            //for(int i=0; i < 32; i++)
            //{
            //    fprintf(stdout, "%02x", blockHash[i]);
            //}
            //fprintf(stdout, "\n");
            return false;
        }
    }
}

void BlockHeader::incrementNonce()
{
    for(int i=0; i < 4; i++)
    {
        if (serializedHeader[BLOCK_HEADER_SIZE_BYTES - 5 + i] < 0xff)
        {
            serializedHeader[BLOCK_HEADER_SIZE_BYTES - 5 + i]++;
            break;
        }
        else if (i < 3 && serializedHeader[BLOCK_HEADER_SIZE_BYTES - 5 + i] == 0xff)
        {
            serializedHeader[BLOCK_HEADER_SIZE_BYTES - 5 + i] == 0;
            serializedHeader[BLOCK_HEADER_SIZE_BYTES - 5 + i + 1] = serializedHeader[BLOCK_HEADER_SIZE_BYTES - 5 + i + 1] + 1;
            break;
        }
        else if (i == 3 && nonce[i] == 0xff)
        {
            throw std::runtime_error("incrementNonce Error: We have exhausted all possible nonce values");
            return;
        }
    }
    copy(serializedHeader + BLOCK_HEADER_SIZE_BYTES - 5, serializedHeader + BLOCK_HEADER_SIZE_BYTES - 1, nonce.begin());
}

