#include "pointslist.h"

PointsList::PointsList(QWidget *parent) :
    QWidget(parent)
{
    this->setMinimumWidth(300);
    this->adjustSize();
}

void PointsList::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen* pen = new QPen();
    pen->setWidth(4);
    painter.setPen(*pen);
    QBrush* brush = new QBrush(Qt::Dense5Pattern);
    brush->setColor(QColor(48,213,208));
    painter.setBrush(*brush);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawRoundedRect(1,1,this->width()-2,this->height()-2,20,10);
}
