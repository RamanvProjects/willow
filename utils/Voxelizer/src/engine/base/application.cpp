#include "application.h"
#include "util/CommonIncludes.h"
#include "screen.h"
#include "src/engine/graphics/graphics.h"
#include "src/view.h"
#include <QApplication>
#include <QKeyEvent>
#include <QStack>

Application::Application(View *v) : m_view(v)
{
    m_screenStack = new QStack<Screen *>();
    m_graphics = new Graphics();
}

Application::~Application() {
    while(!m_screenStack->empty()) {
        delete m_screenStack->pop();
    }
    delete m_screenStack;
}

void Application::pushScreen(Screen *toPush) {
    m_screenStack->push(toPush);
    toPush->initializeDraw(m_graphics);
}

void Application::pushScreen(Screen *toPush, bool popTop) {
    if(popTop && !m_screenStack->empty()) {
        delete m_screenStack->pop();
    }
    pushScreen(toPush);
}

Screen *Application::popScreen() {
    return m_screenStack->pop();
}

void Application::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void Application::mousePressEvent(QMouseEvent *event)
{
    m_screenStack->top()->mousePressEvent(event);
}

void Application::mouseMoveEvent(QMouseEvent *event, int deltaX, int deltaY)
{
    m_screenStack->top()->mouseMoveEvent(event, deltaX, deltaY);
}

void Application::mouseReleaseEvent(QMouseEvent *event)
{
    m_screenStack->top()->mouseReleaseEvent(event);
}

void Application::wheelEvent(QWheelEvent *event)
{
    m_screenStack->top()->wheelEvent(event);
}

void Application::keyPressEvent(QKeyEvent *event)
{
    m_screenStack->top()->keyPressEvent(event);
}

void Application::keyReleaseEvent(QKeyEvent *event)
{
    m_screenStack->top()->keyReleaseEvent(event);
}

void Application::tick(float seconds)
{
    m_screenStack->top()->tick(seconds);
}

void Application::draw() {
    m_screenStack->top()->draw(m_graphics);
}

void Application::onUI() {
    m_screenStack->top()->onUI(m_graphics);
}


