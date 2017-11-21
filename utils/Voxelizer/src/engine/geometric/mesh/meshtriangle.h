#ifndef MESHTRIANGLE_H
#define MESHTRIANGLE_H

#include "src/util/CommonIncludes.h"


class Shape;

class MeshTriangle
{
public:

    const glm::vec3* v1;
    const glm::vec3* v2;
    const glm::vec3* v3;
    const glm::vec3 normal;

    MeshTriangle() : v1(nullptr),v2(nullptr),v3(nullptr),normal() {}

    MeshTriangle(glm::vec3* first, glm::vec3* second, glm::vec3* third) : v1(first),v2(second),v3(third), normal(glm::normalize(glm::cross(*second - *first, *third - *first))){}
};

#endif // MESHTRIANGLE_H
