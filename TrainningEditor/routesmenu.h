#ifndef ROUTESMENU_H
#define ROUTESMENU_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include "routeslist.h"
#include "mapobj.h"

class RoutesMenu : public QWidget
{
    Q_OBJECT

private:
    QPushButton* newRouteButton;
    QPushButton* deleteRouteButton;
    QPushButton* editRouteButton;
    QPushButton* startButton;
    RoutesList* rtList;
    QList<MapObj*> *objects;
    MapObj* route;
public:
    explicit RoutesMenu(QList<MapObj*> *_objects, QWidget *parent = 0);
    QPushButton* getNewRouteButton();
    
signals:
    
public slots:
    void updateList();
    void changeCurRoute( MapObj* _route );
    
};

#endif // ROUTESMENU_H
