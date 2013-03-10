#ifndef SIMULATION_H
#define SIMULATION_H

#include <QWidget>
#include <QPainter>
#include <QBrush>
#include <QTimer>
#include <QTime>
#include <QList>
#include <QBrush>
#include <Qpen>
#include <QLineF>
#include <QPolygonF>
#include <math.h>
#include <iostream>
#include "mapobj.h"
#include "routepoint.h"
#include "options.h"

typedef struct
{
    MapObj* obj;
    QList<RoutePoint>::iterator lastInCurRoute;
    RoutePoint curPoint;
    double aX;
    double aY;
    double vX0;
    double vY0;
    int timeCounter;
    bool complete;
    double angle;
}SIM_OBJ;

class Simulation : public QWidget
{
    Q_OBJECT

private:
    Options* opt;
    SIM_OBJ* simObjects;
    int objCount;
    QTimer *ptimer;
    QTime *timeElapsed;
    QPen* pen;
    QBrush* brush;

public:
    explicit Simulation(QList<MapObj*> *_objects, QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);
    
signals:
    
public slots:
    void start();
    void pause();
    void stop();
    void updateSimulation();
    
};

#endif // SIMULATION_H
