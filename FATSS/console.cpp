#include "console.h"

Console::Console(QWidget *parent) : QWidget(parent)
{
    txtBlockCommand = new QTextEdit(this);
    txtBlockCommand->setReadOnly(true);
    txtBlockCommand->setStyleSheet("QTextEdit { background-color: black; color: green }");
    txtBlockCommand->setText("Maxime@pc >");

    lineEditCommand = new QLineEdit(this);
    lineEditCommand->setStyleSheet("QLineEdit { background-color: black; color: green }");
    connect(lineEditCommand, &QLineEdit::returnPressed, this, &Console::submitCommand);

    vLayout =  new QVBoxLayout(this);
    vLayout->addWidget(txtBlockCommand);
    vLayout->addWidget(lineEditCommand);

    consoleHandle = "default";

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

    if(commandName == "format")
    {
        if(commandArgs.length() == 2)
        {

        }
        else
        {
            writeResult("Format demands 1 argument");
        }
    }
    else if(commandName == "cls")
    {
        if(commandArgs.length() == 1)
        {
            txtBlockCommand->clear();
        }
        else
        {
            writeResult("Format demands 1 argument");
        }
    }
    else if(commandName == "vcreate")
    {
        if(commandArgs.length() == 3)
        {

        }
        else
        {
            writeResult("Format demands 2 argument");
        }
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

