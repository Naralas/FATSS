#include "console.h"
#include <QDebug>

Console::Console(QWidget *parent) : QWidget(parent)
{
    txtBlockCommand = new QTextEdit(this);
    txtBlockCommand->setReadOnly(true);
    txtBlockCommand->setStyleSheet("QTextEdit { background-color: black; color: green }");

    lineEditCommand = new QLineEdit(this);
    //lineEditCommand->setStyleSheet("QLineEdit { background-color: black; color: green }");
    connect(lineEditCommand, &QLineEdit::returnPressed, this, &Console::submitCommand);

    vLayout =  new QVBoxLayout(this);
    vLayout->addWidget(txtBlockCommand);
    vLayout->addWidget(lineEditCommand);

    consoleHandle = "default";

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    commandsMap["create"] = &Console::createFile;
    commandsMap["delete"] = &Console::deleteFile;
    commandsMap["update"] = &Console::updateFile;
}

void Console::connectFileSystem(FileSystem *fs)
{
    connect(this, &Console::createdFile, fs, &FileSystem::createFile);
    connect(this, &Console::deletedFile, fs, &FileSystem::delFile);
    connect(this, &Console::updatedFile, fs, &FileSystem::updateFile);
}

Console::~Console()
{

}


///
/// \brief Console::submitCommand: Slot when user submit the command
///
void Console::submitCommand()
{
    QString command = lineEditCommand->text();
    writeCommand(command);

    QStringList commandArgs = command.split(" ");
    QString commandName = commandArgs[0].toLower();

    if(commandName == "cls")
    {
        txtBlockCommand->clear();
    }
    else if (commandsMap.keys().contains(commandName))
    {
        (this->*commandsMap[commandName])(commandArgs);
    }
    else
    {
        writeResult(commandArgs[0] + " is unknown");
    }

    lineEditCommand->clear();
}

void Console::writeResult(QString message)
{
    txtBlockCommand->append(message);
}

void Console::writeCommand(QString command)
{
    txtBlockCommand->append("user@" + consoleHandle + " > " + command);
}

void Console::createFile(QStringList args)
{
    if(args.length() == 3)
    {
        bool res = false;
        int size = args[2].toInt(&res);
        if(res && args[1] != " ")
        {
            emit createdFile(args[1], size);
        }
        else
        {
            writeResult("A problem occured");
        }
    }
    else
    {
        writeResult("Format demands 2 argument");
    }
}

void Console::deleteFile(QStringList args)
{
    if(args.length() == 2)
    {
        if(args[1] != " ")
        {
            emit deletedFile(args[1]);
        }
        else
        {
            writeResult("A problem occured");
        }
    }
    else
    {
        writeResult("Format demands 2 argument");
    }
}

void Console::updateFile(QStringList args)
{
    if(args.length() == 3)
    {
        bool res = false;
        int size = args[2].toInt(&res);
        if(res && args[1] != " ")
        {
            emit updatedFile(args[1], size);
        }
        else
        {
            writeResult("A problem occured");
        }
    }
    else
    {
        writeResult("Format demands 2 argument");
    }
}


