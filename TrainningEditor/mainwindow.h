#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextCodec>
#include "routesmenu.h"
#include "trainningscene.h"
#include "editroutesmenu.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    void createActions();
    void createMenus();
    void createContectMenu();
    void createToolBars();
    void createStatusBar();
    TrainningScene* scene;
    RoutesMenu* rtMenu;
    EditRoutesMenu* editMenu;
    QList<MapObj*> *objects;
private slots:
    void routeEditing( MapObj* _route );
    void finishEdit();

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void resizeEvent(QResizeEvent *);
};

#endif // MAINWINDOW_H
