#ifndef PICTUREBUTTON_H
#define PICTUREBUTTON_H

#include <QPushButton>
#include <QObject>
#include <QPixmap>
#include <QEvent>
class PictureButton : public QPushButton
{
    Q_OBJECT
public:
    PictureButton(QWidget *parent = nullptr);
    void setIcons(const QString &normal,const QString &hover,const QString &pressed);
protected:
    bool event(QEvent *event) override;
private:
    void setNormalIcon();
    void setHoverIcon();
    void setPressedIcon();
    QString normalPath;
    QString hoverPath;
    QString pressedPath;
};

#endif // PICTUREBUTTON_H
