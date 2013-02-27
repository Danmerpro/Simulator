#ifndef MAPITEM_H
#define MAPITEM_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <math.h>

class MapItem : public QGraphicsPixmapItem
{    
signals:
    void mapScaled(double& factor);
public:
    MapItem();
    MapItem(const QPixmap &pixmap): QGraphicsPixmapItem(pixmap) {};
};

#endif // MAPITEM_H
