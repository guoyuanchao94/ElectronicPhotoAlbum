#include "slideshowdialog.h"
#include "ui_slideshowdialog.h"
#include "projecttreewidget.h"
SlideShowDialog::SlideShowDialog(QTreeWidgetItem *firstItem, QTreeWidgetItem *lastItem,QWidget *parent)
    :QDialog(parent),ui(new Ui::SlideShowDialog),
    myFirstItem(firstItem),myLastItem(lastItem)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    ui->btnSlidePrevious->setIcons(":/resource/icon/previous.png",":/resource/icon/previous_hover.png",":/resource/icon/previous_press.png");
    ui->btnSlideNext->setIcons(":/resource/icon/next.png",":/resource/icon/next_hover.png",":/resource/icon/next_press.png");

    ui->btnPlay->setIcons(":/resource/icon/play.png",":/resource/icon/play_hover.png",":/resource/icon/play_press.png",
                          ":/resource/icon/pause.png",":/resource/icon/pause_hover.png",":/resource/icon/pause_press.png");

    ui->btnClose->setIcons(":/resource/icon/closeshow.png",":/resource/icon/closeshow_hover.png",":/resource/icon/closeshow_press.png");

    //关闭按钮
    connect(ui->btnClose,&PictureButton::clicked,this,&SlideShowDialog::close);
    //连接轮播图播放页面,前进和后退按钮
    connect(ui->btnSlidePrevious,&PictureButton::clicked,this,&SlideShowDialog::slotSlidePrevious);
    connect(ui->btnSlideNext,&PictureButton::clicked,this,&SlideShowDialog::slotSlideNext);

    // 可以实现上面动画播放时将图像的预览图添加到列表中的效果
    connect(ui->pictureAnimation,&PictureAnimationWidget::signalUpdatePreview,ui->previewListWidget,&PreviewListWidget::slotUpdatePreview);
    // 可以实现上面动画播放时根据图像显示预览图选中效果
    connect(ui->pictureAnimation,&PictureAnimationWidget::signalSelectedItem,ui->previewListWidget,&PreviewListWidget::slotUpdateSelected);
    // 可以实现点击下方预览图，上方显示对应的动画效果
    connect(ui->previewListWidget,&PreviewListWidget::signalUpdateSelectedShow,ui->pictureAnimation,&PictureAnimationWidget::slotUpdateSelectedShow);

    connect(ui->previewListWidget,&PreviewListWidget::itemPressed,ui->pictureAnimation,&PictureAnimationWidget::Stop);
    connect(ui->btnPlay,&PictureStateButton::clicked,ui->pictureAnimation,&PictureAnimationWidget::slotStartOrPause);
    connect(ui->pictureAnimation,&PictureAnimationWidget::signalStatr,ui->btnPlay,&PictureStateButton::slotStart);
    connect(ui->pictureAnimation,&PictureAnimationWidget::signalStop,ui->btnPlay,&PictureStateButton::slotStop);

    // slideshow的父节点就是projecttree
    ProjectTreeWidget *projectWidget=dynamic_cast<ProjectTreeWidget *>(parent);
    connect(ui->pictureAnimation,&PictureAnimationWidget::signalStartMusic,projectWidget,&ProjectTreeWidget::slotStartMusic);
    connect(ui->pictureAnimation,&PictureAnimationWidget::signalStopMusic,projectWidget,&ProjectTreeWidget::slotStopMusic);
    //setPixmap这个函数已经发射了信号,所以连接在前
    ui->pictureAnimation->setPixmap(firstItem);
    ui->pictureAnimation->Start();
}


SlideShowDialog::~SlideShowDialog()
{
    delete ui;
}

void SlideShowDialog::slotSlideNext()
{
    ui->pictureAnimation->slideNext();
}

void SlideShowDialog::slotSlidePrevious()
{
    ui->pictureAnimation->slidePrevious();
}
