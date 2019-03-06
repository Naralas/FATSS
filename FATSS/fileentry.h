#ifndef FILEENTRY_H
#define FILEENTRY_H

#include <QObject>

class FileEntry : public QObject
{
    Q_OBJECT
private:
    QString fileName;

public:
    explicit FileEntry(QObject *parent = nullptr);

signals:

public slots:
};

#endif // FILEENTRY_H
