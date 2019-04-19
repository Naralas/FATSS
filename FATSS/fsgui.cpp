#include "fsgui.h"
#include <QDebug>

FSGUI::FSGUI(QWidget *parent) : QWidget(parent)
{
    mainLayout = new QHBoxLayout(this);
    clusterLayout = new QGridLayout(this);
    entriesLayout = new QVBoxLayout(this);
    lvTable = new QListWidget(this);

    clusterSize = QSize(25,25);
    initClusters(clusterSize);

    lvTable->setMinimumWidth(250);
    lvTable->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    clusterLayout->setSpacing(1);


    connect(lvTable, &QListWidget::itemPressed, [=]
        (QListWidgetItem *item)
        {
            FileEntry *targetEntry;
            for(int i = 0; i < fileEntries.size(); i++)
            {
                targetEntry = fileEntries.at(i);
                if(QString::compare(targetEntry->fileName, item->text()) == 0)
                {
                    highlightFileClusters(targetEntry);
                    break;
                }
            }
        }
    );

    QPushButton *btnClear = new QPushButton("Clear", this);
    connect(btnClear, &QPushButton::clicked, this, &FSGUI::resetHighlights);
    btnClear->setMinimumWidth(250);
    btnClear->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    entriesLayout->addWidget(lvTable);
    entriesLayout->addWidget(btnClear);

    mainLayout->addLayout(entriesLayout);
    //mainLayout->addWidget(lvTable);
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
            diskViewWidget->setStyleSheet("background-color: " + FileColorManager::unusedColor->name() + ";");
        }
        clusterWidgets.append(tempList);
    }

    // memory leak but only for tests
    // hope nobody sees this
    // it will be handled differently
    /*QList<int> *entryItemList = new QList<int>();
    entryItemList->append(0);
    entryItemList->append(1);
    entryItemList->append(2);
    entryItemList->append(29);
    entryItemList->append(25);
    FileEntry *entry = new FileEntry("Example item", entryItemList);
    insertFileEntry(entry);
    entryItemList = new QList<int>();
    entryItemList->append(0);
    entryItemList->append(1);
    entryItemList->append(2);
    entryItemList->append(29);
    entryItemList->append(25);
    entryItemList->append(3);
    entryItemList->append(5);
    entry = new FileEntry("Example item", entryItemList);
    updateFileEntry(entry);

    //removeFileEntry("Example item");
    //updateFileEntry(entry, FileEntryAction::INSERT);

    /*
    QList<int> *entryItemList2 = new QList<int>();
    entryItemList2->append(4);
    entryItemList2->append(5);
    entryItemList2->append(6);
    entryItemList2->append(9);
    entryItemList2->append(12);
    FileEntry *entry2 = new FileEntry("Example item2", entryItemList2);

    */
}

FSGUI::~FSGUI()
{
    qDeleteAll(fileEntries);
    qDeleteAll(clusterWidgets);
}

FileEntry *FSGUI::findFileEntry(QString filename)
{
    auto itrObj = std::find_if(fileEntries.begin(), fileEntries.end(),
                                          [=](FileEntry *entry) { return QString::compare(entry->fileName, filename, Qt::CaseSensitive) == 0;});
    if(itrObj == fileEntries.end())
        return nullptr;
    else
        return (*itrObj);
}

void FSGUI::insertFileEntry(FileEntry *fileEntry)
{
    fileEntries.append(fileEntry);

    addTableItem(fileEntry->fileName);
    fileEntry->displayColor = FileColorManager::getColor();

    for(int i = 0; i < fileEntry->clusterIndexes->size(); i++)
    {
        int index = fileEntry->clusterIndexes->at(i);

        QWidget *targetWidget = clusterWidgets.at(index % clusterSize.width())->at(index / clusterSize.height());
        setWidgetColor(targetWidget, fileEntry->displayColor);

        usedClusters.append(targetWidget);
    }

}

void FSGUI::removeFileEntry(QString filename)
{
    QList<QListWidgetItem*> items = lvTable->findItems(filename, Qt::MatchExactly);
    delete items.first();

    FileEntry *entry = findFileEntry(filename);
    if(entry == nullptr)
        return;

    for(int i = 0; i < entry->clusterIndexes->size(); i++)
    {
        int index = entry->clusterIndexes->at(i);
        QWidget *targetWidget = clusterWidgets.at(index % clusterSize.width())->at(index / clusterSize.height());
        setWidgetColor(targetWidget, FileColorManager::unusedColor);

        usedClusters.removeAt(usedClusters.indexOf(targetWidget));
    }

    fileEntries.removeAt(fileEntries.indexOf(entry));
    delete entry;
}

void FSGUI::updateFileEntry(FileEntry *fileEntry)
{
    removeFileEntry(fileEntry->fileName);
    insertFileEntry(fileEntry);
}

void FSGUI::highlightFileClusters(FileEntry *highlightEntry)
{
    for(QWidget *targetWidget : usedClusters)
    {
        setWidgetColor(targetWidget, FileColorManager::unhighlightColor);
    }
    qDebug() << highlightEntry->clusterIndexes;
    for(int i = 0; i < highlightEntry->clusterIndexes->size();i++)
    {
        int clusterIndex = highlightEntry->clusterIndexes->at(i);

        QWidget *targetWidget = clusterWidgets.at(clusterIndex % clusterSize.width())->at(clusterIndex / clusterSize.height());
        setWidgetColor(targetWidget, highlightEntry->displayColor);
    }
}


void FSGUI::resetHighlights()
{
    for(FileEntry *entry : fileEntries)
    {
        for(int i = 0; i < entry->clusterIndexes->size();i++)
        {
            int clusterIndex = entry->clusterIndexes->at(i);
            QWidget *targetWidget = clusterWidgets.at(clusterIndex % clusterSize.width())->at(clusterIndex / clusterSize.height());

            setWidgetColor(targetWidget, entry->displayColor);
        }
    }

    for(QListWidgetItem *item : lvTable->selectedItems())
      item->setSelected(false);
}





void FSGUI::addTableItem(QString item)
{
    lvTable->addItem(item);
}

void FSGUI::setWidgetColor(QWidget *targetWidget, QColor *color)
{
    targetWidget->setStyleSheet("background-color:" + color->name() + ";");
    targetWidget->style()->unpolish(targetWidget);
    targetWidget->style()->polish(targetWidget);
}

