#ifndef DEBUGSCREEN_H
#define DEBUGSCREEN_H

#include "src/engine/base/screen.h"
#include "src/engine/graphics/camera.h"

class GraphicalMesh;
class Voxelizer;

class DebugScreen : public Screen
{
public:
    DebugScreen(Application *app, size_t resolution, std::string pathToObj, std::string outputDir);

    ~DebugScreen();

    virtual void initializeDraw(Graphics *g) override;

    virtual void draw(Graphics *gr);

    virtual void mouseMoveEvent(QMouseEvent *event, int deltaX, int deltaY) override;

    virtual void keyPressEvent(QKeyEvent *event) override;

private:
    Camera *m_cam;
    bool* m_data;
    std::shared_ptr<GraphicalMesh> m_mesh;
};

#endif // DEBUGSCREEN_H
