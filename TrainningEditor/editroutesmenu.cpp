#include "editroutesmenu.h"

EditRoutesMenu::EditRoutesMenu(QWidget *parent) :
    QWidget(parent)
{
    accessoryCombo = new QComboBox(this);
    accessoryCombo->addItem( tr ("Свой"), 0 );
    accessoryCombo->addItem( tr ("Чужой"), 1 );
    accessoryCombo->adjustSize();

    startTime = new QTimeEdit();

    speedBox = new QDoubleSpinBox(this);
    speedBox->setMaximum(15000);

    altBox = new QDoubleSpinBox(this);
    altBox->setMaximum(50000);

    nextButton = new QPushButton( this );
    nextButton->setText( tr("Далее") );

    prevButton = new QPushButton( this );
    prevButton->setText( tr("Назад") );

    readyButton = new QPushButton( this );
    readyButton->setText( tr("Готово") );

    curPoint = new QLineEdit();
    curPoint->setMaximumWidth(30);

    routeGropBox = new QGroupBox(this);
    QFont* font = new QFont();
    font->setBold(true);
    routeGropBox->setFont(*font);
    routeGropBox->setTitle(tr("Маршрут"));
    QFormLayout *formLayout1 = new QFormLayout();
    QLabel *asseccoryLable = new QLabel(tr("Принадлежность"));
    QLabel *stTimeLable = new QLabel(tr("Время начала (чч:мм:сс)"));
    formLayout1->addRow( asseccoryLable, accessoryCombo);
    formLayout1->addRow( stTimeLable, startTime);
    font->setBold(false);
    asseccoryLable->setFont(*font);
    startTime->setFont(*font);
    stTimeLable->setFont(*font);
    accessoryCombo->setFont(*font);
    routeGropBox->setLayout(formLayout1);

    pointGropBox = new QGroupBox(this);
    font->setBold(true);
    pointGropBox->setFont(*font);
    QLabel *speedLable = new QLabel(tr("Скорость:"));
    QLabel *altLable = new QLabel(tr("Высота:"));
    pointGropBox->setTitle(tr("Маршрутная точка"));
    QFormLayout *formLayout2 = new QFormLayout();
    formLayout2->addRow( speedLable, speedBox);
    formLayout2->addRow( altLable, altBox);
    font->setBold(false);
    speedLable->setFont(*font);
    altLable->setFont(*font);
    speedBox->setFont(*font);
    altBox->setFont(*font);
    pointGropBox->setLayout(formLayout2);

    QHBoxLayout *HLayout1 = new QHBoxLayout();
    HLayout1->addStretch();
    HLayout1->addWidget(prevButton);
    HLayout1->addWidget(curPoint);
    HLayout1->addWidget(nextButton);

    QHBoxLayout *HLayout2 = new QHBoxLayout();
    HLayout2->addStretch();
    HLayout2->addWidget(readyButton);

    QVBoxLayout *VLayout1 = new QVBoxLayout();
    VLayout1->addWidget(routeGropBox);
    VLayout1->addWidget(pointGropBox);
    VLayout1->addLayout(HLayout1);
    VLayout1->addLayout(HLayout2);
    VLayout1->addStretch();
    this->setLayout(VLayout1);

    connect(readyButton, SIGNAL(clicked()), this, SIGNAL(readyButtonPushed()));
    connect(speedBox, SIGNAL(valueChanged(double)), this, SLOT(pointSpeedChanged()));
    connect(altBox, SIGNAL(valueChanged(double)), this, SLOT(pointAltChanged()));
    connect(nextButton, SIGNAL(clicked()), this, SLOT(toNextPoint()));
    connect(prevButton, SIGNAL(clicked()), this, SLOT(toPrevPoint()));
    connect(curPoint, SIGNAL(textChanged(QString)),this, SLOT(toNumPoint(QString)));
    connect(accessoryCombo, SIGNAL(currentIndexChanged(int)),this, SLOT(accessoryChanged(int)));
    connect(startTime, SIGNAL(editingFinished()), this, SLOT(startTimeChanged()));

    this->setMinimumWidth(300);
    this->adjustSize();
}

void EditRoutesMenu::setEditingRoute()
{
    int accessory;
    switch( route->getAsseccory() )
    {
    case(ours):
        accessory = 0;
        break;
    case(alien):
        accessory = 1;
    }
    it = route->getPoints()->begin();
    route->setActivePoint(&(*it));
    pointNum = 1;
    speedBox->setValue((*it).getSpeed());
    altBox->setValue((*it).getAlt());
    accessoryCombo->setCurrentIndex( accessoryCombo->findData( accessory ));
    curPoint->setText(QString().setNum(pointNum));
    prevButton->setEnabled(false);
    startTime->setTime(route->getStartTime());
}

void EditRoutesMenu::changeCurRoute( MapObj* _route )
{
    route = _route;
}

void EditRoutesMenu::pointSpeedChanged( )
{
    (*it).setSpeed(speedBox->value());
    emit updateScene();
}

void EditRoutesMenu::pointAltChanged( )
{
    (*it).setAlt(altBox->value());
    emit updateScene();
}

void EditRoutesMenu::toNextPoint()
{
    if( it+1 != route->getPoints()->end() )
    {
        it++;
        pointNum++;
        curPoint->setText(QString().setNum(pointNum));
        route->setActivePoint(&(*it));
        if( it+1 == route->getPoints()->end())
        {
            nextButton->setEnabled(false);
        }
        if( it != route->getPoints()->begin() )
        {
            prevButton->setEnabled(true);
        }
        speedBox->setValue((*it).getSpeed());
        altBox->setValue((*it).getAlt());
        emit activePointChanged();
    }
}

void EditRoutesMenu::toPrevPoint()
{
    if( it != route->getPoints()->begin() )
    {
        it--;
        pointNum--;
        curPoint->setText(QString().setNum(pointNum));
        route->setActivePoint(&(*it));
        if( it == route->getPoints()->begin())
        {
            prevButton->setEnabled(false);
        }
        if( it+1 != route->getPoints()->end() )
        {
            nextButton->setEnabled(true);
        }
        speedBox->setValue((*it).getSpeed());
        altBox->setValue((*it).getAlt());
        emit activePointChanged();
    }
}

void EditRoutesMenu::toNumPoint( QString _num )
{
    int num = _num.toInt();
    if( num > route->getPoints()->size())
    {
        QMessageBox::warning(this, tr("Ошибка"),tr("В маршруте нет точки с таким номером!"));
        num = pointNum;
    }
    it = route->getPoints()->begin();
    int i = 0;
    for( it ; i != num-1; it++, i++ );
    pointNum = num;
    curPoint->setText(QString().setNum(pointNum));
    route->setActivePoint(&(*it));
    if( it == route->getPoints()->begin())
    {
        prevButton->setEnabled(false);
    }
    else
    {
        prevButton->setEnabled(true);
    }
    if( it+1 == route->getPoints()->end() )
    {
        nextButton->setEnabled(false);
    }
    else
    {
        nextButton->setEnabled(true);
    }
    speedBox->setValue((*it).getSpeed());
    altBox->setValue((*it).getAlt());
    emit activePointChanged();
}

void EditRoutesMenu::changeActivePoint()
{
    it = route->getPoints()->begin();
    int i = 0;
    for( it ; &(*it) != route->getActivePoint(); it++, i++ );
    pointNum = i+1;
    curPoint->setText(QString().setNum(pointNum));
    if( it == route->getPoints()->begin())
    {
        prevButton->setEnabled(false);
    }
    else
    {
        prevButton->setEnabled(true);
    }
    if( it+1 == route->getPoints()->end() )
    {
        nextButton->setEnabled(false);
    }
    else
    {
        nextButton->setEnabled(true);
    }
    speedBox->setValue((*it).getSpeed());
    altBox->setValue((*it).getAlt());
}

void EditRoutesMenu::accessoryChanged( int index )
{
    ACCESSORY_TYPE accessory;
    switch( accessoryCombo->itemData(index).toInt())
    {
    case(0):
        accessory = ours;
        break;
    case(1):
        accessory = alien;
    }
    route->setAsseccory(accessory);
}

void EditRoutesMenu::paintEvent(QPaintEvent * event )
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

void EditRoutesMenu::startTimeChanged()
{
    route->setStartTime(startTime->time());
    emit updateScene();
}
