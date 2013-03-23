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

void RoutePoint::setSpeed( double _speed )
{
    speed = _speed;
}

void RoutePoint::setAlt( double _alt )
{
    alt = _alt;
}

double RoutePoint::getSpeed()
{
    return speed;
}

double RoutePoint::getAlt()
{
    return alt;
}

void RoutePoint::setPointColor( QColor _color )
{
    pointColor = _color;
}

