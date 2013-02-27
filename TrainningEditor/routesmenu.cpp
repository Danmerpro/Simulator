#include "routesmenu.h"

RoutesMenu::RoutesMenu(QList<MapObj*> *_objects, QWidget *parent) :
    QWidget(parent)
{
    objects = _objects;
    newRouteButton = new QPushButton( this );
    newRouteButton->setText( tr("Новый маршрут") );
    deleteRouteButton = new QPushButton( this );
    deleteRouteButton->setText( tr("Удалить") );
    deleteRouteButton->setEnabled(false);
    editRouteButton = new QPushButton( this );
    editRouteButton->setText( tr("Редактировать") );
    editRouteButton->setEnabled(false);
    QHBoxLayout *hLayout1 = new QHBoxLayout();
    hLayout1->addWidget( newRouteButton );
    hLayout1->addWidget( deleteRouteButton );
    hLayout1->addWidget( editRouteButton );
    QVBoxLayout *VLayout1 = new QVBoxLayout();
    VLayout1->addLayout(hLayout1);
    rtList = new RoutesList(this);
    VLayout1->addWidget(new QLabel(tr("Маршруты:"),this));
    VLayout1->addWidget(rtList);
    startButton = new QPushButton( this );
    startButton->setText( tr("Начать тренеровку") );
    startButton->adjustSize();
    startButton->setEnabled(false);
    QHBoxLayout *hLayout2 = new QHBoxLayout();
    hLayout2->addStretch();
    hLayout2->addWidget(startButton);
    VLayout1->addLayout(hLayout2);
 //   VLayout1->addStretch();
    this->setLayout( VLayout1 );
    this->setMinimumWidth(300);
    connect(rtList, SIGNAL(currentRowChanged(int)), this, SLOT(listIndChanged(int)));
    connect(deleteRouteButton, SIGNAL(clicked()), this, SLOT(deleteCurRoute()));
    connect(editRouteButton, SIGNAL(clicked()), this, SIGNAL(editCurRoute()));
    connect(newRouteButton, SIGNAL(clicked()), this, SLOT(setNewRouteButtonDisable()));
    updateList();
}

QPushButton* RoutesMenu::getNewRouteButton()
{
    return newRouteButton;
}

void RoutesMenu::updateList()
{
    disconnect(rtList, SIGNAL(currentRowChanged(int)), this, SLOT(listIndChanged(int)));
    rtList->clear();
    int i = 1;
    foreach( MapObj* tmp, *objects)
    {

        QListWidgetItem *curIt = new QListWidgetItem(tr("Маршрут №") + QString::number(i), rtList);
        switch( tmp->getAsseccory() )
        {
        case(ours):
            curIt->setBackgroundColor(QColor(137, 164, 254));
            break;
        case(alien):
            curIt->setBackgroundColor(QColor(255, 72, 77));
            break;
        }
        if( tmp == route )
        {
            curIt->setBackgroundColor(Qt::green);
        }
        i++;
    }
    connect(rtList, SIGNAL(currentRowChanged(int)), this, SLOT(listIndChanged(int)));
}

void RoutesMenu::changeCurRoute( MapObj* _route )
{
    route = _route;
}

void RoutesMenu::listIndChanged( int ind )
{
    QList<MapObj*>::iterator it = objects->begin();
    for( int i = 0; i < ind ; i++ )
    {
        it++;
    }
    route = (*it);
    updateList();
    emit curRouteChanged((*it));
}

void RoutesMenu::deleteCurRoute()
{
     QList<MapObj*>::iterator it = objects->begin();
     while( (*it) != route )
     {
         it++;
     }
     objects->erase(it);
     route = (*objects->begin());
     updateList();
     emit routeDeleted();
}

void RoutesMenu::paintEvent(QPaintEvent * event )
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

void RoutesMenu::setNewRouteButtonDisable()
{
    newRouteButton->setEnabled(false);
}

void RoutesMenu::setNewRouteButtonEnable()
{
    newRouteButton->setEnabled(true);
}

void RoutesMenu::checkButtons()
{
    if( objects->size() != NULL )
    {
        deleteRouteButton->setEnabled(true);
        editRouteButton->setEnabled(true);
    }
    else
    {
        deleteRouteButton->setEnabled(false);
        editRouteButton->setEnabled(false);
    }
}


void RoutesMenu::setObjects( QList<MapObj*> *_objects )
{
    objects = _objects;
}
