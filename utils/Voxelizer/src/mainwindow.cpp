#include "mainwindow.h"
#include "src/ui_mainwindow.h"

MainWindow::MainWindow(size_t resolution, std::__cxx11::string input, std::__cxx11::string output, QWidget *parent) :
    QMainWindow(parent),
    m_resolution(resolution),
    m_input(input),
    m_output(output),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this,resolution,input,output);
}

MainWindow::~MainWindow()
{
    delete ui;
}

