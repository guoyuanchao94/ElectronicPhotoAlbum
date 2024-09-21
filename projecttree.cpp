#include "projecttree.h"
#include "ui_projecttree.h"

ProjectTree::ProjectTree(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProjectTree)
{
    ui->setupUi(this);
    this->setMinimumWidth(330);
    this->setMaximumWidth(330);
}

ProjectTree::~ProjectTree()
{
    delete ui;
}

QTreeWidget *ProjectTree::getTreeWidget()
{
    return ui->treeWidget;
}

void ProjectTree::addProjectToTree(const QString name, const QString path)
{
    //调用ui->treeWidget的同名函数
    //name和path是向导完成界面通过信号传递过来的
    ui->treeWidget->addProjectToTree(name,path);
}
