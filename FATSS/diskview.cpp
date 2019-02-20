#include "diskview.h"

DiskView::DiskView(QWidget *parent) : QGraphicsView(parent)
{
    QSize rectSize(50,50);
    const int nb_rekt = 100;
    QGraphicsScene *scene = new QGraphicsScene(this);
    setScene(scene);
    for(int i = 0; i < nb_rekt; i++)
    {
        QGraphicsRectItem *rectItem = new QGraphicsRectItem(i % 10 * rectSize.width(), i / 10 * rectSize.height(), rectSize.width(), rectSize.height());
        rectItem->setBrush(Qt::gray);
        scene->addItem(rectItem);
    }
    //QGraphicsTextItem *item = new QGraphicsTextItem("Salut");

    //this->scene()->addItem(item);
}
