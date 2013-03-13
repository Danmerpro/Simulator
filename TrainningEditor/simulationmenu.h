#ifndef SIMULATIONMENU_H
#define SIMULATIONMENU_H

#include <QWidget>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QLabel>
#include <QTimeEdit>

class SimulationMenu : public QWidget
{
    Q_OBJECT
private:
    QPushButton* play;
    QPushButton* pause;
    QPushButton* stop;
    QPushButton* endTraining;
    QTimeEdit* timeElapsed;
public:
    explicit SimulationMenu(QWidget *parent = 0);
    void paintEvent(QPaintEvent * event );
    
signals:
    void playClicked();
    void pauseClicked();
    void stopClicked();
    
public slots:
    void updateTimeElapsed();
    void zeroTime();
    
};

#endif // SIMULATIONMENU_H
