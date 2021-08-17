#include "unpackerthread.h"

UnpackerThread::UnpackerThread(QObject *parent) : QThread(parent)
{

}

void UnpackerThread::run()
{
    QuaZip zip(packPath);
    if(!zip.open(QuaZip::mdUnzip))
        return;
    QString cleanDir = QDir::cleanPath(dirPath);
    QDir directory(cleanDir);
    QString absCleanDir = directory.absolutePath();
    QStringList extracted;
    if (!zip.goToFirstFile())
        return;
    quint64 allFileSize = 0;
    QList<QuaZipFileInfo64> infoList = zip.getFileInfoList64();
    for (auto i : infoList)
        allFileSize += i.compressedSize;
    quint64 curSize = 0;
    do {
        if (isInterruptionRequested())
        {
            emit unpackInterrupted(packPath);
            return;
        }
        QString name = zip.getCurrentFileName();
        QString absFilePath = directory.absoluteFilePath(name);
        QString absCleanPath = QDir::cleanPath(absFilePath);
        if (!absCleanPath.startsWith(absCleanDir + QLatin1String("/")))
            continue;
        if (extractFile(packPath, name, absFilePath).isNull())
            return;
        QuaZipFileInfo64 info;
        zip.getCurrentFileInfo(&info);
        curSize += info.compressedSize;
        emit unpackPercent(curSize * 100. / allFileSize);
        extracted.append(absFilePath);
    } while (zip.goToNextFile());

    zip.close();
    emit unpackFinished(packPath);
}

void UnpackerThread::setPaths(const QString &pack, const QString &dir)
{
    packPath = pack;
    dirPath = dir;
}

