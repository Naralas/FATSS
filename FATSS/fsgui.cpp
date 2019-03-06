#include "fsgui.h"

FSGUI::FSGUI(QWidget *parent) : QWidget(parent)
{
    setupUi(this);
    const int nb_rects = 100;
    const int nb_cols = 10;
    for(int i = 0; i < nb_rects; i++)
    {
        ClusterWidget *diskViewWidget = new ClusterWidget(this);
        diskViewWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        diskViewWidget->setStyleSheet("background-color: green;");
        clustersGrid->addWidget(diskViewWidget, i / nb_cols, i % nb_cols, 1, 1);
    }
}

void FSGUI::addTableItem(QString item)
{
    lvTable->addItem(item);
}
