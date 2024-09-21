#ifndef PROJECTTREETHREAD_H
#define PROJECTTREETHREAD_H

#include <QThread>
#include <QObject>
#include <QTreeWidgetItem>
class ProjectTreeThread : public QThread
{
    Q_OBJECT
public:
    explicit ProjectTreeThread(QObject *parent=nullptr);
    ProjectTreeThread(const QString &sourcePath, const QString &destinationPath,int &fileCount,QTreeWidget *treeview,QTreeWidgetItem *parentItem,QTreeWidgetItem *root,QObject *parent=nullptr);
    ~ProjectTreeThread() override;
protected:
    void run() override;
private:
    void createProjectTree(const QString &sourcePath, const QString &destinationPath,int &fileCount,QTreeWidget *treeview,QTreeWidgetItem *parentItem,QTreeWidgetItem *root,QTreeWidgetItem *previousItem=nullptr);
private:
    QString mySourcePath; //源路径
    QString myDestinationPath; //目的路径
    int myFileCount; //文件数目,用于更新外界的进度条
    QTreeWidget *myTreeview; //当前节点所在的treewidget
    QTreeWidgetItem *myParentItem; //当前节点的父节点
    QTreeWidgetItem *myRoot; //当前节点的根节点
    bool isStop; //线程停止状态,控制线程退出
    QTreeWidgetItem *globalPreviousItem; //记录递归前继节点
public slots:
    void slotCancleProgress();
signals:
    void updateProgress(int);
    void finishProgress(int);
};

#endif // PROJECTTREETHREAD_H
