#ifndef UNPACKERTHREAD_H
#define UNPACKERTHREAD_H

#include <QThread>

#include <quazip/quazip/JlCompress.h>

class UnpackerThread : public QThread, public  JlCompress
{
    Q_OBJECT

public:
    UnpackerThread(QObject *parent = nullptr);
    void run() override;
    void setPaths(const QString &pack, const QString &dir);

signals:
    void unpackPercent(int p);
    void unpackFinished(const QString &path);
    void unpackInterrupted(const QString &path);

private:
    QString packPath;
    QString dirPath;
};

#endif // UNPACKERTHREAD_H
