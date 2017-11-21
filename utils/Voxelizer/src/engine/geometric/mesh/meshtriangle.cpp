#include "meshtriangle.h"
#include "src/engine/graphics/shape.h"


std::shared_ptr<Shape> MeshTriangle::getShape() {
    glm::vec3 v1 = glm::vec3(*this->v1);
    glm::vec3 v2 = glm::vec3(*this->v2);
    glm::vec3 v3 = glm::vec3(*this->v3);
    std::shared_ptr<Shape> shape = std::make_shared<Shape>();

    GLfloat triData[24] = {
        v1.x,v1.y,v1.z,normal.x,normal.y,normal.z, 0.f,1.f,
        v2.x,v2.y,v2.z,normal.x,normal.y,normal.z, 0.f,1.f,
        v3.x,v3.y,v3.z,normal.x,normal.y,normal.z, 0.f,1.f,
    };
    shape->setVertexData(triData, 24 * sizeof(GLfloat), 3);
    shape->setAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*8, 0);
    shape->setAttribute(1, 3, GL_FLOAT, GL_TRUE, sizeof(GLfloat)*8,sizeof(GLfloat)*3);
    shape->setAttribute(2,2,GL_FLOAT,GL_FALSE,sizeof(GLfloat)*8,sizeof(GLfloat)*6);
    return shape;
}


