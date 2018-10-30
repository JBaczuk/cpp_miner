#include <iostream>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

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
        else
        {
            std::cout << "Invalid command." << endl;
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
