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

void MapObj::setType( MAP_OBJ_TYPE _type )
{
    type = _type;
}

MAP_OBJ_TYPE MapObj::grtType()
{
    return type;
}

