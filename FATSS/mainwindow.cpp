#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QWidget *containerWidget = new QWidget(this);
    QVBoxLayout *containerLayout = new QVBoxLayout(containerWidget);

    fs = new FileSystem(100);
    connect(fs, FileSystem::createdFile, )

    containerLayout->addWidget(new FSGUI(this));
    containerLayout->addWidget(new Console(this));

    this->setCentralWidget(containerWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}
