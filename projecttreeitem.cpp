#include "projecttreeitem.h"
#include "const.h"
//创建顶层节点,自己就是根节点
ProjectTreeItem::ProjectTreeItem(QTreeWidget *treeview, const QString &name, const QString &path, int type)
    :QTreeWidgetItem(treeview,type),itemName(name),itemPath(path),
    itemRoot(this),previousItem(nullptr),nextItem(nullptr)
{
    // qDebug()<<"ProjectItem first";
}

//初始不知道前向节点和后置节点
//创建根节点下面的子节点
ProjectTreeItem::ProjectTreeItem(QTreeWidgetItem *parent, const QString &name, const QString &path, QTreeWidgetItem *root, int type)
    :QTreeWidgetItem(parent,type),itemName(name),itemPath(path),
    itemRoot(root),previousItem(nullptr),nextItem(nullptr)
{
    // qDebug()<<"ProjectItem second";
}

const QString &ProjectTreeItem::getPath()
{
    return itemPath;
}

QTreeWidgetItem *ProjectTreeItem::getRoot()
{
    return itemRoot;
}

void ProjectTreeItem::setPreviousItem(QTreeWidgetItem *item)
{
    previousItem=item;
}

void ProjectTreeItem::setNextItem(QTreeWidgetItem *item)
{
    nextItem=item;
}

ProjectTreeItem *ProjectTreeItem::getPreviousItem()
{
    return dynamic_cast<ProjectTreeItem *>(previousItem);
}

ProjectTreeItem *ProjectTreeItem::getNextItem()
{
    return dynamic_cast<ProjectTreeItem *>(nextItem);
}

ProjectTreeItem *ProjectTreeItem::getFirstPictureChild()
{
    //图片节点没有子节点
    if(this->type()==TreeItemPicture)
    {
        return nullptr;
    }
    int count=this->childCount(); //直接子节点数量,不包括孙子节点什么的
    if(count==0)
    {
        return nullptr;
    }
    for(int i=0;i<count;--i)
    {
        ProjectTreeItem *currentItem=dynamic_cast<ProjectTreeItem *>(this->child(i));
        if(currentItem->type()==TreeItemPicture)
        {
            return currentItem;
        }
        currentItem=currentItem->getFirstPictureChild();
        if(!currentItem)
        {
            continue;
        }
        return currentItem;
    }
    return nullptr;
}

ProjectTreeItem *ProjectTreeItem::getLastPictureChild()
{
    //图片节点没有子节点
    if(this->type()==TreeItemPicture)
    {
        return nullptr;
    }
    int count=this->childCount();
    if(count==0)
    {
        return nullptr;
    }
    for(int i=count-1;i>=0;--i)
    {
        ProjectTreeItem *currentItem=dynamic_cast<ProjectTreeItem *>(this->child(i));
        if(currentItem->type()==TreeItemPicture)
        {
            return currentItem;
        }
        currentItem=currentItem->getLastPictureChild();
        if(!currentItem)
        {
            continue;
        }
        return currentItem;
    }
    return nullptr;
}
