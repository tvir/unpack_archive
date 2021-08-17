#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>
#include <QFileDialog>
#include <QTextEdit>
#include <QStringList>
#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include <QMap>

#include "unpackerthread.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void chooseDir();
    void findArchive();
    void unpack(const QString &path);
    void unpackFinished(const QString &path);
    void unpackInterrupted(const QString &path);
    void setProgressBarValue(int v);

    QLabel *curDirLabel;
    QTextEdit *logs;
    QProgressBar *progessBar;

    QTimer *timer;
    UnpackerThread *unpackerThread;

    QDir curDir;
    QStringList foundFiles;
    QMap <QString, QDateTime> unpackedFiles;

};
#endif // MAINWINDOW_H
