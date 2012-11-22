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
    QList<MapObj> objects;
    bool drawling;
    QList<QPointF>* curCruve;
    QPen* pen;
    QBrush* brush;
    MapObj* curObj;
    bool drawlingMode;
    bool editingMode;

public:
    explicit TrainningScene(QWidget *parent = 0);
    MapObj* ifOnCruve(QPointF p);
    void drawBesierSpline( QPainter* painter,QList<QPointF> *points );      
    
signals:
    
public slots:

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    
};

#endif // TRAINNINGSCENE_H
