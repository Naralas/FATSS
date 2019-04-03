#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <QObject>
#include <QDebug>
#include "fileentry.h"
#include "fileentryaction.h"
#include "fatentry.h"

class FileSystem : public QObject
{
    Q_OBJECT

public:
    FileSystem(int Size);
    void format();
    QString createFile(QString name, int size);
    QString delFile(QString name);


    ~FileSystem();
signals:
    void createdFile(FileEntry* file, FileEntryAction action);
    void deletedFile(QString filename);

private:
    int clusterSize;
    int size;
    int freeClusters;
    QString name;
    bool isReady;

    QVector<FatEntry*> *fat;
    QVector<QPair<QString, int>>* rootDir;

    QVector<int>* data;


    int findFreeCluster(int index);
};

#endif // FILESYSTEM_H
