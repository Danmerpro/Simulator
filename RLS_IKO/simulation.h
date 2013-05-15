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
#include <QPalette>
#include <iostream>
#include <QImage>

#include "mapobj.h"
#include "routepoint.h"

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
    SIM_OBJ* simObjects;
    int objCount;
    QTimer *ptimer;
    QTimer *timerForMenu;
    QTime *timeElapsed;
    QPen* pen;
    QBrush* brush;
    QLineF* radarLine;
    double radarAngle;
    QImage* ScreenTemplate;
    QImage* ScreeenBuffer;

public:
    explicit Simulation(QList<MapObj*> *_objects, QWidget *parent = 0);
    void DrawCirclPart(QPainter& painter,double cx,double cy,double r, double angle );
    void setpixelVu( QPainter& painter, int x, int y, double alpha);

    int IPart(float x);
    float FPart(float x);
    void DrawWuCircle(QPainter& painter, int _x, int _y, int radius, double angle);
    void InitScreenTemplate();

protected:
    void paintEvent(QPaintEvent *event);
    
signals:
    void myTimeout();
    
public slots:
    void start();
    void pause();
    void stop();
    void updateSimulation();
    
};

#endif // SIMULATION_H
