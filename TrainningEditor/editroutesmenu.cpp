#include "editroutesmenu.h"

EditRoutesMenu::EditRoutesMenu(QWidget *parent) :
    QWidget(parent)
{
    title = new QLabel(this);
    title->setText( tr("�������������� ����� ��������"));
    nextButton = new QPushButton( this );
    nextButton->setText( tr("�����") );
    nextButton->adjustSize();
    prevButton = new QPushButton( this );
    prevButton->setText( tr("�����") );
    prevButton->adjustSize();
    readyButton = new QPushButton( this );
    readyButton->setText( tr("������") );
    readyButton->adjustSize();
    curPoint = new QLineEdit();
    curPoint->setMaximumWidth(30);
    accessoryCombo = new QComboBox(this);
    accessoryCombo->addItem( tr ("����"), ours );
    accessoryCombo->addItem( tr ("�����"), alien );
    accessoryCombo->adjustSize();
    QHBoxLayout *HLayout1 = new QHBoxLayout();
    HLayout1->addWidget(prevButton);
    HLayout1->addWidget(curPoint);
    HLayout1->addWidget(nextButton);
    QVBoxLayout *VLayout1 = new QVBoxLayout();
    VLayout1->addWidget(title);
    speedBox = new QSpinBox(this);
    speedBox->setMaximum(10000);
    altBox = new QSpinBox(this);
    altBox->setMaximum(10000);
    QFormLayout *formLayout = new QFormLayout();
    formLayout->addRow( tr("��������������:"), accessoryCombo);
    formLayout->addRow( tr("��������:"), speedBox);
    formLayout->addRow( tr("������:"), altBox);
    VLayout1->addLayout(formLayout);
    VLayout1->addLayout(HLayout1);
    VLayout1->addWidget(readyButton);
    VLayout1->addStretch();
    this->setLayout(VLayout1);
    connect(readyButton, SIGNAL(clicked()), this, SIGNAL(readyButtonPushed()));
    connect(speedBox, SIGNAL(valueChanged(int)), this, SLOT(pointSpeedChanged()));
    connect(altBox, SIGNAL(valueChanged(int)), this, SLOT(pointAltChanged()));
    connect(nextButton, SIGNAL(clicked()), this, SLOT(toNextPoint()));
    connect(prevButton, SIGNAL(clicked()), this, SLOT(toPrevPoint()));
    connect(curPoint, SIGNAL(textChanged(QString)),this, SLOT(toNumPoint(QString)));
    connect(accessoryCombo, SIGNAL(currentIndexChanged(int)),this, SLOT(asseccoryChanged(int index)));
    this->adjustSize();
}

void EditRoutesMenu::setEditingRoute( MapObj* _route )
{
    route = _route;
    it = route->getPoints()->begin();
    route->setActivePoint(&(*it));
    pointNum = 1;
    speedBox->setValue((*it).getSpeed());
    altBox->setValue((*it).getAlt());
    accessoryCombo->setCurrentIndex( accessoryCombo->findData( route->getAsseccory()));
    curPoint->setText(QString().setNum(pointNum));
    prevButton->setEnabled(false);
}

void EditRoutesMenu::pointSpeedChanged( )
{
    (*it).setSpeed(speedBox->value());
}

void EditRoutesMenu::pointAltChanged( )
{
    (*it).setAlt(altBox->value());
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
        QMessageBox::warning(this, tr("������"),tr("� �������� ��� ����� � ����� �������!"));
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

void EditRoutesMenu::asseccoryChanged( int index )
{
 //   ACCESSORY_TYPE assecory = reinterpret_cast<ACCESSORY_TYPE>(accessoryCombo->itemData(index));
 //   route->setAsseccory(assecory);
}
