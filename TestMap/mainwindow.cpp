#include "mainwindow.h"
#include "mapview.h"
#include "mapscene.h"
#include "mapitem.h"
#include "radarline.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    trainningMap = new QPixmap("lenoblast-obz.jpg");
    mscene = new MapScene();
    map = new MapItem(*trainningMap);
    mscene->addItem(map);
    mview = new MapView();
    mview->setDragMode(QGraphicsView::ScrollHandDrag);
    mview->setScene(mscene);
    QGraphicsRectItem* rectIt= new QGraphicsRectItem(QRectF(500,100,500,200),map);
    rectIt->setBrush(QColor(25,103,23));
    QGraphicsRectItem* rectIt2= new QGraphicsRectItem(QRectF(100,0,20,20),rectIt);
    rectIt2->setBrush(QColor(105,153,123));
    rectIt2->setFlag(QGraphicsItem::ItemIgnoresTransformations,true);
    mscene->addItem(rectIt);
    mscene->addItem(rectIt2);
    setCentralWidget(mview);
    setWindowTitle(tr("Map Demo"));
    mview->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
}

MainWindow::~MainWindow()
{
    
}

