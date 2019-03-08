#ifndef FILEENTRY_H
#define FILEENTRY_H

#include <QObject>
#include <QList>

class FileEntry : public QObject
{
    Q_OBJECT
private:
public:
    explicit FileEntry(QString name, QList<int>* clusterList, QObject *parent = nullptr);
    QString fileName;
    QList<int>* clusterIndexes;
    QColor *displayColor;
signals:

public slots:
};

#endif // FILEENTRY_H
