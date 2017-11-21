#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

/**
 * QTs "frame" class, opens a window for you to draw in and interact with.
 *
 * @brief The MainWindow class
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(size_t resolution, std::string input, std::string output, QWidget *parent = 0);
    ~MainWindow();

    size_t m_resolution;
    std::string m_input;
    std::string m_output;


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
