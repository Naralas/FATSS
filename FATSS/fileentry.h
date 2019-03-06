#ifndef FILEENTRY_H
#define FILEENTRY_H

#include <QObject>

class FileEntry : public QObject
{
    Q_OBJECT
private:
public:
    QString fileName;
    QList<int> *clusterIndex;
    explicit FileEntry(QObject *parent = nullptr);

signals:

public slots:
};

#endif // FILEENTRY_H
