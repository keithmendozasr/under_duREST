#include <iostream>
#include <string>
#include <stdexcept>

#include <unordered_map>

//#include <unistd.h>
#include <getopt.h>

#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <log4cpp/Configurator.hh>
#include <log4cpp/NDC.hh>

using namespace std;

typedef unordered_map<string, string> CmdLines;

void usage(const char *progName)
{
    cerr << "Usage: " << progName << " -c|--config <config file> -t|--test <test file"<<endl;
}

CmdLines parseArgs(int argc, char **argv)
{
    static struct option opts[] = {
        { "config", required_argument, NULL, 'c' },
        { "test",   required_argument, NULL, 't' },
        { 0,0,0,0 }
    };

    CmdLines cmds;
    int opt;
    while((opt = getopt_long(argc, argv, "c:t:", opts, nullptr)) != -1)
    {
        cout << "Value of opt: \"" << opt << "\"" << endl;
        switch(opt)
        {
        case 'c':
            cout << "c option found. Arg: " << optarg << endl;
            cmds["config"] = optarg;
            break;
        case 't':
            cout << "t option found. Arg: " << optarg << endl;
            cmds["test"] = optarg;
            break;
        default:
            usage(argv[0]);
            exit(EXIT_FAILURE);
        };
    };

    return move(cmds);
}

int main(int argc, char **argv)
{
    auto cmds = parseArgs(argc, argv);

    // Check all options are there
    {
        string expectedOptions[] = {
            "config",
            "test"
        };

        for(const auto &i : expectedOptions)
            try
            {
                cmds.at(i);
            }
            catch(out_of_range &e)
            {
                cerr << "\"" << i << "\" option not provided\n";
                usage(argv[0]);
                exit(EXIT_FAILURE);
            }
    }

    return 0;
}
