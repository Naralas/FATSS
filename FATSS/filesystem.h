#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <QWidget>

class FileSystem
{
public:
    FileSystem(int Size);
    void format();
    QString createFile(QString name, int size);
    void delFile(QString name);


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
