#include "picturebutton.h"

PictureButton::PictureButton(QWidget *parent)
    :QPushButton(parent)
{

}

void PictureButton::setIcons(const QString &normal, const QString &hover, const QString &pressed)
{
    //成员变量保存图片路径信息
    normalPath=normal;
    hoverPath=hover;
    pressedPath=pressed;
    QPixmap tempPixmap; //临时图片
    tempPixmap.load(normal);
    this->resize(tempPixmap.size()); //按钮扩展成图片大小
    this->setIcon(tempPixmap);
    this->setIconSize(tempPixmap.size()); //保证icon显示和原图大小一样
    //消除按钮边框设置在qss文件中
}

bool PictureButton::event(QEvent *event)
{
    switch(event->type())
    {

        case QEvent::Enter:
            setHoverIcon();
            break;
        case QEvent::Leave:
            setNormalIcon();
            break;
        case QEvent::MouseButtonPress:
            setPressedIcon();
            break;
            //按钮释放时鼠标光标依旧悬浮在按钮上
        case QEvent::MouseButtonRelease:
            setHoverIcon();
            break;
        default:
            break;
    }
    return QPushButton::event(event);
}

void PictureButton::setNormalIcon()
{
    QPixmap tempPixmap;
    tempPixmap.load(normalPath);
    this->setIcon(tempPixmap);
}

void PictureButton::setHoverIcon()
{
    QPixmap tempPixmap;
    tempPixmap.load(hoverPath);
    this->setIcon(tempPixmap);
}

void PictureButton::setPressedIcon()
{
    QPixmap tempPixmap;
    tempPixmap.load(pressedPath);
    this->setIcon(tempPixmap);
}
