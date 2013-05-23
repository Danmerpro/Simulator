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
#include <QPair>
#include <QList>

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
    QPoint centerPoint;
    int radius;
    QList< QPair< QPointF, double> > drawnPoints;

public:
    explicit Simulation(QList<MapObj*> *_objects, QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *);

private:
    void draw30degLines(QPainter &);
    void draw10degLines(QPainter &);
    bool checkRoutePoint(double xcheck, double ycheck, double x, double y);
    void drawOldPoints(QPainter &);
    void drawPoints(QPainter &);
    
signals:
    void myTimeout();
    
public slots:
    void start();
    void pause();
    void stop();
    void updateSimulation();
    
};

#endif // SIMULATION_H
