#ifndef FILEENTRY_H
#define FILEENTRY_H

#include <QObject>
#include <QList>

class FileEntry : public QObject
{
    Q_OBJECT
private:
    QString fileName;
    QList<int>* clusterIndex;
public:
    explicit FileEntry(QString name, QList<int>* clusterList, QObject *parent = nullptr);

signals:

public slots:
};

#endif // FILEENTRY_H
