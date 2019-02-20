#ifndef CONSOLE_H
#define CONSOLE_H

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>

#include "filesystem.h"

class Console : public QWidget
{
public:

    Console(QWidget *parent);
    ~Console();

public slots:
    void writeToConsole(QString Message);


private slots:

    void submitCommand();


private:

    //Vars
    QString consoleHandle;
    QLineEdit *lineEditCommand;
    QTextEdit *txtBlockCommand;
    QVBoxLayout *vLayout;
    FileSystem *fs;


    //Methods
    void createVolume();
    void formatVolume();
    void delVolume();
    void createFile();
    void delFile();



    //Helpers
    void writeCommand(QString command);
    void writeResult(QString message);
};

#endif // CONSOLE_H

