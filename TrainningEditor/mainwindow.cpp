#include "mainwindow.h"
#include "mapview.h"
#include "mapscene.h"
#include "mapitem.h"
#include "radarline.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    void createActions();
    void createMenus();
    void createContectMenu();
    void createToolBars();
    void createStatusBar();

    trainningMap = new QPixmap("lenoblast-obz.jpg");
    mscene = new MapScene();
    map = new MapItem(*trainningMap);
    mscene->addItem(map);
    mview = new MapView();
    mview->setDragMode(QGraphicsView::ScrollHandDrag);
    mview->setScene(mscene);
    setCentralWidget(mview);
    setWindowTitle(tr("Map Demo"));
    setWindowIcon(":/Icon/RLSIcon.png");
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::createActions()
{

}
void MainWindow::createMenus()
{

}

void MainWindow::createContectMenu()
{

}

void MainWindow::createToolBars()
{

}

void MainWindow::createStatusBar()
{

}

