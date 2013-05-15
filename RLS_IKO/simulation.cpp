#include "simulation.h"

Simulation::Simulation(QList<MapObj *> *_objects, QWidget *parent) :
    QWidget(parent)
{
    QBrush bgrBrush(Qt::black);
    QPalette plt = this->palette();
    plt.setBrush(QPalette::Background, bgrBrush);
    setPalette(plt);

    this->setFixedSize(1000,1000);

    ScreenTemplate = new QImage(1000, 1000, QImage::Format_RGB32);
    ScreeenBuffer = new QImage( 1000, 1000, QImage::Format_RGB32);

    timerForMenu = new QTimer();
    timerForMenu->setInterval(1000);

    ptimer = new QTimer();
    ptimer->setInterval(40);

    objCount = _objects->size();
    simObjects = new SIM_OBJ[objCount];

    radarLine = new QLineF();
    radarAngle = -M_PI / 2;

    int i = 0;
    QList<MapObj *>::Iterator it = _objects->begin();
    for( it ; it != _objects->end() ; it++, i++ )
    {
        simObjects[i].obj = (*it);
        simObjects[i].lastInCurRoute = simObjects[i].obj->getPoints()->begin();
        simObjects[i].curPoint = (*(*it)->getPoints()->begin());
        simObjects[i].angle = atan2( ((*(simObjects[i].lastInCurRoute+1)).y()*1000 - (*simObjects[i].lastInCurRoute).y()*1000)
                , ((*(simObjects[i].lastInCurRoute+1)).x()*1000 - (*simObjects[i].lastInCurRoute).x()*1000 ) );
        simObjects[i].vX0 = (*simObjects[i].lastInCurRoute).getSpeed()*cos(simObjects[i].angle);
        simObjects[i].vY0 = (*simObjects[i].lastInCurRoute).getSpeed()*sin(simObjects[i].angle);

        simObjects[i].aX = (pow( (*(simObjects[i].lastInCurRoute+1)).getSpeed()*cos(simObjects[i].angle), 2 ) - pow((*simObjects[i].lastInCurRoute).getSpeed()*cos(simObjects[i].angle), 2 )) /
                ((*(simObjects[i].lastInCurRoute+1)).x()*1000 - (*simObjects[i].lastInCurRoute).x()*1000) /2;
        simObjects[i].aY = (pow( (*(simObjects[i].lastInCurRoute+1)).getSpeed()*sin(simObjects[i].angle), 2 ) - pow((*simObjects[i].lastInCurRoute).getSpeed()*sin(simObjects[i].angle), 2 )) /
                ((*(simObjects[i].lastInCurRoute+1)).y()*1000 - (*simObjects[i].lastInCurRoute).y()*1000) /2;
        simObjects[i].complete = false;
        simObjects[i].timeCounter = 0;
    }


    connect(ptimer, SIGNAL(timeout()), this, SLOT(updateSimulation()));
    connect(timerForMenu, SIGNAL(timeout()), this, SIGNAL(myTimeout()));

    pen = new QPen();

    InitScreenTemplate();
}

void Simulation::InitScreenTemplate()
{
    qreal rX0 = 500;
    qreal rY0 = 500;
    qreal rdLineLen = 400;
    QPainter painter(ScreenTemplate);
    painter.setRenderHint(QPainter::Antialiasing);
    QColor pColor(Qt::green);
    pen->setColor(pColor);
    painter.setPen(*pen);
    painter.drawEllipse( QPointF(rX0, rY0), rdLineLen, rdLineLen);
    painter.drawEllipse( QPointF(rX0, rY0), rdLineLen - 50, rdLineLen - 50);
    painter.drawEllipse( QPointF(rX0, rY0), rdLineLen - 100, rdLineLen - 100);
    painter.drawEllipse( QPointF(rX0, rY0), rdLineLen - 150, rdLineLen - 150);
    painter.drawEllipse( QPointF(rX0, rY0), rdLineLen - 200, rdLineLen - 200);
    painter.drawEllipse( QPointF(rX0, rY0), rdLineLen - 250, rdLineLen - 250);
    painter.drawEllipse( QPointF(rX0, rY0), rdLineLen - 300, rdLineLen - 300);
    painter.drawEllipse( QPointF(rX0, rY0), rdLineLen - 350, rdLineLen - 350);

}

void Simulation::setpixelVu( QPainter& painter, int x, int y, double alpha)
 {
    QColor pColor(Qt::green);
    pColor.setAlpha(255*alpha);
    pen->setColor(pColor);
    painter.setPen(*pen);
//	SetPixel(x, y,RGB(250*(1-alpha),250*(1-alpha),250));
    painter.drawPoint(x, y);
 }

void Simulation::DrawCirclPart(QPainter& painter,double cx,double cy,double r, double angle )
 {
    int x;
    int y;
    double t;
    double d;
    int j;
    int kx;
    int ky;
    int lastx;
    double curAngle;

    x = r;
    lastx = r;
    y = 0;
    t = 0;
    for( j = 0; j <= 3; j++ )
    {
        kx = j % 2 * 2 - 1;
        ky = j / 2 % 2  *2 - 1;

        curAngle = atan2(y, x) * 180 / M_PI;
        if( curAngle >= 0 && curAngle <= 10 )
        {
            setpixelVu(painter, kx * x + cx, ky * y + cy, double(1));
            setpixelVu(painter, kx * y + cx, ky * x + cy, double(1));
        }
        else
        {
            setpixelVu(painter, kx * x + cx, ky * y + cy, double(0));
            setpixelVu(painter, kx * y + cx, ky * x + cy, double(0));
        }
    }
    while( x > y )
    {
        y = y + 1;
        d = ceil(sqrt(double( r * r - y * y ))) - sqrt(double( r * r - y * y ));
        if( d < t )
        {
            x = x - 1;
        }
        if( x < y )
        {
            break;
        }
        if( x == y && lastx == x )
        {
            break;
        }
        for( j = 0; j <= 2; j++ )
        {
            kx = j % 2 * 2 - 1;
            ky = j / 2 % 2 * 2 - 1;
            curAngle = atan2(y, x) * 180 / M_PI;
        //    printf("angle = %f\n", curAngle);
            if( curAngle >= 0 && curAngle <= 10 )
            {
                setpixelVu(painter, kx * x + cx, ky * y + cy, 1 - d);
                setpixelVu(painter, kx * y + cx, ky * x + cy, 1 - d);
            }
            else
            {
                setpixelVu(painter, kx * x + cx, ky * y + cy, 0);
                setpixelVu(painter, kx * y + cx, ky * x + cy, 0);
            }
            if( x-1 >= y )
            {
                curAngle = atan2(y, x) * 180 / M_PI;
        //        printf("angle = %f\n", curAngle);
                if( curAngle >= 0 && curAngle <= 30 )
                {
                    setpixelVu(painter, kx * (x - 1) + cx, ky * y + cy, d);
                    setpixelVu(painter, kx * y + cx, ky * (x - 1) + cy, d);
                }
                else
                {
                    setpixelVu(painter, kx * (x - 1) + cx, ky * y + cy, 0);
                    setpixelVu(painter, kx * y + cx, ky * (x - 1) + cy, 0);
                }
            }
        }
        t = d;
        lastx = x;
    }
 }

//Целая часть числа
int Simulation::IPart(float x)
{
    return (int)x;
}

//дробная часть числа
float Simulation::FPart(float x)
{
    while (x >= 0)
        x--;
    x++;
    return x;
}

void Simulation::DrawWuCircle(QPainter& painter, int _x, int _y, int radius, double angle )
{
    double curAngle;
    //Установка пикселов, лежащих на осях системы координат с началом в центре
    QColor pColor(Qt::green);
    pColor.setAlpha(255);
    pen->setColor(pColor);
    painter.setPen(*pen);
    painter.drawPoint(_x + radius, _y);
    painter.drawPoint(_x, _y + radius);
    painter.drawPoint(_x - radius + 1, _y);
    painter.drawPoint(_x, _y - radius + 1);
 //   printf("%f\n", angle );

    float iy = 0;
    for (int x = 0; x <= radius * cos(M_PI / 4); x++)
    {
        //Вычисление точного значения координаты Y
        iy = (float)sqrt(radius * radius - x * x);

        //IV квадрант, Y
        curAngle = atan2(IPart(iy), -x);
        if( curAngle - M_PI/2 <= angle - M_PI && curAngle + M_PI/2  >= angle - M_PI/2)
        {
            pColor.setAlpha(255 - (int)(FPart(iy) * 255));
        }
        else
        {
            pColor.setAlpha(0);
        }
        pen->setColor(pColor);
        painter.setPen(*pen);
        painter.drawPoint(_x - x, _y + IPart(iy));
        if( curAngle <= angle - M_PI / 2 && curAngle >= angle - M_PI )
        {
            pColor.setAlpha((int)(FPart(iy) * 255));
        }
        else
        {
            pColor.setAlpha(0);
        }
        pen->setColor(pColor);
        painter.setPen(*pen);
        painter.drawPoint(_x - x, _y + IPart(iy) + 1);
        //I квадрант, Y
        curAngle = atan2(IPart(iy), x);
        if( curAngle <= angle - M_PI / 2 && curAngle >= angle - M_PI)
        {
            pColor.setAlpha(255 - (int)(FPart(iy) * 255));
        }
        else
        {
            pColor.setAlpha(0);
        }
        pen->setColor(pColor);
        painter.setPen(*pen);
        painter.drawPoint(_x + x, _y + IPart(iy));
        if( curAngle <= angle - M_PI / 2 && curAngle >= angle - M_PI)
        {
            pColor.setAlpha((int)(FPart(iy) * 255));
        }
        else
        {
            pColor.setAlpha(0);
        }
        pen->setColor(pColor);
        painter.setPen(*pen);
        painter.drawPoint(_x + x, _y + IPart(iy) + 1);
        //I квадрант, X
        curAngle = atan2(IPart(iy), x);
        if( curAngle - M_PI <= -angle + (M_PI / 2) && curAngle - M_PI >= -angle)
        {
            pColor.setAlpha(255 - (int)(FPart(iy) * 255));
        }
        else
        {
            pColor.setAlpha(0);
        }
        pen->setColor(pColor);
        painter.setPen(*pen);
        painter.drawPoint(_x + IPart(iy), _y + x);
        if( curAngle - M_PI <= -angle + (M_PI / 2) && curAngle - M_PI >= -angle)
        {
            pColor.setAlpha((int)(FPart(iy) * 255));
        }
        else
        {
            pColor.setAlpha(0);
        }
        pen->setColor(pColor);
        painter.setPen(*pen);
        painter.drawPoint(_x + IPart(iy) + 1, _y + x);
        //II квадрант, X
        curAngle = atan2(IPart(iy), -x);
        if( curAngle - M_PI <= -angle + (M_PI / 2) && curAngle - M_PI >= -angle)
        {
            pColor.setAlpha(255 - (int)(FPart(iy) * 255));
        }
        else
        {
            pColor.setAlpha(0);
        }
        pen->setColor(pColor);
        painter.setPen(*pen);
        painter.drawPoint(_x + IPart(iy), _y - x);
        if( curAngle - M_PI <= -angle + (M_PI / 2) && curAngle - M_PI >= -angle)
        {
            pColor.setAlpha((int)(FPart(iy) * 255));
        }
        else
        {
            pColor.setAlpha(0);
        }
        pen->setColor(pColor);
        painter.setPen(*pen);
        painter.drawPoint(_x + IPart(iy) + 1, _y - x);

        //С помощью инкремента устраняется ошибка смещения на 1 пиксел*/
        x++;
        //II квадрант, Y
        curAngle = atan2(-IPart(iy), x);
        if( curAngle <= angle - M_PI / 2 && curAngle >= angle - M_PI)
        {
            pColor.setAlpha((int)(FPart(iy) * 255));
        }
        else
        {
            pColor.setAlpha(0);
        }
        pen->setColor(pColor);
        painter.setPen(*pen);
        painter.drawPoint(_x + x, _y - IPart(iy));
        if( curAngle <= angle - M_PI / 2 && curAngle >= angle - M_PI)
        {
            pColor.setAlpha(255 - (int)(FPart(iy) * 255));
        }
        else
        {
            pColor.setAlpha(0);
        }
        pen->setColor(pColor);
        painter.setPen(*pen);
        painter.drawPoint(_x + x, _y - IPart(iy) + 1);
        //III квадрант, Y
        curAngle = atan2(-IPart(iy), -x);
        if( curAngle <= angle - M_PI / 2 && curAngle >= angle - M_PI)
        {
            pColor.setAlpha((int)(FPart(iy) * 255));
        }
        else
        {
            pColor.setAlpha(0);
        }
        pen->setColor(pColor);
        painter.setPen(*pen);
        painter.drawPoint(_x - x, _y - IPart(iy));
        if( curAngle <= angle - M_PI / 2 && curAngle >= angle - M_PI)
        {
            pColor.setAlpha(255 - (int)(FPart(iy) * 255));
        }
        else
        {
            pColor.setAlpha(0);
        }
        pen->setColor(pColor);
        painter.setPen(*pen);
        painter.drawPoint(_x - x, _y - IPart(iy) + 1);
        //III квадрант, X
        curAngle = atan2(-IPart(iy), -x);
        if( curAngle - M_PI <= -angle + (M_PI / 2) && curAngle - M_PI >= -angle)
        {
            pColor.setAlpha((int)(FPart(iy) * 255));
        }
        else
        {
            pColor.setAlpha(0);
        }
        pen->setColor(pColor);
        painter.setPen(*pen);
        painter.drawPoint(_x - IPart(iy), _y - x);
        if( curAngle - M_PI <= -angle + (M_PI / 2) && curAngle - M_PI >= -angle)
        {
            pColor.setAlpha(255 - (int)(FPart(iy) * 255));
        }
        else
        {
            pColor.setAlpha(0);
        }
        pen->setColor(pColor);
        painter.setPen(*pen);
        painter.drawPoint(_x - IPart(iy) + 1, _y - x);
        //IV квадрант, X
        curAngle = atan2(-IPart(iy), x);
        printf( "angle = %f\n", (-angle) * 180 / M_PI );
        printf( "curAngle2 = %f\n", curAngle * 180 / M_PI );
        if( curAngle - M_PI <= -angle + (M_PI / 2) && curAngle - M_PI >= -angle )
        {
            pColor.setAlpha((int)(FPart(iy) * 255));
        }
        else
        {
            pColor.setAlpha(0);
        }
        pen->setColor(pColor);
        painter.setPen(*pen);
        painter.drawPoint(_x - IPart(iy), _y + x);
        if( curAngle - M_PI <= -angle + (M_PI / 2) && curAngle - M_PI >= -angle  )
        {
            pColor.setAlpha(255 - (int)(FPart(iy) * 255));
        }
        else
        {
            pColor.setAlpha(0);
        }
        pen->setColor(pColor);
        painter.setPen(*pen);
        painter.drawPoint(_x - IPart(iy) + 1, _y + x);
        //Возврат значения
        x--;
    }
}
/*
void Simulation::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    double curAngle;
    qreal rX0 = 500;
    qreal rY0 = 500;
    qreal rdLineLen = 400;
    QRgb pix;
    FILE * log = fopen("log.txt", "wt");


    for( int i = 0 ; i < ScreenTemplate->height() ; i++ )
    {
        for( int j = 0 ; j < ScreenTemplate->width() ; j++ )
        {
            if( (pix = ScreenTemplate->pixel( j, i )) != qRgb( 255, 255, 255 ) )
            {
                curAngle = atan2( i - rY0, j - rX0 );
                if( curAngle <= radarAngle && curAngle >= radarAngle - M_PI )
                {
                    ScreeenBuffer->setPixel( j, i, qRgb( 0, qGreen(pix) * (1 - (radarAngle - curAngle) / M_PI) , 0) );
                }
                else
                {
                    ScreeenBuffer->setPixel( j, i, qRgb( 0, 0, 0 ) );
                }
            }
        }
    }

    painter.drawImage( 0, 0, *ScreeenBuffer );
    radarAngle += 0.0251;
    if( radarAngle >= 2 * M_PI )
        radarAngle = 0;
}*/


void Simulation::paintEvent(QPaintEvent *event)
{
    qreal rX0 = 500;
    qreal rY0 = 500;
    qreal rdLineLen = 400;
    QPainter painter(this);
    RoutePoint from;
    RoutePoint to;
    QFont* font = new QFont();
    brush = new QBrush(Qt::SolidPattern);
    pen->setWidth(2);
    pen->setColor(Qt::green);
    painter.setPen(*pen);
    painter.setRenderHint(QPainter::Antialiasing);

    radarLine->setP1(QPointF(rX0,rY0));
    radarLine->setP2(QPointF(rX0 + (rdLineLen * sin(radarAngle)),rY0+(-rdLineLen * cos(radarAngle))));

//    painter.drawLine(*radarLine);

 /*   DrawWuCircle(painter, 500.0, 500.0, 400.0, radarAngle );
    DrawWuCircle(painter, 500.0, 500.0, 350.0, radarAngle );
    DrawWuCircle(painter, 500.0, 500.0, 300.0, radarAngle );
    DrawWuCircle(painter, 500.0, 500.0, 250.0, radarAngle );
    DrawWuCircle(painter, 500.0, 500.0, 200.0, radarAngle );
    DrawWuCircle(painter, 500.0, 500.0, 150.0, radarAngle );
    DrawWuCircle(painter, 500.0, 500.0, 100.0, radarAngle );
    DrawWuCircle(painter, 500.0, 500.0, 50.0, radarAngle );*/

    QColor arcColor(Qt::green);
    arcColor.setAlpha(255);
    pen->setColor(arcColor);
    painter.setPen(*pen);

    double dAngle = 0;
    painter.drawArc( 100, 100, 800, 800, -(radarAngle * 180 / M_PI - 90 ) * 16 , 180 * 16 );
    painter.drawArc( 150, 150, 700, 700, -(radarAngle * 180 / M_PI - 90 ) * 16 , 180 * 16 );
    painter.drawArc( 200, 200, 600, 600, -(radarAngle * 180 / M_PI - 90 ) * 16 , 180 * 16 );
    painter.drawArc( 250, 250, 500, 500, -(radarAngle * 180 / M_PI - 90 ) * 16 , 180 * 16 );
    painter.drawArc( 300, 300, 400, 400, -(radarAngle * 180 / M_PI - 90 ) * 16 , 180 * 16 );
    painter.drawArc( 350, 350, 300, 300, -(radarAngle * 180 / M_PI - 90 ) * 16 , 180 * 16 );
    painter.drawArc( 400, 400, 200, 200, -(radarAngle * 180 / M_PI - 90 ) * 16 , 180 * 16 );
    painter.drawArc( 450, 450, 100, 100, -(radarAngle * 180 / M_PI - 90 ) * 16 , 180 * 16 );
    for( int i = 200 ; i > 0 ; i -= 5 )
    {
        arcColor.setAlpha(i);
        pen->setColor(arcColor);
        pen->setWidth(1);
        painter.setPen(*pen);
        painter.drawArc( 100, 100, 800, 800, -(radarAngle * 180 / M_PI + 90 ) * 16 , dAngle * 16 );
        painter.drawArc( 150, 150, 700, 700, -(radarAngle * 180 / M_PI + 90 ) * 16 , dAngle * 16 );
        painter.drawArc( 200, 200, 600, 600, -(radarAngle * 180 / M_PI + 90 ) * 16 , dAngle * 16 );
        painter.drawArc( 250, 250, 500, 500, -(radarAngle * 180 / M_PI + 90 ) * 16 , dAngle * 16 );
        painter.drawArc( 300, 300, 400, 400, -(radarAngle * 180 / M_PI + 90 ) * 16 , dAngle * 16 );
        painter.drawArc( 350, 350, 300, 300, -(radarAngle * 180 / M_PI + 90 ) * 16 , dAngle * 16 );
        painter.drawArc( 400, 400, 200, 200, -(radarAngle * 180 / M_PI + 90 ) * 16 , dAngle * 16 );
        painter.drawArc( 450, 450, 100, 100, -(radarAngle * 180 / M_PI + 90 ) * 16 , dAngle * 16 );
        dAngle += 2;
    }

    QColor lineColor(Qt::green);
    pen->setColor(lineColor);
    pen->setWidth(2);
    painter.setPen(*pen);

    if( radarAngle < M_PI && radarAngle >= 0)
    {
        lineColor.setAlpha(255 * ((M_PI - radarAngle)/M_PI));
        pen->setColor(lineColor);
        pen->setWidth(2);
        painter.setPen(*pen);
        painter.drawLine(rX0, rY0, rX0 + (rdLineLen * sin(0)), rY0 + (-rdLineLen * cos(0)));
    }
    if( radarAngle < 5 * M_PI / 4 && radarAngle >= (M_PI / 4))
    {
        lineColor.setAlpha(255 * ((5 * M_PI / 4 - radarAngle)/M_PI));
        pen->setColor(lineColor);
        pen->setWidth(2);
        painter.setPen(*pen);
        painter.drawLine(rX0, rY0, rX0 + (rdLineLen * sin(M_PI / 4)), rY0 + (-rdLineLen * cos(M_PI / 4)));
    }
    if( radarAngle < 3 * M_PI / 2 && radarAngle >= (M_PI / 2))
    {
        lineColor.setAlpha(255 * ((3 * M_PI / 2 - radarAngle)/M_PI));
        pen->setColor(lineColor);
        pen->setWidth(2);
        painter.setPen(*pen);
        painter.drawLine(rX0, rY0, rX0 + (rdLineLen * sin(M_PI / 2)), rY0 + (-rdLineLen * cos(M_PI / 2)));
    }
    if( radarAngle < 7 * M_PI / 4 && radarAngle >= (3 * M_PI / 4))
    {
        lineColor.setAlpha(255 * ((7 * M_PI / 4 - radarAngle)/M_PI));
        pen->setColor(lineColor);
        pen->setWidth(2);
        painter.setPen(*pen);
        painter.drawLine(rX0, rY0, rX0 + (rdLineLen * sin(3 * M_PI / 4)), rY0 + (-rdLineLen * cos(3 * M_PI / 4)));
    }
    if( radarAngle < 2 * M_PI  && radarAngle >= M_PI)
    {
        lineColor.setAlpha(255 * ((2 * M_PI - radarAngle)/M_PI));
        pen->setColor(lineColor);
        pen->setWidth(2);
        painter.setPen(*pen);
        painter.drawLine(rX0, rY0, rX0 + (rdLineLen * sin(M_PI)), rY0 + (-rdLineLen * cos(M_PI)));
    }
    if( (radarAngle < 2 * M_PI && radarAngle >= (5 * M_PI / 4)))
    {
        lineColor.setAlpha(255 * (((9 * M_PI / 4) - radarAngle)/M_PI));
        pen->setColor(lineColor);
        pen->setWidth(2);
        painter.setPen(*pen);
        painter.drawLine(rX0, rY0, rX0 + (rdLineLen * sin(5 * M_PI / 4)), rY0 + (-rdLineLen * cos(5 * M_PI / 4)));
    }
    if( radarAngle < M_PI / 4 && radarAngle >= 0)
    {
        lineColor.setAlpha(255 * ( (M_PI / 4 - radarAngle)/M_PI));
        pen->setColor(lineColor);
        pen->setWidth(2);
        painter.setPen(*pen);
        painter.drawLine(rX0, rY0, rX0 + (rdLineLen * sin(5 * M_PI / 4)), rY0 + (-rdLineLen * cos(5 * M_PI / 4)));
    }
    if( radarAngle < 2 * M_PI && radarAngle >= (3 * M_PI / 2))
    {
        lineColor.setAlpha(255 * (((10 * M_PI / 4) - radarAngle )/M_PI));
        pen->setColor(lineColor);
        pen->setWidth(2);
        painter.setPen(*pen);
        painter.drawLine(rX0, rY0, rX0 + (rdLineLen * sin(3 * M_PI / 2)), rY0 + (-rdLineLen * cos(3 * M_PI / 2)));
    }
    if( radarAngle < M_PI / 2 && radarAngle >= 0)
    {
        lineColor.setAlpha(255 * ( (M_PI / 2 - radarAngle)/M_PI));
        pen->setColor(lineColor);
        pen->setWidth(2);
        painter.setPen(*pen);
        painter.drawLine(rX0, rY0, rX0 + (rdLineLen * sin(3 * M_PI / 2)), rY0 + (-rdLineLen * cos(3 * M_PI / 2)));
    }
    if( radarAngle < (2 * M_PI) && radarAngle >= (7 * M_PI / 4))
    {
        lineColor.setAlpha(255 * (((13 * M_PI / 4) - radarAngle )/M_PI));
        pen->setColor(lineColor);
        pen->setWidth(2);
        painter.setPen(*pen);
        painter.drawLine(rX0, rY0, rX0 + (rdLineLen * sin(7 * M_PI / 4)), rY0 + (-rdLineLen * cos(7 * M_PI / 4)));
    }
    if( radarAngle < 3 * M_PI / 4 && radarAngle >= 0)
    {
        lineColor.setAlpha(255 * ( (3 * M_PI / 4 - radarAngle)/M_PI));
        pen->setColor(lineColor);
        pen->setWidth(2);
        painter.setPen(*pen);
        painter.drawLine(rX0, rY0, rX0 + (rdLineLen * sin(7 * M_PI / 4)), rY0 + (-rdLineLen * cos(7 * M_PI / 4)));
    }

    int pointsAmount = 1000;

    pen->setWidth(1);
    pen->setColor(Qt::green);
    painter.setPen(*pen);

    for( int i = 0 ; i < pointsAmount ; i++ )
    {
        double pointAngle = radarAngle + log((double)rand() / RAND_MAX) / 1.5;
        int pointD = rand() % 400;
        painter.drawPoint(rX0 + (pointD * sin(pointAngle)), rY0 + (-pointD * cos(pointAngle)));
    }

    pointsAmount = 200;

    for( int i = 0 ; i < pointsAmount ; i++ )
    {
        int pointD = rand() % 400;
        painter.drawPoint(rX0 + (pointD * sin(radarAngle)), rY0 + (-pointD * cos(radarAngle)));
        painter.drawPoint(rX0 + (pointD * sin(radarAngle - 0.005)), rY0 + (-pointD * cos(radarAngle - 0.005)));
        painter.drawPoint(rX0 + (pointD * sin(radarAngle - 0.01)), rY0 + (-pointD * cos(radarAngle - 0.01)));
    }

    radarAngle += 0.0251;
    if( radarAngle >= 2 * M_PI )
        radarAngle = 0;

    pen->setWidth(1);
    painter.setPen(*pen);
    pen->setWidth(2);
    pen->setColor(Qt::green);
    painter.setPen(*pen);
    font->setBold(false);
    painter.setFont(*font);
    for( int i = 0 ; i < objCount ; i++ )
    {
        pen->setWidth(2);
        if( simObjects[i].obj->getAsseccory() == ours )
        {
            pen->setColor(Qt::blue);
            brush->setColor(Qt::blue);
        }
        if( simObjects[i].obj->getAsseccory() == alien )
        {
            pen->setColor(Qt::red);
            brush->setColor(Qt::red);
        }
        painter.setPen(*pen);
        painter.setBrush(*brush);
        QList<RoutePoint>::iterator itC = simObjects[i].obj->getPoints()->begin();
        for( itC ; itC != simObjects[i].obj->getPoints()->end() ; itC++ )
        {
            if( itC != simObjects[i].lastInCurRoute )
            {
                if( itC+1 != simObjects[i].obj->getPoints()->end())
                {
                    from = (*itC);
                    to = (*(itC+1));
                }
                painter.drawLine(from,to);
            }
            else
            {
                break;
            }
        }
        from = (*itC);
        to = simObjects[i].curPoint;
        painter.drawLine(from,to);
        pen->setWidth(2);
        pen->setColor(Qt::black);
        painter.setPen(*pen);
        QPointF p1, p2, p3;

        p1.setX(to.x()+(-4*cos(simObjects[i].angle+M_PI/2)-3*sin(simObjects[i].angle+M_PI/2)));
        p1.setY(to.y()+(-4*sin(simObjects[i].angle+M_PI/2)+3*cos(simObjects[i].angle+M_PI/2)));
        p2.setX(to.x()+(4*cos(simObjects[i].angle+M_PI/2)-3*sin(simObjects[i].angle+M_PI/2)));
        p2.setY(to.y()+(4*sin(simObjects[i].angle+M_PI/2)+3*cos(simObjects[i].angle+M_PI/2)));
        p3.setX(to.x()+(6*sin(simObjects[i].angle+M_PI/2)));
        p3.setY(to.y()+(-6*cos(simObjects[i].angle+M_PI/2)));

        QPolygonF plain;
        plain.append(p1);
        plain.append(p2);
        plain.append(p3);
        painter.drawPolygon(plain);

        pen->setWidth(1);
        painter.setPen(*pen);

        double curSpeed, curSpeedX, curSpeedY;

        curSpeedX = sqrt( pow(simObjects[i].vX0, 2 ) + 2000*simObjects[i].aX*(simObjects[i].curPoint.x() - (*(simObjects[i].lastInCurRoute)).x()));
        curSpeedY = sqrt( pow(simObjects[i].vY0, 2 ) + 2000*simObjects[i].aY*(simObjects[i].curPoint.y() - (*(simObjects[i].lastInCurRoute)).y()));
        curSpeed = sqrt( pow(curSpeedX, 2) + pow(curSpeedY, 2) );

        QPointF pLable(to.x()-14,to.y()+14);
        qreal lineLen;

        pLable.setX(to.x()+20);
        pLable.setY(to.y()-12);
        painter.drawText(pLable,QString().setNum(to.getAlt())+tr(" м"));

        pLable.setX(to.x()+20);
        pLable.setY(to.y()+4);
        painter.drawText(pLable,QString().setNum(curSpeed*3600/1000) + tr(" км/ч"));

        lineLen = (QString().setNum(to.getAlt())+tr(" м")).length() > (QString().setNum(curSpeed*3600/1000) + tr(" км/ч")).length() ?
                    (QString().setNum(to.getAlt())+tr(" м")).length() : (QString().setNum(curSpeed*3600/1000) + tr(" км/ч")).length();
        painter.drawLine(to.x()+20,to.y()-8,to.x()+20+lineLen*5,to.y()-8);
        painter.drawLine(to.x(),to.y(),to.x()+20,to.y()-8);
    }

}

void Simulation::start()
{
    timeElapsed = new QTime();
    timerForMenu->start();
    ptimer->start();
}

void Simulation::pause()
{
    timerForMenu->stop();
    ptimer->stop();
}

void Simulation::stop()
{
    timerForMenu->stop();
    ptimer->stop();
    for( int i = 0 ; i < objCount ; i++ )
    {
        simObjects[i].lastInCurRoute = simObjects[i].obj->getPoints()->begin();
        simObjects[i].curPoint = *(simObjects[i].obj->getPoints()->begin());
        simObjects[i].angle = atan2( ((*(simObjects[i].lastInCurRoute+1)).y()*1000 - (*simObjects[i].lastInCurRoute).y()*1000)
                , ((*(simObjects[i].lastInCurRoute+1)).x()*1000 - (*simObjects[i].lastInCurRoute).x()*1000 ) );
        simObjects[i].vX0 = (*simObjects[i].lastInCurRoute).getSpeed()*cos(simObjects[i].angle);
        simObjects[i].vY0 = (*simObjects[i].lastInCurRoute).getSpeed()*sin(simObjects[i].angle);

        simObjects[i].aX = (pow( (*(simObjects[i].lastInCurRoute+1)).getSpeed()*cos(simObjects[i].angle), 2 ) - pow((*simObjects[i].lastInCurRoute).getSpeed()*cos(simObjects[i].angle), 2 )) /
                ((*(simObjects[i].lastInCurRoute+1)).x()*1000 - (*simObjects[i].lastInCurRoute).x()*1000) /2;
        simObjects[i].aY = (pow( (*(simObjects[i].lastInCurRoute+1)).getSpeed()*sin(simObjects[i].angle), 2 ) - pow((*simObjects[i].lastInCurRoute).getSpeed()*sin(simObjects[i].angle), 2 )) /
                ((*(simObjects[i].lastInCurRoute+1)).y()*1000 - (*simObjects[i].lastInCurRoute).y()*1000) /2;
        simObjects[i].complete = false;
        simObjects[i].timeCounter = 0;
    }
    update();
}

void Simulation::updateSimulation()
{
    bool leftBoundX, rigthBoundX;
    bool leftBoundY, rigthBoundY;
    leftBoundX = rigthBoundX = leftBoundY = rigthBoundY = false;
    bool trainEnd = true;
    for( int i = 0 ; i < objCount ; i++ )
    {
        if( simObjects[i].complete == false )
        {
            trainEnd = false;
            simObjects[i].curPoint.setX( ((*simObjects[i].lastInCurRoute).x()*1000 + simObjects[i].vX0 * simObjects[i].timeCounter*0.04 +
                                         simObjects[i].aX * pow( simObjects[i].timeCounter*0.04, 2 ) / 2)/1000 );
            simObjects[i].curPoint.setY( ((*simObjects[i].lastInCurRoute).y()*1000 + simObjects[i].vY0 * simObjects[i].timeCounter*0.04 +
                                         simObjects[i].aY * pow( simObjects[i].timeCounter*0.04, 2 ) / 2)/1000 );


            if( simObjects[i].vX0 < 0 && simObjects[i].curPoint.x() < (*(simObjects[i].lastInCurRoute+1)).x() )
            {
                leftBoundX = true;
            }

            if( simObjects[i].vX0 > 0 && simObjects[i].curPoint.x() > (*(simObjects[i].lastInCurRoute+1)).x() )
            {
                rigthBoundX = true;
            }

            if( simObjects[i].vY0 < 0 && simObjects[i].curPoint.y() < (*(simObjects[i].lastInCurRoute+1)).y() )
            {
                leftBoundY = true;
            }

            if( simObjects[i].vX0 > 0 && simObjects[i].curPoint.y() > (*(simObjects[i].lastInCurRoute+1)).y() )
            {
                rigthBoundY = true;
            }

            if( (leftBoundX || rigthBoundX || simObjects[i].vX0 == 0) && (leftBoundX || rigthBoundX || simObjects[i].vY0 == 0) )
            {
                simObjects[i].lastInCurRoute++;
                if( (simObjects[i].lastInCurRoute+1) != simObjects[i].obj->getPoints()->end() )
                {
                    simObjects[i].curPoint = (*simObjects[i].lastInCurRoute);
                    simObjects[i].angle = atan2( ((*(simObjects[i].lastInCurRoute+1)).y()*1000 - (*simObjects[i].lastInCurRoute).y()*1000)
                            , ((*(simObjects[i].lastInCurRoute+1)).x()*1000 - (*simObjects[i].lastInCurRoute).x()*1000 ) );
                    simObjects[i].vX0 = (*simObjects[i].lastInCurRoute).getSpeed()*cos(simObjects[i].angle);
                    simObjects[i].vY0 = (*simObjects[i].lastInCurRoute).getSpeed()*sin(simObjects[i].angle);

                    simObjects[i].aX = (pow( (*(simObjects[i].lastInCurRoute+1)).getSpeed()*cos(simObjects[i].angle), 2 ) - pow((*simObjects[i].lastInCurRoute).getSpeed()*cos(simObjects[i].angle), 2 )) /
                            ((*(simObjects[i].lastInCurRoute+1)).x()*1000 - (*simObjects[i].lastInCurRoute).x()*1000) /2;
                    simObjects[i].aY = (pow( (*(simObjects[i].lastInCurRoute+1)).getSpeed()*sin(simObjects[i].angle), 2 ) - pow((*simObjects[i].lastInCurRoute).getSpeed()*sin(simObjects[i].angle), 2 )) /
                            ((*(simObjects[i].lastInCurRoute+1)).y()*1000 - (*simObjects[i].lastInCurRoute).y()*1000) /2;
                    simObjects[i].timeCounter = 0;
                }
                else
                {
                    simObjects[i].complete = true;
                }
            }
            else
            {
                simObjects[i].timeCounter++;
            }
        }
    }
    if( trainEnd == false )
    {
        update();
    }
    else
    {
        stop();
    }
}
