#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <vector>
#include <stdint.h>

#define COIN    100000000

class Transaction
{
    public:
        // Raw transaction properties
        std::vector<unsigned char> version;
        std::vector<unsigned char> numInputs;
        std::vector<unsigned char> prevOutput;
        std::vector<unsigned char> prevOutputIndex;
        std::vector<unsigned char> scriptSig;
        std::vector<unsigned char> sequence;
        std::vector<unsigned char> numOutputs;
        std::vector<unsigned char> outValue;
        std::vector<unsigned char> scriptPubKey;
        std::vector<unsigned char> lockTime;

        uint32_t sizeBytes;
        bool isSerialized;
	std::vector<unsigned char> serializedTransaction;
    
        std::vector<unsigned char> getTxid();

    Transaction(): version(4), prevOutput(32), prevOutputIndex(4),
		sequence(4), outValue(8), lockTime(4)
    {}

    private:
        void serialize();
};

#endif
