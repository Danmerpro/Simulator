#include "editroutesmenu.h"

EditRoutesMenu::EditRoutesMenu(QWidget *parent) :
    QWidget(parent)
{
    title = new QLabel(this);
    title->setText( tr("Редактирование точек маршрута"));
    nextButton = new QPushButton( this );
    nextButton->setText( tr("Назад") );
    nextButton->adjustSize();
    prevButton = new QPushButton( this );
    prevButton->setText( tr("Далее") );
    prevButton->adjustSize();
    readyButton = new QPushButton( this );
    readyButton->setText( tr("Готово") );
    readyButton->adjustSize();
    curPoint = new QLineEdit();
    curPoint->setMaximumWidth(30);
    accessoryCombo = new QComboBox(this);
    accessoryCombo->addItem( tr ("Свой"), 0);
    accessoryCombo->addItem( tr ("Чужой"), 1);
    accessoryCombo->adjustSize();
    QHBoxLayout *HLayout1 = new QHBoxLayout();
    HLayout1->addWidget(prevButton);
    HLayout1->addWidget(curPoint);
    HLayout1->addWidget(nextButton);
    QVBoxLayout *VLayout1 = new QVBoxLayout();
    VLayout1->addWidget(title);
    speedBox = new QSpinBox(this);
    altBox = new QSpinBox(this);
    QFormLayout *formLayout = new QFormLayout();
    formLayout->addRow( tr("Принадлежность:"), accessoryCombo);
    formLayout->addRow( tr("Скороть:"), speedBox);
    formLayout->addRow( tr("Высота:"), altBox);
    VLayout1->addLayout(formLayout);
    VLayout1->addLayout(HLayout1);
    VLayout1->addWidget(readyButton);
    VLayout1->addStretch();
    this->setLayout(VLayout1);
    connect(readyButton, SIGNAL(clicked()), this, SIGNAL(readyButtonPushed()));
    this->adjustSize();
}
