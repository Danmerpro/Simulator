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
#include <QMessageBox>
#include <QMouseEvent>

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
    QTime startTime;
    bool started;
    bool complete;
    double angle;
}SIM_OBJ;

typedef struct
{
    int index;
    double angle;
    QPointF point;
    bool enemy;
} BufferEntry;

class Simulation : public QWidget
{
    Q_OBJECT

private:
    SIM_OBJ* simObjects;
    int objCount;
    QTimer *ptimer;
    QTimer *timerForMenu;
    QTime *timeElapsed;
    QTime *timeBetweenUpdates;
    double radarAngle;
    QPoint centerPoint;
    int radius;
    double accuracy;//точность обнаружения точки
    int hordHalf;//половина длины хорды отметки о цели
    int arcWidth; //толщина дуг отметки и опознования
    double accuracyClick; //точность при кликах мышкой
    QList< BufferEntry > drawnPoints;

public:
    explicit Simulation(QList<MapObj*> *_objects, QWidget *parent = 0);
    void setObjects(QList<MapObj*> *_objects);

protected:
    void paintEvent(QPaintEvent *event);
    void mouseReleaseEvent(QMouseEvent *);

private:
    void drawLenArcks(QPainter &);
    void draw30degLines(QPainter &);
    void draw10degLines(QPainter &);
    bool checkRoutePoint(double xcheck, double ycheck, double x, double y);
    void drawOldPoints(QPainter &);
    void drawPoints(QPainter &);
    void drawRandomPoints(QPainter &);
    void drawPoint(QPainter &, double, double, double angle, bool enemy = false);
    void drawArcId(QPainter &, double, double, double angle);
    bool checkClick(double x1, double y1, double x2, double y2);
    int findClosest(QList<BufferEntry> &, double, double);

signals:
    void myTimeout();

public slots:
    void start();
    void pause();
    void stop();
    void updateSimulation();

};

#endif // SIMULATION_H
