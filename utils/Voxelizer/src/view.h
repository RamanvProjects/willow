#ifndef VIEW_H
#define VIEW_H

#include "util/CommonIncludes.h"
#include "src/engine/base/application.h"
#include <qgl.h>
#include <QTime>
#include <QTimer>

#define FRAMES_TO_AVERAGE 30

/**
 * @brief The View class
 */
class View : public QGLWidget
{
    Q_OBJECT

public:
    View(QWidget *parent, size_t resolution, std::string input, std::string output);
    ~View();

private:
    QTime time;
    QTimer timer;


    std::string m_input;
    std::string m_output;
    size_t m_resolution;
    Application *m_app;

    float fps;
    int frameIndex;
    float frameTimes[FRAMES_TO_AVERAGE];

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void wheelEvent(QWheelEvent *event);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);


private slots:
    void tick();
};

#endif // VIEW_H

