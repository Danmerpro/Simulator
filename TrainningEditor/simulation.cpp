#include "simulation.h"

Simulation::Simulation(QList<MapObj *> *_objects, QWidget *parent) :
    QWidget(parent)
{
    objects = _objects;

    ptimer = new QTimer();
    ptimer->setInterval(40);

    curPoints = new QList<RoutePoint>();

    QList<MapObj*>::Iterator it = objects->begin();
    for( it ; it != objects->end() ; it++ )
    {
        RoutePoint p;
        p.setX( (*(*it)->getPoints()->begin()).x());
        p.setY( (*(*it)->getPoints()->begin()).y());
        p.setAlt( (*(*it)->getPoints()->begin()).getAlt());
        p.setSpeed( (*(*it)->getPoints()->begin()).getSpeed());
        curPoints->append( p );
    }
    lastInCurRoute = new QList<QList<RoutePoint>::iterator>();
    it = objects->begin();
    for( it ; it != objects->end() ; it++ )
    {
        lastInCurRoute->append((*it)->getPoints()->begin());
    }


    connect(ptimer, SIGNAL(timeout()), this, SLOT(updateSimulation()));
}

void Simulation::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPointF from;
    QPointF to;
    QFont* font = new QFont();
    pen = new QPen();
    brush = new QBrush(Qt::SolidPattern);
    pen->setWidth(2);
    pen->setColor(Qt::black);
    painter.setPen(*pen);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawEllipse(QPoint(500,350),300,300);
    painter.drawEllipse(QPoint(500,350),200,200);
    painter.drawEllipse(QPoint(500,350),100,100);
    painter.drawLine(500,50,500,650);
    painter.drawLine(200,350,800,350);
    pen->setWidth(1);
    painter.setPen(*pen);
    painter.drawEllipse(QPoint(500,350),250,250);
    painter.drawEllipse(QPoint(500,350),150,150);
    painter.drawEllipse(QPoint(500,350),50,50);
    pen->setWidth(2);
    pen->setColor(Qt::green);
    painter.setPen(*pen);
    font->setBold(false);
    painter.setFont(*font);
    QList<MapObj*>::iterator it = objects->begin();
    QList<QList<RoutePoint>::iterator>::iterator itL = lastInCurRoute->begin();
    QList<RoutePoint>::iterator itCur = curPoints->begin();
    for( it,itL,itCur ; it != objects->end() ; it++,itL++,itCur++ )
    {
        pen->setWidth(2);
        if( (*it)->getAsseccory() == ours )
        {
            pen->setColor(Qt::blue);
            brush->setColor(Qt::blue);
        }
        if( (*it)->getAsseccory() == alien )
        {
            pen->setColor(Qt::red);
            brush->setColor(Qt::red);
        }
        painter.setPen(*pen);
        painter.setBrush(*brush);
        QList<RoutePoint>::iterator itC = (*it)->getPoints()->begin();
        for( itC ; itC != (*it)->getPoints()->end() ; itC++ )
        {
            if( itC != (*itL) )
            {
                if( itC+1 != (*it)->getPoints()->end())
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
        to = (*itCur);
        painter.drawLine(from,to);
    }

}

void Simulation::start()
{
    timeElapsed = new QTime();
    timeCounter = 0;
    ptimer->start();
}

void Simulation::pause()
{

}

void Simulation::stop()
{

}

void Simulation::updateSimulation()
{
    qreal dSpeedX, dSpeedY;
    qreal dAltX, dAltY;
    qreal dX, dY;
    qreal aX, aY;
    double angle;

    QList<RoutePoint>::iterator it = curPoints->begin();
    QList<QList<RoutePoint>::iterator>::iterator itL = lastInCurRoute->begin();
    for( it,itL ; it != curPoints->end() ; it++,itL++ )
    {
        aX = 10;
        aY = 10;
        (*it).setX( (*itL)->x() + 10*timeCounter*0.04 );
        (*it).setY( (*itL)->y() + 10*timeCounter*0.04 );
    }
    timeCounter++;
    update();
}
