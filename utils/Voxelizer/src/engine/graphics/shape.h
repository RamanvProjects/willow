#ifndef SHAPE_H
#define SHAPE_H
#include "util/CommonIncludes.h"


class Shape
{
public:
    Shape();
    virtual ~Shape();

    void setVertexData(GLfloat *data, GLsizeiptr size, int numVertices);

    void setAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, size_t pointer);

    void setTransform(glm::mat4 *mat);

    glm::mat4 *getTransform();

    void draw() const;

private:
    GLuint m_vbo;
    GLuint m_vao;
    GLfloat *m_data;
    int m_numVerts;
};

#endif // SHAPE_H
