#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QWidget *containerWidget = new QWidget(this);
    QVBoxLayout *containerLayout = new QVBoxLayout(containerWidget);

    console = new Console(this);

    fs = new FileSystem(100);
    fsgui = new FSGUI(this);

    console->connectFileSystem(fs);

    connect(fs, &FileSystem::createdFile, fsgui, &FSGUI::updateFileEntry);

    containerLayout->addWidget(fsgui);
    containerLayout->addWidget(console);

    this->setCentralWidget(containerWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}
