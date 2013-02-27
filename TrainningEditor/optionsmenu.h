#ifndef OPTIONSMENU_H
#define OPTIONSMENU_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QGroupBox>
#include <QFont>
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
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
