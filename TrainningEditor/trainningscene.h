#ifndef TRAINNINGSCENE_H
#define TRAINNINGSCENE_H

#include <QWidget>
#include <QPainter>
#include <QBrush>
#include <QMouseEvent>
#include <Qpen>
#include <math.h>
#include "mapobj.h"
#include "routepoint.h"


class TrainningScene : public QWidget
{
    Q_OBJECT
private:
    QList<MapObj*> *objects;
    MapObj* curObj;
    QPointF curMousePos;
    QPen* pen;
    QBrush* brush;
    bool darwlingRouteMode;
    bool editingMode;

public:
    explicit TrainningScene(QList<MapObj*> *_objects, QWidget *parent = 0);
//    void drawBesierSpline( QPainter* painter,QList<QPointF> *points );
    
signals:
    void newRouteAdded();
    void routeEditing( MapObj* _route);
    
private slots:
    void drawlingModeOn();
    void procesingNewRoute();
    void finishEdit();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    
};

#endif // TRAINNINGSCENE_H
