#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "util/CommonIncludes.h"
#include "src/engine/graphics/shape.h"
#include <QHash>
#include <qgl.h>
#include "src/engine/graphics/shader.h"

class Camera;

class Graphics
{
public:
    Graphics();
    virtual ~Graphics();

    void loadTexture(QString key, QString path);

    void bindTexture(QString key);
    void unbindTexture();

    void load3dShader(const char* vert, const char* frag);
    //GLuint get3dShader();

    void loadUIShader(const char* vert, const char* frag);
    //GLuint getUIShader();

    void begin3dDraw();
    void beginUIDraw();
    void endDraw();

    void drawQuad(glm::mat4 m, QString textureKey);

    void drawCylinder(glm::mat4 m);
    void drawCylinder(glm::mat4 m, glm::vec3 color);

    void drawTriangle(glm::mat4 m, glm::vec3 color);

    void drawSphere(glm::mat4 m, glm::vec3 color);

    void drawShape(std::shared_ptr<Shape> s, glm::mat4 m);
    void drawShape(std::shared_ptr<Shape> s, glm::mat4 m, QString textureKey);
    void drawShape(std::shared_ptr<Shape> s, glm::mat4 m, glm::vec3 color);

    void drawUI(const QString textureKey, const glm::vec3 pos, const glm::vec3 scale);

    void drawFullScreen(const QString textureKey);

    void transformCamera(const Camera c);

private:
    QHash<QString, GLuint> m_textureHash;
//    GLuint m_3dShader;
//    GLuint m_UIShader;

    std::shared_ptr<Shader> m_3dShader;
    std::shared_ptr<Shader> m_UIShader;

    Shape *m_quad;
    Shape *m_cyl;
    Shape *m_elip;
    Shape *m_tri;
    Shape *m_ui;

    bool m_drawingUI;

    uint m_time;

    void setBaseUniforms(const glm::mat4 m, const glm::vec3 color);

};

#endif // GRAPHICS_H
