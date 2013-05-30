#ifndef POINTSLIST_H
#define POINTSLIST_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QBrush>

class PointsList : public QWidget
{
    Q_OBJECT
public:
    explicit PointsList(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    
signals:
    
public slots:
    
};

#endif // POINTSLIST_H
