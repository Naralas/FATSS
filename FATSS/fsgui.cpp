#include "fsgui.h"
#include <QDebug>

FSGUI::FSGUI(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    clusterLayout = new QGridLayout(this);
    lvTable = new QListWidget(this);

    initClusters(QSize(30, 25));

    lvTable->setMinimumWidth(200);
    lvTable->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    clusterLayout->setSpacing(1);

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


    addTableItem("Example item");

    setClusterUsed(0,0);
    setClusterUsed(1,0);
    setClusterUsed(2,0);
}

FSGUI::~FSGUI()
{
    qDeleteAll(clusterWidgets);
}

void FSGUI::setClusterUsed(int x, int y, bool used)
{
    QWidget *targetWidget = clusterWidgets.at(x)->at(y);

    if(used)
    {
        targetWidget->setStyleSheet("background-color: blue;");
    }
    else
        targetWidget->setStyleSheet("background-color: lightgray;");

    targetWidget->style()->unpolish(targetWidget);
    targetWidget->style()->polish(targetWidget);
}
void FSGUI::addTableItem(QString item)
{
    lvTable->addItem(item);
}
