#ifndef RESPONSE_H
#define RESPONSE_H

#include "glm/common.hpp"
class Response
{
public:

    const bool collided;
    const float t;
    const glm::vec3 normal;
    const glm::vec3 contactPoint;

    Response(bool coll, float tValue, glm::vec3 n, glm::vec3 p) : t(tValue), collided(coll),normal(n),contactPoint(p){}
};

#endif // RESPONSE_H
