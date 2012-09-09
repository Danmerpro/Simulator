#include "indicator.h"
#include <QPainter>
#include <QImage>
#include <math.h>
Indicator::Indicator(QWidget *parent) :
    QWidget(parent)
{
    angle = 0;
    connect(&timer, SIGNAL(timeout()), this, SLOT(scan()));
    timer.start();
}

void Indicator::paintEvent(QPaintEvent *e)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    //вычислим размеры виджета, найдем центр, вычислим размеры круга
    int x = this->width()/2;
    int y = this->height()/2;
    int rad = ((width() < height()) ? width() : height()) / 2.2;

    QPixmap  *map= new QPixmap(width(), height());

    //рисуем круг
    drawCircle(*map, QPoint(x, y), rad);
    drawLine(*map, QPoint(x, y), rad);
    painter.drawPixmap(0, 0, *map);
    painter.end();
    delete map;
}

void Indicator::drawLine(QPixmap &_m, QPoint _center, int _rad)
{
    QPainter painter;
    painter.begin(&_m);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::black);

    //вычисляем точку на окружности для проведения линии
    //тригонометрия рулит
    double b = _rad * sin(angle);
    double a = _rad * cos(angle);
    painter.drawLine(_center, QPoint(_center.x() + b, _center.y() + a));
    painter.end();
}

void Indicator::drawCircle(QPixmap &_m, QPoint _center, int _rad)
{
    QPainter painter;
    painter.begin(&_m);
    painter.setRenderHint(QPainter::Antialiasing);
   // QPen p(Qt::green);
    painter.setPen(Qt::green);
    painter.fillRect(0,0,_m.width(), _m.height(), Qt::white);
    painter.drawEllipse(_center, _rad, _rad);


    int tmp = _rad*_rad;
    for(int i = (-1)*tmp; i <= tmp; i++)
    {
        double y =(double) tmp* (double) (1 - (double) (i*i)/(tmp));
        double y1 = sqrt((int) y);
        painter.drawLine(_center.x() + i, _center.y() - y1,
                     _center.x() + i, _center.y() + y1);
    }
    painter.end();
}

void Indicator::scan()
{
    angle-=0.05;
    if(angle <= M_PI * (-2))
        angle = 0;
    update();
}
