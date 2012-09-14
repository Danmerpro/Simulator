#include "mainwindow.h"
#include "mapview.h"
#include "mapscene.h"
#include "mapitem.h"
#include "radarline.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createActions();
    createMenus();
    createContectMenu();
    createToolBars();
    createStatusBar();

    mscene = new MapScene();
    mview = new MapView();
    mview->setBaseSize(500,500);
    mview->setScene(mscene);
    mview->setRenderHint(QPainter::Antialiasing);
    setCentralWidget(mview);
    setWindowTitle(tr("Map Demo"));
    setWindowIcon(QIcon(":/Icon/RLSIcon.png"));
    QGraphicsEllipseItem *IKO = new QGraphicsEllipseItem(QRectF(0,0,600,600));
    IKO->setPen(QPen(Qt::green));
    IKO->setBrush(QBrush(Qt::black));
    mscene->addItem(IKO);
    rl = new RadarLine();
    mscene->addItem(rl);
    this->adjustSize();
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::createActions()
{
    OpenMapAction = new QAction( tr("Open map"), this );
    OpenMapAction->setIcon(QIcon(":/Icon/map.png"));
    OpenMapAction->setStatusTip(tr("Open map"));
    connect(OpenMapAction,SIGNAL(triggered()),this,SLOT(openMap()));

    dragMapMode = new QAction( tr("Drag mode"), this );
    dragMapMode->setStatusTip(tr("Activate drag mode"));
    dragMapMode->setIcon(QIcon(":/Icon/cursor_drag_hand.png"));
    dragMapMode->setCheckable(true);
    connect(dragMapMode,SIGNAL(triggered()),this,SLOT(dragMapModeOn()));

    clickMapMode = new QAction( tr("Click mode"), this );
    clickMapMode->setStatusTip(tr("Activate Click mode"));
    clickMapMode->setIcon(QIcon(":/Icon/cursor_arrow.png"));
    clickMapMode->setCheckable(true);
    connect(clickMapMode,SIGNAL(triggered()),this,SLOT(clickMapModeOn()));

    mapModes = new QActionGroup(this);
    mapModes->addAction(dragMapMode);
    mapModes->addAction(clickMapMode);
    mapModes->setExclusive(true);
}
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(OpenMapAction);
    mapMenu = menuBar()->addMenu(tr("Map"));
    mapMenu->addAction(dragMapMode);
    mapMenu->addAction(clickMapMode);
    mapMenu->setEnabled(false);
}

void MainWindow::createContectMenu()
{

}

void MainWindow::createToolBars()
{
    mapToolBar = addToolBar(tr("Map"));
    mapToolBar->addAction(dragMapMode);
    mapToolBar->addAction(clickMapMode);
    mapToolBar->setEnabled(false);
}

void MainWindow::createStatusBar()
{
    statusBar();
}

void MainWindow::openMap()
{
    QString mapFileName = QFileDialog::getOpenFileName(this, tr("Open map"),".", tr("map files (*.jpg *.gif)"));
    trainningMap = new QPixmap(mapFileName);
    map = new MapItem(*trainningMap);
    mscene->addItem(map);
    mapMenu->setEnabled(true);
    mapToolBar->setEnabled(true);
    dragMapMode->setChecked(true);
    mview->setDragMode(QGraphicsView::ScrollHandDrag);
}

void MainWindow::dragMapModeOn()
{
    mview->setDragMode(QGraphicsView::ScrollHandDrag);
}

void MainWindow::clickMapModeOn()
{
    mview->setDragMode(QGraphicsView::NoDrag);
}

