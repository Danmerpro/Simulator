#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ipAdress = new QString("0.0.0.0");
    port = 0;
    status = NOT_CONNECTED;

    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout* mainLayout = new QHBoxLayout();
    QVBoxLayout* menuesLayout = new QVBoxLayout();

    pList = new PointsList();
    conMenu = new ConnectionMenu(ipAdress, &port);

    menuesLayout->addWidget(pList);
    menuesLayout->addWidget(conMenu);

    scrollSim = new QScrollArea();
    objects = new QList<MapObj*>();
    sim = new Simulation( objects );
    scrollSim->setWidget(sim);
    mainLayout->addWidget(scrollSim);
    mainLayout->addLayout(menuesLayout);
    mainLayout->setStretch(0,8);
    mainLayout->setStretch(1,1);
    centralWidget->setLayout( mainLayout );
    this->setCentralWidget( centralWidget );

    connect(&tcpClient, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));
    connect(&tcpClient, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(&tcpClient, SIGNAL(connected()), this, SLOT(clientConnected()));
    connect(conMenu, SIGNAL(conButClicked()), SLOT(connectToServer()));
    connect(this, SIGNAL(statusChanged(CLIENT_STATUS)), conMenu, SLOT(statusChanged(CLIENT_STATUS)));
}

MainWindow::~MainWindow()
{
    
}

void  MainWindow::connectToServer()
{
    tcpClient.connectToHost(*ipAdress, port);
}

void MainWindow::displayError(QAbstractSocket::SocketError socketError)
{
    if (socketError == QTcpSocket::RemoteHostClosedError)
        return;

    QMessageBox::information(this, tr("Network error"),
                             tr("The following error occurred: %1.")
                             .arg(tcpClient.errorString()));

    tcpClient.close();
}

void MainWindow::readData()
{
    int bytesInBuf;
    char* data = new char[64*1024];
    while(1)
    {
        if( tcpClient.bytesAvailable() >= 64 * 1024 )
        {
            tcpClient.read(data,64 * 1024);

            if(data[0] == 1) //начало передачи файла тренажа
            {
                ftrans = fopen("training.xml", "wb");
            }

            if(data[0] == 2) //передача данных о тренаже
            {
                bytesInBuf = 0;
                for(int i = 1 ; i < 5 ; i++)
                {
                    bytesInBuf = bytesInBuf << 8;
                    bytesInBuf += (uchar)data[i];
                }
                fwrite(data + 5, sizeof(char), bytesInBuf, ftrans);
            }

            if(data[0] == 3) //конец передачи данных о тренаже
            {
                fclose(ftrans);
                open();
                sim->setObjects(objects);
            }
            if(data[0] == 4) //команда запустить тренеровку
            {
                sim->start();
                status = TRAININNG;
                emit statusChanged(TRAININNG);
            }
            if(data[0] == 5) //команда приостановить тренеровку
            {
                sim->pause();
                status = PAUSE;
                emit statusChanged(PAUSE);
            }
            if(data[0] == 6) //команда отсановить тренорвку
            {
                sim->stop();
                status = WAIT_FOR_START;
                emit statusChanged(WAIT_FOR_START);
            }
        }
        else
        {
            break;
        }
    }
}

void MainWindow::open()
{
    fileName = "training.xml";
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

void  MainWindow::closeEvent(QCloseEvent *)
{
    tcpClient.disconnectFromHost();
}

void MainWindow::clientConnected()
{
    status = WAIT_FOR_START;
    emit statusChanged(WAIT_FOR_START);
}

