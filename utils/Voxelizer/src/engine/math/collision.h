#ifndef COLLISION_H
#define COLLISION_H
#include "util/CommonIncludes.h"


class Interval;

class Collision
{
public:

    static float intervalMTV(Interval a, Interval b);

    static glm::vec2 circleCollidesCircle(glm::vec2 posA, glm::vec2 posB, float radA, float radB);

    static bool isBehindPlane(glm::vec4 plane, glm::vec3 pos, float dim);

    static bool isBehindPlane(glm::vec4 plane, glm::vec3 point);

    static glm::vec3 voxelCollides(glm::vec3 aPos, glm::vec3 aDim, glm::vec3 bPos, glm::vec3 bDim, glm::vec3 step, glm::vec3 stop);

    static bool aabbCollidesAabb(glm::vec3 aPos, glm::vec3 aDim,glm::vec3 bPos, glm::vec3 bDim);

    static bool aabbContainsPoint(glm::vec3 pos, glm::vec3 dim, glm::vec3 point);

};

#endif // COLLISION_H
