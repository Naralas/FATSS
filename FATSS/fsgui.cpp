#include "fsgui.h"
#include <QDebug>

FSGUI::FSGUI(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    clusterLayout = new QGridLayout(this);
    lvTable = new QListWidget(this);

    clusterSize = QSize(25,25);
    initClusters(clusterSize);

    lvTable->setMinimumWidth(200);
    lvTable->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    clusterLayout->setSpacing(1);


    connect(lvTable, &QListWidget::itemPressed, [=]
        (QListWidgetItem *item)
        {
            FileEntry *targetEntry;
            for(int i = 0; i < fileEntries.size(); i++)
            {
                targetEntry = fileEntries[i];
                if(QString::compare(targetEntry->fileName, item->text()) == 0)
                {
                    highlightFileClusters(targetEntry);
                    break;
                }
            }
        }
    );

    mainLayout->addWidget(lvTable);
    mainLayout->addLayout(clusterLayout);
}

void FSGUI::initClusters(QSize diskLayout)
{

    for(int i = 0; i < diskLayout.width(); i++)
    {
        QList<QWidget*> *tempList = new QList<QWidget*>();
        for(int j = 0; j < diskLayout.height(); j++)
        {
            QWidget *diskViewWidget = new QWidget(this);

            QVBoxLayout *layout = new QVBoxLayout(diskViewWidget);
            // init column by column
            clusterLayout->addWidget(diskViewWidget, j, i, 1, 1);

            tempList->append(diskViewWidget);
            diskViewWidget->setLayout(layout);
            diskViewWidget->setStyleSheet("background-color: lightgray;");
        }
        clusterWidgets.append(tempList);
    }


    // memory leak but only for tests
    // hope nobody sees this
    // it will be handled differently
    QList<int> *entryItemList = new QList<int>();
    entryItemList->append(0);
    entryItemList->append(1);
    entryItemList->append(2);
    entryItemList->append(29);
    entryItemList->append(25);
    FileEntry *entry = new FileEntry("Example item", entryItemList);
    updateFileEntry(entry, FileEntryAction::INSERT);

    QList<int> *entryItemList2 = new QList<int>();
    entryItemList2->append(4);
    entryItemList2->append(5);
    entryItemList2->append(6);
    entryItemList2->append(9);
    entryItemList2->append(12);
    FileEntry *entry2 = new FileEntry("Example item2", entryItemList2);

    updateFileEntry(entry2, FileEntryAction::INSERT);
}

FSGUI::~FSGUI()
{
    qDeleteAll(clusterWidgets);
}



void FSGUI::setClusterUsed(int x, int y, QString colorName, bool used)
{
    QWidget *targetWidget = clusterWidgets.at(x)->at(y);

    if(used)
    {
        targetWidget->setStyleSheet("background-color:" + colorName + ";");
    }
    else
        targetWidget->setStyleSheet("background-color: lightgray;");

    targetWidget->style()->unpolish(targetWidget);
    targetWidget->style()->polish(targetWidget);

    usedClusters.append(targetWidget);
}

void FSGUI::updateFileEntry(FileEntry *fileEntry, FileEntryAction action)
{
    addTableItem(fileEntry->fileName);
    QColor *color = FileColorManager::getColor();
    fileEntry->displayColor = color;
    for(int i = 0; i < fileEntry->clusterIndexes->size(); i++)
    {
        if(action == FileEntryAction::INSERT)
            fileEntries.append(fileEntry);
        int index = fileEntry->clusterIndexes->at(i);
        switch(action)
        {
            case FileEntryAction::INSERT:
                setClusterUsed(index % clusterSize.width(), index / clusterSize.height(), color->name(),  true);
                break;
            case FileEntryAction::UPDATE:
                setClusterUsed(index % clusterSize.width(), index / clusterSize.height(), color->name(), true);
                break;
            case FileEntryAction::DELETE:
                setClusterUsed(index % clusterSize.width(), index / clusterSize.height(), color->name(), !true);
                break;
        }

    }
}

void FSGUI::highlightFileClusters(FileEntry *highlightEntry)
{
    for(FileEntry *entry : fileEntries)
    {
        QColor *entryColor = entry->displayColor;
        for(int i = 0; i < entry->clusterIndexes->size(); i++)
        {
            int clusterIndex = entry->clusterIndexes->at(i);

            QWidget *targetWidget = clusterWidgets.at(clusterIndex * clusterSize.width())->at(clusterIndex * clusterSize.height());
            if(highlightEntry != entry)
            {
                QColor displayColor = QColor::fromRgb(entryColor->rgba());
                displayColor.setAlpha(40);
                targetWidget->setStyleSheet("background-color:" + displayColor.name() + ";");
            }
        }
    }
    /*for(int i = 0; i < clusterWidgets.size(); i++)
    {
        for(int j = 0; j < clusterWidgets.at(i)->size(); j++)
        {
            QWidget *targetWidget = clusterWidgets.at(i)->at(j);

            targetWidget->setStyleSheet("background-color:" + FileColorManager::unhighlightColor->name() + ";");
            targetWidget->style()->unpolish(targetWidget);
            targetWidget->style()->polish(targetWidget);
        }
    }*/
}

void FSGUI::addTableItem(QString item)
{
    lvTable->addItem(item);
}
