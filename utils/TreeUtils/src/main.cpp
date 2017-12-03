#include <iostream>
#include <boost/program_options.hpp>
#include "parser.h"

void processCommandLine(int argc, char **argv, std::string &input, std::string &output)
{
    boost::program_options::options_description desc("Allowed options");
    desc.add_options()
        ("input-obj", boost::program_options::value<std::string>(&input)->required(), "input obj to be voxelized")
        ("output-path", boost::program_options::value<std::string>(&output)->required(), "desired output file location")
    ;

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);
}

int main(int argc, char *argv[], char **envp)
{
    std::string input;
    std::string output;
    processCommandLine(argc,argv,input,output);
    Parser::parseAndWrite(input,output);
    return 0;
}

