#include "voxelizer.h"
#include <iostream>
#include <fstream>
#include "src/engine/datastructs/octree.h"
#include "src/engine/geometric/mesh/graphicalmesh.h"

void Voxelizer::voxelize(const char *inputPath, const char *outputDirectory, size_t resolution)
{
    std::shared_ptr<GraphicalMesh> mesh = std::make_shared<GraphicalMesh>(QString(inputPath), false);
    bool *data = new bool[resolution*resolution*resolution];
    meshToVoxel(mesh,data,resolution);
    writeVoxelizedData(data, outputDirectory, resolution);
}


//todo: change this to raycast against mesh instead of just checking if points lie inside some bounding box
void Voxelizer::meshToVoxel(std::shared_ptr<GraphicalMesh> mesh, bool *data, size_t resolution)
{
    float voxelSize = 1.f/resolution;
    glm::vec3 voxelVector = glm::vec3(voxelSize);
    glm::vec3 epsilonVector = glm::vec3(.0001f);

    std::shared_ptr<Octree> tree = mesh->getOctree();
    glm::vec3 scale = mesh->getMaxVertex() - mesh->getMinVertex();
    glm::vec3 offset = mesh->getMinVertex();

    for(size_t x = 0; x < resolution; x++)
    {
        for(size_t y = 0; y < resolution; y++)
        {
            for(size_t z = 0; z < resolution; z++)
            {
                //bounds of this voxel
                glm::vec3 bMin = glm::vec3(x*voxelSize,y*voxelSize,z*voxelSize);
                glm::vec3 bMax = bMin + voxelVector;

                bMin = bMin * scale + offset - epsilonVector;
                bMax = bMax * scale + offset + epsilonVector;

                std::vector<OctreePoint*> containedPoints;
                tree->getPointsInsideBox(bMin,bMax,containedPoints);
                data[index(x,y,z,resolution,resolution)] = containedPoints.size() > 0;
            }
        }
    }
}

void Voxelizer::writeVoxelizedData(const bool *data, const char *outputDirectory, size_t resolution)
{
    std::ofstream outputStream;
    outputStream.open (outputDirectory);
    if(!outputStream)
    {
        std::cerr << "Couldn't write output to " << outputDirectory <<". Got error \'" << strerror(errno) << "\'. Exiting.\n";
        return;
    }

    for(size_t x = 0; x < resolution; x++)
    {
        outputStream << "[";
        for(size_t y = 0; y < resolution; y++)
        {
            outputStream << "[";
            for(size_t z = 0; z < resolution; z++)
            {
                outputStream << data[index(x,y,z,resolution,resolution)];
            }
            outputStream << "]";
        }
        outputStream << "]";
    }
    outputStream << "\n";

    outputStream.close();
}

size_t Voxelizer::index(size_t x, size_t y, size_t z, size_t width, size_t height)
{
    return x + (y * width) + (z * width * height);
}
