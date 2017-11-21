#ifndef VOXELIZER_H
#define VOXELIZER_H

#include "util/CommonIncludes.h"
#include <vector>

class GraphicalMesh;
class Voxelizer
{
public:
    static void voxelize(const char *inputPath, const char *outputDirectory, size_t resolution);
    static void meshToVoxel(std::shared_ptr<GraphicalMesh> mesh, bool *data, size_t resolution);
    static void writeVoxelizedData(const bool *data, const char *outputDirectory, size_t resolution);
    static inline size_t index(size_t x, size_t y, size_t z, size_t width, size_t height);
};

#endif // VOXELIZER_H
