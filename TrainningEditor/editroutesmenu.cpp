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
    formLayout->addRow( tr("�������:"), speedBox);
    formLayout->addRow( tr("������:"), altBox);
    VLayout1->addLayout(formLayout);
    VLayout1->addLayout(HLayout1);
    VLayout1->addWidget(readyButton);
    VLayout1->addStretch();
    this->setLayout(VLayout1);
    connect(readyButton, SIGNAL(clicked()), this, SIGNAL(readyButtonPushed()));
    connect(speedBox, SIGNAL(editingFinished()), this, SLOT(pointSpeedChanged()));
    connect(altBox, SIGNAL(editingFinished()), this, SLOT(pointAltChanged()));
    this->adjustSize();
}

void EditRoutesMenu::setEditingRoute( MapObj* _route )
{
    route = _route;
    it = route->getPoints()->begin();
    pointNum = 1;
    speedBox->setValue((*it).getSpeed());
    altBox->setValue((*it).getAlt());
    accessoryCombo->setCurrentIndex( accessoryCombo->findData( route->getAsseccory()));
    curPoint->setText(QString().setNum(pointNum));
}

void EditRoutesMenu::pointSpeedChanged( )
{
    (*it).setSpeed(speedBox->value());
}

void EditRoutesMenu::pointAltChanged( )
{
    (*it).setAlt(altBox->value());
}
