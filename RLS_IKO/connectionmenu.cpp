#include "connectionmenu.h"

ConnectionMenu::ConnectionMenu(QString* _ipAdress, int* _port, QWidget *parent) :
    QWidget(parent)
{
    ipAdress = _ipAdress;
    port = _port;

    QFont* font = new QFont();

    QVBoxLayout* mainLayout = new QVBoxLayout();

    netSets = new QGroupBox(tr("Настройки подключения"));
    font->setBold(true);
    netSets->setFont(*font);

    QFormLayout* setsLayout = new QFormLayout();
    font->setBold(false);
    QLabel* ipLabel = new QLabel(tr("IP-адрес сервера:"));
    ipLabel->setFont(*font);
    ipLineEdit = new QLineEdit();
    ipLineEdit->setFont(*font);
    QRegExp ipRegExp( "\\b(([01]?\\d?\\d|2[0-4]\\d|25[0-5])\\.){3}([01]?\\d?\\d|2[0-4]\\d|25[0-5])\\b" );
    ipLineEdit->setValidator(new QRegExpValidator(ipRegExp,this));
    ipLineEdit->setInputMask(QString("000.000.000.000;_"));
    ipLineEdit->adjustSize();
    ipLineEdit->setFixedWidth(90);
    setsLayout->addRow(ipLabel, ipLineEdit);
    QLabel* portLabel = new QLabel(tr("Порт:"));
    portLabel->setFont(*font);
    portEdit = new QLineEdit();
    portEdit->setText(QString::number(*port));
    portEdit->setFont(*font);
    portEdit->setFixedWidth(90);
    setsLayout->addRow(portLabel, portEdit);
    netSets->setLayout(setsLayout);

    QHBoxLayout* butLayout = new QHBoxLayout();
    connectBut = new QPushButton(tr("Подключиться"));
    butLayout->addStretch();
    butLayout->addWidget(connectBut);
    statusLabel = new QLabel(tr("НЕ ПОДКЛЮЧЕН"));
    QPalette* palette = new QPalette();
    palette->setColor(QPalette::Background, Qt::red);
    statusLabel->setAutoFillBackground(true);
    statusLabel->setPalette(*palette);
    font->setBold(true);
    statusLabel->setFont(*font);
    statusLabel->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    mainLayout->addWidget(netSets);
    mainLayout->addWidget(statusLabel);
    mainLayout->addLayout(butLayout);
   // mainLayout->setStretch(0,1);
  //  mainLayout->setStretch(1,1);

    this->setLayout(mainLayout);
    this->setMinimumWidth(300);
    this->adjustSize();

 //   connectBut->setEnabled(false);

    connect(connectBut, SIGNAL(clicked()), this, SIGNAL(conButClicked()));
    connect(ipLineEdit, SIGNAL(textChanged(QString)), this, SLOT(ipChanged(QString)));
    connect(portEdit, SIGNAL(textChanged(QString)), this, SLOT(portChanged(QString)));
}

void ConnectionMenu::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen* pen = new QPen();
    pen->setWidth(4);
    painter.setPen(*pen);
    QBrush* brush = new QBrush(Qt::Dense5Pattern);
    brush->setColor(QColor(48,213,208));
    painter.setBrush(*brush);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawRoundedRect(1,1,this->width()-2,this->height()-2,20,10);
}

void ConnectionMenu::ipChanged(QString text)
{
    *ipAdress = ipLineEdit->text();
}

void ConnectionMenu::portChanged(QString text)
{
    *port = portEdit->text().toInt();
}

void ConnectionMenu::statusChanged(CLIENT_STATUS st)
{
    QPalette* palette = new QPalette();
    switch(st)
    {
    case NOT_CONNECTED:
        statusLabel->setText(tr("НЕ ПОДКЛЮЧЕН"));
        palette->setColor(QPalette::Background, Qt::red);
        statusLabel->setAutoFillBackground(true);
        statusLabel->setPalette(*palette);
        break;
    case WAIT_FOR_START:
        statusLabel->setText(tr("ОЖИДАНИЕ НАЧАЛА ТРЕНАЖА"));
        palette->setColor(QPalette::Background, Qt::yellow);
        statusLabel->setAutoFillBackground(true);
        statusLabel->setPalette(*palette);
        connectBut->setEnabled(false);
        break;
    case TRAININNG:
        statusLabel->setText(tr("ТРЕНАЖ"));
        palette->setColor(QPalette::Background, Qt::green);
        statusLabel->setAutoFillBackground(true);
        statusLabel->setPalette(*palette);
        connectBut->setEnabled(false);
        break;
    case PAUSE:
        statusLabel->setText(tr("ПАУЗА"));
        palette->setColor(QPalette::Background, Qt::blue);
        statusLabel->setAutoFillBackground(true);
        statusLabel->setPalette(*palette);
        connectBut->setEnabled(false);
        break;
    }
}
