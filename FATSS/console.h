#ifndef CONSOLE_H
#define CONSOLE_H

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QMap>

#include "filesystem.h"

class Console : public QWidget
{
    Q_OBJECT

public:

    Console(QWidget *parent);
    void connectFileSystem(FileSystem* fs);
    ~Console();

private slots:

    void submitCommand();

signals:
    void createdFile(QString filename, int size);
    void deletedFile(QString filename);
    void updatedFile(QString filename, int newSize);

private:

    typedef void(Console::*pfunc)(QStringList);
    QMap<QString, pfunc> commandsMap;

    //Vars
    QString consoleHandle;
    QLineEdit *lineEditCommand;
    QTextEdit *txtBlockCommand;
    QVBoxLayout *vLayout;
    FileSystem *fs;


    //Methods
    void createFile(QStringList args);
    void deleteFile(QStringList args);
    void updateFile(QStringList args);



    //Helpers
    void writeCommand(QString command);
    void writeResult(QString message);

};

#endif // CONSOLE_H

