#ifndef EDITROUTESMENU_H
#define EDITROUTESMENU_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QFormLayout>
#include <QLineEdit>
#include "mapobj.h"

class EditRoutesMenu : public QWidget
{
    Q_OBJECT
private:
    QLabel* title;
    QPushButton* nextButton;
    QPushButton* prevButton;
    QPushButton* readyButton;
    QComboBox* accessoryCombo;
    QSpinBox* speedBox;
    QSpinBox* altBox;
    QLineEdit* curPoint;
    MapObj* route;
    QList<RoutePoint>::iterator it;
    int pointNum;
public:
    explicit EditRoutesMenu(QWidget *parent = 0);
    void setEditingRoute( MapObj* _route );
    
signals:
    void readyButtonPushed();
    
public slots:
    void pointSpeedChanged( );
    void pointAltChanged( );
    
};

#endif // EDITROUTESMENU_H
