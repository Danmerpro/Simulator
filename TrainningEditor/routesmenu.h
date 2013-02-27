#ifndef ROUTESMENU_H
#define ROUTESMENU_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QPainter>
#include <QPen>
#include <QBrush>
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
    void paintEvent(QPaintEvent * event );
    void setObjects( QList<MapObj*> *_objects );
    
signals:
    void curRouteChanged( MapObj* _route );
    void routeDeleted();
    void editCurRoute();
    
public slots:
    void updateList();
    void changeCurRoute( MapObj* _route );
    void deleteCurRoute();
    void listIndChanged( int ind );
    void setNewRouteButtonDisable();
    void setNewRouteButtonEnable();
    void checkButtons();
    
};

#endif // ROUTESMENU_H
