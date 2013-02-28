#ifndef OPTIONSMENU_H
#define OPTIONSMENU_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QtWidgets/QGroupBox>
#include <QFont>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include "options.h"

class OptionsMenu : public QWidget
{
    Q_OBJECT
private:
    Options* opt;

    QCheckBox* speedOnMap;
    QCheckBox* altOnMap;
    QCheckBox* startTimeOnMap;

    QComboBox* speedUnitsCombo;
    QComboBox* altUnitsCombo;


    QGroupBox* onMapGroupBox;
    QGroupBox* unitGroupBox;

public:
    explicit OptionsMenu(Options* _opt, QWidget *parent = 0);
    void paintEvent(QPaintEvent * event );

protected slots:
    void speedOnMapChanged( bool val);
    void altOnMapChanged( bool val);
    void startTimeOnMapChanged( bool val);
    
signals:
    void updateScene();
    
public slots:
    
};

#endif // OPTIONSMENU_H
