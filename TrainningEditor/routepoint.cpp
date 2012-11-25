#include "routepoint.h"

RoutePoint::RoutePoint() :
    QPointF()
{
    speed = 0;
    alt = 0;
}

RoutePoint::RoutePoint( QPointF _p) :
    QPointF(_p)
{
    speed = 0;
    alt = 0;
}

void RoutePoint::setSpeed( int _speed )
{
    speed = _speed;
}

void RoutePoint::setAlt( int _alt )
{
    alt = _alt;
}

int RoutePoint::getSpeed()
{
    return speed;
}

int RoutePoint::getAlt()
{
    return alt;
}

void RoutePoint::setPointColor( QColor _color )
{
    pointColor = _color;
}

