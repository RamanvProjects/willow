#ifndef SHADER_H
#define SHADER_H

#include <QHash>
#include "util/CommonIncludes.h"

enum GLType {
    Int,
    Float,
    Vec3,
    Vec4,
    Mat4,
};

struct Uniform {
    GLType type;
    GLint loc;
};

class Shader
{
public:
    Shader(const char* vert, const char* frag);

    void use() const;

    //get the location of the uniform of a shader. returns true if success, false if fail
    bool initUniformLocation(const char* name, const GLType uniformType);

    void setUniform(const char* name, const GLint val);
    void setUniform(const char* name, const GLfloat val);
    void setUniform(const char* name, const glm::vec3 val);
    void setUniform(const char* name, const glm::vec4 val);
    void setUniform(const char* name, const glm::mat4 val);


private:
    GLuint m_shader;

    QHash<QString, Uniform> m_locs;
};

#endif // SHADER_H
