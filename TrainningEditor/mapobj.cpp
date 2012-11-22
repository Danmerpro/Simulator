#include "mapobj.h"

MapObj::MapObj( QList<QPointF> _points)
{
    points = _points;
    color = Qt::green;
}

void MapObj::setColor( QColor _color )
{
    color = _color;
}

QColor MapObj::getColor()
{
    return color;
}

