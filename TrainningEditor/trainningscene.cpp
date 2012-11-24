#include "trainningscene.h"

TrainningScene::TrainningScene(QWidget *parent) :
    QWidget(parent)
{
    drawling = false;
    drawlingMode = false;
    editingMode = false;
    curCruve = NULL;
    curObj = NULL;
    curPoint = NULL;
    connect(this,SIGNAL(newRouteAdded(MapObj*)),this, SLOT(procesingNewRoute(MapObj*)));
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
    if( editingMode )
    {
        if( curPoint != NULL )
        {
            pen->setColor(Qt::black);
            brush->setColor(Qt::red);
            painter.setBrush(*brush);
            painter.setPen(*pen);
            painter.drawEllipse(curPoint->getPoints()->first(),5,5);
        }
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
    if( editingMode )
    {
        ifOnCurRoute(event->posF());
        update();
    }
}

void TrainningScene::mouseReleaseEvent(QMouseEvent *event)
{
    if( drawlingMode )
    {
        if(event->button() == Qt::LeftButton)
        {
            drawlingMode = false;
            drawling = false;
            this->setCursor(Qt::ArrowCursor);
            if(!curCruve->isEmpty())
            {
                MapObj route( *curCruve );
                route.setType(cruve);
                objects.append(route);
                emit newRouteAdded( &objects.last());
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

 qreal TrainningScene::absSC( qreal val )
 {
     return ( val >= 0 ) ? val :-val;
 }

void TrainningScene::drawlingModeOn()
{
    drawlingMode = true;
}

void TrainningScene::procesingNewRoute( MapObj *obj )
{
    curObj = obj;
    editingMode = true;
    this->setMouseTracking(true);
}

bool TrainningScene::ifOnCurRoute(QPointF p)
{
    curPoint = NULL;
    QList<QPointF> *points = curObj->getPoints();
    QList<QPointF>::iterator it = points->begin();
    for( it ; it != points->end() ; it+=3 )
    {
        if( it+1 != points->end() && it+2 != points->end() && it+3 != points->end() )
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
                qreal delta = 1.0/line.length();
                for( qreal k = 0 ; k <= 1.0 ; k+=delta )
                {
                    QPointF centr = line.pointAt(k);
                    QRectF rect(centr.x()-2,centr.y()-2,4,4);
                    if( rect.contains(p))
                    {
                        QList<QPointF> pt;
                        pt.append(p);
                        curPoint = new MapObj(pt);
                        curPoint->setType(point);
                        return true;
                    }
                }
                if( delta > 1 )
                {
                    QPointF centr = line.pointAt(1);
                    QRectF rect(centr.x()-2,centr.y()-2,4,4);
                    if( rect.contains(p))
                    {
                        QList<QPointF> pt;
                        pt.append(p);
                        curPoint = new MapObj(pt);
                        curPoint->setType(point);
                        return true;
                    }
                }

            }
        }
        else
        {
            break;
        }
    }
}
