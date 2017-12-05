#ifndef POINTCLOUD_H
#define POINTCLOUD_H
#include "CommonIncludes.h"

class CompareVec3 {
    const glm::vec3 origin = glm::vec3(0.f);
public:
    bool operator() (const glm::vec3& a, const glm::vec3& b)
    {
        return glm::distance2(a, origin) < glm::distance2(b, origin);
    }
};

class PointCloud
{
public:
    PointCloud(const std::vector<glm::vec3> &points);
    void resortPoints();
    void generateEdges(std::vector<std::vector<size_t>> &directedEdges);

    std::vector<glm::vec3> m_points;
private:
    inline float approximateCost(const size_t i0, const size_t i1);
};

#endif // POINTCLOUD_H
