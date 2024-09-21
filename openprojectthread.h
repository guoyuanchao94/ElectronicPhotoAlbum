#ifndef OPENPROJECTTHREAD_H
#define OPENPROJECTTHREAD_H

#include <QThread>
#include <QObject>
#include <QTreeWidget>
#include <QTreeWidgetItem>
class OpenProjectThread : public QThread
{
    Q_OBJECT
public:
    explicit OpenProjectThread(QThread *parent=nullptr);
    OpenProjectThread(const QString &importPath, int &fileCount, QTreeWidget *treeview, QObject *parent = nullptr);

public:
    ~OpenProjectThread();
public:
    void openProject(const QString &importPath, int &fileCount, QTreeWidget *treeView);
protected:
    void run() override;
private:
    void recursiveProjectTree(const QString &importPath, int &fileCount,
                            QTreeWidget *treeView,QTreeWidgetItem *parentItem,
                            QTreeWidgetItem *root,QTreeWidgetItem *previousItem);
private:
    QString myImportPath;
    int myFileCount;
    QTreeWidget *myTreeview;
    QTreeWidgetItem *root;
    bool isStop;
    QTreeWidgetItem *globalPreviousItem; //记录前继节点
public slots:
    void slotOpenCancleProgress();
signals:
    void updateProgress(int fileCount);
    void finishedProgress(int fileCount);
};

#endif // OPENPROJECTTHREAD_H
