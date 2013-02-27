#include "optionsmenu.h"

OptionsMenu::OptionsMenu(Options *_opt, QWidget *parent) :
    QWidget(parent)
{
    opt = _opt;
    speedOnMap = new QCheckBox(this);
    speedOnMap->setText(tr("скорость"));
    speedOnMap->setChecked( opt->getSpeedOnMap());

    altOnMap = new QCheckBox(this);
    altOnMap->setText(tr("высоту"));
    altOnMap->setChecked( opt->getAltOnMap());

    startTimeOnMap = new QCheckBox(this);
    startTimeOnMap->setText(tr("время начала"));
    startTimeOnMap->setChecked( opt->getStartTimeOnMap());

    speedUnitsCombo = new QComboBox(this);
    speedUnitsCombo->addItem( tr ("км/ч"), 0 );
    speedUnitsCombo->addItem( tr ("м/с"), 1 );

    altUnitsCombo = new QComboBox(this);
    altUnitsCombo->addItem( tr ("м"), 0 );
    altUnitsCombo->addItem( tr ("км"), 1 );

    QVBoxLayout* mainLayout = new QVBoxLayout();

    onMapGroupBox = new QGroupBox(this);
    onMapGroupBox->setTitle(tr("Отображать на карте"));
    QFont* font = new QFont();
    font->setBold(true);
    onMapGroupBox->setFont(*font);
    QVBoxLayout* onMapLayout = new QVBoxLayout();
    font->setBold(false);
    speedOnMap->setFont(*font);
    altOnMap->setFont(*font);
    startTimeOnMap->setFont(*font);
    onMapLayout->addWidget(speedOnMap);
    onMapLayout->addWidget(altOnMap);
    onMapLayout->addWidget(startTimeOnMap);
    onMapGroupBox->setLayout(onMapLayout);
    onMapGroupBox->adjustSize();

    unitGroupBox = new QGroupBox(this);
    unitGroupBox->setTitle(tr("Отображать"));
    font->setBold(true);
    unitGroupBox->setFont(*font);
    QLabel *speedLable = new QLabel(tr("скорость в"));
    QLabel *altLable = new QLabel(tr("высоту в"));
    QFormLayout *unitLayout = new QFormLayout();
    font->setBold(false);
    speedLable->setFont(*font);
    altLable->setFont(*font);
    speedUnitsCombo->setFont(*font);
    altUnitsCombo->setFont(*font);
    unitLayout->addRow(speedLable, speedUnitsCombo);
    unitLayout->addRow(altLable, altUnitsCombo);
    unitGroupBox->setLayout(unitLayout);

    mainLayout->addWidget( onMapGroupBox);
    mainLayout->addWidget( unitGroupBox);
    this->setLayout(mainLayout);

    this->setMinimumWidth(300);
    this->adjustSize();

    connect(this->speedOnMap,SIGNAL(clicked(bool)),this,SLOT(speedOnMapChanged(bool)));
    connect(this->altOnMap,SIGNAL(clicked(bool)),this,SLOT(altOnMapChanged(bool)));
    connect(this->startTimeOnMap,SIGNAL(clicked(bool)),this,SLOT(startTimeOnMapChanged(bool)));
}

void OptionsMenu::paintEvent(QPaintEvent * event )
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

void OptionsMenu::speedOnMapChanged( bool val)
{
    opt->setSpeedOnMap(val);
    emit updateScene();
}

void OptionsMenu::altOnMapChanged( bool val)
{
    opt->setAltOnMap(val);
    emit updateScene();
}

void OptionsMenu::startTimeOnMapChanged( bool val)
{
    opt->setStartTimeOnMap(val);
    emit updateScene();
}
