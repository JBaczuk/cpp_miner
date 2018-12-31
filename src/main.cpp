#include <iostream>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <util.hpp>
#include <block_header.hpp>
#include <stdexcept>
#include <transaction.hpp>
#include <limits.h>

using namespace std;
namespace po = boost::program_options;

int main (int argc, char* argv[])
{
	try
    {
        std::string appName = boost::filesystem::basename(argv[0]);
        string command;
	int verbosity = 0;
        vector<string> command_params;

	string mineUsage = "$ cpp_miner mine <blockHeader 80B-hex> OR <version 4B-hex> <hashPrevBlock 32B-hex> <merkleRoot 32B-hex> <time 4B-hex> <nBits 4B-hex> <nonce 4B-hex>";
        string genesisGenUsage = "$ cpp_miner genesisgen <pubkey 65B-hex> \"<coinbase-message 91B-string>\" <value 8B-decimal> <time 4B-hex> <nBits 4B-hex> <nonce 4B-hex>";

	string mineUsage = "$ cpp_miner mine <blockHeader 80B-hex> OR <version 4B-hex> <hashPrevBlock 32B-hex> <merkleRoot 32B-hex> <time 4B-hex> <nBits 4B-hex> <nonce 4B-hex>";
        string genesisGenUsage = "$ cpp_miner genesisgen <pubkey 65B-hex> \"<coinbase-message 91B-string>\" <value 8B-decimal> <time 4B-hex> <nBits 4B-hex> <nonce 4B-hex>";

        po::options_description desc("Allowed options");
        desc.add_options()
<<<<<<< HEAD
            ("help,h", "Show this help message")
=======
            ("help", "Show this help message")
>>>>>>> add verbose modes
	    ("verbosity,v", po::value<int>()->implicit_value(1),
"Enable verbosity (optionally specify level)")
            ("command", po::value< vector<string> >(), "Command");

        po::positional_options_description p;
        p.add("command", -1);

        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).
                  options(desc).positional(p).run(), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << appName + "\n\n";
            std::cout << "Usage: $ cpp_miner command [options]\n\n";
            std::cout << "block miner\n\n";
            std::cout << mineUsage << std::endl << std::endl;
            std::cout << "genesis block miner\n\n";
            std::cout << genesisGenUsage << std::endl << std::endl;
            std::cout << desc;
            return 0;
        }

        if (vm.count("command"))
        {
            auto commands = vm["command"].as<vector<string> >();
            command = commands[0];
            vector<string> command_params_new(commands.begin() + 1, commands.end());
            command_params = command_params_new;
        }

        if (vm.count("verbosity"))
        {
	    verbosity = vm["verbosity"].as<int>();
	    std::cout << "Verbosity enabled.  Level is " << verbosity << "\n";
        }

        // Command Handler
        if (command == "help")
        {
            std::cout << "Usage: $ cpp_miner command[options]\n";
            std::cout << desc;
            return 0;
        }

        if (command == "genesisgen")
        {
            auto params = vm["command"].as<vector<string> >();
            BlockHeader blockHeader;
            Transaction coinbaseTransaction;

            if (params.size() < 7)
            {
                std::cout << genesisGenUsage;
                return -1;
            }

            auto pubkey = parseHex(params[1].c_str());
            if (pubkey.size() != 65)
            {
                throw std::logic_error("pubkey must be a hex string of 65 bytes");
                return -1;
            }

            std::vector<unsigned char> coinbaseMessage(params[2].begin(), params[2].end());
            if (coinbaseMessage.size() > 91)
            {
                throw std::logic_error("coinbaseMessage must be a hex string not greater than 91 bytes");
                return -1;
            }

            auto valueInt = std::stoi(params[3].c_str());
            if (valueInt < 0 || valueInt > ULLONG_MAX)
            {
                throw std::logic_error("value must be a valid 64 bit unsigned int");
                return -1;
            }
            uint64_t satoshiValue = (uint64_t)valueInt * COIN;
            std::vector<unsigned char> value = bigIntToHex(satoshiValue);

            auto timeInt = std::stoi(params[4].c_str());
            if (timeInt < 0 || timeInt > ULONG_MAX)
            {
                throw std::logic_error("time must be a valid 32 bit unsigned int");
                return -1;
            }
            std::vector<unsigned char> time = intToHex((uint32_t)timeInt);

            auto nBitsInt = std::stoi(params[5].c_str());
            if (nBitsInt < 0 || nBitsInt > ULONG_MAX)
            {
                throw std::logic_error("nBits must be a valid 32 bit unsigned int");
                return -1;
            }
            std::vector<unsigned char> nBits = intToHex((uint32_t)nBitsInt);

            auto nonceInt = std::stoi(params[6].c_str());
            if (nonceInt < 0 || nonceInt > ULONG_MAX)
            {
                throw std::logic_error("nonce must be a valid 32 bit unsigned int");
                return -1;
            }
            std::vector<unsigned char> nonce = intToHex((uint32_t)nonceInt);

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
            // Notice this only supports up to 255 bytes
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
            coinbaseTransaction.outValue = value;
            coinbaseTransaction.scriptPubKey.assign({0xac}); // OP_CHECKSIG
            coinbaseTransaction.scriptPubKey.insert(coinbaseTransaction.scriptPubKey.begin(), pubkey.begin(), pubkey.end());
	        coinbaseTransaction.scriptPubKey.insert(coinbaseTransaction.scriptPubKey.begin(), {0x43, 0x41}); // script size and pubkey push data
            coinbaseTransaction.lockTime.assign({0x00, 0x00, 0x00, 0x00});

            blockHeader.nVersion.assign({0x01, 0x00, 0x00, 0x00});
            std::fill(blockHeader.hashPrevBlock.begin(), blockHeader.hashPrevBlock.end(), 0);
	        std::vector<uint8_t> txid = coinbaseTransaction.getTxid();
            blockHeader.merkleRoot.assign(txid.begin(), txid.end());
            blockHeader.time.assign(time.begin(), time.end());
            blockHeader.nBits.assign(nBits.begin(), nBits.end());
            blockHeader.nonce.assign(nonce.begin(), nonce.end());
	        std::vector<unsigned char> reversedNBits = blockHeader.nBits;
	        std::reverse(reversedNBits.begin(), reversedNBits.end());
            blockHeader.target = nBitsToTarget(reversedNBits.end()[-1] | (reversedNBits.end()[-2] << 8) | (reversedNBits.end()[-3] << 16) | (reversedNBits.end()[-4] << 24));
            
            // Mine the block
            blockHeader.mine(verbosity);

	        std::vector<unsigned char> merkleRootSwapped = blockHeader.merkleRoot;
	        std::reverse(merkleRootSwapped.begin(), merkleRootSwapped.end());

            fprintf(stdout, "merkle root: ");
            for(int i=0; i < 32; i++)
            {
                fprintf(stdout, "%02x", merkleRootSwapped[i]);
            }
            fprintf(stdout, "\n");
	    
            return 0;

        }

        if (command == "mine")
        {
            auto params = vm["command"].as<vector<string> >();
            BlockHeader blockHeader;

            if (params.size() < 2)
            {
                std::cout << mineUsage;
                return -1;
            }
            if (params.size() == 2)
            {
				auto rawBlockHeader = parseHex(params[1].c_str());
                if (rawBlockHeader.size() == 0)
                {
                    throw std::logic_error("Block header is not valid hex");
                    return -1;
                }
                else if (rawBlockHeader.size() != 80)
                {
                    throw std::logic_error("Block header must be 80 bytes");
                    return -1;
                }

                blockHeader.nVersion.assign(rawBlockHeader.begin(), rawBlockHeader.begin() + 4);
                blockHeader.hashPrevBlock.assign(rawBlockHeader.begin() + 4, rawBlockHeader.begin() + 36);
                blockHeader.merkleRoot.assign(rawBlockHeader.begin() + 36, rawBlockHeader.begin() + 68);
                blockHeader.time.assign(rawBlockHeader.begin() + 68, rawBlockHeader.begin() + 72);
                blockHeader.nBits.assign(rawBlockHeader.begin() + 72, rawBlockHeader.begin() + 76);
                blockHeader.nonce.assign(rawBlockHeader.begin() + 76, rawBlockHeader.begin() + 80);
		        std::vector<unsigned char> reversedNBits = blockHeader.nBits;
		        std::reverse(reversedNBits.begin(), reversedNBits.end());
                blockHeader.target = nBitsToTarget(reversedNBits.end()[-1] | (reversedNBits.end()[-2] << 8) | (reversedNBits.end()[-3] << 16) | (reversedNBits.end()[-4] << 24));

                fprintf(stdout, "target: ");
                for (int i=0; i<blockHeader.target.size(); i++)
                {
                    fprintf(stdout, "%02x", blockHeader.target[i]);
                }
                fprintf(stdout, "\n");
            }
            else
            {
                auto version = parseHex(params[1].c_str());
                if (version.size() != 4)
                {
                    throw std::logic_error("version must be a hex string of 4 bytes");
                    return -1;
                }

                auto hashPrevBlock = parseHex(params[2].c_str());
                if (hashPrevBlock.size() != 32)
                {
                    throw std::logic_error("hashPrevBlock must be a hex string of 32 bytes");
                    return -1;
                }

                auto merkleRoot = parseHex(params[3].c_str());
                if (merkleRoot.size() != 32)
                {
                    throw std::logic_error("merkleRoot must be a hex string of 32 bytes");
                    return -1;
                }

                auto time = parseHex(params[4].c_str());
                if (time.size() != 4)
                {
                    throw std::logic_error("time must be a hex string of 4 bytes");
                    return -1;
                }

                auto nBits = parseHex(params[5].c_str());
                if (nBits.size() != 4)
                {
                    throw std::logic_error("nBits must be a hex string of 4 bytes");
                    return -1;
                }

                auto nonce = parseHex(params[6].c_str());
                if (nonce.size() != 4)
                {
                    throw std::logic_error("nonce must be a hex string of 4 bytes");
                    return -1;
                }

                blockHeader.nVersion.assign(version.begin(), version.end());
                blockHeader.hashPrevBlock.assign(hashPrevBlock.begin(), hashPrevBlock.end());
                blockHeader.merkleRoot.assign(merkleRoot.begin(), merkleRoot.end());
                blockHeader.time.assign(time.begin(), time.end());
                blockHeader.nBits.assign(nBits.begin(), nBits.end());
                blockHeader.nonce.assign(nonce.begin(), nonce.end());
                std::vector<unsigned char> reversedNBits = blockHeader.nBits;
                std::reverse(reversedNBits.begin(), reversedNBits.end());
                blockHeader.target = nBitsToTarget(reversedNBits.end()[-1] | (reversedNBits.end()[-2] << 8) | (reversedNBits.end()[-3] << 16) | (reversedNBits.end()[-4] << 24));
            }
            // Mine the block
            blockHeader.mine(verbosity);
            return 0;
        }

        else
        {
            std::cout << std::endl << "Usage: $ cpp_miner command [options]\n";
            std::cout << desc;
            std::cout << std::endl << "Commands" << std::endl << mineUsage << std::endl << genesisGenUsage << std::endl << std::endl;
        }

    }
    catch(std::exception& e)
    {
        std::cout << e.what() << "\n";
        return 1;
    }

    return 0;
}
