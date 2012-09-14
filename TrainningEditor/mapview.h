#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QGraphicsRectItem>
#include <math.h>

class MapView : public QGraphicsView
{
public:
    MapView();
protected:
  //  void wheelEvent(QWheelEvent *event);
};

#endif // MAPVIEW_H
