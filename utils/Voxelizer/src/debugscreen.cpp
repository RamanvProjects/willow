#include "debugscreen.h"
#include "src/engine/voxelizer.h"
#include "src/engine/geometric/mesh/graphicalmesh.h"
#include "src/engine/base/application.h"

DebugScreen::DebugScreen(Application *app, size_t resolution, std::string pathToObj, std::string output) :
    Screen::Screen(app),
    m_mesh(std::make_shared<GraphicalMesh>(pathToObj.c_str(), true)),
    m_cam(new Camera(glm::vec2(16.f,9.f)))
{
    size_t size = 64*64*64;
    m_data = new bool[size];
    Voxelizer::meshToVoxel(m_mesh,m_data,resolution);
    Voxelizer::writeVoxelizedData(m_data, output.c_str(), resolution);
}

DebugScreen::~DebugScreen()
{
    delete m_cam;
    delete m_data;
}

void DebugScreen::initializeDraw(Graphics *g)
{
    g->load3dShader("res/shaders/shader.vert", "res/shaders/shader.frag");
}

void DebugScreen::draw(Graphics *gr)
{
    gr->begin3dDraw();
    gr->transformCamera(*m_cam);
    for(size_t x = 0; x < 64; x++)
    {
        for(size_t y = 0; y < 64; y++)
        {
            for(size_t z = 0; z < 64; z++)
            {
                size_t index = Voxelizer::index(x,y,z,64,64);
                if(m_data[index])
                {
                    glm::mat4 mat;
                    mat = glm::translate(mat, glm::vec3(x,y,z) * glm::vec3(.3f));
                    gr->drawSphere(mat,glm::vec3(1,1,0));
                }
            }
        }
    }

    gr->endDraw();
}


void DebugScreen::keyPressEvent(QKeyEvent *event)
{
    float yaw = m_cam->getYaw(); // the angle of the rotation about the y-axis
    glm::vec3 dir(cosf(yaw),0,sinf(yaw)); // the direction we are looking in
    glm::vec3 perp(dir.z,0,-dir.x); // the perpendicular to dir
    //forward-backward movement
    if(event->key() == Qt::Key_W) {
        m_cam->translate(dir);
    }
    else if(event->key() == Qt::Key_S) {
        m_cam->translate(dir * -1.f);
    }
    //strafe movement
    else if(event->key() == Qt::Key_A) {
        m_cam->translate(perp);
    }
    else if(event->key() == Qt::Key_D) {
        m_cam->translate(perp * -1.f);
    }
}


void DebugScreen::mouseMoveEvent(QMouseEvent *event, int deltaX, int deltaY)
{
    m_cam->rotate(deltaX / 100.f, -deltaY / 100.f);
}
