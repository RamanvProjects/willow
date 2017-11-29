#include <QApplication>
#include <iostream>
#include "src/engine/voxelizer.h"
#include "mainwindow.h"
#include <boost/program_options.hpp>

void processCommandLine(int argc, char **argv, int &resolution, bool &visualize, std::string &input, std::string &output)
{
    boost::program_options::options_description desc("Allowed options");
    desc.add_options()
        ("resolution", boost::program_options::value<int>(&resolution)->default_value(64), "voxel resolution level")
        ("visualize", boost::program_options::value<bool>(&visualize)->default_value(false), "render output in window using OpenGL")
        ("input-obj", boost::program_options::value<std::string>(&input)->required(), "input obj to be voxelized")
        ("output-path", boost::program_options::value<std::string>(&output)->required(), "desired output file location")
    ;

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);

    if (vm.count("resolution")) {
        std::cout << "Voxel resolution was set to "
     << vm["resolution"].as<int>() << ".\n";
    } else {
        std::cout << "Resolution level was not set.\n";
    }
}


///home/bsheeran/Documents/CS2951w/Voxelizer/res/obj/tree.obj

int main(int argc, char *argv[], char **envp)
{
    for (char **env = envp; *env != 0; env++)
    {
      char *thisEnv = *env;
      printf("%s\n", thisEnv);
    }

    int resolution;
    bool visualize;
    std::string input;
    std::string output;
    processCommandLine(argc,argv,resolution,visualize,input,output);

    if(resolution <= 0)
    {
        std::cerr << "INVALID RESOLUTION " << resolution << "\n";
        return 1;
    }

    size_t unsignedRes = resolution;

    if(visualize)
    {
        std::cout << "Launching OpenGL window to view voxelized OBJ. Use mouse and WASD to navigate the scene.\n";
        QApplication a(argc, argv);
        MainWindow w(unsignedRes,input,output);
        w.show();
        return a.exec();
    }
    else
    {
        std::cout << "Voxelizing OBJ at path " << input << ".\n";
        Voxelizer::voxelize(input.c_str(),output.c_str(), unsignedRes);
        std::cout << "Done voxelizing. Output written to " << output << "\n";
    }

    return 0;
}

