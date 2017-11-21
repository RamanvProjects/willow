#ifndef GEOMETRICCOLLISION_H
#define GEOMETRICCOLLISION_H

#include "response.h"
#include "src/engine/geometric/mesh/meshtriangle.h"
#include "ellipsoid.h"
#include <glm/glm.hpp>


class GeometricCollision
{
public:

    static Response sphereInterior(glm::vec3 startCenter, glm::vec3 endCenter, const MeshTriangle tri);

    static Response sphereEdge(glm::vec3 startCenter, glm::vec3 endCenter, glm::vec3 c, glm::vec3 d);

    static Response sphereVertex(glm::vec3 startCenter, glm::vec3 endCenter, glm::vec3 vertex);

    static Response sphereTriangle(glm::vec3 startCenter, glm::vec3 endCenter, const MeshTriangle tri);

    static Response rayTriangle(glm::vec3 startCenter, glm::vec3 endCenter, const MeshTriangle tri);

    static Response raySphere(glm::vec3 startCenter, glm::vec3 endCenter, glm::vec3 sphereCenter);

    static Response ellipsoidTriangle(const Ellipsoid e, const glm::vec3 endCenter, const MeshTriangle tri);

    static glm::vec3 respond(const Ellipsoid e, Response r, glm::vec3 vel, int numSlides);

    //returns the amout the ellipsoid should move
    static glm::vec3 slideEllipsoid(const glm::vec3 normal, const glm::vec3 vel);
};

#endif // GEOMETRICCOLLISION_H
