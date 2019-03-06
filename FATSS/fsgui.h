#ifndef FSGUI_H
#define FSGUI_H

#include "fileentry.h"
#include "fileentryaction.h"
#include "filecolormanager.h"

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

    QSize clusterSize;
    void initClusters(QSize diskLayout);
    void addTableItem(QString item);
    ~FSGUI();
public:
    explicit FSGUI(QWidget *parent = nullptr);

    void setClusterUsed(int x, int y,QString colorName, bool used = true);
signals:

public slots:
    void updateFileEntry(FileEntry* fileEntry, FileEntryAction action);
    void highlightFileClusters(FileEntry* entry);

};

#endif // FSGUI_H
