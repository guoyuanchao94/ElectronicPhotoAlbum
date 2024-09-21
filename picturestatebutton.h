#ifndef PICTURESTATEBUTTON_H
#define PICTURESTATEBUTTON_H

#include <QPushButton>
#include <QObject>
#include <QEvent>
class PictureStateButton : public QPushButton
{
    Q_OBJECT
public:
    PictureStateButton(QWidget *parent = nullptr);
    ~PictureStateButton();
public:
    void setIcons(const QString &normalPlay,const QString &hoverPlay,const QString &pressedPlay,
                  const QString &normalPause,const QString &hoverPause,const QString &pressedPause);
protected:
    bool event(QEvent *event) override;
private:
    void setPlayNormalIcon();
    void setPlayHoverIcon();
    void setPlayPressedIcon();

    void setPauseNormalIcon();
    void setPauseHoverIcon();
    void setPausePressedIcon();
private:
    QString normalPlayPath;
    QString hoverPlayPath;
    QString pressedPlayPath;
    QString normalPausePath;
    QString hoverPausePath;
    QString pressedPausePath;
    int currentState; //记录当前状态
public slots:
    void slotStart();
    void slotStop();
};

#endif // PICTURESTATEBUTTON_H
