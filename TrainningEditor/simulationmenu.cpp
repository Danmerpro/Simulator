#include "simulationmenu.h"

SimulationMenu::SimulationMenu(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout();
    QHBoxLayout* timeLayout = new QHBoxLayout();
    QHBoxLayout* butLayout = new QHBoxLayout();
    play = new QPushButton();
    play->setIcon(QIcon(":/Icon/play (3).png"));
    pause = new QPushButton();
    pause->setIcon(QIcon(":/Icon/pause (3).png"));
    stop = new QPushButton();
    stop->setIcon(QIcon(":/Icon/close_box_red.png"));
    butLayout->addWidget(play);
    butLayout->addWidget(pause);
    butLayout->addWidget(stop);
    timeElapsed = new QTimeEdit();
    timeElapsed->setReadOnly(true);
    QLabel* timeLabel = new QLabel(tr("Текущее время: "));
    timeLayout->addWidget(timeLabel);
    timeLayout->addWidget(timeElapsed);
    mainLayout->addLayout(butLayout);
    mainLayout->addLayout(timeLayout);
    mainLayout->addStretch();
    this->setLayout(mainLayout);
    this->setMinimumWidth(300);
    this->adjustSize();

    connect(play, SIGNAL(clicked()), this, SIGNAL(playClicked()));
    connect(pause, SIGNAL(clicked()), this, SIGNAL(pauseClicked()));
    connect(stop, SIGNAL(clicked()), this, SIGNAL(stopClicked()));
    connect(stop, SIGNAL(clicked()), this, SLOT(zeroTime()));
}

void SimulationMenu::paintEvent(QPaintEvent * event )
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

void SimulationMenu::updateTimeElapsed()
{
    timeElapsed->setTime(timeElapsed->time().addMSecs(1000));
}

void SimulationMenu::zeroTime()
{
    timeElapsed->setTime(QTime(0, 0));
}
