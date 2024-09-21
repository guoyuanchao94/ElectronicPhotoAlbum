#ifndef PICTUREANIMATIONWIDGET_H
#define PICTUREANIMATIONWIDGET_H

#include <QObject>
#include <QWidget>
#include <QTreeWidgetItem>
#include <QTimer>
#include <QMap>

//幻灯片播放动画类
class PictureAnimationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PictureAnimationWidget(QWidget *parent = nullptr);
    ~PictureAnimationWidget();
public:
    void setPixmap(QTreeWidgetItem *item);
    void Start();
    void Stop();
    void slidePrevious();
    void slideNext();
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    void updateSelected(QTreeWidgetItem *);
private:
    float myFactor; //播放因子(0-1.0)
    QTreeWidgetItem *currentItem; //记录当前播放图片
    bool isStart; //是否开始播放
    QTimer *timer;
    QMap<QString,QTreeWidgetItem*> mapItems; //记录当前播放过的图片的信息
    QPixmap pixmapFirst;
    QPixmap pixmapSecond;
signals:
    void signalUpdatePreview(QTreeWidgetItem *); //预览列表更新信号
    void signalSelectedItem(QTreeWidgetItem *);
    void signalStatr(); //更新按钮使用
    void signalStop();
    void signalStartMusic();
    void signalStopMusic();
public slots:
    void slotUpdateSelectedShow(QString path);
    void slotStartOrPause();
private slots:
    void slotTimeOut();
};

#endif // PICTUREANIMATIONWIDGET_H
