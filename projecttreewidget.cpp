#include <QIcon>
#include <QDir>
#include <QHeaderView>
#include <QGuiApplication>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QThread>
#include <functional>
#include <QFont>
#include "const.h"
#include "projecttreewidget.h"
#include "projecttreeitem.h"
#include "removeprojectdialog.h"

ProjectTreeWidget::ProjectTreeWidget(QWidget *parent)
    :QTreeWidget(parent),rightButtonItem(nullptr),
    activeItem(nullptr),progressDialog(nullptr),openProgressDialog(nullptr),
    mySelectedItem(nullptr),threadCreateTree(nullptr),threadOpenTree(nullptr)
{
    this->header()->hide(); //隐藏头数字
    actImport=new QAction(QIcon(":/resource/icon/import.png"),tr("导入文件"),this);
    actSetActive=new QAction(QIcon(":/resource/icon/core.png"),tr("设置活动项目"),this);
    actCloseProject=new QAction(QIcon(":/resource/icon/close.png"),tr("关闭项目"),this);
    actSlideShow=new QAction(QIcon(":/resource/icon/slideshow.png"),tr("轮播图播放"),this);

    connect(this,&ProjectTreeWidget::itemPressed,this,&ProjectTreeWidget::slotItemPressed);
    connect(actImport,&QAction::triggered,this,&ProjectTreeWidget::slotImport);
    connect(actSetActive,&QAction::triggered,this,&ProjectTreeWidget::slotSetActive);
    connect(actCloseProject,&QAction::triggered,this,&ProjectTreeWidget::slotCloseProject);
    connect(this,&ProjectTreeWidget::itemDoubleClicked,this,&ProjectTreeWidget::slotDoubleClickedItem);
    connect(actSlideShow,&QAction::triggered,this,&ProjectTreeWidget::slotSlideShow);
    player=new QMediaPlayer(this);
    audio=new QAudioOutput(this);
    player->setAudioOutput(audio);
}

void ProjectTreeWidget::addProjectToTree(const QString &name, const QString &path)
{
    QDir dir(path);
    //构造一个绝对路劲filePath
    QString filePath=dir.absoluteFilePath(name);
    //QSet容器找到说明存在同名路径
    //这里的判断感觉,没有必要,毕竟创建向导页面已经避免创建同路径的同名文件夹
    if(pathSet.find(filePath)!=pathSet.end())
    {
        qDebug()<<"项目树中存在同名文件夹,无法重复添加"<<Qt::endl;
        return;
    }
    qDebug()<<"添加到目录树中的文件夹的路径: "<<filePath;
    QDir projectDir(filePath);
    if(!projectDir.exists())
    {
        bool enable=projectDir.mkpath(filePath);
        if(!enable)
        {
            qDebug()<<"路径创建失败"<<Qt::endl;
            return;
        }
    }
    pathSet.insert(filePath); //记录文件夹路径
    //构造顶层节点
    ProjectTreeItem *item=new ProjectTreeItem(this,name,filePath,TreeItemProject);
    item->setData(0,Qt::DisplayRole,name);
    item->setData(0,Qt::DecorationRole,QIcon(":/resource/icon/dir.png"));
    item->setData(0,Qt::ToolTipRole,filePath);
}

void ProjectTreeWidget::slotOpenProject(const QString &importPath)
{
    if(pathSet.find(importPath)!=pathSet.end())
    {
        qDebug()<<"项目已经打开,不要重复打开";
        return;
    }
    pathSet.insert(importPath);
    QDir dir(importPath);
    int fileCount=0;
    openProgressDialog = new QProgressDialog(this);
    threadOpenTree=QSharedPointer<OpenProjectThread>(new OpenProjectThread(std::ref(importPath),std::ref(fileCount),this,nullptr));
    threadOpenTree->start();

    connect(threadOpenTree.get(), &OpenProjectThread::updateProgress, this,&ProjectTreeWidget::slotOpenUpdateProgress);
    connect(threadOpenTree.get(), &OpenProjectThread::finishedProgress,this,&ProjectTreeWidget::slotOpenFinished);
    connect(openProgressDialog, &QProgressDialog::canceled,this,&ProjectTreeWidget::slotOpenCancleProgress);
    connect(this,&ProjectTreeWidget::signalOpenCancleProgress, threadOpenTree.get(),&OpenProjectThread::slotOpenCancleProgress);

    openProgressDialog->setWindowTitle(tr("请等待文件加载完毕"));
    openProgressDialog->setFixedWidth(PROGRESS_LENGTH);
    openProgressDialog->setRange(0,PROGRESS_LENGTH);
    openProgressDialog->exec();
}

void ProjectTreeWidget::slotPreviousShow()
{
    if(mySelectedItem==nullptr)
    {
        return;
    }
    ProjectTreeItem *currentItem=dynamic_cast<ProjectTreeItem *>(mySelectedItem)->getPreviousItem();
    if(currentItem==nullptr)
    {
        return;
    }
    emit signalUpdatePicture(currentItem->getPath()); //发射信号通知PictureShow更换图片
    mySelectedItem=currentItem; //更新当前选中图片
    this->setCurrentItem(currentItem); //高亮显示当前项
}

void ProjectTreeWidget::slotNextShow()
{
    if(mySelectedItem==nullptr)
    {
        return;
    }
    ProjectTreeItem *currentItem=dynamic_cast<ProjectTreeItem *>(mySelectedItem)->getNextItem();
    if(currentItem==nullptr)
    {
        return;
    }
    //获取到下一个项的路径
    emit signalUpdatePicture(currentItem->getPath()); //发射信号通知PictureShow更换图片
    mySelectedItem=currentItem;
    this->setCurrentItem(currentItem); //高亮显示当前项
}

void ProjectTreeWidget::slotSetMusic()
{
    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setWindowTitle(tr("选择音频文件"));
    fileDialog.setDirectory(QDir::currentPath());
    fileDialog.setViewMode(QFileDialog::Detail);
    fileDialog.setNameFilter("MP3文件(*.mp3)");
    QStringList fileNames;
    if(fileDialog.exec())
    {
        fileNames=fileDialog.selectedFiles();
    }
    if(fileNames.size()<=0)
    {
        return;
    }
    QString filePath=fileNames.at(0);
    qDebug()<<filePath;
    player->setSource(QUrl::fromLocalFile(filePath));
}

void ProjectTreeWidget::slotStartMusic()
{
    player->play();
}

void ProjectTreeWidget::slotStopMusic()
{
    player->stop();
}

//弹出右键菜单
void ProjectTreeWidget::slotItemPressed(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column)
    //如果点击的是鼠标右键
    if(QGuiApplication::mouseButtons()==Qt::RightButton)
    {
        QMenu menu(this); //创建右键菜单
        int itemType=item->type();
        //如果是项目节点,弹出右键菜单
        if(itemType==TreeItemProject)
        {
            rightButtonItem=item; //记录右键点击的项
            menu.addAction(actImport);
            menu.addAction(actCloseProject);
            menu.addAction(actSetActive);
            menu.addAction(actSlideShow);
            menu.exec(QCursor::pos());
        }
    }
}

void ProjectTreeWidget::slotDoubleClickedItem(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column)
    if(QGuiApplication::mouseButtons()==Qt::LeftButton)
    {
        ProjectTreeItem *doubleItem=dynamic_cast<ProjectTreeItem *>(item);
        if(!doubleItem)
        {
            return;
        }
        int itemType=doubleItem->type();
        if(itemType==TreeItemPicture)
        {
            //发射信号去通知pictureshow加载图片
            emit signalUpdateSelected(doubleItem->getPath());
            mySelectedItem=doubleItem; //记录当前选中的项
        }
    }
}

void ProjectTreeWidget::slotImport()
{
    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::Directory);
    fileDialog.setWindowTitle(tr("请选择要导入的文件夹"));
    QString path=""; //目的路径
    //其实这里也是不需要判断,毕竟我们rightButtonItem记录了根目录,如果根目录无效
    //那么就无法点击,也不会触发slotImport这个槽函数
    if(rightButtonItem==nullptr)
    {
        qDebug()<<"右键选择项为空";
        return;
    }
    path=dynamic_cast<ProjectTreeItem *>(rightButtonItem)->getPath(); //获取选择项目所在的路径
    fileDialog.setDirectory(path); //设置当前打开路径
    fileDialog.setViewMode(QFileDialog::Detail);
    QStringList fileNames; //记录打开的文件夹名字
    if(fileDialog.exec())
    {
        fileNames=fileDialog.selectedFiles();
    }
    if(fileNames.size()<=0)
    {
        qDebug()<<"没有导入任何文件夹";
        return;
    }
    //其实就打开了一个,所以就获取第一个
    QString importPath=fileNames.at(0);
    int fileCount=0; //初始化文件数目,便于进入函数统计
    progressDialog=new QProgressDialog(this); //创建一个进度条对话框,显示文件复制的进度
    //复制文件是一个耗时的操作,所以我们创建线程去执行文件复制的操作,避免主线程阻塞造成界面卡顿
    threadCreateTree=QSharedPointer<ProjectTreeThread>(new ProjectTreeThread(std::ref(importPath),std::ref(path),std::ref(fileCount),this,rightButtonItem,rightButtonItem,nullptr));
    threadCreateTree->start();
    connect(threadCreateTree.get(),&ProjectTreeThread::updateProgress,this,&ProjectTreeWidget::slotUpdateProgress);
    connect(threadCreateTree.get(),&ProjectTreeThread::finishProgress,this,&ProjectTreeWidget::slotFinished);

    //文件复制过程中取消复制
    //进度条对话框发出取消信号,treewidget删除对话框再去通知线程停止
    //进度条直接通知线程,进度条对话框删除不能实现,treewidget是二者通信的桥梁
    connect(progressDialog,&QProgressDialog::canceled,this,&ProjectTreeWidget::slotCancleProgress);
    connect(this,&ProjectTreeWidget::signalCancleProgress,threadCreateTree.get(),&ProjectTreeThread::slotCancleProgress);
    progressDialog->setWindowTitle(tr("请等待文件加载完毕"));
    progressDialog->setFixedWidth(PROGRESS_LENGTH);
    progressDialog->setRange(0,PROGRESS_LENGTH);
    progressDialog->exec();
}

//创建项目对话框更新槽函数
void ProjectTreeWidget::slotUpdateProgress(int fileCount)
{
    if(!progressDialog)
    {
        qDebug()<<"文件导入对话框异常";
        return;
    }
    if(fileCount>=PROGRESS_MAX)
    {
        progressDialog->setValue(fileCount%PROGRESS_MAX);
    }
    else
    {
        progressDialog->setValue(fileCount);
    }
}

void ProjectTreeWidget::slotFinished(int fileCount)
{
    // Q_UNUSED(fileCount)
    qDebug()<<"导入的文件数目"<<fileCount;
    progressDialog->setValue(PROGRESS_MAX); //设置为最大值
    progressDialog->deleteLater();
}

void ProjectTreeWidget::slotCancleProgress()
{
    emit signalCancleProgress(); //通知线程停止
    delete progressDialog;
    progressDialog=nullptr;
}

//设置活动项目槽函数
void ProjectTreeWidget::slotSetActive()
{
    //就是字体加粗,表示当前项目是活动的
    //右键项目无效
    if(!rightButtonItem)
    {
        return;
    }
    QFont commonFont;
    commonFont.setBold(false);

    if(activeItem) //存在活动项
    {
        qDebug()<<"当前存在活动项目";
        //当前活动项字体设置普通
        activeItem->setFont(0,commonFont);
    }
    activeItem=rightButtonItem; //记录当前活动项
    commonFont.setBold(true);
    activeItem->setFont(0,commonFont);
}

void ProjectTreeWidget::slotCloseProject()
{
    RemoveProjectDialog dialog;
    int result=dialog.exec();
    //获取对话框的返回结果,判断按下的按钮
    if(result!=QDialog::Accepted)
    {
        return;
    }
    bool removeLocal=dialog.isRemoveFromLocal(); //判断复选框是否按下
    if(!rightButtonItem)
    {
        return;
    }
    int index=this->indexOfTopLevelItem(rightButtonItem); //获取右键项目的索引
    ProjectTreeItem *selectedItem=dynamic_cast<ProjectTreeItem *>(mySelectedItem);
    QString deletePath=dynamic_cast<ProjectTreeItem *>(rightButtonItem)->getPath();
    pathSet.remove(deletePath); //在集合中删除右键项目的路径
    if(removeLocal) //复选框按下递归的删除本地文件
    {
        QDir deleteDir(deletePath);
        deleteDir.removeRecursively();
    }
    //当前被删除的项目是活动项目
    if(activeItem==rightButtonItem)
    {
        activeItem=nullptr;
    }
    //关闭项目时,判断Pictureshow展示图片是否和被关闭项目是同一目录
    ProjectTreeItem *rightItem=dynamic_cast<ProjectTreeItem *>(rightButtonItem);
    if(selectedItem && rightItem==selectedItem->getRoot())
    {
        //选中项有效且选中项在被关闭根目录
        selectedItem=nullptr;
        mySelectedItem=nullptr;
        emit signalClearPicture(); //发射信号通知PictureShow清空图片
    }
    delete this->takeTopLevelItem(index); //在目录树中删除
    rightButtonItem=nullptr; //右键项目置空,防止野指针
    rightItem=nullptr;
}

void ProjectTreeWidget::slotOpenUpdateProgress(int fileCount)
{
    if(!openProgressDialog)
    {
        qDebug()<<"文件导入对话框异常";
        return;
    }
    if(fileCount>=PROGRESS_MAX)
    {
        openProgressDialog->setValue(fileCount%PROGRESS_MAX);
    }
    else
    {
        openProgressDialog->setValue(fileCount);
    }
}

void ProjectTreeWidget::slotOpenFinished(int fileCount)
{
    Q_UNUSED(fileCount)
    if(!openProgressDialog)
    {
        return;
    }
    openProgressDialog->setValue(PROGRESS_MAX);
    openProgressDialog->deleteLater();
}

void ProjectTreeWidget::slotOpenCancleProgress()
{
    emit signalOpenCancleProgress();
    delete openProgressDialog;
    openProgressDialog=nullptr;
}

void ProjectTreeWidget::slotSlideShow()
{
    if(rightButtonItem==nullptr)
    {
        return;
    }
    ProjectTreeItem *firstChild=dynamic_cast<ProjectTreeItem *>(rightButtonItem)->getFirstPictureChild();
    if(firstChild==nullptr)
    {
        return;
    }
    ProjectTreeItem *lastChild=dynamic_cast<ProjectTreeItem *>(rightButtonItem)->getLastPictureChild();
    if(lastChild==nullptr)
    {
        return;
    }
    qDebug()<<"firstChild"<<firstChild->data(0,Qt::DisplayRole).toString();
    qDebug()<<"lastChild"<<lastChild->data(0,Qt::DisplayRole).toString();
    slideDialog=QSharedPointer<SlideShowDialog>(new SlideShowDialog(firstChild,lastChild,this));
    slideDialog->setModal(true); //设置模态对话框
    slideDialog->showMaximized();
}
