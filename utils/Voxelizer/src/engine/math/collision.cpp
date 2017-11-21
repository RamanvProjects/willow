#include "collision.h"
#include "src/engine/math/interval.h"

glm::vec2 Collision::circleCollidesCircle(glm::vec2 posA, glm::vec2 posB, float radA, float radB) {
    float dist2 = glm::distance2(posA, posB);
    float sum = radA + radB;
    if(dist2 >= sum*sum) {
        return glm::vec2(0,0);
    }
    float len = sum - glm::sqrt(dist2);
    glm::vec2 vec = glm::normalize(posA - posB);
    return vec*len;
}

float Collision::intervalMTV(Interval a, Interval b) {
    float aRight = b.max - a.min;
    float aLeft = a.max - b.min;
    if(aLeft < 0 || aRight < 0) {
        return 0;
    }
    if(aRight < aLeft) {
        return aRight;
    }
    return -aLeft;
}

//pos is center
bool Collision::isBehindPlane(const glm::vec4 plane, const glm::vec3 pos, float dim) {

    dim = dim / 2.f;

    if(!isBehindPlane(plane, glm::vec3(pos.x - dim, pos.y-dim, pos.z-dim))) {
        return false;

    }
    if(!isBehindPlane(plane, glm::vec3(pos.x + dim, pos.y-dim, pos.z-dim))) {
        return false;

    }
    if(!isBehindPlane(plane, glm::vec3(pos.x - dim, pos.y+dim, pos.z-dim))) {
        return false;

    }
    if(!isBehindPlane(plane, glm::vec3(pos.x + dim, pos.y+dim, pos.z-dim))) {
        return false;

    }
    if(!isBehindPlane(plane, glm::vec3(pos.x - dim, pos.y-dim, pos.z+dim))) {
        return false;

    }
    if(!isBehindPlane(plane, glm::vec3(pos.x + dim, pos.y-dim, pos.z+dim))) {
        return false;

    }
    if(!isBehindPlane(plane, glm::vec3(pos.x - dim, pos.y+dim, pos.z+dim))) {
        return false;

    }
    if(!isBehindPlane(plane, glm::vec3(pos.x + dim, pos.y+dim, pos.z+dim))) {
        return false;

    }
    return true;
}

bool Collision::isBehindPlane(glm::vec4 plane, glm::vec3 point) {
    return plane.x * point.x + plane.y * point.y + plane.z * point.z + plane.w < 0;
}

glm::vec3 Collision::voxelCollides(glm::vec3 aPos, glm::vec3 aDim, glm::vec3 bPos, glm::vec3 bDim, glm::vec3 step, glm::vec3 max){

    if(step.y > 0) {
        for(float y = aPos.y + step.y; y < max.y; y += step.y) {
            if(aabbCollidesAabb(bPos, bDim, glm::vec3(aPos.x, y, aPos.z), aDim)) {
                return glm::vec3(aPos.x, y, aPos.z);
            }
        }
    } else {
        for(float y = aPos.y + step.y; y > max.y; y += step.y) {
            if(aabbCollidesAabb(bPos, bDim, glm::vec3(aPos.x, y, aPos.z), aDim)) {
                return glm::vec3(aPos.x, y, aPos.z);
            }
        }
    }
    return aPos;
}

bool Collision::aabbCollidesAabb(glm::vec3 aPos, glm::vec3 aDim, glm::vec3 bPos, glm::vec3 bDim) {
    float w = aDim.x/2.f;
    float h = aDim.y/2.f;
    float l = aDim.z/2.f;

    if(aabbContainsPoint(bPos,bDim,glm::vec3(aPos.x-w,aPos.y-h,aPos.z-l))){
            return true;
    }
    if(aabbContainsPoint(bPos,bDim,glm::vec3(aPos.x-w,aPos.y-h,aPos.z+l))){
            return true;
    }
    if(aabbContainsPoint(bPos,bDim,glm::vec3(aPos.x-w,aPos.y+h,aPos.z-l))){
            return true;
    }
    if(aabbContainsPoint(bPos,bDim,glm::vec3(aPos.x-w,aPos.y+h,aPos.z+l))){
            return true;
    }
    if(aabbContainsPoint(bPos,bDim,glm::vec3(aPos.x+w,aPos.y-h,aPos.z-l))){
            return true;
    }
    if(aabbContainsPoint(bPos,bDim,glm::vec3(aPos.x+w,aPos.y-h,aPos.z+l))){
            return true;
    }
    if(aabbContainsPoint(bPos,bDim,glm::vec3(aPos.x+w,aPos.y+h,aPos.z-l))){
            return true;
    }
    if(aabbContainsPoint(bPos,bDim,glm::vec3(aPos.x+w,aPos.y+h,aPos.z+l))){
            return true;
    }
    return false;
}

bool Collision::aabbContainsPoint(glm::vec3 pos, glm::vec3 dim, glm::vec3 point) {
    dim = dim * .5f;

    if(point.x < pos.x - dim.x || point.x > pos.x + dim.x) {
        return false;
    }

    if(point.y < pos.y - dim.y || point.y > pos.y + dim.y) {
        return false;
    }

    if(point.z < pos.z - dim.z || point.z > pos.z + dim.z) {
        return false;
    }

    return true;
}

