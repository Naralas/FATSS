#ifndef DISKVIEW_H
#define DISKVIEW_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QString>

class ClusterWidget : public QWidget
{
    Q_OBJECT
public:
    ClusterWidget(QWidget *parent = nullptr);
};

#endif // DISKVIEW_H
