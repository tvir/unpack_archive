#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("Распаковка архивов");

    QPushButton *but = new QPushButton("Выбрать директорию");
    curDirLabel = new QLabel("Текущая директория: ");
    logs = new QTextEdit;
    logs->setReadOnly(true);
    progessBar = new QProgressBar;
    progessBar->setRange(0, 100);

    connect(but, &QPushButton::clicked, this, &MainWindow::chooseDir);

    QGridLayout *l = new QGridLayout;
    l->addWidget(but, 0, 0);
    l->addWidget(curDirLabel, 0, 1);
    l->addWidget(logs, 1, 0, 1, 2);
    l->addWidget(progessBar, 2, 0, 1, 2);
    l->setColumnStretch(1, 1);
    QWidget *widget = new QWidget(this);
    widget->setLayout(l);
    this->setCentralWidget(widget);

    timer =  new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::findArchive);

    unpackerThread = new UnpackerThread(this);
    connect(unpackerThread, &UnpackerThread::unpackPercent, this, &MainWindow::setProgressBarValue);
    connect(unpackerThread, &UnpackerThread::unpackFinished, this, &MainWindow::unpackFinished);
    connect(unpackerThread, &UnpackerThread::unpackInterrupted, this, &MainWindow::unpackInterrupted);
}

MainWindow::~MainWindow()
{
    unpackerThread->requestInterruption();
    unpackerThread->wait();
}

void MainWindow::chooseDir()
{
    timer->stop();
    QString dir = QFileDialog::getExistingDirectory(this);
    curDir = QDir(dir);
    curDirLabel->setText(QString("Текущая директория: %1").arg(dir));
    logs->setText(QString("Выбрана новая директория"));
    foundFiles.clear();
    unpackedFiles.clear();
    if (unpackerThread->isRunning())
        unpackerThread->requestInterruption();
    else
        findArchive();
    setProgressBarValue(0);
    if (dir != "")
        timer->start(10000);
}

void MainWindow::findArchive()
{
    if (!curDir.exists())
    {
        logs->append("Текущей директории не существует");
        return;
    }
    if (foundFiles.isEmpty() && !unpackerThread->isRunning())
    {
        logs->append("\nПроизводится проверка текущей директории");
        QStringList filters;
        filters << "*.zip";
        foundFiles = curDir.entryList(filters);
        for (int i = 0; i < foundFiles.size(); ++i)
        {
            QString curFile = curDir.absoluteFilePath(foundFiles[i]);
            if (unpackedFiles.contains(curFile))
            {
                if (unpackedFiles[curFile] == QFileInfo(curFile).lastModified())
                {
                    foundFiles.removeAt(i);
                    i--;
                }
                else
                    unpackedFiles[curFile] = QFileInfo(curFile).lastModified();
            }
            else
                unpackedFiles[curFile] = QFileInfo(curFile).lastModified();
        }
        if (foundFiles.isEmpty())
            logs->append("Не обнаружено файлов для распаковки");
        else
        {
            logs->append("Обнаружены файлы для распаковки");
            logs->append(foundFiles.join(" "));
            unpack(curDir.absoluteFilePath(foundFiles.takeFirst()));
        }
    }
}

void MainWindow::unpack(const QString &path)
{
    if (!curDir.exists() || !QFileInfo(path).exists())
        return;
    logs->append(QString("Начата распаковка архива %1").arg(path));
    unpackerThread->setPaths(path, curDir.absolutePath());
    unpackerThread->start();
}

void MainWindow::unpackFinished(const QString &path)
{
    logs->append(QString("Завершена распаковка архива %1").arg(path));
    if (!foundFiles.isEmpty())
        unpack(curDir.absoluteFilePath(foundFiles.takeFirst()));
}

void MainWindow::unpackInterrupted(const QString &path)
{
    logs->append(QString("Прервана распаковка архива %1").arg(path));
    setProgressBarValue(0);
}

void MainWindow::setProgressBarValue(int v)
{
    v = std::min(v, progessBar->maximum());
    v = std::max(v, progessBar->minimum());
    progessBar->setValue(v);

}

