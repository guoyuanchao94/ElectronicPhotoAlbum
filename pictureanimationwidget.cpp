#include "pictureanimationwidget.h"
#include "projecttreeitem.h"
#include <QPainter>
PictureAnimationWidget::PictureAnimationWidget(QWidget *parent)
    : QWidget{parent},myFactor(0.0),currentItem(nullptr),isStart(false)
{
    timer=new QTimer(this);
    connect(timer,&QTimer::timeout,this,&PictureAnimationWidget::slotTimeOut);
}

PictureAnimationWidget::~PictureAnimationWidget()
{
    timer->stop();
}

void PictureAnimationWidget::setPixmap(QTreeWidgetItem *item)
{
    // slideshowdialog构造函数传递了第一个要加载的图片item
    if(!item)
    {
        return;
    }
    ProjectTreeItem *treeItem=dynamic_cast<ProjectTreeItem *>(item);
    QString path=treeItem->getPath(); //获取图片路径
    pixmapFirst.load(path);
    currentItem=treeItem; //记录当前播放的图片项
    //如果当前图片没有被播放过,则记录到map里
    if(mapItems.find(path)==mapItems.end())
    {
        mapItems[path]=treeItem;
        emit signalUpdatePreview(item); //通知预览窗口更新预览图
    }
    emit signalSelectedItem(item); //通知下方预览图选中效果
    ProjectTreeItem *nextTreeItem=treeItem->getNextItem();
    if(nextTreeItem==nullptr)
    {
        return;
    }
    QString nextPath=nextTreeItem->getPath();
    pixmapSecond.load(nextPath); //双缓冲绘图
    if(mapItems.find(nextPath)==mapItems.end())
    {
        mapItems[nextPath]=nextTreeItem;
        //更新预览列表逻辑
        emit signalUpdatePreview(nextTreeItem);
    }
}

//开启动画
void PictureAnimationWidget::Start()
{
    emit signalStatr(); //通知右上方按钮是播放还是暂停状态
    emit signalStartMusic();
    myFactor=0.0;
    timer->start(25);
    isStart=true;
}

void PictureAnimationWidget::Stop()
{
    emit signalStop();
    emit signalStopMusic();
    timer->stop();
    myFactor=0.0;
    isStart=false;
}

//播放前一张幻灯片
void PictureAnimationWidget::slidePrevious()
{
    Stop();//停止幻灯片播放
    if(!currentItem)
    {
        return;
    }
    ProjectTreeItem *projectItem=dynamic_cast<ProjectTreeItem *>(currentItem);
    ProjectTreeItem *previousItem=projectItem->getPreviousItem();
    if(!previousItem)
    {
        return;
    }
    setPixmap(previousItem);
    update(); //刷新paintEvent事件
}

void PictureAnimationWidget::slideNext()
{
    Stop();//停止幻灯片播放
    if(!currentItem)
    {
        return;
    }
    ProjectTreeItem *projectItem=dynamic_cast<ProjectTreeItem *>(currentItem);
    ProjectTreeItem *nextItem=projectItem->getNextItem();
    if(!nextItem)
    {
        return;
    }
    setPixmap(nextItem);
    update();
}

void PictureAnimationWidget::paintEvent(QPaintEvent *event)
{
    //首先判断pixmap是否有内容
    if(pixmapFirst.isNull())
    {
        return;
    }
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    QRect rect=geometry(); //获取绘制区域
    int myWidth=rect.width();
    int myHeight=rect.height();
    // qDebug()<<"myWidth= "<<myWidth;
    // qDebug()<<"myHeight= "<<myHeight;

    //将图片拉伸到画布的矩形区域
    pixmapFirst=pixmapFirst.scaled(myWidth,myHeight,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    int alpha=255*(1.0-myFactor); //myFactor会逐渐增大,alpha逐渐减小
    QPixmap alphaPixmap(pixmapFirst.size());
    alphaPixmap.fill(Qt::transparent); //透明色,相当于QColor(0,0,0,0);

    QPainter painterImage(&alphaPixmap); //在透明图绘制
    painterImage.setCompositionMode(QPainter::CompositionMode_Source);
    painterImage.drawPixmap(0,0,pixmapFirst); //在alphaPixmap的起始位置绘制pixmapFirst
    painterImage.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    painterImage.fillRect(alphaPixmap.rect(),QColor(0,0,0,alpha));
    painterImage.end();

    int x=(myWidth-pixmapFirst.width())/2;
    int y=(myHeight-pixmapFirst.height())/2;
    painter.drawPixmap(x,y,alphaPixmap);

    if(pixmapSecond.isNull())
    {
        return;
    }

    pixmapSecond=pixmapSecond.scaled(myWidth,myHeight,Qt::KeepAspectRatio);
    alpha=255*myFactor;
    QPixmap alphaPixmapSecond(pixmapSecond.size());
    alphaPixmapSecond.fill(Qt::transparent);
    QPainter painterSecond(&alphaPixmapSecond);
    painterSecond.setCompositionMode(QPainter::CompositionMode_Source);
    painterSecond.drawPixmap(0,0,pixmapSecond);
    painterSecond.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    painterSecond.fillRect(alphaPixmapSecond.rect(),QColor(0,0,0,alpha));
    painterSecond.end();
    x=(myWidth-pixmapSecond.width())/2;
    y=(myHeight-pixmapSecond.height())/2;
    painter.drawPixmap(x,y,alphaPixmapSecond);
}

void PictureAnimationWidget::updateSelected(QTreeWidgetItem *item)
{
    if(!item)
    {
        return;
    }
    ProjectTreeItem *projectItem=dynamic_cast<ProjectTreeItem *>(item);
    QString path=projectItem->getPath();
    pixmapFirst.load(path);
    currentItem=projectItem;
    if(mapItems.find(path)==mapItems.end())
    {
        mapItems[path]=projectItem;
    }
    ProjectTreeItem *nextItem=projectItem->getNextItem();
    if(!nextItem)
    {
        return;
    }
    QString nextPath=projectItem->getPath();
    pixmapSecond.load(nextPath);
    if(mapItems.find(nextPath)==mapItems.end())
    {
        mapItems[nextPath]=projectItem;
    }
}


void PictureAnimationWidget::slotUpdateSelectedShow(QString path)
{
    //判断路径是否存在
    if(mapItems.find(path)==mapItems.end())
    {
        return;
    }
    //路径存在更新界面
    updateSelected(mapItems.find(path).value());
    update();//paintEvent
}

void PictureAnimationWidget::slotStartOrPause()
{
    if(!isStart)
    {
        myFactor=0;
        timer->start(25);
        isStart=true;
        emit signalStartMusic(); //发射播放音乐信号
    }
    else
    {
        timer->stop();
        myFactor=0;
        update();
        isStart=false;
        emit signalStopMusic();
    }
}

void PictureAnimationWidget::slotTimeOut()
{
    if(currentItem==nullptr)
    {
        Stop(); //暂停播放
        update(); //刷新界面
        return;
    }
    myFactor+=0.01;
    //当前图片播放完毕,开始播放下一张图片
    if(myFactor>=1)
    {
        myFactor=0;
        ProjectTreeItem *nextItem=dynamic_cast<ProjectTreeItem *>(currentItem)->getNextItem();
        //如果说当前播放的是最后一张图片
        if(nextItem==nullptr)
        {
            Stop(); //停止动画
            update(); //刷新页面
            return;
        }
        setPixmap(nextItem);  //播放下一张图片
        update();
        return;
    }
    update();
}
