#include "filecolormanager.h"

FileColorManager* FileColorManager::managerInstance = 0;
QColor* FileColorManager::unhighlightColor;

FileColorManager::FileColorManager()
{
    qsrand(QDateTime::currentMSecsSinceEpoch() / 1000);
    FileColorManager::unhighlightColor = new QColor(150,150,150);
    usedColors.append(FileColorManager::unhighlightColor);
}

QColor* FileColorManager::getNewColor()
{
    QColor *color;
    do
    {
        int r = qrand() % 256;
        int g = qrand() % 256;
        int b = qrand() % 256;

        color = new QColor(r,g,b);
    } while(usedColors.contains(color));

    return color;
}

FileColorManager::~FileColorManager()
{
    qDeleteAll(usedColors);
}

FileColorManager *FileColorManager::getInstance()
{
    if(managerInstance == 0)
        managerInstance = new FileColorManager();

    return managerInstance;
}



QColor* FileColorManager::getColor()
{
    return FileColorManager::getInstance()->getNewColor();
}
