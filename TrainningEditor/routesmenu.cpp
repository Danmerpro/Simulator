#include "routesmenu.h"

RoutesMenu::RoutesMenu(QWidget *parent) :
    QWidget(parent)
{
    newRouteButton = new QPushButton( this );
    newRouteButton->setText( tr("����� �������") );
    newRouteButton->adjustSize();
    deleteRouteButton = new QPushButton( this );
    deleteRouteButton->setText( tr("������� �������") );
    deleteRouteButton->adjustSize();
    editRouteButton = new QPushButton( this );
    editRouteButton->setText( tr("������������� �������") );
    editRouteButton->adjustSize();
    QHBoxLayout *hLayout1 = new QHBoxLayout();
    hLayout1->addWidget( newRouteButton );
    hLayout1->addWidget( deleteRouteButton );
    hLayout1->addWidget( editRouteButton );
    QVBoxLayout *VLayout1 = new QVBoxLayout();
    VLayout1->addLayout(hLayout1);
    rtTable = new RoutesTable();
    VLayout1->addWidget(new QLabel(tr("��������:"),this));
    VLayout1->addWidget(rtTable);
    startButton = new QPushButton( this );
    startButton->setText( tr("������ ����������") );
    startButton->adjustSize();
    QHBoxLayout *hLayout2 = new QHBoxLayout();
    hLayout2->addStretch();
    hLayout2->addWidget(startButton);
    VLayout1->addLayout(hLayout2);
    VLayout1->addStretch();
    this->setLayout( VLayout1 );
    this->adjustSize();
}
