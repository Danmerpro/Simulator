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
    int timeCounter;
    bool complete;
    double angle;
}SIM_OBJ;

typedef struct
{
    int index;
    double angle;
    QPointF point;
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
    QBrush* brush;
    QLineF* radarLine;
    double radarAngle;
    QImage* ScreenTemplate;
    QImage* ScreeenBuffer;
    QPoint centerPoint;
    int radius;
    double accuracy;//точность обнаружения точки
    int hordHalf;//половина длины хорды отметки о цели
    int arcWidth; //толщина дуг отметки и опознования
    double accuracyClick; //точность при кликах мышкой
    QList< BufferEntry > drawnPoints;

public:
    explicit Simulation(QList<MapObj*> *_objects, QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *);
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
