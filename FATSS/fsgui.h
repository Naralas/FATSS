#ifndef FSGUI_H
#define FSGUI_H

#include <QWidget>
#include <QListWidget>
#include <QStringListModel>
#include <QLayout>
#include <QLabel>


class FSGUI : public QWidget
{
    Q_OBJECT

private:
    QHBoxLayout *mainLayout;
    QListWidget *lvTable;
    QGridLayout *clusterLayout;
    QList<QList<QWidget*>*> clusterWidgets;

    void initClusters(QSize diskLayout);

    ~FSGUI();
public:
    explicit FSGUI(QWidget *parent = nullptr);

    void setClusterUsed(int x, int y, bool used = true);
signals:

public slots:
    void addTableItem(QString item);
};

#endif // FSGUI_H
