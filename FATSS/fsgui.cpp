#include "fsgui.h"

FSGUI::FSGUI(QWidget *parent) : QWidget(parent)
{
    setupUi(this);
    diskViewWidget = new DiskView(this);
    gridLayout->addWidget(diskViewWidget, 1, 1, 1, 1);
}

void FSGUI::addTableItem(QString item)
{
    lvTable->addItem(item);
}
