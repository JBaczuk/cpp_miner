#include <iostream>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <util.hpp>
#include <block_header.hpp>
#include <stdexcept>

using namespace std;
namespace po = boost::program_options;

int main (int argc, char* argv[])
{
	try {
        std::string appName = boost::filesystem::basename(argv[0]);
        bool some_flag_en;
        string command;
        vector<string> command_params;

        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "Show this help message")
            ("verbose,v", po::value<int>()->implicit_value(1),
                  "Enable verbosity (optionally specify level)")
            ("some_flag,t", po::bool_switch(&some_flag_en)->default_value(false),
                  "Use some_flag configuration")
            ("command", po::value< vector<string> >(), "Command");

        po::positional_options_description p;
        p.add("command", -1);

        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).
                  options(desc).positional(p).run(), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << appName + "\n";
            std::cout << "<insert program description here>\n\n";
            std::cout << "Usage: command [options]\n";
            std::cout << desc;
            return 0;
        }

        if (vm.count("command"))
        {
            auto commands = vm["command"].as<vector<string>>();
            command = commands[0];
            vector<string> command_params_new(commands.begin() + 1, commands.end());
            command_params = command_params_new;
        }

        if (vm.count("verbose")) {
            std::cout << "Verbosity enabled.  Level is " << vm["verbose"].as<int>()
                 << "\n";
        }

        if (some_flag_en) {
            std::cout << "Some flag enabled." << "\n";
        }

        // Command Handler
        if(command == "help")
        {
            std::cout << "Usage: command [options]\n";
            std::cout << desc;
            return 0;
        }

        if(command == "mine")
        {
            auto params = vm["command"].as<vector<string>>();
            BlockHeader blockHeader;

            if (params.size() < 2)
            {
                std::cout << "Usage: mine <blockHeader 80B> OR <version 4B> <hashPrevBlock 32B> <merkleRoot 32B> <time 4B> <nBits 4B> <nonce 4B>\n";
                return -1;
            }
            if (params.size() == 2)
            {
				auto rawBlockHeader = parseHex(params[1].c_str());
                if(rawBlockHeader.size() == 0)
                {
                    throw std::logic_error("Block header is not valid hex");
                    return -1;
                }
                else if(rawBlockHeader.size() != 80)
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
                blockHeader.target = nBitsToTarget(blockHeader.nBits.end()[-1] | (blockHeader.nBits.end()[-2] << 8) | (blockHeader.nBits.end()[-3] << 16) | (blockHeader.nBits.end()[-4] << 24));

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
                if(version.size() == 0)
                {
                    throw std::logic_error("version is not valid hex");
                    return -1;
                }

                auto hashPrevBlock = parseHex(params[2].c_str());
                if(hashPrevBlock.size() == 0)
                {
                    throw std::logic_error("hashPrevBlock is not valid hex");
                    return -1;
                }

                auto merkleRoot = parseHex(params[3].c_str());
                if(merkleRoot.size() == 0)
                {
                    throw std::logic_error("merkleRoot is not valid hex");
                    return -1;
                }

                auto time = parseHex(params[4].c_str());
                if(time.size() == 0)
                {
                    throw std::logic_error("time is not valid hex");
                    return -1;
                }

                auto nBits = parseHex(params[5].c_str());
                if(nBits.size() == 0)
                {
                    throw std::logic_error("nBits is not valid hex");
                    return -1;
                }

                auto nonce = parseHex(params[6].c_str());
                if(nonce.size() == 0)
                {
                    throw std::logic_error("nonce is not valid hex");
                    return -1;
                }

                if(version.size() != 4)
                {
                    throw std::logic_error("version must be 4 bytes");
                    return -1;
                }
                if(hashPrevBlock.size() != 32)
                {
                    throw std::logic_error("hashPrevBlock must be 32 bytes");
                    return -1;
                }
                if(merkleRoot.size() != 32)
                {
                    throw std::logic_error("merkleRoot must be 32 bytes");
                    return -1;
                }
                if(time.size() != 4)
                {
                    throw std::logic_error("time must be 4 bytes");
                    return -1;
                }
                if(nBits.size() != 4)
                {
                    throw std::logic_error("nBits must be 4 bytes");
                    return -1;
                }
                if(nonce.size() != 4)
                {
                    throw std::logic_error("nonce must be 4 bytes");
                    return -1;
                }

                blockHeader.nVersion.assign(version.begin(), version.end());
                blockHeader.hashPrevBlock.assign(hashPrevBlock.begin(), hashPrevBlock.end());
                blockHeader.merkleRoot.assign(merkleRoot.begin(), merkleRoot.end());
                blockHeader.time.assign(time.begin(), time.end());
                blockHeader.nBits.assign(nBits.begin(), nBits.end());
                blockHeader.nonce.assign(nonce.begin(), nonce.end());
                blockHeader.target = nBitsToTarget(blockHeader.nBits.end()[-1] | (blockHeader.nBits.end()[-2] << 8) | (blockHeader.nBits.end()[-3] << 16) | (blockHeader.nBits.end()[4] << 24));
            }
            // Mine the block
            blockHeader.serialize();
            blockHeader.mine();
            // TODO: print final hash and nonce
            return 0;
        }

        else
        {
            throw std::logic_error("Invalid command");
            std::cout << "Usage: command [options]\n";
            std::cout << desc;
        }

    }
    catch(std::exception& e)
    {
        std::cout << e.what() << "\n";
        return 1;
    }

    return 0;
}
