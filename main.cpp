#include "mainwindow.h"

#include <QApplication>
#include <QFile>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("电子相册");
    w.showMaximized();
    w.setWindowIcon(QIcon(":/resource/icon/album.svg"));
    QFile qssFile(":/resource/style/style.qss");
    if(!qssFile.open(QIODeviceBase::ReadOnly))
    {
        qDebug()<<"qss文件打开失败";
        return -1;
    }
    QString style=QLatin1String(qssFile.readAll());
    a.setStyleSheet(style);
    qssFile.close();
    return a.exec();
}
