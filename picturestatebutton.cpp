#include "picturestatebutton.h"
#include "const.h"
PictureStateButton::PictureStateButton(QWidget *parent)
    :QPushButton(parent)
{

}

PictureStateButton::~PictureStateButton()
{

}

void PictureStateButton::setIcons(const QString &normalPlay, const QString &hoverPlay,
                                  const QString &pressedPlay,const QString &normalPause,
                                  const QString &hoverPause, const QString &pressedPause)
{
    normalPlayPath=normalPlay;
    hoverPlayPath=hoverPlay;
    pressedPlayPath=pressedPlay;
    normalPausePath=normalPause;
    hoverPausePath=hoverPause;
    pressedPausePath=pressedPause;
    QPixmap tempPixmap;
    tempPixmap.load(normalPlay);
    this->resize(tempPixmap.size());
    this->setIcon(tempPixmap);
    this->setIconSize(tempPixmap.size());

    currentState=NormalPlayState;
}

bool PictureStateButton::event(QEvent *event)
{
    switch(event->type())
    {
        case QEvent::Enter:
            //这里枚举值的值,如果当前是正常播放状态
            if(currentState < NormalPauseState)
            {
                setPlayHoverIcon();
            }
            else
            {
                setPauseHoverIcon();
            }
            break;
        case QEvent::Leave:
            if(currentState < NormalPauseState)
            {
                setPlayNormalIcon();
            }
            else
            {
                setPauseNormalIcon();
            }
            break;

        case QEvent::MouseButtonPress:
            if(currentState < NormalPauseState)
            {
                setPlayPressedIcon();
            }
            else
            {
                setPausePressedIcon();
            }
            break;
            //播放变暂停,暂停变播放
            //鼠标释放时更换图标
        case QEvent::MouseButtonRelease:
            if(currentState < NormalPauseState)
            {
                setPauseHoverIcon();
            }
            else
            {
                setPlayHoverIcon();
            }
            break;
        default:
            break;
    }
    return QPushButton::event(event);
}

void PictureStateButton::setPlayNormalIcon()
{
    QPixmap tempPixmap;
    tempPixmap.load(normalPlayPath);
    this->setIcon(tempPixmap);
    currentState=NormalPlayState;
}

void PictureStateButton::setPlayHoverIcon()
{
    QPixmap tempPixmap;
    tempPixmap.load(hoverPlayPath);
    this->setIcon(tempPixmap);
    currentState=HoverPlayState;
}

void PictureStateButton::setPlayPressedIcon()
{
    QPixmap tempPixmap;
    tempPixmap.load(pressedPlayPath);
    this->setIcon(tempPixmap);
    currentState=PressedPlayState;
}

void PictureStateButton::setPauseNormalIcon()
{
    QPixmap tempPixmap;
    tempPixmap.load(normalPausePath);
    this->setIcon(tempPixmap);
    currentState=NormalPauseState;
}

void PictureStateButton::setPauseHoverIcon()
{
    QPixmap tempPixmap;
    tempPixmap.load(hoverPausePath);
    this->setIcon(tempPixmap);
    currentState=HoverPauseState;
}

void PictureStateButton::setPausePressedIcon()
{
    QPixmap tempPixmap;
    tempPixmap.load(pressedPausePath);
    this->setIcon(tempPixmap);
    currentState=NormalPauseState;
}

void PictureStateButton::slotStart()
{
    setPauseNormalIcon();
}

void PictureStateButton::slotStop()
{
    setPlayNormalIcon();
}

