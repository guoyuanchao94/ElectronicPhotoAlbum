#ifndef PREVIEWLISTITEM_H
#define PREVIEWLISTITEM_H

#include <QListWidgetItem>
#include <QObject>

class PreviewListItem : public QListWidgetItem
{
public:
    PreviewListItem(const QIcon &icon,const QString &text, const int &index,QListWidget *listview = nullptr, int type = Type);
public:
    QString getText();
    int getIndex();
private:
    QString myText; //代表文件路径
    int myIndex; //在listwidget中的索引
};

#endif // PREVIEWLISTITEM_H
