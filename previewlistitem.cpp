#include "previewlistitem.h"


PreviewListItem::PreviewListItem(const QIcon &icon, const QString &text, const int &index, QListWidget *listview, int type)
    :QListWidgetItem(icon,"",listview,type),myText(text),myIndex(index)
{

}

QString PreviewListItem::getText()
{
    return myText;
}

int PreviewListItem::getIndex()
{
    return myIndex;
}
