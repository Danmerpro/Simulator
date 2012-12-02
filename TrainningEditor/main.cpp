#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec *russianCodec=QTextCodec::codecForName("Windows-1251");
    QTextCodec::setCodecForTr(russianCodec);
    MainWindow w;
    w.showMaximized();
    w.show();
    return a.exec();
}
