#include "previewlistwidget.h"
#include "projecttreeitem.h"
#include "const.h"
#include <QPixmap>
#include <QPainter>
#include "previewlistitem.h"
#include <QGuiApplication>
PreviewListWidget::PreviewListWidget(QWidget *parent)
    :QListWidget(parent),mapItems{},globalCount(0),lastIndex(14),originPoint(0,0)
{
    //设置listwidget的展示模式
    setViewMode(QListWidget::IconMode);
    // IconMode模式下拖拽是free,我们禁止拖拽
    setDragDropMode(QAbstractItemView::NoDragDrop);
    setIconSize(QSize(PREVIEWICON_SIZE,PREVIEWICON_SIZE)); //设置图片大小
    setSpacing(5); //设置图片间隔
    connect(this,&PreviewListWidget::itemPressed,this,&PreviewListWidget::slotItemPressed);
}

PreviewListWidget::~PreviewListWidget()
{

}

//绘制图片
void PreviewListWidget::addItemToList(const QString &path)
{
    QPixmap sourcePixmap;
    sourcePixmap.load(path);
    sourcePixmap=sourcePixmap.scaled(PREVIEWICON_SIZE,PREVIEWICON_SIZE,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    int width=sourcePixmap.width();
    int height=sourcePixmap.height();
    QPixmap destinationPixmap(QSize(PREVIEWICON_SIZE,PREVIEWICON_SIZE));
    int destinationWidth=destinationPixmap.width();
    int destinationHeight=destinationPixmap.height();

    destinationPixmap.fill(QColor(220,220,220,50));
    QPainter painter(&destinationPixmap);
    int x=(destinationWidth-width)/2;
    int y=(destinationHeight-height)/2;

    painter.drawPixmap(x,y,sourcePixmap);
    globalCount++;
    PreviewListItem *item=new PreviewListItem(destinationPixmap,path,globalCount,this);
    item->setSizeHint(QSize(PREVIEW_SIZE,PREVIEW_SIZE)); //调整项目大小
    this->addItem(item); //添加项目
    mapItems[path]=item; //记录项目
    if(globalCount==1)
    {
        // 记录第一张预览图在其父窗口的位置
        originPoint=this->pos();
    }
}

void PreviewListWidget::slotUpdatePreview(QTreeWidgetItem *item)
{
    if(!item)
    {
        return;
    }
    ProjectTreeItem *projectItem=dynamic_cast<ProjectTreeItem *>(item);
    QString path=projectItem->getPath(); //获取路径
    if(mapItems.find(path)!=mapItems.end())
    {
        //路径已经存在,无需重复添加
        return;
    }
    addItemToList(path);
}

void PreviewListWidget::slotUpdateSelected(QTreeWidgetItem *item)
{
    //setPixmap传递过来的当前播放的图片的项
    if(!item)
    {
        return;
    }
    ProjectTreeItem *projectItem=dynamic_cast<ProjectTreeItem *>(item);
    QString path=projectItem->getPath(); //获取路径
    if(mapItems.find(path)==mapItems.end())
    {
        return;
    }
    PreviewListItem *listItem=dynamic_cast<PreviewListItem *>(mapItems.find(path).value());
    int index=listItem->getIndex(); //获取当前项的索引
    //全屏状态下容纳14张图片
    if(index>14)
    {
        QPoint currentPoint=this->pos();
        // 如果索引大于14说明该item在屏幕外,向左移动(当前索引前去上一次索引的插值)*100
        this->move(currentPoint.x()-(index-lastIndex)*PREVIEW_SIZE,this->pos().y());
        lastIndex=index; //更新最后一个索引
    }
    else
    {
        this->move(originPoint);
        lastIndex=14;
    }
    this->setCurrentItem(mapItems.find(path).value());
}

void PreviewListWidget::slotItemPressed(QListWidgetItem *item)
{
    if(QGuiApplication::mouseButtons()!=Qt::LeftButton)
    {
        return;
    }
    PreviewListItem *previewItem=dynamic_cast<PreviewListItem *>(item);
    // int index=previewItem->getIndex();
    QString path=previewItem->getText();
    this->setCurrentItem(item);
    emit signalUpdateSelectedShow(path); //通知pictureanimationWidget更换图片
}
