#include "simulation.h"
#include <iostream>

Simulation::Simulation(QList<MapObj *> *_objects, QWidget *parent) :
    QWidget(parent)
{
    this->setFixedSize(1000,1000);

    timerForMenu = new QTimer();
    timerForMenu->setInterval(1000);

    ptimer = new QTimer();
    ptimer->setInterval(40);

    objCount = _objects->size();
    simObjects = new SIM_OBJ[objCount];

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
}

void Simulation::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    RoutePoint from;
    RoutePoint to;
    QFont* font = new QFont();
    pen = new QPen();
    brush = new QBrush(Qt::SolidPattern);
    pen->setWidth(2);
    pen->setColor(Qt::black);
    painter.setPen(*pen);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawEllipse(QPoint(500,500),400,400);
    painter.drawEllipse(QPoint(500,500),300,300);
    painter.drawEllipse(QPoint(500,500),200,200);
    painter.drawEllipse(QPoint(500,500),100,100);
    painter.drawLine(500,100,500,900);
    painter.drawLine(100,500,900,500);
    pen->setWidth(1);
    painter.setPen(*pen);
    painter.drawEllipse(QPoint(500,500),350,350);
    painter.drawEllipse(QPoint(500,500),250,250);
    painter.drawEllipse(QPoint(500,500),150,150);
    painter.drawEllipse(QPoint(500,500),50,50);
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
    timeElapsed = new QTime(0, 0, 0, 0);
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
    delete timeElapsed;
    timeElapsed = new QTime(0, 0, 0, 0);
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
        if( simObjects[i].complete == false && *timeElapsed >= simObjects[i].obj->getStartTime() )
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
        *timeElapsed = timeElapsed->addMSecs(40);
        update();
    }
    else
    {
        stop();
    }
}
