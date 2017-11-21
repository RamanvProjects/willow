#include "shader.h"
#include "util/ResourceLoader.h"

Shader::Shader(const char* vert, const char* frag){
    m_shader = ResourceLoader::loadShaders(vert, frag);
}

void Shader::use() const{
    glUseProgram(m_shader);
}

bool Shader::initUniformLocation(const char *name, const GLType uniformType) {
    glUseProgram(m_shader);
    GLint loc = glGetUniformLocation(m_shader,name);
    if(loc == -1) {
        return false;
    }
    m_locs.insert(name, Uniform{uniformType,loc});
    glUseProgram(0);
    return true;
}

void Shader::setUniform(const char *name, const GLint val) {
    Uniform uniform = m_locs.value(name);
    assert(uniform.type == Int);

    glUniform1i(uniform.loc, val);
}

void Shader::setUniform(const char *name, const GLfloat val) {
    Uniform uniform = m_locs.value(name);
    assert(uniform.type == Float);

    glUniform1f(uniform.loc, val);
}

void Shader::setUniform(const char *name, const glm::vec3 val) {
    Uniform uniform = m_locs.value(name);
    assert(uniform.type == Vec3);

    glUniform3fv(uniform.loc,1,glm::value_ptr(val));
}

void Shader::setUniform(const char *name, const glm::vec4 val) {
    Uniform uniform = m_locs.value(name);
    assert(uniform.type == Vec4);

    glUniform4fv(uniform.loc,1,glm::value_ptr(val));

}

//something is wonky with the hashmap?
void Shader::setUniform(const char *name, const glm::mat4 val) {
    Uniform uniform = m_locs.value(name);
//    assert(uniform.type == Mat4);
    if(uniform.type != Mat4) {
        int tt = 5;
    }

    glUniformMatrix4fv(uniform.loc, 1, GL_FALSE, glm::value_ptr(val));
}


//glUniformMatrix4fv(glGetUniformLocation(m_UIShader, "v"), 1, GL_FALSE, glm::value_ptr(ident));
