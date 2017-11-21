#ifndef GRAPHICALMESH_H
#define GRAPHICALMESH_H

#include <QString>
#include <QList>

#include "src/engine/geometric/ellipsoid.h"
#include "src/engine/geometric/response.h"
#include "src/engine/datastructs/octree.h"
#include "src/util/CommonIncludes.h"

//class MeshTriangle;
class Graphics;
class Shape;
class OBJ;
class MeshTriangle;
class GraphicalMesh
{
public:
    GraphicalMesh(const QString &path, const bool makeVbo);

    ~GraphicalMesh();

    void draw(Graphics *g);

    Response collidesMesh(const Ellipsoid e, const glm::vec3 finalPos);

    inline std::shared_ptr<Octree> getOctree() {return m_octree;}
    inline glm::vec3 getMinVertex() {return m_min;}
    inline glm::vec3 getMeshCenter() {return m_center;}
    inline glm::vec3 getMaxVertex() {return m_max;}

private:
    inline glm::vec3 checkMin(const glm::vec3 val, glm::vec3 min);
    inline glm::vec3 checkMax(const glm::vec3 val, glm::vec3 max);
    inline bool constructOctreePoint(const glm::vec3& position, const size_t& location);

    std::shared_ptr<Shape> m_mesh;
    std::shared_ptr<Octree> m_octree;
    std::vector<const glm::vec3*> m_vertices;
    std::vector<MeshTriangle> m_triangles;
    std::vector<OctreePoint *> m_points;
    std::vector<std::vector<size_t>> m_vertexToContainingTris;
    glm::vec3 m_min;
    glm::vec3 m_max;
    glm::vec3 m_center;
};

#endif // GraphicalMesh
