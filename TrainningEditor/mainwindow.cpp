#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
    createActions();
    createMenus();
    createContectMenu();
    createToolBars();
    createStatusBar();
    fileName = "";
    setWindowTitle(tr("Training Editor - Новая тренировка"));
    setWindowIcon(QIcon(":/Icon/RLSIcon.png"));
    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout* mainLayout = new QHBoxLayout();
    QVBoxLayout* rigthLayout = new QVBoxLayout();
    QHBoxLayout* menuLayout = new QHBoxLayout();
    sceneLayout = new QVBoxLayout();
    sim = NULL;
    opt = new Options();
    optMenu = new OptionsMenu( opt, this );
    objects = new QList<MapObj*>();
    scene = new TrainningScene( objects, opt, this );
    rtMenu = new RoutesMenu (objects,this);
    editMenu = new EditRoutesMenu(this);
    scrollScene = new QScrollArea();
    scrollSim = new QScrollArea();
    scrollScene->setBackgroundRole(QPalette::Midlight);
    scrollScene->setWidget(scene);
    sceneLayout->addWidget(scrollScene);
    sceneLayout->addWidget(scrollSim);
    scrollSim->hide();
    menuLayout->addWidget(rtMenu);
    menuLayout->addWidget(editMenu);
    rigthLayout->addLayout(menuLayout);
    rigthLayout->addWidget(optMenu);
    mainLayout->addLayout(sceneLayout);
    mainLayout->addLayout(rigthLayout);
    mainLayout->setStretch(0,8);
    mainLayout->setStretch(1,1);
    centralWidget->setLayout( mainLayout );
    this->setCentralWidget( centralWidget );

    connect(scene, SIGNAL(routeEditing()), this, SLOT(routeEditing()));
    connect(scene, SIGNAL(curRouteChanged(MapObj*)), rtMenu, SLOT(changeCurRoute(MapObj*)));
    connect(scene, SIGNAL(curRouteChanged(MapObj*)), editMenu, SLOT(changeCurRoute(MapObj*)));
    connect(scene, SIGNAL(activePointChanged()),editMenu, SLOT(changeActivePoint()));

    connect(rtMenu->getNewRouteButton(),SIGNAL(clicked()),scene,SLOT(drawlingModeOn()));
    connect(rtMenu->getNewRouteButton(),SIGNAL(clicked()), rtMenu, SLOT(updateList()));
    connect(rtMenu, SIGNAL(curRouteChanged(MapObj*)),scene, SLOT(changeCurRoute(MapObj*)));
    connect(rtMenu, SIGNAL(curRouteChanged(MapObj*)),editMenu, SLOT(changeCurRoute(MapObj*)));
    connect(rtMenu, SIGNAL(routeDeleted()),scene, SLOT(deleteRoute()));
    connect(rtMenu, SIGNAL(routeDeleted()),rtMenu, SLOT(checkButtons()));
    connect(rtMenu, SIGNAL(editCurRoute()),scene, SLOT(procesingRoute()));
    connect(rtMenu, SIGNAL(trModified()),this, SLOT(trainingModified()));
    connect(rtMenu, SIGNAL(startTraining()),this, SLOT(startTraining()));

    connect(editMenu, SIGNAL(readyButtonPushed()), rtMenu, SLOT(checkButtons()));
    connect(editMenu, SIGNAL(readyButtonPushed()), scene, SLOT(finishEdit()));
    connect(editMenu, SIGNAL(activePointChanged()),scene, SLOT(changeAvtivePoint()));
    connect(editMenu, SIGNAL(readyButtonPushed()), this, SLOT(finishEdit()));
    connect(editMenu, SIGNAL(readyButtonPushed()), rtMenu, SLOT(updateList()));
    connect(editMenu, SIGNAL(readyButtonPushed()), rtMenu, SLOT(setNewRouteButtonEnable()));
    connect(editMenu, SIGNAL(updateScene()), scene, SLOT(update()));
    connect(optMenu, SIGNAL(updateScene()), scene, SLOT(update()));

    editMenu->hide();
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
    newAction = saveAction = new QAction(tr("Создать"),this);
    connect(saveAction, SIGNAL(triggered()), this, SLOT(newFile()));

    saveAction = new QAction(tr("Сохранить"),this);
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAction = new QAction(tr("Сохранить как"),this);
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

    openAction = new QAction(tr("Открыть"),this);
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

    exitAction = new QAction(tr("Выход"),this);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

}
void MainWindow::createMenus()
{
    fileMenu = new QMenu(tr("Файл"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(exitAction);
    this->menuBar()->addMenu(fileMenu);

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

void MainWindow::newFile()
{
    if(okToContinue())
    {
        fileName = "";
        if( objects )
            delete objects;
        objects = new QList<MapObj*>();
        setWindowTitle(tr("Training Editor - Новая тренировка"));
        this->setWindowModified(false);
        systemReinit();
        statusBar()->showMessage(tr("Новый тренаж создан"), 2000);
    }
}

bool MainWindow::save()
{
    if(fileName.isEmpty())
    {
        return saveAs();
    }
    else
    {
        return saveFile(fileName);
    }
}

bool MainWindow::saveAs()
{
    fileName = QFileDialog::getSaveFileName(this,tr("Сохранить тренажную обстановку..."),".",tr("Routes Data Base files (*.xml)"));
    if(fileName.isEmpty())
        return false;
    return saveFile(fileName);
}

void MainWindow::open()
{
    fileName = QFileDialog::getOpenFileName(this,tr("Открыть тренажную обстановку..."),".",tr("Routes Data Base files (*.xml)"));
    QFile* inputXML = new QFile(fileName);
    inputXML->open(QIODevice::ReadOnly);
    QXmlStreamReader xmlReader(inputXML);
    objects = new QList<MapObj*>();
    xmlReader.readNext();
    while(!xmlReader.atEnd())
    {
        if(xmlReader.isStartElement())
        {
            if(xmlReader.name() == "routes_data_base")
            {
                readDataBaseElement(xmlReader);
            }
            else
            {
                xmlReader.readNext();
            }
        }
        else
        {
            xmlReader.readNext();
        }

    }
    inputXML->close();
    if(xmlReader.hasError())
    {
        std::cerr<<"Error: failed to parse file"<<qPrintable(fileName)<<": "<<qPrintable(xmlReader.errorString())<< std::endl;
    }
    setWindowTitle(fileName);
    systemReinit();
    statusBar()->showMessage(tr("Файл тренажа загружен"), 2000);
}


void MainWindow::readDataBaseElement(QXmlStreamReader &xmlReader)
{
    MapObj* obj;
    xmlReader.readNext();
    while(!xmlReader.atEnd())
    {
        if(xmlReader.isEndElement())
        {
            if(xmlReader.name() == "routes_data_base")
            {
                xmlReader.readNext();
                break;
            }
        }
        if(xmlReader.isStartElement())
        {
            if(xmlReader.name() == "route")
            {
                obj = new MapObj();
                QString asseccory = xmlReader.attributes().value("asseccory").toString();
                if( asseccory == "ours" )
                    obj->setAsseccory(ours);
                if( asseccory == "alien" )
                    obj->setAsseccory(alien);
                obj->setStartTime(QTime::fromString(xmlReader.attributes().value("start_time").toString()));
                readRouteElement( xmlReader, obj );
            }
            else
            {
                 xmlReader.readNext();
            }
        }
        else
        {
            xmlReader.readNext();
        }
    }
}

void  MainWindow::readRouteElement(QXmlStreamReader &xmlReader, MapObj* obj )
{
    xmlReader.readNext();
    RoutePoint* p;
    while(!xmlReader.atEnd())
    {
        if(xmlReader.isStartElement())
        {
            if(xmlReader.name() == "point")
            {
                p = new RoutePoint();
                p->setAlt( xmlReader.attributes().value("alt").toString().toDouble() );
                p->setSpeed( xmlReader.attributes().value("speed").toString().toDouble() );
                p->setX( xmlReader.attributes().value("x").toString().toDouble());
                p->setY( xmlReader.attributes().value("y").toString().toDouble());
            }
        }
        if(xmlReader.isEndElement())
        {
            if(xmlReader.name() == "point")
            {
               obj->appendPoint( *p );
               xmlReader.readNext();
            }
            if(xmlReader.name() == "route")
            {
                objects->append( obj );
                xmlReader.readNext();
                break;
            }
        }
        xmlReader.readNext();
    }
}


void MainWindow::closeEvent(QCloseEvent* event)
{
    if(okToContinue())
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

bool MainWindow::okToContinue()
{
    if(isWindowModified())
    {
        int r = QMessageBox::warning(this,tr("Редактор тренажа"),tr("Тренажная обстановка была изменена\nСохранить изменения?"),QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if(r == QMessageBox::Yes)
        {
            return save();
        }
        else
        {
            if(r == QMessageBox::Cancel)
                return false;
        }
    }
    return true;
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile* outputXML = new QFile(fileName);
    outputXML->open(QIODevice::WriteOnly);
    QXmlStreamWriter xmlWriter(outputXML);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("routes_data_base");
    QList<MapObj*>::iterator it = objects->begin();
    for( it ; it != objects->end() ; it++ )
    {
        xmlWriter.writeStartElement("route");
        if( (*it)->getAsseccory() == ours)
            xmlWriter.writeAttribute("asseccory", "ours");
        if( (*it)->getAsseccory() == alien)
            xmlWriter.writeAttribute("asseccory", "alien");
        xmlWriter.writeAttribute("start_time", (*it)->getStartTime().toString());
        QList<RoutePoint>::Iterator itP = (*it)-> getPoints()->begin();
        for( itP ; itP != (*it)-> getPoints()->end() ; itP++ )
        {
            xmlWriter.writeStartElement("point");
            xmlWriter.writeAttribute("speed", QString::number((*itP).getSpeed(),'f'));
            xmlWriter.writeAttribute("alt", QString::number((*itP).getAlt(),'f'));
            xmlWriter.writeAttribute("x", QString::number((*itP).x(),'f'));
            xmlWriter.writeAttribute("y", QString::number((*itP).y(),'f'));
            xmlWriter.writeEndElement();
        }
        xmlWriter.writeEndElement();
    }
    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();
    outputXML->close();
    this->setWindowModified(false);
    setWindowTitle(tr("Training Editor - ") + fileName );
     statusBar()->showMessage(tr("Тренаж сохранен"), 2000);
    return true;
}

void MainWindow::systemReinit()
{
    scene->setObjects( objects );
    rtMenu->setObjects( objects );
    scene->update();
    scene->changeCurRoute( NULL );
    rtMenu->updateList();
    rtMenu->checkButtons();
}

void MainWindow::trainingModified()
{
    if( fileName == NULL )
        setWindowTitle(tr("Training Editor - Новая тренировка[*]"));
    else
        setWindowTitle(tr("Training Editor - ") + fileName + tr("[*]"));
    this->setWindowModified(true);
}

void  MainWindow::startTraining()
{
    scrollScene->hide();
    if( sim != NULL)
        delete sim;
    sim = new Simulation( objects );
    scrollSim->setWidget(sim);
    scrollSim->show();
    sim->start();
}


