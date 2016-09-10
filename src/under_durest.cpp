#include <iostream>
#include <string>
#include <stdexcept>
#include <fstream>

#include <unordered_map>

#include <unistd.h>
#include <getopt.h>

#include <jsoncpp/json/json.h>

#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <log4cpp/Configurator.hh>
#include <log4cpp/NDC.hh>

using namespace std;
using namespace Json;
using namespace log4cpp;

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
        // DO NOT DELETE COMMENTED OUT CODE
        //cout << "Value of opt: \"" << opt << "\"" << endl;
        switch(opt)
        {
        case 'c':
            //cout << "c option found. Arg: " << optarg << endl;
            cmds["config"] = optarg;
            break;
        case 't':
            //cout << "t option found. Arg: " << optarg << endl;
            cmds["test"] = optarg;
            break;
        default:
            usage(argv[0]);
            exit(EXIT_FAILURE);
        };
    };

    return move(cmds);
}

Value parseConfig(const string& fileName)
{
    ifstream inFile(fileName.c_str(), ios::in | ios::binary);
    if(!inFile.is_open())
    {
        cerr << "File not found: " << fileName << "\n";
        throw runtime_error("Config file " + fileName + " not found");
    }

    Value root;
    inFile >> root;

    /* DO NOT DELETE
    cout << "Config members:\n";
    for(auto i : root.getMemberNames())
        cout << "\t" << i << endl;
    */

    return move(root);
}

bool logReady = false;

int main(int argc, char **argv)
{
    // Check all options are there
    auto cmds = parseArgs(argc, argv);
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

    try
    {
        //Load config file
        auto config = parseConfig(cmds["config"]);

        //Configure logger
        PropertyConfigurator::configure(config["logconfig"].asString());

        Category &root = Category::getRoot();
        logReady = true;
        root.infoStream() << "under_durest initialized";
    }
    catch(const ConfigureFailure &e)
    {
        cerr << "Error encountered configuring logger. Cause: " << e.what() << endl;
        return EXIT_FAILURE;
    }
    catch(const exception &e)
    {
        if(logReady)
            Category::getRoot().errorStream() << "Error encountered. Cause: " << e.what();
        else
            cerr << "Error encountered. Cause: " << e.what() << endl;
    }

    return 0;
}
