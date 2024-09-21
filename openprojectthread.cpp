#include <QDir>
#include "projecttreeitem.h"
#include "const.h"
#include "openprojectthread.h"
OpenProjectThread::OpenProjectThread(QThread *parent)
    :QThread(parent)
{

}

OpenProjectThread::OpenProjectThread(const QString &importPath, int &fileCount, QTreeWidget *treeview, QObject *parent)
    :QThread(parent),myImportPath(importPath),myFileCount(fileCount),
    myTreeview(treeview),root(nullptr),isStop(false),globalPreviousItem(nullptr)
{

}

void OpenProjectThread::openProject(const QString &importPath, int &fileCount, QTreeWidget *treeview)
{
    QDir dir(importPath);
    QString dirName=dir.dirName();
    ProjectTreeItem *item=new ProjectTreeItem(treeview,dirName,importPath,TreeItemProject);
    item->setData(0,Qt::DisplayRole,dirName);
    item->setData(0,Qt::DecorationRole,QIcon(":/resource/icon/dir.png"));
    item->setData(0,Qt::ToolTipRole,importPath);
    root = item;
    recursiveProjectTree(importPath,fileCount,treeview,item,root,nullptr);
}

void OpenProjectThread::recursiveProjectTree(const QString &importPath, int &fileCount, QTreeWidget *treeview, QTreeWidgetItem *parentItem, QTreeWidgetItem *root, QTreeWidgetItem *previousItem)
{
    QDir dir(importPath);
    dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    dir.setSorting(QDir::Name);
    QFileInfoList infoList = dir.entryInfoList();

    for (int i = 0; i < infoList.size(); ++i)
    {
        if (isStop)
        {
            return;
        }

        QFileInfo fileInfo = infoList.at(i);

        if(fileInfo.isDir())
        {
            if(isStop)
            {
                return;
            }
            qDebug() << "当前是文件夹, 继续添加";
            fileCount++;
            emit updateProgress(fileCount); // 发射信号更新进度条

            ProjectTreeItem *item = new ProjectTreeItem(parentItem, fileInfo.fileName(), fileInfo.absoluteFilePath(), root, TreeItemDirectory);
            item->setData(0, Qt::DisplayRole, fileInfo.fileName());
            item->setData(0, Qt::DecorationRole, QIcon(":/resource/icon/dir.png"));
            item->setData(0, Qt::ToolTipRole, fileInfo.absoluteFilePath());

            // 递归处理子文件夹
            recursiveProjectTree(fileInfo.absoluteFilePath(), fileCount, treeview, item, root, globalPreviousItem);
        }
        else
        {
            if(isStop)
            {
                return;
            }

            QString suffix = fileInfo.completeSuffix();
            if (suffix != "png" && suffix != "jpg" && suffix != "jpeg")
            {
                continue;
            }

            fileCount++;
            emit updateProgress(fileCount);

            ProjectTreeItem *item = new ProjectTreeItem(parentItem, fileInfo.fileName(), fileInfo.absoluteFilePath(), root, TreeItemPicture);
            item->setData(0, Qt::DisplayRole, fileInfo.fileName());
            item->setData(0, Qt::DecorationRole, QIcon(":/resource/icon/pic.png"));
            item->setData(0, Qt::ToolTipRole, fileInfo.absoluteFilePath());

            if (globalPreviousItem)
            {
                ProjectTreeItem *previousProjectItem = dynamic_cast<ProjectTreeItem *>(globalPreviousItem);
                previousProjectItem->setNextItem(item);
            }
            item->setPreviousItem(globalPreviousItem);
            globalPreviousItem = item; // 更新为当前图片文件项
        }
    }
    emit finishedProgress(fileCount);
}

OpenProjectThread::~OpenProjectThread()
{
    qDebug()<<"打开项目线程退出";
}

void OpenProjectThread::run()
{
    openProject(myImportPath,myFileCount,myTreeview);
    if(isStop)
    {
        QString path=dynamic_cast<ProjectTreeItem *>(root)->getPath();
        auto index=myTreeview->indexOfTopLevelItem(root);
        delete myTreeview->takeTopLevelItem(index);
        QDir dir(path);
        dir.removeRecursively();
        return;
    }
    emit finishedProgress(myFileCount);
    qDebug()<<"文件数目"<<myFileCount;
}

void OpenProjectThread::slotOpenCancleProgress()
{
    isStop=true;
}
