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
    scene = new TrainningScene( this );
    rtMenu = new RoutesMenu(this);
    editMenu = new EditRoutesMenu(this);
    sceneLayout->addWidget(scene);
    menuLayout->addWidget(rtMenu);
    menuLayout->addWidget(editMenu);
    mainLayout->addLayout(sceneLayout);
    mainLayout->addLayout(menuLayout);
    mainLayout->setStretch(0,4);
    centralWidget->setLayout( mainLayout );
    this->setCentralWidget( centralWidget );
    connect(rtMenu->getNewRouteButton(),SIGNAL(clicked()),scene,SLOT(drawlingModeOn()));
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


