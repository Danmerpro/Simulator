#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QAction>
#include <QMenu>
#include "mapview.h"
#include "mapscene.h"
#include "mapitem.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    void createActions();
    void createMenus();
    void createContectMenu();
    void createToolBars();


    MapScene *mscene;
    MapView *mview;
    QPixmap *trainningMap;
    MapItem *map;

    QAction* OpenMapAction;
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
