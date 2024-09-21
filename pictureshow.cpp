#include "pictureshow.h"
#include "ui_pictureshow.h"

PictureShow::PictureShow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PictureShow),btnVisible(false),selectedPath("")
{
    ui->setupUi(this);
    //前进按钮和后退按钮分别设置路径信息
    ui->btnPrevious->setIcons(":/resource/icon/previous.png",":/resource/icon/previous_hover.png",":/resource/icon/previous_press.png");
    ui->btnNext->setIcons(":/resource/icon/next.png",":/resource/icon/next_hover.png",":/resource/icon/next_press.png");

    //设置透明度为0,加载时为透明状态,看不到按钮的存在
    QGraphicsOpacityEffect *opacityPrevious= new QGraphicsOpacityEffect(this);
    opacityPrevious->setOpacity(0);
    ui->btnPrevious->setGraphicsEffect(opacityPrevious);

    QGraphicsOpacityEffect *opacityNext= new QGraphicsOpacityEffect(this);
    opacityNext->setOpacity(0);
    ui->btnNext->setGraphicsEffect(opacityNext);

    //设置动画效果
    animationShowPrevious=new QPropertyAnimation(opacityPrevious,"opacity",this);
    animationShowPrevious->setEasingCurve(QEasingCurve::Linear); //线性
    animationShowPrevious->setDuration(500); //500ms完成动画
    animationShowNext=new QPropertyAnimation(opacityNext,"opacity",this);
    animationShowNext->setEasingCurve(QEasingCurve::Linear); //线性
    animationShowNext->setDuration(500); //500ms完成动画

    connect(ui->btnPrevious,&PictureButton::clicked,this,&PictureShow::signalPreviousClicked);
    connect(ui->btnNext,&PictureButton::clicked,this,&PictureShow::signalNextClicked);
}

PictureShow::~PictureShow()
{
    delete ui;
}

void PictureShow::reloadPicture()
{
    if(selectedPath=="")
    {
        return;
    }
    const int &width=ui->gridLayout->geometry().width();
    const int &height=ui->gridLayout->geometry().height();
    myPixmap.load(selectedPath);
    myPixmap=myPixmap.scaled(width,height,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui->labPicture->setPixmap(myPixmap);
}

void PictureShow::slotSelectedItem(const QString &path)
{
    selectedPath=path; //记录选中图片路径
    myPixmap.load(path); //加载路径图片
    int pictureWidth=this->width()-20;
    int pictureHeight=this->height()-20;
    myPixmap=myPixmap.scaled(pictureWidth,pictureHeight,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui->labPicture->setPixmap(myPixmap);
}

void PictureShow::slotUpdatePicture(const QString &path)
{
    selectedPath=path; //记录选中图片路径
    if(selectedPath=="")
    {
        return;
    }
    int width=ui->gridLayout->geometry().width();
    int height=ui->gridLayout->geometry().height();
    myPixmap.load(selectedPath);
    myPixmap=myPixmap.scaled(width,height,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui->labPicture->setPixmap(myPixmap);
}

void PictureShow::slotClearPicture()
{
    selectedPath="";
    ui->labPicture->clear();
}

bool PictureShow::event(QEvent *event)
{
    switch(event->type())
    {
        case QEvent::Enter:
            showButtons(true);
            break;
        case QEvent::Leave:
            showButtons(false);
            break;
        default:
            break;
    }
    return QDialog::event(event);
}

void PictureShow::showButtons(bool isShow)
{
    //isShow代表是否显示按钮,如果isShow=false
    // 表示不显示按钮,并且按钮目前可见,就隐藏
    //要隐藏按钮,按钮目前是可见的
    if(!isShow && btnVisible)
    {
        animationShowPrevious->stop(); //初始暂停动画
        animationShowPrevious->setStartValue(1); //开始值,也就是设置的透明度的值
        animationShowPrevious->setEndValue(0); //停止值
        animationShowPrevious->start();

        animationShowNext->stop();
        animationShowNext->setStartValue(1);
        animationShowNext->setEndValue(0);
        animationShowNext->start();

        btnVisible=false; //按钮不可见
        return;
    }
    if(isShow && !btnVisible)
    {
        animationShowPrevious->stop();
        animationShowPrevious->setStartValue(0);
        animationShowPrevious->setEndValue(1);
        animationShowPrevious->start();

        animationShowNext->stop();
        animationShowNext->setStartValue(0);
        animationShowNext->setEndValue(1);
        animationShowNext->start();

        btnVisible=true; //按钮可见
        return;
    }
}
