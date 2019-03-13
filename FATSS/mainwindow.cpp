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
    fsgui = new FSGUI(this);
    connect(fs, &FileSystem::createdFile, fsgui, &FSGUI::highlightFileClusters);

    containerLayout->addWidget(fsgui);
    containerLayout->addWidget(new Console(this));

    this->setCentralWidget(containerWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}
