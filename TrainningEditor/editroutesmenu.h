#ifndef EDITROUTESMENU_H
#define EDITROUTESMENU_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QFormLayout>
#include <QLineEdit>

class EditRoutesMenu : public QWidget
{
    Q_OBJECT
private:
    QLabel* title;
    QPushButton* nextButton;
    QPushButton* prevButton;
    QPushButton* readyButton;
    QComboBox* accessoryCombo;
    QSpinBox* speedBox;
    QSpinBox* altBox;
    QLineEdit* curPoint;
public:
    explicit EditRoutesMenu(QWidget *parent = 0);
    
signals:
    void readyButtonPushed();
    
public slots:
    
};

#endif // EDITROUTESMENU_H
