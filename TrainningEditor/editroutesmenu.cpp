#include "editroutesmenu.h"

EditRoutesMenu::EditRoutesMenu(QWidget *parent) :
    QWidget(parent)
{
    title = new QLabel(this);
    title->setText( tr("�������������� ����� ��������"));
    nextButton = new QPushButton( this );
    nextButton->setText( tr("����� �������") );
    nextButton->adjustSize();
    prevButton = new QPushButton( this );
    prevButton->setText( tr("����� �������") );
    prevButton->adjustSize();
    readyButton = new QPushButton( this );
    readyButton->setText( tr("����� �������") );
    readyButton->adjustSize();
    accessoryCombo = new QComboBox(this);
    accessoryCombo->addItem( tr ("����"), 0);
    accessoryCombo->addItem( tr ("�����"), 1);
    accessoryCombo->adjustSize();
    QVBoxLayout *VLayout1 = new QVBoxLayout();
    VLayout1->addWidget(title);
    VLayout1->addWidget(accessoryCombo);
    VLayout1->addWidget(nextButton);
    VLayout1->addWidget(prevButton);
    VLayout1->addWidget(readyButton);
    VLayout1->addStretch();
    this->setLayout(VLayout1);
    this->adjustSize();
}
