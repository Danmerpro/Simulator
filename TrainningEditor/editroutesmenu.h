#ifndef EDITROUTESMENU_H
#define EDITROUTESMENU_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QGroupBox>
#include <QFont>
#include <QLabel>
#include <QTimeEdit>
#include "mapobj.h"

class EditRoutesMenu : public QWidget
{
    Q_OBJECT
private:
    QGroupBox* routeGropBox;
    QGroupBox* pointGropBox;
    QPushButton* nextButton;
    QPushButton* prevButton;
    QPushButton* readyButton;
    QComboBox* accessoryCombo;
    QDoubleSpinBox* speedBox;
    QDoubleSpinBox* altBox;
    QLineEdit* curPoint;
    QTimeEdit* startTime;
    MapObj* route;
    QList<RoutePoint>::iterator it;
    int pointNum;
public:
    explicit EditRoutesMenu(QWidget *parent = 0);
    void setEditingRoute();
    void paintEvent(QPaintEvent * event );
    
signals:
    void readyButtonPushed();
    void activePointChanged();
    void updateScene();
    
public slots:
    void pointSpeedChanged( );
    void pointAltChanged( );
    void toNextPoint();
    void toPrevPoint();
    void toNumPoint( QString _num );
    void changeActivePoint();
    void accessoryChanged(int index);
    void startTimeChanged();
    void changeCurRoute( MapObj* _route );
    
};

#endif // EDITROUTESMENU_H
