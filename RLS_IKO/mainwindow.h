#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QXmlStreamReader>
#include <QString>
#include <QtWidgets/QFileDialog>
#include "simulation.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QString fileName;
    Simulation* sim;
    QList<MapObj*> *objects;
    QScrollArea* scrollSim;

private slots:
    void open();
    void readDataBaseElement( QXmlStreamReader &xmlReader );
    void readRouteElement(QXmlStreamReader &xmlReader , MapObj *obj);
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
