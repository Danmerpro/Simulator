#include "trainningscene.h"

TrainningScene::TrainningScene(QWidget *parent) :
    QWidget(parent)
{
    drawling = false;
    drawlingMode = false;
    curCruve = NULL;
    this->setMouseTracking(true);
}

void TrainningScene::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPointF from;
    QPointF to;
    pen = new QPen();
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
    if( drawling )
    {
        if( curCruve != NULL )
        {
            QList<QPointF>::iterator itC = curCruve->begin();
            for( itC ; itC != curCruve->end() ; itC++ )
            {
                if( itC+1 != curCruve->end())
                {
                    from = (*itC);
                    to = (*(itC+1));
                }
                painter.drawLine(from,to);
            }
        }
    }
    QList<MapObj>::iterator it = objects.begin();
    for( it ; it != objects.end() ; it++ )
    {
        pen->setColor(Qt::green);
        if( &(*it) == curObj)
        {
            pen->setColor(Qt::blue);
        }
        painter.setPen(*pen);
        drawBesierSpline( &painter, (*it).getPoints());
    }
}

void TrainningScene::mousePressEvent(QMouseEvent *event)
{
    if( drawlingMode )
    {
        if(event->button() == Qt::LeftButton)
        {
            drawling = true;
            this->setCursor(Qt::CrossCursor);
            curCruve = new QList<QPointF>();
        }
    }
}

void TrainningScene::mouseMoveEvent(QMouseEvent *event)
{
    if( drawlingMode )
    {
        if( drawling )
        {
            curCruve->append(event->posF());
            update();
        }
    }
}

void TrainningScene::mouseReleaseEvent(QMouseEvent *event)
{
    if( drawlingMode )
    {
        if(event->button() == Qt::LeftButton)
        {
            drawling = false;
            this->setCursor(Qt::ArrowCursor);
            if(!curCruve->isEmpty())
            {
                MapObj cruve( *curCruve );
                objects.append(cruve);
                update();
            }
        }
    }
}

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
                painter->drawLine(QPointF( xpos1, ypos1 ), QPointF( xpos2, ypos2 ) );
            }

        }
        else
        {
            break;
        }
    }
}

MapObj* TrainningScene::ifOnCruve(QPointF p)
{

}
