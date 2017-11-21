#ifndef CAMERA_H
#define CAMERA_H

#include "util/CommonIncludes.h"

#include <QPair>

class Shader;

class Camera
{

private:
    glm::vec3 m_eye;
    float m_yaw;
    float m_pitch;
    float m_fov;

    bool m_isPerspectiveToggled;
    glm::vec2 m_ratio;

    bool m_needsUpdate;

    //order is -x, -y, -z, x, y, z
    glm::vec4 m_planes[6] = {};


public:
    Camera(glm::vec2 size);

//    void transform(GLuint shader) const;
    void transform(std::shared_ptr<Shader> shader) const;

    void translate(glm::vec3 vec);
    void rotate(float yaw, float pitch);

    QPair<glm::vec3,glm::vec3> getLook();

    void togglePerspective();

    glm::vec4 *getPlanes();

    void setEye(glm::vec3 eye);

    float getYaw();

    bool needsUpdate();

    void updatePlanes();

    glm::vec3 convertToScreenSpace(const glm::vec3 pos);

    virtual ~Camera() {}
};

#endif // CAMERA_H
