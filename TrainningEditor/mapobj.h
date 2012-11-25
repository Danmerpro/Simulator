#ifndef MAPOBJ_H
#define MAPOBJ_H

#include <QList>
#include <QColor>
#include "routepoint.h"

enum MAP_OBJ_TYPE { route };
enum ACCESSORY_TYPE { ours, alien };

class MapObj
{

private:
    QList<RoutePoint> points;
    MAP_OBJ_TYPE type;
    QColor routeColor;
    ACCESSORY_TYPE asseccory;
public:
    MapObj();
    MapObj( QList<RoutePoint> _points );
    void appendPoint( RoutePoint p );
    QList<RoutePoint>* getPoints(){ return &points; };
    void setRouteColor( QColor _color );
    QColor getRouteColor();
    void setType( MAP_OBJ_TYPE _type );
    MAP_OBJ_TYPE grtType();
    void setAsseccory( ACCESSORY_TYPE _asseccory );
    ACCESSORY_TYPE getAsseccory();
};

#endif // CRUVE_H
