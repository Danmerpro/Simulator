#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ind = new Indicator(this);
    setCentralWidget(ind);
}

MainWindow::~MainWindow()
{
    delete ind;
}
