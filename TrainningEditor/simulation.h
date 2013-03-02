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
#include <math.h>
#include "mapobj.h"
#include "routepoint.h"
#include "options.h"

class Simulation : public QWidget
{
    Q_OBJECT

private:
    Options* opt;
    QList<MapObj*> *objects;
    QList<RoutePoint> *curPoints;
    QList<QList<RoutePoint>::iterator> *lastInCurRoute;
    QTimer *ptimer;
    QTime *timeElapsed;
    int timeCounter;
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
