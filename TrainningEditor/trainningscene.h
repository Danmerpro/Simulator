#ifndef TRAINNINGSCENE_H
#define TRAINNINGSCENE_H

#include <QWidget>
#include <QPainter>
#include <QBrush>
#include <QMouseEvent>
#include <Qpen>
#include <math.h>
#include "mapobj.h"


class TrainningScene : public QWidget
{
    Q_OBJECT
private:
    QList<MapObj*> objects;
    QList<QPointF>* curRoute;
    MapObj* curObj;
    QPointF curMousePos;
    QPen* pen;
    QBrush* brush;
    bool darwlingRouteMode;
    bool editingMode;

public:
    explicit TrainningScene(QWidget *parent = 0);
//    void drawBesierSpline( QPainter* painter,QList<QPointF> *points );
    
signals:
    void newRouteAdded();
    
private slots:
    void drawlingModeOn();
    void procesingNewRoute();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    
};

#endif // TRAINNINGSCENE_H