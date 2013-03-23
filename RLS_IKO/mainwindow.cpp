#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout* mainLayout = new QHBoxLayout();
    scrollSim = new QScrollArea();
    open();
    sim = new Simulation( objects );
    scrollSim->setWidget(sim);
    mainLayout->addWidget(scrollSim);
    centralWidget->setLayout( mainLayout );
    this->setCentralWidget( centralWidget );
    sim->start();
}

MainWindow::~MainWindow()
{
    
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
