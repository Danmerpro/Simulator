#ifndef ROUTEPOINT_H
#define ROUTEPOINT_H

#include <QPointF>
#include <QColor>

class RoutePoint : public QPointF
{
private:
    double speed;
    double alt;
    QColor pointColor;
public:
    RoutePoint();
    RoutePoint( QPointF _p);
    void setSpeed( double _speed );
    void setAlt( double _alt );
    double getSpeed();
    double getAlt();
    void setPointColor( QColor _color );
    
};

#endif // ROUTEPOINT_H
