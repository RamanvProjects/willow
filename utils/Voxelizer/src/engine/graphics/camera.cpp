#include "camera.h"
#include "glm/gtc/matrix_access.hpp"

#include "src/engine/graphics/shader.h"

Camera::Camera(glm::vec2 size)
{
//    m_eye = glm::vec3(0,2,0);
    m_eye = glm::vec3(0.f);
    m_yaw = 0;
    m_pitch = 0;
    m_fov = 60;
    m_ratio = size;
    m_isPerspectiveToggled = false;
    m_needsUpdate = true;
    updatePlanes();
}

//void Camera::transform(GLuint shader) const{
void Camera::transform(std::shared_ptr<Shader> shader) const{
//    glUseProgram(shader);


    shader->use();
    glm::mat4 proj = glm::perspective(glm::radians(m_fov), m_ratio.x/m_ratio.y, .01f, 200.f);
    //look is initially 1,0,0
    glm::vec3 look = glm::normalize(glm::vec3(glm::cos(m_yaw) * glm::cos(m_pitch), glm::sin(m_pitch), glm::sin(m_yaw) * glm::cos(m_pitch)));
    glm::vec3 eye = m_eye;
    if(m_isPerspectiveToggled) {
        eye = eye + -5.f*look;
    }
    glm::mat4 view = glm::lookAt(eye, eye + look, glm::vec3(0, 1, 0));

//    glUniformMatrix4fv(glGetUniformLocation(shader, "v"), 1, GL_FALSE, glm::value_ptr(view));
//    glUniformMatrix4fv(glGetUniformLocation(shader, "p"), 1, GL_FALSE, glm::value_ptr(proj));
    shader->setUniform("v", view);
    shader->setUniform("p", proj);
}

QPair<glm::vec3,glm::vec3> Camera::getLook() {
    glm::vec3 look = glm::normalize(glm::vec3(glm::cos(m_yaw) * glm::cos(m_pitch), glm::sin(m_pitch), glm::sin(m_yaw) * glm::cos(m_pitch)));
    glm::vec3 eye = m_eye;
    if(m_isPerspectiveToggled) {
        eye = eye + -5.f*look;
    }
    return QPair<glm::vec3,glm::vec3>(eye,look);
}


void Camera::translate(glm::vec3 vec) {
    m_eye = m_eye + vec;
    m_needsUpdate = true;
}

void Camera::setEye(glm::vec3 eye) {
    m_eye = eye;
    m_needsUpdate = true;
}

void Camera::togglePerspective(){
    m_isPerspectiveToggled = !m_isPerspectiveToggled;
    m_needsUpdate = true;
}

void Camera::rotate(float yaw, float pitch) {
    float sum = m_pitch + pitch;
    m_yaw = m_yaw + yaw;
    if(sum >= -glm::pi<float>()/2 && sum <= glm::pi<float>()/2) {
        m_pitch = sum;
    }
    m_needsUpdate = true;
}

glm::vec4 *Camera::getPlanes() {
    return m_planes;
}

float Camera::getYaw() {
    return m_yaw;
}

bool Camera::needsUpdate() {
    return m_needsUpdate;
}

void Camera::updatePlanes() {
    glm::mat4 proj = glm::perspective(glm::radians(m_fov), m_ratio.x/m_ratio.y, .01f, 200.f);
    glm::vec3 look = glm::normalize(glm::vec3(glm::cos(m_yaw) * glm::cos(m_pitch), glm::sin(m_pitch), glm::sin(m_yaw) * glm::cos(m_pitch)));
    glm::vec3 eye = m_eye;
    if(m_isPerspectiveToggled) {
        eye = eye + -10.f*look;
    }
    glm::mat4 view = glm::lookAt(eye, eye + look, glm::vec3(0, 1, 0));

    glm::mat4 planes = proj * view;
    glm::vec4 r0 = glm::vec4(planes[0][0], planes[1][0], planes[2][0], planes[3][0]);
    glm::vec4 r1 = glm::vec4(planes[0][1], planes[1][1], planes[2][1], planes[3][1]);
    glm::vec4 r2 = glm::vec4(planes[0][2], planes[1][2], planes[2][2], planes[3][2]);
    glm::vec4 r3 = glm::vec4(planes[0][3], planes[1][3], planes[2][3], planes[3][3]);

    m_planes[0] = r3 - r0;
    m_planes[1] = r3 - r1;
    m_planes[2] = r3 - r2;
    m_planes[3] = r3 + r0;
    m_planes[4] = r3 + r1;
    m_planes[5] = r3 + r2;

    m_needsUpdate = false;
}

glm::vec3 Camera::convertToScreenSpace(const glm::vec3 pos) {
//    glm::vec4 fourVec = glm::vec4(pos.x,pos.y,pos.z,1.f);
    glm::mat4 proj = glm::perspective(glm::radians(m_fov), m_ratio.x/m_ratio.y, .01f, 200.f);
    glm::vec3 look = glm::normalize(glm::vec3(glm::cos(m_yaw) * glm::cos(m_pitch), glm::sin(m_pitch), glm::sin(m_yaw) * glm::cos(m_pitch)));
    glm::vec3 eye = m_eye;
    if(m_isPerspectiveToggled) {
        eye = eye + -5.f*look;
    }
    glm::mat4 view = glm::lookAt(eye, eye + look, glm::vec3(0, 1, 0));
    glm::vec4 fourVec = proj * view * glm::vec4(pos,1.f);
    return glm::vec3(fourVec.x,fourVec.y,fourVec.z);
}

