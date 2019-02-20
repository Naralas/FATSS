#include "diskview.h"

ClusterWidget::ClusterWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel("Salut", this));
}
