#include "pointcloud.h"

PointCloud::PointCloud(const std::vector<glm::vec3> &points)
{
    m_points.assign(points.begin(),points.end());
}

void PointCloud::resortPoints()
{

}

void PointCloud::generateEdges()
{

}
