#include <iostream>
#include <boost/program_options.hpp>
#include "parser.h"
#include "pointcloud.h"

void processCommandLine(int argc, char **argv, std::string &input, std::string &output, bool &recalculate, bool &path, bool &as_json)
{
    boost::program_options::options_description desc("Allowed options");
    desc.add_options()
            ("input_path", boost::program_options::value<std::string>(&input)->required(), "input obj filepath")
            ("output_path", boost::program_options::value<std::string>(&output)->required(), "desired output filepath")
            ("output_as_json", boost::program_options::value<bool>(&as_json)->required(), "output the file as json if TRUE, output as obj if FALSE")
            ("recalculate_branches", boost::program_options::value<bool>(&recalculate)->default_value(false), "sets the executable to just parse the obj verbatum into JSON and NOT recalculate branches between vertices")
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
    bool recalculate;
    bool path;
    bool as_json;
    processCommandLine(argc,argv,input,output,recalculate,path,as_json);

    std::vector<glm::vec3> vertices;
    std::vector<std::vector<size_t>> edges;
    Parser::readObj(input,vertices);
    if(recalculate)
    {
      PointCloud cloud(vertices);
      cloud.generateEdges(edges);
      vertices = cloud.m_points;
    }

    if(as_json)
    {
        Parser::writeJson(output,vertices,edges);
    }
    else
    {
        Parser::writeObj(output,vertices,edges);
    }

    return 0;
}
