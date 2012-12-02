#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createActions();
    createMenus();
    createContectMenu();
    createToolBars();
    createStatusBar();
    setWindowTitle(tr("Training Editor"));
    setWindowIcon(QIcon(":/Icon/RLSIcon.png"));
    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout* mainLayout = new QHBoxLayout();
    QHBoxLayout* menuLayout = new QHBoxLayout();
    QVBoxLayout* sceneLayout = new QVBoxLayout();
    objects = new QList<MapObj*>();
    scene = new TrainningScene( objects, this );
    rtMenu = new RoutesMenu (objects,this);
    editMenu = new EditRoutesMenu(this);
    sceneLayout->addWidget(scene);
    menuLayout->addWidget(rtMenu);
    menuLayout->addWidget(editMenu);
    mainLayout->addLayout(sceneLayout);
    mainLayout->addLayout(menuLayout);
    mainLayout->setStretch(0,8);
    mainLayout->setStretch(1,1);
    centralWidget->setLayout( mainLayout );
    this->setCentralWidget( centralWidget );
    connect(rtMenu->getNewRouteButton(),SIGNAL(clicked()),scene,SLOT(drawlingModeOn()));
    connect(scene, SIGNAL(routeEditing()), this, SLOT(routeEditing()));
    connect(scene, SIGNAL(curRouteChanged(MapObj*)), rtMenu, SLOT(changeCurRoute(MapObj*)));
    connect(scene, SIGNAL(curRouteChanged(MapObj*)), editMenu, SLOT(changeCurRoute(MapObj*)));
    connect(editMenu, SIGNAL(readyButtonPushed()), this, SLOT(finishEdit()));
    connect(editMenu, SIGNAL(readyButtonPushed()), rtMenu, SLOT(updateList()));
    connect(rtMenu->getNewRouteButton(),SIGNAL(clicked()), rtMenu, SLOT(updateList()));
    connect(editMenu, SIGNAL(readyButtonPushed()), scene, SLOT(finishEdit()));
    connect(editMenu, SIGNAL(activePointChanged()),scene, SLOT(changeAvtivePoint()));
    connect(scene, SIGNAL(activePointChanged()),editMenu, SLOT(changeActivePoint()));
    editMenu->hide();
 //   this->showFullScreen();
}

void MainWindow::resizeEvent(QResizeEvent *)
{
//    scene->setFixedSize(this->width(),this->height());
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
    statusBar();
}

void MainWindow::routeEditing()
{
    editMenu->show();
    rtMenu->hide();
    editMenu->setEditingRoute();
}

void MainWindow::finishEdit()
{
    editMenu->hide();
    rtMenu->show();
}


