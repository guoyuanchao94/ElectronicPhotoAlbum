#ifndef PREVIEWLISTWIDGET_H
#define PREVIEWLISTWIDGET_H

#include <QListWidget>
#include <QObject>
#include <QTreeWidgetItem>
class PreviewListWidget : public QListWidget
{
    Q_OBJECT
public:
    PreviewListWidget(QWidget *parent = nullptr);
    ~PreviewListWidget();
private:
    void addItemToList(const QString &path);
private:
    QMap<QString,QListWidgetItem *> mapItems;
    int globalCount;
    int lastIndex;
    QPoint originPoint;
public slots:
    void slotUpdatePreview(QTreeWidgetItem *item);
    void slotUpdateSelected(QTreeWidgetItem *item);
    void slotItemPressed(QListWidgetItem *item);
signals:
    void signalUpdateSelectedShow(QString path); //传递路径便于显示
};

#endif // PREVIEWLISTWIDGET_H
