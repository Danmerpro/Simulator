#ifndef MAPOBJ_H
#define MAPOBJ_H

#include <QList>
#include <QPointF>
#include <QColor>

enum MAP_OBJ_TYPE { cruve , point };

class MapObj
{

private:
    QList<QPointF> points;
    MAP_OBJ_TYPE type;
    QColor color;
public:
    MapObj( QList<QPointF> _points );
    QList<QPointF>* getPoints(){ return &points; };
    void setColor( QColor _color );
    QColor getColor();
    void setType( MAP_OBJ_TYPE _type );
    MAP_OBJ_TYPE grtType();
};

#endif // CRUVE_H
