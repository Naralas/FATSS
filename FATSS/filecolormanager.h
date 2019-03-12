#ifndef FILECOLORMANAGER_H
#define FILECOLORMANAGER_H

#include <QColor>
#include <QDateTime>

class FileColorManager
{
private:
    static FileColorManager *managerInstance;
    FileColorManager();

    QList<QColor*> usedColors;
    QColor* getNewColor();

    ~FileColorManager();
public:
    static FileColorManager* getInstance();
    static QColor* getColor();
};

#endif // FILECOLORMANAGER_H
