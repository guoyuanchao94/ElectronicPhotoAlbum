#ifndef PROJECTTREE_H
#define PROJECTTREE_H

#include <QDialog>
#include <QTreeWidget>
namespace Ui {
class ProjectTree;
}

class ProjectTree : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectTree(QWidget *parent = nullptr);
    ~ProjectTree();
public:
    QTreeWidget *getTreeWidget();
private:
    Ui::ProjectTree *ui;
public slots:
    void addProjectToTree(const QString name,const QString path);
};

#endif // PROJECTTREE_H
