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
    QVBoxLayout* menuLayout = new QVBoxLayout();
    QVBoxLayout* sceneLayout = new QVBoxLayout();
    scene = new TrainningScene( this );
    rtMenu = new RoutesMenu (this);
    sceneLayout->addWidget(scene);
    menuLayout->addWidget(rtMenu);
    mainLayout->addLayout(sceneLayout);
    mainLayout->addLayout(menuLayout);
    mainLayout->setStretch(0,4);
    mainLayout->setStretch(0,1);
    centralWidget->setLayout( mainLayout );
    this->setCentralWidget( centralWidget );
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


