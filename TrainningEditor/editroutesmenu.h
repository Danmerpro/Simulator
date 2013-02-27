#ifndef EDITROUTESMENU_H
#define EDITROUTESMENU_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMessageBox>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QtWidgets/QGroupBox>
#include <QFont>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTimeEdit>
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
