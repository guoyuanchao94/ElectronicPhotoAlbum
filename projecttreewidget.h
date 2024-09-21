#ifndef PROJECTTREEWIDGET_H
#define PROJECTTREEWIDGET_H

#include <QTreeWidget>
#include <QObject>
#include <QSet>
#include <QAction>
#include <QProgressDialog>
#include <QSharedPointer>
#include "projecttreethread.h"
#include "openprojectthread.h"
#include "slideshowdialog.h"
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QAudioOutput>
class ProjectTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    ProjectTreeWidget(QWidget *parent = nullptr);
public:
    void addProjectToTree(const QString &name,const QString &path);
private:
    QSet<QString> pathSet{}; //记录当前创建或打开的项目的路径
    QTreeWidgetItem *rightButtonItem; //记录右键点击的项
    QTreeWidgetItem *activeItem; //记录当前活动项目
    QAction *actImport; //导入项目动作
    QAction *actSetActive; //设置活动项目
    QAction *actCloseProject; //关闭项目动作
    QAction *actSlideShow; //轮播图播放动作
    QProgressDialog *progressDialog; // 导入文件对话框
    QProgressDialog *openProgressDialog; //打开项目的对话框
    QTreeWidgetItem *mySelectedItem; //记录选中的图片
    QSharedPointer<ProjectTreeThread> threadCreateTree; //创建项目的线程
    QSharedPointer<OpenProjectThread> threadOpenTree; //打开项目的线程
    QSharedPointer<SlideShowDialog> slideDialog; //幻灯片展示区域
    QMediaPlayer *player;
    QAudioOutput *audio;
public slots:
    void slotOpenProject(const QString &importPath);
    void slotPreviousShow();
    void slotNextShow();
    void slotSetMusic();
    void slotStartMusic();
    void slotStopMusic();
private slots:
    void slotItemPressed(QTreeWidgetItem *item, int column);
    void slotDoubleClickedItem(QTreeWidgetItem *item, int column);
    void slotImport();
    void slotUpdateProgress(int fileCount);
    void slotFinished(int fileCount); //创建项目对话框文件复制完成槽函数
    void slotCancleProgress(); //进度条对话框取消按钮

    void slotSetActive(); //设置活动项目槽函数
    void slotCloseProject(); //关闭项目

    //另一个打开文件对话框
    void slotOpenUpdateProgress(int fileCount);
    void slotOpenFinished(int fileCount); //
    void slotOpenCancleProgress(); //进度条对话框取消按钮

    void slotSlideShow(); //幻灯片播放槽函数
signals:
    void signalCancleProgress(); //进度条取消槽函数中发射的信号,用于停止线程
    void signalOpenCancleProgress();
    void signalUpdateSelected(const QString &); //通知pictureshow更换图片
    void signalUpdatePicture(const QString &); //通知pictureshow更换图片
    void signalClearPicture();
};

#endif // PROJECTTREEWIDGET_H
