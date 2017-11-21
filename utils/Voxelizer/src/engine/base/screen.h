#ifndef SCREEN_H
#define SCREEN_H

#include <QApplication>
#include <QKeyEvent>

#include "src/engine/graphics/graphics.h"

class Application;

class Screen
{
public:

    Screen(Application *app);

    ~Screen();

    virtual void resizeGL(int w, int h) {}

    virtual void mousePressEvent(QMouseEvent *event) {}

    virtual void mouseMoveEvent(QMouseEvent *event, int deltaX, int deltaY) {}

    virtual void mouseReleaseEvent(QMouseEvent *event) {}

    virtual void wheelEvent(QWheelEvent *event) {}

    virtual void keyPressEvent(QKeyEvent *event) {}

    virtual void keyReleaseEvent(QKeyEvent *event) {}

    virtual void tick(float seconds) {}

    virtual void draw(Graphics *gr) {}

    virtual void initializeDraw(Graphics *g) {}

    virtual void onUI(Graphics *g) {}

    //Graphics *getGraphics();

    Application *getApp();


protected:

    Application *m_app;

    //Graphics *m_graphics;
};

#endif // SCREEN_H
