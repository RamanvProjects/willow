#ifndef POINTCLOUD_H
#define POINTCLOUD_H
#include "CommonIncludes.h"

class PointCloud
{
public:
    PointCloud(const std::vector<glm::vec3> &points);
    void resortPoints();
    void generateEdges();
private:
    std::vector<glm::vec3> m_points;
};

#endif // POINTCLOUD_H
