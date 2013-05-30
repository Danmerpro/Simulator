#ifndef CONNECTIONMENU_H
#define CONNECTIONMENU_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QtWidgets/QGroupBox>
#include <QFont>
#include <QFormLayout>
#include <QRegExpValidator>
#include <QPalette>
#include <QString>

enum CLIENT_STATUS{ NOT_CONNECTED, WAIT_FOR_START, TRAININNG, PAUSE };

class ConnectionMenu : public QWidget
{
    Q_OBJECT
private:
    QLineEdit* ipLineEdit;
    QLineEdit* portEdit;
    QPushButton* connectBut;
    QGroupBox* netSets;
    QLabel* statusLabel;
    QString* ipAdress;
    int* port;

public:
    explicit ConnectionMenu(QString* _ipAdress,int* port, QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    
signals:
    void conButClicked();
    
public slots:
    void ipChanged(QString text);
    void portChanged(QString text);
    void statusChanged(CLIENT_STATUS st);
    
};

#endif // CONNECTIONMENU_H
