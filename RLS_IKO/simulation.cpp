#include "simulation.h"

#include <QThread> //for test

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

    radarAngle = 3*M_PI / 2;

    int i = 0;
    QList<MapObj *>::Iterator it = _objects->begin();
    for( ; it != _objects->end() ; it++, i++ )
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

    centerPoint.setX(500);
    centerPoint.setY(500);
    radius = 400;
    accuracy = 4;
    hordHalf = 10;
    arcWidth = 3;
    accuracyClick = 10;
}

void Simulation::paintEvent(QPaintEvent *)
{
   // qDebug(QString::number(radarAngle/M_PI).toStdString().c_str());
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawLenArcks(painter);

    draw30degLines(painter);
    draw10degLines(painter);

    drawOldPoints(painter);
    drawPoints(painter);

    drawRandomPoints(painter);


   // radarAngle += 0.0251;
    radarAngle += 0.011;
    if(radarAngle >= 2*M_PI)
        radarAngle -= 2*M_PI;
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

void Simulation::draw30degLines(QPainter & painter )
{
    //painter.drawLine(centerPoint, QPointF(centerPoint.x()+radius*sin(-radarAngle + M_PI), centerPoint.y()+radius*cos(-radarAngle + M_PI)));
    QColor lineColor(Qt::green);
    lineColor.setAlpha(255);
    QPen pen;
    pen.setWidth(3);


    if(radarAngle >=1.5*M_PI && radarAngle <= 2*M_PI)
            lineColor.setAlpha(0);
    else
        lineColor.setAlpha(255);
    pen.setColor(lineColor);
    painter.setPen(pen);
    painter.drawLine(centerPoint, QPointF(centerPoint.x(), centerPoint.y() -radius));

    if(radarAngle >=0.5 * M_PI && radarAngle <= M_PI)
            lineColor.setAlpha(0);
    else
        lineColor.setAlpha(255);
    pen.setColor(lineColor);
    painter.setPen(pen);
    painter.drawLine(centerPoint, QPointF(centerPoint.x(), centerPoint.y() + radius));

    if(radarAngle >=0 && radarAngle <= 0.5 * M_PI)
            lineColor.setAlpha(0);
    else
        lineColor.setAlpha(255);
    pen.setColor(lineColor);
    painter.setPen(pen);
    painter.drawLine(centerPoint, QPointF(centerPoint.x() + radius, centerPoint.y()));

    if(radarAngle >= M_PI && radarAngle <= 1.5 * M_PI)
            lineColor.setAlpha(0);
    else
        lineColor.setAlpha(255);
    pen.setColor(lineColor);
    painter.setPen(pen);
    painter.drawLine(centerPoint, QPointF(centerPoint.x() - radius, centerPoint.y()));

    if(radarAngle >= 7*M_PI/6 && radarAngle <=  10*M_PI/6)
                lineColor.setAlpha(0);
    else
        lineColor.setAlpha(255);
    pen.setColor(lineColor);
    painter.setPen(pen);
    painter.drawLine(centerPoint, QPointF( centerPoint.x() - radius*cos(M_PI/6), centerPoint.y()- radius*sin(M_PI/6)));


    if(radarAngle >= 8*M_PI/6 && radarAngle <=  11*M_PI/6)
                lineColor.setAlpha(0);
    else
        lineColor.setAlpha(255);
    pen.setColor(lineColor);
    painter.setPen(pen);
    painter.drawLine(centerPoint, QPointF( centerPoint.x() - radius*cos(M_PI/3), centerPoint.y()- radius*sin(M_PI/3)));

    if((radarAngle >= 5*M_PI/3 && radarAngle <=  2*M_PI) || (radarAngle >= 0 && radarAngle <=  M_PI/6))
        lineColor.setAlpha(0);
    else
        lineColor.setAlpha(255);
    pen.setColor(lineColor);
    painter.setPen(pen);
    painter.drawLine(centerPoint, QPointF( centerPoint.x() + radius*cos(M_PI/3), centerPoint.y()- radius*sin(M_PI/3)));

    if((radarAngle >= 11*M_PI/6 && radarAngle <= 2*M_PI) || (radarAngle >= 0 && radarAngle <= M_PI/3))
        lineColor.setAlpha(0);
    else
        lineColor.setAlpha(255);
    pen.setColor(lineColor);
    painter.setPen(pen);
    painter.drawLine(centerPoint, QPointF( centerPoint.x() + radius*cos(M_PI/6), centerPoint.y()- radius*sin(M_PI/6)));

    if(radarAngle >= M_PI/6 && radarAngle <= 2*M_PI/3)
        lineColor.setAlpha(0);
    else
        lineColor.setAlpha(255);
    pen.setColor(lineColor);
    painter.setPen(pen);
    painter.drawLine(centerPoint, QPointF( centerPoint.x() + radius*cos(M_PI/6), centerPoint.y() + radius*sin(M_PI/6)));

    if(radarAngle >= M_PI/3 && radarAngle <= 5*M_PI/6)
        lineColor.setAlpha(0);
    else
        lineColor.setAlpha(255);
    pen.setColor(lineColor);
    painter.setPen(pen);
    painter.drawLine(centerPoint, QPointF( centerPoint.x() + radius*cos(M_PI/3), centerPoint.y() + radius*sin(M_PI/3)));

    if(radarAngle >= 2*M_PI/3 && radarAngle <= 7*M_PI/6)
        lineColor.setAlpha(0);
    else
        lineColor.setAlpha(255);
    pen.setColor(lineColor);
    painter.setPen(pen);
    painter.drawLine(centerPoint, QPointF( centerPoint.x() - radius*cos(M_PI/3), centerPoint.y() + radius*sin(M_PI/3)));


    if(radarAngle >= 5*M_PI/6 && radarAngle <= 4*M_PI/3)
        lineColor.setAlpha(0);
    else
        lineColor.setAlpha(255);
    pen.setColor(lineColor);
    painter.setPen(pen);
    painter.drawLine(centerPoint, QPointF( centerPoint.x() - radius*cos(M_PI/6), centerPoint.y() + radius*sin(M_PI/6)));
}

void Simulation::draw10degLines(QPainter &painter)
{
    QColor lineColor(Qt::green);
    QPen pen;
    pen.setWidth(1);
    if( radarAngle >= 19 * M_PI / 18 && radarAngle <= 14*M_PI / 9)
        lineColor.setAlpha(0);
    else
        lineColor.setAlpha(255);
    pen.setColor(lineColor);
    painter.setPen(pen);
    painter.drawLine(centerPoint, QPointF( centerPoint.x() - radius*cos(M_PI/18), centerPoint.y() - radius*sin(M_PI/18)));

    if( radarAngle >= 10 * M_PI / 9 && radarAngle <= 29*M_PI / 18)
        lineColor.setAlpha(0);
    else
        lineColor.setAlpha(255);
    pen.setColor(lineColor);
    painter.setPen(pen);
    painter.drawLine(centerPoint, QPointF( centerPoint.x() - radius*cos(M_PI/9), centerPoint.y() - radius*sin(M_PI/9)));

    if( radarAngle >= 11 * M_PI / 9 && radarAngle <= 31*M_PI / 18)
        lineColor.setAlpha(0);
    else
        lineColor.setAlpha(255);
    pen.setColor(lineColor);
    painter.setPen(pen);
    painter.drawLine(centerPoint, QPointF( centerPoint.x() - radius*cos(2*M_PI/9), centerPoint.y() - radius*sin(2*M_PI/9)));

    if( radarAngle >= 23 * M_PI / 18 && radarAngle <= 32*M_PI / 18)
        lineColor.setAlpha(0);
    else
        lineColor.setAlpha(255);
    pen.setColor(lineColor);
    painter.setPen(pen);
    painter.drawLine(centerPoint, QPointF( centerPoint.x() - radius*cos(5*M_PI/18), centerPoint.y() - radius*sin(5*M_PI/18)));

    if( radarAngle >= 25 * M_PI / 18 && radarAngle <= 17*M_PI / 9)
        lineColor.setAlpha(0);
    else
        lineColor.setAlpha(255);
    pen.setColor(lineColor);
    painter.setPen(pen);
    painter.drawLine(centerPoint, QPointF( centerPoint.x() - radius*cos(7*M_PI/18), centerPoint.y() - radius*sin(7*M_PI/18)));

    if( radarAngle >= 13 * M_PI / 9 && radarAngle <= 35*M_PI / 18)
        lineColor.setAlpha(0);
    else
        lineColor.setAlpha(255);
    pen.setColor(lineColor);
    painter.setPen(pen);
    painter.drawLine(centerPoint, QPointF( centerPoint.x() - radius*cos(4*M_PI/9), centerPoint.y() - radius*sin(4*M_PI/9)));

    if( (radarAngle >= 14 * M_PI / 9 && radarAngle <= 2*M_PI) || (radarAngle >= 0 && radarAngle <= M_PI / 18))
        lineColor.setAlpha(0);
    else
        lineColor.setAlpha(255);
    pen.setColor(lineColor);
    painter.setPen(pen);
    painter.drawLine(centerPoint, QPointF( centerPoint.x() - radius*cos(5*M_PI/9), centerPoint.y() - radius*sin(5*M_PI/9)));

    if( (radarAngle >= 29 * M_PI / 18 && radarAngle <= 2*M_PI) || (radarAngle >= 0 && radarAngle <= M_PI / 9))
        lineColor.setAlpha(0);
    else
        lineColor.setAlpha(255);
    pen.setColor(lineColor);
    painter.setPen(pen);
    painter.drawLine(centerPoint, QPointF( centerPoint.x() - radius*cos(11*M_PI/18), centerPoint.y() - radius*sin(11*M_PI/18)));

    if( (radarAngle >= 31 * M_PI / 18 && radarAngle <= 2*M_PI) || (radarAngle >= 0 && radarAngle <= 2*M_PI / 9))
        lineColor.setAlpha(0);
    else
        lineColor.setAlpha(255);
    pen.setColor(lineColor);
    painter.setPen(pen);
    painter.drawLine(centerPoint, QPointF( centerPoint.x() - radius*cos(13*M_PI/18), centerPoint.y() - radius*sin(13*M_PI/18)));

    if( (radarAngle >= 16 * M_PI / 9 && radarAngle <= 2*M_PI) || (radarAngle >= 0 && radarAngle <= 5*M_PI / 18))
        lineColor.setAlpha(0);
    else
        lineColor.setAlpha(255);
    pen.setColor(lineColor);
    painter.setPen(pen);
    painter.drawLine(centerPoint, QPointF( centerPoint.x() - radius*cos(7*M_PI/9), centerPoint.y() - radius*sin(7*M_PI/9)));

    if( (radarAngle >= 17 * M_PI / 9 && radarAngle <= 2*M_PI) || (radarAngle >= 0 && radarAngle <= 7*M_PI / 18))
        lineColor.setAlpha(0);
    else
        lineColor.setAlpha(255);
    pen.setColor(lineColor);
    painter.setPen(pen);
    painter.drawLine(centerPoint, QPointF( centerPoint.x() - radius*cos(8*M_PI/9), centerPoint.y() - radius*sin(8*M_PI/9)));

    if( (radarAngle >= 35 * M_PI / 18 && radarAngle <= 2*M_PI) || (radarAngle >= 0 && radarAngle <= 4*M_PI / 9))
        lineColor.setAlpha(0);
    else
        lineColor.setAlpha(255);
    pen.setColor(lineColor);
    painter.setPen(pen);
    painter.drawLine(centerPoint, QPointF( centerPoint.x() - radius*cos(17*M_PI/18), centerPoint.y() - radius*sin(17*M_PI/18)));

    if( radarAngle >= M_PI / 18  && radarAngle <= 5*M_PI / 9)
        lineColor.setAlpha(0);
    else
        lineColor.setAlpha(255);
    pen.setColor(lineColor);
    painter.setPen(pen);
    painter.drawLine(centerPoint, QPointF( centerPoint.x() - radius*cos(19*M_PI/18), centerPoint.y() - radius*sin(19*M_PI/18)));

    if( radarAngle >= M_PI / 9  && radarAngle <= 11*M_PI / 18)
        lineColor.setAlpha(0);
    else
        lineColor.setAlpha(255);
    pen.setColor(lineColor);
    painter.setPen(pen);
    painter.drawLine(centerPoint, QPointF( centerPoint.x() - radius*cos(10*M_PI/9), centerPoint.y() - radius*sin(10*M_PI/9)));

    if( radarAngle >= 2*M_PI / 9  && radarAngle <= 13*M_PI / 18)
        lineColor.setAlpha(0);
    else
        lineColor.setAlpha(255);
    pen.setColor(lineColor);
    painter.setPen(pen);
    painter.drawLine(centerPoint, QPointF( centerPoint.x() - radius*cos(11*M_PI/9), centerPoint.y() - radius*sin(11*M_PI/9)));

    if( radarAngle >= 5*M_PI / 18  && radarAngle <= 7*M_PI / 9)
        lineColor.setAlpha(0);
    else
        lineColor.setAlpha(255);
    pen.setColor(lineColor);
    painter.setPen(pen);
    painter.drawLine(centerPoint, QPointF( centerPoint.x() - radius*cos(23*M_PI/18), centerPoint.y() - radius*sin(23*M_PI/18)));

    if( radarAngle >= 7*M_PI / 18  && radarAngle <= 8*M_PI / 9)
        lineColor.setAlpha(0);
    else
        lineColor.setAlpha(255);
    pen.setColor(lineColor);
    painter.setPen(pen);
    painter.drawLine(centerPoint, QPointF( centerPoint.x() - radius*cos(25*M_PI/18), centerPoint.y() - radius*sin(25*M_PI/18)));

    if( radarAngle >= 4*M_PI / 9  && radarAngle <= 17*M_PI / 18)
        lineColor.setAlpha(0);
    else
        lineColor.setAlpha(255);
    pen.setColor(lineColor);
    painter.setPen(pen);
    painter.drawLine(centerPoint, QPointF( centerPoint.x() - radius*cos(13*M_PI/9), centerPoint.y() - radius*sin(13*M_PI/9)));

    if( radarAngle >= 5*M_PI / 9  && radarAngle <= 19*M_PI / 18)
        lineColor.setAlpha(0);
    else
        lineColor.setAlpha(255);
    pen.setColor(lineColor);
    painter.setPen(pen);
    painter.drawLine(centerPoint, QPointF( centerPoint.x() - radius*cos(14*M_PI/9), centerPoint.y() - radius*sin(14*M_PI/9)));

    if( radarAngle >= 11*M_PI / 18  && radarAngle <= 10*M_PI / 9)
        lineColor.setAlpha(0);
    else
        lineColor.setAlpha(255);
    pen.setColor(lineColor);
    painter.setPen(pen);
    painter.drawLine(centerPoint, QPointF( centerPoint.x() - radius*cos(29*M_PI/18), centerPoint.y() - radius*sin(29*M_PI/18)));

    if( radarAngle >= 13*M_PI / 18  && radarAngle <= 11*M_PI / 9)
        lineColor.setAlpha(0);
    else
        lineColor.setAlpha(255);
    pen.setColor(lineColor);
    painter.setPen(pen);
    painter.drawLine(centerPoint, QPointF( centerPoint.x() - radius*cos(31*M_PI/18), centerPoint.y() - radius*sin(31*M_PI/18)));

    if( radarAngle >= 7*M_PI / 9  && radarAngle <= 23*M_PI / 18)
        lineColor.setAlpha(0);
    else
        lineColor.setAlpha(255);
    pen.setColor(lineColor);
    painter.setPen(pen);
    painter.drawLine(centerPoint, QPointF( centerPoint.x() - radius*cos(16*M_PI/9), centerPoint.y() - radius*sin(16*M_PI/9)));

    if( radarAngle >= 8*M_PI / 9  && radarAngle <= 25*M_PI / 18)
        lineColor.setAlpha(0);
    else
        lineColor.setAlpha(255);
    pen.setColor(lineColor);
    painter.setPen(pen);
    painter.drawLine(centerPoint, QPointF( centerPoint.x() - radius*cos(17*M_PI/9), centerPoint.y() - radius*sin(17*M_PI/9)));

    if( radarAngle >= 17*M_PI / 18  && radarAngle <= 13*M_PI / 9)
        lineColor.setAlpha(0);
    else
        lineColor.setAlpha(255);
    pen.setColor(lineColor);
    painter.setPen(pen);
    painter.drawLine(centerPoint, QPointF( centerPoint.x() - radius*cos(35*M_PI/18), centerPoint.y() - radius*sin(35*M_PI/18)));
}

bool Simulation::checkRoutePoint(double xcheck, double ycheck, double x, double y)
{
    //сначала проверим угол
    double difx = x - xcheck;
    double dify = y - ycheck;
    difx = (difx > 0) ? difx : -difx;
    dify = (dify > 0) ? dify : -dify;

    return (difx <= accuracy) && (dify <= accuracy);
}

void Simulation::drawOldPoints(QPainter &painter)
{
    //обработать нарисованные точки, которые в слепой зоне надо удалить из списка
    for(int i = 0; i < drawnPoints.size();)
    {
        BufferEntry p = drawnPoints.at(i);
        double ang = p.angle;
        //слепая зона - от realAngle и до realAngle + пи/2
        double realAngle = (radarAngle >= M_PI/2) ? radarAngle - M_PI/2 : 1.5*M_PI + radarAngle;
        if(realAngle > 1.5*M_PI) // надо сделать две проверки
        {
            double from = realAngle;
            double to = 2*M_PI;
            if(ang >= from && ang <= to)
            {
                //убрать из списка
                drawnPoints.removeAt(i);
                continue;
            }
            if(ang >= 0 && ang <= (realAngle - 1.5 * M_PI))
            {
                //убрать из списка
                drawnPoints.removeAt(i);
                continue;
            }
        }
        else //только одна проверка
        {
            if(ang >= realAngle && ang <= (realAngle + 0.5 * M_PI))
            {
                drawnPoints.removeAt(i);
                continue;
            }
        }

        drawPoint(painter, p.point.x(), p.point.y(), p.angle);
        i++;
    }
}


void Simulation::drawPoints(QPainter &painter)
{
    QColor pointColor(Qt::green);
    pointColor.setAlpha(255);
    QPen pen;
    pen.setColor(pointColor);
    pen.setWidth(5);
    painter.setPen(pen);
    for(int i = 0; i < objCount; i++)
    {
        SIM_OBJ obj = simObjects[i];
        double x = obj.curPoint.x();
        double y = obj.curPoint.y();

        double len = sqrt(pow((x - centerPoint.x()),2) + pow((y - centerPoint.y()), 2));
        if(len > radius)
            continue;

        //высчитываем уравнение прямой для линии рлс
        double realAngle = (radarAngle >= M_PI/2) ? radarAngle - M_PI/2 : 1.5*M_PI + radarAngle;
        double y1 =centerPoint.y() -  radius * sin(realAngle);
        double x1 =centerPoint.x() -  radius * cos(realAngle);

        double k = (centerPoint.y() - y1) / (centerPoint.x() - x1);

        double b = y1 - k * x1;

        double ycheck = k * x + b;
        double xcheck = (y - b) / k;

        //проверим на зеркальность
        if(x1 <= centerPoint.x())
        {
            if(!(x < x1 || x > centerPoint.x()))
                continue;
        }
        else
        {
            if(!( x < centerPoint.x() || x > x1))
                continue;
        }

        if(y1 <= centerPoint.y())
        {
            if(!(y < y1 || y > centerPoint.y()))
                continue;
        }
        else
        {
            if(!( y < centerPoint.y()|| y > y1))
                continue;
        }

        if(checkRoutePoint(xcheck, ycheck, x, y))//точка совпадает с линией
        {
            drawPoint(painter,x,y,realAngle);

            //сохранить точку в список точек
            BufferEntry be;
            be.angle = realAngle;
            be.point =  obj.curPoint;
            be.index = i;
            drawnPoints.append( be );
        }
    }
}

void Simulation::drawPoint(QPainter &painter, double x, double y, double angle, bool enemy)
{
    QPen pp;
    pp.setColor(Qt::green);
    pp.setWidth(arcWidth);
    painter.setPen(pp);
    painter.drawPoint(x,y);

    double ta = x - centerPoint.x();
    double ba = y - centerPoint.y();

    double arcOffset = sqrt(ta*ta + ba*ba);

    double a = arcOffset * cos(angle);
    double b = arcOffset * sin(angle);
    double xcoord = x - a;
    double ycoord = y - b;

    double xr = xcoord - arcOffset;
    double yr = ycoord - arcOffset;

    //вычислим угол
    double ang = 180 * asin(hordHalf/arcOffset) / M_PI;
   // qDebug(QString("ang = " + QString::number(ang)).toStdString().c_str());

    painter.drawArc(xr,yr, arcOffset*2, arcOffset*2, (360 - ang - angle * 180/M_PI) * 16, 2*ang * 16  );

    //нарисовать дужку опознования

    if(!enemy)
    {
        //вычислить координаты центра новой дуги,т.е. та же точка, только несколько дальше
        double os = 6;
        double oy = os * sin(angle);
        double ox = os * cos(angle);
        x += ox;
        y += oy;
        drawArcId(painter, x, y, angle);


    }
}

void Simulation::drawArcId(QPainter &painter, double x, double y, double angle)
{
    QPen pp;
    pp.setColor(Qt::green);
    pp.setWidth(arcWidth-1);
    painter.setPen(pp);
    painter.drawPoint(x,y);

    double ta = x - centerPoint.x();
    double ba = y - centerPoint.y();

    double arcOffset = sqrt(ta*ta + ba*ba);

    double a = arcOffset * cos(angle);
    double b = arcOffset * sin(angle);
    double xcoord = x - a;
    double ycoord = y - b;

    double xr = xcoord - arcOffset;
    double yr = ycoord - arcOffset;

    //вычислим угол
    double ang = 180 * asin((hordHalf + 12) /arcOffset) / M_PI;
    //qDebug(QString("ang = " + QString::number(ang)).toStdString().c_str());

    painter.drawArc(xr,yr, arcOffset*2, arcOffset*2, (360 - ang - angle * 180/M_PI) * 16, 2*ang * 16  );

}

void Simulation::drawLenArcks(QPainter &painter)
{
    QColor arcColor(Qt::green);
    arcColor.setAlpha(255);
    QPen pen;
    pen.setColor(arcColor);
    painter.setPen(pen);

    //нарисуем 8 круговых отметок
    painter.drawArc( 100, 100, 800, 800, -(radarAngle * 180 / M_PI - 90 ) * 16 , 270 * 16 );
    painter.drawArc( 150, 150, 700, 700, -(radarAngle * 180 / M_PI - 90 ) * 16 , 270 * 16 );
    painter.drawArc( 200, 200, 600, 600, -(radarAngle * 180 / M_PI - 90 ) * 16 , 270 * 16 );
    painter.drawArc( 250, 250, 500, 500, -(radarAngle * 180 / M_PI - 90 ) * 16 , 270 * 16 );
    painter.drawArc( 300, 300, 400, 400, -(radarAngle * 180 / M_PI - 90 ) * 16 , 270 * 16 );
    painter.drawArc( 350, 350, 300, 300, -(radarAngle * 180 / M_PI - 90 ) * 16 , 270 * 16 );
    painter.drawArc( 400, 400, 200, 200, -(radarAngle * 180 / M_PI - 90 ) * 16 , 270 * 16 );
    painter.drawArc( 450, 450, 100, 100, -(radarAngle * 180 / M_PI - 90 ) * 16 , 270 * 16 );
}

void Simulation::drawRandomPoints(QPainter &painter)
{
    QPen pen;

    pen.setColor(Qt::green);
    pen.setWidth(2);
    painter.setPen(pen);

    int pointsAmount = 1000;

    pen.setWidth(1);
    pen.setColor(Qt::green);
    painter.setPen(pen);

    for( int i = 0 ; i < pointsAmount ; i++ )
    {
        double pointAngle = radarAngle + log((double)rand() / RAND_MAX) / 1.5;
        int pointD = rand() % 400;
        painter.drawPoint(centerPoint.x() + (pointD * sin(pointAngle)), centerPoint.y() + (-pointD * cos(pointAngle)));
    }

    pointsAmount = 200;

    for( int i = 0 ; i < pointsAmount ; i++ )
    {
        int pointD = rand() % 400;
        painter.drawPoint(centerPoint.x() + (pointD * sin(radarAngle)), centerPoint.y() + (-pointD * cos(radarAngle)));
        painter.drawPoint(centerPoint.x() + (pointD * sin(radarAngle - 0.005)), centerPoint.y() + (-pointD * cos(radarAngle - 0.005)));
        painter.drawPoint(centerPoint.x() + (pointD * sin(radarAngle - 0.01)), centerPoint.y() + (-pointD * cos(radarAngle - 0.01)));
    }
}

bool Simulation::checkClick(double x1, double y1, double x2, double y2)
{
    x1 = x1 - x2;
    y1 = y1 - y2;

    x1 = (x1 >= 0) ? x1 : -x1;
    y1 = (y1 >= 0) ? y1 : -y1;
    return (x1 <= accuracyClick) && (y1 <= accuracyClick);

}

void Simulation::mouseReleaseEvent(QMouseEvent *e)
{
    int cx = e->x();
    int cy = e->y();
    QList<BufferEntry> candidates;
    foreach(BufferEntry be, drawnPoints)
    {
        double x = be.point.x();
        double y = be.point.y();
        if(checkClick(cx, cy, x, y))
        {
            //запишем точку в кандидаты
            candidates.append(be);
        }
    }

    if(candidates.size() == 0)//никого не занесли
    {
        //вызов добавления
        qDebug(QString("no target, x =  " + QString::number(cx) + " y = " + QString::number(cy)).toStdString().c_str());

    }
    else {
        int id = findClosest(candidates, cx, cy);
        qDebug(QString(QString::number(id) + " x =  " + QString::number(cx) + " y = " + QString::number(cy)).toStdString().c_str());
    }
}

int Simulation::findClosest(QList<BufferEntry> &l, double x, double y)
{
    int minind = l.first().index;
    double minlen = pow(x - l.first().point.x(), 2) + pow(y - l.first().point.y(), 2);

    for(int i = 1; i < l.size(); i++)
    {
        BufferEntry be = l.at(i);
        double len = pow(x - be.point.x(), 2) + pow(y - be.point.y(), 2);
        if( len < minlen)
        {
            minlen = len;
            minind = l.at(i).index;
        }
    }
    return minind;
}
