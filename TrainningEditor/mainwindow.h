#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QtWidgets/QFileDialog>
#include <QScrollArea>
#include <iostream>
#include <QString>
#include "routesmenu.h"
#include "trainningscene.h"
#include "editroutesmenu.h"
#include "simulationmenu.h"
#include "options.h"
#include "optionsmenu.h"
#include "simulation.h"

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
    Simulation* sim;
    RoutesMenu* rtMenu;
    EditRoutesMenu* editMenu;
    SimulationMenu* simMenu;
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
    QVBoxLayout* sceneLayout;
    QScrollArea* scrollScene;
    QScrollArea* scrollSim;

private slots:
    void routeEditing();
    void finishEdit();
    bool save();
    bool saveAs();
    void open();
    void newFile();
    void readDataBaseElement( QXmlStreamReader &xmlReader );
    void readRouteElement(QXmlStreamReader &xmlReader , MapObj *obj);
    void systemReinit();
    void trainingModified();
    void startTraining();
    void exitToEditor();

public:
    MainWindow(QWidget *parent = 0);
    bool saveFile(const QString &fileName);
    ~MainWindow();
protected:
    void resizeEvent(QResizeEvent *);
    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
