#ifndef FSGUI_H
#define FSGUI_H

#include "fileentry.h"
#include "filecolormanager.h"

#include <QWidget>
#include <QListWidget>
#include <QStringListModel>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QRgb>


class FSGUI : public QWidget
{
    Q_OBJECT

private:
    QHBoxLayout *mainLayout;
    QVBoxLayout *entriesLayout;
    QListWidget *lvTable;
    QGridLayout *clusterLayout;
    QList<FileEntry*> fileEntries;
    QList<QList<QWidget*>*> clusterWidgets;
    QList<QWidget*> usedClusters;

    QSize clusterSize;
    void initClusters(QSize diskLayout);
    void addTableItem(QString item);
    void setWidgetColor(QWidget *targetWidget, QColor *color);
    FileEntry* findFileEntry(QString filename);
    ~FSGUI();
public:
    explicit FSGUI(QWidget *parent = nullptr);

public slots:
    void highlightFileClusters(FileEntry* highlightEntry);
    void insertFileEntry(FileEntry *FileEntry);
    void updateFileEntry(FileEntry *fileEntry);
    void removeFileEntry(QString filename);
    void resetHighlights();


};

#endif // FSGUI_H
