#ifndef DISKVIEW_H
#define DISKVIEW_H

#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QString>
#include <QRect>

class DiskView : public QGraphicsView
{
public:
    DiskView(QWidget *parent = nullptr);
};

#endif // DISKVIEW_H
