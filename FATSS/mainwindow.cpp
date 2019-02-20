#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "console.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(new Console());
}

MainWindow::~MainWindow()
{
    delete ui;
}
