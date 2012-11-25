#ifndef ROUTEPOINT_H
#define ROUTEPOINT_H

#include <QPointF>
#include <QColor>

class RoutePoint : public QPointF
{
private:
    int speed;
    int alt;
    QColor pointColor;
public:
    RoutePoint();
    RoutePoint( QPointF _p);
    void setSpeed( int _speed );
    void setAlt( int _alt );
    int getSpeed();
    int getAlt();
    void setPointColor( QColor _color );
    
};

#endif // ROUTEPOINT_H
