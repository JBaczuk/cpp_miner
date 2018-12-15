#include <transaction.hpp>
#include <openssl/sha.h>
#include <stdexcept>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>

void Transaction::serialize()
{
    serializedTransaction.assign(version.begin(), version.end());
    serializedTransaction.insert(serializedTransaction.end(), numInputs.begin(), numInputs.end()); 
    serializedTransaction.insert(serializedTransaction.end(), prevOutput.begin(), prevOutput.end()); 
    serializedTransaction.insert(serializedTransaction.end(), prevOutputIndex.begin(), prevOutputIndex.end());
    serializedTransaction.insert(serializedTransaction.end(), scriptSig.begin(), scriptSig.end()); 
    serializedTransaction.insert(serializedTransaction.end(), sequence.begin(), sequence.end()); 
    serializedTransaction.insert(serializedTransaction.end(), numOutputs.begin(), numOutputs.end()); 
    serializedTransaction.insert(serializedTransaction.end(), outValue.begin(), outValue.end()); 
    serializedTransaction.insert(serializedTransaction.end(), scriptPubKey.begin(), scriptPubKey.end()); 
    serializedTransaction.insert(serializedTransaction.end(), lockTime.begin(), lockTime.end()); 
    isSerialized = true;

    //fprintf(stdout, "tx: ");
    //for (int i=0; i < serializedTransaction.size(); i++)
    //{
    //    fprintf(stdout, "%02x", serializedTransaction[i]);
    //}
    //fprintf(stdout, "\n");
}

std::vector<unsigned char> Transaction::getTxid()
{
    serialize();
    std::vector<unsigned char> txid;
    
    if (!isSerialized)
    {
        throw std::logic_error("hash256() Error: The transaction has not yet been serialized");
        return txid;
    }
    unsigned char tmp[32];
    unsigned char hash[32];
    SHA256(&serializedTransaction[0], serializedTransaction.size(), tmp);
    SHA256(tmp, 32, hash);
    txid.assign(hash, hash + 32);

    return txid;
}

