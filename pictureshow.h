#ifndef PICTURESHOW_H
#define PICTURESHOW_H

#include <QDialog>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QEvent>
namespace Ui {
class PictureShow;
}

class PictureShow : public QDialog
{
    Q_OBJECT

public:
    explicit PictureShow(QWidget *parent = nullptr);
    ~PictureShow();
public:
    void reloadPicture();
protected:
    bool event(QEvent *event) override;
private:
    void showButtons(bool isShow);
private:
    Ui::PictureShow *ui;
private:
    //定义动画效果
    QPropertyAnimation *animationShowPrevious;
    QPropertyAnimation *animationShowNext;
    bool btnVisible; //按钮是否可见
    QString selectedPath; //存储选中的项的路径
    QPixmap myPixmap; //存储加载的图片
public slots:
    void slotSelectedItem(const QString &path);
    void slotUpdatePicture(const QString &path);
    void slotClearPicture();
signals:
    void signalPreviousClicked();
    void signalNextClicked();
};

#endif // PICTURESHOW_H
