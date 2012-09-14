#ifndef RADARLINE_H
#define RADARLINE_H

#include <QObject>
#include <QGraphicsLineItem>
#include <QTimer>
#include <QColor>
#include <QPainter>
#include <math.h>

class RadarLine : public QObject, public QGraphicsLineItem
{
    Q_OBJECT

    QTimer timer;
    int radius;
    double angle;
    static const int framesPerSec = 25;
    static const int scanInSecs = 10;
public:
    RadarLine();
protected:
    void timerEvent(QTimerEvent *);
private slots:
    void scan();
};

#endif // RADARLINE_H
