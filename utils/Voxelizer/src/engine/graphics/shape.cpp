#include "shape.h"
#include "util/CommonIncludes.h"


//GLuint m_vbo;
//GLuint m_vao;
//int m_numVerts;
Shape::Shape() : m_data(nullptr)
{
    glGenBuffers(1,&m_vbo);
    glGenVertexArrays(1, &m_vao);
}

Shape::~Shape() {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
}

void Shape::setVertexData(GLfloat *data, GLsizeiptr size, int numVertices) {
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    m_numVerts = numVertices;

    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    m_data = data;
}

void Shape::setAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, size_t pointer) {
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, type, normalized, stride, (void *)pointer);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
}

void Shape::draw() const{
    if(!m_data)
    {
        return;
    }
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, m_numVerts);
    glBindVertexArray(0);
}

