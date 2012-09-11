#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QActionGroup>
#include <QToolBar>
#include "mapview.h"
#include "mapscene.h"
#include "mapitem.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
private slots:
    void openMap();
    void dragMapModeOn();
    void clickMapModeOn();

private:
    void createActions();
    void createMenus();
    void createContectMenu();
    void createToolBars();
    void createStatusBar();

    MapScene *mscene;
    MapView *mview;
    QPixmap *trainningMap;
    MapItem *map;

    QAction *OpenMapAction;
    QAction *dragMapMode;
    QAction *clickMapMode;

    QActionGroup *mapModes;

    QToolBar *mapToolBar;

    QMenu *fileMenu;
    QMenu *mapMenu;

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
