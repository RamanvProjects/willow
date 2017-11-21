#ifndef APPLICATION_H
#define APPLICATION_H
#include "application.h"
#include "util/CommonIncludes.h"
#include "src/engine/base/screen.h"
#include "src/engine/graphics/graphics.h"
#include <QApplication>
#include <QKeyEvent>
#include <QStack>

class View;

class Application
{
public:
    Application(View *v);
    ~Application();

    void pushScreen(Screen *toPush);

    void pushScreen(Screen *toPush, bool popTop);

    Screen *popScreen();

    void resizeGL(int w, int h);

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event, int deltaX, int deltaY);

    void mouseReleaseEvent(QMouseEvent *event);

    void wheelEvent(QWheelEvent *event);

    void keyPressEvent(QKeyEvent *event);

    void keyReleaseEvent(QKeyEvent *event);

    void onUI();

    void tick(float seconds);

    void draw();

    void toggleMouse(bool isHidden);

private:
    QStack<Screen *> *m_screenStack;

    Graphics *m_graphics;

    View *m_view;
};

#endif // APPLICATION_H
