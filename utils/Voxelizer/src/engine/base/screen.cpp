#include "screen.h"

#include "src/engine/base/application.h"

Screen::Screen(Application *app)
{
    m_app = app;
    //m_graphics = new Graphics();
}

Screen::~Screen() {
}

Application *Screen::getApp() {
    return m_app;
}
