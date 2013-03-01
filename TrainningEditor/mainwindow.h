#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QTextCodec>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QtWidgets/QFileDialog>
#include <iostream>
#include "routesmenu.h"
#include "trainningscene.h"
#include "editroutesmenu.h"
#include "options.h"
#include "optionsmenu.h"
#include "QString"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    void createActions();
    void createMenus();
    void createContectMenu();
    void createToolBars();
    void createStatusBar();
    bool okToContinue();
    TrainningScene* scene;
    RoutesMenu* rtMenu;
    EditRoutesMenu* editMenu;
    QList<MapObj*> *objects;
    Options* opt;
    OptionsMenu* optMenu;
    QMenu *fileMenu;
    QAction *newAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *openAction;
    QAction *exitAction;
    QString fileName;
private slots:
    void routeEditing();
    void finishEdit();
    bool save();
    bool saveAs();
    void open();
    void newFile();
    void readDataBaseElement( QXmlStreamReader &xmlReader );
    void readRouteElement(QXmlStreamReader &xmlReader , MapObj *obj);

public:
    MainWindow(QWidget *parent = 0);
    bool saveFile(const QString &fileName);
    ~MainWindow();
protected:
    void resizeEvent(QResizeEvent *);
    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
