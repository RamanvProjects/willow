#include <iostream>
#include <boost/program_options.hpp>
#include "parser.h"
#include "pointcloud.h"

void processCommandLine(int argc, char **argv, std::string &input, std::string &output, bool &parse, bool &path)
{
    boost::program_options::options_description desc("Allowed options");
    desc.add_options()
        ("input-obj", boost::program_options::value<std::string>(&input)->required(), "input obj filepath")
        ("output-path", boost::program_options::value<std::string>(&output)->required(), "desired output filepath")
        ("parse", boost::program_options::value<bool>(&parse)->default_value(false), "sets the executable to just parse the obj verbatum into JSON and NOT recalculate edges between vertices")
        ("path", boost::program_options::bool_switch(&path), "optional flag, to run pathing version of parser")
    ;
    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);
}

int main(int argc, char *argv[])
{
    std::string input;
    std::string output;
    bool parse;
    bool path;
    processCommandLine(argc,argv,input,output,parse,path);
    if(parse)
    {
        Parser::parseObjAndWriteJson(input,output);
    }
    else
    {
        std::vector<glm::vec3> vertices;
        std::vector<std::vector<size_t>> edges;
        Parser::readObj(input,vertices);
        PointCloud cloud(vertices);
        cloud.generateEdges(edges);
        Parser::writeObj(output,cloud.m_points,edges);
    }
    return 0;
}

