#ifndef TRAINNINGSCENE_H
#define TRAINNINGSCENE_H

#include <QWidget>
#include <QPainter>
#include <QBrush>
#include <QMouseEvent>
#include <Qpen>
#include <QMessageBox>
#include <math.h>
#include "mapobj.h"
#include "routepoint.h"
#include "options.h"


class TrainningScene : public QWidget
{
    Q_OBJECT
private:
    Options* opt;
    QList<MapObj*> *objects;
    MapObj* curObj;
    QPointF curMousePos;
    QPen* pen;
    QBrush* brush;
    RoutePoint* overCursor;
    RoutePoint* movingPoint;
    bool darwlingRouteMode;
    bool editingMode;
    bool mooving;

public:
    explicit TrainningScene(QList<MapObj*> *_objects, Options* _opt, QWidget *parent = 0);
//    void drawBesierSpline( QPainter* painter,QList<QPointF> *points );
    RoutePoint* isOnPoint(QPointF p);
    void setObjects( QList<MapObj*> *_objects );
    
signals:
    void newRouteAdded();
    void curRouteChanged( MapObj* _route );
    void routeEditing();
    void activePointChanged();
    
public slots:
    void drawlingModeOn();
    void procesingRoute();
    void finishEdit();
    void changeAvtivePoint();
    void changeCurRoute( MapObj* _route );
    void deleteRoute();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event );
    
};

#endif // TRAINNINGSCENE_H
