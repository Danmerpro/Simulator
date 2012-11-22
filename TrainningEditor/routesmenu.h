#ifndef ROUTESMENU_H
#define ROUTESMENU_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include "routestable.h"

class RoutesMenu : public QWidget
{
    Q_OBJECT

private:
    QPushButton* newRouteButton;
    QPushButton* deleteRouteButton;
    QPushButton* editRouteButton;
    QPushButton* startButton;
    RoutesTable* rtTable;
public:
    explicit RoutesMenu(QWidget *parent = 0);
    
signals:
    
public slots:
    
};

#endif // ROUTESMENU_H
