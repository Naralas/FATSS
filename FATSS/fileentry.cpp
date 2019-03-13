#include "fileentry.h"

FileEntry::FileEntry(QString name, QList<int>* clusterList, QObject *parent) : QObject(parent)
{
    fileName = name;
    clusterIndexes = clusterList;
}

