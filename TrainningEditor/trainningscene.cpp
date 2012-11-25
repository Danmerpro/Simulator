#include "trainningscene.h"

TrainningScene::TrainningScene( QList<MapObj*> *_objects, QWidget *parent) :
    QWidget(parent)
{
    objects = _objects;
    darwlingRouteMode = false;
    editingMode = false;
    curObj = NULL;
    connect(this,SIGNAL(newRouteAdded()),this, SLOT(procesingNewRoute()));
}

void TrainningScene::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPointF from;
    QPointF to;
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
    QList<MapObj*>::iterator it = objects->begin();
    for( it ; it != objects->end() ; it++ )
    {
        if( (*it) != curObj )
        {
            pen->setColor(Qt::green);
            pen->setWidth(2);
            brush->setColor(Qt::green);
            painter.setPen(*pen);
            painter.setBrush(*brush);
            QList<RoutePoint>::iterator itC = (*it)->getPoints()->begin();
            for( itC ; itC != (*it)->getPoints()->end() ; itC++ )
            {
                if( itC+1 != (*it)->getPoints()->end())
                {
                    from = (*itC);
                    to = (*(itC+1));
                }
                painter.drawLine(from,to);
            }
            pen->setColor(Qt::black);
            pen->setWidth(1);
            painter.setPen(*pen);
            int c = 0;
            for( itC = (*it)->getPoints()->begin(); itC != (*it)->getPoints()->end() ; itC++, c++ )
            {
                QPointF pLable((*itC).x()+6,(*itC).y()+6);
                painter.drawEllipse((*itC),4,4);
                painter.drawText(pLable, QString().setNum(c+1));
            }
        }
    }
    if( curObj != NULL )
    {
        pen->setColor(Qt::blue);
        pen->setWidth(2);
        brush->setColor(Qt::blue);
        painter.setBrush(*brush);
        painter.setPen(*pen);
        QList<RoutePoint>::iterator itC = curObj->getPoints()->begin();
        for( itC ; itC != curObj->getPoints()->end() ; itC++ )
        {
            if( itC+1 != curObj->getPoints()->end())
            {
                from = (*itC);
                to = (*(itC+1));
            }
            else
            {
                if( darwlingRouteMode )
                {
                    from = (*itC);
                    to = curMousePos;
                }
            }
            painter.drawLine(from,to);
        }
        pen->setColor(Qt::black);
        pen->setWidth(1);
        painter.setPen(*pen);
        int c = 0;
        for( itC = curObj->getPoints()->begin(); itC != curObj->getPoints()->end() ; itC++, c++ )
        {
            QPointF pLable((*itC).x()+6,(*itC).y()+6);
            painter.drawEllipse((*itC),4,4);
            painter.drawText(pLable,QString().setNum( c+1 ));
        }
    }
}

void TrainningScene::mousePressEvent(QMouseEvent *event)
{
    if( darwlingRouteMode )
    {
        if(event->button() == Qt::LeftButton)
        {
            curObj->appendPoint( RoutePoint(event->posF()) );
        }
        if(event->button() == Qt::RightButton)
        {
            emit newRouteAdded();
        }
        update();
    }
}

void TrainningScene::mouseMoveEvent(QMouseEvent *event)
{
    if( darwlingRouteMode )
    {
        curMousePos = event->posF();
        update();
    }
}
/*
void TrainningScene::drawBesierSpline( QPainter* painter,QList<QPointF> *points )
{
    int pointsAmount = points->size();
    int twoGroups = 0;
    int threeGroups = 0;
    int toAdd = 0;
    if( pointsAmount <= 4 )
    {
        toAdd = 4 - pointsAmount;
        threeGroups = 1;
    }
    else
    {
        if( pointsAmount <= 6 )
        {
            toAdd = 6 - pointsAmount;
            threeGroups = 2;
        }
        else
        {
            threeGroups = 2;
            twoGroups = ceil((double)(pointsAmount - 6)/2);
            toAdd = twoGroups*2 - pointsAmount;
        }
    }
    for( int j = 0 ; j < toAdd; j++)
        points->append(points->last());
    QList<QPointF> tmp;
    QList<QPointF>::iterator itC = points->begin();
    int i = 0;
    for( itC ; itC != points->end() ; itC++, i++ )
    {
        tmp.append((*itC));
        if( i == 2 )
        {
            tmp.append(QPointF(((*itC).x()+(*(itC+1)).x())/2, ((*itC).y()+(*(itC+1)).y())/2));
        }
        if( (i-3)%2 == 1 && points->size()-i > 1 )
        {
            tmp.append(QPointF(((*itC).x()+(*(itC+1)).x())/2, ((*itC).y()+(*(itC+1)).y())/2));
        }
    }
    QList<QPointF>::iterator it = tmp.begin();
    for( it ; it != tmp.end() ; it+=3 )
    {
        if( it+1 != tmp.end() && it+2 != tmp.end() && it+3 != tmp.end())
        {
            qreal xpos1;
            qreal ypos1;
            qreal xpos2;
            qreal ypos2;
            for( double t = 0.0; t < 1-0.1 ; t+=0.1 )
            {
                xpos1 = pow(1.0-t,3)*(*it).x()+3*t*pow(1.0-t,2)*(*(it+1)).x()+3*pow(t,2)*(1.0-t)*(*(it+2)).x()+pow(t,3)*(*(it+3)).x();
                ypos1 = pow(1.0-t,3)*(*it).y()+3*t*pow(1.0-t,2)*(*(it+1)).y()+3*pow(t,2)*(1.0-t)*(*(it+2)).y()+pow(t,3)*(*(it+3)).y();
                xpos2 = pow(1.0-t-0.1,3)*(*it).x()+3*(t+0.1)*pow(1.0-t-0.1,2)*(*(it+1)).x()+3*pow(t+0.1,2)*(1.0-t-0.1)*(*(it+2)).x()+pow(t+0.1,3)*(*(it+3)).x();
                ypos2 = pow(1.0-t-0.1,3)*(*it).y()+3*(t+0.1)*pow(1.0-t-0.1,2)*(*(it+1)).y()+3*pow(t+0.1,2)*(1.0-t-0.1)*(*(it+2)).y()+pow(t+0.1,3)*(*(it+3)).y();
                QLineF line(QPointF(xpos1, ypos1),QPointF(xpos2, ypos2));
                painter->drawLine( line );
            }
        }
        else
        {
            break;
        }
    }
}
*/

void TrainningScene::drawlingModeOn()
{
    darwlingRouteMode = true;
    this->setCursor(Qt::CrossCursor);
    curObj = new MapObj();
    objects->append( curObj );
    this->setMouseTracking( true );
}

void TrainningScene::procesingNewRoute()
{
    darwlingRouteMode = false;
    this->setCursor(Qt::ArrowCursor);
    this->setMouseTracking( false );
    editingMode = true;
    emit routeEditing( curObj );
}

void TrainningScene::finishEdit()
{
    editingMode = false;
}


