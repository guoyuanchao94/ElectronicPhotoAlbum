#ifndef PROJECTTREEITEM_H
#define PROJECTTREEITEM_H

#include <QTreeWidgetItem>
#include <QTreeWidget>

class ProjectTreeItem : public QTreeWidgetItem
{
public:
    //顶层节点,父对象是treewidget
    ProjectTreeItem(QTreeWidget *treeview, const QString &name, const QString &path, int type = Type);
    //父对象,名字,隐含路径,根节点,类型
    //根节点,可能嵌套了好几层,记录一个根节点,递归创建文件
    //使用父类指针,更通用一些
    ProjectTreeItem(QTreeWidgetItem *parent, const QString &name, const QString &path,QTreeWidgetItem *root,int type=Type);
public:
    const QString &getPath(); //获取当前节点路径
    QTreeWidgetItem *getRoot(); //获取根节点
    void setPreviousItem(QTreeWidgetItem *item);
    void setNextItem(QTreeWidgetItem *item);
    ProjectTreeItem *getPreviousItem();
    ProjectTreeItem *getNextItem();
    ProjectTreeItem *getFirstPictureChild();
    ProjectTreeItem *getLastPictureChild();
private:
    QString itemName;
    QString itemPath;
    QTreeWidgetItem *itemRoot;
    QTreeWidgetItem *previousItem;
    QTreeWidgetItem *nextItem;
};

#endif // PROJECTTREEITEM_H
