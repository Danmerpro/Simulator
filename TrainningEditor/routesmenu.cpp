#include "routesmenu.h"

RoutesMenu::RoutesMenu(QList<MapObj*> *_objects, QWidget *parent) :
    QWidget(parent)
{
    objects = _objects;
    newRouteButton = new QPushButton( this );
    newRouteButton->setText( tr("Новый маршрут") );
    deleteRouteButton = new QPushButton( this );
    deleteRouteButton->setText( tr("Удалить") );
    editRouteButton = new QPushButton( this );
    editRouteButton->setText( tr("Редактировать") );
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
    QHBoxLayout *hLayout2 = new QHBoxLayout();
    hLayout2->addStretch();
    hLayout2->addWidget(startButton);
    VLayout1->addLayout(hLayout2);
    VLayout1->addStretch();
    this->setLayout( VLayout1 );
    this->setMinimumWidth(300);
    updateList();
}

QPushButton* RoutesMenu::getNewRouteButton()
{
    return newRouteButton;
}

void RoutesMenu::updateList()
{
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
}

void RoutesMenu::changeCurRoute( MapObj* _route )
{
    route = _route;
}
