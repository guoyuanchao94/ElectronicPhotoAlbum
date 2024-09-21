#ifndef SLIDESHOWDIALOG_H
#define SLIDESHOWDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>
namespace Ui {
class SlideShowDialog;
}

class SlideShowDialog : public QDialog
{
    Q_OBJECT

public:
    SlideShowDialog(QTreeWidgetItem *firstItem,QTreeWidgetItem *lastItem,QWidget *parent = nullptr);
    ~SlideShowDialog();
private:
    Ui::SlideShowDialog *ui;
private:
    QTreeWidgetItem *myFirstItem; //记录第一个孩子节点
    QTreeWidgetItem *myLastItem; //记录最后一个孩子节点,用于幻灯片播放结束
public slots:
    //按钮点击的槽函数
    void slotSlideNext();
    void slotSlidePrevious();
};

#endif // SLIDESHOWDIALOG_H
