#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "projecttreewidget.h"
#include "projecttree.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void resizeEvent(QResizeEvent *event) override;
private slots:
    void slotCreateProject(bool checked); //创建项目树
    void slotOpenProject(bool checked); //打开项目树
private:
    Ui::MainWindow *ui;
private:
    QWidget *projectTree; //目录树区域
    QWidget *pictureShow; //图片展示区域
signals:
    void signalOpenPath(const QString &importPath); //打开文件夹的路径信号
};
#endif // MAINWINDOW_H
