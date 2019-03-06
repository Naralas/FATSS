#ifndef FSGUI_H
#define FSGUI_H

#include <QWidget>
#include <QStringListModel>

#include "diskview.h"
#include "ui_fsgui.h"


class FSGUI : public QWidget, private Ui::FSGUI
{
    Q_OBJECT

private:
public:
    explicit FSGUI(QWidget *parent = nullptr);

signals:

public slots:
    void addTableItem(QString item);
};

#endif // FSGUI_H
