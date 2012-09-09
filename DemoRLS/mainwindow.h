#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "indicator.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
    Indicator *ind;
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
