#include "mapobj.h"


MapObj::MapObj( )
{
    routeColor = Qt::green;
    asseccory = ours;
    activePoint = NULL;
    startTime.setHMS(0,0,0);
}

MapObj::MapObj( QList<RoutePoint> _points)
{
    points = _points;
    routeColor = Qt::green;
    asseccory = ours;
    activePoint = NULL;
    startTime.setHMS(0,0,0);
}

void MapObj::setRouteColor( QColor _color )
{
    routeColor = _color;
}

QColor MapObj::getRouteColor()
{
    return routeColor;
}

void MapObj::setType( MAP_OBJ_TYPE _type )
{
    type = _type;
}

MAP_OBJ_TYPE MapObj::grtType()
{
    return type;
}

void MapObj::appendPoint( RoutePoint p )
{
    points.append( p );
}

void MapObj::setAsseccory( ACCESSORY_TYPE _asseccory )
{
    asseccory = _asseccory;
}

ACCESSORY_TYPE MapObj::getAsseccory()
{
    return asseccory;
}

void MapObj::setActivePoint( RoutePoint* p )
{
    activePoint = p;
}

RoutePoint* MapObj::getActivePoint()
{
    return activePoint;
}

void MapObj::setStartTime( QTime t)
{
    startTime = t;
}

QTime MapObj::getStartTime()
{
    return startTime;
}


