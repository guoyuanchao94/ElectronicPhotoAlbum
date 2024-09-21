#include "projecttreethread.h"
#include <QDir>
#include "projecttreeitem.h"
#include "const.h"
ProjectTreeThread::ProjectTreeThread(QObject *parent)
    :QThread(parent)
{

}

ProjectTreeThread::ProjectTreeThread(const QString &sourcePath, const QString &destinationPath, int &fileCount, QTreeWidget *treeview, QTreeWidgetItem *parentItem,QTreeWidgetItem *root, QObject *parent)
    :QThread(parent),mySourcePath(sourcePath),myDestinationPath(destinationPath),
    myFileCount(fileCount),myTreeview(treeview),myParentItem(parentItem),
    myRoot(root),isStop(false),globalPreviousItem(nullptr)
{

}

ProjectTreeThread::~ProjectTreeThread()
{
    qDebug()<<"线程退出"<<Qt::endl;
}

void ProjectTreeThread::run()
{
    createProjectTree(mySourcePath,myDestinationPath,myFileCount,myTreeview,myParentItem,myRoot,nullptr);
    //创建过程中取消创建
    if(isStop)
    {
        //获取根节点的路径
        QString path=dynamic_cast<ProjectTreeItem *>(myRoot)->getPath();
        int index=myTreeview->indexOfTopLevelItem(myRoot); //获取索引便于删除
        delete myTreeview->takeTopLevelItem(index);
        QDir dir(path);
        dir.removeRecursively();
        return;
    }
    emit finishProgress(myFileCount);
}

void ProjectTreeThread::createProjectTree(const QString &sourcePath, const QString &destinationPath, int &fileCount, QTreeWidget *treeview, QTreeWidgetItem *parentItem, QTreeWidgetItem *root, QTreeWidgetItem *previousItem)
{
    // 线程退出, 我们及时结束操作
    if (isStop)
    {
        return;
    }

    bool needCopy = true;
    if (sourcePath == destinationPath)
    {
        qDebug() << "源路径和目的路径相同, 不需要拷贝";
        needCopy = false;
    }

    QDir importPath(sourcePath); // 获取导入路径
    importPath.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    importPath.setSorting(QDir::Name);
    QFileInfoList infoList = importPath.entryInfoList();

    for (int i = 0; i < infoList.size(); ++i)
    {
        // 为了保证线程及时退出
        if (isStop)
        {
            return;
        }

        QFileInfo fileInfo = infoList.at(i);

        if (fileInfo.isDir())
        {
            qDebug() << fileInfo.fileName() << fileInfo.filePath();
            qDebug() << "文件夹继续遍历";

            if (isStop)
            {
                return;
            }

            fileCount++; // 文件夹也要更新数量
            emit updateProgress(fileCount); // 发射信号更新进度条

            QDir destinationDir(destinationPath); // 构造目的路径
            // 构造源路径当前文件夹对应的目的路径文件夹
            QString subDestinationPath = destinationDir.absoluteFilePath(fileInfo.fileName());
            QDir subDestinationDir(subDestinationPath);
            if (!subDestinationDir.exists())
            {
                bool ok = subDestinationDir.mkpath(subDestinationPath);
                if (!ok)
                {
                    continue;
                }
            }
            ProjectTreeItem *item = new ProjectTreeItem(parentItem, fileInfo.fileName(), subDestinationPath, root, TreeItemDirectory);
            item->setData(0, Qt::DisplayRole, fileInfo.fileName());
            item->setData(0, Qt::DecorationRole, QIcon(":/resource/icon/dir.png"));
            item->setData(0, Qt::ToolTipRole, subDestinationPath);

            // parentItem 变成了新构造的文件夹
            createProjectTree(fileInfo.absoluteFilePath(), subDestinationPath, fileCount, treeview, item, root, globalPreviousItem);
        }
        else
        {
            if (isStop)
            {
                return;
            }

            QString suffix = fileInfo.completeSuffix();
            if (suffix != "png" && suffix != "jpg" && suffix != "jpeg" && suffix != "webp")
            {
                qDebug() << "文件后缀不符, 取消拷贝";
                continue;
            }

            fileCount++;
            emit updateProgress(fileCount); // 发射信号通知对话框进度条更新
            if (!needCopy)
            {
                qDebug() << "不需要拷贝";
                continue;
            }
            QDir destinationDir(destinationPath);
            QString destinationFilePath = destinationDir.absoluteFilePath(fileInfo.fileName());
            if (!QFile::copy(fileInfo.absoluteFilePath(), destinationFilePath))
            {
                qDebug() << "文件拷贝失败";
                continue;
            }
            ProjectTreeItem *item = new ProjectTreeItem(parentItem, fileInfo.fileName(), destinationFilePath, root, TreeItemPicture);
            item->setData(0, Qt::DisplayRole, fileInfo.fileName());
            item->setData(0, Qt::DecorationRole, QIcon(":/resource/icon/pic.png"));
            item->setData(0, Qt::ToolTipRole, destinationFilePath);

            if (globalPreviousItem)
            {
                ProjectTreeItem *preProjectItem = dynamic_cast<ProjectTreeItem *>(globalPreviousItem);
                preProjectItem->setNextItem(item);
            }
            item->setPreviousItem(globalPreviousItem); // 构造双向链表
            globalPreviousItem = item; // 更新前继节点
        }
    }
    emit finishProgress(fileCount);
}

void ProjectTreeThread::slotCancleProgress()
{
    this->isStop=true;
}
