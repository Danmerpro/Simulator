#ifndef INDICATOR_H
#define INDICATOR_H

#include <QtWidgets/QWidget>
#include <QTimer>

class Indicator : public QWidget
{
    Q_OBJECT

    QTimer timer;
    double angle;
    static const int framesPerSec = 25;
    static const int scanInSecs = 10;
public:
    explicit Indicator(QWidget *parent = 0);
    
private slots:
    void scan();
    
private:
    void drawCircle(QPixmap &_m, QPoint _center, int _rad);
    void drawLine(QPixmap &_m, QPoint _center, int _rad);
protected:
    void paintEvent(QPaintEvent *e);
    
};

#endif // INDICATOR_H
