#include "geometriccollision.h"
#include "glm/gtx/norm.hpp"
#include "glm/gtx/perpendicular.hpp"
#include "qdebug.h"

Response GeometricCollision::sphereInterior(glm::vec3 startCenter, glm::vec3 endCenter, MeshTriangle tri) {

//    glm::vec3 n = glm::normalize(endCenter - startCenter);
//    Response res = rayTriangle(startCenter, endCenter, tri);
    float dott = glm::dot(tri.normal,endCenter - startCenter);
    glm::vec3 collisionNormal = tri.normal;
    if(dott < 0) {
        collisionNormal = -tri.normal;
    }
    Response res = rayTriangle(startCenter + collisionNormal, endCenter, tri);
    if(res.collided) {

        return Response(true,res.t,tri.normal,res.contactPoint);
    }
    return Response(false,0,glm::vec3(0),glm::vec3(0));
}

Response GeometricCollision::sphereEdge(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d) {

    float qa = glm::length2(glm::cross(b - a, d - c));
    float qb = 2 * glm::dot(glm::cross(b - a, d - c), glm::cross(a - c, d - c));
    float qc = glm::length2(glm::cross(a - c, d - c)) - glm::length2(d - c);


    float det = qb * qb - 4 * qa * qc;
    if(det < 0) {
        return Response(false,0,glm::vec3(0),glm::vec3(0));
    }

    float root = glm::sqrt(det);
    float t = glm::min(-qb - root, -qb + root)/(2 * qa);
    if(t < 0 || t > 1) {
        return Response(false,t,glm::vec3(0),glm::vec3(0));
    }

    glm::vec3 dir = b - a;
    glm::vec3 p = a + dir * t;

    float isBetween = glm::dot(p - c, d - c)/glm::length2(d - c);



    //this isnt being calculated correctly and thats the problem
    if(0 < isBetween && isBetween < 1) {
//        glm::vec3 normal = glm::normalize(d - c);
//        glm::vec3 dc = d - c;
//        glm::vec3 pPrime = glm::proj(p, dc);
//        pPrime = pPrime + c;

        glm::vec3 pPrime = c + (d - c)*isBetween;
        return Response(true, t,glm::normalize(p - pPrime), pPrime);
    }
    return Response(false, t,glm::vec3(0),glm::vec3(0));
}


Response GeometricCollision::ellipsoidTriangle(Ellipsoid e, glm::vec3 endCenter, MeshTriangle tri) {
    glm::vec3 scaleVec = glm::vec3(1.f/e.scale.x,1.f/e.scale.y,1.f/e.scale.z);
    glm::vec3 a = glm::vec3(*tri.v1) * scaleVec;
    glm::vec3 b = glm::vec3(*tri.v2) * scaleVec;
    glm::vec3 c = glm::vec3(*tri.v3) * scaleVec;
    MeshTriangle transformed = MeshTriangle(&a,&b,&c);
    glm::vec3 newEnd = glm::vec3(endCenter.x/e.scale.x,endCenter.y/e.scale.y,endCenter.z/e.scale.z);
    glm::vec3 newStart = glm::vec3(e.center.x/e.scale.x,e.center.y/e.scale.y,e.center.z/e.scale.z);


    Response r = sphereTriangle(newStart, newEnd, transformed);
    if(!r.collided) {
        return r;
    }

    glm::vec3 p = r.contactPoint * e.scale;

    return Response(r.collided,r.t,glm::normalize(r.normal/e.scale),p);
}

Response GeometricCollision::sphereTriangle(glm::vec3 startCenter, glm::vec3 endCenter, MeshTriangle tri) {

    Response interior = sphereInterior(startCenter, endCenter, tri);
    if(interior.collided) {
        return interior;
    }

    float minT = 4;
    glm::vec3 norm = glm::vec3(0);
    glm::vec3 p = glm::vec3(0);

    Response edge1 = sphereEdge(startCenter, endCenter, *tri.v1, *tri.v2);

    Response edge2 = sphereEdge(startCenter, endCenter, *tri.v2, *tri.v3);

    Response edge3 = sphereEdge(startCenter, endCenter, *tri.v3, *tri.v1);

    if(edge1.collided && edge1.t < minT) {
        minT = edge1.t;
        norm = edge1.normal;
        p = edge1.contactPoint;
    }
    if(edge2.collided && edge2.t < minT) {
        minT = edge2.t;
        norm = edge2.normal;
        p = edge2.contactPoint;
    }
    if(edge3.collided && edge3.t < minT) {
        minT = edge3.t;
        norm = edge3.normal;
        p = edge3.contactPoint;
    }

    Response v1 = sphereVertex(startCenter, endCenter, *tri.v1);
    Response v2 = sphereVertex(startCenter, endCenter, *tri.v2);
    Response v3 = sphereVertex(startCenter, endCenter, *tri.v3);

    if(v1.collided && v1.t < minT) {
        minT = v1.t;
        norm = v1.normal;
        p = v1.contactPoint;
    }
    if(v2.collided && v2.t < minT) {
        minT = v2.t;
        norm = v2.normal;
        p = v2.contactPoint;
    }
    if(v3.collided && v3.t < minT) {
        minT = v3.t;
        norm = v3.normal;
        p = v3.contactPoint;
    }

    if(minT > 1) {
        return Response(false, 0, norm, p);
    } else {
        return Response(true, minT,norm, p);
    }


}

Response GeometricCollision::sphereVertex(glm::vec3 a, glm::vec3 b, glm::vec3 v) {
    float qa = glm::length2(b - a);

    float qb = -2.f * glm::dot(b - a, v - a);
    float qc = glm::length2(v - a) - 1;

    float det = qb * qb - 4 * qa * qc;
    if(det <= 0) {
        return Response(false,0,glm::vec3(0), glm::vec3(0));
    }

    float root = glm::sqrt(det);
//    float t = glm::min(-qb - root, -qb + root)/(2 * qa);
    float t1 = (-qb + root)/(2 * qa);
    float t2 = (-qb - root)/(2 * qa);
    if(t1 < 0) {
        t1 = 1.1f;
    }
    if(t2 < 0) {
        t2 = 1.1;
    }
    float t = glm::min(t1,t2);
    if(t < 0 || t > 1) {
        return Response(false,t,glm::vec3(0), glm::vec3(0));
    }


//    return Response(true,t, glm::normalize(a - v), a + (b - a) * t);
    return Response(true,t, glm::normalize(a - v), v);

}


Response GeometricCollision::raySphere(glm::vec3 startCenter, glm::vec3 endCenter, glm::vec3 sphereCenter) {
    float a = glm::length2(endCenter - sphereCenter);
    float b = -2 * glm::dot(endCenter - sphereCenter, startCenter - sphereCenter);
    float c = glm::length2(startCenter - sphereCenter) - 1;

    float det = b * b - 4 * a * c;

    if(det < 0) {
        qDebug("det negative");
        return Response(false, 0,glm::vec3(0),glm::vec3(0));
    } else {
        float root = glm::sqrt(det);
        float t1 = (-b + root)/(2 * a);
        float t2 = (-b - root)/(2 * a);

        glm::vec3 normal = -glm::normalize(endCenter - startCenter);
        glm::vec3 t1p = startCenter + t1 * (endCenter - startCenter);
        glm::vec3 t2p = startCenter + t2 * (endCenter - startCenter);



        if(t1 < 0 || t1 > 1) {
            if(t2 < 0 || t2 > 1) {
                qDebug("vals");
                std::cout << glm::to_string(t1);
                std::cout << glm::to_string(t2);
                return Response(false, 0,glm::vec3(0),glm::vec3(0));
            } else {
                return Response(true, t2, normal, t2p);
            }
        } else {
            if(t2 < 0 || t2 > 1) {
                return Response(true, t1, normal, t1p);
            } else {
                if(t1 < t2) {
                    return Response(true, t1, normal, t1p);
                } else {
                    return Response(true, t2, normal, t2p);
                }
            }
        }
    }
}


Response GeometricCollision::rayTriangle(glm::vec3 startCenter, glm::vec3 endCenter, MeshTriangle tri) {

    glm::vec3 diff = endCenter - startCenter - tri.normal;

    float dott = glm::dot(tri.normal,diff);
    glm::vec3 collisionNormal = tri.normal;
    if(dott < 0) {
        collisionNormal = -tri.normal;
    }

    float top = glm::dot(collisionNormal, startCenter - *tri.v1);
    float bottom = glm::dot(collisionNormal, diff);
    float t = -top/bottom;


    if(t < 0 || t > 1) {
        return Response(false,t, glm::vec3(0),glm::vec3(0));
    }

    glm::vec3 inPlane = startCenter  + t * (diff);

    glm::vec3 n1 = glm::cross(*tri.v1 - inPlane, inPlane - *tri.v2);
    glm::vec3 n2 = glm::cross(*tri.v2 - inPlane, inPlane - *tri.v3);
    glm::vec3 n3 = glm::cross(*tri.v3 - inPlane, inPlane - *tri.v1);


    if(glm::dot(n1,n2) > 0 && glm::dot(n2,n3) > 0) {
        return Response(true,t,collisionNormal, inPlane);
    }
    return Response(false,t,collisionNormal,glm::vec3(0));
}

//This is useless now i think??
glm::vec3 GeometricCollision::respond(Ellipsoid e, Response r, glm::vec3 vel, int numSlides) {

//    glm::vec3 finalDisp = glm::vec3(0);

    glm::vec3 slide = vel;
    for(int i = 0; i < numSlides; i++) {
        slide = slide * .8f;
        slide = slideEllipsoid(r.normal, slide);
    }

    glm::vec3 dir = glm::normalize(vel);
    glm::vec3 final = e.center + vel * r.t;
    return final + slide - dir * .0001f;

}


glm::vec3 GeometricCollision::slideEllipsoid(glm::vec3 normal, glm::vec3 disp) {
    //project displacement onto the collision normal to get the component that is perpendicular to the plane
    glm::vec3 perp = glm::proj(disp, normal);
    //subtract the perpendicular component
    return disp - perp;
}

