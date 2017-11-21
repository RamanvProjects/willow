#include "graphicalmesh.h"
#include "meshtriangle.h"
#include "src/util/obj.h"
#include "src/engine/graphics/graphics.h"
#include "src/engine/graphics/shape.h"
#include "src/engine/geometric/geometriccollision.h"

GraphicalMesh::GraphicalMesh(const QString &path, const bool makeVbo) : m_octree(nullptr), m_mesh(nullptr)
{
    OBJ *obj = new OBJ(path);

    if(!obj->triangles.length())
    {
        return;
    }

    m_vertices.reserve(obj->vertices.size());
    m_max = glm::vec3(obj->triangles.at(0)->vertices[0]);
    m_min = m_max;
    for(int i = 0; i < obj->vertices.size(); i++)
    {
        glm::vec3 vert = obj->vertices.at(i);
        m_max = checkMax(vert,m_max);
        m_min = checkMin(vert,m_min);
    }
    m_center = (m_min + m_max) * glm::vec3(.5f);

    for(int i = 0; i < obj->vertices.size(); i++)
    {
        const glm::vec3* vert = new glm::vec3(obj->vertices.at(i));
        m_vertices.push_back(vert);
    }

    m_octree = std::make_shared<Octree>(m_center,m_max-m_min);

    m_triangles.resize(obj->triangles.size());
    m_vertexToContainingTris.resize(m_vertices.size());
    size_t oPointIndex = 0;
    for(size_t i = 0; i < m_triangles.size(); i++)
    {
        size_t v1Index = obj->triangles.at(i)->a.vertexIndex;
        size_t v2Index = obj->triangles.at(i)->b.vertexIndex;
        size_t v3Index = obj->triangles.at(i)->c.vertexIndex;
        m_triangles[i].v1 = m_vertices[v1Index];
        m_triangles[i].v2 = m_vertices[v2Index];
        m_triangles[i].v3 = m_vertices[v3Index];
        constructOctreePoint(*m_triangles[i].v1,v1Index);
        constructOctreePoint(*m_triangles[i].v2,v2Index);
        constructOctreePoint(*m_triangles[i].v3,v3Index);
        m_vertexToContainingTris.at(v1Index).push_back(i);
        m_vertexToContainingTris.at(v2Index).push_back(i);
        m_vertexToContainingTris.at(v3Index).push_back(i);
    }

    if(makeVbo)
    {
        m_mesh = std::make_shared<Shape>();
        m_mesh->setVertexData(obj->vboData.data(), obj->vboData.size() * sizeof(GLfloat),obj->vertexCount);
        m_mesh->setAttribute(0,3,GL_FLOAT,GL_FALSE, sizeof(GLfloat)*8,0);
        m_mesh->setAttribute(1, 3, GL_FLOAT, GL_TRUE, sizeof(GLfloat)*8, sizeof(GLfloat)*3);
        m_mesh->setAttribute(2,2,GL_FLOAT,GL_FALSE,sizeof(GLfloat)*8, sizeof(GLfloat)*6);
    }

    delete obj;
}

bool GraphicalMesh::constructOctreePoint(const glm::vec3 &position, const size_t &location)
{
    OctreePoint *opoint = new OctreePoint(position,location);
    if(m_octree->insert(opoint))
    {
        m_points.push_back(opoint);
        return true;
    }
    else
    {
        delete opoint;
        return false;
    }
}

GraphicalMesh::~GraphicalMesh()
{
    for(int i = m_points.size() - 1; i >= 0; i--)
    {
        delete m_points[i];
    }

    for(int i = m_vertices.size() - 1; i >= 0; i--)
    {
        delete m_vertices[i];
    }
}



glm::vec3 GraphicalMesh::checkMin(const glm::vec3 val, glm::vec3 min){
    if(val.x < min.x)
    {
        min.x = val.x;
    }
    if(val.y < min.y)
    {
        min.y = val.y;
    }
    if(val.z < min.z)
    {
        min.z = val.z;
    }
    return min;
}

glm::vec3 GraphicalMesh::checkMax(const glm::vec3 val, glm::vec3 max) {
    if(val.x > max.x)
    {
        max.x = val.x;
    }
    if(val.y > max.y)
    {
        max.y = val.y;
    }
    if(val.z > max.z)
    {
        max.z = val.z;
    }
    return max;
}

void GraphicalMesh::draw(Graphics *g)
{
    if(m_mesh)
    {
        glm::mat4 m = glm::mat4(1.f);
        g->drawShape(m_mesh, m);
    }
}

Response GraphicalMesh::collidesMesh(Ellipsoid e, glm::vec3 finalPos)
{
    std::vector<OctreePoint*> nearby;
//    glm::vec3 bmin = checkMin(e.center - e.scale,finalPos + e.scale);
//    glm::vec3 bmax = checkMax(e.center - e.scale,finalPos + e.scale);
    glm::vec3 bmin = glm::vec3(-100000);
    glm::vec3 bmax = glm::vec3(100000);
    m_octree->getPointsInsideBox(bmin,bmax,nearby);

    float minT = 4;
    glm::vec3 norm = glm::vec3(0);
    glm::vec3 contact = glm::vec3(0);
    for(size_t i = 0; i < nearby.size(); i++)
    {
        OctreePoint* opoint = nearby.at(i);
        std::vector<size_t> tris = m_vertexToContainingTris.at(opoint->getIndex());
        for(size_t j = 0; j < tris.size(); j++)
        {
            MeshTriangle tri = m_triangles.at(tris.at(i));
            Response r = GeometricCollision::ellipsoidTriangle(e,finalPos,tri);
            if(r.collided && r.t < minT)
            {
                minT = r.t;
                norm = r.normal;
                contact = r.contactPoint;
            }

            if(minT < 1)
            {
                return Response(true, minT, norm, contact);
            }
            else
            {
                return Response(false, 0, norm, contact);
            }
        }

    }

}
