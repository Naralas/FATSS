#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <QObject>
#include "fileentry.h"

class FileSystem : public QObject
{
    Q_OBJECT

public:
    FileSystem(int Size);
    void format();
    QString createFile(QString name, int size);
    QString delFile(QString name);

signals:
    void createdFile(FileEntry* file);
    void deletedFile(QString filename);

private:
    int clusterSize;
    int size;
    int freeClusters;
    QString name;
    bool isReady;

    QVector<QPair<int, int>>* fat;
    QVector<QPair<QString, int>>* rootDir;

    QVector<int>* data;


    int findFreeCluster(int index);
};

#endif // FILESYSTEM_H
