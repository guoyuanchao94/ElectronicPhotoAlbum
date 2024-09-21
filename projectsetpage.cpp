#include "projectsetpage.h"
#include "ui_projectsetpage.h"
#include <QLineEdit>
#include <QDir>
#include <QFileDialog>
ProjectSetPage::ProjectSetPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ProjectSetPage)
{
    ui->setupUi(this);
    registerField("projectName*",ui->lineEditName); //名称为空,下一步按钮不可用
    registerField("projectPath",ui->lineEditPath);
    //当文本框编辑的时候通知向导界面,注册的域发生变化
    //让向导界面判断界面是否完成
    connect(ui->lineEditName,&QLineEdit::textEdited,this,&ProjectSetPage::completeChanged);
    connect(ui->lineEditPath,&QLineEdit::textEdited,this,&ProjectSetPage::completeChanged);
    QString currentPath=QDir::currentPath();
    ui->lineEditPath->setText(currentPath); //填充当前路径
    ui->lineEditPath->setCursorPosition(ui->lineEditPath->text().size()); //光标在文本最后
    ui->lineEditPath->setClearButtonEnabled(true); //有输入直接全选可以删除
}

ProjectSetPage::~ProjectSetPage()
{
    delete ui;
}

bool ProjectSetPage::isComplete() const
{
    //判断两个文本框是否为空,为空不允许向下进行,注册域已经避免了这个问题
    if(ui->lineEditName->text().isEmpty() || ui->lineEditPath->text().isEmpty())
    {
        return false;
    }
    QDir dir(ui->lineEditPath->text());
    if(!dir.exists())
    {
        ui->labTips->setText("项目路径不存在");
        return false;
    }
    //构建绝对路径,不管枯井是否存在
    QString absolutePath=dir.absoluteFilePath(ui->lineEditName->text());
    QDir mayPath(absolutePath);
    if(mayPath.exists())
    {
        ui->labTips->setText("项目路径已经存在"); //不能再创建相同的目录
        return false;
    }
    ui->labTips->setText("");
    return QWizardPage::isComplete();
}

bool ProjectSetPage::validatePage()
{
    return QWizardPage::validatePage();
}

//返回向导页面的文件名和路径,便于后续添加到目录树
void ProjectSetPage::getProjectSetting(QString &name, QString &path)
{
    name=ui->lineEditName->text();
    path=ui->lineEditPath->text();
}

void ProjectSetPage::on_btnBrowser_clicked()
{
    QFileDialog fileDialog(this);
    // fileDialog.setOption(QFileDialog::DontUseNativeDialog,true); //不使用系统自带的文件对话框窗口
    fileDialog.setFileMode(QFileDialog::Directory);
    fileDialog.setViewMode(QFileDialog::Detail);
    fileDialog.setWindowTitle(tr("选择导入的文件夹"));
    QString currentPath=QDir::currentPath();
    fileDialog.setDirectory(currentPath);
    QStringList fileNames; //记录打开的文件夹的路径
    if(fileDialog.exec())
    {
        fileNames=fileDialog.selectedFiles(); //打开的是文件夹,所以记录的是文件夹的路径
    }
    if(fileNames.size()<=0)
    {
        qDebug()<<"没有获取任何文件";
        return;
    }
    QString importPath=fileNames.at(0);
    qDebug()<<"向导界面导入的文件夹的路径: "<<importPath;
    ui->lineEditPath->setText(importPath);
}

