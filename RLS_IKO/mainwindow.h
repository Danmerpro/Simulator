#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QXmlStreamReader>
#include <QString>
#include <QtWidgets/QFileDialog>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QMessageBox>
#include <stdio.h>
#include <windows.h>
#include "simulation.h"
#include "pointslist.h"
#include "connectionmenu.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QString fileName;
    Simulation* sim;
    QList<MapObj*> *objects;
    QScrollArea* scrollSim;
    QTcpSocket tcpClient;
    FILE* ftrans;
    PointsList* pList;
    ConnectionMenu* conMenu;
    QString* ipAdress;
    int port;
    CLIENT_STATUS status;

private slots:
    void open();
    void readDataBaseElement( QXmlStreamReader &xmlReader );
    void readRouteElement(QXmlStreamReader &xmlReader , MapObj *obj);
    void connectToServer();
    void displayError(QAbstractSocket::SocketError socketError);
    void readData();
    void clientConnected();
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *);

signals:
    void statusChanged(CLIENT_STATUS st);
};

#endif // MAINWINDOW_H
