#include "graphics.h"
#include "util/CommonIncludes.h"
#include "util/CylinderData.h"
#include "util/SphereData.h"
#include <QHash>
#include <QApplication>
#include <QString>
#include "src/engine/graphics/camera.h"

//QHash<QString, GLuint> textureHash;
//GLuint m_shader;

Graphics::Graphics()
{
    m_quad = new Shape();

    m_cyl = new Shape();

    m_elip = new Shape();

    m_tri = new Shape();

    m_ui  = new Shape();

    GLfloat quadData[48] = {
            -.5f,0.f,-.5f, 0.f,1.f,0.f, 0.f,1.f,
            -.5f,0.f,.5f, 0.f,1.f,0.f, 0.f,0.f,
            .5f,0.f,-.5f, 0.f,1.f,0.f, 1.f,1.f,
            .5f,0.f,-.5f, 0.f,1.f,0.f, 1.f,1.f,
            -.5f,0.f,.5f, 0.f,1.f,0.f, 0.f,0.f,
            .5f,0.f,.5f, 0.f,1.f,0.f, 1.f,0.f
        };

    GLfloat uiData[48] = {
            -.5f,-.5f,0.f, 0.f,0.f,-1.f, 0.f,0.f,
            .5f,-.5f,0.f, 0.f,0.f,-1.f, 1.f,0.f,
            .5f,.5f,0.f, 0.f,0.f,-1.f, 1.f,1.f,
            .5f,.5f,0.f, 0.f,0.f,-1.f, 1.f,1.f,
            -.5f,.5f,0.f, 0.f,0.f,-1.f, 0.f,1.f,
            -.5f,-.5f,0.f, 0.f,0.f,-1.f, 0.f,0.f
        };

    GLfloat triData[24] = {
        -.5f,0.f,-.5f, 0.f,1.f,0.f, 0.f,1.f,
        -.5f,0.f,.5f, 0.f,1.f,0.f, 0.f,0.f,
        .5f,0.f,-.5f, 0.f,1.f,0.f, 1.f,1.f,
    };

    m_quad->setVertexData(quadData, 48 * sizeof(GLfloat), 6);
    m_quad->setAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*8, 0);
    m_quad->setAttribute(1, 3, GL_FLOAT, GL_TRUE, sizeof(GLfloat)*8,sizeof(GLfloat)*3);
    m_quad->setAttribute(2,2,GL_FLOAT,GL_FALSE,sizeof(GLfloat)*8,sizeof(GLfloat)*6);

    m_ui->setVertexData(uiData, 48 * sizeof(GLfloat), 6);
    m_ui->setAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*8, 0);
    m_ui->setAttribute(1, 3, GL_FLOAT, GL_TRUE, sizeof(GLfloat)*8,sizeof(GLfloat)*3);
    m_ui->setAttribute(2,2,GL_FLOAT,GL_FALSE,sizeof(GLfloat)*8,sizeof(GLfloat)*6);

    m_tri->setVertexData(triData, 24 * sizeof(GLfloat), 3);
    m_tri->setAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*8, 0);
    m_tri->setAttribute(1, 3, GL_FLOAT, GL_TRUE, sizeof(GLfloat)*8,sizeof(GLfloat)*3);
    m_tri->setAttribute(2,2,GL_FLOAT,GL_FALSE,sizeof(GLfloat)*8,sizeof(GLfloat)*6);

    m_cyl->setVertexData(cylinderVertexBufferData, cylinderDataSize, cylinderVertexCount);
    m_cyl->setAttribute(0,3,GL_FLOAT,GL_FALSE,sizeof(GLfloat)*8,0);
    m_cyl->setAttribute(1,3,GL_FLOAT,GL_TRUE,sizeof(GLfloat)*8,sizeof(GLfloat)*3);
    m_cyl->setAttribute(2,2,GL_FLOAT,GL_FALSE,sizeof(GLfloat)*8,sizeof(GLfloat)*6);

    m_elip->setVertexData(vertexBufferData, arraySize, vertexCount);
    m_elip->setAttribute(0,3,GL_FLOAT,GL_FALSE,sizeof(GLfloat)*8,0);
    m_elip->setAttribute(1,3,GL_FLOAT,GL_TRUE,sizeof(GLfloat)*8,sizeof(GLfloat)*3);
    m_elip->setAttribute(2,2,GL_FLOAT,GL_FALSE,sizeof(GLfloat)*8,sizeof(GLfloat)*6);

    //m_time = 0;

    m_drawingUI = false;
}

Graphics::~Graphics() {
    delete m_cyl;
    delete m_quad;
    delete m_elip;
    delete m_tri;
    delete m_ui;
}

void Graphics::loadTexture(QString key, QString path) {
    QImage img(path);
    img = QGLWidget::convertToGLFormat(img);
    GLuint imageID;
    glGenTextures(1, &imageID);
    glBindTexture(GL_TEXTURE_2D, imageID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width(), img.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.bits());
    glBindTexture(GL_TEXTURE_2D, 0);
    m_textureHash.insert(key, imageID);
    img = QImage();
}

void Graphics::setBaseUniforms(const glm::mat4 m, const glm::vec3 color) {
    if(m_drawingUI) {
//        glUniformMatrix4fv(glGetUniformLocation(m_UIShader,"m"),1,GL_FALSE,glm::value_ptr(m));
//        glUniform3fv(glGetUniformLocation(m_UIShader,"color"),1,glm::value_ptr(color));
        m_UIShader->setUniform("m", m);
        m_UIShader->setUniform("color",color);
    } else {
//        glUniformMatrix4fv(glGetUniformLocation(m_3dShader,"m"),1,GL_FALSE,glm::value_ptr(m));
//        glUniform3fv(glGetUniformLocation(m_3dShader,"color"),1,glm::value_ptr(color));
        m_3dShader->setUniform("m", m);
        m_3dShader->setUniform("color",color);
    }
}

void Graphics::transformCamera(const Camera c) {
    c.transform(m_3dShader);
}


//something seems weird here cause we aren't binding a texture before setting uniforms sooooooo
void Graphics::bindTexture(QString key) {
    m_3dShader->setUniform("useTexture", 1);
    m_3dShader->setUniform("tex", GL_TEXTURE0);
//    glUniform1i(glGetUniformLocation(m_3dShader, "useTexture"), 1);
//    glUniform1i(glGetUniformLocation(m_3dShader, "tex"), GL_TEXTURE0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureHash[key]);
}
void Graphics::unbindTexture() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
}



void Graphics::load3dShader(const char* vert, const char* frag){
    m_3dShader = std::make_shared<Shader>(vert, frag);
    m_3dShader->initUniformLocation("m", Mat4);
    m_3dShader->initUniformLocation("p", Mat4);
    m_3dShader->initUniformLocation("v", Mat4);
    m_3dShader->initUniformLocation("useTexture", Int);
    m_3dShader->initUniformLocation("tex", Int);
    m_3dShader->initUniformLocation("color", Vec3);

}


void Graphics::loadUIShader(const char* vert, const char* frag){
    m_UIShader = std::make_shared<Shader>(vert, frag);
    m_UIShader->initUniformLocation("m", Mat4);
    m_UIShader->initUniformLocation("useTexture", Int);
    m_UIShader->initUniformLocation("tex", Int);
    m_UIShader->initUniformLocation("color", Vec3);
}


void Graphics::begin3dDraw() {
    m_drawingUI = false;
    m_3dShader->use();
    m_time++;
}

void Graphics::beginUIDraw() {
    m_drawingUI  = true;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    m_UIShader->use();
}

void Graphics::endDraw() {
    glDisable(GL_BLEND);
    glUseProgram(0);
    m_drawingUI = false;
}


void Graphics::drawShape(std::shared_ptr<Shape> s, glm::mat4 m) {
    assert(!m_drawingUI);
    glm::vec3 color = glm::vec3(.47f,0.53f,0.60f);
    setBaseUniforms(m,color);
    s->draw();
}

void Graphics::drawShape(std::shared_ptr<Shape> s, glm::mat4 m, glm::vec3 color) {
    assert(!m_drawingUI);
    setBaseUniforms(m, color);
    s->draw();
}

void Graphics::drawShape(std::shared_ptr<Shape> s, glm::mat4 m, QString textureKey) {
    assert(!m_drawingUI);


//    glUniformMatrix4fv(glGetUniformLocation(m_3dShader,"m"),1,GL_FALSE,glm::value_ptr(m));
    glm::vec3 color = glm::vec3(0.f,1.f,0.f);
//    glUniform3fv(glGetUniformLocation(m_3dShader,"color"),1,glm::value_ptr(color));

    setBaseUniforms(m, color);

    bindTexture(textureKey);
    s->draw();
    unbindTexture();
}

void Graphics::drawCylinder(glm::mat4 m) {
    assert(!m_drawingUI);

    glm::vec3 color = glm::vec3(0.f,1.f,0.f);
    setBaseUniforms(m, color);
    m_cyl->draw();
}

void Graphics::drawCylinder(glm::mat4 m, glm::vec3 color) {
    assert(!m_drawingUI);


    setBaseUniforms(m, color);
    m_cyl->draw();
}

void Graphics::drawTriangle(glm::mat4 m, glm::vec3 color) {
    assert(!m_drawingUI);

    setBaseUniforms(m, color);
    m_tri->draw();
}

void Graphics::drawSphere(glm::mat4 m, glm::vec3 color) {
    assert(!m_drawingUI);

    setBaseUniforms(m, color);
    m_elip->draw();
}


void Graphics::drawFullScreen(const QString textureKey) {
    assert(m_drawingUI);

    glm::mat4 m = glm::mat4(1.f);
    m = glm::scale(m,glm::vec3(2.f));

    glm::vec3 color = glm::vec3(0.f,1.f,0.f);
    setBaseUniforms(m, color);

    m_UIShader->setUniform("useTexture", 1);
    m_UIShader->setUniform("tex", GL_TEXTURE0);


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureHash[textureKey]);
    //for quad normals are positive y
    m_ui->draw();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

//scale is a percent of screen
void Graphics::drawUI(const QString textureKey, const glm::vec3 pos, const glm::vec3 scale) {

    assert(m_drawingUI);

    if(pos.z <= 0) {
        return;
    }

    glm::mat4 m = glm::mat4(1.f);
    m = glm::translate(m, glm::vec3(pos.x/pos.z,pos.y/pos.z,0.f));
    m = glm::scale(m,scale*2.f);

    glm::vec3 color = glm::vec3(0.f,1.f,0.f);

    setBaseUniforms(m, color);

    m_UIShader->setUniform("useTexture", 1);
    m_UIShader->setUniform("tex", GL_TEXTURE0);

//    glUniform1i(glGetUniformLocation(m_UIShader, "useTexture"), 1);
//    glUniform1i(glGetUniformLocation(m_UIShader, "tex"), GL_TEXTURE0);
    //glUniform1i(glGetUniformLocation(m_UIShader, "time"), m_time);

    //we dont explicitly set p and v with glUniformMatrix because i changed the shader to not use those values at all

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureHash[textureKey]);
    m_ui->draw();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Graphics::drawQuad(glm::mat4 m, QString textureKey) {
    assert(!m_drawingUI);

    glm::vec3 color = glm::vec3(0.f,1.f,0.f);

    setBaseUniforms(m, color);

    bindTexture(textureKey);
    m_quad->draw();
    unbindTexture();
}
